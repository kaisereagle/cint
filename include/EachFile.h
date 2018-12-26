/////////////////////////////////////////////////////////////////////
// File scanner
/////////////////////////////////////////////////////////////////////

#pragma include <CSV.dll>
//#pragma include <EasyExp.dll>
#include <string>
#include <map>

int eachfile(void (*f)(const char*),const char* expr) {
  int isdir=0;
  int len=strlen(expr);
  if(len&&expr[len-1]=='/') {
    isdir=1;
    expr[len-1]=0;
  }
  Scandir dir(expr,isdir);
  char* fname;
  while((fname=dir.Next()) && fname[0]) {
    f(fname);
  }
}

/////////////////////////////////////////////////////////////////////
// sample function object

void _disp(const char *fname) {printf("%s\n",fname);}

/////////////////////////////////////////////////////////////////////
