/////////////////////////////////////////////////////////////////////////
// csviosream.h
//
// Collection 2D/3D drawing
// 
// Sheet s(fname);
// Collection c=s.collection();
// draw << c << endl;
// draw.xl(xmin,xmax).xlog().title(title).xname(xname).yname(yname)<< c << endl;
// // Assuming A and B are in adjacent columns, to draw correlation graph, 
// // use ya(0) method or "YA 0" option.
//   draw.ya(0) << c.column("A","B") << endl;
//   draw       << c.column("A","B") << "YA 0\n";
// // Additional x=y line can be drawn using TF1 class
//   char buf[100];
//   sprintf(buf,"f%d",i+1);
//   TF1 *f1=new TF1(buf,"x",-1e100,1e100);
//   f1->SetLineWidth(1);
//   f1->Draw("Same");
/////////////////////////////////////////////////////////////////////////
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
#ifndef G__CSVIOSTREAM
#define G__CSVIOSTREAM

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
// Moved from CSV.h
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
#include <vector>
#include <deque>
#include <string>
#include <map>
#include "Collection.h"
#include "Vector.h"
#include "Vectoriostream.h"

#define CSV_DRAWMODE 0
#define CSV_NNMODE 1

/**********************************************************************
* csv output stream 
**********************************************************************/
#define csv_csvinvalid 1e99
#define csv_CSVMAX     100
class csvostream {
 public:
  csvostream(int mode=CSV_DRAWMODE) {
    m_mode = mode;
    init();
  }
  void init() {
    for(int i=0;i<3;i++) {
      min[i] = 0;
      max[i] = 0;
      axis[i] = -1;
      log[i] = 0;
    }
    m_dim=2;
    m_dir=1;
    m_hist=0;
    m_XY=0;
    clear();
  }
  void clear() {
    //for(int i=0;i<csv_CSVMAX;i++) m_c[i].init();
    m_c.clear();
    m_n=0;
  }

  csvostream& xl(double mi,double ma) {min[0]=mi;max[0]=ma;return(*this);}
  csvostream& yl(double mi,double ma) {min[1]=mi;max[1]=ma;return(*this);}
  csvostream& zl(double mi,double ma) {min[2]=mi;max[2]=ma;return(*this);}
  csvostream& xa(int n=-1) { m_dim=1; m_dir=0; axis[0]=n; return(*this);}
  csvostream& ya(int n=-1) { m_dim=1; m_dir=1; axis[1]=n; return(*this);}
  csvostream& za(int n=-1) { m_dim=1; m_dir=2; axis[2]=n; return(*this);}
  csvostream& xlog() {log[0]=1;return(*this);}
  csvostream& ylog() {log[1]=1;return(*this);}
  csvostream& zlog() {log[2]=1;return(*this);}
  csvostream& title(const char* s) { m_title = s; return(*this); }
  csvostream& xname(const char* s) { m_xname = s; return(*this); }
  csvostream& yname(const char* s) { m_yname = s; return(*this); }

  csvostream& XY() { m_XY = 1; return(*this); }

  // ostream pipeline operator
  //csvostream& operator <<(const Collection& a) { m_c[m_n++]=a; return(*this); }
  csvostream& operator <<(const Collection& a) { m_c=a; return(*this); }
  csvostream& operator <<(const char *s) {return(Draw(s));}
  csvostream& operator <<(const char c){return(Draw("\n"));}
  csvostream& operator <<(const G__CINT_ENDL c) {return(Draw("\n"));}

