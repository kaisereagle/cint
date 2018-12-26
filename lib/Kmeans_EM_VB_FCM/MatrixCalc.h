/*************************************************************************
* Eigen value, absolute value and Inverse for N * N matrix 
*
*************************************************************************/

#ifndef MATRIXCALC_H
#define MATRIXCALC_H

#include "Vector.h"
#if  !defined(__CINT__) && (!defined(_MSC_VER) || _MSC_VER>1300)
#include <Eigen/Dense>
using namespace Eigen;
#endif
using namespace std;

#if  !defined(__CINT__) && (!defined(_MSC_VER) || _MSC_VER>1300)
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
// Utility using Eigen library
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
template<class T,int N,int M>
void Matrix2EigenMatrix(Eigen::Matrix<T,N,M>& aa 
			,const Vector<Vector<T> >& a) {
  for(unsigned int i=0;i<N;i++) {
    for(unsigned int j=0;j<M;j++) {
      aa(i,j) = a[i][j];
    }
  }
}

///////////////////////////////////////////////////////////////////////
template<class T,int N,int M>
void EigenMatrix2Matrix(Vector<Vector<T> >& a
			//,const Eigen::Matrix<T,N,M,0,N,M>& aa) {
			,const Eigen::Matrix<T,N,M>& aa) {
  a.resize(M); Vector<T> v(0,0,M); a = v;
  for(unsigned int i=0;i<N;i++) {
    for(unsigned int j=0;j<M;j++) {
      a[i][j] = aa(i,j);
    }
  }
}

///////////////////////////////////////////////////////////////////////
#if 0
template<class T,int N>
void Vector2EigenArray(Eigen::Array<T,N>& aa ,const Vector<T>& a) {
  for(unsigned int i=0;i<N;i++) {
    aa(i) << a[i];
  }
}
#endif

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
template<class T,int N,int M>
inline void Inverse(Eigen::Matrix<T,N,M>& aa
		    ,const Vector<Vector<T> >& a
		    ,Vector<Vector<T> >& c) {
  Matrix2EigenMatrix(aa,a);
  Eigen::Matrix<T,N,M> bb;
  bb = aa.inverse();
  EigenMatrix2Matrix(c,bb);
}

