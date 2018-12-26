
#include <stdio.h>

class A {
 public:
  A() { printf("A()\n"); }
  ~A() { printf("~A()\n"); }
};

A f1() {
 A a;
 return a;
}

int main() {
  const A& b=f1();
  //A  c=f1();
  //G__pause();
  //printf("z\n");
  return 0;
}
