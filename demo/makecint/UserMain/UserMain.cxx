// UserMain.cxx

#include <stdio.h>
#include <G__ci.h>
#include "UserMain.h"   /* host.h can be an empty file */
#ifdef G__CYGWIN
extern "C" void G__cpp_setup(void);
#endif

//////////////////////////////////////////////////////////////////////
// main application 
//////////////////////////////////////////////////////////////////////
void TheSimplestExample() {
  G__init_cint("cint script.cxx");
  G__scratch_all();
}

void LoadScriptfileAfterwards() {
  int state;
  state=G__init_cint("cint");
  switch(state) {
  case G__INIT_CINT_SUCCESS_MAIN:
    /* Should never happen */
    break;
  case G__INIT_CINT_SUCCESS:
    state=G__loadfile("script.cxx");
    if(state==G__LOADFILE_SUCCESS) {
      // G__calc and G__exec_text API can be used to evaluate C/C++ command
      // Read doc/ref.txt for those APIs.
      G__calc("script(\"Calling from compiled main application 1\")");
      G__exec_text("script(\"Calling from compiled main application 2\")");
    }
    break;
  case G__INIT_CINT_FAILURE:
  default:
    printf("Cint initialization failed.\n");
  }
  G__scratch_all();
}

int main() {
#ifdef G__CYGWIN
  G__add_setup_func("UserMain",G__cpp_setup);
#endif
  TheSimplestExample();
  LoadScriptfileAfterwards();
}

//////////////////////////////////////////////////////////////////////
// precompiled library
//////////////////////////////////////////////////////////////////////
void f1(int i) {
  printf("f1(%d)\n",i);
}

void f2(double d) {
  printf("f2(%g)\n",d);
}

