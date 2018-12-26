
#include <CSV.h>

int main() {
  vector<string> a,b;
  string s;
  int i;
  a.push_back("abc_apple_defh");  
  a.push_back("abc_orange_defh1");  
  for(i=0;i<500;i++) {
    a.push_back("abc_orange_defh1");  
    a.push_back("abc_orange_defh2");  
    a.push_back("abc_orange_defh2");  
    a.push_back("abc_pear_defh1");  
    a.push_back("abc_pear_defh1");  
    a.push_back("abc_pear_defh2");  
    a.push_back("abc_pear_defh1");  
    a.push_back("abc_pear_defh3");  
  }

  b.push_back("abc_pen_defh1");
  b.push_back("abc_paper_defh2");

  s= EasyExpStatExtraction(a,b,1,100);

  printf("%s\n",s.c_str());
}

