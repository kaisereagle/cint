
#ifndef G__NVECTOR
#define G__NVECTOR

//#include <vector>
//#ifndef G__NVECTOR
#pragma include <NVector.dll>
//#endif
//#include <Vectoriostream.h> // included in NVector.dll
//#include <iostream>

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
G__FFT& G__FFT::operator>>(G__CINT_ENDL c)
{
  *this << '\n';
  return(*this);
}
G__FFT& G__FFT::operator<<(G__CINT_ENDL c)
{
  *this << '\n';
  return(*this);
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////



#if 1
//////////////////////////////////////////////////////////////////////
template<class T>
int G__ateval(const Vector<T>& x) {
  x.disp();
  return(1); 
}
#else
int G__ateval(const DVector& a) {
  int n = a.size();
  printf("(DVector 0x%p:%d)",&a,n);
  unsigned int i;
#ifdef G__DISPALL
  for(i=0;i<n;i++) printf("%g,",a[i]);
#else
  if(n>20) {
    for(i=0;i<10;i++) printf("%g,",a[i]);
    cout << ",,,";
    for(i=n-10;i<n;i++) printf("%g,",a[i]);
  }
  else for(i=0;i<n;i++) printf("%g,",a[i]);
#endif
  printf("\n");
}

int G__ateval(const DCVector& a) {
  int n = a.size();
  printf("(DCVector 0x%p:%d)",&a,n);
  unsigned int i;
#ifdef G__DISPALL
  for(i=0;i<n;i++) printf("(%g,%g),",a[i].real(),a[i].imag());
#else
  if(n>20) {
    for(i=0;i<10;i++) printf("(%g,%g),",a[i].real(),a[i].imag());
    cout << ",,,";
    for(i=n-10;i<n;i++) printf("(%g,%g),",a[i].real(),a[i].imag());
  }
  else for(i=0;i<n;i++) printf("(%g,%g),",a[i].real(),a[i].imag());
#endif
  printf("\n");
}
#endif

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
DMatrix ToDMatrix(const DVector& x,const DVector& y
	      ,double (*f)(double a,double b)) {
  DVector v;
  DMatrix m;
  v.resize(x.size());
  for(int j=0;j<y.size();j++) {
    for(int i=0;i<x.size();i++) {
      v[i] = f(x[i],y[j]);
    }
    m.push_back(v);
  }
  return(m);
}


#endif // G__NVECTOR
