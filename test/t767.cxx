/* 
 ===========================================================================
  -*- mode: c -*-   Last Modified Time-stamp: <01/12/31 18:21:37 hata>

  sslib253.c  Cint Version  MATRIX OK! VECTOR NG! cint -03 �Ǽ¹ԤΤ���  
  Ȫ��ɧ <kazuhiko.hata@nifty.ne.jp>
 ===========================================================================
*/
/*
 *----------------------------------------------------------------------------
 * �� �ʳص��ѷ׻����֥롼����饤�֥�꡼
 *    p.89  2.5.3 �·���ϢΩ����������������������������ˡ�ʣǣ��գʣϣҡ�
 *				ʬ�䡢module�� OK ��ǧ�Ѥ�
 *               option base 0    �ݥ��󥿤ˤ��������Ϥ�
 *
 *   ��ǽ        ������Ʊ���Ǥ���k�Ȥμ·���ϢΩ����������
 *                       ��[m, m]����[m,k] = ��[m,k]
 *               �β��gaussjordanˡ�ǰ��٤˵��롣
 *               �ݤ��Ф��ݡ���(pivot)�Ȥ��������Ǥ������ͺ�������򤹤뤿��
 *               ��������촹����Ԥ�
 *    ��  int gaujor(double *a, int l, int m, int n, double eps)
 *
 *   ������  ������
 *           a   :a[L][N]�ʤ������̾�Ǽ·���ϢΩ�����������η������󤪤��
 *                �������Ǥ��롣�黻�����������������ʤ��a[i][j],
 *                i=0,1,2,...,M-1, j=M,M+1,M+2,...,N-1�˲������롣
 *            ����
 *           l   :�ᥤ��ץ����������������A���裱ź�����ͤ����������
 *                ���ѿ�̾��Ϳ���롣��L��M
 *           m,n :����A���⡢�黻�оݤȤʤ����������������ѿ�̾��Ϳ����
 *                â����N=M+K��K��������������Ǥ��롣N>M  80��M��2
 *           eps :��«Ƚ���ͤ����������ѿ�̾��Ϳ���롣EPS>0
 *            ����
 *           �ؿ���   :���顼�ե饰�Ǥ��롣
 *                       0   : ���顼�ʤ�
 *                       1   : �ݤ��Ф������桢�ԥܥå����Ǥ�eps��꾮����
 *                      -1   : m<2, m>80, m��n, eps��0 �Τ����줫�Ǥ���
 *   ���졼�֥��֥롼����    : �ʤ�
 *----------------------------------------------------------------------------
 */
 
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

/* #define CGI */
#undef  MATRIX         // <-------- �����Ǥ���
//#define MATRIX         // <-------- �����Ǥ���
#define MAIN

#define NMAX 50
#define L 3
#define M 3
#define N 5
#define EPS 1.0e-16

int gaujor2(double a[NMAX][NMAX], int l, int m, int n, double eps);
int gaujor(double *, int, int, int, double);

#ifndef MATRIX
int gaujor(double *a, int l, int m, int n, double eps)
{
  int i,ii,j,k,lc,lr,iw;
  double wmax,w,pivot,api;
  static int work[80];
  
  if(m<1 || m>79 || m>=n || eps<=0)
    return(-1);
  for(i=0; i<m; i++)
    work[i] = i;
  for(k=0; k<m; k++){
    wmax = 0.0;
    for(ii=k; ii<m; ii++){
      for(i=k; i<m; i++){
	w = fabs(*(a+i*n+i));
	if(w>=wmax){
	  wmax = w;
	  lc = i;
	  lr = ii;
	}
      }
    }
    pivot = *(a+lr*n+lc);
    api = fabs(pivot);
    if(api<=eps)
      return(-1);
    if(lc!=k){
      iw = work[k];
      work[k] = work[lc];
      work[lc] = iw;
      for(i=0; i<m; i++){
	w = *(a+i*n+k);
	*(a+i*n+k) = *(a+i*n+lc);
	*(a+i*n+lc) = w;
      }
    }
    if(lr!=k){
      for(j=k; j<n; j++){
	w = *(a+lr*n+j);
	*(a+lr*n+j) = *(a+k*n+j);
	*(a+k*n+j) = w;
      }
    }
    for(i=k+1;i<n; i++)
      *(a+k*n+i) = *(a+k*n+i) / pivot;
    for(i=0; i<m; i++){
      if(i!=k){
	w = *(a+i*n+k);
	for(j=k+1; j<n; j++)
	  *(a+i*n+j) = *(a+i*n+j) - w * *(a+k*n+j);
      }
    }
  }
  for(i=0; i<m; i++){
    do{
      k = work[i];
      if(k==i) break;
      iw = work[k];
      work[k] = work[i];
      work[i] = iw;
      for(j=m; j<n; j++){
	w = *(a+k*n+j);
	*(a+k*n+j) = *(a+i*n+j);
	*(a+i*n+j) = w;
      }
    } while(1);
  }
  return(0);
}
#endif

