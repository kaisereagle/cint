/***************************************************************************
* VectoriostreamNN.h
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
*     	     >>  x2 << y4 
*            >>  x3 << y5 
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
#else
#ifndef RG_PLOTMODE
#define RG_MASK_PLOTMODE  0xf
#define RG_PLOTMODE       0x2
#define RG_SAMEMODE       0x4
#endif
#endif

#include <VirtualGraph.h>
#include <ReadF.h>

#include <string>

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

typedef unsigned char ISLOG;
ISLOG LOG=1;
ISLOG LIN=0;

/**********************************************************
* Vectorostream
**********************************************************/
class Vectorostream {
 public:
  // constructor
  Vectorostream(int mode=RG_PLOTMODE) { init(); m_mode=mode; }
  ~Vectorostream() { clear(); }
  
  // ostream pipeline operator
  Vectorostream& operator <<(const vector<double>& a) {
    if(m_clear) clear();
    if(0==m_x.size()) m_x.push_back(a);
    else if(m_x.size()>m_y.size()) m_y.push_back(a);
    else {  
      m_x.push_back(m_x.back());
      m_y.push_back(a);
    }
    return(*this);
  }
  Vectorostream& operator <<(const DVector& a) {
    if(m_clear) clear();
    if(0==m_x.size()) m_x.push_back(a);
    else if(m_x.size()>m_y.size()) m_y.push_back(a);
    else {  
      m_x.push_back(m_x.back());
      m_y.push_back(a);
    }
    return(*this);
  }
  Vectorostream& operator <<(const DCVector& a) {
    if(m_clear) clear();
    DVector x,y;
    for(int i=0;i<a.size();i++) {
      x.push_back(a[i].real());
      y.push_back(a[i].imag());
    }
    if(0==m_x.size()) {
      m_x.push_back(x);
      m_y.push_back(y);
    }
    else if(m_x.size()>m_y.size()) {
      m_x.push_back(m_x.back());
      m_y.push_back(x);
      m_y.push_back(y);
    }
    else {
      m_x.push_back(m_x.back());
      m_x.push_back(m_x.back());
      m_y.push_back(x);
      m_y.push_back(y);
    }
    return(*this);
  }
  Vectorostream& operator >> (const vector<double>& a) {
    if(m_clear) clear();
    m_x.push_back(a);
    return(*this);
  }
  Vectorostream& operator >> (const DVector& a) {
    if(m_clear) clear();
    m_x.push_back(a);
    return(*this);
  }
  Vectorostream& operator >> (const DCVector& a) {
    if(m_clear) clear();
    DVector x,y;
    for(int i=0;i<a.size();i++) {
      x.push_back(a[i].real());
      y.push_back(a[i].imag());
    }
    m_x.push_back(x);
    m_y.push_back(y);
    return(*this);
  }
  Vectorostream& operator <<(const string& s) {
    if(m_clear) clear();
    char buf[80]; strcpy(buf,s.c_str());
    char *p = strchr(buf,'\n');
    if(p) {
      if(p>buf) {
        *p = 0;
        m_yunit = buf;
      }
      plotdata();
    }
    if(m_x.size()==0) m_title = s;
    else if(m_x.size()>m_y.size()) m_xunit = s;
    else                      m_yname.push_back(s);
    return(*this);
  }
  Vectorostream& operator <<(double min) {
    if(m_clear) clear();
    if(0==m_x.size())    ;
    if(m_x.size()>m_y.size()) m_xmin = min;
    else                      m_ymin = min;
    return(*this);
  }
  Vectorostream& operator >> (double max) {
    if(m_clear) clear();
    if(0==m_x.size())    ;
    if(m_x.size()>m_y.size()) m_xmax = max;
    else                      m_ymax = max;
    return(*this);
  }
  Vectorostream& operator <<(int min) {
    if(m_clear) clear();
    if(0==m_x.size())    ;
    if(m_x.size()>m_y.size()) m_xmin = min;
    else                      m_ymin = min;
    return(*this);
  }
  Vectorostream& operator >>(int max) {
    if(m_clear) clear();
    if(0==m_x.size())    ;
    if(m_x.size()>m_y.size()) m_xmax = max;
    else                      m_ymax = max;
    return(*this);
  }
  Vectorostream& operator <<(ISLOG log) { 
    if(m_clear) clear();
    if(0==m_x.size())    ;
    if(m_x.size()>m_y.size()) m_xlog = log;
    else                      m_ylog = log;
    return(*this);
  }
  Vectorostream& operator <<(UNITCIRCLE_t uc) { 
    if(m_clear) clear();
    m_unitcircle = uc; 
    return(*this);
  }
  Vectorostream& operator <<(char c) {
    plotdata();
    return(*this);
  }
  Vectorostream& operator <<(G__CINT_ENDL c) {
    plotdata();
    return(*this);
  }

