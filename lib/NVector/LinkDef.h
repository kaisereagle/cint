
#include "Fft.h"

#ifdef __MAKECINT__

#pragma link off all functions;
#pragma link off all classes;
#pragma link off all globals;

#pragma link C++ all typedef;

#pragma link C++ global G__NVECTOR;

///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
#pragma link C++ class Complex<double>;

#pragma link C++ class Vector<double>;
#pragma link C++ class Vector<DComplex>;

#pragma link C++ class Complex<float>;
#pragma link C++ class Vector<float>;
#pragma link C++ class Vector<FComplex>;
#pragma link C++ class Complex<int>;
#pragma link C++ class Vector<int>;
#pragma link C++ class Vector<int>;

#pragma link C++ class Vector<DVector>;
#pragma link C++ class Vector<FVector>;
#pragma link C++ class Vector<IVector>;

#pragma link C++ class DMatrix;
#pragma link C++ class FMatrix;
#pragma link C++ class IMatrix;


///////////////////////////////////////////////////////////////////
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

#pragma link C++ function operator+(const FVector&,const FVector&);
#pragma link C++ function operator-(const FVector&,const FVector&);
#pragma link C++ function operator*(const FVector&,const FVector&);
#pragma link C++ function operator/(const FVector&,const FVector&);
#pragma link C++ function operator+(const FVector&,double);
#pragma link C++ function operator-(const FVector&,double);
#pragma link C++ function operator*(const FVector&,double);
#pragma link C++ function operator/(const FVector&,double);

#pragma link C++ function innerproduct(const DVector&,const DVector&);
#pragma link C++ function normalizedinnerproduct(const DVector&,const DVector&);
#pragma link C++ function pearsoncorrelation(const DVector&,const DVector&);
#pragma link C++ function cosinesimilarity(const DVector&,const DVector&);
#pragma link C++ function tanimotocoefficient(const DVector&,const DVector&);

///////////////////////////////////////////////////////////////////

#pragma link C++ function exp(const DComplex&);
#pragma link C++ function rotate(const DComplex&,double);
#pragma link C++ function abs(const DComplex&);

#pragma link C++ function exp(const DVector&);
#pragma link C++ function log(const DVector&);
#pragma link C++ function log10(const DVector&);
#pragma link C++ function sqrt(const DVector&);
#pragma link C++ function sinc(const DVector&);
#pragma link C++ function sin(const DVector&);
#pragma link C++ function cos(const DVector&);
#pragma link C++ function tan(const DVector&);
#pragma link C++ function asin(const DVector&);
#pragma link C++ function acos(const DVector&);
#pragma link C++ function atan(const DVector&);
#pragma link C++ function abs(const DVector&);
#pragma link C++ function fabs(const DVector&);
#pragma link C++ function sum(const DVector&);
#pragma link C++ function sum2(const DVector&);
#pragma link C++ function accum(const DVector&);
#pragma link C++ function rect(const DVector&);
#pragma link C++ function triangle(const DVector&);
#pragma link C++ function square(const DVector&);
#pragma link C++ function pulse(const DVector&);
#pragma link C++ function rand(const DVector&);
#pragma link C++ function pow(const DVector&,const DVector&);
#pragma link C++ function pow(double,const DVector&);
#pragma link C++ function pow(const DVector&,double);
#pragma link C++ function conv(const DVector&,const DVector&);
#pragma link C++ function integ(const DVector&,const DVector&);
#pragma link C++ function integ(const DVector&);
#pragma link C++ function diff(const DVector&,const DVector&);
#pragma link C++ function diff(const DVector&);
#pragma link C++ function gauss(const DVector&,const DVector&);
#pragma link C++ function movingave(const DVector&,int,int);
#pragma link C++ function resamplePowerOf2(const DVector&,const DVector&,DVector&,DVector&,double,double);

#pragma link C++ function fir(const DVector&,const DVector&);
#pragma link C++ function iir1(const DVector&,double,double);

#pragma link C++ function min(const DVector&,const DVector&);
#pragma link C++ function max(const DVector&,const DVector&);
#pragma link C++ function min(const DVector&,double);
#pragma link C++ function max(const DVector&,double);

#pragma link C++ function indexmin(const DVector&);
#pragma link C++ function indexmax(const DVector&);
#pragma link C++ function indexrange(const DVector&,double,double);
#pragma link C++ function peak(const DVector&,DVector&,IVector&,const double&,unsigned int);
#pragma link C++ function transition(const Vector<double>&,Vector<int>&,const double&,int); 

#pragma link C++ function mean(const DVector&);
#pragma link C++ function max(const DVector&);
#pragma link C++ function min(const DVector&);
#pragma link C++ function argmax(const DVector&);
#pragma link C++ function argmin(const DVector&);
#pragma link C++ function stddev(const DVector&);
#pragma link C++ function rms(const DVector&);
#pragma link C++ function pp(const DVector&);

