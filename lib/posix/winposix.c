/****************************************************************
* winposix.c
*  POSIX emulation function on Windows
****************************************************************/

#include "winposix.h"

/****************************************************************
* error function
****************************************************************/
static void printWinposixError(char* fname) {
  fprintf(stderr,"winposix: %s not supported\n",fname);
}

/****************************************************************
* POSIX system call emulation
****************************************************************/

DIR* opendir(const char *name) { 
  DIR* dir=(DIR*)malloc(sizeof(DIR));
  sprintf(dir->dirname,"%s\\*",name);
  dir->h=0;
  dir->posixdir.pwindir = &dir->windir;
  return(dir);
}

dirent* readdir(DIR* dir) {
  if(0==dir->h) {
    dir->h=FindFirstFile(dir->dirname,&dir->windir);
    if((void*)0xffffffff==dir->h) dir->h=0;
  }
  else {
    if(!FindNextFile(dir->h,&dir->windir)) dir->h=0;
  }
  if(dir->h && (void*)0xffffffff!=dir->h) {
    strcpy(dir->posixdir.d_name,dir->windir.cFileName);
    dir->posixdir.d_reclen=strlen(dir->posixdir.d_name);
    dir->posixdir.d_off = 0;
    dir->posixdir.d_ino = 0;
  }
  if(0==dir->h) return(0);
  else return(&dir->posixdir);
}

void rewinddir(DIR *dir) {
  if(dir->h) {
    FindClose(dir->h);
    dir->h=0;
  }
}

int closedir(DIR *dir) {
  if(dir) {
    if(dir->h) FindClose(dir->h);
    free((void*)dir);
    return(0);
  }
  return(-1);
}

char *getcwd(char *buf,size_t size) {
  if(GetCurrentDirectory(size,buf)) return(buf);
  else return(0);
}

int chdir(char *path) {
  if(SetCurrentDirectory(path)) return(0);
  else                          return(-1);
}

int rmdir(char *path) {
  if(RemoveDirectory(path)) return(0);
  else                      return(-1);
}

int mkdir(char *path,mode_t mode) {
  SECURITY_ATTRIBUTES secatt;
  memset((void*)(&secatt),0,sizeof(SECURITY_ATTRIBUTES));
  if(CreateDirectory(path,(LPVOID)(&secatt))) return(0);
  else                                        return(-1);
}

pid_t getpid(void) {
  return(GetCurrentProcessId());
}

uid_t getuid(void) {
  printWinposixError("getuid");
  return(-1);
}

gid_t getgid(void) {
  printWinposixError("getgid");
  return(-1);
}

char *getlogin(void) {
#if defined(__SC__) || defined(__VC13__)
  printWinposixError("getlogin");
  return(0);
#else
  static char buf[256];
  long nSize;
  if(!GetUserName(buf,&nSize)) return(0);
  else return(buf);
#endif
}

int uname(struct utsname* buf) {
  printWinposixError("uname");
  return(-1);
}

int stat(char *filename,struct stat *buf) {
  WIN32_FIND_DATA windat;
  HANDLE h=FindFirstFile(filename,&windat);
  if(!h) return(1);
  buf->st_size=windat.nFileSizeHigh*0x10000+windat.nFileSizeLow;
  return(0);
}

#ifndef __SC__
unsigned int sleep(unsigned int seconds) {
  Sleep(seconds*1000);
  return(0);
}
#endif

int putenv(const char *string) {
  char buf[1000];
  char *p;
  strcpy(buf,string);
  p = strchr(buf,'=');
  if(p) {
    *p++ = 0;
    if(SetEnvironmentVariable(buf,p)) return(0);  
    else                              return(-1);
  }
  return(-1);
}

#ifdef G__PROCESSPRIORITY
// Parameter for SetPriorityClass()
#define ABOVE_NORMAL_PRIORITY_CLASS     0x00008000
#define BELOW_NORMAL_PRIORITY_CLASS     0x00004000
#define HIGH_PRIORITY_CLASS             0x00000080
#define IDLE_PRIORITY_CLASS             0x00000040
#define NORMAL_PRIORITY_CLASS           0x00000020
#define PROCESS_MODE_BACKGROUND_BEGIN   0x00100000
#define PROCESS_MODE_BACKGROUND_END     0x00200000
#define REALTIME_PRIORITY_CLASS         0x00000100
int getpriority(int whitch,int who) {
  HANDLE hProcess;
  if(0==who) hProcess = GetCurrentProcess();
  else       hProcess = who;
  DWORD prio=GetPriorityClass(hProcess);
  switch(prio) {
  case REALTIME_PRIORITY_CLASS : return(-20);
  case HIGH_PRIORITY_CLASS : return(-15);
  case ABOVE_NORMAL_PRIORITY_CLASS : return(-10);
  case NORMAL_PRIORITY_CLASS : return(0);
  case BELOW_NORMAL_PRIORITY_CLASS : return(10);
  case IDLE_PRIORITY_CLASS : return(15);
  case PROCESS_MODE_BACKGROUND_BEGIN : return(19);
  case PROCESS_MODE_BACKGROUND_END : return(0);
  default: return(0);
  }
}
int setpriority(int whitch,int who,int prio) {
  HANDLE hProcess;
  if(0==who) hProcess = GetCurrentProcess();
  else       hProcess = who;
  DWORD prioclass;
  if(prio<=-20) prioclass = REALTIME_PRIORITY_CLASS;
  else if(prio<=-15) prioclass = HIGH_PRIORITY_CLASS;
  else if(prio<=-10) prioclass = ABOVE_NORMAL_PRIORITY_CLASS;
  else if(prio<=0)   prioclass = NORMAL_PRIORITY_CLASS;
  else if(prio<=10)  prioclass = BELOW_NORMAL_PRIORITY_CLASS;
  else if(prio<=15)  prioclass = IDLE_PRIORITY_CLASS;
  else if(prio<=19)  prioclass = PROCESS_MODE_BACKGROUND_BEGIN;
  else if(prio<=20)  prioclass = PROCESS_MODE_BACKGROUND_END;
  
  int result=SetPriorityClass(hProcess,prioclass);
  return(result);
}
#endif

/****************************************************************
* Original extension
****************************************************************/

int isDirectory(struct dirent* pd) {
  LPWIN32_FIND_DATA lpfd=pd->pwindir;
  return( lpfd->dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY ? 1:0);
}
