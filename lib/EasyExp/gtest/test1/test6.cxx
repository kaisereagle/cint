// My 1st Google test 

#ifndef __CINT__
#include "../../EasyExp.h"
#include <gtest/gtest.h>
extern void RESET_LOCALFAIL();
extern int LOCALFAIL_COUNT() ;
extern int G__pause() ;
#include <string>
#include <vector>
using namespace std;
#else
#include <_util_cinttest.h>
#endif


// Thorough synthesis test is performed in module test ../../test


///////////////////////////////////////////////////////////
TEST(EasyExpSynthesisTest, StringList)
{
  vector<string> matchin, unmatchin;
  string out;

  matchin.push_back("I like an apple.");
  matchin.push_back("You like bananas.");
  matchin.push_back("We like potatos.");
  EXPECT_EQ(EasyExpSynthesis(matchin,unmatchin,0,100,0)
	    ,"* like *");

  EXPECT_EQ(EasyExpSynthesis(matchin,unmatchin,1,100,0)
	    ,"* like *"); //,"(I|We|You) like (an apple.|bananas.|potatos.)");

  unmatchin.push_back("They like orange.");
  EXPECT_EQ(EasyExpSynthesis(matchin,unmatchin,0,100,0)
	    ,"(I|We|You) like *"); //,"(I|We|You) like (an apple.|bananas.|potatos.)");

  EXPECT_EQ(EasyExpSynthesis(matchin,unmatchin,0,2,0)
	    ,"* like *");
}

///////////////////////////////////////////////////////////
TEST(EasyExpSynthesisTest, positionFlag)
{
  vector<string> matchin, unmatchin;
  string out;

  matchin.push_back("He was born in 1993");
  matchin.push_back("He was born in United States in 1993");
  matchin.push_back("She was born in France in 1993");
  EXPECT_EQ(EasyExpSynthesis(matchin,unmatchin,0,100,0)
	    ,"*e was born in *1993");

  EXPECT_EQ(EasyExpSynthesis(matchin,unmatchin,0,100,20)
	    ,"*e was born in *1993");

  EXPECT_EQ(EasyExpSynthesis(matchin,unmatchin,0,100,15)
	    ,"*e was born in *");

  EXPECT_EQ(EasyExpSynthesis(matchin,unmatchin,0,100,5)
	    ,"*e was born in *");

  unmatchin.push_back("Jane was born in France in 1993");
  EXPECT_EQ(EasyExpSynthesis(matchin,unmatchin,0,100,0)
	    ,"(H|Sh)e was born in *1993");// ,"(H|Sh)e was born in (|France in |United States in )1993");

  EXPECT_EQ(EasyExpSynthesis(matchin,unmatchin,0,100,20)
	    ,"(H|Sh)e was born in *1993");//,"(H|Sh)e was born in (|France in |United States in )1993");

  EXPECT_EQ(EasyExpSynthesis(matchin,unmatchin,0,100,15)
	    ,"(H|Sh)e was born in *"); //,"(H|Sh)e was born in (1993|France in 1993|United States in 1993)");

  EXPECT_EQ(EasyExpSynthesis(matchin,unmatchin,0,100,5)
	    ,"(H|Sh)e was born in *"); //,"(H|Sh)e was born in (1993|France in 1993|United States in 1993)");

}
