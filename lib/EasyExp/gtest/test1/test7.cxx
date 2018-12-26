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


// Thorough pattern match test is performed in module test ../../test


///////////////////////////////////////////////////////////
TEST(StatFloatTest, Few)
{
  int i,j=0;

  Stat<float>  stat1,stat2,stat3;
  vector<float> d3;
  float x[] = {  1.0, 2.0, 3.0 ,0};
  int   n[] = {    2,   4,  2,  0};

  while(n[j]) {
    stat1.addN(x[j],n[j]);
    for(i=0;i<n[j];i++) {
      stat2.add(x[j]);
      d3.push_back(x[j]);
    }
    ++j;
  }
  stat3.add(d3);

  EXPECT_NEAR(stat1.mean(),2.0,0.001);
  EXPECT_NEAR(stat2.mean(),2.0,0.001);
  EXPECT_NEAR(stat3.mean(),2.0,0.001);

  EXPECT_NEAR(stat1.min(),1.0,0.001);
  EXPECT_NEAR(stat2.min(),1.0,0.001);
  EXPECT_NEAR(stat3.min(),1.0,0.001);

  EXPECT_NEAR(stat1.max(),3.0,0.001);
  EXPECT_NEAR(stat2.max(),3.0,0.001);
  EXPECT_NEAR(stat3.max(),3.0,0.001);

  EXPECT_NEAR(stat1.sigma(),16.0,0.001);
  EXPECT_NEAR(stat2.sigma(),16.0,0.001);
  EXPECT_NEAR(stat3.sigma(),16.0,0.001);

  EXPECT_NEAR(stat1.sigma2(),36.0,0.001);
  EXPECT_NEAR(stat2.sigma2(),36.0,0.001);
  EXPECT_NEAR(stat3.sigma2(),36.0,0.001);

  EXPECT_NEAR(stat1.stddev(),0.755929,0.001);
  EXPECT_NEAR(stat2.stddev(),0.755929,0.001);
  EXPECT_NEAR(stat3.stddev(),0.755929,0.001);

  EXPECT_EQ(stat1.nsample(),8);
  EXPECT_EQ(stat2.nsample(),8);
  EXPECT_EQ(stat3.nsample(),8);

}

///////////////////////////////////////////////////////////
TEST(StatFloatTest, Thousand)
{
  int i,j=0;

  Stat<float>  stat1,stat2,stat3;
  vector<float> d3;
  float x[] = {  1.0, 2.0, 3.0 ,0};
  int   n[] = { 2000, 4000,  2000,  0};

  while(n[j]) {
    stat1.addN(x[j],n[j]);
    for(i=0;i<n[j];i++) {
      stat2.add(x[j]);
      d3.push_back(x[j]);
    }
    ++j;
  }
  stat3.add(d3);

  EXPECT_NEAR(stat1.mean(),2.0,0.001);
  EXPECT_NEAR(stat2.mean(),2.0,0.001);
  EXPECT_NEAR(stat3.mean(),2.0,0.001);

  EXPECT_NEAR(stat1.min(),1.0,0.001);
  EXPECT_NEAR(stat2.min(),1.0,0.001);
  EXPECT_NEAR(stat3.min(),1.0,0.001);

  EXPECT_NEAR(stat1.max(),3.0,0.001);
  EXPECT_NEAR(stat2.max(),3.0,0.001);
  EXPECT_NEAR(stat3.max(),3.0,0.001);

  EXPECT_NEAR(stat1.sigma(),16000.0,0.001);
  EXPECT_NEAR(stat2.sigma(),16000.0,0.001);
  EXPECT_NEAR(stat3.sigma(),16000.0,0.001);

  EXPECT_NEAR(stat1.sigma2(),36000.0,0.001);
  EXPECT_NEAR(stat2.sigma2(),36000.0,0.001);
  EXPECT_NEAR(stat3.sigma2(),36000.0,0.001);

  EXPECT_NEAR(stat1.stddev(),0.707151,0.001);
  EXPECT_NEAR(stat2.stddev(),0.707151,0.001);
  EXPECT_NEAR(stat3.stddev(),0.707151,0.001);

  EXPECT_EQ(stat1.nsample(),8000);
  EXPECT_EQ(stat2.nsample(),8000);
  EXPECT_EQ(stat3.nsample(),8000);

}

