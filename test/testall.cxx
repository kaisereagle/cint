#! ../cint
/* /% C++ %/ */
/***********************************************************************
 * cint (C/C++ interpreter)
 ************************************************************************
 * C++ Script testcint.cxx
 ************************************************************************
 * Description:
 *  Automatic test suite of cint
 ************************************************************************
 * Copyright(c) 2002~2004  Masaharu Goto 
 *
 * Permission to use, copy, modify and distribute this software and its 
 * documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation. The author makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 ************************************************************************/
// Usage:
//  $ cint testall.cxx

#ifndef G__VISUAL // ??? fprintf crashes if stdfunc.dll is loaded ???
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif

#ifdef DEBUG2
#ifndef DEBUG
#define DEBUG
#endif
#endif

char* debug;
char* mkcintoption = "";

char* cintoption = "";
//char* cintoption = "-@";

//////////////////////////////////////////////////////////////////////
// run system command
//////////////////////////////////////////////////////////////////////
int clear(const char *fname) {
  FILE *fp = fopen(fname,"w");
  fclose(fp);
}

//////////////////////////////////////////////////////////////////////
// remove a file
//////////////////////////////////////////////////////////////////////
int exist(const char *fname) {
  FILE* fp = fopen(fname,"r");
  if(fp) {
    fclose(fp);
    return(1);
  }
  else {
    return 0;
  }
}

//////////////////////////////////////////////////////////////////////
// remove a file
//////////////////////////////////////////////////////////////////////
int rm(const char *fname) {
#if 1
  int stat;
  do {
    stat = remove(fname);
  } while(exist(fname));
  return(stat);
#else
  return(remove(fname));
#endif
}

//////////////////////////////////////////////////////////////////////
// display file
//////////////////////////////////////////////////////////////////////
int cat(FILE* fout,const char *fname) {
  FILE *fp = fopen(fname,"r");
  char b1[500];
  while(fgets(b1,400,fp)) {
    fprintf(fout,"%s",b1);
  }
  fclose(fp);
}

//////////////////////////////////////////////////////////////////////
// run system command
//////////////////////////////////////////////////////////////////////
int run(const char* com) {
#ifdef DEBUG
  printf("%s\n",com);
#endif
  return(system(com));
}

//////////////////////////////////////////////////////////////////////
// check difference of 2 output files
//////////////////////////////////////////////////////////////////////
int readahead(FILE* fp,const char *b,int ahead=10) {
  int a=0,result=0;
  int i;
  char *c;
  char buf[500];
  fpos_t p;
  fgetpos(fp,&p);
  for(i=0;i<ahead;i++) {
    if(!fp) break;
    c=fgets(buf,400,fp); 
    ++a; 
    if(!c) break;
    if(strcmp(b,buf)==0) {
      result = a;
      break;
    }
  }
  fsetpos(fp,&p);
  return result;
}

void outdiff(FILE *fp,FILE *fpi,int a,char *b,int& l,const char *m) {
  int i;
  char *c;
  //fprintf(fp,"outdiff %d %d\n",a,l);
  for(i=0;i<a;i++) {
    fprintf(fp,"%3d%s %s",l,m,b);
    if(!fpi) break;
    c=fgets(b,400,fpi); 
    ++l; 
    if(!c) break;
  }
}

void checkdiff(FILE* fp,FILE* fp1,FILE* fp2,const char *b1,const char *b2
	       ,int& l1,int& l2,const char *m1,const char *m2) {
  int a1,a2;

  //fprintf(fp,"checkdiff %d %d\n",l1,l2);
  a1 = readahead(fp1,b2);
  a2 = readahead(fp2,b1);

  if(a1&&a2) {
    if(a1<=a2) outdiff(fp,fp1,a1,b1,l1,m1);
    else       outdiff(fp,fp2,a2,b2,l2,m2);
  }
  else if(a1) {
    outdiff(fp,fp1,a1,b1,l1,m1);
  }
  else if(a2) {
    outdiff(fp,fp2,a2,b2,l2,m2);
  }
  else {
    fprintf(fp,"%3d%s %s",l1,m1,b1);
    fprintf(fp,"%3d%s %s",l2,m2,b2);
  }
}

