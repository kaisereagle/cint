/**************************************************************************
* darray.h
*
* Array class
*
*  Constructor, copy constructor, destructor, operator overloading
* function overloading, reference type
*
**************************************************************************/

#ifndef G__DARRAY_H
#define G__DARRAY_H

/**********************************************************
* definition of array class
**********************************************************/

int G__arraysize = 100;

class array  {
  int malloced;
 public:
  double *dat;         // pointer to data array
  int n;               // number of data

  array(double start,double stop,int ndat);
  array(int start,int stop,int ndat);
  array(double x);
  array(const array& X);
  array(void);
  array(array& X,int offset,int ndat);
  array(double *p,int ndat,int isnew=0);
  ~array(); 

  void set(double *p,int ndat,int isnew=0);
  void set(double start,double stop) {G__ary_assign(dat,start,stop,n);}
  void set() {G__ary_assign(dat,0.0,(double)(n-1),n);}

  array& operator =(const array& a);

  array operator()(int from,int to);
  double& operator[](int index);

  int resize(int size);
  void push_back(double x);
  int getsize() { return(n); }
  int size() { return(n); }
  void disp() ;
} ;

/***********************************************
* Destructor
***********************************************/
array::~array()
{
  if(malloced) {
    delete[] dat;
  }
}

/***********************************************
* Copy constructor
***********************************************/
array::array(const array& X)
{
  int i;
  if(X.malloced) {
    dat = new double[X.n];
    memcpy(dat,X.dat,X.n*sizeof(double));
    n = X.n;
    malloced=1;
  }
  else {
    dat=X.dat;
    n = X.n;
    malloced=0;
  }
}

/***********************************************
* Implicit conversion constructor 
***********************************************/
array::array(double x)
{
  if(G__arraysize==0) {
    cerr << "Error: Size of array 0\n";
    return;
  }
  dat = new double[G__arraysize];
  G__ary_assign(dat,x,x,G__arraysize);
  n=G__arraysize;
  malloced=1;
}


/***********************************************
* Constructor
***********************************************/
array::array(double start,double stop,int ndat)
{
  double res;
  G__arraysize=ndat;
  dat = new double[G__arraysize];
  G__ary_assign(dat,start,stop,G__arraysize);
  n = G__arraysize;
  malloced=1;
}

/***********************************************
* Constructor
***********************************************/
array::array(int start,int stop,int ndat)
{
  double res;
  G__arraysize=ndat;
  dat = new double[G__arraysize];
  G__ary_assign(dat,(double)start,(double)stop,G__arraysize);
  n = G__arraysize;
  malloced=1;
}

/***********************************************
* constructor 
***********************************************/
array::array(void)
{
  if(G__arraysize==0) {
    cerr << "Error: Size of array 0\n";
    return;
  }
  dat = new double[G__arraysize];
  n=G__arraysize;
  malloced=1;
}

/***********************************************
* constructor 
***********************************************/
array::array(double *p,int ndat,int isnew)
{
  set(p,ndat,isnew);
}

void array::set(double *p,int ndat,int isnew)
{
  G__arraysize=ndat;
  if(isnew==0) {
    dat = p;
    n = G__arraysize;
    malloced=0;
  }
  else {
    dat = new double[ndat];
    memcpy(dat,p,ndat*sizeof(double));
    n=G__arraysize;
    malloced=1;
  }
}


/***********************************************
* constructor for rvalue subarray
***********************************************/
array::array(array& X,int offset,int ndat)
{
  int i;
  dat = new double[ndat];
  if(offset+ndat>X.n) {
    memcpy(dat,X.dat+offset,(X.n-offset)*sizeof(double));
    for(i=X.n-offset;i<ndat;i++) dat[i] = 0.0;
  }
  else {
    memcpy(dat,X.dat+offset,ndat*sizeof(double));
  }
  n = ndat;
  malloced=1;
}

