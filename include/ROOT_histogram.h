/**********************************************************************
* ROOT histogram drawing
**********************************************************************/

#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#define TRANSVERSE

// DrawHistogram.dll is actually libSimCUtil.dll
#if 1
#include <ReadF.h>
//#include <EasyExp.h>
#include <Matrix23.h>
#else
#include <DrawHistogram.dll>
#endif
#include <vector>

void SetHistRange(double xmin=0,double xmax=0
		   ,double ymin=0,double ymax=0
		   ,double zmin=0,double zmax=0) {
  rh_hxmin=xmin;
  rh_hxmax=xmax;
  rh_hymin=ymin;
  rh_hymax=ymax;
  rh_hzmin=zmin;
  rh_hzmax=zmax;
}

static  double rh_hxmin,rh_hxmax;
static  double rh_hymin,rh_hymax;
static  double rh_hzmin,rh_hzmax;
/**********************************************************************
* 2D Histogramによる 3D 表示
**********************************************************************/
static double rh_phi=20;
static double rh_theta=30;
static double rh_phiRotate=0,rh_phiStop=360, _phiRot=0;
static double rh_thetaRotate=0,rh_thetaStop=90, _thetaRot=0;

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// 2D/3D Visualization
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
static const int G__MAXCANVAS=50;
static  TH1D *rh_h1[G__MAXCANVAS];
static  TH2D *rh_h2[G__MAXCANVAS];
static  TH3D *rh_h3[G__MAXCANVAS];
static  TCanvas *rh_c[G__MAXCANVAS];
static  TPad *rh_pad[G__MAXCANVAS];
static  int rh_a=0;
//vector<double> HX,HY,HZ; // nonuniform mesh size information
static  TGraph2D *rh_g2d[G__MAXCANVAS];

// TCanvas
static int rh_wx=800,rh_wy=700;
static int rh_xoffset=0,rh_offset=0; // 表示Windowの位置

static double rh_histlog=0.0;

//static double rx,ry,rz;
static int rh_x,rh_y,rh_z; // cross section
static int rh_step=0,rh_max=0;

// SCAT BOX ARR 
// COLZ CONTZ CONT1-3 
// LEGO LEGO1-2 
// POL SPH PSR 
// SURF1-4 SURFZ
//string rh_draw="surf1z";
//string rh_opt="surf1z";
//string rh_opt1="C"; // smooth line
//string rh_opt1="L"; // straight line
string rh_opt1="H"; // at each bin
string rh_opt2="surf1";
string rh_opt3="surf1";

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// Transform from non-uniform mesh to uniform mesh
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//int remesh=0;

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
/**********************************************************************
* Reduce dimentaion
**********************************************************************/
void ReduceDimention2(Matrix2<double>& D2,Matrix1<double>& D1
	             ,int x,int y){
  double *mat2 = D2.Data();
  int i,j;
  int xsize=D2.XSize();
  int ysize=D2.YSize();
  double *mat;
  if(x) {
    D1.Allocate(ysize);
    mat = D1.Data();
    for(j=0;j<ysize;j++) mat[j] = mat2[x*ysize+j];
  }
  else if(y) {
    D1.Allocate(xsize);
    mat = D1.Data();
    for(i=0;i<xsize;i++) mat[i] = mat2[i*ysize+y];
  }
  else {
    fprintf(stderr,"Error(ReduceDimention2): Both x and y are 0\n");
  }
}

