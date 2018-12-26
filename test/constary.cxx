#include <stdio.h>
class complex { 
	double re,im;
      public:
	complex(double val=5) {re=val;im=0;}
	complex(complex& a) {*this = a;}
	void print(void);
};

void complex::print(void)
{
	printf("%g %g\n",re,im);
}
int main()
{
	complex a;
	a.print();
	complex b(3);
	b.print();
	complex c[2];
	c[0].print();
	c[1].print();

	return 0;
}
