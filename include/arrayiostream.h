/***************************************************************************
* arrayiostream.h
*
*  array and carray type graph plot and file I/O class
*
*	plot << x << y << "\n";
*
*	plot << "title of graph"
*            << x  << "xunit"           << xmin >> xmax << LOG
*     	     << y1 << "name of y1 data" << ymin >> ymax << LOG
*     	     << y2 << "name of y2 data"  // multiple y axis data(optional)
*     	     << y3 << "name of y3 data"  //
*     	     << "yunit\n" ;              // must end with \n
*
*       fout << x << y1 << y2 << y3 << "\n";
*       fin  >> x >> y1 >> y2 >> y3 >> "\n";
*
****************************************************************************/
#ifndef G__ARRAYIOSTREAM_H
#define G__ARRAYIOSTREAM_H

#define CINTPLOT

#include <rootgraph.cxx>

#ifndef G__XGRAPHSL


#ifdef G__SHAREDLIB
#pragma include_noerr <xgraph.dl>
# ifndef G__XGRAPHSL
#include <xgraph.c>
# endif
#else
#include <xgraph.c>
#endif

#endif


typedef unsigned short UNITCIRCLE_t;
UNITCIRCLE_t UNITCIRCLE;

class XY_t DRAW_XY;

/**********************************************************
* arrayostream
**********************************************************/
class arrayostream {
  // Due to cint limitation, base class inheritance is emulated by
  // having pointer of graphbuf and construct it by new operator.
  graphbuf *buf;  
  
  // data member
  FILE *fp;
  int filen;
  int importedfp;
  int unitcircle;               // unit circle mode
  int iscsv;
  int isflushed;
  
  // private member function
  void plotdata(void);
  void rootgraph(void);
  void printdata(void);
  void csvdata(void);
  
 public:
  // constructor
  arrayostream(FILE *fp_init,int iscsvin=0);
  arrayostream(char *filename,int iscsvin=0);
  ~arrayostream();
  
  void init(FILE *fp_init)
    {if(fp&&fp!=stdout&&fp!=stderr) fclose(fp); fp=fp_init;}
  void init(char* fname)
    {if(fp&&fp!=stdout&&fp!=stderr) fclose(fp);fp=fopen(fname,"w");}
  void close() 
    {if(fp&&fp!=stdout&&fp!=stderr) fclose(fp); fp=NULL;}
  
  // ostream pipeline operator
  arrayostream& operator <<(array& a);
  arrayostream& operator <<(carray& a);
  arrayostream& operator <<(char *s);    // give name or title+do plot
  arrayostream& operator <<(double min); // specify min scale
  arrayostream& operator  |(double max); // specify max scale
  arrayostream& operator <<(int min); // specify min scale
  arrayostream& operator >>(int max); // specify max scale
  arrayostream& operator <<(ISLOG log);  // specify log scale
  arrayostream& operator <<(UNITCIRCLE_t uc);  // specify log scale
  arrayostream& operator <<(char c);     // do plot
  arrayostream& plotcsv(const char* fname);
#ifdef G__ROOT
  TGraph* gr() { return(rootgraph_gr()); }
#endif
  arrayostream& XY() { iscsv |= RG_CORR; return(*this); }
} ;


// constructor

arrayostream::arrayostream(FILE *fp_init,int iscsvin)
{
  buf = new graphbuf; // base class emulation
  filen=0;
  fp=fp_init;
  importedfp=1;
  unitcircle=0;
  iscsv = iscsvin;
  isflushed=0;
}

arrayostream::arrayostream(char *filename,int iscsvin)
{
  buf = new graphbuf; // base class emulation
  filen=0;
  fp=fopen(filename,"wb");
  if(NULL==fp) cerr << filename << " could not open\n" ;
  importedfp=0;
  unitcircle=0;
  iscsv = iscsvin;
  isflushed=0;
}

arrayostream::~arrayostream()
{
  if(0==importedfp && fp && stdout!=fp && stderr!=fp) fclose(fp);
  delete buf; // base class emulation
}


//
arrayostream& arrayostream::operator <<(graphbuf& a)
{
  *buf = a;
  if(buf->isterminate()) {
    terminated();
  }
  return(*this);
}

// add array or carray to plot
arrayostream& arrayostream::operator <<(array& a)
{
  *buf << a;
  return(*this);
}

arrayostream& arrayostream::operator <<(carray& a)
{
  *buf << a;
  return(*this);
}


// add min scale information
arrayostream& arrayostream::operator <<(double min)
{
  *buf << min;
  return(*this);
}