//////////////////////////////////////////////////////////////////////
void ReduceDimention3(Matrix3<double>& D3,Matrix2<double>& D2
	             ,int x,int y,int z){
  double *mat3 = D3.Data();
  int i,j,v;
  int xsize=D3.XSize();
  int ysize=D3.YSize();
  int zsize=D3.ZSize();
  double *mat;

  if(x) {
    D2.Allocate(ysize,zsize);
    mat = D2.Data();
    for(j=0;j<ysize;j++) 
     for(v=0;v<zsize;v++) 
      mat[j*zsize+v] = mat3[(x*ysize+j)*zsize+v];
  }
  else if(y) {
    D2.Allocate(xsize,zsize);
    mat = D2.Data();
    for(i=0;i<xsize;i++) 
     for(v=0;v<zsize;v++) 
      mat[i*zsize+v] = mat3[(i*ysize+y)*zsize+v];
  }
  else if(z) {
    D2.Allocate(xsize,ysize);
    mat = D2.Data();
    for(i=0;i<xsize;i++) 
     for(j=0;j<ysize;j++) 
      mat[i*ysize+j] = mat3[(i*ysize+j)*zsize+z];
  }
  else {
    fprintf(stderr,"Error(ReduceDimention3): All of x,y,z are 0\n");
  }
}

