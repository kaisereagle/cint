/*************************************************************************
* K means and EM method
*
* From Bishop, PRML Vol2  p142 Kmeans,  p154 EM, chapter 10 Variational Bayes
*
* Usage:
*  DEM em;
*  em.clear();
*  DVector datavector; datavector.resize(datadim);
*  for(datavector)  em.push_back(datavector);
*  em.Initialize(K);
*  for() { em.Kmeans_Estep() ;  em.Kmeans_Mstep() ; }
*  for() { em.EM_Estep() ;      em.EM_Mstep() ; }
*  em.Initialize(K,FuzzificationConst,thresh);
*  for() { em.FuzzyCmeans_Estep() ;      em.FuzzyCmeans_Mstep() ; }
*************************************************************************/

#ifndef KMEANS_H
#define KMEANS_H

//#define NOEIGEN

#include "Vector.h"
#include "MatrixCalc.h"
#include <float.h>

////////////////////////////////////////////////////////////////////
template<class T>
inline T RandomEqualdouble(T Low, T High) 
  {return ((T) rand() / RAND_MAX) * (High-Low) + Low;}

template<class T>
inline T RandomGauss(T mean,T sigma) {
  T d = RandomEqualdouble(-1.0,1.0);
  d = pow(d,3.0);
  return(mean + d*sigma);
}


////////////////////////////////////////////////////////////////////

template<class T> 
class EM {
 public:
  typedef Vector<T>             TVector;
  typedef Vector<TVector>       TMatrix;
  typedef Vector<TMatrix>       TVectorMatrix;

  //COMMON
  //void clear() ;
  //for each data
  //  void push_back(const TVector& x) ; 

  // CHOOSE ONE OF FOLLOWING CLUSTERING METHOD

  //int Initialize(unsigned int K) ;
  //int Kmeans_Estep() ;
  //int Kmeans_Mstep() ;
  //int Kmeans_Iteration(unsigned int L=0) ;

  //int Initialize(unsigned int K) ;
  //int EM_Estep() ;
  //int EM_Mstep() ;
  //int EM_Iteration(unsigned int L=0) ;

  //int InitializeVB(TBD), Variational Bayes method is not ready yet
  //int VB_Estep() ;
  //int VB_Mstep() ;
  //int VB_Iteration(unsigned int L=0) ;

  //int Initialize(unsigned int K,double m=2.0,double change thresh=1e-2);
  //int FuzzyCmeans_Estep() ;
  //int FuzzyCmeans_Mstep() ;
  //int FuzzyCmeans_Iteration(unsigned int L=0) ;


  ///////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////
 private:
  // Kmeans & EM common input
  TMatrix             m_x;     // N     * M      , data(M) samples(N)
  unsigned int        m_M;     // data vector dimention
  unsigned int        m_N;     // sample count
  unsigned int        m_K;     // number of category
  // VB (Variational Bayes) input
  double              m_alpha0; 
  double              m_m0; 
  double              m_beta0; 
  double              m_v0; 
  TMatrix             m_iW0; 
  // Fuzzy C-means input
  double              m_m;     //    m > 1, fuzzification constant
  double              m_changethresh; // 

  // Kmeans output. (Also calculated for EM as the largest gamma category)
  Vector<int>         m_r;     // N          , ï™óﬁ , K ÇÃindex

  // Kmeans, EM, VB & Fuzzy C-means output 
  TMatrix             m_mean;  //     K * M  , CategoryïΩãœ

  // EM, VB & Fuzzy C-means output
  TMatrix             m_gamma; // N * K      , ïâíSó¶
  // EM output
  TVector             m_pi;    //     K      , ç¨çáåWêî
  TVectorMatrix       m_covar; //     K *M*M , ã§ï™éUçsóÒ
  // VB (Variational Bayes) output
  TVector             m_alphak; //     K  ,  
  TMatrix             m_mk;     //     K  ,  M
  TVector             m_betak;  //     K  ,  
  TVector             m_vk;     //     K  ,  
  Vector<TMatrix>     m_Wk;     //     K  ,  accuracy matrix, 1/covar?

