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

#define RG_MASK_PLOTMODE  0xf
#define RG_NORMALMODE     0x0
#define RG_CSVMODE        0x1
#define RG_PLOTMODE       0x2
#define RG_SAMEMODE       0x4
#define RG_FLUSHMODE      0x8

#define RG_MASK_AUXMODE  0xf0
#define RG_CORR          0x10

#ifdef G__ROOT

/************************************************************************
* rootgraph library
************************************************************************/

//////////////////////////////////////////////////////////////////////
// static data buffer
//////////////////////////////////////////////////////////////////////
static const int G__MAXGRAPH = 200;
static const int G__MAXGRAPHLINE = 200;

static TMultiGraph *rg_mg[G__MAXGRAPH];
static TCanvas *rg_cgr[G__MAXGRAPH];
static TH2F   *rg_scale[G__MAXGRAPH];
static TPaveText *rg_pt[G__MAXGRAPH];
static TText *rg_text[G__MAXGRAPH];
static TLatex *rg_tex[G__MAXGRAPH];
static TLatex *rg_lname[G__MAXGRAPH][G__MAXGRAPHLINE];
static TLine  *rg_lguide[G__MAXGRAPH][G__MAXGRAPHLINE];
static TMarker *rg_marker[G__MAXGRAPH][G__MAXGRAPHLINE];
static int rg_ga=-1;
static int rg_gatitle=-1;
static int rg_ga_pos_origin=0;

static TGraph *rg_gr[G__MAXGRAPH][G__MAXGRAPHLINE];
static string rg_linename[G__MAXGRAPHLINE];
static int nline;

static int rg_canvas_xsize=600;
static int rg_canvas_ysize=400;
static int rg_canvas_shift=20;

static string rg_title[G__MAXGRAPH];
static string rg_comment;
static int rg_linewidth=2;
//
char rg_opt[10] = "ALP";   
//char rg_opt[10] = "ALP"; // axis, line, marker
//char rg_opt[10] = "A*";  // axis, star
//char rg_opt[10] = "AB";  // bar chart
// 
// L : Simple poly-line between every points 
// F : Fill area
// A : Axis
// C : Smooth curve (spline function?)
// * : Star is plotted at each point
// P : Current marker of the graph is plotted at each point
// B : Bar chart
// 

static char rg_opt_bk[10];
void rootgraph_option(const char* opt) { rootgraphrg_option(opt); }
void rootgraphrg_option(const char* opt) {
  strcpy(rg_opt_bk,rg_opt);
  strcpy(rg_opt,opt);
}
void rootgraph_recoveroption(const char* opt) {
  strcpy(rg_opt,rg_opt_bk);
}

//////////////////////////////////////////////////////////////////////
// color table
//////////////////////////////////////////////////////////////////////
static EColor rg_plotcolorTable1[] = { 
    kBlack , kRed    , kBlue   , kGreen  , kMagenta , kCyan  , kYellow 
   //, kGray   , kOrange , kSpring , kTeal  , kAzure  , kViolet , kPink   
    , 8 , 9 /*,10 */ ,11 , 12, 13,14, 15 , 16 , 17
    ,kBlack , kRed    , kBlue   , kGreen  , kMagenta , kCyan  , kYellow 
    , 8 , 9 /*,10 */ ,11 , 12, 13,14, 15 , 16 , 17
    ,kBlack , kRed    , kBlue   , kGreen  , kMagenta , kCyan  , kYellow 
    , 8 , 9 /*,10 */ ,11 , 12, 13,14, 15 , 16 , 17
  //, kBlack , kBlack  , kBlack  , kBlack  , kBlack
  //, kBlack , kBlack  , kBlack  , kBlack  , kBlack
  , kBlack , kBlack  , kBlack  , kBlack  , kBlack
  , kBlack , kBlack  , kBlack  , kBlack  , kBlack
  , kBlack , kBlack  , kBlack  , kBlack  , kBlack
  , kBlack , kBlack  , kBlack  , kBlack  , kBlack
  , kBlack , kBlack  , kBlack  , kBlack  , kBlack
  , kBlack , kBlack  , kBlack  , kBlack  , kBlack
  , kBlack , kBlack  , kBlack  , kBlack  , kBlack
  , kBlack , kBlack  , kBlack  , kBlack  , kBlack
  , kBlack , kBlack  , kBlack  , kBlack  , kBlack
  , kBlack , kBlack  , kBlack  , kBlack  , kBlack
  , -1 };
