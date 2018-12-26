
#include <stdio.h>

#define TEST

class A {
  int i;
 public:
  A(int iin=0) : i(iin) { }
  operator int() const { return i; }
  void disp() const { printf("%d\n",i); }
#ifdef TEST
  friend const A& operator+=(const A& a,const A& b) ;
#endif
  friend A& operator+=(A& a,int b) ;

};

#ifdef TEST
const A& operator+=(const A& a,const A& b) {
  printf("operator+=(const A&,const A&)\n");
  return(a);
}
#endif

A& operator+=(A& a,int b) {
  printf("operator+=(A&,int)\n");
  a.i += b;
  return(a);
}

int main() {
  A a(3);
  const A ac(4),ad(5);
  a.disp();
  a += 4;
  a.disp();
  ac += ad;
  a.disp();

  ac.disp();
  ac += 5;
  ac.disp();
  ac += ad;
  ac.disp();

  return 0;
}

