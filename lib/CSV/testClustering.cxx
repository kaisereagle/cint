#include <CSV.h>
#include "Clustering.cxx"

Sheet s("testClustering.csv");
Collection c,cword,cchar;

void copytitle(Collection& t) {
  for(int i=0;i<t.size();i++) {
    t.getkey().let(i+1,t[i][0].tostring());
    t[i].let(t.size()+1,"");
  }
}

int main() {
  G__optimizemode(3);
  c = s.collection();
  c.setkey(0);

  cword = c.duplicate();
  cchar = c.duplicate();

  int i,j;
  double wordratio,charratio;
  for(i=0;i<c.size();i++) {
    for(j=0;j<c.size();j++) {
      wordratio = wordmatchratio(c[i][0].tostring(),c[j][0].tostring());
      cword[i].let(j+1,wordratio);
      charratio = charmatchratio(c[i][0].tostring(),c[j][0].tostring());
      cchar[i].let(j+1,charratio);
    }
  }

  while(!G__pause());
  return(0);
}


