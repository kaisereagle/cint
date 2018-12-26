/******************************************************
* G__main.cxx
*  automatically generated main() function for cint
*  WinNT Symantec C++ environment
******************************************************/
#include <stdio.h>
extern "C" {
extern void G__setothermain(int othermain);
extern int G__main(int argc,char **argv);
extern void G__set_p2fsetup(void (*p2f)());
extern void G__free_p2fsetup();
extern void G__cpp_setupComplex();
}

int main(int argc,char **argv)
{
  int result;
  G__set_p2fsetup(G__cpp_setupComplex);
  G__setothermain(0);
  result=G__main(argc,argv);
  G__free_p2fsetup();
  return(result);
}
