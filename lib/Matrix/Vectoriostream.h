/***************************************************************************
* Graphbuf.h
*
*  DVector and DCVector type graph data buffer
*
*	buf  << x << y << "\n";
*
*	buf  << "title of graph"
*            << x  << "xunit"           << xmin | xmax << LOG
*     	     << y1 << "name of y1 data" << ymin | ymax << LOG
*     	     << y2 << "name of y2 data"  // multiple y axis data(optional)
*     	     << y3 << "name of y3 data"  //
*     	     << "yunit\n" ;              // must end with \n
*
*       plot << buf ;
*       Vectorostream("fname") << buf ;
*       Vectorostream("fname") >> buf ;
*
****************************************************************************/


#ifndef G__GRAPHBUF
#define G__GRAPHBUF


const int G__DVECTORMAX= 100;

typedef unsigned char ISLOG;
ISLOG LIN=0;  // log flag linear
ISLOG LOG=1;  // log flag log

/**********************************************************
* Graphbuf
**********************************************************/
class Graphbuf {
public:
  enum DATATYPE { G__DVECTORTYPE, G__DCVECTORTYPE, G__DCVECTORTYPE_RE, G__DCVECTORTYPE_IM };
  enum GRSTATUS { G__GRAPHBUF_PUSHING , G__GRAPHBUF_FIXED };

private:
  // data member
  int minn;                     // minimum of n[]
  int pout;                     // output pointer
  int status;                   // input status
  int iden[G__DVECTORMAX];        // identity, DVector, DCVector re,im
  int nplot;                    // number of data, input pointer
  int xlog,ylog;                // log flag
  double xmin,xmax,ymin,ymax;   // scale
  int n[G__DVECTORMAX];           // DVector of number of data
  
  char *title;                  // title of graph
  char *yunit;                  // y axis unit
  
  char *dataname[G__DVECTORMAX];  // name of data
public:
  double *pdat[G__DVECTORMAX];    // pointer to data DVector
  
public:
  // constructor, destructor, initialization
  Graphbuf(void);
  ~Graphbuf(void);
  void setnull(void);
  void freebuf(void);
  
  // assignment operator
  Graphbuf& operator =(Graphbuf& X);
  
  // ostream pipeline operator
  Graphbuf& operator <<(DVector& a);
  Graphbuf& operator <<(DCVector& a);
  Graphbuf& operator <<(char *s);    // give name or title+do plot
  Graphbuf& operator <<(double min); // specify min scale
  Graphbuf& operator  |(double max); // specify max scale
  Graphbuf& operator <<(ISLOG log);  // specify log scale
  virtual Graphbuf& operator <<(char c);  // do plot
  
  // istream pipeline operator
  Graphbuf& operator >>(DVector& a);
  Graphbuf& operator >>(DCVector& a);
  Graphbuf& operator >>(char *s);    // give name or title+do plot
  Graphbuf& operator >>(double min); // dummy
  Graphbuf& operator >>(ISLOG log);  // dummy
  Graphbuf& operator >>(char c);     // 
  
  int isterminate(void) {
    if(G__GRAPHBUF_FIXED==status) return(1);
    else                          return(0);
  }
  
  int Nplot(void)   { return(nplot); }
  int Npoint(int i) { return(n[i]); }
  int Minn(void)    { return(minn); }
  char *Name(int i) { return(dataname[i]); }
  char *Title(void) { return(title); }
  char *Yunit(void) { return(yunit); }
  double *Pdat(int i) { return(pdat[i]); }
  double Dat(int i,int n) { return(pdat[i][n]); }
  int Xlog(void)    { return(xlog); }
  int Ylog(void)    { return(ylog); }
  double Xmin(void) { return(xmin); }
  double Xmax(void) { return(xmax); }
  double Ymin(void) { return(ymin); }
  double Ymax(void) { return(ymax); }
  int Pout(void) { return(pout); }
  int Status(void) { return(status); }
  int Iden(int i)  { return(iden[i]); }
  void setStatus(int stat) { status = stat; }
  
  void dumpdata(FILE *fp);
  void loaddata(FILE *fp);
} ;