// add max scale information
arrayostream& arrayostream::operator |(double max)
{
  *buf | max;
  return(*this);
}
// add min scale information
arrayostream& arrayostream::operator <<(int min)
{
  *buf << (double)min;
  return(*this);
}

// add max scale information
arrayostream& arrayostream::operator >>(int max)
{
  *buf | (double)max;
  return(*this);
}
arrayostream& arrayostream::operator >>(double max)
{
  *buf | max;
  return(*this);
}

// add log information
arrayostream& arrayostream::operator <<(ISLOG log)
{
  *buf << log;
  return(*this);
}

// add unit circle information
arrayostream& arrayostream::operator <<(UNITCIRCLE_t uc)
{
  unitcircle=1;
  return(*this);
}

// add unit circle information
arrayostream& arrayostream::operator <<(XY_t uc)
{
  iscsv |= RG_CORR;
  return(*this);
}

// add title of plot
arrayostream& arrayostream::operator <<(char *s)
{
  *buf << s;
  if(buf->isterminate()) {
    terminated();
  }
  return(*this);
}

// do plot or print
arrayostream& arrayostream::operator <<(char c)
{
  *buf << c;
  terminated();
  return(*this);
}
arrayostream& arrayostream::operator <<(G__CINT_ENDL c)
{
  *buf << '\n';
  terminated();
  return(*this);
}

arrayostream::terminated(void)
{
  int i;
  if(iscsv&RG_CSVMODE) {
    csvdata();
  }
  else if((iscsv&RG_MASK_PLOTMODE)>=RG_PLOTMODE) {
    rootgraph(iscsv);
  }
  else if(NULL==fp) {
    plotdata();
  }
  else if(importedfp) {
    printdata();
  }
  else {
    FILE *FP;
    FP=fp;             // Assigning tp FP
    buf->dumpdata(FP); // cint bug work around
  }
  buf->freebuf();
  isflushed=0;
}

// flush buffer for superimposing
arrayostream& arrayostream::operator <<(G__CINT_FLUSH c)
{
  *buf << '\n';
  flushed();
  return(*this);
}

void arrayostream::flushed(void) {
  if(iscsv&RG_CSVMODE) {
    //csvdata();
  }
  else if((iscsv&RG_MASK_PLOTMODE)>=RG_PLOTMODE) {
    rootgraph_flushed(iscsv);
  }
  else if(NULL==fp) {
    //plotdata();
  }
  else if(importedfp) {
    //printdata();
  }
  else {
    //
  }
  buf->freebuf();
  ++isflushed;
}

void arrayostream::rootgraph_flushed(int mode) {
  int i,nplot,n,n;
  char fname[100];

  nplot = buf->Nplot();
  switch(nplot) {
  case 0:
    cerr << "No data to plot\n";
    break;
  case 1:
    n=buf->Npoint(0);
    array tmpx=array(0,n-1,n);
    sprintf(fname,"G__graph%d",filen++);
    if(!isflushed) rootgraph_open(fname,buf->Title(),mode);
    else rootgraph_open(fname,buf->Title(),4);
    rootgraph_data(buf->Title()
		,tmpx.dat
		,buf->Pdat(0)
		,n
		,buf->Name(0)
		);
    break;
  default:
    sprintf(fname,"G__graph%d",filen++);
    if(!isflushed) rootgraph_open(fname,buf->Title(),mode);
    else rootgraph_open(fname,buf->Title(),4);
    for(i=1;i<nplot;i++) {
      if(buf->Npoint(0)>buf->Npoint(i))  n = buf->Npoint(i);
      else                               n = buf->Npoint(0);
      rootgraph_data(buf->Title()
		  ,buf->Pdat(0)
		  ,buf->Pdat(i)
		  ,n
		  ,buf->Name(i)
		  );
    }
    if(unitcircle) {
      int nuc = 100;
      double dpi = 3.141592*2/100;
      double x[100],y[100];
      for(i=0;i<nuc;i++) {
	x[i] = cos(dpi*i);
	y[i] = sin(dpi*i);
      }
      rootgraph_data(buf->Title()
		  ,x
		  ,y
		  ,nuc
		  ,"unit_circle"
		  );
    }
    break;
  }
}

