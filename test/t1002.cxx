

#if defined(interp) && defined(makecint)
#pragma include "test.dll"
#else
#include "t1002.h"
#endif

//#include <stdio.h>
#include <iostream>
using namespace std;

int main() {
  //printf("success\n");
  cout <<"success"<<endl;
  Marshal<double> d;
  Marshal<int> i;
  return 0;
}

