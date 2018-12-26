/*****************************************************************************
 * _find4.cxx
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
FStringSimilarity simfunc[] = { charmatchratio, wordmatchratio, headmatchratio, 0 };
int low=8, high=20;


typedef string Model_t;
typedef string SamplesID_t;


int regexp=0;

int CLST= 0;
int ListLimit = 20;
int EasyExpMode = 0;

int caseindependent=1;
int isdirectory=2;
int prompt=1;


/////////////////////////////////////////////////////////////////////////
void DependencyAnalysis(Collection& c) {
  int xexpr=c.getx("expr");
  EasyExp e;
  string inc,sup;
  char buf[100];

  c.getkey().let(3,"cmplx");
  c.getkey().let(4,"id");
  c.getkey().let(5,"sub");
  c.getkey().let(6,"super");

  unsigned int i,j;
  for(i=0;i<c.size();i++) {
    e.compile(c[i][xexpr].c_str());
    //e.pattern_optimization();
    //c[i].let(0,e.backannotation().c_str());
    inc="";
    for(j=0;j<c.size();j++) {
      //if(i==j) continue;
      if(e.match(c[j][xexpr].c_str())) {

	 sprintf(buf,"%d-",j);
	 inc += buf;

	 sup = c[j][6].tostring();
	 sprintf(buf,"%d+",i);
	 sup += buf;
	 c[j].let(6,sup);
      }
    }
    c[i].let(3,e.complexity());
    c[i].let(5,inc);
    c[i].let(4,i);
  }
}


/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
void FindFiles(const char* fexpr) {
  Collection c,t;
  Line l;

  IC_DataSetS strset;
  strset.setRepresentativeDataSize(RepresentativeData);
  strset.setRepresentativeDataSizeLimit(RepresentativeDataLimit);
  strset.setSimfunc(charmatchratio);  // 

  //printf("reading...\n");
  Scandir d(fexpr,isdirectory,caseindependent);

  // Incremental clustering
  // data loading and min-clustering
  char* fname;
  while((fname=d.Next())&&fname[0]) {
    strset.add(fname,fname);
    if((strset.size()%1000)==0) {
      printf("%d ",strset.size()); fflush(stdout); 
    }
  }
  printf("\n");

  unsigned int i,j,k,cls,index;
  int ncls;
  map<SamplesID_t,Model_t>  done_list;
  map<Model_t,int>          model_list;
  vector<string>            cluster, _cluster;
  SamplesID_t               clusterID;
  char buf[100];
  string                    clusterExpr;
  int mat,unm;
  EasyExp   ex;

  // set title line
  l.clear();
  l.let(0,"cls"); l.let(1,"name"); 
  c.push_back(l); c.setkey(0);
  // fill contents, raw data
  c.clear();
  for(i=0;i<strset.size();i++) {
    l.let(0,strset.getDataCluster(i));
    l.let(1,strset.getDataName(i));
    c.push_back(l);
  }

  l.clear();
  l.let(0,"expr"); 
  l.let(1,"cntorig"); 
  l.let(2,"cntexpr"); 
  t.push_back(l); t.setkey(0);

  //G__optimizemode(0);

  //for(int isimfunc=0;simfunc[isimfunc];isimfunc++) {
  for(int isimfunc=0;isimfunc<3;isimfunc++) { // simfunc loop
    // Hierarchical clustering
    // strset.clustering(low,high+5,2,simfunc[isimfunc]);
    if(isimfunc==0) strset.setSimfunc(charmatchratio);  // 
    else if(isimfunc==1) strset.setSimfunc(wordmatchratio);  // 
    else if(isimfunc==2) strset.setSimfunc(headmatchratio);  // 
    strset.clustering(low,high+5,2);
    
    //G__optimizemode(0);
    //G__tracemode(1);
    for(ncls=low;ncls<=high;ncls++) { // ncls loop
      printf("target ncls %d\n",ncls);
      index=strset.selectHierClusteringIndex(ncls);
      strset.assignHierClustering(index);

      for(cls=0;cls<strset.getNcls();cls++) { // cls loop
	cluster = strset.getClusterRepresentativeData(cls); //cluster = strset.getClusterDataName(cls);
	mat =  cluster.size();

	sort(cluster.begin(),cluster.end());
	sprintf(buf,"%d",cluster.size()); 
	clusterID=buf; clusterID+=cluster.front();

	if(done_list[clusterID]=="") {
	  if(cluster.size()<20) {
	    _cluster = strset.getClusterDataName(cls);
	    if(_cluster.size()<200) {
	      //printf(" %d->%d ",cluster.size(),_cluster.size());
	      cluster=_cluster;
	    }
	    else {
	      //printf(" %d->%d->+50 ",cluster.size(),_cluster.size());
	      for(i=0;i<50;i++) 
		cluster.push_back(_cluster[rand()%_cluster.size()]);
	    }
	  }

	  _cluster = strset._getClusterRepresentativeData(cls); 
	  //unm = _cluster.size();

	  clusterExpr=EasyExpSynthesis(cluster,_cluster,0,5);
	  ex.compile(clusterExpr.c_str());
	  ex.pattern_optimization();

	  printf("%s : %s\n",clusterID.c_str(),clusterExpr.c_str());
	  done_list[clusterID] = clusterExpr;
	  
	  //l.let(0,clusterExpr);
	  l.let(0,ex.backannotation());
	  l.let(1,cluster.size());
	  //l.let(2,c.match(1,clusterExpr).size());
	  l.let(2,c.match(1,ex.backannotation()).size());
	  t.push_back(l);
	}
      }
    }
  }


  t.sort(0);
  Collection u = t.unique_count(0,2);
  DependencyAnalysis(u);
  //u.disp();

  Collection uc = u.range("cmplx",2,4);
  uc.multisort("cmplx","cntexpr");
  uc.reverse();
  uc.disp();

  if(prompt) {
    while(!G__pause());
  }
}

///////////////////////////////////////////////////////////////////////////////

int main(int argc,char** argv) {
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
    //else if(strcmp(argv[i],"-char")==0) simfunc=charmatchratio;
    //else if(strcmp(argv[i],"-word")==0) simfunc=wordmatchratio;
    //else if(strcmp(argv[i],"-head")==0) simfunc=headmatchratio;
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


