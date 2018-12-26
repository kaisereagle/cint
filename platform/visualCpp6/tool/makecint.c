/* /% C %/ */
/***********************************************************************
 * makecint (C/C++ interpreter-compiler)
 ************************************************************************
 * Source file platform/visual/tool/makecint.c
 ************************************************************************
 * Description:
 *  This tool creates Makefile for encapsurating arbitrary C/C++ object
 * into Cint as Dynamic Link Library for Windows-NT Visual C++ 4.0.
 ************************************************************************
 * Copyright(c) 1995~2004  Masaharu Goto (MXJ02154@niftyserve.or.jp)
 *
 * Permission to use, copy, modify and distribute this software and its 
 * documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  The author makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define G__WIN32
#include <windows.h>
char *G__psep = "\\";

#ifndef EXIT_FAILURE
#define EXIT_FAILURE 1
#endif
#ifndef EXIT_SUCCESS
#define EXIT_SUCCESS 0
#endif


#define G__MAXFILENAME  512
#define G__LONGLINE     512
#define G__MAXLINE      512
#define G__MAXARG       100
#define G__MAXNAME      100

char G__cintsysdir[G__MAXFILENAME] ;

char G__CSRCPOST[20];
char G__CHDRPOST[20];
char G__CPPSRCPOST[20];
char G__CPPHDRPOST[20];
char G__DLLPOST[20];
char G__OBJPOST[20];

int G__isDLL=1;
char G__object[G__MAXFILENAME];
char G__makefile[G__MAXFILENAME];
char G__DLLID[G__MAXNAME];
char G__INITFUNC[G__LONGLINE];
char G__CINTLIBNAME[G__MAXFILENAME];
char G__LIBNAME[G__MAXFILENAME];

struct G__string_list {
  char *string;
  char *object;
  char *misc;
  struct G__string_list *next;
};

struct G__string_list *G__MACRO;
struct G__string_list *G__IPATH;
struct G__string_list *G__CHDR;
struct G__string_list *G__CPPHDR;
struct G__string_list *G__CSRC;
struct G__string_list *G__CPPSRC;
struct G__string_list *G__LIB;
#ifndef G__OLDIMPLEMENTATION783
struct G__string_list *G__CCOPT;
struct G__string_list *G__CIOPT;
#endif
struct G__string_list *G__CSTUB;
struct G__string_list *G__CPPSTUB;

char G__preprocess[10];
int G__ismain=0;

enum G__MODE { G__IDLE, G__CHEADER, G__CSOURCE, G__CPPHEADER, G__CPPSOURCE
	     , G__LIBRARY , G__CSTUBFILE , G__CPPSTUBFILE
#ifndef G__OLDIMPLEMENTATION783
	     , G__COMPILEROPT, G__CINTOPT
#endif
};

enum G__PRINTMODE { G__PRINTOBJECT, G__PRINTSTRING, G__PRINTOBJECT_WPLUS
		    , G__PRINTOBJECT_WSRC };

/****************************************************************
* G__printtitle()
****************************************************************/
void G__printtitle()
{
  printf("################################################################\n");
  printf("# makecint : interpreter-compiler for cint (Visual C++ version)\n");
  printf("#\n");
  printf("# Copyright(c) 1995~2004 Masaharu Goto (MXJ02154@niftyserve.or.jp)\n");
  printf("#                        (cint mailing list 'cint@root.cern.ch')\n");
  printf("################################################################\n");
}

/****************************************************************
* G__displayhelp()
****************************************************************/
void G__displayhelp()
{
  printf("Usage :\n");
  printf(" makecint -mk [Makefile] -o [Object] -H [C++header] -C++ [C++source]\n");
  printf("          <-m> <-p>      -dl [DLL]   -h [Cheader]   -C   [Csource]\n");
  printf("                          -l [Lib] -i [StubC] -i++ [StubC++]\n");
  printf("  -o [obj]      :Object name\n");
  printf("  -dl [dynlib]  :Generate dynamic link library object\n");
  printf("  -mk [mkfile]  :Create makefile (no actual compilation)\n");
  printf("  -p            :Use preprocessor for header files\n");
  printf("  -m            :Needed if main() is included in the source file\n");
  printf("  -D [macro]    :Define macro\n");
  printf("  -I [incldpath]:Set Include file search path\n");
  printf("  -H [sut].h    :C++ header as parameter information file\n");
  printf("  -h [sut].h    :C header as parameter information file\n");
  printf("    +P          :Turn on preprocessor mode for following header files\n");
  printf("    -P          :Turn off preprocessor mode for following header files\n");
  printf("    +V          :Turn on class title loading for following header files\n");
  printf("    -V          :Turn off class title loading for following header files\n");
  printf("  -C++ [sut].C  :Link C++ object. Not accessed unless -H [sut].h is given\n");
  printf("  -C [sut].c    :Link C object. Not accessed unless -h [sut].h is given\n");
  printf("  -i++ [stub].h :C++ STUB function parameter information file\n");
  printf("  -i [stub].h   :C STUB function parameter information file\n");
  printf("  -c [sut].c    :Same as '-h [sut].c -C [sut].c'\n");
  printf("  -l -l[lib]    :Compiled object, Library or linker options\n");
#ifndef G__OLDIMPLEMENTATION1452
  printf("  -u [file]     :Generate dummy class for undefined typename\n");
  printf("  -U [dir]      :Directory to disable interface method generation\n");
  printf("  -Y [0|1]      :Ignore std namespace (default=1:ignore)\n");
  printf("  -Z [0|1]      :Automatic loading of standard header files\n");
#endif
#ifndef G__OLDIMPLEMENTATION783
  printf("  -cc   [opt]   :Compiler option\n");
  printf("  -cint [opt]   :Cint option\n");
#endif
  printf("  -B [funcname] :Initialization function name\n");
  printf("  -y [LIBNAME]  :Name of CINT core DLL, LIBCINT or WILDC(WinNT/95 only)\n");
}

/****************************************************************
* G__displaytodo()
****************************************************************/
void G__displaytodo()
{
  printf("%s is created. Makecint success.\n",G__makefile);
  printf("Do 'make -f %s' to compile the object\n",G__makefile);
}

/****************************************************************
* G__split(original,stringbuf,argc,argv)
* split arguments separated by space char.
* CAUTION: input string will be modified. If you want to keep
*         the original string, you should copy it to another string.
****************************************************************/
int G__split(line,string,argc,argv)
char *string,*line;
char *argv[];
int *argc;
{
  int lenstring;
  int i=0;
  int flag=0;
  int n_eof=1;
  int single_quote=0,double_quote=0,back_slash=0;

  while((string[i]!='\n')&&
	(string[i]!='\0')&&
	(string[i]!=EOF)) i++;
  string[i]='\0';
  line[i]='\0';
  lenstring=i;
  if(string[i]==EOF) n_eof=0;
  argv[0]=line;

  *argc=0;
  for(i=0;i<lenstring;i++) {
    switch(string[i]) {
    case '\\':
      if(back_slash==0) back_slash=1;
      else              back_slash=0;
      break;
    case '\'':
      if((double_quote==0)&&(back_slash==0)) {
	single_quote ^= 1;
	string[i]='\0';
	flag=0;
      }
      break;
    case '"' :
      if((single_quote==0)&&(back_slash==0)) {
	double_quote ^= 1;
	string[i]='\0';
	flag=0;
      }
      break;
    default  :
      if((isspace(string[i]))&&(back_slash==0)&&
	 (single_quote==0)&&(double_quote==0)) {
	string[i]='\0';
	flag=0;
      }
      else {
	if(flag==0) {
	  (*argc)++;
	  argv[*argc] = &string[i];
	  flag=1;
	}
      }
      back_slash=0;
      break;
    }
  }
  return(n_eof);
}

/****************************************************************
* G__readline(fp,line,argbuf,argn,arg)
****************************************************************/
int G__readline(fp,line,argbuf,argn,arg)
FILE *fp;
int *argn;
char *line,*argbuf;
char *arg[];
{
  /* int i; */
  char *null_fgets;
  null_fgets=fgets(line,G__LONGLINE*2,fp);
  if(null_fgets!=NULL) {
    strcpy(argbuf,line);
    G__split(line,argbuf,argn,arg);
  }
  else {
    line[0]='\0';;
    argbuf='\0';
    *argn=0;
    arg[0]=line;
  }
  if(null_fgets==NULL) return(0);
  else                 return(1);
}

/**************************************************************************
* G__copyfile()
**************************************************************************/
char *G__strrstr(string1,string2)
char *string1,*string2;
{
  char *p=NULL,*s,*result=NULL;
  s=string1;
  while(p=strstr(s,string2)) {
    result=p;
    s=p+1;
  }
  return(result);
}


/**************************************************************************
* G__copyfile()
**************************************************************************/
int G__copyfile(to,from)
FILE *to,*from;
{
  int c=0;
  while(EOF != (c=fgetc(from))) {
    fputc(c,to);
  }
}


/******************************************************************
* G__getcintsysdir()
*
*  print out error message for unsupported capability.
******************************************************************/
int G__getcintsysdir()
{
  char *env;
  if('\0'==G__cintsysdir[0]) {
    env=getenv("CINTSYSDIR");
    if(env) {
      strcpy(G__cintsysdir,env);
      return(0);
    }
    else {
#ifdef G__WIN32
      HMODULE hmodule;
      if(GetModuleFileName(hmodule,G__cintsysdir,G__MAXFILENAME)) {
        char *p = G__strrstr(G__cintsysdir,G__psep);
        if(p) *p = 0;
	return(EXIT_SUCCESS);
      }
#endif
      fprintf(stderr,"Error: environment variable CINTSYSDIR is not set. makecint aborted.\n");
      G__cintsysdir[0]='\0';
      return(1);
    }
  }
  return(0);
}


/******************************************************************
* G__readMAKEINFO
******************************************************************/
void G__readMAKEINFO()
{
  char makeinfo[G__MAXFILENAME];
  FILE *fp;
  char line[G__MAXLINE];
  char argbuf[G__MAXLINE];
  char *arg[G__MAXARG];
  int argn;

  /* Get $CINTSYSDIR/MAKEINFO file name */
  if(G__getcintsysdir()) exit(EXIT_FAILURE);
  sprintf(makeinfo,"%s/MAKEINFO",G__cintsysdir);

  /* Open MAKEINFO file */
  fp = fopen(makeinfo,"r");
  if(!fp) {
    fprintf(stderr,"Error: can not open %s. Makecint aborted\n",makeinfo);
    fprintf(stderr
,"!!!Advice. There are examples of MAKEINFO files under %s/platform/ directory.\n"
	    ,G__cintsysdir);
    fprintf(stderr
	    ,"Please refer to these examples and create for your platform\n");
    exit(EXIT_FAILURE);
  }

  /* Read the MAKEINFO file */
  while(G__readline(fp,line,argbuf,&argn,arg)) {
    if(argn>2 && strcmp(arg[1],"CSRCPOST")==0) strcpy(G__CSRCPOST,arg[3]);
    else if(argn>2 && strcmp(arg[1],"CHDRPOST")==0) strcpy(G__CHDRPOST,arg[3]);
    else if(argn>2 && strcmp(arg[1],"CPPHDRPOST")==0)
      strcpy(G__CPPHDRPOST,arg[3]);
    else if(argn>2 && strcmp(arg[1],"CPPSRCPOST")==0)
      strcpy(G__CPPSRCPOST,arg[3]);
    else if(argn>2 && strcmp(arg[1],"OBJPOST")==0) strcpy(G__OBJPOST,arg[3]);
    else if(argn>2 && strcmp(arg[1],"DLLPOST")==0) strcpy(G__DLLPOST,arg[3]);
  }
  fclose(fp);
}

