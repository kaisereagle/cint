/********************************************************************
* cbstrm.h
********************************************************************/
#ifdef __CINT__
#error cbstrm.h/C is only for compilation. Abort cint.
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
#include "strstrm.h"
#include "linkdef.h"

#ifndef G__MEMFUNCBODY
#endif

extern G__linked_taginfo G__G__streamLN_mbstate_t;
extern G__linked_taginfo G__G__streamLN_ios_base;
extern G__linked_taginfo G__G__streamLN_ios_basecLcLio_state;
extern G__linked_taginfo G__G__streamLN_ios_basecLcLopen_mode;
extern G__linked_taginfo G__G__streamLN_ios_basecLcLseek_dir;
extern G__linked_taginfo G__G__streamLN_ios_basecLcLfmt_flags;
extern G__linked_taginfo G__G__streamLN_ios_basecLcLevent;
extern G__linked_taginfo G__G__streamLN_ios_basecLcLInit;
extern G__linked_taginfo G__G__streamLN_char_traitslEchargR;
extern G__linked_taginfo G__G__streamLN_basic_istreamlEcharcOchar_traitslEchargRsPgR;
extern G__linked_taginfo G__G__streamLN_basic_ioslEcharcOchar_traitslEchargRsPgR;
extern G__linked_taginfo G__G__streamLN_basic_streambuflEcharcOchar_traitslEchargRsPgR;
extern G__linked_taginfo G__G__streamLN_basic_ostreamlEcharcOchar_traitslEchargRsPgR;
extern G__linked_taginfo G__G__streamLN_basic_ostreamlEcharcOchar_traitslEchargRsPgRcLcLsentry;
extern G__linked_taginfo G__G__streamLN_basic_istreamlEcharcOchar_traitslEchargRsPgRcLcLsentry;
extern G__linked_taginfo G__G__streamLN_basic_filebuflEcharcOchar_traitslEchargRsPgR;
extern G__linked_taginfo G__G__streamLN_basic_ifstreamlEcharcOchar_traitslEchargRsPgR;
extern G__linked_taginfo G__G__streamLN_basic_ofstreamlEcharcOchar_traitslEchargRsPgR;
extern G__linked_taginfo G__G__streamLN_basic_fstreamlEcharcOchar_traitslEchargRsPgR;
extern G__linked_taginfo G__G__streamLN_basic_iostreamlEcharcOchar_traitslEchargRsPgR;
extern G__linked_taginfo G__G__streamLN_strstreambuf;
extern G__linked_taginfo G__G__streamLN_istrstream;
extern G__linked_taginfo G__G__streamLN_ostrstream;

/* STUB derived class for protected member access */
typedef char_traits<char> G__char_traitslEchargR;
typedef basic_istream<char,char_traits<char> > G__basic_istreamlEcharcOchar_traitslEchargRsPgR;
typedef basic_ios<char,char_traits<char> > G__basic_ioslEcharcOchar_traitslEchargRsPgR;
typedef basic_streambuf<char,char_traits<char> > G__basic_streambuflEcharcOchar_traitslEchargRsPgR;
typedef basic_ostream<char,char_traits<char> > G__basic_ostreamlEcharcOchar_traitslEchargRsPgR;
typedef basic_ostream<char,char_traits<char> >::sentry G__basic_ostreamlEcharcOchar_traitslEchargRsPgRcLcLsentry;
typedef basic_istream<char,char_traits<char> >::sentry G__basic_istreamlEcharcOchar_traitslEchargRsPgRcLcLsentry;
typedef basic_filebuf<char,char_traits<char> > G__basic_filebuflEcharcOchar_traitslEchargRsPgR;
typedef basic_ifstream<char,char_traits<char> > G__basic_ifstreamlEcharcOchar_traitslEchargRsPgR;
typedef basic_ofstream<char,char_traits<char> > G__basic_ofstreamlEcharcOchar_traitslEchargRsPgR;
typedef basic_fstream<char,char_traits<char> > G__basic_fstreamlEcharcOchar_traitslEchargRsPgR;
typedef basic_iostream<char,char_traits<char> > G__basic_iostreamlEcharcOchar_traitslEchargRsPgR;
