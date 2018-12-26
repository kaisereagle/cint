#ifndef G__STDIO_H
#define G__STDIO_H
#ifndef NULL
#pragma setstdio
#endif
typedef struct fpos_t {
  long l,u;
  fpos_t(long i=0){l=i;u=0;}
  void operator=(long i){l=i;u=0;}
} fpos_t;
#pragma link off class fpos_t;
#pragma link off typedef fpos_t;
typedef unsigned int size_t;
#define 	_IOFBF (0)
#define 	_IOLBF (64)
#define 	_IONBF (4)
#define 	BUFSIZ (512)
#define 	FILENAME_MAX (260)
#define 	L_tmpnam (14)
#define 	TMP_MAX (32767)
#ifndef SEEK_CUR
#define 	SEEK_CUR (1)
#endif
#ifndef SEEK_END
#define 	SEEK_END (2)
#endif
#ifndef SEEK_SET
#define 	SEEK_SET (0)
#endif
#ifdef __cplusplus
#include <bool.h>
#endif
#pragma include_noerr <stdfunc.dll>
#endif
