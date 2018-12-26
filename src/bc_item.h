/* /% C %/ */
/***********************************************************************
 * cint (C/C++ interpreter)
 ************************************************************************
 * Source file bc_item.h
 ************************************************************************
 * Description:
 *  item expression compiler
 *   object
 *   function
 *   object.member
 *   ::member
 *   object.member
 *   pointer->member
 *   object->member      (object.operator->())->member
 *   pointer[expr]
 *   array[expr][expr][expr]
 *   object[expr]
 *   (type)expr
 *   (expr)
 *   object(expr,expr)
 *   function(expr,expr)
 ************************************************************************
 * Copyright(c) 2004~2005  Masaharu Goto 
 *
 * Permission to use, copy, modify and distribute this software and its 
 * documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  The author makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 ************************************************************************/

#ifndef BC_ITEM_H
#define BC_ITEM_H

extern "C" {
#ifdef __CINT__
#include "../G__ci.h"
#else
#include "common.h"
#endif
}

#include "bc_parse.h"

////////////////////////////////////////////////////////////////////
// G__object_id
////////////////////////////////////////////////////////////////////
struct G__object_id {
 public:
  struct G__var_array *m_var;
  int m_ig15;
  struct G__ifunc_table *m_ifunc;
  int m_ifn;
  G__value m_obj;
};


////////////////////////////////////////////////////////////////////
// G__blockscope_expr
////////////////////////////////////////////////////////////////////
class G__blockscope_expr {
 public:
  G__blockscope_expr(G__blockscope* blockscope);
  ~G__blockscope_expr() {}

  G__value getitem(const string& item);  

 private:
  G__value getitem(const string& item,int i) 
    { return(getitem(item.substr(i))); }

  G__value scope_operator(const string& item,int& i);
  G__value member_operator(const string& item,int& i);
  G__value pointer_operator(const string& item,int& i);
  G__value index_operator(const string& item,int& i);
  G__value fcall_operator(const string& item,int& i);

 private:
  G__value getobject(const string& name,G__object_id* id=0);
  G__ClassInfo getscope(const string& name);
  G__TypeInfo gettype(const string& name);
  G__MethodInfo getfunction(const string& name);

 private:
  G__blockscope *m_blockscope;
  G__bc_inst* m_pinst;
  int m_isfixed;
  int m_isobject;
  G__ClassInfo m_localscope;
};


////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////


#endif
