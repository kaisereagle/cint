////////////////////////////////////////////////////////////////////
// include/_fileline.cxx
//  The Incremental Clustering post-processing example
//  lines in files 
////////////////////////////////////////////////////////////////////
#ifdef __CINT__ // C++ interpreter
#include "IncClustering4.dll"
#include <CSV.h>
#else // C++ commpiler, Visual C++, g++, etc...
#include "IC_string.h"
#include "IC_DataSet4.h"
#include "EasyExp.h" // for using Scandir
#include "ReadF.h" // for using Scandir
#endif

#include <vector>
#include <string>
using namespace std;

string objectFile="tmp_IC_Object_filevec.csv";
string oversizeStorage ="tmp_oversizeStorage_filevec.csv";

////////////////////////////////////////////////////////////////////
// Clustering
////////////////////////////////////////////////////////////////////
int totalline=0;
void LineClustering(IC_DataSetS& dataset,const string& fname) {
  char id[1000];
  ReadF f(fname.c_str());
  while(f.read()) {
    sprintf(id,"%s_%d",fname.c_str(),f.line);
    dataset.add(id,f.argv[0]);
    if(++totalline%1000==0) printf("%d ",totalline);
  }
}

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
void LineClustering(const vector<string>& name,const vector<string>& fn
			,IC_REAL simThresh,int howManyClusters) {
  printf("Data accumulation process\n");
  IC_DataSetS dataset;  

  // restore previous session if exists
  // if no previous session, set parameters
  dataset.setName("ICfileline");
  dataset.setVersion(1);
  dataset.setSimfunc((void*)wordmatchratio,IC_SIMFUNC_THREADSAFE);
  //dataset.setSimfunc((void*)stringmatchratio,IC_SIMFUNC_THREADSAFE);
  dataset.setRepresentativeDataSizeLimit(1000); 
  dataset.setRepresentativeDataSize(500); 
  dataset.setOnMemorySizeLimit(100000); 
  dataset.setKeepOriginalData(true);
  dataset.setOversizeStorageFilename("");

  // accumulate data 
  string label, id;
  string data;
  for(unsigned int i=0;i<name.size();i++) {
    printf("%s ",name[i].c_str());
    LineClustering(dataset,name[i]);
  }
  printf("\n");

  printf("clustering\n");
  if(simThresh!=0)
    dataset.clustering(simThresh,IC_AVERAGE); 
  else
    dataset.clustering(howManyClusters,howManyClusters*2,IC_AVERAGE);

  Collection c,t,u;
  c.setkey("cls","id","line");
  Line l;
  int cls;
  for(unsigned int i=0;i<dataset.size();i++) {
    id   = dataset.getDataName(i);
    data = dataset.getData(i);
    cls = dataset.getDataCluster(i);
    l.let(0,cls);
    l.let(1,id);
    l.let(2,data);
    c.push_back(l);
  }

  c.multisort(0,1);

  printf("easyexp synthesis...\n");
  string re;
  for(cls=0;cls<dataset.getNcls();cls++) {
    printf("%d/%d ",cls,dataset.getNcls());
    EachClusterEasyExpSynthesis(dataset,re,cls,EasyExpMode,ListLimit);
    l.let(0,cls);
    l.let(1,dataset.getClusterSize(cls));
    l.let(2,re);
    t.push_back(l);
  }

  G__ateval(t);
  //G__ateval(c);
  
  printf(" c:cls,name,vec  t:cls,name\n");
  while(!G__pause());

  printf("nsample=%d, on-memory=%d  ncls=%d.  " 
	 ,dataset.nsample(),dataset.size(),dataset.getNcls()); 
  printf("Run 'filevecpostprocess <-o [outfile]>' for postprocessing and output\n");
}

///////////////////////////////////////////////////////////////////////////////
void EachClusterEasyExpSynthesis(IC_DataSetS& strset,string& re
			  ,unsigned int cls ,int mode=0,int LIST_LIMIT=20) {
  vector<string>  scluster = strset.getClusterRepresentativeData(cls);
  vector<string> _scluster = strset._getClusterRepresentativeData(cls);
  if(scluster.size()<20) { // if representative samples are too few, add others
    vector<string> allsamples = strset.getClusterData(cls);
    if(allsamples.size()<200) scluster=allsamples;
    else for(int i=0;i<50;i++) scluster.push_back(allsamples[rand()%allsamples.size()]);
  }
  re=EasyExpSynthesis(scluster,_scluster,mode,LIST_LIMIT);
}

int EasyExpMode=0;
int ListLimit=50;
////////////////////////////////////////////////////////////////////
int main(int argc,char **argv) {
  int howManyClusters=20;
  IC_REAL simThresh=(IC_REAL)0.0;  // similarity threshold 0.0 - 1.0
  string pathExpr="*"; // all files under incremental clustering lib
  for(int i=1;i<argc;i++) {
    if(strncmp(argv[i],"-thresh",2)==0) simThresh=(IC_REAL)atof(argv[++i]);
    else if(strncmp(argv[i],"-clusters",4)==0) howManyClusters=atoi(argv[++i]);
    else if(strncmp(argv[i],"-path",3)==0) pathExpr=argv[++i];
    else if(strncmp(argv[i],"-g",2)==0) ++IC_DEBUG;
    else if(strncmp(argv[i],"-clean",4)==0) {
      remove(objectFile.c_str()); 
      remove(oversizeStorage.c_str()); 
    }
    else if(strcmp(argv[i],"-?")==0) {
      printf("filevec incremental clustering example : Usage\n");
      printf(" filevec <options> \n");
      printf("    -path [fileexpr]    : file names to be listed\n");
      printf("    -thresh [simThresh] : similarity threshold 0.0 - 1.0 default=0.5\n");
      printf("    -clusters [K]       : How many clusters. Default=0 (Auto)\n");
      printf("    -clean              : clean-up data accumulation\n");
      return(255);
    }
    else {
      pathExpr=argv[i];
    }
  }

  // Data preparation
  vector<string> fname,fullpath;
  Scandir d(pathExpr);     // scan files by expression
  d.getfname(fname); // store filename in a vector
  d.getfullpath(fullpath); // store fullpath in a vector
  // Data accumulation and clustering
  LineClustering(fullpath,fname,simThresh,howManyClusters);
  return(0);
}

////////////////////////////////////////////////////////////////////

