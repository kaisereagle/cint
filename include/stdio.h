#ifndef G__STDIO_H
#define G__STDIO_H
#ifndef NULL
#pragma setstdio
#endif
typedef long fpos_t;
typedef unsigned long size_t;
#define 	_IOFBF (0)
#define 	_IOLBF (1)
#define 	_IONBF (2)
#define 	BUFSIZ (1024)
#define 	FILENAME_MAX (1024)
#define 	L_tmpnam (1024)
#define 	TMP_MAX (308915776)
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
