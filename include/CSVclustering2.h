/*****************************************************************************
 * CSVclustering2.h
 *  Description: 
 *    CSV table format manipulator optional library for incremental clustering.
 *  Status: Experimental
 *  Date: 2016/Jan/4
 *  Author: Masaharu Goto
 *****************************************************************************/
/************************************************************************
 * Copyright(c) 1995~2016  Masaharu Goto (gotom@hanno.jp)
 *
 * Permission to use, copy, modify and distribute this software and its 
 * documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation. The author makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 ************************************************************************/

#ifndef CSVCLUSTERING2_H
#define CSVCLUSTERING2_H
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
inline vector<int> vint(int a0=-1,int a1=-1,int a2=-1,int a3=-1 ,int a4=-1
			,int a5=-1 ,int a6=-1 ,int a7=-1 ,int a8=-1 ,int a9=-1) {
  vector<int> r;
  if(a0>=0) r.push_back(a0);
  if(a1>=0) r.push_back(a1);
  if(a2>=0) r.push_back(a2);
  if(a3>=0) r.push_back(a3);
  if(a4>=0) r.push_back(a4);
  if(a5>=0) r.push_back(a5);
  if(a6>=0) r.push_back(a6);
  if(a7>=0) r.push_back(a7);
  if(a8>=0) r.push_back(a8);
  if(a9>=0) r.push_back(a9);
  return(r);
}
/////////////////////////////////////////////////////////////////////////////

int RepresentativeData = 200;
int RepresentativeDataLimit = 400;

/////////////////////////////////////////////////////////////////////////////
// TF_IDF object creation
/////////////////////////////////////////////////////////////////////////////
template<class T>
int Collection::TF_IDF(TF_IDF& tfidf,const vector<int>& xtarget
		       ,const vector<int>& xpartition
		       ,ReadF& p
		       ,T& e // EasyExp or vector<EasyExp>
		       ,bool single=false) {
  if(single) tfidf.setSingleCount();

  // accumulate term count 
  if(xpartition.size()==0) {
    // every line as clustering item
    char name[80];
    for(int i=0;i<size();i++) {
      sprintf(name,"%d",i); // line number as id
      for(int j=0;j<xtarget.size();j++) {
	string str=get(i)[xtarget[j]].tostring();
	//printf("%d %d %s %s\n",i,j,name,str.c_str());
	tfidf.add(name,str,p,e);
      }
    }
  }
  else {
    // unique conbination of xpartition columns as clustering item
    CollectionIterator first=begin(xpartition),last=end(first);
    string separator="#";
    string id;
    for(;first!=last;++first) {
      Collection cpartition = *first;
      id = first.id(separator); // partition columns contents as id
      for(int i=0;i<cpartition.size();i++) {
	for(int j=0;j<xtarget.size();j++) {
	  string str=cpartition[i][xtarget[j]].tostring();
	  //printf("XXX %d %d %s %s\n",i,j,id.c_str(),str.c_str());
	  tfidf.add(id,str,p,e);
	}
      }
    }
  }
  return(0);
}

#ifdef IC_DATASET_H
/////////////////////////////////////////////////////////////////////////////
// IC_DataSet object creation
/////////////////////////////////////////////////////////////////////////////
int Collection::IC_DataSet(IC_DataSetFSV& vecset,TF_IDF& tfidf
			   ,void* simfunc=SparseNormalizedInnerProduct
			   ,int Representative=200) {
  vecset.setKeepOriginalData(true);
  vecset.setRepresentativeDataSize(RepresentativeData);
  vecset.setRepresentativeDataSizeLimit(RepresentativeDataLimit);
  vecset.setSimfunc(simfunc);

  // retrieve doc and TFIDF sparse vector from TF_IDF object
  vector<string> vdoc;
  vector<FSparseVector> vsv; // SparseVector<termindex,TFIDF>
  tfidf.TFIDF_DocVector(vdoc,vsv);

  // set TF_IDF to IC_DataSet object
  for(int i=0;i<vdoc.size();i++) vecset.add(vdoc[i],vsv[i]);
  return(0);
}
/////////////////////////////////////////////////////////////////////////////
int Collection::IC_DataSet(IC_DataSetS& strset,int xfrom,int xto
			   ,void* simfunc=stringmatchratio
			   ,int Representative=200) {
  strset.setKeepOriginalData(true);
  strset.setRepresentativeDataSize(Representative);
  strset.setRepresentativeDataSizeLimit(Representative*2);
  strset.setSimfunc(Ssimfunc);

  // set string to IC_DataSet object
  string s;
  if(xto<xfrom) xto=getkey().size()-1;
  for(int i=0;i<size();i++) {
    s = get(i)[xfrom].tostring;
    for(int j=xfrom+1;j<=xto;j++) {
      s += " ";
      s += get(i)[j].tostring();
    }
    vecset.add(i,s);
  }

  return(0);
}

