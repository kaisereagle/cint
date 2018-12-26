
#if defined(interp) && defined(makecint)
#pragma include "test.dll"
#else
#include "t996.h"
#endif

#include <stdio.h>

int main() {
  printf("success\n");
  return 0;
}

