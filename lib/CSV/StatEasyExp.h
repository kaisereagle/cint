/*****************************************************************************
* StatEasyExp.h
*
* Original easy expression library
*
* 2008/July/9 Masaharu Goto
*
* Easy Expression definition
*
*   expr Ÿ*----- atom
*
*   atom
*    *        : Any string of >=0 length
*    ?         : Any character
*    [12aeg]  : An character match with listed candidiate
*    [a-z0-9] : An character match with ranged candidate
*    [^12ea]  : An character not match with listed candidate
*    [^a-z]   : An character not match with ranged candidate
*    [12aeg:5]: 5 characters match with listed candidiate
*    [12aeg:2-5]: 2 to 5 characters match with listed candidiate
*    [12aeg:2-*]: 2 to infinit characters match with listed candidiate
*    [12aeg:*]  : * characters match with listed candidiate -> [12aeg]*
*    (ab|cd|ef)           : Match with string candidate
*    (ab;0.1|cd;0.2|ef;0.25) : Match with string candidate + probability
*    {5-20}    (5..20)    : Match with number range
*    {05-20}   (05..20)   : Match with number range with length of 2
*    {05-20:2} (05..20:2) : Match with number range with length of 2 
*    {21-35:2} (21..35:2) : Match with number range with length of 2
*  
* Easy Expression Replace definition
*
*   patchpattern    : expr
*   replacepattern  : expr
*
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

#ifndef STATSTREXPH
#define STATSTREXPH

#if defined(__CINT__) && !defined(__MAKECINT__)
#pragma include_noerr <StatEasyExp.dll>
#endif


#define E__REPLACE2
#define NUMBER_IN_PARENTHESIS
#define MULTICHARLIST
#define DIRHIERARCHY
#define CHARLISTRANGE
#define E__ARGS

#if defined(__linux__) && !defined(__CINT__)
#include <strings.h>
#elif defined(_WIN32)
#define strncasecmp strnicmp
//#define strncasecmp _strnicmp
#define strcasecmp stricmp
#define strcasestr stristr
char* stristr(const char* a,const char* b);
#endif

#include "EasyExp.h"
#include "MultiByteChar.h"

#ifndef E__MULTIBYTELIB
#ifndef E__MULTIBYTE
#define E__MULTIBYTE
#endif
#endif

#ifdef E__MULTIBYTE
#define E__UNKNOWNCODING 0
#define E__EUC           1
#define E__SJIS          2
#define E__JIS           3
#define E__ONEBYTE       4 
#endif


#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <deque>
#include <vector>
#include <map>
using namespace std;

//////////////////////////////////////////////////////////////////////////
//static string toupper(const string& a) ;
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// Match element bass class
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
class StatMatchElement {
 public:
  StatMatchElement() { }
  virtual ~StatMatchElement() {}
  virtual const char* match(double& prob,const char* s,vector<string>& argv,vector<int>& argi,bool icase=false) =0;
  virtual const char* retrieve(int i) { return(0); }
  virtual void disp(FILE* fp=stdout) const=0;
  virtual bool isstar() const { return(false); }

};

//////////////////////////////////////////////////////////////////////////
// StatMatch element derived classes
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//  'abcd'
class StatMatchString : public StatMatchElement {
 public:
  StatMatchString(const string& exp) : m_s(exp) {}
  virtual ~StatMatchString() {}
  virtual const char* match(double& prob,const char* s,vector<string>& argv,vector<int>& argi,bool icase=false) {
    if(icase) {
      if(s && 0==strncasecmp(s,m_s.c_str(),m_s.size())) {
	  argv.push_back(string(s,m_s.size()));
	  argi.push_back(0);
	  prob = 1;
	  return(s+m_s.size());
      }
    }
    else {
      if(s && 0==strncmp(s,m_s.c_str(),m_s.size())) {
	  argv.push_back(string(s,m_s.size()));
	  argi.push_back(0);
	  prob = 1;
	  return(s+m_s.size());
      }
    }
    prob = 0;
    return 0;
  }
  virtual const char* retrieve(int i) {
    return(m_s.c_str());
  }
  virtual void disp(FILE* fp=stdout) const { fprintf(fp,"%-15s'%s'\n","String",m_s.c_str()); }
 private:
  string m_s;

};
//////////////////////////////////////////////////////////////////////////
//  '*abc'
class StatMatchStarString : public StatMatchElement {
 public:
  StatMatchStarString(const string& exp) : m_s(exp) {}
  virtual ~StatMatchStarString() {}
  virtual const char* match(double& prob,const char* s,vector<string>& argv,vector<int>& argi,bool icase=false) {
    if(s) { 
      char *p;
      if(icase) p = strcasestr((char*)s,(char*)m_s.c_str());
      else      p = strstr((char*)s,(char*)m_s.c_str());
      if(p) {
	  argv.push_back(string(s,p-s));
	  argv.push_back(string(p,m_s.size()));
	  argi.push_back(0);
	  argi.push_back(0);
	  prob = 1;
	  return(p+m_s.size());
      }
      else {
	prob = 0;
	return(0);
      }
    }
    else {
      prob = 0;
      return 0;
    }
  }
  virtual void disp(FILE* fp=stdout) const { fprintf(fp,"%-15s'%s'\n","*String",m_s.c_str()); }
  virtual bool isstar() const { return(true); }
 private:
  string m_s;

};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//  '(ab|cd|ef)'
class StatMatchStringList : public StatMatchElement {
 public:
  StatMatchStringList(const deque<string>& exp,const deque<double>& prob)
    : m_s(exp), m_p(prob) {}
  virtual ~StatMatchStringList() {}
  virtual const char* match(double& prob,const char* s,vector<string>& argv,vector<int>& argi,bool icase=false) {
    if(!s) {
      prob = 0;
      return(0);
    }
    deque<string>::iterator first =m_s.begin();
    deque<string>::iterator last  =m_s.end();
    int i=0;
    while(first!=last) {
      if(icase) {
	if(0==strncasecmp(s,(*first).c_str(),(*first).size())) {
	  argv.push_back(string(s,(*first).size()));
	  argi.push_back(i);
	  prob = m_p[i];
	  return(s+(*first).size());
	}
      }
      else {
	if(0==strncmp(s,(*first).c_str(),(*first).size())) {
	  argv.push_back(string(s,(*first).size()));
	  argi.push_back(i);
	  prob = m_p[i];
	  return(s+(*first).size());
        }
      }
      ++first;
      ++i;
    }
    prob = 0;
    return(0);
  }
  virtual const char* retrieve(int i) {
    if((unsigned int)i<m_s.size()) return(m_s[i].c_str());
    else return(0);
  }
  virtual void disp(FILE* fp=stdout) const { 
    fprintf(fp,"%-15s","StringList");
    for(unsigned int i=0;i<m_s.size();i++) fprintf(fp,"'%s'%f,"
						   ,m_s[i].c_str()
						   ,m_p[i]);
    fprintf(fp,"\n");
  }
 private:
  deque<string> m_s;
  deque<double> m_p;
};
//////////////////////////////////////////////////////////////////////////
//  '*(ab|cd|ef)'
class StatMatchStarStringList : public StatMatchElement {
 public:
 StatMatchStarStringList(const deque<string>& exp,const deque<double>& prob)
   : m_s(exp) ,m_p(prob) {}
  virtual ~StatMatchStarStringList() {}
  virtual const char* match(double& prob,const char* s,vector<string>& argv,vector<int>& argi,bool icase=false) {
    char* tmp;
    char* p=0;
    int len=0;
    if(!s) {
      prob = 0;
      return(0);
    }
    deque<string>::iterator first =m_s.begin();
    deque<string>::iterator last  =m_s.end();
    int i=0,j=0;
    while(first!=last) {
      if(icase) tmp = strcasestr((char*)s,(char*)(*first).c_str());
      else      tmp = strstr((char*)s,(char*)(*first).c_str());
      if(tmp && (!p || tmp<p) ) { 
	p=tmp; len=(*first).size(); 
	j=i;
      }
      ++first;
      ++i;
    }
    if(p) {
      argv.push_back(string(s,p-s)); 
      argv.push_back(string(p,len));
      argi.push_back(0);
      argi.push_back(j);
      prob = m_p[j];
      return(p+len);
    }
    else {
      prob = 0;
      return(0);
    }
  }
  virtual void disp(FILE* fp=stdout) const { 
    fprintf(fp,"%-15s","*StringList");
    for(unsigned int i=0;i<m_s.size();i++)
      fprintf(fp,"'%s'," ,m_s[i].c_str() /* ,m_p[i] */ );
    fprintf(fp,"\n");
  }
  virtual bool isstar() const { return(true); }
 private:
  deque<string> m_s;
  deque<double> m_p;
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//  '[0-1a-f]'  '[0-1a-f:*]' '[0-1a-f:15]'
class StatMatchCharList : public StatMatchElement {
 public:
 StatMatchCharList(const string& exp,const deque<double>& prob,int seq=1,int seqend=-1)
   : m_charlist(exp), m_p(prob), m_seq(seq), m_seqend(seqend) {}
  virtual ~StatMatchCharList() {}
  virtual const char* match(double& prob,const char* s,vector<string>& argv,vector<int>& argi,bool icase=false) {
    string tmp;
    string charlist;
    if(icase) charlist = toupper(m_charlist);
    else      charlist = m_charlist;
    int i=0;
    if(m_seq == -1) {
      if(icase) tmp = toupper(s+i);
      else      tmp = s+i;
      while(s&&s[i]&& 0==tmp.find_first_of(charlist)) { ++i; tmp = s+i; }
    }
    else {
      for(i=0;i<m_seq;i++) {
        if(icase) tmp = toupper(s+i);
        else      tmp = s+i;
        if  (s&&s[i]&& 0==tmp.find_first_of(charlist)) { }
        else {
	  prob = 0;
	  return(0);
	}
      }
      if(m_seqend != -1) {
        for(;i<m_seqend;i++) {
          if(icase) tmp = toupper(s+i);
          else      tmp = s+i;
          if  (s&&s[i]&& 0==tmp.find_first_of(charlist)) { }
          else break;
	}
      }
    }
    argv.push_back(string(s,i));
    int index = charlist.find_first_of(argv.back()[0]); 
    argi.push_back(index);
    prob = m_p[index];
    return(s+i);
  }
  virtual const char* retrieve(int i) {
    if((unsigned int)i<m_charlist.size()) {
      m_buf[0] = m_charlist[i];
      m_buf[1] = 0;
      return(m_buf);
    }
    else return(0);
  }
  virtual void disp(FILE* fp=stdout) const {
    fprintf(fp,"%-15s'%s' %d - %d","[a-z] ",m_charlist.c_str(),m_seq,m_seqend);
    for(int i=0;i<m_p.size();i++) fprintf(fp," %f",m_p[i]);
    fprintf(fp,"\n");
  }
 private:
  string m_charlist;
  deque<double> m_p;
  int    m_seq;
  int    m_seqend;
  char   m_buf[3];

};
//////////////////////////////////////////////////////////////////////////
//  '*[0-1a-f]'  '*[0-1a-f:*]' '*[0-1a-f:15]'
class StatMatchStarCharList : public StatMatchElement {
 public:
 StatMatchStarCharList(const string& exp,const deque<double>& prob,int seq=1,int seqend=-1)
   : m_charlist(exp), m_p(prob), m_seq(seq), m_seqend(seqend) {}
  virtual ~StatMatchStarCharList() {}
  virtual const char* match(double& prob,const char* s,vector<string>& argv,vector<int>& argi,bool icase=false) {
    string tmp;
    if(icase)  tmp=toupper(s);
    else       tmp=s;
    string charlist;
    if(icase) charlist=toupper(m_charlist);
    else      charlist=m_charlist;
    unsigned int pos=tmp.find_first_of(charlist);
    if(s&&s[0]&& /* 0<=pos && */ pos<tmp.size()) {
      int i=(int)pos;
      if(m_seq == -1) {
        tmp = s+i;
        while(s&&s[i]&& 0==tmp.find_first_of(charlist)) { ++i; tmp = s+i; }
      }
      else {
        for(i=(int)pos;i<m_seq+(int)pos;i++) {
          tmp = s+i;
          if(s&&s[i]&& 0==tmp.find_first_of(charlist)) { }
          else {
	    prob = 0;
	    return(0);
	  }
        }
        if(m_seqend != -1) {
          for(;i<m_seqend;i++) {
            if(icase) tmp = toupper(s+i);
            else      tmp = s+i;
            if  (s&&s[i]&& 0==tmp.find_first_of(charlist)) { }
            else break;
	  }
        }
      }
      argv.push_back(string(s,pos));
      argv.push_back(string(s+pos,i-pos));
      argi.push_back(0);
      int index = charlist.find_first_of(argv.back()[0]); 
      argi.push_back(index);
      prob = m_p[index];
      return(s+i);
    }
    else {
      prob = 0;
      return 0;
    }
  }
  virtual void disp(FILE* fp=stdout) const {
    fprintf(fp,"%-15s'%s' %d - %d","*[a-z]",m_charlist.c_str(),m_seq,m_seqend);
    for(int i=0;i<m_p.size();i++) fprintf(fp," %f",m_p[i]);
    fprintf(fp,"\n");
  }
  virtual bool isstar() const { return(true); }
 private:
  string m_charlist;
  deque<double> m_p;
  int    m_seq;
  int    m_seqend;

};


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//  '[^0-1a-f]'  '[^0-1a-f:*]' '[^0-1a-f:15]'
class StatMatchNotCharList : public StatMatchElement {
 public:
  StatMatchNotCharList(const string& exp,int seq=1,int seqend=-1)
    : m_charlist(exp), m_seq(seq), m_seqend(seqend) {}
  virtual ~StatMatchNotCharList() {}
  virtual const char* match(double& prob,const char* s,vector<string>& argv,vector<int>& argi,bool icase=false) {
    string tmp;
    string charlist;
    if(icase) charlist=toupper(m_charlist);
    else      charlist=m_charlist;
    int i=0;
    if(m_seq == -1) {
      if(icase) tmp = toupper(s+i);
      else      tmp = s+i;
      while(s&&s[i]&& 0==tmp.find_first_not_of(charlist)) { ++i; tmp = s+i; }
    }
    else {
      for(i=0;i<m_seq;i++) {
        if(icase) tmp = toupper(s+i);
        else      tmp = s+i;
        if(s&&s[i]&& 0==tmp.find_first_not_of(charlist)) { }
        else {
	  prob = 0;
	  return(0);
	}
      }
      if(m_seqend != -1) {
        for(;i<m_seqend;i++) {
          if(icase) tmp = toupper(s+i);
          else      tmp = s+i;
          if  (s&&s[i]&& 0==tmp.find_first_not_of(charlist)) { }
          else break;
        }
      }
    }
    argv.push_back(string(s,i));
    argi.push_back(0); 
    prob = 1;
    return(s+i);
  }
  virtual void disp(FILE* fp=stdout) const { fprintf(fp,"%-15s'%s' %d - %d\n","[^a-z]",m_charlist.c_str(),m_seq,m_seqend); }
 private:
  string m_charlist;
  int m_seq;
  int m_seqend;

};
//////////////////////////////////////////////////////////////////////////
//  '*[^0-1a-f]'  '*[^0-1a-f:*]'  '*[^0-1a-f:15]'
class StatMatchStarNotCharList : public StatMatchElement {
 public:
  StatMatchStarNotCharList(const string& exp,int seq=1,int seqend=-1) : m_charlist(exp), m_seq(seq), m_seqend(seqend) {}
  virtual ~StatMatchStarNotCharList() {}
  virtual const char* match(double& prob,const char* s,vector<string>& argv,vector<int>& argi,bool icase=false) {
    string tmp;
    if(icase) tmp=toupper(s);
    else      tmp=s;
    string charlist;
    if(icase) charlist=toupper(m_charlist);
    else      charlist=m_charlist;
    unsigned int pos=tmp.find_first_not_of(charlist);
    if(s&&s[0]&& /* 0<=pos && */ pos<tmp.size()) {
      int i=(int)pos;
      if(m_seq == -1) {
        tmp = s+i;
        while(s&&s[i]&& 0==tmp.find_first_not_of(charlist)) { ++i; tmp = s+i; }
      }
      else {
        for(i=(int)pos;i<m_seq+(int)pos;i++) {
          tmp = s+i;
          if(s&&s[i]&& 0==tmp.find_first_not_of(charlist)) { }
          else {
	    prob = 0;
	    return(0);
	  }
        }
        if(m_seqend != -1) {
          for(;i<m_seqend;i++) {
            if(icase) tmp = toupper(s+i);
            else      tmp = s+i;
            if  (s&&s[i]&& 0==tmp.find_first_not_of(charlist)) { }
            else break;
          }
        }
      }
      argv.push_back(string(s,pos));
      argv.push_back(string(s+pos,i-pos));
      argi.push_back(0);
      argi.push_back(0); 
      prob = 1;
      return(s+i);
    }
    else {
      prob = 0;
      return 0;
    }
  }
  virtual void disp(FILE* fp=stdout) const { fprintf(fp,"%-15s'%s' %d - %d\n","*[^a-z]",m_charlist.c_str(),m_seq,m_seqend); }
  virtual bool isstar() const { return(true); }
 private:
  string m_charlist;
  int m_seq;
  int m_seqend;
};

