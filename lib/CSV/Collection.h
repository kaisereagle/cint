/////////////////////////////////////////////////////////////////////////////
// Collection.h
//
//  CSV table manipulator
//
/////////////////////////////////////////////////////////////////////////////
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

#ifndef COLLECTION_H
#define COLLECTION_H

#include <map>
#include <deque>
#include <vector>
#include <algorithm>
#include "Value.h"
#include "ReadF.h"
#include "EasyExp.h"
#include "Stat.h"
#ifdef UNIQUE_HIST
#include "Histogram.h"
#endif
using namespace std;
#include <cstdio>
#include <cstdarg>
#include <cstddef>
#include <cstdlib>
#include <climits>

#ifdef __CINTLIB__
#include <G__ci.h>
#endif

#include "MultiByteChar.h"
#ifndef E__MULTIBYTELIB
#define E__MULTIBYTE
#endif

#define E__EFFICIENTREPLACE

#define MULTIPLEKEYS
#define COLLECTIONCOMPARE
#define MATHOPERATOR
//#define MAXCOLUMNINDEX

#define INVALIDSTR "###"
//#define INVALIDSTR ""


/////////////////////////////////////////////////////////////////////////////
// Implementation option
/////////////////////////////////////////////////////////////////////////////

// Change 'non line persistent' functions to 'line persistent'
//#ifdef LINE_PERSISTENCY

//#define MULTIKEY

/////////////////////////////////////////////////////////////////////////////
#if (defined(WIN32) || defined(__linux)) && !defined(NOMINMAXCSV)
//template<class T> T max(const T& a,const T& b) {return(a>b?a:b);}
//template<class T> T min(const T& a,const T& b) {return(a<b?a:b);}
//template<class T> T max(T a,T b) {return(a>b?a:b);}
//template<class T> T min(T a,T b) {return(a<b?a:b);}
#endif

#define CHARIF

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// Utility
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// convert A,B,...,Z,AA,AB,AC...  to integer
int strtonum(const string& s) ;
#ifdef CHARIF
inline int strtonum(const char* s) { return(strtonum(string(s))); }
#endif

/////////////////////////////////////////////////////////////////////////////
// 
#ifdef WIN32
template<class T> 
typename T::iterator numtoiter(T& container,int x) {
  T::iterator first=container.begin();
  T::iterator last =container.end(), beforelast;
  int i=0;
  while(i<x && first!=last) {beforelast=first; ++i; ++first;}
  if(first==last) return(beforelast);
  return(first);
}
#else
#endif


/////////////////////////////////////////////////////////////////////////////
class CollectionStatistics {
 public:
  CollectionStatistics() { init(); }
  void init() { m_stat.clear(); }
  statistics& get(int x) {
    if(x>=0 && x<(int)m_stat.size()) return(m_stat[x]);
    else return(m_dummy);
  }
  statistics& get(const string& s) {return(get(strtonum(s)));}
  statistics& operator[](int x) { return(get(x)); }
  statistics& operator[](const string& s) { return(get(s)); }
  void push_front(const statistics& stat) { m_stat.push_front(stat); }
  void push_back(const statistics& stat) { m_stat.push_back(stat); }
  void pop_front() { m_stat.pop_front(); }
  void pop_back() { m_stat.pop_back(); }
  statistics& front() { return(m_stat.front()); }
  statistics& back() { return(m_stat.back()); }
  int erase(int x);
  int erase(int x1,int x2);
  int size() const { return(m_stat.size()); }

  CollectionStatistics  column(int from,int to= -1);
  CollectionStatistics _column(int from,int to= -1);

  CollectionStatistics  column(const vector<int>& p);
  CollectionStatistics _column(const vector<int>& p);

  void minmax(double& min,double& max) const;
  
  void dispmin(FILE* fp=stdout) const;
  void dispmax(FILE* fp=stdout) const;
  void dispsigma(FILE* fp=stdout) const;
  void dispstddev(FILE* fp=stdout) const;
  void dispmean(FILE* fp=stdout) const;
  void dispnsample(FILE* fp=stdout) const;
  void disperror(FILE* fp=stdout) const;

  void disp(FILE* fp=stdout) const;

#ifdef CHARIF
  statistics& get(const char* s) {return(get(strtonum(string(s))));}
  statistics& operator[](const char* s) { return(get(string(s))); }
#endif


 //private:
  statistics        m_dummy;
  deque<statistics> m_stat;
};

/////////////////////////////////////////////////////////////////////////////
inline int G__ateval(const CollectionStatistics& a) {a.disp(); return(1);}

/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
class Line {
 public:
  Line() { init(); }
  void init() { m_column.clear(); }
  void clear() { init(); }
  void set(const ReadF& f);
  void set(const char* a1=0 ,const char* a2=0 ,const char* a3=0
	   ,const char* a4=0 ,const char* a5=0 ,const char* a6=0
	   ,const char* a7=0 ,const char* a8=0 ,const char* a9=0
	   ,const char* a10=0 ,const char* a11=0 ,const char* a12=0
	   ,const char* a13=0 ,const char* a14=0 ,const char* a15=0
	   ,const char* a16=0 ,const char* a17=0 ,const char* a18=0
	   ,const char* a19=0 ,const char* a20=0 ,const char* a21=0) {
    m_column.clear();
    if(a1) m_column.push_back(a1);
    if(a2) m_column.push_back(a2);
    if(a3) m_column.push_back(a3);
    if(a4) m_column.push_back(a4);
    if(a5) m_column.push_back(a5);
    if(a6) m_column.push_back(a6);
    if(a7) m_column.push_back(a7);
    if(a8) m_column.push_back(a8);
    if(a9) m_column.push_back(a9);
    if(a10) m_column.push_back(a10);
    if(a11) m_column.push_back(a11);
    if(a12) m_column.push_back(a12);
    if(a13) m_column.push_back(a13);
    if(a14) m_column.push_back(a14);
    if(a15) m_column.push_back(a15);
    if(a16) m_column.push_back(a16);
    if(a17) m_column.push_back(a17);
    if(a18) m_column.push_back(a18);
    if(a19) m_column.push_back(a19);
    if(a20) m_column.push_back(a20);
    if(a21) m_column.push_back(a21);
  }
  value getconst(int x) const {
    if(x>=0 && x<(int)m_column.size()) return(m_column[x]);
    else {
      //fprintf(stderr,"Warning: Line::getconst() index %d out of range. size=%d\n",x,size());
#ifdef __CINTLIB__
      //G__printlinenum();
#endif
      //if(size()) disp(stderr);
      //exit(255);
      return(m_dummy);
    }
  }
  value& get(int x) {
    if(x>=0 && x<(int)m_column.size()) return(m_column[x]);
    else return(m_dummy);
  }
  value& get(const string& s) {return(get(strtonum(s)));}
  value& Get(int x);
  value& operator[](int x) { return(get(x)); }
  value& operator[](const string& s) { return(get(s)); }
  void stat(CollectionStatistics& s);
  void disp(FILE* fp=stdout) const ;

#ifdef COLLECTIONCOMPARE
  const value& operator[](int x) const { return(get(x)); }
  const value& operator[](const string& s) const { return(get(s)); }
  const value& get(int x) const {
    if(x>=0 && x<(int)m_column.size()) return(m_column[x]);
    else return(m_dummy);
  }
  const value& get(const string& s) const {return(get(strtonum(s)));}
#endif


  Line  column(int from,int to= -1);
  Line _column(int from,int to= -1);

  Line  column(const vector<int>& p);
  Line _column(const vector<int>& p);

  int empty();
  int _empty() { return(!empty()); }

  vector<int>  emptycolumns();
  vector<int> _emptycolumns();

  void push_front(const value& a) {m_column.push_front(a);}
  void push_back(const value& a) {m_column.push_back(a);}
  void push_front(const Line& a);
  void push_back(const Line& a);
  void pop_front(){m_column.pop_front();}
  void pop_back(){m_column.pop_back();}
  value& front() { return(m_column.front()); }
  value& back() { return(m_column.back()); }
  int erase(int x);
  int erase(int x1,int x2);
  int size() const { return(m_column.size()); }
  void resize(int i) { m_column.resize(i); }

  // Numeric operation
  Line& add(const Line& a) ;
  Line& sub(const Line& a) ;
  Line& mul(const Line& a) ;
  Line& div(const Line& a) ;
  Line& mod(const Line& a) ;
  Line& add(int x,double a) {get(x).set(get(x).d+a);return(*this);}
  Line& sub(int x,double a) {get(x).set(get(x).d-a);return(*this);}
  Line& mul(int x,double a) {get(x).set(get(x).d*a);return(*this);}
  Line& div(int x,double a) {if(a!=0.0) get(x).set(get(x).d/a); return(*this);}
  Line& mod(int x,double a) {if(a!=0.0) get(x).set(int(get(x).d)%int(a)); return(*this);}

