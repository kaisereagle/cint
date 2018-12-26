
#include <ReadFile.cxx>
//#include <array.h>
#include <vector>
#include <rootgraph.cxx>
#include <ROOT_histogram.h>

#define QUOTATION

//char *graphbuf="/Users/gotom/root/cint/cint/graphbuf";
//char *flagfile="/Users/gotom/root/cint/cint/graphbuf/doplot";
string graphbuf = string(getenv("CINTSYSDIR")) + "/graphbuf";
string flagfile = string(getenv("CINTSYSDIR")) + "/graphbuf/doplot";

int looping=1;
////////////////////////////////////////////////////////////////////////////
int Remove(const char* fname) {
  remove(fname);
  char* p=strrchr(fname,'/');
  if(p) {
    remove(p+1);
  }
}
////////////////////////////////////////////////////////////////////////////
string removequotation(const string& a) {
  string r;
  for(int i=0;i<a.size();i++) {
    switch(a[i]) {
    case '\'':
    case '\"':
      break;
    default:
      r += a[i];
    }
  }
  return(r);
}
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
int checkfile(vector<string>& filesXY,vector<string>& filesH2D,vector<string>& filesH3D) {
  static int cwd=0;
  FILE *fp=0;
  while(!fp) {
    if(cwd==0) fp = fopen(flagfile.c_str(),"r");
    if(!fp) {
      size_t p = flagfile.find_last_of("/");
      if(p!=string::npos) {
	fp = fopen(flagfile.substr(p+1).c_str(),"r");
        cwd=1;
      }
    }
    gSystem->Sleep(100); // ms wait
  }
  fclose(fp);
  ReadFile f(flagfile.c_str());
  f.setquotation("\"'");
  if(!f.isvalid()) {
    size_t p = flagfile.find_last_of("/");
    if(p!=string::npos) {
      f.open(flagfile.substr(p+1).c_str());
      cwd=1;
    }
  }
  while(f.read()) {
    if(f.argc>=1) {
      f.disp();
      if(strcmp(f.argv[1],"#reset")==0) rootgraph_reset();
      else if(strcmp(f.argv[1],"#clear")==0) rootgraph_clear();
      else if(strcmp(f.argv[1],"#save")==0) rootgraph_save();
      else if(strcmp(f.argv[1],"#stop")==0) {
	f.close();
	Remove(flagfile.c_str());
	looping=0;
      }
      else if(strcmp(f.argv[1],"#exit")==0) {
	f.close();
	Remove(flagfile.c_str());
	exit();
      }
      else if(strcmp(f.argv[1],"#option")==0) {
	rootgraph_option(f.argv[2]);
      }
      else if(strcmp(f.argv[1],"#option2D")==0) {
	rh_opt2 = f.argv[2];
      }
      else if(strcmp(f.argv[1],"#lineatt")==0) {
	rootgraph_lineatt(atoi(f.argv[2]));
      }
      else if(strcmp(f.argv[1],"plot2D")==0) {
#ifdef QUOTATION
	filesXY.push_back(removequotation(f.argv[2]));
#else
	filesXY.push_back(f.argv[2]);
#endif
      }
      else if(strcmp(f.argv[1],"plot3D")==0) {
#ifdef QUOTATION
	filesH2D.push_back(removequotation(f.argv[2]));
#else
	filesH2D.push_back(f.argv[2]);
#endif
      }
      else if(strcmp(f.argv[1],"plotScatter")==0) {
#ifdef QUOTATION
	filesH3D.push_back(removequotation(f.argv[2]));
#else
	filesH3D.push_back(f.argv[2]);
#endif
      }
      else filesXY.push_back(f.argv[0]);
    }
  }
  f.close();
  Remove(flagfile.c_str());
  return(0);
}
////////////////////////////////////////////////////////////////////////////
int doplotXY(const vector<string>& files) {
  double xmin=0,xmax=0,ymin=0,ymax=0;
  int xlog=0,ylog=0;
  vector<double> x,y;
  char *fname;
  ReadFile f;
  f.setseparator("\r");
  f.setdelimitor(",");
  int n=0;
  string title,xname,yname,yunit;
  int color=-1;
  int openflag=0;
  for(int i=0;i<files.size();i++) {
    fname = files[i].c_str();
    f.open(fname);
    if(!f.isvalid()) {
      char *p = strrchr(fname,'/');
      if(p) f.open(p+1);
    }
    x.clear(); y.clear();
    int nplot=0;
    while(f.read()) {
      if(f.argc>=2) {
        if(strcmp(f.argv[1],"title")==0) {
	  title=f.argv[2];
          if(!openflag) rootgraph_open("",title.c_str(),RG_PLOTMODE);
          ++openflag;
	}
        else if(strcmp(f.argv[1],"xname")==0) xname=f.argv[2];
        else if(strcmp(f.argv[1],"yname")==0) yname=f.argv[2];
        else if(strcmp(f.argv[1],"color")==0) color=atoi(f.argv[2]);
        else if(strcmp(f.argv[1],"yunit")==0) yunit=f.argv[2];
        else if(strcmp(f.argv[1],"xmin")==0) xmin=atof(f.argv[2]);
        else if(strcmp(f.argv[1],"xmax")==0) xmax=atof(f.argv[2]);
        else if(strcmp(f.argv[1],"ymin")==0) ymin=atof(f.argv[2]);
        else if(strcmp(f.argv[1],"ymax")==0) ymax=atof(f.argv[2]);
        else if(strcmp(f.argv[1],"xlog")==0) xlog=atoi(f.argv[2]);
        else if(strcmp(f.argv[1],"ylog")==0) ylog=atoi(f.argv[2]);
        else {
          x.push_back(atof(f.argv[1]));
          y.push_back(atof(f.argv[2]));
	}
      }
      else if(f.argc>=1) {
	if(strcmp(f.argv[1],"end")==0 && x.size()>1 && y.size()>1 ) {
          if(!openflag) rootgraph_open("",title.c_str(),RG_PLOTMODE);
          ++openflag;
          rootgraph_data("",&x[0],&y[0],x.size(),yname.c_str(),color);
	  nplot++;
	  x.clear(); y.clear();
	}
      }
    }
    if(nplot) {
      rootgraph_invoke("",xmin,xmax,ymin,ymax,xlog,ylog
		       ,xname.c_str(),yunit.c_str()
		       ,RG_PLOTMODE);
      rootgraph_clear();
      openflag=0;
      ++n;
    }
    f.close();
    Remove(fname);
  }
  files.clear();
  return(n);
}

