/************************************************************************
* cintgraph.cxx
*
*  makecint -A -dl xgraph.sl -c xgraph.c
************************************************************************/

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

#include "cintgraph.h"

#define QUOTATION

//char *graphbuf="/Users/gotom/root/cint/cint/graphbuf";
//char *flagfile="/Users/gotom/root/cint/cint/graphbuf/doplot";
#ifdef _WIN32
string cintgraphbuf = string("c:/cint") + "/graphbuf";
string cintflagfile = string("c:/cint") + "/graphbuf/doplot";
#else
string cintgraphbuf = string(getenv("CINTSYSDIR")) + "/graphbuf";
string cintflagfile = string(getenv("CINTSYSDIR")) + "/graphbuf/doplot";
#endif
/************************************************************************
* rootgraph library
************************************************************************/

int cintgraph_open(const char *filename,const char *title) {
  FILE *fp;
  char fname[500];
  sprintf(fname,"%s/%s",cintgraphbuf.c_str(),filename);
  fp=fopen(fname,"w");
  if(title) fprintf(fp,"title,%s\n",title);
  fclose(fp);
  return(0);
}

int cintgraph_data(const char *filename,DMatrix& xdata,DMatrix& ydata
		   ,int n,int ndata,const char *name,int color) {
  int i;
  FILE *fp;
  char fname[500];
  sprintf(fname,"%s/%s",cintgraphbuf.c_str(),filename);
  fp=fopen(fname,"a");
  fprintf(fp,"\n");
  //fprintf(fp,"yname,\"%s\"\n",name);
  fprintf(fp,"yname,%s\n",name);
  fprintf(fp,"color,%d\n",color);
  for(i=0;i<ndata;i++) {
    fprintf(fp,"%e,%e\n",xdata[n][i],ydata[n][i]);
  }
  fprintf(fp,"end\n");
  fclose(fp);
  return(0);
}
int cintgraph_data(const char *filename
		   ,double *xdata,double *ydata,int ndata
		   ,const char *name,int color) {
  int i;
  FILE *fp;
  char fname[500];
  sprintf(fname,"%s/%s",cintgraphbuf.c_str(),filename);
  fp=fopen(fname,"a");
  fprintf(fp,"\n");
  fprintf(fp,"yname,\"%s\"\n",name);
  fprintf(fp,"color,%d\n",color);
  for(i=0;i<ndata;i++) {
    fprintf(fp,"%e,%e\n",xdata[i],ydata[i]);
  }
  fprintf(fp,"end\n");
  fclose(fp);
  return(0);
}

int cintgraph_invoke(const char *filename
		     ,double xmin,double xmax,double ymin,double ymax
		     ,int xlog,int ylog
		     ,const char *xunit,const char *yunit) {
  //char temp1[200],temp2[200],temp3[80];
  char *pc;
  
  FILE *fp;
  char fname[500];
  sprintf(fname,"%s/%s",cintgraphbuf.c_str(),filename);
  fp=fopen(fname,"a");
  if( 0!=xmin || 0!=xmax ) {
    fprintf(fp,"xmin,%g\n",xmin);
    fprintf(fp,"xmax,%g\n",xmax);
  }
  if( 0!=ymin || 0!=ymax ) {
    fprintf(fp,"ymin,%g\n",ymin);
    fprintf(fp,"ymax,%g\n",ymax);
  }
  if(xunit && xunit[0]) {
    fprintf(fp,"xname,%s\n",xunit);
  }
  if(yunit && yunit[0]) {
    fprintf(fp,"yunit,%s\n",yunit);
  }
  if(xlog) {
    fprintf(fp,"xlog,%d",xlog);
  }
  if(ylog) {
    fprintf(fp,"ylog,%d",ylog);
  }
  fclose(fp);
  
  // create plot flag file
  fp=0;
  int n=0;
  while(!fp && n++<10000) {
    fp = fopen(cintflagfile.c_str(),"a");
  }
  if(fp) {
    //fprintf(fp,"%s\n",fname);
#ifdef QUOTATION
    fprintf(fp,"plot2D '%s'\n",fname);
#else
    fprintf(fp,"plot2D %s\n",fname);
#endif
  }
  fclose(fp);
  
  return(0);
}

int cintgraph_command(const char* command,const char* arg) {
  FILE *fp=0;
  int n=0;
  if(cintflagfile=="") {
#ifdef _WIN32
    cintgraphbuf = string("c:/cint") + "/graphbuf";
    cintflagfile = string("c:/cint") + "/graphbuf/doplot";
#else
    cintgraphbuf = string(getenv("CINTSYSDIR")) + "/graphbuf";
    cintflagfile = string(getenv("CINTSYSDIR")) + "/graphbuf/doplot";
#endif
  }
  while(!fp && n++<10000) {
    fp = fopen(cintflagfile.c_str(),"a");
  }
  if(fp) {
    fprintf(fp,"#%s",command);
    if(arg) fprintf(fp," %s",arg);
    fprintf(fp,"\n");
    fclose(fp);
  }
  else {
    fprintf(stderr,"Error: Failed creating '%s'\n",cintflagfile.c_str());
  }
  return(0);
}

