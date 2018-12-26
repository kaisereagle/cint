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
*     	     >>  x2 << y4 
*            >>  x3 << y5 
*     	     << "yunit\n" ;              // must end with \n
*
*       fout << x << y1 << y2 << y3 << "\n";
*       fin  >> x >> y1 >> y2 >> y3 >> "\n";
*
****************************************************************************/
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
#ifndef G__VECTORIOSTREAM
#define G__VECTORIOSTREAM

#define CINTPLOT
#define FILENAME

#ifdef G__ROOT
#include <rootgraph.cxx>
#else // G__ROOT

#ifndef RG_PLOTMODE
#define RG_MASK_PLOTMODE  0xf
#define RG_PLOTMODE       0x2
#define RG_SAMEMODE       0x4
#endif
#include "cintgraph.h"

#endif // G__ROOT

#include "VirtualGraph.h"
#ifndef EXCEL_H
#include "ReadF.h"
#endif

#include <string>

#ifndef NEURALNET_H
typedef float NN_REAL;
#endif

/**********************************************************
* Support object and class definitions
**********************************************************/
typedef unsigned short UNITCIRCLE_t;
extern UNITCIRCLE_t UNITCIRCLE;

typedef unsigned char ISLOG;
extern ISLOG LOG;
extern ISLOG LIN;

class G__CINT_ENDL { int dmy; } ;
class G__CINT_ENDS { int dmy; } ;
class G__CINT_FLUSH { int dmy; };
class G__CINT_ws { int dmy; } ;
class G__CINT_WS { int dmy; } ;
class G__CINT_HEX { int dmy; } ;
class G__CINT_DEC { int dmy; } ;
class G__CINT_OCT { int dmy; } ;
class G__CINT_SCIENTIFIC { int dmy; } ;
class G__CINT_FIXED { int dmy; } ;
class G__CINT_NOSUPPORT { int dmy; } ;

#if !defined(_WIN32) && !defined(G__WIN32)
extern G__CINT_ENDL    endl;
extern G__CINT_ENDS    ends;
extern G__CINT_FLUSH   flush;
extern G__CINT_ws      ws;
extern G__CINT_WS      WS;
extern G__CINT_HEX     hex;
extern G__CINT_DEC     dec;
extern G__CINT_OCT     oct;
extern G__CINT_SCIENTIFIC  scientific;
extern G__CINT_FIXED       fixed;
#endif

#ifdef _WIN32
#include <iostream>
using namespace std;
inline ostream& operator<<(ostream& os,const G__CINT_ENDL& el) {return(os);}
#endif

/**********************************************************
* color
**********************************************************/
class COLOR {
 public:
  COLOR(int color) { m_color=color; }
  int m_color;
};

/**********************************************************
***********************************************************
* Vectorostream
***********************************************************
**********************************************************/
class Vectorostream {
 public:
  // constructor
  Vectorostream(int mode=RG_PLOTMODE) { init(); m_mode=mode; }
  ~Vectorostream() { clear(); }

  // Color assignment
  Vectorostream& operator <<(const COLOR& a) {
    m_color.push_back(a.m_color);
    return(*this);
  }
  
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
#ifdef G__COMPLEX
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
#endif
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
#ifdef G__COMPLEX
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
#endif
  Vectorostream& operator <<(const string& s) {
    if(m_clear) clear();
    char buf[1000]; strcpy(buf,s.c_str());
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
    if(0==m_x.size()) {  }
    if(m_x.size()>m_y.size()) m_xmin = min;
    else                      m_ymin = min;
    return(*this);
  }
  Vectorostream& operator >> (double max) {
    if(m_clear) clear();
    if(0==m_x.size()) { }
    if(m_x.size()>m_y.size()) m_xmax = max;
    else                      m_ymax = max;
    return(*this);
  }
  Vectorostream& operator <<(int min) {
    if(m_clear) clear();
    if(0==m_x.size())  { }
    if(m_x.size()>m_y.size()) m_xmin = min;
    else                      m_ymin = min;
    return(*this);
  }
  Vectorostream& operator >>(int max) {
    if(m_clear) clear();
    if(0==m_x.size())   { }
    if(m_x.size()>m_y.size()) m_xmax = max;
    else                      m_ymax = max;
    return(*this);
  }
  Vectorostream& operator <<(ISLOG log) { 
    if(m_clear) clear();
    if(0==m_x.size())  { }
    if(m_x.size()>m_y.size()) m_xlog = log;
    else                      m_ylog = log;
    return(*this);
  }
  Vectorostream& operator <<(UNITCIRCLE_t uc) { 
    if(m_clear) clear();
    m_unitcircle = uc; 
    return(*this);
  }
  Vectorostream& operator <<(const char c) {
    plotdata();
    return(*this);
  }
  Vectorostream& operator <<(const G__CINT_ENDL c) {
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
#if 0
#ifndef G__ROOT
    if(0==(m_mode & VG_MASK_PLOTMODE)) {
      if(graph_mode()) cintgraph_clear();
    }
#endif
#endif
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
		,( m_color.size()>i?m_color[i]:-1 )
		);
      }
      if(m_unitcircle) unitcircle();
      rootgraph_invoke(""
		  ,m_xmin,m_xmax
		  ,m_ymin,m_ymax
		  ,m_xlog,m_ylog
		  ,m_xunit.c_str(),m_yunit.c_str(),m_mode);