////////////////////////////////////////////////////////////////////////////
int doplotH2D(const vector<string>& files) {
  double xmin=0,xmax=0,ymin=0,ymax=0,zmin=0,zmax=0;
  int xlog=0,ylog=0;
  int xsize=0,ysize=0;
  Matrix2<double> D;
  int x,y;
  char *fname;
  ReadFile f;
  f.setseparator("\r");
  f.setdelimitor(",");
  int n=0;
  string title,xname,yname,yunit;
  int openflag=0;
  for(int i=0;i<files.size();i++) {
    fname = files[i].c_str();
    f.open(fname);
    if(!f.isvalid()) {
      char *p = strrchr(fname,'/');
      if(p) f.open(p+1);
    }
    int nplot=0;
    x = y = 0;
    while(f.read()) {
      //f.disp();
      if(f.argc>=2) {
        if(strcmp(f.argv[1],"title")==0) {
	  title=f.argv[2];
          ++openflag;
	}
        else if(strcmp(f.argv[1],"xname")==0) xname=f.argv[2];
        else if(strcmp(f.argv[1],"yname")==0) yname=f.argv[2];
        else if(strcmp(f.argv[1],"yunit")==0) yunit=f.argv[2];
        else if(strcmp(f.argv[1],"xmin")==0) xmin=atof(f.argv[2]);
        else if(strcmp(f.argv[1],"xmax")==0) xmax=atof(f.argv[2]);
        else if(strcmp(f.argv[1],"ymin")==0) ymin=atof(f.argv[2]);
        else if(strcmp(f.argv[1],"ymax")==0) ymax=atof(f.argv[2]);
        else if(strcmp(f.argv[1],"zmin")==0) zmin=atof(f.argv[2]);
        else if(strcmp(f.argv[1],"zmax")==0) zmax=atof(f.argv[2]);
        else if(strcmp(f.argv[1],"xlog")==0) xlog=atoi(f.argv[2]);
        else if(strcmp(f.argv[1],"ylog")==0) ylog=atoi(f.argv[2]);
        else if(strcmp(f.argv[1],"xsize")==0) {
	  xsize=atoi(f.argv[2]);
	  if(xsize>0 && ysize>0) D.Allocate(xsize,ysize);
	}
        else if(strcmp(f.argv[1],"ysize")==0) {
	  ysize=atoi(f.argv[2]);
	  if(xsize>0 && ysize>0) D.Allocate(xsize,ysize);
	}
        else {
	  if(D.XSize()==0 || D.YSize()==0) { /* error */ }
          //if(y>=D.YSize()) break; /* error */
	  //G__pause();
	  for(x=1;x<=f.argc&&x<=D.XSize();x++) {
	    D(x-1,y) = atof(f.argv[x]);
	    //printf("%g,",D(x-1,y));
          }
	  ++y;
	  //printf("\n");
	}
      }
      else if(f.argc>=1) {
	if(strcmp(f.argv[1],"end")==0) {
	  nplot++;
	}
      }
    }
    if(nplot) {
  	  //G__pause();
      SetHistRange(xmin,xmax,ymin,ymax,0,0);
      DispHistogram2D(D,title.c_str(),xname.c_str());
      openflag=0;
      ++n;
    }
    f.close();
    Remove(fname);
  }
  files.clear();
  return(n);
}
////////////////////////////////////////////////////////////////////////////
int doplotH3D(const vector<string>& files) {
  return(0);
}
////////////////////////////////////////////////////////////////////////////
void daemonloop() { // NOT USED NOW
  //G__optimizemode(3);
  looping=1;
  vector<string> filesXY,filesH2D,filesH3D;
  while(looping) {
    checkfile(filesXY,filesH2D,filesH3D);
    doplotXY(filesXY);
    doplotH2D(filesH2D);
    doplotH3D(filesH3D);
  }
}
////////////////////////////////////////////////////////////////////////////
void CheckFile() {
  vector<string> filesXY,filesH2D,filesH3D;
  checkfile(filesXY,filesH2D,filesH3D);
  doplotXY(filesXY);
  doplotH2D(filesH2D);
  doplotH3D(filesH3D);
  if(looping)  timer->TurnOn();
  else         timer->TurnOff();
}

////////////////////////////////////////////////////////////////////////////
TTimer *timer;
void rootgraphdaemon() {
  looping = 1;
  timer = new TTimer(20);
  timer->SetCommand("CheckFile()");
  timer->TurnOn();
}
void loop() { rootgraphdaemon();}
void l() { rootgraphdaemon();}

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
