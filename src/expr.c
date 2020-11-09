/* /% C %/ */
/***********************************************************************
 * cint (C/C++ interpreter)
 ************************************************************************
 * Source file expr.c
 ************************************************************************
 * Description:
 *  Parse C/C++ expression
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

#include "common.h"

#if defined(G__NONANSI) || defined(G__SUNOS4) || defined(G__NEWSOS6) || defined(G__NEWSOS4)
#define G__OLDIMPLEMENTATION918
#endif

#ifndef G__ROOT
#define G__NOPOWEROPR
#endif

extern int G__const_noerror;
extern int G__initval_eval;
extern int G__dynconst;


#ifndef G__OLDIMPLEMENTATION1825
/******************************************************************
* G__setiparseobject()
******************************************************************/
char* G__setiparseobject(result,str)
G__value* result;
char *str;
{
  sprintf(str,"_$%c%d%c_%d_%c%lu"
	  ,result->type
	  ,0
	  ,(0==result->isconst)?'0':'1'
	  ,result->tagnum
	  ,(result->obj.i<0)?'M':'P'
	  ,labs(result->obj.i)
	  );
  return(str);
}

/******************************************************************
* G__getiparseobject()
******************************************************************/
static void G__getiparseobject(result,item)
G__value* result;
char *item;
{
  /* '_$trc_[tagnum]_[addr]' */
  char *xtmp = item+6;
  char *xx=strchr(xtmp,'_');
  result->type = item[2];
  result->obj.reftype.reftype = (int)(item[3]-'0');
  result->isconst = (int)(item[4]-'0');
  result->typenum = -1;
  *xx=0;
  result->tagnum = atoi(xtmp);
  *xx='_';
  result->obj.i = atol(xx+2);
  if('M'==xx[1]) result->obj.i = -result->obj.i;
  result->ref = result->obj.i;
}
#endif


/******************************************************************
* G__get_last_error
******************************************************************/
int G__lasterror() 
{
  return(G__last_error);
}

/******************************************************************
* G__reset_last_error
******************************************************************/
void G__reset_lasterror()
{
  G__last_error = G__NOERROR;
}

/******************************************************************
* G__value G__calc_internal(exprwithspace)
*
* Grand entry for C/C++ expression evaluator. This function is
* open to public as CINT API.
*
******************************************************************/
G__value G__calc_internal(exprwithspace)
char *exprwithspace;
{
#ifdef G__EH_SIGNAL
  void (*fpe)();
  void (*segv)();
#ifdef SIGILL
  void (*ill)();
#endif
#ifdef SIGEMT
  void (*emt)();
#endif
#ifdef SIGBUS
  void (*bus)();
#endif
#endif

#if !defined(G__OLDIMPLEMENTATION1863)
  char *exprnospace=(char*)malloc(strlen(exprwithspace)+2);
#elif !defined(G__OLDIMPLEMENTATION1802)
  char *exprnospace=(char*)malloc(strlen(exprwithspace)+1);
#else
  char exprnospace[G__ONELINE];
#endif
  int iin=0,iout=0,ipunct=0;
  int single_quote=0,double_quote=0;
  G__value result;
  int len=0;
  int store_asm_exec = G__asm_exec;
  int store_asm_noverflow = G__asm_noverflow;

  G__asm_noverflow = 0;

  G__asm_exec = 0;

  exprnospace[0]='\0';
  
  while( exprwithspace[iin] != '\0' ) {
    switch( exprwithspace[iin] ) {
    case '"' : /* double quote */
      if(single_quote==0) {
	double_quote ^= 1;
      }
      exprnospace[iout++] = exprwithspace[iin++] ;
      break;
    case '\'' : /* single quote */
      if(double_quote==0) {
	single_quote ^= 1;
      }
      exprnospace[iout++] = exprwithspace[iin++] ;
      break;
    case '\n': /* end of line */
    case '\r': /* end of line */
    case ';' : /* semi-column */
    case ' ' : /* space */
    case '\t' : /* tab */
      exprnospace[iout] = '\0'; /* temporarily terminate string */
      len=strlen(exprnospace);
      if((single_quote!=0)||(double_quote!=0)||
	 (len>=3+ipunct&&strncmp(exprnospace+ipunct,"new",3)==0)
	 || (len>=5+ipunct&&strncmp(exprnospace+ipunct,"const",5)==0)
	 ) {
	exprnospace[iout++] = exprwithspace[iin] ;
      }
      else if(len>=8&&strncmp(exprnospace,"delete[]",8)==0) {
	G__getstream(exprwithspace ,&iin ,exprnospace,"\0");
	G__delete_operator(exprnospace,1);
	result = G__null;
	goto deletereturn;
      }
      else if(len>=6&&strncmp(exprnospace,"delete",6)==0) {
	G__getstream(exprwithspace ,&iin ,exprnospace,"\0");
	if(exprnospace[0]=='[') {
	  G__delete_operator(exprnospace+2,1);
	}
	else {
	  G__delete_operator(exprnospace,0);
	}
	result = G__null;
	goto deletereturn;
      }
      iin++;
      break;
    case '=': 
    case '(': 
    case ')': 
    case ',': 
      ipunct = iout+1;
    default :
      exprnospace[iout++] = exprwithspace[iin++] ;
      break;
    }
  }
  exprnospace[iout++] = '\0';

#ifdef G__EH_SIGNAL
  fpe = signal(SIGFPE,G__error_handle);
  segv = signal(SIGSEGV,G__error_handle);
#ifdef SIGILL
  ill = signal(SIGILL,G__error_handle);
#endif
#ifdef SIGEMT
  emt = signal(SIGEMT,G__error_handle);
#endif
#ifdef SIGBUS
  bus = signal(SIGBUS,G__error_handle);
#endif
#endif

  result=G__getexpr(exprnospace);
  G__last_error = G__security_error;

#ifdef G__EH_SIGNAL
  signal(SIGFPE,fpe);
  signal(SIGSEGV,segv);
#ifdef SIGILL
  signal(SIGILL,ill);
#endif
#ifdef SIGEMT
  signal(SIGEMT,emt);
#endif
#ifdef SIGBUS
  signal(SIGBUS,bus);
#endif
#endif

  deletereturn:

  G__asm_exec = store_asm_exec;
  G__asm_noverflow = store_asm_noverflow;

#ifndef G__OLDIMPLEMENTATION1802
  free((void*)exprnospace);
#endif
  return(result);
}

/******************************************************************
* G__value G__calc(exprwithspace)
*
* Grand entry for C/C++ expression evaluator. This function is
* open to public as CINT API.
*
******************************************************************/
G__value G__calc(exprwithspace)
char *exprwithspace;
{
  G__value result;
  int store_security_error; 

  G__LockCriticalSection();

  store_security_error = G__security_error;
  G__security_error = G__NOERROR;

  G__storerewindposition();

  result = G__calc_internal(exprwithspace);

  G__security_recover(G__serr);
  
  G__security_error = store_security_error;

  G__UnlockCriticalSection();

  return(result);
}



/******************************************************************
* G__value G__conditionaloperator(expression)
*
*  evaluate a?b:c operator
*
******************************************************************/
static G__value G__conditionaloperator(defined,expression,ig1,ebuf)
G__value defined;
char *expression;
int ig1;
char *ebuf;
{
  int tempop;
  int ppointer=0;
  int store_no_exec_compile;

  tempop = G__int(defined); /* eval condition */

  ++ig1;
  G__getstream(expression,&ig1,ebuf,":");

#ifdef G__ASM
  if(G__asm_noverflow) {
#ifdef G__ASM_DBG
    if(G__asm_dbg) G__fprinterr(G__serr,"%3x: CNDJMP assigned later\n",G__asm_cp);
#endif
    G__asm_inst[G__asm_cp]=G__CNDJMP;
    G__asm_inst[G__asm_cp+1] = G__PVOID;
    ppointer = G__asm_cp+1;
    G__inc_cp_asm(2,0);
  }
#endif /* G__ASM */

  /******************************************************************
  * if a?b:c true
  ******************************************************************/
  if(tempop) {
    defined=G__getexpr(ebuf); /* eval true case */
    G__getstream(expression,&ig1,ebuf,";");
#ifdef G__ASM
    if(G__asm_noverflow) {
#ifdef G__ASM_DBG
      if(G__asm_dbg) {
	G__fprinterr(G__serr,"%3x: JMP assigned later\n" ,G__asm_cp);
	G__fprinterr(G__serr,"  CNDJMP assigned %x?%x\n",G__asm_cp+2,ppointer-1);
      }
#endif
      G__asm_inst[G__asm_cp]=G__JMP;
      G__asm_inst[G__asm_cp+1] = G__PVOID;
      G__inc_cp_asm(2,0);
      G__asm_inst[ppointer]=G__asm_cp;
      ppointer=G__asm_cp-1;
      store_no_exec_compile=G__no_exec_compile;
      G__no_exec_compile=1;
      G__getexpr(ebuf); /* eval false case */
      G__no_exec_compile=store_no_exec_compile;
    }
#endif /* G__ASM */
  }

  /******************************************************************
  * if a?b:c false
  ******************************************************************/
  else {
#ifdef G__ASM
    if(G__asm_noverflow) {
      store_no_exec_compile=G__no_exec_compile;
      G__no_exec_compile=1;
      G__getexpr(ebuf); /* eval true case */
      G__no_exec_compile=store_no_exec_compile;
#ifdef G__ASM_DBG
      if(G__asm_dbg) {
	G__fprinterr(G__serr,"%3x: JMP assigned later\n",G__asm_cp);
	G__fprinterr(G__serr,"  CNDJMP assigned %x?%x\n",G__asm_cp+2,ppointer-1);
      }
#endif
      G__asm_inst[G__asm_cp]=G__JMP;
      G__asm_inst[G__asm_cp+1] = G__PVOID;
      G__inc_cp_asm(2,0);
      G__asm_inst[ppointer]=G__asm_cp;
      ppointer=G__asm_cp-1;
    }
#endif /* G__ASM */
    G__getstream(expression,&ig1,ebuf,";");
    defined=G__getexpr(ebuf); /* eval false case */
  }

  /******************************************************************
  * assign jump destination
  ******************************************************************/
#ifdef G__ASM
  if(G__asm_noverflow) {
#ifdef G__ASM_DBG
    if(G__asm_dbg) G__fprinterr(G__serr,"  JMP assigned %x,%x\n" ,G__asm_cp,ppointer-1);
#endif
    G__asm_inst[ppointer] = G__asm_cp;
    G__asm_cond_cp=G__asm_cp; /* avoid wrong optimization */
  }
#endif /* G__ASM */

  return(defined);
}

