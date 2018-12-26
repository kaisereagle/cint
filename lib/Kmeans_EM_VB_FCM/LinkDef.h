
#include "Kmeans_EM_VB_FCM.h"

#ifdef __MAKECINT__

#pragma link off all functions;
#pragma link off all classes;
#pragma link off all globals;


typedef Vector<double>  DVector;
typedef Vector<DVector> DMatrix;
typedef Vector<int>  IVector;
typedef Vector<IVector> IMatrix;
typedef EM<double>      DEM;

#pragma link C++ all typedef;

#pragma link C++ class Vector<double>;
#pragma link C++ class Vector<DVector>;
#pragma link C++ class Vector<int>;
#pragma link C++ class Vector<IVector>;
#pragma link C++ class EM<double>;

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

#pragma link C++ function distance2(const DVector&,const DVector&);
#pragma link C++ function distance1(const DVector&,const DVector&);

#pragma link C++ function findpos(const DVector&,double);
#pragma link C++ function range(const DVector&,unsigned int,unsigned int);
#pragma link C++ function linear(double,double,unsigned int);

#pragma link C++ function RandomEqualdouble(double,double);
#pragma link C++ function RandomGauss(double,double);

#pragma link C++ function Inverse(const DMatrix&):
#pragma link C++ function Determinant(const DMatrix&):

#pragma link C++ function Product__(const DVector&,const DVector&);
#pragma link C++ function Product_T(const DVector&,const DVector&);
#pragma link C++ function ProductT_(const DVector&,const DVector&);
#pragma link C++ function ProductTT(const DVector&,const DVector&);
//#pragma link C++ function InnerProduct(const DVector&,const DVector&);
#pragma link C++ function Product(const DVector&,const DMatrix&);
#pragma link C++ function ProductT(const DVector&,const DMatrix&);
#pragma link C++ function Product(const DMatrix&,const DVector&);
#pragma link C++ function ProductT(const DMatrix&,const DVector&);

//#pragma link C++ function InnerProduct(const DMatrix&,const DMatrix&);
#pragma link C++ function SimpleProduct(const DMatrix&,const DMatrix&);
#pragma link C++ function SimpleDivide(const DMatrix&,const DMatrix&);
#pragma link C++ function Add(const DMatrix&,const DMatrix&);
#pragma link C++ function Subtract(const DMatrix&,const DMatrix&);

#endif
