/*****************************************************************************
 * _tfidf2.cxx
 *  Description: 
 *    Read files and calculate TF-IDF sparse matrix, do clustering and 
 *   regular expression synthesis on file name.
 *      (Term Frequency - Inverse Document Frequency clustering)
 *  Usage:
 *     cint _tfidf.cxx <option> [grepExpr] [fileExpr] <[fileExpr2]. <...>>
 *  Status: Experimental
 *  Date: 2015/Dec/7
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
int caseinsensitive=1;
int isdirectory=0;

#ifdef IC_DATASET_H
#endif

int regexp=0;
int ListLimit = 50;
int EasyExpMode = 0;

int prompt=0;

Collection c,t,u; // output collection
Collection c1,c2,t1,t2,u; // aux buffer
int    loadflag=0;

// TF_IDF is defined in CSVclustering.h
TF_IDF g_tfidf;

// Incremental Hierarchical Clustering object for Sparse Vector
IC_DataSetFSV vecset;
vector<string> vdoc;

int debug=0;

/////////////////////////////////////////////////////////////////////////
void FindFiles(const char* fexpr,const char* gexpr) {
  EasyExp e(gexpr);

  c.setkey("fname","line","expr");

  //printf("reading...\n");
  Scandir d(fexpr,isdirectory,caseinsensitive);
  char* fname;

  while((fname=d.Next())&&fname[0]) {
    if(debug>1) printf("%s ",fname);
    g_tfidf.ReadFile(fname,e);
  }
  if(debug>1) printf("\n");
}

  
/////////////////////////////////////////////////////////////////////////
void clustering(double criteria,double idf_thresh=0) {
  int CLST=0;
  NN_REAL thresh=1.0;
  if(criteria==1.0) { }
  if(criteria>1.0) { CLST=(int)criteria; thresh=1.0; }
  else {CLST=0; thresh=criteria; }

  // update RepresentativeData 
  if(loadflag!=0 && criteria<1.0 && vecset.getHierClusteringThresh(1)<thresh) {
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
    // calculate TF-IDF
    //vector<string> vdoc;
    vdoc.clear();
    vector<FSparseVector> vsv;
    g_tfidf.TFIDF_DocVector(vdoc,vsv); // get TF-IDF sparse vector from TFIDF object
    if(debug>0) printf("size=%d %d\n",vdoc.size(),vsv.size());
    
    // set up Incremental Hierarchical Clustering object
    //IC_DataSetFSV vecset;
    FSparseVectorSimilarity simfunc= SparseNormalizedInnerProduct;
    if(criteria>200) {
      RepresentativeData = (int)criteria+10;
      RepresentativeDataLimit = (int)criteria*2;
    }
    vecset.setKeepOriginalData(false);
    vecset.setRepresentativeDataSize(RepresentativeData);
    vecset.setRepresentativeDataSizeLimit(RepresentativeDataLimit);
    vecset.setSimfunc(simfunc);
    // set data
    for(unsigned int i=0;i<vdoc.size();i++) {
      vecset.add(vdoc[i],vsv[i]);
      if((vecset.size()%1000)==0){
	printf("%d ",vecset.size()); fflush(stdout); 
      }
    }
    if(vecset.size()>1000) printf("\n");
  }
  
  // final clustering
  if(thresh<1.0) 
    vecset.clustering(thresh,IC_AVERAGE);
  else 
    vecset.clustering(CLST,CLST*2,IC_AVERAGE);

  // copy clustering result to Collection object
  Line l;
  t.clear();
  t.setkey("cls","fname");
  for(unsigned int i=0;i<vdoc.size();i++) {
    l.let(0,vecset.getDataCluster(i));
    l.let(1,vdoc[i]);
    t.push_back(l);
  }

  t.multisort(0,1);

  if(regexp) t.EasyExpSynthesis(1,2,0,EasyExpMode,ListLimit);

#ifdef __CINT__
  if(prompt) G__ateval(t);
  else t.disp();
#else
  t.disp();
#endif
}

///////////////////////////////////////////////////////////////////////////////
void AnalyzeWithSparseVector(double criteria,double idf_thresh=0) {
  clustering(criteria,idf_thresh);

#ifdef __CINT__
  if(prompt) {
    if(criteria==1.0) G__ateval(c);
    printf(" c:TFIDF matrix(huge matrix), t:clsutering result");
    while(!G__pause());
  }
  else if(criteria==1.0) c.disp();
#else
  if(criteria==1.0) c.disp();
#endif
}


/////////////////////////////////////////////////////////////////////////
void AnalyzeWithSimpleVector(double criteria,double idf_thresh=0) {
  Collection term=g_tfidf.ToCollection();
  term.sort("idf");
  if(idf_thresh>0) term = term.range("idf",idf_thresh,10000.0);
  Collection doc=term.rotate(1),doccls;
  doc.erase(0); // erase IDF line
  doc.getkey().let(0,"fname");

  if(criteria!=1.0) {
#ifdef TERM
    printf("clustering term...\n");
    term.clustering(1,0,-1,criteria);
#endif
    printf("clustering doc...\n");
    doc.clustering(1,0,-1,criteria);

#ifdef TERM
    term.sort("cls");
#endif
    doc.sort("cls");

    vector<int> xs; 
    xs.push_back(doc.getx("cls"));
    xs.push_back(0); // doc
    doccls = doc.column(xs);

#ifdef TERM
    xs[0]=term.getx("cls"); 
    xs[1]=term.getx("idf");
    xs.push_back(0); // term 
    Collection termcls = term.column(xs);
#endif
  }

  Collection& t=doccls;
  Collection& c=doc;

  t.multisort(0,1);

#ifdef __CINT__
  if(prompt) {
    if(criteria!=1.0) t.disp();
    else G__ateval(c);
    printf(" c:TFIDF matrix(huge matrix), t:clsutering result");
    while(!G__pause());
  }
  else {
    if(criteria!=1.0) t.disp();
    else c.disp();
  }
#else
  if(criteria!=1.0) t.disp();
  else c.disp();
#endif
}


///////////////////////////////////////////////////////////////////////////////
int main(int argc,char** argv) {
  int flag, simplevector=0;
  double criteria=1.0, idf_thresh=0.0;
  string grepExpr="*";
  for(int i=1;i<argc;i++) {
    // directory scan
    if(strcmp(argv[i],"-f")==0) isdirectory=0; // default
    else if(strcmp(argv[i],"-d")==0) isdirectory=1; // invalid
    else if(strcmp(argv[i],"-b")==0) isdirectory=2; // invalid
    else if(strcmp(argv[i],"-c")==0) { caseinsensitive=0; g_tfidf.casesensitive(); }
    else if(strcmp(argv[i],"-i")==0) { caseinsensitive=1; g_tfidf.caseinsensitive();}//default

    // clustering
    else if(strcmp(argv[i],"-C")==0) criteria=atof(argv[++i]);
    else if(strcmp(argv[i],"-t")==0) criteria=atof(argv[++i]);

#ifdef IC_DATASET_H
    //else if(strcmp(argv[i],"-innerproduct")==0) simfunc=NormalizedInnerProduct; // default
#endif
    else if(strcmp(argv[i],"-idf")==0) idf_thresh=atof(argv[++i]); 

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
    else if(strcmp(argv[i],"-e")==0) grepExpr = argv[++i];

    // other option
    else if(strcmp(argv[i],"-sparse")==0) simplevector=0;
    else if(strcmp(argv[i],"-simple")==0) simplevector=1;

    else if(strcmp(argv[i],"-g")==0) ++debug;

    // help
    else if(strcmp(argv[i],"-?")==0) {
      printf("Usage: cint _grep.cxx <option> [fileExpr] <[fileExpr2]. <...>>\n");
      printf("  -e [expr]   : line select expression\n");
      printf("  -idf [thresh] : IDF threshold\n");

      printf("  -f          : list file only\n");
      printf("  -d          : list directory only\n");
      printf("  -b          : list file and directory (default)\n");

      printf("  -c          : case sensitive\n");
      printf("  -i          : case insensitive (default)\n");


      printf("  -C [N]      : number of clusters\n");
      printf("  -t [thresh] : clustering threshold\n");
      //printf("    -string   : string match mode (default)\n");
      //printf("    -char     : character match mode\n");
      ////printf("    -word     : word match mode\n");
      //printf("    -head     : head-string match mode\n");
      printf("    -m [Nrep] : Set number of representative data cluster\n");

      printf("  -r          : turn on regexp synthesis\n");
      printf("    -mode [mode]   : Set regexp synthesis mode [0-2]\n");
      printf("    -l [ListLimit] : Set regexp synthesis string list limit\n");

      printf("  -p          : Prompt interaction\n");
      printf("  -sparse     : Use sparse vector (default)\n");
      printf("  -simple     : Use simple vector\n");

      printf("  -g          : debug\n");

      exit(0);
    }
    else {
      FindFiles(argv[i],grepExpr.c_str());
    }
  }

  if(simplevector) AnalyzeWithSimpleVector(criteria,idf_thresh);
  else             AnalyzeWithSparseVector(criteria,idf_thresh);
  return(0);
}