/******************************************************************
* G__iscastexpr_body()
*
******************************************************************/
int G__iscastexpr_body(ebuf,lenbuf)
char *ebuf;
int lenbuf;
{
#ifndef G__OLDIMPLEMENTATION1802
  int result;
  char *temp = (char*)malloc(strlen(ebuf)+1);
  if(!temp) {
    G__genericerror("Internal error: malloc, G__iscastexpr_body(), temp");
    return(0);
  }
#else
  char temp[G__LONGLINE];
#endif
  strcpy(temp,ebuf+1);
  temp[lenbuf-2]=0;
#ifndef G__OLDIMPLEMENTATION1802
  result = G__istypename(temp);
  free((void*)temp);
  return(result);
#else
  return (G__istypename(temp));
#endif
  /* using G__istypename() above is questionable. 
   * May need to use G__string2type() for better language compliance */
}

/******************************************************************
* G__iscastexpr()
*
******************************************************************/
#define G__iscastexpr(ebuf)                                              \
  (lenbuf>3 && '('==ebuf[0] && ')'==ebuf[lenbuf-1] &&                    \
   ('*'==ebuf[lenbuf-2] || '&'==ebuf[lenbuf-2] ||                        \
    G__iscastexpr_body(ebuf,lenbuf)))

/******************************************************************
* ANSI compliant operator precedences,  smaller the higher
******************************************************************/
#define G__PREC_SCOPE     1
#define G__PREC_FCALL     2

#define G__PREC_UNARY     3
#define G__PREC_P2MEM     4
#define G__PREC_PWR       5
#define G__PREC_MULT      6
#define G__PREC_ADD       7
#define G__PREC_SHIFT     8
#define G__PREC_RELATION  9
#define G__PREC_EQUAL    10
#define G__PREC_BITAND   11
#define G__PREC_BITEXOR  12
#define G__PREC_BITOR    13
#define G__PREC_LOGICAND 14
#define G__PREC_LOGICOR  15
#define G__PREC_TEST     16
#define G__PREC_ASSIGN   17

#define G__PREC_COMMA    18

#define G__PREC_NOOPR   100

/******************************************************************
* G__expr_error
*
******************************************************************/
#define G__expr_error                                                  \
  G__syntaxerror(expression);                                          \
  return(G__null)

/******************************************************************
* G__ASSIGN_CNDJMP
******************************************************************/
#ifdef G__ASM_DBG 
#define G__ASSIGN_CNDJMP                                             \
    if('O'==opr[op] && G__asm_noverflow) { /* 1575 begin */          \
      int store_pp_and = pp_and;                                     \
      while(pp_and) {                                                \
        if(G__asm_dbg)                                               \
          G__fprinterr(G__serr,"     CNDJMP assigned %x&%x\n",G__asm_cp,ppointer_and[pp_and-1]-1); \
        if(G__PVOID==G__asm_inst[ppointer_and[pp_and-1]]) /* 1575 */ \
        G__asm_inst[ppointer_and[--pp_and]] = G__asm_cp;             \
        else --pp_and; /* 1575 */ \
      }                                                              \
      pp_and = store_pp_and;                                         \
    } /* 1575 end */ 
#else
#define G__ASSIGN_CNDJMP                                             \
    if('O'==opr[op] && G__asm_noverflow) { /* 1575 begin */          \
      int store_pp_and = pp_and;                                     \
      while(pp_and) {                                                \
        if(G__PVOID==G__asm_inst[ppointer_and[pp_and-1]]) /* 1575 */ \
        G__asm_inst[ppointer_and[--pp_and]] = G__asm_cp;             \
        else --pp_and; /* 1575 */ \
      }                                                              \
      pp_and = store_pp_and;                                         \
    } /* 1575 end */ 
#endif


/******************************************************************
* G__exec_evalall
*
* Evaluate all operators in stack and get result as vstack[0]
* This macro contributes to execution speed. Don't implemented by
* a function.
******************************************************************/
#define G__exec_evalall                                                \
  /* Evaluate item */                                                  \
  if(lenbuf) {                                                         \
    ebuf[lenbuf] = '\0';                                               \
    vstack[sp++] = G__getitem(ebuf);                                   \
    lenbuf=0;                                                          \
    iscastexpr = 0; /* ON1342 */                                       \
  }                                                                    \
  /* process unary operator */                                         \
  while(up && sp>=1) {                                                 \
    --up;                                                              \
    if('*'==unaopr[up]) {                                              \
      vstack[sp-1] = G__tovalue(vstack[sp-1]);                         \
    }                                                                  \
    else if('&'==unaopr[up]) {    /* ON717 */                          \
      vstack[sp-1] = G__toXvalue(vstack[sp-1],'P');                    \
    }                                                                  \
    else {                                                             \
      vstack[sp] = vstack[sp-1];                                       \
      vstack[sp-1] = G__null;                                          \
      G__bstore(unaopr[up],vstack[sp],&vstack[sp-1]);                  \
    }                                                                  \
  }                                                                    \
  /* process binary operator */                                        \
  while(op /* && opr[op-1]<=G__PROC_NOOPR */ && sp>=2) {               \
    --op;                                                              \
    --sp;                                                              \
    G__ASSIGN_CNDJMP /* 1575 */                                        \
    G__bstore(opr[op],vstack[sp],&vstack[sp-1]);                       \
  }                                                                    \
  if(1!=sp || op!=0 || up!=0) { G__expr_error; }


/******************************************************************
* G__exec_binopr()
*
* Evaluate all operators in stack and get result as vstack[0]
* then push binary operator to operator stack.
* This macro contributes to execution speed. Don't implemented by
* a function.
******************************************************************/
#define G__exec_binopr(oprin,precin)                                   \
  /* evaluate left value */                                            \
  ebuf[lenbuf] = '\0';                                                 \
  vstack[sp++] = G__getitem(ebuf);                                     \
  lenbuf=0;                                                            \
  iscastexpr = 0; /* ON1342 */                                         \
  /* process unary operator */                                         \
  while(up && sp>=1) {                                                 \
    --up;                                                              \
    if('*'==unaopr[up]) {                                              \
      vstack[sp-1] = G__tovalue(vstack[sp-1]);                         \
    }                                                                  \
    else if('&'==unaopr[up]) {    /* ON717 */                          \
      vstack[sp-1] = G__toXvalue(vstack[sp-1],'P');                    \
    }                                                                  \
    else {                                                             \
      vstack[sp] = vstack[sp-1];                                       \
      vstack[sp-1] = G__null;                                          \
      G__bstore(unaopr[up],vstack[sp],&vstack[sp-1]);                  \
    }                                                                  \
  }                                                                    \
  /* process higher precedence operator at left */                     \
  while(op && prec[op-1]<=precin && sp>=2) {                           \
    --op;                                                              \
    --sp;                                                              \
    G__ASSIGN_CNDJMP /* 1575 */                                        \
    G__bstore(opr[op],vstack[sp],&vstack[sp-1]);                       \
  }                                                                    \
  /* set operator */                                                   \
  opr[op] = oprin;                                                     \
  if(G__PREC_NOOPR!=precin) prec[op++] = precin



/******************************************************************
* G__exec_unaopr()
* 
******************************************************************/
#define G__exec_unaopr(oprin)                                          \
  unaopr[up++] = oprin


/******************************************************************
* G__exec_oprassignopr()
*
******************************************************************/
#define G__exec_oprassignopr(oprin)                                    \
  G__exec_evalall;                                                     \
  vstack[1]=G__getexpr(expression+ig1+1);                              \
  G__bstore(oprin,vstack[1],&vstack[0]);                               \
  G__var_type='p';                                                     \
  return(vstack[0])


