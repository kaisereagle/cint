/*****************************************************************************
 * CSVclustering.h
 *  Description: 
 *    CSV table format manipulator optional library for incremental clustering.
 *  Status: Experimental
 *  Date: 2015/Dec/26
 *  Author: Masaharu Goto
 *****************************************************************************/
/************************************************************************
 * Copyright(c) 1995~2015  Masaharu Goto (gotom@hanno.jp)
 *
 * Permission to use, copy, modify and distribute this software and its 
 * documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation. The author makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 ************************************************************************/

#ifndef CSVCLUSTERING_H
#define CSVCLUSTERING_H
/////////////////////////////////////////////////////////////////////////////
// Memo.
// Implementation in this source file is premature.
//  IC_DataSetX and TF_IDF objects has to be presistent in order to use those
// objects iteratively and utilize useful information there.
/////////////////////////////////////////////////////////////////////////////

#pragma include_noerr <IncClustering4.dll>
#ifndef IC_DATASET_H
void* stringmatchratio=0;
void* NormalizedInnerProduct=0;
class FSparseVector;
#endif

/////////////////////////////////////////////////////////////////////////////
#define IC_STRING 0
#define IC_VECTOR 1

int RepresentativeData = 200;
int RepresentativeDataLimit = 400;

/////////////////////////////////////////////////////////////////////////////
// Single column string or scalar clustering
/////////////////////////////////////////////////////////////////////////////
int Collection::clustering(int x,int xcls,double criteria=0.7,void* simfunc=stringmatchratio) {
  FStringSimilarity Ssimfunc = 0;
  FVectorSimilarity FVsimfunc = 0;
  if(simfunc==stringmatchratio ||
     simfunc==charmatchratio ||
     simfunc==wordmatchratio ||
     simfunc==headmatchratio) Ssimfunc=simfunc;
  else if(simfunc==NormalizedInnerProduct ||
	  simfunc==NormalizedEuclideanDistance ||
	  simfunc==NormalizedVectorInnerProduct ||
	  simfunc==NormalizedEuclideanSimilarity) FVsimfunc=simfunc;

  if(criteria>200) {
    RepresentativeData = (int)criteria+10;
    RepresentativeDataLimit = (int)criteria*2;
  }

  if(xcls<0) {
    xcls=getx("cls");
    if(xcls<0) xcls=getkey().size();
  }

  int CLST,i;
  NN_REAL thresh;

  if(Ssimfunc) {
    IC_DataSetS strset;
    strset.setKeepOriginalData(true);
    strset.setRepresentativeDataSize(RepresentativeData);
    strset.setRepresentativeDataSizeLimit(RepresentativeDataLimit);
    strset.setSimfunc(Ssimfunc);
    for(i=0;i<size();i++) strset.add(i,get(i)[x].tostring());

    if(criteria<1.0) {
      thresh = criteria;
      strset.clustering(thresh,IC_AVERAGE);
    }
    else {
      CLST = criteria;
      strset.clustering(CLST,CLST*2,IC_AVERAGE);
    }
    for(i=0;i<size();i++) get(i).let(xcls,strset.getDataCluster(i));
  }
  else if(FVsimfunc) {
    IC_DataSetFV vecset;
    vecset.setKeepOriginalData(true);
    vecset.setRepresentativeDataSize(RepresentativeData);
    vecset.setRepresentativeDataSizeLimit(RepresentativeDataLimit);
    vecset.setSimfunc(FVsimfunc);
    FVector d; d.resize(1);
    for(i=0;i<size();i++) {
      d[0] = get(i)[x].tofloat();
      vecset.add(i,d);
    }

    if(criteria<1.0) {
      thresh = criteria;
      vecset.clustering(thresh,IC_AVERAGE);
    }
    else {
      CLST = criteria;
      vecset.clustering(CLST,CLST*2,IC_AVERAGE);
    }
    for(i=0;i<size();i++) get(i).let(xcls,vecset.getDataCluster(i));
  }
  getkey().let(xcls,"cls");
  return(0);
}

