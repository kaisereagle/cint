
#ifdef __CINT__
#pragma include "test.dll"
#else
#include "t961.h"
#endif

#include <stdio.h>

void test() {
  printf("success\n");
}

int main() {
  test(); 
  return 0;
}
