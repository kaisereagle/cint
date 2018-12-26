/**************************************************************************
* cint (C++ Interpreter)
************************************************************************
* Source file testmain.c
************************************************************************
* Description:
*  Test version of G__main.c. With testmain.c, you can interpret
* cint source code by cint itself and it interprets another C program.
************************************************************************
* To execute
*  $ cint G__ci.c G__cfunc.c G__setup.c G__testmain.c <source file>
*
* For example,
*  $ cint -I$CINTSYSDIR -I$CINTSYSDIR/src +P G__testmain.c simple.c
*                                            ------------- -----------
*                                            this part is  this part is
*                                            interpreted   interpreted
*                                            by compiled   by interpreted
*                                            cint.         cint
*  
************************************************************************
*
* (C) Copyright 1991,1992,1993  Yokogawa Hewlett Packard HSTD R&D
* Author                        Masaharu Goto (gotom@tky.hp.com)
*
*  Refer to README file for conditions of using, copying and distributing
* CINT.
*
**************************************************************************/

#define G__TESTMAIN
#undef G__SHAREDLIB

#undef G__MEMTEST

#define G__POSIX_H

#include "src/auxu.c"
#include "src/cast.c"
#include "src/debug.c"
#include "src/decl.c"
#include "src/disp.c"
#include "src/dump.c"
#include "src/end.c"
#include "src/error.c"
#include "src/expr.c"
#include "src/fread.c"
#include "src/func.c"
#include "src/gcoll.c"
#include "src/global1.c"
#include "src/global2.c"
#include "src/g__cfunc.c"
#include "src/ifunc.c"
#include "src/inherit.c"
#include "src/init.c"
#include "src/input.c"
#include "src/intrpt.c"
#include "src/loadfile.c"
#include "src/macro.c"
#include "src/malloc.c"
/* #include "src/memtest.c" */
#include "src/new.c"
#include "src/newlink.c"
#include "src/oldlink.c"
#include "src/opr.c"
#include "src/parse.c"
#include "src/pause.c"
#include "src/pcode.c"
#include "src/pragma.c"
#include "src/quote.c"
#include "src/scrupto.c"
#include "src/shl.c"
#include "src/sizeof.c"
#include "src/struct.c"
#include "src/stub.c"
#include "src/sunos.c"
#include "src/tmplt.c"
#include "src/typedef.c"
#include "src/val2a.c"
#include "src/value.c"
#include "src/var.c"
#include "src/dmystrm.c"

#ifndef G__XREF
#include "src/Api.cxx"
#include "src/Class.cxx"
#include "src/BaseCls.cxx"
#include "src/CallFunc.cxx"
#include "src/DataMbr.cxx"
#include "src/Method.cxx"
#include "src/MethodAr.cxx"
#include "src/Token.cxx"
#include "src/Type.cxx"
#include "src/Typedf.cxx"
#endif

#include "main/G__setup.c"

extern short G__othermain;

main(argc,argv)
int argc;
char *argv[];
{
	G__othermain=0;
	return(G__main(argc,argv));
}


#ifdef G__NEVER
signal(signal,func)
int signal;
void *func;  // void (*func)();
{
}
#endif

alarm(int time) // dummy
{
}

void *rl_attempted_completion_function;
char *rl_basic_word_break_characters;


G__ASSERT(int x) // dummy
{
}

#ifndef G__CHECK
G__CHECK(x,y,z)
int x;
int y;
int z;
{
}
#endif
