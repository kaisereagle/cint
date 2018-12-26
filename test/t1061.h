
#include <stdio.h>
#include <vector>

using namespace std;

class A {
 public:
};

void f(vector<A*> vf) { }

class B {
 public:
  typedef vector<float>::iterator vfi;
  void g(vfi x) { }

};

#ifdef __MAKECINT__
#pragma link off all classes;
#pragma link off all typedefs;
#pragma link C++ class A;
#pragma link C++ class B;
#endif

