
#include <stdio.h>

#if defined(interp) && defined(makecint)
#pragma include "test.dll"
#else
#include "t1062.h"
#endif

int main() {
  printf("success\n");
  return(0);
}
