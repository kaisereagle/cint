
#include <iostream>
#include "qtcint.dll"

ostream& operator<<(ostream& ost,const QString& x) {
  ost << x.data();
  return(ost);
}

int main() {
  QString a("abc");
  QString b("def");
  QString c("def");
  a += b;
  cout << a.data() << " " << b.data() << endl;
  cout << a << " " << b << endl;
  return 0;
}
