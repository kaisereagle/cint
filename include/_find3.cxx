/*****************************************************************************
 * _find3.cxx
 *    Find files with EasyExp scanning
 *    Classificadtion with Incremental Clustering (new lib, fast, no size limit)
 *    Experimental creation of Supervised learning model by EasyExp synthesis
 *  Usage:
 *    cint _find.cxx <option> [fileExpr] <[fileExpr2] <...>>
 *  Status: Experimental
 *  Date: 2015/Aug/10
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

#pragma include <IncClustering4.dll>
#include <CSV.h>
#include <exception>

/////////////////////////////////////////////////////////////////////////
// Parameters
/////////////////////////////////////////////////////////////////////////
int RepresentativeData=200;
int RepresentativeDataLimit=400;
FStringSimilarity simfunc= charmatchratio;
//FStringSimilarity simfunc= wordmatchratio;
//FStringSimilarity simfunc= headmatchratio;
int regexp=0;

int CLST= 0;
int ListLimit = 20;
int EasyExpMode = 0;

int caseindependent=1;
int isdirectory=2;
int prompt=1;
int low=8, high=20;

///////////////////////////////////////////////////////////////////////////////
void getEachClusterRegExp(IC_DataSetS& strset,string& re
			  ,unsigned int cls ,int mode=0,int LIST_LIMIT=20) {
  vector<string> scluster,_scluster;
  scluster  =  strset.getClusterRepresentativeData(cls);
  if(scluster.size()<20) {
    _scluster = strset.getClusterDataName(cls);
    if(_scluster.size()<200) {
      //printf(" %d->%d ",cluster.size(),_cluster.size());
      scluster=_scluster;
    }
    else {
      //printf(" %d->%d->+50 ",cluster.size(),_cluster.size());
      for(int i=0;i<50;i++) 
	scluster.push_back(_scluster[rand()%_scluster.size()]);
    }
  }
  _scluster = strset._getClusterRepresentativeData(cls);
  //re=EasyExpSynthesis(scluster,_scluster,-1,600);
  re=EasyExpSynthesis(scluster,_scluster,0,1);
  //re=EasyExpSynthesis(scluster,_scluster,0,600);
}

/////////////////////////////////////////////////////////////////////////
void FindFiles(const char* fexpr) {
  Collection t2,u2;
  string s2;
  int j;
  int target_ncls,index;
  unsigned int cls,cls2,clssize,cls2size;
  vector<string> clsdatabody;
  string re;
  EasyExp e;
  // Multi thread settings
#ifdef N
  Clustering_num_threads=N;
  EasyExp_num_threads=N;
#endif
#ifdef LIM
  EasyExp_num_threads_limit=LIM;
#endif
#ifdef DEBUG
  g_DEBUG=1;
#endif
  char Clst[20]; sprintf(Clst,"%d",CLST);
  IC_DataSetS strset;
  strset.setRepresentativeDataSize(RepresentativeData);
  strset.setRepresentativeDataSizeLimit(RepresentativeDataLimit);
  strset.setSimfunc(simfunc);

  //printf("reading...\n");
  Scandir d(fexpr,isdirectory,caseindependent);
  char* fname;
  Collection c,t,b;
  Collection c1,c2,c3,cu;
  Line l;


  // data loading and min-clustering
  while((fname=d.Next())&&fname[0]) {
    strset.add(fname,fname);
    if((strset.size()%1000)==0) {
      printf("%d ",strset.size()); fflush(stdout); 
    }
  }
  printf("\n");

  // set title line
  l.clear();
  l.let(0,"ncls"); 
  l.let(1,"cls");
  l.let(2,"clssize"); 
  l.let(3,"matchsize"); 
  l.let(5,"regexp"); 
  t.push_back(l); t.setkey(0);

  // set title line
  l.clear();
  l.let(0,"cls"); l.let(1,"name"); 
  c.push_back(l); c.setkey(0);
  l.clear();

  strset.clustering(low,high+5,2);
   
  for(target_ncls=low;target_ncls<=high;target_ncls++) {
    printf("target ncls %d\n",target_ncls);
    index=strset.selectHierClusteringIndex(target_ncls);
    strset.assignHierClustering(index);
    
    // fill contents, raw data
    c.clear(); l.clear();
    l.let(0,"cls"); l.let(1,"name"); 
    c.push_back(l); c.setkey(0);
    l.clear();
    unsigned int i;
    for(i=0;i<strset.size();i++) {
      l.let(0,strset.getDataCluster(i));
      l.let(1,strset.getDataName(i));
      c.push_back(l);
    }
  
    for(cls=0;cls<strset.getNcls();cls++) {
      getEachClusterRegExp(strset,re,cls,EasyExpMode,ListLimit);
      // class ID
      l.let(0,target_ncls);
      l.let(1,cls);

      // number of samples belonging to this class
      clssize = strset.getClusterSize(cls);
      l.let(2,clssize);

      // number of samples match with synthesized EasyExp
      t2=c.match(1,re);
      l.let(3,t2.size());

      // 
      u2 = t2.unique_count(0); u2.sortnumber(0);
      s2="";
      for(j=0;j<u2.size();j++) { 
	cls2=u2[j][0].toint();
	cls2size = c.match("cls",cls2).size();
	s2+=u2[j][0].tostring(); 
	if(cls2size==u2[j][1].toint()) s2+="a";
	else if(cls2size>u2[j][1].toint()) s2+="_";
	else {
	  printf("Internal Error: cls=%d j=%d clssize=%d match=%d t2size=%d %s\n",cls,j,clssize,u2[j][1].toint(),t2.size(),re.c_str());
	  s2+="E"; 
	  u2[j].disp();
	}
      }
      l.let(4,s2);
      
      // Synthesized EasyExp
      l.let(5,re);
      
      t.push_back(l);
    }
  }

  // fill contents, mini-cluster body
  unsigned int cls;
  vector<string> clsdatabody;
  l.clear();
  l.let(0,"cls"); 
  l.let(1,"databody"); 
  b.push_back(l); b.setkey(0);
  for(cls=0;cls<strset.getNcls();cls++) {
    clsdatabody = strset.getClusterRepresentativeData(cls);
    for(i=0;i<clsdatabody.size();i++) {
      l.let(0,cls);
      l.let(1,clsdatabody[i]);
      b.push_back(l);
    }
  }

  cu = t.unique_count(3,5);
  cu.sort(1);
  cu.disp();

  //t.match(0,high).disp();

  if(prompt) {
    while(!G__pause());
  }
}

///////////////////////////////////////////////////////////////////////////////

int main(int argc,char** argv) {
  for(int i=1;i<argc;i++) {
    if(strcmp(argv[i],"-C")==0) CLST=atoi(argv[++i]);
    else if(strcmp(argv[i],"-f")==0) isdirectory=0;
    else if(strcmp(argv[i],"-d")==0) isdirectory=1;
    else if(strcmp(argv[i],"-b")==0) isdirectory=2;
    else if(strcmp(argv[i],"-c")==0) caseindependent=0;
    else if(strcmp(argv[i],"-i")==0) caseindependent=1;
    else if(strcmp(argv[i],"-h")==0) high=atoi(argv[++i]);
    else if(strcmp(argv[i],"-l")==0) low =atoi(argv[++i]);
    else if(strcmp(argv[i],"-p")==0) prompt=1;
    else if(strcmp(argv[i],"-e")==0) prompt=0;
    else if(strcmp(argv[i],"-g")==0) EasyExp_DEBUG++;
    else if(strcmp(argv[i],"-char")==0) simfunc=charmatchratio;
    else if(strcmp(argv[i],"-word")==0) simfunc=wordmatchratio;
    else if(strcmp(argv[i],"-head")==0) simfunc=headmatchratio;
    else if(strcmp(argv[i],"-r")==0) {regexp++; if(!CLST) CLST=16;}
    else if(strcmp(argv[i],"-m")==0) { 
      RepresentativeData=atoi(argv[++i]);
      RepresentativeDataLimit = RepresentativeData*2;
    }
    else if(strcmp(argv[i],"-mode")==0) EasyExpMode=atoi(argv[++i]);
    else if(strcmp(argv[i],"-l")==0) ListLimit=atoi(argv[++i]); 
    else if(strcmp(argv[i],"-?")==0) {
      printf("Usage: cint _find.cxx <option> [fileExpr]\n");
      printf("  -f     : list file only\n");
      printf("  -d     : list directory only\n");
      printf("  -b     : list file and directory (default)\n");
      printf("  -c     : case sensitive\n");
      printf("  -i     : case insensitive (default)\n");
      printf("  -e     : No prompt interaction\n");
      printf("  -p     : Prompt interaction (default)\n");
      printf("  -h     : clustering high limit\n");
      printf("  -l     : clustering low  limit\n");
      printf("   -char  : character match mode\n");
      printf("   -word  : word match mode\n");
      printf("   -head  : head-string match mode\n");
      printf("   -m [Nrepdata] : Set number of representative data cluster\n");
      printf("  -r     : activate regular expression synthesis\n");
      printf("   -mode [mode]   : Set regexp synthesis mode [0-2]\n");
      printf("   -l [ListLimit] : Set regexp synthesis string list limit\n");
      exit(0);
    }
    else {
      FindFiles(argv[i]);
    }
  }
  return(0);
}


