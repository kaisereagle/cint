/***************************************************************************
* Clustering.cxx
*
***************************************************************************/
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

#include "Clustering.h"
#include "StatEasyExp.h"

////////////////////////////////////////////////////////////////////////////
#ifndef NOMINMAXCLUSTERING
template<class T> static inline T max(const T& a,const T& b) {return(a>b?a:b);}
template<class T> static inline T min(const T& a,const T& b) {return(a<b?a:b);}
#endif

int CLDEBUG=0;
double NEGATIVEDICAV = 1;

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
// General Clustering Framework 
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

#ifdef CSVCLUSTERING_MULTI_THREAD
////////////////////////////////////////////////////////////////////////////
unsigned int CSVClustering_num_threads = 4;
unsigned int CSVClustering_num_threads_limit = 64;
unsigned int CSVClusteringColumn_num_threads_limit = 500;
unsigned int CSVClustering_thread_timeout = 1000000;
struct CSVClustering_multithread_data {
  unsigned int  threadid;
  vector<void*>     p_obj;
  unsigned int  istart;
  unsigned int  iend;
};

#define MULTI_THREAD_OBJ_RETRIEVE(Tobj,obj,index)	\
  Tobj& obj = *(Tobj*)(mtdata->p_obj[index])

#define MULTI_THREAD_OBJ_ASSIGN(Tobj,obj,index)	\
  mtdataary[i].p_obj.push_back((void*)&obj)

#define MULTI_THREAD_VAL_RETRIEVE(Tobj,obj,index)	\
  Tobj obj = (Tobj)(mtdata->p_obj[index])

#define MULTI_THREAD_VAL_ASSIGN(Tobj,obj,index)	\
  mtdataary[i].p_obj.push_back((void*)obj)

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
#ifdef WIN32
#include <windows.h>
#undef min
#undef max
#else // LINUX, MacOS
#include <pthread.h>
#endif // WINDOWS, LINUX, MacOS
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
static
#ifdef WIN32
int 
#else
void*
#endif
thread_makesimilaritymatrixcolumn(void* arg) {
  CSVClustering_multithread_data *mtdata = (CSVClustering_multithread_data*)arg;
  MULTI_THREAD_OBJ_RETRIEVE(Collection,cx,0);  // customize
  MULTI_THREAD_VAL_RETRIEVE(long,j,1);         // customize
  MULTI_THREAD_VAL_RETRIEVE(SimilarityFunction,simfunc,2);         // customize
  unsigned int istart = mtdata->istart;
  unsigned int iend   = mtdata->iend;
  unsigned int i,k;
  //printf("%d istart=%d iend=%d\n",mtdata->threadid,istart,iend);
  double r;
  Line key = cx.getkey();
  string a = key[j+1].c_str();
  for(i=istart;i<iend;i++) {
    if(cx[i][j+1].tostring()=="") {
      r = simfunc(cx[i][0].c_str(),a.c_str());
      cx[i].let(j+1,r);
    }
  }
  return(0);
}

////////////////////////////////////////////////////////////////////////////
static
void multithread_makesimilaritymatrixcolumn(Collection& cx,int j
					    ,SimilarityFunction simfunc) {
  unsigned int i, istep = cx.size()/CSVClustering_num_threads;
  struct CSVClustering_multithread_data *mtdataary
    = new CSVClustering_multithread_data[CSVClustering_num_threads];
  for(i=0;i<CSVClustering_num_threads;i++) {
    mtdataary[i].threadid = i;
    MULTI_THREAD_OBJ_ASSIGN(Collection,cx,0);  // customize
    MULTI_THREAD_VAL_ASSIGN(long,j,1);         // customize
    MULTI_THREAD_VAL_ASSIGN(SimilarityFunction,simfunc,2);         // customize
    mtdataary[i].istart = istep*i;
    mtdataary[i].iend   = istep*(i+1);
  }
  mtdataary[i-1].iend = cx.size();
#ifdef WIN32
  HANDLE *h = (HANDLE*)malloc(sizeof(HANDLE)*CSVClustering_num_threads);
  DWORD  *ThreadID = (DWORD*)malloc(sizeof(DWORD)*CSVClustering_num_threads);
  for(i=0;i<CSVClustering_num_threads;i++) {
    h[i] = CreateThread(NULL,0
			,(LPTHREAD_START_ROUTINE)thread_makesimilaritymatrixcolumn
			,(void*)(&mtdataary[i]),0
			,&ThreadID[i]);
  }
  WaitForMultipleObjects(CSVClustering_num_threads,h,true,CSVClustering_thread_timeout); 
  for(i=0;i<CSVClustering_num_threads;i++) CloseHandle(h[i]);
  //printf("joined \n");
  free((void*)ThreadID);
  free((void*)h);
#else
  pthread_t *pt = (pthread_t *)malloc(CSVClustering_num_threads*sizeof(pthread_t));
  for (i=0;i<CSVClustering_num_threads;i++) {
    pthread_create(&pt[i], NULL,thread_makesimilaritymatrixcolumn, (void *)(&mtdataary[i]));
  }
  for (i=0;i<CSVClustering_num_threads;i++) pthread_join(pt[i], NULL);
  free((void*)pt);
#endif
  //printf("before merge \n");
  delete[] mtdataary;
  //printf("merged \n");
}

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
static
#ifdef WIN32
int 
#else
void*
#endif
thread_makesimilaritymatrix(void* arg) {
  CSVClustering_multithread_data *mtdata = (CSVClustering_multithread_data*)arg;
  MULTI_THREAD_OBJ_RETRIEVE(Collection,cx,0);  // customize
  MULTI_THREAD_VAL_RETRIEVE(SimilarityFunction,simfunc,2);         // customize
  unsigned int istart = mtdata->istart;
  unsigned int iend   = mtdata->iend;
  unsigned int i,j;
  //printf("%d istart=%d iend=%d\n",mtdata->threadid,istart,iend);
  double r;
  Line key = cx.getkey();
  string a = key[j+1].c_str();
  for(j=istart;j<iend;j++) {
    r = simfunc(cx[j][0].c_str(),key[j+1].c_str());
    cx[j].let(j+1,r);
    for(i=j+1;i<cx.size();i++) {
      if(cx[i][j+1].tostring()=="") {
	r = simfunc(cx[i][0].c_str(),key[j+1].c_str());
	cx[i].let(j+1,r);
	cx[j].let(i+1,r);
      }
    }
  }
  return(0);
}

////////////////////////////////////////////////////////////////////////////
static
void multithread_makesimilaritymatrix(Collection& cx,SimilarityFunction simfunc) {
  unsigned int i, istep = cx.size()/CSVClustering_num_threads;
  struct CSVClustering_multithread_data *mtdataary
    = new CSVClustering_multithread_data[CSVClustering_num_threads];
  for(i=0;i<CSVClustering_num_threads;i++) {
    mtdataary[i].threadid = i;
    MULTI_THREAD_OBJ_ASSIGN(Collection,cx,0);  // customize
    MULTI_THREAD_VAL_ASSIGN(SimilarityFunction,simfunc,2);         // customize
    mtdataary[i].istart = istep*i;
    mtdataary[i].iend   = istep*(i+1);
  }
  mtdataary[i-1].iend = cx.size();
#ifdef WIN32
  HANDLE *h = (HANDLE*)malloc(sizeof(HANDLE)*CSVClustering_num_threads);
  DWORD  *ThreadID = (DWORD*)malloc(sizeof(DWORD)*CSVClustering_num_threads);
  for(i=0;i<CSVClustering_num_threads;i++) {
    h[i] = CreateThread(NULL,0
			,(LPTHREAD_START_ROUTINE)thread_makesimilaritymatrix
			,(void*)(&mtdataary[i]),0
			,&ThreadID[i]);
  }
  WaitForMultipleObjects(CSVClustering_num_threads,h,true,CSVClustering_thread_timeout); 
  for(i=0;i<CSVClustering_num_threads;i++) CloseHandle(h[i]);
  //printf("joined \n");
  free((void*)ThreadID);
  free((void*)h);
#else
  pthread_t *pt = (pthread_t *)malloc(CSVClustering_num_threads*sizeof(pthread_t));
  for (i=0;i<CSVClustering_num_threads;i++) {
    pthread_create(&pt[i], NULL,thread_makesimilaritymatrix, (void *)(&mtdataary[i]));
  }
  for (i=0;i<CSVClustering_num_threads;i++) pthread_join(pt[i], NULL);
  free((void*)pt);
#endif
  //printf("before merge \n");
  delete[] mtdataary;
  //printf("merged \n");
}

////////////////////////////////////////////////////////////////////////////
#endif // CSVMULTITHREAD_MULTI_THREAD



////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
Collection makeMatrix(const Collection& ci) {
  Collection cx = ci;
  cx = cx.column(0,0)._empty(0);
  int i,sz = cx.size();
  Line line;
  for(i=0;i<cx.size();i++) {
    cx[i].let(sz,"");
    line.let(i+1,cx[i][0]);
  }
  cx.push_back(line);
  cx.setkey(cx.size()-1);
  return(cx);
}


////////////////////////////////////////////////////////////////////////
Collection MakeSimilarityMatrixColumn(Collection& cx,int j,SimilarityFunction simfunc
				      ,unsigned int& done) {
  double r;
  int i;
  Line key = cx.getkey();
#ifdef CSVCLUSTERINGCOLUMN_MULTI_THREAD
  if(CSVClustering_num_threads>1 &&
     cx.size()-done>CSVClusteringColumn_num_threads_limit) {
    if(CLDEBUG) printf("th ");
    multithread_makesimilaritymatrixcolumn(cx,j,simfunc);
    ++done;
    return(cx);
  }
#else
  for(i=0;i<cx.size();i++) {
    if(cx[i][j+1].tostring()=="") {
      r = simfunc(cx[i][0].c_str(),key[j+1].c_str());
      cx[i].let(j+1,r);
      //cx[j].let(i+1,r);
    }
  }
  ++done;
#endif 
  return(cx);
}

////////////////////////////////////////////////////////////////////////
Collection MakeSimilarityMatrix(Collection& cx,SimilarityFunction simfunc) {
  double r;
  int i,j;
#ifdef CSVCLUSTERING_MULTI_THREAD
  if(CSVClustering_num_threads>1 &&
     cx.size()>CSVClustering_num_threads_limit) {
    if(CLDEBUG) printf("multi-thead\n");
    multithread_makesimilaritymatrix(cx,simfunc);
    return(cx);
  }
#endif
  Line key = cx.getkey();
  for(j=0;j<cx.size();j++) {
    r = simfunc(cx[j][0].c_str(),key[j+1].c_str());
    cx[j].let(j+1,r);
    for(i=j+1;i<cx.size();i++) {
      if(cx[i][j+1].tostring()=="") {
        r = simfunc(cx[i][0].c_str(),key[j+1].c_str());
        cx[i].let(j+1,r);
        cx[j].let(i+1,r);
      }
    }
  }
  return(cx);
}


