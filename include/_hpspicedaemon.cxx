
#include <CSV.h>
#include <unistd.h>

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
void runspice(ReadF& f) {
  char com[2000];
  char* cktname = f.argv[2];
  char* txtname = f.argv[3];
  char* rawname = f.argv[4];
  char* tmp="tmp.txt";
  //sprintf(com,"spice %s %s %s",cktname,txtname,rawname);
  sprintf(com,"spice %s %s %s",cktname,tmp,rawname);
  printf("%s\n",com);
  system(com);
  rename(tmp,txtname);
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
    sleep(1);
  }
  fclose(fp);
  ReadF f(flagfile.c_str());
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
      }
      else if(strcmp(f.argv[1],"spice")==0) {
	runspice(f);
      }
      else {
      }
    }
  }
  f.close();
  Remove(flagfile.c_str());
  return(0);
}
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
void daemonloop() { // NOT USED NOW
  //G__optimizemode(3);
  looping=1;
  while(looping) {
    checkfile();
    sleep(1);
  }
}

////////////////////////////////////////////////////////////////////////////
void loop() { daemonloop();}
void l() { daemonloop();}

////////////////////////////////////////////////////////////////////////////
int main() {
  daemonloop();
}
////////////////////////////////////////////////////////////////////////////
