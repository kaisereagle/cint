/*****************************************************************************
* EasyExp.h
*
* Description: Proprietary Regular Expression library associated with open 
*              source C++ interpreter CINT
*
*  Original library was named RegExp which had name conflict with standard 
* regular expression library.
*
* Author : Masaharu Goto
*
* 2008/July/9 Masaharu Goto
*
* Easy Expression definition
*
*  EasyExp::EasyExp(const char* pattern,Mode mode=Auto);
*  EasyExp::comple(const char* pattern,Mode mode=Auto);
*
*  pattern := expr <>-----* atom
*
*   atom
*    *           : Match any string of 0 or more characters
*    ?           : Match any signle character
*    [12aeg]     : Match a single character from the list "12aeg"
*    [a-z0-9]    : Match a single character in the range between a-z and 0-9
*    [^12ea]     : Match a single character not from the list "12ea"
*    [^a-z]      : Match a single character not in the range bwtween a-z
*    [12aeg:5]   : Match 5 characters from the list "12aeg"
*    [12aeg:2-5] : Match 2 to 5 characters from the list "12aeg"
*    [0-9:2-*]   : Match 2 more more characters in the range between 0-9
*    [12aeg:*]   : Match any number of characters from the list "12aeg"
*    (ab|cd|ef)  : Match from alternative "ab" "cd" or "ef"
*    #ifndef NUMBER_IN_BRACES
*    (5..20)     : Match a number in the range between 5 to 20
*    (05..20)    : Match 2 digits number in the range between 5 to 20
*    (05..20:2)  : Match 2 digits number in the range between 5 to 20
*    #else
*    {5-20}    (5..20)    : Match a number in the range between 5 to 20
*    {05-20}   (05..20)   : Match 2 digits number in the range between 5 to 20
*    {05-20:2} (05..20:2) : Match 2 digits number in the range between 5 to 20
*    #endif
*  
* Easy Expression Replace definition
*
*  EasyExp::comple_replace(const char* pattern,const char* r,Mode mode=Auto);
*
*  EasyExp e;    //   $0 $1 $2 $3 $4
*  e.compile_replace("abc*defghi?.cxx","$2=DEF,$4=.h");
*
*
* Easy Expression Synthesis
*
*  string EasyExpExtraction(const vector<string>& matchs
*			   ,const vector<string>& unmatchs
*			   ,int mode=0,int LIST_LIMIT=100) ;
*
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

#if defined(__CINT__) && !defined(__MAKECINT__)
#pragma include_noerr <EasyExp.dll>
#endif

#ifndef EASYEXPH
#define EASYEXPH

#define E__REPLACE2 // ifdef eliminated
#define NUMBER_IN_PARENTHESIS
//#define NUMBER_IN_BRACES
#define MULTICHARLIST
#define DIRHIERARCHY
#define CHARLISTRANGE

#define E__ARGS // << ifdef eliminated
#define E__SMARTREPLACE

#define E__PATTERN_MODIFICATION

#if defined(__linux__) && !defined(__CINT__)
#include <strings.h>
#elif defined(_WIN32)
#ifndef __VC13__
#define strncasecmp strnicmp
#else
#define strncasecmp _strnicmp
#endif
#define strcasecmp stricmp
#define strcasestr stristr
char* stristr(const char* a,const char* b);
#endif

#include "ReadF.h"
#include "Stat.h"
#include "MultiByteChar.h"

extern int EasyExp_DEBUG;

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

#define STRINGLIST_FIX1

//////////////////////////////////////////////////////////////////////////
string toupper(const string& a) ;
string EasyExp_anystring();
string EasyExp_anychar();
string EasyExp_notcharlist(const string& charlist);
unsigned int EasyExp_numberofsamples();
string EasyExp_numberrange(long low,long high,long padding) ;
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// Match element bass class
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
class MatchElement {
 public:
  MatchElement() { }
  virtual MatchElement* duplicate() const { return(0); }
  MatchElement(const MatchElement& a) { }
  virtual ~MatchElement() {}
  virtual const char* match(const char* s,vector<string>& argv,vector<int>& argi,bool icase=false) =0;
  virtual const char* what() { return(""); }
  virtual int nlist() { return(0); }
  virtual const char* contents(int i=0) { return(""); }
  virtual const char* retrieve(int i) { return(0); }
  virtual long parameter(int i=0) { return(0); }
  virtual int complexity() const { return(0); }
  virtual void disp(FILE* fp=stdout) const=0;
  virtual bool isstar() const { return(false); }
  virtual bool hasemptystring() const { return(false); }

  virtual void verification_samples(vector<string>& vs,int mode=0) const {};
};

//////////////////////////////////////////////////////////////////////////
// Match element derived classes
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//  'abcd'
class MatchString : public MatchElement {
 public:
  MatchString(const MatchString& a)  { m_s = a.m_s; }
  virtual MatchElement* duplicate() const { return(new MatchString(*this)); }
  MatchElement* duplicate() { return(new MatchString(*this));  }
  MatchString(const string& exp) : m_s(exp) {}
  virtual ~MatchString() {}
  /////////////////////////////////////////////////////////////
  virtual const char* match(const char* s,vector<string>& argv,vector<int>& argi,bool icase=false) {
    if(icase) {
      if(s && 0==strncasecmp(s,m_s.c_str(),m_s.size())) {
	  argv.push_back(string(s,m_s.size()));
	  argi.push_back(0);
	  return(s+m_s.size());
      }
    }
    else {
      if(s && 0==strncmp(s,m_s.c_str(),m_s.size())) {
	  argv.push_back(string(s,m_s.size()));
	  argi.push_back(0);
	  return(s+m_s.size());
      }
    }
    return 0;
  }
  /////////////////////////////////////////////////////////////
  virtual const char* what() { return("MatchString"); }
  virtual int nlist() { return(1); }
  virtual const char* contents(int i=0) {return(m_s.c_str());}
  virtual const char* retrieve(int i) {
    return(m_s.c_str());
  }
  virtual int complexity() const { return(1); }
  virtual void disp(FILE* fp=stdout) const { fprintf(fp,"%-15s'%s'\n","String",m_s.c_str()); }
  virtual void verification_samples(vector<string>& vs,int mode=0) const {
    vector<string>::iterator first,last=vs.end();
    for(first=vs.begin();first!=last;++first) {
      (*first) += m_s;
    }
  }
 private:
  string m_s;

};
//////////////////////////////////////////////////////////////////////////
//  '*abc'
class MatchStarString : public MatchElement {
 public:
  MatchStarString(const MatchStarString& a) { m_s = a.m_s; }
  MatchStarString(const string& exp) : m_s(exp) {}
  virtual ~MatchStarString() {}
  /////////////////////////////////////////////////////////////
  virtual const char* match(const char* s,vector<string>& argv,vector<int>& argi,bool icase=false) {
    if(s) { 
      char *p;
      if(icase) p = strcasestr((char*)s,(char*)m_s.c_str());
      else      p = strstr((char*)s,(char*)m_s.c_str());
      if(p) {
	argv.push_back(string(s,p-s));
	argv.push_back(string(p,m_s.size()));
	argi.push_back(0);
	argi.push_back(0);
	return(p+m_s.size());
      }
      else  return(0);
    }
    else return 0;
  }
  /////////////////////////////////////////////////////////////
  virtual const char* what() { return("MatchStarString"); }
  virtual int nlist() { return(1); }
  virtual const char* contents(int i=0) {return(m_s.c_str());}
  virtual int complexity() const { return(2); }
  virtual void disp(FILE* fp=stdout) const { fprintf(fp,"%-15s'%s'\n","*String",m_s.c_str()); }
  virtual bool isstar() const { return(true); }
  /////////////////////////////////////////////////////////////
  virtual void verification_samples(vector<string>& vs,int mode=0) const {
    vector<string>::iterator first,last=vs.end();
    for(first=vs.begin();first!=last;++first) {
      (*first) += EasyExp_anystring() + m_s;
    }
  }
  /////////////////////////////////////////////////////////////
 private:
  string m_s;

};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//  '(ab|cd|ef)'
class MatchStringList : public MatchElement {
 public:
  MatchStringList(const MatchStringList& a) { m_s = a.m_s; }
  virtual MatchElement* duplicate() const { return(new MatchStringList(*this)); }
  MatchStringList(const deque<string>& exp) : m_s(exp) {}
  virtual ~MatchStringList() {}
  /////////////////////////////////////////////////////////////
  virtual const char* match(const char* s,vector<string>& argv,vector<int>& argi,bool icase=false) {
    if(!s) return(0);
    //deque<string>::reverse_iterator first=m_s.rbegin();
    //deque<string>::reverse_iterator last =m_s.rend();
    deque<string>::iterator first=m_s.begin();
    deque<string>::iterator last =m_s.end();
    int i=0;
    int zeroflag=0;
#ifdef STRINGLIST_FIX1
    string matchpattern; int matchi, matchflag=0;
#endif
    while(first!=last) {
      if((*first).size()==0) {
	// zero length string, match anything if no other string match
	++zeroflag; ++first; ++i;
	continue;
      }
      if(icase) { // case independent non-zero length match
	if(0==strncasecmp(s,(*first).c_str(),(*first).size())) {
#ifdef STRINGLIST_FIX1
	  if(matchpattern.size()<(*first).size()) {
	    matchpattern = (*first);
	    matchi = i;
	    ++matchflag;
	  }
#else
	  argv.push_back(string(s,(*first).size()));
	  argi.push_back(i);
	  return(s+(*first).size());
#endif
	}
      }
      else { // case dependent non-zero length match
	if(0==strncmp(s,(*first).c_str(),(*first).size())) {
#ifdef STRINGLIST_FIX1
	  if(matchpattern.size()<(*first).size()) {
	    matchpattern = (*first);
	    matchi = i;
	    ++matchflag;
	  }
#else
	  argv.push_back(string(s,(*first).size()));
	  argi.push_back(i);
	  return(s+(*first).size());
#endif
        }
      }
      ++first;
      ++i;
    }
#ifdef STRINGLIST_FIX1
    if(matchflag) {
      argv.push_back(string(s,matchpattern.size()));
      argi.push_back(matchi);
      return(s+matchpattern.size());
    }
#endif
    if(zeroflag) {
      // non-zero length matching are detected and returned before this point.
      return(s);
    }
    return(0);
  }
  /////////////////////////////////////////////////////////////
  virtual bool hasemptystring() const {
    deque<string>::const_iterator first=m_s.begin();
    deque<string>::const_iterator last =m_s.end();
    while(first!=last) {
      if((*first).size()==0) return(true);
      ++first;
    }
    return(false);
  }
  /////////////////////////////////////////////////////////////
  virtual const char* what() { return("MatchStringList"); }
  virtual int nlist() { return(m_s.size()); }
  /////////////////////////////////////////////////////////////
  virtual const char* contents(int i=0) {
    if((unsigned int)i<m_s.size()) return(m_s[i].c_str());
    else return("");
  }
  /////////////////////////////////////////////////////////////
  virtual const char* retrieve(int i) {
    if((unsigned int)i<m_s.size()) return(m_s[i].c_str());
    else return(0);
  }
  /////////////////////////////////////////////////////////////
  virtual int complexity() const { return(1); }
  /////////////////////////////////////////////////////////////
  virtual void disp(FILE* fp=stdout) const { 
    fprintf(fp,"%-15s","StringList");
    for(unsigned int i=0;i<m_s.size();i++) fprintf(fp,"'%s',",m_s[i].c_str());
    fprintf(fp,"\n");
  }
  /////////////////////////////////////////////////////////////
  virtual void verification_samples(vector<string>& vs,int mode=0) const {
    // naive bayes assumption
    deque<string>::const_iterator firsts=m_s.begin(),lasts=m_s.end();
    vector<string>::iterator first=vs.begin(),last=vs.end();
    vector<string> vsout;
    bool flag1=true,flag2=true;
    while(flag1 || flag2) {
      //  previous_variation[M++] + current_variation[N++]
      vsout.push_back((*first) + (*firsts));
      if((++first)==last) { first=vs.begin(); flag1=false; }
      if((++firsts)==lasts) { firsts=m_s.begin(); flag2=false; }
    }
    vs = vsout;
  }
  /////////////////////////////////////////////////////////////
 private:
  deque<string> m_s;

};
//////////////////////////////////////////////////////////////////////////
//  '*(ab|cd|ef)'
class MatchStarStringList : public MatchElement {
 public:
  MatchStarStringList(const MatchStarStringList& a) { m_s = a.m_s; }
  virtual MatchElement* duplicate() const { return(new MatchStarStringList(*this)); }
  MatchStarStringList(const deque<string>& exp) : m_s(exp) {}
  virtual ~MatchStarStringList() {}
  /////////////////////////////////////////////////////////////
  virtual const char* match(const char* s,vector<string>& argv,vector<int>& argi,bool icase=false) {
    char* tmp; // current match point
    char* p=0; // left most match point
    int len=0;
    int zeroflag=0;
    if(!s) return(0);
    //deque<string>::reverse_iterator first =m_s.rbegin();
    //deque<string>::reverse_iterator last  =m_s.rend();
    deque<string>::iterator first =m_s.begin();
    deque<string>::iterator last  =m_s.end();
    int i=0,j=0;
    while(first!=last) {
      if((*first).size()==0) {
	// zero length string, match anything if no other string match
	++zeroflag; ++first; ++i;
	continue;
      }
      if(icase) tmp = strcasestr((char*)s,(char*)(*first).c_str());
      else      tmp = strstr((char*)s,(char*)(*first).c_str());
      if(tmp && (!p || tmp<p) ) {  // select let most match point
	p=tmp; len=(*first).size(); 
	j=i;
      }
      ++first;
      ++i;
    }
    if(p) { // stringlist match  *StringList
      //    argv[i]='*'               argv[i+1]='stringlist[j]'
      argv.push_back(string(s,p-s)); argv.push_back(string(p,len));
      //    no index info            index of matching stringlist 
      argi.push_back(0);             argi.push_back(j);
      return(p+len);
    }
    else if(zeroflag) { // zero-length string match anything
      // non-deterministic pattern == *(|X) -> '*'
      // this should be a problem...
      return(s);
    }
    else return(0);
  }
  /////////////////////////////////////////////////////////////
  virtual bool hasemptystring() const {
    deque<string>::const_iterator first=m_s.begin();
    deque<string>::const_iterator last =m_s.end();
    while(first!=last) {
      if((*first).size()==0) return(true);
      ++first;
    }
    return(false);
  }
  /////////////////////////////////////////////////////////////
  virtual const char* what() { return("MatchStarStringList"); }
  virtual int nlist() { return(m_s.size()); }
  /////////////////////////////////////////////////////////////
  virtual const char* contents(int i=0) {
    if((unsigned int)i<m_s.size()) return(m_s[i].c_str());
    else return("");
  }
  /////////////////////////////////////////////////////////////
  virtual const char* retrieve(int i) {
    if((unsigned int)i<m_s.size()) return(m_s[i].c_str());
    else return(0);
  }
  /////////////////////////////////////////////////////////////
  virtual int complexity() const { return(2); }
  /////////////////////////////////////////////////////////////
  virtual void disp(FILE* fp=stdout) const { 
    fprintf(fp,"%-15s","*StringList");
    for(unsigned int i=0;i<m_s.size();i++) fprintf(fp,"'%s',",m_s[i].c_str());
    fprintf(fp,"\n");
  }
  /////////////////////////////////////////////////////////////
  virtual bool isstar() const { return(true); }
  /////////////////////////////////////////////////////////////
  virtual void verification_samples(vector<string>& vs,int mode=0) const {
    // naive bayes assumption
    deque<string>::const_iterator firsts=m_s.begin(),lasts=m_s.end();
    vector<string>::iterator first=vs.begin(),last=vs.end();
    vector<string> vsout;
    bool flag1=true,flag2=true;
    while(flag1 || flag2) {
      //  previous_variation[M++] + anystring + current_variation[N++]
      vsout.push_back((*first) + EasyExp_anystring() + (*firsts));
      if((++first)==last) { first=vs.begin(); flag1=false; }
      if((++firsts)==lasts) { firsts=m_s.begin(); flag2=false; }
    }
    vs = vsout;
  }
  /////////////////////////////////////////////////////////////
 private:
  deque<string> m_s;

};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//  '[0-1a-f]'  '[0-1a-f:*]' '[0-1a-f:15]'
class MatchCharList : public MatchElement {
 public:
  MatchCharList(const MatchCharList& a) 
    { m_charlist=a.m_charlist; m_seq=a.m_seq; m_seqend=a.m_seqend; }
  virtual MatchElement* duplicate() const { return(new MatchCharList(*this)); }
  MatchCharList(const string& exp,int seq=1,int seqend=-1) : m_charlist(exp), m_seq(seq), m_seqend(seqend) {}
  virtual ~MatchCharList() {}
  /////////////////////////////////////////////////////////////
  virtual const char* match(const char* s,vector<string>& argv,vector<int>& argi,bool icase=false) {
#ifdef MULTICHARLIST
    string tmp;
    string charlist;
    if(icase) charlist = toupper(m_charlist);
    else      charlist = m_charlist;
    int i=0;
    if(m_seq == -1) {
      if(icase) tmp = toupper(s+i);
      else      tmp = s+i;
      while(s&&s[i]&& 0==tmp.find_first_of(charlist)) { 
	++i; 
	if(icase) tmp = toupper(s+i);
	else      tmp = s+i;
      }
    }
    else {
      for(i=0;i<m_seq;i++) {
        if(icase) tmp = toupper(s+i);
        else      tmp = s+i;
        if  (s&&s[i]&& 0==tmp.find_first_of(charlist)) { }
        else return(0);
      }
#ifdef CHARLISTRANGE
      if(m_seqend != -1) {
        for(;i<m_seqend;i++) {
          if(icase) tmp = toupper(s+i);
          else      tmp = s+i;
          if  (s&&s[i]&& 0==tmp.find_first_of(charlist)) { }
          else break;
	}
      }
#endif
    }
    argv.push_back(string(s,i));
    argi.push_back(charlist.find_first_of(argv.back()[0])); 
    return(s+i);
#else
    string tmp=s;
    if(s&&s[0]&& 0==tmp.find_first_of(m_charlist)) return(s+1);
    else return 0;
#endif
  }
  /////////////////////////////////////////////////////////////
  virtual bool hasemptystring() const {
    if(m_seq==0) return(true);
    else         return(false);
  }
  /////////////////////////////////////////////////////////////
  virtual const char* what() { return("MatchCharList"); }
  virtual int nlist() { return(m_charlist.size()); }
  /////////////////////////////////////////////////////////////
  virtual const char* contents(int i=0) {
    if((unsigned int)i<m_charlist.size()&&i>=0) {
      m_buf[0] = m_charlist[i];
      m_buf[1] = 0;
      return(m_buf);
    }
    else return(&m_charlist[0]);
  }
  /////////////////////////////////////////////////////////////
  virtual const char* retrieve(int i) {
    if((unsigned int)i<m_charlist.size()) {
      m_buf[0] = m_charlist[i];
      m_buf[1] = 0;
      return(m_buf);
    }
    else return(0);
  }
  /////////////////////////////////////////////////////////////
  virtual long parameter(int i=0) { 
    if(i==0) return(m_seq); 
    else     return(m_seqend); 
  }
  /////////////////////////////////////////////////////////////
  virtual int complexity() const { return(1); }
  virtual void disp(FILE* fp=stdout) const { fprintf(fp,"%-15s'%s' %d - %d\n","[a-z]",m_charlist.c_str(),m_seq,m_seqend); }
  /////////////////////////////////////////////////////////////
  virtual void verification_samples(vector<string>& vs,int mode=0) const {
    // naive bayes assumption
    vector<string>::iterator first=vs.begin(),last=vs.end();
    unsigned int i=0;
    vector<string> vsout;
    bool flag1=true,flag2=true;
    while(flag1 || flag2) {
      //  previous_variation[M++] + current_variation[N++]
      vsout.push_back((*first) + m_charlist[i]);
      if((++first)==last) { first=vs.begin(); flag1=false; }
      if(++i>=m_charlist.size()) { i=0; flag2=false; }
    }
    vs = vsout;
  }
  /////////////////////////////////////////////////////////////
 private:
  string m_charlist;
  int    m_seq;
  int    m_seqend;
  char   m_buf[3];

};
//////////////////////////////////////////////////////////////////////////
//  '*[0-1a-f]'  '*[0-1a-f:*]' '*[0-1a-f:15]'
class MatchStarCharList : public MatchElement {
 public:
  MatchStarCharList(const MatchStarCharList& a) 
    { m_charlist=a.m_charlist; m_seq=a.m_seq; m_seqend=a.m_seqend; }
  virtual MatchElement* duplicate() const { return(new MatchStarCharList(*this)); }
  MatchStarCharList(const string& exp,int seq=1,int seqend=-1) : m_charlist(exp), m_seq(seq), m_seqend(seqend) {}
  virtual ~MatchStarCharList() {}
  /////////////////////////////////////////////////////////////
  virtual const char* match(const char* s,vector<string>& argv,vector<int>& argi,bool icase=false) {
    string tmp;
    if(icase)  tmp=toupper(s);
    else       tmp=s;
    string charlist;
    if(icase) charlist=toupper(m_charlist);
    else      charlist=m_charlist;
    unsigned int pos=tmp.find_first_of(charlist);
#ifdef MULTICHARLIST
    if(s&&s[0]&& /* 0<=pos && */ pos<tmp.size()) {
      int i=(int)pos;
      if(m_seq == -1) {
	if(icase) tmp = toupper(s+i);
	else      tmp = s+i;
        while(s&&s[i]&& 0==tmp.find_first_of(charlist)) { 
	  ++i; 
	  if(icase) tmp = toupper(s+i);
	  else      tmp = s+i;
	}
      }
      else {
        for(i=(int)pos;i<m_seq+(int)pos;i++) {
	  if(icase) tmp = toupper(s+i);
	  else      tmp = s+i;
          if(s&&s[i]&& 0==tmp.find_first_of(charlist)) { }
          else return(0);
        }
#ifdef CHARLISTRANGE
        if(m_seqend != -1) {
          for(;i<m_seqend;i++) {
            if(icase) tmp = toupper(s+i);
            else      tmp = s+i;
            if  (s&&s[i]&& 0==tmp.find_first_of(charlist)) { }
            else break;
	  }
        }
#endif
      }
      argv.push_back(string(s,pos));
      argv.push_back(string(s+pos,i-pos));
      argi.push_back(0);
      argi.push_back(charlist.find_first_of(argv.back()[0])); 
      return(s+i);
    }
    else return 0;
