// MacOS X compiled object member offset test

class A {
 public:
  int a;
  double b;
 A() : a(1), b(1.2) { }
 A(int ain,double bin) : a(ain), b(bin) {}
  void disp() {
    printf("this=%p  &a=%p %d  &b=%p %g\n",this,&a,a,&b,b);
  }
};
