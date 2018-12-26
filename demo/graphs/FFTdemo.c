/******************************************************
* FFTdemo.c
*
* array pre-compiled class library is needed to run 
* this demo program.
******************************************************/
//#include <array.h>
//#include <constants.h>
//#include <fft.h>
#include <NVector.h>
#define NPOINT 1024
typedef DVector array;
typedef DCVector carray;

const double PI=3.141592;
main() {
  double width = 10*2*PI*(NPOINT-1)/NPOINT;
  array x=array(0 , width , NPOINT ),y; // start,stop,npoint
  y = sin(x) + sin(x*3)*0.5;
  plot << x << y << "\n" ;

  array freq,fftout;
  SPECTRUM << x << y >> freq >> fftout >> '\n';
  plot << freq << fftout << '\n';

  carray cfftout;
  FFT << x << y >> freq >> cfftout >> endl;
  plot << freq << cfftout << endl;
}
#ifdef G__ROOT
void FFTdemo() { main(); }
#endif
