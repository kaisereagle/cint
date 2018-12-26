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
* xgraph library
************************************************************************/

int cintgraph_open(const char *filename,const char *title)
{
	FILE *fp;
	char fname[500];
	sprintf(fname,"%s/%s",cintgraphbuf.c_str(),filename);
	fp=fopen(fname,"w");
	if(title) fprintf(fp,"title,%s\n",title);
	fclose(fp);
	return(0);
}

int cintgraph_data(const char *filename,DMatrix& xdata,DMatrix& ydata,int n,int ndata,const char *name)
{
	int i;
	FILE *fp;
	char fname[500];
	sprintf(fname,"%s/%s",cintgraphbuf.c_str(),filename);
	fp=fopen(fname,"a");
	fprintf(fp,"\n");
	//fprintf(fp,"yname,\"%s\"\n",name);
	fprintf(fp,"yname,%s\n",name);
	for(i=0;i<ndata;i++) {
		fprintf(fp,"%e,%e\n",xdata[n][i],ydata[n][i]);
	}
	fprintf(fp,"end\n");
	fclose(fp);
	return(0);
}
int cintgraph_data(const char *filename,double *xdata,double *ydata,int ndata,const char *name)
{
	int i;
	FILE *fp;
	char fname[500];
	sprintf(fname,"%s/%s",cintgraphbuf.c_str(),filename);
	fp=fopen(fname,"a");
	fprintf(fp,"\n");
	fprintf(fp,"yname,\"%s\"\n",name);
	for(i=0;i<ndata;i++) {
		fprintf(fp,"%e,%e\n",xdata[i],ydata[i]);
	}
	fprintf(fp,"end\n");
	fclose(fp);
	return(0);
}

int cintgraph_invoke(const char *filename,double xmin,double xmax,double ymin,double ymax
		     ,int xlog,int ylog,const char *xunit,const char *yunit)
{
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
          fprintf(fp,"plot2D %s\n",fname);
        }
        fclose(fp);

	return(0);
}
int cintgraph_command(const char* command) {
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
    fprintf(fp,"#%s\n",command);
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
  system("root $ROOTSYS/cint/cint/rootgraphdaemon.h &");
  return(0);
}

/************************************************************************
* 2D/3D Histogram library
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
    fprintf(fp,"plot3D %s\n",fname);
  }
  fclose(fp);
  //cintgraph_stop() ;
}


