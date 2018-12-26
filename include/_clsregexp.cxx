/*****************************************************************************
 * _clsregexp.cxx
 *  Description: Sort csv file by column contents
 *    cint _clsregexp.cxx -c [clskey] -d [datkey] <-o [osuffix]> [targetfileExpr]
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
#include <CSV.h>

//////////////////////////////////////////////////////////
int clsregexp(const string& fname
	      ,const string& cls
	      ,const string& dat
	      ,const string& osuffix) {
  printf("regexp synthesis %s : %s %s\n",fname.c_str()
	 ,cls.c_str() ,dat.c_str());
  Sheet s(fname.c_str());
  Collection c=s.collection();
  c.setkey(0);
  int xcls = c.getx(cls);
  int xdat = c.getx(dat);
  int xregexp = c.getkey();
  Collection cu=c.unique_count(xcls),t,co;
  Line l;
  l.let(0,"cls");
  l.let(1,"regexp");
  co.push_back(l); co.setkey(0);
  vector<string> vs,_vs;
  string re;
  for(int i=0;i<cu.size();i++) {
    t = c.match(xcls,cu[i][0].c_str());
    vs = t.VectorString(xdat);
    t = c._match(xcls,cu[i][0].c_str());
    _vs = t.VectorString(xdat);
    re = EasyExpExtraction(vs,_vs,0,20);
    l.let(0,cu[i][0].c_str());
    l.let(1,re);
    //l.disp();
    co.push_back(l);
  }

  if(osuffix.size()) {
    string ofname, rep="$1="; rep+=osuffix;
    EasyExp er("*.csv",rep.c_str());
    er.replace(fname.c_str(),ofname);
    co.disp(ofname.c_str(),"w");
  }
  else {
    co.disp();
  }
}

//////////////////////////////////////////////////////////
int main(int argc,char** argv) {
#ifdef N
  EasyExp_num_threads = N;
  CSVClustering_num_threads = N;
#endif
#ifdef DEBUG
  CLDEBUG=1;
#endif
  string cls;
  string dat;
  string osuffix;
  Scandir d;
  for(int i=1;i<argc;i++) {
    if(argv[i][0]=='-') {
      if(strcmp(argv[i],"-c")==0) cls=argv[++i];
      else if(strcmp(argv[i],"-d")==0) dat=argv[++i];
      else if(strcmp(argv[i],"-o")==0) osuffix=argv[++i];
      else {
        fprintf(stderr,"Unknown option %s\n",argv[i]);
        fprintf(stderr,"Usage:\n");
        fprintf(stderr," _clsregexp.cxx -c [clskey] -d [datkey]> <-o [osuffix]> fileexpr\n");
	exit(255);
      }
    }
    else {
      d.scan(argv[i]);
      char* fname;
      while((fname=d.Next()) && fname[0]) {
	clsregexp(fname,cls,dat,osuffix);
      }
    }
  }

  return(0);
}