/***********************************************
* resize
***********************************************/
int array::resize(int size)
{
  double *temp;
  if(size<n) {
    temp = new double[size];
    memcpy(temp,dat,sizeof(double)*size);
    if(malloced) delete[] dat;
    dat=temp;
    n=size;
    malloced=1;
  }
  else if(size>n) {
    temp = new double[size];
    memset(temp,0,sizeof(double)*size);
    memcpy(temp,dat,sizeof(double)*n);
    if(malloced) delete[] dat;
    dat=temp;
    n=size;
    malloced=1;
  }
  return(n);
}

/***********************************************
* push_back
***********************************************/
void array::push_back(double x) {
  resize(n+1);
  dat[n]=x;
}

/**********************************************************
* operator = as member function
**********************************************************/
array& array::operator =(const array& a)
{
  int i;
#if 1
  if(a.n!=n) resize(a.n);
  memcpy(dat,a.dat,n*sizeof(double));
#else
  if(malloced && a.malloced) {
    if(a.n<n) memcpy(dat,a.dat,a.n*sizeof(double));
    else      memcpy(dat,a.dat,n*sizeof(double));
  }
  else {
    array c=array(a.dat,a.n,1);
    if(c.n<n) memcpy(dat,c.dat,c.n*sizeof(double));
    else      memcpy(dat,c.dat,n*sizeof(double));
  }
#endif
  return(*this);
}

/**********************************************************
* operator () as member function
**********************************************************/
array array::operator()(int from,int to)
{
  if(from<0 || n<=to) {
    fprintf(stderr,"Error: array index out of range %(d,%d),%d\n"
	    ,from,to,n);
    return(*this);
  }
  else {
    array c=array(dat+from,to-from+1,0);
    return(c);
  }
}

/**********************************************************
* operator [] as member function
**********************************************************/
double& array::operator[](int index)
{
  if(index<0||n<=index) {
    fprintf(stderr,"Error: array index out of range %d/%d\n"
	    ,index,n);
    return(dat[0]);
  }
  return(dat[index]);
}


/***********************************************
* operator +
***********************************************/
array operator +(array& a,array& b)
{
  array c=array(0.0 , 0.0 , a.n);
  int i;
  G__ary_plus(c.dat,a.dat,b.dat,a.n);
  c.n=a.n;
  return(c);
}

/***********************************************
* operator -
***********************************************/
array operator -(array& a,array& b)
{
  array c=array(0.0 , 0.0 , a.n);
  int i;
  G__ary_minus(c.dat,a.dat,b.dat,a.n);
  c.n=a.n;
  return(c);
}

/***********************************************
* operator -
***********************************************/
array operator -(array& a)
{
  array c=array(0.0 , 0.0 , a.n);
  array b=array(0.0 , 0.0 , a.n);
  int i;
  G__ary_minus(c.dat,b.dat,a.dat,a.n);
  c.n=a.n;
  return(c);
}

/***********************************************
* operator *
***********************************************/
array operator *(array& a,array& b)
{
  array c=array(0.0 , 0.0 , a.n);
  int i;
  G__ary_multiply(c.dat,a.dat,b.dat,a.n);
  c.n=a.n;
  return(c);
}

/***********************************************
* operator /
***********************************************/
array operator /(array& a,array& b)
{
  array c=array(0.0 , 0.0 , a.n);
  int i;
  G__ary_divide(c.dat,a.dat,b.dat,a.n);
  c.n=a.n;
  return(c);
}

/***********************************************
* operator %
***********************************************/
array operator %(array& a,array& b)
{
  array c=array(0.0 , 0.0 , a.n);
  int di;
  int i;
  for(i=0;i<a.n;i++) {
    di = int(a.dat[i]/b.dat[i]);
    c.dat[i] = a.dat[i]-di*b.dat[i];
  }
  c.n=a.n;
  return(c);
}

/***********************************************
* operator @ (power)
***********************************************/
array operator @(array& a,array& b)
{
  array c=array(0.0 , 0.0 , a.n);
  int i;
  G__ary_power(c.dat,a.dat,b.dat,a.n);
  c.n=a.n;
  return(c);
}

