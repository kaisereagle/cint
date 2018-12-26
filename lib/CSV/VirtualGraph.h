/////////////////////////////////////////////////////////////////////////////
// VirtualGraph.h
//
//  Virtual Graph drawing driver for Neural Network experiment
//
// Date 2014 June 12, Masaharu Goto
/////////////////////////////////////////////////////////////////////////////
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

#ifndef VIRTUALGRAPH_H
#define VIRTUALGRAPH_H

#include "Vector.h"
#define VG_N_MAX 20

#include <stdio.h>
#include <vector>
using namespace std;
#ifndef NEURALNET_H
typedef float NN_REAL;
///////////////////////////////////////////////////////////////////////
template<class T>
void savevectorbin(const vector<T>& a,FILE* fp) {
  unsigned int vsize = a.size();
  fwrite(&vsize,sizeof(unsigned int),1,fp);
  //if(DEBUG>=2) printf("savevectorbin size = %d\n",vsize);
  fwrite(&a.front(),sizeof(NN_REAL),vsize,fp);
}

///////////////////////////////////////////////////////////////////////
template<class T>
void loadvectorbin(vector<T>& a,FILE* fp) {
  unsigned int vsize;
  fread(&vsize,sizeof(unsigned int),1,fp);
  //if(DEBUG>=2) printf("loadvectorbin size = %d\n",vsize);
  a.resize(vsize);
  fread(&a.front(),sizeof(NN_REAL),vsize,fp);
}
#endif

template<class T> inline T Max__(const T& a,const T& b) { return(a>b?a:b); }
//template<class T> inline T Min__(const T& a,const T& b) { return(a<b?a:b); }

#define VG_MASK_PLOTMODE  0xf0
#define VG_PLOTMODE       0x20
#define VG_SAMEMODE       0x40

//#include <NVector.h>
#ifndef NOCOMPLEX
#define NOCOMPLEX
#endif
#include "Vector.h"
#include <string>

extern int VG_XSIZE;
extern int VG_YSIZE;

//extern string vg_opt ;
//extern int    vg_width;
extern const string& virtualgraph_getoption() ;

//#define NOVECTORMEMBER

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// VIrtual Graph
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
class VirtualGraph {
 public:
  //////////////////////////////////////////////////////////////////
#ifdef NOVECTORMEMBER
  NN_REAL* m_canvas; // x1.... x2.... x3.....
  int      m_canvas_size;
#else
  vector<NN_REAL> m_canvas; // x1.... x2.... x3.....
#endif
  int m_xsize;
  int m_ysize;
  double m_xmin;
  double m_xmax;
  double m_ymin;
  double m_ymax;
  int    m_xlog;
  int    m_ylog;

  //////////////////////////////////////////////////////////////////
  double m_xscale;
  double m_yscale;

  void clear() {
#ifdef NOVECTORMEMBER
    if(m_canvas) delete[] m_canvas;
    m_canvas=0;
    m_canvas_size=0;
#else
    m_canvas.clear();
#endif
  }

  VirtualGraph(int xsize=VG_XSIZE,int ysize=VG_YSIZE) { init(xsize,ysize); }
  //////////////////////////////////////////////////////////////////
  // Initialization
  void init(int xsize=VG_XSIZE,int ysize=VG_YSIZE) {
    m_xsize=xsize;
    m_ysize=ysize;
#ifdef NOVECTORMEMBER
    m_canvas = new NN_REAL[m_xsize*m_ysize];
    m_canvas_size=m_xsize*m_ysize;
#else
    m_canvas.resize(m_xsize*m_ysize);
#endif
  }
  void setXscale(double xmin,double xmax,int xlog=0) { 
    double m = (xmax - xmin)/50;
    m_xmin=xmin-m; m_xmax=xmax+m; m_xlog=xlog; m_xscale=m_xmax-m_xmin;
  }
  void setYscale(double ymin,double ymax,int ylog=0) { 
    double m = (ymax - ymin)/50;
    m_ymin=ymin-m; m_ymax=ymax+m; m_ylog=ylog; m_yscale=m_ymax-m_ymin;
  }

