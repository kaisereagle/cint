/*****************************************************************************
* EasyExp.cxx
*
* Description: Proprietary Regular Expression library associated with open 
*              source C++ interpreter CINT
*
*  Original library was named RegExp which had name conflict with standard 
* regular expression library.
*
* 2008/July/9 Masaharu Goto
*
*****************************************************************************/
/************************************************************************
 * Copyright(c) 1995~2015  Masaharu Goto (cint@pcroot.cern.ch) (gotom@hanno.jp)
 *
 * Permission to use, copy, modify and distribute this software and its 
 * documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation. The author makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 ************************************************************************/

#include "EasyExp.h"
#include "ReadF.h"
#ifdef __linux__
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#endif
#ifdef __APPLE__
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#endif
#include <algorithm>
using namespace std;

#ifndef WIN32
// For MacOS and Linux, common string search for EasyExp Synthesis does not
// work with multi-thread.
// Need to check if same problem occurs for match/unmatch multi-threading.
// It uses vector<>::push_back().
#define EASYEXP_SYNTHESIS_NO_MULTITHREAD

// Critical section did not resolve multithread segv
//#define CRITICALSECTION_PROTECTION
#endif

#ifdef EASYEXP_SYNTHESIS
#include "Stat.h"
#endif
#include <limits.h>

#define SYNTH_NUMBERRANGE

#define EMPTYSTRING_REWINDING

#define SYNTH_ITERATE

int EasyExp_DEBUG=0;

