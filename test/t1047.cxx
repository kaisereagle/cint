
// demo/exception/ehdemo.cxx
// This source has to be interpreted

#include <stdio.h>

#if defined(interp) && defined(makecint)
#pragma include "test.dll"
#else
#include "t1047.h"
#endif


////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
int main() {
  printf("success\n");
  printf("%s %s %s\n",test::abc.c_str(),test::default_str,test::default_strx);
  return 0;
}
