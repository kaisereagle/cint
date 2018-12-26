// aryinit1.cxx
#include <iostream>
using namespace std;

int main() 
{
  int a[] = { 0, 2, 4, 6, 8, 10, 12, 14, 16, 18 };
  const int ASIZE = sizeof(a)/sizeof(int); 

  int i;
  for(i=0;i<ASIZE;++i) { 
    cout << a[i] << " ";
  }
  cout << endl;

  return(0);
}

