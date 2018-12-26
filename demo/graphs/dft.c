/******************************************************
* dft.c
*
* array pre-compiled class library is needed to run 
* this demo program.
******************************************************/
#include <array.h>
#include <constants.h>
#include <fft.h>
#define NPOINT 512
main() {
  double width = 2*PI*(NPOINT-1)/NPOINT;
  array x=array(0 , width , NPOINT ),y; // start,stop,npoint
  y = sin(x) + cos(2*x)+0;
  plot << x << y << "\n" ;

  array freq,fftout;
  spectrum << x << y >> freq >> fftout >> endl;
  // plot << freq << fftout << endl;

  carray cfftout;
  fft << x << y >> freq >> cfftout >> '\n';
  plot << freq << cfftout << '\n';
}
#ifdef G__ROOT
void dft() { main(); }
#endif