  void init() {
    clear();
  }
  void clear() {
    m_x.clear();
    m_y.clear();
    m_title = "";
    m_xunit = "";
    m_yunit = "";
    m_yname.clear();
    m_xmin = m_xmax = m_ymin = m_ymax = 0.0;
    m_xlog = m_ylog = m_unitcircle = 0;
    m_clear=0;
  }

 private:
  // private member function
  void plotdata() {
    if(m_mode & RG_MASK_PLOTMODE) {
#ifdef G__ROOT
      rootgraph_open("",m_title.c_str(),m_mode);
      for(int i=0;i<m_x.size()&&i<m_y.size();i++) {
        rootgraph_data("",&m_x[i][0] ,&m_y[i][0]
		,( m_x[i].size()<=m_y[i].size()?m_x[i].size():m_y[i].size() )
		,( m_yname.size()>i?m_yname[i].c_str():"" )
		);
      }
      if(m_unitcircle) unitcircle();
      rootgraph_invoke(""
		  ,m_xmin,m_xmax
		  ,m_ymin,m_ymax
		  ,m_xlog,m_ylog
		  ,m_xunit.c_str(),m_yunit.c_str(),m_mode);
#else
      fprintf(stderr,"*** Warning: No graphic driver in CINT. Ignore X-Y graph drawing ***\n");
#endif
    }
    else if(m_mode & VG_MASK_PLOTMODE) {
      virtualgraph_invoke(*this);
    }
    //clear();
    m_clear = 1;
  }
  void unitcircle() {
    int nuc = 100;
    double dpi = 3.141592*2/100;
    double x[100],y[100];
    for(i=0;i<nuc;i++) {
      x[i] = cos(dpi*i);
      y[i] = sin(dpi*i);
    }
    rootgraph_data("" ,x ,y ,nuc ,"unit_circle");
  }

 public:
  ///////////////////////////////////////////////////////////////////////
  void saveNNInput(const char* fname=0,const char* mode="w") {
    string sfname;
    if(fname) sfname=fname;
    else if(m_title.size()) sfname=m_title+"_NN.csv";
    if(m_mode & VG_MASK_PLOTMODE) {
      virtualgraph_back().disp(sfname.c_str(),mode);
    }
  }

  ///////////////////////////////////////////////////////////////////////
  void saveRawData(const char* fname=0,const char* mode="w") {
    string sfname;
    if(fname) sfname = fname;
    else if(m_title.size()) sfname = m_title + "_RAW.csv";
    FILE* fp = fopen(sfname.c_str(),mode);
    if(!fp) {
      fprintf(stderr
       ,"Error: Vectoriostream::saveRawData() Failed opening output file %s\n"
       ,sfname.c_str());
      return;
    }
    saveRawData(fp);
    fclose(fp);
  }

  void saveRawData(FILE* fp) {
    fprintf(fp,"BEGIN RAWDATA\n");
    fprintf(fp,"TITLE:,%s,",m_title.c_str());
    fprintf(fp,"UNIT:,%s,%s,",m_xunit.c_str(),m_yunit.c_str());
    fprintf(fp,"SCALE:,%g,%g,%g,%g,",m_xmin,m_xmax,m_ymin,m_ymax);
    fprintf(fp,"LOG:,%d,%d,",m_xlog,m_ylog);
    fprintf(fp,"\n");

    int i,j=0,count=1;
    for(i=0;i<m_yname.size();i++) 
      fprintf(fp,"%s,%s,",m_xunit.c_str(),m_yname[i].c_str());
    fprintf(fp,"\n");
    while(count) {
      count=0;
      for(i=0;i<m_x.size()&&i<m_y.size();i++) {
        if(j<m_x[i].size()) {fprintf(fp,"%g,",m_x[i][j]);++count;}
        else                {fprintf(fp,",");}
        if(j<m_y[i].size()) {fprintf(fp,"%g,",m_y[i][j]);++count;}
        else                {fprintf(fp,",");}
      }
      ++j;
      fprintf(fp,"\n");
    }

    fprintf(fp,"END RAWDATA\n");
  }

