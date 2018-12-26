/*************************************************************************
* skineffect.c
*
* array pre-compiled class library is needed to run 
* this demo program.
*************************************************************************/
#include <array.h>
#include <constants.h>

//
// e = ( 1 - É”( É…*x/2/sqrt(t-x/v0) ) )* E
//
//  É”(x) = 2/sqrt(pi) * ÅÁ0->x exp(-u*u) du
//
// v0 = 1/sqrt(L*C)
// Z  = sqrt(L*C)
// É… = (1/2/pi/r*sqrt(É /É–)+1/2/pi*sqrt(É 1/É–e) ) / 2/Z
// É   : ì±ëÃÇÃìßé•ó¶
// É 1 : ãÛä‘ÇÃìßé•ó¶
// É–  : ì±ëÃÇÃì±ìdó¶
// É–e : ì∫î≈ÇÃì±ìdó¶
// L   = É /2/pi * log(2*h/r)
// C   = 2*pi*É√r*É√0/log(2*h/r)
//
// É√r= 1.438
//  C = 40pF
//  L = 400nH
//  É 1 = É  = 4*pi*10e-7
//  É– = É–e = 5.5e7
//  r  = 0.3mm
//  h = 1.108mm
//  x = 5m
//

double s=5.8e7;  // conductivity
double se = s;
double r=0.3e-3; // radius
double h=1.108e-3; // height  l=r/2*exp(2*PI*l/U0)
double e0 = 8.82e-12;
double er = 1.438; 
double u1 = 4*PI*10e-7;
double u = u1*1;
double x=5; // cable length

#ifdef G__ROOT
void skineffect() { main(); }
#endif

double PI = 3.1415;
double L = u/2/PI*log(2*h/r);
double C = 2*PI*er*e0/log(2*h/r);
double Z = sqrt(L*C);
double v0 = 1/sqrt(L*C);
double lamda = (1.0/2/PI/r*sqrt(u/s)+1.0/2/PI*sqrt(u1/se) ) / 2.0/Z;

array phai;
array phaiparam;

array t(1e-13, 10e-9, 200);
array phaiin = lamda * x/2/sqrt(t);
array e;

double phai(double x) {
  array U(0.0,x,101);
  array r1 = exp(-U*U);
  array r2 = integ(U,r1);
  double r = r2[100]*2/sqrt(PI);
  return(r);
}

int main()
{
  //cerr << "Not ready yet\n" ;

  int i;
  for(i=0;i<t.size();i++) {
    e[i] = 1-phai(phaiin[i]);
  }

  plot << "skin effect" << t << "time" << e << "time\n";
  

  return 0;
}


