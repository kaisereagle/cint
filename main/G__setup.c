/* /% C++ %/ */
/***********************************************************************
 * cint (C/C++ interpreter)
 ************************************************************************
 * Source file main/G__setup.c
 ************************************************************************
 * Description:
 *  Archived precompiled library initialization routine
 ************************************************************************
 * Copyright(c) 1995~1999  Masaharu Goto (cint@pcroot.cern.ch)
 *
 * Permission to use, copy, modify and distribute this software and its 
 * documentation for non-commercial purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  The author makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 ************************************************************************/

#ifdef G__CPPLINK_ON
void G__cpp_setup();
#endif
#ifdef G__CLINK_ON
void G__c_setup();
#endif

int G__globalsetup() {
#ifdef G__CPPLINK_ON
  G__cpp_setup();
#endif
#ifdef G__CLINK_ON
  G__c_setup();
#endif
  return(0);
}