///////////////////////////////////////////////////////////
TEST(StatFloatTest, Million1)
{
  int i,j=0;

  Stat<float>  stat1,stat2,stat3;
  vector<float> d3;
  float x[] = {  1.0, 2.0, 3.0 ,0};
  int   n[] = { 2000000, 4000000,  2000000,  0};

  while(n[j]) {
    stat2.addN(x[j],n[j]);
    for(i=0;i<n[j];i++) {
      stat1.add(x[j]);
      d3.push_back(x[j]);
    }
    ++j;
  }
  stat3.add(d3);

  EXPECT_NEAR(stat1.mean(),2.0,0.001);
  EXPECT_NEAR(stat2.mean(),2.0,0.001);
  EXPECT_NEAR(stat3.mean(),2.0,0.001);

  EXPECT_NEAR(stat1.min(),1.0,0.001);
  EXPECT_NEAR(stat2.min(),1.0,0.001);
  EXPECT_NEAR(stat3.min(),1.0,0.001);

  EXPECT_NEAR(stat1.max(),3.0,0.001);
  EXPECT_NEAR(stat2.max(),3.0,0.001);
  EXPECT_NEAR(stat3.max(),3.0,0.001);

  EXPECT_NEAR(stat1.sigma(),16000000.0,0.001);
  EXPECT_NEAR(stat2.sigma(),16000000.0,0.001);
  EXPECT_NEAR(stat3.sigma(),16000000.0,0.001);

  EXPECT_NEAR(stat1.sigma2(),36000000.0,0.001); 
  EXPECT_NEAR(stat2.sigma2(),36000000.0,0.001);
  EXPECT_NEAR(stat3.sigma2(),36000000.0,0.001); 

  EXPECT_NEAR(stat1.stddev(),0.707106825,0.001);
  EXPECT_NEAR(stat2.stddev(),0.707106825,0.001); 
  EXPECT_NEAR(stat3.stddev(),0.707106825,0.001);

  EXPECT_EQ(stat1.nsample(),8000000);
  EXPECT_EQ(stat2.nsample(),8000000);
  EXPECT_EQ(stat3.nsample(),8000000);

}

///////////////////////////////////////////////////////////
TEST(StatFloatTest, Million2)
{
  int i,j=0;

  Stat<float>  stat1,stat2,stat3;
  vector<float> d3;
  float x[] = {  1000.0, 2000.0, 3000.0 ,0};
  int   n[] = { 2000000, 4000000,  2000000,  0};

  while(n[j]) {
    stat2.addN(x[j],n[j]);
    for(i=0;i<n[j];i++) {
      stat1.add(x[j]);
      d3.push_back(x[j]);
    }
    ++j;
  }
  stat3.add(d3);

  EXPECT_NEAR(stat1.mean(),2000.0,0.001); 
  EXPECT_NEAR(stat2.mean(),2000.0,0.001);
  EXPECT_NEAR(stat3.mean(),2000.0,0.001); 

  EXPECT_NEAR(stat1.min(),1000.0,0.001);
  EXPECT_NEAR(stat2.min(),1000.0,0.001);
  EXPECT_NEAR(stat3.min(),1000.0,0.001);

  EXPECT_NEAR(stat1.max(),3000.0,0.001);
  EXPECT_NEAR(stat2.max(),3000.0,0.001);
  EXPECT_NEAR(stat3.max(),3000.0,0.001);

  EXPECT_NEAR(stat1.sigma(),16000000000.0,0.1);
  EXPECT_NEAR(stat2.sigma(),16000000000.0,0.1);
  EXPECT_NEAR(stat3.sigma(),16000000000.0,0.1);

  EXPECT_NEAR(stat1.sigma2(),36000000000000.0,1000000);
  EXPECT_NEAR(stat2.sigma2(),36000000000000.0,1000000);
  EXPECT_NEAR(stat3.sigma2(),36000000000000.0,1000000);

  EXPECT_NEAR(stat1.stddev(),707.106825381,0.001);
  EXPECT_NEAR(stat2.stddev(),707.106825381,0.001); 
  EXPECT_NEAR(stat3.stddev(),707.106825381,0.001);

  EXPECT_EQ(stat1.nsample(),8000000);
  EXPECT_EQ(stat2.nsample(),8000000);
  EXPECT_EQ(stat3.nsample(),8000000);

}


