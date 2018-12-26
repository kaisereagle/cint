
#include "Collection.h"
#include <string>
#include <deque>
#include <map>

#ifndef G__ROOT

template<class Container> 
class AutoContainer : public Container {
 public:
  ~AutoContainer() {
#ifndef __GNUC__
    Container::reverse_iterator rfirst=rbegin();
    Container::reverse_iterator rlast =rend();
    while(rfirst!=rlast) {
      delete (*rfirst);
      rfirst++;
    }
#endif
  }
};

template<class Key,class T> 
class AutoMap : public map<Key,T> {
 public:
  ~AutoMap() {
#ifndef __GNUC__
    map<Key,T>::reverse_iterator rfirst=rbegin();
    map<Key,T>::reverse_iterator rlast =rend();
    while(rfirst!=rlast) {
      delete (*rfirst).second;
      rfirst++;
    }
#endif
  }
};

#ifdef __MAKECINT__
#endif

#endif

#if defined(__MAKECINT__) || defined(G__ROOT)
#pragma link off all classes;

#pragma link C++ class string-;
#pragma link C++ class EasyExp-;
#pragma link C++ class MatchElement-;
#pragma link C++ class StatEasyExp-;
#pragma link C++ class vector<EasyExp>;
#ifdef E__REPLACE
//#pragma link C++ class EasyExpReplace-;
#endif
#pragma link C++ class ReadF-;
//#pragma link C++ class vector<const char*>-;
#pragma link C++ class G__MultiByteMode-;
// Cannot fix TCollectionProxyInfo and ROOT::Destruct problem with deque
#pragma link C++ class deque<Sheet>-;
#pragma link C++ class deque<Collection>-;
#pragma link C++ class deque<value>-;
//#pragma link C++ class map<string,Sheet>-;
#pragma link C++ class map<string,Collection>-;
#pragma link C++ class map<string,Collection>::iterator-;
#pragma link C++ function operator==(const map<string,Collection>::iterator,const map<string,Collection>::iterator);
#pragma link C++ function operator!=(const map<string,Collection>::iterator,const map<string,Collection>::iterator);

#pragma link C++ class FileStrCollection-;

#pragma link C++ class Sheet-;
#pragma link C++ class Collection-;
#pragma link C++ class CollectionIterator-;
#pragma link C++ class EachValue-;
//#pragma link C++ class StrCollection-;
#pragma link C++ class CompareLine-;
#pragma link C++ class Line-;
#pragma link C++ class statistics-;
#pragma link C++ class DStat-;
#pragma link C++ class FStat-;
#pragma link C++ class CollectionStatistics-;
#pragma link C++ class value-;
#pragma link C++ class EasyExp::Mode-;
#pragma link C++ class StatEasyExp::Mode-;
#pragma link C++ class CompareLine::mode_t-;
#pragma link C++ class Scandir-;

#pragma link C++ class TF_IDF;
#pragma link C++ class TermFrequency;
#pragma link C++ class TermFrequencyTable;
#pragma link C++ class DocumentFrequency;
#pragma link C++ class InverseDocumentFrequencyTable;
#pragma link C++ class InverseDocumentFrequencyTable;
#pragma link C++ class map<string,DocumentFrequency*>;
#pragma link C++ class map<string,DocumentFrequency*>::iterator;
#pragma link C++ class map<string,DocumentFrequency*>::value_type;
#pragma link C++ class map<string,TermFrequency*>;
#pragma link C++ class map<string,TermFrequency*>::iterator;
#pragma link C++ class map<string,TermFrequency*>::value_type;

//#pragma link C++ function G__ateval(const Sheet& a);
//#pragma link C++ function G__ateval(const Collection& a);
#pragma link C++ function G__ateval(const DStat&);
#pragma link C++ function G__ateval(const FStat&);
#pragma link C++ function operator<<(ostream&,DStat&);
#pragma link C++ function operator<<(ostream&,FStat&);
#pragma link C++ function operator==(const DStat&,const DStat&);
#pragma link C++ function operator!=(const DStat&,const DStat&);
#pragma link C++ function operator< (const DStat&,const DStat&);
#pragma link C++ function operator==(const FStat&,const FStat&);
#pragma link C++ function operator!=(const FStat&,const FStat&);
#pragma link C++ function operator< (const FStat&,const FStat&);

#pragma link off function operator<<(ostream&,void*);


#pragma link C++ function EX_fopen;
#pragma link C++ function EX_fclose;
#pragma link C++ function EX_fprintf;
#pragma link C++ function EX_printf;
#pragma link C++ function isdbl;
#pragma link C++ function isint;

//#pragma link C++ function readuntil;
//#pragma link C++ function skipuntil;
//#pragma link C++ function skipline;

#pragma link C++ function operator+(const Collection&,const Collection&);
#pragma link C++ function operator-(const Collection&,const Collection&);
#pragma link C++ function operator*(const Collection&,const Collection&);
#pragma link C++ function operator/(const Collection&,const Collection&);

#pragma link C++ function operator+(const Line&,const Line&);
#pragma link C++ function operator-(const Line&,const Line&);
#pragma link C++ function operator*(const Line&,const Line&);
#pragma link C++ function operator/(const Line&,const Line&);

