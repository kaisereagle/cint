
// Linux2.0 egcs : g++ -lpthread pthread1.cxx
// HP-UX aCC     : aCC -D_POSIX_C_SOURCE=199506L -lpthread pthread1.cxx


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "thread.dll"

#if 0
void *thread2(void* arg) {
  int n = 1000000;
  for(int i=0;i<10*n;i++) {
    if(i%n==0) {
      printf("%s i=%d\n",(char*)arg,i);
    }
  }
  return(0);
}
#endif

pthread_t test(int n) {
  pthread_t thread;
  pthread_attr_t attr;
  void *arg = (void*)"sub thread";

  int stat = pthread_create(&thread,NULL,thread1,arg);
  printf("stat=%d\n",stat);
  //if(stat==EAGAIN) printf("EAGAIN\n");
  //if(stat==EPARM) printf("EPARM\n");
  //if(stat==EINVAL) printf("EINVAL\n");
  return(thread);
}

void test4(int n) {
  pthread_t thread[2];
  pthread_attr_t attr;
  void *arg = (void*)"sub thread";
  int stat[2];

  stat[0] = pthread_create(&thread[0],NULL,thread2,arg);
  stat[1] = pthread_create(&thread[1],NULL,thread2,arg);
  printf("stat=%d\n",stat);
  //if(stat==EAGAIN) printf("EAGAIN\n");
  //if(stat==EPARM) printf("EPARM\n");
  //if(stat==EINVAL) printf("EINVAL\n");
  pthread_join(thread[0],0);
  pthread_join(thread[1],0);
}


int main(int argc, char** argv) {
  int n=100;
  if(argc>1) n=atoi(argv[1]);
  test2(100);
  test3(100);
  //test4(100);
#if 0
  thread2((void*)"main thread1");
  pthread_t pthread = test(n);
  thread2((void*)"main thread2");
  pthread_join(pthread,0);
#endif
  return(0);
}


