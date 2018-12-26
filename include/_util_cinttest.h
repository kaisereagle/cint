/* /% C %/ */
/***********************************************************************
 * test utility using cint (C/C++ interpreter)
 ************************************************************************
 * header file _util_cinttest.h
 ************************************************************************
 * Description:
 *  Google test emulator and original test utility using CINT
 ************************************************************************
 * Copyright(c) 2015~2015  Masaharu Goto (cint@pcroot.cern.ch) (gotom@hanno.jp)
 *
 * Permission to use, copy, modify and distribute this software and its 
 * documentation for non-commercial purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  The author makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 ************************************************************************/
#pragma ifndef UTIL_CINTTEST_H
#pragma define UTIL_CINTTEST_H

#include <stdio.h>
#include <float.h>
#pragma include <string>
// Do not include other library here.

/////////////////////////////////////////////////////////
static int g_pass=0,g_fail=0, g_localfail=0;
/////////////////////////////////////////////////////////
const char* fresult = "passfail.csv";

/////////////////////////////////////////////////////////
void REMOVE_TOTAL_PASSFAIL() {
  remove(fresult);
  FILE *fp = fopen(fresult,"w");
  if(!fp) return;
  fprintf(fp,"test,pass,fail\n");
  fclose(fp);
}
/////////////////////////////////////////////////////////
void SAVE_PASSFAIL(const string& name) {
  if(strcmp(G__lasterror_filename(),"")!=0) {
    ++g_fail;++g_localfail;
    printf("Test code error at %s:%d\n",G__lasterror_filename(),G__lasterror_linenum);
  }
  G__clearerror();
  if(g_fail) 
    printf("####### [ SUBTEST %s ] pass=%d fail=%d ########\n",name.c_str(),g_pass,g_fail);
  else
    printf("[ SUBTEST %s ] pass=%d fail=%d\n",name.c_str(),g_pass,g_fail);
  FILE* fp=fopen(fresult,"a");
  if(!fp) {
    return;
  }
  fprintf(fp,"%s,%d,%d\n",name.c_str(),g_pass,g_fail);
  fclose(fp);
}

/////////////////////////////////////////////////////////
int READ_TOTAL_PASSFAIL() {
  // Loading extra library does not affect test result, 
  // since, this function is called at the very end.
  G__loadfile("CSV.h"); 
  Sheet s(fresult);
  Collection c=s.collection();
  c.setkey(0);
  Collection tfail=c.range("fail",1,1000000);
  Collection t0pass=c.range("pass",0,0);
  CollectionStatistics cs = c.stat();
  int g_fail=cs.get(2).sigma();
  printf("### Summary ###\n");
  //t.disp();
  if(tfail.size()) tfail.disp();
  if(t0pass.size()) t0pass.disp();
  if(g_fail) {
    printf("####### [   TOTAL   ] pass=%g fail=%g ########\n",cs.get(1).sigma(),g_fail);
  }
  else {
    printf("[   TOTAL   ] pass=%g fail=%g\n",cs.get(1).sigma(),g_fail);
  }
  return(g_fail);
}
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
void CINTTEST_ATERROR() {++g_fail; ++g_localfail;}
int PASS_COUNT() { return(g_pass); }
int FAIL_COUNT() { return(g_fail); }
int LOCALFAIL_COUNT() { return(g_localfail); }
void RESET_PASSFAIL() { g_pass=g_fail=0; G__set_aterror(CINTTEST_ATERROR); }
void RESET_LOCALFAIL() { g_localfail=0; G__set_aterror(CINTTEST_ATERROR); }

#define  EXPECT_TRUE(a) EXPECT_TRUE_(a,__FILE__,__LINE__)
#define  EXPECT_FALSE(a) EXPECT_FALSE_(a,__FILE__,__LINE__)
#define  EXPECT_EQ(a,b) EXPECT_EQ_(a,b,__FILE__,__LINE__)
#define  EXPECT_NE(a,b) EXPECT_NE_(a,b,__FILE__,__LINE__)
#define  EXPECT_LT(a,b) EXPECT_LT_(a,b,__FILE__,__LINE__)
#define  EXPECT_LE(a,b) EXPECT_LE_(a,b,__FILE__,__LINE__)
#define  EXPECT_GT(a,b) EXPECT_GT_(a,b,__FILE__,__LINE__)
#define  EXPECT_GE(a,b) EXPECT_GE_(a,b,__FILE__,__LINE__)

