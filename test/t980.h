
#include <string>
#include <stdio.h>

class A {
  public:
    A(): mString() {}
    A(const A &s): mString(s.mString) {}
    A(const char *s): mString(s) {}
    virtual ~A() {}

    A operator +(const char *s)
      { A r = mString.c_str(); r.mString += s; return r; }

    const char *val() const { return mString.c_str(); }
    operator const char*() const { return mString.c_str(); }

  private:
    std::string mString;

    //ClassDef(A, 1)
};

void f(const char *a,const char *b) {
  printf("%s. %s.\n",a,b);
}


