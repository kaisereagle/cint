//////////////////////////////////////////////////////////////////////
// EasyExp replace test
//
//  2015/Aug/18  Masaahru Goto
//////////////////////////////////////////////////////////////////////
#pragma include <EasyExp.dll> // Test Target
//#include <CSV.h>       // Utility, Caution:: this library also includes EasyExp. 
//#include <exception>
#include "_util_cinttest.h"

char *emptypattern[] = { 0 };

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
struct testcase_t {
  char* pattern;
  char* expect;
  int   match;
};
struct expr_t {
  char* expression;
  char* replace;
};

int pass=0;
//////////////////////////////////////////////////////////////////////
// test engine
//////////////////////////////////////////////////////////////////////
int test(const string& name,expr_t& expr,testcase_t* testcase,int mode=0,int icase=0) {
  int err=0,i=0;
  RESET_LOCALFAIL();

  EasyExp e;
  e.compile_replace(expr.expression,expr.replace,mode);

  string result;
  int m;
  while(testcase[i].pattern) {
    m=e.replace(testcase[i].pattern,result,icase);
    EXPECT_FALSE(result!=testcase[i].expect || m!=testcase[i].match) ;
    if(result!=testcase[i].expect || m!=testcase[i].match) {
      ++err;
      printf("%s %d (%s,%s) fail %s->%s!=%s || %d!=%d\n",name,i
	     ,expr.expression,expr.replace
	     ,testcase[i].pattern
	     ,result.c_str(),testcase[i].expect
	     ,m,testcase[i].match);
      e.disp();
    }
    else ++pass;
    ++i;
  }

  return(err);
}


//////////////////////////////////////////////////////////////////////
// test1,  String, StarString 
//////////////////////////////////////////////////////////////////////
expr_t expr1a = { "*.txt", "$1=.csv" };
testcase_t pat1a[] = {
  { "file1.txt"     , "file1.csv"    ,  1  }, // 0
  { "file1.csv"     , "file1.csv"    ,  0  }, // 1
  { "file1.tsv"     , "file1.tsv"    ,  0  }, // 2
  { "file1.TXT"     , "file1.TXT"    ,  0  }, // 3
  { 0               , 0              ,  0  }
};

//////////////////////////////////////////////////////////////////////
expr_t expr1b = { "*\\\\*", "$1=/" };
testcase_t pat1b[] = {
  { "c:\\cint"      , "c:/cint"      ,  1  }, // 0
  { "\\cint\\src"   , "/cint\\src"   ,  1  }, // 1
  { "file1.tsv"     , "file1.tsv"    ,  0  }, // 2
  { "file1.TXT"     , "file1.TXT"    ,  0  }, // 3
  { 0               , 0              ,  0  }
};

//////////////////////////////////////////////////////////////////////
expr_t expr1c = { "*abcd*", "$1=HIJK" };
testcase_t pat1c[] = {
  { "XAFEabcdJIEF"  , "XAFEHIJKJIEF" ,  1  }, // 0
  { "XAFEhijkJIEF"  , "XAFEhijkJIEF" ,  0  }, // 1
  { "XAFEvwxyJIEF"  , "XAFEvwxyJIEF" ,  0  }, // 2
  { "XAFEopqrJIEF"  , "XAFEopqrJIEF" ,  0  }, // 3
  { 0               , 0              ,  0  }
};

//////////////////////////////////////////////////////////////////////
expr_t expr1d = { "abcd*", "$0=HIJK" };
testcase_t pat1d[] = {
  { "abcdJIEF"  , "HIJKJIEF" ,  1  }, // 0 
  { "hijkJIEF"  , "hijkJIEF" ,  0  }, // 1 
  { "vwxyJIEF"  , "vwxyJIEF" ,  0  }, // 2 
  { "opqrJIEF"  , "opqrJIEF" ,  0  }, // 3 
  { 0           , 0          ,  0  }
};

//////////////////////////////////////////////////////////////////////
expr_t expr1e = { "abcd*", "$0=" };
testcase_t pat1e[] = {
  { "abcdJIEF"  , "JIEF"     ,  1  }, // 0 
  { "hijkJIEF"  , "hijkJIEF" ,  0  }, // 1 
  { "vwxyJIEF"  , "vwxyJIEF" ,  0  }, // 2 
  { "opqrJIEF"  , "opqrJIEF" ,  0  }, // 3 
  { 0           , 0          ,  0  }
};

//////////////////////////////////////////////////////////////////////
expr_t expr1f = { "abcd*", "$1=" };
testcase_t pat1f[] = {
  { "abcdJIEF"  , "abcd"     ,  1  }, // 0 
  { "hijkJIEF"  , "hijkJIEF" ,  0  }, // 1 
  { "vwxyJIEF"  , "vwxyJIEF" ,  0  }, // 2 
  { "opqrJIEF"  , "opqrJIEF" ,  0  }, // 3 
  { 0           , 0          ,  0  }
};

//////////////////////////////////////////////////////////////////////
// test2, string list, star string list
//////////////////////////////////////////////////////////////////////
expr_t expr2a = { "(abcd|hijk|vwxy)*", "$0=(ABCD|HIJK|VWXY)" };
testcase_t pat2a[] = {
  { "abcdJIEF"  , "ABCDJIEF" ,  1  }, // 0
  { "hijkJIEF"  , "HIJKJIEF" ,  1  }, // 1
  { "vwxyJIEF"  , "VWXYJIEF" ,  1  }, // 2
  { "opqrJIEF"  , "opqrJIEF" ,  0  }, // 3
  { 0               , 0              ,  0  }
};

//////////////////////////////////////////////////////////////////////
expr_t expr2b = { "(abcd|hijk|vwxy|0123)*", "$0=(ABCD|HIJK|VWXY)" };
testcase_t pat2b[] = {
  { "abcdJIEF"  , "ABCDJIEF" ,  1  }, // 0 
  { "hijkJIEF"  , "HIJKJIEF" ,  1  }, // 1 
  { "vwxyJIEF"  , "VWXYJIEF" ,  1  }, // 2 
  { "opqrJIEF"  , "opqrJIEF" ,  0  }, // 3 
  { "0123JIEF"  , "0123JIEF" ,  1  }, // 4 
  { 0               , 0              ,  0  }
};