#else
      //fprintf(stderr,"*** Warning: No graphic driver in CINT. Ignore X-Y graph drawing ***\n");
      static int ngr=0;
      char xgraphfile[1000];
#ifdef FILENAME
      sprintf(xgraphfile,"%s_g%d.csv",m_title.c_str(),ngr++);
#else
      sprintf(xgraphfile,"G__graph%d.csv",ngr++);
#endif
      if(graph_mode()) {
	cintgraph_open(xgraphfile,m_title.c_str());
	//double *mx;
	//double *my;
	for(int i=0;i<m_x.size()&&i<m_y.size();i++) {
	  cintgraph_data(xgraphfile,m_x,m_y,i
			 ,( m_x[i].size()<=m_y[i].size()?m_x[i].size():m_y[i].size() )
			 ,( m_yname.size()>i?m_yname[i].c_str():"" )
			 ,( m_color.size()>i?m_color[i]:-1 )
			 );
	}
	if(m_unitcircle) unitcircle();
	cintgraph_invoke(xgraphfile
			 ,m_xmin,m_xmax
			 ,m_ymin,m_ymax
			 ,m_xlog,m_ylog
			 ,m_xunit.c_str(),m_yunit.c_str());
      }
      else {
	gnuplot_open(xgraphfile,m_title.c_str()
		     ,m_xunit.c_str(),m_yunit.c_str()
		     ,m_xmin,m_xmax
		     ,m_ymin,m_ymax
		     ,m_xlog,m_ylog);
	for(int i=0;i<m_x.size()&&i<m_y.size();i++) {
	  gnuplot_title(xgraphfile,i
			 ,( m_x[i].size()<=m_y[i].size()?m_x[i].size():m_y[i].size() )
			 ,( m_yname.size()>i?m_yname[i].c_str():"" )
			 ,( m_color.size()>i?m_color[i]:-1 )
			 );
	}
	for(int i=0;i<m_x.size()&&i<m_y.size();i++) {
	  printf("I=%d NDATA=\n",i);
	  gnuplot_data(xgraphfile,m_x,m_y,i
			 ,( m_x[i].size()<=m_y[i].size()?m_x[i].size():m_y[i].size() )
			 ,( m_yname.size()>i?m_yname[i].c_str():"" )
			 ,( m_color.size()>i?m_color[i]:-1 )
			 );
	}
	if(m_unitcircle) unitcircle();
	gnuplot_invoke(xgraphfile
			 ,m_xmin,m_xmax
			 ,m_ymin,m_ymax
			 ,m_xlog,m_ylog
			 ,m_xunit.c_str(),m_yunit.c_str());
      }
#endif
    }
    else if(m_mode & VG_MASK_PLOTMODE) {
      virtualgraph_invoke(*this);
      //virtualgraph_invoke(this);
    }
    //clear();
    m_clear = 1;
  }
  void unitcircle() {
    int i,nuc = 100;
    double dpi = 3.141592*2/100;
    double x[100],y[100];
    for(i=0;i<nuc;i++) {
      x[i] = cos(dpi*i);
      y[i] = sin(dpi*i);
    }
#ifdef G__ROOT
    rootgraph_data("" ,x ,y ,nuc ,"unit_circle");
#else
    if(graph_mode()) cintgraph_data("" ,x ,y ,nuc ,"unit_circle");
    else {
      //gnuplot_title("",0 ,nuc ,"unit_circle");
      //gnuplot_data("" ,x ,y ,nuc ,"unit_circle");
    }
#endif
  }

 public:
  ///////////////////////////////////////////////////////////////////////