/******************************************************************
* G__wrap_binassignopr()
*
******************************************************************/
#define G__wrap_binassignopr(oprin,precin,assignopr)                  \
  if((nest==0)&&(single_quote==0)&&(double_quote==0)) {               \
    if(0==lenbuf) { G__expr_error; }                                  \
    if('='==expression[ig1+1]) {                                      \
      /* a@=b, a@=b */                                                \
      ++ig1;                                                          \
      G__exec_oprassignopr(assignopr);                                \
    }                                                                 \
    else {                                                            \
      /* a@b, a@b */                                                  \
      G__exec_binopr(c,precin);                                       \
    }                                                                 \
  }                                                                   \
  else ebuf[lenbuf++]=c

/******************************************************************
* G__wrap_plusminus()
*
******************************************************************/


#define G__wrap_plusminus(oprin,assignopr,preincopr,postincopr)       \
  if((nest==0)&&(single_quote==0)&&(double_quote==0)) {               \
    if(oprin==expression[ig1+1]                                       \
       && (!lenbuf||(!isdigit(ebuf[0])&&'.'!=ebuf[0]))  /* 1831 */    \
       ) {                                                            \
      if(lenbuf) {                                                    \
        if('='==expression[ig1+2] && 'v'==G__var_type) {              \
          /* *a++=expr */                                             \
          G__var_type='p';                                            \
          ebuf[lenbuf++]=c;                                           \
          ebuf[lenbuf++]=c;                                           \
          ++ig1;                                                      \
        }                                                             \
        else if(iscastexpr) { /* added ON1342 */                      \
          ebuf[lenbuf++]=c;                                           \
          ebuf[lenbuf++]=c;                                           \
          ++ig1;                                                      \
        }                                                             \
        else if(isalnum(expression[ig1+2])|| /* 2008 */               \
                '.'==expression[ig1+2]||'_'==expression[ig1+2]) {     \
	  /* a+ +b, a- -b */                                          \
	  ebuf[lenbuf]=0;                                             \
          ++ig1;                                                      \
          G__exec_binopr('+',G__PREC_ADD);                            \
        }                                                             \
        else {                                                        \
	  /* a++, a-- */                                              \
          ++ig1;                                                      \
          if('v'==G__var_type) {                                      \
            G__exec_unaopr('*');                                      \
            G__var_type = 'p';                                        \
          }                                                           \
          unaopr[up++] = postincopr;                                  \
          /* G__exec_binopr(0,G__PREC_NOOPR); */                      \
        }                                                             \
      }                                                               \
      else {                                                          \
        /* *++a = expr should be handled at assignment oprerator */   \
	/* ++a, --a */                                                \
        ++ig1;                                                        \
        if('v'==G__var_type) {                                        \
          G__exec_unaopr('*');                                        \
          G__var_type = 'p';                                          \
        }                                                             \
        G__exec_unaopr(preincopr);                                    \
      }                                                               \
    }                                                                 \
    else if('='==expression[ig1+1]) {                                 \
      /* +=, -= */                                                    \
      if(0==lenbuf) { G__expr_error; }                                \
      ++ig1;                                                          \
      G__exec_oprassignopr(assignopr);                                \
    }                                                                 \
    else if('>'==expression[ig1+1]) {                                 \
      /* a->b */                                                      \
      ++ig1;                                                          \
      ebuf[lenbuf++]=c;                                               \
      ebuf[lenbuf++]=expression[ig1];                                 \
    }                                                                 \
    else if(lenbuf) {                                                 \
      char *pebuf;                                                    \
      if('e'==tolower(expression[ig1-1])&&                            \
         'x'!=tolower(expression[1]) &&   /* 2201 */                  \
         (isdigit(ebuf[0])||'.'==ebuf[0]||                            \
         ('('==ebuf[0]&&(pebuf=strchr(ebuf,')'))&&                    \
          (isdigit(*++pebuf)||'.'==(*pebuf))))) {                     \
	/* 1e+10, 1e-10, (double)1e+6 */                              \
        ebuf[lenbuf++]=c;                                             \
      }                                                               \
      else {                                                          \
	ebuf[lenbuf]=0; /* ON742 */                                   \
	if(!G__iscastexpr(ebuf)) {                                    \
	  /* a+b, a-b */                                              \
          G__exec_binopr(c,G__PREC_ADD);                              \
	}                                                             \
	else {                                                        \
	  /* (int)-abc */                                             \
	  ebuf[lenbuf++]=c;                                           \
	}                                                             \
        /* G__exec_binopr(c,G__PREC_ADD); ON742 */                    \
      }                                                               \
    }                                                                 \
    else if('-'==c) {                                                 \
      /* -a */                                                        \
      G__exec_unaopr(oprin);                                          \
    }                                                                 \
    /* else +a , ignored */                                           \
  }                                                                   \
  else ebuf[lenbuf++]=c


/******************************************************************
* G__wrap_shifts()
*
******************************************************************/

#define G__wrap_shifts(oprin,assignopr,shiftopr,relationopr)          \
    if(oprin==expression[ig1+1]) {                                    \
      if('='==expression[ig1+2]) {                                    \
        /* a<<=b */                                                   \
        ig1+=2;                                                       \
        G__exec_oprassignopr(assignopr);                              \
      }                                                               \
      else {                                                          \
        /* a<<b */                                                    \
        ++ig1;                                                        \
        G__exec_binopr(shiftopr,G__PREC_SHIFT);                       \
      }                                                               \
    }                                                                 \
    else if('='==expression[ig1+1]) {                                 \
      /* a<=b */                                                      \
      ++ig1;                                                          \
      G__exec_binopr(relationopr,G__PREC_RELATION);                   \
    }                                                                 \
    else {                                                            \
      /* a<b */                                                       \
      G__exec_binopr(c,G__PREC_RELATION);                             \
    }                                                                 \
 

/******************************************************************
* && || operator handling
*
******************************************************************/
#ifdef G__ASM_DBG


#define G__SUSPEND_ANDOPR                                             \
        if('u'!=vstack[sp-1].type) {                                  \
	  store_no_exec_compile_and[pp_and] = G__no_exec_compile;     \
	  if(!G__no_exec_compile && 0==G__double(vstack[sp-1])) {     \
            if(G__asm_dbg) G__fprinterr(G__serr,"    G__no_exec_compile set\n"); \
            G__no_exec_compile = 1;                                   \
            vtmp_and = vstack[sp-1];                                  \
	  }                                                           \
	  if(G__asm_noverflow) {                                      \
	    if(G__asm_dbg) {                                          \
	      G__fprinterr(G__serr,"%3x: PUSHCPY\n",G__asm_cp);            \
	      G__fprinterr(G__serr,"%3x: CNDJMP assigned later\n",G__asm_cp+1); \
	    }                                                         \
	    G__asm_inst[G__asm_cp]=G__PUSHCPY;                        \
	    G__asm_inst[G__asm_cp+1]=G__CNDJMP;                       \
            G__asm_inst[G__asm_cp+2] = G__PVOID; /* 1575 */ \
	    ppointer_and[pp_and] = G__asm_cp+2;                       \
	    G__inc_cp_asm(3,0);                                       \
	  }                                                           \
          ++G__templevel; /* 1516 */ \
	  ++pp_and;                                                   \
        }

#define G__SUSPEND_OROPR                                              \
        if('u'!=vstack[sp-1].type) {                                  \
	  store_no_exec_compile_or[pp_or] = G__no_exec_compile;       \
	  if(!G__no_exec_compile && 0!=G__double(vstack[sp-1])) {     \
            if(G__asm_dbg) G__fprinterr(G__serr,"    G__no_exec_compile set\n"); \
            G__no_exec_compile = 1;                                   \
            vstack[sp-1] = G__one;                                    \
            vtmp_or = vstack[sp-1];                                   \
	  }                                                           \
	  if(G__asm_noverflow) {                                      \
	    if(G__asm_dbg) {                                          \
	      G__fprinterr(G__serr,"%3x: BOOL\n",G__asm_cp);               \
	      G__fprinterr(G__serr,"%3x: PUSHCPY\n",G__asm_cp+1);          \
	      G__fprinterr(G__serr,"%3x: CND1JMP assigned later\n",G__asm_cp+2); \
	    }                                                         \
	    G__asm_inst[G__asm_cp]=G__BOOL;                           \
	    G__asm_inst[G__asm_cp+1]=G__PUSHCPY;                      \
	    G__asm_inst[G__asm_cp+2]=G__CND1JMP;                      \
            G__asm_inst[G__asm_cp+3] = G__PVOID; /* 1575 */ \
	    ppointer_or[pp_or] = G__asm_cp+3;                         \
	    G__inc_cp_asm(4,0);                                       \
	  }                                                           \
          ++G__templevel; /* 1516 */ \
	  ++pp_or;                                                    \
        }

#define G__RESTORE_NOEXEC_ANDOPR                                      \
  if(pp_and) {                                                        \
    if(G__asm_dbg) G__fprinterr(G__serr,"    G__no_exec_compile reset %d\n"\
                     ,store_no_exec_compile_and[0]);                  \
    if(!store_no_exec_compile_and[0]&&G__no_exec_compile)             \
      vstack[sp-1] = vtmp_and;                                        \
    G__no_exec_compile = store_no_exec_compile_and[0];                \
  }

#define DBGCOM       \
  G__fprinterr(G__serr,"pp_and=%d  G__templevel=%d  G__p_tepbuf->level=%d G__decl=%d\n",pp_and,G__templevel,G__p_tempbuf->level,G__decl);