/////////////////////////////////////////////////////////////////////////////
int Collection::IC_DataSet(IC_DataSetFV& vecset,int xfrom,int xto
			   ,void* simfunc=NormalizedInnerProduct
			   ,int Representative=200) {
  vecset.setKeepOriginalData(true);
  vecset.setRepresentativeDataSize(RepresentativeData);
  vecset.setRepresentativeDataSizeLimit(RepresentativeDataLimit);
  vecset.setSimfunc(simfunc);

  // set vector to IC_DataSet object
  FVector d; 
  if(xto<xfrom) xto=getkey().size()-1;
  for(int i=0;i<size();i++) {
    d.clear();
    for(int j=xfrom;j<=xto;j++) d.push_back(get(i)[j].tofloat());
    vecset.add(i,d);
  }
  return(0);
}

/////////////////////////////////////////////////////////////////////////////
// clustering
/////////////////////////////////////////////////////////////////////////////
template<class T>
int Collection::clustering(T& dataset,double criteria=0.7,int xcls=-1) {
  if(criteria<1.0) {
    NN_REAL thresh = criteria;
    dataset.clustering(thresh,IC_AVERAGE);
  }
  else {
    int CLST = criteria;
    dataset.clustering(CLST,CLST*2,IC_AVERAGE);
  }
  if(xcls<0) xcls=getx("cls"); 
  if(xcls<0) {
    xcls=getkey().size();
    getkey().let(xcls,"cls");
  }
  for(int i=0;i<size();i++) get(i).let(xcls,dataset.getDataCluster(i));
  return(0);
}