/***********************************************
* operator << (shift)
***********************************************/
array operator <<(array& a,int shift)
{
  array c=array(0.0 , 0.0 , a.n);
  int i;
  for(i=0;i<a.n-shift;i++) {c.dat[i] = a.dat[i+shift] ;}
  c.n=a.n;
  return(c);
}

/***********************************************
* operator >> (shift)
***********************************************/
array operator >>(array& a,int shift)
{
  array c=array(0.0 , 0.0 , a.n);
  int i;
  for(i=0;i<a.n-shift;i++) {c.dat[i+shift] = a.dat[i] ;}
  c.n=a.n;
  return(c);
}


/**********************************************************
* class array function overloading
**********************************************************/

/***********************************************
* exp
***********************************************/
array exp(array& a)
{
  array c=array(0.0 , 0.0 , a.n);
  int i;
  G__ary_exp(c.dat,a.dat,a.n);
  c.n=a.n;
  return(c);
}

/***********************************************
* log
***********************************************/
array log(array& a)
{
  array c=array(0.0 , 0.0 , a.n);
  int i;
  G__ary_log(c.dat,a.dat,a.n);
  c.n=a.n;
  return(c);
}

/***********************************************
* log10
***********************************************/
array log10(array& a)
{
  array c=array(0.0 , 0.0 , a.n);
  int i;
  G__ary_log10(c.dat,a.dat,a.n);
  c.n=a.n;
  return(c);
}

/***********************************************
* sinc
***********************************************/
array sinc(array& a)
{
  array c=array(0.0 , 0.0 , a.n);
  int i;
  G__ary_sinc(c.dat,a.dat,a.n);
  c.n=a.n;
  return(c);
}

/***********************************************
* sin
***********************************************/
array sin(array& a)
{
  array c=array(0.0 , 0.0 , a.n);
  int i;
  G__ary_sin(c.dat,a.dat,a.n);
  c.n=a.n;
  return(c);
}

/***********************************************
* cos
***********************************************/
array cos(array& a)
{
  array c=array(0.0 , 0.0 , a.n);
  int i;
  G__ary_cos(c.dat,a.dat,a.n);
  c.n=a.n;
  return(c);
}

/***********************************************
* tan
***********************************************/
array tan(array& a)
{
  array c=array(0.0 , 0.0 , a.n);
  int i;
  G__ary_tan(c.dat,a.dat,a.n);
  c.n=a.n;
  return(c);
}

/***********************************************
* sinh
***********************************************/
array sinh(array& a)
{
  array c=array(0.0 , 0.0 , a.n);
  int i;
  G__ary_sinh(c.dat,a.dat,a.n);
  c.n=a.n;
  return(c);
}

/***********************************************
* cosh
***********************************************/
array cosh(array& a)
{
  array c=array(0.0 , 0.0 , a.n);
  int i;
  G__ary_cosh(c.dat,a.dat,a.n);
  c.n=a.n;
  return(c);
}

/***********************************************
* tanh
***********************************************/
array tanh(array& a)
{
  array c=array(0.0 , 0.0 , a.n);
  int i;
  G__ary_tanh(c.dat,a.dat,a.n);
  c.n=a.n;
  return(c);
}

/***********************************************
* asin
***********************************************/
array asin(array& a)
{
  array c=array(0.0 , 0.0 , a.n);
  int i;
  G__ary_asin(c.dat,a.dat,a.n);
  c.n=a.n;
  return(c);
}

/***********************************************
* acos
***********************************************/
array acos(array& a)
{
  array c=array(0.0 , 0.0 , a.n);
  int i;
  G__ary_acos(c.dat,a.dat,a.n);
  c.n=a.n;
  return(c);
}

/***********************************************
* atan
***********************************************/
array atan(array& a)
{
  array c=array(0.0 , 0.0 , a.n);
  int i;
  G__ary_atan(c.dat,a.dat,a.n);
  c.n=a.n;
  return(c);
}

/***********************************************
 * abs
 ***********************************************/
