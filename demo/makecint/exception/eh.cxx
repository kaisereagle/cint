#pragma include_noerr "eh.dll" #on_err "eh.h"

test2(char* name) {
  try {
    test(name);
  }
  catch(MyException& x) {
    printf("%s\n",x.what()); 
  }
  catch(exception& z) {
    printf("exception %s\n",z.what()); 
  }
  catch(...) {
    printf("unknown exception\n"); 
  }
}

main() {
  MyException *p = new MyException("abc");
  delete p;
  for(int i=0;i<2;i++) {
    test2("abcd");	
    test2(0);
    test2("");
    test2("hijk");
  }
}
