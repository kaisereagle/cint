/////////////////////////////////////////////////////////////////////////////
// VirtualGraph.cxx
//
//  Virtual Graph drawing driver for Neural Network experiment
//
// Date 2014 June 12, Masaharu Goto
/////////////////////////////////////////////////////////////////////////////

#include "VirtualGraphLib.h"

int VG_XSIZE = 40;
int VG_YSIZE = 20;

string vg_opt = "line"; // "dot";
int    vg_width = 1;

VirtualGraph VG_OBJ[VG_N_MAX];
int VG_N =0 ;
