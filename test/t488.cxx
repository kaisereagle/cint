//macro2.C

#include <stdio.h>
#include <vector>
#ifndef __hpux
using namespace std;
#endif

void myfunc(vector<int> &v) {
  printf("%d\n",v[0]);
}

int main() {
  vector<int> v;
  v.push_back(3);
  myfunc(v);
  return 0;
}