#define  EXPECT_STREQ(a,b) EXPECT_STREQ_(a,b,__FILE__,__LINE__)
#define  EXPECT_STRNE(a,b) EXPECT_STRNE_(a,b,__FILE__,__LINE__)
#define  EXPECT_STRCASEEQ(a,b) EXPECT_STRCASEEQ_(a,b,__FILE__,__LINE__)
#define  EXPECT_STRCASENE(a,b) EXPECT_STRCASENE_(a,b,__FILE__,__LINE__)

#define  EXPECT_FLOAT_EQ(a,b) EXPECT_FLOAT_EQ_(a,b,__FILE__,__LINE__)
#define  EXPECT_DOUBLE_EQ(a,b) EXPECT_DOUBLE_EQ_(a,b,__FILE__,__LINE__)

#define  EXPECT_NEAR(a,b,er) EXPECT_NEAR_(a,b,er,__FILE__,__LINE__)

/////////////////////////////////////////////////////////
inline int EXPECT_TRUE_(int condition,const char* fname,int line) {
  if(condition) {++g_pass; return(0);}
  else {++g_fail;++g_localfail; printf("EXPECT_TRUE failed at %s:%d\n",fname,line); return(1);}
}
/////////////////////////////////////////////////////////
inline int EXPECT_FALSE_(int condition,const char* fname,int line) {
  if(!condition) {++g_pass; return(0);}
  else {++g_fail;++g_localfail; printf("EXPECT_FALSE failed at %s:%d\n",fname,line);return(1);}
}
/////////////////////////////////////////////////////////
template<class T,class E>
inline int EXPECT_EQ_(const T& a,const E& b,const char* fname,int line) {
  if(a==b) {++g_pass;return(0);}
  else {
    ++g_fail;
    ++g_localfail; 
    cout << "EXPECT_EQ failed at " << fname << ":" << line << " ";
    cout << "'" << a << "' == '" << b << "'" << endl;
    return(1);
  }
}
/////////////////////////////////////////////////////////
template<class T,class E>
inline int EXPECT_NE_(const T& a,const E& b,const char* fname,int line) {
  if(a!=b) {++g_pass;return(0);}
  else {
    ++g_fail;
    ++g_localfail; 
    cout << "EXPECT_NE failed at " << fname << ":" << line << " ";
    cout << "'" << a << "' != '" << b << "'" << endl;
    return(1);
  }
}
/////////////////////////////////////////////////////////
template<class T,class E>
inline int EXPECT_LT_(const T& a,const E& b,const char* fname,int line) {
  if(a<b) {++g_pass;return(0);}
  else {
    ++g_fail;
    ++g_localfail; 
    cout << "EXPECT_LT failed at " << fname << ":" << line << " ";
    cout << "'" << a << "' < '" << b << "'" << endl;
    return(1);
  }
}
/////////////////////////////////////////////////////////
template<class T,class E>
inline int EXPECT_LE_(const T& a,const E& b,const char* fname,int line) {
  if(a<=b) {++g_pass;return(0);}
  else {
    ++g_fail;
    ++g_localfail; 
    cout << "EXPECT_LE failed at " << fname << ":" << line << " ";
    cout << "'" << a << "' <= '" << b << "'" << endl;
    return(1);
  }
}
/////////////////////////////////////////////////////////
template<class T,class E>
inline int EXPECT_GT_(const T& a,const E& b,const char* fname,int line) {
  if(a>b) {++g_pass;return(0);}
  else {
    ++g_fail;
    ++g_localfail; 
    cout << "EXPECT_GT failed at " << fname << ":" << line << " ";
    cout << "'" << a << "' > '" << b << "'" << endl;
    return(1);
  }
}
/////////////////////////////////////////////////////////
template<class T,class E>
inline int EXPECT_GE_(const T& a,const E& b,const char* fname,int line) {
  if(a>=b) {++g_pass;return(0);}
  else {
    ++g_fail;
    ++g_localfail; 
    cout << "EXPECT_GE failed at " << fname << ":" << line << " ";
    cout << "'" << a << "' >= '" << b << "'" << endl;
    return(1);
  }
}
/////////////////////////////////////////////////////////
inline int EXPECT_STREQ_(const char* a,const char* b,const char* fname,int line) {
  if(strcmp(a,b)==0) {++g_pass;return(0);}
  else {
    ++g_fail;
    ++g_localfail; 
    cout << "EXPECT_STREQ failed at " << fname << ":" << line << " ";
    cout << "'" << a << "' == '" << b << "'" << endl;
    return(1);
  }
}
/////////////////////////////////////////////////////////
inline int EXPECT_STRNE_(const char* a,const char* b,const char* fname,int line) {
  if(strcmp(a,b)) {++g_pass;return(0);}
  else {
    ++g_fail;
    ++g_localfail; 
    cout << "EXPECT_STRNE failed at " << fname << ":" << line << " ";
    cout << "'" << a << "' != '" << b << "'" << endl;
    return(1);
  }
}
/////////////////////////////////////////////////////////
inline int EXPECT_STRCASEEQ_(const char* a,const char* b,const char* fname,int line) {
#ifdef G__WIN32
  if(stricmp(a,b)==0) {++g_pass;return(0);}
  else {
    ++g_fail;
    ++g_localfail; 
    cout << "EXPECT_STRCASEEQ failed at " << fname << ":" << line << " ";
    cout << "'" << a << "' == '" << b << "'" << endl;
    return(1);
  }
#else
  if(strcasecmp(a,b)==0) {++g_pass;return(0);}
  else {
    ++g_fail;
    ++g_localfail; 
    cout << "EXPECT_STRCASEEQ failed at " << fname << ":" << line << " ";
    cout << "'" << a << "' == '" << b << "'" << endl;
    return(1);
  }
#endif
}
/////////////////////////////////////////////////////////
inline int EXPECT_STRCASENE_(const char* a,const char* b,const char* fname,int line) {
#ifdef G__WIN32
  if(stricmp(a,b)) {++g_pass;return(0);}
  else {
    ++g_fail;
    ++g_localfail; 
    cout << "EXPECT_STRCASENE failed at " << fname << ":" << line << " ";
    cout << "'" << a << "' == '" << b << "'" << endl;
    return(1);
  }
#else
  if(strcasecmp(a,b)) {++g_pass;return(0);}
  else {
    ++g_fail;
    ++g_localfail; 
    cout << "EXPECT_STRCASENE failed at " << fname << ":" << line << " ";
    cout << "'" << a << "' == '" << b << "'" << endl;
    return(1);
  }
#endif
}
/////////////////////////////////////////////////////////
template<class T,class E>
inline int EXPECT_FLOAT_EQ_(const T& a,const E& b,const char* fname,int line) {
  if(fabs(a-b)<=FLT_MIN*4) {++g_pass;return(0);}
  else {
    ++g_fail;
    ++g_localfail; 
    cout << "EXPECT_FLOAT_EQ failed at " << fname << ":" << line << " ";
    cout << "'" << a << "' == '" << b << "'" << endl;
    return(1);
  }
}
/////////////////////////////////////////////////////////
template<class T,class E>
inline int EXPECT_DOUBLE_EQ_(const T& a,const E& b,const char* fname,int line) {
  if(fabs(a-b)<=DBL_MIN*4) {++g_pass;return(0);}
  else {
    ++g_fail;
    ++g_localfail; 
    cout << "EXPECT_DOUBLE_EQ failed at " << fname << ":" << line << " ";
    cout << "'" << a << "' == '" << b << "'" << endl;
    return(1);
  }
}