//////////////////////////////////////////////////////////////////////////
//  '{05-21L2}'  '{05-21:2}' 
class StatMatchNumberRange: public StatMatchElement {
 public:
  StatMatchNumberRange(long min,long max,long padding) : m_min(min), m_max(max), m_padding(padding) { }
  virtual ~StatMatchNumberRange() {}
  virtual const char* match(double& prob,const char* s,vector<string>& argv,vector<int>& argi,bool icase=false) {
    string str;
    if(!s) {
      prob = 0;
      return(0);
    }
    int i=0;
    while(s[i] && isdigit(s[i]) && (0==m_padding || (long)str.size()<m_padding)) { str+=s[i]; ++i; }
    if(!str.size()) {
      prob = 0;
      return(0);
    }
    int num = atoi(str.c_str());
    if(m_min<=num && num<=m_max && (0==m_padding || m_padding==(long)str.size())) {
      argv.push_back(string(s,i));
      argi.push_back(num-m_min);
      prob = 1;
      return(s+i);
    }
    prob = 0;
    return(0);
  }
  virtual const char* retrieve(int i) {
    sprintf(m_buf,"%ld",m_min+i);
    return(m_buf);
  }
  virtual void disp(FILE* fp=stdout) const { fprintf(fp,"%-15s min=%ld max=%ld padding=%ld\n","NumberRange",m_min,m_max,m_padding); }
 private:
  long m_min;
  long m_max;
  long m_padding;
  char m_buf[20];

};
//////////////////////////////////////////////////////////////////////////
//  '*{05-21L2}'   '*{05-21:2}' 
class StatMatchStarNumberRange: public StatMatchElement {
 public:
  StatMatchStarNumberRange(long min,long max,long padding) : m_min(min), m_max(max), m_padding(padding) { }
  virtual ~StatMatchStarNumberRange() {}
  virtual const char* match(double& prob,const char* s,vector<string>& argv,vector<int>& argi,bool icase=false) {
    string str;
    if(!s) {
      prob = 0;
      return(0);
    }
    int i=0;
    int pos=0;      
    do {
      while(s[i] && !isdigit(s[i])) { ++i; }
      pos=i;      
      while(s[i] && isdigit(s[i]) && (m_padding==0 || (long)str.size()<m_padding)) { str+=s[i]; ++i; }
      if(!str.size()) {
	prob = 0;
	return(0);
      }
      int num = atoi(str.c_str());
      if(m_min<=num && num<=m_max && (0==m_padding || m_padding==(long)str.size())) {
        argv.push_back(string(s,pos)); 
        argv.push_back(string(s+pos,i-pos)); 
	argi.push_back(0);
	argi.push_back(num-m_min);
	prob = 1;
        return(s+i);
      }
      str="";
    } while(s[i]);
    prob = 0;
    return(0);
  }
  virtual void disp(FILE* fp=stdout) const { fprintf(fp,"%-15s min=%ld max=%ld padding=%ld\n","*NumberRange",m_min,m_max,m_padding); }
  virtual bool isstar() const { return(true); }
 private:
  long m_min;
  long m_max;
  long m_padding;

};