array abs(array& a)
{
  array c=array(0.0 , 0.0 , a.n);
  int i;
  G__ary_fabs(c.dat,a.dat,a.n);
  c.n=a.n;
  return(c);
}

/***********************************************
 * fabs
 ***********************************************/
array fabs(array& a)
{
  array c=array(0.0 , 0.0 , a.n);
  int i;
  G__ary_fabs(c.dat,a.dat,a.n);
  c.n=a.n;
  return(c);
}

/***********************************************
 * sqrt
 ***********************************************/
array sqrt(array& a)
{
  array c=array(0.0 , 0.0 , a.n);
  int i;
  G__ary_sqrt(c.dat,a.dat,a.n);
  c.n=a.n;
  return(c);
}

/***********************************************
 * rect
 ***********************************************/
array rect(array& a)
{
  array c=array(0.0 , 0.0 , a.n);
  int i;
  G__ary_rect(c.dat,a.dat,a.n);
  c.n=a.n;
  return(c);
}

/***********************************************
 * square
 ***********************************************/
array square(array& a)
{
  array c=array(0.0 , 0.0 , a.n);
  int i;
  G__ary_square(c.dat,a.dat,a.n);
  c.n=a.n;
  return(c);
}

/***********************************************
  * rand
  ***********************************************/
array rand(array& a)
{
  array c=array(0.0 , 0.0 , a.n);
  int i;
  G__ary_rand(c.dat,a.dat,a.n);
  c.n=a.n;
  return(c);
}

/***********************************************
  * pulse
  ***********************************************/
array pulse(array& a)
{
  
  array c=array(0.0 , 0.0 , a.n);
  int i;
  G__ary_pulse(c.dat,a.dat,a.n);
  c.n=a.n;
  return(c);
}


/***********************************************
 * conv cross convolution
 ***********************************************/
array conv(array& a,array& b)
{
  array c=array(0.0 , 0.0 , a.n);
  int i;
  G__ary_conv(c.dat,a.dat,a.n,b.dat,b.n);
  c.n=a.n;
  return(c);
}

/***********************************************
 * integ
 ***********************************************/
array integ(array& a,array& b) // a : x , b : y
{
  array c=array(0.0 , 0.0 , a.n);
  int i;
  G__ary_integ(c.dat,a.dat,b.dat,a.n);
  c.n=a.n;
  return(c);
}

/***********************************************
 * diff differential
 ***********************************************/
array diff(array& a,array& b) // a : x , b : y
{
  array c=array(0.0 , 0.0 , a.n);
  int i;
  G__ary_diff(c.dat,a.dat,b.dat,a.n);
  c.n=a.n;
  return(c);
}

/***********************************************
 * max
 ***********************************************/
array max(array& a,array& b) // a : x , b : y
{
  array c=array(0.0 , 0.0 , a.n);
  int i;
  G__ary_max(c.dat,a.dat,b.dat,a.n);
  c.n=a.n;
  return(c);
}

double max(array& a) {
  double r=-1e99;
  for(int i=0;i<a.size();i++) if(r<a[i]) r=a[i];
  return(r);
}

/***********************************************
 * min
 ***********************************************/
array min(array& a,array& b) // a : x , b : y
{
  array c=array(0.0 , 0.0 , a.n);
  int i;
  G__ary_min(c.dat,a.dat,b.dat,a.n);
  c.n=a.n;
  return(c);
}

double min(array& a) {
  double r=1e99;
  for(int i=0;i<a.size();i++) if(r>a[i]) r=a[i];
  return(r);
}

/***********************************************
 * rms
 ***********************************************/
double rms(const array v) {
  double vmean = mean(v);
  int i;
  double sum=0;
  double tmp;
  for(i=0;i<v.size();i++) {
    tmp = v[i]-vmean;
    sum += tmp*tmp;
  }
  return(sqrt(sum/v.size()));
}

/***********************************************
 * p-p
 ***********************************************/
double pp(const array v) {
  double min=1e99,max=-1e99;
  int i;
  for(i=0;i<v.size();i++) {
    if(min>v[i]) min = v[i];
    if(v[i]>max) max = v[i];
  }
  return(max-min);
}


