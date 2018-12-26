/**************************************************************************
 * Fft.h
 *
 *  FFT << time << x_in >> freq >> complex_out >> '\n';
 *  IFFT << freq << complex_in >> time >> x_out >> '\n';
 *  SPECTRUM << time << x_in >> freq >> spect_out >> '\n';
 *  CEPSTRUM << time << x_in >> else freq >> cepst_out >> '\n';
 *
 **************************************************************************/
#ifndef G__NFFT
#define G__NFFT

#if 1
#include "Vector.h"
#else
#include "NArray.h"
#endif
//#include <iostream>

/**************************************************************************
*
**************************************************************************/
int dft_cal(double *x,double *re,double *im,int ndat);
int fft_cal(double *x,double *re,double *im,int ndat);
int fft(double *x,double *re,double *im,int ndat);
int ifft(double *x,double *re,double *im,int ndat);
int spectrum(double *x,double *y,int ndat);
int colleration(double *x,double *y,int ndat);
int cepstrum(double *x,double *y,int ndat);

//////////////////////////////////////////////////////////////////
int fft(DVector& x,DVector& re,DVector& im) {
  if( x.size()!=re.size()) re.resize(x.size());
  if( x.size()!=im.size() ) im.resize(x.size());
  return(fft(&x[0],&re[0],&im[0],x.size()));
}

//////////////////////////////////////////////////////////////////
int ifft(DVector& x,DVector& re,DVector& im) {
  if( x.size()!=re.size()) re.resize(x.size());
  if( x.size()!=im.size() ) im.resize(x.size());
  return(ifft(&x[0],&re[0],&im[0],x.size()));
}

//////////////////////////////////////////////////////////////////
int spectrum(DVector& x,DVector& re) {
  if( x.size()!=re.size()) re.resize(x.size());
  return(spectrum(&x[0],&re[0],x.size()));
}

//////////////////////////////////////////////////////////////////
int correlation(DVector& x,DVector& re) {
  if( x.size()!=re.size()) re.resize(x.size());
  return(colleration(&x[0],&re[0],x.size()));
}

//////////////////////////////////////////////////////////////////
int cepstrum(DVector& x,DVector& re) {
  if( x.size()!=re.size()) re.resize(x.size());
  return(cepstrum(&x[0],&re[0],x.size()));
}

//////////////////////////////////////////////////////////////////
int fft(DVector& x,DCVector& y) {
  if( x.size()!=y.size()) y.resize(x.size());
  DVector re(x.size());
  DVector im(x.size());
  int r=fft(&x[0],&re[0],&im[0],x.size());
  DVector2DCVector(re,im,y);
  return(r);
}

//////////////////////////////////////////////////////////////////
int ifft(DVector& x,DCVector& y) {
  if( x.size()!=y.size()) y.resize(x.size());
  DVector re(x.size());
  DVector im(x.size());
  int r = ifft(&x[0],&re[0],&im[0],x.size());
  DVector2DCVector(re,im,y);
  return(r);
}


#define IDEN_FFT      1
#define IDEN_IFFT     2
#define IDEN_SPECTRUM 3
#define IDEN_CEPSTRUM 4


union G__FFT_DATA {
  DCVector *pc;
  DVector  *pa[2];
};

class G__FFT {

  enum DATATYPE { 
    G__DVECTORTYPE, G__DCVECTORTYPE, G__DCVECTORTYPE_RE, G__DCVECTORTYPE_IM 
  };

  int icount,ocount;
  int iden;

  DVector *xin;
  G__FFT_DATA yin;
  int iden_yin;

  DVector *xout;
  G__FFT_DATA yout;
  int iden_yout;

public:
  G__FFT(int identity); 
  G__FFT& operator <<(DVector& x);
  G__FFT& operator >>(DVector& x);
  G__FFT& operator <<(DCVector& x);
  G__FFT& operator >>(DCVector& x);
  G__FFT& operator <<(char c);
  G__FFT& operator <<(char *s);
  G__FFT& operator >>(char c);
  G__FFT& operator >>(char *s);
private:
  void sub_init(void);
  void do_fft(void);
  void sub_fft(void);
  void sub_ifft(void);
  void sub_spectrum(void);
  void sub_cepstrum(void);
};

G__FFT::G__FFT(int identity)
{ 
  iden=identity; 
  sub_init();
}

void G__FFT::sub_init(void)
{
  int i;
  xin=xout=(DVector*)NULL;
  for(i=0;i<2;i++) yin.pa[i]=yout.pa[i]=(DVector*)NULL;
  icount=ocount=0;
}


G__FFT& G__FFT::operator <<(DVector& x)
{
  switch(icount) {
  case 0:
    xin = &x;
    break;
  case 1:
    yin.pa[0] = &x;
    iden_yin=G__FFT::G__DVECTORTYPE;
    break;
  case 2:
    yin.pa[1] = &x;
    iden_yin=G__FFT::G__DVECTORTYPE;
    break;
  default:
    cerr << "Too many input is given\n";
    break;
  }
  ++icount;
  return(*this);
}


