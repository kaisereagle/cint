#include <stdio.h>
class A {
public:
  int a;
  int ary[10];
  A(int ain) ;
  A func() ;
  void f1(int ain);
};

A::A(int ain) {
  int i;
  a=ain;
  for(i=0;i<10;i++) ary[i]=i+a;
}

A A::func() {
  A tmp=A(a),tmp1(7);
  f1(a);
  this->f1(a);
  tmp1.f1(a);
  printf("ary[i]=%d %d %d\n",ary[a],this->ary[a],tmp1.ary[a]);
  return(tmp);
}

void A::f1(int ain) {
  printf("a=%d ain=%d\n",a,ain);
}

int main() {
  A obja=A(1);
  obja.func();
  return 0;
}
