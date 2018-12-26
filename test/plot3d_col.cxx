
#include <CSV.h>

double sinc2d(double x,double y) {
  double r = sqrt( x*x + y*y );
  double z;
  if(r==0) z=1.0;
  else     z = sin(r)/r;
  return(z);
}

int main() {
  DMatrix m;
  DVector x(-15,15,50);

  m = ToDMatrix(x,x,sinc2d);

  Collection c;
  c.let(m);

#ifdef ROOT
  cintgraph_mode();
  draw << c << endl;
  G__pause();
  cintgraph_stop();
#else
  gnuplot_mode();
  draw << c << endl;
#endif

  DMatrix m2 = c.DMatrix();
  draw << "TiTLe" << "XnamE" << c << endl;

}
