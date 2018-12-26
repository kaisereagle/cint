
#include <EasyExp.dll>

void test(EasyExp& e,const char* pattern,const char* expected) {
  printf("%d :",e.match(pattern));
  for(int i=0;i<e.argv.size();i++) printf("'%s' ",e.argv[i].c_str());
  printf(" : %s\n",expected);
}

/////////////////////////////////////////////
void test1() {
  EasyExp e;
  e.compile("abc*");
  e.disp();
  test(e,"abcdef","abc def");
  test(e,"abcZXEEAR","abc ZXEEAR");
  test(e,"abc","abc");
}

/////////////////////////////////////////////
void test2() {
  EasyExp e;
  e.compile("abc");
  e.disp();
  test(e,"abcdef","");
  test(e,"abc","abc");
}

/////////////////////////////////////////////
void test3() {
  EasyExp e;
  e.compile("abc*hij");
  e.disp();
  test(e,"abcdefhij","abc def hij");
  test(e,"abchij","abc '' hij");
  test(e,"abcxxhj","");
}
/////////////////////////////////////////////
void test4() {
  EasyExp e;
  e.compile("abc(ABC|D|EFG|HIJK)hij");
  e.disp();
  test(e,"abcEFGhij","abc EFG hij");
  test(e,"abchij","");
  test(e,"abcDhj","");
  test(e,"abcDhij","abc D hij");
}

/////////////////////////////////////////////
void test5() {
  EasyExp e;
  e.compile("abc*(ABC|D|EFG|HIJK)hij");
  e.disp();
  test(e,"abcEFGhij","abc '' EFG hij");
  test(e,"abchij","");
  test(e,"abcdefEFGhij","abc def EFG hij");
  test(e,"abcDhij","abc '' D hij");
}

/////////////////////////////////////////////
void test6() {
  EasyExp e;
  e.compile("abc[A-M]hij");
  e.disp();
  test(e,"abcGhij","abc G  hij");
  test(e,"abchij","");
  test(e,"abcMhij","abc M hij");
  test(e,"abcDhij","abc D hij");
}

/////////////////////////////////////////////
void test7() {
  EasyExp e;
  e.compile("abc*[A-M]hij");
  printf("### test7\n");
  e.disp();
  test(e,"abcGhij","abc '' G  hij");
  test(e,"abchij","");
  test(e,"abcdefMhij","abc def M hij");
  test(e,"abchijDhij","abc hij D hij");
  test(e,"abcABhij","abc A B hij");
  test(e,"abcABCChij","abc ABC C hij");
}

/////////////////////////////////////////////
void test8() {
  EasyExp e;
  e.compile("abc[^A-M]hij");
  printf("### test8\n");
  e.disp();
  test(e,"abcGhij","");
  test(e,"abchij","");
  test(e,"abcZhij","abc Z hij");
  test(e,"abc0hij","abc 0 hij");
}

/////////////////////////////////////////////
void test9() {
  EasyExp e;
  e.compile("abc*[^A-M]hij");
  printf("### test9\n");
  e.disp();
  test(e,"abcGhij","");
  test(e,"abcYhij","abc '' Y hij");
  test(e,"abcdWhij","abc d W hij");
  test(e,"abcdefWhij","abc def W hij");
}
/////////////////////////////////////////////
void test10() {
  EasyExp e;
  e.compile("abc{12-84}hij");
  printf("### test10\n");
  e.disp();
  test(e,"abc89hij","");
  test(e,"abc64hij","abc 64 hij"); 
  test(e,"abcd61hij","");
  test(e,"abc12xhij","");
}
/////////////////////////////////////////////
void test11() {
  EasyExp e;
  e.compile("abc*{12-84}hij");
  //e.compile("abc*(12..84)hij");
  printf("### test11\n");
  e.disp();
  test(e,"abc89hij","");
  test(e,"abc64hij","abc 64 hij"); 
  test(e,"abcd61hij","abc d 61 hij"); 
  test(e,"abcdef11hij","");
  test(e,"abcdef34hij","abc def 34 hij"); 
  test(e,"abcd3ef34hij","abc d3ef 34 hij"); 
  test(e,"abcd3ef343hij",""); 
}
/////////////////////////////////////////////
/////////////////////////////////////////////
void test12() {
  EasyExp e;
  e.compile("abc*");
  printf("### test12\n");
  e.disp();
  test(e,"abc89hij","abc 89hij");
  test(e,"abcd61hij","abc d61hij"); 
  test(e,"abCdef11hij","");
  test(e,"abcdef34hij","abc def34hij"); 
}
/////////////////////////////////////////////

int main() {
  test1();
  test2();
  test3();
  test4();
  test5();
  test6();
  test7();
  test8();
  test9();
  test10();
  test11();
  test12();
}
