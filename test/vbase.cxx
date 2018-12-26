#if defined(interp) && defined(makecint)
#pragma include "test.dll"
#else
#include "vbase.h"
#endif

int main(){
 btest();
 ctest();
 dtest();
 etest();
 ftest();
 gtest();
 return 0;
}
