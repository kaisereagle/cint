// Value.h

#ifndef VALUE_H
#define VALUE_H

#include <string>
#include <cstdio>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <cstring>
using namespace std;

#define DOUBLEQUOTEESCAPE
//#define DOUBLEQUOTEESCAPE2

///////////////////////////////////////////////////
#if 1

typedef int Result;

#else

class Result {
 public:
  Result(const Result& x) { }
};

#endif

///////////////////////////////////////////////////
bool isdbl(const string& s) ;
bool isint(const string& s) ;

///////////////////////////////////////////////////
class value {
 public:
  string s;
  //const char*   cstr;
  double d;
  unsigned int count;

  size_t size() const { return(s.size()); }
  void init() { s=""; d=0.0; count=0; /*cstr=s.c_str();*/ }
  void set(double x) ; 
  void set(const char* x) ;
  void set(const string& x) ;
  void set(const value& v) { s=v.s; d=v.d; count=v.count; /*cstr=s.c_str();*/ }
  //void setvoid(void* x) { cstr = (char*)x; }
  //void* getvoid() { return((void*)cstr); }

  value() { set(""); }
  value(double x) {set(x);}
  value(const string& x) { set(x); };
  value(const char* x) { set(string(x)); };
  value(const value& x) { s=x.s; d=x.d; count=x.count; /*cstr=x.cstr;*/ }
  value& operator=(const value& x) { s=x.s; d=x.d; return(*this);/* cstr=x.cstr;*/ }
  bool isdouble() const {return(::isdbl(s));}
  bool isint() const {return(::isint(s));}

  int toint() const { return(int(d)); }
  int tolong() const { return(long(d)); }
  double& todouble() { return(d); }
  double todouble() const { return(d); }
  double tofloat() const { return((float)d); }
  string& tostring() { return(s); }
  const string& tostring() const { return(s); }
  const char* c_str() const { return(s.c_str()); }
  //const char* c_str() const { return(cstr); }
  int getcount() {return(count); }

  //operator double() const { return(d); }
  //operator string() const { return(s); }

  void add(double x) { set(d+x); }

  void increment() {++count;}
  value& operator++() {
    ++count;
    return(*this);
  }
  value& operator--() { 
    --count;
    return(*this);
  }
  void disp(FILE* fp=stdout) const ;
  void PrintDoubleQuote(FILE* fp,const string& s) const;
};

value operator+(const value& a,const value& b) ;
value operator-(const value& a,const value& b) ;
value operator*(const value& a,const value& b) ;
value operator/(const value& a,const value& b) ;
bool operator==(const value& a,const value& b) ;
bool operator!=(const value& a,const value& b) ;
bool operator<(const value& a,const value& b) ;
bool operator>(const value& a,const value& b) ;

inline int G__ateval(const value& a) { a.disp(); printf("\n"); return(0);}


#endif
