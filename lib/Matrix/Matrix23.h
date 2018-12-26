/***********************************************************************
* Matrix23.h
*
***********************************************************************/

#ifndef MATRIX23
#define MATRIX23

////////////////////////////////////////////////////////////////////
// 1ŽŸŒ³”z—ñ = Matrix ‚Ìutility
template<class T> class Matrix1 {
 public:
  Matrix1() : mat(0), Xsize(0) {}
  Matrix1(const Matrix1& a) {
    Allocate(a.XSize());
    const T* dat=a.Data();
    for(int i=0;i<a.XSize();i++) {
      mat[i] = dat[i];
    }
  }
  Matrix1& operator=(const Matrix1& a) {
    Deallocate();
    Allocate(a.XSize());
    const T* dat=a.Data();
    for(int i=0;i<a.XSize();i++) {
      mat[i] = dat[i];
    }
    return(*this);
  }
  Matrix1& operator=(const T& a) {
    for(int i=0;i<XSize();i++) {
      mat[i] = a;
    }
    return(*this);
  }
  ~Matrix1() { Deallocate(); }
  void Allocate(int XsizeIn) {
    Deallocate();
    Xsize = XsizeIn;
    mat = new T[Xsize];
    //Init();
  }
#if 0
  void Allocate(double start,double stop,int Xsizein) { 
    Deallocate();
    Xsize = XsizeIn;
    mat = new T[Xsize];
    double step=(stop-start)/(Xsize-1);
    for(int i=0;i<Xsize;++i) mat[i] = i*step + start;
  }
  Matrix1() : mat(0), Xsize(0) (double start,double stop,int Xsizein) 
   { Allocate(start,stop,Xsizein); }
  void Allocate(int start,int stop,int Xsizein) 
   {Allocate((double)start,(double)stop,Xsizein);}
  Matrix1() : mat(0), Xsize(0) (int start,int stop,int Xsizein) 
   { Allocate(start,stop,Xsizein); }
#endif
  void Init() { }
  void Deallocate() { if(mat) delete[] mat; mat=0; }
  T& operator()(int i) {return(mat[i]);}
  int XSize() const { return(Xsize); }
  int Size() const { return(Xsize); }
  T* Data() { return(mat); }
  const T* Data() const { return(mat); }

  void print() ;
 private:
  T *mat;
  int Xsize;
};

template<class T> 
void Matrix1<T>::print() {
  int i;
  for(i=0;i<Xsize;i++) {
    printf("%c",mat[i].S());
  }
  printf("\n");
}

inline void Matrix1<double>::Init() {for(int i=0;i<Xsize;i++) mat[i]=0;}

////////////////////////////////////////////////////////////////////
template<class T>
inline Matrix1<T> operator+(Matrix1<T>& a,Matrix1<T>& b) {
  Matrix1<double> r;
  r.Allocate(a.XSize());
  int Xsize=a.XSize();
  T* dat=r.Data();
  T* data=a.Data();
  T* datb=b.Data();
  for(int i=0;i<Xsize;i++) {
    dat[i] = data[i]+datb[i];
  }
  return(r);
}

////////////////////////////////////////////////////////////////////
template<class T>
inline Matrix1<T> operator-(Matrix1<T>& a,Matrix1<T>& b) {
  Matrix1<double> r;
  r.Allocate(a.XSize());
  int Xsize=a.XSize();
  T* dat=r.Data();
  T* data=a.Data();
  T* datb=b.Data();
  for(int i=0;i<Xsize;i++) {
    dat[i] = data[i]-datb[i];
  }
  return(r);
}

////////////////////////////////////////////////////////////////////
inline void Matrix1<double>::print() {
  int i;
  for(i=0;i<Xsize;i++) {
    printf("%g,",mat[i]);
  }
  printf("\n");
}