//////////////////////////////////////////////////////////////////////
expr_t expr2c = { "*(abcd|hijk|vwxy)*", "$1=(ABCD|HIJK|VWXY)" };
testcase_t pat2c[] = {
  { "XAFEabcdJIEF"  , "XAFEABCDJIEF" ,  1  }, // 0 
  { "XAFEhijkJIEF"  , "XAFEHIJKJIEF" ,  1  }, // 1 
  { "XAFEvwxyJIEF"  , "XAFEVWXYJIEF" ,  1  }, // 2 
  { "XAFEopqrJIEF"  , "XAFEopqrJIEF" ,  0  }, // 3 
  { 0               , 0              ,  0  } 
};

//////////////////////////////////////////////////////////////////////
expr_t expr2d = { "*(abcd|hijk|vwxy|0123)*", "$1=(ABCD|HIJK|VWXY)" };
testcase_t pat2d[] = {
  { "XAFEabcdJIEF"  , "XAFEABCDJIEF" ,  1  }, // 0 
  { "XAFEhijkJIEF"  , "XAFEHIJKJIEF" ,  1  }, // 1 
  { "XAFEvwxyJIEF"  , "XAFEVWXYJIEF" ,  1  }, // 2 
  { "XAFEopqrJIEF"  , "XAFEopqrJIEF" ,  0  }, // 3 
  { "XAFE0123JIEF"  , "XAFE0123JIEF" ,  1  }, // 4 
  { 0               , 0              ,  0  }
};

//////////////////////////////////////////////////////////////////////
expr_t expr2e = { "(abcd|hijk|vwxy)*", "$0=0123" };
testcase_t pat2e[] = {
  { "abcdJIEF"  , "0123JIEF" ,  1  }, // 0 
  { "hijkJIEF"  , "0123JIEF" ,  1  }, // 1 
  { "vwxyJIEF"  , "0123JIEF" ,  1  }, // 2 
  { "opqrJIEF"  , "opqrJIEF" ,  0  }, // 3 
  { 0               , 0              ,  0  } 
};

//////////////////////////////////////////////////////////////////////
expr_t expr2f = { "*(abcd|hijk|vwxy|0123)*", "$1=0123" };
testcase_t pat2f[] = {
  { "XAFEabcdJIEF"  , "XAFE0123JIEF" ,  1  }, // 0 
  { "XAFEhijkJIEF"  , "XAFE0123JIEF" ,  1  }, // 1 
  { "XAFEvwxyJIEF"  , "XAFE0123JIEF" ,  1  }, // 2 
  { "XAFEopqrJIEF"  , "XAFEopqrJIEF" ,  0  }, // 3 
  { "XAFE0123JIEF"  , "XAFE0123JIEF" ,  1  }, // 4 
  { 0               , 0              ,  0  }
};

//////////////////////////////////////////////////////////////////////
expr_t expr2g = { "(abcd|hijk|vwxy)*", "$0=(2000..2010)" };
testcase_t pat2g[] = {
  { "abcdJIEF"  , "2000JIEF" ,  1  }, // 0 
  { "hijkJIEF"  , "2001JIEF" ,  1  }, // 1 
  { "vwxyJIEF"  , "2002JIEF" ,  1  }, // 2 
  { "opqrJIEF"  , "opqrJIEF" ,  0  }, // 3 
  { 0               , 0              ,  0  } 
};

//////////////////////////////////////////////////////////////////////
expr_t expr2h = { "*(abcd|hijk|vwxy|0123)*", "$1=(2000..2010)" };
testcase_t pat2h[] = {
  { "XAFEabcdJIEF"  , "XAFE2000JIEF" ,  1  }, // 0 
  { "XAFEhijkJIEF"  , "XAFE2001JIEF" ,  1  }, // 1 
  { "XAFEvwxyJIEF"  , "XAFE2002JIEF" ,  1  }, // 2 
  { "XAFEopqrJIEF"  , "XAFEopqrJIEF" ,  0  }, // 3 
  { "XAFE0123JIEF"  , "XAFE2003JIEF" ,  1  }, // 4 
  { 0               , 0              ,  0  }
};

//////////////////////////////////////////////////////////////////////
// test3, number range, star number range
//////////////////////////////////////////////////////////////////////
expr_t expr3a = { "(1995..2015)*", "$0=(2000..2020)" };
testcase_t pat3a[] = {
  { "1995"     , "2000"    ,  1  }, // 0 
  { "2000"     , "2005"    ,  1  }, // 1 
  { "1994"     , "1994"    ,  0  }, // 2 
  { "2015"     , "2020"    ,  1  }, // 3 
  { "2016"     , "2016"    ,  0  }, // 4 
  { "1995abc"  , "2000abc" ,  1  }, // 5 
  { "2000abc"  , "2005abc" ,  1  }, // 6 
  { "1994abc"  , "1994abc" ,  0  }, // 7 
  { "2015abc"  , "2020abc" ,  1  }, // 8 
  { "2016abc"  , "2016abc" ,  0  }, // 9 
  { 0               , 0              ,  0  }
};

//////////////////////////////////////////////////////////////////////
expr_t expr3b = { "*(1995..2015)*", "$1=(2000..2020)" };
testcase_t pat3b[] = {
  { "xyz1995"     , "xyz2000"    ,  1  }, // 0 
  { "xyz2000"     , "xyz2005"    ,  1  }, // 1 
  { "xyz1994"     , "xyz1994"    ,  0  }, // 2 
  { "xyz2015"     , "xyz2020"    ,  1  }, // 3 
  { "xyz2016"     , "xyz2016"    ,  0  }, // 4 
  { "xyz1995abc"  , "xyz2000abc" ,  1  }, // 5 
  { "xyz2000abc"  , "xyz2005abc" ,  1  }, // 6 
  { "xyz1994abc"  , "xyz1994abc" ,  0  }, // 7 
  { "xyz2015abc"  , "xyz2020abc" ,  1  }, // 8 
  { "xyz2016abc"  , "xyz2016abc" ,  0  }, // 9 
  { 0               , 0              ,  0  }
};

//////////////////////////////////////////////////////////////////////
expr_t expr3c = { "(1995..2015)*", "$0=(2000..2010)" };
testcase_t pat3c[] = {
  { "1995"     , "2000"    ,  1  }, // 0 
  { "2000"     , "2005"    ,  1  }, // 1 
  { "1994"     , "1994"    ,  0  }, // 2 
  { "2015"     , "2015"    ,  1  }, // 3 
  { "2016"     , "2016"    ,  0  }, // 4 
  { "1995abc"  , "2000abc" ,  1  }, // 5 
  { "2000abc"  , "2005abc" ,  1  }, // 6 
  { "1994abc"  , "1994abc" ,  0  }, // 7 
  { "2015abc"  , "2015abc" ,  1  }, // 8 
  { "2016abc"  , "2016abc" ,  0  }, // 9 
  { 0               , 0              ,  0  }
};

