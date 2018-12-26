#ifndef G__SYSTYPES_H
#define G__SYSTYPES_H
typedef int ssize_t;
typedef int pid_t;
typedef unsigned int pid_t;
typedef void* ptr_t;
typedef unsigned long dev_t;
typedef unsigned long gid_t;
typedef unsigned long uid_t;
typedef unsigned short mode_t;
typedef struct off_t {
  long l,u;
  off_t(long i){l=i;u=0;}
  void operator=(long i){l=i;u=0;}
} off_t;
#pragma link off class off_t;
#pragma link off typedef off_t;
typedef unsigned long ino_t;
typedef unsigned short nlink_t;
typedef unsigned short ushort;
typedef int key_t;
#endif
