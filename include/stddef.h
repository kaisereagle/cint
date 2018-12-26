#ifndef G__STDDEF_H
#define G__STDDEF_H
#if (G__GNUC==2)
typedef long ptrdiff_t;
#else
typedef long ptrdiff_t;
#endif
typedef unsigned long size_t;
typedef unsigned int wchar_t;
#endif
