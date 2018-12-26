
#include <CSV.h>

#ifdef G__WIN32
const char* cp="copy";
const char  sep='\\';
#else
const char* cp="cp";
const char  sep='/';
#endif

int main(int argc,char** argv) {
  if(argc<2) {
    fprintf(stderr,"Usage: _copyfile [SourceExp] [DestinationDirectory]\n");
    return(255);
  }
  Scandir d(argv[1]);
  char com[500];
  string orig;
  string fname;
  size_t pos;
  orig=d.next();
  while(orig.size()) {
    pos = orig.find_last_of(sep);
    if(pos==string::npos) fname=orig;
    else  fname = orig.substr(pos+1,string::npos);
    sprintf(com,"%s \"%s\" \"%s%c%s\"",cp,orig.c_str(),argv[2],sep,fname.c_str());
    printf("%s\n",com);
    system(com);
    orig=d.next();
  }
  return(0);
}
