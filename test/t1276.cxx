
#if defined(interp) && defined(makecint)
#pragma include "test.dll"
#else
#include "t1276.h"
#endif


int main() {
  printf("Success\n");
  FOO::bug a(5);
  FOO::bug b(6);
  b = a + 1;
  b = a > 2;
  return 0;
}