/////////////////////////////////////////////////////////////////////////////
// Multiple column vector clustering
/////////////////////////////////////////////////////////////////////////////
int Collection::clustering(int xfrom,int xto,int xcls,double criteria,void* simfunc=NormalizedInnerProduct) {
  FStringSimilarity Ssimfunc = 0;
  FVectorSimilarity FVsimfunc = 0;
  if(simfunc==stringmatchratio ||
     simfunc==charmatchratio ||
     simfunc==wordmatchratio ||
     simfunc==headmatchratio) Ssimfunc=simfunc;
  else if(simfunc==NormalizedInnerProduct ||
	  simfunc==NormalizedEuclideanDistance ||
	  simfunc==NormalizedVectorInnerProduct ||
	  simfunc==NormalizedEuclideanSimilarity) FVsimfunc=simfunc;

  if(criteria>200) {
    RepresentativeData = (int)criteria+10;
    RepresentativeDataLimit = (int)criteria*2;
  }

  if(xto<xfrom) xto=getkey().size()-1;
  if(xcls<0) {
    xcls=getx("cls");
    if(xcls<0) xcls=getkey().size();
  }

  int CLST,i;
  NN_REAL thresh;
  char id[20];

  if(Ssimfunc) {
    IC_DataSetS strset;
    strset.setKeepOriginalData(true);
    strset.setRepresentativeDataSize(RepresentativeData);
    strset.setRepresentativeDataSizeLimit(RepresentativeDataLimit);
    strset.setSimfunc(Ssimfunc);
    string s;
    for(i=0;i<size();i++) {
      s = "";
      for(j=xfrom;j<=xto;j++) s += get(i)[j].tostring();
      vecset.add(i,s);
    }

    if(criteria<1.0) {
      thresh = criteria;
      strset.clustering(CLST,CLST*2,IC_AVERAGE);
    }
    else {
      CLST = criteria;
      strset.clustering(thresh,IC_AVERAGE);
    }
    for(i=0;i<size();i++) get(i).let(xcls,strset.getDataCluster(i));
  }
  else if(FVsimfunc) {
    IC_DataSetFV vecset;
    vecset.setKeepOriginalData(true);
    vecset.setRepresentativeDataSize(RepresentativeData);
    vecset.setRepresentativeDataSizeLimit(RepresentativeDataLimit);
    vecset.setSimfunc(FVsimfunc);
    FVector d; 
    int j;
    for(i=0;i<size();i++) {
      d.clear();
      for(j=xfrom;j<=xto;j++) d.push_back(get(i)[j].tofloat());
      vecset.add(i,d);
    }

    if(criteria<1.0) {
      thresh = criteria;
      vecset.clustering(thresh,IC_AVERAGE);
    }
    else {
      CLST = criteria;
      vecset.clustering(CLST,CLST*2,IC_AVERAGE);
    }
    for(i=0;i<size();i++) get(i).let(xcls,vecset.getDataCluster(i));
  }
  getkey().let(xcls,"cls");
  return(0);
}

