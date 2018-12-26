/********************************************************************
* vcstrm.h
********************************************************************/
#ifdef __CINT__
#error vcstrm.h/C is only for compilation. Abort cint.
#endif
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define G__ANSIHEADER
#define G__DICTIONARY
#include "G__ci.h"
extern "C" {
extern void G__cpp_setup_tagtableG__stream();
extern void G__cpp_setup_inheritanceG__stream();
extern void G__cpp_setup_typetableG__stream();
extern void G__cpp_setup_memvarG__stream();
extern void G__cpp_setup_globalG__stream();
extern void G__cpp_setup_memfuncG__stream();
extern void G__cpp_setup_funcG__stream();
extern void G__set_cpp_environmentG__stream();
}


#include "iostrm.h"
#include "fstrm.h"
#include "stdiostr.h"
#include "strstrm.h"

#ifndef G__MEMFUNCBODY
#endif

extern G__linked_taginfo G__G__streamLN_streambuf;
extern G__linked_taginfo G__G__streamLN_ostream;
extern G__linked_taginfo G__G__streamLN_ios;
extern G__linked_taginfo G__G__streamLN_ioscLcLio_state;
extern G__linked_taginfo G__G__streamLN_ioscLcLopen_mode;
extern G__linked_taginfo G__G__streamLN_ioscLcLseek_dir;
extern G__linked_taginfo G__G__streamLN_ioscLcLdA;
extern G__linked_taginfo G__G__streamLN_istream;
extern G__linked_taginfo G__G__streamLN_iostream;
extern G__linked_taginfo G__G__streamLN_filebuf;
extern G__linked_taginfo G__G__streamLN_ifstream;
extern G__linked_taginfo G__G__streamLN_ofstream;
extern G__linked_taginfo G__G__streamLN_fstream;
extern G__linked_taginfo G__G__streamLN_strstreambuf;
extern G__linked_taginfo G__G__streamLN_istrstream;
extern G__linked_taginfo G__G__streamLN_ostrstream;
extern G__linked_taginfo G__G__streamLN_strstream;

/* STUB derived class for protected member access */
