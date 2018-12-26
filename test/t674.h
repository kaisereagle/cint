
#include <stdio.h>
#include <string.h>

class string {
  char buf[100];
 public:
  string() { printf("string()\n"); buf[0] = 0; }
  string(const char* x) { printf("string(%s)\n",x); strcpy(buf,x); }
  string(const string& x) {
    printf("string((string)%s)\n",x.c_str()); 
    strcpy(buf,x.c_str()); 
  }
  ~string() { printf("~string(%s)\n",buf); }
  const char* c_str() const { return(buf);}
};