// plot data using root graphics
void arrayostream::rootgraph(int mode)
{
  int i,nplot,n,n;
  char fname[100] = "";
  
  nplot = buf->Nplot();

  switch(nplot) {
  case 0:
#ifdef CINTPLOT
    rootgraph_invoke(fname
		  ,buf->Xmin(),buf->Xmax()
		  ,buf->Ymin(),buf->Ymax()
		  ,buf->Xlog(),buf->Ylog()
		  ,(char*)NULL,buf->Yunit(),mode);
#else
    cerr << "No data to plot\n";
#endif
    break;
  case 1:
    n=buf->Npoint(0);
    array tmpx=array(0,n-1,n);
    sprintf(fname,"G__graph%d",filen++);
    if(!isflushed) { 
      rootgraph_open(fname,buf->Title(),mode);
      fname[0]=0;
    }
    else { 
      rootgraph_open(fname,buf->Title(),4);
      strcpy(fname,buf->Title());
    }
    rootgraph_data(fname
		,tmpx.dat
		,buf->Pdat(0)
		,n
		,buf->Name(0)
		);
    rootgraph_invoke(fname
		  ,buf->Xmin(),buf->Xmax()
		  ,buf->Ymin(),buf->Ymax()
		  ,buf->Xlog(),buf->Ylog()
		  ,(char*)NULL,buf->Yunit(),mode);
    break;
  default:
    sprintf(fname,"G__graph%d",filen++);
    if(!isflushed) {
      rootgraph_open(fname,buf->Title(),mode);
      fname[0]=0;
    }
    else { 
      rootgraph_open(fname,buf->Title(),4);
      strcpy(fname,buf->Title());
    }
    for(i=1;i<nplot;i++) {
      if(buf->Npoint(0)>buf->Npoint(i))  n = buf->Npoint(i);
      else                               n = buf->Npoint(0);
      rootgraph_data(fname
		  ,buf->Pdat(0)
		  ,buf->Pdat(i)
		  ,n
		  ,buf->Name(i)
		  );
    }
    if(unitcircle) {
      int nuc = 100;
      double dpi = 3.141592*2/100;
      double x[100],y[100];
      for(i=0;i<nuc;i++) {
	x[i] = cos(dpi*i);
	y[i] = sin(dpi*i);
      }
      rootgraph_data(""
		  ,x
		  ,y
		  ,nuc
		  ,"unit_circle"
		  );
    }
    rootgraph_invoke(fname
		  ,buf->Xmin(),buf->Xmax()
		  ,buf->Ymin(),buf->Ymax()
		  ,buf->Xlog(),buf->Ylog()
		  ,buf->Name(0),buf->Yunit(),mode);
    break;
  }
}

// plot data using xgraph
void arrayostream::plotdata(void)
{
  int i,nplot,n,n;
  char fname[100];
  int mode = iscsv;
  
  nplot = buf->Nplot();

  switch(nplot) {
  case 0:
    cerr << "No data to plot\n";
    break;
  case 1:
    n=buf->Npoint(0);
    array tmpx=array(0,n-1,n);
    sprintf(fname,"G__graph%d",filen++);
#ifdef G__ROOT
    if(!isflushed) rootgraph_open(fname,buf->Title(),mode);
    else rootgraph_open(fname,buf->Title(),4);
#else
    xgraph_data(fname
		,tmpx.dat
		,buf->Pdat(0)
		,n
		,buf->Name(0)
		);
    xgraph_invoke(fname
		  ,buf->Xmin(),buf->Xmax()
		  ,buf->Ymin(),buf->Ymax()
		  ,buf->Xlog(),buf->Ylog()
		  ,(char*)NULL,buf->Yunit());
#endif
    break;
  default:
    sprintf(fname,"G__graph%d",filen++);
    //xgraph_open(fname,buf->Title());
    if(!isflushed) rootgraph_open(fname,buf->Title(),mode);
    else rootgraph_open(fname,buf->Title(),4);
    for(i=1;i<nplot;i++) {
#ifdef G__ROOT
      if(buf->Npoint(0)>buf->Npoint(i))  n = buf->Npoint(i);
      else                               n = buf->Npoint(0);
#else
      xgraph_data(fname
		  ,buf->Pdat(0)
		  ,buf->Pdat(i)
		  ,n
		  ,buf->Name(i)
		  );
#endif
    }
    if(unitcircle) {
      int nuc = 100;
      double dpi = 3.141592*2/100;
      double x[100],y[100];
      for(i=0;i<nuc;i++) {
	x[i] = cos(dpi*i);
	y[i] = sin(dpi*i);
      }
#ifndef G__ROOT
      xgraph_data(fname
		  ,x
		  ,y
		  ,nuc
		  ,"unit_circle"
		  );
#endif
    }
#ifndef G__ROOT
    xgraph_invoke(fname
		  ,buf->Xmin(),buf->Xmax()
		  ,buf->Ymin(),buf->Ymax()
		  ,buf->Xlog(),buf->Ylog()
		  ,buf->Name(0),buf->Yunit());
#endif
    break;
  }
}