#else
    if(s&&s[0]&& 0<=pos && pos<tmp.size()) return(s+pos+1);
    else return 0;
#endif
  }
  /////////////////////////////////////////////////////////////
  virtual bool hasemptystring() const {
    if(m_seq==0) return(true);
    else         return(false);
  }
  /////////////////////////////////////////////////////////////
  virtual const char* what() { return("MatchStarCharList"); }
  virtual int nlist() { return(m_charlist.size()); }
  /////////////////////////////////////////////////////////////
  virtual const char* contents(int i=0) {
    if((unsigned int)i<m_charlist.size()&&i>=0) {
      m_buf[0] = m_charlist[i];
      m_buf[1] = 0;
      return(m_buf);
    }
    else return(&m_charlist[0]);
  }
  /////////////////////////////////////////////////////////////
  virtual long parameter(int i=0) { 
    if(i==0) return(m_seq); 
    else     return(m_seqend); 
  }
  /////////////////////////////////////////////////////////////
  virtual int complexity() const { return(2); }
  virtual void disp(FILE* fp=stdout) const { fprintf(fp,"%-15s'%s' %d - %d\n","*[a-z]",m_charlist.c_str(),m_seq,m_seqend); }
  virtual bool isstar() const { return(true); }
  /////////////////////////////////////////////////////////////
  virtual void verification_samples(vector<string>& vs,int mode=0) const {
    // naive bayes assumption
    vector<string>::iterator first=vs.begin(),last=vs.end();
    unsigned int i=0;
    vector<string> vsout;
    bool flag1=true,flag2=true;
    while(flag1 || flag2) {
      //  previous_variation[M++] + anystring  + current_variation[N++]
      vsout.push_back((*first) + EasyExp_anystring() + m_charlist[i]);
      if((++first)==last) { first=vs.begin(); flag1=false; }
      if(++i>=m_charlist.size()) { i=0; flag2=false; }
    }
    vs = vsout;
  }
  /////////////////////////////////////////////////////////////
 private:
  string m_charlist;
  int    m_seq;
  int    m_seqend;
  char   m_buf[3];

};


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//  '[^0-1a-f]'  '[^0-1a-f:*]' '[^0-1a-f:15]'
class MatchNotCharList : public MatchElement {
 public:
  MatchNotCharList(const MatchNotCharList& a) 
    { m_charlist=a.m_charlist; m_seq=a.m_seq; m_seqend=a.m_seqend; }
  virtual MatchElement* duplicate() const { return(new MatchNotCharList(*this)); }
  MatchNotCharList(const string& exp,int seq=1,int seqend=-1) : m_charlist(exp), m_seq(seq), m_seqend(seqend) {}
  virtual ~MatchNotCharList() {}
  /////////////////////////////////////////////////////////////
  virtual const char* match(const char* s,vector<string>& argv,vector<int>& argi,bool icase=false) {
#ifdef MULTICHARLIST
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
        else return(0);
      }
#ifdef CHARLISTRANGE
      if(m_seqend != -1) {
        for(;i<m_seqend;i++) {
          if(icase) tmp = toupper(s+i);
          else      tmp = s+i;
          if  (s&&s[i]&& 0==tmp.find_first_not_of(charlist)) { }
          else break;
        }
      }
#endif
    }
    argv.push_back(string(s,i));
    argi.push_back(0); 
    return(s+i);