#pragma link C++ function distance2(const DVector&,const DVector&);
#pragma link C++ function distance1(const DVector&,const DVector&);

#pragma link C++ function findpos(const DVector&,double);
#pragma link C++ function range(const DVector&,unsigned int,unsigned int);
#pragma link C++ function linear(double,double,unsigned int);

#pragma link C++ function gauss(double,double,double);
#pragma link C++ function gauss(const DVector&,double,double);
//#pragma link C++ function gauss(const DVector&,const DVector&,const DVector&);

///////////////////////////////////////////////////////////////////
#pragma link C++ function operator==(const FVector&,const FVector&);
#pragma link C++ function operator!=(const FVector&,const FVector&);
#pragma link C++ function operator>(const FVector&,const FVector&);
#pragma link C++ function operator<(const FVector&,const FVector&);
#pragma link C++ function operator>=(const FVector&,const FVector&);
#pragma link C++ function operator<=(const FVector&,const FVector&);

#pragma link C++ function operator+(const FVector&,const FVector&);
#pragma link C++ function operator-(const FVector&,const FVector&);
#pragma link C++ function operator*(const FVector&,const FVector&);
#pragma link C++ function operator/(const FVector&,const FVector&);
#pragma link C++ function operator+(const FVector&,float);
#pragma link C++ function operator-(const FVector&,float);
#pragma link C++ function operator*(const FVector&,float);
#pragma link C++ function operator/(const FVector&,float);

#pragma link C++ function operator+(const FVector&,const FVector&);
#pragma link C++ function operator-(const FVector&,const FVector&);
#pragma link C++ function operator*(const FVector&,const FVector&);
#pragma link C++ function operator/(const FVector&,const FVector&);
#pragma link C++ function operator+(const FVector&,float);
#pragma link C++ function operator-(const FVector&,float);
#pragma link C++ function operator*(const FVector&,float);
#pragma link C++ function operator/(const FVector&,float);

#pragma link C++ function innerproduct(const FVector&,const FVector&);
#pragma link C++ function normalizedinnerproduct(const FVector&,const FVector&);
#pragma link C++ function pearsoncorrelation(const FVector&,const FVector&);
#pragma link C++ function cosinesimilarity(const FVector&,const FVector&);
#pragma link C++ function tanimotocoefficient(const FVector&,const FVector&);

////////////////////////////////////////////////////////
#pragma link C++ function exp(const FVector&);
#pragma link C++ function log(const FVector&);
#pragma link C++ function log10(const FVector&);
#pragma link C++ function sqrt(const FVector&);
#pragma link C++ function sinc(const FVector&);
#pragma link C++ function sin(const FVector&);
#pragma link C++ function cos(const FVector&);
#pragma link C++ function tan(const FVector&);
#pragma link C++ function asin(const FVector&);
#pragma link C++ function acos(const FVector&);
#pragma link C++ function atan(const FVector&);
#pragma link C++ function abs(const FVector&);
#pragma link C++ function fabs(const FVector&);
#pragma link C++ function sum(const FVector&);
#pragma link C++ function sum2(const FVector&);
#pragma link C++ function accum(const FVector&);
#pragma link C++ function rect(const FVector&);
#pragma link C++ function triangle(const FVector&);
#pragma link C++ function square(const FVector&);
#pragma link C++ function pulse(const FVector&);
#pragma link C++ function rand(const FVector&);
#pragma link C++ function pow(const FVector&,const FVector&);
#pragma link C++ function pow(float,const FVector&);
#pragma link C++ function pow(const FVector&,float);
#pragma link C++ function conv(const FVector&,const FVector&);
#pragma link C++ function integ(const FVector&,const FVector&);
#pragma link C++ function integ(const FVector&);
#pragma link C++ function diff(const FVector&,const FVector&);
#pragma link C++ function diff(const FVector&);
#pragma link C++ function gauss(const FVector&,const FVector&);
#pragma link C++ function movingave(const FVector&,int,int);
#pragma link C++ function resamplePowerOf2(const FVector&,const FVector&,FVector&,FVector&,float,float);

#pragma link C++ function fir(const FVector&,const FVector&);
#pragma link C++ function iir1(const FVector&,float,float);

#pragma link C++ function min(const FVector&,const FVector&);
#pragma link C++ function max(const FVector&,const FVector&);
#pragma link C++ function min(const FVector&,float);
#pragma link C++ function max(const FVector&,float);

#pragma link C++ function indexmin(const FVector&);
#pragma link C++ function indexmax(const FVector&);
#pragma link C++ function indexrange(const FVector&,float,float);
#pragma link C++ function peak(const FVector&,FVector&,IVector&,const float&,unsigned int);
#pragma link C++ function transition(const FVector&,IVector&,const float&,int); 

#pragma link C++ function mean(const FVector&);
#pragma link C++ function max(const FVector&);
#pragma link C++ function min(const FVector&);
#pragma link C++ function argmax(const FVector&);
#pragma link C++ function argmin(const FVector&);
#pragma link C++ function stddev(const FVector&);
#pragma link C++ function rms(const FVector&);
#pragma link C++ function pp(const FVector&);

