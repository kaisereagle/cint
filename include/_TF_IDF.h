/*****************************************************************************
 * _TF_IDF.h
 *  Description: 
 *    Term Frequency - Inverse Document Frequncy library adapter
 *    Main body is defined in include/EasyExp.h
 *  Status: Production
 *  Date: 2015/Dec/29
 *  Author: Masaharu Goto
 *****************************************************************************/
/************************************************************************
 * Copyright(c) 1995~2016  Masaharu Goto (cint@pcroot.cern.ch)
 *
 * Permission to use, copy, modify and distribute this software and its 
 * documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation. The author makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 ************************************************************************/

/////////////////////////////////////////////////////////////////////////
// Output 
// 
// List of terms, docs
//   vector<string> getTerm() ;
//   vector<string> getDoc() ;
//
// TF
//   Collection TF_TermCollection();
//
// IDF
//   map<string,double> getIDF() ;
//
// TFIDF
//   Collection TFIDF_TermCollection();
//   Collection TFIDF_DocCollection();
//   int TFIDF_DocVector(vector<string>& vdoc,vector<FSparseVector>& vsv) ;
//   int TFIDF_TermVector(vector<string>& vterm,vector<FSparseVector>& vsv) ;
//
/////////////////////////////////////////////////////////////////////////

#ifndef _TF_IDF_H
#define _TF_IDF_H
/////////////////////////////////////////////////////////////////////////
// TF-IDF class
//    Collection, dense vector output
/////////////////////////////////////////////////////////////////////////
Collection TF_IDF::TFIDF_TermCollection() {
  Collection cx;
  Line l;
  int i;
  l.let(i=0,"term");
  l.let(++i,"idf");
  printf("calculating IDF...\n");
  TermFrequencyTable::iterator firstt,lastt=TF.end();
  for(firstt=TF.begin();firstt!=lastt;++firstt) {
    string doc = (*firstt).first;
    l.let(++i,doc);
  }
  cx.setkey(l);
  
  printf("calculating TF-IDF...\n");
  InverseDocumentFrequencyTable::iterator firstd,lastd=IDF.end();
  for(firstd=IDF.begin();firstd!=lastd;++firstd) {
    string term = (*firstd).first;
    double idf = IDF.idf(term);
    l.clear();
    l.let(i=0,term);
    l.let(++i,idf);
    lastt=TF.end();
    for(firstt=TF.begin();firstt!=lastt;++firstt) {
      string doc = (*firstt).first;
      double tf = TF.termfrequency(doc,term);
      double tfidf = tf*idf;
      l.let(++i,tfidf);
    }
    cx.push_back(l);
  }
  return(cx);
}

/////////////////////////////////////////////////////////////////////////
Collection TF_IDF::TFIDF_DocCollection() {
  Collection cx;
  Line l;
  int iterm=0;
  
  l.let(iterm=0,"doc");
  TermFrequencyTable::iterator firstt,lastt=TF.end();
  InverseDocumentFrequencyTable::iterator firstd,lastd=IDF.end();
  for(firstd=IDF.begin();firstd!=lastd;++firstd) {
    string term = (*firstd).first;
    l.let(++iterm,term);
  }
  cx.setkey(l);
  
  printf("calculating TF-IDF...\n");
  for(firstt=TF.begin();firstt!=lastt;++firstt) {
    string doc = (*firstt).first;
    l.clear();
    l.let(iterm=0,doc);
    lastd=IDF.end();
    for(firstd=IDF.begin();firstd!=lastd;++firstd) {
      string term = (*firstd).first;
      double idf = IDF.idf(term);
      double tf = TF.termfrequency(doc,term);
      double tfidf = tf*idf;
      l.let(++iterm,tfidf);
    }
    cx.push_back(l);
  }
  return(cx);
}

