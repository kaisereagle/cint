#include <stdio.h>
class complex {
      public:
	double re,im;
	complex(double r=0,double i=0) { re=r; im=i; }
};


double operator+(complex& a,complex& b) {return(a.re+b.re);}
int operator <(complex& a,complex& b) {return(a.re<b.re);}
int operator >(complex& a,complex& b) {return(a.re>b.re);}
int operator <=(complex& a,complex& b) {return(a.re<=b.re);}
int operator >=(complex& a,complex& b) {return(a.re>=b.re);}
int operator ==(complex& a,complex& b) {return(a.re==b.re);}
int operator!(complex& a) {return(!a.re);}
int operator!=(complex& a,complex& b) {return(a.re!=b.re);}

int main()
{
  complex a(1),b(2) /*,c(3) */;

  if(a<b) printf("a<b o\n");
  else    printf("a<b x\n");
  
  if(a>b) printf("a>b o\n");
  else    printf("a>b x\n");
  
  if(a<=b) printf("a<=b o\n");
  else    printf("a<=b x\n");
  
  if(a>=b) printf("a>=b o\n");
  else    printf("a>=b x\n");
  
  return 0;
}