static EColor rg_plotcolorTable2[] = { 
    kBlack , kBlack  , kBlack  , kBlack  , kBlack
  , kBlack , kBlack  , kBlack  , kBlack  , kBlack
  , kBlack , kBlack  , kBlack  , kBlack  , kBlack
  , kBlack , kBlack  , kBlack  , kBlack  , kBlack
  , kBlack , kBlack  , kBlack  , kBlack  , kBlack
  , kBlack , kBlack  , kBlack  , kBlack  , kBlack
  , kBlack , kBlack  , kBlack  , kBlack  , kBlack
  , kBlack , kBlack  , kBlack  , kBlack  , kBlack
  , kBlack , kBlack  , kBlack  , kBlack  , kBlack
  , kBlack , kBlack  , kBlack  , kBlack  , kBlack
  , kBlack , kBlack  , kBlack  , kBlack  , kBlack
  , kBlack , kBlack  , kBlack  , kBlack  , kBlack
  , -1 };
static EColor rg_plotcolorTable3[] = { 
     kRed    , kBlue   , kGreen  , kMagenta 
  , kCyan  , kYellow 
  //, kGray   , kOrange , kSpring , kTeal  , kAzure  , kViolet , kPink   
    , 8 , 9 /*,10 */ ,11 , 12, 13,14, 15 , 16 
  , kBlack , kBlack  , kBlack  , kBlack  , kBlack
  , kBlack , kBlack  , kBlack  , kBlack  , kBlack, kBlack
  , kBlack , kBlack  , kBlack  , kBlack  , kBlack, kBlack
  , kBlack , kBlack  , kBlack  , kBlack  , kBlack, kBlack
  , kBlack , kBlack  , kBlack  , kBlack  , kBlack, kBlack
  , kBlack , kBlack  , kBlack  , kBlack  , kBlack, kBlack
  , kBlack , kBlack  , kBlack  , kBlack  , kBlack, kBlack
  , kBlack , kBlack  , kBlack  , kBlack  , kBlack, kBlack
  , kBlack , kBlack  , kBlack  , kBlack  , kBlack, kBlack
  , kBlack , kBlack  , kBlack  , kBlack  , kBlack, kBlack
  , kBlack , kBlack  , kBlack  , kBlack  , kBlack, kBlack
  , kBlack , kBlack  , kBlack  , kBlack  , kBlack, kBlack
  , -1 };
