/* /% C %/ */
/***********************************************************************
 * cint (C/C++ interpreter)
 ************************************************************************
 * Source file bc_item.cxx
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

#include "bc_item.h"

////////////////////////////////////////////////////////////////////
// G__blockscope_expr
////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////
G__blockscope_expr::G__blockscope_expr(G__blockscope* blockscope) {
  m_blockscope = blockscope;
  m_pinst = &blockscope->GetInst();
  m_isfixed=0;
  m_isobject=0;
  m_localscope.Init(-1);
}

////////////////////////////////////////////////////////////////////
G__value G__blockscope_expr::getitem(const string& item) {
  G__value result;
  int i=0;
  int c;
  while((c=item[i])) {
    switch(c) {
    case ':':
      if(':'==item[i+1]) {
        result = scope_operator(item,i);
      }
      else {
        //error
      }
      break;

    case '.':
      result = member_operator(item,i);
      break;

    case '-':
      if('>'==item[i+1]) {
        result = pointer_operator(item,i);
      }
      else {
        //error
      }
      break;
      
    case '[':
      result = index_operator(item,i);
      break;

    case '(':
      result = fcall_operator(item,i);
      break;

    case 0:
      result = getobject(item);
      break;

    default:
      break;
    }
    ++i;
  }
  return(result);
}
////////////////////////////////////////////////////////////////////
G__value G__blockscope_expr::scope_operator(const string& item,int& i) {
  //class_name::member
  //::member
  if(i==0) { 
    //::member
    m_isfixed=1;
    m_localscope.Init();
    return(getitem(item,i+2));
  }
  else {
    string scopename=item.substr(0,i);
    G__ClassInfo scope = getscope(scopename);
    if(scope.IsValid()) {
      m_isfixed=1;
      m_localscope.Init(scope.Tagnum());
      return(getitem(item,i+2));
    }
    // error
    G__fprinterr(G__serr,"Error: undefined scope name '%s'",scopename.c_str());
    G__genericerror((char*)NULL);
  }
  return(G__null);
}

////////////////////////////////////////////////////////////////////
G__value G__blockscope_expr::member_operator(const string& item,int& i) {
  //object.member
  //  G__getexpr(object)
  //  PUSHSTROS
  //  SETSTROS
  //  -> member
  //  POPSTROS

  string name=item.substr(0,i);
  G__value obj = getobject(name);
  m_localscope.Init(obj.tagnum);
  m_isobject = 1;
  m_isfixed=0;

  m_pinst->PUSHSTROS();
  m_pinst->SETSTROS();

  G__value result = getitem(item,i+1);
  
  m_pinst->POPSTROS();

  m_localscope.Init(-1);
  m_isobject = 0;

  return(result);
}
////////////////////////////////////////////////////////////////////
G__value G__blockscope_expr::pointer_operator(const string& item,int& i) {
  //pointer->member
  //  G__getexpr(pointer)
  //  PUSHSTROS
  //  SETSTROS
  //  -> member
  //  POPSTROS
  //object->member      (object.operator->())->member
  //  G__getexpr(object)
  //  PUSHSTROS
  //  SETSTROS
  //  operator->
  //  PUSHSTROS
  //  SETSTROS
  //  -> member
  //  POPSTROS
  //  POPSTROS

  string name=item.substr(0,i);
  G__value obj = getobject(name);
  m_isfixed=0;
  G__TypeReader ty(obj);

  if(ty.Ispointer() && -1!=ty.Tagnum()) {
    //pointer->member
    //  G__getexpr(pointer)
    //  PUSHSTROS
    //  SETSTROS
    //  -> member
    //  POPSTROS

    m_localscope.Init(obj.tagnum);
    m_isobject = 1;
    
    m_pinst->PUSHSTROS();
    m_pinst->SETSTROS();
    
    G__value result = getitem(item,i+2);
    
    m_pinst->POPSTROS();
    
    m_localscope.Init(-1);
    m_isobject = 0;
    return(result);
  }
  else if(ty.Ispointer()==0 && -1!=ty.Tagnum()) {
    //object->member      (object.operator->())->member
    //  G__getexpr(object)
    //  PUSHSTROS
    //  SETSTROS
    //  operator->
    //  PUSHSTROS
    //  SETSTROS
    //  -> member
    //  POPSTROS
    //  POPSTROS

    m_localscope.Init(ty.Tagnum());
    m_isobject = 1;
    
    m_pinst->PUSHSTROS();
    m_pinst->SETSTROS();

    struct G__param para;
    para.paran=0;
    G__value obj = m_blockscope->call_func(m_localscope,"operator->",&para,0,0
                                           ,G__ClassInfo::ExactMatch);
    m_localscope.Init(obj.tagnum);
    m_isobject = 1;
    m_isfixed=0;

    m_pinst->PUSHSTROS();
    m_pinst->SETSTROS();

    G__value result = getitem(item,i+1);

    m_pinst->POPSTROS();
    m_pinst->POPSTROS();
    
    m_localscope.Init(-1);
    m_isobject = 0;
    return(result);
  }
  else {
    // error
  }

}
////////////////////////////////////////////////////////////////////
G__value G__blockscope_expr::index_operator(const string& item,int& i) {
  //pointer[expr]
  //array[expr][expr][expr]
  //   G__getexpr(expr)
  //   LD_VAR pointer index=1
  //object[expr]
  //   SETMEMFUNCENV
  //   G__getexpr(expr)
  //   RECMEMFUNCENV
  //   G__getexpr(object)
  //   PUSHSTROS
  //   SETSTROS
  //   LD_FUNC operator[] paran=1
  //   POPSTROS

  string name=item.substr(0,i);

  G__object_id objid;
  getobject(name,&objid);

  m_isfixed=0;
  G__TypeReader ty(objid.m_obj);

  if(ty.Ispointer() && -1!=ty.Tagnum()) {
  }
  return(G__value());
}
////////////////////////////////////////////////////////////////////
G__value G__blockscope_expr::fcall_operator(const string& item,int& i) {
  //(type)expr
  //   G__getexpr(expr)
  //   CAST type
  //(expr)
  //   G__getexpr(expr)
  //object(expr,expr)
  //   G__getexpr(expr)
  //   G__getexpr(expr)
  //   G__getexpr(object)
  //   PUSHSTROS
  //   SETSTROS
  //   LD_FUNC operator() paran
  //   POPSTROS
  //This happens the last, since function overloading makes it complicated
  //function(expr,expr)
  //   G__getexpr(expr)
  //   G__getexpr(expr)
  //   LD_FUNC function paran
  return(G__value());
}
////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////
G__value G__blockscope_expr::getobject(const string& name,G__object_id* id) {

  return(G__value());
}

////////////////////////////////////////////////////////////////////
G__ClassInfo G__blockscope_expr::getscope(const string& name) {
  int hash,ig15;
  const char* pname = name.c_str();
  G__hash(name,hash,ig15);
  for(int i=0;i<G__struct.alltag;i++) {
    if(hash==G__struct.hash[i] && strcmp(G__struct.name[i],pname)==0) {
      if(m_isfixed && m_localscope.Tagnum()!=G__struct.parent_tagnum[i]) {
        continue;
      }
      if(-1!=G__struct.parent_tagnum[i]) {
        int tagnum = m_blockscope->GetTagnum();
        int j=i;
        while(-1!=j) {
          if(j==tagnum) {
            G__ClassInfo scope(i);
            return(scope);
          }
          j=G__struct.parent_tagnum[j];
        }
        continue;
      }
      G__ClassInfo scope(i);
      return(scope);
    }
  }
}

////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////
extern "C" G__value G__bc_getitem(char* item) {
  G__blockscope_expr expr(G__currentscope);
  return(expr.getitem(string(item)));
}

////////////////////////////////////////////////////////////////////