///////////////////////////////////////////////////////////
TEST(StatFloatTest, Trillion1)
{
  int i,j=0;

  Stat<float>  stat1,stat2;
  float x[] = {  1000.0, 2000.0, 3000.0 ,0};
  int   n[] = { 400000000, 800000000,  400000000,  0};

  while(n[j]) {
    printf("j=%d\n",j);
    stat2.addN(x[j],n[j]);
    for(i=0;i<n[j];i++) {
      stat1.add(x[j]);
    }
    ++j;
  }

  EXPECT_NEAR(stat1.mean(),2000.0,0.001);  
  EXPECT_NEAR(stat2.mean(),2000.0,0.001);  

  EXPECT_NEAR(stat1.min(),1000.0,0.001);
  EXPECT_NEAR(stat2.min(),1000.0,0.001);

  EXPECT_NEAR(stat1.max(),3000.0,0.001);
  EXPECT_NEAR(stat2.max(),3000.0,0.001);

  EXPECT_NEAR(stat1.sigma(),3200000000000.0,10);
  EXPECT_NEAR(stat2.sigma(),3200000000000.0,100000);

  EXPECT_NEAR(stat1.sigma2(),7200000000000000.0,1000000.0);
  EXPECT_NEAR(stat2.sigma2(),7200000000000000.0,100000000.0);

  EXPECT_NEAR(stat1.stddev(),707.106825381,0.001);
  EXPECT_NEAR(stat2.stddev(),707.106825381,0.01);

  EXPECT_EQ(stat1.nsample(),1600000000);
  EXPECT_EQ(stat2.nsample(),1600000000);

}


///////////////////////////////////////////////////////////
TEST(StatDoubleTest, Few)
{
  int i,j=0;

  Stat<double>  stat1,stat2,stat3;
  vector<double> d3;
  double x[] = {  1.0, 2.0, 3.0 ,0};
  int    n[] = {    2,   4,  2  ,0};

  while(n[j]) {
    stat1.addN(x[j],n[j]);
    for(i=0;i<n[j];i++) {
      stat2.add(x[j]);
      d3.push_back(x[j]);
    }
    ++j;
  }
  stat3.add(d3);

  EXPECT_NEAR(stat1.mean(),2.0,0.001);
  EXPECT_NEAR(stat2.mean(),2.0,0.001);
  EXPECT_NEAR(stat3.mean(),2.0,0.001);

  EXPECT_NEAR(stat1.min(),1.0,0.001);
  EXPECT_NEAR(stat2.min(),1.0,0.001);
  EXPECT_NEAR(stat3.min(),1.0,0.001);

  EXPECT_NEAR(stat1.max(),3.0,0.001);
  EXPECT_NEAR(stat2.max(),3.0,0.001);
  EXPECT_NEAR(stat3.max(),3.0,0.001);

  EXPECT_NEAR(stat1.sigma(),16.0,0.001);
  EXPECT_NEAR(stat2.sigma(),16.0,0.001);
  EXPECT_NEAR(stat3.sigma(),16.0,0.001);

  EXPECT_NEAR(stat1.sigma2(),36.0,0.001);
  EXPECT_NEAR(stat2.sigma2(),36.0,0.001);
  EXPECT_NEAR(stat3.sigma2(),36.0,0.001);

  EXPECT_NEAR(stat1.stddev(),0.755929,0.001);
  EXPECT_NEAR(stat2.stddev(),0.755929,0.001);
  EXPECT_NEAR(stat3.stddev(),0.755929,0.001);

  EXPECT_EQ(stat1.nsample(),8);
  EXPECT_EQ(stat2.nsample(),8);
  EXPECT_EQ(stat3.nsample(),8);

}

///////////////////////////////////////////////////////////
TEST(StatDoubleTest, Thousand)
{
  int i,j=0;

  Stat<double>  stat1,stat2,stat3;
  vector<double> d3;
  double x[] = {  1.0, 2.0, 3.0 ,0};
  int    n[] = { 2000, 4000,  2000,  0};

  while(n[j]) {
    stat1.addN(x[j],n[j]);
    for(i=0;i<n[j];i++) {
      stat2.add(x[j]);
      d3.push_back(x[j]);
    }
    ++j;
  }
  stat3.add(d3);

  EXPECT_NEAR(stat1.mean(),2.0,0.001);
  EXPECT_NEAR(stat2.mean(),2.0,0.001);
  EXPECT_NEAR(stat3.mean(),2.0,0.001);

  EXPECT_NEAR(stat1.min(),1.0,0.001);
  EXPECT_NEAR(stat2.min(),1.0,0.001);
  EXPECT_NEAR(stat3.min(),1.0,0.001);

  EXPECT_NEAR(stat1.max(),3.0,0.001);
  EXPECT_NEAR(stat2.max(),3.0,0.001);
  EXPECT_NEAR(stat3.max(),3.0,0.001);

  EXPECT_NEAR(stat1.sigma(),16000.0,0.001);
  EXPECT_NEAR(stat2.sigma(),16000.0,0.001);
  EXPECT_NEAR(stat3.sigma(),16000.0,0.001);

  EXPECT_NEAR(stat1.sigma2(),36000.0,0.001);
  EXPECT_NEAR(stat2.sigma2(),36000.0,0.001);
  EXPECT_NEAR(stat3.sigma2(),36000.0,0.001);

  EXPECT_NEAR(stat1.stddev(),0.707151,0.001);
  EXPECT_NEAR(stat2.stddev(),0.707151,0.001);
  EXPECT_NEAR(stat3.stddev(),0.707151,0.001);

  EXPECT_EQ(stat1.nsample(),8000);
  EXPECT_EQ(stat2.nsample(),8000);
  EXPECT_EQ(stat3.nsample(),8000);

}

