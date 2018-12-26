/*****************************************************************************
 * _unique_count.cxx
 *  Description: read CSV file and extract unique words in specific column
 *  Status: Experimental
 *  Date: 2014/Jan/3
 *  Author: Masaharu Goto
 *****************************************************************************/
/************************************************************************
 * Copyright(c) 1995~2015  Masaharu Goto (cint@pcroot.cern.ch)
 *
 * Permission to use, copy, modify and distribute this software and its 
 * documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation. The author makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 ************************************************************************/

#ifdef __CINT__
#include <CSV.h>
#else
#include "Collection.h"
#endif
#include <string>

////////////////////////////////////////////////////////
map<string,int> tbl;
Collection t;
int main(int argc,char** argv) {
  G__optimizemode(3);
  if(argc<3) {
    fprintf(stderr,"Usage: cint _unique_count.cxx <-k [key]> <-o [outfile]> [fnameexpr] [x1] <[x2] <[x3]>>\n");
    exit(255); 
  }

  int i;
  int key=-1;
  int item=0;
  string fnameexpr,ofname;
  string x1,x2,x3;
  
  for(i=1;i<argc;i++) {
    if(strcmp(argv[i],"-k")==0)      key=atoi(argv[++i]);
    else if(strcmp(argv[i],"-o")==0) ofname=argv[++i];
    else if(item==0) { fnameexpr=argv[i]; ++item; }
    else if(item==1) { x1=argv[i]; ++item; }
    else if(item==2) { x2=argv[i]; ++item; }
    else if(item==3) { x3=argv[i]; ++item; }
  }

  Scandir d(fnameexpr.c_str());
  char* fname;
  while((fname=d.Next())&&fname[0]) {
    unique_count(fname,key,x1,x2,x3);
    printf("%s %d\n",fname,t.size());
  }

  t.sort(0);
  t.reverse();
  if(ofname=="") t.disp();
  else t.disp(ofname.c_str(),"w");
  return(0);
}
////////////////////////////////////////////////////////
void unique_count(Collection& c
                 ,int x1,int x2,int x3) {
  Collection u = c.unique_count(x1,x2,x3);
  Collection m;
  if(x3>=0) {
    for(int i=0;i<u.size();i++) {
      m = t.match(0,u[i][0].c_str())
	   .match(1,u[i][1].c_str())
           .match(2,u[i][2s].c_str());
      if(m.size()==0) t.push_back(u[i]);
      else t.add(3,u[i][3].todouble());
    }
  }
  else if(x2>=0) {
    for(int i=0;i<u.size();i++) {
      m = t.match(0,u[i][0].c_str())
	   .match(1,u[i][1].c_str());
      if(m.size()==0) t.push_back(u[i]);
      else t.add(2,u[i][2].todouble());
    }
  }
  else {
    for(int i=0;i<u.size();i++) {
      m = t.match(0,u[i][0].c_str());
      if(m.size()==0) t.push_back(u[i]);
      else t.add(1,u[i][1].todouble());
    }
  }
  //t.multisort(x1,x2,x3);
}


////////////////////////////////////////////////////////
void unique_count(const string& fname,int key
      ,const string& sx1 ,const string& sx2="" ,const string& sx3="") {
  
#ifdef STR
  StrCollection c(fname.c_str(),key);
#else
  Sheet s(fname);
  Collection c=s.collection();
  if(key>=0) c.setkey(key);
#endif
  if(t.size()==0 && key>=0) {
    t.push_back(c.getkey());
    t.setkey(0);
  }
  int x1 = -1 ,x2 = -1 ,x3 = -1;
  if(sx3!="") {
    if(isdigit(sx3[0])) x3 = atoi(sx3.c_str());
    else                x3 = c.getx(sx3.c_str());
  }
  if(sx2!="") {
    if(isdigit(sx2[0])) x2 = atoi(sx2.c_str());
    else                x2 = c.getx(sx2.c_str());
  }
  if(isdigit(sx1[0])) x1 = atoi(sx1.c_str());
  else                x1 = c.getx(sx1.c_str());

  unique_count(c,x1,x2,x3);
}

