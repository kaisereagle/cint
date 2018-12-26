// 021218loop0-2.txt,  related with t964.cxx(root)

#ifdef __CINT__
#pragma include "test.dll"
#else
#include "t970.h"
#endif

int main() {
  TVector indice(4);
  for(int i=2;i<7;i++) {
    if(i!=2) {
#ifdef __CINT__
      TMatrixRow(i) = indice;
#else
      TMatrixRow x(i);
      x=indice;
#endif
    }
  }
  return  0;
}
