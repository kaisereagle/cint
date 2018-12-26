/****************************************************************************
* NNclassify.cxx
*
*  NeuralNet Data Evaluation.
*   Use pre-trained NeuralNet weight objects to classify target data samples.
*
****************************************************************************/
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
/****************************************************************************
*
* Input
*   DataSet   <>---* Data 
*   NNs       <>---* LearntNet <>---1..5 Label
*
* Intermediate Output
*   Result <>--* DataSet <>--* Data --* probability 1--1 Label 1..5--LearntNet
*
*     nn ,cls,shift,label,  D0,   D1,   D2,  ...   ,  Dn
*     NN0, 0 ,  0  , lbl1, 
*     NN0, 1 ,  0  ,     ,      0 〜 1 の確率
*     NN0, 2 ,  0  ,     ,
*     NN1, 0 ,  0  ,     ,
*     NN1, 1 ,  0  ,     ,
*     NN1, 0 ,  1  ,     ,
*     NN1, 1 ,  1  ,     ,
*     NN1, 0 ,  1  ,     ,
*     NN1, 1 ,  1  ,     ,
*
* Final Output (from cint __NNcombine.cxx)
* 
*     Data,label1,shift1,prob1,label2,shift2,prob2,label3,shift3,prob3,
*     D0,
*     D1,    label,shift,probability triples
*     D2,
*      .
*      .
*     Dn,
* 
****************************************************************************
* Usage:
*  $ NNclassify -T "[DataFileExpr].csv" -NN "[NNfileExpr].csv" 
* command option
*    -T    DataSetExpr.csv
*    -NN   TrainedNetExpr.nnw
*    -o    OutputFile.csv  
*    -conv convolutionalShift
*
****************************************************************************
* 1. Read DataSet 
* 2. for each LearntNet
*      read LearntNet
*      for convolutional shift
*         Evaluate DataSet 
*         Select the largest probability
*      end
*
****************************************************************************/


#ifdef __CINT__
#include <NeuralNet.h>
#else
#include "MachineLearningElementNN.h"
#include "NeuralNetClustering.h"
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <string>
#include <map>
using namespace std;

///////////////////////////////////////////////////////////////////////
int xNN=0,xcls=1,xshift=2,xlabel=3,xdata0=4;

///////////////////////////////////////////////////////////////////////
//     NN ,class,label,  D0,   D1,   D2,  ...   ,  Dn
int OutputHeader(const string& ofile,const NeuralNetDataSet& d) {
  FILE *fp = fopen(ofile.c_str(),"w");
  if(!fp) {
    fprintf(stderr,"Error: NNclassify, Failed creating %s\n",ofile.c_str());
    return(255);
  }
  fprintf(fp,"NN,class,shift,label,");

  int i;
  for(i=0;i<d.size();i++) {
    const LeafBase& leaf = d[i];
    if(leaf.name!="") fprintf(fp,"%s,",leaf.name.c_str());
    else              fprintf(fp,"$%d,",i);
  }
  fprintf(fp,"\n");

  fclose(fp);
  return(0);
}

///////////////////////////////////////////////////////////////////////
// Data Matrix shifting for image Convolution
///////////////////////////////////////////////////////////////////////
NN_REAL& DataRef(vector<NN_REAL>& v,int x,int y,int xshift,int yshift
		 ,int nline) {
  static NN_REAL dummy;
  dummy = 0.0;
  int xn = x+xshift, yn = y+yshift;
  int index = xn + yn*nline;
  if(xn<0 || nline<=xn || yn<0 || index>=v.size()) return(dummy);
  else return(v[index]);
}
///////////////////////////////////////////////////////////////////////
NN_REAL GetData(const vector<NN_REAL>& v,int x,int y,int xshift,int yshift
		,int nline) {
  int xn = x+xshift, yn = y+yshift;
  int index = xn + yn*nline;
  if(xn<0 || nline<=xn || yn<0 || index>=v.size()) return(0.0);
  else return(v[index]);
}

