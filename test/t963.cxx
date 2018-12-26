// 021211rootcint.txt, Maarten 
// 021214rootcint.txt
#include "t963.h"

int main() {
  TPhTopoDataI a;
  TPhTopoDataI b(a);
  TPhTopoDataI::TError e;
  a.disp();
  b.disp();
  e.disp();
  return 0;
}
