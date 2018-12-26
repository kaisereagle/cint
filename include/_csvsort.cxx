/*****************************************************************************
 * _csvsort.cxx
 *  Description: Sort csv file by column contents
 *    cint _csvsort.cxx -k key1 <-k2 key2> <-k3 key3> <-k4 key4> [targetfileExpr]
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
int csvsort(const string& fname
	    ,const string& k1
	    ,const string& k2
	    ,const string& k3
	    ,const string& k4) {
  printf("sorting %s : %s %s %s %s\n",fname.c_str()
	 ,k1.c_str() ,k2.c_str() ,k3.c_str() ,k4.c_str());
  Sheet s(fname.c_str());
  Collection c=s.collection();
  c.setkey(0);
  c.multisort(k1,k2,k3,k4);
  c.disp(fname.c_str(),"w");
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
  string k1;
  string k2="###";
  string k3="###";
  string k4="###";
  Scandir d;
  for(int i=1;i<argc;i++) {
    if(argv[i][0]=='-') {
      if(strcmp(argv[i],"-k")==0) k1=argv[++i];
      else if(strcmp(argv[i],"-k1")==0) k1=argv[++i];
      else if(strcmp(argv[i],"-k2")==0) k2=argv[++i];
      else if(strcmp(argv[i],"-k3")==0) k3=argv[++i];
      else if(strcmp(argv[i],"-k4")==0) k4=argv[++i];
      else {
        fprintf(stderr,"Unknown option %s\n",argv[i]);
        fprintf(stderr,"Usage:\n");
        fprintf(stderr," _csvsort.cxx -k [key1] <-k2 [key2]> <-k3 [key3]> fileexpr\n");
	exit(255);
      }
    }
    else {
      d.scan(argv[i]);
      char* fname;
      while((fname=d.Next()) && fname[0]) {
	csvsort(fname,k1,k2,k3,k4);
      }
    }
  }

  return(0);
}
