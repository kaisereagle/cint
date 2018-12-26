
// test.C
//#include <iostream>
#include <stdio.h>
class A
{
 public:
  class B;
};

class A::B
{
public:
  int b;
};

int main()
{
  A::B bb;
  
  bb.b = 3;
  //cerr<<bb.b<<endl;
  printf("%d\n",bb.b);
  return 0;
}

