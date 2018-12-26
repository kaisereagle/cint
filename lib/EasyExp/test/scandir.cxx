//////////////////////////////////////////////////////////////////////
// Scandir test
//
//  2015/Aug/16  Masaahru Goto
//////////////////////////////////////////////////////////////////////
#pragma include <EasyExp.dll>
#include <unistd.h>
#include "_util_cinttest.h"

int pass;
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
int test1a() {
  RESET_LOCALFAIL();
  unsigned int i;
  Scandir d("*.cxx");
  vector<string> filelist;
  d.get(filelist);

  EXPECT_EQ(filelist.size(),7) ;
  EXPECT_EQ(filelist[0],"compile.cxx") ;
  EXPECT_EQ(filelist[1],"match.cxx") ;
  EXPECT_EQ(filelist[2],"multithread.cxx") ;
  EXPECT_EQ(filelist[3],"replace.cxx") ;
  EXPECT_EQ(filelist[4],"scandir.cxx") ;
  EXPECT_EQ(filelist[5],"synthesis.cxx") ;
  EXPECT_EQ(filelist[6],"testall.cxx") ;
  char *fname; i=0;
  while((fname=d.Next())&&fname[0]) {
    EXPECT_EQ(filelist[i++],fname) ;
  }
  if(LOCALFAIL_COUNT()) {
    printf("test1a fail %d\n",LOCALFAIL_COUNT());
    d.disp();
  }
  return(LOCALFAIL_COUNT());
}

////////////////////////////////////////////////////////////////////////
int test1b() {
  RESET_LOCALFAIL();
  unsigned int i;
  Scandir d;
  d.scan("*.cxx");
  vector<string> filelist;
  d.get(filelist);

  EXPECT_EQ(filelist.size(),7) ;
  EXPECT_EQ(filelist[0],"compile.cxx") ;
  EXPECT_EQ(filelist[1],"match.cxx") ;
  EXPECT_EQ(filelist[2],"multithread.cxx") ;
  EXPECT_EQ(filelist[3],"replace.cxx") ;
  EXPECT_EQ(filelist[4],"scandir.cxx") ;
  EXPECT_EQ(filelist[5],"synthesis.cxx") ;
  EXPECT_EQ(filelist[6],"testall.cxx") ;
  char *fname; i=0;
  while((fname=d.Next())&&fname[0]) {
    EXPECT_EQ(filelist[i++],fname) ;
  }
  if(LOCALFAIL_COUNT()) {
    printf("test1b fail %d\n",LOCALFAIL_COUNT());
    d.disp();
  }
  return(LOCALFAIL_COUNT());
}

