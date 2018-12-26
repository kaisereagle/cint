/////////////////////////////////////////////////////////////////////////////
// VirtualGraph.cxx
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

/////////////////////////////////////////////////////////////////////////////
// VirtualGraph class is defined in
//   $CINTSYSDIR/lib/NVector/VirtualGraphLib.h/cxx
/////////////////////////////////////////////////////////////////////////////
#include "VirtualGraph.h"
#include "Vectoriostream.h"

int VG_XSIZE = 40;
int VG_YSIZE = 20;

static string vg_opt = "line"; // "dot";
static int    vg_width = 1;

static VirtualGraph VG_OBJ[VG_N_MAX];
int VG_N =0 ;

/////////////////////////////////////////////////////////////////////////////
VirtualGraph& virtualgraph_front() { 
  return(VG_OBJ[0]);
}
VirtualGraph& virtualgraph_back() { 
  return(VG_OBJ[VG_N-1]);
}
VirtualGraph& virtualgraph_get(int i) { 
  return(VG_OBJ[i]);
}
int virtualgraph_size() { return(VG_N); }

vector<NN_REAL>& virtualgraph_NNInput() { 
  return(VG_OBJ[VG_N-1].m_canvas);
}

void virtualgraph_setXYSize(int xsize,int ysize) 
  {VG_XSIZE=xsize; VG_YSIZE=ysize;}
void virtualgraph_option(const char* opt) { vg_opt = opt; }
void virtualgraph_setWidth(int width) { vg_width=width; }
const string& virtualgraph_getoption() { return(vg_opt); }


/////////////////////////////////////////////////////////////////////////////
void Virtualgraph_getMinMax(const DMatrix& d,double& dmin,double& dmax){
  dmin = 1e99, dmax = -1e99;
  double val;
  unsigned int i,j;
  for(j=0;j<d.size();j++) {
    for(i=0;i<d[j].size();i++) {
      val = d[j][i];
      if(dmax<val) dmax = val;
      if(dmin>val) dmin = val;
    }
  }
}

/////////////////////////////////////////////////////////////////////////////
int virtualgraph_invoke(Vectorostream& vs) {
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
  return(0);
}


/////////////////////////////////////////////////////////////////////////////
int virtualgraph_invoke(Vectorostream* vs) {
  int width=1;
  if(vs->m_mode==VG_PLOTMODE) {
    if(VG_N>=VG_N_MAX) VG_N=0;
    VG_OBJ[VG_N].clear();
    VG_OBJ[VG_N].init(VG_XSIZE,VG_YSIZE);
    double xmin=vs->m_xmin, xmax=vs->m_xmax;
    double ymin=vs->m_ymin, ymax=vs->m_ymax;
    if(xmin==xmax) Virtualgraph_getMinMax(vs->m_x,xmin,xmax);
    if(ymin==ymax) Virtualgraph_getMinMax(vs->m_y,ymin,ymax);
    VG_OBJ[VG_N].setXscale(xmin,xmax,vs->m_xlog);
    VG_OBJ[VG_N].setYscale(ymin,ymax,vs->m_ylog);
    ++VG_N;
  }
  for(int i=0;i<vs->m_x.size()&&i<vs->m_y.size();i++) {
    VG_OBJ[VG_N-1].drawLines(vs->m_x[i],vs->m_y[i],vg_width);
  }   
  return(0);
}

/************************************************************************
* 2D/3D Histogram library
************************************************************************/
void VirtualGraph_Histogram2D(const char* filename, const DMatrix& D
			      ,const char* title,const char* xname
			      ,double xmin,double xmax
			      ,double ymin,double ymax
			      ,double zmin,double zmax) {
  // how to normalize magnitude
  // if this is not a numeric matrix, dump m_cellid pattern
  //      
  // if this is a numeric matrix,     dump normalized m_original 
  //   min to max -> -1 to 1
  //   2 or 3 discrete values
  //   log10(x)

  // how to normalize table size
  //   table size as is?
  //   fixed size          , min, max, mean, median, stddev, max-min, etc.. 
  //   regex normalization

  // TO BE IMPLEMENTED
  const int xymax = 10;

  FILE *fp;
  char fname[500];
  strcpy(fname,filename);
  fp=fopen(fname,"w");
  if(!fp) {
    fprintf(stderr,"Error: Failed opening %s, VIrtualGraph_Histogram2D()\n"
	,fname);
    return;
  }

  int i,j;
  int xsize = (D.size()>0)?D[0].size():0;
  int ysize = D.size();

  double vmin=1e99,vmax=-1e99;
  for(i=0;i<ysize&&i<xymax;i++) {
    for(j=0;j<xsize&&i<xymax;j++) {
      if(D[i][j]<vmin) vmin=D[i][j];
      if(D[i][j]>vmax) vmax=D[i][j];
    }
  }
  double absmax = fabs(vmax)>fabs(vmin)?fabs(vmax):fabs(vmin);

  for(i=0;i<ysize&&i<xymax;i++) {
    for(j=0;j<xsize&&i<xymax;j++) {
      fprintf(fp,"%g,",D[i][j]/absmax);
    }
    while(j++<xymax) fprintf(fp,"%g,",0.0); // 0 padding if size < xymax
    fprintf(fp,"\n");
  }
  while(i++<xymax) {
    for(j=0;j<xymax;j++) fprintf(fp,"%g,",0.0); fprintf(fp,"\n"); // padding
  }
  
  //fprintf(fp,"END\n");
  fclose(fp);
}

/////////////////////////////////////////////////////////////////////////////
