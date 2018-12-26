// My 1st Google test 

#ifndef __CINT__
#include "../../EasyExp.h"
#include <gtest/gtest.h>
#include <ctype.h>
#else
#include <_util_cinttest.h>
#endif

//////////////////////////////////////////////////////////////////////////
TEST(globalFuncTest, StringToupper)
{
  string a="abcdef", b="ABCDEF";
  EXPECT_EQ(toupper(a),b);

  a="Abcdef"; b = "ABCDEF";
  EXPECT_EQ(toupper(a),b);

  a="01234"; b = "01234";
  EXPECT_EQ(toupper(a),b);
}

//////////////////////////////////////////////////////////////////////////
TEST(globalFuncTest, EasyExpAnystring)
{
  EXPECT_EQ(EasyExp_anystring(),"@_ZaR@");
}

//////////////////////////////////////////////////////////////////////////
TEST(globalFuncTest, EasyExpAnychar)
{
  EXPECT_EQ(EasyExp_anychar(),"$");
}

//////////////////////////////////////////////////////////////////////////
TEST(globalFuncTest, EasyExpnumberofsamples)
{
  EXPECT_EQ(EasyExp_numberofsamples(),5);
}

//////////////////////////////////////////////////////////////////////////
TEST(globalFuncTest, EasyExpNotcharlist)
{
  string a="0156aBXZ";
  size_t npos = string::npos;
  EXPECT_TRUE(isprint(EasyExp_notcharlist(a)[0]));
  EXPECT_TRUE(isprint(EasyExp_notcharlist(a)[0]));
  EXPECT_TRUE(isprint(EasyExp_notcharlist(a)[0]));
  EXPECT_TRUE(isprint(EasyExp_notcharlist(a)[0]));
  EXPECT_TRUE(isprint(EasyExp_notcharlist(a)[0]));
  EXPECT_TRUE(isprint(EasyExp_notcharlist(a)[0]));
  EXPECT_TRUE(isprint(EasyExp_notcharlist(a)[0]));
  EXPECT_EQ(EasyExp_notcharlist(a).find(a),npos);
  EXPECT_EQ(EasyExp_notcharlist(a).find(a),npos);
  EXPECT_EQ(EasyExp_notcharlist(a).find(a),npos);
  EXPECT_EQ(EasyExp_notcharlist(a).find(a),npos);
  EXPECT_EQ(EasyExp_notcharlist(a).find(a),npos);
  EXPECT_EQ(EasyExp_notcharlist(a).find(a),npos);
  EXPECT_EQ(EasyExp_notcharlist(a).find(a),npos);
  EXPECT_EQ(EasyExp_notcharlist(a).find(a),npos);
}

//////////////////////////////////////////////////////////////////////////

TEST(globalFuncTest, EasyExpnumberrange)
{
  int low=153, high=500,padding=3;
  int a=atoi(EasyExp_numberrange(low,high,padding).c_str());
  EXPECT_GE(a,low);
  EXPECT_LE(a,high);

  low=153; high=500; padding=4;
  a=atoi(EasyExp_numberrange(low,high,padding).c_str());
  EXPECT_GE(a,low);
  EXPECT_LE(a,high);

  low=0; high=10; padding=2;
  a=atoi(EasyExp_numberrange(low,high,padding).c_str());
  EXPECT_GE(a,low);
  EXPECT_LE(a,high);

  low=19990801; high=20150115; padding=8;
  a=atoi(EasyExp_numberrange(low,high,padding).c_str());
  EXPECT_GE(a,low);
  EXPECT_LE(a,high);
}


//////////////////////////////////////////////////////////////////////////
TEST(globalFuncTest, operatorEQEasyExpstring)
{
  EasyExp ex("abc*xyz");
  EXPECT_TRUE(ex=="abcdefghijklmnopqrstuvwxyz");
  EXPECT_FALSE(ex=="Abcdefghijklmnopqrstuvwxyz");
  EXPECT_TRUE("abcdefghijklmnopqrstuvwxyz"==ex);
  EXPECT_FALSE("Abcdefghijklmnopqrstuvwxyz"==ex);
}

//////////////////////////////////////////////////////////////////////////
TEST(globalFuncTest, operatorNEEasyExpstring)
{
  EasyExp ex("abc*xyz");
  EXPECT_FALSE(ex!="abcdefghijklmnopqrstuvwxyz");
  EXPECT_TRUE(ex!="Abcdefghijklmnopqrstuvwxyz");
  EXPECT_FALSE("abcdefghijklmnopqrstuvwxyz"!=ex);
  EXPECT_TRUE("Abcdefghijklmnopqrstuvwxyz"!=ex);
}

//////////////////////////////////////////////////////////////////////////
TEST(globalFuncTest, operatorEQNEEasyExpEasyExp)
{
  EasyExp ex1("abc*xyz"),ex2;
  EXPECT_TRUE(ex1==ex2);
  EXPECT_FALSE(ex1!=ex2);
  EXPECT_FALSE(ex1<ex2);
}
