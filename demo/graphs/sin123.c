#include <array.h>
const double PI=3.141592;
main() {
  array x=array(0,PI*2,100);
  plot << "Signal processing example"
       << x
       << sin(x)                << "sin(1)"
       << square(x*2)           << "square(2)"
       << sin(x*3)              << "sin(3)"
       << diff(x,sin(x*3))      << "diff(sin(3))"
       << abs(diff(x,sin(x*3))) << "abs(diff(sin(3)))"
       << rand(x)               << "rand()"
       << endl;
}

#ifdef G__ROOT
void sin123() { main(); }
#endif