  Line& add(int x,int x2,int xo) 
    {get(xo).set(get(x).d+get(x2).d);return(*this);}
  Line& sub(int x,int x2,int xo) 
    {get(xo).set(get(x).d-get(x2).d);return(*this);}
  Line& mul(int x,int x2,int xo) 
    {get(xo).set(get(x).d*get(x2).d);return(*this);}
  Line& div(int x,int x2,int xo) 
    {if(get(x2).d!=0.0) get(xo).set(get(x).d/get(x2).d); return(*this);}
  Line& mod(int x,int x2,int xo) 
    {if(get(x2).d!=0.0) get(xo).set(int(get(x).d)%int(get(x2).d)); return(*this);}

  // hyperlink
  Line& hyperlink(int x);

  // string replacement, column
  Line& replace(int x,const string& match,const string& rep);
  Line& replace(int x,EasyExp& r,const string& rep);
  // string replacement, line
  Line& replace(const string& match,const string& rep);
  Line& replace(EasyExp& r,const string& rep);

  // Set value
  Line& let(int x,const string& val) { Get(x).set(val); return(*this); }
  Line& let(int x,double val) { Get(x).set(val); return(*this); }
  Line& let(int x,const value& val) { Get(x) = val; return(*this); }
  Line& let(int x,const vector<double>& v,int offsetmode=0,int offset=0) ;

  Line match(const string& key,bool icase=false);
  Line match(const char* key,bool icase=false) 
    { return(match(string(key),icase)); }

#ifdef CHARIF
  value& get(const char* s) {return(get(strtonum(string(s))));}
  value& operator[](const char* s) { return(get(string(s))); }
  Line& let(int x,const char* val) { Get(x).set(string(val)); return(*this); }
#endif

  statistics stat(int from=0,int to= -1) {
    statistics s;
    if(to<0) to = size()-1;
    Line line = column(from,to);
    for(int i=0;i<line.size();i++) s.add(line[i].todouble());
    return(s);
  }
  double min(int from=0,int to= -1) {
    return(stat(from,to).min());
  }
  double max(int from=0,int to= -1) {
    return(stat(from,to).max());
  }
  double mean(int from=0,int to= -1) {
    return(stat(from,to).mean());
  }
  double stddev(int from=0,int to= -1) {
    return(stat(from,to).stddev());
  }

#ifdef MAXCOLUMNINDEX
  int maxcolumnindex(int istart=0,int istop=INT_MAX) const {
    double tmp = -1e99;
    int maxcol;
    for(int i=istart;i<m_column.size()&&i<istop;i++) {
      if(tmp<m_column[i].todouble()) { tmp=m_column[i].todouble(); maxcol=i; }
    }
    return(maxcol);
  } 
  int mincolumnindex(int istart=0,int istop=INT_MAX) const {
    double tmp = 1e99;
    int mincol;
    for(int i=istart;i<m_column.size()&&i<istop;i++) {
      if(tmp>m_column[i].todouble()) { tmp=m_column[i].todouble(); mincol=i; }
    }
    return(mincol);
  } 
#endif

  void setlinenum(int linenum) { m_linenum=linenum; }
  int  linenum() const { return(m_linenum); }

#ifdef COLLECTIONCOMPARE
  friend bool operator==(const Line& a,const Line& b) ;
  friend bool operator!=(const Line& a,const Line& b) ;
  friend bool operator<(const Line& a,const Line& b) ;
#endif
#ifdef MATHOPERATOR
  friend Line operator+(const Line& a,const Line& b) ;
  friend Line operator-(const Line& a,const Line& b) ;
  friend Line operator*(const Line& a,const Line& b) ;
  friend Line operator/(const Line& a,const Line& b) ;
#endif

 //private:
  typedef deque<value>::const_iterator const_iterator;
  typedef deque<value>::iterator iterator;
  deque<value> m_column;
  value        m_dummy;
  int          m_linenum;
};
////////////////////////////////////////////////////////////////////////
#ifdef COLLECTIONCOMPARE
bool operator==(const Line& a,const Line& b) ;
inline bool operator!=(const Line& a,const Line& b){return(!operator==(a,b));}
bool operator<(const Line& a,const Line& b) ;
#else
inline bool operator==(const Line& a,const Line& b) { return(&a==&b); }
inline bool operator!=(const Line& a,const Line& b) { return(&a!=&b); }
inline bool operator<(const Line& a,const Line& b) { return(&a<&b); }
#endif
int compare(const Line& a,const Line& b,int x) ;
int compare(const Line& a,const Line& b,const int* x,int n) ;
inline int compare(const Line& a,const Line& b,const vector<int> x) 
  { return(compare(a,b,&x[0],x.size())); }

#ifdef MATHOPERATOR
Line operator+(const Line& a,const Line& b) ;
Line operator-(const Line& a,const Line& b) ;
Line operator*(const Line& a,const Line& b) ;
Line operator/(const Line& a,const Line& b) ;
#endif

////////////////////////////////////////////////////////////////////////
inline int G__ateval(const Line& a) {a.disp(); return(1);}

/////////////////////////////////////////////////////////////////////////////
// Function object for sorting Collection
//
// sort(iter,iter,comp);
/////////////////////////////////////////////////////////////////////////////
class CompareLine {
 public:
  enum mode_t { Auto, String, Number, LineNum };
  CompareLine(int x=0,mode_t mode=Auto) { setrule(x,mode); }
  bool operator()(const Line* a,const Line *b ) ;
  bool operator()(const Line& a,const Line& b ) ;
  void setrule(int x,mode_t mode=String) { m_x=x; m_mode=mode; }
 //private:
  int m_x;
  mode_t m_mode;
};

/////////////////////////////////////////////////////////////////////////////
// Iterator concept 1
/////////////////////////////////////////////////////////////////////////////
class Collection;
class EachValue {
 private:
  deque<value> m_v;
  unsigned int m_i;
 public:
  value& operator[](unsigned int i) { return(m_v[i]); }
  unsigned int size() const { return(m_v.size()); }
  void push_back(const value& v) { m_v.push_back(v); m_i=m_v.size(); }
  void clear() { m_v.clear(); m_i=0; }

  EachValue() { m_i=0; }
  EachValue(const deque<value>& v) { m_v = v; m_i=v.size(); }
  EachValue(const EachValue& fe) { m_v = fe.m_v; m_i = fe.m_i; }
 private:
  void init() { m_i=0; }
  bool  isvalid() const { return(m_i<m_v.size()?true:false); }
  int next() {
    if(!isvalid()) m_i=0; else ++m_i;
    return(isvalid());
  }
  const char* c_str() { return(m_v[m_i].c_str()); }
  string tostring() { return(m_v[m_i].tostring()); }
  double todouble() { return(m_v[m_i].todouble()); }
  int toint() { return(m_v[m_i].toint()); }
};


