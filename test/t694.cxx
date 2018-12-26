
#if defined(__CINT__) && !defined(INTERPRET)
#pragma include "test.dll"
#else
#include "t694.h"
#endif

int main() {
  A x;
  //printf("t694 causes problem due to 1558, default param evaluation scheme\n");
  for(int i=0;i<2;i++) {
    //printf("%d %d ",x.add(i),x.add(i,i+1));
    h(x.add(i),x.add(i,i+1));
    f(i);
    f();
    g(i);
    g();
    //printf("\n");
    endline();
  }
  return 0;
}
