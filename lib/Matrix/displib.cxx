// displib.cxx

#define DISPLIB
#define CINTPLOT

#include "TF2.h"
#include "TH2.h"
#include "TCutG.h"
#include <vector>
#include <map>

#ifdef G__WIN32
#pragma include_noerr "../rootcintscript/libSimCUtil.dll"
#pragma include_noerr "../src/libSimC.dll"
#ifndef SIMCUTILDLL
#pragma include_noerr "../../rootcintscript/libSimCUtil.dll"
#pragma include_noerr "../../src/libSimC.dll"
#endif

#elif defined(G__LINUX)
#pragma include_noerr "../rootcintscript/libSimCUtilLinux.dll"
#pragma include_noerr "../src/libSimCLinux.dll"
#ifndef SIMCUTILDLL
#pragma include_noerr "../../rootcintscript/libSimCUtilLinux.dll"
#pragma include_noerr "../../src/libSimCLinux.dll"
#endif

#elif defined(G__CYGWIN)
#pragma include_noerr "../rootcintscript/libSimCUtilCygwin.dll"
#pragma include_noerr "../src/libSimCCygwin.dll"
#ifndef SIMCUTILDLL
#pragma include_noerr "../../rootcintscript/libSimCUtilCygwin.dll"
#pragma include_noerr "../../src/libSimCCygwin.dll"
#endif

#endif
//#include "../src/ReadF.cxx"
#include "../src/Vector.h"
//#include "../src/Matrix23.h"
//#include <lsm.h> // Cint Least square method library
#ifdef CINTPLOT
#include <array.h>
#endif
//#include <fft.h>

int linewidth=2;



string match;
string not;
EasyExp Rmatch("");
EasyExp Rnot("");
string mode;

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// Common utility
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
FILE *OpenBinaryFile(char* fname,char *argv) {
  string binfname;
  char *px=strrchr(fname,'/');
  char *py=strrchr(fname,'\\');
  if(px) {
    *px=0;
    binfname = string(fname) + "/" + argv;
    *px='/';
  }
  else if(py) {
    *py=0;
    binfname = string(fname) + "\\" + argv;
    *py='\\';
  }
  else binfname = argv;

#ifndef G__WIN32
  size_t pp = binfname.find("\\");
  if(pp!=string::npos) binfname.replace(pp,1,"/");
#endif

  FILE *bfp = fopen(binfname.c_str(),"rb");
  if(!bfp) {
    binfname = argv;
    bfp = fopen(binfname.c_str(),"rb");
  }
  if(!bfp) {
    px = strrchr(argv,'/');
    if(px) binfname = px+1;
    bfp = fopen(binfname.c_str(),"rb");
  }
  if(!bfp) {
    px = strrchr(argv,'\\');
    if(px) binfname = px+1;
    bfp = fopen(binfname.c_str(),"rb");
  }
  if(!bfp) {
    fprintf(stderr,"Failed to open %s\n",binfname.c_str());
  }
  return(bfp);
}

/**********************************************************************
* file output
**********************************************************************/
class fout_t {
 public:
  string fname;
  FILE* fp;
  fout_t() { fp=0; }
  void operator=(const string& name) { fname=name; }
#if 1
  void fopen() { }
  void fclose() { }
#else
  void fopen() { if(fname.size()) fp=fopen(fname.c_str(),"wb"); }
  void fclose() { if(fp) fclose(fp); fp=0; }
#endif
  void print(const string& s) { if(fp) fprintf(fp,"%s ,",s.c_str()); }
  void print(char* s) { if(fp) fprintf(fp,"%s ,",s); }
  void print(double d) { if(fp) fprintf(fp,"%g ,",d); }
  void print() { if(fp) fprintf(fp,"\n"); }
  void print(int i) { if(fp) fprintf(fp,"%d ,",i); }
} fout;
int G__ateval(const fout_t& fo) {
  printf("%s\n",fo.fname.c_str());
  return(0);
}

/**********************************************************************
* Save X-Y Graph
**********************************************************************/
void SaveGraph(char* pref="",int from=0,int to=0,const char* opt="gif") {
#ifdef CINTPLOT
  rootgraph_save(from,to);
#else
  string buf;
  for(int i=MAXGRAPH-1;i>=0;i--) {
    if(cgr[i]) {
      buf = pref;
      buf += cgr[i]->GetName();
      buf += ".";
      buf += opt;
      cgr[i]->SaveAs(buf.c_str());
    }
  }
#endif
}

/**********************************************************************
* Save Historgram
**********************************************************************/
void SaveHist(char* pref="",int from=0,int to=0,const char* opt="gif") {
  string buf;
  for(int i=MAXCANVAS-1;i>=0;i--) {
#if 1
    if(pad[i]) {
      buf = pref;
      buf += c[i]->GetName();
      buf += ".";
      buf += opt;
      pad[i]->SaveAs(buf.c_str());
    }
#else
    if(h1[i]) {
      buf = pref;
      buf += h1[i]->GetName();
      buf += ".";
      buf += opt;
      h1[i]->SaveAs(buf.c_str());
    }
    if(h2[i]) {
      buf = pref;
      buf += h2[i]->GetName();
      buf += ".";
      buf += opt;
      h2[i]->SaveAs(buf.c_str());
    }
    if(h3[i]) {
      buf = pref;
      buf += h3[i]->GetName();
      buf += ".";
      buf += opt;
      h3[i]->SaveAs(buf.c_str());
    }
#endif
  }
}

/**********************************************************************
* Clean up display windows
**********************************************************************/
int append=0;
void Clean() {
  if(append) return;
  ibuf1=0;
  ibuf2=0;
  ibuf3=0;
  for(int i=0;i<MAXCANVAS;i++) {
    if(h1[i]>1) delete h1[i];
    if(h2[i]>1) delete h2[i];
    if(h3[i]>1) delete h3[i];
    if(pad[i]>1) delete pad[i];
    if(c[i]>1) delete c[i];
    if(g2d[i]>1) delete g2d[i];
    h1[i]=0;
    h2[i]=0;
    h3[i]=0;
    g2d[i]=0;
    c[i]=0;
    pad[i]=0;
  }

#ifdef CINTPLOT
  rootgraph_clear();
#else
  for(int i=MAXGRAPH-1;i>=0;i--) {
    if(gr[i]>1) delete gr[i];
    if(gr2[i]>1) delete gr2[i];
    if(gr3[i]>1) delete gr3[i];
    if(gr4[i]>1) delete gr4[i];
    if(gr5[i]>1) delete gr5[i];
    if(gr6[i]>1) delete gr6[i];
    if(gr7[i]>1) delete gr7[i];
    if(gr8[i]>1) delete gr8[i];
    if(pt[i]>1) delete pt[i];
    //if(text[i]>1) delete text[i];
    if(tex[i]>1) delete tex[i];
    if(mg[i]>1) delete mg[i];
    if(cgr[i]>1) delete cgr[i];
    gr[i] = 0;
    gr2[i] = 0;
    gr3[i] = 0;
    gr4[i] = 0;
    gr5[i] = 0;
    gr6[i] = 0;
    gr7[i] = 0;
    gr8[i] = 0;
    cgr[i] = 0;
    pt[i] = 0;
    //text[i] = 0;
    tex[i] = 0;
    mg[i] = 0;
  }
#endif

  lastsamega=0;
  lastfname[0]=0;
  lastcomment[0]=0;
  Cmeas=0;
}

/**********************************************************************
* initialization
**********************************************************************/
void init() {
  if(append) return;
  a=0;
  ga=0;
  Colors.init();
  samecount=0;
  fout.fopen();
  ibuf1=0;
  ibuf2=0;
  ibuf3=0;

  Rmatch.compile(match.c_str());
  Rnot.compile(not.c_str());
}

/**********************************************************************
* terminate display function
**********************************************************************/
double gupper=0.0;
double glower=0.0;
void SetGraphScale(double ymin,double ymax) { glower=ymin; gupper=ymax; }

