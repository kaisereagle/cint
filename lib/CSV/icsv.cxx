/*****************************************************************************
 * icsv.cxx
 *  Description: Intelligent CSV file handler
 *  Status: Experimental
 *  Date: 2014/Jan/3
 *  Author: Masaharu Goto  gotom@hanno.jp
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
//////////////////////////////////////////////////////////////////////////
// main function : $CINTSYSDIR/include/_icsv.cxx
//
// Output format of DetailClassificationTop()
//
//  obj      ,ID    ,prop    ,val,rowmin,rowmax,colmin,colmax,rowtitle,coltitle,classlabel,
//  Table    ,main  ,subtable,6  ,0     ,76    ,0     ,19,
//  Table    ,0     ,        ,   ,0     ,19    ,0     ,20,
//  matrix   ,1001  ,        ,   ,0     ,19    ,1     ,20    ,-1      ,0       ,matrix
//  Table    ,1     ,        ,   ,20    ,21    ,0     ,1,
//  Table    ,2     ,        ,   ,22    ,22    ,0     ,13,
//  Strvector,1002  ,row     ,22 ,22    ,22    ,0     ,13    ,-1      ,-1      ,
//  Table    ,3     ,        ,   ,23    ,74    ,0     ,10,
//  matrix   ,1003  ,        ,   ,24    ,74    ,0     ,10    ,23      ,-1      ,XYgraphsingle
//  Table    ,4     ,        ,   ,75    ,95    ,0     ,10,
//  Numvector,1004  ,column  ,0  ,76    ,95    ,0     ,10    ,75      ,-1      ,convexH
//  Table    ,5     ,        ,   ,76    ,76    ,0     ,1,
//
//////////////////////////////////////////////////////////////////////////

#include <string>
#include <sys/stat.h>
#include "icsv.h"
#include "EasyExp.h"
#include "Vector.h"
#include "Vectoriostream.h" // VirtualGraph.h and cintgraph.h are also included
#include "csviostream.h"    // histogram.h should be also included
#include "Clustering.h"

//DVector g_x,g_y;

string NNW_Vector = "T_*.nnw";
string NNW_Matrix = "M_*.nnw";

string ClassifyTableProgress;

/////////////////////////////////////////////////////////////////////////
// graphbuf directory for graphics and Neural Network pattern matching
/////////////////////////////////////////////////////////////////////////
#ifdef _WIN32
static string NNgraphbuf = string("c:/cint") + "/graphbuf";
#else
static string NNgraphbuf = string(getenv("CINTSYSDIR")) + "/graphbuf";
#endif

#define BUFSIZE  2000

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
// EasyExp matching patterns
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////
// Cell signatures
/////////////////////////////////////////////////////////////////////////
EasyExpTable CellExpr[] = {
// blank, space
 { ""                                                     , " " }, // blnak
 { "[ \t][ \t:*]"                                         , " " }, // space

// number
 { "[ :*](0|0.0)[ :*]"                                    , "z" }, // zero
 { "[ :*](1|1.0)[ :*]"                                    , "o" }, // one
 { "[ :*][-+0-9][0-9:*][ :*]"                             , "i" }, // int
//// { "[ :*][-+0-9:1-12][ :*]"                           , "i" }, // int 
// { "[ :*][-+0-9:1-*][A-Za-z][ :*][A-Za-z:1-10]"         , "u" }, // int with unit

 { "[ :*][-+0-9:*].[0-9:*][ :*]"                          , "r" }, // real 1
 { "[ :*][-+0-9.][0-9.:*][eE][-+0-9][0-9:*][ :*]"         , "r" }, // real 2
 { "[ :*][-+0-9][0-9:*].[0-9:*][eE][-+0-9][0-9:*][ :*]"   , "r" }, // real 3

 { "[ :*][-+0-9:*][ :*]%[ :*]"                            , "p" }, // percent 1
 { "[ :*][-+0-9:*].[0-9:*][ :*]%[ :*]"                    , "p" }, // percent 2

 { "[ :*][-+0-9:*].[0-9:*][ :*][A-Za-z:1-10]"             , "r" }, // real with unit 1
 { "[ :*][-+0-9.][0-9.:*][eE][-+0-9][0-9:*][ :*][A-Za-z:1-10]" , "r" }, // real with unit 2
 { "[ :*][-+0-9][0-9:*].[0-9:*][eE][-+0-9][0-9:*][ :*][A-Za-z:1-10]" , "r" }, // real with unit 3

// string
 { "http://*"                                             , "U" }, // url 1
 { "*www.*"                                               , "U" }, // url 2
 { "*.[a-zA-Z:1-4]"                                       , "F" }, // filename 
 { "[A-Za-z]*@*.*"                                        , "E" }, // email
// { "*[=\\-+*/%]*"                                       , "M" }, // formula

 { "<[^/<>:*]>"                                           , "H" }, // XML Head
 { "</[^/<>:*]>"                                          , "T" }, // XML Tail
 { "<[^/<>:*]>[^/<>]</[^/<>:*]>"                          , "X" }, // XML H+T 

 { "[0-9:2-4]/[0-9:1-2]/[0-9:1-2]*"                       , "d" }, // date
 { "*/*"                                                  , "D" }, // dirname
 //{ "<[-+=\"A-Za-z0-9_ :*]>"                               , "H" }, // XML Head
 //{ "</[-+=\"A-Za-z0-9_ :*]>"                              , "T" }, // XML Tail
 //{ "<[-+=\"A-Za-z0-9_ :*]>[-+=\"A-Za-z0-9_ :*]</[-+=\"A-Za-z0-9_ :*]>" , "X" }, // XML Tail
 
 //{ "[ \t:*][A-Za-z_$#@!()\[\]\{\}]*"                      , "S" }, // string 1
 { "[ \t:*][A-Za-z0-9_]*"                                 , "S" }, // string 1
 { "[<>!#$%&()~\\~\\*]*"                                  , "W" }, // string 2
 { "[\"\']*[\"\']"                                        , "Q" }, // string 3
 { 0                                                      , 0   }
};

PatternToSymbol CellPattern(CellExpr);

/////////////////////////////////////////////////////////////////////////
// Row/Column signatures
//
// Numerical
//
//  Integer with    key  0 1 T(01) I(1-N)   J(1-NW )
//  Integer without key  2 3 t(01) i(1-N)   j(1-NW )
//  real    with    key            N(real)  M(realW )
//  real    without key            n(real)  m(realW )
//  Strings with    key
//  Strings without key
//
/////////////////////////////////////////////////////////////////////////
EasyExpTable RowColExpr[] = {
 { "[B :*]"                                     , " " }, // empty 1
 { "[SQBDUEMW:1][B :*]"                         , "L" }, // label 1

 { "[SQBDUEFMW][z:2-*][B :*]"                    , "0" }, // key1 zero vector
 { "[SQBDUEFMW:2-5][z:2-*][B :*]"                , "2" }, // keyN zero vector
 { "[SQBDUEFMW][o:2-*][B :*]"                    , "1" }, // key1 one vector
 { "[SQBDUEFMW:2-5][o:2-*][B :*]"                , "3" }, // keyN one vector
 { "[SQBDUEFMW][zo:2-*][B :*]"                   , "T" }, // key1 one-zero vector
 { "[SQBDUEFMW:0-5][zo:2-*][B :*]"               , "t" }, // keyN one-zero vector

 { "[SQBDUEFMW][izo:2-*][B :*]"                  , "I" }, // key1 int vector
 { "[SQBDUEFMW:0-5][izo:2-*][B :*]"              , "i" }, // keyN int vector
 { "[SQBDUEFMW][izo:2-*W ][B :*]"                , "J" }, // key1 int union
 { "[SQBDUEFMW:0-5][izoW :2-*][B :*]"            , "j" }, // keyN int union

 { "[SQBDUEFMW][p:2-*][B :*]"                    , "P" }, // key1 percent vector
 { "[SQBDUEFMW:0-5][p:2-*][B :*]"                , "p" }, // keyN percent vector
 { "[SQBDUEFMW][pW :2-*][B :*]"                  , "B" }, // key1 percent union
 { "[SQBDUEFMW:0-5][pW :2-*][B :*]"              , "b" }, // keyN percent union

 { "[SQBDUEFMW][ripzouf:2-*][B :*]"              , "R" }, // key1 numeric vector
 { "[SQBDUEFMW:0-5][ripzouf:2-*][B :*]"          , "r" }, // keyN numeric vector
 { "[SQBDUEFMW][ripzoufW :2-*][B :*]"            , "M" }, // key1 numeric union
 { "[SQBDUEFMW:0-5][ripzoufW :2-*][B :*]"        , "m" }, // keyN numeric union

 { "[SQBDUEFMW][F:2-*][B :*]"                    , "F" }, // key1 file vector
 { "[SQBDUEFMW:0-5][F:2-*][B :*]"                , "f" }, // keyN file vector
 { "[SQBDUEFMW][U:2-*][B :*]"                    , "U" }, // key1 url vector
 { "[SQBDUEFMW:0-5][U:2-*][B :*]"                , "u" }, // keyN url vector
 { "[SQBDUEFMW][SQBDUEM:2-*][B :*]"              , "S" }, // key1 string vector
 //{"[SQBDUEFMW:0-5][SQBDUEM:1-*][B :*]"          , "s" }, // keyN string vector
 { "[SQBDUEM:2-*][B :*]"                         , "s" }, // keyN string vector

 { "[SQBDUEFMW][SQBDUEMW :2-*][B :*]"            , "A" }, // key1 string union
 //{"[SQBDUEFFMW:0-5][SQBDUEMW :1-*][B :*]"       , "a" }, // keyN string union
 { "[SQBDUEMW :1-*][B :*]"                       , "a" }, // keyN string union

 { "[SQBDUEFMW][SQBDUEMWripzouf :2-*][B :*]"     , "X" }, // key1 union vector
 //{ "[SQBDUEFMW:0-5][SQBDUEMWripzouf :1-*][B :*]" , "x" }, // keyN union vector
 { "SQBDUEMWripzouf :1-*][B :*]"                 , "x" }, // keyN union vector
 { 0                                             , 0 }
};