/******************************************************************
* G__storestringlist
******************************************************************/
struct G__string_list* G__storestringlist(list,string)
struct G__string_list *list;
char *string;
{
  struct G__string_list *p;
  if(!list) {
    p = (struct G__string_list*)malloc(sizeof(struct G__string_list));
    p->string = (char*)malloc(strlen(string)+1);
    strcpy(p->string,string);
    p->object = (char*)NULL;
    p->misc = (char*)NULL;
    p->next = (struct G__string_list*)NULL;
    return(p);
  }
  else {
    p = list;
    while(p->next) p=p->next;
    p->next = (struct G__string_list*)malloc(sizeof(struct G__string_list));
    p = p->next;
    p->string = (char*)malloc(strlen(string)+1);
    strcpy(p->string,string);
    p->object = (char*)NULL;
    p->misc = (char*)NULL;
    p->next = (struct G__string_list*)NULL;
    return(list);
  }
}

/******************************************************************
* G__freestringlist
******************************************************************/
void G__freestringlist(list)
struct G__string_list *list;
{
  struct G__string_list *p;
  p = list;
  if(p) {
    if(p->string) free((void*)p->string);
    if(p->object) free((void*)p->object);
    if(p->misc) free((void*)p->misc);
    if(p->next) G__freestringlist(p->next);
    free((void*)p);
  }
}


/******************************************************************
* G__cleanup()
******************************************************************/
void G__cleanup()
{
  if(G__MACRO) G__freestringlist(G__MACRO);
  if(G__IPATH) G__freestringlist(G__IPATH);
  if(G__CHDR)  G__freestringlist(G__CHDR);
  if(G__CPPHDR) G__freestringlist(G__CPPHDR);
  if(G__CSRC)   G__freestringlist(G__CSRC);
  if(G__CPPSRC) G__freestringlist(G__CPPSRC);
  if(G__LIB)    G__freestringlist(G__LIB);
#ifndef G__OLDIMPLEMENTATION783
  if(G__CCOPT)    G__freestringlist(G__CCOPT);
  if(G__CIOPT)    G__freestringlist(G__CIOPT);
#endif
  if(G__CSTUB)   G__freestringlist(G__CSTUB);
  if(G__CPPSTUB)   G__freestringlist(G__CPPSTUB);
}

/******************************************************************
* G__ispostfix()
*  test if the string has specific postfix
*****************************************************************/
int G__ispostfix(string,postfix)
char *string;
char *postfix;
{
  int lenpost;
  int len;
  lenpost = strlen(postfix);
  len = strlen(string);

  if(len>=lenpost && strcmp(string+len-lenpost,postfix)==0) return(1);
  else return(0);
}

/******************************************************************
* G__replacepostfix()
*  test if the string has specific postfix
*****************************************************************/
char *G__replacepostfix(string,postfix,buf)
char *string;
char *postfix;
char *buf;
{
  int lenpost;
  int len;
  char *postpos;
  lenpost = strlen(postfix);
  len = strlen(string);

  if(len>=lenpost && strcmp(string+len-lenpost,postfix)==0) {
    strcpy(buf,string);
    postpos = G__strrstr(buf,postfix);
    return(postpos);
  }
  else return((char*)NULL);
}

/******************************************************************
* G__checksourcefiles()
*
* check string list and identify it the item is
*   1) source file      string="xxx.C" object="xxx.o"  misc=NULL
*   2) object file      string=NULL    object="xxx.o"  misc=NULL
*   3) others(misc)     string=NULL    object=NULL     misc="xxx"
******************************************************************/
void G__checksourcefiles(list,srcpost,objpost)
struct G__string_list *list;
char *srcpost;
char *objpost;
{
  char buf[G__MAXFILENAME];
  struct G__string_list *p;
  char *postfix;

  p = list;
  while(p) {
    /* assignment intended in following if statement */
    if((postfix=G__replacepostfix(p->string,srcpost,buf))||
       (postfix=G__replacepostfix(p->string,".c",buf))||
       (postfix=G__replacepostfix(p->string,".C",buf))||
       (postfix=G__replacepostfix(p->string,".cc",buf))||
       (postfix=G__replacepostfix(p->string,".CC",buf))||
       (postfix=G__replacepostfix(p->string,".cxx",buf))||
       (postfix=G__replacepostfix(p->string,".CXX",buf))||
       (postfix=G__replacepostfix(p->string,".cpp",buf))||
       (postfix=G__replacepostfix(p->string,".CPP",buf))) {
      strcpy(postfix,objpost);
      p->object = (char*)malloc(strlen(buf)+1);
      strcpy(p->object,buf);
    }
    else if(G__ispostfix(p->string,objpost)) {
      p->object = p->string;
      p->string = (char*)NULL;
    }
    else {
      fprintf(stderr
	      ,"makecint: Warning unrecognized name %s given as source file\n"
	      ,p->string);
      p->misc = p->string;
      p->string = (char*)NULL;
    }
    p = p->next;
  }
}

/******************************************************************
* G__printstringlist()
******************************************************************/
void G__printstringlist(fp,list,mode)
FILE *fp;
struct G__string_list *list;
int mode;
{
  struct G__string_list *p;

  p = list;
  while(p) {
    switch(mode) {
    case G__PRINTSTRING:
      if(p->string) fprintf(fp,"\\\n\t\t%s ",p->string);
      else if(p->misc)   fprintf(fp,"\\\n\t\t%s ",p->misc);
      break;
    case G__PRINTOBJECT:
      if(p->object) fprintf(fp,"\\\n\t\t%s ",p->object);
      break;
    case G__PRINTOBJECT_WPLUS:
      if(p->object) fprintf(fp,"+\n%s",p->object);
      break;
    case G__PRINTOBJECT_WSRC:
      if(p->object && p->string) fprintf(fp,"\\\n\t\t%s ",p->object);
      break;
    default:
      fprintf(stderr,"makecint internal error G__printstringlist()\n");
      break;
    }
    p = p->next;
  }
}

/******************************************************************
* G__printstringlist_vc()
******************************************************************/
void G__printstringlist_vc(fp,list,opt)
FILE *fp;
struct G__string_list *list;
char *opt;
{
  struct G__string_list *p;

  p = list;
  while(p) {
    if(p->string) {
      fprintf(fp,"%s \"%s\" ",opt,p->string+2);
    }
    p = p->next;
  }
}

/******************************************************************
* G__printstringlist_noopt()
******************************************************************/
void G__printstringlist_noopt(fp,list,mode)
FILE *fp;
struct G__string_list *list;
int mode;
{
  struct G__string_list *p;

  p = list;
  while(p) {
    switch(p->string[0]) {
    case '+':
    case '-':
      /* +P,-P,+V,-V options are ignored */
      break;
    default:
      switch(mode) {
      case G__PRINTSTRING:
	if(p->string) fprintf(fp,"\\\n\t\t%s ",p->string);
	else if(p->misc)   fprintf(fp,"\\\n\t\t%s ",p->misc);
	break;
      case G__PRINTOBJECT:
	if(p->object) fprintf(fp,"\\\n\t\t%s ",p->object);
	break;
      default:
	fprintf(stderr,"makecint internal error G__printstringlist()\n");
	break;
      }
      break;
    }
    p = p->next;
  }
}


/******************************************************************
* G__printstringlist()
******************************************************************/
void G__printsourcecompile(fp,list,headers,compiler)
FILE *fp;
struct G__string_list *list;
char *headers;
char *compiler;
{
  struct G__string_list *p;

  p = list;
  while(p) {
    if(p->string && p->object) {
      fprintf(fp,"%s : %s %s\n",p->object,p->string,headers);
      fprintf(fp,"\t%s $(IPATH) $(MACRO) $(OPTIMIZE) $(OPTION) -o %s -c %s\n"
	      ,compiler,p->object,p->string);
      fprintf(fp,"\n");
    }
    p = p->next;
  }
}