void terminate() {
  fout.fclose();
#ifdef CINTPLOT
  if(samenu) int samega = lastsamega;
  if(samenu && (samecount-1)%samenu!=samenu-1) {
    plot << glower >> gupper << "A\n";
  }
#else
  if(samenu) int samega = lastsamega;
  if(samenu && (samecount-1)%samenu!=samenu-1) {
//printf("samega=%d  %p %p\n",samega,mg[samega],mg[samega]->GetXaxis());
    double xmin= mg[samega]->GetXaxis()->GetXmin();
    double xmax= mg[samega]->GetXaxis()->GetXmax();
    double xdif=xmax-xmin;
    double ymin= mg[samega]->GetYaxis()->GetXmin();
    double ymax= mg[samega]->GetYaxis()->GetXmax();
    double ydif=ymax-ymin;
    // top title
    pt[samega] = new TPaveText(xmin+xdif/10,ymax+ydif/50,xmax,ymax+ydif/9,"br");
    pt[samega]->SetFillColor(19);
    text[samega] = pt[samega]->AddText(lastfname);
    pt[samega]->Draw();
    // bottom comment
    tex[samega] = new TLatex(xmax-xdif/50*strlen(lastcomment),ymin-ydif/10,lastcomment);
    tex[samega]->SetLineWidth(2);
    tex[samega]->Draw();
    // x axis unit
    tex[samega] = new TLatex(xmin+xdif*0.45,ymin-ydif/15,"(sec)");
    tex[samega]->SetLineWidth(2);
    tex[samega]->Draw();
    // y axis unit
    tex[samega] = new TLatex(xmin-xdif/10,ymin+ydif/2,"(A)");
    tex[samega]->SetLineWidth(2);
    tex[samega]->Draw();
    if(Cmeas) {
      // measured capacitance
      char capbuf[80];
      sprintf(capbuf,"%gF",Cmeas);
      tex[samega] = new TLatex(xmax-xdif/4,ymax-ydif/10,capbuf);
      tex[samega]->SetLineWidth(2);
      tex[samega]->Draw();
      Cmeas=0;
    }
    // draw
    cgr[samega]->Modified();
    cgr[samega]->cd();
    cgr[samega]->SetSelected(cgr[ga]);
  }
#endif
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// Text summary processing
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


/**********************************************************************
* 電荷情報表示
**********************************************************************/
double S_Orig;
double G_Orig;
double C_Orig;
double S_DA;
double G_DA;
double C_DA;
double Cmeas;
void ReadCharge(ReadF& f,char* fname,char* rmode,char* comment) {
  printf("%s %s\n",rmode,comment);
  while(f.read()) {
    if(!f.argc) continue;
    if(f.argv[1][0]=='/' || f.argv[1][0]=='#') continue;
    if(f.argc>=1 && strncmp(f.argv[1],"END",3)==0) {
      break;
    }
    printf("%s:%s:%s  %s\n",fname,rmode,comment,f.argv[0]);
    if(strstr(comment,"Orig") && f.argv[1][0]=='S') S_Orig = atof(f.argv[3]);
    else if(strstr(comment,"Orig") && f.argv[1][0]=='G') G_Orig = atof(f.argv[3]);
    else if(strstr(comment,"Orig") && f.argv[1][0]=='C') C_Orig = atof(f.argv[3]);
    else if(strstr(comment,"DA") && f.argv[1][0]=='S') S_DA = atof(f.argv[3]);
    else if(strstr(comment,"DA") && f.argv[1][0]=='G') G_DA = atof(f.argv[3]);
    else if(strstr(comment,"DA") && f.argv[1][0]=='C') C_DA = atof(f.argv[3]);
    else if(strstr(comment,"Cap") && strcmp(f.argv[1],"C")==0) {
      Cmeas=atof(f.argv[3]);
    }
//f.disp();
//printf("'%s' '%s' '%s' %g\n",comment,f.argv[1],f.argv[2],S_Orig);
  }
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// X-Y data processing
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/**********************************************************************
* Moving average filter
**********************************************************************/
int ave=0;
void movingAve() {
  if(X.size())  X=ave(X,ave);
  if(Y1.size()) Y1=ave(Y1,ave);
  if(Y2.size()) Y2=ave(Y2,ave);
  if(Y3.size()) Y3=ave(Y3,ave);
  if(Y4.size()) Y4=ave(Y4,ave);
  if(Y5.size()) Y5=ave(Y5,ave);
  if(Y6.size()) Y6=ave(Y6,ave);
  if(Y7.size()) Y7=ave(Y7,ave);
  if(Y8.size()) Y8=ave(Y8,ave);
  if(Y9.size()) Y9=ave(Y9,ave);
  if(Y10.size()) Y10=ave(Y10,ave);
  if(Y11.size()) Y11=ave(Y11,ave);
  if(Y12.size()) Y12=ave(Y12,ave);
}
/**********************************************************************
* Normalization
**********************************************************************/
int ave=0;
void normalization() {
  if(Y1.size()) normalize(Y1,0,y1);
  if(Y2.size()) normalize(Y2,1,y2);
  if(Y3.size()) normalize(Y3,2,y3);
  if(Y4.size()) normalize(Y4,3,y4);
  if(Y5.size()) normalize(Y5,4,y5);
  if(Y6.size()) normalize(Y6,5,y6);
  if(Y7.size()) normalize(Y7,6,y7);
  if(Y8.size()) normalize(Y8,7,y8);
  if(Y9.size()) normalize(Y9,8,y9);
  if(Y10.size()) normalize(Y10,9,y10);
  if(Y11.size()) normalize(Y11,10,y11);
  if(Y12.size()) normalize(Y12,11,y12);
}

void normalize(Vector<double>& Y,double os,const string& name) {
  double minimum=1e99;
  double maximum= -1e99;
  int n=Y.size();
  double val;
  for(int i=0;i<n;i++) {
    val=Y[i];
    if(val<minimum) minimum=val;
    if(val>maximum) maximum=val;
  }
  printf("%2d %10s  %15g : %15g\n",os,name.c_str(),minimum,maximum);
  double amplitude=(maximum-minimum)*1.05;
  if(amplitude!=0) Y = (Y-minimum)/amplitude+os;
  else Y=os;
}

/**********************************************************************
* Curve fitting
**********************************************************************/
#define FIT
#ifdef FIT
int fit=0;
string fitmode="expX";
double expXTau=0.0;
double expXPwr=1.0;
double expTau=0.0;
int    fitStart=0;
#endif

int samenu=0;
int samecount=0;

int normalize=0;

///////////////////////////////////////////////////////////////////////
double findtau(const Vector<double>& X,const Vector<double>& Y,int start) {
  double thresh = fabs(Y[start])*exp(-1);
  int n=Y.size();
  int i;
  for(i=start;i<n;i++) {
    if(fabs(Y[i])<thresh) return(X[i]-X[start]);
  }
  double tau=0.2;
  if(fabs(Y[start])>fabs(Y[n-1])) 
    tau = (X[n]-X[start])/log(Y[n]/Y[n-1])+X[start];
  return(tau);
}

///////////////////////////////////////////////////////////////////////
void curvefit(char* fname,const string& fitmode,TGraph* gr,const Vector<double>& Y1) {
  FILE *fp=fopen("CurveFit.txt","a");
  char form[500];
  int n = X.size();
  int start=fitStart;
  double Tau;
  if(Y1.size() && fitmode.size() && fabs(Y1[0])>fabs(Y1[Y1.size()-1])*2) {
     if(fitmode=="expX") {
       if(expXTau!=0) Tau=expXTau;
       else Tau=findtau(X,Y1,start);
       //gr[ga]->Fit("expo","","",0.0,2.0);
       //gr[ga]->Fit("pol6");
       sprintf(form,"expX((x-%g)/[0],[1])*%g",X[start],Y1[start]);
       printf("fitting %s [0]=%g [1]=%g ... \n",form,Tau,expXPwr);
       TF1 *f1=new TF1("f1",form,X[start],X[n-1]);
       f1->SetParameter(0,Tau);
       f1->SetParameter(1,expXPwr);
       f1->SetLineWidth(linewidth);
       f1->SetLineColor(kCyan);
       f1->SetMarkerColor(kCyan);
       f1->SetLineWidth(1);
       gr->Fit(f1,"Q");
       printf("Fit Y1=expX((x-%g)/(%g),%g)*%g  chi2=%g\n",X[start]
	       ,f1->GetParameter(0) ,f1->GetParameter(1),Y1[start]
	       ,f1->GetChisquare());
       fout.print(f1->GetParameter(0));
       fout.print(f1->GetParameter(1));
       fout.print(f1->GetChisquare());
       fout.print(Y1[start]);
       fout.print(Y1[Y1.size()-1]);
       if(fp) {
         fprintf(fp,"%s\n",fname);
         fprintf(fp," Y1=expX((x-%g)/(%g),%g)*%g  chi2=%g\n",X[start]
	         ,f1->GetParameter(0) ,f1->GetParameter(1),Y1[start]
	         ,f1->GetChisquare());
       }
     }
     else if(fitmode=="exp") {
       if(expTau!=0) Tau=expTau;
       else Tau=findtau(X,Y1,start);
       sprintf(form,"exp((x-%g)/[0])*%g",X[start],Y1[start]);
       printf("fitting %s [0]=%g ... \n",form,Tau);
       TF1 *f1=new TF1("f1",form,X[start],X[n-1]);
       f1->SetParameter(0,-Tau);
       f1->SetLineWidth(linewidth);
       f1->SetLineColor(kCyan);
       f1->SetMarkerColor(kCyan);
       f1->SetLineWidth(1);
       gr->Fit(f1,"Q");
       printf("Fit Y1=exp((x-%g)/(%g))*%g  chi2=%g\n",X[start]
               ,f1->GetParameter(0),Y1[0],f1->GetChisquare());
       fout.print(f1->GetParameter(0));
       fout.print(f1->GetChisquare());
       fout.print(Y1[start]);
       fout.print(Y1[Y1.size()-1]);
       if(fp) {
         fprintf(fp,"%s\n",fname);
         fprintf(fp,"Fit Y1=exp((x-%g)/(%g))*%g  chi2=%g\n",X[start]
               ,f1->GetParameter(0),Y1[0],f1->GetChisquare());
       }
     }
     else if(fitmode=="1/T") {
       printf("fitting 1/T... \n");
       sprintf(form,"1/pow(x+[0],[1])*[2]");
       TF1 *f1=new TF1("f1",form,X[0],X[n-1]);
       f1->SetParameter(0,.2);
       f1->SetParameter(1,1.0);
       f1->SetParameter(2,Y1[0]);
       f1->SetLineWidth(linewidth);
       f1->SetLineColor(kCyan);
       f1->SetMarkerColor(kCyan);
       f1->SetLineWidth(1);
       gr->Fit(f1,"Q");
       printf("Fit Y1=1/pow(x+%g,%g)*%g  chi2=%g\n"
               ,f1->GetParameter(0)
		,f1->GetParameter(1)
		,f1->GetParameter(2)
		,f1->GetChisquare());
       fout.print(f1->GetParameter(0));
       fout.print(f1->GetParameter(1));
       fout.print(f1->GetParameter(2));
       fout.print(f1->GetChisquare());
       fout.print(Y1[start]);
       fout.print(Y1[Y1.size()-1]);
       if(fp) {
         fprintf(fp,"%s\n",fname);
         fprintf(fp,"Fit Y1=1/pow(x+%g,%g)*%g  chi2=%g\n"
                ,f1->GetParameter(0)
		,f1->GetParameter(1)
		,f1->GetParameter(2)
		,f1->GetChisquare());
       }
     }
  }
  if(fp) fclose(fp);
}

/**********************************************************************
* Formula processing
**********************************************************************/

/////////////////////////////////////////////////////////////////////////////
void ExecFormula(const string& formula,const string& tgt=""
		,Vector<double>& x 
		,Vector<double>& y1 ,Vector<double>& y2
		,Vector<double>& y3 ,Vector<double>& y4
		,Vector<double>& y5 ,Vector<double>& y6
		,Vector<double>& y7 ,Vector<double>& y8
		) {
  string buf = tgt+formula;
  printf("Formula %s\n",buf.c_str());
  G__exec_text(buf.c_str());
  //if(G__lasterror_linenum()>0) {G__clearerror();}
}


/////////////////////////////////////////////////////////////////////////////
void setdisp(const string& y1in="", const string& y2in="",
             const string& y3in="", const string& y4in="",
             const string& y5in="", const string& y6in="",
             const string& y7in="", const string& y8in="") {
  y1=y1in; y2=y2in; y3=y3in; y4=y4in; y5=y5in; y6=y6in; y7=y7in; y8=y8in;
}

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// XY Graph
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

/**********************************************************************
* X-Y graph 
**********************************************************************/
//double yhigh =0;
//double ylow =0;
double tbegin =0;
double tend   =0;

#define MAXGRAPH 100

int xylog=0;

//string y1="S(A)";
//string y2="G(A)";
//string y3="C(A)";
//string y4="y1+y2+y3";
string y1;
string y2;
string y3;
string y4;
string y5;
string y6;
string y7;
string y8;
string y9;
string y10;
string y11;
string y12;


Vector<double> X;
Vector<double> Y1;
Vector<double> Y2;
Vector<double> Y3;
Vector<double> Y4;
Vector<double> Y5;
Vector<double> Y6;
Vector<double> Y7;
Vector<double> Y8;
Vector<double> Y9;
Vector<double> Y10;
Vector<double> Y11;
Vector<double> Y12;

string formula="";

// dummy
double A;
double C;
double J;

// Histogram の方が遅くmeritない

TCanvas *cgr[MAXGRAPH];
TH2F   *scale[MAXGRAPH];
TGraph *gr[MAXGRAPH];
TGraph *gr2[MAXGRAPH];
TGraph *gr3[MAXGRAPH];
TGraph *gr4[MAXGRAPH];
TGraph *gr5[MAXGRAPH];
TGraph *gr6[MAXGRAPH];
TGraph *gr7[MAXGRAPH];
TGraph *gr8[MAXGRAPH];
TMultiGraph *mg[MAXGRAPH];
int ga=0;

TPaveText *pt[MAXGRAPH];
TText *text[MAXGRAPH];
TLatex *tex[MAXGRAPH];

int noformula=0;
/////////////////////////////////////////////////////////////////////////////
int findsym(ReadF& f,string& s) {
  int i;
  for(i=1;i<=f.argc;i++) if(s==f.argv[i]) return(i);
  return(0);
}


///////////////////////////////////////////////////////////////////////
void ReadGraph(ReadF& f,char* fname,char* rmode,char* comment,int isbinary) {
  int i,target1=0,target2=0,target3=0,target4=0;
  int target5=0,target6=0,target7=0,target8=0;
  int target9=0,target10=0,target11=0,target12=0;
  int n;
  int isauto=0;
  printf("%s %s\n",rmode,comment);
#if 0
  if((match.size() && !strstr(comment,match.c_str())) ||
     (not.size() && strstr(comment,not.c_str()))) {
    while(f.read()) {
      if(f.argc>=1 && strcmp(f.argv[1],"END")==0) return;
    }
  }
#else
  if(strstr(comment,"sampled")) {
    while(f.read()) {
      if(f.argc>=1 && strcmp(f.argv[1],"END")==0) return;
    }
  }
#endif

  // time, (C),C(C),G(C),S(C), (A),C(A),G(A),S(A),
  f.read();
  string xname = f.argv[1];
  for(i=1;i<=f.argc;i++) {
    if(y1==f.argv[i]) target1 = i;
    if(y2==f.argv[i]) target2 = i;
    if(y3==f.argv[i]) target3 = i;
    if(y4==f.argv[i]) target4 = i;
    if(y5==f.argv[i]) target5 = i;
    if(y6==f.argv[i]) target6 = i;
    if(y7==f.argv[i]) target7 = i;
    if(y8==f.argv[i]) target8 = i;
    if(y9==f.argv[i]) target9 = i;
    if(y10==f.argv[i]) target10 = i;
    if(y11==f.argv[i]) target11 = i;
    if(y12==f.argv[i]) target12 = i;
    if(f.argv[i][0]) n = i;
  }
  if(target1==0) {
    if(f.argc<100) {
      isauto=1;
      target1=2; y1=f.argv[2];
      if(f.argc>2) { target2=3; y2=f.argv[3]; } else y2="";
      if(f.argc>3) { target3=4; y3=f.argv[4]; } else y3="";
      if(f.argc>4) { target4=5; y4=f.argv[5]; } else y4="";
      if(f.argc>5) { target5=6; y5=f.argv[6]; } else y5="";
      if(f.argc>6) { target6=7; y6=f.argv[7]; } else y6="";
      if(f.argc>7) { target7=8; y7=f.argv[8]; } else y7="";
      if(f.argc>8) { target8=9; y8=f.argv[9]; } else y8="";
      if(f.argc>9) { target8=9; y9=f.argv[10]; } else y9="";
      if(f.argc>10) { target8=9; y10=f.argv[11]; } else y10="";
      if(f.argc>11) { target8=9; y11=f.argv[12]; } else y11="";
      if(f.argc>12) { target8=9; y12=f.argv[13]; } else y12="";
    }
    else {
      target1=findsym(f,"S(A)"); if(target1) y1="S(A)";
      target2=findsym(f,"G(A)"); if(target2) y2="G(A)";
      target3=findsym(f,"C(A)"); if(target3) y3="C(A)";
    }
  }
  if(!target1) {
    printf("ReadGraph:: No data to display\n");
    return;
  }
  X.clear();
  Y1.clear();
  Y2.clear();
  Y3.clear();
  Y4.clear();
  Y5.clear();
  Y6.clear();
  Y7.clear();
  Y8.clear();
  Y9.clear();
  Y10.clear();
  Y11.clear();
  Y12.clear();
  double time,val;

  if(isbinary) {
    f.read(); // INCLUDE statement
    FILE *bfp;
    if(strcmp(f.argv[1],"INCLUDE")==0) {
      bfp = OpenBinaryFile(fname,f.argv[2]);    
      if(!bfp) return;
    }
    else if(strcmp(f.argv[1],"EMBEDDED")==0) {
      bfp = f.FP();
    }
    else {
      fprintf(stderr,"ERROR: Illegal binary file format\n");
      return;
    }

    double *dat = new double[n+2];
    // skip untile time>tbegin 
    size_t fstat;
    if(tbegin) {
      do {
        fstat = fread(dat,sizeof(double),n,bfp); 
      } while(fstat && dat[0]<tbegin);
    }
    for(;;) {
      fread(dat,sizeof(double),n,bfp);
      if(dat[0]== -1) break;
      X.push_back(dat[0]);
      Y1.push_back(dat[target1-1]);
      if(target2) Y2.push_back(dat[target2-1]);
      if(target3) Y3.push_back(dat[target3-1]);
      if(target4) Y4.push_back(dat[target4-1]);
      if(target5) Y5.push_back(dat[target5-1]);
      if(target6) Y6.push_back(dat[target6-1]);
      if(target7) Y7.push_back(dat[target7-1]);
      if(target8) Y8.push_back(dat[target8-1]);
      if(target9) Y9.push_back(dat[target9-1]);
      if(target10) Y10.push_back(dat[target10-1]);
      if(target11) Y11.push_back(dat[target11-1]);
      if(target12) Y12.push_back(dat[target12-1]);
      if(tend!=0 && dat[0]>tend) break;
    }
    if(bfp==f.FP() && dat[0]!= -1) {
      for(;;) {
        fstat = fread(dat,sizeof(double),n,bfp);
        if(!fstat || dat[0]== -1) break;
      }
    }
    delete dat;
    if(bfp && bfp!=f.FP()) fclose(bfp);
  }
  else {
    // skip untile time>tbegin 
    if(tbegin) {
      time=0;
      while(time<tbegin) {
        f.read(); 
        time=atof(f.argv[0]);
      }
    }
    while(f.read()) {
      if(!f.argc) continue;
      if(f.argv[1][0]=='/' || f.argv[1][0]=='#') continue;
      if(f.argc>=1 && strncmp(f.argv[1],"END",3)==0) {
        break;
      }
      time=atof(f.argv[0]);
      if(tend==0.0 || time<=tend) {
        X.push_back(time);
        Y1.push_back(atof(f.argv[target1]));
        if(target2) Y2.push_back(atof(f.argv[target2]));
        if(target3) Y3.push_back(atof(f.argv[target3]));
        if(target4) Y4.push_back(atof(f.argv[target4]));
        if(target5) Y5.push_back(atof(f.argv[target5]));
        if(target6) Y6.push_back(atof(f.argv[target6]));
        if(target7) Y7.push_back(atof(f.argv[target7]));
        if(target8) Y8.push_back(atof(f.argv[target8]));
        if(target9) Y9.push_back(atof(f.argv[target9]));
        if(target10) Y10.push_back(atof(f.argv[target10]));
        if(target11) Y11.push_back(atof(f.argv[target11]));
        if(target12) Y12.push_back(atof(f.argv[target12]));
      }
    }
  }


  if(!target1&&y1.size()) { ExecFormula(y1,"y1=",X,Y1,Y2,Y3,Y4,Y5,Y6,Y7,Y8); }
  if(!target2&&y2.size()) { ExecFormula(y2,"y2=",X,Y1,Y2,Y3,Y4,Y5,Y6,Y7,Y8); }
  if(!target3&&y3.size()) { ExecFormula(y3,"y3=",X,Y1,Y2,Y3,Y4,Y5,Y6,Y7,Y8); }
  if(!target4&&y4.size()) { ExecFormula(y4,"y4=",X,Y1,Y2,Y3,Y4,Y5,Y6,Y7,Y8); }
  if(!target5&&y5.size()) { ExecFormula(y5,"y5=",X,Y1,Y2,Y3,Y4,Y5,Y6,Y7,Y8); }
  if(!target6&&y6.size()) { ExecFormula(y6,"y6=",X,Y1,Y2,Y3,Y4,Y5,Y6,Y7,Y8); }
  if(!target7&&y7.size()) { ExecFormula(y7,"y7=",X,Y1,Y2,Y3,Y4,Y5,Y6,Y7,Y8); }
  if(!target8&&y8.size()) { ExecFormula(y8,"y8=",X,Y1,Y2,Y3,Y4,Y5,Y6,Y7,Y8); }
  //G__tracemode(1);
  if(formula.size()) ExecFormula(formula,"",X,Y1,Y2,Y3,Y4,Y5,Y6,Y7,Y8);
  //G__tracemode(0);

  if(ave>1) movingAve();

  if(normalize) normalization();

  if(ga>MAXGRAPH-5) {
    fprintf(stderr,"Warning: Too many graphs\n");
  }
  else if(!nowaveform) {
    if(samenu) samegraph(fname,comment,xname);
    else     graph(fname,comment,xname);
  }

  f.read(); // END

  if(isauto) {
    y1="";
    y2=y3=y4=y5=y6=y7=y8="";
  }
}

///////////////////////////////////////////////////////////////////////
EColor colorTable[] = { kBlack , kRed  , kBlue , kGreen , kMagenta ,
  , kCyan , kYellow 
  , 8 , 9 /*,10 */ ,11 , 12, 13,14, 15 , 16 
  //, kGray , kOrange , kSpring , kTeal , kAzure , kViolet , kPink 
  , -1 };
////////////////////////////////////////////////////////////////////////
class Colors_t {
 public:
  Colors() { colorindex=0; }
  void init() { colorindex=0;}
  enum EColor Get(int inc=0) { 
    EColor result= colorTable[colorindex];
    colorindex+=inc;
    if(colorTable[colorindex]<0) colorindex=0;
    return(result);
  }
 private:
  int colorindex;
} Colors;

#ifdef CINTPLOT
///////////////////////////////////////////////////////////////////////
// X-Y graph 表示
///////////////////////////////////////////////////////////////////////
void graph(char* fname="",char* comment="",const string& xname="") {
  cintgraph(fname,comment,xname);
  plot << glower >> gupper << "A\n";
}

///////////////////////////////////////////////////////////////////////
// X-Y graph 表示 重ねがき
///////////////////////////////////////////////////////////////////////
int lastsamega;
char lastfname[200];
char lastcomment[200];
void samegraph(char* fname="",char* comment="",const string& xname="") {
  cintgraph(fname,comment,xname);

  int samega = samecount/samenu;
  if(samecount%samenu==samenu-1) {
    plot << glower >> gupper << "A\n";
  }
  else {
    plot << glower >> gupper << flush;
  }
  samecount++;
}

///////////////////////////////////////////////////////////////////////
// CINTPLOT driver
///////////////////////////////////////////////////////////////////////
void cintgraph(char* fname="",char* comment="",const string& xname="") {
  array a_x(&X[0],X.size());
  plot << fname << a_x << "sec" ;
  if(Y1.size()) {
    array a_y1(&Y1[0],Y1.size());
    plot << a_y1 << y1.c_str() ;
  }
  if(Y2.size()) {
    array a_y2(&Y2[0],Y2.size());
    plot << a_y2 << y2.c_str() ;
  }
  if(Y3.size()) {
    array a_y3(&Y3[0],Y3.size());
    plot << a_y3 << y3.c_str() ;
  }
  if(Y4.size()) {
    array a_y4(&Y4[0],Y4.size());
    plot << a_y4 << y4.c_str() ;
  }
  if(Y5.size()) {
    array a_y5(&Y5[0],Y5.size());
    plot << a_y5 << y5.c_str() ;
  }
  if(Y6.size()) {
    array a_y6(&Y6[0],Y6.size());
    plot << a_y6 << y6.c_str() ;
  }
  if(Y7.size()) {
    array a_y7(&Y7[0],Y7.size());
    plot << a_y7 << y7.c_str() ;
  }
  if(Y8.size()) {
    array a_y8(&Y8[0],Y8.size());
    plot << a_y8 << y8.c_str() ;
  }
  if(Y9.size()) {
    array a_y9(&Y9[0],Y9.size());
    plot << a_y9 << y9.c_str() ;
  }
  if(Y10.size()) {
    array a_y10(&Y10[0],Y10.size());
    plot << a_y10 << y10.c_str() ;
  }
  if(Y11.size()) {
    array a_y11(&Y11[0],Y11.size());
    plot << a_y11 << y11.c_str() ;
  }
  if(Y12.size()) {
    array a_y12(&Y12[0],Y12.size());
    plot << a_y12 << y12.c_str() ;
  }

}

#else // CINTPLOT
///////////////////////////////////////////////////////////////////////
// X-Y graph 表示
///////////////////////////////////////////////////////////////////////
void graph(char* fname="",char* comment="",const string& xname="") {

  mg[ga] = new TMultiGraph;
  if(Y1.size()) {
    gr[ga] = new TGraph(X.size(),&X[0],&Y1[0]);
    gr[ga]->SetLineWidth(linewidth);
    gr[ga]->SetLineColor(kBlack);
    gr[ga]->SetMarkerColor(kBlack);
    if(fname[0]&& fit==1) curvefit(fname,fitmode,gr[ga],Y1);
  }
  if(Y2.size()) {
    gr2[ga] = new TGraph(X.size(),&X[0],&Y2[0]);
    gr2[ga]->SetLineWidth(linewidth);
    gr2[ga]->SetLineColor(kRed);
    gr2[ga]->SetMarkerColor(kRed);
    if(fname[0]&& fit==2) curvefit(fname,fitmode,gr2[ga],Y2);
  }
  if(Y3.size()) {
    gr3[ga] = new TGraph(X.size(),&X[0],&Y3[0]);
    gr3[ga]->SetLineWidth(linewidth);
    gr3[ga]->SetLineColor(kBlue);
    gr3[ga]->SetMarkerColor(kBlue);
    if(fname[0]&& fit==3) curvefit(fname,fitmode,gr3[ga],Y3);
  }
  if(Y4.size()) {
    gr4[ga] = new TGraph(X.size(),&X[0],&Y4[0]);
    gr4[ga]->SetLineWidth(linewidth);
    gr4[ga]->SetLineColor(kGreen);
    gr4[ga]->SetMarkerColor(kGreen);
    if(fname[0]&& fit==4) curvefit(fname,fitmode,gr4[ga],Y4);
  }
  if(Y5.size()) {
    gr5[ga] = new TGraph(X.size(),&X[0],&Y5[0]);
    gr5[ga]->SetLineWidth(linewidth);
    gr5[ga]->SetLineColor(kMagenta);
    gr5[ga]->SetMarkerColor(kMagenta);
    if(fname[0]&& fit==5) curvefit(fname,fitmode,gr5[ga],Y5);
  }
  if(Y6.size()) {
    gr6[ga] = new TGraph(X.size(),&X[0],&Y6[0]);
    gr6[ga]->SetLineWidth(linewidth);
    gr6[ga]->SetLineColor(kCyan);
    gr6[ga]->SetMarkerColor(kCyan);
    if(fname[0]&& fit==6) curvefit(fname,fitmode,gr6[ga],Y6);
  }
  if(Y7.size()) {
    gr7[ga] = new TGraph(X.size(),&X[0],&Y7[0]);
    gr7[ga]->SetLineWidth(linewidth);
    gr7[ga]->SetLineColor(kYellow);
    gr7[ga]->SetMarkerColor(kYellow);
    if(fname[0]&& fit==7) curvefit(fname,fitmode,gr7[ga],Y7);
  }
  if(Y8.size()) {
    gr8[ga] = new TGraph(X.size(),&X[0],&Y8[0]);
    gr8[ga]->SetLineWidth(linewidth);
    gr8[ga]->SetLineColor(kBlack);
    gr8[ga]->SetMarkerColor(kBlack);
    if(fname[0]&& fit==8) curvefit(fname,fitmode,gr8[ga],Y8);
  }
  if(Y9.size()) {
    gr9[ga] = new TGraph(X.size(),&X[0],&Y9[0]);
    gr9[ga]->SetLineWidth(linewidth);
    gr9[ga]->SetLineColor(kRed);
    gr9[ga]->SetMarkerColor(kRed);
    if(fname[0]&& fit==9) curvefit(fname,fitmode,gr9[ga],Y9);
  }
  if(Y10.size()) {
    gr10[ga] = new TGraph(X.size(),&X[0],&Y10[0]);
    gr10[ga]->SetLineWidth(linewidth);
    gr10[ga]->SetLineColor(kBlue);
    gr10[ga]->SetMarkerColor(kBlue);
    if(fname[0]&& fit==10) curvefit(fname,fitmode,gr10[ga],Y10);
  }
  if(Y11.size()) {
    gr11[ga] = new TGraph(X.size(),&X[0],&Y11[0]);
    gr11[ga]->SetLineWidth(linewidth);
    gr11[ga]->SetLineColor(kGreen);
    gr11[ga]->SetMarkerColor(kGreen);
    if(fname[0]&& fit==11) curvefit(fname,fitmode,gr11[ga],Y11);
  }
  if(Y12.size()) {
    gr12[ga] = new TGraph(X.size(),&X[0],&Y12[0]);
    gr12[ga]->SetLineWidth(linewidth);
    gr12[ga]->SetLineColor(kMagenta);
    gr12[ga]->SetMarkerColor(kMagenta);
    if(fname[0]&& fit==12) curvefit(fname,fitmode,gr12[ga],Y12);
  }

  char buf[1000];
  char id[500];
  sprintf(buf,"%s:%s %s %s %s:r %s:b %s:g %s:m %s:c %s:y %s:bl",fname,comment
         ,xname.c_str()
         ,y1.c_str(),y2.c_str()
	 ,y3.c_str(),y4.c_str()
	 ,y5.c_str(),y6.c_str()
	 ,y7.c_str(),y8.c_str()
         );

  //mg[ga]->SetTitle(buf);
  //printf("%d %x %x \n",ga, gr[ga],gr[ga]->GetXaxis());
  gr[ga]->GetXaxis()->SetTitle("time (s)");
  gr[ga]->GetYaxis()->SetTitle("Current (A)");

  sprintf(id,"%s_g%d",fname,ga);
  //sprintf(buf,"Time domain data %s",s.c_str());
  cgr[ga] = new TCanvas(id,buf,500+ga*20,ga*20+20,600,400);
  cgr[ga]->SetGrid();

  if(Y1.size()) mg[ga]->Add(gr[ga]);
  if(Y2.size()) mg[ga]->Add(gr2[ga]);
  if(Y3.size()) mg[ga]->Add(gr3[ga]);
  if(Y4.size()) mg[ga]->Add(gr4[ga]);
  if(Y5.size()) mg[ga]->Add(gr5[ga]);
  if(Y6.size()) mg[ga]->Add(gr6[ga]);
  if(Y7.size()) mg[ga]->Add(gr7[ga]);
  if(Y8.size()) mg[ga]->Add(gr8[ga]);
  if(Y9.size()) mg[ga]->Add(gr9[ga]);
  if(Y10.size()) mg[ga]->Add(gr10[ga]);
  if(Y11.size()) mg[ga]->Add(gr11[ga]);
  if(Y12.size()) mg[ga]->Add(gr12[ga]);
  mg[ga]->Draw("ALP");

#if 1
  double xmin= mg[ga]->GetXaxis()->GetXmin();
  double xmax= mg[ga]->GetXaxis()->GetXmax();
  double xdif=xmax-xmin;
  double ymin= mg[ga]->GetYaxis()->GetXmin();
  double ymax= mg[ga]->GetYaxis()->GetXmax();
  double ydif=ymax-ymin;
  // top title
  pt[ga] = new TPaveText(xmin+xdif/10,ymax+ydif/50,xmax,ymax+ydif/9,"br");
  pt[ga]->SetFillColor(19);
  text[ga] = pt[ga]->AddText(fname);
  pt[ga]->Draw();
  // low comment
  tex[ga] = new TLatex(xmax-xdif/60*strlen(comment),ymin-ydif/10,comment);
  tex[ga]->SetLineWidth(2);
  tex[ga]->Draw();
  // x axis unit
  tex[ga] = new TLatex(xmin+xdif*0.45,ymin-ydif/15,"(sec)");
  tex[ga]->SetLineWidth(2);
  tex[ga]->Draw();
  // y axis unit
  tex[ga] = new TLatex(xmin-xdif/10,ymin+ydif/2,"(A)");
  tex[ga]->SetLineWidth(2);
  tex[ga]->Draw();
  if(Cmeas) {
    // measured capacitance
    char capbuf[80];
    sprintf(capbuf,"%gF",Cmeas);
    tex[ga] = new TLatex(xmax-xdif/4,ymax-ydif/10,capbuf);
    tex[ga]->SetLineWidth(2);
    tex[ga]->Draw();
    Cmeas=0;
  }
  // draw
  cgr[ga]->Modified();
  cgr[ga]->cd();
  cgr[ga]->SetSelected(cgr[ga]);
#endif

  ++ga;
}


///////////////////////////////////////////////////////////////////////
// X-Y graph 表示 重ねがき
////////////////////////////////////////////////////////////////////////
int lastsamega;
char lastfname[200];
char lastcomment[200];
void samegraph(char* fname="",char* comment="",const string& xname="") {
  int samega = samecount/samenu;
  if(samecount%samenu==0) {
    mg[samega] = new TMultiGraph;
    Colors.init();
  }
  if(Y1.size()) {
    gr[ga] = new TGraph(X.size(),&X[0],&Y1[0]);
    gr[ga]->SetLineWidth(linewidth);
    gr[ga]->SetLineColor(Colors.Get());
    gr[ga]->SetMarkerColor(Colors.Get(1));
    if(fname[0]&& fit==1) curvefit(fname,fitmode,gr[ga],Y1);
  }
  if(Y2.size()) {
    gr2[ga] = new TGraph(X.size(),&X[0],&Y2[0]);
    gr2[ga]->SetLineWidth(linewidth);
    gr2[ga]->SetLineColor(Colors.Get());
    gr2[ga]->SetMarkerColor(Colors.Get(1));
    if(fname[0]&& fit==2) curvefit(fname,fitmode,gr2[ga],Y2);
  }
  if(Y3.size()) {
    gr3[ga] = new TGraph(X.size(),&X[0],&Y3[0]);
    gr3[ga]->SetLineWidth(linewidth);
    gr3[ga]->SetLineColor(Colors.Get());
    gr3[ga]->SetMarkerColor(Colors.Get(1));
    if(fname[0]&& fit==3) curvefit(fname,fitmode,gr3[ga],Y3);
  }
  if(Y4.size()) {
    gr4[ga] = new TGraph(X.size(),&X[0],&Y4[0]);
    gr4[ga]->SetLineWidth(linewidth);
    gr4[ga]->SetLineColor(Colors.Get());
    gr4[ga]->SetMarkerColor(Colors.Get(1));
    if(fname[0]&& fit==4) curvefit(fname,fitmode,gr4[ga],Y4);
  }
  if(Y5.size()) {
    gr5[ga] = new TGraph(X.size(),&X[0],&Y5[0]);
    gr5[ga]->SetLineWidth(linewidth);
    gr5[ga]->SetLineColor(Colors.Get());
    gr5[ga]->SetMarkerColor(Colors.Get(1));
    if(fname[0]&& fit==5) curvefit(fname,fitmode,gr5[ga],Y5);
  }
  if(Y6.size()) {
    gr6[ga] = new TGraph(X.size(),&X[0],&Y6[0]);
    gr6[ga]->SetLineWidth(linewidth);
    gr6[ga]->SetLineColor(Colors.Get());
    gr6[ga]->SetMarkerColor(Colors.Get(1));
    if(fname[0]&& fit==6) curvefit(fname,fitmode,gr6[ga],Y6);
  }
  if(Y7.size()) {
    gr7[ga] = new TGraph(X.size(),&X[0],&Y7[0]);
    gr7[ga]->SetLineWidth(linewidth);
    gr7[ga]->SetLineColor(Colors.Get());
    gr7[ga]->SetMarkerColor(Colors.Get(1));
    if(fname[0]&& fit==7) curvefit(fname,fitmode,gr7[ga],Y7);
  }
  if(Y8.size()) {
    gr8[ga] = new TGraph(X.size(),&X[0],&Y8[0]);
    gr8[ga]->SetLineWidth(linewidth);
    gr8[ga]->SetLineColor(Colors.Get());
    gr8[ga]->SetMarkerColor(Colors.Get(1));
    if(fname[0]&& fit==8) curvefit(fname,fitmode,gr8[ga],Y8);
  }
  if(Y9.size()) {
    gr9[ga] = new TGraph(X.size(),&X[0],&Y9[0]);
    gr9[ga]->SetLineWidth(linewidth);
    gr9[ga]->SetLineColor(kBlack);
    gr9[ga]->SetMarkerColor(kBlack);
    if(fname[0]&& fit==9) curvefit(fname,fitmode,gr9[ga],Y9);
  }
  if(Y10.size()) {
    gr10[ga] = new TGraph(X.size(),&X[0],&Y10[0]);
    gr10[ga]->SetLineWidth(linewidth);
    gr10[ga]->SetLineColor(kBlack);
    gr10[ga]->SetMarkerColor(kBlack);
    if(fname[0]&& fit==10) curvefit(fname,fitmode,gr10[ga],Y10);
  }
  if(Y11.size()) {
    gr11[ga] = new TGraph(X.size(),&X[0],&Y11[0]);
    gr11[ga]->SetLineWidth(linewidth);
    gr11[ga]->SetLineColor(kBlack);
    gr11[ga]->SetMarkerColor(kBlack);
    if(fname[0]&& fit==11) curvefit(fname,fitmode,gr11[ga],Y11);
  }
  if(Y12.size()) {
    gr12[ga] = new TGraph(X.size(),&X[0],&Y12[0]);
    gr12[ga]->SetLineWidth(linewidth);
    gr12[ga]->SetLineColor(kBlack);
    gr12[ga]->SetMarkerColor(kBlack);
    if(fname[0]&& fit==12) curvefit(fname,fitmode,gr12[ga],Y12);
  }

  char buf[1000];
  char id[500];
  sprintf(buf,"%s:%s %s %s %s:r %s:b %s:g %s:m %s:c %s:y %s:bl",fname,comment
         ,xname.c_str()
         ,y1.c_str(),y2.c_str()
	 ,y3.c_str(),y4.c_str()
	 ,y5.c_str(),y6.c_str()
	 ,y7.c_str(),y8.c_str()
         );

  //mg[ga]->SetTitle(buf);
  //printf("%d %x %x \n",ga, gr[ga],gr[ga]->GetXaxis());
  gr[ga]->GetXaxis()->SetTitle("time (s)");
  gr[ga]->GetYaxis()->SetTitle("Current (A)");

  sprintf(id,"%s_g%ds%d",fname,ga,samenu);
  //sprintf(buf,"Time domain data %s",s.c_str());
  if(samecount%samenu==0) {
    cgr[samega] = new TCanvas(id,buf,600+samega*20,samega*20+20,600,400);
    cgr[samega]->SetGrid();
  }
  cgr[samega]->cd();

  if(Y1.size()) mg[samega]->Add(gr[ga]);
  if(Y2.size()) mg[samega]->Add(gr2[ga]);
  if(Y3.size()) mg[samega]->Add(gr3[ga]);
  if(Y4.size()) mg[samega]->Add(gr4[ga]);
  if(Y5.size()) mg[samega]->Add(gr5[ga]);
  if(Y6.size()) mg[samega]->Add(gr6[ga]);
  if(Y7.size()) mg[samega]->Add(gr7[ga]);
  if(Y8.size()) mg[samega]->Add(gr8[ga]);
  if(Y9.size()) mg[samega]->Add(gr9[ga]);
  if(Y10.size()) mg[samega]->Add(gr10[ga]);
  if(Y11.size()) mg[samega]->Add(gr11[ga]);
  if(Y12.size()) mg[samega]->Add(gr12[ga]);
  mg[samega]->Draw("ALP");

#if 1
  if(samecount%samenu==samenu-1) {
    double xmin= mg[samega]->GetXaxis()->GetXmin();
    double xmax= mg[samega]->GetXaxis()->GetXmax();
    double xdif=xmax-xmin;
    double ymin= mg[samega]->GetYaxis()->GetXmin();
    double ymax= mg[samega]->GetYaxis()->GetXmax();
    double ydif=ymax-ymin;
    // top title
    pt[samega] = new TPaveText(xmin+xdif/10,ymax+ydif/50,xmax,ymax+ydif/9,"br");
    pt[samega]->SetFillColor(19);
    text[samega] = pt[samega]->AddText(fname);
    pt[samega]->Draw();
    // bottom comment
    tex[samega] = new TLatex(xmax-xdif/50*strlen(comment),ymin-ydif/10,comment);
    tex[samega]->SetLineWidth(2);
    tex[samega]->Draw();
    // x axis unit
    tex[samega] = new TLatex(xmin+xdif*0.45,ymin-ydif/15,"(sec)");
    tex[samega]->SetLineWidth(2);
    tex[samega]->Draw();
    // y axis unit
    tex[samega] = new TLatex(xmin-xdif/10,ymin+ydif/2,"(A)");
    tex[samega]->SetLineWidth(2);
    tex[samega]->Draw();
    if(Cmeas) {
      // measured capacitance
      char capbuf[80];
      sprintf(capbuf,"%gF",Cmeas);
      tex[samega] = new TLatex(xmax-xdif/4,ymax-ydif/10,capbuf);
      tex[samega]->SetLineWidth(2);
      tex[samega]->Draw();
      Cmeas=0;
    }
    // draw
    cgr[samega]->Modified();
    cgr[samega]->cd();
    cgr[samega]->SetSelected(cgr[ga]);
  }
//printf("samega=%d\n",samega);
  lastsamega = samega;
  strcpy(lastfname,fname);
  strcpy(lastcomment,comment);
#endif

  ++ga;
  samecount++;
}
#endif // CINTPLOT


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// 2D/3D Visualization
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
#define MAXCANVAS 50
TH1D *h1[MAXCANVAS];
TH2D *h2[MAXCANVAS];
TH3D *h3[MAXCANVAS];
TCanvas *c[MAXCANVAS];
TPad *pad[MAXCANVAS];
int a=0;
//vector<double> HX,HY,HZ; // nonuniform mesh size information
TGraph2D *g2d[MAXCANVAS];

double hxmin,hxmax;
double hymin,hymax;
double hzmin,hzmax;
void SetHistRange(double xmin,double xmax
		 ,double ymin=0,double ymax=0
		 ,double zmin=0,double zmax=0) {
  hxmin=xmin;
  hxmax=xmax;
  hymin=ymin;
  hymax=ymax;
  hzmin=zmin;
  hzmax=zmax;
}


//int maxrow,maxcol,maxver;

// TCanvas
int wx=800,wy=700;
int xoffset=0,yoffset=0; // 表示Windowの位置

double upper=0.0;
double lower=0.0;
double histlog=0.0;

double rx,ry,rz;
int x,y,z; // cross section
int step=0,max=0;

// SCAT BOX ARR 
// COLZ CONTZ CONT1-3 
// LEGO LEGO1-2 
// POL SPH PSR 
// SURF1-4 SURFZ
//string draw="surf1z";
//string opt="surf1z";
//string opt1="C"; // smooth line
string opt1="L"; // straight line
string opt2="surf1";
string opt3="surf1";

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// Transform from non-uniform mesh to uniform mesh
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
int remesh=0;

/**********************************************************************
* Remesh
**********************************************************************/
void Remesh2D(Matrix2<double>& Do,double ho
             ,const Matrix2<double>& Di
             ,double hi
	     ,const vector<double>& HX,const vector<double>& HY) {
  if(0==HX.size() || 0==HY.size()) {
    Do = Di;
    return;
  }
  int i,j;
  double xw,yw;
  int xsize,ysize;
  if(hi==0.0) hi=HX.front();
  if(ho==0.0) ho=hi;
  if(HX.size()) xw = HX.back();
  else          xw = hi*(Di.XSize()-1);
  if(HY.size()) yw = HY.back();
  else          yw = hi*(Di.YSize()-1);
  xsize = xw/ho+1;
  ysize = yw/ho+1;

  vector<double> axis;
  double pos=0;
  axis.push_back(0);
  for(i=0;i<HX.size();i++) { 
    pos += HX[i];
    axis.push_back(pos);
  }

  Matrix2<double> Dt;
  Dt.Allocate(xsize,Di.YSize());

  int ii=0;
  double xo, k1,k2,;
  for(i=0;i<xsize;i++) {
    xo=ho*i;
    while(xo>axis[ii] && ii<Di.XSize()) ++ii;
    if(xo==axis[ii]) { 
      for(j=0;j<Di.YSize();j++) {
        Dt(i,j) = Di(ii,j);
      }
      ++ii;
    }
    else if(xo<axis[ii]) {
      //           HX[ii-1]
      // in   +axis(ii-1)       +axis(ii)
      // out  +      +xo     +
      k1 = (xo-axis[ii-1])/HX[ii-1];
      k2 = (axis[ii]-xo)/HX[ii-1];
      for(j=0;j<Di.YSize();j++) {
        Dt(i,j) = Di(ii-1,j)*k2+Di(ii,j)*k1;
      }
    }
    else {
      break;
    }
  }

  if(ysize<HY.size()+2) {
    Do = Dt;
    printf("Remesh %d,%d -> %d,%d\n",Di.XSize(),Di.YSize(),Do.XSize(),Do.YSize());
    return;
  }

  axis.clear();
  pos=0;
  axis.push_back(0);
  for(j=0;j<HY.size();j++) { 
    pos += HY[j];
    axis.push_back(pos);
  }

  Do.Allocate(xsize,ysize);
  int jj=0;
  double yo;
  for(j=0;j<ysize;j++) {
    yo=ho*j;
    while(yo>axis[jj] && jj<Di.YSize()) ++jj;
    if(yo==axis[jj]) { 
      for(i=0;i<xsize;i++) {
        Do(i,j) = Dt(i,jj);
      }
      ++jj;
    }
    else if(yo<axis[jj]) {
      //           HY[jj-1]
      // in   +axis(jj-1)       +axis(jj)
      // out  +      +yo     +
      k1 = (yo-axis[jj-1])/HY[jj-1];
      k2 = (axis[jj]-yo)/HY[jj-1];
      for(i=0;i<xsize;i++) {
        Do(i,j) = Dt(i,jj-1)*k2+Dt(i,jj)*k1;
      }
    }
    else {
      break;
    }
  }

  printf("Remesh %d,%d -> %d,%d\n",Di.XSize(),Di.YSize(),Do.XSize(),Do.YSize());

}

/**********************************************************************
* Remesh
**********************************************************************/
void Remesh3D(Matrix3<double>& Do,double ho
             ,const Matrix3<double>& Di
             ,double hi
	     ,const vector<double>& HX,const vector<double>& HY
	     ,const vector<double>& HZ) {
  // THIS FUNCTION IS NOT READY YET
  if(0==HX.size() || 0==HY.size() || 0==HZ.size()) {
    Do = Di;
    return;
  }
  int i,j,v;
  double xw,yw,zw;
  int xsize,ysize,zsize;
  if(hi==0.0) hi=HX.front();
  if(ho==0.0) ho=hi;
  if(HX.size()) xw = HX.back();
  else          xw = hi*(Di.XSize()-1);
  if(HY.size()) yw = HY.back();
  else          yw = hi*(Di.YSize()-1);
  if(HZ.size()) zw = HZ.back();
  else          zw = hi*(Di.ZSize()-1);
  xsize = xw/ho+1;
  ysize = yw/ho+1;
  zsize = zw/ho+1;

  vector<double> axis;
  double pos=0;
  axis.push_back(0);
  for(i=0;i<HX.size();i++) { 
    pos += HX[i];
    axis.push_back(pos);
  }

  Matrix3<double> Dt;
  Dt.Allocate(xsize,Di.YSize(),Di.ZSize());

  int ii=0;
  double xo, k1,k2,;
  for(i=0;i<xsize;i++) {
    xo=ho*i;
    while(xo>axis[ii] && ii<Di.XSize()-1) ++ii;
    if(xo==axis[ii]) { 
      for(j=0;j<Di.YSize();j++) {
        for(v=0;v<Di.ZSize();v++) {
          Dt(i,j,v) = Di(ii,j,v);
        }
      }
      if(ii<Di.XSize()-1) ++ii;
    }
    else if(xo<axis[ii]) {
      //           HX[ii-1]
      // in   +axis(ii-1)       +axis(ii)
      // out  +      +xo     +
      k1 = (xo-axis[ii-1])/HX[ii-1];
      k2 = (axis[ii]-xo)/HX[ii-1];
      for(j=0;j<Di.YSize();j++) {
        for(v=0;v<Di.ZSize();v++) {
          Dt(i,j,v) = Di(ii-1,j,v)*k2+Di(ii,j,v)*k1;
        }
      }
    }
    else {
      break;
    }
  }

  if(ysize<HY.size()+2 && zsize<HZ.size()+2) {
    Do = Dt;
    printf("Remesh %d,%d,%d -> %d,%d,%d\n"
            ,Di.XSize(),Di.YSize(),Di.ZSize()
            ,Do.XSize(),Do.YSize(),Do.ZSize());
    return;
  }

  axis.clear();
  pos=0;
  axis.push_back(0);
  for(j=0;j<HY.size();j++) { 
    pos += HY[j];
    axis.push_back(pos);
  }

  Matrix3<double> Ds;
  Ds.Allocate(xsize,ysize,Di.ZSize());
  int jj=0;
  double yo;
  for(j=0;j<ysize;j++) {
    yo=ho*j;
    while(yo>axis[jj] && jj<Di.YSize()-1) ++jj;
    if(yo==axis[jj]) { 
      for(i=0;i<xsize;i++) {
        for(v=0;v<Di.ZSize();v++) {
          Ds(i,j,v) = Dt(i,jj,v);
        }
      }
      if(jj<Di.YSize()-1) ++jj;
    }
    else if(yo<axis[jj]) {
      //           HY[jj-1]
      // in   +axis(jj-1)       +axis(jj)
      // out  +      +yo     +
      k1 = (yo-axis[jj-1])/HY[jj-1];
      k2 = (axis[jj]-yo)/HY[jj-1];
      for(i=0;i<xsize;i++) {
        for(v=0;v<Di.ZSize();v++) {
          Ds(i,j,v) = Dt(i,jj-1,v)*k2+Dt(i,jj,v)*k1;
        }
      }
    }
    else {
      break;
    }
  }

  if(zsize<HZ.size()+2) {
    Ds = Dt;
    printf("Remesh %d,%d,%d -> %d,%d,%d\n"
            ,Di.XSize(),Di.YSize(),Di.ZSize()
            ,Do.XSize(),Do.YSize(),Do.ZSize());
    return;
  }

  axis.clear();
  pos=0;
  axis.push_back(0);
  for(v=0;v<HZ.size();v++) { 
    pos += HZ[v];
    axis.push_back(pos);
  }

  Do.Allocate(xsize,ysize,zsize);
  int vv=0;
  double zo;
  for(v=0;v<zsize;v++) {
    zo=ho*v;
    while(zo>axis[vv] && vv<Di.ZSize()-1) ++vv;
    if(zo==axis[vv]) { 
      for(i=0;i<xsize;i++) {
        for(j=0;j<ysize;j++) {
          Do(i,j,v) = Ds(i,j,vv);
        }
      }
      if(vv<Di.ZSize()-1) ++vv;
    }
    else if(zo<axis[vv]) {
      //           HZ[vv-1]
      // in   +axis(vv-1)       +axis(vv)
      // out  +      +zo     +
      k1 = (zo-axis[vv-1])/HZ[vv-1];
      k2 = (axis[vv]-zo)/HZ[vv-1];
      for(i=0;i<xsize;i++) {
        for(j=0;j<ysize;j++) {
          Do(i,j,v) = Ds(i,j,vv-1)*k2+Ds(i,j,vv)*k1;
        }
      }
    }
    else {
      break;
    }
  }

  printf("Remesh %d,%d,%d -> %d,%d,%d\n"
          ,Di.XSize(),Di.YSize(),Di.ZSize()
          ,Do.XSize(),Do.YSize(),Do.ZSize());

}
//

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// Preprocessing non-uniform histogram for TGraph2D (NOT USED NOW)
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
/**********************************************************************
* 2D Graph Data処理
**********************************************************************/

/**********************************************************************
* 2D Graph Data処理
**********************************************************************/
void Graph2D(Matrix2<double>& D,char* fname,char* rmode,char* comment
                ,double llim,double ulim
                ,vector<double>& HX,vector<double>& HY) {

  if(a>MAXCANVAS-10) {
    fprintf(stderr,"Warning: Too many displays\n");
    return;
  }

  if((dx || dy) && strstr(comment,"Charge XD")) {
    Matrix1<double> D1;
    double A;
    DiffusionAnalysis2(D,D1,A,dx,dy);
    WaveformSynthesis1D(D1,A,2e-6,h,0.1,fname,comment);
#if 1
    printf("Xsize=%d Ysize=%d\n",D.XSize(),D.YSize());
    DispGraph(D1,fname,rmode,comment);
#else
    DispHistogram1D(D1,fname,rmode,comment,llim,ulim);
#endif
    //return;
  }

  setxyz(D);
  if(x||y) {
    Matrix1<double> D1;
    ReduceDimention2(D,D1,x,y);
    vector<double> *p;
    if(x) p = &HY;
    else  p = &HX;
    DispGraph1D(D1,fname,rmode,comment,llim,ulim,*p);
  }
  else {
    DispGraph2D(D,fname,rmode,comment,llim,ulim,HX,HY);
  }
  resetxyz();
}
/**********************************************************************
* 3D Graph Data処理
**********************************************************************/
void Graph3D(Matrix3<double>& D,char* fname,char* rmode,char* comment
                ,double llim,double ulim 
                ,vector<double>& HX,vector<double>& HY,vector<double>& HZ) {

  if(a>MAXCANVAS-10) {
    fprintf(stderr,"Warning: Too many displays\n");
    return;
  }

  setxyz(D);
  if((x&&y) || (y&&z) || (z&&x)) {
    Matrix1<double> D1;
    ReduceDimention3(D,D1,x,y,z);
    vector<double> *p;
    if(x&&y) p = &HZ;
    else if(y&&z) p = &HX;
    else if(x&&z) p = &HY;
    DispGraph1D(D1,fname,rmode,comment,llim,ulim,*p);
  }
  else if(x||y||z) {
    Matrix2<double> D2;
    ReduceDimention3(D,D2,x,y,z);
    vector<double> *px,*py;
    if(x) { px = &HY; py= &HZ; }
    if(y) { px = &HX; py= &HZ; }
    if(z) { px = &HX; py= &HY; }
    DispGraph2D(D2,fname,rmode,comment,llim,ulim,*px,*py);
  }
  else {
    DispGraph3D(D,fname,rmode,comment,llim,ulim,HX,HY,HZ);
  }
  resetxyz();
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// Preprocessing Historgram display
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
/**********************************************************************
* 2D Histogram Data処理
**********************************************************************/
void Disp2D(Matrix2<double>& D,char* fname,char* rmode,char* comment
                ,double llim,double ulim) {

  if(a>MAXCANVAS-10) {
    fprintf(stderr,"Warning: Too many displays\n");
    return;
  }

  if((dx || dy) && strstr(comment,"Charge XD")) {
    Matrix1<double> D1;
    double A;
    DiffusionAnalysis2(D,D1,A,dx,dy);
    WaveformSynthesis1D(D1,A,2e-6,h,0.1,fname,comment);
#if 1
    printf("Xsize=%d Ysize=%d\n",D.XSize(),D.YSize());
    DispGraph(D1,fname,rmode,comment);
#else
    DispHistogram1D(D1,fname,rmode,comment,llim,ulim);
#endif
    //return;
  }

  setxyz(D);
  if(x||y) {
    Matrix1<double> D1;
    ReduceDimention2(D,D1,x,y);
    DispHistogram1D(D1,fname,rmode,comment,llim,ulim);
  }
  else {
    DispHistogram2D(D,fname,rmode,comment,llim,ulim);
  }
  resetxyz();
}

/**********************************************************************
* 3D Histogram Data処理
**********************************************************************/
void Disp3D(Matrix3<double>& D,char* fname,char* rmode,char* comment
                ,double llim,double ulim) {

  if(a>MAXCANVAS-10) {
    fprintf(stderr,"Warning: Too many displays\n");
    return;
  }

  setxyz(D);
  if((x&&y) || (y&&z) || (z&&x)) {
    Matrix1<double> D1;
    ReduceDimention3(D,D1,x,y,z);
    DispHistogram1D(D1,fname,rmode,comment,llim,ulim);
  }
  else if((x||y||z) && step==0) {
    Matrix2<double> D2;
    ReduceDimention3(D,D2,x,y,z);
    DispHistogram2D(D2,fname,rmode,comment,llim,ulim);
  }
  else if(x) {
    char com[500];
    Matrix2<double> D2;
    for(int i=x;i<=max;i+=step) {
      ReduceDimention3(D,D2,i,y,z);
      sprintf(com,"%s x%d\n",comment,i);
      DispHistogram2D(D2,fname,rmode,com,llim,ulim);
    }
  }
  else if(y) {
    char com[500];
    Matrix2<double> D2;
    for(int i=y;i<=max;i+=step) {
      ReduceDimention3(D,D2,x,i,z);
      sprintf(com,"%s y%d\n",comment,i);
      DispHistogram2D(D2,fname,rmode,com,llim,ulim);
    }
  }
  else if(z) {
    char com[500];
    Matrix2<double> D2;
    for(int i=z;i<=max;i+=step) {
      ReduceDimention3(D,D2,x,y,i);
      sprintf(com,"%s z%d\n",comment,i);
      DispHistogram2D(D2,fname,rmode,com,llim,ulim);
    }
  }
  else {
    DispHistogram3D(D,fname,rmode,comment,llim,ulim);
  }
  resetxyz();
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
/**********************************************************************
* 2D Histogram Data処理
**********************************************************************/
void Update2D(Matrix2<double>& D,char* fname,char* rmode,char* comment
                ,double llim,double ulim) {
  setxyz(D);
  if(x||y) {
    Matrix1<double> D1;
    ReduceDimention2(D,D1,x,y);
    UpdateHistogram1D(D1,fname,rmode,comment,llim,ulim);
  }
  else {
    UpdateHistogram2D(D,fname,rmode,comment,llim,ulim);
  }
  resetxyz();
}

/**********************************************************************
* 3D Histogram Data処理
**********************************************************************/
void Update3D(Matrix3<double>& D,char* fname,char* rmode,char* comment
                ,double llim,double ulim) {
  setxyz(D);
  if((x&&y) || (y&&z) || (z&&x)) {
    Matrix1<double> D1;
    ReduceDimention3(D,D1,x,y,z);
    UpdateHistogram1D(D1,fname,rmode,comment,llim,ulim);
  }
  else if(x||y||z) {
    Matrix2<double> D2;
    ReduceDimention3(D,D2,x,y,z);
    UpdateHistogram2D(D2,fname,rmode,comment,llim,ulim);
  }
  else {
    UpdateHistogram3D(D,fname,rmode,comment,llim,ulim);
  }
  resetxyz();
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
/**********************************************************************
* setxyz
**********************************************************************/
void setxyz(Matrix2<double>& D) {
  if(rx) x=D.XSize()*rx;
  if(ry) y=D.YSize()*ry;
}
void setxyz(Matrix3<double>& D) {
  if(rx) x=D.XSize()*rx;
  if(ry) y=D.YSize()*ry;
  if(rz) z=D.ZSize()*rz;
}

void resetxyz() {
  if(rx) x=0;
  if(ry) y=0;
  if(rz) z=0;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
/**********************************************************************
* Reduce dimentaion
**********************************************************************/
void ReduceDimention2(Matrix2<double>& D2,Matrix1<double>& D1
	             ,int x,int y){
  double *mat2 = D2.Data();
  int i,j;
  int xsize=D2.XSize();
  int ysize=D2.YSize();
  double *mat;
  if(x) {
    D1.Allocate(ysize);
    mat = D1.Data();
    for(j=0;j<ysize;j++) mat[j] = mat2[x*ysize+j];
  }
  else if(y) {
    D1.Allocate(xsize);
    mat = D1.Data();
    for(i=0;i<xsize;i++) mat[i] = mat2[i*ysize+y];
  }
  else {
    fprintf(stderr,"Error(ReduceDimention2): Both x and y are 0\n");
  }
}

//////////////////////////////////////////////////////////////////////
void ReduceDimention3(Matrix3<double>& D3,Matrix2<double>& D2
	             ,int x,int y,int z){
  double *mat3 = D3.Data();
  int i,j,v;
  int xsize=D3.XSize();
  int ysize=D3.YSize();
  int zsize=D3.ZSize();
  double *mat;

  if(x) {
    D2.Allocate(ysize,zsize);
    mat = D2.Data();
    for(j=0;j<ysize;j++) 
     for(v=0;v<zsize;v++) 
      mat[j*zsize+v] = mat3[(x*ysize+j)*zsize+v];
  }
  else if(y) {
    D2.Allocate(xsize,zsize);
    mat = D2.Data();
    for(i=0;i<xsize;i++) 
     for(v=0;v<zsize;v++) 
      mat[i*zsize+v] = mat3[(i*ysize+y)*zsize+v];
  }
  else if(z) {
    D2.Allocate(xsize,ysize);
    mat = D2.Data();
    for(i=0;i<xsize;i++) 
     for(j=0;j<ysize;j++) 
      mat[i*ysize+j] = mat3[(i*ysize+j)*zsize+z];
  }
  else {
    fprintf(stderr,"Error(ReduceDimention3): All of x,y,z are 0\n");
  }
}

//////////////////////////////////////////////////////////////////////
void ReduceDimention3(Matrix3<double>& D3,Matrix1<double>& D1
	             ,int x,int y,int z){
  double *mat3 = D3.Data();
  int i,j,v;
  int xsize=D3.XSize();
  int ysize=D3.YSize();
  int zsize=D3.ZSize();
  double *mat;

  if(x&&y) {
    D1.Allocate(zsize);
    mat = D1.Data();
    for(v=0;v<zsize;v++) 
      mat[v] = mat3[(x*ysize+y)*zsize+v];
  }
  else if(y&&z) {
    D1.Allocate(xsize);
    mat = D1.Data();
    for(i=0;i<xsize;i++) 
      mat[i] = mat3[(i*ysize+y)*zsize+z];
  }
  else if(z&&x) {
    D1.Allocate(ysize);
    mat = D1.Data();
    for(j=0;j<ysize;j++) 
      mat[j] = mat3[(x*ysize+j)*zsize+z];
  }
  else {
    fprintf(stderr,"Error(ReduceDimention2): All of x,y,z are 0\n");
  }
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/**********************************************************************
* DataProcessing
**********************************************************************/
//////////////////////////////////////////////////////////////////////
void DataProcessing(Matrix1<double>& D) {
  double val;
  int i;
  int xsize = D.XSize(); 
  double *mat = D.Data();
  double absmin=1e99;

  if(histlog) {
    for(i=0;i<xsize;i++) {
      val = mat[i]; 
      if(fabs(val)<absmin && val!=0) absmin=fabs(val);
    }
    printf("ABSMIN=%g , histlog=%g\n",absmin,histlog);
    absmin=histlog;
    for(i=0;i<xsize;i++) {
      val = mat[i]/absmin; 
      if(fabs(val)<1.0) val=val>0?1.0:-1.0;
      if(val==0.0)   val = 0.0;
      else if(val>0) val = log10(val);
      else           val = -log10(-val);
      mat[i] = val;
    }
  }
}

//////////////////////////////////////////////////////////////////////
void DataProcessing(Matrix2<double>& D) {
  double val;
  int i,j;
  int xsize = D.XSize(); 
  int ysize = D.YSize();
  double *mat = D.Data();
  double absmin=1e99;

  if(histlog) {
    for(i=0;i<xsize;i++) {
     for(j=0;j<ysize;j++) {
      val = mat[i*ysize+j]; 
      if(fabs(val)<absmin && val!=0) absmin=fabs(val);
     }
    }
    printf("ABSMIN=%g , histlog=%g\n",absmin,histlog);
    absmin=histlog;
    for(i=0;i<xsize;i++) {
     for(j=0;j<ysize;j++) {
      val = mat[i*ysize+j]/absmin; 
      if(fabs(val)<1.0) val=val>0?1.0:-1.0;
      if(val==0)     val = 0.0;
      else if(val>0) val = log10(val);
      else           val = -log10(-val);
      mat[i*ysize+j] = val; 
     }
    }
  }
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void DataProcessing(Matrix3<double>& D) {
  double val;
  int i,j,z;
  int xsize = D.XSize(); 
  int ysize = D.YSize();
  int zsize = D.ZSize();
  double *mat = D.Data();
  double absmin=1e99;

  if(histlog) {
    for(i=0;i<xsize;i++) {
     for(j=0;j<ysize;j++) {
      for(int v=0;v<zsize;v++) {
       val = mat[(i*ysize+j)*zsize+v]; 
       if(fabs(val)<absmin && val!=0) absmin=fabs(val);
      }
     }
    }
    printf("ABSMIN=%g , histlog=%g\n",absmin,histlog);
    absmin=histlog;
    for(i=0;i<xsize;i++) {
     for(j=0;j<ysize;j++) {
      for(int i=0;i<xsize;i++) {
       val = mat[i*ysize+j]/absmin; 
       if(fabs(val)<1.0) val=val>0?1.0:-1.0;
       if(val==0)     val = 0.0;
       else if(val>0) val = log10(val);
       else           val = -log10(-val);
       mat[i*ysize+j] = val; 
      }
     }
    }
  }
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// Display non-uniform mesh with TGraph2D (NOT USED)
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
/**********************************************************************
* 1D Graphによる表示
**********************************************************************/
void DispGraph1D(Matrix1<double>& D,char* fname="",char* rmode="",char* comment=""
                ,double llim=0,double ulim=0,vector<double>& HX) {
  // There is no corresponding TGraph1D class
  DispHistogram1D(D,fname,rmode,comment ,llim,ulim);
}

/**********************************************************************
* 2D Graphによる 3D 表示
**********************************************************************/
void DispGraph2D(Matrix2<double>& D,char* fname="",char* rmode="",char* comment=""
                ,double llim=0,double ulim=0,vector<double>& HX,vector<double>& HY) {
  // printf("ReadMatrix %s %s\n",rmode,comment);
  char name[500];
  char com[505];
  double val;

  if(ibuf2<MAXBUF) buf2[ibuf2++] = D;

  if(lower<upper) {
    llim = lower;
    ulim = upper;
  }
  int xsize = D.XSize(); 
  int ysize = D.YSize();
  int i,j;

  sprintf(name,"%s_c%d%s",fname,a,rmode);
  //sprintf(name,"%s_c%d",fname,a);
  //sprintf(com,"%s:%s",fname,rmode);
  c[a] = new TCanvas(name,com,10+a*20+xoffset,10+a*20+yoffset,wx,wy);
  sprintf(name,"pad%d",a);
  //sprintf(com,"%s %s",rmode,comment);
  //pad[a] = new TPad(name,com,0.01,0.01,0.99,0.99,21);

  sprintf(name,"%s_h%d",fname,a);
  sprintf(com,"%s:%s %s",fname,rmode,comment);
  g2d[a] = new TGraph2D();
  g2d[a]->SetTitle(name);

  double *mat = D.Data();

  //printf("D:%d %d HX %d HY %d\n",D.XSize(),D.YSize(),HX.size(),HY.size());

  DataProcessing(D);
  vector<double> Xaxis,Yaxis;
  double h;
  int i;
  val=0;
  for(i=0;i<xsize;i++) {
    Xaxis.push_back(val);
    val+=HX[i];
  }
  Xaxis.push_back(val);
  val=0;
  for(i=0;i<ysize;i++) {
    Yaxis.push_back(val);
    val+=HY[i];
  }
  Yaxis.push_back(val);

  h=HX[1];

  int N=0;
  for(i=0;i<xsize;i++) {
    if(hxmin!=hxmax && (Xaxis[i]<hxmin || hxmax<Xaxis[i])) continue;
    for(j=0;j<ysize;j++) {
      if(hymin!=hymax && (Yaxis[i]<hymin || hymax<Yaxis[i])) continue;
      val = mat[i*ysize+j]; 
      if(llim<ulim) {
        if(llim>val) val=llim;
        if(ulim<val) val=ulim;
      }
      //g2d[a]->SetPoint(N++,Xaxis[i],Yaxis[j],val);
      //g2d[a]->SetPoint(N++,Yaxis[j],Xaxis[i],val); // xyを逆にしている
      g2d[a]->SetPoint(N++,Yaxis[j],Xaxis[i],val); // xyを逆にしている
      //printf("vaL N=%d Y[%d]=%g X=[%d]=%g val=%g\n",N,j,Yaxis[j],i,Xaxis[i],val);
    }
  }

  //pad[a]->Draw();

  //g2d[a]->SetStats(kFALSE);
  //pad[a]->SetTheta(theta);
  //pad[a]->SetPhi(phi);
  //pad[a]->cd();
  gStyle->SetPalette(1);
  g2d[a]->Draw(opt2.c_str());

  ++a;
}

/**********************************************************************
* 3D Graphによる 3D 表示 (Scatter plot)
**********************************************************************/
void DispGraph3D(Matrix3<double>& D,char* fname="",char* rmode="",char* comment=""
                  ,double llim=0,double ulim=0
                  ,vector<double>& HX,vector<double>& HY,vector<double>& HZ) {
  // There is no corresponding TGraph3D class
  DispHistogram3D(D,fname,rmode,comment ,llim,ulim);
}



//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// Display uniform mesh using TH123D
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
/**********************************************************************
* 1D Histogramによる表示
**********************************************************************/
void DispHistogram1D(Matrix1<double>& D,char* fname="",char* rmode="",char* comment=""
                ,double llim=0,double ulim=0) {
  // printf("ReadMatrix %s %s\n",rmode,comment);
  char name[500];
  char com[505];
  double val;

  if(ibuf1<MAXBUF) buf1[ibuf1++] = D;

  if(lower<upper) {
    llim = lower;
    ulim = upper;
  }
  int xsize = D.XSize(); 
  int i=0;

  sprintf(name,"%s_h%d",fname,a);
  sprintf(com,"%s:%s %s",fname,rmode,comment);

  h1[a]=new TH1D(name,com,xsize-1,0,xsize-1);

  double *mat = D.Data();

  DataProcessing(D);

  for(i=0;i<xsize;i++) {
    val = mat[i]; 
    if(llim<ulim) {
      if(llim>val) val=llim;
      if(ulim<val) val=ulim;
    }
    h1[a]->SetBinContent(i,val);
  }

  sprintf(name,"%s_c%d%s",fname,a,rmode);
  //sprintf(name,"%s_c%d",fname,a);
  //sprintf(com,"%s:%s",fname,rmode);
  c[a] = new TCanvas(name,com,10+a*20+xoffset,10+a*20+yoffset,wx,wy);

  sprintf(name,"pad%d",a);
  //sprintf(com,"%s %s",rmode,comment);
  pad[a] = new TPad(name,com,0.01,0.01,0.99,0.99,21);
  pad[a]->Draw();

  if(hxmin!=hxmax) h1[a]->SetAxisRange(hxmin,hxmax,"X");
  h1[a]->SetStats(kFALSE);
  pad[a]->cd();
  h1[a]->Draw(opt1.c_str());

  ++a;
}

/**********************************************************************
* 2D Histogramによる 3D 表示
**********************************************************************/
void DispHistogram2D(Matrix2<double>& D,char* fname="",char* rmode="",char* comment=""
                ,double llim=0,double ulim=0) {
  // printf("ReadMatrix %s %s\n",rmode,comment);
  char name[500];
  char com[505];
  double val;

  if(ibuf2<MAXBUF) buf2[ibuf2++] = D;

  if(lower<upper) {
    llim = lower;
    ulim = upper;
  }
  int xsize = D.XSize(); 
  int ysize = D.YSize();
  int i,j;

  sprintf(name,"%s_h%d",fname,a);
  sprintf(com,"%s:%s %s",fname,rmode,comment);

  h2[a]=new TH2D(name,com,ysize-1,0,ysize-1,xsize-1,0,xsize-1);

  double *mat = D.Data();

  DataProcessing(D);

  for(i=0;i<xsize;i++) {
    for(j=0;j<ysize;j++) {
      val = mat[i*ysize+j]; 
      if(llim<ulim) {
        if(llim>val) val=llim;
        if(ulim<val) val=ulim;
      }
      h2[a]->SetBinContent(j,i,val); // わざとxyを逆にしている
    }
  }

  sprintf(name,"%s_c%d%s",fname,a,rmode);
  //sprintf(name,"%s_c%d",fname,a);
  //sprintf(com,"%s:%s",fname,rmode);
  c[a] = new TCanvas(name,com,10+a*20+xoffset,10+a*20+yoffset,wx,wy);
sprintf(name,"pad%d",a);
  //sprintf(com,"%s %s",rmode,comment);
  pad[a] = new TPad(name,com,0.01,0.01,0.99,0.99,21);
  pad[a]->Draw();

  if(hxmin!=hxmax) h2[a]->SetAxisRange(hxmin,hxmax,"Y");
  if(hymin!=hymax) h2[a]->SetAxisRange(hymin,hymax,"X");
  h2[a]->SetStats(kFALSE);
  pad[a]->SetTheta(theta);
  pad[a]->SetPhi(phi);
  pad[a]->cd();
  h2[a]->Draw(opt2.c_str());

  ++a;
}

double max3d=100;
double min3d=-50;

/**********************************************************************
* 3D Histogramによる 3D 表示 (Scatter plot)
**********************************************************************/
void DispHistogram3D(Matrix3<double>& D,char* fname="",char* rmode="",char* comment=""
                  ,double llim=0,double ulim=0) {
  // printf("ReadMatrix %s %s\n",rmode,comment);
  char name[500];
  char com[505];
  double val;

  if(ibuf3<MAXBUF) buf3[ibuf3++] = D;

  if(lower<upper) {
    llim = lower;
    ulim = upper;
  }
  int xsize = D.XSize();
  int ysize = D.YSize();
  int zsize = D.ZSize();
  int i=0;

  sprintf(name,"%s_h%d",fname,a);
  sprintf(com,"%s:%s %s",fname,rmode,comment);

  h3[a]=new TH3D(name,com
                ,xsize-1,0,xsize-1
                ,ysize-1,0,ysize-1
                ,zsize-1,0,zsize-1);

  double *mat = D.Data();
  double min=1e99;
  double max=-1e99;
  double sum=0;

  DataProcessing(D);

  for(int v=0;v<zsize;v++) {
   for(int j=0;j<ysize;j++) {
    for(int i=0;i<xsize;i++) {
      val = mat[(i*ysize+j)*zsize+v]; 
      if(val<min) min=val;
      if(val>max) max=val;
    }
   }
  }

  if(max==min) {
    printf("min==max==%g,%g, %s %s 3D histogram aborted\n",min,max,fname,comment);
    //return;
  }
  else {
  double gain=max3d/(fabs(max)>fabs(min)?fabs(max):fabs(min));
  double offset=0;

  for(int v=0;v<zsize;v++) {
   for(int j=0;j<ysize;j++) {
    for(int i=0;i<xsize;i++) {
      val = mat[(i*ysize+j)*zsize+v]; 
      if(llim<ulim) {
        if(llim>val) val=llim;
        if(ulim<val) val=ulim;
      }
      val = fabs(val)*gain+offset;
      h3[a]->SetBinContent(i,j,v,val);
    }
   }
  }
  }

  sprintf(name,"%s_c%d%s",fname,a,rmode);
  //sprintf(name,"%s_c%d",fname,a);
  //sprintf(com,"%s:%s",fname,rmode);
  c[a] = new TCanvas(name,com,10+a*20+xoffset,10+a*20+yoffset,wx,wy);

  sprintf(name,"pad%d",a);
  //sprintf(com,"%s %s",rmode,comment);
  pad[a] = new TPad(name,com,0.01,0.01,0.99,0.99,21);
  pad[a]->Draw();

  if(hxmin!=hxmax) h2[a]->SetAxisRange(hxmin,hxmax,"Y");
  if(hymin!=hymax) h2[a]->SetAxisRange(hymin,hymax,"X");
  h3[a]->SetStats(kFALSE);
  h3[a]->SetMarkerColor(kBlue);
  //h3[a]->SetMarkerColor(kRed);
  //h3[a]->SetMarkerColor(a);
  pad[a]->SetTheta(theta);
  pad[a]->SetPhi(phi);
  pad[a]->cd();
  h3[a]->Draw(opt3.c_str());

  ++a;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
/**********************************************************************
* 1D Histogramによる表示
**********************************************************************/
void UpdateHistogram1D(Matrix1<double>& D,char* fname="",char* rmode="",char* comment=""
                ,double llim=0,double ulim=0) {
  double val;

  if(lower<upper) {
    llim = lower;
    ulim = upper;
  }
  int xsize = D.XSize(); 
  int i=0;

  double *mat = D.Data();
  DataProcessing(D);

  for(i=0;i<xsize;i++) {
    val = mat[i]; 
    if(llim<ulim) {
      if(llim>val) val=llim;
      if(ulim<val) val=ulim;
    }
    h1[a-1]->SetBinContent(i,val);
  }

  if(llim!=ulim) {
    //h1[a-1]->GetYaxis()->SetLimits(llim,ulim);
    h1[a-1]->GetYaxis()->SetRangeUser(llim,ulim);
  }

  pad[a-1]->Modified();
  pad[a-1]->Update();
}

/**********************************************************************
* 2D Histogramによる 3D 表示
**********************************************************************/
double phi=20;
double theta=30;
double phiRotate=0,phiStop=360, _phiRot=0;
double thetaRotate=0,thetaStop=90, _thetaRot=0;
void UpdateHistogram2D(Matrix2<double>& D,char* fname="",char* rmode="",char* comment=""
                ,double llim=0,double ulim=0) {
  double val;

  if(lower<upper) {
    llim = lower;
    ulim = upper;
  }
  int xsize = D.XSize(); 
  int ysize = D.YSize();
  int i,j;

  double *mat = D.Data();

  DataProcessing(D);

  for(i=0;i<xsize;i++) {
    for(j=0;j<ysize;j++) {
      val = mat[i*ysize+j]; 
      if(llim<ulim) {
        if(llim>val) val=llim;
        if(ulim<val) val=ulim;
      }
      h2[a-1]->SetBinContent(j,i,val); // わざとxyを逆にしている
    }
  }

  if(llim!=ulim) {
    //h2[a-1]->GetZaxis()->SetLimits(llim,ulim);
    h2[a-1]->GetZaxis()->SetRangeUser(llim,ulim);
  }

  pad[a-1]->SetTheta(theta+_thetaRot);
  pad[a-1]->SetPhi(phi+_phiRot);
  pad[a-1]->Modified();
  pad[a-1]->Update();
}

/**********************************************************************
* 3D Histogramによる 3D 表示 (Scatter plot)
**********************************************************************/
void UpdateHistogram3D(Matrix3<double>& D,char* fname="",char* rmode="",char* comment=""
                  ,double llim=0,double ulim=0) {
  double val;

  if(lower<upper) {
    llim = lower;
    ulim = upper;
  }
  int xsize = D.XSize();
  int ysize = D.YSize();
  int zsize = D.ZSize();
  int i=0;

  double *mat = D.Data();
  double min=1e99;
  double max=-1e99;
  double sum=0;

  DataProcessing(D);

  for(int v=0;v<zsize;v++) {
   for(int j=0;j<ysize;j++) {
    for(int i=0;i<xsize;i++) {
      val = mat[(i*ysize+j)*zsize+v]; 
      if(val<min) min=val;
      if(val>max) max=val;
    }
   }
  }

  if(replaymin!=replaymax) {
    min=replaymin;
    max=replaymax;
  }

  if(max==min) {
    printf("min==max==%g,%g, %s %s 3D histogram aborted\n",min,max,fname,comment);
    return;
  }
  double gain=max3d/(fabs(max)>fabs(min)?fabs(max):fabs(min));
  double offset=0;

  for(int v=0;v<zsize;v++) {
   for(int j=0;j<ysize;j++) {
    for(int i=0;i<xsize;i++) {
      val = mat[(i*ysize+j)*zsize+v]; 
      if(llim<ulim) {
        if(llim>val) val=llim;
        if(ulim<val) val=ulim;
      }
      val = fabs(val)*gain+offset;
      h3[a-1]->SetBinContent(i,j,v,val);
    }
   }
  }

  pad[a-1]->SetTheta(theta);
  pad[a-1]->SetPhi(phi);
  pad[a-1]->Modified();
  pad[a-1]->Update();
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// Parsing
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// variables for LET statement
double NLAYER,VF,ER,Diffu,G,S,H,d,Qs;

double h;

/**********************************************************************
* CSV file parser
**********************************************************************/
void ReadCSV(char* fname) {
  S_Orig=0;
  G_Orig=0;
  C_Orig=0;
  S_DA=0;
  G_DA=0;
  C_DA=0;

  printf("# %s ##############################################\n",fname);
  fout.print(fname);

  char comment[100] = "";
  ReadF f(fname);
  f.setdelimiter(",");
  f.setquotation("'");

  while(f.read()) {
    if(!f.argc) continue;
    if(f.argv[1][0]=='/' || f.argv[1][0]=='#') continue;

    if(f.argc>=2 && strncmp(f.argv[1],"BEGIN",3)==0) {
      if(f.argc>=3) strcpy(comment,f.argv[3]);
      if(strncmp(f.argv[2],"M",1)==0) {
        ReadMatrix(f,fname,"M",comment);
      }
      if(strncmp(f.argv[2],"V",1)==0) {
        ReadMatrix(f,fname,"V",comment);
      }
      if(strncmp(f.argv[2],"Q",1)==0) {
        ReadMatrix(f,fname,"Q",comment);
      }
      if(strncmp(f.argv[2],"I",1)==0 ||
         strncmp(f.argv[2],"binI",4)==0) {
        ReadCharge(f,fname,"C",comment);
      }
      if(strncmp(f.argv[2],"C",1)==0 ||
         strncmp(f.argv[2],"binC",4)==0) {
        ReadCharge(f,fname,"C",comment);
      }
      if(strncmp(f.argv[2],"T",1)==0) {
        ReadGraph(f,fname,"T",comment,0);
      }
      if(strncmp(f.argv[2],"binM",4)==0) {
        ReadMatrixBin(f,fname,"M",comment);
      }
      if(strncmp(f.argv[2],"binV",4)==0) {
        ReadMatrixBin(f,fname,"V",comment);
      }
      if(strncmp(f.argv[2],"binQ",4)==0) {
        ReadMatrixBin(f,fname,"Q",comment);
      }
      if(strncmp(f.argv[2],"bini",4)==0) {
        ReadMatrixBin(f,fname,"i",comment);
      }
      if(strncmp(f.argv[2],"binT",4)==0) {
        ReadGraph(f,fname,"T",comment,1);
      }
      if(strncmp(f.argv[2],"binBUFFER",4)==0) {
        ReadGraph(f,fname,"T",comment,0);
      }
      if(strncmp(f.argv[2],"BUFFER",4)==0) {
        ReadGraph(f,fname,"T",comment,0);
      }
      if(strncmp(f.argv[2],"STRING",4)==0 ||
         strncmp(f.argv[2],"binSTRING",5)==0) {
        f.read();
        if(!noformula) {
          for(int i=2;i<=f.argc;i++) {
            if(f.argv[i][0]=='/'||f.argv[i][0]=='#') break;
            G__calc(f.argv[i]);
          }
        }
        f.read(); // END
      }
    }
    else if(f.argc>=1 && strncmp(f.argv[1],"INPUT",3)==0) {
      printf("%s\n",f.argv[0]);
      char buf[100];
      strcpy(buf,f.argv[f.argc]);
      char *p = strchr(buf,'=');
      if(p) {
        p++;
        char *p2=strchr(p,'(');
        if(p2) {
          *p2=0;
          h = atof(p)*1e-3;
        }
      }
    }
    else if(f.argc>=1 && strncmp(f.argv[1],"PAUSE",3)==0) {
      G__pause();
    }
    else if(f.argc>=1 && strncmp(f.argv[1],"EXIT",3)==0) {
      //printf("EXIT %s\n",name);
      return;
    }
    else if(f.argc>=2 && strncmp(f.argv[1],"LET",3)==0) {
      string decl = "";
      decl += f.argv[2];
      decl += "=";
      decl += f.argv[3];
      decl += ";";
      //printf("%s\n",decl.c_str());
      G__exec_text(decl.c_str());
    }
    else if(f.argc>=2 && strncmp(f.argv[1],"C++",3)==0) {
      string expr=f.argv[0]+3;
      G__exec_text(expr.c_str());
    }
    else {
      //printf("Unknown statement %d:%s\n",f.line,f.argv[0]);
      //return;
    }
  }

  if(S_Orig) {
    printf("S_DA/S_Orig=%g/%g=%g%%\nS_DA-S_Orig=%g  S_Orig*2%%=%g\n"
	,S_DA,S_Orig,(S_DA/S_Orig-1)*100,S_DA-S_Orig,S_Orig*0.02);
    printf("G_Orig/S_Orig = %g/%g = %g%%\n",G_Orig,S_Orig,(G_Orig/S_Orig)*100);
  }
  printf("\n");
  fout.print();

}



/**********************************************************************
 * This function is automatically called with '.x disp.cxx("fname")' command
 **********************************************************************/
void disp(const char *fname1="",double ymin,double ymax) {
  Clean();
  init();
  SetGraphScale(ymin,ymax);
  if(strcmp(fname1,"")) dispz(fname1);
  terminate();
}
void disp(const char *fname1=""
         ,const char *fname2=""
         ,const char *fname3="" 
         ,const char *fname4="" 
         ,const char *fname5=""
         ,const char *fname6=""
         ,const char *fname7=""
         ,const char *fname8=""
         ,const char *fname9=""
         ,const char *fname10=""
         ,const char *fname11=""
         ,const char *fname12=""
         ,const char *fname13=""
         ,const char *fname14=""
         ,const char *fname15=""
         ,const char *fname16="") {

  Clean();
  init();
#if 1
  if(strcmp(fname1,"")) dispz(fname1);
  if(strcmp(fname2,"")) dispz(fname2);
  if(strcmp(fname3,"")) dispz(fname3);
  if(strcmp(fname4,"")) dispz(fname4);
  if(strcmp(fname5,"")) dispz(fname5);
  if(strcmp(fname6,"")) dispz(fname6);
  if(strcmp(fname7,"")) dispz(fname7);
  if(strcmp(fname8,"")) dispz(fname8);
  if(strcmp(fname9,"")) dispz(fname9);
  if(strcmp(fname10,"")) dispz(fname10);
  if(strcmp(fname11,"")) dispz(fname11);
  if(strcmp(fname12,"")) dispz(fname12);
  if(strcmp(fname13,"")) dispz(fname13);
  if(strcmp(fname14,"")) dispz(fname14);
  if(strcmp(fname15,"")) dispz(fname15);
#else
  if(strcmp(fname1,"")) ReadCSV(fname1);
  if(strcmp(fname2,"")) ReadCSV(fname2);
  if(strcmp(fname3,"")) ReadCSV(fname3);
  if(strcmp(fname4,"")) ReadCSV(fname4);
  if(strcmp(fname5,"")) ReadCSV(fname5);
  if(strcmp(fname6,"")) ReadCSV(fname6);
  if(strcmp(fname7,"")) ReadCSV(fname7);
  if(strcmp(fname8,"")) ReadCSV(fname8);
  if(strcmp(fname9,"")) ReadCSV(fname9);
  if(strcmp(fname10,"")) ReadCSV(fname10);
  if(strcmp(fname11,"")) ReadCSV(fname11);
  if(strcmp(fname12,"")) ReadCSV(fname12);
  if(strcmp(fname13,"")) ReadCSV(fname13);
  if(strcmp(fname14,"")) ReadCSV(fname14);
  if(strcmp(fname15,"")) ReadCSV(fname15);
#endif
  terminate();
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
/**********************************************************************
* meta dispX
**********************************************************************/
//////////////////////////////////////////////////////////////////////
class Meta {
 public:
  Meta(const char *expr="") : x((FILE*)1) {  Init(expr); }
  void Init(const string& expr) {
    index = 0;
    x.setseparator(",");
    x.parse(expr.c_str()); 
  }

  const char* next() {
     ++index;
    if(1==index && strlen(x.argv[0])==0) return("");
    if(index<=x.argc) return(x.argv[index]);
    else return(0);
  }

 //private:
  ReadF x;
  int index;
};

//////////////////////////////////////////////////////////////////////
void dispx(string expr1=""
          ,string expr2=""
          ,string expr3=""
          ,string expr4=""
          ,string expr5=""
          ,string expr6=""
          ) {
  char *s1;
  char *s2;
  char *s3;
  char *s4;
  char *s5;
  char *s6;
  string fname;
  Meta m1,m2,m3,m4,m5,m6;

  Clean();
  init();

  m1.Init(expr1);
  while((s1=m1.next())) {
   m2.Init(expr2);
   while((s2=m2.next())) {
    m3.Init(expr3);
    while((s3=m3.next())) {
     m4.Init(expr4);
     while((s4=m4.next())) {
      m5.Init(expr5);
      while((s5=m5.next())) {
       m6.Init(expr6);
       while((s6=m6.next())) {
        fname = string(s1)+s2+s3+s4+s5+s6;
        printf("fname=%s\n",fname.c_str());
        ReadCSV(fname.c_str());
       }
      }
     }
    }
   }
  }
  terminate();
}

//////////////////////////////////////////////////////////////////////
void dispy(string expr1=""
          ,string expr2=""
          ,string expr3=""
          ,string expr4=""
          ,string expr5=""
          ,string expr6=""
          ) {
  char *s1;
  char *s2;
  char *s3;
  char *s4;
  char *s5;
  char *s6;
  string fname;
  Meta m1,m2,m3,m4,m5,m6;

  Clean();
  init();

  m6.Init(expr6);
  while((s6=m6.next())) {
   m5.Init(expr5);
   while((s5=m5.next())) {
    m4.Init(expr4);
    while((s4=m4.next())) {
     m3.Init(expr3);
     while((s3=m3.next())) {
      m2.Init(expr2);
      while((s2=m2.next())) {
       m1.Init(expr1);
       while((s1=m1.next())) {
        fname = string(s1)+s2+s3+s4+s5+s6;
        printf("fname=%s\n",fname.c_str());
        ReadCSV(fname.c_str());
       }
      }
     }
    }
   }
  }
  terminate();
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/**********************************************************************
* isexclude
**********************************************************************/
char *exclude[] = { ".txt",".C",".cxx","" };

int isexclude(char* name) {
  int flag=0;
  int i=0;
  if(0==strcmp(name,".")) return(1);
  if(0==strcmp(name,"..")) return(1);
  while(exclude[i][0]) {
    if(strstr(name,exclude[i++])) flag=1;
  }
  return(flag);
}

char *include[] = { ".out", ".sum", "" };

int isinclude(char* name) {
  int flag=0;
  int i=0;
  while(include[i][0]) {
    if(strstr(name,include[i++])) flag=1;
  }
  return(flag);
}

/**********************************************************************
* scandir
**********************************************************************/
void dispz(char* expr,char* originaldir=".") {
  //Clean();
  //init();

  TSystemDirectory dir;
  dir.SetDirectory(originaldir);
  TList *pl = dir.GetListOfFiles();
  TIter next(pl);
  TObject *obj;
  char in[500];

  EasyExp rege(expr);

  map<string,int> files;
  while((obj=next())) {
    //printf("%s\n",obj->GetName());
    if(!isexclude(obj->GetName()) && isinclude(obj->GetName())) {
      if(rege.match(obj->GetName())) {
        files[obj->GetName()] = 1;
        //printf("%s\n",obj->GetName());
        //sprintf(in,"%s",obj->GetName());
        //ReadCSV(in);
      }
    }
  }

  string prevfname;
  int prevdif=0;

  string fname;
  map<string,int>::iterator first = files.begin();
  map<string,int>::iterator last  = files.end();
  while(first!=last) {
    fname = (*first).first;
    printf("%s",fname.c_str());
    if(samenu>=1000) {
      int dif = strdif(fname.c_str(),prevfname.c_str());
 printf("%s %s %d %d\n",fname.c_str(),prevfname.c_str(),dif,prevdif);
      if(prevdif && dif>prevdif) terminate();
      prevfname=fname;
      prevdif=dif;
    }
    ReadCSV(fname.c_str());
    ++first;
  }

  //terminate();
}

//////////////////////////////////////////////////////////////////////
int strdif(const char* a,const char* b) {
  int count=0;
  int i=0;
  while(a[i]) {
    if(b[i]==0) {
      while(a[i++]) ++count;
      return(count);
    }
    if(a[i]!=b[i]) ++count;
    ++i;
  }
  while(b[i++]) ++count;
  return(count);
}
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// Diffusion length analysis
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

int dx=0,dy=0,dz=0;
int nowaveform=0;

/**********************************************************************
* Diffusion ballanced dielectric absorption analysis
**********************************************************************/
void DiffusionAnalysis2(Matrix2<double>& D2,Matrix1<double>& Out,double& A
	             ,int x,int y){
  double *mat2 = D2.Data();
  int i,j;
  int xsize=D2.XSize();
  int ysize=D2.YSize();
  double *mat = D2.Data();
  double *dat;
  double a;
  statistics stat;
  int L;
  if(x>0) {
    Out.Allocate(xsize);
    Out.Init();
    dat = Out.Data();
    // 傾きの平均値
    stat.init();
    for(j=0;j<ysize-1;j++) {
      for(i=0;i<xsize-1;i++) {
        a = mat[(i+1)*ysize+j]-mat[i*ysize+j];
        if(a>0) stat.add(a);
      }
    }
    A = stat.mean();

    // Histogram filling
    for(j=0;j<ysize-1;j++) {
      L=0;
      for(i=0;i<xsize-1;i++) {
        a = mat[(i+1)*ysize+j]-mat[i*ysize+j];
        if(A*0.8<a && a<A*1.2) ++L;
        else {
          if(L)	dat[L]+=1.0;
          L=0;
        }
      }
    }
  }
  else if(x<0) {
    Out.Allocate(xsize);
    Out.Init();
    dat = Out.Data();
    // 傾きの平均値
    stat.init();
    for(j=0;j<ysize-1;j++) {
      for(i=0;i<xsize-1;i++) {
        a = -mat[(i+1)*ysize+j]+mat[i*ysize+j];
        if(a>0) stat.add(a);
      }
    }
    A = stat.mean();
    printf("Gradient ");
    stat.disp();

    // Histogram filling
    for(j=0;j<ysize-1;j++) {
      L=0;
      for(i=0;i<xsize-1;i++) {
        a = -mat[(i+1)*ysize+j]+mat[i*ysize+j];
        if(A*0.8<a && a<A*1.2) ++L;
        else {
          if(L) dat[L]+=1.0; 
          L=0;
        }
      }
    }
  }
  else if(y>0) {
  }
  else if(y<0) {
  }
  else {
    fprintf(stderr,"Error(DiffusionAnalysis2): Both x and y are 0\n");
  }
}

//////////////////////////////////////////////////////////////////////
void WaveformSynthesis1D(Matrix1<double>& D1,double A,double D,double h,double Z,char* fname,char* comment) {

  // This may be wrong
  // A = dq/dx = g*V/D/d;
  // I(t)  = g*V/x/d*exp(-t*12*D/x/x);
  //       = A*D/x*exp(-t*12*D/x/x);
  // This should be right
  // A = dq/dx = g*V/D/d;
  // I(t)  = ∫0->d g*V*x/d/d*exp(-t*12*D/x/x) dx;
  //       = ∫0->d A*D*x/d*exp(-t*12*D/x/x) dx;
  double d; 
  double ADx_d;
  double tau;
  double h;
  double x;

  int size = D1.Size();
  d = h*(size-2);
  double *mat = D1.Data();
  Y3 = X*0.0;
  int i;
  for(i=1;i<size;i++) {
    x = h*i;
    tau = -x*x/12/D*10;
    if(mat[i]) {
      ADx_d = A*D*x/d*mat[i]*h*Z*3e3;
      Y3 += ADx_d*exp(X/tau);
      printf("AD_X=%g mat[%d]=%g x=%g tau=%g\n",ADx_d,i,mat[i],x,tau);
    }
    Y4 = integ(X,Y3);
  }
  string tmp;
  if(!nowaveform) {
    if(samenu) samegraph(fname,comment,tmp);
    else     graph(fname,comment,tmp);
  }

}

//////////////////////////////////////////////////////////////////////
#if 0
void DispGraph(Matrix1<double>& D1,char* fname,char* rmode,char* comment) {
  double *dat = D1.Data();
  int ysize=3;
  Matrix2<double> D2;
  D2.Allocate(D1.Size(),ysize);
  double *mat = D2.Data();
  int i;
  for(i=0;i<D1.Size();i++) {
    mat[(i+1)*ysize+1] = dat[i];
    if(i) mat[(i+1)*ysize+2] = dat[1]/i;
    else  mat[(i+1)*ysize+2] = 0;
  }
  DispHistogram2D(D2,fname,rmode,comment,0,0);
  //DispHistogram1D(D1,fname,rmode,comment,0,0);
}
#else
void DispGraph(Matrix1<double>& D1,char* fname,char* rmode,char* comment) {
  int i;
  //return;
  //Vector<double> Xbk=X;
  //Vector<double> Y1bk=Y1;
  //Vector<double> Y2bk=Y2;
  string y1bk=y1; y1="f(x)";
  string y2bk=y2; y2="f(1)/x";
  string y3bk=y3; //y3="integ(x,f(x))";
  string y4bk=y4;
  string y5bk=y5;
  string y6bk=y6;
  string y7bk=y7;
  string y8bk=y8;
  X.clear();
  Y1.clear();
  Y2.clear();
  double *dat = D1.Data();
  for(i=0;i<D1.Size();i++) {
    X.push_back(i);
    Y1.push_back(dat[i]);
    if(i) Y2.push_back(dat[1]/i);
    else Y2.push_back(0);
  }
  //Y3=integ(X,Y1);
  //graph(fname,comment,"");
  if(samenu) samegraph(fname,comment,"");
  else     graph(fname,comment,"");

  y1=y1bk;
  y2=y2bk;
  y3=y3bk;
  y4=y4bk;
  y5=y5bk;
  y6=y6bk;
  y7=y7bk;
  y8=y8bk;
  //X=Xbk;
  //Y2=Y1bk;
  //Y2=Y2bk;
}
#endif

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// Display difference of 2 histrograms
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
#define MAXBUF 6
int ibuf1=0;
int ibuf2=0;
int ibuf3=0;
Matrix1<double> buf1[10]; 
Matrix2<double> buf2[10]; 
Matrix3<double> buf3[10]; 

//////////////////////////////////////////////////////////////////////
void DispDiff1(int a,int b,char* fname="",char* rmode="",char* comment="") {
  if(a>=MAXBUF || b>=MAXBUF) {
    printf("Buffer index out of range %d %d > %d\n",a,b,MAXBUF);
    return;
  }
  Matrix1<double> M1,M2,Diff;
  M1=buf1[a];
  M2=buf1[b];
  Diff = M1-M2;
  DispHistogram1D(Diff,fname,rmode,comment);
}
//////////////////////////////////////////////////////////////////////
void DispDiff2(int a,int b,char* fname="",char* rmode="",char* comment="") {
  if(a>=MAXBUF || b>=MAXBUF) {
    printf("Buffer index out of range %d %d > %d\n",a,b,MAXBUF);
    return;
  }
  Matrix2<double> M1,M2,Diff;
  M1=buf2[a];
  M2=buf2[b];
  Diff = M1-M2;
  DispHistogram2D(Diff,fname,rmode,comment);
}

//////////////////////////////////////////////////////////////////////
void DispDiff3(int a,int b,char* fname="",char* rmode="",char* comment="") {
  if(a>=MAXBUF || b>=MAXBUF) {
    printf("Buffer index out of range %d %d > %d\n",a,b,MAXBUF);
    return;
  }
  Matrix3<double> M1,M2,Diff;
  M1=buf3[a];
  M2=buf3[b];
  Diff = M1-M2;
  DispHistogram3D(Diff,fname,rmode,comment);
}

//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// ROOT Embedded Simulator,  Real-time simulation replay
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
char* _replayfname;
//////////////////////////////////////////////////////////////////////
void replay(char* expr,char* overrideopt=""
            ,double min=0,double max=0
            ,char* originaldir=".") {
  Clean();
  init();
  map<string,int> files;

  double storemin = replaymin;
  double storemax = replaymax;
  if(min!=max) {
    replaymin = min;
    replaymax = max;
  }

  TSystemDirectory dir;
  dir.SetDirectory(originaldir);
  TList *pl = dir.GetListOfFiles();
  TIter next(pl);
  TObject *obj;
  char in[500];

  EasyExp rege(expr);

  while((obj=next())) {
    //printf("%s\n",obj->GetName());
    if(!isexclude(obj->GetName()) && isinclude(obj->GetName())) {
      if(rege.match(obj->GetName())) {
        printf("%s\n",obj->GetName());
        sprintf(in,"%s",obj->GetName());
        files[in]=1;
      }
    }
  }

  map<string,int>::iterator first = files.begin();
  map<string,int>::iterator last  = files.end();
  while(first!=last) {
    sprintf(in,"%s",(*first).first.c_str());
    printf("%s\n",in);
    replayz(in,overrideopt);
    first++;
  }

  replaymin = storemin;
  replaymax = storemax;

  terminate();
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void replayz(const char *fname,const char* overrideopt="") {
  ReadF f(fname);
  _replayfname = fname;
  char com[500];
  while(f.read()) {
    f.disp();
    if(f.argc>2 && 0==strncmp(f.argv[1],"COMMAND",4)) {
      sprintf(com,"%s",f.argv[0]+8);
      //printf("%s\n",com);
      _phiRot=0.0;
      do {
        _thetaRot=0.0;
        do {
          printf("theta=%g phi=%g\n",theta+_thetaRot,phi+_phiRot);
          simcmain(com,overrideopt);
          _thetaRot+=thetaRotate;
        } while(fabs(_thetaRot)<thetaStop && thetaRotate!=0.0);
        _phiRot+=phiRotate;
      } while(fabs(_phiRot)<phiStop && phiRotate!=0.0);
      _thetaRot=0.0;
      _phiRot=0.0;
      break;
    }
  }
}


//////////////////////////////////////////////////////////////////////
int replayindex=Replay::Qd;
double replaymin=0;
double replaymax=0;
//////////////////////////////////////////////////////////////////////
void ROOT_callbackinit(SimA& sima,Matrix2<double>* D) {
  //printf("callbackinit\n");
  Disp2D(D[replayindex],_replayfname,Replay::replayname[replayindex],"",replaymin,replaymax);
}
//////////////////////////////////////////////////////////////////////
void ROOT_callback(SimA& sima,Matrix2<double>* D,double dt,int count) {
  //printf("callback T=%g\n",dt*count);
  Update2D(D[replayindex],_replayfname,Replay::replayname[replayindex],"",replaymin,replaymax);
}

//////////////////////////////////////////////////////////////////////
void ROOT_callbackinit(SimB& sima,Matrix3<double>* D) {
  //printf("callbackinit\n");
  Disp3D(D[replayindex],_replayfname,Replay::replayname[replayindex],"",replaymin,replaymax);
}
//////////////////////////////////////////////////////////////////////
void ROOT_callback(SimB& sima,Matrix3<double>* D,double dt,int count) {
  //printf("callback T=%g\n",dt*count);
  Update3D(D[replayindex],_replayfname,Replay::replayname[replayindex],"",replaymin,replaymax);
}
//////////////////////////////////////////////////////////////////////
int ROOT_interrupt(SimC& simc) {
  int stat=-1;
  printf("Processing ROOT_interrupt routine\n");
  if(strchr(simc.ID(),'A')) {
    SimA& sima=simc;
    //G__pause();
  }
  else {
    SimB& simb=simc;
    //G__pause();
  }
  return(stat);
}

//////////////////////////////////////////////////////////////////////
int ROOT_disp(char* fname) {
  ReadCSV(fname);
  return(stat);
}


//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