int diff(const char *title,const char *f1,const char *f2,const char *out
	 ,const char *macro="",const char *m1=">",const char *m2="<") {
  FILE *fp = fopen(out,"a");
  FILE *fp1= fopen(f1,"r");
  FILE *fp2= fopen(f2,"r");
  char b1[500];
  char b2[500];
  char *c1;
  char *c2;
  int l1=0;
  int l2=0;

  fprintf(fp,"%s %s\n",title,macro);

  for(;;) {
    if(fp1) { c1=fgets(b1,400,fp1); ++l1; } else c1=0;
    if(fp2) { c2=fgets(b2,400,fp2); ++l2; } else c2=0;
    if(c1&&c2) {
      if(strcmp(b1,b2)) {
#ifndef G__VISUAL
	checkdiff(fp,fp1,fp2,b1,b2,l1,l2,m1,m2);
#else
	fprintf(fp,"%3d%s %s",l1,m1,b1);
	fprintf(fp,"%3d%s %s",l2,m2,b2);
#endif
      }
    }
    else if(c1) {
      fprintf(fp,"%3d%s %s",l1,m1,b1);
    }
    else if(c2) {
      fprintf(fp,"%3d%s %s",l2,m2,b2);
    }
    else {
      break;
    }
  }

  if(fp2) fclose(fp2);
  if(fp1) fclose(fp1);
  if(fp)  fclose(fp);
}


