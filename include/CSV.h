/*****************************************************************************
 * CSV.h
 *  Description: 
 *    CSV table format manipulator library.
 *  Status: Production
 *  Date: 2004
 *  Author: Masaharu Goto
 *****************************************************************************/
/************************************************************************
 * Copyright(c) 1995~2015  Masaharu Goto (cint@pcroot.cern.ch) (gotom@hanno.jp)
 *
 * Permission to use, copy, modify and distribute this software and its 
 * documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation. The author makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 ************************************************************************/

#ifndef CSV_H
#define CSV_H
#define G__STATISTICS_H

#pragma include <CSV.dll>     // Built under lib/CSV directory
#pragma include <EachFile.h>
#ifndef __MAKECINT__
#include <CSVclustering2.h>
#endif

//#include <ReadF.h>
int G__ateval(const Collection& c) { 
  if(c.size() && c[0].size()<8) {
    if(c.size()<200) c.dispform(); 
    else {
      int s=c.size();
      c.line(0,10).dispform(); 
      printf("... size=%d ...\n",s);
      //c.line(s-5,s-1).dispform(); // Problem on MacOS
      c.line(s-5,s-1).disp(); 
      //c.back().disp(); 
    }
  }
  else {
    if(c.size()<200) c.disp(); 
    else {
      int s=c.size();
      c.line(0,10).disp(); 
      printf("... size=%d ...\n",s);
      c.line(s-5,s-1)._disp(); 
      //c.back().disp(); 
    }
  }
  return(1); 
}
int G__ateval(const Sheet& s) { s.disp(); return(1); }

template<class T> int G__ateval(const Vector<T>& x) {x.disp(); return(1);}

/////////////////////////////////////////////////////////////////////////
#include <iostream>
ostream& operator<<(ostream& os,const Collection& c) { 
  if(c.size()<20) {
    for(int i=0;i<c.size();i++) {
      for(int j=0;j<c[i].size();j++) {
	os << c[i][j].tostring() << "," ;
      }
      os << endl;
    }
  }
  else {
    int s=c.size();
    os << c.line(0,10);
    os << "... size=" << s << " ...\n";
    os << c.line(s-5,s);
    //c.back().disp(); 
  }
  return(os);
}
/////////////////////////////////////////////////////////////////////////
int viewcsv(const char* fnameexpr) {
#if 1
  Scandir d(fnameexpr);
  char* fname;
  char com[500];
  ReadF f;
  while((fname=d.Next())&&fname[0]) {
    f.open(fname);
    while(f.read()) {
      printf("%s\n",f.argv[0]);
    }
    f.close();
  }
#else
  Sheet sx;
  Collection cx;
  Scandir d(fnameexpr);
  char* fname;
  int n=0;
  while((fname=d.Next())&&fname[0]) {
    sx.Read(fname);
    cx=s.collection();
    cx.disp();
    ++n;
  }
  return(n);
#endif
}

/////////////////////////////////////////////////////////////////////////
#if 1
//#include <NVector.h>
typedef DVector array;
//typedef DCVector carray;

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
void Line::let(const DVector& v) {
  for(int x=0;x<v.size();x++) let(x,v[x]);
}
/////////////////////////////////////////////////////////////////////
void Collection::let(int x,const DVector& v) {
  Line l;
  for(int y=0;y<v.size();y++) {
    if(y>=size()) push_back(l);
    get(y).let(x,v[x]);
  }
}
/////////////////////////////////////////////////////////////////////
DVector Collection::DVector(int x) {
  DVector v;
  for(int y=0;y<size();y++) v.push_back(get(y)[x].todouble());
  return(v);
}
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
void Collection::let(const DMatrix& m) {
  Line l;
  if(m.size()==0 || m[0].size()==0) return;
  for(int y=0;y<m[0].size();y++) {
    for(int x=0;x<m.size();x++) {
      l.let(x,m[y][x]);
    }
    push_back(l);
  }
}
/////////////////////////////////////////////////////////////////////
DMatrix Collection::DMatrix() {
  DMatrix m;
  DVector v;
  if(size()==0 || get(0).size()==0) return(m);
  int Xsize=get(0).size(), Ysize=size();
  v.resize(size());
#ifdef G__APPLE
  G__optimizemode(0);
#endif
  for(int x=0;x<Xsize;x++) {
    for(int y=0;y<Ysize;y++) {
      if(get(y).size()<x) {
	v[y] = get(y)[x].todouble();
      }
    }
    m.push_back(v);
  }
#ifdef G__APPLE
  G__optimizemode(3);
#endif
  return(m);
}
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
DMatrix ToDMatrix(const DVector& x,const DVector& y
	      ,double (*f)(double a,double b)) {
  DVector v;
  DMatrix m;
  v.resize(x.size());
  for(int j=0;j<y.size();j++) {
    for(int i=0;i<x.size();i++) {
      v[i] = f(x[i],y[j]);
    }
    m.push_back(v);
  }
  return(m);
}

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
DVector ToDVector(Collection& c1,int x) {
  DVector tmp;
  for(int i=0;i<c1.size();i++) {
    if(x<c1[i].size()) tmp.push_back(c1[i][x].todouble());
  }
  return tmp;
}
/////////////////////////////////////////////////////////////////////
vector<string> ToSVector(Collection& c1,int x) {
  vector<string> tmp;
  for(int i=0;i<c1.size();i++) {
    if(x<c1[i].size()) tmp.push_back(c1[i][x].c_str());
  }
  return tmp;
}
#if 0
/////////////////////////////////////////////////////////////////////
DVector ToDCVector(Collection& c,int x,int y) {
  DCVector tmp;
  DComplex dc;
  for(int i=0;i<c.size();i++) {
    dc.real() = c[i][x].todouble();
    dc.imag() = c[i][y].todouble();
    tmp.push_back(dc);
  }
  return tmp;
}
#endif
/////////////////////////////////////////////////////////////////////

