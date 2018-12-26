#include <NVector.h>

int main() {

  DVector time(0,1e-6,1024), car=time/2e-9, mod=time/2e-7
    , sig = sin(car)*cos(mod)+cos(car)*cos(mod*5) ;

  DVector I = sig * sin(car);
  DVector Q = sig * cos(car);

  plot << "IQ Modulation 2" 
       << time << "time"
       << sig  << "signal"
       << I    << "I"
       << Q    << "Q"
       << endl;

  DVector f,spec;
  SPECTRUM << time << sig >> f >> spec << endl;
  plot << "Frequency Domain"
       << f << "freq"  << 0 >> 200e6 
       << spec << "spectrum"
       << endl;

  cintgraph_stop();
  return(0);
}