/***********************************************
 * gauss
 ***********************************************/
array gauss(const array& scale,const array& hist) {
  array g(0.0,0.0,hist.size());
  double m=sum(scale*hist);
  array s2 = scale-m;
  double sd = sqrt(sum(s2*s2*hist));
  double pi2sqsd = 1/sqrt(2*3.141592)*sd;
  double sd22 = 2*sd*sd;
  double sum=0;
  for(int i=0;i<hist.size();i++) {
    g[i] = pi2sqsd * exp(-pow(scale[i]-m,2)/sd22);
    sum += g[i];
  } 
  g = g/sum;
  return(g);
}

#include <statistics.h>
/***********************************************
 * min
 ***********************************************/
statistics stat(array& a) // a : x , b : y
{
  statistics result;
  int i;
  for(i=0;i<a.n;i++) result.add(a.dat[i]);
  return(result);
}
/***********************************************
 * mean
 ***********************************************/
double mean(const array& a) {
  double r=0;
  int i;
  for(i=0;i<a.n;i++) r+=a.dat[i];
  return(r/a.n);
}

/***********************************************
 * sum
 ***********************************************/
double sum(const array& a) {
  double r=0;
  int i;
  for(i=0;i<a.n;i++) r+=a.dat[i];
  return(r);
}

/***********************************************
* pow
***********************************************/
array pow(array& a,array& b) {
  array c=array(0.0 , 0.0 , a.n);
  int i;
  G__ary_power(c.dat,a.dat,b.dat,a.n);
  c.n=a.n;
  return(c);
}

/***********************************************
* FIR filter
***********************************************/
array fir(array& in,array& filter) {
  // convolution is done in conv(). This function
  // simply does energy normalization 
  array out(0.0,0.0,in.getsize());
  int i,j,k;
  array fil=array(0.0,0.0 ,filter.n);
  double sum=0;
  k=fil.n;
  for(i=0;i<k;i++) sum += filter.dat[i];
  fil = filter/sum;
  out=conv(in,fil);
  return(out);
}

/***********************************************
* IIR filter
***********************************************/
array iir1_calc(const array& in,double k,double norm) {
  array out(0.0,0.0,in.size());
  int n=in.size();
  double* idat=in.dat;
  double* odat=out.dat;
  odat[0] = idat[0]*k;
  for(int i=1;i<n;i++) {
    odat[i] = (idat[i]+odat[i-1])*k;
  }
  return(out/norm);
}

///////////////////////////////////////////////
array iir1(array& in,double Ts,double tau) {
  ////////////////////////////////////////////
  // IIR low pass filter
  //  |                     .      .    .    1     ----
  //  |             .                                exp(-1) = k^(tau/Ts)
  //  |        .  <---------------------- 1-exp(-1) ---
  //  |    .    
  //  |  .     ^
  //  | .      |
  //   -----------------------------------
  //   ||     tau
  //   Ts
  //
  // From above figure, k is obtained by following formula 
  double k=pow(exp(-1),Ts/tau);
  
  // calculate normalization factor
  //        +----------+
  //        |          |  norm = ƒ°k^i | i=0..infinity
  //        v          |
  //   --->(+)--- *k --+---> /norm --->
  //
  double norm=0,r=k;
  while(r>0.001) { 
    norm += r;
    r *= k;
  }

  printf("IIR1 Ts=%.3g(fs=%.3g) tau=%.3g(fc=%.3g) coefficient k=%.3g\n"
	 ,Ts,1/Ts/2/3.14,tau,1/tau/2/3.14,k);
  return(iir1_calc(in,k,norm));
}

//////////////////////////////////////////////////////////

#ifndef G__ARRAY_H
#include <array.h>
#endif

