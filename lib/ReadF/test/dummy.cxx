//////////////////////////////////////////////////////////////////////
// ReadF test dummy 
//
//  2015/Aug/12  Masaahru Goto
//////////////////////////////////////////////////////////////////////
#pragma include <ReadF.dll> // Test Target
#include "_util_cinttest.h"

int pass=0,fail=0;
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
int test1() {
  printf("test1\n");
  RESET_LOCALFAIL();

  ReadF f;
  f.parse("abc def 0123");
  EXPECT_EQ(f.argc,3);
  EXPECT_EQ(string("abc def 0123"),f.argv[0]);
  EXPECT_EQ(string("abc"),f.argv[1]);
  EXPECT_EQ(string("def"),f.argv[2]);
  EXPECT_EQ(string("0123"),f.argv[3]);

  f.parse("abc, def ,0123");
  EXPECT_EQ(f.argc,3);
  //EXPECT_EQ(string("abc, def ,0123",f.argv[0]);//bug in CINT macro instantiation
  string a= "abc, def ,0123"; EXPECT_EQ(a,f.argv[0]);//workaround
  EXPECT_EQ(string("abc,"),f.argv[1]);
  EXPECT_EQ(string("def"),f.argv[2]);
  EXPECT_EQ(string(",0123"),f.argv[3]);
  
  return(LOCALFAIL_COUNT());
}


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
#ifdef DEBUG
  EasyExp_DEBUG=2;
#endif

  int err=0; 
  printf("### dummy test start\n");

  err += test1();  // StringList StarStringList with zero length string

  fail=err;
  SAVE_PASSFAIL("dummy");
  exit(err);
}

//////////////////////////////////////////////////////////////////////