  //////////////////////////////////////////////////////////////////
  void drawLine(double x1,double y1,double x2,double y2,int width=1) {
    double xdiff = x2-x1;
    double ydiff = y2-y1;
    int  stepx = int(fabs(xdiff)/m_xscale*VG_XSIZE);
    int  stepy = int(fabs(ydiff)/m_yscale*VG_YSIZE);
    int step = Max__(stepx,stepy)*2.0;
    if(step<1) step=1;
    xdiff /= step;
    ydiff /= step;
    double x,y;
    int xi,yi; 
    for(int i=0;i<step;i++) {
      x = x1 + xdiff*i;
      y = y1 + ydiff*i;
      mapXscale(x,xi); 
      mapYscale(y,yi);
      putDot(xi,yi,width);
    }
  }
  void drawLines(const vector<double>& x,const vector<double>& y,int width=1) {
    //if(tolower(vg_opt[0])=='d') {
    if(tolower(virtualgraph_getoption()[0])=='d') {
      for(int i=0;i<x.size()-1 && i<y.size()-1 ;i++)
       drawDot(x[i],y[i],width);
    }
    else { //if(tolower(vg_opt[0])=='l') {
#if 1
      for(int i=0;i<x.size()-1 && i<y.size()-1 ;i++)  
        drawLine(x[i],y[i],x[i+1],y[i+1],width);
#else
      statistics stat;
      double xdif;
      for(int i=0;i<x.size()-1 && i<y.size()-1 ;i++)  {
        xdif = x[i+1]-x[i];
        if(stat.nsample()==0 || (stat.mean()*xdif>=0 && fabs(xdif) < m_xscale*0.7) ) {
          stat.add(xdif);
          drawLine(x[i],y[i],x[i+1],y[i+1],width);
        }
      }
#endif
    }
  }
  void drawDot(double xd,double yd,double size=1) {
    int xi,yi; mapXscale(xd,xi); mapYscale(yd,yi);
    for(int i=0;i<size;i++) 
      for(int j=0;j<size;j++) 
        putDot(xi+i,yi+j);
  }

  //////////////////////////////////////////////////////////////////
  void mapXscale(double xd,int& xi) {
    xi = int(m_xsize*(xd-m_xmin)/m_xscale);
  }
  void mapYscale(double yd,int& yi) {
    yi = int(m_ysize*(1-(yd-m_ymin)/m_yscale));
  }
  void mapXYscale(double xd,double yd,int xi,int yi) {
    mapXscale(xd,xi);
    mapYscale(yd,yi);
  }

  //////////////////////////////////////////////////////////////////
  void putDot(int x,int y,int size=1) {
    if(x>=0 && x<m_xsize && y>=0 && y<m_ysize) {
      for(int i=0;i<size;i++) 
        for(int j=0;j<size;j++) 
         m_canvas[x+i+(y+j)*m_xsize] = 1.0;
    } 
  }

