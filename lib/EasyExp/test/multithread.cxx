//////////////////////////////////////////////////////////////////////
// EasyExp multithread test
//
//  2015/Aug/19  Masaahru Goto
//////////////////////////////////////////////////////////////////////
#pragma include <EasyExp.dll> // Test Target
#pragma include <string>
#include "_util_cinttest.h"
//#include <CSV.h>       // Utility, Caution:: this library also includes EasyExp. 
//#include <exception>

//////////////////////////////////////////////////////////////////////
char* onelength_str[] = {"a"};
//////////////////////////////////////////////////////////////////////
char* randstr(char* in[],int Size) {
  return(in[rand()%Size]);
}

int num_threads[] = { 1,2,3,4, 8 };  
int num_threads_limit[] = { 64, 500, 50000 };  
int onelength_int[] = {0};

//////////////////////////////////////////////////////////////////////
int unmatch(const vector<string>& a,const vector<string>& b) {
  if(a.size()!=b.size()) return(a.size()-b.size());
  int err=0;
  for(unsigned int i=0;i<a.size();i++) {
    if(a[i]!=b[i]) ++err;
  }
  return(err);
}

int pass=0;
//////////////////////////////////////////////////////////////////////
// EasyExp match , unmatch
//////////////////////////////////////////////////////////////////////
int test1a() {
  RESET_LOCALFAIL();
  int err=0;
  unsigned int i;
  vector<string> in;
  vector<string> out[4][8];
  EasyExp e("Test(Pattern|String|Case)(10210..20021)_MultiThread[34]");
  char buf[200];
  char* a[]={ "Pattern" , "String" , "Case" , "suite", "banana", "room" };
  char* b[]={ "0","1","2","3","a","b","c"};
  for(i=0;i<30000;i++) {
    sprintf(buf,"Test%s%d_MultiThread%s"
	    ,randstr(a,sizeof(a)/sizeof(onelength_str))
	    ,rand()%25000
	    ,randstr(b,sizeof(b)/sizeof(onelength_str)));
    in.push_back(buf);
  }

  int ithread, ilimit;
  printf("test1a ithread:ilimit ");
  for(ithread=0;ithread<sizeof(num_threads)/sizeof(onelength_int);ithread++) {
    for(ilimit=0;ilimit<sizeof(num_threads_limit)/sizeof(onelength_int);ilimit++) {
      EasyExp_num_threads = num_threads[ithread];
      EasyExp_num_threads_limit = num_threads_limit[ilimit];
      printf("%d:%d  ",EasyExp_num_threads,EasyExp_num_threads_limit);
      out[ilimit][ithread] = e.match(in);
      //printf("num_threads=%d limit=%d  %d/%d\n",EasyExp_num_threads,EasyExp_num_threads_limit,out[ilimit][ithread].size(),in.size());
      err=unmatch(out[0][0],out[ilimit][ithread]);
      EXPECT_FALSE(err);
      if(err) {
	printf(" test1a error num_threads=%d limit=%d\n",EasyExp_num_threads,EasyExp_num_threads_limit);
      }
    }
  }
  printf("\n");
  return(LOCALFAIL_COUNT());
}

