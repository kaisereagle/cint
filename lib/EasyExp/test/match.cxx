//////////////////////////////////////////////////////////////////////
// EasyExp pattern match test
//
//  2015/Aug/12  Masaahru Goto
//////////////////////////////////////////////////////////////////////
#pragma include <EasyExp.dll> // Test Target
#pragma include <CSV.dll>       // Utility, Caution:: this library also includes EasyExp. 
#include <exception>
#include "_util_cinttest.h"

//////////////////////////////////////////////////////////////////////
struct Pattern_t {
  char* pattern;
  int   expect1;
  int   expect2;
  int   expect3;
  int   expect4;
  int   expect5;
  int   expect6;
  int   expect7;
  int   expect8;
  int   expect9;
  int   expect10;
  int   expect11;
  int   expect12;
  int   expect13;
  int   expect14;
  int   expect15;
};

int vervose=0;
int pass=0,fail=0;

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
int test(char* name,int num,EasyExp& e,char* pattern,int expect,bool icase=false) {
  RESET_LOCALFAIL();
  int result;
  try {
    result = e.match(pattern,icase);
  }
  catch(...) {
    G__pause();
  }
  EXPECT_EQ(result,expect) ;
  if(result!=expect) {
    printf("FAIL %s %d '%s'\t'%s' result=%d expect=%d\n",name,num,pattern,e.pattern().c_str(),result,expect);
    ++fail;
    return(1);
  }
  if(vervose==1) {
    printf("p %s %d '%s'\t'%s' rslt=%d expct=%d, ",name,num,pattern,e.pattern().c_str(),result,expect);
    for(int i=0;i<e.argv.size();i++) printf("%d:%s ",i,e.argv[i].c_str());
    printf("\n");
  }
  else if(vervose==2) {
    static int flag=0;
    if(!flag) {
      printf("passfail,easyexpID,easyexp,patternID,pattern,arg0,arg1,arg2,arg3,arg4,arg5\n");
      ++flag;
    }
    printf("%d,%s,\"%s\",%d,\"%s\",",result,name,e.pattern(),num,pattern);
    for(int i=0;i<e.argv.size();i++) printf("%s,",e.argv[i].c_str());
    printf("\n");
  }
  ++pass;
  return(0);
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
int tabletest(const char* testpattern) {
  RESET_LOCALFAIL();
  Sheet s(testpattern);
  Collection c=s.collection();
  c.setkey(0);
  int xpassfail = c.getx("passfail");
  int xexpr     = c.getx("easyexp");
  int xpat      = c.getx("pattern");
  int xarg      = c.getx("arg0");
  int err=0,matcherr=0,argerr=0;
  EasyExp e;
  int result,j,i;
  for(i=0;i<c.size();i++) {
    matcherr=0; argerr=0;
    e.compile(c[i][xexpr].c_str());
    result = e.match(c[i][xpat].c_str());
    if(result!=c[i][xpassfail].toint()) ++matcherr;
    if(result) { // test argument spliting
      for(j=0;j<e.argv.size();j++) {
	if(e.argv[j]!=c[i][xarg+j].tostring()) ++argerr;
      }
    }
    EXPECT_FALSE(matcherr || argerr);
    if(matcherr || argerr) {
      printf("table test failure : match %d  arg %d\n",matcherr,argerr);
      c[i].disp();
      printf("match=%d argc=%d ",result,e.argv.size());
      for(j=0;j<e.argv.size();j++) printf(" %s",e.argv[j].c_str());
      printf("\n");
      ++err;
      ++fail;
      //G__pause();
    }
    else ++pass;
  }
  return(err);
}


//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// test 1, Zero length string list
Pattern_t test1Pattern[]={//e1 e2  e3  e4  e5  e6  e7  e8  e9  e10
  { "pattern"            , 0 , 1 , 0 , 1 , 1 , 0 , 1 , 0 , 1 , 0 }, // 0
  { "pattern.tsv"        , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 0 }, // 1
  { "pattern.csv"        , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 0 }, // 2
  { "pattern.doc"        , 0 , 1 , 0 , 1 , 1 , 0 , 1 , 0 , 0 , 0 }, // 3
  { "patternAB.csv"      , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 0 , 0 , 0 }, // 4
  { "patternAB.tsv"      , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 0 , 0 , 0 }, // 5
  { "patternAB.doc"      , 0 , 1 , 0 , 1 , 1 , 0 , 1 , 0 , 0 , 0 }, // 6
  { "patternAB.csvx"     , 0 , 1 , 1 , 1 , 1 , 1 , 1 , 0 , 0 , 0 }, // 7
  { "patternAB.tsvx"     , 0 , 1 , 1 , 1 , 1 , 1 , 1 , 0 , 0 , 0 }, // 8
  { "patternAB.docx"     , 0 , 1 , 0 , 1 , 1 , 0 , 1 , 0 , 0 , 0 }, // 9
  { "patternAB.tsvy"     , 0 , 1 , 0 , 1 , 1 , 1 , 1 , 0 , 0 , 0 }, // 10
  { "patternAB.tsvy"     , 0 , 1 , 0 , 1 , 1 , 1 , 1 , 0 , 0 , 0 }, // 11
  { "patternA__.tsv"     , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 0 , 0 , 1 }, // 12
  { "patternB__.tsv"     , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 0 , 0 , 1 }, // 13
  { "pattern__.tsv"      , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 0 , 0 , 1 }, // 14
  { "pattern_.tsv"       , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 0 , 0 , 1 }, // 15
  { "pattern_.tsv_aa.csv", 1 , 1 , 1 , 1 , 1 , 1 , 1 , 0 , 0 , 0 }, // 16
  {  0                   , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 }
};

int test1(Pattern_t* pattern=test1Pattern) {
  printf("test1\n");
  int err=0;
  EasyExp e1("pattern*(.tsv|.csv)");        // must have (.tsv|.csv)
  EasyExp e2("pattern*(|.tsv|.csv)");       // same as pattern*
  EasyExp e3("pattern*(.tsv|.csv)(|x)");    // must have (.tsv|.csv|.tsvx|.csvx)
  EasyExp e4("pattern*(|.tsv|.csv)(|x)");   // same as pattern*
  EasyExp e5("pattern*(|.tsv|.csv)[x:0-1]");// same as pattern*
  EasyExp e6("pattern*(.tsv|.csv)*(|x)");   // must have  (.tsv|.csv)
  EasyExp e7("pattern*(|.tsv|.csv)*(|x)");  // same as pattern*
  EasyExp e8("pattern(.tsv|.csv)");         // must have (.tsv|.csv)
  EasyExp e9("pattern(|.tsv|.csv)");        // 
  EasyExp e10("pattern(|A_|B_|_)_(.tsv|.csv)");   // 

  int i;
  for(i=0;pattern[i].pattern;++i) {
    err += test("e1",i,e1,pattern[i].pattern,pattern[i].expect1);
    err += test("e2",i,e2,pattern[i].pattern,pattern[i].expect2);
    err += test("e3",i,e3,pattern[i].pattern,pattern[i].expect3);
    err += test("e4",i,e4,pattern[i].pattern,pattern[i].expect4);
    err += test("e5",i,e5,pattern[i].pattern,pattern[i].expect5);
    err += test("e6",i,e6,pattern[i].pattern,pattern[i].expect6);
    err += test("e7",i,e7,pattern[i].pattern,pattern[i].expect7);
    err += test("e8",i,e8,pattern[i].pattern,pattern[i].expect8);
    err += test("e9",i,e9,pattern[i].pattern,pattern[i].expect9);
    err += test("e10",i,e10,pattern[i].pattern,pattern[i].expect10);
  }

  EasyExp_DEBUG=3;
  //e1.match("pattern_.tsv_aa.csv");
  //e10.match("pattern_.tsv");
  //e4.match("patternAB.tsvy");
  //e2.match("patternAB.csvx");
  //e7.match("patternAB.doc");
  //G__pause();
  EasyExp_DEBUG=0;
  
  return(err);
}

//////////////////////////////////////////////////////////////////////
// test 2, Long pattern
Pattern_t test2Pattern[]={    // e1  e2  e3  e4  e5  e6  e7  e8  e9  e10
  { "pnAXE_19901130_0153.csv"  , 1 , 0 , 1 , 1 , 1 , 0 , 0 , 0 , 0 , 0 }, // 0
  { "pnAX_19901130_0153.csv"   , 1 , 0 , 1 , 1 , 1 , 0 , 0 , 0 , 0 , 0 }, // 1
  { "pnA_19901130_0153.csv"    , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 }, // 2
  { "pnAXE_17901130_0153.csv"  , 0 , 0 , 1 , 1 , 1 , 0 , 0 , 0 , 0 , 0 }, // 3
  { "pnAXE_20101530_0153.csv"  , 0 , 0 , 1 , 1 , 1 , 0 , 0 , 0 , 0 , 0 }, // 4
  { "pnAXE_20101232_0153.csv"  , 0 , 0 , 1 , 1 , 1 , 0 , 0 , 0 , 0 , 0 }, // 5
  { "pnAXE_20101230_0153.csv"  , 1 , 0 , 1 , 1 , 1 , 0 , 0 , 0 , 0 , 0 }, // 6
  { "pnAXE_20101230_01534.csv" , 0 , 0 , 0 , 1 , 1 , 0 , 0 , 0 , 0 , 0 }, // 7
  { "XnAXE_20101230_0153.csv"  , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 }, // 8
  { "pnAXE_20101230_.csv"      , 0 , 0 , 0 , 1 , 1 , 0 , 0 , 0 , 0 , 0 }, // 8
  { "pnAXE__.csv"              , 0 , 0 , 0 , 0 , 1 , 0 , 0 , 0 , 0 , 0 }, // 8
  { "fr015_apple_2001_AB.txt"  , 0 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 }, // 9
  { "fr015_orange_2001_AB.txt" , 0 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 }, // 10
  { "fr015_banana_2001_AB.txt" , 0 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 }, // 11
  { "fr015_apple_2000_AB.txt"  , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 }, // 12
  { "fr015_apple_2021_AB.txt"  , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 }, // 13
  { "fr015_aple_2001_AB.txt"   , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 }, // 14
  { "fr015_apple_2001.txt"     , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 }, // 15
  { "fr015_apple_2001_.txt"    , 0 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 }, // 16
  { "fr015_apple_2001_AB.doc"  , 0 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 }, // 17
  { "fr015_apple_2001_AB.xls"  , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 }, // 18
  {  0                         , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 }
};

int test2(Pattern_t* pattern=test2Pattern) {
  printf("test2\n");
  int err=0;
  EasyExp e1("pn[A-Z:2-3]_(1985..2015:4)(01..12:2)(01..31:2)_(0000..9999:4).csv");
  EasyExp e2("fr*_(apple|orange|banana)_(2001..2020)_*.(txt|doc|docx)");
  EasyExp e3("pn[A-Z:2-3]_[0-9:8]_[0-9:4].csv");
  EasyExp e4("pn[A-Z:2-3]_[0-9:6-10]_[0-9:*].csv");
  EasyExp e5("pn[A-Z:2-3]_[0-9:*]_[0-9:*].csv");

  int i;
  for(i=0;pattern[i].pattern;++i) {
    err += test("e1",i,e1,pattern[i].pattern,pattern[i].expect1);
    err += test("e2",i,e2,pattern[i].pattern,pattern[i].expect2);
    err += test("e3",i,e3,pattern[i].pattern,pattern[i].expect3);
    err += test("e4",i,e4,pattern[i].pattern,pattern[i].expect4);
    err += test("e5",i,e5,pattern[i].pattern,pattern[i].expect5);
  }

  EasyExp_DEBUG=3;
  //e7.match("pattern0222011yeq");
  //e7.match("pattern9112011yeq");
  //e8.match("pattern0222011yeq");
  //G__pause();
  EasyExp_DEBUG=0;
  
  return(err);
}

//////////////////////////////////////////////////////////////////////
// test 3, String, StarString
Pattern_t test3Pattern[]={                 //e1  e2  e3  e4  e5  e6  e7  e8  e9  e10
  { "pattern"                              , 1 , 0 , 1 , 0 , 1 , 0 , 0 , 0 , 0 , 0 }, // 0
  { "pattern!@#$%^&*()-_=\\+[]:;/?<>.,~`"  , 0 , 1 , 0 , 1 , 1 , 1 , 0 , 0 , 0 , 0 }, // 1
  { "ABpattern"                            , 0 , 0 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 }, // 2
  { "ABpattern!@#$%^&*()-_=\\+[]:;/?<>.,~`", 0 , 0 , 0 , 1 , 0 , 0 , 0 , 0 , 0 , 0 }, // 3
  { "patternXY"                            , 0 , 0 , 0 , 0 , 1 , 0 , 0 , 0 , 0 , 0 }, // 4
  { "pattern!@#$%^&*()-_=\\+[]:;/?<>.,~`XY", 0 , 0 , 0 , 0 , 1 , 1 , 0 , 0 , 0 , 0 }, // 5
  { "Apattern"                             , 0 , 0 , 1 , 0 , 0 , 0 , 1 , 0 , 0 , 0 }, // 6
  { "Apattern!@#$%^&*()-_=\\+[]:;/?<>.,~`" , 0 , 0 , 0 , 1 , 0 , 0 , 0 , 1 , 0 , 0 }, // 7
  { "patternX"                             , 0 , 0 , 0 , 0 , 1 , 0 , 0 , 0 , 1 , 0 }, // 8
  { "pattern!@#$%^&*()-_=\\+[]:;/?<>.,~`Y" , 0 , 0 , 0 , 0 , 1 , 1 , 0 , 0 , 0 , 1 }, // 9
  {  0                                     , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 }
};

int test3(Pattern_t* pattern=test3Pattern) {
  printf("test3\n");
  int err=0;
  EasyExp e1("pattern");  
  EasyExp e2("pattern!@#$%^&\\*\\(\\)-_=\\\\+\\[\\]:;/\\?<>.,~`"); 
  EasyExp e3("*pattern");  
  EasyExp e4("*pattern!@#$%^&\\*\\(\\)-_=\\\\+\\[\\]:;/\\?<>.,~`"); 
  EasyExp e5("pattern*");  
  EasyExp e6("pattern!@#$%^&\\*\\(\\)-_=\\\\+\\[\\]:;/\\?<>.,~`*"); 
  EasyExp e7("?pattern");  
  EasyExp e8("?pattern!@#$%^&\\*\\(\\)-_=\\\\+\\[\\]:;/\\?<>.,~`"); 
  EasyExp e9("pattern?");  
  EasyExp e10("pattern!@#$%^&\\*\\(\\)-_=\\\\+\\[\\]:;/\\?<>.,~`?"); 

  int i;
  for(i=0;pattern[i].pattern;++i) {
    err += test("e1",i,e1,pattern[i].pattern,pattern[i].expect1);
    err += test("e2",i,e2,pattern[i].pattern,pattern[i].expect2);
    err += test("e3",i,e3,pattern[i].pattern,pattern[i].expect3);
    err += test("e4",i,e4,pattern[i].pattern,pattern[i].expect4);
    err += test("e5",i,e5,pattern[i].pattern,pattern[i].expect5);
    err += test("e6",i,e6,pattern[i].pattern,pattern[i].expect6);
    err += test("e7",i,e7,pattern[i].pattern,pattern[i].expect7);
    err += test("e8",i,e8,pattern[i].pattern,pattern[i].expect8);
    err += test("e9",i,e9,pattern[i].pattern,pattern[i].expect9);
    err+=test("e10",i,e10,pattern[i].pattern,pattern[i].expect10);
  }

  EasyExp_DEBUG=3;
  //e2.disp();
  //e3.disp();
  //e2.match(pattern[1].pattern);
  //e10.match("pattern_.tsv");
  //e4.match("patternAB.tsvy");
  //e2.match("patternAB.csvx");
  //e7.match("patternAB.doc");
  //G__pause();
  EasyExp_DEBUG=0;
  
  return(err);
}

//////////////////////////////////////////////////////////////////////
// test 4, StringList, StarStringList
Pattern_t test4Pattern[]={                //e1  e2  e3  e4  e5  e6  e7  e8  e9  e10
  { "abc"                                 , 1 , 1 , 1 , 1 , 1 , 0 , 0 , 1 , 0 , 0 }, // 0
  { "def"                                 , 1 , 1 , 1 , 1 , 1 , 0 , 0 , 0 , 0 , 0 }, // 1
  { "hij"                                 , 1 , 1 , 1 , 1 , 1 , 0 , 0 , 0 , 0 , 0 }, // 2
  { "cde"                                 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 }, // 3
  { ""                                    , 0 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 }, // 4
  { "123"                                 , 0 , 0 , 0 , 0 , 0 , 1 , 0 , 0 , 0 , 0 }, // 5
  { "!#$%&'"                              , 0 , 0 , 0 , 0 , 0 , 1 , 0 , 0 , 0 , 0 }, // 6
  { "-^|;:<>/"                            , 0 , 0 , 0 , 0 , 0 , 1 , 0 , 0 , 0 , 0 }, // 7
  { "()[]*?"                              , 0 , 0 , 0 , 0 , 0 , 1 , 0 , 0 , 0 , 0 }, // 8
  { "hijkabcabcdefhij"                    , 0 , 0 , 1 , 1 , 1 , 0 , 1 , 1 , 1 , 0 }, // 9
  { "xyzabc"                              , 0 , 0 , 1 , 0 , 1 , 0 , 0 , 1 , 0 , 0 }, // 10
  { "abcxyz"                              , 0 , 0 , 0 , 1 , 1 , 0 , 0 , 1 , 0 , 0 }, // 11
  { "hijabcxyz"                           , 0 , 0 , 0 , 1 , 1 , 0 , 0 , 1 , 0 , 0 }, // 12
  {  0                                    , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 }
};

int test4(Pattern_t* pattern=test4Pattern) {
  printf("test4\n");
  int err=0;
  EasyExp e1("(abc|def|hij)");  
  EasyExp e2("(abc||def|hij)");  
  EasyExp e3("*(abc|def|hij)");  
  EasyExp e4("(abc|def|hij)*");  
  EasyExp e5("*(abc|def|hij)*");  
  EasyExp e6("(123|!#$%&'|-^\\|;:<>/|\\(\\)\\[\\]\\*\\?)");  
  EasyExp e7("*(abc|abcdef)hij*");   // TO BE FIXED with pattern 9, This can be tough.
  EasyExp e8("*(abc|abcdef)*");  
  EasyExp e9("*(abcdef|abc)hij*");  

  int i;
  for(i=0;pattern[i].pattern;++i) {
    err += test("e1",i,e1,pattern[i].pattern,pattern[i].expect1);
    err += test("e2",i,e2,pattern[i].pattern,pattern[i].expect2);
    err += test("e3",i,e3,pattern[i].pattern,pattern[i].expect3);
    err += test("e4",i,e4,pattern[i].pattern,pattern[i].expect4);
    err += test("e5",i,e5,pattern[i].pattern,pattern[i].expect5);
    err += test("e6",i,e6,pattern[i].pattern,pattern[i].expect6);
    err += test("e7",i,e7,pattern[i].pattern,pattern[i].expect7);
    err += test("e8",i,e8,pattern[i].pattern,pattern[i].expect8);
    err += test("e9",i,e9,pattern[i].pattern,pattern[i].expect9);
    //err+=test("e10",i,e10,pattern[i].pattern,pattern[i].expect10);
  }

  EasyExp_DEBUG=3;
  //e2.match(pattern[1].pattern);
  //e10.match("pattern_.tsv");
  //e4.match("patternAB.tsvy");
  //e2.match("patternAB.csvx");
  //e7.match("patternAB.doc");
  //e7.match("hijkabcabcdefhij");
  //e9.match("hijkabcabcdefhij");
  //G__pause();
  EasyExp_DEBUG=0;
  
  return(err);
}

//////////////////////////////////////////////////////////////////////
// test 5, CharList, StarCharList
Pattern_t test5Pattern[]={           //e1  e2  e3  e4  e5  e6  e7  e8  e9  e10 e11 e12
  { "0"                              , 0 , 1 , 0 , 1 , 0 , 1 , 0 , 0 , 0 , 0 , 0 , 0 }, // 0
  { "1"                              , 1 , 1 , 1 , 1 , 1 , 1 , 0 , 0 , 0 , 0 , 0 , 0 }, // 1
  { "H"                              , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 }, // 2
  { "a"                              , 1 , 1 , 1 , 1 , 1 , 1 , 0 , 0 , 0 , 0 , 0 , 0 }, // 3
  { "A"                              , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 }, // 4
  { "pattern1"                       , 0 , 0 , 1 , 1 , 0 , 1 , 0 , 0 , 0 , 0 , 1 , 1 }, // 5
  { "patterna"                       , 0 , 0 , 1 , 1 , 0 , 1 , 0 , 0 , 0 , 0 , 0 , 0 }, // 6
  { "patternA"                       , 0 , 0 , 0 , 0 , 0 , 1 , 0 , 0 , 0 , 0 , 0 , 0 }, // 7
  { "1pattern"                       , 0 , 0 , 0 , 1 , 1 , 1 , 0 , 0 , 0 , 0 , 0 , 1 }, // 8
  { "apattern"                       , 0 , 0 , 0 , 1 , 1 , 1 , 0 , 0 , 0 , 0 , 0 , 1 }, // 9
  { "Apattern"                       , 0 , 0 , 0 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1 }, // 10
  { "pattern1pattern"                , 0 , 0 , 0 , 1 , 0 , 1 , 0 , 0 , 1 , 0 , 0 , 0 }, // 11
  { "pattern12pattern"               , 0 , 0 , 0 , 1 , 0 , 1 , 0 , 1 , 1 , 0 , 0 , 0 }, // 12
  { "patternpattern"                 , 0 , 0 , 0 , 1 , 0 , 1 , 0 , 0 , 1 , 0 , 0 , 1 }, // 13
  { "pattern0pattern"                , 0 , 0 , 0 , 1 , 0 , 1 , 0 , 0 , 1 , 0 , 0 , 1 }, // 14
  { "pattern01pattern"               , 0 , 0 , 0 , 1 , 0 , 1 , 0 , 1 , 1 , 0 , 0 , 1 }, // 15
  { "pattern012pattern"              , 0 , 0 , 0 , 1 , 0 , 1 , 1 , 1 , 0 , 0 , 0 , 1 }, // 16
  { "pattern0123pattern"             , 0 , 0 , 0 , 1 , 0 , 1 , 1 , 1 , 0 , 0 , 0 , 1 }, // 17
  { "pattern01234pattern"            , 0 , 0 , 0 , 1 , 0 , 1 , 1 , 1 , 0 , 0 , 0 , 1 }, // 18
  { "pattern012345pattern"           , 0 , 0 , 0 , 1 , 0 , 1 , 0 , 0 , 0 , 0 , 0 , 1 }, // 19
  { "pattern0123456pattern"          , 0 , 0 , 0 , 1 , 0 , 1 , 0 , 0 , 0 , 0 , 0 , 1 }, // 20
  { "pattern^pattern"                , 0 , 0 , 0 , 1 , 0 , 1 , 0 , 0 , 0 , 1 , 0 , 1 }, // 21
  { "pattern-pattern"                , 0 , 0 , 0 , 1 , 0 , 1 , 0 , 0 , 0 , 1 , 0 , 1 }, // 22
  { "pattern"                        , 0 , 0 , 0 , 1 , 0 , 1 , 0 , 0 , 0 , 0 , 1 , 1 }, // 23
  { "pattern34"                      , 0 , 0 , 0 , 1 , 0 , 1 , 0 , 0 , 0 , 0 , 1 , 1 }, // 24
  { "pattern345"                     , 0 , 0 , 0 , 1 , 0 , 1 , 0 , 0 , 0 , 0 , 0 , 0 }, // 25
  {  0                               , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 }
};

int test5(Pattern_t* pattern=test5Pattern) {
  printf("test5\n");
  int err=0;
  EasyExp e1("[12aeg]");  
  EasyExp e2("[a-z0-9]");  
  EasyExp e3("*[12aeg]");  
  EasyExp e4("*[a-z0-9]");  
  EasyExp e5("[12aeg]*");  
  EasyExp e6("[a-z0-9]*");  
  EasyExp e7("*n[0-9:3-5]p*");  
  EasyExp e8("*n[0-9:2-5]p*");  
  EasyExp e9("*n[0-9:0-2]p*");  
  EasyExp e10("*n[-\\^]p*");  
  EasyExp e11("pattern[0-9:0-2]");  
  EasyExp e12("*pattern[0-9:0-2]");  // TO BE FIXED
  //e10.disp();

  int i;
  for(i=0;pattern[i].pattern;++i) {
    err += test("e1",i,e1,pattern[i].pattern,pattern[i].expect1);
    err += test("e2",i,e2,pattern[i].pattern,pattern[i].expect2);
    err += test("e3",i,e3,pattern[i].pattern,pattern[i].expect3);
    err += test("e4",i,e4,pattern[i].pattern,pattern[i].expect4);
    err += test("e5",i,e5,pattern[i].pattern,pattern[i].expect5);
    err += test("e6",i,e6,pattern[i].pattern,pattern[i].expect6);
    err += test("e7",i,e7,pattern[i].pattern,pattern[i].expect7);
    err += test("e8",i,e8,pattern[i].pattern,pattern[i].expect8);
    err += test("e9",i,e9,pattern[i].pattern,pattern[i].expect9);
    err+=test("e10",i,e10,pattern[i].pattern,pattern[i].expect10);
    err+=test("e11",i,e11,pattern[i].pattern,pattern[i].expect11);
    //err+=test("e12",i,e12,pattern[i].pattern,pattern[i].expect12); // TO BE FIXED
  }

  EasyExp_DEBUG=3;
  //e4.match(pattern[5].pattern);
  //e10.match("pattern_.tsv");
  //e4.match("patternAB.tsvy");
  //e2.match("patternAB.csvx");
  //e7.match("patternAB.doc");
  //G__pause();
  EasyExp_DEBUG=0;
  
  return(err);
}

//////////////////////////////////////////////////////////////////////
// test 6, NotCharList, StarNotCharList
Pattern_t test6Pattern[]={                //e1  e2  e3  e4  e5  e6  e7  e8  e9  e10
  { "0"                                   , 1 , 0 , 1 , 0 , 1 , 0 , 0 , 0 , 0 , 0 }, // 0
  { "1"                                   , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 }, // 1
  { "H"                                   , 1 , 1 , 1 , 1 , 1 , 1 , 0 , 0 , 0 , 0 }, // 2
  { "a"                                   , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 }, // 3
  { "A"                                   , 1 , 1 , 1 , 1 , 1 , 1 , 0 , 0 , 0 , 0 }, // 4
  { "pattern1"                            , 0 , 0 , 0 , 0 , 1 , 0 , 0 , 0 , 0 , 0 }, // 5
  { "patterna"                            , 0 , 0 , 0 , 0 , 1 , 0 , 0 , 0 , 0 , 0 }, // 6
  { "patternA"                            , 0 , 0 , 1 , 1 , 1 , 0 , 0 , 0 , 0 , 0 }, // 7
  { "1pattern"                            , 0 , 0 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 }, // 8
  { "apattern"                            , 0 , 0 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 }, // 9
  { "Apattern"                            , 0 , 0 , 1 , 0 , 1 , 1 , 0 , 0 , 0 , 0 }, // 10
  { "pattern1pattern"                     , 0 , 0 , 1 , 0 , 1 , 0 , 0 , 0 , 0 , 0 }, // 11
  { "pattern12pattern"                    , 0 , 0 , 1 , 0 , 1 , 0 , 0 , 0 , 0 , 0 }, // 12
  { "patternpattern"                      , 0 , 0 , 1 , 0 , 1 , 0 , 0 , 0 , 1 , 1 }, // 13
  { "pattern0pattern"                     , 0 , 0 , 1 , 0 , 1 , 0 , 0 , 0 , 0 , 0 }, // 14
  { "pattern01pattern"                    , 0 , 0 , 1 , 0 , 1 , 0 , 0 , 0 , 0 , 0 }, // 15
  { "pattern012pattern"                   , 0 , 0 , 1 , 0 , 1 , 0 , 0 , 0 , 0 , 0 }, // 16
  { "pattern0123pattern"                  , 0 , 0 , 1 , 0 , 1 , 0 , 0 , 0 , 0 , 0 }, // 17
  { "pattern01234pattern"                 , 0 , 0 , 1 , 0 , 1 , 0 , 0 , 0 , 0 , 0 }, // 18
  { "pattern012345pattern"                , 0 , 0 , 1 , 0 , 1 , 0 , 0 , 0 , 0 , 0 }, // 19
  { "pattern0123456pattern"               , 0 , 0 , 1 , 0 , 1 , 0 , 0 , 0 , 0 , 0 }, // 20
  { "patternApattern"                     , 0 , 0 , 1 , 0 , 1 , 0 , 0 , 0 , 1 , 1 }, // 21
  { "patternABpattern"                    , 0 , 0 , 1 , 0 , 1 , 0 , 0 , 0 , 1 , 1 }, // 22
  { "patternABCpattern"                   , 0 , 0 , 1 , 0 , 1 , 0 , 1 , 1 , 0 , 0 }, // 23
  { "patternABCDpattern"                  , 0 , 0 , 1 , 0 , 1 , 0 , 1 , 0 , 0 , 0 }, // 24
  { "patternABCDEpattern"                 , 0 , 0 , 1 , 0 , 1 , 0 , 1 , 0 , 0 , 0 }, // 25
  { "patternABCDEFpattern"                , 0 , 0 , 1 , 0 , 1 , 0 , 0 , 0 , 0 , 0 }, // 26
  { "patternABCDEFGpattern"               , 0 , 0 , 1 , 0 , 1 , 0 , 0 , 0 , 0 , 0 }, // 27
  { "patternA^-Dpattern"                  , 0 , 0 , 1 , 0 , 1 , 0 , 1 , 0 , 0 , 0 }, // 28
  {  0                                    , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 }
};

int test6(Pattern_t* pattern=test6Pattern) {
  printf("test6\n");
  int err=0;
  EasyExp e1("[^12aeg]");  
  EasyExp e2("[^a-z0-9]");  
  EasyExp e3("*[^12aeg]");  
  EasyExp e4("*[^a-z0-9]");  
  EasyExp e5("[^12aeg]*");  
  EasyExp e6("[^a-z0-9]*");  
  EasyExp e7("*n[^0-9a-z:3-5]p*");  
  EasyExp e8("*n[^0-9a-z:3]p*");  
  EasyExp e9("*n[^0-9a-z:0-2]p*");  
  EasyExp e10("*n[^\\-\\^0-9a-z:0-2]p*");  

  int i;
  for(i=0;pattern[i].pattern;++i) {
    err += test("e1",i,e1,pattern[i].pattern,pattern[i].expect1);
    err += test("e2",i,e2,pattern[i].pattern,pattern[i].expect2);
    err += test("e3",i,e3,pattern[i].pattern,pattern[i].expect3);
    err += test("e4",i,e4,pattern[i].pattern,pattern[i].expect4);
    err += test("e5",i,e5,pattern[i].pattern,pattern[i].expect5);
    err += test("e6",i,e6,pattern[i].pattern,pattern[i].expect6);
    err += test("e7",i,e7,pattern[i].pattern,pattern[i].expect7);
    err += test("e8",i,e8,pattern[i].pattern,pattern[i].expect8);
    err += test("e9",i,e9,pattern[i].pattern,pattern[i].expect9);
    err+=test("e10",i,e10,pattern[i].pattern,pattern[i].expect10);
  }

  EasyExp_DEBUG=3;
  //e9.match(pattern[13].pattern);
  //e7.match(pattern[23].pattern);
  //e10.match("pattern_.tsv");
  //e4.match("patternAB.tsvy");
  //e2.match("patternAB.csvx");
  //e7.match("patternAB.doc");
  //G__pause();
  EasyExp_DEBUG=0;
  
  return(err);
}

//////////////////////////////////////////////////////////////////////
// test 7, NumberRange, StarNumberRange
Pattern_t test7Pattern[]={ // e1  e2  e   e4  e5  e6  e7  e8  e9  e10
  { "pattern"               , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 }, // 0
  { "pattern1991"           , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 }, // 1
  { "pattern1998"           , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 }, // 1
  { "pattern1999"           , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 0 , 0 }, // 1
  { "pattern2015"           , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 0 , 0 }, // 2
  { "pattern2020"           , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 0 , 0 }, // 2
  { "pattern2021"           , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 }, // 2
  { "pattern19910401"       , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 }, // 3
  { "pattern20151001"       , 0 , 0 , 0 , 0 , 0 , 1 , 0 , 1 , 0 , 0 }, // 4
  { "pattern1991xyz"        , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 }, // 5
  { "pattern2015xyz"        , 0 , 0 , 0 , 0 , 1 , 1 , 1 , 1 , 0 , 0 }, // 6
  { "patternxyq19910401"    , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 }, // 7
  { "patternerx20151001"    , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 0 , 0 }, // 8
  { "patternxyq1991yeq"     , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 }, // 9
  { "patternerx2015yeq"     , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 1 , 0 , 0 }, // 10
  { "pattern01019910401"    , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 }, // 11
  { "pattern01120151001"    , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 1 , 1 }, // 12
  { "pattern0222011yeq"     , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 1 , 1 }, // 13
  { "pattern0221991yeq"     , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 }, // 14
  { "pattern9112015yeq"     , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 1 , 0 }, // 15
  {  0                      , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 }
};

int test7(Pattern_t* pattern=test7Pattern) {
  printf("test7\n");
  int err=0;
  EasyExp e1("pattern(1999..2020)");           
  EasyExp e2("pattern(1999..2020:4)");         
  EasyExp e3("pattern*(1999..2020)");           
  EasyExp e4("pattern*(1999..2020:4)");         
  EasyExp e5("pattern(1999..2020)*");           
  EasyExp e6("pattern(1999..2020:4)*");         
  EasyExp e7("pattern*(1999..2020)*");          
  EasyExp e8("pattern*(1999..2020:4)*");         
  EasyExp e9("pattern[0-9:3](1999..2020:4)*");       
  EasyExp e10("pattern(010..050:3)(1999..2020:4)*");   

  int i;
  for(i=0;pattern[i].pattern;++i) {
    err += test("e1",i,e1,pattern[i].pattern,pattern[i].expect1);
    err += test("e2",i,e2,pattern[i].pattern,pattern[i].expect2);
    err += test("e3",i,e3,pattern[i].pattern,pattern[i].expect3);
    err += test("e4",i,e4,pattern[i].pattern,pattern[i].expect4);
    err += test("e5",i,e5,pattern[i].pattern,pattern[i].expect5);
    err += test("e6",i,e6,pattern[i].pattern,pattern[i].expect6);
    err += test("e7",i,e7,pattern[i].pattern,pattern[i].expect7);
    err += test("e8",i,e8,pattern[i].pattern,pattern[i].expect8);
    err += test("e9",i,e9,pattern[i].pattern,pattern[i].expect9);
    err += test("e10",i,e10,pattern[i].pattern,pattern[i].expect10);
  }

  EasyExp_DEBUG=3;
  //e7.match("pattern0222011yeq");
  //e7.match("pattern9112011yeq");
  //e8.match("pattern0222011yeq");
  //G__pause();
  EasyExp_DEBUG=0;
  
  return(err);
}

//////////////////////////////////////////////////////////////////////
// test 8, End, StarEnd, AnyChar
Pattern_t test8Pattern[]={ // e1  e2  e3  e4  e5  e6  e7  e8  e9  e10
  { ""                      , 1 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 }, // 0
  { "p"                     , 0 , 1 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 }, // 1
  { "a"                     , 0 , 1 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 }, // 2
  { "0"                     , 0 , 1 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 }, // 3
  { "9"                     , 0 , 1 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 }, // 4
  { "^"                     , 0 , 1 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 }, // 5
  { "#"                     , 0 , 1 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 }, // 6
  { "("                     , 0 , 1 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 }, // 7
  { "["                     , 0 , 1 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 }, // 8
  { "-"                     , 0 , 1 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 }, // 9
  { "$"                     , 0 , 1 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 }, // 10
  { "pattern"               , 0 , 1 , 0 , 0 , 1 , 0 , 0 , 0 , 0 , 0 }, // 11
  { "pattern1PATTERN"       , 0 , 1 , 0 , 1 , 1 , 0 , 0 , 0 , 0 , 0 }, // 12
  { "pattern#PATTERN"       , 0 , 1 , 0 , 1 , 1 , 0 , 0 , 0 , 0 , 0 }, // 13
  { "pattern$PATTERN"       , 0 , 1 , 0 , 1 , 1 , 0 , 0 , 0 , 0 , 0 }, // 14
  { "pattern-PATTERN"       , 0 , 1 , 0 , 1 , 1 , 0 , 0 , 0 , 0 , 0 }, // 15
  { "pattern:PATTERN"       , 0 , 1 , 0 , 1 , 1 , 0 , 0 , 0 , 0 , 0 }, // 16
  { "pattern::PATTERN"      , 0 , 1 , 0 , 0 , 1 , 0 , 0 , 0 , 0 , 0 }, // 17
  { "pattern=PATTERN"       , 0 , 1 , 0 , 1 , 1 , 0 , 0 , 0 , 0 , 0 }, // 18
  { "pattern+=PATTERN"      , 0 , 1 , 0 , 0 , 1 , 0 , 0 , 0 , 0 , 0 }, // 19
  { "1pattern"              , 0 , 1 , 0 , 0 , 0 , 0 , 1 , 0 , 0 , 0 }, // 20
  { "#pattern"              , 0 , 1 , 0 , 0 , 0 , 0 , 1 , 0 , 0 , 0 }, // 21
  { "$pattern"              , 0 , 1 , 0 , 0 , 0 , 0 , 1 , 0 , 0 , 0 }, // 22
  { "-pattern"              , 0 , 1 , 0 , 0 , 0 , 0 , 1 , 0 , 0 , 0 }, // 23
  { ":pattern"              , 0 , 1 , 0 , 0 , 0 , 0 , 1 , 0 , 0 , 0 }, // 24
  { "::pattern"             , 0 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 }, // 25
  { "=pattern"              , 0 , 1 , 0 , 0 , 0 , 0 , 1 , 0 , 0 , 0 }, // 26
  { "+pattern"              , 0 , 1 , 0 , 0 , 0 , 0 , 1 , 0 , 0 , 0 }, // 27
  { "pattern1"              , 0 , 1 , 0 , 0 , 1 , 1 , 0 , 0 , 0 , 0 }, // 28
  { "pattern#"              , 0 , 1 , 0 , 0 , 1 , 1 , 0 , 0 , 0 , 0 }, // 29
  { "pattern$"              , 0 , 1 , 0 , 0 , 1 , 1 , 0 , 0 , 0 , 0 }, // 30
  { "pattern-"              , 0 , 1 , 0 , 0 , 1 , 1 , 0 , 0 , 0 , 0 }, // 31
  { "pattern:"              , 0 , 1 , 0 , 0 , 1 , 1 , 0 , 0 , 0 , 0 }, // 32
  { "pattern::"             , 0 , 1 , 0 , 0 , 1 , 0 , 0 , 0 , 0 , 0 }, // 33
  { "pattern="              , 0 , 1 , 0 , 0 , 1 , 1 , 0 , 0 , 0 , 0 }, // 34
  { "pattern+"              , 0 , 1 , 0 , 0 , 1 , 1 , 0 , 0 , 0 , 0 }, // 35
  {  0                      , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 }
};

int test8(Pattern_t* pattern=test8Pattern) {
  printf("test8\n");
  int err=0;
  EasyExp e1("");           
  EasyExp e2("*");         
  EasyExp e3("?");           
  EasyExp e4("pattern?PATTERN");         
  EasyExp e5("pattern*");         
  EasyExp e6("pattern?");          
  EasyExp e7("?pattern");         

  int i;
  for(i=0;pattern[i].pattern;++i) {
    err += test("e1",i,e1,pattern[i].pattern,pattern[i].expect1);
    err += test("e2",i,e2,pattern[i].pattern,pattern[i].expect2);
    err += test("e3",i,e3,pattern[i].pattern,pattern[i].expect3);
    err += test("e4",i,e4,pattern[i].pattern,pattern[i].expect4);
    err += test("e5",i,e5,pattern[i].pattern,pattern[i].expect5);
    err += test("e6",i,e6,pattern[i].pattern,pattern[i].expect6);
    err += test("e7",i,e7,pattern[i].pattern,pattern[i].expect7);
  }

  EasyExp_DEBUG=3;
  //e7.match("pattern0222011yeq");
  //e7.match("pattern9112011yeq");
  //e8.match("pattern0222011yeq");
  //G__pause();
  EasyExp_DEBUG=0;
  
  return(err);
}

//////////////////////////////////////////////////////////////////////
// test 9, case dependent matching
Pattern_t test9Pattern[]={ // e1  e2  e3  e4  e5  e6  e7  e8  e9  e10
  { "abcd"                  , 0 , 0 , 0 , 0 , 0 , 1 , 0 , 0 , 0 , 0 }, // 0
  { "ABCD"                  , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 }, // 1
  { "aBcD"                  , 1 , 1 , 1 , 1 , 0 , 0 , 0 , 0 , 0 , 0 }, // 2
  { "AbcD"                  , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 }, // 3
  { "efgh"                  , 0 , 0 , 0 , 0 , 0 , 1 , 0 , 0 , 0 , 0 }, // 4
  { "EFGH"                  , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 }, // 5
  { "efGh"                  , 0 , 0 , 1 , 1 , 0 , 1 , 0 , 0 , 0 , 0 }, // 6
  { "AbcD"                  , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 }, // 7
  { "efghabcd"              , 0 , 0 , 0 , 0 , 0 , 1 , 0 , 0 , 0 , 0 }, // 8
  { "EFGHABCD"              , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 }, // 9
  { "efGhaBcD"              , 0 , 1 , 0 , 1 , 0 , 0 , 0 , 0 , 0 , 0 }, // 10
  { "abcdijkl"              , 0 , 0 , 0 , 1 , 0 , 1 , 0 , 0 , 0 , 0 }, // 11
  { "ABCDIJKL"              , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 }, // 12
  { "AbcDijkl"              , 0 , 0 , 0 , 1 , 0 , 1 , 0 , 0 , 0 , 0 }, // 13
  { "b"                     , 0 , 0 , 0 , 0 , 1 , 1 , 0 , 0 , 0 , 0 }, // 14
  { "B"                     , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 }, // 15
  { "n"                     , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 }, // 16
  { "N"                     , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 }, // 17
  { "0"                     , 0 , 0 , 0 , 0 , 1 , 1 , 0 , 0 , 0 , 0 }, // 18
  { "9"                     , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 }, // 19
  { "abcdb"                 , 0 , 0 , 0 , 0 , 0 , 1 , 0 , 0 , 0 , 0 }, // 20
  { "ABCDB"                 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 }, // 21
  { "aBcD0"                 , 0 , 0 , 0 , 0 , 0 , 1 , 0 , 0 , 0 , 0 }, // 22
  { "efGh9"                 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 }, // 23
  { "EFGH9"                 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 }, // 24
  { "efgh9"                 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 }, // 25
  {  0                      , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 }
};

int test9(Pattern_t* pattern=test9Pattern) {
  printf("test9\n");
  int err=0;
  EasyExp e1("aBcD");           
  EasyExp e2("*aBcD");           
  EasyExp e3("(aBcD|efGh|ijkl)");           
  EasyExp e4("*(aBcD|efGh|ijkl)");           
  EasyExp e5("[a-m0-5]");         
  EasyExp e6("*[a-m0-5]");         

  bool icase = false;
  int i;
  for(i=0;pattern[i].pattern;++i) {
    err += test("e1",i,e1,pattern[i].pattern,pattern[i].expect1,icase);
    err += test("e2",i,e2,pattern[i].pattern,pattern[i].expect2,icase);
    err += test("e3",i,e3,pattern[i].pattern,pattern[i].expect3,icase);
    err += test("e4",i,e4,pattern[i].pattern,pattern[i].expect4,icase);
    err += test("e5",i,e5,pattern[i].pattern,pattern[i].expect5,icase);
    err += test("e6",i,e6,pattern[i].pattern,pattern[i].expect6,icase);
  }

  EasyExp_DEBUG=3;
  //e7.match("pattern0222011yeq");
  //e7.match("pattern9112011yeq");
  //e8.match("pattern0222011yeq");
  //G__pause();
  EasyExp_DEBUG=0;
  
  return(err);
}

//////////////////////////////////////////////////////////////////////
// test 10, case dependent matching
Pattern_t test10Pattern[]={// e1  e2  e3  e4  e5  e6  e7  e8  e9  e10
  { "abcd"                  , 1 , 1 , 1 , 1 , 0 , 1 , 0 , 0 , 0 , 0 }, // 0
  { "ABCD"                  , 1 , 1 , 1 , 1 , 0 , 1 , 0 , 0 , 0 , 0 }, // 1
  { "aBcD"                  , 1 , 1 , 1 , 1 , 0 , 1 , 0 , 0 , 0 , 0 }, // 2
  { "AbcD"                  , 1 , 1 , 1 , 1 , 0 , 1 , 0 , 0 , 0 , 0 }, // 3
  { "efgh"                  , 0 , 0 , 1 , 1 , 0 , 1 , 0 , 0 , 0 , 0 }, // 4
  { "EFGH"                  , 0 , 0 , 1 , 1 , 0 , 1 , 0 , 0 , 0 , 0 }, // 5
  { "efGh"                  , 0 , 0 , 1 , 1 , 0 , 1 , 0 , 0 , 0 , 0 }, // 6
  { "AbcD"                  , 1 , 1 , 1 , 1 , 0 , 1 , 0 , 0 , 0 , 0 }, // 7
  { "efghabcd"              , 0 , 1 , 0 , 1 , 0 , 1 , 0 , 0 , 0 , 0 }, // 8
  { "EFGHABCD"              , 0 , 1 , 0 , 1 , 0 , 1 , 0 , 0 , 0 , 0 }, // 9
  { "efGhaBcD"              , 0 , 1 , 0 , 1 , 0 , 1 , 0 , 0 , 0 , 0 }, // 10
  { "abcdijkl"              , 0 , 0 , 0 , 1 , 0 , 1 , 0 , 0 , 0 , 0 }, // 11
  { "ABCDIJKL"              , 0 , 0 , 0 , 1 , 0 , 1 , 0 , 0 , 0 , 0 }, // 12
  { "AbcDijkl"              , 0 , 0 , 0 , 1 , 0 , 1 , 0 , 0 , 0 , 0 }, // 13
  { "b"                     , 0 , 0 , 0 , 0 , 1 , 1 , 0 , 0 , 0 , 0 }, // 14
  { "B"                     , 0 , 0 , 0 , 0 , 1 , 1 , 0 , 0 , 0 , 0 }, // 15
  { "n"                     , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 }, // 16
  { "N"                     , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 }, // 17
  { "0"                     , 0 , 0 , 0 , 0 , 1 , 1 , 0 , 0 , 0 , 0 }, // 18
  { "9"                     , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 }, // 19
  { "abcdb"                 , 0 , 0 , 0 , 0 , 0 , 1 , 0 , 0 , 0 , 0 }, // 20
  { "ABCDB"                 , 0 , 0 , 0 , 0 , 0 , 1 , 0 , 0 , 0 , 0 }, // 21
  { "aBcD0"                 , 0 , 0 , 0 , 0 , 0 , 1 , 0 , 0 , 0 , 0 }, // 22
  { "efGh9"                 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 }, // 23
  { "EFGH9"                 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 }, // 24
  { "efgh9"                 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 }, // 25
  {  0                      , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 }
};

int test10(Pattern_t* pattern=test10Pattern) {
  printf("test10\n");
  int err=0;
  EasyExp e1("aBcD",EasyExp::RegEx);           
  //EasyExp e1("aBcD");
  EasyExp e2("*aBcD");           
  EasyExp e3("(aBcD|efGh|ijkl)");           
  EasyExp e4("*(aBcD|efGh|ijkl)");           
  EasyExp e5("[a-m0-5]");         
  EasyExp e6("*[a-m0-5]");         

  bool icase = true;
  int i;
  for(i=0;pattern[i].pattern;++i) {
    err += test("e1",i,e1,pattern[i].pattern,pattern[i].expect1,icase);
    err += test("e2",i,e2,pattern[i].pattern,pattern[i].expect2,icase);
    err += test("e3",i,e3,pattern[i].pattern,pattern[i].expect3,icase);
    err += test("e4",i,e4,pattern[i].pattern,pattern[i].expect4,icase);
    err += test("e5",i,e5,pattern[i].pattern,pattern[i].expect5,icase);
    err += test("e6",i,e6,pattern[i].pattern,pattern[i].expect6,icase);
  }

  EasyExp_DEBUG=3;
  //e7.match("pattern0222011yeq");
  //e7.match("pattern9112011yeq");
  //e8.match("pattern0222011yeq");
  //G__pause();
  EasyExp_DEBUG=0;
  
  return(err);
}

//////////////////////////////////////////////////////////////////////
// test 11, exact match
Pattern_t test11Pattern[]={ //e1  e2  e3  e4  e5  e6  e7  e8  e9  e10
  { "abcd"                  , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 }, // 0
  { "ABCD"                  , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 }, // 1
  { "aBcD"                  , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 }, // 2
  { "*aBcD"                 , 0 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 }, // 3
  { "(aBcD|efGh|ijkl)"      , 0 , 0 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 }, // 4
  { "*(aBcD|efGh|ijkl)"     , 0 , 0 , 0 , 1 , 0 , 0 , 0 , 0 , 0 , 0 }, // 5
  { "[a-m0-5]"              , 0 , 0 , 0 , 0 , 1 , 0 , 0 , 0 , 0 , 0 }, // 6
  { "*[a-m0-5]"             , 0 , 0 , 0 , 0 , 0 , 1 , 0 , 0 , 0 , 0 }, // 7
  {  0                      , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 }
};

int test11(Pattern_t* pattern=test11Pattern) {
  printf("test11\n");
  int err=0;
  EasyExp e1("aBcD",EasyExp::Exact);           
  EasyExp e2("*aBcD",EasyExp::Exact);           
  EasyExp e3("(aBcD|efGh|ijkl)",EasyExp::Exact);           
  EasyExp e4("*(aBcD|efGh|ijkl)",EasyExp::Exact);           
  EasyExp e5("[a-m0-5]",EasyExp::Exact);         
  EasyExp e6("*[a-m0-5]",EasyExp::Exact);         

  int i;
  for(i=0;pattern[i].pattern;++i) {
    err += test("e1",i,e1,pattern[i].pattern,pattern[i].expect1);
    err += test("e2",i,e2,pattern[i].pattern,pattern[i].expect2);
    err += test("e3",i,e3,pattern[i].pattern,pattern[i].expect3);
    err += test("e4",i,e4,pattern[i].pattern,pattern[i].expect4);
    err += test("e5",i,e5,pattern[i].pattern,pattern[i].expect5);
    err += test("e6",i,e6,pattern[i].pattern,pattern[i].expect6);
  }

  EasyExp_DEBUG=3;
  //e7.match("pattern0222011yeq");
  //e7.match("pattern9112011yeq");
  //e8.match("pattern0222011yeq");
  //G__pause();
  EasyExp_DEBUG=0;
  
  return(err);
}


//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
int quick=0,intentionalfail=0;
int main(int argc,char** argv) {
  for(int i=1;i<argc;i++) {
    if(strcmp(argv[i],"-v")==0) ++vervose;
    else if(strcmp(argv[i],"-g")==0) ++vervose;
    else if(strcmp(argv[i],"-q")==0) ++quick;
    else if(strcmp(argv[i],"-f")==0) ++intentionalfail;
    else if(argv[i][0]=='-') {
      printf("compile.cxx : Unknown option %s\n",argv[i]);
      return(0);
    }
  }
#ifdef DEBUG
  EasyExp_DEBUG=2;
#endif

  int err=0; 
  printf("### match test start\n");

  err += test1();  // StringList StarStringList with zero length string
  err += tabletest("matching_test1.csv");  

  err += test2();  // long pattern
  err += tabletest("matching_test2.csv");

  err += test3();  // String, StarString
  //err += tabletest("matching_test3.csv"); // too many special chars

  err += test4();  // StringList StarStringList
  err += tabletest("matching_test4.csv");

  err += test5();  // CharList StarCharList
  err += tabletest("matching_test5.csv");

  err += test6();  // NotCharList StarNotCharList
  err += tabletest("matching_test6.csv");

  err += test7();  // NumberRange, StarNumberRange
  err += tabletest("matching_test7.csv");

  err += test8();  // End, StarEnd, AnyChar
  err += tabletest("matching_test8.csv");

  err += test9();  // case dependent
  err += test10(); // case independent

  err += test11(); // exact match

  printf("A known problem generates 2 errors - severity:minor.\n");
  SAVE_PASSFAIL("match");
  return(err);
}

//////////////////////////////////////////////////////////////////////