#else
#include <array.h>
#endif
#ifdef G__ROOT
#include <ROOT_histogram.h>
#endif
//#include <vector>
//#include <deque>
//#include <string>
//#include <map>


/////////////////////////////////////////////////////////////////////////

/**********************************************************************
* Collection to array conversion
**********************************************************************/
///////////////////////////////////////////////////////////////////
int operator=(array& a,const Collection& c) {
  int i,n;
  double *dat = a.dat;
  if(c.size()==0 || c[0].size()==0) {
    fprintf(stderr,"Error: operator=(array&,const Collection&), no data in Collection\n");
    return(255);
  }
  if(c.size() > c[0].size() ) {
    n = c.size();
    a.resize(n);
    for(i=0;i<n;i++) {
      dat[i] = c[i][0].todouble();
    }
  }
  else {
    n = c[0].size();
    a.resize(n);
    for(i=0;i<n;i++) {
      dat[i] = c[0][i].todouble();
    }
  }
  return(0);
}

///////////////////////////////////////////////////////////////////
int operator=(carray& a,const Collection& c) {
  int i,n;
  double *re = a.re;
  double *im = a.im;
  if(c.size()==0 || c[0].size()==0) {
    fprintf(stderr,"Error: operator=(carray&,const Collection&), no data in Collection\n");
    return(255);
  }
  if(c.size() > c[0].size() ) {
    n = c.size();
    a.resize(n);
    for(i=0;i<n;i++) {
      re[i] = c[i][0].todouble();
      im[i] = c[i][1].todouble();
    }
  }
  else {
    n = c[0].size();
    a.resize(n);
    for(i=0;i<n;i++) {
      re[i] = c[0][i].todouble();
      im[i] = c[1][i].todouble();
    }
  }
  return(0);
}


/**********************************************************************
* ROOT file output
**********************************************************************/
int Root2CSV(const char* fname,const Collection& c) {

  if(c.size()<=0) {
    fprintf(stderr,"Warning: %s Data size is 0. Abort creating ROOT file\n"
	    ,fname);
    return(255);
  }

  string filename = fname;
  string name = fname;
  size_t pos = name.find(".root");
  if(pos<name.size()) name = name.substr(0,pos);
  else                filename += ".root";

  TFile file = new TFile(filename.c_str(),"recreate");
  TTree tree = new TTree(name.c_str(),name.c_str());

  vector<string> elements;
  vector<long>   pointers;
  vector<char>   types;
  int i,j;

  // header,  tree->Branch("name", pointer, "name/T")
  Line keyline = c.getkey();
  LIne line = c[0];
  string element;
  char type;
  void *p;
  int n = min(keyline.size(),line.size());
  for(j=0;j<n;j++) {
    if(line[j].isdouble()) {
      type = 'D';
      p = new double[1];
    }
    else {
      type = 'C';
      p = new char[256];
    }
    element = keyline[0].c_str();
    element += '/'; element += type;
    elements.push_back(element);
    pointers.push_back((long)p);
    types.push_back(type);
    tree->Branch(keyline[j].c_str(),p,element.c_str());
  }

  // body
  for(i=0;i<c.size();c++) {
    line = c[i];
    for(j=0;j<line.size()&&j<n;j++) {
      element = elements[j];
      type = types[j];
      p = (void*)pointers[j];
      switch(type) {
      case 'D':
	*(double*)p = line[j].todouble();
	break;
      case 'C':
	strcpy((char*)p, line[j].c_str());
	break;
      default:
	break;
      }
      tree->Fill();
    }
  }
  tree->Write();
  file->Write();

  for(j=0;j<n;j++) {
    switch(types[j]) {
    case 'D':
      delete[] (double*)pointers[j];
      break;
    case 'C':
      delete[] (char*)char[j];
      break;
    default:
      break;
    }
  }

  delete(tree);
  delete(file);
  
  return(0);
}

