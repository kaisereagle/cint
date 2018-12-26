/*****************************************************************************
 * icsv.h
 *  Description: Intelligent CSV file handler
 *  Status: Experimental
 *  Date: 2015/Jan/3
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

#ifndef ICSV_H
#define ICSV_H

#if defined(__CINT__) && !defined(__MAKECINT__)
#include <CSV.h>
//#include <NVector.h>
#else
#include "Collection.h"
#include "Stat.h"
#endif
#ifndef _WIN32
#include <unistd.h>
#endif
#include <deque>
using namespace std;


#define PLOTMODE      0x01
#define PLOTEACHMODE  0x08
#define VIEWMODE      0x02
#define DISPMODE      0x04
#define DEBUGMODE     0x10

extern string NNW_Vector;
extern string NNW_Matrix;

extern string ClassifyTableProgress;

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
class ClassifyTable {
 private:
 public:

  string  m_fname; // to be filled
  string  m_name; // to be filled

  Collection  m_original;
  // position in parent table
  int m_colmin;
  int m_colmax;
  int m_rowmin;
  int m_rowmax;

  Collection m_cellid;
  Collection m_rowid;
  Collection m_colid;

  statistics m_colstat; // column length statistics
  statistics m_rowstat; // row    length statistics
  deque<ClassifyTable*> p_subtable;

  ClassifyTable* m_top;

  int m_debugmode;

 public:
  /////////////////////////////////////////////////////////////
  ClassifyTable() ;
  ClassifyTable(const ClassifyTable& a) ;
  ClassifyTable(const Collection& c,ClassifyTable* top
		   ,const string& fname,const string& name)
    {setOriginal(c,top,fname,name);}
  ~ClassifyTable() ;

  ClassifyTable operator=(const ClassifyTable& a) ;

  /////////////////////////////////////////////////////////////
  void clear() ;

  /////////////////////////////////////////////////////////////
  // analysis main entries
  void setOriginal(const Collection& c,ClassifyTable* top
		   ,const string& fname,const string& name) ;
  int Classification(int hier=1) ;
  int ClassificationEach(int rowmin=-1,int rowmax=-1,int colmin=-1,int colmax=-1);
  Collection DetailClassification() {return(DetailClassificationTop());}

  /////////////////////////////////////////////////////////////
  // subroutines

  // EasyExp pattern matching
  int CellClassification();
  int RowClassification() ;
  int ColClassification() ;

  // Table division
  int DivideSubTable() ;
  int DivideSubTableWRowClustering();
  int DivideSubTableWColClustering();
  int AddSubTable(const ClassifyTable* psub);

  // data classification
  int NNClassification(Collection& r
		       ,const string& fileexpr,const string& nnexpr
		       ,const string& targetobj="*",const string& targetprop="");
  Collection DetailClassificationTop() ;
  Collection DetailClassificationEach(int tableid) ;

  // display result
  int DisplayResult(Collection& r,const string& scriptout="") ;

  /////////////////////////////////////////////////////////////
  void setdebugmode(int mode) { m_debugmode |= mode; }
  void setplotmode(int mode) {if(mode)m_debugmode|=PLOTMODE;else m_debugmode&=0xe;}
  void setploteachmode(int mode) {if(mode)m_debugmode|=PLOTEACHMODE;else m_debugmode&=0x7;}
  void setviewmode(int mode) {if(mode)m_debugmode|=VIEWMODE;else m_debugmode&=0xd;}
  void setdispmode(int mode) {if(mode)m_debugmode|=DISPMODE;else m_debugmode&=0xb;}
  /////////////////////////////////////////////////////////////
  void dispProp() const;
  void disp(int limit=10) const;
  void dispHier(int limit=10) const;
#if 0
  void saveNNDB(const char* fname=0,const char* mode="w");
#endif
};


inline bool operator<(const ClassifyTable& a,const ClassifyTable& b) { return(true); }
inline bool operator!=(const ClassifyTable& a,const ClassifyTable& b) {
  if(a.m_fname==b.m_fname
     && a.m_colmin==b.m_colmin && a.m_colmax==b.m_colmax 
     && a.m_rowmin==b.m_rowmin && a.m_rowmax==b.m_rowmax) return(false);
  else return(true);
}
inline bool operator==(const ClassifyTable& a,const ClassifyTable& b) {
  if(a.m_fname==b.m_fname
     && a.m_colmin==b.m_colmin && a.m_colmax==b.m_colmax 
     && a.m_rowmin==b.m_rowmin && a.m_rowmax==b.m_rowmax) return(true);
  else return(false);
}

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
DVector ColumnToDVector(Collection& c1,int x,int yoffset=0) ;
DVector RowToDVector(Collection& c1,int y,int xoffset=0) ;

//int ClassifyNumericVector(Collection& c,ClassifyTable& CC) ;

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
// Data Structure and utility class
/////////////////////////////////////////////////////////////////////////
struct EasyExpTable {const char* expr;  const char* symbol;};

/////////////////////////////////////////////////////////////////////////
class PatternToSymbol {
 public:
  deque<EasyExp*> p_easyexp;
  deque<const char*>    p_symbol;
  PatternToSymbol(EasyExpTable* eet) {
    int i=0;
    while(eet[i].expr) {
      EasyExp* easyexp = new EasyExp(eet[i].expr);
      p_easyexp.push_back(easyexp);
      p_symbol.push_back(eet[i].symbol);
      ++i;
    }
  }
  const char* getsymbol(const char* pattern) {
    for(unsigned int i=0;i<p_easyexp.size();i++) {
      if(p_easyexp[i]->match(pattern)) return(p_symbol[i]);
    }
    return("");
  }
};
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
extern PatternToSymbol CellPattern;
extern PatternToSymbol RowColPattern;


#endif // ICSV_H

