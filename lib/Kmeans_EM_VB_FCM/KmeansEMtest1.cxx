
#ifndef G__ROOT
#include "Kmeans_EM_VB_FCM.dll"
#include <NVector.h>
int main() {
  KmeansEMtest1();
  cintgraph_stop();
  G__pause();
  return(0);
}
#else
#ifdef G__LINUX
#include "KmeansEM.dll"
#else
#include "libKmeansEM.dll"
#endif
#endif
#include <NVector.h>

void emdisp(const DEM& em) {
  printf("-----------");
  printf("Mean "); em.getMean().disp();
  printf("PI "); em.getPi().disp();
  printf("Covar0 "); em.getCovar(0).disp();
  printf("Covar1 "); em.getCovar(1).disp();
  //em.getGamma().disp();
  //em.getR().disp();
}

#if !defined(TROUBLE) && !defined(SAFE) && !defined(TROUBLE2) !defined(MARGINAL)
//#define TROUBLE
#define MARGINAL
//#define SAFE
#endif

void KmeansEMtest1(
#ifdef TROUBLE
  // Trouble case
  double axsigma=1.5,
  double aysigma=0.7,
  double bxsigma=1.4,
  double bysigma=0.2,
  double axmean=1.0, 
  double aymean=0.5, 
  double bxmean=-2.0, 
  double bymean=0.1, 
#endif
#ifdef MARGINAL
  // Marginal case
  double axsigma=1.5,
  double aysigma=0.7,
  double bxsigma=1.1, // ï™éUî‰ëÂÇ´Ç¢Ç∆é˚ë©ê´Ç™à´Ç≠Ç»ÇÈ
  double bysigma=0.2, //  
  double axmean=1.0, 
  double aymean=0.5, 
  double bxmean=-2.0, 
  double bymean=0.1, 
#endif
#ifdef SAFE
  // safe case
  double axsigma=1.5,
  double aysigma=0.7,
  double bxsigma=0.9,
  double bysigma=0.2,
  double axmean=1.0, 
  double aymean=0.5, 
  double bxmean=-2.0, 
  double bymean=0.1, 
#endif
  unsigned int Na=100,unsigned int Nb=51,
  int Xtotal = 20, int XKmeans = 10
) {
  G__optimizemode(3);

  DVector ax,ay,bx,by;
  // Trouble case
  //double axmean=1.0,axsigma=1.5;
  //double aymean=0.5,aysigma=0.7;
  //double bxmean=-2.0,bxsigma=1.4;
  //double bymean=0.1,bysigma=0.2;
  // Marginal case
  //double axmean=1.0,axsigma=1.5;
  //double aymean=0.5,aysigma=0.7;
  //double bxmean=-2.0,bxsigma=1.1; // ï™éUÇÃî‰Ç™ëÂÇ´Ç¢Ç∆é˚ë©ê´Ç™à´Ç≠Ç»ÇÈÇÁÇµÇ¢
  //double bymean=0.1,bysigma=0.2;  //  
  // safe case
  //double axmean=1.0,axsigma=1.5;
  //double aymean=0.5,aysigma=0.7;
  //double bxmean=-2.0,bxsigma=0.9;
  //double bymean=0.1,bysigma=0.2;
  //unsigned int Na=100,Nb=51;
  unsigned int i;
  for(i=0;i<Na;i++) {
    ax.push_back(RandomGauss(axmean,axsigma));
    ay.push_back(RandomGauss(aymean,aysigma));
  }
  for(i=0;i<Nb;i++) {
    bx.push_back(RandomGauss(bxmean,bxsigma));
    by.push_back(RandomGauss(bymean,bysigma));
  }

  rootgraph_option("A*");
  rootgraph_lineatt(1);


  DEM em;
  DVector x(0.0,0.0,2);
  unsigned int ia,ib;
  while(ia<Na || ib<Nb) {
    if(rand()%2) {
      if(ia<Na) {
        x[0] = ax[ia];
        x[1] = ay[ia++];
        em.push_back(x);
      }
    }
    else {
      if(ib<Nb) {
        x[0] = bx[ib];
        x[1] = by[ib++];
        em.push_back(x);
      }
    }
  }

  char title[100];

  em.Initialize(2);

  DVector xm(0.0,0.0,2);
  DVector ym(0.0,0.0,2);
  DVector tmp(0.0,0.0,2);
  tmp = em.getMean(0); xm[0] = tmp[0]; ym[0] = tmp[1];
  tmp = em.getMean(1); xm[1] = tmp[0]; ym[1] = tmp[1];
  drawgraph("Kmeans",ax,ay,"A",bx,by,"B",xm,ym,"K");

  //int Xtotal = 10, XKmeans = 0; // Bad
  //int Xtotal = 10, XKmeans = 2; // Bad
  //int Xtotal = 10, XKmeans = 3; // good
  //int Xtotal = 10, XKmeans = 5;
  //int Xtotal = 7, XKmeans = 5;
  DVector axx,ayy,bxx,byy;

  //if(XKmeans<0) em.Kmeans_Iteration(10);

  DVector prevam(1e99,1e99,2),prevbm(1e99,1e99,2);
  for(i=0;i<Xtotal;i++) {
    if(i<XKmeans) {
      int change;
      sprintf(title,"KMeans%d",i);
      printf("%s\n",title);
      change=em.Kmeans_Estep();
      em.Kmeans_Mstep();
      if(change==0) {
        printf("**** Kmeans method converged *** \n");
        XKmeans = i;
        emdisp(em);
      }
    }
    else {
      sprintf(title,"EM%d",i);
      printf("%s\n",title);
      if(i==0) em.EM_Estep();
      em.EM_Mstep();
      em.EM_Estep();
      emdisp(em);
    }

    tmp = em.getMean(0); xm[0] = tmp[0]; ym[0] = tmp[1];
    tmp = em.getMean(1); xm[1] = tmp[0]; ym[1] = tmp[1];
    //rootgraph_clear();
    axx.clear(); ayy.clear(); bxx.clear(); byy.clear();
    for(unsigned int j=0;j<Na+Nb;j++) {
      tmp=em.getX(j);
//printf("n=%d r=%d\n",j,em.getR(j));
      if(em.getR(j)==0) { axx.push_back(tmp[0]); ayy.push_back(tmp[1]); }
      else              { bxx.push_back(tmp[0]); byy.push_back(tmp[1]); }
    }
    printf("axx %d bxx %d \n",axx.size(),bxx.size());

    drawgraph(title,axx,ayy,"A",bxx,byy,"B",xm,ym,"K");

    double thresh=0.01;
    if(distance1(prevam,em.getMean(0))<thresh &&
       distance1(prevbm,em.getMean(1))<thresh &&
       i>XKmeans+2) {
      //prevam.disp(); em.getMean(0).disp();
      printf("***** %g %g ***\n" 
	     ,distance1(prevam,em.getMean(0))
             ,distance1(prevbm,em.getMean(1)));
      break;
    }
    prevam = em.getMean(0);
    prevbm = em.getMean(1);
  }


}

void drawgraph(const char* title,
	const DVector& axx,const DVector& ayy,const char* labelA,
	const DVector& bxx,const DVector& byy,const char* labelB,
	const DVector& xm,const DVector& ym,const char* labelK) {
  char labelAx[100],labelBx[100],labelKx[100];
  sprintf(labelAx,"%s %d (%g,%g)",labelA,axx.size(),xm[0],ym[0]);
  sprintf(labelBx,"%s %d (%g,%g)",labelB,bxx.size(),xm[1],ym[1]);
#if 1
  plot << title ;
  if(axx.size()) plot << axx <<"X" << ayy <<  labelAx ;
  if(bxx.size()) plot >> bxx <<"X" << byy <<  labelBx ;
  plot >> xm << ym << labelK << "Y\n";
#else
  rootgraph_open("",title,RG_PLOTMODE);
  if(axx.size()) rootgraph_data("",&axx[0],&ayy[0],axx.size(),labelAx);
  if(bxx.size()) rootgraph_data("",&bxx[0],&byy[0],bxx.size(),labelBx);
  rootgraph_data("",&xm[0],&ym[0],xm.size(),labelK);
  rootgraph_invoke("",0,0,0,0,0,0,"","",RG_PLOTMODE);
#endif
}