///////////////////////////////////////////////////////////
TEST(StatDoubleTest, Million1)
{
  int i,j=0;

  Stat<double>  stat1,stat2,stat3;
  vector<double> d3;
  double x[] = {  1.0, 2.0, 3.0 ,0};
  int    n[] = { 2000000, 4000000,  2000000,  0};

  while(n[j]) {
    stat2.addN(x[j],n[j]);
    for(i=0;i<n[j];i++) {
      stat1.add(x[j]);
      d3.push_back(x[j]);
    }
    ++j;
  }
  stat3.add(d3);

  EXPECT_NEAR(stat1.mean(),2.0,0.001);
  EXPECT_NEAR(stat2.mean(),2.0,0.001);
  EXPECT_NEAR(stat3.mean(),2.0,0.001);

  EXPECT_NEAR(stat1.min(),1.0,0.001);
  EXPECT_NEAR(stat2.min(),1.0,0.001);
  EXPECT_NEAR(stat3.min(),1.0,0.001);

  EXPECT_NEAR(stat1.max(),3.0,0.001);
  EXPECT_NEAR(stat2.max(),3.0,0.001);
  EXPECT_NEAR(stat3.max(),3.0,0.001);

  EXPECT_NEAR(stat1.sigma(),16000000.0,0.1);
  EXPECT_NEAR(stat2.sigma(),16000000.0,0.1);
  EXPECT_NEAR(stat3.sigma(),16000000.0,0.1);

  EXPECT_NEAR(stat1.sigma2(),36000000.0,0.1);
  EXPECT_NEAR(stat2.sigma2(),36000000.0,0.1); 
  EXPECT_NEAR(stat3.sigma2(),36000000.0,0.1);

  EXPECT_NEAR(stat1.stddev(),0.707106825,0.001);
  EXPECT_NEAR(stat2.stddev(),0.707106825,0.001); 
  EXPECT_NEAR(stat3.stddev(),0.707106825,0.001);

  EXPECT_EQ(stat1.nsample(),8000000);
  EXPECT_EQ(stat2.nsample(),8000000);
  EXPECT_EQ(stat3.nsample(),8000000);

}

///////////////////////////////////////////////////////////
TEST(StatDoubleTest, Million2)
{
  int i,j=0;

  Stat<double>  stat1,stat2,stat3;
  vector<double> d3;
  double x[] = {  1000.0, 2000.0, 3000.0 ,0};
  int    n[] = { 2000000, 4000000,  2000000,  0};

  while(n[j]) {
    stat2.addN(x[j],n[j]);
    for(i=0;i<n[j];i++) {
      stat1.add(x[j]);
      d3.push_back(x[j]);
    }
    ++j;
  }
  stat3.add(d3);

  EXPECT_NEAR(stat1.mean(),2000.0,0.001);
  EXPECT_NEAR(stat2.mean(),2000.0,0.001);
  EXPECT_NEAR(stat3.mean(),2000.0,0.001);

  EXPECT_NEAR(stat1.min(),1000.0,0.001);
  EXPECT_NEAR(stat2.min(),1000.0,0.001);
  EXPECT_NEAR(stat3.min(),1000.0,0.001);

  EXPECT_NEAR(stat1.max(),3000.0,0.001);
  EXPECT_NEAR(stat2.max(),3000.0,0.001);
  EXPECT_NEAR(stat3.max(),3000.0,0.001);

  EXPECT_NEAR(stat1.sigma(),16000000000.0,0.1);
  EXPECT_NEAR(stat2.sigma(),16000000000.0,0.1);
  EXPECT_NEAR(stat3.sigma(),16000000000.0,0.1);

  EXPECT_NEAR(stat1.sigma2(),36000000000000.0,100);
  EXPECT_NEAR(stat2.sigma2(),36000000000000.0,0.1);
  EXPECT_NEAR(stat3.sigma2(),36000000000000.0,100);

  EXPECT_NEAR(stat1.stddev(),707.106825381,0.001);
  EXPECT_NEAR(stat2.stddev(),707.106825381,0.001); 
  EXPECT_NEAR(stat3.stddev(),707.106825381,0.001);

  EXPECT_EQ(stat1.nsample(),8000000);
  EXPECT_EQ(stat2.nsample(),8000000);
  EXPECT_EQ(stat3.nsample(),8000000);

}

