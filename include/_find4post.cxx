
#include <CSV.h>

/////////////////////////////////////////////////////////////////////////
int main() {
  Sheet s("cu1.csv");
  Collection c=s.collection();
  c.setkey(0);
  int xexpr=c.getx("expr");
  EasyExp e;
  string inc,sup;
  char buf[100];

  unsigned int i,j;
  for(i=0;i<c.size();i++) {
    inc="";
    for(j=0;j<c.size();j++) {
      if(i==j) continue;
      e.compile(c[i][xexpr].c_str());
      if(e.match(c[j][xexpr].c_str())) {

	 sprintf(buf,"%d-",j);
	 inc += buf;

	 sup = c[j][8].tostring();
	 sprintf(buf,"%d+",i);
	 sup += buf;
	 c[j].let(8,sup);
      }
    }
    c[i].let(6,inc);
    c[i].let(4,i);
  }

  c.disp();
  while(!G__pause());

}
