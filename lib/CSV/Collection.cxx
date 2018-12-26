/////////////////////////////////////////////////////////////////////////////
// Collection.cxx
//
//  CSV table manipulator (on-memory)
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

#include "Collection.h"

#define MAP_UNIQUE_COUNT

#ifndef NOMINMAXCSV
template<class T> static inline T max(const T& a,const T& b) {return(a>b?a:b);}
template<class T> static inline T min(const T& a,const T& b) {return(a<b?a:b);}
#endif

const string g_sep="\f";  // "\r" does not work

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// convert A,B,...,Z,AA,AB,AC...  to integer
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
static Sheet s_sheet;
void ClearSheet() {s_sheet.clear();}
/////////////////////////////////////////////////////////////////////////////
// convert $A,$B,...,$Z,$AA,$AB,$AC...  to integer
int strtonum(const string& s1) {
  if(s1[0]!='$') return(-1);
  string s = s1.substr(1);
  switch(s.size()) {
  case 0: return(0);
  case 1: return(s[0]-'A'); 
  case 2: return((s[0]-'A'+1)*26+s[1]-'A'); 
  default:
    int x=0,i;
    for(i=0;i<(int)s.size();++i) {
      x = (x+1)*26;
      x += s[i]-'A';
    }
    return(x);
  }
}

/////////////////////////////////////////////////////////////////////////////
#ifdef WIN32
#else
// gcc 3.4.4 has problem reading above template function
deque<value>::iterator numtoiter(deque<value>& container,int x) {
  deque<value>::iterator first=container.begin();
  deque<value>::iterator last =container.end(), beforelast;
  int i=0;
  while(i<x && first!=last) {beforelast=first; ++i; ++first;}
  if(first==last) return(beforelast);
  return(first);
}

deque<statistics>::iterator numtoiter(deque<statistics>& container,int x) {
  deque<statistics>::iterator first=container.begin();
  deque<statistics>::iterator last =container.end(), beforelast;
  int i=0;
  while(i<x && first!=last) {beforelast=first; ++i; ++first;}
  if(first==last) return(beforelast);
  return(first);
}

deque<Line*>::iterator numtoiter(deque<Line*>& container,int x) {
  deque<Line*>::iterator first=container.begin();
  deque<Line*>::iterator last =container.end(), beforelast;
  int i=0;
  while(i<x && first!=last) {beforelast=first; ++i; ++first;}
  if(first==last) return(beforelast);
  return(first);
}
#endif

deque<Line*>::iterator linenumtoiter(deque<Line*>& container,int x) {
  deque<Line*>::iterator first=container.begin();
  deque<Line*>::iterator last =container.end();
  while((*first)->linenum()!=x && first!=last) {++first;}
  return(first);
}


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// CollectionStatistics
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void CollectionStatistics::minmax(double& min,double& max) const {
  min=1e99;
  max= -1e99;
  for(deque<statistics>::const_iterator i=m_stat.begin();i!=m_stat.end();++i) {
    if((*i).nsample()>0) {
      if(min>(*i).min()) min = (*i).min();
      if(max<(*i).max()) max = (*i).max();
    }
  }
}
/////////////////////////////////////////////////////////////////////////////
void CollectionStatistics::dispmin(FILE* fp) const {
  for(deque<statistics>::const_iterator i=m_stat.begin();i!=m_stat.end();++i) {
    if((*i).nsample()>0) fprintf(fp,"%g,",(*i).min());
    else fprintf(fp,",");
  }
  fprintf(fp,"min\n");
}
/////////////////////////////////////////////////////////////////////////////
void CollectionStatistics::dispmax(FILE* fp) const {
  for(deque<statistics>::const_iterator i=m_stat.begin();i!=m_stat.end();++i) {
    if((*i).nsample()>0) fprintf(fp,"%g,",(*i).max());
    else fprintf(fp,",");
  }
  fprintf(fp,"max\n");
}
/////////////////////////////////////////////////////////////////////////////
void CollectionStatistics::dispsigma(FILE* fp) const {
  for(deque<statistics>::const_iterator i=m_stat.begin();i!=m_stat.end();++i) {
    if((*i).nsample()>0) fprintf(fp,"%g,",(*i).sigma());
    else fprintf(fp,",");
  }
  fprintf(fp,"sigma\n");
}
/////////////////////////////////////////////////////////////////////////////
void CollectionStatistics::dispstddev(FILE* fp) const {
  for(deque<statistics>::const_iterator i=m_stat.begin();i!=m_stat.end();++i) {
    if((*i).nsample()>0) fprintf(fp,"%g,",(*i).stddev());
    else fprintf(fp,",");
  }
  fprintf(fp,"stddev\n");
}
/////////////////////////////////////////////////////////////////////////////
void CollectionStatistics::dispmean(FILE* fp) const {
  for(deque<statistics>::const_iterator i=m_stat.begin();i!=m_stat.end();++i) {
    if((*i).nsample()>0) fprintf(fp,"%g,",(*i).mean());
    else fprintf(fp,",");
  }
  fprintf(fp,"mean\n");
}
/////////////////////////////////////////////////////////////////////////////
void CollectionStatistics::dispnsample(FILE* fp) const {
  for(deque<statistics>::const_iterator i=m_stat.begin();i!=m_stat.end();++i) {
    fprintf(fp,"%d,",(*i).nsample());
  }
  fprintf(fp,"nsample\n");
}
/////////////////////////////////////////////////////////////////////////////
void CollectionStatistics::disperror(FILE* fp) const {
  for(deque<statistics>::const_iterator i=m_stat.begin();i!=m_stat.end();++i) {
    if((*i).nsample()>0) fprintf(fp,"%d,",(*i).error());
    else fprintf(fp,",");
  }
  fprintf(fp,"error\n");
}
/////////////////////////////////////////////////////////////////////////////
void CollectionStatistics::disp(FILE* fp) const {
  for(deque<statistics>::const_iterator i=m_stat.begin();i!=m_stat.end();++i) {
    (*i).disp(fp);
  }
}
/////////////////////////////////////////////////////////////////////////////
CollectionStatistics CollectionStatistics::column(int from,int to) {
  if(to<0) to=from;
  if(to<from && to==0) to=m_stat.size()-1;
  CollectionStatistics sub;
  deque<statistics>::iterator pos1= numtoiter(m_stat,from);
  deque<statistics>::iterator pos2= numtoiter(m_stat,to);
  if(pos1!=m_stat.end() && pos2!=m_stat.end()) {
    ++pos2;
    while(pos1!=pos2) {
      sub.push_back(*pos1);
      ++pos1;
    }
  }
  return(sub);
}
/////////////////////////////////////////////////////////////////////////////
CollectionStatistics CollectionStatistics::_column(int from,int to) {
  if(to<0) to=from;
  if(to<from && to==0) to=m_stat.size()-1;
  CollectionStatistics sub;
  deque<statistics>::iterator first=m_stat.begin();
  deque<statistics>::iterator pos1= numtoiter(m_stat,from);
  deque<statistics>::iterator pos2= numtoiter(m_stat,to);
  deque<statistics>::iterator last =m_stat.end();
  if(pos1!=m_stat.end() && pos2!=m_stat.end()) {
    ++pos2;
    while(first!=pos1) {
      sub.push_back(*first);
      ++first;
    }
    first=pos2;
    while(first!=last) {
      sub.push_back(*first);
      ++first;
    }
  }
  return(sub);
}
/////////////////////////////////////////////////////////////////////////////
CollectionStatistics CollectionStatistics::column(const vector<int>& p) {
  CollectionStatistics sub;
  vector<int>::const_iterator first = p.begin();
  vector<int>::const_iterator last  = p.end();
  int i;
  statistics empty;
  while(first!=last) {
    i = (*first++);
    if(size()>i) sub.push_back(get(i));
    else         sub.push_back(empty);
  }
  return(sub);
}
/////////////////////////////////////////////////////////////////////////////
CollectionStatistics CollectionStatistics::_column(const vector<int>& p) {
  if(p.size()==0)  return(*this); 
  int lsize = size();
  vector<int> _p;
  int i,j=0;
  for(i=0;i<lsize;i++) {
    if(p[j]==i)  ++j;
     else        _p.push_back(i);
  }
  return(column(_p));
}
/////////////////////////////////////////////////////////////////////////////
int CollectionStatistics::erase(int x) {
  deque<statistics>::iterator pos = numtoiter(m_stat,x);
  if(pos!=m_stat.end()) {
    m_stat.erase(pos);
    return(0);
  }
  else {
    return(1);
  }
}
/////////////////////////////////////////////////////////////////////////////
int CollectionStatistics::erase(int x1,int x2) {
  int stat=1;
  deque<statistics>::iterator pos1 = numtoiter(m_stat,x1);
  deque<statistics>::iterator pos2 = numtoiter(m_stat,x2);
  if(pos1!=m_stat.end() && pos2!=m_stat.end()) {
    ++pos2;
    m_stat.erase(pos1,pos2);
    stat=0;
  }
  return(stat);
}
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// Line
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void Line::stat(CollectionStatistics& s) {
  statistics sx;
  for(int x=0;x<(int)m_column.size();++x) {
    if((int)s.size()<=x) s.push_back(sx);
    if(m_column[x].isdouble()) s[x].add(m_column[x].d);
    //else if(m_column[x].tostring()!="") s[x].add(1.0);
  }
}
/////////////////////////////////////////////////////////////////////////////
value& Line::Get(int x) {
  if(x>=0) {
    while(x>=(int)m_column.size()) m_column.push_back(value());
    return(m_column[x]);
  }
  else {
    //fprintf(stderr,"Warning: Line::get() index %d out of range. size=%d (null returned)\n",x,size());
#ifdef __CINTLIB__
     //G__printlinenum();
#endif
    //if(size()) disp(stderr);
    //exit(255);
    return(m_dummy);
  }
}

/////////////////////////////////////////////////////////////////////////////
Line& Line::let(int x,const vector<double>& v,int offsetmode,int offset) {
  int i,j;
  for(i=0,j=0;(unsigned int)i<v.size();i++) {
    if(abs(i-offset)<offsetmode) {}
    else let((j++)+x,v[i]);
  }
  return(*this);
}

/////////////////////////////////////////////////////////////////////////////
void Line::set(const ReadF& f) {
  int i;
  value val;
  setlinenum(f.line);
  for(i=1;i<=f.argc;++i) {
    val.set(f.argv[i]);
    m_column.push_back(val);
  }
}

/////////////////////////////////////////////////////////////////////////////
void Line::disp(FILE* fp) const {
  deque<value>::const_iterator first= m_column.begin();
  deque<value>::const_iterator last = m_column.end();
  while(first!=last) {
    (*first).disp(fp);
    ++first;
  }
  fprintf(fp,"\n");
}

/////////////////////////////////////////////////////////////////////////////
void Line::push_front(const Line& a) {
  deque<value>::const_reverse_iterator first=a.m_column.rbegin();
  deque<value>::const_reverse_iterator last =a.m_column.rend();
  while(first!=last) {
    m_column.push_front(*first);
    ++first;
  }
}
/////////////////////////////////////////////////////////////////////////////
void Line::push_back(const Line& a) {
  deque<value>::const_iterator first=a.m_column.begin();
  deque<value>::const_iterator last =a.m_column.end();
  while(first!=last) {
    m_column.push_back(*first);
    ++first;
  }
}
/////////////////////////////////////////////////////////////////////////////
int Line::erase(int x) {
  deque<value>::iterator pos = numtoiter(m_column,x);
  if(pos!=m_column.end()) {
    m_column.erase(pos);
    return(0);
  }
  else {
    return(1);
  }
}
/////////////////////////////////////////////////////////////////////////////
int Line::erase(int x1,int x2) {
  int stat=1;
  deque<value>::iterator pos1 = numtoiter(m_column,x1);
  deque<value>::iterator pos2 = numtoiter(m_column,x2);
  if(pos1!=m_column.end() && pos2!=m_column.end()) {
    ++pos2;
    m_column.erase(pos1,pos2);
    stat=0;
  }
  return(stat);
}
/////////////////////////////////////////////////////////////////////////////
Line Line::column(int from,int to) {
  if(to<0) to=from;
  if(to<from && to==0) to=m_column.size()-1;
  Line sub;
  deque<value>::iterator pos1= numtoiter(m_column,from);
  deque<value>::iterator pos2= numtoiter(m_column,to);
  if(pos1!=m_column.end() && pos2!=m_column.end()) {
    ++pos2;
    while(pos1!=pos2) {
      sub.push_back(*pos1);
      ++pos1;
    }
  }
  return(sub);
}

/////////////////////////////////////////////////////////////////////////////
Line Line::_column(int from,int to) {
  if(to<0) to=from;
  if(to<from && to==0) to=m_column.size()-1;
  Line sub;
  deque<value>::iterator first= m_column.begin();
  deque<value>::iterator pos1= numtoiter(m_column,from);
  deque<value>::iterator pos2= numtoiter(m_column,to);
  deque<value>::iterator last = m_column.end();
  if(pos1!=m_column.end() && pos2!=m_column.end()) {
    ++pos2;
    while(first!=pos1) {
      sub.push_back(*first);
      ++first;
    }
    first=pos2;
    while(first!=last) {
      sub.push_back(*first);
      ++first;
    }
  }
  return(sub);
}
/////////////////////////////////////////////////////////////////////////////
Line Line::column(const vector<int>& p) {
  Line sub;
  vector<int>::const_iterator first = p.begin();
  vector<int>::const_iterator last  = p.end();
  int i;
  value empty;
  while(first!=last) {
    i = (*first++);
    if(size()>i) sub.push_back(Get(i));
    else         sub.push_back(empty);
  }
  return(sub);
}
/////////////////////////////////////////////////////////////////////////////
Line Line::_column(const vector<int>& p) {
  if(p.size()==0)  return(*this); 
  int lsize = size();
  vector<int> _p;
  int i,j=0;
  for(i=0;i<lsize;i++) {
    if(p[j]==i)  ++j;
     else        _p.push_back(i);
  }
  return(column(_p));
}

/////////////////////////////////////////////////////////////////////////////
Line& Line::replace(int x,const string& match,const string& rep) {
  if(x<0 || x>=size()) return(*this);
#ifdef E__REPLACE2
  EasyExp r; r.compile_replace(match.c_str(),rep.c_str());
#else
  EasyExpReplace r(match.c_str(),rep.c_str());
#endif
  const char* pin= get(x).c_str();
  string pout;
  r.replace(pin,pout);
  return(let(x,pout));
}
/////////////////////////////////////////////////////////////////////////////
Line& Line::replace(int x,EasyExp& r,const string& rep) {
  if(x<0 || x>=size()) return(*this);
  const char* pin= get(x).c_str();
  string pout;
  r.replace(pin,pout);
  return(let(x,pout));
}
/////////////////////////////////////////////////////////////////////////////
Line& Line::replace(const string& match,const string& rep) {
#ifdef E__REPLACE2
  EasyExp r; r.compile_replace(match.c_str(),rep.c_str());
#else
  EasyExpReplace r(match.c_str(),rep.c_str());
#endif
  for(int x=0;x<size();x++) {
    const char* pin= get(x).c_str();
    string pout;
    r.replace(pin,pout);
    let(x,pout);
  }
  return(*this);
}
/////////////////////////////////////////////////////////////////////////////
Line& Line::replace(EasyExp& r,const string& rep) {
  for(int x=0;x<size();x++) {
    const char* pin= get(x).c_str();
    string pout;
    r.replace(pin,pout);
    let(x,pout);
  }
  return(*this);
}
/////////////////////////////////////////////////////////////////////////////
Line Line::match(const string& key,bool icase) {
  Line r;
  int i;
  EasyExp exp(key.c_str());
  for(i=0;(unsigned int)i<m_column.size();i++) {
    if(exp.match(m_column[i].c_str(),icase)) {
      r.push_back(m_column[i].c_str());
    }
  }
  return(r);
}

/////////////////////////////////////////////////////////////////////////////
int Line::empty() {
  if(0==size()) return(1);
  deque<value>::iterator first = m_column.begin();
  deque<value>::iterator last  = m_column.end();
  value v;
  while(first!=last) {
    v = *first++;
    if(v.s!="") return(0);
  }
  return(1);
}

/////////////////////////////////////////////////////////////////////////////
vector<int> Line::emptycolumns() {
  vector<int> xs;
  value v;
  for(int i=0;i<size();i++) {
    v = m_column[i];
    if(v.s=="") xs.push_back(i);
  }
  return(xs);
}