// constructor
Graphbuf::Graphbuf(void)
{
  setnull();
  pout=0;
}

// initialization
void Graphbuf::setnull(void)
{
	int i;
	for(i=0;i<G__DVECTORMAX;i++) {
		pdat[i] = NULL;
		n[i] = 0;
		dataname[i] = NULL;
	}
	title=NULL;
	yunit=NULL;
	nplot=0;
	status=G__GRAPHBUF_PUSHING;
	minn=2000000;
	xmin=xmax=ymin=ymax=0;
	xlog=ylog=0;
}

void Graphbuf::freebuf(void)
{
	int i;
	for(i=nplot-1;i>=0;i--) {
		if(dataname[i]) free(dataname[i]);
		if(pdat[i])     free(pdat[i]);
	}
	if(yunit) free(yunit);
	if(title) free(title);
	setnull();
}


// destructor
Graphbuf::~Graphbuf(void)
{
	freebuf();
}


/**************************************************************************
* operator overloading
**************************************************************************/
Graphbuf& Graphbuf::operator =(Graphbuf& a)
{
	int i;
	if(G__GRAPHBUF_FIXED==status) {
		freebuf();
	}
	if(title) free(title);
	if(a.Title()) {
		title=malloc(strlen(a.Title())+1);
		strcpy(title,a.Title());
	}
	else title=NULL;

	if(yunit) free(yunit);
	if(a.Yunit()) {
		yunit=malloc(strlen(a.Yunit())+1);
		strcpy(yunit,a.Yunit());
	}
	else yunit=NULL;
	
	minn=a.Minn();
	xlog=a.Xlog();
	ylog=a.Ylog();
	xmin=a.Xmin();
	xmax=a.Xmax();
	ymin=a.Ymin();
	ymax=a.Ymax();
	nplot=a.Nplot();
	pout=a.Pout();
	status=a.Status();

	for(i=0;i<nplot;i++) {
		n[i]=a.Npoint(i);
		iden[i]=a.Iden(i);

		if(pdat[i]) free(pdat[i]);
		pdat[i] = malloc(n[i]*sizeof(double));
		memcpy((char*)pdat[i],(char*)a.pdat[i],n[i]*sizeof(double));

		if(dataname[i]) free(dataname);
		if(a.Name(i)) {
			dataname[i]=malloc(strlen(a.Name(i))+1);
			strcpy(dataname[i],a.Name(i));
		}
		else dataname[i]=NULL;
	}
	return(*this);
}


// add DVector or DCVector to plot
Graphbuf& Graphbuf::operator <<(DVector& a)
{
	if(G__GRAPHBUF_FIXED==status) {
		freebuf();
	}
	if(pdat[nplot]) free(pdat[nplot]);
	pdat[nplot] = malloc(a.size()*sizeof(double));
	memcpy((char*)pdat[nplot],(char*)&a[0],a.size()*sizeof(double));
	iden[nplot]=G__DVECTORTYPE;
	n[nplot]=a.size();
	if(minn>a.size()) minn=a.size();
	++nplot;
	return(*this);
}

Graphbuf& Graphbuf::operator <<(DCVector& a)
{
	if(G__GRAPHBUF_FIXED==status) {
		freebuf();
	}
	// real part
	if(pdat[nplot]) free(pdat[nplot]);
	pdat[nplot] = malloc(a.size()*sizeof(double));
	memcpy((char*)pdat[nplot],(char*)a.re,a.size()*sizeof(double));
	iden[nplot]=G__DCVECTORTYPE_RE;
	n[nplot]=a.size();
	if(minn>a.size()) minn=a.size();
	++nplot;
	// imaginary part
	if(pdat[nplot]) free(pdat[nplot]);
	pdat[nplot] = malloc(a.size()*sizeof(double));
	memcpy((char*)pdat[nplot],(char*)a.im,a.size()*sizeof(double));
	iden[nplot]=G__DCVECTORTYPE_IM;
	n[nplot]=a.size();
	++nplot;
	return(*this);
}


// add min scale information
Graphbuf& Graphbuf::operator <<(double min)
{
	if(1==nplot) { // on x data
		xmin=min;
	}
	else { // on y data
		ymin=min;
	}
	return(*this);
}

