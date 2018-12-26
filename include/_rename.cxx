/*****************************************************************************
 * _rename.cxx
 *  Description: 
 *    Find files with EasyExp scanning
 *    Classificadtion with Incremental Clustering (new lib, fast, no size limit)
 *    Summarize result with EasyExp synthesis
 *    This version requires number of cluster as input. 
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
NN_REAL thresh=1.0;
int ListLimit = 5;
int EasyExpMode = 0;

int caseindependent=1;
int isdirectory=2;
int prompt=0;

/////////////////////////////////////////////////////////////////////////
void FindFiles(const char* fexpr) {
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
  EasyExp_DEBUG=1;
#endif
  IC_DataSetS strset;
  strset.setRepresentativeDataSize(RepresentativeData);
  strset.setRepresentativeDataSizeLimit(RepresentativeDataLimit);
  strset.setSimfunc(charmatchratio);

  //printf("reading...\n");
  Scandir d(fexpr,isdirectory,caseindependent);
  char* fname;
  Collection c,t,b;
  Line l;

  if(CLST==0 && regexp==0) {
    // without clustering
    l.let(0,"name");
    c.push_back(l);
    c.setkey(0);
    while((fname=d.Next())&&fname[0]) {
      l.let(0,fname);
      c.push_back(l);
    }
    G__ateval(c);
  }
  else {
    //printf("clustering...\n");
    while((fname=d.Next())&&fname[0]) {
      strset.push_back(fname,fname);
      if((strset.size()%1000)==0) {
	printf("%d ",strset.size()); fflush(stdout); 
      }
    }
    printf("\n");
    
    if(thresh<1.0) // clsutering by similarity threshold
      strset.clustering(thresh,2,simfunc);
    else           // clustering by number of clusters
      strset.clustering(CLST,CLST*2,2,simfunc);

    // set title line
    l.let(0,"cls"); l.let(1,"name"); 
    c.push_back(l); c.setkey(0);

    // fill contents, raw data
    unsigned int i;
    for(i=0;i<strset.size();i++) {
      l.let(0,strset.getDataClass(i));
      l.let(1,strset.getDataName(i));
      c.push_back(l);
    }
    // fill contents, mini-cluster body
    unsigned int cls;
    vector<string> clsdatabody;
    l.let(0,"cls"); l.let(1,"databody"); 
    b.push_back(l); b.setkey(0);
    for(cls=0;cls<strset.getNcls();cls++) {
      clsdatabody = strset.getClusterRepresentativeData(cls);
      for(i=0;i<clsdatabody.size();i++) {
	l.let(0,cls);
	l.let(1,clsdatabody[i]);
	b.push_back(l);
      }
    }
    
    if(regexp) {
      //printf("regular expression synthesis...\n");
      // set title line
      l.let(0,"cls"); l.let(1,"count"); l.let(2,"regexp");
      t.push_back(l); t.setkey(0); 
      
      // fill contents, synthesized regular expression
      unsigned int cls;
      string re;
      for(cls=0;cls<strset.getNcls();cls++) {
	EachClusterEasyExpSynthesis(strset,re,cls,EasyExpMode,ListLimit);
	l.let(0,cls);
	l.let(1,strset.getClusterSize(cls));
	l.let(2,re);
	t.push_back(l);
      }
      G__ateval(t);
    }
    else G__ateval(c);
  }

  if(prompt) {
    printf(" c : complete list of files,  t : synthesized regular expression for each clsuter ");
    while(!G__pause());
  }
}

///////////////////////////////////////////////////////////////////////////////
void EachClusterEasyExpSynthesis(IC_DataSetS& strset,string& re
			  ,unsigned int cls ,int mode=0,int LIST_LIMIT=20) {
  vector<string>  scluster = strset.getClusterRepresentativeData(cls);
  vector<string> _scluster = strset._getClusterRepresentativeData(cls);
  if(scluster.size()<20) { // if representative samples are too few, add others
    vector<string> allsamples = strset.getClusterDataName(cls);
    if(allsamples.size()<200) scluster=allsamples;
    else for(int i=0;i<50;i++) scluster.push_back(allsamples[rand()%allsamples.size()]);
  }
  re=EasyExpSynthesis(scluster,_scluster,mode,LIST_LIMIT);
}
///////////////////////////////////////////////////////////////////////////////

int main(int argc,char** argv) {
  for(int i=1;i<argc;i++) {
    if(strcmp(argv[i],"-C")==0) CLST=atoi(argv[++i]);
    else if(strcmp(argv[i],"-t")==0) { thresh=atof(argv[++i]); CLST=2; }
    else if(strcmp(argv[i],"-f")==0) isdirectory=0;
    else if(strcmp(argv[i],"-d")==0) isdirectory=1;
    else if(strcmp(argv[i],"-b")==0) isdirectory=2;

    else if(strcmp(argv[i],"-c")==0) caseindependent=0;
    else if(strcmp(argv[i],"-i")==0) caseindependent=1;

    else if(strcmp(argv[i],"-p")==0) prompt=1;
    else if(strcmp(argv[i],"-v")==0) prompt=1;
    else if(strcmp(argv[i],"-e")==0) prompt=0;

    else if(strcmp(argv[i],"-char")==0) simfunc=charmatchratio;
    else if(strcmp(argv[i],"-word")==0) simfunc=wordmatchratio;
    else if(strcmp(argv[i],"-head")==0) simfunc=headmatchratio;

    else if(strcmp(argv[i],"-r")==0) {regexp++; if(!CLST) CLST=10;}
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

      printf("  -p     : Prompt interaction (default)\n");
      //printf("  -v     : Prompt interaction (default)\n");
      //printf("  -e     : No prompt interaction\n");

      printf("  -t [thresh] : set similarity thresh (0.0 - 1.0) and activate clustering\n");
      printf("  -C [N]  : set number of Clusters (2..*) and activate clustering\n");
      printf("    -char  : character match mode\n");
      printf("    -word  : word match mode\n");
      printf("    -head  : head-string match mode\n");
      printf("    -m [Nrepdata] : Set number of representative data cluster\n");
      printf("  -r     : activate regular expression synthesis\n");
      printf("    -mode [mode]   : Set regexp synthesis mode [0-2]\n");
      printf("    -l [ListLimit] : Set regexp synthesis string list limit\n");
      exit(0);
    }
    else {
      FindFiles(argv[i]);
    }
  }
  return(0);
}




