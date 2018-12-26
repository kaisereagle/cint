
#include <CSV.h>

int main() {
  Sheet s("csv1.csv");
  Collection c=s.collection();
  for(int i=0;i<c.size();i++) {
    printf("%2d %2d:",i,c[i].size());
    c[i].disp();
  }
  printf("%d\n",c.size());
  return 0;
}

