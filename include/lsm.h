/*************************************************************************
* lsm.h
*
* Least square method library
*
*************************************************************************/

#ifndef G__LSM_H
#define G__LSM_H

#ifndef G__LSMSL 
# ifdef G__SHAREDLIB
//#pragma include <lsm.dll>
#pragma include_noerr <lsm.dll>
#ifndef G__LSMSL
#include <lsm.c>
#endif
# else
#include <lsm.c>
# endif
#endif // G__LSMSL

#endif