int cintgraph_clear() {
  cintgraph_command("clear");
  return(0);
}

int cintgraph_save() {
  cintgraph_command("save");
  return(0);
}

int cintgraph_exit() {
  cintgraph_command("exit");
  return(0);
}

int cintgraph_stop() {
  cintgraph_command("stop");
  return(0);
}

int cintgraph_start() {
  //system("(cd $CINTSYSDIR/graphbuf;root $ROOTSYS/cint/cint/rootgraphdaemon.h &)");
  system("(cd $CINTSYSDIR/graphbuf;root &)");
  return(0);
}

/************************************************************************
* Rootgraph 2D/3D Histogram library
************************************************************************/
void cintgraph_Histogram2D(const char* filename ,const DMatrix& D
			   ,const char* title,const char* xname
			   ,double xmin,double xmax
			   ,double ymin,double ymax
			   ,double zmin,double zmax) {
  FILE *fp;
  char fname[500];
  sprintf(fname,"%s/%s",cintgraphbuf.c_str(),filename);
  fp=fopen(fname,"w");
  fprintf(fp,"title,%s\n",title);
  fprintf(fp,"xname,%s\n",xname);
  if( 0!=xmin || 0!=xmax ) {
    fprintf(fp,"xmin,%g\n",xmin);
    fprintf(fp,"xmax,%g\n",xmax);
  }
  if( 0!=ymin || 0!=ymax ) {
    fprintf(fp,"ymin,%g\n",ymin);
    fprintf(fp,"ymax,%g\n",ymax);
  }
  if( 0!=zmin || 0!=zmax ) {
    fprintf(fp,"zmin,%g\n",zmin);
    fprintf(fp,"zmax,%g\n",zmax);
  }
  if(xname && xname[0]) {
    fprintf(fp,"xname,%s\n",xname);
  }

  int i,j;
  int xsize = (D.size()>0)?D[0].size():0;
  int ysize = D.size();
  fprintf(fp,"xsize,%d\n",xsize);
  fprintf(fp,"ysize,%d\n",ysize);
  fprintf(fp,"begin\n");
  for(i=0;i<ysize;i++) {
    for(j=0;j<xsize;j++) {
      fprintf(fp,"%g,",D[i][j]);
    }
    fprintf(fp,"\n");
  }
  fprintf(fp,"end\n");
  fclose(fp);

  // create plot flag file
  fp=0;
  int n=0;
  while(!fp && n++<10000) {
    fp = fopen(cintflagfile.c_str(),"a");
  }
  if(fp) {
    //fprintf(fp,"%s\n",fname);
#ifdef QUOTATION
    fprintf(fp,"plot3D '%s'\n",fname);
#else
    fprintf(fp,"plot3D %s\n",fname);
#endif
  }
  fclose(fp);
  //cintgraph_stop() ;
}

//////////////////////////////////////////////////////////////
string winfile(const string& a) {
  string r;
  for(int i=0;i<a.size();i++) {
    switch(a[i]) {
    case '/': r += '\\'; break;
    default:  r += a[i]; break;
    }
  }
  return(r);
}

//////////////////////////////////////////////////////////////
int plot_cintgraph(const string& fnamein) {
  string fnamefull,fname;
  if(fnamein.substr(0,2)==string("./")) fnamefull=fnamein.substr(2);
  else fnamefull=fnamein;

  size_t pos=fnamefull.find_last_of("/\\");
  if(pos!=string::npos) fname=fnamefull.substr(pos+1);
  else                  fname=fnamefull;

#ifdef _WIN32
  fnamefull = winfile(fnamefull);
#endif

  char graphbuffile[1000],com[1000];
  sprintf(graphbuffile,"%s/%s",cintgraphbuf.c_str(),fname.c_str());
#ifdef _WIN32
  sprintf(com,"copy \"%s\" \"%s\"",fnamefull.c_str(),graphbuffile);
#else
  sprintf(com,"cp \"%s\" \"%s\"",fnamefull.c_str(),graphbuffile);
#endif
  printf("%s\n",com);
  system(com);

  FILE*  fp = fopen(cintflagfile.c_str(),"a");
  fprintf(fp,"plot2D '%s'\n",graphbuffile);
  fclose(fp);

  return(0);
}

/************************************************************************
* gnuplot library
************************************************************************/

int gnuplot_open(const char *filename,const char *title
		 ,const char* xname,const char* yunit
		 ,double xmin,double xmax,double ymin,double ymax
		 ,int xlog,int ylog
		 ) {
  FILE *fp;
  char fname[500];
  sprintf(fname,"%s/%s",cintgraphbuf.c_str(),filename);
  fp=fopen(fname,"w");
  //if(title) fprintf(fp,"title,%s\n",title);
  fprintf(fp,"set grid\n");
  if(title) fprintf(fp,"set title \"%s\"\n",title);
  if(xname) fprintf(fp,"set xlabel \"%s\"\n",xname);
  if(yunit) fprintf(fp,"set ylabel \"%s\"\n",yunit);
  if(xlog)  fprintf(fp,"set logscale x\n");
  if(ylog)  fprintf(fp,"set logscale y\n");
  if(xmin!=xmax) fprintf(fp,"set xrange [%g:%g]\n",xmin,xmax);
  if(ymin!=ymax) fprintf(fp,"set yrange [%g:%g]\n",ymin,ymax);
  
  fprintf(fp,"#set term x11 0 font clean persist\n");
  if(title) fprintf(fp,"plot \\\n",title);
  fclose(fp);
  return(0);
}
  
