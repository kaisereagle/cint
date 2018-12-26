
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
TEST(ScandirTest, getX)
{
  unsigned int i;
  Scandir d("../*.cxx");
  vector<string> filelist;

  d.get(filelist);
  EXPECT_EQ(d.size(),7) ;
  EXPECT_EQ(filelist.size(),7) ;
  EXPECT_EQ(filelist[0],"./../test1.cxx") ; // questionable tests
  EXPECT_EQ(filelist[1],"./../test2.cxx") ;
  EXPECT_EQ(filelist[2],"./../test3.cxx") ;
  EXPECT_EQ(filelist[3],"./../test4.cxx") ;
  EXPECT_EQ(filelist[4],"./../test5.cxx") ;
  EXPECT_EQ(filelist[5],"./../test6.cxx") ;
  EXPECT_EQ(filelist[6],"./../test7.cxx") ;
  const char *fname; i=0;
  while((fname=d.Next())&&fname[0]) {
    EXPECT_EQ(filelist[i++],fname) ;
  }

  filelist.clear();
  d.getfullpath(filelist);
  EXPECT_EQ(d.size(),7) ;
  EXPECT_EQ(filelist.size(),7) ;
  EXPECT_EQ(filelist[0],"./../test1.cxx") ; // questionable tests
  EXPECT_EQ(filelist[1],"./../test2.cxx") ;
  EXPECT_EQ(filelist[2],"./../test3.cxx") ;
  EXPECT_EQ(filelist[3],"./../test4.cxx") ;
  EXPECT_EQ(filelist[4],"./../test5.cxx") ;
  EXPECT_EQ(filelist[5],"./../test6.cxx") ;
  EXPECT_EQ(filelist[6],"./../test7.cxx") ;
  i=0;
  while((fname=d.Next())&&fname[0]) {
    EXPECT_EQ(filelist[i++],fname) ;
  }

  filelist.clear();
  d.getfname(filelist);
  EXPECT_EQ(d.size(),7) ;
  EXPECT_EQ(filelist.size(),7) ;
  EXPECT_EQ(filelist[0],"test1.cxx") ;
  EXPECT_EQ(filelist[1],"test2.cxx") ;
  EXPECT_EQ(filelist[2],"test3.cxx") ;
  EXPECT_EQ(filelist[3],"test4.cxx") ;
  EXPECT_EQ(filelist[4],"test5.cxx") ;
  EXPECT_EQ(filelist[5],"test6.cxx") ;
  EXPECT_EQ(filelist[6],"test7.cxx") ;

  filelist.clear();
  d.getpath(filelist);
  EXPECT_EQ(d.size(),7) ;
  EXPECT_EQ(filelist.size(),7) ;
  EXPECT_EQ(filelist[0],"./..") ; // questionable tests
  EXPECT_EQ(filelist[1],"./..") ;
  EXPECT_EQ(filelist[2],"./..") ;
  EXPECT_EQ(filelist[3],"./..") ;
  EXPECT_EQ(filelist[4],"./..") ;
  EXPECT_EQ(filelist[5],"./..") ;
  EXPECT_EQ(filelist[6],"./..") ;

}

///////////////////////////////////////////////////////////
TEST(ScandirTest, Scan)
{
  unsigned int i;
  Scandir d("../*",1);
  vector<string> filelist;

  d.get(filelist);
  EXPECT_EQ(d.size(),1) ;
  EXPECT_EQ(filelist.size(),1) ;
  EXPECT_EQ(filelist[0],"./../build") ; // questionable tests
  const char *fname; i=0;
  while((fname=d.Next())&&fname[0]) {
    EXPECT_EQ(filelist[i++],fname) ;
  }

  d.scan("../[^#]*",0);
  EXPECT_EQ(d.size(),8);

  d.scan("../[^#]*",1);
  EXPECT_EQ(d.size(),1);

  d.scan("../[^#]*",2);
  EXPECT_EQ(d.size(),9);
}

///////////////////////////////////////////////////////////
TEST(ScandirTest, rewind)
{
  unsigned int i;
  Scandir d("../*.cxx");
  vector<string> filelist;

  d.get(filelist);
  EXPECT_EQ(d.size(),7) ;
  EXPECT_EQ(filelist.size(),7) ;
  EXPECT_EQ(filelist[0],"./../test1.cxx") ; // questionable tests
  EXPECT_EQ(filelist[1],"./../test2.cxx") ;
  EXPECT_EQ(filelist[2],"./../test3.cxx") ;
  EXPECT_EQ(filelist[3],"./../test4.cxx") ;
  EXPECT_EQ(filelist[4],"./../test5.cxx") ;
  EXPECT_EQ(filelist[5],"./../test6.cxx") ;
  EXPECT_EQ(filelist[6],"./../test7.cxx") ;
  const char *fname; i=0;
  while((fname=d.Next())&&fname[0]) {
    EXPECT_EQ(filelist[i++],fname) ;
  }
  d.rewind();
  i=0;
  while((fname=d.Next())&&fname[0]) {
    EXPECT_EQ(filelist[i++],fname) ;
  }
}

///////////////////////////////////////////////////////////
TEST(ScandirTest, clear)
{
  Scandir d("../*.cxx");
  EXPECT_EQ(d.size(),7) ;
  d.clear();
  EXPECT_EQ(d.size(),0) ;
}


///////////////////////////////////////////////////////////
TEST(ScandirTest, next)
{
  unsigned int i;
  Scandir d("../*.cxx");
  vector<string> filelist;

  d.get(filelist);
  EXPECT_EQ(d.size(),7) ;
  EXPECT_EQ(filelist.size(),7) ;
  EXPECT_EQ(filelist[0],"./../test1.cxx") ; // questionable tests
  EXPECT_EQ(filelist[1],"./../test2.cxx") ;
  EXPECT_EQ(filelist[2],"./../test3.cxx") ;
  EXPECT_EQ(filelist[3],"./../test4.cxx") ;
  EXPECT_EQ(filelist[4],"./../test5.cxx") ;
  EXPECT_EQ(filelist[5],"./../test6.cxx") ;
  EXPECT_EQ(filelist[6],"./../test7.cxx") ;
  string fname; i=0;
  while((fname=d.next())!="") {
    EXPECT_EQ(filelist[i++],fname) ;
  }
}

///////////////////////////////////////////////////////////
TEST(ScandirTest, clearx)
{
  Scandir d("../*.cxx");
  EXPECT_EQ(d.size(),7) ;
  d.clear();
  EXPECT_EQ(d.size(),0) ;
}

///////////////////////////////////////////////////////////
TEST(ScandirTest, scandisp)
{
  Scandir d; d.scandisp("../*.cxx");
  EXPECT_EQ(d.size(),7) ;
  d.clear();
  EXPECT_EQ(d.size(),0) ;
}

