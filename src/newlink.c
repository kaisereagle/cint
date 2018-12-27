/* /% C %/ */
/***********************************************************************
 * cint (C/C++ interpreter)
 ************************************************************************
 * Source file newlink.c
 ************************************************************************
 * Description:
 *  New style compiled object linkage
 ************************************************************************
 * Copyright(c) 1995~2004  Masaharu Goto 
 *
 * Permission to use, copy, modify and distribute this software and its 
 * documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  The author makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 ************************************************************************/

/* #define G__OLDIMPLEMENTATION2047 */
#define G__OLDIMPLEMENTATION2044 /* Problem with t980.cxx */


#include "common.h"
#include "dllrev.h"
#ifndef G__TESTMAIN
#include <sys/stat.h>
#endif
#ifndef __VC13__
//#include "unistd.h"
#endif

#ifdef _WIN32
#include "windows.h"
#include <errno.h>
FILE *FOpenAndSleep(const char *filename, const char *mode) {
   int tries=0;
   FILE *ret=0;
   while (!ret && ++tries<51)
      if (!(ret=fopen(filename, mode)) && tries<50)
         if (errno!=EACCES && errno!=EEXIST) return 0;
         else Sleep(200);
   if (tries>1)  printf("fopen slept for %g seconds until it succeeeded.\n", (tries-1)/5.);
   return ret;
}

# ifdef fopen
#  undef fopen
# endif
# define fopen(A,B) FOpenAndSleep((A),(B))
#endif
 

#define G__OLDIMPLEMENTATION1702
#define G__OLDIMPLEMENTATION1714

/* This is a very complicated decision. The change 1714 avoids compiled stub
 * function registeration to the dictionary. From the interpreter, only 
 * interpreted stub function should be visible. If there is a 
 */
#undef G__OLDIMPLEMENTATION1714

#define G__OLDIMPLEMENTATION1336

#define G__PROTECTEDACCESS   1
#define G__PRIVATEACCESS     2
static int G__privateaccess = 0;

/**************************************************************************
* CAUTION:
*  Following macro G__BUILTIN must not be defined at normal cint
* installation. This macro must be deleted only when you generate following
* source files. 
*     src/libstrm.cxx  in lib/stream    'make'
*     src/gcc3strm.cxx in lib/gcc3strm  'make'
*     src/iccstrm.cxx  in lib/iccstrm   'make'
*     src/vcstrm.cxx   in lib/vcstream  'make' , 'make -f Makefileold'
*     src/vc7strm.cxx  in lib/vc7stream 'make' 
*     src/bcstrm.cxx   in lib/bcstream  'make'
*     src/cbstrm.cpp   in lib/cbstream  'make'
*     src/sunstrm.cxx  in lib/snstream  'make'
*     src/kccstrm.cxx  (lib/kcc_work not included in the package)
*     src/stdstrct.c  in lib/stdstrct  'make'
*     src/Apiif.cxx   in src           'make -f Makeapi' , 'make -f Makeapiold'
* g++ has a bug of distinguishing 'static operator delete(void* p)' in
* different file scope. Deleting this macro will avoid this problem.
**************************************************************************/
/* #define G__BUILTIN */

#if !defined(G__DECCXX) && !defined(G__BUILTIN) && !defined(__hpux)
#define G__DEFAULTASSIGNOPR
#endif

#if !defined(G__DECCXX) && !defined(G__BUILTIN)
#define G__N_EXPLICITDESTRUCTOR
#endif

#ifndef G__N_EXPLICITDESTRUCTOR
#ifdef G__P2FCAST
#undef G__P2FCAST
#endif
#ifdef G__P2FDECL
#undef G__P2FDECL
#endif
#endif


/**************************************************************************
* If this is Windows-NT/95, create G__PROJNAME.DEF file
**************************************************************************/
#if  defined(G__WIN32) && !defined(G__BORLAND)
#define G__GENWINDEF
#endif




/**************************************************************************
* Following static variables must be protected by semaphoe for
* multi-threading.
**************************************************************************/
static struct G__ifunc_table *G__incset_p_ifunc;
static int G__incset_tagnum;
static int G__incset_func_now;
static int G__incset_func_page;
static struct G__var_array *G__incset_p_local;
static int G__incset_def_struct_member;
static int G__incset_tagdefining;
static int G__incset_def_tagnum;
static long G__incset_globalvarpointer;
static int G__incset_var_type;
static int G__incset_typenum;
static int G__incset_static_alloc;
static int G__incset_access;
static int G__suppress_methods = 0;
static int G__nestedclass = 0;
static int G__nestedtypedef = 0;
static int G__store_asm_noverflow;
static int G__store_no_exec_compile;
static int G__store_asm_exec;
static int G__extra_inc_n = 0;
static char** G__extra_include = 0; /*  [G__MAXFILENAME] = NULL;  */

/**************************************************************************
* G__CurrentCall
**************************************************************************/
static int   s_CurrentCallType = 0;
static void* s_CurrentCall  = 0;
static int   s_CurrentIndex = 0;
void G__CurrentCall(int call_type, void* call_ifunc, int* ifunc_idx)
{
  switch( call_type )   {
  case G__NOP:
    s_CurrentCallType = call_type;
    s_CurrentCall     = 0;
    s_CurrentIndex    = -1;
    break;
  case G__SETMEMFUNCENV:
    s_CurrentCallType = call_type;
    s_CurrentCall     = call_ifunc;
    s_CurrentIndex    = *ifunc_idx;
    break;
  case G__DELETEFREE:
    s_CurrentCallType = call_type;
    s_CurrentCall     = call_ifunc;
    s_CurrentIndex    = *ifunc_idx;
    break;
  case G__RECMEMFUNCENV:
    if ( call_ifunc) *(void**)call_ifunc = s_CurrentCall;
    if ( ifunc_idx)  *ifunc_idx = s_CurrentIndex;
    break;
  case G__RETURN:
    if ( call_ifunc) *(void**)call_ifunc = 0;
    if ( ifunc_idx)  *ifunc_idx  = s_CurrentCallType;
    break;
  }
}


/**************************************************************************
* Checking private constructor
**************************************************************************/
#define G__CTORDTOR_UNINITIALIZED     0x00000000
#define G__CTORDTOR_PRIVATECTOR       0x00000001
#define G__CTORDTOR_NOPRIVATECTOR     0x00000002
#define G__CTORDTOR_PRIVATECOPYCTOR   0x00000010
#define G__CTORDTOR_NOPRIVATECOPYCTOR 0x00000020
#define G__CTORDTOR_PRIVATEDTOR       0x00000100
#define G__CTORDTOR_NOPRIVATEDTOR     0x00000200
#define G__CTORDTOR_PRIVATEASSIGN     0x00001000
#define G__CTORDTOR_NOPRIVATEASSIGN   0x00002000
static int* G__ctordtor_status;


/**************************************************************************
* G__cpplink file name
**************************************************************************/
static char *G__CPPLINK_H;
static char *G__CPPLINK_C;

static char *G__CLINK_H;
static char *G__CLINK_C;

#ifdef G__GENWINDEF
static char *G__WINDEF;
static int G__nexports = 0;
static FILE* G__WINDEFfp = (FILE*)NULL;
static int G__isDLL=0;
static char G__CINTLIBNAME[10] = "LIBCINT";
#endif

#define G__MAXDLLNAMEBUF 512

static char G__PROJNAME[G__MAXNAME];
static char G__DLLID[G__MAXDLLNAMEBUF];
static char *G__INITFUNC;

static char G__NEWID[G__MAXDLLNAMEBUF];

#ifdef G__BORLANDCC5
int G__debug_compiledfunc_arg(FILE *fout,struct G__ifunc_table *ifunc,int ifn,struct G__param *libp);
static void G__ctordtor_initialize(void);
static void G__fileerror(char* fname);
static void G__ctordtor_destruct(void);
void G__gen_newdelete(FILE *fp);
void G__cpplink_protected_stub(FILE *fp,FILE *hfp);
void G__gen_cppheader(char *headerfilein);
static void G__gen_headermessage(FILE *fp,char *fname);
void G__add_macro(char *macroin);
int G__isnonpublicnew(int tagnum);
void  G__if_ary_union_reset(int ifn,struct G__ifunc_table *ifunc);
static int G__isprotecteddestructoronelevel(int tagnum);
void  G__if_ary_union(FILE *fp,int ifn,struct G__ifunc_table *ifunc);
char *G__mark_linked_tagnum(int tagnum);
static int G__isprivateconstructorifunc(int tagnum,int iscopy);
static int G__isprivateconstructorvar(int tagnum,int iscopy);
static int G__isprivatedestructorifunc(int tagnum);
static int G__isprivatedestructorvar(int tagnum);
static int G__isprivateassignoprifunc(int tagnum);
static int G__isprivateassignoprvar(int tagnum);
void G__cppif_gendefault(FILE *fp,FILE *hfp,int tagnum,int ifn,struct G__ifunc_table *ifunc,int isconstructor,int iscopyconstructor,int isdestructor,int isassignmentoperator,int isnonpublicnew);
static char* G__vbo_funcname(int tagnum,int basetagnum,int basen);
static int G__hascompiledoriginalbase(int tagnum);
static void G__declaretruep2f(FILE *fp,struct G__ifunc_table *ifunc,int j);
static void G__printtruep2f(FILE *fp,struct G__ifunc_table *ifunc,int j);
int G__tagtable_setup(int tagnum,int size,int cpplink,int isabstract,char *comment,G__incsetup setup_memvar,G__incsetup setup_memfunc);
int G__tag_memfunc_setup(int tagnum);
int G__memfunc_setup(char *funcname,int hash,int (*funcp)(),int type,int tagnum,int typenum,int reftype,int para_nu,int ansi,int accessin,int isconst,char *paras,char *comment
#ifdef G__TRUEP2F
                     ,void* truep2f, int isvirtual
#endif
);
int G__memfunc_next(void);
static void G__pragmalinkenum(int tagnum,int globalcomp);
void G__incsetup_memvar(int tagnum);
void G__incsetup_memfunc(int tagnum);
#endif


int FileWaitTime(int n) {
#if 1
  //printf("FilewaitTime(%d)\n",n);
#ifndef __VC13__
  //usleep(n); //TODO X64
#endif
  return(n);
#else
  int i,out=0;
  char buf[100];
  for(i=0;i<n;i++) {
    sprintf(buf,"%d",i);
    out+=strlen(buf);
  }
  return(out);
#endif
}

/**************************************************************************
* G__check_setup_version()
*
*  Verify CINT and DLL version
**************************************************************************/
extern char *G__cint_version();

void G__check_setup_version(version, func)
int version;
char *func;
{
   G__init_globals();
   if (version > G__ACCEPTDLLREV_UPTO || version < G__ACCEPTDLLREV_FROM) {
      fprintf(G__sout,"\n\
!!!!!!!!!!!!!!   W A R N I N G    !!!!!!!!!!!!!\n\n\
The internal data structures have been changed.\n\
Please regenerate and recompile your dictionary which\n\
contains the definition \"%s\"\n\
using CINT version %s.\n\
your dictionary=%d. This version accepts=%d-%d\n\
and creates %d\n\n\
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n\n",
            func, G__cint_version(),version
	      ,G__ACCEPTDLLREV_FROM
	      ,G__ACCEPTDLLREV_UPTO
	      ,G__CREATEDLLREV
	      );
      exit(1);
   }
   G__store_asm_noverflow = G__asm_noverflow;
   G__store_no_exec_compile = G__no_exec_compile;
   G__store_asm_exec = G__asm_exec;
   G__abortbytecode();
   G__no_exec_compile =0;
   G__asm_exec = 0;
}

/**************************************************************************
* G__fileerror()
**************************************************************************/
static void G__fileerror(fname)
char* fname;
{
  char *buf = malloc(strlen(fname)+80);
  sprintf(buf,"Error opening %s",fname);
  perror(buf);
  exit(2);
}

/**************************************************************************
* G__fulltypename
**************************************************************************/
char* G__fulltypename(typenum)
int typenum;
{
  static char buf[G__LONGLINE];
  if(-1==typenum) return("");
  if(-1==G__newtype.parent_tagnum[typenum]) return(G__newtype.name[typenum]);
  else {
    strcpy(buf,G__fulltagname(G__newtype.parent_tagnum[typenum],0));
    strcat(buf,"::");
    strcat(buf,G__newtype.name[typenum]);
    return(buf);
  }
}

/**************************************************************************
* G__debug_compiledfunc_arg(ifunc,ifn,libp);
*
*  Show compiled function call parameters
**************************************************************************/
int G__debug_compiledfunc_arg(fout,ifunc,ifn,libp)
FILE *fout;
struct G__ifunc_table *ifunc;
int ifn;
struct G__param *libp;
{
  char temp[G__ONELINE];
  int i;
  fprintf(fout,"\n!!!Calling compiled function %s()\n",ifunc->funcname[ifn]);
  G__in_pause=1;
  for(i=0;i<libp->paran;i++) {
    G__valuemonitor(libp->para[i],temp);
    fprintf(fout,"  arg%d = %s\n",i+1,temp);
#ifdef G__NEVER
    if('u'==libp->para[i].type && -1!=libp->para[i].tagnum &&
       libp->para[i].obj.i) {
       G__varmonitor(fout,G__struct.memvar[libp->para[i].tagnum],""
		     ,"    ",libp->para[i].obj.i);
    }
#endif
  }
  G__in_pause=0;
  return(G__pause());
}

/**************************************************************************
**************************************************************************
* Calling C++ compiled function
**************************************************************************
**************************************************************************/


/**************************************************************************
* G__call_cppfunc()
**************************************************************************/
int G__call_cppfunc(result7,libp,ifunc,ifn)
G__value *result7;
struct G__param *libp;
struct G__ifunc_table *ifunc;
int ifn;
{
  int (*cppfunc)();
  int result;

  cppfunc = (int (*)())ifunc->pentry[ifn]->p;

#ifdef G__ASM
  if(G__asm_noverflow) {
    /****************************************
     * LD_FUNC (C++ compiled)
     ****************************************/
    if(cppfunc==G__DLL_direct_globalfunc) {
#ifdef G__ASM_DBG
      if(G__asm_dbg) G__fprinterr(G__serr,
				  "%3x: LD_FUNC direct global function %s paran=%d\n"
				  ,G__asm_cp,ifunc->funcname[ifn],libp->paran);
#endif
      G__asm_inst[G__asm_cp]=G__LD_FUNC;
      G__asm_inst[G__asm_cp+1] = (long)ifunc;
      G__asm_inst[G__asm_cp+2]= ifn;
      G__asm_inst[G__asm_cp+3]=libp->paran;
      G__asm_inst[G__asm_cp+4]=(long)cppfunc;
      G__inc_cp_asm(5,0);
    }
    else {
#ifdef G__ASM_DBG
      if(G__asm_dbg) G__fprinterr(G__serr,
				  "%3x: LD_FUNC C++ compiled %s paran=%d\n"
				  ,G__asm_cp,ifunc->funcname[ifn],libp->paran);
#endif
      G__asm_inst[G__asm_cp]=G__LD_FUNC;
      G__asm_inst[G__asm_cp+1] = ifunc->p_tagtable[ifn];
      G__asm_inst[G__asm_cp+2]= - ifunc->type[ifn];
      G__asm_inst[G__asm_cp+3]=libp->paran;
      G__asm_inst[G__asm_cp+4]=(long)cppfunc;
      G__inc_cp_asm(5,0);
    }
  }
#endif /* of G__ASM */

  /* compact G__cpplink.C */
  *result7 = G__null;
  result7->tagnum = ifunc->p_tagtable[ifn];
  result7->typenum = ifunc->p_typetable[ifn];
#ifndef G__OLDIMPLEMENTATION1259
  result7->isconst = ifunc->isconst[ifn];
#endif
  if(-1!=result7->tagnum&&'e'!=G__struct.type[result7->tagnum]) {
    if(isupper(ifunc->type[ifn])) result7->type='U';
    else                          result7->type='u';
  }
  else
    result7->type = ifunc->type[ifn];

#ifdef G__ASM
  if(G__no_exec_compile) {
    if(isupper(ifunc->type[ifn])) result7->obj.i=G__PVOID;
    else                          result7->obj.i=0;
    result7->ref = ifunc->reftype[ifn];
    if('u'==ifunc->type[ifn]&&0==result7->ref&&-1!=result7->tagnum) {
      G__store_tempobject(*result7); /* To free tempobject in pcode */
    }
    if('u'==result7->type&&-1!=result7->tagnum) {
      result7->ref = 1;
      result7->obj.i=1;
    }
    return(1);
  }
#endif

  /* show function arguments when step into mode */
  if(G__breaksignal) {
    if(G__PAUSE_IGNORE==G__debug_compiledfunc_arg(G__sout,ifunc,ifn,libp)) {
      return(0);
    }
  }

  if('~'==ifunc->funcname[ifn][0] && 1==G__store_struct_offset &&
     -1!=ifunc->tagnum && 0==ifunc->staticalloc[ifn]) {
    /* Object is constructed when 1==G__no_exec_compile at loop compilation
     * and destructed at 0==G__no_exec_compile at 2nd iteration. 
     * G__store_struct_offset is set to 1. Need to avoid calling destructor. */
    return(1);
  }

  {
    int store_asm_noverflow = G__asm_noverflow;
    G__suspendbytecode();

    G__CurrentCall(G__SETMEMFUNCENV, ifunc, &ifn);
#ifdef G__EXCEPTIONWRAPPER
    G__ExceptionWrapper(cppfunc,result7,(char*)ifunc,libp,ifn);
#else
    (*cppfunc)(result7,(char*)ifunc,libp,ifn);
#endif
    G__CurrentCall(G__NOP, 0, 0);
    result = 1;

  if(isupper(ifunc->type[ifn])) 
    result7->obj.reftype.reftype=ifunc->reftype[ifn];

    G__asm_noverflow = store_asm_noverflow;
  }
  return(result);
}

/**************************************************************************
* G__ctordtor_initialize()
**************************************************************************/
static void G__ctordtor_initialize()
{
  int i;
  G__ctordtor_status=(int*)malloc(sizeof(int)*(G__struct.alltag+1));
  for(i=0;i<G__struct.alltag+1;i++) {
    /* If link for this class is turned off but one or more member functions
     * are explicitly turned on, set G__ONLYMETHODLINK flag for the class */
    struct G__ifunc_table *ifunc=G__struct.memfunc[i];
    int ifn;
    if(G__NOLINK==G__struct.globalcomp[i]) {
      while(ifunc) {
	for(ifn=0;ifn<ifunc->allifunc;ifn++) {
	  if(G__METHODLINK==ifunc->globalcomp[ifn]) {
	    G__struct.globalcomp[i] = G__ONLYMETHODLINK;
	  }
	}
	ifunc=ifunc->next;
      }
    }
    G__ctordtor_status[i]=G__CTORDTOR_UNINITIALIZED;
  }
}
/**************************************************************************
* G__ctordtor_destruct()
**************************************************************************/
static void G__ctordtor_destruct()
{
  if(G__ctordtor_status) free(G__ctordtor_status);
}


#ifdef G__SMALLOBJECT

void G__gen_clink() {}
void G__gen_cpplink() {}

#else

/**************************************************************************
**************************************************************************
* Function to generate C interface routine G__clink.C
**************************************************************************
**************************************************************************/

/**************************************************************************
* G__gen_clink()
*
*  Generate C++ interface routine source file.
*
**************************************************************************/
void G__gen_clink()
{
/*
 *   include header files
 *   struct G__tagtable G__struct;
 *   struct G__typedef G__newtype;
 *   struct G__var_array G__struct.*memvar;
 *   struct G__var_array G__global;
 *   struct G__ifunc_table G__ifunc;
 */
  FILE *fp;
  FILE *hfp;

  G__ctordtor_initialize();

  fp = fopen(G__CLINK_C,"a");
  if(!fp) G__fileerror(G__CLINK_C);
  fprintf(fp,"  G__c_reset_tagtable%s();\n",G__DLLID);
  fprintf(fp,"}\n");

  hfp = fopen(G__CLINK_H,"a");
  if(!hfp) G__fileerror(G__CLINK_H);

#ifdef G__BUILTIN
  fprintf(fp,"#include \"dllrev.h\"\n");
  fprintf(fp,"int G__c_dllrev%s() { return(G__CREATEDLLREV); }\n",G__DLLID);
#else
  fprintf(fp,"int G__c_dllrev%s() { return(%d); }\n",G__DLLID,G__CREATEDLLREV);
#endif

  G__cppif_func(fp,hfp);
  G__cppstub_func(fp);

  G__cpplink_typetable(fp,hfp);
  G__cpplink_memvar(fp);
  G__cpplink_global(fp);
  G__cpplink_func(fp);
  G__cpplink_tagtable(fp,hfp);
  fprintf(fp,"void G__c_setup%s() {\n",G__DLLID);
#ifdef G__BUILTIN
  fprintf(fp,"  G__check_setup_version(G__CREATEDLLREV,\"G__c_setup%s()\");\n",
          G__DLLID);
#else
  fprintf(fp,"  G__check_setup_version(%d,\"G__c_setup%s()\");\n",
          G__CREATEDLLREV,G__DLLID);
#endif
  fprintf(fp,"  G__set_c_environment%s();\n",G__DLLID);
  fprintf(fp,"  G__c_setup_tagtable%s();\n\n",G__DLLID);
  fprintf(fp,"  G__c_setup_typetable%s();\n\n",G__DLLID);
  fprintf(fp,"  G__c_setup_memvar%s();\n\n",G__DLLID);
  fprintf(fp,"  G__c_setup_global%s();\n",G__DLLID);
  fprintf(fp,"  G__c_setup_func%s();\n",G__DLLID);
  fprintf(fp,"  return;\n");
  fprintf(fp,"}\n");
  fclose(fp);
  fclose(hfp);
  FileWaitTime(10000);
  G__ctordtor_destruct();
}

#ifdef G__ROOT
/**************************************************************************
* G__cpp_initialize()
*
**************************************************************************/
void G__cpp_initialize(fp)
FILE *fp;
{
  fprintf(fp,"class G__cpp_setup_init%s {\n",G__DLLID);
  fprintf(fp,"  public:\n");
  if (G__DLLID && G__DLLID[0]) {
    fprintf(fp,"    G__cpp_setup_init%s() { G__add_setup_func(\"%s\",(G__incsetup)(&G__cpp_setup%s)); G__call_setup_funcs(); }\n",G__DLLID,G__DLLID,G__DLLID);
    fprintf(fp,"   ~G__cpp_setup_init%s() { G__remove_setup_func(\"%s\"); }\n",G__DLLID,G__DLLID);
  } else {
    fprintf(fp,"    G__cpp_setup_init() { G__add_setup_func(\"G__Default\",(G__incsetup)(&G__cpp_setup)); }\n");
    fprintf(fp,"   ~G__cpp_setup_init() { G__remove_setup_func(\"G__Default\"); }\n");
  }
  fprintf(fp,"};\n");
  fprintf(fp,"G__cpp_setup_init%s G__cpp_setup_initializer%s;\n\n",G__DLLID,G__DLLID);
}
#endif

/**************************************************************************
* G__gen_newdelete()
*
**************************************************************************/
void G__gen_newdelete(fp) 
FILE *fp;
{
  if(G__is_operator_newdelete&(G__DUMMYARG_NEWDELETE)){
    fprintf(fp,"class G__%s_tag {};\n\n",G__NEWID);
    
    if(G__is_operator_newdelete&(G__DUMMYARG_NEWDELETE_STATIC))
      fprintf(fp,"static ");
    fprintf(fp,"void* operator new(size_t size,G__%s_tag* p) {\n",G__NEWID);
    fprintf(fp,"  if(p && G__PVOID!=G__getgvp()) return((void*)p);\n");
    fprintf(fp,"#ifndef G__ROOT\n");
    fprintf(fp,"  return(malloc(size));\n");
    fprintf(fp,"#else\n");
    fprintf(fp,"  return(::operator new(size));\n");
    fprintf(fp,"#endif\n");
    fprintf(fp,"}\n\n");

    fprintf(fp,"/* dummy, for exception */\n");
    fprintf(fp,"#ifdef G__EH_DUMMY_DELETE\n");
    if(G__is_operator_newdelete&(G__DUMMYARG_NEWDELETE_STATIC))
      fprintf(fp,"static ");
    fprintf(fp,"void operator delete(void *p,G__%s_tag* x) {\n",G__NEWID);
    fprintf(fp,"  if((long)p==G__getgvp() && G__PVOID!=G__getgvp()) return;\n");
    fprintf(fp,"#ifndef G__ROOT\n");
    fprintf(fp,"  free(p);\n");
    fprintf(fp,"#else\n");
    fprintf(fp,"  ::operator delete(p);\n");
    fprintf(fp,"#endif\n");
    fprintf(fp,"}\n");
    fprintf(fp,"#endif\n\n");
    
    fprintf(fp,"static void G__operator_delete(void *p) {\n");
    fprintf(fp,"  if((long)p==G__getgvp() && G__PVOID!=G__getgvp()) return;\n");
    fprintf(fp,"#ifndef G__ROOT\n");
    fprintf(fp,"  free(p);\n");
    fprintf(fp,"#else\n");
    fprintf(fp,"  ::operator delete(p);\n");
    fprintf(fp,"#endif\n");
    fprintf(fp,"}\n\n");

    fprintf(fp,"void G__DELDMY_%s() { G__operator_delete(0); }\n\n",G__NEWID);
    
  }
  else {
    if(0==(G__is_operator_newdelete&(G__MASK_OPERATOR_NEW|G__IS_OPERATOR_NEW))){
      if(G__is_operator_newdelete&(G__NOT_USING_2ARG_NEW)) {
	fprintf(fp,"static void* operator new(size_t size) {\n");
	fprintf(fp,"  if(G__PVOID!=G__getgvp()) return((void*)G__getgvp());\n");
      }
      else {
	fprintf(fp,"static void* operator new(size_t size,void* p) {\n");
	fprintf(fp,"  if(p && (long)p==G__getgvp() && G__PVOID!=G__getgvp()) return(p);\n");
      }
      /* fprintf(fp,"  if(G__PVOID!=(long)p) return(p);\n"); */
      fprintf(fp,"#ifndef G__ROOT\n");
      fprintf(fp,"  return(malloc(size));\n");
      fprintf(fp,"#else\n");
      fprintf(fp,"  return new char[size];\n");
      fprintf(fp,"#endif\n");
      fprintf(fp,"}\n");
    }
    else {
      if(G__dispmsg>=G__DISPNOTE) {
	G__fprinterr(G__serr,"Note: operator new() masked %x\n"
		     ,G__is_operator_newdelete);
      }
    }
#ifdef G__N_EXPLICITDESTRUCTOR
    if(0==(G__is_operator_newdelete&
	   (G__MASK_OPERATOR_DELETE|G__IS_OPERATOR_DELETE))) {
      fprintf(fp,"static void operator delete(void *p) {\n");
      fprintf(fp,"  if((long)p==G__getgvp() && G__PVOID!=G__getgvp()) return;\n");
      /* fprintf(fp,"  if(G__PVOID!=G__getgvp()) return;\n"); */
      fprintf(fp,"  free(p);\n");
      fprintf(fp,"}\n");
    }
    else {
      if(G__dispmsg>=G__DISPNOTE) {
	G__fprinterr(G__serr,"Note: operator delete() masked %x\n"
		     ,G__is_operator_newdelete);
      }
    }
#endif /* G__N_EXPLICITDESTRUCTOR */
  }
}

/**************************************************************************
* G__gen_cpplink()
*
*  Generate C++ interface routine source file.
*
**************************************************************************/
void G__gen_cpplink()
{
/*
 *   include header files
 *   struct G__tagtable G__struct;
 *   struct G__typedef G__newtype;
 *   struct G__var_array G__struct.*memvar;
 *   struct G__ifunc_table G__struct.*memfunc;
 *   struct G__var_array G__global;
 *   struct G__ifunc_table G__ifunc;
 */
  FILE *fp;
  FILE *hfp;

  G__ctordtor_initialize();

  fp = fopen(G__CPPLINK_C,"a");
  if(!fp) G__fileerror(G__CPPLINK_C);
  fprintf(fp,"  G__cpp_reset_tagtable%s();\n",G__DLLID);
  fprintf(fp,"}\n");

  hfp=fopen(G__CPPLINK_H,"a");
  if(!hfp) G__fileerror(G__CPPLINK_H);

  {
    int algoflag=0;
    int filen;
    char *fname;
    int lenstl;
    char *sysstl;
    G__getcintsysdir();
    sysstl=(char*)malloc(strlen(G__cintsysdir)+10);
    sprintf(sysstl,"%s%sstl%s",G__cintsysdir,G__psep,G__psep);
    lenstl=strlen(sysstl);
    for(filen=0;filen<G__nfile;filen++) {
      fname = G__srcfile[filen].filename;
      if(strncmp(fname,sysstl,lenstl)==0) fname += lenstl;
      if(strcmp(fname,"vector")==0 || strcmp(fname,"list")==0 || 
	 strcmp(fname,"deque")==0 || strcmp(fname,"map")==0 || 
	 strcmp(fname,"multimap")==0 || strcmp(fname,"set")==0 || 
	 strcmp(fname,"multiset")==0 || strcmp(fname,"stack")==0 || 
	 strcmp(fname,"queue")==0) {
	algoflag |= 1;
      }
      if(strcmp(fname,"vector.h")==0 || strcmp(fname,"list.h")==0 || 
	 strcmp(fname,"deque.h")==0 || strcmp(fname,"map.h")==0 || 
	 strcmp(fname,"multimap.h")==0 || strcmp(fname,"set.h")==0 || 
	 strcmp(fname,"multiset.h")==0 || strcmp(fname,"stack.h")==0 || 
	 strcmp(fname,"queue.h")==0) {
	algoflag |= 2;
      }
    }
    if(algoflag&1) {
      fprintf(hfp,"#include <algorithm>\n");
      if(G__ignore_stdnamespace) {
	/* fprintf(hfp,"#ifndef __hpux\n"); */
	fprintf(hfp,"namespace std { }\n");
	fprintf(hfp,"using namespace std;\n");
	/* fprintf(hfp,"#endif\n"); */
      }
    }
    else if(algoflag&2) fprintf(hfp,"#include <algorithm.h>\n");
    if(sysstl) free((void*)sysstl);
  }

#if !defined(G__ROOT) || defined(G__OLDIMPLEMENTATION1817)
  if(G__CPPLINK==G__globalcomp&&-1!=G__defined_tagname("G__longlong",2)) {
#if defined(__hpux) && !defined(G__ROOT)
    G__getcintsysdir();
    fprintf(hfp,"\n#include \"%s%slib/longlong/longlong.h\"\n",G__cintsysdir,G__psep);
#else
    fprintf(hfp,"\n#include \"lib/longlong/longlong.h\"\n");
#endif
  }
#endif /* G__ROOT */

  G__gen_newdelete(fp);

#ifdef G__BUILTIN
    fprintf(fp,"#include \"dllrev.h\"\n");
    fprintf(fp,"extern \"C\" int G__cpp_dllrev%s() { return(G__CREATEDLLREV); }\n",G__DLLID);
#else
    fprintf(fp,"extern \"C\" int G__cpp_dllrev%s() { return(%d); }\n",G__DLLID,G__CREATEDLLREV);
#endif

  fprintf(hfp,"\n#ifndef G__MEMFUNCBODY\n");
  if(!G__suppress_methods) G__cppif_memfunc(fp,hfp);
  G__cppif_func(fp,hfp);
  if(!G__suppress_methods) G__cppstub_memfunc(fp);
  G__cppstub_func(fp);
  fprintf(hfp,"#endif\n\n");

  G__cppif_p2memfunc(fp);

#ifdef G__VIRTUALBASE
  G__cppif_inheritance(fp);
#endif
  G__cpplink_inheritance(fp);
  G__cpplink_typetable(fp,hfp);
  G__cpplink_memvar(fp);
  if(!G__suppress_methods) G__cpplink_memfunc(fp);
  G__cpplink_global(fp);
  G__cpplink_func(fp);
  G__cpplink_tagtable(fp,hfp);
  fprintf(fp,"extern \"C\" void G__cpp_setup%s(void) {\n",G__DLLID);
#ifdef G__BUILTIN
  fprintf(fp,"  G__check_setup_version(G__CREATEDLLREV,\"G__cpp_setup%s()\");\n",
          G__DLLID);
#else
  fprintf(fp,"  G__check_setup_version(%d,\"G__cpp_setup%s()\");\n",
          G__CREATEDLLREV,G__DLLID);
#endif
  fprintf(fp,"  G__set_cpp_environment%s();\n",G__DLLID);
  fprintf(fp,"  G__cpp_setup_tagtable%s();\n\n",G__DLLID);
  fprintf(fp,"  G__cpp_setup_inheritance%s();\n\n",G__DLLID);
  fprintf(fp,"  G__cpp_setup_typetable%s();\n\n",G__DLLID);
  fprintf(fp,"  G__cpp_setup_memvar%s();\n\n",G__DLLID);
  if(!G__suppress_methods)
    fprintf(fp,"  G__cpp_setup_memfunc%s();\n",G__DLLID);
  fprintf(fp,"  G__cpp_setup_global%s();\n",G__DLLID);
  fprintf(fp,"  G__cpp_setup_func%s();\n",G__DLLID);
  G__set_sizep2memfunc(fp);
  fprintf(fp,"  return;\n");
  fprintf(fp,"}\n");

#ifdef G__ROOT
  /* Only activated for ROOT at this moment. Need to come back */
  G__cpp_initialize(fp);
#endif

  fclose(fp);
  fclose(hfp);
  FileWaitTime(10000);
#ifdef G__GENWINDEF
  fprintf(G__WINDEFfp,"\n");
  fclose(G__WINDEFfp);
#endif

  if(
     (0==(G__is_operator_newdelete&G__DUMMYARG_NEWDELETE)) &&
     (G__is_operator_newdelete&(G__IS_OPERATOR_NEW|G__IS_OPERATOR_DELETE))) {
    fp = fopen("G__ISOPRNEW","w");
    if(!fp) G__fileerror("G__ISOPRNEW");
    fprintf(fp,"Global function new/delete are overloaded\n");
    fclose(fp);
    G__fprinterr(G__serr,"################### CAUTION ##########################\n");
    G__fprinterr(G__serr,"//Overloaded global operator new and/or delete are\n");
    G__fprinterr(G__serr,"//found in user's source file.\n");
    G__fprinterr(G__serr,"//Modify functions as follows. Otherwise, you may find\n");
    G__fprinterr(G__serr,"//problems later.  (%x)\n",G__is_operator_newdelete);
    G__fprinterr(G__serr,"\n");
    G__fprinterr(G__serr,"// Giving memory arena to a base class object for constructor call\n");
    G__fprinterr(G__serr,"#define G__PVOID (-1)\n");
    G__fprinterr(G__serr,"extern \"C\" long G__getgvp();\n");
    G__fprinterr(G__serr,"\n");
    if(G__is_operator_newdelete&(G__IS_OPERATOR_NEW)) {
      if(G__is_operator_newdelete&(G__NOT_USING_2ARG_NEW)) {
        G__fprinterr(G__serr,"void* operator new(size_t size) {\n");
        G__fprinterr(G__serr,"  if(G__PVOID!=G__getgvp()) return((void*)G__getgvp());\n");
      }
      else {
        G__fprinterr(G__serr,"void* operator new(size_t size,void* p) {\n");
        G__fprinterr(G__serr,"  if(p && (long)p==G__getgvp() && G__PVOID!=G__getgvp()) {G__setgvp(G__PVOID);return(p);}\n");
      }
      G__fprinterr(G__serr,"  // Yourown things...\n");
      G__fprinterr(G__serr,"}\n");
      G__fprinterr(G__serr,"\n");
    }
#ifdef G__N_EXPLICITDESTRUCTOR
    if(G__is_operator_newdelete&(G__IS_OPERATOR_DELETE)) {
      G__fprinterr(G__serr,"void operator delete(void *p) {\n");
      G__fprinterr(G__serr,"  if((long)p==G__getgvp() && G__PVOID!=G__getgvp()) {G__setgvp(G__PVOID);return;}\n");
      G__fprinterr(G__serr,"  // Yourown things...\n");
      G__fprinterr(G__serr,"}\n");
    }
#endif
    G__fprinterr(G__serr,"######################################################\n");
  }

  G__ctordtor_destruct();
}

/**************************************************************************
* G__cleardictfile()
**************************************************************************/
int G__cleardictfile(flag)
int flag;
{
  if(EXIT_SUCCESS!=flag) {
    G__fprinterr(G__serr,"!!!Removing ");
    if(G__CPPLINK_C) {
      remove(G__CPPLINK_C);
      G__fprinterr(G__serr,"%s ",G__CPPLINK_C);
    }
    if(G__CPPLINK_H) {
      remove(G__CPPLINK_H);
      G__fprinterr(G__serr,"%s ",G__CPPLINK_H);
    }
    if(G__CLINK_C) {
      remove(G__CLINK_C);
      G__fprinterr(G__serr,"%s ",G__CLINK_C);
    }
    if(G__CLINK_H) {
      remove(G__CLINK_H);
      G__fprinterr(G__serr,"%s ",G__CLINK_H);
    }
    G__fprinterr(G__serr,"!!!\n");
  }
#ifdef G__GENWINDEF
  if(G__WINDEF) free(G__WINDEF);
#endif
  if(G__CPPLINK_H) free(G__CPPLINK_H);
  if(G__CPPLINK_C) free(G__CPPLINK_C);
  if(G__CLINK_H) free(G__CLINK_H);
  if(G__CLINK_C) free(G__CLINK_C);

#ifdef G__GENWINDEF
  G__WINDEF = (char*)NULL;
#endif
  G__CPPLINK_C = (char*)NULL;
  G__CPPLINK_H = (char*)NULL;
  G__CLINK_C = (char*)NULL;
  G__CLINK_H = (char*)NULL;
  return(0);
}


/**************************************************************************
* G__clink_header()
*
**************************************************************************/
void G__clink_header(fp)
FILE *fp;
{
  int i;
  fprintf(fp,"#include <stddef.h>\n");
  fprintf(fp,"#include <stdio.h>\n");
  fprintf(fp,"#include <stdlib.h>\n");
  fprintf(fp,"#include <math.h>\n");
  fprintf(fp,"#include <string.h>\n");
  if(G__multithreadlibcint) 
    fprintf(fp,"#define G__MULTITHREADLIBCINTC\n");
  fprintf(fp,"#define G__ANSIHEADER\n");
#if defined(G__VAARG_COPYFUNC) || !defined(G__OLDIMPLEMENTATION1530)
  fprintf(fp,"#define G__DICTIONARY\n");
#endif
#if defined(__hpux) && !defined(G__ROOT)
  G__getcintsysdir();
  fprintf(fp,"#include \"%s%sG__ci.h\"\n",G__cintsysdir,G__psep);
#else
  fprintf(fp,"#include \"G__ci.h\"\n");
#endif
  if(G__multithreadlibcint) 
    fprintf(fp,"#undef G__MULTITHREADLIBCINTC\n");

#ifdef G__BORLAND
  fprintf(fp,"extern G__DLLEXPORT int G__c_dllrev%s();\n",G__DLLID);
  fprintf(fp,"extern G__DLLEXPORT void G__set_c_environment%s();\n",G__DLLID);
  fprintf(fp,"extern G__DLLEXPORT void G__c_setup_tagtable%s();\n",G__DLLID);
  fprintf(fp,"extern G__DLLEXPORT void G__c_setup_typetable%s();\n",G__DLLID);
  fprintf(fp,"extern G__DLLEXPORT void G__c_setup_memvar%s();\n",G__DLLID);
  fprintf(fp,"extern G__DLLEXPORT void G__c_setup_global%s();\n",G__DLLID);
  fprintf(fp,"extern G__DLLEXPORT void G__c_setup_func%s();\n",G__DLLID);
  fprintf(fp,"extern G__DLLEXPORT void G__c_setup%s();\n",G__DLLID);
  if(G__multithreadlibcint) {
    fprintf(fp,"extern G__DLLEXPORT void G__SetCCintApiPointers G__P((\n");
#if !defined(G__OLDIMPLEMENTATION1485)
    for(i=0;i<125;i++) {
      fprintf(fp,"\tvoid*");
      if(i!=124) fprintf(fp,",\n");
    }
#elif !defined(G__OLDIMPLEMENTATION1546)
    for(i=0;i<124;i++) {
      fprintf(fp,"\tvoid*");
      if(i!=123) fprintf(fp,",\n");
    }
#else
    for(i=0;i<122;i++) {
      fprintf(fp,"\tvoid*");
      if(i!=121) fprintf(fp,",\n");
    }
#endif
    fprintf(fp,"));\n");
  }
#else
  fprintf(fp,"extern void G__c_setup_tagtable%s();\n",G__DLLID);
  fprintf(fp,"extern void G__c_setup_typetable%s();\n",G__DLLID);
  fprintf(fp,"extern void G__c_setup_memvar%s();\n",G__DLLID);
  fprintf(fp,"extern void G__c_setup_global%s();\n",G__DLLID);
  fprintf(fp,"extern void G__c_setup_func%s();\n",G__DLLID);
  fprintf(fp,"extern void G__set_c_environment%s();\n",G__DLLID);
  if(G__multithreadlibcint) {
    fprintf(fp,"extern void G__SetCCintApiPointers G__P((\n");
#if !defined(G__OLDIMPLEMENTATION1485)
    for(i=0;i<125;i++) {
      fprintf(fp,"\tvoid*");
      if(i!=124) fprintf(fp,",\n");
    }
#elif !defined(G__OLDIMPLEMENTATION1546)
    for(i=0;i<124;i++) {
      fprintf(fp,"\tvoid*");
      if(i!=123) fprintf(fp,",\n");
    }
#else
    for(i=0;i<122;i++) {
      fprintf(fp,"\tvoid*");
      if(i!=121) fprintf(fp,",\n");
    }
#endif
    fprintf(fp,"));\n");
  }
#endif


  fprintf(fp,"\n");
  fprintf(fp,"\n");
}

/**************************************************************************
* G__cpplink_header()
*
**************************************************************************/
void G__cpplink_header(fp)
FILE *fp;
{
  int i;
  fprintf(fp,"#include <stddef.h>\n");
  fprintf(fp,"#include <stdio.h>\n");
  fprintf(fp,"#include <stdlib.h>\n");
  fprintf(fp,"#include <math.h>\n");
  fprintf(fp,"#include <string.h>\n");
  if(G__multithreadlibcint) 
    fprintf(fp,"#define G__MULTITHREADLIBCINTCPP\n");
  fprintf(fp,"#define G__ANSIHEADER\n");
#if defined(G__VAARG_COPYFUNC) || !defined(G__OLDIMPLEMENTATION1530)
  fprintf(fp,"#define G__DICTIONARY\n");
#endif
#if defined(__hpux) && !defined(G__ROOT)
  G__getcintsysdir();
  fprintf(fp,"#include \"%s%sG__ci.h\"\n",G__cintsysdir,G__psep);
#else
  fprintf(fp,"#include \"G__ci.h\"\n");
#endif
  if(G__multithreadlibcint) 
    fprintf(fp,"#undef G__MULTITHREADLIBCINTCPP\n");

  fprintf(fp,"extern \"C\" {\n");

#ifdef G__BORLAND
  fprintf(fp,"extern G__DLLEXPORT int G__cpp_dllrev%s();\n",G__DLLID);
  fprintf(fp,"extern G__DLLEXPORT void G__set_cpp_environment%s();\n",G__DLLID);
  fprintf(fp,"extern G__DLLEXPORT void G__cpp_setup_tagtable%s();\n",G__DLLID);
  fprintf(fp,"extern G__DLLEXPORT void G__cpp_setup_inheritance%s();\n",G__DLLID);
  fprintf(fp,"extern G__DLLEXPORT void G__cpp_setup_typetable%s();\n",G__DLLID);
  fprintf(fp,"extern G__DLLEXPORT void G__cpp_setup_memvar%s();\n",G__DLLID);
  fprintf(fp,"extern G__DLLEXPORT void G__cpp_setup_global%s();\n",G__DLLID);
  fprintf(fp,"extern G__DLLEXPORT void G__cpp_setup_memfunc%s();\n",G__DLLID);
  fprintf(fp,"extern G__DLLEXPORT void G__cpp_setup_func%s();\n",G__DLLID);
  fprintf(fp,"extern G__DLLEXPORT void G__cpp_setup%s();\n",G__DLLID);
  if(G__multithreadlibcint) {
    fprintf(fp,"extern G__DLLEXPORT void G__SetCppCintApiPointers G__P((\n");
#if !defined(G__OLDIMPLEMENTATION1485)
    for(i=0;i<125;i++) {
      fprintf(fp,"\tvoid*");
      if(i!=124) fprintf(fp,",\n");
    }
#elif !defined(G__OLDIMPLEMENTATION1546)
    for(i=0;i<124;i++) {
      fprintf(fp,"\tvoid*");
      if(i!=123) fprintf(fp,",\n");
    }
#else
    for(i=0;i<122;i++) {
      fprintf(fp,"\tvoid*");
      if(i!=121) fprintf(fp,",\n");
    }
#endif
    fprintf(fp,"));\n");
  }
#else
  fprintf(fp,"extern void G__cpp_setup_tagtable%s();\n",G__DLLID);
  fprintf(fp,"extern void G__cpp_setup_inheritance%s();\n",G__DLLID);
  fprintf(fp,"extern void G__cpp_setup_typetable%s();\n",G__DLLID);
  fprintf(fp,"extern void G__cpp_setup_memvar%s();\n",G__DLLID);
  fprintf(fp,"extern void G__cpp_setup_global%s();\n",G__DLLID);
  fprintf(fp,"extern void G__cpp_setup_memfunc%s();\n",G__DLLID);
  fprintf(fp,"extern void G__cpp_setup_func%s();\n",G__DLLID);
  fprintf(fp,"extern void G__set_cpp_environment%s();\n",G__DLLID);
  if(G__multithreadlibcint) {
    fprintf(fp,"extern void G__SetCppCintApiPointers G__P((\n");
#if !defined(G__OLDIMPLEMENTATION1485)
    for(i=0;i<125;i++) {
      fprintf(fp,"\tvoid*");
      if(i!=124) fprintf(fp,",\n");
    }
#elif !defined(G__OLDIMPLEMENTATION1546)
    for(i=0;i<124;i++) {
      fprintf(fp,"\tvoid*");
      if(i!=123) fprintf(fp,",\n");
    }
#else
    for(i=0;i<122;i++) {
      fprintf(fp,"\tvoid*");
      if(i!=121) fprintf(fp,",\n");
    }
#endif
    fprintf(fp,"));\n");
  }
#endif


  fprintf(fp,"}\n");
  fprintf(fp,"\n");
  fprintf(fp,"\n");
}

/**************************************************************************
**************************************************************************
* Function to generate C++ interface routine G__cpplink.C
**************************************************************************
**************************************************************************/


/**************************************************************************
* G__map_cpp_name()
**************************************************************************/
char *G__map_cpp_name(in)
char *in;
{
  static char out[G__MAXNAME*6];
  int i=0,j=0,c;
  while((c=in[i])) {
    switch(c) {
    case '+': strcpy(out+j,"pL"); j+=2; break;
    case '-': strcpy(out+j,"mI"); j+=2; break;
    case '*': strcpy(out+j,"mU"); j+=2; break;
    case '/': strcpy(out+j,"dI"); j+=2; break;
    case '&': strcpy(out+j,"aN"); j+=2; break;
    case '%': strcpy(out+j,"pE"); j+=2; break;
    case '|': strcpy(out+j,"oR"); j+=2; break;
    case '^': strcpy(out+j,"hA"); j+=2; break;
    case '>': strcpy(out+j,"gR"); j+=2; break;
    case '<': strcpy(out+j,"lE"); j+=2; break;
    case '=': strcpy(out+j,"eQ"); j+=2; break;
    case '~': strcpy(out+j,"wA"); j+=2; break;
    case '.': strcpy(out+j,"dO"); j+=2; break;
    case '(': strcpy(out+j,"oP"); j+=2; break;
    case ')': strcpy(out+j,"cP"); j+=2; break;
    case '[': strcpy(out+j,"oB"); j+=2; break;
    case ']': strcpy(out+j,"cB"); j+=2; break;
    case '!': strcpy(out+j,"nO"); j+=2; break;
    case ',': strcpy(out+j,"cO"); j+=2; break;
    case '$': strcpy(out+j,"dA"); j+=2; break;
    case ' ': strcpy(out+j,"sP"); j+=2; break;
    case ':': strcpy(out+j,"cL"); j+=2; break;
    case '"': strcpy(out+j,"dQ"); j+=2; break;
    case '@': strcpy(out+j,"aT"); j+=2; break;
    case '\'': strcpy(out+j,"sQ"); j+=2; break;
    case '\\': strcpy(out+j,"fI"); j+=2; break;
    default: out[j++]=c; break;
    }
    ++i;
  }
  out[j]='\0';
  return(out);
}


/**************************************************************************
* G__map_cpp_funcname()
*
* Mapping between C++ function and parameter name to cint interface
* function name. This routine handles mapping of function and operator
* overloading in linked C++ object.
**************************************************************************/
char *G__map_cpp_funcname(tagnum,funcname,ifn,page)
int tagnum;
char *funcname;
int ifn,page;
{
  static char mapped_name[G__MAXNAME];
  char *dllid;

#ifndef G__OLDIMPLEMENTATION1911
  if(0 && funcname) return((char*)NULL);
#endif

  if(G__DLLID[0]) dllid=G__DLLID;
  else if(G__PROJNAME[0]) dllid=G__PROJNAME;
  else dllid="";

  if(-1==tagnum) {
    sprintf(mapped_name,"G__%s__%d_%d",G__map_cpp_name(dllid),ifn,page);
  }
  else {
    sprintf(mapped_name,"G__%s_%d_%d_%d",G__map_cpp_name(dllid),tagnum,ifn,page);
  }
  return(mapped_name);

}

/**************************************************************************
* G__cpplink_protected_stub_ctor
*
**************************************************************************/
void G__cpplink_protected_stub_ctor(tagnum,hfp)
int tagnum;
FILE* hfp;
{
  struct G__ifunc_table *memfunc = G__struct.memfunc[tagnum];
  int ifn;

  while(memfunc) {
    for(ifn=0;ifn<memfunc->allifunc;ifn++) {
      if(strcmp(G__struct.name[tagnum],memfunc->funcname[ifn])==0) {
	int i;
	fprintf(hfp,"  %s_PR(" ,G__get_link_tagname(tagnum));
	for(i=0;i<memfunc->para_nu[ifn];i++) {
	  if(i) fprintf(hfp,",");
	  fprintf(hfp,"%s a%d"
		  ,G__type2string(memfunc->para_type[ifn][i]
				  ,memfunc->para_p_tagtable[ifn][i]
				  ,memfunc->para_p_typetable[ifn][i]
				  ,memfunc->para_reftype[ifn][i]
				  ,memfunc->para_isconst[ifn][i])
		  ,i);
	}
	fprintf(hfp,")\n");
	fprintf(hfp,": %s(" ,G__fulltagname(tagnum,1));
	for(i=0;i<memfunc->para_nu[ifn];i++) {
	  if(i) fprintf(hfp,",");
	  fprintf(hfp,"a%d",i);
	}
	fprintf(hfp,") {}\n");
      }
    }
    memfunc=memfunc->next;
  }
}

/**************************************************************************
* G__cpplink_protected_stub
*
**************************************************************************/
void G__cpplink_protected_stub(fp,hfp) 
FILE *fp;
FILE *hfp;
{
  int i;
  /* Create stub derived class for protected member access */
  fprintf(hfp,"\n/* STUB derived class for protected member access */\n");
  for(i=0;i<G__struct.alltag;i++) {
    if(G__CPPLINK == G__struct.globalcomp[i] && G__struct.hash[i] &&
       G__struct.protectedaccess[i] ) {
      int ig15,ifn,n;
      struct G__var_array *memvar = G__struct.memvar[i];
      struct G__ifunc_table *memfunc = G__struct.memfunc[i];
      fprintf(hfp,"class %s_PR : public %s {\n" 
	      ,G__get_link_tagname(i),G__fulltagname(i,1));
      fprintf(hfp," public:\n");
      if(((G__struct.funcs[i]&G__HAS_XCONSTRUCTOR) ||
	  (G__struct.funcs[i]&G__HAS_COPYCONSTRUCTOR)) 
	 && 0==(G__struct.funcs[i]&G__HAS_DEFAULTCONSTRUCTOR)) {
	G__cpplink_protected_stub_ctor(i,hfp);
      }
      /* member function */
      while(memfunc) {
	for(ifn=0;ifn<memfunc->allifunc;ifn++) {
	  if((G__PROTECTED==memfunc->access[ifn]
	      || ((G__PRIVATEACCESS&G__struct.protectedaccess[i]) &&
		  G__PRIVATE==memfunc->access[ifn])
	      ) &&
	     strcmp(memfunc->funcname[ifn],G__struct.name[i])==0) {
	    fprintf(hfp,"  %s_PR(",G__get_link_tagname(i));
	    if(0==memfunc->para_nu[ifn]) {
	      fprintf(hfp,"void");
	    }
	    else {
	      for(n=0;n<memfunc->para_nu[ifn];n++) {
		if(n!=0) fprintf(hfp,",");
		fprintf(hfp,"%s G__%d"
			,G__type2string(memfunc->para_type[ifn][n]
					,memfunc->para_p_tagtable[ifn][n]
					,memfunc->para_p_typetable[ifn][n]
					,memfunc->para_reftype[ifn][n]
					,memfunc->para_isconst[ifn][n]),n);
	      }
	    }
	    fprintf(hfp,") : %s(",G__fulltagname(i,1));
	    if(0<memfunc->para_nu[ifn]) {
	      for(n=0;n<memfunc->para_nu[ifn];n++) {
		if(n!=0) fprintf(hfp,",");
		fprintf(hfp,"G__%d",n);
	      }
	    }
	    fprintf(hfp,") { }\n");
	  }
	  if((G__PROTECTED==memfunc->access[ifn]
	      || ((G__PRIVATEACCESS&G__struct.protectedaccess[i]) &&
		  G__PRIVATE==memfunc->access[ifn])
	      ) &&
	     strcmp(memfunc->funcname[ifn],G__struct.name[i])!=0 &&
	     '~'!=memfunc->funcname[ifn][0]) {
	    if(memfunc->staticalloc[ifn]) fprintf(hfp,"  static ");
	    fprintf(hfp,"  %s G__PT_%s("
		    ,G__type2string(memfunc->type[ifn]
				    ,memfunc->p_tagtable[ifn]
				    ,memfunc->p_typetable[ifn]
				    ,memfunc->reftype[ifn]
				    ,memfunc->isconst[ifn])
		    ,memfunc->funcname[ifn]);
	    if(0==memfunc->para_nu[ifn]) {
	      fprintf(hfp,"void");
	    }
	    else {
	      for(n=0;n<memfunc->para_nu[ifn];n++) {
		if(n!=0) fprintf(hfp,",");
		fprintf(hfp,"%s G__%d"
			,G__type2string(memfunc->para_type[ifn][n]
					,memfunc->para_p_tagtable[ifn][n]
					,memfunc->para_p_typetable[ifn][n]
					,memfunc->para_reftype[ifn][n]
					,memfunc->para_isconst[ifn][n]),n);
	      }
	    }
	    fprintf(hfp,") {\n");
	    if('y'!=memfunc->type[ifn]) fprintf(hfp,"    return(");
	    else                        fprintf(hfp,"    ");
	    fprintf(hfp,"%s(",memfunc->funcname[ifn]);
	    for(n=0;n<memfunc->para_nu[ifn];n++) {
	      if(n!=0) fprintf(hfp,",");
	      fprintf(hfp,"G__%d",n);
	    }
	    fprintf(hfp,")");
	    if('y'!=memfunc->type[ifn]) fprintf(hfp,")");
	    fprintf(hfp,";\n");
	    fprintf(hfp,"  }\n");
	  }
	}
	memfunc = memfunc->next;
      }
      /* data member */
      while(memvar) {
	for(ig15=0;ig15<memvar->allvar;ig15++) {
	  if(G__PROTECTED==memvar->access[ig15]) {
	    if(G__AUTO==memvar->statictype[ig15]) 
	      fprintf(hfp,"  long G__OS_%s(){return((long)(&%s)-(long)this);}\n"
		      ,memvar->varnamebuf[ig15],memvar->varnamebuf[ig15]);
	    else
	      fprintf(hfp,"  static long G__OS_%s(){return((long)(&%s));}\n"
		      ,memvar->varnamebuf[ig15],memvar->varnamebuf[ig15]);
	  }
	}
	memvar = memvar->next;
      }
      fprintf(hfp,"};\n");
    }
  }
  fprintf(fp,"\n");
}

/**************************************************************************
* G__cpplink_linked_taginfo
*
**************************************************************************/
void G__cpplink_linked_taginfo(fp,hfp)
FILE *fp;
FILE *hfp;
{
  int i;
  char buf[G__MAXFILENAME];
  FILE* pfp;
  if(G__privateaccess) {
    char *xp;
    strcpy(buf,G__CPPLINK_H);
    xp = strstr(buf,".h");
    if(xp) strcpy(xp,"P.h");
    pfp = fopen(buf,"r");
    if(pfp) {
      fclose(pfp);
      FileWaitTime(10000);
      remove(buf);
    }
    pfp = fopen(buf,"w");
    fprintf(pfp,"#ifdef PrivateAccess\n");
    fprintf(pfp,"#undef PrivateAccess\n");
    fprintf(pfp,"#endif\n");
    fprintf(pfp,"#define PrivateAccess(name) PrivateAccess_##name\n");
    fclose(pfp);
    FileWaitTime(10000);
  }
  fprintf(fp,"/* Setup class/struct taginfo */\n");
  for(i=0;i<G__struct.alltag;i++) {
    if((G__NOLINK > G__struct.globalcomp[i]
	|| G__ONLYMETHODLINK==G__struct.globalcomp[i]
	) &&
       (
	(G__struct.hash[i] || 0==G__struct.name[i][0])
	|| -1!=G__struct.parent_tagnum[i])) {
      fprintf(fp,"G__linked_taginfo %s = { \"%s\" , %d , -1 };\n"
	      ,G__get_link_tagname(i),G__fulltagname(i,0),G__struct.type[i]);
      fprintf(hfp,"extern G__linked_taginfo %s;\n",G__get_link_tagname(i));
      if(G__privateaccess) {
	pfp = fopen(buf,"a");
	if(pfp) {
	  if(G__PRIVATEACCESS&G__struct.protectedaccess[i]) 
	    fprintf(pfp,"#define PrivateAccess_%s  friend class %s_PR;\n"
		    ,G__fulltagname(i,1),G__get_link_tagname(i));
	  else
	    fprintf(pfp,"#define PrivateAccess_%s \n",G__fulltagname(i,1));
	  fclose(pfp);
	  FileWaitTime(10000);
	}
      }
    }
  }
  fprintf(fp,"\n");

  fprintf(fp,"/* Reset class/struct taginfo */\n");
  switch(G__globalcomp) {
  case G__CLINK:
    fprintf(fp,"void G__c_reset_tagtable%s() {\n",G__DLLID);
    break;
  case G__CPPLINK:
  default:
    fprintf(fp,"extern \"C\" void G__cpp_reset_tagtable%s() {\n",G__DLLID);
    break;
  }
  
  for(i=0;i<G__struct.alltag;i++) {
    if((G__NOLINK > G__struct.globalcomp[i] 
       || G__ONLYMETHODLINK==G__struct.globalcomp[i]
	) && 
       (
	(G__struct.hash[i] || 0==G__struct.name[i][0])
	|| -1!=G__struct.parent_tagnum[i])) {
      fprintf(fp,"  %s.tagnum = -1 ;\n",G__get_link_tagname(i));
    }
  }

  fprintf(fp,"}\n\n");

  G__cpplink_protected_stub(fp,hfp);

}



typedef void (*G__pMethodUpdateClassInfo)(char *item,long tagnum);
G__pMethodUpdateClassInfo G__UserSpecificUpdateClassInfo;

/**************************************************************************
* G__get_linked_tagnum
*
*  Setup and return tagnum
**************************************************************************/
int G__get_linked_tagnum(p)
G__linked_taginfo *p;
{
  if(!p) return(-1);
  if(-1==p->tagnum) {
     p->tagnum = G__search_tagname(p->tagname,p->tagtype);
     if (G__UserSpecificUpdateClassInfo) {
        long varp = G__globalvarpointer;
        G__globalvarpointer = G__PVOID;
        (*G__UserSpecificUpdateClassInfo)(p->tagname,p->tagnum);
        G__globalvarpointer = varp;
     }
  }
  return(p->tagnum);
}

/**************************************************************************
* G__get_linked_tagnum_with_param
*
*  Setup and return tagnum; also set user parameter
**************************************************************************/
int G__get_linked_tagnum_with_param(p, param)
G__linked_taginfo *p;
void* param;
{
  int tag = G__get_linked_tagnum(p);
  if(tag != -1) {
    G__struct.userparam[tag] = param;
    return tag;
  }
  return -1;
}

/**************************************************************************
* G__get_linked_user_param
*
*  Retrieve user parameter
**************************************************************************/
void* G__get_linked_user_param(tag_num)
int tag_num;
{
  if ( tag_num<0 || tag_num>G__MAXSTRUCT ) return 0;
  return G__struct.userparam[tag_num];
}

/**************************************************************************
* G__get_link_tagname
*
*  Setup and return tagnum
**************************************************************************/
char *G__get_link_tagname(tagnum)
int tagnum;
{
  static char mapped_tagname[G__MAXNAME*6];
  if(G__struct.hash[tagnum]) {
    sprintf(mapped_tagname,"G__%sLN_%s"  ,G__DLLID
	    ,G__map_cpp_name(G__fulltagname(tagnum,0)));
  }
  else {
    sprintf(mapped_tagname,"G__%sLN_%s%d"  ,G__DLLID
	   ,G__map_cpp_name(G__fulltagname(tagnum,0)),tagnum);
  }
  return(mapped_tagname);
}

/**************************************************************************
* G__mark_linked_tagnum
*
*  Setup and return tagnum
**************************************************************************/
char *G__mark_linked_tagnum(tagnum)
int tagnum;
{
  int tagnumorig = tagnum;
  if(tagnum<0) {
    G__fprinterr(G__serr,"Internal error: G__mark_linked_tagnum() Illegal tagnum %d\n",tagnum);
    return("");
  }

  while(tagnum>=0) {
    if(G__NOLINK == G__struct.globalcomp[tagnum]) {
      /* this class is unlinked but tagnum interface requested.
       * G__globalcomp is already G__CLINK=-2 or G__CPPLINK=-1,
       * Following assignment will decrease the value by 2 more */
      G__struct.globalcomp[tagnum] = G__globalcomp-2;
    }
    tagnum = G__struct.parent_tagnum[tagnum];
  }
  return(G__get_link_tagname(tagnumorig));
}


/**************************************************************************
* G__set_DLLflag()
*
*
**************************************************************************/
void G__setDLLflag(flag)
int flag;
{
#ifdef G__GENWINDEF
  G__isDLL = flag;
#else
#ifndef G__OLDIMPLEMENTATION1911
  if(flag) return;
#endif
#endif
}

/**************************************************************************
* G__setPROJNAME()
*
*
**************************************************************************/
void G__setPROJNAME(proj)
char *proj;
{
  strcpy(G__PROJNAME,G__map_cpp_name(proj));
}

/**************************************************************************
* G__setCINTLIBNAME()
*
*
**************************************************************************/
void G__setCINTLIBNAME(cintlib)
char *cintlib;
{
#ifdef G__GENWINDEF
  strcpy(G__CINTLIBNAME,cintlib);
#else
#ifndef G__OLDIMPLEMENTATION1911
  if(cintlib) return;
#endif
#endif
}

#ifdef G__GENWINDEF
/**************************************************************************
* G__write_windef_header()
*
*
**************************************************************************/
static void G__write_windef_header()
{
  FILE* fp;

  fp = fopen(G__WINDEF,"w");
  if(!fp) G__fileerror(G__WINDEF);
  G__WINDEFfp=fp;

  if(G__isDLL)
    fprintf(fp,"LIBRARY           \"%s\"\n",G__PROJNAME);
  else
    fprintf(fp,"NAME              \"%s\" WINDOWAPI\n",G__PROJNAME);
  fprintf(fp,"\n");
#if defined(G__OLDIMPLEMENTATION1971) || !defined(G__VISUAL)
  fprintf(fp,"DESCRIPTION       '%s'\n",G__PROJNAME);
  fprintf(fp,"\n");
#endif
#if !defined(G__VISUAL) && !defined(G__CYGWIN)
  fprintf(fp,"EXETYPE           NT\n");
  fprintf(fp,"\n");
  if(G__isDLL)
    fprintf(fp,"SUBSYSTEM	WINDOWS\n");
  else
    fprintf(fp,"SUBSYSTEM   CONSOLE\n");
  fprintf(fp,"\n");
  fprintf(fp,"STUB              'WINSTUB.EXE'\n");
  fprintf(fp,"\n");
#endif	/* G__VISUAL */
  fprintf(fp,"VERSION           1.0\n");
  fprintf(fp,"\n");
#if defined(G__OLDIMPLEMENTATION1971) || !defined(G__VISUAL)
  fprintf(fp,"CODE               EXECUTE READ\n");
  fprintf(fp,"\n");
  fprintf(fp,"DATA               READ WRITE\n");
  fprintf(fp,"\n");
#endif
  fprintf(fp,"HEAPSIZE  1048576,4096\n");
  fprintf(fp,"\n");
#ifndef G__VISUAL
  fprintf(fp,"IMPORTS\n");
  fprintf(fp,"        _G__main=%s.G__main\n",G__CINTLIBNAME);
  fprintf(fp,"        _G__setothermain=%s.G__setothermain\n",G__CINTLIBNAME);
  fprintf(fp,"        _G__getnumbaseclass=%s.G__getnumbaseclass\n",G__CINTLIBNAME);
  fprintf(fp,"        _G__setnewtype=%s.G__setnewtype\n",G__CINTLIBNAME);
  fprintf(fp,"        _G__setnewtypeindex=%s.G__setnewtypeindex\n",G__CINTLIBNAME);
  fprintf(fp,"        _G__resetplocal=%s.G__resetplocal\n",G__CINTLIBNAME);
  fprintf(fp,"        _G__getgvp=%s.G__getgvp\n",G__CINTLIBNAME);
  fprintf(fp,"        _G__resetglobalenv=%s.G__resetglobalenv\n",G__CINTLIBNAME);
  fprintf(fp,"        _G__lastifuncposition=%s.G__lastifuncposition\n",G__CINTLIBNAME);
  fprintf(fp,"        _G__resetifuncposition=%s.G__resetifuncposition\n",G__CINTLIBNAME);
  fprintf(fp,"        _G__setnull=%s.G__setnull\n",G__CINTLIBNAME);
  fprintf(fp,"        _G__getstructoffset=%s.G__getstructoffset\n",G__CINTLIBNAME);
  fprintf(fp,"        _G__getaryconstruct=%s.G__getaryconstruct\n",G__CINTLIBNAME);
  fprintf(fp,"        _G__gettempbufpointer=%s.G__gettempbufpointer\n",G__CINTLIBNAME);
  fprintf(fp,"        _G__setsizep2memfunc=%s.G__setsizep2memfunc\n",G__CINTLIBNAME);
  fprintf(fp,"        _G__getsizep2memfunc=%s.G__getsizep2memfunc\n",G__CINTLIBNAME);
  fprintf(fp,"        _G__get_linked_tagnum=%s.G__get_linked_tagnum\n",G__CINTLIBNAME);
  fprintf(fp,"        _G__tagtable_setup=%s.G__tagtable_setup\n",G__CINTLIBNAME);
  fprintf(fp,"        _G__search_tagname=%s.G__search_tagname\n",G__CINTLIBNAME);
  fprintf(fp,"        _G__search_typename=%s.G__search_typename\n",G__CINTLIBNAME);
  fprintf(fp,"        _G__defined_typename=%s.G__defined_typename\n",G__CINTLIBNAME);
  fprintf(fp,"        _G__tag_memvar_setup=%s.G__tag_memvar_setup\n",G__CINTLIBNAME);
  fprintf(fp,"        _G__memvar_setup=%s.G__memvar_setup\n",G__CINTLIBNAME);
  fprintf(fp,"        _G__tag_memvar_reset=%s.G__tag_memvar_reset\n",G__CINTLIBNAME);
  fprintf(fp,"        _G__tag_memfunc_setup=%s.G__tag_memfunc_setup\n",G__CINTLIBNAME);
  fprintf(fp,"        _G__memfunc_setup=%s.G__memfunc_setup\n",G__CINTLIBNAME);
  fprintf(fp,"        _G__memfunc_next=%s.G__memfunc_next\n",G__CINTLIBNAME);
  fprintf(fp,"        _G__memfunc_para_setup=%s.G__memfunc_para_setup\n",G__CINTLIBNAME);
  fprintf(fp,"        _G__tag_memfunc_reset=%s.G__tag_memfunc_reset\n",G__CINTLIBNAME);
  fprintf(fp,"        _G__letint=%s.G__letint\n",G__CINTLIBNAME);
  fprintf(fp,"        _G__letdouble=%s.G__letdouble\n",G__CINTLIBNAME);
  fprintf(fp,"        _G__store_tempobject=%s.G__store_tempobject\n",G__CINTLIBNAME);
  fprintf(fp,"        _G__inheritance_setup=%s.G__inheritance_setup\n",G__CINTLIBNAME);
  fprintf(fp,"        _G__add_compiledheader=%s.G__add_compiledheader\n",G__CINTLIBNAME);
  fprintf(fp,"        _G__add_ipath=%s.G__add_ipath\n",G__CINTLIBNAME);
  fprintf(fp,"        _G__add_macro=%s.G__add_macro\n",G__CINTLIBNAME);
  fprintf(fp
	  ,"        _G__check_setup_version=%s.G__check_setup_version\n",G__CINTLIBNAME);
  fprintf(fp,"        _G__int=%s.G__int\n",G__CINTLIBNAME);
  fprintf(fp,"        _G__double=%s.G__double\n",G__CINTLIBNAME);
  fprintf(fp,"        _G__calc=%s.G__calc\n",G__CINTLIBNAME);
  fprintf(fp,"        _G__loadfile=%s.G__loadfile\n",G__CINTLIBNAME);
  fprintf(fp,"        _G__unloadfile=%s.G__unloadfile\n",G__CINTLIBNAME);
  fprintf(fp,"        _G__init_cint=%s.G__init_cint\n",G__CINTLIBNAME);
  fprintf(fp,"        _G__scratch_all=%s.G__scratch_all\n",G__CINTLIBNAME);
  fprintf(fp,"        _G__setdouble=%s.G__setdouble\n",G__CINTLIBNAME);
  fprintf(fp,"        _G__setint=%s.G__setint\n",G__CINTLIBNAME);
  fprintf(fp,"        _G__stubstoreenv=%s.G__stubstoreenv\n",G__CINTLIBNAME);
  fprintf(fp,"        _G__stubrestoreenv=%s.G__stubrestoreenv\n",G__CINTLIBNAME);
  fprintf(fp,"        _G__getstream=%s.G__getstream\n",G__CINTLIBNAME);
  fprintf(fp,"        _G__type2string=%s.G__type2string\n",G__CINTLIBNAME);
  fprintf(fp,"        _G__alloc_tempobject=%s.G__alloc_tempobject\n",G__CINTLIBNAME);
  fprintf(fp,"        _G__set_p2fsetup=%s.G__set_p2fsetup\n",G__CINTLIBNAME);
  fprintf(fp,"        _G__free_p2fsetup=%s.G__free_p2fsetup\n",G__CINTLIBNAME);
  fprintf(fp,"        _G__search_typename2=%s.G__search_typename2\n",G__CINTLIBNAME);
  fprintf(fp,"\n");
#endif /* G__VISUAL */
  fprintf(fp,"EXPORTS\n");
  if(G__CPPLINK==G__globalcomp) {
    fprintf(fp,"        G__cpp_dllrev%s @%d\n",G__DLLID,++G__nexports);
    fprintf(fp,"        G__set_cpp_environment%s @%d\n",G__DLLID,++G__nexports);
    fprintf(fp,"        G__cpp_setup_tagtable%s @%d\n",G__DLLID,++G__nexports);
    fprintf(fp,"        G__cpp_setup_inheritance%s @%d\n",G__DLLID,++G__nexports);
    fprintf(fp,"        G__cpp_setup_typetable%s @%d\n",G__DLLID,++G__nexports);
    fprintf(fp,"        G__cpp_setup_memvar%s @%d\n",G__DLLID,++G__nexports);
    fprintf(fp,"        G__cpp_setup_memfunc%s @%d\n",G__DLLID,++G__nexports);
    fprintf(fp,"        G__cpp_setup_global%s @%d\n",G__DLLID,++G__nexports);
    fprintf(fp,"        G__cpp_setup_func%s @%d\n",G__DLLID,++G__nexports);
    fprintf(fp,"        G__cpp_setup%s @%d\n",G__DLLID,++G__nexports);
    if(G__multithreadlibcint) 
      fprintf(fp,"        G__SetCppCintApiPointers @%d\n",++G__nexports);
  }
  else {
    fprintf(fp,"        G__c_dllrev%s @%d\n",G__DLLID,++G__nexports);
    fprintf(fp,"        G__set_c_environment%s @%d\n",G__DLLID,++G__nexports);
    fprintf(fp,"        G__c_setup_tagtable%s @%d\n",G__DLLID,++G__nexports);
    fprintf(fp,"        G__c_setup_typetable%s @%d\n",G__DLLID,++G__nexports);
    fprintf(fp,"        G__c_setup_memvar%s @%d\n",G__DLLID,++G__nexports);
    fprintf(fp,"        G__c_setup_global%s @%d\n",G__DLLID,++G__nexports);
    fprintf(fp,"        G__c_setup_func%s @%d\n",G__DLLID,++G__nexports);
    fprintf(fp,"        G__c_setup%s @%d\n",G__DLLID,++G__nexports);
    if(G__multithreadlibcint) 
      fprintf(fp,"        G__SetCCintApiPointers @%d\n",++G__nexports);
  }
}
#endif /* G__GENWINDEF */

/**************************************************************************
* G__set_globalcomp()
*
*
**************************************************************************/
void G__set_globalcomp(mode,linkfilename ,dllid)
char *mode;
char *linkfilename;
char *dllid;
{
  FILE *fp;
  char buf[G__LONGLINE];
  char linkfilepref[G__LONGLINE];
  char linkfilepostf[20];
  char *p;

  strcpy(linkfilepref,linkfilename);
  p = strrchr(linkfilepref,'/'); /* ../aaa/bbb/ccc.cxx */
#ifdef G__WIN32
  if (!p) p = strrchr(linkfilepref,'\\'); /* in case of Windows pathname */
#endif
  if (!p) p = linkfilepref;      /*  /ccc.cxx */
  p = strrchr (p, '.');          /*  .cxx     */
  if(p) {
    strcpy(linkfilepostf,p+1);
    *p = '\0';
  }
  else {
    sprintf(linkfilepostf,"C");
  }

  G__globalcomp = atoi(mode); /* this is redundant */
  if(abs(G__globalcomp)>=10) {
     G__default_link = abs(G__globalcomp)%10;
     G__globalcomp /= 10;
  }
  G__store_globalcomp=G__globalcomp;

  strcpy(G__DLLID,G__map_cpp_name(dllid));

    if(0==strncmp(linkfilename,"G__cpp_",7)) 
      strcpy(G__NEWID,G__map_cpp_name(linkfilename+7));
    else if(0==strncmp(linkfilename,"G__",3)) 
      strcpy(G__NEWID,G__map_cpp_name(linkfilename+3));
    else
      strcpy(G__NEWID,G__map_cpp_name(linkfilename));

  switch(G__globalcomp) {
  case G__CPPLINK:
    sprintf(buf,"%s.h",linkfilepref);
    G__CPPLINK_H = (char*)malloc(strlen(buf)+1);
    strcpy(G__CPPLINK_H,buf);

    sprintf(buf,"%s.%s",linkfilepref,linkfilepostf);
    G__CPPLINK_C = (char*)malloc(strlen(buf)+1);
    strcpy(G__CPPLINK_C,buf);

#ifdef G__GENWINDEF
    sprintf(buf,"%s.def",G__PROJNAME);
    G__WINDEF = (char*)malloc(strlen(buf)+1);
    strcpy(G__WINDEF,buf);
    G__write_windef_header();
#endif

    fp = fopen(G__CPPLINK_C,"w");
    if(!fp) G__fileerror(G__CPPLINK_C);
    fprintf(fp,"/********************************************************\n");
    fprintf(fp,"* %s\n",G__CPPLINK_C);
    fprintf(fp,"* CAUTION: DON'T CHANGE THIS FILE. THIS FILE IS AUTOMATICALLY GENERATED\n");
    fprintf(fp,"*          FROM HEADER FILES LISTED IN G__setup_cpp_environmentXXX().\n");
    fprintf(fp,"*          CHANGE THOSE HEADER FILES AND REGENERATE THIS FILE.\n");
    fprintf(fp,"********************************************************/\n");
    fprintf(fp,"#include \"%s\"\n",G__CPPLINK_H);

    fprintf(fp,"\n");
    fprintf(fp,"#ifdef G__MEMTEST\n");
    fprintf(fp,"#undef malloc\n");
    fprintf(fp,"#undef free\n");
    fprintf(fp,"#endif\n");
    fprintf(fp,"\n");

    fprintf(fp,"extern \"C\" void G__cpp_reset_tagtable%s();\n",G__DLLID);

    fprintf(fp,"\nextern \"C\" void G__set_cpp_environment%s() {\n",G__DLLID);
    fclose(fp);
    FileWaitTime(10000);
    break;
  case G__CLINK:
    sprintf(buf,"%s.h",linkfilepref);
    G__CLINK_H = (char*)malloc(strlen(buf)+1);
    strcpy(G__CLINK_H,buf);

    sprintf(buf,"%s.c",linkfilepref);
    G__CLINK_C = (char*)malloc(strlen(buf)+1);
    strcpy(G__CLINK_C,buf);

#ifdef G__GENWINDEF
    sprintf(buf,"%s.def",G__PROJNAME);
    G__WINDEF = (char*)malloc(strlen(buf)+1);
    strcpy(G__WINDEF,buf);
    G__write_windef_header();
#endif

    fp = fopen(G__CLINK_C,"w");
    if(!fp) G__fileerror(G__CLINK_C);
    fprintf(fp,"/********************************************************\n");
    fprintf(fp,"* %s\n",G__CLINK_C);
    fprintf(fp,"********************************************************/\n");
    fprintf(fp,"#include \"%s\"\n",G__CLINK_H);
    fprintf(fp,"void G__c_reset_tagtable%s();\n",G__DLLID);
    fprintf(fp,"void G__set_c_environment%s() {\n",G__DLLID);
    fclose(fp);
    FileWaitTime(10000);
    break;
  }
}

/**************************************************************************
* G__gen_headermessage()
*
**************************************************************************/
static void G__gen_headermessage(fp,fname)
FILE *fp;
char *fname;
{
  fprintf(fp,"/********************************************************************\n");
  fprintf(fp,"* %s\n",fname);
  fprintf(fp,"* CAUTION: DON'T CHANGE THIS FILE. THIS FILE IS AUTOMATICALLY GENERATED\n");
  fprintf(fp,"*          FROM HEADER FILES LISTED IN G__setup_cpp_environmentXXX().\n");
  fprintf(fp,"*          CHANGE THOSE HEADER FILES AND REGENERATE THIS FILE.\n");
  fprintf(fp,"********************************************************************/\n");
  fprintf(fp,"#ifdef __CINT__\n");
  fprintf(fp,"#error %s/C is only for compilation.\n"
	  ,fname);
  fprintf(fp,"#endif\n");
}

/**************************************************************************
* G__gen_linksystem()
*
**************************************************************************/
int G__gen_linksystem(headerfile)
char *headerfile;
{
  FILE *fp;

  /* if(G__autoload_stdheader) return(0); */

  switch(G__globalcomp) {
  case G__CPPLINK: /* C++ link */
    fp = fopen(G__CPPLINK_C,"a");
    break;
  case G__CLINK:   /* C link */
    fp = fopen(G__CLINK_C,"a");
    break;
  default: 
    return(0);
  }
  fprintf(fp,"  G__add_compiledheader(\"<%s\");\n",headerfile);
  fclose(fp);
  FileWaitTime(10000);

  return(0);
}

/**************************************************************************
* G__gen_cppheader()
*
*
**************************************************************************/
void G__gen_cppheader(headerfilein)
char *headerfilein;
{
  FILE *fp;
  static char hdrpost[10]="";
  char headerfile[G__ONELINE];
  char* p;

  switch(G__globalcomp) {
  case G__CPPLINK: /* C++ link */
  case G__CLINK:   /* C link */
    break;
  default: 
    return;
  }

  if(headerfilein) {
    /*************************************************************
    * if header file is already created
    *************************************************************/

    strcpy(headerfile,headerfilein);
    /*************************************************************
    * if preprocessed file xxx.i is given rename as xxx.h
    *************************************************************/
    if(strlen(headerfile)>2 &&
       (strcmp(".i",headerfile+strlen(headerfile)-2)==0 ||
	strcmp(".I",headerfile+strlen(headerfile)-2)==0)) {
      if('\0'==hdrpost[0]) {
	switch(G__globalcomp) {
	case G__CPPLINK: /* C++ link */
	  strcpy(hdrpost,G__getmakeinfo1("CPPHDRPOST"));
	  break;
	case G__CLINK: /* C link */
	  strcpy(hdrpost,G__getmakeinfo1("CHDRPOST"));
	  break;
	}
      }
      strcpy(headerfile+strlen(headerfile)-2,hdrpost);
    }

    /* backslash escape sequence */
    p=strchr(headerfile,'\\');
    if(p) {
      char temp2[G__ONELINE];
      int i=0,j=0;
      while(headerfile[i]) {
	switch(headerfile[i]) {
	case '\\':
	  temp2[j++] = headerfile[i];
	  temp2[j++] = headerfile[i++];
	  break;
	default:
	  temp2[j++] = headerfile[i++];
	  break;
	}
      }
      temp2[j]='\0';
      strcpy(headerfile,temp2);
    }
    
#ifdef G__ROOT
    /* if (!strstr(headerfile,"LinkDef.h")&&!strstr(headerfile,"Linkdef.h") &&
       !strstr(headerfile,"linkdef.h")) { */
    if (!((strstr(headerfile,"LinkDef") || strstr(headerfile,"Linkdef") ||
           strstr(headerfile,"linkdef")) && strstr(headerfile,".h"))) {
#endif
    /* if(strstr(headerfile,".h")||strstr(headerfile,".H")) { */
      switch(G__globalcomp) {
      case G__CPPLINK:
	fp = fopen(G__CPPLINK_H,"a");
	if(!fp) G__fileerror(G__CPPLINK_H);
	fprintf(fp,"#include \"%s\"\n",headerfile);
	fclose(fp);
	FileWaitTime(10000);
	fp = fopen(G__CPPLINK_C,"a");
	if(!fp) G__fileerror(G__CPPLINK_C);
	fprintf(fp,"  G__add_compiledheader(\"%s\");\n",headerfile);
	fclose(fp);
	FileWaitTime(10000);
	break;
      case G__CLINK:
	fp = fopen(G__CLINK_H,"a");
	if(!fp) G__fileerror(G__CLINK_H);
	fprintf(fp,"#include \"%s\"\n",headerfile);
	fclose(fp);
	FileWaitTime(10000);
	fp = fopen(G__CLINK_C,"a");
	if(!fp) G__fileerror(G__CLINK_C);
	fprintf(fp,"  G__add_compiledheader(\"%s\");\n",headerfile);
	fclose(fp);
	FileWaitTime(10000);
	break;
      }
    /* } */
#ifdef G__ROOT
    }
#endif
  }

  else {
    /*************************************************************
    * if header file is not created yet
    *************************************************************/
    switch(G__globalcomp) {
    case G__CPPLINK:
      fp = fopen(G__CPPLINK_H,"w");
      if(!fp) G__fileerror(G__CPPLINK_H);
      G__gen_headermessage(fp,G__CPPLINK_H);
      G__cpplink_header(fp);
      fclose(fp);
      FileWaitTime(10000);
      break;
    case G__CLINK:
      fp = fopen(G__CLINK_H,"w");
      if(!fp) G__fileerror(G__CLINK_H);
      G__gen_headermessage(fp,G__CLINK_H);
      G__clink_header(fp);
      fclose(fp);
      FileWaitTime(10000);
      break;
    }
  }
}

/**************************************************************************
* G__add_compiledheader()
*
**************************************************************************/
void G__add_compiledheader(headerfile)
char *headerfile;
{
  if(headerfile && headerfile[0]=='<' && G__autoload_stdheader ) {
    int store_tagnum = G__tagnum;
    int store_def_tagnum = G__def_tagnum;
    int store_tagdefining = G__tagdefining;
    int store_def_struct_member = G__def_struct_member;
#ifdef G__OLDIMPLEMENTATION1284_YET
    if(G__def_struct_member && 'n'==G__struct.type[G__tagdefining]) {
      G__def_struct_member = 1;
    }
    else {
      G__tagnum = -1;
      G__def_tagnum = -1;
      G__tagdefining = -1;
      G__def_struct_member = 0;
    }
#else
    G__tagnum = -1;
    G__def_tagnum = -1;
    G__tagdefining = -1;
    G__def_struct_member = 0;
#endif
    G__loadfile(headerfile+1);
    G__tagnum = store_tagnum;
    G__def_tagnum = store_def_tagnum;
    G__tagdefining = store_tagdefining;
    G__def_struct_member = store_def_struct_member;
  }
}

/**************************************************************************
* G__add_macro()
*
**************************************************************************/
void G__add_macro(macroin)
char *macroin;
{
  char temp[G__LONGLINE];
  FILE *fp;
  char *p;
  char macro[G__LONGLINE];
  int store_tagnum = G__tagnum;
  int store_def_tagnum = G__def_tagnum;
  int store_tagdefining = G__tagdefining;
  int store_def_struct_member = G__def_struct_member;
  int store_var_type = G__var_type;
  struct G__var_array *store_p_local = G__p_local;
  G__tagnum = -1;
  G__def_tagnum = -1;
  G__tagdefining = -1;
  G__def_struct_member = 0;
  G__var_type = 'p';
  G__p_local = (struct G__var_array*)0;
  
  strcpy(macro,macroin);
  G__definemacro=1;
  if((p=strchr(macro,'='))) {
    if(G__cpp && '"'==*(p+1)) {
      G__add_quotation(p+1,temp);
      strcpy(p+1,temp+1);
      macro[strlen(macro)-1]=0;
    }
    else {
      strcpy(temp,macro);
    }
  }
  else {
    sprintf(temp,"%s=1",macro);
  }
  G__getexpr(temp);
  G__definemacro=0;

  sprintf(temp,"-D%s ",macro);
  p = strstr(G__macros,temp);
  /*   " -Dxxx -Dyyy -Dzzz"
   *       p  ^              */
  if(p) goto end_add_macro;
  strcpy(temp,G__macros);
  if(strlen(temp)+strlen(macro)+3>G__LONGLINE) {
    if(G__dispmsg>=G__DISPWARN) {
      G__fprinterr(G__serr,"Warning: can not add any more macros in the list\n");
      G__printlinenum();
    }
  }
  else {
    sprintf(G__macros,"%s-D%s ",temp,macro);
  }

  switch(G__globalcomp) {
  case G__CPPLINK:
    fp=fopen(G__CPPLINK_C,"a");
    if(!fp) G__fileerror(G__CPPLINK_C);
    fprintf(fp,"  G__add_macro(\"%s\");\n",macro);
    fclose(fp);
    FileWaitTime(10000);
    break;
  case G__CLINK:
    fp=fopen(G__CLINK_C,"a");
    if(!fp) G__fileerror(G__CLINK_C);
    fprintf(fp,"  G__add_macro(\"%s\");\n",macro);
    fclose(fp);
    FileWaitTime(10000);
    break;
  }
 end_add_macro:
  G__tagnum = store_tagnum;
  G__def_tagnum = store_def_tagnum;
  G__tagdefining = store_tagdefining;
  G__def_struct_member = store_def_struct_member;
  G__var_type = store_var_type;
  G__p_local = store_p_local;
}

/**************************************************************************
* G__add_ipath()
*
**************************************************************************/
void G__add_ipath(path)
char *path;
{
  struct G__includepath *ipath;
  char temp[G__ONELINE];
  FILE *fp;
  char *p;
  char *store_allincludepath;

  /* strip double quotes if exist */
  if('"'==path[0]) {
    strcpy(temp,path+1);
    if('"'==temp[strlen(temp)-1]) temp[strlen(temp)-1]='\0';
  }
  else {
    strcpy(temp,path);
  }

  /* to the end of list */
  ipath = &G__ipathentry;
  while(ipath->next) {
    if(ipath->pathname&&strcmp(ipath->pathname,temp)==0) return;
    ipath = ipath->next;
  }

  /* G__allincludepath will be given to real preprocessor */
  if(!G__allincludepath) {
    G__allincludepath = (char*)malloc(1);
    G__allincludepath[0] = '\0';
  }
  store_allincludepath = realloc((void*)G__allincludepath
				 ,strlen(G__allincludepath)+strlen(temp)+6);
  if(store_allincludepath) {
    int i=0,flag=0;
    while(temp[i]) if(isspace(temp[i++])) flag=1;
    G__allincludepath = store_allincludepath;
    if(flag)
      sprintf(G__allincludepath+strlen(G__allincludepath) ,"-I\"%s\" ",temp);
    else
      sprintf(G__allincludepath+strlen(G__allincludepath) ,"-I%s ",temp);
  }
  else {
    G__genericerror("Internal error: memory allocation failed for includepath buffer");
  }


  /* copy the path name */
  ipath->pathname = (char *)malloc((size_t)(strlen(temp)+1));
  strcpy(ipath->pathname,temp);

  /* allocate next entry */
  ipath->next=(struct G__includepath *)malloc(sizeof(struct G__includepath));
  ipath->next->next=(struct G__includepath *)NULL;
  ipath->next->pathname = (char *)NULL;

  /* backslash escape sequence */
  p=strchr(temp,'\\');
  if(p) {
    char temp2[G__ONELINE];
    int i=0,j=0;
    while(temp[i]) {
      switch(temp[i]) {
      case '\\':
	temp2[j++] = temp[i];
	temp2[j++] = temp[i++];
	break;
      default:
	temp2[j++] = temp[i++];
	break;
      }
    }
    temp2[j]='\0';
    strcpy(temp,temp2);
  }

  /* output include path information to interface routine */
  switch(G__globalcomp) {
  case G__CPPLINK:
    fp=fopen(G__CPPLINK_C,"a");
    if(!fp) G__fileerror(G__CPPLINK_C);
    fprintf(fp,"  G__add_ipath(\"%s\");\n",temp);
    fclose(fp);
    FileWaitTime(10000);
    break;
  case G__CLINK:
    fp=fopen(G__CLINK_C,"a");
    if(!fp) G__fileerror(G__CLINK_C);
    fprintf(fp,"  G__add_ipath(\"%s\");\n",temp);
    fclose(fp);
    FileWaitTime(10000);
    break;
  }
}


/**************************************************************************
* G__delete_ipath()
*
**************************************************************************/
int G__delete_ipath(path)
char *path;
{
  struct G__includepath *ipath;
  struct G__includepath *previpath;
  char temp[G__ONELINE];
  char temp2[G__ONELINE];
  int i=0,flag=0;
  char *p;

  /* strip double quotes if exist */
  if('"'==path[0]) {
    strcpy(temp,path+1);
    if('"'==temp[strlen(temp)-1]) temp[strlen(temp)-1]='\0';
  }
  else {
    strcpy(temp,path);
  }

  /* to the end of list */
  ipath = &G__ipathentry;
  previpath = (struct G__includepath*)NULL;
  while(ipath->next) {
    if(ipath->pathname&&strcmp(ipath->pathname,temp)==0) {
      /* delete this entry */
      free((void*)ipath->pathname);
      ipath->pathname=(char*)NULL;
      if(previpath) {
        previpath->next = ipath->next;
        free((void*)ipath);
      }
      else if(ipath->next) {
        G__ipathentry.pathname = calloc(1,1);
      }
      else {
        free((void*)G__ipathentry.pathname);
        G__ipathentry.pathname=(char*)NULL;
      }
      break;
    }
    previpath=ipath;
    ipath = ipath->next;
  }

  /* G__allincludepath will be given to real preprocessor */
  if(!G__allincludepath) return(0);
  i=0;
  while(temp[i]) if(isspace(temp[i++])) flag=1;
  if(flag) sprintf(temp2,"-I\"%s\" ",temp);
  else     sprintf(temp2,"-I%s ",temp);

  p = strstr(G__allincludepath,temp2);
  if(p) {
    char *p2 = p+strlen(temp2);
    while(*p2) *p++ = *p2++;
    *p = *p2;
    return(1);
  }

  return(0);
}




/**************************************************************************
**************************************************************************
* Generate C++ function access entry function
**************************************************************************
**************************************************************************/


/**************************************************************************
* G__isnonpublicnew()
*
**************************************************************************/
int G__isnonpublicnew(tagnum)
int tagnum;
{
  int i;
  int hash;
  char *namenew = "operator new";
  struct G__ifunc_table *ifunc;

  G__hash(namenew,hash,i);
  ifunc = G__struct.memfunc[tagnum];

  while(ifunc) {
    for(i=0;i<ifunc->allifunc;i++) {
      if(hash==ifunc->hash[i] && strcmp(ifunc->funcname[i],namenew)==0) {
	 if(G__PUBLIC!=ifunc->access[i]) return(1);
       }
    }
    ifunc = ifunc->next;
  }
  return(0);
}

/**************************************************************************
* G__cppif_memfunc() working
*
*
**************************************************************************/
void G__cppif_memfunc(fp,hfp)
FILE *fp;
FILE *hfp;
{
#ifndef G__SMALLOBJECT
  int i,j;
  struct G__ifunc_table *ifunc;
  int isconstructor,iscopyconstructor,isdestructor,isassignmentoperator;
  int isnonpublicnew;

  fprintf(fp,"\n/*********************************************************\n");
  fprintf(fp,"* Member function Interface Method\n");
  fprintf(fp,"*********************************************************/\n");

  for(i=0;i<G__struct.alltag;i++) {
    if(
       (G__CPPLINK==G__struct.globalcomp[i]||
	G__CLINK==G__struct.globalcomp[i]
	|| G__ONLYMETHODLINK==G__struct.globalcomp[i]
	) &&
       (-1==(int)G__struct.parent_tagnum[i]
	|| G__nestedclass
	)
       &&
       -1!=G__struct.line_number[i]&&G__struct.hash[i]&&
       '$'!=G__struct.name[i][0] && 'e'!=G__struct.type[i]) {
      ifunc = G__struct.memfunc[i];
      isconstructor=0;
      iscopyconstructor=0;
      isdestructor=0;
      isassignmentoperator=0;
      isnonpublicnew=G__isnonpublicnew(i);

      /* member function interface */
      fprintf(fp,"\n/* %s */\n",G__fulltagname(i,0));

      while(ifunc) {
	for(j=0;j<ifunc->allifunc;j++) {
	  if(G__PUBLIC==ifunc->access[j]
	     || (G__PROTECTED==ifunc->access[j] && 
		 (G__PROTECTEDACCESS&G__struct.protectedaccess[i]))
	     || (G__PRIVATEACCESS&G__struct.protectedaccess[i])
	     ) {
	    if(G__ONLYMETHODLINK==G__struct.globalcomp[i]&&
	       G__METHODLINK!=ifunc->globalcomp[j]) continue;
#ifndef G__OLDIMPLEMENTATION2039
	    if(0==ifunc->hash[j]) continue;
#endif
#ifndef G__OLDIMPLEMENTATION1656
	    if(ifunc->pentry[j]->size<0) continue; /* already precompiled */
#endif
	    if(strcmp(ifunc->funcname[j],G__struct.name[i])==0) {
	      /* constructor need special handling */
	      if(0==G__struct.isabstract[i]&&0==isnonpublicnew
		 )
		G__cppif_genconstructor(fp,hfp,i,j,ifunc);
	      ++isconstructor;
	      if(ifunc->para_nu[j]>=1&&
		 'u'==ifunc->para_type[j][0]&&
		 i==ifunc->para_p_tagtable[j][0]&&
		 G__PARAREFERENCE==ifunc->para_reftype[j][0]&&
		 (1==ifunc->para_nu[j]||ifunc->para_default[j][1])) {
		++iscopyconstructor;
	      }
	    }
	    else if('~'==ifunc->funcname[j][0]) {
	      /* destructor is created in gendefault later */
	      if(G__PUBLIC==ifunc->access[j]) isdestructor = -1;
	      else ++isdestructor;
	      continue;
	    }
#ifndef G__OLDIMPLEMENTATION2039
	    else if('\0'==ifunc->funcname[j][0] && j==0) {
	      /* this must be the place holder for the destructor.
	       * let's skip it! */
	      continue;
	    }
#endif
	    else {
#ifdef G__DEFAULTASSIGNOPR
	      if(strcmp(ifunc->funcname[j],"operator=")==0
		 && 'u'==ifunc->para_type[j][0] 
		 && i==ifunc->para_p_tagtable[j][0]
		  ) {
		++isassignmentoperator;
	      }
#endif
	      G__cppif_genfunc(fp,hfp,i,j,ifunc);
	    }
	  } /* if PUBLIC */
	  else { /* if PROTECTED or PRIVATE */
	    if(strcmp(ifunc->funcname[j],G__struct.name[i])==0) {
	      ++isconstructor;
	      if(
		 ifunc->para_nu[j]>0 &&
		 'u'==ifunc->para_type[j][0]&&i==ifunc->para_p_tagtable[j][0]&&
		 G__PARAREFERENCE==ifunc->para_reftype[j][0]&&
		 (1==ifunc->para_nu[j]||ifunc->para_default[j][1])) {
		++iscopyconstructor;
	      }
	    }
	    else if('~'==ifunc->funcname[j][0]) {
	      ++isdestructor;
	    }
	    else if(strcmp(ifunc->funcname[j],"operator new")==0) {
	      ++isconstructor;
	      ++iscopyconstructor;
	    }
	    else if(strcmp(ifunc->funcname[j],"operator delete")==0) {
	      ++isdestructor;
	    }
#ifdef G__DEFAULTASSIGNOPR
	    else if(strcmp(ifunc->funcname[j],"operator=")==0
		    && 'u'==ifunc->para_type[j][0] 
		    && i==ifunc->para_p_tagtable[j][0]
		) {
	      ++isassignmentoperator;
	    }
#endif
	  }
	} /* for(j) */
	if(NULL==ifunc->next
#ifndef G__OLDIMPLEMENTATON1656
	   && G__NOLINK==G__struct.iscpplink[i]
#endif
	   && G__ONLYMETHODLINK!=G__struct.globalcomp[i]
	   )
	  G__cppif_gendefault(fp,hfp,i,j,ifunc
			      ,isconstructor
			      ,iscopyconstructor
			      ,isdestructor
			      ,isassignmentoperator
			      ,isnonpublicnew);
	ifunc=ifunc->next;
      } /* while(ifunc) */
    } /* if(globalcomp) */
  } /* for(i) */
#endif
}

/**************************************************************************
* G__cppif_func() working
*
*
**************************************************************************/
void G__cppif_func(fp,hfp)
FILE *fp;
FILE *hfp;
{
  int j;
  struct G__ifunc_table *ifunc;

  fprintf(fp,"\n/* Setting up global function */\n");
  ifunc = &G__ifunc;

  /* member function interface */
  while(ifunc) {
    for(j=0;j<ifunc->allifunc;j++) {
      if(G__NOLINK>ifunc->globalcomp[j] &&
	 G__PUBLIC==ifunc->access[j] &&
	 ifunc->hash[j]) {

	G__cppif_genfunc(fp,hfp,-1,j,ifunc);

      } /* if(access) */
    } /* for(j) */
    ifunc=ifunc->next;
  } /* while(ifunc) */
}

/**************************************************************************
* G__cppif_dummyfuncname()
*
**************************************************************************/
void G__cppif_dummyfuncname(fp)
FILE *fp;
{
#ifndef G__IF_DUMMY
  fprintf(fp,"   return(1);\n");
#else
  fprintf(fp,"   return(1 || funcname || hash || result7 || libp) ;\n");
#endif
}

/**************************************************************************
*  G__if_ary_union()
*
**************************************************************************/
void  G__if_ary_union(fp,ifn,ifunc)
FILE *fp;
int ifn;
struct G__ifunc_table *ifunc;
{
  int k,m;
  char *p;

  m = ifunc->para_nu[ifn];

  for(k=0;k<m;k++) {
    if(ifunc->para_name[ifn][k]) {
      p = strchr(ifunc->para_name[ifn][k],'[');
      if(p) {
	fprintf(fp,"  struct G__aRyp%d {%s a[1]%s;} *G__Ap%d=(struct G__aRyp%d*)G__int(libp->para[%d]);\n"
		,k
		,G__type2string(ifunc->para_type[ifn][k]
				,ifunc->para_p_tagtable[ifn][k]
				,ifunc->para_p_typetable[ifn][k]
				,0
				,0
				)
		,p+2,k,k,k);
      }
    }
  }
}

/**************************************************************************
*  G__if_ary_union_reset()
*
**************************************************************************/
void  G__if_ary_union_reset(ifn,ifunc)
int ifn;
struct G__ifunc_table *ifunc;
{
  int k,m;
  int type;
  char *p;

  m = ifunc->para_nu[ifn];

  for(k=0;k<m;k++) {
    if(ifunc->para_name[ifn][k]) {
      p = strchr(ifunc->para_name[ifn][k],'[');
      if(p) {
	int pointlevel=1;
	*p = 0;
	while((p=strchr(p+1,'['))) ++pointlevel;
	type=ifunc->para_type[ifn][k];
	if(isupper(type)) {
	  switch(pointlevel) {
	  case 2:
	    ifunc->para_reftype[ifn][k] = G__PARAP2P2P;
	    break;
	  default:
	    G__genericerror("Cint internal error ary parameter dimension");
	    break;
	  }
	}
	else {
	  ifunc->para_type[ifn][k]=toupper(type);
	  switch(pointlevel) {
	  case 2:
	    ifunc->para_reftype[ifn][k] = G__PARAP2P;
	    break;
	  case 3:
	    ifunc->para_reftype[ifn][k] = G__PARAP2P2P;
	    break;
	  default:
	    G__genericerror("Cint internal error ary parameter dimension");
	    break;
	  }
	}
      }
    }
  }
}

#ifdef G__CPPIF_EXTERNC
/**************************************************************************
* G__p2f_typedefname
**************************************************************************/
char* G__p2f_typedefname(ifn,page,k)
int ifn;
short page;
int k;
{
  static char buf[G__ONELINE];
  sprintf(buf,"G__P2F%d_%d_%d%s",ifn,page,k,G__PROJNAME);
  return(buf);
}

/**************************************************************************
* G__p2f_typedef
**************************************************************************/
void G__p2f_typedef(fp,ifn,ifunc) 
FILE *fp;
int ifn;
struct G__ifunc_table *ifunc;
{
  char buf[G__LONGLINE];
  char *p;
  int k;
  if(G__CPPLINK!=G__globalcomp) return;
  for(k=0;k<ifunc->para_nu[ifn];k++) {
    /*DEBUG*/ printf("%s %d\n",ifunc->funcname[ifn],k);
    if(
#ifndef G__OLDIMPLEMENTATION2191
       '1'==ifunc->para_type[ifn][k]
#else
       'Q'==ifunc->para_type[ifn][k]
#endif
       ) {
      strcpy(buf,G__type2string(ifunc->para_type[ifn][k],
				ifunc->para_p_tagtable[ifn][k],
				ifunc->para_p_typetable[ifn][k],0,
				ifunc->para_isconst[ifn][k]));
      /*DEBUG*/ printf("buf=%s\n",buf);
      p = strstr(buf,"(*)(");
      if(p) {
	p += 2;
	*p = 0;
	fprintf(fp,"typedef %s%s",buf,G__p2f_typedefname(ifn,ifunc->page,k));
	*p = ')';
	fprintf(fp,"%s;\n",p);
      }
    }
  }
}
#endif

/**************************************************************************
* G__isprotecteddestructoronelevel()
*
**************************************************************************/
static int G__isprotecteddestructoronelevel(tagnum)
int tagnum;
{
  char *dtorname;
  struct G__ifunc_table *ifunc;
  int ifn;
  ifunc=G__struct.memfunc[tagnum];
  dtorname = malloc(strlen(G__struct.name[tagnum])+2);
  dtorname[0]='~';
  strcpy(dtorname+1,G__struct.name[tagnum]);
  do {
    for(ifn=0;ifn<ifunc->allifunc;ifn++) {
      if(strcmp(dtorname,ifunc->funcname[ifn])==0) {
	if(G__PRIVATE==ifunc->access[ifn]||G__PROTECTED==ifunc->access[ifn]) {
	  free((void*)dtorname);
	  return(1);
	}
      }
    }
    ifunc=ifunc->next;
  } while(ifunc);
  free((void*)dtorname);
  return(0);
}

/**************************************************************************
* G__cppif_genconstructor()
*
* Constructor must be separately handled because constructors can not
* be called on existing memory area unless appropreate new operator is
* overloaded.
*
**************************************************************************/
void G__cppif_genconstructor(fp,hfp,tagnum,ifn,ifunc)
FILE *fp;
FILE *hfp;
int tagnum,ifn;
struct G__ifunc_table *ifunc;
{
#ifndef G__SMALLOBJECT
  int k,m;
  int isprotecteddtor = G__isprotecteddestructoronelevel(tagnum);
  char buf[G__LONGLINE]; /* 1481 */

  G__ASSERT( tagnum != -1 );
#ifndef G__OLDIMPLEMENTATION1911
  if(0 && hfp) return;
#endif

  if(G__PROTECTED==ifunc->access[ifn]
     || G__PRIVATE==ifunc->access[ifn]
     ) 
    sprintf(buf,"%s_PR",G__get_link_tagname(tagnum));
  else
    strcpy(buf,G__fulltagname(tagnum,1));


#ifdef G__CPPIF_EXTERNC
  G__p2f_typedef(fp,ifn,ifunc) ;
#endif

#ifdef G__CPPIF_STATIC
  fprintf(fp,"static int %s(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash)"
	  ,G__map_cpp_funcname(tagnum,G__struct.name[tagnum],ifn,ifunc->page));
#else /* G__CPPIF_STATIC */
#ifdef G__GENWINDEF
  fprintf(G__WINDEFfp,"        %s @%d\n"
	  ,G__map_cpp_funcname(tagnum,G__struct.name[tagnum],ifn,ifunc->page)
	  ,++G__nexports);
#endif
  fprintf(hfp,"extern \"C\" int %s(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash);\n"
	  ,G__map_cpp_funcname(tagnum,G__struct.name[tagnum],ifn,ifunc->page));

  fprintf(fp,"extern \"C\" int %s(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash)"
	  ,G__map_cpp_funcname(tagnum,G__struct.name[tagnum],ifn,ifunc->page));
#endif /* G__CPPIF_STATIC */
  fprintf(fp," {\n");
#if  !defined(G__BORLAND)
  fprintf(fp,"   %s *p=NULL;\n",G__type2string('u',tagnum,-1,0,0));
#else
  fprintf(fp,"   %s *p;\n",G__type2string('u',tagnum,-1,0,0));
#endif
#ifndef G__VAARG_COPYFUNC
  if(2==ifunc->ansi[ifn]) {
    fprintf(fp,"  G__va_arg_buf G__va_arg_bufobj;\n");
    fprintf(fp,"  G__va_arg_put(&G__va_arg_bufobj,libp,%d);\n"
	    ,ifunc->para_nu[ifn]);
  }
#endif

  G__if_ary_union(fp,ifn,ifunc);

  m = ifunc->para_nu[ifn] ;

  /* compact G__cpplink.C */
  if(m>0 && ifunc->para_default[ifn][m-1]) {
    fprintf(fp,"   switch(libp->paran) {\n");
    do {
      if(m>=0) fprintf(fp,"   case %d:\n",m);
      else     fprintf(fp,"   case 0:\n");

      if(0==m) {
	if(0==(G__is_operator_newdelete&G__NOT_USING_2ARG_NEW)) {
	  fprintf(fp,"   if(G__getaryconstruct())\n");
	  fprintf(fp,"     if(G__PVOID==G__getgvp())\n");
	  if(isprotecteddtor) {
	    fprintf(fp,"       {p=0;G__genericerror(\"Error: Array construction with private/protected destructor is illegal\");}\n");
	  }
	  else {
	    fprintf(fp,"       p=new %s[G__getaryconstruct()];\n" ,buf);
	  }
	  fprintf(fp,"     else {\n");
	  fprintf(fp,"       for(int i=0;i<G__getaryconstruct();i++)\n");
	  if(G__is_operator_newdelete&G__DUMMYARG_NEWDELETE
	     && 0==(G__struct.funcs[tagnum]&G__HAS_OPERATORNEW2ARG)
	     ) 
	    fprintf(fp,"         p=::new((G__%s_tag*)(G__getgvp()+sizeof(%s)*i)) "
		    ,G__NEWID,buf);
	  else
	    fprintf(fp,"         p=new((void*)(G__getgvp()+sizeof(%s)*i)) " ,buf);
	  fprintf(fp,"%s;\n",buf);
	  fprintf(fp,"       p=(%s*)G__getgvp();\n",buf);
	  fprintf(fp,"     }\n");
	  if(G__is_operator_newdelete&G__DUMMYARG_NEWDELETE 
	     && 0==(G__struct.funcs[tagnum]&G__HAS_OPERATORNEW2ARG)
	     )
	    fprintf(fp,"   else p=::new((G__%s_tag*)G__getgvp()) %s;\n" 
		    ,G__NEWID,buf);
	  else
	    fprintf(fp,"   else p=new((void*)G__getgvp()) %s;\n" ,buf);
	}
	else {
	  if(isprotecteddtor) {
	    fprintf(fp,"   if(G__getaryconstruct()) {p=0;G__genericerror(\"Error: Array construction with private/protected destructor is illegal\");}\n");
	  }
	  else {
	    fprintf(fp,"   if(G__getaryconstruct()) p=new %s[G__getaryconstruct()];\n" ,buf);
	  }
	  if(G__is_operator_newdelete&G__DUMMYARG_NEWDELETE 
	     && 0==(G__struct.funcs[tagnum]&G__HAS_OPERATORNEW1ARG)
	     )
	    fprintf(fp,"   else p=::new((G__%s_tag*)G__getgvp()) %s;\n" 
		    ,G__NEWID,buf);
	  else
	    fprintf(fp,"   else                    p=new %s;\n" ,buf);
	}
      }
      else {
	if(0==(G__is_operator_newdelete&G__NOT_USING_2ARG_NEW)) {
	  if(G__is_operator_newdelete&G__DUMMYARG_NEWDELETE 
	     && 0==(G__struct.funcs[tagnum]&G__HAS_OPERATORNEW2ARG)
	     )
	    fprintf(fp,"      p = ::new((G__%s_tag*)G__getgvp()) %s("
		    ,G__NEWID,buf);
	  else
	    fprintf(fp,"      p = new((void*)G__getgvp()) %s(",buf);
	}
	else {
	  if(G__is_operator_newdelete&G__DUMMYARG_NEWDELETE 
	     && 0==(G__struct.funcs[tagnum]&G__HAS_OPERATORNEW1ARG)
	     )
	    fprintf(fp,"      p = ::new((G__%s_tag*)G__getgvp()) %s("
		    ,G__NEWID,buf);
	  else
	    fprintf(fp,"      p = new %s(",buf);
	}
	if(m>2) fprintf(fp,"\n");
	for(k=0;k<m;k++) G__cppif_paratype(fp,ifn,ifunc,k);
	if(2==ifunc->ansi[ifn]) {
#if defined(G__VAARG_COPYFUNC)
	  fprintf(fp,",libp,%d",k);
#elif defined(__hpux)
	  int i;
	  for(i=G__VAARG_SIZE/sizeof(long)-1;i>G__VAARG_SIZE/sizeof(long)-100;i--)	
	    fprintf(fp,",G__va_arg_bufobj.x.i[%d]",i);
#elif defined(__sparc) || defined(__sparc__) || defined(__SUNPRO_C)
	  int i;
	  for(i=0;i<100 /* G__VAARG_SIZE/4 */;i++)	
	    fprintf(fp,",G__va_arg_bufobj.x.i[%d]",i);
#elif (defined(__PPC__)||defined(__ppc__))&&(defined(_AIX)||defined(__APPLE__))
	  int i;
	  for(i=0;i<100;i++)	
	    fprintf(fp,",G__va_arg_bufobj.x.i[%d]",i);
#else
	  fprintf(fp,",G__va_arg_bufobj");
#endif
	}
	fprintf(fp,");\n");
      }

      fprintf(fp,"      break;\n");
      --m;
    } while(m>=0 && ifunc->para_default[ifn][m]);
    fprintf(fp,"   }\n");
  }
  else {
    if(0==m) {
      if(0==(G__is_operator_newdelete&G__NOT_USING_2ARG_NEW)) {
	fprintf(fp,"   if(G__getaryconstruct())\n");
	fprintf(fp,"     if(G__PVOID==G__getgvp())\n");
	if(isprotecteddtor) {
	  fprintf(fp,"       {p=0;G__genericerror(\"Error: Array construction with private/protected destructor is illegal\");}\n");
	}
	else {
	  fprintf(fp,"       p=new %s[G__getaryconstruct()];\n" ,buf);
	}
	fprintf(fp,"     else {\n");
	fprintf(fp,"       for(int i=0;i<G__getaryconstruct();i++)\n");
	if(G__is_operator_newdelete&G__DUMMYARG_NEWDELETE 
	     && 0==(G__struct.funcs[tagnum]&G__HAS_OPERATORNEW2ARG)
	   )
	  fprintf(fp,"         p=::new((G__%s_tag*)(G__getgvp()+sizeof(%s)*i)) " 
		  ,G__NEWID,buf);
	else
	  fprintf(fp,"         p=new((void*)(G__getgvp()+sizeof(%s)*i)) " ,buf);
	fprintf(fp,"%s;\n",buf);
	fprintf(fp,"       p=(%s*)G__getgvp();\n",buf);
	fprintf(fp,"     }\n");
	if(G__is_operator_newdelete&G__DUMMYARG_NEWDELETE 
	     && 0==(G__struct.funcs[tagnum]&G__HAS_OPERATORNEW2ARG)
	   )
	  fprintf(fp,"   else p=::new((G__%s_tag*)G__getgvp()) %s;\n" 
		  ,G__NEWID,buf);
	else
	  fprintf(fp,"   else p=new((void*)G__getgvp()) %s;\n" ,buf);
      }
      else {
	if(isprotecteddtor) {
	  fprintf(fp,"   if(G__getaryconstruct()) {p=0;G__genericerror(\"Error: Array construction with private/protected destructor is illegal\");}\n");
	}
	else {
	  fprintf(fp ,"   if(G__getaryconstruct()) p=new %s[G__getaryconstruct()];\n" ,buf);
	}
	if(G__is_operator_newdelete&G__DUMMYARG_NEWDELETE 
	     && 0==(G__struct.funcs[tagnum]&G__HAS_OPERATORNEW1ARG)
	   )
	  fprintf(fp,"   else p=::new((G__%s_tag*)G__getgvp()) %s;\n" 
		  ,G__NEWID,buf);
	else
	  fprintf(fp,"   else                    p=new %s;\n" ,buf);}
    }
    else {
      if(0==(G__is_operator_newdelete&G__NOT_USING_2ARG_NEW)) {
	if(G__is_operator_newdelete&G__DUMMYARG_NEWDELETE 
	     && 0==(G__struct.funcs[tagnum]&G__HAS_OPERATORNEW2ARG)
	   )
	  fprintf(fp,"      p=::new((G__%s_tag*)G__getgvp()) %s("
		  ,G__NEWID,buf);
	else
	  fprintf(fp,"      p=new((void*)G__getgvp()) %s(",buf);
      }
      else {
	if(G__is_operator_newdelete&G__DUMMYARG_NEWDELETE 
	     && 0==(G__struct.funcs[tagnum]&G__HAS_OPERATORNEW1ARG)
	   )
	  fprintf(fp,"      p=::new((G__%s_tag*)G__getgvp()) %s("
		  ,G__NEWID,buf);
	else
	  fprintf(fp,"      p = new %s(",buf);
      }
      if(m>2) fprintf(fp,"\n");
      for(k=0;k<m;k++) G__cppif_paratype(fp,ifn,ifunc,k);
      if(2==ifunc->ansi[ifn]) {
#if defined(G__VAARG_COPYFUNC)
	fprintf(fp,",libp,%d",k);
#elif defined(__hpux)
	int i;
	for(i=G__VAARG_SIZE/sizeof(long)-1;i>G__VAARG_SIZE/sizeof(long)-100;i--)	
	  fprintf(fp,",G__va_arg_bufobj.x.i[%d]",i);
#elif defined(__sparc) || defined(__sparc__) || defined(__SUNPRO_C)
	int i;
	for(i=0;i<100 /* G__VAARG_SIZE/4 */;i++)	
	  fprintf(fp,",G__va_arg_bufobj.x.i[%d]",i);
#elif (defined(__PPC__)||defined(__ppc__))&&(defined(_AIX)||defined(__APPLE__))
	int i;
	for(i=0;i<100;i++)	
	  fprintf(fp,",G__va_arg_bufobj.x.i[%d]",i);
#else
	fprintf(fp,",G__va_arg_bufobj");
#endif
      }
      fprintf(fp,");\n");
    }

  }

  fprintf(fp,"      result7->obj.i = (long)p;\n");
  fprintf(fp,"      result7->ref = (long)p;\n");
  fprintf(fp,"      result7->type = 'u';\n");
  fprintf(fp,"      result7->tagnum = G__get_linked_tagnum(&%s);\n"
	  ,G__mark_linked_tagnum(tagnum));
  G__if_ary_union_reset(ifn,ifunc);
  G__cppif_dummyfuncname(fp);
  fprintf(fp,"}\n\n");
#endif
}

/**************************************************************************
* G__isprivateconstructorifunc()
*
**************************************************************************/
static int G__isprivateconstructorifunc(tagnum,iscopy)
int tagnum;
int iscopy;
{
  struct G__ifunc_table *ifunc;
  int ifn;
  ifunc=G__struct.memfunc[tagnum];
  do {
    for(ifn=0;ifn<ifunc->allifunc;ifn++) {
      if(strcmp(G__struct.name[tagnum],ifunc->funcname[ifn])==0) {
	if(iscopy) { /* Check copy constructor */
	  if((1<=ifunc->para_nu[ifn]&&'u'==ifunc->para_type[ifn][0]&&
	      tagnum==ifunc->para_p_tagtable[ifn][0]) &&
	     (1==ifunc->para_nu[ifn]||ifunc->para_default[ifn][1])
	     && G__PRIVATE==ifunc->access[ifn]
	     ) {
	    return(1);
	  }
	}
	else { /* Check default constructor */
	  if((0==ifunc->para_nu[ifn]||ifunc->para_default[ifn][0])
	     && G__PRIVATE==ifunc->access[ifn]
	     ) {
	    return(1);
	  }
	  /* Following solution may not be perfect */
	  if((1<=ifunc->para_nu[ifn]&&'u'==ifunc->para_type[ifn][0]&&
	      tagnum==ifunc->para_p_tagtable[ifn][0]) &&
	     (1==ifunc->para_nu[ifn]||ifunc->para_default[ifn][1])
	     &&G__PRIVATE==ifunc->access[ifn]
	     ) {
	    return(1);
	  }
	}
      }
      else if(strcmp("operator new",ifunc->funcname[ifn])==0) {
	if(G__PRIVATE==ifunc->access[ifn]||G__PROTECTED==ifunc->access[ifn])
	  return(1);
      }
    }
    ifunc=ifunc->next;
  } while(ifunc);
  return(0);
}

#ifndef __CINT__
static int G__isprivateconstructorclass G__P((int tagnum,int iscopy));
int G__isprivateconstructor G__P((int tagnum,int iscopy));
#endif
/**************************************************************************
* G__isprivateconstructorvar()
*
* check if private constructor exists in this particular class
**************************************************************************/
static int G__isprivateconstructorvar(tagnum,iscopy)
int tagnum;
int iscopy;
{
  int ig15;
  struct G__var_array *var;
  int memtagnum;
  var=G__struct.memvar[tagnum];
  while(var) {
    for(ig15=0;ig15<var->allvar;ig15++) {
      if('u'==var->type[ig15] && -1!=(memtagnum=var->p_tagtable[ig15]) &&
	 'e'!=G__struct.type[memtagnum]	
	 && memtagnum!=tagnum
	 ) {
	if(G__isprivateconstructorclass(memtagnum,iscopy)) return(1);
      }
    }
    var=var->next;
  }
  return(0);
}

/**************************************************************************
* G__isprivateconstructorclass()
*
* check if private constructor exists in this particular class
**************************************************************************/
static int G__isprivateconstructorclass(tagnum,iscopy)
int tagnum;
int iscopy;
{
  int t,f;
  if(iscopy) {
    t=G__CTORDTOR_PRIVATECOPYCTOR;
    f=G__CTORDTOR_NOPRIVATECOPYCTOR;
  }
  else {
    t=G__CTORDTOR_PRIVATECTOR;
    f=G__CTORDTOR_NOPRIVATECTOR;
  }
  if(G__ctordtor_status[tagnum]&t) return(1);
  if(G__ctordtor_status[tagnum]&f) return(0);
  if(G__isprivateconstructorifunc(tagnum,iscopy)||
     G__isprivateconstructor(tagnum,iscopy)
     ) {
    G__ctordtor_status[tagnum]|=t;
    return(1);
  }
  G__ctordtor_status[tagnum]|=f;
  return(0);
}

/**************************************************************************
* G__isprivateconstructor()
*
* check if private constructor exists in base class or class of member obj
**************************************************************************/
int G__isprivateconstructor(tagnum,iscopy)
int tagnum;
int iscopy;
{
  int basen;
  int basetagnum;
  struct G__inheritance *baseclass;

  baseclass = G__struct.baseclass[tagnum];

  /* Check base class private constructor */
  for(basen=0;basen<baseclass->basen;basen++) {
    basetagnum = baseclass->basetagnum[basen];
    if(G__isprivateconstructorclass(basetagnum,iscopy)) return(1);
  }

  /* Check Data member object */
  if(G__isprivateconstructorvar(tagnum,iscopy)) return(1);

  return(0);
}


/**************************************************************************
* G__isprivatedestructorifunc()
*
**************************************************************************/
static int G__isprivatedestructorifunc(tagnum)
int tagnum;
{
  char *dtorname;
  struct G__ifunc_table *ifunc;
  int ifn;
  ifunc=G__struct.memfunc[tagnum];
  dtorname = malloc(strlen(G__struct.name[tagnum])+2);
  dtorname[0]='~';
  strcpy(dtorname+1,G__struct.name[tagnum]);
  do {
    for(ifn=0;ifn<ifunc->allifunc;ifn++) {
      if(strcmp(dtorname,ifunc->funcname[ifn])==0) {
	if(G__PRIVATE==ifunc->access[ifn]) {
	  free((void*)dtorname);
	  return(1);
	}
      }
      else if(strcmp("operator delete",ifunc->funcname[ifn])==0) {
	if(G__PRIVATE==ifunc->access[ifn]||G__PROTECTED==ifunc->access[ifn]) {
	  free((void*)dtorname);
	  return(1);
	}
      }
    }
    ifunc=ifunc->next;
  } while(ifunc);
  free((void*)dtorname);
  return(0);
}

#ifndef __CINT__
static int G__isprivatedestructorclass G__P((int tagnum));
int G__isprivatedestructor G__P((int tagnum));
#endif
/**************************************************************************
* G__isprivatedestructorvar()
*
* check if private destructor exists in this particular class
**************************************************************************/
static int G__isprivatedestructorvar(tagnum)
int tagnum;
{
  int ig15;
  struct G__var_array *var;
  int memtagnum;
  var=G__struct.memvar[tagnum];
  while(var) {
    for(ig15=0;ig15<var->allvar;ig15++) {
      if('u'==var->type[ig15] && -1!=(memtagnum=var->p_tagtable[ig15]) &&
	 'e'!=G__struct.type[memtagnum]
	 && memtagnum!=tagnum
	 ) {
	if(G__isprivatedestructorclass(memtagnum)) return(1);
      }
    }
    var=var->next;
  }
  return(0);
}

/**************************************************************************
* G__isprivatedestructorclass()
*
* check if private destructor exists in this particular class
**************************************************************************/
static int G__isprivatedestructorclass(tagnum)
int tagnum;
{
  int t,f;
  t=G__CTORDTOR_PRIVATEDTOR;
  f=G__CTORDTOR_NOPRIVATEDTOR;
  if(G__ctordtor_status[tagnum]&t) return(1);
  if(G__ctordtor_status[tagnum]&f) return(0);
  if(G__isprivatedestructorifunc(tagnum)||
     G__isprivatedestructor(tagnum)
     ) {
    G__ctordtor_status[tagnum]|=t;
    return(1);
  }
  G__ctordtor_status[tagnum]|=f;
  return(0);
}
/**************************************************************************
* G__isprivatedestructor()
*
* check if private destructor exists in base class or class of member obj
**************************************************************************/
int G__isprivatedestructor(tagnum)
int tagnum;
{
  int basen;
  int basetagnum;
  struct G__inheritance *baseclass;

  baseclass = G__struct.baseclass[tagnum];

  /* Check base class private destructor */
  for(basen=0;basen<baseclass->basen;basen++) {
    basetagnum = baseclass->basetagnum[basen];
    if(G__isprivatedestructorclass(basetagnum)) {
      return(1);
    }
  }

  /* Check Data member object */
  if(G__isprivatedestructorvar(tagnum)) return(1);

  return(0);
}

#ifdef G__DEFAULTASSIGNOPR
/**************************************************************************
* G__isprivateassignoprifunc()
*
**************************************************************************/
static int G__isprivateassignoprifunc(tagnum)
int tagnum;
{
  struct G__ifunc_table *ifunc;
  int ifn;
  ifunc=G__struct.memfunc[tagnum];
  do {
    for(ifn=0;ifn<ifunc->allifunc;ifn++) {
      if(strcmp("operator=",ifunc->funcname[ifn])==0) {
	if((G__PRIVATE==ifunc->access[ifn]||G__PROTECTED==ifunc->access[ifn])
	   && 'u'==ifunc->para_type[ifn][0] 
           && tagnum==ifunc->para_p_tagtable[ifn][0]
	    ) {
	  return(1);
	}
      }
    }
    ifunc=ifunc->next;
  } while(ifunc);
  return(0);
}

#ifndef __CINT__
static int G__isprivateassignoprclass G__P((int tagnum));
int G__isprivateassignopr G__P((int tagnum));
#endif
/**************************************************************************
* G__isprivateassignoprvar()
*
* check if private assignopr exists in this particular class
**************************************************************************/
static int G__isprivateassignoprvar(tagnum)
int tagnum;
{
  int ig15;
  struct G__var_array *var;
  int memtagnum;
  var=G__struct.memvar[tagnum];
  while(var) {
    for(ig15=0;ig15<var->allvar;ig15++) {
      if('u'==var->type[ig15] && -1!=(memtagnum=var->p_tagtable[ig15]) &&
	 'e'!=G__struct.type[memtagnum]
	 && memtagnum!=tagnum
	 ) {
	if(G__isprivateassignoprclass(memtagnum)) return(1);
      }
      if(G__PARAREFERENCE==var->reftype[ig15] && 
	G__LOCALSTATIC!=var->statictype[ig15]) {
	return(1);
      }
      if(var->constvar[ig15] &&
        G__LOCALSTATIC!=var->statictype[ig15]) {
	return(1);
      }
    }
    var=var->next;
  }
  return(0);
}

/**************************************************************************
* G__isprivateassignoprclass()
*
* check if private assignopr exists in this particular class
**************************************************************************/
static int G__isprivateassignoprclass(tagnum)
int tagnum;
{
  int t,f;
  t=G__CTORDTOR_PRIVATEASSIGN;
  f=G__CTORDTOR_NOPRIVATEASSIGN;
  if(G__ctordtor_status[tagnum]&t) return(1);
  if(G__ctordtor_status[tagnum]&f) return(0);
  if(G__isprivateassignoprifunc(tagnum)||G__isprivateassignopr(tagnum)) {
    G__ctordtor_status[tagnum]|=t;
    return(1);
  }
  G__ctordtor_status[tagnum]|=f;
  return(0);
}
/**************************************************************************
* G__isprivateassignopr()
*
* check if private assignopr exists in base class or class of member obj
**************************************************************************/
int G__isprivateassignopr(tagnum)
int tagnum;
{
  int basen;
  int basetagnum;
  struct G__inheritance *baseclass;

  baseclass = G__struct.baseclass[tagnum];

  /* Check base class private assignopr */
  for(basen=0;basen<baseclass->basen;basen++) {
    basetagnum = baseclass->basetagnum[basen];
    if(G__isprivateassignoprclass(basetagnum)) {
      return(1);
    }
  }

  /* Check Data member object */
  if(G__isprivateassignoprvar(tagnum)) return(1);

  return(0);
}
#endif


/**************************************************************************
* G__cppif_gendefault()
*
* Create default constructor and destructor. If default constructor is
* given in the header file, the interface function created here for
* the default constructor will be redundant and won't be used.
*
* Copy constructor and operator=(), if not explisitly specified in the
* header file, are handled as memberwise copy by cint parser. Validity of
* this handling is questionalble especially when base class has explicit
* copy constructor or operator=().
*
**************************************************************************/
void G__cppif_gendefault(fp,hfp,tagnum,ifn,ifunc
			 ,isconstructor
			 ,iscopyconstructor
			 ,isdestructor
			 ,isassignmentoperator
			 ,isnonpublicnew)
FILE *fp;
FILE *hfp;
int tagnum,ifn;
struct G__ifunc_table *ifunc;
int isconstructor;
int iscopyconstructor;
int isdestructor;
int isassignmentoperator;
int isnonpublicnew;
{
#ifndef G__SMALLOBJECT
  /* int k,m; */
  int page;
#define G__OLDIMPLEMENtATION1972
#ifndef G__OLDIMPLEMENtATION1972
  char buf1[G__MAXNAME];
  char buf2[G__MAXNAME];
  char buf3[G__MAXNAME];
  char *funcname=buf1;
  char *temp=buf2;
  char *dtorname=buf3;
#else
  char funcname[G__MAXNAME*6];
  char temp[G__MAXNAME*6];
#endif
  int isprotecteddtor = G__isprotecteddestructoronelevel(tagnum);
#ifdef G__OLDIMPLEMENtATION1972
  char dtorname[G__LONGLINE];
#endif

  G__ASSERT( tagnum != -1 );

#ifndef G__OLDIMPLEMENTATION1911
  if(0 && hfp && isassignmentoperator) return;
#endif

  if('n'==G__struct.type[tagnum]) return;

  page = ifunc->page;
  if(ifn==G__MAXIFUNC) {
    ifn=0;
    ++page;
  }

#ifndef G__OLDIMPLEMENtATION1972
  if(strlen(G__struct.name[tagnum])>G__MAXNAME-2) {
    funcname = (char*)malloc(strlen(G__struct.name[tagnum])+5);
    dtorname = (char*)malloc(strlen(G__struct.name[tagnum])+5);
  }
  if(strlen(G__fulltagname(tagnum,1))>G__MAXNAME-2) {
    dtorname = (char*)malloc(strlen(G__fulltagname(tagnum,1))+5);
  }
#endif

  /*********************************************************************
  * default constructor
  *********************************************************************/
  if(0==isconstructor) isconstructor=G__isprivateconstructor(tagnum,0);
  if(0==isconstructor&&0==G__struct.isabstract[tagnum]&&0==isnonpublicnew){

    sprintf(funcname,"%s",G__struct.name[tagnum]);
    fprintf(fp,"// automatic default constructor\n");

#ifdef G__CPPIF_STATIC
    fprintf(fp,"static int %s(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash)"
	    ,G__map_cpp_funcname(tagnum ,funcname ,ifn,page));
#else /* G__CPPIF_STATIC */
#ifdef G__GENWINDEF
    fprintf(G__WINDEFfp,"        %s @%d\n"
	    ,G__map_cpp_funcname(tagnum ,funcname ,ifn,page)
	    ,++G__nexports);
#endif
    fprintf(hfp,"extern \"C\" int %s(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash);\n"
	    ,G__map_cpp_funcname(tagnum ,funcname ,ifn,page));

    fprintf(fp,"extern \"C\" int %s(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash)"
	    ,G__map_cpp_funcname(tagnum ,funcname ,ifn,page));
#endif /* G__CPPIF_STATIC */
    fprintf(fp," {\n");
    fprintf(fp,"   %s *p;\n",G__fulltagname(tagnum,1));

    if(0==(G__is_operator_newdelete&G__NOT_USING_2ARG_NEW)) {
      fprintf(fp,"   if(G__getaryconstruct())\n");
      fprintf(fp,"     if(G__PVOID==G__getgvp())\n");
      if(isprotecteddtor) {
	fprintf(fp,"       {p=0;G__genericerror(\"Error: Array construction with private/protected destructor is illegal\");}\n");
      }
      else {
	fprintf(fp,"       p=new %s[G__getaryconstruct()];\n" ,G__fulltagname(tagnum,1));
      }
      fprintf(fp,"     else {\n");
      fprintf(fp,"       for(int i=0;i<G__getaryconstruct();i++)\n");
      if(G__is_operator_newdelete&G__DUMMYARG_NEWDELETE 
	 && 0==(G__struct.funcs[tagnum]&G__HAS_OPERATORNEW2ARG)
	 )
	fprintf(fp,"         p=::new((G__%s_tag*)(G__getgvp()+sizeof(%s)*i)) " 
		,G__NEWID,G__fulltagname(tagnum,1));
      else 
	fprintf(fp,"         p=new((void*)(G__getgvp()+sizeof(%s)*i)) " ,G__fulltagname(tagnum,1));
      fprintf(fp,"%s;\n",G__fulltagname(tagnum,1));
      fprintf(fp,"       p=(%s*)G__getgvp();\n",G__fulltagname(tagnum,1));
      fprintf(fp,"     }\n");
      if(G__is_operator_newdelete&G__DUMMYARG_NEWDELETE 
	 && 0==(G__struct.funcs[tagnum]&G__HAS_OPERATORNEW2ARG)
	 )
	fprintf(fp,"   else p=::new((G__%s_tag*)G__getgvp()) %s;\n" 
		,G__NEWID,G__fulltagname(tagnum,1));
      else
	fprintf(fp,"   else p=new((void*)G__getgvp()) %s;\n" ,G__fulltagname(tagnum,1));
    }
    else {
      if(isprotecteddtor) {
	fprintf(fp,"   if(G__getaryconstruct()) {p=0;G__genericerror(\"Error: Array construction with private/protected destructor is illegal\");}\n");
      }
      else {
	fprintf(fp,"   if(G__getaryconstruct()) p=new %s[G__getaryconstruct()];\n" ,G__fulltagname(tagnum,1));
      }
      if(G__is_operator_newdelete&G__DUMMYARG_NEWDELETE 
	 && 0==(G__struct.funcs[tagnum]&G__HAS_OPERATORNEW1ARG)
	 )
	fprintf(fp,"   else p=::new((G__%s_tag*)G__getgvp()) %s;\n" 
		,G__NEWID,G__fulltagname(tagnum,1));
      else
	fprintf(fp,"   else                    p=new %s;\n" ,G__fulltagname(tagnum,1));
    }
    fprintf(fp,"   result7->obj.i = (long)p;\n");
    fprintf(fp,"   result7->ref = (long)p;\n");
    fprintf(fp,"   result7->type = 'u';\n");
    fprintf(fp,"   result7->tagnum = G__get_linked_tagnum(&%s);\n"
	    ,G__mark_linked_tagnum(tagnum));
    G__cppif_dummyfuncname(fp);
    fprintf(fp,"}\n\n");

    ++ifn;
    if(ifn==G__MAXIFUNC) {
      ifn=0;
      ++page;
    }
  } /* if(isconstructor) */

  /*********************************************************************
  * copy constructor
  *********************************************************************/
  if(0==iscopyconstructor) iscopyconstructor=G__isprivateconstructor(tagnum,1);
  if(0==iscopyconstructor&&0==G__struct.isabstract[tagnum]&&0==isnonpublicnew){
    sprintf(funcname,"%s",G__struct.name[tagnum]);
    fprintf(fp,"// automatic copy constructor\n");

#ifdef G__CPPIF_STATIC
    fprintf(fp,"static int %s(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash)\n"
	    ,G__map_cpp_funcname(tagnum ,funcname ,ifn,page));
#else /* G__CPPIF_STATIC */
#ifdef G__GENWINDEF
    fprintf(G__WINDEFfp,"        %s @%d\n"
	    ,G__map_cpp_funcname(tagnum ,funcname ,ifn,page)
	    ,++G__nexports);
#endif
    fprintf(hfp,"extern \"C\" int %s(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash);\n"
	    ,G__map_cpp_funcname(tagnum ,funcname ,ifn,page));

    fprintf(fp,"extern \"C\" int %s(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash)\n"
	    ,G__map_cpp_funcname(tagnum ,funcname ,ifn,page));
#endif /* G__CPPIF_STATIC */
    fprintf(fp,"{\n");
    fprintf(fp,"   %s *p;\n",G__fulltagname(tagnum,1));

    strcpy(temp,G__fulltagname(tagnum,1));
    if(0==(G__is_operator_newdelete&G__NOT_USING_2ARG_NEW)) {
      if(G__is_operator_newdelete&G__DUMMYARG_NEWDELETE 
	 && 0==(G__struct.funcs[tagnum]&G__HAS_OPERATORNEW2ARG)
	 )
	fprintf(fp,"   p=::new((G__%s_tag*)G__getgvp()) %s(*(%s*)G__int(libp->para[0]));\n",G__NEWID,temp,temp);
      else
	fprintf(fp,"   p=new((void*)G__getgvp()) %s(*(%s*)G__int(libp->para[0]));\n",temp,temp);
    }
    else {
      fprintf(fp,"   void *xtmp = (void*)G__int(libp->para[0]);\n");
      fprintf(fp,"   p=new %s(*(%s*)xtmp);\n",temp,temp);
    }
    fprintf(fp,"   result7->obj.i = (long)p;\n");
    fprintf(fp,"   result7->ref = (long)p;\n");
    fprintf(fp,"   result7->type = 'u';\n");
    fprintf(fp,"   result7->tagnum = G__get_linked_tagnum(&%s);\n"
	    ,G__mark_linked_tagnum(tagnum));
    G__cppif_dummyfuncname(fp);
    fprintf(fp,"}\n\n");

    ++ifn;
    if(ifn==G__MAXIFUNC) {
      ifn=0;
      ++page;
    }
  }


  /*********************************************************************
  * destructor
  *********************************************************************/
  if(0>=isdestructor) isdestructor=G__isprivatedestructor(tagnum);
  if(0>=isdestructor
     && G__struct.type[tagnum]!='n'
     ) {
    sprintf(funcname,"~%s",G__struct.name[tagnum]);
    fprintf(fp,"// automatic destructor\n");

    if(G__is_operator_newdelete&G__DUMMYARG_NEWDELETE) {
      if(1 /* strchr(funcname,'<') */ ) {
	sprintf(dtorname,"G__T%s",G__map_cpp_name(G__fulltagname(tagnum,0)));
	fprintf(fp,"typedef %s %s;\n",G__fulltagname(tagnum,0),dtorname);
      }
      else strcpy(dtorname,funcname+1);
    }

#ifdef G__CPPIF_STATIC
    fprintf(fp,"static int %s(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash)"
	    ,G__map_cpp_funcname(tagnum ,funcname ,ifn,page));
#else /* G__CPPIF_STATIC */
#ifdef G__GENWINDEF
    fprintf(G__WINDEFfp,"        %s @%d\n"
	    ,G__map_cpp_funcname(tagnum ,funcname ,ifn,page)
	    ,++G__nexports);
#endif
    fprintf(hfp,"extern \"C\" int %s(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash);\n"
	    ,G__map_cpp_funcname(tagnum ,funcname ,ifn,page));

    fprintf(fp,"extern \"C\" int %s(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash)"
	    ,G__map_cpp_funcname(tagnum ,funcname ,ifn,page));
#endif /* G__CPPIF_STATIC */
    fprintf(fp," {\n");
    fprintf(fp,"   if(0==G__getstructoffset()) return(1);\n");
    fprintf(fp,"   if(G__getaryconstruct())\n");
    fprintf(fp,"     if(G__PVOID==G__getgvp())\n");
    fprintf(fp,"       delete[] (%s *)(G__getstructoffset());\n" ,G__fulltagname(tagnum,1));
    fprintf(fp,"     else\n");
    fprintf(fp,"       for(int i=G__getaryconstruct()-1;i>=0;i--)\n");
    if(G__is_operator_newdelete&G__DUMMYARG_NEWDELETE
       && 0==(G__struct.funcs[tagnum]&G__HAS_OPERATORDELETE)
       ) {
      fprintf(fp,"         ((%s *)",G__fulltagname(tagnum,1));
      fprintf(fp,"((G__getstructoffset())+sizeof(%s)*i))" ,G__fulltagname(tagnum,1));
      fprintf(fp,"->~%s();\n",dtorname);
    }
    else {
#ifndef G__N_EXPLICITDESTRUCTOR
      fprintf(fp,"         ((%s *)",G__fulltagname(tagnum,1));
      fprintf(fp,"((G__getstructoffset())+sizeof(%s)*i))" ,G__fulltagname(tagnum,1));
      fprintf(fp,"->~%s();\n",G__fulltagname(tagnum,1));
#else
      fprintf(fp,"         delete (%s *)",G__fulltagname(tagnum,1));
      fprintf(fp,"((G__getstructoffset())+sizeof(%s)*i);\n" ,G__fulltagname(tagnum,1));
#endif
    }
    if(G__is_operator_newdelete&G__DUMMYARG_NEWDELETE
       && 0==(G__struct.funcs[tagnum]&G__HAS_OPERATORDELETE)
       ) {
      fprintf(fp,"   else {\n");
      fprintf(fp,"     long G__Xtmp=G__getgvp();\n");
      fprintf(fp,"     G__setgvp(G__PVOID);\n");
      fprintf(fp,"     ((%s *)(G__getstructoffset()))",G__fulltagname(tagnum,1));
      fprintf(fp,"->~%s();\n",dtorname);
      fprintf(fp,"     G__setgvp(G__Xtmp);\n");
      fprintf(fp,"     G__operator_delete((void*)G__getstructoffset());\n");
      fprintf(fp,"   }\n");
    }
    else {
#ifndef G__N_EXPLICITDESTRUCTOR
      fprintf(fp,"   else if(G__PVOID==G__getgvp()) delete (%s *)(G__getstructoffset());\n"
	      ,G__fulltagname(tagnum,1));
      fprintf(fp,"   else ((%s *)(G__getstructoffset()))"
	      ,G__fulltagname(tagnum,1));
      fprintf(fp,"->~%s();\n",G__fulltagname(tagnum,1));
#else
      fprintf(fp,"   else  delete (%s *)(G__getstructoffset());\n"
	      ,G__fulltagname(tagnum,1));
#endif
    }
    fprintf(fp,"      G__setnull(result7);\n");
    G__cppif_dummyfuncname(fp);
    fprintf(fp,"}\n\n");

    ++ifn;
    if(ifn==G__MAXIFUNC) {
      ifn=0;
      ++page;
    }
  }


#ifdef G__DEFAULTASSIGNOPR
  /*********************************************************************
  * assignment operator
  *********************************************************************/
  if(0==isassignmentoperator) 
    isassignmentoperator=G__isprivateassignopr(tagnum);
  if(0==isassignmentoperator) {
    sprintf(funcname,"operator=");
    fprintf(fp,"// automatic assignment operator\n");

#ifdef G__CPPIF_STATIC
    fprintf(fp,"static int %s(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash)"
	    ,G__map_cpp_funcname(tagnum ,funcname ,ifn,page));
#else /* G__CPPIF_STATIC */
#ifdef G__GENWINDEF
    fprintf(G__WINDEFfp,"        %s @%d\n"
	    ,G__map_cpp_funcname(tagnum ,funcname ,ifn,page)
	    ,++G__nexports);
#endif
    fprintf(hfp,"extern \"C\" int %s(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash);\n"
	    ,G__map_cpp_funcname(tagnum ,funcname ,ifn,page));

    fprintf(fp,"extern \"C\" int %s(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash)"
	    ,G__map_cpp_funcname(tagnum ,funcname ,ifn,page));
#endif /* G__CPPIF_STATIC */
    fprintf(fp," {\n");
    strcpy(temp,G__type2string('u',tagnum,-1,0,0));
    fprintf(fp,"   %s *dest = (%s*)(G__getstructoffset());\n",temp,temp);
    if(1>=G__struct.size[tagnum] && 0==G__struct.memvar[tagnum]->allvar) {}
    else fprintf(fp,"   *dest = (*(%s*)libp->para[0].ref);\n",temp);
    fprintf(fp,"   const %s& obj = *dest;\n",temp);
    fprintf(fp,"   result7->ref=(long)(&obj); result7->obj.i=(long)(&obj);\n");
    G__cppif_dummyfuncname(fp);
    fprintf(fp,"}\n\n");

    ++ifn;
    if(ifn==G__MAXIFUNC) {
      ifn=0;
      ++page;
    }
  }
#endif

#ifndef G__OLDIMPLEMENtATION1972
    if(funcname!=buf1) free((void*)funcname);
    if(temp!=buf2) free((void*)temp);
    if(dtorname!=buf3) free((void*)dtorname);
#endif

#endif
}

/**************************************************************************
* G__cppif_genfunc()
*
**************************************************************************/
void G__cppif_genfunc(fp,hfp,tagnum,ifn,ifunc)
FILE *fp;
FILE *hfp;
int tagnum,ifn;
struct G__ifunc_table *ifunc;
{
#ifndef G__SMALLOBJECT
  int k,m;
#if !defined(G__OLDIMPLEMENTATION1823)
  char buf2[G__LONGLINE];
  char *endoffunc=buf2;
#else
  char endoffunc[G__LONGLINE];
#endif
#ifndef G__OLDIMPLEMENTATION1823
  char buf[G__BUFLEN*4];
  char *castname=buf;
#else
  char castname[G__ONELINE];
#endif

#ifndef G__OLDIMPLEMENTATION1911
  if(0 && hfp) return;
#endif

#ifndef G__OLDIMPLEMENTATION1823
  if(-1!=tagnum) {
    int len=strlen(G__fulltagname(tagnum,1));
    if(len>G__BUFLEN*4-30) {
      castname=(char*)malloc(len+30);
    }
    if(len>G__LONGLINE-256) {
      endoffunc=(char*)malloc(len+256);
    }
  }
#endif

#ifdef G__CPPIF_EXTERNC
  G__p2f_typedef(fp,ifn,ifunc) ;
#endif

#ifdef G__VAARG_COPYFUNC
  if(2==ifunc->ansi[ifn] && 0<ifunc->pentry[ifn]->line_number)
    G__va_arg_copyfunc(fp,ifunc,ifn);
#endif

#ifndef G__CPPIF_STATIC
#ifdef G__GENWINDEF
  fprintf(G__WINDEFfp,"        %s @%d\n"
	  ,G__map_cpp_funcname(tagnum,ifunc->funcname[ifn],ifn,ifunc->page)
	  ,++G__nexports);
#endif

  if(G__CPPLINK==G__globalcomp) {
    fprintf(hfp,"extern \"C\" int %s(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash);\n"
	    ,G__map_cpp_funcname(tagnum,ifunc->funcname[ifn],ifn,ifunc->page));
  }
  else {
    fprintf(hfp,"int %s();\n"
	    ,G__map_cpp_funcname(tagnum,ifunc->funcname[ifn],ifn,ifunc->page));
  }
#endif

#ifdef G__CPPIF_STATIC
  fprintf(fp,"static ");
#else
  if(G__CPPLINK==G__globalcomp) fprintf(fp,"extern \"C\" ");
#endif
  if(G__clock) {
    /* K&R style header */
    fprintf(fp,"int %s(result7,funcname,libp,hash)\n"
	    ,G__map_cpp_funcname(tagnum
				 ,ifunc->funcname[ifn]
				 ,ifn,ifunc->page));
    fprintf(fp,"G__value *result7;\n");
    fprintf(fp,"char *funcname;\n");
    fprintf(fp,"struct G__param *libp;\n");
    fprintf(fp,"int hash;\n");
  }
  else {
    /* ANSI style header */
    fprintf(fp,"int %s(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash)"
	    ,G__map_cpp_funcname(tagnum
				 ,ifunc->funcname[ifn]
				 ,ifn,ifunc->page));
  }

  fprintf(fp," {\n");

  G__if_ary_union(fp,ifn,ifunc);

#ifndef G__VAARG_COPYFUNC
  if(2==ifunc->ansi[ifn]) {
    fprintf(fp,"  G__va_arg_buf G__va_arg_bufobj;\n");
    fprintf(fp,"  G__va_arg_put(&G__va_arg_bufobj,libp,%d);\n"
	    ,ifunc->para_nu[ifn]);
  }
#endif

  m = ifunc->para_nu[ifn] ;

  /*************************************************************
  * compact G__cpplink.C
  *************************************************************/
  if(m>0 && ifunc->para_default[ifn][m-1]) {
    fprintf(fp,"   switch(libp->paran) {\n");
    do {
      if(m>=0) fprintf(fp,"   case %d:\n",m);
      else     fprintf(fp,"   case 0:\n");

      G__cppif_returntype(fp,ifn,ifunc,endoffunc);
      if(-1 != tagnum) {
	if('n'==G__struct.type[tagnum]) 
	  fprintf(fp,"%s::"
		  ,G__fulltagname(tagnum,1));
	else {
	  if(G__PROTECTED==ifunc->access[ifn]
	     || (G__PRIVATE==ifunc->access[ifn] &&
		 (G__PRIVATEACCESS&G__struct.protectedaccess[tagnum]))
	     ) 
	    sprintf(castname,"%s_PR",G__get_link_tagname(tagnum));
	  else 
	    strcpy(castname,G__fulltagname(tagnum,1));
          if(ifunc->staticalloc[ifn]) {
             fprintf(fp,"%s::",castname);
	  } else 
	  if(ifunc->isconst[ifn]&G__CONSTFUNC) 
	    fprintf(fp,"((const %s*)(G__getstructoffset()))->",castname);
	  else 
	    fprintf(fp,"((%s*)(G__getstructoffset()))->",castname);
	}
      }
      if(G__PROTECTED==ifunc->access[ifn]
	 || (G__PRIVATE==ifunc->access[ifn] &&
	     (G__PRIVATEACCESS&G__struct.protectedaccess[tagnum]))
	 ) 
	fprintf(fp,"G__PT_%s(",ifunc->funcname[ifn]);
      else
	fprintf(fp,"%s(",ifunc->funcname[ifn]);

      if(m>6) fprintf(fp,"\n");
      for(k=0;k<m;k++) G__cppif_paratype(fp,ifn,ifunc,k);
      if(2==ifunc->ansi[ifn]) {
#if defined(G__VAARG_COPYFUNC)
	fprintf(fp,",libp,%d",k);
#elif defined(__hpux)
	int i;
	for(i=G__VAARG_SIZE/sizeof(long)-1;i>G__VAARG_SIZE/sizeof(long)-100;i--)	
	  fprintf(fp,",G__va_arg_bufobj.x.i[%d]",i);
#elif defined(__sparc) || defined(__sparc__) || defined(__SUNPRO_C)
	int i;
	for(i=0;i<100 /* G__VAARG_SIZE/4 */;i++)	
	  fprintf(fp,",G__va_arg_bufobj.x.i[%d]",i);
#elif (defined(__PPC__)||defined(__ppc__))&&(defined(_AIX)||defined(__APPLE__))
	int i;
	for(i=0;i<100;i++)	
	  fprintf(fp,",G__va_arg_bufobj.x.i[%d]",i);
#else
	fprintf(fp,",G__va_arg_bufobj");
#endif
      }

      fprintf(fp,")%s\n",endoffunc);

      fprintf(fp,"      break;\n");
      --m;
    } while(m>=0 && ifunc->para_default[ifn][m]);
    fprintf(fp,"   }\n");
  }
  else {
    G__cppif_returntype(fp,ifn,ifunc,endoffunc);
    if(-1 != tagnum) {
      if('n'==G__struct.type[tagnum]) 
	fprintf(fp,"%s::",G__fulltagname(tagnum,1));
      else {
	if(G__PROTECTED==ifunc->access[ifn] 
	   || (G__PRIVATE==ifunc->access[ifn] &&
	       (G__PRIVATEACCESS&G__struct.protectedaccess[tagnum]))
	   )
	  sprintf(castname,"%s_PR",G__get_link_tagname(tagnum));
	else 
	  strcpy(castname,G__fulltagname(tagnum,1));
        if(ifunc->staticalloc[ifn]) { 
           fprintf(fp,"%s::",castname);
	} else 
	if(ifunc->isconst[ifn]&G__CONSTFUNC) 
	  fprintf(fp,"((const %s*)(G__getstructoffset()))->",castname);
	else 
	  fprintf(fp,"((%s*)(G__getstructoffset()))->",castname);
      }
    }
    if(G__PROTECTED==ifunc->access[ifn] 
	 || (G__PRIVATE==ifunc->access[ifn] &&
	     (G__PRIVATEACCESS&G__struct.protectedaccess[tagnum]))
       )
      fprintf(fp,"G__PT_%s(",ifunc->funcname[ifn]);
    else
      fprintf(fp,"%s(",ifunc->funcname[ifn]);

    if(m>6) fprintf(fp,"\n");
    for(k=0;k<m;k++) G__cppif_paratype(fp,ifn,ifunc,k);
    if(2==ifunc->ansi[ifn]) {
#if defined(G__VAARG_COPYFUNC)
      fprintf(fp,",libp,%d",k);
#elif defined(__hpux)
	int i;
	for(i=G__VAARG_SIZE/sizeof(long)-1;i>G__VAARG_SIZE/sizeof(long)-100;i--)	
	  fprintf(fp,",G__va_arg_bufobj.x.i[%d]",i);
#elif defined(__sparc) || defined(__sparc__) || defined(__SUNPRO_C)
	int i;
	for(i=0;i<100 /* G__VAARG_SIZE/4 */;i++)	
	  fprintf(fp,",G__va_arg_bufobj.x.i[%d]",i);
#elif (defined(__PPC__)||defined(__ppc__))&&(defined(_AIX)||defined(__APPLE__))
	int i;
	for(i=0;i<100;i++)	
	  fprintf(fp,",G__va_arg_bufobj.x.i[%d]",i);
#else
      fprintf(fp,",G__va_arg_bufobj");
#endif
    }

    fprintf(fp,")%s\n",endoffunc);
  }

  G__if_ary_union_reset(ifn,ifunc);
  G__cppif_dummyfuncname(fp);
  fprintf(fp,"}\n\n");
#endif

#ifndef G__OLDIMPLEMENTATION1823
  if(castname!=buf) free((void*)castname);
  if(endoffunc!=buf2) free((void*)endoffunc);
#endif
}

/**************************************************************************
* G__cppif_returntype()
*
**************************************************************************/
int G__cppif_returntype(fp,ifn,ifunc,endoffunc)
FILE *fp;
int ifn;
struct G__ifunc_table *ifunc;
char *endoffunc;
{
#ifndef G__SMALLOBJECT
  int type,tagnum,typenum,reftype,isconst;
#ifndef G__OLDIMPLEMENTATION1503
  int deftyp = -1;
#endif

  type = ifunc->type[ifn];
  tagnum = ifunc->p_tagtable[ifn];
  typenum = ifunc->p_typetable[ifn];
  reftype = ifunc->reftype[ifn];
  isconst = ifunc->isconst[ifn];

  /* Promote link-off typedef to link-on if used in function */
  if(-1!=typenum && G__NOLINK==G__newtype.globalcomp[typenum] &&
     G__NOLINK==G__newtype.iscpplink[typenum])
    G__newtype.globalcomp[typenum] = G__globalcomp;

  /* return type reference */
#ifdef G__OLDIMPLEMENTATION1859 /* questionable with 1859 */
  if(-1!=typenum&&G__PARAREFERENCE==G__newtype.reftype[typenum]) {
    reftype=G__PARAREFERENCE;
    typenum= -1;
  }
#endif
  if(G__PARAREFERENCE==reftype) {
    fprintf(fp,"      {\n");
    fprintf(fp,"        ");
    if(isconst&G__CONSTFUNC) {
      if(isupper(type)) isconst |= G__PCONSTVAR;
      else              isconst |= G__CONSTVAR;
    }
    if(islower(type) && !isconst) 
      fprintf(fp,"const %s obj=",G__type2string(type,tagnum,typenum,reftype,isconst));
    else
      fprintf(fp,"%s obj=",G__type2string(type,tagnum,typenum,reftype,isconst));
    if(G__newtype.nindex[typenum]) {
      sprintf(endoffunc ,";\n         result7->ref=(long)(&obj); result7->obj.i=(long)(obj);result7->type=%d;\n      }",toupper(type));
      return(0);
    }
    switch(type) {
    case 'd':
    case 'f':
      sprintf(endoffunc ,";\n         result7->ref=(long)(&obj); result7->obj.d=(double)(obj);\n      }");
      break;
    case 'u':
      if('e'==G__struct.type[tagnum])
	sprintf(endoffunc ,";\n         result7->ref=(long)(&obj); result7->obj.i=(long)(obj);\n      }");
      else
	sprintf(endoffunc ,";\n         result7->ref=(long)(&obj); result7->obj.i=(long)(&obj);\n      }");
      break;
    default:
      sprintf(endoffunc ,";\n         result7->ref=(long)(&obj); result7->obj.i=(long)(obj);\n      }");
      break;
    }
    return(0);
  }

  /* return type pointer */
  if(isupper(type)) {
    fprintf(fp,"   G__letint(result7,%d,(long)",type);
    if(reftype) 
      sprintf(endoffunc,");\n   result7->obj.reftype.reftype=%d;",reftype);
    else 
      sprintf(endoffunc,");");
    return(0);
  }

  /* return object body */
  switch(type) {
  case 'y':
    fprintf(fp,"      G__setnull(result7);\n");
    fprintf(fp,"      ");
    sprintf(endoffunc,";");
    return(0);
  case 'e':
  case 'c':
  case 's':
  case 'i':
  case 'l':
  case 'b':
  case 'r':
  case 'h':
  case 'k':
  case 'g':
    fprintf(fp,"      G__letint(result7,%d,(long)",type);
    sprintf(endoffunc,");");
    return(0);
  case 'n':
    fprintf(fp,"      G__letLonglong(result7,%d,(G__int64)",type);
    sprintf(endoffunc,");");
    return(0);
  case 'm':
    fprintf(fp,"      G__letULonglong(result7,%d,(G__uint64)",type);
    sprintf(endoffunc,");");
    return(0);
  case 'q':
    fprintf(fp,"      G__letLongdouble(result7,%d,(long double)",type);
    sprintf(endoffunc,");");
    return(0);
  case 'f':
  case 'd':
    fprintf(fp,"      G__letdouble(result7,%d,(double)",type);
    sprintf(endoffunc,");");
    return(0);
  case 'u':
    switch(G__struct.type[tagnum]) {
    case 'c':
    case 's':
    case 'u':
      deftyp = typenum;
      if(reftype) {
	fprintf(fp,"      {\n");
	if(isconst&G__CONSTFUNC) fprintf(fp,"const ");
	fprintf(fp,"         const %s& obj=",G__type2string('u',tagnum,deftyp,0,0));
	sprintf(endoffunc,";\n        result7->ref=(long)(&obj); result7->obj.i=(long)(&obj);\n      }");
      }
      else {
	if(G__CPPLINK==G__globalcomp) {
#ifndef G__VC60BUGFIXED /***************************************************/
	  fprintf(fp,"      {\n");
	  if(isconst&G__CONSTFUNC) fprintf(fp,"const ");
	  fprintf(fp,"        %s *pobj,xobj="
		  ,G__type2string('u',tagnum,deftyp,0,0));
	  if(0==(G__is_operator_newdelete&G__NOT_USING_2ARG_NEW)) {
	    if(G__is_operator_newdelete&G__DUMMYARG_NEWDELETE
	      && 0==(G__struct.funcs[tagnum]&G__HAS_OPERATORNEW2ARG)
		)
	      sprintf(endoffunc,";\n        pobj=::new((G__%s_tag*)G__getgvp()) %s(xobj);\n        result7->obj.i=(long)((void*)pobj); result7->ref=result7->obj.i;\n        G__store_tempobject(*result7);\n      }"
		      ,G__NEWID,G__type2string('u',tagnum,deftyp,0,0));
	    else
	      sprintf(endoffunc,";\n        pobj=new((void*)G__getgvp()) %s(xobj);\n        result7->obj.i=(long)((void*)pobj); result7->ref=result7->obj.i;\n        G__store_tempobject(*result7);\n      }"
		      ,G__type2string('u',tagnum,deftyp,0,0));

	  }
	  else {
	    sprintf(endoffunc,";\n        pobj=new %s(xobj);\n        result7->obj.i=(long)((void*)pobj); result7->ref=result7->obj.i;\n        G__store_tempobject(*result7);\n      }"
		    ,G__type2string('u',tagnum,deftyp,0,0));
	  }
#else /* ON902 **************************************************************/
	  fprintf(fp,"      {\n");
	  fprintf(fp,"        %s *pobj;\n"
		  ,G__type2string('u',tagnum,deftyp,0,0));
	  if(0==(G__is_operator_newdelete&G__NOT_USING_2ARG_NEW)) {
	    if(G__is_operator_newdelete&G__DUMMYARG_NEWDELETE
	       && 0==(G__struct.funcs[tagnum]&G__HAS_OPERATORNEW2ARG)
	       )
	      fprintf(fp,"        pobj=::new((G__%s_tag*)G__getgvp()) %s("
		      ,G__NEWID,G__type2string('u',tagnum,deftyp,0,0));
	    else
	      fprintf(fp,"        pobj=new((void*)G__getgvp()) %s("
		      ,G__type2string('u',tagnum,deftyp,0,0));
	  }
	  else {
	    fprintf(fp,"        pobj=new %s("
		    ,G__type2string('u',tagnum,deftyp,0,0));
	  }
	  sprintf(endoffunc,");\n        result7->obj.i=(long)((void*)pobj); result7->ref=result7->obj.i;\n        G__store_tempobject(*result7);\n      }");
#endif /* ON902 **************************************************************/
	}
	else {
	  fprintf(fp,"      {\n");
	  fprintf(fp,"        G__alloc_tempobject(result7->tagnum,result7->typenum);\n");
	  fprintf(fp,"        result7->obj.i=G__gettempbufpointer();\n");
	  fprintf(fp,"        result7->ref=G__gettempbufpointer();\n");
	  fprintf(fp,"        *((%s *)result7->obj.i)="
		  ,G__type2string(type,tagnum,typenum,reftype,0));
	  sprintf(endoffunc,";\n      }");
	}
      }
      break;
    default:
      fprintf(fp,"      G__letint(result7,%d,(long)",type);
      sprintf(endoffunc,");");
      break;
    }
    return(0);
  }
  return(1); /* never happen, avoiding lint error */
#endif
}


/**************************************************************************
* G__cppif_paratype()
*
**************************************************************************/
void G__cppif_paratype(fp,ifn,ifunc,k)
FILE *fp;
int ifn;
struct G__ifunc_table *ifunc;
int k;
{
#ifndef G__SMALLOBJECT
  int type,tagnum,typenum,reftype;
  int isconst;

  type=ifunc->para_type[ifn][k];
  tagnum=ifunc->para_p_tagtable[ifn][k];
  typenum=ifunc->para_p_typetable[ifn][k];
  reftype=ifunc->para_reftype[ifn][k];
  isconst=ifunc->para_isconst[ifn][k];

  /* Promote link-off typedef to link-on if used in function */
  if(-1!=typenum && G__NOLINK==G__newtype.globalcomp[typenum] &&
     G__NOLINK==G__newtype.iscpplink[typenum])
    G__newtype.globalcomp[typenum] = G__globalcomp;

  if(k && 0==k%2) fprintf(fp,"\n");
  if(0!=k) fprintf(fp,",");

  if(ifunc->para_name[ifn][k]) {
    char *p = strchr(ifunc->para_name[ifn][k],'[');
    if(p) {
      fprintf(fp,"G__Ap%d->a",k);
      return;
    }
  }

  if(
#ifndef G__OLDIMPLEMENTATION2191
     '1'!=type && 'a'!=type
#else
     'Q'!=type && 'a'!=type
#endif
     ) {
    switch(reftype) {
    case G__PARANORMAL:
      if(-1!=typenum&&G__PARAREFERENCE==G__newtype.reftype[typenum]) {
	reftype=G__PARAREFERENCE;
	typenum = -1;
      }
      else break;
    case G__PARAREFERENCE:
      if(islower(type)) {
	switch(type) {
        case 'u':
	  fprintf(fp,"*(%s*)libp->para[%d].ref"
		  ,G__type2string(type,tagnum,typenum,0,0) ,k);
	  break;
#ifndef G__OLDIMPLEMENTATION1167
        case 'd':
	  fprintf(fp,"*(%s*)G__Doubleref(&libp->para[%d])"
		  ,G__type2string(type,tagnum,typenum,0,0),k);
	  break;
        case 'l':
	  fprintf(fp,"*(%s*)G__Longref(&libp->para[%d])"
		  ,G__type2string(type,tagnum,typenum,0,0),k);
	  break;
        case 'i':
	  if(-1==tagnum) /* int */
	    fprintf(fp,"*(%s*)G__Intref(&libp->para[%d])"
		    ,G__type2string(type,tagnum,typenum,0,0),k);
	  else /* enum type */
	    fprintf(fp,"*(%s*)libp->para[%d].ref"
		    ,G__type2string(type,tagnum,typenum,0,0) ,k);
	  break;
        case 's':
	  fprintf(fp,"*(%s*)G__Shortref(&libp->para[%d])"
		  ,G__type2string(type,tagnum,typenum,0,0),k);
	  break;
        case 'c':
	  fprintf(fp,"*(%s*)G__Charref(&libp->para[%d])"
		  ,G__type2string(type,tagnum,typenum,0,0),k);
	  break;
        case 'h':
	  fprintf(fp,"*(%s*)G__UIntref(&libp->para[%d])"
		  ,G__type2string(type,tagnum,typenum,0,0),k);
	  break;
        case 'r':
	  fprintf(fp,"*(%s*)G__UShortref(&libp->para[%d])"
		  ,G__type2string(type,tagnum,typenum,0,0),k);
	  break;
        case 'b':
	  fprintf(fp,"*(%s*)G__UCharref(&libp->para[%d])"
		  ,G__type2string(type,tagnum,typenum,0,0),k);
	  break;
        case 'k':
	  fprintf(fp,"*(%s*)G__ULongref(&libp->para[%d])"
		  ,G__type2string(type,tagnum,typenum,0,0),k);
	  break;
        case 'n':
	  fprintf(fp,"*(%s*)G__Longlongref(&libp->para[%d])"
		  ,G__type2string(type,tagnum,typenum,0,0),k);
	  break;
        case 'm':
	  fprintf(fp,"*(%s*)G__ULonglongref(&libp->para[%d])"
		  ,G__type2string(type,tagnum,typenum,0,0),k);
	  break;
        case 'q':
	  fprintf(fp,"*(%s*)G__Longdoubleref(&libp->para[%d])"
		  ,G__type2string(type,tagnum,typenum,0,0),k);
	  break;
        case 'g':
	  fprintf(fp,"*(%s*)G__Boolref(&libp->para[%d])"
		  ,G__type2string(type,tagnum,typenum,0,0),k);
	  break;
        case 'f':
	  fprintf(fp,"*(%s*)G__Floatref(&libp->para[%d])"
		  ,G__type2string(type,tagnum,typenum,0,0),k);
	  break;
#else
        case 'd':
	  fprintf(fp,"libp->para[%d].ref?*(%s*)libp->para[%d].ref:G__Mdouble(libp->para[%d])"
		  ,k,G__type2string(type,tagnum,typenum,0,0) ,k,k);
	  break;
        case 'l':
	  fprintf(fp,"libp->para[%d].ref?*(%s*)libp->para[%d].ref:G__Mlong(libp->para[%d])"
		  ,k,G__type2string(type,tagnum,typenum,0,0) ,k ,k);
	  break;
        case 'i':
	  if(-1==tagnum) /* int */
	    fprintf(fp,"libp->para[%d].ref?*(%s*)libp->para[%d].ref:G__Mint(libp->para[%d])"
		    ,k,G__type2string(type,tagnum,typenum,0,0) ,k ,k);
	  else /* enum type */
	    fprintf(fp,"*(%s*)libp->para[%d].ref"
		    ,G__type2string(type,tagnum,typenum,0,0) ,k);
	  break;
        case 's':
	  fprintf(fp,"libp->para[%d].ref?*(%s*)libp->para[%d].ref:G__Mshort(libp->para[%d])"
		  ,k,G__type2string(type,tagnum,typenum,0,0) ,k ,k);
	  break;
        case 'c':
	  fprintf(fp,"libp->para[%d].ref?*(%s*)libp->para[%d].ref:G__Mchar(libp->para[%d])"
		  ,k,G__type2string(type,tagnum,typenum,0,0) ,k ,k);
	  break;
        case 'h':
	  fprintf(fp,"libp->para[%d].ref?*(%s*)libp->para[%d].ref:G__Muint(libp->para[%d])"
		  ,k,G__type2string(type,tagnum,typenum,0,0) ,k ,k);
	  break;
        case 'r':
	  fprintf(fp,"libp->para[%d].ref?*(%s*)libp->para[%d].ref:G__Mushort(libp->para[%d])"
		  ,k,G__type2string(type,tagnum,typenum,0,0) ,k ,k);
	  break;
        case 'b':
	  fprintf(fp,"libp->para[%d].ref?*(%s*)libp->para[%d].ref:G__Muchar(libp->para[%d])"
		  ,k,G__type2string(type,tagnum,typenum,0,0) ,k ,k);
	  break;
        case 'k':
	  fprintf(fp,"libp->para[%d].ref?*(%s*)libp->para[%d].ref:G__Mulong(libp->para[%d])"
		  ,k,G__type2string(type,tagnum,typenum,0,0) ,k ,k);
	  break;
        case 'n':
	  fprintf(fp,"libp->para[%d].ref?*(%s*)libp->para[%d].ref:G__Mlonglong(libp->para[%d])"
		  ,k,G__type2string(type,tagnum,typenum,0,0) ,k ,k);
	  break;
        case 'm':
	  fprintf(fp,"libp->para[%d].ref?*(%s*)libp->para[%d].ref:G__Mulonglong(libp->para[%d])"
		  ,k,G__type2string(type,tagnum,typenum,0,0) ,k ,k);
	  break;
        case 'q':
	  fprintf(fp,"libp->para[%d].ref?*(%s*)libp->para[%d].ref:G__Mlongdouble(libp->para[%d])"
		  ,k,G__type2string(type,tagnum,typenum,0,0) ,k ,k);
	  break;
        case 'g':
#ifdef G__BOOL4BYTE
	  fprintf(fp,"libp->para[%d].ref?*(%s*)libp->para[%d].ref:G__Mint(libp->para[%d])"
		  ,k,G__type2string(type,tagnum,typenum,0,0) ,k ,k);
#else
	  fprintf(fp,"libp->para[%d].ref?*(%s*)libp->para[%d].ref:G__Muchar(libp->para[%d])"
		  ,k,G__type2string(type,tagnum,typenum,0,0) ,k ,k);
#endif
	  break;
        case 'f':
	  fprintf(fp,"libp->para[%d].ref?*(%s*)libp->para[%d].ref:G__Mfloat(libp->para[%d])"
		  ,k,G__type2string(type,tagnum,typenum,0,0) ,k ,k);
	  break;
#endif
	default:
	  fprintf(fp,"libp->para[%d].ref?*(%s*)libp->para[%d].ref:(%s)G__int(libp->para[%d])"
		  ,k,G__type2string(type,tagnum,typenum,0,0) ,k
		  ,G__type2string(type,tagnum,typenum,0,0) ,k);
	  break;
        }
      }
      else {
	if(-1!=typenum&&isupper(G__newtype.type[typenum])) {
	  /* This part is not perfect. Cint data structure bug.
	   * typedef char* value_type;
	   * void f(value_type& x);  // OK
	   * void f(value_type x);   // OK
	   * void f(value_type* x);  // OK
	   * void f(value_type*& x); // bad 
	   *  reference and pointer to pointer can not happen at once */
	  fprintf(fp,"libp->para[%d].ref?*(%s*)libp->para[%d].ref:*(%s*)(&G__Mlong(libp->para[%d]))"
		  ,k,G__type2string(type,tagnum,typenum,0,isconst&G__CONSTVAR) 
		  ,k,G__type2string(type,tagnum,typenum,0,isconst&G__CONSTVAR)
		  ,k);
	  /* above is , in fact, not good. G__type2string returns pointer to
	   * static buffer. This relies on the fact that the 2 calls are
	   * identical */
	}
	else {
	    fprintf(fp,"libp->para[%d].ref?*(%s)libp->para[%d].ref:*(%s)(&G__Mlong(libp->para[%d]))"
		,k,G__type2string(type,tagnum,typenum,2,isconst&G__CONSTVAR) 
		,k,G__type2string(type,tagnum,typenum,2,isconst&G__CONSTVAR)
		    ,k);
	  /* above is , in fact, not good. G__type2string returns pointer to
	   * static buffer. This relies on the fact that the 2 calls are
	   * identical */
	}
      }
      return;

#ifndef G__OLDIMPLEMENTATION1975
    case G__PARAREFP2P:
    case G__PARAREFP2P2P:
      reftype = G__PLVL(reftype);
      if(-1!=typenum&&isupper(G__newtype.type[typenum])) {
	fprintf(fp,"libp->para[%d].ref?*(%s*)libp->para[%d].ref:*(%s*)(&G__Mlong(libp->para[%d]))"
		  ,k,G__type2string(type,tagnum,typenum,reftype,isconst) 
		  ,k,G__type2string(type,tagnum,typenum,reftype,isconst) ,k);
      }
      else {
#if !defined(G__OLDIMPLEMENTATION1976)
	fprintf(fp,"libp->para[%d].ref?*(%s*)libp->para[%d].ref:*(%s*)(&G__Mlong(libp->para[%d]))"
		,k,G__type2string(type,tagnum,typenum,reftype,isconst) 
		,k,G__type2string(type,tagnum,typenum,reftype,isconst),k);
#else
	fprintf(fp,"libp->para[%d].ref?*(%s*)libp->para[%d].ref:*(%s*)(&G__Mlong(libp->para[%d]))"
		,k,G__type2string(type,tagnum,typenum,reftype,isconst&~G__CONSTVAR) 
		,k,G__type2string(type,tagnum,typenum,reftype,isconst&~G__CONSTVAR),k);
#endif
      }
      return;
#endif /* 1975 */

    case G__PARAP2P:
      G__ASSERT(isupper(type));
      fprintf(fp,"(%s)G__int(libp->para[%d])"
	      ,G__type2string(type,tagnum,typenum,reftype,isconst),k);
      return;
    case G__PARAP2P2P:
      G__ASSERT(isupper(type));
      fprintf(fp,"(%s)G__int(libp->para[%d])"
	      ,G__type2string(type,tagnum,typenum,reftype,isconst),k);
      return;
    }
  }

  switch(type) {
#ifndef G__OLDIMPLEMENTATION2191
  case '1': /* Pointer to function */
#else
  case 'Q': /* Pointer to function */
#endif
#ifdef G__CPPIF_EXTERNC
    fprintf(fp,"(%s)G__int(libp->para[%d])"
	      ,G__p2f_typedefname(ifn,ifunc->page,k),k);
    break;
#endif
  case 'c':
  case 'b':
  case 's':
  case 'r':
  case 'i':
  case 'h':
  case 'l':
  case 'k':
  case 'g':
  case 'F':
  case 'D':
  case 'E':
  case 'Y':
  case 'U':
    fprintf(fp,"(%s)G__int(libp->para[%d])"
	      ,G__type2string(type,tagnum,typenum,reftype,isconst),k);
    break;
  case 'a':  /* Pointer to member , THIS IS BAD , WON'T WORK */
    fprintf(fp,"*(%s *)G__int(libp->para[%d])"
	      ,G__type2string(type,tagnum,typenum,0,isconst),k);
    break;
  case 'n':
    fprintf(fp,"(%s)G__Longlong(libp->para[%d])"
	      ,G__type2string(type,tagnum,typenum,reftype,isconst),k);
    break;
  case 'm':
    fprintf(fp,"(%s)G__ULonglong(libp->para[%d])"
	      ,G__type2string(type,tagnum,typenum,reftype,isconst),k);
    break;
  case 'q':
    fprintf(fp,"(%s)G__Longdouble(libp->para[%d])"
	      ,G__type2string(type,tagnum,typenum,reftype,isconst),k);
    break;
  case 'f':
  case 'd':
    fprintf(fp,"(%s)G__double(libp->para[%d])"
	    ,G__type2string(type,tagnum,typenum,0,isconst),k);
    break;
  case 'u':
    if('e'==G__struct.type[tagnum]) {
      fprintf(fp,"(%s)G__int(libp->para[%d])"
	      ,G__type2string(type,tagnum,typenum,0,isconst),k);
    }
    else {
      fprintf(fp,"*((%s*)G__int(libp->para[%d]))"
	      ,G__type2string(type,tagnum,typenum,0,isconst),k);
    }
    break;
  default:
    fprintf(fp,"(%s)G__int(libp->para[%d])"
	      ,G__type2string(type,tagnum,typenum,0,isconst),k);
    break;
  }

#endif
}

/**************************************************************************
**************************************************************************
* Generate C++ symbol binding routine calls
**************************************************************************
**************************************************************************/

/**************************************************************************
* G__cpplink_tagtable()
*
**************************************************************************/
void G__cpplink_tagtable(fp,hfp)
FILE *fp;
FILE *hfp;
{
#ifndef G__SMALLOBJECT
  int i;
  char tagname[G__MAXNAME*8];
  char mappedtagname[G__MAXNAME*6];
  char buf[G__ONELINE];

  fprintf(fp,"\n/*********************************************************\n");
  fprintf(fp,"* Class,struct,union,enum tag information setup\n");
  fprintf(fp,"*********************************************************/\n");

  if(G__CPPLINK == G__globalcomp) {
    G__cpplink_linked_taginfo(fp,hfp);
    fprintf(fp,"extern \"C\" void G__cpp_setup_tagtable%s() {\n",G__DLLID);
  }
  else {
    G__cpplink_linked_taginfo(fp,hfp);
    fprintf(fp,"void G__c_setup_tagtable%s() {\n",G__DLLID);
  }

  fprintf(fp,"\n   /* Setting up class,struct,union tag entry */\n");
  for(i=0;i<G__struct.alltag;i++) {
    if(
       (G__struct.hash[i] || 0==G__struct.name[i][0]) && 
       (G__CPPLINK==G__struct.globalcomp[i]
	||G__CLINK==G__struct.globalcomp[i]
	||G__ONLYMETHODLINK==G__struct.globalcomp[i]
	)) {
      if(!G__nestedclass) {
	if(0<=G__struct.parent_tagnum[i] &&
	   -1!=G__struct.parent_tagnum[G__struct.parent_tagnum[i]])
	  continue;
	if(G__CLINK==G__struct.globalcomp[i] && -1!=G__struct.parent_tagnum[i])
	  continue;
      }

      if(-1==G__struct.line_number[i]) { 
	/* Philippe and Fons's request to display this */
	if(G__dispmsg>= G__DISPERR /*G__DISPNOTE*/) {
	  if(G__NOLINK==G__struct.iscpplink[i]) {
	    G__fprinterr(G__serr,"Note: Link requested for undefined class %s (ignore this message)"
			 ,G__fulltagname(i,1));
	  }
	  else {
	    G__fprinterr(G__serr,
			 "Note: Link requested for already precompiled class %s (ignore this message)"
			 ,G__fulltagname(i,1));
	  }
	  G__printlinenum();
	}
	/* G__genericerror((char*)NULL); */
      }

      G__getcommentstring(buf,i,&G__struct.comment[i]);

      strcpy(tagname,G__fulltagname(i,0));
      if(-1!=G__struct.line_number[i]
	 && (-1==G__struct.parent_tagnum[i]||G__nestedclass)
	 ) {
	if('e'==G__struct.type[i])
	  fprintf(fp,"   G__tagtable_setup(G__get_linked_tagnum(&%s),sizeof(%s),%d,%d,%s,NULL,NULL);\n"
		  ,G__mark_linked_tagnum(i) ,"int" ,G__globalcomp 
#if  !defined(G__OLDIMPLEMENTATION1545)
		  ,G__struct.isabstract[i]+G__struct.funcs[i]*0x100
		  +G__struct.rootflag[i]*0x10000
#else
		  ,G__struct.isabstract[i]+G__struct.funcs[i]*0x100
#endif
		  ,buf);
	else if('n'==G__struct.type[i]) {
	  strcpy(mappedtagname,G__map_cpp_name(tagname));
	  fprintf(fp,"   G__tagtable_setup(G__get_linked_tagnum(&%s),0,%d,%d,%s,G__setup_memvar%s,G__setup_memfunc%s);\n"
		  ,G__mark_linked_tagnum(i)
		  /* ,G__type2string('u',i,-1,0,0) */
		  ,G__globalcomp 
#if  !defined(G__OLDIMPLEMENTATION1545)
		  ,G__struct.isabstract[i]+G__struct.funcs[i]*0x100
		  +G__struct.rootflag[i]*0x10000
#else
		  ,G__struct.isabstract[i]+G__struct.funcs[i]*0x100
#endif
		  ,buf,mappedtagname,mappedtagname);
	}
	else if(0==G__struct.name[i][0]) {
	  strcpy(mappedtagname,G__map_cpp_name(tagname));
	  if(G__CPPLINK==G__globalcomp) {
	    fprintf(fp,"   G__tagtable_setup(G__get_linked_tagnum(&%s),%s,%d,%d,%s,G__setup_memvar%s,G__setup_memfunc%s);\n"
		    ,G__mark_linked_tagnum(i)
		    ,"0" /* G__type2string('u',i,-1,0,0) */
		    ,G__globalcomp 
#if  !defined(G__OLDIMPLEMENTATION1545)
		    ,G__struct.isabstract[i]+G__struct.funcs[i]*0x100
		    +G__struct.rootflag[i]*0x10000
#else
		    ,G__struct.isabstract[i]+G__struct.funcs[i]*0x100
#endif
		    ,buf ,mappedtagname,mappedtagname);
	  }
	  else {
	    fprintf(fp,"   G__tagtable_setup(G__get_linked_tagnum(&%s),%s,%d,%d,%s,G__setup_memvar%s,NULL);\n"
		    ,G__mark_linked_tagnum(i)
		    ,"0" /* G__type2string('u',i,-1,0,0) */
		    ,G__globalcomp 
#if  !defined(G__OLDIMPLEMENTATION1545)
		    ,G__struct.isabstract[i]+G__struct.funcs[i]*0x100
		    +G__struct.rootflag[i]*0x10000
#else
		    ,G__struct.isabstract[i]+G__struct.funcs[i]*0x100
#endif
		    ,buf ,mappedtagname);
	  }
	}
	else {
	  strcpy(mappedtagname,G__map_cpp_name(tagname));
	  if(G__CPPLINK==G__globalcomp && '$'!=G__struct.name[i][0]) {
	    if(G__ONLYMETHODLINK==G__struct.globalcomp[i])
	      fprintf(fp,"   G__tagtable_setup(G__get_linked_tagnum(&%s),sizeof(%s),%d,%d,%s,NULL,G__setup_memfunc%s);\n"
		      ,G__mark_linked_tagnum(i)
		      ,G__type2string('u',i,-1,0,0)
		      ,G__globalcomp 
#if  !defined(G__OLDIMPLEMENTATION1545)
		      ,G__struct.isabstract[i]+G__struct.funcs[i]*0x100
		      +G__struct.rootflag[i]*0x10000
#else
		      ,G__struct.isabstract[i]+G__struct.funcs[i]*0x100
#endif
		      ,buf ,mappedtagname);
	    else
	    if(G__suppress_methods) 
	      fprintf(fp,"   G__tagtable_setup(G__get_linked_tagnum(&%s),sizeof(%s),%d,%d,%s,G__setup_memvar%s,NULL);\n"
		      ,G__mark_linked_tagnum(i)
		      ,G__type2string('u',i,-1,0,0)
		      ,G__globalcomp 
#if  !defined(G__OLDIMPLEMENTATION1545)
		      ,G__struct.isabstract[i]+G__struct.funcs[i]*0x100
		      +G__struct.rootflag[i]*0x10000
#else
		      ,G__struct.isabstract[i]+G__struct.funcs[i]*0x100
#endif
		      ,buf ,mappedtagname);
	    else
	      fprintf(fp,"   G__tagtable_setup(G__get_linked_tagnum(&%s),sizeof(%s),%d,%d,%s,G__setup_memvar%s,G__setup_memfunc%s);\n"
		      ,G__mark_linked_tagnum(i)
		      ,G__type2string('u',i,-1,0,0)
		      ,G__globalcomp 
#if  !defined(G__OLDIMPLEMENTATION1545)
		      ,G__struct.isabstract[i]+G__struct.funcs[i]*0x100
		      +G__struct.rootflag[i]*0x10000
#else
		      ,G__struct.isabstract[i]+G__struct.funcs[i]*0x100
#endif
		      ,buf ,mappedtagname,mappedtagname);
	  }
	  else if('$'==G__struct.name[i][0]&&
	  isupper(G__newtype.type[G__defined_typename(G__struct.name[i]+1)])) {
	    fprintf(fp,"   G__tagtable_setup(G__get_linked_tagnum(&%s),sizeof(%s),%d,%d,%s,NULL,NULL);\n"
		    ,G__mark_linked_tagnum(i)
		    ,G__type2string('u',i,-1,0,0)
		    ,G__globalcomp 
#if  !defined(G__OLDIMPLEMENTATION1545)
		    ,G__struct.isabstract[i]+G__struct.funcs[i]*0x100
		    +G__struct.rootflag[i]*0x10000
#else
		    ,G__struct.isabstract[i]+G__struct.funcs[i]*0x100
#endif
		    ,buf);
	  }
	  else {
	    fprintf(fp,"   G__tagtable_setup(G__get_linked_tagnum(&%s),sizeof(%s),%d,%d,%s,G__setup_memvar%s,NULL);\n"
		    ,G__mark_linked_tagnum(i)
		    ,G__type2string('u',i,-1,0,0)
		    ,G__globalcomp 
#if  !defined(G__OLDIMPLEMENTATION1545)
		    ,G__struct.isabstract[i]+G__struct.funcs[i]*0x100
		    +G__struct.rootflag[i]*0x10000
#else
		    ,G__struct.isabstract[i]+G__struct.funcs[i]*0x100
#endif
		    ,buf ,mappedtagname);
	  }

	}
      }
      else {
	fprintf(fp,"   G__tagtable_setup(G__get_linked_tagnum(&%s),0,%d,%d,%s,NULL,NULL);\n"
		,G__mark_linked_tagnum(i)
		,G__globalcomp 
#if  !defined(G__OLDIMPLEMENTATION1545)
		    ,G__struct.isabstract[i]+G__struct.funcs[i]*0x100
		    +G__struct.rootflag[i]*0x10000
#else
		,G__struct.isabstract[i]+G__struct.funcs[i]*0x100
#endif
		,buf);
      }
      if('e'!=G__struct.type[i]) {
	if(strchr(tagname,'<')) { /* template class */
	  fprintf(hfp,"typedef %s G__%s;\n",tagname,G__map_cpp_name(tagname));
	}
      }
    }
    else if((G__struct.hash[i] || 0==G__struct.name[i][0]) && 
	    (G__CPPLINK-2)==G__struct.globalcomp[i]) {
      fprintf(fp,"   G__get_linked_tagnum(&%s);\n" ,G__mark_linked_tagnum(i));
    }
  }

  fprintf(fp,"}\n");
#endif
}

#ifdef G__VIRTUALBASE
/**************************************************************************
* G__vbo_funcname()
*
**************************************************************************/
static char* G__vbo_funcname(tagnum,basetagnum,basen)
int tagnum;
int basetagnum;
int basen;
{
  static char result[G__LONGLINE*2];
  char temp[G__LONGLINE];
  strcpy(temp,G__map_cpp_name(G__fulltagname(tagnum,1)));
  sprintf(result,"G__2vbo_%s_%s_%d",temp
	  ,G__map_cpp_name(G__fulltagname(basetagnum,1)),basen);
  return(result);
}

/**************************************************************************
* G__cpplink_inheritance()
*
**************************************************************************/
void G__cppif_inheritance(fp)
FILE *fp;
{
  int i;
  int basen;
  int basetagnum;
  char temp[G__LONGLINE*2];

  fprintf(fp,"\n/*********************************************************\n");
  fprintf(fp,"* virtual base class offset calculation interface\n");
  fprintf(fp,"*********************************************************/\n");

  fprintf(fp,"\n   /* Setting up class inheritance */\n");
  for(i=0;i<G__struct.alltag;i++) {
    if(G__NOLINK>G__struct.globalcomp[i]&&
       (-1==(int)G__struct.parent_tagnum[i]
	|| G__nestedclass
	)
       && -1!=G__struct.line_number[i]&&G__struct.hash[i]&&
       ('$'!=G__struct.name[i][0])) {
      switch(G__struct.type[i]) {
      case 'c': /* class */
      case 's': /* struct */
	if(G__struct.baseclass[i]->basen>0) {
	  for(basen=0;basen<G__struct.baseclass[i]->basen;basen++) {
	    if(G__PUBLIC!=G__struct.baseclass[i]->baseaccess[basen] ||
	       0==(G__struct.baseclass[i]->property[basen]&G__ISVIRTUALBASE))
	      continue;
	    basetagnum=G__struct.baseclass[i]->basetagnum[basen];
	    fprintf(fp,"static long %s(long pobject) {\n"
		    ,G__vbo_funcname(i,basetagnum,basen));
	    strcpy(temp,G__fulltagname(i,1));
	    fprintf(fp,"  %s *G__Lderived=(%s*)pobject;\n",temp,temp);
	    fprintf(fp,"  %s *G__Lbase=G__Lderived;\n",G__fulltagname(basetagnum,1));
	    fprintf(fp,"  return((long)G__Lbase-(long)G__Lderived);\n");
	    fprintf(fp,"}\n\n");
	  }
	}
	break;
      default: /* enum */
	break;
      }
    } /* if() */
  } /* for(i) */
}
#endif

/**************************************************************************
* G__cpplink_inheritance()
*
**************************************************************************/
void G__cpplink_inheritance(fp)
FILE *fp;
{
#ifndef G__SMALLOBJECT
  int i;
  int basen;
  int basetagnum;
  char temp[G__MAXNAME*6];
  int flag;

  fprintf(fp,"\n/*********************************************************\n");
  fprintf(fp,"* Inheritance information setup/\n");
  fprintf(fp,"*********************************************************/\n");

  if(G__CPPLINK == G__globalcomp) {
    fprintf(fp,"extern \"C\" void G__cpp_setup_inheritance%s() {\n",G__DLLID);
  }
  else  {
  }

  fprintf(fp,"\n   /* Setting up class inheritance */\n");
  for(i=0;i<G__struct.alltag;i++) {
    if(G__NOLINK>G__struct.globalcomp[i]&&
       (-1==(int)G__struct.parent_tagnum[i]
       || G__nestedclass
	)
       && -1!=G__struct.line_number[i]&&G__struct.hash[i]&&
       ('$'!=G__struct.name[i][0])) {
      switch(G__struct.type[i]) {
      case 'c': /* class */
      case 's': /* struct */
	if(G__struct.baseclass[i]->basen>0) {
	  fprintf(fp,"   if(0==G__getnumbaseclass(G__get_linked_tagnum(&%s))) {\n"
	          ,G__get_link_tagname(i));
	  flag=0;
	  for(basen=0;basen<G__struct.baseclass[i]->basen;basen++) {
	    if(0==(G__struct.baseclass[i]->property[basen]&G__ISVIRTUALBASE)) 
	      ++flag;
	  }
	  if(flag) {
	    fprintf(fp,"     %s *G__Lderived;\n",G__fulltagname(i,0));
	    fprintf(fp,"     G__Lderived=(%s*)0x1000;\n",G__fulltagname(i,1));
	  }
	  for(basen=0;basen<G__struct.baseclass[i]->basen;basen++) {
	    basetagnum=G__struct.baseclass[i]->basetagnum[basen];
	    fprintf(fp,"     {\n");
#ifdef G__VIRTUALBASE
	    strcpy(temp,G__mark_linked_tagnum(basetagnum));
	    if(G__struct.baseclass[i]->property[basen]&G__ISVIRTUALBASE) {
	      char temp2[G__LONGLINE*2];
	      strcpy(temp2,G__vbo_funcname(i,basetagnum,basen));
	      fprintf(fp,"       G__inheritance_setup(G__get_linked_tagnum(&%s),G__get_linked_tagnum(&%s),(long)%s,%d,%ld);\n"
		      ,G__mark_linked_tagnum(i)
		      ,temp
		      ,temp2
		      ,G__struct.baseclass[i]->baseaccess[basen]
		      ,(long)G__struct.baseclass[i]->property[basen]
		      );
	    }
	    else {
	      int basen2,flag2=0;
	      for(basen2=0;basen2<G__struct.baseclass[i]->basen;basen2++) {
		if(basen2!=basen &&
		   (G__struct.baseclass[i]->basetagnum[basen]
		    == G__struct.baseclass[i]->basetagnum[basen2]) &&
		   ((G__struct.baseclass[i]->property[basen]&G__ISVIRTUALBASE)
		    ==0 ||
		    (G__struct.baseclass[i]->property[basen2]&G__ISVIRTUALBASE)
		    ==0 )) {
		  flag2=1;
		}
	      }
	      strcpy(temp,G__fulltagname(basetagnum,1));
	      if(!flag2) 
		fprintf(fp,"       %s *G__Lpbase=(%s*)G__Lderived;\n"
			,temp,G__fulltagname(basetagnum,1));
	      else {
		G__fprinterr(G__serr,
			     "Warning: multiple ambiguous inheritance %s and %s. Cint will not get correct base object address\n"
			     ,temp,G__fulltagname(i,1));
		fprintf(fp,"       %s *G__Lpbase=(%s*)((long)G__Lderived);\n"
			,temp,G__fulltagname(basetagnum,1));
	      }
	      strcpy(temp,G__mark_linked_tagnum(basetagnum));
	      fprintf(fp,"       G__inheritance_setup(G__get_linked_tagnum(&%s),G__get_linked_tagnum(&%s),(long)G__Lpbase-(long)G__Lderived,%d,%ld);\n"
		      ,G__mark_linked_tagnum(i)
		      ,temp
		      ,G__struct.baseclass[i]->baseaccess[basen]
		      ,(long)G__struct.baseclass[i]->property[basen]
		      );
	    }
#else
	    strcpy(temp,G__fulltagname(basetagnum,1));
	    if(G__struct.baseclass[i]->property[basen]&G__ISVIRTUALBASE) {
	      fprintf(fp,"       %s *pbase=(%s*)0x1000;\n"
		      ,temp,G__fulltagname(basetagnum,1));
	    }
	    else {
	      fprintf(fp,"       %s *pbase=(%s*)G__Lderived;\n"
		      ,temp,G__fulltagname(basetagnum,1));
	    }
	    strcpy(temp,G__mark_linked_tagnum(basetagnum));
	    fprintf(fp,"       G__inheritance_setup(G__get_linked_tagnum(&%s),G__get_linked_tagnum(&%s),(long)pbase-(long)G__Lderived,%d,%ld);\n"
		    ,G__mark_linked_tagnum(i)
		    ,temp
		    ,G__struct.baseclass[i]->baseaccess[basen]
		    ,G__struct.baseclass[i]->property[basen]
		    );
#endif
	    fprintf(fp,"     }\n");
	  }
	  fprintf(fp,"   }\n");
	}
	break;
      default: /* enum */
	break;
      }
    } /* if() */
  } /* for(i) */

  fprintf(fp,"}\n");
#endif
}

/**************************************************************************
* G__cpplink_typetable()
*
**************************************************************************/
void G__cpplink_typetable(fp,hfp)
FILE *fp;
FILE *hfp;
{
  int i;
  int j;
  char temp[G__ONELINE];
  char *p;
  char buf[G__ONELINE];


  fprintf(fp,"\n/*********************************************************\n");
  fprintf(fp,"* typedef information setup/\n");
  fprintf(fp,"*********************************************************/\n");

  if(G__CPPLINK == G__globalcomp) {
    fprintf(fp,"extern \"C\" void G__cpp_setup_typetable%s() {\n",G__DLLID);
  }
  else {
    fprintf(fp,"void G__c_setup_typetable%s() {\n",G__DLLID);
  }

  fprintf(fp,"\n   /* Setting up typedef entry */\n");
  for(i=0;i<G__newtype.alltype;i++) {
    if(G__NOLINK>G__newtype.globalcomp[i]) {
      if(!(G__newtype.parent_tagnum[i] == -1 ||
	   (G__nestedtypedef &&
	    (G__struct.globalcomp[G__newtype.parent_tagnum[i]]<G__NOLINK
#define G__OLDIMPLEMENTATION1830
	     )
	    )))
	continue;
      if(strncmp("G__p2mf",G__newtype.name[i],7)==0 &&
	 G__CPPLINK==G__globalcomp){
	G__ASSERT(i>0);
	strcpy(temp,G__newtype.name[i-1]);
	p = strstr(temp,"::*");
	*(p+3)='\0';
	fprintf(hfp,"typedef %s%s)%s;\n",temp,G__newtype.name[i],p+4);
      }
      if('u'==tolower(G__newtype.type[i]))
	fprintf(fp,"   G__search_typename2(\"%s\",%d,G__get_linked_tagnum(&%s),%d,"
		,G__newtype.name[i]
		,G__newtype.type[i]
		,G__mark_linked_tagnum(G__newtype.tagnum[i])
#if !defined(G__OLDIMPLEMENTATION1861)
		,G__newtype.reftype[i] | (G__newtype.isconst[i]*0x100)
#else
		,G__newtype.reftype[i] & (G__newtype.isconst[i]*0x100)
#endif
		);
      else
	fprintf(fp,"   G__search_typename2(\"%s\",%d,-1,%d,\n"
		,G__newtype.name[i]
		,G__newtype.type[i]
#if !defined(G__OLDIMPLEMENTATION1861)
		,G__newtype.reftype[i] | (G__newtype.isconst[i]*0x100)
#else
		,G__newtype.reftype[i] & (G__newtype.isconst[i]*0x100)
#endif
		);
      if(G__newtype.parent_tagnum[i] == -1) 
        fprintf(fp,"-1);\n");
      else 
        fprintf(fp,"G__get_linked_tagnum(&%s));\n"
	       ,G__mark_linked_tagnum(G__newtype.parent_tagnum[i]));

      if(-1!=G__newtype.comment[i].filenum) {
	G__getcommenttypedef(temp,&G__newtype.comment[i],i);
	if(temp[0]) G__add_quotation(temp,buf);
	else strcpy(buf,"NULL");
      }
      else strcpy(buf,"NULL");
      if(G__newtype.nindex[i]>G__MAXVARDIM) {
	/* This is just a work around */
	G__fprinterr(G__serr,"CINT INTERNAL ERROR? typedef %s[%d] 0x%lx\n"
		,G__newtype.name[i],G__newtype.nindex[i]
		,(long)G__newtype.index[i]);
	G__newtype.nindex[i] = 0;
	if(G__newtype.index[i]) free((void*)G__newtype.index[i]);
      }
      fprintf(fp,"   G__setnewtype(%d,%s,%d);\n",G__globalcomp,buf
	      ,G__newtype.nindex[i]);
      if(G__newtype.nindex[i]) {
	for(j=0;j<G__newtype.nindex[i];j++) {
	  fprintf(fp,"   G__setnewtypeindex(%d,%d);\n"
		  ,j,G__newtype.index[i][j]);
	}
      }

    }
  }
  fprintf(fp,"}\n");
}

/**************************************************************************
* G__hascompiledoriginalbase()
*
**************************************************************************/
static int G__hascompiledoriginalbase(tagnum)
int tagnum;
{
  struct G__ifunc_table *memfunc;
  struct G__inheritance *baseclass = G__struct.baseclass[tagnum];
  int basen,ifn;
  for(basen=0;basen<baseclass->basen;basen++) {
    if(G__CPPLINK!=G__struct.iscpplink[baseclass->basetagnum[basen]])
      continue;
    memfunc=G__struct.memfunc[baseclass->basetagnum[basen]];
    while(memfunc) {
      for(ifn=0;ifn<memfunc->allifunc;ifn++) {
	if(memfunc->isvirtual[ifn]) return(1);
      }
      memfunc=memfunc->next;
    }
  }
  return(0);
}

/**************************************************************************
* G__cpplink_memvar()
*
**************************************************************************/
void G__cpplink_memvar(fp)
FILE *fp;
{
  int i,j,k;
  struct G__var_array *var;
  int typenum;
  int pvoidflag; /* local enum compilation bug fix */
  G__value buf;
  char value[G__MAXNAME*6],ttt[G__MAXNAME*6];
  int store_var_type;
  fpos_t pos;
  int count;
  char commentbuf[G__ONELINE];
  /* int alltag=0; */

  fprintf(fp,"\n/*********************************************************\n");
  fprintf(fp,"* Data Member information setup/\n");
  fprintf(fp,"*********************************************************/\n");

  fprintf(fp,"\n   /* Setting up class,struct,union tag member variable */\n");
  for(i=0;i<G__struct.alltag;i++) {
    if((G__CPPLINK==G__struct.globalcomp[i]||
	G__CLINK==G__struct.globalcomp[i])&&
       (-1==(int)G__struct.parent_tagnum[i]
	|| G__nestedclass
	)
       && -1!=G__struct.line_number[i]&&
       (G__struct.hash[i] || 0==G__struct.name[i][0])
       ) {

      var = G__struct.memvar[i];

      if('$'==G__struct.name[i][0]) {
	typenum=G__defined_typename(G__struct.name[i]+1);
	if(isupper(G__newtype.type[typenum])) continue;
      }

      /* link member variable information */
      fprintf(fp,"\n   /* %s */\n",G__type2string('u',i,-1,0,0));

      if('e'==G__struct.type[i]) continue;


      if(G__CPPLINK == G__globalcomp) {
	fprintf(fp,"static void G__setup_memvar%s(void) {\n",G__map_cpp_name(G__fulltagname(i,0)));
      }
      else {
	if(G__clock)
	  fprintf(fp,"static void G__setup_memvar%s() {\n",G__map_cpp_name(G__fulltagname(i,0)));
	else
	  fprintf(fp,"static void G__setup_memvar%s(void) {\n",G__map_cpp_name(G__fulltagname(i,0)));
      }

      count=0;
      fgetpos(fp,&pos);

      fprintf(fp,"   G__tag_memvar_setup(G__get_linked_tagnum(&%s));\n"
	      ,G__mark_linked_tagnum(i));
      if('n'==G__struct.type[i]
	 || 0==G__struct.name[i][0]
	 ) 
	fprintf(fp,"   {\n");
      else
	fprintf(fp,"   { %s *p; p=(%s*)0x1000; if (p) { }\n"
		,G__type2string('u',i,-1,0,0),G__type2string('u',i,-1,0,0));
      while(var) {
	for(j=0;j<var->allvar;j++) {
	  if(-1!=G__struct.virtual_offset[i] &&
	     strcmp(var->varnamebuf[j],"G__virtualinfo")==0
	     && 0==G__hascompiledoriginalbase(i)
	     ) {
	  }
	  if(((G__PUBLIC==var->access[j]
	       ||(G__PROTECTED==var->access[j] && 
		  (G__PROTECTEDACCESS&G__struct.protectedaccess[i]))
	       || (G__PRIVATEACCESS&G__struct.protectedaccess[i])
	       ) && 0==var->bitfield[j])||
	     G__precomp_private) {
	    ++count;
	    if((-1!=var->p_tagtable[j]&&
		islower(var->type[j])&&var->constvar[j]&&
		'e'==G__struct.type[var->p_tagtable[j]])
#ifdef G__UNADDRESSABLEBOOL
	       ||'g'==var->type[j]
#endif
	       )
	      pvoidflag=1;
	    else pvoidflag=0;
	    fprintf(fp,"   G__memvar_setup(");
	    if(G__PUBLIC==var->access[j] && 0==var->bitfield[j]) {
	      if(0==G__struct.name[i][0]) {
		fprintf(fp,"(void*)0,");
	      }
	      else
	      if(G__LOCALSTATIC==var->statictype[j]) {
		if(pvoidflag) fprintf(fp,"(void*)G__PVOID,");
		else          fprintf(fp,"(void*)(&%s::%s),"
				      ,G__fulltagname(i,1),var->varnamebuf[j]);
	      }
	      else {
		fprintf(fp,"(void*)((long)(&p->%s)-(long)(p)),"
			,var->varnamebuf[j]);
	      }
	    }
	    else if(G__PROTECTED==var->access[j] && 
		    G__struct.protectedaccess[i]) {
	      fprintf(fp,"(void*)((%s_PR*)p)->G__OS_%s(),"
		      ,G__get_link_tagname(i)
		      ,var->varnamebuf[j]);
	    }
	    else { /* Private or protected member */
	      fprintf(fp,"(void*)NULL,");
	    }
	    fprintf(fp,"%d,",var->type[j]);
	    fprintf(fp,"%d,",var->reftype[j]);
	    fprintf(fp,"%d,",var->constvar[j]);

	    if(-1!=var->p_tagtable[j])
	      fprintf(fp,"G__get_linked_tagnum(&%s),"
		      ,G__mark_linked_tagnum(var->p_tagtable[j]));
	    else
	      fprintf(fp,"-1,");

	    if(-1!=var->p_typetable[j])
	      fprintf(fp,"G__defined_typename(\"%s\"),"
		      ,G__newtype.name[var->p_typetable[j]]);
	    else
	      fprintf(fp,"-1,");

	    fprintf(fp,"%d,",var->statictype[j]);
	    fprintf(fp,"%d,",var->access[j]);
	    fprintf(fp,"\"%s"
		    ,var->varnamebuf[j]);
	    if(INT_MAX==var->varlabel[j][1] /* && 1== var->varlabel[j][0] */){
	      fprintf(fp,"[]");
	    } else
	    if(var->varlabel[j][1])
	      fprintf(fp,"[%d]",
		      (var->varlabel[j][1]+1)/var->varlabel[j][0]);
	    for(k=1;k<var->paran[j];k++) {
	      fprintf(fp,"[%d]",var->varlabel[j][k+1]);
	    }
	    if(pvoidflag
	       && G__LOCALSTATIC==var->statictype[j]
#ifdef G__UNADDRESSABLEBOOL
	       && 'g'!=var->type[j]
#endif
	       ) {
	      /* local enum member as static member.
	       * CAUTION: This implementation cause error on enum in
	       * nested class. */
	      sprintf(ttt,"%s::%s",G__fulltagname(i,1),var->varnamebuf[j]);
	      store_var_type=G__var_type; /* questionable */
	      G__var_type='p';
	      buf = G__getitem(ttt);
	      G__var_type=store_var_type; /* questionable */
	      G__string(buf,value);
	      G__quotedstring(value,ttt);
	      fprintf(fp,"=%s\",0",ttt);
	    }
	    else fprintf(fp,"=\",0");
	    G__getcommentstring(commentbuf,i,&var->comment[j]);
	    fprintf(fp,",%s);\n",commentbuf);
	  } /* end if(G__PUBLIC) */
	  G__var_type='p';
	} /* end for(j) */
	var=var->next;
      }  /* end while(var) */
      fprintf(fp,"   }\n");

      fprintf(fp,"   G__tag_memvar_reset();\n");
      fprintf(fp,"}\n\n");


    } /* end if(globalcomp) */
  } /* end for(i) */

  if(G__CPPLINK == G__globalcomp) {
    fprintf(fp,"extern \"C\" void G__cpp_setup_memvar%s() {\n",G__DLLID);
  }
  else {
    fprintf(fp,"void G__c_setup_memvar%s() {\n",G__DLLID);
  }
  fprintf(fp,"}\n");

  /* Following dummy comment string is needed to clear rewinded part of the
   * interface method source file. */
  fprintf(fp,"/***********************************************************\n");
  fprintf(fp,"************************************************************\n");
  fprintf(fp,"************************************************************\n");
  fprintf(fp,"************************************************************\n");
  fprintf(fp,"************************************************************\n");
  fprintf(fp,"************************************************************\n");
  fprintf(fp,"************************************************************\n");
  fprintf(fp,"***********************************************************/\n");
}

/**************************************************************************
* G__isprivatectordtorassgn()
*
**************************************************************************/
int G__isprivatectordtorassgn(tagnum,ifunc,ifn)
int tagnum;
struct G__ifunc_table *ifunc;
int ifn;
{
  /* if(G__PRIVATE!=ifunc->access[ifn]) return(0); */
  if(G__PUBLIC==ifunc->access[ifn]) return(0);
  if('~'==ifunc->funcname[ifn][0]) return(1);
  if(strcmp(ifunc->funcname[ifn],G__struct.name[tagnum])==0) return(1);
  if(strcmp(ifunc->funcname[ifn],"operator=")==0) return(1);
  return(0);
}


/**************************************************************************
* G__cpplink_memfunc()
*
**************************************************************************/
void G__cpplink_memfunc(fp)
FILE *fp;
{
#ifndef G__SMALLOBJECT
  int i,j,k;
  int hash,page;
  struct G__ifunc_table *ifunc;
  char funcname[G__MAXNAME*6];
  int isconstructor,iscopyconstructor,isdestructor,isassignmentoperator;
  /* int isvirtualdestructor; */
  char buf[G__ONELINE];
  int isnonpublicnew;
  /* struct G__ifunc_table *baseifunc; */
  /* int baseifn; */
  /* int alltag=0; */
  int virtualdtorflag;
  int dtoraccess=G__PUBLIC;

  fprintf(fp,"\n/*********************************************************\n");
  fprintf(fp,"* Member function information setup for each class\n");
  fprintf(fp,"*********************************************************/\n");

  if(G__CPPLINK == G__globalcomp) {
  }
  else {
  }

  for(i=0;i<G__struct.alltag;i++) {
    dtoraccess=G__PUBLIC;
    if((G__CPPLINK==G__struct.globalcomp[i]
	|| G__ONLYMETHODLINK==G__struct.globalcomp[i]
	)&&
       (-1==(int)G__struct.parent_tagnum[i]
	|| G__nestedclass
	)
       && -1!=G__struct.line_number[i]&&
       (G__struct.hash[i] || 0==G__struct.name[i][0])
       &&
       '$'!=G__struct.name[i][0] && 'e'!=G__struct.type[i]) {
      ifunc = G__struct.memfunc[i];
      isconstructor=0;
      iscopyconstructor=0;
      isdestructor=0;
      /* isvirtualdestructor=0; */
      isassignmentoperator=0;
      isnonpublicnew=G__isnonpublicnew(i);
      virtualdtorflag=0;

      if(G__clock)
	fprintf(fp,"static void G__setup_memfunc%s() {\n"
		,G__map_cpp_name(G__fulltagname(i,0)));
      else
	fprintf(fp,"static void G__setup_memfunc%s(void) {\n"
		,G__map_cpp_name(G__fulltagname(i,0)));

      /* link member function information */
      fprintf(fp,"   /* %s */\n",G__type2string('u',i,-1,0,0));

      fprintf(fp,"   G__tag_memfunc_setup(G__get_linked_tagnum(&%s));\n"
	      ,G__mark_linked_tagnum(i));

      if(0==G__struct.name[i][0]) {
	fprintf(fp,"}\n");
	continue;
      }

      while(ifunc) {
	for(j=0;j<ifunc->allifunc;j++) {
	  if((G__PUBLIC==ifunc->access[j]) || G__precomp_private
	     || G__isprivatectordtorassgn(i,ifunc,j)
	     || (G__PROTECTED==ifunc->access[j]&&
		 (G__PROTECTEDACCESS&G__struct.protectedaccess[i]))
	     || (G__PRIVATEACCESS&G__struct.protectedaccess[i])
	     ) {
	    if(G__ONLYMETHODLINK==G__struct.globalcomp[i]&&
	       G__METHODLINK!=ifunc->globalcomp[j]) continue;
#ifndef G__OLDIMPLEMENTATION2039
	    if(0==ifunc->hash[j]) continue;
#endif
#ifndef G__OLDIMPLEMENTATION1656
	    if(ifunc->pentry[j]->size<0) continue; /* already precompiled */
#endif
	    /* check if constructor */
	    if(strcmp(ifunc->funcname[j],G__struct.name[i])==0) {
	      if(G__struct.isabstract[i]) continue;
	      if(isnonpublicnew) continue;
	      ++isconstructor;
	      if(ifunc->para_nu[j]>=1&&
		 'u'==ifunc->para_type[j][0]&&
		 i==ifunc->para_p_tagtable[j][0]&&
		 G__PARAREFERENCE==ifunc->para_reftype[j][0]&&
		 (1==ifunc->para_nu[j]||ifunc->para_default[j][1])) {
		++iscopyconstructor;
	      }
	    }
	    else if('~'==ifunc->funcname[j][0]) {
	      /* if(ifunc->isvirtual[j]) isvirtualdestructor=1; */
	      dtoraccess = ifunc->access[j];
	      virtualdtorflag= ifunc->isvirtual[j]+ifunc->ispurevirtual[j]*2;
	      if(G__PUBLIC!=ifunc->access[j]) ++isdestructor;
	      if(G__PROTECTED==ifunc->access[j]&&G__struct.protectedaccess[i]
		 && !G__precomp_private){
		G__fprinterr(G__serr,
  "Limitation: can not generate dictionary for protected destructor for %s\n"
			,G__fulltagname(i,1));
		continue;
	      }
	      continue;
	    }
#ifdef G__DEFAULTASSIGNOPR
	    else if(strcmp(ifunc->funcname[j],"operator=")==0
		    && 'u'==ifunc->para_type[j][0] 
		    && i==ifunc->para_p_tagtable[j][0]
                    ) {
	      ++isassignmentoperator;
	    }
#endif

	    /****************************************************************
	     * setup normal function
	     ****************************************************************/
	    /* function name and return type */
	    fprintf(fp,"   G__memfunc_setup(");
	    fprintf(fp,"\"%s\",%d,",ifunc->funcname[j],ifunc->hash[j]);
	    if(G__PUBLIC==ifunc->access[j]
	       || (((G__PROTECTED==ifunc->access[j] &&
		    (G__PROTECTEDACCESS&G__struct.protectedaccess[i])) ||
		    (G__PRIVATEACCESS&G__struct.protectedaccess[i])) &&
		   '~'!=ifunc->funcname[j][0])
	       ) {
	      fprintf(fp,"%s,",G__map_cpp_funcname(i,ifunc->funcname[j]
						   ,j,ifunc->page));
	    }
	    else {
	      fprintf(fp,"(G__InterfaceMethod)NULL,");
	    }
	    fprintf(fp,"%d,",ifunc->type[j]);

	    if(-1!=ifunc->p_tagtable[j])
	      fprintf(fp,"G__get_linked_tagnum(&%s),"
		      ,G__mark_linked_tagnum(ifunc->p_tagtable[j]));
	    else
	      fprintf(fp,"-1,");

	    if(-1!=ifunc->p_typetable[j])
	      fprintf(fp,"G__defined_typename(\"%s\"),"
		      ,G__fulltypename(ifunc->p_typetable[j]));
	    else
	      fprintf(fp,"-1,");

	    fprintf(fp,"%d,",ifunc->reftype[j]);

	    /* K&R style if para_nu==-1, force it to 0 */
	    if(0>ifunc->para_nu[j]) fprintf(fp,"0,");
	    else                    fprintf(fp,"%d,",ifunc->para_nu[j]);

	    if(2==ifunc->ansi[j]) 
	      fprintf(fp,"%d," ,8 + ifunc->staticalloc[j]*2
		      + ifunc->isexplicit[j]*4);
	    else 
	      fprintf(fp,"%d," ,ifunc->ansi[j] + ifunc->staticalloc[j]*2
		      + ifunc->isexplicit[j]*4);
	    fprintf(fp,"%d,",ifunc->access[j]);
	    fprintf(fp,"%d,",ifunc->isconst[j]);
	    /* newline to avoid lines more than 256 char for CMZ */
	    if(ifunc->para_nu[j]>1) fprintf(fp,"\n");
	    fprintf(fp,"\"");

	    /****************************************************************
	     * function parameter
	     ****************************************************************/
	    for(k=0;k<ifunc->para_nu[j];k++) {
	      /* newline to avoid lines more than 256 char for CMZ */
	      if(G__CPPLINK==G__globalcomp&&k&&0==(k%2)) fprintf(fp,"\"\n\"");
	      if(isprint(ifunc->para_type[j][k])) {
		fprintf(fp,"%c ",ifunc->para_type[j][k]);
	      }
	      else {
		G__fprinterr(G__serr,"Internal error: function parameter type\n");
		fprintf(fp,"%d ",ifunc->para_type[j][k]);
	      }

	      if(-1!=ifunc->para_p_tagtable[j][k]) {
		fprintf(fp,"'%s' "
			,G__fulltagname(ifunc->para_p_tagtable[j][k],0));
		G__mark_linked_tagnum(ifunc->para_p_tagtable[j][k]);
	      }
	      else
		fprintf(fp,"- ");

	      if(-1!=ifunc->para_p_typetable[j][k])
		fprintf(fp,"'%s' "
			,G__fulltypename(ifunc->para_p_typetable[j][k]));
	      else
		fprintf(fp,"- ");

	      fprintf(fp,"%d "
		      ,ifunc->para_reftype[j][k]+ifunc->para_isconst[j][k]*10);
	      if(ifunc->para_def[j][k])
		fprintf(fp,"%s ",G__quotedstring(ifunc->para_def[j][k],buf));
	      else                          fprintf(fp,"- ");
	      if(ifunc->para_name[j][k])
		fprintf(fp,"%s",ifunc->para_name[j][k]);
	      else fprintf(fp,"-");
	      if(k!=ifunc->para_nu[j]-1) fprintf(fp," ");
	    }
	    fprintf(fp,"\"");

	    G__getcommentstring(buf,i,&ifunc->comment[j]);
	    fprintf(fp,",%s",buf);
#ifdef G__TRUEP2F
#if defined(G__OLDIMPLEMENTATION1289_YET) || !defined(G__OLDIMPLEMENTATION1993)
	    if(
#ifndef G__OLDIMPLEMENTATION1993
	       (ifunc->staticalloc[j] || 'n'==G__struct.type[i])
#else
	       ifunc->staticalloc[j]
#endif
#ifndef G__OLDIMPLEMENTATION1292
	       && G__PUBLIC==ifunc->access[j]
#endif
	       && G__MACROLINK!=ifunc->globalcomp[j]
	       ) {
#ifndef G__OLDIMPLEMENTATION1993
	      int k;
	      fprintf(fp,",(void*)(%s (*)("
		      ,G__type2string(ifunc->type[j]
				      ,ifunc->p_tagtable[j]
				      ,ifunc->p_typetable[j]
				      ,ifunc->reftype[j]
				      ,ifunc->isconst[j] /* g++ may have problem */
				      )
		      );
	      for(k=0;k<ifunc->para_nu[j];k++) {
		if(k) fprintf(fp,",");
		fprintf(fp,"%s"
			,G__type2string(ifunc->para_type[j][k]
					,ifunc->para_p_tagtable[j][k]
					,ifunc->para_p_typetable[j][k]
					,ifunc->para_reftype[j][k]
					,ifunc->para_isconst[j][k]));
	      }
	      fprintf(fp,"))(&%s::%s)"
		      ,G__fulltagname(ifunc->tagnum,1),ifunc->funcname[j]);
#else
	      fprintf(fp,",(void*)%s::%s"
		      ,G__fulltagname(ifunc->tagnum,1),ifunc->funcname[j]);
#endif
	    }
	    else
	      fprintf(fp,",(void*)NULL");
	    fprintf(fp,",%d",ifunc->isvirtual[j]+ifunc->ispurevirtual[j]*2);
#else /* 1289_YET  || !1993 */
	    fprintf(fp,",(void*)NULL,%d"
		    ,ifunc->isvirtual[j]+ifunc->ispurevirtual[j]*2);
#endif /* 1289_YET */
#endif /* G__TRUEP2F */
	    fprintf(fp,");\n");

	  } /* end of if access public && not pure virtual func */

	  else { /* in case of protected,private or pure virtual func */
	    if(strcmp(ifunc->funcname[j],G__struct.name[i])==0) {
	      ++isconstructor;
	      if('u'==ifunc->para_type[j][0]&&i==ifunc->para_p_tagtable[j][0]&&
		 G__PARAREFERENCE==ifunc->para_reftype[j][0]&&
		 (1==ifunc->para_nu[j]||ifunc->para_default[j][1])) {
		++iscopyconstructor;
	      }
	    }
	    else if('~'==ifunc->funcname[j][0]) {
	      ++isdestructor;
	    }
	    else if(strcmp(ifunc->funcname[j],"operator new")==0) {
	      ++isconstructor;
	      ++iscopyconstructor;
	    }
	    else if(strcmp(ifunc->funcname[j],"operator delete")==0) {
	      ++isdestructor;
	    }
#ifdef G__DEFAULTASSIGNOPR
	    else if(strcmp(ifunc->funcname[j],"operator=")==0
		    && 'u'==ifunc->para_type[j][0] 
		    && i==ifunc->para_p_tagtable[j][0]
		    ) {
	      ++isassignmentoperator;
	    }
#endif
	  } /* end of if access not public */

	} /* end for(j) */

	if(NULL==ifunc->next
#ifndef G__OLDIMPLEMENTATON1656
	   && G__NOLINK==G__struct.iscpplink[i]
#endif
#ifndef G__OLDIMPLEMENTATON1730
	   && G__ONLYMETHODLINK!=G__struct.globalcomp[i]
#endif
	   ) {
	  page=ifunc->page;
	  if(j==G__MAXIFUNC) {
	    j=0;
	    ++page;
	  }
	  /****************************************************************
	   * setup default constructor
	   ****************************************************************/
	  if(0==isconstructor) isconstructor=G__isprivateconstructor(i,0);
	  if('n'==G__struct.type[i]) isconstructor=1;
	  if(0==isconstructor&&0==G__struct.isabstract[i]&&0==isnonpublicnew){
	    sprintf(funcname,"%s",G__struct.name[i]);
	    G__hash(funcname,hash,k);
	    fprintf(fp,"   // automatic default constructor\n");
	    fprintf(fp,"   G__memfunc_setup(");
	    fprintf(fp,"\"%s\",%d,",funcname,hash);
	    fprintf(fp,"%s,",G__map_cpp_funcname(i ,funcname ,j,page));
	    fprintf(fp,"(int)('i'),");
	    if(strlen(G__struct.name[i])>25) fprintf(fp,"\n");
	    fprintf(fp,"G__get_linked_tagnum(&%s),"
		    ,G__mark_linked_tagnum(i));
	    fprintf(fp,"-1,"); /* typenum */
	    fprintf(fp,"0,"); /* reftype */
	    fprintf(fp,"0,"); /* para_nu */
	    fprintf(fp,"1,"); /* ansi */
	    fprintf(fp,"%d,0",G__PUBLIC);
#ifdef G__TRUEP2F
	    fprintf(fp,",\"\",(char*)NULL,(void*)NULL,%d);\n"
		    ,0);
#else
	    fprintf(fp,",\"\",(char*)NULL);\n");
#endif

	    ++j;
	    if(j==G__MAXIFUNC) {
	      j=0;
	      ++page;
	    }
	  } /* if(isconstructor) */

	  /****************************************************************
	   * setup copy constructor
	   ****************************************************************/
	  if(0==iscopyconstructor)
	    iscopyconstructor=G__isprivateconstructor(i,1);
	  if('n'==G__struct.type[i]) iscopyconstructor=1;
	  if(0==iscopyconstructor&&0==G__struct.isabstract[i]&&0==isnonpublicnew){
	    sprintf(funcname,"%s",G__struct.name[i]);
	    G__hash(funcname,hash,k);
	    fprintf(fp,"   // automatic copy constructor\n");
	    fprintf(fp,"   G__memfunc_setup(");
	    fprintf(fp,"\"%s\",%d,",funcname,hash);
	    fprintf(fp,"%s,",G__map_cpp_funcname(i ,funcname ,j,page));
	    fprintf(fp,"(int)('i'),");
	    if(strlen(G__struct.name[i])>20) fprintf(fp,"\n");
	    fprintf(fp,"G__get_linked_tagnum(&%s),"
		    ,G__mark_linked_tagnum(i));
	    fprintf(fp,"-1,"); /* typenum */
	    fprintf(fp,"0,"); /* reftype */
	    fprintf(fp,"1,"); /* para_nu */
	    fprintf(fp,"1,"); /* ansi */
	    fprintf(fp,"%d,0",G__PUBLIC);
#ifdef G__TRUEP2F
	    fprintf(fp,",\"u '%s' - 11 - -\",(char*)NULL,(void*)NULL,%d);\n"
		    ,G__fulltagname(i,0)
		    ,0);
#else
	    fprintf(fp,",\"u '%s' - 11 - -\",(char*)NULL);\n"
		    ,G__fulltagname(i,0));
#endif
	    ++j;
	    if(j==G__MAXIFUNC) {
	      j=0;
	      ++page;
	    }
	  }

	  /****************************************************************
	   * setup destructor
	   ****************************************************************/
	  if(0==isdestructor) isdestructor=G__isprivatedestructor(i);
	  if('n'==G__struct.type[i]) isdestructor=1;
	  if(
             'n'!=G__struct.type[i]
	     ) {
	    sprintf(funcname,"~%s",G__struct.name[i]);
	    G__hash(funcname,hash,k);
	    fprintf(fp,"   // automatic destructor\n");
	    fprintf(fp,"   G__memfunc_setup(");
	    fprintf(fp,"\"%s\",%d,",funcname,hash);
	    if(0==isdestructor) 
	      fprintf(fp,"%s,",G__map_cpp_funcname(i ,funcname ,j,page));
	    else 
	      fprintf(fp,"(G__InterfaceMethod)NULL,");
	    fprintf(fp,"(int)('y'),");
	    fprintf(fp,"-1,"); /* tagnum */
	    fprintf(fp,"-1,"); /* typenum */
	    fprintf(fp,"0,"); /* reftype */
	    fprintf(fp,"0,"); /* para_nu */
	    fprintf(fp,"1,"); /* ansi */
	    fprintf(fp,"%d,0",dtoraccess);
#ifdef G__TRUEP2F
	    fprintf(fp,",\"\",(char*)NULL,(void*)NULL,%d);\n"
		    ,virtualdtorflag);
#else
	    fprintf(fp,",\"\",(char*)NULL);\n");
#endif
	    if(0==isdestructor) ++j;
	    if(j==G__MAXIFUNC) {
	      j=0;
	      ++page;
	    }
	  }

#ifdef G__DEFAULTASSIGNOPR
	  /****************************************************************
	   * setup assignment operator
	   ****************************************************************/
	  if(0==isassignmentoperator) 
	    isassignmentoperator=G__isprivateassignopr(i);
	  if('n'==G__struct.type[i]) isassignmentoperator=1;
	  if(0==isassignmentoperator) {
	    sprintf(funcname,"operator=");
	    G__hash(funcname,hash,k);
	    fprintf(fp,"   // automatic assignment operator\n");
	    fprintf(fp,"   G__memfunc_setup(");
	    fprintf(fp,"\"%s\",%d,",funcname,hash);
	    fprintf(fp,"%s,",G__map_cpp_funcname(i ,funcname ,j,page));
	    fprintf(fp,"(int)('u'),");
	    fprintf(fp,"G__get_linked_tagnum(&%s),"
		    ,G__mark_linked_tagnum(i));
	    fprintf(fp,"-1,"); /* typenum */
	    fprintf(fp,"1,"); /* reftype */
	    fprintf(fp,"1,"); /* para_nu */
	    fprintf(fp,"1,"); /* ansi */
	    fprintf(fp,"%d,0",G__PUBLIC);
#ifdef G__TRUEP2F
	    fprintf(fp,",\"u '%s' - 11 - -\",(char*)NULL,(void*)NULL,%d);\n"
		    ,G__fulltagname(i,0)
		    ,0);
#else
	    fprintf(fp,",\"u '%s' - 11 - -\",(char*)NULL);\n"
		    ,G__fulltagname(i,0));
#endif
	  }
#endif
	} /* end of ifunc->next */
	ifunc = ifunc->next;
      } /* end while(ifunc) */
      fprintf(fp,"   G__tag_memfunc_reset();\n");
      fprintf(fp,"}\n\n");
    } /* end if(globalcomp) */
  } /* end for(i) */

  fprintf(fp,"\n/*********************************************************\n");
  fprintf(fp,"* Member function information setup\n");
  fprintf(fp,"*********************************************************/\n");

  if(G__CPPLINK == G__globalcomp) {
    fprintf(fp,"extern \"C\" void G__cpp_setup_memfunc%s() {\n",G__DLLID);
  }
  else {
    /* fprintf(fp,"void G__c_setup_memfunc%s() {\n",G__DLLID); */
  }

  fprintf(fp,"}\n");

#endif
}


/**************************************************************************
* G__cpplink_global()
*
**************************************************************************/
void G__cpplink_global(fp)
FILE *fp;
{
#ifndef G__SMALLOBJECT
  int j,k;
  struct G__var_array *var;
  int pvoidflag;
  G__value buf;
  char value[G__ONELINE],ttt[G__ONELINE];
  int divn=0;
  int maxfnc=100;
  int fnc=0;

  fprintf(fp,"\n/*********************************************************\n");
  fprintf(fp,"* Global variable information setup for each class\n");
  fprintf(fp,"*********************************************************/\n");

#ifdef G__BORLANDCC5
  fprintf(fp,"static void G__cpp_setup_global%d(void) {\n",divn++);
#else
  fprintf(fp,"static void G__cpp_setup_global%d() {\n",divn++);
#endif

  fprintf(fp,"\n   /* Setting up global variables */\n");
  var = &G__global;
  fprintf(fp,"   G__resetplocal();\n\n");

  while((struct G__var_array*)NULL!=var) {
    for(j=0;j<var->allvar;j++) {
      if(fnc++>maxfnc) {
	fnc=0;
	fprintf(fp,"}\n\n");
#ifdef G__BORLANDCC5
	fprintf(fp,"static void G__cpp_setup_global%d(void) {\n",divn++);
#else
	fprintf(fp,"static void G__cpp_setup_global%d() {\n",divn++);
#endif
      }
      if((G__AUTO==var->statictype[j] /* not static */ ||
	  (0==var->p[j] && G__COMPILEDGLOBAL==var->statictype[j] &&
	   INT_MAX == var->varlabel[j][1])) && /* extern type v[]; */
	 G__NOLINK>var->globalcomp[j] &&   /* with -c-1 or -c-2 option */
#ifndef G__OLDIMPLEMENTATION2191
	 'j'!=tolower(var->type[j]) /* questionable */
#else
	 'm'!=tolower(var->type[j])
#endif
	 && var->varnamebuf[j][0]
	 ) {

	if((-1!=var->p_tagtable[j]&&
	    islower(var->type[j])&&var->constvar[j]&&
	    'e'==G__struct.type[var->p_tagtable[j]])
	   || 'p'==tolower(var->type[j])
	   || 'T'==var->type[j]
#ifdef G__UNADDRESSABLEBOOL
	   || 'g'==var->type[j]
#endif
	   )
	  pvoidflag=1;
	else
	  pvoidflag=0;

	fprintf(fp,"   G__memvar_setup(");
	if(pvoidflag) fprintf(fp,"(void*)G__PVOID,");
	else {
	  fprintf(fp,"(void*)(&%s),",var->varnamebuf[j]);
#ifdef G__GENWINDEF
	  fprintf(G__WINDEFfp,"        %s @%d\n"
		  ,var->varnamebuf[j] ,++G__nexports);
#endif
	}
	fprintf(fp,"%d,",var->type[j]);
	fprintf(fp,"%d,",var->reftype[j]);
	fprintf(fp,"%d,",var->constvar[j]);

	if(-1!=var->p_tagtable[j])
	  fprintf(fp,"G__get_linked_tagnum(&%s),"
		  ,G__mark_linked_tagnum(var->p_tagtable[j]));
	else
	  fprintf(fp,"-1,");

	if(-1!=var->p_typetable[j])
	  fprintf(fp,"G__defined_typename(\"%s\"),"
		  ,G__newtype.name[var->p_typetable[j]]);
	else
	  fprintf(fp,"-1,");

	fprintf(fp,"%d,",var->statictype[j]);
	fprintf(fp,"%d,",var->access[j]);
	fprintf(fp,"\"%s"
		,var->varnamebuf[j]);
	if(INT_MAX==var->varlabel[j][1] /* && 1== var->varlabel[j][0] */ ) 
	  fprintf(fp,"[]");
	else if(var->varlabel[j][1])
	  fprintf(fp,"[%d]",
		  (var->varlabel[j][1]+1)/var->varlabel[j][0]);
	for(k=1;k<var->paran[j];k++) {
	  fprintf(fp,"[%d]",var->varlabel[j][k+1]);
	}
	if(pvoidflag) {
	  buf = G__getitem(var->varnamebuf[j]);
	  G__string(buf,value);
	  G__quotedstring(value,ttt);
	  if('p'==tolower(var->type[j])
	     || 'T'==var->type[j]
	     )
	    fprintf(fp,"=%s\",1,(char*)NULL);\n",ttt);
	  else
	    fprintf(fp,"=%s\",0,(char*)NULL);\n",ttt);
	}
	else fprintf(fp,"=\",0,(char*)NULL);\n");
      } /* end if(G__PUBLIC) */
      G__var_type='p';
    } /* end for(j) */
    var=var->next;
  }  /* end while(var) */

  fprintf(fp,"\n");
  fprintf(fp,"   G__resetglobalenv();\n");

  fprintf(fp,"}\n");

  if(G__CPPLINK == G__globalcomp) {
    fprintf(fp,"extern \"C\" void G__cpp_setup_global%s() {\n",G__DLLID);
  }
  else {
    fprintf(fp,"void G__c_setup_global%s() {\n",G__DLLID);
  }
  for(fnc=0;fnc<divn;fnc++) {
    fprintf(fp,"  G__cpp_setup_global%d();\n",fnc);
  }
  fprintf(fp,"}\n");


#endif
}

#ifdef G__P2FDECL  /* used to be G__TRUEP2F */
/**************************************************************************
* G__declaretruep2f()
*
**************************************************************************/
static void G__declaretruep2f(fp,ifunc,j)
FILE *fp;
struct G__ifunc_table *ifunc;
int j;
{
#ifdef G__P2FDECL
  int i;
  int ifndefflag=1;
  if(strncmp(ifunc->funcname[j],"operator",8)==0) ifndefflag=0;
  if(ifndefflag) {
    switch(G__globalcomp) {
    case G__CPPLINK:
      if(G__MACROLINK==ifunc->globalcomp[j]||
	 0==
	 strcmp("iterator_category",ifunc->funcname[j])) fprintf(fp,"#if 0\n");
      else fprintf(fp,"#ifndef %s\n",ifunc->funcname[j]);
      fprintf(fp,"%s (*%sp2f)("
	      ,G__type2string(ifunc->type[j]
			      ,ifunc->p_tagtable[j]
			      ,ifunc->p_typetable[j]
			      ,ifunc->reftype[j]
			      ,ifunc->isconst[j]
			      /* ,0  avoiding g++ bug */
			      )
	      ,G__map_cpp_funcname(-1,ifunc->funcname[j],j,ifunc->page)
	      );
      for(i=0;i<ifunc->para_nu[j];i++) {
	if(i) fprintf(fp,",");
	fprintf(fp,"%s"
		,G__type2string(ifunc->para_type[j][i]
				,ifunc->para_p_tagtable[j][i]
				,ifunc->para_p_typetable[j][i]
				,ifunc->para_reftype[j][i]
				,ifunc->para_isconst[j][i]));
      }
      fprintf(fp,") = %s;\n",ifunc->funcname[j]);
      fprintf(fp,"#else\n");
      fprintf(fp,"void* %sp2f = (void*)NULL;\n"
	      ,G__map_cpp_funcname(-1,ifunc->funcname[j],j,ifunc->page));
      fprintf(fp,"#endif\n");
      break;
    default:
      break;
    }
  }
#else
  if(fp && ifunc && j) return;
#endif
}
#endif

#ifdef G__TRUEP2F 
/**************************************************************************
* G__printtruep2f()
*
**************************************************************************/
static void G__printtruep2f(fp,ifunc,j)
FILE *fp;
struct G__ifunc_table *ifunc;
int j;
{
#if defined(G__P2FCAST)
  int i;
#endif
  int ifndefflag=1;
#if defined(G__FUNCPOINTER)
  if(strncmp(ifunc->funcname[j],"operator",8)==0)
    ifndefflag=0;
#else
  ifndefflag=0;
#endif
  if(ifndefflag) {
    switch(G__globalcomp) {
    case G__CPPLINK:
#if defined(G__P2FDECL)
      fprintf(fp,",(void*)%sp2f);\n"
	      ,G__map_cpp_funcname(-1,ifunc->funcname[j],j,ifunc->page));
#elif defined(G__P2FCAST)
      if(G__MACROLINK==ifunc->globalcomp[j]||
	 0==
	 strcmp("iterator_category",ifunc->funcname[j])) fprintf(fp,"#if 0\n");
      else fprintf(fp,"#ifndef %s\n",ifunc->funcname[j]);

      fprintf(fp,",(void*)(%s (*)("
	      ,G__type2string(ifunc->type[j]
			      ,ifunc->p_tagtable[j]
			      ,ifunc->p_typetable[j]
			      ,ifunc->reftype[j]
			      ,ifunc->isconst[j] /* g++ may have problem */
			      )
	      /* ,G__map_cpp_funcname(-1,ifunc->funcname[j],j,ifunc->page) */
	      );
      for(i=0;i<ifunc->para_nu[j];i++) {
	if(i) fprintf(fp,",");
	fprintf(fp,"%s"
		,G__type2string(ifunc->para_type[j][i]
				,ifunc->para_p_tagtable[j][i]
				,ifunc->para_p_typetable[j][i]
				,ifunc->para_reftype[j][i]
				,ifunc->para_isconst[j][i]));
      }
      fprintf(fp,"))%s,%d);\n",ifunc->funcname[j]
	      ,ifunc->isvirtual[j]+ifunc->ispurevirtual[j]*2);
      fprintf(fp,"#else\n");
      fprintf(fp,",(void*)NULL,%d);\n"
	      ,ifunc->isvirtual[j]+ifunc->ispurevirtual[j]*2);
      fprintf(fp,"#endif\n");
#else
      fprintf(fp,",(void*)NULL,%d);\n"
	      ,ifunc->isvirtual[j]+ifunc->ispurevirtual[j]*2);
#endif
      break;
    case G__CLINK:
    default:
      fprintf(fp,"#ifndef %s\n",ifunc->funcname[j]);
      fprintf(fp,",(void*)%s,%d);\n",ifunc->funcname[j]
	      ,ifunc->isvirtual[j]+ifunc->ispurevirtual[j]*2);
      fprintf(fp,"#else\n");
      fprintf(fp,",(void*)NULL,%d);\n"
	      ,ifunc->isvirtual[j]+ifunc->ispurevirtual[j]*2);
      fprintf(fp,"#endif\n");
      break;
    }
  }
  else {
    fprintf(fp,",(void*)NULL,%d);\n"
	      ,ifunc->isvirtual[j]+ifunc->ispurevirtual[j]*2);
  }
}
#endif

/**************************************************************************
* G__cpplink_func()
*
*  making C++ link routine to global function
**************************************************************************/
void G__cpplink_func(fp)
FILE *fp;
{
  int j,k;
  struct G__ifunc_table *ifunc;
  char buf[G__ONELINE];
  int divn=0;
  int maxfnc=100;
  int fnc=0;

  fprintf(fp,"\n/*********************************************************\n");
  fprintf(fp,"* Global function information setup for each class\n");
  fprintf(fp,"*********************************************************/\n");

#ifdef G__BORLANDCC5
  fprintf(fp,"static void G__cpp_setup_func%d(void) {\n",divn++);
#else
  fprintf(fp,"static void G__cpp_setup_func%d() {\n",divn++);
#endif

  ifunc = &G__ifunc;

  fprintf(fp,"   G__lastifuncposition();\n\n");

  while((struct G__ifunc_table*)NULL!=ifunc) {
    for(j=0;j<ifunc->allifunc;j++) {
      if(fnc++>maxfnc) {
	fnc=0;
	fprintf(fp,"}\n\n");
#ifdef G__BORLANDCC5
	fprintf(fp,"static void G__cpp_setup_func%d(void) {\n",divn++);
#else
	fprintf(fp,"static void G__cpp_setup_func%d() {\n",divn++);
#endif
      }
      if(G__NOLINK>ifunc->globalcomp[j] &&  /* with -c-1 option */
	 G__PUBLIC==ifunc->access[j] && /* public, this is always true */
	 0==ifunc->staticalloc[j] &&
	 ifunc->hash[j]) {   /* not static */

	if(strcmp(ifunc->funcname[j],"operator new")==0 &&
	   (ifunc->para_nu[j]==2 || ifunc->para_default[j][2])) {
	  G__is_operator_newdelete |= G__IS_OPERATOR_NEW;
	}
	else if(strcmp(ifunc->funcname[j],"operator delete")==0) {
	  G__is_operator_newdelete |= G__IS_OPERATOR_DELETE;
	}

#ifdef G__P2FDECL  /* used to be G__TRUEP2F */
	G__declaretruep2f(fp,ifunc,j);
#endif

	/* function name and return type */
	fprintf(fp,"   G__memfunc_setup(");
	fprintf(fp,"\"%s\",%d,",ifunc->funcname[j],ifunc->hash[j]);
	fprintf(fp,"%s,",G__map_cpp_funcname(-1
					     ,ifunc->funcname[j]
					     ,j,ifunc->page));
	fprintf(fp,"%d,",ifunc->type[j]);

	if(-1!=ifunc->p_tagtable[j])
	  fprintf(fp,"G__get_linked_tagnum(&%s),"
		  ,G__mark_linked_tagnum(ifunc->p_tagtable[j]));
	else
	  fprintf(fp,"-1,");

	if(-1!=ifunc->p_typetable[j])
	  fprintf(fp,"G__defined_typename(\"%s\"),"
		  ,G__newtype.name[ifunc->p_typetable[j]]);
	else
	  fprintf(fp,"-1,");

	fprintf(fp,"%d,",ifunc->reftype[j]);

	/* K&R style if para_nu==-1, force it to 0 */
	if(0>ifunc->para_nu[j]) fprintf(fp,"0,");
	else                    fprintf(fp,"%d,",ifunc->para_nu[j]);

	if(2==ifunc->ansi[j]) 
	  fprintf(fp,"%d,",8 + ifunc->staticalloc[j]*2);
	else
	  fprintf(fp,"%d,",ifunc->ansi[j] + ifunc->staticalloc[j]*2);
	fprintf(fp,"%d,",ifunc->access[j]);
	fprintf(fp,"%d,",ifunc->isconst[j]);

	/* newline to avoid lines more than 256 char for CMZ */
	if(ifunc->para_nu[j]>1) fprintf(fp,"\n");
	fprintf(fp,"\"");

	/****************************************************************
	 * function parameter
	 ****************************************************************/
	for(k=0;k<ifunc->para_nu[j];k++) {
	  /* newline to avoid lines more than 256 char for CMZ */
	  if(G__CPPLINK==G__globalcomp&&k&&0==(k%2)) fprintf(fp,"\"\n\"");
	  if(isprint(ifunc->para_type[j][k])) {
	    fprintf(fp,"%c ",ifunc->para_type[j][k]);
	  }
	  else {
	    G__fprinterr(G__serr,"Internal error: function parameter type\n");
	    fprintf(fp,"%d ",ifunc->para_type[j][k]);
	  }

	  if(-1!=ifunc->para_p_tagtable[j][k]) {
	    fprintf(fp,"'%s' "
		    ,G__fulltagname(ifunc->para_p_tagtable[j][k],0));
	    G__mark_linked_tagnum(ifunc->para_p_tagtable[j][k]);
	  }
	  else
	    fprintf(fp,"- ");

	  if(-1!=ifunc->para_p_typetable[j][k])
	    fprintf(fp,"'%s' "
		    ,G__newtype.name[ifunc->para_p_typetable[j][k]]);
	  else
	    fprintf(fp,"- ");

	  fprintf(fp,"%d "
		,ifunc->para_reftype[j][k]+ifunc->para_isconst[j][k]*10);
	  if(ifunc->para_def[j][k])
	    fprintf(fp,"%s ",G__quotedstring(ifunc->para_def[j][k],buf));
	  else fprintf(fp,"- ");
	  if(ifunc->para_name[j][k])
	    fprintf(fp,"%s",ifunc->para_name[j][k]);
	  else fprintf(fp,"-");
	  if(k!=ifunc->para_nu[j]-1) fprintf(fp," ");
	}
#ifdef G__TRUEP2F
	fprintf(fp,"\",(char*)NULL\n");
	G__printtruep2f(fp,ifunc,j);
#else
	fprintf(fp,"\",(char*)NULL);\n");
#endif

      }
    } /* end for(j) */
    ifunc = ifunc->next;
  } /* end while(ifunc) */

  fprintf(fp,"\n");
  fprintf(fp,"   G__resetifuncposition();\n");

  /********************************************************
  * call user initialization function if specified
  ********************************************************/
  if(G__INITFUNC) {
    fprintf(fp,"  %s();\n",G__INITFUNC);
  }

  fprintf(fp,"}\n\n");

  if(G__CPPLINK == G__globalcomp) {
    fprintf(fp,"extern \"C\" void G__cpp_setup_func%s() {\n",G__DLLID);
  }
  else {
    fprintf(fp,"void G__c_setup_func%s() {\n",G__DLLID);
  }
  for(fnc=0;fnc<divn;fnc++) {
    fprintf(fp,"  G__cpp_setup_func%d();\n",fnc);
  }
  fprintf(fp,"}\n");
}

/**************************************************************************
**************************************************************************
*  Functions used in G__cpplink.C to bind C++ symbols
*
**************************************************************************
**************************************************************************/

/**************************************************************************
* G__tagtable_setup()
*
*  Used in G__cpplink.C
**************************************************************************/
int G__tagtable_setup(tagnum,size,cpplink,isabstract,comment
		      ,setup_memvar, setup_memfunc)
int tagnum;
int size;
int cpplink;
int isabstract;
char *comment;
G__incsetup setup_memvar;
G__incsetup setup_memfunc;
{
  char *p;
#ifndef G__OLDIMPLEMENTATION1823
  char xbuf[G__BUFLEN];
  char *buf=xbuf;
#else
  char buf[G__ONELINE];
#endif
  if(0==size && 0!=G__struct.size[tagnum]
     && 'n'!=G__struct.type[tagnum]
     ) return(0);

  if(0!=G__struct.size[tagnum]
     && 'n'!=G__struct.type[tagnum]
     ) {
#ifndef G__OLDIMPLEMENTATION1656
    if(G__struct.incsetup_memvar[tagnum])
      (*G__struct.incsetup_memvar[tagnum])();
    if(G__struct.incsetup_memfunc[tagnum])
      (*G__struct.incsetup_memfunc[tagnum])();

    if(G__struct.incsetup_memvar[tagnum] != setup_memvar)
      G__struct.incsetup_memvar[tagnum] = setup_memvar;
    else
      G__struct.incsetup_memvar[tagnum] = (G__incsetup)NULL;

    if(G__struct.incsetup_memfunc[tagnum] != setup_memfunc)
      G__struct.incsetup_memfunc[tagnum] = setup_memfunc;
    else
      G__struct.incsetup_memfunc[tagnum] = (G__incsetup)NULL;

#endif /* 1656 */
    if(G__asm_dbg ) {
      if(G__dispmsg>=G__DISPWARN) {
	G__fprinterr(G__serr,"Warning: Try to reload %s from DLL\n"
		     ,G__fulltagname(tagnum,1));
      }
    }
    return(0);
  }
  G__struct.size[tagnum] = size;
  G__struct.iscpplink[tagnum] = cpplink;
#if  !defined(G__OLDIMPLEMENTATION1545)
  G__struct.rootflag[tagnum] = (isabstract/0x10000)%0x100;
  G__struct.funcs[tagnum] = (isabstract/0x100)%0x100;
  G__struct.isabstract[tagnum] = isabstract%0x100;
#else
  G__struct.funcs[tagnum] = isabstract/0x100;
  G__struct.isabstract[tagnum] = isabstract%0x100;
#endif
  G__struct.filenum[tagnum] = G__ifile.filenum;

  G__struct.comment[tagnum].p.com = comment;
  if(comment) G__struct.comment[tagnum].filenum = -2;
  else        G__struct.comment[tagnum].filenum = -1;
  if(G__struct.incsetup_memvar[tagnum])
    (*G__struct.incsetup_memvar[tagnum])();
  if(G__struct.incsetup_memfunc[tagnum])
    (*G__struct.incsetup_memfunc[tagnum])();
  if(0==G__struct.memvar[tagnum]->allvar
     || 'n'==G__struct.type[tagnum])
    G__struct.incsetup_memvar[tagnum] = setup_memvar;
  else
    G__struct.incsetup_memvar[tagnum] = 0;
  if(
#ifndef G__OLDIMPLEMENTATION2027
     1==G__struct.memfunc[tagnum]->allifunc 
#else
     0==G__struct.memfunc[tagnum]->allifunc 
#endif
     || 'n'==G__struct.type[tagnum]
     || (
#if !defined(G__OLDIMPLEMENTATION2027)
	 -1!=G__struct.memfunc[tagnum]->pentry[1]->size
#else
	 -1!=G__struct.memfunc[tagnum]->pentry[0]->size
#endif
	 && 2>=G__struct.memfunc[tagnum]->allifunc))
    G__struct.incsetup_memfunc[tagnum] = setup_memfunc;
  else 
    G__struct.incsetup_memfunc[tagnum] = 0;

  /* add template names */
#ifndef G__OLDIMPLEMENTATION1823
  if(strlen(G__struct.name[tagnum])>G__BUFLEN-10) {
    buf = (char*)malloc(strlen(G__struct.name[tagnum])+10);
  }
#endif
  strcpy(buf,G__struct.name[tagnum]);
  if((p=strchr(buf,'<'))) {
    *p='\0';
    if(!G__defined_templateclass(buf)) {
      int store_def_tagnum = G__def_tagnum;
      int store_tagdefining = G__tagdefining;
      FILE* store_fp = G__ifile.fp;
      G__ifile.fp = (FILE*)NULL;
      G__def_tagnum = G__struct.parent_tagnum[tagnum];
      G__tagdefining = G__struct.parent_tagnum[tagnum];
      G__createtemplateclass(buf,(struct G__Templatearg*)NULL,0);
      G__ifile.fp = store_fp;
      G__def_tagnum = store_def_tagnum;
      G__tagdefining = store_tagdefining;
    }
  }
#ifndef G__OLDIMPLEMENTATION1823
  if(buf!=xbuf) free((void*)buf);
#endif
  return(0);
}

/**************************************************************************
* G__inheritance_setup()
*
*  Used in G__cpplink.C
**************************************************************************/
int G__inheritance_setup(tagnum,basetagnum
			 ,baseoffset,baseaccess
			 ,property
			 )
int tagnum,basetagnum;
long baseoffset;
int baseaccess;
int property;
{
#ifndef G__SMALLOBJECT
  int basen;
  G__ASSERT(0<=tagnum && 0<=basetagnum);
  basen=G__struct.baseclass[tagnum]->basen;
  G__struct.baseclass[tagnum]->basetagnum[basen] = basetagnum;
  G__struct.baseclass[tagnum]->baseoffset[basen]=baseoffset;
  G__struct.baseclass[tagnum]->baseaccess[basen]=baseaccess;
  G__struct.baseclass[tagnum]->property[basen]=property;
  ++G__struct.baseclass[tagnum]->basen;
#endif
  return(0);
}


/**************************************************************************
* G__tag_memvar_setup()
*
* Used in G__cpplink.C
**************************************************************************/
int G__tag_memvar_setup(tagnum)
int tagnum;
{
#ifndef G__OLDIMPLEMENTATON285
  G__incset_tagnum = G__tagnum;
  G__incset_p_local = G__p_local;
  G__incset_def_struct_member = G__def_struct_member;
  G__incset_tagdefining = G__tagdefining;
  G__incset_globalvarpointer = G__globalvarpointer ;
  G__incset_var_type = G__var_type ;
  G__incset_typenum = G__typenum ;
  G__incset_static_alloc = G__static_alloc ;
  G__incset_access = G__access ;
#endif
  G__tagnum = tagnum;
  G__p_local=G__struct.memvar[G__tagnum];
  G__def_struct_member = 1;
  G__incset_def_tagnum = G__def_tagnum;
  G__def_tagnum = G__struct.parent_tagnum[G__tagnum];
  G__tagdefining=G__tagnum;
  return(0);
}

/**************************************************************************
* G__memvar_setup()
*
* Used in G__cpplink.C
**************************************************************************/
int G__memvar_setup(p,type
		    ,reftype
		    ,constvar,tagnum,typenum,statictype,accessin
		    ,expr,definemacro,comment)
void *p;
int type;
int reftype;
int constvar,tagnum,typenum,statictype,accessin;
char *expr;
int definemacro;
char *comment;
{
  int store_asm_noverflow;
  int store_prerun;
  int store_asm_wholefunction;
  int store_constvar = G__constvar;
  int store_def_struct_member = G__def_struct_member;
  int store_tagdefining = G__tagdefining;
  struct G__var_array *store_p_local = G__p_local;
  if('p'==type && G__def_struct_member) {
    G__def_struct_member = 0;
    G__tagdefining = -1;
    G__p_local = (struct G__var_array*)0;
  }

  G__setcomment = comment;

  G__globalvarpointer = (long)p;
  G__var_type = type;
  G__reftype = reftype;
  G__tagnum = tagnum;
  G__typenum = typenum;
  if(G__AUTO == statictype) G__static_alloc=0;
  else                      G__static_alloc=1;
  /* G__access = constvar;*/  /* dummy statement to avoid lint error */
  G__constvar = constvar; /* Not sure why I didn't do this for a long time */
  G__access = accessin;
  G__definemacro=definemacro;
  store_asm_noverflow=G__asm_noverflow;
  G__asm_noverflow=0;
  store_prerun = G__prerun;
  G__prerun=1;
  store_asm_wholefunction = G__asm_wholefunction;
  G__asm_wholefunction = G__ASM_FUNC_NOP;
  G__getexpr(expr);
  if('p'==type && store_def_struct_member) {
    G__def_struct_member = store_def_struct_member;
    G__tagdefining = store_tagdefining;
    G__p_local = store_p_local;
  }
  G__asm_wholefunction = store_asm_wholefunction;
  G__prerun=store_prerun;
  G__asm_noverflow=store_asm_noverflow;
  G__definemacro=0;
  G__setcomment = (char*)NULL;
  G__reftype=G__PARANORMAL;
  G__constvar = store_constvar;
  return(0);
}

/**************************************************************************
* G__tag_memvar_reset()
*
* Used in G__cpplink.C
**************************************************************************/
int G__tag_memvar_reset()
{
  G__p_local = G__incset_p_local ;
  G__def_struct_member = G__incset_def_struct_member ;
  G__tagdefining = G__incset_tagdefining ;
  G__def_tagnum = G__incset_def_tagnum;

  G__globalvarpointer = G__incset_globalvarpointer ;
  G__var_type = G__incset_var_type ;
  G__tagnum = G__incset_tagnum ;
  G__typenum = G__incset_typenum ;
  G__static_alloc = G__incset_static_alloc ;
  G__access = G__incset_access ;
  return(0);
}

/**************************************************************************
* G__usermemfunc_setup
*
**************************************************************************/
int G__usermemfunc_setup(funcname,hash,funcp,type,tagnum,typenum,reftype,
                         para_nu,ansi,accessin,isconst,paras,comment
#ifdef G__TRUEP2F
			 ,truep2f,isvirtual
#endif
			 ,userparam
			 )
int hash,(*funcp)(),type,tagnum,typenum,reftype, para_nu,ansi,accessin,isconst;
char *funcname, *paras, *comment;
#ifdef G__TRUEP2F
void* truep2f;
int isvirtual;
#endif
void* userparam;
{
  G__p_ifunc->userparam[G__p_ifunc->allifunc] = userparam;
  return G__memfunc_setup(funcname,hash,funcp,type,tagnum,typenum,reftype,
                          para_nu,ansi,accessin,isconst,paras,comment
#ifdef G__TRUEP2F
		     ,truep2f,isvirtual
#endif
		     );
}


/**************************************************************************
* G__tag_memfunc_setup()
*
* Used in G__cpplink.C
**************************************************************************/
int G__tag_memfunc_setup(tagnum)
int tagnum;
{
  G__incset_tagnum = G__tagnum;
  G__incset_p_ifunc = G__p_ifunc;
  G__incset_func_now = G__func_now;
  G__incset_func_page = G__func_page;
  G__incset_var_type = G__var_type;
  G__incset_tagdefining = G__tagdefining;
  G__incset_def_tagnum = G__def_tagnum;
  G__tagdefining = G__struct.parent_tagnum[tagnum];
  G__def_tagnum = G__tagdefining;
  G__tagnum = tagnum;
  G__p_ifunc = G__struct.memfunc[G__tagnum];

  while(G__p_ifunc->next) G__p_ifunc=G__p_ifunc->next;

  --G__p_ifunc->allifunc;
  G__memfunc_next();
  return(0);
}

/**************************************************************************
* G__memfunc_setup()
*
* Used in G__cpplink.C
**************************************************************************/
int G__memfunc_setup(funcname,hash,funcp
		     ,type,tagnum,typenum,reftype
		     ,para_nu,ansi,accessin,isconst
		     ,paras,comment
#ifdef G__TRUEP2F
		     ,truep2f
		     ,isvirtual
#endif
		     )
char *funcname;
int hash;
int (*funcp)();
int type,tagnum,typenum,reftype;
int para_nu,ansi,accessin,isconst;
char *paras;
char *comment;
#ifdef G__TRUEP2F
void* truep2f;
int isvirtual;
#endif
{
#ifndef G__SMALLOBJECT
#ifndef G__OLDIMPLEMENTATION2027
  int store_func_now = -1;
  struct G__ifunc_table *store_p_ifunc = 0;
  int dtorflag=0;
#endif

  G__func_now=G__p_ifunc->allifunc;

#ifndef G__OLDIMPLEMENTATION2027
  if('~'==funcname[0] && 0==G__struct.memfunc[G__p_ifunc->tagnum]->hash[0]) {
    store_func_now = G__func_now;
    store_p_ifunc = G__p_ifunc; 
    G__p_ifunc = G__struct.memfunc[G__p_ifunc->tagnum];
    G__func_now = 0;
    dtorflag=1;
  }
#endif

  G__savestring(&G__p_ifunc->funcname[G__func_now],funcname);
  G__p_ifunc->hash[G__func_now] = hash;

  /* set entry pointer */
  G__p_ifunc->pentry[G__func_now] = &G__p_ifunc->entry[G__func_now];
  G__p_ifunc->entry[G__func_now].p=(void*)funcp;
#ifndef G__OLDIMLEMENTATION2012
  if(-1!=G__p_ifunc->tagnum) 
    G__p_ifunc->entry[G__func_now].filenum=G__struct.filenum[G__p_ifunc->tagnum];
  else G__p_ifunc->entry[G__func_now].filenum = G__ifile.filenum;
  G__p_ifunc->entry[G__func_now].size = -1;
#else
  G__p_ifunc->entry[G__func_now].filenum = -1;
#endif
  G__p_ifunc->entry[G__func_now].line_number = -1;
#ifdef G__ASM_WHOLEFUNC
  G__p_ifunc->entry[G__func_now].bytecode = (struct G__bytecodefunc*)NULL;
#endif
#ifdef G__TRUEP2F
  if(truep2f) G__p_ifunc->entry[G__func_now].tp2f=truep2f;
  else G__p_ifunc->entry[G__func_now].tp2f=(void*)funcp;
#endif

  G__p_ifunc->type[G__func_now] = type;
  G__p_ifunc->p_tagtable[G__func_now] = tagnum;
  G__p_ifunc->p_typetable[G__func_now] = typenum;
  G__p_ifunc->reftype[G__func_now] = reftype;

  G__p_ifunc->para_nu[G__func_now] = para_nu;
  if(ansi&8) G__p_ifunc->ansi[G__func_now] = 2;
  else if(ansi&1) G__p_ifunc->ansi[G__func_now] = 1;

  G__p_ifunc->isconst[G__func_now] = isconst;

  G__p_ifunc->busy[G__func_now] = 0;

  G__p_ifunc->access[G__func_now] = accessin;

  G__p_ifunc->globalcomp[G__func_now] = G__NOLINK;
  G__p_ifunc->isexplicit[G__func_now] = (ansi&4)/4;
  G__p_ifunc->staticalloc[G__func_now] = (ansi&2)/2;
#ifdef G__TRUEP2F
  G__p_ifunc->isvirtual[G__func_now] = isvirtual&0x01;
  G__p_ifunc->ispurevirtual[G__func_now] = (isvirtual&0x02)/2;
#else
  G__p_ifunc->isvirtual[G__func_now] = 0;
  G__p_ifunc->ispurevirtual[G__func_now] = 0;
#endif

  G__p_ifunc->para_name[G__func_now][0]=(char*)NULL;
  /* parse parameter setup information */
  G__parse_parameter_link(paras);

#ifdef G__FRIEND
  G__p_ifunc->friendtag[G__func_now] = (struct G__friendtag*)NULL;
#endif

  G__p_ifunc->comment[G__func_now].p.com = comment;
  if(comment) G__p_ifunc->comment[G__func_now].filenum = -2;
  else        G__p_ifunc->comment[G__func_now].filenum = -1;

  /* end */

#ifndef G__OLDIMPLEMENTATION1702
 {
   struct G__ifunc_table *ifunc;
   int iexist;
   if(-1==G__p_ifunc->tagnum) 
     ifunc = G__ifunc_exist(G__p_ifunc,G__func_now
			    ,&G__ifunc,&iexist,0xffff);
   else
     ifunc = G__ifunc_exist(G__p_ifunc,G__func_now
			    ,G__struct.memfunc[G__p_ifunc->tagnum],&iexist
			    ,0xffff);
   
   if(ifunc) {
     /* Overriding old function definition */
     int func_now = G__func_now;
     int paranu,iin;
     /* Overriding old function definition */
     ifunc->ansi[iexist]=G__p_ifunc->ansi[func_now];
     if(-1==G__p_ifunc->para_nu[func_now]) paranu=0;
     else paranu=ifunc->para_nu[iexist];
     if(0==ifunc->ansi[iexist]) 
       ifunc->para_nu[iexist] = G__p_ifunc->para_nu[func_now];
     ifunc->type[iexist]=G__p_ifunc->type[func_now];
     ifunc->p_tagtable[iexist]=G__p_ifunc->p_tagtable[func_now];
     ifunc->p_typetable[iexist]=G__p_ifunc->p_typetable[func_now];
     ifunc->reftype[iexist]=G__p_ifunc->reftype[func_now];
     ifunc->isconst[iexist]|=G__p_ifunc->isconst[func_now];
     ifunc->isexplicit[iexist]|=G__p_ifunc->isexplicit[func_now];
     for(iin=0;iin<paranu;iin++) {
       ifunc->para_reftype[iexist][iin]
	 =G__p_ifunc->para_reftype[func_now][iin];
       ifunc->para_p_typetable[iexist][iin]
	 =G__p_ifunc->para_p_typetable[func_now][iin];
       if(G__p_ifunc->para_default[func_now][iin]) {
	 if(-1!=(long)G__p_ifunc->para_default[func_now][iin])
	   free((void*)G__p_ifunc->para_default[func_now][iin]);
	 free((void*)G__p_ifunc->para_def[func_now][iin]);
       }
       G__p_ifunc->para_default[func_now][iin]=(G__value*)NULL;
       G__p_ifunc->para_def[func_now][iin]=(char*)NULL;
       if(ifunc->para_name[iexist][iin]) {
	 if(G__p_ifunc->para_name[func_now][iin]) {
	   free((void*)G__p_ifunc->para_name[func_now][iin]);
	   G__p_ifunc->para_name[func_now][iin]=(char*)NULL;
	 }
       }
       else {
	 ifunc->para_name[iexist][iin]=G__p_ifunc->para_name[func_now][iin];
	 G__p_ifunc->para_name[func_now][iin]=(char*)NULL;
       }
     }
     ifunc->entry[iexist]=G__p_ifunc->entry[func_now];
     /* The copy in previous get the wrong tp2f ... let's restore it */
     ifunc->entry[iexist].tp2f = (void*)ifunc->funcname[iexist];
     ifunc->pentry[iexist]= &ifunc->entry[iexist];
     if(1==ifunc->ispurevirtual[iexist]) {
       ifunc->ispurevirtual[iexist]=G__p_ifunc->ispurevirtual[func_now];
       if(G__tagdefining>=0) --G__struct.isabstract[G__tagdefining];
     }
     else if(1==G__p_ifunc->ispurevirtual[func_now]) {
       ifunc->ispurevirtual[iexist]=G__p_ifunc->ispurevirtual[func_now];
     }
     if((ifunc!=G__p_ifunc || iexist!=func_now) && 
	G__p_ifunc->funcname[func_now]) {
       free((void*)G__p_ifunc->funcname[func_now]);
       G__p_ifunc->funcname[func_now] = (char*)NULL;
     }
   }
   else {
#ifndef G__OLDIMPLEMENTATION2027
     if(dtorflag) {
       G__func_now = store_func_now;
       G__p_ifunc = store_p_ifunc;
     }
     else {
       G__memfunc_next();
     }
#else /* 2027 */
     G__memfunc_next();
#endif /* 2027 */
   }
 }
#else /* 1706 */

#ifndef G__OLDIMPLEMENTATION2027
 if(dtorflag) {
    G__func_now = store_func_now;
    G__p_ifunc = store_p_ifunc;
  }
  else {
    G__memfunc_next();
  }
#else /* 2027 */
  G__memfunc_next();
#endif /* 2027 */

#endif /* 1706 */


#endif
  return(0);
}

/**************************************************************************
* G__separate_parameter()
*
**************************************************************************/
int G__separate_parameter(original,pos,param)
char *original;
int *pos;
char *param;
{
#ifndef G__SMALLOBJECT
  int i;
  int single_quote=0;
  int double_quote=0;
  int c;

  i = (*pos);

  while(1) {
    c = (*(original+(i++)));
    switch(c) {
    case '\'':
      if(double_quote==0) single_quote ^= 1;
      break;
    case '"':
      if(single_quote==0) double_quote ^= 1;
      break;
    case ' ':
    case '\0':
      if(0==single_quote && 0==double_quote) {
	*param = '\0';
	*pos = i;
	return(c);
      }
    default:
      *(param++) = c;
    }
  }

  /* return(c); */
#endif
}

/**************************************************************************
* G__parse_parameter_link()
*
* Used in G__cpplink.C
**************************************************************************/
int G__parse_parameter_link(paras)
char *paras;
{
#ifndef G__SMALLOBJECT
  int ifn,type,tagnum,typenum,reftype_const;
  G__value *para_default;
  char c_type[10],tagname[G__MAXNAME*6],typename[G__MAXNAME*6];
  char c_reftype_const[10],c_default[G__MAXNAME*2],c_paraname[G__MAXNAME*2];
  char c;
  int os=0;
  int store_var_type;
  int store_loadingDLL = G__loadingDLL;
  G__loadingDLL=1;

  store_var_type = G__var_type;

  ifn = 0;

  do {
    c = G__separate_parameter(paras,&os,c_type);
    if(c) {
      type = c_type[0];
      c = G__separate_parameter(paras,&os,tagname);
      if('-'==tagname[0]) tagnum = -1;
      else tagnum = G__search_tagname(tagname,0);
      c = G__separate_parameter(paras,&os,typename);
      if('-'==typename[0]) typenum = -1;
      else {
	if('\''==typename[0]) {
	  typename[strlen(typename)-1]='\0';
	  typenum = G__defined_typename(typename+1);
	}
	else typenum = G__defined_typename(typename);
      }
      c = G__separate_parameter(paras,&os,c_reftype_const);
      reftype_const = atoi(c_reftype_const);
#ifndef G__OLDIMPLEMENTATION1861
      if(-1!=typenum) reftype_const += G__newtype.isconst[typenum]*10;
#endif
      c = G__separate_parameter(paras,&os,c_default);
      if('-'==c_default[0] && '\0'==c_default[1]) {
	para_default=(G__value*)NULL;
	c_default[0]='\0';
      }
      else {
	para_default=(G__value*)(-1);
      }
      c = G__separate_parameter(paras,&os,c_paraname);
      if('-'==c_paraname[0]) c_paraname[0]='\0';
      G__memfunc_para_setup(ifn,type,tagnum,typenum,reftype_const
			    ,para_default,c_default,c_paraname);
      ++ifn;
    }
  } while('\0'!=c) ;
  G__var_type = store_var_type;
  G__loadingDLL=store_loadingDLL;
#endif
  return(0);
}

/**************************************************************************
* G__memfunc_para_setup()
*
* Used in G__cpplink.C
**************************************************************************/
int G__memfunc_para_setup(ifn,type,tagnum,typenum,reftype_const,para_default
			  ,para_def,para_name
			  )
int ifn,type,tagnum,typenum,reftype_const;
G__value *para_default;
char *para_def;
char *para_name;
{
#ifndef G__SMALLOBJECT
  G__p_ifunc->para_type[G__func_now][ifn] = type;
  G__p_ifunc->para_p_tagtable[G__func_now][ifn] = tagnum;
  G__p_ifunc->para_p_typetable[G__func_now][ifn] = typenum;
#if !defined(G__OLDIMPLEMENTATION1975)
  G__p_ifunc->para_isconst[G__func_now][ifn] = (reftype_const/10)%10;
  G__p_ifunc->para_reftype[G__func_now][ifn] = reftype_const-(reftype_const/10%10*10);
#else
  G__p_ifunc->para_reftype[G__func_now][ifn] = reftype_const%10;
  G__p_ifunc->para_isconst[G__func_now][ifn] = reftype_const/10;
#endif
  G__p_ifunc->para_default[G__func_now][ifn] = para_default;
  if(para_def[0]
     || (G__value*)NULL!=para_default
     ) {
    G__p_ifunc->para_def[G__func_now][ifn]=(char*)malloc(strlen(para_def)+1);
    strcpy(G__p_ifunc->para_def[G__func_now][ifn],para_def);
  }
  else {
    G__p_ifunc->para_def[G__func_now][ifn]=(char*)NULL;
  }
  if(para_name[0]) {
    G__p_ifunc->para_name[G__func_now][ifn]=(char*)malloc(strlen(para_name)+1);
    strcpy(G__p_ifunc->para_name[G__func_now][ifn],para_name);
  }
  else {
    G__p_ifunc->para_name[G__func_now][ifn]=(char*)NULL;
  }
#endif
  return(0);
}

/**************************************************************************
* G__memfunc_next()
*
* Used in G__cpplink.C
**************************************************************************/
int G__memfunc_next()
{
#ifndef G__SMALLOBJECT
  /* increment count */
  ++G__p_ifunc->allifunc;
  /***************************************************************
   * Allocate and initialize function table list
   ***************************************************************/
  if(G__p_ifunc->allifunc==G__MAXIFUNC) {
    G__p_ifunc->next=(struct G__ifunc_table *)malloc(sizeof(struct G__ifunc_table));
    G__p_ifunc->next->allifunc=0;
    G__p_ifunc->next->next=(struct G__ifunc_table *)NULL;
    G__p_ifunc->next->page = G__p_ifunc->page+1;
    G__p_ifunc->next->tagnum = G__p_ifunc->tagnum;
    
    /* set next G__p_ifunc */
    G__p_ifunc = G__p_ifunc->next;
    {
      int ix;
      for(ix=0;ix<G__MAXIFUNC;ix++) {
	G__p_ifunc->funcname[ix] = (char*)NULL;
	G__p_ifunc->userparam[ix] = 0;
      }
    }
    {
      int ix;
      for(ix=0;ix<G__MAXIFUNC;ix++) G__p_ifunc->userparam[ix] = 0;
    }
  }
#endif
  return(0);
}

/**************************************************************************
* G__tag_memfunc_reset()
*
* Used in G__cpplink.C
**************************************************************************/
int G__tag_memfunc_reset()
{
  G__tagnum = G__incset_tagnum;
  G__p_ifunc = G__incset_p_ifunc;
  G__func_now = G__incset_func_now;
  G__func_page = G__incset_func_page;
  G__var_type = G__incset_var_type;
  G__tagdefining = G__incset_tagdefining;
  G__def_tagnum = G__incset_def_tagnum;
  return(0);
}
#ifdef G__NEVER
/**************************************************************************
* G__p2ary_setup()
*
* Used in G__cpplink.C
**************************************************************************/
int G__p2ary_setup(n,...)
int n;
{
  return(0);
}
#endif


/**************************************************************************
* G__cppif_p2memfunc(fp)
*
* Used in G__cpplink.C
**************************************************************************/
int G__cppif_p2memfunc(fp)
FILE *fp;
{
  fprintf(fp,"\n/*********************************************************\n");
  fprintf(fp,"* Get size of pointer to member function\n");
  fprintf(fp,"*********************************************************/\n");
  fprintf(fp,"class G__Sizep2memfunc%s {\n",G__DLLID);
  fprintf(fp," public:\n");
  fprintf(fp,"  G__Sizep2memfunc%s() {p=&G__Sizep2memfunc%s::sizep2memfunc;}\n"
	  ,G__DLLID,G__DLLID);
  fprintf(fp,"    size_t sizep2memfunc() { return(sizeof(p)); }\n");
  fprintf(fp,"  private:\n");
  fprintf(fp,"    size_t (G__Sizep2memfunc%s::*p)();\n",G__DLLID);
  fprintf(fp,"};\n\n");

  fprintf(fp,"size_t G__get_sizep2memfunc%s()\n",G__DLLID);
  fprintf(fp,"{\n");
  fprintf(fp,"  G__Sizep2memfunc%s a;\n",G__DLLID);
  fprintf(fp,"  G__setsizep2memfunc((int)a.sizep2memfunc());\n");
  fprintf(fp,"  return((size_t)a.sizep2memfunc());\n");
  fprintf(fp,"}\n\n");
  return(0);
}

/**************************************************************************
* G__set_sizep2memfunc(fp)
*
* Used in G__cpplink.C
**************************************************************************/
int G__set_sizep2memfunc(fp)
FILE *fp;
{
  fprintf(fp,"\n   if(0==G__getsizep2memfunc()) G__get_sizep2memfunc%s();\n"
	  ,G__DLLID);
  return(0);
}

/**************************************************************************
* G__getcommentstring()
*
**************************************************************************/
int G__getcommentstring(buf,tagnum,pcomment)
char *buf;
int tagnum;
struct G__comment_info *pcomment;
{
  char temp[G__ONELINE];
  G__getcomment(temp,pcomment,tagnum);
  if('\0'==temp[0]) {
    sprintf(buf,"(char*)NULL");
  }
  else {
    G__add_quotation(temp,buf);
  }
  return(1);
}

/**************************************************************************
* G__pragmalinkenum()
**************************************************************************/
static void G__pragmalinkenum(tagnum,globalcomp)
int tagnum;
int globalcomp;
{
  /* double check tagnum points to a enum */
  if(-1==tagnum || 'e'!=G__struct.type[tagnum]) return; 

  /* enum in global scope */
  if(-1==G__struct.parent_tagnum[tagnum]
     || G__nestedclass
     ) {
    int ig15;
    struct G__var_array *var = &G__global;
    while(var) {
      for(ig15=0;ig15<var->allvar;ig15++) {
	/* modify globalcomp flag if enum member */
        if(tagnum==var->p_tagtable[ig15]) var->globalcomp[ig15]=globalcomp;
      }
      var=var->next;
    }
  }
  /* enum enclosed in class  */
  else {
    /* do nothing, should already be OK. */
  }
}

#if !defined(G__OLDIMPLEMENTATION1955) && defined(G__ROOT)
/**************************************************************************
* G__linknestedtypedef() 
**************************************************************************/
static void G__linknestedtypedef(tagnum,globalcomp)
int tagnum;
int globalcomp;
{
  int i;
  for(i=0;i<G__newtype.alltype;i++) {
    if (G__newtype.parent_tagnum[i] == -1) continue;
    if (G__newtype.parent_tagnum[i]==tagnum) {
      G__newtype.globalcomp[i] = globalcomp;
    }
  }
}
#endif

/**************************************************************************
* G__specify_link()
*
* #pragma link C++ class ClassName;      can use regexp
* #pragma link C   class ClassName;      can use regexp
* #pragma link off class ClassName;      can use regexp
* #ifdef G__ROOTSPECIAL
* #pragma link off class ClassName-;     set ROOT specific flag
* #endif
* #pragma link C++ enum ClassName;      can use regexp
* #pragma link C   enum ClassName;      can use regexp
* #pragma link off enum ClassName;      can use regexp
*
* #pragma link C++ nestedclass;
* #pragma link C   nestedclass;
* #pragma link off nestedclass;
*
* #pragma link C++ nestedtypedef;
* #pragma link C   nestedtypedef;
* #pragma link off nestedtypedef;
*
* #pragma link C++ function funcname;    can use regexp
* #pragma link C   function funcname;    can use regexp
* #pragma link off function funcname;    can use regexp
* #pragma link MACRO function funcname;  can use regexp
* #pragma stub C++ function funcname;    can use regexp
* #pragma stub C   function funcname;    can use regexp
*
* #pragma link C++ global variablename;  can use regexp
* #pragma link C   global variablename;  can use regexp
* #pragma link off global variablename;  can use regexp
*
* #pragma link C++ defined_in filename;
* #pragma link C   defined_in filename;
* #pragma link off defined_in filename;
* #pragma link C++ defined_in classname;
* #pragma link C   defined_in classname;
* #pragma link off defined_in classname;
* #pragma link C++ defined_in [class|struct|namespace] classname;
* #pragma link C   defined_in [class|struct|namespace] classname;
* #pragma link off defined_in [class|struct|namespace] classname;
*
* #pragma link C++ typedef TypeName;      can use regexp
* #pragma link C   typedef TypeName;      can use regexp
* #pragma link off typedef TypeName;      can use regexp
*
* #pragma link off all classes;
* #pragma link off all functions;
* #pragma link off all variables;
* #pragma link off all typedefs;
* #pragma link off all methods;
*
* #pragma link [C++|off] all_method     ClassName;
* #pragma link [C++|off] all_datamember ClassName;
*              ^
*
* #pragma link postprocess file func;
*
**************************************************************************/
void G__specify_link(link_stub)
int link_stub;
{
  int c;
  char buf[G__ONELINE];
  int globalcomp=G__NOLINK;
  /* int store_globalcomp; */
  int i;
  int hash;
  struct G__ifunc_table *ifunc;
  struct G__var_array *var;
#ifdef G__REGEXP
  regex_t re;
  int regstat;
#endif
#ifdef G__REGEXP1
  char *re;
#endif
  int os;
  char *p;
  int done=0;


  /* Get link language interface */
  c = G__fgetname_template(buf,";\n\r");

  if(strncmp(buf,"postproc",5)==0) {
    int store_globalcomp2 = G__globalcomp;
    int store_globalcomp3 = G__store_globalcomp;
    int store_prerun = G__prerun;
    G__globalcomp = G__NOLINK;
    G__store_globalcomp = G__NOLINK;
    G__prerun = 0;
    c = G__fgetname_template(buf,";");
    if(G__LOADFILE_SUCCESS<=G__loadfile(buf)) {
      char buf2[G__ONELINE];
      c = G__fgetstream(buf2,";");
      G__calc(buf2);
      G__unloadfile(buf);
    }
    G__globalcomp = store_globalcomp2;
    G__store_globalcomp = store_globalcomp3;
    G__prerun = store_prerun;
    if(';'!=c) G__fignorestream(";");
    return;
  }

  if(strncmp(buf,"default",3)==0) {
    c=G__read_setmode(&G__default_link);
    if('\n'!=c&&'\r'!=c) G__fignoreline();
    return;
  }

  if(G__SPECIFYLINK==link_stub) {
    if(strcmp(buf,"C++")==0) {
      globalcomp = G__CPPLINK;
      if(G__CLINK==G__globalcomp) {
	G__fprinterr(G__serr,"Warning: '#pragma link C++' ignored. Use '#pragma link C'");
	G__printlinenum();
      }
    }
    else if(strcmp(buf,"C")==0) {
      globalcomp = G__CLINK;
      if(G__CPPLINK==G__globalcomp) {
	G__fprinterr(G__serr,"Warning: '#pragma link C' ignored. Use '#pragma link C++'");
	G__printlinenum();
      }
    }
    else if(strcmp(buf,"MACRO")==0) globalcomp = G__MACROLINK;
    else if(strcmp(buf,"off")==0) globalcomp = G__NOLINK;
    else if(strcmp(buf,"OFF")==0) globalcomp = G__NOLINK;
    else {
      G__genericerror("Error: '#pragma link' syntax error");
      globalcomp = G__NOLINK; /* off */
    }
  }
  else {
    if(strcmp(buf,"C++")==0) {
      globalcomp = G__CPPSTUB;
      if(G__CLINK==G__globalcomp) {
	G__fprinterr(G__serr,"Warning: '#pragma stub C++' ignored. Use '#pragma stub C'");
	G__printlinenum();
      }
    }
    else if(strcmp(buf,"C")==0) {
      globalcomp = G__CSTUB;
      if(G__CPPLINK==G__globalcomp) {
	G__fprinterr(G__serr,"Warning: '#pragma stub C' ignored. Use '#pragma stub C++'");
	G__printlinenum();
      }
    }
    else if(strcmp(buf,"MACRO")==0) globalcomp = G__MACROLINK;
    else if(strcmp(buf,"off")==0) globalcomp = G__NOLINK;
    else if(strcmp(buf,"OFF")==0) globalcomp = G__NOLINK;
    else {
      G__genericerror("Error: '#pragma link' syntax error");
      globalcomp = G__NOLINK; /* off */
    }
  }

  if(';'==c)  return;

  /* Get type of language construct */
  c = G__fgetname_template(buf,";\n\r");

  if(G__MACROLINK==globalcomp&&strncmp(buf,"function",3)!=0) {
    G__fprinterr(G__serr,"Warning: #pragma link MACRO only valid for global function. Ignored\n");
    G__printlinenum();
    c=G__fignorestream(";\n");
    return;
  }

  /*************************************************************************
  * #pragma link [spec] nestedclass;
  *************************************************************************/
  if(strncmp(buf,"nestedclass",3)==0) {
    G__nestedclass = globalcomp;
  }

  if(strncmp(buf,"nestedtypedef",7)==0) {
    G__nestedtypedef=globalcomp;
  }

  if(';'==c)  return;

  switch(globalcomp) {
  case G__CPPSTUB:
  case G__CSTUB:
    if(strncmp(buf,"function",3)!=0) {
      G__fprinterr(G__serr,"Warning: #pragma stub only valid for global function. Ignored\n");
      c=G__fignorestream(";\n");
      return;
    }
    break;
  default:
    break;
  }

  /*************************************************************************
  * #pragma link [spec] class [name];
  *************************************************************************/
  if(strncmp(buf,"class",3)==0||strncmp(buf,"struct",3)==0||
     strncmp(buf,"union",3)==0||strncmp(buf,"enum",3)==0
#ifndef G__OLDIKMPLEMENTATION1242
     || strncmp(buf,"namespace",3)==0
#endif
     ) {
    int len;
    char* p2;
    int rf1 = 0;
    int rf2 = 0;
    int rf3 = 0;
    int iirf;
#ifndef G__OLDIKMPLEMENTATION1334
    char protectedaccess=0;
#ifndef G__OLDIKMPLEMENTATION1483
    if(strncmp(buf,"class+protected",10)==0) 
      protectedaccess=G__PROTECTEDACCESS;
    else if(strncmp(buf,"class+private",10)==0) {
      protectedaccess=G__PRIVATEACCESS;
      G__privateaccess = 1;
    }
#else
    if(strncmp(buf,"class+protected",6)==0) protectedaccess = 1;
#endif
#endif
    c = G__fgetstream_template(buf,";\n\r");
    for (iirf = 0; iirf < 3; iirf++) {
      if (buf[strlen(buf)-1] == '-') {
	rf1 = 1;
	buf[strlen(buf)-1] = '\0';
      }
      if (buf[strlen(buf)-1] == '!') {
	rf2 = 1;
	buf[strlen(buf)-1] = '\0';
      }
      if (buf[strlen(buf)-1] == '+') {
	rf3 = 1;
	buf[strlen(buf)-1] = '\0';
      }
    }
    len = strlen(buf);
    p2 = strchr(buf,'[');
    p = strrchr(buf,'*');
    if(len&&p&&(p2||'*'==buf[len-1]||('>'!=buf[len-1]&&'-'!=buf[len-1]))) {
      if(*(p+1)=='>') p=(char*)NULL;
      else p=p;
    }
    else p=(char*)NULL;
    if(p) {
#if defined(G__REGEXP)
#ifndef G__OLDIKMPLEMENTATION1583
      if('.'!=buf[len-2]) {
	buf[len-1] = '.';
	buf[len++] = '*';
	buf[len] = 0;
      }
#endif
      regstat=regcomp(&re,buf,REG_EXTENDED|REG_NOSUB);
      if(regstat!=0) {
	G__genericerror("Error: regular expression error");
	return;
      }
      for(i=0;i<G__struct.alltag;i++) {
	if('$'==G__struct.name[i][0]) os=1;
	else                          os=0;
	if(0==regexec(&re,G__struct.name[i]+os,(size_t)0,(regmatch_t*)NULL,0)){
	  G__struct.globalcomp[i] = globalcomp;
#ifndef G__OLDIKMPLEMENTATION1334
	  G__struct.protectedaccess[i] = protectedaccess;
#endif
	  ++done;
	  if('e'==G__struct.type[i]) G__pragmalinkenum(i,globalcomp);
#if  !defined(G__OLDIMPLEMENTATION1955) && defined(G__ROOT)
	  else if (G__NOLINK>G__nestedtypedef)
	    G__linknestedtypedef(i,globalcomp);
#endif
	}
      }
      regfree(&re);
#elif defined(G__REGEXP1)
      re = regcmp(buf, NULL);
      if (re==0) {
	G__genericerror("Error: regular expression error");
	return;
      }
      for(i=0;i<G__struct.alltag;i++) {
	if('$'==G__struct.name[i][0]) os=1;
	else                          os=0;
	if(0!=regex(re,G__struct.name[i]+os)){
	  G__struct.globalcomp[i] = globalcomp;
#ifndef G__OLDIKMPLEMENTATION1334
	  G__struct.protectedaccess[i] = protectedaccess;
#endif
	  ++done;
	  if('e'==G__struct.type[i]) G__pragmalinkenum(i,globalcomp);
#if  !defined(G__OLDIMPLEMENTATION1955) && defined(G__ROOT)
	  else if (G__NOLINK>G__nestedtypedef) 
	    G__linknestedtypedef(i,globalcomp);
#endif
	}
      }
      free(re);
#else /* G__REGEXP */
      *p='\0';
      hash=strlen(buf);
      for(i=0;i<G__struct.alltag;i++) {
	if('$'==G__struct.name[i][0]) os=1;
	else                          os=0;
	if(strncmp(buf,G__struct.name[i]+os,hash)==0
	   || ('*'==buf[0]&&strstr(G__struct.name[i],buf+1))
	   ) {
	  G__struct.globalcomp[i] = globalcomp;
#ifndef G__OLDIKMPLEMENTATION1334
	  G__struct.protectedaccess[i] = protectedaccess;
#endif
	  ++done;
	  /*G__fprinterr(G__serr,"#pragma link changed %s\n",G__struct.name[i]);*/
	  if('e'==G__struct.type[i]) G__pragmalinkenum(i,globalcomp);
#if  !defined(G__OLDIMPLEMENTATION1955) && defined(G__ROOT)
	  else if (G__NOLINK>G__nestedtypedef) 
	    G__linknestedtypedef(i,globalcomp);
#endif
	}
      }
#endif /* G__REGEXP */
    } /* if(p) */
    else {
      i = G__defined_tagname(buf,1);
      if(i>=0) {
	G__struct.globalcomp[i] = globalcomp;
#ifndef G__OLDIKMPLEMENTATION1334
	G__struct.protectedaccess[i] = protectedaccess;
#endif
	++done;
	if('e'==G__struct.type[i]) G__pragmalinkenum(i,globalcomp);
#if  !defined(G__OLDIMPLEMENTATION1955) && defined(G__ROOT)
	else if (G__NOLINK>G__nestedtypedef) 
	  G__linknestedtypedef(i,globalcomp);
#endif
	G__struct.rootflag[i] = 0;
	if (rf1 == 1) G__struct.rootflag[i] = G__NOSTREAMER;
	if (rf2 == 1) G__struct.rootflag[i] |= G__NOINPUTOPERATOR;
	if (rf3 == 1) {
	  G__struct.rootflag[i] |= G__USEBYTECOUNT;
	  if(rf1) {
	    G__struct.rootflag[i] &= ~G__NOSTREAMER;
	    G__fprinterr(G__serr, "option + mutual exclusive with -, + prevails\n");
	  }
	}
      }
    }
    if(!done && G__NOLINK!=globalcomp) {
#ifdef G__ROOT
      if(G__dispmsg>=G__DISPERR) {
	G__fprinterr(G__serr,"Error: link requested for unknown class %s",buf);
	G__genericerror((char*)NULL);
#else
      if(G__dispmsg>=G__DISPNOTE) {
	G__fprinterr(G__serr,"Note: link requested for unknown class %s",buf);
	G__printlinenum();
#endif
      }
    }
  }

  /*************************************************************************
  * #pragma link [spec] function [name];
  *************************************************************************/
  else if(strncmp(buf,"function",3)==0) {
    struct G__ifunc_table *x_ifunc = &G__ifunc;
#ifndef G__OLDIMPLEMENTATION828
    fpos_t pos;
    int store_line = G__ifile.line_number;
    fgetpos(G__ifile.fp,&pos);
    c = G__fgetstream_template(buf,";\n\r<>");

    if(G__CPPLINK==globalcomp) globalcomp=G__METHODLINK;

    if(('<'==c || '>'==c) 
       &&(strcmp(buf,"operator")==0||strstr(buf,"::operator"))) {
      int len=strlen(buf);
      buf[len++]=c;
      store_line = G__ifile.line_number;
      fgetpos(G__ifile.fp,&pos);
      buf[len] = G__fgetc();
      if(buf[len]==c||'='==buf[len]) c=G__fgetstream_template(buf+10,";\n\r");
      else {
	fsetpos(G__ifile.fp,&pos);
	G__ifile.line_number = store_line;
	if(G__dispsource) G__disp_mask = 1;
	c = G__fgetstream_template(buf+len,";\n\r");
      }
    }
    else {
      fsetpos(G__ifile.fp,&pos);
      G__ifile.line_number = store_line;
      c = G__fgetstream_template(buf,";\n\r");
    }


#else /* 828 */
    c = G__fgetstream_template(buf,";\n\r");
#endif /* 828 */


    /* if the function is specified with paramters */
    p = strchr(buf,'(');
    if(p && strstr(buf,"operator()")==0) {
      if(strncmp(p,")(",2)==0) p+=2;
      else if(strcmp(p,")")==0) p=0;
    }
    if(p) {
      char funcname[G__LONGLINE];
      char param[G__LONGLINE];
      if(')' == *(p+1) && '('== *(p+2) ) p = strchr(p+1,'(');
      *p='\0';
      strcpy(funcname,buf);
      strcpy(param,p+1);
      p=strrchr(param,')');
      *p='\0';
      G__SetGlobalcomp(funcname,param,globalcomp);
      ++done;
      return;
    }

    p = G__strrstr(buf,"::");
    if(p) {
      int ixx=0;
      if(-1==x_ifunc->tagnum) {
	int tagnum;
	*p = 0;
	tagnum = G__defined_tagname(buf,0);
	if(-1!=tagnum) {
	  x_ifunc = G__struct.memfunc[tagnum];
	}
	*p = ':';
      }
      p+=2;
      while(*p) buf[ixx++] = *p++;
      buf[ixx] = 0;
    }

    /* search for wildcard character */
    p = strrchr(buf,'*');

    /* in case of operator*  */
    if(strncmp(buf,"operator",8)==0) p = (char*)NULL;

    if(p) {
#if defined(G__REGEXP)
#ifndef G__OLDIKMPLEMENTATION1583
      int len = strlen(buf);
      if('.'!=buf[len-2]) {
	buf[len-1] = '.';
	buf[len++] = '*';
	buf[len] = 0;
      }
#endif
      regstat=regcomp(&re,buf,REG_EXTENDED|REG_NOSUB);
      if(regstat!=0) {
	G__genericerror("Error: regular expression error");
	return;
      }
      ifunc = x_ifunc;
      while(ifunc) {
	for(i=0;i<ifunc->allifunc;i++) {
	  if(0==regexec(&re,ifunc->funcname[i],(size_t)0,(regmatch_t*)NULL,0)
	     && (ifunc->para_nu[i]<2 ||
		 -1==ifunc->para_p_tagtable[i][1] ||
		 strncmp(G__struct.name[ifunc->para_p_tagtable[i][1]]
			 ,"G__CINT_",8)!=0)
	     ){
	    ifunc->globalcomp[i] = globalcomp;
	    ++done;
	  }
	}
	ifunc = ifunc->next;
      }
      regfree(&re);
#elif defined(G__REGEXP1)
      re = regcmp(buf, NULL);
      if (re==0) {
	G__genericerror("Error: regular expression error");
	return;
      }
      ifunc = x_ifunc;
      while(ifunc) {
	for(i=0;i<ifunc->allifunc;i++) {
 	  if(0!=regex(re,ifunc->funcname[i])
	     && (ifunc->para_nu[i]<2 ||
		 -1==ifunc->para_p_tagtable[i][1] ||
		 strncmp(G__struct.name[ifunc->para_p_tagtable[i][1]]
			 ,"G__CINT_",8)!=0)
	     ){
	    ifunc->globalcomp[i] = globalcomp;
	    ++done;
	  }
	}
	ifunc = ifunc->next;
      }
      free(re);
#else /* G__REGEXP */
      *p='\0';
      hash=strlen(buf);
      ifunc = x_ifunc;
      while(ifunc) {
	for(i=0;i<ifunc->allifunc;i++) {
 	  if((strncmp(buf,ifunc->funcname[i],hash)==0
	      || ('*'==buf[0]&&strstr(ifunc->funcname[i],buf+1)))
	     && (ifunc->para_nu[i]<2 ||
		 -1==ifunc->para_p_tagtable[i][1] ||
		 strncmp(G__struct.name[ifunc->para_p_tagtable[i][1]]
			 ,"G__CINT_",8)!=0)
	     ) {
	    ifunc->globalcomp[i] = globalcomp;
	    ++done;
	    /*G__fprinterr(G__serr,"#pragma link changed %s\n",ifunc->funcname[i]);*/
	  }
	}
	ifunc = ifunc->next;
      }
#endif /* G__REGEXP */
    }
    else {
      ifunc = x_ifunc;
      while(ifunc) {
	for(i=0;i<ifunc->allifunc;i++) {
	  if(strcmp(buf,ifunc->funcname[i])==0
	     && (ifunc->para_nu[i]<2 ||
		 -1==ifunc->para_p_tagtable[i][1] ||
		 strncmp(G__struct.name[ifunc->para_p_tagtable[i][1]]
			 ,"G__CINT_",8)!=0)
	     ) {
	    ifunc->globalcomp[i] = globalcomp;
	    ++done;
	  }
	}
	ifunc = ifunc->next;
      }
    }
    if(!done && (p=strchr(buf,'<'))) {
      struct G__param fpara;
      struct G__funclist *funclist=(struct G__funclist*)NULL;
      int tmp=0;

      fpara.paran=0;

      G__hash(buf,hash,tmp);
      funclist=G__add_templatefunc(buf,&fpara,hash,funclist,x_ifunc,0);
      if(funclist) {
	funclist->ifunc->globalcomp[funclist->ifn] = globalcomp;
	G__funclist_delete(funclist);
	++done;
      }
    }
    if(!done && G__NOLINK!=globalcomp) {
#ifdef G__ROOT
      if(G__dispmsg>=G__DISPERR) {
	G__fprinterr(G__serr,"Error: link requested for unknown function %s",buf);
	G__genericerror((char*)NULL);
#else
      if(G__dispmsg>=G__DISPNOTE) {
	G__fprinterr(G__serr,"Note: link requested for unknown function %s",buf);
	G__printlinenum();
#endif
      }
    }
  }

  /*************************************************************************
  * #pragma link [spec] global [name];
  *************************************************************************/
  else if(strncmp(buf,"global",3)==0) {
    c = G__fgetname_template(buf,";\n\r");
    p = strrchr(buf,'*');
    if(p) {
#if defined(G__REGEXP)
#ifndef G__OLDIKMPLEMENTATION1583
      int len = strlen(buf);
      if('.'!=buf[len-2]) {
	buf[len-1] = '.';
	buf[len++] = '*';
	buf[len] = 0;
      }
#endif
      regstat=regcomp(&re,buf,REG_EXTENDED|REG_NOSUB);
      if(regstat!=0) {
	G__genericerror("Error: regular expression error");
	return;
      }
      var = &G__global;
      while(var) {
	for(i=0;i<var->allvar;i++) {
	  if(0==regexec(&re,var->varnamebuf[i],(size_t)0,(regmatch_t*)NULL,0)){
	    var->globalcomp[i] = globalcomp;
	    ++done;
	  }
	}
	var=var->next;
      }
      regfree(&re);
#elif defined(G__REGEXP1)
      re = regcmp(buf, NULL);
      if (re==0) {
	G__genericerror("Error: regular expression error");
	return;
      }
      var = &G__global;
      while(var) {
	for(i=0;i<var->allvar;i++) {
 	  if(0!=regex(re,var->varnamebuf[i])){
	    var->globalcomp[i] = globalcomp;
	    ++done;
	  }
	}
	var=var->next;
      }
      free(re);
#else /* G__REGEXP */
      *p = '\0';
      hash = strlen(buf);
      var = &G__global;
      while(var) {
	for(i=0;i<var->allvar;i++) {
	  if(strncmp(buf,var->varnamebuf[i],hash)==0
	     || ('*'==buf[0]&&strstr(var->varnamebuf[i],buf+1))
	     ) {
	    var->globalcomp[i] = globalcomp;
	    ++done;
	    /*G__fprinterr(G__serr,"#pragma link changed %s\n",var->varnamebuf[i]);*/
	  }
	}
	var=var->next;
      }
#endif /* G__REGEXP */
    }
    else {
      G__hash(buf,hash,i);
      var = G__getvarentry(buf,hash,&i,&G__global,(struct G__var_array*)NULL);
      if(var) {
	var->globalcomp[i] = globalcomp;
	++done;
      }
    }
    if(!done && G__NOLINK!=globalcomp) {
      if(G__dispmsg>=G__DISPNOTE) {
	G__fprinterr(G__serr,"Note: link requested for unknown global variable %s",buf);
	G__printlinenum();
      }
    }
  }

  /*************************************************************************
  * #pragma link [spec] all_datamember [classname];
  *  This is not needed because G__METHODLINK and G__ONLYMETHODLINK are
  *  introduced. Keeping this just for future needs.
  *************************************************************************/
  else if(strncmp(buf,"all_datamembers",5)==0) {
    if(';'!=c) c = G__fgetstream_template(buf,";\n\r");
    if(buf[0]) {
      struct G__var_array *var;
      int ig15;
      if(strcmp(buf,"::")==0) {
	var = &G__global;
      }
      else {
	int tagnum = G__defined_tagname(buf,0);
	if(-1!=tagnum) {
	  var= G__struct.memvar[tagnum];
	}
	else { /* must be an error */
	  return;
	}
      }
      while(var) {
	for(ig15=0;ig15<var->allvar;ig15++) {
	  var->globalcomp[ig15] = globalcomp;
	  if(G__NOLINK==globalcomp) var->access[ig15] = G__PRIVATE;
	  else                      var->access[ig15] = G__PUBLIC;
	}
	var=var->next;
      }
    }
  }

  /*************************************************************************
  * #pragma link [spec] all_function|all_method [classname];
  *  This is not needed because G__METHODLINK and G__ONLYMETHODLINK are
  *  introduced. Keeping this just for future needs.
  *************************************************************************/
  else if(strncmp(buf,"all_methods",5)==0||
	  strncmp(buf,"all_functions",5)==0) {
    if(';'!=c) c = G__fgetstream_template(buf,";\n\r");
    if(G__CPPLINK==globalcomp) globalcomp=G__METHODLINK;
    if(buf[0]) {
      struct G__ifunc_table *ifunc;
      int ifn;
      if(strcmp(buf,"::")==0) {
	ifunc = &G__ifunc;
      }
      else {
	int tagnum = G__defined_tagname(buf,0);
	if(-1!=tagnum) {
	  ifunc = G__struct.memfunc[tagnum];
	}
	else { /* must be an error */
	  return;
	}
      }
      while(ifunc) {
	for(ifn=0;ifn<ifunc->allifunc;ifn++) {
	  ifunc->globalcomp[ifn] = globalcomp;
	  if(G__NOLINK==globalcomp) ifunc->access[ifn] = G__PRIVATE;
	  else                      ifunc->access[ifn] = G__PUBLIC;
	}
	ifunc=ifunc->next;
      }
    }
    else {
      G__suppress_methods = (globalcomp==G__NOLINK);
    }
  }

  /*************************************************************************
  * #pragma link [spec] methods;
  *************************************************************************/
  else if(strncmp(buf,"methods",3)==0) {
    G__suppress_methods = (globalcomp==G__NOLINK);
  }

  /*************************************************************************
  * #pragma link [spec] typedef [name];
  *************************************************************************/
  else if(strncmp(buf,"typedef",3)==0) {
    c = G__fgetname_template(buf,";\n\r");
    p = strrchr(buf,'*');
    if(p && *(p+1)=='>') p=(char*)NULL;
    if(p) {
#if defined(G__REGEXP)
#ifndef G__OLDIKMPLEMENTATION1583
      int len = strlen(buf);
      if('.'!=buf[len-2]) {
	buf[len-1] = '.';
	buf[len++] = '*';
	buf[len] = 0;
      }
#endif
      regstat=regcomp(&re,buf,REG_EXTENDED|REG_NOSUB);
      if(regstat!=0) {
	G__genericerror("Error: regular expression error");
	return;
      }
      for(i=0;i<G__newtype.alltype;i++) {
	if(0==regexec(&re,G__newtype.name[i],(size_t)0,(regmatch_t*)NULL,0)){
	  G__newtype.globalcomp[i] = globalcomp;
	  if(-1!=G__newtype.tagnum[i] && 
	     '$'==G__struct.name[G__newtype.tagnum[i]][0]) {
	    G__struct.globalcomp[G__newtype.tagnum[i]] = globalcomp;
	  }
	  ++done;
	}
      }
      regfree(&re);
#elif defined(G__REGEXP1)
      re = regcmp(buf, NULL);
      if (re==0) {
	G__genericerror("Error: regular expression error");
	return;
      }
      for(i=0;i<G__newtype.alltype;i++) {
	if(0!=regex(re,G__newtype.name[i])){
	  G__newtype.globalcomp[i] = globalcomp;
	  if(-1!=G__newtype.tagnum[i] && 
	     '$'==G__struct.name[G__newtype.tagnum[i]][0]) {
	    G__struct.globalcomp[G__newtype.tagnum[i]] = globalcomp;
	  }
	  ++done;
	}
      }
      free(re);
#else /* G__REGEXP */
      *p='\0';
      hash=strlen(buf);
      for(i=0;i<G__newtype.alltype;i++) {
	if(strncmp(buf,G__newtype.name[i],hash)==0
	   || ('*'==buf[0]&&strstr(G__newtype.name[i],buf+1))
	   ) {
	  G__newtype.globalcomp[i] = globalcomp;
	  if(-1!=G__newtype.tagnum[i] && 
	     '$'==G__struct.name[G__newtype.tagnum[i]][0]) {
	    G__struct.globalcomp[G__newtype.tagnum[i]] = globalcomp;
	  }
	  ++done;
	}
      }
#endif /* G__REGEXP */
    }
    else {
      i = G__defined_typename(buf);
      if(-1!=i) {
	G__newtype.globalcomp[i] = globalcomp;
	if(-1!=G__newtype.tagnum[i] && 
	   '$'==G__struct.name[G__newtype.tagnum[i]][0]) {
	  G__struct.globalcomp[G__newtype.tagnum[i]] = globalcomp;
	}
	  ++done;
      }
    }
    if(!done && G__NOLINK!=globalcomp) {
#ifdef G__ROOT
      if(G__dispmsg>=G__DISPERR) {
	G__fprinterr(G__serr,"Error: link requested for unknown typedef %s",buf);
	G__genericerror((char*)NULL);
#else
      if(G__dispmsg>=G__DISPNOTE) {
	G__fprinterr(G__serr,"Note: link requested for unknown typedef %s",buf);
	G__printlinenum();
#endif
      }
    }
  }

  /*************************************************************************
  * #pragma link [spec] defined_in [item];
  *************************************************************************/
  else if(strncmp(buf,"defined_in",3)==0) {
    fpos_t pos;
    int tagflag = 0;
    int ifile=0;
    struct stat statBufItem;  
    struct stat statBuf;  
#ifdef G__WIN32
    char fullItem[_MAX_PATH], fullIndex[_MAX_PATH];
#endif
    fgetpos(G__ifile.fp,&pos);
    c = G__fgetname(buf,";\n\r");
    if(strcmp(buf,"class")==0||strcmp(buf,"struct")==0||
	strcmp(buf,"namespace")==0) {
      if(isspace(c)) c = G__fgetstream_template(buf,";\n\r");
      tagflag = 1;
    }
    else {
      fsetpos(G__ifile.fp,&pos);
      c = G__fgetstream_template(buf,";\n\r<>");
    }
    if ( 
	0==tagflag &&
	0 == stat( buf, & statBufItem ) ) {
#ifdef G__WIN32
      _fullpath( fullItem, buf, _MAX_PATH );      
#endif
      for(ifile=0;ifile<G__nfile;ifile++) {
	if (0 == stat( G__srcfile[ifile].filename, & statBuf ) ) {
#ifndef G__WIN32
	  if ( statBufItem.st_ino == statBuf.st_ino ) {
#else
	  _fullpath( fullIndex, G__srcfile[ifile].filename, _MAX_PATH );
          /* Windows is case insensitive! */ 
	  if (0==stricmp(fullItem,fullIndex)) {
#endif
	    ++done;
	    /* link class,struct */
	    for(i=0;i<G__struct.alltag;i++) {
	      if(G__struct.filenum[i]==ifile) {
		struct G__var_array *var = G__struct.memvar[i];
		int ifn;
		while(var) {
		  for(ifn=0;ifn<var->allvar;ifn++) {
		    if(var->filenum[ifn]==ifile
#define G__OLDIMPLEMENTATION1740
		       ) {
		      var->globalcomp[ifn] = globalcomp;
		    }
		  }
		  var = var->next;
		}
		ifunc=G__struct.memfunc[i];
		while(ifunc) {
		  for(ifn=0;ifn<ifunc->allifunc;ifn++) {
		    if(ifunc->pentry[ifn]&&ifunc->pentry[ifn]->filenum==ifile
		       ) {
		      ifunc->globalcomp[ifn] = globalcomp;
		    }
		  }
		  ifunc = ifunc->next;
		}
		G__struct.globalcomp[i]=globalcomp;
                /* Note this make the equivalent of '+' the
		   default for defined_in type of linking */
                if ( 0 == (G__struct.rootflag[i] & G__NOSTREAMER) ) {
                  G__struct.rootflag[i] |= G__USEBYTECOUNT;
                }
	      }
	    }
	    /* link global function */
	    ifunc = &G__ifunc;
	    while(ifunc) {
	      for(i=0;i<ifunc->allifunc;i++) {
		if(ifunc->pentry[i]&&ifunc->pentry[i]->filenum==ifile) {
		  ifunc->globalcomp[i] = globalcomp;
		}
	      }
	      ifunc = ifunc->next;
	    }
#ifdef G__VARIABLEFPOS
	    /* link global variable */
	    {
	      struct G__var_array *var = &G__global;
	      while(var) {
		for(i=0;i<var->allvar;i++) {
		  if(var->filenum[i]==ifile) {
		    var->globalcomp[i] = globalcomp;
		  }
		}
		var = var->next;
	      }
	    }
#endif
#ifdef G__TYPEDEFFPOS
	    /* link typedef */
	    for(i=0;i<G__newtype.alltype;i++) {
	      if(G__newtype.filenum[i]==ifile) {
		G__newtype.globalcomp[i] = globalcomp;
	      }
	    }
#endif
	    break;
	  }
	}
      }
    }
    /* #pragma link [C|C++|off] defined_in [class|struct|namespace] name; */
    if(!done) {
      int parent_tagnum = G__defined_tagname(buf,0);
      int j,flag;
      if(-1!=parent_tagnum) {
	for(i=0;i<G__struct.alltag;i++) {
	  struct G__var_array *var = G__struct.memvar[parent_tagnum];
	  int ifn;
	  done = 1;
	  while(var) {
	    for(ifn=0;ifn<var->allvar;ifn++) {
	      if(var->filenum[ifn]==ifile
		 ) {
		var->globalcomp[ifn] = globalcomp;
	      }
	    }
	    var = var->next;
	  }
	  ifunc=G__struct.memfunc[i];
	  while(ifunc) {
	    for(ifn=0;ifn<ifunc->allifunc;ifn++) {
	      if(ifunc->pentry[ifn]&&ifunc->pentry[ifn]->filenum==ifile
		 ) {
		ifunc->globalcomp[ifn] = globalcomp;
	      }
	    }
	    ifunc = ifunc->next;
	  }
	  flag = 0;
	  j = i;
	  G__struct.globalcomp[parent_tagnum]=globalcomp;
	  while(-1!=G__struct.parent_tagnum[j]) {
	    if(G__struct.parent_tagnum[j]==parent_tagnum) flag=1;
	    j = G__struct.parent_tagnum[j];
	  }
	  if(flag) {
	    var = G__struct.memvar[i];
	    while(var) {
	      for(ifn=0;ifn<var->allvar;ifn++) {
		if(var->filenum[ifn]==ifile
		   &&G__PUBLIC==var->access[ifn]
		   ) {
		  var->globalcomp[ifn] = globalcomp;
		}
	      }
	      var = var->next;
	    }
	    ifunc=G__struct.memfunc[i];
	    while(ifunc) {
	      for(ifn=0;ifn<ifunc->allifunc;ifn++) {
		if(ifunc->pentry[ifn]&&ifunc->pentry[ifn]->filenum==ifile
		   &&G__PUBLIC==ifunc->access[ifn]
		   ) {
		  ifunc->globalcomp[ifn] = globalcomp;
		}
	      }
	      ifunc = ifunc->next;
	    }
	    G__struct.globalcomp[i]=globalcomp;
	    /* Note this make the equivalent of '+' the
	       default for defined_in type of linking */
	    if ( (G__struct.rootflag[i] & G__NOSTREAMER) == 0 ) {
	      G__struct.rootflag[i] |= G__USEBYTECOUNT;
	    }
          }
	}
	for(i=0;i<G__newtype.alltype;i++) {
	  flag = 0;
	  j = G__newtype.parent_tagnum[i];
	  do {
	    if(j == parent_tagnum) flag = 1;
	    j = G__struct.parent_tagnum[j];
	  } while(-1 != j);
	  if(flag) {
	    G__struct.globalcomp[i]=globalcomp;
	    /* Note this make the equivalent of '+' the
	       default for defined_in type of linking */
	    if ( 0 == (G__struct.rootflag[i] & G__NOSTREAMER) ) {
	      G__struct.rootflag[i] |= G__USEBYTECOUNT;
	    }
          }
	}
      }
    }
    if(!done && G__NOLINK!=globalcomp) {
      G__fprinterr(G__serr,"Warning: link requested for unknown srcfile %s",buf);
      G__printlinenum();
    }
  }

  /*************************************************************************
  * #pragma link [spec] all [item];
  *************************************************************************/
  else if(strncmp(buf,"all",2)==0) {
    c = G__fgetname_template(buf,";\n\r");
    if(strncmp(buf,"class",3)==0) {
      for(i=0;i<G__struct.alltag;i++) {
	if(G__NOLINK==globalcomp ||
	   (G__NOLINK==G__struct.iscpplink[i] && 
	    (-1!=G__struct.filenum[i] && 
	     0==(G__srcfile[G__struct.filenum[i]].hdrprop&G__CINTHDR))))
	  G__struct.globalcomp[i] = globalcomp;
      }
    }
    else if(strncmp(buf,"function",3)==0) {
      ifunc = &G__ifunc;
      while(ifunc) {
	for(i=0;i<ifunc->allifunc;i++) {
	if(G__NOLINK==globalcomp ||
	   (
	    0<=ifunc->pentry[i]->size && 0<=ifunc->pentry[i]->filenum &&
	    0==(G__srcfile[ifunc->pentry[i]->filenum].hdrprop&G__CINTHDR)))
	  ifunc->globalcomp[i] = globalcomp;
	}
	ifunc = ifunc->next;
      }
    }
    else if(strncmp(buf,"global",3)==0) {
      var = &G__global;
      while(var) {
	for(i=0;i<var->allvar;i++) {
	  if(G__NOLINK==globalcomp ||
	     (0<=var->filenum[i] &&
	      0==(G__srcfile[var->filenum[i]].hdrprop&G__CINTHDR)))
	    var->globalcomp[i] = globalcomp;
	}
	var = var->next;
      }
    }
    else if(strncmp(buf,"typedef",3)==0) {
      for(i=0;i<G__newtype.alltype;i++) {
	if(G__NOLINK==globalcomp ||
	   (G__NOLINK==G__newtype.iscpplink[i] &&
	    0<=G__newtype.filenum[i] &&
	    0==(G__srcfile[G__newtype.filenum[i]].hdrprop&G__CINTHDR))) {
	  G__newtype.globalcomp[i] = globalcomp;
	  if((-1!=G__newtype.tagnum[i] && 
	      '$'==G__struct.name[G__newtype.tagnum[i]][0])) {
	    G__struct.globalcomp[G__newtype.tagnum[i]] = globalcomp;
	  }
	}
      }
    }
    /*************************************************************************
     * #pragma link [spec] all methods;
     *************************************************************************/
    else if(strncmp(buf,"methods",3)==0) {
      G__suppress_methods = (globalcomp==G__NOLINK);
  }
  }

  if(';'!=c) c=G__fignorestream("#;");
  if(';'!=c) G__genericerror("Syntax error: #pragma link");
}

#endif /* G__SMALLOBJECT */

/**************************************************************************
* G__incsetup_memvar()
*
**************************************************************************/
void G__incsetup_memvar(tagnum)
int tagnum;
{
  int store_asm_exec;
  char store_var_type;
  int store_static_alloc = G__static_alloc;
  int store_constvar = G__constvar;
  if(G__struct.incsetup_memvar[tagnum]) {
    store_asm_exec = G__asm_exec;
    G__asm_exec=0;
    store_var_type = G__var_type;
#ifdef G__OLDIMPLEMENTATION1125_YET
    if(0==G__struct.memvar[tagnum]->allvar
       || 'n'==G__struct.type[tagnum])
      (*G__struct.incsetup_memvar[tagnum])();
#else
    (*G__struct.incsetup_memvar[tagnum])();
#endif
    G__struct.incsetup_memvar[tagnum] = (G__incsetup)NULL;
#ifdef G__DEBUG
    if(G__var_type!=store_var_type)
      G__fprinterr(G__serr,"Cint internal error: G__incsetup_memvar %c %c\n"
	      ,G__var_type,store_var_type);
#endif
    G__var_type = store_var_type;
    G__asm_exec = store_asm_exec;
    G__constvar = store_constvar;
    G__static_alloc = store_static_alloc;
  }
}

/**************************************************************************
* G__incsetup_memfunc()
*
**************************************************************************/
void G__incsetup_memfunc(tagnum)
int tagnum;
{
  char store_var_type;
  int store_asm_exec;
  if(G__struct.incsetup_memfunc[tagnum]) {
    store_asm_exec = G__asm_exec;
    G__asm_exec=0;
    store_var_type = G__var_type;
#ifdef G__OLDIMPLEMENTATION1125_YET /* G__PHILIPPE26 */
    if(0==G__struct.memfunc[tagnum]->allifunc 
       || 'n'==G__struct.type[tagnum]
       || (
	   -1!=G__struct.memfunc[tagnum]->pentry[0]->size
	   && 2>=G__struct.memfunc[tagnum]->allifunc))
      (*G__struct.incsetup_memfunc[tagnum])();
#else
    (*G__struct.incsetup_memfunc[tagnum])();
#endif
    G__struct.incsetup_memfunc[tagnum] = (G__incsetup)NULL;
    G__var_type = store_var_type;
    G__asm_exec = store_asm_exec;
  }
}

/**************************************************************************
* G__getnumbaseclass()
*
**************************************************************************/
int G__getnumbaseclass(tagnum)
int tagnum;
{
  return(G__struct.baseclass[tagnum]->basen);
}

/**************************************************************************
* G__setnewtype_settypeum()
*
**************************************************************************/
static int G__setnewtype_typenum = -1;
void G__setnewtype_settypeum(typenum)
int typenum;
{
  G__setnewtype_typenum=typenum;
}

/**************************************************************************
* G__setnewtype()
*
**************************************************************************/
void G__setnewtype(globalcomp,comment,nindex)
int globalcomp;
char* comment;
int nindex;
{
  int typenum = 
    (-1!=G__setnewtype_typenum)? G__setnewtype_typenum:G__newtype.alltype-1;
  G__newtype.iscpplink[typenum] = globalcomp;
  G__newtype.comment[typenum].p.com = comment;
  if(comment) G__newtype.comment[typenum].filenum = -2;
  else        G__newtype.comment[typenum].filenum = -1;
  G__newtype.nindex[typenum] = nindex;
  if(nindex)
    G__newtype.index[typenum]=(int*)malloc(G__INTALLOC*nindex);
}

/**************************************************************************
* G__setnewtypeindex()
*
**************************************************************************/
void G__setnewtypeindex(j,index)
int j;
int index;
{
  int typenum = 
    (-1!=G__setnewtype_typenum)? G__setnewtype_typenum:G__newtype.alltype-1;
  G__newtype.index[typenum][j] = index;
}

/**************************************************************************
* G__getgvp()
*
**************************************************************************/
long G__getgvp()
{
  return(G__globalvarpointer);
}

/**************************************************************************
* G__setgvp()
*
**************************************************************************/
void G__setgvp(gvp)
long gvp;
{
  G__globalvarpointer=gvp;
}

/**************************************************************************
* G__resetplocal()
*
**************************************************************************/
void G__resetplocal()
{
  if(G__def_struct_member && 'n'==G__struct.type[G__tagdefining]) {
    G__incset_tagnum = G__tagnum;
    G__incset_p_local = G__p_local;
    G__incset_def_struct_member = G__def_struct_member;
    G__incset_tagdefining = G__tagdefining;
    G__incset_globalvarpointer = G__globalvarpointer ;
    G__incset_var_type = G__var_type ;
    G__incset_typenum = G__typenum ;
    G__incset_static_alloc = G__static_alloc ;
    G__incset_access = G__access ;

    G__tagnum = G__tagdefining;
    G__p_local=G__struct.memvar[G__tagnum];
    while(G__p_local->next) G__p_local = G__p_local->next;
    G__def_struct_member = 1;
    G__tagdefining=G__tagnum;
    /* G__static_alloc = 1; */
  }
  else {
    G__p_local = (struct G__var_array*)NULL;
    G__incset_def_struct_member =0; 
  }
}

/**************************************************************************
* G__resetglobalenv()
*
**************************************************************************/
void G__resetglobalenv()
{
  if(G__incset_def_struct_member && 'n'==G__struct.type[G__incset_tagdefining]){
    G__p_local = G__incset_p_local ;
    G__def_struct_member = G__incset_def_struct_member ;
    G__tagdefining = G__incset_tagdefining ;
    
    G__globalvarpointer = G__incset_globalvarpointer ;
    G__var_type = G__incset_var_type ;
    G__tagnum = G__incset_tagnum ;
    G__typenum = G__incset_typenum ;
    G__static_alloc = G__incset_static_alloc ;
    G__access = G__incset_access ;
  }
  else {
    G__globalvarpointer = G__PVOID;
    G__var_type = 'p';
    G__tagnum = -1;
    G__typenum = -1;
    G__static_alloc = 0;
    G__access = G__PUBLIC;
  }
}

/**************************************************************************
* G__lastifuncposition()
*
**************************************************************************/
void G__lastifuncposition()
{
  if(G__def_struct_member && 'n'==G__struct.type[G__tagdefining]) {
    G__incset_tagnum = G__tagnum;
    G__incset_p_ifunc = G__p_ifunc;
    G__incset_func_now = G__func_now;
    G__incset_func_page = G__func_page;
    G__incset_var_type = G__var_type;
    G__tagnum = G__tagdefining;
    G__p_ifunc = G__struct.memfunc[G__tagnum];
    while(G__p_ifunc->next) G__p_ifunc=G__p_ifunc->next;
  }
  else {
    G__p_ifunc = &G__ifunc;
    while(G__p_ifunc->next) G__p_ifunc=G__p_ifunc->next;
    G__incset_def_struct_member = 0;
  }
}

/**************************************************************************
* G__resetifuncposition()
*
**************************************************************************/
void G__resetifuncposition()
{
  if(G__incset_def_struct_member && 'n'==G__struct.type[G__incset_tagdefining]){
    G__tagnum = G__incset_tagnum;
    G__p_ifunc = G__incset_p_ifunc;
    G__func_now = G__incset_func_now;
    G__func_page = G__incset_func_page;
    G__var_type = G__incset_var_type;
  }
  else {
    G__tagnum = -1;
    G__p_ifunc = &G__ifunc;
    G__func_now = -1;
    G__func_page = 0;
    G__var_type = 'p';
  }
  G__globalvarpointer = G__PVOID;
  G__static_alloc = 0;
  G__access = G__PUBLIC;
  G__typenum = -1;
}

/**************************************************************************
* G__setnull()
*
**************************************************************************/
void G__setnull(result)
G__value *result;
{
  *result = G__null;
}

/**************************************************************************
* G__getstructoffset()
*
**************************************************************************/
long G__getstructoffset()
{
  return(G__store_struct_offset);
}

/**************************************************************************
* G__getaryconstruct()
*
**************************************************************************/
int G__getaryconstruct()
{
  return(G__cpp_aryconstruct);
}

/**************************************************************************
* G__gettempbufpointer()
*
**************************************************************************/
long G__gettempbufpointer()
{
  return(G__p_tempbuf->obj.obj.i);
}

/**************************************************************************
* G__setsizep2memfunc()
*
**************************************************************************/
void G__setsizep2memfunc(sizep2memfunc)
int sizep2memfunc;
{
  G__sizep2memfunc = sizep2memfunc;
}

/**************************************************************************
* G__getsizep2memfunc()
*
**************************************************************************/
int G__getsizep2memfunc()
{
   G__asm_noverflow = G__store_asm_noverflow;
   G__no_exec_compile = G__store_no_exec_compile;
   G__asm_exec = G__store_asm_exec;
  return(G__sizep2memfunc);
}


/**************************************************************************
* G__setInitFunc()
*
**************************************************************************/
void G__setInitFunc(initfunc)
char *initfunc;
{
  G__INITFUNC=initfunc;
}

#ifdef G__WILDCARD
/**************************************************************************
* Access functions for WildCard interpreter
**************************************************************************/

/**************************************************************************
* G__getIfileFp()
**************************************************************************/
FILE *G__getIfileFp()
{
   return(G__ifile.fp);
}

/**************************************************************************
* G__incIfileLineNumber()
**************************************************************************/
void G__incIfileLineNumber()
{
  ++G__ifile.line_number;
}

/**************************************************************************
* G__getIfileLineNumber()
**************************************************************************/
int G__getIfileLineNumber()
{
  return(G__ifile.line_number);
}

/**************************************************************************
* G__setReturn()
**************************************************************************/
void G__setReturn(rtn)
int rtn;
{
  G__return=rtn;
}

/**************************************************************************
* G__getFuncNow()
**************************************************************************/
int G__getFuncNow()
{
  return(G__func_now);
}

/**************************************************************************
* G__getPrerun
**************************************************************************/
int G__getPrerun()
{
  return(G__prerun);
}

/**************************************************************************
* G__setPrerun()
**************************************************************************/
void G__setPrerun(prerun)
int prerun;
{
  G__prerun=prerun;
}

/**************************************************************************
* G__getDispsource()
**************************************************************************/
short G__getDispsource()
{
  return(G__dispsource);
}

/**************************************************************************
* G__getSerr()
**************************************************************************/
FILE* G__getSerr()
{
  return(G__serr);
}

/**************************************************************************
* G__getIsMain()
**************************************************************************/
int G__getIsMain()
{
  return(G__ismain);
}

/**************************************************************************
* G__setIsMain()
**************************************************************************/
void G__setIsMain(ismain)
int ismain;
{
  G__ismain=ismain;
}

/**************************************************************************
* G__setStep()
**************************************************************************/
void G__setStep(step)
int step;
{
  G__step=step;
}

/**************************************************************************
* G__getStepTrace()
**************************************************************************/
int G__getStepTrace()
{
  return(G__steptrace);
}

/**************************************************************************
* G__setDebug
**************************************************************************/
void G__setDebug(dbg)
int dbg;
{
  G__debug=dbg;
}

/**************************************************************************
* G__getDebugTrace
**************************************************************************/
int G__getDebugTrace()
{
  return(G__debugtrace);
}

/**************************************************************************
* G__set_asm_noverflow
**************************************************************************/
void G__set_asm_noverflow(novfl)
int novfl;
{
  G__asm_noverflow=novfl;
}

/**************************************************************************
* G__get_no_exec()
**************************************************************************/
int G__get_no_exec()
{
  return(G__no_exec);
}

/**************************************************************************
* int G__get_no_exec_compile
**************************************************************************/
int G__get_no_exec_compile()
{
  return(G__no_exec_compile);
}
#endif /* G__WILDCARD */

/* #ifndef G__OLDIMPLEMENTATION1167 */
/**************************************************************************
* G__Charref()
**************************************************************************/
char* G__Charref(buf)
G__value *buf;
{
  if('c'==buf->type && buf->ref) 
    return((char*)buf->ref);
  else if('d'==buf->type || 'f'==buf->type) 
    buf->obj.ch = (char)buf->obj.d;
  else 
    buf->obj.ch = (char)buf->obj.i; 
  return(&buf->obj.ch);
}

/**************************************************************************
* G__Shortref()
**************************************************************************/
short* G__Shortref(buf)
G__value *buf;
{
  if('s'==buf->type && buf->ref) 
    return((short*)buf->ref);
  else if('d'==buf->type || 'f'==buf->type) 
    buf->obj.sh = (short)buf->obj.d;
  else 
    buf->obj.sh = (short)buf->obj.i; 
  return(&buf->obj.sh);
}

/**************************************************************************
* G__Intref()
**************************************************************************/
int* G__Intref(buf)
G__value *buf;
{
  if('i'==buf->type && buf->ref) 
    return((int*)buf->ref);
  else if('d'==buf->type || 'f'==buf->type) 
    buf->obj.in = (int)buf->obj.d;
  else 
    buf->obj.in = (int)buf->obj.i;
  return(&buf->obj.in);
}

/**************************************************************************
* G__Longref()
**************************************************************************/
long* G__Longref(buf)
G__value *buf;
{
  if('l'==buf->type && buf->ref) 
    return((long*)buf->ref);
  else if('d'==buf->type || 'f'==buf->type) 
    buf->obj.i = (long)buf->obj.d;
  /* else 
    buf->obj.i = (long)buf->obj.i; */
  return(&buf->obj.i);
}

/**************************************************************************
* G__UCharref()
**************************************************************************/
unsigned char* G__UCharref(buf)
G__value *buf;
{
  if('b'==buf->type && buf->ref) 
    return((unsigned char*)buf->ref);
  else if('d'==buf->type || 'f'==buf->type) 
    buf->obj.uch = (unsigned char)buf->obj.d;
  else 
    buf->obj.uch = (unsigned char)buf->obj.i; 
  return(&buf->obj.uch);
}

/**************************************************************************
* G__Boolref()
**************************************************************************/
unsigned char* G__Boolref(buf)
G__value *buf;
{
#ifdef G__BOOL4BYTE
  if('g'==buf->type && buf->ref) 
    return((unsigned char*)buf->ref);
  else if('d'==buf->type || 'f'==buf->type) 
    buf->obj.i = (int)buf->obj.d;
  else 
    buf->obj.i = (int)buf->obj.i; 
  return((unsigned char*)&buf->obj.i);
#else
  if('g'==buf->type && buf->ref) 
    return((unsigned char*)buf->ref);
  else if('d'==buf->type || 'f'==buf->type) 
    buf->obj.uch = (unsigned char)buf->obj.d;
  else 
    buf->obj.uch = (unsigned char)buf->obj.i; 
  return(&buf->obj.uch);
#endif
}

/**************************************************************************
* G__UShortref()
**************************************************************************/
unsigned short* G__UShortref(buf)
G__value *buf;
{
  if('r'==buf->type && buf->ref) 
    return((unsigned short*)buf->ref);
  else if('d'==buf->type || 'f'==buf->type) 
    buf->obj.ush = (unsigned short)buf->obj.d;
  else 
    buf->obj.ush = (unsigned short)buf->obj.i; 
  return(&buf->obj.ush);
}

/**************************************************************************
* G__UIntref()
**************************************************************************/
unsigned int* G__UIntref(buf)
G__value *buf;
{
  if('h'==buf->type && buf->ref) 
    return((unsigned int*)buf->ref);
  else if('d'==buf->type || 'f'==buf->type) 
    buf->obj.uin = (unsigned int)buf->obj.d;
  /* else 
    buf->obj.uin = (unsigned int)buf->obj.i; */
  return(&buf->obj.uin);
}

/**************************************************************************
* G__ULongref()
**************************************************************************/
unsigned long* G__ULongref(buf)
G__value *buf;
{
  if('k'==buf->type && buf->ref) 
    return((unsigned long*)buf->ref);
  else if('d'==buf->type || 'f'==buf->type) 
    buf->obj.ulo = (unsigned long)buf->obj.d;
  /* else 
    buf->obj.ulo = (unsigned long)buf->obj.i; */
  return(&buf->obj.ulo);
}

/**************************************************************************
* G__Floatref()
**************************************************************************/
float* G__Floatref(buf)
G__value *buf;
{
  if('f'==buf->type && buf->ref) {
    return((float*)buf->ref);
  }
  else if('d'==buf->type || 'f'==buf->type) 
    buf->obj.fl = (float)buf->obj.d;
  else 
    buf->obj.fl = (float)buf->obj.i; 
  return(&buf->obj.fl);
}

/**************************************************************************
* G__Doubleref()
**************************************************************************/
double* G__Doubleref(buf)
G__value *buf;
{
  if('d'==buf->type && buf->ref) 
    return((double*)buf->ref);
  else if('d'==buf->type || 'f'==buf->type) 
    return(&buf->obj.d);
  else 
    buf->obj.d = (double)buf->obj.i; 
  return(&buf->obj.d);
}
/* #endif   ON1167 */

/**************************************************************************
* G__Longlongref()
**************************************************************************/
G__int64* G__Longlongref(buf)
G__value *buf;
{
  if('n'==buf->type && buf->ref) 
    return((G__int64*)buf->ref);
  else if('m'==buf->type && buf->ref) 
    buf->obj.ll = (G__int64)buf->obj.ull;
  else if('d'==buf->type || 'f'==buf->type) 
    buf->obj.ll = (G__int64)buf->obj.d;
  else if('n'==buf->type) 
    buf->obj.ll = (G__int64)buf->obj.ll;
  else if('m'==buf->type) 
    buf->obj.ll = (G__int64)buf->obj.ull;
  else
    buf->obj.ll = (G__int64)buf->obj.i;
  return(&buf->obj.ll);
}
/**************************************************************************
* G__ULonglongref()
**************************************************************************/
G__uint64* G__ULonglongref(buf)
G__value *buf;
{
  if('m'==buf->type && buf->ref) 
    return((G__uint64*)buf->ref);
  else if('n'==buf->type && buf->ref) 
    buf->obj.ull = (G__uint64)buf->obj.ll;
  else if('d'==buf->type || 'f'==buf->type) 
    buf->obj.ull = (G__uint64)buf->obj.d;
  else if('n'==buf->type) 
    buf->obj.ull = (G__int64)buf->obj.ll;
  else if('m'==buf->type) 
    buf->obj.ull = (G__int64)buf->obj.ull;
  else 
    buf->obj.ull = (G__uint64)buf->obj.i;
  return(&buf->obj.ull);
}
/**************************************************************************
* G__Longdoubleref()
**************************************************************************/
long double* G__Longdoubleref(buf)
G__value *buf;
{
  if('q'==buf->type && buf->ref) 
    return((long double*)buf->ref);
  else if('n'==buf->type)
    buf->obj.ld = (long double)buf->obj.ll;
  else if('m'==buf->type) {
#ifdef G__WIN32
    buf->obj.ld = (long double)buf->obj.ll;
#else
    buf->obj.ld = (long double)buf->obj.ull;
#endif
  }
  else if('d'==buf->type || 'f'==buf->type) 
    buf->obj.ld = (long double)buf->obj.d;
  else 
    buf->obj.ld = (long double)buf->obj.i;
  return(&buf->obj.ld);
}



/**************************************************************************
* G__specify_extra_include()
* has to be called from the pragma decoding!
**************************************************************************/
void G__specify_extra_include() {
  int i;
  int c;
  char buf[G__ONELINE];
  char *tobecopied;
  if (!G__extra_include) {
    G__extra_include = (char**)malloc(G__MAXFILE*sizeof(char*));
    for(i=0;i<G__MAXFILE;i++) 
      G__extra_include[i]=(char*)malloc(G__MAXFILENAME*sizeof(char));
  };
  c = G__fgetstream_template(buf,";\n\r<>");
  if ( 1 ) { /* should we check if the file exist ? */
    tobecopied = buf;
    if (buf[0]=='\"' || buf[0]=='\'') tobecopied++;
    i = strlen(buf);
    if (buf[i-1]=='\"' || buf[i-1]=='\'') buf[i-1]='\0';
    strcpy(G__extra_include[G__extra_inc_n++],tobecopied);
  }
}

/**************************************************************************
 * G__gen_extra_include()
 * prepend the extra header files to the C or CXX file
 **************************************************************************/
void G__gen_extra_include() {
  char * tempfile;
  FILE *fp,*ofp;
  char line[BUFSIZ];
  int i;
  
  if (G__extra_inc_n) {
#ifndef G__ADD_EXTRA_INCLUDE_AT_END
    /* because of a bug in (at least) the KAI compiler we have to
       add the files at the beginning of the dictionary header file
       (Specifically, the extra include files have to be include 
       before any forward declarations!) */
    
    tempfile = (char*) malloc(strlen(G__CPPLINK_H)+6);
    sprintf(tempfile,"%s.temp", G__CPPLINK_H);
    rename(G__CPPLINK_H,tempfile);
    
    fp = fopen(G__CPPLINK_H,"w");
    if(!fp) G__fileerror(G__CPPLINK_H);
    ofp = fopen(tempfile,"r");
    if(!ofp) G__fileerror(tempfile);
    
    /* Add the extra include ad the beginning of the files */
    fprintf(fp,"\n/* Includes added by #pragma extra_include */\n");
    for(i=0; i< G__extra_inc_n; i++) {
      fprintf(fp,"#include \"%s\"\n",G__extra_include[i]);
    }
    
    /* Copy rest of the header file */
    while (fgets(line, BUFSIZ, ofp)) {
      fprintf(fp, "%s", line);
    }
    fprintf(fp,"\n");
    
    fclose(fp);
    fclose(ofp);
    FileWaitTime(10000);
    unlink(tempfile);
    free(tempfile);
    
#else
    fp = fopen(G__CPPLINK_H,"a");
    if(!fp) G__fileerror(G__CPPLINK_H);
    
    fprintf(fp,"\n/* Includes added by #pragma extra_include */\n");
    for(i=0; i< G__extra_inc_n; i++) {
      fprintf(fp,"#include \"%s\"\n",G__extra_include[i]);
    }
    fprintf(fp,"\n");
    fclose(fp);    
    FileWaitTime(10000);
#endif
    
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