/******************************************************************
* G__readargument
******************************************************************/
int G__readargument(argc,argv)
int argc;
char **argv;
{
  char *p;
  char buf[G__MAXFILENAME];
  int mode=G__IDLE;
  int i=1;
  while(i<argc) {
    /*************************************************************************
    * options with no argument
    *************************************************************************/
    if(strcmp(argv[i],"-p")==0) {
      strcpy(G__preprocess,argv[i]);
      mode = G__IDLE;
    }
#ifndef G__OLDIMPLEMENTATION1452
    /*************************************************************************/
    else if(strcmp(argv[i],"-u")==0) {
      G__CIOPT=G__storestringlist(G__CIOPT,argv[i]);
      G__CIOPT=G__storestringlist(G__CIOPT,argv[++i]);
      mode = G__IDLE;
    }
    else if(strncmp(argv[i],"-u",2)==0) {
      G__CIOPT=G__storestringlist(G__CIOPT,argv[i]);
      mode = G__IDLE;
    }
    /*************************************************************************/
    else if(strcmp(argv[i],"-U")==0) {
      G__CIOPT=G__storestringlist(G__CIOPT,argv[i]);
      G__CIOPT=G__storestringlist(G__CIOPT,argv[++i]);
      mode = G__IDLE;
    }
    else if(strncmp(argv[i],"-U",2)==0) {
      G__CIOPT=G__storestringlist(G__CIOPT,argv[i]);
      mode = G__IDLE;
    }
    /*************************************************************************/
    else if(strcmp(argv[i],"-Y")==0) {
      G__CIOPT=G__storestringlist(G__CIOPT,argv[i]);
      G__CIOPT=G__storestringlist(G__CIOPT,argv[++i]);
      mode = G__IDLE;
    }
    else if(strncmp(argv[i],"-Y",2)==0) {
      G__CIOPT=G__storestringlist(G__CIOPT,argv[i]);
      mode = G__IDLE;
    }
    /*************************************************************************/
    else if(strcmp(argv[i],"-Z")==0) {
      G__CIOPT=G__storestringlist(G__CIOPT,argv[i]);
      G__CIOPT=G__storestringlist(G__CIOPT,argv[++i]);
      mode = G__IDLE;
    }
    else if(strncmp(argv[i],"-Z",2)==0) {
      G__CIOPT=G__storestringlist(G__CIOPT,argv[i]);
      mode = G__IDLE;
    }
#endif
    /*************************************************************************/
    else if(strcmp(argv[i],"-m")==0) {
      G__ismain = 1;
      mode = G__IDLE;
    }
    /*************************************************************************/
    else if(strcmp(argv[i],"-?")==0) {
      G__displayhelp();
      mode = G__IDLE;
      exit(EXIT_SUCCESS);
    }
    /*************************************************************************
    * options with 1 argument
    *************************************************************************/
    else if(strcmp(argv[i],"-D")==0) {
      sprintf(buf,"%s%s",argv[i],argv[i+1]);
      G__MACRO = G__storestringlist(G__MACRO,buf);
      i++;
      mode = G__IDLE;
    }
    else if(strncmp(argv[i],"-D",2)==0
#ifndef G__OLDIMPLEMENTATION783
	    && G__COMPILEROPT!=mode && G__CINTOPT!=mode
#endif
	    ) {
      G__MACRO = G__storestringlist(G__MACRO,argv[i]);
      mode = G__IDLE;
    }
    /*************************************************************************/
    else if(strcmp(argv[i],"-I")==0) {
      sprintf(buf,"%s%s",argv[i],argv[i+1]);
      G__IPATH = G__storestringlist(G__IPATH,buf);
      i++;
      mode = G__IDLE;
    }
    else if(strncmp(argv[i],"-I",2)==0 
#ifndef G__OLDIMPLEMENTATION783
	    && G__COMPILEROPT!=mode && G__CINTOPT!=mode
#endif
	    ) {
      G__IPATH = G__storestringlist(G__IPATH,argv[i]);
      mode = G__IDLE;
    }
    /*************************************************************************/
    else if(strcmp(argv[i],"-B")==0) {
      ++i;
      sprintf(G__INITFUNC,"-B%s",argv[i]);
    }
    else if(strcmp(argv[i],"-y")==0) {
      ++i;
      sprintf(G__CINTLIBNAME,"-y%s",argv[i]);
	  if(strcmp("WILDC",argv[i])==0) strcpy(G__LIBNAME,"\\WILDC\\BIN\\WILDC");
	  else if(strcmp("LIBCINT",argv[i])==0) strcpy(G__LIBNAME,"\\CINT\\LIBCINT");
	  else strcpy(G__LIBNAME,argv[i]);
    }
    /*************************************************************************/
    else if(strcmp(argv[i],"-o")==0) {
      i++;
      strcpy(G__object,argv[i]);
      p = strrchr(G__object,'/');
      if(!p) p = G__object;
      else p++;
      strcpy(G__DLLID,p);
      p = strchr(G__DLLID,'.');
      if(p) *p = '\0';
      G__isDLL = 0;
      mode = G__IDLE;
    }
    /*************************************************************************/
    else if(strcmp(argv[i],"-dl")==0 || strcmp(argv[i],"-sl")==0 ||
	    strcmp(argv[i],"-dll")==0 || strcmp(argv[i],"-DLL")==0 ||
	    strcmp(argv[i],"-so")==0) {
      i++;
      strcpy(G__object,argv[i]);
      p = strrchr(G__object,'/');
      if(!p) p = G__object;
      else p++;
      strcpy(G__DLLID,p);
      p = strchr(G__DLLID,'.');
      if(p) *p = '\0';
      G__isDLL = 1;
      mode = G__IDLE;
    }
    /*************************************************************************/
    else if(strcmp(argv[i],"-mk")==0) {
      i++;
      strcpy(G__makefile,argv[i]);
      mode = G__IDLE;
    }
    /*************************************************************************
    * options with multiple argument
    *************************************************************************/
    else if(strcmp(argv[i],"-h")==0) {
      mode = G__CHEADER;
    }
    else if(strcmp(argv[i],"-H")==0) {
      mode = G__CPPHEADER;
    }
    else if(strcmp(argv[i],"-C")==0) {
      mode = G__CSOURCE;
    }
    else if(strcmp(argv[i],"-C++")==0) {
      mode = G__CPPSOURCE;
    }
    else if(strcmp(argv[i],"-l")==0) {
      mode = G__LIBRARY;
    }
#ifndef G__OLDIMPLEMENTATION783
    else if(strcmp(argv[i],"-cc")==0) {
      mode = G__COMPILEROPT;
    }
    else if(strcmp(argv[i],"-cint")==0) {
      mode = G__CINTOPT;
    }
#endif
    else if(strcmp(argv[i],"-i")==0) {
      mode = G__CSTUBFILE;
    }
    else if(strcmp(argv[i],"-i++")==0) {
      mode = G__CPPSTUBFILE;
    }
    else if(strcmp(argv[i],"-c")==0) {
      /* fprintf(stderr,"makecint: -c being obsoleted. no guarantee\n"); */
      mode = G__CHEADER;
    }
    /*************************************************************************/
    else {
      switch(mode) {
      case G__CHEADER:
	G__CHDR=G__storestringlist(G__CHDR,argv[i]);
	break;
      case G__CSOURCE:
	G__CSRC=G__storestringlist(G__CSRC,argv[i]);
	break;
      case G__CPPHEADER:
	G__CPPHDR=G__storestringlist(G__CPPHDR,argv[i]);
	break;
      case G__CPPSOURCE:
	G__CPPSRC=G__storestringlist(G__CPPSRC,argv[i]);
	break;
      case G__LIBRARY:
	G__LIB=G__storestringlist(G__LIB,argv[i]);
	break;
#ifndef G__OLDIMPLEMENTATION783
      case G__COMPILEROPT:
	G__CCOPT=G__storestringlist(G__CCOPT,argv[i]);
	break;
      case G__CINTOPT:
	G__CIOPT=G__storestringlist(G__CIOPT,argv[i]);
	break;
#endif
      case G__CSTUBFILE:
	G__CSTUB=G__storestringlist(G__CSTUB,argv[i]);
	break;
      case G__CPPSTUBFILE:
	G__CPPSTUB=G__storestringlist(G__CPPSTUB,argv[i]);
	break;
      case G__IDLE:
      default:
	break;
      }
    }
    ++i;
  }
}

/******************************************************************
* G__check
******************************************************************/
int G__check(buf,item,where)
char *buf;
char *item;
char *where;
{
  if((!buf) || '\0'==buf[0]) {
    fprintf(stderr,"Error: %s must be set %s\n",item,where);
    return(1);
  }
  return(0);
}

/******************************************************************
* G__checksetup
******************************************************************/
int G__checksetup()
{
  int error=0;
  if(G__isDLL) {
    error+=G__check(G__object,"'-dl [DLL]'","in the command line");
    error+=G__check(G__DLLPOST,"DLLPOST","in the $(CINTSYSDIR)/MAKEINFO file");
  }
  else {
    error+=G__check(G__object,"'-o [Object]'","in the command line");
  }
  error+=G__check(G__makefile,"'-mk [Makefile]'","in the command line");
  error+=G__check(G__OBJPOST,"OBJPOST","in the $(CINTSYSDIR)/MAKEINFO file");
  if(G__CHDR) {
    error+=G__check(G__CHDRPOST,"CSRCPOST"
		    ,"in the $(CINTSYSDIR)/MAKEINFO file");
  }
  if(G__CPPHDR) {
    error+=G__check(G__CHDRPOST,"CPPSRCPOST"
		    ,"in the $(CINTSYSDIR)/MAKEINFO file");
  }
  return(error);
}


/* Visual C++ specific */
/**************************************************************************
* G__ouputcintspecific()
*
**************************************************************************/
G__outputcintspecific(fp)
FILE *fp;
{
 fprintf(fp,"################################################################################\n");
 fprintf(fp,"# CINT SPECIFIC\n");
 fprintf(fp,"IPATH      = ");
 G__printstringlist_vc(fp,G__IPATH,"/I");
 fprintf(fp,"\n");
 fprintf(fp,"IPATHCINT      = ");
 G__printstringlist(fp,G__IPATH,G__PRINTSTRING);
 fprintf(fp,"\n");
 
 fprintf(fp,"MACRO      = ");
 G__printstringlist_vc(fp,G__MACRO,"/D");
#ifndef G__OLDIMPLEMENTATION783
 G__printstringlist(fp,G__CCOPT,G__PRINTSTRING);
#endif
 fprintf(fp,"\n");
 fprintf(fp,"MACROCINT      = ");
 G__printstringlist(fp,G__MACRO,G__PRINTSTRING);
#ifndef G__OLDIMPLEMENTATION783
 G__printstringlist(fp,G__CIOPT,G__PRINTSTRING);
#endif
 fprintf(fp,"\n");

 fprintf(fp,"CHEADER    = ");
 G__printstringlist_noopt(fp,G__CHDR,G__PRINTSTRING);
 fprintf(fp,"\n");
 fprintf(fp,"CHEADERCINT = ");
 G__printstringlist(fp,G__CHDR,G__PRINTSTRING);
 fprintf(fp,"\n");
 fprintf(fp,"\n");

 fprintf(fp,"CPPHEADER  = ");
 G__printstringlist_noopt(fp,G__CPPHDR,G__PRINTSTRING);
 fprintf(fp,"\n");
 fprintf(fp,"CPPHEADERCINT  = ");
 G__printstringlist(fp,G__CPPHDR,G__PRINTSTRING);
 fprintf(fp,"\n");
 fprintf(fp,"\n");

 fprintf(fp,"CSTUB    = ");
 G__printstringlist_noopt(fp,G__CSTUB,G__PRINTSTRING);
 fprintf(fp,"\n");
 fprintf(fp,"CSTUBCINT = ");
 G__printstringlist(fp,G__CSTUB,G__PRINTSTRING);
 fprintf(fp,"\n");
 fprintf(fp,"\n");
 
 fprintf(fp,"CPPSTUB    = ");
 G__printstringlist_noopt(fp,G__CPPSTUB,G__PRINTSTRING);
 fprintf(fp,"\n");
 fprintf(fp,"CPPSTUBCINT = ");
 G__printstringlist(fp,G__CPPSTUB,G__PRINTSTRING);
 fprintf(fp,"\n");
 fprintf(fp,"\n");

 if(G__isDLL) {
   fprintf(fp,"DLLSPEC       =\n");
 }
 else {
    fprintf(fp,"DLLSPEC     = -N%s\n",G__DLLID);
 }

  if(G__CHDR) {
    fprintf(fp,"CIFC       = G__c_%s%s\n",G__DLLID,G__CSRCPOST);
    fprintf(fp,"CIFH       = G__c_%s%s\n",G__DLLID,G__CHDRPOST);
    fprintf(fp,"CIFO       = G__c_%s%s\n",G__DLLID,G__OBJPOST);
  }
  else {
    fprintf(fp,"CIFC       =\n");
    fprintf(fp,"CIFH       =\n");
    fprintf(fp,"CIFO       =\n");
  }
  if(G__CPPHDR) {
    fprintf(fp,"CPPIFC     = G__cpp_%s%s\n",G__DLLID,G__CPPSRCPOST);
    fprintf(fp,"CPPIFH     = G__cpp_%s%s\n",G__DLLID,G__CPPHDRPOST);
    fprintf(fp,"CPPIFO     = G__cpp_%s%s\n",G__DLLID,G__OBJPOST);
  }
  else {
    fprintf(fp,"CPPIFC     =\n");
    fprintf(fp,"CPPIFH     =\n");
    fprintf(fp,"CPPIFO     =\n");
  }
  fprintf(fp,"\n");
}

#ifndef G__OLDIMPLEMENTATION574
/**************************************************************************
* G__ouputstring()
*
**************************************************************************/
G__outputstring(fp,list,pre,post)
FILE *fp;
struct G__string_list *list;
char *pre;
char *post;
{
  struct G__string_list *p;
  
  p = list;
  while(p) {
    fprintf(fp,"%s%s%s",pre,p->string,post);
    p = p->next;
  }
}
#endif


/**************************************************************************
* G__ouputsrc()
*
**************************************************************************/
G__outputsrc(fp,list,pre,post)
FILE *fp;
struct G__string_list *list;
char *pre;
char *post;
{
  struct G__string_list *p;
  
  p = list;
  while(p) {
    switch(p->string[0]) {
    case '+':
    case '-':
      /* +P,-P,+V,-V options are ignored */
      break;
    default:
      fprintf(fp,"%s%s%s",pre,p->object,post);
    }
    p = p->next;
  }
}



/**************************************************************************
* G__ouputobj()
*
**************************************************************************/
G__outputobj(fp,list,pre,post)
FILE *fp;
struct G__string_list *list;
char *pre;
char *post;
{
  struct G__string_list *p;
  
  p = list;
  while(p) {
    if(p->string && p->object) {
      fprintf(fp,"%s%s%s",pre,p->object,post);
    }
    p = p->next;
  }
}


