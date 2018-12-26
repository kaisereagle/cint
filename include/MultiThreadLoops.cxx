/*****************************************************************************
* MultiThreadLoops.cxx
*
* Description: 
*  Multi-Thread looping library
*
* Author : Masaharu Goto
*
* 2015/July/2 Masaharu Goto
*
*****************************************************************************/
/************************************************************************
 * Copyright(c) 1995~2015  Masaharu Goto (cint@pcroot.cern.ch)
 *
 * Permission to use, copy, modify and distribute this software and its 
 * documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation. The author makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 ************************************************************************/

#ifdef MULTITHREADLOOPS
#define MULTITHREADLOOPS

////////////////////////////////////////////////////////////////////////////
#ifdef WIN32
#include <windows.h>
#else // LINUX, MacOS
#include <pthread.h>
#endif // WINDOWS, LINUX, MacOS

////////////////////////////////////////////////////////////////////////////
unsigned int MT_num_threads = 4;
unsigned int MT_num_threads_limit 64;
unsigned int MT_thread_timeout = 10000000;

////////////////////////////////////////////////////////////////////////////
struct MT_multithread_data {
  unsigned int  threadid;
  unsigned int  istart;
  unsigned int  iend;
  vector<void*> p_obj;
};

////////////////////////////////////////////////////////////////////////////
void multithread_dispatcher(
			    map<string,int>& comstr
			    ,const vector<string>& cmatchs) {
  unsigned int i, istep = cmatchs.size()/EasyExp_num_threads;
  struct EasyExp_multithread_data *mtdataary
    = new EasyExp_multithread_data[EasyExp_num_threads];
  vector<map<string,int> > mtcomstr; mtcomstr.resize(EasyExp_num_threads);// customize
  //printf("size = %d  istep=%d\n",cmatchs.size(),istep);
  for(i=0;i<EasyExp_num_threads;i++) {
    mtdataary[i].threadid = i;
#ifdef MT2
    mtdataary[i].p_obj.push_back((void*)&mtcomstr[i]); // customize
    //MULTI_THREAD_OBJ_ASSIGN(map<string,int>,comstr,0);
    MULTI_THREAD_OBJ_ASSIGN(const vector<string>,cmatchs,1);
    //mtdataary[i].p_obj.push_back((void*)&cmatchs); // customize
#else
    mtdataary[i].p_comstr = &mtcomstr[i];
    mtdataary[i].p_cmatchs = &cmatchs;
#endif
    mtdataary[i].istart = istep*i;
    mtdataary[i].iend   = istep*(i+1);
  }
  mtdataary[i-1].iend = cmatchs.size();
#ifdef WIN32
  HANDLE *h = (HANDLE*)malloc(sizeof(HANDLE)*EasyExp_num_threads);
  DWORD  *ThreadID = (DWORD*)malloc(sizeof(DWORD)*EasyExp_num_threads);
  for(i=0;i<EasyExp_num_threads;i++) {
    h[i] = CreateThread(NULL,0
			,(LPTHREAD_START_ROUTINE)thread_commonsubstrmap
			,(void*)(&mtdataary[i]),0
			,&ThreadID[i]);
  }
  WaitForMultipleObjects(EasyExp_num_threads,h,true,EasyExp_thread_timeout); 
  for(i=0;i<EasyExp_num_threads;i++) CloseHandle(h[i]);
  //printf("joined \n");
  free((void*)ThreadID);
  free((void*)h);
#else
  pthread_t *pt = (pthread_t *)malloc(EasyExp_num_threads*sizeof(pthread_t));
  for (i=0;i<EasyExp_num_threads;i++) {
    pthread_create(&pt[i], NULL,thread_commonsubstrmap, (void *)(&mtdataary[i]));
  }
  for (i=0;i<EasyExp_num_threads;i++) pthread_join(pt[i], NULL);
  free((void*)pt);
#endif
  //printf("before merge \n");
  mergemap_string_int(comstr,mtcomstr); // customize
  delete[] mtdataary;
  //printf("merged \n");
}

#endif  //MULTITHREADLOOPS