  ///////////////////////////////////////////////////////////////////

 public:
  ///////////////////////////////////////////////////////////////////
  // Data input
  ///////////////////////////////////////////////////////////////////
  EM() {m_M = m_N = m_K = 0;}  
  ///////////////////////////////////////////////////////////////////
  // Clear sample data 
  void clear() {m_x.clear(); m_N = 0; }
  ///////////////////////////////////////////////////////////////////
  // Set each sample data
  void push_back(const TVector& x) { 
    if(m_x.size() && m_x[0].size()!=x.size()) {
      fprintf(stderr,"EM::push_back() : Error, Data dimention mismatch\n");
    }
    else m_M = x.size();
    m_x.push_back(x); 
    m_N = m_x.size();
  }
  
  ///////////////////////////////////////////////////////////////////
  // Initialize Kmeans, EM and Fuzzy C-means parameters
  ///////////////////////////////////////////////////////////////////
  int Initialize(unsigned int K,double m_in=2.0,double changethresh=1e-2) {
    if(m_x.size()<2) {
      fprintf(stderr,"EM::Initialize() : Error, no data\n");
      return(255);
    }

    // äeóvëfÇÃïΩãœÇãÅÇﬂÇÈ
    TVector sum (0.0,0.0,m_M);
    unsigned int n,k,m;
    TVector dat;
    for(n=0;n<m_N;n++) {
      dat = m_x[n];
      sum += dat;
    }
    TVector mean = sum/T(m_N);

    // äeóvëfÇÃï™éU(ïWèÄïŒç∑)ÇãÅÇﬂÇÈ
    sum = 0.0;
    for(n=0;n<m_N;n++) {
      dat = m_x[n]-mean;
      sum += (dat*dat);
    }
    TVector stddev = sqrt(sum/double(m_N));

    // Set random data as initial value for m_mean
    TVector ran(0.0,0.0,m_M);
    m_K = K;
    m_mean.clear();
    for(k=0;k<m_K;k++) {
      for(m=0;m<m_M;m++) {
        ran[m] = RandomEqualdouble(mean[m]-stddev[m]*2,mean[m]+stddev[m]*2);
      }
      m_mean.push_back(ran);
    }

    // set gamma table size 
    TVector gamma(0.0,0.0,m_K);
    m_gamma.resize(m_N);
    m_gamma = gamma;

    m_r.resize(m_N);

    // set m_pi table size
    m_pi.resize(m_K);
    m_pi = 1.0;

    // set covariance matrix size
    TVector Vcov(0.0,0.0,m_M);
    TMatrix Mcov; Mcov.resize(m_M); Mcov = Vcov;
    for(m=0;m<m_M;m++) Mcov[m][m] = stddev[m];
    m_covar.resize(m_K); m_covar = Mcov;

    // Initialize Fuzzy C-means parameters
    m_m = m_in;
    m_changethresh = changethresh;
    
    return(0);
  }


