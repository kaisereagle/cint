#include <stdio.h>
class A { 
 public:
  int b;
  static int a;
};

int A::a=3229;

void test1()
{
  A *a=new A;
  A* b=new A;

  printf("a->a=%d\n",a->a);
  a->a += 1;
  printf("b->a=%d\n",b->a);

  delete a;
  delete b;
}

int main()
{
  test1();
  return 0;
}
