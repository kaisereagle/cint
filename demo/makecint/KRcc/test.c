#include "Complex.h"

main()
{
  struct Complex a,b,c,d;

  ComplexInit();

  ComplexSet(&a,2,-2);
  ComplexDisplay(a);

  b=ComplexExp(a);
  ComplexDisplay(b);

  c=ComplexAdd(a,b);
  ComplexDisplay(c);

  d=ComplexMultiply(a,b);
  ComplexDisplay(d);

  printf("%g %g %g %g\n"
	 ,ComplexImag(j) ,ComplexAbs(b) ,ComplexReal(c) ,ComplexImag(d));
}

