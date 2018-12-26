/////////////////////////////////////////////////////////////////////////////
// FileStrCollection.h
//
//  CSV table manipulator (off-memory)
// - Collection.h/cxx         : On-memory 
// - FileStrCollection.h/cxx  : On-disk, N files
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

#ifndef FILESTRCOLLECTION_H
#define FILESTRCOLLECTION_H
#include "Collection.h"
#ifdef UNIQUE_HIST
#include "Histogram.h"
#endif

//#define FILESTRCOLLECTION_ITER

//static const unsigned int  OPEN        =0x01;
static const unsigned int  NEW_FILE    =0x01;
static const unsigned int  ENDOF_FILE  =0x02;
static const unsigned int  ENDOF_FEXP  =0x04;
static const unsigned int  ENDOF_ALL   =0x08;

/////////////////////////////////////////////////////////////////////////////
typedef int (*ForeachFunc)(const Line& l);

/////////////////////////////////////////////////////////////////////////////
class FileStrCollection : public Collection {
 public:

  ////////////////////////////////////////////////////////////////////////
  FileStrCollection() { m_flag=0; }
  FileStrCollection(const string& fname,int key=0
		    ,EasyExp::Mode m=EasyExp::Auto) { init(fname,key,m); }
  ~FileStrCollection() { }

  void addfiles(const string& fexpr,EasyExp::Mode m=EasyExp::Auto) { m_fexpr.push_back(fexpr); m_fexprflag.push_back(m); }

  void init(const string& fexpr,int key=0,EasyExp::Mode m=EasyExp::Auto) ; 
  void clear() { 
    Line key = getkey();
    Collection::clear(); 
    m_sheet.clear(); 
    push_back(key);
    setkey(0);
  }

  ////////////////////////////////////////////////////////////////////////
  void setseparator(const char *separatorin) {m_separator=separatorin;} 
  void setdelimiter(const char *delimiterin) {m_delimiter=delimiterin;}
  void setendofline(const char *endoflinein) {m_endofline=endoflinein;} 
  void setquotation(const char *quotationin) {m_quotation=quotationin;} 
  void setkeepquotation(bool flag) { m_keepquotation = flag; }
  string m_separator;
  string m_delimiter;
  string m_endofline;
  string m_quotation;
  bool   m_keepquotation;

  //void setseparator(const char *separatorin) {m_f.setseparator(separatorin);} 
  //void setdelimiter(const char *delimiterin) {m_f.setdelimiter(delimiterin);}
  //void setendofline(const char *endoflinein) {m_f.setendofline(endoflinein);} 
  //void setquotation(const char *quotationin) {m_f.setquotation(quotationin);} 
  //void setkeepquotation(bool flag) { m_f.setkeepquotation(flag); }
  void SetCodingSystem() {m_f.SetCodingSystem();}
  void SetCodingSystem(int codingsystem) {m_f.SetCodingSystem(codingsystem);}
  int GetCodingSystem() {return(m_f.GetCodingSystem());}

  ////////////////////////////////////////////////////////////////////////

  virtual Collection  match(int x,const string& expr,bool icase=false);
  virtual Collection _match(int x,const string& expr,bool icase=false);
  virtual Collection  range(int x,double lower,double upper);
  virtual Collection _range(int x,double lower,double upper);
  virtual Collection  line(int from,int to= -1);
  virtual Collection _line(int from,int to= -1);
  virtual Collection  empty(int x);
  virtual Collection _empty(int x);
  virtual Collection  empty();
  virtual Collection _empty();

  ////////////////////////////////////////////////////////////////////////
  virtual Collection unique(int x,int x2=-1,int x3=-1);

  virtual Collection unique_count(int x,int x2=-1,int x3=-1,int x4=-1);
  virtual Collection unique_stat(map<string,statistics>& mexist,int xval,int x,int x2=-1,int x3=-1,int x4=-1) ;
  virtual Collection unique_stat(int xval,int x,int x2=-1,int x3=-1,int x4=-1) {
    map<string,statistics> mexist;
    return(unique_stat(mexist,xval,x,x2,x3,x4)); 
  }
  virtual Collection unique_list(int xval,int x,int x2=-1,int x3=-1,int x4=-1);
  //virtual Collection unique_substr_count(int key,const string& separator,int i1,int i2=-1,int i3=-1,int i4=-1);

