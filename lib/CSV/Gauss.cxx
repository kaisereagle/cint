/**************************************************************************
* Gauss.cxx
*
* 2013/Sep 
* Masaharu Goto
*
**************************************************************************/
/************************************************************************
 * Copyright(c) 1995~2015  Masaharu Goto (cint@pcroot.cern.ch)
 *
 * Permission to use, copy, modify and distribute this software and its 
 * documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation. The author makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 ************************************************************************/

#include "Gauss.h"
#include "Stat.h"


// TODO.  improvement needed for following limitations
// 1. Presently, <1% probability is cut off. No data beyond >2.8
// 2. Flat distribution in single Look-up entry area  

static vector<float> g_N;  // look up table
static float g_step;       // look up table step size

// Initialization 
class g_gauss_t {
public:
  g_gauss_t() {
    DVector g_x(-3.0,3.0,1000),g_y= gauss(g_x,0.0,1.0)*100.0;
    g_step = g_x[1]-g_x[0];
    int i,j,s;
    for(i=0;i<g_x.size();i++) {
      s=int(g_y[i]);
      for(j=0;j<s;j++) g_N.push_back(g_x[i]);
    }
  }
};

static g_gauss_t g_gauss;  // initialization class

// gauss distribution generator
double randgauss(double mean,double stddev) {
  //return(g_N[rand()%g_N.size()]+rand()%1000*g_step/1000);
  return((g_N[rand()%g_N.size()]+rand()%1000*g_step/1000)/3.0*stddev);
}