PatternToSymbol RowColPattern(RowColExpr);


/////////////////////////////////////////////////////////////////////////
// Sub Row/Column signatures
//
// Numerical
//
//  Integer with    key  0 1 T(01) I(1-N)   J(1-NW )
//  real    with    key            N(real)  M(realW )
//
/////////////////////////////////////////////////////////////////////////
EasyExp g_sub_key1_intvector_1("[SWQDFUEM:1-5][rzoiup:5-*][B :0-2][SWQDFUM]]*");


/////////////////////////////////////////////////////////////////////////
// Output table, column format 
int xindex=0;
int xobj_class = xindex++;
int xobj_id    = xindex++;
int xtable_id  = xindex++;
int xproperty   = xindex++;
int xvalue      = xindex++;
int xpos_rowmin  = xindex++;
int xpos_rowmax  = xindex++;
int xpos_colmin  = xindex++;
int xpos_colmax  = xindex++;
int xpos_rowtitle = xindex++;
int xpos_coltitle = xindex++;
int xdone         = xindex++;
int xclasslabel   = xindex++;
int xclasslabel2  = xindex++;

int objectid = 1000;

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
// ClassifyTable class
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
ClassifyTable::ClassifyTable() {
  m_debugmode=0;
  m_top=this;
  m_colmin=0; m_colmax=0; m_rowmin=0; m_rowmax=0;
}
/////////////////////////////////////////////////////////////////////////
ClassifyTable::ClassifyTable(const ClassifyTable& a) {
  m_fname=a.m_fname;
  m_name=a.m_name;
  m_colmin=a.m_colmin;
  m_colmax=a.m_colmax;
  m_rowmin=a.m_rowmin;
  m_rowmax=a.m_rowmax;
  m_original = a.m_original;
  m_cellid = a.m_cellid;
  m_rowid = a.m_rowid;
  m_colid = a.m_colid;
  m_colstat=a.m_colstat;
  m_rowstat=a.m_rowstat;
  m_debugmode=a.m_debugmode;
#if 0
  for(unsigned int i=0;i<a.p_subtable.size();i++) {
    ClassifyTable &p = &a.p_subtable[i];
    p_subtable.push_back(new ClassifyTable(p));
  }
#endif
}
/////////////////////////////////////////////////////////////////////////
ClassifyTable::~ClassifyTable() {
  for(unsigned int i=0;i<p_subtable.size();i++) {
    ClassifyTable *p = p_subtable[i];
    delete p;
  }
}

/////////////////////////////////////////////////////////////////////////
ClassifyTable ClassifyTable::operator=(const ClassifyTable& a) {
  m_colmin=a.m_colmin;
  m_colmax=a.m_colmax;
  m_rowmin=a.m_rowmin;
  m_rowmax=a.m_rowmax;
  m_original = a.m_original;
  m_cellid = a.m_cellid;
  m_rowid = a.m_rowid;
  m_colid = a.m_colid;
  m_colstat=a.m_colstat;
  m_rowstat=a.m_rowstat;
#if 0
  for(unsigned int i=0;i<a.p_subtable.size();i++) {
    ClassifyTable &p = &a.p_subtable[i];
    p_subtable.push_back(new ClassifyTable(p));
  }
#endif
  return(*this);
}

/////////////////////////////////////////////////////////////////////////
void ClassifyTable::setOriginal(const Collection& c,ClassifyTable* top
				,const string& fname,const string& name) {
  ClassifyTableProgress = "begin setOriginal";
  m_original = c;
  m_fname = fname;
  m_name = name;
  if(top) {
    m_top=top;
    m_debugmode=m_top->m_debugmode;
  }
  else {
    m_colmin=m_rowmin=0;
    m_rowmax=m_original.size()-1;
    if(m_rowmax>=0) m_colmax=m_original[0].size()-1;
    m_top=this;
  }
  ClassifyTableProgress = "end setOriginal";
}

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
void ClassifyTable::clear() {
  m_cellid.clear();
  m_rowid.clear();
  m_colid.clear();
}

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
// 
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
int ClassifyTable::Classification(int hier) {
  ClassifyTableProgress = "begin Classification";
  ClassificationEach();
  if(hier) DivideSubTable();
  //DetailClassification();
  ClassifyTableProgress = "end Classification";
  return(0);
}

