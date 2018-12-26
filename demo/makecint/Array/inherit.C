//
// demo/makecint/Array/inherit.C
//
//
//

class Position : public Complex {
 public:
  enum MODE { POLAR , XY };
  MODE mode;
  Position() : Complex(1,2) {
    mode = POLAR;
  }
  Position(double r,double i) : Complex(r,i) {
    mode = POLAR;
  }
};

Position a;

main()
{
  Position b;
  Position c(3,4);
  a.disp();
  b.disp();
  c.disp();
  printf("\n");
}