////////////////////////////////////////////////////////////////////
// 2ŽŸŒ³”z—ñ = Matrix ‚Ìutility
template<class T> class Matrix2 {
 public:
  Matrix2() : mat(0), Xsize(0), Ysize(0) {}
  Matrix2(const Matrix2& a) {
    Allocate(a.XSize(),a.YSize());
    const T* dat=a.Data();
    for(int i=0;i<a.XSize();i++) {
      for(int j=0;j<a.YSize();j++) {
        mat[i*Ysize+j] = dat[i*Ysize+j];
      }
    }
  }
  Matrix2& operator=(const Matrix2& a) {
    Deallocate();
    Allocate(a.XSize(),a.YSize());
    const T* dat=a.Data();
    for(int i=0;i<a.XSize();i++) {
      for(int j=0;j<a.YSize();j++) {
        mat[i*Ysize+j] = dat[i*Ysize+j];
      }
    }
    return(*this);
  }
  Matrix2& operator=(const T& a) {
    for(int i=0;i<XSize();i++) {
      for(int j=0;j<YSize();j++) {
        mat[i*Ysize+j] = a;
      }
    }
    return(*this);
  }
  ~Matrix2() { Deallocate(); }
  void Allocate(int XsizeIn,int YsizeIn) {
    Deallocate();
    Xsize = XsizeIn;
    Ysize = YsizeIn;
    mat = new T[Xsize*Ysize];
    //Init();
  }
#if 0
  void Allocate(double xstart,double xstop,int Xsizein
               ,double ystart,double ystop,int Ysizein) { 
    Deallocate();
    Xsize = XsizeIn;
    Ysize = YsizeIn;
    mat = new T[Xsize*Ysize];
    double xstep=(xstop-xstart)/(Xsize-1);
    double ystep=(ystop-ystart)/(Ysize-1);
    for(int i=0;i<Xsize;++i) mat[i] = i*xstep + xstart;
  }
  Matrix2() : mat(0), Xsize(0), Ysize(0) (double start,double stop,int Xsizein) 
   { Allocate(start,stop,Xsizein); }
  void Allocate(int start,int stop,int Xsizein) 
   {Allocate((double)start,(double)stop,Xsizein);}
  Matrix2() : mat(0), Xsize(0) , Ysize(0) (int start,int stop,int Xsizein) 
   { Allocate(start,stop,Xsizein); }
#endif
  void Init() { }
  void Deallocate() { if(mat) delete[] mat; mat=0; }
  T& operator()(int i,int j) {return(mat[i*Ysize+j]);}
  int XSize() const { return(Xsize); }
  int YSize() const { return(Ysize); }
  int Size() const { return(Xsize*Ysize); }
  T* Data() { return(mat); }
  const T* Data() const { return(mat); }

  void print() ;

 private:
  T *mat;
  int Xsize;
  int Ysize;
};

////////////////////////////////////////////////////////////////////
template<class T> 
void Matrix2<T>::print() {
  int i,j;
  for(j=0;j<Ysize;j++) {
    printf("j%d ",j);
    for(i=0;i<Xsize;i++) {
      printf("%c",mat[i*Ysize+j].S());
    }
    printf("\n");
  }
  printf("\n");
}

inline void Matrix2<char>::print() {
  int i,j;
  for(j=0;j<Ysize;j++) {
    printf("j%-3d ",j);
    for(i=0;i<Xsize;i++) {
      if(mat[i*Ysize+j]==0) printf(".");
      else if(mat[i*Ysize+j]==1) printf("1");
      else printf("%c",'0'+mat[i*Ysize+j]);
    }
    printf("\n");
  }
  printf("\n");
}

inline void Matrix2<double>::Init() {
  for(int i=0;i<Xsize*Ysize;i++) mat[i]=0.0;
}

////////////////////////////////////////////////////////////////////
template<class T>
inline Matrix2<T> operator+(Matrix2<T>& a,Matrix2<T>& b) {
  Matrix2<double> r;
  r.Allocate(a.XSize(),a.YSize());
  int Xsize=a.XSize();
  int Ysize=a.YSize();
  T* dat=r.Data();
  T* data=a.Data();
  T* datb=b.Data();
  for(int i=0;i<Xsize;i++) {
    for(int j=0;j<a.YSize();j++) {
      dat[i*Ysize+j] = data[i*Ysize+j]+datb[i*Ysize+j];
    }
  }
  return(r);
}