/////////////////////////////////////////////////////////////////////////
int ClassifyTable::ClassificationEach(int rowmin,int rowmax
				      ,int colmin,int colmax) {
  ClassifyTableProgress = "begin ClassificationEasy";
  if(rowmin>=0) m_rowmin=rowmin;
  if(rowmax>=0) m_rowmax=rowmax;
  if(colmin>=0) m_colmin=colmin;
  if(colmax>=0) m_colmax=colmax;
  ClassifyTableProgress = "begin CellClassification";
  CellClassification();
  ClassifyTableProgress = "begin RowClassification";
  RowClassification();
  ClassifyTableProgress = "begin ColClassification";
  ColClassification();
  //DetailClassification();
  ClassifyTableProgress = "end ClassificationEach";
  return(0);
}
/////////////////////////////////////////////////////////////////////////
int ClassifyTable::CellClassification() {
  const Collection& c=m_original;
  int i,j;
  Line key,line;
  string s;
  string x;
  for(i=0;i<c.size();i++) {
    line.clear();
    for(j=0;j<c[i].size();j++) {
      x = "";
      s = c[i][j].c_str();

      x += CellPattern.getsymbol(s.c_str());
      
      if(x.size()==0) x="x";
      line.push_back(x);
    }
    m_cellid.push_back(line);
  }
  return(0);
}
/////////////////////////////////////////////////////////////////////////
int ClassifyTable::RowClassification() {
  const Collection& c=m_original;
  int i,j;
  Line key,line;
  string s;
  string x;
  string y;
  m_rowstat.clear();
  for(i=0;i<c.size();i++) {
    m_rowstat.add(c[i].size());
    line.clear();
    x = ""; y= "";
    for(j=0;j<c[i].size();j++) {
      //s = c[i][j].c_str();
      s = m_cellid[i][j].c_str();
      x += s[0];
    }
    
    y += RowColPattern.getsymbol(x.c_str());
    
    line.let(0,y);
    line.let(1,x);
    line.let(2,x.size());
    m_rowid.push_back(line);
  }
  return(0);
}
/////////////////////////////////////////////////////////////////////////
int ClassifyTable::ColClassification() {
  const Collection& c=m_original;
  int i,j;
  Line key,line1,line2,line3;
  string s;
  string x;
  string y;
  int maxj=0;
  for(i=0;i<c.size();i++) {
    if(maxj<c[i].size()) maxj=c[i].size();
  }
  
  m_colstat.clear();
  line1.clear(); line2.clear();
  for(j=0;j<maxj;j++) {
    x = ""; y= "";
    for(i=0;i<c.size();i++) {
      //s = c[i][j].c_str();
      s = m_cellid[i][j].c_str();
      if(s.size()>0) x+=s[0];
      else x+=" ";
    }

    y += RowColPattern.getsymbol(x.c_str());
    
    line1.push_back(y);
    if(x.size()>20) line2.push_back(x.substr(0,10)); 
    else            line2.push_back(x); 
    line3.push_back(x.size());
    m_colstat.add(x.size());
  }
  m_colid.push_back(line1);
  m_colid.push_back(line2);
  m_colid.push_back(line3);
  return(0);
}

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
int ClassifyTable::DivideSubTable() {
  ClassifyTableProgress = "begin DivideSubTable";
  const Collection& c=m_original;
  // Divide SubTable with Row Size
  if(m_colstat.stddev() && m_rowstat.stddev()==0) {
    DivideSubTableWRowClustering(); // added
    return(0);
  }
  // Markup languate block
  EasyExp BeginExp("H");
  EasyExp EndExp("T");
  int nest=0;
  //if(m_rowstat.stddev()==0) return(0);
  int i, istart=0, iend=0, sz, newsz, rowcontentscount=0;
  string rowcontents;
  Collection t;
  for(i=0;i<m_rowid.size();i++) {
    if(i==0) {
      sz = m_rowid[i][2].toint();
      rowcontents = m_rowid[i][0].tostring();
      rowcontentscount=1;
    }
    else {
      // separate table if row size is different
      newsz = m_rowid[i][2].toint(); 
#if 1
      if(sz != newsz /* && 0==nest */) {
#else  // this caused problem
	//if((sz*1.1 < newsz) && (sz*0.9 > newsz)) {
#endif
	sz = newsz;
	iend = i-1;
	t = c.line(istart,iend);
	ClassifyTable Sub(t,m_top,m_top->m_fname,m_top->m_name);
	//Sub.ClassificationEach(istart,iend,m_colmin,m_colmax);
	Sub.ClassificationEach(istart,iend,m_colmin,m_colmin+t[0].size());
	Sub.DivideSubTableWRowClustering();
	istart = i;
      }
    }
    //if(BeginExp.match(m_cellid[i][0].tostring())) ++nest;
    //else if(BeginExp.match(m_cellid[i][0].tostring())) --nest;
  }
  if(istart>0 && istart<i) { // if there is uncaptured tail rows
    iend = i-1;
    t = c.line(istart,iend);
    ClassifyTable Sub(t,m_top,m_top->m_fname,m_top->m_name);
    Sub.ClassificationEach(istart,iend,m_colmin,m_colmin+t[0].size());
    Sub.DivideSubTableWRowClustering();
  }
  else {
    DivideSubTableWRowClustering();
  }
  return(0);
}

/////////////////////////////////////////////////////////////////////////
int ClassifyTable::DivideSubTableWRowClustering() {
  // c contains rows with same column number only
  int i;

  // string,regex, class, count,
  Collection rowidclst = StringClustering(m_rowid,1,0.5,"CHAR",-1);
  int xclstclass=rowidclst.getx("class");
  int xclstcount=rowidclst.getx("count");
  rowidclst.setlinenum();
  Collection t,u = rowidclst.unique_count(2); // unique count by class
  int xucount = u.getx("count"), xuclass=u.getx("class");
  u.sort(xucount); // sort by category size

  if(u.size()>1) {
    map<int,int> istitle;
    int largestcls = u.front()[xucount].toint();
    int smallestcls = u.back()[xucount].toint();
    int total = rowidclst.size(), ntitle=0;

    // data vs title 
    for(i=0;i<u.size();i++) {
      if(u[i][xucount].toint()<=smallestcls*6) {
	istitle[u[i][xuclass].toint()] = 1;
	++ntitle;
      }
      else
	istitle[u[i][xuclass].toint()] = 0;
    }

    if((smallestcls>1 || ntitle>1) &&
       smallestcls*9<largestcls) { // few title columns and many data columns
      // scan and divide columns 
      int i=0,istart2=0,iend2=0;
      while(i<rowidclst.size()) {
        while(i<rowidclst.size()&&istitle[rowidclst[i][xclstclass].toint()])
	  ++i;
        while(i<rowidclst.size()&&!istitle[rowidclst[i][xclstclass].toint()])
	  ++i;
	
	iend2 = i-1;
	t = m_original.line(istart2,iend2);
	ClassifyTable Sub(t,m_top,m_top->m_fname,m_top->m_name);
	Sub.ClassificationEach(m_rowmin+istart2,m_rowmin+iend2,m_colmin,m_colmax);
	Sub.DivideSubTableWColClustering();
	istart2 = i;
      }
      if(istart2<i) { // the last block
	iend2 = i-1;
	t = m_original.line(istart2,iend2);
	ClassifyTable Sub(t,m_top,m_top->m_fname,m_top->m_name);
	Sub.ClassificationEach(m_rowmin+istart2,m_rowmin+iend2,m_colmin,m_colmax);
	Sub.DivideSubTableWColClustering();
	istart2 = i;
      }
      return(0);
    }
    else { // data rows with multiple different characteristics
           // for the moment, we don't split this
      DivideSubTableWColClustering();
    }
  }
  else { // no row divide
    DivideSubTableWColClustering();
  }
  return(0);
}

/////////////////////////////////////////////////////////////////////////
int ClassifyTable::DivideSubTableWColClustering() {
  int i;
  Line line;
  int ycolidsymbol=0,ycolidcontents=1;

  // transpose m_colid from column-wise to row-wise vector
  Collection colid;
  for(i=0;i<m_colid[ycolidcontents].size();i++) {
    // the other choise is to simply use unique_count()
    // on m_colid[m_colidsymbol][i]
    line.let(ycolidsymbol,m_colid[ycolidcontents][i].tostring());
    line.let(ycolidcontents,m_colid[ycolidcontents][i].tostring());
    colid.push_back(line);
  }

  // string,regex, class, count, ,
  int xcolidcontents=ycolidcontents;
  //  returns string,regex,class,count
  Collection colidclst = StringClustering(colid,xcolidcontents,0.5,"CHAR",-1);
  int xclstclass=colidclst.getx("class");
  int xclstcount=colidclst.getx("count");
  colidclst.setlinenum();
  Collection t,u = colidclst.unique_count(xclstclass); // unique count by class
  int xucount = u.getx("count"), xuclass=u.getx("class");
  u.sort(xucount); // sort by category size

  if(u.size()>1) {
    map<int,int> istitle;
    int largestcls = u.front()[xucount].toint();
    int smallestcls = u.back()[xucount].toint();
    int total = colidclst.size(), ntitle=0;

    // data vs title 
    for(i=0;i<u.size();i++) {
      if(u[i][xucount].toint()<=smallestcls*6) {
	istitle[u[i][xuclass].toint()] = 1;
	++ntitle;
      }
      else
	istitle[u[i][xuclass].toint()] = 0;
    }

    if((smallestcls>1 || ntitle>1) &&
       smallestcls*9<largestcls) { // few title columns and many data columns
      // scan and divide columns 
      int jstart2=0,jend2=0; i=0;
      while(i<colidclst.size()) {
        while(i<colidclst.size()&&istitle[colidclst[i][xclstclass].toint()])
	  ++i;
        while(i<colidclst.size()&&!istitle[colidclst[i][xclstclass].toint()])
	  ++i;
        jend2 = i-1;
        t = m_original.column(jstart2,jend2);
	ClassifyTable Sub(t,m_top,m_top->m_fname,m_top->m_name);
	Sub.ClassificationEach(m_rowmin,m_rowmax,jstart2,jend2);
        m_top->AddSubTable(&Sub);
        jstart2 = i;
      }
      if(jstart2<i) { // the last block
        jend2 = i-1;
        t = m_original.column(jstart2,jend2);
	ClassifyTable Sub(t,m_top,m_top->m_fname,m_top->m_name);
	Sub.ClassificationEach(m_rowmin,m_rowmax,jstart2,jend2);
        m_top->AddSubTable(&Sub);
        jstart2 = i;
      }
      return(0);
    }
    else { // data columns with multiple different characteristics
           // for the moment, we don't split this
      if((*m_top)!=(*this)) m_top->AddSubTable(this);
      return(0);
    }
  }
  else { // no subtable
    if((*m_top)!=(*this)) m_top->AddSubTable(this);
    return(0);
  }
  return(0);
}