int gnuplot_title(const char *filename,int n,int ndata
                  ,const char *name,int color) {
  FILE *fp;
  char fname[500];
  sprintf(fname,"%s/%s",cintgraphbuf.c_str(),filename);
  fp=fopen(fname,"a");
  if(n>0) fprintf(fp,",");
  if(name) fprintf(fp,"'-' title \"%s\" lw 1 with lines\\\n",name);
  else     fprintf(fp,"'-' title \"%s\" lw 1 with lines\\\n","");
  fclose(fp);
  return(0);
}

int gnuplot_data(const char *filename,DMatrix& xdata,DMatrix& ydata
                 ,int n,int ndata,const char *name,int color) {
  int i;
  FILE *fp;
  printf("i=%d ndata=%d\n",n,ndata);
  char fname[500];
  sprintf(fname,"%s/%s",cintgraphbuf.c_str(),filename);
  fp=fopen(fname,"a");
  fprintf(fp,"\n");
  for(i=0;i<ndata;i++) {
    fprintf(fp,"%e\t%e\n",xdata[n][i],ydata[n][i]);
  }
  fprintf(fp,"e\n");
  fclose(fp);
  return(0);
}

int gnuplot_data(const char *filename
		   ,double *xdata,double *ydata,int ndata
		   ,const char *name,int color) {
  int i;
  FILE *fp;
  char fname[500];
  sprintf(fname,"%s/%s",cintgraphbuf.c_str(),filename);
  fp=fopen(fname,"a");
  fprintf(fp,"\n");
  for(i=0;i<ndata;i++) {
    fprintf(fp,"%e\t%e\n",xdata[i],ydata[i]);
  }
  fprintf(fp,"e\n");
  fclose(fp);
  return(0);
}

int gnuplot_invoke(const char *filename
		     ,double xmin,double xmax,double ymin,double ymax
		     ,int xlog,int ylog
		     ,const char *xunit,const char *yunit) {
  FILE *fp;
  char fname[500];
  sprintf(fname,"%s/%s",cintgraphbuf.c_str(),filename);
  fp=fopen(fname,"a");
  fprintf(fp,"pause -1\n");
  fclose(fp);
  
  char com[100];
  sprintf(com,"gnuplot %s",fname);
  system(com);
  remove(fname);
  
  return(0);
}

/////////////////////////////////////////////////////////
void gnuplot_Histogram2D(const char* filename ,const DMatrix& D
			 ,const char* title,const char* xname
			 ,double xmin,double xmax
			 ,double ymin,double ymax
			 ,double zmin,double zmax) {
  FILE *fp;
  char fname[500],commandfile[500];
  sprintf(commandfile,"%s/%s_gnu3Dcom",cintgraphbuf.c_str(),title);
  sprintf(fname,"%s/%s_gnu3Ddata",cintgraphbuf.c_str(),filename);
  printf("commandfile=%s  fname=%s\n",commandfile,fname);

  fp=fopen(commandfile,"w");
  fprintf(fp,"set grid\n");
  if(title) fprintf(fp,"set title \"%s\"\n",title);
  if(xname) fprintf(fp,"set xlabel \"%s\"\n",xname);
  if(xmin!=xmax) fprintf(fp,"set xrange [%g:%g]\n",xmin,xmax);
  if(ymin!=ymax) fprintf(fp,"set yrange [%g:%g]\n",ymin,ymax);
  if(zmin!=zmax) fprintf(fp,"set zrange [%g:%g]\n",zmin,zmax);
  fprintf(fp,"splot '%s' with lines \n",fname);
  //fprintf(fp,"splot '%s' with linespoits pt 2 ps 1\n",fname);
  fprintf(fp,"pause -1\n");
  fclose(fp);

  int i,j;
  int xsize = (D.size()>0)?D[0].size():0;
  int ysize = D.size();
  fp=fopen(fname,"w");
  for(i=0;i<ysize;i++) {
    for(j=0;j<xsize;j++) {
      fprintf(fp,"%g %g %g\n",(double)i,(double)j,D[i][j]);
    }
    fprintf(fp,"\n");
  }
  fclose(fp);

  // invoke gnuplot
  char com[100];
  sprintf(com,"gnuplot %s",commandfile);
  system(com);
  remove(commandfile);
  remove(fname);
}


/////////////////////////////////////////////////////////
static int g_graph_mode=1;

void cintgraph_mode() { g_graph_mode=1; }
void gnuplot_mode() {g_graph_mode=0;}
int graph_mode() {return(g_graph_mode); }

/////////////////////////////////////////////////////////