  ///////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////
  //
  ///////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////
#if 0
  // K-mean, EM ï™óﬁâ¸ëPàƒ
  // 1 Mode (ç≈ïpíl) íäèoÇ…ÇÊÇÈ K Ç∆ mean[k] èâä˙íléZèo
  //   1-1 1éüå≥ histogram Ç…ÇÊÇÈéüå≥ñàÇÃã…ëÂì_íäèo
  //   1-2 éüå≥ñàÇÃã…ëÂì_ãﬂñTÇ…å¿íËÇµÇΩ M éüå≥ histogramÇ…ÇÊÇÈ Méüå≥modeíäèo
  //    Ç±ÇÍÇ™åæÇ§ÇÕà’ÇµçsÇ§ÇÕìÔÇµ
  // 2 Modeå≈íË K-means E step
  //   2-1 M step ÇÃë„ÇÌÇËÇ∆Çµ mean[k]=mode[k] ê›íË
  //   2-2 E step : ÉÜÅ[ÉNÉäÉbÉhãóó£Çå≥Ç…ï™óﬁÇàÍìxÇæÇØçsÇ§
  // 3 Modeå≈íË EM
  //   3-1 E step ÇÃë„ÇÌÇËÇ∆ÇµÇƒïâíSó¶Ç 2-2 ÇÃï™óﬁÇ…è]Ç¡ÇƒdatañàÇ… 1,0 äÑêUÇË
  //   3-2 M step : mean ÇåvéZÇπÇ∏ covar (sigma) Ç∆ ïâíSó¶,ç¨çáåWêîÇåvéZÇ∑ÇÈ
  //   3-3 E step : mean, covar, ç¨çáåWêîÇópÇ¢ÇƒäedataÇÃïâíSó¶ÇåvéZÇ∑ÇÈ
  //   3-4 M step : mean ÇåvéZÇπÇ∏ covar (sigma) Ç∆ ïâíSó¶,ç¨çáåWêîÇåvéZÇ∑ÇÈ
  //    Ç±Ç±Ç‹Ç≈Ç≈ mean=modeå≈íË EMñ@äÆóπ
  //    à»å„ÇÕ meanå≈íËÇäOÇµÇƒ EMñ@

  void Histogram(int nbin=10) {
    // äeóvëfÇÃ min, maxÇãÅÇﬂÇÈ
    typedef Vector<IMatrix> ICube;
    TVector dat;
    TVector Vmin = FLT_MAX,Vmax = -FLT_MAX;
    unsigned int n;
    for(n=0;n<m_N;n++) {
      dat = m_x[n];
      Vmin = min(Vmin,dat);
      Vmax = max(Vmax,dat);
    }
    TVector step = (Vmax-Vmin)/nbin;
    TVector base = Vmin-step/2;
    TVector indx; indx.resize(m_M);
    unsigned int i,j;
    switch(m_M) {
    case 1:
      IVector hist1; hist1.resize(nbin);
      for(i=0;i<m_x.size();i++) {
        indx = (m_x[i]-base)/step;
        ++hist1[(int)indx[0]];
      }
      break;
    case 2:
      IMatrix hist2; hist2.resize(nbin); for(i=0;i<nbin;i++) hist2[i].resize(nbin);
      for(i=0;i<m_x.size();i++) {
        indx = (m_x[i]-base)/step;
        ++hist2[(int)indx[0]][(int)indx[1]];
      }
      for(i=0;i<nbin;i++) {
      }
      break;
    case 3:
      //ICube hist3;
      break;
    default:
      break;
    }
  }
#endif

  ///////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////
  // K means
  // Bishop, PRML  p142
  ///////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////
  int Kmeans_Estep() {
    int change=0;
    T dist2,dist2min;
    unsigned int n,k,kmin=0;
    for(n=0;n<m_N;n++) {
      dist2min = 1e99;
      for(k=0;k<m_K;k++) {
        if(!m_pi[k]) continue;
        dist2 = distance2(m_x[n],m_mean[k]);
        if(dist2<dist2min) {
          dist2min = dist2;
          kmin = k;
        }
      }
      for(k=0;k<m_K;k++) {
        if(k==kmin) { 
          if(m_gamma[n][k]!=1.0) ++change;
          m_gamma[n][k] =  1.0;
          m_r[n] = k;
        }
        else        m_gamma[n][k] =  0.0;
      }
    }
    return(change);
  }
  ///////////////////////////////////////////////////////////////////
  int Kmeans_Mstep() {
    unsigned int n,k;
    m_pi = 0.0;
    m_mean = m_pi;
    for(n=0;n<m_N;n++) {
      m_mean[m_r[n]] += m_x[n];
      ++m_pi[m_r[n]];
    }
    for(k=0;k<m_K;k++) {
      if(m_pi[k]) m_mean[k] /= m_pi[k]; 
      else        m_mean[k] = 1e99;
      m_pi[k] /= m_N;
    }
    return(1);
  }
  ///////////////////////////////////////////////////////////////////
  int Kmeans_Iteration(unsigned int L=0) {
    unsigned int count=0;
    while(Kmeans_Estep() && (count++>L||L==0)) {
      Kmeans_Mstep();
    } 
    return(count);
  }