////////////////////////////////////////////////////////////////////
template<class T>
inline Matrix2<T> operator-(Matrix2<T>& a,Matrix2<T>& b) {
  Matrix2<T> r;
  r.Allocate(a.XSize(),a.YSize());
  int Xsize=a.XSize();
  int Ysize=a.YSize();
  T* dat=r.Data();
  T* data=a.Data();
  T* datb=b.Data();
  for(int i=0;i<Xsize;i++) {
    for(int j=0;j<a.YSize();j++) {
      dat[i*Ysize+j] = data[i*Ysize+j]-datb[i*Ysize+j];
    }
  }
  return(r);
}

////////////////////////////////////////////////////////////////////
inline void Matrix2<double>::print() {
  int i,j;
  for(j=0;j<Ysize;j++) {
    for(i=0;i<Xsize;i++) {
      printf("%g,",mat[i*Ysize+j]);
    }
    printf("\n");
  }
  printf("\n");
}


////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
// 3ŽŸŒ³”z—ñ = Matrix3 ‚Ìutility
template<class T> class Matrix3 {
 public:
  Matrix3() : mat(0), Xsize(0), Ysize(0) , Zsize(0) {}
  Matrix3(const Matrix3& a) {
    Allocate(a.XSize(),a.YSize(),a.ZSize());
    const T* dat=a.Data();
    int Xsize=a.XSize();
    int Ysize=a.YSize();
    int Zsize=a.ZSize();
    for(int i=0;i<Xsize;i++) {
      for(int j=0;j<Ysize;j++) {
        for(int k=0;k<Zsize;k++) {
          mat[(i*Ysize+j)*Zsize+k] = dat[(i*Ysize+j)*Zsize+k];
        }
      }
    }
  }
  Matrix3& operator=(const Matrix3& a) {
    Deallocate();
    Allocate(a.XSize(),a.YSize(),a.ZSize());
    const T* dat=a.Data();
    int Xsize=a.XSize();
    int Ysize=a.YSize();
    int Zsize=a.ZSize();
    for(int i=0;i<Xsize;i++) {
      for(int j=0;j<Ysize;j++) {
        for(int k=0;k<Zsize;k++) {
          mat[(i*Ysize+j)*Zsize+k] = dat[(i*Ysize+j)*Zsize+k];
        }
      }
    }
    return(*this);
  }
  Matrix3& operator=(const T& a) {
    int Xsize=XSize();
    int Ysize=YSize();
    int Zsize=ZSize();
    for(int i=0;i<Xsize;i++) {
      for(int j=0;j<Ysize;j++) {
        for(int k=0;k<Zsize;k++) {
          mat[(i*Ysize+j)*Zsize+k] = a;
        }
      }
    }
    return(*this);
  }
  ~Matrix3() { Deallocate(); }
  void Init() { }
  void Allocate(int XsizeIn,int YsizeIn,int ZsizeIn) {
    Deallocate();
    Xsize = XsizeIn;
    Ysize = YsizeIn;
    Zsize = ZsizeIn;
    mat = new T[Xsize*Ysize*Zsize];
    //Init();
  }
  void Deallocate() { if(mat) delete[] mat; mat=0; }
  T& operator()(int i,int j,int k) {return(mat[(i*Ysize+j)*Zsize+k]);}
  int XSize() const { return(Xsize); }
  int YSize() const { return(Ysize); }
  int ZSize() const { return(Zsize); }
  int Size() const { return(Xsize*Ysize*Zsize); }
  T* Data() { return(mat); }
  const T* Data() const { return(mat); }

  void Print() { printf("%d:%d:%d\n",Xsize,Ysize,Zsize); }
  void print() ;
 private:
  T *mat;
  int Xsize;
  int Ysize;
  int Zsize;
};

template<class T> 
void Matrix3<T>::print() {

  int i,j,k;
  for(k=0;k<Zsize;k++) {
    printf("k%d ",k);
    for(j=0;j<Ysize;j++) {
      printf("j%d ",j);
      for(i=0;i<Xsize;i++) {
        printf("%c",mat[(i*Ysize+j)*Zsize+k].S());
      }
      printf("\n");
    }
    printf("\n");
  }
  printf("\n");
}

inline void Matrix3<double>::Init() {
  for(int i=0;i<Xsize*Ysize*Zsize;i++) mat[i]=0.0;
}

