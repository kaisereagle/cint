#include <stdio.h>
#include <unistd.h>
#include <exception>
using namespace std;

class MyException : public exception {
 public:
  char msg[100];
  MyException(const char* m) { strcpy(msg,m); }
  virtual const char* what() const /* throw() */ { return(msg); }
};

void test(char* name) {
  if(!name) throw MyException("NULL pointer");
  if(!name[0]) throw exception();
  printf("%s\n",name);
}

