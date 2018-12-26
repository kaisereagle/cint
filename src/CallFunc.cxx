/* /% C++ %/ */
/***********************************************************************
 * cint (C/C++ interpreter)
 ************************************************************************
 * Source file CallFunc.cxx
 ************************************************************************
 * Description:
 *  Extended Run Time Type Identification API
 ************************************************************************
 * Copyright(c) 1995~2003  Masaharu Goto 
 *
 * Permission to use, copy, modify and distribute this software and its 
 * documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  The author makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 ************************************************************************/

#include "Api.h"
#include "common.h"

/*********************************************************************
* class G__CallFunc
*
* Usage1:
*
*   TCanvas c[10];
*   void *address;
*   long offset;
*   G__CallFunc func;
*   G__ClassInfo canvas("TCanvas");
*   // set pointer to interface method and argument
*   func.SetFunc(&canvas,"Draw","\"ABC\",1234,3.14",&offset);
*   // call function
*   for(int i=0;i<10;i++) {
*     address = (void*)(&c[i]) + offset;
*     func.Exec(address);
*   }
*   // reset everything
*   func.Init();
*
*
* Usage2:
*
*   TCanvas c[10];
*   void *address;
*   long offset;
*   G__CallFunc func;
*   G__ClassInfo canvas("TCanvas");
*   // set pointer to interface method
*   func.SetFunc(canvas.GetMethod("Draw","char*,int,double",&offset).InterfaceMethod());
*   // set arguments
*   char *title="ABC";
*   func.SetArg((long)title);
*   func.SetArg((long)1234);
*   func.SetArg((double)3.14);
*   // call function
*   for(int i=0;i<10;i++) {
*     address = (void*)(&c[i]) + offset;
*     func.Exec(address);
*   }
*   // reset everything
*   func.Init();
* 
*********************************************************************/
///////////////////////////////////////////////////////////////////////////
G__CallFunc::G__CallFunc()
{
  G__LockCriticalSection();
  Init();
  G__UnlockCriticalSection();
}
///////////////////////////////////////////////////////////////////////////
void G__CallFunc::Init()
{
  pfunc = (G__InterfaceMethod)NULL;
  para.paran = 0;
  para.para[0] = G__null;
  para.para[1] = G__null;
  result = G__null;
#ifdef G__ASM_WHOLEFUNC
  bytecode = (struct G__bytecodefunc*)NULL;
#endif
}
///////////////////////////////////////////////////////////////////////////
void G__CallFunc::SetFunc(G__InterfaceMethod f)
{
  pfunc = f; // Set pointer to interface method
}
///////////////////////////////////////////////////////////////////////////
void G__CallFunc::SetFunc(G__MethodInfo m)
{
  method = m;
  pfunc = m.InterfaceMethod(); // Set pointer to interface method
}
#ifdef G__ASM_WHOLEFUNC
///////////////////////////////////////////////////////////////////////////
void G__CallFunc::SetBytecode(struct G__bytecodefunc* bc)
{
  bytecode = bc;
  if(bytecode) pfunc = (G__InterfaceMethod)G__exec_bytecode;
  else {
    pfunc = (G__InterfaceMethod)NULL;
#ifndef G__ROOT
    if(G__asm_dbg) {
      if(G__dispmsg>=G__DISPWARN) {
	G__fprinterr(G__serr,"Warning: Bytecode compilation of %s failed. G__CallFunc::Exec may be slow\n",method.Name());
      }
    }
#endif
  }
  para.paran=0;
}
#endif
///////////////////////////////////////////////////////////////////////////
void G__CallFunc::SetArgArray(long *p,int narg)
{
  int i,n;
  if(method.IsValid()) {
    if(0>narg) n = method.NArg();
    else {
      n = narg;
      if (narg > method.NArg()) {
	G__fprinterr(G__serr,"Warning: G__CallFunc::SetArgArray() too many arguments specified (%d expected %d)\n",narg,method.NArg());
	G__printlinenum();
	/* G__genericerror((char*)NULL); */
        n = method.NArg();
      }
      else if(n<method.NArg()-method.NDefaultArg()) {
	G__fprinterr(G__serr,"Error: G__CallFunc::SetArgArray() too few arguments");
	G__printlinenum();
	/* G__genericerror((char*)NULL); */
        n = method.NArg();
      }
    }
    G__MethodArgInfo arginfo;
    arginfo.Init(method);
#ifndef G__OLDIMPLEMENTATION1707
    para.paran=0;
#endif
    for(i=0;i<n;i++) {
#if !defined(G__OLDIMPLEMENTATION1852)
      para.para[i].obj.i = p[i];
      para.para[i].ref = p[i];
      // Following data shouldn't matter, but set just in case
      arginfo.Next();
      para.para[i].type = arginfo.Type()->Type();
      para.para[i].tagnum = arginfo.Type()->Tagnum();
      para.para[i].typenum = arginfo.Type()->Typenum();
      para.paran=i+1;
#elif !defined(G__OLDIMPLEMENTATION1707)
      if (p[i]) {
        para.para[i].obj.i = p[i];
        para.para[i].ref = p[i];
        // Following data shouldn't matter, but set just in case
        arginfo.Next();
        para.para[i].type = arginfo.Type()->Type();
        para.para[i].tagnum = -1;
        para.para[i].typenum = -1;
        para.paran=i+1;
      } else
        break;
#else /* 1707 */
      para.para[i].obj.i = p[i];
      para.para[i].ref = p[i];
      // Following data shouldn't matter, but set just in case
      arginfo.Next();
      para.para[i].type = arginfo.Type()->Type();
      para.para[i].tagnum = -1;
      para.para[i].typenum = -1;
#endif /* 1707 */
    }
#ifdef G__OLDIMPLEMENTATION1707
    para.paran=n;
#endif
  }
  else {
    G__fprinterr(G__serr,"Error: G__CallFunc::SetArgArray() must be initialized with 'G__CallFunc::SetFunc(G__ClassInfo* cls,char* fname,char* args,long* poffset)' first\n");
  }
}
///////////////////////////////////////////////////////////////////////////
void G__CallFunc::SetArg(long l)
{
  para.para[para.paran].obj.i = l;  
  para.para[para.paran].ref = l;
  // Following data shouldn't matter, but set just in case
  para.para[para.paran].type = 'l';
  para.para[para.paran].tagnum = -1;
  para.para[para.paran].typenum = -1;
  ++para.paran; // Increment number of argument
}
///////////////////////////////////////////////////////////////////////////
void G__CallFunc::SetArg(double d)
{
  para.para[para.paran].obj.d = d;
  // Following data shouldn't matter, but set just in case
  para.para[para.paran].ref = 0 ;
  para.para[para.paran].type = 'd';
  para.para[para.paran].tagnum = -1;
  para.para[para.paran].typenum = -1;
  ++para.paran; // Increment number of argument
}
#ifdef G__NATIVELONGLONG
///////////////////////////////////////////////////////////////////////////
void G__CallFunc::SetArg(G__int64 ll)
{
  para.para[para.paran].obj.ll = ll;  
  para.para[para.paran].ref = 0;
  // Following data shouldn't matter, but set just in case
  para.para[para.paran].type = 'n';
  para.para[para.paran].tagnum = -1;
  para.para[para.paran].typenum = -1;
  ++para.paran; // Increment number of argument
}
///////////////////////////////////////////////////////////////////////////
void G__CallFunc::SetArg(G__uint64 ull)
{
  para.para[para.paran].obj.ull = ull;  
  para.para[para.paran].ref = 0;
  // Following data shouldn't matter, but set just in case
  para.para[para.paran].type = 'm';
  para.para[para.paran].tagnum = -1;
  para.para[para.paran].typenum = -1;
  ++para.paran; // Increment number of argument
}
///////////////////////////////////////////////////////////////////////////
void G__CallFunc::SetArg(long double ld)
{
  para.para[para.paran].obj.ld = ld;
  // Following data shouldn't matter, but set just in case
  para.para[para.paran].ref = 0 ;
  para.para[para.paran].type = 'q';
  para.para[para.paran].tagnum = -1;
  para.para[para.paran].typenum = -1;
  ++para.paran; // Increment number of argument
}
#endif
///////////////////////////////////////////////////////////////////////////
void G__CallFunc::SetArgs(const char* args)
{
  int isrc=0;
  char *endmark=(char*)",";
  char *tmp = new char[ strlen(args)+2 ]; 
  tmp[0] = 0;

  // separate and evaluate argument list
  para.paran=0;
  int c;
  do {
    c=G__getstream((char*)args,&isrc,tmp,endmark);
    if (tmp[0]) {
      // evaluate arg
      para.para[para.paran] = G__calc(tmp);
      if(strlen(tmp)<G__ONELINE-1) strcpy(para.parameter[para.paran],tmp);
      else para.parameter[para.paran][0]=0;
      ++para.paran; // increment argument count
    }
  } while (','==c);

  delete [] tmp;
}
///////////////////////////////////////////////////////////////////////////
void G__CallFunc::SetFuncProto(G__ClassInfo* cls
			       ,const char* fname  ,const char* argtype
			       ,long* poffset)
{
  G__LockCriticalSection();

  method = cls->GetMethod(fname,argtype,poffset); // get G__MethodInfo object
  pfunc = method.InterfaceMethod(); // get compiled interface method
  para.paran=0; // reset parameters, not needed actually, done in SetBytecode

  G__UnlockCriticalSection();
}
///////////////////////////////////////////////////////////////////////////
void G__CallFunc::SetFunc(G__ClassInfo* cls
			  ,const char* fname  ,const char* args
			  ,long* poffset
			  ,MatchMode mode
			  )
{
  // G__getstream(), G__type2string()
  int isrc=0;
  char *endmark=(char*)",";
  char argtype[G__ONELINE];
  int pos=0;
  G__value *buf;

  // separate and evaluate argument list
  para.paran=0;
  argtype[0]='\0';
  int c;
  do {
    c=G__getstream((char*)args,&isrc,para.parameter[para.paran],endmark);
    if (para.parameter[para.paran][0]) {
      // evaluate arg
      para.para[para.paran] = G__calc(para.parameter[para.paran]);
      buf = &para.para[para.paran];
      // set type string
      if(pos) argtype[pos++]=',';
      if(islower(buf->type))
	strcpy(argtype+pos
	       ,G__type2string(buf->type,buf->tagnum,buf->typenum,0,0));
      else 
	strcpy(argtype+pos
	       ,G__type2string(buf->type,buf->tagnum,buf->typenum
			       ,buf->obj.reftype.reftype,0));
      pos = strlen(argtype);
      ++para.paran; // increment argument count
    }
  } while (','==c);

  // get G__MethodInfo object
  method=cls->GetMethod(fname,argtype,poffset,(G__ClassInfo::MatchMode)mode); 
  pfunc = method.InterfaceMethod(); // get compiled interface method
  if((G__InterfaceMethod)NULL==pfunc) {
    int store_paran=para.paran;
    SetBytecode(method.GetBytecode()); // try to compile bytecode
    para.paran=store_paran;
  }
}
///////////////////////////////////////////////////////////////////////////
G__value G__CallFunc::Execute(void *pobject)
{
  int ret;
  long store_struct_offset;
  G__LockCriticalSection();
  // Set object address
  store_struct_offset = G__store_struct_offset;
  G__store_struct_offset = (long)pobject;
  SetFuncType();
  // Call function
  G__CurrentCall(G__SETMEMFUNCENV, method.ifunc(), method.Index());
#ifdef G__ASM_WHOLEFUNC
  if(pfunc) {
    if(pfunc == G__DLL_direct_globalfunc) 
      ret = (*pfunc)(&result,(char*)method.ifunc(),&para,method.Index());
    else 
      ret = (*pfunc)(&result,(char*)bytecode,&para,0);
  }
#else
  if(pfunc) ret = (*pfunc)(&result,(char*)NULL,&para,0);
#endif
  else ret = ExecInterpretedFunc(&result);
  G__CurrentCall(G__NOP, 0, 0);
  // Restore  object address
  G__store_struct_offset = store_struct_offset;
  if(0==ret) {
    /* error */
  }
  G__UnlockCriticalSection();
  return result;
}
///////////////////////////////////////////////////////////////////////////
int G__CallFunc::ExecInterpretedFunc(G__value* presult)
{
  int ret=0;
  if(method.IsValid()) {
    G__ClassInfo *pcls=method.MemberOf();
    if(pcls && pcls->Name() && method.Name() && 
       strcmp(pcls->Name(),method.Name())==0) {
#ifdef G__ROOT
      G__store_struct_offset = (long)(new char[pcls->Size()]);
#else
      G__store_struct_offset = (long)malloc(pcls->Size());
#endif
    }
    int store_asm_exec=G__asm_exec;
    int store_asm_index=G__asm_index;
    int store_asm_noverflow = G__asm_noverflow;
    G__asm_exec=1;
    G__asm_index = method.Index();
    G__asm_noverflow = 0;
    ret = G__interpret_func(presult,(char*)method.Name()
		            ,&para,method.Hash(),method.ifunc()
			    ,G__EXACT,G__TRYNORMAL);
    G__asm_exec = store_asm_exec;
    G__asm_index= store_asm_index;
    G__asm_noverflow = store_asm_noverflow;
  }
  return(ret);
}
///////////////////////////////////////////////////////////////////////////
void G__CallFunc::SetFuncType() {
  if(method.IsValid()) {
    struct G__ifunc_table *ifunc = method.ifunc();
    int ifn = method.Index();
    result.type = ifunc->type[ifn];
    result.tagnum = ifunc->p_tagtable[ifn];
    result.typenum = ifunc->p_typetable[ifn];
    result.isconst = ifunc->isconst[ifn];
    if('d'!=result.type&&'f'!=result.type) {
      result.obj.reftype.reftype = ifunc->reftype[ifn];
    }
  }
}
///////////////////////////////////////////////////////////////////////////

