// 021203dupfile.txt

// -----------8<--------------------
// File i.inc
int i=0;

#include <stdio.h>
// File test.cpp
namespace testcpp0
{
  namespace A {
    #include "t958.h"
  };
  
  namespace B {
    #include "t958.h"
  };
  
  void DoStuff() {
    A::i++;
    A::f();
    B::f();
    printf("i=%d\n",i);
    B::i++;
    A::f();
    B::f();
    printf("i=%d\n",i);
    A::i=5;
    B::i=9;
    A::f();
    B::f();
    printf("i=%d\n",i);
  }
}
// -----------8<--------------------

int main() {
  testcpp0::DoStuff();
  return  0;
}
