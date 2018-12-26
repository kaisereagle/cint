/*************************************************************************
* Stat.h
*
*  Standard deviation calculation library
*
*************************************************************************/
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
#ifndef STATISTICS_H
#define STATISTICS_H

#include <iostream>
#include <cstdio>
#include <cmath>
#include <vector>
using namespace std;

#define TEMPLATE_STAT
#define PRECISION_STAT

////////////////////////////////////////////////////////////////////////
template<class T>
inline T gauss(T x,T mean,T stddev) {
  double Xmean = x-mean;
  if(stddev==0) {
    if(Xmean==0) return(1);
    else         return(0);
  }
  return(1/sqrt(6.28)/stddev*exp(-Xmean*Xmean/2/stddev/stddev));
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
// statistics class
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////
// Big data statistics 1
//  1 N
//  2 Di = Mx + Axi = My + Ayi
//  3 sigmaN  = sumN(Di)   = N*Mx   + sunN(Axi)
//                         = N*My   + sumN(Ayi)
//  4 sigma2N = sumN(Di^2) = sumN( Mx^2 + 2*Mx*Axi   + Axi^2 )
//                         = N*Mx^2 + 2*Mx*sumN(Axi) + sumN(Axi^2)
//                         = N*My^2 + 2*My*sumN(Ayi) + sumN(Ayi^2)
//
//  5  initialize N=0, Mx=0, sumN(Axi)=0,  sumN(Axi^2)=0
//  For each data
//  6  axi = Di - Mx
//  7  sumN(Axi) += Axi    
//  8  sumN(Axi^2) += Axi^2
//
// if(N>thresh && sumN(Axi)/N>Mx/100) given N, Mx, sumN(Axi) and sumN(Axi^2),
//  9  My = Mx + sumN(Axi)/N
// Then from 3,
//  10 sumN(Ayi)   = N*Mx + sumN(Axi) - N*My = (Mx-My)*N + sumN(Axi)
// From 4, 5 and 6,
//  11 sumN(Ayi^2) =   N*Mx^2 + 2*Mx*sumN(Axi) + sumN(Axi^2)
//                   - N*My^2 - 2*My*sumN(Ayi)
//                 = N*(Mx^2-My^2) + 2(Mx*sumN(axi)-My*sumN(Ayi)) + sumN(Axi^2)
//
// Then, continue
//  6  ayi = Di - My
//  7  sumN(Ayi) += Ayi         << average is close to zero. no overflow
//  8  sumN(Ayi^2) += Ayi^2     <<< this can still overflow
// 
// Finally, 
//  11 nsample() = N
//  12 sigma()   = My*N + sumN(ayi)
//  13 mean()    = My + sumN(ayi)/N
//  14 stddev()  = sqrt((sumN(Ayi^2) - sumN(ayi)^2/N)/(N-1))
//  15 sigma2()  = N*My^2 + 2*My*SumN(ayi) + sumN(Ayi^2)
//
/////////////////////////////////////////////////////////////////////////
// Big data statistics 2
//  21  N       =  K      +  L
// if K and L are very large, we can approximate statistics of consolidated
// data as follows.
//  22  k = K/factor,  l = L/factor
//  23  meanN   = (meanK*k + meanL*l)/(k+l)
//  24  stddevN = sqrt((stddevk^2*k + stddevl^2*l)/(k+l))
//
/////////////////////////////////////////////////////////////////////////

template<class T>
class Stat {
 private:
  T m_min,m_max;
#ifdef PRECISION_STAT
  double m_sigma,m_sigma2,m_smean;
  static int s_Nthresh, s_Mratio;
#else
  T m_sigma,m_sigma2;
#endif
  int m_nsample;
  int m_error;
public:
  Stat() { init(); }
  Stat(const Stat& a) { 
    m_min=a.m_min; 
    m_max=a.m_max; 
    m_sigma=a.m_sigma;
    m_sigma2=a.m_sigma2;
    m_error=a.m_error;
    m_nsample = a.m_nsample;
#ifdef PRECISION_STAT
    m_smean=a.m_smean;
#endif
  }
  void init() {
    m_min = 1e37;
    m_max = -1e37;
    m_sigma = 0.0;
    m_sigma2 = 0.0;
    m_error = 0;
    m_nsample = 0;
#ifdef PRECISION_STAT
    m_smean = 0.0;
#endif
  }
  void clear() { init(); }
  int fill(T data,T lowlimit=-1e37, T uplimit=1e37) 
   {return(add(data,lowlimit,uplimit));}
  int fill(const vector<T>& data,T lowlimit=-1e37,T uplimit=1e37) 
   {return(add(data,lowlimit,uplimit));}
  int add(T data,T lowlimit=-1e37, T uplimit=1e37) {
    double ddata = data;
#ifdef PRECISION_STAT
    if(m_nsample>s_Nthresh && m_sigma>m_smean/s_Mratio*m_nsample) {
      double smeanx=m_smean, sigmax=m_sigma;
      m_smean = smeanx + m_sigma/m_nsample;           // 9
      m_sigma = (smeanx-m_smean)*m_nsample + m_sigma; // 10 ASSERT(0.0)
      m_sigma2 = (smeanx*smeanx-m_smean*m_smean)*m_nsample  // 11
	         +2*(smeanx*sigmax-m_smean*m_sigma) 
	         + m_sigma2;
    }
    ddata -= m_smean;
#endif
    m_sigma += ddata;
    m_sigma2 += (ddata*ddata);
    m_nsample+=1 ;
    
    if(data>m_max) m_max=data;
    if(data<m_min) m_min=data;
    
    if(data < lowlimit || uplimit < data) {
      /* fprintf(stderr,"%g\n",data); */
      m_error+=1;
      return(m_error);
    }
    return(0);
  }
  int add(const vector<T>& data,T lowlimit=-1e37, T uplimit=1e37) {
    int stat=0;
    for(unsigned int i=0;i<data.size();i++) stat+=add(data[i],lowlimit,uplimit);
    return(stat);
  }
  int fillN(T data,int N,T lowlimit=-1e37, T uplimit=1e37) 
    {return(addN(data,N,lowlimit,uplimit));}
  int addN(T data,int N,T lowlimit=-1e37, T uplimit=1e37) {
#if 0
    for(int i=0;i<N;i++) add(data,lowlimit,uplimit);
#else
    double ddata = data;
#ifdef PRECISION_STAT
    if(m_nsample>s_Nthresh && m_sigma>m_smean/s_Mratio*m_nsample) {
      double smeanx=m_smean, sigmax=m_sigma;
      m_smean = smeanx + m_sigma/m_nsample;           // 9
      m_sigma = (smeanx-m_smean)*m_nsample + m_sigma; // 10 ASSERT(0.0)
      m_sigma2 = (smeanx*smeanx-m_smean*m_smean)*m_nsample  // 11
	         +2*(smeanx*sigmax-m_smean*m_sigma) 
	         + m_sigma2;
    }
    ddata -= m_smean;
#endif
    m_sigma += ddata*N;
    m_sigma2 += (ddata*ddata)*N;
    m_nsample += N;
    
    if(data>m_max) m_max=data;
    if(data<m_min) m_min=data;
    
    if(data < lowlimit || uplimit < data) {
      /* fprintf(stderr,"%g\n",data); */
      m_error+=1;
      return(m_error);
    }
#endif
    return(0);
  }
  int add(const Stat& st) {
    m_sigma += st.m_sigma;
    m_sigma2 += st.m_sigma2;
    m_nsample += st.m_nsample;
    
    if(st.m_max>m_max) m_max=st.m_max;
    if(st.m_min<m_min) m_min=st.m_min;
    
    return(0);
  }
  int del(T data,T lowlimit=-1e37, T uplimit=1e37) {
    m_sigma -= data;
    m_sigma2 -= (data*data);
    m_nsample-=1 ;
    return(0);
  }
  Stat& operator<<(T d) { add(d); return(*this); }
  T min() const { return(m_nsample?m_min:T()); }
  T max() const { return(m_nsample?m_max:T()); }
#ifdef PRECISION_STAT
  double sigma() const { return(m_sigma + m_nsample*m_smean); } // 12
  double sigma2() const { return(m_nsample*m_smean*m_smean +
				 2*m_smean*m_sigma + m_sigma2);} // 15
  double stddev() const {
    double result;
    if(m_nsample<=1) result=0.0;
    else result=sqrt((m_sigma2 - m_sigma*m_sigma/m_nsample)/(m_nsample-1));
    return(result);
  }
  T mean() const { 
    if(m_nsample!=0) return(m_sigma/m_nsample + m_smean); 
    else             return(0);
  }
#else
  T sigma() const { return(m_sigma); }
  T sigma2() const { return(m_sigma2); }
  T stddev() const {
    T result;
    if(1>=m_nsample) 
      result=0.0;
    else
      result=sqrt((m_sigma2 - m_sigma*m_sigma/m_nsample)/(m_nsample-1));
    return(result);
  }
  T mean() const { 
    if(m_nsample!=0) return(m_sigma/m_nsample); 
    else             return(0);
  }
#endif
  int nsample() const { return(m_nsample); }
  int error() const { return(m_error); }

  void disp(FILE* fp=stdout) const {
    fprintf(fp
	    ,"sigma=%g min=%g max=%g stddev=%g mean=%g nsample=%d err=%d sigma2=%g\n"
	    ,sigma(),min(),max(),stddev(),mean(),nsample(),error(),m_sigma2);
  }

#if 0
  T sigma2() const { return(m_sigma2); }
  void set(T minin,T maxin,T sigmain,T sigma2in
	   ,int nsamplein,int errorin) {
    m_min = minin; m_max = maxin;
    m_sigma = sigmain; m_sigma2 = sigma2in;
    m_nsample = nsamplein; m_error = errorin;
  }
#endif
  void set(T minin,T maxin,T meanin,T stddevin
	   ,int nsamplein,int errorin) {
    m_min = minin; m_max = maxin;
    m_nsample = nsamplein; m_error = errorin;
    m_sigma = meanin*nsamplein; 
    //stddev=sqrt((m_sigma2 - m_sigma*m_sigma/m_nsample)/(m_nsample-1));
    //stddev^2=(m_sigma2 - m_sigma*m_sigma/m_nsample)/(m_nsample-1)
    //stddev^2*(nsample-1) = sigma2 - sigma*sigma/nsample;
    //stddev^2*(nsample-1) + sigma*sigma/nsample= sigma2 ;
    m_sigma2 =  stddevin*stddevin*(m_nsample-1) + m_sigma*m_sigma/m_nsample;
  }
};

template<class T> int Stat<T>::s_Nthresh=100000;
template<class T> int Stat<T>::s_Mratio=256;

typedef Stat<double> statistics;
typedef Stat<double> DStat;
typedef Stat<float > FStat;
typedef Stat<int>  IStat;
typedef Stat<long> LStat;

////////////////////////////////////////////////////////////////////////
template<class T>
inline int G__ateval(const Stat<T>& stat) {
  stat.disp();
  return(0);
}
//extern int G__ateval(const DStat& stat);
//extern int G__ateval(const FStat& stat);

////////////////////////////////////////////////////////////////////////
template<class T>
inline ostream& operator<<(ostream& ios,Stat<T>& stat) {
  ios << "sigma=" << stat.sigma() ;
  ios << "  min=" << stat.min() << "  max=" << stat.max() ;
  ios << "  stddev=" << stat.stddev() << "  mean=" << stat.mean() ;
  ios << "  nsample=" << stat.nsample() << "  error=" << stat.error() ;
  ios << "  sigma2=" << stat.sigma2() ;
  ios << "\n";
  return(ios);
}
//extern ostream& operator<<(ostream& ios,DStat& stat);
//extern ostream& operator<<(ostream& ios,FStat& stat);
////////////////////////////////////////////////////////////////////////
template<class T>
inline bool operator==(const Stat<T>& a,const Stat<T>& b) {
  return(a.nsample()==b.nsample()); 
}
template<class T>
inline bool operator!=(const Stat<T>& a,const Stat<T>& b) { 
  return(a.nsample()!=b.nsample()); 
}
template<class T>
inline bool operator<(const Stat<T>& a,const Stat<T>& b) { 
  return(a.nsample()<b.nsample()); 
}

////////////////////////////////////////////////////////////////////////
template<class T> bool CompMean(const Stat<T>& a,const Stat<T>& b) 
{return(a.mean()<b.mean());}
template<class T> bool CompStddev(const Stat<T>& a,const Stat<T>& b) 
{return(a.stddev()<b.stddev());}
template<class T> bool CompMin(const Stat<T>& a,const Stat<T>& b) 
{return(a.min()<b.min());}
template<class T> bool CompMax(const Stat<T>& a,const Stat<T>& b) 
{return(a.max()<b.max());}

//extern bool operator==(const DStat& a,const DStat& b);
//extern bool operator!=(const DStat& a,const DStat& b);
//extern bool operator<(const DStat& a,const DStat& b);

//extern bool operator==(const FStat& a,const FStat& b);
//extern bool operator!=(const FStat& a,const FStat& b);
//extern bool operator<(const FStat& a,const FStat& b);


#ifndef NO_STATOBJ
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
// StatObj
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
// Requirements to data type T
//  T operator* (const T& a,const T& b);
//  T operator+ (const T& a,const T& b);
//  T operator- (const T& a,const T& b);
//  T operator/ (const T& a,int b);
//  T& T::operator=(const T& a);
//  T T::operator+=(const T& a);
//  T T::operator-=(const T& a); // optional
//  bool operator<(const T& a,const T& b);
//  bool operator==(const T& a,const T& b);
//  bool operator!=(const T& a,const T& b);
////////////////////////////////////////////////////////////////////////
//template<class T>
typedef double T;
class StatObj {
 private:
  T m_sigma,m_sigma2;
  int m_nsample;
public:
  StatObj() { init(); }
  StatObj(const StatObj& a) { 
    m_sigma=a.m_sigma;
    m_sigma2=a.m_sigma2;
    m_nsample = a.m_nsample;
  }
  void init() {
    m_sigma = 0.0;
    m_sigma2 = 0.0;
    m_nsample = 0;
  }
  void clear() { init(); }
  int fill(T data) {return(add(data));}
  int fill(const vector<T>& data) {return(add(data));}
  int add(T data) {
    m_sigma2 += (data*data);
    m_nsample+=1 ;
    return(0);
  }
  int add(const vector<T>& data) {
    int stat=0;
    for(unsigned int i=0;i<data.size();i++) stat+=add(data[i]);
    return(stat);
  }
  int fillN(T data,int N) {return(addN(data,N));}
  int addN(T data,int N) {
#if 0
    for(int i=0;i<N;i++) add(data,lowlimit,uplimit);
#else
    m_sigma2 += (data*data)*N;
    m_nsample += N;
#endif
    return(0);
  }
  int add(const StatObj& st) {
    m_sigma += st.m_sigma;
    m_sigma2 += st.m_sigma2;
    m_nsample += st.m_nsample;
    return(0);
  }
  int del(T data) {
    m_sigma -= data;
    m_sigma2 -= (data*data);
    m_nsample-=1 ;
    return(0);
  }
  StatObj& operator<<(T d) { add(d); return(*this); }
  T sigma() const { return(m_sigma); }
  T sigma2() const { return(m_sigma2); }
  T stddev() const {
    T result;
    if(1>=m_nsample) 
      result=0.0;
    else
      result=sqrt((m_sigma2 - m_sigma*m_sigma/m_nsample)/(m_nsample-1));
    return(result);
  }
  T mean() const { 
    if(m_nsample!=0) return(m_sigma/m_nsample); 
    else             return(0);
  }
  int nsample() const { return(m_nsample); }

  void disp(FILE* fp=stdout) const {
  }

  void set(const T& meanin,const T& stddevin ,int nsamplein) {
    m_nsample = nsamplein; 
    m_sigma = meanin*nsamplein; 
    //stddev=sqrt((m_sigma2 - m_sigma*m_sigma/m_nsample)/(m_nsample-1));
    //stddev^2=(m_sigma2 - m_sigma*m_sigma/m_nsample)/(m_nsample-1)
    //stddev^2*(nsample-1) = sigma2 - sigma*sigma/nsample;
    //stddev^2*(nsample-1) + sigma*sigma/nsample= sigma2 ;
    m_sigma2 =  stddevin*stddevin*(m_nsample-1) + m_sigma*m_sigma/m_nsample;
  }
};

#endif // NO_STATOBJ

#endif // STATISTICS_H

