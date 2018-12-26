#include <excel.dll>
#include <unistd.h>

#ifdef G__WIN32
char *cp = "copy";
char *sep = "\\";
#else
char *cp = "cp";
char *sep = "/";
#endif

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
void cpall(char *targetdir) {
  cpfile("UPDATELINUX.cxx",targetdir);

#if 1

  // ROOT GRAPH
  cpfile("include/Vectoriostream.h",targetdir);
  cpfile("include/NVector.h",targetdir);
  cpfile("include/arrayiostream.h",targetdir);
  cpfile("include/tanidaGraph.h",targetdir);
  //cpfile("include/darray.h",targetdir);
  //cpfile("include/carray.h",targetdir);
  //cpfile("include/array.c",targetdir);
  cpfile("include/rootgraph.cxx",targetdir);
  cpfile("include/graphbuf.h",targetdir);
  cpfile("include/CSV.h",targetdir);
  cpfile("include/NNCluster.h",targetdir);
  cpfile("include/excel.h",targetdir);
  cpfile("include/SPICE.h",targetdir);
  cpfile("include/histogram.h",targetdir);
  cpfile("include/Matrix23.h",targetdir);
  cpfile("include/rmcrlf.cxx",targetdir);
  cpfile("include/_complex.h",targetdir);
  cpfile("include/spice.h",targetdir);
  cpfile("include/_iostream",targetdir);
  cpfile("include/NeuralNet.h",targetdir);
  cpfile("include/NeuralNetOld.h",targetdir);
  cpfile("include/_NN.cxx",targetdir);
  cpfile("include/VirtualGraph.h",targetdir);
#endif

#if 1
  // READFILE
  cpfile("include/ReadF.cxx",targetdir);
  cpfile("include/ReadF.C",targetdir);
  cpfile("include/ReadF.h",targetdir);
  // EasyExp
  cpfile("include/EasyExp.cxx",targetdir);
  cpfile("include/EasyExp.h",targetdir);
  cpfile("include/EachFile.h",targetdir);
#endif

  //cpfile("lib/Excel/LinkDef.h",targetdir);
#if 1
  // Excel
  cpfile("lib/Excel",targetdir,"*.cxx");
  cpfile("lib/Excel",targetdir,"*.h");
  cpfile("lib/Excel",targetdir,"make*");
  cpfile("lib/Excel",targetdir,"setup*");
  cpfile("lib/Excel",targetdir,"*.cxx");
  cpfile("include/csvPrompt.cxx",targetdir);
#endif

  // NVector
  cpfile("lib/NVector",targetdir,"*.cxx");
  cpfile("lib/NVector",targetdir,"*.h");
  cpfile("lib/NVector",targetdir,"make*");
  cpfile("lib/NVector",targetdir,"setup*");
  cpfile("lib/NVector",targetdir,"*.cxx");

  // KmeansEM
  cpfile("lib/KmeansEM",targetdir,"*.cxx");
  cpfile("lib/KmeansEM",targetdir,"*.h");
  cpfile("lib/KmeansEM",targetdir,"make*");
  cpfile("lib/KmeansEM",targetdir,"setup*");
  cpfile("lib/KmeansEM",targetdir,"*.cxx");

  // NeuralNet
  cpfile("lib/NeuralNet",targetdir);
  cpfile("lib/NeuralNet",targetdir,"*.cxx");
  cpfile("lib/NeuralNet",targetdir,"*.h");
  cpfile("lib/NeuralNet",targetdir,"make*");
  cpfile("lib/NeuralNet",targetdir,"setup*");
  cpfile("lib/NeuralNet",targetdir,"*.cxx");
  //cpfile("lib/NeuralNet/testGraph",targetdir);
  cpfile("lib/NeuralNet/testGraph",targetdir,"*.cxx");
  cpfile("lib/NeuralNet/testGraph",targetdir,"*.h");
  cpfile("lib/NeuralNet/testGraph",targetdir,"*.bat");
  cpfile("lib/NeuralNet/testGraph",targetdir);

#if 1
  cpfile("include/_battery.cxx",targetdir);
  cpfile("include/_find.cxx",targetdir);
  cpfile("include/_csvPrompt.cxx",targetdir);
  cpfile("include/_copyfile.cxx",targetdir);
  cpfile("include/_grepcsv.cxx",targetdir);
#endif

  cpfile("stl/_vector.h",targetdir);
  cpfile("lib\\prec_stl\\vector",targetdir);

#if 1
  // EasyExp
  cpfile("lib/EasyExp",targetdir,"*.c");
  cpfile("lib/EasyExp",targetdir,"*.cxx");
  cpfile("lib/EasyExp",targetdir,"*.h");
  cpfile("lib/EasyExp",targetdir,"make*");
  cpfile("lib/EasyExp",targetdir,"setup*");
  // ReadF
  //cpfile("lib/ReadF",targetdir,"*.c");
  //cpfile("lib/ReadF",targetdir,"*.cxx");
  //cpfile("lib/ReadF",targetdir,"*.h");
  //cpfile("lib/ReadF",targetdir,"make*");
  //cpfile("lib/ReadF",targetdir,"setup*");
#endif

#if 1
  // unistd, posix.dll
  cpfile("lib/stdstrct/stdfunc.h",targetdir);
  cpfile("lib/stdstrct/Makestdfunc.linux",targetdir);
  cpfile("lib/stdstrct/LinkDef.h",targetdir);
#endif

#if 1
  cpfile("lib/win32api/winfunc.h",targetdir);
  cpfile("lib/win32api/cintwin.h",targetdir);
#endif

#if 1
  cpfile("demo/graphs/README.txt",targetdir);
  cpfile("demo/graphs/dft.c",targetdir);
  cpfile("demo/graphs/eular.c",targetdir);
  cpfile("demo/graphs/FFTdemo.c",targetdir);
  cpfile("demo/graphs/fir2.c",targetdir);
  cpfile("demo/graphs/fir.c",targetdir);
  cpfile("demo/graphs/jitter.c",targetdir);
  cpfile("demo/graphs/modulation.c",targetdir);
  cpfile("demo/graphs/sin123.c",targetdir);
  cpfile("demo/graphs/sin.c",targetdir);
  cpfile("demo/graphs/skineffect.c",targetdir);
  cpfile("demo/graphs/stripline.c",targetdir);
#endif
}

