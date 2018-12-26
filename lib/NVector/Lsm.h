/*************************************************************************
* lsm.h
*
* Least square method library
*
*************************************************************************/

#ifndef G__LSM
#define G__LSM

#include "Vector.h"

int lstsq(const double x[],const double y[],int n,int m,double c[]);
int eval_lstsq(const double x[],double z[],int n,int m,const double c[]);

inline int lstsq(const DVector& x,const DVector& y,int m,DVector& c) {
  int n = x.size()<y.size()? x.size() : y.size();
  c.resize(m);
  int stat = lstsq(&x[0],&y[0],n,m,&c[0]);
  return(stat);
}

inline int eval_lstsq(const DVector& x,DVector& z,int m,const DVector& c) {
  int n = x.size();
  z.resize(n);
  int stat = eval_lstsq(&x[0],&z[0],n,m,&c[0]);
  return(stat);
}

#endif

