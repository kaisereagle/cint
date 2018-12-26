
#include <CSV.h>

////////////////////////////////////////////////////////
//Sheet s;
FileStrCollection c;
Collection t,t2,t3,u,u2,u3;
Line key,line;
int main(int argc,char** argv) {
  if(argc==2) csvPrompt(argv[1]);
  else if(argc==3) csvPrompt(argv[1],atoi(argv[2]));
  while(!G__pause());
  return(0);
}

////////////////////////////////////////////////////////
void csvPrompt(char* fname,int key=0) {
#if 1
  c.init(fname,key);
#else
  c.setquotation("\"");
  c.setseparator("");
  Scandir d(fname);
  char* name;
  int flag=0;
  while((name=d.Next())&&name[0]) { // THIS IS DONE inside FileStrCollection
    if(!flag++) c.init(name,key);
    else        c.addfiles(name);
  }
#endif
}

////////////////////////////////////////////////////////
void save(char* fname) {
  FILE* fp = EX_fopen(fname,"w");
  c.disp(fp);
  EX_fclose(fp);
  printf("result wrote to %s\n",fname);
}

////////////////////////////////////////////////////////
void NextFile() {
  Line l;
  printf("file = %s\n",c.fname());
  while(c.read(l) && !c.isendoffile()) {
    l.disp();
  }
}

////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
// foreach action example
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
class Action {
 public:
  Action() {line=sum=0;}
  int operator()(const Line& l) {
    ++line;
    sum += l.size();
    return(1);
  }
  void clear() {line=sum=0;}
  int line; int sum;
} ac;

////////////////////////////////////////////////////////
int action(const Line& l) {
  static int sum,nline;
  ++nline;
  sum += l.size();
  return(sum);
}
////////////////////////////////////////////////////////