//////////////////////////////////////////////////////////////////////
expr_t expr3d = { "*(1995..2015)*", "$1=(2000..2010)" };
testcase_t pat3d[] = {
  { "xyz1995"     , "xyz2000"    ,  1  }, // 0
  { "xyz2000"     , "xyz2005"    ,  1  }, // 1
  { "xyz1994"     , "xyz1994"    ,  0  }, // 2
  { "xyz2015"     , "xyz2015"    ,  1  }, // 3
  { "xyz2016"     , "xyz2016"    ,  0  }, // 4
  { "xyz1995abc"  , "xyz2000abc" ,  1  }, // 5
  { "xyz2000abc"  , "xyz2005abc" ,  1  }, // 6
  { "xyz1994abc"  , "xyz1994abc" ,  0  }, // 7
  { "xyz2015abc"  , "xyz2015abc" ,  1  }, // 8
  { "xyz2016abc"  , "xyz2016abc" ,  0  }, // 9
  { 0               , 0              ,  0  }
};

//////////////////////////////////////////////////////////////////////
expr_t expr3e = { "(1995..2015)*", "$0=abcd" };
testcase_t pat3e[] = {
  { "1995"     , "abcd"    ,  1  }, // 0 
  { "2000"     , "abcd"    ,  1  }, // 1 
  { "1994"     , "1994"    ,  0  }, // 2 
  { "2015"     , "abcd"    ,  1  }, // 3 
  { "2016"     , "2016"    ,  0  }, // 4 
  { "1995abc"  , "abcdabc" ,  1  }, // 5 
  { "2000abc"  , "abcdabc" ,  1  }, // 6 
  { "1994abc"  , "1994abc" ,  0  }, // 7 
  { "2015abc"  , "abcdabc" ,  1  }, // 8 
  { "2016abc"  , "2016abc" ,  0  }, // 9 
  { 0               , 0              ,  0  }
};

//////////////////////////////////////////////////////////////////////
expr_t expr3f = { "*(1995..2015)*", "$1=abcd" };
testcase_t pat3f[] = {
  { "xyz1995"     , "xyzabcd"    ,  1  }, // 0
  { "xyz2000"     , "xyzabcd"    ,  1  }, // 1
  { "xyz1994"     , "xyz1994"    ,  0  }, // 2
  { "xyz2015"     , "xyzabcd"    ,  1  }, // 3
  { "xyz2016"     , "xyz2016"    ,  0  }, // 4
  { "xyz1995abc"  , "xyzabcdabc" ,  1  }, // 5
  { "xyz2000abc"  , "xyzabcdabc" ,  1  }, // 6
  { "xyz1994abc"  , "xyz1994abc" ,  0  }, // 7
  { "xyz2015abc"  , "xyzabcdabc" ,  1  }, // 8
  { "xyz2016abc"  , "xyz2016abc" ,  0  }, // 9
  { 0               , 0              ,  0  }
};

//////////////////////////////////////////////////////////////////////
expr_t expr3g = { "(1995..2015)*", "$0=(apple|orange|pear|banana)" };
testcase_t pat3g[] = {
  { "1995"     , "apple"     ,  1  }, // 0 
  { "1996"     , "orange"    ,  1  }, // 1 
  { "1994"     , "1994"      ,  0  }, // 2 
  { "2015"     , "2015"      ,  1  }, // 3 
  { "2016"     , "2016"      ,  0  }, // 4 
  { "1995abc"  , "appleabc"  ,  1  }, // 5 
  { "1997abc"  , "pearabc"   ,  1  }, // 6 
  { "1998abc"  , "bananaabc" ,  1  }, // 7 
  { "2015abc"  , "2015abc"   ,  1  }, // 8 
  { "2016abc"  , "2016abc"   ,  0  }, // 9 
  { 0               , 0              ,  0  }
};

//////////////////////////////////////////////////////////////////////
expr_t expr3h = { "*(1995..2015)*", "$1=(apple|orange|pear|banana)" };
testcase_t pat3h[] = {
  { "xyz1995"     , "xyzapple"    ,  1  }, // 0
  { "xyz1996"     , "xyzorange"   ,  1  }, // 1
  { "xyz1997"     , "xyzpear"     ,  1  }, // 2
  { "xyz1998"     , "xyzbanana"   ,  1  }, // 3
  { "xyz1999"     , "xyz1999"     ,  1  }, // 4
  { "xyz1994abc"  , "xyz1994abc"  ,  0  }, // 5
  { "xyz1995abc"  , "xyzappleabc" ,  1  }, // 6
  { "xyz1996abc"  , "xyzorangeabc",  1  }, // 7
  { "xyz1997abc"  , "xyzpearabc"  ,  1  }, // 8
  { "xyz1998abc"  , "xyzbananaabc",  1  }, // 9
  { "xyz1999abc"  , "xyz1999abc"  ,  1  }, // 10
  { "xyz2000abc"  , "xyz2000abc"  ,  1  }, // 11
  { "xyz2015abc"  , "xyz2015abc"  ,  1  }, // 12
  { "xyz2016abc"  , "xyz2016abc"  ,  0  }, // 13
  { 0               , 0              ,  0  }
};

//////////////////////////////////////////////////////////////////////
// test4, number range, star number range
//////////////////////////////////////////////////////////////////////
expr_t expr4a = { "[a-d]*", "$0=[A-D]" };
testcase_t pat4a[] = {
  { "a"             , "A"            ,  1  }, // 0 
  { "b"             , "B"            ,  1  }, // 1 
  { "c"             , "C"            ,  1  }, // 2 
  { "d"             , "D"            ,  1  }, // 3 
  { "e"             , "e"            ,  0  }, // 4 
  { "A"             , "A"            ,  0  }, // 5 
  { "0"             , "0"            ,  0  }, // 6 
  { "1"             , "1"            ,  0  }, // 7 
  { "2"             , "2"            ,  0  }, // 8 
  { "3"             , "3"            ,  0  }, // 9 
  { "4"             , "4"            ,  0  }, // 10
  { "axyz"          , "Axyz"         ,  1  }, // 11
  { "bxyz"          , "Bxyz"         ,  1  }, // 12
  { "cxyz"          , "Cxyz"         ,  1  }, // 13
  { "dxyz"          , "Dxyz"         ,  1  }, // 14
  { "exyz"          , "exyz"         ,  0  }, // 15
  { "Axyz"          , "Axyz"         ,  0  }, // 16
  { "0xyz"          , "0xyz"         ,  0  }, // 17
  { "1xyz"          , "1xyz"         ,  0  }, // 18
  { "2xyz"          , "2xyz"         ,  0  }, // 19
  { "3xyz"          , "3xyz"         ,  0  }, // 20
  { "4xyz"          , "4xyz"         ,  0  }, // 21
  { 0               , 0              ,  0  }
};

