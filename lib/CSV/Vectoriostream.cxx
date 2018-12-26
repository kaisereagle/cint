/***************************************************************************
* Vectoriostream.h
*
*  DVector and DCVector type graph plot and file I/O class
****************************************************************************/
/************************************************************************
 * Copyright(c) 1995~2015  Masaharu Goto (cint@pcroot.cern.ch)
 *
 * Permission to use, copy, modify and distribute this software and its 
 * documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation. The author makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 ************************************************************************/

#include "Vectoriostream.h"

UNITCIRCLE_t UNITCIRCLE;
ISLOG LOG=1;
ISLOG LIN=0;

#if !defined(_WIN32) && !defined(G__WIN32)
G__CINT_ENDL    endl;
G__CINT_ENDS    ends;
G__CINT_FLUSH   flush;
G__CINT_ws      ws;
G__CINT_WS      WS;
G__CINT_HEX     hex;
G__CINT_DEC     dec;
G__CINT_OCT     oct;
G__CINT_SCIENTIFIC  scientific;
G__CINT_FIXED       fixed;
#endif


// global object
#ifdef G__ROOT
Vectorostream plot=Vectorostream(RG_PLOTMODE);
Vectorostream same=Vectorostream(RG_SAMEMODE);
#else
Vectorostream plot=Vectorostream(2);
#endif

#ifdef VIRTUALGRAPH_H
Vectorostream plotNN=Vectorostream(VG_PLOTMODE);
Vectorostream sameNN=Vectorostream(VG_SAMEMODE);
#endif


Matrixostream splot;
//extern Matrixostream splotNN;


