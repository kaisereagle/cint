/*****************************************************************************
 * _csvCompare.cxx
 *  Description: remove files
 *    cint _csvCompare.cxx <option> [fileExpr] <[fileExpr2] <...>>
 *  Status: Experimental
 *  Date: 2015/Nov/14
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
//////////////////////////////////////////////////////////////////////
// _remove.cxx
//////////////////////////////////////////////////////////////////////
//#include <EasyExp.dll>
#include <CSV.h>
#include <unistd.h>

int isfirst=1;
Collection c,t,u;
vector<Sheet*> ps;

//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
void accumulate(const string& fname
		,int xkey,const string& keyexpr
		,int xvalue,const string& valueexpr) {
  ps.push_back(new Sheet(fname.c_str()));
  Collection cl,cl1,cl2;
  cl = ps.back()->collection();
  cl.setkey(0);
  string s1,s2;
  int i;

  if(xkey<0 && keyexpr!="") xkey=cl.getx(keyexpr);
  if(xvalue<0 && valueexpr!="") xvalue=cl.getx(valueexpr);

  if(xvalue<0) xvalue=0;

  if(xkey<0) {
    if(isfirst) {
      s1 = fname;
      if(valueexpr!="") s1 += ":" + valueexpr;
      c.setkey(s1.c_str());
    }
    c.column_push_back(cl.column(xvalue));
  }
  else {
    s1 = fname;
    //if(valueexpr!="") s1 += ":" + valueexpr;
    if(isfirst) { // first time, setup key line and keycolumn
      s2 = fname;
      c.setkey(s2.c_str(),s1.c_str());
      c=cl.column(xkey);
    }
    // fill value by matching key value
    int xnew = c[0].size();
    c.getkey().let(xnew,s1);
    for(i=0;i<c.size();i++) {
      cl1 = cl.match(xkey,c[i][0].c_str());
      if(cl1.size()) {
	c[i].let(xnew,cl1[0][xvalue]);
	cl1.let(xkey,"");
      }
    }
    // new key value, add new lines
    cl2 = cl._empty(xkey);
    Line line;
    for(i=0;i<cl2.size();i++) {
      line.let(0,cl2[i][xkey]);
      line.let(xnew,cl2[i][xvalue]);
      c.push_back(line);
    }
  }
  
  isfirst=0;
}
//////////////////////////////////////////////////////////////////////
void analyze() {
}

//////////////////////////////////////////////////////////////////////
int main(int argc,char** argv) {
  string keyexpr, valueexpr;
  int xkey= -1, xvalue= -1;
  int caseindependent=0;
  int isdirectory=2;
  int debug=0;
  Scandir d;
  for(int i=1;i<argc;i++) {
    if(argv[i][0]=='-') {
      if(strcmp(argv[i],"-g")==0)  debug=1;
      else if(strcmp(argv[i],"-k")==0) {
	++i;
	if(isdigit(argv[i][0])) xkey=atoi(argv[++i]); 
	else keyexpr=argv[i]; 
      }
      else if(strcmp(argv[i],"-v")==0) {
	++i;
	if(isdigit(argv[i][0])) xvalue=atoi(argv[++i]); 
	else valueexpr=argv[i]; 
      }
      else if(strcmp(argv[i],"-c")==0) caseindependent=0; 
      else if(strcmp(argv[i],"-i")==0) caseindependent=1; 
      else {
        fprintf(stderr,"Unknown option %s\n",argv[i]);
        fprintf(stderr,"Usage:\n");
        fprintf(stderr," _csvCompare.cxx <-f|-d|-b> <-c|-i> pathdescription\n");
        fprintf(stderr,"    -k [keycolumn]      : key column expression\n");
        fprintf(stderr,"    -v [valcolumn]      : value column expression\n");
        fprintf(stderr,"    -c : case sensitive\n");
        fprintf(stderr,"    -i : case insensitive (default)\n");
        fprintf(stderr,"    -g : debug mode\n");
        exit(255);
      }  
    }
    else {
      d.scan(argv[i],isdirectory,caseindependent);
      char* fname;
      while((fname=d.Next())&&fname[0]) {
	printf("Reading %s\n",fname);
	accumulate(fname,xkey,keyexpr,xvalue,valueexpr);
      }
      printf("\n");
    }
  }

  analyze();

  G__ateval(c);
  while(!G__pause()) ;

  return(0);
}
