/* /% C %/ */
/***********************************************************************
 * cint (C/C++ interpreter)
 ************************************************************************
 * CINT installation script setup.cxx
 ************************************************************************
 * Description:
 *  Build and install aux library
 ************************************************************************
 * Copyright(c) 1995~2003  Masaharu Goto (MXJ02154@niftyserve.or.jp)
 *
 * Permission to use, copy, modify and distribute this software and its 
 * documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation. The author makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 ************************************************************************/

/***********************************************************************/
const char *cintsysdir;

/***********************************************************************/
int docommand(const char* com) {
  printf("%s\n",com);
  int stat = system(com);
  return(stat);
}

/***********************************************************************/
void build(const char* subdir,const char* prog,const char* progpath) {
  char dir[300];
  char com[300];
  sprintf(dir,"%s\\%s",cintsysdir,subdir);
  G__chdir(dir);

  sprintf(com,"nmake -f %s.mak ALL CFG=\"%s - Win32 Release\" ",prog,prog);
  docommand(com);

  sprintf(com,"del %s\\%s",cintsysdir,progpath);
  docommand(com);

  sprintf(com,"move %s\\%s\\Release\\%s.exe %s\\%s"
	,cintsysdir,subdir,prog,cintsysdir,progpath);
  docommand(com);
}


/***********************************************************************/
void mkincld() {

  build("include\\mkincld","mkincld","include\\mkincld.exe");

  char dir[300];
  sprintf(dir,"%s\\include",cintsysdir);
  G__chdir(dir);

  docommand("mkincld.exe");
}

/***********************************************************************/
void buildlib(const char* subdir) {
  char dir[300];
  char com[300];
  sprintf(dir,"%s\\%s",cintsysdir,subdir);
  G__chdir(dir);

  docommand("setup.bat");
  //docommand(com);
}

/***********************************************************************/
void makeit() {
  char dir[300];
  char com[300];
  sprintf(dir,"%s\\include",cintsysdir);
  G__chdir(dir);

  sprintf(com,"%s\\cint.exe makeit.c");
  docommand(com);
}

/***********************************************************************/
int main(int argc,char** argv) {
  char *platform = "visualCpp6";
  for(int i=1;i<argc;i++) {
    if(strcmp(argv[i],"-p")==0) {
      platform = argv[++i];
    }
    ++i;
  }
 
  cintsysdir = getenv("CINTSYSDIR");
  if(!cintsysdir) cintsysdir="c:\\cint";

  mkincld();

  char makecintdir[300];
  sprintf(makecintdir,"platform\\%s\\tool\\makecint",platform);
  build(makecintdir,"makecint","makecint.exe");

  buildlib("lib\\longlong");
  buildlib("lib\\win32api");
  buildlib("lib\\posix");
  buildlib("lib\\socket");
  buildlib("lib\\dll_stl");
  buildlib("lib\\stdstrct");
  buildlib("tool\\ifdef");

  makeit();

  G__chdir(cintsysdir);

  return 0;
}

/***********************************************************************/