/////////////////////////////////////////////////////////
template<class T,class E,class K>
inline int EXPECT_NEAR_(const T& a,const E& b,const K& err,const char* fname,int line) {
  if(fabs(a-b)<=err) {++g_pass;return(0);}
  else {
    ++g_fail;
    ++g_localfail; 
    cout << "EXPECT_NEAR failed at " << fname << ":" << line << " ";
    cout << "'" << a << "' == '" << b << "'" << endl;
    return(1);
  }
}

/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
#if 1
#define  ASSERT_TRUE(a) if(EXPECT_TRUE_(a,__FILE__,__LINE__)) return;
#define  ASSERT_FALSE(a) if(EXPECT_FALSE_(a,__FILE__,__LINE__)) return;
#define  ASSERT_EQ(a,b) if(EXPECT_EQ_(a,b,__FILE__,__LINE__)) return;
#define  ASSERT_NE(a,b) if(EXPECT_NE_(a,b,__FILE__,__LINE__)) return;
#define  ASSERT_LT(a,b) if(EXPECT_LT_(a,b,__FILE__,__LINE__)) return;
#define  ASSERT_LE(a,b) if(EXPECT_LE_(a,b,__FILE__,__LINE__)) return;
#define  ASSERT_GT(a,b) if(EXPECT_GT_(a,b,__FILE__,__LINE__)) return;
#define  ASSERT_GE(a,b) if(EXPECT_GE_(a,b,__FILE__,__LINE__)) return;

