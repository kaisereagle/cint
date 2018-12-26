/**************************************************************************
* Complex.h
*
*
**************************************************************************/

#ifndef COMPLEX_H
#define COMPLEX_H

#include <stdio.h>
#include <math.h>
#ifdef __VC13__
#include <iostream>
#else
#include <iostream.h>
#endif

class Complex {
 public:
  Complex(double a=0.0,double b=0.0) { re=a; im=b; }
  Complex(const Complex& x) { re=x.re; im=x.im; }
  //Complex& operator=(const Complex& x) { re=x.re; im=x.im; return *this;}
  double real(void) { return(re); }
  double imag(void) { return(im); }
  double abs(void) { return(sqrt(re*re+im*im)); }
  void disp(void) ;

  friend Complex operator +(Complex const & a,Complex const & b);
  friend Complex operator -(Complex const & a,Complex const & b);
  friend Complex operator *(Complex const & a,Complex const & b);
  friend Complex operator /(Complex const & a,Complex const & b);
  friend Complex exp(const Complex& a);
  friend double fabs(const Complex& a);
  friend double real(const Complex& a);
  friend double imag(const Complex& a);

 private:
  double re,im;
};

// #ifndef __MAKECINT__
ostream& operator<<(ostream& ios,Complex& a);
// #endif

// Added for test
const int cf1(const int a);
const int& cf2(const int& a);
int const& cf3(int const& a);
int const & cf4(int const & a);
int const &cf5(int const &a);
const int & cf6(const int & a);
const int* cf7(const int* a);
const int *const cf8(const int *const a);
const int * const cf9(const int * const a);

#endif