#else
    string tmp=s;
    if(s&&s[0]&& 0==tmp.find_first_not_of(m_charlist)) return(s+1);
    else return 0;
#endif
  }
  /////////////////////////////////////////////////////////////
  virtual const char* what() { return("MatchNotCharList"); }
  virtual int nlist() { return(m_charlist.size()); }
  /////////////////////////////////////////////////////////////
  virtual const char* contents(int i=0) {
    if((unsigned int)i<m_charlist.size()) {
      m_buf[0] = m_charlist[i];
      m_buf[1] = 0;
      return(m_buf);
    }
    else return(&m_charlist[0]);
  }
  /////////////////////////////////////////////////////////////
  virtual long parameter(int i=0) { 
    if(i==0) return(m_seq); 
    else     return(m_seqend); 
  }
  /////////////////////////////////////////////////////////////
  virtual int complexity() const { return(1); }
  virtual void disp(FILE* fp=stdout) const { fprintf(fp,"%-15s'%s' %d - %d\n","[^a-z]",m_charlist.c_str(),m_seq,m_seqend); }
  /////////////////////////////////////////////////////////////
  virtual void verification_samples(vector<string>& vs,int mode=0) const {
    // naive bayes assumption
    vector<string>::iterator first=vs.begin(),last=vs.end();
    unsigned int i=0;
    vector<string> vsout;
    unsigned int N=EasyExp_numberofsamples();
    bool flag1=true,flag2=true;
    while(flag1 || flag2) {
      //  previous_variation[M++] + current_variation[N++]
      vsout.push_back((*first) + EasyExp_notcharlist(m_charlist));
      if((++first)==last) { first=vs.begin(); flag1=false; }
      if(++i>=N) { i=0; flag2=false; }
    }
    vs = vsout;
  }
  /////////////////////////////////////////////////////////////
 private:
  string m_charlist;
  int m_seq;
  int m_seqend;
  char   m_buf[3];

};
//////////////////////////////////////////////////////////////////////////
//  '*[^0-1a-f]'  '*[^0-1a-f:*]'  '*[^0-1a-f:15]'
class MatchStarNotCharList : public MatchElement {
 public:
  MatchStarNotCharList(const MatchStarNotCharList& a) 
    { m_charlist=a.m_charlist; m_seq=a.m_seq; m_seqend=a.m_seqend; }
  virtual MatchElement* duplicate() const { return(new MatchStarNotCharList(*this)); }
  MatchStarNotCharList(const string& exp,int seq=1,int seqend=-1) : m_charlist(exp), m_seq(seq), m_seqend(seqend) {}
  virtual ~MatchStarNotCharList() {}
  /////////////////////////////////////////////////////////////
  virtual const char* match(const char* s,vector<string>& argv,vector<int>& argi,bool icase=false) {
    string tmp;
    if(icase) tmp=toupper(s);
    else      tmp=s;
    string charlist;
    if(icase) charlist=toupper(m_charlist);
    else      charlist=m_charlist;
    unsigned int pos=tmp.find_first_not_of(charlist);
#ifdef MULTICHARLIST
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
          else return(0);
        }
#ifdef CHARLISTRANGE
        if(m_seqend != -1) {
          for(;i<m_seqend;i++) {
            if(icase) tmp = toupper(s+i);
            else      tmp = s+i;
            if  (s&&s[i]&& 0==tmp.find_first_not_of(charlist)) { }
            else break;
          }
        }
#endif
      }
      argv.push_back(string(s,pos));
      argv.push_back(string(s+pos,i-pos));
      argi.push_back(0);
      argi.push_back(0); 
      return(s+i);
    }
    else return 0;
