/**************************************************************************
* SparseVector.h
*
* 2013/Sep 
* Masaharu Goto
*
**************************************************************************/
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

#ifndef G__SPARSEVECTOR
#define G__SPARSEVECTOR

#if defined(G__ROOT) 
//#if defined(G__LINUX) || defined(__linux) || defined(G__ROOT) || defined(__VC13__)
#define G__NOSPARSEVECTOR
#endif

#ifndef G__NOSPARSEVECTOR

#include <cstdio>
#include <map>
#include <algorithm>
#include <iostream>
#include <cmath>
using namespace std;

//////////////////////////////////////////////////////////////////////
template<class T> class	SparseVector {
 private:
 public:
  int m_Xsize;
  T m_dmy;
  map<unsigned int,T> m_sparsedata;
  //map<unsigned int,T>::iterator m_it;

 public:
#if defined(__APPLE__) || defined(__VC13__) || defined(__linux)
  typedef typename map<unsigned int,T>::iterator iterator;
  typedef typename map<unsigned int,T>::const_iterator const_iterator;
#else
  typedef map<unsigned int,T>::iterator iterator;
  typedef map<unsigned int,T>::const_iterator const_iterator;
#endif
  
  ////////////////////////////////////////////////////////
  SparseVector() :  m_dmy(), m_Xsize(0) { }
  SparseVector(const SparseVector& x) { 
    m_dmy = x.m_dmy;
    m_sparsedata = x.m_sparsedata;
    //m_it = x.m_it;
    m_Xsize = x.m_Xsize;
  }

  ////////////////////////////////////////////////////////
  void clear() { m_sparsedata.clear(); }
  void setsize(int Xsize) { m_Xsize=Xsize; }
  void resize(int Xsize) { m_Xsize=Xsize; }
  void push_back(const T& a) { set(m_Xsize++,a);} //  dummy , do not use
  unsigned int size() const { return(m_Xsize); }
  unsigned int sparsesize() const { return(m_sparsedata.size()); }
  ////////////////////////////////////////////////////////
  unsigned int internalindex(int i) const { 
    unsigned int ij = i;
    return(ij);
  }

  void index(unsigned int ij,int& i) const {
    i = ij; 
  }

  ////////////////////////////////////////////////////////
  // non sparse access
  const T& operator[](int ij) const { return(get(ij)); }

  ////////////////////////////////////////////////////////
  // sparse access
  const T& get(int i) const { 
    unsigned int ij = internalindex(i);
    const_iterator it = m_sparsedata.find(ij);
    if(it==m_sparsedata.end()) {
      return(m_dmy);
    }
    else {
      return((*it).second);
    }
  }
  ////////////////////////////////////////////////////////
#if 0
  T& getref(int i) {
    unsigned int ij = internalindex(i);
    iterator it = m_sparsedata.find(ij);
    if(it==m_sparsedata.end()) {
      pair<unsigned int,T> v(ij,T());
      m_sparsedata.insert(v);
      it = m_sparsedata.find(ij);
    }
    return((*it).second);
  }
  T& operator()(int i) { return(getref(i)); }
#endif
  ////////////////////////////////////////////////////////
  const T& set(int i,const T& val) {
#if 0
    return(getref(i) = val);
#else
    //printf("A i=%d val=%g size=%u %d\n",i,val,m_sparsedata.size(),m_Xsize);
    if(val==m_dmy) return(m_dmy);
    unsigned int ij = internalindex(i);
    //printf("B i=%d val=%g size=%u %d\n",i,val,m_sparsedata.size(),m_Xsize);
    iterator it = m_sparsedata.find(ij);
    if(it==m_sparsedata.end()) {
      pair<unsigned int,T> v(ij,val);
      m_sparsedata.insert(v);
      it = m_sparsedata.find(ij);
      if(i>m_Xsize) m_Xsize=i+1;
      //printf("C i=%d val=%g size=%u %d\n",i,val,m_sparsedata.size(),m_Xsize);
    }
    else (*it).second=val;
    return((*it).second);
#endif
  }

#if defined(__APPLE__) || defined(__VC13__) || defined(__linux)
  ////////////////////////////////////////////////////////
  typename map<unsigned int,T>::iterator insert(int i,const T& val) {
    unsigned int ij = internalindex(i);
    pair<unsigned int,T> v(ij,val);
    m_sparsedata.insert(v);
    iterator it = m_sparsedata.find(ij);
    return(it);
  }

  ////////////////////////////////////////////////////////
  void index(typename map<unsigned int,T>::iterator& it,int& i) { 
    if(it!=m_sparsedata.end()) {
      index((*it).first,i);
    }
    else {
      i = -1;
    }
  }

  ////////////////////////////////////////////////////////
  T& value(typename map<unsigned int,T>::iterator& it) { 
    if(it!=m_sparsedata.end()) return((*it).second); 
    else {
      return(m_dmy);
    }
  }
#else
  ////////////////////////////////////////////////////////
  map<unsigned int,T>::iterator insert(int i,const T& val) {
    unsigned int ij = internalindex(i);
    pair<unsigned int,T> v(ij,val);
    m_sparsedata.insert(v);
    iterator it = m_sparsedata.find(ij);
    if(i>m_Xsize) m_Xsize=i+1;
    return(it);
  }
  ////////////////////////////////////////////////////////
  void index(map<unsigned int,T>::iterator& it,int& i) { 
    if(it!=m_sparsedata.end()) {
      index((*it).first,i);
    }
    else {
      i = -1;
    }
  }
  ////////////////////////////////////////////////////////
  T& value(map<unsigned int,T>::iterator& it) { 
    if(it!=m_sparsedata.end()) return((*it).second); 
    else {
      return(m_dmy);
    }
  }
#endif

  ////////////////////////////////////////////////////////
  iterator begin() {return(m_sparsedata.begin());}
  iterator end() {return(m_sparsedata.end());}
  ////////////////////////////////////////////////////////
  
};
//////////////////////////////////////////////////////////////////////
template<class T>
bool operator==(const SparseVector<T>& ain,const SparseVector<T>& bin){
  SparseVector<T>& a = (SparseVector<T>&)ain;
  SparseVector<T>& b = (SparseVector<T>&)bin;
  if(a.size()!=b.size() || a.sparsesize()!=b.sparsesize()) return(false);

  typename SparseVector<T>::const_iterator firsta=a.begin();
  typename SparseVector<T>::const_iterator lasta=a.end();
  typename SparseVector<T>::const_iterator firstb=b.begin();
  typename SparseVector<T>::const_iterator lastb=b.end();
  for(;firsta!=lasta && firstb!=lastb; ++firsta, ++firstb) {
    if((*firsta).first != (*firstb).first ||
       (*firsta).second!= (*firstb).second ) return(false);
  }
  // SparseVector equality test sometimes fails probably due to floating
  // point error.
  return(true);
}