////////////////////////////////////////////////////////////////////////
int test1c() {
  RESET_LOCALFAIL();
  unsigned int i;
  string path2, path = getenv("CINTSYSDIR");
  EasyExp e;
  e.compile_replace("*\\\\*","$1=/");
  e.replace(path.c_str(),path2);
  path2 += "/lib/EasyExp/test/*.cxx";
  Scandir d(path2.c_str());
  vector<string> filelist;
  d.getfname(filelist);

  EXPECT_EQ(filelist.size(),7) ;
  if(filelist.size(),7) return(8);
  EXPECT_EQ(filelist[0],"compile.cxx") ;
  EXPECT_EQ(filelist[1],"match.cxx") ;
  EXPECT_EQ(filelist[2],"multithread.cxx") ;
  EXPECT_EQ(filelist[3],"replace.cxx") ;
  EXPECT_EQ(filelist[4],"scandir.cxx") ;
  EXPECT_EQ(filelist[5],"synthesis.cxx") ;
  EXPECT_EQ(filelist[6],"testall.cxx") ;
  if(LOCALFAIL_COUNT()) {
    printf("test1c fail %d\n",LOCALFAIL_COUNT());
    d.disp();
  }
  return(LOCALFAIL_COUNT());
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
int test2a() {
  RESET_LOCALFAIL();
  unsigned int i;
  string path2, path = getenv("CINTSYSDIR");
  EasyExp e;
  e.compile_replace("*\\\\*","$1=/");
  e.replace(path.c_str(),path2);
  path2 += "/tool/*[^o]";
  vector<string> filelist;
  Scandir d;

  d.scan(path2.c_str(),2); // both
  d.getfname(filelist);
  EXPECT_EQ(filelist.size(),5) ;
  EXPECT_EQ(filelist[0],"batch.c") ;
  EXPECT_EQ(filelist[1],"crlf") ;
  EXPECT_EQ(filelist[2],"ifdef") ;
  EXPECT_EQ(filelist[3],"makecint.c") ;
  EXPECT_EQ(filelist[4],"makerootcint.cxx") ;
  //for(i=0;i<filelist.size();i++) printf("%d %s\n",i,filelist[i].c_str());
  if(LOCALFAIL_COUNT()) {
    printf("test2a-1 fail %d\n",LOCALFAIL_COUNT());
    d.disp();
  }

  d.scan(path2.c_str(),0); // file only
  filelist.clear();
  d.getfname(filelist);
  EXPECT_EQ(filelist.size(),3) ;
  EXPECT_EQ(filelist[0],"batch.c") ;
  EXPECT_EQ(filelist[1],"makecint.c") ;
  EXPECT_EQ(filelist[2],"makerootcint.cxx") ;
  //for(i=0;i<filelist.size();i++) printf("%d %s\n",i,filelist[i].c_str());
  if(LOCALFAIL_COUNT()) {
    printf("test2a-2 fail %d\n",LOCALFAIL_COUNT());
    d.disp();
  }

  d.scan(path2.c_str(),1); // dir only
  filelist.clear();
  d.getfname(filelist);
  EXPECT_EQ(filelist.size(),2) ;
  EXPECT_EQ(filelist[0],"crlf") ;
  EXPECT_EQ(filelist[1],"ifdef") ;
  //for(i=0;i<filelist.size();i++) printf("%d %s\n",i,filelist[i].c_str());
  if(LOCALFAIL_COUNT()) {
    printf("test2a-3 fail %d\n",LOCALFAIL_COUNT());
    d.disp();
  }

  return(LOCALFAIL_COUNT());
}

////////////////////////////////////////////////////////////////////////
int test2b() {
  RESET_LOCALFAIL();
  unsigned int i;
  string path2 = "../../../tool/*[^o]";
  vector<string> filelist;
  Scandir d;

  d.scan(path2.c_str(),2); // both
  d.getfname(filelist);
  EXPECT_EQ(filelist.size(),5) ;
  EXPECT_EQ(filelist[0],"batch.c") ;
  EXPECT_EQ(filelist[1],"crlf") ;
  EXPECT_EQ(filelist[2],"ifdef") ;
  EXPECT_EQ(filelist[3],"makecint.c") ;
  EXPECT_EQ(filelist[4],"makerootcint.cxx") ;
  //for(i=0;i<filelist.size();i++) printf("%d %s\n",i,filelist[i].c_str());
  if(LOCALFAIL_COUNT()) {
    printf("test2b-1 fail %d\n",LOCALFAIL_COUNT());
    d.disp();
  }

  d.scan(path2.c_str(),0); // file only
  filelist.clear();
  d.getfname(filelist);
  EXPECT_EQ(filelist.size(),3) ;
  EXPECT_EQ(filelist[0],"batch.c") ;
  EXPECT_EQ(filelist[1],"makecint.c") ;
  EXPECT_EQ(filelist[2],"makerootcint.cxx") ;
  //for(i=0;i<filelist.size();i++) printf("%d %s\n",i,filelist[i].c_str());
  if(LOCALFAIL_COUNT()) {
    printf("test2b-2 fail %d\n",LOCALFAIL_COUNT());
    d.disp();
  }

  d.scan(path2.c_str(),1); // dir only
  filelist.clear();
  d.getfname(filelist);
  EXPECT_EQ(filelist.size(),2) ;
  EXPECT_EQ(filelist[0],"crlf") ;
  EXPECT_EQ(filelist[1],"ifdef") ;
  //for(i=0;i<filelist.size();i++) printf("%d %s\n",i,filelist[i].c_str());
  if(LOCALFAIL_COUNT()) {
    printf("test2b-3 fail %d\n",LOCALFAIL_COUNT());
    d.disp();
  }

  return(LOCALFAIL_COUNT());
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
int test3a() {
  RESET_LOCALFAIL();
  unsigned int i;
  //string path2 = "../../../**/libcint[.mak:1-4]";
  //string path2 = "../../../**/cint*";
  //string path2 = "../../../tool/**/*";
  string path2 = "../../../**/libcint*";
  vector<string> filelist;
  Scandir d;

  d.scan(path2.c_str(),2); // both
  d.getfullpath(filelist);
  EXPECT_EQ(filelist.size(),27) ;
  EXPECT_FALSE(filelist[0].find("../../libcint.dll")==string::npos) ;
  EXPECT_FALSE(filelist[1].find("../../libcint.lib")==string::npos) ;
  EXPECT_FALSE(filelist[2].find("/platform/borland/libcint.bpr")==string::npos) ;
  EXPECT_FALSE(filelist[3].find("/platform/borland/libcint.cpp")==string::npos) ;
  EXPECT_FALSE(filelist[4].find("/platform/cygwin51/libcint.def")==string::npos) ;
  EXPECT_FALSE(filelist[5].find("/platform/symantec/src/libcint.def")==string::npos) ;
  EXPECT_FALSE(filelist[6].find("/platform/symantec/src/libcint.lnk")==string::npos) ;
  EXPECT_FALSE(filelist[7].find("/platform/symantec/src/libcint.mak")==string::npos) ;
  if(LOCALFAIL_COUNT()) printf("aaa %d\n",LOCALFAIL_COUNT());
  EXPECT_FALSE(filelist[8].find("/platform/symantec/src/libcint.opn")==string::npos) ;
  EXPECT_FALSE(filelist[9].find("/platform/symantec/src/libcint.prj")==string::npos) ;
  EXPECT_FALSE(filelist[10].find("/platform/visual/src/libcint.def")==string::npos) ;
  EXPECT_FALSE(filelist[11].find("/platform/visual/src/libcint.mak")==string::npos) ;
  EXPECT_FALSE(filelist[12].find("/platform/visual/src/libcint.mdp")==string::npos) ;
  EXPECT_FALSE(filelist[13].find("/platform/visual/src/libcintold.mak")==string::npos) ;
  if(LOCALFAIL_COUNT()) printf("bbb %d\n",LOCALFAIL_COUNT());
  EXPECT_FALSE(filelist[14].find("/platform/visualCpp13/src/libcintvc13.def")==string::npos) ;
  EXPECT_FALSE(filelist[15].find("/platform/visualCpp13/src/libcintvc13.mak")==string::npos) ;
  EXPECT_FALSE(filelist[16].find("/platform/visualCpp6/src/libcint.def")==string::npos) ;
  EXPECT_FALSE(filelist[17].find("/platform/visualCpp6/src/libcint.dsp")==string::npos) ;
  EXPECT_FALSE(filelist[18].find("/platform/visualCpp6/src/libcint.dsw")==string::npos) ;
  if(LOCALFAIL_COUNT()) printf("ccc %d\n",LOCALFAIL_COUNT());
  EXPECT_FALSE(filelist[19].find("/platform/visualCpp6/src/libcint.mak")==string::npos) ;
  EXPECT_FALSE(filelist[20].find("/platform/visualCpp7/src/libcint.def")==string::npos) ;
  EXPECT_FALSE(filelist[21].find("/platform/visualCpp7/src/libcint.dsp")==string::npos) ;
  EXPECT_FALSE(filelist[22].find("/platform/visualCpp7/src/libcint.dsw")==string::npos) ;
  EXPECT_FALSE(filelist[23].find("/platform/visualCpp7/src/libcint.mak")==string::npos) ;
  EXPECT_FALSE(filelist[24].find("/src/libcint")==string::npos) ;
  EXPECT_FALSE(filelist[25].find("/src/libcint/libcintvc13.def")==string::npos) ;
  EXPECT_FALSE(filelist[26].find("/src/libcint/libcintvc13.mak")==string::npos) ;
  //for(i=0;i<filelist.size();i++) printf("%d %s\n",i,filelist[i].c_str());
  if(LOCALFAIL_COUNT()) {
    printf("test3a-1 fail %d\n",LOCALFAIL_COUNT());
    d.disp();
  }

  d.scan(path2.c_str(),0); // file only
  filelist.clear();
  d.getfullpath(filelist);
  EXPECT_EQ(filelist.size(),26) ;
  EXPECT_FALSE(filelist[0].find("../../libcint.dll")==string::npos) ;
  EXPECT_FALSE(filelist[1].find("../../libcint.lib")==string::npos) ;
  EXPECT_FALSE(filelist[2].find("/platform/borland/libcint.bpr")==string::npos) ;
  EXPECT_FALSE(filelist[3].find("/platform/borland/libcint.cpp")==string::npos) ;
  EXPECT_FALSE(filelist[4].find("/platform/cygwin51/libcint.def")==string::npos) ;
  EXPECT_FALSE(filelist[5].find("/platform/symantec/src/libcint.def")==string::npos) ;
  EXPECT_FALSE(filelist[6].find("/platform/symantec/src/libcint.lnk")==string::npos) ;
  EXPECT_FALSE(filelist[7].find("/platform/symantec/src/libcint.mak")==string::npos) ;
  if(LOCALFAIL_COUNT()) printf("aaa %d\n",LOCALFAIL_COUNT());
  EXPECT_FALSE(filelist[8].find("/platform/symantec/src/libcint.opn")==string::npos) ;
  EXPECT_FALSE(filelist[9].find("/platform/symantec/src/libcint.prj")==string::npos) ;
  EXPECT_FALSE(filelist[10].find("/platform/visual/src/libcint.def")==string::npos) ;
  EXPECT_FALSE(filelist[11].find("/platform/visual/src/libcint.mak")==string::npos) ;
  EXPECT_FALSE(filelist[12].find("/platform/visual/src/libcint.mdp")==string::npos) ;
  EXPECT_FALSE(filelist[13].find("/platform/visual/src/libcintold.mak")==string::npos) ;
  if(LOCALFAIL_COUNT()) printf("bbb %d\n",LOCALFAIL_COUNT());
  EXPECT_FALSE(filelist[14].find("/platform/visualCpp13/src/libcintvc13.def")==string::npos) ;
  EXPECT_FALSE(filelist[15].find("/platform/visualCpp13/src/libcintvc13.mak")==string::npos) ;
  EXPECT_FALSE(filelist[16].find("/platform/visualCpp6/src/libcint.def")==string::npos) ;
  EXPECT_FALSE(filelist[17].find("/platform/visualCpp6/src/libcint.dsp")==string::npos) ;
  EXPECT_FALSE(filelist[18].find("/platform/visualCpp6/src/libcint.dsw")==string::npos) ;
  if(LOCALFAIL_COUNT()) printf("ccc %d\n",LOCALFAIL_COUNT());
  EXPECT_FALSE(filelist[19].find("/platform/visualCpp6/src/libcint.mak")==string::npos) ;
  EXPECT_FALSE(filelist[20].find("/platform/visualCpp7/src/libcint.def")==string::npos) ;
  EXPECT_FALSE(filelist[21].find("/platform/visualCpp7/src/libcint.dsp")==string::npos) ;
  EXPECT_FALSE(filelist[22].find("/platform/visualCpp7/src/libcint.dsw")==string::npos) ;
  EXPECT_FALSE(filelist[23].find("/platform/visualCpp7/src/libcint.mak")==string::npos) ;
  EXPECT_FALSE(filelist[24].find("/src/libcint/libcintvc13.def")==string::npos) ;
  EXPECT_FALSE(filelist[25].find("/src/libcint/libcintvc13.mak")==string::npos) ;
  if(LOCALFAIL_COUNT()) {
    printf("test3a-2 fail %d\n",LOCALFAIL_COUNT());
    d.disp();
  }

  d.scan(path2.c_str(),1); // dir only
  filelist.clear();
  d.getfullpath(filelist);
  EXPECT_EQ(filelist.size(),1) ;
  EXPECT_FALSE(filelist[0].find("/src/libcint")==string::npos) ;
  if(LOCALFAIL_COUNT()) {
    printf("test3a-3 fail %d\n",LOCALFAIL_COUNT());
    d.disp();
  }

  return(LOCALFAIL_COUNT());
}

////////////////////////////////////////////////////////////////////////
int test3b() {
  RESET_LOCALFAIL();
  unsigned int i;
  string path2 = "../../../tool/**/*";
  vector<string> filelist;
  Scandir d;

  d.scan(path2.c_str(),2); // both
  d.getfullpath(filelist);
  EXPECT_EQ(filelist.size(),15) ;
  EXPECT_FALSE(filelist[0].find("/tool/batch.c")==string::npos) ;
  EXPECT_FALSE(filelist[1].find("/tool/crlf")==string::npos) ;
  EXPECT_FALSE(filelist[2].find("/tool/crlf/crlf.cxx")==string::npos) ;
  EXPECT_FALSE(filelist[3].find("/tool/ifdef")==string::npos) ;
  EXPECT_FALSE(filelist[4].find("/tool/ifdef/Makefile.base")==string::npos) ;
  EXPECT_FALSE(filelist[5].find("/tool/ifdef/Makefile.bcc5")==string::npos) ;
  EXPECT_FALSE(filelist[6].find("/tool/ifdef/get.c")==string::npos) ;
  EXPECT_FALSE(filelist[7].find("/tool/ifdef/ifdef.cxx")==string::npos) ;
  EXPECT_FALSE(filelist[8].find("/tool/ifdef/ifdef.mak")==string::npos) ;
  EXPECT_FALSE(filelist[9].find("/tool/ifdef/ifdefvc13.mak")==string::npos) ;
  EXPECT_FALSE(filelist[10].find("/tool/ifdef/setup")==string::npos) ;
  EXPECT_FALSE(filelist[11].find("/tool/ifdef/setup.bat")==string::npos) ;
  EXPECT_FALSE(filelist[12].find("/tool/ifdef/winnt.c")==string::npos) ;
  EXPECT_FALSE(filelist[13].find("/tool/makecint.c")==string::npos) ;
  EXPECT_FALSE(filelist[14].find("/tool/makerootcint.cxx")==string::npos) ;
  //for(i=0;i<filelist.size();i++) printf("%d %s\n",i,filelist[i].c_str());
  if(LOCALFAIL_COUNT()) {
    printf("test3b-1 fail %d\n",LOCALFAIL_COUNT());
    d.disp();
  }

  d.scan(path2.c_str(),0); // file only
  filelist.clear();
  d.getfullpath(filelist);
  EXPECT_EQ(filelist.size(),13) ;
  EXPECT_FALSE(filelist[0].find("/tool/batch.c")==string::npos) ;
  EXPECT_FALSE(filelist[1].find("/tool/crlf/crlf.cxx")==string::npos) ;
  EXPECT_FALSE(filelist[2].find("/tool/ifdef/Makefile.base")==string::npos) ;
  EXPECT_FALSE(filelist[3].find("/tool/ifdef/Makefile.bcc5")==string::npos) ;
  EXPECT_FALSE(filelist[4].find("/tool/ifdef/get.c")==string::npos) ;
  EXPECT_FALSE(filelist[5].find("/tool/ifdef/ifdef.cxx")==string::npos) ;
  EXPECT_FALSE(filelist[6].find("/tool/ifdef/ifdef.mak")==string::npos) ;
  EXPECT_FALSE(filelist[7].find("/tool/ifdef/ifdefvc13.mak")==string::npos) ;
  EXPECT_FALSE(filelist[8].find("/tool/ifdef/setup")==string::npos) ;
  EXPECT_FALSE(filelist[9].find("/tool/ifdef/setup.bat")==string::npos) ;
  EXPECT_FALSE(filelist[10].find("/tool/ifdef/winnt.c")==string::npos) ;
  EXPECT_FALSE(filelist[11].find("/tool/makecint.c")==string::npos) ;
  EXPECT_FALSE(filelist[12].find("/tool/makerootcint.cxx")==string::npos) ;
  if(LOCALFAIL_COUNT()) {
    G__pause();
    printf("test3b-2 fail %d\n",LOCALFAIL_COUNT());
    d.disp();
  }

  d.scan(path2.c_str(),1); // dir only
  filelist.clear();
  d.getfullpath(filelist);
  EXPECT_EQ(filelist.size(),2) ;
  EXPECT_FALSE(filelist[0].find("/tool/crlf")==string::npos) ;
  EXPECT_FALSE(filelist[1].find("/tool/ifdef")==string::npos) ;
  if(LOCALFAIL_COUNT()) {
    printf("test3b-3 fail %d\n",LOCALFAIL_COUNT());
    d.disp();
  }

  return(LOCALFAIL_COUNT());
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
int DEBUG=0;
int quick=0,intentionalfail=0;
int main(int argc,char** argv) {
  for(int i=1;i<argc;i++) {
    if(strcmp(argv[i],"-g")==0) ++DEBUG;
    else if(strcmp(argv[i],"-q")==0) ++quick;
    else if(strcmp(argv[i],"-f")==0) ++intentionalfail;
    else if(argv[i][0]=='-') {
      printf("compile.cxx : Unknown option %s\n",argv[i]);
      return(0);
    }
  }
  int err=0;
  printf("### scandir test\n");
 
  printf("test1a-c\n");
  err += test1a();
  err += test1b();
  err += test1c();

  printf("test2a-b\n");
  err += test2a();
  err += test2b();

  printf("test3a-b\n");
  err += test3a();
  err += test3b();

  SAVE_PASSFAIL("scandir");
  return(FAIL_COUNT());
}
////////////////////////////////////////////////////////////////////////