/////////////////////////////////////////////////////////////////////////
Collection TF_IDF::TF_TermCollection() {
  Collection cx;
  Line l;
  int i;
  l.let(i=0,"term");
  l.let(++i,"idf");
  TermFrequencyTable::iterator firstt,lastt=TF.end();
  for(firstt=TF.begin();firstt!=lastt;++firstt) {
    string doc = (*firstt).first;
    l.let(++i,doc);
  }
  cx.setkey(l);
  
  printf("calculating TF...\n");
  InverseDocumentFrequencyTable::iterator firstd,lastd=IDF.end();
  for(firstd=IDF.begin();firstd!=lastd;++firstd) {
    string term = (*firstd).first;
    double idf = IDF.idf(term);
    l.clear();
    l.let(i=0,term);
    l.let(++i,idf);
    lastt=TF.end();
    for(firstt=TF.begin();firstt!=lastt;++firstt) {
      string doc = (*firstt).first;
      double tf = TF.termfrequency(doc,term);
      l.let(++i,tf);
    }
    cx.push_back(l);
  }
  return(cx);
}

/////////////////////////////////////////////////////////////////////////
Collection TF_IDF::TC_TermCollection() {
  Collection cx;
  Line l;
  int i;
  l.let(i=0,"term");
  l.let(++i,"idf");
  TermFrequencyTable::iterator firstt,lastt=TF.end();
  for(firstt=TF.begin();firstt!=lastt;++firstt) {
    string doc = (*firstt).first;
    l.let(++i,doc);
  }
  cx.setkey(l);
  
  printf("calculating TF...\n");
  InverseDocumentFrequencyTable::iterator firstd,lastd=IDF.end();
  for(firstd=IDF.begin();firstd!=lastd;++firstd) {
    string term = (*firstd).first;
    double idf = IDF.idf(term);
    l.clear();
    l.let(i=0,term);
    l.let(++i,idf);
    lastt=TF.end();
    for(firstt=TF.begin();firstt!=lastt;++firstt) {
      string doc = (*firstt).first;
      int tc = TF.termccount(doc,term);
      l.let(++i,tc);
    }
    cx.push_back(l);
  }
  return(cx);
}
/////////////////////////////////////////////////////////////////////////
Collection TF_IDF::TF_DocCollection() {
  Collection cx;
  Line l;
  int iterm=0;
  
  l.let(iterm=0,"doc");
  TermFrequencyTable::iterator firstt,lastt=TF.end();
  InverseDocumentFrequencyTable::iterator firstd,lastd=IDF.end();
  for(firstd=IDF.begin();firstd!=lastd;++firstd) {
    string term = (*firstd).first;
    l.let(++iterm,term);
  }
  cx.setkey(l);
  
  printf("calculating TF...\n");
  for(firstt=TF.begin();firstt!=lastt;++firstt) {
    string doc = (*firstt).first;
    l.clear();
    l.let(iterm=0,doc);
    lastd=IDF.end();
    for(firstd=IDF.begin();firstd!=lastd;++firstd) {
      string term = (*firstd).first;
      double tf = TF.termfrequency(doc,term);
      l.let(++iterm,tf);
    }
    cx.push_back(l);
  }
  return(cx);
}
/////////////////////////////////////////////////////////////////////////
Collection TF_IDF::TC_DocCollection() {
  Collection cx;
  Line l;
  int iterm=0;
  
  l.let(iterm=0,"doc");
  TermFrequencyTable::iterator firstt,lastt=TF.end();
  InverseDocumentFrequencyTable::iterator firstd,lastd=IDF.end();
  for(firstd=IDF.begin();firstd!=lastd;++firstd) {
    string term = (*firstd).first;
    l.let(++iterm,term);
  }
  cx.setkey(l);
  
  printf("calculating TC...\n");
  for(firstt=TF.begin();firstt!=lastt;++firstt) {
    string doc = (*firstt).first;
    l.clear();
    l.let(iterm=0,doc);
    lastd=IDF.end();
    for(firstd=IDF.begin();firstd!=lastd;++firstd) {
      string term = (*firstd).first;
      int tc = TF.termcount(doc,term);
      l.let(++iterm,tc);
    }
    cx.push_back(l);
  }
  return(cx);
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
#ifdef IC_DATASET_H
Collection Collection::TFIDF_ClusterMean_TermCollection(TF_IDF& tfidf
						,IC_DataSetFSV& vecset) {
  ////////////////////////////////////////////////////////
  // Calculate TFIDF average for each cluster
  // term,cls0 ,cls1,...,clsn
  // t1  ,tfidf,...
  Collection cterm;
  Collection cdoc = tfidf.TFIDF_DocCollection();
  cdoc.getkey().let(0,"cls");
  for(i=0;i<cdoc.size();i++) cdoc[i].let(0,vecset.getDataCluster(i));
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
Collection Collection::TF_ClusterMean_TermCollection(TF_IDF& tfidf
						     ,IC_DataSetFSV& vecset) {
  ////////////////////////////////////////////////////////
  // Calculate TF average for each cluster
  // term,cls0 ,cls1,...,clsn
  // t1  ,tf,...
  Collection cterm;
  Collection cdoc = tfidf.TF_DocCollection();
  cdoc.getkey().let(0,"cls");
  for(i=0;i<cdoc.size();i++) cdoc[i].let(0,vecset.getDataCluster(i));
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
Collection Collection::TC_ClusterMean_TermCollection(TF_IDF& tfidf
						     ,IC_DataSetFSV& vecset) {
  ////////////////////////////////////////////////////////
  // Calculate TC average for each cluster
  // term,cls0 ,cls1,...,clsn
  // t1  ,tc,...
  Collection cterm;
  Collection cdoc = tfidf.TC_DocCollection();
  cdoc.getkey().let(0,"cls");
  for(i=0;i<cdoc.size();i++) cdoc[i].let(0,vecset.getDataCluster(i));
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


/////////////////////////////////////////////////////////////////////////
// TF-IDF class
//    SparseVector output
/////////////////////////////////////////////////////////////////////////
int TF_IDF::TFIDF_TermVector(vector<string>& vterm
			     ,vector<FSparseVector>& vsv) {
  vterm.clear(); vsv.clear();
  FSparseVector sv;
  int idoc=0,iterm=0;
  printf("calculating IDF...\n");
  printf("calculating TF-IDF...\n");
  TermFrequencyTable::iterator firstt,lastt=TF.end();
  InverseDocumentFrequencyTable::iterator firstd,lastd=IDF.end();
  for(firstd=IDF.begin();firstd!=lastd;++firstd) {
    string term = (*firstd).first;
    double idf = IDF.idf(term);
    vterm.push_back(term);
    idoc=0;
    sv.clear();
    sv.set(0,idf);
    lastt=TF.end();
    for(firstt=TF.begin();firstt!=lastt;++firstt) {
      string doc = (*firstt).first;
      double tf = TF.termfrequency(doc,term);
      double tfidf = tf*idf;
      sv.set(idoc,tfidf);
      ++idoc;
    }
    vsv.push_back(sv);
    ++iterm;
  }
  // term1, term1, ... termn
  // term2 sorted as string,  sparse vector
  // term3 sorted as string,  sparse vector
 
  return(0);
}
///////////////////////////////////////////////////////////
int TF_IDF::TFIDF_DocVector(vector<string>& vdoc,vector<FSparseVector>& vsv) {
  vdoc.clear(); vsv.clear();
  FSparseVector sv;
  int idoc=0,iterm=0;
  printf("calculating TF-IDF...\n");
  TermFrequencyTable::iterator firstt,lastt=TF.end();
  InverseDocumentFrequencyTable::iterator firstd,lastd=IDF.end();
  for(firstt=TF.begin();firstt!=lastt;++firstt) {
    string doc = (*firstt).first;
    vdoc.push_back(doc);
    lastd=IDF.end();
    iterm=0;
    sv.clear();
    for(firstd=IDF.begin();firstd!=lastd;++firstd) {
      string term = (*firstd).first;
      double idf = IDF.idf(term);
      //if(idf>0) { // this had only small gain
      double tf = TF.termfrequency(doc,term);
      double tfidf = tf*idf;
      //printf("iterm=%d val=%g\n",iterm,tfidf);
      sv.set(iterm,tfidf);
      ++iterm;
      //}
    }
    vsv.push_back(sv);
    ++idoc;
  }
  // doc, term1, ... termn
  // doc sorted as string,  sparse vector
  // doc sorted as string,  sparse vector
  return(0);
}
//////////////////////////////////////////////////////////
#endif // IC_DATASET_H

/////////////////////////////////////////////////////////////////////////
// End of TF-IDF class
/////////////////////////////////////////////////////////////////////////

#endif // _TF_IDF_H