// add max scale information
Graphbuf& Graphbuf::operator |(double max)
{
	if(G__GRAPHBUF_PUSHING==status) {
		if(1==nplot) { // on x data
			xmax=max;
		}
		else { // on y data
			ymax=max;
		}
	}
	return(*this);
}

// add log information
Graphbuf& Graphbuf::operator <<(ISLOG log)
{
	if(1==nplot) xlog = (int)log;
	else         ylog = (int)log;
	return(*this);
}

// add title of plot
Graphbuf& Graphbuf::operator <<(char *s)
{
	if(G__GRAPHBUF_FIXED==status) {
		freebuf();
	}
	if(strcmp(s,"\n")==0) {
		*this << '\n';
	}
	else if(strlen(s)>0 && s[strlen(s)-1]=='\n') {
		if(yunit) free(yunit);
		yunit=malloc(strlen(s)+1);
		sprintf(yunit,"%s",s);
		yunit[strlen(s)-1]='\0';
		*this << '\n';
	}
	else {
		if(nplot>0) {
			if(G__DVECTORTYPE==iden[nplot-1]) {
				if(dataname[nplot-1]) free(dataname[nplot-1]);
				dataname[nplot-1]=malloc(strlen(s)+1);
				strcpy(dataname[nplot-1],s);
			}
			else {
				if(dataname[nplot-2]) free(dataname[nplot-2]);
				dataname[nplot-2]=malloc(strlen(s)+5);
				sprintf(dataname[nplot-2],"%s(re)",s);
				if(dataname[nplot-1]) free(dataname[nplot-1]);
				dataname[nplot-1]=malloc(strlen(s)+5);
				sprintf(dataname[nplot-1],"%s(im)",s);
			}
		}
		else  {
			if(title) free(title);
			title=malloc(strlen(s)+1);
			strcpy(title,s);
		}
	}
	return(*this);
}

// do plot or print
Graphbuf& Graphbuf::operator <<(char c)
{
	status=G__GRAPHBUF_FIXED;
	return(*this);
}


/**********************************************************
* output
**********************************************************/

// add DVector or DCVector 
Graphbuf& Graphbuf::operator >>(DVector& a)
{
	if(pout>nplot) {
		cerr << "Error: no more data in Graphbuf\n";
		return(*this);
	}
	a = DVector(pdat[pout],n[pout]);
	++pout;
	return(*this);
}

Graphbuf& Graphbuf::operator >>(DCVector& a)
{
	if(pout>nplot-1) {
		cerr << "Error: no more data in Graphbuf\n";
		return(*this);
	}
	a = DCVector(pdat[pout],pdat[pout+1],n[pout]);
	pout+=2;
	return(*this);
}

// dummy 
Graphbuf& Graphbuf::operator >>(double d)
{
	return(*this);
}

#ifdef NEVER
Graphbuf& Graphbuf::operator |(double d)
{
	return(*this);
}
#endif

Graphbuf& Graphbuf::operator >>(ISLOG log)
{
	return(*this);
}

// reset ouput pointer
Graphbuf& Graphbuf::operator >>(char *s)
{
	pout=0;
	return(*this);
}

Graphbuf& Graphbuf::operator >>(char c)
{
	pout=0;
	return(*this);
}


/********************************************************************
*  binary dump
********************************************************************/
void Graphbuf::dumpdata(FILE *fp)
{
	int i;
	int len;

	fwrite(&nplot,sizeof(nplot),1,fp);
	fwrite(&xlog,sizeof(xlog),1,fp);
	fwrite(&ylog,sizeof(ylog),1,fp);
	fwrite(&xmin,sizeof(xmin),1,fp);
	fwrite(&xmax,sizeof(xmax),1,fp);
	fwrite(&ymin,sizeof(ymin),1,fp);
	fwrite(&ymax,sizeof(ymax),1,fp);
	fwrite(&minn,sizeof(minn),1,fp);
	fwrite(&pout,sizeof(pout),1,fp);
	fwrite(&status,sizeof(status),1,fp);
	fwrite(&n,sizeof(n),1,fp);
	fwrite(&iden,sizeof(iden),1,fp);

	if(title) len=strlen(title);
	else      len=0;
	fwrite(&len,sizeof(len),1,fp);
	if(len) fwrite(title,len+1,1,fp);
	

	if(yunit) len=strlen(yunit);
	else      len=0;
	fwrite(&len,sizeof(len),1,fp);
	if(len) fwrite(yunit,len+1,1,fp);

	for(i=0;i<nplot;i++) {
		fwrite(pdat[i],n[i]*sizeof(double),1,fp);

		if(dataname[i]) len=strlen(dataname[i]);
		else            len=0;
		fwrite(&len,sizeof(len),1,fp);
		if(len) fwrite(dataname[i],len+1,1,fp);
	}
}


