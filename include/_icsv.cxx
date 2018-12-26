/*****************************************************************************
 * _icsv.cxx
 *  Description: Intelligent CSV file handler
 *  Status: Experimental
 *  Date: 2014/Jan/3
 *  Author: Masaharu Goto
 *****************************************************************************/
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

#ifndef _ICSV_CXX
#define _ICSV_CXX

#ifdef __CINT__
#include <CSV.h>
//#include <NVector.h>
#else
#include "Collection.h"
#endif
#include <unistd.h>
#include <exception>


/////////////////////////////////////////////////////////////////////////
Sheet s;
Collection c;
ClassifyTable CC;

/////////////////////////////////////////////////////////////////////////
void classifyHugeFile(char* fname,const char* script) {
}

/////////////////////////////////////////////////////////////////////////
int classify(char* fname,const char* script) {
  int mode=0;
#ifdef NNW_V
  NNW_Vector = NNW_V;
#else
  //NNW_Vector = "T_*.nnw";
  NNW_Vector = "T3_*.nnw";
#endif
#ifdef NNW_M
  NNW_Matrix = NNW_M;
#endif
#ifdef PLOT
  CC.setplotmode(1);
#endif
#ifdef PLOTEACH
  CC.setploteachmode(1);
#endif
#ifdef VIEW
  CC.setviewmode(1);
#endif
#ifdef DISP
  CC.setdispmode(1);
#endif
  s.Read(fname);
  c=s.collection();

  Collection r;
  printf("############# Classification\n");
  try {
    CC.setOriginal(c,0,fname,"");
  }
  catch(exception& e) {
    printf("Exception setOriginal: %s  %s\n",e.what(),ClassifyTableProgress.c_str());
    return(1);
  }

  try {
    CC.Classification(1);
  }
  catch(exception& e) {
    printf("Exception Classification: %s  %s\n",e.what(),ClassifyTableProgress.c_str());
    return(2);
  }

  //CC.dispHier();

  printf("############# DetailClassificationTop\n");
  try {
    r=CC.DetailClassificationTop() ;
  }
  catch(exception& e) {
    printf("Exception DetailClassification: %s  %s\n",e.what(),ClassifyTableProgress.c_str());
    return(3);
  }

  printf("############# r.disp()\n");
  r.disp();

  CC.DisplayResult(r,script);
  return(0);
}

/////////////////////////////////////////////////////////////////////////
unsigned int FileSize(const char* fname) {
#if 0
  FileStrCollection fsc(fname);
  return(fsc.size());
#else
  return(0);
#endif
}

/////////////////////////////////////////////////////////////////////////
int main(int argc,char **argv) {
#ifdef __CINT__
  G__optimizemode(3);
#endif
  //cintgraph_start();
  Scandir d;
  char *fname;
  string script;
  for(int i=1;i<argc;i++) {
    if(strcmp(argv[i],"-s")==0) script = argv[++i];
    else {
      d.scan(argv[i]);
      while((fname=d.Next())&&fname[0]) {
	CC.clear();
	printf("%s\n",fname);
	classify(fname,script.c_str());
      }
    }
  }
#ifdef __CINT__
  G__pause();
#endif
  cintgraph_stop();
  return(0);
}
/////////////////////////////////////////////////////////////////////////

#if 0
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
icsv :  intelligent CSV handler experimental prototype
Demonstration

/////////////////////////////////////////////////////////////////
 $ cd DesignConsideration/Measurement
 $ cint _icsv.cxx EUT1/SettlingRoomTempEUT1_NoGd_EUT1_Ch1_Vset0_Vgd0.csv 
 $ cint _icsv.cxx EUT1/SettlingRoomTempEUT1_NoGd_EUT1_Ch1_Vset100_Vgd0.csv 
 $ cint _icsv.cxx EUT_Comparison/VFIMSettling_NoGD_No1_EUT4_Ch7_Vset100_Vgd-100_Vc-100.csv
 $ cint _icsv.cxx EUT_Comparison/VFIMSettling_NoGD_No1_EUT4_Ch7_Vset0_Vgd0_Vc0.csv 

/////////////////////////////////////////////////////////////////
 $ cd cint/lib/NeuralNet/testGraph/NNInput20x20
 $ cint _icsv.cxx BJT_VcIb_Ic-3.csv 
 $ cint _icsv.cxx BJT_Vb_Ib-1.csv
 $ cint _icsv.cxx X_Log4Dec_2.csv	

/////////////////////////////////////////////////////////////////
 $ cd DesignConsideration/000_MARS_Experiment/20140529_TohokuUniv_MTJ_Test_Results
 $ cint _icsv.cxx summary3_reanal_sort.csv    table x 8
 $ cint _icsv.cxx summary3_reanal_cluster.csv table
 $ cint _icsv.cxx summary.csv     table?
 $ cint _icsv.cxx summary2.csv    long table
 $ cint _icsv.cxx summary3.csv    long table

 $ cint _icsv.cxx csim.csv        3Dx1   asym
 $ cint _icsv.cxx cpostproc.csv   3Dx10  sym

 $ cint _icsv.cxx SortDistanceCicd.csv  3Dx1 XYx1 sym 
 $ cint _icsv.cxx SortDistanceCsim.csv  3dx1 XYx1 asym

 $ cint _icsv.cxx NNClustering.csv          3Dx2 XYx?
 $ cint _icsv.cxx NNClustering8_8_3.csv          3Dx2 XYx?
 $ cint _icsv.cxx NNClusteringSorted.csv    ???
 $ cint _icsv.cxx NNClusteringConflict.csv  3Dx1 XYx1


/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
#endif