  ///////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////
  // EM (Expectation Maximization)
  // Bishop, PRML  p154
  ///////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////
  int EM_Estep() {
    unsigned int n,k;

    // Prepare  covariance determinant and accuracy matrix (inverse covariance)
    const double pi = 3.141592;
    TVectorMatrix Icovar; Icovar.resize(m_K); // precision_matrix A = 1/covar_matrix
    TVector determinant; determinant.resize(m_K);
    TVector A; A.resize(m_K); // pi[k]/sqrt((2pi)^M * determinant[k])
    for(k=0;k<m_K;k++) {
      Icovar[k] = Inverse(m_covar[k]);
      determinant[k] = Determinant(m_covar[k]);
      A[k] = m_pi[k]/sqrt(pow(2*pi,double(m_M))*determinant[k]);
    }

    // Calculate gamma , p154 (9.23)
    T sum,val;
    for(n=0;n<m_N;n++) {
      sum = 0.0;
      for(k=0;k<m_K;k++) {
        val = A[k] *exp(GaussExp(m_x[n]-m_mean[k],Icovar[k]));
        m_gamma[n][k] = val;
        sum += val;
      }
      for(k=0;k<m_K;k++) m_gamma[n][k] /= sum; // normalization
      m_r[n] = indexmax(m_gamma[n]); // quantization, optional for EM
    }
    return(1);
  }
  ///////////////////////////////////////////////////////////////////
  int EM_Mstep() {
    const TVector dat0(0.0,0.0,m_M);
    unsigned int n,k;
    TVector Nk(0.0,0.0,m_K);
    m_mean = dat0; // matrix
    for(k=0;k<m_K;k++) {
      for(n=0;n<m_N;n++) {
        Nk[k] += m_gamma[n][k];
        m_mean[k] += m_gamma[n][k]*m_x[n]; // vector 
      }
    }
    TVector xu(0.0,0.0,m_M);
    TMatrix covar; covar.resize(m_M); covar = dat0; // set M*M size
    unsigned int m1,m2;
    for(k=0;k<m_K;k++) {
      m_pi[k] = Nk[k]/m_N;
      m_mean[k] /= Nk[k]; // vector of M / double
      covar = dat0;
      for(n=0;n<m_N;n++) {
        T gammank = m_gamma[n][k];
        xu = m_x[n] - m_mean[k]; // vector of M
        for(m1=0;m1<m_M;m1++) {
          for(m2=0;m2<m_M;m2++) {
            covar[m1][m2] += (xu[m1]*xu[m2])*gammank;
          }
        }
      }
      for(m1=0;m1<m_M;m1++) {
        //covar[m1] /= Nk[k];
        for(m2=0;m2<m_M;m2++) covar[m1][m2] /= Nk[k];
      }
      m_covar[k] = covar;
    }
    return(1);
  }
  ///////////////////////////////////////////////////////////////////
  int EM_Iteration(unsigned int L=0) {
    unsigned int count=0;
    Kmeans_Estep();
    EM_Mstep();
    while(EM_Estep() && (count++>L||L==0)) {
      EM_Mstep();
    } 
    return(count);
  }