  //////////////////////////////////////////////////////////////////
  void disp(FILE* fp) {
    int i;
#ifdef NOVECTORMEMBER
    for(i=0;i<m_canvas_size;i++) {
#else
    for(i=0;i<m_canvas.size();i++) {
#endif
      if(i&&i%m_xsize==0) fprintf(fp,"\n");
      fprintf(fp,"%g,",m_canvas[i]);
    }
    fprintf(fp,"\n");
    //fprintf(fp,"END NN-INPUT\n"); // put this in upper structure
  }

  //////////////////////////////////////////////////////////////////
  void disp(const char* fname=0,const char* mode="w") {
    if(fname) {
      FILE* fp=fopen(fname,mode);
      disp(fp);
      //fprintf(fp,"END NN-INPUT\n"); // put this in upper structure
      fclose(fp);
    }
    else {
      printf("%d %d\n",m_xsize,m_ysize);
      printf("x:%g,%g %g    y:%g:%g  %g\n"
              ,m_xmin,m_xmax,m_xscale
              ,m_ymin,m_ymax,m_yscale);
    }
  }

  //////////////////////////////////////////////////////////////////
  void savebin(FILE* fp) {
#ifdef NOVECTORMEMBER
    vector<NN_REAL> tmp;
    for(int i=0;i<m_canvas_size;i++) tmp.push_back(m_canvas[i]);
    savevectorbin(tmp,fp);
#else
    savevectorbin(m_canvas,fp);
#endif
  }

  //////////////////////////////////////////////////////////////////
  void savebin(const char* fname,const char* mode="wb") {
    FILE* fp=fopen(fname,mode);
    if(!fp) {
      fprintf(stderr
	      ,"Error: VirtualGraph::savebin() cannot create %s\n",fname);
      return;
    }
    savebin(fp);
    fclose(fp);
  }
  //////////////////////////////////////////////////////////////////
  void loadbin(FILE* fp) {
#ifdef NOVECTORMEMBER
    vector<NN_REAL> tmp;
    loadvectorbin(tmp,fp);
    if(m_canvas) { delete[] m_canvas; }
    m_canvas = new NN_REAL[tmp.size()];
    for(int i=0;i<tmp.size();i++) m_canvas[i] = tmp[i];
#else
    loadvectorbin(m_canvas,fp);
#endif
  }

  //////////////////////////////////////////////////////////////////
  void loadbin(const char* fname,const char* mode="rb") {
    FILE* fp=fopen(fname,mode);
    if(!fp) {
      fprintf(stderr
	      ,"Error: VirtualGraph::savebin() cannot open %s\n",fname);
      return;
    }
    loadbin(fp);
    fclose(fp);
  }
};
/////////////////////////////////////////////////////////////////////////////
inline NN_REAL innerproduct(const VirtualGraph& a,const VirtualGraph& b) {
#if 1
  unsigned int i;
#ifdef NOVECTORMEMBER
  vector<NN_REAL> ai;
  for(i=0;i<a.m_canvas_size;i++) ai.push_back(a.m_canvas[i]);
  vector<NN_REAL> bi;
  for(i=0;i<b.m_canvas_size;i++) bi.push_back(b.m_canvas[i]);
  for(i=0;i<a.m_canvas_size();i++) ai[i] = a.m_canvas[i]*2.0-1.0;
  for(i=0;i<b.m_canvas_size();i++) bi[i] = b.m_canvas[i]*2.0-1.0;
#else
  vector<NN_REAL> ai = a.m_canvas;
  vector<NN_REAL> bi = b.m_canvas;
  for(i=0;i<a.m_canvas.size();i++) ai[i] = a.m_canvas[i]*2.0-1.0;
  for(i=0;i<b.m_canvas.size();i++) bi[i] = b.m_canvas[i]*2.0-1.0;
#endif
  unsigned int n=ai.size();
  NN_REAL m = 0;
  for(i=0;i<n;i++) m += ai[i] * bi[i];
  return(m);
#else
  vector<NN_REAL> ai = a.m_canvas*2.0-1.0;
  vector<NN_REAL> bi = b.m_canvas*2.0-1.0;
  return(innerproduct(ai,bi));
#endif
}
/////////////////////////////////////////////////////////////////////////////

//extern VirtualGraph VG_OBJ[];
extern int VG_N;

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// end VIrtual Graph
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
VirtualGraph& virtualgraph_front() ;
VirtualGraph& virtualgraph_back() ; 
VirtualGraph& virtualgraph_get(int i) ;
int virtualgraph_size() ;

vector<NN_REAL>& virtualgraph_NNInput() ;

void virtualgraph_setXYSize(int xsize,int ysize) ;
void virtualgraph_option(const char* opt) ;
void virtualgraph_setWidth(int width) ;

/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// VirtualGraph class is defined in
//   $CINTSYSDIR/lib/NVector/VirtualGraphLib.h/cxx
/////////////////////////////////////////////////////////////////////////////

class Vectorostream;
void Virtualgraph_getMinMax(const DMatrix& d,double& dmin,double& dmax);
int virtualgraph_invoke(Vectorostream& vs) ;
int virtualgraph_invoke(Vectorostream* vs) ;

/////////////////////////////////////////////////////////////////////////////

/************************************************************************
* 2D/3D Histogram library
************************************************************************/
void VirtualGraph_Histogram2D(const char* filename, const DMatrix& D
			      ,const char* title,const char* xname
			      ,double xmin,double xmax
			      ,double ymin,double ymax
			      ,double zmin,double zmax) ;

#endif // VIRTUALGRAPH_H