#define G__RESTORE_ANDOPR                                             \
  if(G__asm_noverflow) {                                              \
    while(pp_and) {                                                   \
      G__free_tempobject(); --G__templevel; /* 1516 */ \
      if(G__asm_dbg)                                                  \
          G__fprinterr(G__serr,"     CNDJMP assigned %x&%x\n",G__asm_cp,ppointer_and[pp_and-1]-1);     \
      if(G__PVOID==G__asm_inst[ppointer_and[pp_and-1]]) /* 1575 */ \
      G__asm_inst[ppointer_and[--pp_and]] = G__asm_cp;                \
      else --pp_and; /* 1575 */ \
    }                                                                 \
  } \
  else while(pp_and) {G__free_tempobject();--G__templevel; --pp_and;/*1524*/}

#define G__RESTORE_NOEXEC_OROPR                                       \
  if(pp_or) {                                                         \
    if(G__asm_dbg) G__fprinterr(G__serr,"    G__no_exec_compile reset %d\n"\
                     ,store_no_exec_compile_or[0]);                   \
    if(!store_no_exec_compile_or[0]&&G__no_exec_compile)              \
      vstack[sp-1] = vtmp_or;                                         \
    G__no_exec_compile = store_no_exec_compile_or[0];                 \
  }

#define G__RESTORE_OROPR                                              \
  if(G__asm_noverflow) {                                              \
    while(pp_or) {                                                    \
      G__free_tempobject(); --G__templevel; /* 1516 */ \
      if(G__asm_dbg)                                                  \
          G__fprinterr(G__serr,"     CND1JMP assigned %x|%x\n",G__asm_cp,ppointer_or[pp_or-1]-1);    \
      G__asm_inst[ppointer_or[--pp_or]] = G__asm_cp;                  \
    }                                                                 \
  } \
  else while(pp_or) {G__free_tempobject();--G__templevel; --pp_or;/*1524*/}

#else /* G__ASM_DBG */


#define G__SUSPEND_ANDOPR                                             \
        if('u'!=vstack[sp-1].type) {                                  \
	  store_no_exec_compile_and[pp_and] = G__no_exec_compile;     \
	  if(!G__no_exec_compile && 0==G__double(vstack[sp-1])) {     \
            G__no_exec_compile = 1;                                   \
            vtmp_and = vstack[sp-1];                                  \
	  }                                                           \
	  if(G__asm_noverflow) {                                      \
	    G__asm_inst[G__asm_cp]=G__PUSHCPY;                        \
	    G__asm_inst[G__asm_cp+1]=G__CNDJMP;                       \
            G__asm_inst[G__asm_cp+2] = G__PVOID; /* 1575 */ \
	    ppointer_and[pp_and] = G__asm_cp+2;                       \
	    G__inc_cp_asm(3,0);                                       \
	  }                                                           \
          ++G__templevel; /* 1516 */ \
	  ++pp_and;                                                   \
        }

#define G__SUSPEND_OROPR                                              \
        if('u'!=vstack[sp-1].type) {                                  \
	  store_no_exec_compile_or[pp_or] = G__no_exec_compile;       \
	  if(!G__no_exec_compile && 0!=G__double(vstack[sp-1])) {     \
            G__no_exec_compile = 1;                                   \
            vstack[sp-1] = G__one;                                    \
            vtmp_or = vstack[sp-1];                                   \
	  }                                                           \
	  if(G__asm_noverflow) {                                      \
	    G__asm_inst[G__asm_cp]=G__BOOL;                           \
	    G__asm_inst[G__asm_cp+1]=G__PUSHCPY;                      \
	    G__asm_inst[G__asm_cp+2]=G__CND1JMP;                      \
            G__asm_inst[G__asm_cp+3] = G__PVOID; /* 1575 */ \
	    ppointer_or[pp_or] = G__asm_cp+3;                         \
	    G__inc_cp_asm(4,0);                                       \
	  }                                                           \
          ++G__templevel; /* 1516 */ \
	  ++pp_or;                                                    \
        }

#define G__RESTORE_NOEXEC_ANDOPR                                      \
  if(pp_and) {                                                        \
    if(!store_no_exec_compile_and[0]&&G__no_exec_compile)             \
      vstack[sp-1] = vtmp_and;                                        \
    G__no_exec_compile = store_no_exec_compile_and[0];                \
  }

#define G__RESTORE_ANDOPR                                             \
  if(G__asm_noverflow) {                                              \
    while(pp_and) {                                                   \
      G__free_tempobject();--G__templevel; /* 1516 */ \
      if(G__PVOID==G__asm_inst[ppointer_and[pp_and-1]]) /* 1575 */ \
      G__asm_inst[ppointer_and[--pp_and]] = G__asm_cp;                \
      else --pp_and; /* 1575 */ \
    }                                                                 \
  } \
  else while(pp_and) {G__free_tempobject();--G__templevel; --pp_and;/*1524*/}

#define G__RESTORE_NOEXEC_OROPR                                       \
  if(pp_or) {                                                         \
    if(!store_no_exec_compile_or[0]&&G__no_exec_compile)              \
      vstack[sp-1] = vtmp_or;                                         \
    G__no_exec_compile = store_no_exec_compile_or[0];                 \
  }

#define G__RESTORE_OROPR                                              \
  if(G__asm_noverflow) {                                              \
    while(pp_or) {                                                    \
      G__free_tempobject();--G__templevel; /* 1516 */ \
      G__asm_inst[ppointer_or[--pp_or]] = G__asm_cp;                  \
    }                                                                 \
  } \
  else while(pp_or) {G__free_tempobject();--G__templevel; --pp_or;/*1524*/}

#endif /* G__ASM_DBG */


