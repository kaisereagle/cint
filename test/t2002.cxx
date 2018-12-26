#ifdef INTERPRET
#include "t2002.h"
#else
#include "t2002.dll"
#endif
#include <vector>

class B {
 public:
  vector<void*> vv;
  A aobj;
  A aobj2;
  void disp() {
    
    printf("B\nthis=%p &vv=%p %d &aobj=%p\n",this,&vv,vv.size(),&aobj);
    aobj.disp();
    //printf("this=%p &aobj2=%p\n",this,&aobj2);
    //aobj2.disp();
  }
};

int main() {
  //A a;
  //a.disp();
  //printf("\n");
  //B b;
  //b.disp();
  G__pause();

  for(int i=0;i<3;i++) {
    B b2;
    b2.disp();
  }
  return(0);
}