#else
    if(s&&s[0]&& 0<=pos && pos<tmp.size()) return(s+pos+1);
    else return 0;
#endif
  }
  /////////////////////////////////////////////////////////////
  virtual const char* what() { return("MatchStarNotCharList"); }
  virtual int nlist() { return(m_charlist.size()); }
  /////////////////////////////////////////////////////////////
  virtual const char* contents(int i=0) {
    if((unsigned int)i<m_charlist.size()) {
      m_buf[0] = m_charlist[i];
      m_buf[1] = 0;
      return(m_buf);
    }
    else return(&m_charlist[0]);
  }
  /////////////////////////////////////////////////////////////
  virtual long parameter(int i=0) { 
    if(i==0) return(m_seq); 
    else     return(m_seqend); 
  }
  /////////////////////////////////////////////////////////////
  virtual int complexity() const { return(2); }
  virtual void disp(FILE* fp=stdout) const { fprintf(fp,"%-15s'%s' %d - %d\n","*[^a-z]",m_charlist.c_str(),m_seq,m_seqend); }
  virtual bool isstar() const { return(true); }
  /////////////////////////////////////////////////////////////
  virtual void verification_samples(vector<string>& vs,int mode=0) const {
    // naive bayes assumption
    vector<string>::iterator first=vs.begin(),last=vs.end();
    unsigned int i=0;
    vector<string> vsout;
    unsigned int N=EasyExp_numberofsamples();
    bool flag1=true,flag2=true;
    while(flag1 || flag2) {
      //  previous_variation[M++] + anystring() + current_variation[N++]
      vsout.push_back((*first) + EasyExp_anystring() + EasyExp_notcharlist(m_charlist));
      if((++first)==last) { first=vs.begin(); flag1=false; }
      if(++i>=N) { i=0; flag2=false; }
    }
    vs = vsout;
  }
  /////////////////////////////////////////////////////////////
 private:
  string m_charlist;
  int m_seq;
  int m_seqend;
  char   m_buf[3];

};