/**************************************************************************
* G__compileallsources()
*
**************************************************************************/
G__compileallsources(fp,list,headers)
FILE *fp;
struct G__string_list *list;
char *headers;
{
  struct G__string_list *p;

  p = list;
  while(p) {
    if(p->string && p->object) {
      G__compileeachsource(fp,p->string,p->object,headers);
    }
    p = p->next;
  }
}

/**************************************************************************
* G__compileeachsource()
*
**************************************************************************/
G__compileeachsource(fp,source,object,header) 
FILE *fp;
char *source;
char *object;
char *header;
{
  fprintf(fp,"################################################################################\n");
  fprintf(fp,"# Begin Source File\n");
  fprintf(fp,"\n");
  switch(source[0]) {
  case '.':
  case '\\': 
	fprintf(fp,"SOURCE=%s\n",source);
	break;
  default:
    fprintf(fp,"SOURCE=.\\%s\n",source);
	break;
  }
  fprintf(fp,"DEP_CPP_COMPL=%s\n",header);
  fprintf(fp,"	\n");
  fprintf(fp,"\n");
  fprintf(fp,"\"$(INTDIR)\\%s\" : $(SOURCE) $(DEP_CPP_COMPL) \"$(INTDIR)\"\n"
	  ,object);
  fprintf(fp,"\n");
  fprintf(fp,"\n");
  fprintf(fp,"# End Source File\n");
}


/**************************************************************************
* G__outputifsrc()
*
**************************************************************************/
G__outputifsrc(fp)
FILE *fp;
{
  fprintf(fp,"################################################################################\n");
  fprintf(fp,"# Begin Source File\n");
  fprintf(fp,"\n");
  if(G__CHDR) {
    fprintf(fp,"$(CIFC) : $(CHEADER) $(CSTUB)\n");
    fprintf(fp,"\tcint %s %s -K -w%d -z%s -n$(CIFC) $(DLLSPEC) -D__MAKECINT__ %s -c-2 $(IPATHCINT) $(MACROCINT) $(CHEADERCINT)"
	  ,G__INITFUNC,G__CINTLIBNAME,G__isDLL,G__DLLID,G__preprocess);
    if(G__CSTUB) fprintf(fp," +STUB $(CSTUBCINT) -STUB\n");
    else      fprintf(fp,"\n");
    fprintf(fp,"\n");

    fprintf(fp,".\\%s.def : $(CHEADER) $(CSTUB)\n",G__DLLID);
    fprintf(fp,"\tcint %s %s -K -w%d -z%s -n$(CIFC) $(DLLSPEC) -D__MAKECINT__ %s -c-2 $(IPATHCINT) $(MACROCINT) $(CHEADERCINT)"
	  ,G__INITFUNC,G__CINTLIBNAME,G__isDLL,G__DLLID,G__preprocess);
    if(G__CSTUB) fprintf(fp," +STUB $(CSTUBCINT) -STUB\n");
    else      fprintf(fp,"\n");
    fprintf(fp,"\n");

    fprintf(fp,"# End Source File\n");
  }
  if(G__CPPHDR) {
    fprintf(fp,"################################################################################\n");
    fprintf(fp,"$(CPPIFC) : $(CPPHEADER) $(CPPSTUB)\n");
    fprintf(fp,"\tcint %s %s -w%d -z%s -n$(CPPIFC) $(DLLSPEC) -D__MAKECINT__ %s -c-1 -A $(IPATHCINT) $(MACROCINT) $(CPPHEADERCINT)"
 	  ,G__INITFUNC,G__CINTLIBNAME,G__isDLL,G__DLLID,G__preprocess);
    if(G__CPPSTUB) fprintf(fp," +STUB $(CPPSTUBCINT) -STUB\n");
    else        fprintf(fp,"\n");
    fprintf(fp,"\n");

    fprintf(fp,".\\%s.def : $(CPPHEADER) $(CPPSTUB)\n",G__DLLID);
    fprintf(fp,"\tcint %s %s -w%d -z%s -n$(CPPIFC) $(DLLSPEC) -D__MAKECINT__ %s -c-1 -A $(IPATHCINT) $(MACROCINT) $(CPPHEADERCINT)"
 	  ,G__INITFUNC,G__CINTLIBNAME,G__isDLL,G__DLLID,G__preprocess);
    if(G__CPPSTUB) fprintf(fp," +STUB $(CPPSTUBCINT) -STUB\n");
    else        fprintf(fp,"\n");
    fprintf(fp,"\n");

    fprintf(fp,"# End Source File\n");
  }
}