/********************************************************************
*  binary load
********************************************************************/
void Graphbuf::loaddata(FILE *fp)
{
	int i;
	int len;

	fread(&nplot,sizeof(nplot),1,fp);
	fread(&xlog,sizeof(xlog),1,fp);
	fread(&ylog,sizeof(ylog),1,fp);
	fread(&xmin,sizeof(xmin),1,fp);
	fread(&xmax,sizeof(xmax),1,fp);
	fread(&ymin,sizeof(ymin),1,fp);
	fread(&ymax,sizeof(ymax),1,fp);
	fread(&minn,sizeof(minn),1,fp);
	fread(&pout,sizeof(pout),1,fp);
	fread(&status,sizeof(status),1,fp);
	fread(&n,sizeof(n),1,fp);
	fread(&iden,sizeof(iden),1,fp);

	fread(&len,sizeof(len),1,fp);
	if(title) {
		free(title);
		title=NULL;
	}
	if(len) {
		title=malloc(len+1);
		fread(title,len+1,1,fp);
	}

	fread(&len,sizeof(len),1,fp);
	if(yunit) {
		free(yunit);
		yunit=NULL;
	}
	if(len) {
		yunit=malloc(len+1);
		fread(yunit,len+1,1,fp);
	}

	for(i=0;i<nplot;i++) {
		if(pdat[i]) free(pdat[i]);
		pdat[i]=malloc(n[i]*sizeof(double));
		fread(pdat[i],n[i]*sizeof(double),1,fp);

		fread(&len,sizeof(len),1,fp);
		if(dataname[i]) {
			free(dataname[i]);
			dataname[i]=NULL;
		}
		if(len) {
			dataname[i]=malloc(len+1);
			fread(dataname[i],len+1,1,fp);
		}
	}
}

#endif


/***************************************************************************
* Vectoriostream.h
*
*  DVector and DCVector type graph plot and file I/O class
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
#ifndef G__VECTORIOSTREAM
#define G__VECTORIOSTREAM

#define CINTPLOT

#ifdef G__ROOT
#include <rootgraph.cxx>
#endif

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

/**********************************************************
* Vectorostream
**********************************************************/
class Vectorostream {
  // Due to cint limitation, base class inheritance is emulated by
  // having pointer of Graphbuf and construct it by new operator.
  Graphbuf *buf;  
  
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
  Vectorostream(FILE *fp_init,int iscsv=0);
  Vectorostream(char *filename,int iscsv=0);
  ~Vectorostream();
  
  void init(FILE *fp_init)
    {if(fp&&fp!=stdout&&fp!=stderr) fclose(fp); fp=fp_init;}
  void init(char fname)
    {if(fp&&fp!=stdout&&fp!=stderr) fclose(fp);fp=fopen(fname,"w");}
  void close() 
    {if(fp&&fp!=stdout&&fp!=stderr) fclose(fp); fp=NULL;}
  
  // ostream pipeline operator
  Vectorostream& operator <<(const DVector& a);
  Vectorostream& operator <<(const DCVector& a);
  Vectorostream& operator <<(char *s);    // give name or title+do plot
  Vectorostream& operator <<(double min); // specify min scale
  Vectorostream& operator  |(double max); // specify max scale
  Vectorostream& operator <<(int min); // specify min scale
  Vectorostream& operator >>(int max); // specify max scale
  Vectorostream& operator <<(ISLOG log);  // specify log scale
  Vectorostream& operator <<(UNITCIRCLE_t uc);  // specify log scale
  Vectorostream& operator <<(char c);     // do plot
} ;


