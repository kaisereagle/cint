/////////////////////////////////////////////////////////////////////////////
// FileStrCollection.cxx
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

#include "FileStrCollection.h"

// Line object was stored twice in m_sheet, p_sheet. Avoid duplication by not
// defining following macro
//#define DUPLICATESTORAGE

template<class T> static inline T max(const T& a,const T& b) {return(a>b?a:b);}
template<class T> static inline T min(const T& a,const T& b) {return(a<b?a:b);}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void FileStrCollection::init(const string& fexpr,int key,EasyExp::Mode m) { 
  if(fexpr!="") {
    m_fexpr.push_back(fexpr);
    m_fexprflag.push_back(m);
  }

  if(fexpr.find(".csv")!=string::npos /*<100000*/) {
    m_f.setdelimiter(",");
    m_f.setseparator("");
  }
  else if(fexpr.find(".tsv")!=string::npos /*<100000*/) {
    m_f.setdelimiter("\t");
    m_f.setseparator("");
  }
  else {
    //m_f.setdelimiter(",");
    m_f.setdelimiter(",");
    m_f.setseparator("");
  }
  m_f.setquotation("'\"");
  m_f.setescapebyquotation(true);

  if(m_separator.size()) m_f.setseparator(m_separator.c_str());
  if(m_delimiter.size()) m_f.setdelimiter(m_delimiter.c_str());
  if(m_endofline.size()) m_f.setendofline(m_endofline.c_str());
  if(m_quotation.size()) m_f.setquotation(m_quotation.c_str());
  m_f.setkeepquotation(m_keepquotation);

  rewind(0);
  m_flag = NEW_FILE;
  if(key>=0) { 
    m_key=0;
    for(int i=0;i<=key;i++) m_f.read();
    m_f.fseek_cur(-1);
    m_f.getpos(0);
    Line line;
    line.set(m_f);
    m_sheet.m_lines.push_back(line);
    p_key = &m_sheet.m_lines.back();
  }
  else m_key = -1;

  p_sheet = &m_sheet;
  totalline = 0;

  if(m_f.isvalid()) m_flag=0;
  else              m_flag=ENDOF_FILE;

}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// Collection virtual function
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
Collection  FileStrCollection::match(int x,const string& expr,bool icase) {
  EasyExp regexpr(expr.c_str(),m_EasyExpMode);
  Collection sub;
  sub.setsheet(p_sheet);
  sub.p_key=p_key;
  sub.m_stat = m_stat;

  rewind();
  Line p;
  while(read(p)) {
    value v;
    if(p.size()>x) v = p[x];
    if(regexpr.match(v.s.c_str(),icase)) {
#ifndef DUPLICATESTORAGE
      sub.push_back(p);
#else
      m_sheet.m_lines.push_back(p); // duplication, this is done below
      sub.push_back(&m_sheet.m_lines.back());
#endif
    }
  }
  return(sub);
}