///////////////////////////////////////////////////////////////////////
template<class T,int N,int M>
inline void Determinant(Eigen::Matrix<T,N,M>& aa
			,const Vector<Vector<T> >& a
			,T& c) {
  Matrix2EigenMatrix(aa,a);
  c = aa.determinant();
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
#define MACRO_OPERATION(M,OPERATION)                                 \
  if(M==a.size() && M==a[0].size()) {                                \
    Eigen::Matrix<T,M,M> aa;                                         \
    OPERATION(aa,a,c);                                               \
    return(c);                                                       \
  }

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

#else //  !defined(__CINT__) && (!defined(_MSC_VER) || _MSC_VER>1300)
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
// Utility without Eigen library
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
template<class T>
inline void Inverse2(const Vector<Vector<T> >& a,Vector<Vector<T> >& c) {
  T det;
  Determinant2(a,det);
  c.resize(2);
  c[0].resize(2);
  c[1].resize(2);
  if(det!=0) {
    c[0][0] =  a[1][1]/det;
    c[0][1] = -a[0][1]/det;
    c[1][0] = -a[1][0]/det;
    c[1][1] =  a[0][0]/det;
  }
  else {
    for(int i=0;i<2;i++) {
      for(int j=0;j<2;j++) c[i][j] =  0;
    }
  }
}

///////////////////////////////////////////////////////////////////////
template<class T>
inline void Inverse3(const Vector<Vector<T> >& a,Vector<Vector<T> >& c) {
  T det;
  Determinant3(a,det);
  c.resize(3);
  c[0].resize(3);
  c[1].resize(3);
  c[2].resize(3);
  if(det!=0) {
    c[0][0] =  (a[1][1]*a[2][2]-a[1][2]*a[2][1])/det;
    c[0][1] =  (a[0][2]*a[2][1]-a[0][1]*a[2][2])/det;
    c[0][2] =  (a[0][1]*a[1][2]-a[0][2]*a[1][1])/det;

    c[1][0] =  (a[1][2]*a[2][0]-a[1][0]*a[2][2])/det;
    c[1][1] =  (a[0][0]*a[2][2]-a[0][2]*a[2][0])/det;
    c[1][2] =  (a[0][2]*a[1][0]-a[0][0]*a[1][2])/det;

    c[2][0] =  (a[1][0]*a[2][1]-a[1][1]*a[2][0])/det;
    c[2][1] =  (a[0][1]*a[2][0]-a[0][0]*a[2][1])/det;
    c[2][2] =  (a[0][0]*a[1][1]-a[0][1]*a[1][0])/det;
  }
  else {
    for(int i=0;i<3;i++) {
      for(int j=0;j<3;j++) c[i][j] =  0;
    }
  }
}

///////////////////////////////////////////////////////////////////////
template<class T>
inline void Determinant2(const Vector<Vector<T> >& a ,T& c) {
  c = a[0][0]*a[1][1] - a[0][1]*a[1][0];
}

///////////////////////////////////////////////////////////////////////
template<class T>
inline void Determinant3(const Vector<Vector<T> >& a ,T& c) {
  c = a[0][0]*a[1][1]*a[2][2] 
    + a[1][0]*a[2][1]*a[0][2] 
    + a[2][0]*a[0][1]*a[1][2] 
    - a[0][0]*a[2][1]*a[1][2] 
    - a[2][0]*a[1][1]*a[0][2] 
    - a[1][0]*a[0][1]*a[2][2] ;
}

///////////////////////////////////////////////////////////////////////
#define MACRO_OPERATION(M,OPERATION)                                 \
  if(M==a.size() && M==a[0].size()) {                                \
    OPERATION##M(a,c);                                               \
    return(c);                                                       \
  }

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
#endif //  !defined(__CINT__) && (!defined(_MSC_VER) || _MSC_VER>1300)

///////////////////////////////////////////////////////////////////////
template<class T>
Vector<Vector<T> > Inverse(const Vector<Vector<T> >& a) {
  Vector<Vector<T> > c;
  MACRO_OPERATION(2,Inverse);
  MACRO_OPERATION(3,Inverse);
  //#if !defined(__CINT__) && (!defined(_MSC_VER) || _MSC_VER>1300)
  // Need Eigen Matrix library
#ifndef NOEIGEN
  MACRO_OPERATION(4,Inverse);
  MACRO_OPERATION(5,Inverse);
  MACRO_OPERATION(6,Inverse);
  MACRO_OPERATION(7,Inverse);
  MACRO_OPERATION(8,Inverse);
  MACRO_OPERATION(9,Inverse);
  MACRO_OPERATION(10,Inverse);
  MACRO_OPERATION(11,Inverse);
  MACRO_OPERATION(12,Inverse);
  MACRO_OPERATION(13,Inverse);
  MACRO_OPERATION(14,Inverse);
  MACRO_OPERATION(15,Inverse);
  MACRO_OPERATION(16,Inverse);
  MACRO_OPERATION(17,Inverse);
  MACRO_OPERATION(18,Inverse);
  MACRO_OPERATION(19,Inverse);
  MACRO_OPERATION(20,Inverse);
  MACRO_OPERATION(21,Inverse);
  MACRO_OPERATION(22,Inverse);
  MACRO_OPERATION(23,Inverse);
  MACRO_OPERATION(24,Inverse);
  MACRO_OPERATION(25,Inverse);
  MACRO_OPERATION(26,Inverse);
  MACRO_OPERATION(27,Inverse);
  MACRO_OPERATION(28,Inverse);
  MACRO_OPERATION(29,Inverse);
  MACRO_OPERATION(30,Inverse);
  MACRO_OPERATION(31,Inverse);
  MACRO_OPERATION(32,Inverse);
#endif
  if(!a.size() || !a[0].size() || a.size()!=a[0].size()) {
    fprintf(stderr,"Matrix::Inverse() Error: Inverse() illegal matrix size %d %d\n",a.size(),a[0].size());
    exit(255);
  }
  fprintf(stderr,"Matrix::Inverse() Error: Add 'MACRO_OPERATION(%d,Inverse)' in %s %d and recompile\n",a.size(),__FILE__,__LINE__);
  exit(255);
  return(c);
}

///////////////////////////////////////////////////////////////////////
template<class T>
T Determinant(const Vector<Vector<T> >& a) {
  T c=0;
  MACRO_OPERATION(2,Determinant);
  MACRO_OPERATION(3,Determinant);
  //#if !defined(__CINT__) && (!defined(_MSC_VER) || _MSC_VER>1300)
  // Need Eigen Matrix library
#ifndef NOEIGEN
  MACRO_OPERATION(4,Determinant);
  MACRO_OPERATION(5,Determinant);
  MACRO_OPERATION(6,Determinant);
  MACRO_OPERATION(7,Determinant);
  MACRO_OPERATION(8,Determinant);
  MACRO_OPERATION(9,Determinant);
  MACRO_OPERATION(10,Determinant);
  MACRO_OPERATION(11,Determinant);
  MACRO_OPERATION(12,Determinant);
  MACRO_OPERATION(13,Determinant);
  MACRO_OPERATION(14,Determinant);
  MACRO_OPERATION(15,Determinant);
  MACRO_OPERATION(16,Determinant);
  MACRO_OPERATION(17,Determinant);
  MACRO_OPERATION(18,Determinant);
  MACRO_OPERATION(19,Determinant);
  MACRO_OPERATION(20,Determinant);
  MACRO_OPERATION(21,Determinant);
  MACRO_OPERATION(22,Determinant);
  MACRO_OPERATION(23,Determinant);
  MACRO_OPERATION(24,Determinant);
  MACRO_OPERATION(25,Determinant);
  MACRO_OPERATION(26,Determinant);
  MACRO_OPERATION(27,Determinant);
  MACRO_OPERATION(28,Determinant);
  MACRO_OPERATION(29,Determinant);
  MACRO_OPERATION(30,Determinant);
  MACRO_OPERATION(31,Determinant);
  MACRO_OPERATION(32,Determinant);
#endif
  fprintf(stderr,"Matrix::Determinant() Error: Add 'MACRO_OPERATION(%d,Determinant)' in %s %d and recompile\n",a.size(),__FILE__,__LINE__);
  exit(255);
  return(c);
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
template<class T>
T GaussExp(const Vector<T>& xu,const Vector<Vector<T> >& Icovar) {
  const unsigned int M = xu.size();
  Vector<T> t(0.0,0.0,M);
  unsigned int m1,m2;
  for(m1=0;m1<M;m1++) {
    for(m2=0;m2<M;m2++) {
      t[m1] += xu[m1] * Icovar[m1][m2];
    }
  }
  T e = 0;
  for(m1=0;m1<M;m1++) {
    e += t[m1]*xu[m1];
  }
  return(-e/2);
}


///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
// CAUTION :  NEED TO REVIEW AND DEBUG FOLLOWING FUNCTIONS
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
// vector opr vector
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////
// scalar  = vectorT * vector
///////////////////////////////////////////////////////////////////////
template<class T>
T InnerProduct(const Vector<T>& xT,const Vector<T>& y) {
  return(ProductT_(xT,y));
}

///////////////////////////////////////////////////////////////////////
// scalar  = vectorT * vector
///////////////////////////////////////////////////////////////////////
template<class T>
T ProductT_(const Vector<T>& xT,const Vector<T>& y) {
  T r=0.0;
  unsigned int i,sx=xT.size(),sy=y.size(),s=min(sy,sx);
  for(i=0;i<s;i++) {
    r += xT[i] * y[i];
  }
  return(r);
}

///////////////////////////////////////////////////////////////////////
// vector = vector * vectorT
///////////////////////////////////////////////////////////////////////
template<class T>
vector<T> Product_T(const Vector<T>& x,const Vector<T>& yT) {
  unsigned int i,sx=x.size(),sy=yT.size(),s=min(sy,sx);
  vector<T> r; r.resize(s);
  for(i=0;i<s;i++) {
    r[i] = x[i] * yT[i];
  }
  return(r);
}

///////////////////////////////////////////////////////////////////////
// vector = vectorT * vectorT
///////////////////////////////////////////////////////////////////////
template<class T>
vector<T> ProductTT(const Vector<T>& xT,const Vector<T>& yT) {
  unsigned int i,j,sx=xT.size(),sy=yT.size();
  vector<T> r; r.resize(sx);
  for(i=0;i<sx;i++) {
    for(j=0;j<sy;j++) r[i] += xT[i] * yT[j];
  }
  return(r);
}

///////////////////////////////////////////////////////////////////////
// vector = vector * vector
///////////////////////////////////////////////////////////////////////
template<class T>
vector<T> Product__(const Vector<T>& x,const Vector<T>& y) {
  unsigned int i,j,sx=x.size(),sy=y.size();
  vector<T> r; r.resize(sy);
  for(i=0;i<sy;i++) {
    for(j=0;j<sx;j++) r[i] += x[j] * y[i];
  }
  return(r);
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
// vector opr Matrix
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////
// vector  = vectorT * Matrix
///////////////////////////////////////////////////////////////////////
template<class T>
Vector<T> ProductT(const Vector<T>& xT,const Vector<Vector<T> >& A) {
  Vector<T> r; 
  unsigned int i,j,sxT=xT.size(),sax=A.size();
  if(!sax) return(r);
  unsigned int say=A[0].size(),s=min(say,sxT);
  r.resize(say);
  for(i=0;i<say;i++) {
    for(j=0;j<sax;j++) {
      r[i] += xT[j] * A[i][j];
      //r[i] += xT[j] * A[j][i];
    }
  }
  return(r);
}

///////////////////////////////////////////////////////////////////////
// vector  = vector * Matrix  (not popular)
///////////////////////////////////////////////////////////////////////
template<class T>
Vector<T> Product(const Vector<T>& x,const Vector<Vector<T> >& A) {
  Vector<T> r; 
  unsigned int i,j,sx=xT.size(),sax=A.size();
  if(!sax) return(r);
  unsigned int say=A[0].size(),s=min(sax,sx);
  r.resize(say);
  for(i=0;i<s;i++) {
    for(j=0;j<s;j++) {
      r[i] += x[i] * A[i][j];
      //r[i] += x[i] * A[j][i];
    }
  }
  return(r);
}

///////////////////////////////////////////////////////////////////////
// vector  = Matrix * vector
///////////////////////////////////////////////////////////////////////
template<class T>
Vector<T> Product(const Vector<Vector<T> >& A,const Vector<T>& x) {
  Vector<T> r; 
  unsigned int i,j,sx=xT.size(),sax=A.size();
  if(!sax) return(r);
  unsigned int say=A[0].size(),s=min(say,sx);
  r.resize(say);
  for(i=0;i<s;i++) {
    for(j=0;j<s;j++) {
      r[i] += A[j][i] * x[j];
      //r[i] += A[i][j] * x[j];
    }
  }
  return(r);
}

///////////////////////////////////////////////////////////////////////
// vector  = Matrix * vectorT  (not popular)
///////////////////////////////////////////////////////////////////////
template<class T>
Vector<T> ProductT(const Vector<Vector<T> >& A,const Vector<T>& xT) {
  Vector<T> r; 
  unsigned int i,j,sxT=xT.size(),sax=A.size();
  if(!sax) return(r);
  unsigned int say=A[0].size(),s=min(say,sxT);
  r.resize(say);
  for(i=0;i<s;i++) {
    for(j=0;j<s;j++) {
      r[i] += A[j][i] * xT[i];
      //r[i] += A[i][j] * xT[i];
    }
  }
  return(r);
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
// Matrix opr Matrix
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////
// Matrix = Matrix * Matrix
///////////////////////////////////////////////////////////////////////
template<class T>
Vector<Vector<T> > InnerProductM(const Vector<Vector<T> >& A
				,const Vector<Vector<T> >& B) {
  Vector<Vector<T> > r; 
  unsigned int i,j,sax=A.size(),sbx=B.size();
  if(!sax || !say) return(r);
  unsigned int say=A[0].size(),sby=B[0].size();
  unsigned int sx=min(say,sbx), sy=min(sax,sby);
  r.resize(sx);
  for(i=0;i<sx;i++) {
    r[i].resize(sy);
    for(j=0;j<sy;j++) {
      r[i][j] += A[j][i] * B[i][j];
      //r[j][i] += A[i][j] * B[j][i];
    }
  }
  return(r);
}


///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////
// Matrix = Matrix * Matrix
///////////////////////////////////////////////////////////////////////
template<class T>
Vector<Vector<T> > SimpleProduct(const Vector<Vector<T> >& A
				 ,const Vector<Vector<T> >& B) {
  Vector<Vector<T> > r; 
  unsigned int i,j,sax=A.size(),sbx=B.size();
  if(!sax || !say) return(r);
  unsigned int say=A[0].size(),sby=B[0].size();
  unsigned int sx=min(sax,sbx), sy=min(say,sby);
  r.resize(sx);
  for(i=0;i<sx;i++) {
    r[i].resize(sy);
    for(j=0;j<sy;j++) {
      r[i][j] = A[i][j] * B[i][j];
    }
  }
  return(r);
}

///////////////////////////////////////////////////////////////////////
// Matrix = Matrix / Matrix
///////////////////////////////////////////////////////////////////////
template<class T>
Vector<Vector<T> > SimpleDivide(const Vector<Vector<T> >& A
				     ,const Vector<Vector<T> >& B) {
  Vector<Vector<T> > r; 
  unsigned int i,j,sax=A.size(),sbx=B.size();
  if(!sax || !say) return(r);
  unsigned int say=A[0].size(),sby=B[0].size();
  unsigned int sx=min(sax,sbx), sy=min(say,sby);
  r.resize(sx);
  for(i=0;i<sx;i++) {
    r[i].resize(sy);
    for(j=0;j<sy;j++) {
      r[i][j] = A[i][j] / B[i][j];
    }
  }
  return(r);
}

///////////////////////////////////////////////////////////////////////
// Matrix = Matrix + Matrix
///////////////////////////////////////////////////////////////////////
template<class T>
Vector<Vector<T> > Add(const Vector<Vector<T> >& A
			    ,const Vector<Vector<T> >& B) {
  Vector<Vector<T> > r; 
  unsigned int i,j,sax=A.size(),sbx=B.size();
  if(!sax || !say) return(r);
  unsigned int say=A[0].size(),sby=B[0].size();
  unsigned int sx=min(sax,sbx), sy=min(say,sby);
  r.resize(sx);
  for(i=0;i<sx;i++) {
    r[i].resize(sy);
    for(j=0;j<sy;j++) {
      r[i][j] = A[i][j] + B[i][j];
    }
  }
  return(r);
}

///////////////////////////////////////////////////////////////////////
// Matrix = Matrix - Matrix
///////////////////////////////////////////////////////////////////////
template<class T>
Vector<Vector<T> > Subtract(const Vector<Vector<T> >& A
			    ,const Vector<Vector<T> >& B) {
  Vector<Vector<T> > r; 
  unsigned int i,j,sax=A.size(),sbx=B.size();
  if(!sax || !say) return(r);
  unsigned int say=A[0].size(),sby=B[0].size();
  unsigned int sx=min(sax,sbx), sy=min(say,sby);
  r.resize(sx);
  for(i=0;i<sx;i++) {
    r[i].resize(sy);
    for(j=0;j<sy;j++) {
      r[i][j] = A[i][j] + B[i][j];
    }
  }
  return(r);
}


///////////////////////////////////////////////////////////////////////

#endif
