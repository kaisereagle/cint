
#pragma hdrstop
#include <condefs.h>


//---------------------------------------------------------------------------
#pragma argsused

#include <stdio.h>

USELIB("..\..\libcint.lib");
//---------------------------------------------------------------------------
extern "C" {
extern void G__setothermain(int othermain);
extern int G__main(int argc,char **argv);
}

int main(int argc,char **argv)
{
  G__setothermain(0);
  return(G__main(argc,argv));
}


