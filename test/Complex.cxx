// Complex.cxx

#include <math.h>
#include "Complex.h"

// �����o�֐��ɂ�鉉�Z�q���d��` //////////////////////////////////
Complex& Complex::operator+=(Complex& a) {
  re += a.re;
  im += a.im;
  return(*this);
}

Complex& Complex::operator-=(Complex& a) {
  re -= a.re;
  im -= a.im;
  return(*this);
}

Complex& Complex::operator*=(Complex& a) {
  re = re*a.re-im*a.im;
  im = re*a.im+im*a.re;
  return(*this);
}

Complex& Complex::operator/=(Complex& a) {
  double x;
  x = a.re*a.re+a.im*a.im;
  re = (re*a.re+im*a.im)/x;
  im = (im*a.re-re*a.im)/x;
 return(*this);
}

// �t�����h�֐��ɂ�鉉�Z�q���d��` ////////////////////////////////
bool operator ==(Complex& a,Complex& b)
{
  return( a.re==b.re && a.im==b.im );
}

Complex operator +(Complex& a,Complex& b)
{
  Complex c;
  c.re = a.re+b.re;
  c.im = a.im+b.im;
  return(c);
}

Complex operator -(Complex& a,Complex& b)
{
  Complex c;
  c.re = a.re-b.re;
  c.im = a.im-b.im;
  return(c);
}

Complex operator *(Complex& a,Complex& b)
{
  Complex c;
  c.re = a.re*b.re-a.im*b.im;
  c.im = a.re*b.im+a.im*b.re;
  return(c);
}

Complex operator /(Complex& a,Complex& b)
{
  Complex c;
  double x;
  x = b.re*b.re+b.im*b.im;
  c.re = (a.re*b.re+a.im*b.im)/x;
  c.im = (a.im*b.re-a.re*b.im)/x;
  return(c);
}

// ���o�̓X�g���[�����Z�q�̑��d��` //////////////////////////////
ostream& operator <<(ostream& ios,Complex& a)
{
  ios << "(" << a.re << "," << a.im << ")" ;
  return(ios);
}

// �Z�p�֐��̑��d��` ////////////////////////////////////////////
double abs(Complex &a)
{
  double result = sqrt(a.re*a.re+a.im*a.im);
  return(result);
}

Complex exp(Complex& a)
{
  Complex c;
  double mag = exp(a.re);
  c.re=mag*cos(a.im);
  c.im=mag*sin(a.im);
  return(c);
}