///////////////////////////////////////////////////////////////////////
LeafBase ConvShift(int xshift,int yshift,int nline,const LeafBase& leafin
		   ,int Noutput) {
  LeafBase leafout;
  leafout.x.resize(leafin.Ninput());
  leafout.t.resize(Noutput);
  leafout.y.resize(Noutput);
  leafout.c.resize(Noutput);
  int x,y, ymax = leafin.x.size()/nline;
  for(x=0;x<nline;x++) {
    for(y=0;y<ymax;y++) {
      DataRef(leafout.x,x,y,0,0,nline) 
	= GetData(leafin.x,x,y,xshift,yshift,nline);
    }
  }
  return(leafout);
}

///////////////////////////////////////////////////////////////////////
  NeuralNetDataSet ConvShift(int xshift,int yshift,int nline
			     ,const NeuralNetDataSet& din,int Noutput) {
  NeuralNetDataSet dout = din;
  for(int i=0;i<dout.size();i++) {
    dout[i] = ConvShift(xshift,yshift,nline,dout[i],Noutput);
  }
  return(dout);
}

///////////////////////////////////////////////////////////////////////
// Evaluate data set with learnt neural network weight parameters
///////////////////////////////////////////////////////////////////////
int EvaluateDataSet(const string& ofile,NeuralNet& net
		    ,NeuralNetDataSet& d,int convshift) {
  int xshift,yshift,i,j;
  FILE *fp = fopen(ofile.c_str(),"a");
  if(!fp) {
    fprintf(stderr,"Error: EvaluateDataSet(), Failed opening %s\n",ofile.c_str());
    return(255);
  }

  // emulation of simplified convolutional neural network
  for(xshift = -convshift;xshift<=convshift;++xshift) {
    for(yshift = -convshift;yshift<=convshift;++yshift) {
      NeuralNetDataSet dwork = ConvShift(xshift,yshift,d.m_nline,d,net.Noutput());
      printf("shift %d,%d\n",xshift,yshift);
      //dwork.disp();
#if 1
      NN_REAL err,beta;
      int fail,marginal,lowconfidence;
      dwork.UpdateOutput_Evaluate(net,err,fail,marginal,lowconfidence,beta);
      //printf("err=%g fail=%d margilan=%d lowconf=%d\n",err,fail,marginal,lowconfidence);
#else
      NN_REAL *o = net.getOutput();
      for(i=0;i<dwork.size();i++) {
	LeafBase& leaf = dwork[i];
	net.propagation(&(leaf.x[0]));
	leaf.setOutput(o,net.Noutput());
      }
#endif
      double shift = sqrt(double(xshift*xshift+yshift*yshift));
      for(j=0;j<dwork.Noutput();j++) {
	fprintf(fp,"%s,%d,%g,%s,",net.m_name.c_str()
		,j,shift ,net.m_label[j].c_str());
	for(i=0;i<dwork.size();i++) {
	  LeafBase& leaf = dwork[i];
	  fprintf(fp,"%g,",leaf.y[j]);
	}
	fprintf(fp,"\n");
      }
    }
  }

  fclose(fp);
  return(0);
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
int main(int argc,char** argv) {
#ifdef __CINT__
  G__optimizemode(3);
#endif
  DEBUG=0; //defined in  NeuralNet library
  int nline=40; // number of chars in line for NeuralNetDataSet::disp

  vector<string> DataSetExpr;
  vector<string> LearntNetExpr;
  string         ofile, tmpfile;
  int            convshift=0;

  ///////////////////////////////////////////////////////////////
  // command line option processing
  int i;
  for(i=1;i<argc;i++) {
    if(     strcmp(argv[i],"-T")==0) DataSetExpr.push_back(argv[++i]);
    else if(strcmp(argv[i],"-NN")==0) LearntNetExpr.push_back(argv[++i]);
    else if(strcmp(argv[i],"-o")==0) ofile=argv[++i];
    else if(strcmp(argv[i],"-conv")==0) convshift = atoi(argv[++i]);
  }

  // verify setting
  if(argc<=1) {
    fprintf(stderr,"Usage: NeuralNetClassify <options>\n");
    fprintf(stderr,"    -T [DataSetExpr].csv\n");
    fprintf(stderr,"    -NN [LearntNetExpr].nnw\n");
    fprintf(stderr,"    -o  [Output].csv\n");
    fprintf(stderr,"    -conv [convolutionalShift]\n");
    exit(255);
  }
  int err=0;
  if(DataSetExpr.size()==0) {
    fprintf(stderr,"Error: need '-T [DataSetExpr].csv' for giving input data\n");
    ++err;
  }
  if(LearntNetExpr.size()==0) {
    fprintf(stderr,"Error: need '-NN [NetWeight].nnw' for giving NeuralNet weight\n");
    ++err;
  }
  if(ofile.size()==0) {
    fprintf(stderr,"Error: need '-o [Output].csv' for giving output file name\n");
    ++err;
  }
  else {
    tmpfile = string("_tmp_") + ofile;
  }
  if(err) exit(255);


  ////////////////////////////////////////////////////////////////////////
  NeuralNetDataSet d;
  NeuralNet        net;

  ////////////////////////////////////////////////////////////////////////
  for(i=0;i<DataSetExpr.size();i++) {
    printf("Reading Training Data from %s\n",DataSetExpr[i].c_str());
    // If provided, cls and label are read and set to Dataset and NN
    ::readNNDataFiles(DataSetExpr[i].c_str(),d);
  }
  printf("d.size=%d\n",d.size());

  ////////////////////////////////////////////////////////////////////////
  // output acolumn header
  if(OutputHeader(tmpfile,d)) return(255);

  ////////////////////////////////////////////////////////////////////////
  // Set NeuralNet initialization parameters
  unsigned int InputUnitNo = d.Ninput();
  unsigned int OutputUnitNo = d.Noutput();
  unsigned int HiddenUnitNo = 16;
  vector<int> Nnode;
  vector<NN_REAL> sigma;
  Nnode.push_back(InputUnitNo); sigma.push_back((NN_REAL)1.0); // input
  Nnode.push_back(InputUnitNo/2); sigma.push_back((NN_REAL)0.1); // hidden 1,L1
  Nnode.push_back(HiddenUnitNo); sigma.push_back((NN_REAL)0.1); // hidden 2, L2
  Nnode.push_back(HiddenUnitNo); sigma.push_back((NN_REAL)(NN_REAL)0.1); //  L3
  Nnode.push_back(OutputUnitNo); sigma.push_back((NN_REAL)0.1); // output,  L4
  NN_REAL Lambda[4] = { 0,0,0,0 }; // 0 is fine. No normalization at NN

  ////////////////////////////////////////////////////////////////////////
  // scan over learnt neural net weight files
  int stat;
  for(i=0;i<LearntNetExpr.size();i++) {
    //printf("Reading LearntNet wight from %s\n",LearntNetExpr[i].c_str());
    Scandir dir(LearntNetExpr[i].c_str());
    const char* fname;
    while((fname=dir.Next())&&fname[0]) {
      // initialize Neural Network
      NeuralNet net;
      net.init(Nnode,sigma);
      net.setLambda(Lambda[0],Lambda[1],Lambda[2],Lambda[3]);
      printf("Loading LearntNet wight from %s\n",fname);
      stat += net.loadbin(fname); // load main neural net

      // evaluate data sample with each NN weight and append result in tmpfile
      EvaluateDataSet(tmpfile,net,d,convshift);
    }
  }

  ////////////////////////////////////////////////////////////////////////
  // postprocess ofile with _NNcombine.cxx
  string postprocess = "cint _NNcombine.cxx ";
  postprocess += tmpfile; 
  postprocess += " > "; 
  postprocess += ofile; 
  system(postprocess.c_str());

  return(0);
}
///////////////////////////////////////////////////////////////////////