G__FFT& G__FFT::operator >>(DVector& x)
{
  switch(ocount) {
  case 0:
    xout = &x;
    break;
  case 1:
    yout.pa[0] = &x;
    iden_yout=G__FFT::G__DVECTORTYPE;
    break;
  case 2:
    yout.pa[1] = &x;
    iden_yout=G__FFT::G__DVECTORTYPE;
    break;
  default:
    cerr << "Too many output is specified\n";
    break;
  }
  ++ocount;
  return(*this);
}

G__FFT& G__FFT::operator <<(DCVector& x)
{
  yin.pc = &x;
  iden_yin=G__FFT::G__DCVECTORTYPE;
  return(*this);
}

G__FFT& G__FFT::operator >>(DCVector& x)
{
  yout.pc = &x;
  iden_yout=G__FFT::G__DCVECTORTYPE;
  return(*this);
}

G__FFT& G__FFT::operator >>(char *s)
{
  *this << '\n';
  return(*this);
}

G__FFT& G__FFT::operator >>(char c)
{
  *this << '\n';
  return(*this);
}
#if 0
G__FFT& G__FFT::operator >>(G__CINT_ENDL c)
{
  *this << '\n';
  return(*this);
}
G__FFT& G__FFT::operator <<(G__CINT_ENDL c)
{
  *this << '\n';
  return(*this);
}
#endif
G__FFT& G__FFT::operator <<(char *s)
{
  *this << '\n';
  return(*this);
}

G__FFT& G__FFT::operator <<(char c)
{
  if(NULL!=yin.pa[0] && NULL!=yout.pa[0] &&
     NULL!=xin && NULL!=xout) {
    do_fft();
  }
  else {
    cerr << "Error: FFT not enough data or buffer given\n";
  }
  sub_init();
  return(*this);
}

void G__FFT::do_fft(void)
{
  switch(iden) {
  case IDEN_FFT:
    sub_fft();
    break;
  case IDEN_IFFT:
    sub_ifft();
    break;
  case IDEN_SPECTRUM:
    sub_spectrum();
    break;
  case IDEN_CEPSTRUM:
    sub_cepstrum();
    break;
  default:
    cerr << "Unknown FFT calculation\n";
    break;
  }
}

void G__FFT::sub_fft() {
  *xout = *xin;
  if(G__FFT::G__DCVECTORTYPE==iden_yout) {
    if(G__FFT::G__DCVECTORTYPE==iden_yin) {
      *yout.pc = *yin.pc ;
    }
    else {
      if(NULL==yin.pa[1]) {
        DVector2DCVector(*yin.pa[0],*yout.pc);
      }
      else {
        DVector2DCVector(*yin.pa[0],*yin.pa[1],*yout.pc);
      }
    }
    fft(*xout,*yout.pc);
  }
  else {
    if(NULL==yout.pa[1]) {
      cerr << "Error: FFT imaginary output not specified\n";
    }
    else {
      *yout.pa[0] = *yin.pa[0];
      if(yin.pa[1]) *yout.pa[1] = *yin.pa[1];
      else          *yout.pa[1] = 0;
      fft(*xout,*yout.pa[0],*yout.pa[1]);
    }
  }
}

void G__FFT::sub_ifft(void)
{
  *xout = *xin;
  if(G__FFT::G__DCVECTORTYPE==iden_yout) {
    if(G__FFT::G__DCVECTORTYPE==iden_yin) {
      *yout.pc = *yin.pc;
    }
    else {
      if(NULL==yin.pa[1]) {
        DVector2DCVector(*yin.pa[0],*yout.pc);
      }
      else {
        DVector2DCVector(*yin.pa[0],*yin.pa[1],*yout.pc);
      }
    }
    ifft(*xout,*yout.pc);
  }
  else {
    if(NULL==yout.pa[1]) {
      cerr << "Error: IFFT imaginary output not specified\n";
    }
    else {
      *yout.pa[0] = *yin.pa[0];
      if(yin.pa[1]) *yout.pa[1] = *yin.pa[1];
      else          *yout.pa[1] = 0;
      ifft(*xout,*yout.pa[0],*yout.pa[1]);
    }
  }
}

void G__FFT::sub_spectrum(void)
{
  if(G__FFT::G__DCVECTORTYPE==iden_yin || G__FFT::G__DCVECTORTYPE==iden_yout) {
    cerr << "Error: spectrum, DCVector not expected\n";
  }
  else {
    *xout = *xin;
    *yout.pa[0] = *yin.pa[0];
    spectrum(*xout,*yout.pa[0]);
  }
}

void G__FFT::sub_cepstrum(void)
{
  if(G__FFT::G__DCVECTORTYPE==iden_yin || G__FFT::G__DCVECTORTYPE==iden_yout) {
    cerr << "Error: cepstrum, DCVector not expected\n";
  }
  else {
    *xout = *xin;
    *yout.pa[0]=*yin.pa[0];
    cepstrum(*xout,*yout.pa[0]);
  }
}


// Global FFT object
G__FFT FFT=G__FFT(IDEN_FFT);
G__FFT IFFT=G__FFT(IDEN_IFFT);
G__FFT SPECTRUM=G__FFT(IDEN_SPECTRUM);
G__FFT CEPSTRUM=G__FFT(IDEN_CEPSTRUM);


#endif