///////////////////////////////////////////////////////////
TEST(StatDoubleTest, Trillion1)
{
  int i,j=0;

  Stat<double>  stat1,stat2;
  double x[] = {  1000.0, 2000.0, 3000.0 ,0};
  int    n[] = { 400000000, 800000000,  400000000,  0};

  while(n[j]) {
    printf("j=%d\n",j);
    stat2.addN(x[j],n[j]);
    for(i=0;i<n[j];i++) {
      stat1.add(x[j]);
    }
    ++j;
  }

  EXPECT_NEAR(stat1.mean(),2000.0,0.001); // 
  EXPECT_NEAR(stat2.mean(),2000.0,0.001); // 

  EXPECT_NEAR(stat1.min(),1000.0,0.001);
  EXPECT_NEAR(stat2.min(),1000.0,0.001);

  EXPECT_NEAR(stat1.max(),3000.0,0.001);
  EXPECT_NEAR(stat2.max(),3000.0,0.001);

  EXPECT_NEAR(stat1.sigma(),3200000000000.0,10);
  EXPECT_NEAR(stat2.sigma(),3200000000000.0,100000);

  EXPECT_NEAR(stat1.sigma2(),7200000000000000.0,1000000.0);
  EXPECT_NEAR(stat2.sigma2(),7200000000000000.0,100000000.0);

  EXPECT_NEAR(stat1.stddev(),707.106825381,0.001);
  EXPECT_NEAR(stat2.stddev(),707.106825381,0.01);

  EXPECT_EQ(stat1.nsample(),1600000000);
  EXPECT_EQ(stat2.nsample(),1600000000);

}

///////////////////////////////////////////////////////////
TEST(StatIntTest, Few)
{
  int i,j=0;

  Stat<int>  stat1,stat2,stat3;
  vector<int> d3;
  int x[] = {  1.0, 2.0, 3.0 ,0};
  int n[] = {    2,   4,  2  ,0};

  while(n[j]) {
    stat1.addN(x[j],n[j]);
    for(i=0;i<n[j];i++) {
      stat2.add(x[j]);
      d3.push_back(x[j]);
    }
    ++j;
  }
  stat3.add(d3);

  EXPECT_NEAR(stat1.mean(),2.0,0.001);
  EXPECT_NEAR(stat2.mean(),2.0,0.001);
  EXPECT_NEAR(stat3.mean(),2.0,0.001);

  EXPECT_NEAR(stat1.min(),1.0,0.001);
  EXPECT_NEAR(stat2.min(),1.0,0.001);
  EXPECT_NEAR(stat3.min(),1.0,0.001);

  EXPECT_NEAR(stat1.max(),3.0,0.001);
  EXPECT_NEAR(stat2.max(),3.0,0.001);
  EXPECT_NEAR(stat3.max(),3.0,0.001);

  EXPECT_NEAR(stat1.sigma(),16.0,0.001);
  EXPECT_NEAR(stat2.sigma(),16.0,0.001);
  EXPECT_NEAR(stat3.sigma(),16.0,0.001);

  EXPECT_NEAR(stat1.sigma2(),36.0,0.001);
  EXPECT_NEAR(stat2.sigma2(),36.0,0.001);
  EXPECT_NEAR(stat3.sigma2(),36.0,0.001);

  EXPECT_NEAR(stat1.stddev(),0.755929,0.001);
  EXPECT_NEAR(stat2.stddev(),0.755929,0.001);
  EXPECT_NEAR(stat3.stddev(),0.755929,0.001);

  EXPECT_EQ(stat1.nsample(),8);
  EXPECT_EQ(stat2.nsample(),8);
  EXPECT_EQ(stat3.nsample(),8);

}

