
#include <CSV.h>

////////////////////////////////////////////////////////
//Sheet s;
StrCollection c;
Collection t,t2,t3,u,u2,u3;
int main(int argc,char** argv) {
  for(int i=1;i<argc;i++) {
    csvPrompt(argv[i]);
  }
  while(!G__pause());
  return(0);
}

////////////////////////////////////////////////////////
void csvPrompt(char* fname) {
//printf("%s\n",fname);
  c.setquotation("\"");
  c.setseparator("");
  c.init(fname);
  //while(!G__pause());
}

////////////////////////////////////////////////////////
void save(char* fname) {
  FILE* fp = EX_fopen(fname,"w");
  c.disp(fp);
  EX_fclose(fp);
  printf("result wrote to %s\n",fname);
}