#ifdef MATRIX
int gaujor2(double a[NMAX][NMAX], int l, int m, int n, double eps)
{
  int i,ii,j,k,lc,lr,iw;
  double wmax,w,pivot,api;
  static int work[80];
  
  if(m<1 || m>79 || m>=n || eps<=0)
    return(-1);
  for(i=0; i<m; i++)
    work[i] = i;
  for(k=0; k<m; k++){
    wmax = 0.0;
    for(ii=k; ii<m; ii++){
      for(i=k; i<m; i++){
	w = fabs(a[i][i]);
	if(w>=wmax){
	  wmax = w;
	  lc = i;
	  lr = ii;
	}
      }
    }
    pivot = a[lr][lc];
    api = fabs(pivot);
    if(api<=eps)
      return(-1);
    if(lc!=k){
      iw = work[k];
      work[k] = work[lc];
      work[lc] = iw;
      for(i=0; i<m; i++){
	w = a[i][k];
	a[i][k] = a[i][lc];
	a[i][lc] = w;
      }
    }
    if(lr!=k){
      for(j=k; j<n; j++){
	w = a[lr][j];
	a[lr][j] = a[k][j];
	a[k][j] = w;
      }
    }
    for(i=k+1;i<n; i++)
      a[k][i] = a[k][i] / pivot;
    for(i=0; i<m; i++){
      if(i!=k){
	w = a[i][k];
	for(j=k+1; j<n; j++)
	  a[i][j] = a[i][j] - w * a[k][j];
      }
    }
  }
  for(i=0; i<m; i++){
    do{
      k = work[i];
      if(k==i) break;
      iw = work[k];
      work[k] = work[i];
      work[i] = iw;
      for(j=m; j<n; j++){
	w = a[k][j];
	a[k][j] = a[i][j];
	a[i][j] = w;
      }
    } while(1);
  }
  return(0);
}
#endif

#ifdef MAIN
int main(void)
{
  int i,j,g /* ,l,m,n */ ;
  
#ifdef MATRIX
  static double a[NMAX][NMAX]={{5.0, 1.0, 1.0, 10.0, 18.0},
				 {1.0,-7.0, 2.0, -7.0, -9.0},
				 {1.0, 1.0, 6.0, 21.0, 11.0}};
#endif
  
#ifndef MATRIX
  static double a[L][N]={{5.0, 1.0, 1.0, 10.0, 18.0},
			   {1.0,-7.0, 2.0, -7.0, -9.0},
			   {1.0, 1.0, 6.0, 21.0, 11.0}};
#endif

#if !defined(G__MSC_VER) && !defined(_MSC_VER)
  system("clear");
#endif

  /* printf("\x01b[2J"); */                /* clear screan */
  printf("\n");
#if 0
  printf("              �� �ʳص��ѷ׻����֥롼����饤�֥�꡼�ʣá�\n");
  printf("      2.5.3 �·���ϢΩ����������������������������ˡ�ʣǣ��գʣϣҡ�\n\n");
#endif
  printf("    given matrix:\n");
  
  for(i=0; i<L; i++){
    printf("    ");
    for(j=0; j<N; j++)
      printf("% 6.2f", a[i][j]);
    printf("\n");
  }
  
/******************************************************************************
  printf("              given equation:\n\n");
  printf("          [no. 1]                      [no. 2]\n");
  printf("               5x1 + x2 + x3 = 10           5x1 + x2 + x3 = 18\n");
  printf("                x1 -7x2 +2x3 = -7            x1 -7x2 +2x3 = -9\n");
  printf("                x1 + x2 +6x3 = 21            x1 + x2 +6x3 = 11\n\n");
  ******************************************************************************/
  
#ifdef MATRIX
  g = gaujor2(a, L, M, N, EPS);
#endif
  
#ifndef MATRIX
  //g = gaujor(*a, L, M, N, EPS);
  g = gaujor(a[0], L, M, N, EPS);
#endif
  
  printf("\n    solution by gaujor:\n");
  
  for(i=1; i<=N-M;i++)
    printf("          [no. %d]     ",i);
  printf("\n");

  for(i=0; i<L; i++){
    for(j=M; j<N; j++)
      printf("    x%d =% 10.6E",i+1,a[i][j]);
    printf("\n");
  }
  return 0;
}
#endif


