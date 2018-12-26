/*****************************************************************************
 * _extract.h
 *  Description: Extract regular expression matching words from files
 *  Status: Experimental
 *  Date: 2014/Jan/11
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

#ifndef _EXTRACT_H
#define _EXTRACT_H

#ifdef __CINT__
#include <CSV.h>
//#include <NVector.h>
#else
#include "Collection.h"
#endif
#include <unistd.h>
#include <string>

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
map<string,string> words;


/////////////////////////////////////////////////////////////////////////
void extract(EasyExp& we,const char* fname,const char* separator=" \t") {
  ReadF f(fname);
  f.setseparator(separator);
  string fileline;
  char sline[100];
  int i;
  while(f.read()) {
    for(i=1;i<=f.argc;i++) {
      if(we.match(f.argv[i])) {
	fileline = words[f.argv[i]];
        if(fileline.size()) fileline += ",";
	fileline += fname; fileline += ":";
        sprintf(sline,"%d",f.line);
        fileline += sline;
	words[f.argv[i]] = fileline;
	printf("'%s' : %s %d\n",f.argv[i],fname,f.line);
      }
    }
  }
}

/////////////////////////////////////////////////////////////////////////
int main(int argc,char **argv) {
#ifdef __CINT__
  G__optimizemode(3);
#endif

  // parse command line 
  string fileexpr;
  string wordexpr;
  string separator=" \t\r\n";
  for(int i=1;i<argc;i++) {
    if(i==1) wordexpr = argv[i];
    else if(i==2) fileexpr = argv[i];
    else if(i==3) separator = argv[i];
  }
  if(wordexpr=="" || fileexpr=="") {
    printf("Usage: cint _extract.cxx [wordexpr] [fileexpr]\n");
    exit(255);
  }

  EasyExp we;
  we.compile(wordexpr.c_str());

  Scandir d;
  d.scan(fileexpr.c_str());
  char *fname;
  while((fname=d.Next())&&fname[0]) {
    printf("%s\n",fname);
    extract(we,fname,separator.c_str());
  }
  return(0);
}
/////////////////////////////////////////////////////////////////////////

#endif // _EXTRACT_H
