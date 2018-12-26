
#include <EasyExp.dll>

int main() {
  EasyExp e;
  e.compile_replace("*_20(02..20)*.(jpg|txt|dat)","$2=(00..18),$3=XXX,$5=(JPG|TXT|DAT)");
  string a;

  e.replace("ptreport_20140429.txt",a);
  printf("a=%s\n",a.c_str());

  e.replace("PTreport_20140429.jpg",a);
  printf("a=%s\n",a.c_str());

  e.replace("PTreport_20240429.jpg",a);
  printf("a=%s\n",a.c_str());

  //e.disp();
  
}
