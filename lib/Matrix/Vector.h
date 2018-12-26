/**************************************************************************
* Vector.h
*
*
**************************************************************************/

#ifndef G__VECTOR
#define G__VECTOR

#include <vector>
#include <algorithm>
#include <iostream>
#include <cmath>
using namespace std;

#ifdef __STAT__
//#include <statistics.h>
#endif


//////////////////////////////////////////////////////////////////////
template<class T> class Vector : public vector<T> {
 public:
  Vector() {}
  Vector(const Vector<T>& a) {
    unsigned int n=a.size();
    for(unsigned int i=0;i<n;++i) Vector<T>::push_back(a[i]);
  }
  Vector(int n,const T* a) {
    for(int i=0;i<n;++i) Vector<T>::push_back(a[i]);
  }
  Vector(const T& start,const T& stop,int n) {
    T v = start;
    T d = (stop-start)/(double)n;
    for(int i=0;i<n;++i) Vector<T>::push_back(start+d*(double)n);
  }
  Vector(const T& a) {
    unsigned int n=Vector<T>::size();
    for(unsigned int i=0;i<n;++i) Vector<T>::push_back(a); 
  }
  Vector<T>& operator=(const Vector<T>& a) {
    unsigned int n=a.size();
    if(n==Vector<T>::size()) {
      T *d = (&Vector<T>::front());
      for(unsigned int i=0;i<n;++i) d[i] = a[i];
    }
    else {
      Vector<T>::clear();
      for(unsigned int i=0;i<n;i++) Vector<T>::push_back(a[i]);
    }
    return(*this);
  }
  Vector<T>& operator=(double a) {
    unsigned int n=Vector<T>::size();
    if(!n) return(*this);
    T *d = (&Vector<T>::front());
    for(unsigned int i=0;i<n;++i) d[i] = a;
    return(*this);
  }

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
    if(n>Vector<T>::size()) n=Vector<T>::size();
    if(!n) return(*this);
    T *dat = &Vector<T>::front();
    const T *data = &a.Vector<T>::front();
    for(unsigned int i=0;i<n;++i) dat[i] = dat[i] +  data[i];
    return(*this);
  }
  Vector<T>& operator-=(const Vector<T>& a) {
    unsigned int n=a.size();
    if(n>Vector<T>::size()) n=Vector<T>::size();
    if(!n) return(*this);
    T *dat = &Vector<T>::front();
    for(unsigned int i=0;i<n;++i) dat[i] = dat[i] -  a[i];
    return(*this);
  }
  Vector<T>& operator*=(const Vector<T>& a) {
    unsigned int n=a.size();
    if(n>Vector<T>::size()) n=Vector<T>::size();
    if(!n) return(*this);
    T *dat = &Vector<T>::front();
    for(unsigned int i=0;i<n;++i) dat[i] = dat[i] * a[i];
    return(*this);
  }
  Vector<T>& operator/=(const Vector<T>& a) {
    unsigned int n=a.size();
    if(n>Vector<T>::size()) n=Vector<T>::size();
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

  Vector<T>& i() {
    unsigned int n=Vector<T>::size();
    if(!n) return(*this);
    T *dat = &Vector<T>::front();
    for(unsigned int i=0;i<n;++i) dat[i] = i;
    return(*this);
  }

  T last() { 
    unsigned int n=Vector<T>::size();
    if(!n) return(0);
    T *dat = &Vector<T>::front();
    return(dat[n-1]);
  }

  void print(int start=0,int stop=0) {
    unsigned int n=Vector<T>::size();
    if(!n) return;
    if(stop && n>(unsigned int)stop) n=stop;
    T *dat = &Vector<T>::front();
    for(unsigned int i=start;i<n;++i) printf("%d:%g ",i,dat[i]);
    printf("\n");
  }

  void disp(ostream& os) const ;
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
  int mismatch=0;
  for(unsigned int i=0;i<n;++i) if(a[i]!=b[i]) ++mismatch;
  return(mismatch?false:true);
}
//////////////////////////////////////////////////////////////////////
template<class T> 
bool operator!=(const Vector<T>& a,const Vector<T>& b){
  unsigned int n=a.size();
  int mismatch=0;
  for(unsigned int i=0;i<n;++i) if(a[i]!=b[i]) ++mismatch;
  return(mismatch?true:false);
}
//////////////////////////////////////////////////////////////////////
template<class T> 
bool operator>(const Vector<T>& a,const Vector<T>& b){
  unsigned int n=a.size();
  double diff=0;
  for(unsigned int i=0;i<n;++i) diff += a[i]-b[i];
  return(diff>0?true:false);
}
//////////////////////////////////////////////////////////////////////
template<class T> 
bool operator <(const Vector<T>& a,const Vector<T>& b){
  unsigned int n=a.size();
  double diff=0;
  for(unsigned int i=0;i<n;++i) diff += a[i]-b[i];
  return(diff<0?true:false);
}
//////////////////////////////////////////////////////////////////////
template<class T> 
bool operator>=(const Vector<T>& a,const Vector<T>& b){
  unsigned int n=a.size();
  double diff=0;
  for(unsigned int i=0;i<n;++i) diff += a[i]-b[i];
  return(diff>=0?true:false);
}
//////////////////////////////////////////////////////////////////////
template<class T> 
bool operator <=(const Vector<T>& a,const Vector<T>& b){
  unsigned int n=a.size();
  double diff=0;
  for(unsigned int i=0;i<n;++i) diff += a[i]-b[i];
  return(diff<=0?true:false);
}
//////////////////////////////////////////////////////////////////////

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
//////////////////////////////////////////////////////////////////////
template<class T> 
Vector<T> pow(const Vector<T>& a,double b){
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
Vector<T> pow(const Vector<T>& a,const Vector<T>& b){
  Vector<T> r;
  unsigned int n=a.size();
  T tmp;
  for(unsigned int i=0;i<n;++i) {
    tmp = pow(a[i],b[i]);
    r.push_back(tmp);
  }
  return(r);
}
//////////////////////////////////////////////////////////////////////
template<class T> 
Vector<T> operator+(double a,const Vector<T>& b){
  Vector<T> r;
  int n=b.size();
  for(unsigned int i=0;i<n;++i) r.push_back(a+b[i]);
  return(r);
}
//////////////////////////////////////////////////////////////////////
template<class T> 
Vector<T> operator-(double a,const Vector<T>& b){
  Vector<T> r;
  int n=b.size();
  for(unsigned int i=0;i<n;++i) r.push_back(a-b[i]);
  return(r);
}
//////////////////////////////////////////////////////////////////////
template<class T> 
Vector<T> operator*(double a,const Vector<T>& b){
  Vector<T> r;
  int n=b.size();
  for(unsigned int i=0;i<n;++i) r.push_back(a*b[i]);
  return(r);
}
//////////////////////////////////////////////////////////////////////
template<class T> 
Vector<T> operator/(double a,const Vector<T>& b){
  Vector<T> r;
  int n=b.size();
  for(unsigned int i=0;i<n;++i) r.push_back(a/b[i]);
  return(r);
}

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
Vector<T>& operator/(const Vector<T>& a,int b){
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
  for(unsigned int i=0;i<n;++i) {
    tmp = sqrt(a[i]);
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
  double s=0;
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
  double tmp;
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
static void G__ary_rand(T *c,const T *a,int n) {
  int i;
  for(i=0;i<n;i++) {
    c[i] = rand();
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
static void G__ary_pulse(T *c,const T *a,int n) {
  unsigned int i=0;
  double tmp;
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
static void G__ary_conv(T* c,const T* a,int n,const T* b,int m)
{
  int i,j,k;
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
  double s=0;
  r.push_back(s);
  for(unsigned int i=1;i<n;++i) {
    s += (a[i]+a[i-1])*(x[i]-x[i-1])/2;
    r.push_back(s);
  }
  return(r);
}

//////////////////////////////////////////////////////////////////////
template<class T> 
Vector<T> diff(const Vector<T>& x,const Vector<T>& a){
  Vector<T> r;
  unsigned int n=a.size();
  double s=0;
  r.push_back(s);
  for(unsigned int i=1;i<n;++i) {
    s = (a[i]-a[i-1])/(x[i]-x[i-1]);
    r.push_back(s);
  }
  return(r);
}

//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
template<class T> 
Vector<T> movingave(const Vector<T>& a,int average,int begin_end=0){
  Vector<T> r;
  unsigned int n=a.size();
  double s=0;
  double val;
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
  double sum=0;
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
  int n=in.size();
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
  //        |          |  norm = ƒ°k^i | i=0..infinity
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
Vector<T> min(const Vector<T>& x,double y) {
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
Vector<T> max(const Vector<T>& x,double y) {
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
double mean(const Vector<T>& v) {
  double s = sum(v);
  return(s/v.size());
}

//////////////////////////////////////////////////////////////////////
template<class T>
double min(const Vector<T>& v) {
  double m=1e99;
  for(unsigned int i=0;i<v.size();i++) if(m>v[i]) m = v[i];
  return(m);
}

//////////////////////////////////////////////////////////////////////
template<class T>
double max(const Vector<T>& v) {
  double m=-1e99;
  for(unsigned int i=0;i<v.size();i++) if(m<v[i]) m = v[i];
  return(m);
}

//////////////////////////////////////////////////////////////////////
template<class T>
double stddev(const Vector<T>& v) {
  return(rms(v));
}

//////////////////////////////////////////////////////////////////////
template<class T>
double rms(const Vector<T>& v) {
  double vmean = mean(v);
  double sum=0;
  double tmp;
  for(unsigned i=0;i<v.size();i++) {
    tmp = v[i]-vmean;
    sum += tmp*tmp;
  }
  return(sqrt(sum/v.size()));
}

//////////////////////////////////////////////////////////////////////
template<class T>
double pp(const Vector<T>& v) {
  double min=1e99,max=-1e99;
  for(unsigned i=0;i<v.size();i++) {
    if(min>v[i]) min = v[i];
    if(v[i]>max) max = v[i];
  }
  return(max-min);
}

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
  int n=x.size();
  for(unsigned int i=0;i<n;i++) if(x[i]>=val) return(i);
  return(-1);
}
//////////////////////////////////////////////////////////////////////
template<class T>
Vector<T> range(const Vector<T>& x,int i1,int i2) {
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
Vector<T> linear(T start,T stop,int n) {
  Vector<T> r;
  double step=(stop-start)/(n-1);
  for(unsigned int i=0;i<n;i++) r.push_back(start+step*i);
  return(r);
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

#ifdef __STAT__
//////////////////////////////////////////////////////////////////////
statistics stat(const Vector<double>& x) {
  statistics stat;
  int n=x.size();
  for(unsigned int i=0;i<n;i++) stat.add(x[i]);
  return(stat);
}
//////////////////////////////////////////////////////////////////////
double mean(const Vector<double>& x){
  statistics stat;
  int n=x.size();
  for(unsigned int i=0;i<n;i++) stat.add(x[i]);
  return(stat.mean());
}
//////////////////////////////////////////////////////////////////////
double stddev(const Vector<double>& x){
  statistics stat;
  int n=x.size();
  for(unsigned int i=0;i<n;i++) stat.add(x[i]);
  return(stat.stddev());
}
//////////////////////////////////////////////////////////////////////
double min(const Vector<double>& x){
  statistics stat;
  int n=x.size();
  for(unsigned int i=0;i<n;i++) stat.add(x[i]);
  return(stat.min());
}

//////////////////////////////////////////////////////////////////////
double max(const Vector<double>& x){
  statistics stat;
  int n=x.size();
  for(unsigned int i=0;i<n;i++) stat.add(x[i]);
  return(stat.max());
}
//////////////////////////////////////////////////////////////////////
statistics stat(const Vector<double>& x,const Vector<double>& y
                ,double xstart,double xend) {
  statistics stat;
  int n=x.size();
  for(unsigned int i=0;i<n;i++) {
    if(xstart<=x[i] && x[i]<xend) stat.add(y[i]);
  }
  return(stat);
}
//////////////////////////////////////////////////////////////////////
#endif


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
template<class T>
void Vector<T>::disp(ostream& os) const {
#if 0
  int n = size();
  const T *dat = &operator[](0);
  os << "(Vector " << this << ":" << n << ")" ;
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

template<class T>
int G__ateval(const Vector<T>& x) {
  x.disp();
  return(1); 
}

#endif // G__VECTOR
