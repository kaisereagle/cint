

template<class T>
class smart_ptr {
 public:
  smart_ptr(T* x=0) : ptr(x) { }
  T* ptr;
  T* operator->() { return ptr; }  
};

#include <string>
using namespace std;

class String {
  char buf[100];
 public:
  String(const char* x="") { strcpy(buf,x); }
  const char* c_str() const { return(buf); }
};

#ifdef __MAKECINT__
#pragma link C++ class smart_ptr<string>;
#pragma link C++ class smart_ptr<String>;
#endif



