

#if defined(interp) && defined(makecint)
#pragma include "test.dll"
#else
#include "t1178.h"
#endif

int main() {
  CTest a;
  PointVectorIterator iter;
  a.Execute(iter);
  a.f(iter);
  a.g(iter);
  a.h(iter);
  return 0;
}
