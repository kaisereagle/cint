#include <NVector.h>
#include <ReadF.h>

/***********************************************************************
 * void ReadSpice()
 * 
 * Description:
 *  Read SPICE .print result from text report.
 ***********************************************************************/
class SpiceData {
 public:
  string xname;
  vector<string> yname;
  DVector x;
  DMatrix y;
  int nplot;
  int n;
  void split() {
    double x0 = x.front();
    int i,j,N,k;
    DVector tmp;
    DMatrix y_bk;
    vector<string> yname_bk;
    char buf[500];
    for(i=1;i<x.size();i++) if(x[i]==x0) {
      N=i;
      x.resize(N);
      for(j=0;j<y.size();j++) {
        k = -1;
        i = 0;
        while(i<y[j].size()) {
          if(i%N==0) {
            ++k;
            sprintf(buf,"%s %d",yname[j].c_str(),k);
            yname_bk.push_back(buf);
            y_bk.push_back(tmp);
          }
          y_bk[j+k].push_back(y[j][i++]);
        }
      }
      y = y_bk;
      yname = yname_bk;
      nplot = yname.size();
      n = N;
      return;
    }
  }
  void clear() {
    xname = "";
    yname.clear();
    x.clear();
    y.clear();
  }
};

void ReadSpice(char* fname="spicesweep_tmp.txt",SpiceData& s) {
  s.clear();
  int i,j;
  int num;
  ReadF t(fname);
  if(!t.isvalid()) {
    printf("Failed to open %s\n",fname);
  }
  vector<string> argv;
  while(t.read()) {
    if(t.argc==1&&strcmp(t.argv[1],"x")==0) break;
    argv.clear();
    for(i=0;i<t.argv.size();i++) argv.push_back(t.argv[i]);
  }

  if(argv.size()>=1) s.xname=argv[1];
  for(i=2;i<argv.size();i++) {
    s.yname.push_back(argv[i]);
    s.y.push_back(s.x);
  }

  t.read();
  while(t.read()) {
    if(t.argc==1&&strcmp(t.argv[1],"y")==0) break;
    s.x.push_back(atof(t.argv[1]));
    for(i=2;i<=t.argc;i++) {
      s.y[i-2].push_back(atof(t.argv[i]));
    }
    num = t.argc-1;
    s.nplot = num;
  }

  printf("SPICE data size=%d x %d\n",s.x.size(),s.nplot);
  if(s.x.size()<2) return;

  int n=s.x.size();
  s.n = n;

#if 0
  plot << fname << x  << s.xname.c_str() << xmin | xmax ;
  for(i=0;i<num;i++) plot << s.y[i] << s.yname[i].c_str();
  if(ymin!=ymax) plot << ymin >> ymax;
  plot << "\n";
#endif
}

/***********************************************************************
 * void ReadSpice()
 * 
 * Description:
 *  Read SPICE .print result from text report.
 ***********************************************************************/
void ReadSpice(const char* fname,DVector& x,DVector& y,int ndata=0,int offset=0)
{
  double res, span;
  ReadF f(fname);
 skip2:
  while(f.read()) {
    if(f.argc>1) {
      if(strcmp(f.argv[1],".tran")==0) {
	res = atof(f.argv[2]);
	span = atof(f.argv[3]);
      }
      if(strcmp(f.argv[1],"time")==0) {
	break;
      }
    }
  }

  int size = (span / res)+1;
  if(ndata) size = ndata;
  int i=0;
  x.resize(size);
  y.resize(size);
  f.read();
  if(f.argc!=1 || 0!=strcmp(f.argv[1],"x")) goto skip2;

  f.read();

  for(int jx=0;jx<offset;jx++) f.read();
  while(f.read() && i<size && f.argc>1) {
    //char *p=strstr(f.argv[2],"E+00"); if(p) *p=0;
    x[i] = atof(f.argv[1]);
    y[i] = atof(f.argv[2]);
#ifdef DEBUG
    printf("%s=%g\t\t%s=%g\n",f.argv[1],x[i],f.argv[2],y[i]);
#endif
    ++i;
  }
  if(0==ndata && i!=size) {
    x.resize(i);
    y.resize(i);
  }
#ifdef DEBUG
  printf("DVector size = %g/%g = %d true size %d\n",span,res,size,i);
#endif
}