  ///////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////
  // VB (Variational Bayes)
  // Bishop, PRML  charpeter 10
  //  Work on progress, Not completed yet
  ///////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////
  int InitializeVB(unsigned int K,double alpha0,double beta0,double v0) {
    Initialize(K);

    m_m0 = 0; // from symmetry condtion
    m_alpha0 = alpha0;
    m_beta0 = beta0;
    m_v0 = v0;

    // W0 = iW0 = identity matrix (unit matrix)
    unsigned int k;
    m_iW0.resize(K);
    for(k=0;k<m_K;k++) {
      m_iW0[k].resize(K);
      m_iW0[k][k] = 1;
    }
    return(0);
  }
  ///////////////////////////////////////////////////////////////////
  int VB_Estep() {
    unsigned int n,k, D=m_M; // dimension of input vector

    // Prepare  covariance determinant and accuracy matrix (inverse covariance)
    const double pi = 3.141592;
    TVectorMatrix Icovar; Icovar.resize(m_K); // precision_matrix A = 1/covar_matrix
    TVector determinant; determinant.resize(m_K);
    TVector A; A.resize(m_K); // pi[k]/sqrt((2pi)^M * determinant[k])
    for(k=0;k<m_K;k++) {
      Icovar[k] = Inverse(m_covar[k]);
      determinant[k] = Determinant(m_covar[k]);
      A[k] = m_pi[k]/sqrt(pow(2*pi,double(m_M))*determinant[k]);
    }

    // Calculate gamma , p154 (9.23)
    T sum,val;
    for(n=0;n<m_N;n++) {
      sum = 0.0;
      for(k=0;k<m_K;k++) {
        //val = A[k] *exp(                        GaussExp(m_x[n]-m_mean[k],Icovar[k])); // EM
        val   = A[k] *exp(-D/2/m_betak[k]+m_vk[k]*GaussExp(m_x[n]-m_mk[k]  ,m_Wk[k]));    // VB
        m_gamma[n][k] = val;
        sum += val;
      }
      for(k=0;k<m_K;k++) m_gamma[n][k] /= sum; // normalization
      m_r[n] = indexmax(m_gamma[n]); // quantization, optional for VB
    }
    return(1);
  }
  ///////////////////////////////////////////////////////////////////
  int VB_Mstep() {
    const TVector dat0(0.0,0.0,m_M);
    unsigned int n,k;
    TVector xu(0.0,0.0,m_M);
    TVector Nk(0.0,0.0,m_K);
    TVector Sk(0.0,0.0,m_K);
    m_mean = dat0; // matrix
    for(k=0;k<m_K;k++) {
      for(n=0;n<m_N;n++) {
        Nk[k] += m_gamma[n][k];
        m_mean[k] += m_gamma[n][k]*m_x[n]; // vector 
      }
      m_pi[k] = Nk[k]/m_N;
      m_mean[k] /= Nk[k]; // vector of M / double
    }
    for(k=0;k<m_K;k++) {
      for(n=0;n<m_N;n++) {
	xu = m_x[n]-m_mean[k];
	Sk[k] += m_gamma[n][k]*innerproduct(xu,xu);
      }
      Sk[k] /= Nk[k];
    }
    TMatrix covar; covar.resize(m_M); covar = dat0; // set M*M size
    unsigned int m1,m2;
    for(k=0;k<m_K;k++) {
      m_alphak[k] = m_alpha0 + Nk[k];
      m_betak[k]  = m_beta0  + Nk[k];
      m_vk[k]     = m_v0     + Nk[k];
      m_mk[k]     = (m_beta0*m_m0+Nk[k]*m_mean[k])/(m_beta0+Nk[k]); // vector of M
      for(m1=0;m1<m_M;m1++) {
	m_Wk[k][m1] = m_iW0[m1]
	  + Nk[k]*Sk[k]
	  +m_beta0*Nk[k]/(m_beta0*Nk[k])*innerproduct(m_mean[k]-m_mk[k],m_mean[k]-m_mk[k]);
      }

      covar = dat0;
      for(n=0;n<m_N;n++) {
        T gammank = m_gamma[n][k];
        xu = m_x[n] - m_mean[k]; // vector of M
        for(m1=0;m1<m_M;m1++) {
          for(m2=0;m2<m_M;m2++) {
            covar[m1][m2] += (xu[m1]*xu[m2])*gammank;
          }
        }
      }
      for(m1=0;m1<m_M;m1++) {
        //covar[m1] /= Nk[k];
        for(m2=0;m2<m_M;m2++) covar[m1][m2] /= Nk[k];
      }
      m_covar[k] = covar;
    }
    return(1);
  }
  ///////////////////////////////////////////////////////////////////
  int VB_Iteration(unsigned int L=0) {
    unsigned int count=0;
    Kmeans_Estep();
    EM_Mstep();
    while(EM_Estep() && (count++>L||L==0)) {
      EM_Mstep();
    } 
    return(count);
  }