/////////////////////////////////////////////////////////////////////////////
Collection FileStrCollection::_match(int x,const string& expr,bool icase) {
  EasyExp regexpr(expr.c_str(),m_EasyExpMode);
  Collection sub;
  sub.setsheet(p_sheet);
  sub.p_key=p_key;
  sub.m_stat = m_stat;

  rewind();
  Line p;
  while(read(p)) {
    value v;
    if(p.size()>x) v = p[x];
    if(!regexpr.match(v.s.c_str(),icase)) {
#ifndef DUPLICATESTORAGE
      sub.push_back(p);
#else
      m_sheet.m_lines.push_back(p);
      sub.push_back(&m_sheet.m_lines.back());
#endif
    }
  }
  return(sub);
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
Collection  FileStrCollection::range(int x,double lower,double upper) {
  Collection sub;
  sub.setsheet(p_sheet);
  sub.p_key=p_key;
  sub.m_stat = m_stat;

  rewind();
  Line p;
  while(read(p)) {
    if(p.size()>x) {
      const value& v = p[x];
      if(lower<=v.d && v.d<=upper) {
#ifndef DUPLICATESTORAGE
	sub.push_back(p);
#else
        m_sheet.m_lines.push_back(p);
        sub.push_back(&m_sheet.m_lines.back());
#endif
      }
    }
  }
  return(sub);
}

/////////////////////////////////////////////////////////////////////////////
Collection FileStrCollection::_range(int x,double lower,double upper) {
  Collection sub;
  sub.setsheet(p_sheet);
  sub.p_key=p_key;
  sub.m_stat = m_stat;

  rewind();
  Line p;
  while(read(p)) {
    if(p.size()>x) {
      const value& v = p[x];
      if(lower>v.d || v.d>upper) {
#ifndef DUPLICATESTORAGE
	sub.push_back(p);
#else
        m_sheet.m_lines.push_back(p);
        sub.push_back(&m_sheet.m_lines.back());
#endif
      }
    }
  }
  return(sub);
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
Collection  FileStrCollection::line(int from,int to) {
  if(to<from) to=from;
  Collection sub;
  sub.setsheet(p_sheet);
  sub.p_key=p_key;
  sub.m_stat = m_stat;
  int first = ::max(0,from);
  int last  = to+1;
  int i=0;
  rewind();
  Line p;
  while(i++<first) read(p) ;
  for(i=first;i<last;++i) {
    if(!m_f.isvalid()) break;
    read(p);
#ifndef DUPLICATESTORAGE
    sub.push_back(p);
#else
    m_sheet.m_lines.push_back(p);
    sub.push_back(&m_sheet.m_lines.back());
#endif
  }
  return(sub);
}
/////////////////////////////////////////////////////////////////////////////
Collection FileStrCollection::_line(int from,int to) {
  if(to<from) to=from;
  Collection sub;
  sub.setsheet(p_sheet);
  sub.p_key=p_key;
  sub.m_stat = m_stat;
  int first = ::max(0,from);
  int last  = to+1;
  int i=0;
  rewind();
  Line p;

  while(i<first) {
    if(!m_f.isvalid()) break;
    read(p);
#ifndef DUPLICATESTORAGE
    sub.push_back(p);
#else
    m_sheet.m_lines.push_back(p);
    sub.push_back(&m_sheet.m_lines.back());
#endif
    ++i;
  }
  for(i=first;i<last;++i) m_f.read();
  while(read(p)) {
#ifndef DUPLICATESTORAGE
    sub.push_back(p);
#else
    m_sheet.m_lines.push_back(p);
    sub.push_back(&m_sheet.m_lines.back());
#endif
  }
  return(sub);
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
Collection  FileStrCollection::empty(int x) {
  Collection sub;
  sub.setsheet(p_sheet);
  sub.p_key=p_key;
  sub.m_stat = m_stat;

  rewind();
  Line p;
  while(read(p)) {
    if(p.size()<=x) {
#ifndef DUPLICATESTORAGE
      sub.push_back(p);
#else
      m_sheet.m_lines.push_back(p);
      sub.push_back(&m_sheet.m_lines.back());
#endif
      continue;
    }
    const value& v = p[x];
    if(v.s.size()==0) {
#ifndef DUPLICATESTORAGE
      sub.push_back(p);
#else
      m_sheet.m_lines.push_back(p);
      sub.push_back(&m_sheet.m_lines.back());
#endif
    }
  }
  return(sub);
}

/////////////////////////////////////////////////////////////////////////////
Collection FileStrCollection::_empty(int x) {
  Collection sub;
  sub.setsheet(p_sheet);
  sub.p_key=p_key;
  sub.m_stat = m_stat;

  rewind();
  Line p;
  while(read(p)) {
    if(p.size()<=x) continue;
    const value& v = p[x];
    if(v.s.size()) {
#ifndef DUPLICATESTORAGE
      sub.push_back(p);
#else
      m_sheet.m_lines.push_back(p);
      sub.push_back(&m_sheet.m_lines.back());
#endif
    }
  }
  return(sub);
}

/////////////////////////////////////////////////////////////////////////////
Collection  FileStrCollection::empty() {
  Collection sub;
  sub.setsheet(p_sheet);
  sub.p_key=p_key;
  sub.m_stat = m_stat;

  rewind();
  Line p;
  while(read(p)) {
    if(p.empty()) {
#ifndef DUPLICATESTORAGE
      sub.push_back(p);
#else
      m_sheet.m_lines.push_back(p);
      sub.push_back(&m_sheet.m_lines.back());
#endif
    }
  }
  return(sub);
}

/////////////////////////////////////////////////////////////////////////////
Collection FileStrCollection::_empty() {
  Collection sub;
  sub.setsheet(p_sheet);
  sub.p_key=p_key;
  sub.m_stat = m_stat;

  rewind();
  Line p;
  while(read(p)) {
    if(p._empty()) {
#ifndef DUPLICATESTORAGE
      sub.push_back(p);
#else
      m_sheet.m_lines.push_back(p);
      sub.push_back(&m_sheet.m_lines.back());
#endif
    }
  }
  return(sub);
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
Collection FileStrCollection::unique(int x,int x2,int x3) {
  Collection sub;
  // copy common property
  sub.setsheet(p_sheet);
  sub.p_key=p_key;
  sub.m_stat = m_stat;
  if(x<0) return(sub);

  Line line;
  Collection exist;
  value keyvalue;
  //value countvalue;
  // copy unique
  if(x2<0) {

    rewind();
    Line p;
    while(read(p)) {
      keyvalue = p.get(x);
      exist=sub.match(x,keyvalue.s);
      if(exist.size()==0) { // new item
#ifndef DUPLICATESTORAGE
	sub.push_back(p);
#else
        m_sheet.m_lines.push_back(p);
        sub.push_back(&m_sheet.m_lines.back());
#endif
      }
    }
  }
  else if(x3<0) {
    value keyvalue2;

    rewind();
    Line p;
    while(read(p)) {
      keyvalue = p.get(x);
      keyvalue2 = p.get(x2);

      exist=sub.match(x,keyvalue.s).match(x2,keyvalue2.s);
      if(exist.size()==0) { // new item
#ifndef DUPLICATESTORAGE
	sub.push_back(p);
#else
        m_sheet.m_lines.push_back(p);
        sub.push_back(&m_sheet.m_lines.back());
#endif
      }
    }
  }
  else {
    value keyvalue2,keyvalue3;

    rewind();
    Line p;
    while(read(p)) {
      keyvalue = p.get(x);
      keyvalue2 = p.get(x2);
      keyvalue3 = p.get(x3);
      exist=sub.match(x,keyvalue.s).match(x2,keyvalue2.s).match(x3,keyvalue3.s);
      if(exist.size()==0) { // new item
#ifndef DUPLICATESTORAGE
	sub.push_back(p);
#else
        m_sheet.m_lines.push_back(p);
        sub.push_back(&m_sheet.m_lines.back());
#endif
      }
    }
  }
  return(sub);
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// list all result of each category in one line
//   keyx,keyx2,keyx3,count
//    ...
/////////////////////////////////////////////////////////////////////////////
Collection FileStrCollection::unique_count(int x,int x2,int x3,int x4) {
  Collection sub;
  // copy common property
  sub.setsheet(p_sheet);
  sub.p_key=p_key;
  sub.m_stat = m_stat;
  if(x<0) return(sub);

  // create key
  Line key;
  if(p_key) {
    key.push_back(getkey().get(x).c_str());
    if(x2>=0) key.push_back(getkey().get(x2).c_str());
    if(x3>=0) key.push_back(getkey().get(x3).c_str());
    if(x4>=0) key.push_back(getkey().get(x4).c_str());
  }
  else {
    key.push_back(x);
    if(x2>=0) key.push_back(x2);
    if(x3>=0) key.push_back(x3);
    if(x4>=0) key.push_back(x4);
  }
  key.push_back("count");
  sub.push_back(key);
  sub.setkey(0);

  Line line;
  Collection exist;
  map<string,int> mexist;
  string skeyvalue;
  string sep="#";
  value keyvalue;
  //value countvalue;
  // copy unique
  if(x2<0) {

    rewind();
    Line p;
    while(read(p)) {
      keyvalue = p.get(x);
      ++mexist[keyvalue.tostring()];
    }
    map<string,int>::iterator first=mexist.begin();
    map<string,int>::iterator last =mexist.end();
    Line line;
    while(first!=last) {
      line.let(0,(*first).first);
      line.let(1,(*first).second);
#ifndef DUPLICATESTORAGE
      sub.push_back(p);
#else
      m_sheet.m_lines.push_back(line);
      sub.push_back(&m_sheet.m_lines.back());
#endif
      ++first;
    }
  }
  else if(x3<0) {
    value keyvalue2;

    rewind();
    Line p;
    while(read(p)) {
      keyvalue = p.get(x);
      keyvalue2 = p.get(x2);
      skeyvalue = keyvalue.tostring() + sep + keyvalue2.tostring();
      ++mexist[skeyvalue];
    }
    map<string,int>::iterator first=mexist.begin();
    map<string,int>::iterator last =mexist.end();
    Line line;
    ReadF ps; ps.setdelimiter(sep.c_str()); ps.setseparator("");
    while(first!=last) {
      ps.parse((*first).first.c_str());
      line.let(0,ps.argv[1]);
      line.let(1,ps.argv[2]);
      line.let(2,(*first).second);
      p_sheet->push_back(line);
      sub.push_back(&p_sheet->back());
      ++first;
    }
  }
  else if(x4<0) {
    value keyvalue2,keyvalue3;

    rewind();
    Line p;
    while(read(p)) {
      keyvalue = p.get(x);
      keyvalue2 = p.get(x2);
      keyvalue3 = p.get(x3);
      skeyvalue = keyvalue.tostring() + sep + keyvalue2.tostring() + sep 
	          + keyvalue3.tostring();
      ++mexist[skeyvalue];
    }
    map<string,int>::iterator first=mexist.begin();
    map<string,int>::iterator last =mexist.end();
    Line line;
    ReadF ps; ps.setdelimiter(sep.c_str()); ps.setseparator("");
    while(first!=last) {
      ps.parse((*first).first.c_str());
      line.let(0,ps.argv[1]);
      line.let(1,ps.argv[2]);
      line.let(2,ps.argv[3]);
      line.let(3,(*first).second);
      p_sheet->push_back(line);
      sub.push_back(&p_sheet->back());
      ++first;
    }
  }
  else {
    value keyvalue2,keyvalue3,keyvalue4;

    rewind();
    Line p;
    while(read(p)) {
      keyvalue = p.get(x);
      keyvalue2 = p.get(x2);
      keyvalue3 = p.get(x3);
      keyvalue4 = p.get(x4);
      skeyvalue = keyvalue.tostring() + sep + keyvalue2.tostring() + sep 
	        + keyvalue3.tostring() + sep + keyvalue4.tostring();
      ++mexist[skeyvalue];
    }
    map<string,int>::iterator first=mexist.begin();
    map<string,int>::iterator last =mexist.end();
    Line line;
    ReadF ps; ps.setdelimiter(sep.c_str()); ps.setseparator("");
    while(first!=last) {
      ps.parse((*first).first.c_str());
      line.let(0,ps.argv[1]);
      line.let(1,ps.argv[2]);
      line.let(2,ps.argv[3]);
      line.let(3,ps.argv[4]);
      line.let(4,(*first).second);
      p_sheet->push_back(line);
      sub.push_back(&p_sheet->back());
      ++first;
    }
  }
  return(sub);
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// calculate statistics of unique key value collection
//   keyx,keyx2,keyx3,nsample,mean,stddev,min,max.
//    ...
/////////////////////////////////////////////////////////////////////////////
Collection FileStrCollection::unique_stat(map<string,statistics>& mexist
					  ,int xval,int x,int x2,int x3,int x4) {
  Collection sub;
  // copy common property
  sub.setsheet(p_sheet);
  sub.p_key=p_key;
  sub.m_stat = m_stat;
  if(x<0) return(sub);

  // create key
  Line key;
  if(p_key) {
    key.push_back(getkey().get(x).c_str());
    if(x2>=0) key.push_back(getkey().get(x2).c_str());
    if(x3>=0) key.push_back(getkey().get(x3).c_str());
    if(x4>=0) key.push_back(getkey().get(x4).c_str());
  }
  else {
    key.push_back(x);
    if(x2>=0) key.push_back(x2);
    if(x3>=0) key.push_back(x3);
    if(x4>=0) key.push_back(x4);
  }
  key.push_back("count");// key.push_back("nsample");
  key.push_back("mean");
  key.push_back("stddev");
  key.push_back("min");
  key.push_back("max");
  key.push_back("sigma");

  sub.push_back(key);
  sub.setkey(0);

  Line line;
  Collection exist;
#define MAP_UNIQUE_COUNT
  //map<string,statistics> mexist;
  string skeyvalue;
  string sep="#";
  value keyvalue;
  //value countvalue;
  // copy unique
  if(x2<0) {

    rewind();
    Line p;
    while(read(p)) {
      keyvalue = p.get(x);
      mexist[keyvalue.tostring()].add(p.get(xval).todouble());
    }
    map<string,statistics>::iterator first=mexist.begin();
    map<string,statistics>::iterator last =mexist.end();
    Line line;
    while(first!=last) {
      line.let(0,(*first).first);
      line.let(1,(*first).second.nsample());
      line.let(2,(*first).second.mean());
      line.let(3,(*first).second.stddev());
      line.let(4,(*first).second.min());
      line.let(5,(*first).second.max());
      line.let(6,(*first).second.sigma());
      p_sheet->push_back(line);
      sub.push_back(&p_sheet->back());
      ++first;
    }
  }
  else if(x3<0) {
    value keyvalue2;

    rewind();
    Line p;
    while(read(p)) {
      keyvalue = p.get(x);
      keyvalue2 = p.get(x2);
      skeyvalue = keyvalue.tostring() + sep + keyvalue2.tostring();
      mexist[skeyvalue].add(p.get(xval).todouble());
    }
    map<string,statistics>::iterator first=mexist.begin();
    map<string,statistics>::iterator last =mexist.end();
    Line line;
    ReadF ps; ps.setdelimiter(sep.c_str()); ps.setseparator("");
    while(first!=last) {
      ps.parse((*first).first.c_str());
      line.let(0,ps.argv[1]);
      line.let(1,ps.argv[2]);
      line.let(2,(*first).second.nsample());
      line.let(3,(*first).second.mean());
      line.let(4,(*first).second.stddev());
      line.let(5,(*first).second.min());
      line.let(6,(*first).second.max());
      line.let(7,(*first).second.sigma());
      p_sheet->push_back(line);
      sub.push_back(&p_sheet->back());
      ++first;
    }
  }
  else if(x4<0) {
    value keyvalue2,keyvalue3;

    rewind();
    Line p;
    while(read(p)) {
      keyvalue = p.get(x);
      keyvalue2 = p.get(x2);
      keyvalue3 = p.get(x3);
      skeyvalue = keyvalue.tostring() + sep + keyvalue2.tostring() + sep 
	          + keyvalue3.tostring();
      mexist[skeyvalue].add(p.get(xval).todouble());
    }
    map<string,statistics>::iterator first=mexist.begin();
    map<string,statistics>::iterator last =mexist.end();
    Line line;
    ReadF ps; ps.setdelimiter(sep.c_str()); ps.setseparator("");
    while(first!=last) {
      ps.parse((*first).first.c_str());
      line.let(0,ps.argv[1]);
      line.let(1,ps.argv[2]);
      line.let(2,ps.argv[3]);
      line.let(3,(*first).second.nsample());
      line.let(4,(*first).second.mean());
      line.let(5,(*first).second.stddev());
      line.let(6,(*first).second.min());
      line.let(7,(*first).second.max());
      line.let(8,(*first).second.sigma());
      p_sheet->push_back(line);
      sub.push_back(&p_sheet->back());
      ++first;
    }
  }
  else {
    value keyvalue2,keyvalue3,keyvalue4;

    rewind();
    Line p;
    while(read(p)) {
      keyvalue = p.get(x);
      keyvalue2 = p.get(x2);
      keyvalue3 = p.get(x3);
      keyvalue4 = p.get(x4);
      skeyvalue = keyvalue.tostring() + sep + keyvalue2.tostring() + sep 
	        + keyvalue3.tostring() + sep + keyvalue4.tostring();
      mexist[skeyvalue].add(p.get(xval).todouble());
    }
    map<string,statistics>::iterator first=mexist.begin();
    map<string,statistics>::iterator last =mexist.end();
    Line line;
    ReadF ps; ps.setdelimiter(sep.c_str()); ps.setseparator("");
    while(first!=last) {
      ps.parse((*first).first.c_str());
      line.let(0,ps.argv[1]);
      line.let(1,ps.argv[2]);
      line.let(2,ps.argv[3]);
      line.let(3,ps.argv[4]);
      line.let(4,(*first).second.nsample());
      line.let(5,(*first).second.mean());
      line.let(6,(*first).second.stddev());
      line.let(7,(*first).second.min());
      line.let(8,(*first).second.max());
      line.let(9,(*first).second.sigma());
      p_sheet->push_back(line);
      sub.push_back(&p_sheet->back());
      ++first;
    }
  }
  return(sub);
}
/////////////////////////////////////////////////////////////////////////////
// list all result of each category in one line
//   keyx,keyx2,keyx3,count,xval,
//   valx,valx2,valx3,n1   ,val1,val2,val3, ...
//   valx,valx2,valx3,n2   ,val1,val2,val3, ...
//    ...
/////////////////////////////////////////////////////////////////////////////
Collection FileStrCollection::unique_list(int xval,int x,int x2,int x3,int x4) {
  Collection sub;
  // copy common property
  sub.setsheet(p_sheet);
  sub.p_key=p_key;
  sub.m_stat = m_stat;
  if(x<0) return(sub);

  // create key
  Line key;
  if(p_key) {
    key.push_back(getkey().get(x).c_str());
    if(x2>=0) key.push_back(getkey().get(x2).c_str());
    if(x3>=0) key.push_back(getkey().get(x3).c_str());
    if(x4>=0) key.push_back(getkey().get(x4).c_str());
  }
  else {
    key.push_back(x);
    if(x2>=0) key.push_back(x2);
    if(x3>=0) key.push_back(x3);
    if(x4>=0) key.push_back(x4);
  }
  key.push_back("count");// key.push_back("nsample");
  key.push_back("data...");

  sub.push_back(key);
  sub.setkey(0);

  Line line;
  Collection exist;
  map<string,vector<double> > mexist;
  string skeyvalue;
  string sep="#";
  value keyvalue;
  //value countvalue;
  // copy unique
  if(x2<0) {

    rewind();
    Line p;
    while(read(p)) {
      keyvalue = p.get(x);
      mexist[keyvalue.tostring()].push_back(p.get(xval).todouble());
    }
    map<string,vector<double> >::iterator first=mexist.begin();
    map<string,vector<double> >::iterator last =mexist.end();
    Line line;
    while(first!=last) {
      line.let(0,(*first).first);
      line.let(2,(*first).second.size());
      for(int ii=0;ii<(*first).second.size();ii++) 
        line.let(ii+2,(*first).second[ii]);
      p_sheet->push_back(line);
      sub.push_back(&p_sheet->back());
      ++first;
    }
  }
  else if(x3<0) {
    value keyvalue2;

    rewind();
    Line p;
    while(read(p)) {
      keyvalue = p.get(x);
      keyvalue2 = p.get(x2);
      skeyvalue = keyvalue.tostring() + sep + keyvalue2.tostring();
      mexist[skeyvalue].push_back(p.get(xval).todouble());
    }
    map<string,vector<double> >::iterator first=mexist.begin();
    map<string,vector<double> >::iterator last =mexist.end();
    Line line;
    ReadF ps; ps.setdelimiter(sep.c_str()); ps.setseparator("");
    while(first!=last) {
      ps.parse((*first).first.c_str());
      line.let(0,ps.argv[1]);
      line.let(1,ps.argv[2]);
      line.let(2,(*first).second.size());
      for(int ii=0;ii<(*first).second.size();ii++) 
        line.let(ii+3,(*first).second[ii]);
      p_sheet->push_back(line);
      sub.push_back(&p_sheet->back());
      ++first;
    }
  }
  else if(x3<0) {
    value keyvalue2,keyvalue3;

    rewind();
    Line p;
    while(read(p)) {
      keyvalue = p.get(x);
      keyvalue2 = p.get(x2);
      keyvalue3 = p.get(x3);
      skeyvalue = keyvalue.tostring() + sep + keyvalue2.tostring() + sep 
	          + keyvalue3.tostring();
      mexist[skeyvalue].push_back(p.get(xval).todouble());
    }
    map<string,vector<double> >::iterator first=mexist.begin();
    map<string,vector<double> >::iterator last =mexist.end();
    Line line;
    ReadF ps; ps.setdelimiter(sep.c_str()); ps.setseparator("");
    while(first!=last) {
      ps.parse((*first).first.c_str());
      line.let(0,ps.argv[1]);
      line.let(1,ps.argv[2]);
      line.let(2,ps.argv[3]);
      line.let(3,(*first).second.size());
      for(int ii=0;ii<(*first).second.size();ii++) 
        line.let(ii+4,(*first).second[ii]);
      p_sheet->push_back(line);
      sub.push_back(&p_sheet->back());
      ++first;
    }
  }
  else {
    value keyvalue2,keyvalue3,keyvalue4;

    rewind();
    Line p;
    while(read(p)) {
      keyvalue = p.get(x);
      keyvalue2 = p.get(x2);
      keyvalue3 = p.get(x3);
      keyvalue4 = p.get(x4);
      skeyvalue = keyvalue.tostring() + sep + keyvalue2.tostring() + sep 
	        + keyvalue3.tostring() + sep + keyvalue4.tostring();
      mexist[skeyvalue].push_back(p.get(xval).todouble());
    }
    map<string,vector<double> >::iterator first=mexist.begin();
    map<string,vector<double> >::iterator last =mexist.end();
    Line line;
    ReadF ps; ps.setdelimiter(sep.c_str()); ps.setseparator("");
    while(first!=last) {
      ps.parse((*first).first.c_str());
      line.let(0,ps.argv[1]);
      line.let(1,ps.argv[2]);
      line.let(2,ps.argv[3]);
      line.let(3,ps.argv[4]);
      line.let(4,(*first).second.size());
      for(int ii=0;ii<(*first).second.size();ii++) 
        line.let(ii+5,(*first).second[ii]);
      p_sheet->push_back(line);
      sub.push_back(&p_sheet->back());
      ++first;
    }
  }
  return(sub);
}
/////////////////////////////////////////////////////////////////////////////
#ifdef UNIQUE_HIST
/////////////////////////////////////////////////////////////////////////////
// 
//   keyx,keyx2,keyx3,nsample,xmin,... ,xmax
//    ...
/////////////////////////////////////////////////////////////////////////////
Collection FileStrCollection::unique_hist(
			double minstddev,double maxstddev,int nbin,
			int xval,int x,int x2,int x3,int x4){
  map<string,statistics> mstat;
  Collection cstat = unique_stat(mstat,xval,x,x2,x3,x4);
  map<string,Histogram<double> > mexist;
  return(unique_hist(mstat,mexist,minstddev,maxstddev,nbin,xval,x,x2,x3,x4)); 
}

/////////////////////////////////////////////////////////////////////////////
Collection FileStrCollection::unique_hist(
  				map<string,statistics>& mstat
				,map<string,Histogram<double> >& mexist
				,double minstddev,double maxstddev,int nbin
				,int xval,int x,int x2,int x3,int x4) {
  map<string,statistics>::iterator sfirst,slast=mstat.end();
  string skeyvalue;
  double vmin,vmax,vmean,vstddev;
  //int nbin=100;
  for(sfirst=mstat.begin();sfirst!=slast;++sfirst) {
    vmean   = (*sfirst).second.mean();
    vstddev = (*sfirst).second.stddev();
    vmin    = vmean - minstddev*vstddev;
    vmax    = vmean + maxstddev*vstddev;
    skeyvalue = (*sfirst).first;
    mexist[skeyvalue].setrange(nbin,vmin,vmax);
  }

  Collection sub;
  // copy common property
  sub.setsheet(p_sheet);
  sub.p_key=p_key;
  sub.m_stat = m_stat;
  if(x<0) return(sub);

  // create key
  Line key;
  if(p_key) {
    key.push_back(getkey().get(x).c_str());
    if(x2>=0) key.push_back(getkey().get(x2).c_str());
    if(x3>=0) key.push_back(getkey().get(x3).c_str());
    if(x4>=0) key.push_back(getkey().get(x4).c_str());
  }
  else {
    key.push_back(x);
    if(x2>=0) key.push_back(x2);
    if(x3>=0) key.push_back(x3);
    if(x4>=0) key.push_back(x4);
  }
  key.push_back("count");// key.push_back("nsample");

  int i;
  char buf[100];
  for(i=0;i<nbin;i++) {
    sprintf(buf,"h%d",i);
    key.push_back(buf);
  }

  sub.push_back(key);
  sub.setkey(0);

  Line line;
  //map<string,statistics> mexist;
  //string skeyvalue;
  string sep="#";
  value keyvalue;
  //value countvalue;
  // copy unique

  if(x2<0) {
    rewind();
    Line p;
    while(read(p)) {
      keyvalue = p.get(x);
      mexist[keyvalue.tostring()].fill(p.get(xval).todouble());
    }
    Line line;
    map<string,Histogram<double> >::iterator first=mexist.begin();
    map<string,Histogram<double> >::iterator last =mexist.end();
    while(first!=last) {
      line.let(0,(*first).first);
      line.let(1,(*first).second.sum());
      Histogram<double>& hd = (*first).second;
      for(i=0;i<nbin;i++) line.let(2+i,hd[i]);
      p_sheet->push_back(line);
      sub.push_back(&p_sheet->back());
      ++first;
    }
  }

  else if(x3<0) {
    value keyvalue2;
    rewind();
    Line p;
    while(read(p)) {
      keyvalue = p.get(x);
      keyvalue2 = p.get(x2);
      skeyvalue = keyvalue.tostring() + sep + keyvalue2.tostring();
      mexist[skeyvalue].fill(p.get(xval).todouble());
    }
    map<string,Histogram<double> >::iterator first=mexist.begin();
    map<string,Histogram<double> >::iterator last =mexist.end();
    Line line;
    ReadF ps; ps.setdelimiter(sep.c_str()); ps.setseparator("");
    while(first!=last) {
      ps.parse((*first).first.c_str());
      line.let(0,ps.argv[1]);
      line.let(1,ps.argv[2]);
      line.let(2,(*first).second.sum());
      Histogram<double>& hd = (*first).second;
      for(i=0;i<nbin;i++) line.let(3+i,hd[i]);
      p_sheet->push_back(line);
      sub.push_back(&p_sheet->back());
      ++first;
    }
  }

  else if(x4<0) {
    value keyvalue2,keyvalue3;
    rewind();
    Line p;
    while(read(p)) {
      keyvalue = p.get(x);
      keyvalue2 = p.get(x2);
      keyvalue3 = p.get(x3);
      skeyvalue = keyvalue.tostring() + sep + keyvalue2.tostring() + sep 
	          + keyvalue3.tostring();
      mexist[skeyvalue].fill(p.get(xval).todouble());
    }
    map<string,Histogram<double> >::iterator first=mexist.begin();
    map<string,Histogram<double> >::iterator last =mexist.end();
    Line line;
    ReadF ps; ps.setdelimiter(sep.c_str()); ps.setseparator("");
    while(first!=last) {
      ps.parse((*first).first.c_str());
      line.let(0,ps.argv[1]);
      line.let(1,ps.argv[2]);
      line.let(2,ps.argv[3]);
      line.let(3,(*first).second.sum());
      Histogram<double>& hd = (*first).second;
      for(i=0;i<nbin;i++) line.let(4+i,hd[i]);
      p_sheet->push_back(line);
      sub.push_back(&p_sheet->back());
      ++first;
    }
  }

  else {
    value keyvalue2,keyvalue3,keyvalue4;
    rewind();
    Line p;
    while(read(p)) {
      keyvalue = p.get(x);
      keyvalue2 = p.get(x2);
      keyvalue3 = p.get(x3);
      keyvalue4 = p.get(x3);
      skeyvalue = keyvalue.tostring() + sep + keyvalue2.tostring() + sep 
	        + keyvalue3.tostring() + sep + keyvalue4.tostring();
      mexist[skeyvalue].fill(p.get(xval).todouble());
    }
    map<string,Histogram<double> >::iterator first=mexist.begin();
    map<string,Histogram<double> >::iterator last =mexist.end();
    Line line;
    ReadF ps; ps.setdelimiter(sep.c_str()); ps.setseparator("");
    while(first!=last) {
      ps.parse((*first).first.c_str());
      line.let(0,ps.argv[1]);
      line.let(1,ps.argv[2]);
      line.let(2,ps.argv[3]);
      line.let(3,ps.argv[4]);
      line.let(4,(*first).second.sum());
      Histogram<double>& hd = (*first).second;
      for(i=0;i<nbin;i++) line.let(5+i,hd[i]);
      p_sheet->push_back(line);
      sub.push_back(&p_sheet->back());
      ++first;
    }
  }
  return(sub);
}
#endif

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// unique_split utilities
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
static string demangle(const string& targetdir,const string& s,const char* xchar) {
  // replace filename prohibited chars with _
  string xxchar = xchar;
  string r = s;
  size_t p;
  for(unsigned int i=0;i<s.size();i++) {
    p = xxchar.find(r[i]);
    if(p!=string::npos) r[i] = '_';
  }
  string fname ;
  if(targetdir!="") {
    fname = targetdir;
    fname += "/";
    fname += r;
  }
  else {
    fname = r;
  }
  fname += ".csv";
  return(fname);
}
/////////////////////////////////////////////////////////////////////////////
static void initial_split(const char* targetdir,const string& keyvalue
			,const char* xchar,const Collection& sub) {
  string fname = demangle(targetdir,keyvalue,xchar);
  sub.disp(fname.c_str(),"w");
}
/////////////////////////////////////////////////////////////////////////////
static void split_write(const char* targetdir,map<string,Collection>& mexist
			,const char* xchar) {
  map<string,Collection>::iterator first=mexist.begin();
  map<string,Collection>::iterator last =mexist.end();
  while(first!=last) {
    string fname = demangle(targetdir,(*first).first,xchar);
    Collection t = (*first).second;
    if(t.size()) {
      t.disp(fname.c_str(),"a");
      (*first).second.clear();
      //printf("%p\n",(*first).second.p_sheet);
    }
    ++first;
  }
  ClearSheet();
}
/////////////////////////////////////////////////////////////////////////////
// split big data file into mutiple files
//   [targetdir]/[x]_[x2]_[x3].csv
/////////////////////////////////////////////////////////////////////////////
void FileStrCollection::unique_split(const Line& keyin
	,const char* targetdir
	,const char* prohibitedchar // prohibited chars for file name
				     ,int x,int x2,int x3,int x4) {
  Sheet skey;
  Collection sub(&skey);
  // copy common property
  sub.setsheet(p_sheet);
  sub.m_stat = m_stat;

  if(keyin.size()) {
    sub.push_back(keyin);
    sub.setkey(0);   
  }
  else {
    sub.p_key=p_key;
  }
  if(x<0) return;


  Line line;
  Collection exist;
  map<string,Collection> mexist;
  map<string,int> mflag;
  string skeyvalue;
  string sep=":";
  value keyvalue;
  //value countvalue;
  // copy unique
  int accum=0;
  const int flushlim=10000;

  if(x2<0) {
    rewind();
    Line p;
    while(read(p)) {
      keyvalue = p.get(x);
      if(keyvalue.size()==0) continue;
      Collection& t=mexist[keyvalue.tostring()];
      int flag = mflag[keyvalue.tostring()]++;
      if(flag==0) initial_split(targetdir,keyvalue.tostring(),prohibitedchar,sub);
      t.push_back(p);
      if(++accum > flushlim) {
        split_write(targetdir,mexist,prohibitedchar);
        accum=0;
      }
    }
    split_write(targetdir,mexist,prohibitedchar);
  }

  else if(x3<0) {
    value keyvalue2;
    rewind();
    Line p;
    while(read(p)) {
      keyvalue = p.get(x);
      keyvalue2 = p.get(x2);
      skeyvalue = keyvalue.tostring() + sep + keyvalue2.tostring();
      if(skeyvalue.size()==1) continue;
      Collection& t = mexist[skeyvalue];
      int flag = mflag[skeyvalue]++;
      if(flag==0)  initial_split(targetdir,skeyvalue,prohibitedchar,sub);
      t.push_back(p);
      if(++accum > flushlim) {
        split_write(targetdir,mexist,prohibitedchar);
        accum=0;
      }
    }
    split_write(targetdir,mexist,prohibitedchar);
  }

  else if(x4<0) {
    value keyvalue2,keyvalue3;
    rewind();
    Line p;
    while(read(p)) {
      keyvalue = p.get(x);
      keyvalue2 = p.get(x2);
      keyvalue3 = p.get(x3);
      skeyvalue = keyvalue.tostring() + sep + keyvalue2.tostring() + sep 
	          + keyvalue3.tostring();
      if(skeyvalue.size()==2) continue;

      Collection& t = mexist[skeyvalue];
      int flag = mflag[skeyvalue]++;
      if(flag==0)  initial_split(targetdir,skeyvalue,prohibitedchar,sub);
      t.push_back(p);
      if(++accum > flushlim) {
        split_write(targetdir,mexist,prohibitedchar);
        accum=0;
      }
    }
    split_write(targetdir,mexist,prohibitedchar);
  }

  else {
    value keyvalue2,keyvalue3,keyvalue4;
    rewind();
    Line p;
    while(read(p)) {
      keyvalue = p.get(x);
      keyvalue2 = p.get(x2);
      keyvalue3 = p.get(x3);
      keyvalue4 = p.get(x4);
      skeyvalue = keyvalue.tostring() + sep + keyvalue2.tostring() + sep 
	        + keyvalue3.tostring() + sep + keyvalue4.tostring();
      if(skeyvalue.size()==2) continue;

      Collection& t = mexist[skeyvalue];
      int flag = mflag[skeyvalue]++;
      if(flag==0)  initial_split(targetdir,skeyvalue,prohibitedchar,sub);
      t.push_back(p);
      if(++accum > flushlim) {
        split_write(targetdir,mexist,prohibitedchar);
        accum=0;
      }
    }
    split_write(targetdir,mexist,prohibitedchar);
  }
}
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
Collection FileStrCollection::column(int from,int to) {
  if(to<0) to=from;
  Collection sub;
  sub.setsheet(p_sheet);
  sub.p_key=p_key;
  sub.m_stat = m_stat.column(from,to);

  rewind();
  Line line;
  while(read(line)) {
    line.set(m_f);
    line = line.column(from,to);
#ifndef DUPLICATESTORAGE
    sub.push_back(line);
#else
    m_sheet.m_lines.push_back(line);
    sub.push_back(&m_sheet.m_lines.back());
#endif
  }
  if(p_key) {
    Line line=p_key->column(from,to);
#ifndef DUPLICATESTORAGE
    sub.push_back(line);
#else
    m_sheet.m_lines.push_back(line);
    sub.p_key = &m_sheet.m_lines.back();
#endif
  }
  return(sub);
}
/////////////////////////////////////////////////////////////////////////////
Collection FileStrCollection::_column(int from,int to) {
  if(to<0) to=from;
  Collection sub;
  sub.setsheet(p_sheet);
  sub.p_key=p_key;
  sub.m_stat = m_stat._column(from,to);

  rewind();
  Line line;
  while(read(line)) {
    line.set(m_f);
    line = line._column(from,to);
#ifndef DUPLICATESTORAGE
    sub.push_back(line);
#else
    m_sheet.m_lines.push_back(line);
    sub.push_back(&m_sheet.m_lines.back());
#endif
  }
  if(p_key) {
    Line line=p_key->_column(from,to);
#ifndef DUPLICATESTORAGE
    sub.push_back(line);
#else
    m_sheet.m_lines.push_back(line);
    sub.p_key = &m_sheet.m_lines.back();
#endif
  }
  return(sub);
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
Collection FileStrCollection::column(const vector<int>& p) {
  Collection sub;
  sub.setsheet(p_sheet);
  sub.p_key=p_key;
  sub.m_stat = m_stat.column(p);

  rewind();
  Line line;
  while(read(line)) {
    line.set(m_f);
    line = line.column(p);
#ifndef DUPLICATESTORAGE
    sub.push_back(line);
#else
    m_sheet.m_lines.push_back(line);
    sub.push_back(&m_sheet.m_lines.back());
#endif
  }
  if(p_key) {
    Line line=p_key->column(p);
#ifndef DUPLICATESTORAGE
    sub.push_back(line);
#else
    m_sheet.m_lines.push_back(line);
    sub.p_key = &m_sheet.m_lines.back();
#endif
  }
  return(sub);
}
/////////////////////////////////////////////////////////////////////////////
Collection FileStrCollection::_column(const vector<int>& p) {
  Collection sub;
  sub.setsheet(p_sheet);
  sub.p_key=p_key;
  sub.m_stat = m_stat._column(p);

  rewind();
  Line line;
  while(read(line)) {
    line.set(m_f);
    line = line._column(p);
#ifndef DUPLICATESTORAGE
    sub.push_back(line);
#else
    m_sheet.m_lines.push_back(line);
    sub.push_back(&m_sheet.m_lines.back());
#endif
  }
  if(p_key) {
    Line line=p_key->_column(p);
#ifndef DUPLICATESTORAGE
    sub.push_back(line);
#else
    m_sheet.m_lines.push_back(line);
    sub.p_key = &m_sheet.m_lines.back();
#endif
  }
  return(sub);
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
Collection FileStrCollection::randomsampling(int n,int seed) {
  Collection sub;
  sub.setsheet(p_sheet);
  sub.p_key=p_key;
  sub.m_stat = m_stat;
  totalline =0;
  srand(seed);
  totalline = size(); 
  unsigned int M = totalline/n*2;
  if(M==0) M=1;
  unsigned int jump=rand()%M; // 
  unsigned int i=0;
  rewind();
  Line p;
  while(read(p)) {
    if(i>jump) {
      p.set(m_f);
#ifndef DUPLICATESTORAGE
      sub.push_back(p);
#else
      m_sheet.m_lines.push_back(p);
      sub.push_back(&m_sheet.m_lines.back());
#endif
      i=1;
      jump=rand()%M; // 
    }
    else ++i;
  }
  return(sub);
}
/////////////////////////////////////////////////////////////////////////////
int FileStrCollection::size() {
  rewind();
  Line p;
  totalline=0;
  while(read(p)) ++totalline;
  return(totalline);
}


/////////////////////////////////////////////////////////////////////////////
void FileStrCollection::rewind(int readkey) {
  // 3 layers of iteration
  m_ifexpr = 0;
  m_f.close();
  const char* fname;
  if(m_ifexpr<m_fexpr.size()) {
    m_d.scan(m_fexpr[m_ifexpr]);
    fname = m_d.Next();
    if(m_fexprflag[m_ifexpr]==EasyExp::Exact) fname=m_fexpr[m_ifexpr].c_str();
    if(fname && fname[0]) {
      m_f.open(fname);
      if(m_key>=0 && readkey) m_f.read();
    }
    else {
    }
  }
  else {
  }
}

/////////////////////////////////////////////////////////////////////////////
int FileStrCollection::read(Line& p) {
  p.clear();
  m_flag = 0;
  // ReadF next
  if(m_f.read()) {
    p.set(m_f);
    return(1);
  }
  m_f.close();
  m_flag = ENDOF_FILE;

  // Scandir next
  const char* fname = m_d.Next();
  if(fname && fname[0]) {
    m_f.open(fname);
    m_flag |= NEW_FILE;
    if(m_key>=0) m_f.read();
    if(m_f.read()) {
      p.set(m_f);
      return(1);
    }
    m_f.close();
    m_flag = ENDOF_FILE;
    return(1); // this is needed if file has EOF at the first line
  }

  m_flag = ENDOF_FILE | ENDOF_FEXP;

  // vector<string> next
  ++m_ifexpr;
  if(m_ifexpr<m_fexpr.size()) {
    m_d.scan(m_fexpr[m_ifexpr]);
    fname = m_d.Next();
    if(m_fexprflag[m_ifexpr]==EasyExp::Exact) fname=m_fexpr[m_ifexpr].c_str();
    if(fname && fname[0]) {
      m_f.open(fname);
      m_flag |= NEW_FILE;
      if(m_key>=0) m_f.read();
      if(m_f.read()) {
        p.set(m_f);
        return(1);
      }
      m_f.close();
      m_flag = ENDOF_FILE;
      return(1); // this is needed if file has EOF at the first line
    }
  }
  // the very end
  m_flag = ENDOF_FILE | ENDOF_FEXP | ENDOF_ALL;
  return(0);
}

/////////////////////////////////////////////////////////////////////////////
int FileStrCollection::isvalid() {
  return(m_f.isvalid());
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
int FileStrCollection::foreach(const char* f
			       ,int x,const string& expr,bool icase) {
  EasyExp regexpr;
  if(x>=0) regexpr.compile(expr.c_str(),m_EasyExpMode);

  rewind();
  Line p;
  int r=0;
  char buf[500];
  if(x>=0) {
    while(read(p)) {
      value v;
      if(p.size()>x) v = p[x];
      if(regexpr.match(v.s.c_str(),icase)) {
	sprintf(buf,"%s((const Line&)%ld)",f,&p);
	r += G__int(G__calc(buf));
      }
    }
  }
  else {
    while(read(p)) {
      sprintf(buf,"%s((const Line&)%ld)",f,&p);
      r += G__int(G__calc(buf));
    }
  }
  return(r);
}
/////////////////////////////////////////////////////////////////////////////
int FileStrCollection::foreach(ForeachFunc& f
			       ,int x,const string& expr,bool icase) {
  EasyExp regexpr;
  if(x>=0) regexpr.compile(expr.c_str(),m_EasyExpMode);
  
  rewind();
  Line p;
  int r=0;
  if(x>=0) {
    while(read(p)) {
      value v;
      if(p.size()>x) v = p[x];
      if(regexpr.match(v.s.c_str(),icase)) {
	r += f(p);
      }
    }
  }
  else {
    while(read(p)) {
      r += f(p);
    }
  }
  return(r);
}


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////