void array::disp() {
  int n = getsize();
  cout << "(array " << this << ":" << n << ")" ;
#ifdef G__DISPALL
  for(int i=0;i<n-1;i++) cout << dat[i] << ",";
#else
  if(n>20) {
    for(int i=0;i<10;i++) cout << dat[i] << ",";
    cout << ",,,";
    for(int i=n-10;i<n-1;i++) cout << dat[i] << ",";
  }
  else for(int i=0;i<n-1;i++) cout << dat[i] << ",";
#endif
  cout << dat[n-1] << endl;
  //return(1); 
}

#ifdef __CINT__
int G__ateval(const array& x) {
  int n = x.getsize();
  cout << "(array " << &x << ":" << n << ")" ;
#ifdef G__DISPALL
  for(int i=0;i<n-1;i++) cout << x[i] << ",";
#else
  if(n>20) {
    for(int i=0;i<10;i++) cout << x[i] << ",";
    cout << ",,,";
    for(int i=n-10;i<n-1;i++) cout << x[i] << ",";
  }
  else for(int i=0;i<n-1;i++) cout << x[i] << ",";
#endif
  cout << x[n-1] << endl;
  return(1); 
}
#endif

/***********************************************
* Histogram adapter
***********************************************/
class histogram : public array {
 public:
  histogram() {setrange(200);} 
  histogram(int nbin,double minin=0,double maxin=0) 
    {setrange(nbin,minin,maxin);}

  void clear() { m_sum=0; for(int i=0;i<size();i++) dat[i]=0;}

  void setrange(const array& x) 
   {setrange(x.size(),x[1],x[x.size()-2]);}

  void setrange(int nbin,double minin=0,double maxin=0) 
    {resize(nbin); setrange(minin,maxin); clear() ;}

  void setrange(double minin=0,double maxin=0) {
    m_min=minin; m_max=maxin; 
    m_step=(m_max-m_min)/(size()-(2+0.001/size()));
    if(minin!=maxin) genscale();
  }

  void fill(const array& x) {
    if(m_step==0) setrange(min(x),max(x));
    int index;
    for(int i=0;i<x.size();i++) {
      index = int((x[i]-m_min)/m_step)+1;
      if(index<0) index=0;
      else if(index>=size()) index=size()-1;
      dat[index] += 1.0;
    }
    m_sum += x.size();
  }

  array getarray() const { array *p = (array*)this; return(*p); }

  void normalize() {
    if(m_sum==0) return;
    for(int i=0;i<size();i++) dat[i] = dat[i]/m_sum;
    m_sum=1.0;
  }

  int   nbin() const { return(size()); }
  double min() const { return(m_min); }
  double max() const { return(m_max); }
  double step() const { return(m_step); }
  double sum() const { return(m_sum); }
  array scale() const { return(m_scale); }
  void genscale() {
    m_scale.resize(size());
    for(int i=0;i<size();i++) m_scale[i] = m_min+m_step*(i-1);
  }
  double stddev() const {
    double m=mean();
    array s = scale();
    s = s-m;
    s = s*s;
    s = sqrt(sum(s*(*this)));
    return(s);
  }
  double rms() const { return(stddev()); }
  double mean() const {
    result r = sqrt(sum(scale()*(*this)));
    return(r);
  }
  histogram gauss() const {
    histogram g(size(),m_min,m_max);
    double m=mean();
    double sd=stddev();
    double pi2sqsd = 1/sqrt(2*3.141592)*sd;
    double sd22 = 2*sd*sd;
    double sum=0;
    for(int i=0;i<size();i++) {
      g[i] = pi2sqsd * exp(pow(m_scale[i]-m,2));
      sum += g[i];
    } 
    g = g/sum;
    return(g);
  }

 private:
  array  m_scale;
  double m_min;
  double m_max;
  double m_step;
  double m_sum;
};

//////////////////////////////////////////////////////////
array hist(const array& x,int nbin=100,double xmin=0.0,double xmax=0.0) {
  histogram h;
  h.setrange(nbin,xmin,xmax);
  h.fill(x);
  return(h.getarray());
}

array xhist(const array& y,int nbin=100) {
  array x(0.0,0.0,nbin);
  x.set(min(y),max(y));
  return(x);
}


#endif