/**************************************************************************
* G__outputmakefile()
*
**************************************************************************/
G__outputmakefile() {
 FILE *fp;

 fp = fopen(G__makefile,"w");

 fprintf(fp,"# Microsoft Developer Studio Generated NMAKE File, Format Version 4.00\n");
 fprintf(fp,"# ** DO NOT EDIT **\n");
 fprintf(fp,"\n");
 fprintf(fp,"# TARGTYPE \"Win32 (x86) Console Application\" 0x0103\n");
 fprintf(fp,"\n");
 fprintf(fp,"!IF \"$(CFG)\" == \"\"\n");
 fprintf(fp,"CFG=%s - Win32 Debug\n",G__DLLID);
 fprintf(fp,"!MESSAGE No configuration specified.  Defaulting to %s - Win32 Debug.\n",G__DLLID);
 fprintf(fp,"!ENDIF \n");
 fprintf(fp,"\n");
 fprintf(fp,"!IF \"$(CFG)\" != \"%s - Win32 Release\" && \"$(CFG)\" !=\\\n",G__DLLID);
 fprintf(fp," \"%s - Win32 Debug\"\n",G__DLLID);
 fprintf(fp,"!MESSAGE Invalid configuration \"$(CFG)\" specified.\n");
 fprintf(fp,"!MESSAGE You can specify a configuration when running NMAKE on this makefile\n");
 fprintf(fp,"!MESSAGE by defining the macro CFG on the command line.  For example:\n");
 fprintf(fp,"!MESSAGE \n");
 fprintf(fp,"!MESSAGE NMAKE /f \"%s.mak\" CFG=\"%s - Win32 Debug\"\n",G__DLLID,G__DLLID);
 fprintf(fp,"!MESSAGE \n");
 fprintf(fp,"!MESSAGE Possible choices for configuration are:\n");
 fprintf(fp,"!MESSAGE \n");
 fprintf(fp,"!MESSAGE \"%s - Win32 Release\" (based on \"Win32 (x86) Console Application\")\n",G__DLLID);
 fprintf(fp,"!MESSAGE \"%s - Win32 Debug\" (based on \"Win32 (x86) Console Application\")\n",G__DLLID);
 fprintf(fp,"!MESSAGE \n");
 fprintf(fp,"!ERROR An invalid configuration is specified.\n");
 fprintf(fp,"!ENDIF \n");
 fprintf(fp,"\n");
 fprintf(fp,"!IF \"$(OS)\" == \"Windows_NT\"\n");
 fprintf(fp,"NULL=\n");
 fprintf(fp,"!ELSE \n");
 fprintf(fp,"NULL=nul\n");
 fprintf(fp,"!ENDIF \n");

 G__outputcintspecific(fp);

 fprintf(fp,"################################################################################\n");
 fprintf(fp,"# Begin Project\n");
 fprintf(fp,"# PROP Target_Last_Scanned \"%s - Win32 Debug\"\n",G__DLLID);
 fprintf(fp,"CPP=cl.exe\n");
 fprintf(fp,"RSC=rc.exe\n");
 fprintf(fp,"\n");
 fprintf(fp,"!IF  \"$(CFG)\" == \"%s - Win32 Release\"\n",G__DLLID);
 fprintf(fp,"\n");
 fprintf(fp,"# PROP BASE Use_MFC 0\n");
 fprintf(fp,"# PROP BASE Use_Debug_Libraries 0\n");
 fprintf(fp,"# PROP BASE Output_Dir \"Release\"\n");
 fprintf(fp,"# PROP BASE Intermediate_Dir \"Release\"\n");
 fprintf(fp,"# PROP BASE Target_Dir \"\"\n");
 fprintf(fp,"# PROP Use_MFC 0\n");
 fprintf(fp,"# PROP Use_Debug_Libraries 0\n");
 fprintf(fp,"# PROP Output_Dir \"Release\"\n");
 fprintf(fp,"# PROP Intermediate_Dir \"Release\"\n");
 fprintf(fp,"# PROP Target_Dir \"\"\n");
 fprintf(fp,"OUTDIR=.\\Release\n");
 fprintf(fp,"INTDIR=.\\Release\n");
 fprintf(fp,"\n");
 fprintf(fp,"ALL : \"$(OUTDIR)\\%s.exe\"\n",G__DLLID);
 fprintf(fp,"\n");
 fprintf(fp,"CLEAN : \n");
 fprintf(fp,"	-@erase \".\\Release\\%s.exe\"\n",G__DLLID);
 if(G__CPPHDR) {
   fprintf(fp,"	-@erase \".\\Release\\G__cpp_%s.obj\"\n",G__DLLID);
   fprintf(fp,"	-@erase \".\\G__cpp_%s.cxx\"\n",G__DLLID);
   fprintf(fp,"	-@erase \".\\G__cpp_%s.h\"\n",G__DLLID);
 }
 if(G__CHDR) {
   fprintf(fp,"	-@erase \".\\Release\\G__c_%s.obj\"\n",G__DLLID);
   fprintf(fp,"	-@erase \".\\G__c_%s.c\"\n",G__DLLID);
   fprintf(fp,"	-@erase \".\\G__c_%s.h\"\n",G__DLLID);
 }
 G__outputobj(fp,G__CPPSRC,"	-@erase \".\\Release\\","\"\n");
 G__outputobj(fp,G__CSRC,"	-@erase \".\\Release\\","\"\n");
 /* fprintf(fp,"	-@erase \".\\Release\\Complex.obj\"\n"); */

 fprintf(fp,"	-@erase \".\\Release\\G__main.obj\"\n");
 fprintf(fp,"\n");
 fprintf(fp,"\"$(OUTDIR)\" :\n");
 fprintf(fp,"    if not exist \"$(OUTDIR)/$(NULL)\" mkdir \"$(OUTDIR)\"\n");
 fprintf(fp,"\n");
 fprintf(fp,"# ADD BASE CPP /nologo /W3 /GX /O2 /D \"WIN32\" /D \"NDEBUG\" /D \"_CONSOLE\" /YX /c\n");
 fprintf(fp,"# ADD CPP /nologo /W3 /GX /O2 /I \"$(CINTSYSDIR)\" /I \"$(CINTSYSDIR)/src\" /D \"NDEBUG\" /D \"WIN32\" /D \"_CONSOLE\" /D \"G__VISUAL\" /D \"G__WIN32\" /D \"G__SHAREDLIB\" /YX /c\n");
 fprintf(fp,"CPP_PROJ=/nologo /ML /W3 /GX /O2 /I \"$(CINTSYSDIR)\" /I \"$(CINTSYSDIR)/src\" /D \"NDEBUG\" /D\\\n");
 fprintf(fp," \"WIN32\" /D \"_CONSOLE\" /D \"G__VISUAL\" /D \"G__WIN32\" /D \"G__SHAREDLIB\"\\\n");
 fprintf(fp," /Fp\"$(INTDIR)/%s.pch\" /YX /Fo\"$(INTDIR)/\" /c $(IPATH) $(MACRO)\n",G__DLLID);
 fprintf(fp,"CPP_OBJS=.\\Release/\n");
 fprintf(fp,"CPP_SBRS=\n");
 fprintf(fp,"# ADD BASE RSC /l 0x411 /d \"NDEBUG\"\n");
 fprintf(fp,"# ADD RSC /l 0x411 /d \"NDEBUG\"\n");
 fprintf(fp,"BSC32=bscmake.exe\n");
 fprintf(fp,"# ADD BASE BSC32 /nologo\n");
 fprintf(fp,"# ADD BSC32 /nologo\n");
 fprintf(fp,"BSC32_FLAGS=/nologo /o\"$(OUTDIR)/%s.bsc\" \n",G__DLLID);
 fprintf(fp,"BSC32_SBRS=\n");
 fprintf(fp,"LINK32=link.exe\n");
 fprintf(fp,"# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /machine:I386\n");
 fprintf(fp,"# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /machine:I386\n");
 fprintf(fp,"LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\\\n");
 fprintf(fp," advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo\\\n");
 fprintf(fp," /subsystem:console /incremental:no /pdb:\"$(OUTDIR)/%s.pdb\" /machine:I386\\\n",G__DLLID);
 fprintf(fp," /out:\"$(OUTDIR)/%s.exe\" \n",G__DLLID);
 fprintf(fp,"LINK32_OBJS= \\\n");
 if(G__CPPHDR) fprintf(fp,"	\"$(INTDIR)/G__cpp_%s.obj\" \\\n",G__DLLID);
 if(G__CHDR) fprintf(fp,"	\"$(INTDIR)/G__c_%s.obj\" \\\n",G__DLLID);
 G__outputobj(fp,G__CPPSRC,"	\"$(INTDIR)/","\" \\\n");
 G__outputobj(fp,G__CSRC,"	\"$(INTDIR)/","\" \\\n");
 /* fprintf(fp,"	\"$(INTDIR)/Complex.obj\" \\\n"); */

 fprintf(fp,"	\"$(INTDIR)/G__main.obj\" \\\n");
#ifndef G__OLDIMPLEMENTATION574
 G__outputstring(fp,G__LIB,"	\"","\"\\\n");
#else
 G__outputsrc(fp,G__LIB,"	\"","\"\\\n");
#endif
 fprintf(fp,"	\"%s.lib\"\n",G__LIBNAME);
 fprintf(fp,"\n");
 fprintf(fp,"\"$(OUTDIR)\\%s.exe\" : \"$(OUTDIR)\" $(DEF_FILE) $(LINK32_OBJS)\n",G__DLLID);
 fprintf(fp,"    $(LINK32) @<<\n");
 fprintf(fp,"  $(LINK32_FLAGS) $(LINK32_OBJS)\n");
 fprintf(fp,"<<\n");
 fprintf(fp,"\n");
 fprintf(fp,"!ELSEIF  \"$(CFG)\" == \"%s - Win32 Debug\"\n",G__DLLID);
 fprintf(fp,"\n");
 fprintf(fp,"# PROP BASE Use_MFC 0\n");
 fprintf(fp,"# PROP BASE Use_Debug_Libraries 1\n");
 fprintf(fp,"# PROP BASE Output_Dir \"Debug\"\n");
 fprintf(fp,"# PROP BASE Intermediate_Dir \"Debug\"\n");
 fprintf(fp,"# PROP BASE Target_Dir \"\"\n");
 fprintf(fp,"# PROP Use_MFC 0\n");
 fprintf(fp,"# PROP Use_Debug_Libraries 1\n");
 fprintf(fp,"# PROP Output_Dir \"Debug\"\n");
 fprintf(fp,"# PROP Intermediate_Dir \"Debug\"\n");
 fprintf(fp,"# PROP Target_Dir \"\"\n");
 fprintf(fp,"OUTDIR=.\\Debug\n");
 fprintf(fp,"INTDIR=.\\Debug\n");
 fprintf(fp,"\n");
 fprintf(fp,"ALL : \"$(OUTDIR)\\%s.exe\"\n",G__DLLID);
 fprintf(fp,"\n");
 fprintf(fp,"CLEAN : \n");
 fprintf(fp,"	-@erase \".\\Debug\\vc40.pdb\"\n");
 fprintf(fp,"	-@erase \".\\Debug\\vc40.idb\"\n");
 fprintf(fp,"	-@erase \".\\Debug\\%s.exe\"\n",G__DLLID);

 G__outputobj(fp,G__CPPSRC,"	-@erase \".\\Debug\\","\"\n");
 G__outputobj(fp,G__CSRC,"	-@erase \".\\Debug\\","\"\n");
 /* fprintf(fp,"	-@erase \".\\Debug\\Complex.obj\"\n"); */

 fprintf(fp,"	-@erase \".\\Debug\\G__main.obj\"\n");
 if(G__CPPHDR) {
   fprintf(fp,"	-@erase \".\\Debug\\G__cpp_%s.obj\"\n",G__DLLID);
   fprintf(fp,"	-@erase \".\\G__cpp_%s.cxx\"\n",G__DLLID);
   fprintf(fp,"	-@erase \".\\G__cpp_%s.h\"\n",G__DLLID);
 }
 if(G__CHDR) {
   fprintf(fp,"	-@erase \".\\Debug\\G__c_%s.obj\"\n",G__DLLID);
   fprintf(fp,"	-@erase \".\\G__c_%s.c\"\n",G__DLLID);
   fprintf(fp,"	-@erase \".\\G__c_%s.h\"\n",G__DLLID);
 }
 fprintf(fp,"	-@erase \".\\Debug\\%s.ilk\"\n",G__DLLID);
 fprintf(fp,"	-@erase \".\\Debug\\%s.pdb\"\n",G__DLLID);
 fprintf(fp,"\n");
 fprintf(fp,"\"$(OUTDIR)\" :\n");
 fprintf(fp,"    if not exist \"$(OUTDIR)/$(NULL)\" mkdir \"$(OUTDIR)\"\n");
 fprintf(fp,"\n");
 fprintf(fp,"# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D \"WIN32\" /D \"_DEBUG\" /D \"_CONSOLE\" /YX /c\n");
 fprintf(fp,"# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /I \"$(CINTSYSDIR)\" /I \"$(CINTSYSDIR)/src\" /D \"_DEBUG\" /D \"WIN32\" /D \"_CONSOLE\" /D \"G__VISUAL\" /D \"G__WIN32\" /D \"G__SHAREDLIB\" /YX /c\n");
 fprintf(fp,"CPP_PROJ=/nologo /MLd /W3 /Gm /GX /Zi /Od /I \"$(CINTSYSDIR)\" /I \"$(CINTSYSDIR)/src\" /D\\\n");
 fprintf(fp," \"_DEBUG\" /D \"WIN32\" /D \"_CONSOLE\" /D \"G__VISUAL\" /D \"G__WIN32\" /D\\\n");
 fprintf(fp," \"G__SHAREDLIB\" /Fp\"$(INTDIR)/%s.pch\" /YX /Fo\"$(INTDIR)/\" /Fd\"$(INTDIR)/\"\\\n",G__DLLID);
 fprintf(fp," /c $(IPATH) $(MACRO)\n");
 fprintf(fp,"CPP_OBJS=.\\Debug/\n");
 fprintf(fp,"CPP_SBRS=\n");
 fprintf(fp,"# ADD BASE RSC /l 0x411 /d \"_DEBUG\"\n");
 fprintf(fp,"# ADD RSC /l 0x411 /d \"_DEBUG\"\n");
 fprintf(fp,"BSC32=bscmake.exe\n");
 fprintf(fp,"# ADD BASE BSC32 /nologo\n");
 fprintf(fp,"# ADD BSC32 /nologo\n");
 fprintf(fp,"BSC32_FLAGS=/nologo /o\"$(OUTDIR)/%s.bsc\" \n",G__DLLID);
 fprintf(fp,"BSC32_SBRS=\n");
 fprintf(fp,"LINK32=link.exe\n");
 fprintf(fp,"# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /debug /machine:I386\n");
 fprintf(fp,"# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /debug /machine:I386\n");
 fprintf(fp,"LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\\\n");
 fprintf(fp," advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo\\\n");
 fprintf(fp," /subsystem:console /incremental:yes /pdb:\"$(OUTDIR)/%s.pdb\" /debug\\\n",G__DLLID);
 fprintf(fp," /machine:I386 /out:\"$(OUTDIR)/%s.exe\" \n",G__DLLID);
 fprintf(fp,"LINK32_OBJS= \\\n");

 G__outputobj(fp,G__CPPSRC,"	\"$(INTDIR)/","\" \\\n");
 G__outputobj(fp,G__CSRC,"	\"$(INTDIR)/","\" \\\n");
 /* fprintf(fp,"	\"$(INTDIR)/Complex.obj\" \\\n"); */

 fprintf(fp,"	\"$(INTDIR)/G__main.obj\" \\\n");
 if(G__CPPHDR) fprintf(fp,"	\"$(INTDIR)/G__cpp_%s.obj\" \\\n",G__DLLID);
 if(G__CHDR) fprintf(fp,"	\"$(INTDIR)/G__c_%s.obj\" \\\n",G__DLLID);
#ifndef G__OLDIMPLEMENTATION574
 G__outputstring(fp,G__LIB,"	\"","\"\\\n");
#else
 G__outputsrc(fp,G__LIB,"	\"","\"\\\n");
#endif
 fprintf(fp,"	\"%s.lib\"\n",G__LIBNAME);
 fprintf(fp,"\n");
 fprintf(fp,"\"$(OUTDIR)\\%s.exe\" : \"$(OUTDIR)\" $(DEF_FILE) $(LINK32_OBJS)\n",G__DLLID);
 fprintf(fp,"    $(LINK32) @<<\n");
 fprintf(fp,"  $(LINK32_FLAGS) $(LINK32_OBJS)\n");
 fprintf(fp,"<<\n");
 fprintf(fp,"\n");
 fprintf(fp,"!ENDIF \n");
 fprintf(fp,"\n");
 fprintf(fp,".c{$(CPP_OBJS)}.obj:\n");
 fprintf(fp,"   $(CPP) $(CPP_PROJ) $<  \n");
 fprintf(fp,"\n");
 fprintf(fp,".cpp{$(CPP_OBJS)}.obj:\n");
 fprintf(fp,"   $(CPP) $(CPP_PROJ) $<  \n");
 fprintf(fp,"\n");
 fprintf(fp,".cxx{$(CPP_OBJS)}.obj:\n");
 fprintf(fp,"   $(CPP) $(CPP_PROJ) $<  \n");
 fprintf(fp,"\n");
 fprintf(fp,".c{$(CPP_SBRS)}.sbr:\n");
 fprintf(fp,"   $(CPP) $(CPP_PROJ) $<  \n");
 fprintf(fp,"\n");
 fprintf(fp,".cpp{$(CPP_SBRS)}.sbr:\n");
 fprintf(fp,"   $(CPP) $(CPP_PROJ) $<  \n");
 fprintf(fp,"\n");
 fprintf(fp,".cxx{$(CPP_SBRS)}.sbr:\n");
 fprintf(fp,"   $(CPP) $(CPP_PROJ) $<  \n");
 fprintf(fp,"\n");
 fprintf(fp,"################################################################################\n");
 fprintf(fp,"# Begin Target\n");
 fprintf(fp,"\n");
 fprintf(fp,"# Name \"%s - Win32 Release\"\n",G__DLLID);
 fprintf(fp,"# Name \"%s - Win32 Debug\"\n",G__DLLID);
 fprintf(fp,"\n");
 fprintf(fp,"!IF  \"$(CFG)\" == \"%s - Win32 Release\"\n",G__DLLID);
 fprintf(fp,"\n");
 fprintf(fp,"!ELSEIF  \"$(CFG)\" == \"%s - Win32 Debug\"\n",G__DLLID);
 fprintf(fp,"\n");
 fprintf(fp,"!ENDIF \n");
 fprintf(fp,"\n");
 
 G__compileallsources(fp,G__CPPSRC,"$(CPPHEADER)");
 G__compileallsources(fp,G__CSRC,"$(CHEADER)");
 
 if(G__CPPHDR) {
 fprintf(fp,"################################################################################\n");
 fprintf(fp,"# Begin Source File\n");
 fprintf(fp,"\n");
 fprintf(fp,"SOURCE=.\\G__cpp_%s.cxx\n",G__DLLID);
 fprintf(fp,"\n");
 fprintf(fp,"!IF  \"$(CFG)\" == \"%s - Win32 Release\"\n",G__DLLID);
 fprintf(fp,"\n");
 fprintf(fp,"DEP_CPP_G__CP=\\\n");
 fprintf(fp,"	\".\\G__cpp_%s.h\"\\\n",G__DLLID);
 fprintf(fp,"	\"%s\\G__ci.h\"\\\n",G__cintsysdir);
 fprintf(fp,"	\n");
 fprintf(fp,"\n");
 fprintf(fp,"\"$(INTDIR)\\G__cpp_%s.obj\" : $(SOURCE) $(DEP_CPP_G__CP) \"$(INTDIR)\"\n",G__DLLID);
 fprintf(fp,"\n");
 fprintf(fp,"\n");
 fprintf(fp,"!ELSEIF  \"$(CFG)\" == \"%s - Win32 Debug\"\n",G__DLLID);
 fprintf(fp,"\n");
 fprintf(fp,"DEP_CPP_G__CP=\\\n");
 fprintf(fp,"	\".\\G__cpp_%s.h\"\\\n",G__DLLID);
 fprintf(fp,"	\"%s\\G__ci.h\"\\\n",G__cintsysdir);
 fprintf(fp,"	\n");
 fprintf(fp,"\n");
 fprintf(fp,"\"$(INTDIR)\\G__cpp_%s.obj\" : $(SOURCE) $(DEP_CPP_G__CP) \"$(INTDIR)\"\n",G__DLLID);
 fprintf(fp,"\n");
 fprintf(fp,"\n");
 fprintf(fp,"!ENDIF \n");
 fprintf(fp,"\n");
 fprintf(fp,"# End Source File\n");
 }
  if(G__CHDR) {
 fprintf(fp,"################################################################################\n");
 fprintf(fp,"# Begin Source File\n");
 fprintf(fp,"\n");
 fprintf(fp,"SOURCE=.\\G__c_%s.c\n",G__DLLID);
 fprintf(fp,"\n");
 fprintf(fp,"!IF  \"$(CFG)\" == \"%s - Win32 Release\"\n",G__DLLID);
 fprintf(fp,"\n");
 fprintf(fp,"DEP_CPP_G__CP=\\\n");
 fprintf(fp,"	\".\\G__c_%s.h\"\\\n",G__DLLID);
 fprintf(fp,"	\"%s\\G__ci.h\"\\\n",G__cintsysdir);
 fprintf(fp,"	\n");
 fprintf(fp,"\n");
 fprintf(fp,"\"$(INTDIR)\\G__c_%s.obj\" : $(SOURCE) $(DEP_CPP_G__CP) \"$(INTDIR)\"\n",G__DLLID);
 fprintf(fp,"\n");
 fprintf(fp,"\n");
 fprintf(fp,"!ELSEIF  \"$(CFG)\" == \"%s - Win32 Debug\"\n",G__DLLID);
 fprintf(fp,"\n");
 fprintf(fp,"DEP_CPP_G__CP=\\\n");
 fprintf(fp,"	\".\\G__c_%s.h\"\\\n",G__DLLID);
 fprintf(fp,"	\"%s\\G__ci.h\"\\\n",G__cintsysdir);
 fprintf(fp,"	\n");
 fprintf(fp,"\n");
 fprintf(fp,"\"$(INTDIR)\\G__c_%s.obj\" : $(SOURCE) $(DEP_CPP_G__CP) \"$(INTDIR)\"\n",G__DLLID);
 fprintf(fp,"\n");
 fprintf(fp,"\n");
 fprintf(fp,"!ENDIF \n");
 fprintf(fp,"\n");
 fprintf(fp,"# End Source File\n");
 }
 fprintf(fp,"################################################################################\n");
 fprintf(fp,"# Begin Source File\n");
 fprintf(fp,"\n");
 fprintf(fp,"SOURCE=.\\G__main.cxx\n");
 fprintf(fp,"\n");
 fprintf(fp,"\"$(INTDIR)\\G__main.obj\" : $(SOURCE) \"$(INTDIR)\"\n");
 fprintf(fp,"\n");
 fprintf(fp,"\n");
 fprintf(fp,"# End Source File\n");

 G__outputifsrc(fp);

 fprintf(fp,"################################################################################\n");
 fprintf(fp,"# Begin Source File\n");
 fprintf(fp,"\n");
 fprintf(fp,"SOURCE=%s.lib\n",G__LIBNAME);
 fprintf(fp,"\n");
 fprintf(fp,"!IF  \"$(CFG)\" == \"%s - Win32 Release\"\n",G__DLLID);
 fprintf(fp,"\n");
 fprintf(fp,"!ELSEIF  \"$(CFG)\" == \"%s - Win32 Debug\"\n",G__DLLID);
 fprintf(fp,"\n");
 fprintf(fp,"!ENDIF \n");
 fprintf(fp,"\n");
 fprintf(fp,"# End Source File\n");
 fprintf(fp,"# End Target\n");
 fprintf(fp,"# End Project\n");
 fprintf(fp,"################################################################################\n");
 fprintf(fp,"\n");

 fclose(fp);
}