//////////////////////////////////////////////////////////////////////
void ReduceDimention3(Matrix3<double>& D3,Matrix1<double>& D1
	             ,int x,int y,int z){
  double *mat3 = D3.Data();
  int i,j,v;
  int xsize=D3.XSize();
  int ysize=D3.YSize();
  int zsize=D3.ZSize();
  double *mat;

  if(x&&y) {
    D1.Allocate(zsize);
    mat = D1.Data();
    for(v=0;v<zsize;v++) 
      mat[v] = mat3[(x*ysize+y)*zsize+v];
  }
  else if(y&&z) {
    D1.Allocate(xsize);
    mat = D1.Data();
    for(i=0;i<xsize;i++) 
      mat[i] = mat3[(i*ysize+y)*zsize+z];
  }
  else if(z&&x) {
    D1.Allocate(ysize);
    mat = D1.Data();
    for(j=0;j<ysize;j++) 
      mat[j] = mat3[(x*ysize+j)*zsize+z];
  }
  else {
    fprintf(stderr,"Error(ReduceDimention2): All of x,y,z are 0\n");
  }
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/**********************************************************************
* DataProcessing
**********************************************************************/
//////////////////////////////////////////////////////////////////////
void DataProcessing(Matrix1<double>& D) {
  double val;
  int i;
  int xsize = D.XSize(); 
  double *mat = D.Data();
  double absmin=1e99;

  if(rh_histlog) {
    for(i=0;i<xsize;i++) {
      val = mat[i]; 
      if(fabs(val)<absmin && val!=0) absmin=fabs(val);
    }
    printf("ABSMIN=%g , rh_histlog=%g\n",absmin,rh_histlog);
    absmin=rh_histlog;
    for(i=0;i<xsize;i++) {
      val = mat[i]/absmin; 
      if(fabs(val)<1.0) val=val>0?1.0:-1.0;
      if(val==0.0)   val = 0.0;
      else if(val>0) val = log10(val);
      else           val = -log10(-val);
      mat[i] = val;
    }
  }
}

//////////////////////////////////////////////////////////////////////
void DataProcessing(Matrix2<double>& D) {
  double val;
  int i,j;
  int xsize = D.XSize(); 
  int ysize = D.YSize();
  double *mat = D.Data();
  double absmin=1e99;

  if(rh_histlog) {
    for(i=0;i<xsize;i++) {
     for(j=0;j<ysize;j++) {
      val = mat[i*ysize+j]; 
      if(fabs(val)<absmin && val!=0) absmin=fabs(val);
     }
    }
    printf("ABSMIN=%g , rh_histlog=%g\n",absmin,rh_histlog);
    absmin=rh_histlog;
    for(i=0;i<xsize;i++) {
     for(j=0;j<ysize;j++) {
      val = mat[i*ysize+j]/absmin; 
      if(fabs(val)<1.0) val=val>0?1.0:-1.0;
      if(val==0)     val = 0.0;
      else if(val>0) val = log10(val);
      else           val = -log10(-val);
      mat[i*ysize+j] = val; 
     }
    }
  }
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void DataProcessing(Matrix3<double>& D) {
  double val;
  int i,j,z;
  int xsize = D.XSize(); 
  int ysize = D.YSize();
  int zsize = D.ZSize();
  double *mat = D.Data();
  double absmin=1e99;

  if(rh_histlog) {
    for(i=0;i<xsize;i++) {
     for(j=0;j<ysize;j++) {
      for(int v=0;v<zsize;v++) {
       val = mat[(i*ysize+j)*zsize+v]; 
       if(fabs(val)<absmin && val!=0) absmin=fabs(val);
      }
     }
    }
    printf("ABSMIN=%g , rh_histlog=%g\n",absmin,rh_histlog);
    absmin=rh_histlog;
    for(i=0;i<xsize;i++) {
     for(j=0;j<ysize;j++) {
      for(int i=0;i<xsize;i++) {
       val = mat[i*ysize+j]/absmin; 
       if(fabs(val)<1.0) val=val>0?1.0:-1.0;
       if(val==0)     val = 0.0;
       else if(val>0) val = log10(val);
       else           val = -log10(-val);
       mat[i*ysize+j] = val; 
      }
     }
    }
  }
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// Display non-uniform mesh with TGraph2D (NOT USED)
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
/**********************************************************************
* 1D Graphによる表示
**********************************************************************/
void DispGraph1D(Matrix1<double>& D,char* fname="",char* rmode="",char* comment=""
                ,double llim=0,double ulim=0,vector<double>& HX) {
  // There is no corresponding TGraph1D class
  DispHistogram1D(D,fname,rmode,comment ,llim,ulim);
}

/**********************************************************************
* 2D Graphによる 3D 表示
**********************************************************************/
void DispGraph2D(Matrix2<double>& D,char* fname="",char* rmode="",char* comment=""
                ,vector<double>& HX,vector<double>& HY) {
  // printf("ReadMatrix %s %s\n",rmode,comment);
  char name[500];
  char com[505];
  double val;

  //if(ibuf2<MAXBUF) buf2[ibuf2++] = D;

  int xsize = D.XSize(); 
  int ysize = D.YSize();
  int i,j;

  sprintf(name,"%s_c%d%s",fname,a,rmode);
  //sprintf(name,"%s_c%d",fname,a);
  //sprintf(com,"%s:%s",fname,rmode);
  rh_c[rh_a] = new TCanvas(name,com,10+rh_a*20+rh_xoffset,10+rh_a*20+rh_offset,rh_wx,rh_wy);
  sprintf(name,"pad%d",a);
  //sprintf(com,"%s %s",rmode,comment);
  //rh_pad[rh_a] = new TPad(name,com,0.01,0.01,0.99,0.99,21);

  sprintf(name,"%s_h%d",fname,a);
  sprintf(com,"%s:%s %s",fname,rmode,comment);
  rh_g2d[rh_a] = new TGraph2D();
  rh_g2d[rh_a]->SetTitle(name);

  double *mat = D.Data();

  //printf("D:%d %d HX %d HY %d\n",D.XSize(),D.YSize(),HX.size(),HY.size());

  DataProcessing(D);
  vector<double> Xaxis,Yaxis;
  double h;
  int i;
  val=0;
  for(i=0;i<xsize;i++) {
    Xaxis.push_back(val);
    val+=HX[i];
  }
  Xaxis.push_back(val);
  val=0;
  for(i=0;i<ysize;i++) {
    Yaxis.push_back(val);
    val+=HY[i];
  }
  Yaxis.push_back(val);

  h=HX[1];

  int N=0;
  for(i=0;i<xsize;i++) {
    if(rh_hxmin!=rh_hxmax && (Xaxis[i]<rh_hxmin || rh_hxmax<Xaxis[i])) continue;
    for(j=0;j<ysize;j++) {
      if(rh_hymin!=rh_hymax && (Yaxis[i]<rh_hymin || rh_hymax<Yaxis[i])) continue;
      val = mat[i*ysize+j]; 
      //rh_g2d[rh_a]->SetPoint(N++,Xaxis[i],Yaxis[j],val);
      //rh_g2d[rh_a]->SetPoint(N++,Yaxis[j],Xaxis[i],val); // xyを逆にしている
      rh_g2d[rh_a]->SetPoint(N++,Yaxis[j],Xaxis[i],val); // xyを逆にしている
      //printf("vaL N=%d Y[%d]=%g X=[%d]=%g val=%g\n",N,j,Yaxis[j],i,Xaxis[i],val);
    }
  }

  //rh_pad[rh_a]->Draw();

  //rh_g2d[rh_a]->SetStats(kFALSE);
  //rh_pad[rh_a]->SetTheta(rh_theta);
  //rh_pad[rh_a]->SetPhi(rh_phi);
  //rh_pad[rh_a]->cd();
  gStyle->SetPalette(1);
  rh_g2d[rh_a]->Draw(rh_opt2.c_str());

  ++rh_a;
}

/**********************************************************************
* 3D Graphによる 3D 表示 (Scatter plot)
**********************************************************************/
void DispGraph3D(Matrix3<double>& D,char* fname="",char* rmode="",char* comment=""
                  ,vector<double>& HX,vector<double>& HY,vector<double>& HZ) {
  // There is no corresponding TGraph3D class
  DispHistogram3D(D,fname,rmode,comment);
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// Display uniform mesh using TH123D
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
/**********************************************************************
* 1D Histogramによる表示
**********************************************************************/
void DispHistogram1D(Matrix1<double>& D,char* fname="",char* rmode="",char* comment="") {
  // printf("ReadMatrix %s %s\n",rmode,comment);
  char name[500];
  char com[505];
  double val;

  if(ibuf1<MAXBUF) buf1[ibuf1++] = D;

  int xsize = D.XSize(); 
  int i=0;

  sprintf(name,"%s_h%d",fname,a);
  sprintf(com,"%s:%s %s",fname,rmode,comment);

  rh_h1[rh_a]=new TH1D(name,com,xsize-1,0,xsize-1);

  double *mat = D.Data();

  DataProcessing(D);

  for(i=0;i<xsize;i++) {
    val = mat[i]; 
    rh_h1[rh_a]->SetBinContent(i,val);
  }

  sprintf(name,"%s_c%d%s",fname,a,rmode);
  //sprintf(name,"%s_c%d",fname,a);
  //sprintf(com,"%s:%s",fname,rmode);
  rh_c[rh_a] = new TCanvas(name,com,10+rh_a*20+rh_xoffset,10+rh_a*20+rh_offset,rh_wx,rh_wy);

  sprintf(name,"pad%d",a);
  //sprintf(com,"%s %s",rmode,comment);
  rh_pad[rh_a] = new TPad(name,com,0.01,0.01,0.99,0.99,21);
  rh_pad[rh_a]->Draw();

  if(rh_hxmin!=rh_hxmax) rh_h1[rh_a]->SetAxisRange(rh_hxmin,rh_hxmax,"X");
  rh_h1[rh_a]->SetStats(kFALSE);
  rh_pad[rh_a]->cd();
  rh_h1[rh_a]->Draw(rh_opt1.c_str());

  ++rh_a;
}

/**********************************************************************
* 2D Histogramによる 3D 表示
**********************************************************************/
void DispHistogram2D(Matrix2<double>& D,char* fname="",char* rmode="",char* comment="") {
  // printf("ReadMatrix %s %s\n",rmode,comment);
  char name[500];
  char com[505];
  double val;
  int a = rh_a;

  //if(ibuf2<MAXBUF) buf2[ibuf2++] = D;

  int xsize = D.XSize(); 
  int ysize = D.YSize();
  int i,j;

#ifdef TRANSVERSE
  printf("Transverse\n");
  int tmp = xsize;  xsize=ysize; ysize=tmp;
  Matrix2<double> D2;
  D2.Allocate(xsize,ysize);
  double *mat=D.Data();
  double *mat2=D2.Data();
  for(i=0;i<xsize;i++) {
    for(j=0;j<ysize;j++) {
      mat2[i*ysize+j] = mat[j*xsize+i];
    }
  }
  D = D2;
#endif

  sprintf(name,"%s_h%d",fname,a);
  sprintf(com,"%s:%s %s",fname,rmode,comment);

  rh_h2[rh_a]=new TH2D(name,com,xsize-1,0,xsize-1,ysize-1,0,ysize-1);

  double *mat = D.Data();

  DataProcessing(D);

  for(i=0;i<xsize;i++) {
    for(j=0;j<ysize;j++) {
      val = mat[i*ysize+j]; 
      rh_h2[rh_a]->SetBinContent(i,j,val); // わざとxyを逆にしている
    }
  }

  sprintf(name,"%s_c%d%s",fname,a,rmode);
  //sprintf(name,"%s_c%d",fname,a);
  //sprintf(com,"%s:%s",fname,rmode);
  rh_c[rh_a] = new TCanvas(name,com,10+rh_a*20+rh_xoffset,10+rh_a*20+rh_offset,rh_wx,rh_wy);
sprintf(name,"pad%d",a);
  //sprintf(com,"%s %s",rmode,comment);
  rh_pad[rh_a] = new TPad(name,com,0.01,0.01,0.99,0.99,21);
  rh_pad[rh_a]->Draw();

  if(rh_hxmin!=rh_hxmax) rh_h2[rh_a]->SetAxisRange(rh_hxmin,rh_hxmax,"X");
  if(rh_hymin!=rh_hymax) rh_h2[rh_a]->SetAxisRange(rh_hymin,rh_hymax,"Y");
  if(rh_hzmin!=rh_hzmax) rh_h2[rh_a]->SetAxisRange(rh_hzmin,rh_hzmax,"Z");
  rh_h2[rh_a]->SetStats(kFALSE);
  rh_pad[rh_a]->SetTheta(rh_theta);
  rh_pad[rh_a]->SetPhi(rh_phi);
  rh_pad[rh_a]->cd();
  rh_h2[rh_a]->Draw(rh_opt2.c_str());

  ++rh_a;
}

double rh_max3d=100;
double rh_min3d=-50;

/**********************************************************************
* 3D Histogramによる 3D 表示 (Scatter plot)
**********************************************************************/
void DispHistogram3D(Matrix3<double>& D,char* fname="",char* rmode="",char* comment="") {
  // printf("ReadMatrix %s %s\n",rmode,comment);
  char name[500];
  char com[505];
  double val;

  if(ibuf3<MAXBUF) buf3[ibuf3++] = D;

  int xsize = D.XSize();
  int ysize = D.YSize();
  int zsize = D.ZSize();
  int i=0;

  sprintf(name,"%s_h%d",fname,a);
  sprintf(com,"%s:%s %s",fname,rmode,comment);

  rh_h3[rh_a]=new TH3D(name,com
                ,xsize-1,0,xsize-1
                ,ysize-1,0,ysize-1
                ,zsize-1,0,zsize-1);

  double *mat = D.Data();
  double min=1e99;
  double max=-1e99;
  double sum=0;

  DataProcessing(D);

  for(int v=0;v<zsize;v++) {
   for(int j=0;j<ysize;j++) {
    for(int i=0;i<xsize;i++) {
      val = mat[(i*ysize+j)*zsize+v]; 
      if(val<min) min=val;
      if(val>max) max=val;
    }
   }
  }

  if(max==min) {
    printf("min==max==%g,%g, %s %s 3D histogram aborted\n",min,max,fname,comment);
    //return;
  }
  else {
  double gain=rh_max3d/(fabs(max)>fabs(min)?fabs(max):fabs(min));
  double offset=0;

  for(int v=0;v<zsize;v++) {
   for(int j=0;j<ysize;j++) {
    for(int i=0;i<xsize;i++) {
      val = mat[(i*ysize+j)*zsize+v]; 
      val = fabs(val)*gain+offset;
      rh_h3[rh_a]->SetBinContent(i,j,v,val);
    }
   }
  }
  }

  sprintf(name,"%s_c%d%s",fname,a,rmode);
  //sprintf(name,"%s_c%d",fname,a);
  //sprintf(com,"%s:%s",fname,rmode);
  rh_c[rh_a] = new TCanvas(name,com,10+rh_a*20+rh_xoffset,10+rh_a*20+rh_offset,rh_wx,rh_wy);

  sprintf(name,"pad%d",a);
  //sprintf(com,"%s %s",rmode,comment);
  rh_pad[rh_a] = new TPad(name,com,0.01,0.01,0.99,0.99,21);
  rh_pad[rh_a]->Draw();

  if(rh_hxmin!=rh_hxmax) rh_h2[rh_a]->SetAxisRange(rh_hxmin,rh_hxmax,"Y");
  if(rh_hymin!=rh_hymax) rh_h2[rh_a]->SetAxisRange(rh_hymin,rh_hymax,"X");
  rh_h3[rh_a]->SetStats(kFALSE);
  rh_h3[rh_a]->SetMarkerColor(kBlue);
  //rh_h3[rh_a]->SetMarkerColor(kRed);
  //rh_h3[rh_a]->SetMarkerColor(a);
  rh_pad[rh_a]->SetTheta(rh_theta);
  rh_pad[rh_a]->SetPhi(rh_phi);
  rh_pad[rh_a]->cd();
  rh_h3[rh_a]->Draw(rh_opt3.c_str());

  ++rh_a;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// Preprocessing Historgram display
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void  resetxyz() {rh_x=rh_y=rh_z=0;}
/**********************************************************************
* 2D Histogram Data処理
**********************************************************************/
void Disp2D(Matrix2<double>& D,char* fname,char* rmode,char* comment) {

  if(a>G__MAXCANVAS-10) {
    fprintf(stderr,"Warning: Too many displays\n");
    return;
  }

  setxyz(D);
  if(rh_x||rh_y) {
    Matrix1<double> D1;
    ReduceDimention2(D,D1,rh_x,rh_y);
    DispHistogram1D(D1,fname,rmode,comment,llim,ulim);
  }
  else {
    DispHistogram2D(D,fname,rmode,comment,llim,ulim);
  }
  resetxyz();
}

/**********************************************************************
* 3D Histogram Data処理
**********************************************************************/
void Disp3D(Matrix3<double>& D,char* fname,char* rmode,char* comment) {

  if(a>G__MAXCANVAS-10) {
    fprintf(stderr,"Warning: Too many displays\n");
    return;
  }

  setxyz(D);
  if((rh_x&&rh_y) || (rh_y&&rh_z) || (rh_z&&rh_x)) {
    Matrix1<double> D1;
    ReduceDimention3(D,D1,rh_x,rh_y,rh_z);
    DispHistogram1D(D1,fname,rmode,comment,llim,ulim);
  }
  else if((rh_x||rh_y||rh_z) && rh_step==0) {
    Matrix2<double> D2;
    ReduceDimention3(D,D2,rh_x,rh_y,rh_z);
    DispHistogram2D(D2,fname,rmode,comment,llim,ulim);
  }
  else if(rh_x) {
    char com[500];
    Matrix2<double> D2;
    for(int i=rh_x;i<=rh_max;i+=rh_step) {
      ReduceDimention3(D,D2,i,y,z);
      sprintf(com,"%s x%d\n",comment,i);
      DispHistogram2D(D2,fname,rmode,com,llim,ulim);
    }
  }
  else if(y) {
    char com[500];
    Matrix2<double> D2;
    for(int i=y;i<=rh_max;i+=rh_step) {
      ReduceDimention3(D,D2,rh_x,i,rh_z);
      sprintf(com,"%s y%d\n",comment,i);
      DispHistogram2D(D2,fname,rmode,com,llim,ulim);
    }
  }
  else if(z) {
    char com[500];
    Matrix2<double> D2;
    for(int i=z;i<=rh_max;i+=rh_step) {
      ReduceDimention3(D,D2,rh_x,rh_y,i);
      sprintf(com,"%s z%d\n",comment,i);
      DispHistogram2D(D2,fname,rmode,com,llim,ulim);
    }
  }
  else {
    DispHistogram3D(D,fname,rmode,comment,llim,ulim);
  }
  resetxyz();
}


#endif