  ///////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////
  // Fuzzy C-Means
  //  Objective Function : 
  //    J = sigma[i=1,N] sigma[k=1,K] (gik)^m |xi-ck|^2
  ///////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////
  int FuzzyCmeans_Estep() {
    int change=0;
    T dist2,dist2min,distj;
    unsigned int n,k,kmin=0,j;
    int count1=0,count2=0;
    for(n=0;n<m_N;n++) {
      dist2min = 1e99;
      for(k=0;k<m_K;k++) {
	if(m_x[n] == m_mean[k]) {
          dist2min = 0;
          kmin = k;
        }
      }
      if(dist2min==0) {
	++count1;
	for(k=0;k<m_K;k++) {
	  if(k==kmin) { 
	    if(m_gamma[n][k]!=1.0) ++change;
	    m_gamma[n][k] =  1.0;
	  }
	  else        m_gamma[n][k] =  0.0;
	}
      }
      else {
	++count2;
	T sgm=0, pwr2m_1 = 2/(m_m-1), prev = m_gamma[n][k];
	for(k=0;k<m_K;k++) {
	  dist2 = distance2(m_x[n],m_mean[k]);
	  sgm = 0 ;
	  for(j=0;j<m_K;j++) {
	    distj= distance2(m_x[n],m_mean[j]);
	    sgm += pow(dist2/distj,pwr2m_1);
	  }
	  m_gamma[n][k] = 1/sgm;
	  if(prev-m_gamma[n][k]>m_changethresh) ++change;
	}
      }
      m_r[n] = indexmax(m_gamma[n]);
    }
    //printf("c1=%d c2=%d\n",count1,count2);
    return(change);
  }
  ///////////////////////////////////////////////////////////////////
  int FuzzyCmeans_Mstep() {
    if(m_mean.size()<m_K) m_mean.resize(m_K);
    unsigned int n,k;
    TVector sigma_gik_xi; sigma_gik_xi.resize(m_M);
    T sigma_gik,tmp;
    for(k=0;k<m_K;k++) {
      sigma_gik_xi=0.0; sigma_gik=0.0;
      for(n=0;n<m_N;n++) {
	tmp = pow(m_gamma[n][k],m_m);
	sigma_gik_xi += m_x[n] * tmp; // vector
	sigma_gik    += tmp;
      }
      m_mean[k] = sigma_gik_xi/sigma_gik; // vector
    }
    return(1);
  }
  ///////////////////////////////////////////////////////////////////
  int FuzzyCmeans_Iteration(unsigned int L=0) {
    unsigned int count=0;
    while(FuzzyCmeans_Estep() && (count++>L||L==0)) {
      FuzzyCmeans_Mstep();
    } 
    return(count);
  }
  ///////////////////////////////////////////////////////////////////
  // 
  ///////////////////////////////////////////////////////////////////
 public:
  unsigned int getM() const { return(m_M); }
  unsigned int getN() const { return(m_N); }
  unsigned int getK() const { return(m_K); }
  const TVector& getX(unsigned int n) const { return(m_x[n]); }
  int getR(unsigned int n) const { return(m_r[n]); } // Kmeans
  T   getGamma(unsigned int n,unsigned int k) const { return(m_gamma[n][k]); } // EM
  T   getPi(unsigned int k) const { return(m_pi[k]); } // EM
  const TVector& getMean(unsigned int k) const { return(m_mean[k]); }
  const TMatrix& getCovar(unsigned int k) const { return(m_covar[k]); } // EM

  const TMatrix& getX() const { return(m_x); }
  const TMatrix&  getGamma() const { return(m_gamma); } // EM
  const TVector& getPi() const { return(m_pi); } // EM
  const Vector<int>& getR() const { return(m_r); } // Kmeans
  const TMatrix& getMean() const { return(m_mean); }
  const TVectorMatrix& getCovar() const { return(m_covar); } // EM

  void setMean(const TVector& mean,unsigned int k) { m_mean[k]=mean; }
  ///////////////////////////////////////////////////////////////////