///////////////////////////////////////////////////////////////////////
// Collection to array conversion
///////////////////////////////////////////////////////////////////////
array column(const Collection& cx,int x) {
  int sz = cx.size();
  array r(0.0,0.0,sz);
  for(int i=0;i<sz;i++) {
    r[i] = cx[i][x].todouble();
  }
  return(r);
}
///////////////////////////////////////////////////////////////////////
array row(const Collection& cx,int y) {
  Line l = cx[y];
  int sz = l.size();
  array r(0.0,0.0,sz);
  for(int i=0;i<sz;i++) {
    r[i] = l[i].todouble();
  }
  return(r);
}

/////////////////////////////////////////////////////////////////////////
// Correlation graph
/////////////////////////////////////////////////////////////////////////
//#include <vector>
//#include <string>
//#include <array>

/////////////////////////////////////////////////////////////////////////
void CorrGraph(Collection& cx
	       ,const vector<int>& ipos
	       ,const char* title
	       ,const char* xname
	       ,const char* yname) {

  Collection cy = cx.column(ipos);

  strcpy(_opt, "A*");  // axis, marker
  draw.title(title).xname(xname).yname(yname).XY()
      << cy
      << "YA 0\n";

  static int i=0;
  char buf[100];
  sprintf(buf,"f%d",i+1);
  TF1 *f1=new TF1(buf,"x",-1e100,1e100);
  f1->SetLineWidth(1);
  f1->SetLineColor(3);
  f1->Draw("Same");
  rootgraph_flush();

}
/////////////////////////////////////////////////////////////////////////
void CorrGraph(Collection& cx
	       ,const vector<string>& pos
	       ,const char* title
	       ,const char* xname
	       ,const char* yname) {

  vector<int> ipos;
  int x;
#if 1
  for(int i=0;i<pos.size();i++) {
    x = cx.getx(pos[i].c_str());
    if(x>=0) ipos.push_back(x);
  }
#else
  vector<string>::const_iterator first = pos.begin();
  vector<string>::const_iterator last  = pos.end();
  while(first!=last) {
    x = cx.getx(*first++);
    if(x>=0) ipos.push_back(x);
  }
#endif
  CorrGraph(cx,ipos,title,xname,yname);
}
/////////////////////////////////////////////////////////////////////////
DVector toDVector(const Collection& c) {
  DVector r;
  for(int i=0;i<c.size();i++) {
    for(int j=0;j<c[i].size();j++) {
      if(c[i][j].isdouble()) r.push_back(c[i][j].todouble());
    }
  }
  return(r);
}

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
// NTuple data handling
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
// plot
/////////////////////////////////////////////////////////////////////////
void plot(int x,const Collection& c,const string& title="",const string& xname="",const string& opt="A*") {
  rootgraph_option(opt.c_str());
  DVector dx = column(c,x);
  DVector di = dx;
  di.i();
  string ystr = xname; ystr+="\n";
  plot << title << di << "i" << dx << ystr;
}
/////////////////////////////////////////////////////////////////////////
void plot(const string& x,const Collection& c,const string& title="") {
  int xx = c.getx(x.c_str());
  plot(xx,c,title,x);
}
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
void plot(int x,int y,const Collection& c,const string& title="",const string& xname="",const string& yname="",const string& opt="A*") {
  rootgraph_option(opt.c_str());
  DVector dx = column(c,x);
  DVector dy = column(c,y);
  string ystr = yname; ystr+="\n";
  plot << title.c_str() << dx << xname.c_str() << dy << ystr.c_str();
}
/////////////////////////////////////////////////////////////////////////
void plot(const string& x,const string& y,const Collection& c,const string& title="") {
  int xx = c.getx(x.c_str());
  int xy = c.getx(y.c_str());
  plot(xx,xy,c,title,x,y);
}
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
void plot(int x,int y,int z,const Collection& c,const string& title="",const string& opt="surf4") {
  rootgraph_option(opt.c_str());
  Collection tmp = c;
  tmp.stat();
  double xmin = tmp.getstat()[x].min();
  double xmax = tmp.getstat()[x].max();
  double ymin = tmp.getstat()[y].min();
  double ymax = tmp.getstat()[y].max();
  Collection l_t;
  l_t.resize((int)(ymax-ymin+1));
  int xi,yi;
  double val;
  for(int i=0;i<c.size();i++) {
    xi = (int)c[i][x].todouble();
    yi = (int)c[i][y].todouble();
    val = c[i][z].todouble();
    l_t[yi-ymin].let(xi-xmin,val);
  }
  draw << title.c_str() << l_t << endl;
}
/////////////////////////////////////////////////////////////////////////
void plot(const string& x,const string& y,const string& z,const Collection& c,const string& title="") {
  int xx = c.getx(x.c_str());
  int xy = c.getx(y.c_str());
  int xz = c.getx(z.c_str());
  plot(xx,xy,xz,c,title);
}