/////////////////////////////////////////////////////////////////////////////
class Sheet;
class CollectionIterator;
class Collection 
#ifdef E__MULTIBYTELIB
: public G__MultiByteMode
#endif
{
 public:
  typedef deque<Line*>::const_iterator const_iterator;
  typedef deque<Line*>::iterator iterator;

  Collection() { init(); }
  Collection(const Collection& col) ;
  Collection(Sheet* psheet) { init(); p_sheet=psheet; m_isauto=0; }
  ~Collection() ;

  Collection duplicate() const; // duplicate line object
  Collection& duplicate(const Collection& col); // duplicate line object

  void setsheet(Sheet* sheet) ;
  void init() ; 
  void clear() { init(); }
  void clear_lines() { m_plines.clear(); }
  void copy_lines(const Collection& col) { m_plines = col.m_plines; }

  ////////////////////////////////////////////////////////////////
  // Container operation
  void push_front(Line* p) { m_plines.push_front(p); }
  void push_back(Line* p) { m_plines.push_back(p); }
  void push_front(const Line& line) ; 
  void push_back(const Line& line) ; 
  void push_back(const char* a1=0 ,const char* a2=0 ,const char* a3=0
	   ,const char* a4=0 ,const char* a5=0 ,const char* a6=0
	   ,const char* a7=0 ,const char* a8=0 ,const char* a9=0
	   ,const char* a10=0 ,const char* a11=0 ,const char* a12=0
	   ,const char* a13=0 ,const char* a14=0 ,const char* a15=0
	   ,const char* a16=0 ,const char* a17=0 ,const char* a18=0
	   ,const char* a19=0 ,const char* a20=0 ,const char* a21=0) {
    Line l;
    l.set(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,a17,a18,a19,a10);
    push_back(l);
  }
  void pop_front() { m_plines.pop_front(); }
  void pop_back() { m_plines.pop_back(); }
  Line& front() { return(*m_plines.front()); }
  Line& back() { return(*m_plines.back()); }
  int erase(int y);
  int erase(int y1,int y2);
  int erase_by_linenum(int l);
  int erase_by_linenum(int l1,int l2);
  int size() const { return(m_plines.size()); }
  void resize(int i) ; 
  void column_resize(int i) ; 
  void resize(int maxrow,int maxcol)
  {resize(maxrow); column_resize(maxcol);}
  int maxlinesize()  const ;
  int maxline()  const  { return(maxlinesize()); } 

  ////////////////////////////////////////////////////////////////
  // column index vectorization
  ////////////////////////////////////////////////////////////////
  vector<int> getxvec(int x1=-1,int x2=-1,int x3=-1,int x4=-1) {
    vector<int> xs;
    if(x1>=0) xs.push_back(x1);
    if(x2>=0) xs.push_back(x2);
    if(x3>=0) xs.push_back(x3);
    if(x4>=0) xs.push_back(x4);
    return(xs);
  }
  vector<int> getxvec(const string& key1,const string& key2=INVALIDSTR,const string& key3=INVALIDSTR,const string& key4=INVALIDSTR) {
    vector<int> xs;
    if(key1!=INVALIDSTR) xs.push_back(getx(key1));
    if(key2!=INVALIDSTR) xs.push_back(getx(key2));
    if(key3!=INVALIDSTR) xs.push_back(getx(key3));
    if(key4!=INVALIDSTR) xs.push_back(getx(key4));
    return(xs);
  }
  vector<int> getxvec(const vector<string>& keys) {
    vector<int> xs;
    for(unsigned int i=0;i<keys.size();i++) 
      if(keys[i]!=INVALIDSTR) xs.push_back(getx(keys[i]));
    return(xs);
  }

#ifdef MAXCOLUMNINDEX
  ////////////////////////////////////////////////////////////////
  Collection maxcolumnindex(int istart=0,int istop=INT_MAX) const {
    Collection r;
    Line l; l.push_back("");
    for(int i=0;i<size();i++) {
      l.let(0,get(i).maxcolumnindex(istart,istop));
      r.push_back(l);
    }
    return(r);
  }
  Collection mincolumnindex(int istart=0,int istop=INT_MAX) const {
    Collection r;
    Line l; l.push_back("");
    for(int i=0;i<size();i++) {
      l.let(0,get(i).mincolumnindex(istart,istop));
      r.push_back(l);
    }
    return(r);
  }
  Line maxrowindex(int istart=0,int istop=INT_MAX) const {
    Line l;
  }
  Line minrowindex(int istart=0,int istop=INT_MAX) const {
    Line l;
  }
#endif

  // Value extraction
  //value& get(int y,int x) {}
  //value& get(const string& y,const string& x) {}

  ////////////////////////////////////////////////////////////////
  // Cell extraction
  Collection  match_cell(const string& expr,bool icase=false
			 ,bool surround=true) const;
  Collection _match_cell(const string& expr,bool icase=false) const;
  map<string,int> unique_cell_map() const;
  Collection  unique_cell_count() const;
  int strcell2numcell() ;

  ////////////////////////////////////////////////////////////////
  // Line extraction
  Line& get(int y);
  Line& operator[](int y) { return(get(y)); }
#ifdef COLLECTIONCOMPARE
  const Line& get(int y) const;
  const Line& operator[](int y) const { return(get(y)); }
#endif

  // Line extraction
  Collection  match(const string& key,const string& expr,bool icase=false);
  Collection _match(const string& key,const string& expr,bool icase=false);
  Collection  match(const string& key,double val){return(range(key,val,val));}
  Collection _match(const string& key,double val){return(_range(key,val,val));}
  Collection  match(const string& key,Collection& val,const string& keyv,bool icase=false);
  Collection _match(const string& key,Collection& val,const string& keyv,bool icase=false);
  virtual Collection  match(int x,const string& expr,bool icase=false);
  virtual Collection _match(int x,const string& expr,bool icase=false);
  Collection  match(int x,double val){return(range(x,val,val));}
  Collection _match(int x,double val){return(_range(x,val,val));}
  Collection  match(int x,Collection& val,int keyv,bool icase=false);
  Collection _match(int x,Collection& val,int keyv,bool icase=false);
#ifdef CHARIF
  Collection  match(const char* key,const char* expr,bool icase=false)
    {return(match(string(key),string(expr),icase));}
  Collection _match(const char* str,const char* expr,bool icase=false)
    {return(_match(string(str),string(expr),icase));}
  Collection  match(const char* key,double val)
    {return(range(string(key),val,val));}
  Collection _match(const char* key,double val)
    {return(_range(string(key),val,val));}
  Collection  match(int x,const char* expr,bool icase=false)
    {return(match(x,string(expr),icase));}
  Collection _match(int x,const char* expr,bool icase=false)
    {return(_match(x,string(expr),icase));}
  Collection  match(const char* key,Collection& val,const char* keyv,bool icase=false)
    {return(match(string(key),val,string(keyv),icase));}
  Collection _match(const char* key,Collection& val,const char* keyv,bool icase=false)
    {return(_match(string(key),val,string(keyv),icase));}
#endif
  Collection match(const vector<int>& xs,const vector<string>& expr) {
    Collection r= *this;
    for(int i=0;i<xs.size()&&i<expr.size();i++) r=r.match(xs[i],expr[i]);
    return(r);
  }
  Collection match(const vector<string>& keys,const vector<string>& expr) {
    vector<int> xs;
    for(int i=0;i<keys.size();i++) xs.push_back(getx(keys[i]));
    return(match(xs,expr));
  }

  Collection  range(const string& key,double lower,double upper);
  Collection _range(const string& key,double lower,double upper);
  virtual Collection  range(int x,double lower,double upper);
  virtual Collection _range(int x,double lower,double upper);
#ifdef CHARIF
  Collection  range(const char* key,double lower,double upper)
    {return(range(string(key),lower,upper));}
  Collection _range(const char* key,double lower,double upper)
    {return(_range(string(key),lower,upper));}
#endif

#ifdef MULTIPLEKEYS
  Collection  match(const vector<int>& xs,const string& expr,bool icase=false);
  Collection _match(const vector<int>& xs,const string& expr,bool icase=false);
  Collection  match(const vector<int>& xs,Collection& val,int keyv,bool icase=false);
  Collection _match(const vector<int>& xs,Collection& val,int keyv,bool icase=false);
  Collection  range(const vector<int>& xs,double lower,double upper);
  Collection _range(const vector<int>& xs,double lower,double upper);
#endif

  virtual Collection  line(int from,int to= -1) const;
  virtual Collection _line(int from,int to= -1);

  Collection  empty(const string& key);
  Collection _empty(const string& key);
  virtual Collection  empty(int x);
  virtual Collection _empty(int x);
#ifdef CHARIF
  Collection  empty(const char* key){return(empty(string(key)));}
  Collection _empty(const char* key){return(_empty(string(key)));}
#endif
  virtual Collection  empty();
  virtual Collection _empty();

  Collection  emptycolumns(int y);
  Collection _emptycolumns(int y);
  Collection  emptycolumns();
  Collection _emptycolumns();

  Collection  enclosedlines(int x,const string& start,const string& stop,int n=1);
  Collection _enclosedlines(int x,const string& start,const string& stop,int n=1);
  Collection  enclosedlines(const string& key,const string& start,const string& stop,int n=1);
  Collection _enclosedlines(const string& key,const string& start,const string& stop,int n=1);
#ifdef CHARIF
  Collection  enclosedlines(int x,const char* start,const char* stop,int n=1)
    {return(enclosedlines(x,string(start),string(stop),n));}
  Collection _enclosedlines(int x,const char* start,const char* stop,int n=1)
    {return(_enclosedlines(x,string(start),string(stop),n));}
  Collection  enclosedlines(const char* key,const char* start,const char* stop,int n=1){return(enclosedlines(string(key),string(start),string(stop),n));}
  Collection _enclosedlines(const char* key,const char* start,const char* stop,int n=1){return(_enclosedlines(string(key),string(start),string(stop),n));}
#endif


  Collection  match_erase(int x,const string& expr,bool icase=false);
  Collection  match_erase(const string& key,const string& expr,bool icase=false);
  Collection  match_erase(const char* key,const char* expr,bool icase=false)
    {return(match_erase(string(key),string(expr),icase));}
#ifdef MULTIPLEKEYS
  Collection  match_erase(const vector<int>& xs,const string& expr,bool icase=false);
#endif

  Collection unique(const string& key,const string& key2=INVALIDSTR,const string& key3=INVALIDSTR,const string& key4=INVALIDSTR) { return(unique(getx(key),getx(key2),getx(key3),getx(key4))); }
  //Collection unique(const string& key) { return(unique(getx(key))); }
  virtual Collection unique(int x,int x2=-100,int x3=-100,int x4=-100);
#ifdef CHARIF
  Collection unique(const char* key,const char* key2,const char* key3,const char* key4) 
    { return(unique(getx(string(key)),getx(string(key2)),getx(string(key3)),getx(string(key4))));}
  Collection unique(const char* key,const char* key2,const char* key3) 
    { return(unique(getx(string(key)),getx(string(key2)),getx(string(key3))));}
  Collection unique(const char* key,const char* key2) 
    { return(unique(getx(string(key)),getx(string(key2))));}
  Collection unique(const char* key) { return(unique(getx(string(key))));}
#endif

  Collection _unique(const string& key,const string& key2=INVALIDSTR,const string& key3=INVALIDSTR,const string& key4=INVALIDSTR) { return(_unique(getx(key),getx(key2),getx(key3),getx(key4))); }
  Collection _unique(int x,int x2=-100,int x3=-100,int x4=-100);
#ifdef CHARIF
  Collection _unique(const char* key,const char* key2,const char* key3,const char* key4) 
    { return(_unique(getx(string(key)),getx(string(key2)),getx(string(key3)),getx(string(key4))));}
  Collection _unique(const char* key,const char* key2,const char* key3) 
    { return(_unique(getx(string(key)),getx(string(key2)),getx(string(key3))));}
  Collection _unique(const char* key,const char* key2) 
    { return(_unique(getx(string(key)),getx(string(key2))));}
  Collection _unique(const char* key) { return(_unique(getx(string(key))));}
#endif

  Collection unique_count(const string& key,const string& key2=INVALIDSTR,const string& key3=INVALIDSTR,const string& key4=INVALIDSTR)
  { return(unique_count(getx(key),getx(key2),getx(key3),getx(key4))); }
  virtual Collection unique_count(int x,int x2=-100,int x3=-100,int x4=-100);
  Collection unique_count(const vector<int> x) {
    switch(x.size()) {
    // case 0:  TO BE CONSIDERED
    case 1: return(unique_count(x[0])); 
    case 2: return(unique_count(x[0],x[1])); 
    case 3: return(unique_count(x[0],x[1],x[2])); 
    case 4: return(unique_count(x[0],x[1],x[2],x[3])); 
    default: return(unique_count(x[0],x[1],x[2],x[3])); 
    }
  }
#ifdef CHARIF
  Collection unique_count(const char* key,const char* key2,const char* key3,const char* key4) 
    { return(unique_count(getx(string(key)),getx(string(key2)),getx(string(key3)),getx(string(key3))));}
  Collection unique_count(const char* key,const char* key2,const char* key3) 
    { return(unique_count(getx(string(key)),getx(string(key2)),getx(string(key3))));}
  Collection unique_count(const char* key,const char* key2) 
    { return(unique_count(getx(string(key)),getx(string(key2))));}
  Collection unique_count(const char* key) 
    { return(unique_count(getx(string(key)))); }
#endif

  ///////////////////////////////////////////////////////////////////
  map<string,Collection> unique_map(const string& key,const string& key2=INVALIDSTR,const string& key3=INVALIDSTR,const string& key4=INVALIDSTR) 
    { return(unique_map(getx(key),getx(key2),getx(key3),getx(key4))); }
  virtual map<string,Collection> unique_map(int x,int x2=-100,int x3=-100,int x4=-100);
#ifdef CHARIF
  map<string,Collection> unique_map(const char* key,const char* key2,const char* key3,const char* key4) 
    { return(unique_map(getx(string(key)),getx(string(key2)),getx(string(key3)),getx(string(key4))));}
  map<string,Collection> unique_map(const char* key,const char* key2,const char* key3) 
    { return(unique_map(getx(string(key)),getx(string(key2)),getx(string(key3))));}
  map<string,Collection> unique_map(const char* key,const char* key2) 
    { return(unique_map(getx(string(key)),getx(string(key2))));}
  map<string,Collection> unique_map(const char* key) 
    { return(unique_map(getx(string(key)))); }
#endif

  ///////////////////////////////////////////////////////////////////
  virtual Collection unique_substr_count(int key,const string& separator,int i1,int i2=-100,int i3=-100,int i4=-100);
  Collection unique_substr_count(const string& key,const string& separator,int i1,int i2=-100,int i3=-100,int i4=-100) { return(unique_substr_count(getx(string(key)),string(separator),i1,i2,i3,i4)); }
#ifdef CHARIF
  Collection unique_substr_count(const char* key,const char* separator,int i1,int i2=-100,int i3=-100,int i4=-100) { return(unique_substr_count(string(key),string(separator),i1,i2,i3,i4)); }
  Collection unique_substr_count(int key,const char* separator,int i1,int i2=-100,int i3=-100,int i4=-100) { return(unique_substr_count(key,string(separator),i1,i2,i3,i4));}
#endif

  Collection unique_key_substr_count(const string& separator,int i1,int i2=-100,int i3=-100,int i4=-100);
#ifdef CHARIF
  Collection unique_key_substr_count(const char* separator,int i1,int i2=-100,int i3=-100,int i4=-100) { return(unique_key_substr_count(string(separator),i1,i2,i3,i4)); }
#endif

  ///////////////////////////////////////////////////////////////////
  virtual Collection unique_stat(map<string,statistics>& mexist,int xval,int x,int x2=-1,int x3=-1,int x4=-1) ;
  virtual Collection unique_stat(int xval,int x,int x2=-1,int x3=-1,int x4=-1) {
    map<string,statistics> mexist;
    return(unique_stat(mexist,xval,x,x2,x3,x4)); 
  }
  Collection unique_stat(const string& valkey,const string& key,const string& key2=INVALIDSTR,const string& key3=INVALIDSTR,const string& key4=INVALIDSTR) 
    { return(unique_stat(getx(valkey),getx(key),getx(key2),getx(key3),getx(key4))); }
#ifdef CHARIF
  Collection unique_stat(const char* valkey,const char* key,const char* key2,const char* key3,const char* key4) 
    { return(unique_stat(getx(string(valkey)),getx(string(key)),getx(string(key2)),getx(string(key3)),getx(string(key4))));}
  Collection unique_stat(const char* valkey,const char* key,const char* key2,const char* key3) 
    { return(unique_stat(getx(string(valkey)),getx(string(key)),getx(string(key2)),getx(string(key3))));}
  Collection unique_stat(const char* valkey,const char* key,const char* key2) 
    { return(unique_stat(getx(string(valkey)),getx(string(key)),getx(string(key2))));}
  Collection unique_stat(const char* valkey,const char* key) 
    { return(unique_stat(getx(string(valkey)),getx(string(key)))); }
#endif

  ///////////////////////////////////////////////////////////////////
  virtual Collection unique_list(int xstat,int x,int x2=-1,int x3=-1,int x4=-1);
  Collection unique_list(const string& valkey,const string& key,const string& key2=INVALIDSTR,const string& key3=INVALIDSTR,const string& key4=INVALIDSTR) 
    { return(unique_list(getx(valkey),getx(key),getx(key2),getx(key3),getx(key4))); }
#ifdef CHARIF
  Collection unique_list(const char* valkey,const char* key,const char* key2,const char* key3,const char* key4) 
    { return(unique_list(getx(string(valkey)),getx(string(key)),getx(string(key2)),getx(string(key3)),getx(string(key4))));}
  Collection unique_list(const char* valkey,const char* key,const char* key2,const char* key3) 
    { return(unique_list(getx(string(valkey)),getx(string(key)),getx(string(key2)),getx(string(key3))));}
  Collection unique_list(const char* valkey,const char* key,const char* key2) 
    { return(unique_list(getx(string(valkey)),getx(string(key)),getx(string(key2))));}
  Collection unique_list(const char* valkey,const char* key) 
    { return(unique_list(getx(string(valkey)),getx(string(key)))); }
#endif

  ///////////////////////////////////////////////////////////////////
#ifdef UNIQUE_HIST
  virtual Collection unique_hist(map<string,statistics>& mstat
				 ,map<string,Histogram<double> >& mexist
				 ,double minstddev,double maxstddev,int nbin
				 ,int xval,int x,int x2=-1,int x3=-1,int x4=-1);
  virtual Collection unique_hist(double minstddev,double maxstddev,int nbin,
				 int xval,int x,int x2=-1,int x3=-1,int x4=-1) ;
#endif

  ///////////////////////////////////////////////////////////////////
  virtual Collection unique_sampling(const Collection& u,const char* mode
				     ,int xval,int x,int x2=-1,int x3=-1,int x4=-1);
  virtual Collection unique_sampling(const char* mode,int xval
				     ,int x,int x2=-1,int x3=-1,int x4=-1) {
    Collection u = unique_count(x,x2,x3,x4);
    return(unique_sampling(u,mode,xval,x,x2,x3,x4));
  }
  virtual Collection unique_sampling(const char* mode,const char* xval
				     ,const char* x
				     ,const char* x2=""
				     ,const char* x3="") 
    {return(unique_sampling(mode,getx(xval)
			   ,getx(x),x2[0]?getx(x2):-1,x3[0]?getx(x3):-1));}
  virtual Collection unique_sampling(const string& mode,const string& xval
				     ,const string& x
				     ,const string& x2=""
				     ,const string& x3="")
    {return(unique_sampling(mode.c_str(),xval.c_str()
		   ,x.c_str(),x2.c_str(),x3.c_str()));}

  ///////////////////////////////////////////////////////////////////
  virtual Collection align_normalize(Collection& ctemplate_stat
				     ,const char* mode
				     ,double normgain, double normmean
				     ,int xval
				     ,int x,int x2=-1,int x3=-1,int x4=-1);
  virtual Collection align_normalize(Collection& ctemplate_stat
				     ,const char* mode
				     ,double normgain, double normmean
				     ,const char* xval
				     ,const char* x,const char* x2=""
				     ,const char* x3="",const char* x4="")
    { return(align_normalize(ctemplate_stat
			     ,mode
			     ,normgain,normmean
			     ,getx(xval),getx(x)
			     ,x2[0]?getx(x2):-1
			     ,x3[0]?getx(x3):-1
			     ,x4[0]?getx(x4):-1)); }
  virtual Collection align_normalize(Collection& ctemplate_stat
				     ,const string& mode
				     ,double normgain, double normmean
				     ,const string& xval
				     ,const string& x
				     ,const string& x2=""
				     ,const string& x3=""
				     ,const string& x4="")
    { return(align_normalize(ctemplate_stat
			     ,mode.c_str()
			     ,normgain, normmean
			     ,xval.c_str()
			     ,x.c_str()
			     ,x2.c_str()
			     ,x3.c_str()
			     ,x4.c_str())); }

  ///////////////////////////////////////////////////////////////////
  virtual Collection featurespace(const char* mode 
				,double inmin,double inmax,double instep
				,int offsetmode
			        ,double outlow, double outhigh,double nullval
				,int xval) ;
  virtual Collection featurespace(const char* mode 
				,double inmin,double inmax,double instep
				,int offsetmode
			        ,double outlow, double outhigh,double nullval
				,const char* xval) 
  { return(featurespace(mode,inmin,inmax,instep,offsetmode,outlow,outhigh,nullval,getx(xval)));}
  virtual Collection featurespace(const string& mode 
				,double inmin,double inmax,double instep
				,int offsetmode
			        ,double outlow, double outhigh,double nullval
				,const string& xval) 
  { return(featurespace(mode.c_str(),inmin,inmax,instep,offsetmode,outlow,outhigh,nullval,xval.c_str()));}

  ///////////////////////////////////////////////////////////////////

  // clustering
  int m_clusterlimit;
  void set_clusterlimit(int clusterlimit) { m_clusterlimit = clusterlimit; }
  Collection cluster(int x,statistics& stat,const string& separator=" \t-_.,:;/*+=");
  Collection cluster_substr(int x,int y,const string& separator=" \t-_.,:;/*+=");

  // Line operation
  Collection& merge(const Collection& a);
  Collection& unique_merge(const Collection& a);
  Collection& operator+=(const Collection& a) { return(unique_merge(a)); }
  Collection& operator-=(const Collection& a);
  Collection& operator&=(const Collection& a);

  // Column extraction
  virtual Collection  column(int from,int to= -1); // line persistent
  virtual Collection _column(int from,int to= -1); // line persistent
  Collection  column(const string& from,const string& to=""); //line persistent
  Collection _column(const string& from,const string& to=""); //line persistent
#ifdef CHARIF
  Collection  column(const char* from,const char* to="") 
    {return(column(string(from),string(to)));} 
  Collection _column(const char* from,const char* to="") 
    {return(_column(string(from),string(to)));} 
#endif

  Collection  columns(const string& keyexpr); // line persistent
  Collection _columns(const string& keyexpr); // line persistent

  virtual Collection  column(const vector<int>& p);
  virtual Collection _column(const vector<int>& p);

  // Column operation
  Collection& column_push_front(double d); // non line persistent
  Collection& column_push_back(double d); // non line persistent
  Collection& column_push_front(const string& s); // non line persistent
  Collection& column_push_back(const string& s); // non line persistent
  Collection& column_push_front(const value& val); // non line persistent
  Collection& column_push_back(const value& val); // non line persistent

  Collection& column_push_front(const string& key,double d); // non line persistent
  Collection& column_push_back(const string& key,double d); // non line persistent
  Collection& column_push_front(const string& key,const string& s); // non line persistent
  Collection& column_push_back(const string& key,const string& s); // non line persistent
  Collection& column_push_front(const string& key,const value& val); // non line persistent
  Collection& column_push_back(const string& key,const value& val); // non line persistent
#ifdef CHARIF
  Collection& column_push_front(const char* s) { return(column_push_front(string(s))); }
  Collection& column_push_back(const char* s) { return(column_push_back(string(s))); }
  Collection& column_push_front(const char* key,double d) {return(column_push_front(string(key),d));}
  Collection& column_push_back(const char* key,double d) {return(column_push_back(string(key),d));}
  Collection& column_push_front(const char* key,const char* s){return(column_push_front(string(key),string(s)));} 
  Collection& column_push_back(const char* key,const char* s){return(column_push_back(string(key),string(s)));} 
  Collection& column_push_front(const char* key,const value& val){return(column_push_front(string(key),val));}
  Collection& column_push_back(const char* key,const value& val){return(column_push_back(string(key),val));} 
#endif
  Collection& column_merge_front(const Collection& a); // non line persistent
  Collection& column_merge_back(const Collection& a); // non line persistent
  Collection& column_push_front(const Collection& a) 
    { return(column_merge_front(a)); }
  Collection& column_push_back(const Collection& a)
    { return(column_merge_back(a)); }

  Collection& column_push_front(const Line& key,const Line& val); // non line persistent
  Collection& column_push_back(const Line& key,const Line& val);  // non line persistent
  Collection& column_push_front(const Line& val) { return(column_push_front(Line(),val)); }
  Collection& column_push_back(const Line& val) { return(column_push_back(Line(),val)); }

  int column_erase(int x); // non line persistent
  int column_erase(int x1,int x2); // non line persistent
  int column_erase(const string& key) ; // non line persistent
  int column_erase(const string& key1,const string& key2) ; // non line persistent
#ifdef CHARIF
  int column_erase(const char* key1,const char* key2)
    {return(column_erase(string(key1),string(key2)));}
#endif

  int column_insert(const string& after,const string& key,const string& val) 
   {int x = getx(after); return(column_insert(x,key,val));}
  int column_insert(const string& after,const string& key,double val)
   {int x = getx(after); return(column_insert(x,key,val));}
  int column_insert(const string& after,const string& val) 
   {int x = getx(after); return(column_insert(x,val));}
  int column_insert(const string& after,double val)
   {int x = getx(after); return(column_insert(x,val));}
  int column_insert(const string& after) 
   {int x = getx(after); return(column_insert(x));}
  int column_insert(int after,const string& key,const string& val) ;
  int column_insert(int after,const string& key,double val) ;
  int column_insert(int after,const string& val);
  int column_insert(int after,double val);
  int column_insert(int after);
#ifdef CHARIF
  int column_insert(const char* after,const char* key,const char* val)
    {return(column_insert(string(after),string(key),string(val))); }
  int column_insert(const char* after,const char* key,double val)
    {return(column_insert(string(after),string(key),val)); }
  int column_insert(const char* after,const char* val) 
    {return(column_insert(string(after),string(val))); }
  int column_insert(const char* after,double val) 
    {return(column_insert(string(after),val)); }
  int column_insert(const char* after) 
    {return(column_insert(string(after))); }
  int column_insert(int after,const char* key,const char* val) 
    { return(column_insert(after,string(key),string(val))); }
  int column_insert(int after,const char* key,double val) 
    { return(column_insert(after,string(key),val)); }
  int column_insert(int after,const char* val)
    { return(column_insert(after,string(val))); }
#endif

  // string
  int maxstrlen(int x);
  int maxstrlen(const string& key) { return(maxstrlen(getx(key))); }
  int maxstrlen(const char* key) { return(maxstrlen(string(key))); };

  // Numeric operaton
  Collection& add(const Collection& a);
  Collection& sub(const Collection& a);
  Collection& mul(const Collection& a);
  Collection& div(const Collection& a);
  Collection& mod(const Collection& a);

  Collection& add(int x,double a);
  Collection& sub(int x,double a);
  Collection& mul(int x,double a);
  Collection& div(int x,double a);
  Collection& mod(int x,double a);

  Collection& add(const string& key,double a) ;
  Collection& sub(const string& key,double a) ;
  Collection& mul(const string& key,double a) ;
  Collection& div(const string& key,double a) ;
  Collection& mod(const string& key,double a) ;

  Collection& add(const char* key,double a) {return(add(string(key),a));}
  Collection& sub(const char* key,double a) {return(sub(string(key),a));}
  Collection& mul(const char* key,double a) {return(mul(string(key),a));}
  Collection& div(const char* key,double a) {return(div(string(key),a));}
  Collection& mod(const char* key,double a) {return(mod(string(key),a));}

  Collection& add(int x,int x2,int xo);
  Collection& sub(int x,int x2,int xo);
  Collection& mul(int x,int x2,int xo);
  Collection& div(int x,int x2,int xo);
  Collection& mod(int x,int x2,int xo);

  Collection& add(const string& x,const string& x2,const string& xo);
  Collection& sub(const string& x,const string& x2,const string& xo);
  Collection& mul(const string& x,const string& x2,const string& xo);
  Collection& div(const string& x,const string& x2,const string& xo);
  Collection& mod(const string& x,const string& x2,const string& xo);

  Collection& add(const char* x,const char* x2,const char* xo)
    { return(add(string(x),string(x2),string(xo))); }
  Collection& sub(const char* x,const char* x2,const char* xo)
    { return(sub(string(x),string(x2),string(xo))); }
  Collection& mul(const char* x,const char* x2,const char* xo)
    { return(mul(string(x),string(x2),string(xo))); }
  Collection& div(const char* x,const char* x2,const char* xo)
    { return(div(string(x),string(x2),string(xo))); }
  Collection& mod(const char* x,const char* x2,const char* xo)
    { return(mod(string(x),string(x2),string(xo))); }

  Collection& limit(int x,double lower,double upper,double lim=1);
  Collection& limit(const string& key,double lower,double upper,double lim=1);
  Collection& limit(const char* key,double lower,double upper,double lim=1.0)
    {return(limit(string(key),lower,upper,lim));}
#ifdef MULTIPLEKEYS
  Collection& limit(const vector<int>& xs,double lower,double upper,double lim=1);
#endif

  // hyperlink function
  Collection& hyperlink(const string& key);
  Collection& hyperlink(int x);
#ifdef CHARIF
  Collection& hyperlink(const char* key) { return(hyperlink(string(key))); }
#endif

  // string replacement
  Collection& replace(const string& key,const string& match,const string& rep);
  Collection& replace(int x,const string& match,const string& rep);
#ifdef MULTIPLEKEYS
  Collection& replace(const vector<int>& xs,const string& match,const string& rep);
#endif

  // Set value 
  Collection& let(int x,const string& val);
  Collection& let(int x,double val);
  Collection& let(const string& key,const string& val);
  Collection& let(const string& key,double val);
  Collection& let(int x,const value& val);
#ifdef MULTIPLEKEYS
  Collection& let(const vector<int>& xs,const string& val);
  Collection& let(const vector<int>& xs,double val);
#endif

  // column to vector 
  vector<double> VectorDouble(int x) {
    vector<double> r; r.resize(size());
    for(int i=0;i<size();i++) {
      if(get(i)[x].isdouble()) r[i] = get(i)[x].todouble();
      else break;
    }
    return(r);
  }
  vector<string> VectorString(int x) {
    vector<string> r; r.resize(size());
    for(int i=0;i<size();i++) r[i] = get(i)[x].tostring();
    return(r);
  }

  vector<double> VectorDouble() {
    vector<double> r; 
    int i,j;
    for(i=0;i<size();i++) {
      for(j=0;j<get(i).size();j++) {
        if(get(i)[j].isdouble()) r.push_back(get(i)[j].todouble());
      }
    }
    return(r);
  }
  vector<string> VectorString() {
    vector<string> r; 
    int i,j;
    for(i=0;i<size();i++) {
      for(j=0;j<get(i).size();j++) {
        if(get(i)[j].isdouble()) r.push_back(get(i)[j].tostring());
      }
    }
    return(r);
  }

  // for each item in a column
  EachValue eachvalue(int key) { 
    Collection t = unique_count(key);
    EachValue fe;
    for(int i=0;i<t.size();i++) fe.push_back(t[i][0]);
    return(fe);
  }
  EachValue eachvalue(const string key) { return(eachvalue(getx(key))); } 
#ifdef CHARIF
  EachValue eachvalue(const char* key) { return(eachvalue(getx(string(key)))); } 
#endif

  // Iterator concept 2
//inline bool Collection::rdstate() {return(size()?true:false);}
  bool rdstate() {return(size()?false:true);}

  CollectionIterator begin(int x1=-1,int x2=-1,int x3=-1,int x4=-1) ;
  CollectionIterator begin(const vector<int>& xs);
  CollectionIterator begin(const string& key1,const string& key2=INVALIDSTR,const string& key3=INVALIDSTR,const string& key4=INVALIDSTR);
  CollectionIterator begin(const vector<string>& keys);

  CollectionIterator end(const CollectionIterator& first) ;
  CollectionIterator end(int x1=-1,int x2=-1,int x3=-1,int x4=-1) ;
  CollectionIterator end(const vector<int>& xs);
  CollectionIterator end(const string& key1,const string& key2=INVALIDSTR,const string& key3=INVALIDSTR,const string& key4=INVALIDSTR);
  CollectionIterator end(const vector<string>& keys);


  // Sorting
  Collection& sort(int x,CompareLine::mode_t mode=CompareLine::Auto) ;
  Collection& sort(const string& key,CompareLine::mode_t mode=CompareLine::Auto) ;
  Collection& sortstring(int x) {sort(x,CompareLine::String); return(*this);}
  Collection& sortstring(const string& key) {sort(key,CompareLine::String); return(*this);}
  Collection& sortnumber(int x) {sort(x,CompareLine::Number); return(*this);}
  Collection& sortnumber(const string& key) {sort(key,CompareLine::Number); return(*this);}

  Collection& sort() {  
    CompareLine comp(-1,CompareLine::LineNum);
    std::sort(m_plines.begin(),m_plines.end(),comp);
    return(*this);
  }

  // xfix columnÇå≈íËÇµÇΩîÕàÕÇ≈ x columnÇ≈sort
  Collection& fixsort(int x,int xfix);
  Collection& fixsort(const string& key,const string& fixkey);
  Collection& fixsort(const char* key,const char* fixkey)
    {return(fixsort(string(key),string(fixkey)));}
#ifndef MULTISORT_FIX2
  Collection& fixsort(int x,int xfix,int xfix2,int xfix3); // added 2015/6/16
#endif
  Collection& fixsort(int x,int xfix,int xfix2); // added 2015/4/15
  Collection& fixsort(const string& key,const string& fixkey,const string& fixkey2);
  Collection& fixsort(const char* key,const char* fixkey,const char* fixkey2)
    {return(fixsort(string(key),string(fixkey),string(fixkey2)));}

  // ï°êîÇÃcolumnÇÃì‡óeÇäKëwìIÇ… sort,  only good up to key0-2.  key3,4 result is not guaranteed
#define MULTISORTVECTOR
#ifdef MULTISORTVECTOR
  Collection& multisort(const vector<int> xs,CompareLine::mode_t mode=CompareLine::Auto);
#else
  Collection& multisort(int xs[]);
#endif
  Collection& multisort(int x0,int x1 = -1,int x2= -1,int x3= -1,int x4= -1);
  Collection& multisort(const string* key);
  Collection& multisort(const string& key0,const string& key1=INVALIDSTR,const string& key2=INVALIDSTR
	    ,const string& key3=INVALIDSTR,const string& key4=INVALIDSTR);
  Collection& multisort(char* key[]);
  Collection& multisort(const char* key0,const char* key1=INVALIDSTR,const char* key2=INVALIDSTR,const char* key3=INVALIDSTR,const char* key4=INVALIDSTR)
    { return(Collection::multisort(string(key0),string(key1),string(key2)
				   ,string(key3) ,string(key4))); }

  Collection allignkey(const string& key,int extra=0){int x=getx(key);return(allignkey(x,extra));}
  Collection allignkey(const char* key,int extra=0) { return(allignkey(string(key),extra)); }
  Collection allignkey(int x=0,int extra=0);

  // reverse
  Collection& reverse() {std::reverse(m_plines.begin(),m_plines.end()); return(*this);}

  // rotate
  Collection rotate(int key=0) /* const */ ;

  // Event <=> Table
  Collection Event2Table();
  Collection Table2Event();

  // statistics
  void initstat() {m_stat.init();}
  CollectionStatistics& stat();
  CollectionStatistics& stat(const_iterator first,const_iterator last) ;
  CollectionStatistics& getstat() { return(m_stat); }

  CollectionStatistics linestat(int xfrom=0,int xto=-1) ;

  // sort + statistics 
  // sort with key and calculate statistics of the key item.
  // Returned content is 'sortkey,sigma,max,min,mean,nsample,stddev'
  Collection sortstat(int xsort,int xstat);
  Collection sortstat(const string& sortkey,const string& statkey);

  // Line number inquiry
  int gety(Line* p);
  int getx(const string& key,bool icase=false);
  int getxexact(const string& key);
  vector<int> getxs(const string& key,bool icase=false);
  int getxs(vector<int>& xs,const string& key,bool icase=false);
  int getkeyy(int x,const string& key);
  //int find(const string& str);
  int findy(int x,const string& str,int n=1);
  int findy(const string& key,const string& str,int n=1);

  // Title key setting
  void setkey(int keyy=0) ; 
  void setkey(int x,const string& key) { setkey(getkeyy(x,key));}
  void setkey(const Collection& c) { p_key= c.p_key; } 
  void setkey(const Line& l) { push_back(l); setkey(size()-1); /* p_key=&back(); */ }
  void setkey(const char* a1 ,const char* a2=0 ,const char* a3=0
	   ,const char* a4=0 ,const char* a5=0 ,const char* a6=0
	   ,const char* a7=0 ,const char* a8=0 ,const char* a9=0
	   ,const char* a10=0 ,const char* a11=0 ,const char* a12=0
	   ,const char* a13=0 ,const char* a14=0 ,const char* a15=0
	   ,const char* a16=0 ,const char* a17=0 ,const char* a18=0
	   ,const char* a19=0 ,const char* a20=0 ,const char* a21=0) {
    push_back(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,a17,a18,a19,a20,a21);
    setkey((int)(size()-1));
  }
  Line& getkey() const {
    static Line dmyline;
    if(p_key) return(*p_key);
    else return(dmyline);
  }

  // Title key2 setting
  void setkey2(int keyy=0) ;
  void setkey2(int x,const string& key) { setkey2(getkeyy(x,key));}
  Line& getkey2() { return(*p_key2); }

  int getx2(const string& key,int xoffset,bool icase=false);
  int getx2(const char* key,int xoffset,bool icase=false) 
    { return(getx2(string(key),xoffset,icase)); }

  int getx(const string& key,const string& key2,bool icase=false) 
   { return(getx2(key2,getx(key,icase),icase)); }
  int getx(const char* key,const char* key2,bool icase=false) 
   { return(getx(string(key),string(key2),icase)); }


  // Output
  void print(const string& fname,const string& mode="w",int statmode=10) ;
  void dispkey(FILE* fp=stdout) const {if(p_key) p_key->disp(fp);}
  void disp(FILE* fp=stdout,int statmode=10) const ;
  void dispform(FILE* fp=stdout,int more=0,int keylength=0) const ;
  void _disp(FILE* fp=stdout,int statmode=10) const ;
  void disp2(FILE* fp=stdout,int flag=0) const ;
  void disp3(FILE* fp=stdout,int flag=0) const ;
  void disp(const char* fname,const char* mode,int statmode=10) const ;
  void disp2(const char* fname,const char* mode,int flag=0) const ;
  void disp3(const char* fname,const char* mode,int flag=0) const ;
  void dispform(const char* fname,const char* mode,int flag=0) const ;

  void more(int s=25) const;

#ifdef CHARIF


  Collection& replace(const char* key,const char* match,const char* rep)
   { return(replace(string(key),string(match),string(rep)));}
  Collection& replace(int x,const char* match,const char* rep) 
   { return(replace(x,string(match),string(rep)));}

  Collection& let(int x,const char* val) {return(let(x,string(val)));}
  Collection& let(const char* key,const char* val){return(let(string(key),string(val)));}
  Collection& let(const char* key,double val){return(let(string(key),val));}

  void sort(const char* key,CompareLine::mode_t mode=CompareLine::Auto){sort(string(key),mode);}
  void sortstring(const char* key) {sort(string(key),CompareLine::String);}
  void sortnumber(const char* key) {sort(string(key),CompareLine::Number);}

  Collection sortstat(const char* sortkey,const char* statkey){return(sortstat(string(sortkey),string(statkey)));}

  int getx(const char* key,bool icase=false){return(getx(string(key),icase));}
  int getxexact(const char* key){return(getxexact(string(key)));}
  int getkeyy(int x,const char* key){return(getkeyy(x,string(key)));}
  int findy(int x,const char* str,int n=1){return(findy(x,string(str),n));}
  int findy(const char* key,const char* str,int n=1){return(findy(string(key),string(str),n));}

#ifdef MULTIPLEKEYS
  vector<int> findys(int x,const string& str);
  vector<int> findys(const vector<int>& xs,const string& str);
  vector<int> findys(const string& key,const string& str);
  vector<int> findys(const vector<int>& xs,const char* str){return(findys(xs,string(str)));}
  vector<int> findys(const char* key,const char* str){return(findys(string(key),string(str)));}
#endif

  statistics stat(int xfrom  ,int yfrom=0,int xto= -1,int yto=-1) {
    statistics s;
    if(yto<0 || yto>size()-1) yto=size()-1;
    int to;
    for(int i=yfrom;i<=yto;i++) {
      Line& line = operator[](i);
      if(xto<0 || xto>line.size()-1) to = line.size()-1;
      else                           to = xto;
      for(int j=xfrom;j<=xto;j++) s.add(line[j].todouble());
    }
    return(s);
  }
  double sigma(int xfrom=0,int yfrom=0,int xto= -1,int yto=-1) {
    return(stat(xfrom,yfrom,xto,yto).sigma());
  }
  int nsample(int xfrom=0,int yfrom=0,int xto= -1,int yto=-1) {
    return(stat(xfrom,yfrom,xto,yto).nsample());
  }
  double min(int xfrom=0,int yfrom=0,int xto= -1,int yto=-1) {
    return(stat(xfrom,yfrom,xto,yto).min());
  }
  double max(int xfrom=0,int yfrom=0,int xto= -1,int yto=-1) {
    return(stat(xfrom,yfrom,xto,yto).max());
  }
  double mean(int xfrom=0,int yfrom=0,int xto= -1,int yto=-1) {
    return(stat(xfrom,yfrom,xto,yto).mean());
  }
  double stddev(int xfrom=0,int yfrom=0,int xto= -1,int yto=-1) {
    return(stat(xfrom,yfrom,xto,yto).stddev());
  }

  void setkey(int x,const char* key) { setkey(getkeyy(x,string(key)));}

  void print(const char* fname,const char* mode="w",int statmode=10) {print(string(fname),string(mode),statmode);}


  void setmatchmode(EasyExp::Mode mode) { m_EasyExpMode = mode; }
  void setlinenum() ;
  int  linenum(int i) { if(0<=i && (unsigned int)i<m_plines.size()) return(m_plines[i]->linenum()); else return(-1); }

#endif

#ifdef COLLECTIONCOMPARE
  friend bool operator==(const Collection& a,const Collection& b) ;
  friend bool operator!=(const Collection& a,const Collection& b) ;
  friend bool operator<(const Collection& a,const Collection& b) ;
#endif
#ifdef MATHOPERATOR
  friend Collection operator+(const Collection& a,const Collection& b) ;
  friend Collection operator-(const Collection& a,const Collection& b) ;
  friend Collection operator*(const Collection& a,const Collection& b) ;
  friend Collection operator/(const Collection& a,const Collection& b) ;
#endif

 //private:
#ifdef MULTIKEY
  deque<Line*> m_keys;
#else
  Line*        p_key;
  Line*        p_key2;
#endif
  //map<string,int> m_keyindex;
  deque<Line*> m_plines;

  Line         m_dummy;

  //deque<statistics> m_stat;
  CollectionStatistics m_stat;

  Sheet*       p_sheet;
  int          m_isauto;

  EasyExp::Mode m_EasyExpMode;
};

