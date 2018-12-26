/***************************************************************************
* tanidaGraph.h
*
*   gr(char* regexp,xmin,xmax,ymin,ymax,char* directory);
*
****************************************************************************/
#include <array.h>
#include <vector>
#include <map>
#include <ReadF.h>
#include <RegExp.h>

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
const int gr_MAXLINES=50;
string gr_title[gr_MAXLINES];
string gr_xname[gr_MAXLINES];
string gr_yname[gr_MAXLINES];
string gr_linename[gr_MAXLINES];
array gr_x[gr_MAXLINES];
array gr_y[gr_MAXLINES];

int gr_samenu=0;
int gr_a=0;
int gr_start=0,gr_stop=0;
double gr_dmin=1e99,gr_dmax=-1e99;
int gr_linenameoffset=0;

//////////////////////////////////////////////////////////////////////
void gr(char* expr
        ,char* originaldir="."
        ,double xmin=0.0,double xmax=0.0
        ,double ymin=0.0,double ymax=0.0) {
  map<string,int> files;

  gr_a =0;
  gr_dmin=1e99;
  gr_dmax=-1e99;
  RegExp rege(expr);
  char in[500];
  char* xdir[] = {
     //"",
     "../",
     "../../",
     0
  };
  int i=0;

#ifdef G__ROOT
  TSystemDirectory dir;
  dir.SetDirectory(originaldir);
  TList *pl = dir.GetListOfFiles();
  TIter next(pl);
  TObject *obj;

  while((obj=next())) {
    if(rege.match(obj->GetName())) {
      //printf("%s\n",obj->GetName());
      sprintf(in,"%s",obj->GetName());
      //_grz(in,xmin,xmax,ymin,ymax);
      files[in]=1;
    }
  }
  while(files.size()==0 && xdir[i]) {
    char originaldir2[500];
    sprintf(originaldir2,"%s%s",xdir[i++],originaldir);
    dir.SetDirectory(originaldir2);
    pl = dir.GetListOfFiles();
    TIter next(pl);
    while((obj=next())) {
      if(rege.match(obj->GetName())) {
          //printf("%s\n",obj->GetName());
        sprintf(in,"%s",obj->GetName());
        //_grz(in,xmin,xmax,ymin,ymax);
        files[in]=1;
      }
    }
  }
#else
  struct DIR *dir = opendir(originaldir);
  struct dirent *d;
  int s;
  struct stat buf;
  while((d=readdir(dir))) {
    s=stat(d->d_name,&buf);
    if(rege.match(d->d_name)) {
      //printf("%s\n",d->d_name);
      sprintf(in,"%s",d->d_name);
      //_grz(in,xmin,xmax,ymin,ymax);
      files[in]=1;
    }
  }
  closedir(dir);
#endif

  map<string,int>::iterator first=files.begin();
  map<string,int>::iterator last =files.end();
  char fname[500];
  char *p;
  while(first!=last) {
    p = (*first).first.c_str();
    if(strcmp(originaldir,".")!=0) {
      //sprintf(fname,"%s/%s",originaldir,p);
      sprintf(fname,"%s",p);
    }
    else sprintf(fname,"%s",p);
    printf("%s\n",fname);
    _grz(fname,xmin,xmax,ymin,ymax);
    ++first;
  }

  if(gr_samenu && (gr_a)%gr_samenu) _grz_draw(xmin,xmax,ymin,ymax);
}

