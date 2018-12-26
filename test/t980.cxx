
#if defined(interp) && defined(makecint)
#pragma include "test.dll"
#else
#include "t980.h"
#endif
#include <stdio.h>

int main() {
  A b ;
  b = A("A part") + " of a whole";
  A a = A("A part") + " of a whole";
  printf("%s. %s.\n",a.val(),b.val());

  f(a,"A part of a whole");
  f("A part of a whole",a);

  if(strcmp(a,"A part of a whole")==0) printf("true\n");
  else printf("false\n");
  if(strcmp(a,"a part of a whole")==0) printf("true\n");
  else printf("false\n");

  if(strcmp(a.val(),"A part of a whole")==0) printf("true\n");
  else printf("false\n");
  if(strcmp(a.val(),"a part of a whole")==0) printf("true\n");
  else printf("false\n");
  return 0;
}