////////////////////////////////////////////////////////////////////////
#ifdef COLLECTIONCOMPARE
bool operator==(const Collection& a,const Collection& b) ;
inline bool operator!=(const Collection& a,const Collection& b){return(!operator==(a,b));}
bool operator<(const Collection& a,const Collection& b) ;
#else
inline bool operator==(const Collection& a,const Collection& b) 
  { return(&a==&b); }
inline bool operator!=(const Collection& a,const Collection& b) 
  { return(&a!=&b); }
inline bool operator<(const Collection& a,const Collection& b) 
  { return(&a<&b); }
#endif

#ifdef MATHOPERATOR
Collection operator+(const Collection& a,const Collection& b) ;
Collection operator-(const Collection& a,const Collection& b) ;
Collection operator*(const Collection& a,const Collection& b) ;
Collection operator/(const Collection& a,const Collection& b) ;
#endif
////////////////////////////////////////////////////////////////////////
int alignCollections(Collection& ain,Collection& bin
		     ,Collection& aout,Collection& bout
		     ,int xval,const vector<int> xalign
		     ,const char* mode="ave");

////////////////////////////////////////////////////////////////////////
inline int G__ateval(const Collection& a) {a.disp(); return(1);}

/////////////////////////////////////////////////////////////////////////////
class Sheet {
 public:
  Sheet() { init(); }
  Sheet(const string& fname,int head=0) { init(); Read(fname,head); }
  Sheet(const Sheet& s) ;
  void init() { m_lines.clear(); m_keepquotation=false; }
  void clear() { m_lines.clear(); }
  int Read(const string& fname,int head=0);
  int IncrementalRead(int head);
  Collection collection();
  Collection collection(int keyy) { 
    Collection c=collection();
    if(keyy>=0) {
      c.setkey(keyy); 
    }
    else {
      while(c.size() && c[0][0].tostring()=="") c.erase(0);
      c.setkey(0);
    }
    return(c);
  }
  Collection collection(int x,const string& key) {
    Collection c=collection();
    c.setkey(x,key);
    return(c);
  }
  void push_front(const Line& line) { m_lines.push_front(line); }
  void push_back(const Line& line) { m_lines.push_back(line); }
  void pop_front() { m_lines.pop_front(); }
  void pop_back() { m_lines.pop_back(); }
  Line& front() { return(m_lines.front()); }
  Line& back() { return(m_lines.back()); }
  //void erase(int x); // this operation is bad because m_lines keeps master info
  //void erase(int x1,int x2);
  int isopen() const { return(f.fp?1:0); }
  int isvalid() { return(f.isvalid()); }
  int close() { return(f.close()); }
  int size() const { return(m_lines.size()); }
  const string& filename() const { return(m_fname); }
  void disp(FILE* fp=stdout) const 
    {fprintf(fp,"%s %d\n",filename().c_str(),size());}

#ifdef CHARIF
  Sheet(const char* fname) { init(); Read(string(fname)); }
  int Read(const char*  fname) {return(Read(string(fname)));}
#endif

