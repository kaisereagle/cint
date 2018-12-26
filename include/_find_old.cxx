/*****************************************************************************
 * _find_old.cxx
 *  Description: Find files
 *    cint _find_old.cxx <option> [fileExpr] <[fileExpr2] <...>>
 *  Status: Out of date
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
//////////////////////////////////////////////////////////////////////
// _find.cxx
//////////////////////////////////////////////////////////////////////
//#include <EasyExp.dll>
#include <CSV.h>


int main(int argc,char** argv) {
#ifdef N
  EasyExp_num_threads = N;
  CSVClustering_num_threads = N;
#endif
#ifdef DEBUG
  CLDEBUG=1;
#endif
  int caseindependent=1;
  int isdirectory=2;
  int tocsv=0;
  double clstthresh=0.5;
  string clstmode="";
  int regexpmode=1;
  int list_limit=100;
  Scandir d;
  for(int i=1;i<argc;i++) {
    if(argv[i][0]=='-') {
      switch(tolower(argv[i][1])) {
      case 'f': isdirectory=0; break; // file only
      case 'd': isdirectory=1; break; // directory only
      case 'b': isdirectory=2; break; // both
      case 'c': caseindependent=0; break; // case sensitive
      case 'i': caseindependent=1; break; // case independent
      case 'p': tocsv=1; break; // store result to Collection
      case 't': clstthresh=atof(argv[++i]); // 0.0-1.0 
	if(clstmode=="") clstmode="CHAR";
	tocsv=1;
	break; 
      case 'm': clstmode=argv[++i]; // CHAR, HEAD, WORD
	break; 
      case 'r': regexpmode=atoi(argv[++i]); // -1,0,1,2,3
	if(clstmode=="") clstmode="CHAR";
	tocsv=1;
	break; 
      case 'l': list_limit=atoi(argv[++i]); // list_limit
	if(clstmode=="") clstmode="CHAR";
	break; 
      default: 
        fprintf(stderr,"Unknown option %s\n",argv[i]);
        fprintf(stderr,"Usage:\n");
        fprintf(stderr," _find.cxx <options> pathdescription\n");
        fprintf(stderr,"    -f : list file only\n");
        fprintf(stderr,"    -d : list directory only\n");
        fprintf(stderr,"    -b : list file and directory (default)\n");
        fprintf(stderr,"    -c : case sensitive\n");
        fprintf(stderr,"    -i : case insensitive (default)\n");
        fprintf(stderr,"    -p : Prompt interaction\n");
        fprintf(stderr,"    -m [CHAR|HEAD|WORD] : clustering mode\n");
        fprintf(stderr,"    -t [thresh] : set clustering threshold\n");
        fprintf(stderr,"    -r [-1,0,1] : set regular expression compilation mode\n");
        fprintf(stderr,"    -l [2..*] : set regular expression compilation list limit\n");
        exit(255);
      }
    }
    else {
      if(tocsv) d.scan(argv[i],isdirectory,caseindependent);
      else d.scandisp(argv[i],isdirectory,caseindependent);
    }
  }

  if(tocsv || clstmode!="") {
    Collection c,t; Line line;
    // set key line
    line.let(0,"filename");
    c.push_back(line);
    c.setkey(0);
    // fill contents
    vector<string> v;
    d.get(v);
    for(i=0;i<v.size();i++) {
      line.let(0,v[i]);
      c.push_back(line);
    }
    if(clstmode!="") {
      t=StringClustering(c,0,clstthresh,clstmode.c_str(),regexpmode,list_limit);
      t = t.column("regex","count");
      G__ateval(t);
    }
    else G__ateval(c);
    if(tocsv) while(!G__pause());
  }
  return(0);
}
