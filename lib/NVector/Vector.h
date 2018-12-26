/**************************************************************************
* Vector.h
*
*  Numerical vector class template
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

#ifndef G__NVECTOR
#define G__NVECTOR

#include <vector>
#include <algorithm>
#include <iostream>
#include <cmath>
#include <cfloat>
using namespace std;


#define __STAT__
#ifdef __STAT__
//#include <statistics.h>
#include "Stat.h"
#endif

#ifndef NOCOMPLEX
#ifndef COMPLEX_FILENAME
#include "Complex.h"
#else
#include "CINT_Complex.h"
#endif
#endif

#define DISPIOS
//#define DISPFILE

#define COMPABS

//////////////////////////////////////////////////////////////////////
template<class T,class E>
inline T Min_(const T& a,const E& b) { return(a<b?a:b); }
template<class T,class E>
inline T Max_(const T& a,const E& b) { return(a>b?a:b); }

//////////////////////////////////////////////////////////////////////
template<class T> class Vector : public vector<T> {
 public:
  //
  Vector() {}
  Vector(const Vector<T>& a) {
    unsigned int n=a.size();
    for(unsigned int i=0;i<n;++i) Vector<T>::push_back(a[i]);
  }
  Vector(int n,const T* a) {
    for(int i=0;i<n;++i) Vector<T>::push_back(a[i]);
  }
  Vector(const vector<T>& a) {
    for(unsigned int i=0;i<a.size();++i) Vector<T>::push_back(a[i]);
  }
  Vector(const T& start,const T& stop,int n) {
    T d = (stop-start)/(T)n;
    T v;
    for(int i=0;i<n;++i) {
      v = start + d*(T)i;
      push_back(v);
    }
  }
  Vector(const T& a) {
    unsigned int n=Vector<T>::size();
    for(unsigned int i=0;i<n;++i) Vector<T>::push_back(a); 
  }
  Vector<T>& operator=(const Vector<T>& a) {
    unsigned int n=a.size();
    if(n && n==Vector<T>::size()) {
      T *d = (&Vector<T>::front());
      for(unsigned int i=0;i<n;++i) d[i] = a[i];
    }
    else {
      Vector<T>::clear();
      for(unsigned int i=0;i<n;i++) Vector<T>::push_back(a[i]);
    }
    return(*this);
  }
  Vector<T>& operator=(const T& a) {
    unsigned int n=Vector<T>::size();
    if(!n) return(*this);
    T *d = (&Vector<T>::front());
    for(unsigned int i=0;i<n;++i) d[i] = a;
    return(*this);
  }

  void Set(unsigned int n,const T* a) {
    if(n==Vector<T>::size()) {
      for(unsigned int i=0;i<n;++i) get(i) = a[i]; 
    }
    else {
      Vector<T>::clear();
      for(unsigned int i=0;i<n;i++) Vector<T>::push_back(a[i]);
    }
  }

  void Set(const T& start,const T& stop,unsigned int n) {
    T d = (stop-start)/(T)n;
    T v;
    if(n==Vector<T>::size()) {
      for(unsigned int i=0;i<n;++i) {
        v = start + d*(T)i;
        get(i) = v;
      }
    }
    else {
      vector<T>::clear();
      for(unsigned int i=0;i<n;++i) {
        v = start + d*(T)i;
        push_back(v);
      }
    }
  }

#if 0
  T min() {
    T m=1e37;
    for(unsigned int i=0;i<size();i++) if(m>get(i)) m = get(i);
    return(m);
  }
  T max() {
    T m= -1e37;
    for(unsigned int i=0;i<size();i++) if(m<get(i)) m = get(l);
    return(m);
  }
#endif

  unsigned int size() const { return(vector<T>::size()); }
  T& get(unsigned int i) { return(vector<T>::operator[](i)); }
  T  get(unsigned int i) const { return(vector<T>::operator[](i)); }
  //T& operator[](unsigned int i) { return(vector<T>::operator[](i)); }
  //T operator[](unsigned int i) const { return(vector<T>::operator[](i)); }

#if 0
  Vector<T>& operator-() {
    fprintf(stderr,"ERROR: Unary operator- has problem\n");
    //exit();
    unsigned int n=Vector<T>::size();
    if(!n) return(*this);
    T *dat = &Vector<T>::front();
    for(unsigned int i=0;i<n;++i) dat[i] = dat[i]* (-1);
    return(*this);
  }
  Vector<T>& operator+() {
    fprintf(stderr,"ERROR: Unary operator+ has problem\n");
    //exit();
    return(*this);
  }
#endif

  Vector<T>& operator+=(const Vector<T>& a) {
    unsigned int n=a.size();
    if(n>Vector<T>::size()) resize(n); //n=Vector<T>::size();
    if(!n) return(*this);
    T *dat = &Vector<T>::front();
    const T *data = &a.Vector<T>::front();
    for(unsigned int i=0;i<n;++i) dat[i] = dat[i] +  data[i];
    return(*this);
  }
  Vector<T>& operator-=(const Vector<T>& a) {
    unsigned int n=a.size();
    if(n>Vector<T>::size()) resize(n); //n=Vector<T>::size();
    if(!n) return(*this);
    T *dat = &Vector<T>::front();
    for(unsigned int i=0;i<n;++i) dat[i] = dat[i] -  a[i];
    return(*this);
  }
  Vector<T>& operator*=(const Vector<T>& a) {
    unsigned int n=a.size();
    if(n>Vector<T>::size()) resize(n); //n=Vector<T>::size();
    if(!n) return(*this);
    T *dat = &Vector<T>::front();
    for(unsigned int i=0;i<n;++i) dat[i] = dat[i] * a[i];
    return(*this);
  }
  Vector<T>& operator/=(const Vector<T>& a) {
    unsigned int n=a.size();
    if(n>Vector<T>::size()) resize(n); //n=Vector<T>::size();
    if(!n) return(*this);
    T *dat = &Vector<T>::front();
    for(unsigned int i=0;i<n;++i) dat[i] = dat[i] / a[i];
    return(*this);
  }

  Vector<T>& operator+=(const T& a) {
    unsigned int n=Vector<T>::size();
    if(!n) return(*this);
    T *dat = &Vector<T>::front();
    for(unsigned int i=0;i<n;++i) dat[i] = dat[i] + a;
    return(*this);
  }
  Vector<T>& operator-=(const T& a) {
    unsigned int n=Vector<T>::size();
    if(!n) return(*this);
    T *dat = &Vector<T>::front();
    for(unsigned int i=0;i<n;++i) dat[i] = dat[i] - a;
    return(*this);
  }
  Vector<T>& operator*=(const T& a) {
    unsigned int n=Vector<T>::size();
    if(!n) return(*this);
    T *dat = &Vector<T>::front();
    for(unsigned int i=0;i<n;++i) dat[i] = dat[i] * a;
    return(*this);
  }
  Vector<T>& operator/=(const T& a) {
    unsigned int n=Vector<T>::size();
    if(!n) return(*this);
    T *dat = &Vector<T>::front();
    for(unsigned int i=0;i<n;++i) dat[i] = dat[i] / a;
    return(*this);
  }
#if 0
  Vector<T>& operator+=(int a) {
    unsigned int n=Vector<T>::size();
    if(!n) return(*this);
    T *dat = &Vector<T>::front();
    for(unsigned int i=0;i<n;++i) dat[i] = dat[i] + a;
    return(*this);
  }
  Vector<T>& operator-=(int a) {
    unsigned int n=Vector<T>::size();
    if(!n) return(*this);
    T *dat = &Vector<T>::front();
    for(unsigned int i=0;i<n;++i) dat[i] = dat[i] - a;
    return(*this);
  }
  Vector<T>& operator*=(int a) {
    unsigned int n=Vector<T>::size();
    if(!n) return(*this);
    T *dat = &Vector<T>::front();
    for(unsigned int i=0;i<n;++i) dat[i] = dat[i] * a;
    return(*this);
  }
  Vector<T>& operator/=(int a) {
    unsigned int n=Vector<T>::size();
    if(!n) return(*this);
    T *dat = &Vector<T>::front();
    for(unsigned int i=0;i<n;++i) dat[i] = dat[i] / a;
    return(*this);
  }
#endif

  Vector<T> interleave(const Vector<T>& a,int ratio=1) {
    Vector<T> r;
    int count=1,j=0;
    for(unsigned int i=0;i<size();i++) {
      r.push_back(get(i));
      if(count==ratio) {
        r.push_back(a[j++]);
        count=1;
      }
      else ++count;
    }
    return(r);
  }

  Vector<T> index(unsigned int from,unsigned int to) const {
    Vector<T> r;
    if(to<from) to=size();
    for(unsigned int i=from;i<to;i++) r.push_back(get(i));
    return(r);
  }

#if 0
  unsigned int indexmin() const {
    T m=1e37;
    unsigned int index=0;
    for(unsigned int i=0;i<size();i++) if(m>get(i)) { m=get(i); index = i; }
    return(index);
  }

  unsigned int indexmax() const {
    T m=-1e37;
    unsigned int index=0;
    for(unsigned int i=0;i<size();i++) if(m<get(i)) { m=get(i); index = i; }
    return(index);
  }

  T min() const {
    T m=1e37;
    for(unsigned int i=0;i<size();i++) if(m>get(i)) m = get(i);
    return(m);
  }

  T max() const {
    T m=-1e37;
    for(unsigned int i=0;i<size();i++) if(m<get(i)) m = get(i);
    return(m);
  }

#endif

#ifndef __MAKECINT__
  Vector<T> reduce(unsigned int d) {
    Vector<T> r;
    T val=0;
    unsigned int N=0;
    for(unsigned int i=0;i<size();i++) {
      val = val + get(i);
      if(++N==d) {
        r.push_back(val/(T)N);
        val = 0;
	N = 0;
      }
    }
    return(r);
  }
#endif

  Vector<T>& i() {
    unsigned int n=Vector<T>::size();
    if(!n) { n=100; resize(n);  } 
    T *dat = &Vector<T>::front();
    for(unsigned int i=0;i<n;++i) dat[i] = i;
    return(*this);
  }

  T last() { 
    unsigned int n=Vector<T>::size();
    if(!n) return(T());
    T *dat = &Vector<T>::front();
    return(dat[n-1]);
  }

  void print(int start=0,int stop=0) {
    unsigned int n=Vector<T>::size();
    if(!n) return;
    if(stop && n>(unsigned int)stop) n=stop;
    T *dat = &Vector<T>::front();
    //for(unsigned int i=start;i<n;++i) printf("%d:%g ",i,dat[i]);
    //printf("\n");
    for(unsigned int i=start;i<n;++i) cout << i << ":" << dat[i] << " ";
    cout << endl;
  }

#if 0
  void resize(unsigned int n) {
    while(size()<n) vector<T>::push_back(0);
    while(n<size()) vector<T>::pop_back();
  }
#endif

  void disp(ostream& os=cout) const ;
#ifdef DISPFILE
  void disp(FILE* fp) const ;
#endif

  void clear() { vector<T>::clear(); }
  void resize(unsigned int i) { vector<T>::resize(i); }
  void push_back(const T& a) { vector<T>::push_back(a); }
  const T& front() const { return(vector<T>::front()); }
  T& front() { return(vector<T>::front()); }
  const T& back() const { return(vector<T>::back()); }
  T& back() { return(vector<T>::back()); }
  const T& operator[](unsigned int i) const { return(vector<T>::operator[](i)); }
  T& operator[](unsigned int i) { return(vector<T>::operator[](i)); }

};


//////////////////////////////////////////////////////////////////////
template<class T> 
Vector<T> operator+(const Vector<T>& a,const Vector<T>& b){
  Vector<T> r;
  unsigned int n=a.size();
  for(unsigned int i=0;i<n;++i) r.push_back(a[i]+b[i]);
  return(r);
}
//////////////////////////////////////////////////////////////////////
template<class T> 
Vector<T> operator-(const Vector<T>& a,const Vector<T>& b){
  Vector<T> r;
  unsigned int n=a.size();
  for(unsigned int i=0;i<n;++i) r.push_back(a[i]-b[i]);
  return(r);
}
//////////////////////////////////////////////////////////////////////
template<class T> 
Vector<T> operator*(const Vector<T>& a,const Vector<T>& b){
  Vector<T> r;
  unsigned int n=a.size();
  for(unsigned int i=0;i<n;++i) r.push_back(a[i]*b[i]);
  return(r);
}
//////////////////////////////////////////////////////////////////////
template<class T> 
Vector<T> operator/(const Vector<T>& a,const Vector<T>& b){
  Vector<T> r;
  unsigned int n=a.size();
  for(unsigned int i=0;i<n;++i) r.push_back(a[i]/b[i]);
  return(r);
}
//////////////////////////////////////////////////////////////////////
template<class T> 
bool operator==(const Vector<T>& a,const Vector<T>& b){
  unsigned int n=a.size();
  for(unsigned int i=0;i<n;++i) if(a[i]!=b[i]) return(false);
  return(true);
}
//////////////////////////////////////////////////////////////////////
template<class T> 
bool operator!=(const Vector<T>& a,const Vector<T>& b){
  unsigned int n=a.size();
  for(unsigned int i=0;i<n;++i) if(a[i]!=b[i]) return(true);
  return(false);
}
//////////////////////////////////////////////////////////////////////
template<class T> 
bool operator>(const Vector<T>& a,const Vector<T>& b){
#ifdef COMPABS
  return(sum2(a)>sum2(b));
#else
  unsigned int n=a.size();
  T diff=0;
  for(unsigned int i=0;i<n;++i) diff += a[i]-b[i];
  return(diff>T()?true:false);
#endif
}
//////////////////////////////////////////////////////////////////////
template<class T> 
bool operator<(const Vector<T>& a,const Vector<T>& b){
#ifdef COMPABS
  return(sum2(a)<sum2(b));
#else
  unsigned int n=a.size();
  T diff=0;
  for(unsigned int i=0;i<n;++i) diff += a[i]-b[i];
  return(diff<T()?true:false);
#endif
}
//////////////////////////////////////////////////////////////////////
template<class T> 
bool operator>=(const Vector<T>& a,const Vector<T>& b){
#ifdef COMPABS
  return(sum2(a)>=sum2(b));
#else
  unsigned int n=a.size();
  T diff=0;
  for(unsigned int i=0;i<n;++i) diff += a[i]-b[i];
  return(diff>=T()?true:false);
#endif
}
//////////////////////////////////////////////////////////////////////
template<class T> 
bool operator<=(const Vector<T>& a,const Vector<T>& b){
#ifdef COMPABS
  return(sum2(a)<=sum2(b));
#else
  unsigned int n=a.size();
  T diff=0;
  for(unsigned int i=0;i<n;++i) diff += a[i]-b[i];
  return(diff<=T()?true:false);
#endif
}
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
template<class T> 
Vector<T> operator+(const Vector<T>& a,const T& b){
  Vector<T> r;
  unsigned int n=a.size();
  for(unsigned int i=0;i<n;++i) r.push_back(a[i]+b);
  return(r);
}
//////////////////////////////////////////////////////////////////////
template<class T> 
Vector<T> operator-(const Vector<T>& a,const T& b) {
  Vector<T> r;
  unsigned int n=a.size();
  for(unsigned int i=0;i<n;++i) r.push_back(a[i]-b);
  return(r);
}
//////////////////////////////////////////////////////////////////////
template<class T> 
Vector<T> operator*(const Vector<T>& a,const T& b){
  Vector<T> r;
  unsigned int n=a.size();
  for(unsigned int i=0;i<n;++i) r.push_back(a[i]*b);
  return(r);
}
//////////////////////////////////////////////////////////////////////
template<class T> 
Vector<T> operator/(const Vector<T>& a,const T& b){
  Vector<T> r;
  unsigned int n=a.size();
  for(unsigned int i=0;i<n;++i) r.push_back(a[i]/b);
  return(r);
}
//////////////////////////////////////////////////////////////////////
template<class T> 
Vector<T> operator^(const Vector<T>& a,const T& b){
  Vector<T> r;
  unsigned int n=a.size();
  for(unsigned int i=0;i<n;++i) r.push_back(pow(a[i],b));
  return(r);
}
//////////////////////////////////////////////////////////////////////
template<class T> 
Vector<T> pow(const Vector<T>& a,const T& b){
  Vector<T> r;
  unsigned int n=a.size();
  T tmp;
  for(unsigned int i=0;i<n;++i) {
    tmp = pow(a[i],b);
    r.push_back(tmp);
  }
  return(r);
}
//////////////////////////////////////////////////////////////////////
template<class T> 
Vector<T> pow(const T& a,const Vector<T>& b){
  Vector<T> r;
  unsigned int n=b.size();
  T tmp;
  for(unsigned int i=0;i<n;++i) {
    tmp = pow(a,b[i]);
    r.push_back(tmp);
  }
  return(r);
}
//////////////////////////////////////////////////////////////////////
template<class T> 
Vector<T> pow(const Vector<T>& a,const Vector<T>& b){
  Vector<T> r;
  unsigned int n= a.size();
  T tmp;
  for(unsigned int i=0;i<n;++i) {
    tmp = pow(a[i],b[i]);
    r.push_back(tmp);
  }
  return(r);
}
//////////////////////////////////////////////////////////////////////
template<class T> 
Vector<T> operator+(const T& a,const Vector<T>& b){
  Vector<T> r;
  unsigned int n=b.size();
  for(unsigned int i=0;i<n;++i) r.push_back(a+b[i]);
  return(r);
}
//////////////////////////////////////////////////////////////////////
template<class T> 
Vector<T> operator-(const T& a,const Vector<T>& b){
  Vector<T> r;
  unsigned int n=b.size();
  for(unsigned int i=0;i<n;++i) r.push_back(a-b[i]);
  return(r);
}
//////////////////////////////////////////////////////////////////////
template<class T> 
Vector<T> operator*(const T& a,const Vector<T>& b){
  Vector<T> r;
  unsigned int n=b.size();
  for(unsigned int i=0;i<n;++i) r.push_back(a*b[i]);
  return(r);
}
//////////////////////////////////////////////////////////////////////
template<class T> 
Vector<T> operator/(const T& a,const Vector<T>& b){
  Vector<T> r;
  unsigned int n=b.size();
  for(unsigned int i=0;i<n;++i) r.push_back(a/b[i]);
  return(r);
}

#if 0
//////////////////////////////////////////////////////////////////////
template<class T> 
Vector<T> operator+(const Vector<T>& a,double b){
  Vector<T> r;
  unsigned int n=a.size();
  for(unsigned int i=0;i<n;++i) r.push_back(a[i]+b);
  return(r);
}
//////////////////////////////////////////////////////////////////////
template<class T> 
Vector<T> operator-(const Vector<T>& a,double b) {
  Vector<T> r;
  unsigned int n=a.size();
  for(unsigned int i=0;i<n;++i) r.push_back(a[i]-b);
  return(r);
}
//////////////////////////////////////////////////////////////////////
template<class T> 
Vector<T> operator*(const Vector<T>& a,double b){
  Vector<T> r;
  unsigned int n=a.size();
  for(unsigned int i=0;i<n;++i) r.push_back(a[i]*b);
  return(r);
}
//////////////////////////////////////////////////////////////////////
template<class T> 
Vector<T> operator/(const Vector<T>& a,double b){
  Vector<T> r;
  unsigned int n=a.size();
  for(unsigned int i=0;i<n;++i) r.push_back(a[i]/b);
  return(r);
}
//////////////////////////////////////////////////////////////////////
template<class T> 
Vector<T> operator^(const Vector<T>& a,double b){
  Vector<T> r;
  unsigned int n=a.size();
  for(unsigned int i=0;i<n;++i) r.push_back(pow(a[i],b));
  return(r);
}
#endif
#if 0
//////////////////////////////////////////////////////////////////////
template<class T> 
Vector<T> operator+(const Vector<T>& a,int b){
  Vector<T> r;
  unsigned int n=a.size();
  for(unsigned int i=0;i<n;++i) r.push_back(a[i]+b);
  return(r);
}
//////////////////////////////////////////////////////////////////////
template<class T> 
Vector<T> operator-(const Vector<T>& a,int b) {
  Vector<T> r;
  unsigned int n=a.size();
  for(unsigned int i=0;i<n;++i) r.push_back(a[i]-b);
  return(r);
}
//////////////////////////////////////////////////////////////////////
template<class T> 
Vector<T> operator*(const Vector<T>& a,int b){
  Vector<T> r;
  unsigned int n=a.size();
  for(unsigned int i=0;i<n;++i) r.push_back(a[i]*b);
  return(r);
}
//////////////////////////////////////////////////////////////////////
template<class T> 
Vector<T> operator/(const Vector<T>& a,int b){
  Vector<T> r;
  unsigned int n=a.size();
  for(unsigned int i=0;i<n;++i) r.push_back(a[i]/b);
  return(r);
}
//////////////////////////////////////////////////////////////////////
template<class T> 
Vector<T> operator^(const Vector<T>& a,int b){
  Vector<T> r;
  unsigned int n=a.size();
  for(unsigned int i=0;i<n;++i) r.push_back(pow(a[i],b));
  return(r);
}
#endif

//////////////////////////////////////////////////////////////////////
template<class T> 
Vector<T> operator+(int a,const Vector<T>& b){
  Vector<T> r;
  int n=b.size();
  for(unsigned int i=0;i<n;++i) r.push_back(a+b[i]);
  return(r);
}
//////////////////////////////////////////////////////////////////////
template<class T> 
Vector<T> operator-(int a,const Vector<T>& b){
  Vector<T> r;
  int n=b.size();
  for(unsigned int i=0;i<n;++i) r.push_back(a-b[i]);
  return(r);
}
//////////////////////////////////////////////////////////////////////
template<class T> 
Vector<T> operator*(int a,const Vector<T>& b){
  Vector<T> r;
  int n=b.size();
  for(unsigned int i=0;i<n;++i) r.push_back(a*b[i]);
  return(r);
}
//////////////////////////////////////////////////////////////////////
template<class T> 
Vector<T> operator/(int a,const Vector<T>& b){
  Vector<T> r;
  int n=b.size();
  for(unsigned int i=0;i<n;++i) r.push_back(a/b[i]);
  return(r);
}

//////////////////////////////////////////////////////////////////////
template<class T> 
Vector<T> exp(const Vector<T>& a){
  Vector<T> r;
  unsigned int n=a.size();
  T tmp;
  for(unsigned int i=0;i<n;++i) {
    tmp = exp(a[i]);
    r.push_back(tmp);
  }
  return(r);
}
//////////////////////////////////////////////////////////////////////
template<class T> 
Vector<T> log(const Vector<T>& a){
  Vector<T> r;
  unsigned int n=a.size();
  T tmp;
  for(unsigned int i=0;i<n;++i) {
    tmp = log(a[i]);
    r.push_back(tmp);
  }
  return(r);
}
//////////////////////////////////////////////////////////////////////
template<class T> 
Vector<T> log10(const Vector<T>& a){
  Vector<T> r;
  unsigned int n=a.size();
  T tmp;
  for(unsigned int i=0;i<n;++i) {
    tmp = log10(a[i]);
    r.push_back(tmp);
  }
  return(r);
}
//////////////////////////////////////////////////////////////////////
template<class T> 
Vector<T> sqrt(const Vector<T>& a){
  Vector<T> r;
  unsigned int n=a.size();
  T tmp;
  for(unsigned int i=0;i<n;++i) {
    tmp = sqrt(a[i]);
    r.push_back(tmp);
  }
  return(r);
}
//////////////////////////////////////////////////////////////////////
template<class T> 
Vector<T> sinc(const Vector<T>& a){
  Vector<T> r;
  unsigned int n=a.size();
  T tmp;
  for(unsigned int i=0;i<n;++i) {
    if(a[i]!=0) tmp = sin(a[i])/a[i];
    else        tmp = 1;
    r.push_back(tmp);
  }
  return(r);
}
//////////////////////////////////////////////////////////////////////
template<class T> 
Vector<T> sin(const Vector<T>& a){
  Vector<T> r;
  unsigned int n=a.size();
  T tmp;
  for(unsigned int i=0;i<n;++i) {
    tmp = sin(a[i]);
    r.push_back(tmp);
  }
  return(r);
}
//////////////////////////////////////////////////////////////////////
template<class T> 
Vector<T> cos(const Vector<T>& a){
  Vector<T> r;
  unsigned int n=a.size();
  T tmp;
  for(unsigned int i=0;i<n;++i) {
    tmp = cos(a[i]);
    r.push_back(tmp);
  }
  return(r);
}
//////////////////////////////////////////////////////////////////////
template<class T> 
Vector<T> tan(const Vector<T>& a){
  Vector<T> r;
  unsigned int n=a.size();
  T tmp;
  for(unsigned int i=0;i<n;++i) {
    tmp = tan(a[i]);
    r.push_back(tmp);
  }
  return(r);
}
//////////////////////////////////////////////////////////////////////
template<class T> 
Vector<T> asin(const Vector<T>& a){
  Vector<T> r;
  unsigned int n=a.size();
  T tmp;
  for(unsigned int i=0;i<n;++i) {
    tmp = asin(a[i]);
    r.push_back(tmp);
  }
  return(r);
}
//////////////////////////////////////////////////////////////////////
template<class T> 
Vector<T> acos(const Vector<T>& a){
  Vector<T> r;
  unsigned int n=a.size();
  T tmp;
  for(unsigned int i=0;i<n;++i) {
    tmp = acos(a[i]);
    r.push_back(tmp);
  }
  return(r);
}
//////////////////////////////////////////////////////////////////////
template<class T> 
Vector<T> atan(const Vector<T>& a){
  Vector<T> r;
  unsigned int n=a.size();
  T tmp;
  for(unsigned int i=0;i<n;++i) {
    tmp = tan(a[i]);
    r.push_back(tmp);
  }
  return(r);
}
//////////////////////////////////////////////////////////////////////
template<class T> 
Vector<T> fabs(const Vector<T>& a){
  Vector<T> r;
  unsigned int n=a.size();
  T tmp;
  for(unsigned int i=0;i<n;++i) {
    tmp = fabs(a[i]);
    r.push_back(tmp);
  }
  return(r);
}
//////////////////////////////////////////////////////////////////////
template<class T> 
Vector<T> abs(const Vector<T>& a){
  Vector<T> r;
  unsigned int n=a.size();
  //for(unsigned int i=0;i<n;++i) r.push_back(fabs(a[i]));
  T tmp;
  for(unsigned int i=0;i<n;++i) {
    tmp = fabs(a[i]);
    r.push_back(tmp);
  }
  return(r);
}

//////////////////////////////////////////////////////////////////////
template<class T> 
Vector<T> accum(const Vector<T>& a){
  Vector<T> r;
  unsigned int n=a.size();
  T s=0;
  for(unsigned int i=0;i<n;++i) {
    s += a[i];
    r.push_back(s);
  }
  return(r);
}

//////////////////////////////////////////////////////////////////////
template<class T> 
T sum(const Vector<T>& a){
  T r=0;
  unsigned int n=a.size();
  for(unsigned int i=0;i<n;++i) r = r + a[i];
  return(r);
}

//////////////////////////////////////////////////////////////////////
template<class T> 
T sum2(const Vector<T>& a){
  T r=0;
  unsigned int n=a.size();
  for(unsigned int i=0;i<n;++i) r = r + a[i]*a[i];
  return(r);
}

//////////////////////////////////////////////////////////////////////
template<class T>
static void G__ary_rect(T *c,const T *a,int n) {
  int i;
  for(i=0;i<n;i++) {
    if(-0.5<a[i] && a[i]<0.5) 
      c[i]=1.0;
    else if(a[i]==0.5 || a[i]==0.5)
      c[i]=0.5;
    else
      c[i]=0.0;
  }
}
template<class T> 
Vector<T> rect(const Vector<T>& a){
  Vector<T> c=Vector<T>(0.0 , 0.0 , a.size());
  G__ary_rect(&c[0],&a[0],a.size());
  return(c);
}

//////////////////////////////////////////////////////////////////////
template<class T>
static void G__ary_square(T *c,const T *a,int n) {
  int i;
  T tmp;
  for(i=0;i<n;i++) {
    tmp = sin(a[i]);
    if(tmp<0)       c[i] = -1.0;
    else if(tmp==0) c[i]=   0.0;
    else            c[i] =  1.0;
  }
}
template<class T> 
Vector<T> square(const Vector<T>& a){
  Vector<T> c=Vector<T>(0.0 , 0.0 , a.size());
  G__ary_square(&c[0],&a[0],a.size());
  return(c);
}

//////////////////////////////////////////////////////////////////////
template<class T>
static void G__ary_triangle(T *c,const T *a,int n) {
  int i;
  T tmp,prev=0,accum=0;
  for(i=0;i<n;i++) {
    tmp = sin(a[i]);
    c[i]=accum++;
    if(i && tmp>=0 && prev<0) {
      accum=0;
    }
    prev = tmp;
  }
  tmp = -1e-99;
  for(i=0;i<n;i++) {
    if(tmp<c[i]) tmp = c[i];
  }
  for(i=0;i<n;i++) {
    c[i] /= tmp;
  }
}
template<class T> 
Vector<T> triangle(const Vector<T>& a){
  Vector<T> c=Vector<T>(0.0 , 0.0 , a.size());
  G__ary_triangle(&c[0],&a[0],a.size());
  return(c);
}

//////////////////////////////////////////////////////////////////////
template<class T>
static void G__ary_rand(T *c,const T *a,int n) {
  int i;
  for(i=0;i<n;i++) {
    c[i] = (rand()%20000-10000)/10000.0;
  }
}

template<class T> 
Vector<T> rand(const Vector<T>& a){
  Vector<T> c=Vector<T>(0.0 , 0.0 , a.size());
  G__ary_rand(&c[0],&a[0],a.size());
  return(c);
}

//////////////////////////////////////////////////////////////////////
template<class T>
static void G__ary_pulse(T *c,const T *a,unsigned int n) {
  unsigned int i=0;
  T tmp;
  c[i] = 1;
  for(i=1;i<n;i++) {
    tmp = sin(a[i]);
    if(tmp>=0 && c[i-1]<0) c[i]=1;
    else if(tmp<=0 && c[i-1]>0) c[i]= -1;
    else c[i] = 0;
  }
}

template<class T> 
Vector<T> pulse(const Vector<T>& a){
  Vector<T> c=Vector<T>(0.0 , 0.0 , a.size());
  G__ary_pulse(&c[0],&a[0],a.size());
  return(c);
}

//////////////////////////////////////////////////////////////////////
template<class T>
static void G__ary_conv(T* c,const T* a,unsigned int n,const T* b,unsigned int m)
{
  unsigned int i,j;
  int k;
  int f,t;
  f = m/2;
  t = m-f;
  for(i=0;i<n;i++) {
    c[i]=0.0;
    for(j=0;j<m;j++) {
      k=i-f+j;
      if(k<0)       c[i] += a[0]*b[j];
      else if(k>=n) c[i] += a[n-1]*b[j];
      else          c[i] += a[k]*b[j];
    }
  }
}
///////////////////////////////////////////////////////////////////////
template<class T> 
Vector<T> conv(const Vector<T>& a,const Vector<T>& b){
  Vector<T> c=Vector<T>(0.0 , 0.0 , a.size());
  G__ary_conv(&c[0],&a[0],a.size(),&b[0],b.size());
  return(c);
}

//////////////////////////////////////////////////////////////////////
template<class T> 
Vector<T> integ(const Vector<T>& x,const Vector<T>& a){
  Vector<T> r;
  unsigned int n=a.size();
  T s=0;
  r.push_back(s);
  for(unsigned int i=1;i<n;++i) {
    s += (a[i]+a[i-1])*(x[i]-x[i-1])/2;
    r.push_back(s);
  }
  return(r);
}
//////////////////////////////////////////////////////////////////////
template<class T> 
Vector<T> integ(const Vector<T>& a){
  return(accum(a));
}

//////////////////////////////////////////////////////////////////////
template<class T> 
Vector<T> diff(const Vector<T>& x,const Vector<T>& a){
  Vector<T> r;
  unsigned int n=a.size();
  T s=0;
  r.push_back(s);
  for(unsigned int i=1;i<n;++i) {
    s = (a[i]-a[i-1])/(x[i]-x[i-1]);
    r.push_back(s);
  }
  return(r);
}
//////////////////////////////////////////////////////////////////////
template<class T> 
Vector<T> diff(const Vector<T>& a){
  Vector<T> r;
  unsigned int n=a.size();
  T s=0;
  r.push_back(s);
  for(unsigned int i=1;i<n;++i) {
    s = (a[i]-a[i-1]);
    r.push_back(s);
  }
  return(r);
}

//////////////////////////////////////////////////////////////////////
// Resample un-even time domain data to make even and n^2 size vector for FFT
//////////////////////////////////////////////////////////////////////
template<class T> 
int resamplePowerOf2(const Vector<T>& x,const Vector<T>& a
		       ,Vector<T>& rx,Vector<T>& ra
		       ,T period,T delay=0){
  // calculate rx range
  T xrange = x.back() - x.front() - delay;
  int Ncycle = xrange/period;
  T rxrange = period*Ncycle;

  // calculate sample count n2 and drx;
  // calculate dx statistics
  Vector<T> d = diff(x);
  Stat<T> st;
  unsigned int i;
  for(i=1;i<d.size();i++) st.add(d[i]);
  // from dx statistics and rxrange, calculate crude number of sample
  T dxmean = st.mean();
  int ncrude = rxrange/dxmean;
  // find the closest n^2 to ncrude
  int N2=4;
  while(N2<ncrude) N2 *= 2;
  if((double)N2/(double)ncrude>1.1) N2 /= 2;
  // calculate drx
  T drx = rxrange/N2;

  // calculate xstart
  T xnow, xstart = x.back() - rxrange - drx;
  
  // data interpolation
  rx.clear(); ra.clear();
  unsigned int ix=0;
  for(i=0;i<N2;i++) {
    xnow = xstart + drx*i;
    rx.push_back(xnow);
    while(ix<x.size() && x[ix]<xnow) ++ix;
    if(ix>1 && ix<x.size()) {
      T x1 = xnow-x[ix-1];
      T x2 = x[ix]-xnow;
      T val = (a[ix-1]*x1 + a[ix]*x2)/(x1+x2);
      ra.push_back(val);
    }
    else {
      // error
      return(255);
    }
  }
  
  return(0);
}


//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
template<class T> 
Vector<T> movingave(const Vector<T>& a,int average,int begin_end=0){
  Vector<T> r;
  unsigned int n=a.size();
  T s=0;
  T val;
  unsigned int i;
  if(begin_end==0) {
    for(i=0;i<(unsigned int)average;i++) {
      s += a[i];
      val = s/(i+1);
      r.push_back(val);
    }
    for(i=(unsigned int)average;i<n;i++) {
      s += (a[i]-a[i-average]);
      val = s/average;
      r.push_back(val);
    }
  }
  else {
    // Following part is not qualified. May be some errors.
    for(i=0;i<(unsigned int)average;i++) {
      s += a[i];
      val = s/average;
    }
    r.push_back(val);
    for(i=0;i<n-(unsigned int)average;i++) {
      s += (a[i+(unsigned int)average]-a[i]);
      val = s/average;
      r.push_back(val);
    }
    for(i=n-(unsigned int)average;i<n-1;i++) {
      s -= a[i];
      val = s/(n-i);
      r.push_back(val);
    }
  }
  return(r);
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/***********************************************
* FIR filter
***********************************************/
template<class T> 
Vector<T> fir(const Vector<T>& in,const Vector<T>& filter) {
  // convolution is done in conv(). This function
  // simply does energy normalization 
  Vector<T> out(0.0,0.0,in.size());
  unsigned int i,k;
  Vector<T> fil=Vector<T>(0.0,0.0 ,filter.size());
  T sum=0;
  k=fil.size();
  for(i=0;i<k;i++) sum += filter[i];
  fil = filter/sum;
  out=conv(in,fil);
  return(out);
}