  const string& getfname() const { return(m_fname); }
  ReadF& getReadF() { return(f); }
  const Line& getline(int n) const { return(m_lines[n]); }

  void setseparator(const char *separatorin) {m_separator=separatorin;} 
  void setdelimiter(const char *delimiterin) {m_delimiter=delimiterin;}
  void setendofline(const char *endoflinein) {m_endofline=endoflinein;} 
  void setquotation(const char *quotationin) {m_quotation=quotationin;} 
  void setkeepquotation(bool flag) { m_keepquotation = flag; }
  void SetCodingSystem() {f.SetCodingSystem();}
  void SetCodingSystem(int codingsystem) {f.SetCodingSystem(codingsystem);}
  int GetCodingSystem() {return(f.GetCodingSystem());}
  string m_separator;
  string m_delimiter;
  string m_endofline;
  string m_quotation;
  bool   m_keepquotation;


 //private:
  string m_fname;
  deque<Line> m_lines;

  ReadF  f;
};

////////////////////////////////////////////////////////////////////////
inline bool operator==(const Sheet& a,const Sheet& b) 
  { return(a.m_fname==b.m_fname); }
inline bool operator!=(const Sheet& a,const Sheet& b) 
  { return(a.m_fname!=b.m_fname); }
inline bool operator<(const Sheet& a,const Sheet& b) 
  { return(a.m_fname<b.m_fname); }

