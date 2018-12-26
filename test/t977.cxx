

#if defined(interp) && defined(makecint)
#pragma include "test.dll"
#else
#include "t977.h"
#endif

#include <stdio.h>

void test(void (*f)(void)) {
  try {
    f();
  }
  catch(r_exception& x) {
    printf("caught r_exception %s\n",x.what());
  }
  catch(r_space::r_space_exception& x) {
    printf("caught r_space::r_space_exception %s\n",x.what());
  }
  catch(r_space::eh1::eh_exception1& x) {
    printf("caught r_space::eh1::eh_exception1 %s\n",x.what());
  }
  catch(r_space::eh1::errorhandling::eh_exception& x) {
    printf("caught r_space::eh1::errorhandling::eh_exception %s\n",x.what());
  }
#ifdef G__EX
  catch(G__exception& y) {
    printf("caught G__exception %s  %s\n",y.what(),y.classname());
  }
#endif
  catch(exception& x) {
    printf("caught exception %s\n",x.what());
  }
  catch(int a) {
    printf("caught int %d\n",a);
  }
#if 0
  catch(float f) {
    printf("caught float %g\n",f);
  }
  catch(double d) {
    printf("caught double %g\n",d);
  }
  catch(long b) {
    printf("caught long %ld\n",b);
  }
  catch(void* p) {
    printf("caught void* %p %d\n",p,*(int*)p);
  }
#endif
  catch(...) {
    printf("caught unknown exception\n");
  }
}


int main() {
  test(throw_std_exception);
  test(throw_r_exception);
  test(throw_r_space_exception);
  test(throw_r_space_eh_exception);
  test(throw_r_space_eh_errorhandling_exception);
  test(throw_int);
  //test(throw_long);
  //test(throw_void);
  //test(throw_float);
  //test(throw_double);
  return 0;
}

