/* /% C++ %/ */
/***********************************************************************
 * cint (C/C++ interpreter)
 ************************************************************************
 * header file iostrm.h
 ************************************************************************
 * Description:
 *  iostream library generator for gcc and VC++
 ************************************************************************
 * Copyright(c) 1991~2015,  Masaharu Goto (gotom@hanno.jp)
 *
 ************************************************************************/

#ifndef G__IOSTRM
#define G__IOSTRM

#ifndef __MAKECINT__

#include <iostream>
using namespace std;

#else

typedef long streampos ;
typedef long streamoff ;

class ostream;
class streambuf;
/////////////////////////////////////////////////////////////////
class ios_base {
 protected:
  ios_base();
  ios_base(const ios_base& a);
};
/////////////////////////////////////////////////////////////////
class ios : public ios_base {
 public: /* Some enums are declared in ios to avoid pollution of
	  * global namespace
	  */
  enum io_state	{ goodbit=0, eofbit=1, failbit=2, badbit=4, 
		    hardfail=0200};
  /* hard fail can be set and reset internally,
   * but not via public function */
  enum open_mode	{ in=1, out=2, ate=4, app=010, trunc=020,
			    nocreate=040, noreplace=0100, binary=0x80} ;
  enum seek_dir	{ beg=0, cur=1, end=2 } ;
  
  /* flags for controlling format */
  enum		{ skipws=01,	
		    /* skip whitespace on input */
		    left=02,  right=04, internal=010,
		    /* padding location */
		    dec=020, oct=040, hex=0100, 
		    /* conversion base */
		    showbase=0200, showpoint=0400, uppercase=01000,
		    showpos=02000, 
		    /* modifiers */
		    scientific=04000, fixed=010000,
		    /* floating point notation */
		    unitbuf=020000, stdio=040000
		      /* stuff to control flushing */
                    , boolalpha = 100000 , adjustfield = 14
                    , basefield = 112 , floatfield = 6144
		    } ;
  // static const long basefield ; /* dec|oct|hex */
  // static const long adjustfield ; /* left|right|internal */
  // static const long floatfield ; /* scientific|fixed */

 protected:
  ios(streambuf*) ;
  virtual ~ios() ;

 public:
  long		flags() const;
#if !defined(G__LINUX)
  long		flags(long f);

  long		setf(long setbits, long field);
  long		setf(long) ;
#endif
  //long		unsetf(long) ;
  
  int		width() const;
  int		width(int w);
		
  ostream*	tie(ostream* s); 
  ostream*	tie();	
  char		fill(char) ;
  char		fill() const;
  int		precision(int) ;
  int		precision() const;

  int		rdstate() const;
  //operator void*();
  // operator const void*() const;
  int	operator!() const;
  int		eof() const;
  int		fail() const;
  int		bad() const;
  int		good() const;
#if definedG__LINUX()
  void		clear() ;
#endif
#if defined(G__WIN32) || defined(G__APPLE)
  void		clear(int i =0) ;
#endif
  streambuf*	rdbuf() ;

public: /* Members related to user allocated bits and words */
  long &		iword(int) ;
  void* &		pword(int) ;
  //static long	bitalloc() ;
  static int	xalloc() ;

private: /*** privates for implemting allocated bits and words */ 
  static long	nextbit ;
  static long	nextword ;
	
  int		nuser ;
  // union ios_user_union* x_user ;
  void	uresize(int) ;
public: /* static member functions */
  static void	sync_with_stdio() ;
protected:
  /* Does the real work of a constructor */
  ios() ; /* No initialization at all. Needed by
	   * multiple inheritance versions */
  int		assign_private ;
  /* needed by with_assgn classes */
 private:		
  ios(ios&) ; /* Declared but not defined */
  void		operator=(ios&) ; /* Declared but not defined */
 public:   /* old stream package compatibility */
  //int		skip(int i) ; 
};
/////////////////////////////////////////////////////////////////
class streambuf {
 public: 
  virtual int	overflow(int c=EOF);
  virtual int	underflow();
  virtual int	pbackfail(int c);
  virtual int	sync();
  virtual streampos seekoff(streamoff,ios::seek_dir,int =ios::in|ios::out);
  virtual streampos seekpos(streampos, int =ios::in|ios::out) ;
  virtual int	xsputn(const char*  s,int n);
  virtual int	xsgetn(char*  s,int n);

  int		in_avail();
  int		out_waiting() ;
  int		sgetc();
  int		snextc();
  int		sbumpc();
  // int		optim_in_avail();
  // int		optim_sbumpc();
  void		stossc();
  int		sputbackc(char c);
  int		sputc(int c);
  int		sputn(const char*  s,int n);
  int		sgetn(char*  s,int n);
  // virtual streambuf* setbuf(char*  p, int len) ;
  // streambuf*	setbuf(unsigned char*  p, int len) ;
  // streambuf*	setbuf(char*  p, int len, int count) ;
  /* obsolete third argument */
  /*** Constructors -- should be protected ***/
 protected:
  streambuf() ;
 private:
  streambuf(streambuf&) ;
 public:
  // streambuf(char*  p, int l) ;

