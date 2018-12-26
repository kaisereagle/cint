
#include <stdio.h>

typedef long Long_t;
typedef unsigned long ULong_t;
typedef long long Long64_t;
typedef unsigned long long ULong64_t;
typedef long double Double92_t;
typedef double Double_t;

void test0() {
#if 0
  ULong_t a = dynamic_cast<ULong_t>(123);
  ULong64_t b = dynamic_cast<ULong64_t>(456);
  Double92_t c = dynamic_cast<Double92_t>(1.234);
#else
  ULong_t a = (ULong_t)(123);
  ULong64_t b = (ULong64_t)(456);
  Double92_t c = (Double92_t)(1.234);
  Double_t d = (Double_t)(5.678);
#endif
  printf("%lu %llu %LG %g\n",a,b,c,d);
  printf("%lu  %g\n",a,d);
  printf("%LG\n",c);
}

void test1() {
  ULong64_t aa;
  aa = 1<<31;         // ULong64_t aa = (ULong64_t)(-1) = 0xffffffffffffffff
  printf("%llu ",aa);
  aa = aa<<1;
  printf("%llu ",aa);
  aa = aa*2;
  printf("%llu ",aa);
  aa = aa/(1<<31);   // (ULong64_t)aa/(0x00000000ffffffff)
  printf("%llu\n",aa);
}

void test2() {
  ULong64_t aa;
  aa = 1ULL<<31;
  printf("%llu ",aa);
  aa = aa<<1;
  printf("%llu ",aa);
  aa = aa*2;
  printf("%llu ",aa);
  aa = aa/(1<<31);
  printf("%llu\n",aa);
}

void test3() {
  Long64_t aa;
  aa = 1<<31;
  printf("%lld ",aa);
  aa = aa<<1;
  printf("%lld ",aa);
  aa = aa*2;
  printf("%lld ",aa);
  aa = aa/(1LL<<31);
  printf("%lld\n",aa);
}

void test4() {
  Long64_t aa;
  aa = 1LL<<31;
  printf("%lld ",aa);
  aa = aa<<1;
  printf("%lld ",aa);
  aa = aa*2;
  printf("%lld ",aa);
  aa = aa/(1LL<<31);
  printf("%lld\n",aa);
}

int main() {
  test0();
  test1();
  test2();
  test3();
  test4();
  return 0;
}
