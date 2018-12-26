#include <cstdio>
#include <vector>
#include <list>
#include <typeinfo>
using namespace std;


template<class T>
void func(T dmy) {
  vector<T> x;
  for(int i=0;i<5;i++) {
    x.push_back((T)i);
  }
  vector<T>::iterator first = x.begin();
  vector<T>::iterator last  = x.end();

  while(first!=last) {
    printf("%g ",*first++);
  }
  printf("%s\n",typeid(x).name());
}

template<class T>
void lfunc(T dmy) {
  list<T> x;
  for(int i=0;i<5;i++) {
    x.push_back((T)i);
  }
  list<T>::iterator first = x.begin();
  list<T>::iterator last  = x.end();

  while(first!=last) {
    printf("%g ",*first++);
  }
  printf("%s\n",typeid(x).name());
}

main() {
  func(double());
  func(float());
  func(int());
  //func(char());
  func(short());
  func(long());
#if 0
  func((unsigned int)0);
  func((unsigned char)0);
  func((unsigned short)0);
  func((unsigned long)0);
#endif

  lfunc(double());
  lfunc(int());
  lfunc(long());
#if 0
  // Need to compile list containers
  //lfunc(float());
  //lfunc(char());
  //lfunc(short());
#endif
}