// print data to file
void arrayostream::printdata(void)
{
  int i,n;
  int Ndata,Nplot;
  
  Ndata = buf->Minn();
  Nplot = buf->Nplot();
  for(i=0;i<Ndata;i++) {
    for(n=0;n<Nplot;n++) {
      fprintf(fp,"%g ",buf->pdat[n][i]);
      // Doing above due to cint limitation,
      // supposed to be like below.
      // fprintf(fp,"%g ",buf->Dat(n,i)); 
    }
    fprintf(fp,"\n");
  }
}

// output data in CSV (Comma Separated Value)
void arrayostream::csvdata(void)
{
  int i,nplot,n,n;
  int nplot = buf->Nplot();
  int n=buf->Npoint(0);
  double *p;
  if(!fp) fp=stdout;
  printf("output csv data... %d %d\n",n,nplot);
  fprintf(fp,"npoint,ndata,title,xmin,xmax,xlog,xunit,ymin,ymax,ylog,yunit,\n");
  fprintf(fp,"%d,%d,%s,%g,%g,%d,%s,%g,%g,%d,%s,\n"
	  ,n,nplot-1 ,buf->Title()
	  ,buf->Xmin() ,buf->Xmax(), buf->Xlog(), ""
	  ,buf->Ymin() ,buf->Ymax(), buf->Ylog(), buf->Yunit());
  int flag=0;
  for(int j=0;j<nplot;j++) {
    if(buf->Name(j) && strlen(buf->Name(j))) flag=1;
  }
  if(flag) {
    for(int j=0;j<nplot;j++) {
      if(buf->Name(j)) fprintf(fp,"%s,",buf->Name(j));
      else fprintf(fp,",");
    }
  }
  fprintf(fp,"\n");
  for(int i=0;i<n;i++) {
    for(int j=0;j<nplot;j++) {
      p = buf->Pdat(j);
      fprintf(fp,"%g,",p[i]);
    }
    fprintf(fp,"\n");
  }
  fprintf(fp,"\n");
}

///////////////////////////////////////////////////////////////////////
// plot all data from a csv file saved in defined format
///////////////////////////////////////////////////////////////////////
#include <ReadF.h>
#include <string>
void arrayostream::csv(const char* fname) {
  ReadF f(fname);
  while(csv(f));
}

///////////////////////////////////////////////////////////////////////
// plot one data from csv file saved in defined format
///////////////////////////////////////////////////////////////////////
int arrayostream::csv(ReadF& f) {
#ifndef G__ROOT
  return 0;
#endif
  f.setseparator("");
  f.setdelimiter(",");
  f.setquotation("\"'");
  if(!f.read()) return(0);
  //f.disp();
  string title1,xname1,yname1[50],xunit,yunit;
  double xmin=0,xmax=0,ymin=0,ymax=0;
  int n,nplot;
  int xlog=0,ylog=0;
  if(f.argc>=11) {
    if(!f.read()) return(0);
    n=atoi(f.argv[1]);
    nplot=atoi(f.argv[2]);
    title1=f.argv[3];
    xmin=atof(f.argv[4]);
    xmax=atof(f.argv[5]);
    xlog=atof(f.argv[6]);
    xunit=f.argv[7];
    ymin=atof(f.argv[8]);
    ymax=atof(f.argv[9]);
    ylog=atof(f.argv[10]);
    yunit=f.argv[11];
    if(!f.read()) return(0);
  }
  if(f.argc>=1) xname1=f.argv[1];
  int i,j;
  for(i=2;i<=f.argc;i++) yname1[i-2]=f.argv[i];
  array xx(0.0,0.0,n);
  array yy[50];
  j=0;
  while(f.read()&&j<n) {
    xx[j] = atof(f.argv[1]);
    for(i=0;i<nplot&&i+2<=f.argc;i++) yy[i][j] = atof(f.argv[i+2]);
    j++;
  }
  //f.disp();
  plot << title1.c_str() ;
  plot << xx << xname1.c_str();
  if(xmin!=xmax) plot << xmin >> xmax ; 
  if(xlog) plot << LOG;
  for(i=0;i<nplot;i++) {
    plot << yy[i] << yname1[i].c_str();
  }
  if(ymin!=ymax) plot << ymin >> ymax ; 
  if(ylog) plot << LOG;
  char buf[100];
  sprintf(buf,"%s\n",yunit.c_str());
  plot << buf;
  return(f.isvalid());
}

