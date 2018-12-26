/* /% C %/ */
/***********************************************************************
 * cint (C/C++ interpreter)
 ************************************************************************
 * Source file dump.c
 ************************************************************************
 * Description:
 *  Readline dump
 ************************************************************************
 * Copyright(c) 1995~2003  Masaharu Goto 
 *
 * Permission to use, copy, modify and distribute this software and its 
 * documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  The author makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 ************************************************************************/

#include "common.h"

/****************************************************************
* G__pushdumpinput(fp,exflag)
****************************************************************/
int G__pushdumpinput(fp,exflag)
FILE *fp;
int exflag;
{
  int i;
  for(i=5;i>0;i--) {
    G__dumpreadline[i]=G__dumpreadline[i-1];
    G__Xdumpreadline[i]=G__Xdumpreadline[i-1];
  }
  G__dumpreadline[0]=fp;
  G__Xdumpreadline[0]=exflag;
  return(0);
}

/****************************************************************
* G__popdumpinput()
****************************************************************/
int G__popdumpinput()
{
  int i;
  for(i=0;i<5;i++) {
    G__dumpreadline[i]=G__dumpreadline[i+1];
    G__Xdumpreadline[i]=G__Xdumpreadline[i+1];
  }
  G__dumpreadline[5]=NULL;
  G__Xdumpreadline[5]=0;
  if(G__dumpreadline[0]==NULL) {
    fprintf(G__sout,"All readline dumpfiles have been closed.\n");
    G__Xdumpreadline[0]=0;
  }
  else {
    fprintf(G__sout,"Some more readline dumpfiles remain in stack.\n");
  }
  return(0);
}

/****************************************************************
* G__dumpinput(char *line)
*
*  Write readline string to a dump file.
****************************************************************/
int G__dumpinput(line)
char *line;
{
  if(G__dumpreadline[0]!=NULL) {
    fprintf(G__dumpreadline[0],"%s\n",line);
  }
  return(0);
}

/****************************************************************
* G__xdumpinput(char *line)
*
*  Read readline string from a dump file.
****************************************************************/
char *G__xdumpinput(prompt)
char *prompt;
{
  static char line[G__LONGLINE];
  char *null_fgets;
  int i;
  if(G__dumpreadline[0]!=NULL) {
    null_fgets=fgets(line,G__LONGLINE-1,G__dumpreadline[0]);
    if(null_fgets==NULL) {
      fclose(G__dumpreadline[0]);
      fprintf(G__sout,"End of readline dumpfile. ");
      G__popdumpinput();
      sprintf(line,"P"); 
      return(line);
    }
    for(i=0;i<G__LONGLINE-1;i++) {
      if(line[i]=='\n'||line[i]=='\r') line[i]='\0';
    }
    fprintf(G__sout,"%s%s\n",prompt,line);
  }
  return(line);
}


/*
 * Local Variables:
 * c-tab-always-indent:nil
 * c-indent-level:2
 * c-continued-statement-offset:2
 * c-brace-offset:-2
 * c-brace-imaginary-offset:0
 * c-argdecl-indent:0
 * c-label-offset:-2
 * compile-command:"make -k"
 * End:
 */