//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// compare compiled and interpreted result
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void ci(const char *compiler,const char *sname,const char *dfile
	,const char *macro="",const char *exsname="",const char *cintopt="") {

  if(debug && strcmp(debug,sname)) return;

  printf("%s %s %s\n",sname,macro,exsname);

  char exename[100];
  strcpy(exename,sname);
  char *p = strrchr(exename,'.');
  strcpy(p,".exe");

  // compile source
  char com[500];
  strcpy(com,G__getmakeinfo(compiler));
  strcat(com," ");
  strcat(com,"-Dcompiled");
  strcat(com," ");
#ifdef G__BORLAND
  strcat(com,macro);
  strcat(com," ");
  strcat(com,sname);
  strcat(com," ");
  strcat(com,exsname);
  strcat(com," -e");
  strcat(com,exename);
#else
  strcat(com,macro);
  strcat(com," ");
  strcat(com,sname);
  strcat(com," ");
  strcat(com,exsname);
  strcat(com," -o ");
  strcat(com,exename);
#endif
  run(com);

  // run compiled program
  sprintf(com,"%s > compiled",exename);
  run(com);
#ifdef DEBUG2
  run(exename);
#endif

#ifndef DEBUG
  rm(exename);
#endif

#if defined(G__WIN32) || defined(G__CYGWIN)
  char *p = strrchr(exename,'.');
  strcpy(p,".obj");
  rm(exename);
#endif
#ifdef G__BORLAND
  char *p = strrchr(exename,'.');
  strcpy(p,".tds");
  rm(exename);
#endif

  // run interpreted program
  sprintf(com,"cint %s -Dinterp %s %s %s %s > interpreted"
	  ,cintoption,cintopt,macro,exsname,sname);
  run(com);

  diff(sname,"compiled","interpreted",dfile,macro,"c","i");

  for(int i=0;i<100000;i++) ; // wait for a while

#ifndef DEBUG
  rm("compiled");
  rm("interpreted");
#endif
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// compare compiled and interpreted result
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void io(const char *sname,const char* old ,const char *dfile
	,const char *macro=""){

  if(debug && strcmp(debug,sname)) return;

  printf("%s\n",sname);

  // run interpreted program
  char com[500];
  sprintf(com,"cint %s %s %s > interpreted",cintoption,macro,sname);
  run(com);

  diff(sname,old,"interpreted",dfile,"","o","i");

  rm("interpreted");
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// make sure that dictionary can be compiled
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void mkc(const char *compiler,const char *sname,const char *dfile
	 ,const char *macro="",const char *src="") {

  if(debug && strcmp(debug,sname)) return;

  printf("%s\n",sname);

  // run interpreted program

  char com[500];
  sprintf(com,"makecint -mk Makefile %s -dl test.dll %s -H %s %s"
	  ,mkcintoption,macro,sname,src);
  run(com);

#ifdef G__VISUAL
  char exename[100];
  strcpy(exename,sname);
  char *p = strrchr(exename,'.');
  strcpy(p,"");
  sprintf(com,"nmake /F Makefile CFG=\"%s - Win32 Release\"",exename);
#else
  sprintf(com,"make -f Makefile");
#endif
  run(com);
#ifdef G__VISUAL
  rename("Release\\test.dll","test.dll");
#endif

  sprintf(com,"-DHNAME=\\\"%s\\\" -DDNAME=\\\"test.dll\\\"",sname);
  ci(compiler,"mkcmain.cxx",dfile,com);

#ifndef DEBUG
#ifdef G__VISUAL
  rename("test.dll","Release\\test.dll");
  run("nmake /F Makefile CFG=\"test - Win32 Release\" clean");
  remove("make.bat");
#else
  run("make -f Makefile clean");
#endif
#endif
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// make sure that dictionary can be compiled and program runs
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void mkco(const char *compiler,const char *sname,const char *hname
	  ,const char *old
	  ,const char *dfile,const char *macro="",const char *src=""
	  ,const char *cintopt=""){

  if(debug && strcmp(debug,sname)) return;

  printf("%s\n",sname);

  // run interpreted program

  char com[500];
  if(strcmp(compiler,"CPP")==0) {
    sprintf(com,"makecint -mk Makefile %s -dl test.dll %s %s -H %s %s"
	    ,mkcintoption,cintopt,macro,hname,src);
  }
  else {
    sprintf(com,"makecint -mk Makefile %s -dl test.dll %s %s -h %s %s"
	    ,mkcintoption,cintopt,macro,hname,src);
  }
  run(com);

#ifdef G__VISUAL
  sprintf(com,"nmake /F Makefile CFG=\"test - Win32 Release\" CLEAN");
  sprintf(com,"nmake /F Makefile CFG=\"test - Win32 Release\"");
#else
  sprintf(com,"make -f Makefile");
#endif
  run(com);
#ifdef G__VISUAL
  rename("Release\\test.dll","test.dll");
#endif

  char imacro[500];
  sprintf(imacro,"%s -Dmakecint",macro);
  io(sname,old,dfile,imacro);

#ifndef DEBUG
#ifdef G__VISUAL
  rename("test.dll","Release\\test.dll");
  run("nmake /F Makefile CFG=\"test - Win32 Release\" clean");
  remove("make.bat");
#else
  run("make -f Makefile clean");
#endif
#endif
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// make sure that dictionary can be compiled and program runs
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void mkci(const char *compiler,const char *sname,const char *hname
	  ,const char *dfile,const char *macro="",const char *src=""
	  ,const char *cintopt=""){

  if(debug && strcmp(debug,sname)) return;

  printf("%s\n",sname);

  // run interpreted program

  char com[500];
  if(strcmp(compiler,"CPP")==0) {
    sprintf(com,"makecint -mk Makefile %s -dl test.dll %s %s -H %s %s"
	    ,mkcintoption,cintopt,macro,hname,src);
  }
  else {
    sprintf(com,"makecint -mk Makefile %s -dl test.dll %s %s -h %s %s"
	    ,mkcintoption,cintopt,macro,hname,src);
  }
  run(com);

#ifdef G__VISUAL
  sprintf(com,"nmake /F Makefile CFG=\"test - Win32 Release\"");
#else
  sprintf(com,"make -f Makefile");
#endif
  run(com);
#ifdef G__VISUAL
  rename("Release\\test.dll","test.dll");
#endif

  char imacro[500];
  sprintf(imacro,"%s -Dmakecint",macro);
  ci(compiler,sname,dfile,imacro,"",cintopt);

#ifndef DEBUG
#ifdef G__VISUAL
  rename("test.dll","Release\\test.dll");
  run("nmake /F Makefile CFG=\"test - Win32 Release\" clean");
  remove("make.bat");
#else
  run("make -f Makefile clean");
#endif
#endif
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// make sure that dictionary can be compiled and program runs
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void mkciN(const char *compiler,const char *sname
	   ,const char *hname1,const char *dfile
	   ,const char *macro=""
	   ,const char *hname2="",const char *hname3=""){

  if(debug && strcmp(debug,sname)) return;

  printf("%s\n",sname);

  // run interpreted program

  char com[500];
  sprintf(com,"makecint -mk Makefile1 %s -dl test1.dll %s -H %s"
	  ,mkcintoption,macro,hname1);
  run(com);

#ifdef G__VISUAL
  sprintf(com,"nmake /F Makefile1 CFG=\"test1 - Win32 Release\"");
#else
  sprintf(com,"make -f Makefile1");
#endif
  run(com);
#ifdef G__VISUAL
  rename("Release\\test1.dll","test1.dll");
#endif

  if(hname2[0]) {
    sprintf(com,"makecint -mk Makefile2 %s -dl test2.dll %s -H %s"
	    ,mkcintoption,macro,hname2);
    run(com);
#ifdef G__VISUAL
    sprintf(com,"nmake /F Makefile2 CFG=\"test2 - Win32 Release\"");
#else
    sprintf(com,"make -f Makefile2");
#endif
    run(com);
#ifdef G__VISUAL
    rename("Release\\test2.dll","test2.dll");
#endif
  }

  if(hname3[0]) {
    sprintf(com,"makecint -mk Makefile3 %s -dl test3.dll %s -H %s"
	    ,mkcintoption,macro,hname3);
    run(com);
#ifdef G__VISUAL
    sprintf(com,"nmake /F Makefile3 CFG=\"test3 - Win32 Release\"");
#else
    sprintf(com,"make -f Makefile3");
#endif
    run(com);
#ifdef G__VISUAL
    rename("Release\\test3.dll","test3.dll");
#endif
  }

  char imacro[500];
  sprintf(imacro,"%s -Dmakecint2",macro);
  ci(compiler,sname,dfile,imacro);

#ifndef DEBUG
#ifdef G__VISUAL
  rename("test1.dll","Release\\test1.dll");
  run("nmake /F Makefile1 CFG=\"test1 - Win32 Release\" clean");
#else
  run("make -f Makefile1 clean");
#endif
  rm("Makefile1");
  if(hname2[0]) {
#ifdef G__VISUAL
  rename("test2.dll","Release\\test2.dll");
  run("nmake /F Makefile2 CFG=\"test2 - Win32 Release\" clean");
#else
    run("make -f Makefile2 clean");
#endif
    rm("Makefile2");
  }
  if(hname3[0]) {
#ifdef G__VISUAL
    rename("test3.dll","Release\\test3.dll");
    run("nmake /F Makefile3 CFG=\"test3 - Win32 Release\" clean");
#else
    run("make -f Makefile3 clean");
#endif
    rm("Makefile3");
  }
#ifdef G__VISUAL
  remove("make.bat");
#endif
#endif
}

//////////////////////////////////////////////////////////////////////
// test series of files with enumerated suffix
//////////////////////////////////////////////////////////////////////
int testn(const char *cpp,const char *hdr,int *num,const char *ext
	  ,const char *dfile,const char *macro="") {


  char sname[100];

  int i=0;
  while(-1!=num[i]) {
    sprintf(sname,"%s%d%s",hdr,num[i],ext);
    ci(cpp,sname,dfile,macro);
    ++i;
  }
}


//////////////////////////////////////////////////////////////////////
// runt
//////////////////////////////////////////////////////////////////////
int runt(const char *dfile) {
  char basename[20];
  for(int i=1;i<1000;i++) {
    sprintf(basename,"t%d",i);
  }
}

// void stopthis(int x=0) { exit(0); } // did not work

//////////////////////////////////////////////////////////////////////
// testsuite main program
//////////////////////////////////////////////////////////////////////
int main(int argc,char** argv) {

  char *difffile="testdiff.txt";

  //signal(SIGINT,stopthis);

  int i;
  for(i=1;i<argc;i++) {
    if(strcmp("-d",argv[i])==0 && !strstr(argv[i+1],".cxx")) 
      difffile=argv[++i];
    else if(strcmp("-c",argv[i])==0 && !strstr(argv[i+1],".cxx"))
      cintoption = argv[++i];
    else if(strcmp("-m",argv[i])==0 && !strstr(argv[i+1],".cxx"))
      mkcintoption = argv[++i];
    else if(strcmp("-?",argv[i])==0) {
      fprintf(stderr,"%s <-d [difffile]> <-c [cintoption]> <-m [makecintoption]> <[testcase.cxx]>\n",argv[0]);
      return 0;
    }
    else debug = argv[i];
  }

  clear(difffile);
  remove("test.dll");
  remove("test1.dll");
  remove("test2.dll");
  remove("test3.dll");
  remove("G__cpp_test.cxx");
  remove("G__cpp_test.h");

#ifndef NEWTEST
  int cpp[] = {0, 1, 2, 3, 4, 5, 6, 8, -1};
  testn("CPP","cpp",cpp,".cxx",difffile);

  ci("CPP","refassign.cxx",difffile);
  ci("CPP","ostream.cxx",difffile);    // cout << pointer
  ci("CPP","setw0.cxx",difffile);      // VC++6.0 setbase()

  int inherit[] = { 0, 1, 2, -1 };
  testn("CPP","inherit",inherit,".cxx",difffile);

  int virtualfunc[] = { 0, 1, 2, -1 };
  testn("CPP","virtualfunc",virtualfunc,".cxx",difffile);

  int oprovld[] = { 0, 2, -1 };
  testn("CPP","oprovld",oprovld,".cxx",difffile);

  ci("CPP","constary.cxx",difffile);
  ci("CPP","const.cxx",difffile);
  ci("CPP","scope0.cxx",difffile);
  ci("CPP","idxscope0.cxx",difffile);
  ci("CPP","access0.cxx",difffile);
  ci("CPP","staticmem0.cxx",difffile);
  ci("CPP","staticmem1.cxx",difffile);
  ci("CPP","staticary.cxx",difffile);
  ci("CPP","minexam.cxx",difffile);
  ci("CPP","btmplt.cxx",difffile);

  int loopcompile[] = { 1, 2, 3, 4, 5, -1 };
  testn("CPP","loopcompile",loopcompile,".cxx",difffile);

  ci("CPP","mfstatic.cxx",difffile);
  ci("CPP","new0.cxx",difffile);

#if defined(G__MSC_VER)&&(G__MSC_VER<=1200)
  int template[] = { 0, 1, 2, 4, 6, -1 };
#else
  int template[] = { 0, 1, 2, 4, 5, 6, -1 };
#endif
  testn("CPP","template",template,".cxx",difffile);
  io("template3.cxx","template3.old",difffile);

  ci("CPP","minherit0.cxx",difffile);
  ci("CPP","enumscope.cxx",difffile);
  ci("CPP","baseconv0.cxx",difffile);
  ci("CPP","friend0.cxx",difffile);
  ci("CPP","anonunion.cxx",difffile);
  ci("CPP","init1.cxx",difffile);
  ci("CPP","init2.cxx",difffile);
  ci("CPP","include.cxx",difffile);
  ci("CPP","eh1.cxx",difffile);
  ci("CPP","ifs.cxx",difffile);
  ci("CPP","bitfield.cxx",difffile);
  ci("CPP","cout1.cxx",difffile);
  ci("CPP","explicitdtor.cxx",difffile);//fails due to base class dtor

  int nick[] = { 3, 4, -1 };
  testn("CPP","nick",nick,".cxx",difffile);

  ci("CPP","nick4.cxx",difffile,"-DDEST");

  int telea[] = { 0,1,2,3,5,6,7, -1 };
  testn("CPP","telea",telea,".cxx",difffile);

  ci("CPP","fwdtmplt.cxx",difffile);
  ci("CPP","VPersonTest.cxx",difffile);
  ci("CPP","convopr0.cxx",difffile);
  ci("CPP","nstmplt1.cxx",difffile);
  ci("CPP","aoki0.cxx",difffile);
  ci("CPP","borg1.cxx",difffile);
  ci("CPP","borg2.cxx",difffile);
  ci("CPP","bruce1.cxx",difffile);
  ci("CPP","fons3.cxx",difffile);
  ci("CPP","Test0.cxx",difffile,"","MyString.cxx");
  ci("CPP","Test1.cxx",difffile,"","Complex.cxx MyString.cxx");
  ci("CPP","delete0.cxx",difffile);
  ci("CPP","pb19.cxx",difffile);

#ifdef AUTOCC
  ci("CPP","autocc.cxx",difffile);
  system("rm G__*");
#endif

  ci("CPP","maincmplx.cxx",difffile,"","complex1.cxx");
  ci("CPP","funcmacro.cxx",difffile); 

  ci("CPP","template.cxx",difffile); 
  mkci("CPP","template.cxx","template.h",difffile);

  ci("CPP","vbase.cxx",difffile); 
  mkci("CPP","vbase.cxx","vbase.h",difffile);

  ci("CPP","vbase1.cxx",difffile); 
  mkci("CPP","vbase1.cxx","vbase1.h",difffile);

  //
  //
  //

  ci("CPP","t215.cxx",difffile); 

  ci("CPP","t358.cxx",difffile); 



  ci("CPP","t488.cxx",difffile); 
  ci("CPP","t516.cxx",difffile); 

  ci("CPP","t603.cxx",difffile);
  ci("CPP","t627.cxx",difffile);
  mkci("CPP","t627.cxx","t627.h",difffile);
  ci("CPP","t630.cxx",difffile);
  ci("CPP","t633.cxx",difffile);
  mkci("CPP","t633.cxx","t633.h",difffile);
  ci("CPP","t634.cxx",difffile);

  ci("CPP","t674.cxx",difffile,"-DINTERPRET"); 

#if !defined(G__WIN32) && !defined(G__CYGWIN) && !defined(G__APPLE)
  ci("CPP","t676.cxx",difffile); //recursive call stack too deep for Visual C++
#endif
  mkci("CPP","t694.cxx","t694.h",difffile);
  ci("CPP","t694.cxx",difffile,"-DINTERPRET"); //fails due to default param
  ci("CPP","t695.cxx",difffile); //fails due to tmplt specialization
  mkci("CPP","t705.cxx","t705.h",difffile);
  ci("CPP","t705.cxx",difffile,"-DINTERPRET");
  ci("CPP","t714.cxx",difffile);
  io("t733.cxx","t733.old",difffile);
#if !defined(G__WIN32) || defined(FORCEWIN32)
  //NOT WORKING: in debug mode on WINDOWS! 
  ci("CPP","t749.cxx",difffile);
#endif
  ci("CPP","t751.cxx",difffile);
  ci("CPP","t764.cxx",difffile);
  ci("CPP","t767.cxx",difffile);
  ci("CPP","t776.cxx",difffile);
  ci("CPP","t777.cxx",difffile);
  ci("CPP","t784.cxx",difffile);
  ci("CPP","t825.cxx",difffile);
  ci("CPP","t910.cxx",difffile);
  ci("CPP","t916.cxx",difffile);
#if !defined(G__VISUAL) || defined(FORCEWIN32)
  io("t927.cxx","t927.old",difffile);
#endif
#if !defined(G__WIN32) || defined(FORCEWIN32)
  mkciN("CPP","t928.cxx","t928.h",difffile,"","t928a.h","t928b.h");
#endif
  ci("CPP","t930.cxx",difffile);
  ci("CPP","t938.cxx",difffile);
  ci("CPP","t958.cxx",difffile);
  ci("CPP","t959.cxx",difffile);
  mkci("CPP","t961.cxx","t961.h",difffile); //mkc("CPP","t961.h",difffile);
                                            //Borland C++5.5 has a problem
                                            //with reverse_iterator::reference
  ci("CPP","t963.cxx",difffile);
#ifdef G__P2F
  mkci("CPP","t966.cxx","t966.h",difffile);
#endif
  mkci("CPP","t968.cxx","t968.h",difffile); // problem with BC++5.5 & VC++6.0
  mkci("CPP","t970.cxx","t970.h",difffile);
  mkciN("CPP","t972.cxx","t972a.h",difffile,"","t972b.h");

#if !defined(G__WIN32) || defined(FORCEWIN32)
  mkci("CPP","t980.cxx","t980.h",difffile);
#endif
#if !defined(G__WIN32) || defined(FORCEWIN32)
  ci("CPP","t986.cxx",difffile,"-DTEST");
#endif
  mkci("CPP","t987.cxx","t987.h",difffile);
  mkciN("CPP","t991.cxx","t991a.h",difffile,"","t991b.h","t991c.h");
  mkci("CPP","t992.cxx","t992.h",difffile);  // problem gcc3.2
  mkci("CC","t993.c","t993.h",difffile); 
  mkci("CPP","t995.cxx","t995.h",difffile); 
  mkci("CPP","t996.cxx","t996.h",difffile); 
  ci("CPP","t998.cxx",difffile); 
  mkci("CPP","t1002.cxx","t1002.h",difffile); 
  ci("CPP","t1004.cxx",difffile); 
  ci("CPP","t1011.cxx",difffile); 
  mkci("CPP","t1011.cxx","t1011.h",difffile); 
  ci("CPP","t1015.cxx",difffile); 
  ci("CPP","t1016.cxx",difffile); 
  mkci("CPP","t1016.cxx","t1016.h",difffile); 
  ci("CPP","t1023.cxx",difffile); 
  ci("CPP","t1024.cxx",difffile); 
  mkci("CPP","t1024.cxx","t1024.h",difffile); 
#if !defined(G__WIN32) || defined(FORCEWIN32)
  mkci("CPP","t1025.cxx","t1025.h",difffile); 
#endif
  ci("CPP","t1026.cxx",difffile); // problem with BC++5.5
  mkci("CPP","t1026.cxx","t1026.h",difffile); 
  io("t1027.cxx","t1027.old",difffile);
  //ci("CPP","t1027.cxx",difffile); // problem with BC++5.5
  //mkci("CPP","t1027.cxx","t1027.h",difffile); 
  ci("CPP","t1032.cxx",difffile); 
  ci("CPP","t1032.cxx",difffile); 
#if !defined(G__WIN32) || defined(FORCEWIN32)
  io("t1034.cxx","t1034.old",difffile); // sizeof(long double)==12
#endif
  ci("CPP","t1035.cxx",difffile);  
  mkci("CPP","t1035.cxx","t1035.h",difffile); 
  ci("CPP","t1036.cxx",difffile);  
  mkci("CPP","t1040.cxx","t1040.h",difffile); // gcc3.2 has problem 
  io("t1042.cxx","t1042.old",difffile);

#if !defined(G__WIN32) || defined(FORCEWIN32)
  ci("CPP","t1046.cxx",difffile); 
  mkci("CPP","t1046.cxx","t1046.h",difffile); 
#endif
  ci("CPP","t1047.cxx",difffile); 
  mkci("CPP","t1047.cxx","t1047.h",difffile); 
  ci("CPP","t1048.cxx",difffile); 
  ci("CPP","t1157.cxx",difffile); 
  ci("CPP","t1158.cxx",difffile); 
  ci("CPP","t1160.cxx",difffile); 
  ci("CPP","aryinit0.cxx",difffile); 
  ci("CPP","aryinit1.cxx",difffile); 
  ci("CPP","t1164.cxx",difffile); 
  ci("CPP","t1165.cxx",difffile); 
  ci("CPP","t1178.cxx",difffile); 
  mkci("CPP","t1187.cxx","t1187.h",difffile); 
  ci("CPP","t1192.cxx",difffile);  
  mkci("CPP","t1193.cxx","t1193.h",difffile); 
  ci("CPP","t1203.cxx",difffile);  
  ci("CPP","t1205.cxx",difffile);  
  mkci("CPP","t1205.cxx","t1205.h",difffile); 
  ci("CPP","t1213.cxx",difffile);  
  ci("CPP","t1214.cxx",difffile);  
  ci("CPP","t1215.cxx",difffile);  
  mkci("CPP","t1215.cxx","t1215.h",difffile); 
  ci("CPP","t1221.cxx",difffile);  
  ci("CPP","t1222.cxx",difffile);  
  ci("CPP","t1223.cxx",difffile);  
  ci("CPP","t1224.cxx",difffile);  
  io("t1228.cxx","t1228.old",difffile);

#if defined(G__CYGWIN)
  // For Cygwin, ERTTI API can not be accessed from precompiled library
#elif defined(G__WIN32) 
  mkci("CPP","t1048.cxx","t1048.h",difffile,"-I%CINTSYSDIR% -I%CINTSYSDIR%\src"); 
#else
  mkci("CPP","t1048.cxx","t1048.h",difffile,"-I$CINTSYSDIR -I$CINTSYSDIR/src");
#endif
  ci("CPP","t1049.cxx",difffile); 
  ci("CPP","t1054.cxx",difffile); 
  ci("CPP","t1055.cxx",difffile); 
  mkci("CPP","t1061.cxx","t1061.h",difffile);
#if !defined(G__WIN32) || defined(FORCEWIN32)
  mkci("CPP","t1062.cxx","t1062.h",difffile); 
#endif
  ci("CPP","t1067.cxx",difffile); 
  mkci("CPP","t1067.cxx","t1067.h",difffile); 
  ci("CPP","t1068.cxx",difffile); 
  mkci("CPP","t1068.cxx","t1068.h",difffile); 
  ci("CPP","t1079.cxx",difffile);
  mkci("CPP","t1079.cxx","t1079.h",difffile); 
  ci("CPP","t1084.cxx",difffile);
  ci("CPP","t1085.cxx",difffile);
  ci("CPP","t1086.cxx",difffile);
  ci("CPP","t1088.cxx",difffile);
  ci("CPP","t1094.cxx",difffile);
  ci("CPP","t1101.cxx",difffile); 
  mkci("CPP","t1115.cxx","t1115.h",difffile); 
  ci("CPP","t1124.cxx",difffile); 
  ci("CPP","t1125.cxx",difffile); 
  ci("CPP","t1126.cxx",difffile); 
  ci("CPP","t1127.cxx",difffile); 
  mkci("CPP","t1127.cxx","t1127.h",difffile);  // 
  ci("CPP","t1128.cxx",difffile);  // looks to me gcc3.2 has a bug
  ci("CPP","t1129.cxx",difffile);  // g++3.2 fails
  ci("CPP","t1134.cxx",difffile);  
  ci("CPP","t1136.cxx",difffile);  
  ci("CPP","t1140.cxx",difffile);  

  ci("CPP","t1144.cxx",difffile);  
  ci("CPP","t1144.cxx",difffile,"","","-Y0");  
  ci("CPP","t1144.cxx",difffile,"","","-Y1");  

  ci("CPP","t1148.cxx",difffile);  

  mkciN("CPP","t1247.cxx","t1247.h",difffile,"","t1247a.h");
  mkci("CPP","t1276.cxx","t1276.h",difffile); 

  mkci("CPP","t1277.cxx","t1277.h",difffile); // works only with gcc2.96

#define PROBLEM
#if defined(PROBLEM) && (!defined(G__WIN32) || defined(FORCEWIN32))
  mkci("CPP","t674.cxx","t674.h",difffile); // Problem with VC++6.0

  ci("CPP","t648.cxx",difffile); // long long has problem with BC++5.5
                                 // also with VC++6.0 bug different

  mkci("CPP","t977.cxx","t977.h",difffile); // VC++ problem is known

  ci("CPP","t980.cxx",difffile); // problem with BC++5.5

#if (G__GNUC==2)
  mkci("CPP","t1030.cxx","t1030.h",difffile); // works only with gcc2.96
  mkci("CPP","t1031.cxx","t1031.h",difffile); // works only with gcc2.96
  //mkci("CPP","t1030.cxx","t1030.h",difffile,"","","-Y0"); 
  //mkci("CPP","t1031.cxx","t1031.h",difffile,"","","-Y0"); 
#endif

#endif

#endif // NEWTEST



  printf("Summary==================================================\n");
  cat(stdout,difffile);
  printf("=========================================================\n");

#ifndef DEBUG
  rm("Makefile");
#endif
}
//////////////////////////////////////////////////////////////////////

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