//////////////////////////////////////////////////////////////////////
void _grz(const char *fname
          ,double xmin=0.0,double xmax=0.0
          ,double ymin=0.0,double ymax=0.0) {
  char tmp[200];
  int i;
  ReadF f(fname);
  f.setseparator(",");
  char *p = strrchr(fname,'\\');;
  if(!p) p=fname;
  gr_title[gr_a] = p;
  char *linename;
  vector<double> xd,yd;
  while(f.read()) {
    //f.disp();
    if(f.argc>0) {
      if(strcmp(f.argv[1],"<DATA>")==0) {
        f.read();
        //f.disp();
        gr_xname[gr_a] = f.argv[1];
        gr_yname[gr_a] = f.argv[2];
        while(f.read()) {
          if(strcmp(f.argv[1],"</DATA>")==0 || f.argc==0) break;
          xd.push_back(atof(f.argv[1]));
          yd.push_back(atof(f.argv[2]));
        }
        gr_x[gr_a].resize(xd.size());
        gr_y[gr_a].resize(yd.size());
        for(i=0;i<xd.size();i++) {
          gr_x[gr_a][i] = xd[i];
          gr_y[gr_a][i] = yd[i];
          if(gr_dmin>yd[i]) gr_dmin=yd[i];
          if(gr_dmax<yd[i]) gr_dmax=yd[i];
        }

        if(gr_linenameoffset) {
          linename = gr_title[gr_a].c_str()+gr_linenameoffset;
        }
        else {
          linename = strchr(gr_title[gr_a].c_str(),'_');
          if(!linename) linename=title.c_str();
          else linename++;
          char *pos,*orig=gr_title[gr_a].c_str();
          if((pos=strchr(orig,'0'))&&pos<linename) linename=pos;
          if((pos=strchr(orig,'2'))&&pos<linename) linename=pos;
          if((pos=strchr(orig,'3'))&&pos<linename) linename=pos;
          if((pos=strchr(orig,'4'))&&pos<linename) linename=pos;
          if((pos=strchr(orig,'5'))&&pos<linename) linename=pos;
          if((pos=strchr(orig,'6'))&&pos<linename) linename=pos;
          if((pos=strchr(orig,'7'))&&pos<linename) linename=pos;
          if((pos=strchr(orig,'8'))&&pos<linename) linename=pos;
          if((pos=strchr(orig,'9'))&&pos<linename) linename=pos;
          if((pos=strchr(orig,'.'))&&pos<linename) linename=pos;
        }
        gr_linename[gr_a]=linename;

        if(gr_samenu==0) {
          // one data on one graph
          sprintf(tmp,"%s\n",gr_yname[gr_a].c_str());
          plot << gr_title[gr_a].c_str()
               << gr_x[gr_a] << gr_xname[gr_a].c_str() << xmin >> xmax
               << gr_y[gr_a] << gr_linename[gr_a].c_str() << ymin >> ymax
               << tmp;
        }
        else if(gr_a%gr_samenu==0) {
          // multiple data on one graph, start
          gr_start = gr_a;
        }
        else if((gr_a+1)%gr_samenu==0) {
          gr_a++;
          _grz_draw(xmin,xmax,ymin,ymax);
          gr_a--;
        }

        gr_a++;
        if(gr_a>=gr_MAXLINES) gr_a=0;
        return;
      }
    }
  }
}

void _grz_draw(double xmin,double xmax,double ymin,double ymax) {
  // multiple data on one graph, end, draw all
  int i;
  char tmp[200];
  gr_stop=gr_a;
  if(ymin==ymax) {
    ymin=gr_dmin;
    ymax=gr_dmax;
  }
  gr_dmin=1e99;
  gr_dmax=-1e99;
  sprintf(tmp,"%s\n",gr_yname[gr_start].c_str());
  plot << gr_title[gr_start].c_str()
       << gr_x[gr_start] << gr_xname[gr_start].c_str()<<xmin>>xmax
       << gr_y[gr_start] << gr_linename[gr_start].c_str()<<ymin>>ymax
       << tmp;
  for(i=gr_start+1;i<gr_stop;i++) {
    sprintf(tmp,"%s\n",gr_yname[i].c_str());
    same << title.c_str()
         << gr_x[i] << gr_xname[i].c_str() << xmin >> xmax
	 << gr_y[i] << gr_linename[i].c_str() << ymin >> ymax
	 << tmp; 
  }
}