//////////////////////////////////////////////////////////////////////////
string toupper(const string& a) {
  string r;
  for(unsigned int i=0;i<a.size();i++)  r += toupper(a[i]);
  return(r);
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// EasyExp class
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
EasyExp::EasyExp(const EasyExp& a) {
#if 1
  if(a.m_replacepattern=="") compile(a.m_pattern);
  else compile_replace(a.m_pattern,a.m_replacepattern);
#else
  unsigned int i;
  for(i=0;i<a.m_element.size();i++) 
    m_element.push_back( a.m_element[i]->duplicate() );
  for(i=0;i<a.m_replace.size();i++) 
    m_replace.push_back( a.m_replace[i]->duplicate() );
  argv = a.argv;
  argi = a.argi;
#endif
}

//////////////////////////////////////////////////////////////////////////
EasyExp::EasyExp(const char* pattern,Mode mode) {
  compile(pattern,mode);
}
EasyExp::EasyExp(const string& pattern,Mode mode) {
  compile(pattern,mode);
}

//////////////////////////////////////////////////////////////////////////
EasyExp::EasyExp(const char* pattern,const char* replace,Mode mode) {
  compile_replace(pattern,replace,mode);
}
EasyExp::EasyExp(const string& pattern,const string& replace,Mode mode) {
  compile_replace(pattern,replace,mode);
}

//////////////////////////////////////////////////////////////////////////
EasyExp::~EasyExp() {
  clear_replace();
  EasyExp::clear();
}

//////////////////////////////////////////////////////////////////////////
void EasyExp::clear(int del) {
  deque<MatchElement*>::iterator first=m_element.begin();
  deque<MatchElement*>::iterator last =m_element.end();
  while(first!=last) {
    if(del || first!=m_element.begin()) delete (*first);
    ++first;
  }
  m_element.clear();
#ifdef E__PATTERN_MODIFICATION
  m_pattern = "";
  m_replacepattern = "";
#endif
  //clear_replace();
  //argv.clear();
  //argi.clear();
}

//////////////////////////////////////////////////////////////////////////
void disp_arglist(const vector<string>& argv,const vector<int>& argi) {
  if(EasyExp_DEBUG>2) {
    for(unsigned int k=0;k<argv.size();k++) 
      printf("%d:%s:%d ",k,argv[k].c_str(),argi[k]);
    printf("\n");
  }
}

//////////////////////////////////////////////////////////////////////////
//Simple case
// EasyExp   AA(x|y)BB*CC
// Compiled  0:string,1:stringlist,2:string,3:starstring,4:end
// char* s   AAxxxxxBByCC
//           ^-^----^-^--^
//           0 1    2 3  4
//Case 2
// EasyExp   AA(x|y)BB*(|CC)
// Compiled  0:string,1:stringlist,2:string,3:starstringlist,4:end
// char* s   AAxxxxxBByCC
//           ^-^----^-^--^
//           0 1    2 3  4
//////////////////////////////////////////////////////////////////////////
bool EasyExp::match(const char* s,bool icase)  {
  argv.clear();
  argi.clear();
  int rewound=0; //infinit loop checker
  if(!s) return(0);
  const char *p=s; // scanning position
  const char* prev_p;
  deque<MatchElement*>::const_iterator first=m_element.begin();
  deque<MatchElement*>::const_iterator last =m_element.end();
  deque<MatchElement*>::const_iterator star = last;
  if(EasyExp_DEBUG>1) printf("AAA %s #############\n",s);
  int argpos=0, argstar=0, argbackflag=0;
#ifdef EMPTYSTRING_REWINDING
  int posbackflag=0;
  const char* empty_p=0;
#endif
  while(first!=last && p) {
    prev_p = p; // store current position
    p = (*first)->match(p,argv,argi,icase); // search pattern and move to next position

    if(EasyExp_DEBUG>1) {
      (*first)->disp(); printf("BBB p=%p ",p); 
      if(p) printf("%s\n",p); else printf("\n");
    }

    if(argbackflag && argv.size()>=3) { 
      // * Rewind process 3, concatinate argv[size-3] & argv[size-2]
      //   in  argv[size-3]                 argv[size-2]     argv[size-1]     last
      //   out argv[size-3]&argv[size-2]    argv[size-1]     last
      argv[argv.size()-3] += argv[argv.size()-2];  argi[argi.size()-3]=0;
      argv[argv.size()-2]  = argv[argv.size()-1];  argi[argi.size()-2]=argi[argi.size()-1];
      argv.pop_back();                             argi.pop_back();
      argbackflag=0;
      if(EasyExp_DEBUG>1) printf("argback 3 \n");
      if(EasyExp_DEBUG>2) disp_arglist(argv,argi);
    }

    if(!p) {
      if(++rewound>strlen(s)) {
	// Infinite loop, REVIEW NEEDED
	argv.clear(); argi.clear();
	if(EasyExp_DEBUG>1) printf("false 3\n");
	return(false);
      }
      if(star==last) { // normal unmatch
	// element unmatch and no rewinding position -> unmatch
	argv.clear(); argi.clear();
	if(EasyExp_DEBUG>1) printf("false 1\n");
	return(false);
      }
      else { // rewinding
#ifdef EMPTYSTRING_REWINDING
	if(empty_p) {
	  first=star; // match element rewinding 
	  p = empty_p; // return to empty string matching position
	  star=last; // reset rewinding buffer
	  argv.pop_back(); argi.pop_back(); // discard previous argument
	}
	else {
#endif
	// * Rewind process 2
	// match failed for *string, rewind match element position for alternative matching
        first=star; // match element rewinding
        p = prev_p; // previous position, not to the star position
        star=last;  // reset rewinding buffer
        string buf;
	// reorganize argument buffer
        for(unsigned int j=argstar;j<argv.size();j++) buf += argv[j];
        argpos = argstar;
        argv.resize(argpos); argi.resize(argpos);
        argv.push_back(buf); argi.push_back(0);
        argbackflag = 1; // set flag for process 3
#ifdef EMPTYSTRING_REWINDING
	}
#endif
	if(EasyExp_DEBUG>1) printf("argback 2 argstar=%d '%s'\n",argstar,p);
	if(EasyExp_DEBUG>2) disp_arglist(argv,argi);
      }
    } // !p
    else { // if(p) 
      if(p==prev_p) { // zero length string match
	if(strcmp((*first)->what(),"MatchStarStringList")==0 &&
	   (first+1)!=last && strcmp((*(first+1))->what(),"MatchEnd")==0) {
	  //                           *stringlist with zero length string
	  // AAxxBB*(|.CSV)  AAxxBBanystr[END]  AAAxxBBanystr.CSV
	  //                       p                             p
	  if(rewound) argv.back() += p; 
	  else { argv.push_back(p); argi.push_back(0); }
	  if(EasyExp_DEBUG>1) printf("true 2\n");
	  if(EasyExp_DEBUG>2) disp_arglist(argv,argi);
	  return(true);
	}
	if(strcmp((*first)->what(),"MatchStarStringList")==0 &&
	   (first+1)!=last && (*(first+1))->hasemptystring() &&
	   (first+2)!=last && strcmp((*(first+2))->what(),"MatchEnd")==0) {
	  // consequitive zero-length string match + END
	  if(rewound) argv.back() += p; 
	  else { argv.push_back(p); argi.push_back(0); }
	  if(EasyExp_DEBUG>1) printf("true 3\n");
	  if(EasyExp_DEBUG>2) disp_arglist(argv,argi);
	  return(true);
	}
	// Above can happen N times, No support -> return unmatch
      } // p==prev_p
      if( (*first)->isstar() ) { // store position for xxAAxxAAxx
	// * Rewind process 1 
#ifdef EMPTYSTRING_REWINDING
	empty_p = 0;
#endif
        argstar=argpos; star=first; // store match element position
	if(EasyExp_DEBUG>1) printf("store star position argstar=%d\n",argstar);
	if(EasyExp_DEBUG>2) disp_arglist(argv,argi);
      }
#ifdef EMPTYSTRING_REWINDING
      else if(p!=prev_p && (*first)->hasemptystring() ) { // store position for (|_)_ 'xx_'
	// emptystring rewind process 1
	empty_p = prev_p;
	argstar=argpos; star=first+1;
	if(EasyExp_DEBUG>1) printf("store emptystring position argstar=%d\n",argstar);
	if(EasyExp_DEBUG>2) disp_arglist(argv,argi);
      }
#endif
      ++first; ++argpos;
      if(EasyExp_DEBUG>1) printf("next\n");
    }
  }
  if(EasyExp_DEBUG>1) printf("true 1\n");
  if(EasyExp_DEBUG>2) disp_arglist(argv,argi);
  return(true);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
#ifdef E__MULTIBYTE
char* EasyExp::strchr_multibyte(const char* s,int c) {
  char *a = (char*)s;
  while(*a) {
    if(IsDBCSLeadByte(*a)) ++a;
    else if(*a == c) return(a);  
    ++a;
  }
  return(0);
}
#endif
//////////////////////////////////////////////////////////////////////////
void EasyExp::compile(const char* pattern,Mode mode) {
  // clear Regular expression sequence
  clear();

#ifdef E__PATTERN_MODIFICATION
  m_pattern = pattern;
  m_replacepattern="";
#endif
//printf("mode=%d ",mode);
//EasyExp_DEBUG(pattern);

  // Auto mode judgement
  if(mode==Auto) {
#if defined(E__MULTIBYTE) || defined(E__MULTIBYTELIB)
    char *a = strchr_multibyte(pattern,'*');
    char *q = strchr_multibyte(pattern,'?');
    char *b = strchr_multibyte(pattern,'[');
    char *p = strchr_multibyte(pattern,'(');
#ifdef NUMBER_IN_BRACES
    char *m = strchr_multibyte(pattern,'{');
#else
    char *m = 0;
#endif
    if(a||q||b||p||m) mode=RegEx;
    else mode=Exact;
//printf("%d %d %d %d %d\n",a,q,b,p,m);
#else
    char *a = strchr(pattern,'*');
    char *q = strchr(pattern,'?');
    char *b = strchr(pattern,'[');
    if(a||q||b) mode=RegEx;
#endif
  }

  // in case of exact string match222
  if(mode==Exact) {
    if(strlen(pattern)) m_element.push_back(new MatchString(pattern));
    m_element.push_back(new MatchEnd());
    return;
  }
//printf("%s\n",pattern);
  // in case of regular expression matching
  const char *s = pattern;
  int len = strlen(pattern);
  int i,c;
  bool star=false;
  for(i=0;i<=len;i++) {
    c=s[i];
    switch(c) {
    case '*':
      star=true;
      break;
    case '?':
      m_element.push_back(new MatchAnyChar());
      //star=false;
      break;
    case '[':
      compile_braket(s,len,i,star);
      star=false;
      break;
    case '(':
      compile_parenthesis(s,len,i,star);
      star=false;
      break;
#ifdef NUMBER_IN_BRACES
    case '{':
      compile_brace(s,len,i,star);
      star=false;
      break;
#endif
    case 0:
      compile_end(star);
      star=false;
      return;
    default:
      if(compile_string(s,len,i,star)) 
      return;
      star=false;
      break;
    }
  }
//disp();
}
//////////////////////////////////////////////////////////////////////////
bool EasyExp::compile_string(const char* s,int len,int& i,bool star) {
  string buf;
  bool theend = getstring(buf,s,len,i);
  if(buf.size()) {
    if(star) 
      m_element.push_back(new MatchStarString(buf));
    else
      m_element.push_back(new MatchString(buf));
  }
  if(theend) {
    compile_end(false);
    return(true);
  }
  else {
    return(false);
  }
}
//////////////////////////////////////////////////////////////////////////
bool EasyExp::getstring(string& buf,const char* s,int len,int& i) {
  int c;
  while((c=s[i])) {
    switch(c) {
    case '[':
#ifdef NUMBER_IN_BRACES
    case '{':
#endif
    case '(':
    case '*':
    case '?':
      --i;
      return(false);
    case '\\':
      c=s[++i];
    default:
      buf+=(char)c;
#if defined(E__MULTIBYTE) || defined(E__MULTIBYTELIB)
      if(IsDBCSLeadByte(c)) {
        c=s[++i];
        buf+=(char)c;
        CheckDBCS2ndByte(c);
      }
#endif
//printf("A '%s'\n",buf.c_str());
      break;
    }
    ++i;
  }
  return(true);
}
//////////////////////////////////////////////////////////////////////////
bool EasyExp::compile_braket(const char* s,int len,int& i,bool star) {
  string charlist;
  int seq,seqend;
  bool negation = getcharlist(charlist,s,len,i,seq,seqend);
  if(charlist.size()) {
    if(star) 
      if(negation) m_element.push_back(new MatchStarNotCharList(charlist,seq,seqend));
      else    m_element.push_back(new MatchStarCharList(charlist,seq,seqend));
    else
      if(negation) m_element.push_back(new MatchNotCharList(charlist,seq,seqend));
      else    m_element.push_back(new MatchCharList(charlist,seq,seqend));
  }
  return(false);
}
//////////////////////////////////////////////////////////////////////////
bool EasyExp::getcharlist(string& charlist,const char* s,int len,int& i,int& seq,int& seqend) {
  int c;
  bool negation=false;
  int i1,i2;
  seq=1;
  seqend= -1;
  ++i;
  if(s[i]=='^') {
    negation=true; ++i;
  }
  else {
#if defined(E__MULTIBYTE) || defined(E__MULTIBYTELIB)
    c = s[i++];
    charlist+=c;
    if(IsDBCSLeadByte(c)) {
      c = s[i++];
      charlist+=c;
      CheckDBCS2ndByte(c);
    }
#else
    charlist+=s[i++];
#endif
  }
  while((c=s[i])) {
    switch(c) {
    case ']':
      return(negation);
      break;
    case ':':
      ++i;
      if(s[i]=='*') {seq = -1; seqend= -1;}  // [xyz:*]
      else if(isdigit(s[i])) { // [xyz:12]
        seq=0;
        while(isdigit(s[i])) {
          seq = seq*10 + s[i]-'0';
          ++i;
        }
#ifdef CHARLISTRANGE
	if(s[i]=='-') {
          seqend=0; ++i;
          while(isdigit(s[i])) {
            seqend = seqend*10 + s[i]-'0';
            ++i;
          }
          if(s[i]=='*') seqend= INT_MAX; 
	}
#endif
        --i;
      }
      break;
    case '-':
      //i1=s[i-1];
      i1=s[i-1]+1;
      i2=s[++i];
      for(c=i1;c<=i2;c++) charlist+=(char)c;
      break;
    case '\\':
      c=s[++i];
    default:
      charlist+=(char)c;
#if defined(E__MULTIBYTE) || defined(E__MULTIBYTELIB)
      if(IsDBCSLeadByte(c)) {
        c=s[++i];
        charlist+=(char)c;
        CheckDBCS2ndByte(c);
      }
#endif
      break;
    }
    ++i;
  }
  return(negation);
}
//////////////////////////////////////////////////////////////////////////
bool EasyExp::compile_parenthesis(const char* s,int len,int& i,bool star) {
  deque<string> strlist;
#ifdef NUMBER_IN_PARENTHESIS
  long min=0,max=0,padding;
  padding=getstringlist(strlist,s,len,i,&min,&max);
  if(padding<0 && strlist.size()) {
    if(star) 
      m_element.push_back(new MatchStarStringList(strlist));
    else
      m_element.push_back(new MatchStringList(strlist));
  }
  else if(padding>=0) {
    if(star) 
      m_element.push_back(new MatchStarNumberRange(min,max,padding));
    else
      m_element.push_back(new MatchNumberRange(min,max,padding));
  }
#else
  getstringlist(strlist,s,len,i);
  if(strlist.size()) {
    if(star) 
      m_element.push_back(new MatchStarStringList(strlist));
    else
      m_element.push_back(new MatchStringList(strlist));
  }
#endif
  return(false);
}
//////////////////////////////////////////////////////////////////////////
long EasyExp::getstringlist(deque<string>& strlist,const char* s,int len,int& i
                           ,long* pmin,long* pmax) {
  int c, flag=0;
  ++i;
  string buf;
  while((c=s[i])) {
    switch(c) {
    case ')':
      if(buf.size()) strlist.push_back(buf);
      else ++flag;
      if(flag) strlist.push_back("");
      return(-1);
      break;
    case '|':
      if(buf.size()) strlist.push_back(buf);
      else ++flag;
      buf = "";
      break;
#ifdef NUMBER_IN_PARENTHESIS
    case '.':
      if(strlist.size()==0 && i+2<len && s[i+1]=='.') {
        ++i; 
        if(buf.size()) *pmin = atoi(buf.c_str());
        long padding;
        if(buf[0]=='0') padding = buf.size();
        else            padding = 0;
        long padding2 = getnumberrange(pmin,pmax,s,len,i);
        if(padding2>0) padding=padding2;
        return(padding);
      }
      buf += s[i];
      break;
#endif
    case '\\':
      //++i;
      c=s[++i];
    default:
      //buf += s[i];
      buf += c;
#if defined(E__MULTIBYTE) || defined(E__MULTIBYTELIB)
      if(IsDBCSLeadByte(c)) {
        c=s[++i];
        buf+=(char)c;
        CheckDBCS2ndByte(c);
      }
#endif
      break;
    }
    ++i;
  }
  if(flag) strlist.push_back("");
  return(-1);
}
//////////////////////////////////////////////////////////////////////////
bool EasyExp::getstringlist(deque<string>& strlist,const char* s,int len,int& i) {
  int c;
  ++i;
  string buf;
  while((c=s[i])) {
    switch(c) {
    case ')':
      if(buf.size()) strlist.push_back(buf);
      return(false);
      break;
    case '|':
      if(buf.size()) strlist.push_back(buf);
      buf = "";
      break;
    case '\\':
      //++i;
      c=s[++i];
    default:
      //buf += s[i];
      buf += c;
#if defined(E__MULTIBYTE) || defined(E__MULTIBYTELIB)
      if(IsDBCSLeadByte(c)) {
        c=s[++i];
        buf+=(char)c;
        CheckDBCS2ndByte(c);
      }
#endif
      break;
    }
    ++i;
  }
  return(false);
}
//////////////////////////////////////////////////////////////////////////
bool EasyExp::compile_brace(const char* s,int len,int& i,bool star) {
  long min=0,max=0,padding;
  padding = getnumberrange(&min,&max,s,len,i);
  if(padding>=0) {
    if(star) 
      m_element.push_back(new MatchStarNumberRange(min,max,padding));
    else
      m_element.push_back(new MatchNumberRange(min,max,padding));
  }
  return(false);
}
//////////////////////////////////////////////////////////////////////////
long EasyExp::getnumberrange(long* pmin,long* pmax,const char* s,int len,int& i) {
  ++i;
  int c;
  long padding = -1;
  string buf;
  while((c=s[i])) {
    switch(c) {
    case 'L':
    case 'l':
    case 'S':
    case 's':
    case 'P':
    case 'p':
    case ':':
    case '|':
      if(buf.size()) *pmax = atoi(buf.c_str());
      buf="";
      while((c=s[++i])) {
        if(c=='}' || c==')') break;
        buf+=c;
      }
      padding = atoi(buf.c_str());
      return(padding);
      break;
#ifdef NUMBER_IN_BRACES
    case '}':
#endif
#ifdef NUMBER_IN_PARENTHESIS
    case ')':
#endif
      if(buf.size()) *pmax = atoi(buf.c_str());
      return(padding);
      break;
    case '-':
      if(buf[0]=='0') padding = buf.size();
      else            padding = 0;
      if(buf.size()) *pmin = atoi(buf.c_str());
      buf = "";
      break;
    case '\\':
      //++i;
      c=s[++i];
    default:
      //buf += s[i];
      buf += c;
#if defined(E__MULTIBYTE) || defined(E__MULTIBYTELIB)
      if(IsDBCSLeadByte(c)) {
        c=s[++i];
        buf+=(char)c;
        CheckDBCS2ndByte(c);
      }
#endif
      break;
    }
    ++i;
  }
  return(padding);
}
//////////////////////////////////////////////////////////////////////////
void EasyExp::compile_end(bool star) {
  if(star) m_element.push_back(new MatchStarEnd());
  else     m_element.push_back(new MatchEnd());
}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// String replacement
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void EasyExp::compile_replace(const char* r) {
  ReadF parsecomma,parseequal;
  parsecomma.setdelimiter(",");
  parsecomma.setseparator("");
  parsecomma.setquotation("'\"");

  parseequal.setdelimiter("=");
  parseequal.setseparator("");
  parseequal.setquotation("'\"");

  parsecomma.parse(r);
  int index;
  for(int i=1;i<=parsecomma.argc;i++) {
//printf("%d %s\n",i,parsecomma.argv[i]);
    parseequal.parse(parsecomma.argv[i]);
    if(parseequal.argc>=1 && strlen(parsecomma.argv[i])>0) {
      //printf("'%s' i=%d arg=%d\n",parsecomma.argv[i],i,parseequal.argc);
      if(parseequal.argv[1][0]=='$') index=atoi(parseequal.argv[1]+1);
      else                           index=atoi(parseequal.argv[1]);
      if(parseequal.argc>=2) compile_replace(index,parseequal.argv[2]);
      else                   compile_replace(index,"");
    }
  }
}
//////////////////////////////////////////////////////////////////////////
void EasyExp::compile_replace(int i,const char* r) {
  if(strlen(r)>0) compile(r);
  else {
    m_element.push_back(new MatchString(""));
    m_element.push_back(new MatchEnd());
  }
  if((unsigned int)i>=m_replace.size()) m_replace.resize(i+1);
  m_replace[i] = m_element[0];
  clear(0);
}
//////////////////////////////////////////////////////////////////////////
bool EasyExp::replace(const char *in,string& out,bool icase)  {
  bool m = match(in,icase);
  if(m) {
#ifdef E__SMARTREPLACE
    string tmp1,tmp2,tmp3;
    size_t p1,p2;
    int index;
#endif
    out = "";
    const char *p;
    for(unsigned int i=0;i<argv.size();i++) {
      // bug fix, 2015/Dec/24, m_replace.size()==1 when replace==""
      if(i<m_replace.size() && m_replace[i] &&
      //if(i<m_replace.size()-1 && m_replace[i] &&  
         (p=m_replace[i]->retrieve(argi[i]))) {
#ifdef E__SMARTREPLACE
	tmp1 = p; // abc$5$def, abc$5, $5
	p1 = tmp1.find_first_of("$");
	if(p1!=string::npos) {
	  tmp2 = tmp1.substr(0,p1); // abc,   abc,  ""
	  tmp3 = tmp1.substr(p1+1); // 5$def, 5  ,  5
	  p2 = tmp3.find_first_of("$");
	  if(p2!=string::npos) {
	    tmp1 = tmp3.substr(p2+1); // def
	    tmp3 = tmp3.substr(0,p2); // 5
	    index = atoi(tmp3.c_str());
	    out += tmp2;           // abc
	    out += argv[index];    // $5 contents
	    out += tmp1;           // def
          }
	  else {
	    index = atoi(tmp3.c_str());
	    out += tmp2;           // abc
	    out += argv[index];    // $5 contents
          }
	}
	else out += p;
#else
        out += p;
#endif
      }
      else {
        out += argv[i];
      }
    }
  }
  else {
    out = in;
  }
  return(m);
}
//////////////////////////////////////////////////////////////////////////
void EasyExp::clear_replace() {
  deque<MatchElement*>::iterator first=m_replace.begin();
  deque<MatchElement*>::iterator last =m_replace.end();
  while(first!=last) {
    delete (*first);
    ++first;
  }
  m_replace.clear();
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// End string replacement
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#ifdef E__PATTERN_MODIFICATION
//////////////////////////////////////////////////////////////////////////
// Pattern inversion
// '*A*' <- abcAdef  -->  'abc*def', replace("*A*","$1=");
// '*A'  <- abcA     -->  'abc*'   , replace("*A*","$1=");
// 'A*'  <- Adef     -->  '*def'   , replace("A*" ,"$0=");
// '*(A|B|C)*' <- abcAdef  ->  'abc*def'  or '*A*'  
// '*[ABC]*'   <- abcAdef  ->  'abc*def'  or '*A*'  
//////////////////////////////////////////////////////////////////////////
static void append(string& target,const char* s,int n,const char* r="=") {
  if(target.size()) target += ",";
  target += s;
  char buf[20]; sprintf(buf,"%d",n); target += buf;
  target += r;
}
//////////////////////////////////////////////////////////////////////////
bool EasyExp::pattern_inversion(const char *s,string& out,bool icase) { 
  // Note: For some reason, I could not use '*' directly as replacement char.
  //  Probably, $n=* is interpreted as $n=$n, but I don't recall where I 
  //  implemented it.  So, matching sub-pattern is once replaces as '\f'
  //  which is a safe character.
  bool m = match(s,icase);
  if(m) {
    string rep, exprtype;
    int j=0;
    for(unsigned int i=0;i<m_element.size();i++) {
      exprtype = m_element[i]->what();
      if(exprtype=="MatchString")                append(rep,"$",j,"=\f");
      else if(exprtype=="MatchStarString")       append(rep,"$",++j,"=\f"); 
      else if(exprtype=="MatchStringList")       append(rep,"$",j,"=\f");
      else if(exprtype=="MatchStarStringList")   append(rep,"$",++j,"=\f");
      else if(exprtype=="MatchCharList")         append(rep,"$",j,"=\f");
      else if(exprtype=="MatchStarCharList")     append(rep,"$",++j,"=\f");
      else if(exprtype=="MatchNotCharList")      append(rep,"$",j,"=\f");
      else if(exprtype=="MatchStarNotCharList")  append(rep,"$",++j,"=\f");
      else if(exprtype=="MatchNumberRange")      append(rep,"$",j,"=\f");
      else if(exprtype=="MatchStarNumberRange")  append(rep,"$",++j,"=\f");
      ++j;
    }
    EasyExp e; 
    e.compile_replace(m_pattern.c_str(),rep.c_str());
    e.replace(s,out,icase);
    size_t pos;
    while((pos=out.find_first_of('\f'))!=string::npos) out[pos] = '*';
  }
  else {
    out = s;
  }
  return(m);
}

//////////////////////////////////////////////////////////////////////////
// Generalize EasyExp pattern 
//   String List   -> Any String
//   Number Range  -> Any String
//   Char List     -> Any Char
//  But things are not this simple...
//////////////////////////////////////////////////////////////////////////
void EasyExp::pattern_generalization() {
  deque<MatchElement*> element;
  string exprtype;
  for(unsigned int i=0;i<m_element.size();i++) {
    exprtype = m_element[i]->what();
    if(exprtype=="MatchString" ||
       exprtype=="MatchStarString") {
      element.push_back(m_element[i]);
    }
    else if(exprtype=="MatchStringList" ||
	    exprtype=="MatchStarStringList" ||
	    exprtype=="MatchStarCharList" ||
	    exprtype=="MatchStarNotCharList" ||
	    exprtype=="MatchNumberRange" ||
	    exprtype=="MatchStarNumberRange") {
      while((exprtype=m_element[++i]->what())=="MatchStringList" ||
	    exprtype=="MatchStarStringList" ||
	    exprtype=="MatchNumberRange" ||
	    exprtype=="MatchStarNumberRange" || 
	    exprtype=="MatchStarCharList" ||
	    exprtype=="MatchStarNotCharList") {
	delete m_element[i];
      }
      if(exprtype=="MatchString") {
	element.push_back(new MatchStarString(m_element[i]->contents()));
	delete m_element[i];
      }
      else if(exprtype=="MatchEnd") {
	element.push_back(new MatchStarEnd());
	delete m_element[i];
      }
      else if(exprtype=="MatchStarEnd") {
	element.push_back(m_element[i]);
      }
    }

    else if(exprtype=="MatchAnyChar") {
      element.push_back(m_element[i]);
    }
    else if(exprtype=="MatchCharList" ||  
	    exprtype=="MatchNotCharList") {
      element.push_back(new MatchAnyChar);
      delete m_element[i];
    }
    else {
      element.push_back(m_element[i]);
    }
  }
  m_element = element;
}
//////////////////////////////////////////////////////////////////////////
// Pattern optimization
//////////////////////////////////////////////////////////////////////////
int EasyExp::pattern_optimization() {
  deque<MatchElement*> element;
  string exprtype;
  int index_anystring=0, index_anychar=0,index;
  for(unsigned int i=0;i<m_element.size();i++) {
    exprtype = m_element[i]->what();
    if(exprtype=="MatchAnyChar") {
      index_anystring=1;
      index_anychar=1;
    }
    else if(m_element[i]->hasemptystring()) {
      index_anystring=1;
      index_anychar=0;
    }
    else {
      if(index_anychar) {
	if(exprtype=="MatchString" ||
	   exprtype=="MatchStringList" ||
	   exprtype=="MatchStringList" ||
	   exprtype=="MatchCharList" ||
	   exprtype=="MatchNotCharList" ||
	   exprtype=="MatchNumberRange" ||
	   exprtype=="MatchEnd") {
	  element.push_back(new MatchAnyChar());
	  element.push_back(m_element[i]);
	}
	else if(exprtype=="MatchStarString" ||
		exprtype=="MatchStarStringList" ||
		exprtype=="MatchStarCharList" ||
		exprtype=="MatchStarNotCharList" ||
		exprtype=="MatchStarNumberRange" ||
		exprtype=="MatchStarEnd") {
	  element.push_back(m_element[i]);
	}
	else {
	  element.push_back(m_element[i]);
	}
      }
      else if(index_anystring) {
	if(exprtype=="MatchString") {
	  element.push_back(new MatchStarString(m_element[i]->contents()));
	  delete m_element[i];
	}
	else if(exprtype=="MatchStringList") {
	  deque<string> ds;
	  for(int j=0;j<ds.size();j++) ds.push_back(m_element[i]->contents(j));
	  element.push_back(new MatchStarStringList(ds));
	  delete m_element[i];
	}
	else if(exprtype=="MatchCharList") {
	  element.push_back(new MatchStarCharList(m_element[i]->contents(-1),
						  m_element[i]->parameter(0),
						  m_element[i]->parameter(1)));
	  delete m_element[i];
	}
	else if(exprtype=="MatchNotCharList") {
	  element.push_back(new MatchStarNotCharList(m_element[i]->contents()));
	  delete m_element[i];
	}
	else if(exprtype=="MatchNumberRange") {
	  element.push_back(new MatchStarNumberRange(m_element[i]->parameter(0),
						     m_element[i]->parameter(1),
						     m_element[i]->parameter(2)));
	  delete m_element[i];
	}
	else if(exprtype=="MatchEnd") {
	  element.push_back(new MatchStarEnd());
	  delete m_element[i];
	}
	else if(exprtype=="MatchStarString" ||
		exprtype=="MatchStarStringList" ||
		exprtype=="MatchStarCharList" ||
		exprtype=="MatchStarNotCharList" ||
		exprtype=="MatchStarNumberRange" ||
		exprtype=="MatchStarEnd") {
	  element.push_back(m_element[i]);
	}
	else {
	  element.push_back(m_element[i]);
	}
      }
      else {
	element.push_back(m_element[i]);
      }
      index_anystring = 0;
      index_anychar=0;
    }
  }
  m_element = element;
  return(1);
}

//////////////////////////////////////////////////////////////////////////
// Pattern splitting
//////////////////////////////////////////////////////////////////////////
unsigned int EasyExp::pattern_splitting_size() const {
  unsigned int count=0;
  string exprtype;
  for(unsigned int i=0;i<m_element.size();i++) {
    exprtype = m_element[i]->what();
    if(exprtype=="MatchStringList" ||
       exprtype=="MatchStarStringList" ||
       exprtype=="MatchCharList" ||
       exprtype=="MatchStarCharList") ++count;
  }
  return(count);
}
//////////////////////////////////////////////////////////////////////////
vector<EasyExp> EasyExp::pattern_splitting(int index) const {
  vector<EasyExp> ve;
  EasyExp e;
  unsigned int count=0;
  string exprtype;
  for(unsigned int i=0;i<m_element.size();i++) {
    exprtype = m_element[i]->what();
    if(exprtype=="MatchStringList" ||
       exprtype=="MatchStarStringList" ||
       exprtype=="MatchCharList" ||
       exprtype=="MatchStarCharList") {
      if(count++ ==index) {
	// TO BE IMPLEMENTED
      }
    }
  }
  return(ve);
}

//////////////////////////////////////////////////////////////////////////
// Back synthesize EasyExp string from compiled expression
//////////////////////////////////////////////////////////////////////////
string EasyExp::backannotation() const {
  return(backannotation(m_element));
}
//////////////////////////////////////////////////////////////////////////
string EasyExp::backannotation(const deque<MatchElement*>& element) const {
  char buf[100];
  string expr, exprtype;
  unsigned int j;
  for(unsigned int i=0;i<element.size();i++) {
    exprtype = element[i]->what();
    if(exprtype=="MatchString") expr += element[i]->contents();
    if(exprtype=="MatchStarString") {
      expr += "*";
      expr += element[i]->contents();
    }
    else if(exprtype=="MatchStringList") {
      for(j=0;j<element[i]->nlist();j++) {
	if(j==0) expr += "(";
	else     expr += "|";
	expr += element[i]->contents(j);
      }
      expr += ")";
    }
    else if(exprtype=="MatchStarStringList") {
      for(j=0;j<element[i]->nlist();j++) {
	if(j==0) expr += "*(";
	else     expr += "|";
	expr += element[i]->contents(j);
      }
      expr += ")";
    }
    else if(exprtype=="MatchCharList") {
      expr += "[";
      expr += element[i]->contents(-1);
      if(element[i]->parameter(0)>1 && element[i]->parameter(1)==-1) {
	sprintf(buf,":%ld",element[i]->parameter(0));
	expr += buf;
      }
      else if(element[i]->parameter(0)==-1 && element[i]->parameter(1)==-1) expr+=":*";
      else if(element[i]->parameter(0)>=0 && element[i]->parameter(1)>0) {
	sprintf(buf,":%ld-%ld",element[i]->parameter(0),element[i]->parameter(1));
	expr += buf;
      }
      expr += "]";
    }
    else if(exprtype=="MatchStarCharList") {
      expr += "*[";
      expr += element[i]->contents(-1);
      if(element[i]->parameter(0)>1 && element[i]->parameter(1)==-1) {
	sprintf(buf,":%ld",element[i]->parameter(0));
	expr += buf;
      }
      else if(element[i]->parameter(0)==-1 && element[i]->parameter(1)==-1) expr+=":*";
      else if(element[i]->parameter(0)>=0 && element[i]->parameter(1)>0) {
	sprintf(buf,":%ld-%ld",element[i]->parameter(0),element[i]->parameter(1));
	expr += buf;
      }
      expr += "]";
    }
    else if(exprtype=="MatchNotCharList") {
      expr += "[^";
      expr += element[i]->contents(-1);
      if(element[i]->parameter(0)>1 && element[i]->parameter(1)==-1) {
	sprintf(buf,":%ld",element[i]->parameter(0));
	expr += buf;
      }
      else if(element[i]->parameter(0)==-1 && element[i]->parameter(1)==-1) expr+=":*";
      else if(element[i]->parameter(0)>=0 && element[i]->parameter(1)>0) {
	sprintf(buf,":%ld-%ld",element[i]->parameter(0),element[i]->parameter(1));
	expr += buf;
      }
      expr += "]";
    }
    else if(exprtype=="MatchStarNotCharList") {
      expr += "*[^";
      expr += element[i]->contents(-1);
      if(element[i]->parameter(0)>1 && element[i]->parameter(1)==-1) {
	sprintf(buf,":%ld",element[i]->parameter(0));
	expr += buf;
      }
      else if(element[i]->parameter(0)==-1 && element[i]->parameter(1)==-1) expr+=":*";
      else if(element[i]->parameter(0)>=0 && element[i]->parameter(1)>0) {
	sprintf(buf,":%ld-%ld",element[i]->parameter(0),element[i]->parameter(1));
	expr += buf;
      }
      expr += "]";
    }
    else if(exprtype=="MatchNumberRange") {
      if(element[i]->parameter(2)<1)
	sprintf(buf,"(%ld..%ld)",element[i]->parameter(0),element[i]->parameter(1));
      else 
	sprintf(buf,"(%ld..%ld:%ld)",element[i]->parameter(0),element[i]->parameter(1)
		,element[i]->parameter(2));
      expr += buf;
    }
    else if(exprtype=="MatchStarNumberRange") {
      if(element[i]->parameter(2)<1)
	sprintf(buf,"*(%ld..%ld)",element[i]->parameter(0),element[i]->parameter(1));
      else 
	sprintf(buf,"*(%ld..%ld:%ld)",element[i]->parameter(0),element[i]->parameter(1)
		,element[i]->parameter(2));
      expr += buf;
    }
    else if(exprtype=="MatchEnd") {
    }
    else if(exprtype=="MatchStarEnd") {
      expr += "*";
    }
    else if(exprtype=="MatchAnyChar") {
      expr += "?";
    }
    else {
    }
  }
  return(expr);
}
//////////////////////////////////////////////////////////////////////////
#endif // E__PATTERN_MODIFICATION

//////////////////////////////////////////////////////////////////////////
void EasyExp::help() {
  printf("Easy Expression definition\n");
  printf(" expr <>-----* atom\n");
  printf(" atom\n");
  printf("  *           : Match any string of 0 or more characters\n");
  printf("  ?           : Match any signle character\n");
  printf("  [12aeg]     : Match a single character from the list \"12aeg\"\n");
  printf("  [a-z0-9]    : Match a single character in the range between a-z and 0-9\n");
  printf("  [^12ea]     : Match a single character not from the list \"12ea\"\n");
  printf("  [^a-z]      : Match a single character not in the range bwtween a-z\n");
  printf("  [12aeg:5]   : Match 5 characters from the list \"12aeg\"\n");
  printf("  [12aeg:2-5] : Match 2 to 5 characters from the list \"12aeg\"\n");
  printf("  [0-9:2-*]   : Match 2 more more characters in the range between 0-9\n");
  printf("  [12aeg:*]   : Match any number of characters from the list \"12aeg\"\n");
  printf("  (ab|cd|ef)  : Match from alternative \"ab\" \"cd\" or \"ef\"\n");
#ifndef NUMBER_IN_BRACES
  printf("  (5..20)     : Match a number in the range between 5 to 20\n");
  printf("  (05..20)    : Match 2 digits number in the range between 5 to 20\n");
  printf("  (05..20:2)  : Match 2 digits number in the range between 5 to 20\n");
#else
  printf("  {5-20}    (5..20)    : Match a number in the range between 5 to 20\n");
  printf("  {05-20}   (05..20)   : Match 2 digits number in the range between 5 to 20\n");
  printf("  {05-20:2} (05..20:2) : Match 2 digits number in the range between 5 to 20\n");
#endif
}

//////////////////////////////////////////////////////////////////////////
int EasyExp::complexity() const {
  int r=0;
  for(unsigned int i=0;i<m_element.size();i++) {
    r += m_element[i]->complexity();
  }
  return(r);
}

//////////////////////////////////////////////////////////////////////////
void EasyExp::disp(FILE* fp) const {
  deque<MatchElement*>::const_iterator first,last=m_element.end();
  for(first=m_element.begin();first!=last;++first) {
    (*first)->disp(fp);
  }
  printf("%lu-%lu ",argv.size(),argi.size());
  for(unsigned int i=0;i<argv.size();i++) {
    printf("%s(%d) | ",argv[i].c_str(),argi[i]);
  }
  printf("\n");

  last =m_replace.end();
  for(first=m_replace.begin();first!=last;++first) {
    if(*first) (*first)->disp(fp);
    else       fprintf(fp,"0\n");
  }
  printf("\n");
}


//////////////////////////////////////////////////////////////////////////
// EasyExp verification sample generation library
//////////////////////////////////////////////////////////////////////////
vector<string> EasyExp::verification_samples(int mode) { 
  vector<string> vs; vs.resize(1);
  deque<MatchElement*>::const_iterator first,last=m_element.end();
  for(first=m_element.begin();first!=last;++first) {
    (*first)->verification_samples(vs,mode);
  }
  return(vs);
}

//////////////////////////////////////////////////////////////////////////
string EasyExp_anystring() {return("@_ZaR@");}
string EasyExp_anychar() {return("$");}
//////////////////////////////////////////////////////////////////////////
const unsigned int EasyExp_notcharlist_limit=5;
unsigned int EasyExp_numberofsamples() {
  return(EasyExp_notcharlist_limit);
}
//////////////////////////////////////////////////////////////////////////
string EasyExp_notcharlist(const string& charlist) {
  for(unsigned int i=0;i<256;i++) {
    int c=rand()%128;
    if(isprint(c) && string::npos==charlist.find(c)) {
      char buf[4] = {0};
      buf[0]=c;
      return(buf);
    }
  }
  return("@");
}
//////////////////////////////////////////////////////////////////////////
string EasyExp_numberrange(long low,long high,long padding) {
  char buf[50],fmt[20];
  long n = low + rand()%(high-low+1);
  if(padding>0) sprintf(fmt,"%%%ldld",padding);
  else          strcpy(fmt,"%%ld");
  //printf("%s\n",fmt);
  sprintf(buf,fmt,n);
  if(padding>0) {
    for(unsigned int i=0;i<padding;i++) if(isspace(buf[i])) buf[i]='0';
  }
  return(buf);
}

//////////////////////////////////////////////////////////////////////////
// end of EasyExp verification sample generation library
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// Utitity function
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
int matchregex(const char *pattern,const char *string) {
  EasyExp EasyExpr(pattern);
  return(EasyExpr.match(string));
}

//////////////////////////////////////////////////////////////////////////
int operator==(EasyExp& ex,const char *string) {return(ex.match(string));}
int operator!=(EasyExp& ex,const char *string) {return(!ex.match(string));}
int operator==(const char *string,EasyExp& ex) {return(ex.match(string));}
int operator!=(const char *string,EasyExp& ex) {return(!ex.match(string));}
int operator==(const EasyExp& ex1,const EasyExp ex2) { return(true); }
int operator!=(const EasyExp& ex1,const EasyExp ex2) { return(false); }
int operator<(const EasyExp& ex1,const EasyExp ex2) { return(false); }
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////



#ifdef SCANDIR
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// Scan directory
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
#if defined(WIN32) || defined(_CYGWIN_) 
typedef unsigned int mode_t;
extern "C" int mkdir(const char *path,mode_t mode);
extern "C" int rmdir(const char *path);
extern "C" unsigned int sleep(unsigned int seconds);
#ifndef __CINT__
//typedef unsigned long off_t;
struct DIR;
#define NAME_MAX 256
typedef struct dirent {
  long d_ino;                /* inode number */
  off_t d_off;               /* offset to this dirent */
  unsigned short d_reclen;   /* length of d_name */
  char d_name[NAME_MAX+1];   /* file name */
} dirent;
extern "C" DIR* opendir(const char *name) ; 
extern "C" int closedir(DIR *dirp) ;
extern "C" dirent* readdir(DIR* dir);
#endif // __CINT__
#endif // WIN32
#ifdef __linux__
#ifndef __CINT__
#include <unistd.h>
#include <dirent.h>
#endif // __CINT__
#endif // __linux__
#ifdef __APPLE__
#ifndef __CINT__
#include <unistd.h>
#include <dirent.h>
#endif // __CINT__
#endif // __linux__
extern "C" int isDirectory(struct dirent* pd);

const int isdirectory = 2;
const int isfile      = 1;


//////////////////////////////////////////////////////////////////////////
void Scandir::rewind() {
  m_first = m_files.begin();
  m_last  = m_files.end();
}

//////////////////////////////////////////////////////////////////////////
string Scandir::next() {
  string result;
  if(m_first!=m_last) {
    result = (*m_first).first;
    ++m_first;
    return(result);
  }
  return(result);
}

///////////////////////////////////////// /////////////////////////////////
void Scandir::disp(FILE *fp) const {
  map<string,int>::const_iterator first = m_files.begin();
  map<string,int>::const_iterator last  = m_files.end();
  while(first!=last) {
    if((*first).second) fprintf(fp," %s/\n",(*first).first.c_str());
    else                fprintf(fp," %s\n",(*first).first.c_str());
    ++first;
  }
}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
static void merge(map<string,int>& a ,const map<string,int>& b
		  ,const string& dir) {
  map<string,int>::const_iterator first = b.begin();
  map<string,int>::const_iterator last  = b.end();
  string key;
  while(first!=last) {
    key = (*first).first;
    if(key.find_first_of("/")==0 || key.find(":/")!=string::npos) { }
    else if(key==".") { }
    else if(key=="..") { }
    else if(key.size()>2 && key[0]=='.' && key[1]=='/') { }
    else  key = dir + "/" + (*first).first;
    a[key] = (*first).second;
    ++first;
  }
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
int Scandir::scan(const string& expr,int isdir,int icase) {
  m_files.clear();
  m_isdir = isdir;
  m_icase = icase;
  size_t p;
  if(expr.find("/")==0) {
    string subexpr = expr.substr(1,string::npos);
    scandir(m_files,subexpr,"/");
  }
  else if(expr.find("\\")==0) {
    string subexpr = expr.substr(1,string::npos);
    scandir(m_files,subexpr,"\\");
  }
  else if((p=expr.find(":/"))!=string::npos) {
    string dir = expr.substr(0,p+1);
    string subexpr = expr.substr(p+2,string::npos);
    scandir(m_files,subexpr,dir);
  }
  else {
    scandir(m_files,expr,".");
  }
  rewind();
  return(size());
}

//////////////////////////////////////////////////////////////////////////
int Scandir::scandir(map<string,int>& result,const string& direxpr,const string& rootdir) {
  //printf("scandir(result,%s,%s)\n",direxpr.c_str(),rootdir.c_str());
  size_t a=direxpr.find_first_of("/",1);
  size_t b=direxpr.find_first_of("\\",1);
  size_t c= a<b?a:b;
  size_t d= 0;
  string rt = rootdir;
  if(c<direxpr.size()) {
    string dir = direxpr.substr(d,c-d);
    string expr = direxpr.substr(c+1,string::npos);
    map<string,int> dirs;
#ifdef DIRHIERARCHY
    if(dir=="**") {
      scandir_element(result,expr,rt,m_isdir); // files in this directory
      scandir_dirhierarchy(dirs,rt,rt.size()); // subdir hierarchy
    }
    else scandir_element(dirs,dir,rt,1); // one hierarchy
#else
    scandir_element(dirs,dir,rt,1); // try correct one
#endif
    //scandir_element(dirs,dir,rt,2); // workaround to linux isDirectory problem
//printf("1 %s %s %d\n",dir.c_str(),rt.c_str(),dirs.size());
    scandir_subdir(result,expr,dirs,rt);
//printf("2 %s %d %d\n",expr.c_str(),dirs.size(),result.size());
  }
  else {
    scandir_element(result,direxpr,rt,m_isdir);
  }
  return(result.size());
}

#ifdef DIRHIERARCHY
//////////////////////////////////////////////////////////////////////////
int Scandir::scandir_dirhierarchy(map<string,int>& dirs,const string& rootdir,int rtsize){
  string dir = "*";
  map<string,int> tmp;
//printf("3 scandir_dirhierarchy(%d,%s)\n",dirs.size(),rootdir.c_str());
  if(scandir_element(tmp,dir,rootdir,1)==0) return(0); 
  map<string,int>::iterator first = tmp.begin();
  map<string,int>::iterator last  = tmp.end();
  while(first!=last) {
    string nextlevel = rootdir + "/" + (*first++).first;
    dirs[nextlevel.substr(rtsize+1,string::npos)] = 1;
    scandir_dirhierarchy(dirs,nextlevel,rtsize);
  }
  return(dirs.size());
}
#endif
//////////////////////////////////////////////////////////////////////////
int Scandir::scandir_element(map<string,int>& files
                             ,const string& expr,const string& dirname
                             ,int isdir) {
  //printf("scandir_element(files,%s,%s,%d)\n",expr.c_str(),dirname.c_str(),isdir);
  EasyExp rege(expr.c_str());
  DIR *dir = opendir(dirname.c_str());
  if(!dir) {
    fprintf(stderr,"Error: Scandir::scandir_element(), opendir('%s') failed\n",dirname.c_str());
    return(-1);
  }
  struct dirent *d;
  int count=files.size();
  while((d=readdir(dir))) {
#if defined(__linux__) || defined(__APPLE__)
    int s;
    struct stat buf;
    char tmpx[1000];
    strcpy(tmpx,dirname.c_str());
    //if(d->d_name[0]!='/'&&(dirname.size()==0||dirname[dirname.size()-1]!='/'))
    strcat(tmpx,"/");
    strcat(tmpx,d->d_name);
    s=stat(tmpx,&buf);
#endif
    if(strcmp(d->d_name,".")!=0 && 
       (strcmp(d->d_name,"..")!=0 || strcmp(d->d_name,expr.c_str())==0)&&
       rege.match(d->d_name,m_icase?true:false)) {
      string tmp = d->d_name;
      //printf("3 %s %s %d %d\n",dirname.c_str(),tmp.c_str(),isDirectory(d),isdir);
      //printf(" 3' %s %s %d %d",tmpx,tmp.c_str(),S_ISDIR(buf.st_mode),isdir);
      switch(isdir) {
      case 0: // file
#if defined(__linux__) || defined(__APPLE__)
        if(!S_ISDIR(buf.st_mode)) files[tmp] = 0; 
#else
        if(!isDirectory(d)) files[tmp] = 0; 
#endif
        break;
      case 1: // directory
#if defined(__linux__) || defined(__APPLE__)
        if(S_ISDIR(buf.st_mode)) files[tmp] = 1; 
#else
        if(isDirectory(d)) files[tmp] = 1; 
#endif
        break;
      case 2:
      default: // both
#if defined(__linux__) || defined(__APPLE__)
        files[tmp] = S_ISDIR(buf.st_mode);
#else
        files[tmp] = isDirectory(d);
#endif
        break;
      }
      //printf(" %s files.size= %d %d\n",expr.c_str(),files.size(),files[tmp]);
    }
  }
  closedir(dir);
  return(files.size()-count);
}

//////////////////////////////////////////////////////////////////////////
int Scandir::scandir_subdir(map<string,int>& result
                   ,const string& expr,map<string,int>& dirs
                   ,const string& rt) {
//printf("scandir_subdir(result,%s,dirs,%s)\n",expr.c_str(),rt.c_str());
  map<string,int>::iterator first = dirs.begin();
  map<string,int>::iterator last  = dirs.end();
  string dir;
  while(first!=last) {
    if((*first).second!=0) { // Linux problem,,, to be fixed
      if(rt=="/") dir = rt+(*first).first;
      else        dir = rt+"/"+(*first).first;
      map<string,int> tmp;
 //printf("4 %s %s\n",expr.c_str(),dir.c_str());
      scandir(tmp,expr,dir);
      merge(result,tmp,dir);
 //printf("result.size=%d tmp.size=%d\n",result.size(),tmp.size());
    }
    ++first;
  }

  return(0);
}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
#ifdef _WIN32
char* stristr(const char* a,const char* b) {
  int lena = strlen(a);
  char *target = new char[lena+1];
  int lenb = strlen(b);
  char *sub    = new char[lenb+1];
  int i;

  for(i=0;i<lena;i++) target[i] = toupper(a[i]);
  target[i]=0;

  for(i=0;i<lenb;i++) sub[i] = toupper(b[i]);
  sub[i]=0;
  
  char *p = strstr(target,sub);
  delete[] sub;
  if(p) {
    int di = p-target;
    const char* p1 = a + di;
    delete[] target;
    return((char*)p1);
  }
  delete[] target;
  return(p);
}
#ifdef __ROOT__
int strnicmp(const char* a,const char*b,size_t n) {
  int lena = strlen(a);
  char *A  = new char[lena+1];
  int lenb = strlen(b);
  char *B  = new char[lenb+1];
  int i;

  for(i=0;i<lena&&i<n;i++) A[i] = toupper(a[i]);
  A[i]=0;

  for(i=0;i<lenb&&i<n;i++) B[i] = toupper(b[i]);
  B[i]=0;

  int result = strncmp(A,B,n);
  delete[] A;
  delete[] B;
  return(result);
}
#endif // __ROOT__
#endif // _WIN32
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
#endif // SCANDIR



#ifdef EASYEXP_SYNTHESIS
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
// EasyExp Synthesis
//
// string EasyExpExtraction(const vector<string>& cmatchs // matching string
// 			   ,const vector<string>& cunmatchs // umnatch string
//			   ,int mode // 0: try '*' '?' first, if fail enumerate
//                                   // 1: emumerate variation
//			   ,int LIST_LIMIT); // emumeration list limit
//
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

#if !defined(NOMINMAXEASYEXP) && defined(_WIN32)
template<class T> inline T min(const T& a,const T& b) { return(a<b?a:b); }
template<class T> inline T max(const T& a,const T& b) { return(a>b?a:b); }
#endif

////////////////////////////////////////////////////////////////////////////
// common substring vector collection
//  Did not improve segv on MacOS
////////////////////////////////////////////////////////////////////////////
void _commonsubstring(const string& a,const string& b,int MinString
		      ,int offset,vector<string>& r) {
  unsigned int i,j,count;
  string buf;
  for(i=offset;i<a.size();i++) {
    count = 0;
    buf = "";
    for(j=0;j<a.size()&&i+j<b.size();j++) {
      if(a[j]==b[i+j]) {
	// mutex protection
        buf += a[j];
	// end mutex protection
        ++count;
      }
      else {
        if(count>(unsigned int)MinString) {
	  // mutex protection
	  r.push_back(buf);
	  // end mutex protection
	}
        buf = "";
        count=0;
      }
    }
    if(count>(unsigned int)MinString) {
      // mutex protection
      r.push_back(buf);
      // end mutex protection
    }
  }
}

////////////////////////////////////////////////////////////////////////////
void commonsubstring(vector<string>& r,const string& a,const string& b,int MinString=3) {
  _commonsubstring(a,b,MinString,0,r);
  _commonsubstring(b,a,MinString,1,r);
}

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
// common substring map collection,  single thread version
////////////////////////////////////////////////////////////////////////////
void _commonsubstring(const string& a,const string& b,int MinString,int offset,map<string,int>& comstr) {
  unsigned int i,j,comlength;
  string buf;
  for(i=offset;i<b.size();i++) {
    comlength = 0;
    buf = "";
    for(j=0;j<a.size()&&(i+j)<b.size();j++) {
      if(a[j]==b[i+j]) {
        buf += a[j];
        ++comlength;
      }
      else {
        if(comlength>(unsigned int)MinString) {
	  // mutex protection
	  ++comstr[buf];
	  // end mutex protection
	}
        buf = "";
        comlength=0;
      }
    }
    if(comlength>(unsigned int)MinString) {
      // mutex protection
      ++comstr[buf];
      // end mutex protection
    }
  }
}

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
void commonsubstring(map<string,int>& comstr,const string& a,const string& b,int MinString) {
  _commonsubstring(a,b,MinString,0,comstr);
  _commonsubstring(b,a,MinString,1,comstr);
}
////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
int StrCmp_commonSubstr(const string& a
                        ,vector<string>::const_iterator first
			,vector<string>::const_iterator last
			,vector<double>& result
			,const string& param) {
  result.clear();
  map<string,int> m;
  map<string,int>::iterator first2,last2;
  int minlen = param.size();
  int count;
  while(first!=last) {
    commonsubstring(m,a,(*first),minlen);
    count = 0;
    first2 = m.begin(); last2 = m.end();
    while(first2!=last2) {
      count += (*first2).first.size()*(*first2).second; 
      ++first2;
    }
    // calculate matching ratio
    result.push_back((double)count/(*first).size());
    ++first;
  }
  return(0);
}
////////////////////////////////////////////////////////////////////////////
void MakeSearchString(const string& str,string& buf,int mode) {
#if defined(E__MULTIBYTE) || defined(E__MULTIBYTELIB) 
  G__MultiByteMode multibyte;
#endif
  int c;
  unsigned int i;
  for(i=0;i<str.size();i++) {
    switch((c=str[i])) {
    case '(': 
      if(mode==0) buf += "[(]";
      else        buf += "\\(";
      break;
    case ')': 
      if(mode==0) buf += "[)]"; 
      else        buf += "\\)";
      break;
    case '[': case ']': case '*': case '?':
      buf += '\\';
    default:
      buf += c;
#if defined(E__MULTIBYTE) || defined(E__MULTIBYTELIB) 
      if(multibyte.IsDBCSLeadByte(c)) {
        c=str[++i];
        buf+=(char)c;
        multibyte.CheckDBCS2ndByte(c);
      }
#endif
      break;
    }
  }
}

////////////////////////////////////////////////////////////////////////////
// number range synthesis
////////////////////////////////////////////////////////////////////////////
int MakeSearchNumberRange(map<string,int> prestr,statistics& st,string& expr) {
  // find first non digit char position in prestr 
  statistics stn;
  string s;
  size_t pos;
  map<string,int>::iterator first,last = prestr.end();
  for(first=prestr.begin();first!=last;++first) {
    s = (*first).first;
    pos=s.find_first_not_of("0123456789");
    if(string::npos!=pos) stn.add(pos);
    else stn.add(s.size());
  }

  // if there is fixed digit number
  if(stn.mean()>0 && stn.stddev()==0 && stn.nsample()>1) {
    // synthesize number range expression
    char tmp[200];
    first = prestr.begin();
    map<string,int>::reverse_iterator rfirst = prestr.rbegin();
    if(stn.mean()<st.mean()) {
      sprintf(tmp,"(%s..%s:%g)",(*first).first.substr(0,stn.mean()).c_str()
	      ,(*rfirst).first.substr(0,stn.mean()).c_str()
	      ,stn.mean());
      expr += tmp;

      // TODO: Following implementation is not perfect.
#ifdef SYNTH_NUMBERRANGE
      // add rest of the strings if exist which is not guaranteed to be unique.
      // so, first put it into map.
      map<string,int> poststr;
      for(first=prestr.begin();first!=last;++first) ++poststr[(*first).first.substr(stn.mean())];
      
      // then output
      expr += "(";
      first = poststr.begin(); last = poststr.end();
      MakeSearchString((*first).first,expr,1);
      for(++first;first!=last;++first) {
	expr += "|";
	MakeSearchString((*first).first,expr,1);
      }
      expr += ")";
#else
      // Maybe simple '*' output is better. Maybe not
      expr += "*";
#endif
      // END TODO
    }
    else {
      sprintf(tmp,"(%s..%s:%g)",(*first).first.c_str()
	      ,(*rfirst).first.c_str()
	      ,stn.mean());
      expr += tmp;
    }
    return(1);
  }
  else {
    // there is no fixed digit number
    return(0);
  }
}

#ifdef EASYEXP_MULTI_THREAD
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
// Include header 
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
#ifdef WIN32
#include <windows.h>
#undef min  // need to undef min and max for Stat<T>::min and max
#undef max
#else // LINUX, MacOS
#include <pthread.h>
#include <sys/resource.h>
#endif // WINDOWS, LINUX, MacOS

////////////////////////////////////////////////////////////////////////////
// Critical Section
////////////////////////////////////////////////////////////////////////////
#ifdef WIN32
// Windows Critical Section API as is
#else
// Emulation of Windows Critical Section API
typedef  pthread_mutex_t      CRITICAL_SECTION;
typedef  pthread_mutex_t*   LPCRITICAL_SECTION;
inline void InitializeCriticalSection(LPCRITICAL_SECTION pc) {
  pthread_mutexattr_t attr;
  pthread_mutexattr_init(&attr); 
  //pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE_NP);
  pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
  pthread_mutex_init(pc, &attr);
  pthread_mutexattr_destroy(&attr);
}
inline void EnterCriticalSection(LPCRITICAL_SECTION pc) {pthread_mutex_lock(pc);}
inline void LeaveCriticalSection(LPCRITICAL_SECTION pc) {pthread_mutex_unlock(pc);}
#endif

#ifdef CRITICALSECTION_PROTECTION
////////////////////////////////////////////////////////////////////////////
// common substring map collection,  multi thread version
////////////////////////////////////////////////////////////////////////////
void _commonsubstring(const string& a,const string& b,int MinString,int offset,map<string,int>& comstr
		      ,LPCRITICAL_SECTION pc) {
  unsigned int i,j,jstart,comlength;
  string buf;
  for(i=offset;i<b.size();i++) {
    jstart=0;
    comlength = 0;
    buf = "";
    for(j=0;j<a.size()&&(i+j)<b.size();j++) {
      if(a[j]==b[i+j]) {
        ++comlength;
      }
      else {
        if(comlength>(unsigned int)MinString) {
	  EnterCriticalSection(pc);
	  buf = a.substr(jstart,comlength);
	  ++comstr[buf];
	  LeaveCriticalSection(pc);
	}
        buf = "";
	jstart=j+1;
        comlength=0;
      }
    }
    if(comlength>(unsigned int)MinString) {
      EnterCriticalSection(pc);
      buf = a.substr(jstart,comlength);
      ++comstr[buf];
      LeaveCriticalSection(pc);
    }
  }
}

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
void commonsubstring(map<string,int>& comstr,const string& a,const string& b,int MinString
		     ,LPCRITICAL_SECTION pc) {
  _commonsubstring(a,b,MinString,0,comstr,pc);
  _commonsubstring(b,a,MinString,1,comstr,pc);
}
////////////////////////////////////////////////////////////////////////////
#endif

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
// merge multi-thread data set
////////////////////////////////////////////////////////////////////////////
void mergevec_string(vector<string>& a,vector<string>& b) {
  vector<string>::iterator first,last = b.end();
  for(first=b.begin();first!=last;++first) a.push_back(*first);
}
////////////////////////////////////////////////////////////////////////////
void mergevec_string(vector<string>& a,vector<vector<string> >& b) {
  for(unsigned int i=0;i<b.size();i++) {
    mergevec_string(a,b[i]);
  }
}
////////////////////////////////////////////////////////////////////////////
void mergemap_string_int(map<string,int>& a,map<string,int>& b) {
  map<string,int>::iterator first,last = b.end();
  for(first=b.begin();first!=last;++first) a[(*first).first]+=(*first).second;
}
////////////////////////////////////////////////////////////////////////////
void mergemap_string_int(map<string,int>& a,vector<map<string,int> >& b) {
  for(unsigned int i=0;i<b.size();i++) {
    mergemap_string_int(a,b[i]);
  }
}

static inline unsigned int TotalCount(unsigned int Size) {return(Size*Size/2);}
////////////////////////////////////////////////////////////////////////////
unsigned int EasyExp_num_threads = 4;
unsigned int EasyExp_num_threads_limit = 64;
unsigned int EasyExp_thread_timeout = 1000000; // msec=1000sec, Windows only


////////////////////////////////////////////////////////////////////////////
struct EasyExp_multithread_data {
  unsigned int  threadid;
  vector<void*>     p_obj;
  unsigned int  istart;
  unsigned int  iend;
#ifdef CRITICALSECTION_PROTECTION
  CRITICAL_SECTION criticalsection;
  EasyExp_multithread_data() {InitializeCriticalSection(&criticalsection);}
#endif
};

#define MULTI_THREAD_OBJ_RETRIEVE(Tobj,obj,index)	\
  Tobj& obj = *(Tobj*)(mtdata->p_obj[index])

#define MULTI_THREAD_OBJ_ASSIGN(Tobj,obj,index)	\
  mtdataary[i].p_obj.push_back((void*)&obj)

#define MULTI_THREAD_VAL_RETRIEVE(Tobj,obj,index)	\
  Tobj obj = (Tobj)(mtdata->p_obj[index])

#define MULTI_THREAD_VAL_ASSIGN(Tobj,obj,index)	\
  mtdataary[i].p_obj.push_back((void*)obj)

////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////
// easy exp matching
////////////////////////////////////////////////////////////////////////////
static
#ifdef WIN32
int 
#else
void*
#endif
thread_easyexpmatch(void* arg) {
  EasyExp_multithread_data *mtdata = (EasyExp_multithread_data*)arg;
  // Customize area
  MULTI_THREAD_OBJ_RETRIEVE(const vector<string>,in,0);
  MULTI_THREAD_OBJ_RETRIEVE(vector<string>,result,1);
  MULTI_THREAD_OBJ_RETRIEVE(EasyExp,e,2);
  MULTI_THREAD_VAL_RETRIEVE(bool,icase,3);
  MULTI_THREAD_VAL_RETRIEVE(bool,matchunmatch,4);
  // End customize area
  //const vector<string>& cmatchs = *(const vector<string>*)mtdata->p_obj[1];
  unsigned int istart = mtdata->istart;
  unsigned int iend   = mtdata->iend;
  unsigned int i;
  string a;
  //printf("%d istart=%d iend=%d\n",mtdata->threadid,istart,iend);
  //e.disp();
  if(matchunmatch) {
    for(i=istart;i<iend;i++) {
      if(e.match(in[i].c_str(),icase)) result.push_back(in[i]);
    }
  }
  else {
    for(i=istart;i<iend;i++) {
      if(!e.match(in[i].c_str(),icase)) result.push_back(in[i]);
    }
  }
  //printf("result size=%d   matchunmatch=%d\n",result.size(),matchunmatch);
  return(0);
}

////////////////////////////////////////////////////////////////////////////
static
void multithread_easyexpmatch(const vector<string>& in,bool icase
			      ,EasyExp& e,vector<string>& result
			      ,bool matchunmatch) {
  unsigned int i, istep = in.size()/EasyExp_num_threads;
  struct EasyExp_multithread_data *mtdataary
    = new EasyExp_multithread_data[EasyExp_num_threads];
  // Customize area
  vector<vector<string> > mtresult; mtresult.resize(EasyExp_num_threads);
  vector<EasyExp> mtexp; mtexp.resize(EasyExp_num_threads);
  // End customize area
  //printf("size = %d  istep=%d\n",cmatchs.size(),istep);
  for(i=0;i<EasyExp_num_threads;i++) {
    mtdataary[i].threadid = i;
    // Customize area
    mtexp[i].compile(e.pattern().c_str());
    MULTI_THREAD_OBJ_ASSIGN(const vector<string>,in,0);
    MULTI_THREAD_OBJ_ASSIGN(vector<string>,mtresult[i],1);
    MULTI_THREAD_OBJ_ASSIGN(EasyExp,mtexp[i],2);
    MULTI_THREAD_VAL_ASSIGN(bool,icase,3);
    MULTI_THREAD_VAL_ASSIGN(bool,matchunmatch,4);
    // End customize area
    mtdataary[i].istart = istep*i;
    mtdataary[i].iend   = istep*(i+1);
  }
  mtdataary[i-1].iend = in.size();
#ifdef WIN32
  DWORD prio=GetPriorityClass(GetCurrentProcess()); // store proc priority
  SetPriorityClass(GetCurrentProcess(),NORMAL_PRIORITY_CLASS);
  HANDLE *h = (HANDLE*)malloc(sizeof(HANDLE)*EasyExp_num_threads);
  DWORD  *ThreadID = (DWORD*)malloc(sizeof(DWORD)*EasyExp_num_threads);
  for(i=0;i<EasyExp_num_threads;i++) {
    h[i] = CreateThread(NULL,0
			,(LPTHREAD_START_ROUTINE)thread_easyexpmatch //customize
			,(void*)(&mtdataary[i]),0
			,&ThreadID[i]);
  }
  WaitForMultipleObjects(EasyExp_num_threads,h,true,EasyExp_thread_timeout); 
  for(i=0;i<EasyExp_num_threads;i++) CloseHandle(h[i]);
  //printf("joined \n");
  free((void*)ThreadID);
  free((void*)h);
  SetPriorityClass(GetCurrentProcess(),prio);// restore proc priority
#else
  int prio=getpriority(PRIO_PROCESS,0); // store process priority
  setpriority(PRIO_PROCESS,0,10); // set low priority
  pthread_t *pt = (pthread_t *)malloc(EasyExp_num_threads*sizeof(pthread_t));
  for (i=0;i<EasyExp_num_threads;i++) {
    pthread_create(&pt[i], NULL,thread_easyexpmatch, (void *)(&mtdataary[i]));
  }
  for (i=0;i<EasyExp_num_threads;i++) pthread_join(pt[i], NULL);
  free((void*)pt);
  setpriority(PRIO_PROCESS,0,prio); // restore process priority
#endif
  //printf("before merge \n");
  mergevec_string(result,mtresult); // customize line
  delete[] mtdataary;
  //e.disp();
  //printf("merged size=%d  %d\n",result.size(),matchunmatch);
}

////////////////////////////////////////////////////////////////////////////
// common string extraction using vector<string>
//   Not used
////////////////////////////////////////////////////////////////////////////
static
#ifdef WIN32
int 
#else
void*
#endif
thread_commonsubstrvec(void* arg) {
  EasyExp_multithread_data *mtdata = (EasyExp_multithread_data*)arg;
  // Customize area
  MULTI_THREAD_OBJ_RETRIEVE(vector<string>,comstr,0);
  MULTI_THREAD_OBJ_RETRIEVE(const vector<string>,cmatchs,1);
  // End customize area
  unsigned int istart = mtdata->istart;
  unsigned int iend   = mtdata->iend;
  unsigned int i,k;
  string a;
  //printf("%d istart=%d iend=%d\n",mtdata->threadid,istart,iend);
  for(k=istart;k<iend;k++) {
    a = cmatchs[k]; // cmatchs[k];
    for(i=k+1;i<cmatchs.size();i++) {
      commonsubstring(comstr,a,cmatchs[i],1);
    }
  }
  return(0);
}

////////////////////////////////////////////////////////////////////////////
static
void multithread_commonsubstrvec(vector<string>& comstr
				 ,const vector<string>& cmatchs) {
  unsigned int i,prev=0, sum=0, istep=TotalCount(cmatchs.size())/EasyExp_num_threads;
  struct EasyExp_multithread_data *mtdataary
    = new EasyExp_multithread_data[EasyExp_num_threads];
  // Customize area
  vector<vector<string> > mtcomstr; mtcomstr.resize(EasyExp_num_threads);
  // End Customize area
  //printf("size = %d  istep=%d\n",cmatchs.size(),istep);
  for(i=0;i<EasyExp_num_threads;i++) {
    mtdataary[i].threadid = i;
    // Customize area
    MULTI_THREAD_OBJ_ASSIGN(vector<string>,comstr,0);
    MULTI_THREAD_OBJ_ASSIGN(const vector<string>,cmatchs,1);
    // End customize area
    mtdataary[i].istart = prev;
    while(sum<istep*(i+1)) {
      sum += cmatchs.size()-prev;
      ++prev;
    }
    mtdataary[i].iend   = prev;
  }
  mtdataary[i-1].iend = cmatchs.size();
#ifdef WIN32
  DWORD prio=GetPriorityClass(GetCurrentProcess()); // store proc priority
  SetPriorityClass(GetCurrentProcess(),NORMAL_PRIORITY_CLASS);
  HANDLE *h = (HANDLE*)malloc(sizeof(HANDLE)*EasyExp_num_threads);
  DWORD  *ThreadID = (DWORD*)malloc(sizeof(DWORD)*EasyExp_num_threads);
  for(i=0;i<EasyExp_num_threads;i++) {
    h[i] = CreateThread(NULL,0
			,(LPTHREAD_START_ROUTINE)thread_commonsubstrvec
			,(void*)(&mtdataary[i]),0
			,&ThreadID[i]);
  }
  WaitForMultipleObjects(EasyExp_num_threads,h,true,EasyExp_thread_timeout); 
  for(i=0;i<EasyExp_num_threads;i++) CloseHandle(h[i]);
  //printf("joined \n");
  free((void*)ThreadID);
  free((void*)h);
  SetPriorityClass(GetCurrentProcess(),prio);// restore proc priority
#else
  int prio=getpriority(PRIO_PROCESS,0); // store process priority
  setpriority(PRIO_PROCESS,0,10); // set low priority
  pthread_t *pt = (pthread_t *)malloc(EasyExp_num_threads*sizeof(pthread_t));
  for (i=0;i<EasyExp_num_threads;i++) {
    pthread_create(&pt[i], NULL,thread_commonsubstrvec,(void *)(&mtdataary[i]));
  }
  for (i=0;i<EasyExp_num_threads;i++) pthread_join(pt[i], NULL);
  free((void*)pt);
  setpriority(PRIO_PROCESS,0,prio); // restore process priority
#endif
  //printf("before merge \n");
  mergevec_string(comstr,mtcomstr); // customize
  delete[] mtdataary;
  //printf("merged \n");
}


////////////////////////////////////////////////////////////////////////////
// common string extraction using map<string,int>
////////////////////////////////////////////////////////////////////////////
static
#ifdef WIN32
int 
#else
void*
#endif
thread_commonsubstrmap(void* arg) {
  EasyExp_multithread_data *mtdata = (EasyExp_multithread_data*)arg;
  // Customize area
  typedef map<string,int> map_string_int;
  MULTI_THREAD_OBJ_RETRIEVE(map_string_int,comstr,0);
  MULTI_THREAD_OBJ_RETRIEVE(const vector<string>,cmatchs,1);
  // End customize area
#ifdef CRITICALSECTION_PROTECTION
  LPCRITICAL_SECTION p_criticalsection = &mtdata->criticalsection;
#endif
  unsigned int istart = mtdata->istart;
  unsigned int iend   = mtdata->iend;
  unsigned int i,k;
  string a;
  //printf("%d istart=%d iend=%d\n",mtdata->threadid,istart,iend);
  for(k=istart;k<iend;k++) {
    a = cmatchs[k]; // cmatchs[k];
    for(i=k+1;i<cmatchs.size();i++) {
#ifdef CRITICALSECTION_PROTECTION
      commonsubstring(comstr,a,cmatchs[i],1,p_criticalsection);
#else
      commonsubstring(comstr,a,cmatchs[i],1);
#endif
    }
    //comstr=reject_partialcomstr(comstr,istart-iend);
  }
  return(0);
}

////////////////////////////////////////////////////////////////////////////
static
void multithread_commonsubstrmap(map<string,int>& comstr
				 ,const vector<string>& cmatchs) {
  unsigned int i,prev=0, j=0, sum=0, istep=TotalCount(cmatchs.size())/EasyExp_num_threads;
  struct EasyExp_multithread_data *mtdataary
    = new EasyExp_multithread_data[EasyExp_num_threads];
  // Customize area
  typedef map<string,int> map_string_int;
  vector<map_string_int> mtcomstr; mtcomstr.resize(EasyExp_num_threads);
  // End Customize area
  //printf("size = %d  istep=%d\n",cmatchs.size(),istep);
  for(i=0;i<EasyExp_num_threads;i++) {
    mtdataary[i].threadid = i;
    // Customize area
    MULTI_THREAD_OBJ_ASSIGN(map_string_int,comstr,0);
    MULTI_THREAD_OBJ_ASSIGN(const vector<string>,cmatchs,1);
    // End customize area
    mtdataary[i].istart = prev;
    while(sum<istep*(i+1)) {
      sum += cmatchs.size()-prev;
      ++prev;
    }
    mtdataary[i].iend   = prev;
  }
  mtdataary[i-1].iend = cmatchs.size();
#ifdef WIN32
  DWORD prio=GetPriorityClass(GetCurrentProcess()); // store proc priority
  SetPriorityClass(GetCurrentProcess(),NORMAL_PRIORITY_CLASS);
  HANDLE *h = (HANDLE*)malloc(sizeof(HANDLE)*EasyExp_num_threads);
  DWORD  *ThreadID = (DWORD*)malloc(sizeof(DWORD)*EasyExp_num_threads);
  for(i=0;i<EasyExp_num_threads;i++) {
    h[i] = CreateThread(NULL,0
			,(LPTHREAD_START_ROUTINE)thread_commonsubstrmap
			,(void*)(&mtdataary[i]),0
			,&ThreadID[i]);
  }
  WaitForMultipleObjects(EasyExp_num_threads,h,true,EasyExp_thread_timeout); 
  for(i=0;i<EasyExp_num_threads;i++) CloseHandle(h[i]);
  //printf("joined \n");
  free((void*)ThreadID);
  free((void*)h);
  SetPriorityClass(GetCurrentProcess(),prio);// restore proc priority
#else
  int prio=getpriority(PRIO_PROCESS,0); // store process priority
  setpriority(PRIO_PROCESS,0,10); // set low priority
  pthread_t *pt = (pthread_t *)malloc(EasyExp_num_threads*sizeof(pthread_t));
  for (i=0;i<EasyExp_num_threads;i++) {
    pthread_create(&pt[i], NULL,thread_commonsubstrmap,(void *)(&mtdataary[i]));
  }
  for (i=0;i<EasyExp_num_threads;i++) pthread_join(pt[i], NULL);
  free((void*)pt);
  setpriority(PRIO_PROCESS,0,prio); // restore process priority
#endif
  //printf("before merge \n");
  mergemap_string_int(comstr,mtcomstr); // customize
  delete[] mtdataary;
  //printf("merged \n");
}

////////////////////////////////////////////////////////////////////////////
#endif // EASYEXP_MULTI_THREAD

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// eashexp matching 
//////////////////////////////////////////////////////////////////////////
vector<string> EasyExp::match(const vector<string>& in,bool icase) {
  vector<string> result;
#if defined(EASYEXP_MULTI_THREAD) 
  // Multi-thread implementation finds result in parallel and merge at the end.
  if(EasyExp_num_threads>1 && in.size()>EasyExp_num_threads_limit) {
    multithread_easyexpmatch(in,icase,*this,result,1);
    return(result);
  }
#endif
  for(unsigned int i=0;i<in.size();i++) {
    if(match(in[i].c_str(),icase)) result.push_back(in[i]);
  }
  return(result);
}

//////////////////////////////////////////////////////////////////////////
vector<string> EasyExp::_match(const vector<string>& in,bool icase) {
  vector<string> result;
#if defined(EASYEXP_MULTI_THREAD) 
  // Multi-thread implementation finds result in parallel and merge at the end.
  if(EasyExp_num_threads>1 && in.size()>EasyExp_num_threads_limit) {
    multithread_easyexpmatch(in,icase,*this,result,0);
    return(result);
  }
#endif
  for(unsigned int i=0;i<in.size();i++) {
    if(!match(in[i].c_str(),icase)) result.push_back(in[i]);
  }
  return(result);
}

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
// common string extraction
////////////////////////////////////////////////////////////////////////////
// Out of date
void EasyExpExtraction_comstr(const vector<string>& cmatchs
			      ,vector<map<string,int> >& prestr_list
			      ,vector<statistics>& prestr_size
			      ,vector<string>& comstr_list) {
  // prestr is a variable string ahead of comstr
  // prestr[0] comstr[0] prestr[1] comstr[1] ...
  map<string,int> comstr;
  unsigned int i,j,k;
  string a,b;

  // Extract common substring among all data samples. THIS IS TIME CONSUMING, 
#if defined(EASYEXP_MULTI_THREAD) && !defined(EASYEXP_SYNTHESIS_NO_MULTITHREAD)
  // Multi-thread implementation finds comstr in parallel and merge at the end.
  if(EasyExp_num_threads>1 && cmatchs.size()>EasyExp_num_threads_limit) 
    multithread_commonsubstrmap(comstr,cmatchs);
  else 
#endif
  for(k=0;k<cmatchs.size();k+=1) {
    a = cmatchs[k];
    for(i=k+1;i<cmatchs.size();i++) {
      commonsubstring(comstr,a,cmatchs[i],1);
    }
    //comstr=reject_partialcomstr(comstr,cmatchs.size());
  }

  // sort common strings with position
  unsigned int count=0 ,sz = cmatchs.size();
  statistics st;
  size_t pos;
  string str;
  map<double,string> Substr;
  map<string,int>::iterator first,last  = comstr.end();
  for(first=comstr.begin();first!=last;++first) {
    st.init();
    str = (*first).first;
    count = 0;
    for(j=0;j<sz;j++) {
      pos = cmatchs[j].find(str);
      if(pos!=string::npos) {
        ++count;
        st.add((double)pos);
      }
    }
    if(count==sz) {
      Substr[st.mean()] = str;
    }
  }

  // list common substring in order and analyze string between them
  map<string,int> prestr; 
  string expr;
  st.init();
  map<double,string>::iterator first2=Substr.begin(),last2=Substr.end();
  if(first2==last2) { // no common string
    // whole string as one prestring '*' or '(A|B|C|..)'
    for(i=0;i<cmatchs.size();i++) {
      ++prestr[cmatchs[i]];
      st.add(cmatchs[i].size());
    }
    prestr_list.push_back(prestr); // the 1st entry has all cmatchs_in strings
    prestr_size.push_back(st); // length statistics
  }
  else {
    vector<string> cnext;
    str = (*first2).second; // get the first comstr
    for(i=0;i<cmatchs.size();i++) {
      pos = cmatchs[i].find(str); // get position of comstr
      ++prestr[cmatchs[i].substr(0,pos)]; // retrieve expression
      st.add(pos); // position statistics, is this used??
      pos += str.size(); // move after the comstr position
      if(cmatchs[i].size()>pos) { // if target string is longer then pos
	cnext.push_back(cmatchs[i].substr(pos));//retrive lest of the string
      }
    }
    // aaaCOMzzz, bbCOMyyy, aaaCOMxxx
    //  prestr_list[0] = prestr = <aaa,2>, <bb,1>
    //  prestr_size[0] = st     = stat(3,2,3) 
    //  comstr_list[0] = str    = COM
    prestr_list.push_back(prestr);//expression string
    prestr_size.push_back(st);//expression size statistics
    comstr_list.push_back(str);//common string
    if(cnext.size()) { // recursively extract expression and comstr
      EasyExpExtraction_comstr(cnext,prestr_list,prestr_size,comstr_list) ;
    }
  }
}
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
// smart, reduced version of common string extraction
// faster in some cases, but probability output is not accurate
////////////////////////////////////////////////////////////////////////////
void SmartEasyExpExtraction_comstr(const vector<string>& cmatchs_in
				   ,const vector<int>& cmatchs_count_in
				   ,vector<map<string,int> >& prestr_list
				   ,vector<statistics>& prestr_size
				   ,vector<string>& comstr_list
				   ,int position_mode) {
  // prestr is a variable string ahead of comstr
  // prestr[0] comstr[0] prestr[1] comstr[1] ...
  string str;
  unsigned int i,j,k;
  string a,b;

  ////////////////////////////////////////////////////////////////////////
  // Extranct unique set of input vector string. This part is the difference
  // between EasyExpExtraction_comstr()
  map<string,int> unique;
  for(i=0;i<cmatchs_in.size();i++) ++unique[cmatchs_in[i]];
  vector<string> cmatchs;
  vector<int> cmatchs_count;
  if(cmatchs_count_in.size()) {
    cmatchs = cmatchs_in;
    cmatchs_count = cmatchs_count_in;
  }
  else {
    map<string,int>::iterator first0,last0 = unique.end();
    for(first0=unique.begin();first0!=last0;++first0) {
      cmatchs.push_back((*first0).first);
      cmatchs_count.push_back((*first0).second);
    }
  }

  ////////////////////////////////////////////////////////////////////////
  // Extract common substring among all data samples. THIS IS TIME CONSUMING, 
  map<string,int> comstr;
#if defined(EASYEXP_MULTI_THREAD) && !defined(EASYEXP_SYNTHESIS_NO_MULTITHREAD)
  // Multi-thread implementation finds comstr in parallel and merge at the end.
  if(EasyExp_num_threads>1 && cmatchs.size()>EasyExp_num_threads_limit) 
    multithread_commonsubstrmap(comstr,cmatchs);
  else 
#endif
  for(k=0;k<cmatchs.size();k+=1) {
    a = cmatchs[k];
    for(i=k+1;i<cmatchs.size();i++) {
      commonsubstring(comstr,a,cmatchs[i],1);
    }
    //comstr=reject_partialcomstr(comstr,cmatchs.size());
  }

  ////////////////////////////////////////////////////////////////////////

  ////////////////////////////////////////////////////////////////////////
  // sort common strings with position
  int count=0 ,sz = cmatchs.size();
  statistics st, st_e, cmatch_st;
  // calculate legth statistics
  for(j=0;j<(unsigned int)sz;j++) cmatch_st.add(cmatchs[j].size());
  // calculate pos statistics for each common string candidate
  size_t pos;
  map<double,string> Substr;
  map<string,int>::iterator first,last  = comstr.end();
  for(first=comstr.begin();first!=last;++first) { // scan each comstr
    st.init();
    st_e.init();
    str = (*first).first;
    count = 0;
    for(j=0;j<(unsigned int)sz;j++) { // find position in original string
      pos = cmatchs[j].find(str);
      if(pos!=string::npos) {
        ++count;
        st.add((double)pos);
      }
      pos = cmatchs[j].find_last_of(str);
      if(pos!=string::npos) st_e.add((double)(cmatchs[j].size()-pos));
    }
    if(count==sz) { // all original string contains this comstr
      // CAUTION: comstr may be lost if st.mean() matches.
      // For example, comstr[]={"ABCD","EFGH"}
      //  cmatchs[0] = "xxxxABCDxxxxEFGHxxxx"
      //  cmatchs[1] = "xxxxEFGHxxxxABCDxxxx"
      // Is this bad?
      // In this particular case, either one can be treated as comstr
      // and the other one has to be ignored. So, this is probably ok.
      // Alternatively, we can ignore both.
      // Another issue. If we have xxxAAA, xxxBBB, xxxAAA, etc...
      //
      // Trying to solve above problem. If pos deviation is relatively 
      // larger than deviation of whole string length, we will discard it.
      if(position_mode<0) {
	// anything goes
	Substr[st.mean()]=str; 
      }
      else if(position_mode==0) {
	// allow typical deviation
	if(
	   st.max()-st.min() < 4 ||
	   st_e.max()-st_e.min() < 4 ||
	   //cmatch_st.stddev()>st.stddev() ||
	   //cmatch_st.stddev()>st_e.stddev() ||
	   (cmatch_st.stddev()==0 && st.max()-st.min()<3)) // experimental
	  Substr[st.mean()]=str; 
      }
      else if(position_mode>0) {
	// manual setting
	if(st.max()-st.min()<position_mode) Substr[st.mean()]=str; 
      }
      else {
	// strict allignment
	if(st.stddev()==0) Substr[st.mean()]=str; 
      }
    }
  }

  ////////////////////////////////////////////////////////////////////////
  // list common substring in order and analyze string between them
  map<string,int> prestr; 
  string expr;
  st.init();
  map<double,string>::iterator first2=Substr.begin(),last2=Substr.end();
  if(first2==last2) { // no common string
    // whole string as one prestring '*' or '(A|B|C|..)'
    for(i=0;i<sz;i++) {
      prestr[cmatchs[i]] += cmatchs_count[i];
      //printf("FFFF %s %d %d\n",cmatchs[i].c_str(),cmatchs_count[i],prestr[cmatchs[i]]);
      for(j=0;j<(unsigned int)cmatchs_count[i];j++) st.add(cmatchs[i].size());
    }
    // aaa, bb, aaa
    //  prestr_list[0] = prestr = <aaa,2>, <bb,1>
    //  prestr_size[0] = st     = stat(3,2,3) 
#undef XX_DEBUG
#ifdef XX_DEBUG
    map<string,int>::iterator first3,last3=prestr.end(); // DEBUG 
    printf(" non-common str '%s'\n",str.c_str());
    for(first3=prestr.begin();first3!=last3;++first3) {
      printf(" EEE '%s' %d\n",(*first3).first.c_str(),(*first3).second);
    }  // END DEBUG
    //
#endif
    prestr_list.push_back(prestr); // the 1st entry has all cmatchs_in strings
    prestr_size.push_back(st); // length statistics
  }
  else {
    vector<string> cnext;
    vector<int> cnext_count;
    str = (*first2).second; // get the first comstr
    for(i=0;i<sz;i++) {
      pos = cmatchs[i].find(str); // get position of comstr
      prestr[cmatchs[i].substr(0,pos)]+=cmatchs_count[i];// retrieve expression
      for(j=0;j<(unsigned int)cmatchs_count[i];j++) st.add(pos);//is this used?
      pos += str.size(); // move after the comstr position
      if(cmatchs[i].size()>pos) { // if target string is longer then pos
	// cmatchs_count information is lost. This is the cause of probability error.
	cnext.push_back(cmatchs[i].substr(pos));//retrive lest of the string
	cnext_count.push_back(cmatchs_count[i]);
      }
    }
    // aaaCOMzzz, bbCOMyyy, aaaCOMxxx
    //  prestr_list[0] = prestr = <aaa,2>, <bb,1>
    //  prestr_size[0] = st     = stat(3,2,3) 
    //  comstr_list[0] = str    = COM
#ifdef XX_DEBUG
    map<string,int>::iterator first3,last3=prestr.end(); // DEBUG 
    printf(" common str '%s'\n",str.c_str());
    for(first3=prestr.begin();first3!=last3;++first3) {
      printf(" DDD '%s' %d\n",(*first3).first.c_str(),(*first3).second);
    }  // END DEBUG
    //
#endif
    prestr_list.push_back(prestr);//expression string
    prestr_size.push_back(st);//expression size statistics
    comstr_list.push_back(str);//common string
    if(cnext.size()) { // recursively extract expression and comstr
      // cmatchs_count information is lost. This is the cause of probability error.
      SmartEasyExpExtraction_comstr(cnext,cnext_count,prestr_list,prestr_size,comstr_list
				    ,position_mode) ;
    }
  }
}

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
void synthesize_stringlist_with_stat(string& expr,map<string,int>& prestr
				     ,unsigned int SampleSize) {
  map<string,int>::iterator first;
  char buf[100];
  double vsum=(double)SampleSize, vvsum=0 ,count;
  expr += "(";
  first = prestr.begin();
  //expr += (*first).first;
  MakeSearchString((*first).first,expr);
  count = (*first).second;
  sprintf(buf,";%f",count/vsum);
  vvsum += count;
  expr += buf;
  for(++first;first!=prestr.end();++first) {
    expr += "|";
    //expr += (*first).first;
    MakeSearchString((*first).first,expr);
    count = (*first).second;
    sprintf(buf,";%f",count/vsum);
    vvsum += count;
    expr += buf;
  }
  if(vsum-vvsum>0.5) {
    sprintf(buf,"|;%f",(vsum-vvsum)/vsum);
    expr += buf;
  }
  expr += ")";
}

////////////////////////////////////////////////////////////////////////////
string chartosym(int ch) {
  string expr;
  switch(ch) {
  case '[': case ']': case '^': case '\\': case '-': case ':':
    expr += "\\";
  default:
    expr += ch;
#if defined(E__MULTIBYTE) || defined(E__MULTIBYTELIB)
    // multibyte char is not allowed in [ ]
#endif
    break;
  }
  return(expr);
}
////////////////////////////////////////////////////////////////////////////
string synthesize_charlist(map<string,int>& prestr ,unsigned int prestr_total
			   ,unsigned int SampleSize
			   ,int mode,int LIMIT_CHAR=30) {
  string buf = "[";
  char ch,pre_ch=0,count=0,start_ch=0;
  int zeroflag=0;
  map<string,int>::iterator first,last = prestr.end();
  for(first=prestr.begin();first!=last;++first) {
    ch = (*first).first[0];
    if(!ch) { // detect "" 
      zeroflag=1;
      continue;
    }
    // a little complicated logic to synthesize [a-zA-Z0-9] 
    if(ch==pre_ch+1) {
      if(0==count) {
	start_ch=pre_ch;
      }
      ++count;
    }
    else {
      if(count>2) {
	buf += "-";
	buf += chartosym(pre_ch);
      }
      else if(start_ch) {
	for(int c=start_ch+1;c<=pre_ch;c++) buf += chartosym(c);
      }
      else {
      }
      buf += chartosym(ch);
      start_ch=0;
      count=0;
    }
    pre_ch = ch;
  }
  if(pre_ch) {
    if(count>2) {
      buf += "-";
      buf += chartosym(pre_ch);
    }
    else if(start_ch) {
      for(int c=start_ch+1;c<=pre_ch;c++) buf += chartosym(c);
    }
    else {
      //  buf += chartosym(ch);
    }
  }
  // if there is "" in the list, match 0 or 1 length string 
  if(zeroflag || prestr_total<SampleSize) buf += ":0-1";
  buf += "]";

  if(mode==0 || buf.size()>LIMIT_CHAR) {
    string expr;
    if(zeroflag || prestr_total<SampleSize) expr = "*";
    else         expr = "?";
    return(expr);
  }
  return(buf);
}

////////////////////////////////////////////////////////////////////////////
// Synthesis body
////////////////////////////////////////////////////////////////////////////
string EasyExpSynthesis_body(//string& comstr,string& expr,
			     unsigned int SampleSize
			     ,vector<map<string,int> >& prestr_list 
			     ,const vector<statistics>& prestr_size 
			     ,const vector<string>& comstr_list
			     ,int mode,int LIST_LIMIT,int statmode=0) {
  unsigned int i;
  map<string,int>::iterator first,last;
  statistics st; // statistics of common string length
  string  comstr, expr;
  unsigned int prestr_total;
  for(i=0;i<prestr_list.size();i++) {
    st = prestr_size[i]; // statistics of string list length
    // get string list
    map<string,int>& prestr = prestr_list[i];
    // calculate number of total string list
    // if prestr_total<SampleSize, '' is also a candidate
    prestr_total=0;
    for(first=prestr.begin();first!=prestr.end();++first) 
      prestr_total+=(*first).second;
    
    ////////////////////////////////////////////////////////////////
    if(st.mean()==0 && st.stddev()==0 /*prestr.size()==1*/) {
      /* Empty string, DO NOTHING */ 
    }
    ////////////////////////////////////////////////////////////////
    else if(st.nsample()==0 || (st.mean()==0 && st.stddev()==0)) { 
      /* Nothing, DO NOTHING */
    }
    ////////////////////////////////////////////////////////////////
    // single char 
    //else if(st.mean()==1 && st.stddev()==0) {
#undef max
    else if(st.max()==1) {
      expr += synthesize_charlist(prestr,prestr_total,SampleSize,mode);
    }
    ////////////////////////////////////////////////////////////////
    // simple string, this case should never happen
    else if(prestr.size()==1 && prestr.size()==SampleSize) { 
      first = prestr.begin();
      MakeSearchString((*first).first,expr);
    }
    ////////////////////////////////////////////////////////////////
    // incomplete string list
    else if(prestr_total<SampleSize) {
      if(mode==1 && prestr.size()<=(unsigned int)LIST_LIMIT) { 
	// string list + ''
	// there are both 'AAxxBB' & 'AAxxBByyZZ' ->  AA*BB(|yy1|yy2)(|ZZ)
	if(statmode) {
	  synthesize_stringlist_with_stat(expr,prestr,SampleSize);
	}
	else {
	  expr += "(|"; // Add '' as a choise
	  first = prestr.begin();
	  MakeSearchString((*first).first,expr,1); //expr += (*first).first;
	  for(++first;first!=prestr.end();++first) {
	    expr += "|";
	    MakeSearchString((*first).first,expr,1); //expr += (*first).first;
	  }
	  expr += ")";
	}
      }
      else {
	// string list + ''
	//  AAxxBB   AAxxBByyZZ ->  AA*BB(|yy1|yy2)(|ZZ)
	expr += "*";
      }
    }
    ////////////////////////////////////////////////////////////////
    else { // string list and number range
      if(mode==0) {
	expr += "*";
      }
      else if(MakeSearchNumberRange(prestr,st,expr)) {
      }
      else if(mode==1 && prestr.size()<=(unsigned int)LIST_LIMIT) { // This is error prone...
	if(statmode) {
	  synthesize_stringlist_with_stat(expr,prestr,SampleSize);
	}
	else {
	  expr += "(";
	  first = prestr.begin();
	  MakeSearchString((*first).first,expr,1); //expr += (*first).first;
	  for(++first;first!=prestr.end();++first) {
	    expr += "|";
	    MakeSearchString((*first).first,expr,1); //expr += (*first).first;
	  }
	  expr += ")";
	}
      }
      else {
	expr += "*";
      }
    }
    ////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////
    
    // common string
    if(i<comstr_list.size()) {
      if(prestr_total<SampleSize) {
	//printf("CCC prestr_total=%d prestr.size()=%d SampleSize=%d\n",prestr_total,prestr.size(),SampleSize);
	// there are both 'AAxxBB' & 'AAxxBByyZZ' ->  AA*BB(|yy1|yy2)(|ZZ)
	expr += "(|";
	comstr = comstr_list[i];
	MakeSearchString(comstr,expr,1);
	expr += ")";
      }
      else {
	// there are only 'AAxxBByyZZ' ->  AA*BB(yy1|yy2)ZZ
	comstr = comstr_list[i];
	MakeSearchString(comstr,expr);
      }
    }
  }
  return(expr);
}

///////////////////////////////////////////////////////////////////////////
#ifdef SYNTH_ITERATE
string EasyExpSynthesis_body2(//string& comstr,string& expr,
			     unsigned int SampleSize
			     ,vector<map<string,int> >& prestr_list 
			     ,const vector<statistics>& prestr_size 
			     ,const vector<string>& comstr_list
			     ,vector<int> mode
			     ,int LIST_LIMIT,int statmode=0) {
  unsigned int i;
  map<string,int>::iterator first,last;
  statistics st; // statistics of common string length
  string  comstr, expr;
  unsigned int prestr_total;
  for(i=0;i<prestr_list.size();i++) {
    st = prestr_size[i]; // statistics of string list length
    // get string list
    map<string,int>& prestr = prestr_list[i];
    // calculate number of total string list
    // if prestr_total<SampleSize, '' is also a candidate
    prestr_total=0;
    for(first=prestr.begin();first!=prestr.end();++first) 
      prestr_total+=(*first).second;
    
    ////////////////////////////////////////////////////////////////
    if(st.mean()==0 && st.stddev()==0 /*prestr.size()==1*/) {
      /* Empty string, DO NOTHING */ 
    }
    ////////////////////////////////////////////////////////////////
    else if(st.nsample()==0 || (st.mean()==0 && st.stddev()==0)) { 
      /* Nothing, DO NOTHING */
    }
    ////////////////////////////////////////////////////////////////
    // single char 
    //else if(st.mean()==1 && st.stddev()==0) {
    else if(st.max()==1) {
      expr += synthesize_charlist(prestr,prestr_total,SampleSize,mode[i]);
    }
    ////////////////////////////////////////////////////////////////
    // simple string, this case should never happen
    else if(prestr.size()==1 && prestr.size()==SampleSize) { 
      first = prestr.begin();
      MakeSearchString((*first).first,expr);
    }
    ////////////////////////////////////////////////////////////////
    // incomplete string list
    else if(prestr_total<SampleSize) {
      if(mode[i]==1 && prestr.size()<=(unsigned int)LIST_LIMIT) { 
	// string list + ''
	// there are both 'AAxxBB' & 'AAxxBByyZZ' ->  AA*BB(|yy1|yy2)(|ZZ)
	if(statmode) {
	  synthesize_stringlist_with_stat(expr,prestr,SampleSize);
	}
	else {
	  expr += "(|"; // Add '' as a choise
	  first = prestr.begin();
	  MakeSearchString((*first).first,expr,1); //expr += (*first).first;
	  for(++first;first!=prestr.end();++first) {
	    expr += "|";
	    MakeSearchString((*first).first,expr,1); //expr += (*first).first;
	  }
	  expr += ")";
	}
      }
      else {
	// string list + ''
	//  AAxxBB   AAxxBByyZZ ->  AA*BB(|yy1|yy2)(|ZZ)
	expr += "*";
      }
    }
    ////////////////////////////////////////////////////////////////
    else { // string list and number range
      if(mode[i]==0) {
	expr += "*";
      }
      else if(MakeSearchNumberRange(prestr,st,expr)) {
      }
      else if(mode[i]==1 && prestr.size()<=(unsigned int)LIST_LIMIT) { // This is error prone...
	if(statmode) {
	  synthesize_stringlist_with_stat(expr,prestr,SampleSize);
	}
	else if(prestr.size()==1) { // Fix 2015/11/24
	  first = prestr.begin();
	  MakeSearchString((*first).first,expr,1); //expr += (*first).first;
	}
	else {
	  expr += "(";
	  first = prestr.begin();
	  MakeSearchString((*first).first,expr,1); //expr += (*first).first;
	  for(++first;first!=prestr.end();++first) {
	    expr += "|";
	    MakeSearchString((*first).first,expr,1); //expr += (*first).first;
	  }
	  expr += ")";
	}
      }
      else {
	expr += "*";
      }
    }
    ////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////
    
    // common string
    if(i<comstr_list.size()) {
      if(prestr_total<SampleSize) {
	//printf("CCC prestr_total=%d prestr.size()=%d SampleSize=%d\n",prestr_total,prestr.size(),SampleSize);
	// there are both 'AAxxBB' & 'AAxxBByyZZ' ->  AA*BB(|yy1|yy2)(|ZZ)
	expr += "(|";
	comstr = comstr_list[i];
	MakeSearchString(comstr,expr,1);
	expr += ")";
      }
      else {
	// there are only 'AAxxBByyZZ' ->  AA*BB(yy1|yy2)ZZ
	comstr = comstr_list[i];
	MakeSearchString(comstr,expr);
      }
    }
  }
  return(expr);
}
#endif
///////////////////////////////////////////////////////////////////////////
class statsort_nsample_t {
public:
  unsigned int index;
  const statistics* pointer;
};
bool operator==(const statsort_nsample_t& a,const statsort_nsample_t& b)
{return(a.pointer->nsample()==b.pointer->nsample());}
bool operator!=(const statsort_nsample_t& a,const statsort_nsample_t& b)
{return(a.pointer->nsample()!=b.pointer->nsample());}
bool operator<(const statsort_nsample_t& a,const statsort_nsample_t& b)
{return(a.pointer->nsample()<b.pointer->nsample());}
bool operator>(const statsort_nsample_t& a,const statsort_nsample_t& b)
{return(a.pointer->nsample()>b.pointer->nsample());}

template<class InputContainer,class SortPair>
void sortstat(const InputContainer& in,vector<SortPair>& statsort) {
  SortPair pair;
  for(unsigned int i=0;i<in.size();i++) {
    pair.index = i;
    pair.pointer = &in[i];
    statsort.push_back(pair);
  }
  sort(statsort.begin(),statsort.end());
}

///////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
// public interface of EasyExp synthesis
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
string EasyExpSynthesis(const vector<string>& cmatchs
			,const vector<string>& cunmatchs
			,int generality_mode // 0:try'*',enumerate if fail
			,int LIST_LIMIT 
			,int position_mode) { //
  // prestr_list[0] comstr_list[0] prestr_list[1] comstr_list[1] ...
  // prestr_size[0] statistics     prestr_size[1] 
  vector<map<string,int> > prestr_list; 
  vector<statistics> prestr_size; 
  vector<string> comstr_list ; 

  // copy title string to cnext 
  vector<string> cnext;
  vector<int> cnext_count; // Here, cnext_count is empty 
  unsigned int i,j;
  for(i=0;i<cmatchs.size();i++) cnext.push_back(cmatchs[i]);

  // extract pre-string and common-string list
  //  prestr0 comstr0 prestr1 comstr1  ...  prestrN-1 comstrN-1 prestrN 
  SmartEasyExpExtraction_comstr(cnext,cnext_count,prestr_list,prestr_size,comstr_list
				,position_mode); // faster
  //EasyExpExtraction_comstr(cnext,prestr_list,prestr_size,comstr_list);

  // Synthesize regexp from pre-string, common-string list
  map<string,int>::iterator first,last;
  //map<string,int> prestr;
  statistics st; // statistics of common string length
  string  comstr, expr;
  EasyExp e;

#ifdef SYNTH_ITERATE
  vector<statsort_nsample_t> statsort;
  sortstat(prestr_size,statsort);
  int sortindex=0,modeindex=0;
  
  // initialize mode flags to all 0 -> everything goes to '*' '?'
  vector<int> mode; mode.resize(prestr_list.size()); 

  vector<string> unwantedmatch;
  for(j=generality_mode*mode.size();j<mode.size()*2+1;j++) { 
    expr=EasyExpSynthesis_body2(cmatchs.size()
				,prestr_list,prestr_size,comstr_list
				,mode,LIST_LIMIT,0);

    // check if there is unwanted matches from unmatch samples
    e.compile(expr.c_str());
    unwantedmatch=e.match(cunmatchs);
    if(0==unwantedmatch.size()) break;

    // seek next strategy and set mode flags
    if(sortindex<mode.size()) {
      if(modeindex==0) if(sortindex>0) mode[sortindex-1]=0;
      mode[sortindex++]=1;
    }
    else if(modeindex==0) {
      ++modeindex;
      sortindex=0;
    }
  }
#else // SYNTH_ITERATE
  for(j=generality_mode;j<2;j++) { // This is error prone...
    expr=EasyExpSynthesis_body(cmatchs.size()
			       ,prestr_list,prestr_size,comstr_list
			       ,j,LIST_LIMIT,0);
    
    e.compile(expr.c_str());
    if(0==e.match(cunmatchs).size()) break;
  }
#endif // SYNTH_ITERATE

  //if(j>=2) e.compile(expr.c_str());
  e.compile(expr.c_str());
  unsigned int matchsize = e.match(cmatchs).size();
  if(cmatchs.size()!=matchsize) {
    printf("Error: EasyExpExtraction(), %lu-%u=%lu unmatched samples\n"
	   ,cmatchs.size(),matchsize,cmatchs.size()-matchsize);
    printf(" expr=%s\n",expr.c_str());
#define E__DEBUG
#ifdef E__DEBUG
    //for(i=0;i<cmatchs.size();i++) printf("a %s\n",cmatchs[i].c_str());
    if(EasyExp_DEBUG>0) {
      vector<string> tmp=e.match(cmatchs);
      for(i=0;i<tmp.size();i++) printf("m %s\n",tmp[i].c_str());
      tmp=e._match(cmatchs);
      for(i=0;i<tmp.size();i++) printf("u %s\n",tmp[i].c_str());
    }
    if(EasyExp_DEBUG>0) e.disp();
    if(EasyExp_DEBUG>1) {
      for(i=0;i<prestr_list.size();i++) {
	printf("%d:",i);
	map<string,int>::iterator first,last=prestr_list[i].end();
	for(first=prestr_list[i].begin();first!=last;++first) 
	  printf(" %d '%s' ",(*first).second,(*first).first.c_str());
	printf("\n");
      }
      for(i=0;i<prestr_size.size();i++) printf("%d:%d,%g ",i,prestr_size[i].nsample(),prestr_size[i].mean());
      printf("\n");
      for(i=0;i<comstr_list.size();i++) printf("%d:%s ",i,comstr_list[i].c_str());
      printf("\n");
    }
#endif
  }

  return(expr);
}
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
// Following function is fast, but has problem for calculating probability
// slower but working version is defined in StatEasyExp.cxx
string EasyExpStatSynthesis(const vector<string>& cmatchs
			    ,const vector<string>& cunmatchs
			    ,int generality_mode // 0:try'*',enumerate if fail
			    ,int LIST_LIMIT 
			    ,int position_mode) { 
  // prestr_list[0] comstr_list[0] prestr_list[1] comstr_list[1] ...
  // prestr_size[0] statistics     prestr_size[1] 
  vector<map<string,int> > prestr_list;
  vector<statistics> prestr_size;
  vector<string> comstr_list ;

  // copy title string to cnext 
  vector<string> cnext;
  unsigned int i,j;
  for(i=0;i<cmatchs.size();i++) cnext.push_back(cmatchs[i]);

  // extract pre-string and common-string list
  //  prestr0 comstr0 prestr1 comstr1  ...  prestrN-1 comstrN-1 prestrN 
  EasyExpExtraction_comstr(cnext,prestr_list,prestr_size,comstr_list);
  //SmartEasyExpExtraction_comstr(cnext,prestr_list,prestr_size,comstr_list,position_mode); // there is problem

  // Synthesize regexp from pre-string, common-string list
  map<string,int>::iterator first,last;
  //map<string,int> prestr;
  statistics st; // statistics of common string length
  string  comstr, expr;
  EasyExp e;
  //for(j=0;j<generality_mode+1;j++) {
  for(j=generality_mode;j<2;j++) { // This is error prone... 
    expr=EasyExpSynthesis_body(cmatchs.size()
			       ,prestr_list,prestr_size,comstr_list
			       ,j,LIST_LIMIT,1);
  
    e.compile(expr.c_str());
    if(e.match(cunmatchs).size()==0) break;
  }

  unsigned int matchsize = e.match(cmatchs).size();
  if(cmatchs.size()!=matchsize) {
    printf("Error: EasyExpStatExtraction(), %lu-%d=%lu unmatched samples %s\n"
	   ,cmatchs.size(),matchsize,cmatchs.size()-matchsize,expr.c_str());
#ifdef E__DEBUG
    for(i=0;i<cmatchs.size();i++) printf("a %s\n",cmatchs[i].c_str());
    vector<string> tmp=e.match(cmatchs);
    printf("%lu %lu\n",cmatchs.size(),tmp.size());
    for(i=0;i<tmp.size();i++) printf("b %s\n",tmp[i].c_str());
#endif
  }

  return(expr);
}
////////////////////////////////////////////////////////////////////////////
#endif // EASYEXP_SYNTHESIS



