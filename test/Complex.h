// Complex.h

#ifndef COMPLEX_H
#define COMPLEX_H

#ifdef __hpux
#include <iostream.h>
#else
#include <iostream>
using namespace std;
#endif

// ���f���N���X //////////////////////////////////////////////////
class Complex {
 public:
  // ������
  Complex(double a=0.0,double b=0.0) { re=a; im=b; }

  // ���擾
  double real() const { return(re); }
  double imag() const { return(im); }

  // �����o�֐��ɂ�鉉�Z�q���d��`
  Complex& operator+=(Complex& a);
  Complex& operator-=(Complex& a);
  Complex& operator*=(Complex& a);
  Complex& operator/=(Complex& a);

  // �t�����h�֐��ɂ�鉉�Z�q���d��`
  friend bool operator==(Complex& a,Complex& b);
  friend bool operator!=(Complex& a,Complex& b){return(!(a==b));}
  friend Complex operator +(Complex& a,Complex& b);
  friend Complex operator -(Complex& a,Complex& b);
  friend Complex operator *(Complex& a,Complex& b);
  friend Complex operator /(Complex& a,Complex& b);
  friend ostream& operator <<(ostream& ios,Complex& a);

  // �Z�p�֐��̑��d��`
  friend double abs(Complex& a);
  friend Complex exp(Complex& a);

 private:
  double re,im;
};

#endif