#pragma link C++ function distance2(const FVector&,const FVector&);
#pragma link C++ function distance1(const FVector&,const FVector&);

#pragma link C++ function findpos(const FVector&,float);
#pragma link C++ function range(const FVector&,unsigned int,unsigned int);
#pragma link C++ function linear(float,float,unsigned int);

#pragma link C++ function gauss(float,float,float);
#pragma link C++ function gauss(const FVector&,float,float);
//#pragma link C++ function gauss(const FVector&,const FVector&,const FVector&);
////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
Vector<Vector<double> > similaritymatrix(const Vector<Vector<double> >& m,double (*simfunc)(const Vector<double>&,const Vector<double>&)) ;
Vector<Vector<float> > similaritymatrix(const Vector<Vector<float> >& m,float (*simfunc)(const Vector<float>&,const Vector<float>&)) ;
#pragma link C++ function similaritymatrix;


#pragma link C++ function Hash;

//#pragma link C++ function G__ateval(const DVector& a);
//#pragma link C++ function G__ateval(const DCVector& a);

///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////

#pragma link C++ class G__FFT;
#pragma link C++ function fft;
#pragma link C++ function ifft;
#pragma link C++ function spectrum;
#pragma link C++ function cepstrum;
#pragma link C++ global FFT;
#pragma link C++ global IFFT;
#pragma link C++ global SPECTRUM;
#pragma link C++ global CEPSTRUM;

///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////

#pragma link C++ function lstsq;
#pragma link C++ function eval_lstsq;

///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////

#pragma link C++ function randgauss(double,double);

///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////

#ifndef G__NOSPARSEVECTOR
#pragma link C++ class  DSparseVector;
#pragma link C++ class  FSparseVector;
#pragma link C++ class  ISparseVector;

#pragma link C++ function innerproduct(const DSparseVector&,const DSparseVector&);
#pragma link C++ function innerproduct(const FSparseVector&,const FSparseVector&);
#pragma link C++ function innerproduct(const ISparseVector&,const ISparseVector&);
#pragma link C++ function normalizedinnerproduct(const DSparseVector&,const DSparseVector&);
#pragma link C++ function normalizedinnerproduct(const FSparseVector&,const FSparseVector&);
#pragma link C++ function normalizedinnerproduct(const ISparseVector&,const ISparseVector&);

#pragma link C++ class  DSparseMatrix;
#pragma link C++ class  FSparseMatrix;
#pragma link C++ class  ISparseMatrix;
#endif

///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////

#pragma link C++ class Histogram<double>;
//#pragma link C++ function hist(const DVector&,int=100,double=0.0,double=0.0);
#pragma link C++ function hist;

///////////////////////////////////////////////////////////////////
#pragma link C++ class statistics;
#pragma link C++ function stat(const Vector<double>&);
#pragma link C++ function stat(const Vector<double>&,const Vector<double>&,double,double) ;

#pragma link C++ global VIRTUALGRAPH_H;
#pragma link C++ global VG_MASK_PLOTMODE;
#pragma link C++ global VG_PLOTMODE;
#pragma link C++ global VG_SAMEMODE;
#pragma link C++ global VG_N;
#pragma link C++ global VG_N_MAX;
#pragma link C++ global VG_OBJ;
#pragma link C++ global VG_XSIZE;
#pragma link C++ global VG_YSIZE;
#pragma link C++ global vg_width;
#pragma link C++ global vg_opt;
#pragma link C++ class VirtualGraph;
#pragma link C++ function virtualgraph_front;
#pragma link C++ function virtualgraph_back;
#pragma link C++ function virtualgraph_get(int);
#pragma link C++ function virtualgraph_size;
#pragma link C++ function virtualgraph_NNInput;

///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////

#pragma link C++ class COLOR;
#pragma link C++ class Vectorostream ;
#pragma link C++ global plot;
#pragma link C++ global plotNN;
#pragma link C++ global sameNN;

#pragma link C++ class Matrixostream ;
#pragma link C++ global splot;

#ifndef G__ROOT
//#pragma link C++ function cintgraph_open;
//#pragma link C++ function cintgraph_data;
//#pragma link C++ function cintgraph_invoke;
#pragma link C++ function cintgraph_command;
#pragma link C++ function cintgraph_clear;
#pragma link C++ function cintgraph_exit;
#pragma link C++ function cintgraph_start;
#pragma link C++ function cintgraph_stop;
#pragma link C++ function plot_cintgraph;

#pragma link C++ function rootgraph_option;
#pragma link C++ function rootgraph_lineatt;

#pragma link C++ function cintgraph_mode;
#pragma link C++ function gnuplot_mode;
#pragma link C++ function graph_mode;

#pragma link C++ global LOG;
#pragma link C++ global LIN;


#endif


///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
#endif