/***********************************************
* IIR filter
***********************************************/
template<class T> 
Vector<T> iir1_calc(const Vector<T>& in,const T& k,const T& norm) {
  Vector<T> out(0.0,0.0,in.size());
  unsigned int n=in.size();
  out[0] = in[0]*k;
  for(unsigned int i=1;i<n;i++) {
    out[i] = (in[i]+out[i-1])*k;
  }
  return(out/norm);
}

///////////////////////////////////////////////
template<class T>
Vector<T> iir1(const Vector<T>& in,double Ts,double tau) {
  ////////////////////////////////////////////
  // IIR low pass filter
  //  |                     .      .    .    1     ----
  //  |             .                                exp(-1) = k^(tau/Ts)
  //  |        .  <---------------------- 1-exp(-1) ---
  //  |    .    
  //  |  .     ^
  //  | .      |
  //   -----------------------------------
  //   ||     tau
  //   Ts
  //
  // From above figure, k is obtained by following formula 
  double k=pow(exp(-1),Ts/tau);
  
  // calculate normalization factor
  //        +----------+
  //        |          |  norm = sigma k^i | i=0..infinity
  //        v          |
  //   --->(+)--- *k --+---> /norm --->
  //
  double norm=0,r=k;
  while(r>0.001) { 
    norm += r;
    r *= k;
  }

  printf("IIR1 Ts=%.3g(fs=%.3g) tau=%.3g(fc=%.3g) coefficient k=%.3g\n"
	 ,Ts,1/Ts/2/3.14,tau,1/tau/2/3.14,k);
  return(iir1_calc(in,k,norm));
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
template<class T> 
Vector<T> min(const Vector<T>& x,const Vector<T>& y) {
  Vector<T> r;
  unsigned int n=x.size();
  for(unsigned int i=0;i<n;++i) {
    if(x[i]>y[i]) r.push_back(y[i]);
    else          r.push_back(x[i]);
  }
  return(r);
}

//////////////////////////////////////////////////////////////////////
template<class T> 
Vector<T> max(const Vector<T>& x,const Vector<T>& y) {
  Vector<T> r;
  unsigned int n=x.size();
  for(unsigned int i=0;i<n;++i) {
    if(x[i]>y[i]) r.push_back(x[i]);
    else          r.push_back(y[i]);
  }
  return(r);
}

//////////////////////////////////////////////////////////////////////
template<class T> 
Vector<T> min(const Vector<T>& x,const T& y) {
  Vector<T> r;
  unsigned int n=x.size();
  for(unsigned int i=0;i<n;++i) {
    if(x[i]>y) r.push_back(y);
    else       r.push_back(x[i]);
  }
  return(r);
}

//////////////////////////////////////////////////////////////////////
template<class T> 
Vector<T> max(const Vector<T>& x,const T& y) {
  Vector<T> r;
  unsigned int n=x.size();
  for(unsigned int i=0;i<n;++i) {
    if(x[i]>y) r.push_back(x[i]);
    else       r.push_back(y);
  }
  return(r);
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
template<class T>
T mean(const Vector<T>& v) {
  T s = sum(v);
  return(s/v.size());
}

//////////////////////////////////////////////////////////////////////
template<class T>
T min(const Vector<T>& v) {
  T m=1e37;
  for(unsigned int i=0;i<v.size();i++) if(m>v[i]) m = v[i];
  return(m);
}

//////////////////////////////////////////////////////////////////////
template<class T>
T max(const Vector<T>& v) {
  T m=-1e37;
  for(unsigned int i=0;i<v.size();i++) if(m<v[i]) m = v[i];
  return(m);
}

//////////////////////////////////////////////////////////////////////
template<class T>
int argmin(const Vector<T>& v) {
  T m=1e37;
  unsigned int  argi = 0;
  for(unsigned int i=0;i<v.size();i++) if(m>v[i]) { m = v[i] ; argi = i; }
  return(argi);
}
//////////////////////////////////////////////////////////////////////
template<class T>
int argmax(const Vector<T>& v) {
  T m=-1e37;
  unsigned int  argi = 0;
  for(unsigned int i=0;i<v.size();i++) if(m<v[i]) { m = v[i] ; argi = i; }
  return(argi);
}


//////////////////////////////////////////////////////////////////////
template<class T>
unsigned int indexmin(const Vector<T>& v) {
  T m=1e37;
  unsigned int index=0;
  for(unsigned int i=0;i<v.size();i++) if(m>v[i]) { m=v[i]; index = i; }
  return(index);
}
//////////////////////////////////////////////////////////////////////
template<class T>
unsigned int indexmax(const Vector<T>& v) {
  T m=-1e37;
  unsigned int index=0;
  for(unsigned int i=0;i<v.size();i++) if(m<v[i]) { m=v[i]; index = i; }
  return(index);
}
//////////////////////////////////////////////////////////////////////
template<class T>
vector<int> indexrange(const Vector<T>& v,const T min,const T max) {
  vector<int> index;
  for(unsigned int i=0;i<v.size();i++) {
    if(min<=v[i] && v[i]<=max) index.push_back(i);
  }
  return(index);
}


//////////////////////////////////////////////////////////////////////
template<class T>
double stddev(const Vector<T>& v) {
  return(rms(v));
}

//////////////////////////////////////////////////////////////////////
template<class T>
double rms(const Vector<T>& v) {
  T vmean = mean(v);
  double sum=0;
  T tmp;
  for(unsigned i=0;i<v.size();i++) {
    tmp = v[i]-vmean;
    sum += tmp*tmp;
  }
  return(sqrt(sum/v.size()));
}

//////////////////////////////////////////////////////////////////////
template<class T>
T pp(const Vector<T>& v) {
  T min=1e37,max=-1e37;
  for(unsigned i=0;i<v.size();i++) {
    if(min>v[i]) min = v[i];
    if(v[i]>max) max = v[i];
  }
  return(max-min);
}

//////////////////////////////////////////////////////////////////////
template<class T>
T distance2(const Vector<T>& a,const Vector<T>& b) {
  T r2=0;
  T c;
  for(unsigned i=0;i<a.size()&&i<b.size();i++) {
    c = a[i]-b[i];
    r2 += c * c;
  }
  return(r2);
}

//////////////////////////////////////////////////////////////////////
#if 1
template<class T>
T distance1(const Vector<T>& a,const Vector<T>& b) {
  T r = distance2(a,b);
  return(sqrt(r));
}
#endif

//////////////////////////////////////////////////////////////////////
template<class T>
Vector<T> gauss(const Vector<T>& scale,const Vector<T>& hist) {
  Vector<T> g(0.0,0.0,hist.size());
  T m=sum(scale*hist);
  Vector<T> s2 = scale-m;
  T sd = sqrt(sum(s2*s2*hist));
  T pi2sqsd = 1/sqrt(2*3.141592)*sd;
  T sd22 = 2*sd*sd;
  T sum=0;
  for(unsigned int i=0;i<hist.size();i++) {
    g[i] = pi2sqsd * exp(-pow(scale[i]-m,2)/sd22);
    sum += g[i];
  } 
  g = g/sum;
  return(g);
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
template<class T>
int findpos(const Vector<T>& x,const T& val) {
  unsigned int n=x.size();
  for(unsigned int i=0;i<n;i++) if(x[i]>=val) return(i);
  return(-1);
}
//////////////////////////////////////////////////////////////////////
template<class T>
Vector<T> range(const Vector<T>& x,unsigned int i1,unsigned int i2) {
  Vector<T> r;
  if(i1<0||i1>x.size() || i2<0||i2>x.size() || i1>i2) {
    fprintf(stderr,"Error:Vector<T>::range, illegal index range %d %d, %d\n"
            ,i1,i2,x.size());
    return(r);
  }
  for(unsigned int i=i1;i<i2;i++) r.push_back(x[i]);
  return(r);
}

//////////////////////////////////////////////////////////////////////
template<class T>
Vector<T> linear(T start,T stop,unsigned int n) {
  Vector<T> r;
  T step=(stop-start)/(n-1);
  for(unsigned int i=0;i<n;i++) r.push_back(start+step*i);
  return(r);
}

//////////////////////////////////////////////////////////////////////
template<class T>
Vector<T> gauss(const Vector<T>& x,T mean,T stddev) {
  Vector<T> r;
  unsigned int n = x.size();
  for(unsigned int i=0;i<n;i++) r.push_back(gauss(x[i],mean,stddev));
  return(r);
}

//////////////////////////////////////////////////////////////////////
#if 0
template<class T>
Vector<T> gauss(const Vector<T>& x
		,const Vector<T>& mean,const Vector<T>& stddev) {
  Vector<T> r;
  unsigned int n = x.size();
  for(unsigned int i=0;i<n;i++) r.push_back(gauss(x[i],mean[i],stddev[i]));
  return(r);
}
#endif


//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

#ifdef __STAT__
//////////////////////////////////////////////////////////////////////
inline statistics stat(const Vector<double>& x) {
  statistics stat;
  unsigned int n=x.size();
  for(unsigned int i=0;i<n;i++) stat.add(x[i]);
  return(stat);
}
//////////////////////////////////////////////////////////////////////
inline statistics stat(const Vector<double>& x,const Vector<double>& y
                ,double xstart,double xend) {
  statistics stat;
  unsigned int n=x.size();
  for(unsigned int i=0;i<n;i++) {
    if(xstart<=x[i] && x[i]<xend) stat.add(y[i]);
  }
  return(stat);
}
#if 0
//////////////////////////////////////////////////////////////////////
inline double mean(const Vector<double>& x){
  statistics stat;
  unsigned int n=x.size();
  for(unsigned int i=0;i<n;i++) stat.add(x[i]);
  return(stat.mean());
}
//////////////////////////////////////////////////////////////////////
inline double stddev(const Vector<double>& x){
  statistics stat;
  unsigned int n=x.size();
  for(unsigned int i=0;i<n;i++) stat.add(x[i]);
  return(stat.stddev());
}
//////////////////////////////////////////////////////////////////////
inline double min(const Vector<double>& x){
  statistics stat;
  unsigned int n=x.size();
  for(unsigned int i=0;i<n;i++) stat.add(x[i]);
  return(stat.min());
}

//////////////////////////////////////////////////////////////////////
inline double max(const Vector<double>& x){
  statistics stat;
  unsigned int n=x.size();
  for(unsigned int i=0;i<n;i++) stat.add(x[i]);
  return(stat.max());
}
#endif
//////////////////////////////////////////////////////////////////////
#else
#endif
//////////////////////////////////////////////////////////////////////
inline double Min(const Vector<double>& x){
  unsigned int n=x.size();
  double m = 1e99;
  for(unsigned int i=0;i<n;i++) if(m>x[i]) m=x[i];
  return(m);
}

//////////////////////////////////////////////////////////////////////
inline double Max(const Vector<double>& x){
  unsigned int n=x.size();
  double m = -1e99;
  for(unsigned int i=0;i<n;i++) if(m<x[i]) m=x[i];
  return(m);
}

//////////////////////////////////////////////////////////////////////
inline float Min(const Vector<float>& x){
  unsigned int n=x.size();
  float m = 1e37;
  for(unsigned int i=0;i<n;i++) if(m>x[i]) m=x[i];
  return(m);
}

//////////////////////////////////////////////////////////////////////
inline float Max(const Vector<float>& x){
  unsigned int n=x.size();
  float m = -1e37;
  for(unsigned int i=0;i<n;i++) if(m<x[i]) m=x[i];
  return(m);
}

//////////////////////////////////////////////////////////////////////
template<class T>
T innerproduct(const Vector<T>& a,const Vector<T>& b){
  T m = 0;
  unsigned int i;
  for(i=0;i<a.size() && i<b.size();i++) {
    m += a[i] * b[i];
  }
  return(m);
}

//////////////////////////////////////////////////////////////////////
template<class T>
T normalizedinnerproduct(const Vector<T>& a,const Vector<T>& b){
  T sum=0.0,suma=0.0,sumb=0.0;
  unsigned int i;
  for(i=0;i<a.size() && i<b.size();i++) {
    sum += a[i] * b[i];
    suma += a[i] * a[i];
    sumb += b[i] * b[i];
  }
  if((suma+sumb)==0.0) return(1.0);
  return(sum/(suma+sumb)*2);
}

//////////////////////////////////////////////////////////////////////
template<class T>
T pearsoncorrelation(const Vector<T>& a,const Vector<T>& b){
  T meana=0.0,meanb=0.0;
  unsigned int i;
  // calculate meana, meanb;
  for(i=0;i<a.size() && i<b.size();i++) {
    meana+=a[i]; 
    meanb+=b[i]; 
  }
  if(i==0) return(0.0);
  meana /= i; 
  meanb /= i;

  T sumab=0.0, suma2=0.0, sumb2=0.0,tmp;
  for(i=0;i<a.size() && i<b.size();i++) {
    sumab += (a[i]-meana)*(b[i]-meanb);
    suma2 += (a[i]-meana)*(a[i]-meana);
    sumb2 += (b[i]-meanb)*(b[i]-meanb);
  }
  T r = sumab / (sqrt(suma2)*sqrt(sumb2));
  return(r);
}

//////////////////////////////////////////////////////////////////////
template<class T>
T cosinesimilarity(const Vector<T>& a,const Vector<T>& b){
  T sum=0.0, suma=0.0 ,sumb=0.0, r ;
  unsigned int i;
  for(i=0;i<a.size() && i<b.size();i++) {
    sum += a[i] * b[i];
    suma += a[i]*a[i];
    sumb += b[i]*b[i];
  }
  if(suma==0 && sumb==0) r=1.0;
  else r = sum / (sqrt(suma)*sqrt(sumb));
  return(r);
}


//////////////////////////////////////////////////////////////////////
template<class T>
T tanimotocoefficient(const Vector<T>& a,const Vector<T>& b){
  T sum=0.0, suma=0.0 ,sumb=0.0, r ;
  unsigned int i;
  for(i=0;i<a.size() && i<b.size();i++) {
    sum += a[i] * b[i];
    suma += a[i]*a[i];
    sumb += b[i]*b[i];
  }
  if(suma==0 && sumb==0) r=1.0;
  else r = sum / (suma+sumb-sum);
  return(r);
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
template<class T>
Vector<Vector<T> > similaritymatrix(const Vector<Vector<T> >& m,T (*simfunc)(const Vector<T>&,const Vector<T>&)) {
  T tmp;
  Vector<Vector<T> > simmat;
  unsigned int i,j,Size = m.size(), VSize;
  simmat.resize(Size);
  if(Size) VSize = m[0].size();
  T vmin=FLT_MAX,vmax=FLT_MIN, val;
  for(i=0;i<Size;i++) simmat[i].resize(Size);
  for(i=0;i<Size;i++) {
    for(j=i;j<Size;j++) {
      tmp = simfunc(m[i],m[j]);
      simmat[i][j] = tmp;
      simmat[j][i] = tmp;
    }
  }
  return(simmat);
}



//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
template<class T>
Vector<T> resample(const Vector<T>& xin,const Vector<T>& yin
		  ,const Vector<T>& x) {
// TO BE COMPLETED
  Vector<T> r;
  int nin=xin.size();
  int n=x.size();
  int i=0,j=0;
  int xpre=0;

  if(xin[j]>x[i]) {
    //   xin   <-->xxxxxxxxx
    //   x     xxxxxxxxxxxxx
    while(xin[j]>x[i]) ++i;
    xpre=i;
  }
  while(xin[j]<x[i]) ++j;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

#if 1
//////////////////////////////////////////////////////////////////////
template<class T>
class VectorPeak {
 public:
  T val;
  int index;
  int min_or_max;
};

//////////////////////////////////////////////////////////////////////
template<class T> 
int PeakAroundN(const Vector<T>& v,Vector<VectorPeak<T> >& vp
		,const T& thresh,unsigned int range=2) {
  int i,j;
  VectorPeak<T> p;
  T val;
  int nmaxflag;
  vp.clear();
  for(i=0;i<v.size();i++) {
    val = v.get(i);
    if(val<thresh) continue;
    nmaxflag=0;
    for(j=Max_(0,i-range);j<Min_((int)v.size(),i+range);j++) {
      //if(i==j) continue;
      if(val<v.get(j)) ++nmaxflag;
      else if(val==v.get(j) && i<j) ++nmaxflag;
    }
    if(0==nmaxflag) {
      p.val=val;
      p.index = i;
      p.min_or_max = 1;
      vp.push_back(p);
    }
  }
  return(vp.size());
}

//////////////////////////////////////////////////////////////////////
template<class T>
int peak(const Vector<T>& v
	 ,Vector<T>& localpeak
	 ,Vector<int>& localpeakindex
	 ,const T& threshratio=0.05,unsigned int range=2) {
  T vmin = Min(v);
  T vmax = Max(v);
  T thresh = (vmax-vmin)*threshratio + vmin;
  Vector<VectorPeak<T> > vp;
  PeakAroundN(v,vp,thresh,range);
  unsigned int i;
  localpeak.clear(); localpeakindex.clear();
  for(i=0;i<vp.size();i++) {
    localpeak.push_back(vp[i].val);
    localpeakindex.push_back(vp[i].index);
  }
  return(localpeak.size());
}

//////////////////////////////////////////////////////////////////////
template<class T>
int transition(const Vector<T>& v
	       ,Vector<int>& transitionindex
	       ,const T& thresh,int mode=0) {
  unsigned int i;
  transitionindex.clear();
  for(i=0;i<v.size();i++) {
    if(v[i]>thresh && v[i-1]<thresh && mode>=0) {
      transitionindex.push_back(i);
    }
    else if(v[i]<thresh && v[i-1]>thresh && mode<=0) {
      transitionindex.push_back(i);
    }
  }
  return(transitionindex.size());
}
#endif
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

#if 0
#ifndef SIMCUTILDLL
//////////////////////////////////////////////////////////////////////
double expX(double x,double p){
  unsigned int n=100;
  double s=1;
  double dt = x/n;
  for(unsigned int i=0;i<n;++i) {
    s -= pow(s,p)*dt;
  }
  return(s);
}
#endif
#endif

#if 0
//////////////////////////////////////////////////////////////////////
template<class T> 
Vector<T> expx(const Vector<T>& x,double a,double b){
  Vector<T> r;
  int n=x.size();
  double s=1;
  double dt = x[1]-x[0];
  for(unsigned int i=0;i<n;++i) {
    r.push_back(s);
    //s = s*(1-pow(s,a)*dt/b);
    s -= pow(s,a)*dt/b;
  }
  return(r);
}
#endif

//////////////////////////////////////////////////////////////////////
template<class T> 
Vector<T> expX(const Vector<T>& x,double p){
  Vector<T> r;
  unsigned int n=x.size();
  double s=1;
  double dt = x[1]-x[0];
  for(unsigned int i=0;i<n;++i) {
    r.push_back(s);
    //s = s*(1+pow(s,p)*dt);
    s -= pow(s,p)*dt;
  }
  return(r);
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//#define G__DISPALL

template<class T>
void Vector<T>::disp(ostream& os) const {
#ifdef DISPIOS
  unsigned int n = size();
  os << "(Vector " << this << ":" << n << ")" ;
  if(!n) { os << endl; return; }
  const T *dat = &(Vector<T>::front());
  unsigned int i;
#ifdef G__DISPALL
  for(i=0;i<n-1;i++) os << dat[i] << ",";
#else
  if(n>20) {
    for(i=0;i<10;i++) os << dat[i] << ",";
    os << ",,,";
    for(i=n-10;i<n-1;i++) os << dat[i] << ",";
  }
  else for(i=0;i<n-1;i++) os << dat[i] << ",";
#endif
  os << dat[n-1] << endl;
#endif
}

#ifdef DISPFILE
//////////////////////////////////////////////////////////////////////
template<>
inline void Vector<double>::disp(FILE* fp) const {
#if 1
  int n = size();
  fprintf(fp,"(Vector<double> 0x%p:%d)",this,n);
  if(!n) { fprintf(fp,"\n"); return; }
  const double *dat = &front();
  unsigned int i;
#ifdef G__DISPALL
  for(i=0;i<n-1;i++) fprintf(fp,"%g,",dat[i]);
#else
  if(n>20) {
    for(i=0;i<10;i++) fprintf(fp,"%g,",dat[i]);
    fprintf(fp, ",,,");
    for(i=n-10;i<n-1;i++) fprintf(fp,"%g,",dat[i]);
  }
  else for(i=0;i<n-1;i++) fprintf(fp,"%g,",dat[i]);
#endif
  fprintf(fp,"%g\n",dat[n-1]);
#endif
}

//////////////////////////////////////////////////////////////////////
template<>
inline void Vector<float>::disp(FILE* fp) const {
#if 1
  int n = size();
  fprintf(fp,"(Vector<float> 0x%p:%d)",this,n);
  if(!n) { fprintf(fp,"\n"); return; }
  const float *dat = &front();
  unsigned int i;
#ifdef G__DISPALL
  for(i=0;i<n-1;i++) fprintf(fp,"%f,",dat[i]);
#else
  if(n>20) {
    for(i=0;i<10;i++) fprintf(fp,"%f,",dat[i]);
    fprintf(fp, ",,,");
    for(i=n-10;i<n-1;i++) fprintf(fp,"%f,",dat[i]);
  }
  else for(i=0;i<n-1;i++) fprintf(fp,"%f,",dat[i]);
#endif
  fprintf(fp,"%f\n",dat[n-1]);
#endif
}

//////////////////////////////////////////////////////////////////////
template<>
inline void Vector<int>::disp(FILE* fp) const {
#if 1
  int n = size();
  fprintf(fp,"(Vector<int> 0x%p:%d)",this,n);
  if(!n) { fprintf(fp,"\n"); return; }
  const int *dat = &front();
  unsigned int i;
#ifdef G__DISPALL
  for(i=0;i<n-1;i++) fprintf(fp,"%d,",dat[i]);
#else
  if(n>20) {
    for(i=0;i<10;i++) fprintf(fp,"%d,",dat[i]);
    fprintf(fp, ",,,");
    for(i=n-10;i<n-1;i++) fprintf(fp,"%d,",dat[i]);
  }
  else for(i=0;i<n-1;i++) fprintf(fp,"%d,",dat[i]);
#endif
  fprintf(fp,"%d\n",dat[n-1]);
#endif
}

//////////////////////////////////////////////////////////////////////
template<>
inline void Vector<Vector<double> >::disp(FILE* fp) const {
#if 1
  int n = size();
  fprintf(fp,"(Vector<Vector<double> > 0x%p:%d)",this,n);
  if(!n) { fprintf(fp,"\n"); return; }
  const Vector<double> *dat = &front();
  unsigned int i;
#ifdef G__DISPALL
  for(i=0;i<n-1;i++) dat[i].disp(fp);
#else
  if(n>20) {
    for(i=0;i<10;i++) dat[i].disp(fp);
    fprintf(fp, ",,,");
    for(i=n-10;i<n-1;i++) dat[i].disp(fp);
  }
  else for(i=0;i<n-1;i++) dat[i].disp(fp);
#endif
  dat[n-1].disp(fp);
#endif
}

//////////////////////////////////////////////////////////////////////
template<>
inline void Vector<Vector<float> >::disp(FILE* fp) const {
#if 1
  int n = size();
  fprintf(fp,"(Vector<Vector<float> > 0x%p:%d)",this,n);
  if(!n) { fprintf(fp,"\n"); return; }
  const Vector<float> *dat = &front();
  unsigned int i;
#ifdef G__DISPALL
  for(i=0;i<n-1;i++) dat[i].disp(fp);
#else
  if(n>20) {
    for(i=0;i<10;i++) dat[i].disp(fp);
    fprintf(fp, ",,,");
    for(i=n-10;i<n-1;i++) dat[i].disp(fp);
  }
  else for(i=0;i<n-1;i++) dat[i].disp(fp);
#endif
  dat[n-1].disp(fp);
#endif
}

//////////////////////////////////////////////////////////////////////
template<>
inline void Vector<Vector<int> >::disp(FILE* fp) const {
#if 1
  int n = size();
  fprintf(fp,"(Vector<Vector<int> > 0x%p:%d)",this,n);
  if(!n) { fprintf(fp,"\n"); return; }
  const Vector<int> *dat = &front();
  unsigned int i;
#ifdef G__DISPALL
  for(i=0;i<n-1;i++) dat[i].disp(fp);
#else
  if(n>20) {
    for(i=0;i<10;i++) dat[i].disp(fp);
    fprintf(fp, ",,,");
    for(i=n-10;i<n-1;i++) dat[i].disp(fp);
  }
  else for(i=0;i<n-1;i++) dat[i].disp(fp);
#endif
  dat[n-1].disp(fp);
#endif
}

#ifndef NOCOMPLEX
//////////////////////////////////////////////////////////////////////
template<>
inline void Vector<Complex<double> >::disp(FILE* fp) const {
#if 1
  int n = size();
  fprintf(fp,"(Vector<Complex<double> > 0x%p:%d)",this,n);
  if(!n) { fprintf(fp,"\n"); return; }
  const Complex<double> *dat = &front();
  unsigned int i;
#ifdef G__DISPALL
  for(i=0;i<n-1;i++) fprintf(fp,"(%g,%g),",dat[i].real(),dat[i].imag());
#else
  if(n>20) {
    for(i=0;i<10;i++) fprintf(fp,"(%g,%g),",dat[i].real(),dat[i].imag());
    fprintf(fp, ",,,");
    for(i=n-10;i<n-1;i++) fprintf(fp,"(%g,%g),",dat[i].real(),dat[i].imag());
  }
  else for(i=0;i<n-1;i++) fprintf(fp,"(%g,%g),",dat[i].real(),dat[i].imag());
#endif
  fprintf(fp,"(%g,%g)\n",dat[n-1].real(),dat[n-1].imag());
#endif
}
//////////////////////////////////////////////////////////////////////
template<>
inline void Vector<Complex<float> >::disp(FILE* fp) const {
#if 1
  int n = size();
  fprintf(fp,"(Vector<Complex<float> > 0x%p:%d)",this,n);
  if(!n) { fprintf(fp,"\n"); return; }
  const Complex<float> *dat = &front();
  unsigned int i;
#ifdef G__DISPALL
  for(i=0;i<n-1;i++) fprintf(fp,"(%f,%f),",dat[i].real(),dat[i].imag());
#else
  if(n>20) {
    for(i=0;i<10;i++) fprintf(fp,"(%f,%f),",dat[i].real(),dat[i].imag());
    fprintf(fp, ",,,");
    for(i=n-10;i<n-1;i++) fprintf(fp,"(%f,%f),",dat[i].real(),dat[i].imag());
  }
  else for(i=0;i<n-1;i++) fprintf(fp,"(%f,%f),",dat[i].real(),dat[i].imag());
#endif
  fprintf(fp,"(%f,%f)\n",dat[n-1].real(),dat[n-1].imag());
#endif
}
#endif // NOCOMPLEX
#endif // DISPFILE

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
#define VECTOR_HASH_INVALID  1e-99

inline double hashseed(unsigned int i) {
  const double g_hashseed[] = { 
    1.0, 1.102135, 1.213415, 1.3751257, 1.431857, 1.58173134, 1.6137589, 
    1.7913847, 1.838343, 1.938474 };
  const double g_hashseed2 = 2.10234;
  return(g_hashseed[i%10]*g_hashseed2*(i/10));
}

//////////////////////////////////////////////////////////////////////
template<class T>
double hash_scalar(const Vector<T>& a) {
  double h=0;
  for(unsigned int i=0;i<a.size();i++) h += a[i]*hashseed(i);
  return(h);
}

inline double Hash(const Vector<double>& a) { return(hash_scalar(a)); }
inline double Hash(const Vector<float>& a) { return(hash_scalar(a)); }
inline double Hash(const Vector<int>& a) { return(hash_scalar(a)); }

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
#ifndef NOCOMPLEX
template<class T>
double hash_complex(const Vector<Complex<T> >& a) {
  double h=0;
  for(unsigned int i=0;i<a.size();i++) 
    h = h + a[i].real()*hashseed(i*2) + a[i].imag()*hashseed(i*2+1);
  return(h);
}

inline double Hash(const Vector<Complex<double> >& a){return(hash_complex(a));}
inline double Hash(const Vector<Complex<float> >& a){return(hash_complex(a));}
#endif

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
template<class T>
ostream& operator<<(ostream& os,const Vector<T>& a) {
  for(unsigned int i=0;i<a.size();++i) os << a[i] << " ";
  return(os);
}

//////////////////////////////////////////////////////////////////////
template<class T>
int G__ateval(const Vector<T>& x) {
  x.disp();
  return(1); 
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
typedef Vector<double> DVector;
typedef Vector<float> FVector;
typedef Vector<int> IVector;

typedef Vector<DVector> DMatrix;
typedef Vector<FVector> FMatrix;
typedef Vector<IVector> IMatrix;

#ifdef G__COMPLEX
typedef Complex<double> DComplex;
typedef Vector<DComplex> DCVector;

typedef Complex<float> FComplex;
typedef Vector<FComplex> FCVector;

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
inline void DVector2DCVector(const DVector& re,const DVector& im,DCVector& y) {
  y.resize(re.size());
  DComplex c;
  for(unsigned int i=0;i<re.size();i++) {
    c=DComplex(re[i],im[i]);
    y[i] = c;
  }
}

//////////////////////////////////////////////////////////////////////
inline void DVector2DCVector(const DVector& re,DCVector& y) {
  y.resize(re.size());
  DComplex c;
  for(unsigned int i=0;i<re.size();i++) {
    c=DComplex(re[i],0);
    y[i] = c;
  }
}

//////////////////////////////////////////////////////////////////////
inline void DCVector2DVector(const DCVector& in,DVector& re,DVector& im){
  re.resize(in.size());
  im.resize(in.size());
  for(unsigned int i=0;i<re.size();i++) {
    re[i] = in[i].real();
    im[i] = in[i].imag();
  }
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
#endif // G__COMPLEX

#endif // G__NVECTOR