///////////////////////////////////////////////////////////
TEST(StatIntTest, Thousand)
{
  int i,j=0;

  Stat<int>  stat1,stat2,stat3;
  vector<int> d3;
  int x[] = {  1.0, 2.0, 3.0 ,0};
  int n[] = { 2000, 4000,  2000,  0};

  while(n[j]) {
    stat1.addN(x[j],n[j]);
    for(i=0;i<n[j];i++) {
      stat2.add(x[j]);
      d3.push_back(x[j]);
    }
    ++j;
  }
  stat3.add(d3);

  EXPECT_NEAR(stat1.mean(),2.0,0.001);
  EXPECT_NEAR(stat2.mean(),2.0,0.001);
  EXPECT_NEAR(stat3.mean(),2.0,0.001);

  EXPECT_NEAR(stat1.min(),1.0,0.001);
  EXPECT_NEAR(stat2.min(),1.0,0.001);
  EXPECT_NEAR(stat3.min(),1.0,0.001);

  EXPECT_NEAR(stat1.max(),3.0,0.001);
  EXPECT_NEAR(stat2.max(),3.0,0.001);
  EXPECT_NEAR(stat3.max(),3.0,0.001);

  EXPECT_NEAR(stat1.sigma(),16000.0,0.001);
  EXPECT_NEAR(stat2.sigma(),16000.0,0.001);
  EXPECT_NEAR(stat3.sigma(),16000.0,0.001);

  EXPECT_NEAR(stat1.sigma2(),36000.0,0.001);
  EXPECT_NEAR(stat2.sigma2(),36000.0,0.001);
  EXPECT_NEAR(stat3.sigma2(),36000.0,0.001);

  EXPECT_NEAR(stat1.stddev(),0.707151,0.001);
  EXPECT_NEAR(stat2.stddev(),0.707151,0.001);
  EXPECT_NEAR(stat3.stddev(),0.707151,0.001);

  EXPECT_EQ(stat1.nsample(),8000);
  EXPECT_EQ(stat2.nsample(),8000);
  EXPECT_EQ(stat3.nsample(),8000);

}

///////////////////////////////////////////////////////////
TEST(StatIntTest, Million1)
{
  int i,j=0;

  Stat<int>  stat1,stat2,stat3;
  vector<int> d3;
  int x[] = {  1.0, 2.0, 3.0 ,0};
  int n[] = { 2000000, 4000000,  2000000,  0};

  while(n[j]) {
    stat2.addN(x[j],n[j]);
    for(i=0;i<n[j];i++) {
      stat1.add(x[j]);
      d3.push_back(x[j]);
    }
    ++j;
  }
  stat3.add(d3);

  EXPECT_NEAR(stat1.mean(),2.0,0.001);
  EXPECT_NEAR(stat2.mean(),2.0,0.001);
  EXPECT_NEAR(stat3.mean(),2.0,0.001);

  EXPECT_NEAR(stat1.min(),1.0,0.001);
  EXPECT_NEAR(stat2.min(),1.0,0.001);
  EXPECT_NEAR(stat3.min(),1.0,0.001);

  EXPECT_NEAR(stat1.max(),3.0,0.001);
  EXPECT_NEAR(stat2.max(),3.0,0.001);
  EXPECT_NEAR(stat3.max(),3.0,0.001);

  EXPECT_NEAR(stat1.sigma(),16000000.0,0.1);
  EXPECT_NEAR(stat2.sigma(),16000000.0,0.1);
  EXPECT_NEAR(stat3.sigma(),16000000.0,0.1);

  EXPECT_NEAR(stat1.sigma2(),36000000.0,0.1);
  EXPECT_NEAR(stat2.sigma2(),36000000.0,0.1); 
  EXPECT_NEAR(stat3.sigma2(),36000000.0,0.1);

  EXPECT_NEAR(stat1.stddev(),0.707106825,0.001);
  EXPECT_NEAR(stat2.stddev(),0.707106825,0.001); 
  EXPECT_NEAR(stat3.stddev(),0.707106825,0.001);

  EXPECT_EQ(stat1.nsample(),8000000);
  EXPECT_EQ(stat2.nsample(),8000000);
  EXPECT_EQ(stat3.nsample(),8000000);

}

#if 0
///////////////////////////////////////////////////////////
TEST(StatIntTest, Million2)
{
  int i,j=0;

  Stat<int>  stat1,stat2,stat3;
  vector<int> d3;
  int x[] = {  1000.0, 2000.0, 3000.0 ,0};
  int n[] = { 2000000, 4000000,  2000000,  0};

  while(n[j]) {
    stat2.addN(x[j],n[j]);
    for(i=0;i<n[j];i++) {
      stat1.add(x[j]);
      d3.push_back(x[j]);
    }
    ++j;
  }
  stat3.add(d3);

  EXPECT_NEAR(stat1.mean(),2000.0,0.001);
  EXPECT_NEAR(stat2.mean(),2000.0,0.001);//
  EXPECT_NEAR(stat3.mean(),2000.0,0.001);

  EXPECT_NEAR(stat1.min(),1000.0,0.001);
  EXPECT_NEAR(stat2.min(),1000.0,0.001);
  EXPECT_NEAR(stat3.min(),1000.0,0.001);

  EXPECT_NEAR(stat1.max(),3000.0,0.001);
  EXPECT_NEAR(stat2.max(),3000.0,0.001);
  EXPECT_NEAR(stat3.max(),3000.0,0.001);

  EXPECT_NEAR(stat1.sigma(),16000000000.0,0.1);
  EXPECT_NEAR(stat2.sigma(),16000000000.0,0.1);//
  EXPECT_NEAR(stat3.sigma(),16000000000.0,0.1);

  EXPECT_NEAR(stat1.sigma2(),36000000000000.0,0.1);
  EXPECT_NEAR(stat2.sigma2(),36000000000000.0,0.1);//
  EXPECT_NEAR(stat3.sigma2(),36000000000000.0,0.1);

  EXPECT_NEAR(stat1.stddev(),707.106825381,0.001);
  EXPECT_NEAR(stat2.stddev(),707.106825381,0.001);//
  EXPECT_NEAR(stat3.stddev(),707.106825381,0.001);

  EXPECT_EQ(stat1.nsample(),8000000);
  EXPECT_EQ(stat2.nsample(),8000000);
  EXPECT_EQ(stat3.nsample(),8000000);

}
#endif