#define  ASSERT_STREQ(a,b) if(EXPECT_STREQ_(a,b,__FILE__,__LINE__)) return;
#define  ASSERT_STRNE(a,b) if(EXPECT_STRNE_(a,b,__FILE__,__LINE__)) return;
#define  ASSERT_STRCASEEQ(a,b) if(EXPECT_STRCASEEQ_(a,b,__FILE__,__LINE__)) return;
#define  ASSERT_STRCASENE(a,b) if(EXPECT_STRCASENE_(a,b,__FILE__,__LINE__)) return;
#define  ASSERT_FLOAT_EQ(a,b) if(EXPECT_FLOAT_EQ_(a,b,__FILE__,__LINE__)) return;
#define  ASSERT_DOUBLE_EQ(a,b) if(EXPECT_DOUBLE_EQ_(a,b,__FILE__,__LINE__)) return;
#define  ASSERT_NEAR(a,b,c) if(EXPECT_NEAR_(a,b,c,__FILE__,__LINE__)) return;
#else
#define  ASSERT_TRUE(a) ASSERT_TRUE_(a,__FILE__,__LINE__)
#define  ASSERT_FALSE(a) ASSERT_FALSE_(a,__FILE__,__LINE__)
#define  ASSERT_EQ(a,b) ASSERT_EQ_(a,b,__FILE__,__LINE__)
#define  ASSERT_NE(a,b) ASSERT_NE_(a,b,__FILE__,__LINE__)
#define  ASSERT_LT(a,b) ASSERT_LT_(a,b,__FILE__,__LINE__)
#define  ASSERT_LE(a,b) ASSERT_LE_(a,b,__FILE__,__LINE__)
#define  ASSERT_GT(a,b) ASSERT_GT_(a,b,__FILE__,__LINE__)
#define  ASSERT_GE(a,b) ASSERT_GE_(a,b,__FILE__,__LINE__)

