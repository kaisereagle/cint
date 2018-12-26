/**************************************************************************
* Complex.h
*
*  Complex number class template
*
* 2013/Sep 
* Masaharu Goto
*
**************************************************************************/
/************************************************************************
 * Copyright(c) 1995~2015  Masaharu Goto (cint@pcroot.cern.ch) (gotom@hanno.jp)
 *
 * Permission to use, copy, modify and distribute this software and its 
 * documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation. The author makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 ************************************************************************/

#ifndef G__COMPLEX
#define G__COMPLEX

#define __EXP__

#include <iostream>
#include <cmath>

template<class T>
class Complex {
 public:
   Complex(T a=0,T b=0) { re=a; im=b; }
   Complex(const Complex<T>& a) {re=a.re; im=a.im;}
   Complex& operator=(const Complex<T>& a) { 
    this->re=a.re;
    this->im=a.im; 
    return(*this);
   }
   Complex& operator=(const T& a) { 
    this->re=a;
    this->im=0;
    return(*this);
   }
   T real() const { return re; }
   T imag() const { return im; }
   T& real() { return re; }
   T& imag() { return im; }
 //protected:
  T re,im;
 public:
#if 0
  friend Complex operator +(const Complex<T>& a,const Complex<T>& b);
  friend Complex operator -(const Complex<T>& a,const Complex<T>& b);
  friend Complex operator *(const Complex<T>& a,const Complex<T>& b);
  friend Complex operator /(const Complex<T>& a,const Complex<T>& b);
  friend Complex operator +(const Complex<T>& a,const T& b);
  friend Complex operator -(const Complex<T>& a,const T& b);
  friend Complex operator *(const Complex<T>& a,const T& b);
  friend Complex operator /(const Complex<T>& a,const T& b);
#ifdef __EXP__
  friend Complex exp(const Complex<T>& a);
#endif
  friend T abs(const Complex& a);
  friend Complex rotate(const Complex<T>& a,double rad);
#endif
};

////////////////////////////////////////////////////////////////////
template<class T>
Complex<T> operator +(const Complex<T>& a,const Complex<T>& b) {
  Complex<T> c;
  c.re = a.re+b.re;
  c.im = a.im+b.im;
  return(c);
}
 
template<class T>
Complex<T> operator -(const Complex<T>& a,const Complex<T>& b) {
  Complex<T> c;
  c.re = a.re-b.re;
  c.im = a.im-b.im;
  return(c);
}

template<class T>
Complex<T> operator *(const Complex<T>& a,const Complex<T>& b) {
  Complex<T> c;
  c.re = a.re*b.re-a.im*b.im;
  c.im = a.re*b.im+a.im*b.re;
  return(c);
}

template<class T>
Complex<T> operator /(const Complex<T>& a,const Complex<T>& b) {
  Complex<T> c;
  T x;
  x = b.re*b.re+b.im*b.im;
  c.re = (a.re*b.re+a.im*b.im)/x;
  c.im = (a.im*b.re-a.re*b.im)/x;
  return(c);
}

////////////////////////////////////////////////////////////////////
template<class T>
Complex<T> operator +(const Complex<T>& a,const T& b) {
  Complex<T> c;
  c.re = a.re+b;
  c.im = a.im;
  return(c);
}
 
template<class T>
Complex<T> operator -(const Complex<T>& a,const T& b) {
  Complex<T> c;
  c.re = a.re-b;
  c.im = a.im;
  return(c);
}

template<class T>
Complex<T> operator *(const Complex<T>& a,const T& b) {
  Complex<T> c;
  c.re = a.re*b;
  c.im = a.im*b;
  return(c);
}

template<class T>
Complex<T> operator /(const Complex<T>& a,const T& b) {
  Complex<T> c;
  c.re = a.re/b;
  c.im = a.im/b;
  return(c);
}


//**********************************************************************

#ifdef __EXP__
template<class T>
Complex<T> exp(const Complex<T>& a)
{
  Complex<T> c;
  const T mag = exp(a.real());
  c.re=mag*cos(a.im);
  c.im=mag*sin(a.im);
  return(c);
}
#endif

template<class T>
T abs(const Complex<T>& a)
{
  T result;
  result = sqrt(a.re*a.re+a.im*a.im);
  return(result);
}

template<class T>
Complex<T> rotate(const Complex<T>& a,double rad)
{
  Complex<T> c;
  c.re= a.re*cos(rad) - a.im*sin(rad);
  c.im= a.re*sin(rad) + a.im*cos(rad);
  return(c);
}


/**************************************************************************
* iostream
**************************************************************************/

template<class T>
ostream& operator <<(ostream& ios,const Complex<T>& a)
{
  ios << "(" << a.real() << "," << a.imag() << ")" ;
  return(ios);
}

#if 0
template<class T>
ostrstream& operator <<(ostrstream& ios,const Complex<T>& a)
{
  ios << "(" << a.real() << "," << a.imag() << ")" ;
  return(ios);
}
#endif

template<class T>
istream& operator >>(istream& ios,const Complex<T>& a)
{
  ios >> a.real() >> a.imag() ;
  return(ios);
}

#if 0
template<class T>
istrstream& operator >>(istrstream& ios,const Complex<T>& a)
{
  ios >> a.real() >> b.imag() ;
  return(ios);
}
#endif


#endif