//////////////////////////////////////////////////////////////////////
expr_t expr4b = { "*[a-d]*", "$1=[A-D]" };
testcase_t pat4b[] = {
  { "a"             , "A"            ,  1  }, // 0 
  { "b"             , "B"            ,  1  }, // 1 
  { "c"             , "C"            ,  1  }, // 2 
  { "d"             , "D"            ,  1  }, // 3 
  { "e"             , "e"            ,  0  }, // 4 
  { "A"             , "A"            ,  0  }, // 5 
  { "0"             , "0"            ,  0  }, // 6 
  { "1"             , "1"            ,  0  }, // 7 
  { "2"             , "2"            ,  0  }, // 8 
  { "3"             , "3"            ,  0  }, // 9 
  { "4"             , "4"            ,  0  }, // 10
  { "axyz"          , "Axyz"         ,  1  }, // 11
  { "bxyz"          , "Bxyz"         ,  1  }, // 12
  { "cxyz"          , "Cxyz"         ,  1  }, // 13
  { "dxyz"          , "Dxyz"         ,  1  }, // 14
  { "exyz"          , "exyz"         ,  0  }, // 15
  { "Axyz"          , "Axyz"         ,  0  }, // 16
  { "0xyz"          , "0xyz"         ,  0  }, // 17
  { "1xyz"          , "1xyz"         ,  0  }, // 18
  { "2xyz"          , "2xyz"         ,  0  }, // 19
  { "3xyz"          , "3xyz"         ,  0  }, // 20
  { "4xyz"          , "4xyz"         ,  0  }, // 21
  { "lmna"          , "lmnA"         ,  1  }, // 22
  { "lmnb"          , "lmnB"         ,  1  }, // 23
  { "lmnc"          , "lmnC"         ,  1  }, // 24
  { "lmnd"          , "lmnD"         ,  1  }, // 25
  { "lmne"          , "lmne"         ,  0  }, // 26
  { "lmnA"          , "lmnA"         ,  0  }, // 27
  { "lmn0"          , "lmn0"         ,  0  }, // 28
  { "lmn1"          , "lmn1"         ,  0  }, // 29
  { "lmn2"          , "lmn2"         ,  0  }, // 30
  { "lmn3"          , "lmn3"         ,  0  }, // 31
  { "lmn4"          , "lmn4"         ,  0  }, // 32
  { "lmnaxyz"       , "lmnAxyz"      ,  1  }, // 33
  { "lmnbxyz"       , "lmnBxyz"      ,  1  }, // 34
  { "lmncxyz"       , "lmnCxyz"      ,  1  }, // 35
  { "lmndxyz"       , "lmnDxyz"      ,  1  }, // 36
  { "lmnexyz"       , "lmnexyz"      ,  0  }, // 37
  { "lmnAxyz"       , "lmnAxyz"      ,  0  }, // 38
  { "lmn0xyz"       , "lmn0xyz"      ,  0  }, // 39
  { "lmn1xyz"       , "lmn1xyz"      ,  0  }, // 40
  { "lmn2xyz"       , "lmn2xyz"      ,  0  }, // 41
  { "lmn3xyz"       , "lmn3xyz"      ,  0  }, // 42
  { "lmn4xyz"       , "lmn4xyz"      ,  0  }, // 43
  { 0               , 0              ,  0  }
};

//////////////////////////////////////////////////////////////////////
expr_t expr4c = { "[a-d]*", "$0=0123" };
testcase_t pat4c[] = {
  { "a"             , "0123"         ,  1  }, // 0 
  { "b"             , "0123"         ,  1  }, // 1 
  { "c"             , "0123"         ,  1  }, // 2 
  { "d"             , "0123"         ,  1  }, // 3 
  { "e"             , "e"            ,  0  }, // 4 
  { "A"             , "A"            ,  0  }, // 5 
  { "0"             , "0"            ,  0  }, // 6 
  { "1"             , "1"            ,  0  }, // 7 
  { "2"             , "2"            ,  0  }, // 8 
  { "3"             , "3"            ,  0  }, // 9 
  { "4"             , "4"            ,  0  }, // 10
  { "axyz"          , "0123xyz"      ,  1  }, // 11
  { "bxyz"          , "0123xyz"      ,  1  }, // 12
  { "cxyz"          , "0123xyz"      ,  1  }, // 13
  { "dxyz"          , "0123xyz"      ,  1  }, // 14
  { "exyz"          , "exyz"         ,  0  }, // 15
  { "Axyz"          , "Axyz"         ,  0  }, // 16
  { "0xyz"          , "0xyz"         ,  0  }, // 17
  { "1xyz"          , "1xyz"         ,  0  }, // 18
  { "2xyz"          , "2xyz"         ,  0  }, // 19
  { "3xyz"          , "3xyz"         ,  0  }, // 20
  { "4xyz"          , "4xyz"         ,  0  }, // 21
  { "lmna"          , "lmna"         ,  0  }, // 22
  { "lmnb"          , "lmnb"         ,  0  }, // 23
  { "lmnc"          , "lmnc"         ,  0  }, // 24
  { "lmnd"          , "lmnd"         ,  0  }, // 25
  { "lmne"          , "lmne"         ,  0  }, // 26
  { "lmnA"          , "lmnA"         ,  0  }, // 27
  { "lmn0"          , "lmn0"         ,  0  }, // 28
  { "lmn1"          , "lmn1"         ,  0  }, // 29
  { "lmn2"          , "lmn2"         ,  0  }, // 30
  { "lmn3"          , "lmn3"         ,  0  }, // 31
  { "lmn4"          , "lmn4"         ,  0  }, // 32
  { "lmnaxyz"       , "lmnaxyz"      ,  0  }, // 33
  { "lmnbxyz"       , "lmnbxyz"      ,  0  }, // 34
  { "lmncxyz"       , "lmncxyz"      ,  0  }, // 35
  { "lmndxyz"       , "lmndxyz"      ,  0  }, // 36
  { "lmnexyz"       , "lmnexyz"      ,  0  }, // 37
  { "lmnAxyz"       , "lmnAxyz"      ,  0  }, // 38
  { "lmn0xyz"       , "lmn0xyz"      ,  0  }, // 39
  { "lmn1xyz"       , "lmn1xyz"      ,  0  }, // 40
  { "lmn2xyz"       , "lmn2xyz"      ,  0  }, // 41
  { "lmn3xyz"       , "lmn3xyz"      ,  0  }, // 42
  { "lmn4xyz"       , "lmn4xyz"      ,  0  }, // 43
  { 0               , 0              ,  0  }
};

