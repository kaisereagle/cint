#if defined(interp) && defined(makecint)
#pragma include "test.dll"
#else
#include "template.h"
#endif

void test(void);
int main()
{
   test();
   return 0;
}
