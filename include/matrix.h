
#include <complex.h>

class matrix22 {
 public:
  matrix22() { for(int i=0;i<4;i++) c[i] = 0; }
  matrix22(const matrix22& a) {for(int i=0;i<4;i++) c[i] = a.c[i];}
  matrix22& operator=(const matrix22& a) {
    for(int i=0;i<4;i++) c[i] = a.c[i];
    return(*this);
  }
  complex& operator()(int x,int y) { return(c[x*2+y]); }
  void disp() {for(int i=0;i<4;i++) { c[i].disp(); printf(" "); }}
  complex& A() { return(c[0]); }
  complex& B() { return(c[2]); }
  complex& C() { return(c[1]); }
  complex& D() { return(c[3]); }
 private:
  complex c[4];
};

class matrix2 {
 public:
  matrix2() { for(int i=0;i<2;i++) c[i] = 0; }
  matrix2(const matrix2& a) {for(int i=0;i<2;i++) c[i] = a.c[i];}
  matrix2& operator=(const matrix2& a) {
    for(int i=0;i<2;i++) c[i] = a.c[i];
    return(*this);
  }
  complex& operator()(int x) { return(c[x]); }
  void disp() {for(int i=0;i<2;i++) { c[i].disp(); printf(" "); }}
 private:
  complex c[2];
};

matrix2 operator*(const matrix22& a,const matrix2& b) {
  matrix2 r;
  r(0) = a(0,0)*b(0) + a(1,0)*b(1);
  r(1) = a(0,1)*b(0) + a(1,1)*b(1);
  return(r);
}

matrix22 inv(const matrix22& a) {
  matrix22 r;
  double d = det(a);
  r(0,0) = a(1,1)/d;  // A
  r(1,1) = a(0,0)/d;  // D
  r(1,0) = -a(1,0)/d; // B
  r(0,1) = -a(0,1)/d; // C
  return(r);
}

double det(const matrix22& a) {
  double r = a(0,0)*a(1,1) - a(1,0)*a(0,1);
  return(r);
}