//////////////////////////////////////////////////////////////////////////
//   ''
class StatMatchEnd : public StatMatchElement {
 public:
  StatMatchEnd() {}
  virtual ~StatMatchEnd() {}
  virtual const char* match(double& prob,const char* s,vector<string>& argv,vector<int>& argi,bool icase=false) {
   if(s && s[0]==0) {
     prob = 1;
     return(s);
    }
   else {
     prob = 1;
     return 0;
   }
  }
  virtual void disp(FILE* fp=stdout) const { fprintf(fp,"%-15s'%s'\n","End",""); }

};
//////////////////////////////////////////////////////////////////////////
//  '*'
class StatMatchStarEnd : public StatMatchElement {
 public:
  StatMatchStarEnd() {}
  virtual ~StatMatchStarEnd() {}
  virtual const char* match(double& prob,const char* s,vector<string>& argv,vector<int>& argi,bool icase=false) {
    if(s) { 
      int len=strlen(s);
      argv.push_back(s);
      argi.push_back(0);
      prob = 1;
      return(s+len);
    }
    else {
      prob = 1;
      return 0;
    }
  }
  virtual void disp(FILE* fp=stdout) const { fprintf(fp,"%-15s'%s'\n","*End",""); }
  virtual bool isstar() const { return(true); }

};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//  '?'
class StatMatchAnyChar : public StatMatchElement {
 public:
  StatMatchAnyChar() {}
  virtual ~StatMatchAnyChar() {}
  virtual const char* match(double& prob,const char* s,vector<string>& argv,vector<int>& argi,bool icase=false) {
    if(s&&s[0]) {
      argv.push_back(string(s,1));
      argi.push_back(0);
      prob = 1;
      return(s+1); 
    }
    else {
      prob = 0;
      return 0;
    }
  }
  virtual void disp(FILE* fp=stdout) const { fprintf(fp,"%-15s'%s'\n","?",""); }

};
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// Regular Expression class
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
class StatEasyExp 
#ifdef E__MULTIBYTELIB
 : public G__MultiByteMode
