/* /% C %/ */
/***********************************************************************
 * cint (C/C++ interpreter)
 ************************************************************************
 * Header file bc_exec.h
 ************************************************************************
 * Description:
 *  bytecode executor, execution subsystem
 ************************************************************************
 * Copyright(c) 2004~2004  Masaharu Goto 
 *
 * Permission to use, copy, modify and distribute this software and its 
 * documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  The author makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 ************************************************************************/

#ifndef BC_EXEC_H
#define BC_EXEC_H

extern "C" {
#ifdef __CINT__
#include "../G__ci.h"
#else
#include "common.h"
#endif
}

#include "bc_vtbl.h"
using namespace std;

/***********************************************************************
* G__bc_compile_error
***********************************************************************/
class G__bc_compile_error /* : public exception */ {
  // just to identify error type
};

/***********************************************************************
* G__bc_runtime_error
***********************************************************************/
class G__bc_runtime_error /* : public exception */ {
  // just to identify error type
};


//////////////////////////////////////////////////////////////////////
class G__bc_store_bytecode_env {
 public:
  void save() {
    m_asm_inst = G__asm_inst;
    m_asm_stack = G__asm_stack;
    m_asm_name = G__asm_name;
    m_asm_name_p = G__asm_name_p;
    m_asm_param  = G__asm_param ;
    m_asm_exec  = G__asm_exec ;
    m_asm_noverflow  = G__asm_noverflow ;
    m_asm_cp  = G__asm_cp ;
    m_asm_dt  = G__asm_dt ;
    m_asm_index  = G__asm_index ;
    m_tagnum = G__tagnum;
    //
    m_scopelevel = G__scopelevel;
    m_catchexception = G__catchexception;
  }
  void restore() {
    G__asm_inst = m_asm_inst;
    G__asm_stack = m_asm_stack;
    G__asm_name = m_asm_name;
    G__asm_name_p = m_asm_name_p;
    G__asm_param  = m_asm_param ;
    G__asm_exec  = m_asm_exec ;
    G__asm_noverflow  = m_asm_noverflow ;
    G__asm_cp  = m_asm_cp ;
    G__asm_dt  = m_asm_dt ;
    G__asm_index  = m_asm_index ;
    G__tagnum = m_tagnum;
    //
    G__scopelevel = m_scopelevel;
    G__catchexception = m_catchexception;
  }
 private:
  // from legacy Cint
  long *m_asm_inst;
  G__value *m_asm_stack;
  char *m_asm_name;
  int m_asm_name_p;
  struct G__param *m_asm_param;
  int m_asm_exec;
  int m_asm_noverflow;
  int m_asm_cp;
  int m_asm_dt;
  int m_asm_index; /* maybe unneccessary */
  int m_tagnum;
  // introduced in ver6
  int m_scopelevel;
  int m_catchexception;
};

//////////////////////////////////////////////////////////////////////
extern "C" int G__bc_exec_virtualbase_bytecode(G__value *result7
			,char *funcname        // objtagnum
			,struct G__param *libp
			,int hash              // vtblindex,basetagnum
			) ;
extern "C" int G__bc_exec_virtual_bytecode(G__value *result7
			,char *funcname        // vtagnum
			,struct G__param *libp
			,int hash              // vtblindex,basetagnum
			) ;
extern "C" int G__bc_exec_normal_bytecode(G__value *result7
			,char *funcname        // ifunc
			,struct G__param *libp
			,int hash              // ifn
			) ;
extern "C" int G__bc_exec_ctor_bytecode(G__value *result7
			,char *funcname        // ifunc
			,struct G__param *libp
			,int hash              // ifn
			) ;
extern "C" int G__bc_exec_ctorary_bytecode(G__value *result7
			,char *funcname        // ifunc
			,struct G__param *libp 
			,int hash              // ifn
			) ;
extern "C" int G__bc_exec_dtorary_bytecode(G__value *result7
			,char *funcname        // ifunc
			,struct G__param *libp
			,int hash              // ifn
			) ;
extern "C" int G__bc_exec_try_bytecode(int start,
				       int stack,
				       G__value *presult,
				       long localmem) ;
extern "C" int G__bc_exec_throw_bytecode(G__value* pval);
extern "C" int G__bc_exec_typematch_bytecode(G__value* catchtype,G__value* excptobj);
extern "C" int G__exec_bytecode(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash);
extern "C" int G__bc_throw_compile_error();
extern "C" int G__bc_throw_runtime_error();

#endif

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
