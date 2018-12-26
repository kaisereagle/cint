

#include <ReadF.h>
#include <unistd.h>

void batch(const char* fname) {
  ReadF f(fname);
  while(f.read()) {

    if(f.argc>1 && f.argv[0][0]!='#') {
      printf("%s\n",f.argv[0]);
      if(strcmp(f.argv[1],"cd")==0) chdir(f.argv[2]);
      else {
        system(f.argv[0]);
      }
    }
  }
}

int main(int argc,char* argv[]) {
  G__optimizemode(3);
  for(int i=1;i<argc;i++) {
    batch(argv[i]);
  }
}
