/*****************************************************************************
 * _find.cxx
 *  Description: 
 *    Find files with EasyExp scanning
 *    Classificadtion with Incremental Clustering (new lib, fast, no size limit)
 *    Summarize result with EasyExp synthesis (-r) 
 *    Number of cluster (-C [n]) or similarity threshold (-t {thresh]) 
 *     has to be given
 *  Usage:
 *    cint _find.cxx <option> [fileExpr] <[fileExpr2] <...>>
 *  Status: Production
 *  Date: 2015/Aug/3
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

#ifdef __CINT__
#pragma include_noerr <IncClustering4.dll> // optional component
#include <CSV.h>
#include <exception>
#else
#include "IC_string.h"
#include "IC_DataSet.h"
#include "Collection.h"
#include "EasyExp.h"
#endif

/////////////////////////////////////////////////////////////////////////
// Parameters
/////////////////////////////////////////////////////////////////////////
int caseindependent=1;
int isdirectory=2;
int clusteringByFilename=0;

#ifdef IC_DATASET_H
IC_DataSetS strset;
int RepresentativeData=200;
int RepresentativeDataLimit=400;
FStringSimilarity simfunc= stringmatchratio;
//FStringSimilarity simfunc= charmatchratio;
//FStringSimilarity simfunc= wordmatchratio;
//FStringSimilarity simfunc= headmatchratio;
#endif

int regexp=0;
int ListLimit = 50;
int EasyExpMode = 0;

int prompt=0;

Collection c,t; // output collection
Collection c1,c2,t1,t2,u; // aux buffer
int    loadflag=0;

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

/////////////////////////////////////////////////////////////////////////
void FindFiles(const char* fexpr) {
  // Multi thread settings
#ifdef N
  IC_num_threads=N;
  EasyExp_num_threads=N;
#endif
#ifdef LIM
  IC_num_threads_limit=LIM;
  EasyExp_num_threads_limit=LIM;
#endif
#ifdef DEBUG
  g_DEBUG=1;
  EasyExp_DEBUG=1;
#endif

  //printf("reading...\n");
  Scandir d(fexpr,isdirectory,caseindependent);
  char* fname;
  Line l;
  int i;

  if(clusteringByFilename) {
    vector<string> path; d.getpath(path);
    vector<string> fnameonly; d.getfname(fnameonly);
    c.setkey("path","fname");
    for(i=0;i<fnameonly.size();i++) {
      l.let(0,path[i]);
      l.let(1,fnameonly[i]);
      c.push_back(l);
    }
  }
  else {
    c.setkey("fname");
    vector<string> fullpath; d.getfullpath(fullpath);
    c.setkey("fname");
    for(i=0;i<fullpath.size();i++) {
      l.let(0,fullpath[i]);
      c.push_back(l);
    }
  }
}

/////////////////////////////////////////////////////////////////////////
void clustering(NN_REAL criteria=1.0) {
  int CLST=0;
  NN_REAL thresh=1.0;
  if(criteria==1.0) { }
  if(criteria>1.0) { CLST=(int)criteria; thresh=1.0; }
  else {CLST=0; thresh=criteria; }
  Line l;
  printf("total = %d\n",c.size());

  // initialize incremental clustering object
  unsigned int i,cls, xregexp=3;
  string id,data;

  // update RepresentativeData 
  if(loadflag!=0 && criteria<1.0 && strset.getHierClusteringThresh(1)<thresh) {
    RepresentativeData *=2;
    RepresentativeDataLimit *=2;
    loadflag=0;
  }
  else if(criteria>1.0 && CLST>RepresentativeData) {
    RepresentativeData = CLST*2;
    RepresentativeDataLimit = CLST*4;
    loadflag=0;
  }
  
  // re-load sample data  (only necessary)
  if(loadflag==0) {
    ++loadflag;
    strset.clear();
    strset.setKeepOriginalData(true);
    strset.setRepresentativeDataSize(RepresentativeData);
    strset.setRepresentativeDataSizeLimit(RepresentativeDataLimit);
    strset.setSimfunc(simfunc);
    // incremental clustering
    if(clusteringByFilename) {
      for(i=0;i<c.size();i++) {
	id = c[i][0].tostring() + "/" + c[i][1].tostring();
	data = c[i][1].tostring();
	strset.add(id,data);
	if((strset.size()%1000)==0){
	  printf("%d ",strset.size()); fflush(stdout); 
	}
      }
    }
    else {
      for(i=0;i<c.size();i++) {
	id=data=c[i][0].tostring();
	strset.add(id,data);
	if((strset.size()%1000)==0){
	  printf("%d ",strset.size()); fflush(stdout); 
	}
      }
    }
    if(strset.size()>1000) printf("\n");
  }
  
  // final clustering
  if(thresh<1.0) // clsutering by similarity threshold
    strset.clustering(thresh,IC_AVERAGE);
  else           // clustering by number of clusters
    strset.clustering(CLST,CLST*2,IC_AVERAGE);

  // output result to t table
  t.clear();
  if(clusteringByFilename) {
    vector<pair<string,string> > vps;
    t.setkey("cls","fname","path");
    for(cls=0;cls<strset.getNcls();cls++) {
      vps = strset.getClusterDataPair(cls);
      for(i=0;i<vps.size();i++) {
	l.let(0,cls);
	l.let(2,vps[i].first);
	l.let(1,vps[i].second);
	t.push_back(l);
      }
    }
    xregexp=3;
  }
  else {
    vector<string> vs;
    t.setkey("cls","fname");
    for(cls=0;cls<strset.getNcls();cls++) {
      vs = strset.getClusterData(cls);
      for(i=0;i<vs.size();i++) {
	l.let(0,cls);
	l.let(1,vs[i]);
	t.push_back(l);
      }
    }
    xregexp=2;
  }

  if(regexp) {
    printf("Ncls=%d regexp synthesis...",strset.getNcls());
    Collection tmp;
    string re;
    t.getkey().push_back("regexp");
    for(cls=0;cls<strset.getNcls();cls++) {
      EachClusterEasyExpSynthesis(strset,re,cls,EasyExpMode,ListLimit);
      tmp = t.match(0,cls);
      if(tmp.size()) tmp[0].let(xregexp,re);
    }
  }

#ifdef __CINT__
  if(prompt) G__ateval(t);
  else t.disp();
#else
  t.disp();
#endif

}


///////////////////////////////////////////////////////////////////////////////
void Analyze(double criteria) {
  if(criteria!=1.0) clustering(criteria);

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

///////////////////////////////////////////////////////////////////////////////

int main(int argc,char** argv) {
  double criteria=1.0;
  for(int i=1;i<argc;i++) {
    // directory scan
    if(strcmp(argv[i],"-f")==0) isdirectory=0;
    else if(strcmp(argv[i],"-d")==0) isdirectory=1;
    else if(strcmp(argv[i],"-b")==0) isdirectory=2; // default

    else if(strcmp(argv[i],"-c")==0) caseindependent=0; // defatul
    else if(strcmp(argv[i],"-i")==0) caseindependent=1;

    // clustering
    else if(strcmp(argv[i],"-C")==0) criteria=atoi(argv[++i]);
    else if(strcmp(argv[i],"-t")==0)  criteria=atof(argv[++i]);

#ifdef IC_DATASET_H
    else if(strcmp(argv[i],"-string")==0) simfunc=stringmatchratio; // default
    else if(strcmp(argv[i],"-char")==0) simfunc=charmatchratio;
    else if(strcmp(argv[i],"-word")==0) simfunc=wordmatchratio;
    else if(strcmp(argv[i],"-head")==0) simfunc=headmatchratio;
#endif

    else if(strcmp(argv[i],"-m")==0) { 
      RepresentativeData=atoi(argv[++i]);
      RepresentativeDataLimit = RepresentativeData*2;
    }

    else if(strcmp(argv[i],"-F")==0) clusteringByFilename=1;

    // EasyExp synthesis
    else if(strcmp(argv[i],"-r")==0) { regexp++; if(1.0==criteria) criteria=0.7; }
    else if(strcmp(argv[i],"-mode")==0) EasyExpMode=atoi(argv[++i]);
    else if(strcmp(argv[i],"-l")==0) ListLimit=atoi(argv[++i]); 

    // prompt
    else if(strcmp(argv[i],"-p")==0) prompt=1;
    else if(strcmp(argv[i],"-v")==0) prompt=1;
    else if(strcmp(argv[i],"-e")==0) prompt=0;

    // help
    else if(strcmp(argv[i],"-?")==0) {
      printf("Usage: cint _find.cxx <option> [fileExpr] <[fileExpr2]. <...>>\n");
      printf("  -f          : list file only\n");
      printf("  -d          : list directory only\n");
      printf("  -b          : list file and directory (default)\n");

      printf("  -c          : case sensitive\n");
      printf("  -i          : case insensitive (default)\n");

      printf("  -C [N]      : number of clusters\n");
      printf("  -t [thresh] : clustering threshold\n");
      printf("    -string   : string match mode (default)\n");
      printf("    -char     : character match mode\n");
      printf("    -word     : word match mode\n");
      printf("    -head     : head-string match mode\n");
      printf("    -m [Nrep] : Set number of representative data cluster\n");
      printf("    -F        : clustering by filename (not fullpath)\n");

      printf("  -r          : activate regular expression synthesis\n");
      printf("    -mode [mode]   : Set regexp synthesis mode [0-2]\n");
      printf("    -l [ListLimit] : Set regexp synthesis string list limit\n");

      printf("  -p          : Prompt interaction\n");

      exit(0);
    }
    else {
      FindFiles(argv[i]);
    }
  }

  Analyze(criteria);
  return(0);
}



