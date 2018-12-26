
/*****************************************************************************
* StatEasyExp.cxx
*
* Original regular expression library
*
* 2008/July/9 Masaharu Goto
*
*****************************************************************************/
/************************************************************************
 * Copyright(c) 1995~2015  Masaharu Goto (cint@pcroot.cern.ch)
 *
 * Permission to use, copy, modify and distribute this software and its 
 * documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation. The author makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 ************************************************************************/

#include "StatEasyExp.h"
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

#define MAKESTATEASYEXP
#ifdef MAKESTATEASYEXP
#include "Stat.h"
#endif
#include <limits.h>


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// StatEasyExp class
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
StatEasyExp::StatEasyExp(const char* pattern,Mode mode) {
  compile(pattern,mode);
}

//////////////////////////////////////////////////////////////////////////
StatEasyExp::~StatEasyExp() {
  clear_replace();
  StatEasyExp::clear();
}

//////////////////////////////////////////////////////////////////////////
void StatEasyExp::clear(int del) {
  deque<StatMatchElement*>::iterator first=m_element.begin();
  deque<StatMatchElement*>::iterator last =m_element.end();
  while(first!=last) {
    if(del || first!=m_element.begin()) delete (*first);
    ++first;
  }
  m_element.clear();
}

//////////////////////////////////////////////////////////////////////////
double StatEasyExp::match(const char* s,bool icase)  {
  argv.clear();
  argi.clear();
  if(!s) return(0);
  const char *p = s;
  deque<StatMatchElement*>::const_iterator first=m_element.begin();
  deque<StatMatchElement*>::const_iterator last =m_element.end();
  deque<StatMatchElement*>::const_iterator star = last;
  const char *tmp;
  int i=0,argstar = 0,argbackflag=0;
  double rate=1, prob=0;
  while(first!=last && p) {
    tmp = p;
    p = (*first)->match(prob,p,argv,argi,icase);
    //(*first)->disp();
    //printf("%g %g '%s'\n",rate,prob,p);
    rate *= prob;
    if(argbackflag && argv.size()>=2) {
      argv[argv.size()-2] = argv.back();
      argv.resize(argv.size()-1);
      argi[argi.size()-2] = argi.back();
      argi.resize(argi.size()-1);
      argbackflag=0;
    }
    if(!p) {
      if(star==last) {
	argv.clear();
	argi.clear();
	return(0);
      }
      else { 
        first=star; 
        star=last; 
        p = tmp; 
        string buf;
        for(unsigned int j=argstar;j<argv.size();j++) buf += argv[j];
        i = argstar;
        argv.resize(i);
        argv.push_back(buf);
        argi.resize(i);
        argi.push_back(0);
        argbackflag = 1;
      }
    }
    else {
      if( (*first)->isstar() ) {
        argstar = i;
        star=first; 
      }
      ++first;
      ++i;
    }
  }
  //printf("return rate %g %g \n",rate,prob);
  return(rate);
}
//////////////////////////////////////////////////////////////////////////
vector<string> StatEasyExp::match(const vector<string>& in,bool icase) {
  vector<string> result;
  for(unsigned int i=0;i<in.size();i++) {
    if(match(in[i].c_str(),icase)) result.push_back(in[i]);
  }
  return(result);
}