  ////////////////////////////////////////////////
  int readoption(const char* s) {
#if 0
    char* p=strchr(s,'\n');
    if(p) *p=0;
    ReadF f;
    f.parse(s);
    for(int i=1;i<=f.argc;i++) {
      // Vertical(column) Horizontal(line)
      if(tolower(f.argv[i][0])=='v') { m_dir=0; axis[0]=0; }
      else if(tolower(f.argv[i][0])=='h') { m_dir=1; axis[1]=0; }
      // XLOG YLOG ZLOG
      else if(strstr(f.argv[i],"LOG")) {
        log[tolower(f.argv[i][0])-'x']=1;
      }
      // XAxis 0   YAxis 0   ZAxis  0
      else if(tolower(f.argv[i][1])=='a') {
        m_dim=1;
        m_dir = tolower(f.argv[i][0])-'x'; // X:0 Y:1 Z=2
        if(f.argc>=i+1) axis[tolower(f.argv[i][0])-'x']=atoi(f.argv[i+1]); 
        ++i;
      }
      //  XLim min max    YLim min max  ZLim min max
      else if(tolower(f.argv[i][1])=='l' && f.argc>=i+2) {
        min[tolower(f.argv[i][0])-'x']=atof(f.argv[i+1]); 
        max[tolower(f.argv[i][0])-'x']=atof(f.argv[i+2]); 
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
  csvostream& Draw(const char* s) {
    if(!readoption(s)) return(*this);
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
    Collection& c = m_c;
    int ax,N,M;
    char buf[1000];
    sprintf(buf,"%s\n",m_yname.c_str());
    if(m_dir==0) {
      ax = axis[0];
      if(ax<0) ax = -1;
      N = c[0].size();
      M = c.size()-ax-1;
      if(M>20) return(draw2D());
      DVector x(0.0,double(N-1),N); 
      DVector *y = new DVector[M];
      if(ax>=0) for(i=0;i<N;i++) x[i] = c[ax][i].todouble();
      for(j=0;j<M;j++) {
        for(i=0;i<N;i++) y[j][i] = c[ax+j+1][i].todouble();
      }
      plot << m_title.c_str() << x << m_xname.c_str();
      if(min[0]!=max[0]) plot << min[0] >> max[0];
      for(j=0;j<M;j++) plot << y[j] ;
      if(min[1]!=max[1]) plot << min[1] >> max[1];
      //if(m_XY) plot.XY();
      plot << buf;
      delete[] y;
    }
    else if(m_dir==1) {
      ax = axis[1];
      if(ax<0) ax = -1;
      N = c.size();
      M = c[0].size()-ax-1;
      if(M>20) return(draw2D());
      DVector x(0.0,double(N-1),N); 
      DVector *y = new DVector[M];
      string *ss = new string[M];
      if(ax>=0) for(i=0;i<N;i++) x[i] = c[i][ax].todouble();
      for(j=0;j<M;j++) {
        for(i=0;i<N;i++) y[j][i] = c[i][ax+j+1].todouble();
        if(c.p_key) ss[j] = c.p_key->get(ax+j+1).c_str();
      }
      plot << m_title.c_str() << x << m_xname.c_str();
      if(min[0]!=max[0]) plot << min[0] >> max[0];
      for(j=0;j<M;j++) plot << y[j] << ss[j].c_str();
      if(min[1]!=max[1]) plot << min[1] >> max[1];
      //if(m_XY) plot.XY();
      plot << buf;
      delete[] ss;
      delete[] y;
    }
    else {
    }
    return(0);
  }

  ////////////////////////////////////////////////
  int draw2D() {
    //Collection& c = m_c[m_n-1];
    Collection& c = m_c;
    //DMatrix D;
    D.clear();
    DVector v;
    int N = c.size();
    int M = c[0].size();
    if(M<=2) {
      m_dim=1;
      m_dir=1;
      return(draw1D());
    }
    if(N<=2) {
      m_dim=1;
      m_dir=0;
      return(draw1D());
    }
    v.resize(M);
    for(int i=0;i<N;i++) {
      for(int j=0;j<M;j++) {
        v[j] = c[i][j].todouble();
      }
      D.push_back(v);
    }
    static int nhist = 0 ;
    char fname[500];
    sprintf(fname,"G__hist%d.csv",nhist++);
    if(m_mode==CSV_DRAWMODE) {
      if(graph_mode()==1) 
	cintgraph_Histogram2D(fname,D,m_title.c_str(),m_xname.c_str()
			      ,min[0],max[0],min[1],max[1],min[2],max[2]);
      else 
	gnuplot_Histogram2D(fname,D,m_title.c_str(),m_xname.c_str()
			    ,min[0],max[0],min[1],max[1],min[2],max[2]);
    }
    else if(m_mode==CSV_NNMODE) {
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
			     ,min[0],max[0],min[1],max[1],min[2],max[2]);

  }

  ////////////////////////////////////////////////
  ////////////////////////////////////////////////
#if 0
  int drawHist() {
    if(m_dim==1) { hist1D(); }
    else if(m_dim==2) { hist2D(); }
    else if(m_dim==3) { hist3D(); }
    return(0);
  }

  ////////////////////////////////////////////////
  int hist1D() {
    char hname[20];
    sprintf(hname,"h%d",m_hist++);
    TF1F f(hname,m_title.c_str(),m_hist);

    //Collection& c = m_c[m_n-1];
    Collection& c = m_c;
    int ax,N,M;
    char buf[1000];
    sprintf(buf,"%s\n",m_yname.c_str());
    if(m_dir==0) {
      ax = axis[0];
      if(ax<0) ax = -1;
      N = c[0].size();
      M = c.size()-ax-1;
      if(M>20) return(draw2D());
      DVector x(0.0,double(N-1),N); 
      DVector *y = new DVector[M];
      if(ax>=0) for(int i=0;i<N;i++) x[i] = c[ax][i].todouble();
      for(int j=0;j<M;j++) {
        for(int i=0;i<N;i++) y[j][i] = c[ax+j+1][i].todouble();
      }
      plot << m_title.c_str() << x << m_xname.c_str();
      if(min[0]!=max[0]) plot << min[0] | max[0];
      for(int j=0;j<M;j++) plot << y[j] ;
      if(min[1]!=max[1]) plot << min[1] | max[1];
      if(m_XY) plot.XY();
      plot << buf;
      delete[] y;
    }
    else if(m_dir==1) {
      ax = axis[1];
      if(ax<0) ax = -1;
      N = c.size();
      M = c[0].size()-ax-1;
      if(M>20) return(draw2D());
      DVector x(0.0,double(N-1),N); 
      DVector *y = new DVector[M];
      string *ss = new string[M];
      if(ax>=0) for(int i=0;i<N;i++) x[i] = c[i][ax].todouble();
      for(int j=0;j<M;j++) {
        for(int i=0;i<N;i++) y[j][i] = c[i][ax+j+1].todouble();
        if(c.p_key) ss[j] = c.p_key->get(ax+j+1).c_str();
      }
      plot << m_title.c_str() << x << m_xname.c_str();
      if(min[0]!=max[0]) plot << min[0] | max[0];
      for(int j=0;j<M;j++) plot << y[j] << ss[j].c_str();
      if(min[1]!=max[1]) plot << min[1] | max[1];
      if(m_XY) plot.XY();
      plot << buf;
      delete[] ss;
      delete[] y;
    }
    else {
    }
    return(0);
  }
#endif

  ////////////////////////////////////////////////
  ////////////////////////////////////////////////
  DMatrix D;

  int        m_n;
  //Collection m_c[csv_CSVMAX];
  Collection m_c;
  string     m_title;
  string     m_xname;
  string     m_yname;

  int        m_dir;
  char       m_dim;
  double min[3];
  double max[3];
  int log[3];
  int axis[3];

  int m_hist;
  int m_XY;

  int m_mode;
};

//static int m_hist=0;

extern csvostream draw;
extern csvostream drawNN;

#endif // G__CSVIOSTREAM