/******************************************************************
* G__value G__getexpr(expression)
*
* Grand entry for C/C++ expression evaluator. Space chars must be
* removed. 
*
******************************************************************/
#define G__STACKDEPTH 100 /* increased from 12 */
G__value G__getexpr(expression)
char *expression;
{
  G__value vstack[G__STACKDEPTH]; /* evaluated value stack */
  int sp=0;                       /* stack pointer */

  int opr[G__STACKDEPTH]; /* operator stack */
  int prec[G__STACKDEPTH];/* operator precedence */
  int op=0;               /* operator stack pointer */

  int unaopr[G__STACKDEPTH]; /* unary operator stack */
  int up=0;                    /* unary operator stack pointer */

#ifndef G__OLDIMPLEMENTATION1802
  char vv[G__BUFLEN];
  char *ebuf=vv;
#else
  char ebuf[G__LONGLINE]; /* evaluation buffer */
#endif
  int lenbuf=0;           /* ebuf pointer */
  int c; /* temp char */

  int ig1=0;  /* input expression pointer */
  int length; /* length of input expression */

  int nest=0; /* parenthesis nesting state variable */
  int single_quote=0,double_quote=0; /* quotation flags */

  int iscastexpr = 0; /* whether this expression start with a cast */

  G__value defined = G__null;
  int store_var_type = G__var_type;
  int explicitdtor = 0;
  int inew=0; /* ON994 */
  int pp_and = 0, pp_or = 0;
  int ppointer_and[G__STACKDEPTH], ppointer_or[G__STACKDEPTH];
  int store_no_exec_compile_and[G__STACKDEPTH];
  int store_no_exec_compile_or[G__STACKDEPTH];
  G__value vtmp_and,vtmp_or;

  /******************************************************************
  * return null for no expression
  ******************************************************************/
  length=strlen(expression);
  if(0==length) return(G__null);

  

#ifndef G__OLDIMPLEMENTATION1802
  if(strlen(expression)>G__BUFLEN-2) {
#ifndef G__OLDIMPLEMENTATION1863
    ebuf=(char*)malloc(strlen(expression)+2); /* VC++6.0 has problem. VC bug */
#else
    ebuf=(char*)malloc(strlen(expression)+1);
#endif
  }
  if(!ebuf) {
    G__genericerror("Internal error: malloc, G__getexpr(), ebuf");
    return(G__null);
  }
#endif

  /******************************************************************
  * operator expression
  ******************************************************************/
  for(ig1=0;ig1<length;ig1++) {
    c=expression[ig1];
    switch(c) {

    /***************************************************
    * quotation
    ****************************************************/
    case '"': 
      if(single_quote==0) double_quote ^= 1;
      ebuf[lenbuf++]=c;
      break;
    case '\'': 
      if(double_quote==0) single_quote ^= 1;
      ebuf[lenbuf++]=c;
      break;

    /***************************************************
    * parenthesis
    ****************************************************/
    case '(': /* new(arena) type(),  (type)val, (expr) */
      if((nest==0)&&(single_quote==0)&&(double_quote==0)&&
	 lenbuf==3&&strncmp(expression+inew,"new",3)==0) { /* ON994 */
#ifndef G__OLDIMPLEMENTATION1802
	if(vv!=ebuf) free((void*)ebuf);
#endif
	return(G__new_operator(expression+ig1));
      }
      /* no break here */
    case '[': 
    case '{': 
      if((double_quote==0)&&(single_quote==0)) {
	nest++;
	ebuf[lenbuf++]=c;
	inew=ig1+1;
      }
      else ebuf[lenbuf++]=c;
      break;

    case ')': 
    case ']': 
    case '}': 
      if((double_quote==0)&&(single_quote==0)) {
	nest--;
	ebuf[lenbuf++]=c;
	inew=ig1+1;
        if (!iscastexpr && '('==ebuf[0]) {
          ebuf[lenbuf] = '\0';
          iscastexpr = G__iscastexpr(ebuf);
        }
      }
      else ebuf[lenbuf++]=c;
      break;

    /***************************************************
    * operators
    ****************************************************/
    case ' ': /* new type, new (arena) type */
      if((nest==0)&&(single_quote==0)&&(double_quote==0)) {
	if(lenbuf-inew==3&&strncmp(expression+inew,"new",3)==0) { /* ON994 */
#ifndef G__OLDIMPLEMENTATION1802
	  if(vv!=ebuf) free((void*)ebuf);
#endif
	  return(G__new_operator(expression+ig1+1));
	}
	/* else ignore c, shoud not happen, but not sure */
	inew=ig1+1;
      }
      else ebuf[lenbuf++]=c;
      break;
    case '!': /* !a, a!=b */
      if((nest==0)&&(single_quote==0)&&(double_quote==0)) {
	if('='==expression[ig1+1]) {
	  /* a!=b */
	  ++ig1;
	  if(0==lenbuf) { G__expr_error; }
	  G__exec_binopr(G__OPR_NE,G__PREC_EQUAL);
	  break;
	}
      }
      /* no break here */
    case '~': /* ~a */
      if((nest==0)&&(single_quote==0)&&(double_quote==0)) {
        if(lenbuf) {
	  /* a->~b(), a::~b(), a.~b() */
	  explicitdtor=1;
	  ebuf[lenbuf++]=c; 
	}
	else {
	  /* ~a, !a */
	  G__exec_unaopr(c);
	}
      }
      else ebuf[lenbuf++]=c;
      break;
    case '/': /* a/b, a/=b */
      G__wrap_binassignopr(c,G__PREC_MULT,G__OPR_DIVASSIGN);
      break;
    case '%': /* a%b, a%=b */
      G__wrap_binassignopr(c,G__PREC_MULT,G__OPR_MODASSIGN);
      break;
    case '^': /* a^b, a^=b */
      G__wrap_binassignopr(c,G__PREC_BITEXOR,G__OPR_EXORASSIGN);
      break;
    case '+': /* ++a, a++, +a, a+b, a+=b, 1e+10, a+ +b */
      G__wrap_plusminus(c,G__OPR_ADDASSIGN,G__OPR_PREFIXINC,G__OPR_POSTFIXINC);
      break;
    case '-': /* --a, a--, -a, a-b, a-=b, 1e-10, a->b , a- -b */
      G__wrap_plusminus(c,G__OPR_SUBASSIGN,G__OPR_PREFIXDEC,G__OPR_POSTFIXDEC);
      break;
    case '<': /* a<<b, a<b, a<=b, a<<=b */
      if(nest==0&&single_quote==0&&double_quote==0&&explicitdtor==0) {
	ebuf[lenbuf]='\0'; 
	if(G__defined_templateclass(ebuf)) {
/* #define G__OLDIMPLEMENTATION790 */
	  ++ig1;
	  ebuf[lenbuf++] = c;
	  c=G__getstream_template(expression,&ig1,ebuf+lenbuf,">");
	  lenbuf=strlen(ebuf);
	  ebuf[lenbuf++] = c;
	  ebuf[lenbuf] = '\0';
	  --ig1;
	  /* try to instantiate the template */
	  (void)G__defined_tagname(ebuf,1);
	  lenbuf=strlen(ebuf);
	  break;
	}
#ifndef G__OLDIMPLEMENTATION1560
	else if(G__defined_templatefunc(ebuf)
		|| G__defined_templatememfunc(ebuf)
		) {
	  ++ig1;
	  ebuf[lenbuf++] = c;
	  c=G__getstream_template(expression,&ig1,ebuf+lenbuf,">");
	  if('>'==c) strcat(ebuf,">");
	  lenbuf = strlen(ebuf);
	  c=G__getstream_template(expression,&ig1,ebuf+lenbuf,"(");
	  if('('==c) strcat(ebuf,"(");
	  lenbuf = strlen(ebuf);
	  c=G__getstream_template(expression,&ig1,ebuf+lenbuf,")");
	  if(')'==c) strcat(ebuf,")");
	  lenbuf=strlen(ebuf);
	  --ig1;
	  break;
	}
#endif
	else if(strcmp(ebuf,"dynamic_cast")==0 ||
		strcmp(ebuf,"static_cast")==0 ||
		strcmp(ebuf,"reinterpret_cast")==0 ||
		strcmp(ebuf,"const_cast")==0) {
          /* TODO, implement casts, may need to introduce new instruction */
	  ++ig1;
          ebuf[0]='(';
	  c=G__getstream_template(expression,&ig1,ebuf+1,">");
	  lenbuf=strlen(ebuf);
	  ebuf[lenbuf++] = ')';
	  ebuf[lenbuf] = '\0';
	  --ig1;
	  break;
	}
	G__wrap_shifts(c,G__OPR_LSFTASSIGN,G__OPR_LSFT,G__OPR_LE)
      }
      else ebuf[lenbuf++]=c;
      break;
    case '>': /* a>>b, a>b, a>=b, a>>=b */
      if(nest==0&&single_quote==0&&double_quote==0&&explicitdtor==0) {
	G__wrap_shifts(c,G__OPR_RSFTASSIGN,G__OPR_RSFT,G__OPR_GE)
      }
      else ebuf[lenbuf++]=c;
      break;

    case '@': /* a@b */
      if((nest==0)&&(single_quote==0)&&(double_quote==0)) {
	if(0==lenbuf) { G__expr_error; }
	G__exec_binopr(c,G__PREC_PWR);
      }
      else ebuf[lenbuf++]=c;
      break;
    case '*': /* *a, a*b, a*=b, a**b, **a */
      if((nest==0)&&(single_quote==0)&&(double_quote==0)) {
	if('='==expression[ig1+1]) {
	  /* a*=b */
	  ++ig1;
	  G__exec_oprassignopr(G__OPR_MULASSIGN);
	}
	else if(c==expression[ig1+1]) {
	  if(lenbuf) {
#ifndef G__NOPOWEROPR
	    /* a**b handle as power operator */
	    ++ig1;
	    G__exec_binopr('@',G__PREC_PWR);
#else
	    /* a**b handle as a*(*b) */
	    G__exec_binopr('*',G__PREC_MULT);
	    G__exec_unaopr('*');
	    ++ig1;
#endif
	  }
	  else {
	    /* **a */
	    ++ig1;
	    G__exec_unaopr(c);
	    /* it is questionable whether to change following to 
	     * G__exec_unaopr(c); */
	     G__exec_unaopr(c);
	  }
	}
	else if(lenbuf) {
	  ebuf[lenbuf]=0;
#define G__OLDIMPLEMENTATION792 /* BAD CHANGE */
	  if(!G__iscastexpr(ebuf)) {
	    /* a*b */
	    G__exec_binopr(c,G__PREC_MULT);
	  }
	  else {
	    /* (int)*abc */
	    ebuf[lenbuf++]=c;
	  }
	}
	else {
	  /* *a */
	  /* it is questionable whether to change following to 
	   * G__exec_unaopr(c); */
#define G__OLDIMPLEMENTATION1619
	   G__exec_unaopr(c);
	}
      }
      else ebuf[lenbuf++]=c;
      break;
    case '&': /* &a, a&b, a&&b, a&=b */
      if((nest==0)&&(single_quote==0)&&(double_quote==0)) {
	if(c==expression[ig1+1]) {
	  /* a&&b */
	  ++ig1;
	  G__exec_binopr('A',G__PREC_LOGICAND);
	  G__SUSPEND_ANDOPR;
	}
	else if('='==expression[ig1+1]) {
	  /* a&=b */
	  ++ig1;
	  G__exec_oprassignopr(G__OPR_BANDASSIGN);
	}
	else if(lenbuf) {
	  ebuf[lenbuf]=0;
	  if(!G__iscastexpr(ebuf)) {
	    /* a&b */
	    G__exec_binopr(c,G__PREC_BITAND);
	  }
	  else {
	    /* (int*)&abc */
	    ebuf[lenbuf++]=c;
	  }
	}
	else {
	  /* &a */
	  G__exec_unaopr(c); /* ON717 */
	}
      }
      else ebuf[lenbuf++]=c;
      break;
    case '|': /* a|b, a||b, a|=b */
      if((nest==0)&&(single_quote==0)&&(double_quote==0)) {
	if(c==expression[ig1+1]) {
	  /* a||b */
	  ++ig1;
	  G__exec_binopr('O',G__PREC_LOGICOR);
          G__RESTORE_NOEXEC_ANDOPR
          G__RESTORE_ANDOPR
	  G__SUSPEND_OROPR;
	}
	else if('='==expression[ig1+1]) {
	  /* a|=b */
	  ++ig1;
	  G__exec_oprassignopr(G__OPR_BORASSIGN);
	}
	else if(lenbuf) {
	  /* a&b */
	  G__exec_binopr(c,G__PREC_BITOR);
	}
	else {
	  /* &a */
	  G__exec_unaopr(c);
	}
      }
      else ebuf[lenbuf++]=c;
      break;

    /***************************************************
    * lowest precedence, a=b and a?b:c
    ****************************************************/
    case '=': /* a==b, a=b */
      if((nest==0)&&(single_quote==0)&&(double_quote==0)) {
	if(c==expression[ig1+1]) {
	  /* a==b */
	  ++ig1;
	  G__exec_binopr(G__OPR_EQ,G__PREC_EQUAL);
	}
	else {
	  /* a=b */
	  G__var_type='p';
	  defined=G__getexpr(expression+ig1+1);
	  strncpy(ebuf,expression,ig1);
	  ebuf[ig1] = '\0';
	  G__var_type=store_var_type;
#ifndef G__OLDIMPLEMENTATION1802
	  vstack[0]= G__letvariable(ebuf,defined,&G__global,G__p_local);
	  if(vv!=ebuf) free((void*)ebuf);
	  return(vstack[0]);
#else
	  return(G__letvariable(ebuf,defined,&G__global,G__p_local));
#endif
	}
	inew=ig1+1;
      }
      else ebuf[lenbuf++]=c;
      break;
    case '?': /* a?b:c */
      if((nest==0)&&(single_quote==0)&&(double_quote==0)) {
	G__exec_evalall;
	G__RESTORE_NOEXEC_ANDOPR
        G__RESTORE_NOEXEC_OROPR
	G__RESTORE_ANDOPR
	G__RESTORE_OROPR
#ifndef G__OLDIMPLEMENTATION1802
        vstack[1]=G__conditionaloperator(vstack[0],expression,ig1,ebuf);
	if(vv!=ebuf) free((void*)ebuf);
	return(vstack[1]);
#else
	return(G__conditionaloperator(vstack[0],expression,ig1,ebuf));
#endif
      }
      else ebuf[lenbuf++]=c;
      break;

    case '\\' : 
      ebuf[lenbuf++]=c;
      ebuf[lenbuf++]=expression[++ig1];
      break;

    /***************************************************
    * non-operator characters
    ****************************************************/
    default:
      ebuf[lenbuf++]=c;
      break;
    }
  }

  /******************************************************************
  * Evaluate operators in stack
  ******************************************************************/
  G__exec_evalall;

  G__RESTORE_NOEXEC_ANDOPR
  G__RESTORE_NOEXEC_OROPR
  G__RESTORE_ANDOPR
  G__RESTORE_OROPR

#ifndef G__OLDIMPLEMENTATION1802
  if(vv!=ebuf) free((void*)ebuf);
#endif
  return(vstack[0]);
}


