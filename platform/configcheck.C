#! ../cint -qlevel1
//
// configcheck.c
// C++ script to check platform dependency file format
//
#include <iostream.h>
#include <readfile.h>

/////////////////////////////////////////////////////////////////////////
// ConfigItem class
/////////////////////////////////////////////////////////////////////////
class ConfigItem {
 public:

  ConfigItem(const char *namein) { 
    item=new char[strlen(namein)+1];
    strcpy(item,namein);
  }

  ~ConfigItem() { delete item; }

  const char* GetItem() { return(item); }

 private:
  char *item;
};


/////////////////////////////////////////////////////////////////////////
// platform Config class
/////////////////////////////////////////////////////////////////////////
class Config {
 public:

  Config(const char *filename) {
    n=0;
    char *p;
    ReadFile f(filename);
    while(f.read()) {
      if(f.argc && '#'!=f.argv[1][0]) {
#ifdef DETAIL
	if(f.argc>2) {
	  p = strstr(f.argv[0],f.argv[3]);
	  value[n] = new ConfigItem(p);
	}
	else         value[n] = new ConfigItem("");
#endif
	items[n++] = new ConfigItem(f.argv[1]);
      }
    }
  }

  ~Config() { 
    while(n--) {
#ifdef DETAIL
      delete value[n];
#endif
      delete items[n]; 
    }
  }

  int Compare(Config& x) {
    int error=0;
    int minn;
    if(n>x.n) minn = x.n;
    else      minn = n;
    for(int i=0;i<minn;i++) {
      if(strcmp(items[i]->GetItem(),x.items[i]->GetItem())!=0) {
	cout<<"Mismatch "<<i<<" "<<items[i]->GetItem()<<" and "
	                 <<x.items[i]->GetItem()<<"\n";
	++error;
      }
#ifdef DETAIL
      if(strcmp(value[i]->GetItem(),x.value[i]->GetItem())!=0) {
	cout<<items[i]->GetItem()<<" '"
	  <<value[i]->GetItem()<<"' and '"
	  <<x.value[i]->GetItem()<<"'\n";
	++error;
      }
#endif
    }
    if(n>x.n) {
      for(;i<n;i++) cout<<items[i]->GetItem()<<" Missigng\n";
      ++error;
    }
    else if(n<x.n) {
      for(;i<x.n;i++) cout<<x.items[i]->GetItem()<<" Unexpected\n";
      ++error;
    }
    return(error);
  }

 private:
  ConfigItem *items[1000];
#ifdef DETAIL
  ConfigItem *value[1000];
#endif
  int n;
};

/////////////////////////////////////////////////////////////////////////
// file fileter
/////////////////////////////////////////////////////////////////////////
int invalidfile(const char *filename)
{
  if(strncmp(filename,"README",6)==0||
     strcmp(filename,"G__memhist")==0||
     strcmp(filename,"aixdlfcn")==0||
     strcmp(filename,"borland")==0||
     strcmp(filename,"visual")==0||
     strcmp(filename,"sun")==0||
     strcmp(filename,"symantec")==0||
     strcmp(filename,"djgpp")==0||
     strcmp(filename,"configcheck.C")==0||
     filename[0]=='#') {
    return(1);
  }
  else {
    return(0);
  }
}

/////////////////////////////////////////////////////////////////////////
// main
/////////////////////////////////////////////////////////////////////////
main(int argc,char **argv) {
  Config  hpux8("hpux8");
  Config  *target;
  for(int i=1;i<argc;i++) {
    if(invalidfile(argv[i])) continue;
    target = new Config(argv[i]);
    if(hpux8.Compare(*target)) {
      cout<<"######## hpux8 and "<<argv[i]<<" has some differences############\n";
#ifdef DETAIL
      cout.flush();
      G__pause();
#endif
    }
    else
      cout<<argv[i]<<" OK ------------------------------------------\n";
    delete target;
  }
}


/*
 * Local Variables:
 * c-tab-always-indent:nil
 * c-indent-level:2
 * c-continued-statement-offset:2
 * c-brace-offset:-2
 * c-brace-imaginary-offset:0
 * c-argdecl-indent:0
 * c-label-offset:-2
 * compile-command:"make -k"
 * End:
 */