//////////////////////////////////////////////////////////////////////
expr_t expr4d = { "*[a-d]*", "$1=0123" };
testcase_t pat4d[] = {
  { "a"             , "0123"         ,  1  }, // 0 
  { "b"             , "0123"         ,  1  }, // 1 
  { "c"             , "0123"         ,  1  }, // 2 
  { "d"             , "0123"         ,  1  }, // 3 
  { "e"             , "e"            ,  0  }, // 4 
  { "A"             , "A"            ,  0  }, // 5 
  { "0"             , "0"            ,  0  }, // 6 
  { "1"             , "1"            ,  0  }, // 7 
  { "2"             , "2"            ,  0  }, // 8 
  { "3"             , "3"            ,  0  }, // 9 
  { "4"             , "4"            ,  0  }, // 10
  { "axyz"          , "0123xyz"      ,  1  }, // 11
  { "bxyz"          , "0123xyz"      ,  1  }, // 12
  { "cxyz"          , "0123xyz"      ,  1  }, // 13
  { "dxyz"          , "0123xyz"      ,  1  }, // 14
  { "exyz"          , "exyz"         ,  0  }, // 15
  { "Axyz"          , "Axyz"         ,  0  }, // 16
  { "0xyz"          , "0xyz"         ,  0  }, // 17
  { "1xyz"          , "1xyz"         ,  0  }, // 18
  { "2xyz"          , "2xyz"         ,  0  }, // 19
  { "3xyz"          , "3xyz"         ,  0  }, // 20
  { "4xyz"          , "4xyz"         ,  0  }, // 21
  { "lmna"          , "lmn0123"      ,  1  }, // 22
  { "lmnb"          , "lmn0123"      ,  1  }, // 23
  { "lmnc"          , "lmn0123"      ,  1  }, // 24
  { "lmnd"          , "lmn0123"      ,  1  }, // 25
  { "lmne"          , "lmne"         ,  0  }, // 26
  { "lmnA"          , "lmnA"         ,  0  }, // 27
  { "lmn0"          , "lmn0"         ,  0  }, // 28
  { "lmn1"          , "lmn1"         ,  0  }, // 29
  { "lmn2"          , "lmn2"         ,  0  }, // 30
  { "lmn3"          , "lmn3"         ,  0  }, // 31
  { "lmn4"          , "lmn4"         ,  0  }, // 32
  { "lmnaxyz"       , "lmn0123xyz"   ,  1  }, // 33
  { "lmnbxyz"       , "lmn0123xyz"   ,  1  }, // 34
  { "lmncxyz"       , "lmn0123xyz"   ,  1  }, // 35
  { "lmndxyz"       , "lmn0123xyz"   ,  1  }, // 36
  { "lmnexyz"       , "lmnexyz"      ,  0  }, // 37
  { "lmnAxyz"       , "lmnAxyz"      ,  0  }, // 38
  { "lmn0xyz"       , "lmn0xyz"      ,  0  }, // 39
  { "lmn1xyz"       , "lmn1xyz"      ,  0  }, // 40
  { "lmn2xyz"       , "lmn2xyz"      ,  0  }, // 41
  { "lmn3xyz"       , "lmn3xyz"      ,  0  }, // 42
  { "lmn4xyz"       , "lmn4xyz"      ,  0  }, // 43
  { 0               , 0              ,  0  }
};

//////////////////////////////////////////////////////////////////////
expr_t expr4e = { "[a-d]*", "$0=(2000..2010)" };
testcase_t pat4e[] = {
  { "a"             , "2000"         ,  1  }, // 0 
  { "b"             , "2001"         ,  1  }, // 1 
  { "c"             , "2002"         ,  1  }, // 2 
  { "d"             , "2003"         ,  1  }, // 3 
  { "e"             , "e"            ,  0  }, // 4 
  { "A"             , "A"            ,  0  }, // 5 
  { "0"             , "0"            ,  0  }, // 6 
  { "1"             , "1"            ,  0  }, // 7 
  { "2"             , "2"            ,  0  }, // 8 
  { "3"             , "3"            ,  0  }, // 9 
  { "4"             , "4"            ,  0  }, // 10
  { "axyz"          , "2000xyz"      ,  1  }, // 11
  { "bxyz"          , "2001xyz"      ,  1  }, // 12
  { "cxyz"          , "2002xyz"      ,  1  }, // 13
  { "dxyz"          , "2003xyz"      ,  1  }, // 14
  { "exyz"          , "exyz"         ,  0  }, // 15
  { "Axyz"          , "Axyz"         ,  0  }, // 16
  { "0xyz"          , "0xyz"         ,  0  }, // 17
  { "1xyz"          , "1xyz"         ,  0  }, // 18
  { "2xyz"          , "2xyz"         ,  0  }, // 19
  { "3xyz"          , "3xyz"         ,  0  }, // 20
  { "4xyz"          , "4xyz"         ,  0  }, // 21
  { "lmna"          , "lmna"         ,  0  }, // 22
  { "lmnb"          , "lmnb"         ,  0  }, // 23
  { "lmnc"          , "lmnc"         ,  0  }, // 24
  { "lmnd"          , "lmnd"         ,  0  }, // 25
  { "lmne"          , "lmne"         ,  0  }, // 26
  { "lmnA"          , "lmnA"         ,  0  }, // 27
  { "lmn0"          , "lmn0"         ,  0  }, // 28
  { "lmn1"          , "lmn1"         ,  0  }, // 29
  { "lmn2"          , "lmn2"         ,  0  }, // 30
  { "lmn3"          , "lmn3"         ,  0  }, // 31
  { "lmn4"          , "lmn4"         ,  0  }, // 32
  { "lmnaxyz"       , "lmnaxyz"      ,  0  }, // 33
  { "lmnbxyz"       , "lmnbxyz"      ,  0  }, // 34
  { "lmncxyz"       , "lmncxyz"      ,  0  }, // 35
  { "lmndxyz"       , "lmndxyz"      ,  0  }, // 36
  { "lmnexyz"       , "lmnexyz"      ,  0  }, // 37
  { "lmnAxyz"       , "lmnAxyz"      ,  0  }, // 38
  { "lmn0xyz"       , "lmn0xyz"      ,  0  }, // 39
  { "lmn1xyz"       , "lmn1xyz"      ,  0  }, // 40
  { "lmn2xyz"       , "lmn2xyz"      ,  0  }, // 41
  { "lmn3xyz"       , "lmn3xyz"      ,  0  }, // 42
  { "lmn4xyz"       , "lmn4xyz"      ,  0  }, // 43
  { 0               , 0              ,  0  }
};