/**********************************************************
* arraycsv 
**********************************************************/
class arraycsv {
 public:
  arraycsv(const char* fname) : f(fname) { 
    f.setseparator("");
    f.setdelimiter(",");
    f.setquotation("\"'");
  }
  ReadF* getReadF() { return(&f); }
  int readline() { return(f.read()); }
  int open(const char* fname) { return(f.open(fname)); }
  int close() { return(f.close()); }
  int getpos() { return(f.getpos()); }
  int rewind() { return(f.rewind()); }

  // auto format judgement
  int read(int& n,int& nplot,array& xx,array* yy
	   ,char* title=0,char* xname=0,char* yname=0) ;
  int read(int& n,int& nplot,array* yy
	   ,char* title=0,char* xname=0,char* yname=0) ;

  // read (arrayostream)csv output
  int get(int& nplot,array& xx,array* yy
	  ,char* title=0,char* xname=0,char* yname=0) ;
  // read no-format csv data
  int get(int& n,int& nplot,array& xx,array* yy);

  // read (arrayostream)csv output
  int get(int& nplot,array* yy
	  ,char* title=0,char* xname=0,char* yname=0) ;
  // read no-format csv data
  int get(int& n,int& nplot,array* yy);

  // manual read
  int read(int& n,const char* location,array* yy);
  void parselocation(const char* arg,pair<int,int>& loc);
 private:
  int linecategory() ;
  int isnumber(const char* s) {
    if(!s && (isdigit(s[0]) || s[0]=='.' || s[0]=='-' || s[0]=='+')) return(1);
    else return(0);
  }
  int isstring(const char* s) {
    if(!s || (!isdigit(s[0])&&s[0]!='.' && s[0]!='-' && s[0]!='+')) return(1);
    else return(0);
  }
  ReadF f;
};

/////////////////////////////////////////////////////////////////////////
int arraycsv::linecategory() {
  int isstr=0;
  for(int i=1;i<f.argc;i++) isstr+=isstring(f.argv[i]);
  if(isstr) return(-f.argc);
  else      return(f.argc);
}

/////////////////////////////////////////////////////////////////////////
// manual read
/////////////////////////////////////////////////////////////////////////
int arraycsv::read(int& n,const char* location,array* yy) {
  ReadF p;
  p.setseparator("");
  p.setdelimiter(",");
  p.parse(location);
  pair<int,int> loc[20];
  int i,nplot=p.argc;
  for(i=0;i<nplot;i++) {
    parselocation(p.argv[i+1],loc[i]);
  }

  int sizeinc=50000;
  int j=0;
  int flag;
  while(f.read() && (!n||j<n) ) {
    flag=0;
    for(i=0;i<nplot;i++) {
      if(loc[i].second<=f.line && (!n||j<n)) {
	if(yy[i].size()<=j) yy[i].resize(yy[i].size()+sizeinc);
	yy[i][j] = atof(f.argv[loc[i].first+1]);
	++flag;
      }
    }
    if(flag) ++j;
  }
  
  for(i=0;i<nplot;i++) yy[i].resize(j);
  return(1);
}
/////////////////////////////////////////////////////////////////////////
void arraycsv::parselocation(const char* arg,pair<int,int>& loc) {
  loc.first=0;
  loc.second=0;
  int c;
  int i=0;
  while(isalpha(c=toupper(arg[i]))) {
    loc.first = loc.first*26 + c-'A';
    ++i;
  }
  while((c=arg[i])) {
    loc.second = loc.second*10 + c-'0';
    ++i;
  }
}