///////////////////////////////////////////////////////////
TEST(StatLongTest, Few)
{
  int i,j=0;

  Stat<long>  stat1,stat2,stat3;
  vector<long> d3;
  long x[] = {  1.0, 2.0, 3.0 ,0};
  int n[] = {    2,   4,  2  ,0};

  while(n[j]) {
    stat1.addN(x[j],n[j]);
    for(i=0;i<n[j];i++) {
      stat2.add(x[j]);
      d3.push_back(x[j]);
    }
    ++j;
  }
  stat3.add(d3);

  EXPECT_NEAR(stat1.mean(),2.0,0.001);
  EXPECT_NEAR(stat2.mean(),2.0,0.001);
  EXPECT_NEAR(stat3.mean(),2.0,0.001);

  EXPECT_NEAR(stat1.min(),1.0,0.001);
  EXPECT_NEAR(stat2.min(),1.0,0.001);
  EXPECT_NEAR(stat3.min(),1.0,0.001);

  EXPECT_NEAR(stat1.max(),3.0,0.001);
  EXPECT_NEAR(stat2.max(),3.0,0.001);
  EXPECT_NEAR(stat3.max(),3.0,0.001);

  EXPECT_NEAR(stat1.sigma(),16.0,0.001);
  EXPECT_NEAR(stat2.sigma(),16.0,0.001);
  EXPECT_NEAR(stat3.sigma(),16.0,0.001);

  EXPECT_NEAR(stat1.sigma2(),36.0,0.001);
  EXPECT_NEAR(stat2.sigma2(),36.0,0.001);
  EXPECT_NEAR(stat3.sigma2(),36.0,0.001);

  EXPECT_NEAR(stat1.stddev(),0.755929,0.001);
  EXPECT_NEAR(stat2.stddev(),0.755929,0.001);
  EXPECT_NEAR(stat3.stddev(),0.755929,0.001);

  EXPECT_EQ(stat1.nsample(),8);
  EXPECT_EQ(stat2.nsample(),8);
  EXPECT_EQ(stat3.nsample(),8);

}

///////////////////////////////////////////////////////////
TEST(StatLongTest, Thousand)
{
  int i,j=0;

  Stat<long>  stat1,stat2,stat3;
  vector<long> d3;
  long x[] = {  1.0, 2.0, 3.0 ,0};
  int n[] = { 2000, 4000,  2000,  0};

  while(n[j]) {
    stat1.addN(x[j],n[j]);
    for(i=0;i<n[j];i++) {
      stat2.add(x[j]);
      d3.push_back(x[j]);
    }
    ++j;
  }
  stat3.add(d3);

  EXPECT_NEAR(stat1.mean(),2.0,0.001);
  EXPECT_NEAR(stat2.mean(),2.0,0.001);
  EXPECT_NEAR(stat3.mean(),2.0,0.001);

  EXPECT_NEAR(stat1.min(),1.0,0.001);
  EXPECT_NEAR(stat2.min(),1.0,0.001);
  EXPECT_NEAR(stat3.min(),1.0,0.001);

  EXPECT_NEAR(stat1.max(),3.0,0.001);
  EXPECT_NEAR(stat2.max(),3.0,0.001);
  EXPECT_NEAR(stat3.max(),3.0,0.001);

  EXPECT_NEAR(stat1.sigma(),16000.0,0.001);
  EXPECT_NEAR(stat2.sigma(),16000.0,0.001);
  EXPECT_NEAR(stat3.sigma(),16000.0,0.001);

  EXPECT_NEAR(stat1.sigma2(),36000.0,0.001);
  EXPECT_NEAR(stat2.sigma2(),36000.0,0.001);
  EXPECT_NEAR(stat3.sigma2(),36000.0,0.001);

  EXPECT_NEAR(stat1.stddev(),0.707151,0.001);
  EXPECT_NEAR(stat2.stddev(),0.707151,0.001);
  EXPECT_NEAR(stat3.stddev(),0.707151,0.001);

  EXPECT_EQ(stat1.nsample(),8000);
  EXPECT_EQ(stat2.nsample(),8000);
  EXPECT_EQ(stat3.nsample(),8000);

}

