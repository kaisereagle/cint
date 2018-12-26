
#if defined(interp) && defined(makecint)
#pragma include "test.dll"
#else
#include "t1067.h"
#endif

int main() {
  TString a("abc");
  TTree t;
  t.Draw(a);

  TString b("def");
  t.Draw(a,b);

  return(0);
}

