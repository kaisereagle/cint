/*****************************************************************************
 * _wc.cxx
 *  Description: 
 *    line and word counting and clustering.
 *    Classificadtion with Incremental Clustering (new lib, fast, no size limit)
 *    Summarize result with EasyExp synthesis
 *    This version requires number of cluster as input. 
 *  Usage:
 *    cint _wc.cxx <option> [fileExpr] <[fileExpr2] <...>>
 *  Status: Under development
 *  Date: 2015/Oct/6
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

IC_REAL Lthresh=0.5,Wthresh=0.5;
int ListLimit = 5;
int EasyExpMode = 0;

int caseindependent=1;
int isdirectory=2;
int prompt=0;

IC_DataSetS lines,words;

/////////////////////////////////////////////////////////////////////////
void ReadFileContents(const string& fname) {
  ReadF f(fname.c_str());
  if(fname.find(".csv")!=string::npos) {
    f.setseparator("");
    f.setdelimiter(",");
  }
  else {
    f.setseparator(" ,();");
  }
  int i,line;
  char id[1000];
  while(f.read()) {
    sprintf(id,"%s_%d",fname.c_str(),f.line);
    lines.add(id,f.argv[0]);
    for(i=1;i<=f.argc;i++) {
      sprintf(id,"%s_%d_%d",fname.c_str(),f.line,i);
      words.add(id,f.argv[i]);
    }
    line=f.line;
  }
  printf("%s %d\n",fname.c_str(),line);
}

/////////////////////////////////////////////////////////////////////////
void RetrieveRep(Collection& c,IC_DataSetS& s) {
  unsigned int int i,cls,ncls=s.getNcls();
  vector<IC_DataS*> reps;
  Line l;
  for(cls=0;cls<ncls;cls++) {
    reps = s.getClusterRepresentativeDataHolder(cls);
    l.let(0,cls);
    for(i=0;i<reps.size();i++) {
      l.let(1,reps[i]->nsample());
      l.let(2,reps[i]->x);
      c.push_back(l);
    }
  }
}

/////////////////////////////////////////////////////////////////////////
void RetrieveData(Collection& c,IC_DataSetS& s) {
  unsigned int int i,cls,ncls=s.getNcls();
  vector<pair<string,string> > pairs;
  Line l;
  for(cls=0;cls<ncls;cls++) {
    pairs = s.getClusterDataPair(cls);
    l.let(0,cls);
    for(i=0;i<pairs.size();i++) {
      l.let(1,pairs[i].first);
      l.let(2,pairs[i].second);
      c.push_back(l);
    }
  }
}

/////////////////////////////////////////////////////////////////////////
void FindFiles(const char* fexpr) {
  lines.setRepresentativeDataSize(RepresentativeData);
  words.setRepresentativeDataSize(RepresentativeData);
  lines.setRepresentativeDataSizeLimit(RepresentativeDataLimit);
  words.setRepresentativeDataSizeLimit(RepresentativeDataLimit);
  lines.setSimfunc((void*)wordmatchratio);
  words.setSimfunc((void*)charmatchratio);

  Scandir d(fexpr,isdirectory,caseindependent);
  char* fname;
  while((fname=d.Next())&&fname[0]) {
    ReadFileContents(fname);
  }
  printf("\n");

  lines.clustering(Lthresh,IC_AVERAGE);
  words.clustering(Wthresh,IC_AVERAGE);

  Collection cline,cword, rline, rword;
  Line l;

  RetrieveRep(rline,lines);
  RetrieveRep(rword,words);

  RetrieveData(cline,lines);
  RetrieveData(cword,words);
    
  G__ateval(rline);

  printf(" rline : line reps,  rword : word reps, ");
  while(!G__pause());
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
    else if(strcmp(argv[i],"-wt")==0) { Wthresh=atof(argv[++i]); CLST=2; }
    else if(strcmp(argv[i],"-lt")==0) { Lthresh=atof(argv[++i]); CLST=2; }
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
      printf("Usage: cint _wc.cxx <option> [fileExpr]\n");
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


