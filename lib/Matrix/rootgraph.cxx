/* -*- C++ -*- */
/*************************************************************************
 * Copyright(c) 2009  Masaharu Goto (cint@pcroot.cern.ch)
 *
 * X-Y graph utility driver using ROOT graphics
 *
 ************************************************************************/
/************************************************************************
* rootgraph.cxx
*
************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#define G__ROOTGRAPHSL

/************************************************************************
* rootgraph library
************************************************************************/

//////////////////////////////////////////////////////////////////////
// static data buffer
//////////////////////////////////////////////////////////////////////
static const int G__MAXGRAPH = 50;
static const int G__MAXGRAPHLINE = 20;

static TMultiGraph *mg[G__MAXGRAPH];
static TCanvas *cgr[G__MAXGRAPH];
static TH2F   *scale[G__MAXGRAPH];
static TPaveText *pt[G__MAXGRAPH];
static TText *text[G__MAXGRAPH];
static TLatex *tex[G__MAXGRAPH];
static TLatex *lname[G__MAXGRAPH][G__MAXGRAPHLINE];
static TLine  *lguide[G__MAXGRAPH][G__MAXGRAPHLINE];
static TMarker *marker[G__MAXGRAPH][G__MAXGRAPHLINE];
static int ga=-1;
static int gatitle=-1;

static TGraph *gr[G__MAXGRAPH][G__MAXGRAPHLINE];
static string linename[G__MAXGRAPHLINE];
static int nline;

static string title[G__MAXGRAPH];
static string comment;
static int linewidth=2;
//char _opt[10] = "ALP";
char _opt[10] = "ALP";

//////////////////////////////////////////////////////////////////////
// color table
//////////////////////////////////////////////////////////////////////
static EColor _plotcolorTable1[] = { 
    kBlack , kRed    , kBlue   , kGreen  , kMagenta 
  , kCyan  , kYellow 
  //, kGray   , kOrange , kSpring , kTeal  , kAzure  , kViolet , kPink   
    , 8 , 9 /*,10 */ ,11 , 12, 13,14, 15 , 16 
  , kBlack , kBlack  , kBlack  , kBlack  , kBlack
  , kBlack , kBlack  , kBlack  , kBlack  , kBlack
  , -1 };
static EColor _plotcolorTable2[] = { 
    kBlack , kBlack  , kBlack  , kBlack  , kBlack
  , kBlack , kBlack  , kBlack  , kBlack  , kBlack
  , kBlack , kBlack  , kBlack  , kBlack  , kBlack
  , kBlack , kBlack  , kBlack  , kBlack  , kBlack
  , -1 };
static EColor* _plotcolorTable = _plotcolorTable1;
/////////////c////////////////////////////////////////////////////////
// color table
//////////////////////////////////////////////////////////////////////
static Style_t _plotstyleTable1[] = { 
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  -1
};
static Style_t _plotstyleTable2[] = { 
  1,3,10,
  2,7,9,
  4,5,6,8,
  11,12,13,14,15,16,17,18,19,20,21,22,23,24,
  -1
};
static Style_t* _plotstyleTable = _plotstyleTable1;
/////////////c////////////////////////////////////////////////////////
// marker table
//////////////////////////////////////////////////////////////////////
static Style_t _markerstyleTable1[] = { 
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  -1
};
static Style_t _markerstyleTable2[] = { 
  20,21,22,23,24,25,26,27,28,29,30,2,3,5,6,7,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  -1
};
static Style_t* _markerstyleTable = _markerstyleTable1;
//////////////////////////////////////////////////////////////////////
void rootgraph_lineatt(int style) { GraphLineAtt(style); }
//////////////////////////////////////////////////////////////////////
void GraphLineAtt(int style) {
  if(style==1) { // Differentiate with Color
    _plotcolorTable = _plotcolorTable1;
    _plotstyleTable = _plotstyleTable1;
    _markerstyleTable = _markerstyleTable1;
    linewidth=2;
  }
  else if(style==2) { // Differentiate with line style
    _plotcolorTable = _plotcolorTable2;
    _plotstyleTable = _plotstyleTable2;
    _markerstyleTable = _markerstyleTable1;
    linewidth=2;
  }
  else { // Differentiate with line style and marker
    _plotcolorTable = _plotcolorTable2;
    _plotstyleTable = _plotstyleTable2;
    _markerstyleTable = _markerstyleTable2;
    linewidth=1;
  }
}