////////////////////////////////////////////////////////////////////
template<class T>
inline Matrix3<T> operator+(Matrix3<T>& a,Matrix3<T>& b) {
  Matrix3<double> r;
  r.Allocate(a.XSize(),a.YSize(),a.ZSize());
  int Xsize=a.XSize();
  int Ysize=a.YSize();
  int Zsize=a.ZSize();
  T* dat=r.Data();
  T* data=a.Data();
  T* datb=b.Data();
  for(int i=0;i<Xsize;i++) {
    for(int j=0;j<Ysize;j++) {
      for(int k=0;k<Zsize;k++) {
        dat[(i*Ysize+j)*Zsize+k] = data[(i*Ysize+j)*Zsize*k]+datb[(i*Ysize+j)*Zsize+k];
      }
    }
  }
  return(r);
}

////////////////////////////////////////////////////////////////////
template<class T>
inline Matrix3<T> operator-(Matrix3<T>& a,Matrix3<T>& b) {
  Matrix3<double> r;
  r.Allocate(a.XSize(),a.YSize(),a.ZSize());
  int Xsize=a.XSize();
  int Ysize=a.YSize();
  int Zsize=a.ZSize();
  T* dat=r.Data();
  T* data=a.Data();
  T* datb=b.Data();
  for(int i=0;i<Xsize;i++) {
    for(int j=0;j<Ysize;j++) {
      for(int k=0;k<Zsize;k++) {
        dat[(i*Ysize+j)*Zsize+k] = data[(i*Ysize+j)*Zsize*k]-datb[(i*Ysize+j)*Zsize+k];
      }
    }
  }
  return(r);
}

////////////////////////////////////////////////////////////////////
inline void Matrix3<double>::print() {
  int i,j,k;
  for(k=0;k<Zsize;k++) {
    for(j=0;j<Ysize;j++) {
      for(i=0;i<Xsize;i++) {
        printf("%g,",mat[(i*Ysize+j)*Zsize+k]);
      }
      printf("\n");
    }
    printf("\n");
  }
  printf("\n");
}

#if 1
////////////////////////////////////////////////////////////////////
template<class T> struct cordinate1 {
  cordinate1() { x=0; }
  cordinate1(T a) { x=a; }
  void set(T a)  {x=a;}
  void get(T& a) {a=x;}
  double max() const { return(x); }
  double min() const { return(x); }
  double amax() const { return(fabs(x)); }
  double amin() const { return(fabs(x)); }
  double volume() const { return(x); }
  void disp(FILE* fp=stdout) const {fprintf(fp,"(%g)",(double)x);}
  void disp(char *s) const { sprintf(s,"(%g)",(double)x); }
  T x;
};
typedef cordinate1<int> cordinate1D;
typedef cordinate1<double> vector1D;
////////////////////////////////////////////////////////////////////
template<class T> struct cordinate2 {
  cordinate2() { x=0; y=0; }
  cordinate2(T a) { x=a; y=a; }
  cordinate2& operator=(const cordinate2& a) { x=a.x; y=a.y; return(*this); }
  cordinate2& operator+=(const cordinate2& a) { x+=a.x; y+=a.y; return(*this); }
  void set(T a,T b)   {x=a;y=b;}
  void get(T& a,T& b) {a=x;b=y;}
  double max() const { return(x>y?x:y); }
  double min() const { return(x<y?x:y); }
  double amax() const 
   {double mx=fabs(max()); double mi=fabs(min()); return(mx>mi?mx:mi);}
  double amin() const 
   {double mx=fabs(max()); double mi=fabs(min()); return(mx<mi?mx:mi);}
  double volume() const { return(x*y); }
  void disp(FILE* fp=stdout) const {fprintf(fp,"(%g,%g)",(double)x,(double)y);}
  void disp(char *s) const { sprintf(s,"(%g,%g)",(double)x,(double)y); }
  T x;T y; 
};
typedef cordinate2<int> cordinate2D;
typedef cordinate2<double> vector2D;
inline vector2D operator+(const vector2D& a,const vector2D& b) 
  {vector2D c; c.x=a.x+b.x; c.y=a.y+b.y; return(c);}
