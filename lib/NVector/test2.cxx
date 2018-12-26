

#include <NVector.h>
void test2() {
  DVector x(0,10,128);
  DVector y1 = sin(x);
  DVector y2 = cos(x);
  plot << "Orig1" << x << y1 << y2 << endl;

#if 0
  spectrum(x,y1);
 
  plot << "SPECTRUM1" << x << y1 << y2 << endl;
#else

  DVector f(0,0,128);
  DVector re = f;
  DVector im = f;
  SPECTRUM << x << y1 << y2 >> f >> re >> im >> "\n";
  plot << "SPECTRUM2" << f << re << im << endl;
#endif
}

