
#if defined(interp) && defined(makecint)
#pragma include "test.dll"
#else
#include "t627.h"
#endif

int main() {
  GetDefaultGC()();
  GetDefaultGC()(3);
  f(GetDefaultGC()(2));
  f(GetDefaultGC()());
  for(int i=0;i<3;i++) {
    GetDefaultGC()();
    GetDefaultGC()(i);
    f(GetDefaultGC()());
    f(GetDefaultGC()(i));
  }
  return 0;
}