/////////////////////////////////////////////////////////////////////////
// Clustering
/////////////////////////////////////////////////////////////////////////


#pragma link C++ global  InterpretedMatchFunc;
#pragma link C++ function interpretmatchratio;
#pragma link C++ struct CombinationMatch_t;
#pragma link C++ global  CombinationMatch;
#pragma link C++ function combinationmatchratio;
#pragma link C++ function charmatchcount;
#pragma link C++ function charmatchratio;
#pragma link C++ function headmatchcount;
#pragma link C++ function headmatchratio;
#pragma link C++ function wordmatchcount;
#pragma link C++ function wordmatchratio;

#pragma link C++ typedef SimilarityFunction;

#pragma link C++ typedef ClusteringAlgo;
#pragma link C++ function makeMatrix;
#pragma link C++ function Clustering_core1;
#pragma link C++ function Clustering_core2;
#pragma link C++ function Clustering_core3;
#pragma link C++ function Clustering_core4;
#pragma link C++ function Clustering_core5;
#pragma link C++ function Clustering_core6;
#pragma link C++ function Clustering_core8;
#pragma link C++ function Clustering;
#pragma link C++ function Clustering1;
#pragma link C++ function Clustering8;
#pragma link C++ function ClusteringConflict_core1;
#pragma link C++ function ClusteringConflict_core2;
#pragma link C++ function ClusteringConflict;
#pragma link C++ function SparseSampling;
#pragma link C++ function MakeSimilarityMatrix;
#pragma link C++ function SortSimilarityMatrixByColumn;
#pragma link C++ function SetInitialIndex;
#pragma link C++ function StatisticsAtIndex;
#pragma link C++ function StatisticsToIndex;
#pragma link C++ function UpdateIndexByStat;
#pragma link C++ function UpdateIndexByClassStat;
#pragma link C++ function ClassificationByIndex;
#pragma link C++ function RegExpExtraction;
#pragma link C++ global   CLDEBUG;
#pragma link C++ global   NEGATIVEDICAV;
#pragma link C++ function StringClustering;

#pragma link C++ function compare;
#pragma link C++ function alignCollections;
#pragma link C++ function RegExpExtraction;
#pragma link C++ function EasyExpExtraction;
#pragma link C++ function EasyExpSynthesis;
#pragma link C++ function EasyExpStatSynthesis;
#pragma link C++ function StatEasyExpExtraction;
#pragma link C++ function StatEasyExpSynthesis;
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
// icsv.h
#pragma link C++ class ClassifyTable;
#pragma link C++ class deque<ClassifyTable*>;
#pragma link C++ function ColumnToDVector;
#pragma link C++ function RowToDVector;

#pragma link C++ class PatternToSymbol;
#pragma link C++ class EasyExpTable;
#pragma link C++ global CellPattern;
#pragma link C++ global RowColPattern;
//#pragma link C++ function ClassifyNumericVector;
//#pragma link C++ global PLOTMODE;
//#pragma link C++ global VIEWMODE;
//#pragma link C++ global DISPMODE;
//#pragma link C++ global PLOTEACHMODE;

/////////////////////////////////////////////////////////////////////////
// csviostream.h
#pragma link C++ class csvostream;
#pragma link C++ global draw;
#pragma link C++ global drawNN;

#pragma link C++ function cintgraph_command;
#pragma link C++ function cintgraph_clear;
#pragma link C++ function cintgraph_stop;
#pragma link C++ function cintgraph_exit;
#pragma link C++ function cintgraph_start;

#pragma link C++ function cintgraph_mode;
#pragma link C++ function gnuplot_mode;
#pragma link C++ function graph_mode;

#pragma link C++ global LOG;
#pragma link C++ global LIN;

/////////////////////////////////////////////////////////////////////////
//#pragma link C++ class Vector<double>;
//#pragma link C++ class Matrix<double>;
#pragma link C++ class DVector;
#pragma link C++ class DMatrix;
#pragma link C++ class FVector;
#pragma link C++ class FMatrix;

#pragma link C++ class COLOR;
#pragma link C++ class Vectorostream ;
#pragma link C++ global plot;
#pragma link C++ global plotNN;
#pragma link C++ global sameNN;
#pragma link C++ global NNW_Vector;
#pragma link C++ global NNW_Matrix;
#pragma link C++ global ClassifyTableProgress;

#pragma link C++ class Matrixostream ;
#pragma link C++ global splot;

///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////

#pragma link C++ function randgauss(double,double);

///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
#pragma link C++ global  EasyExp_DEBUG;

///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
#ifdef EASYEXP_MULTI_THREAD
#pragma link C++ global  EasyExp_num_threads;
#pragma link C++ global  EasyExp_num_threads_limit;
#pragma link C++ global  EasyExp_thread_timeout;
#endif

#ifdef CSVCLUSTERING_MULTI_THREAD
#pragma link C++ global  CSVClustering_num_threads;
#pragma link C++ global  CSVClustering_num_threads_limit;
#pragma link C++ global  CSVClusteringColumn_num_threads_limit;
#pragma link C++ global  CSVClustering_thread_timeout;
#endif

#endif
