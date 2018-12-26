
#include <array.h>
t1100() {
  array x(0.0,1.0,100);
  array y=x;

  plot << x << y << "y" << "sec" << endl;

  plot << "title1a" << x << "V" << y  << "y" << endl;
  same << "title2a" << x << y*2 << "y*2" << endl;
  same << "title3a" << x/2 << "Volt" << y*3 << "y*3" << "C(F)\n";

  plot << "title1b" << x << "V" << y  << "y" << flush;
  plot << "title2b" << x << y*2 << "y*2" << flush;
  plot << "title3b" << x/2 << "Volt" << y*3 << "y*3" << "C(F)\n";
}
