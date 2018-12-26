// ptlib.h  : Multi thread demo program using pthread
//
//  Usage:
//   makecint -mk makethread -dl thread.dll -H ptlib.h
//   make -f makethread
//   cint ptmain.cxx

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *thread1(void* arg) {
  int n = 1000000;
  for(int i=0;i<10*n;i++) {
    if(i%n==0) {
      printf("%s i=%d\n",(char*)arg,i);
    }
  }
  pthread_exit(NULL);
  return(0);
}

void *thread2(void* arg) {
  int n = 1000000;
  for(int i=0;i<10*n;i++) {
    if(i%n==0) {
      printf("%s i=%d\n",(char*)arg,i);
    }
  }
  return(0);
}

void test2(int n) {
  pthread_t thread[2];
  pthread_attr_t attr;
  void *arg = (void*)"sub thread";
  int stat[2];

  stat[0] = pthread_create(&thread[0],NULL,thread1,arg);
  stat[1] = pthread_create(&thread[1],NULL,thread1,arg);
  printf("stat=%d\n",stat);
  //if(stat==EAGAIN) printf("EAGAIN\n");
  //if(stat==EPARM) printf("EPARM\n");
  //if(stat==EINVAL) printf("EINVAL\n");
  pthread_join(thread[0],0);
  pthread_join(thread[1],0);
}


void test3(int n) {
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