static EColor* rg_plotcolorTable = rg_plotcolorTable1;
/////////////c////////////////////////////////////////////////////////
// color table
//////////////////////////////////////////////////////////////////////
static Style_t rg_plotstyleTable1[] = { 
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
  2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  -1
};
static Style_t rg_plotstyleTable2[] = { 
  1,3,10,
  2,7,9,
  4,5,6,8,
  11,12,13,14,15,16,17,18,19,20,21,22,23,24,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  -1
};
static Style_t* rg_plotstyleTable = rg_plotstyleTable1;
/////////////c////////////////////////////////////////////////////////
// marker table
//////////////////////////////////////////////////////////////////////
static Style_t rg_markerstyleTable1[] = { 
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  -1
};
static Style_t rg_markerstyleTable2[] = { 
  20,21,22,23,24,25,26,27,28,29,30,2,3,5,6,7,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  -1
};
static Style_t* rg_markerstyleTable = rg_markerstyleTable1;
//////////////////////////////////////////////////////////////////////
void rootgraph_color(int n,int color) {rg_plotcolorTable[n] = color;}
//////////////////////////////////////////////////////////////////////
//void GraphLineAtt(int style) { rootgraph_lineatt(style); }
void rootgraph_lineatt(int style) {
  if(style==1) { // Differentiate with Color
    rg_plotcolorTable = rg_plotcolorTable1;
    rg_plotstyleTable = rg_plotstyleTable1;
    rg_markerstyleTable = rg_markerstyleTable1;
    rg_linewidth=2;
  }
  else if(style==2) { // Differentiate with line style
    rg_plotcolorTable = rg_plotcolorTable2;
    rg_plotstyleTable = rg_plotstyleTable2;
    rg_markerstyleTable = rg_markerstyleTable1;
    rg_linewidth=2;
  }
  else if(style==3)  { // Differentiate with line style and marker
    rg_plotcolorTable = rg_plotcolorTable2;
    rg_plotstyleTable = rg_plotstyleTable2;
    rg_markerstyleTable = rg_markerstyleTable2;
    rg_linewidth=1;
  }
  else if(style==4)  { // Differentiate with line style and marker
    rg_plotcolorTable = rg_plotcolorTable1;
    rg_plotstyleTable = rg_plotstyleTable1;
    rg_markerstyleTable = rg_markerstyleTable2;
    rg_linewidth=1;
  }
  else if(style==5)  { // Differentiate with line style and marker
    rg_plotcolorTable = rg_plotcolorTable1;
    rg_plotstyleTable = rg_plotstyleTable2;
    rg_markerstyleTable = rg_markerstyleTable2;
    rg_linewidth=1;
  }
  else if(style==6) { // Differentiate with Color
    rg_plotcolorTable = rg_plotcolorTable3;
    rg_plotstyleTable = rg_plotstyleTable1;
    rg_markerstyleTable = rg_markerstyleTable1;
    rg_linewidth=2;
  }
}

//////////////////////////////////////////////////////////////////////
TGraph* rootgraph_gr() {return(rg_gr[rg_ga][0]);}
//////////////////////////////////////////////////////////////////////
void rootgraph_size(int xsize,int ysize,int shift=20) {
  rg_canvas_xsize=xsize;
  rg_canvas_ysize=ysize;
  rg_canvas_shift=shift;
}
//////////////////////////////////////////////////////////////////////
char* rootgraph_save_opt = "gif";
int rootgraph_save(int from=0,int to=0,const char* opt=rootgraph_save_opt) {
  string fname;
  if(from<1) { from=0;to=rg_ga; }
  else if(to<1) { to=from; }

  for(int i=from;i<=to;i++) {
    if(rg_cgr[i]) {
      fname = rg_cgr[i]->GetName();
      fname += ".";
      fname += opt;
      rg_cgr[i]->SaveAs(fname.c_str());    
    }
  }
  return(0);
}

//////////////////////////////////////////////////////////////////////
void rootgraph_posclear() {
  rg_ga_pos_origin = rg_ga;
}
//////////////////////////////////////////////////////////////////////
int rootgraph_reset() {
  for(int i=G__MAXGRAPH-1;i>=0;i--) {
     for(int j=G__MAXGRAPHLINE-1;j>=0;j--) {
       if(rg_lname[i][j]>1) delete rg_lname[i][j];
       rg_lname[i][j]=0;
       if(rg_lguide[i][j]>1) delete rg_lguide[i][j];
       rg_lguide[i][j]=0;
       if(rg_gr[i][j]>1) delete rg_gr[i][j];
       rg_gr[i][j] = 0;
       if(rg_marker[i][j]>1) delete rg_marker[i][j];
       rg_marker[i][j] = 0;
     }
    if(rg_pt[i]>1) delete rg_pt[i];
    //if(rg_text[i]>1) delete rg_text[i];
    if(rg_tex[i]>1) delete rg_tex[i];
    if(rg_mg[i]>1) delete rg_mg[i];
    if(rg_cgr[i]>1) delete rg_cgr[i];
    rg_pt[i] = 0;
    //rg_text[i] = 0;
    rg_tex[i] = 0;
    rg_mg[i] = 0;
  }
  //rg_ga = -1;
  return(0);
}
//////////////////////////////////////////////////////////////////////
int rootgraph_clear() {
#if 0
  for(int i=G__MAXGRAPH-1;i>=0;i--) {
     for(int j=G__MAXGRAPHLINE-1;j>=0;j--) {
       if(rg_lname[i][j]>1) delete rg_lname[i][j];
       rg_lname[i][j]=0;
       if(rg_lguide[i][j]>1) delete rg_lguide[i][j];
       rg_lguide[i][j]=0;
       if(rg_gr[i][j]>1) delete rg_gr[i][j];
       rg_gr[i][j] = 0;
       if(rg_marker[i][j]>1) delete rg_marker[i][j];
       rg_marker[i][j] = 0;
     }
    if(rg_pt[i]>1) delete rg_pt[i];
    //if(rg_text[i]>1) delete rg_text[i];
    if(rg_tex[i]>1) delete rg_tex[i];
    if(rg_mg[i]>1) delete rg_mg[i];
    if(rg_cgr[i]>1) delete rg_cgr[i];
    rg_pt[i] = 0;
    //rg_text[i] = 0;
    rg_tex[i] = 0;
    rg_mg[i] = 0;
  }
  rg_ga = -1;
#elif 0
  for(int i=G__MAXGRAPH-1;i>=0;i--) {
     for(int j=G__MAXGRAPHLINE-1;j>=0;j--) {
       rg_lname[i][j]=0;
       rg_lguide[i][j]=0;
       rg_gr[i][j] = 0;
       rg_marker[i][j] = 0;
     }
    rg_pt[i] = 0;
    rg_text[i] = 0;
    rg_tex[i] = 0;
    rg_mg[i] = 0;
  }
  rg_ga = -1;
#elif 1
  nline=0;
#endif
  return(0);
}

