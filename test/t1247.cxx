#if defined(__CINT__) && !defined(INTERPRET)
#pragma include "test1.dll"
#pragma include "test2.dll"
#else
#include "t1247.h"
#include "t1247a.h"
#endif

#include <stdio.h>

int main() {
  printf("Success\n");
  return 0;
}