///////////////////////////////////////////////////////////
TEST(StatLongTest, Million1)
{
  int i,j=0;

  Stat<long>  stat1,stat2,stat3;
  vector<long> d3;
  long x[] = {  1.0, 2.0, 3.0 ,0};
  int n[] = { 2000000, 4000000,  2000000,  0};

  while(n[j]) {
    stat2.addN(x[j],n[j]);
    for(i=0;i<n[j];i++) {
      stat1.add(x[j]);
      d3.push_back(x[j]);
    }
    ++j;
  }
  stat3.add(d3);

  EXPECT_NEAR(stat1.mean(),2.0,0.001);
  EXPECT_NEAR(stat2.mean(),2.0,0.001);
  EXPECT_NEAR(stat3.mean(),2.0,0.001);

  EXPECT_NEAR(stat1.min(),1.0,0.001);
  EXPECT_NEAR(stat2.min(),1.0,0.001);
  EXPECT_NEAR(stat3.min(),1.0,0.001);

  EXPECT_NEAR(stat1.max(),3.0,0.001);
  EXPECT_NEAR(stat2.max(),3.0,0.001);
  EXPECT_NEAR(stat3.max(),3.0,0.001);

  EXPECT_NEAR(stat1.sigma(),16000000.0,0.1);
  EXPECT_NEAR(stat2.sigma(),16000000.0,0.1);
  EXPECT_NEAR(stat3.sigma(),16000000.0,0.1);

  EXPECT_NEAR(stat1.sigma2(),36000000.0,0.1);
  EXPECT_NEAR(stat2.sigma2(),36000000.0,0.1); 
  EXPECT_NEAR(stat3.sigma2(),36000000.0,0.1);

  EXPECT_NEAR(stat1.stddev(),0.707106825,0.001);
  EXPECT_NEAR(stat2.stddev(),0.707106825,0.001); 
  EXPECT_NEAR(stat3.stddev(),0.707106825,0.001);

  EXPECT_EQ(stat1.nsample(),8000000);
  EXPECT_EQ(stat2.nsample(),8000000);
  EXPECT_EQ(stat3.nsample(),8000000);

}

#if 0
///////////////////////////////////////////////////////////
TEST(StatLongTest, Million2)
{
  int i,j=0;

  Stat<long>  stat1,stat2,stat3;
  vector<long> d3;
  long x[] = {  1000.0, 2000.0, 3000.0 ,0};
  int n[] = { 2000000, 4000000,  2000000,  0};

  while(n[j]) {
    stat2.addN(x[j],n[j]);
    for(i=0;i<n[j];i++) {
      stat1.add(x[j]);
      d3.push_back(x[j]);
    }
    ++j;
  }
  stat3.add(d3);

  EXPECT_NEAR(stat1.mean(),2000.0,0.001);
  EXPECT_NEAR(stat2.mean(),2000.0,0.001);
  EXPECT_NEAR(stat3.mean(),2000.0,0.001);

  EXPECT_NEAR(stat1.min(),1000.0,0.001);
  EXPECT_NEAR(stat2.min(),1000.0,0.001);
  EXPECT_NEAR(stat3.min(),1000.0,0.001);

  EXPECT_NEAR(stat1.max(),3000.0,0.001);
  EXPECT_NEAR(stat2.max(),3000.0,0.001);
  EXPECT_NEAR(stat3.max(),3000.0,0.001);

  EXPECT_NEAR(stat1.sigma(),16000000000.0,0.1);
  EXPECT_NEAR(stat2.sigma(),16000000000.0,0.1);
  EXPECT_NEAR(stat3.sigma(),16000000000.0,0.1);

  EXPECT_NEAR(stat1.sigma2(),36000000000000.0,0.1);
  EXPECT_NEAR(stat2.sigma2(),36000000000000.0,0.1);
  EXPECT_NEAR(stat3.sigma2(),36000000000000.0,0.1);

  EXPECT_NEAR(stat1.stddev(),707.106825381,0.001);
  EXPECT_NEAR(stat2.stddev(),707.106825381,0.001); 
  EXPECT_NEAR(stat3.stddev(),707.106825381,0.001);

  EXPECT_EQ(stat1.nsample(),8000000);
  EXPECT_EQ(stat2.nsample(),8000000);
  EXPECT_EQ(stat3.nsample(),8000000);

}
#endif