#if 0
  void saveNNInput(const char* fname=0,const char* mode="w") {
    string sfname;
    if(fname) sfname=fname;
    else if(m_title.size()) sfname=m_title+"_NN.csv";
    if(m_mode & VG_MASK_PLOTMODE) {
      if(sfname.find(".csv")!=string::npos) 
        virtualgraph_back().disp(sfname.c_str(),mode);
      else 
        virtualgraph_back().savebin(sfname.c_str(),mode);
    }
  }
#endif

  ///////////////////////////////////////////////////////////////////////
  vector<NN_REAL>& getNNInput() {
    static vector<NN_REAL> dmy;
    if(m_mode & VG_MASK_PLOTMODE) {
      return(virtualgraph_NNInput());
    }
    return(dmy);
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
    int isbin=0;
    if(strchr(mode,'b')) isbin=1;
    if(isbin) {
      if(sfname.find(".bin")==string::npos) sfname += "_NNDB.bin";
    }
    else {
      if(sfname.find(".csv")==string::npos) sfname += "_NNDB.csv";
    }

#if 1
    if(isbin) 
      virtualgraph_back().savebin(sfname.c_str(),mode);
    else
      virtualgraph_back().disp(sfname.c_str(),mode);
#else
    FILE *fp = fopen(sfname.c_str(),mode);
    if(!fp) {
      fprintf(stderr
       ,"Error: Vectoriostream::saveNNDB() Failed opening output file %s\n"
       ,sfname.c_str());
      return;
    }
    if(isbin) {
      virtualgraph_back().savebin(fp);
    }
    else {
      virtualgraph_back().disp(fp);
      fprintf(fp,"END NN-INPUT\n"); // put this in upper structure
      saveRawData(fp);
    }
    fclose(fp);
#endif
  }

  ///////////////////////////////////////////////////////////////////////
  void loadNNDB(const char* fname=0,const char* mode="r") {
    clear();
    string sfname;
    int isbin=0;
    if(strchr(mode,'b')) isbin=1;
    if(fname) sfname = fname;
    else if(m_title.size()) {
      if(isbin) sfname = m_title + "_NNDB.bin";
      else      sfname = m_title + "_NNDB.csv";
    }

    ReadF f;
    if(isbin) {
    }
    else {
      f.open(sfname.c_str());
      f.setseparator("");
      f.setdelimiter(",");
      while(f.read()) {
        if(f.argc>=1 && strncmp(f.argv[1],"BEGIN RAWDATA",8)==0) goto READRD;
      }
      fprintf(stderr
         ,"Error: Vectoriostream::loadRawData() No raw data found in %s\n"
         ,sfname.c_str());
    }
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
  friend int virtualgraph_invoke(Vectorostream* vs) ;
 private:
  // Due to cint limitation, base class inheritance is emulated by
  // having pointer of Graphbuf and construct it by new operator.
  DMatrix   m_x;
  DMatrix   m_y;
  IVector   m_color;
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
extern Vectorostream plot;
extern Vectorostream same;
#else
extern Vectorostream plot;
#endif

#ifdef VIRTUALGRAPH_H
extern Vectorostream plotNN;
extern Vectorostream sameNN;
#endif

//Vectorostream Vectorcout=Vectorostream(stdout);
//Vectorostream Vectorcerr=Vectorostream(stderr);
//Vectoristream Vectorcin=Vectoristream(stdin);
//Vectorostream csv=Vectorostream(stdout,1);
/////////////////////////////////////////////////////////////////////////

/**********************************************************
***********************************************************
* Matrixostream
***********************************************************
**********************************************************/
class Matrixostream {
 public:
  Matrixostream(int mode=0) {
    m_mode = mode;
    init();
  }
  void init() {
    for(int i=0;i<3;i++) {
      m_min[i] = 0;
      m_max[i] = 0;
      m_axis[i] = -1;
      m_log[i] = 0;
    }
    m_dim=2;
    m_dir=1;
    m_hist=0;
    m_XY=0;
    clear();
  }
  void clear() {
    //for(int i=0;i<csv_CSVMAX;i++) m_c[i].init();
    m_title=m_xname=m_yname=m_zname="";
    m_c.clear();
    m_n=0;
  }

  Matrixostream& xl(double mi,double ma) 
    {m_min[0]=mi;m_max[0]=ma;return(*this);}
  Matrixostream& yl(double mi,double ma) 
    {m_min[1]=mi;m_max[1]=ma;return(*this);}
  Matrixostream& zl(double mi,double ma) 
    {m_min[2]=mi;m_max[2]=ma;return(*this);}
  Matrixostream& xa(int n=-1) { m_dim=1; m_dir=0; m_axis[0]=n; return(*this);}
  Matrixostream& ya(int n=-1) { m_dim=1; m_dir=1; m_axis[1]=n; return(*this);}
  Matrixostream& za(int n=-1) { m_dim=1; m_dir=2; m_axis[2]=n; return(*this);}
  Matrixostream& xlog() {m_log[0]=1;return(*this);}
  Matrixostream& ylog() {m_log[1]=1;return(*this);}
  Matrixostream& zlog() {m_log[2]=1;return(*this);}
  Matrixostream& title(const string& s) { m_title = s; return(*this); }
  Matrixostream& xname(const string& s) { m_xname = s; return(*this); }
  Matrixostream& yname(const string& s) { m_yname = s; return(*this); }
  Matrixostream& zname(const string& s) { m_zname = s; return(*this); }

  Matrixostream& XY() { m_XY = 1; return(*this); }

  // ostream pipeline operator
  //Matrixostream& operator <<(const Collection& a) { m_c[m_n++]=a; return(*this); }
  Matrixostream& operator <<(const DMatrix& a) { m_c=a; return(*this); }
  Matrixostream& operator <<(const string& s) {
    if(m_c.size()==0) {
      if(m_title=="") m_title = s;
      else if(m_xname=="") m_xname = s;
      else if(m_yname=="") m_yname = s;
      else if(m_zname=="") m_zname = s;
    }
    else {
      Draw(s);
    }
    return(*this);
  }
  Matrixostream& operator <<(const char c){
    return(Draw("\n"));
  }
  Matrixostream& operator <<(const G__CINT_ENDL c) {return(Draw("\n"));}

  ////////////////////////////////////////////////
  int readoption(const string& sin) {
#if 1
    char *s = new char[sin.size()+1];
    strcpy(s,sin.c_str());
    char* p= (char*)strchr(s,'\n');
    if(p) *p=0;
    ReadF f;
    f.parse(s);
    for(int i=1;i<=f.argc;i++) {
      // Vertical(column) Horizontal(line)
      if(tolower(f.argv[i][0])=='v') { m_dir=0; m_axis[0]=0; }
      else if(tolower(f.argv[i][0])=='h') { m_dir=1; m_axis[1]=0; }
      // XLOG YLOG ZLOG
      else if(strstr(f.argv[i],"LOG")) {
        m_log[int(tolower(f.argv[i][0])-'x')]=1;
      }
      // XAxis 0   YAxis 0   ZAxis  0
      else if(tolower(f.argv[i][1])=='a') {
        m_dim=1;
        m_dir = tolower(f.argv[i][0])-'x'; // X:0 Y:1 Z=2
        if(f.argc>=i+1) m_axis[tolower(f.argv[i][0])-'x']=atoi(f.argv[i+1]); 
        ++i;
      }
      //  XLim min max    YLim min max  ZLim min max
      else if(tolower(f.argv[i][1])=='l' && f.argc>=i+2) {
        m_min[int(tolower(f.argv[i][0])-'x')]=atof(f.argv[i+1]); 
        m_max[int(tolower(f.argv[i][0])-'x')]=atof(f.argv[i+2]); 
        i+=2;
      }
      //  XName, YName
      else if(tolower(f.argv[i][1])=='n' && f.argc>=i+1) {
        if(tolower(f.argv[i][0])=='x') m_xname = f.argv[i+1];
        else if(tolower(f.argv[i][0])=='y') m_yname = f.argv[i+1];
        i+=1;
      }
      //  Title
      else if(tolower(f.argv[i][0])=='t' && f.argc>=i+1) {
        m_title = f.argv[i+1];
        i+=1;
      }
      // 1d 2d 3d
      else if(f.argv[i][0]=='1') m_dim=1;
      else if(f.argv[i][0]=='2') m_dim=2;
      else if(f.argv[i][0]=='3') m_dim=3;
      // error
      else { 
        if(m_n==0) m_title = s;
        else        m_xname = s;
      }
    }
    return(p?1:0);
#else
    return(1);
#endif 
  }

  ////////////////////////////////////////////////
  ////////////////////////////////////////////////
  Matrixostream& Draw(const string& s) {
    if(!readoption(s.c_str())) return(*this);
    //if(m_hist) { drawHist(); }
    if(m_dim==1) { draw1D(); }
    else if(m_dim==2) { draw2D(); }
    else if(m_dim==3) { draw3D(); }
    else { draw2D(); }
    init();
    return(*this);
  }

  ////////////////////////////////////////////////
  int draw1D() {
    int i,j;
    //Collection& c = m_c[m_n-1];
    DMatrix& c = m_c;
    int ax,N,M;
    char buf[1000];
    sprintf(buf,"%s\n",m_yname.c_str());
    if(m_dir==0) {
      ax = m_axis[0];
      if(ax<0) ax = -1;
      N = c[0].size();
      M = c.size()-ax-1;
      if(M>20) return(draw2D());
      DVector x(0.0,double(N-1),N); 
      DVector *y = new DVector[M];
      if(ax>=0) for(i=0;i<N;i++) x[i] = c[ax][i];
      for(j=0;j<M;j++) {
        for(i=0;i<N;i++) y[j][i] = c[ax+j+1][i];
      }
      plot << m_title.c_str() << x << m_xname.c_str();
      if(m_min[0]!=m_max[0]) plot << m_min[0] >> m_max[0];
      for(j=0;j<M;j++) plot << y[j] ;
      if(m_min[1]!=m_max[1]) plot << m_min[1] >> m_max[1];
      //if(m_XY) plot.XY();
      plot << buf;
      delete[] y;
    }
    else if(m_dir==1) {
      ax = m_axis[1];
      if(ax<0) ax = -1;
      N = c.size();
      M = c[0].size()-ax-1;
      if(M>20) return(draw2D());
      DVector x(0.0,double(N-1),N); 
      DVector *y = new DVector[M];
      //string *ss = new string[M];
      if(ax>=0) for(i=0;i<N;i++) x[i] = c[i][ax];
      for(j=0;j<M;j++) {
        for(i=0;i<N;i++) y[j][i] = c[i][ax+j+1];
        //if(c.p_key) ss[j] = c.p_key->get(ax+j+1).c_str();
      }
      plot << m_title.c_str() << x << m_xname.c_str();
      if(m_min[0]!=m_max[0]) plot << m_min[0] >> m_max[0];
      //for(j=0;j<M;j++) plot << y[j] << ss[j].c_str();
      if(m_min[1]!=m_max[1]) plot << m_min[1] >> m_max[1];
      //if(m_XY) plot.XY();
      plot << buf;
      //delete[] ss;
      delete[] y;
    }
    else {
    }
    return(0);
  }

  ////////////////////////////////////////////////
  int draw2D() {
    //Collection& c = m_c[m_n-1];
    static int nhist = 0 ;
    char fname[500];
    sprintf(fname,"G__hist%d.csv",nhist++);
    if(m_mode==0) {
      if(graph_mode()==1) 
	cintgraph_Histogram2D(fname,m_c,m_title.c_str(),m_xname.c_str()
			      ,m_min[0],m_max[0]
			      ,m_min[1],m_max[1]
			      ,m_min[2],m_max[2]);
      else 
	gnuplot_Histogram2D(fname,m_c,m_title.c_str(),m_xname.c_str()
			    ,m_min[0],m_max[0]
			    ,m_min[1],m_max[1]
			    ,m_min[2],m_max[2]);
    }
    else {
      //VirtualGraph_Histogram2D(fname,D,m_title.c_str(),m_xname.c_str()
			       //,min[0],max[0],min[1],max[1],min[2],max[2]);
    }
    return(0);
  }

  ////////////////////////////////////////////////
  int draw3D() {
    return(0);
  }

  ///////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////
  void saveNNDB(const char* fname=0,const char* mode="w") {
    VirtualGraph_Histogram2D(fname,D,m_title.c_str(),m_xname.c_str()
			    ,m_min[0],m_max[0]
			    ,m_min[1],m_max[1]
			    ,m_min[2],m_max[2]);

  }

  ////////////////////////////////////////////////
  ////////////////////////////////////////////////

  ////////////////////////////////////////////////
  ////////////////////////////////////////////////
  DMatrix D;

  int        m_n;
  DMatrix    m_c;
  string     m_title;
  string     m_xname;
  string     m_yname;
  string     m_zname;

  int        m_dir;
  char       m_dim;
  double     m_min[3];
  double     m_max[3];
  int        m_log[3];
  int        m_axis[3];

  int m_hist;
  int m_XY;

  int m_mode;
};


extern Matrixostream splot;
//extern Matrixostream splotNN;


#endif

