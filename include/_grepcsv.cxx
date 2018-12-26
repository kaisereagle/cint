#include <CSV.h>
#include <string>

int grepcsv(const string& fname,const string& linesel,const string& colsel) {
  string colkey;
  int colbegin=-1 ,colend=-1;
  string lineselkey, lineselexpr;
  ReadF p;
  p.setdelimiter("=");
  p.setseparator("");
  p.parse(linesel.c_str());
  if(p.argc>0) lineselkey = p.argv[1];
  if(p.argc>1) lineselexpr = p.argv[2];

  if(isdigit(colsel[0])) {
    p.setdelimiter("-");
    p.parse(colsel.c_str());
    if(p.argc>0) colbegin = atoi(p.argv[1]);
    if(p.argc>1) colend   = atoi(p.argv[2]);
    else colend=colbegin;
  }
  else colkey = colsel;

  Sheet s(fname);
  Collection c=s.collection();
  c.setkey(0);
  if(lineselkey.size()&&lineselexpr.size()) c=c.match(lineselkey,lineselexpr);
  if(colkey.size()) c=c.columns(colkey);
  if(colend>=0) c = c.column(colbegin,colend);

  printf("%s ",fname.c_str());
  for(int i=fname.size();i<78;i++) printf("#");
  printf("\n");
  c.disp();
  //G__ateval(c);
  printf("size=%d\n",c.size());
  return(0);
}

int main(int argc,char** argv) {
#ifdef __CINT__
  G__optimizemode(3);
#endif
  string linesel,colsel;
  int i;
  int caseindependent=1;
  int isdirectory=0;
  Scandir d;
  ReadF p;
  p.setseparator("");
  p.setdelimiter("/");
  for(int i=1;i<argc;i++) {
    if(argv[i][0]=='-') {
      switch(tolower(argv[i][1])) {
      case 'c': caseindependent=0; break; // case sensitive
      case 'i': caseindependent=1; break; // case independent
      default: 
        fprintf(stderr,"Unknown option %s\n",argv[i]);
        fprintf(stderr,"Usage:\n");
        fprintf(stderr," _grepcsv.cxx <-f|-d|-b> <-c|-i> pathdescription\n");
        fprintf(stderr,"    -c : case sensitive\n");
        fprintf(stderr,"    -i : case insensitive (default)\n");
        exit(255);
      }
    }
    else if(argv[i][0]=='/') {
      p.parse(argv[i]);
      if(p.argc>1) linesel = p.argv[2];
      if(p.argc>2) colsel = p.argv[3];
    }
    else {
      char *fname;
      d.scan(argv[i],isdirectory,caseindependent);
      while((fname=d.Next()) && fname[0]) {
	grepcsv(fname,linesel,colsel);
      }
    }
  }
  return(0);
}