#define  ASSERT_STREQ(a,b) ASSERT_STREQ_(a,b,__FILE__,__LINE__)
#define  ASSERT_STRNE(a,b) ASSERT_STRNE_(a,b,__FILE__,__LINE__)
#define  ASSERT_STRCASEEQ(a,b) ASSERT_STRCASEEQ_(a,b,__FILE__,__LINE__)
#define  ASSERT_STRCASENE(a,b) ASSERT_STRCASENE_(a,b,__FILE__,__LINE__)
#define  ASSERT_FLOAT_EQ(a,b) ASSERT_FLOAT_EQ_(a,b,__FILE__,__LINE__)
#define  ASSERT_DOUBLE_EQ(a,b) ASSERT_DOUBLE_EQ_(a,b,__FILE__,__LINE__)
#define  ASSERT_NEAR(a,b,c) ASSERT_NEAR_(a,b,c,__FILE__,__LINE__)
#endif
/////////////////////////////////////////////////////////
inline int ASSERT_TRUE_(int condition,const char* fname,int line) {
  if(EXPECT_TRUE_(condition,fname,line)) abort();
  return(0);
}
/////////////////////////////////////////////////////////
inline int ASSERT_FALSE_(int condition,const char* fname,int line) {
  if(EXPECT_FALSE_(condition,fname,line)) abort();
  return(0);
}
/////////////////////////////////////////////////////////
template<class T,class E>
inline int ASSERT_EQ_(const T& a,const E& b,const char* fname,int line) {
  if(EXPECT_EQ_(a,b,fname,line)) abort();
  return(0);
}
/////////////////////////////////////////////////////////
template<class T,class E>
inline int ASSERT_NE_(const T& a,const E& b,const char* fname,int line) {
  if(EXPECT_NE_(a,b,fname,line)) abort();
  return(0);
}
/////////////////////////////////////////////////////////
template<class T,class E>
inline int ASSERT_LT_(const T& a,const E& b,const char* fname,int line) {
  if(EXPECT_LT_(a,b,fname,line)) abort();
  return(0);
}
/////////////////////////////////////////////////////////
template<class T,class E>
inline int ASSERT_LE_(const T& a,const E& b,const char* fname,int line) {
  if(EXPECT_LE_(a,b,fname,line)) abort();
  return(0);
}
/////////////////////////////////////////////////////////
template<class T,class E>
inline int ASSERT_GT_(const T& a,const E& b,const char* fname,int line) {
  if(EXPECT_GT_(a,b,fname,line)) abort();
  return(0);
}
/////////////////////////////////////////////////////////
template<class T,class E>
inline int ASSERT_GE_(const T& a,const E& b,const char* fname,int line) {
  if(EXPECT_GE_(a,b,fname,line)) abort();
  return(0);
}
/////////////////////////////////////////////////////////
inline int ASSERT_STREQ_(const char* a,const char* b,const char* fname,int line) {
  if(EXPECT_STREQ_(a,b,fname,line)) abort();
  return(0);
}
/////////////////////////////////////////////////////////
inline int ASSERT_STRNE_(const char* a,const char* b,const char* fname,int line) {
  if(EXPECT_STRNE_(a,b,fname,line)) abort();
  return(0);
}
/////////////////////////////////////////////////////////
inline int ASSERT_STRCASEEQ_(const char* a,const char* b,const char* fname,int line) {
  if(EXPECT_STRCASEEQ_(a,b,fname,line)) abort();
  return(0);
}
/////////////////////////////////////////////////////////
inline int ASSERT_STRCASENE_(const char* a,const char* b,const char* fname,int line) {
  if(EXPECT_STRCASENE_(a,b,fname,line)) abort();
  return(0);
}
/////////////////////////////////////////////////////////
template<class T,class E>
inline int ASSERT_FLOAT_EQ_(const T& a,const E& b,const char* fname,int line) {
  if(EXPECT_FLOAT_EQ_(a,b,fname,line)) abort();
  return(0);
}
/////////////////////////////////////////////////////////
template<class T,class E>
inline int ASSERT_DOUBLE_EQ_(const T& a,const E& b,const char* fname,int line) {
  if(EXPECT_DOUBLE_EQ_(a,b,fname,line)) abort();
  return(0);
}
/////////////////////////////////////////////////////////
template<class T,class E,class K>
inline int ASSERT_NEAR_(const T& a,const E& b,const T& c,const char* fname,int line) {
  if(EXPECT_DOUBLE_EQ_(a,b,fname,line)) abort();
  return(0);
}

/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
#include <ertti.h>
/////////////////////////////////////////////////////////
#define TEST(GroupName,TestName) void GroupName##TestName() 
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
int gtest(const char* gtestfile,const char* expr="*") {
  int err=0;
  int store_cpp = G__getpreprocessormode();
  G__preprocessormode(1);
  int stat=G__loadfile(gtestfile);
  G__preprocessormode(store_cpp);
  RESET_LOCALFAIL();
  //RESET_PASSFAIL();
  G__MethodInfo gfunc;
  char call[500];
  while(gfunc.Next()) {
    if(strcmp(gfunc.FileName(),gtestfile)==0) {
      sprintf(call,"%s()",gfunc.Name()); //printf("%s\n",call);
      RESET_PASSFAIL();
      G__calc(call);
      SAVE_PASSFAIL(gfunc.Name());
    }
  }
  err = LOCALFAIL_COUNT();
  //SAVE_PASSFAIL(gtestfile);
  stat=G__unloadfile(gtestfile);
  return(err);
}

/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
int ctest(const string& cintopt,const string& combase
	  ,const string& opt,const string& expr="*") {
  char com[1000];
  //sprintf(com,"cint _time.cxx cint %s %s %s",cintopt.c_str(),combase.c_str(),opt.c_str());
  sprintf(com,"cint %s %s %s",cintopt.c_str(),combase.c_str(),opt.c_str());
  printf("%s\n",com);
  return(system(com));
}

#pragma endif // UTIL_CINTTEST_H
