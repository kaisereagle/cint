#pragma compile   /* Native Code Compilation only supported with DLL */
#include <stdio.h>
#include <math.h>
double NativeCalc(int size) {
  int i,sum=0;
  for(i=0;i<size;i++)  sum+=i;
  return(sum);
}
#pragma endcompile  

main() {
  printf("result=%d\n",NativeCalc(1000000));
}
