
#include <ReadF.h>
int main() {
  ReadF f;
  f.setseparator("");
  f.setdelimiter(",");
  f.setquotation("\"");
  f.parse("\" \",BODY,\"\",abc"); f.disp(); 
  f.parse("\"\" ,BODY,\"\",abc"); f.disp(); 
  f.parse("\"\",BODY,\"\",abc"); f.disp(); 
  f.parse(",BODY,\"\",abc"); f.disp(); 
  f.parse(",BODY,\"\",abc"); f.disp(); 
  f.parse("\"a\",BODY,\"\",abc"); f.disp(); 
  f.parse("\"a\",BODY,\"a\",abc"); f.disp(); 
  f.parse("\"\"a,BODY,\"\"a,abc"); f.disp(); 
}
