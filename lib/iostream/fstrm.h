
/* /% C++ %/ */
/***********************************************************************
 * cint (C/C++ interpreter)
 ************************************************************************
 * header file fstrm.h
 ************************************************************************
 * Description:
 *  Stub file for making iostream library
 ************************************************************************
 * Copyright(c) 1996       Osamu Kotanigawa
 * Author                  Osamu Kotanigawa
 *
 ************************************************************************/

#ifndef G__FSTRM
#define G__FSTRM

#ifndef __MAKECINT__

#include <fstream>
using namespace std;

#else 

#include "iostrm.h"

class  filebuf : public streambuf {	
  /* a stream buffer for files */
public:
  static const int openprot ; /* default protection for open */
public:
  filebuf() ;
  filebuf(int fd);
  filebuf(int fd, char*  p, int l) ;
  
  int		is_open();
  int		fd();
  filebuf*	open(const char *name, int om, int prot=openprot);
  filebuf*	attach(int fd) ;
  // int		detach() ;
  filebuf* 	close() ;
  ~filebuf() ;
 public: /* virtuals */
  virtual int	overflow(int=EOF);
  virtual int	underflow();
  virtual int	sync() ;
  virtual streampos seekoff(streamoff,ios::seek_dir,int) ;
  virtual streambuf* setbuf(char*  p, int len) ;
};
/////////////////////////////////////////////////////////////////////////
class ifstream : public istream {
 public:
  ifstream() ;
#if defined(G__APPLE)
  ifstream(const char* name, int mode=ios::in) ;
#endif
#if defined(G__WIN32)
  ifstream(const char* name, int mode=ios::in, int prot=filebuf::openprot) ;
#endif
  //ifstream(int fd) ;
  //ifstream(int fd, char*  p, int l) ;
  ~ifstream() ;

  //streambuf * setbuf(char *, int);
  filebuf* rdbuf() const ;

  //void attach(int);
  //int fd() const ;

  int is_open() const ;
#if defined(G__APPLE)
  void open(const char *, int =ios::in);
#endif
#if defined(G__WIN32)
  void open(const char *, int =ios::in, int = filebuf::openprot);
#endif
  void close();
//  int setmode(int mode = filebuf::text) ;
} ;

/////////////////////////////////////////////////////////////////////////
class ofstream : public ostream {
public:
  ofstream() ;
#if defined(G__APPLE)
  ofstream(const char* name, int mode=ios::out) ;
#endif
#if defined(G__WIN32)
  ofstream(const char* name, int mode=ios::out, int prot=filebuf::openprot) ;
#endif
  //ofstream(int fd) ;
  //ofstream(int fd, char*  p, int l) ;
  ~ofstream() ;

  //streambuf * setbuf(char *, int);
  filebuf* rdbuf() const ;

  //void attach(int);
  //int fd() const ;

  int is_open() const ;
#if defined(G__APPLE)
  void open(const char *, int =ios::out);
#endif
#if defined(G__WIN32)
  void open(const char *, int =ios::out, int = filebuf::openprot);
#endif
  void close();
//  int setmode(int mode = filebuf::text) ;
} ;

class fstream : public iostream {
public:
  fstream() ;
	
  fstream(const char* name, int mode, int prot=filebuf::openprot) ;
  fstream(int fd) ;
  fstream(int fd, char*  p, int l) ;
  ~fstream() ;

  streambuf * setbuf(char *, int);
  filebuf* rdbuf() const ;

  void attach(int);
  int fd() const ;

  int is_open() const ;
  void open(const char *, int, int = filebuf::openprot);
  void close();
//  int setmode(int mode = filebuf::text) ;
} ;

#endif // __MAKECINT__

#endif // G__FSTRM