//////////////////////////////////////////////////////////////////////
expr_t expr4f = { "*[a-d]*", "$1=(2000..2010)" };
testcase_t pat4f[] = {
  { "a"             , "2000"         ,  1  }, // 0 
  { "b"             , "2001"         ,  1  }, // 1 
  { "c"             , "2002"         ,  1  }, // 2 
  { "d"             , "2003"         ,  1  }, // 3 
  { "e"             , "e"            ,  0  }, // 4 
  { "A"             , "A"            ,  0  }, // 5 
  { "0"             , "0"            ,  0  }, // 6 
  { "1"             , "1"            ,  0  }, // 7 
  { "2"             , "2"            ,  0  }, // 8 
  { "3"             , "3"            ,  0  }, // 9 
  { "4"             , "4"            ,  0  }, // 10
  { "axyz"          , "2000xyz"      ,  1  }, // 11
  { "bxyz"          , "2001xyz"      ,  1  }, // 12
  { "cxyz"          , "2002xyz"      ,  1  }, // 13
  { "dxyz"          , "2003xyz"      ,  1  }, // 14
  { "exyz"          , "exyz"         ,  0  }, // 15
  { "Axyz"          , "Axyz"         ,  0  }, // 16
  { "0xyz"          , "0xyz"         ,  0  }, // 17
  { "1xyz"          , "1xyz"         ,  0  }, // 18
  { "2xyz"          , "2xyz"         ,  0  }, // 19
  { "3xyz"          , "3xyz"         ,  0  }, // 20
  { "4xyz"          , "4xyz"         ,  0  }, // 21
  { "lmna"          , "lmn2000"      ,  1  }, // 22
  { "lmnb"          , "lmn2001"      ,  1  }, // 23
  { "lmnc"          , "lmn2002"      ,  1  }, // 24
  { "lmnd"          , "lmn2003"      ,  1  }, // 25
  { "lmne"          , "lmne"         ,  0  }, // 26
  { "lmnA"          , "lmnA"         ,  0  }, // 27
  { "lmn0"          , "lmn0"         ,  0  }, // 28
  { "lmn1"          , "lmn1"         ,  0  }, // 29
  { "lmn2"          , "lmn2"         ,  0  }, // 30
  { "lmn3"          , "lmn3"         ,  0  }, // 31
  { "lmn4"          , "lmn4"         ,  0  }, // 32
  { "lmnaxyz"       , "lmn2000xyz"   ,  1  }, // 33
  { "lmnbxyz"       , "lmn2001xyz"   ,  1  }, // 34
  { "lmncxyz"       , "lmn2002xyz"   ,  1  }, // 35
  { "lmndxyz"       , "lmn2003xyz"   ,  1  }, // 36
  { "lmnexyz"       , "lmnexyz"      ,  0  }, // 37
  { "lmnAxyz"       , "lmnAxyz"      ,  0  }, // 38
  { "lmn0xyz"       , "lmn0xyz"      ,  0  }, // 39
  { "lmn1xyz"       , "lmn1xyz"      ,  0  }, // 40
  { "lmn2xyz"       , "lmn2xyz"      ,  0  }, // 41
  { "lmn3xyz"       , "lmn3xyz"      ,  0  }, // 42
  { "lmn4xyz"       , "lmn4xyz"      ,  0  }, // 43
  { 0               , 0              ,  0  }
};

//////////////////////////////////////////////////////////////////////
expr_t expr4g = { "[a-d]*", "$0=(A|B|C)" };
testcase_t pat4g[] = {
  { "a"             , "A"            ,  1  }, // 0 
  { "b"             , "B"            ,  1  }, // 1 
  { "c"             , "C"            ,  1  }, // 2 
  { "d"             , "d"            ,  1  }, // 3 
  { "e"             , "e"            ,  0  }, // 4 
  { "A"             , "A"            ,  0  }, // 5 
  { "0"             , "0"            ,  0  }, // 6 
  { "1"             , "1"            ,  0  }, // 7 
  { "2"             , "2"            ,  0  }, // 8 
  { "3"             , "3"            ,  0  }, // 9 
  { "4"             , "4"            ,  0  }, // 10
  { "axyz"          , "Axyz"         ,  1  }, // 11
  { "bxyz"          , "Bxyz"         ,  1  }, // 12
  { "cxyz"          , "Cxyz"         ,  1  }, // 13
  { "dxyz"          , "dxyz"         ,  1  }, // 14
  { "exyz"          , "exyz"         ,  0  }, // 15
  { "Axyz"          , "Axyz"         ,  0  }, // 16
  { "0xyz"          , "0xyz"         ,  0  }, // 17
  { "1xyz"          , "1xyz"         ,  0  }, // 18
  { "2xyz"          , "2xyz"         ,  0  }, // 19
  { "3xyz"          , "3xyz"         ,  0  }, // 20
  { "4xyz"          , "4xyz"         ,  0  }, // 21
  { 0               , 0              ,  0  }
};

