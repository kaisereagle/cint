
#include <stdio.h>

class TGC {
 public:
  float operator()(int x=1) {
    printf("TGC::operator()(%d)\n",x);
    return 1.23*x;
  }
};

TGC dx;

TGC& GetDefaultGC() {
  printf("GetDefaultGC()\n");
  return dx;
}

void f(float x) {
  printf("f(x=%g)\n",x);
}