/////////////////////////////////////////////////////////////////////////////
template<class T>
int Collection::clustering(T& dataset,const vector<int>& xpartition
			   ,double criteria=0.7,int xcls=-1) {
  if(criteria<1.0) {
    NN_REAL thresh = criteria;
    dataset.clustering(thresh,IC_AVERAGE);
  }
  else {
    int CLST = criteria;
    dataset.clustering(CLST,CLST*2,IC_AVERAGE);
  }

  if(xcls<0) xcls=getx("cls"); 
  if(xcls<0) {
    xcls=getkey().size();
    getkey().let(xcls,"cls");
  }

  // copy cluster id to cls column 
  if(xpartition.size()==0) {
    for(int i=0;i<dataset.size();i++) {
      get(atoi(dataset.getDataName(i).c_str())).let(xcls,dataset.getDataCluster(i));
      // maybe this is just fine
      //get(i).let(xcls,dataset.getDataCluster(i));
    }
  }
  else {
    string separator="#";
    ReadF p; p.setseparator(separator);
    for(int i=0;i<dataset.size();i++) {
      vector<string> exprs = p.separatetoken(dataset.getDataName(i));
      Collection citem = match(xpartition,exprs);
      citem.let(xcls,dataset.getDataCluster(i));
    }
  }

  return(0);
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
Collection Collection::TFIDF_ClusterMean_TermCollection(IC_DataSetFSV& dataset
							,TF_IDF& tfidf) {
  ////////////////////////////////////////////////////////
  // Calculate TFIDF average for each cluster
  // term,cls0 ,cls1,...,clsn
  // t1  ,tfidf,...
  Collection cterm;
  Collection cdoc = tfidf.TFIDF_DocCollection();
  cdoc.getkey().let(0,"cls");
  for(int i=0;i<cdoc.size();i++) cdoc[i].let(0,dataset.getDataCluster(i));
  Collection ctest, ccluster;
  Line l;
  ccluster.setkey(cdoc.getkey());
  CollectionIterator first=cdoc.begin(0),last=cdoc.end(first);
  for(;first!=last;++first) {
    ctest = *first;
    //string clsid = first.id();
    //l.let(0,clsid);
    CollectionStatistics& cstat = ctest.stat();
    for(i=0;i<cstat.size();i++) l.let(i,cstat[i].mean());
    ccluster.push_back(l);
  }
  cterm = ccluster.rotate(1);

  return(cterm);
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
Collection Collection::TF_ClusterMean_TermCollection(IC_DataSetFSV& dataset
							,TF_IDF& tfidf) {
  ////////////////////////////////////////////////////////
  // Calculate TFIDF average for each cluster
  // term,cls0 ,cls1,...,clsn
  // t1  ,tfidf,...
  Collection cterm;
  Collection cdoc = tfidf.TF_DocCollection();
  cdoc.getkey().let(0,"cls");
  for(int i=0;i<cdoc.size();i++) cdoc[i].let(0,dataset.getDataCluster(i));
  Collection ctest, ccluster;
  Line l;
  ccluster.setkey(cdoc.getkey());
  CollectionIterator first=cdoc.begin(0),last=cdoc.end(first);
  for(;first!=last;++first) {
    ctest = *first;
    //string clsid = first.id();
    //l.let(0,clsid);
    CollectionStatistics& cstat = ctest.stat();
    for(i=0;i<cstat.size();i++) l.let(i,cstat[i].mean());
    ccluster.push_back(l);
  }
  cterm = ccluster.rotate(1);

  return(cterm);
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
Collection Collection::TC_ClusterMean_TermCollection(IC_DataSetFSV& dataset
							,TF_IDF& tfidf) {
  ////////////////////////////////////////////////////////
  // Calculate TFIDF average for each cluster
  // term,cls0 ,cls1,...,clsn
  // t1  ,tfidf,...
  Collection cterm;
  Collection cdoc = tfidf.TC_DocCollection();
  cdoc.getkey().let(0,"cls");
  for(int i=0;i<cdoc.size();i++) cdoc[i].let(0,dataset.getDataCluster(i));
  Collection ctest, ccluster;
  Line l;
  ccluster.setkey(cdoc.getkey());
  CollectionIterator first=cdoc.begin(0),last=cdoc.end(first);
  for(;first!=last;++first) {
    ctest = *first;
    //string clsid = first.id();
    //l.let(0,clsid);
    CollectionStatistics& cstat = ctest.stat();
    for(i=0;i<cstat.size();i++) l.let(i,cstat[i].mean());
    ccluster.push_back(l);
  }
  cterm = ccluster.rotate(1);

  return(cterm);
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// Legacy functions
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
int Collection::clustering(int x,int xcls,double criteria=0.7,void* simfunc=stringmatchratio) {
  return(Collection::clustering(x,x,xcls,criteria,simfunc));
}

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

  if(Ssimfunc) {
    IC_DataSetS strset;
    IC_DataSet(strset,xfrom,xto,Ssimfunc,RepresentativeData);
    clustering(strset,criteria,xcls)
  }
  else {
    IC_DataSetFV vecset;
    IC_DataSet(vecset,xfrom,xto,Ssimfunc,RepresentativeData);
    clustering(vecset,criteria,xcls)
  }
  return(0);
}

/////////////////////////////////////////////////////////////////////////////
// Multiple column string clustering with TFIDF
/////////////////////////////////////////////////////////////////////////////
// Using dense vector in a Collection. development purpose only
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
int Collection::TFIDFclustering(int xfrom,int xto,int xcls,double criteria) {
  vector<int>& xtarget, xpartition;
  if(xto<xfrom) xto=getkey().size()-1;
  for(int i=xfrom;i<=xto;i++) xtarget.push_back(x);
  TF_IDF tfidf;
  IC_DataSetFSV dataset;
  TF_IDF(tfidf,xtarget,xpartition,p,e);
  IC_DataSet(dataset,tfidf);
  clustering(dataset,xpartition,criteria,xcls);
  return(0);
}

/////////////////////////////////////////////////////////////////////////////
template<class T>
Collection Collection::TFIDFclustering(const vector<int>& xtarget
				       ,const vector<int>& xpartition
				       ,int xcls
				       ,double criteria
				       ,ReadF& p
				       ,T& e // EasyExp or vector<EasyExp>
				       ,bool single=false) {
  TF_IDF tfidf;
  IC_DataSetFSV dataset;
  TF_IDF(tfidf,xtarget,xpartition,p,e);
  IC_DataSet(dataset,tfidf);
  clustering(dataset,xpartition,criteria,xcls);
  Collection cterm = TFIDF_ClusterMean_TermCollection(dataset,tfidf);
  return(cterm);
}

#endif // IC_DATASET_H

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// EasyExp Synthesis
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
int Collection::EasyExpSynthesis(int xtarget,int xregexp
				 ,const vector<int>& xcls 
				 ,int mode=0,int LIST_LIMIT=20) {
  if(xregexp<0) xregexp=getkey().size();
  vector<string>  scluster, _scluster; 
  if(xcls!=T()) { // <<< this line couldn't be generalized
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
  if(xcls>=0) { // <<< this line couldn't be generalized
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



