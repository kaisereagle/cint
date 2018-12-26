/*****************************************************************************
 * _iclst.cxx
 *  Description: CSV file organizer
 *  Status: Experimental
 *  Date: 2015/May/20
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

#ifndef _ICLST_CXX
#define _ICLST_CXX

#ifdef __CINT__
#include <CSV.h>
#else
#include <FileStrCollection.h>
#endif

int flag=0;

EasyExpTable g_CellExpr[] = {
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
 { "[ \t:*][A-Za-z]*=[ \t:*][-+0-9.A-Za-z_]*"             , "f" }, // formula
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

PatternToSymbol g_CellPattern(g_CellExpr);

/////////////////////////////////////////////////////////////////////////
EasyExpTable g_RowColExpr[] = {
 { "[B :*]"                                     , " " }, // empty 1
 { "[SQBDUEMW:1][B :*]"                         , "L" }, // label 1

 { "[H:1]*"                                      , "<" }, // XML Header
 { "[T:1]*"                                      , ">" }, // XML Tail

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

PatternToSymbol g_RowColPattern(g_RowColExpr);

////////////////////////////////////////////////////////
//Sheet s;
FileStrCollection c;
Collection t,t2,t3,u,u2,u3;
Line key,line;
int main(int argc,char** argv) {
  if(argc==2) csvPrompt(argv[1]);
  else if(argc==3) csvPrompt(argv[1],atoi(argv[2]));
  ClassifyCSV();
  //cs.disp();
  //csig.disp();
  t = StringClustering(csig,1,0.5,"CHAR",-1);
  t2 = t.column("class");
  t2.column_push_back(csig.column(0));
  t2.sort(0);
  t2.disp();
  while(!G__pause());
  return(0);
}

////////////////////////////////////////////////////////
void csvPrompt(char* fname,int key=-1) {
   c.init(fname,key);
}

////////////////////////////////////////////////////////
void save(char* fname) {
  FILE* fp = EX_fopen(fname,"w");
  c.disp(fp);
  EX_fclose(fp);
  printf("result wrote to %s\n",fname);
}

////////////////////////////////////////////////////////
void NextFile() {
  Line l;
  while(c.read(l) && !c.isendoffile()) {
    l.disp();
  }
  printf("file = %s\n",c.fname());
}

////////////////////////////////////////////////////////
Collection cs,csig; // use auto-buffer s_sheet
vector<string> Row;
vector<int> lc;
void ClassifyCSV() {
  Line l,line,prevline;
  prevline.let(0,"***NOSUCHLINE***");
  int j;
  string s;
  string x,r,y;
  if(c.isvalid() || c.isnewfile()) {
    line.clear();
    line.let(0,"FILE");
    line.let(1,c.fname());
    cs.push_back(line);
    lc.push_back(0);
    Row.push_back("");
  }
  while(c.read(l)) {
    if(c.isendoffile()) {
      line.clear();
      line.let(0,"EOF");
      cs.push_back(line);
      lc.push_back(0);
      Row.push_back("");
    }
    if(c.isnewfile()) {
      line.clear();
      line.let(0,"FILE");
      line.let(1,c.fname());
      cs.push_back(line);
      lc.push_back(0);
      Row.push_back("");
    }
    line.clear();
    r = "";
    for(j=0;j<l.size();j++) {
      x = "";
      s = l[j].c_str();

      x += g_CellPattern.getsymbol(s.c_str());
      
      if(x.size()==0) x="x";
      line.push_back(x);
      r += x;
    }
    if(prevline==line) {
      ++lc.back();
    }
    else {
      cs.push_back(line);
      lc.push_back(1);
      y = g_RowColPattern.getsymbol(r.c_str());
      Row.push_back(y);
      prevline = line;
    }
  }
  if(c.isendoffile()) {
    line.clear();
    line.let(0,"EOF");
    cs.push_back(line);
    lc.push_back(0);
    Row.push_back("");
  }
  //printf("file = %s\n",c.fname());
  for(j=0;j<cs.size();j++) {
    cs[j].let(cs[j].size(),lc[j]);
    cs[j].let(cs[j].size(),Row[j].c_str());
  }
  //Collection r;
  int i;
  l.clear();
  s="";
  for(j=0;j<cs.size();j++) {
    if(cs[j][0].tostring()=="FILE") {
      l.let(0,cs[j][1].tostring());
    }
    else if(cs[j][0].tostring()=="EOF") {
      l.let(1,s);
      csig.push_back(l);
      s="";
    }
    else {
      for(i=0;i<cs[i].size();i++) {
	s += cs[j][i].tostring();
      }
      s += "#";
    }
  }
}

#endif // _ICLST_CXX




