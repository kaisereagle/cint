/*************************************************************************
* eular.c
*
* array pre-compiled class library is needed to run this demo program.
*************************************************************************/
#include <array.h>
const complex j=complex(0,1);
const double PI=3.141592;

#ifdef G__ROOT
void eular() { main(); }
#endif

main() {
  array x=array(-2*PI , 2*PI , 100 ); // start,stop,npoint
  plot << "Eular's Law" << x << exp(x*j) << "exp(x*j)" << endl ;
}