/////////////////////////////////////////////////////////////////////////////
// Multiple column string clustering with TFIDF
/////////////////////////////////////////////////////////////////////////////
// The oldest version, using dense vector in a Collection.
// development only
int Collection::TFIDFclusteringSimpleVector(int xfrom,int xto,int xcls
					    ,double criteria) {
  if(xto<xfrom) xto=getkey().size()-1;
  if(xcls<0) {
    xcls=getx("cls");
    if(xcls<0) xcls=getkey().size();
  }
  getkey().let(xcls,"cls");

  TF_IDF tfidf;
  char name[80];
  string str;
  int i,j;

  // accumulate term count 
  for(i=0;i<size();i++) {
    sprintf(name,"%d",i);
    for(j=xfrom;j<=xto;j++) {
      str=get(i)[j].tostring();
      //printf("%d %d %s %s\n",i,j,name,str.c_str());
      tfidf.Add(name,str);
    }
  }
  // calculate TF-IDF
  Collection term=tfidf.TFIDF_TermCollection();

  // Rotate term table to make doc table
  //term.sort("idf");
  double idf_thresh=0;
  if(idf_thresh>0) term = term.range("idf",idf_thresh,10000.0);
  Collection doc=term.rotate(1);
  //term.dispform();
  //doc.dispform();
  //G__pause();

  doc.erase(0); // erase IDF line
  doc.getkey().let(0,"fname");

  // clustering
  doc.clustering(1,0,-1,criteria);

  // copy cluster infomation to original table
  doc.sortnumber(0);
  int xclsdoc=doc.getx("cls");
  for(i=0;i<doc.size();i++) {
    get(i).let(xcls,doc[i][xclsdoc].tostring());
  }
  return(0);
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// Older version, using sparse vector, but less flexibility
int Collection::TFIDFclustering(int xfrom,int xto,int xcls,double criteria) {
  if(xto<xfrom) xto=getkey().size()-1;
  if(xcls<0) {
    xcls=getx("cls");
    if(xcls<0) xcls=getkey().size();
  }
  getkey().let(xcls,"cls");

  TF_IDF tfidf;
  char name[80];
  string str;
  int i,j;

  // accumulate term count 
  for(i=0;i<size();i++) {
    sprintf(name,"%d",i);
    for(j=xfrom;j<=xto;j++) {
      str=get(i)[j].tostring();
      //printf("%d %d %s %s\n",i,j,name,str.c_str());
      tfidf.Add(name,str);
    }
  }
  // calculate TF-IDF
  vector<string> vdoc;
  vector<FSparseVector> vsv;
  tfidf.TFIDF_DocVector(vdoc,vsv);

  FSparseVectorSimilarity FSVsimfunc= SparseNormalizedInnerProduct;
  if(criteria>200) {
    RepresentativeData = (int)criteria+10;
    RepresentativeDataLimit = (int)criteria*2;
  }

  if(xto<xfrom) xto=getkey().size()-1;
  if(xcls<0) {
    xcls=getx("cls");
    if(xcls<0) xcls=getkey().size();
  }

  int CLST,i;
  NN_REAL thresh;
  char id[20];

  // set up IHC object
  IC_DataSetFSV vecset;
  vecset.setKeepOriginalData(false);
  vecset.setRepresentativeDataSize(RepresentativeData);
  vecset.setRepresentativeDataSizeLimit(RepresentativeDataLimit);
  vecset.setSimfunc(FSVsimfunc);
  // data accumulation
  for(i=0;i<vdoc.size();i++) {
    vecset.add(vdoc[i],vsv[i]);
  }
  
  // clustering
  if(criteria<1.0) {
    thresh = criteria;
    vecset.clustering(thresh,IC_AVERAGE);
  }
  else {
    CLST = criteria;
    vecset.clustering(CLST,CLST*2,IC_AVERAGE);
  }

  Collection tmp;
  for(i=0;i<vdoc.size();i++) {
    get(atoi(vdoc[i].c_str())).let(xcls,vecset.getDataCluster(i));
  }

  getkey().let(xcls,"cls");
  return(0);
}

/////////////////////////////////////////////////////////////////////////////
// Newer version, still experimental
// Using sparse vector and flexbity
// Improvement needed
//  tfidf object is local and volatile, it is nicer to utilize tfidf object
//  for extarcting other useful information.
template<class T>
Collection Collection::TFIDFclustering(const vector<int>& xtarget
				,const vector<int>& xpartition
				,int xcls
				,double criteria
				,ReadF& p
				,T& e // EasyExp or vector<EasyExp>
				,bool single=false) {
  int i,j;
  string separator="#";

  // setup "cls" column
  if(xcls<0) {
    xcls=getx("cls");
    if(xcls<0) xcls=getkey().size();
  }
  getkey().let(xcls,"cls");

  ///////////////////////////////////////////////////////////
  // setup TFIDF object
  TF_IDF tfidf;
  if(single) tfidf.setSingleCount();

  // accumulate term count 
  if(xpartition.size()==0) {
    // every line as clustering item
    char name[80];
    for(i=0;i<size();i++) {
      sprintf(name,"%d",i); // line number as id
      for(j=0;j<xtarget.size();j++) {
	string str=get(i)[xtarget[j]].tostring();
	//printf("%d %d %s %s\n",i,j,name,str.c_str());
	tfidf.add(name,str,p,e);
      }
    }
  }
  else {
    // unique conbination of xpartition columns as clustering item
    CollectionIterator first=begin(xpartition),last=end(first);
    string id;
    for(;first!=last;++first) {
      Collection cpartition = *first;
      id = first.id(separator); // partition columns contents as id
      for(i=0;i<cpartition.size();i++) {
	for(j=0;j<xtarget.size();j++) {
	  string str=cpartition[i][xtarget[j]].tostring();
	  //printf("XXX %d %d %s %s\n",i,j,id.c_str(),str.c_str());
	  tfidf.add(id,str,p,e);
	  //G__pause();
	}
      }
    }
  }
  
  // calculate TF-IDF
  vector<string> vdoc;
  vector<FSparseVector> vsv; // SparseVector<termindex,TFIDF>
  tfidf.TFIDF_DocVector(vdoc,vsv);

  ///////////////////////////////////////////////////////////
  // set up Incremental Hierarchical Clustering object
  IC_DataSetFSV vecset;
  FSparseVectorSimilarity FSVsimfunc= SparseNormalizedInnerProduct;
  if(criteria>200) {
    RepresentativeData = (int)criteria+10;
    RepresentativeDataLimit = (int)criteria*2;
  }
  vecset.setKeepOriginalData(false);
  vecset.setRepresentativeDataSize(RepresentativeData);
  vecset.setRepresentativeDataSizeLimit(RepresentativeDataLimit);
  vecset.setSimfunc(FSVsimfunc);
  // data accumulation
  for(i=0;i<vdoc.size();i++) {
    vecset.add(vdoc[i],vsv[i]);
  }
  
  // clustering
  if(criteria<1.0) {
    NN_REAL thresh = criteria;
    vecset.clustering(thresh,IC_AVERAGE);
  }
  else {
    int CLST = criteria;
    vecset.clustering(CLST,CLST*2,IC_AVERAGE);
  }

  // copy cluster id to cls column 
  if(xpartition.size()==0) {
    for(i=0;i<vdoc.size();i++) {
      get(atoi(vdoc[i].c_str())).let(xcls,vecset.getDataCluster(i));
      // maybe this is just fine
      //get(i).let(xcls,vecset.getDataCluster(i));
    }
  }
  else {
    ReadF p; p.setseparator(separator);
    for(i=0;i<vdoc.size();i++) {
      vector<string> exprs = p.separatetoken(vdoc[i]);
      Collection citem = match(xpartition,exprs);
      citem.let(xcls,vecset.getDataCluster(i));
    }
  }

  ////////////////////////////////////////////////////////
  // Calculate TFIDF average for each cluster
  //  Same calculation can be obtatined from IC_DataSetFSV
  // term,cls0 ,cls1,...,clsn
  // t1  ,tfidf,...
  Collection cterm;
  //if(xpartition.size()) {
    Collection cdoc = tfidf.TFIDF_DocCollection();
    cdoc.getkey().let(0,"cls");
    for(i=0;i<vdoc.size();i++) cdoc[i].let(0,vecset.getDataCluster(i));
    Collection ctest, cdevice;
    Line l;
    cdevice.setkey(cdoc.getkey());
    CollectionIterator first=cdoc.begin(0),last=cdoc.end(first);
    for(;first!=last;++first) {
      ctest = *first;
      //string clsid = first.id();
      //l.let(0,clsid);
      CollectionStatistics& cstat = ctest.stat();
      for(i=0;i<cstat.size();i++) l.let(i,cstat[i].mean());
      cdevice.push_back(l);
    }
    cterm = cdevice.rotate(1);
  //}

  return(cterm);
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// EasyExp Synthesis
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
template<class T>
int Collection::EasyExpSynthesis(int xtarget,int xregexp
				 ,const T& xcls // T as vector<int>
				 ,int mode=0,int LIST_LIMIT=20) {
  if(xregexp<0) xregexp=getkey().size();
  vector<string>  scluster, _scluster; 
  if(xcls!=T()) {
    CollectionIterator first=begin(xcls),last=end(first);
    for(;first!=last;++first) {
      Collection ccls=first.get(), _ccls=first._get();
      scluster = ccls.VectorString(xtarget);
      _scluster = _ccls.VectorString(xtarget);
      string re=EasyExpSynthesis(scluster,_scluster,mode,LIST_LIMIT);
      ccls[0].let(xregexp,re);
    }
  }
  else {
    for(int j=0;j<size();j++) scluster.push_back(get(j)[xtarget].tostring());
    string re=EasyExpSynthesis(scluster,_scluster,mode,LIST_LIMIT);
    get(0).let(xregexp,re);
  }
  getkey().let(xregexp,"regexp");
  return(0);
}
/////////////////////////////////////////////////////////////////////////////
#if 1
int Collection::EasyExpSynthesis(int xtarget,int xregexp
				 ,int xcls=-1,int mode=0,int LIST_LIMIT=20) {
  if(xregexp<0) xregexp=getkey().size();
  vector<string>  scluster, _scluster; 
  if(xcls>=0) {
    CollectionIterator first=begin(xcls),last=end(first);
    for(;first!=last;++first) {
      Collection ccls=first.get(), _ccls=first._get();
      scluster = ccls.VectorString(xtarget);
      _scluster = _ccls.VectorString(xtarget);
      string re=EasyExpSynthesis(scluster,_scluster,mode,LIST_LIMIT);
      ccls[0].let(xregexp,re);
    }
  }
  else {
    for(int j=0;j<size();j++) scluster.push_back(get(j)[xtarget].tostring());
    string re=EasyExpSynthesis(scluster,_scluster,mode,LIST_LIMIT);
    get(0).let(xregexp,re);
  }
  getkey().let(xregexp,"regexp");
  return(0);
}
#endif

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
int Collection::showfile(int x) {
#ifdef G__WIN32
  string command = "type ";
#else
  string command = "cat ";
#endif
  string com;
  for(int i=0;i<size();i++) {
    com = command + get(i)[x].tostring() ;
#ifdef G__WIN32
    char buf[1000]; strcpy(buf,com.c_str());
    char *p;
    while((p=strchr(buf,'/'))) *p='\\';
    printf("%s\n",buf);
    system(buf);
#else
    printf("%s\n",com.c_str());
    system(com.c_str());
#endif
  }
  return(0);
}

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

#include <_TF_IDF.h>

#endif