inline vector2D operator-(const vector2D& a,const vector2D& b) 
  {vector2D c; c.x=a.x-b.x; c.y=a.y-b.y; return(c);}
inline vector2D operator*(const vector2D& a,const vector2D& b) 
  {vector2D c; c.x=a.x*b.x; c.y=a.y*b.y; return(c);}
inline vector2D operator/(const vector2D& a,const vector2D& b) 
  {vector2D c; c.x=a.x/b.x; c.y=a.y/b.y; return(c);}
////////////////////////////////////////////////////////////////////
template<class T> struct cordinate3 {
  cordinate3() { x=0; y=0; z=0; }
  cordinate3(T a) { x=a; y=a; z=a; }
  cordinate3& operator=(const cordinate3& a) {x=a.x;y=a.y;z=a.z;return(*this);}
  cordinate3& operator+=(const cordinate3& a) {x+=a.x;y+=a.y;z+=a.z;return(*this);}
  void set(T a,T b,T c)    {x=a;y=b;z=c;}
  void get(T& a,T& b,T& c) {a=x;b=y;c=z;}
  double max() const { return((x>y&&x>z)?x:(y>z?y:z)); }
  double min() const { return((x<y&&x<z)?x:(y<z?y:z)); }
  double amax() const 
   {double mx=fabs(max()); double mi=fabs(min()); return(mx>mi?mx:mi);}
  double amin() const 
   {double mx=fabs(max()); double mi=fabs(min()); return(mx<mi?mx:mi);}
  double volume() const { return(x*y*z); }
  void disp(FILE* fp=stdout) const { fprintf(fp,"(%g,%g,%g)",(double)x,(double)y,(double)z); }
  void disp(char* s) const { sprintf(s,"(%g,%g,%g)",(double)x,(double)y,(double)z); }
  T x;T y;T z; 
};
typedef cordinate3<int> cordinate3D;
typedef cordinate3<double> vector3D;
inline vector3D operator+(const vector3D& a,const vector3D& b) 
  {vector3D c; c.x=a.x+b.x; c.y=a.y+b.y; c.z=a.z+b.z; return(c);}
inline vector3D operator-(const vector3D& a,const vector3D& b) 
  {vector3D c; c.x=a.x-b.x; c.y=a.y-b.y; c.z=a.z-b.z; return(c);}
inline vector3D operator*(const vector3D& a,const vector3D& b) 
  {vector3D c; c.x=a.x*b.x; c.y=a.y*b.y; c.z=a.z*b.z; return(c);}
inline vector3D operator/(const vector3D& a,const vector3D& b) 
  {vector3D c; c.x=a.x/b.x; c.y=a.y/b.y; c.z=a.z/b.z; return(c);}

#else

////////////////////////////////////////////////////////////////////
struct cordinate1D { 
  void set(int a) {x=a;}
  void get(int& a) {a=x;}
  int x; 
};
////////////////////////////////////////////////////////////////////
struct cordinate2D { 
  void set(int a,int b) {x=a;y=b;}
  void get(int& a,int& b) {a=x;b=y;}
  int x;int y; 
};
////////////////////////////////////////////////////////////////////
struct cordinate3D { 
  void set(int a,int b,int c) {x=a;y=b;z=c;}
  void get(int& a,int& b,int& c) {a=x;b=y;c=z;}
  int x;int y; int z; 
};
#endif

////////////////////////////////////////////////////////////////////
inline double Distance(const cordinate1D& a,const cordinate1D& b) {
  double x=(a.x-b.x);
  return(x);
}
////////////////////////////////////////////////////////////////////
inline double Distance(const cordinate2D& a,const cordinate2D& b) {
  double x=(a.x-b.x);
  double y=(a.y-b.y);
  return(sqrt(x*x+y*y));
}
////////////////////////////////////////////////////////////////////
inline double Distance(const cordinate3D& a,const cordinate3D& b) {
  double x=(a.x-b.x);
  double y=(a.y-b.y);
  double z=(a.z-b.z);
  return(sqrt(x*x+y*y+z*z));
}

////////////////////////////////////////////////////////////////////
#endif