/***********************************************************************
 * void ReadSpice()
 * 
 * Description:
 *  Read SPICE .print result from text report.
 ***********************************************************************/
void ReadSpice(const char* fname,double x[],double y[]
	       ,int ndata=0,int offset=0)
{
  double res, span;
  ReadF f(fname);
 skip2:
  while(f.read()) {
    if(f.argc>1) {
      if(strcmp(f.argv[1],".tran")==0) {
	res = atof(f.argv[2]);
	span = atof(f.argv[3]);
      }
      if(strcmp(f.argv[1],"time")==0) {
	break;
      }
    }
  }


  int size = (span / res)+1;
  if(ndata) size = ndata;
  int i=0;
  f.read();
  if(f.argc!=1 || 0!=strcmp(f.argv[1],"x")) goto skip2;
  f.read();

  for(int jx=0;jx<offset;jx++) f.read();
  while(f.read() && i<size && f.argc>1) {
    //char *p=strstr(f.argv[2],"E+00"); if(p) *p=0;
    x[i] = atof(f.argv[1]);
    y[i] = atof(f.argv[2]);
    //printf("%s=%g\t\t%s=%g\n",f.argv[1],x[i],f.argv[2],y[i]);
    ++i;
  }
#ifdef DEBUG
  printf("DVector size = %g/%g = %d true size %d\n",span,res,size,i);
#endif
}

/***********************************************************************
 * double todouble(const char* expr)
 * 
 * Description:
 *  Convert  f,p,n,u,m,k,meg,g,t to 1e-15,-12,-9,-6,-3,+3,+6,+9,+12
 ***********************************************************************/
double todouble(const char* expr) {
  double result= atof(expr); 
  if(strstr(expr,"a")) result *= 1e-18;
  else if(strstr(expr,"f")) result *= 1e-15;
  else if(strstr(expr,"p")) result *= 1e-12;
  else if(strstr(expr,"n")) result *= 1e-9;
  else if(strstr(expr,"u")) result *= 1e-6;
  else if(strstr(expr,"m")) result *= 1e-3;
  else if(strstr(expr,"k")) result *= 1e3;
  else if(strstr(expr,"meg")) result *= 1e6;
  else if(strstr(expr,"g")) result *= 1e9;
  else if(strstr(expr,"t")) result *= 1e12;
  else if(strstr(expr,"A")) result *= 1e-18;
  else if(strstr(expr,"F")) result *= 1e-15;
  else if(strstr(expr,"P")) result *= 1e-12;
  else if(strstr(expr,"N")) result *= 1e-9;
  else if(strstr(expr,"U")) result *= 1e-6;
  else if(strstr(expr,"M")) result *= 1e-3;
  else if(strstr(expr,"K")) result *= 1e3;
  else if(strstr(expr,"MEG")) result *= 1e6;
  else if(strstr(expr,"G")) result *= 1e9;
  else if(strstr(expr,"T")) result *= 1e12;
  return(result);
}


#include <NVector.h>
//#include <fft.h>
//#include <CSV.dll>
#include <ReadF.dll>
#include <EasyExp.dll>
#include <string>
#include <vector>
#include <map>

#define DETAIL

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

char *include[] = { ".csv", "" };

int isinclude(char* name) {
  int flag=0;
  int i=0;
  while(include[i][0]) {
    if(strstr(name,include[i++])) flag=1;
  }
  return(flag);
}
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
void disp(void (*f)(char*),char* expr) {
  //Clean();
  //init();
  char *originaldir=".";

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
    f(fname.c_str());
    ++first;
  }

  //terminate();
}


