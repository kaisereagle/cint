#include <map>
#include <iostream>

main() {
#if 0
  map<int,int,less<int> > a;
  map<double,int,less<double> > b;
#else
  map<char*,double> a;

  a["A"] = 1;
  a["B"] = 2;
  a["PI"] = 3.14;
  cout << "A=" << a["A"] << endl;
  cout << "B=" << a["B"] << endl;
  cout << "PI=" << a["PI"] << endl;
#endif
}