////////////////////////////////////////////////////////////////////////
inline int G__ateval(const Sheet& a) {a.disp(); return(1);}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// Iterator concept 2
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

class CollectionIterator {
  //public:
  Collection m_u;
  Collection m_c;
  int m_index;
  vector<int> m_xkey;
 public:
  //////////////////////////////////////////////////////////
  CollectionIterator() {m_index=-1; }
  CollectionIterator(const CollectionIterator& iter){
    m_u=iter.m_u; m_c=iter.m_c; m_index=iter.m_index; m_xkey=iter.m_xkey;
  }
  CollectionIterator(const CollectionIterator& iter,int endflag){
    if(iter.m_xkey.size()) m_index = iter.m_u.size();
    else                   m_index = iter.m_c.size();
  }
  void init(const Collection& cin1,const vector<int>& xkeyin) {
    m_c = cin1;
    m_xkey=xkeyin; 
    if(m_xkey.size()) m_u = m_c.unique_count(m_xkey);
    else            m_u.resize(m_c.size());
  }
  CollectionIterator(const Collection& cin1,const vector<int>& xkeyin) {
    init(cin1,xkeyin);
    m_index = 0;
  }
  CollectionIterator(const Collection& cin1,const vector<int>& xkeyin
		     ,int endflag) {
    init(cin1,xkeyin);
    m_index = m_u.size();
    m_u.clear(); m_c.clear();
  }
  //////////////////////////////////////////////////////////
  string id(const string& separator="#") {
    string result;
    if(m_xkey.size()==0) {
      char buf[20]; sprintf(buf,"%d",m_index); result = buf;
    }
    if(m_xkey.size()>=1) result = m_u[m_index][0].tostring();
    if(m_xkey.size()>=2) result += separator +  m_u[m_index][1].tostring();
    if(m_xkey.size()>=3) result += separator +  m_u[m_index][2].tostring();
    if(m_xkey.size()>=4) result += separator +  m_u[m_index][4].tostring();
    if(m_xkey.size()>=5) {
      fprintf(stderr,"Error: CollectionIterator::id() Too many xindex\n");
    }
    return(result);
  }
  int count() {
    switch(m_xkey.size()) {
    case 0: return(1);
    case 1: return(m_u[m_index][1].toint());
    case 2: return(m_u[m_index][2].toint());
    case 3: return(m_u[m_index][3].toint());
    case 4: return(m_u[m_index][4].toint());
    default: 
      fprintf(stderr,"Error: CollectionIterator::count() Too many xindex\n");
      return(0);
    }
  }
  //////////////////////////////////////////////////////////
  Collection get() {
    Collection r;
    if(m_xkey.size()==0) {
      if(m_index<m_c.size()) r = m_c.line(m_index,m_index);
    }
    else {
      if(m_index<m_u.size()) {
	r = m_c;
	switch(m_xkey.size()) {
	case 4: r=r.match(m_xkey[3],m_u[m_index][3].tostring()); 
	case 3: r=r.match(m_xkey[2],m_u[m_index][2].tostring()); 
	case 2: r=r.match(m_xkey[1],m_u[m_index][1].tostring()); 
	case 1: r=r.match(m_xkey[0],m_u[m_index][0].tostring()); break;
	default: 
	  fprintf(stderr,"Error: CollectionIterator::get() Too many xindex\n");
	  break;
	}
      }
    }
    return(r);
  }
  Collection _get() {
    Collection r;
    if(m_index<m_u.size()) r = m_c._match(m_xkey,m_u[m_index][0].tostring());
    return(r);
  }
  //////////////////////////////////////////////////////////
  Collection operator*() {return(get());}
  CollectionIterator& operator++() { // ++prefix
    ++m_index; if(m_index>m_u.size()) m_index=m_u.size();
    return(*this);
  } 
  CollectionIterator operator++(int) { // postfix++
    CollectionIterator iter= *this;
    ++m_index; if(m_index>m_u.size()) m_index=m_u.size();
    return(iter);
  }
  CollectionIterator& operator--() { // --prefix
    --m_index; if(m_index<0) m_index=m_u.size();
    return(*this);
  } 
  CollectionIterator operator--(int) { // postfix--
    CollectionIterator iter= *this;
    --m_index; if(m_index<0) m_index=m_u.size();
    return(iter);
  }
  Collection Next() {
    Collection r=get();
    ++m_index;
    return(r);
  }
  //////////////////////////////////////////////////////////
  void disp() {
    m_u.disp();
    for(int i=0;i<m_xkey.size();i++) printf(" %d:%d ",i,m_xkey[i]); 
    printf("\nindex=%d\n",m_index);
  }
  //////////////////////////////////////////////////////////
  friend bool operator==(const CollectionIterator& a,const CollectionIterator& b);
  friend bool operator!=(const CollectionIterator& a,const CollectionIterator& b);
  friend bool operator<(const CollectionIterator& a,const CollectionIterator& b);
};

inline bool operator==(const CollectionIterator& a,const CollectionIterator& b)
  {return(a.m_index==b.m_index);}
inline bool operator!=(const CollectionIterator& a,const CollectionIterator& b)
  {return(a.m_index!=b.m_index);}
inline bool operator<(const CollectionIterator& a,const CollectionIterator& b)
  {return(a.m_index<b.m_index);}

////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
inline FILE* EX_fopen(const char* fname,const char* mode) { 
  return(fopen(fname,mode));
}
///////////////////////////////////////////////////
inline void EX_fclose(FILE* fp) {
  fclose(fp);
}
///////////////////////////////////////////////////
inline int EX_fprintf(FILE* fp,const char* fmt,...) {
  va_list argptr;
  if(fmt) {
    va_start(argptr,fmt);
    vfprintf(fp,fmt,argptr);
    va_end(argptr);
  }
  return(-1);
}
///////////////////////////////////////////////////
inline int EX_printf(const char* fmt,...) {
  va_list argptr;
  if(fmt) {
    va_start(argptr,fmt);
    vprintf(fmt,argptr);
    va_end(argptr);
  }
  return(-1);
}


////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

void ClearSheet();

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
#endif
