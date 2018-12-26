#include <iostream.h>
#include <string>

main() {
  string a="abc";
  string b("def");
  string c;
  c="ghi";
  cout << a << " " << a.c_str() << endl;
  cout << b << " " << b.c_str() << endl;
  cout << c << " " << c.c_str() << endl;
}


