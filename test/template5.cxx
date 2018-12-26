
#include <stdio.h>

#if 0

template<class T*> 
struct A  {
  T a;
};

#else

template<class T> 
struct A {
  typename T::x a;
};

// template specialization
template<class T> 
struct A <T*> {
  T a;
};

#endif

class C {
public:
  typedef int x;
};

A<char*> a;
A<C> b;

int main() {
  a.a = 'a';
  b.a = 123;
  printf("a.a=%c b.a=%d\n",a.a,b.a);
  return 0;
}