//////////////////////////////////////////////////////////////////////////
//  '{05-21L2}'  '{05-21:2}' 
class MatchNumberRange: public MatchElement {
 public:
  MatchNumberRange(const MatchNumberRange& a) 
    { m_min=a.m_min; m_max=a.m_max; m_padding=a.m_padding; }
  virtual MatchElement* duplicate() const { return(new MatchNumberRange(*this)); }
  MatchNumberRange(long min,long max,long padding) : m_min(min), m_max(max), m_padding(padding) { }
  virtual ~MatchNumberRange() {}
  /////////////////////////////////////////////////////////////
  virtual const char* match(const char* s,vector<string>& argv,vector<int>& argi,bool icase=false) {
    string str;
    if(!s) return(0);
    int i=0;
    while(s[i] && isdigit(s[i]) && (0==m_padding || (long)str.size()<m_padding))
      { str+=s[i++]; }
    if(!str.size()) return(0);
    int num = atoi(str.c_str());
    if(m_min<=num && num<=m_max && (0==m_padding || m_padding==(long)str.size())) {
      argv.push_back(string(s,i));
      argi.push_back(num-m_min);
      return(s+i);
    }
    return(0);
  }
  /////////////////////////////////////////////////////////////
  virtual const char* what() { return("MatchNumberRange"); }
  virtual int nlist() { return(3); }
  /////////////////////////////////////////////////////////////
  virtual const char* contents(int i=0) {
    if(i==0)         sprintf(m_buf,"%ld",m_min);
    else if(i==1)    sprintf(m_buf,"%ld",m_max);
    else if(i==2)    sprintf(m_buf,"%ld",m_padding);
    return(m_buf);
  }
  /////////////////////////////////////////////////////////////
  virtual const char* retrieve(int i) {
    if(i<=m_max-m_min) {
      sprintf(m_buf,"%ld",m_min+i);
      return(m_buf);
    }
    else return(0);
  }
  /////////////////////////////////////////////////////////////
  virtual long parameter(int i=0) { 
    if(i==0)      return(m_min); 
    else if(i==1) return(m_max); 
    else          return(m_padding); 
  }
  /////////////////////////////////////////////////////////////
  virtual int complexity() const { return(1); }
  virtual void disp(FILE* fp=stdout) const { fprintf(fp,"%-15s min=%ld max=%ld padding=%ld\n","NumberRange",m_min,m_max,m_padding); }
  /////////////////////////////////////////////////////////////
  virtual void verification_samples(vector<string>& vs,int mode=0) const {
    // naive bayes assumption
    vector<string>::iterator first=vs.begin(),last=vs.end();
    unsigned int i=0;
    vector<string> vsout;
    unsigned int N=EasyExp_numberofsamples();
    bool flag1=true,flag2=true;
    while(flag1 || flag2) {
      //  previous_variation[M++] + current_variation[N++]
      vsout.push_back((*first) + EasyExp_numberrange(m_min,m_max,m_padding));
      if((++first)==last) { first=vs.begin(); flag1=false; }
      if(++i>=N) { i=0; flag2=false; }
    }
    vs = vsout;
  }
  /////////////////////////////////////////////////////////////
 private:
  long m_min;
  long m_max;
  long m_padding;
  char m_buf[20];

};
//////////////////////////////////////////////////////////////////////////
//  '*{05-21L2}'   '*{05-21:2}' 
class MatchStarNumberRange: public MatchElement {
 public:
  MatchStarNumberRange(const MatchStarNumberRange& a) 
    { m_min=a.m_min; m_max=a.m_max; m_padding=a.m_padding; }
  virtual MatchElement* duplicate() const { return(new MatchStarNumberRange(*this)); }
  MatchStarNumberRange(long min,long max,long padding) : m_min(min), m_max(max), m_padding(padding) { }
  virtual ~MatchStarNumberRange() {}
  /////////////////////////////////////////////////////////////
  virtual const char* match(const char* s,vector<string>& argv,vector<int>& argi,bool icase=false) {
    string str;
    if(!s) return(0);
    int i=0;
    int pos=0;      
    do {
      while(s[i] && !isdigit(s[i])) { ++i; } // skip non digit characters
      pos=i;      
      while(s[i] && isdigit(s[i]) && (m_padding==0 || (long)str.size()<m_padding))
	{ str+=s[i++]; }  
      if(!str.size()) return(0);
      int num = atoi(str.c_str());
      if(m_min<=num && num<=m_max && (0==m_padding || m_padding==(long)str.size())) {
        argv.push_back(string(s,pos)); 
        argv.push_back(string(s+pos,i-pos)); 
	argi.push_back(0);
	argi.push_back(num-m_min);
        return(s+i);
      }
      str="";
      i=pos+1;
    } while(isdigit(s[i]) && m_padding>0); // this looping condition is debatable
    //} while(isdigit(s[i])); // this looping condition is debatable
    return(0);
  }
  /////////////////////////////////////////////////////////////
  virtual const char* what() { return("MatchStarNumberRange"); }
  virtual int nlist() { return(2); }
  /////////////////////////////////////////////////////////////
  virtual const char* contents(int i=0) {
    if(i==0)         sprintf(m_buf,"%ld",m_min);
    else if(i==1)    sprintf(m_buf,"%ld",m_max);
    else if(i==2)    sprintf(m_buf,"%ld",m_padding);
    return(m_buf);
  }
  /////////////////////////////////////////////////////////////
  virtual const char* retrieve(int i) {
    if(i<=m_max-m_min) {
      sprintf(m_buf,"%ld",m_min+i);
      return(m_buf);
    }
    else return(0);
  }
  /////////////////////////////////////////////////////////////
  virtual long parameter(int i=0) { 
    if(i==0)      return(m_min); 
    else if(i==1) return(m_max); 
    else          return(m_padding); 
  }
  /////////////////////////////////////////////////////////////
  virtual int complexity() const { return(2); }
  virtual void disp(FILE* fp=stdout) const { fprintf(fp,"%-15s min=%ld max=%ld padding=%ld\n","*NumberRange",m_min,m_max,m_padding); }
  virtual bool isstar() const { return(true); }
  /////////////////////////////////////////////////////////////
  virtual void verification_samples(vector<string>& vs,int mode=0) const {
    // naive bayes assumption
    vector<string>::iterator first=vs.begin(),last=vs.end();
    unsigned int i=0;
    vector<string> vsout;
    unsigned int N=EasyExp_numberofsamples();
    bool flag1=true,flag2=true;
    while(flag1 || flag2) {
      //  previous_variation[M++] + anystring() + current_variation[N++]
      vsout.push_back((*first) + EasyExp_anystring() + EasyExp_numberrange(m_min,m_max,m_padding));
      if((++first)==last) { first=vs.begin(); flag1=false; }
      if(++i>=N) { i=0; flag2=false; }
    }
    vs = vsout;
  }
  /////////////////////////////////////////////////////////////
 private:
  long m_min;
  long m_max;
  long m_padding;
  char m_buf[20];

};

//////////////////////////////////////////////////////////////////////////
//   ''
class MatchEnd : public MatchElement {
 public:
  MatchEnd(const MatchEnd& a) {}
  virtual MatchElement* duplicate() const { return(new MatchEnd(*this)); }
  MatchEnd() {}
  virtual ~MatchEnd() {}
  /////////////////////////////////////////////////////////////
  virtual const char* match(const char* s,vector<string>& argv,vector<int>& argi,bool icase=false) {
   if(s && s[0]==0) {
     return(s);
    }
    else return 0;
  }
  /////////////////////////////////////////////////////////////
  virtual const char* what() { return("MatchEnd"); }
  virtual int complexity() const { return(0); }
  virtual void disp(FILE* fp=stdout) const { fprintf(fp,"%-15s'%s'\n","End",""); }
  virtual void verification_samples(vector<string>& vs,int mode=0) const {}

};
//////////////////////////////////////////////////////////////////////////
//  '*'
class MatchStarEnd : public MatchElement {
 public:
  MatchStarEnd(const MatchStarEnd& a) {}
  MatchStarEnd() {}
  virtual ~MatchStarEnd() {}
  /////////////////////////////////////////////////////////////
  virtual const char* match(const char* s,vector<string>& argv,vector<int>& argi,bool icase=false) {
    if(s) { 
      int len=strlen(s);
      argv.push_back(s);
      argi.push_back(0);
      return(s+len);
    }
    else return 0;
  }
  /////////////////////////////////////////////////////////////
  virtual const char* what() { return("MatchStarEnd"); }
  virtual int complexity() const { return(1); }
  virtual void disp(FILE* fp=stdout) const { fprintf(fp,"%-15s'%s'\n","*End",""); }
  virtual bool isstar() const { return(true); }
  /////////////////////////////////////////////////////////////
  virtual void verification_samples(vector<string>& vs,int mode=0) const {
    vector<string>::iterator first,last=vs.end();
    for(first=vs.begin();first!=last;++first) {
      (*first) += EasyExp_anystring();
    }
  }
  /////////////////////////////////////////////////////////////

};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//  '?'
class MatchAnyChar : public MatchElement {
 public:
  MatchAnyChar(const MatchAnyChar& a) {}
  virtual MatchElement* duplicate() const { return(new MatchAnyChar(*this)); }
  MatchAnyChar() {}
  virtual ~MatchAnyChar() {}
  /////////////////////////////////////////////////////////////
  virtual const char* match(const char* s,vector<string>& argv,vector<int>& argi,bool icase=false) {
    if(s&&s[0]) {
      argv.push_back(string(s,1));
      argi.push_back(0);
      return(s+1); 
    }
    else return 0;
  }
  /////////////////////////////////////////////////////////////
  virtual const char* what() { return("MatchAnyChar"); }
  virtual int complexity() const { return(1); }
  virtual void disp(FILE* fp=stdout) const { fprintf(fp,"%-15s'%s'\n","?",""); }
  /////////////////////////////////////////////////////////////
  virtual void verification_samples(vector<string>& vs,int mode=0) const {
    vector<string>::iterator first,last=vs.end();
    for(first=vs.begin();first!=last;++first) {
      (*first) += EasyExp_anychar();
    }
  }
  /////////////////////////////////////////////////////////////
};
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// Regular Expression class
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
class EasyExp 
#ifdef E__MULTIBYTELIB
 : public G__MultiByteMode
