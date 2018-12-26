#include <stdio.h>
#include "EasyExp.h"
#include <string>
using namespace std;
#if defined(__CINT__)
#elif defined(_WIN32)
#include <windows.h>
#else
#include <unistd.h>
#endif

/*************************************************************************
* ChangeDirectory()
*************************************************************************/
void ChangeDirectory(const string& mp3editfile) {
  // Change directory to where spec file exists
  string path;
  unsigned int send=(unsigned int)(-1);
  unsigned int pos;
  pos = mp3editfile.rfind('/');
  if(pos==send) pos = mp3editfile.rfind('\\');
  if(pos!=send) {
    path = mp3editfile.substr(0,pos);
    printf("Working directory: %s\n",path.c_str());
#ifdef _WIN32
    SetCurrentDirectory(path.c_str());
#else
    chdir(path.c_str());
#endif
  }
}

/*************************************************************************
* crlf
*************************************************************************/
int crlf(const char* fname) {
  ChangeDirectory(fname);
  char ofile[500];
  sprintf(ofile,"%s_o.txt",fname);
  FILE *fp=fopen(fname,"rb");
  if(!fp) {
    fprintf(stderr,"Error: cannot open input file %s\n",fname);
    return(255);
  }
  FILE *ofp=fopen(ofile,"wb");
  for(int i=0;i<100&&!ofp;i++) ofp=fopen(ofile,"wb");
  if(!ofp) {
    fprintf(stderr,"Error: cannot open output file %s\n",ofile);
    fclose(fp);
    return(255);
  }
  int c;
  int flag;
  while(EOF!=(c=fgetc(fp))) {
    if(c==13) flag=1;
    else flag=0;
    if(c==10 && !flag) fputc(13,ofp);
    fputc(c,ofp);
  }
  fclose(ofp);
  fclose(fp);
  return(0);
}

/*************************************************************************
* scandir
*************************************************************************/
void scandir(const char* expr) {
  Scandir d(expr);
  string fname;
  while((fname=d.next())!="") {
    //if(fname=="_itemp.txt" || fname=="_otemp.txt") continue;
    if(fname.find("_o.txt")!=string::npos) continue;
    printf("%s\n",fname.c_str());
    crlf(fname.c_str());
  }
}

/*************************************************************************
* main
*************************************************************************/
int main(int argc,char* argv[]) {
  int i;
  for(i=1;i<argc;i++) {
    scandir(argv[i]);
  }
  return(0);
}

/*************************************************************************
*
*************************************************************************/
