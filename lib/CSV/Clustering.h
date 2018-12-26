/***************************************************************************
* Clustering.h
*
***************************************************************************/
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
#ifndef _WIN32
#define NOMINMAX
#endif
#include "Collection.h"
extern int CLDEBUG;
extern double NEGATIVEDICAV;

#define CSVCLUSTERING_MULTI_THREAD  
//#define CSVCLUSTERINGCOLUMN_MULTI_THREAD // no speed gain
#ifdef CSVCLUSTERING_MULTI_THREAD
////////////////////////////////////////////////////////////////////////////
extern unsigned int CSVClustering_num_threads ;
extern unsigned int CSVClustering_num_threads_limit ;
extern unsigned int CSVClusteringColumn_num_threads_limit ;
extern unsigned int CSVClustering_thread_timeout ;
#endif

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
typedef double (*SimilarityFunction)(const char*,const char*);
typedef Collection (*ClusteringAlgo)(Collection&,SimilarityFunction,int,const vector<double>&);

////////////////////////////////////////////////////////////////////////
Collection makeMatrix(const Collection& ci) ;
Collection MakeSimilarityMatrixColumn(Collection& cx,int j,SimilarityFunction f,unsigned int& done) ;
Collection MakeSimilarityMatrix(Collection& cx,SimilarityFunction f) ;

Collection Clustering_core1(Collection& cx,SimilarityFunction f,int clsin,const vector<double>& arg);

void SortSimilarityMatrixByColumn(const Collection& SimilarityMatrix,int xcls
		,Collection& cname, Collection& cinnprod) ;
void CopyClassification(map<string,int>& nametoclass,Collection& c,int xcls) ;
int ClassificationByIndex(const Collection& cinnprod,const Collection& cname
		,const vector<int>& index		,map<string,int>& nametoclass) ;
statistics StatisticsToIndex(const Collection& cinnprod
		,const vector<int>& index) ;
statistics StatisticsAtIndex(const Collection& cinnprod
		,const vector<int>& index) ;
map<int,statistics> StatisticsByClass(const Collection& cinnprod
		,const Collection& cname
		,const vector<int>& index
	        ,map<string,int>& nametoclass) ;
void SetInitialIndex(const Collection& cinnprod,int xcls,int order,vector<int>& index,double thresh=0.9) ;
void UpdateIndexByStat(const Collection& cinnprod
			,int xcls
			,const statistics& st,double thresh
			,vector<int>& index,int pol=0) ;
void UpdateIndexByClassStat(const Collection& cname
			,const Collection& cinnprod,int xcls
			,map<int,statistics>& stc
			,map<string,int>& nametoclass,double thresh
			,vector<int>& index,int pol=0) ;
void UpdateIndexByClassStat(const Collection& cname
			,const Collection& cinnprod,int xcls
			,map<int,statistics>& stc
			,map<string,int>& nametoclass
			,const vector<double>& thresh
			,vector<int>& index,int pol=0) ;
Collection Clustering_core2(Collection& cx,SimilarityFunction f,int clsin,const vector<double>& arg);

Collection Clustering_core3(Collection& cx,SimilarityFunction f,int clsin,const vector<double>& arg);
Collection Clustering_core4(Collection& cx,SimilarityFunction f,int clsin,const vector<double>& arg) ;
double GapRatio(const Collection& cname,const Collection& csim
		,const vector<int>& index
		,map<string,int>& nametoclass,int xcls,int ncls) ;
Collection Clustering_core5(Collection& cx,SimilarityFunction f,int clsin,const vector<double>& arg) ;
Collection Clustering_core6(Collection& cx,SimilarityFunction f,int clsin,const vector<double>& arg) ;
Collection Clustering_core7(Collection& cx,SimilarityFunction f,int clsin,const vector<double>& arg) ;
Collection Clustering_core8(Collection& cx,SimilarityFunction f,int clsin,const vector<double>& arg) ;

Collection Clustering(const Collection& ci,ClusteringAlgo algo,SimilarityFunction f,const vector<double>& arg);
Collection SparseSampling(const Collection& ci,SimilarityFunction f,int iter=1);

Collection ClusteringConflict_core1(Collection& cx,SimilarityFunction f,int clsin,const vector<double>& arg) ;
Collection ClusteringConflict_core2(Collection& cx,SimilarityFunction f,int clsin,const vector<double>& arg) ;
Collection ClusteringConflict(const Collection& ci,ClusteringAlgo algo,SimilarityFunction f,const vector<double>& arg) ;

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
#if 0
extern char* InterpretedMatchFunc;
double interpretmatchratio(const char* a,const char* b) ;
#endif
////////////////////////////////////////////////////////////////////////////
struct CombinationMatch_t {
  SimilarityFunction  fa;
  SimilarityFunction  fb;
  double          ratioa;
  double          ratiob;
};
extern CombinationMatch_t CombinationMatch;

double combinationmatchratio(const char* a,const char* b) ;
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
int charmatchcount(const char* a,const char* b) ;
double charmatchratio(const char* a,const char* b) ;

////////////////////////////////////////////////////////////////////////////
int headmatchcount(const char* ain,const char* bin) ;
double headmatchratio(const char* ain,const char* bin) ;

////////////////////////////////////////////////////////////////////////////
double wordmatchcount(const char* a,const char* b) ;
double wordmatchratio(const char* a,const char* b) ;

////////////////////////////////////////////////////////////////////////////
double EuclideanDistanceRatio(const char* a,const char* b) ;
////////////////////////////////////////////////////////////////////////////
void EasyExpExtraction(Collection& cx,int x,int xcls,int xregex,int mode=0,int LIST_LIMIT=100) ;
inline void RegExpExtraction(Collection& cx,int x,int xcls,int xregex,int mode=0,int LIST_LIMIT=100)
{ EasyExpExtraction(cx,x,xcls,xregex,mode,LIST_LIMIT);}
void StatEasyExpExtraction(Collection& cx,int x,int xcls,int xregex,int mode=0,int LIST_LIMIT=100) ;

Collection Clustering1(const Collection& ci,int x,double thresh,double percentage=0.0,SimilarityFunction f=wordmatchratio) ;
Collection Clustering1(const Collection& ci,const vector<int>& x,double thresh,double percentage=0.0,SimilarityFunction f=EuclideanDistanceRatio) ;
Collection Clustering8(const Collection& ci,int x,int nmin,int nmax,SimilarityFunction f=wordmatchratio) ;
Collection Clustering8(const Collection& ci,const vector<int>& x,int lowlim ,int uplim,SimilarityFunction f=EuclideanDistanceRatio) ;

Collection StringClustering(Collection& c,int x,double thresh=0.5,const char* mode="WORD",int regexpmode=0,int regexpLimit=100) ; 
