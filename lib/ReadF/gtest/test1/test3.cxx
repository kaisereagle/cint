// basename and suffix extraction test

#ifndef __CINT__
#include "../../ReadF.h"
#include <gtest/gtest.h>
extern void RESET_LOCALFAIL();
extern int LOCALFAIL_COUNT() ;
extern int G__pause() ;
#else
#include <_util_cinttest.h>
#endif

// Thorough pattern match test is performed in module test ../../test

///////////////////////////////////////////////////////////
TEST(ExtractTest, BasenameSuffixDirnameFilename)
{
  string in = "c:/topdir\\subdir/abcdef.xyz";
  EXPECT_EQ(extract_basename(in),"c:/topdir\\subdir/abcdef");
  EXPECT_EQ(extract_suffix(in),".xyz");
  EXPECT_EQ(extract_dirname(in),"c:/topdir\\subdir");
  EXPECT_EQ(extract_filename(in),"abcdef.xyz");

  in = "/topdir/subdir/abcdef";
  EXPECT_EQ(extract_basename(in),"/topdir/subdir/abcdef");
  EXPECT_EQ(extract_suffix(in),"");
  EXPECT_EQ(extract_dirname(in),"/topdir/subdir");
  EXPECT_EQ(extract_filename(in),"abcdef");

  in = "/topdir/subdir/.abcdef";
  EXPECT_EQ(extract_basename(in),"/topdir/subdir/");
  EXPECT_EQ(extract_suffix(in),".abcdef");
  EXPECT_EQ(extract_dirname(in),"/topdir/subdir");
  EXPECT_EQ(extract_filename(in),".abcdef");

  in = ".abcdef";
  EXPECT_EQ(extract_basename(in),"");
  EXPECT_EQ(extract_suffix(in),".abcdef");
  EXPECT_EQ(extract_dirname(in),"");
  EXPECT_EQ(extract_filename(in),".abcdef"); 
}

///////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////