///////////////////////////////////////////////////////////////////////////
void cpbin(char *targetdir) {
  // 
#if 1
  cpfile("cint.exe",targetdir);
  cpfile("libcint.dll",targetdir);
  cpfile("libcint.lib",targetdir);
  cpfile("makecint.exe",targetdir);
  cpfile("include/cintsock.dll",targetdir);
  cpfile("include/excel.dll",targetdir);
  cpfile("include/EasyExp.dll",targetdir);
  cpfile("include/long.dll",targetdir);
  cpfile("include/posix.dll",targetdir);
  cpfile("include/pthread.dll",targetdir);
  //cpfile("include/ReadF.dll",targetdir);
  //cpfile("include/RegExp.dll",targetdir);
  cpfile("include/stdfunc.dll",targetdir);
  cpfile("include/stdcxxfunc.dll",targetdir);
  cpfile("include/win32api.dll",targetdir);
  cpfile("stl/complex.dll",targetdir);
  cpfile("stl/deque.dll",targetdir);
  cpfile("stl/exception.dll",targetdir);
  cpfile("stl/list.dll",targetdir);
  cpfile("stl/map.dll",targetdir);
  cpfile("stl/map2.dll",targetdir);
  cpfile("stl/_map",targetdir);
  cpfile("stl/multimap.dll",targetdir);
  cpfile("stl/multimap2.dll",targetdir);
  cpfile("stl/multiset.dll",targetdir);
  cpfile("stl/queue.dll",targetdir);
  cpfile("stl/set.dll",targetdir);
  cpfile("stl/stack.dll",targetdir);
  cpfile("stl/stdexcept.dll",targetdir);
  cpfile("stl/string.dll",targetdir);
  cpfile("stl/valarray.dll",targetdir);
  cpfile("stl/vector.dll",targetdir);
  cpfile("stl/vectorbool.dll",targetdir);
#endif
}

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
char* srctarget[] = {
  "/opt/root/cint/cint",
  "" 
};

///////////////////////////////////////////////////////////////////////////
char* bintarget[] = {
  "" 
};

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
int main() {
  int i=0;
  while(srctarget[i][0]) {
    cpall(srctarget[i]);
    ++i;
  }
  i=0;
  while(bintarget[i][0]) {
    cpbin(bintarget[i]);
    ++i;
  }
  return(0);
}


///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
void cpfile(char* fname,char* targetdir,char* expr="") {
  if(expr[0]==0) {
    mkdir(targetdir,0);
    char buf[500];
    sprintf(buf,"%s ./%s %s/%s",cp,fname,targetdir,fname);
    printf("%s\n",buf);
    system(buf);
  }
  else {
    mkdir(targetdir,0);
    scandir(fname,targetdir,expr);
  }
}

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
void scandir(char* originaldir,char* outdir,char* expr) {
  struct DIR *dir = opendir(originaldir);
  struct dirent *d;
  int s;
  struct stat buf;
  char in[500];
  char out[500];
  char *p;

  EasyExp rege(expr);
  printf("scanning %s ... \n",originaldir);

  while((d=readdir(dir))) {
    s=stat(d->d_name,&buf);
    sprintf(in,"%s/%s",originaldir,d->d_name);
    sprintf(out,"%s/%s",outdir,d->d_name);
    if(rege.match(d->d_name)) {
      cpfile(in,outdir);
    }
  }

  closedir(dir);
}

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
