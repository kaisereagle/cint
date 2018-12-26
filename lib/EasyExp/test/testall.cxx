///////////////////////////////////////////////////////////////////////////
// EasyExp library test suite
//
//  2015/Aug/12  Masaahru Goto
///////////////////////////////////////////////////////////////////////////
#include <_util_cinttest.h>
#pragma include <EasyExp.dll>

int main(int argc,char** argv) {
  string opt,cintopt;
  for(int i=1;i<argc;i++) {
    if(strcmp(argv[i],"-q")==0) opt += " -q ";
    else if(strcmp(argv[i],"-f")==0) opt += " -f ";
    else if(strcmp(argv[i],"-p")==0) cintopt += " -p ";
    else if(argv[i][0]=='-') {
      printf("testall.cxx : Unknown option %s\n",argv[i]);
      printf("Usage: cint testall.cxx <-q> <-p>\n");
      printf("   -q  : quick test\n");
      printf("   -p  : use C/C++ preprocessor \n");
      printf("   -f  : intentional error occurance \n");
      return(0);
    }
  }
  int err=0;
  REMOVE_TOTAL_PASSFAIL();
#ifndef NO_GTEST
  err += gtest("../gtest/test1/test1.cxx");
  err += gtest("../gtest/test1/test2.cxx");
  err += gtest("../gtest/test1/test3.cxx");
  err += gtest("../gtest/test1/test4.cxx");
  // err += gtest("../gtest/test1/test5.cxx"); // depends on files in this directory
  err += gtest("../gtest/test1/test6.cxx");
  err += gtest("../gtest/test1/test7.cxx");
#endif

#ifndef NO_CTEST
  err += ctest(cintopt,"compile.cxx",opt);
  err += ctest(cintopt,"match.cxx",opt);
  err += ctest(cintopt,"replace.cxx",opt);
  err += ctest(cintopt,"scandir.cxx",opt);
  err += ctest(cintopt,"synthesis.cxx",opt);
  err += ctest(cintopt,"multithread.cxx",opt);
#endif

  READ_TOTAL_PASSFAIL();
  return(err);
}
