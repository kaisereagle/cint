
#ifndef NNCLUSTER
#define NNCLUSTER

#include <CSV.h>

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
typedef double (*ClusterCompare)(const char*,const char*);

////////////////////////////////////////////////////////////////////////
Collection makeMatrix(const Collection& ci) {
  Collection cx = ci.column(0)._empty(0);
  int i,j,sz = cx.size();
  Line line;
  for(i=0;i<cx.size();i++) {
    cx[i].let(sz,"");
    line.let(i+1,cx[i][0]);
  }
  cx.push_back(line);
  cx.setkey(cx.size()-1);
  return(cx);
}

////////////////////////////////////////////////////////////////////////
Collection EvalColumn(Collection& cx,int j,ClusterCompare f) {
  double r;
  int i;
  Line key = cx.getkey();
  for(i=0;i<cx.size();i++) {
    if(cx[i][j+1].tostring()=="") {
      r = f(cx[i][0].c_str(),key[j+1].c_str());
      cx[i].let(j+1,r);
      cx[j].let(i+1,r);
    }
  }
  Collection t = cx;
  t.sort(j+1);
  return(t);
}


////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
void Clustering_core(Collection& cx,ClusterCompare f,int clsin=0) {
  Collection t1,t2;
  int i,j1,j2;
  int cls = clsin;
  int xcls = cx.getx("class");

  j1 = cx.getxexact(cx[0][0].c_str())-1;
  t1 = cx;
  t1 = EvalColumn(t1,j1,f);
  //cx.match(j1+1,1).let(xcls,cls++);
  j2=cx.getxexact(t1.back()[0].c_str())-1;
  t2 = EvalColumn(t1,j2,f);
  //cx.match(j2+1,1).let(xcls,cls++);
 
  DVector d1 = ToDVector(t1,j1+1);
  DVector d2 = ToDVector(t2,j2+1);
  DVector dd1 = diff(d1);
  DVector dd2 = diff(d2);
  DVector x = d1; x.i();
  statistics stat1 = stat(dd1);
  statistics stat2 = stat(dd2);
  IVector idx1=indexrange(dd1,stat1.min(),stat1.mean()-stat1.stddev()*2);
  IVector idx2=indexrange(dd2,stat2.min(),stat2.mean()-stat2.stddev()*2);

  if(idx1.size()) {
    t1.line(0,idx1[0]-1).let(xcls,cls++);
  }
  if(idx2.size()) {
    t2.line(0,idx2[0]-1).let(xcls,cls++);
  }
  if(idx1.size() || idx2.size()) {
d1.disp(); dd1.disp(); idx1.disp();
d2.disp(); dd2.disp(); idx2.disp();
//x.column("class")._empty(0).disp();
plot << d1 << d2 << endl;
    t1 = cx.empty(xcls);
    Clustering_core(t1,f,cls); 
  }
  else {
    cx.let(xcls,9999);
  }

  return;
}

////////////////////////////////////////////////////////////////////////
Collection Clustering(const Collection& ci,ClusterCompare f,int numcls=0) {
  Collection cx = makeMatrix(ci);
  cx.getkey().push_back("class");
  Clustering_core(cx,f,0);
  vector<int> col;
  col.push_back(0);
  col.push_back(cx.getx("class"));
  cx = cx.column(col);
  return(cx);
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
Collection SparseSampling(const Collection& ci,ClusterCompare f,int iter=1) {
  Collection cx = makeMatrix(ci);
  Line line;
  Collection t1,t2,cr;
  int i,j=0,j1;
  cr.push_back(cx[j]);
  t2 = cx;

  for(i=0;i<iter&&j>=0;i++) {
    t1 = t2;
    t1.erase(0);
    t2 = EvalColumn(t1,j,f);
    t2 = t2._match(j+1,1);
    if(t2.size()) { 
      t2.reverse();
      cr.push_back(t2[0]);
      j=cx.getxexact(t2[0][0].c_str())-1;
    }
    else j= -1;
  }
  return(cr);
}

////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
#endif // NNCLUSTER
