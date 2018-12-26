/////////////////////////////////////////////////////////////////////////////
// VirtualGraph.h
//
//  Virtual Graph drawing driver for Neural Network experiment
//
// Date 2014 June 12, Masaharu Goto
/////////////////////////////////////////////////////////////////////////////

#ifndef VIRTUALGRAPH_H
#define VIRTUALGRAPH_H

#define VG_N_MAX 20

//#include <NeuralNet.h>
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
#define NOCOMPLEX
#include "Vector.h"
#include <string>

extern int VG_XSIZE;
extern int VG_YSIZE;

extern string vg_opt ;
extern int    vg_width;

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// VIrtual Graph
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
class VirtualGraph {
 public:
  //////////////////////////////////////////////////////////////////
  vector<NN_REAL> m_canvas; // x1.... x2.... x3.....
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
    m_canvas.clear();
  }

  VirtualGraph(int xsize=VG_XSIZE,int ysize=VG_YSIZE) { init(xsize,ysize); }
  //////////////////////////////////////////////////////////////////
  // Initialization
  double init(int xsize=VG_XSIZE,int ysize=VG_YSIZE) {
    m_xsize=xsize;
    m_ysize=ysize;
    m_canvas.resize(m_xsize*m_ysize);
  }
  double setXscale(double xmin,double xmax,int xlog=0) { 
    double m = (xmax - xmin)/50;
    m_xmin=xmin-m; m_xmax=xmax+m; m_xlog=xlog; m_xscale=m_xmax-m_xmin;
  }
  double setYscale(double ymin,double ymax,int ylog=0) { 
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
    if(tolower(vg_opt[0])=='d') {
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
    for(i=0;i<m_canvas.size();i++) {
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
    savevectorbin(m_canvas,fp);
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
    loadvectorbin(m_canvas,fp);
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
  vector<NN_REAL> ai = a.m_canvas;
  vector<NN_REAL> bi = b.m_canvas;
  for(i=0;i<a.m_canvas.size();i++) ai[i] = a.m_canvas[i]*2.0-1.0;
  for(i=0;i<b.m_canvas.size();i++) bi[i] = b.m_canvas[i]*2.0-1.0;
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

extern VirtualGraph VG_OBJ[];
extern int VG_N;

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// end VIrtual Graph
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////


#if 0
/////////////////////////////////////////////////////////////////////////////
inline void Virtualgraph_getMinMax(const DMatrix& d,double& dmin,double& dmax){
  double dmin = 1e99, dmax = -1e99, val;
  unsigned int i,j;
  for(j=0;j<d.size();j++) {
    for(i=0;i<d[j].size();i++) {
      val = d[j][i];
      if(dmax<val) dmax = val;
      if(dmin>val) dmin = val;
    }
  }
}

class Vectorostream;
/////////////////////////////////////////////////////////////////////////////
inline int virtualgraph_invoke(Vectorostream& vs) {
  int width=1;

  if(vs.m_mode==VG_PLOTMODE) {
    if(VG_N>=VG_N_MAX) VG_N=0;
    VG_OBJ[VG_N].clear();
    VG_OBJ[VG_N].init(VG_XSIZE,VG_YSIZE);
    double xmin=vs.m_xmin, xmax=vs.m_xmax;
    double ymin=vs.m_ymin, ymax=vs.m_ymax;
    if(xmin==xmax) Virtualgraph_getMinMax(vs.m_x,xmin,xmax);
    if(ymin==ymax) Virtualgraph_getMinMax(vs.m_y,ymin,ymax);
    VG_OBJ[VG_N].setXscale(xmin,xmax,vs.m_xlog);
    VG_OBJ[VG_N].setYscale(ymin,ymax,vs.m_ylog);
    ++VG_N;
  }
  for(int i=0;i<vs.m_x.size()&&i<vs.m_y.size();i++) {
    VG_OBJ[VG_N-1].drawLines(vs.m_x[i],vs.m_y[i],vg_width);
  }   
}
#endif

/////////////////////////////////////////////////////////////////////////////
inline VirtualGraph& virtualgraph_front() { 
  return(VG_OBJ[0]);
}
inline VirtualGraph& virtualgraph_back() { 
  return(VG_OBJ[VG_N-1]);
}
inline VirtualGraph& virtualgraph_get(int i) { 
  return(VG_OBJ[i]);
}
inline int virtualgraph_size() { return(VG_N); }

inline vector<NN_REAL>& virtualgraph_NNInput() { 
  return(VG_OBJ[VG_N-1].m_canvas);
}


/////////////////////////////////////////////////////////////////////////////

#endif // VIRTUALGRAPH_H
