
#include <stdio.h>
#if defined(interp) && defined(makecint)
#pragma include "test.dll"
#else
#include "t1035.h"
#endif

int main() {
  TestOperator a;

  unsigned int x;

  x = a;

  printf("%d\n",x);

  return 0;
}

