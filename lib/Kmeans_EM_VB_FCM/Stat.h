/*************************************************************************
* Stat.h
*
*  Standard deviation calculation library
*
*************************************************************************/
#ifndef STATISTICS_H
#define STATISTICS_H

#include <iostream>
#include <cstdio>
using namespace std;

class statistics {
public:
  statistics() { init(); }
  void init();
  void clear() { init(); }
  int fill(double data,double lowlimit=-1e99, double uplimit=1e99) 
   {return(add(data,lowlimit,uplimit));}
  int add(double data,double lowlimit=-1e99, double uplimit=1e99);
  int del(double data,double lowlimit=-1e99, double uplimit=1e99);
  statistics& operator<<(double d) { add(d); return(*this); }
  double min() const { return(m_min); }
  double max() const { return(m_max); }
  double sigma() const { return(m_sigma); }
  double stddev() const ;
  double mean() const { 
    if(m_nsample!=0) return(m_sigma/m_nsample); 
    else             return(0);
  }
  int nsample() const { return(m_nsample); }
  int error() const { return(m_error); }

  void disp(FILE* fp=stdout) const ;
 private:
  double m_min,m_max;
  double m_sigma,m_sigma2;
  int m_nsample;
  int m_error;
};
////////////////////////////////////////////////////////////////////////
inline int G__ateval(const statistics& stat) {
  stat.disp();
  return(0);
}

////////////////////////////////////////////////////////////////////////
inline ostream& operator<<(ostream& ios,statistics& stat) {
  ios << "sigma=" << stat.sigma() ;
  ios << "  min=" << stat.min() << "  max=" << stat.max() ;
  ios << "  stddev=" << stat.stddev() << "  mean=" << stat.mean() ;
  ios << "  nsample=" << stat.nsample() << "  error=" << stat.error() ;
  ios << "\n";
  return(ios);
}
////////////////////////////////////////////////////////////////////////
inline bool operator==(const statistics& a,const statistics& b) {
  return(a.nsample()==b.nsample()); 
}
inline bool operator!=(const statistics& a,const statistics& b) { 
  return(a.nsample()!=b.nsample()); 
}
inline bool operator<(const statistics& a,const statistics& b) { 
  return(a.nsample()<b.nsample()); 
}

#endif