/////////////////////////////////////////////////////////////////////////
// hist
/////////////////////////////////////////////////////////////////////////
void hist(int x,const Collection& c,const string& title="",int islog=0) {
  int nbin=100;
  static int n=0;
  char name[50]; 
  Collection tmp = c;
  tmp.stat();
  double xmin = tmp.getstat()[x].min();
  double xmax = tmp.getstat()[x].max();
  TCanvas c1;
  sprintf(name,"c1_%s_%d",title.c_str(),n);
  new TCanvas(name,name,10+n*20,10+20*n,800,600);
  sprintf(name,"p1_%d",n);
  new TPad(name,name,0.01,0.01,0.99,0.99,21);
  sprintf(name,"h1_%d",n);
  TH1D *h1 = new TH1D(name,title.c_str(),nbin,xmin,xmax);
  int i;
  for(i=0;i<c.size();i++) {
    h1->Fill(c[i][x].todouble());
  }
  if(islog) {
    for(i=0;i<=h1->GetNbinsX();i++) h1->SetBinContent(i,log10(h1->GetBinContent(i)+1));
  }
  h1->Draw(rh_opt1.c_str());
  ++n;
}
/////////////////////////////////////////////////////////////////////////
void hist(const string& x,const Collection& c,const string& title="",int islog=0) {
  int xx = c.getx(x.c_str());
  string stitle = title;
  if(stitle=="") stitle = x;
  hist(xx,c,stitle,islog);
}
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
void hist(int x,int y,const Collection& c,const string& title="",int islog=0) {
  int nbinx=100,nbiny=100;
  static int n=0;
  char name[50]; 
  Collection tmp = c;
  tmp.stat();
  double xmin = tmp.getstat()[x].min();
  double xmax = tmp.getstat()[x].max();
  double ymin = tmp.getstat()[y].min();
  double ymax = tmp.getstat()[y].max();
  TCanvas c1;
  sprintf(name,"c2_%s_%d",title.c_str(),n);
  new TCanvas(name,name,10+n*20,10+20*n,800,600);
  sprintf(name,"p2_%d",n);
  new TPad(name,name,0.01,0.01,0.99,0.99,21);
  sprintf(name,"h2_%d",n);
  TH2D *h2 = new TH2D(name,title.c_str(),nbinx,xmin,xmax,nbiny,ymin,ymax);
  int i,j;
  for(i=0;i<c.size();i++) {
    h2->Fill(c[i][x].todouble(),c[i][y].todouble());
  }
  if(islog) {
    for(i=0;i<=h2->GetNbinsX();i++) 
      for(j=0;j<=h2->GetNbinsY();j++) 
       h2->SetBinContent(i,j,log10(h2->GetBinContent(i,j)+1));
  }
  h2->Draw(rh_opt2.c_str());
  ++n;
}
/////////////////////////////////////////////////////////////////////////
void hist(const string& x,const string& y,const Collection& c,const string& title="",int islog=0) {
  int xx = c.getx(x.c_str());
  int xy = c.getx(y.c_str());
  string stitle = title;
  if(stitle=="") stitle = x + " " + y;
  hist(xx,xy,c,stitle,islog);
}
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
void hist(int x,int y,int z,const Collection& c,const string& title="",int islog=0) {
  static int n=0;
  char name[50]; 
  Collection tmp = c;
  tmp.stat();
  double xmin = tmp.getstat()[x].min();
  double xmax = tmp.getstat()[x].max();
  double ymin = tmp.getstat()[y].min();
  double ymax = tmp.getstat()[y].max();
  double zmin = tmp.getstat()[z].min();
  double zmax = tmp.getstat()[z].max();
  TCanvas c1;
  sprintf(name,"c3_%s_%d",title.c_str(),n);
  new TCanvas(name,name,10+n*20,10+20*n,800,600);
  sprintf(name,"p3_%d",n);
  new TPad(name,name,0.01,0.01,0.99,0.99,21);
  sprintf(name,"h3_%d",n);
  TH3D *h3 = new TH3D(name,title.c_str(),100,xmin,xmax,100,ymin,ymax,100,zmin,zmax);
  for(int i=0;i<c.size();i++) {
    h3->Fill(c[i][x].todouble(),c[i][y].todouble(),c[i][z].todouble());
  }
  h3->Draw(rh_opt3.c_str());
  ++n;
}
/////////////////////////////////////////////////////////////////////////
void hist(const string& x,const string& y,const string& z,const Collection& c,const string& title="",int islog=0) {
  int xx = c.getx(x.c_str());
  int xy = c.getx(y.c_str());
  int xz = c.getx(z.c_str());
  string stitle = title;
  if(stitle=="") stitle = x + " " + y + " " + z;
  hist(xx,xy,xz,c,stitle,islog);
}
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
void histX(const Collection& c,const string& title="",int islog=0) {
  int nbinx=100,nbinv=c.size();
  static int n=0;
  char name[50]; 
  Collection tmp = c;
  tmp.stat();
  int i,j;
  double val,vmin=1e99,vmax=-1e99;
  for(i=0;i<c.size();i++) {
    for(j=0;j<c[0].size();j++) {
      val = c[i][j].todouble();
      if(val>vmax) vmax =val;
      if(val<vmin) vmin =val;
    }
  }
  double xmin=0; 
  double xmax = c[0].size()+1;
  TCanvas c1;
  sprintf(name,"c2_%s_%d",title.c_str(),n);
  new TCanvas(name,name,10+n*20,10+20*n,800,600);
  sprintf(name,"p2_%d",n);
  new TPad(name,name,0.01,0.01,0.99,0.99,21);
  sprintf(name,"h2_%d",n);
  TH2D *h2 = new TH2D(name,title.c_str(),nbinx,xmin,xmax,nbinv,vmin,vmax);
  for(j=0;j<c[0].size();j++) {
    for(i=0;i<c.size();i++) {
      h2->Fill((double)j,c[i][j].todouble());
    }
  }
  if(islog) {
    for(i=0;i<=h2->GetNbinsX();i++) 
      for(j=0;j<=h2->GetNbinsY();j++) 
       h2->SetBinContent(i,j,log10(h2->GetBinContent(i,j)+1));
  }
  h2->Draw(rh_opt2.c_str());
  ++n;
}

