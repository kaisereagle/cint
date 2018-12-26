#include <stdio.h>

int main() {
  bool b[3] = {true,false,true};
  for(int i=0;i<3;i++) printf("bool b[%d]=%d\n",i,b[i]);
  return 0;
}
