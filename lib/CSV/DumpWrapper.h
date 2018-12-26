////////////////////////////////////////////////////////////////////////////
// Dump wrapper 
// 2014, Mar 25
//   Just Idea, 
////////////////////////////////////////////////////////////////////////////

#ifndef DUMPWRAPPER
#define DUMPWRAPPER

#include <string>

template<class T>
class DumpWrapper {
 public:
  string name;
  T v;

  DumpWrapper(const string& namein,const T& vin) : name(namein), v(vin) {}
  T& operator=(const T& vin) {v=vin; dump(); return(v);}
  T& operator+=(const T& vin) {v+=vin; dump(); return(v);}
  T& operator-=(const T& vin) {v-=vin; dump(); return(v);}
  T& operator*=(const T& vin) {v*=vin; dump(); return(v);}
  T& operator/=(const T& vin) {v/=vin; dump(); return(v);}
  T& operator%=(const T& vin) {v%=vin; dump(); return(v);}

  T& T() { return(v); }

  void dump() const {
  }
};

T operator+(const T& a,const T& b) { return(a.v+b.v);}


#endif
