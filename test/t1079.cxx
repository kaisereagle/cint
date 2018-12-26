
#include <stdio.h>

#if defined(interp) && defined(makecint)
#pragma include "test.dll"
#else
#include "t1079.h"
#endif

template<class T> 
void f(T& x) {
  smart_ptr<T> p(&x);
  printf("%s\n",p->c_str());
}

int main() {
  string x("stringx");
  f(x);
  String y("Stringy");
  f(y);
  return 0;
}