/**************************************************************************
* G__outputmakefiledll()
*
**************************************************************************/
G__outputmakefiledll() {
 FILE *fp;

 fp = fopen(G__makefile,"w");

 fprintf(fp,"# Microsoft Developer Studio Generated NMAKE File, Format Version 4.00\n");
 fprintf(fp,"# ** DO NOT EDIT **\n");
 fprintf(fp,"\n");
 fprintf(fp,"# TARGTYPE \"Win32 (x86) Dynamic-Link Library\" 0x0102\n");
 fprintf(fp,"\n");
 fprintf(fp,"!IF \"$(CFG)\" == \"\"\n");
 fprintf(fp,"CFG=%s - Win32 Debug\n",G__DLLID);
 fprintf(fp,"!MESSAGE No configuration specified.  Defaulting to %s - Win32 Debug.\n",G__DLLID);
 fprintf(fp,"!ENDIF \n");
 fprintf(fp,"\n");
 fprintf(fp,"!IF \"$(CFG)\" != \"%s - Win32 Release\" && \"$(CFG)\" !=\\\n",G__DLLID);
 fprintf(fp," \"%s - Win32 Debug\"\n",G__DLLID);
 fprintf(fp,"!MESSAGE Invalid configuration \"$(CFG)\" specified.\n");
 fprintf(fp,"!MESSAGE You can specify a configuration when running NMAKE on this makefile\n");
 fprintf(fp,"!MESSAGE by defining the macro CFG on the command line.  For example:\n");
 fprintf(fp,"!MESSAGE \n");
 fprintf(fp,"!MESSAGE NMAKE /f \"%s.mak\" CFG=\"%s - Win32 Debug\"\n",G__DLLID,G__DLLID);
 fprintf(fp,"!MESSAGE \n");
 fprintf(fp,"!MESSAGE Possible choices for configuration are:\n");
 fprintf(fp,"!MESSAGE \n");
 fprintf(fp,"!MESSAGE \"%s - Win32 Release\" (based on\\\n",G__DLLID);
 fprintf(fp," \"Win32 (x86) Dynamic-Link Library\")\n");
 fprintf(fp,"!MESSAGE \"%s - Win32 Debug\" (based on \"Win32 (x86) Dynamic-Link Library\")\n",G__DLLID);
 fprintf(fp,"!MESSAGE \n");
 fprintf(fp,"!ERROR An invalid configuration is specified.\n");
 fprintf(fp,"!ENDIF \n");
 fprintf(fp,"\n");
 fprintf(fp,"!IF \"$(OS)\" == \"Windows_NT\"\n");
 fprintf(fp,"NULL=\n");
 fprintf(fp,"!ELSE \n");
 fprintf(fp,"NULL=nul\n");
 fprintf(fp,"!ENDIF \n");

 G__outputcintspecific(fp);

 fprintf(fp,"################################################################################\n");
 fprintf(fp,"# Begin Project\n");
 fprintf(fp,"MTL=mktyplib.exe\n");
 fprintf(fp,"CPP=cl.exe\n");
 fprintf(fp,"RSC=rc.exe\n");
 fprintf(fp,"\n");
 fprintf(fp,"!IF  \"$(CFG)\" == \"%s - Win32 Release\"\n",G__DLLID);
 fprintf(fp,"\n");
 fprintf(fp,"# PROP BASE Use_MFC 0\n");
 fprintf(fp,"# PROP BASE Use_Debug_Libraries 0\n");
 fprintf(fp,"# PROP BASE Output_Dir \"Release\"\n");
 fprintf(fp,"# PROP BASE Intermediate_Dir \"Release\"\n");
 fprintf(fp,"# PROP BASE Target_Dir \"\"\n");
 fprintf(fp,"# PROP Use_MFC 0\n");
 fprintf(fp,"# PROP Use_Debug_Libraries 0\n");
 fprintf(fp,"# PROP Output_Dir \"Release\"\n");
 fprintf(fp,"# PROP Intermediate_Dir \"Release\"\n");
 fprintf(fp,"# PROP Target_Dir \"\"\n");
 fprintf(fp,"OUTDIR=.\\Release\n");
 fprintf(fp,"INTDIR=.\\Release\n");
 fprintf(fp,"\n");
 fprintf(fp,"ALL : \"$(OUTDIR)\\%s.dll\"\n",G__DLLID);
 fprintf(fp,"\n");
 fprintf(fp,"CLEAN : \n");
 fprintf(fp,"	-@erase \".\\Release\\%s.dll\"\n",G__DLLID);

 G__outputobj(fp,G__CPPSRC,"	-@erase \".\\Release\\","\"\n");
 G__outputobj(fp,G__CSRC,"	-@erase \".\\Release\\","\"\n");
 fprintf(fp,"	-@erase \".\\%s.def\"\n",G__DLLID);
 if(G__CPPHDR) {
   fprintf(fp,"	-@erase \".\\Release\\G__cpp_%s.obj\"\n",G__DLLID);
   fprintf(fp,"	-@erase \".\\G__cpp_%s.cxx\"\n",G__DLLID);
   fprintf(fp,"	-@erase \".\\G__cpp_%s.h\"\n",G__DLLID);
 }
 if(G__CHDR) {
   fprintf(fp,"	-@erase \".\\Release\\G__c_%s.obj\"\n",G__DLLID);
   fprintf(fp,"	-@erase \".\\G__c_%s.c\"\n",G__DLLID);
   fprintf(fp,"	-@erase \".\\G__c_%s.h\"\n",G__DLLID);
 }
 fprintf(fp,"	-@erase \".\\Release\\%s.lib\"\n",G__DLLID);
 fprintf(fp,"	-@erase \".\\Release\\%s.exp\"\n",G__DLLID);
 fprintf(fp,"\n");
 fprintf(fp,"\"$(OUTDIR)\" :\n");
 fprintf(fp,"    if not exist \"$(OUTDIR)/$(NULL)\" mkdir \"$(OUTDIR)\"\n");
 fprintf(fp,"\n");
 fprintf(fp,"# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D \"WIN32\" /D \"NDEBUG\" /D \"_WINDOWS\" /YX /c\n");
 fprintf(fp,"# ADD CPP /nologo /MT /W3 /GX /O2 /I \"$(CINTSYSDIR)\" /I \"$(CINTSYSDIR)/src\" /D \"NDEBUG\" /D \"WIN32\" /D \"_WINDOWS\" /D \"G__VISUAL\" /D \"G__WIN32\" /D \"G__SHAREDLIB\" /YX /c\n");
 fprintf(fp,"CPP_PROJ=/nologo /MT /W3 /GX /O2 /I \"$(CINTSYSDIR)\" /I \"$(CINTSYSDIR)/src\" /D \"NDEBUG\" /D\\\n");
 fprintf(fp," \"WIN32\" /D \"_WINDOWS\" /D \"G__VISUAL\" /D \"G__WIN32\" /D \"G__SHAREDLIB\"\\\n");
 fprintf(fp," /Fp\"$(INTDIR)/%s.pch\" /YX /Fo\"$(INTDIR)/\" /c $(IPATH) $(MACRO)\n",G__DLLID);
 fprintf(fp,"CPP_OBJS=.\\Release/\n");
 fprintf(fp,"CPP_SBRS=\n");
 fprintf(fp,"# ADD BASE MTL /nologo /D \"NDEBUG\" /win32\n");
 fprintf(fp,"# ADD MTL /nologo /D \"NDEBUG\" /win32\n");
 fprintf(fp,"MTL_PROJ=/nologo /D \"NDEBUG\" /win32 \n");
 fprintf(fp,"# ADD BASE RSC /l 0x411 /d \"NDEBUG\"\n");
 fprintf(fp,"# ADD RSC /l 0x411 /d \"NDEBUG\"\n");
 fprintf(fp,"BSC32=bscmake.exe\n");
 fprintf(fp,"# ADD BASE BSC32 /nologo\n");
 fprintf(fp,"# ADD BSC32 /nologo\n");
 fprintf(fp,"BSC32_FLAGS=/nologo /o\"$(OUTDIR)/%s.bsc\" \n",G__DLLID);
 fprintf(fp,"BSC32_SBRS=\n");
 fprintf(fp,"LINK32=link.exe\n");
 fprintf(fp,"# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /machine:I386\n");
 fprintf(fp,"# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /machine:I386\n");
 fprintf(fp,"LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\\\n");
 fprintf(fp," advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo\\\n");
 fprintf(fp," /subsystem:windows /dll /incremental:no /pdb:\"$(OUTDIR)/%s.pdb\"\\\n",G__DLLID);
 fprintf(fp," /machine:I386 /def:\".\\%s.def\" /out:\"$(OUTDIR)/%s.dll\" /implib:\"$(OUTDIR)/%s.lib\" \n",G__DLLID,G__DLLID,G__DLLID);
 fprintf(fp,"DEF_FILE= .\\%s.def\n",G__DLLID);

 fprintf(fp,"LINK32_OBJS= \\\n");

 G__outputobj(fp,G__CPPSRC,"	\".\\Release\\","\" \\\n");
 G__outputobj(fp,G__CSRC,"	\".\\Release\\","\" \\\n");
 /* fprintf(fp,"	\".\\Release\\Complex.obj\" \\\n");	*/
#ifndef G__OLDIMPLEMENTATION574
 G__outputstring(fp,G__LIB,"	\"","\"\\\n"); /* ADDED */
#endif

 if(G__CPPHDR) fprintf(fp,"	\".\\Release\\G__cpp_%s.obj\" \\\n",G__DLLID);
 if(G__CHDR) fprintf(fp,"	\".\\Release\\G__c_%s.obj\" \\\n",G__DLLID);
 fprintf(fp,"	\"%s.lib\"\n",G__LIBNAME);
 fprintf(fp,"\n");
 fprintf(fp,"\"$(OUTDIR)\\%s.dll\" : \"$(OUTDIR)\" $(DEF_FILE) $(LINK32_OBJS)\n",G__DLLID);
 fprintf(fp,"    $(LINK32) @<<\n");
 fprintf(fp,"  $(LINK32_FLAGS) $(LINK32_OBJS)\n");
 fprintf(fp,"<<\n");
 fprintf(fp,"\n");
 fprintf(fp,"!ELSEIF  \"$(CFG)\" == \"%s - Win32 Debug\"\n",G__DLLID);
 fprintf(fp,"\n");
 fprintf(fp,"# PROP BASE Use_MFC 0\n");
 fprintf(fp,"# PROP BASE Use_Debug_Libraries 1\n");
 fprintf(fp,"# PROP BASE Output_Dir \"Debug\"\n");
 fprintf(fp,"# PROP BASE Intermediate_Dir \"Debug\"\n");
 fprintf(fp,"# PROP BASE Target_Dir \"\"\n");
 fprintf(fp,"# PROP Use_MFC 0\n");
 fprintf(fp,"# PROP Use_Debug_Libraries 1\n");
 fprintf(fp,"# PROP Output_Dir \"Debug\"\n");
 fprintf(fp,"# PROP Intermediate_Dir \"Debug\"\n");
 fprintf(fp,"# PROP Target_Dir \"\"\n");
 fprintf(fp,"OUTDIR=.\\Debug\n");
 fprintf(fp,"INTDIR=.\\Debug\n");
 fprintf(fp,"\n");
 fprintf(fp,"ALL : \"$(OUTDIR)\\%s.dll\"\n",G__DLLID);
 fprintf(fp,"\n");
 fprintf(fp,"CLEAN : \n");
 fprintf(fp,"	-@erase \".\\Debug\\%s.dll\"\n",G__DLLID);

 G__outputobj(fp,G__CPPSRC,"	-@erase \".\\Debug\\","\"\n");
 G__outputobj(fp,G__CSRC,"	-@erase \".\\Debug\\","\"\n");
 fprintf(fp,"	-@erase \".\\%s.def\"\n",G__DLLID);
 if(G__CPPHDR) {
   fprintf(fp,"	-@erase \".\\Debug\\G__cpp_%s.obj\"\n",G__DLLID);
   fprintf(fp,"	-@erase \".\\G__cpp_%s.cxx\"\n",G__DLLID);
   fprintf(fp,"	-@erase \".\\G__cpp_%s.h\"\n",G__DLLID);
 }
 if(G__CHDR) {
   fprintf(fp,"	-@erase \".\\Debug\\G__c_%s.obj\"\n",G__DLLID);
   fprintf(fp,"	-@erase \".\\G__c_%s.c\"\n",G__DLLID);
   fprintf(fp,"	-@erase \".\\G__c_%s.h\"\n",G__DLLID);
 }
 fprintf(fp,"	-@erase \".\\Debug\\%s.ilk\"\n",G__DLLID);
 fprintf(fp,"	-@erase \".\\Debug\\%s.lib\"\n",G__DLLID);
 fprintf(fp,"	-@erase \".\\Debug\\%s.exp\"\n",G__DLLID);
 fprintf(fp,"	-@erase \".\\Debug\\%s.pdb\"\n",G__DLLID);
 fprintf(fp,"	-@erase \".\\Debug\\vc40.pdb\"\n");
 fprintf(fp,"	-@erase \".\\Debug\\vc40.idb\"\n");
 fprintf(fp,"\n");
 fprintf(fp,"\"$(OUTDIR)\" :\n");
 fprintf(fp,"    if not exist \"$(OUTDIR)/$(NULL)\" mkdir \"$(OUTDIR)\"\n");
 fprintf(fp,"\n");
 fprintf(fp,"# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D \"WIN32\" /D \"_DEBUG\" /D \"_WINDOWS\" /YX /c\n");
 fprintf(fp,"# ADD CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /I \"$(CINTSYSDIR)\" /I \"$(CINTSYSDIR)/src\" /D \"_DEBUG\" /D \"WIN32\" /D \"_WINDOWS\" /D \"G__VISUAL\" /D \"G__WIN32\" /D \"G__SHAREDLIB\" /YX /c\n");
 fprintf(fp,"CPP_PROJ=/nologo /MTd /W3 /Gm /GX /Zi /Od /I \"$(CINTSYSDIR)\" /I \"$(CINTSYSDIR)/src\" /D\\\n");
 fprintf(fp," \"_DEBUG\" /D \"WIN32\" /D \"_WINDOWS\" /D \"G__VISUAL\" /D \"G__WIN32\" /D\\\n");
 fprintf(fp," \"G__SHAREDLIB\" /Fp\"$(INTDIR)/%s.pch\" /YX /Fo\"$(INTDIR)/\" /Fd\"$(INTDIR)/\"\\\n",G__DLLID);
 fprintf(fp," /c $(IPATH) $(MACRO)\n");
 fprintf(fp,"CPP_OBJS=.\\Debug/\n");
 fprintf(fp,"CPP_SBRS=\n");
 fprintf(fp,"# ADD BASE MTL /nologo /D \"_DEBUG\" /win32\n");
 fprintf(fp,"# ADD MTL /nologo /D \"_DEBUG\" /win32\n");
 fprintf(fp,"MTL_PROJ=/nologo /D \"_DEBUG\" /win32 \n");
 fprintf(fp,"# ADD BASE RSC /l 0x411 /d \"_DEBUG\"\n");
 fprintf(fp,"# ADD RSC /l 0x411 /d \"_DEBUG\"\n");
 fprintf(fp,"BSC32=bscmake.exe\n");
 fprintf(fp,"# ADD BASE BSC32 /nologo\n");
 fprintf(fp,"# ADD BSC32 /nologo\n");
 fprintf(fp,"BSC32_FLAGS=/nologo /o\"$(OUTDIR)/%s.bsc\" \n",G__DLLID);
 fprintf(fp,"BSC32_SBRS=\n");
 fprintf(fp,"LINK32=link.exe\n");
 fprintf(fp,"# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /debug /machine:I386\n");
 fprintf(fp,"# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /debug /machine:I386\n");
 fprintf(fp,"LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\\\n");
 fprintf(fp," advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo\\\n");
 fprintf(fp," /subsystem:windows /dll /incremental:yes /pdb:\"$(OUTDIR)/%s.pdb\" /debug\\\n",G__DLLID);
 fprintf(fp," /machine:I386 /def:\".\\%s.def\" /out:\"$(OUTDIR)/%s.dll\" /implib:\"$(OUTDIR)/%s.lib\" \n",G__DLLID,G__DLLID,G__DLLID);
 fprintf(fp,"DEF_FILE= .\\%s.def\n",G__DLLID);

 fprintf(fp,"LINK32_OBJS= \\\n");

 G__outputobj(fp,G__CPPSRC,"	\".\\Debug\\","\" \\\n");
 G__outputobj(fp,G__CSRC,"	\".\\Debug\\","\" \\\n");
 /* fprintf(fp,"	\".\\Debug\\Complex.obj\" \\\n"); */
#ifndef G__OLDIMPLEMENTATION574
 G__outputstring(fp,G__LIB,"	\"","\"\\\n"); /* ADDED */
#endif

 if(G__CPPHDR) fprintf(fp,"	\".\\Debug\\G__cpp_%s.obj\" \\\n",G__DLLID);
 if(G__CHDR) fprintf(fp,"	\".\\Debug\\G__c_%s.obj\" \\\n",G__DLLID);
 fprintf(fp,"	\"%s.lib\"\n",G__LIBNAME);
 fprintf(fp,"\n");
 fprintf(fp,"\"$(OUTDIR)\\%s.dll\" : \"$(OUTDIR)\" $(DEF_FILE) $(LINK32_OBJS)\n",G__DLLID);
 fprintf(fp,"    $(LINK32) @<<\n");
 fprintf(fp,"  $(LINK32_FLAGS) $(LINK32_OBJS)\n");
 fprintf(fp,"<<\n");
 fprintf(fp,"\n");
 fprintf(fp,"!ENDIF \n");
 fprintf(fp,"\n");
 fprintf(fp,".c{$(CPP_OBJS)}.obj:\n");
 fprintf(fp,"   $(CPP) $(CPP_PROJ) $<  \n");
 fprintf(fp,"\n");
 fprintf(fp,".cpp{$(CPP_OBJS)}.obj:\n");
 fprintf(fp,"   $(CPP) $(CPP_PROJ) $<  \n");
 fprintf(fp,"\n");
 fprintf(fp,".cxx{$(CPP_OBJS)}.obj:\n");
 fprintf(fp,"   $(CPP) $(CPP_PROJ) $<  \n");
 fprintf(fp,"\n");
 fprintf(fp,".c{$(CPP_SBRS)}.sbr:\n");
 fprintf(fp,"   $(CPP) $(CPP_PROJ) $<  \n");
 fprintf(fp,"\n");
 fprintf(fp,".cpp{$(CPP_SBRS)}.sbr:\n");
 fprintf(fp,"   $(CPP) $(CPP_PROJ) $<  \n");
 fprintf(fp,"\n");
 fprintf(fp,".cxx{$(CPP_SBRS)}.sbr:\n");
 fprintf(fp,"   $(CPP) $(CPP_PROJ) $<  \n");
 fprintf(fp,"\n");
 fprintf(fp,"################################################################################\n");
 fprintf(fp,"# Begin Target\n");
 fprintf(fp,"\n");
 fprintf(fp,"# Name \"%s - Win32 Release\"\n",G__DLLID);
 fprintf(fp,"# Name \"%s - Win32 Debug\"\n",G__DLLID);
 fprintf(fp,"\n");
 fprintf(fp,"!IF  \"$(CFG)\" == \"%s - Win32 Release\"\n",G__DLLID);
 fprintf(fp,"\n");
 fprintf(fp,"!ELSEIF  \"$(CFG)\" == \"%s - Win32 Debug\"\n",G__DLLID);
 fprintf(fp,"\n");
 fprintf(fp,"!ENDIF \n");
 fprintf(fp,"\n");
 fprintf(fp,"################################################################################\n");

 G__compileallsources(fp,G__CPPSRC,"$(CPPHEADER)");
 G__compileallsources(fp,G__CSRC,"$(CHEADER)");
/*
 fprintf(fp,"# Begin Source File\n");
 fprintf(fp,"\n");
 fprintf(fp,"SOURCE=.\\Complex.cxx\n");
 fprintf(fp,"DEP_CPP_COMPL=\\\n");
 fprintf(fp,"	\".\\Complex.h\"\\\n");
 fprintf(fp,"	\n");
 fprintf(fp,"\n");
 fprintf(fp,"\"$(INTDIR)\\Complex.obj\" : $(SOURCE) $(DEP_CPP_COMPL) \"$(INTDIR)\"\n");
 fprintf(fp,"\n");
 fprintf(fp,"\n");
 fprintf(fp,"# End Source File\n");
*/

 if(G__CPPHDR) {
 fprintf(fp,"################################################################################\n");
 fprintf(fp,"# Begin Source File\n");
 fprintf(fp,"\n");
 fprintf(fp,"SOURCE=.\\G__cpp_%s.cxx\n",G__DLLID);
 fprintf(fp,"DEP_CPP_G__CP=\\\n");
 fprintf(fp,"	\".\\G__cpp_%s.h\"\\\n",G__DLLID);
 fprintf(fp,"	\n");
 fprintf(fp,"NODEP_CPP_G__CP=\\\n");
 fprintf(fp,"	\"%s\\G__ci.h\"\\\n",G__cintsysdir);
 fprintf(fp,"	\n");
 fprintf(fp,"\n");
 fprintf(fp,"\"$(INTDIR)\\G__cpp_%s.obj\" : $(SOURCE) $(DEP_CPP_G__CP) \"$(INTDIR)\"\n",G__DLLID);
 fprintf(fp,"\n");
 fprintf(fp,"\n");
 fprintf(fp,"# End Source File\n");
 }
 if(G__CHDR) {
 fprintf(fp,"################################################################################\n");
 fprintf(fp,"# Begin Source File\n");
 fprintf(fp,"\n");
 fprintf(fp,"SOURCE=.\\G__c_%s.c\n",G__DLLID);
 fprintf(fp,"DEP_CPP_G__CP=\\\n");
 fprintf(fp,"	\".\\G__c_%s.h\"\\\n",G__DLLID);
 fprintf(fp,"	\n");
 fprintf(fp,"NODEP_CPP_G__CP=\\\n");
 fprintf(fp,"	\"%s\\G__ci.h\"\\\n",G__cintsysdir);
 fprintf(fp,"	\n");
 fprintf(fp,"\n");
 fprintf(fp,"\"$(INTDIR)\\G__c_%s.obj\" : $(SOURCE) $(DEP_CPP_G__CP) \"$(INTDIR)\"\n",G__DLLID);
 fprintf(fp,"\n");
 fprintf(fp,"\n");
 fprintf(fp,"# End Source File\n");
 }


 G__outputifsrc(fp);

 fprintf(fp,"################################################################################\n");
 fprintf(fp,"# Begin Source File\n");
 fprintf(fp,"\n");
 fprintf(fp,"SOURCE=%s.lib\n",G__LIBNAME);
 fprintf(fp,"\n");
 fprintf(fp,"!IF  \"$(CFG)\" == \"%s - Win32 Release\"\n",G__DLLID);
 fprintf(fp,"\n");
 fprintf(fp,"!ELSEIF  \"$(CFG)\" == \"%s - Win32 Debug\"\n",G__DLLID);
 fprintf(fp,"\n");
 fprintf(fp,"!ENDIF \n");
 fprintf(fp,"\n");
 fprintf(fp,"# End Source File\n");
 fprintf(fp,"# End Target\n");
 fprintf(fp,"# End Project\n");
 fprintf(fp,"################################################################################\n");
 fprintf(fp,"\n");

 fclose(fp);
}


