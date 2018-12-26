/*****************************************************************************
 * _filevec.cxx
 *  Description: 
 *    Vectorize line/word/char count as vector, do clustering and regular expression
 *   synthesis on file name.
 *  Usage:
 *    cint _filevec.cxx <option> [fileExpr] 
 *  Status: Experimental
 *  Date: 2015/Dec/3
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
#ifdef __CINT__ // C++ interpreter
#include "IncClustering4.dll"
#include <CSV.h>
#else // C++ commpiler, Visual C++, g++, etc...
#include "IC_vectorfloat.h"
#include "IC_DataSet4.h"
#include "EasyExp.h" // for using Scandir
#include "ReadF.h" // for using Scandir
#endif

#include <vector>
#include <string>
using namespace std;

int caseindependent=1;
int isdirectory=2;

vector<string> fname,fullpath;
#ifdef IC_DATASET_H
IC_DataSetFV dataset;  
int RepresentativeData=200;
int RepresentativeDataLimit=400;
FVectorSimilarity simfunc= NormalizedInnerProduct;
//FVectorsimfunc= NormalizedEuclideanDistance;
#endif

int regexp=0;
int ListLimit = 50;
int EasyExpMode = 0;

int prompt=0;

Collection c,t; // output collection
Collection c1,c2,t1,t2,u; // aux buffer
int    loadflag=0;

////////////////////////////////////////////////////////////////////
#if 0 // this function is included in IncClustering4.dll for speed up
void file2vec(const string& fname,const string& fn,vector<float>& v) {
  Stat<float> wordinline,charinline, charinword;
  ReadF f(fname.c_str());
  int i;
  while(f.read()) {
    wordinline.add(f.argc);
    charinline.add(strlen(f.argv[0]));
    for(i=1;i<f.argc;i++) charinword.add(strlen(f.argv[i]));
  }
  v.clear();

  f.setseparator(" .-_()[]");
  f.parse(fn.c_str());
  v.push_back(fn.size());
  v.push_back(f.argc);

  v.push_back(wordinline.nsample());
  v.push_back(wordinline.mean());
  v.push_back(wordinline.stddev());
  v.push_back(wordinline.min());
  v.push_back(wordinline.max());

  v.push_back(charinline.nsample());
  v.push_back(charinline.mean());
  v.push_back(charinline.stddev());
  v.push_back(charinline.min());
  v.push_back(charinline.max());

  v.push_back(charinword.nsample());
  v.push_back(charinword.mean());
  v.push_back(charinword.stddev());
  v.push_back(charinword.min());
  v.push_back(charinword.max());
}
#endif

////////////////////////////////////////////////////////////////////
// Clustering
////////////////////////////////////////////////////////////////////
void clustering(double criteria) {
  int CLST=0;
  NN_REAL thresh=1.0;
  if(criteria==1.0) { }
  if(criteria>1.0) { CLST=(int)criteria; thresh=1.0; }
  else {CLST=0; thresh=criteria; }

  printf("criteria=%g thresh=%g CLST=%d\n",criteria,thresh,CLST);

  // update RepresentativeData 
  if(loadflag!=0 && criteria<1.0 && dataset.getHierClusteringThresh(1)<thresh) {
    RepresentativeData *=2;
    RepresentativeDataLimit *=2;
    loadflag=0;
  }
  else if(criteria>1.0 && CLST>RepresentativeData) {
    RepresentativeData = CLST*2;
    RepresentativeDataLimit = CLST*4;
    loadflag=0;
  }
  
  string label, id;
  vector<float> data;

  if(loadflag==0) {
    ++loadflag;
    printf("Data accumulation process\n");
    //IC_DataSetFV dataset;  
    
    // restore previous session if exists
    // if no previous session, set parameters
    dataset.clear();
    dataset.setName("ICfilevec");
    dataset.setVersion(1);
    dataset.setSimfunc((void*)simfunc,IC_SIMFUNC_THREADSAFE);
    dataset.setRepresentativeDataSizeLimit(RepresentativeData); 
    dataset.setRepresentativeDataSize(RepresentativeDataLimit); 
    dataset.setOnMemorySizeLimit(100000); 
    dataset.setKeepOriginalData(true);
    dataset.setOversizeStorageFilename("");
    
    // accumulate data 
    for(unsigned int i=0;i<fname.size();i++) {
      id = fullpath[i] ;
      file2vec(fullpath[i],fname[i],data);
      dataset.add(id,data,label);
      if((dataset.size()%1000)==0){
	printf("%d ",dataset.size()); fflush(stdout); 
      }
    }
    if(dataset.size()>1000) printf("\n");
  }

  // final clustering
  if(thresh<1.0) 
    dataset.clustering(thresh,IC_AVERAGE); 
  else
    dataset.clustering(CLST,CLST*2,IC_AVERAGE);

  // output result to c,t table
  //Collection c,t,u;
  c.clear(); t.clear(); 
  c.setkey("cls","name","Ncharinfname","Nwordinfname"
	   ,"Nwordinline","mean","stddev","min","max"
	   ,"Ncharinline","mean","stddev","min","max"
	   ,"Ncharinword","mean","stddev","min","max"
	   );
  Line l;
  int cls;
  for(unsigned int i=0;i<dataset.size();i++) {
    id   = dataset.getDataName(i);
    data = dataset.getData(i);
    cls = dataset.getDataCluster(i);
    l.let(0,cls);
    l.let(1,id);
    for(unsigned int j=0;j<data.size();j++) l.let(2+j,data[j]);
    c.push_back(l);
  }


  t = c.column(0,1);
  t.multisort(0,1);
  c.multisort(0,1);

  if(regexp) t.EasyExpSynthesis(1,2,0,EasyExpMode,ListLimit);

#ifdef __CINT__
  if(prompt) G__ateval(t);
  else t.disp();
#else
  t.disp();
#endif
}

////////////////////////////////////////////////////////////////////
void Analyze(double criteria) {
  clustering(criteria);

#ifdef __CINT__
  if(prompt) {
    if(criteria==1.0) G__ateval(c);
    printf(" c:list of files, t:clsutering and synthesized regexp ");
    while(!G__pause());
  }
  else if(criteria==1.0) c.disp();
#else
  if(criteria==1.0) c.disp();
#endif
}

////////////////////////////////////////////////////////////////////
int main(int argc,char **argv) {
  double criteria=0.9;
  string pathExpr="*"; // all files under current directory
  for(int i=1;i<argc;i++) {

    if(strncmp(argv[i],"-path",3)==0) pathExpr=argv[++i];

    //else if(strcmp(argv[i],"-f")==0) isdirectory=0;
    //else if(strcmp(argv[i],"-d")==0) isdirectory=1;
    //else if(strcmp(argv[i],"-b")==0) isdirectory=2; // default

    else if(strcmp(argv[i],"-c")==0) caseindependent=0; // defatul
    else if(strcmp(argv[i],"-i")==0) caseindependent=1;

    // clustering
    else if(strcmp(argv[i],"-C")==0) criteria=atoi(argv[++i]);
    else if(strcmp(argv[i],"-t")==0)  criteria=atof(argv[++i]);

    else if(strcmp(argv[i],"-m")==0) { 
      RepresentativeData=atoi(argv[++i]);
      RepresentativeDataLimit = RepresentativeData*2;
    }

    // EasyExp synthesis
    else if(strcmp(argv[i],"-r")==0) { regexp++; if(1.0==criteria) criteria=0.7; }
    else if(strcmp(argv[i],"-mode")==0) EasyExpMode=atoi(argv[++i]);
    else if(strcmp(argv[i],"-l")==0) ListLimit=atoi(argv[++i]); 

    // prompt
    else if(strcmp(argv[i],"-p")==0) prompt=1;
    else if(strcmp(argv[i],"-v")==0) prompt=1;
    else if(strcmp(argv[i],"-e")==0) prompt=0;

    // debug
    else if(strncmp(argv[i],"-g",2)==0) ++IC_DEBUG;

    // help
    else if(strcmp(argv[i],"-?")==0) {
      printf("Usage: cint _find.cxx <option> [fileExpr] <[fileExpr2]. <...>>\n");
      printf("  -b          : list file and directory (default)\n");

      printf("  -c          : case sensitive\n");
      printf("  -i          : case insensitive (default)\n");

      printf("  -C [N]      : number of clusters\n");
      printf("  -t [thresh] : clustering threshold\n");

      printf("    -m [Nrep] : Set number of representative data cluster\n");

      printf("  -r          : activate regular expression synthesis\n");
      printf("    -mode [mode]   : Set regexp synthesis mode [0-2]\n");
      printf("    -l [ListLimit] : Set regexp synthesis string list limit\n");

      printf("  -p          : Prompt interaction\n");

      exit(0);
    }
    else {
      pathExpr=argv[i];
    }
  }

  // Data preparation
  //vector<string> fname,fullpath;
  Scandir d(pathExpr,isdirectory,caseindependent);     // scan files by expression
  d.getfname(fname); // store filename in a vector
  d.getfullpath(fullpath); // store fullpath in a vector

  // Data accumulation and clustering
  Analyze(criteria);
  return(0);
}

////////////////////////////////////////////////////////////////////