/////////////////////////////////////////////////////////////////////////
int ClassifyTable::AddSubTable(const ClassifyTable* psub) {
  ClassifyTable* ptbl = new ClassifyTable(*psub);
  //printf("AddSubTable \n"); ptbl->disp();
  p_subtable.push_back(ptbl);
  return(0);
}

/////////////////////////////////////////////////////////////////////////
static int viewcsv(const char* fname) {
  ReadF f;
  f.open(fname);
  while(f.read()) {
    printf("%s\n",f.argv[0]);
  }
  f.close();
  return(0);
}

/////////////////////////////////////////////////////////////////////
DVector ColumnToDVector(Collection& c1,int x,int yoffset) {
  DVector tmp;
  for(int i=yoffset;i<c1.size();i++) {
    if(x<c1[i].size()) tmp.push_back(c1[i][x].todouble());
  }
  return tmp;
}

/////////////////////////////////////////////////////////////////////
DVector RowToDVector(Collection& c1,int y,int xoffset) {
  DVector tmp;
  for(int i=xoffset;i<c1[y].size();i++) {
    tmp.push_back(c1[y][i].todouble());
  }
  return tmp;
}

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
// Detail Classification  
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////
// Neural Network classification , pre-learnt NNs have to be prepared
/////////////////////////////////////////////////////////////////////////
int ClassifyTable::NNClassification(Collection& r
				    ,const string& fileexpr
				    ,const string& nnexpr
				    ,const string& targetobj
				    ,const string& targetprop) {
  string resultfile = "_NNclassification.csv";

  ///////////////////////////////////////////////////////////////////////
   // Neural Network classification
  string com = "NNclassify -o "; com += resultfile;
  com += " -S "; com += fileexpr;
  com += " -NN "; com += nnexpr;
  printf("%s\n",com.c_str()); system(com.c_str());

  if((m_debugmode&DEBUGMODE)==0) {
    com = "cint _remove.cxx "; com += fileexpr;
    printf("%s\n",com.c_str()); system(com.c_str());
  }

  ///////////////////////////////////////////////////////////////////////
  // read NN classifcaiton result
  Sheet sr(resultfile.c_str());
  Collection cr = sr.collection();
  cr.setkey(0);
  // cr format
  //  name    ,label1,shift1,prob1,label2,shift2,prob2,label3,shift3,prob3,
  //  XX_1001 ,clslb1,
  int xnnname=cr.getx("*name*");
  int xnnlabel1=cr.getx("(label|lbl)1");
  int xnnlabel2=cr.getx("(label|lbl)2");
  int xnnlabel3=cr.getx("(label|lbl)3");

  ///////////////////////////////////////////////////////////////////////
  // Merge NN classifcaiton result
  //                                                      classlabel column VVVVVV
  //  obj      ,ID  ,prop,val,rowmin,rowmax,colmin,colmax,rowtitle,coltitle,classlabel,
  //  matrix   ,1001,    ,   ,0     ,19    ,1     ,20    ,-1      ,0       ,matrix
  //  Numvector,1004,row ,0  ,76    ,95    ,0     ,10    ,75      ,-1      ,convexH
  int xoID = r.getx("*ID*");
  int xobj = r.getx("obj*");
  int xoclasslabel = r.getx("*label*");

  int count = 0, err=0;
  size_t pos1,pos2;
  Collection t;
  string FileID, ID, label1,label2,label3;
  for(int i=0;i<cr.size();i++) {
    FileID = cr[i][xnnname].tostring(); // NN input vector file name
    label1 = cr[i][xnnlabel1].tostring(); // primary candidate
    label2 = cr[i][xnnlabel2].tostring(); // secondary
    label3 = cr[i][xnnlabel3].tostring(); // thirdry

    // extract object ID from NN input vector file name
    //  xxxxx/NN_1015.csv  ID is a 4 digit number -> '1015'
    pos1 = FileID.find_last_of("_");
    if(pos1!=string::npos) ID=FileID.substr(pos1+1);
    pos2 = ID.find_last_of(".");
    if(pos2!=string::npos) ID=ID.substr(0,pos2);
    if(pos1==string::npos || pos2==string::npos) {
      fprintf(stderr,"Error: NNClassifiction(), Illegal NN data file name %s\n"
	      ,FileID.c_str());
      continue;
    }

    // Find target object from the result table and copy classification
    t = r.match(xoID,ID).match(xobj,targetobj).match(xproperty,targetprop);
    if(t.size()) {
      if(label1!="") {
        t.let(xoclasslabel,label1);
	if(label2!="") t.let(xoclasslabel+1,label2);
	if(label3!="") t.let(xoclasslabel+2,label3);
        ++count;
      }
    }
    else {
      printf("Internal Error; ID %s %s not found\n"
	     ,targetobj.c_str(),ID.c_str());
      if(!err++) {
	cr.disp();
	r.disp();
      }
    }
  }
  
  return(count);
}
 
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
Collection ClassifyTable::DetailClassificationTop() {
  ClassifyTableProgress = "begin DetailClassification";
  // prepare result container
  Line line;
  Collection r,rs;
  r.setsheet(m_original.p_sheet);
  rs.setsheet(m_original.p_sheet);

  // create table header 
  line.let(xobj_class,"obj");
  line.let(xobj_id,"ID");
  line.let(xtable_id,"table");
  line.let(xproperty,"prop");
  line.let(xvalue,"val");
  line.let(xpos_rowmin,"rowmin");
  line.let(xpos_rowmax,"rowmax");
  line.let(xpos_colmin,"colmin");
  line.let(xpos_colmax,"colmax");
  line.let(xpos_rowtitle,"rowtitle");
  line.let(xpos_coltitle,"coltitle");
  line.let(xclasslabel,"classlabel");
  r.push_back(line);
  r.setkey(0);

  // data classification by EasyExp and clustering
  // create column header 
  line.clear();
  line.let(xobj_class,"Table"); line.let(xtable_id,-1);
  line.let(xproperty,"subtbl"); line.let(xvalue,p_subtable.size());
  line.let(xpos_rowmin,m_rowmin); line.let(xpos_rowmax,m_rowmax);
  line.let(xpos_colmin,m_colmin); line.let(xpos_colmax,m_colmax);
  r.push_back(line);

  printf("## Main Table Classification\n");
  ClassifyTableProgress = "begin Main table classification";
  if(p_subtable.size()==0) {
    rs = DetailClassificationEach(-1) ;
    r.merge(rs);
  }

  ClassifyTableProgress = "begin sub table classification";
  for(int i=0;i<p_subtable.size();i++) {
    printf("##### Sub Table %d Classification\n",i);
    ClassifyTable *p = p_subtable[i];
    line.clear();
    line.let(xobj_class,"Table"); line.let(xtable_id,i);
    line.let(xproperty,"subtbl"); line.let(xvalue,p->p_subtable.size());
    line.let(xpos_rowmin,p->m_rowmin); line.let(xpos_rowmax,p->m_rowmax);
    line.let(xpos_colmin,p->m_colmin); line.let(xpos_colmax,p->m_colmax);
    r.push_back(line);
    rs = p->DetailClassificationEach(i); 
    r.merge(rs);
  }

  ClassifyTableProgress = "begin NeuralNetwork Classification";
  // data classification by neural network
  char DataFileExpr[BUFSIZE], NNFileExpr[BUFSIZE];

  sprintf(DataFileExpr,"\"%s/NNrow_*.csv\"",NNgraphbuf.c_str());
  sprintf(NNFileExpr,"\"%s/NNDB/%s\"",NNgraphbuf.c_str(),NNW_Vector.c_str());
  NNClassification(r,DataFileExpr,NNFileExpr,"Numvector","row");

  sprintf(DataFileExpr,"\"%s/NNcolumn_*.csv\"",NNgraphbuf.c_str());
  sprintf(NNFileExpr,"\"%s/NNDB/%s\"",NNgraphbuf.c_str(),NNW_Vector.c_str());
  NNClassification(r,DataFileExpr,NNFileExpr,"Numvector","column");

  sprintf(DataFileExpr,"\"%s/NNMatrix_*.csv\"",NNgraphbuf.c_str());
  sprintf(NNFileExpr,"\"%s/NNDB/%s\"",NNgraphbuf.c_str(),NNW_Matrix.c_str());
  NNClassification(r,DataFileExpr,NNFileExpr,"matrix","");
  
  ClassifyTableProgress = "end DetailClassification";
  return(r);
}

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
Collection ClassifyTable::DetailClassificationEach(int tableid) {
  if(m_debugmode&4) disp();
  else dispProp();

  // if this table is divided into several subtables, no classification
  Collection r;
  Line line;

  char NNgraphBase[BUFSIZE];
  char NNgraph[BUFSIZE];
  char Title[BUFSIZE], yname[BUFSIZE];
  sprintf(NNgraphBase,"%s/NN",NNgraphbuf.c_str());
  int i;
  static int j=0;
  DVector g_x,g_y;
  Collection t = m_original;

  // Row-Column header(key) and NumVector expression, 
  // See RowColExpr definition above in this file
  EasyExp KeyExpr("[01TIJPBRMFUASX]*"); 
  EasyExp NumVectorExpr("[0213TtIiPpRr]*");
  EasyExp NumUnionExpr("[0213TtIiPpRrJjBbMm]*");

  EasyExp FileVectorExpr("[Ff]*");
  EasyExp UrlVectorExpr("[Uu]*");
  EasyExp StrVectorExpr("[SsAaXx]*");

  //////////////////////////////////////////////////////////////////////////
  // Check row title
  int voteNumVector=0, voteNumUnion=0, voteKey=0;
  int totalCol =m_colid[0].size(), totalRow =m_rowid.size();
  for(i=0;i<m_colid[0].size();i++) {
    if(KeyExpr.match(m_colid[0][i].c_str())) ++voteKey;
    if(NumVectorExpr.match(m_colid[0][i].c_str())) ++voteNumVector;
    if(NumUnionExpr.match(m_colid[0][i].c_str())) ++voteNumUnion;
  }
  // set keyline and remove it from numeric vector or matrix if >50%
  // of columns match the pattern
  int rowtitle=0;
  if(voteKey>=totalCol*0.5) { t.setkey(0); rowtitle=1; }

  //////////////////////////////////////////////////////////////////////////
  // Check column title
  voteKey=0;
  for(i=0;i<m_rowid.size();i++) {
    if(KeyExpr.match(m_rowid[i][0].c_str())) ++voteKey;
  }
  int coltitle=0;
  if(voteKey>t.size()*0.5) { coltitle=1; }

  double MatrixAspectRatio=10, voteRatio=0.6, voteRatioUnion=0.9;
  int  VectorSizeLimit = 5;
  /////////////////////////////////////////////////////////////////////////
  // Matrix
  // REFINEMENT NEEDED
  if(voteNumVector>totalCol*voteRatio && 
     //voteNumUnion>totalCol*voteRatioUnion&& 
     totalRow>VectorSizeLimit && totalCol>VectorSizeLimit && 
     (totalRow < totalCol*MatrixAspectRatio)) {
    // Draw mumeric matrix if >90% of columns match the pattern
    // and table size satisfies above criteria

    printf("Numeric Matrix debugtmode=%d\n",m_debugmode);

    // remove the first column if it contains label information
    if(coltitle) t=t.column(1,t[0].size()-1);

    sprintf(Title,"%s %d %d %d %d",m_fname.c_str()
	      ,m_rowmin,m_rowmax ,m_colmin,m_colmax);

    // dump m_cellid + m_original in NNDB
    static int NNhist=0;
    sprintf(NNgraph,"%sMatrix_%d.csv",NNgraphBase,objectid); // Object ID A
    drawNN.title(Title) << t << "\n"; drawNN.saveNNDB(NNgraph);
    if((m_debugmode&PLOTMODE) || (m_debugmode&PLOTEACHMODE)) 
      draw.title(Title) << t << "\n";

    // output matrix object
    line.clear();
    line.let(xobj_class,"matrix");
    line.let(xobj_id,objectid); // Object ID A
    line.let(xtable_id,tableid); 
    line.let(xpos_rowmin,m_rowmin+rowtitle);
    line.let(xpos_rowmax,m_rowmax);
    line.let(xpos_colmin,m_colmin+coltitle);
    line.let(xpos_colmax,m_colmax);
    line.let(xpos_rowtitle,rowtitle?m_rowmin:-1);
    line.let(xpos_coltitle,coltitle?m_colmin:-1);
    r.push_back(line);

    // output Numvector column object 
    g_y=ColumnToDVector(t,0);
    g_x.resize(g_y.size()); g_x.i();
    sprintf(NNgraph,"%scolumn_%d.csv",NNgraphBase,objectid); // OBJECT ID  A
    plotNN << g_x << g_y << "\n"; plotNN.saveNNDB(NNgraph);
    line.clear();
    line.let(xobj_id,objectid); // OBJECT ID  A
    line.let(xtable_id,tableid); 
    line.let(xproperty,"column"); line.let(xvalue,i+m_colmin);
    line.let(xpos_rowmin,m_rowmin+rowtitle);
    line.let(xpos_rowmax,m_rowmax);
    line.let(xpos_colmin,m_colmin+coltitle);
    line.let(xpos_colmax,m_colmin+coltitle);
    line.let(xpos_rowtitle,rowtitle?m_rowmin:-1);
    line.let(xpos_coltitle,coltitle?m_colmin:-1);
    line.let(xobj_class,"Numvector");
    r.push_back(line);

    // output Numvector row object 
    g_y=RowToDVector(t,0);
    g_x.resize(g_y.size()); g_x.i();
    sprintf(NNgraph,"%srow_%d.csv",NNgraphBase,objectid); // Object ID  C
    plotNN << g_x << g_y << "\n"; plotNN.saveNNDB(NNgraph);
    line.clear();
    line.let(xobj_id,objectid);  // Object ID  A
    line.let(xtable_id,tableid); 
    line.let(xproperty,"row"); line.let(xvalue,i+m_rowmin);
    line.let(xpos_rowmin,m_rowmin+i);
    line.let(xpos_rowmax,m_rowmin+i);
    line.let(xpos_colmin,m_colmin+coltitle);
    line.let(xpos_colmax,m_colmax);
    line.let(xpos_rowtitle,rowtitle?m_rowmin:-1);
    line.let(xpos_coltitle,coltitle?m_colmin:-1);
    line.let(xobj_class,"Numvector");
    r.push_back(line);

    ++objectid;
  }
  else {

    /////////////////////////////////////////////////////////////////////////
    int flag=0;
    // Draw XY graph for individual column otherwise
    if((m_debugmode&PLOTMODE) || (m_debugmode&PLOTEACHMODE)) {
      sprintf(Title,"%s %d %d %d %d",m_fname.c_str()
	      ,m_rowmin,m_rowmax ,m_colmin,m_colmax);
      plot << Title;
    }

    /////////////////////////////////////////////////////////////////////////
    // column scan
    // REFINEMENT NEEDED
    if(totalRow>VectorSizeLimit && (totalCol < totalRow*MatrixAspectRatio)) {

      for(i=0;i<m_colid[0].size();i++) {
	line.clear();
	line.let(xobj_id,objectid); // OBJECT ID  B
	line.let(xtable_id,tableid); 
	line.let(xproperty,"column"); line.let(xvalue,i+m_colmin);
	line.let(xpos_rowmin,m_rowmin+rowtitle);
	line.let(xpos_rowmax,m_rowmax);
	line.let(xpos_colmin,m_colmin+i);
	line.let(xpos_colmax,m_colmin+i);
	line.let(xpos_rowtitle,rowtitle?m_rowmin:-1);
	line.let(xpos_coltitle,coltitle?m_colmin:-1);
	if(NumVectorExpr.match(m_colid[0][i].c_str())) {
	  ++flag;
	  printf("Numeric Vector column %d debugmode=%d\n",i,m_debugmode);
	  g_y=ColumnToDVector(t,i);
	  g_x.resize(g_y.size()); g_x.i();
	  if(g_y.size()>0) {
	    sprintf(NNgraph,"%scolumn_%d.csv",NNgraphBase,objectid); // OBJECT ID  B
	    plotNN << g_x << g_y << "\n";
	    plotNN.saveNNDB(NNgraph);
	    
	    if(rowtitle) sprintf(yname,"col%d %s",i,m_original[0][i].c_str());
	    else sprintf(yname,"col%d",i);
	    if(m_debugmode&PLOTEACHMODE) plot <<  g_x << g_y << yname << "\n";
	    else if(m_debugmode&PLOTMODE) plot >> g_x << g_y << yname;
	    if(m_debugmode&VIEWMODE) viewcsv(NNgraph);
	    
	    //
	    line.let(xobj_class,"Numvector");
	    r.push_back(line);
	  }
	}
	else if(FileVectorExpr.match(m_colid[0][i].c_str())) {
	  line.let(xobj_class,"Filevector");
	  r.push_back(line);
	}
	else if(UrlVectorExpr.match(m_colid[0][i].c_str())) {
	  line.let(xobj_class,"URLvector");
	  r.push_back(line);
	}
	else if(StrVectorExpr.match(m_colid[0][i].c_str())) {
	  line.let(xobj_class,"Strvector");
	  r.push_back(line);
	}
	++objectid;
      }
      
      if(m_debugmode&PLOTMODE && 0==(m_debugmode&PLOTEACHMODE) && flag) plot << "\n";
    }

    /////////////////////////////////////////////////////////////////////////
    // row scan
    // REFINEMENT NEEDED
    if(totalCol>VectorSizeLimit && (totalRow < totalCol*MatrixAspectRatio)) {
      flag=0;
      for(i=0;i<m_rowid.size()&&i<20;i++) {
	line.clear();
	line.let(xobj_id,objectid);  // Object ID  C
	line.let(xtable_id,tableid); 
	line.let(xproperty,"row"); line.let(xvalue,i+m_rowmin);
	line.let(xpos_rowmin,m_rowmin+i);
	line.let(xpos_rowmax,m_rowmin+i);
	line.let(xpos_colmin,m_colmin+coltitle);
	line.let(xpos_colmax,m_colmax);
	line.let(xpos_rowtitle,rowtitle?m_rowmin:-1);
	line.let(xpos_coltitle,coltitle?m_colmin:-1);
	if(NumVectorExpr.match(m_rowid[i][0].c_str())) {
	  ++flag;
	  printf("Numeric Vector row %d debugmode=%d\n",i,m_debugmode);
	  g_y=RowToDVector(t,i,rowtitle);
	  g_x.resize(g_y.size()); g_x.i();
	  if(g_y.size()>0) {
	    sprintf(NNgraph,"%srow_%d.csv",NNgraphBase,objectid); // Object ID  C
	    plotNN << g_x << g_y << "\n";
	    plotNN.saveNNDB(NNgraph);
	    
	    if(coltitle) sprintf(yname,"row%d %s",i,m_original[i][0].c_str());
	    else sprintf(yname,"row%d",i);
	    if(m_debugmode&PLOTEACHMODE) plot <<  g_x << g_y << yname << "\n";
	    else if(m_debugmode&PLOTMODE) plot >> g_x << g_y << yname;
	    if(m_debugmode&VIEWMODE) viewcsv(NNgraph);
	    
	    //
	    line.let(xobj_class,"Numvector");
	    r.push_back(line);
	  }
	}
	else if(FileVectorExpr.match(m_rowid[0][i].c_str())) {
	  line.let(xobj_class,"Filevector");
	  r.push_back(line);
	}
	else if(UrlVectorExpr.match(m_rowid[0][i].c_str())) {
	  line.let(xobj_class,"URLvector");
	  r.push_back(line);
	}
	else if(StrVectorExpr.match(m_rowid[0][i].c_str())) {
	  line.let(xobj_class,"Strvector");
	  r.push_back(line);
	}
	++objectid;
      }
      
      if(m_debugmode&PLOTMODE && 0==(m_debugmode&PLOTEACHMODE) && flag) plot << "\n";
    }
    /////////////////////////////////////////////////////////////////////////
  }

  return(r);
}

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
// Display result using the table analysis 
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
// obj,ID,prop,val,rowmin,rowmax,colmin,colmax,rowtitle,coltitle,classlabel
/////////////////////////////////////////////////////////////////////////
int ClassifyTable::DisplayResult(Collection& r,const string& scriptout) {
  r.setlinenum();
  Collection& orig = m_original;
  orig.setlinenum();
  Collection t,t2,t3,tv;
  DVector x,y;
  int i,j,k, x1,x2,y1,y2,xl,yl,linenum,flag;
  string title,xname,yname,yaccum,ID;

  title = m_fname + " " + m_name;

  FILE *fp=0;
  if(scriptout!="") {
    fp = fopen(scriptout.c_str(),"w");
    if(!fp) {
      printf("Error: Failed creating script output file %s\n",scriptout.c_str());
    }
    else {
      fprintf(fp,"#include <CSV.h>\n\n");
      fprintf(fp,"int readDataFromCSV(const string& fname) {\n");
      fprintf(fp,"  Sheet s(fname);\n");
      fprintf(fp,"  Collection t,t2,t3,tv,c=s.collection();\n");
      fprintf(fp,"  int x1,x2,y1,y2,xl,yl,j;\n");
      fprintf(fp,"  string xname,yname,title=s.getfname();\n");
      if(m_name.size())  
	fprintf(fp,"  title+=\" \"; title+=\"%s\";\n",m_name.c_str());
      fprintf(fp,"  DVector x,y;\n\n");
    }
  }

  printf("##### DisplayResult \n");

  ///////////////////////////////////////////////////////////////////
  // Matrix, 3D plot
  t = r.match(xobj_class,"matrix")._match(xclasslabel,"(col|row)XY*")
       .match(xdone,"");
//t.disp();
  for(i=0;i<t.size();i++) {
    // if first row or column is UpLinear vector, this is not
    ID = t[i][xobj_id].tostring();
    tv = r.match(xobj_id,ID);
    if(tv.match(xclasslabel,"(Up|Down)Linear").size()>0) continue;
    else tv.let(xdone,1);

    // draw 3D
    printf("Matrix %d : ",i); t[i].disp();
    y1=t[i][xpos_rowmin].toint(); y2=t[i][xpos_rowmax].toint();
    x1=t[i][xpos_colmin].toint(); x2=t[i][xpos_colmax].toint();
    yl=t[i][xpos_rowtitle].toint(); xl=t[i][xpos_coltitle].toint();
    if(x2>=m_original[y1].size()) x2=m_original[y1].size()-1;
    t2 = orig.line(y1,y2).column(x1,x2);      
    draw.title(title.c_str()) << t2 << "\n";  
    t[i].let(xdone,1);
    if(fp) {
      fprintf(fp,"  x1=%d; x2=%d; y1=%d; y2=%d;\n",x1,x2,y1,y2);
      fprintf(fp,"  t = c.line(y1,y2).column(x1,x2);\n");
      fprintf(fp,"  draw.title(title.c_str()) << t << \"\\n\";\n\n");
    }
  }

  ///////////////////////////////////////////////////////////////////
  // Matrix, column-wise XY plot with multiple independennt var lines
  t = r.match(xobj_class,"matrix").match(xclasslabel,"colXYmulti?")
       .match(xdone,"");
//t.disp();
  for(i=0;i<t.size();i++) {
    ID = t[i][xobj_id].tostring();
    tv = r.match(xobj_id,ID);
    tv.let(xdone,1);
    printf("Matrix colXYmulti %d : ",i); t[i].disp();
    y1=t[i][xpos_rowmin].toint(); y2=t[i][xpos_rowmax].toint();
    x1=t[i][xpos_colmin].toint(); x2=t[i][xpos_colmax].toint();
    yl=t[i][xpos_rowtitle].toint(); xl=t[i][xpos_coltitle].toint();
    if(x2>=m_original[y1].size()) x2=m_original[y1].size()-1;
    t2 = orig.line(y1,y2).column(x1,x2);
    plot << title.c_str();
    if(fp) {
      fprintf(fp,"  x1=%d; x2=%d; y1=%d; y2=%d; xl=%d; yl=%d;\n",x1,x2,y1,y2,xl,yl);
      fprintf(fp,"  t = c.line(y1,y2).column(x1,x2);\n");
      fprintf(fp,"  plot << title.c_str();\n");
      fprintf(fp,"  for(j=0;j<=x2-x1;j+=2) {\n");
      if(yl>=0) {
	fprintf(fp,"    xname = c[yl][x1+j].tostring();\n");
	fprintf(fp,"    yname = c[yl][x1+j+1].tostring();\n");
      }
      else  fprintf(fp,"    xname = yname = \"\";\n");
      fprintf(fp,"    x = ColumnToDVector(t,j,0);\n");
      fprintf(fp,"    y = ColumnToDVector(t,j+1,0);\n");
      fprintf(fp,"    plot >> x << xname.c_str() << y << yname.c_str();\n");
      fprintf(fp,"  }\n");
    }
    for(j=0;j<=x2-x1;j+=2) {
      if(yl>=0) {
	xname = orig[yl][x1+j].tostring();
	yname = orig[yl][x1+j+1].tostring();
      }
      else { xname = yname = ""; }
      x = ColumnToDVector(t2,j,0);
      y = ColumnToDVector(t2,j+1,0);
      plot >> x << xname.c_str() << y << yname.c_str();
    }
    yname += "\n";
    plot << yname.c_str();
    t[i].let(xdone,1);
    if(fp) {
      fprintf(fp,"  yname += \"\\n\";\n");
      fprintf(fp,"  plot << yname.c_str();\n\n");
    }
  }

  ///////////////////////////////////////////////////////////////////
  // Matrix, column-wise XY plot with multiple independennt var lines
  // Matrix, column-wise XY plot with single independennt var lines
  t = r.match(xobj_class,"matrix").match(xclasslabel,"colXYsingle?")
       .match(xdone,"");
//t.disp();
  for(i=0;i<t.size();i++) {
    y1=t[i][xpos_rowmin].toint(); y2=t[i][xpos_rowmax].toint();
    x1=t[i][xpos_colmin].toint(); x2=t[i][xpos_colmax].toint();
    yl=t[i][xpos_rowtitle].toint(); xl=t[i][xpos_coltitle].toint();
    if(x2>=m_original[y1].size()) x2=m_original[y1].size()-1;
    t2 = m_original.line(y1,y2).column(x1,x2);
    if(fp) {
      fprintf(fp,"  x1=%d; x2=%d; y1=%d; y2=%d; xl=%d; yl=%d;\n",x1,x2,y1,y2,xl,yl);
      fprintf(fp,"  t = c.line(y1,y2).column(x1,x2);\n");
    }
    // if first row or column is UpLinear vector, this is not
    ID = t[i][xobj_id].tostring();
    tv = r.match(xobj_id,ID);
    if(tv.match(xclasslabel,"(Up|Down)Linear").size()>0) {
      tv.let(xclasslabel,"");
      printf("Matrix colXYsingle %d : ",i); t[i].disp();
      if(yl>=0) xname = orig[yl][x1].tostring();
      else      xname = "";
      x = ColumnToDVector(t2,0,0);
      plot << title.c_str() << x << xname.c_str() ;
      if(fp) {
	if(yl>=0) fprintf(fp,"  xname = c[yl][x1].tostring();\n");
	else      fprintf(fp,"  xname = \"\";\n");
	fprintf(fp,"  x = ColumnToDVector(t,0,0);\n");
	fprintf(fp,"  plot << title.c_str() << x << xname.c_str();\n");
	fprintf(fp,"  for(j=0;j<=x2-x1;j+=2) {\n");
	if(yl>=0) fprintf(fp,"    yname = c[yl][x1+j].tostring();\n");
	else      fprintf(fp,"    yname = \"\";\n");
	fprintf(fp,"    y = ColumnToDVector(t,j,0);\n");
	fprintf(fp,"    plot << y << yname.c_str();\n");
	fprintf(fp,"  }\n");
      }
      for(j=1;j<=x2-x1;j++) {
	if(yl>=0) yname = orig[y1][x1+j].tostring();
	else      yname = "";
	y = ColumnToDVector(t2,j,0);
	plot << y << yname.c_str();
      }
      yname += "\n";
      plot << yname.c_str();
      if(fp) {
	fprintf(fp,"  yname += \"\\n\";\n");
	fprintf(fp,"  plot << yname.c_str();\n\n");
      }
    }
    else {
      printf("Matrix colXYsingle -> matrix %d : ",i); t[i].disp();
      draw.title(title.c_str()) << t2 << "\n";
      if(fp) {
	fprintf(fp,"  draw.title(title.c_str()) << t << \"\\n\";\n\n");
      }
    }
    ID = t[i][xobj_id].tostring();
    tv = r.match(xobj_id,ID);
    tv.let(xdone,1);
    t[i].let(xdone,1);
  }

  ///////////////////////////////////////////////////////////////////
  // Matrix, row-wise XY plot with multiple independennt var lines
  t = r.match(xobj_class,"matrix").match(xclasslabel,"rowXYmulti?")
       .match(xdone,"");

  ///////////////////////////////////////////////////////////////////
  // Matrix, row-wise XY plot with single independennt var lines
  t = r.match(xobj_class,"matrix").match(xclasslabel,"rowXYsingle?")
       .match(xdone,"");

  ///////////////////////////////////////////////////////////////////
  // Column vectors, XY plot X linear
  t = r.match(xobj_class,"Numvector").match(xproperty,"col*")
       .match(xclasslabel,"(Up|Down)Linear")
       .match(xdone,"");
  t2 = r.match(xobj_class,"Numvector").match(xproperty,"col*")
       .match(xclasslabel2,"(Up|Down)Linear")
       .match(xdone,"");
  if(t2.size()) t.unique_merge(t2);
  for(i=0;i<t.size();i++) {
    printf("Numvector column %d : ",i); t[i].disp();
    y1=t[i][xpos_rowmin].toint(); y2=t[i][xpos_rowmax].toint();
    x1=t[i][xpos_colmin].toint(); x2=t[i][xpos_colmax].toint();
    yl=t[i][xpos_rowtitle].toint(); xl=t[i][xpos_coltitle].toint();
    if(x2>=m_original[y1].size()) x2=m_original[y1].size()-1;
    t2 = orig.line(y1,y2).column(x1,x2);
    if(yl>=0) xname = orig[yl][x1].tostring();
    else      xname = "";
    x = ColumnToDVector(t2,0,0);
    plot << title.c_str() << x << xname.c_str() ;
    linenum = t[i].linenum()+1;
    if(fp) {
      fprintf(fp,"  x1=%d; x2=%d; y1=%d; y2=%d; xl=%d; yl=%d;\n",x1,x2,y1,y2,xl,yl);
      fprintf(fp,"  t = c.line(y1,y2).column(x1,x2);\n");
      if(yl>=0) fprintf(fp,"  xname = c[yl][x1].tostring();\n");
      else      fprintf(fp,"  xname = \"\";\n");
      fprintf(fp,"  x = ColumnToDVector(t,0,0);\n");
      fprintf(fp,"  plot << title.c_str() << x << xname.c_str();\n");
    }
    while(linenum<r.size() && r[linenum][xproperty].tostring()=="column") {
      if(r[linenum][xobj_class].tostring()=="Numvector") {
	x1 = x2 = r[linenum][xpos_colmin].toint();
	t2 = orig.line(y1,y2).column(x1,x2);
	if(yl>=0) yname = orig[yl][x1].tostring();
	else      yname = "";
	y = ColumnToDVector(t2,0,0);
        plot << y << yname.c_str();
	if(fp) {
	  fprintf(fp,"  x1=%d; x2=%d;\n",x1,x2);
	  fprintf(fp,"  t = c.line(y1,y2).column(x1,x2);\n");
	  if(yl>=0) fprintf(fp,"  yname = c[yl][x1].tostring();\n");
	  else      fprintf(fp,"  yname = \"\";\n");
	  fprintf(fp,"  y = ColumnToDVector(t,0,0);\n");
	  fprintf(fp,"  plot << y << yname.c_str();\n");
	}
	r[linenum].let(xdone,1);
      }
      ++linenum;
    }
    yname += "\n";
    plot << yname.c_str();
    if(fp) {
      fprintf(fp,"  yname += \"\\n\";\n");
      fprintf(fp,"  plot << yname.c_str();\n\n");
    }
    t[i].let(xdone,1);
  }

  ///////////////////////////////////////////////////////////////////
  // Column vectors, XY plot other
  t = r.match(xobj_class,"Numvector").match(xproperty,"col*")
       ._match(xclasslabel,"(Up|Down)Linear")
       .match(xdone,"");
  for(i=0;i<t.size();i++) {
    if(t[i][xdone].toint()>0) continue;
    printf("Numvector column no X %d : ",i); t[i].disp();
    if(x2>=m_original[y1].size()) x2=m_original[y1].size()-1;

    t3 = r.match(xobj_class,"Numvector").match(xproperty,"col*")
          .match(xtable_id,t[i][xtable_id].toint())
          .match(xdone,"");

    xname = "index";
    y1=t[i][xpos_rowmin].toint(); y2=t[i][xpos_rowmax].toint();
    x.resize(y2-y1+1); x.i();
    Collection cname,cnameclust;
    Line line;
    for(j=0;j<t3.size();j++) {
      y1=t3[j][xpos_rowmin].toint(); y2=t3[j][xpos_rowmax].toint();
      x1=t3[j][xpos_colmin].toint(); x2=t3[j][xpos_colmax].toint();
      yl=t3[j][xpos_rowtitle].toint(); xl=t3[j][xpos_coltitle].toint();
      t2 = orig.line(y1,y2).column(x1,x2);
      if(yl>=0) yname = orig[yl][x1].tostring();
      else      yname = "";
      yname = orig[yl][x1].tostring();
      line.let(0,yname);
      cname.push_back(line);
    }
    cnameclust = StringClustering(cname,0,0.3,"CHAR",-1);
    //cnameclust.disp();
    Collection cu = cnameclust.unique_count("class");
    int targetclass,thisclass,xclass=2;
    for(k=0;k<cu.size();k++) {
      targetclass = cu[k][0].toint();
      string clstitle = (title + cu[k][0].tostring());
      plot << clstitle.c_str() << x << xname.c_str() ;
      for(j=0;j<t3.size();j++) {
	y1=t3[j][xpos_rowmin].toint(); y2=t3[j][xpos_rowmax].toint();
	x1=t3[j][xpos_colmin].toint(); x2=t3[j][xpos_colmax].toint();
	yl=t3[j][xpos_rowtitle].toint(); xl=t3[j][xpos_coltitle].toint();
	t2 = orig.line(y1,y2).column(x1,x2);
	if(yl>=0) yname = orig[yl][x1].tostring();
	else      yname = "";
	thisclass = cnameclust.match(0,yname)[0][xclass].toint();
	if(targetclass==thisclass) {
	  y = ColumnToDVector(t2,0,0);
	  plot << y << yname.c_str();
	  t3[j].let(xdone,1);
	}
      }
      yname += "\n";
      plot << yname.c_str();
    }
    t[i].let(xdone,1);
  }

  ///////////////////////////////////////////////////////////////////
  // Row vectors, XY plot X linear
  t = r.match(xobj_class,"Numvector").match(xproperty,"row*")
       .match(xclasslabel,"(Up|Down)Linear")
       .match(xdone,"");
  for(i=0;i<t.size();i++) {
    printf("Numvector row %d : ",i); t[i].disp();
    y1=t[i][xpos_rowmin].toint(); y2=t[i][xpos_rowmax].toint();
    x1=t[i][xpos_colmin].toint(); x2=t[i][xpos_colmax].toint();
    yl=t[i][xpos_rowtitle].toint(); xl=t[i][xpos_coltitle].toint();
    if(x2>=m_original[y1].size()) x2=m_original[y1].size()-1;
    t2 = orig.line(y1,y2).column(x1,x2);
    if(yl>=0) xname = orig[y1][xl].tostring();
    else      xname = "";
    x = RowToDVector(t2,0,0);
    plot << title.c_str() << x << xname.c_str() ;
    linenum = t[i].linenum()+1;
    while(linenum<r.size() && r[linenum][xproperty].tostring()=="column") {
      if(r[linenum][xobj_class].tostring()=="Numvector") {
	x1 = x2 = r[linenum][xpos_colmin].toint();
	t2 = orig.line(y1,y2).column(x1,x2);
	if(yl>=0) yname = orig[y1][xl].tostring();
	else      yname = "";
	y = RowToDVector(t2,0,0);
        plot << y << yname.c_str();
	r[linenum].let(xdone,1);
      }
      ++linenum;
    }
    yname += "\n";
    plot << yname.c_str();
    t[i].let(xdone,1);
  }

  if(fp) {
    fprintf(fp,"  return(0);\n");
    fprintf(fp,"}\n\n");

    fprintf(fp,"int main() {\n");
    if(strncmp(m_fname.c_str(),"./",2)==0) 
      fprintf(fp,"  Scandir d(\"%s\");\n",m_fname.c_str()+2);
    else
      fprintf(fp,"  Scandir d(\"%s\");\n",m_fname.c_str());
    fprintf(fp,"  char *fname;\n");
    fprintf(fp,"  while((fname=d.Next())&&fname[0]) {\n");
    fprintf(fp,"    readDataFromCSV(fname);\n");
    fprintf(fp,"  }\n");
    fprintf(fp,"  G__pause();\n");
    fprintf(fp,"  cintgraph_stop();\n");
    fprintf(fp,"  return(0);\n");
    fprintf(fp,"}\n");
    fclose(fp);
  }
  return(0);
}