//////////////////////////////////////////////////////////////////////
int test1b() {
  RESET_LOCALFAIL();
  int err;
  unsigned int i;
  vector<string> in;
  vector<string> out[4][8];
  EasyExp e("Test(Pattern|String|Case)(10210..20021)_MultiThread[34]");
  char buf[200];
  char* a[]={ "Pattern" , "String" , "Case" , "suite", "banana", "room" };
  char* b[]={ "0","1","2","3","a","b","c"};
  for(i=0;i<30000;i++) {
    sprintf(buf,"Test%s%d_MultiThread%s"
	    ,randstr(a,sizeof(a)/sizeof(onelength_str))
	    ,rand()%25000
	    ,randstr(b,sizeof(b)/sizeof(onelength_str)));
    in.push_back(buf);
  }

  int ithread, ilimit;
  printf("test1b ithread:ilimit ");
  for(ithread=0;ithread<sizeof(num_threads)/sizeof(onelength_int);ithread++) {
    for(ilimit=0;ilimit<sizeof(num_threads_limit)/sizeof(onelength_int);ilimit++) {
      EasyExp_num_threads = num_threads[ithread];
      EasyExp_num_threads_limit = num_threads_limit[ilimit];
      printf("%d:%d  ",EasyExp_num_threads,EasyExp_num_threads_limit);
      out[ilimit][ithread] = e._match(in);
      //printf("num_threads=%d limit=%d  %d/%d\n",EasyExp_num_threads,EasyExp_num_threads_limit,out[ilimit][ithread].size(),in.size());
      err = unmatch(out[0][0],out[ilimit][ithread]) ;
      EXPECT_FALSE(err);
      if(err) {
	printf(" test1a error num_threads=%d limit=%d\n",EasyExp_num_threads,EasyExp_num_threads_limit);
      }
    }
  }
  printf("\n");
  return(LOCALFAIL_COUNT());
}

//////////////////////////////////////////////////////////////////////
// EasyExp synthesis 
//////////////////////////////////////////////////////////////////////
int test2a() {
  RESET_LOCALFAIL();
  int err;
  unsigned int i;
  vector<string> in,_in;
  string out[4][8];
  char buf[200];
  char* a[]={ "Pattern" , "String" , "Case" };
  char* _a[]={ "apple" , "orange" , "banana"  };
  char* b[]={ "0","1","2","3","a","b","c"};
  char* _b[]={ "9","8","7","6","x","y","z"};
  for(i=0;i<300;i++) {
    sprintf(buf,"Test%s%d_MultiThread%s"
	    ,randstr(a,sizeof(a)/sizeof(onelength_str))
	    ,rand()%25000
	    ,randstr(b,sizeof(b)/sizeof(onelength_str)));
    in.push_back(buf);
    //printf("%d %s\n",i,buf);
    sprintf(buf,"Test%s%d_MultiThread%s"
	    ,randstr(_a,sizeof(_a)/sizeof(onelength_str))
	    ,rand()%25000
	    ,randstr(_b,sizeof(_b)/sizeof(onelength_str)));
    _in.push_back(buf);
    //printf("%d %s\n",i,buf);
  }

  int ithread, ilimit;
  printf("test2a ithread:ilimit ");
  for(ithread=0;ithread<sizeof(num_threads)/sizeof(onelength_int);ithread++) {
    for(ilimit=0;ilimit<sizeof(num_threads_limit)/sizeof(onelength_int);ilimit++) {
      EasyExp_num_threads = num_threads[ithread];
      EasyExp_num_threads_limit = num_threads_limit[ilimit];
      printf("%d:%d  ",EasyExp_num_threads,EasyExp_num_threads_limit);
      //printf("num_threads=%d limit=%d\n",EasyExp_num_threads,EasyExp_num_threads_limit);
      out[ilimit][ithread] = EasyExpSynthesis(in,_in,0,10);
      err = out[0][0]!=out[ilimit][ithread] ;
      EXPECT_FALSE(err);
      if(err) {
	printf(" test1a error num_threads=%d limit=%d %s %s\n",EasyExp_num_threads,EasyExp_num_threads_limit,out[0][0].c_str(),out[ilimit][ithread].c_str());
      }
    }
  }
  printf("\n");
  return(LOCALFAIL_COUNT());
}


//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
int main() {
  int err=0;
  printf("### EasyExp Multithread test\n");

  printf("test1a-b\n");
  err+=test1a();
  err+=test1b();

  printf("test2a\n");
  err+=test2a();

  printf("EasyExp Multithread test result pass=%d  fail=%d\n",pass,err);
  SAVE_PASSFAIL("multithread");
  exit(err);
}

//////////////////////////////////////////////////////////////////////



