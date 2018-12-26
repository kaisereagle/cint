

//#include "Eigen/Core"
#include <Eigen/Dense>

using namespace std;
using namespace Eigen;

//////////////////////////////////////////////////////////////////////
template<class MatrixType> 
void disp(const MatrixType& a) {
  int i,j;
  printf("# %d x %d \n",a.rows(),a.cols());
  for(i=0;i<a.rows();i++) {
    for(j=0;j<a.cols();j++) {
      printf("%8.2g,",a(i,j));
    }
    printf("\n");
  }
}

//////////////////////////////////////////////////////////////////////
int test(/* const int rowsizex, const int colsizex */) {
  const int rowsize=5;
  const int colsize=3;
  double d;
  Matrix<double, rowsize, colsize> a;
  a <<  1,  2,  3,  
       11, 12, 13,  
       21, 22, 23,  
       31, 32, 33,  
       41, 42, 43 ;  
  disp(a);

  Matrix<double, colsize, colsize> b,c;
  b <<  1,  0,  0,  
        0,  2,  0,  
        0,  0,  3;  
  c = b.inverse(); disp(b); disp(c);

  b <<  1,  4, -1,  
        4,  2,  1,  
       -1,  1,  3;  
  c = b.inverse(); disp(b); disp(c);

  d = b.determinant(); printf("%g\n",d);
  d = c.determinant(); printf("%g\n",d);

  Matrix<double, 2,2> A,B,C;
  A << 1, 0 ,
       0, 2 ;
  B = A.inverse(); disp(A); disp(B);
  A << 1.57652, 0 ,
       0, 0.292146 ;
  B = A.inverse(); disp(A); disp(B);
  A << 0,  1.57652, 
       0.292146, 0 ;
  B = A.inverse(); disp(A); disp(B);

  return(0);
}


//////////////////////////////////////////////////////////////////////
int main() {
  test(/* 5,3 */);
  return(0);
}