#endif
{
 public:
  enum Mode { Auto, RegEx , Exact };

  StatEasyExp() {} 
  StatEasyExp(const char* pattern,Mode mode=Auto);
  ~StatEasyExp();

  void clear(int del=1);
  void clear_replace();
  void compile(const char* pattern,Mode mode=Auto);
  double match(const char *s,bool icase=false)  ;
  vector<string>  match(const vector<string>& in,bool icase=false)  ;
  vector<string> _match(const vector<string>& in,bool icase=false)  ;
  void disp(FILE* fp=stdout) const;

  void help() ;

 public:
  void compile_replace(const char* pattern,const char* r,Mode mode=Auto) {
    clear_replace();
    compile_replace(r);
    compile(pattern,mode);
  }
  bool replace(const char *s,string& out,bool icase=false)  ;
 private:
  void compile_replace(const char* r);
  void compile_replace(int i,const char* r);

 private:
  bool compile_string(const char* s,int len,int& i,bool star) ;
  bool getstring(string& buf,const char* s,int len,int& i) ;
  bool compile_braket(const char* s,int len,int& i,bool star) ;
  bool getcharlist(string& charlist,deque<double>& problist,const char* s,int len,int& i,int& seq,int& seqend) ;
  bool compile_parenthesis(const char* s,int len,int& i,bool star) ;
  bool getstringlist(deque<string>& strlist,const char* s,int len,int& i) ;
#ifdef NUMBER_IN_PARENTHESIS
  long getstringlist(deque<string>& strlist,deque<double>& problist,const char* s,int len,int& i,long* pmin,long* pmax) ;
#endif
  bool compile_brace(const char* s,int len,int& i,bool star) ;
  long getnumberrange(long* pmin,long* pmax,const char* s,int len,int& i) ;
  void compile_end(bool star) ;

 private:
  deque<StatMatchElement*> m_element;

 public:
  vector<string> argv;
 private:
  vector<int> argi;
  deque<StatMatchElement*> m_replace;

#ifdef E__MULTIBYTE
 private:
  int lang;
  int IsDBCSLeadByte(int c) ;
  void CheckDBCS2ndByte(int c) ;
  int CodingSystem(int c) ;
  char* strchr_multibyte(const char* s,int c);
 public:
  void _SetCodingSystem(int coding) {lang=coding;}
 private:
#endif
};


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// Utility function
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
// Statistical EasyExpr synthesizer
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
#define MAKESTATSTREXP
#ifdef MAKESTATSTREXP
///////////////////////////////////////////////////////////////////////////////////////////////////
// public functions
///////////////////////////////////////////////////////////////////////////////////////////////////
string StatEasyExpExtraction(const vector<string>& cmatchs
			 ,const vector<string>& cunmatchs
			 ,int mode=0,int LIST_LIMIT=100) ;
#endif // MAKEEASYEXP
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

#endif




