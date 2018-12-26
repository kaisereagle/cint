/*****************************************************************************
 * _rmcrlf.cxx
 *  Description: Remove '^M' at end of each line
 *  Usage:
 *    cint _rmcrlf.cxx [fileExpr] <[fileExpr2] <...>>
 *  Status: Experimental
 *  Date: 2014/Jan/3
 *  Author: Masaharu Goto
 *****************************************************************************/
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
#include <ReadF.h>
#include <EasyExp.h>
#include <string>

bool Rename(const char* fin,const char* fout) {
#ifdef G__WIN32
  string com = "copy ";
#else
  string com = "cp ";
#endif
  com += fin; com+=" ";
  com += fout;
  //printf("%s\n",com.c_str());
  system(com.c_str());
}

bool rmcrlf(const char* fname) {
  printf("%s\n",fname);
  string oname="x_"; oname+=fname;
  FILE *fp = fopen(oname.c_str(),"w");
  if(!fp) return(false);
  ReadF f(fname);
  while(f.read()) {
    fprintf(fp,"%s\n",f.argv[0]);
  }
  fclose(fp);
  string tname="z_"; tname+=fname;
  //Rename(fname,tname.c_str());
  Rename(oname.c_str(),fname);
  remove(oname.c_str());
  return(true);
}

void scanfile(const char* expr) {
  Scandir d(expr);
  char* name;
  while((name=d.Next())&&name[0]) {
    rmcrlf(name);
  }
}

int main(int argc,char* argv[]) {
  G__optimizemode(3);
  for(int i=1;i<argc;i++) {
    //rmcrlf(argv[i]);
    scanfile(argv[i]);
  }
  return(0);
}