//////////////////////////////////////////////////////////////////////
expr_t expr4h = { "*[a-d]*", "$1=(A|B|C)" };
testcase_t pat4h[] = {
  { "a"             , "A"            ,  1  }, // 0 
  { "b"             , "B"            ,  1  }, // 1 
  { "c"             , "C"            ,  1  }, // 2 
  { "d"             , "d"            ,  1  }, // 3 
  { "e"             , "e"            ,  0  }, // 4 
  { "A"             , "A"            ,  0  }, // 5 
  { "0"             , "0"            ,  0  }, // 6 
  { "1"             , "1"            ,  0  }, // 7 
  { "2"             , "2"            ,  0  }, // 8 
  { "3"             , "3"            ,  0  }, // 9 
  { "4"             , "4"            ,  0  }, // 10
  { "axyz"          , "Axyz"         ,  1  }, // 11
  { "bxyz"          , "Bxyz"         ,  1  }, // 12
  { "cxyz"          , "Cxyz"         ,  1  }, // 13
  { "dxyz"          , "dxyz"         ,  1  }, // 14
  { "exyz"          , "exyz"         ,  0  }, // 15
  { "Axyz"          , "Axyz"         ,  0  }, // 16
  { "0xyz"          , "0xyz"         ,  0  }, // 17
  { "1xyz"          , "1xyz"         ,  0  }, // 18
  { "2xyz"          , "2xyz"         ,  0  }, // 19
  { "3xyz"          , "3xyz"         ,  0  }, // 20
  { "4xyz"          , "4xyz"         ,  0  }, // 21
  { "lmna"          , "lmnA"         ,  1  }, // 22
  { "lmnb"          , "lmnB"         ,  1  }, // 23
  { "lmnc"          , "lmnC"         ,  1  }, // 24
  { "lmnd"          , "lmnd"         ,  1  }, // 25
  { "lmne"          , "lmne"         ,  0  }, // 26
  { "lmnA"          , "lmnA"         ,  0  }, // 27
  { "lmn0"          , "lmn0"         ,  0  }, // 28
  { "lmn1"          , "lmn1"         ,  0  }, // 29
  { "lmn2"          , "lmn2"         ,  0  }, // 30
  { "lmn3"          , "lmn3"         ,  0  }, // 31
  { "lmn4"          , "lmn4"         ,  0  }, // 32
  { "lmnaxyz"       , "lmnAxyz"      ,  1  }, // 33
  { "lmnbxyz"       , "lmnBxyz"      ,  1  }, // 34
  { "lmncxyz"       , "lmnCxyz"      ,  1  }, // 35
  { "lmndxyz"       , "lmndxyz"      ,  1  }, // 36
  { "lmnexyz"       , "lmnexyz"      ,  0  }, // 37
  { "lmnAxyz"       , "lmnAxyz"      ,  0  }, // 38
  { "lmn0xyz"       , "lmn0xyz"      ,  0  }, // 39
  { "lmn1xyz"       , "lmn1xyz"      ,  0  }, // 40
  { "lmn2xyz"       , "lmn2xyz"      ,  0  }, // 41
  { "lmn3xyz"       , "lmn3xyz"      ,  0  }, // 42
  { "lmn4xyz"       , "lmn4xyz"      ,  0  }, // 43
  { 0               , 0              ,  0  }
};

//////////////////////////////////////////////////////////////////////
expr_t expr4i = { "[a-d]*", "$0=[A-C]" };
testcase_t pat4i[] = {
  { "a"             , "A"            ,  1  }, // 0 
  { "b"             , "B"            ,  1  }, // 1 
  { "c"             , "C"            ,  1  }, // 2 
  { "d"             , "d"            ,  1  }, // 3 
  { "e"             , "e"            ,  0  }, // 4 
  { "A"             , "A"            ,  0  }, // 5 
  { "0"             , "0"            ,  0  }, // 6 
  { "1"             , "1"            ,  0  }, // 7 
  { "2"             , "2"            ,  0  }, // 8 
  { "3"             , "3"            ,  0  }, // 9 
  { "4"             , "4"            ,  0  }, // 10
  { "axyz"          , "Axyz"         ,  1  }, // 11
  { "bxyz"          , "Bxyz"         ,  1  }, // 12
  { "cxyz"          , "Cxyz"         ,  1  }, // 13
  { "dxyz"          , "dxyz"         ,  1  }, // 14
  { "exyz"          , "exyz"         ,  0  }, // 15
  { "Axyz"          , "Axyz"         ,  0  }, // 16
  { "0xyz"          , "0xyz"         ,  0  }, // 17
  { "1xyz"          , "1xyz"         ,  0  }, // 18
  { "2xyz"          , "2xyz"         ,  0  }, // 19
  { "3xyz"          , "3xyz"         ,  0  }, // 20
  { "4xyz"          , "4xyz"         ,  0  }, // 21
  { 0               , 0              ,  0  }
};

//////////////////////////////////////////////////////////////////////
expr_t expr4j = { "*[a-d]*", "$1=[A-C]" };
testcase_t pat4j[] = {
  { "a"             , "A"            ,  1  }, // 0 
  { "b"             , "B"            ,  1  }, // 1 
  { "c"             , "C"            ,  1  }, // 2 
  { "d"             , "d"            ,  1  }, // 3 
  { "e"             , "e"            ,  0  }, // 4 
  { "A"             , "A"            ,  0  }, // 5 
  { "0"             , "0"            ,  0  }, // 6 
  { "1"             , "1"            ,  0  }, // 7 
  { "2"             , "2"            ,  0  }, // 8 
  { "3"             , "3"            ,  0  }, // 9 
  { "4"             , "4"            ,  0  }, // 10
  { "axyz"          , "Axyz"         ,  1  }, // 11
  { "bxyz"          , "Bxyz"         ,  1  }, // 12
  { "cxyz"          , "Cxyz"         ,  1  }, // 13
  { "dxyz"          , "dxyz"         ,  1  }, // 14
  { "exyz"          , "exyz"         ,  0  }, // 15
  { "Axyz"          , "Axyz"         ,  0  }, // 16
  { "0xyz"          , "0xyz"         ,  0  }, // 17
  { "1xyz"          , "1xyz"         ,  0  }, // 18
  { "2xyz"          , "2xyz"         ,  0  }, // 19
  { "3xyz"          , "3xyz"         ,  0  }, // 20
  { "4xyz"          , "4xyz"         ,  0  }, // 21
  { "lmna"          , "lmnA"         ,  1  }, // 22
  { "lmnb"          , "lmnB"         ,  1  }, // 23
  { "lmnc"          , "lmnC"         ,  1  }, // 24
  { "lmnd"          , "lmnd"         ,  1  }, // 25
  { "lmne"          , "lmne"         ,  0  }, // 26
  { "lmnA"          , "lmnA"         ,  0  }, // 27
  { "lmn0"          , "lmn0"         ,  0  }, // 28
  { "lmn1"          , "lmn1"         ,  0  }, // 29
  { "lmn2"          , "lmn2"         ,  0  }, // 30
  { "lmn3"          , "lmn3"         ,  0  }, // 31
  { "lmn4"          , "lmn4"         ,  0  }, // 32
  { "lmnaxyz"       , "lmnAxyz"      ,  1  }, // 33
  { "lmnbxyz"       , "lmnBxyz"      ,  1  }, // 34
  { "lmncxyz"       , "lmnCxyz"      ,  1  }, // 35
  { "lmndxyz"       , "lmndxyz"      ,  1  }, // 36
  { "lmnexyz"       , "lmnexyz"      ,  0  }, // 37
  { "lmnAxyz"       , "lmnAxyz"      ,  0  }, // 38
  { "lmn0xyz"       , "lmn0xyz"      ,  0  }, // 39
  { "lmn1xyz"       , "lmn1xyz"      ,  0  }, // 40
  { "lmn2xyz"       , "lmn2xyz"      ,  0  }, // 41
  { "lmn3xyz"       , "lmn3xyz"      ,  0  }, // 42
  { "lmn4xyz"       , "lmn4xyz"      ,  0  }, // 43
  { 0               , 0              ,  0  }
};


