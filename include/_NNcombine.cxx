///////////////////////////////////////////////////////////////////////////////
// $CINTSYSDIR/include/_NNcombine.cxx
// $CINTSYSDIR/lib/NeuralNet/testCurve/NNcombine.cxx
//
//  Combine multiple neural network classifications
//
//  Called from NNclassify.cxx
//
// Author:  2015 Masaharu Goto,  gotom@hanno.jp
///////////////////////////////////////////////////////////////////////////////
// Refer doit.sh for Workflow
//
// Input table format
//    NN ,class,shift,label,  D0,   D1,   D2,  ...   ,  Dn
//    NN0, 0   ,  0  , lbl1, 
//    NN0, 1   ,  0  ,     ,      probability 0..1
//    NN0, 2   ,  0  ,     ,
//    NN1, 0   ,  0  ,     ,
//    NN1, 1   ,  0  ,     ,
//    NN1, 0   ,  1  ,     ,
//    NN1, 1   ,  1  ,     ,
//    NN1, 0   ,  1  ,     ,
//    NN1, 1   ,  1  ,     ,
//
// Output table format
//    name,label1,shift1,prob1,label2,shift2,prob2,label3,shift3,prob3,
//    D0,
//    D1,    label,shift,probability triples
//    D2,
//     .
//     .
//    Dn,
//
///////////////////////////////////////////////////////////////////////////////
// Usage:
//  $ cint _NNcombine.cxx [input].csv  > [output].csv
//
///////////////////////////////////////////////////////////////////////////////
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

#ifdef __CINT__
#include <CSV.h>
#else
#include "Collection.h"
#endif

void CombineClassification(const char* fname) {

  Sheet s(fname);
  Collection c=s.collection(),t;
  c.setkey(0);
  // get column header
  int xlabel=c.getx("label");
  int xshift=c.getx("shift");
  int xclass=c.getx("class");
  int xnn =c.getx("NN");
  int xstart = xlabel+1;
  
  EasyExp elow("*_X"); // signature for low priority classification
  int x,j1,j2,j3,j;
  double p1,p2;
#if 1
  double swapthresh=0.02, dispthresh=0.2, thresh=0.2; // with unified NN
#else
  double swapthresh=0.02, dispthresh=0.2, thresh=0.8; // with separated NN
#endif

  // output column header
  printf("name,label1,shift1,prob1,label2,shift2,prob2,label3,shift3,prob3,\n");
  
  for(x=xstart;x<c.getkey().size();x++) { // scan over input data samples
    t = c.sort(x);
    j1=-1; j2=-1,j3=-1;
    for(j=0;j<t.size();j++) {
      if(j1<0) {
	j1 = j; // set the highest probability label -> normally 0
      }
      else if(j2<0) {
	if((elow.match(t[j1][xlabel].c_str()) ||
	    t[j1][xshift].todouble()>t[j][xshift].todouble()) && 
	   t[j1][x].todouble()-t[j][x].todouble() < swapthresh) {
	  // if current choise is 'other' category and probability is close
	  // replace the highest probability label 
	  if(!elow.match(t[j][xlabel].c_str()) ||
	     t[j1][xlabel].tostring() != t[j][xlabel].tostring() ) {
	    j2 = j1;
	    j1 = j; 
	  }
	  else {
	  }
	}
	else {
	  j2 = j;
	}
      }
      else if(j3<0) {
	if((elow.match(t[j2][xlabel].c_str()) ||
	    t[j2][xshift].todouble()>t[j][xshift].todouble()) && 
	   t[j2][x].todouble()-t[j][x].todouble() < swapthresh) {
	  // if current choise is 'other' category and probability is close
	  // replace the highest probability label 
	  if(!elow.match(t[j][xlabel].c_str()) ||
	     t[j2][xlabel].tostring() != t[j][xlabel].tostring() ) {
	    j3 = j2;
	    j2 = j; 
	    break;
	  }
	  else {
	  }
	}
	else {
	  j3 = j;
	  break;
	}
      }
    }
    printf("%s," ,t.getkey()[x].c_str());

    if(t[j1][x].todouble()>thresh) {
      printf("%s,%g,%g,"
	     ,t[j1][xlabel].c_str()
	     ,t[j1][xshift].todouble()
	     ,t[j1][x].todouble());
      
      if((t[j1][x].todouble()-t[j2][x].todouble())<dispthresh) {
	printf("%s,%g,%g,"
	       ,t[j2][xlabel].c_str()
	       ,t[j2][xshift].todouble()
	       ,t[j2][x].todouble());
	
	if((t[j2][x].todouble()-t[j3][x].todouble())<dispthresh) 
	  printf("%s,%g,%g,"
		 ,t[j3][xlabel].c_str();
		 ,t[j3][xshift].todouble()
		 ,t[j3][x].todouble());
      }
    }
    
    printf("\n");

    //,t[0][xnn].c_str());
  }
  
}

int main(int argc,char** argv) {
#ifdef __CINT__
  G__optimizemode(3);
#endif
  for(int i=1;i<argc;i++) CombineClassification(argv[i]);
  return(0);
}