// constructor

Vectorostream::Vectorostream(FILE *fp_init,int iscsvin)
{
  buf = new Graphbuf; // base class emulation
  filen=0;
  fp=fp_init;
  importedfp=1;
  unitcircle=0;
  iscsv = iscsvin;
  isflushed=0;
}

Vectorostream::Vectorostream(char *filename,int iscsvin)
{
  buf = new Graphbuf; // base class emulation
  filen=0;
  fp=fopen(filename,"wb");
  if(NULL==fp) cerr << filename << " could not open\n" ;
  importedfp=0;
  unitcircle=0;
  iscsv = iscsvin;
  isflushed=0;
}

Vectorostream::~Vectorostream()
{
  if(0==importedfp && fp && stdout!=fp && stderr!=fp) fclose(fp);
  delete buf; // base class emulation
}


//
Vectorostream& Vectorostream::operator <<(Graphbuf& a)
{
  *buf = a;
  if(buf->isterminate()) {
    terminated();
  }
  return(*this);
}

// add Vector or cVector to plot
Vectorostream& Vectorostream::operator <<(const DVector& a)
{
  *buf << a;
  return(*this);
}

Vectorostream& Vectorostream::operator <<(const DCVector& a)
{
  *buf << a;
  return(*this);
}


// add min scale information
Vectorostream& Vectorostream::operator <<(double min)
{
  *buf << min;
  return(*this);
}

// add max scale information
Vectorostream& Vectorostream::operator |(double max)
{
  *buf | max;
  return(*this);
}
// add min scale information
Vectorostream& Vectorostream::operator <<(int min)
{
  *buf << (double)min;
  return(*this);
}

// add max scale information
Vectorostream& Vectorostream::operator >>(int max)
{
  *buf | (double)max;
  return(*this);
}
Vectorostream& Vectorostream::operator >>(double max)
{
  *buf | max;
  return(*this);
}

// add log information
Vectorostream& Vectorostream::operator <<(ISLOG log)
{
  *buf << log;
  return(*this);
}

// add unit circle information
Vectorostream& Vectorostream::operator <<(UNITCIRCLE_t uc)
{
  unitcircle=1;
  return(*this);
}

// add title of plot
Vectorostream& Vectorostream::operator <<(char *s)
{
  *buf << s;
  if(buf->isterminate()) {
    terminated();
  }
  return(*this);
}

// do plot or print
Vectorostream& Vectorostream::operator <<(char c)
{
  *buf << c;
  terminated();
  return(*this);
}
Vectorostream& Vectorostream::operator <<(G__CINT_ENDL c)
{
  *buf << '\n';
  terminated();
  return(*this);
}