/******************************************************************
* G__value G__getprod(expression1)
*
* Called by
*    G__getexpr()    at operator
*    G__getexpr()    at the end
*
******************************************************************/
G__value G__getprod(expression1)
char *expression1;
{
  G__value defined1,reg;
  char ebuf1[G__ONELINE];
  int operator1,prodpower=0;
  int lenbuf1=0;
  int ig11,ig2;
  int length1;
  int nest1=0;
  int single_quote=0,double_quote=0;
  
  
  operator1='\0';
  defined1=G__null;
  length1=strlen(expression1);
  if(length1==0) return(G__null);
  
  switch(expression1[0]) {
  case '*': /* value of pointer */
    if(expression1[1]=='(') {
      reg=G__getexpr(expression1+1);
      defined1=G__tovalue(reg);
      return(defined1);
    }
    G__var_type='v';
    for(ig2=0;ig2<length1;ig2++) expression1[ig2]=expression1[ig2+1];
    break;
    default :
      break;
  }
  
  for(ig11=0;ig11<length1;ig11++) {
    switch(expression1[ig11]) {
    case '"' : /* double quote */
      if(single_quote==0) {
	double_quote ^= 1;
      }
      ebuf1[lenbuf1++]=expression1[ig11];
      break;
    case '\'' : /* single quote */
      if(double_quote==0) {
	single_quote ^= 1;
      }
      ebuf1[lenbuf1++]=expression1[ig11];
      break;
    case '*':
      if(strncmp(expression1,"new ",4)==0) { 
	ebuf1[lenbuf1++]=expression1[ig11];
	break;
      }
    case '/':
    case '%':
      if((nest1==0)&&(single_quote==0)&&(double_quote==0)) {
	switch(lenbuf1) {
	case 0:
	  operator1=G__getoperator(operator1 ,expression1[ig11]);
	  break;
	default:
	  if(operator1=='\0') operator1='*';
	  ebuf1[lenbuf1]='\0';
	  reg=G__getpower(ebuf1);
	  G__bstore(operator1,reg,&defined1);
	  lenbuf1=0;
	  ebuf1[0]='\0';
	  operator1=expression1[ig11];
	  break;
	}
      }
      else {
	ebuf1[lenbuf1++]=expression1[ig11];
      }
      break;
    case '(':
    case '[':
    case '{':
      if((double_quote==0)&&(single_quote==0)) {
	nest1++;
	ebuf1[lenbuf1++]=expression1[ig11];
      }
      else {
	ebuf1[lenbuf1++]=expression1[ig11];
      }
      break;
    case ')':
    case ']':
    case '}':
      if((double_quote==0)&&(single_quote==0)) {
	ebuf1[lenbuf1++]=expression1[ig11];
	nest1--;
      }
      else {
	ebuf1[lenbuf1++]=expression1[ig11];
      }
      break;
    case '@':
    case '~':
    case ' ':
      if((nest1==0)&&(single_quote==0)&&(double_quote==0)) {
	prodpower=1;
      }
      ebuf1[lenbuf1++]=expression1[ig11];
      break;
      
      
    case '\\' : 
      ebuf1[lenbuf1++]=expression1[ig11++];
      ebuf1[lenbuf1++]=expression1[ig11];
      break;
      
    default:
      ebuf1[lenbuf1++]=expression1[ig11];
      break;
    }
  }
  ebuf1[lenbuf1]='\0';
  if((nest1!=0)||(single_quote!=0)||(double_quote!=0)) {
    G__parenthesiserror(expression1,"G__getprod");
    return(G__null);
  }
  if(prodpower!=0) {
    reg=G__getpower(ebuf1);
  }
  else {
    reg=G__getitem(ebuf1);
  }
  G__bstore(operator1,reg,&defined1);
  return(defined1);
}


/******************************************************************
* G__value G__getpower(expression2)
*
* Called by
*    G__getprod()    at operator
*    G__getprod()    at the end
*
******************************************************************/
G__value G__getpower(expression2)
char *expression2;
{
  G__value defined2,reg;
  char ebuf2[G__ONELINE];
  int operator2 /*,c */;
  int lenbuf2=0;
  int ig12;
  /* int length2; */
  int nest2=0;
  int single_quote=0,double_quote=0;
  
  if(expression2[0]=='\0') return(G__null);
  
  operator2='\0';
  defined2=G__null;
  /* length2=strlen(expression2); */
  /* if(length2==0) return(G__null); */
  /* for(ig12=0;ig12<length2;ig12++) { */
  
  ig12=0;
  /* while((c=expression2[ig12])!='\0') { */
  while(expression2[ig12]!='\0') {
    switch(expression2[ig12]) {
    case '"' : /* double quote */
      if(single_quote==0) {
	double_quote ^= 1;
      }
      ebuf2[lenbuf2++]=expression2[ig12];
      break;
    case '\'' : /* single quote */
      if(double_quote==0) {
	single_quote ^= 1;
      }
      ebuf2[lenbuf2++]=expression2[ig12];
      break;
    case '~': /* 1's complement */
      /* explicit destructor handled in G__getexpr(), just go through here */
    case '@':
      if((nest2==0)&&(single_quote==0)&&(double_quote==0)) {
	switch(lenbuf2) {
	case 0:
	  operator2=G__getoperator(operator2
				   ,expression2[ig12]);
	  break;
	default:
	  ebuf2[lenbuf2]='\0';
	  reg=G__getitem(ebuf2);
	  G__bstore(operator2,reg,&defined2);
	  lenbuf2=0;
	  ebuf2[0]='\0';
	  operator2=expression2[ig12];
	  break;
	}
      }
      else {
	ebuf2[lenbuf2++]=expression2[ig12];
      }
      break;
    case ' ':
      if((nest2==0)&&(single_quote==0)&&(double_quote==0)&&
	 (strncmp(expression2,"new",3)==0)) {
	return(G__new_operator(expression2+ig12+1));
      }
      else {
	G__fprinterr(G__serr,"Error: G__power() expression %s ",expression2 );
	G__genericerror((char*)NULL);
	return(G__null);
      }
      /* break; */
    case '(':
    case '[':
    case '{':
      if((double_quote==0)&&(single_quote==0)) {
	nest2++;
	ebuf2[lenbuf2++]=expression2[ig12];
      }
      else {
	ebuf2[lenbuf2++]=expression2[ig12];
      }
      break;
    case ')':
    case ']':
    case '}':
      if((double_quote==0)&&(single_quote==0)) {
	ebuf2[lenbuf2++]=expression2[ig12];
	nest2--;
      }
      else {
	ebuf2[lenbuf2++]=expression2[ig12];
      }
      break;
      
    case '\\' :
      ebuf2[lenbuf2++]=expression2[ig12++];
      ebuf2[lenbuf2++]=expression2[ig12];
      break;
      
      default :
	ebuf2[lenbuf2++]=expression2[ig12];
      break;
    }
    ig12++;
  }
  ebuf2[lenbuf2]='\0';
  if((nest2!=0)||(single_quote!=0)||(double_quote!=0)) {
    G__parenthesiserror(expression2,"G__getpower");
    return(G__null);
  }
  reg=G__getitem(ebuf2);
  G__bstore(operator2,reg,&defined2);
  return(defined2);
}

