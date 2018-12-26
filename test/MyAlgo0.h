// MyAlgo0.h

#ifdef __hpux
#include <iostream.h>
#else
#include <iostream>
using namespace std;
#endif

// コンテナ中の全要素を表示するアルゴリズム
template<class InputIter> 
void Disp(InputIter first,InputIter last)
{
  while(first!=last) cout << *first++ << " " ;
  cout << endl;
}

// コンテナ中の全要素の総和を求めるアルゴリズム
template<class T,class InputIter> 
T Sum(InputIter first,InputIter last,const T initval)
{
  T sum = initval;
  while(first!=last) sum += *first++ ;
  return(sum);
}
