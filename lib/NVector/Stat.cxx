/*************************************************************************
* Stat.cxx
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

#include <cmath>
#include "Stat.h"

#ifndef TEMPLATE_STAT
////////////////////////////////////////////////////////////////////////
void statistics::disp(FILE* fp) const {
  fprintf(fp
          ,"sigma=%g min=%g max=%g stddev=%g mean=%g nsample=%d err=%d\n"
          ,sigma(),min(),max(),stddev(),mean(),nsample(),error());
}


////////////////////////////////////////////////////////////////////////
void statistics::init() {
  m_min = 1e99;
  m_max = -1e99;
  m_sigma = 0.0;
  m_sigma2 = 0.0;
  m_error = 0;
  m_nsample = 0;
}

////////////////////////////////////////////////////////////////////////
int statistics::add(double data,double lowlimit,double uplimit) {
  m_sigma += data;
  m_sigma2 += (data*data);
  ++m_nsample ;

  if(data>m_max) m_max=data;
  if(data<m_min) m_min=data;

  if(data < lowlimit || uplimit < data) {
    /* fprintf(stderr,"%g\n",data); */
    ++m_error;
    return(m_error);
  }
  return(0);
}

////////////////////////////////////////////////////////////////////////
int statistics::add(const statistics& st) {
  m_sigma += st.m_sigma;
  m_sigma2 += st.m_sigma2;
  m_nsample += st.m_nsample;

  if(st.m_max>m_max) m_max=st.m_max;
  if(st.m_min<m_min) m_min=st.m_min;

  return(0);
}

////////////////////////////////////////////////////////////////////////
int statistics::del(double data,double lowlimit,double uplimit) {
  m_sigma -= data;
  m_sigma2 -= (data*data);
  --m_nsample ;
  return(0);
}

////////////////////////////////////////////////////////////////////////
double statistics::stddev(void) const {
  double result;
  if(1>=m_nsample) 
    result=0.0;
  else
    result=sqrt((m_sigma2 - m_sigma*m_sigma/m_nsample)/(m_nsample-1));
  return(result);
}

////////////////////////////////////////////////////////////////////////
#endif