#ifdef G__PTR2MEMFUNC
/******************************************************************
* G__getpointer2memberfunc
******************************************************************/
int G__getpointer2memberfunc(item,presult)
char *item;
G__value *presult;
{
  int hash=0;
  long scope_struct_offset=0;
  int scope_tagnum = -1;
  int ifn;
  struct G__ifunc_table *memfunc;
  char *p = strstr(item,"::");

  if(!p) return(0);

  G__scopeoperator(item,&hash,&scope_struct_offset,&scope_tagnum);
  if(scope_tagnum<0 || scope_tagnum>=G__struct.alltag) return(0);

  G__incsetup_memfunc(scope_tagnum);
  memfunc=G__struct.memfunc[scope_tagnum];

  while(memfunc) {
    for(ifn=0;ifn<memfunc->allifunc;ifn++) {
      if(strcmp(item,memfunc->funcname[ifn])==0) {
#ifndef G__OLDIMPLEMENTATION1289
	/* For the time being, pointer to member function can only be handled
	 * as function name */
#ifndef G__OLDIMPLEMENTATION1993
	if(('n'==G__struct.type[scope_tagnum] || memfunc->staticalloc[ifn])
	   && memfunc->pentry[ifn]->size<0 
	   && memfunc->pentry[ifn]->tp2f) {
	  G__letint(presult,'Y',(long)memfunc->pentry[ifn]->tp2f);
	}
	else {
	  G__letint(presult,'C',(long)memfunc->funcname[ifn]);
	}
#else
	G__letint(presult,'C',(long)memfunc->funcname[ifn]);
	/* 
	if(
	   memfunc->pentry[ifn]->size>=0
          ) 
	  G__letint(presult,'C',(long)memfunc->funcname[ifn]);
	else 
	  G__letint(presult,'Y',(long)memfunc->pentry[ifn]->tp2f);
	*/
#endif
#else
	G__letint(presult,'C',(long)memfunc->funcname[ifn]);
#endif
	presult->tagnum = -1;
	presult->typenum= -1;
	presult->ref=0;
	return(1);
      }
    }
    memfunc=memfunc->next;
  }
  return(0);
}
#endif

#define G__OLDIMPLEMENTATION1779

/******************************************************************
* G__value G__getitem(item)
*
* Called by
*    G__getexpr()
*    G__getexpr()
*    G__getexpr()
*    G__getexpr()
*    G__getprod()
*    G__getpower()
*    G__getpower()
*    G__getfunction()
*
******************************************************************/
G__value G__getitem(item)
char *item;
{
  int known;
  G__value result3;
  int c;
  char store_var_typeB;
  G__value reg;
  /* char name[G__MAXNAME], *p; */ /* to handle $xxx.yyy $xxx->yyy */
  /* to prevent recursive calling of G__GetSpecialObject() */

  
  
  switch(item[0]) {
    /* constant */
  case '0':
    c=item[1];
    if((c!='\0')&&(c!='.')&&
       ((c=tolower(c))!='f')&&(c!='e')&&
       (c!='l')&&(c!='u')&&(c!='s')) {
      result3=G__checkBase(item,&known);
#ifdef G__ASM
      if(G__asm_noverflow) {
	/**************************************
	 * G__LD instruction
	 * 0 LD
	 * 1 address in data stack
	 * put result3
	 **************************************/
#ifdef G__ASM_DBG
	if(G__asm_dbg) G__fprinterr(G__serr,"%3x: LD %ld from %x\n"
			       ,G__asm_cp,G__int(result3)
			       ,G__asm_dt);
#endif
	G__asm_inst[G__asm_cp]=G__LD;
	G__asm_inst[G__asm_cp+1]=G__asm_dt;
	G__asm_stack[G__asm_dt]=result3;
	G__inc_cp_asm(2,1);
      }
#endif
      result3.tagnum = -1;
      result3.typenum = -1;
      result3.ref = 0;
      result3.isconst = G__CONSTVAR + G__STATICCONST;
      return(result3);
    }
  case '1':
  case '2':
  case '3':
  case '4':
  case '5':
  case '6':
  case '7':
  case '8':
  case '9':
  case '.':
    if(G__isfloat(item,&c)) {
      G__letdouble(&result3,c,atof(item));
      /* G__letdouble(&result3,c,G__atodouble(item)); */
    }
    else {
		//todo
      unsigned long long xxx;
       if('u'==c) { /* long long */
          c='n';
          G__letLonglong(&result3,c,G__expr_strtoll(item,NULL,10));
       } else if('t'==c) {
          c='m';
          G__letULonglong(&result3,c,G__expr_strtoull(item,NULL,10));
       } else {
		//vs2012�ȍ~
		  // xxx = strtoull(item, NULL, 10);
		   //vs2010
		   xxx = _strtoui64(item, NULL, 10);
 	 if(xxx>LONG_MAX && ('i'==c||'l'==c) ) --c;
 	 if(xxx==ULONG_MAX) {
 	   char ulongmax[100];
 	   int ulonglen = strlen(item);
 	   sprintf(ulongmax,"%lu",ULONG_MAX);
 	   while('u'==tolower(item[ulonglen-1])||'l'==tolower(item[ulonglen-1]))
 	     item[--ulonglen]=0;
 	   if(strcmp(ulongmax,item)!=0) 
 	     G__genericerror("Error: integer literal too large, add LL or ULL for long long integer");
 	 } 
          G__letint(&result3,c,xxx);
          result3.obj.i=xxx;
       }
    }
#if  !defined(G__OLDIMPLEMENTATION1874)
    if('u'!=c) {
      result3.tagnum = -1;
      result3.typenum = -1;
      result3.ref = 0;
    }
#else
    result3.tagnum = -1;
    result3.typenum = -1;
    result3.ref = 0;
#endif
    result3.isconst = G__CONSTVAR + G__STATICCONST;
    
#ifdef G__ASM
    if(G__asm_noverflow) {
      /**************************************
       * G__LD instruction
       * 0 LD
       * 1 address in data stack
       * put result3
       **************************************/
#ifdef G__ASM_DBG
      if(G__asm_dbg) G__fprinterr(G__serr,"%3x: LD %g from %x\n"
			     ,G__asm_cp,G__double(result3)
			     ,G__asm_dt);
#endif
      G__asm_inst[G__asm_cp]=G__LD;
      G__asm_inst[G__asm_cp+1]=G__asm_dt;
      G__asm_stack[G__asm_dt]=result3;
      G__inc_cp_asm(2,1);
    }
#endif
    
    break;
  case '\'':
    result3=G__strip_singlequotation(item);
    result3.tagnum = -1;
    result3.typenum = -1;
    result3.ref = 0;
#ifndef G__OLDIMPLEMENTATION1259
    result3.isconst = G__CONSTVAR;
#endif
    
#ifdef G__ASM
    if(G__asm_noverflow) {
      /**************************************
       * G__LD instruction
       * 0 LD
       * 1 address in data stack
       * put result3
       **************************************/
#ifdef G__ASM_DBG
      if(G__asm_dbg) G__fprinterr(G__serr,"%3x: LD '%c' from %x\n"
			     ,G__asm_cp,(char)G__int(result3)
			     ,G__asm_dt);
#endif
      G__asm_inst[G__asm_cp]=G__LD;
      G__asm_inst[G__asm_cp+1]=G__asm_dt;
      G__asm_stack[G__asm_dt]=result3;
      G__inc_cp_asm(2,1);
    }
#endif
    
    break;
    
  case '"': 
    result3=G__strip_quotation(item);
    result3.tagnum = -1;
    result3.typenum = -1;
    result3.ref = 0;
#ifndef G__OLDIMPLEMENTATION1259
    result3.isconst = G__CONSTVAR;
#endif
    
#ifdef G__ASM
    if(G__asm_noverflow) G__asm_gen_strip_quotation(&result3);
#endif /* G__ASM */
    return(result3);

  case '-':
    reg = G__getitem(item+1);
    result3 = G__null;
    G__bstore('-',reg,&result3);
    return(result3);

#ifndef G__OLDIMPLEMENTATION1825
  case '_':
    if('$'==item[1]) {
      G__getiparseobject(&result3,item);
      return(result3);
    }
#endif
    
  default:
    store_var_typeB = G__var_type;
    known=0;
    G__var_type = 'p';
    /* variable */
    result3=G__getvariable(item,&known,&G__global,G__p_local);
    /* function */
    if(known==0) {
      G__var_typeB = store_var_typeB;
      result3=G__getfunction(item,&known,G__TRYNORMAL);
      if(known) {
	result3 = G__toXvalue(result3,store_var_typeB);
	if(G__initval_eval) G__dynconst=G__DYNCONST;
      }
      G__var_typeB = 'p';
    }
#ifdef G__PTR2MEMFUNC
    if(known==0&&result3.obj.i==0) {
      known=G__getpointer2memberfunc(item,&result3);
    }
#endif
    /* undefined */
    if(known==0) {
      if(strncmp(item,"__",2)==0) {
	result3=G__getreserved(item+1,(void**)NULL,(void**)NULL);
        if(result3.type) known = 1;
      } else
      if(
#ifdef G__ROOT
	 G__dispmsg < G__DISPROOTSTRICT &&
#endif
	 G__GetSpecialObject && G__GetSpecialObject != G__getreserved) {
	/* append $ to object and try to find it again */
	if (!G__gettingspecial && item[0] != '$') {
#ifndef G__OLDIMPLEMENTATION1802
	  char *sbuf;
#else
	  char sbuf[G__LONGLINE];
#endif
	  int store_return = G__return;
	  int store_security_error = G__security_error;
	  /* This fix should be verified very carefully */
	  if(G__no_exec_compile && G__asm_noverflow) G__abortbytecode();
#ifndef G__OLDIMPLEMENTATION1802
	  sbuf = (char*)malloc(strlen(item)+2);
	  if(!sbuf) {
	    G__genericerror("Internal error: malloc in G__getitem(),sbuf");
	    return(G__null);
	  }
#endif
	  sprintf(sbuf, "$%s", item);
	  G__gettingspecial = 1;
          G__var_type = store_var_typeB; /* BUG FIX ROOT Special object */
	  result3 = G__getitem(sbuf);
#ifndef G__OLDIMPLEMENTATION1802
	  free((void*)sbuf);
#endif
	  G__gettingspecial = 0;
	  if(G__const_noerror) {
	    G__return = store_return;
	    G__security_error = store_security_error;
	  }
	  return result3;
	}
      }
      if (known == 0 && result3.obj.i == 0) {
	result3=G__null;
	if(G__noerr_defined==0) {
	  
	  if(G__definemacro==0) {
	    char *pxx;
	    G__warnundefined(item);
	    pxx = strstr(item,"::");
	    if(!pxx || pxx==item) {
	      pxx = strchr(item,'(');
	      if(pxx) {
		*pxx = 0;
		if((0==G__const_noerror&&!G__asm_wholefunction)
		   && (!G__no_exec_compile || G__asm_noverflow)
		   ) {
		  G__fprinterr(G__serr,"Possible candidates are...\n");
		  if('$'==item[0]) G__display_proto(G__serr,item+1);
		  else             G__display_proto(G__serr,item);
		}
		*pxx = '(';
	      }
	    }
	    result3=G__interactivereturn();
	  }
	  else {
	    /*
	    G__genericerror("Limitation: This form of macro may not be expanded. Use +P or -p option");
	    */
	    return(G__null);
	  }
	}
      }
    }
  }
  
  return(result3);
}