/////////////////////////////////////////////////////////////////////////
// auto format judgement
/////////////////////////////////////////////////////////////////////////
int arraycsv::read(int& n,int& nplot,array& xx,array* yy
		   ,char* title,char* xname,char* yname) {
  int posnum=1;
  int stat=0;
  f.getpos(posnum);
  while(f.read() && (f.argc==0 || !f.argv[1][0]) ) f.getpos(posnum);
  if(f.isvalid()) {
    if(strcmp(f.argv[1],"npoint")==0) {
      //npoint,ndata,title,xmin,xmax,xlog,xunit,ymin,ymax,ylog,yunit
      //500,1,title,...
      //xname,yname1,yname2
      //1,0.2,0,3
      //2,0.4,0,7
      f.rewind(posnum);
      stat=get(nplot,xx,yy,title,xname,yname) ;
      n = xx.size();
    }
    else if(linecategory()>0) {
      //1,0.2,0,3
      //2,0.4,0,7
      f.rewind(posnum);
      stat=get(n,nplot,xx,yy) ;
    }
    else {
      //comment
      //comment
      //0,3,2,1
      //xname,yname1,yname2
      //1,0.2,0,3
      //2,0.4,0,7
      const int LINES=10;
      int line[LINES];
      line[0] = linecategory();
      int startline= 0;
      int i;
      for(i=1;i<LINES;i++) {
	f.read();
	line[i] = linecategory();
	if(line[i-1]<=0 && line[i]>0) {
	  if(startline==0) startline=i;
	  else if(startline && line[startline+1]<0) startline=i;
	}
      }
//for(i=0;i<LINES;i++) printf("%d:%d ",i,line[i]);
//printf("startline=%d \n",startline);
      f.rewind(posnum);
      for(i=0;i<startline;i++) f.read(); // read up to xname line
//f.disp();
      if(f.argc>=1 && xname) strcpy(xname,f.argv[1]);
      int i,j;
      if(yname) {
	yname[0]=0;
	for(i=2;i<=f.argc;i++) {
	  strcat(yname,f.argv[i]);
	  strcat(yname,",");
	}
      }
      stat=get(n,nplot,xx,yy) ;
    }
  }
  return(stat);
}

/////////////////////////////////////////////////////////////////////////
// read (arrayostream)csv output
/////////////////////////////////////////////////////////////////////////
int arraycsv::get(int& nplot,array& xx,array* yy,char* title,char* xname,char* yname) {
  if(!f.read()) return(0);
  //f.disp();
  string title1,xname1,yname1[50],xunit,yunit;
  double xmin=0,xmax=0,ymin=0,ymax=0;
  int n;
  int xlog=0,ylog=0;
  if(f.argc>=11) {
    if(!f.read()) return(0);
    n=atoi(f.argv[1]);
    if(nplot==0) nplot=atoi(f.argv[2]);
    if(title) strcpy(title,f.argv[3]);
    xmin=atof(f.argv[4]);
    xmax=atof(f.argv[5]);
    xlog=atof(f.argv[6]);
    xunit=f.argv[7];
    ymin=atof(f.argv[8]);
    ymax=atof(f.argv[9]);
    ylog=atof(f.argv[10]);
    yunit=f.argv[11];
    if(!f.read()) return(0);
  }
  if(f.argc>=1 && xname) strcpy(xname,f.argv[1]);
  int i,j;
  if(yname) {
    yname[0]=0;
    for(i=2;i<=f.argc;i++) {
      strcat(yname,f.argv[i]);
      strcat(yname,",");
    }
  }
  xx.resize(n);
  for(i=0;i<nplot;i++) yy[i].resize(n);
  j=0;
  while(f.read()&&j<n) {
    xx[j] = atof(f.argv[1]);
    for(i=0;i<nplot&&i+2<=f.argc;i++) yy[i][j] = atof(f.argv[i+2]);
    j++;
  }
  return(f.isvalid());
}

