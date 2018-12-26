

#if defined(interp) && defined(makecint)
#pragma include "test.dll"
#else
#include "t1011.h"
#endif

#include <iostream>


int main() {
  std::cout << "success" << endl;
  MyClass a;
  a.something.no_op();
  return 0;
}