//////////////////////////////////////////////////////////////////////
int rootgraph_open(char *filename,char *titlein,int mode) {
  printf("rg_ga=%d nline=%d\n",rg_ga,nline);
   if(rg_ga>=G__MAXGRAPH) {
     fprintf(stderr,"TOO MANY GRAPHS > %d\n",G__MAXGRAPH);
     return(255);
   }
   if(mode&RG_PLOTMODE) { // rootgraph plot mode
     ++rg_ga;
     rg_gatitle=rg_ga;
     rg_mg[rg_ga] = new TMultiGraph;
     printf("rootgraph_open rg_ga=%d %p\n",rg_ga,rg_mg[rg_ga]);
     nline=0;
     if(titlein) rg_title[rg_ga] = titlein;
     else rg_title[rg_ga]="";
   }
   else if(mode&RG_SAMEMODE) { // rootgraph same mode
     // do nothing
   }
   else if(mode&RG_FLUSHMODE) { // flush mode
     // not implemented
#if 0
     ++rg_gatitle;
     if(titlein) rg_title[rg_gatitle] = titlein;
     else rg_title[rg_gatitle]="";
#endif
   }
   else if((mode&RG_MASK_PLOTMODE)==RG_NORMALMODE) { // xgraph mode 
     // do nothing
   }
   if(rg_ga<0) rg_ga=0;
   return(0);
}

//////////////////////////////////////////////////////////////////////
int rootgraph_data(char *filename,double *xdata,double *ydata,int ndata
                  ,char *name) {
  if(rg_ga>=G__MAXGRAPH) return(255);
  if(nline>=G__MAXGRAPHLINE) {
    fprintf(stderr,"TOO MANY LINES > %d\n",G__MAXGRAPHLINE);
    return(255);
  }
  rg_gr[rg_ga][nline] = new TGraph(ndata,xdata,ydata);
  rg_gr[rg_ga][nline]->SetLineWidth(rg_linewidth);
  rg_gr[rg_ga][nline]->SetLineColor(rg_plotcolorTable[nline]);
  rg_gr[rg_ga][nline]->SetMarkerColor(rg_plotcolorTable[nline]);
  rg_gr[rg_ga][nline]->SetLineStyle(rg_plotstyleTable[nline]);
  rg_gr[rg_ga][nline]->SetMarkerStyle(rg_markerstyleTable[nline]);
  if(name && name[0]) { 
    if(filename && filename[0]) {
      rg_linename[nline]=filename; 
      rg_linename[nline].append(":");
      rg_linename[nline].append(name);
    }
    else {
      rg_linename[nline]=name; 
    }
  }
  ++nline;
  return(0);
}

