
#if defined(interp) && defined(makecint)
#pragma include "test.dll"
#else
#include "t995.h"
#endif

#include <stdio.h>

int main() {
  List<BaseStar*>::iterator first;
  printf("success\n");
  return 0;
}