/******************************************************************
* G__outputmain()
******************************************************************/
void G__outputmain()
{
  FILE *mainfp;
  /*****************************************************************
  * creating G__main.cxx
  *****************************************************************/
  mainfp = fopen("G__main.cxx","w");
  fprintf(mainfp,"/******************************************************\n");
  fprintf(mainfp,"* G__main.cxx\n");
  fprintf(mainfp,"*  automatically generated main() function for cint\n");
  fprintf(mainfp,"*  WinNT Symantec C++ environment\n");
  fprintf(mainfp,"******************************************************/\n");
  fprintf(mainfp,"#include <stdio.h>\n");
  fprintf(mainfp,"extern \"C\" {\n");
  fprintf(mainfp,"extern void G__setothermain(int othermain);\n");
  fprintf(mainfp,"extern int G__main(int argc,char **argv);\n");
  fprintf(mainfp,"extern void G__set_p2fsetup(void (*p2f)());\n");
  fprintf(mainfp,"extern void G__free_p2fsetup();\n");
  if(G__CHDR) fprintf(mainfp,"extern void G__c_setup%s();\n",G__DLLID);
  if(G__CPPHDR) fprintf(mainfp,"extern void G__cpp_setup%s();\n",G__DLLID);
  fprintf(mainfp,"}\n");
  fprintf(mainfp,"\n");
#ifndef G__OLDIMPLEMENTATION874
  if(G__ismain) {
    fprintf(mainfp,"class G__DMYp2fsetup {\n");
    fprintf(mainfp," public:\n");
    fprintf(mainfp,"  G__DMYp2fsetup() { \n");
    if(G__CHDR) fprintf(mainfp,"    G__set_p2fsetup(G__c_setup%s);\n",G__DLLID);
    if(G__CPPHDR) fprintf(mainfp,"    G__set_p2fsetup(G__cpp_setup%s);\n",G__DLLID);
    fprintf(mainfp,"  }\n");
    fprintf(mainfp,"} G__DMY;\n");
  }
  else {
#endif
    fprintf(mainfp,"int main(int argc,char **argv)\n");
    fprintf(mainfp,"{\n");
    fprintf(mainfp,"  int result;\n");
    if(G__CHDR) fprintf(mainfp,"  G__set_p2fsetup(G__c_setup%s);\n",G__DLLID);
    if(G__CPPHDR) fprintf(mainfp,"  G__set_p2fsetup(G__cpp_setup%s);\n",G__DLLID);
    fprintf(mainfp,"  G__setothermain(0);\n");
    fprintf(mainfp,"  result=G__main(argc,argv);\n");
    fprintf(mainfp,"  G__free_p2fsetup();\n");
    fprintf(mainfp,"  return(result);\n");
    fprintf(mainfp,"}\n");
#ifndef G__OLDIMPLEMENTATION874
  }
#endif

  fclose(mainfp);
  /*****************************************************************
  * end of creating G__main.cxx
  *****************************************************************/
}

