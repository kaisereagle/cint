#include <stdio.h>
class B {
      friend A;
	int b;
	void dispprivate() { printf("b=%d\n",b); }
      public:
	B() { b =3229; }
};

class A {
	int a;
	void dispprivate() { printf("a=%d\n",a); }
      public:
	A() { a=1234; }
	void disppublic() {
		B b;
		A *pa;
		pa = this;
		pa->a++;
		pa->dispprivate();
		// fprintf(stderr,"Intentinnal error below b.dispprivate()\n");
		b.dispprivate();
	}
};


main()
{
	A obja;
	obja.disppublic();
}