  ////////////////////////////////////////////////////////////////////////
#ifdef UNIQUE_HIST
  virtual Collection unique_hist(map<string,statistics>& mstat
				 ,map<string,Histogram<double> >& mexist
				 ,double minstddev,double maxstddev,int nbin
				 ,int xval,int x,int x2=-1,int x3=-1,int x4=-1);
  virtual Collection unique_hist(double minstddev,double maxstddev,int nbin,
				 int xval,int x,int x2=-1,int x3=-1,int x4=-1) ;
  virtual Collection unique_hist(double minstddev,double maxstddev,int nbin,
				 const char* xval,const char* x
				 ,const char* x2="",const char* x3="",const char* x4="") {
    int ixval = getx(xval);
    int ix = getx(x);
    int ix2 = x2[0]? getx(x2) : -1;
    int ix3 = x3[0]? getx(x3) : -1;
    int ix4 = x4[0]? getx(x4) : -1;
    return(unique_hist(minstddev,maxstddev,nbin,ixval,ix,ix2,ix3,ix4));
  }
  virtual Collection unique_hist(double minstddev,double maxstddev,int nbin,
				 const string& xval,const string& x
				 ,const string& x2="",const string& x3="",const string& x4="") {
    return(unique_hist(minstddev,maxstddev,nbin,xval.c_str()
		       ,x.c_str(),x2.c_str(),x3.c_str(),x4.c_str()));
  }
#endif

  ////////////////////////////////////////////////////////////////////////
  virtual void unique_split(const Line& keyin,const char* targetdir
			    ,const char* xchar,int x,int x2=-1,int x3=-1,int x4=-1);
  virtual void unique_split(const Line& keyin,const char* targetdir
			    ,const char* xchar
			    ,const char* x,const char* x2="",const char* x3="",const char* x4="")
  {unique_split(keyin,targetdir,xchar,getx(x) ,getx(x2),getx(x3),getx(x4));}
  virtual void unique_split(const Line& keyin,const string& targetdir
			    ,const string& xchar
			    ,const string& x,const string& x2="",const string& x3="",const string& x4="")
  {unique_split(keyin,targetdir.c_str(),xchar.c_str(),x.c_str(),x2.c_str(),x3.c_str(),x4.c_str());}
  //virtual Collection unique_incrementalclustering(int x,int x2=-1,int x3=-1,int x4=-1);

  ////////////////////////////////////////////////////////////////////////
  Collection randomsampling(int n,int seed=0);

  ////////////////////////////////////////////////////////////////////////
  // Column extraction
  virtual Collection  column(int from,int to= -1); // line persistent
  virtual Collection _column(int from,int to= -1); // line persistent

  virtual Collection  column(const vector<int>& p);
  virtual Collection _column(const vector<int>& p);

  ////////////////////////////////////////////////////////////////////////
  int foreach(const char* f,int x=-1,const string& expr="",bool icase=false) ;
  int foreach(ForeachFunc& f,int x=-1,const string& expr="",bool icase=false) ;

  ////////////////////////////////////////////////////////////////////////
  int size() ;


  ////////////////////////////////////////////////////////////////////////
  void rewind(int readkey=1);
  int  read(Line& p);
  int  isvalid();
  int  linenum() const { return(m_f.line); }
  const vector<string>&  fexpr() const { return(m_fexpr); }
  const char*  fname() const { return(m_f.fname.c_str()); }
  bool isnewfile() const { return(m_flag&NEW_FILE?true:false); }
  bool isendoffile() const { return(m_flag&ENDOF_FILE?true:false); }
  bool isendoffexpr() const { return(m_flag&ENDOF_FEXP?true:false); }
  bool isendoffall() const { return(m_flag&ENDOF_ALL?true:false); }

  ////////////////////////////////////////////////////////////////////////
 private:
  Sheet           m_sheet; // buffer of stored lines, this grows big
  int             m_key;
  unsigned int    totalline;

  // 3 layers of iteration
  vector<string>         m_fexpr; 
  vector<EasyExp::Mode>  m_fexprflag;
  unsigned int m_ifexpr;

  Scandir         m_d;
  ReadF           m_f;

  unsigned int    m_flag;

 public:
#ifdef FILESTRCOLLECTION_ITER
  class iterator {
   public:
    iterator() {  }
    iterator(FileStrCollection& sc) { f=sc.f; }
    Line& operator*() { return(p); }
    iterator& operator++() { f.read(); p.set(f); return(*this); }
    iterator operator++(int) 
      { iterator tmp= *this; f.read(); p.set(f); return(tmp); }
   //private:
    ReadF f;
    Line p;
  };
  iterator begin() { 
    iterator it(*this);
    it.f.rewind(0); 
    it.f.read(); 
    it.p.set(it.f);
    return(it); 
  }
  iterator end() { 
    iterator it(*this); 
    it.f.line= -1; 
    return(it); 
  }
  friend iterator;
#else
#endif

};
/////////////////////////////////////////////////////////////////////////////
#ifdef FILESTRCOLLECTION_ITER
bool operator==(FileStrCollection::iterator& a,FileStrCollection::iterator& b) 
  {return(a.f.fp==b.f.fp && a.f.line==b.f.line);}
bool operator!=(FileStrCollection::iterator& a,FileStrCollection::iterator& b) 
  {return(a.f.fp!=b.f.fp || a.f.line!=b.f.line);}
#endif

/////////////////////////////////////////////////////////////////////////////
#endif // FILESTRCOLLECTION_H