/*******************************************************************
* G__outputmakebat()
*******************************************************************/
void G__outputmakebat()
{
  FILE* fp;
  fp = fopen("make.bat","w");
  fprintf(fp,"\n");
  fprintf(fp,"nmake CFG=\"%s - Win32 Release\" %%1 %%2 %%3 %%4 %%5 %%6\n",G__DLLID);
  if(G__isDLL) {
	fprintf(fp,"del %s.dll\n",G__DLLID);
    fprintf(fp,"move Release\\%s.dll .\\%s.dll\n",G__DLLID,G__DLLID);
  }
  else {
	fprintf(fp,"del %s.exe\n",G__DLLID);
	fprintf(fp,"move Release\\%s.exe .\\%s.exe\n",G__DLLID,G__DLLID);
  }
  fprintf(fp,"\n");
  fclose(fp);
}

/******************************************************************
* G__makecint
******************************************************************/
int G__makecint(argc,argv)
int argc;
char **argv;
{
  G__printtitle();
  G__readargument(argc,argv);
  G__readMAKEINFO();
  G__checksourcefiles(G__CSRC,G__CSRCPOST,G__OBJPOST);
  G__checksourcefiles(G__CPPSRC,G__CPPSRCPOST,G__OBJPOST);
  if(G__checksetup()) {
    fprintf(stderr,"!!!makecint aborted!!!  makecint -? for help\n");
    exit(EXIT_FAILURE);
  }
  if('\0'==G__LIBNAME[0]) sprintf(G__LIBNAME,"%s\\LIBCINT",G__cintsysdir);
  if(G__isDLL)  G__outputmakefiledll();
  else {
	G__outputmakefile();
    G__outputmain();
  }
  G__outputmakebat();
  G__cleanup();
  G__displaytodo();
  return(EXIT_SUCCESS);
}


/******************************************************************
* main
******************************************************************/
int main(argc,argv)
int argc;
char **argv;
{
  return(G__makecint(argc,argv));
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