//////////////////////////////////////////////////////////////////////
// test5, combination pattern
//////////////////////////////////////////////////////////////////////
expr_t expr5a = { "*_(1995..2015)_tmp(0|25|50)C_ch[1-8]_(0|100)V.(csv|dat)"
		  , "$3=_temp,$6=[A-D],$8=(0|-100)" };
testcase_t pat5a[] = {
  { "VItest_1995_tmp0C_ch1_0V.csv"     , "VItest_1995_temp0C_chA_0V.csv"       ,  1  }, // 0 
  { "VItest_1996_tmp25C_ch1_100V.csv"  , "VItest_1996_temp25C_chA_-100V.csv"   ,  1  }, // 1 
  { "VItest_2012_tmp50C_ch8_100V.csv"  , "VItest_2012_temp50C_ch8_-100V.csv"   ,  1  }, // 2 
  { "VItest_2012_tmp20C_ch8_100V.csv"  , "VItest_2012_tmp20C_ch8_100V.csv"     ,  0  }, // 3 
  { "VItest_2012_tmp25C_ch0_100V.csv"  , "VItest_2012_tmp25C_ch0_100V.csv"     ,  0  }, // 4 
  { 0               , 0              ,  0  }
};

//////////////////////////////////////////////////////////////////////
expr_t expr5b = { "*_(1995..2015)_tmp(0|25|50)C_ch[1-8]_(0|100)V.(csv|dat)"
		  , "$2=(1994..2000),$6=(1st|2nd|3rd|4th),$8=(-100|0)" };
testcase_t pat5b[] = {
  { "VItest_1995_tmp0C_ch1_0V.csv"     , "VItest_1994_tmp0C_ch1st_-100V.csv"    ,  1  }, // 0 
  { "VItest_1995_tmp0C_ch3_0V.csv"     , "VItest_1994_tmp0C_ch3rd_-100V.csv"    ,  1  }, // 0 
  { "VItest_1996_tmp25C_ch1_100V.csv"  , "VItest_1995_tmp25C_ch1st_0V.csv"      ,  1  }, // 1 
  { "VItest_1996_tmp25C_ch2_100V.csv"  , "VItest_1995_tmp25C_ch2nd_0V.csv"      ,  1  }, // 1 
  { "VItest_1996_tmp25C_ch4_100V.csv"  , "VItest_1995_tmp25C_ch4th_0V.csv"      ,  1  }, // 1 
  { "VItest_1996_tmp25C_ch5_100V.csv"  , "VItest_1995_tmp25C_ch5_0V.csv"        ,  1  }, // 1 
  { "VItest_2012_tmp50C_ch8_100V.csv"  , "VItest_2012_tmp50C_ch8_0V.csv"        ,  1  }, // 2 
  { "VItest_2012_tmp20C_ch8_100V.csv"  , "VItest_2012_tmp20C_ch8_100V.csv"      ,  0  }, // 3 
  { "VItest_2012_tmp25C_ch0_100V.csv"  , "VItest_2012_tmp25C_ch0_100V.csv"      ,  0  }, // 4 
  { 0               , 0              ,  0  }
};


//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
int DEBUG=0;
int quick=0,intentionalfail=0;
int main(int argc,char** argv) {
  for(int i=1;i<argc;i++) {
    if(strcmp(argv[i],"-g")==0) ++DEBUG;
    else if(strcmp(argv[i],"-q")==0) ++quick;
    else if(strcmp(argv[i],"-f")==0) ++intentionalfail;
    else if(argv[i][0]=='-') {
      printf("compile.cxx : Unknown option %s\n",argv[i]);
      return(0);
    }
  }
  int err=0;
  printf("### EasyExp replace test\n");

  printf("test1a-f\n");
  err+=test("test1a",expr1a,pat1a);
  err+=test("test1b",expr1b,pat1b);
  err+=test("test1c",expr1c,pat1c);
  err+=test("test1d",expr1d,pat1d);
  err+=test("test1e",expr1d,pat1d);
  err+=test("test1f",expr1d,pat1d);

  printf("test2a-h\n");
  err+=test("test2a",expr2a,pat2a);
  err+=test("test2b",expr2b,pat2b);
  err+=test("test2c",expr2c,pat2c);
  err+=test("test2d",expr2d,pat2d);
  err+=test("test2e",expr2e,pat2e);
  err+=test("test2f",expr2f,pat2f);
  err+=test("test2g",expr2g,pat2g);
  err+=test("test2h",expr2h,pat2h);

  printf("test3a-h\n");
  err+=test("test3a",expr3a,pat3a);
  err+=test("test3b",expr3b,pat3b);
  err+=test("test3c",expr3c,pat3c);
  err+=test("test3d",expr3d,pat3d);
  err+=test("test3e",expr3e,pat3e);
  err+=test("test3f",expr3f,pat3f);
  err+=test("test3g",expr3g,pat3g);
  err+=test("test3h",expr3h,pat3h);

  printf("test4a-j\n");
  err+=test("test4a",expr4a,pat4a);
  err+=test("test4b",expr4b,pat4b);
  err+=test("test4c",expr4c,pat4c);
  err+=test("test4d",expr4d,pat4d);
  err+=test("test4e",expr4e,pat4e);
  err+=test("test4f",expr4f,pat4f);
  err+=test("test4g",expr4g,pat4g);
  err+=test("test4h",expr4h,pat4h);
  err+=test("test4i",expr4i,pat4i);
  err+=test("test4j",expr4j,pat4j);

  printf("test5a-b\n");
  err+=test("test5a",expr5a,pat5a);
  err+=test("test5b",expr5b,pat5b);

  SAVE_PASSFAIL("replace");
  exit(err);
}

//////////////////////////////////////////////////////////////////////