//////////////////////////////////////////////////////////////////////////
vector<string> StatEasyExp::_match(const vector<string>& in,bool icase) {
  vector<string> result;
  for(unsigned int i=0;i<in.size();i++) {
    if(!match(in[i].c_str(),icase)) result.push_back(in[i]);
  }
  return(result);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
#ifdef E__MULTIBYTE
char* StatEasyExp::strchr_multibyte(const char* s,int c) {
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
void StatEasyExp::compile(const char* pattern,Mode mode) {
  // clear Regular expression sequence
  clear();
//printf("mode=%d ",mode);
//StatEasyExp_DEBUG(pattern);

  // Auto mode judgement
  if(mode==Auto) {
#if defined(E__MULTIBYTE) || defined(E__MULTIBYTELIB)
    char *a = strchr_multibyte(pattern,'*');
    char *q = strchr_multibyte(pattern,'?');
    char *b = strchr_multibyte(pattern,'[');
    char *p = strchr_multibyte(pattern,'(');
    char *m = strchr_multibyte(pattern,'{');
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
    m_element.push_back(new StatMatchString(pattern));
    m_element.push_back(new StatMatchEnd());
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
      m_element.push_back(new StatMatchAnyChar());
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
    case '{':
      compile_brace(s,len,i,star);
      star=false;
      break;
    case 0:
      compile_end(star);
      star=false;
      return;
    default:
      if(compile_string(s,len,i,star)) return;
      star=false;
      break;
    }
  }
//disp();
}
//////////////////////////////////////////////////////////////////////////
bool StatEasyExp::compile_string(const char* s,int len,int& i,bool star) {
  string buf;
  bool theend = getstring(buf,s,len,i);
  if(buf.size()) {
    if(star) 
      m_element.push_back(new StatMatchStarString(buf));
    else
      m_element.push_back(new StatMatchString(buf));
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
bool StatEasyExp::getstring(string& buf,const char* s,int len,int& i) {
  int c;
  while((c=s[i])) {
    switch(c) {
    case '[':
    case '{':
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
bool StatEasyExp::compile_braket(const char* s,int len,int& i,bool star) {
  string charlist;
  deque<double> problist;
  int seq,seqend;
  bool negation = getcharlist(charlist,problist,s,len,i,seq,seqend);
  if(charlist.size()) {
    if(star) 
      if(negation) m_element.push_back(new StatMatchStarNotCharList(charlist,seq,seqend));
      else    m_element.push_back(new StatMatchStarCharList(charlist,problist,seq,seqend));
    else
      if(negation) m_element.push_back(new StatMatchNotCharList(charlist,seq,seqend));
      else    m_element.push_back(new StatMatchCharList(charlist,problist,seq,seqend));
  }
  return(false);
}
//////////////////////////////////////////////////////////////////////////
bool StatEasyExp::getcharlist(string& charlist,deque<double>& problist,const char* s,int len,int& i,int& seq,int& seqend) {
  int c;
  bool negation=false;
  int i1,i2;
  seq=1;
  seqend= -1;
  problist.clear();
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
  string sprob="";
  while((c=s[i])) {
    switch(c) {
    case ']':
      return(negation);
      break;
    case ';':
      sprob="";
      while(s[++i]!=';') sprob += s[i];
      while(problist.size()<charlist.size()) problist.push_back(1);
      problist.back() = atof(sprob.c_str());
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
	if(s[i]=='-') {
          seqend=0; ++i;
          while(isdigit(s[i])) {
            seqend = seqend*10 + s[i]-'0';
            ++i;
          }
          if(s[i]=='*') seqend= INT_MAX; 
	}
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
bool StatEasyExp::compile_parenthesis(const char* s,int len,int& i,bool star) {
  deque<string> strlist;
  deque<double> problist;
#ifdef NUMBER_IN_PARENTHESIS
  long min=0,max=0,padding;
  padding=getstringlist(strlist,problist,s,len,i,&min,&max);
  if(padding<0 && strlist.size()) {
    if(star) 
      m_element.push_back(new StatMatchStarStringList(strlist,problist));
    else
      m_element.push_back(new StatMatchStringList(strlist,problist));
  }
  else if(padding>=0) {
    if(star) 
      m_element.push_back(new StatMatchStarNumberRange(min,max,padding));
    else
      m_element.push_back(new StatMatchNumberRange(min,max,padding));
  }
#else
  getstringlist(strlist,s,len,i);
  if(strlist.size()) {
    if(star) 
      m_element.push_back(new StatMatchStarStringList(strlist));
    else
      m_element.push_back(new StatMatchStringList(strlist));
  }
#endif
  return(false);
}
//////////////////////////////////////////////////////////////////////////
#ifdef NUMBER_IN_PARENTHESIS
long StatEasyExp::getstringlist(deque<string>& strlist,deque<double>& problist
				,const char* s,int len,int& i,long* pmin,long* pmax) {
  int c, flag=0;
  ++i;
  string buf;
  string sprob;
  double blankprob = -1;
  while((c=s[i])) {
    switch(c) {
    case ';':
      sprob = "";
      while(s[++i]!='|' && s[i]!=')') sprob += s[i];
      if(buf.size()) {
	strlist.push_back(buf);
        while(problist.size()<strlist.size()) problist.push_back(1);
        problist.back() = atof(sprob.c_str());
      }
      else {
	blankprob=atof(sprob.c_str());
	++flag;
      }
      buf = "";
      if(s[i]==')') {
	if(flag) {
	  strlist.push_back("");
	  while(problist.size()<strlist.size()) problist.push_back(1);
	  problist.back() = atof(sprob.c_str());
	}
	return(-1);
      }
      break;
    case ')':
      if(buf.size()) strlist.push_back(buf);
      else ++flag;
      if(flag) {
	strlist.push_back("");
	while(problist.size()<strlist.size()) problist.push_back(1);
	problist.back() = atof(sprob.c_str());
      }
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
  if(flag) {
    strlist.push_back("");
    while(problist.size()<strlist.size()) problist.push_back(1);
    problist.back() = atof(sprob.c_str());
  }
  return(-1);
}
//////////////////////////////////////////////////////////////////////////
#else
bool StatEasyExp::getstringlist(deque<string>& strlist,const char* s,int len,int& i) {
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
#endif
//////////////////////////////////////////////////////////////////////////
bool StatEasyExp::compile_brace(const char* s,int len,int& i,bool star) {
  long min=0,max=0,padding;
  padding = getnumberrange(&min,&max,s,len,i);
  if(padding>=0) {
    if(star) 
      m_element.push_back(new StatMatchStarNumberRange(min,max,padding));
    else
      m_element.push_back(new StatMatchNumberRange(min,max,padding));
  }
  return(false);
}
//////////////////////////////////////////////////////////////////////////
long StatEasyExp::getnumberrange(long* pmin,long* pmax,const char* s,int len,int& i) {
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
    case '}':
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
void StatEasyExp::compile_end(bool star) {
  if(star) m_element.push_back(new StatMatchStarEnd());
  else     m_element.push_back(new StatMatchEnd());
}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void StatEasyExp::compile_replace(const char* r) {
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
    if(parseequal.argc>=2) {
      if(parseequal.argv[1][0]=='$') index=atoi(parseequal.argv[1]+1);
      else                           index=atoi(parseequal.argv[1]);
      compile_replace(index,parseequal.argv[2]);
    }
  }
}
//////////////////////////////////////////////////////////////////////////
void StatEasyExp::compile_replace(int i,const char* r) {
//printf("%d,%s\n",i,r);
  compile(r);
  if(m_element.size()>=1) {
    if((unsigned int)i>=m_replace.size()) m_replace.resize(i+1);
    m_replace[i] = m_element[0];
  }
  clear(0);
}
//////////////////////////////////////////////////////////////////////////
bool StatEasyExp::replace(const char *s,string& out,bool icase)  {
  bool m = match(s,icase);
  if(m) {
    out = "";
    const char *p;
    for(unsigned int i=0;i<argv.size();i++) {
      if(i<m_replace.size() && m_replace[i] && 
         (p=m_replace[i]->retrieve(argi[i]))) {
        out += p;
//printf("%d %s\n",i,p);
      }
      else {
        out += argv[i];
      }
    }
  }
  else {
    out = s;
  }
  return(m);
}
//////////////////////////////////////////////////////////////////////////
void StatEasyExp::clear_replace() {
  deque<StatMatchElement*>::iterator first=m_replace.begin();
  deque<StatMatchElement*>::iterator last =m_replace.end();
  while(first!=last) {
    delete (*first);
    ++first;
  }
  m_replace.clear();
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
void StatEasyExp::disp(FILE* fp) const {
  deque<StatMatchElement*>::const_iterator first=m_element.begin();
  deque<StatMatchElement*>::const_iterator last =m_element.end();
  while(first!=last) {
    (*first)->disp(fp);
    ++first;
  }
  printf("%lu-%lu ",argv.size(),argi.size());
  for(unsigned int i=0;i<argv.size();i++) {
    printf("%s(%d) | ",argv[i].c_str(),argi[i]);
  }
  printf("\n");

  first=m_replace.begin();
  last =m_replace.end();
  while(first!=last) {
    if(*first) (*first)->disp(fp);
    else       fprintf(fp,"0\n");
    ++first;
  }
  printf("\n");
}

#ifdef E__MULTIBYTE
/*****************************************************************************
* IsDBCSLeadByte()
*****************************************************************************/
int StatEasyExp::IsDBCSLeadByte(int c) {
  return((0x80&c)&&E__EUC!=lang&&CodingSystem(c)) ;
}

/*****************************************************************************
* IsDBCSLeadByte()
*****************************************************************************/
void StatEasyExp::CheckDBCS2ndByte(int c) {
  if(0==(0x80&c)) lang=E__SJIS;
}

/***********************************************************************
* CodingSystem()
***********************************************************************/
int StatEasyExp::CodingSystem(int c)
{
  if(-33>=c && -79<=c) return(0); // half kana
  c &= 0x7f;
  switch(lang) {
  case E__UNKNOWNCODING:
    if(0x1f<c&&c<0x60) {
      /* assuming there is no half-sized kana chars, this code does not
       * exist in S-JIS, set EUC flag and return 0 */
      lang=E__EUC;
      return(0); 
    }
    return(1); /* assuming S-JIS but not sure yet */
  case E__EUC:
    return(0);
  case E__SJIS:
    if(c<=0x1f || (0x60<=c && c<=0x7c)) return(1);
    else                                return(0);
  case E__ONEBYTE: 
    return(0);
  }
  return(1);
}

#endif

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// Utitity function
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
int matchstatregex(const char *pattern,const char *string) {
  StatEasyExp StatEasyExpr(pattern);
  return(StatEasyExpr.match(string));
}


//////////////////////////////////////////////////////////////////////////
int operator==(StatEasyExp& ex,const char *string) {
  return(ex.match(string));
}

//////////////////////////////////////////////////////////////////////////
int operator!=(StatEasyExp& ex,const char *string) {
  return(!ex.match(string));
}

//////////////////////////////////////////////////////////////////////////
int operator==(const char *string,StatEasyExp& ex) {
  return(ex.match(string));
}

//////////////////////////////////////////////////////////////////////////
int operator!=(const char *string,StatEasyExp& ex) {
  return(!ex.match(string));
}



//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
void StatEasyExp::help() {
  printf("StatEasy Expression definition\n");
  printf(" expr <>*----- atom\n");
  printf(" atom\n");
  printf("  *        : Any string of >=0 length\n");
  printf("  ?        : Any character\n");
  printf("  [12aeg]  : An character match with listed candidiate\n");
  printf("  [a-z0-9] : An character match with ranged candidate\n");
  printf("  [^12ea]  : An character not match with listed candidate\n");
  printf("  [^a-z]   : An character not match with ranged candidate\n");
  printf("  [12aeg:5]: 5 characters match with listed candidiate\n");
  printf("  [12aeg:2-5]: 2 to 5 characters match with listed candidiate\n");
  printf("  [12aeg:2-*]: 2 to infinit characters match with listed candidiate\n");
  printf("  (ab|cd|ef)          : Match with string candidate\n");
  printf("  (ab;0.1|cd;0.2|ef;0.25)  : Match with string candidate + probability\n");
  printf("  {5-20}    (5..20)   : Match with number range\n");
  printf("  {05-20}   (05..20)  : Match with number range with length of 2\n");
  printf("  {05-20:2} (05..20:2): Match with number range with length of 2\n");
  printf("  {21-35:2} (21..35:2): Match with number range with length of 2\n");
}

#ifdef MAKESTATEASYEXP
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
// Statistical EasyExp Synthesis
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
string StatEasyExpExtraction(const vector<string>& cmatchs
			 ,const vector<string>& cunmatchs
			 ,int mode,int LIST_LIMIT) {
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

  // Synthesize regexp from pre-string, common-string list
  map<string,int>::iterator first,last;
  map<string,int> prestr;
  statistics st;
  string  comstr, expr;
  StatEasyExp e;
  int ch;
  //for(j=0;j<mode+1;j++) {
  for(j=mode;j<2;j++) { // This is error prone...
    for(i=0;i<prestr_list.size();i++) {
      // prestring
      prestr = prestr_list[i];
      st = prestr_size[i];
      if(j==1 && prestr.size()<=LIST_LIMIT) { // This is error prone...
	// if there are only few (<=5) choises, list them all
        if(st.mean()==0 && st.stddev()==0 /*prestr.size()==1*/) {
           /* Empty string, DO NOTHING */ 
        }
        else if(st.nsample()==0 || (st.mean()==0 && st.stddev()==0)) { 
           /* Nothing, DO NOTHING */
        }
        else if(st.mean()==1 && st.stddev()==0) { /* single char */
          double vmax=0, vsum=0,v,count;
          char buf[100];
          for(first=prestr.begin();first!=prestr.end();++first) {
            v=(*first).second;
            if(vmax<v) vmax=v;
            vsum+=v;
          }
          expr += "[";
          for(first=prestr.begin();first!=prestr.end();++first) {
            ch = (*first).first[0];
            switch(ch) {
            case '[': case ']': case '^': case '\\':case '-':case ':':case ';':
              expr += "\\";
            default:
              expr += ch;
              break;
            }
            count = (*first).second;
	    sprintf(buf,";%f;",count/vsum);
            expr += buf;
          }
          expr += "]";
        }
        else if(prestr.size()==1) { // simple string
          first = prestr.begin();
          //expr += (*first).first;
          MakeSearchString((*first).first,expr);
        }
        else { // string list 
          double vmax=0, vsum=0,v,count;
          char buf[100];
          for(first=prestr.begin();first!=prestr.end();++first) {
            v=(*first).second;
            if(vmax<v) vmax=v;
            vsum+=v;
          }
          expr += "(";
          first = prestr.begin();
          //expr += (*first).first;
          MakeSearchString((*first).first,expr);
          count = (*first).second;
	  sprintf(buf,";%f",count/vsum);
          expr += buf;
          for(++first;first!=prestr.end();++first) {
            expr += "|";
            //expr += (*first).first;
            MakeSearchString((*first).first,expr);
            count = (*first).second;
	    sprintf(buf,";%f",count/vsum);
            expr += buf;
          }
          expr += ")";
        }
      }
      else { // if there are many choises
        if(st.mean()==0 && st.stddev()==0 /*prestr.size()==1*/) {
           /* Empty string, DO NOTHING */ 
        }
        else if(st.nsample()==0 || (st.mean()==0 && st.stddev()==0)) { 
           /* Nothing, DO NOTHING */
        }
        else if(st.mean()==1 && st.stddev()==0) { /* single char */
          expr += "?";
	}
        else if(prestr.size()==1) { // simple string
          first = prestr.begin();
          //expr += (*first).first;
          MakeSearchString((*first).first,expr);
	}
        else { // string list 
	  // check if this is a number list
	  if(!MakeSearchNumberRange(prestr,st,expr)) {
            // this is not a number list
            expr += "*";
          }
	}
      }
  
      // common string
      if(i<comstr_list.size()) {
        comstr = comstr_list[i];
        MakeSearchString(comstr,expr);
      }
    }
  
    //printf("%s\n",expr.c_str());
    e.compile(expr.c_str());
    if(e.match(cunmatchs).size()==0) break;
    if(j<1) expr = ""; // This is error prone...
  }

  int matchsize = e.match(cmatchs).size();
  if(cmatchs.size()!=matchsize) {
    printf("Error: EasyExpExtraction(), %d-%d=%d unmatched samples %s\n"
	   ,cmatchs.size(),matchsize,cmatchs.size()-matchsize,expr.c_str());
  }

  return(expr);
}
////////////////////////////////////////////////////////////////////////////
#endif // MAKESTATEASYEXP