/////////////////////////////////////////////////////////////////////////
// read no-format csv data
/////////////////////////////////////////////////////////////////////////
int arraycsv::get(int& n,int& nplot,array& xx,array* yy) {
  if(!f.read()) return(0);
  if(nplot==0) nplot = f.argc-1;
  int i=0,j;
  if(n) {
    xx.resize(n);
    xx[i]=atof(f.argv[1]);
    for(j=0;j<nplot;j++) {
      yy[j][i] = atof(f.argv[2+j]);
      yy[j].resize(n);
    }
    for(i=1;i<n;i++) {
      if(!f.read()) return(0);
      xx[i]=atof(f.argv[1]);
      for(j=0;j<nplot;j++) yy[j][i] = atof(f.argv[2+j]);
    }
  }
  else {
    int sizeinc=50000;
    xx[i]=atof(f.argv[1]);
    for(j=0;j<nplot;j++) {
      if(xx.size()!=yy[j].size()) yy[j].resize(xx.size());
      yy[j][i] = atof(f.argv[2+j]);
    }
    ++i;
    //while(f.read()) {
    while(f.read() && f.argc>=nplot+1 && f.argv[1][0]) {
      if(i>=xx.size()) {
        for(j=0;j<nplot;j++) yy[j].resize(xx.size()+sizeinc);
        xx.resize(xx.size()+sizeinc);
        printf("i=%d\n",i);
      }
      xx[i]=atof(f.argv[1]);
      for(j=0;j<nplot;j++) yy[j][i] = atof(f.argv[2+j]);
      ++i;
    }
    xx.resize(i);
    for(j=0;j<nplot;j++) yy[j].resize(i);
    n=i;
  }
  return(f.isvalid());
}
/////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////
// auto format judgement
/////////////////////////////////////////////////////////////////////////
int arraycsv::read(int& n,int& nplot,array* yy
		   ,char* title,char* xname,char* yname) {
  int posnum=1;
  int stat=0;
  f.getpos(posnum);
  while(f.read() && (f.argc==0 || !f.argv[1][0]) ) f.getpos(posnum);
  if(f.isvalid()) {
    if(strcmp(f.argv[1],"npoint")==0) {
      //npoint,ndata,title,xmin,xmax,xlog,xunit,ymin,ymax,ylog,yunit
      //500,1,title,...
      //xname,yname1,yname2
      //1,0.2,0,3
      //2,0.4,0,7
      f.rewind(posnum);
      stat=get(nplot,yy,title,xname,yname) ;
      n = yy[0].size();
    }
    else if(linecategory()>0) {
      //1,0.2,0,3
      //2,0.4,0,7
      f.rewind(posnum);
      stat=get(n,nplot,yy) ;
    }
    else {
      //comment
      //comment
      //0,3,2,1
      //xname,yname1,yname2
      //1,0.2,0,3
      //2,0.4,0,7
      const int LINES=10;
      int line[LINES];
      line[0] = linecategory();
      int startline= 0;
      int i;
      for(i=1;i<LINES;i++) {
	f.read();
	line[i] = linecategory();
	if(line[i-1]<=0 && line[i]>0) {
	  if(startline==0) startline=i;
	  else if(startline && line[startline+1]<0) startline=i;
	}
      }
//for(i=0;i<LINES;i++) printf("%d:%d ",i,line[i]);
//printf("startline=%d \n",startline);
      f.rewind(posnum);
      for(i=0;i<startline;i++) f.read(); // read up to xname line
//f.disp();
      if(f.argc>=1 && xname) strcpy(xname,f.argv[1]);
      int i,j;
      if(yname) {
	yname[0]=0;
	for(i=2;i<=f.argc;i++) {
	  strcat(yname,f.argv[i]);
	  strcat(yname,",");
	}
      }
      stat=get(n,nplot,yy) ;
    }
  }
  return(stat);
}

/////////////////////////////////////////////////////////////////////////
// read (arrayostream)csv output
/////////////////////////////////////////////////////////////////////////
int arraycsv::get(int& nplot,array* yy,char* title,char* xname,char* yname) {
  if(!f.read()) return(0);
  //f.disp();
  string title1,xname1,yname1[50],xunit,yunit;
  double xmin=0,xmax=0,ymin=0,ymax=0;
  int n;
  int xlog=0,ylog=0;
  if(f.argc>=11) {
    if(!f.read()) return(0);
    n=atoi(f.argv[1]);
    if(nplot==0) nplot=atoi(f.argv[2]);
    if(title) strcpy(title,f.argv[3]);
    xmin=atof(f.argv[4]);
    xmax=atof(f.argv[5]);
    xlog=atof(f.argv[6]);
    xunit=f.argv[7];
    ymin=atof(f.argv[8]);
    ymax=atof(f.argv[9]);
    ylog=atof(f.argv[10]);
    yunit=f.argv[11];
    if(!f.read()) return(0);
  }
  if(f.argc>=1 && xname) strcpy(xname,f.argv[1]);
  int i,j;
  if(yname) {
    yname[0]=0;
    for(i=2;i<=f.argc;i++) {
      strcat(yname,f.argv[i]);
      strcat(yname,",");
    }
  }
  for(i=0;i<nplot;i++) yy[i].resize(n);
  j=0;
  while(f.read()&&j<n) {
    for(i=0;i<nplot&&i+1<=f.argc;i++) yy[i][j] = atof(f.argv[i+1]);
    j++;
  }
  return(f.isvalid());
}

