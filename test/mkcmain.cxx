
#ifdef __CINT__
#include DNAME
#else
#include HNAME
#endif

#include <stdio.h>

void test() {
  printf("success\n");
}

int main() {
  test(); 
  return 0;
}
