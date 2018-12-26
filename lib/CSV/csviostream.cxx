/////////////////////////////////////////////////////////////////////////
// csviosream.h
//
// Collection 2D/3D drawing
// 
// Sheet s(fname);
// Collection c=s.collection();
// draw << c << endl;
// draw.xl(xmin,xmax).xlog().title(title).xname(xname).yname(yname)<< c << endl;
// // Assuming A and B are in adjacent columns, to draw correlation graph, 
// // use ya(0) method or "YA 0" option.
//   draw.ya(0) << c.column("A","B") << endl;
//   draw       << c.column("A","B") << "YA 0\n";
// // Additional x=y line can be drawn using TF1 class
//   char buf[100];
//   sprintf(buf,"f%d",i+1);
//   TF1 *f1=new TF1(buf,"x",-1e100,1e100);
//   f1->SetLineWidth(1);
//   f1->Draw("Same");
/////////////////////////////////////////////////////////////////////////
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

#include "csviostream.h"

csvostream draw(CSV_DRAWMODE);
csvostream drawNN(CSV_NNMODE);