//////////////////////////////////////////////////////////////////////
int rootgraph_save(int from=0,int to=0,const char* opt="gif") {
  string fname;
  if(from<1) { from=0;to=ga; }
  else if(to<1) { to=from; }

  for(int i=from;i<=to;i++) {
    if(cgr[i]) {
      fname = cgr[i]->GetName();
      fname += ".";
      fname += opt;
      cgr[i]->SaveAs(fname.c_str());    
    }
  }
  return(0);
}

//////////////////////////////////////////////////////////////////////
int rootgraph_clear() {
#if 0
  for(int i=G__MAXGRAPH-1;i>=0;i--) {
     for(int j=G__MAXGRAPHLINE-1;j>=0;j--) {
       if(lname[i][j]>1) delete lname[i][j];
       lname[i][j]=0;
       if(lguide[i][j]>1) delete lguide[i][j];
       lguide[i][j]=0;
       if(gr[i][j]>1) delete gr[i][j];
       gr[i][j] = 0;
       if(marker[i][j]>1) delete marker[i][j];
       marker[i][j] = 0;
     }
    if(pt[i]>1) delete pt[i];
    //if(text[i]>1) delete text[i];
    if(tex[i]>1) delete tex[i];
    if(mg[i]>1) delete mg[i];
    if(cgr[i]>1) delete cgr[i];
    pt[i] = 0;
    //text[i] = 0;
    tex[i] = 0;
    mg[i] = 0;
  }
#elif 1
  for(int i=G__MAXGRAPH-1;i>=0;i--) {
     for(int j=G__MAXGRAPHLINE-1;j>=0;j--) {
       lname[i][j]=0;
       lguide[i][j]=0;
       gr[i][j] = 0;
       marker[i][j] = 0;
     }
    pt[i] = 0;
    text[i] = 0;
    tex[i] = 0;
    mg[i] = 0;
  }

#endif
  ga = -1;
  return(0);
}

//////////////////////////////////////////////////////////////////////
int rootgraph_open(char *filename,char *titlein,int mode) {
   if(ga>=G__MAXGRAPH) {
     fprintf(stderr,"TOO MANY GRAPHS > %d\n",G__MAXGRAPH);
     return(255);
   }
   if(mode==2) {
     // plot mode
     ++ga;
     gatitle=ga;
     mg[ga] = new TMultiGraph;
     nline=0;
     if(titlein) title[ga] = titlein;
     else title[ga]="";
   }
   else if(mode==3) {
     // same mode
   }
   else if(mode==4) { // flush mode
#if 0
     ++gatitle;
     if(titlein) title[gatitle] = titlein;
     else title[gatitle]="";
#endif
   }
   if(ga<0) ga=0;
   return(0);
}

//////////////////////////////////////////////////////////////////////
int rootgraph_data(char *filename,double *xdata,double *ydata,int ndata
                  ,char *name) {
  if(ga>=G__MAXGRAPH) return(255);
  if(nline>=G__MAXGRAPHLINE) {
    fprintf(stderr,"TOO MANY LINES > %d\n",G__MAXGRAPHLINE);
    return(255);
  }
  gr[ga][nline] = new TGraph(ndata,xdata,ydata);
  gr[ga][nline]->SetLineWidth(linewidth);
  gr[ga][nline]->SetLineColor(_plotcolorTable[nline]);
  gr[ga][nline]->SetMarkerColor(_plotcolorTable[nline]);
  gr[ga][nline]->SetLineStyle(_plotstyleTable[nline]);
  gr[ga][nline]->SetMarkerStyle(_markerstyleTable[nline]);
  if(name && name[0]) { 
    if(filename && filename[0]) {
      linename[nline]=filename; 
      linename[nline].append(":");
      linename[nline].append(name);
    }
    else {
      linename[nline]=name; 
    }
  }
  ++nline;
  return(0);
}