  // streambuf(char*  p, int l,int c) ; 
  /* 3 argument form is obsolete.
   * Use strstreambuf.
   */
  virtual		~streambuf() ;
};
/////////////////////////////////////////////////////////////////
class istream : virtual public ios {
public: /* Constructor */
  istream(streambuf*) ;
  virtual	~istream() ;
public:	
  //int		ipfx(int noskipws /* =0 */);
#if !defined(G__APPLE) && !defined(G__LINUX)
  void		isfx() { }  
#endif
  //istream&	seekg(streampos p) ;
  //istream&	seekg(streamoff o, ios::seek_dir d) ;
  //streampos	tellg() ; 
#ifdef G__MANIP_SUPPORT
  istream&	operator>> (istream& (*f)(istream&));
  istream&	operator>> (ios& (*f)(ios&) ) ;
#endif
#if !defined(G__WIN32) && !defined(G__APPLE) && !defined(G__LINUX)
  istream&	operator>>(char*);
  istream&	operator>>(unsigned char*);
  istream&	operator>>(unsigned char& c);
  istream&	operator>>(char& c);
#endif
  // istream&	rs_complicated(unsigned char& c);
  // istream&	rs_complicated(char& c);
  istream&	operator>>(short&);
  istream&	operator>>(int&);
  istream&	operator>>(long&);
  istream&	operator>>(unsigned short&);
  istream&	operator>>(unsigned int&);
  istream&	operator>>(unsigned long&);
  istream&	operator>>(float&);
  istream&	operator>>(double&);
  //istream&	operator>>(streambuf*);
  istream&	get(char* , int lim, char delim='\n');
  // istream&	get(unsigned char* b,int lim, char delim='\n');
  istream&	getline(char* b, int lim, char delim='\n');
  //istream&	getline(unsigned char* b, int lim, char delim='\n');
  //istream&	get(streambuf& sb, char delim ='\n');
  // istream&	get_complicated(unsigned char& c);
  // istream&	get_complicated(char& c);
  //istream&	get(unsigned char& c);
  istream&	get(char& c);
  int 		get();
  int		peek() ;
  istream&	ignore(int n=1,int delim=EOF) ;
  istream&	read(char*  s,int n);
  // istream&	read(unsigned char* s,int n) ;
  int		gcount() ;
  istream&	putback(char c);
  // int		sync();

protected:  
  istream() ;

public: /*** Obsolete constructors, carried over from stream package ***/
#ifdef G__NEVER
  istream(streambuf*, int sk, ostream* t=0) ; /* obsolete, set sk and tie
					       * via format state variables */
  istream(int size ,char*,int sk=1) ; /* obsolete, use strstream */
  istream(int fd,int sk=1, ostream* t=0) ; /* obsolete use fstream */
#endif
};

class ostream : virtual public ios {
public: /* Constructor */
  ostream(streambuf*) ;
  virtual	~ostream();
public:	
#if !defined(G__APPLE) && !defined(G__LINUX)
  int		opfx();	/* Output prefix */
  void		osfx() ;
#endif

  ostream&	flush() ;
  //ostream&	seekp(streampos p) ;
  //ostream&	seekp(streamoff o, ios::seek_dir d) ;
  //streampos	tellp() ; 
  ostream&	put(char c);
  // ostream&	complicated_put(char c);
  ostream&	operator<<(char c);
  ostream&	operator<<(unsigned char c) ;
  // ostream& 	ls_complicated(char);
  // ostream& 	ls_complicated(unsigned char);

  ostream&	operator<<(const char*);
  ostream&	operator<<(int a); 
  ostream&	operator<<(long);	
  ostream&	operator<<(double);
  ostream&	operator<<(float);
  ostream&	operator<<(unsigned int a);
  ostream&	operator<<(unsigned long);
  ostream&	operator<<(void*);
  /*	ostream&	operator<<(const void*);   add this later */
  ostream&	operator<<(streambuf*);
  ostream&	operator<<(short i); 
  ostream&	operator<<(unsigned short i) ;

#ifdef G__MANIP_SUPPORT
  ostream&	operator<< (ostream& (*f)(ostream&));
  ostream&	operator<< (ios& (*f)(ios&) ) ;
#endif

  ostream&	write(const char*  s,int n)	;
  // ostream&	write(const unsigned char* s, int n);

public: /*** Obsolete constructors, carried over from stream package ***/
#ifdef G__NEVER
  ostream(int fd) ; /* obsolete use fstream */
  ostream(int size ,char*) ; /* obsolete, use strstream */
#endif
} ;

class iostream : public istream, public ostream {
public:
  iostream(streambuf*) ;
  virtual		~iostream() ;
protected:
  iostream() ;
} ;

/////////////////////////////////////////////////////////////////

extern istream cin ;
extern ostream cout ;
extern ostream cerr ;
extern ostream clog ;


#if defined(G__WIN32) || defined(G__APPLE) || defined(G__LINUX)
ios_base&	dec(ios_base&) ; 
ostream&	endl(ostream& i) ;
ostream&	ends(ostream& i) ;
ostream&	flush(ostream&) ;
ios_base&	hex(ios_base&) ;
ios_base&	oct(ios_base&) ; 
istream&	ws(istream&) ;
#else
ios&		dec(ios&) ; 
ostream&	endl(ostream& i) ;
ostream&	ends(ostream& i) ;
ostream&	flush(ostream&) ;
ios&		hex(ios&) ;
ios&		oct(ios&) ; 
istream&	ws(istream&) ;
#endif


#endif // __MAKECINT__ 


#endif // G__IOSTRM