/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
void dispraw(char* fname="spicesweep_tmp.txt",double xmin=0,double xmax=0,double ymin=0,double ymax=0) {
  int i,j;
  string xname,yname[8];
  vector<double> vx,vy[8];
  int num;
  ReadF t(fname);
  if(!t.isvalid()) {
    printf("Failed to open %s\n",fname);
  }
  while(t.read()) {
    if(t.argc==1&&strcmp(t.argv[1],"x")==0) break;
    if(t.argc>=1) xname=t.argv[1];
    for(i=2;i<=t.argc&&i<7;i++) yname[i-2]=t.argv[i];
  }
  t.read();
  while(t.read()) {
    if(t.argc==1&&strcmp(t.argv[1],"y")==0) break;
    vx.push_back(atof(t.argv[1]));
    for(i=2;i<=t.argc;i++) {
      vy[i-2].push_back(atof(t.argv[i]));
    }
    num = t.argc-1;
  }

  printf("size=%d \n",vx.size());

  if(xmin==xmax) {
    xmin = min(vx);
    xmax = max(vx);
  }

  int n=vx.size();
  DVector x(vx);
  plot << fname
       << x  << xname.c_str() << xmin >> xmax ;

  DVector y[8];
  for(i=0;i<num;i++) {
    y[i] = vy[i];
    //for(j=0;j<n;j++) {y[i][j] = vy[i][j];}
    plot << y[i] << yname[i].c_str();
  }
  if(ymin!=ymax) plot << ymin >> ymax;
  plot << "\n";
}

///////////////////////////////////////////////////////////////////////
void dispfft(char* fname="spicesweep_tmp.txt",double xmin=500e-9,double xmax=520e-9,double fmax=1e9) {
  int i,j;
  string xname,yname[8];
  vector<double> vx,vy[8];
  int num;
  ReadF t(fname);
  if(!t.isvalid()) {
    printf("Failed to open %s\n",fname);
  }
  while(t.read()) {
    if(t.argc==1&&strcmp(t.argv[1],"x")==0) break;
    if(t.argc>=1) xname=t.argv[1];
    for(i=2;i<=t.argc&&i<7;i++) yname[i-2]=t.argv[i];
  }
  t.read();
  while(t.read()) {
    if(t.argc==1&&strcmp(t.argv[1],"y")==0) break;
    vx.push_back(atof(t.argv[1]));
    for(i=2;i<=t.argc;i++) {
      vy[i-2].push_back(atof(t.argv[i]));
    }
    num = t.argc-1;
  }

  printf("size=%d \n",vx.size());


  int n=vx.size();
  DVector x(&vx.front(),vx.size());

  plot << fname
       << x  << xname.c_str();

  DVector y[8];
  DVector f, sp[8]; 
  for(i=0;i<num;i++) {
    y[i] = vy[i];
    //for(j=0;j<n;j++) {y[i][j] = vy[i][j];}
    plot << y[i] << yname[i].c_str();
    spectrum << x << y[i]-mean(y[i]) >> f >> sp[i] << endl;
  }
  plot << "\n";

  if(xmin==xmax) {
    xmin = vx.front();
    xmax = vx.back();
  }
  plot << fname
       << x  << xname.c_str() << xmin | xmax;

  for(i=0;i<num;i++) {
    plot << y[i] << yname[i].c_str();
  }
  plot << "\n";


  plot << fname
       << f  << "Freq" << 0.0 | 5e9 ;
  for(i=0;i<num;i++) {
    plot << sp[i] << yname[i].c_str();
  }
  plot << "\n";
  
  plot << fname
       << f  << "Freq"  << 0.0 | fmax ;
  for(i=0;i<num;i++) {
    plot << sp[i] << yname[i].c_str();
  }
  plot << "\n";
 
}
