
#include "Vector.h"
#include "Complex.h"

typedef Complex<double> DComplex;
typedef Vector<double> DVector;
typedef Vector<DComplex> DCVector;

//typedef Complex<float> FComplex;
//typedef Vector<float> FVector;
//typedef Vector<FComplex> FCVector;

#ifdef __MAKECINT__

#pragma link off all functions;
#pragma link off all classes;
#pragma link off all globals;

#pragma link C++ all typedef;

#pragma link C++ class Complex<double>;


#pragma link C++ class Vector<double>;
#pragma link C++ class Vector<DComplex>;

//#pragma link C++ class Complex<float>;
//#pragma link C++ class Vector<float>;
//#pragma link C++ class Vector<FComplex>;
//#pragma link C++ class Complex<int>;
//#pragma link C++ class Vector<int>;
//#pragma link C++ class Vector<int>;

#pragma link C++ function operator==(const DVector&,const DVector&);
#pragma link C++ function operator!=(const DVector&,const DVector&);
#pragma link C++ function operator>(const DVector&,const DVector&);
#pragma link C++ function operator<(const DVector&,const DVector&);
#pragma link C++ function operator>=(const DVector&,const DVector&);
#pragma link C++ function operator<=(const DVector&,const DVector&);

#pragma link C++ function operator+(const DVector&,const DVector&);
#pragma link C++ function operator-(const DVector&,const DVector&);
#pragma link C++ function operator*(const DVector&,const DVector&);
#pragma link C++ function operator/(const DVector&,const DVector&);
#pragma link C++ function operator+(const DVector&,double);
#pragma link C++ function operator-(const DVector&,double);
#pragma link C++ function operator*(const DVector&,double);
#pragma link C++ function operator/(const DVector&,double);

#pragma link C++ function exp(const DComplex&);
#pragma link C++ function rotate(const DComplex&,double);
#pragma link C++ function abs(const DComplex&);

#pragma link C++ function sin(const DVector&);
#pragma link C++ function cos(const DVector&);
#pragma link C++ function tan(const DVector&);
#pragma link C++ function asin(const DVector&);
#pragma link C++ function acos(const DVector&);
#pragma link C++ function atan(const DVector&);
#pragma link C++ function abs(const DVector&);
#pragma link C++ function fabs(const DVector&);
#pragma link C++ function sum(const DVector&);
#pragma link C++ function accum(const DVector&);
#pragma link C++ function rect(const DVector&);
#pragma link C++ function square(const DVector&);
#pragma link C++ function pulse(const DVector&);
#pragma link C++ function rand(const DVector&);
#pragma link C++ function pow(const DVector&,const DVector&);
#pragma link C++ function pow(const DVector&,double);
#pragma link C++ function conv(const DVector&,const DVector&);
#pragma link C++ function integ(const DVector&,const DVector&);
#pragma link C++ function diff(const DVector&,const DVector&);
#pragma link C++ function gauss(const DVector&,const DVector&);
#pragma link C++ function movingave(const DVector&,int,int);

#pragma link C++ function fir(const DVector&,const DVector&);
#pragma link C++ function iir1(const DVector&,double,double);

#pragma link C++ function min(const DVector&,const DVector&);
#pragma link C++ function max(const DVector&,const DVector&);
#pragma link C++ function min(const DVector&,double);
#pragma link C++ function max(const DVector&,double);

#pragma link C++ function mean(const DVector&);
#pragma link C++ function max(const DVector&);
#pragma link C++ function min(const DVector&);
#pragma link C++ function stddev(const DVector&);
#pragma link C++ function rms(const DVector&);
#pragma link C++ function pp(const DVector&);

#pragma link C++ function findpos(const DVector&,double);
#pragma link C++ function range(const DVector&,int,int);
#pragma link C++ function linear(double,double,int);

#endif
