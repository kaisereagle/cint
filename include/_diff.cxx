//////////////////////////////////////////////////////////////////////
// _diff.cxx
//////////////////////////////////////////////////////////////////////
//#include <EasyExp.dll>
#include <CSV.h>
#include <stdlib.h>

#ifdef G__WIN32
const char sep= '\\';
#else
const char sep= '/';
#endif

int main(int argc,char** argv) {
  int caseindependent=1;
  int isdirectory=0;
  if(argc<3) {
    fprintf(stderr,"Usage: cint _diff.cxx [FileExpr1] [Dir]\n");
    exit(255);
  }
  string target = argv[1];
  string dir = argv[2];
  string cwd;

  size_t pos = target.find_last_of(sep);
  if(pos!=string::npos) {
    char buf[1000];
    cwd = getcwd(buf,1000); 
    string odir = target.substr(0,pos);
    target = target.substr(pos+1);
    chdir(odir.c_str());
  }

  Scandir d(target.c_str(),isdirectory,caseindependent);

  char *fname;
  string com;
  int stat;
  while((fname=d.Next())&&fname[0]) {
    com = "diff \""; com+= fname; com+="\" \"";
    if(dir[0]==sep) { com += dir; com+= sep; com+=fname; }
    else {            com += cwd; com +=sep; com+=dir; com+=sep; com+=fname; }
    com += "\"";
    printf("# %s\n",com.c_str());
    stat = system(com.c_str());
    //printf("%d %s\n",stat,com.c_str());
  }

  return(0);
}
