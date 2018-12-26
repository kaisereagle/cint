/*****************************************************************************
* MultiByteChar.cxx
*
* Multi byte character handling library
*
* 2015/Feb/18 Masaharu Goto
*
*****************************************************************************/

#include "MultiByteChar.h"

#if defined(__APPLE__)
int G__MultiByteMode::defaultlang = G__MultiByteMode::E__UNICODE;
#else
int G__MultiByteMode::defaultlang = G__MultiByteMode::E__UNKNOWNCODING;
#endif

