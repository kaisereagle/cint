#if defined(interp) && defined(makecint)
#pragma include "test.dll"
#else
#include "vbase1.h"
#endif

int main(){
  btest();
  ctest();
  dtest();
  etest();
  ftest();
  gtest();
  return 0;
}