//////////////////////////////////////////////////////////////////////
int rootgraph_invoke(char *filename,double xmin,double xmax
                        ,double ymin,double ymax,int xlog,int ylog
                        ,char *xunit,char *yunit,int mode) {
  if(rg_ga>=G__MAXGRAPH) return(255);
  if(rg_ga<=0 && rg_gr[rg_ga][0]==0){
    fprintf(stderr,"No graph\n");
    return(255);
  }

  char   buf2[1000];
  string sbuf;
  char id[1000];
  int i;
  sbuf = rg_title[rg_ga].c_str(); sbuf+= " ";
  for(i=0;i<nline;i++) sbuf += rg_linename[i].c_str(); sbuf += " ";


  //rg_mg[rg_ga]->SetTitle(sbuf);
  //printf("%d %x %x \n",rg_ga, rg_gr[rg_ga],rg_gr[rg_ga]->GetXaxis());
  rg_gr[rg_ga][0]->GetXaxis()->SetTitle(xunit);
  rg_gr[rg_ga][0]->GetYaxis()->SetTitle(yunit);

  if(mode&RG_PLOTMODE) { 
    sprintf(id,"%s_g%d",rg_title[rg_ga].c_str(),rg_ga);
    rg_cgr[rg_ga] = new TCanvas(id,sbuf.c_str()
				,100+(rg_ga-rg_ga_pos_origin)*rg_canvas_shift
				,(rg_ga-rg_ga_pos_origin)*rg_canvas_shift+20
				,rg_canvas_xsize,rg_canvas_ysize);
    rg_cgr[rg_ga]->SetGrid();

    rg_cgr[rg_ga]->SetFillColor(kWhite);
    rg_cgr[rg_ga]->SetLineColor(kBlack);
    rg_cgr[rg_ga]->SetLineWidth(2);
    rg_cgr[rg_ga]->SetBorderSize(1);
    rg_cgr[rg_ga]->SetFrameLineColor(kBlack);
    rg_cgr[rg_ga]->SetFrameFillColor(kWhite);
    rg_cgr[rg_ga]->SetFrameLineWidth(2);
    rg_cgr[rg_ga]->SetFrameBorderSize(1);
  }

     printf("rg_ga=%d %p\n",rg_ga,rg_mg[rg_ga]);
  for(i=0;i<nline;i++) rg_mg[rg_ga]->Add(rg_gr[rg_ga][i]);
  //rg_mg[rg_ga]->SetFillColor(kWhite);
  rg_mg[rg_ga]->Draw(rg_opt);

  if(xlog) {
    rg_mg[rg_ga]->GetXaxis()->SetNoExponent(kFALSE); // not working
    rg_cgr[rg_ga]->SetLogx(); // this is in effect
  }
  if(ylog) {
    rg_mg[rg_ga]->GetYaxis()->SetNoExponent(kFALSE);
    rg_cgr[rg_ga]->SetLogy(); // this is in effect
  }
  if(xmin!=xmax) rg_mg[rg_ga]->GetXaxis()->SetLimits(xmin,xmax);
  if(ymin!=ymax) rg_mg[rg_ga]->GetYaxis()->SetRangeUser(ymin,ymax);

  if(xmin==xmax) {
    xmin= rg_mg[rg_ga]->GetXaxis()->GetXmin();
    xmax= rg_mg[rg_ga]->GetXaxis()->GetXmax();
  }
  double xdif=xmax-xmin;
  if(ymin==ymax) {
    ymin= rg_mg[rg_ga]->GetYaxis()->GetXmin();
    ymax= rg_mg[rg_ga]->GetYaxis()->GetXmax();
  }
  double ydif=ymax-ymin;
  // top title
  if(rg_title[rg_ga].size()) {
    rg_pt[rg_ga] = new TPaveText(xmin+xdif/12,ymax+ydif/50,xmax+xdif/15,ymax+ydif/9,"br");
    //rg_pt[rg_ga]->SetFillColor(19);
    rg_pt[rg_ga]->SetFillColor(kWhite);
    rg_text[rg_ga] = rg_pt[rg_ga]->AddText(rg_title[rg_ga].c_str());
    rg_pt[rg_ga]->Draw();
  }
  // low comment
  rg_tex[rg_ga] = new TLatex(xmax-xdif/60*rg_comment.size(),ymin-ydif/10,rg_comment.c_str());
  rg_tex[rg_ga]->SetLineWidth(2);
  rg_tex[rg_ga]->Draw();

  // line name
  for(i=0;i<nline;i++) { 
#if 1
    if(rg_linename[i].size()) {
      rg_lguide[rg_ga][i] = new TLine(xmax-xdif*0.48 ,ymax-ydif/35-ydif/25*i
                             ,xmax-xdif*0.42,ymax-ydif/35-ydif/25*i);
      rg_lguide[rg_ga][i]->SetLineColor(rg_plotcolorTable[i]);
      rg_lguide[rg_ga][i]->SetLineStyle(rg_plotstyleTable[i]);
      rg_lguide[rg_ga][i]->Draw();
      rg_marker[rg_ga][i] = new TMarker(xmax-xdif*0.45,ymax-ydif/35-ydif/25*i,rg_markerstyleTable[i]);
      //rg_marker[rg_ga][i]->SetMarkerSize(2);
      rg_marker[rg_ga][i]->SetMarkerColor(rg_plotcolorTable[i]);
      rg_marker[rg_ga][i]->Draw();
    }
#endif
    rg_lname[rg_ga][i] = new TLatex(xmax-xdif*0.4,ymax-ydif/25-ydif/25*i,rg_linename[i].c_str());
    rg_lname[rg_ga][i]->SetLineWidth(1);
    rg_lname[rg_ga][i]->SetTextSize(0.03);
    rg_lname[rg_ga][i]->SetTextColor(rg_plotcolorTable[i]);
    rg_lname[rg_ga][i]->Draw();
    rg_linename[i] = "";
  }

#if 0
  rg_mg[rg_ga]->GetXaxis()->SetTitle(xunit);
  rg_mg[rg_ga]->GetYaxis()->SetTitle(yunit);
  //rg_mg[rg_ga]->GetXaxis()->SetName(xunit);
  //rg_mg[rg_ga]->GetYaxis()->SetName(yunit);
#else
  // x axis unit
  int len= xunit?strlen(xunit):0;
  //rg_tex[rg_ga] = new TLatex(xmin+xdif*0.45,ymin-ydif/10,xunit);
  rg_tex[rg_ga] = new TLatex(xmin+xdif*(0.48-len*0.007),ymin-ydif/10,xunit);
  rg_tex[rg_ga]->SetLineWidth(2);
  rg_tex[rg_ga]->Draw();
  // y axis unit
  len= yunit?strlen(yunit):yunit;
  //rg_tex[rg_ga] = new TLatex(xmin-xdif/10.5,ymin+ydif/2,yunit);
  rg_tex[rg_ga] = new TLatex(xmin-xdif/10.5,ymin+ydif*(0.5-len*0.012),yunit);
  rg_tex[rg_ga]->SetLineWidth(2);
  rg_tex[rg_ga]->SetTextAngle(90);
  rg_tex[rg_ga]->Draw();
#endif

#ifdef RG_CORR
  if(mode&RG_CORR) {
    sprintf(buf2,"f%d",rg_ga);
    sbuf = buf2;
    TF1 *f1=new TF1(sbuf.c_str(),"x",-1e100,1e100);
    f1->SetLineWidth(1);
    f1->SetLineColor(3);
    f1->Draw("Same");
  }
#endif


  if(1) { 
    // draw
#if 1
    rootgraph_flush();
#else
    rg_cgr[rg_ga]->Modified();
    rg_cgr[rg_ga]->cd();
    rg_cgr[rg_ga]->SetSelected(rg_cgr[rg_ga]);
#endif
  }

  //++rg_ga;

  return(0);
}

//////////////////////////////////////////////////////////////////////
int rootgraph_flush() {
  rg_cgr[rg_ga]->Modified();
  rg_cgr[rg_ga]->cd();
  rg_cgr[rg_ga]->SetSelected(rg_cgr[rg_ga]);
  rg_cgr[rg_ga]->Update();
}

//////////////////////////////////////////////////////////////////////

#endif