////////////////////////////////////////////////////////////////////////
// Utility
////////////////////////////////////////////////////////////////////////
void dispindex(const vector<int>& v) {
  vector<int>::const_iterator first,last=v.end();
  for(first=v.begin();first!=last;++first) {
    printf("%d ",*first);
  }
  printf(": %lu\n",v.size());
}
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
Collection GatherSortedInfo(const Collection& csim,const Collection& cid
			,map<int,int>& idtoclass) {
  Collection r;
  Line line;
  unsigned int i,j;

  // Set title line
  line = csim.getkey();
  r.push_back(line);
  r.setkey(0);
  char buf[500];
  int id,cls;

  for(i=0;i<csim.size();i++) {
    for(j=1;j<csim[i].size();j++) {
      id = cid[i][j].toint();
      cls = idtoclass[id];
      sprintf(buf,"%d %s %d",cls,csim[i][j].c_str(),id);
      line.let(j,buf);
    }
    r.push_back(line);
  }
  return(r);
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
// Clustering algorithms
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
//void RegExpExtraction(Collection& cx,int x,int xcls,int xregex) ;

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
// Clustering algorithm 1
//   This adhoc algorithm performs resonably well.
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
Collection Clustering_core1(Collection& cx,SimilarityFunction simfunc,int clsin,const vector<double>& arg) {
  double lowthresh=0.5;
  double upthresh=2.0;
  double percentage=0.0;
  if(arg.size()>=1) lowthresh=arg[0];
  if(arg.size()>=2) upthresh=arg[1];
  if(arg.size()>=3) percentage=arg[2];
if(CLDEBUG) printf("lowthresh=%g upthresh=%g percentatge=%g\n",lowthresh,upthresh,percentage);
  // This algorithm works reasonably well. 
  // However, as can be easily imagined, the first class gets more samples 
  // than consequitive ones. 
  int i,j;
  int cls = clsin;
  int xcls = cx.getxexact("class");
  Line line;
  map<string,int> count,tmp;
  Collection c,categ,t,_t;
  int n=0;
  int x;
  int icateg=0;
  map<string,int>::iterator first,last;

  // 使われていない?
  line.let(0,"item");
  //line.let(1,"count");
  line.let(1,"categ"); 
  categ.push_back(line); categ.setkey(0);
  line.clear();

  c = cx;

  unsigned int done=0;
if(CLDEBUG) printf("core1 AAA %d\nj=",c.getkey().size());
  for(j=1;j<c.getkey().size();j++) {
if(CLDEBUG) printf("%d ",j);
    if(c.match(0,c.getkey()[j].s.c_str()).size()==0) continue;
    // あるsampleに注目し、他全てとの距離を評価、Threshold内 t と外 _t に分類
    c = MakeSimilarityMatrixColumn(c,j-1,simfunc,done);
    c.sort(j); // high value top, low value bottom
    if(j==1 && percentage>0) {
      int nlim = c.size()*percentage/100;
      lowthresh = c[nlim][j].todouble();
      upthresh = c[0][j].todouble()+1;
if(CLDEBUG) printf("Percentage %g%% %d/%d lowthresh=%g upthresh=%g\n",percentage,nlim,c.size(),lowthresh,upthresh);
    }
    t = c.range(j,lowthresh,upthresh);
    _t = c._range(j,lowthresh,upthresh);

    // threshold 内の sample について table 作成
    count.clear();
    for(i=0;i<t.size();i++) count[t[i][0].tostring()];

    // Table要素全てについて、他全てとの距離を評価、thresh内のsampleを数える
    tmp = count;
    first = tmp.begin();
    last  = tmp.end();
    while(first!=last) {
      x = c.getxexact((*first).first);
      c = MakeSimilarityMatrixColumn(c,x-1,simfunc,done);
      c.sort(x); // high value top, low value bottom
      t = c.range(x,lowthresh,upthresh);
      for(i=0;i<t.size();i++) ++count[t[i][0].tostring()];
      ++first;
    }

    // thresh内 sample数がある基準を満たすものを category要素とする
    //if(count.size()) printf("#### %d / %d\n",n++,count.size());
    _t.clear();
    _t.setmatchmode(EasyExp::Exact);
    first = count.begin();
    last  = count.end();
    while(first!=last) {
      if((*first).second>count.size()/10) {
        //printf("%-80s  %3d\n",(*first).first.c_str(),(*first).second);
        line.let(0,(*first).first);
        //line.let(1,(*first).second);
        line.let(1,icateg);
        _t.push_back(line);
      }
      ++first;
    }

    // master table へのcategory付けを行い、次処理対象から削除する
    if(_t.size()) {
      //_t.disp();
      categ.merge(_t); // 使われていない?
      c.match(0,_t,0).let(xcls,(double)icateg);
      c = c._match(0,_t,0); // 次処理対象から削除
      c.setmatchmode(EasyExp::Exact);
      ++icateg;
    }
  }

if(CLDEBUG) printf("\ncore1 YYY\n");
  //RegExpExtraction(cx,0,xcls,xcls+1);
if(CLDEBUG) printf("\ncore1 ZZZ\n");
  //return(categ);
  return(cx);
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
// Clustering algorithm 2
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
void SortSimilarityMatrixByColumn(const Collection& SimilarityMatrix,int xcls
		,Collection& cname, Collection& csim) {
  cname = SimilarityMatrix.duplicate();
  csim = SimilarityMatrix.duplicate();
  Collection c=SimilarityMatrix.duplicate();
  for(unsigned int j=1;j<xcls;j++) {
    c.sort(j); // high value top, low value bottom
    for(unsigned int i=0;i<c.size();i++) {
      cname[i].let(j,c[i][0].tostring());
      csim[i].let(j,c[i][j].todouble());
    }
  }
  cname.let(0,"");
  csim.let(0,"");
}
////////////////////////////////////////////////////////////////////////
void SortSimilarityMatrixByColumnWithID(const Collection& SimilarityMatrix
		,int xcls ,Collection& cid, Collection& csim) {
  cid = SimilarityMatrix.duplicate();
  csim = SimilarityMatrix.duplicate();
  Collection c=SimilarityMatrix.duplicate();
  Collection t=c;
  for(unsigned int j=1;j<xcls;j++) {
    c.sort(j); // high value top, low value bottom
    for(unsigned int i=0;i<c.size();i++) {
      cid[i].let(j,t.gety(&c[i]));
      csim[i].let(j,c[i][j].todouble());
    }
  }
  cid.let(0,"");
  csim.let(0,"");
}
////////////////////////////////////////////////////////////////////////
void CopyClassification(map<string,int>& nametoclass,Collection& c,int xcls) {
  int cls;
  for(unsigned int i=0;i<c.size();i++) {
    cls = nametoclass[c[i][0].tostring()];
    c[i].let(xcls,(double)cls);
  }
}

////////////////////////////////////////////////////////////////////////
void CopyClassificationWithID(map<int,int>& idtoclass,Collection& c,int xcls,const string& label="",int index=0,int ncls=-1,double icav=0,double dicav=0) {
  int cls;
  if(label!="") {
    char buf[10]; sprintf(buf,"%d:%d:%g:%g",index,ncls,icav,dicav);
    c.getkey().let(xcls,label+buf);
  }
  for(unsigned int i=0;i<c.size();i++) {
    cls = idtoclass[i];
    c[i].let(xcls,(double)cls);
  }
}

////////////////////////////////////////////////////////////////////////
// classification
////////////////////////////////////////////////////////////////////////
int ClassificationByIndex(const Collection& csim,const Collection& cname
		,const vector<int>& index
		,map<string,int>& nametoclass) {
  string name;
  int icls=0;
  int itmp;
  nametoclass.clear();
  unsigned int i;
//if(CLDEBUG>1) printf("ClassificationByIndex AAA\n");
  for(unsigned int j=1;j<=csim.size();j++) {
    itmp = -1;
//if(CLDEBUG>1) printf("ClassificationByIndex BBB j=%d index[j]=%d\n",j,index[j]);
    if(index[j]<0)  { printf("%s\n",csim.getkey()[j].c_str()); continue; }
    for(i=0;i<=index[j];i++) {
      name = cname[i][j].tostring();
      if(itmp<0 && nametoclass[name]>0) itmp = nametoclass[name];
    }
    if(itmp<0) itmp = ++icls;
//if(CLDEBUG>1) printf("ClassificationByIndex CCC j=%d index[j]=%d\n",j,index[j]);
    for(i=0;i<=index[j];i++) {
      name = cname[i][j].tostring();
      nametoclass[name] = itmp;
    }
//if(CLDEBUG>1) printf("ClassificationByIndex DDD\n");
  }
//if(CLDEBUG>1) printf("ClassificationByIndex EEE\n");
  map<string,int>::iterator first,last = nametoclass.end();  
  for(first=nametoclass.begin();first!=last;++first) --(*first).second;
//if(CLDEBUG>1) printf("ClassificationByIndex FFF\n");
  return(icls);
}
////////////////////////////////////////////////////////////////////////
int ClassificationByIndexWithID(const Collection& csim,const Collection& cid
		,const vector<int>& index
		,map<int,int>& idtoclass) {
  int id;
  int icls=0;
  int itmp;
  map<int,int> cls;
  idtoclass.clear();
  map<int,int>::iterator first,last;
  unsigned int i;
  for(unsigned int j=1;j<=csim.size();j++) {
    cls.clear();
    if(index[j]<0)  { printf("%s\n",csim.getkey()[j].c_str()); continue; }
    // search already named sample id and cls
    for(i=0;i<=index[j];i++) {
      id = cid[i][j].toint();
      if(idtoclass[id]>0) ++cls[idtoclass[id]];
    }

    if(cls.size()==0) itmp = ++icls; // no classification is found
    else if(cls.size()==1) { // only one classification is found
      first = cls.begin();
      itmp = (*first).first;
    }
    else { // mutiple classifications are found -> consolidate
      // take the first classification
      first = cls.begin();
      itmp = (*first).first;
      // overwrite other classifications
      for(id=0;id<=csim.size();id++) {
        if(idtoclass[id]>0) { // if already classified
          first=cls.begin();last=cls.end();
          for(++first;first!=last;++first) { // from second to last
            if(idtoclass[id]==(*first).first) idtoclass[id] = itmp;
          } 
        } 
      } 
    }
    // assign classification to this column
    for(i=0;i<=index[j];i++) {
      id = cid[i][j].toint();
      idtoclass[id] = itmp;
    }
  }

  // re-number classification,
  itmp = 0; 
  cls.clear();
  last = idtoclass.end();  
  for(first=idtoclass.begin();first!=last;++first) {
    icls = (*first).second;
    if(icls>0) {
      if(cls[icls]==0) cls[icls] = ++itmp;
      (*first).second = cls[icls]-1;
    }
    //else printf("idtoclass[%d]= %d\n",(*first).first,(*first).second);
  }

  return(itmp); 
  //return(cls.size());
}

////////////////////////////////////////////////////////////////////////
// Calculate statistics
////////////////////////////////////////////////////////////////////////
statistics StatisticsToIndex(const Collection& csim
		,const vector<int>& index) {
  statistics st;
  for(unsigned int j=1;j<=csim.size();j++) {
    for(unsigned int i=0;i<index[j];i++) st.add(csim[i][j].todouble());
  }
  return(st);
}

////////////////////////////////////////////////////////////////////////
statistics StatisticsAtIndex(const Collection& csim
		,const vector<int>& index) {
  statistics st;
  for(unsigned int j=1;j<=csim.size();j++) {
    st.add(csim[index[j]][j].todouble());
  }
  return(st);
}

////////////////////////////////////////////////////////////////////////
statistics StatisticsAll(const Collection& csim
		,const vector<int>& index) {
  statistics st;
  value v;
  for(unsigned int j=1;j<=csim.size();j++) {
    for(unsigned int i=0;i<csim.size();i++) st.add(csim[i][j].todouble());
  }
  return(st);
}

////////////////////////////////////////////////////////////////////////
// Calculate class statistics
////////////////////////////////////////////////////////////////////////
map<int,statistics> StatisticsByClass(const Collection& csim
		,const Collection& cname
		,const vector<int>& index
	        ,map<string,int>& nametoclass) {
  map<int,statistics> stc;
  string name;
  int icls;
  for(unsigned int j=1;j<=csim.size();j++) {
    name = cname[0][j].tostring();
    icls = nametoclass[name];
    for(unsigned int i=0;i<=index[j];i++) {
      stc[icls].add(csim[i][j].todouble());
    }
  }
  return(stc);
}
////////////////////////////////////////////////////////////////////////
map<int,statistics> StatisticsByClassWithID(const Collection& csim
		,const Collection& cid
		,const vector<int>& index
	        ,map<int,int>& idtoclass) {
  map<int,statistics> stc;
  int id;
  int icls;
  for(unsigned int j=1;j<=csim.size();j++) {
    id = cid[0][j].toint();
    icls = idtoclass[id];
    for(unsigned int i=0;i<=index[j];i++) {
      stc[icls].add(csim[i][j].todouble());
    }
  }
  return(stc);
}

////////////////////////////////////////////////////////////////////////
map<int,statistics> StatisticsByClassAllWithID(const Collection& cx
		,const Collection& csim
		,const Collection& cid
		,const vector<int>& index
	        ,map<int,int>& idtoclass) {
  map<int,statistics> stc;
  int cls,cls2;
  for(unsigned int j=1;j<=csim.size();j++) {
    cls = idtoclass[j-1];
    for(unsigned int i=0;i<cx.size();i++) {
      cls2 = idtoclass[i];
      if(cls==cls2) stc[cls].add(csim[i][j].todouble());
    }
  }
  return(stc);
}

////////////////////////////////////////////////////////////////////////
// Index setting & update
////////////////////////////////////////////////////////////////////////
#if 0
void SetInitialIndex(const Collection& csim,int xcls,int order,vector<int>& index,double thresh) {
  double prev;
  int flag;
  statistics st;
  index.resize(csim.size()+1);
  unsigned int i,j;
  for(j=1;j<xcls;j++) {
    flag = 0; prev = 1.0;
    for(i=0;i<csim.size();i++) {
      if(prev>csim[i][j].todouble()) {
        if(++flag==order) { 
          st.add(csim[i][j].todouble());
          index[j]=i; 
	  break;
        }
        prev = csim[i][j].todouble();
      }
    }
  }
  for(j=1;j<xcls;j++) {
    if(index[j]>0 && csim[i][j].todouble>st.mean()-st.stddev()*thresh) --index[j];
  }
}
#else
void SetInitialIndex(const Collection& csim,int xcls,int order,vector<int>& index,double thresh) {
//if(CLDEBUG) printf("SetInitialIndex thresh=%g\n",thresh);
  double prev;
  int flag;
  index.resize(csim.size()+1);
  for(unsigned int j=1;j<xcls;j++) {
    flag = 0; prev = 1.0;
    for(unsigned int i=0;i<csim.size();i++) {
      if(prev>csim[i][j].todouble()) {
        if(++flag==order) { 
          if(csim[i][j].todouble()>thresh) index[j]=i; // record the most neighbering sample index
          else                             index[j]=i-1;
	  break;
        }
        prev = csim[i][j].todouble();
      }
    }
  }
}
#endif

////////////////////////////////////////////////////////////////////////
void UpdateIndexByStat(const Collection& csim
			,int xcls
			,const statistics& st,double thresh
			,vector<int>& index,int pol) {
  double Th = st.mean()-st.stddev()*thresh;
  for(unsigned int j=1;j<=csim.size();j++) {
    if(pol>=0)
      while(index[j]<csim.size()&& csim[index[j]][j]>Th) ++index[j];
    if(pol<=0)
      while(index[j]>=1 && csim[index[j]][j]<Th) --index[j];
  }
}

////////////////////////////////////////////////////////////////////////
void UpdateIndexByThresh(const Collection& csim
			,int xcls
			,double thresh
			,vector<int>& index,int pol) {
  double Th = thresh;
  for(unsigned int j=1;j<=csim.size();j++) {
    while(index[j]<csim.size()&& csim[index[j]][j]>Th) ++index[j];
    if(index[j]>0) --index[j];
    if(pol>=0)
      while(index[j]<csim.size()&& csim[index[j]][j]>Th) ++index[j];
    if(pol<=0)
      while(index[j]>=1 && csim[index[j]][j]<Th) --index[j];
  }
}


////////////////////////////////////////////////////////////////////////
void UpdateIndexByClassStat(const Collection& cname
			,const Collection& csim,int xcls
			,map<int,statistics>& stc
			,map<string,int>& nametoclass,double thresh
			,vector<int>& index,int pol) {
  double Th;
  string name;
  statistics st;
  for(unsigned int j=1;j<xcls;j++) {
    name = cname.getkey()[j].tostring();
    st = stc[nametoclass[name]];
    Th = st.mean() - st.stddev()*thresh;
    if(pol>=0)
      while(index[j]<csim.size()&& csim[index[j]][j]>Th) ++index[j];
    if(pol<=0)
      while(index[j]>=1 && csim[index[j]][j]<Th) --index[j];
  }
}

////////////////////////////////////////////////////////////////////////
void UpdateIndexByClassStatWithID(const Collection& cid
			,const Collection& csim,int xcls
			,map<int,statistics>& stc
			,map<int,int>& idtoclass,double thresh
			,vector<int>& index,int pol) {
  double Th;
  int id;
  statistics st;
  for(unsigned int j=1;j<xcls;j++) {
    id = j-1;
    st = stc[idtoclass[id]];
    Th = st.mean() - st.stddev()*thresh;
    if(pol>=0)
      while(index[j]<csim.size()&& csim[index[j]][j]>Th) ++index[j];
    if(pol<=0)
      while(index[j]>=1 && csim[index[j]][j]<Th) --index[j];
  }
}

////////////////////////////////////////////////////////////////////////
void UpdateIndexByWeightedClassStatWithID(const Collection& cid
			,const Collection& csim,int xcls
			,map<int,statistics>& stc
			,map<int,int>& idtoclass,double thresh
			,vector<int>& index,int pol) {
  double Th;
  int id;
  statistics st;

  // data数少ない cluster の stddev が 0 になるのを防ぐ
  map<int,statistics>::iterator first,last=stc.end();
  double maxstddev= -1e99;
  for(first=stc.begin();first!=last;++first) {
    st = (*first).second;
    if(maxstddev<st.stddev()) maxstddev=st.stddev();
  }

  for(unsigned int j=1;j<xcls;j++) {
    id = j-1;
    st = stc[idtoclass[id]];
    if(st.stddev()==0) Th = st.mean() - maxstddev*thresh;
    else               Th = st.mean() - st.stddev()*thresh;
    if(pol>=0)
      while(index[j]<csim.size()&& csim[index[j]][j]>Th) ++index[j];
    if(pol<=0)
      while(index[j]>=1 && csim[index[j]][j]<Th) --index[j];
  }
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
map<int,double> NormalizedThreshold(const map<int,statistics>& stc
				,double thresh,double ratio) {
  int maxn=0,cls,maxcls;
  statistics maxst,st;
  map<int,statistics>::const_iterator first,last = stc.end();
  for(first=stc.begin();first!=last;++first) {
    if((*first).second.nsample()>maxn) {
      maxcls = (*first).first;
      maxst = (*first).second;
      maxn = maxst.nsample();
    }
  }
  //double dif = thresh - maxst.mean();
  double dif = (1-thresh)*ratio;
  int n;
  map<int,double> normthresh;
  for(first=stc.begin();first!=last;++first) {
    cls = (*first).first;
    st = (*first).second;
    n = st.nsample();
    normthresh[cls] = thresh - dif*(maxn-n)/maxn;
//printf("cls=%d  thresh=%g normthresh=%g dif=%g n=%d maxn=%d\n",cls,thresh,normthresh[cls],dif,n,maxn);
  }
  return(normthresh);
}

////////////////////////////////////////////////////////////////////////
void UpdateIndexByClassNormalizedStatWithID(const Collection& cid
			,const Collection& csim,int xcls
			,map<int,statistics>& stc
			,map<int,int>& idtoclass,double thresh,double ratio
			,vector<int>& index,int pol) {
  double Th;
  int id;

  map<int,double> normthresh = NormalizedThreshold(stc,thresh,ratio);
  
  for(unsigned int j=1;j<xcls;j++) {
    id = j-1;
    Th = normthresh[idtoclass[id]];
    if(pol>=0)
      while(index[j]<csim.size()&& csim[index[j]][j]>Th) ++index[j];
    if(pol<=0)
      while(index[j]>=1 && csim[index[j]][j]<Th) --index[j];
  }
}

////////////////////////////////////////////////////////////////////////
void UpdateIndexByClassStat(const Collection& cname
			,const Collection& csim,int xcls
			,map<int,statistics>& stc
			,map<string,int>& nametoclass
			,const vector<double>& thresh
			,vector<int>& index,int pol) {
  double Th;
  string name;
  statistics st;
  int cls;
  for(unsigned int j=1;j<xcls;j++) {
    name = cname.getkey()[j].tostring();
    cls = nametoclass[name];
    st = stc[cls];
    Th = st.mean() - st.stddev()*thresh[cls];
    if(pol>=0)
      while(index[j]<csim.size()&& csim[index[j]][j]>Th) ++index[j];
    if(pol<=0)
      while(index[j]>=1 && csim[index[j]][j]<Th) --index[j];
  }
}

////////////////////////////////////////////////////////////////////////
void UpdateIndexByClassStatWithID(const Collection& cid
			,const Collection& csim,int xcls
			,map<int,statistics>& stc
			,map<int,int>& idtoclass
			,const vector<double>& thresh
			,vector<int>& index,int pol) {
  double Th;
  int id;
  statistics st;
  int cls;
  for(unsigned int j=1;j<xcls;j++) {
    id = j-1;
    cls = idtoclass[id];
    st = stc[cls];
    Th = st.mean() - st.stddev()*thresh[cls];
    if(pol>=0)
      while(index[j]<csim.size()&& csim[index[j]][j]>Th) ++index[j];
    if(pol<=0)
      while(index[j]>=1 && csim[index[j]][j]<Th) --index[j];
  }
}
////////////////////////////////////////////////////////////////////////
// core algorithm 2
//   -> core4
////////////////////////////////////////////////////////////////////////
Collection Clustering_core2(Collection& cx,SimilarityFunction simfunc,int clsin,const vector<double>& arg) {
  double thresh=0.5;
  double repeat=2.0;
  if(arg.size()>=1) thresh=arg[0];
  if(arg.size()>=2) repeat=arg[1];
//if(CLDEBUG) printf("thresh=%g repeat=%g\n",thresh,thresh);
  unsigned int i;
  int xcls = cx.getxexact("class");

  // Evaluate Similarity between all samples
  Collection SimilarityMatrix = MakeSimilarityMatrix(cx,simfunc);

  // make key to y and key to class table 
  map<string,int> nametoy;
  for(i=0;i<SimilarityMatrix.size();i++) nametoy[SimilarityMatrix[i][0].tostring()] = i;

  // make Similarity-name table sorted by similarity for each sample and initialize index table
  Collection cname, csim; 
  SortSimilarityMatrixByColumn(SimilarityMatrix,xcls,cname,csim);

  // 1 あるSampleからの内積距離順位index初期化
  // set initial index
  vector<int> index; 
  SetInitialIndex(csim,xcls,1,index);

  // 2 内積距離同順位統計値との比較
  // get initial statistics
  statistics st;
  st = StatisticsAtIndex(csim,index);

  map<string,int> nametoclass;
  ClassificationByIndex(csim,cname,index,nametoclass);
  if(repeat>=0) {
    UpdateIndexByStat(csim,xcls,st,thresh,index,0);

    // Classification loop
    for(i=0;i<(int)repeat;i++) {
      ClassificationByIndex(csim,cname,index,nametoclass);
  
      // 3 同一class内内積距離統計値との比較
      map<int,statistics> stc;
      stc = StatisticsByClass(csim,cname,index,nametoclass);
  
      UpdateIndexByClassStat(cname,csim,xcls,stc,nametoclass,thresh,index,0);
    }
  }

  CopyClassification(nametoclass,cx,xcls);
  return(cx);
}

////////////////////////////////////////////////////////////////////////
// Evaluate classification
////////////////////////////////////////////////////////////////////////
// |V|      = csm.size()
// |Ak|     = csm.match(xcls,k).size()
//  di      = csim.sigma(i+1,1,i+1,-1)
// vol(V)   = csm.sigma(1,0)-csm.size() or csim.sigma(1,1)
// vol(Ak)  => if(csm[i][xcls]==k) volAk += di
// W(Ak,Am) => t=csm.match(xcls,k).stat()  
//             if(nametoclass[t.getkey()[i+1]]==m) 
//               Wkm += t.getstat()[i+1].sigma()
// W(Ak,/Ak) => t=csm._match(xcls,k).stat()  
//              if(nametoclass[t.getkey()[i+1]]==k) 
//                Wk_ += t.getstat()[i+1].sigma()
// cut      = Σk W(Ak,/Ak) / 2
// Ncut     = Σk { W(Ak,/Ak)/vol(Ak) } / 2
// Ratiocut = Σk { W(Ak,/Ak)/|Ak| } / 2
//

////////////////////////////////////////////////////////////////////////
double TotalVolume(const Collection& csimmat) {
  Collection c = csimmat;
  unsigned int sz = csimmat.size();
  double vol = c.sigma(1,0,sz,sz-1) - c.size();
  return(vol);
}

////////////////////////////////////////////////////////////////////////
vector<double> Volume(const Collection& csimmat,int xcls) {
  unsigned int sz = csimmat.size(), cls;
  Collection c = csimmat;
  Collection cu = c.unique_count(xcls);
  vector<double> vol; vol.resize(cu.size());
  double tmp;
  for(unsigned int i=0;i<c.size();i++) {
    tmp = c[i].stat(1,sz).sigma();
    cls = c[i][xcls].toint();
    vol[cls] += tmp;
  }
  return(vol);
}

////////////////////////////////////////////////////////////////////////
vector<double> Volume(const Collection& csortsim,map<string,int>& nametoclass){
  Collection c=csortsim;
  vector<double> vol; vol.resize(nametoclass.size());
  c.stat();
  unsigned int sz = c.size(), cls;
  for(unsigned int i=1;i<=sz;i++) {
    cls = nametoclass[c.getkey()[i].tostring()];
    vol[cls] += c.getstat()[i].sigma() - 1.0 /*self*/ ;
  }
  return(vol);
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
// iJanbo Shi and Jitendra Malik, 
//
// "Normalized Cuts and Image Segmentation"
// IEEE Transactions on Pattern Analysis and Machine Intelligence, 
// Vol.22, No.8, Aug 2000
//
// Ulrike von Luxburg,
// "A Tutorial on Spectral Clustering"
// Max Planck Institute for Biological Cybernetics
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
double Cut(const Collection& csimmat,int xcls,int ncls,map<string,int>& nametoclass) {
  Collection c = csimmat;
  unsigned int sz = c.size();
  Collection cu = c.unique_count(xcls);
  vector<double> cut2; cut2.resize(ncls);
  Collection ck;
  double val;
  unsigned int i,j;
  int k,m;
  for(i=0;i<cu.size();i++) {
    k = cu[i][0].toint();
    ck = c.match(xcls,(double)k);
    ck.stat();
    for(j=1;j<xcls;j++) {
      m = nametoclass[ck.getkey()[j].tostring()];
      val = ck.getstat()[j].sigma();
      if(k!=m)  cut2[k] += val;
    }
  }
  double unnormalizedcut2=0;
  for(i=0;i<cut2.size();i++) {
    if(cut2[i]>0) 
      unnormalizedcut2 += cut2[i];
  }
  //return(unnormalizedcut2/2);
  return(unnormalizedcut2/2/cut2.size());
}

////////////////////////////////////////////////////////////////////////
double CutWithID(const Collection& csimmat,int xcls,int ncls,map<int,int>& idtoclass) {
  Collection c = csimmat;
  unsigned int sz = c.size();
  Collection cu = c.unique_count(xcls);
  vector<double> cut2; cut2.resize(ncls);
  Collection ck;
  double val;
  unsigned int i,j;
  int k,m;
  for(i=0;i<cu.size();i++) {
    k = cu[i][0].toint();
    ck = c.match(xcls,(double)k);
    ck.stat();
    for(j=1;j<xcls;j++) {
      m = idtoclass[j-1];
      val = ck.getstat()[j].sigma();
      if(k!=m)  cut2[k] += val;
    }
  }
  double unnormalizedcut2=0;
  for(i=0;i<cut2.size();i++) {
    if(cut2[i]>0) 
      unnormalizedcut2 += cut2[i];
  }
  //return(unnormalizedcut2/2);
  return(unnormalizedcut2/2/cut2.size());
}

////////////////////////////////////////////////////////////////////////
double RatioCut(const Collection& csimmat,int xcls,int ncls,map<string,int>& nametoclass) {
  Collection c = csimmat;
  unsigned int sz = c.size();
  Collection cu = c.unique_count(xcls);
  vector<double> ab;  ab.resize(ncls);
  vector<double> cut2; cut2.resize(ncls);
  Collection ck;
  double val;
  unsigned int i,j;
  int k,m;
  for(i=0;i<cu.size();i++) {
    k = cu[i][0].toint();
    ab[k] = cu[i][1].todouble();
    ck = c.match(xcls,(double)k);
    ck.stat();
    for(j=1;j<xcls;j++) {
      m = nametoclass[ck.getkey()[j].tostring()];
      val = ck.getstat()[j].sigma();
      if(k!=m) cut2[k] += val;
    }
  }
  double ratiocut2=0;
  for(i=0;i<cut2.size();i++) {
    if(cut2[i]>0) 
      ratiocut2+= (cut2[i]/ab[i]);
  }
  //return(ratiocut2/2);
  return(ratiocut2/2/cut2.size());
}

////////////////////////////////////////////////////////////////////////
double RatioCutWithID(const Collection& csimmat,int xcls,int ncls,map<int,int>& idtoclass) {
  Collection c = csimmat;
  unsigned int sz = c.size();
  Collection cu = c.unique_count(xcls);
  vector<double> ab;  ab.resize(ncls);
  vector<double> cut2; cut2.resize(ncls);
  Collection ck;
  double val;
  unsigned int i,j;
  int k,m;
  for(i=0;i<cu.size();i++) {
    k = cu[i][0].toint();
    ab[k] = cu[i][1].todouble();
    ck = c.match(xcls,(double)k);
    ck.stat();
    for(j=1;j<xcls;j++) {
      m = idtoclass[j-1];
      val = ck.getstat()[j].sigma();
      if(k!=m) cut2[k] += val;
    }
  }
  double ratiocut2=0;
  for(i=0;i<cut2.size();i++) {
    if(cut2[i]>0) 
      ratiocut2+= (cut2[i]/ab[i]);
  }
  //return(ratiocut2/2);
  return(ratiocut2/2/cut2.size());
}

////////////////////////////////////////////////////////////////////////
double Ncut(const Collection& csimmat,int xcls,int ncls
            ,map<string,int>& nametoclass) {
  Collection c=csimmat;
  Collection cu = c.unique_count(xcls);
  vector<double> vol;  vol.resize(ncls);
  vector<double> cut2; cut2.resize(ncls);
  Collection ck;
  double val;
  unsigned int i,j;
  int k,m;
  for(i=0;i<cu.size();i++) {
    k = cu[i][0].toint();
    ck = c.match(xcls,(double)k);
    ck.stat();
    for(j=1;j<xcls;j++) {
      m = nametoclass[ck.getkey()[j].tostring()];
      val = ck.getstat()[j].sigma();
      if(k==m)  vol[k]  += val - 1.0/*self*/;
      else      cut2[k] += val;
    }
  }
  double normalizedcut2=0;
  for(i=0;i<cut2.size();i++) {
    if(vol[i]>0) 
      normalizedcut2 += (cut2[i]/vol[i]);
  }
  //return(normalizedcut2/2);
  return(normalizedcut2/2/cut2.size());
}

////////////////////////////////////////////////////////////////////////
double NcutWithID(const Collection& csimmat,int xcls,int ncls
            ,map<int,int>& idtoclass) {
  Collection c=csimmat;
  Collection cu = c.unique_count(xcls);
  vector<double> vol;  vol.resize(ncls);
  vector<double> cut2; cut2.resize(ncls);
  Collection ck;
  double val;
  unsigned int i,j;
  int k,m;
  for(i=0;i<cu.size();i++) {
    k = cu[i][0].toint();
    ck = c.match(xcls,(double)k);
    ck.stat();
    for(j=1;j<xcls;j++) {
      m = idtoclass[j-1];
      val = ck.getstat()[j].sigma();
      if(k==m)  vol[k]  += val - 1.0/*self*/;
      else      cut2[k] += val;
    }
  }
  double normalizedcut2=0;
  for(i=0;i<cut2.size();i++) {
    if(vol[i]>0) normalizedcut2 += (cut2[i]/vol[i]);
  }
  //return(normalizedcut2/2);
  return(normalizedcut2/2/cut2.size());
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
double GapRatio(const Collection& cname,const Collection& csim
		,const vector<int>& index
		,map<string,int>& nametoclass,int xcls,int ncls) {
  vector<statistics> gap; gap.resize(ncls);
  vector<statistics> within;  within.resize(ncls);
  unsigned int i,j;
  for(i=0;i<ncls;i++) {
    gap[i].init();
    within[i].init();
  }
  int cls1;
  for(j=1;j<xcls;j++) {
    cls1 = nametoclass[csim.getkey()[j].tostring()];
    i=0;
    while(cls1==nametoclass[cname[i][j].tostring()] && i<csim.size()) ++i;
    // within category statistics
    within[cls1].add(csim[i-1][j].todouble());
    // minimum gap statistics
    gap[cls1].add(csim[i][j].todouble());
  }

  // calculate ratio of minimum gap and within category distance average
  double gr=0;
  int n=0;
  for(i=0;i<ncls;i++) {
    if(gap[i].nsample() && within[i].nsample()) {
      gr += gap[i].max()/within[i].mean();
      //gr += gap[i].max()/within[i].min();
      //gr += gap[i].mean()/within[i].mean();
      ++n;
    }
  }
  return(gr/n);
}

////////////////////////////////////////////////////////////////////////
double GapRatioWithID(const Collection& cid,const Collection& csim
		,const vector<int>& index
		,map<int,int>& idtoclass,int xcls,int ncls) {
  vector<statistics> gap; gap.resize(ncls);
  vector<statistics> within;  within.resize(ncls);
  unsigned int i,j;
  for(i=0;i<ncls;i++) {
    gap[i].init();
    within[i].init();
  }
  int cls1;
  for(j=1;j<xcls;j++) {
    cls1 = idtoclass[j-1];
    i=0;
    while(cls1==idtoclass[cid[i][j].toint()] && i<csim.size()) ++i;
    // within category statistics
    within[cls1].add(csim[i-1][j].todouble());
    // minimum gap statistics
    gap[cls1].add(csim[i][j].todouble());
  }

  // calculate ratio of minimum gap and within category distance average
  double gr=0;
  int n=0;
  for(i=0;i<ncls;i++) {
    if(gap[i].nsample() && within[i].nsample()) {
      gr += gap[i].max()/within[i].mean();
      //gr += gap[i].max()/within[i].min();
      //gr += gap[i].mean()/within[i].mean();
      ++n;
    }
  }
  return(gr/n);
}

////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
// core algorithm 3
//   -> core4
////////////////////////////////////////////////////////////////////////
Collection Clustering_core3(Collection& cx,SimilarityFunction simfunc,int clsin
	,const vector<double>& arg) {
  double thresh=0.5;
  if(arg.size()>=1) thresh = arg[0];
  unsigned int i;
  int xcls = cx.getxexact("class");

  // Evaluate Similarity between all samples
  Collection SimilarityMatrix = MakeSimilarityMatrix(cx,simfunc);

  // make key to y and key to class table 
  map<string,int> nametoy;
  for(i=0;i<SimilarityMatrix.size();i++) nametoy[SimilarityMatrix[i][0].tostring()] = i;

  // make Similarity-name table sorted by similarity for each sample and initialize index table
  Collection cname, csim; 
  SortSimilarityMatrixByColumn(SimilarityMatrix,xcls,cname,csim);
//cname.disp("cname.csv","w");
//csim.disp("csim.csv","w");

  // 1 あるSampleからの内積距離順位index初期化
  // set initial index
  vector<int> index; 
  SetInitialIndex(csim,xcls,1,index);

  // 初期indexによる分類
  map<string,int> nametoclass;
  int ncls = ClassificationByIndex(csim,cname,index,nametoclass);

  CopyClassification(nametoclass,cx,xcls);
  double ncut = Ncut(cx,xcls,ncls,nametoclass);
  double cut = Cut(cx,xcls,ncls,nametoclass);
  double rcut = RatioCut(cx,xcls,ncls,nametoclass);
  double gapratio = GapRatio(cname,csim,index,nametoclass,xcls,ncls),prevgap=1e10;

  // 2 内積距離同順位統計値との比較
  // get initial statistics
  statistics st;
  st = StatisticsAtIndex(csim,index);
  // global statistics による index update
  UpdateIndexByStat(csim,xcls,st,thresh,index,0);

  // Classification loop
  int maxiter=20;
  int count=0;
  for(i=0;i<maxiter;i++) {
    ncls = ClassificationByIndex(csim,cname,index,nametoclass);
    CopyClassification(nametoclass,cx,xcls);
    //ncut = Ncut(cx,xcls,ncls,nametoclass);
    //cut = Cut(cx,xcls,ncls,nametoclass);
    //rcut = RatioCut(cx,xcls,ncls,nametoclass);
    gapratio = GapRatio(cname,csim,index,nametoclass,xcls,ncls);
printf("Gapratio=%g  %d\n",gapratio,ncls);
    if(prevgap==gapratio) ++count; else count=0; if(count>1) break;
    prevgap=gapratio;
  
    // 3 同一class内内積距離統計値との比較
    map<int,statistics> stc;
    stc = StatisticsByClass(csim,cname,index,nametoclass);
//for(int j=0;j<stc.size();j++) stc[j].disp();printf("\n");
 
    UpdateIndexByClassStat(cname,csim,xcls,stc,nametoclass,thresh,index,0);
  }

  //ncls = ClassificationByIndex(csim,cname,index,nametoclass);
  //CopyClassification(nametoclass,cx,xcls);
  return(cx);
}

////////////////////////////////////////////////////////////////////////
void ClusterConsolidation(const Collection& cname,const Collection& csim
			,int xcls,vector<int>& index) {
  double mingap = -1000, gap;
  vector<int> mingapindex;
  for(int j=1;j<xcls;j++) {
    gap = csim[index[j]][j].todouble();
    if(gap==1.0) {
      gap = csim[index[j]+1][j].todouble();
//printf("j=%d gap = %g\n",j,gap);
      if(gap>mingap) {
        mingapindex.clear();
        mingap = gap;
        mingapindex.push_back(j);
      }
      else if(gap==mingap) {
        mingapindex.push_back(j);
      }
    }
  }
  for(int i=0;i<mingapindex.size();i++) {
    ++index[mingapindex[i]];
    printf("Consolidate %d with %s %g %s\n"
		,mingapindex[i]
		,csim.getkey()[mingapindex[i]].c_str()
		,csim[index[mingapindex[i]]][mingapindex[i]].todouble()
		,cname[index[mingapindex[i]]][mingapindex[i]].c_str());
  }
}

////////////////////////////////////////////////////////////////////////
void ClusterConsolidationWithID(const Collection cx,const Collection& cid,const Collection& csim
			,int xcls,vector<int>& index) {
  double mingap = -1000, gap;
  vector<int> mingapindex;
  for(int j=1;j<xcls;j++) {
    gap = csim[index[j]][j].todouble();
    if(gap==1.0) {
      gap = csim[index[j]+1][j].todouble();
//printf("j=%d gap = %g\n",j,gap);
      if(gap>mingap) {
        mingapindex.clear();
        mingap = gap;
        mingapindex.push_back(j);
      }
      else if(gap==mingap) {
        mingapindex.push_back(j);
      }
    }
  }
  for(int i=0;i<mingapindex.size();i++) {
    ++index[mingapindex[i]];
    printf("Consolidate %d with %s %g %s\n"
		,mingapindex[i]
		,csim.getkey()[mingapindex[i]].c_str()
		,csim[index[mingapindex[i]]][mingapindex[i]].todouble()
		,cx[cid[index[mingapindex[i]]][mingapindex[i]].toint()][0].c_str());
  }
}

////////////////////////////////////////////////////////////////////////
// core algorithm 4
////////////////////////////////////////////////////////////////////////
Collection Clustering_core4(Collection& cx,SimilarityFunction simfunc,int clsin,const vector<double>& arg) {
  double thresh=2.0;
  int consolidate = (int)0;
  double initthresh=0.9;
  if(arg.size()>=1) thresh = arg[0];
  if(arg.size()>=2) consolidate = (int)arg[1];
  if(arg.size()>=3) initthresh= arg[2];
//if(CLDEBUG) printf("Clustering_core4 (%g,%d,%g)\n",thresh,consolidate,initthresh);
  unsigned int i;
  int xcls = cx.getxexact("class");

//if(CLDEBUG) printf("core4 AAA\n");
  // Evaluate Similarity between all samples
  Collection SimilarityMatrix = MakeSimilarityMatrix(cx,simfunc);

//if(CLDEBUG) printf("core4 BBB\n");
  // make key to y and key to class table 
  //map<string,int> nametoy;
  //for(i=0;i<SimilarityMatrix.size();i++) nametoy[SimilarityMatrix[i][0].tostring()] = i;

//if(CLDEBUG) printf("core4 CCC\n");
  // make Similarity-name table sorted by similarity for each sample and initialize index table
  Collection cname, csim; 
  SortSimilarityMatrixByColumn(SimilarityMatrix,xcls,cname,csim);
//cname.disp("cname.csv","w");
//csim.disp("csim.csv","w");

//if(CLDEBUG) printf("core4 DDD\n");
  // 1 あるSampleからの内積距離順位index初期化
  // set initial index
  vector<int> index; 
  SetInitialIndex(csim,xcls,1,index,initthresh);

//if(CLDEBUG) printf("core4 EEE\n");
  // 初期indexによる分類
  map<string,int> nametoclass;
  int ncls = ClassificationByIndex(csim,cname,index,nametoclass);

//if(CLDEBUG) printf("core4 FFF\n");
  CopyClassification(nametoclass,cx,xcls);
  //double ncut = Ncut(cx,xcls,ncls,nametoclass);
  //double cut = Cut(cx,xcls,ncls,nametoclass);
  //double rcut = RatioCut(cx,xcls,ncls,nametoclass);
//if(CLDEBUG) printf("core4 GGG\n");
  double prevgap=1e10;
  double gapratio = GapRatio(cname,csim,index,nametoclass,xcls,ncls);
//if(CLDEBUG) printf("core4 GGGb\n");

  // 2 内積距離同順位統計値との比較
  // get initial statistics
  statistics st;
  st = StatisticsAtIndex(csim,index);
  // global statistics による index update
  UpdateIndexByStat(csim,xcls,st,thresh,index,0);

//if(CLDEBUG) printf("core4 HHH\n");
  // Classification loop
  int maxiter=20;
  int count=0;

  for(i=0;i<maxiter;i++) {
//if(CLDEBUG) printf("core4 HHHb\n");
    ncls = ClassificationByIndex(csim,cname,index,nametoclass);
//if(CLDEBUG) printf("core4 HHHc\n");
    CopyClassification(nametoclass,cx,xcls);
//if(CLDEBUG) printf("core4 HHHd\n");
    //ncut = Ncut(cx,xcls,ncls,nametoclass);
    //cut = Cut(cx,xcls,ncls,nametoclass);
    //rcut = RatioCut(cx,xcls,ncls,nametoclass);
    gapratio = GapRatio(cname,csim,index,nametoclass,xcls,ncls);
printf("Gapratio=%g  %d\n",gapratio,ncls);
    if(prevgap==gapratio) ++count; else count=0; if(count>1) break;
    prevgap=gapratio;
  
//if(CLDEBUG) printf("core4 III\n");
    // 3 同一class内内積距離統計値との比較
    map<int,statistics> stc;
    stc = StatisticsByClass(csim,cname,index,nametoclass);
//for(int j=0;j<stc.size();j++) stc[j].disp();printf("\n");
 
    UpdateIndexByClassStat(cname,csim,xcls,stc,nametoclass,thresh,index,0);
  }


//if(CLDEBUG) printf("core4 JJJ\n");
  for(i=0;i<consolidate;i++) {
    ClusterConsolidation(cname,csim,xcls,index);
  }
  
  ncls = ClassificationByIndex(csim,cname,index,nametoclass);
  CopyClassification(nametoclass,cx,xcls);
  return(cx);
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
// Intra-Cluster Distance calculation class
//
// Ariel E. Baya and Pablo M. Granitto, 
// "How Many Clusters: A Validation Index for Arbitrary-Shaped Clusters"
// IEEE/ACM Transactions on Computational Biology and Bioinformatics, 
//   Vol.10 No.2, March/April 2013
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
class IntraClusterDistance_t {
 public:
  /////////////////////////////////////////////////////////////////////
  // constructor
  IntraClusterDistance_t(const Collection& cx_in
		        ,const Collection& cid_in
 			,const Collection& csim_in
			,map<int,int>& idtoclass_in
			,int xcls_in,int& ncls_in) 
			: cx(cx_in), cid(cid_in), csim(csim_in)
			, idtoclass(idtoclass_in)
			, xcls(xcls_in), ncls(ncls_in) {
    cicd = cx.duplicate(); 
    cpath=cx.duplicate();
    //ccount = cx.duplicate(); 
    //clength = cx.duplicate(); 
    progress.resize(cx.size()+1);
    init();
  }

  /////////////////////////////////////////////////////////////////////
  void init() { // initializatino before each calculation
    for(unsigned int i=1;i<=cx.size();i++) {
      cicd.let(i,"");
      cpath.let(i,"");
      //ccount.let(i,"");
      //clength.let(i,"");
      progress[i]=0;
    }
  }

  /////////////////////////////////////////////////////////////////////
  int calculate_sortedcolumn(int j) {
    unsigned int i,i2,k,m;
    string path;
    char buf[500];
    double ic1,ic2;
    if(progress[j]==0) {
      // distance to itself
      cicd[j-1][j].set(1.0);
      sprintf(buf," %d ",j-1); 
      cpath[j-1][j].set(buf);
      // distance to the closest sample
      k = ++progress[j];
      //k = progress[j] = 1;
      i = cid[k][j].toint();
      if(idtoclass[j-1]!=idtoclass[i]) {
//printf("i%d k%d j%d other category idi%d idj%d\n",i,k,j,idtoclass[i],idtoclass[j-1]);
        cicd[i][j].set(0.0); // other category
        cicd[j-1][i+1].set(0.0); // other category
        return(0);
      }
      cicd[i][j].set(csim[k][j]);
      cicd[j-1][i+1].set(csim[k][j]);
      sprintf(buf," %d %d ",j-1,i); path = buf;
      cpath[i][j].set(path);
      cpath[j-1][i+1].set(path);
      //ccount[i][j].add(1.0);
//printf("i%d k%d j%d set path idi%d idj%d '%s'\n",i,k,j,idtoclass[i],idtoclass[j-1],path.c_str());
      return(1); // set a path
    }
    else if(progress[j]>0) {
      k = ++progress[j];
      if(k>cx.size()) return(0);
      i = cid[k][j].toint();
      if(idtoclass[j-1]!=idtoclass[i]) {
        cicd[i][j].set(0.0); // other category
        cicd[j-1][i+1].set(0.0); // other category
        return(0);
      }
      ic1 = csim[k][j].todouble();
      sprintf(buf," %d %d ",j-1,i); path = buf;
      for(m=0;m<k;m++) {
        i2 = cid[m][j].toint();
        ic2 = cicd[i2][ic1+1].todouble();
        if(ic2==0) continue; // other category, pass
        if(cicd[i2][j].tostring()=="") {
          --progress[j]; // unresolved path, postpone
if(CLDEBUG>2) printf("calclate_sortedcolumn j=%d k=%d m=%d i=%d i2=%d POSTPONE\n",j,k,m,i,i2);
          return(1); // postpone
        }
        if(ic1>ic2) {
          ic1 = ic2;
          path = cpath[ic2][j].tostring() ;
          sprintf(buf,"%d ",i2); path += buf;
        }
      }
      cicd[i][j].set(ic1);
      cicd[j-1][i+1].set(ic1);
      cpath[i][j].set(path);
      cpath[j-1][i+1].set(path);
      //ccount[i][j].add(1.0); // Too complicated, work on this later
if(CLDEBUG>2) printf("calclate_sortedcolumn j=%d k=%d SET path\n",j,k);
      return(1); // set a path
    }
    return(0);
  }

  /////////////////////////////////////////////////////////////////////
  double calculate() {
    init();
    int flag,prevflag=0,count=0;
    unsigned int j;
//printf("calculate\n");
    do {
      flag=0;
      for(j=1;j<=cx.size();j++) {
        flag += calculate_sortedcolumn(j);
      } 
if(CLDEBUG>2) printf("IntraClusterDistance_t::calculate() BBB %d %d\n",j,flag);
      if(prevflag==flag && ++count>10) break;
      prevflag=flag;
    } while(flag);
if(CLDEBUG>2) printf("IntraClusterDistance_t::calculate() CCC %d %d\n",j,flag);
if(CLDEBUG) cicd.disp("cicd.csv","w");
if(CLDEBUG) cpath.disp("cpath.csv","w");
    calcMinInterClusterDistance();
    return(ICav());
  }

  /////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////
  double ICav() {
    int ncls2 = ncls;
    icd.clear(); icd1.clear();
    icd.resize(ncls2); icd1.resize(ncls2);
    unsigned int i,j;
    int cls2,cls1;
    double val;
    for(j=1;j<=cx.size();j++) {
      cls1 = idtoclass[j-1];
      for(i=0;i<cx.size();i++) {
        cls2 = idtoclass[i];
        if(cls1==cls2 && cicd[i][j].tostring()!="") {
#if 1
          val = 1.0-cicd[i][j].todouble();
#else
          val = cicd[i][j].todouble();
          if(val>0) val = 1/val; else val = 100;
#endif
          icd[cls1].add(val*val);
          icd1[cls1].add(cicd[i][j].todouble());
        }
      }
    }
    double r=0;
    int n=0;
    for(i=0;i<ncls2;i++) {
      if(icd[i].nsample()>0) { r += icd[i].mean(); ++n; }
    }
    return(r/n);
  }
  /////////////////////////////////////////////////////////////////////
  void calcMinInterClusterDistance() {
    mind.clear();
    mind.resize(ncls);
    unsigned int i,j;
    int id1,id2,cls1,cls2;
    for(i=0;i<ncls;i++) mind[i].resize(ncls);
    for(j=1;j<=csim.size();j++) {
      id1 = cid[0][j].toint();
      cls1 = idtoclass[id1];
      for(i=0;i<csim.size();i++) {
        id2 = cid[i][j].toint();
        cls2 = idtoclass[id2];
        if(cls1!=cls2) {
          mind[cls1][cls2].add(csim[i][j].todouble());
	  break;
	}
      }
    }
  }
  /////////////////////////////////////////////////////////////////////
  double GapRatio(int i,int j) { 
    if(icd1[i].nsample()==0 || mind[i][j].nsample()==0) return(1);
//icd1[i].disp(); mind[i][j].disp();
    //return(icd1[i].mean()/mind[i][j].mean()); 
    return((1-icd1[i].mean())/(1-mind[i][j].mean())); 
  }
  double GapRatio(int i) {
    if(ncls<=1) return(1);
    double r=0,tmp;
    for(unsigned int j=0;j<ncls;j++) if(i!=j) {
      tmp=GapRatio(i,j);
//printf("%d:%d %g ",i,j,tmp);
      r+=tmp;
    }
//printf("\n");
    return(r/(ncls-1)) ;
  }
  double GapRatio() {
    double r=0, tmp;
    for(unsigned int i=0;i<ncls;i++) {
      tmp=GapRatio(i);
      if(CLDEBUG) printf("%d/%d:%g ",i,ncls,tmp);
      r+=tmp;
    }
    if(CLDEBUG) printf("\n");
    return(r/ncls);
  }
  /////////////////////////////////////////////////////////////////////

  Collection& get_cpath() { return(cpath); }
  Collection& get_cicd() { return(cicd); }

  /////////////////////////////////////////////////////////////////////
 private:
  // input
#ifndef __CINT__
  const Collection& cx; // Similarity matrix with temporary clustering
  const Collection& csim; // sorted similarity 
  const Collection& cid; // corresponding data id as cx[index]
  map<int,int>& idtoclass;
  const int& xcls; // "class" column 
  const int& ncls; // number of clusters
#else
  const Collection cx; // Similarity matrix with temporary clustering
  const Collection csim; // sorted similarity 
  const Collection cid; // corresponding data id as cx[index]
  map<int,int> idtoclass;
  const int xcls; // "class" column 
  const int ncls; // number of clusters
#endif

  // internal data
  Collection cicd; // Intra-Cluster Distance matrix
  Collection cpath; // Intra-Cluster connection path matrix
  //Collection ccount; // Intra-Cluster connection count
  //Collection clength; // Intra-Cluster connection length
  vector<int> progress; // internal variable manging ICd calculation progress

  vector<statistics> icd; // Intra-Cluster distance statistics for each cluster
  vector<statistics> icd1; //Intra-Cluster distance statistics for each cluster
  vector<vector<statistics> > mind; // minimum Inter-Cluster distance stat
  //statistics pathcount;
};

////////////////////////////////////////////////////////////////////////
// core algorithm 5
//  GapRatio and ICav are calculated but not used as true criteria
////////////////////////////////////////////////////////////////////////
Collection Clustering_core5(Collection& cx,SimilarityFunction simfunc,int clsin,const vector<double>& arg) {
  double thresh=2.0;
  int consolidate = (int)0;
  double initthresh=0.9;
  if(arg.size()>=1) thresh = arg[0];
  if(arg.size()>=2) consolidate = (int)arg[1];
  if(arg.size()>=3) initthresh= arg[2];
//if(CLDEBUG) printf("Clustering_core5 (%g,%d,%g)\n",thresh,consolidate,initthresh);
  unsigned int i;
  int xcls = cx.getxexact("class");

//if(CLDEBUG) printf("core5 AAA\n");
  // Evaluate Similarity between all samples
  Collection SimilarityMatrix = MakeSimilarityMatrix(cx,simfunc);

//if(CLDEBUG) printf("core5 CCC\n");
  // make Similarity-name table sorted by similarity for each sample and initialize index table
  Collection cid, csim; 
  SortSimilarityMatrixByColumnWithID(SimilarityMatrix,xcls,cid,csim);
//if(CLDEBUG) cid.disp("cid.csv","w");
//if(CLDEBUG) csim.disp("csim.csv","w");

//if(CLDEBUG) printf("core5 DDD\n");
  // 1 あるSampleからの内積距離順位index初期化
  // set initial index
  vector<int> index; 
  SetInitialIndex(csim,xcls,1,index);

//if(CLDEBUG) printf("core5 EEE\n");
  // 初期indexによる分類
  map<int,int> idtoclass;
  int ncls = ClassificationByIndexWithID(csim,cid,index,idtoclass);
  CopyClassificationWithID(idtoclass,cx,xcls);

//if(CLDEBUG) printf("core5 FFF\n");
  double cut = CutWithID(cx,xcls,ncls,idtoclass);
  double rcut = RatioCutWithID(cx,xcls,ncls,idtoclass);
  double ncut = NcutWithID(cx,xcls,ncls,idtoclass);
  double prevgap=1e10;
  double gapratio = GapRatioWithID(cid,csim,index,idtoclass,xcls,ncls);
  IntraClusterDistance_t ICav(cx,cid,csim,idtoclass,xcls,ncls);
  double icav = ICav.calculate();
  printf("Cut>=%8g RatioCut>=%8g NCut>=%8g Gapratio<=%8g ICav<=%8g ncls=%d\n"
	,cut,rcut,ncut,gapratio,icav,ncls);
//if(CLDEBUG) printf("core5 GGGb\n");

  // 2 内積距離同順位統計値との比較と index update
  // get initial statistics
  statistics st;
  st = StatisticsAtIndex(csim,index);
  UpdateIndexByStat(csim,xcls,st,thresh,index,0);

//if(CLDEBUG) printf("core5 HHH\n");
  // Classification loop
  int maxiter=20;
  int count=0;

  int xcls2=xcls;
  int prevncls=0;
  for(i=0;i<maxiter;i++) {
//if(CLDEBUG) printf("core5 HHHb\n");
    ncls = ClassificationByIndexWithID(csim,cid,index,idtoclass);
    CopyClassificationWithID(idtoclass,cx,xcls);
    if(CLDEBUG) {
      if(ncls!=prevncls) ++xcls2;
      CopyClassificationWithID(idtoclass,cx,xcls2,"iter",i,ncls);
      prevncls=ncls;
    }

//if(CLDEBUG) printf("core5 HHHd\n");
    cut = CutWithID(cx,xcls,ncls,idtoclass);
    rcut = RatioCutWithID(cx,xcls,ncls,idtoclass);
    ncut = NcutWithID(cx,xcls,ncls,idtoclass);
    gapratio = GapRatioWithID(cid,csim,index,idtoclass,xcls,ncls);
    icav = ICav.calculate();
    printf("Cut>=%8g RatioCut>=%8g NCut>=%8g Gapratio>=%8g ICav<=%8g ncls=%d\n"
	,cut,rcut,ncut,gapratio,icav,ncls);
    if(prevgap==gapratio) ++count; else count=0; if(count>1) break;
    prevgap=gapratio;
  
//if(CLDEBUG) printf("core5 III\n");
    // 3 同一class内内積距離統計値との比較
    map<int,statistics> stc;
    stc = StatisticsByClassWithID(csim,cid,index,idtoclass);
//for(int j=0;j<stc.size();j++) stc[j].disp();printf("\n");
 
    UpdateIndexByClassStatWithID(cid,csim,xcls,stc,idtoclass,thresh,index,0);
  }

//if(CLDEBUG) printf("core5 JJJ\n");
  for(i=0;i<consolidate;i++) {
    ClusterConsolidationWithID(cx,cid,csim,xcls,index);

    ncls = ClassificationByIndexWithID(csim,cid,index,idtoclass);
    CopyClassificationWithID(idtoclass,cx,xcls);
    if(CLDEBUG) CopyClassificationWithID(idtoclass,cx,xcls2++,"consol",i,ncls);

    cut = CutWithID(cx,xcls,ncls,idtoclass);
    rcut = RatioCutWithID(cx,xcls,ncls,idtoclass);
    ncut = NcutWithID(cx,xcls,ncls,idtoclass);
    gapratio = GapRatioWithID(cid,csim,index,idtoclass,xcls,ncls);
    icav = ICav.calculate();
    printf("Cut>=%8g RatioCut>=%8g NCut>=%8g Gapratio>=%8g ICav<=%8g ncls=%d\n"
	,cut,rcut,ncut,gapratio,icav,ncls);
  }
  
  ncls = ClassificationByIndexWithID(csim,cid,index,idtoclass);
  CopyClassificationWithID(idtoclass,cx,xcls);
  return(cx);
}

////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
Collection SelectCenterSample(const Collection& c) {
  Collection t=c ;
  CollectionStatistics st = t.stat();
  unsigned int i,min_index;
  double min_mean = 1e99;
  for(i=1;i<=t.size();i++) {
    if(min_mean> st[i].mean()) {
      min_index = i-1;
      min_mean = st[i].mean();
    }
  }
  t.clear();
  t.push_back(c[min_index]);
  return(t);
}
////////////////////////////////////////////////////////////////////////
Collection SortDistanceOrder(Collection& cx) {
if(CLDEBUG) printf("SortDictanceOrder AAA\n");
  Collection cr, t, c = cx.duplicate();
  int xcls = c.getx("class");
  Collection cu = c.unique_count(xcls);
  unsigned int k,i,j, newk=0, oldk;
  string cls;
  cr.push_back(c.getkey());
  cr.setkey(0);

  // Select Center sample of each cluster
  for(k=0;k<cu.size();k++) {
    cls = cu[k][0].tostring();
    t = c.match(xcls,cls);
    //t = t.allignkey(0);
    t = SelectCenterSample(t);
    if(t.size()) {
      t = c.match(0,t[0][0].tostring());
      cr.push_back(t[0]);
    }
  }

if(CLDEBUG) printf("SortDictanceOrder BBB\n");
  // Find appropriate order of distance between clusters
  Collection csimmat = cr.allignkey(0);
  Collection cid,csim;
  SortSimilarityMatrixByColumnWithID(csimmat,xcls,cid,csim);
  i=cid.size()-1;
  double val,mincsim=1e99;
  int minindex;
  for(j=1;j<=csim.size();j++) {
    val = csim[i][j].todouble();
    if(val<mincsim || 
       (val==mincsim && csim[1][minindex].todouble()<csim[1][j].todouble())) {
      minindex = j;
      mincsim = val;
    }
  }

  // make old-class-ID to new-class-ID table
  map<int,int> classmap;
  string name;
  cx.setmatchmode(EasyExp::Exact);
  for(i=0;i<cid.size();i++) {
    j = cid[i][minindex].toint();
    name = cid.getkey()[j].tostring();
    k = cx.match(0,name)[0][xcls].toint();
    classmap[k] = newk++;
  }
  
  // Re-assign class-ID
  for(i=0;i<cx.size();i++) {
    oldk = cx[i][xcls].toint();    
    newk = classmap[oldk];
    cx[i].let(xcls,newk);
  }

if(CLDEBUG) printf("SortDictanceOrder CCC\n");
  return(t);
}

////////////////////////////////////////////////////////////////////////
Collection Clustering_core6(Collection& cx,SimilarityFunction simfunc,int clsin,const vector<double>& arg) {
  double thresh=2.0;
  int  ncls_uplimit = 100;
  int  ncls_lowlimit = 2;
  int consolidate = (int)0;
  int reorder =0;
  if(arg.size()>=1) ncls_uplimit  = (int)arg[0];
  if(arg.size()>=2) ncls_lowlimit = (int)arg[1];
  if(arg.size()>=3) thresh = arg[2];
  if(arg.size()>=4) consolidate = (int)arg[3];
  if(arg.size()>=5) reorder = (int)arg[4];
  if(ncls_uplimit>cx.size()) ncls_uplimit=cx.size()-1;
  double initthresh=0.9;
  printf("Clustering_core6 (up%d,low%d,limit%g,consol%d,reorder%d)\n",ncls_uplimit,ncls_lowlimit,thresh,consolidate,reorder);
  unsigned int i;
  int xcls = cx.getxexact("class");

if(CLDEBUG) printf("core6 AAA\n");
  // Evaluate Similarity between all samples
  Collection SimilarityMatrix = MakeSimilarityMatrix(cx,simfunc);

if(CLDEBUG) printf("core6 CCC\n");
  // make Similarity-name table sorted by similarity for each sample and initialize index table
  Collection cid, csim; 
  SortSimilarityMatrixByColumnWithID(SimilarityMatrix,xcls,cid,csim);
if(CLDEBUG) cid.disp("cid.csv","w");
if(CLDEBUG) csim.disp("csim.csv","w");

if(CLDEBUG) printf("core6 DDD\n");
  // 1 あるSampleからの内積距離順位index初期化
  // set initial index
  vector<int> index; 
  SetInitialIndex(csim,xcls,1,index);
  map<int,int> idtoclass;

if(CLDEBUG) printf("core6 FFF\n");
  double cut;
  double rcut;
  double ncut;
  double prevgap;
  double gapratio;
  int ncls;
  double icav, dicav;
if(CLDEBUG) printf("core6 GGGb\n");

  // 2 内積距離同順位統計値との比較と index update
  // get initial statistics
  statistics stall;
  stall = StatisticsAll(csim,index);

if(CLDEBUG) printf("core6 HHH\n");
  // Classification loop
  int maxiter=100;
  int ndiv=20;
  int count=0;
  double stddevx;
  vector<double> v_cut, v_rcut, v_ncut, v_gapratio, v_icav, v_dicav;
  vector<int>    v_ncls;
  vector<vector<int> > v_index;
  double min_gapratio = 1e99,min_icav = 1e99, max_dicav=-1e99;
  int index_dicav;

  IntraClusterDistance_t ICav(cx,cid,csim,idtoclass,xcls,ncls);
  ncls = ncls_uplimit;
  i=0;
  while(i<maxiter && ncls>=ncls_lowlimit) {
if(CLDEBUG) printf("core6 HHHb\n");
    stddevx = 1.0 - stall.stddev()*thresh*i/ndiv;
    printf("mean=%g , stddev=%g  thresh=%g\n"
		,stall.mean(),stall.stddev(),stddevx);
    UpdateIndexByThresh(csim,xcls,stddevx,index,0);
if(CLDEBUG) printf("core6 HHHc\n");
    ncls = ClassificationByIndexWithID(csim,cid,index,idtoclass);
if(CLDEBUG) printf("core6 HHHcc\n");
    CopyClassificationWithID(idtoclass,cx,xcls);

if(CLDEBUG) printf("core6 HHHd\n");
    //if(ncls<=1) break;
    cut = CutWithID(cx,xcls,ncls,idtoclass);
    rcut = RatioCutWithID(cx,xcls,ncls,idtoclass);
    ncut = NcutWithID(cx,xcls,ncls,idtoclass);
    gapratio = GapRatioWithID(cid,csim,index,idtoclass,xcls,ncls);
    icav = ICav.calculate();
    //if(prevgap==gapratio) ++count; else count=0; if(count>1) break;
    prevgap=gapratio;

    v_cut.push_back(cut);
    v_rcut.push_back(rcut);
    v_ncut.push_back(ncut);
    v_gapratio.push_back(gapratio);

    if(!i) v_dicav.push_back(1.0);
    else {
      if(ncls>=ncls_uplimit) index_dicav=i;
      if(icav!=0 && v_icav.back()!=0) {
        dicav = (icav-v_icav.back())/icav;
        if(v_ncls.back()>=ncls_lowlimit && ncls_uplimit>=v_ncls.back()) {
          v_dicav.push_back(dicav);
          if(max_dicav<dicav) {
            printf("MAX dICav %g %d\n",dicav,i);
            max_dicav = dicav;
            index_dicav = i;
          }
          else if(dicav<0 && -dicav*2.0 > max_dicav) {
            printf("MAX dICav %g (%g) %d\n",dicav,-dicav*2,i+1);
            max_dicav = -dicav*2.0;
            index_dicav = i+1;
          }  
        }  
        else {
          printf("Out of nclk limit, abondaned\n");
        }
      }
    }

    v_icav.push_back(icav);
    v_index.push_back(index);
    v_ncls.push_back(ncls);

    printf("Cut>=%8g RatioCut>=%8g NCut>=%8g Gapratio>=%8g ICav<=%8g dICav>=%8g ncls=%d i=%d\n"
	,cut,rcut,ncut,gapratio,icav,dicav,ncls,i);
    ++i;
if(CLDEBUG) printf("core6 III\n");
  }

if(CLDEBUG) printf("core6 JJJ\n");
  if(v_index.size()==0) {
    printf("CLUSTERING FAILED\n");
    return(cx);
  }

#if 0
  printf("Min ICav Index = %d  Min ICav=%g  ncls=%d\n",index_icav,min_icav,v_ncls[index_icav]);
  index = v_index[index_icav];
#else
  printf("Min dICav Index = %d  Min ICav=%g  Max dICav=%g ncls=%d\n",index_dicav-1,min_icav,max_dicav,v_ncls[index_dicav-1]);
  index = v_index[index_dicav-1];
#endif
  
  ncls = ClassificationByIndexWithID(csim,cid,index,idtoclass);
  CopyClassificationWithID(idtoclass,cx,xcls);

  for(i=0;i<consolidate;i++) {
    ClusterConsolidationWithID(cx,cid,csim,xcls,index);
    ncls = ClassificationByIndexWithID(csim,cid,index,idtoclass);
    CopyClassificationWithID(idtoclass,cx,xcls);

    cut = CutWithID(cx,xcls,ncls,idtoclass);
    rcut = RatioCutWithID(cx,xcls,ncls,idtoclass);
    ncut = NcutWithID(cx,xcls,ncls,idtoclass);
    gapratio = GapRatioWithID(cid,csim,index,idtoclass,xcls,ncls);
    icav = ICav.calculate();
    printf("Cut>=%8g RatioCut>=%8g NCut>=%8g Gapratio>=%8g ICav<=%8g ncls=%d\n"
	,cut,rcut,ncut,gapratio,icav,ncls);
  }
  if(reorder) SortDistanceOrder(cx);
  return(cx);
}
////////////////////////////////////////////////////////////////////////

Collection Clustering_core7(Collection& cx,SimilarityFunction simfunc,int clsin,const vector<double>& arg) {
  double limitthresh=2.0;
  int  ncls_uplimit = 100;
  int  ncls_lowlimit = 2;
  int consolidate = (int)0;
  int reorder =0;
  int NEGATIVEDICAV = 0;
  double ratio=-1;
  if(arg.size()>=1) ncls_uplimit  = (int)arg[0];
  if(arg.size()>=2) ncls_lowlimit = (int)arg[1];
  if(arg.size()>=3) limitthresh = arg[2];
  if(arg.size()>=4) consolidate = (int)arg[3];
  if(arg.size()>=5) reorder = (int)arg[4];
  if(arg.size()>=6) ratio= arg[5];
  if(arg.size()>=7) NEGATIVEDICAV = arg[6];
  if(ncls_uplimit>cx.size()) ncls_uplimit=cx.size()-1;
  double initthresh=0.9;
  printf("Clustering_core7 (up%d,low%d,limit%g,consol%d,reorder%d,ratio=%g)\n",ncls_uplimit,ncls_lowlimit,limitthresh,consolidate,reorder,ratio);
  unsigned int i;
  int xcls = cx.getxexact("class");

//if(CLDEBUG) printf("core7 AAA\n");
  // Evaluate Similarity between all samples
  Collection SimilarityMatrix = MakeSimilarityMatrix(cx,simfunc);

//if(CLDEBUG) printf("core7 CCC\n");
  // make Similarity-name table sorted by similarity for each sample and initialize index table
  Collection cid, csim; 
  SortSimilarityMatrixByColumnWithID(SimilarityMatrix,xcls,cid,csim);
if(CLDEBUG) cid.disp("cid.csv","w");
if(CLDEBUG) csim.disp("csim.csv","w");

//if(CLDEBUG) printf("core7 DDD\n");
  // 1 あるSampleからの内積距離順位index初期化
  // set initial index
  vector<int> index; 
  SetInitialIndex(csim,xcls,1,index);
  map<int,int> idtoclass;

//if(CLDEBUG) printf("core7 FFF\n");
  double cut;
  double rcut;
  double ncut;
  double prevgap;
  double gapratio;
  int ncls;
  double icav, dicav;
//if(CLDEBUG) printf("core7 GGGb\n");

  // 2 内積距離同順位統計値との比較と index update
  // get initial statistics
  statistics stall;
  stall = StatisticsAll(csim,index);

//if(CLDEBUG) printf("core7 HHH\n");
  // Classification loop
  int maxiter=100;
  int ndiv=20;
  int count=0;
  double thresh;
  vector<double> v_cut, v_rcut, v_ncut, v_gapratio, v_icav, v_dicav;
  vector<int>    v_ncls;
  vector<vector<int> > v_index;
  double min_gapratio = 1e99,min_icav = 1e99, max_dicav=-1e99;
  int index_dicav;
  map<int,statistics> stc;

  int xcls2=xcls+1;
  int prevncls=0;
  IntraClusterDistance_t ICav(cx,cid,csim,idtoclass,xcls,ncls);
  ncls = ncls_uplimit;
  i=0;
  while(i<maxiter && ncls>=ncls_lowlimit) {
//if(CLDEBUG) printf("core7 HHHb\n");
    thresh = 1.0 - stall.stddev()*limitthresh*i/ndiv;
    printf("mean=%g , stddev=%g  thresh=%g ratio=%g\n"
		,stall.mean(),stall.stddev(),thresh,ratio);

    UpdateIndexByThresh(csim,xcls,thresh,index,0);
    ncls = ClassificationByIndexWithID(csim,cid,index,idtoclass);
    CopyClassificationWithID(idtoclass,cx,xcls);

#if 1
    if(ratio>=0) {
      stc = StatisticsByClassAllWithID(cx,csim,cid,index,idtoclass);
      UpdateIndexByClassNormalizedStatWithID(cid,csim,xcls,stc,idtoclass
					  ,thresh,ratio,index,0);
      ncls = ClassificationByIndexWithID(csim,cid,index,idtoclass);
      CopyClassificationWithID(idtoclass,cx,xcls);
    }
#endif

    if(ncls!=prevncls) {
      ++xcls2;
      CopyClassificationWithID(idtoclass,cx,xcls2,"iter",i,ncls);
      prevncls=ncls;
    }

//if(CLDEBUG) printf("core7 HHHd\n");
    cut = CutWithID(cx,xcls,ncls,idtoclass);
    rcut = RatioCutWithID(cx,xcls,ncls,idtoclass);
    ncut = NcutWithID(cx,xcls,ncls,idtoclass);
    gapratio = GapRatioWithID(cid,csim,index,idtoclass,xcls,ncls);
    icav = ICav.calculate();
    //if(prevgap==gapratio) ++count; else count=0; if(count>1) break;
    prevgap=gapratio;

    v_cut.push_back(cut);
    v_rcut.push_back(rcut);
    v_ncut.push_back(ncut);
    v_gapratio.push_back(gapratio);

    if(!i) v_dicav.push_back(1.0);
    else {
      if(ncls>=ncls_uplimit) index_dicav=i+1;
      if(icav!=0 && v_icav.back()!=0) {
        dicav = (icav-v_icav.back())/icav;
        if(v_ncls.back()>=ncls_lowlimit && ncls_uplimit>=v_ncls.back()) {
          v_dicav.push_back(dicav);
          if(max_dicav<dicav) {
            printf("MAX dICav %g %d\n",dicav,i);
            max_dicav = dicav;
            index_dicav = i;
          }
          else if(NEGATIVEDICAV!=0 && dicav<0 && -dicav*2.0 > max_dicav) {
            printf("MAX dICav %g (%g) %d\n",dicav,-dicav*NEGATIVEDICAV,i+1);
            max_dicav = -dicav*2.0;
            index_dicav = i+1;
          }  
        }  
        else {
          printf("Out of nclk limit, abondaned\n");
        }
      }
    }

    v_icav.push_back(icav);
    v_index.push_back(index);
    v_ncls.push_back(ncls);

    printf("Cut>=%8g RatioCut>=%8g NCut>=%8g Gapratio<=%8g ICav<=%8g dICav>=%8g ncls=%d i=%d\n"
	,cut,rcut,ncut,gapratio,icav,dicav,ncls,i);
    ++i;
//if(CLDEBUG) printf("core7 III\n");
  }

//if(CLDEBUG) printf("core7 JJJ\n");
  if(v_index.size()==0) {
    printf("CLUSTERING FAILED\n");
    return(cx);
  }

#if 0
  printf("Min ICav Index = %d  Min ICav=%g  ncls=%d\n",index_icav,min_icav,v_ncls[index_icav]);
  index = v_index[index_icav];
#else
  printf("Min dICav Index = %d  Min ICav=%g  Max dICav=%g ncls=%d\n",index_dicav-1,min_icav,max_dicav,v_ncls[index_dicav-1]);
  index = v_index[index_dicav-1];
#endif
  
  ncls = ClassificationByIndexWithID(csim,cid,index,idtoclass);
  CopyClassificationWithID(idtoclass,cx,xcls);
  if(CLDEBUG) CopyClassificationWithID(idtoclass,cx,xcls2++,"select",index_dicav-1,ncls);

  for(i=0;i<consolidate;i++) {
    ClusterConsolidationWithID(cx,cid,csim,xcls,index);
    ncls = ClassificationByIndexWithID(csim,cid,index,idtoclass);
    CopyClassificationWithID(idtoclass,cx,xcls);
    if(CLDEBUG) CopyClassificationWithID(idtoclass,cx,xcls2++,"consol",i,ncls);

    cut = CutWithID(cx,xcls,ncls,idtoclass);
    rcut = RatioCutWithID(cx,xcls,ncls,idtoclass);
    ncut = NcutWithID(cx,xcls,ncls,idtoclass);
    gapratio = GapRatioWithID(cid,csim,index,idtoclass,xcls,ncls);
    icav = ICav.calculate();
    printf("Cut>=%8g RatioCut>=%8g NCut>=%8g Gapratio<=%8g ICav<=%8g ncls=%d\n"
	,cut,rcut,ncut,gapratio,icav,ncls);
  }
  if(reorder) SortDistanceOrder(cx);
  return(cx);
}
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
// New Index setting & update
////////////////////////////////////////////////////////////////////////
void InitializeIndex(const Collection& csim,vector<int>& index) {
  unsigned int i,j;
  double prev;
  index.resize(csim.size()+1);
  for(j=1;j<=csim.size();j++) {
    i = 0; prev = 1.0;
    while(csim[i][j].todouble()==prev) ++i;
    index[j]=i-1;
  }
}
////////////////////////////////////////////////////////////////////////
void UpdateIndexByClassificationWithID(const Collection& cid
			,map<int,int>& idtoclass
			,vector<int>& index) {
  unsigned int i,j;
  int id;
  for(j=1;j<=cid.size();j++) {
    i = index[j];
    id = cid[i][j].toint();
    while(i<cid.size() && idtoclass[cid[i][j].toint()]==id) ++i;
    if(i>index[j]) index[j]=i-1;
  }
}
////////////////////////////////////////////////////////////////////////
void UpdateIndexByMergingNextGap(const Collection& csim,vector<int>& index) {
  double mingap = -1e99;
  index.resize(csim.size()+1);
  unsigned int j=0,i;
  vector<int> jmax;
  int ind;
  for(j=1;j<=csim.size();j++) {
    ind = index[j];
    if(ind+1<csim.size()) {
      if(mingap<csim[ind+1][j].todouble()) {
        mingap = csim[ind+1][j].todouble();
	jmax.clear();
        jmax.push_back(j);
      }
      else if(mingap==csim[ind+1][j].todouble()) {
        jmax.push_back(j);
      }
    }
  }
  for(i=0;i<jmax.size();i++) {
    j= jmax[i];
    ind = index[j]+1;
    while(ind<csim.size()&&mingap<=csim[ind][j].todouble()) ++ind;
    if(ind) index[j] = ind-1;
  }
}

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
// Parallel Optimizer Core Algorithm : 
//  Serial+2Parallel result table  ->       sequencing
//
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
int sequence=1;
int SeekMaxParallelism2(Collection& cxin,Collection& rtable,int keepseq=0) {
printf("SeekMaxParallelsm2\n");
//rtable.disp();
  //if(rtable[0][1]=="") return(0);

  if(!keepseq) sequence=1;

  printf("SeekMaxParallelism 3\n");

  char debugfname[1000];
  sprintf(debugfname,"seekmaxpara.csv");
  char *p; while((p=strchr(debugfname,'@'))) *p = '_';
  rtable.disp(debugfname,"w");

  int i,j;
  int x1;
  Collection t = rtable; // copy rtable

  /////////////////////////////////////////////////////////
  int xsequence = cxin.getx("Sequence");
  if(xsequence<0) {
    xsequence = cxin.getkey().size();
    cxin.getkey().let(xsequence,"Sequence");
  }
  int xparaname = 0;

  Collection tv,te,tmp;
  Line key;

  /////////////////////////////////////////////////////////
  // Phase2.1
  // Focus on i-th column, separate the table for lines with value in i-th
  // column (tv) or not (te).
  ////////////////////////////////////////////////////////
  //                               i=1
  //        t                       tv                   te
  // Var,  P1,  P2,  P3,      Var,  P1,  P2,  P3,   Var,  P1,  P2,  P3,
  // P1 , 1.1, 1.1, 1.1,      P1 , 1.1, 1.1, 1.1,
  // P2 , 1.2, 1.2,    ,  =>  P2 , 1.2, 1.2,    ,
  // P3 , 1.3,    , 1.3,      P3 , 1.3,    , 1.3, 
  ////////////////////////////////////////////////////////
  // Phase2.2
  //  Count unmatched cell in tv's corresponding columns.
  // Phase2.3
  //  Sort tv table with count.
  // Phase2.4
  //  Move line with highest count from tv to te.
  //  Go back to Phase2.2 until count in all tv lines become 0
  // Phase2.5
  //  Take tv with count=0 as parallel group and assign sequence number
  // Phase2.6
  //  copy remainig tests (te) to original table (t) 
  // Phase2.7 
  //  Move to next valid column in result table (t) and go back to Phase2.1
  ////////////////////////////////////////////////////////
  //      i=1
  //        tv                             tv                   te
  // Var,  P1,  P2,  P3, count,      Var,  P1,  P2,  P3,   Var,  P1,  P2,  P3,
  // P2 , 1.2, 1.2,    ,     1, =>                         P2 , 1.2, 1.2,    ,
  // P3 , 1.3,    , 1.3,     1,                            P3 , 1.3,    , 1.3, 
  // P1 , 1.1, 1.1, 1.1,     0,     P1 , 1.1, 1.1, 1.1,
  ////////////////////////////////////////////////////////
  i=1;
  while(t.size()) {
    // Phase2.1
    tv = t._empty(i);
    te = t.empty(i);
    key = tv.getkey();
    int xcount = key.size();
    do {
      // Phase2.2
      for(j=0;j<tv.size();j++) {
        x1 = tv.getx(tv[j][0].tostring());
        tv[j].let(xcount,tv.empty(x1).size());
      }
      // Phase2.3
      tv.sort(xcount);
//tv.disp(debugfname,"a");
      // Phase2.4
      if(tv[0][xcount].toint()>0) {
        te.push_back(tv[0]);
        tv.erase(0);
      }
    } while(tv.size() && tv[0][xcount].toint()>0) ;

    tv.disp(debugfname,"a");

    // Phase2.5
    // Assign sequence in the test plan
    string paraname;
    for(j=0;j<tv.size();j++) {
      paraname = tv[j][0].tostring();
      cxin.match(xparaname,paraname).let(xsequence,sequence);
    }

    // Phase2.6
    if(tv.size()) {
      ++sequence;
      t = te;
    }

    // Phase2.7
    i = t.getx(t[0][0].tostring());
  }
  return(sequence);
}
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
Collection ClusterPostProcess1(Collection& cx,int xcls
			,const Collection& csim, const Collection& cid
			,map<int,int>& idtoclass,const vector<int>& index) {
  // make class adjacency matrix
  Collection cu = cx.unique_count(xcls);
  Collection cadjacent = makeMatrix(cu);
  unsigned int i,j,id1,id2,cls1,cls2;
  for(j=1;j<=cx.size();j++) {
    i=index[j]+1;
    id1 = cid[j][0].toint(); // == j-1
    cls1 = idtoclass[id1];
    if(i<cx.size()) {
      id2 = cid[j][i].toint();
      cls2 = idtoclass[id2];
      cadjacent[cls1][cls2+1].add(1);
      cadjacent[cls2][cls1+1].add(1);
    }
  }
if(CLDEBUG) cadjacent.disp("cadj.csv","w");

  // set "" to adjacent categories,  1 to non-adjacent categories
  int thresh=0;
  for(i=0;i<cadjacent.size();i++) {
    for(j=1;j<=cadjacent.size();j++) {
      if(cadjacent[i][j].toint()<=thresh || i+1==j) cadjacent[i].let(j,1);
      else                                          cadjacent[i].let(j,"");
    }
  }
if(CLDEBUG) cadjacent.disp("cadj.csv","a");

  int seq = SeekMaxParallelism2(cx,cadjacent);
  if(seq==cx.size()) {
  }
if(CLDEBUG) cx.disp("cpostproc.csv","w");
  
  return(cx);
}
////////////////////////////////////////////////////////////////////////
Collection ClusterPostProcess1(Collection& cx) {
  Collection csim,cid;
  int xcls = cx.getx("class");
  SortSimilarityMatrixByColumnWithID(cx,xcls,cid,csim);

  map<int,int> idtoclass;
  unsigned int i;
  for(i=0;i<cx.size();i++) idtoclass[i] = cx[i][xcls].toint();

  vector<int> index;
  UpdateIndexByClassificationWithID(cid,idtoclass,index);

  return(ClusterPostProcess1(cx,xcls,csim,cid,idtoclass,index));
}
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
Collection ClusterPostProcess2(Collection& cx,int xcls
			,const Collection& csim, const Collection& cid
			,map<int,int>& idtoclass,const vector<int>& index) {
  const char* nnsample = "NNsample";
  int xsmpl = cx.getx(nnsample);
  if(xsmpl<0) { 
    xsmpl = cx.getkey().size()+1;
    cx.getkey().let(xsmpl,nnsample);
  }
  Collection cu = cx.unique_count(xcls), cc,cxx;
  Collection cs;
  cu.sort(1);
  cu.reverse();
  int ncls = cu.size(), cls, nmem, cls2, id2;
  Line line;
  unsigned int k,i,j;
  statistics st; 
  char buf[80];
  for(k=0;k<ncls;k++) {
    cls = cu[k][0].toint();
    nmem = cu[k][1].toint();
    if(nmem<=1) { // if data sample in this class is one
      for(j=1;j<=csim.size();j++) {
        if(cls==idtoclass[j-1]) {
          for(i=1;i<5 && i<cid.size();i++) { // for 5 neighbering data samples
	    id2 = cid[j][i].toint();
	    cls2 = idtoclass[id2];
	    cc = cu.match(0,cls2);
            if(cc.size() && cc[0][1]>5) { // if the data blongs >5 group
	      cx[id2].let(xsmpl,"Neighber"); 
	      sprintf(buf,"Y%d",cls2);    // mark them Yn for exclusion
	      cx[id2].let(xcls,buf);
            }
          }
        }
      }
    }
    else if(nmem<=5) {
    }
    else { // data samples in this class is greater than 5
      cs.clear();
      for(j=1;j<=csim.size();j++) { // calculate distance statistics
        st.init();
        if(idtoclass[j-1]!=cls) continue;
        for(i=1;i<csim.size();i++) {
	  if(idtoclass[cid[i][j].toint()]==cls) st.add(csim[i][j].todouble());
        }
        line.let(0,j-1);
        line.let(1,st.mean());
        cs.push_back(line);
      }
      cs.sort(1);
      for(i=0;i<cs.size();i++) {
 	if((double)i>(double)cs.size()*0.9) { // samples at >90% border
          sprintf(buf,"Outer%s",cx[cs[i][0].toint()][xsmpl].c_str());
	  cx[cs[i][0].toint()].let(xsmpl,buf);
	  sprintf(buf,"X%d",cls);            // mark them Xn for exclusion
	  cx[cs[i][0].toint()].let(xcls,buf);
        }
 	else if((double)i>(double)cs.size()*0.5) 
              cx[cs[i][0].toint()].let(xsmpl,"Middle");
 	else  cx[cs[i][0].toint()].let(xsmpl,"Inner");
      }
    }
  }
  
  return(cx);
}
////////////////////////////////////////////////////////////////////////
Collection ClusterPostProcess2(Collection& cx) {
  Collection csim,cid;
  int xcls = cx.getx("class");
  SortSimilarityMatrixByColumnWithID(cx,xcls,cid,csim);

  map<int,int> idtoclass;
  unsigned int i;
  for(i=0;i<cx.size();i++) idtoclass[i] = cx[i][xcls].toint();

  vector<int> index;
  UpdateIndexByClassificationWithID(cid,idtoclass,index);

  return(ClusterPostProcess2(cx,xcls,csim,cid,idtoclass,index));
}
////////////////////////////////////////////////////////////////////////
Collection Clustering_core8(Collection& cx,SimilarityFunction simfunc,int clsin,const vector<double>& arg) {
  double limitthresh=2.0;
  int  ncls_uplimit = 100;
  int  ncls_lowlimit = 2;
  int consolidate = (int)0;
  int reorder =0;
  int NEGATIVEDICAV = 0;
  int GAPRATIO = 0;
  int postprocess=1;
  
  if(arg.size()>=1) ncls_uplimit  = (int)arg[0];
  if(arg.size()>=2) ncls_lowlimit = (int)arg[1];
  if(arg.size()>=3) consolidate = (int)arg[2];
  if(arg.size()>=4) reorder = (int)arg[3];
  if(arg.size()>=5) NEGATIVEDICAV = (int)arg[4];
  if(arg.size()>=6) GAPRATIO = (int)arg[5];
  if(arg.size()>=7) postprocess = (int)arg[6];
  if(ncls_uplimit>cx.size()) ncls_uplimit=cx.size()-1;
  double initthresh=0.9;

  printf("Clustering_core8 (up%d,low%d,consol%d,reorder%d,NegdICav%d,GapRatio%d)\n",ncls_uplimit,ncls_lowlimit,consolidate,reorder,NEGATIVEDICAV,GAPRATIO);
  unsigned int i;
  int xcls = cx.getxexact("class");

//if(CLDEBUG) printf("core8 AAA\n");
  // Evaluate Similarity between all samples
  Collection SimilarityMatrix = MakeSimilarityMatrix(cx,simfunc);

//if(CLDEBUG) printf("core8 CCC\n");
  // make Similarity-name table sorted by similarity for each sample and initialize index table
  Collection cid, csim; 
  SortSimilarityMatrixByColumnWithID(SimilarityMatrix,xcls,cid,csim);
  if(CLDEBUG) cid.disp("cid.csv","w");
  if(CLDEBUG) csim.disp("csim.csv","w");

//if(CLDEBUG) printf("core8 DDD\n");
  // 1 あるSampleからの内積距離順位index初期化
  // set initial index
  vector<int> index; 
  InitializeIndex(csim,index);
  map<int,int> idtoclass;

//if(CLDEBUG) printf("core8 FFF\n");
  double cut, rcut, ncut, gapratio, prevgap, icav, dicav, icavgapratio;
  int ncls;

//if(CLDEBUG) printf("core8 HHH\n");
  // Classification loop
  int maxiter=cx.size();
  int count=0;
  vector<double> v_cut, v_rcut, v_ncut, v_gapratio, v_icav, v_dicav;
  vector<int>    v_ncls;
  vector<vector<int> > v_index;
  double min_gapratio = 1e99,min_icav = 1e99, max_dicav=-1e99;
  int index_dicav=-1, index_gapratio=-1;

  int xcls2=xcls+1;
  int prevncls=0;
  IntraClusterDistance_t ICav(cx,cid,csim,idtoclass,xcls,ncls);
  ncls = ncls_uplimit;
  i=0;
  while(i<maxiter && ncls>=ncls_lowlimit) {
//if(CLDEBUG) printf("core8 HHHb\n");
    ncls = ClassificationByIndexWithID(csim,cid,index,idtoclass);
    CopyClassificationWithID(idtoclass,cx,xcls);


//if(CLDEBUG) printf("core8 HHHd\n");
    cut = CutWithID(cx,xcls,ncls,idtoclass);
    rcut = RatioCutWithID(cx,xcls,ncls,idtoclass);
    ncut = NcutWithID(cx,xcls,ncls,idtoclass);
    gapratio = GapRatioWithID(cid,csim,index,idtoclass,xcls,ncls);
    icav = ICav.calculate();
    icavgapratio = ICav.GapRatio();
    prevgap=gapratio;

    v_cut.push_back(cut);
    v_rcut.push_back(rcut);
    v_ncut.push_back(ncut);
    v_gapratio.push_back(gapratio);

    // Minimum gapratio
    if(min_gapratio>gapratio) {
      if(ncls>=ncls_lowlimit && ncls_uplimit>=ncls) {
        printf("MIN GapRatio %g %d (%g %d)\n",gapratio,i,min_gapratio,index_gapratio);
        min_gapratio=gapratio;
        index_gapratio = i;
      }
      else {
        printf("Out of nclk limit %d, abondaned gapratio=%g\n",ncls,gapratio);
      }
    }

    // maximum dICav
    if(!i) v_dicav.push_back(1.0);
    else {
      if(ncls>=ncls_uplimit) index_dicav=i+1;
      if(icav!=0 && v_icav.back()!=0) {
        dicav = (icav-v_icav.back())/icav;
        if(v_ncls.back()>=ncls_lowlimit && ncls_uplimit>=v_ncls.back()) {
          v_dicav.push_back(dicav);
          if(max_dicav<dicav) {
            printf("MAX dICav %g %d (%g %d)\n",dicav,i,max_dicav,index_dicav);
            max_dicav = dicav;
            index_dicav = i;
          }
          else if(NEGATIVEDICAV!=0 && dicav<0 && -dicav*2.0 > max_dicav) {
            printf("MAX dICav %g (%g) %d\n",dicav,-dicav*NEGATIVEDICAV,i+1);
            max_dicav = -dicav*2.0;
            index_dicav = i+1;
          }  
        }  
        else {
          printf("Out of nclk limit %d:%d, abondaned dICav=%g\n",ncls,v_ncls.back(),dicav);
        }
      }
    }

    printf("Cut>=%8g RatioCut>=%8g NCut>=%8g Gapratio<=%8g ICav<=%8g dICav>=%8g Gap=%g ncls=%d i=%d\n"
	,cut,rcut,ncut,gapratio,icav,dicav,icavgapratio,ncls,i);

    v_icav.push_back(icav);
    v_index.push_back(index);
    v_ncls.push_back(ncls);

   if(ncls!=prevncls) {
     ++xcls2;
     CopyClassificationWithID(idtoclass,cx,xcls2,"iter",i,ncls,icav,dicav);
     prevncls=ncls;
   }

//if(CLDEBUG>2) dispindex(index);
    UpdateIndexByClassificationWithID(cid,idtoclass,index);
//if(CLDEBUG>2) dispindex(index);
    UpdateIndexByMergingNextGap(csim,index) ;
//if(CLDEBUG>2) dispindex(index);

//if(CLDEBUG) printf("core8 III\n");
    ++i;
  }

//if(CLDEBUG) printf("core8 JJJ\n");
  if(v_index.size()==0) {
    printf("CLUSTERING FAILED\n");
    return(cx);
  }

  if(GAPRATIO) {
    printf("Min gapratio Index = %d  Min gapratio=%g ncls=%d\n",index_gapratio,min_gapratio,v_ncls[index_gapratio]);
    index = v_index[index_gapratio];
  }
  else {
    printf("Min dICav Index = %d  Min ICav=%g  Max dICav=%g ncls=%d\n",index_dicav-1,min_icav,max_dicav,v_ncls[index_dicav-1]);
    index = v_index[index_dicav-1];
  }
  
  ncls = ClassificationByIndexWithID(csim,cid,index,idtoclass);
  CopyClassificationWithID(idtoclass,cx,xcls);
  CopyClassificationWithID(idtoclass,cx,xcls2++,"select",index_dicav-1,ncls);

  for(i=0;i<consolidate;i++) {
    ClusterConsolidationWithID(cx,cid,csim,xcls,index);
    ncls = ClassificationByIndexWithID(csim,cid,index,idtoclass);
    CopyClassificationWithID(idtoclass,cx,xcls);

    cut = CutWithID(cx,xcls,ncls,idtoclass);
    rcut = RatioCutWithID(cx,xcls,ncls,idtoclass);
    ncut = NcutWithID(cx,xcls,ncls,idtoclass);
    gapratio = GapRatioWithID(cid,csim,index,idtoclass,xcls,ncls);
    icav = ICav.calculate();
    icavgapratio = ICav.GapRatio();

    CopyClassificationWithID(idtoclass,cx,xcls2++,"consol",i,ncls,icav);
    printf("Cut>=%8g RatioCut>=%8g NCut>=%8g Gapratio<=%8g ICav<=%8g Gap=%g ncls=%d\n"
	,cut,rcut,ncut,gapratio,icav,icavgapratio,ncls);
  }

  if(CLDEBUG) {
    Collection cgathered = GatherSortedInfo(csim,cid,idtoclass);
    cgathered.disp("cgathered.csv","w");
  }

  if(postprocess) ClusterPostProcess2(cx,xcls,csim,cid,idtoclass,index);

  EasyExpExtraction(cx,0,xcls,xcls+1);

  if(reorder) SortDistanceOrder(cx);

  return(cx);
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
Collection Clustering(const Collection& ci,ClusteringAlgo algo,SimilarityFunction simfunc,const vector<double>& arg) {
  Collection cx = ci;
  cx = makeMatrix(cx);
  int xcls = cx.getx("class");
  if(xcls<0) cx.getkey().push_back("class");
  cx.setmatchmode(EasyExp::Exact);
  return(algo(cx,simfunc,0,arg));
}

////////////////////////////////////////////////////////////////////////
Collection Clustering1(const Collection& ci,int x,double thresh,double percentage,SimilarityFunction simfunc) {
  ClusteringAlgo algo=Clustering_core1;
  Collection cx ;
  Line line;
  line.let(0,ci.getkey()[x]);
  cx.push_back(line);
  cx.setkey(0); 
  for(unsigned int i=0;i<ci.size();i++) {
    line.let(0,ci[i][x]);
    cx.push_back(line);
  }
  vector<double> arg;
  arg.push_back(thresh);
  arg.push_back(2.0);
  arg.push_back(percentage);
  return(Clustering(cx,algo,simfunc,arg));
}
////////////////////////////////////////////////////////////////////////
static const char* separE=":";
////////////////////////////////////////////////////////////////////////
#if 1
Collection Clustering1(const Collection& ci,const vector<int>& x,double thresh,double percentage,SimilarityFunction simfunc) {
  ClusteringAlgo algo=Clustering_core1;
  Collection cx, ct= ci;
  ct.stat();
  string val;
  char buf[500];
  Line line;
  line.let(0,"vector");
  cx.push_back(line);
  cx.setkey(0); 
  unsigned int i,j,k;
  for(i=0;i<ci.size();i++) {
    val="";
    k=0;
    for(j=0;j<x.size();j++) {
      if(ct.getstat()[x[j]].stddev()==0) continue;
      //sprintf(buf,"%g",ct[i][x[j]].todouble()/ct.getstat()[x[j]].stddev()/6);
      sprintf(buf,"%s",ct[i][x[j]].c_str());
      if(k++) val += separE;
      val += buf;
    }
if(CLDEBUG) printf("i=%d %s\n",i,val.c_str());
    line.let(0,val);
    cx.push_back(line);
  }
  vector<double> arg;
  arg.push_back(thresh);
  arg.push_back(2.0);
  arg.push_back(percentage);
printf("Clustering1 0 AAA\n");
  return(Clustering(cx,algo,simfunc,arg));
}
#else
Collection Clustering1(const Collection& ci,const vector<int>& x,double thresh,SimilarityFunction simfuc) {
  ClusteringAlgo algo=Clustering_core1;
  Collection cx,cv, ct= ci;
  ct.stat();
  vector<vector<double> > dt;
  vector<double> dt1;
  string val;
  char buf[500];
  Line line;
  line.let(0,"vector");
  cx.push_back(line);
  cx.setkey(0); 
  unsigned int i,j,k;
  for(i=0;i<ci.size();i++) {
    val="";
    k=0;
    dt1.clear();
    for(j=0;j<x.size();j++) {
      if(ct.getstat()[x[j]].stddev()==0) continue;
      sprintf(buf,"%s",ct[i][x[j]].c_str());
      if(k++) val += separE;
      val += buf;
      dt1.push_back(ct[i][x[j]].todouble());
    }
if(CLDEBUG) printf("i=%d %s\n",i,val.c_str());
    line.let(0,val);
    cx.push_back(line);
    dt.push_back(dt1);
    line[0].setvoid((void*)(&dt.back()));
    cv.push_back(line);
  }

  vector<double> arg;
  arg.push_back(thresh);
printf("Clustering1 AAA\n");
  cv=  Clustering(cv,algo,simfunc,arg);

printf("Clustering1 BBB\n");
  for(i=0;i<cv.size();i++) cv[i].let(0,cx[i][0]);
printf("Clustering1 CCC\n");
  return(cv);
}
#endif
////////////////////////////////////////////////////////////////////////
Collection Clustering8(const Collection& ci,int x,int nmin,int nmax,SimilarityFunction simfunc) {
  ClusteringAlgo algo=Clustering_core8;
  Collection cx ;
  Line line;
  line.let(0,ci.getkey()[x]);
  cx.push_back(line);
  cx.setkey(0); 
  for(unsigned int i=0;i<ci.size();i++) {
    line.let(0,ci[i][x]);
    cx.push_back(line);
  }
  vector<double> arg;
  arg.push_back(nmax);
  arg.push_back(nmin);
  return(Clustering(cx,algo,simfunc,arg));
}
////////////////////////////////////////////////////////////////////////
Collection Clustering8(const Collection& ci,const vector<int>& x,int lowlim ,int uplim,SimilarityFunction simfunc) {
  ClusteringAlgo algo=Clustering_core8;
  Collection cx, ct= ci;
  ct.stat();
  string val;
  char buf[500];
  Line line;
  line.let(0,"vector");
  cx.push_back(line);
  cx.setkey(0); 
  unsigned int i,j,k;
  for(i=0;i<ci.size();i++) {
    val="";
    k=0;
    for(j=0;j<x.size();j++) {
      if(ct.getstat()[x[j]].stddev()==0) continue;
      //sprintf(buf,"%g",ct[i][x[j]].todouble()/ct.getstat()[x[j]].stddev()/6);
      sprintf(buf,"%s",ct[i][x[j]].c_str());
      if(k++) val += separE;
      val += buf;
    }
if(CLDEBUG) printf("i=%d %s\n",i,val.c_str());
    line.let(0,val);
    cx.push_back(line);
  }
  vector<double> arg;
  arg.push_back(lowlim);
  arg.push_back(uplim);
printf("Clustering8 0 AAA\n");
  return(Clustering(cx,algo,simfunc,arg));
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
// Sparse Sampling algorithm for Neural Network supervising sample selection
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
Collection SparseSampling(const Collection& ci,SimilarityFunction simfunc,int iter) {
  Collection cx = makeMatrix(ci);
  Line line;
  Collection t1,t2,cr;
  int i,j=0;
  cr.push_back(cx[j]);
  t2 = cx;

  unsigned int done=0;
  for(i=0;i<iter&&j>=0;i++) {
    t1 = t2;
    t1.erase(0);
    t2 = MakeSimilarityMatrixColumn(t1,j,simfunc,done);
    t2.sort(j+1);
    t2 = t2._match(j+1,1);
    if(t2.size()) { 
      t2.reverse();
      cr.push_back(t2[0]);
      j=cx.getxexact(t2[0][0].c_str())-1;
    }
    else j= -1;
  }
  return(cr);
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
// Clustering conflict detector for Neural Network supervising sample selection
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
Collection ClusteringConflict_core1(Collection& cx,SimilarityFunction simfunc,int clsin,const vector<double>& arg) {
  double margin=0.9;
  if(arg.size()>=1)  margin = arg[0];
  // Conflict rejecting performance was below expectation.
  int xcls = cx.getxexact("class");
  Collection c1 = MakeSimilarityMatrix(cx,simfunc);
  int i,j,k,xt;
  string cls,cls2,name;
  Collection ct,co,cr;
  Collection cu = c1.unique_count(xcls);
  CollectionStatistics stt;
  double tminmean;
  co.setmatchmode(EasyExp::Exact);
  for(i=0;i<cu.size();i++) {
    // for each class 
    cls = cu[i][0].c_str();       // get class name
    ct =c1.match(xcls,cls);       // collect target class element
    co = c1._match(xcls,cls);     // collect other class element
    stt = ct.stat();  // calculate elemenent distance mean with target class

    // calculate minmean for target category element distance
    tminmean = 1e99; 
    for(j=0;j<ct.size();j++) {
      xt = ct.getxexact(ct[j][0].c_str());
      if(tminmean > stt[xt].mean()) tminmean = stt[xt].mean();
    } // end calculate minmean
printf("%s tminmean=%g\n",cls.c_str(),tminmean);

    // exclude samples from other class if average distance is closer
    for(j=1;j<xcls;j++) {
      name = ct.getkey()[j].c_str();
      cr = co.match(0,name);  // find other category element
      if(cr.size() && tminmean-margin<stt[j].mean()) {
printf(" %s mean=%g-%g<%g %s\n",name.c_str(),tminmean,margin,stt[j].mean(),cr[0][xcls].c_str());
	for(k=0;k<co.size();k++) {
          if(!isalpha(cr[k][xcls].c_str()[0])) {
	    cls2 = string("X_")+co[k][xcls].tostring()+"_"+cls;
            cr[k].let(xcls,cls2.c_str());
          }
        }
      }
    }
  }
  return(cx);
}

////////////////////////////////////////////////////////////////////////
Collection ClusteringConflict_core2(Collection& cx,SimilarityFunction simfunc,int clsin,const vector<double>& arg) {
  double marginmean=0.5;
  double marginmin=2.0;
  if(arg.size()>=1) marginmean = arg[0];
  if(arg.size()>=2) marginmin = arg[1];
  // This algorithm works better than previous one, however, still only exibit
  // marginal performance.
  int xcls = cx.getxexact("class");
  Collection c1 = MakeSimilarityMatrix(cx,simfunc);
  int i,j,xt;
  string cls,cls2,name;
  Collection ct,co,cr;
  Collection cu = c1.unique_count(xcls);
  CollectionStatistics stt;
  double tminmean,tminmin;
  co.setmatchmode(EasyExp::Exact);
  Collection minmeandistance;
  Line line;
  int xcl=0,xmin=1,xmean=2;

  // for each class, calculate average distance min and mean
  for(i=0;i<cu.size();i++) { 
    cls = cu[i][0].c_str();       // get class name
    ct =c1.match(xcls,cls);       // collect target class element
    stt = ct.stat(); //calculate elemenent distance min&mean with target class
    // calculate minmean for target category element distance
    tminmean = 1e99; tminmin  = 1e99; 
    for(j=0;j<ct.size();j++) {
      xt = ct.getxexact(ct[j][0].c_str());
      if(tminmean > stt[xt].mean()) tminmean = stt[xt].mean();
      if(tminmin  > stt[xt].min())  tminmin  = stt[xt].min();
    } // end calculate minmean
    line.let(xcl,cls.c_str());line.let(xmin,tminmin);line.let(xmean,tminmean);
    minmeandistance.push_back(line);
  }
  // sort class with average distance mean
  minmeandistance.sort(xmean); 

  // from class with smaller to larger mean, scan other class samples and
  // reject them if distance is closer than self.
  int flag;
  for(i=0;i<minmeandistance.size();i++) {
    flag=0;
    tminmean = minmeandistance[i][xmean].todouble();
    tminmin  = minmeandistance[i][xmin].todouble();
    cls = minmeandistance[i][xcl].c_str();
    ct = c1.match(xcls,cls);      // collect target class element
    co = c1._match(xcls,cls);     // collect other class element
    stt = ct.stat(); //calculate elemenent distance min&mean with target class
    // calculate minmean for target category element distance
printf("%s tmin %g mean=%g\n",cls.c_str(),tminmin,tminmean);
    for(j=1;j<xcls;j++) {
      name = ct.getkey()[j].c_str();
      cr = co.match(0,name);  // check if this belongs to another category 
      cls2 = cr[0][xcls].c_str();
      if(cr.size() && isdigit(cls2[0])) {
//printf(" %s %s %g-%g %g-%g-%g\n",name.c_str(),cr[0][xcls].c_str(),tminmin,tminmean,stt[j].min(),stt[j].mean(),stt[j].max());
        if(tminmean-marginmean<stt[j].mean()) {
printf(" %s mean-mean=%g-%g<%g %s\n",name.c_str(),tminmean,marginmean,stt[j].mean(),cr[0][xcls].c_str());
	  cls2 = string("X")+cls2+"_"+cls;
          cr[0].let(xcls,cls2.c_str());
	  ++flag;
        }
        else if(tminmean-marginmin<stt[j].max()) {
printf(" %s mean-max=%g-%g<%g %s\n",name.c_str(),tminmean,marginmin,stt[j].max(),cr[0][xcls].c_str());
	  cls2 = string("X")+cls2+"_"+cls;
          cr[0].let(xcls,cls2.c_str());
	  ++flag;
        }
      }
    }
    if(!flag) c1 = c1._match(xcls,cls.c_str());
  }
  return(cx);
}
////////////////////////////////////////////////////////////////////////
Collection ClusteringConflict_core3(Collection& cx,SimilarityFunction simfunc,int clsin,const vector<double>& arg) {
  double marginmean=0.2; // ??
  double marginmin=0.1; // ??
  if(arg.size()>=1) marginmean = arg[0];
  if(arg.size()>=2) marginmin = arg[1];
  unsigned int i,j;
  int cls = clsin;
  int xcls = cx.getxexact("class");
  Collection c1 = MakeSimilarityMatrix(cx,simfunc);

  // make x to y table 
  map<string,string> nametoclass;
  for(i=0;i<c1.size();i++) 
    nametoclass[c1[i][0].tostring()] = c1[i][xcls].tostring();

  // separate collection by xcls column classification
  map<string,Collection> um = c1.unique_map(xcls);

  // make xcls table, calculate statistics
  map<string,Collection>::iterator first = um.begin();
  map<string,Collection>::iterator last  = um.end();
  while(first!=last) {
    (*first).second.stat();
    ++first;
  }

  string name;
  for(j=1;j<xcls;j++) {
    name = c1.getkey()[j].tostring();
    first = um.begin(); last  = um.end();
    while(first!=last) {
//TO BE COMPLETED
      (*first).second.getstat()[j].min();
      (*first).second.sort(j);
      ++first;
    }
  }
  return(cx);
}
////////////////////////////////////////////////////////////////////////
Collection ClusteringConflict(const Collection& ci,ClusteringAlgo algo,SimilarityFunction simfunc,const vector<double>& arg) {
  Collection cii = ci;
  Collection cx = ci;
  cx = makeMatrix(cx);
  int xcls = cii.getx("class");
  if(xcls>=0) cx.column_push_back(cii.column(xcls));
  else        cx.getkey().push_back("class");
  cx.setmatchmode(EasyExp::Exact);
  return(algo(cx,simfunc,0,arg));
}

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
// General adapter
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////
// interpreted function stub
////////////////////////////////////////////////////////////////////////////
#if 0
#ifndef __CINT__
#include "G__ci.h"
#endif
//struct G__value;
extern double G__double (G__value buf);
extern G__value G__calc (const char *expr);
char* InterpretedMatchFunc;
double interpretmatchratio(const char* a,const char* b) {
  char buf[500];
  sprintf(buf,"%s((char*)%p,(char*)%p)",InterpretedMatchFunc,a,b);
  double r = G__double( G__calc(buf) );
  return(r);
}
#endif

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
// combination
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
CombinationMatch_t CombinationMatch;

double combinationmatchratio(const char* a,const char* b) {
  double ra = CombinationMatch.fa(a,b);
  double rb = CombinationMatch.fb(a,b);
  double r = (ra*CombinationMatch.ratioa + rb*CombinationMatch.ratiob) 
	     / (CombinationMatch.ratioa+CombinationMatch.ratiob);
  return(r);
}


////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
// Character comparison
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
// character matching 
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
int charmatchcount(const char* ain,const char* bin) {
  string a=ain;
  string b=bin;
  int i=0, count=0 ;
  while(i<a.size() && i<b.size()) {
    if(a[i]==b[i]) ++count;
    ++i;
  }
  return(count);
}

////////////////////////////////////////////////////////////////////////////
double charmatchratio(const char* ain,const char* bin) {
  string a=ain;
  string b=bin;
  if(a.size()==0 && b.size()==0) return(1.0);
  int len = max(a.size(),b.size());
  return((double)charmatchcount(ain,bin)/len);
}

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
// head matching 
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
int headmatchcount(const char* ain,const char* bin) {
  string a=ain;
  string b=bin;
  int i=0, count=0 ;
  while(i<a.size() && i<b.size()) {
    if(a[i]==b[i]) ++count;
    else break;
    ++i;
  }
  return(count);
}

////////////////////////////////////////////////////////////////////////////
double headmatchratio(const char* ain,const char* bin) {
  string a=ain;
  string b=bin;
  if(a.size()==0 && b.size()==0) return(1.0);
  int len = max(a.size(),b.size());
  return((double)headmatchcount(ain,bin)/len);
}

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
// word matching
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
static const char* separ=" \t~!@#$%^&*()-=_+/;:<>,.\\|[]{}'\"`";
double wordmatchcount(const char* ain,const char* bin) {
  string a=ain;
  string b=bin;
  ReadF parsea; parsea.setseparator(separ);
  parsea.parse(a.c_str());
  ReadF parseb; parsea.setseparator(separ);
  parseb.parse(b.c_str());
  int i,counta=0;
  for(i=1;i<=parsea.argc;i++) {
    if(strstr(b.c_str(),parsea.argv[i])) ++counta;
  }
  int countb=0;
  for(i=1;i<=parseb.argc;i++) {
    if(strstr(a.c_str(),parseb.argv[i])) ++countb;
  }
  return((double)(counta+countb)/2);
}

////////////////////////////////////////////////////////////////////////////
double wordmatchratio(const char* ain,const char* bin) {
  string a=ain;
  string b=bin;
  if(a==b) return(1.0);
  ReadF parsea; parsea.setseparator(separ);
  parsea.parse(a.c_str());
  ReadF parseb; parsea.setseparator(separ);
  parseb.parse(b.c_str());
  int i,counta=0;
  for(i=1;i<=parsea.argc;i++) {
    if(strstr(b.c_str(),parsea.argv[i])) ++counta;
  }
  int countb=0;
  for(i=1;i<=parseb.argc;i++) {
    if(strstr(a.c_str(),parseb.argv[i])) ++countb;
  }
  int wc = max(parsea.argc,parseb.argc);
  return((double)(counta+countb)/2/wc);
}

////////////////////////////////////////////////////////////////////////////
#include <math.h>
////////////////////////////////////////////////////////////////////////////
#if 1
double EuclideanDistanceRatio(const char* ain,const char* bin) {
  string a=ain;
  string b=bin;
  if(a==b) return(1.0);
  ReadF parsea; parsea.setseparator(separE);
  parsea.parse(a.c_str());
  ReadF parseb; parseb.setseparator(separE);
  parseb.parse(b.c_str());
  int i,counta=0;
  double vala,valb,sum2=0,eucd;
//parsea.disp();
//parseb.disp();
//printf("a=%s:%d b=%s:%d ",ain,parsea.argc,bin,parseb.argc);
  for(i=1;i<=parsea.argc && i<=parseb.argc;i++) {
    vala = atof(parsea.argv[i]);
    valb = atof(parseb.argv[i]);
    sum2 += (vala-valb)*(vala-valb);
//printf("i=%d %g %g %g ",i,vala,valb,sum2);
  }
//printf("\n");
  eucd = sqrt(sum2);
  return(1.0-eucd);
}
#else
double EuclideanDistanceRatio(const char* ain,const char* bin) {
  vector<double>* pa =  (vector<double>*)ain;
  vector<double> &a = *pa;
  vector<double>* pb =  (vector<double>*)bin;
  vector<double> &b = *pb;
  int i,n = min(a.size(),b.size());
  double tmp,sum2;
  for(i=0;i<n;i++) {
    tmp += a[i]-b[i];
    sum2 = tmp*tmp;
  }
  double eucd = sqrt(sum2);
  return(1.0-eucd);
}
#endif


////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
// Regexp extraction
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////
// Regular Expression Exgractor main
////////////////////////////////////////////////////////////////////////////
void EasyExpExtraction(Collection& cx,int x,int xcls,int xregex,int mode,int LIST_LIMIT) {
  Collection cu = cx.unique_count(xcls);
  Collection c,_c;
  vector<string> a,_a;
  string s;
  if(cx.p_key) cx.getkey().let(xregex,"regex");
  unsigned int j;
  for(unsigned int i=0;i<cu.size();i++) {
    // for each cluster, extract regexp from title string
    c = cx.match(xcls,cu[i][0].tostring());
    _c = cx._match(xcls,cu[i][0].tostring());
    if(CLDEBUG) printf("%d/%d '%s' %d:%d '%s'\n",i,cu.size(),cu[i][0].c_str()
		       ,c.size(),_c.size(),c[0][x].c_str());
    a.clear(); _a.clear();
    for(j=0;j<c.size();j++) a.push_back(c[j][x].tostring());
    for(j=0;j<_c.size();j++) _a.push_back(_c[j][x].tostring());
    s=EasyExpExtraction(a,_a,mode,LIST_LIMIT);
    c[0].let(xregex,s);
  }
if(CLDEBUG) printf("\n");
}

////////////////////////////////////////////////////////////////////////////
// Regular Expression Exgractor main
////////////////////////////////////////////////////////////////////////////
void StatEasyExpExtraction(Collection& cx,int x,int xcls,int xregex,int mode,int LIST_LIMIT) {
  Collection cu = cx.unique_count(xcls);
  Collection c,_c;
  vector<string> a,_a;
  string s;
  if(cx.p_key) cx.getkey().let(xregex,"regex");
  unsigned int j;
  for(unsigned int i=0;i<cu.size();i++) {
if(CLDEBUG) printf("%d/%d ",i,cu.size());
    // for each cluster, extract regexp from title string
    c = cx.match(xcls,cu[i][0].tostring());
    _c = cx._match(xcls,cu[i][0].tostring());
    if(CLDEBUG) printf("%d/%d '%s' %d:%d  ",i,cu.size(),cu[i][0].c_str()
		       ,c.size(),_c.size());
    a.clear(); _a.clear();
    for(j=0;j<c.size();j++) a.push_back(c[j][x].tostring());
    for(j=0;j<_c.size();j++) _a.push_back(_c[j][x].tostring());
    s=StatEasyExpExtraction(a,_a,mode,LIST_LIMIT);
    c[0].let(xregex,s);
  }
if(CLDEBUG) printf("\n");
}


////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

void disp(const vector<string>& s) {
  vector<string>::const_iterator first = s.begin();
  vector<string>::const_iterator last  = s.end();
  while(first!=last) {
    printf("%s\n",(*first).c_str());
    ++first;
  }
}

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
// Usable interface to string clustering
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
Collection StringClustering(Collection& c,int x,double thresh,const char* mode,int regexpmode,int regexpLimit) { 
  // prepare target string table t
  Collection t;
  int i;
  Line line;
  line.let(0,"string");
  t.push_back(line);
  t.setkey(0);
  for(i=0;i<c.size();i++) {
    line.let(0,c[i][x].tostring());
    t.push_back(line);
  }

  // set parameter and run clustering algorithm
  SimilarityFunction simfunc = wordmatchratio;
  if(strcmp(mode,"WORD")==0) simfunc = wordmatchratio;
  else if(strcmp(mode,"HEAD")==0) simfunc = headmatchratio;
  else if(strcmp(mode,"CHAR")==0) simfunc = charmatchratio;
  vector<double> arg;
  arg.push_back(thresh);
  arg.push_back(2.0);
  if(thresh>1.5) arg.push_back(thresh); // percentage
  Collection c1 = Clustering(t,Clustering_core1,simfunc,arg);

  // copy cluster information to the original class
  t = c;
  int xcls = c.getx("class");
  if(xcls<0 && c.getkey().size()) {
    xcls = c.getkey().size();
    c.getkey().let(xcls,"class");
  }
  int xcls1 = c1.getx("class");
  // Is it really OK to modify the original???
  if(xcls>=0) for(i=0;i<t.size();i++) c[i].let(xcls,c1[i][xcls1].tostring());


  Collection t2;
  line.let(0,"string");
  line.let(1,"regex");
  line.let(2,"class");
  line.let(3,"count");
  t2.push_back(line);
  t2.setkey(0);
  line.let(1,"");
  line.let(3,"");
  for(i=0;i<c1.size();i++) {
    line.let(0,c1[i][0]);
    line.let(2,c1[i][xcls1]);
    t2.push_back(line);
  }

  // Regular Expression (EasyExp) Synthesis
  switch(regexpmode) {
  case 0:
  case 1:
    EasyExpExtraction(t2,0 /*string*/ ,2 /*class*/ ,1 /*regexp*/
		      ,regexpmode,regexpLimit);
    break;
  case 2:
  case 3:
    StatEasyExpExtraction(t2,0 /*string*/ ,2 /*class*/ ,1 /*regexp*/
		         ,regexpmode-2,regexpLimit);
    break;
  default: 
    break;
  }

  if(0<=regexpmode && regexpmode<4) {
    t2 = t2._empty(1);
    Collection tu = c1.unique_count(xcls1);
    for(i=0;i<tu.size();i++) {
       t = t2.match(2,tu[i][0].tostring());
       t.let(3,tu[i][1].tostring());
    }
  }

  return(t2);
}
////////////////////////////////////////////////////////////////////////////