/////////////////////////////////////////////////////////////////////////
// read no-format csv data
/////////////////////////////////////////////////////////////////////////
int arraycsv::get(int& n,int& nplot,array* yy) {
  if(!f.read()) return(0);
  if(nplot==0) nplot = f.argc-1;
  int i=0,j;
  if(n) {
    for(j=0;j<nplot;j++) {
      yy[j][i] = atof(f.argv[1+j]);
      yy[j].resize(n);
    }
    for(i=1;i<n;i++) {
      if(!f.read()) return(0);
      for(j=0;j<nplot;j++) yy[j][i] = atof(f.argv[1+j]);
    }
  }
  else {
    int sizeinc=50000;
    for(j=0;j<nplot;j++) {
      if(yy[0].size()!=yy[j].size()) yy[j].resize(yy[0].size());
      yy[j][i] = atof(f.argv[2+j]);
    }
    ++i;
    //while(f.read()) {
    while(f.read() && f.argc>=nplot+1 && f.argv[1][0]) {
      if(i>=yy[0].size()) {
        int oldsize=yy[0].size();
        for(j=0;j<nplot;j++) yy[j].resize(oldsize+sizeinc);
        printf("i=%d\n",i);
      }
      for(j=0;j<nplot;j++) yy[j][i] = atof(f.argv[1+j]);
      ++i;
    }
    for(j=0;j<nplot;j++) yy[j].resize(i);
    n=i;
  }
  return(f.isvalid());
}
/////////////////////////////////////////////////////////////////////////

/**********************************************************
* istream
**********************************************************/
class arrayistream {
  // data member
  double *dat[G__ARRAYMAX];
  int ndat;
  int nplot;
  FILE *fp;
  int importedfp;
  
 public:
  // constructor
  arrayistream(FILE *fp_init);
  arrayistream(char *filename);
  ~arrayistream();
  
  // operator overloading
  arrayistream& operator >>(array& a);
  arrayistream& operator >>(carray& a);
  arrayistream& operator >>(char *s);
  arrayistream& operator >>(char c);
  arrayistream& operator >>(double d);  // dummy
  arrayistream& operator >>(ISLOG log); // dummy
  arrayistream& operator >>(graphbuf& a);
  
  void close(void);
} ;


arrayistream::arrayistream(FILE *fp_init) 
{ 
  fp=fp_init;
  nplot=0; 
  ndat=2000000; 
  importedfp=1;
}

arrayistream::arrayistream(char *filename)
{ 
  fp=fopen(filename,"rb");
  if(NULL==fp) cerr << filename << " could not open\n" ;
  nplot=0; 
  ndat=2000000; 
  importedfp=0;
}

arrayistream::~arrayistream()
{
  close();
}

void arrayistream::close(void)
{
  if(0==importedfp) fclose(fp);
}


arrayistream& arrayistream::operator >>(graphbuf& a)
{
  if(fp) {
    FILE *FP;       
    FP=fp;          // Assigning to FP
    a.loaddata(FP); // cint bug work around
    nplot=0;
		ndat=2000000;
  }
  else {
    cerr << "Can not read data\n";
    a.freebuf();
  }
  return(*this);
}

// add array or carray 
arrayistream& arrayistream::operator >>(array& a)
{
  dat[nplot++] = a.dat;
  if(ndat>a.n) ndat=a.n;
  return(*this);
}

arrayistream& arrayistream::operator >>(carray& a)
{
  dat[nplot++] = a.re;
  dat[nplot++] = a.im;
  if(ndat>a.n) ndat=a.n;
  return(*this);
}

// dummy 
arrayistream& arrayistream::operator >>(double d)
{
  return(*this);
}

arrayistream& arrayistream::operator |(double d)
{
  return(*this);
}

arrayistream& arrayistream::operator >>(ISLOG log)
{
  return(*this);
}

// do input
arrayistream& arrayistream::operator >>(char *s)
{
  if(s[strlen(s)-1]=='\n') 
    *this >> '\n';
  return(*this);
}

arrayistream& arrayistream::operator >>(char c)
{
  int i,n;
  if(NULL==fp) {
    cerr << "arrayistream: File pointer null\n" ;
    return;
  }
  
  if(nplot) {
    for(i=0;i<ndat;i++) {
      $read(fp);
      for(n=0;n<nplot;n++) {
	dat[n][i]=atof($(n+1));
      }
    }
  }
  
  nplot=0;
  ndat=2000000;
  return(*this);
}


// global object
#ifdef G__ROOT
arrayostream plot=arrayostream((FILE*)NULL,RG_PLOTMODE);
arrayostream same=arrayostream((FILE*)NULL,RG_SAMEMODE);
#else
arrayostream plot=arrayostream((FILE*)NULL);
#endif
arrayostream arraycout=arrayostream(stdout);
arrayostream arraycerr=arrayostream(stderr);
arrayistream arraycin=arrayistream(stdin);
arrayostream csv=arrayostream(stdout,RG_CSVMODE);



///////////////////////////////////////////////////////////////////////////

#endif
