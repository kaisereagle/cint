/****************************************************************************
* disp3D.cxx
*
* Display simulation result in ROOT 2D Histogram
*
****************************************************************************/

#include "displib.cxx"

/**********************************************************************
* 電位、電荷分布 Matrix 読み込みと表示
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
  int zsize = atoi(f.argv[3]); 
  llim = atof(f.argv[4]);
  ulim = atof(f.argv[5]);

  int i=0,j=0,v=0;

  sprintf(name,"h%d",a);
  sprintf(com,"%s:%s %s",fname,rmode,comment);

  if((match.size() && !strstr(com,match.c_str())) ||
     (not.size() && strstr(com,not.c_str()))) {
    for(v=0;v<zsize;v++) {
     for(j=0;j<ysize;j++) {
      f.read();
      if(f.argc>=1 && strncmp(f.argv[1],"END",3)==0) break;
     }
    }
    f.setseparator(" \t\v");
    return;
  }

  Matrix3<double> D;
  D.Allocate(xsize,ysize,zsize); 
  int Ysize=D.YSize();
  int Zsize=D.ZSize();
  double *mat = D.Data();

  vector<double> HX,HY,HZ; // nonuniform mesh size information
  for(v=0;v<zsize;v++) {
   for(j=0;j<ysize;j++) {
    f.read();
    if(!f.argc) continue;
    if(f.argv[1][0]=='/' || f.argv[1][0]=='#') continue;
    if(f.argc>=1 && strncmp(f.argv[1],"END",3)==0) break;
    for(i=0;i<xsize&&i<=f.argc;i++) {
      val = atof(f.argv[i+1]);
      mat[(i*ysize+j)*zsize+v] = val; 
      //D(i,j,v) = val;
    }
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
        f.read(); // Z
      }
      if(f.argc>=1 && strcmp(f.argv[1],"Z")==0) {
        HZ.clear();
        for(int i=2;i<=f.argc;i++) HZ.push_back(atof(f.argv[i]));
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
    Matrix3<double> Do;
    double ho=0,hi=0;
    Remesh3D(Do,ho,D,hi,HX,HY,HZ);
    D = Do;
  }

  Disp3D(D,fname,rmode,comment,llim,ulim);
  //if(HX.size()) Graph3D(D,fname,rmode,comment,llim,ulim,HX,HY,HZ);
  //else          Disp3D(D,fname,rmode,comment,llim,ulim);
}

/**********************************************************************
* 電位、電荷分布 Matrix 読み込みと表示
**********************************************************************/
void ReadMatrixBin(ReadF& f,char* fname,char* rmode,char* comment) {
  f.setseparator("");
  f.read();
  int xsize = atoi(f.argv[1]);
  int ysize = atoi(f.argv[2]);
  int zsize = atoi(f.argv[3]);
  double llim,ulim;
  llim = atof(f.argv[4]);
  ulim = atof(f.argv[5]);

  //f.setquotation("'\"");
  f.setseparator(" \t\v");
  f.read();
#if 1
  char binfname[300];
  strcpy(binfname,f.argv[2]);
#else
  string binfname = f.argv[2];
#endif
  vector<double> HX,HY,HZ; // nonuniform mesh size information
  f.read(); // END or X
  if(f.argc>=1 && strcmp(f.argv[1],"X")==0) {
    HX.clear();
    for(int i=2;i<=f.argc;i++) HX.push_back(atof(f.argv[i]));
    f.read(); // Y
  }
  if(f.argc>=1 && strcmp(f.argv[1],"Y")==0) {
    HY.clear();
    for(int i=2;i<=f.argc;i++) HY.push_back(atof(f.argv[i]));
    f.read(); // Z
  }
  if(f.argc>=1 && strcmp(f.argv[1],"Z")==0) {
    HZ.clear();
    for(int i=2;i<=f.argc;i++) HZ.push_back(atof(f.argv[i]));
    f.read(); // END
  }

  if((match.size() && !strstr(comment,match.c_str())) ||
     (not.size() && strstr(comment,not.c_str()))) {
    return;
  }

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
  Matrix3<double> D;
  D.Allocate(xsize,ysize,zsize); 
  int dmy;
  fread(&dmy, sizeof(int), 1, bfp);
  fread(&dmy, sizeof(int), 1, bfp);
  fread(&dmy, sizeof(int), 1, bfp);
  double *d = D.Data();
  fread(d,sizeof(double),D.Size(),bfp);
  fclose(bfp);

  if(remesh && HX.size()) { 
    Matrix3<double> Do;
    double ho=0,hi=0;
    Remesh3D(Do,ho,D,hi,HX,HY,HZ);
    D = Do;
  }

  Disp3D(D,fname,rmode,comment,llim,ulim);
  //if(HX.size()) Graph3D(D,fname,rmode,comment,llim,ulim,HX,HY,HZ);
  //else          Disp3D(D,fname,rmode,comment,llim,ulim);
}


///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
#if 0
int border[6] = { 0,0,0,0,0,0 };
void setborder(int a,int b,int c,int d,int e,int f) {
  border[0] = a;
  border[1] = b;
  border[2] = c;
  border[3] = d;
  border[4] = e;
  border[5] = f;
}
//double tmax=0.0;
///////////////////////////////////////////////////////////////////
int XSIZE() {
  if(x) {
    return(maxver - border[4]-border[5]);
  }
  else if(y) {
    return(maxcol - border[0]-border[1]);
  }
  else if(z) {
    return(maxcol - border[0]-border[1]);
  }
}
int YSIZE() {
  if(x) {
    return(maxrow - border[2]-border[3]);
  }
  else if(y) {
    return(maxver - border[4]-border[5]);
  }
  else if(z) {
    return(maxrow - border[2]-border[3]);
  }
}
int DRAW(int i,int j,int v) {
  if(!step) {
    if(x) return(i==x);
    else if(y) return(y==j);
    else if(z) return(z==v);
  }
  if(x) {
    return(i>=x && i<=max && !((i-x)%step));
  }
  else if(y) {
    return(j>=y && j<=max && !((j-y)%step));
  }
  else if(z) {
    return(v>=z && v<=max && !((v-z)%step));
  }
}

int NTH(int i,int j,int v) {
  if(!step) return(0);
  if(x) {
    return((i-x)/step);
  }
  else if(y) {
    return((j-y)/step);
  }
  else if(z) {
    return((v-z)/step);
  }
}

int NDX() {
  if(!step) return(1);
  else { 
    if(x) {
      if(max<x) max=x+step*2;
      return((max-x)/step+1);
    }
    else if(y) {
      if(max<y) max=y+step*2;
      return((max-y)/step+1);
    }
    else if(z) {
      if(max<z) max=z+step*2;
      return((max-z)/step+1);
    }
  }
}

char* LABEL(int n) {
  static char buf[80];
  if(!step) sprintf(buf,"");
  else { 
    if(x) {
      sprintf(buf,"x=%d\n",x+n*step);
    }
    else if(y) {
      sprintf(buf,"y=%d\n",y+n*step);
    }
    else if(z) {
      sprintf(buf,"z=%d\n",z+n*step);
    }
  }
  return(buf);
}

int X(int i,int j,int v) {
  if(x) {
    return(v-border[4]);
  }
  else if(y) {
    return(i-border[0]);
  }
  else if(z) {
    return(i-border[0]);
  }
}
int Y(int i,int j,int v) {
  if(x) {
    return(j-border[2]);
  }
  else if(y) {
    return(v-border[4]);
  }
  else if(z) {
    return(j-border[2]);
  }
}
#endif

