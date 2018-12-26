/****************************************************************************
* disp2D.cxx
*
* Display simulation result in ROOT 2D Histogram
*
****************************************************************************/

#include "displib.cxx"


/**********************************************************************
* 電位、電荷分布 Matrix 読み込みと 2D Histogramによる 3D 表示
**********************************************************************/
void ReadMatrix(ReadF& f,char* fname,char* rmode,char* comment) {
  // printf("ReadMatrix %s %s\n",rmode,comment);
  char name[20];
  char com[105];
  f.setseparator("");
  f.read();
  double llim,ulim,val;
  int xsize = atoi(f.argv[1]);
  int ysize = atoi(f.argv[2]);
  llim = atof(f.argv[3]);
  ulim = atof(f.argv[4]);
  int i=0,j=0;

  sprintf(name,"h%d",a);
  sprintf(com,"%s:%s %s",fname,rmode,comment);

#if 0
  if((match.size() && !Rmatch.match(com)) ||
     (not.size() && Rnot.match(com))){
    for(j=0;j<ysize;j++) {
      f.read();
      if(f.argc>=1 && strncmp(f.argv[1],"END",3)==0) break;
    }
    f.setseparator(" \t\v");
    return;
  }
#else
  if((match.size() && !strstr(com,match.c_str())) ||
     (not.size() && strstr(com,not.c_str()))) {
    for(j=0;j<ysize;j++) {
      f.read();
      if(f.argc>=1 && strncmp(f.argv[1],"END",3)==0) break;
    }
    f.setseparator(" \t\v");
    return;
  }
#endif

  Matrix2<double> D;
  D.Allocate(xsize,ysize); 
  int Ysize = D.YSize();
  double *mat=D.Data();

  vector<double> HX,HY; // nonuniform mesh size information
  double h=0;
  for(j=0;j<ysize;j++) {
    f.read();
    if(!f.argc) continue;
    if(f.argv[1][0]=='/' || f.argv[1][0]=='#') continue;
    if(f.argc>=1 && strncmp(f.argv[1],"END",3)==0) break;
    for(int i=0;i<xsize&&i<=f.argc;i++) {
      val = atof(f.argv[i+1]);
      mat[i*ysize+j] = val; 
    }
  }

  if(strcmp(f.argv[1],"END")!=0) {
    f.read(); // X or H
    if(f.argc>=1 && strcmp(f.argv[1],"X")==0) {
      HX.clear();
      for(int i=2;i<=f.argc;i++) HX.push_back(atof(f.argv[i]));
      f.read(); // Y
      if(f.argc>=1 && strcmp(f.argv[1],"Y")==0) {
        HY.clear();
        for(int i=2;i<=f.argc;i++) HY.push_back(atof(f.argv[i]));
        f.read(); // END
      }
    }
    else if(f.argc>=1 && strcmp(f.argv[1],"H")==0) {
      h = atof(f.argv[2]);
      f.read(); // END
    }
  }

  f.setseparator(" \t\v");

  if(remesh && HX.size()) { 
    Matrix2<double> Do;
    double ho=0,hi=0;
    Remesh2D(Do,ho,D,hi,HX,HY);
    D = Do;
  }

  Disp2D(D,fname,rmode,comment,llim,ulim);
  //if(HX.size()) Graph2D(D,fname,rmode,comment,llim,ulim,HX,HY);
  //else          Disp2D(D,fname,rmode,comment,llim,ulim);
}

/**********************************************************************
* 電位、電荷分布 Matrix 読み込みと表示
**********************************************************************/
void ReadMatrixBin(ReadF& f,char* fname,char* rmode,char* comment) {
  f.setseparator("");
  f.read();
  int xsize,ysize;
  xsize = atoi(f.argv[1]);
  ysize = atoi(f.argv[2]);
  double llim,ulim;
  llim = atof(f.argv[3]);
  ulim = atof(f.argv[4]);

  //f.setquotation("'\"");
  f.setseparator(" \t\v");
  f.read();
#if 1
  char binfname[300];
  strcpy(binfname,f.argv[2]);
#else
  string binfname = f.argv[2];
#endif
  vector<double> HX,HY; // nonuniform mesh size information
  double h=0;
  f.read(); // END or X or H
  if(f.argc>=1 && strcmp(f.argv[1],"X")==0) {
    HX.clear();
    for(int i=2;i<=f.argc;i++) HX.push_back(atof(f.argv[i]));
    f.read(); // Y
    if(f.argc>=1 && strcmp(f.argv[1],"Y")==0) {
      HY.clear();
      for(int i=2;i<=f.argc;i++) HY.push_back(atof(f.argv[i]));
      f.read(); // END
    }
  }
  else if(f.argc>=1 && strcmp(f.argv[1],"H")==0) {
    h = atof(f.argv[2]);
    f.read(); // END
  }
  

#if 0
  if((match.size() && !Rmatch.match(com)) ||

    return;
  }
#else
  if((match.size() && !strstr(comment,match.c_str())) ||
     (not.size() && strstr(comment,not.c_str()))) {
    return;
  }
#endif

  //printf("%s %s %s\n",f.argv[0],f.argv[2],binfname.c_str());

#if 1
  FILE *bfp = OpenBinaryFile(fname,binfname);
  if(!bfp) return;
#else
  FILE *bfp = fopen(binfname.c_str(),"rb");
  if(!bfp) {
    fprintf(stderr,"Failed to open %s\n",binfname.c_str());
    return;
  }
#endif
  Matrix2<double> D;
  D.Allocate(xsize,ysize); 
  int dmy;
  fread(&dmy, sizeof(int), 1, bfp);
  fread(&dmy, sizeof(int), 1, bfp);
  double *d = D.Data();
  fread(d,sizeof(double),D.Size(),bfp);
  fclose(bfp);

  if(remesh && HX.size()) { 
    Matrix2<double> Do;
    double ho=0,hi=0;
    Remesh2D(Do,ho,D,hi,HX,HY);
    D = Do;
  }

  Disp2D(D,fname,rmode,comment,llim,ulim);
  //if(HX.size()) Graph2D(D,fname,rmode,comment,llim,ulim,HX,HY);
  //else          Disp2D(D,fname,rmode,comment,llim,ulim);
}


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
#if 0
int border[4] = { 0,0,0,0 };
void setborder(int a,int b,int c,int d) {
  border[0] = a;
  border[1] = b;
  border[2] = c;
  border[3] = d;
}
double tmax=0.0;
#endif
