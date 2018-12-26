/**************************************************************************
* complex.h
*
*
**************************************************************************/

#ifndef G__COMPLEX_H
#define G__COMPLEX_H

#include <iostream.h>
#include <math.h>

class complex {
      public:
	double re,im;
	complex(double a=0,double b=0) { re=a; im=b; }
	complex(const complex& a) {re=a.re; im=a.im;}
	complex& operator =(const complex& a) 
	  {re=a.re; im=a.im; return(*this);}
        void disp(FILE* fp=stdout) {fprintf(fp,"(%g,%g)",re,im);}
};

complex operator +(const complex& a,const complex& b)
{
	complex c;
	c.re = a.re+b.re;
	c.im = a.im+b.im;
	return(c);
}

complex operator -(const complex& a,const complex& b)
{
	complex c;
	c.re = a.re-b.re;
	c.im = a.im-b.im;
	return(c);
}

complex operator *(const complex& a,const complex& b)
{
	complex c;
	c.re = a.re*b.re-a.im*b.im;
	c.im = a.re*b.im+a.im*b.re;
	return(c);
}

complex operator /(const complex& a,const complex& b)
{
	complex c;
	double x;
	x = b.re*b.re+b.im*b.im;
	c.re = (a.re*b.re+a.im*b.im)/x;
	c.im = (a.im*b.re-a.re*b.im)/x;
	return(c);
}


//**********************************************************************

complex exp(complex& a)
{
	complex c;
	double mag;
	mag = exp(a.re);
	c.re=mag*cos(a.im);
	c.im=mag*sin(a.im);
	return(c);
}

double abs(complex& a)
{
	double result;
	result = sqrt(a.re*a.re+a.im*a.im);
	return(result);
}

double re(complex& a)
{
	return(a.re);
}

double im(complex& a)
{
	return(a.im);
}

complex rotate(const complex& a,double rad)
{
  complex c;
  c.re= a.re*cos(rad) - a.im*sin(rad);
  c.im= a.re*sin(rad) + a.im*cos(rad);
  return(c);
}

double atan(const complex& a) {
  return(atan(a.re/a.im));
}


/**************************************************************************
* iostream
**************************************************************************/
ostream& operator <<(ostream& ios,complex& a)
{
	ios << "(" << a.re << "," << a.im << ")" ;
	return(ios);
}

#if 0
ostrstream& operator <<(ostrstream& ios,complex& a)
{
	ios << "(" << a.re << "," << a.im << ")" ;
	return(ios);
}
#endif

#if 0
istream& operator >>(istream& ios,complex& a)
{
	ios >> a.re >> a.re ;
	return(ios);
}

istrstream& operator >>(istrstream& ios,complex& a)
{
	ios >> a.re >> b.reî;
	return(ios);
}
#endif

#ifdef __CINT__
int G__ateval(const complex& x) {
  cout << "(complex)" << x << endl;
  return(1);
}
#endif

#endif