#endif
{
 public:
  enum Mode { Auto, RegEx, Exact };

  EasyExp() {} 
  EasyExp(const EasyExp& a) ;
  EasyExp(const char* pattern,Mode mode=Auto);
  EasyExp(const string& pattern,Mode mode=Auto);
  EasyExp(const char* pattern,const char* replace,Mode mode=Auto);
  EasyExp(const string& pattern,const string& replace,Mode mode=Auto);
  ~EasyExp();
  //void set(const deque<MatchElement*>& element,Mode mode=Auto);

  void compile(const char* pattern,Mode mode=Auto);
  void compile(const string& pattern,Mode mode=Auto){compile(pattern.c_str(),mode);}
  bool match(const char *s,bool icase=false)  ;
  bool match(const string& s,bool icase=false){return(match(s.c_str(),icase));}
  vector<string>  match(const vector<string>& in,bool icase=false)  ;
  vector<string> _match(const vector<string>& in,bool icase=false)  ;
  virtual int complexity() const ;
  void disp(FILE* fp=stdout) const;

  static void help() ;

 public:
  void compile_replace(const char* pattern,const char* r,Mode mode=Auto) {
    if(mode==Exact) {
      fprintf(stderr,"EasyExp::compile_replace(), mode==EXACT is not allowed -> Auto is used\n");
      mode=Auto;
    }
    clear_replace();
    compile_replace(r);
    compile(pattern,mode);
    m_pattern=pattern;
    m_replacepattern=r;
  }
  void compile_replace(const string& pattern,const string& r,Mode mode=Auto)
    {compile_replace(pattern.c_str(),r.c_str(),mode);}
  bool replace(const char *in,string& out,bool icase=false)  ;
  bool replace(const string& in,string& out,bool icase=false)
    {return(replace(in.c_str(),out,icase));}

 private:
  void clear(int del=1);
  void clear_replace();
  void compile_replace(const char* r);
  void compile_replace(int i,const char* r);

 public:
  vector<string> verification_samples(int mode=0) ;

 private:
  bool compile_string(const char* s,int len,int& i,bool star) ;
  bool getstring(string& buf,const char* s,int len,int& i) ;
  bool compile_braket(const char* s,int len,int& i,bool star) ;
  bool getcharlist(string& charlist,const char* s,int len,int& i,int& seq,int& seqend) ;
  bool compile_parenthesis(const char* s,int len,int& i,bool star) ;
  bool getstringlist(deque<string>& strlist,const char* s,int len,int& i) ;
#ifdef NUMBER_IN_PARENTHESIS
  long getstringlist(deque<string>& strlist,const char* s,int len,int& i,long* pmin,long* pmax) ;
#endif
  bool compile_brace(const char* s,int len,int& i,bool star) ;
  long getnumberrange(long* pmin,long* pmax,const char* s,int len,int& i) ;
  void compile_end(bool star) ;

 private:
  deque<MatchElement*> m_element;

 public:
  vector<string> argv;
 private:
  vector<int> argi;
  deque<MatchElement*> m_replace;

 public: // testing purpose 
  unsigned int element_size() const { return(m_element.size()); }
  MatchElement* element(int i) const { return(m_element[i]); }
  unsigned int replace_element_size() const { return(m_replace.size()); }
  MatchElement* replace_element(int i) const { return(m_replace[i]); }

 public:
  // get original expression 
  //const char* pattern() const { return(m_pattern.c_str()); }
  const string pattern() const { return(m_pattern); }
  const string replacepattern() const { return(m_replacepattern); }

#ifdef E__PATTERN_MODIFICATION
  // Non public interface
  // pattern modificaitons
  bool pattern_inversion(const char *s,string& out,bool icase=false)  ;
  void pattern_generalization() ;
  int pattern_optimization() ;
  unsigned int pattern_splitting_size() const ;
  vector<EasyExp> pattern_splitting(int index=-1) const ;

  // expression back-annotatation
  string backannotation() const;
 private:
  string backannotation(const deque<MatchElement*>& element) const;
  string  m_pattern; // expression
  string  m_replacepattern; // replace expression
#endif

};


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// Utility function
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

int matchregex(const char *pattern,const char *string);
int operator==(EasyExp& ex,const char *string) ;
int operator!=(EasyExp& ex,const char *string) ;
int operator==(const char *string,EasyExp& ex) ;
int operator!=(const char *string,EasyExp& ex) ;
int operator==(const EasyExp& ex1,const EasyExp ex2) ;
int operator!=(const EasyExp& ex1,const EasyExp ex2) ;
int operator<(const EasyExp& ex1,const EasyExp ex2) ;

//////////////////////////////////////////////////////////////////////////



#define SCANDIR
#ifdef SCANDIR
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// Scan directory
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
class Scandir {
 public:
  Scandir() { init(); }
  Scandir(const string& expr,int isdir=0,int icase=0) { init(); scan(expr,isdir,icase); }
  Scandir(const char *expr,int isdir=0,int icase=0) { init(); scan(expr,isdir,icase); }
  int scan(const string& expr,int isdir=0,int icase=0) ;
  int scan(const char* expr,int isdir=0,int icase=0) { return(scan(string(expr),isdir,icase)); }
  void clear() { m_files.clear(); }
  void rewind() ;
  string next() ;
  const char* Next() { static string s; s=next(); return(s.c_str()); }
  size_t size() {return(m_files.size()); }
  void disp(FILE *fp=stdout) const ;
  void setvervose(int in) { m_vervose=in; }
  void init() {m_vervose=0;}

  int scandisp(const string& expr,int isdir=0,int icase=0,FILE *fp=stdout) {
    int result = scan(expr,isdir,icase);
    disp(fp);
    return(result);
  }
  int scandisp(const char* expr,int isdir=0,int icase=0,FILE *fp=stdout) 
    { return(scandisp(string(expr),isdir,icase,fp)); }

  void get(vector<string>& v) {
    map<string,int>::iterator first=m_files.begin();
    map<string,int>::iterator last =m_files.end();
    while(first!=last) {
      v.push_back((*first).first);
      ++first;
    }
  }
  void getfullpath(vector<string>& v) { get(v); }
  map<string,int>& get() { return(m_files); }

  void getfname(vector<string>& v) { 
    string s;
    size_t pos;
    map<string,int>::iterator first=m_files.begin();
    map<string,int>::iterator last =m_files.end();
    while(first!=last) {
      s = (*first).first;
      pos = s.find_last_of("/");
      if(pos!=string::npos) s = s.substr(pos+1);
      v.push_back(s);
      ++first;
    }
  }
  void getpath(vector<string>& v) { 
    string s;
    size_t pos;
    map<string,int>::iterator first=m_files.begin();
    map<string,int>::iterator last =m_files.end();
    while(first!=last) {
      s = (*first).first;
      pos = s.find_last_of("/");
      if(pos!=string::npos) s = s.substr(0,pos);
      v.push_back(s);
      ++first;
    }
  }

 private:
  int scandir_element(map<string,int>& result,const string& expr,const string& dir,int isdir) ;
#ifdef DIRHIERARCHY
  int scandir_dirhierarchy(map<string,int>& dirs,const string& rootdir,int rtsize);
#endif
  int scandir_subdir(map<string,int>& result,const string& expr,map<string,int>& dirs,const string& rt) ;
  int scandir(map<string,int>& result,const string& direxpr,const string& rootdir=".") ;
  map<string,int> m_files;
  map<string,int>::iterator m_first;
  map<string,int>::iterator m_last;
  int m_isdir;
  int m_icase;

  int m_vervose;
};
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
#endif // SCANDIR



#ifdef _WIN32
/////////////////////////////////////////////////////////////////////////////
// missing utility
/////////////////////////////////////////////////////////////////////////////
char* stristr(const char* a,const char* b) ;
#ifdef __ROOT__
int strnicmp(const char* a,const char*b,size_t n) ;
#endif
/////////////////////////////////////////////////////////////////////////////
#endif // _WIN32


#define EASYEXP_SYNTHESIS
#ifdef EASYEXP_SYNTHESIS
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// EasyExp Synthesis
//
// string EasyExpExtraction(const vector<string>& cmatchs // matching string
// 			   ,const vector<string>& cunmatchs // umnatch string
//			   ,int mode // 0: try '*' '?' first, if fail enumerate
//                                   // 1: emumerate variation
//			   ,int LIST_LIMIT); // emumeration list limit
//
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#define EASYEXP_MULTI_THREAD
#ifdef EASYEXP_MULTI_THREAD
extern unsigned int EasyExp_num_threads;
extern unsigned int EasyExp_num_threads_limit;
extern unsigned int EasyExp_thread_timeout;
#endif

#ifndef __MAKECINT__
/////////////////////////////////////////////////////////////////////////////
// private functions,  referenced in EasyExp.cxx and StatEasyExp.cxx
/////////////////////////////////////////////////////////////////////////////
void _commonsubstring(const string& a,const string& b,int MinString
			,int offset,vector<string>& r) ;