  ///////////////////////////////////////////////////////////////////////
  void saveNNDB(const char* fname=0,const char* mode="w") {
    string sfnamebase,sfname;
    if(fname) sfnamebase = fname;
    else if(m_title.size()) sfnamebase = m_title ;
    sfname = sfnamebase;
    if(sfname.find(".csv")==string::npos) sfname += "_NNDB.csv";

    FILE *fp = fopen(sfname.c_str(),mode);
    if(!fp) {
      fprintf(stderr
       ,"Error: Vectoriostream::saveNNDB() Failed opening output file %s\n"
       ,sfname.c_str());
      return;
    }
    virtualgraph_back().disp(fp);
    fprintf(fp,"END NN-INPUT\n"); // put this in upper structure
    saveRawData(fp);
    fclose(fp);
  }

  ///////////////////////////////////////////////////////////////////////
  void loadNNDB(const char* fname=0,const char* mode="r") {
    clear();
    string sfname;
    if(fname) sfname = fname;
    else if(m_title.size()) sfname = m_title + "_NNDB.csv";

    ReadF f(sfname.c_str());
    f.setseparator("");
    f.setdelimiter(",");
    while(f.read()) {
      if(f.argc>=1 && strncmp(f.argv[1],"BEGIN RAWDATA",8)==0) goto READRD;
    }
    fprintf(stderr
       ,"Error: Vectoriostream::loadRawData() No raw data found in %s\n",
       ,sfname.c_str());
    return;

   READRD: 
    int i=1;
    // Read Header
    f.read();
    m_title=f.argv[i]; i+=2;
    m_xunit=f.argv[i]; ++i;
    m_yunit=f.argv[i]; i+=2;
    m_xmin=atof(f.argv[i]); ++i;
    m_xmax=atof(f.argv[i]); ++i;
    m_ymin=atof(f.argv[i]); ++i;
    m_ymax=atof(f.argv[i]); i+=2;
    m_xlog=atoi(f.argv[i]); ++i;
    m_ylog=atoi(f.argv[i]); 

    // Read yname
    f.read();
    i=1;
    while(i<f.argv.size()) {
      m_yname.push_back(f.argv[i]); i+=2;
    }
    DVector tmp;
    for(i=0;i<m_yname.size();i++) {
      m_x.push_back(tmp);
      m_y.push_back(tmp);
    }

    // Read Data
    int count = 1;
    char *p;
    int j;
    while(count) {
      f.read();
      if(strncmp(f.argv[1],"END",3)==0) {
        break;
      }
      count=0;
      for(i=1;i<f.argv.size();i+=2) {
        j=(i-1)/2;
        p = f.argv[i];
        if(p[0]) {
          ++count;
          m_x[j].push_back(atof(p));
        }
        p = f.argv[i+1];
        if(p[0]) {
          ++count;
          m_y[j].push_back(atof(p));
        }
      }
    }
  }

  ///////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////

  //void rootgraph(void);
  //void printdata(void);
  //void csvdata(void);

  friend int virtualgraph_invoke(Vectorostream& vs);
 private:
  // Due to cint limitation, base class inheritance is emulated by
  // having pointer of Graphbuf and construct it by new operator.
  DMatrix   m_x;
  DMatrix   m_y;
  string    m_title;
  string    m_xunit;
  string    m_yunit;
  vector<string> m_yname;
  int m_xlog,m_ylog;            // log flag
  double m_xmin,m_xmax,m_ymin,m_ymax;   // scale
  
  // data member
  int m_unitcircle;               // unit circle mode
  int m_mode;
  
  int m_clear;
} ;


// global object
#ifdef G__ROOT
Vectorostream plot=Vectorostream(RG_PLOTMODE);
Vectorostream same=Vectorostream(RG_SAMEMODE);
#else
Vectorostream plot=Vectorostream(2);
#endif

#ifdef VIRTUALGRAPH_H
Vectorostream plotNN=Vectorostream(VG_PLOTMODE);
Vectorostream sameNN=Vectorostream(VG_SAMEMODE);
#endif

//Vectorostream Vectorcout=Vectorostream(stdout);
//Vectorostream Vectorcerr=Vectorostream(stderr);
//Vectoristream Vectorcin=Vectoristream(stdin);
//Vectorostream csv=Vectorostream(stdout,1);


///////////////////////////////////////////////////////////////////////////

#endif




