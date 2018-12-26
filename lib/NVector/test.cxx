
#include <NVector.h>
void test() {
  DVector x(0,10,128);
  DVector y1 = sin(x);
  DVector y2 = cos(x);
  plot << "Orig1" << x << y1 << y2 << endl;

#if 0
  fft(x,y1,y2);
 
  plot << "FFT1" << x << y1 << y2 << endl;
#else

  DVector f(0,0,128);
  DVector re = f;
  DVector im = f;
  FFT << x << y1 << y2 >> f >> re >> im >> "\n";
  plot << "FFT2" << f << re << im << endl;
#endif
}