void _commonsubstring(const string& a,const string& b,int MinString,int offset,map<string,int>& comstr) ;
void commonsubstring(map<string,int>& comstr,const string& a,const string& b,int MinString) ;
int StrCmp_commonSubstr(const string& a
                        ,vector<string>::const_iterator first
			,vector<string>::const_iterator last
			,vector<double>& result
			,const string& param) ;
void MakeSearchString(const string& str,string& buf,int mode=0) ;
int MakeSearchNumberRange(map<string,int> prestr,statistics& st,string& expr) ;
void EasyExpExtraction_comstr(const vector<string>& cmatchs
			      ,vector<map<string,int> >& prestr_list
			      ,vector<statistics>& prestr_size
			      ,vector<string>& comstr_list);
void SmartEasyExpExtraction_comstr(const vector<string>& cmatchs_in
				   ,vector<map<string,int> >& prestr_list
				   ,vector<statistics>& prestr_size
				   ,vector<string>& comstr_list
				   ,int position_mode=0) ;
/////////////////////////////////////////////////////////////////////////////
#endif
 
/////////////////////////////////////////////////////////////////////////////
// public functions
/////////////////////////////////////////////////////////////////////////////
string EasyExpSynthesis(const vector<string>& matchs
			,const vector<string>& unmatchs
			,int generality_mode=0,int LIST_LIMIT=100
			,int position_mode=0) ;

// This function is faster but has issue in calculating probability
// Use StatEasyExpExtraction in StatEasyExp.h, instead.
string EasyExpStatSynthesis(const vector<string>& matchs
			    ,const vector<string>& unmatchs
			    ,int generality_mode=0,int LIST_LIMIT=100
			    ,int position_mode=0) ;

/////////////////////////////////////////////////////////////////////////////
// Old name, keeping this for compatibility
/////////////////////////////////////////////////////////////////////////////
inline string EasyExpExtraction(const vector<string>& matchs
				,const vector<string>& unmatchs
				,int mode=0,int LIST_LIMIT=100) {
  return(EasyExpSynthesis(matchs,unmatchs,mode,LIST_LIMIT));
}
inline string EasyExpStatExtraction(const vector<string>& matchs
				    ,const vector<string>& unmatchs
				    ,int mode=0,int LIST_LIMIT=100) {
  return(EasyExpStatSynthesis(matchs,unmatchs,mode,LIST_LIMIT));
}
/////////////////////////////////////////////////////////////////////////////
#endif // EASYEXP_SYNTHESIS

#define TFIDF
#ifdef TFIDF
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
// TF-IDF (TermFrequency * InverseDocumentFrequency)
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////
class TermFrequency {
public:
  int    m_totalcount;     // total term count in a document
  string m_doc;    // target document
  bool   m_single; // if set, don't count number of terms, but just 1/0
  map<string,int> m_termcount; // <term,count> 
public:
  //TermFrequency() { m_totalcount=0; }
  typedef map<string,int>::iterator iterator;
  TermFrequency(const string& doc,bool single) : m_doc(doc), m_totalcount(0), m_single(single) { }
 TermFrequency(const TermFrequency& a) : m_totalcount(a.m_totalcount), m_doc(a.m_doc)
    , m_single(a.m_single), m_termcount(a.m_termcount) { }
  void add(const string& term) {
    if(m_single) {
      if(!m_termcount[term]) {
	m_termcount[term]=1;
	++m_totalcount;
      }
    }
    else {
      ++m_termcount[term];
      ++m_totalcount;
    }
  }
  double termfrequency(const string& term) 
    { return((double)m_termcount[term]/m_totalcount); }
  int termcount(const string& term) { return(m_termcount[term]); }
  int totaltermcount() { return(m_totalcount); }
  iterator begin() { return(m_termcount.begin()); }
  iterator end() { return(m_termcount.end()); }
  void disp(FILE* fp=stdout) {
    iterator first,last=m_termcount.end();
    for(first=m_termcount.begin();first!=last;++first) {
      fprintf(fp,"%s %d,",(*first).first.c_str(),(*first).second);
    }
    fprintf(fp,"\n");
  }
};

/////////////////////////////////////////////////////////////////////////
class TermFrequencyTable {
public:
  map<string,TermFrequency*> m_TF; // <doc,tf>
  bool  m_single;
public:
  typedef map<string,TermFrequency*>::iterator iterator;
  TermFrequencyTable() : m_single(false) {}
  TermFrequencyTable(const TermFrequency& a) : m_single(a.m_single) {
    iterator first=begin(),last=end();
    for(;first!=last;++first) {
      m_TF[(*first).first] = new TermFrequency(*(*first).second);
    }
  }
  void clear() {
    iterator first=begin(),last=end();
    for(;first!=last;++first) delete (*first).second;
    m_TF.clear();
  }
  ~TermFrequencyTable() {clear();}
  void setSingleCount() {m_single=true; }
  void add(const string& doc,const string& term) {
    TermFrequency* tf = m_TF[doc];
    if(!tf) { // new document
      tf = new TermFrequency(doc,m_single);
      m_TF[doc] = tf;
      //printf("new '%s' %g '%s' %d\n",term.c_str(),(double)tf->termcount(term),doc.c_str(),tf->totalcount());
    }
    tf->add(term);
    //printf("add '%s' %g '%s' %d\n",term.c_str(),(double)tf->termcount(term),doc.c_str(),tf->totalcount());
  }
  double termfrequency(const string& doc,const string& term) {
    TermFrequency* tf = m_TF[doc];
    if(!tf) return(0.0);
    return((double)tf->termfrequency(term));
    //printf("tf '%s' %g '%s' %d\n",term.c_str(),(double)tf->termcount(term),doc.c_str(),tf->totalcount());
    //return((double)tf->termcount(term)/tf->totalcount());
  }
  int termcount(const string& doc,const string& term) {
    TermFrequency* tf = m_TF[doc];
    if(!tf) return(0.0);
    return(tf->termcount(term));
  }
  int totaltermcount(const string& doc) {
    TermFrequency* tf = m_TF[doc];
    if(!tf) return(0.0);
    return(tf->totaltermcount());
  }
  iterator begin() { return(m_TF.begin()); }
  iterator end() { return(m_TF.end()); }
  void disp(FILE* fp=stdout) {
    iterator first,last=m_TF.end();
    fprintf(fp,"Term Freuqncy in documents\n");
    for(first=m_TF.begin();first!=last;++first) {
      fprintf(fp,"doc=%s : ",(*first).first.c_str());
      (*first).second->disp(fp);
    }
  }
};

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
class DocumentFrequency {
public:
  string m_term;        // target term
  map<string,int> m_df; // <doc,exist>
  bool m_single;        // if set, don't count number of docs using the term
  //long m_dmy; // gcc 64bit bug? memory destruction
public:
  DocumentFrequency() {}
  typedef map<string,int>::iterator iterator;
  DocumentFrequency(const string& term,bool single)
    : m_term(term), m_single(single) { }
  DocumentFrequency(const DocumentFrequency& a) : m_term(a.m_term)
    , m_df(a.m_df), m_single(a.m_single) { }
  int add(const string& doc) {
    if(!m_single) {
      ++m_df[doc];
      return(1);
    }
    else {
      if(!m_df[doc]) {
	m_df[doc]=1;
	return(1);
      }
      return(0);
    }
  }
  int doccount() {return(m_df.size());}
  iterator begin() { return(m_df.begin()); }
  iterator end() { return(m_df.end()); }
  void disp(FILE* fp=stdout) {
    iterator first,last=m_df.end();
    for(first=m_df.begin();first!=last;++first) {
      fprintf(fp,"%s %d,",(*first).first.c_str(),(*first).second);
    }
    fprintf(fp,"\n");
  }
};

