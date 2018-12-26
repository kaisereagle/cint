#ifdef __hpux
#include <iostream.h>
#else
#include <iostream>
using namespace std;
#endif

template<class T,class E> class A {
 T t;
 E e;
public:
 A(T tin,E ein) { t=tin; e=ein; }
 void disp() { cout << "t=" << t << " e=" << e << endl; }
};

template<class T,class E> void f(A<T,E>& x) 
{
  x.disp();
}

int main() {
  A<char,int> y('a',3229);
  A<double,char*> z(3.14,"abcdef");
  f(y);
  f(z);
  return 0;
}