Vectorostream::terminated(void)
{
  int i;
  if(iscsv==1) {
    csvdata();
  }
  else if(iscsv>=2) {
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
Vectorostream& Vectorostream::operator <<(G__CINT_FLUSH c)
{
  *buf << '\n';
  flushed();
  return(*this);
}

void Vectorostream::flushed(void) {
  if(iscsv==1) {
    //csvdata();
  }
  else if(iscsv>=2) {
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

void Vectorostream::rootgraph_flushed(int mode) {
  int i,nplot,n,n;
  char fname[100];

  nplot = buf->Nplot();
  switch(nplot) {
  case 0:
    cerr << "No data to plot\n";
    break;
  case 1:
    n=buf->Npoint(0);
    Vector tmpx=Vector(0,n-1,n);
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
void Vectorostream::rootgraph(int mode)
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
    Vector tmpx=Vector(0,n-1,n);
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
void Vectorostream::plotdata(void)
{
  int i,nplot,n,n;
  char fname[100];
  
  nplot = buf->Nplot();

  switch(nplot) {
  case 0:
    cerr << "No data to plot\n";
    break;
  case 1:
    n=buf->Npoint(0);
    Vector tmpx=Vector(0,n-1,n);
    sprintf(fname,"G__graph%d",filen++);
    if(!isflushed) rootgraph_open(fname,buf->Title(),mode);
    else rootgraph_open(fname,buf->Title(),4);
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
    break;
  default:
    sprintf(fname,"G__graph%d",filen++);
    //xgraph_open(fname,buf->Title());
    if(!isflushed) rootgraph_open(fname,buf->Title(),mode);
    else rootgraph_open(fname,buf->Title(),4);
    for(i=1;i<nplot;i++) {
      if(buf->Npoint(0)>buf->Npoint(i))  n = buf->Npoint(i);
      else                               n = buf->Npoint(0);
      xgraph_data(fname
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
      xgraph_data(fname
		  ,x
		  ,y
		  ,nuc
		  ,"unit_circle"
		  );
    }
    xgraph_invoke(fname
		  ,buf->Xmin(),buf->Xmax()
		  ,buf->Ymin(),buf->Ymax()
		  ,buf->Xlog(),buf->Ylog()
		  ,buf->Name(0),buf->Yunit());
    break;
  }
}

// print data to file
void Vectorostream::printdata(void)
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
void Vectorostream::csvdata(void)
{
  int i,nplot,n,n;
  int nplot = buf->Nplot();
  int n=buf->Npoint(0);
  double *p;
  if(!fp) fp=stdout;
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
}


/**********************************************************
* istream
**********************************************************/
class Vectoristream {
  // data member
  double *dat[G__VECTORMAX];
  int ndat;
  int nplot;
  FILE *fp;
  int importedfp;
  
 public:
  // constructor
  Vectoristream(FILE *fp_init);
  Vectoristream(char *filename);
  ~Vectoristream();
  
  // operator overloading
  Vectoristream& operator >>(const DVector& a);
  Vectoristream& operator >>(const DCVector& a);
  Vectoristream& operator >>(char *s);
  Vectoristream& operator >>(char c);
  Vectoristream& operator >>(double d);  // dummy
  Vectoristream& operator >>(ISLOG log); // dummy
  Vectoristream& operator >>(Graphbuf& a);
  
  void close(void);
} ;


Vectoristream::Vectoristream(FILE *fp_init) 
{ 
  fp=fp_init;
  nplot=0; 
  ndat=2000000; 
  importedfp=1;
}

Vectoristream::Vectoristream(char *filename)
{ 
  fp=fopen(filename,"rb");
  if(NULL==fp) cerr << filename << " could not open\n" ;
  nplot=0; 
  ndat=2000000; 
  importedfp=0;
}

Vectoristream::~Vectoristream()
{
  close();
}

void Vectoristream::close(void)
{
  if(0==importedfp) fclose(fp);
}


Vectoristream& Vectoristream::operator >>(Graphbuf& a)
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

// add Vector or cVector 
Vectoristream& Vectoristream::operator >>(const DVector& a)
{
  dat[nplot++] = &a[0];
  if(ndat>a.size()) ndat=a.size();
  return(*this);
}

Vectoristream& Vectoristream::operator >>(const DCVector& a)
{
  dat[nplot++] = a.re;
  dat[nplot++] = a.im;
  if(ndat>a.size()) ndat=a.size();
  return(*this);
}

// dummy 
Vectoristream& Vectoristream::operator >>(double d)
{
  return(*this);
}

Vectoristream& Vectoristream::operator |(double d)
{
  return(*this);
}

Vectoristream& Vectoristream::operator >>(ISLOG log)
{
  return(*this);
}

// do input
Vectoristream& Vectoristream::operator >>(char *s)
{
  if(s[strlen(s)-1]=='\n') 
    *this >> '\n';
  return(*this);
}

Vectoristream& Vectoristream::operator >>(char c)
{
  int i,n;
  if(NULL==fp) {
    cerr << "Vectoristream: File pointer null\n" ;
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
Vectorostream plot=Vectorostream((FILE*)NULL,2);
Vectorostream same=Vectorostream((FILE*)NULL,3);
#else
Vectorostream plot=Vectorostream((FILE*)NULL);
#endif
Vectorostream Vectorcout=Vectorostream(stdout);
Vectorostream Vectorcerr=Vectorostream(stderr);
Vectoristream Vectorcin=Vectoristream(stdin);
Vectorostream csv=Vectorostream(stdout,1);



///////////////////////////////////////////////////////////////////////////

#endif