 public:
  ///////////////////////////////////////////////////////////////////
  // Set/Get sample data from vector
  ///////////////////////////////////////////////////////////////////
  void setDimentionData(const TVector& x,int dim) {
    int sz;
    if(m_x.size()) sz = dim>(int)m_x[0].size()?dim:m_x[0].size();
    else sz = dim;
    TVector tmp;  tmp.resize(sz);

    unsigned int n = x.size()>m_x.size()?x.size():m_x.size();
    for(unsigned int i=0;i<n;i++) {
      if(m_x.size()<=i) m_x.push_back(tmp);
      while((int)m_x[i].size()<=dim) m_x[i].push_back(0);
      m_x[i][dim] = x[i];
    }
    m_M = m_x[0].size();
    m_N = m_x.size();
  }
  ///////////////////////////////////////////////////////////////////
  void getDimentionData(TVector& x,int dim) {
    x.clear();
    unsigned int n = m_x.size();
    for(unsigned int i=0;i<n;i++) {
      if((int)m_x[i].size()<dim) x.push_back(0);
      else                  x.push_back(m_x[i][dim]);
    }
  }
  ///////////////////////////////////////////////////////////////////
  void getDimentionDataK(TVector& x,int dim,int K) {
    x.clear();
    unsigned int n = m_x.size();
    for(unsigned int i=0;i<n;i++) {
      if(getR(i)==K) x.push_back(m_x[i][dim]);
    }
  }
  ///////////////////////////////////////////////////////////////////
  void getDimentionMean(TVector& m,int dim) {
    m.clear(); 
    for(unsigned int i=0;i<m_K;i++) {
      m.push_back(m_mean[i][dim]);
    }
  }

  ///////////////////////////////////////////////////////////////////
  // Set/Get sample data from 2D vectors
  ///////////////////////////////////////////////////////////////////
  void add2DData(const TVector& x0,const TVector& x1) {
    TVector x; x.resize(2);
    for(unsigned int i=0;i<x0.size()&&i<x1.size();i++) {
      x[0] = x0[i];
      x[1] = x1[i];
      push_back(x);
    }
  }
  ///////////////////////////////////////////////////////////////////
  void get2DData(TVector& x0,TVector& x1) {
    getDimentionData(x0,0);
    getDimentionData(x1,1);
  }
  ///////////////////////////////////////////////////////////////////
  void get2DDataK(TVector& x0,TVector& x1,int K) {
    getDimentionDataK(x0,0,K);
    getDimentionDataK(x1,1,K);
  }
  ///////////////////////////////////////////////////////////////////
  void get2DMean(TVector& x0,TVector& x1) {
    getDimentionMean(x0,0);
    getDimentionMean(x1,1);
  }
  ///////////////////////////////////////////////////////////////////
  // Set/Get sample data from 3D vectors
  ///////////////////////////////////////////////////////////////////
  void add3DData(const TVector& x0,const TVector& x1,const TVector& x2) {
    TVector x; x.resize(3);
    for(unsigned int i=0;i<x0.size()&&i<x1.size();i++) {
      x[0] = x0[i];
      x[1] = x1[i];
      x[2] = x2[i];
      push_back(x);
    }
  }
  ///////////////////////////////////////////////////////////////////
  void get3DData(TVector& x0,TVector& x1,TVector& x2) {
    getDimentionData(x0,0);
    getDimentionData(x1,1);
    getDimentionData(x1,2);
  }
  ///////////////////////////////////////////////////////////////////
  void get3DDataK(TVector& x0,TVector& x1,TVector& x2,int K) {
    getDimentionDataK(x0,0,K);
    getDimentionDataK(x1,1,K);
    getDimentionDataK(x1,2,K);
  }
  ///////////////////////////////////////////////////////////////////
  void get3DMean(TVector& x0,TVector& x1,TVector& x2) {
    getDimentionMean(x0,0);
    getDimentionMean(x1,1);
    getDimentionMean(x2,2);
  }
};

/////////////////////////////////////////////////////////////////////////

#endif


