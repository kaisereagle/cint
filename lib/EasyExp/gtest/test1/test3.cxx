// My 1st Google test 

#ifndef __CINT__
#include "../../EasyExp.h"
#include <gtest/gtest.h>
extern void RESET_LOCALFAIL();
extern int LOCALFAIL_COUNT() ;
extern int G__pause() ;
#else
#include <_util_cinttest.h>
#endif

// Thorough pattern match test is performed in module test ../../test

///////////////////////////////////////////////////////////
TEST(EasyExpMatchTest, ZeroLengthStringList1)
{
  EasyExp e1("pattern*(.tsv|.csv)");        // must have (.tsv|.csv)
  EXPECT_TRUE(e1.match("patternABC.tsv"));
  EXPECT_FALSE(e1.match("patternABC.tsvx"));
  EXPECT_TRUE(e1.match("pattern.csv"));
  EXPECT_FALSE(e1.match("pattern"));
  EasyExp e2("pattern*(|.tsv|.csv)");       // same as pattern*
  EXPECT_TRUE(e2.match("patternABC.tsv"));
  EXPECT_TRUE(e2.match("patternABC.tsvx"));
  EXPECT_TRUE(e2.match("pattern.csv"));
  EXPECT_TRUE(e2.match("pattern"));
  EasyExp e3("pattern*(.tsv|.csv)(|x)");    // must have (.tsv|.csv|.tsvx|.csvx)
  EXPECT_TRUE(e3.match("patternABC.tsv"));
  EXPECT_TRUE(e3.match("patternABC.tsvx"));
  EXPECT_TRUE(e3.match("pattern.csv"));
  EXPECT_FALSE(e3.match("pattern"));
  EasyExp e4("pattern*(|.tsv|.csv)(|x)");   // same as pattern*
  EXPECT_TRUE(e4.match("patternABC.tsv"));
  EXPECT_TRUE(e4.match("patternABC.tsvx"));
  EXPECT_TRUE(e4.match("pattern.csv"));
  EXPECT_TRUE(e4.match("pattern"));
  EasyExp e5("pattern*(|.tsv|.csv)[x:0-1]");// same as pattern*
  EXPECT_TRUE(e5.match("patternABC.tsv"));
  EXPECT_TRUE(e5.match("patternABC.tsvx"));
  EXPECT_TRUE(e5.match("pattern.csv"));
  EXPECT_TRUE(e5.match("pattern"));
  EasyExp e6("pattern*(.tsv|.csv)*(|x)");   // must have  (.tsv|.csv)
  EXPECT_TRUE(e6.match("patternABC.tsv"));
  EXPECT_TRUE(e6.match("patternABC.tsvx"));
  EXPECT_TRUE(e6.match("pattern.csv"));
  EXPECT_FALSE(e6.match("pattern"));
  EasyExp e7("pattern*(|.tsv|.csv)*(|x)");  // same as pattern*
  EXPECT_TRUE(e7.match("patternABC.tsv"));
  EXPECT_TRUE(e7.match("patternABC.tsvx"));
  EXPECT_TRUE(e7.match("pattern.csv"));
  EXPECT_TRUE(e7.match("pattern"));
}

///////////////////////////////////////////////////////////
TEST(EasyExpMatchTest, LongPattern)
{

  EasyExp e1("pn[A-Z:2-3]_(1985..2015:4)(01..12:2)(01..31:2)_(0000..9999:4).csv");
  EXPECT_TRUE(e1.match("pnAB_20150901_0010.csv"));
  EXPECT_FALSE(e1.match("pnAB_20160901_0010.csv"));
  
  EasyExp e2("fr*_(apple|orange|banana)_(2001..2020)_*.(txt|doc|docx)");
  EXPECT_TRUE(e2.match("fruit_apple_2002_.txt"));
  EXPECT_FALSE(e2.match("fruit_pear_2001_.txt"));

  EasyExp e3("pn[A-Z:2-3]_[0-9:8]_[0-9:4].csv");
  EXPECT_TRUE(e3.match("pnAB_20150901_0010.csv"));
  EXPECT_FALSE(e3.match("pnAB_150901_0010.csv"));

  EasyExp e4("pn[A-Z:2-3]_[0-9:6-10]_[0-9:*].csv");
  EXPECT_TRUE(e4.match("pnAB_20150901_0010.csv"));
  EXPECT_TRUE(e4.match("pnAB_150901_0010.csv"));

  EasyExp e5("pn[A-Z:2-3]_[0-9:*]_[0-9:*].csv");
  EXPECT_TRUE(e5.match("pnAB_20150901_0010.csv"));
  EXPECT_TRUE(e5.match("pnAB_150901_0010.csv"));
}

///////////////////////////////////////////////////////////
TEST(EasyExpMatchTest, StringListStarStringList)
{
  EasyExp e1("(abc|def|hij)");  
  EXPECT_TRUE(e1.match("abc"));
  EXPECT_TRUE(e1.match("def"));
  EXPECT_FALSE(e1.match("xyz"));
  EXPECT_FALSE(e1.match(""));
  
  EasyExp e2("(abc||def|hij)");  
  EXPECT_TRUE(e2.match("abc"));
  EXPECT_TRUE(e2.match("def"));
  EXPECT_FALSE(e2.match("xyz"));
  EXPECT_TRUE(e2.match(""));

  EasyExp e3("*(abc|def|hij)");  
  EXPECT_TRUE(e3.match("ABBabc"));
  EXPECT_TRUE(e3.match("ABBdef"));
  EXPECT_FALSE(e3.match("abcAAB"));
  EXPECT_FALSE(e3.match("AAxyz"));
  EXPECT_FALSE(e3.match(""));

  EasyExp e4("(abc|def|hij)*"); 
  EXPECT_FALSE(e4.match("ABBabc"));
  EXPECT_FALSE(e4.match("ABBdef"));
  EXPECT_TRUE(e4.match("abcAAB"));
  EXPECT_FALSE(e4.match("AAxyz"));
  EXPECT_FALSE(e4.match(""));
}

///////////////////////////////////////////////////////////
TEST(EasyExpMatchTest, CharListStarCharList)
{
  EasyExp e1("[12aeg]");  
  EXPECT_TRUE(e1.match("a"));
  EXPECT_FALSE(e1.match("A"));

  EasyExp e2("[a-z0-9]");  
  EXPECT_TRUE(e2.match("0"));
  EXPECT_FALSE(e2.match("A"));

  EasyExp e3("*[12aeg]");  
  EXPECT_TRUE(e3.match("xxa"));
  EXPECT_FALSE(e3.match("aXx"));
  EXPECT_TRUE(e3.match("aaa"));

  EasyExp e4("*[a-z0-9]");  
  EXPECT_TRUE(e4.match("AA0"));
  EXPECT_FALSE(e4.match("0AA"));
}

//
// Rest of the pattern match tests are too complicated to be handled as 
// unit test.  If you have CINT installed in your computer, 
// Go to $CINTSYSDIR/lib/EasyExp/test directory and run following command.
//   $  cd $CINTSYSDIR/lib/EasyExp/test
//   $  cint testall.cxx -p
//
