
#include <ReadF.dll>

main(){
  ReadF f("csv1.csv");
  f.setdelimiter(",");
  f.setseparator("");
  f.setquotation("'\"");
  while(f.read()) {
    f.disp();
  }

}