template<class T>
bool operator!=(const SparseVector<T>& a,const SparseVector<T>& b){
  return(!operator==(a,b));
}

//////////////////////////////////////////////////////////////////////
template<class T>
T euclideandistance(const SparseVector<T>& a,const SparseVector<T>& b){
  T sum2=0.0,tmp;
  unsigned int i;
  for(i=0;i<a.size() && i<b.size();i++) {
    tmp = a[i] - b[i];
    sum2 += tmp*tmp;
  }
  for(;i<a.size();i++) sum2 += a[i]*a[i]; // in case a.size > b.size
  for(;i<b.size();i++) sum2 += b[i]*b[i]; // in case a.size < b.size
  return(sqrt(sum2));
}


//////////////////////////////////////////////////////////////////////
template<class T>
T innerproduct(const SparseVector<T>& ain,const SparseVector<T>& bin){
  SparseVector<T>& a = (SparseVector<T>&)ain;
  SparseVector<T>& b = (SparseVector<T>&)bin;
  T m = 0;
  typename SparseVector<T>::const_iterator first,last=a.end();
  unsigned int i;
  for(first=a.begin();first!=last;++first) {
    i = (*first).first;
    m += (*first).second * b.get(i);
  }
  return(m);
}

//////////////////////////////////////////////////////////////////////
template<class T>
T normalizedinnerproduct(const SparseVector<T>& ain
			 ,const SparseVector<T>& bin){
#if 0
  T r=0,ra=0,rb=0;
  unsigned int i;
  for(i=0;i<ain.size()&&i<bin.size();i++) {
    r  += ain[i] * bin[i];
    ra += ain[i] * ain[i];
    rb += bin[i] * bin[i];
  }
  return(r/(ra+rb)*2);
#else
  SparseVector<T>& a = (SparseVector<T>&)ain;
  SparseVector<T>& b = (SparseVector<T>&)bin;
  T sum=0.0,suma=0.0,sumb=0.0;
  unsigned int i;
  typename SparseVector<T>::const_iterator first,last=a.end();
  for(first=a.begin();first!=last;++first) {
    i = (*first).first;
    sum += (*first).second * b.get(i);
    suma += (*first).second * (*first).second;
  }
  last=b.end();
  for(first=b.begin();first!=last;++first) {
    sumb += (*first).second * (*first).second;
  }
  if((suma+sumb)==0.0) return(1.0);
  return(sum/(suma+sumb)*2);
#endif
}

