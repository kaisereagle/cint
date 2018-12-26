/*****************************************************************************
 * _copy.cxx
 *  Description: remove files
 *    cint _copy.cxx <option> [fileExpr] <[fileExpr2] <...>>
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
//////////////////////////////////////////////////////////////////////
// _copy.cxx
//////////////////////////////////////////////////////////////////////
//#include <EasyExp.dll>
#include <CSV.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc,char** argv) {
  int fail=0;
  int caseindependent=0;
  int isdirectory=2;
  int debug=0;
  Scandir d;
  for(int i=1;i<argc;i++) {
    if(argv[i][0]=='-') {
      switch(tolower(argv[i][1])) {
      case 'g': debug=1; break; 
      case 'f': isdirectory=0; break; // file only
      case 'd': isdirectory=1; break; // directory only
      case 'b': isdirectory=2; break; // both
      case 'c': caseindependent=0; break; // case sensitive
      case 'i': caseindependent=1; break; // case independent
      default: 
        fprintf(stderr,"Unknown option %s\n",argv[i]);
        fprintf(stderr,"Usage:\n");
        fprintf(stderr," _copy.cxx <-f|-d|-b> <-c|-i> pathdescription outdir\n");
        fprintf(stderr,"    -f : list file only\n");
        fprintf(stderr,"    -d : list directory only\n");
        fprintf(stderr,"    -b : list file and directory (default)\n");
        fprintf(stderr,"    -c : case sensitive\n");
        fprintf(stderr,"    -i : case insensitive (default)\n");
        fprintf(stderr,"    -g : debug mode\n");
        exit(255);
      }
    }
    else {
      d.scan(argv[i],isdirectory,caseindependent);
      char* fname;
      string odir=argv[++i], ofname,tmp;
      printf("copy ");
      while((fname=d.Next())&&fname[0]) {
	ofname = fname;
	size_t pos=ofname.find_last_of("/\\");
	if(pos!=string::npos) 
	  ofname = odir + string("/")+ ofname.substr(pos+1);
	else 
	  ofname = odir + string("/")+ ofname;
	printf("'%s'   '%s'\n",fname,ofname.c_str());
	if(!debug) {
	  fail += copy(fname,ofname.c_str());
	}
      }
      printf("\n");
    }
  }

  if(debug) printf("debug mode --- above commands are not executed");
  else if(fail) printf("Fail %d\n",fail);
  else     printf("Success\n");

  return(0);
}

int copy(const char* orig,const char* target) {
  FILE* fporig  = fopen(orig,"rb");
  if(!fporig) {
    fprintf(stderr,"Failed opening %s\n",orig);
    return(1);
  }
  FILE* fptarget= fopen(target,"wb");
  if(!fptarget) {
    fclose(fporig);
    fprintf(stderr,"Failed creating %s\n",target);
    return(1);
  }
  int c;
  while((c=fgetc(fporig))!=EOF) fputc(c,fptarget);

  fclose(fporig);
  fclose(fptarget);
  return(0);
}
