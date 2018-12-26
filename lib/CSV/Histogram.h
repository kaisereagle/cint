/**************************************************************************
* Histogram.h
*
* 2014/Mar
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

#ifndef G__HISTOGRAM
#define G__HISTOGRAM

#include "Vector.h"

#if 0
template<class Tbin,class Tval>
class Histogram : public Vector<Tbin> {
 private:
  TVal m_base;
  TVal m_step;
  TVal m_min;
  TVal m_max;
 public:
};
#endif

/***********************************************
* Histogram adapter
***********************************************/
template<class Tval>
class Histogram : public DVector {
 public:
  Histogram() {setrange(100);} 
  Histogram(int nbin,Tval minin=0,Tval maxin=0) 
    {setrange(nbin,minin,maxin);}

  void clear() { 
    m_stat.clear();
    m_sum=0; 
    for(unsigned int i=0;i<size();i++) operator[](i)=0;
  }

  void setrange(const DVector& x) 
   {setrange(x.size(),x[1],x[x.size()-2]);}

  void setrange(int nbin,Tval minin=0,Tval maxin=0) 
    {resize(nbin); setrange(minin,maxin); clear() ;}

  void setrange(Tval minin=0,Tval maxin=0) {
    m_min=minin; m_max=maxin; 
    m_step=(m_max-m_min)/(size()-(2+0.001/size()));
    if(minin!=maxin) genscale();
  }

  void fill(const Vector<Tval>& x) {
    if(m_step==0) setrange(Min(x),Max(x));
    int index;
    for(unsigned int i=0;i<x.size();i++) {
      m_stat.add(x[i]);
      index = int((x[i]-m_min)/m_step)+1;
      if(index<0) index=0;
      else if(index>=size()) index=size()-1;
      get(index) += 1.0;
    }
    m_sum += x.size();
  }

  void fill(const Tval v) {
    m_stat.add(v);
    int  index = int((v-m_min)/m_step)+1;
    if(index<0) index=0;
    else if(index>=size()) index=size()-1;
    get(index) += 1.0;
    ++m_sum ;
  }

  DVector& hist() const { DVector *p = (DVector*)this; return(*p); }
  DVector loghist() const { 
    DVector r;
    double d;
    for(unsigned int i=0;i<size();i++) {
      d = log10(operator[](i)+1);
      r.push_back(d);
    }
    return(r);
  }

  void normalize() {
    if(m_sum==0) return;
    for(unsigned int i=0;i<size();i++) get(i) = get(i)/m_sum;
    m_sum=1.0;
  }

  statistics stat() { return(m_stat); }

  int  nbin() const { return(size()); }
  Tval min() const { return(m_min); }
  Tval max() const { return(m_max); }
  Tval step() const { return(m_step); }
  Tval sum() const { return(m_sum); }
  const Vector<Tval>& scale() { return(m_scale); }
  void genscale() {
    m_scale.resize(size());
    for(unsigned int i=0;i<size();i++) m_scale[i] = m_min+m_step*(i-1);
    m_scale[0] = m_min-m_step;
  }

 private:
  Vector<Tval>  m_scale;
  statistics    m_stat;
  Tval m_min;
  Tval m_max;
  Tval m_step;
  Tval m_sum;
};

//////////////////////////////////////////////////////////
typedef Histogram<double> DHistogram;
//////////////////////////////////////////////////////////
inline DVector hist(const DVector& x,int nbin=100
		    ,double xmin=0.0,double xmax=0.0) {
  DHistogram h;
  h.setrange(nbin,xmin,xmax);
  h.fill(x);
  return(h.hist());
}

#if 0
template<class Tval> 
inline DVector xhist(const Vector<Tval>& y,int nbin=100) {
  DVector x(0.0,0.0,nbin);
  x.set(min(y),max(y));
  return(x);
}
#endif



#endif // G__HISTOGRAM