/////////////////////////////////////////////////////////////////////////
void histY(const Collection& c,const string& title="",int islog=0) {
  int nbinv=c[0].size(),nbiny=100;
  static int n=0;
  char name[50]; 
  Collection tmp = c;
  tmp.stat();
  int i,j;
  double val,vmin=1e99,vmax=-1e99;
  for(i=0;i<c.size();i++) {
    for(j=0;j<c[0].size();j++) {
      val = c[i][j].todouble();
      if(val>vmax) vmax =val;
      if(val<vmin) vmin =val;
    }
  }
  double ymin=0, ymax = c.size()+1;
  TCanvas c1;
  sprintf(name,"c2_%s_%d",title.c_str(),n);
  new TCanvas(name,name,10+n*20,10+20*n,800,600);
  sprintf(name,"p2_%d",n);
  new TPad(name,name,0.01,0.01,0.99,0.99,21);
  sprintf(name,"h2_%d",n);
  TH2D *h2 = new TH2D(name,title.c_str(),nbinv,vmin,vmax,nbiny,ymin,ymax);
  for(j=0;j<c[0].size();j++) {
    for(i=0;i<c.size();i++) {
      h2->Fill(c[i][j].todouble(),i);
    }
  }
  if(islog) {
    for(i=0;i<=h2->GetNbinsX();i++) 
      for(j=0;j<=h2->GetNbinsY();j++) 
       h2->SetBinContent(i,j,log10(h2->GetBinContent(i,j)+1));
  }
  h2->Draw(rh_opt2.c_str());
  ++n;
}

#endif

