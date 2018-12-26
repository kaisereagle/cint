#include <time.h>

#include <string>

int main(int argc,char** argv) {
  string com;
  for(int i=1;i<argc;i++) {
    com += argv[i];
    com += " ";
  }

  time_t t1,t2;
  time(&t1);
  int stat = system(com.c_str());
  time(&t2);

  double dt = difftime(t2,t1);

#ifdef G__NEVER
  printf("%s",ctime(t1));
  printf("%s",ctime(t2));
#endif
  printf("%g sec\n",dt);

  return(stat);
}