/////////////////////////////////////////////////////////////////////////
class InverseDocumentFrequencyTable {
 public:
  map<string,int>   m_D;
  map<string,DocumentFrequency*> m_DF; // <term,df>
  bool m_single;
  //long m_dmy2; // gcc 64bit bug? memory destruction
 public:
  InverseDocumentFrequencyTable() : m_single(false) {}
  InverseDocumentFrequencyTable(const InverseDocumentFrequencyTable& a)
    : m_single(a.m_single) {
    iterator first=begin(),last=end();
    for(;first!=last;++first) {
      m_DF[(*first).first] = new DocumentFrequency(*(*first).second);
    }
  }
  void clear() {
    iterator first=begin(),last=end();
    for(;first!=last;++first) delete (*first).second;
    m_D.clear();
    m_DF.clear();
  }
  ~InverseDocumentFrequencyTable() {clear();}
  void setSingleCount() {m_single=true; }
  typedef map<string,DocumentFrequency*>::iterator iterator;
  void add(const string& doc,const string& term) {
    DocumentFrequency* df = m_DF[term];
    if(!df) { // new document
      df = new DocumentFrequency(term,m_single);
      m_DF[term] = df;
    }
    m_D[doc] += df->add(doc);
  }
  double idf(const string& term) {
    DocumentFrequency* df = m_DF[term];
    if(!df) return(0);
    double idf=log((double)m_D.size()/(double)df->doccount());
    return(idf);
  }
  iterator begin() { return(m_DF.begin()); }
  iterator end() { return(m_DF.end()); }
  void disp(FILE* fp=stdout) {
    fprintf(fp,"Inverse Document Freuqncy of terms\n");
    map<string,int>::iterator first1,last1=m_D.end();
    for(first1=m_D.begin();first1!=last1;++first1) {
      fprintf(fp,"doc=%s %d, ",(*first1).first.c_str(),(*first1).second);
    }
    fprintf(fp,"\n");
    iterator first,last=m_DF.end();
    for(first=m_DF.begin();first!=last;++first) {
      fprintf(fp,"term=%s : ",(*first).first.c_str());
      (*first).second->disp(fp);
    }
  }
};

/////////////////////////////////////////////////////////////////////////
// TF_IDF class needs Collection class, which is not included in EasyExp library
// Hence, this class is implemented in include/CSVclustering.h
//class TF_IDF ;
/////////////////////////////////////////////////////////////////////////
// TF-IDF class
//    Collection & Dense Vector version
/////////////////////////////////////////////////////////////////////////
class TF_IDF {
 public:
  TermFrequencyTable            TF;
  InverseDocumentFrequencyTable IDF;
  bool            m_caseinsensitive;
  bool            m_ignorenumbers;
 public:
  ///////////////////////////////////////////////////////////
  // Setup
  ///////////////////////////////////////////////////////////
  TF_IDF() : m_caseinsensitive(true), m_ignorenumbers(true) {}
  TF_IDF(const TF_IDF& a) : m_caseinsensitive(a.m_caseinsensitive)
    , m_ignorenumbers(a.m_ignorenumbers), TF(a.TF), IDF(a.IDF) {}
  void clear() { TF.clear(); IDF.clear(); }
  ~TF_IDF() {}
  void casesensitive() {m_caseinsensitive=false;}
  void caseinsensitive(bool in=true) {m_caseinsensitive=in;}
  void ignorenumbers(bool in=true) {m_ignorenumbers=in;}
  void takenumbers() {m_ignorenumbers=false;}
  void setSingleCount() {TF.setSingleCount(); IDF.setSingleCount();}

  ///////////////////////////////////////////////////////////
  // Input
  ///////////////////////////////////////////////////////////
  void add(const string& name,const string& contents) {
    TF.add(name,contents);
    IDF.add(name,contents);
  }
  ///////////////////////////////////////////////////////////
  void add(const string& name,const string& contents,ReadF& f) {
    f.parse(contents);
    for(int i=1;i<f.argv.size();i++) {
      if(!m_ignorenumbers || !isdigit(f.argv[i][0])) {
	TF.add(name,f.argv[i]);
	IDF.add(name,f.argv[i]);
      }
    }
  }
  ///////////////////////////////////////////////////////////
  void add(const string& name,const string& contents,ReadF& f,EasyExp& e) {
    f.parse(contents);
    string term;
    if(e.replace_element_size()==0) {
      for(int i=1;i<f.argv.size();i++) {
	if(e.match(f.argv[i])) {
	  TF.add(name,f.argv[i]);
	  IDF.add(name,f.argv[i]);
	}
      }
    }
    else {
      string term;
      for(int i=1;i<f.argv.size();i++) {
	if(e.replace(f.argv[i],term)) {
	  //printf("%d %s %s %s\n",i,name.c_str(),f.argv[i],term.c_str());
	  TF.add(name,term);
	  IDF.add(name,term);
	}
      }
    }
  }
  ///////////////////////////////////////////////////////////
  void add(const string& name,const string& contents
	   ,ReadF& f,vector<EasyExp>& ve) {
    f.parse(contents);
    if(ve.size()==0) {
      for(int i=1;i<f.argv.size();i++) {
	TF.add(name,f.argv[i]);
	IDF.add(name,f.argv[i]);
      }
    }
    else {
      //for(int i=1;i<f.argv.size()&&f.argv[i]&&strlen(f.argv[i]);i++) {
      for(int i=1;i<f.argv.size()&&strlen(f.argv[i]);i++) {
	for(int j=0;j<ve.size();j++) {
	  EasyExp& e=ve[j];
	  string term;
	  if(e.replace_element_size()>0) {
	    if(e.replace(f.argv[i],term)) {
	      TF.add(name,term);
	      IDF.add(name,term);
	      break;
	    }
	  }
	  else {
	    if(e.match(f.argv[i])) {
	      TF.add(name,f.argv[i]);
	      IDF.add(name,f.argv[i]);
	      break;
	    }
	  }
	}
      }
    }
  }
  ///////////////////////////////////////////////////////////
  // OLD 
  void Add(const string& name,const string& contents
		,const string& sep=" \t\v\r\f()[]{}=.,+-*/%!|&$#<>?~@^_:;'\"\"\\\\"
		,const string& quote="") {
    ReadF f;
    f.setseparator(sep);
    f.setquotation(quote);
    f.parse(contents);
    for(int i=1;i<f.argc;i++) {
      if(!m_ignorenumbers || !isdigit(f.argv[i][0])) {
	//printf("%d %s %s\n",i,name.c_str(),f.argv[i]);
	TF.add(name,f.argv[i]);
	IDF.add(name,f.argv[i]);
      }
    }
  }
  ///////////////////////////////////////////////////////////
  void ReadFile(const char* fname,EasyExp& e
		,const string& sep=" \t\v\r\f()[]{}=.,+-*/%!|&$#<>?~@^_:;'\"\"\\\\"
		,const string& quote="") {
    ReadF f(fname);
    f.setseparator(sep);
    f.setquotation(quote);
    while(f.read()) {
      if(e.match(f.argv[0],m_caseinsensitive)) {
	for(int i=1;i<f.argc;i++) {
	  if(!m_ignorenumbers || !isdigit(f.argv[i][0])) {
	    TF.add(fname,f.argv[i]);
	    IDF.add(fname,f.argv[i]);
	  }
	}
      }
    }
  }

  ///////////////////////////////////////////////////////////
  // Output
  ///////////////////////////////////////////////////////////
  map<string,double> getIDF() {
    map<string,double> termidf;
    InverseDocumentFrequencyTable::iterator firstd,lastd=IDF.end();
    for(firstd=IDF.begin();firstd!=lastd;++firstd) {
      string term = (*firstd).first;
      double idf = IDF.idf(term);
      termidf[term] = idf;
    }
    return(termidf);
  }
  
  ///////////////////////////////////////////////////////////
  vector<string> getTerm() {
    vector<string> vterm;
    InverseDocumentFrequencyTable::iterator firstd,lastd=IDF.end();
    for(firstd=IDF.begin();firstd!=lastd;++firstd) {
      string term = (*firstd).first;
      vterm.push_back(term);
    }
    return(vterm);
  }
  ///////////////////////////////////////////////////////////
  vector<string> getDoc() {
    vector<string> vdoc ;
    vdoc.clear();
    TermFrequencyTable::iterator firstt,lastt=TF.end();
    for(firstt=TF.begin();firstt!=lastt;++firstt) {
      string doc = (*firstt).first;
      vdoc.push_back(doc);
    }
  return(vdoc);
  }

#if 0
  // Output methods are defined in _TF_IDF.h as interpreted function.
  // because target format is defined in other library.
  Collection TFIDF_TermCollection();
  Collection TFIDF_DocCollection();
  int TFIDF_DocVector(vector<string>& vdoc,vector<FSparseVector>& vsv) ;
  int TFIDF_TermVector(vector<string>& vterm,vector<FSparseVector>& vsv) ;
#endif

  ///////////////////////////////////////////////////////////
  void disp(FILE* fp=stdout) {
    TF.disp(fp);
    IDF.disp(fp);
  }
};
#endif // TFIDF

#endif

