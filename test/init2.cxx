#ifdef __hpux
#include <iostream.h>
#else
#include <iostream>
using namespace std;
#endif

int k(117);
double d(3.14);

int main() {
  cout << "k=" << (unsigned char)k << endl;
  cout << "k=" << k << endl;
  cout << "d=" << d << endl;
  return 0;
}
