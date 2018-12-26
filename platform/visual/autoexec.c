/*********************************************************
* platform\visual\autoexec.c
*   Adding environment variable settings in C:\autoexec.bat
*
*  Copyright(c)  1996-1997   Masaharu Goto
*********************************************************/

int changed=0;

// check given environment variable if value is already set
// returns 0 if already set
// returns 1 if not set
// Test is case insensitive
int check_env(char *envname,char *value) {

  // get environment variable
  char buf[2000];
  if(getenv(envname)) strcpy(buf,getenv(envname));
  else buf[0]='\0';

  // force every char to be upper case
  int i=0;
  while(buf[i]) {
    buf[i]=toupper(buf[i]);
    ++i;
  }
  i=0;
  while(value[i]) {
    value[i]=toupper(value[i]);
    ++i;
  }

  // test if value is already set
  if(strstr(buf,value)) return 0; // set
  else                  return 1; // not set
}

// add a line to autoexec.bat
int add_autoexec(char *line) {
  if(0==changed) {
    printf("backup c:\\autoexec.bat as .\\autoexec.bk\n");
    system("copy C:\\AUTOEXEC.BAT AUTOEXEC.BK");
  }
  FILE *fp=fopen("C:\\AUTOEXEC.BAT","a");
  if(!fp) return 1;  // open failed
  ++changed; // increment changed count 

  // add a line
  fprintf(fp,"%s\n",line);
  printf("added to autoexec.bat : %s\n",line);

  // close file
  fclose(fp);
  return 0; // successful
}

// set CINT environment variable
void add_cint() {
  if(check_env("PATH","C:\\CINT")) {
    add_autoexec("");
    add_autoexec("REM # CINT");
    add_autoexec("SET CINTSYSDIR=C:\\CINT");
    add_autoexec("SET PATH=%PATH%;%CINTSYSDIR%");
  }
}

// set WILDC environment variable
void add_wildc() {
  if(check_env("WILDCDIR","C:\\WILDC")) {
    add_autoexec("");
    add_autoexec("REM # WILDC");
    add_autoexec("SET WILDCDIR=C:\\WILDC");
  }
  if(check_env("PATH","C:\\WILDC\\BIN"))
    add_autoexec("SET PATH=%PATH%;%WILDCDIR%\\BIN");
}

// set ROOT environment variable
void add_root() {
  if(check_env("ROOTDIR","C:\\ROOT\\ROOT")) {
    add_autoexec("");
    add_autoexec("REM # ROOT");
    add_autoexec("SET ROOTDIR=C:\\ROOT\\ROOT");
  }
  if(check_env("PATH","C:\\ROOT\\ROOT\\BIN"))
   add_autoexec("SET PATH=%PATH%;%ROOTDIR%\\BIN");
}

// add Tcl/Tk environment variable
void add_tcltk() {
  if(check_env("PATH","C:\\TCL\\BIN")) {
    add_autoexec("");
    add_autoexec("REM # Tck/Tk");
    add_autoexec("SET PATH=%PATH%;C:\\TCL\\BIN");
  }
}

// main
int main() {
  add_cint();
  // add_root();
  // add_wildc();
  // add_tcltk();
  if(changed) {
    printf("\n!!! C:\\AUTOEXEC.BAT is modified. Re-boot system !!!\n");
    printf("The old c:\\autoexec.bat is copied as .\\autoexec.bk\n"); 
  } 
  return 0;
}