/////////////////////////////////////////////////////////////////////////////
vector<int> Line::_emptycolumns() {
  vector<int> xs;
  value v;
  for(int i=0;i<size();i++) {
    v = m_column[i];
    if(v.s!="") xs.push_back(i);
  }
  return(xs);
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
Line& Line::add(const Line& a) {
  int i;
  for(i=0;i<size()&&i<a.size();i++) {
    if(get(i).isdouble() && a[i].isdouble()) 
      get(i).set(get(i).todouble()+a.get(i).todouble());
  }
  return(*this);
}
/////////////////////////////////////////////////////////////////////////////
Line& Line::sub(const Line& a) {
  int i;
  for(i=0;i<size()&&i<a.size();i++) {
    if(get(i).isdouble() && a[i].isdouble()) 
      get(i).set(get(i).todouble()-a.get(i).todouble());
  }
  return(*this);
}
/////////////////////////////////////////////////////////////////////////////
Line& Line::mul(const Line& a) {
  int i;
  for(i=0;i<size()&&i<a.size();i++) {
    if(get(i).isdouble() && a[i].isdouble()) 
      get(i).set(get(i).todouble()*a.get(i).todouble());
  }
  return(*this);
}
/////////////////////////////////////////////////////////////////////////////
Line& Line::div(const Line& a) {
  int i;
  for(i=0;i<size()&&i<a.size();i++) {
    if(get(i).isdouble() && a[i].isdouble()) 
      get(i).set(get(i).todouble()/a.get(i).todouble());
  }
  return(*this);
}
/////////////////////////////////////////////////////////////////////////////
Line& Line::mod(const Line& a) {
  int i;
  for(i=0;i<size()&&i<a.size();i++) {
    if(get(i).isdouble() && a[i].isdouble()) 
      get(i).set((int)get(i).todouble()/(int)a.get(i).todouble());
  }
  return(*this);
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
Line& Line::hyperlink(int x) {
  if(x<size()) {
    string s = "=hyperlink(\"";
    s += get(x).tostring();
    s += "\")";
    let(x,s);
  }
  return(*this);
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
#ifdef COLLECTIONCOMPARE
/////////////////////////////////////////////////////////////////////////////
bool operator==(const Line& a,const Line& b)  {
  if(a.size()!=b.size()) return(false);
  int n = ::min(a.size(),b.size());
  for(int i=0;i<n;i++) {
    if(a[i]!=b[i]) return(false);
  }
  return(true);
}
/////////////////////////////////////////////////////////////////////////////
bool operator<(const Line& a,const Line& b)  {return(&a<&b);}
/////////////////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////////////////
int compare(const Line& a,const Line& b,int x) {
  if(a[x].tostring()==b[x].tostring()) return(0);
  else if(a[x].tostring()<b[x].tostring()) return(1);
  else return(-1);
}
/////////////////////////////////////////////////////////////////////////////
int compare(const Line& a,const Line& b,const int* x,int n) {
  int cmp= compare(a,b,x[0]);
  if(cmp!=0 || n==1) return(cmp);
  return(compare(a,b,x+1,n-1));
}
////////////////////////////////////////////////////////////////////////
int alignCollections(Collection& a,Collection& b
		     ,Collection& aout,Collection& bout
		     ,int xval,const vector<int> xalign,const char* mode){
printf("alignCollection A\n");
  enum mode_t { AVE, MIN, MAX, STDDEV, NSAMPLE  };
  int md;
  if(strncmp(mode,"AVE",3)==0 || strncmp(mode,"ave",3)==0) md=AVE;
  else if(strncmp(mode,"MEAN",3)==0 || strncmp(mode,"mean",3)==0) md=AVE;
  else if(strncmp(mode,"MIN",3)==0 || strncmp(mode,"min",3)==0) md=MIN;
  else if(strncmp(mode,"MAX",3)==0 || strncmp(mode,"max",3)==0) md=MAX;
  else if(strncmp(mode,"STDDEV",3)==0 || strncmp(mode,"stddev",3)==0) md=STDDEV;
  else if(strncmp(mode,"NSAMPLE",3)==0 || strncmp(mode,"nsample",3)==0) md=NSAMPLE;
  else md=AVE;
printf("alignCollection B\n");

printf("Sorting\n");
  a.multisort(xalign,CompareLine::String);
  b.multisort(xalign,CompareLine::String);
  int i=0, j=0, i0,j0;
  Collection atmp,btmp;
  Line aline,bline;
  double aval,bval;
  aout.clear(); bout.clear();

  while(i<a.size() || j<b.size()) {
    while(i<a.size() && compare(a[i],b[j],xalign)>0) i++;
    while(j<b.size() && compare(b[j],a[i],xalign)<0) j++;
    atmp.clear(); btmp.clear();
    i0=i; j0=j;
    while(i<a.size() && compare(a[i],b[j0],xalign)==0) atmp.push_back(a[i++]);
    while(j<b.size() && compare(a[i0],b[j],xalign)==0) btmp.push_back(b[j++]);
printf("atmpsize=%d btmpsize=%d\n",atmp.size(),btmp.size());
    if(atmp.size() && btmp.size()) {
      aline = atmp[0]; bline = btmp[0];
      switch(md) {
      case AVE:aval=atmp.mean(xval,0,xval);bval=btmp.mean(xval,0,xval);break;
      case MIN:aval=atmp.min(xval,0,xval);bval=btmp.min(xval,0,xval);break;
      case MAX:aval=atmp.max(xval,0,xval);bval=btmp.max(xval,0,xval);break;
      case STDDEV:aval=atmp.stddev(xval,0,xval);bval=btmp.stddev(xval,0,xval);
	break;
      case NSAMPLE:
	aval=atmp.nsample(xval,0,xval);bval=btmp.nsample(xval,0,xval);break;
      default:  
	aval=atmp.mean(xval,0,xval); bval=btmp.mean(xval,0,xval); break;
      }
      aline.let(xval,aval); bline.let(xval,bval);
aline.disp();
bline.disp();
      aout.push_back(aline); bout.push_back(bline);
    }
  }
  return(0);
}
////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
#ifdef MATHOPERATOR
/////////////////////////////////////////////////////////////////////////////
Line operator+(const Line& a,const Line& b)  {
  int i;
  Line line;
  string buf;
  for(i=0;i<a.size()&&i<b.size();i++) {
    if(a[i].isdouble() && b[i].isdouble()) 
      line.push_back(a[i].todouble()+b[i].todouble());
    else {
      buf = a[i].tostring();
      buf += b[i].tostring();
      line.push_back(buf);
    }
  }
  return(line);
}
/////////////////////////////////////////////////////////////////////////////
Line operator-(const Line& a,const Line& b)  {
  int i;
  Line line;
  string buf;
  for(i=0;i<a.size()&&i<b.size();i++) {
    if(a[i].isdouble() && b[i].isdouble()) 
      line.push_back(a[i].todouble()-b[i].todouble());
    else {
      buf = a[i].tostring();
      buf += b[i].tostring();
      line.push_back(buf);
    }
  }
  return(line);
}
/////////////////////////////////////////////////////////////////////////////
Line operator*(const Line& a,const Line& b)  {
  int i;
  Line line;
  string buf;
  for(i=0;i<a.size()&&i<b.size();i++) {
    if(a[i].isdouble() && b[i].isdouble()) 
      line.push_back(a[i].todouble()*b[i].todouble());
    else {
      buf = a[i].tostring();
      buf += b[i].tostring();
      line.push_back(buf);
    }
  }
  return(line);
}
/////////////////////////////////////////////////////////////////////////////
Line operator/(const Line& a,const Line& b)  {
  int i;
  Line line;
  string buf;
  for(i=0;i<a.size()&&i<b.size();i++) {
    if(a[i].isdouble() && b[i].isdouble()) 
      line.push_back(a[i].todouble()/b[i].todouble());
    else {
      buf = a[i].tostring();
      buf += b[i].tostring();
      line.push_back(buf);
    }
  }
  return(line);
}
/////////////////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// CompareLine
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
bool CompareLine::operator()(const Line* a,const Line *b ) {
  if(m_mode==Auto)        return(a->getconst(m_x)  >b->getconst(m_x)  );
  else if(m_mode==String) return(a->getconst(m_x).s>b->getconst(m_x).s);
  else if(m_mode==Number) return(a->getconst(m_x).d>b->getconst(m_x).d);
  else                    return(a->linenum()>b->linenum());
}
/////////////////////////////////////////////////////////////////////////////
bool CompareLine::operator()(const Line& a,const Line& b ) {
  if(m_mode==Auto)        return(a.getconst(m_x)  >b.getconst(m_x)  );
  else if(m_mode==String) return(a.getconst(m_x).s>b.getconst(m_x).s);
  else if(m_mode==Number) return(a.getconst(m_x).d>b.getconst(m_x).d);
  else                    return(a.linenum()>b.linenum());
}
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// Collection
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
Collection::Collection(const Collection& col) {
  p_key=col.p_key;
  p_key2=col.p_key2;
  m_plines=col.m_plines;
  m_stat=col.m_stat;
  p_sheet=col.p_sheet;
  //m_isauto=col.m_isauto;
  m_isauto=0;
  //if(m_isauto && p_sheet) delete p_sheet;
  m_EasyExpMode = col.m_EasyExpMode;
}
/////////////////////////////////////////////////////////////////////////////
Collection::~Collection() {if(m_isauto && p_sheet) delete p_sheet;}

/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
void Collection::setsheet(Sheet* sheet) { 
  if(m_isauto && p_sheet) {
    sheet->m_lines.insert(sheet->m_lines.end()
                          ,p_sheet->m_lines.begin(),p_sheet->m_lines.end());
    delete p_sheet;
  }
  p_sheet=sheet; 
  m_isauto=0; 
}
/////////////////////////////////////////////////////////////////////////////
void Collection::init() { 
  //if(m_isauto && p_sheet) delete p_sheet;
  m_isauto=0;
  p_sheet=0;
  p_key=0; 
  p_key2=0; 
  m_EasyExpMode = EasyExp::Auto;
  initstat();
  m_plines.clear();
}
/////////////////////////////////////////////////////////////////////////////
void Collection::push_front(const Line& line) { 
  if(!p_sheet) {
    p_sheet = &s_sheet;
    m_isauto=0;
  }
  p_sheet->push_back(line);
  m_plines.push_front(&p_sheet->back()); 
}
/////////////////////////////////////////////////////////////////////////////
void Collection::push_back(const Line& line) { 
  if(!p_sheet) {
    p_sheet = &s_sheet;
    m_isauto=0;
  }
  p_sheet->push_back(line);
  m_plines.push_back(&p_sheet->back()); 
}
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// get column number
int Collection::getxexact(const string& key) {
  int x;
  if(p_key) {
    for(x=0;x<p_key->size();++x) {
      const value& v = (*p_key)[x];
      if(v.s==key) {
        return(x);
      }
    }
  }
#if 0
  fprintf(stderr,"Warning: Collection::getxexact() key %s not found\n",key.c_str());
#endif
  return(-1);
}

/////////////////////////////////////////////////////////////////////////////
// get column number
int Collection::getx(const string& key,bool icase) {
  int x;
  if(p_key) {
    EasyExp regexpr(key.c_str(),m_EasyExpMode);
    for(x=0;x<p_key->size();++x) {
      const value& v = (*p_key)[x];
      if(regexpr.match(v.s.c_str(),icase)) {
        return(x);
      }
    }
  }
  if(key.size()==1 || key.size()==2) {
    x=strtonum(key);
    if(x>0) return(x);
  }
#if 0
  if(key!="###") {
#ifdef __CINTLIB__
    G__printlinenum();
#endif
    fprintf(stderr,"Warning: Collection::getx() key %s not found\n",key.c_str());
  }
#endif
  return(-1);
}

/////////////////////////////////////////////////////////////////////////////
// get column numbers
vector<int> Collection::getxs(const string& key,bool icase) {
  int x;
  vector<int> xs;
  if(p_key) {
    EasyExp regexpr(key.c_str(),m_EasyExpMode);
    for(x=0;x<p_key->size();++x) {
      const value& v = (*p_key)[x];
      if(regexpr.match(v.s.c_str(),icase)) {
        xs.push_back(x);
      }
    }
  }
  if(xs.size()) return(xs);
  if(key.size()==1 || key.size()==2) {
    x=strtonum(key);
    if(x>0) {
      xs.push_back(x);
      return(xs);
    }
  }
#if 0
#ifdef __CINTLIB__
  G__printlinenum();
#endif
  fprintf(stderr,"Warning: Collection::getxs() key %s not found\n",key.c_str());
#endif
  return(xs);
}
/////////////////////////////////////////////////////////////////////////////
// get column numbers
int Collection::getxs(vector<int>& xs,const string& key,bool icase) {
  xs.clear();
  int x;
  if(p_key) {
    EasyExp regexpr(key.c_str(),m_EasyExpMode);
    for(x=0;x<p_key->size();++x) {
      const value& v = (*p_key)[x];
      if(regexpr.match(v.s.c_str(),icase)) {
        xs.push_back(x);
      }
    }
  }
  if(xs.size()) return(xs.size());
  if(key.size()==1 || key.size()==2) {
    x=strtonum(key);
    if(x>0) {
      xs.push_back(x);
      return(xs.size());
    }
  }
#if 0
#ifdef __CINTLIB__
  G__printlinenum();
#endif
  fprintf(stderr,"Warning: Collection::getxs() key %s not found\n",key.c_str());
#endif
  return(xs.size());
}

/////////////////////////////////////////////////////////////////////////////
// get column number 2
int Collection::getx2(const string& key,int xoffset,bool icase) {
  int x;
  if(p_key2) {
    EasyExp regexpr(key.c_str(),m_EasyExpMode);
    for(x=xoffset;x<p_key2->size();++x) {
      const value& v = (*p_key2)[x];
      if(regexpr.match(v.s.c_str(),icase)) {
        return(x);
      }
    }
  }
  if(key.size()==1 || key.size()==2) {
    x=strtonum(key);
    if(x>0) return(x);
  }
#if 0
  fprintf(stderr,"Warning: key %s not found\n",key.c_str());
#ifdef __CINTLIB__
  G__printlinenum();
#endif
#endif
  return(-1);
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// collection functions
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
Collection  Collection::match_cell(const string& expr,bool icase
				   ,bool surround) const {
  Collection sub; sub.setsheet(p_sheet);
  Line line;
  // set key line
  line.let(0,"value"); line.let(1,"row"); line.let(2,"column");
  if(surround) {
    line.let(3,"v(row+1)"); line.let(4,"v(col+1)"); 
    line.let(5,"v(row-1)"); line.let(6,"v(col-1)");
  }
  sub.push_back(line); sub.setkey(0);

  // scan table
  EasyExp regexpr(expr.c_str(),m_EasyExpMode);
  for(int i=0;i<size();i++) {
    const Line& p = get(i);
    for(int j=0;j<p.size();j++) {
      const value& v = p[j];
      if(regexpr.match(v.s.c_str(),icase)) {
	line.let(0,v); line.let(1,i); line.let(2,j);
	if(surround) {
	  line.let(3,get(i+1)[j]); line.let(4,get(i)[j+1]);
	  line.let(5,get(i-1)[j]); line.let(6,get(i)[j-1]);
	}
	sub.push_back(line);
      }
    }
  }
  return(sub);
}
/////////////////////////////////////////////////////////////////////////////
Collection  Collection::_match_cell(const string& expr,bool icase) const {
  Collection sub; sub.setsheet(p_sheet);
  Line line;
  // set key line
  line.let(0,"value"); line.let(1,"row"); line.let(2,"column");
  sub.push_back(line); sub.setkey(0);

  // scan table
  EasyExp regexpr(expr.c_str(),m_EasyExpMode);
  for(int i=0;i<size();i++) {
    const Line& p = get(i);
    for(int j=0;j<p.size();j++) {
      const value& v = p[j];
      if(!regexpr.match(v.s.c_str(),icase)) {
	line.let(0,v); line.let(1,i); line.let(2,j);
	sub.push_back(line);
      }
    }
  }
  return(sub);
}
/////////////////////////////////////////////////////////////////////////////
map<string,int> Collection::unique_cell_map() const {
  map<string,int> count;
  // scan table
  for(int i=0;i<size();i++) {
    const Line& p = get(i);
    for(int j=0;j<p.size();j++) {
      const value& v = p[j];
      ++count[v.tostring()];
    }
  }
  return(count);
}

/////////////////////////////////////////////////////////////////////////////
Collection Collection::unique_cell_count() const {
  Collection sub; sub.setsheet(p_sheet);
  Line line;
  // set key line
  line.let(0,"value"); line.let(1,"count");
  sub.push_back(line); sub.setkey(0);

  map<string,int> count = unique_cell_map();

  map<string,int>::iterator first,last=count.end();
  for(first=count.begin();first!=last;++first) {
    line.let(0,(*first).first);
    line.let(1,(*first).second);
    sub.push_back(line);
  }
  return(sub);
}
/////////////////////////////////////////////////////////////////////////////
int Collection::strcell2numcell() {
  EasyExp regexpr("[-+0-9.,:2-20][ :1-3]");
  int i,j,k,count=0 /* ,iv */;
  string tmp;
  //double d;
  for(i=0;i<size();i++) {
    Line& p = get(i);
    for(j=0;j<p.size();j++) {
      value& v = p[j];
      if(regexpr.match(v.s.c_str())) {
	tmp=""; 
	for(k=0;(unsigned int)k<v.s.size();k++) {
	  if(v.s[k]!=',' && v.s[k]!=' ') tmp += v.s[k];
	}
	p.let(j,tmp);
	++count;
      }
    }
  }
  return(count);
}

/////////////////////////////////////////////////////////////////////////////
Collection  Collection::match(const string& key,const string& expr,bool icase) {
#ifdef MULTIPLEKEYS
  vector<int> xs=getxs(key);
  if(xs.size()==0) {
    fprintf(stderr,"Error: match(%s,%s) key not found\n"
	    ,key.c_str(),expr.c_str());
#ifdef __CINTLIB__
    G__printlinenum();
#endif
    return(Collection(p_sheet));
  }
  else if(xs.size()==1) return(match(xs[0],expr,icase));
  else return(match(xs,expr,icase));
#else // MULTIPLEKEYS
  int x=getx(key);
  if(x<0) {
    fprintf(stderr,"Error: match(%s,%s) key not found\n"
	    ,key.c_str(),expr.c_str());
#ifdef __CINTLIB__
    G__printlinenum();
#endif
    return(Collection(p_sheet));
  }
  return(match(x,expr,icase));
#endif // MULTIPLEKEYS
}
/////////////////////////////////////////////////////////////////////////////
Collection Collection::_match(const string& key,const string& expr,bool icase) {
#ifdef MULTIPLEKEYS
  vector<int> xs=getxs(key);
  if(xs.size()==0) {
    fprintf(stderr,"Error: _match(%s,%s) key not found\n"
	    ,key.c_str(),expr.c_str());
#ifdef __CINTLIB__
    G__printlinenum();
#endif
    return(Collection(p_sheet));
  }
  else if(xs.size()==1) return(_match(xs[0],expr,icase));
  else return(_match(xs,expr,icase));
#else // MULTIPLEKEYS
  int x=getx(key);
  if(x<0) {
    fprintf(stderr,"Error: _match(%s,%s) key not found\n"
	    ,key.c_str(),expr.c_str());
#ifdef __CINTLIB__
    G__printlinenum();
#endif
    return(Collection(p_sheet));
  }
  return(_match(x,expr,icase));
#endif // MULTIPLEKEYS
}
/////////////////////////////////////////////////////////////////////////////
Collection  Collection::match(const string& key,Collection& val,const string& keyv,bool icase) {
#ifdef MULTIPLEKEYS
  vector<int> xs=getxs(key);
  int xv=val.getx(keyv);
  if(xs.size()==0 || xv<0) {
    fprintf(stderr,"Error: match(%s,Col,%s) key not found\n"
	    ,key.c_str(),keyv.c_str());
#ifdef __CINTLIB__
    G__printlinenum();
#endif
    return(Collection(p_sheet));
  }
  else if(xs.size()==1) return(match(xs[0],val,xv,icase));
  else return(match(xs,val,xv,icase));
#else // MULTIPLEKEYS
  int x=getx(key);
  int xv=val.getx(keyv);
  if(x<0 || xv<0) {
    fprintf(stderr,"Error: match(%s,Col,%s) key not found\n"
	    ,key.c_str(),keyv.c_str());
#ifdef __CINTLIB__
    G__printlinenum();
#endif
    return(Collection(p_sheet));
  }
  return(match(x,val,xv,icase));
#endif // MULTIPLEKEYS
}
/////////////////////////////////////////////////////////////////////////////
Collection  Collection::_match(const string& key,Collection& val,const string& keyv,bool icase) {
#ifdef MULTIPLEKEYS
  vector<int> xs=getxs(key);
  int xv=val.getx(keyv);
  if(xs.size()==0 || xv<0) {
    fprintf(stderr,"Error: _match(%s,Col,%s) key not found\n"
	    ,key.c_str(),keyv.c_str());
#ifdef __CINTLIB__
    G__printlinenum();
#endif
    return(Collection(p_sheet));
  }
  else if(xs.size()==1) return(_match(xs[0],val,xv,icase));
  else return(_match(xs,val,xv,icase));
#else
  int x=getx(key);
  int xv=val.getx(keyv);
  if(x<0 || xv<0) {
    fprintf(stderr,"Error: _match(%s,Col,%s) key not found\n"
	    ,key.c_str(),keyv.c_str());
#ifdef __CINTLIB__
    G__printlinenum();
#endif
    return(Collection(p_sheet));
  }
  return(_match(x,val,xv,icase));
#endif
}
/////////////////////////////////////////////////////////////////////////////
Collection  Collection::range(const string& key,double lower,double upper) {
#ifdef MULTIPLEKEYS
  vector<int> xs=getxs(key);

  if(xs.size()==0) {
    fprintf(stderr,"Error: range(%s,%g,%g) key not found\n"
	    ,key.c_str(),lower,upper);
#ifdef __CINTLIB__
    G__printlinenum();
#endif
    return(Collection(p_sheet));
  }
  else if(xs.size()==1) return(range(xs[0],lower,upper));
  else return(range(xs,lower,upper));
#else // MULTIPLEKEYS
  int x=getx(key);
  if(x<0) {
    fprintf(stderr,"Error: range(%s,%g,%g) key not found\n"
	    ,key.c_str(),lower,upper);
#ifdef __CINTLIB__
    G__printlinenum();
#endif
    return(Collection(p_sheet));
  }
  return(range(x,lower,upper));
#endif // MULTIPLEKEYS
}
/////////////////////////////////////////////////////////////////////////////
Collection Collection::_range(const string& key,double lower,double upper) {
#ifdef MULTIPLEKEYS
  vector<int> xs=getxs(key);
  if(xs.size()==0) {
    fprintf(stderr,"Error: _range(%s,%g,%g) key not found\n"
	    ,key.c_str(),lower,upper);
#ifdef __CINTLIB__
    G__printlinenum();
#endif
    return(Collection(p_sheet));
  }
  else if(xs.size()==1) return(_range(xs[0],lower,upper));
  else return(_range(xs,lower,upper));
#else // MULTIPLEKES
  int x=getx(key);
  if(x<0) {
    fprintf(stderr,"Error: _range(%s,%g,%g) key not found\n"
	    ,key.c_str(),lower,upper);
#ifdef __CINTLIB__
    G__printlinenum();
#endif
    return(Collection(p_sheet));
  }
  return(_range(x,lower,upper));
#endif // MULTIPLEKEYS
}

/////////////////////////////////////////////////////////////////////////////
Collection  Collection::match(int x,const string& expr,bool icase) {
  EasyExp regexpr(expr.c_str(),m_EasyExpMode);
  Collection sub;
  sub.setsheet(p_sheet);
  sub.p_key=p_key;
  sub.m_stat = m_stat;
  for(const_iterator iy=m_plines.begin();iy!=m_plines.end();++iy) {
    Line& p = *(*iy);
    value v;
    if(p.size()>x) v = p[x];
    if(regexpr.match(v.s.c_str(),icase)) sub.push_back(&p);
  }
  return(sub);
}

/////////////////////////////////////////////////////////////////////////////
Collection Collection::_match(int x,const string& expr,bool icase) {
  EasyExp regexpr(expr.c_str(),m_EasyExpMode);
  Collection sub;
  sub.setmatchmode(m_EasyExpMode);
  sub.setsheet(p_sheet);
  sub.p_key=p_key;
  sub.m_stat = m_stat;
  for(const_iterator iy=m_plines.begin();iy!=m_plines.end();++iy) {
    Line& p = *(*iy);
    value v;
    if(p.size()>x) v = p[x];
    if(!regexpr.match(v.s.c_str(),icase)) sub.push_back(&p);
  }
  return(sub);
}
/////////////////////////////////////////////////////////////////////////////
Collection Collection::match(int x,Collection& val,int xval,bool icase) {
  string expr;
  Collection sub;
  sub.setmatchmode(m_EasyExpMode);
  for(int i=0;i<val.size();i++) {
    expr = val[i][xval].tostring();
    if(sub.size()==0) sub = match(x,expr,icase); 
    else              sub.merge(match(x,expr,icase));
  }
  return(sub);
}
/////////////////////////////////////////////////////////////////////////////
Collection Collection::_match(int x,Collection& val,int xval,bool icase) {
  string expr;
  Collection sub = *this;
  for(int i=0;i<val.size();i++) {
    expr = val[i][xval].tostring();
    sub = sub._match(x,expr,icase); 
  }
  return(sub);
}

/////////////////////////////////////////////////////////////////////////////
Collection  Collection::range(int x,double lower,double upper) {
  Collection sub;
  sub.setsheet(p_sheet);
  sub.p_key=p_key;
  sub.m_stat = m_stat;
  for(const_iterator iy=m_plines.begin();iy!=m_plines.end();++iy) {
    Line& p = *(*iy);
    if(p.size()>x) {
      const value& v = p[x];
      if(lower<=v.d && v.d<=upper) sub.push_back(&p);
    }
  }
  return(sub);
}

/////////////////////////////////////////////////////////////////////////////
Collection Collection::_range(int x,double lower,double upper) {
  Collection sub;
  sub.setsheet(p_sheet);
  sub.p_key=p_key;
  sub.m_stat = m_stat;
  for(const_iterator iy=m_plines.begin();iy!=m_plines.end();++iy) {
    Line& p = *(*iy);
    if(p.size()>x) {
      const value& v = p[x];
      if(lower>v.d || v.d>upper) sub.push_back(&p);
    }
  }
  return(sub);
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
#ifdef MULTIPLEKEYS
/////////////////////////////////////////////////////////////////////////////
Collection  Collection::match(const vector<int>& xs,const string& expr,bool icase) {
  EasyExp regexpr(expr.c_str(),m_EasyExpMode);
  Collection sub;
  sub.setsheet(p_sheet);
  sub.p_key=p_key;
  sub.m_stat = m_stat;
  for(const_iterator iy=m_plines.begin();iy!=m_plines.end();++iy) {
    Line& p = *(*iy);
    vector<int>::const_iterator ifirst=xs.begin();
    vector<int>::const_iterator ilast=xs.end();
    while(ifirst!=ilast) {
      int x = *ifirst++;
      if(p.size()>x) {
        const value& v = p[x];
        if(regexpr.match(v.s.c_str(),icase)) {
	  sub.push_back(&p);
	  break;
        }
      }
    }
  }
  return(sub);
}

/////////////////////////////////////////////////////////////////////////////
Collection Collection::match(const vector<int>& xs,Collection& val,int xval,bool icase) {
  string expr;
  Collection sub;
  for(int i=0;i<val.size();i++) {
    expr = val[i][xval].tostring();
    if(sub.size()==0) sub = match(xs,expr,icase); 
    else              sub.merge(match(xs,expr,icase));
  }
  return(sub);
}
/////////////////////////////////////////////////////////////////////////////
Collection Collection::_match(const vector<int>& xs,Collection& val,int xval,bool icase) {
  string expr;
  Collection sub = *this;
  for(int i=0;i<val.size();i++) {
    expr = val[i][xval].tostring();
    sub = sub._match(xs,expr,icase); 
  }
  return(sub);
}

/////////////////////////////////////////////////////////////////////////////
Collection Collection::_match(const vector<int>& xs,const string& expr,bool icase) {
  EasyExp regexpr(expr.c_str(),m_EasyExpMode);
  Collection sub;
  sub.setsheet(p_sheet);
  sub.p_key=p_key;
  sub.m_stat = m_stat;
  for(const_iterator iy=m_plines.begin();iy!=m_plines.end();++iy) {
    Line& p = *(*iy);
    vector<int>::const_iterator ifirst=xs.begin();
    vector<int>::const_iterator ilast=xs.end();
    while(ifirst!=ilast) {
      int x = *ifirst++;
      if(p.size()>x) {
        const value& v = p[x];
        if(!regexpr.match(v.s.c_str(),icase)) { 
	  sub.push_back(&p);
          break;
        }
      }
    }
  }
  return(sub);
}
/////////////////////////////////////////////////////////////////////////////
Collection  Collection::range(const vector<int>& xs,double lower,double upper) {
  Collection sub;
  sub.setsheet(p_sheet);
  sub.p_key=p_key;
  sub.m_stat = m_stat;
  for(const_iterator iy=m_plines.begin();iy!=m_plines.end();++iy) {
    Line& p = *(*iy);
    vector<int>::const_iterator ifirst=xs.begin();
    vector<int>::const_iterator ilast=xs.end();
    while(ifirst!=ilast) {
      int x = *ifirst++;
      if(p.size()>x) {
        const value& v = p[x];
        if(lower<=v.d && v.d<=upper) { 
	  sub.push_back(&p);
          break;
        }
      }
    }
  }
  return(sub);
}

/////////////////////////////////////////////////////////////////////////////
Collection Collection::_range(const vector<int>& xs,double lower,double upper) {
  Collection sub;
  sub.setsheet(p_sheet);
  sub.p_key=p_key;
  sub.m_stat = m_stat;
  for(const_iterator iy=m_plines.begin();iy!=m_plines.end();++iy) {
    Line& p = *(*iy);
    vector<int>::const_iterator ifirst=xs.begin();
    vector<int>::const_iterator ilast=xs.end();
    while(ifirst!=ilast) {
      int x = *ifirst++;
      if(p.size()>x) {
        const value& v = p[x];
        if(lower>v.d || v.d>=upper) {
	  sub.push_back(&p);
          break;
        }
      }
    }
  }
  return(sub);
}
#endif // MULTIPLEKEYS
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
Collection  Collection::line(int from,int to) const {
  if(to<from) to=from;
  Collection sub;
  sub.setsheet(p_sheet);
  sub.p_key=p_key;
  sub.m_stat = m_stat;
  int first = ::max(0,from);
  int last  = ::min(to+1,(int)m_plines.size());
  for(int i=first;i<last;++i) sub.push_back(m_plines[i]);
  return(sub);
}
/////////////////////////////////////////////////////////////////////////////
Collection Collection::_line(int from,int to) {
  if(to<from) to=from;
  Collection sub;
  sub.setsheet(p_sheet);
  sub.p_key=p_key;
  sub.m_stat = m_stat;
  int first = ::max(0,from);
  int last  = ::min(to+1,(int)m_plines.size());
  int i;
  for(i=0;i<first;++i) sub.push_back(m_plines[i]);
  for(i=last;i<(int)m_plines.size();++i) sub.push_back(m_plines[i]);
  return(sub);
}
/////////////////////////////////////////////////////////////////////////////
Collection  Collection::empty(const string& key) {
  int x=getx(key);
  if(x<0) {
    fprintf(stderr,"Error: empty(%s) key not found\n"
	    ,key.c_str());
#ifdef __CINTLIB__
    G__printlinenum();
#endif
    return(Collection(p_sheet));
  }
  return(empty(x));
}
/////////////////////////////////////////////////////////////////////////////
Collection Collection::_empty(const string& key) {
  int x=getx(key);
  if(x<0) {
    fprintf(stderr,"Error: _empty(%s key not found\n"
	    ,key.c_str());
#ifdef __CINTLIB__
    G__printlinenum();
#endif
    return(Collection(p_sheet));
  }
  return(_empty(x));
}
/////////////////////////////////////////////////////////////////////////////
Collection  Collection::empty(int x) {
  Collection sub;
  sub.setsheet(p_sheet);
  sub.p_key=p_key;
  sub.m_stat = m_stat;
  for(const_iterator iy=m_plines.begin();iy!=m_plines.end();++iy) {
    Line& p = *(*iy);
    if(p.size()<=x) {
      sub.push_back(&p);
      continue;
    }
    const value& v = p[x];
    if(v.s.size()==0) sub.push_back(&p);
  }
  return(sub);
}

/////////////////////////////////////////////////////////////////////////////
Collection Collection::_empty(int x) {
  Collection sub;
  sub.setsheet(p_sheet);
  sub.p_key=p_key;
  sub.m_stat = m_stat;
  for(const_iterator iy=m_plines.begin();iy!=m_plines.end();++iy) {
    Line& p = *(*iy);
    if(p.size()<=x) continue;
    const value& v = p[x];
    if(v.s.size()) sub.push_back(&p);
  }
  return(sub);
}

/////////////////////////////////////////////////////////////////////////////
Collection  Collection::empty() {
  Collection sub;
  sub.setsheet(p_sheet);
  sub.p_key=p_key;
  sub.m_stat = m_stat;
  for(const_iterator iy=m_plines.begin();iy!=m_plines.end();++iy) {
    Line& p = *(*iy);
    if(p.empty()) sub.push_back(p);
  }
  return(sub);
}

/////////////////////////////////////////////////////////////////////////////
Collection Collection::_empty() {
  Collection sub;
  sub.setsheet(p_sheet);
  sub.p_key=p_key;
  sub.m_stat = m_stat;
  for(const_iterator iy=m_plines.begin();iy!=m_plines.end();++iy) {
    Line& p = *(*iy);
    if(p._empty()) sub.push_back(p);
  }
  return(sub);
}

/////////////////////////////////////////////////////////////////////////////
Collection  Collection::emptycolumns(int y) {
  Collection sub;
  vector<int> xs;
  map<int,int> xsall;
  Line& p = line(y)[0];
  xs = p.emptycolumns();
  for(unsigned int j=0;j<xs.size();j++) {
    ++xsall[xs[j]];
  }
  xs.clear();
  map<int,int>::iterator first = xsall.begin(); 
  map<int,int>::iterator last  = xsall.end(); 
  while(first!=last) {
    xs.push_back((*first).first);
    ++first;
  }
  sub = column(xs);
  return(sub);
}

/////////////////////////////////////////////////////////////////////////////
Collection Collection::_emptycolumns(int y) {
  Collection sub;
  vector<int> xs;
  map<int,int> xsall;
  Line& p = line(y)[0];
  xs = p._emptycolumns();
  for(unsigned int j=0;j<xs.size();j++) {
    ++xsall[xs[j]];
  }
  xs.clear();
  map<int,int>::iterator first = xsall.begin(); 
  map<int,int>::iterator last  = xsall.end(); 
  while(first!=last) {
    xs.push_back((*first).first);
    ++first;
  }
  sub = column(xs);
  return(sub);
}

/////////////////////////////////////////////////////////////////////////////
Collection  Collection::emptycolumns() {
  Collection sub;
  vector<int> xs;
  map<int,int> xsall;
  for(const_iterator iy=m_plines.begin();iy!=m_plines.end();++iy) {
    Line& p = *(*iy);
    xs = p.emptycolumns();
    for(unsigned int j=0;j<xs.size();j++) {
      ++xsall[xs[j]];
    }
  }
  xs.clear();
  map<int,int>::iterator first = xsall.begin(); 
  map<int,int>::iterator last  = xsall.end(); 
  while(first!=last) {
    xs.push_back((*first).first);
    ++first;
  }
  sub = column(xs);
  return(sub);
}

/////////////////////////////////////////////////////////////////////////////
Collection Collection::_emptycolumns() {
  Collection sub;
  vector<int> xs;
  map<int,int> xsall;
  for(const_iterator iy=m_plines.begin();iy!=m_plines.end();++iy) {
    Line& p = *(*iy);
    xs = p._emptycolumns();
    for(unsigned int j=0;j<xs.size();j++) {
      ++xsall[xs[j]];
    }
  }
  xs.clear();
  map<int,int>::iterator first = xsall.begin(); 
  map<int,int>::iterator last  = xsall.end(); 
  while(first!=last) {
    xs.push_back((*first).first);
    ++first;
  }
  sub = column(xs);
  return(sub);
}

/////////////////////////////////////////////////////////////////////////////
Collection  Collection::enclosedlines(int x,const string& start,const string& stop,int n) {
  int from = findy(x,start,n)+1;
  int to   = findy(x,stop,n)-1;
  return(line(from,to));
}
/////////////////////////////////////////////////////////////////////////////
Collection  Collection::_enclosedlines(int x,const string& start,const string& stop,int n) {
  int from = findy(x,start,n)+1;
  int to   = findy(x,stop,n)-1;
  return(_line(from,to));
}
/////////////////////////////////////////////////////////////////////////////
Collection  Collection::enclosedlines(const string& key,const string& start,const string& stop,int n) {
  int x=getx(key);
  int from = findy(x,start,n)+1;
  int to   = findy(x,stop,n)-1;
  return(line(from,to));
}
/////////////////////////////////////////////////////////////////////////////
Collection  Collection::_enclosedlines(const string& key,const string& start,const string& stop,int n) {
  int x=getx(key);
  int from = findy(x,start,n)+1;
  int to   = findy(x,stop,n)-1;
  return(_line(from,to));
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
Collection Collection::unique(int x,int x2,int x3,int x4) {
  Collection sub = unique_count(x,x2,x3,x4);
  //int xcount = sub.getkey().size();
  for(int i=0;i<sub.size();i++) sub[i].pop_back();
  return(sub);
}
/////////////////////////////////////////////////////////////////////////////
Collection Collection::_unique(int x,int x2,int x3,int x4) {
  Collection sub;
  Collection val = unique_count(x).range(1,2,1e99);
  sub = match(x,val,0);
  if(x2>=0) {
    val = unique_count(x2).range(1,2,1e99);
    sub.merge(match(x2,val,0));
  }
  if(x3>=0) {
    val = unique_count(x3).range(1,2,1e99);
    sub.merge(match(x3,val,0));
  }
  if(x4>=0) {
    val = unique_count(x4).range(1,2,1e99);
    sub.merge(match(x4,val,0));
  }
  sub = sub.unique(x,x2,x3,x4);
  sub.multisort(x,x2,x3,x4);
  return(sub);
}
/////////////////////////////////////////////////////////////////////////////
Collection Collection::unique_count(int x,int x2,int x3,int x4) {
  Collection sub;
  // copy common property
  sub.setsheet(p_sheet);
  sub.m_stat = m_stat;
  if(x<0) return(sub);

  // create key
  Line key;
  if(p_key) {
    key.push_back(getkey().get(x).c_str());
    if(x2>=0) key.push_back(getkey().get(x2).tostring());
    if(x3>=0) key.push_back(getkey().get(x3).tostring());
  }
  else {
    key.push_back(x);
    //if(x2>=0) key.back().tostring() += string(",") + x2;
    //if(x3>=0) key.back().tostring() += string(",") + x3;
  }
  key.push_back("count");
  sub.push_back(key);
  sub.setkey(0);

  // unique count
  Line line;
  Collection exist;
  map<string,int> mexist;
  string skeyvalue;
  string sep=g_sep;
  value keyvalue;
  //value countvalue;
  // copy unique
  iterator first=m_plines.begin();
  iterator last =m_plines.end();
  if(x2<0) {
    while(first!=last) {
      keyvalue = (*first)->get(x);
      ++mexist[keyvalue.tostring()];
      ++first;
    }
    map<string,int>::iterator first=mexist.begin();
    map<string,int>::iterator last =mexist.end();
    Line line;
    while(first!=last) {
      line.let(0,(*first).first);
      line.let(1,(*first).second);
      p_sheet->push_back(line);
      sub.push_back(&p_sheet->back());
      ++first;
    }
  }
  else if(x3<0) {
    value keyvalue2;
    while(first!=last) {
      keyvalue = (*first)->get(x);
      keyvalue2 = (*first)->get(x2);
      skeyvalue = keyvalue.tostring() + sep + keyvalue2.tostring();
      ++mexist[skeyvalue];
      ++first;
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
    while(first!=last) {
      keyvalue = (*first)->get(x);
      keyvalue2 = (*first)->get(x2);
      keyvalue3 = (*first)->get(x3);
      skeyvalue = keyvalue.tostring() + sep + keyvalue2.tostring() + sep 
	          + keyvalue3.tostring();
      ++mexist[skeyvalue];
      ++first;
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
    while(first!=last) {
      keyvalue = (*first)->get(x);
      keyvalue2 = (*first)->get(x2);
      keyvalue3 = (*first)->get(x3);
      keyvalue3 = (*first)->get(x4);
      skeyvalue = keyvalue.tostring()  + sep + keyvalue2.tostring() + sep 
	        + keyvalue3.tostring() + sep + keyvalue4.tostring();
      ++mexist[skeyvalue];
      ++first;
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
// map["key1:key2:..."] ->  Collection of matched lines
map<string,Collection> Collection::unique_map(int x,int x2,int x3,int x4) {
  Collection cu = unique_count(x,x2,x3,x4);
  Collection dmy;
  map<string,Collection> um;
  string id;
  for(int i=0;i<cu.size();i++) {
    id = cu[i][0].tostring();
    dmy = match(x,cu[i][0].tostring());
    if(x2>=0) {
      id += string(":") + cu[i][1].tostring();
      dmy = dmy.match(x2,cu[i][1].tostring());
    }
    if(x3>=0) {
      id += ":" + cu[i][2].tostring();
      dmy = dmy.match(x3,cu[i][2].tostring());
    }
    if(x4>=0) {
      id += ":" + cu[i][2].tostring();
      dmy = dmy.match(x4,cu[i][3].tostring());
    }
    um[id] = dmy;
  }
  return(um);
}
/////////////////////////////////////////////////////////////////////////////
Collection Collection::unique_substr_count(int key,const string& separator,int i1,int i2,int i3,int i4) {
  ReadF p;
  p.setseparator(separator.c_str());
  Collection t;
  Line l;
  int i,j;
  for(i=0;i<size();i++) {
    p.parse(get(i)[key].c_str());
    l.clear();
    for(j=1;j<=p.argc;j++) l.push_back(p.argv[j]);
    t.push_back(l);
  }
  Collection r = t.unique_count(i1,i2,i3,i4);
  t.clear();
  l.clear();
  l.push_back(getkey()[key].tostring());
  l.push_back("count");
  t.push_back(l);
  t.setkey(0);
  string buf;
  j = 1;
  if(i2>=0) ++j;
  if(i3>=0) ++j;
  if(i4>=0) ++j;
  for(i=0;i<r.size();i++) {
    if(i1==0) buf = r[i][0].tostring();
    else { buf="*"; buf+=r[i][0].tostring(); }
    buf += "*";
    if(i2>=0) {
      buf+=r[i][1].tostring();
#if defined(E__MULTIBYTE) || defined(E__MULTIBYTELIB) 
      if((buf.size()<2||!IsDBCSLeadByte(buf[buf.size()-2]))
	 && buf[buf.size()-1]!='*') buf += "*";
#else
      if(buf[buf.size()-1]!='*') buf += "*";
#endif
    }
    if(i3>=0) {
      buf+=r[i][2].tostring();
#if defined(E__MULTIBYTE) || defined(E__MULTIBYTELIB) 
      if((buf.size()<2||!IsDBCSLeadByte(buf[buf.size()-2]))
	 && buf[buf.size()-1]!='*') buf += "*";
#else
      if(buf[buf.size()-1]!='*') buf += "*";
#endif
    }
    if(i4>=0) {
      buf+=r[i][3].tostring();
#if defined(E__MULTIBYTE) || defined(E__MULTIBYTELIB) 
      if((buf.size()<2||!IsDBCSLeadByte(buf[buf.size()-2]))
	 && buf[buf.size()-1]!='*') buf += "*";
#else
      if(buf[buf.size()-1]!='*') buf += "*";
#endif
    }
    l.let(0,buf);
    l.let(1,r[i][j].tostring());
    t.push_back(l);
  }  
  return(t);
}
/////////////////////////////////////////////////////////////////////////////
Collection Collection::unique_key_substr_count(const string& separator,int i1,int i2,int i3,int i4) {
  ReadF p;
  p.setseparator(separator.c_str());
  Line key = getkey();
  Collection t;
  Line l;
  int i,j;
  for(i=0;i<key.size();i++) {
    p.parse(key[i].c_str());
    l.clear();
    for(j=1;j<=p.argc;j++) l.push_back(p.argv[j]);
    t.push_back(l);
  }
  Collection r = t.unique_count(i1,i2,i3,i4);
  t.clear();
  l.clear();
  l.push_back("key");
  l.push_back("count");
  t.push_back(l);
  t.setkey(0);
  string buf;
  j = 1;
  if(i2>=0) ++j;
  if(i3>=0) ++j;
  if(i4>=0) ++j;
  for(i=0;i<r.size();i++) {
    if(i1==0) buf = r[i][0].tostring();
    else { buf="*"; buf+=r[i][0].tostring(); }
    buf += "*";
    if(i2>=0 && r[i].size()>1) {
      buf+=r[i][1].tostring();
#if defined(E__MULTIBYTE) || defined(E__MULTIBYTELIB) 
      if((buf.size()<2||!IsDBCSLeadByte(buf[buf.size()-2]))
	 && buf[buf.size()-1]!='*') buf += "*";
#else
      if(buf.size()>0 && buf[buf.size()-1]!='*') buf += "*";
#endif
    }
    if(i3>=0 && r[i].size()>2) {
      buf+=r[i][2].tostring();
#if defined(E__MULTIBYTE) || defined(E__MULTIBYTELIB) 
      if((buf.size()<2||!IsDBCSLeadByte(buf[buf.size()-2]))
	 && buf[buf.size()-1]!='*') buf += "*";
#else
      if(buf.size()>0 && buf[buf.size()-1]!='*') buf += "*";
#endif
    }
    if(i4>=0 && r[i].size()>3) {
      buf+=r[i][2].tostring();
#if defined(E__MULTIBYTE) || defined(E__MULTIBYTELIB) 
      if((buf.size()<2||!IsDBCSLeadByte(buf[buf.size()-2]))
	 && buf[buf.size()-1]!='*') buf += "*";
#else
      if(buf.size()>0 && buf[buf.size()-1]!='*') buf += "*";
#endif
    }
    l.let(0,buf);
    l.let(1,r[i][j].tostring());
    t.push_back(l);
  }  
  return(t);
}
/////////////////////////////////////////////////////////////////////////////
#if 0
static void setstatlabel(Collection& c,int x) {
  Line& key = c.getkey();
  key.let(x  ,"count");
  key.let(x+1,"mean");
  key.let(x+2,"stddev");
  key.let(x+3,"min");
  key.let(x+4,"max");
}
/////////////////////////////////////////////////////////////////////////////
static void setstat(Line& l,const statistics& s,int x) {
  l.let(x  ,s.nsample());
  l.let(x+1,s.mean());
  l.let(x+2,s.stddev());
  l.let(x+3,s.min());
  l.let(x+4,s.max());
}
#endif
/////////////////////////////////////////////////////////////////////////////
Collection Collection::unique_stat(map<string,statistics>& mexist
				   ,int xval,int x,int x2,int x3,int x4) {
  Collection sub;
  // copy common property
  sub.setsheet(p_sheet);
  sub.m_stat = m_stat;
  if(x<0) return(sub);

  // create key
  Line key;
  if(p_key) {
    key.push_back(getkey().get(x).c_str());
    if(x2>=0) key.push_back(getkey().get(x2).tostring());
    if(x3>=0) key.push_back(getkey().get(x3).tostring());
    if(x4>=0) key.push_back(getkey().get(x4).tostring());
  }
  else {
    key.push_back(x);
    //if(x2>=0) key.back().tostring() += string(",") + x2;
    //if(x3>=0) key.back().tostring() += string(",") + x3;
    //if(x4>=0) key.back().tostring() += string(",") + x4;
  }
  key.push_back("count");// key.push_back("nsample");
  key.push_back("mean");
  key.push_back("stddev");
  key.push_back("min");
  key.push_back("max");
  key.push_back("sigma");

  sub.push_back(key);
  sub.setkey(0);

  // unique count
  Line line;
  Collection exist;
  //map<string,statistics> mexist;
  string skeyvalue;
  string sep=g_sep;
  value keyvalue;
  //value countvalue;
  // copy unique
  iterator first=m_plines.begin();
  iterator last =m_plines.end();
  if(x2<0) {
    while(first!=last) {
      keyvalue = (*first)->get(x);
      mexist[keyvalue.tostring()].add((*first)->get(xval).todouble());
      ++first;
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
    while(first!=last) {
      keyvalue = (*first)->get(x);
      keyvalue2 = (*first)->get(x2);
      skeyvalue = keyvalue.tostring() + sep + keyvalue2.tostring();
      mexist[skeyvalue].add((*first)->get(xval).todouble());
      ++first;
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
    while(first!=last) {
      keyvalue = (*first)->get(x);
      keyvalue2 = (*first)->get(x2);
      keyvalue3 = (*first)->get(x3);
      skeyvalue = keyvalue.tostring() + sep + keyvalue2.tostring() + sep 
	          + keyvalue3.tostring();
      mexist[skeyvalue].add((*first)->get(xval).todouble());
      ++first;
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
    while(first!=last) {
      keyvalue = (*first)->get(x);
      keyvalue2 = (*first)->get(x2);
      keyvalue3 = (*first)->get(x3);
      keyvalue4 = (*first)->get(x4);
      skeyvalue = keyvalue.tostring()  + sep + keyvalue2.tostring() + sep 
	        + keyvalue3.tostring() + sep + keyvalue3.tostring();
      mexist[skeyvalue].add((*first)->get(xval).todouble());
      ++first;
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
      line.let(3,ps.argv[3]);
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
Collection Collection::unique_list(int xval,int x,int x2,int x3,int x4) {
  Collection sub;
  // copy common property
  sub.setsheet(p_sheet);
  sub.m_stat = m_stat;
  if(x<0) return(sub);

  // create key
  Line key;
  if(p_key) {
    key.push_back(getkey().get(x).c_str());
    if(x2>=0) key.push_back(getkey().get(x2).tostring());
    if(x3>=0) key.push_back(getkey().get(x3).tostring());
    if(x4>=0) key.push_back(getkey().get(x4).tostring());
  }
  else {
    key.push_back(x);
    //if(x2>=0) key.back().tostring() += string(",") + x2;
    //if(x3>=0) key.back().tostring() += string(",") + x3;
    //if(x4>=0) key.back().tostring() += string(",") + x3;
  }
  key.push_back("count");// key.push_back("nsample");
  key.push_back("data");

  sub.push_back(key);
  sub.setkey(0);

  // unique count
  Line line;
  Collection exist;
  map<string,vector<double> > mexist;
  string skeyvalue;
  string sep=g_sep;
  value keyvalue;
  //value countvalue;
  // copy unique
  iterator first=m_plines.begin();
  iterator last =m_plines.end();
  if(x2<0) {
    // make list
    while(first!=last) {
      keyvalue = (*first)->get(x);
      mexist[keyvalue.tostring()].push_back((*first)->get(xval).todouble());
      ++first;
    }
    // copy list to output Collection
    map<string,vector<double> >::iterator first=mexist.begin();
    map<string,vector<double> >::iterator last =mexist.end();
    Line line;
    while(first!=last) {
      line.let(0,(*first).first);
      line.let(1,(*first).second.size());
      for(int ii=0;(unsigned int)ii<(*first).second.size();ii++) 
        line.let(ii+2,(*first).second[ii]);
      p_sheet->push_back(line);
      sub.push_back(&p_sheet->back());
      ++first;
    }
  }
  else if(x3<0) {
    // make list
    value keyvalue2;
    while(first!=last) {
      keyvalue = (*first)->get(x);
      keyvalue2 = (*first)->get(x2);
      skeyvalue = keyvalue.tostring() + sep + keyvalue2.tostring();
      mexist[skeyvalue].push_back((*first)->get(xval).todouble());
      ++first;
    }
    // copy list to output Collection
    map<string,vector<double> >::iterator first=mexist.begin();
    map<string,vector<double> >::iterator last =mexist.end();
    Line line;
    ReadF ps; ps.setdelimiter(sep.c_str()); ps.setseparator("");
    while(first!=last) {
      ps.parse((*first).first.c_str());
      line.let(0,ps.argv[1]);
      line.let(1,ps.argv[2]);
      line.let(2,(*first).second.size());
      for(int ii=0;(unsigned int)ii<(*first).second.size();ii++) 
        line.let(ii+3,(*first).second[ii]);
      p_sheet->push_back(line);
      sub.push_back(&p_sheet->back());
      ++first;
    }
  }
  else if(x4<0) {
    // make list
    value keyvalue2,keyvalue3;
    while(first!=last) {
      keyvalue = (*first)->get(x);
      keyvalue2 = (*first)->get(x2);
      keyvalue3 = (*first)->get(x3);
      skeyvalue = keyvalue.tostring() + sep + keyvalue2.tostring() + sep 
	          + keyvalue3.tostring();
      mexist[skeyvalue].push_back((*first)->get(xval).todouble());
      ++first;
    }
    // copy list to output Collection
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
      for(int ii=0;(unsigned int)ii<(*first).second.size();ii++) 
        line.let(ii+4,(*first).second[ii]);
      p_sheet->push_back(line);
      sub.push_back(&p_sheet->back());
      ++first;
    }
  }
  else {
    // make list
    value keyvalue2,keyvalue3,keyvalue4;
    while(first!=last) {
      keyvalue = (*first)->get(x);
      keyvalue2 = (*first)->get(x2);
      keyvalue3 = (*first)->get(x3);
      keyvalue4 = (*first)->get(x4);
      skeyvalue = keyvalue.tostring() + sep + keyvalue2.tostring() + sep 
	        + keyvalue3.tostring() + sep + keyvalue4.tostring();
      mexist[skeyvalue].push_back((*first)->get(xval).todouble());
      ++first;
    }
    // copy list to output Collection
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
      for(int ii=0;(unsigned int)ii<(*first).second.size();ii++) 
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
Collection Collection::unique_hist(
			double minstddev,double maxstddev,int nbin,
			int xval,int x,int x2,int x3,int x4){
  map<string,statistics> mstat;
  Collection cstat = unique_stat(mstat,xval,x,x2,x3,x4);
  map<string,Histogram<double> > mexist;
  return(unique_hist(mstat,mexist,minstddev,maxstddev,nbin,xval,x,x2,x3,x4)); 
}

/////////////////////////////////////////////////////////////////////////////
Collection Collection::unique_hist(
  				map<string,statistics>& mstat
				,map<string,Histogram<double> >& mexist
				,double minstddev,double maxstddev,int nbin
				,int xval,int x,int x2,int x3,int x4) {
  map<string,statistics>::iterator sfirst,slast=mstat.end();
  string skeyvalue;
  double vmin,vmax,vmean,vstddev;
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

  // define separater 
  string sep=g_sep;
  value keyvalue;

  if(x2<0) { // 1 keyvalue
    // fill histogram
    for(i=0;i<size();i++) {
      keyvalue = get(i).get(x);
      mexist[keyvalue.tostring()].fill(get(i).get(xval).todouble());
    }

    // copy histogram to output Collection
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

  else if(x3<0) { // 2 keyvalues
    // fill histogram
    value keyvalue2;
    for(i=0;i<size();i++) {
      keyvalue = get(i).get(x);
      keyvalue2 = get(i).get(x2);
      skeyvalue = keyvalue.tostring() + sep + keyvalue2.tostring();
      mexist[skeyvalue].fill(get(i).get(xval).todouble());
    }

    // copy histogram to output Collection
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

  else if(x4<0) { // 3 keyvalues
    // fill histogram
    value keyvalue2,keyvalue3;
    for(i=0;i<size();i++) {
      keyvalue = get(i).get(x);
      keyvalue2 = get(i).get(x2);
      keyvalue3 = get(i).get(x3);
      skeyvalue = keyvalue.tostring() + sep + keyvalue2.tostring() + sep 
	          + keyvalue3.tostring();
      mexist[skeyvalue].fill(get(i).get(xval).todouble());
    }

    // copy histogram to output Collection
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
  else { // 4 key values
    // fill histogram
    value keyvalue2,keyvalue3,keyvalue4;
    for(i=0;i<size();i++) {
      keyvalue = get(i).get(x);
      keyvalue2 = get(i).get(x2);
      keyvalue3 = get(i).get(x3);
      keyvalue4 = get(i).get(x4);
      skeyvalue = keyvalue.tostring() + sep + keyvalue2.tostring() + sep 
	        + keyvalue3.tostring() + sep + keyvalue4.tostring();
      mexist[skeyvalue].fill(get(i).get(xval).todouble());
    }

    // copy histogram to output Collection
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
/////////////////////////////////////////////////////////////////////////////
// When there are multiple lines in same key values, sammple certain value.
// ...,x ,...,x2   ,...,x3  ,...,xval,...
// ...,va,...,za   ,...,y1  ,...,0.15,...  << "first"
// ...,va,...,za   ,...,y1  ,...,5.02,...
// ...,va,...,za   ,...,y1  ,...,1.23,...  << "last"
/////////////////////////////////////////////////////////////////////////////
Collection Collection::unique_sampling(const Collection& u
				       ,const char* mode //first,last,mean,max,min
				       ,int xval ,int x,int x2,int x3,int x4) {
  Collection sub, m;
  if(p_key) { // copy key line
    sub.push_back(getkey());
    sub.setkey(0);
  }

  int i;
  for(i=0;i<u.size();i++) {
    // gather lines with same key values
    if(x4>=0) {
      m = match(x,u[i][0].c_str()) .match(x2,u[i][1].c_str())
	                           .match(x3,u[i][2].c_str())
	                           .match(x4,u[i][3].c_str());
    }
    else if(x3>=0) {
      m = match(x,u[i][0].c_str()) .match(x2,u[i][1].c_str())
	                           .match(x3,u[i][2].c_str());
    }
    else if(x2>=0) {
      m = match(x,u[i][0].c_str()) .match(x2,u[i][1].c_str());
    }
    else  {
      m = match(x,u[i][0].c_str());
    }

    // calculate according to sampling criteria
    switch(m.size()) {
    case 0: break;
    case 1: sub.push_back(m[0]); break;
    default:
      if(strcmp(mode,"first")==0) sub.push_back(m.front());
      else if(strcmp(mode,"last")==0) sub.push_back(m.back());
      else if(strcmp(mode,"mean")==0) {
         sub.push_back(m.front());
         sub.back().let(xval,m.stat()[xval].mean());
      }
      else if(strcmp(mode,"max")==0) {
	sub.push_back(m.front());
        sub.back().let(xval,m.stat()[xval].max());
      }
      else if(strcmp(mode,"min")==0) {
	sub.push_back(m.front());
        sub.back().let(xval,m.stat()[xval].min());
      }
      else {
        fprintf(stderr
                 ,"Error: Collection::unique_sampling() unknown mode '%s'\n"
		 ,mode);
	return(sub);
      }
      break;
    }
  }
  return(sub);
}

/////////////////////////////////////////////////////////////////////////////
// Align lines with cteplate_stat and normalize value by
/////////////////////////////////////////////////////////////////////////////
Collection Collection::align_normalize(Collection& ctemplate_stat
				       ,const char* mode // gain, offset, stddev
				       ,double normgain,double normmean
				       ,int xval,int x,int x2,int x3,int x4) {
  // normalization mode
  EasyExp gain("*gain*");
  EasyExp sd("*stddev*");
  EasyExp offset("*offset*");
  int imode_offset = offset.match(mode);
  int imode_gain   = gain.match(mode);
  int imode_stddev = sd.match(mode);

  int i,j;
  double origval,normval,stddev,mean=0.0;
  int xmean=0;
  int xstddev=0;
  int xcount  = ctemplate_stat.getx("count");
  //int xsigma  = ctemplate_stat.getx("sigma");
  Collection m;

  if(imode_offset) xmean   = ctemplate_stat.getx("mean");
  if(imode_stddev) xstddev = ctemplate_stat.getx("stddev");

  // copy template testitem and key
  Collection sub;
  Line line;
  if(ctemplate_stat.p_key) {
    for(i=0;i<xcount;i++) line.push_back(ctemplate_stat.getkey()[i]);
    sub.push_back(line);
    sub.setkey(0);
  }
  for(j=0;j<ctemplate_stat.size();j++) {
    line.clear();
    for(i=0;i<xcount;i++) line.push_back(ctemplate_stat[j][i].c_str());
    sub.push_back(line);
  }

  // set value key
  if(sub.p_key && p_key) sub.getkey().let(xcount,getkey()[xval].c_str());

  // fill output Collection
  for(i=0;i<ctemplate_stat.size();i++) {
    // find matching line
    if(x4>=0)      m=match(x ,ctemplate_stat[i][0].c_str())
	            .match(x2,ctemplate_stat[i][1].c_str())
	            .match(x3,ctemplate_stat[i][2].c_str())
	            .match(x4,ctemplate_stat[i][3].c_str());
    else if(x3>=0)      m=match(x ,ctemplate_stat[i][0].c_str())
	            .match(x2,ctemplate_stat[i][1].c_str())
	            .match(x3,ctemplate_stat[i][2].c_str());
    else if(x2>=0) m=match(x ,ctemplate_stat[i][0].c_str())
	            .match(x2,ctemplate_stat[i][1].c_str());
    else           m=match(x ,ctemplate_stat[i][0].c_str());

    // if exists, normalize and set value to output Collection
    if(m.size()) {
      origval = m[0][xval].todouble();
      if(imode_offset) {
	if(xmean>=0) mean   = ctemplate_stat[i][xmean].todouble();
	origval -= mean;
      }
      if(imode_gain)   normval = origval/normgain;
      else if(imode_stddev) {
        if(xstddev>=0) stddev = ctemplate_stat[i][xstddev].todouble();
        else           stddev = 0;
        if(stddev!=0)  normval = origval/stddev/normgain;
        else           normval = origval;
      }
      else             normval = origval;

      sub[i].let(xcount,normval);
    }
  }
  return(sub);
}

/////////////////////////////////////////////////////////////////////////////
static void setvector(vector<double>& v,double val) {
  for(unsigned int i=0;i<v.size();i++) v[i]=val;
}
/////////////////////////////////////////////////////////////////////////////
// convert normalized value on feature space
// mode:
//  dot : min   max       bar : min   max      bipol bar : min         max
//        **       step         *        step              *******
//           **                 *******                           *******
//
// flat n, 1/x n, gauss n
//  value : [value]
//
// range: -1 to 1, 0 to 1, low to high
/////////////////////////////////////////////////////////////////////////////
Collection Collection::featurespace(const char* mode 
				,double inmin,double inmax,double instep
				,int offsetmode
			        ,double outlow, double outhigh,double nullval
				,int xval) {
  // declare common variables
  int i,j,ndiv=0;;
  double val;             // temporary value buffer
  vector<double> feature; // temporary feature vector storage
  Collection sub;         // output

  // add empty lines to output
  Line line; 
  for(j=0;j<size();j++) sub.push_back(line);

  // Symbol char table
  const char* symp =  "!+>^hmxxxxxxx";
  const char* symm =  "!-<_lwzzzzzzz";
  char symbuf[10];

  // major mode definition
  EasyExp dot("*dot*");
  EasyExp bar("*bar*"); 
  EasyExp value("*value*");
  // dot mode /////////////////////////////////////////////////////////
  if(dot.match(mode)) {
    // preliminarily feature space vector dimension
    ndiv=int((inmax-inmin)/instep)+1;
    int index;

    // capture minor mode
    int dotmode=0; 
    EasyExp flat2("*flat2*"); 
    EasyExp flat3("*flat3*"); 
    EasyExp x_1("*1/*"); 
    EasyExp gau("*gau*");
    if(flat2.match(mode))    { dotmode = 1; ndiv+=1; }
    if(flat3.match(mode))    { dotmode = 2; ndiv+=2; }
    else if(x_1.match(mode)) { dotmode = 3; ndiv+=2; }
    else if(gau.match(mode)) { dotmode = 4; ndiv+=2; }

    // allocate feature space vector
    feature.resize(ndiv);

    // symbol char table medium point
    int med = ndiv/2;

    // scan through data for feature space translation
    for(i=0;i<sub.size();i++) {
      // get original value 
      if(get(i).size()>xval && get(i)[xval].tostring()!="") 
	val = get(i)[xval].todouble();
      else {
        //setvector(feature,outlow);
        setvector(feature,nullval);
        sub[i].let(0,feature);
        sub[i].let(ndiv,"# ");
        continue;
      }

      if(val<inmin) val=inmin;
      else if(val>inmax) val=inmax;

      // calculate feature space vector index
      index = int((val-inmin)/instep) + ((dotmode==0)?0:1);

      // generate symbol char and set to ndiv column
      // MLl<-..+>hHW
      if(index>med) sprintf(symbuf,"#%c",symp[index-med]);
      else sprintf(symbuf,"#%c",symm[med-index]);
      sub[i].let(ndiv,symbuf);

      // reset feature vector
      setvector(feature,outlow);

      // feature space translation
      switch(dotmode) {
      case 0: // dot
        feature[index]=outhigh; break; 
      case 1: // flat2;
        feature[index-1]=outhigh; 
        feature[index  ]=outhigh; 
        break;
      case 2: // flat3;
        feature[index-1]=outhigh; 
        feature[index  ]=outhigh; 
        feature[index+1]=outhigh;
        break;
      case 3: // 1/x
        for(j=0;j<ndiv;j++) {
          if(j==index) feature[j]=outhigh;
          else {
            int distance = abs(j-index)+1;
            feature[j] = (outhigh-outlow)/distance + outlow;
          }
        }
        break;
      case 4: // gauss
	// TO BE IMPLEMENTED
      default:
        break;
      }
      // copy feature space data to output Collection
      sub[i].let(0,feature);
    }
  }

  // bar mode ////////////////////////////////////////////////////////
  else if(bar.match(mode)) {
    // preliminarily feature space vector dimension
    ndiv=int((inmax-inmin)/instep);
    int index;

    // capture minor mode
    int barmode=0, offset=0;
    EasyExp bipolar("*bi*");
    if(bipolar.match(mode))   { barmode = 1; offset=ndiv/2; }

    EasyExp offsetexpr("*off*");
    if(offsetexpr.match(mode)) { offsetmode=1; }

    // allocate feature space vector
    feature.resize(ndiv);

    // scan through data for feature space translation
    for(i=0;i<sub.size();i++) {
      // get original value 
      if(get(i).size()>xval && get(i)[xval].tostring()!="") 
	val = get(i)[xval].todouble();
      else {
        //setvector(feature,outlow);
        setvector(feature,nullval);
        sub[i].let(0,feature,offsetmode,offset);
        sub[i].let(ndiv,"# ");
        continue;
      }
      if(val<inmin) val=inmin;
      else if(val>inmax) val=inmax;

      // reset feature vector
      setvector(feature,outlow);

      // feature space translation
      switch(barmode) {
      case 0: // bar
        // calculate feature space vector index
        index = abs(int(val/instep));
        // generate symbol char and set to ndiv column
        // TBD   .+>hHMW
        sprintf(symbuf,"#%c",symp[index]);
        sub[i].let(ndiv,symbuf);
	
        for(j=offset;j<index;j++) feature[j]=outhigh;
        // copy feature space data to output Collection
        sub[i].let(0,feature,offsetmode,offset);
        break;
      case 1: // bipolar bar
        index = abs(int(val/instep));
        // generate symbol char and set to ndiv column
        // TBD   MLl<-..+>hHW
        if(val>=0) {
          for(j=offset;j<offset+index;j++) feature[j]=outhigh;
          sprintf(symbuf,"#%c",symp[index]);
        }
        else {
          for(j=offset;j>offset-index;j--) feature[j-1]=outhigh;
          sprintf(symbuf,"#%c",symm[index]);
        }
        sub[i].let(ndiv,symbuf);

        // copy feature space data to output Collection
        sub[i].let(0,feature,offsetmode,offset);
        break;
      default:
        break;
      }
    }
  }

  // value mode //////////////////////////////////////////////////////
  else if(value.match(mode)) {
    ndiv=1;
    double scale = (inmax-inmin)/2;
    int index;
    for(i=0;i<sub.size();i++) {
      if(get(i).size()>xval && get(i)[xval].tostring()!="") {
        val = get(i)[xval].todouble();
        sub[i].let(0,val);
        // generate symbol char and set to ndiv column
        // TBD
        // sub[i].let(ndiv,0);
        if(val>0) {
          index = abs(int((val<inmax?val:inmax)/scale/instep));
          sprintf(symbuf,"#%c",symp[index]);
        }
        else {
          index = abs(int((val>inmin?val:inmin)/scale/instep));
          sprintf(symbuf,"#%c",symm[index]);
        }
        sub[i].let(ndiv,symbuf);
      }
      else {
        sub[i].let(0,0.0);
        sub[i].let(ndiv,"# ");
      }
    }
  }

  // unknown mode ///////////////////////////////////////////////////
  else {
    fprintf(stderr,"Error: Collection::featurespace() unknown mode '%s'\n"
		,mode);
  }
  // End of feature vector translation
  ////////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////
  // append testitem name as comment after each line
  ///////////////////////////////////////////////////////////////
  for(j=0;j<sub.size();j++) {
    // # will be added before symbol char in previous blocks. 
    // Remove following line when that is implemented. TBD
    // sub[j].let(ndiv,"#");
    for(i=0;i<xval;i++) sub[j].let(ndiv+i+1,get(j)[i].c_str());
  }

  return(sub);
}

/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
Collection Collection::cluster(int x,statistics& stat,const string& separator) {
  Collection r;
  ReadF p; p.setseparator(separator.c_str());
  int abort_clustering=0;
  map<string,int> mexist;
  value keyvalue;
  iterator first=m_plines.begin();
  iterator last =m_plines.end();
  while(first!=last) {
    keyvalue = (*first)->get(x);
    p.parse(keyvalue.c_str());
    ++mexist[keyvalue.tostring()];
    if(mexist.size()>(unsigned int)m_clusterlimit) {
      abort_clustering=1;
      break;
    }
    ++first;
  }

  Line key; key.push_back(getkey()[x]); key.push_back("count");
  Collection sub ; sub.push_back(key); sub.setkey(0);
  sub.setsheet(p_sheet); sub.m_stat = m_stat;

  stat.init();

  map<string,int>::iterator first2=mexist.begin();
  map<string,int>::iterator last2 =mexist.end();
  Line line;
  string val;
  while(first2!=last2) {
    if(abort_clustering==0) {
      line.let(0,(*first2).first);
      line.let(1,(*first2).second);
      p_sheet->push_back(line);
      sub.push_back(&p_sheet->back());
    }
    val = (*first2).first; 
    if(isdbl(val) || isint(val)) stat.add(1);
    else { p.parse(val.c_str()); stat.add(p.argc); }
    ++first2;
  }
  return(sub);
}

/////////////////////////////////////////////////////////////////////////////
Collection Collection::cluster_substr(int x,int y,const string& separator) {
  ReadF p; p.setseparator(separator.c_str());
  Collection r;
  int abort_clustering=0;
  map<string,int> mexist;
  value keyvalue;
  iterator first=m_plines.begin();
  iterator last =m_plines.end();
  while(first!=last) {
    keyvalue = (*first)->get(x);
    p.parse(keyvalue.c_str());
    if(y<p.argc) ++mexist[p.argv[y+1]];
    if(mexist.size()>(unsigned int)m_clusterlimit) {
      abort_clustering=1;
      break;
    }
    ++first;
  }

  char buf[500];
  sprintf(buf,"%s:%d",getkey()[x].c_str(),y);
  Line key; key.push_back(buf); key.push_back("count");
  Collection sub ; sub.push_back(key); sub.setkey(0);
  sub.setsheet(p_sheet); sub.m_stat = m_stat;

  map<string,int>::iterator first2=mexist.begin();
  map<string,int>::iterator last2 =mexist.end();
  Line line;
  string k;
  while(first2!=last2) {
    if(abort_clustering==0) {
      line.let(0,(*first2).first);
      line.let(1,(*first2).second);
      p_sheet->push_back(line);
      sub.push_back(&p_sheet->back());
    }
    ++first2;
  }
  return(sub);
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
Collection Collection::column(int from,int to) {
  if(to<0) to=from;
  Collection sub;
  sub.setsheet(p_sheet);
  sub.p_key=p_key;
  if(sub.m_stat.size()) sub.m_stat = m_stat.column(from,to);
  Line line;
  iterator first=m_plines.begin();
  iterator last =m_plines.end();
  while(first!=last) {
    line=(*first)->column(from,to);
    p_sheet->push_back(line);
    sub.push_back(&p_sheet->back());  
    ++first;
  }
  if(p_key) {
    line=p_key->column(from,to);
    p_sheet->push_back(line);
    sub.p_key = &p_sheet->back();
  }
  return(sub);
}
/////////////////////////////////////////////////////////////////////////////
Collection Collection::_column(int from,int to) {
  if(to<0) to=from;
  Collection sub;
  sub.setsheet(p_sheet);
  sub.p_key=p_key;
  sub.m_stat = m_stat._column(from,to);
  Line line;
  iterator first=m_plines.begin();
  iterator last =m_plines.end();
  while(first!=last) {
    line=(*first)->_column(from,to);
    p_sheet->push_back(line);
    sub.push_back(&p_sheet->back());  
    ++first;
  }
  if(p_key) {
    line=p_key->_column(from,to);
    p_sheet->push_back(line);
    sub.p_key = &p_sheet->back();
  }
  return(sub);
}
/////////////////////////////////////////////////////////////////////////////
Collection  Collection::column(const string& from,const string& to) { 
  int x1=getx(from);
  if(x1<0) {
    fprintf(stderr,"Error: column(%s,%s) key %s not found\n"
     ,from.c_str(),to.c_str(),from.c_str());
#ifdef __CINTLIB__
    G__printlinenum();
#endif
    return(Collection(p_sheet));
  }
  int x2;
  if(to=="") x2=x1;
  else       x2=getx(to);
  if(x2<0) {
    fprintf(stderr,"Error: column(%s,%s) key %s not found\n"
     ,from.c_str(),to.c_str(),to.c_str());
#ifdef __CINTLIB__
    G__printlinenum();
#endif
    return(Collection(p_sheet));
  }
  return(column(x1,x2));
}
/////////////////////////////////////////////////////////////////////////////
Collection Collection::_column(const string& from,const string& to) {
  int x1=getx(from);
  if(x1<0) {
    fprintf(stderr,"Error: column(%s,%s) key %s not found\n"
     ,from.c_str(),to.c_str(),from.c_str());
#ifdef __CINTLIB__
    G__printlinenum();
#endif
    return(Collection(p_sheet));
  }
  int x2;
  if(to=="") x2=x1;
  else       x2=getx(to);
  if(x2<0) {
    fprintf(stderr,"Error: column(%s,%s) key %s not found\n"
     ,from.c_str(),to.c_str(),to.c_str());
#ifdef __CINTLIB__
    G__printlinenum();
#endif
    return(Collection(p_sheet));
  }
  return(_column(x1,x2));
}
/////////////////////////////////////////////////////////////////////////////
Collection  Collection::columns(const string& keyexpr) {
  int i;
  Line key;
  vector<int> xs;
  EasyExp e(keyexpr.c_str(),m_EasyExpMode);
  if(p_key) {
    for(i=0;i<p_key->size();i++) {
      if(e.match(p_key->get(i).c_str())) {
        key.push_back(p_key->get(i).c_str());
	xs.push_back(i);
      } 
    }
  }
  return(column(xs));
}
/////////////////////////////////////////////////////////////////////////////
Collection  Collection::_columns(const string& keyexpr) {
  int i;
  Line key;
  vector<int> xs;
  EasyExp e(keyexpr.c_str(),m_EasyExpMode);
  if(p_key) {
    for(i=0;i<p_key->size();i++) {
      if(e.match(p_key->get(i).c_str())) {
        key.push_back(p_key->get(i).c_str());
	xs.push_back(i);
      } 
    }
  }
  return(_column(xs));
}
/////////////////////////////////////////////////////////////////////////////
Collection Collection::column(const vector<int>& p) {
  Collection sub;
  sub.setsheet(p_sheet);
  sub.p_key=p_key;
  //sub.m_stat = m_stat.column(p);
  Line line;
  iterator first=m_plines.begin();
  iterator last =m_plines.end();
  while(first!=last) {
    line=(*first)->column(p);
    p_sheet->push_back(line);
    sub.push_back(&p_sheet->back());  
    ++first;
  }
  if(p_key) {
    line=p_key->column(p);
    p_sheet->push_back(line);
    sub.p_key = &p_sheet->back();
  }
  return(sub);
}
/////////////////////////////////////////////////////////////////////////////
Collection Collection::_column(const vector<int>& p) {
  if(p.size()==0)  return(*this); 
  int lsize = getkey().size();
  vector<int> _p;
  int i,j=0;
  for(i=0;i<lsize;i++) {
    if(p[j]==i)  ++j;
     else        _p.push_back(i);
  }
  return(column(_p));
}
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
Collection  Collection::match_erase(int x,const string& expr,bool icase) {
  EasyExp regexpr(expr.c_str(),m_EasyExpMode);
  Collection sub;
  sub.setsheet(p_sheet);
  sub.p_key=p_key;
  sub.m_stat = m_stat;

  Collection remain;
  remain.setsheet(p_sheet);
  remain.p_key=p_key;
  remain.m_stat = m_stat;
  for(const_iterator iy=m_plines.begin();iy!=m_plines.end();++iy) {
    Line& p = *(*iy);
    if(p.size()>x) {
      const value& v = p[x];
      if(regexpr.match(v.s.c_str(),icase)) sub.push_back(&p);
      else                                 remain.push_back(&p);
    }
  }
  m_plines = remain.m_plines;
  return(sub); // returning erased lines, questionable definition
}
/////////////////////////////////////////////////////////////////////////////
Collection  Collection::match_erase(const string& key,const string& expr,bool icase) {
  vector<int> xs = getxs(key);
  if(xs.size()==0) {
    fprintf(stderr,"Error: match_erase(%s,%s,%d) key not found\n"
	    ,key.c_str(),expr.c_str(),icase);
#ifdef __CINTLIB__
    G__printlinenum();
#endif
    return(Collection(p_sheet));
  }
  else if(xs.size()==1) return(match_erase(xs[0],expr,icase));
  else return(match_erase(xs,expr,icase));
}

#ifdef MULTIPLEKEYS
/////////////////////////////////////////////////////////////////////////////
Collection  Collection::match_erase(const vector<int>& xs,const string& expr,bool icase) {
  EasyExp regexpr(expr.c_str(),m_EasyExpMode);
  Collection sub;
  sub.setsheet(p_sheet);
  sub.p_key=p_key;
  sub.m_stat = m_stat;

  Collection remain;
  remain.setsheet(p_sheet);
  remain.p_key=p_key;
  remain.m_stat = m_stat;
  for(const_iterator iy=m_plines.begin();iy!=m_plines.end();++iy) {
    Line& p = *(*iy);
    vector<int>::const_iterator ifirst=xs.begin();
    vector<int>::const_iterator ilast=xs.end();
    int flag=0;
    while(ifirst!=ilast) {
      int x = *ifirst++;
      if(p.size()>x) {
        const value& v = p[x];
        if(regexpr.match(v.s.c_str(),icase)) ++flag;
      }  
    }  
    if(flag) sub.push_back(&p);
    else     remain.push_back(&p);
  }
  m_plines = remain.m_plines;
  return(sub); // returning erased lines, questionable definition
}
#endif
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
Collection& Collection::merge(const Collection& a) {
  const_iterator first=a.m_plines.begin();
  const_iterator last =a.m_plines.end();
  iterator pos;
  while(first!=last) {
    m_plines.push_back(*first);
    ++first;
  }
  if(!p_key && a.p_key) p_key = a.p_key;
  return(*this);
}
/////////////////////////////////////////////////////////////////////////////
Collection& Collection::unique_merge(const Collection& a) {
  const_iterator first=a.m_plines.begin();
  const_iterator last =a.m_plines.end();
  iterator pos;
  while(first!=last) {
    pos = find(m_plines.begin(),m_plines.end(),*first);  
    if(pos==m_plines.end()) m_plines.push_back(*first);
    ++first;
  }
  return(*this);
}

/////////////////////////////////////////////////////////////////////////////
Collection& Collection::operator-=(const Collection& a) {
  const_iterator first=a.m_plines.begin();
  const_iterator last =a.m_plines.end();
  iterator pos;
  while(first!=last) {
    pos = find(m_plines.begin(),m_plines.end(),*first);  
    if(pos!=m_plines.end()) m_plines.erase(pos);
    ++first;
  }
  return(*this);
}

/////////////////////////////////////////////////////////////////////////////
Collection& Collection::operator&=(const Collection& a) {
  const_iterator first=a.m_plines.begin();
  const_iterator last =a.m_plines.end();
  iterator pos;
  Collection tmp;
  while(first!=last) {
    pos = find(m_plines.begin(),m_plines.end(),*first);  
    if(pos!=m_plines.end()) tmp.push_back(*first);
    ++first;
  }
  m_plines.clear();
  m_plines=tmp.m_plines;
  return(*this);
}

/////////////////////////////////////////////////////////////////////////////
Line& Collection::get(int y) { 
  if(y>=0 && y<(int)m_plines.size()) return(*m_plines[y]); 
  else return(m_dummy);
}
/////////////////////////////////////////////////////////////////////////////
#ifdef COLLECTIONCOMPARE
const Line& Collection::get(int y) const { 
  if(y>=0 && y<(int)m_plines.size()) return(*m_plines[y]); 
  else return(m_dummy);
}
#endif
/////////////////////////////////////////////////////////////////////////////

//
/////////////////////////////////////////////////////////////////////////////
Collection& Collection::sort(const string& key,CompareLine::mode_t mode) {
  int x=getx(key);
  if(x<0) {
    fprintf(stderr,"Error: sort(%s,%d) key not found\n",key.c_str(),mode);
#ifdef __CINTLIB__
    G__printlinenum();
#endif
    return(*this);
  }
  sort(x,mode);
  return(*this);
}

/////////////////////////////////////////////////////////////////////////////
Collection& Collection::sort(int x,CompareLine::mode_t mode) {
  CompareLine comp(x,mode);
  std::sort(m_plines.begin(),m_plines.end(),comp);
  return(*this);
}

/////////////////////////////////////////////////////////////////////////////
Collection& Collection::fixsort(int x,int xfix) {
  CompareLine comp(x,CompareLine::Auto);
  iterator first;
  iterator last ;
  value fix;
  int i=0;
  while(i<(int)m_plines.size()) {
    first = numtoiter(m_plines,i);
    fix = m_plines[i]->get(xfix);
    while(i<(int)m_plines.size() && m_plines[i]->get(xfix)==fix) ++i;
    if(i>=(int)m_plines.size()) last = m_plines.end();
    else last = numtoiter(m_plines,i);
    std::sort(first,last,comp);
  }
  return(*this);
}
/////////////////////////////////////////////////////////////////////////////
#ifndef MULTISORT_FIX2
Collection& Collection::fixsort(int x,int xfix,int xfix2,int xfix3) {
  if(xfix3<0) return(fixsort(x,xfix,xfix2));
  CompareLine comp(x,CompareLine::Auto);
  iterator first;
  iterator last ;
  value fix,fix2,fix3;
  int i=0;
  while(i<(int)m_plines.size()) {
    first = numtoiter(m_plines,i);
    fix = m_plines[i]->get(xfix);
    fix2 = m_plines[i]->get(xfix2);
    fix3 = m_plines[i]->get(xfix3);
    while(i<(int)m_plines.size() 
	  && m_plines[i]->get(xfix)==fix 
	  && m_plines[i]->get(xfix2)==fix2
	  && m_plines[i]->get(xfix3)==fix3) ++i;
    if(i>=(int)m_plines.size()) last = m_plines.end();
    else last = numtoiter(m_plines,i);
    std::sort(first,last,comp);
  }
  return(*this);
}
#endif
/////////////////////////////////////////////////////////////////////////////
Collection& Collection::fixsort(int x,int xfix,int xfix2) {
  if(xfix2<0) return(fixsort(x,xfix));
  CompareLine comp(x,CompareLine::Auto);
  iterator first;
  iterator last ;
  value fix,fix2;
  int i=0;
  while(i<(int)m_plines.size()) {
    first = numtoiter(m_plines,i);
    fix = m_plines[i]->get(xfix);
    fix2 = m_plines[i]->get(xfix2);
    while(i<(int)m_plines.size() && m_plines[i]->get(xfix)==fix 
	  && m_plines[i]->get(xfix2)==fix2) ++i;
    if(i>=(int)m_plines.size()) last = m_plines.end();
    else last = numtoiter(m_plines,i);
    std::sort(first,last,comp);
  }
  return(*this);
}
/////////////////////////////////////////////////////////////////////////////
Collection& Collection::fixsort(const string& key,const string& fixkey) {
  int xfix = getx(fixkey);
  if(xfix<0) {
    fprintf(stderr,"Error: dualsort(%s,%s) fix key not found\n"
            ,fixkey.c_str(),key.c_str());
#ifdef __CINTLIB__
    G__printlinenum();
#endif
    return(*this);
  }
  int x = getx(key);
  if(x<0) {
    fprintf(stderr,"Error: dualsort(%s,%s) sort key not found\n"
            ,fixkey.c_str(),key.c_str());
#ifdef __CINTLIB__
    G__printlinenum();
#endif
    return(*this);
  }
  fixsort(x,xfix);
  return(*this);
}
/////////////////////////////////////////////////////////////////////////////
Collection& Collection::fixsort(const string& key,const string& fixkey,const string& fixkey2) {
  int xfix = getx(fixkey);
  int xfix2 = getx(fixkey2);
  if(xfix<0 || xfix2<0) {
    fprintf(stderr,"Error: dualsort(%s,%s,%s) fix key not found\n"
	    ,key.c_str(),fixkey.c_str(),fixkey2.c_str());
#ifdef __CINTLIB__
    G__printlinenum();
#endif
    return(*this);
  }
  int x = getx(key);
  if(x<0) {
    fprintf(stderr,"Error: dualsort(%s,%s,%s) sort key not found\n"
	    ,key.c_str(),fixkey.c_str(),fixkey2.c_str());
#ifdef __CINTLIB__
    G__printlinenum();
#endif
    return(*this);
  }
  fixsort(x,xfix,xfix2);
  return(*this);
}
/////////////////////////////////////////////////////////////////////////////
#ifdef MULTISORTVECTOR
/////////////////////////////////////////////////////////////////////////////
Collection& Collection::multisort(const vector<int> xs,CompareLine::mode_t mode) {
#if   !defined(MULTISORT_FIX2)
  // CAUTION: This algorithm is good only up to 4 xs elements
  CompareLine comp;
  int x=xs[0];
  int xfix=x, xfix2= -1, xfix3= -1;
  if(x<0) return(*this);;
  comp.setrule(x,mode);
  std::sort(m_plines.begin(),m_plines.end(),comp);
  for(unsigned int i=1;i<xs.size() && xs[i]>=0;++i) {
    x=xs[i];
    fixsort(x,xfix,xfix2,xfix3);
    xfix3=xfix2;
    xfix2=xfix;
    xfix=x;
  }
  return(*this);
#elif !defined(MULTISORT_FIX1)
  // CAUTION: This algorithm is good only up to 3 xs elements
  CompareLine comp;
  int x=xs[0];
  int xfix=x, xfix2= -1;
  if(x<0) return(*this);;
  comp.setrule(x,mode);
  std::sort(m_plines.begin(),m_plines.end(),comp);
  for(unsigned int i=1;i<xs.size() && xs[i]>=0;++i) {
    x=xs[i];
    fixsort(x,xfix,xfix2);
    xfix2=xfix;
    xfix=x;
  }
  return(*this);
#else
  // CAUTION: This algorithm is good only up to 2 xs elements
  CompareLine comp;
  int x=xs[0];
  int xfix=x;
  if(x<0) return(*this);;
  comp.setrule(x,mode);
  std::sort(m_plines.begin(),m_plines.end(),comp);
  for(unsigned int i=1;i<xs.size() && xs[i]>=0;++i) {
    x=xs[i];
    fixsort(x,xfix);
    xfix=x;
  }
  return(*this);
#endif
}
/////////////////////////////////////////////////////////////////////////////
Collection& Collection::multisort(int x0,int x1,int x2,int x3,int x4) {
  vector<int> xs;
  if(x0>=0) xs.push_back(x0); 
  if(x1>=0) xs.push_back(x1); 
  if(x2>=0) xs.push_back(x2); 
  if(x3>=0) xs.push_back(x3); 
  if(x4>=0) xs.push_back(x4); 
  multisort(xs);
  return(*this);
}
/////////////////////////////////////////////////////////////////////////////
Collection& Collection::multisort(const string* key) {
  vector<int> xs;
  for(int i=0;i<5;i++) xs.push_back(getx(key[i])); 
  multisort(xs);
  return(*this);
}
/////////////////////////////////////////////////////////////////////////////
Collection& Collection::multisort(char* key[]) {
  vector<int> xs;
  for(int i=0;i<5;i++) xs.push_back(getx(key[i])); 
  multisort(xs);
  return(*this);
}
/////////////////////////////////////////////////////////////////////////////
Collection& Collection::multisort(const string& key0,const string& key1
                          ,const string& key2,const string& key3
			  ,const string& key4) {
  vector<int> xs;
  if(key0!=INVALIDSTR) xs.push_back(getx(key0)); 
  if(key1!=INVALIDSTR) xs.push_back(getx(key1)); 
  if(key2!=INVALIDSTR) xs.push_back(getx(key2)); 
  if(key3!=INVALIDSTR) xs.push_back(getx(key3)); // invalid
  if(key4!=INVALIDSTR) xs.push_back(getx(key4)); // invalid
  multisort(xs);
  return(*this);
}
#else
/////////////////////////////////////////////////////////////////////////////
Collection& Collection::multisort(int xs[]) {
  CompareLine comp;
  int x=xs[0];
  int xfix=x;
  if(x<0) return(*this);;
  comp.setrule(x,CompareLine::Auto);
  std::sort(m_plines.begin(),m_plines.end(),comp);
  for(int i=1;xs[i]>=0;++i) {
    x=xs[i];
    fixsort(x,xfix);
    xfix=x;
  }
  return(*this);
}
/////////////////////////////////////////////////////////////////////////////
Collection& Collection::multisort(int x0,int x1,int x2,int x3,int x4) {
  int xs[6];
  xs[0]=x0; 
  xs[1]=x1;
  xs[2]=x2;
  xs[3]=x3;
  xs[4]=x4; // invalid
  xs[5]= -1;// invalid
  multisort(xs);
  return(*this);
}
/////////////////////////////////////////////////////////////////////////////
Collection& Collection::multisort(const string* key) {
  int xs[6];
  for(int i=0;i<5;i++) xs[i] = getx(key[i]); 
  multisort(xs);
  return(*this);
}
/////////////////////////////////////////////////////////////////////////////
Collection& Collection::multisort(char* key[]) {
  int xs[6];
  for(int i=0;i<5;i++) xs[i] = getx(key[i]); 
  multisort(xs);
  return(*this);
}
/////////////////////////////////////////////////////////////////////////////
Collection& Collection::multisort(const string& key0,const string& key1
                          ,const string& key2,const string& key3
			  ,const string& key4) {
  int xs[6];
  if(key0!=INVALIDSTR) xs[0] = getx(key0); else xs[0] = -1;
  if(key1!=INVALIDSTR) xs[1] = getx(key1); else xs[1] = -1;
  if(key2!=INVALIDSTR) xs[2] = getx(key2); else xs[2] = -1;
  if(key3!=INVALIDSTR) xs[3] = getx(key3); else xs[3] = -1; // invalid
  if(key4!=INVALIDSTR) xs[4] = getx(key4); else xs[4] = -1; // invalid
  xs[5] = -1;
  multisort(xs);
  return(*this);
}
#endif
/////////////////////////////////////////////////////////////////////////////
Collection Collection::sortstat(int xsort,int xstat) {
  // Sort
  sort(xsort);

  // Set key line
  Line line;
  value val;
  if(p_key) val = p_key->get(xsort);
  line.push_back(val);
  val = "sigma"; line.push_back(val);
  val = "max"; line.push_back(val);
  val = "min"; line.push_back(val);
  val = "mean"; line.push_back(val);
  val = "nsample"; line.push_back(val);
  val = "stddev"; line.push_back(val);
  Collection s;
  p_sheet->push_back(line);
  s.push_back(&p_sheet->back());
  s.setkey(0);

  //
  const_iterator first;
  const_iterator last ;
  int xfix = xsort;
  value fix;
  int i=0;
  while(i<(int)m_plines.size()) {
    // Idendify fixed item range first..last
    fix = m_plines[i]->get(xfix);
    first = numtoiter(m_plines,i);
    while(i<(int)m_plines.size() && m_plines[i]->get(xfix)==fix) ++i;
    if(i>=(int)m_plines.size()) last = m_plines.end();
    else last = numtoiter(m_plines,i);

    // statistic calculation
    stat(first,last);

    // set value 
    line.init();
    line.push_back(fix);
    val=getstat().get(xstat).sigma(); line.push_back(val);
    val=getstat().get(xstat).max(); line.push_back(val);
    val=getstat().get(xstat).min(); line.push_back(val);
    val=getstat().get(xstat).mean(); line.push_back(val);
    val=getstat().get(xstat).nsample(); line.push_back(val);
    val=getstat().get(xstat).stddev(); line.push_back(val);
    p_sheet->push_back(line);
    s.push_back(&p_sheet->back());
  }

  return(s);
}
/////////////////////////////////////////////////////////////////////////////
Collection Collection::sortstat(const string& sortkey,const string& statkey) {
  int xsort = getx(sortkey);
  if(xsort<0) {
    fprintf(stderr,"Error: sortstat(%s,%s) sortkey not found\n"
            ,sortkey.c_str(),statkey.c_str());
#ifdef __CINTLIB__
    G__printlinenum();
#endif
    return(Collection(p_sheet));
  }
  int xstat = getx(statkey);
  if(xstat<0) {
    fprintf(stderr,"Error: sortstat(%s,%s) statkey not found\n"
            ,sortkey.c_str(),statkey.c_str());
#ifdef __CINTLIB__
    G__printlinenum();
#endif
    return(Collection(p_sheet));
  }
  return(sortstat(xsort,xstat));
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
Collection Collection::allignkey(int x,int extra) {
  //copy original matrix for keeping misc columns info remains

  // copy column by column
  int xold;
  int i;
  string name;
  vector<int> ind;
  Line line;
  ind.push_back(x);
  for(i=0;i<size();i++) {
    name = get(i)[x].tostring();
    xold = getx(name);
    ind.push_back(xold);
  }
  xold = getx("class");
  if(xold>=0) ind.push_back(xold);
  if(extra) {
    for(i=xold+1;i<getkey().size();i++) ind.push_back(i);
  }

  Collection r;
  line = getkey().column(ind);
  r.push_back(line);
  r.setkey(0);
  for(i=0;i<size();i++) {
    line = get(i).column(ind);
    r.push_back(line);
  }
  return(r);
}

/////////////////////////////////////////////////////////////////////////////
int Collection::gety(Line* p) {
  for(int i=0;i<(int)m_plines.size();++i) if(p==m_plines[i]) return(i);
  return(-1);
}

/////////////////////////////////////////////////////////////////////////////
int Collection::getkeyy(int x,const string& key) {
  Collection candidates = match(x,key);
  if(candidates.size()==0) return(-1);
  if(candidates.size()==1) return(gety(&candidates[0]));
  return(gety(&candidates[0]));
}
/////////////////////////////////////////////////////////////////////////////
int Collection::findy(int x,const string& str,int n) {
  Collection candidates = match(x,str);
  if(candidates.size()<n) return(-1);
  return(gety(&candidates[n-1]));
}
/////////////////////////////////////////////////////////////////////////////
int Collection::findy(const string& key,const string& str,int n) {
  Collection candidates = match(key,str);
  if(candidates.size()<n) return(-1);
  return(gety(&candidates[n-1]));
}

#ifdef MULTIPLEKEYS
/////////////////////////////////////////////////////////////////////////////
vector<int> Collection::findys(int x,const string& str) {
  Collection candidates = match(x,str);
  vector<int> ys;
  for(int i=0;i<candidates.size();i++) ys.push_back(gety(&candidates[i]));
  return(ys);
}
/////////////////////////////////////////////////////////////////////////////
vector<int> Collection::findys(const vector<int>& xs,const string& str) {
  Collection candidates = match(xs,str);
  vector<int> ys;
  for(int i=0;i<candidates.size();i++) ys.push_back(gety(&candidates[i]));
  return(ys);
}
/////////////////////////////////////////////////////////////////////////////
vector<int> Collection::findys(const string& key,const string& str) {
  Collection candidates = match(key,str);
  vector<int> ys;
  for(int i=0;i<candidates.size();i++) ys.push_back(gety(&candidates[i]));
  return(ys);
}
#endif


/////////////////////////////////////////////////////////////////////////////
void Collection::setkey(int keyy) {
  if(keyy<0 || (int)m_plines.size()<keyy) {
  }
  deque<Line*>::iterator first=m_plines.begin();
  deque<Line*>::iterator last =m_plines.end();
  int y=0;
  while(first!=last) {
    if(y==keyy) {
      p_key = *first;
      m_plines.erase(first);
      return;
    }
    ++y;
    ++first;
  }
}

/////////////////////////////////////////////////////////////////////////////
void Collection::setkey2(int keyy) {
  if(keyy<0 || (int)m_plines.size()<keyy) {
  }
  deque<Line*>::iterator first=m_plines.begin();
  deque<Line*>::iterator last =m_plines.end();
  int y=0;
  while(first!=last) {
    if(y==keyy) {
      p_key2 = *first;
      m_plines.erase(first);
      return;
    }
    ++y;
    ++first;
  }
}
/////////////////////////////////////////////////////////////////////////////
int Collection::maxlinesize() const  {
  int max=0,tmp;
  for(int i=0;i<size();i++) {
    tmp = get(i).size();
    if(tmp>max) max=tmp;
  }
  return(max);
}
/////////////////////////////////////////////////////////////////////////////
Collection Collection::rotate(int key) /* const */ {
  Collection r;
  Line l;
  int i,j;
  int max = maxlinesize();
  if(key && getkey().size()==0) key=0;
  for(j=0;j<max;j++) {
    l.clear();
    if(key) l.let(0,getkey()[j].tostring()); // ??????
    for(i=0;i<size();i++) {
      //l.let(i,get(i)[j].tostring());
      l.let(i+key,get(i)[j].tostring()); // ???? 
    }
    r.push_back(l);
  }
  if(key) r.setkey(0);
  return(r);
}

/////////////////////////////////////////////////////////////////////////////
CollectionStatistics& Collection::stat() {
  initstat();
  for(const_iterator iy=m_plines.begin();iy!=m_plines.end();++iy) {
    Line& line = *(*iy);
    line.stat(m_stat);
  }
  return(m_stat);
}

/////////////////////////////////////////////////////////////////////////////
CollectionStatistics& Collection::stat(const_iterator first,const_iterator last) {
  initstat();
  for(const_iterator iy=first;iy!=last;++iy) {
    Line& line = *(*iy);
    line.stat(m_stat);
  }
  return(m_stat);
}

/////////////////////////////////////////////////////////////////////////////
CollectionStatistics Collection::linestat(int xfrom,int xto) {
  CollectionStatistics cs;
  statistics s;
  int j;
  if(xto<0) xto = getkey().size()-1;
  for(const_iterator iy=m_plines.begin();iy!=m_plines.end();++iy) {
    Line& line = *(*iy);
    s.clear();
    for(j=xfrom;j<=xto&&j<line.size();j++) 
      if(line[j].isdouble()) s.add(line[j].todouble());
    cs.push_back(s);
  }
  return(cs);
}

/////////////////////////////////////////////////////////////////////////////
void Collection::disp(FILE* fp,int statmode) const {
  if(p_key) p_key->disp(fp);
  if(p_key2) p_key2->disp(fp);
  for(const_iterator iy=m_plines.begin();iy!=m_plines.end();++iy) {
    Line& line = *(*iy);
    line.disp(fp);
  }
  if(m_stat.size()) {
    //if(p_key) p_key->disp(fp);
    if(statmode>=1) m_stat.dispmean(fp);    
    if(statmode>=2) m_stat.dispmax(fp);     
    if(statmode>=3) m_stat.dispmin(fp);     
    if(statmode>=4) m_stat.dispstddev(fp);  
    if(statmode>=5) m_stat.dispnsample(fp); 
    if(statmode>=6) m_stat.dispsigma(fp);   
    if(statmode>=7) m_stat.disperror(fp);   
  }
}

/////////////////////////////////////////////////////////////////////////////
void Collection::_disp(FILE* fp,int statmode) const {
  for(const_iterator iy=m_plines.begin();iy!=m_plines.end();++iy) {
    Line& line = *(*iy);
    line.disp(fp);
  }
  if(m_stat.size()) {
    //if(p_key) p_key->disp(fp);
    if(statmode>=1) m_stat.dispmean(fp);    
    if(statmode>=2) m_stat.dispmax(fp);     
    if(statmode>=3) m_stat.dispmin(fp);     
    if(statmode>=4) m_stat.dispstddev(fp);  
    if(statmode>=5) m_stat.dispnsample(fp); 
    if(statmode>=6) m_stat.dispsigma(fp);   
    if(statmode>=7) m_stat.disperror(fp);   
  }
}

/////////////////////////////////////////////////////////////////////////////
void Collection::disp2(FILE* fp,int flag) const {
  Line key;
  Line line;
  if(p_key) key = *p_key;
  if(p_key2) key = *p_key2;
  for(const_iterator iy=m_plines.begin();iy!=m_plines.end();++iy) {
    Line& line = *(*iy);
    for(int i=0;i<line.size();i++) {
      if(flag==0) {
	if(i<key.size()) fprintf(fp,"%s=",key[i].c_str());
	line[i].disp(fp);
      }
      else if(line[i].tostring().size()) {
	if(i<key.size()) fprintf(fp,"%s=",key[i].c_str());
	line[i].disp(fp);
      }
      else if(flag==1) {
	fprintf(fp,",");
      }
      else {
      }
    }
    fprintf(fp,"\n");
  }
}

/////////////////////////////////////////////////////////////////////////////
void Collection::disp3(FILE* fp,int flag) const {
  Line key;
  Line line;
  if(p_key) key = *p_key;
  if(p_key2) key = *p_key2;
  for(const_iterator iy=m_plines.begin();iy!=m_plines.end();++iy) {
    Line& line = *(*iy);
    for(int i=0;i<line.size();i++) {
      if(!line[i].size()) continue;
      if(flag==0) {
	if(i<key.size()) fprintf(fp,"%s=",key[i].c_str());
	line[i].disp(fp);
      }
      else if(line[i].tostring().size()) {
	if(i<key.size()) fprintf(fp,"%s=",key[i].c_str());
	line[i].disp(fp);
      }
      else if(flag==1) {
	fprintf(fp,",");
      }
      else {
      }
    }
    fprintf(fp,"\n");
  }
}

/////////////////////////////////////////////////////////////////////////////
void Collection::dispform(FILE* fp,int more,int keylength) const {
  if(!fp) fp=stdout;
  vector<int> len;
  int j;
  /////////////////////////////////////////////////////////////
  // Check max column length
  if(keylength) {
    // Check max column length of key line, if flag is set
    len.resize(getkey().size());
    for(j=0;j<getkey().size();j++) {
      if(getkey()[j].size()>len[j]) len[j] = getkey()[j].size();
    }
  }
  // Check max column length of contents
  const_iterator iy;
  for(iy=m_plines.begin();iy!=m_plines.end();++iy) {
    Line& line = *(*iy);
    if(len.size()<line.size()) len.resize(line.size());
    for(j=0;j<line.size();j++) if(line[j].size()>len[j]) len[j]=line[j].size();
  }

  /////////////////////////////////////////////////////////////
  // print key line
  int i=0;
  char fmt[50];
  int indent=0,indentflag=0;
  for(j=0;j<getkey().size();j++) {
    if(indentflag) {
      sprintf(fmt,"\n%%-%ds,",indent-1);
      fprintf(fp,fmt,"");
      indentflag=0;
    }
    sprintf(fmt,"%%-%ds,",len[j]);
    fprintf(fp,fmt,getkey()[j].c_str());
    if(getkey()[j].size()>len[j]) indentflag=1; // need improvement 
    indent += len[j]+1;
  }
  fprintf(fp,"\n");

  // print contents
  char *p=0;
  int store_more=0;
  string pattern;
  for(iy=m_plines.begin();iy!=m_plines.end();++iy) {
    Line& line = *(*iy);
    for(j=0;j<line.size();j++) {
      sprintf(fmt,"%%-%ds,",len[j]);
      fprintf(fp,fmt,line[j].c_str());
      if(pattern.size()>0 && strstr(line[j].c_str(),pattern.c_str())) {
	pattern="";
	more = store_more;
	i=more;
      }
    }
    fprintf(fp,"\n");
    if(more && ++i>more) {
      i=0;
      char com[50];
      fprintf(fp,"more>");
      /* scanf("%s",com); */
      fgets(com,50,stdin);
      switch(tolower(com[0])) {
      case 'q': return;
      case 'c': more=1000000; break;
      case '/': store_more=more; more=1000000; 
	p=strchr(com,'\n'); if(p) *p=0; pattern=com+1; 
	break;
      default: if(isdigit(com[0])) more = atoi(com); break;
      }
    }
  }
}

/////////////////////////////////////////////////////////////////////////////
void Collection::disp(const char* fname,const char* mode,int statmode) const {
  FILE *fp = fopen(fname,mode);
  if(!fp) {
    fprintf(stderr,"Error: Cannot create output file %s\n",fname);
    return;
  }
  disp(fp,statmode);
  fclose(fp);
}

/////////////////////////////////////////////////////////////////////////////
void Collection::disp2(const char* fname,const char* mode,int flag) const {
  FILE *fp = fopen(fname,mode);
  if(!fp) {
    fprintf(stderr,"Error: Cannot create output file %s\n",fname);
    return;
  }
  disp2(fp,flag);
  fclose(fp);
}

/////////////////////////////////////////////////////////////////////////////
void Collection::disp3(const char* fname,const char* mode,int flag) const {
  FILE *fp = fopen(fname,mode);
  if(!fp) {
    fprintf(stderr,"Error: Cannot create output file %s\n",fname);
    return;
  }
  disp3(fp,flag);
  fclose(fp);
}

/////////////////////////////////////////////////////////////////////////////
void Collection::dispform(const char* fname,const char* mode,int keylength) const {
  FILE *fp = fopen(fname,mode);
  if(!fp) {
    fprintf(stderr,"Error: Cannot create output file %s\n",fname);
    return;
  }
  dispform(fp,0,keylength);
  fclose(fp);
}

/////////////////////////////////////////////////////////////////////////////
void Collection::more(int s) const {
  FILE *fp=stdout;
  if(p_key) p_key->disp(fp);
  if(p_key2) p_key2->disp(fp);
  int i=0;

  char *p=0;
  int store_more=0;
  string pattern;
  for(const_iterator iy=m_plines.begin();iy!=m_plines.end();++iy) {
    Line& line = *(*iy);
    line.disp(fp);
    if(pattern.size() && strstr(line[0].c_str(),pattern.c_str())) {
      pattern="";
      s = store_more;
      i=s;
    }
    if(++i>s) {
      i=0;
      char com[50];
      fprintf(fp,"more>");
      /* scanf("%s",com); */
      fgets(com,50,stdin);
      switch(tolower(com[0])) {
      case 'q': return;
      case 'c': s=1000000; break;
      case '/': store_more=s; s=1000000; 
	p=strchr(com,'\n'); if(p) *p=0; pattern=com+1; 
	break;
      default: if(isdigit(com[0])) s = atoi(com); break;
      }
    }
  }
}

/////////////////////////////////////////////////////////////////////////////
void Collection::print(const string& fname,const string& mode,int statmode) {
  FILE* fp=fopen(fname.c_str(),mode.c_str());
  if(!fp) {
    fprintf(stderr,"Error: %s can not be opened\n",fname.c_str());
#ifdef __CINTLIB__
    G__printlinenum();
#endif
    return;
  }
  disp(fp,statmode);
  fprintf(fp,"\n");
  fclose(fp);
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
Collection& Collection::column_push_front(double d) {
  value val = d;
  return(column_push_front(val));
}
/////////////////////////////////////////////////////////////////////////////
Collection& Collection::column_push_back(double d) {
  value val = d;
  return(column_push_back(val));
}
/////////////////////////////////////////////////////////////////////////////
Collection& Collection::column_push_front(const string& s) {
  value val = s;
  return(column_push_front(val));
}
/////////////////////////////////////////////////////////////////////////////
Collection& Collection::column_push_back(const string& s) {
  value val = s;
  return(column_push_back(val));
}
/////////////////////////////////////////////////////////////////////////////
Collection& Collection::column_push_front(const value& val) {
  return(column_push_front("",val));
}
/////////////////////////////////////////////////////////////////////////////
Collection& Collection::column_push_back(const value& val) {
  return(column_push_back("",val));
}
/////////////////////////////////////////////////////////////////////////////
Collection& Collection::column_push_front(const string& key,double d) {
  value val=d;
  return(column_push_front(key,val));
}
/////////////////////////////////////////////////////////////////////////////
Collection& Collection::column_push_back(const string& key,double d) {
  value val=d;
  return(column_push_back(key,val));
}
/////////////////////////////////////////////////////////////////////////////
Collection& Collection::column_push_front(const string& key,const string& s) {
  value val=s;
  return(column_push_front(key,val));
}
/////////////////////////////////////////////////////////////////////////////
Collection& Collection::column_push_back(const string& key,const string& s) {
  value val=s;
  return(column_push_back(key,val));
}
/////////////////////////////////////////////////////////////////////////////
Collection& Collection::column_push_front(const string& key,const value& val) {
  int n=size();
#ifdef LINE_PERSISTENCY
  Line line;
  if(!p_sheet)  p_sheet->push_back(line);
#endif
  for(int i=0;i<n;i++) {
#ifdef LINE_PERSISTENCY
    line = *m_plines[i];
    line.push_front(val);
    p_sheet->push_back(line);
    m_plines[i] = &p_sheet->back();
#else
    m_plines[i]->push_front(val);
#endif
  }
  if(p_key) {
    value vkey = key;
#ifdef LINE_PERSISTENCY
    line = *p_key;
    line.push_front(vkey);
    p_sheet->push_back(line);
    p_key = &p_sheet->back();
#else
    p_key->push_front(vkey);
#endif
  }
  return(*this);
}
/////////////////////////////////////////////////////////////////////////////
Collection& Collection::column_push_back(const string& key,const value& val) {
  int n=size();
#ifdef LINE_PERSISTENCY
  Line line;
  if(!p_sheet)  p_sheet->push_back(line);
#endif
  for(int i=0;i<n;i++) {
#ifdef LINE_PERSISTENCY
    line = *m_plines[i];
    line.push_back(val);
    p_sheet->push_back(line);
    m_plines[i] = &p_sheet->back();
#else
    m_plines[i]->push_back(val);
#endif
  }
  if(p_key) {
    value vkey = key;
#ifdef LINE_PERSISTENCY
    line = *p_key;
    line.push_back(vkey);
    p_sheet->push_back(line);
    p_key = &p_sheet->back();
#else
    p_key->push_back(vkey);
#endif
  }
  return(*this);
}
/////////////////////////////////////////////////////////////////////////////
Collection& Collection::column_push_front(const Line& key,const Line& val) {
  int n=size();
#ifdef LINE_PERSISTENCY
  Line line;
  if(!p_sheet)  p_sheet->push_back(line);
#endif
  for(int i=0;i<n;i++) {
#ifdef LINE_PERSISTENCY
    line = *m_plines[i];
    line.push_front(val);
    p_sheet->push_back(line);
    m_plines[i] = &p_sheet->back();
#else
    m_plines[i]->push_front(val);
#endif
  }
  if(p_key) {
#ifdef LINE_PERSISTENCY
    line = *p_key;
    line.push_front(key);
    p_sheet->push_back(line);
    p_key = &p_sheet->back();
#else
    p_key->push_front(key);
#endif
  }
  return(*this);
}
/////////////////////////////////////////////////////////////////////////////
Collection& Collection::column_push_back(const Line& key,const Line& val) {
  int n=size();
#ifdef LINE_PERSISTENCY
  Line line;
  if(!p_sheet)  p_sheet->push_back(line);
#endif
  for(int i=0;i<n;i++) {
#ifdef LINE_PERSISTENCY
    line = *m_plines[i];
    line.push_back(val);
    p_sheet->push_back(line);
    m_plines[i] = &p_sheet->back();
#else
    m_plines[i]->push_back(val);
#endif
  }
  if(p_key) {
#ifdef LINE_PERSISTENCY
    line = *p_key;
    line.push_back(key);
    p_sheet->push_back(line);
    p_key = &p_sheet->back();
#else
    p_key->push_back(key);
#endif
  }
  return(*this);
}
/////////////////////////////////////////////////////////////////////////////
Collection& Collection::column_merge_front(const Collection& a) {
  int n=::min(size(),a.size());  
  //for(int i=0;i<n;i++) m_plines[i]->push_front(*a.m_plines[i]);
  //if(p_key && a.p_key) p_key->push_front(*a.p_key);
#ifdef LINE_PERSISTENCY
  Line line;
#endif
  int i;
  if(!p_sheet) p_sheet = a.p_sheet;
  for(i=0;i<n;i++) {
#ifdef LINE_PERSISTENCY
    line = *m_plines[i];
    line.push_front(*a.m_plines[i]);
    p_sheet->push_back(line);
    m_plines[i] = &p_sheet->back();
#else
    m_plines[i]->push_front(*a.m_plines[i]);
#endif
  }
  for(i=n;i<a.size();i++) m_plines.push_back(a.m_plines[i]);
  if(a.p_key) {
#ifdef LINE_PERSISTENCY
    if(p_key) line = *p_key;
    else      line.clear();
    line.push_front(*a.p_key);
    p_sheet->push_back(line);
    p_key = &p_sheet->back();
#else
    if(!p_key) {
      if(!p_sheet) p_sheet = a.p_sheet;
      p_sheet->push_back(Line());
      p_key = &p_sheet->back();
    }
    p_key->push_front(*a.p_key);
#endif
  }
  return(*this);
}
/////////////////////////////////////////////////////////////////////////////
Collection& Collection::column_merge_back(const Collection& a) {
  int n=::min(size(),a.size());
  //for(int i=0;i<n;i++) m_plines[i]->push_back(*a.m_plines[i]);
  //if(p_key && a.p_key) p_key->push_back(*a.p_key);
#ifdef LINE_PERSISTENCY
  Line line;
#endif
  int i;
  if(!p_sheet) p_sheet = a.p_sheet;
  for(i=0;i<n;i++) {
#ifdef LINE_PERSISTENCY
    line = *m_plines[i];
    line.push_back(*a.m_plines[i]);
    p_sheet->push_back(line);
    m_plines[i] = &p_sheet->back();
#else
    m_plines[i]->push_back(*a.m_plines[i]);
#endif
  }
  for(i=n;i<a.size();i++) m_plines.push_back(a.m_plines[i]);
  if(a.p_key) {
#ifdef LINE_PERSISTENCY
    if(p_key) line = *p_key;
    else      line.clear();
    line.push_back(*a.p_key);
    p_sheet->push_back(line);
    p_key = &p_sheet->back();
#else
    if(!p_key) {
      p_sheet->push_back(Line());
      p_key = &p_sheet->back();
    }
    p_key->push_back(*a.p_key);
#endif
  }
  return(*this);
}
/////////////////////////////////////////////////////////////////////////////
int Collection::column_erase(const string& key) {
  int x=getx(key); 
  if(x<0) {
    fprintf(stderr,"Error: erase(%s) key not found\n",key.c_str());
#ifdef __CINTLIB__
    G__printlinenum();
#endif
    return(1);
  }
  return(column_erase(x));
}
/////////////////////////////////////////////////////////////////////////////
int Collection::column_erase(const string& key1,const string& key2) {
  int x1=getx(key1); 
  if(x1<0) {
    fprintf(stderr,"Error: erase(%s,%s) key %s not found\n"
     ,key1.c_str(),key2.c_str(),key1.c_str());
#ifdef __CINTLIB__
    G__printlinenum();
#endif
    return(1);
  }
  int x2=getx(key2); 
  if(x2<0) {
    fprintf(stderr,"Error: erase(%s,%s) key %s not found\n"
     ,key1.c_str(),key2.c_str(),key2.c_str());
#ifdef __CINTLIB__
    G__printlinenum();
#endif
    return(1);
  }
  return(column_erase(x1,x2)); 
}

/////////////////////////////////////////////////////////////////////////////
int Collection::column_erase(int x) {
  int stat=0;
  iterator first=m_plines.begin();
  iterator last =m_plines.end();
#ifdef LINE_PERSISTENCY
  Line line;
#endif
  while(first!=last) {
#ifdef LINE_PERSISTENCY
    line = *(*first);
    stat+=line.erase(x);
    p_sheet->push_back(line);
#else
    stat+=(*first)->erase(x);
#endif
    ++first;
  }
  if(p_key) {
#ifdef LINE_PERSISTENCY
    line = *p_key;
    stat+=line.erase(x);
    p_sheet->push_back(line);
    p_key = &p_sheet->back();
#else
    stat+=p_key->erase(x);
#endif
  }
  stat+=m_stat.erase(x);
  return(stat);
}
/////////////////////////////////////////////////////////////////////////////
int Collection::column_erase(int x1,int x2) {
  int stat=0;
  iterator first=m_plines.begin();
  iterator last =m_plines.end();
#ifdef LINE_PERSISTENCY
  Line line;
#endif
  while(first!=last) {
#ifdef LINE_PERSISTENCY
    line = *(*first);
    stat+=line.erase(x1,x2);
    p_sheet->push_back(line);
#else
    stat+=(*first)->erase(x1,x2);
#endif
    ++first;
  }
  if(p_key) {
#ifdef LINE_PERSISTENCY
    line = *p_key;
    stat+=line.erase(x1,x2);
    p_sheet->push_back(line);
    p_key = &p_sheet->back();
#else
    stat+=p_key->erase(x1,x2);
#endif
  }
  stat+=m_stat.erase(x1,x2);
  //return(stat);
  return(0);
}
/////////////////////////////////////////////////////////////////////////////
int Collection::column_insert(int after,const string& key,const string& val) {
  column_insert(after);
  getkey().let(after+1,key);
  let(after+1,val);
  return(0);
}

/////////////////////////////////////////////////////////////////////////////
int Collection::column_insert(int after,const string& key,double val) {
  column_insert(after);
  getkey().let(after+1,key);
  let(after+1,val);
  return(0);
}

/////////////////////////////////////////////////////////////////////////////
int Collection::column_insert(int after,const string& val) {
  column_insert(after);
  let(after+1,val);
  return(0);
}

/////////////////////////////////////////////////////////////////////////////
int Collection::column_insert(int after,double val) {
  column_insert(after);
  let(after+1,val);
  return(0);
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
int Collection::column_insert(int after) {
  Line& keyline=getkey();
  Collection t1 = column(0,after);;
  Collection t2 = column(after+1,keyline.size()-1);
  t1.column_push_back("");
  t1.column_push_back(t2);
  *this = t1;
  return(0);
}

/////////////////////////////////////////////////////////////////////////////
Collection Collection::duplicate() const {
  Collection c;
#ifdef DUPLICATE
  c.p_sheet = new Sheet;
  c.m_isauto=0;
#else
  c.p_sheet = p_sheet;
  c.m_isauto=0;
#endif
  if(p_key) {
    c.p_sheet->push_back(*p_key);
    c.p_key = &p_sheet->back();
  }
  for(int i=0;i<size();i++) {
    c.p_sheet->push_back(*m_plines[i]);
    c.m_plines.push_back(&p_sheet->back());
  }
  c.m_stat=m_stat;
  return(c);
}
/////////////////////////////////////////////////////////////////////////////
Collection& Collection::duplicate(const Collection& col) {
  clear();
#ifdef DUPLICATE
  p_sheet = new Sheet;
  m_isauto=0;
#else
  p_sheet = col.p_sheet;
  m_isauto=0;
#endif
  if(col.p_key) {
    p_sheet->push_back(*col.p_key);
    p_key = &p_sheet->back();
  }
  Line line;
  for(int i=0;i<col.size();i++) {
    p_sheet->push_back(*col.m_plines[i]);
    m_plines.push_back(&p_sheet->back());
  }
  m_stat=col.m_stat;
  return(*this);
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
int Collection::maxstrlen(int x) {
  iterator first=m_plines.begin();
  iterator last =m_plines.end();
  int len=0,tmp;
  if(x<0) return(0);
  while(first!=last) {
    tmp = strlen((*first)->get(x).c_str());
    if(tmp>len) len=tmp;
    ++first;
  }
  return(len);
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
Collection& Collection::add(const Collection& a) {
  int i;
  for(i=0;i<size()&&i<a.size();i++) get(i).add(a[i]);
  return(*this);
}
/////////////////////////////////////////////////////////////////////////////
Collection& Collection::sub(const Collection& a) {
  int i;
  for(i=0;i<size()&&i<a.size();i++) get(i).sub(a[i]);
  return(*this);
}
/////////////////////////////////////////////////////////////////////////////
Collection& Collection::mul(const Collection& a) {
  int i;
  for(i=0;i<size()&&i<a.size();i++) get(i).mul(a[i]);
  return(*this);
}
/////////////////////////////////////////////////////////////////////////////
Collection& Collection::div(const Collection& a) {
  int i;
  for(i=0;i<size()&&i<a.size();i++) get(i).div(a[i]);
  return(*this);
}
/////////////////////////////////////////////////////////////////////////////
Collection& Collection::mod(const Collection& a) {
  int i;
  for(i=0;i<size()&&i<a.size();i++) get(i).mod(a[i]);
  return(*this);
}
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
Collection& Collection::add(int x,double a) {
  iterator first=m_plines.begin();
  iterator last =m_plines.end();
  while(first!=last) {
    (*first)->add(x,a);
    ++first;
  }
  return(*this);
}
/////////////////////////////////////////////////////////////////////////////
Collection& Collection::sub(int x,double a) {
  iterator first=m_plines.begin();
  iterator last =m_plines.end();
  while(first!=last) {
    (*first)->sub(x,a);
    ++first;
  }
  return(*this);
}
/////////////////////////////////////////////////////////////////////////////
Collection& Collection::mul(int x,double a) {
  iterator first=m_plines.begin();
  iterator last =m_plines.end();
  while(first!=last) {
    (*first)->mul(x,a);
    ++first;
  }
  return(*this);
}
/////////////////////////////////////////////////////////////////////////////
Collection& Collection::div(int x,double a) {
  iterator first=m_plines.begin();
  iterator last =m_plines.end();
  while(first!=last) {
    (*first)->div(x,a);
    ++first;
  }
  return(*this);
}
/////////////////////////////////////////////////////////////////////////////
Collection& Collection::mod(int x,double a) {
  iterator first=m_plines.begin();
  iterator last =m_plines.end();
  while(first!=last) {
    (*first)->div(x,a);
    ++first;
  }
  return(*this);
}
/////////////////////////////////////////////////////////////////////////////
Collection& Collection::limit(int x,double lower,double upper,double lim) {
  for(const_iterator iy=m_plines.begin();iy!=m_plines.end();++iy) {
    Line& p = *(*iy);
    const value& v = p[x];
    if(lower>v.d) p[x] = lower*lim;
    if(upper<v.d) p[x] = upper*lim;
  }
  return(*this);
}
#ifdef MULTIPLEKEYS
/////////////////////////////////////////////////////////////////////////////
Collection& Collection::limit(const vector<int>& xs,double lower,double upper,double lim) {
  for(const_iterator iy=m_plines.begin();iy!=m_plines.end();++iy) {
    Line& p = *(*iy);
    vector<int>::const_iterator ifirst = xs.begin();
    vector<int>::const_iterator ilast  = xs.end();
    while(ifirst!=ilast) {
     int x = *ifirst++;
      const value& v = p[x];
      if(lower>v.d) p[x] = lower*lim;
      if(upper<v.d) p[x] = upper*lim;
    }
  }
  return(*this);
}
/////////////////////////////////////////////////////////////////////////////
#endif

/////////////////////////////////////////////////////////////////////////////
Collection& Collection::add(const string& key,double a) {
  int x=getx(key); 
  if(x<0) {
    fprintf(stderr,"Error: add(%s,%g) key not found\n",key.c_str(),a);
#ifdef __CINTLIB__
    G__printlinenum();
#endif
    return(*this);
  }
  return(add(x,a)); 
}
/////////////////////////////////////////////////////////////////////////////
Collection& Collection::sub(const string& key,double a) {
  int x=getx(key); 
  if(x<0) {
    fprintf(stderr,"Error: sub(%s,%g) key not found\n",key.c_str(),a);
#ifdef __CINTLIB__
    G__printlinenum();
#endif
    return(*this);
  }
  return(sub(x,a)); 
}
/////////////////////////////////////////////////////////////////////////////
Collection& Collection::mul(const string& key,double a) {
  int x=getx(key); 
  if(x<0) {
    fprintf(stderr,"Error: mul(%s,%g) key not found\n",key.c_str(),a);
#ifdef __CINTLIB__
    G__printlinenum();
#endif
    return(*this);
  }
  return(mul(x,a)); 
}
/////////////////////////////////////////////////////////////////////////////
Collection& Collection::div(const string& key,double a) {
  int x=getx(key); 
  if(x<0) {
    fprintf(stderr,"Error: div(%s,%g) key not found\n",key.c_str(),a);
#ifdef __CINTLIB__
    G__printlinenum();
#endif
    return(*this);
  }
  return(div(x,a)); 
}
/////////////////////////////////////////////////////////////////////////////
Collection& Collection::mod(const string& key,double a) {
  int x=getx(key); 
  if(x<0) {
    fprintf(stderr,"Error: mod(%s,%g) key not found\n",key.c_str(),a);
#ifdef __CINTLIB__
    G__printlinenum();
#endif
    return(*this);
  }
  return(mod(x,a)); 
}
/////////////////////////////////////////////////////////////////////////////
Collection& Collection::limit(const string& key,double lower,double upper,double lim) {
#ifdef MULTIPLEKEYS
  vector<int> xs=getxs(key);
  if(xs.size()==0) {
    fprintf(stderr,"Error: limit(%s,%g,%g,%g) key not found\n"
	    ,key.c_str(),lower,upper,lim);
#ifdef __CINTLIB__
    G__printlinenum();
#endif
    return(*this);
  }
  else if(xs.size()==1) return(limit(xs[0],lower,upper,lim));
  else return(limit(xs,lower,upper,lim));
#else // MULTIPLEKEYS
  int x=getx(key);
  if(x<0) {
    fprintf(stderr,"Error: limit(%s,%g,%g,%g) key not found\n"
	    ,key.c_str(),lower,upper,lim);
#ifdef __CINTLIB__
    G__printlinenum();
#endif
    return(*this);
  }
  return(limit(x,lower,upper,lim));
#endif // MULTIPLEKEYS
}

/////////////////////////////////////////////////////////////////////////////
Collection& Collection::add(int x,int x2,int xo) {
  iterator first=m_plines.begin();
  iterator last =m_plines.end();
  while(first!=last) {
    (*first)->add(x,x2,xo);
    ++first;
  }
  return(*this);
}
/////////////////////////////////////////////////////////////////////////////
Collection& Collection::sub(int x,int x2,int xo) {
  iterator first=m_plines.begin();
  iterator last =m_plines.end();
  while(first!=last) {
    (*first)->sub(x,x2,xo);
    ++first;
  }
  return(*this);
}
/////////////////////////////////////////////////////////////////////////////
Collection& Collection::mul(int x,int x2,int xo) {
  iterator first=m_plines.begin();
  iterator last =m_plines.end();
  while(first!=last) {
    (*first)->mul(x,x2,xo);
    ++first;
  }
  return(*this);
}
/////////////////////////////////////////////////////////////////////////////
Collection& Collection::div(int x,int x2,int xo) {
  iterator first=m_plines.begin();
  iterator last =m_plines.end();
  while(first!=last) {
    (*first)->div(x,x2,xo);
    ++first;
  }
  return(*this);
}
/////////////////////////////////////////////////////////////////////////////
Collection& Collection::mod(int x,int x2,int xo) {
  iterator first=m_plines.begin();
  iterator last =m_plines.end();
  while(first!=last) {
    (*first)->mod(x,x2,xo);
    ++first;
  }
  return(*this);
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
Collection& Collection::add(const string& x,const string& x2,const string& xo) {
  int ix=getx(x); 
  int ix2=getx(x2); 
  int ixo=getx(xo); 
  if(ix<0 || ix2<0 || ixo<0 ) {
    fprintf(stderr,"Error: add(%s,%s,%s) %d,%d,%d key not found\n"
	    ,x.c_str() ,x2.c_str() ,xo.c_str(),ix,ix2,ixo);
#ifdef __CINTLIB__
    G__printlinenum();
#endif
    return(*this);
  }
  return(add(ix,ix2,ixo)); 
}
/////////////////////////////////////////////////////////////////////////////
Collection& Collection::sub(const string& x,const string& x2,const string& xo) {
  int ix=getx(x); 
  int ix2=getx(x2); 
  int ixo=getx(xo); 
  if(ix<0 || ix2<0 || ixo<0 ) {
    fprintf(stderr,"Error: sub(%s,%s,%s) %d,%d,%d key not found\n"
	    ,x.c_str() ,x2.c_str() ,xo.c_str(),ix,ix2,ixo);
#ifdef __CINTLIB__
    G__printlinenum();
#endif
    return(*this);
  }
  return(sub(ix,ix2,ixo)); 
}
/////////////////////////////////////////////////////////////////////////////
Collection& Collection::mul(const string& x,const string& x2,const string& xo) {
  int ix=getx(x); 
  int ix2=getx(x2); 
  int ixo=getx(xo); 
  if(ix<0 || ix2<0 || ixo<0 ) {
    fprintf(stderr,"Error: mul(%s,%s,%s) %d,%d,%d key not found\n"
	    ,x.c_str() ,x2.c_str() ,xo.c_str(),ix,ix2,ixo);
#ifdef __CINTLIB__
    G__printlinenum();
#endif
    return(*this);
  }
  return(mul(ix,ix2,ixo)); 
}
/////////////////////////////////////////////////////////////////////////////
Collection& Collection::div(const string& x,const string& x2,const string& xo) {
  int ix=getx(x); 
  int ix2=getx(x2); 
  int ixo=getx(xo); 
  if(ix<0 || ix2<0 || ixo<0 ) {
    fprintf(stderr,"Error: div(%s,%s,%s) %d,%d,%d key not found\n"
	    ,x.c_str() ,x2.c_str() ,xo.c_str(),ix,ix2,ixo);
#ifdef __CINTLIB__
    G__printlinenum();
#endif
    return(*this);
  }
  return(div(ix,ix2,ixo)); 
}
/////////////////////////////////////////////////////////////////////////////
Collection& Collection::mod(const string& x,const string& x2,const string& xo) {
  int ix=getx(x); 
  int ix2=getx(x2); 
  int ixo=getx(xo); 
  if(ix<0 || ix2<0 || ixo<0 ) {
    fprintf(stderr,"Error: mod(%s,%s,%s) %d,%d,%d key not found\n"
	    ,x.c_str() ,x2.c_str() ,xo.c_str(),ix,ix2,ixo);
#ifdef __CINTLIB__
    G__printlinenum();
#endif
    return(*this);
  }
  return(mod(ix,ix2,ixo)); 
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
Collection&  Collection::hyperlink(const string& key) {
  int x=getx(key);
  if(x<0) {
    fprintf(stderr,"Error: hyperlink(%s) key not found\n"
	    ,key.c_str());
#ifdef __CINTLIB__
    G__printlinenum();
#endif
    return(*this);
  }
  return(hyperlink(x));
}
/////////////////////////////////////////////////////////////////////////////
Collection&  Collection::hyperlink(int x) {
  for(const_iterator iy=m_plines.begin();iy!=m_plines.end();++iy) {
    Line& p = *(*iy);
    p.hyperlink(x);
  }
  return(*this);
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
Collection& Collection::replace(const string& key
				,const string& match,const string& rep){
#ifdef MULTIPLEKEYS
  vector<int> xs=getxs(key); 
  if(xs.size()==0) {
    fprintf(stderr,"Error: replace(%s,%s,%s) key not found\n"
            ,key.c_str(),match.c_str(),rep.c_str());
#ifdef __CINTLIB__
    G__printlinenum();
#endif
    return(*this);
  }
  else if(xs.size()==1) return(replace(xs[0],match,rep));
  else return(replace(xs,match,rep));
#else // MULTIPLEKEYS
  int x=getx(key); 
  if(x<0) {
    fprintf(stderr,"Error: replace(%s,%s,%s) key not found\n"
            ,key.c_str(),match.c_str(),rep.c_str());
#ifdef __CINTLIB__
    G__printlinenum();
#endif
    return(*this);
  }
  return(replace(x,match,rep));
#endif // MULTIPLEKEYS
}

/////////////////////////////////////////////////////////////////////////////
Collection& Collection::replace(int x,const string& match,const string& rep){
  if(x<0) return(*this);
#ifdef E__EFFICIENTREPLACE
  EasyExp r; r.compile_replace(match.c_str(),rep.c_str());
#endif
  iterator first=m_plines.begin();
  iterator last =m_plines.end();
  while(first!=last) {
#ifdef E__EFFICIENTREPLACE
    (*first)->replace(x,r,rep);
#else
    (*first)->replace(x,match,rep);
#endif
    ++first;
  }
  return(*this);
}
#ifdef MULTIPLEKEYS
/////////////////////////////////////////////////////////////////////////////
Collection& Collection::replace(const vector<int>& xs,const string& match,const string& rep){
  if(xs.size()==0) return(*this);
#ifdef E__EFFICIENTREPLACE
  EasyExp r; r.compile_replace(match.c_str(),rep.c_str());
#endif
  iterator first=m_plines.begin();
  iterator last =m_plines.end();
  while(first!=last) {
    vector<int>::const_iterator ifirst=xs.begin();
    vector<int>::const_iterator ilast=xs.end();
    while(ifirst!=ilast) {
      int x = *ifirst++;
#ifdef E__EFFICIENTREPLACE
      (*first)->replace(x,r,rep);
#else
      (*first)->replace(x,match,rep);
#endif
      ++first;
    }
  }
  return(*this);
}
#endif

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
Collection& Collection::let(int x,double a) {
  if(p_key && p_key->size()<=x) p_key->let(x,"");
  iterator first=m_plines.begin();
  iterator last =m_plines.end();
  while(first!=last) {
    (*first)->let(x,a);
    ++first;
  }
  return(*this);
}
/////////////////////////////////////////////////////////////////////////////
Collection& Collection::let(int x,const string& a) {
  if(p_key && p_key->size()<=x) p_key->let(x,"");
  iterator first=m_plines.begin();
  iterator last =m_plines.end();
  while(first!=last) {
    (*first)->let(x,a);
    ++first;
  }
  return(*this);
}
/////////////////////////////////////////////////////////////////////////////
Collection& Collection::let(int x,const value& a) {
  if(p_key && p_key->size()<=x) p_key->let(x,"");
  iterator first=m_plines.begin();
  iterator last =m_plines.end();
  while(first!=last) {
    (*first)->let(x,a);
    ++first;
  }
  return(*this);
}
/////////////////////////////////////////////////////////////////////////////
Collection& Collection::let(const string& key,double a) {
#ifdef MULTIPLEKEYS
  vector<int> xs=getxs(key); 
  if(xs.size()==0) {
    fprintf(stderr,"Error: Collection::let() index %s not found\n",key.c_str());
#ifdef __CINTLIB__
    G__printlinenum();
#endif
    return(*this);
  }
  else if(xs.size()==1) return(let(xs[0],a)); 
  else return(let(xs,a)); 
#else // MULTIPLEKEYS
  int x=getx(key); 
  if(x<0) {
    fprintf(stderr,"Error: Collection::let() index %s not found\n",key.c_str());
#ifdef __CINTLIB__
    G__printlinenum();
#endif
    return(*this);
  }
  return(let(x,a)); 
#endif // MULTIPLEKEYS
}
/////////////////////////////////////////////////////////////////////////////
Collection& Collection::let(const string& key,const string& a) {
#ifdef MULTIPLEKEYS
  vector<int> xs=getxs(key); 
  if(xs.size()==0) {
    fprintf(stderr,"Error: Collection::let() index %s not found\n",key.c_str());
#ifdef __CINTLIB__
    G__printlinenum();
#endif
    return(*this);
  }
  else if(xs.size()==1) return(let(xs[0],a)); 
  else return(let(xs,a)); 
#else // MULTIPLEKEYS
  int x=getx(key); 
  if(x<0) {
    fprintf(stderr,"Error: Collection::let() index %s not found\n",key.c_str());
#ifdef __CINTLIB__
    G__printlinenum();
#endif
    return(*this);
  }
  return(let(x,a)); 
#endif // MULTIPLEKEYS
}
/////////////////////////////////////////////////////////////////////////////
#ifdef MULTIPLEKEYS
Collection& Collection::let(const vector<int>& xs,double a) {
  iterator first=m_plines.begin();
  iterator last =m_plines.end();
  while(first!=last) {
    vector<int>::const_iterator ifirst=xs.begin();
    vector<int>::const_iterator ilast =xs.end();
    while(ifirst!=ilast) {
      int x = *ifirst++;
      (*first)->let(x,a);
    }
    ++first;
  }
  return(*this);
}
/////////////////////////////////////////////////////////////////////////////
Collection& Collection::let(const vector<int>& xs,const string& a) {
  iterator first=m_plines.begin();
  iterator last =m_plines.end();
  while(first!=last) {
    vector<int>::const_iterator ifirst=xs.begin();
    vector<int>::const_iterator ilast =xs.end();
    while(ifirst!=ilast) {
      int x = *ifirst++;
      (*first)->let(x,a);
    }
    ++first;
  }
  return(*this);
}
#endif
/////////////////////////////////////////////////////////////////////////////
int Collection::erase(int y) {
  deque<Line*>::iterator pos = numtoiter(m_plines,y);
  if(pos!=m_plines.end()) {
    m_plines.erase(pos);
    return(0);
  }
  else {
    return(1);
  }
}
/////////////////////////////////////////////////////////////////////////////
int Collection::erase(int y1,int y2) {
  int stat=0;
  deque<Line*>::iterator pos1 = numtoiter(m_plines,y1);
  deque<Line*>::iterator pos2 = numtoiter(m_plines,y2);
  if(pos1!=m_plines.end() && pos2!=m_plines.end()) {
    ++pos2;
    m_plines.erase(pos1,pos2);
  }
  return(stat);
}
/////////////////////////////////////////////////////////////////////////////
int Collection::erase_by_linenum(int l) {
  deque<Line*>::iterator pos = linenumtoiter(m_plines,l);
  if(pos!=m_plines.end()) {
    m_plines.erase(pos);
    return(0);
  }
  else {
    return(1);
  }
}
/////////////////////////////////////////////////////////////////////////////
int Collection::erase_by_linenum(int l1,int l2) {
  int stat=0;
  deque<Line*>::iterator pos1 = linenumtoiter(m_plines,l1);
  deque<Line*>::iterator pos2 = linenumtoiter(m_plines,l2);
  if(pos1!=m_plines.end() && pos2!=m_plines.end()) {
    ++pos2;
    m_plines.erase(pos1,pos2);
  }
  return(stat);
}

/////////////////////////////////////////////////////////////////////////////
void Collection::setlinenum() {
  for(int i=0;i<size();i++) {
    get(i).setlinenum(i);
  }
}
/////////////////////////////////////////////////////////////////////////////
Collection Collection::Event2Table() {
  map<string,int> xp1;
  Collection t;
  t.push_back(Line());
  t.setkey(0);
  Line& keyline = t.getkey();
  string key;
  int x;
  value val;
  int i,j;
  for(i=0;i<size();i++) {
    Line& line = get(i);
    for(j=0;j<line.size();j+=2) {
      key = line[j].tostring();
      val = line[j+1];
      x = xp1[key];
      if(!x) {
        keyline.push_back(key);
        x = keyline.size();
	xp1[key] = x;
      }
      t[i].let(x-1,val);
    }
  }
  return(t);
}
/////////////////////////////////////////////////////////////////////////////
void Collection::resize(int i) { 
  Line line;
  if(!p_sheet) p_sheet = new Sheet;
  for(int j=0;j<i;j++) {
    p_sheet->push_back(line);
    m_plines.push_back(&p_sheet->back());
  }
}

/////////////////////////////////////////////////////////////////////////////
void Collection::column_resize(int i) { 
  Line line;
  if(!p_sheet) p_sheet = new Sheet;
  getkey().resize(i);
  for(int j=0;j<size();j++) {
    get(i).resize(i);
  }
}

/////////////////////////////////////////////////////////////////////////////
Collection Collection::Table2Event() {
  Collection t;
  int i,j;
  if(!p_key) {
    fprintf(stderr,"Error: Collection::Table2Event(), key line is not set\n");
    return(t);
  }
  Line line;
  Line& keyline = t.getkey();
  if(size()>0) {
    Line& l0 = get(0);
    for(j=0;j<l0.size();j++) {
      line.push_back(keyline[j].tostring());
      line.push_back(l0[j].tostring());
    }
    if(line.size()) t.push_back(line);
  }
  for(i=1;i<size();i++) {
    line.clear();
    Line& l0 = get(i-1);
    Line& l1 = get(i);
    for(j=0;j<l1.size();j++) {
      if(l1[j]!=l0[j]) {
        line.push_back(keyline[j].tostring());
        line.push_back(l1[j].tostring());
      }
    }
    if(line.size()) t.push_back(line);
  }
  return(t);
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
CollectionIterator Collection::begin(const vector<int>& xs) {
  Collection *pc= this;
  CollectionIterator iter(*pc,xs);
  return(iter);
}
CollectionIterator Collection::begin(int x1,int x2,int x3,int x4) {
  vector<int> xs = getxvec(x1,x2,x3,x4);
  return(begin(xs));
}
CollectionIterator Collection::begin(const vector<string>& keys) {
  vector<int> xs = getxvec(keys);
  return(begin(xs));
}
CollectionIterator Collection::begin(const string& key1,const string& key2,const string& key3,const string& key4) {
  vector<int> xs = getxvec(key1,key2,key3,key4);
  return(begin(xs));
}
/////////////////////////////////////////////////////////////////////////////
CollectionIterator Collection::end(const CollectionIterator& first) {
  CollectionIterator iter(first,0);
  return(iter);
}
CollectionIterator Collection::end(const vector<int>& xs) {
  Collection *pc= this;
  CollectionIterator iter(*pc,xs,0);
  return(iter);
}
CollectionIterator Collection::end(int x1,int x2,int x3,int x4) {
  vector<int> xs;
  if(x1>=0) xs.push_back(x1);
  if(x2>=0) xs.push_back(x2);
  if(x3>=0) xs.push_back(x3);
  if(x4>=0) xs.push_back(x4);
  return(end(xs));
}
CollectionIterator Collection::end(const vector<string>& keys) {
  vector<int> xs;
  for(unsigned int i=0;i<keys.size();i++) {
    if(keys[i]!=INVALIDSTR) xs.push_back(getx(keys[i]));
  }
  return(end(xs));
}
CollectionIterator Collection::end(const string& key1,const string& key2,const string& key3,const string& key4) {
  vector<int> xs;
  if(key1!=INVALIDSTR) xs.push_back(getx(key1));
  if(key2!=INVALIDSTR) xs.push_back(getx(key2));
  if(key3!=INVALIDSTR) xs.push_back(getx(key3));
  if(key4!=INVALIDSTR) xs.push_back(getx(key4));
  return(end(xs));
}
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
#ifdef COLLECTIONCOMPARE
/////////////////////////////////////////////////////////////////////////////
bool operator==(const Collection& a,const Collection& b)  {
  if(a.size()!=b.size()) return(false);
  int n = ::min(a.size(),b.size());
  for(int i=0;i<n;i++) {
    if(a[i]!=b[i]) return(false);
  }
  return(true);
}
/////////////////////////////////////////////////////////////////////////////
bool operator<(const Collection& a,const Collection& b)  {return(&a<&b);}
/////////////////////////////////////////////////////////////////////////////
#endif

#ifdef MATHOPERATOR
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
Collection operator+(const Collection& a,const Collection& b)  {
  int i;
  Collection sub;
  sub.setsheet(a.p_sheet);
  sub.m_stat = a.m_stat;
  Line line;
  for(i=0;i<a.size()&&i<b.size();i++) {
    line = a[i] + b[i];
    sub.p_sheet->push_back(line);
    sub.push_back(&sub.p_sheet->back());  
  }
  return(sub);
}
/////////////////////////////////////////////////////////////////////////////
Collection operator-(const Collection& a,const Collection& b)  {
  int i;
  Collection sub;
  sub.setsheet(a.p_sheet);
  sub.m_stat = a.m_stat;
  Line line;
  for(i=0;i<a.size()&&i<b.size();i++) {
    line = a[i] - b[i];
    sub.p_sheet->push_back(line);
    sub.push_back(&sub.p_sheet->back());  
  }
  return(sub);
}
/////////////////////////////////////////////////////////////////////////////
Collection operator*(const Collection& a,const Collection& b)  {
  int i;
  Collection sub;
  sub.setsheet(a.p_sheet);
  sub.m_stat = a.m_stat;
  Line line;
  for(i=0;i<a.size()&&i<b.size();i++) {
    line = a[i] * b[i];
    sub.p_sheet->push_back(line);
    sub.push_back(&sub.p_sheet->back());  
  }
  return(sub);
}
/////////////////////////////////////////////////////////////////////////////
Collection operator/(const Collection& a,const Collection& b)  {
  int i;
  Collection sub;
  sub.setsheet(a.p_sheet);
  sub.m_stat = a.m_stat;
  Line line;
  for(i=0;i<a.size()&&i<b.size();i++) {
    line = a[i] / b[i];
    sub.p_sheet->push_back(line);
    sub.push_back(&sub.p_sheet->back());  
  }
  return(sub);
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
#endif

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// Sheet
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
Sheet::Sheet(const Sheet& s) {
  m_fname = s.m_fname;
  m_lines = s.m_lines;
}

/////////////////////////////////////////////////////////////////////////////
int Sheet::Read(const string& fname,int head) {
  m_fname=fname;
  //ReadF f(m_fname.c_str());
  f.open(m_fname.c_str());
  
  if(!f.isvalid()) return(-1);

  if(fname.find(".csv")!=string::npos /*<100000*/) {
    f.setdelimiter(",");
    f.setseparator("");
  }
  else if(fname.find(".tsv")!=string::npos /*<100000*/) {
    f.setdelimiter("\t");
    f.setseparator("");
  }
  else {
    //f.setdelimiter(",");
    f.setdelimiter(",");
    f.setseparator("");
  }
  f.setquotation("'\"");
  //f.setendofline("\n\r\f");
  f.setescapebyquotation(true);

  if(m_separator.size()) f.setseparator(m_separator.c_str());
  if(m_delimiter.size()) f.setdelimiter(m_delimiter.c_str());
  if(m_endofline.size()) f.setendofline(m_endofline.c_str());
  if(m_quotation.size()) f.setquotation(m_quotation.c_str());
  f.setkeepquotation(m_keepquotation);

  Line line;

  if(head) {
    int i=0;
    while(f.read() && i++<head) {
      line.init();
      line.set(f);
      m_lines.push_back(line);
    }
    if(f.isvalid()) {
      // ReadF f is kept open
    }
    else {
      f.close();
    }
    return(0);
  }

  while(f.read()) {
    line.init();
    line.set(f);
    m_lines.push_back(line);
  }
  f.close();
  return(0);
}

/////////////////////////////////////////////////////////////////////////////
int Sheet::IncrementalRead(int head) {
  m_lines.clear();
  if(!f.isvalid()) return(1);
  Line line;
  int i=0;
  while(f.read() && i++<head) {
    line.init();
    line.set(f);
    m_lines.push_back(line);
  }

  if(f.isvalid()) {
    // ReadF f is kept open
    return(1);
  }
  else {
    f.close();
  }
  return(0);
}

/////////////////////////////////////////////////////////////////////////////
Collection Sheet::collection() {
  Collection whole;
  whole.setsheet(this);
  deque<Line>::iterator first=m_lines.begin();
  deque<Line>::iterator last =m_lines.end();
  while(first!=last) {
    Line* p = &(*first);
    whole.push_back(p);
    ++first;
  }
  return(whole);
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
