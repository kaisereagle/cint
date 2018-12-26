
// -- file const.C
//#include "iostream.h"
#include <stdio.h>

class MyClass4 {};

const MyClass4* g(const MyClass4* arg ) {
  printf("with const\n");
  //cout << "with const MyClass4*" << endl;
  return arg;
}

MyClass4* g(MyClass4* arg  ) {
  printf("without const\n");
  //cout << "with MyClass4*" << endl;
  return arg;
}

void run() {
  g( (const MyClass4*)0 );
  g( (MyClass4*)0 );

}

int main() {
  run();
  return 0;
}
