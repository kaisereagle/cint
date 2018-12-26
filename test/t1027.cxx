
// File t02.C
#if defined(interp) && defined(makecint)
#pragma include "test.dll"
#else
#include "t1027.h"
#endif

void t02() {
  myclass m;
  int a = m;
  printf("created an int of value %d\n",a);
}  

int main() {
  t02(); 
  return 0;
}
