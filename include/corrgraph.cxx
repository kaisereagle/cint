///////////////////////////////////////////////////////////////////////
// corrgraph.cxx
// Description:
//  Drawing correlation graph from Collection object
//
// Example1:
//  s.Read(fname);
//  c = s.collection();
//  c.setkey(0);
//  vector<int> ipos;
//  ipos.push_back(c.getx("*4080*"));
//  ipos.push_back(c.getx("*MPPT*Seri*"));
//  ipos.push_back(c.getx("*MPPT*Para*"));
//  CorrGraph(c,ipos,"Capacitance Correlation","4080(F)","MPPT(F)");
//
// Example2:
//  s.Read(fname);
//  c = s.collection();
//  c.setkey(0);
//  vector<string> spos;
//  spos.push_back("*4080*");
//  spos.push_back("*MPPT*Seri*");
//  spos.push_back("*MPPT*Para*");
//  CorrGraph(c,spos,"Capacitance Correlation","4080(F)","MPPT(F)");
///////////////////////////////////////////////////////////////////////

#include <CSV.h>
#include <vector>
#include <string>
#include <array>

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
void CorrGraph(Collection& cx
	       ,const vector<int>& ipos
	       ,const char* title
	       ,const char* xname
	       ,const char* yname
	       ,double xmin=0.0, double xmax=0.0
	       ,double ymin=0.0, double ymax=0.0
		) {
  if(xmin!=xmax && ymin==ymax) {ymin = xmin; ymax = xmax;}

  Collection cy = cx.column(ipos);

  strcpy(_opt, "A*");  // axis, marker
  draw.title(title).xname(xname).yname(yname).XY().xl(xmin,xmax).yl(ymin,ymax)
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
	       ,const char* yname
	       ,double xmin=0.0, double xmax=0.0
	       ,double ymin=0.0, double ymax=0.0
		) {

  vector<int> ipos;
  int x;
  vector<string>::const_iterator first = pos.begin();
  vector<string>::const_iterator last  = pos.end();
  while(first!=last) {
    x = getx(*first++);
    if(x>=0) ipos.push_back(x);
  }
  CorrGraph(cx,ipos,title,xname,yname,xmin,xmax,ymin,ymax);
}

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
void CorrGraph(Collection& cx
	       ,const vector<int>& ipos
	       ,const char* title
	       ,const vector<string>& name
	       ,double xmin=0.0, double xmax=0.0
	       ,double ymin=0.0, double ymax=0.0
		) {
  if(xmin!=xmax && ymin==ymax) {ymin = xmin; ymax = xmax;}

  Collection cy = cx.column(ipos);

  strcpy(_opt, "A*");  // axis, marker
  draw.title(title).xname(name[0].c_str()).yname(name[1].c_str()).XY().xl(xmin,xmax).yl(ymin,ymax)
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
	       ,const vector<string>& name
	       ,double xmin=0.0, double xmax=0.0
	       ,double ymin=0.0, double ymax=0.0
		) {

  vector<int> ipos;
  int x;
  for(int i=0;i<pos.size();i++) {
    x = getx(pos[i]);
    if(x>=0) ipos.push_back(x);
  }
  CorrGraph(cx,ipos,title,name,xmin,xmax,ymin,ymax);
}
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
void SuperimposeLine(const char* formula,int width=1,int color=3) {
  static int n =0;
  char name[10];
  sprintf(name,"ls%d",n++);
  TF1 *f2=new TF1(name,formula,-1e100,1e100);
  f2->SetLineWidth(width);
  f2->SetLineColor(color);
  f2->Draw("Same");
  rootgraph_flush();
}

/////////////////////////////////////////////////////////////////////////