/***********************************************************************
* G__getoperator(char *newoperator,*oldoperator)  
*
* Called by
*   G__getexpr()
*   G__getprod()
*   G__getpower()
*
***********************************************************************/

int G__getoperator(newoperator,oldoperator)
int newoperator,oldoperator;
{
  switch(newoperator) {
  case '+':
    switch(oldoperator) {
    case '+':
      return('I');
    case '-':
      return('-');
    case '~':
      return(G__UNARYOP);
    case '=':
      return(G__OPR_ADDASSIGN);
    default:
      return(oldoperator);
    }
    /* break; */
  case '-':
    switch(oldoperator) {
    case '+':
      return('-');
    case '-':
      return('D');
    case '~':
      return(G__UNARYOP);
    case '=':
      return(G__OPR_SUBASSIGN);
    default:
      return(oldoperator);
    }
    /* break; */
  case '>':
    switch(oldoperator) {
    case '>':
      return('R'); /* right shift */
    case '=':
      return('G'); /* greater or equal */
    case '~':
    case '+':
    case '-':
      return(G__UNARYOP);
    default:
      return(oldoperator);
    }
    /* break; */
  case 'R': /* right shift */
    switch(oldoperator) {
    case '~':
    case '+':
    case '-':
      return(G__UNARYOP);
    case '=':
      return(G__OPR_RSFTASSIGN);
    default:
      return(oldoperator);
    }
    /* break; */
  case '<':
    switch(oldoperator) {
    case '<':
      return('L'); /* left shift */
    case '=':
      return('l'); /* less or equal */
    case '~':
    case '+':
    case '-':
      return(G__UNARYOP);
    default:
      return(oldoperator);
    }
    /* break; */
  case 'L':
    switch(oldoperator) {
    case '~':
    case '+':
    case '-':
      return(G__UNARYOP);
    case '=':
      return(G__OPR_LSFTASSIGN);
    default:
      return(oldoperator);
    }
    /* break; */
  case '&':
    switch(oldoperator) {
    case '&':
      return('A');
    case '~':
    case '+':
    case '-':
      return(G__UNARYOP);
    case '=':
      return(G__OPR_BANDASSIGN);
    default:
      return(oldoperator);
    }
    /* break; */
  case '|':
    switch(oldoperator) {
    case '|':
      return('O');
    case '~':
    case '+':
    case '-':
      return(G__UNARYOP);
    case '=':
      return(G__OPR_BORASSIGN);
    default:
      return(oldoperator);
    }
    /* break; */
  case '^':
    switch(oldoperator) {
    case '~':
    case '+':
    case '-':
      return(G__UNARYOP);
    case '=':
      return(G__OPR_EXORASSIGN);
    default:
      return(oldoperator);
    }
    /* break; */
  case '%':
    switch(oldoperator) {
    case '~':
    case '+':
    case '-':
      return(G__UNARYOP);
    case '=':
      return(G__OPR_MODASSIGN);
    default:
      return(oldoperator);
    }
    /* break; */
  case '*':
    switch(oldoperator) {
    case '/':
      return('/');
    case '*':
      return('@');
    case '~':
    case '+':
    case '-':
      return(G__UNARYOP);
    case '=':
      return(G__OPR_MULASSIGN);
    default:
      return(newoperator);
    }
    /* break; */
  case '/':
    switch(oldoperator) {
    case '/':
      return('*');
    case '*':
      return('/');
    case '~':
    case '+':
    case '-':
      return(G__UNARYOP);
    case '=':
      return(G__OPR_DIVASSIGN);
    default:
      return(newoperator);
    }
    /* break; */
  case '=':
    switch(oldoperator) {
    case '=':
      return('E');
    default:
      return(newoperator);
    }
    /* break; */
  case '!':
    switch(oldoperator) {
    case '=':
      return('N');
    default:
      return(newoperator);
    }
    /* break; */
  case 'E':
  case 'N':
  case 'G':
  case 'l':
    switch(oldoperator) {
    case '~':
    case '+':
    case '-':
      return(G__UNARYOP);
    default:
      return(newoperator);
    }
  case 'A':
    switch(oldoperator) {
    case '=':
      return(G__OPR_ANDASSIGN);
    default:
      return(newoperator);
    }
  case 'O':
    switch(oldoperator) {
    case '=':
      return(G__OPR_ORASSIGN);
    default:
      return(newoperator);
    }
  }
  return(oldoperator);
}


/***********************************************************************
* int *G__test(char *expression2)
***********************************************************************/
int G__test(expression2)
char *expression2;
{
  G__value result;
  result=G__getexpr(expression2);
  if('u'==result.type) return(G__iosrdstate(&result));
  if('f'==result.type||'d'==result.type) {
    /*
    printf("\n!!! %s type=%c  d=%g i=%ld",expression2
       ,result.type,result.obj.d,result.obj.i); 
     G__printlinenum();
    */
    return(0.0!=result.obj.d);
  }
  else {
    return(result.obj.i);
  }
   
}




/******************************************************************
* int G__btest(operator2,lresult,rresult)
*
* Called by
*   G__exec_asm()
*   G__test()
*   G__test()
*   G__test()
*
******************************************************************/
int G__btest(operator2,lresult,rresult)
int operator2;
G__value lresult,rresult;
{

  if(lresult.type=='u' || rresult.type=='u') {
    G__overloadopr(operator2,rresult,&lresult);
    return(G__int(lresult));
  }
  else if(lresult.type=='U' || rresult.type=='U') {
    G__publicinheritance(&lresult,&rresult);
  }
  
#ifdef G__ASM
  if(G__asm_noverflow) {
#ifdef G__ASM_DBG
    if(G__asm_dbg) G__fprinterr(G__serr,"%3x: CMP2  '%c'\n" ,G__asm_cp,operator2);
#endif
    G__asm_inst[G__asm_cp]=G__CMP2;
    G__asm_inst[G__asm_cp+1]=(long)operator2;
    G__inc_cp_asm(2,0);
  }
#endif

  if(G__no_exec_compile || G__no_exec) return(1); /* avoid Alpha crash */
  
  switch(operator2) {
  case 'E': /* == */
    if(G__double(lresult) ==G__double(rresult)) return(1);
    else return(0);
    /* break; */
  case 'N': /* != */
    if(G__double(lresult) !=G__double(rresult)) return(1);
    else return(0);
    /* break; */
  case 'G': /* >= */
    if(G__double(lresult) >=G__double(rresult)) return(1);
    else return(0);
    /* break; */
  case 'l': /* <= */
    if(G__double(lresult) <=G__double(rresult)) return(1);
    else return(0);
    /* break; */
  case '<': /* <  */
    if(G__double(lresult) <G__double(rresult)) return(1);
    else return(0);
    /* break; */
  case '>': /* >  */
    if(G__double(lresult) >G__double(rresult)) return(1);
    else return(0);
    /* break; */
  }
  
  G__genericerror("Error: Unknow operator in test condition");
  return(0);

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