//////////////////////////////////////////////////////////////////////
int rootgraph_invoke(char *filename,double xmin,double xmax
                        ,double ymin,double ymax,int xlog,int ylog
                        ,char *xunit,char *yunit,int mode) {
  if(ga>=G__MAXGRAPH) return(255);
  if(ga<=0 && gr[ga][0]==0){
    fprintf(stderr,"No graph\n");
    return(255);
  }

  char buf[1000];
  char id[500];
  int i;
  for(i=0;i<nline;i++) { 
    sprintf(buf+strlen(buf),"%s ",linename[i].c_str());
  }


  //mg[ga]->SetTitle(buf);
  //printf("%d %x %x \n",ga, gr[ga],gr[ga]->GetXaxis());
  gr[ga][0]->GetXaxis()->SetTitle(xunit);
  gr[ga][0]->GetYaxis()->SetTitle(yunit);

  if(mode==2) { 
    sprintf(id,"%s_g%d",title[ga].c_str(),ga);
    cgr[ga] = new TCanvas(id,buf,100+ga*20,ga*20+20,600,400);
    cgr[ga]->SetGrid();

    cgr[ga]->SetFillColor(kWhite);
    cgr[ga]->SetLineColor(kBlack);
    cgr[ga]->SetLineWidth(2);
    cgr[ga]->SetBorderSize(1);
    cgr[ga]->SetFrameLineColor(kBlack);
    cgr[ga]->SetFrameFillColor(kWhite);
    cgr[ga]->SetFrameLineWidth(2);
    cgr[ga]->SetFrameBorderSize(1);
  }

  for(i=0;i<nline;i++) mg[ga]->Add(gr[ga][i]);
  //mg[ga]->SetFillColor(kWhite);
  mg[ga]->Draw(_opt);

  if(xlog) mg[ga]->GetXaxis()->SetNoExponent(kFALSE);
  if(ylog) mg[ga]->GetYaxis()->SetNoExponent(kFALSE);
  if(xmin!=xmax) mg[ga]->GetXaxis()->SetLimits(xmin,xmax);
  if(ymin!=ymax) mg[ga]->GetYaxis()->SetRangeUser(ymin,ymax);

  if(xmin==xmax) {
    xmin= mg[ga]->GetXaxis()->GetXmin();
    xmax= mg[ga]->GetXaxis()->GetXmax();
  }
  double xdif=xmax-xmin;
  if(ymin==ymax) {
    ymin= mg[ga]->GetYaxis()->GetXmin();
    ymax= mg[ga]->GetYaxis()->GetXmax();
  }
  double ydif=ymax-ymin;
  // top title
  if(title[ga].size()) {
    pt[ga] = new TPaveText(xmin+xdif/10,ymax+ydif/50,xmax,ymax+ydif/9,"br");
    //pt[ga]->SetFillColor(19);
    pt[ga]->SetFillColor(kWhite);
    text[ga] = pt[ga]->AddText(title[ga].c_str());
    pt[ga]->Draw();
  }
  // low comment
  tex[ga] = new TLatex(xmax-xdif/60*comment.size(),ymin-ydif/10,comment.c_str());
  tex[ga]->SetLineWidth(2);
  tex[ga]->Draw();

  // line name
  for(i=0;i<nline;i++) { 
#if 1
    if(linename[i].size()) {
      lguide[ga][i] = new TLine(xmax-xdif*0.48 ,ymax-ydif/35-ydif/25*i
                             ,xmax-xdif*0.42,ymax-ydif/35-ydif/25*i);
      lguide[ga][i]->SetLineColor(_plotcolorTable[i]);
      lguide[ga][i]->SetLineStyle(_plotstyleTable[i]);
      lguide[ga][i]->Draw();
      marker[ga][i] = new TMarker(xmax-xdif*0.45,ymax-ydif/35-ydif/25*i,_markerstyleTable[i]);
      //marker[ga][i]->SetMarkerSize(2);
      marker[ga][i]->SetMarkerColor(_plotcolorTable[i]);
      marker[ga][i]->Draw();
    }
#endif
    lname[ga][i] = new TLatex(xmax-xdif*0.4,ymax-ydif/25-ydif/25*i,linename[i].c_str());
    lname[ga][i]->SetLineWidth(1);
    lname[ga][i]->SetTextSize(0.03);
    lname[ga][i]->SetTextColor(_plotcolorTable[i]);
    lname[ga][i]->Draw();
  }

#if 0
  mg[ga]->GetXaxis()->SetTitle(xunit);
  mg[ga]->GetYaxis()->SetTitle(yunit);
  //mg[ga]->GetXaxis()->SetName(xunit);
  //mg[ga]->GetYaxis()->SetName(yunit);
#else
  // x axis unit
  tex[ga] = new TLatex(xmin+xdif*0.45,ymin-ydif/10,xunit);
  tex[ga]->SetLineWidth(2);
  tex[ga]->Draw();
  // y axis unit
  tex[ga] = new TLatex(xmin-xdif/17,ymin+ydif/2,yunit);
  tex[ga]->SetLineWidth(2);
  tex[ga]->SetTextAngle(90);
  tex[ga]->Draw();
#endif

  if(1) { 
    // draw
    cgr[ga]->Modified();
    cgr[ga]->cd();
    cgr[ga]->SetSelected(cgr[ga]);
  }

  //++ga;

  return(0);
}

//////////////////////////////////////////////////////////////////////