//////////////////////////////////////////////////////////////////////
template<class T>
T normalizedvectorinnerproduct(const SparseVector<T>& ain
			       ,const SparseVector<T>& bin){
  SparseVector<T>& a = (SparseVector<T>&)ain;
  SparseVector<T>& b = (SparseVector<T>&)bin;
  T sum=0.0,suma=0.0,sumb=0.0;
  unsigned int i;
  typename SparseVector<T>::const_iterator first,last=a.end();
  for(first=a.begin();first!=last;++first) {
    i = (*first).first;
    sum += (*first).second * b.get(i);
    suma += (*first).second * (*first).second;
  }
  last=b.end();
  for(first=b.begin();first!=last;++first) {
    sumb += (*first).second * (*first).second;
  }
  if(suma==0 && sumb==0) return(1.0);
  if(suma==0 || sumb==0) return(0.0);
  return(sum/sqrt(suma*sumb));
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// SparseMatrix Implementation is incomplete. 
// Use vector<SparseVector<T> > for now.
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
template<class T> class	SparseMatrix {
 private:
  T m_default;
  T m_dmy;
  map<unsigned int,T> m_sparsedata;
  //map<unsigned int,T>::iterator m_it;
  int m_Xsize;
  int m_Ysize;

 public:
#if defined(__APPLE__) || defined(__VC13__) || defined(__linux)
  typedef typename map<unsigned int,T>::iterator iterator;
  typedef typename map<unsigned int,T>::const_iterator const_iterator;
#else
  typedef map<unsigned int,T>::iterator iterator;
  typedef map<unsigned int,T>::const_iterator const_iterator;
#endif

  ////////////////////////////////////////////////////////
  SparseMatrix() : m_default(), m_dmy() { m_Ysize=0x10000; }
  SparseMatrix(const SparseMatrix& x) { 
    m_default = x.m_default;
    m_dmy = x.m_dmy;
    m_sparsedata = x.m_sparsedata;
    //m_it = x.m_it;
    m_Xsize = x.m_Xsize;
    m_Ysize = x.m_Ysize;
  }

  ////////////////////////////////////////////////////////
  void setsize(int Xsize,int Ysize) { m_Xsize=Xsize; m_Ysize=Ysize; }
  ////////////////////////////////////////////////////////
  unsigned int internalindex(int i,int j) { 
    unsigned int ij = i*m_Ysize + j;
    return(ij);
  }

  void index(unsigned int ij,int& i,int& j) {
    i = ij/m_Ysize; 
    j = ij%m_Ysize;
  }

  ////////////////////////////////////////////////////////
  // non Sparse access
  //T& operator[](int ij) const { return(get(ij]); }

  ////////////////////////////////////////////////////////
  // Sparse access
  T& operator()(int i,int j) { return(get(i,j)); }
  T& get(int i,int j) { 
    unsigned int ij = internalindex(i,j);
    iterator it = m_sparsedata.find(ij);
    if(it==m_sparsedata.end()) {
      m_dmy = m_default;
      return(m_dmy);
    }
    else {
      return((*it).second);
    }
  }
  ////////////////////////////////////////////////////////
  T& set(int i,int j,const T& val) {
    unsigned int ij = internalindex(i,j);
    iterator it = m_sparsedata.find(ij);
    if(it==m_sparsedata.end()) {
      pair<unsigned int,T> v(ij,val);
      m_sparsedata.insert(v);
      it = m_sparsedata.find(ij);
    }
    return((*it).second);
  }

  ////////////////////////////////////////////////////////
  iterator insert(int i,int j,const T& val) {
    unsigned int ij = internalindex(i,j);
    pair<unsigned int,T> v(ij,val);
    m_sparsedata.insert(v);
    iterator it = m_sparsedata.find(ij);
    return(it);
  }

#if defined(__APPLE__) || defined(__VC13__) || defined(__linux)
  ////////////////////////////////////////////////////////
  void index(const typename map<unsigned int,T>::iterator& it,int& i,int& j) { 
    if(it!=m_sparsedata.end()) {
      index((*it).first,i,j);
    }
    else {
      i = j = -1;
    }
  }

  ////////////////////////////////////////////////////////
  T& value(typename map<unsigned int,T>::iterator& it) { 
    if(it!=m_sparsedata.end()) return((*it).second); 
    else {
      m_dmy = m_default;
      return(m_dmy);
    }
  }
#else
  ////////////////////////////////////////////////////////
  void index(const map<unsigned int,T>::iterator& it,int& i,int& j) { 
    if(it!=m_sparsedata.end()) {
      index((*it).first,i,j);
    }
    else {
      i = j = -1;
    }
  }

  ////////////////////////////////////////////////////////
  T& value(map<unsigned int,T>::iterator& it) { 
    if(it!=m_sparsedata.end()) return((*it).second); 
    else {
      m_dmy = m_default;
      return(m_dmy);
    }
  }
#endif

  ////////////////////////////////////////////////////////
#if defined(__APPLE__) || defined(__VC13__) || defined(__linux)
  typename map<unsigned int,T>::iterator begin() {return(m_sparsedata.begin());}
  typename map<unsigned int,T>::iterator end() {return(m_sparsedata.end());}
#else
  map<unsigned int,T>::iterator begin() {return(m_sparsedata.begin());}
  map<unsigned int,T>::iterator end() {return(m_sparsedata.end());}
#endif
  size_t size() const { return(m_sparsedata.size()); }
  ////////////////////////////////////////////////////////

  void disp(const char* fname,const char* mode) {
    FILE* fp=fopen(fname,mode);
    if(!fp) {
      fprintf(stderr,"Error: Cannot create %s\n",fname);
      return;
    }
#if defined(__APPLE__) || defined(__VC13__) || defined(__linux)
    typename map<unsigned int,T>::iterator first = m_sparsedata.begin();
    typename map<unsigned int,T>::iterator last  = m_sparsedata.end();
#else
    map<unsigned int,T>::iterator first = m_sparsedata.begin();
    map<unsigned int,T>::iterator last  = m_sparsedata.end();
#endif
    unsigned int ij;
    int i,j;
    T val;
    fprintf(fp,"i,j,Covar\n");
    while(first!=last) {
      ij = (*first).first;
      index(ij,i,j);
      val = (*first).second;
      //index(first,i,j);
      //val=value(first);   
      fprintf(fp,"%d,%d,%g\n",i,j,val);
      ++first;
    }
    fclose(fp);
  }
  
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
typedef SparseVector<double> DSparseVector;
typedef SparseVector<float> FSparseVector;
typedef SparseVector<int> ISparseVector;

typedef SparseMatrix<double> DSparseMatrix;
typedef SparseMatrix<float> FSparseMatrix;
typedef SparseMatrix<int> ISparseMatrix;

#endif

#endif // G__SPARSEVECTOR

