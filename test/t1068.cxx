
#if defined(interp) && defined(makecint)
#pragma include "test.dll"
#else
#include "t1068.h"
#endif

//using namespace FOO;

int main() {
  A a;
  //A b = a+3.14;
  func();
  FOO::bug c(10);
  FOO::bug d = c+10;
  a.disp();
  d.disp();
  return 0;
}
