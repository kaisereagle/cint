#include "simple.h"

void Simple::f1(int a,double b) {
  printf("Simple::f() a=%d b=%g\n",a,b);
}

void f1(int a,double b) {
  printf("::f()  a=%d b=%g\n",a,b);
}