/////////////////////////////////////////////////////////////////////////
// Utility methods
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
void ClassifyTable::dispProp() const {
  printf("'%s' '%s' %d %d %d %d\n",m_fname.c_str(),m_name.c_str()
	 ,m_rowmin,m_rowmax,m_colmin,m_colmax);
}
/////////////////////////////////////////////////////////////////////////
void ClassifyTable::disp(int limit) const {
  printf("'%s' '%s' %d %d %d %d\n",m_fname.c_str(),m_name.c_str()
	 ,m_rowmin,m_rowmax,m_colmin,m_colmax);

  printf("## original\n");
  if(limit>0) m_original.line(0,limit).disp();
  else m_original.disp();

  printf("## cell\n");
  if(limit>0) m_cellid.line(0,limit).disp();
  else m_cellid.disp();

  printf("## row\n");
  if(limit>0) m_rowid.line(0,limit).disp();
  else m_rowid.disp();

  printf("## col\n");
  if(limit>0) m_colid.line(0,limit).disp();
  else m_colid.disp();
}

/////////////////////////////////////////////////////////////////////////
void ClassifyTable::dispHier(int limit) const {
  printf("##### disp - top \n");
  disp(limit);
  for(int i=0;i<p_subtable.size();i++) {
    printf("##### disp - Sub Table %d Classification\n",i);
    p_subtable[i]->disp(limit); 
  }
  printf("##### disp end\n");
}

/////////////////////////////////////////////////////////////////////////
#if 0
void ClassifyTable::saveNNDB(const char* fname,const char* mode) {
  // how to normalize magnitude
  // if this is not a numeric matrix, dump m_cellid pattern
  //      
  // if this is a numeric matrix,     dump normalized m_original 
  //   min to max -> -1 to 1
  //   2 or 3 discrete values
  //   log10(x)

  // how to normalize table size
  //   table size as is?
  //   fixed size          , min, max, mean, median, stddev, max-min, etc.. 
  //   regex normalization

  // TO BE IMPLEMENTED

  // Here or in VirtualGraph.cxx VirtualGraph_Histogram2D()
}
#endif

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
