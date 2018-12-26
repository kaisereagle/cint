/************************************************************************
* cintgraph.h
*
*  makecint -A -dl xgraph.sl -c xgraph.c
************************************************************************/

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

#ifndef CINTGRAPH_H
#define CINTGRAPH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
using namespace std;
#include "Vector.h"

#define G__XGRAPHSL

//char *graphbuf="/Users/gotom/root/cint/cint/graphbuf";
//char *flagfile="/Users/gotom/root/cint/cint/graphbuf/doplot";
extern string cintgraphbuf;
extern string cintflagfile;
/************************************************************************
* rootgraph library
************************************************************************/

int cintgraph_open(const char *filename,const char *title) ;
int cintgraph_data(const char *filename,DMatrix& xdata,DMatrix& ydata,int n,int ndata,const char *name,int color= -1);
int cintgraph_data(const char *filename,double *xdata,double *ydata,int ndata,const char *name,int color= -1);
int cintgraph_invoke(const char *filename,double xmin,double xmax,double ymin,double ymax
		     ,int xlog,int ylog,const char *xunit,const char *yunit);
int cintgraph_command(const char* command,const char* arg=0) ;
int cintgraph_clear() ;
int cintgraph_save() ;
int cintgraph_exit() ;
int cintgraph_stop() ;
int cintgraph_start() ;

int plot_cintgraph(const string& fname) ;

inline int rootgraph_option(const char* opt) {
  return(cintgraph_command("option",opt));
}
inline int rootgraph_lineatt(int att) {
  char buf[10]; 
  sprintf(buf,"%d",att);
  return(cintgraph_command("lineatt",buf));
}

/************************************************************************
* 2D/3D Histogram library
************************************************************************/
void cintgraph_Histogram2D(const char* filename, const DMatrix& D
			   ,const char* title,const char* xname
			   ,double xmin,double xmax
			   ,double ymin,double ymax
			   ,double zmin,double zmax) ;

/************************************************************************
* gnuplot
************************************************************************/
void cintgraph_mode() ;
void gnuplot_mode() ;
int graph_mode() ;

int gnuplot_open(const char *filename,const char *title,const char* xname
		 ,const char* yunit
		 ,double xmin,double xmax,double ymin,double ymax
		 ,int xlog,int ylog);
int gnuplot_title(const char *filename,int n,int ndata
                  ,const char *name,int color) ;
int gnuplot_data(const char *filename,DMatrix& xdata,DMatrix& ydata
                  ,int n,int ndata,const char *name,int color) ;
int gnuplot_data(const char *filename
		   ,double *xdata,double *ydata,int ndata
		   ,const char *name,int color) ;
int gnuplot_invoke(const char *filename
		     ,double xmin,double xmax,double ymin,double ymax
		     ,int xlog,int ylog
		     ,const char *xunit,const char *yunit) ;

void gnuplot_Histogram2D(const char* filename, const DMatrix& D
			   ,const char* title,const char* xname
			   ,double xmin,double xmax
			   ,double ymin,double ymax
			   ,double zmin,double zmax) ;
#endif // CINTGRAPH_H
