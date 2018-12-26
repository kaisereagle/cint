/* /% C %/ */
/***********************************************************************
 * cint (C/C++ interpreter)
 ************************************************************************
 * Source file tmplt.c
 ************************************************************************
 * Description:
 *  Class and member function template
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

#ifndef G__OLDIMPLEMENTATION1712
int G__templatearg_enclosedscope=0;
#endif

/***********************************************************************
* G__IntList_init()
***********************************************************************/
void G__IntList_init(body,iin,prev)
struct G__IntList *body;
long iin;
struct G__IntList *prev;
{
  body->i=iin;
  body->next=(struct G__IntList*)NULL;
  body->prev = prev;
}

/***********************************************************************
* G__IntList_new()
***********************************************************************/
struct G__IntList* G__IntList_new(iin,prev)
long iin;
struct G__IntList *prev;
{
  struct G__IntList *body;
  body = (struct G__IntList*)malloc(sizeof(struct G__IntList));
  G__IntList_init(body,iin,prev);
  return(body);
}

/***********************************************************************
* G__IntList_add()
***********************************************************************/
void G__IntList_add(body,iin)
struct G__IntList *body;
long iin;
{
  while(body->next) body=body->next;
  body->next = G__IntList_new(iin,body);
}

/***********************************************************************
* G__IntList_addunique()
***********************************************************************/
void G__IntList_addunique(body,iin)
struct G__IntList *body;
long iin;
{
  while(body->next) {
    if(body->i==iin) return;
    body=body->next;
  }
  if(body->i==iin) return;
  body->next = G__IntList_new(iin,body);
}

/***********************************************************************
* G__IntList_delete(body)
***********************************************************************/
void G__IntList_delete(body)
struct G__IntList *body;
{
  if(body->prev && body->next) {
    body->prev->next = body->next;
    body->next->prev = body->prev;
  }
  else if(body->next) {
    body->next->prev = (struct G__IntList*)NULL;
  }
  else if(body->prev) {
    body->prev->next = (struct G__IntList*)NULL;
  }
  free(body);
}

/***********************************************************************
* G__IntList_find()
***********************************************************************/
struct G__IntList* G__IntList_find(body,iin)
struct G__IntList *body;
long iin;
{
  while(body->next) {
    if(body->i == iin) return(body);
    body=body->next;
  }
  if(body->i == iin) return(body);
  return((struct G__IntList*)NULL);
}

/***********************************************************************
* G__IntList_free()
***********************************************************************/
void G__IntList_free(body)
struct G__IntList *body;
{
  if(!body) return;
  if(body->prev) body->prev->next = (struct G__IntList*)NULL;
  while(body->next) G__IntList_free(body->next);
  free(body);
}

/***********************************************************************
* G__instantiate_templateclasslater()
*
*  instantiation of forward declared template class body
***********************************************************************/
void G__instantiate_templateclasslater(deftmpclass)
struct G__Definedtemplateclass *deftmpclass;
{
  /* forward declaration of template -> instantiation ->
   * definition of template NOW instantiate forward declaration */
  struct G__IntList *ilist = deftmpclass->instantiatedtagnum;
  int store_def_tagnum=G__def_tagnum;
  int store_tagdefining=G__tagdefining;
  int store_def_struct_member=G__def_struct_member;
  char tagname[G__LONGLINE];
  while(ilist) {
    G__ASSERT(ilist->i>=0);
    strcpy(tagname,G__struct.name[ilist->i]);
    if(-1!=G__struct.parent_tagnum[ilist->i]) {
      G__def_tagnum=G__struct.parent_tagnum[ilist->i];
      G__tagdefining=G__struct.parent_tagnum[ilist->i];
      G__def_struct_member=1;
    }
    else {
      G__def_tagnum=store_def_tagnum;
      G__tagdefining=store_tagdefining;
      G__def_struct_member=store_def_struct_member;
    }
    G__instantiate_templateclass(tagname);
    ilist = ilist->next;
  }
  G__def_tagnum=store_def_tagnum;
  G__tagdefining=store_tagdefining;
  G__def_struct_member=store_def_struct_member;
}

/***********************************************************************
* G__instantiate_templatememfunclater()
*
*  instantiation of forward declared template class member function
***********************************************************************/
void G__instantiate_templatememfunclater(deftmpclass,deftmpmemfunc)
struct G__Definedtemplateclass *deftmpclass;
struct G__Definedtemplatememfunc *deftmpmemfunc;
{
  struct G__IntList* ilist=deftmpclass->instantiatedtagnum;
  struct G__Charlist call_para;
  char templatename[G__LONGLINE];
  char tagname[G__LONGLINE];
  char *arg;
  int npara=0;
  int store_def_tagnum=G__def_tagnum;
  int store_tagdefining=G__tagdefining;
  int store_def_struct_member=G__def_struct_member;

  while(ilist) {
    G__ASSERT(0<=ilist->i);
    if (G__struct.name[ilist->i]==0) {
       ilist = ilist->next;
       continue;
    }
    strcpy(tagname,G__struct.name[ilist->i]);
    strcpy(templatename,tagname);
    arg = strchr(templatename,'<');
    if(arg) {
      *arg='\0';
      ++arg;
    }
    else {
      arg = "";
    }
    call_para.string=(char*)NULL;
    call_para.next = (struct G__Charlist*)NULL;
    G__gettemplatearglist(arg,&call_para,deftmpclass->def_para,&npara
			  ,-1
			  );
    if(-1!=G__struct.parent_tagnum[ilist->i]) {
      G__def_tagnum=G__struct.parent_tagnum[ilist->i];
      G__tagdefining=G__struct.parent_tagnum[ilist->i];
      G__def_struct_member=1;
    }
    else {
      G__def_tagnum=store_def_tagnum;
      G__tagdefining=store_tagdefining;
      G__def_struct_member=store_def_struct_member;
    }
    G__replacetemplate(templatename,tagname,&call_para
		       ,deftmpmemfunc->def_fp
		       ,deftmpmemfunc->line
		       ,deftmpmemfunc->filenum
		       ,&(deftmpmemfunc->def_pos)
		       ,deftmpclass->def_para
		       ,0
		       ,npara
		       ,deftmpclass->parent_tagnum
		       );
    G__freecharlist(&call_para);
    ilist=ilist->next;
  }
  G__def_tagnum=store_def_tagnum;
  G__tagdefining=store_tagdefining;
  G__def_struct_member=store_def_struct_member;
}

#ifndef G__OLDIMPLEMENTATION1867
/***********************************************************************
* G__settemplatealias()
*
***********************************************************************/
int G__settemplatealias(tagnamein,tagname,tagnum,charlist,defpara,encscope)
char *tagnamein;
char *tagname;
int tagnum;
struct G__Charlist *charlist;
struct G__Templatearg *defpara;
int encscope;
{
  char *p;
  p=strchr(tagname,'<');
  if(p) ++p;
  else {
    p = tagname + strlen(tagname);
    *p++ = '<';
  }
  /* B<int,5*2>
   *   ^ => p */
  while(charlist->next) {
    if(defpara->default_parameter) {
      *(p-1)='>'; *p=0;
      if(0!=strcmp(tagnamein,tagname) && -1==G__defined_typename(tagname)) {
	int typenum=G__newtype.alltype++;
	G__newtype.type[typenum]='u';
	G__newtype.tagnum[typenum] = tagnum;
	G__newtype.name[typenum]=(char*)malloc(strlen(tagname)+1);
	strcpy(G__newtype.name[typenum],tagname);
	G__newtype.hash[typenum] = strlen(tagname);
	G__newtype.globalcomp[typenum] = G__globalcomp;
	G__newtype.reftype[typenum] = G__PARANORMAL;
	G__newtype.nindex[typenum] = 0;
	G__newtype.index[typenum] = (int*)NULL;
	G__newtype.iscpplink[typenum] = G__NOLINK;
	if(encscope) {
	  G__newtype.parent_tagnum[typenum] = G__get_envtagnum();
	}
	else {
	  G__newtype.parent_tagnum[typenum] = G__struct.parent_tagnum[tagnum];
	}
      }
    }
    strcpy(p,charlist->string);
    p+=strlen(charlist->string);
    charlist=charlist->next;
    defpara=defpara->next;
    if(charlist->next) {
      *p=','; ++p;
    }
  }
  *p='>'; ++p;
  *p='\0'; ++p;
  return 0;
}
#endif


#ifdef G__TEMPLATECLASS
/***********************************************************************
* G__cattemplatearg()
*
* Concatinate templatename and template arguments
***********************************************************************/
int G__cattemplatearg(tagname,charlist)
char *tagname;
struct G__Charlist *charlist;
{
  char *p;
  p=strchr(tagname,'<');
  if(p) ++p;
  else {
    p = tagname + strlen(tagname);
    *p++ = '<';
  }
  /* B<int,5*2>
   *   ^ => p */
  while(charlist->next) {
    strcpy(p,charlist->string);
    p+=strlen(charlist->string);
    charlist=charlist->next;
    if(charlist->next) {
      *p=','; ++p;
    }
  }
  *p='>'; ++p;
  *p='\0'; ++p;
  return 0;
}

/***********************************************************************
* G__catparam()
*
* Cancatinate parameter string to libp->parameter[0] and return.
*
*  "B<int"   "double"   "5>"     =>    "B<int,double,5>"
***********************************************************************/
char *G__catparam(libp,catn,connect)
struct G__param *libp;
int catn;
char *connect;
{
  int i;
  char *p;
  int lenconnect;
  /* B<int\0
   *      ^ => p */
  p = libp->parameter[0]+strlen(libp->parameter[0]);
  lenconnect = strlen(connect);
  for(i=1;i<catn;i++) {
    strcpy(p,connect);
    p+=lenconnect;
    strcpy(p,libp->parameter[i]);
    p+=strlen(libp->parameter[i]);
  }
  return(libp->parameter[0]);
}

/**************************************************************************
* G__read_formal_templatearg()
*
*  template<class T,class E,int S> ...
*           ^
**************************************************************************/
struct G__Templatearg *G__read_formal_templatearg()
{
  struct G__Templatearg *targ=NULL;
  struct G__Templatearg *p=NULL;
  char type[G__MAXNAME],name[G__MAXNAME];
  int c;
  int stat=1;

  do {

    /* allocate entry of template argument list */
    if(stat) {
      p = (struct G__Templatearg *)malloc(sizeof(struct G__Templatearg));
      p->next = (struct G__Templatearg *)NULL;
      /* store entry of the template argument list */
      targ = p;
      stat=0;
    }
    else {
      p->next = (struct G__Templatearg *)malloc(sizeof(struct G__Templatearg));
      p=p->next;
      p->next = (struct G__Templatearg *)NULL;
    }

    /*  template<class T,class E,int S> ...
     *           ^                            */
    c = G__fgetname(type,"<");
    if (strcmp (type, "const") == 0 && c == ' ') c=G__fgetname(type,"<");
    if(strcmp(type,"class")==0 || strcmp(type,"typename")==0) {
      p->type = G__TMPLT_CLASSARG;
    }
    else if('<'==c && strcmp(type,"template")==0) {
      c=G__fignorestream(">");
      c=G__fgetname(type,"");
      G__ASSERT(0==strcmp(type,"class")||0==strcmp(type,"typename"));
      p->type = G__TMPLT_TMPLTARG;
    }
    else {
      if(strcmp(type,"int")==0) p->type = G__TMPLT_INTARG;
      else if(strcmp(type,"size_t")==0) p->type = G__TMPLT_SIZEARG;
      else if(strcmp(type,"unsignedint")==0) p->type = G__TMPLT_UINTARG;
      else if(strcmp(type,"unsigned")==0) {
	fpos_t pos;
	int linenum;
	fgetpos(G__ifile.fp,&pos);
	linenum = G__ifile.line_number;
	c = G__fgetname(name,",>="); 
	if(strcmp(name,"int")==0) p->type = G__TMPLT_UINTARG;
	else if(strcmp(name,"short")==0) p->type = G__TMPLT_USHORTARG;
	else if(strcmp(name,"char")==0) p->type = G__TMPLT_UCHARARG;
	else if(strcmp(name,"long")==0) {
	  p->type = G__TMPLT_ULONGARG;
	  fgetpos(G__ifile.fp,&pos);
	  linenum = G__ifile.line_number;
	  c = G__fgetname(name,",>="); 
	  if(strcmp(name,"int")==0) {
	    p->type = G__TMPLT_ULONGARG;
	  }
	  else {
	    p->type = G__TMPLT_ULONGARG;
	    fsetpos(G__ifile.fp,&pos);
	    G__ifile.line_number = linenum;
	  }
	}
	else {
	  p->type = G__TMPLT_UINTARG;
	  fsetpos(G__ifile.fp,&pos);
	  G__ifile.line_number = linenum;
	}
      }
      else if(strcmp(type,"char")==0) p->type = G__TMPLT_CHARARG;
      else if(strcmp(type,"unsignedchar")==0) p->type = G__TMPLT_UCHARARG;
      else if(strcmp(type,"short")==0) p->type = G__TMPLT_SHORTARG;
      else if(strcmp(type,"unsignedshort")==0) p->type = G__TMPLT_USHORTARG;
      else if(strcmp(type,"long")==0) p->type = G__TMPLT_LONGARG;
      else if(strcmp(type,"unsignedlong")==0) p->type = G__TMPLT_ULONGARG;
      else if(strcmp(type,"float")==0) p->type = G__TMPLT_FLOATARG;
      else if(strcmp(type,"double")==0) p->type = G__TMPLT_DOUBLEARG;
      else if(strcmp(type,">")==0) {
	if(targ) free((void*)targ);
	targ = (struct G__Templatearg *)NULL;
	return(targ);
      }
      else {
	if(G__dispsource) {
	  G__fprinterr(G__serr,"Limitation: template argument type '%s' may cause problem",type);
	  G__printlinenum();
	}
	p->type = G__TMPLT_INTARG;
      }
    }

    /*  template<class T,class E,int S> ...
     *                 ^                     */
    c = G__fgetstream(name,",>="); /* G__fgetstream_tmplt() ? */
    while(name[0] && '*'==name[strlen(name)-1]) {
      if(G__TMPLT_CLASSARG==p->type) p->type = G__TMPLT_POINTERARG1;
      else p->type+=G__TMPLT_POINTERARG1;
      name[strlen(name)-1] = '\0';
    }
    p->string=(char*)malloc(strlen(name)+1);
    strcpy(p->string,name);

    if('='==c) {
      c = G__fgetstream_template(name,",>"); /* G__fgetstream_tmplt() ? */
      p->default_parameter=(char*)malloc(strlen(name)+1);
      strcpy(p->default_parameter,name);
    }
    else {
      p->default_parameter=(char*)NULL;
    }

    /*  template<class T,class E,int S> ...
     *                   ^                  */
  } while(','==c) ;

  /*  template<class T,class E,int S> ...
   *                                 ^    */

  return(targ);
}

/**************************************************************************
* G__read_specializationarg()
*
*  template<class T,class E,int S> ...
*           ^
**************************************************************************/
struct G__Templatearg *G__read_specializationarg(source)
char* source;
{
  struct G__Templatearg *targ=NULL;
  struct G__Templatearg *p=NULL;
  char type[G__MAXNAME];
  /* int c; */
  int i,j,nest;
  int stat=1;
  int isrc=0;
  int len;

  do {

    /* allocate entry of template argument list */
    if(stat) {
      p = (struct G__Templatearg *)malloc(sizeof(struct G__Templatearg));
      p->next = (struct G__Templatearg *)NULL;
      p->default_parameter=(char*)NULL;
      /* store entry of the template argument list */
      targ = p;
      stat=0;
    }
    else {
      p->next = (struct G__Templatearg *)malloc(sizeof(struct G__Templatearg));
      p=p->next;
      p->default_parameter=(char*)NULL;
      p->next = (struct G__Templatearg *)NULL;
    }

    p->type = 0;
    /*  templatename<T*,E,int> ...
     *                ^                            */
    /* We need to insure to get the real arguments and nothing else! */
    if(strncmp (source, "const ", strlen("const ")) == 0) {
      p->type |= G__TMPLT_CONSTARG;
      isrc = strlen("const ");
    }
    len = strlen(source);
    for(i=isrc,j=0,nest=0;i<len;++i) {
      switch(source[i]) {
      case '<': ++nest; break;
      case '>': --nest; if (nest<0) { i=len; continue; } break;
      case ',': if (nest==0) { i=len; continue; } break;
      }
      type[j++] = source[i];
    }
    type[j] = 0;
    len = strlen(type);
    if('&'==type[len-1]) {
      p->type |= G__TMPLT_REFERENCEARG;
      type[--len] = 0;
    }
    while('*'==type[len-1]) {
      p->type += G__TMPLT_POINTERARG1;
      type[--len] = 0;
    }

    if(strcmp(type,"int")==0) p->type |= G__TMPLT_INTARG;
    else if(strcmp(type,"size_t")==0) p->type |= G__TMPLT_SIZEARG;
    else if(strcmp(type,"unsigned int")==0) p->type |= G__TMPLT_UINTARG;
    else if(strcmp(type,"unsigned")==0) p->type |= G__TMPLT_UINTARG;
    else if(strcmp(type,"char")==0) p->type |= G__TMPLT_CHARARG;
    else if(strcmp(type,"unsigned char")==0) p->type |= G__TMPLT_UCHARARG;
    else if(strcmp(type,"short")==0) p->type |= G__TMPLT_SHORTARG;
    else if(strcmp(type,"unsigned short")==0) p->type |= G__TMPLT_USHORTARG;
    else if(strcmp(type,"long")==0) p->type |= G__TMPLT_LONGARG;
    else if(strcmp(type,"unsigned long")==0) p->type |= G__TMPLT_ULONGARG;
    else if(strcmp(type,"float")==0) p->type |= G__TMPLT_FLOATARG;
    else if(strcmp(type,"double")==0) p->type |= G__TMPLT_DOUBLEARG;
    else if(strcmp(type,">")==0) {
      if(targ) free((void*)targ);
      targ = (struct G__Templatearg *)NULL;
      return(targ);
    }
    else {
      p->type |= G__TMPLT_CLASSARG;
    }

    p->string=(char*)malloc(strlen(type)+1);
    strcpy(p->string,type);

    /*  template<T*,E,int> ...
     *              ^                  */
#ifndef G__OLDIMPLEMENTATION2180
  } while (0) ;
#else
  } while(','==c) ;
#endif

  /*  template<T*,E,int> ...
   *                   ^                  */

  return(targ);
}

/**************************************************************************
* G__delete_string
*
**************************************************************************/
static void G__delete_string(str,del)
char *str;
char *del;
{
  char *e;
  char *p = strstr(str,del);
  if(p) {
    e = p + strlen(del);
    while(*e) *(p++) = *(e++);
    *p=0;
  }
}
/**************************************************************************
* G__modify_callpara()
*
**************************************************************************/
static void G__modify_callpara(spec_arg,call_arg,pcall_para)
struct G__Templatearg *spec_arg;
struct G__Templatearg *call_arg ;
struct G__Charlist *pcall_para;
{
  while(spec_arg && call_arg && pcall_para) {
    int spec_p = spec_arg->type & G__TMPLT_POINTERARGMASK;
    int call_p = call_arg->type & G__TMPLT_POINTERARGMASK;
    int spec_r = spec_arg->type & G__TMPLT_REFERENCEARG;
    int call_r = call_arg->type & G__TMPLT_REFERENCEARG;
    int spec_c = spec_arg->type & G__TMPLT_CONSTARG;
    int call_c = call_arg->type & G__TMPLT_CONSTARG;
    if(spec_p>0 && spec_p<=call_p) {
      int i;
      int n = spec_p/G__TMPLT_POINTERARG1;
      char buf[10];
      for(i=0;i<n;i++) buf[i]='*';
      buf[n]=0;
      G__delete_string(pcall_para->string,buf);
    }
    if(spec_r && spec_r == call_r) {
      G__delete_string(pcall_para->string,"&");
    }
    if(spec_c && spec_c == call_c) {
      G__delete_string(pcall_para->string,"const ");
    }
    spec_arg = spec_arg->next;
    call_arg = call_arg->next;
    pcall_para = pcall_para->next;
  }
}

/**************************************************************************
* G__resolve_specialization(deftmpclass,pcall_para)
*
**************************************************************************/
static struct G__Definedtemplateclass *G__resolve_specialization(arg
								 ,deftmpclass
								 ,pcall_para)
char *arg;
struct G__Definedtemplateclass *deftmpclass;
struct G__Charlist *pcall_para;
{
  struct G__Definedtemplateclass *spec = deftmpclass->specialization;
  struct G__Templatearg *call_arg = G__read_specializationarg(arg);
  struct G__Templatearg *pcall_arg ;
  struct G__Templatearg *spec_arg;
  int match;
  struct G__Definedtemplateclass *bestmatch = deftmpclass;
  int best = 0;

  while(spec->next) {
    match = 0;
    spec_arg = spec->spec_arg;
    pcall_arg = call_arg;
    while(spec_arg && pcall_arg) {
      if(spec_arg->type==pcall_arg->type) match+=10;
      else {
	int spec_p = spec_arg->type & G__TMPLT_POINTERARGMASK;
	int call_p = call_arg->type & G__TMPLT_POINTERARGMASK;
	int spec_r = spec_arg->type & G__TMPLT_REFERENCEARG;
	int call_r = call_arg->type & G__TMPLT_REFERENCEARG;
	int spec_c = spec_arg->type & G__TMPLT_CONSTARG;
	int call_c = call_arg->type & G__TMPLT_CONSTARG;
	if(spec_r==call_r) ++match;
	else if(spec_r>call_r) {
	  match = 0;
	  break;
	}
	if(spec_p==call_p) ++match;
	else if(spec_p>call_p) {
	  match = 0;
	  break;
	}
	if(spec_c==call_c) ++match;
	else if(spec_c>call_c) {
	  match = 0;
	  break;
	}
      }
      spec_arg = spec_arg->next;
      pcall_arg = pcall_arg->next;
    }
    if(match>best) {
      bestmatch = spec;
      best = match;
    }
    spec = spec->next;
  }

  if(bestmatch!=deftmpclass) {
    G__modify_callpara(bestmatch->spec_arg,call_arg,pcall_para);
  }

  G__freetemplatearg(call_arg);

  return(bestmatch);
}

#ifdef G__TEMPLATEMEMFUNC
/**************************************************************************
* G__createtemplatememfunc()
*  template<class T,class E,int S> type A<T,E,S>::f() { .... }
*                                         ^
**************************************************************************/
int G__createtemplatememfunc(new_name)
char *new_name;
{
  /* int c; */
  struct G__Definedtemplateclass *deftmpclass;
  struct G__Definedtemplatememfunc *deftmpmemfunc;
  int os=0;

  /* funcname="*f()" "&f()" */
  while('*'==new_name[os] || '&'==new_name[os]) ++os;

  /* get defined tempalte class identity */
  deftmpclass = G__defined_templateclass(new_name+os);
  if(!deftmpclass) {
    /* error */
    G__fprinterr(G__serr,"Error: Template class %s not defined",new_name+os);
    G__genericerror((char*)NULL);
  }
  else {
    /* get to the end of defined member function list */
    deftmpmemfunc = &(deftmpclass->memfunctmplt) ;
    while(deftmpmemfunc->next) deftmpmemfunc=deftmpmemfunc->next;

    /* allocate member function template list */
    deftmpmemfunc->next = (struct G__Definedtemplatememfunc*)malloc(sizeof(struct G__Definedtemplatememfunc));
    deftmpmemfunc->next->next = (struct G__Definedtemplatememfunc*)NULL;

    /* set file position */
    deftmpmemfunc->def_fp = G__ifile.fp;
    deftmpmemfunc->line = G__ifile.line_number;
    deftmpmemfunc->filenum = G__ifile.filenum;
    fgetpos(G__ifile.fp,&deftmpmemfunc->def_pos);

    /* if member function is defined after template class instantiation
     * instantiate member functions here */
    if(deftmpclass->instantiatedtagnum) {
      G__instantiate_templatememfunclater(deftmpclass,deftmpmemfunc);
    }
  }
   return(0);
}
#endif


/**************************************************************************
* G__createtemplateclass()
*  template<class T,class E,int S> class A { .... };
*                                 ^
**************************************************************************/
int G__createtemplateclass(new_name,targ
                          ,isforwarddecl
                          )
char *new_name;
struct G__Templatearg *targ;
int isforwarddecl;
{
  struct G__Definedtemplateclass *deftmpclass;
  int hash,i;
  int override=0;
  int env_tagnum = G__get_envtagnum();

  struct G__Templatearg *spec_arg=(struct G__Templatearg*)NULL;
  char *spec = strchr(new_name,'<');
  if(spec) {
    *spec = 0;
    spec_arg = G__read_specializationarg(spec+1);
  }

  /* Search for the end of list */
  deftmpclass = &G__definedtemplateclass;
  G__hash(new_name,hash,i)
  while(deftmpclass->next) {
    if(deftmpclass->hash==hash && strcmp(deftmpclass->name,new_name)==0
       && env_tagnum==deftmpclass->parent_tagnum
       ) {
      if(0==deftmpclass->isforwarddecl && deftmpclass->def_fp) {
	if(isforwarddecl) {
	  /* Ignore an incomplete declaration after a complete one */
	  G__fignorestream(";");
	  return(0);
	}
	if(spec_arg) {
	  if(!deftmpclass->specialization) {
	    deftmpclass->specialization = (struct G__Definedtemplateclass*)
	      malloc(sizeof(struct G__Definedtemplateclass));
	    deftmpclass = deftmpclass->specialization;
	    deftmpclass->def_para = (struct G__Templatearg*)NULL;
	    deftmpclass->next = (struct G__Definedtemplateclass*)NULL;
	    deftmpclass->name = (char*)NULL;
	    deftmpclass->hash = 0;
	    deftmpclass->memfunctmplt.next
	      = (struct G__Definedtemplatememfunc*)NULL;
	    deftmpclass->def_fp = (FILE*)NULL;
	    deftmpclass->isforwarddecl = 0;
	    deftmpclass->instantiatedtagnum = (struct G__IntList*)NULL;
	    deftmpclass->specialization=(struct G__Definedtemplateclass*)NULL;
	    deftmpclass->spec_arg=(struct G__Templatearg*)NULL;
	  }
	  else {
	    deftmpclass = deftmpclass->specialization;
	    while(deftmpclass->next) deftmpclass=deftmpclass->next;
	  }
	  deftmpclass->spec_arg = spec_arg;
	  override=0;
	  break;
	}
	/* ignore duplicate template class definition */
	if(G__dispmsg>=G__DISPWARN) {
	  G__fprinterr(G__serr,"Warning: template %s duplicate definition",new_name);
	  G__printlinenum();
	}
	G__fignorestream(";");
	return(0);
      }
      override=1;
      break;
    }
    deftmpclass=deftmpclass->next;
  }

  if(!override) {
    /* store name and hash key */
    deftmpclass->name = malloc(strlen(new_name)+1);
    strcpy(deftmpclass->name,new_name);
    deftmpclass->hash=hash;
  }

  /* store parent_tagnum */
  {
      int env_tagnum;
      if(-1!=G__def_tagnum) {
	if(G__tagdefining!=G__def_tagnum) env_tagnum=G__tagdefining;
	else                              env_tagnum=G__def_tagnum;
      }
      else env_tagnum = -1;
      deftmpclass->parent_tagnum = env_tagnum;
  }

  /* store template argument list */
  if(!override || !deftmpclass->def_para) deftmpclass->def_para=targ;
  else {
    struct G__Templatearg* t1 = deftmpclass->def_para;
    struct G__Templatearg* t2 = targ;
    while (t1 && t2) {
      if (strcmp (t1->string, t2->string) != 0) {
	char *tmp = t2->string;
	t2->string = t1->string;
	t1->string = tmp;
      }
      if(t1->default_parameter && t2->default_parameter) {
	G__genericerror("Error: Redefinition of default template argument");
      }
      else if(!t1->default_parameter && t2->default_parameter) {
	t1->default_parameter = t2->default_parameter;
	t2->default_parameter = 0;
      }
      t1 = t1->next;
      t2 = t2->next;
    }
    G__freetemplatearg (targ);
  }

  /* store file pointer, line number and position */
  deftmpclass->def_fp = G__ifile.fp;
  if(G__ifile.fp) fgetpos(G__ifile.fp,&deftmpclass->def_pos);
  deftmpclass->line = G__ifile.line_number;
  deftmpclass->filenum = G__ifile.filenum;

  if(!override) {
    /* allocate and initialize next list */
    deftmpclass->next = (struct G__Definedtemplateclass*)malloc(sizeof(struct G__Definedtemplateclass));
    deftmpclass->next->def_para = (struct G__Templatearg*)NULL;
    deftmpclass->next->next = (struct G__Definedtemplateclass*)NULL;
    deftmpclass->next->name = (char*)NULL;
    deftmpclass->next->hash = 0;
    deftmpclass->next->memfunctmplt.next
      = (struct G__Definedtemplatememfunc*)NULL;
    deftmpclass->next->def_fp = (FILE*)NULL;
    deftmpclass->next->isforwarddecl = 0;
    deftmpclass->next->instantiatedtagnum = (struct G__IntList*)NULL;
    deftmpclass->next->specialization=(struct G__Definedtemplateclass*)NULL;
    deftmpclass->next->spec_arg=(struct G__Templatearg*)NULL;
  }
  /* skip template class body */
  if(targ) G__fignorestream(";");
  /*  template<class T,class E,int S> class A { .... };
   *                                                   ^ */

  /* forward declaration of template -> instantiation ->
   * definition of template NOW instantiate forward declaration */
  if(1==deftmpclass->isforwarddecl && 0==isforwarddecl &&
     deftmpclass->instantiatedtagnum) {
    G__instantiate_templateclasslater(deftmpclass);
  }
  deftmpclass->isforwarddecl = isforwarddecl;

  return(0);
}

/***********************************************************************
* G__getobjecttagnum
***********************************************************************/
int G__getobjecttagnum(name)
char *name;
{
  int result = -1;
  char *p;
  char *p1;
  char *p2;
  p1 = strrchr(name,'.');
  p2 = G__strrstr(name,"->");

  if(!p1 && !p2) {
    struct G__var_array *var;
    int ig15;
    int itmpx,varhash;
    long store_struct_offset1=0,store_struct_offset2=0;
    G__hash(name,varhash,itmpx);
    var = G__searchvariable(name,varhash,G__p_local,&G__global
			    ,&store_struct_offset1,&store_struct_offset2
			    ,&ig15
			    ,0);
    if(var && 'u'==tolower(var->type[ig15]) && -1!=var->p_tagtable[ig15]) {
      result = var->p_tagtable[ig15];
      return(result);
    }
    else {
      char *p3 = strchr(name,'(');
      if(p3) {
	/* LOOK FOR A FUNCTION */
      }
    }
  }

  else {
    if(p1>p2 || !p2) {
      *p1 = 0;
      p = p1+1;
    }
    else /* if(p2>p1 || !p1) */ {
      *p2 = 0;
      p = p2+2;
    }
    
    result = G__getobjecttagnum(name);
    if(-1!=result) {
      /* TO BE IMPLEMENTED */
      /* struct G__var_array *var = G__struct.memvar[result];
	 struct G__ifunc_table *ifunc = G__struct.memfunc[result]; */
    }
  }

  if(p1 && 0==(*p1)) *p1 = '.';
  if(p2 && 0==(*p2)) *p2 = '-';
  return(result);
}


/***********************************************************************
* G__defined_templatememfunc()
*
* t.Handle<int>();
* a.t.Handle<int>();
* a.f().Handle<int>();
*
***********************************************************************/
struct G__Definetemplatefunc *G__defined_templatememfunc(name)
char *name;
{
  char *p;
  char *p1;
  char *p2;
  char atom_name[G__LONGLINE];
  int store_asm_noverflow = G__asm_noverflow ;
  struct G__Definetemplatefunc *result= NULL;

  /* separate "t" and "Handle" */
  strcpy(atom_name,name);
  p1 = strrchr(atom_name,'.');
  p2 = G__strrstr(atom_name,"->");
  if(!p1 && !p2) return(result);

  if(p1>p2 || !p2) {
    *p1 = 0;
    p = p1+1;
  }
  else /* if(p2>p1 || !p1) */ {
    *p2 = 0;
    p = p2+2;
  }
  /* "t" as name "Handle" as p */

  G__suspendbytecode();

  {
    int tagnum = G__getobjecttagnum(atom_name);
    if(-1!=tagnum) {
      int store_def_tagnum = G__def_tagnum;
      int store_tagdefining = G__tagdefining;
      /* Have to look at base class */
      G__def_tagnum = tagnum;
      G__tagdefining = tagnum;
      result = G__defined_templatefunc(p);
      G__def_tagnum = store_def_tagnum;
      G__tagdefining = store_tagdefining;
      if(!result) {
	struct G__ifunc_table *ifunc=G__struct.memfunc[tagnum];
	int ifn;
	int len=strlen(p);
	p[len++]='<';
	p[len]=0;
	while(ifunc) {
	  for(ifn=0;ifn<ifunc->allifunc;ifn++) {
	    if(0==strncmp(ifunc->funcname[ifn],p,len)) {
	      result = (struct G__Definetemplatefunc*)G__PVOID;
	    }
	  }
	  ifunc = ifunc->next;
	}
	p[len-1]=0;
      }
    }
  }

  G__asm_noverflow = store_asm_noverflow;
  if(p1 && 0==(*p1)) *p1 = '.';
  if(p2 && 0==(*p2)) *p2 = '-';
  return(result);
}

#ifndef G__OLDIMPLEMENTATION1560
/***********************************************************************
* G__defined_templatefunc()
*
* Check if the template function is declared
***********************************************************************/
struct G__Definetemplatefunc *G__defined_templatefunc(name)
char *name;
{
  struct G__Definetemplatefunc *deftmplt;
  int hash,temp;
  long dmy_struct_offset=0;
  char atom_name[G__LONGLINE];
  int env_tagnum=G__get_envtagnum();
  int scope_tagnum = -1;
  struct G__inheritance *baseclass;

  /* return if no name */
  if('\0'==name[0]||strchr(name,'.')||strchr(name,'-') || strchr(name,'('))
    return((struct G__Definetemplatefunc*)NULL);

  /* get a handle for using declaration info */
  if(-1!=env_tagnum && G__struct.baseclass[env_tagnum]->basen!=0)
     baseclass = G__struct.baseclass[env_tagnum];
  else
     baseclass = (struct G__inheritance*)NULL;

  /* scope operator resolution, A::templatename<int> ... */
  strcpy(atom_name,name);
  G__hash(atom_name,hash,temp)
  G__scopeoperator(atom_name,&hash,&dmy_struct_offset,&scope_tagnum);

  /* Don't crash on a null name (like 'std::'). */
  if('\0' == atom_name[0])
    return((struct G__Definetemplatefunc*)NULL);

  /* search for template name and scope match */
  deftmplt = &G__definedtemplatefunc;
  while(deftmplt->next) { /* BUG FIX */
    if(hash==deftmplt->hash && strcmp(atom_name,deftmplt->name)==0) {
      /* look for ordinary scope resolution */
      if((-1==scope_tagnum &&
	  -1==G__tagdefining &&
	  (-1==deftmplt->parent_tagnum||env_tagnum==deftmplt->parent_tagnum))
	 || (scope_tagnum==deftmplt->parent_tagnum
	     && (-1==G__tagdefining || G__tagdefining==deftmplt->parent_tagnum)
	     )) {
	return(deftmplt);
      }
      else if(-1==scope_tagnum) {
	int env_parent_tagnum = env_tagnum;
	if(baseclass) {
	  /* look for using directive scope resolution */
	  for(temp=0;temp<baseclass->basen;temp++) {
	    if(baseclass->basetagnum[temp]==deftmplt->parent_tagnum) {
	      return(deftmplt);
	    }
	  }
	}
	/* look for enclosing scope resolution */
	while(-1!=env_parent_tagnum) {
	  env_parent_tagnum = G__struct.parent_tagnum[env_parent_tagnum];
	  if(env_parent_tagnum==deftmplt->parent_tagnum
	     && (-1==G__tagdefining || G__tagdefining==deftmplt->parent_tagnum)
	     ) 
	    return(deftmplt);
          if(G__struct.baseclass[env_parent_tagnum]) {
            for(temp=0;temp<G__struct.baseclass[env_parent_tagnum]->basen;temp++) {
              if(G__struct.baseclass[env_parent_tagnum]->basetagnum[temp]==deftmplt->parent_tagnum) {
                return(deftmplt);
              }
	    }
	  }
	}
        /* look in global scope (handle for using declaration info */
        for(temp=0;temp<G__globalusingnamespace.basen;temp++) {
          if(G__globalusingnamespace.basetagnum[temp]==deftmplt->parent_tagnum) {
            return(deftmplt);
          }
        }
      }
    }
    deftmplt=deftmplt->next;
  }
  return((struct G__Definetemplatefunc*)NULL);
}
#endif


/***********************************************************************
* G__defined_templateclass()
*
* Check if the template class is declared
*  but maybe in future I might need this to handle case 4,5
***********************************************************************/
struct G__Definedtemplateclass *G__defined_templateclass(name)
char *name;
{
  struct G__Definedtemplateclass *deftmplt;
  int hash,temp;
  long dmy_struct_offset=0;
  char atom_name[G__LONGLINE];
  int env_tagnum=G__get_envtagnum();
  int scope_tagnum = -1;
  struct G__inheritance *baseclass;

  /* return if no name */
  if('\0'==name[0]||strchr(name,'.')||strchr(name,'-')
     || strchr(name,'(')
     || isdigit(name[0]) || (!isalpha(name[0]) && '_'!=name[0] && ':'!=name[0])
     )
     return((struct G__Definedtemplateclass *)NULL);

  /* get a handle for using declaration info */
  if(-1!=env_tagnum && G__struct.baseclass[env_tagnum]->basen!=0)
    baseclass = G__struct.baseclass[env_tagnum];
  else
    baseclass = (struct G__inheritance*)NULL;

  /* scope operator resolution, A::templatename<int> ... */
  strcpy(atom_name,name);
  G__hash(atom_name,hash,temp)
  G__scopeoperator(atom_name,&hash,&dmy_struct_offset,&scope_tagnum);

  /* Don't crash on a null name (like 'std::'). */
  if('\0' == atom_name[0])
    return((struct G__Definedtemplateclass*)NULL);

  /* search for template name and scope match */
  deftmplt = &G__definedtemplateclass;
  while(deftmplt->next) { /* BUG FIX */
    if(hash==deftmplt->hash && strcmp(atom_name,deftmplt->name)==0) {
      /* look for ordinary scope resolution */
      if((-1==scope_tagnum&&(-1==deftmplt->parent_tagnum||
			     env_tagnum==deftmplt->parent_tagnum))||
	 scope_tagnum==deftmplt->parent_tagnum) {
	return(deftmplt);
      }
      else if(-1==scope_tagnum) {
	int env_parent_tagnum = env_tagnum;
	if(baseclass) {
	  /* look for using directive scope resolution */
	  for(temp=0;temp<baseclass->basen;temp++) {
	    if(baseclass->basetagnum[temp]==deftmplt->parent_tagnum) {
	      return(deftmplt);
	    }
	  }
	}
	/* look for enclosing scope resolution */
	while(-1!=env_parent_tagnum) {
	  env_parent_tagnum = G__struct.parent_tagnum[env_parent_tagnum];
	  if(env_parent_tagnum==deftmplt->parent_tagnum) return(deftmplt);
          if(G__struct.baseclass[env_parent_tagnum]) {
            for(temp=0;temp<G__struct.baseclass[env_parent_tagnum]->basen;temp++) {
              if(G__struct.baseclass[env_parent_tagnum]->basetagnum[temp]==deftmplt->parent_tagnum) {
                return(deftmplt);
              }
            }
          }
        }
        /* look in global scope (handle for using declaration info */
        for(temp=0;temp<G__globalusingnamespace.basen;temp++) {
          if(G__globalusingnamespace.basetagnum[temp]==deftmplt->parent_tagnum) {
            return(deftmplt);
          }
	}
      }
    }
    deftmplt=deftmplt->next;
  }
  return((struct G__Definedtemplateclass *)NULL);

}

/***********************************************************************
* G__explicit_template_specialization()
*
*  Handle explicit template specialization
*
*  template<>  class A<int> { A(A& x); A& operator=(A& x); };
*  template<>  void A<int>::A(A& x) { }
*             ^
*
***********************************************************************/
int G__explicit_template_specialization()
{
#if !defined(G__OLDIMPLEMENTATION1792)
  char buf[G__ONELINE];
  int cin;

  /* store file position */
  fpos_t store_pos;
  int store_line=G__ifile.line_number;
  fgetpos(G__ifile.fp,&store_pos);
  G__disp_mask = 1000;

  /* forward proving */
  cin = G__fgetname_template(buf,":{;");
  if(strcmp(buf,"class")==0 || strcmp(buf,"struct")==0) {
    /* template<>  class A<int> { A(A& x); A& operator=(A& x); };
     *                  ^                      */
    char *pp;
    char templatename[G__ONELINE];
    int npara=0;
    int envtagnum = G__get_envtagnum();
    struct G__Charlist call_para;
    /* struct G__Templatearg def_para; */
    fpos_t posend;
    int lineend;

    call_para.string=(char*)NULL;
    call_para.next = (struct G__Charlist*)NULL;

    /* def_para.next = (struct G__Templatearg *)NULL; */

    cin = G__fgetname_template(buf,":{;");
    strcpy(templatename,buf);
    pp=strchr(templatename,'<');
    if(pp) *pp=0;

    if(':'==cin) {
      cin = G__fignorestream("{;");
    }
    if('{'==cin) {
      G__disp_mask = 1;
      fseek(G__ifile.fp,-1,SEEK_CUR);
      cin = G__fignorestream("};");
    }
    fgetpos(G__ifile.fp,&posend);
    lineend=G__ifile.line_number;

    /* rewind file position 
     * template<> class A<int> { ... } 
     *           ^--------------       */
    G__disp_mask = 0;
    fsetpos(G__ifile.fp,&store_pos);
    G__ifile.line_number = store_line;

    G__replacetemplate(templatename,buf,&call_para
		       ,G__ifile.fp
		       ,G__ifile.line_number
		       ,G__ifile.filenum
		       ,&store_pos
		       ,(struct G__Templatearg*)NULL
		       ,1
		       ,npara
		       ,envtagnum
		       );

    fsetpos(G__ifile.fp,&posend);
    G__ifile.line_number=lineend;
    return(0);
  }
  else {
    G__disp_mask = 0;
    fsetpos(G__ifile.fp,&store_pos);
    G__ifile.line_number = store_line;
    G__exec_statement();
    return(0);
  }

#else
  G__exec_statement();
  return(0);
#endif
}

/***********************************************************************
* G__declare_template()
*
* Entry of template declaration parsing
*
*   template<class T> class A { };
*            ^
*   template<class T> type A <T>::f() { }
*   template<class T> A <T>::B<T> A <T>::f() { }
*   template<class T> A <T>::A() { }
*            ^
*   template<class T> type A <T>::staticmember;
*   template<class T> A <T>::B<T> A <T>::staticmember;
*            ^
*   template<class T> type f() { }
*   template<class T> A <T>::B<T> f() { }
*            ^
***********************************************************************/
void G__declare_template()
{
  char temp[G__LONGLINE];
  fpos_t pos;
  int store_line_number;
  struct G__Templatearg *targ;
  int c;
  char *p;
  char temp2[G__LONGLINE];
  char temp3[G__LONGLINE];
  int ismemvar=0;
  int isforwarddecl = 0;
  int isfrienddecl = 0;

  if(G__ifile.filenum>G__gettempfilenum()) {
    G__fprinterr(G__serr,"Limitation: template can not be defined in a command line or a tempfile\n");
    G__genericerror("You need to write it in a source file");
    return;
  }

  /* Set a flag that template or macro is included in the source file,
   * so that this file won't be closed even with -cN option after preRUN */
  ++G__macroORtemplateINfile;

  /* read template argument declaration */
  targ=G__read_formal_templatearg();
  if(!targ) {/* in case of 'template<>' */
    G__explicit_template_specialization();
    return;
  }

  /*  template<class T,class E,int S> ...
   *                                 ^   store this position below */
  fgetpos(G__ifile.fp,&pos);
  store_line_number = G__ifile.line_number;
  /* if(G__dispsource) G__disp_mask=1000; */

  do {
    c=G__fgetname_template(temp,"(<");
    if (strcmp(temp,"friend")==0) {
       isfrienddecl = 1;
       c=G__fgetname_template(temp,"(<");
    }
  } while(strcmp(temp,"inline")==0||strcmp(temp,"const")==0
	  || strcmp(temp,"typename")==0
	  ) ;

  /* template class */
  if(strcmp(temp,"class")==0 || strcmp(temp,"struct")==0) {
    c = G__fgetstream_template(temp,":{;"); /* read template name */
    if(';'==c) {
      isforwarddecl = 1;
    }
    fsetpos(G__ifile.fp,&pos);
    if(G__dispsource) G__disp_mask=0;
    G__ifile.line_number = store_line_number;
    G__createtemplateclass(temp,targ,isforwarddecl);
    return;
  }

  /* Judge between template class member and global function */
  if('<'==c) {
    /* must judge if this is a constructor or other function
     *1 template<class T> A<T>::f()  constructor
     *2 template<class T> A<T>::B<T> A<T>::f()
     *3 template<class T> A<T> A<T>::f()
     *4 template<class T> A<T>::B<T> f()
     *5 template<class T> A<T> f()
     *6 template<class T> A<T> A<T>::v;
     *6'template<class T> A<T> A<T>::v = 0;
     *7 template<class T> A<T> { }  constructor
     *  also the return value could be a pointer or reference or const 
     *  or any combination of the 3
     *                      ^>^            */
    c = G__fgetstream_template(temp3,">");
    c = G__fgetname_template(temp2,"*&(;");
    if (c=='*' && strncmp(temp2,"operator",strlen("operator"))==0) {
       strcat(temp2,"*");
       c = G__fgetname_template(temp2+strlen(temp2),"*&(;=");
    }
    while (c=='&'||c=='*') {
       /* we skip all the & and * we see and what's in between.
          This should be removed from the func name (what we are looking for)
          anything preceding combinations of *,& and const. */
       c = G__fgetname_template(temp2,"*&(;=");
        if (c=='=' && strncmp(temp2,"operator",strlen("operator"))==0) {
           strcat(temp2,"=");
           c = G__fgetname_template(temp2+strlen(temp2),"*&(;=");
        }
    }
    if(0==temp2[0]) { /* constructor template in class definition */
      strcat(temp,"<");
      strcat(temp,temp3);
      strcat(temp,">");
    }
    if(isspace(c)) {
      if(strcmp(temp2,"::~")==0)
	c = G__fgetname_template(temp2+3,"(;");
      else if(strcmp(temp2,"::")==0)
	c = G__fgetname_template(temp2+2,"(;");
      else if((p=strstr(temp2,"::"))&&strcmp(p,"::operator")==0) {
	/* A<T> A<T>::operator T () { } */
	c='<'; /* this is a flag indicating this is a member function tmplt */
      }
      else if(strcmp(temp2,"operator")==0) {
	c = G__fgetstream(temp2+8,"(");
      }
    }
#ifdef G__OLDIMPLEMENTATION2157_YET
    if(isspace(c)) {
      /* static member with initialization */
      fsetpos(G__ifile.fp,&pos);
      G__ifile.line_number = store_line_number;
      if(G__dispsource) G__disp_mask=0;
      G__createtemplatememfunc(temp);
      /* skip body of member function template */
      c = G__fignorestream("{;");
      if(';'!=c) c = G__fignorestream("}");
      G__freetemplatearg(targ);
      return;
    }
#endif
    if(';'==c || '='==c) ismemvar=1;
    if('('==c||';'==c
       || '='==c
       ) {
      /*1 template<class T> A<T>::f()           ::f
       *3 template<class T> A<T> A<T>::f()      A<T>::f
       *6 template<class T> A<T> A<T>::v;       A<T>::v
       *6'template<class T> A<T> A<T>::v=0;     A<T>::v
       *7 template<class T> A<T> { }  constructor
       *5 template<class T> A<T> f()            f        */
      p=strchr(temp2,':');
      if(p) {
	c='<';
	if(p!=temp2) {
	  p=strchr(temp2,'<');
	  *p='\0';  /* non constructor/destructor member function */
	  strcpy(temp,temp2);
	}
      }
      else {
	if(temp2[0]) strcpy(temp,temp2);
     }
    }
    else if('<'==c) {
      /* Do nothing */
    }
#ifdef G__OLDIMPLEMENTATION2157_YET
    else if('='==c) {
      /*6'template<class T> A<T> A<T>::v=0;     A<T>::v */
      c = G__fignorestream(";");
      ismemvar=1;
    }
#endif
    else { /* if(strncmp(temp,"::",2)==0) { */
      /*2 template<class T> A<T>::B<T> A<T>::f()  ::B<T>
       *4 template<class T> A<T>::B<T> f()        ::B<T> */
      /* take out keywords const */
      fpos_t posx;
      int linex;
      G__disp_mask = 1000;
      fgetpos(G__ifile.fp,&posx);
      linex = G__ifile.line_number;
      c=G__fgetname(temp,"&*(;<");
      if(0==strcmp(temp,"const")) {
	G__constvar = G__CONSTVAR;
	if(G__dispsource) G__fprinterr(G__serr,"%s",temp);
	if(!isspace(c)) fseek(G__ifile.fp,-1,SEEK_CUR);
      }
      else {
	G__disp_mask = 0;
	fsetpos(G__ifile.fp,&posx);
	G__ifile.line_number = linex;
      }
      c=G__fgetstream(temp,"(;<");
      /* Judge by c? '('  global or '<' member */
    }
    /*
    else {
      p=strchr(temp2,'<');
      if(p) {
	*p = '\0';
	strcpy(temp,temp2);
	c='<';
      }
      else if(isspace(c)&&'\0'==temp[0]) {
	c=G__fgetspace();
      }
    }
    */
  }
 /* template<...> X() in class context could be a ctor. */
  else if (c == '(' && G__def_struct_member && G__tagdefining >= 0 &&
           strcmp (temp, G__struct.name[G__tagdefining]) == 0)
  {
    /*8 template<class T> A(const T& x) { }  constructor 
    *                       ^                            */
    /* Do nothing */
  }
  else if(isspace(c) && strcmp(temp,"operator")==0) {
    temp[8] = ' ';
    c=G__fgetname_template(temp+9,"(");
  }
  else { /* if('<'==c) */
    /* template<..> inline|const type A<T,S>::f() { ... }
     * template<..> inline|const type f(T a,S b) { ... }
     *                               ^              */
    do {
      c=G__fgetname_template(temp,"(<&*");
      if(isspace(c) && strcmp(temp,"operator")==0) {
	c=G__fgetstream(temp+8,"(");
	if('('==c&&0==strcmp(temp,"operator(")) c=G__fgetname(temp+9,"(");
      }
    } while('('!=c && '<'!=c) ;
  }

  /* template<..> type A<T,S>::f() { ... }
   * template<..> type f(T a,S b) { ... }
   *                     ^                   */
  if('<'==c && strcmp(temp,"operator")!=0) {
    /* member function template */
    fsetpos(G__ifile.fp,&pos);
    G__ifile.line_number = store_line_number;
    if(G__dispsource) G__disp_mask=0;
    G__createtemplatememfunc(temp);
    /* skip body of member function template */
    c = G__fignorestream("{;");
    if(';'!=c) c = G__fignorestream("}");
    G__freetemplatearg(targ);
  }
  else {
    if(G__dispsource) G__disp_mask=0;
    /* global function template */
    if(strcmp(temp,"operator")==0) {
      /* in case of operator< operator<= operator<< */
      temp[8]=c; /* operator< */
      c=G__fgetstream(temp+9,"(");
      if (temp[8] == '(') {
        if (c == ')') {
          temp[9] = c;
          c=G__fgetstream(temp+10,"(");
        }
        else {
          G__genericerror("Error: operator() overloading syntax error");
          return;
        }
      }
    }
    G__createtemplatefunc(temp,targ,store_line_number,&pos);
  }
}

/**************************************************************************
* G__templatemaptypename()
*
* separate and evaluate template argument list
**************************************************************************/
static void G__templatemaptypename(string)
char *string;
{
  int tagnum;
#ifdef G__OLDIMPLEMENTATION609_YET
  int typenum;
#endif
  if(strncmp(string,"const",5)==0 && string[5]!=' ') {
    if(
       strcmp(string+5,"int")==0||
       strcmp(string+5,"unsignedint")==0||
       strcmp(string+5,"char")==0||
       strcmp(string+5,"unsignedchar")==0||
       strcmp(string+5,"short")==0||
       strcmp(string+5,"unsignedshort")==0||
       strcmp(string+5,"long")==0||
       strcmp(string+5,"unsignedlong")==0||
       strcmp(string+5,"double")==0||
       strcmp(string+5,"float")==0||
       strcmp(string+5,"int*")==0||
       strcmp(string+5,"unsignedint*")==0||
       strcmp(string+5,"char*")==0||
       strcmp(string+5,"unsignedchar*")==0||
       strcmp(string+5,"short*")==0||
       strcmp(string+5,"unsignedshort*")==0||
       strcmp(string+5,"long*")==0||
       strcmp(string+5,"unsignedlong*")==0||
       strcmp(string+5,"double*")==0||
       strcmp(string+5,"float*")==0
       || G__istypename(string+5)
      ) {
      int len=strlen(string);
      while(len>=5) {
	string[len+1] = string[len];
	--len;
      }
      string[5] = ' ';
      string += 6;
    }
  }
  while(strncmp(string,"const ",6)==0) string+=6;
  if(strcmp(string,"shortint")==0) strcpy(string,"short");
  else if(strcmp(string,"shortint*")==0) strcpy(string,"short*");
  else if(strcmp(string,"longint")==0) strcpy(string,"long");
  else if(strcmp(string,"longint*")==0) strcpy(string,"long*");
  else if(strcmp(string,"longlong")==0) strcpy(string,"long long");
  else if(strcmp(string,"longlong*")==0) strcpy(string,"long long*");
  else if(strcmp(string,"unsignedchar")==0) strcpy(string,"unsigned char");
  else if(strcmp(string,"unsignedchar*")==0) strcpy(string,"unsigned char*");
  else if(strcmp(string,"unsignedint")==0) strcpy(string,"unsigned int");
  else if(strcmp(string,"unsignedint*")==0) strcpy(string,"unsigned int*");
  else if(strcmp(string,"unsignedlong")==0||
	  strcmp(string,"unsignedlongint")==0)
    strcpy(string,"unsigned long");
  else if(strcmp(string,"unsignedlong*")==0||
	  strcmp(string,"unsignedlongint*")==0)
    strcpy(string,"unsigned long*");
  else if(strcmp(string,"unsignedlonglong")==0)
    strcpy(string,"unsigned long long ");
  else if(strcmp(string,"unsignedlonglong*")==0)
    strcpy(string,"unsigned long long*");
  else if(strcmp(string,"unsignedshort")==0||
	  strcmp(string,"unsignedshortint")==0)
    strcpy(string,"unsigned short");
  else if(strcmp(string,"unsignedshort*")==0||
	  strcmp(string,"unsignedshortint*")==0)
    strcpy(string,"unsigned short*");
/* #define G__OLDIMPLEMENTATION787 */
  else {
    char saveref[G__LONGLINE];
    char* p = string + strlen (string);
    while (p > string && (p[-1] == '*' || p[-1] == '&'))
      --p;
    G__ASSERT (strlen (p) < sizeof (saveref));
    strcpy (saveref, p);
    *p = '\0';
    if(-1!=(tagnum=G__defined_typename(string))) {
      char type = G__newtype.type[tagnum];
      int ref = G__newtype.reftype[tagnum];
#ifndef G__OLDIMPLEMENTATION1712
      if(0==strstr(string,"::") && -1!=G__newtype.parent_tagnum[tagnum]) {
	++G__templatearg_enclosedscope;
      }
#endif
      if (G__newtype.tagnum[tagnum] >= 0 &&
	  G__struct.name[G__newtype.tagnum[tagnum]][0] == '$') {
	ref = 0;
	type = tolower (type);
      }
      strcpy (string,G__type2string (type,
				     G__newtype.tagnum[tagnum],
				     -1, ref, 0));
    } else
    if(-1!=(tagnum=G__defined_tagname(string,1))) {
#ifndef G__OLDIMPLEMENTATION1712
      if(0==strstr(string,"::") && -1!=G__struct.parent_tagnum[tagnum]) {
	++G__templatearg_enclosedscope;
      }
#endif
      strcpy(string,G__fulltagname(tagnum,1));
    }
    strcat (string, saveref);
  }
}

/**************************************************************************
* G__expand_def_template_arg()   by Scott Snyder 1997/Oct/17
*
* Returns a malloc'd string.
**************************************************************************/
char* G__expand_def_template_arg (str_in, def_para, charlist)
char* str_in;
struct G__Charlist *charlist;
struct G__Templatearg *def_para;
{
  char *punctuation=" \t\n;:=+-)(*&^%$#@!~'\"\\|][}{/?.>,<";
  int siz_out = strlen (str_in) * 2;
  char* str_out;
  char* temp;
  int iout;
  int iin;
  int single_quote;
  int double_quote;
  char c;
  int isconst=0;

  if (siz_out < 10)
    siz_out = 10;

  temp = (char*) malloc (siz_out + 1);

  str_out = (char*) malloc (siz_out + 1);
  str_out[0] = 0;
  iout = 0;

  iin = 0;

  /* The text has been through the reader once, so we shouldn't
     have to worry about comments.
     We should still be prepared to handle quotes though. */

  single_quote = double_quote = 0;

  do {
    int lreslt;
    char* reslt = temp;

    c = G__getstream (str_in, &iin, temp, punctuation);

    if (*reslt != '\0' && 0 == single_quote && 0 == double_quote) {
      struct G__Charlist* cl = charlist;
      struct G__Templatearg* ta = def_para;

      while (cl && cl->string) {
        G__ASSERT (ta && ta->string);
        if (strcmp (ta->string, reslt) == 0) {
          reslt = cl->string;
          break;
        }
        ta = ta->next;
        cl = cl->next;
      }
    }

    /* ??? Does this handle backslash escapes properly? */
    if('\''==c && 0==double_quote)
      single_quote = single_quote ^ 1 ;
    else if('"'==c && 0==single_quote)
      double_quote = double_quote ^ 1 ;

    lreslt = strlen (reslt);
    if (iout + lreslt + 1 > siz_out) {
      siz_out = (iout + lreslt + 1) * 2;
      str_out = (char*) realloc (str_out, siz_out + 1);
    }

    {
    int rlen = strlen(reslt);
    if(isconst && strncmp(reslt,"const ",6)==0 &&
       rlen>0 && '*'==reslt[rlen-1]) {
      strcpy(str_out+iout,reslt+6);
      strcat (str_out, " const");
      iout += lreslt;
      isconst=0;
    } 
    else {
      strcpy (str_out + iout, reslt);
      iout += lreslt;
      if(strcmp(reslt,"const")==0 && ' '==c) isconst=1;
      else isconst=0;
    }
    }
    str_out[iout++] = c;
  } while (c != '\0');

  str_out[iout] = '\0';

  free (temp);

  return str_out;
}

/**************************************************************************
* G__gettemplatearglist()
*
* separate and evaluate template argument list
**************************************************************************/
int G__gettemplatearglist(paralist,charlist_in,def_para_in ,pnpara
			  ,parent_tagnum
			  )
char *paralist;
struct G__Charlist *charlist_in;
struct G__Templatearg *def_para_in;
int *pnpara;
int parent_tagnum;
{
  struct G__Charlist *charlist = charlist_in;
  struct G__Templatearg *def_para = def_para_in;
  int isrc;
  char string[G__LONGLINE];
  char temp[G__LONGLINE];
  int c;
  G__value buf;
  int searchflag=0;
  int store_tagdefining,store_def_tagnum;

  /**************************************************************
  * explicitly given template argument
  **************************************************************/
#ifndef G__OLDIMPLEMENTATION2180
  if (paralist[0]=='>' && paralist[1]==0) 
     c='>';
  else
     c=','; 
#else
  c=',';
#endif
  isrc=0;
  while(','==c) {
    if('\0'==paralist[0]) break;
    c = G__getstream_template(paralist,&isrc,string,",>\0");
    if(def_para) {
      switch(def_para->type) {
      case G__TMPLT_CLASSARG:
	strcpy(temp,string);
	G__templatemaptypename(temp);
	if(strcmp(temp,string)!=0) {
	  searchflag=1;
	  strcpy(string,temp);
	}
	break;
      case G__TMPLT_TMPLTARG:
	break;
      case G__TMPLT_POINTERARG3:
	if(string[0] && '*'==string[strlen(string)-1])
	  string[strlen(string)-1]='\0';
	else G__genericerror("Error: this template requests pointer arg 3");
      case G__TMPLT_POINTERARG2:
	if(string[0] && '*'==string[strlen(string)-1])
	  string[strlen(string)-1]='\0';
	else G__genericerror("Error: this template requests pointer arg 2");
      case G__TMPLT_POINTERARG1:
	if(string[0] && '*'==string[strlen(string)-1])
	  string[strlen(string)-1]='\0';
	else G__genericerror("Error: this template requests pointer arg 1");
	break;
      default:
	{
	  int store_memberfunc_tagnum = G__memberfunc_tagnum;
	  int store_exec_memberfunc = G__exec_memberfunc;
	  int store_no_exec_compile = G__no_exec_compile;
	  int store_asm_noverflow = G__asm_noverflow;
	  G__no_exec_compile=0;
	  G__asm_noverflow=0;
	  if(-1!=G__tagdefining) {
	    G__exec_memberfunc = 1;
	    G__memberfunc_tagnum = G__tagdefining;
	  }
	  buf = G__getexpr(string);
	  G__no_exec_compile = store_no_exec_compile;
	  G__asm_noverflow = store_asm_noverflow;
	  G__exec_memberfunc = store_exec_memberfunc;
	  G__memberfunc_tagnum = store_memberfunc_tagnum;
	}
	G__string(buf,temp);
	if(strcmp(temp,string)!=0) {
	  searchflag=1;
	  strcpy(string,temp);
	}
	break;
      }
      def_para = def_para->next;
    }
    else {
      G__genericerror("Error: Too many template arguments");
    }
    charlist->string = malloc(strlen(string)+1);
    strcpy(charlist->string,string);
    charlist->next = (struct G__Charlist*)malloc(sizeof(struct G__Charlist));
    charlist->next->next = (struct G__Charlist *)NULL;
    charlist->next->string = (char *)NULL;
    charlist = charlist->next;
    ++(*pnpara);
  }

  /**************************************************************
  * default template argument
  **************************************************************/
  store_tagdefining = G__tagdefining;
  store_def_tagnum = G__def_tagnum;
  if(-1!=parent_tagnum) {
    G__tagdefining = parent_tagnum;
    G__def_tagnum = parent_tagnum;
  }
  if(def_para) {
    while(def_para) {
      if(def_para->default_parameter) {
	strcpy(string,def_para->default_parameter);
        charlist->string = G__expand_def_template_arg (string,def_para_in,
                                                       charlist_in);
        {
	  int len=strlen(charlist->string)*2;
	  /* workaround, G__templatemaptemplatename() overrides malloced mem*/
	  if(len<G__LONGLINE) len=G__LONGLINE;
	  charlist->string=realloc(charlist->string,len+1);
	  G__templatemaptypename(charlist->string);
	  G__ASSERT((int)strlen(charlist->string)<=(int)len);
        }
	charlist->next=(struct G__Charlist*)malloc(sizeof(struct G__Charlist));
	charlist->next->next = (struct G__Charlist *)NULL;
	charlist->next->string = (char *)NULL;
	charlist = charlist->next;
#ifndef G__OLDIMPLEMENTATION1503
	searchflag = 3;
#else
	searchflag = 1;
#endif
      }
      else {
	G__genericerror("Error: Too few template arguments");
      }
      def_para=def_para->next;
    }
  }
  G__tagdefining = store_tagdefining;
  G__def_tagnum = store_def_tagnum;

  return(searchflag);
}

/***********************************************************************
* G__instantiate_templateclass()
*
***********************************************************************/
int G__instantiate_templateclass(tagnamein)
char *tagnamein;
{
  int typenum;
  int tagnum;
  int hash,temp;
  char templatename[G__LONGLINE];
  char *arg;
  struct G__Definedtemplateclass *deftmpclass;
  struct G__Charlist call_para;
#ifdef G__TEMPLATEMEMFUNC
  struct G__Definedtemplatememfunc *deftmpmemfunc;
#endif
  int npara=0;
  int store_tagdefining;
  int store_def_tagnum;
  char atom_name[G__LONGLINE];
  int env_tagnum=G__get_envtagnum();
  int scope_tagnum = -1;
  struct G__inheritance *baseclass;
  int parent_tagnum;
  int store_constvar = G__constvar;
#ifndef G__OLDIMPLEMENTATION1503
  int defarg=0;
#endif
#ifndef G__OLDIMPLEMENTATION1712
  int store_templatearg_enclosedscope;
#endif
#define G__OLDIMPLEMENTATION778
  char *tagname_pass;
  char tagname[G__LONGLINE];
  strcpy(tagname,tagnamein);
  tagname_pass=tagname;

  typenum =G__defined_typename(tagname);
  if(-1!=typenum) return(G__newtype.tagnum[typenum]);

#ifdef G__ASM
#ifndef G__OLDIMPLEMENTATION2124
  if(!G__cintv6) G__abortbytecode();
#else
  G__abortbytecode();
#endif
#endif

  call_para.string=(char*)NULL;
  call_para.next = (struct G__Charlist*)NULL;

  /* separate template name and argument into templatename and arg  */
  strcpy(templatename,tagname);
  arg = strchr(templatename,'<');
  if(arg) {
    *arg='\0';
    ++arg;
  }
  else {
    arg = "";
  }

  /* prepare for using directive scope resolution */
  if(-1!=env_tagnum && G__struct.baseclass[env_tagnum]->basen!=0)
    baseclass = G__struct.baseclass[env_tagnum];
  else
    baseclass = (struct G__inheritance*)NULL;

  /* scope operator resolution, A::templatename<int> ... */
 {
   char *patom;
   char *p;
   strcpy(atom_name,templatename);
   patom = atom_name;
   while( (p=G__find_first_scope_operator(patom)) ) patom = p+2;
   if(patom==atom_name) {
     scope_tagnum = -1;
     G__hash(atom_name,hash,temp)
   }
   else {
     *(patom-2) = 0;
     if(strlen(atom_name)==0||strcmp(atom_name,"::")==0) scope_tagnum = -1;
     else scope_tagnum = G__defined_tagname(atom_name,0);
     p = atom_name;
     while(*patom) *p++ = *patom++;
     *p = 0;
     G__hash(atom_name,hash,temp)
#define G__OLDIMPLEMENTATION1830 /* side effect t1011.h */
   }
 }

  /* search for template class name */
  deftmpclass = &G__definedtemplateclass;
  while(deftmpclass->next) { /* BUG FIX */
    if(hash==deftmpclass->hash && strcmp(atom_name,deftmpclass->name)==0) {
      /* look for ordinary scope resolution */
      if((-1==scope_tagnum&&(-1==deftmpclass->parent_tagnum||
			     env_tagnum==deftmpclass->parent_tagnum))||
	 scope_tagnum==deftmpclass->parent_tagnum) {
	goto exit_loop;
      }
      else if(-1==scope_tagnum) {
	int env_parent_tagnum = env_tagnum;
	if(baseclass) {
	  /* look for using directive scope resolution */
	  for(temp=0;temp<baseclass->basen;temp++) {
	    if(baseclass->basetagnum[temp]==deftmpclass->parent_tagnum) {
	      goto exit_loop;
	    }
	  }
	}
	/* look for enclosing scope resolution */
	while(-1!=env_parent_tagnum) {
	  env_parent_tagnum = G__struct.parent_tagnum[env_parent_tagnum];
	  if(env_parent_tagnum==deftmpclass->parent_tagnum) goto exit_loop;
          if(G__struct.baseclass[env_parent_tagnum]) {
            for(temp=0;temp<G__struct.baseclass[env_parent_tagnum]->basen;temp++) {
              if(G__struct.baseclass[env_parent_tagnum]->basetagnum[temp]==deftmpclass->parent_tagnum) {
                goto exit_loop;
              }
            }
          }
        }
        /* look in global scope (handle for using declaration info */
        for(temp=0;temp<G__globalusingnamespace.basen;temp++) {
          if(G__globalusingnamespace.basetagnum[temp]==deftmpclass->parent_tagnum) {
            goto exit_loop;
          }
	}
      }
    }
    deftmpclass=deftmpclass->next;
  }
 exit_loop:


  /* if no such template, error */
  if(!deftmpclass->next) {
    G__fprinterr(G__serr,"Error: no such template %s",tagname);
    G__genericerror((char*)NULL);
    return(-1);
  }

  if(!deftmpclass->def_fp) {
    G__fprinterr(G__serr,"Limitation: Can't instantiate precompiled template %s"
	    ,tagname);
    G__genericerror(NULL);
    return(-1);
  }

  /* separate and evaluate template argument */
#ifndef G__OLDIMPLEMENTATION1712
  store_templatearg_enclosedscope = G__templatearg_enclosedscope;
  G__templatearg_enclosedscope = 0;
#endif
#ifndef G__OLDIMPLEMENTATION1503
  if((defarg=
      G__gettemplatearglist(arg,&call_para,deftmpclass->def_para,&npara
			  ,deftmpclass->parent_tagnum
			    ))) {
#else
  if(G__gettemplatearglist(arg,&call_para,deftmpclass->def_para,&npara
			  ,-1
			   )) {
#endif
    /* If evaluated template argument is not identical as string to
     * the original argument, recursively call G__defined_tagname()
     * to find actual tagname. */
    int typenum = -1;
#ifndef G__OLDIMPLEMENTATION1712
    int templatearg_enclosedscope=G__templatearg_enclosedscope;
    G__templatearg_enclosedscope=store_templatearg_enclosedscope;
#endif
    if(-1==G__defined_typename(tagname)) {
      typenum=G__newtype.alltype++;
      G__newtype.type[typenum]='u';
      G__newtype.name[typenum]=(char*)malloc(strlen(tagname)+1);
      strcpy(G__newtype.name[typenum],tagname);
      G__newtype.hash[typenum] = strlen(tagname);
      G__newtype.globalcomp[typenum] = G__globalcomp;
      G__newtype.reftype[typenum] = G__PARANORMAL;
      G__newtype.nindex[typenum] = 0;
      G__newtype.index[typenum] = (int*)NULL;
      G__newtype.iscpplink[typenum] = G__NOLINK;
    }
    G__cattemplatearg(tagname,&call_para);
    tagnum = G__defined_tagname(tagname,1);
#ifndef G__OLDIMPLEMENTATION1867
    G__settemplatealias(tagnamein,tagname,tagnum,&call_para
			,deftmpclass->def_para,templatearg_enclosedscope);
#endif
    if(-1!=typenum) {
      G__newtype.tagnum[typenum] = tagnum;
#ifndef G__OLDIMPLEMENTATION1712
      if(templatearg_enclosedscope) {
	G__newtype.parent_tagnum[typenum] = G__get_envtagnum();
      }
      else {
	G__newtype.parent_tagnum[typenum] = G__struct.parent_tagnum[tagnum];
      }
#else
      G__newtype.parent_tagnum[typenum] = G__struct.parent_tagnum[tagnum];
#endif
#ifndef G__OLDIMPLEMENTATION1503
      if(3==defarg) G__struct.defaulttypenum[tagnum] = typenum;
#endif
    }
    G__freecharlist(&call_para);
    return(tagnum);
  }

  if(-1!=scope_tagnum
     || ':'==templatename[0]
     ) {
    int i=0;
    char *p = strrchr(templatename,':');
    tagname_pass += (p-templatename)+1;
    while(*p) templatename[i++] = *(++p);
  }

  /* resolve template specialization */
  if(deftmpclass->specialization) {
    deftmpclass = G__resolve_specialization(arg,deftmpclass,&call_para);
  }

  /* store tagnum */
  tagnum = G__struct.alltag;
  store_tagdefining = G__tagdefining;
  store_def_tagnum = G__def_tagnum;
  G__def_tagnum = G__tagdefining = deftmpclass->parent_tagnum;

  /* string substitution and parse substituted template class definition */
  G__replacetemplate(templatename,tagname_pass,&call_para
		     ,deftmpclass->def_fp
		     ,deftmpclass->line
		     ,deftmpclass->filenum
		     ,&(deftmpclass->def_pos)
		     ,deftmpclass->def_para
		     ,deftmpclass->isforwarddecl?2:1
		     ,npara
		     ,deftmpclass->parent_tagnum
		     );

#ifdef G__TEMPLATEMEMFUNC
  parent_tagnum = deftmpclass->parent_tagnum;
  while(-1!=parent_tagnum && 'n'!=G__struct.type[parent_tagnum])
    parent_tagnum = G__struct.parent_tagnum[parent_tagnum];
  deftmpmemfunc= &(deftmpclass->memfunctmplt);
  while(deftmpmemfunc->next) {
    G__replacetemplate(templatename,tagname_pass,&call_para
		       ,deftmpmemfunc->def_fp
		       ,deftmpmemfunc->line
		       ,deftmpmemfunc->filenum
		       ,&(deftmpmemfunc->def_pos)
		       ,deftmpclass->def_para
		       ,0
		       ,npara
		       ,parent_tagnum
		       );
    deftmpmemfunc=deftmpmemfunc->next;
  }
#endif /* G__TEMPLATEFUNC */

  if(tagnum<G__struct.alltag && G__struct.name[tagnum] &&
     strcmp(tagname,G__struct.name[tagnum])!=0) {
    char *p1 = strchr(tagname,'<');
    char *p2 = strchr(G__struct.name[tagnum],'<');
    if(p1 && p2 && (p1-tagname)==(p2-G__struct.name[tagnum]) &&
       0==strncmp(tagname,G__struct.name[tagnum],p1-tagname)) {
      free((void*)G__struct.name[tagnum]);
      G__struct.name[tagnum] = (char*)malloc(strlen(tagname)+1);
      strcpy(G__struct.name[tagnum],tagname);
      G__struct.hash[tagnum] = strlen(tagname);
    }
  }

  tagnum = G__defined_tagname(tagname,2);
  if(-1!=tagnum) {
    if(deftmpclass->instantiatedtagnum) {
      G__IntList_addunique(deftmpclass->instantiatedtagnum,tagnum);
    }
    else {
      deftmpclass->instantiatedtagnum=G__IntList_new(tagnum,NULL);
    }
  }

  G__def_tagnum = store_def_tagnum;
  G__tagdefining = store_tagdefining;
  G__constvar = store_constvar;

  /* free template argument lisst */
  G__freecharlist(&call_para);

  /* return instantiated class template id */
  return(tagnum);
}

/**************************************************************************
*
**************************************************************************/
#define SET_READINGFILE               \
    fgetpos(G__mfp,&out_pos);         \
    fsetpos(G__ifile.fp,&in_pos)
#define SET_WRITINGFILE               \
    fgetpos(G__ifile.fp,&in_pos);     \
    fsetpos(G__mfp,&out_pos)

/**************************************************************************
* G__replacetemplate()
*
* Replace template string and prerun
*
**************************************************************************/
void G__replacetemplate(templatename,tagname,callpara
			,def_fp,line,filenum,pdef_pos,def_para,isclasstemplate
			,npara
			,parent_tagnum
			)
char *templatename;
char *tagname;
struct G__Charlist *callpara;
FILE *def_fp;
int line;
int filenum;
fpos_t *pdef_pos;
struct G__Templatearg *def_para;
int isclasstemplate;
int npara;
int parent_tagnum;
{
  fpos_t store_mfpos;
  int store_mfline;
  fpos_t orig_pos;
  fpos_t pos;
  int c,c2;
  int mparen;
  char symbol[G__LONGLINE];
  char *punctuation=" \t\n;:=+-)(*&^%$#@!~'\"\\|][}{/?.>,<";
  int double_quote=0,single_quote=0;
  struct G__input_file store_ifile;
  int store_prerun;
  int store_tagnum,store_def_tagnum;
  int store_tmplt_def_tagnum;
  int store_tagdefining,store_def_struct_member;
  int store_var_type;
  int store_breaksignal;
  int store_no_exec_compile;
  int store_asm_noverflow;
  int store_func_now;
  int store_func_page;
  int store_decl;
  int store_asm_wholefunction;
  int store_reftype;
  int isnew=0;
  struct G__ifunc_table *store_ifunc;
  int slash=0;
  fpos_t out_pos,in_pos;
  fpos_t const_pos;
  char const_c = 0;
  int store_memberfunc_tagnum;
  int store_globalcomp;

  /*******************************************************************
   * open macro and template substitution file and get ready for
   * template instantiation
   *******************************************************************/
  /* store restard position, used later in this function */
  if(G__ifile.fp) fgetpos(G__ifile.fp,&orig_pos);

  /* get tmpfile file pinter */
  if(G__mfp==NULL) {
    G__openmfp();
    fgetpos(G__mfp,&G__nextmacro);
    G__mline=1;
    store_mfline=0;
  }
  else {
    fgetpos(G__mfp,&store_mfpos);
    store_mfline=G__mline;
    fsetpos(G__mfp,&G__nextmacro);
  }

  if(G__dispsource) {
    G__fprinterr(G__serr,"\n!!!Instantiating template %s\n",tagname);
  }

  /* print out header */
  ++G__mline;
  fprintf(G__mfp,"// template %s  FILE:%s LINE:%d\n"
	  ,tagname ,G__ifile.name,G__ifile.line_number);
  if(G__dispsource) {
    G__fprinterr(G__serr,"// template %s  FILE:%s LINE:%d\n"
	    ,tagname ,G__ifile.name,G__ifile.line_number);
  }

  fgetpos(G__mfp,&pos);

  /* set file pointer and position */
  store_ifile = G__ifile;
  G__ifile.fp = def_fp;
  G__ifile.line_number = line;
  G__ifile.filenum = filenum;
  in_pos = *pdef_pos;

  /* output file position indicator */
  ++G__mline;
  fprintf(G__mfp,"# %d \"%s\"\n"
	  ,G__ifile.line_number,G__srcfile[G__ifile.filenum].filename);
  if(G__dispsource) {
    G__fprinterr(G__serr,"# %d \"%s\"\n"
	    ,G__ifile.line_number,G__srcfile[G__ifile.filenum].filename);
  }

  /*******************************************************************
   * read template definition and substitute template arguments
   *******************************************************************/

  /* We are always ignoring the :: when they are alone (and thus specify
     the global name space, we also need to ignore them here! */
  if (strncmp(templatename,"::",2)==0) {
    templatename += 2;
  }  
  if (strncmp(tagname,"::",2)==0) {
    tagname += 2;
  }  

  /* read definition and substitute */
  mparen=0;
  while(1) {
    G__disp_mask = 10000;
    SET_READINGFILE; /* ON777 */
    c = G__fgetstream(symbol,punctuation);
    SET_WRITINGFILE; /* ON777 */
    if('~'==c) isnew=1;
    else if(','==c) isnew=0;
    else if(';'==c) {
      isnew = 0;
      const_c = 0;
    }
    if('\0' != symbol[0]) {
      if(0==double_quote && 0==single_quote) {
        if(isspace(c)) {
          c2=c;
            SET_READINGFILE; /* ON777 */
            while(isspace(c=G__fgetc())){
               if (c=='\n') {
                  /* strcat(symbol,"\n");   BAD  */
                  /* if (c=='\n') c2='\n'; Fix by Philippe */
                  break;  /* Fix by Masa Goto */
               }
            }
            if('<'!=c) {
               fseek(G__ifile.fp,-1,SEEK_CUR);
               c=c2;
            }
            SET_WRITINGFILE; /* ON777 */
         }
         if(strcmp("new",symbol)==0) isnew=1;
        if(strcmp("operator",symbol)==0) isnew=1;
        if(G__templatesubstitute(symbol,callpara,def_para,templatename
           ,tagname,c,npara,isnew) && '>'!=c) {
              char ignorebuf[G__LONGLINE];
              SET_READINGFILE; /* ON777 */
              c=G__fgetstream(ignorebuf,">");
              SET_WRITINGFILE; /* ON777 */
              G__ASSERT('>'==c);
              c='>';
           }      
      }
      if(const_c && '*'==symbol[strlen(symbol)-1]) {
         fsetpos(G__mfp,&const_pos);
         fprintf(G__mfp,"%s",symbol);
         fprintf(G__mfp," const%c",const_c); /* printing %c is not perfect */
         const_c = 0;
      }
      else if(const_c&&(strstr(symbol,"*const")||strstr(symbol,"* const"))) {
         fsetpos(G__mfp,&const_pos);
         fprintf(G__mfp,"%s",symbol);
         fprintf(G__mfp,"%c",const_c); /* printing %c is not perfect */
         const_c = 0;
      }
      else {
         if(';'!=c && strcmp("const",symbol)==0) {
            const_c = c;
            fgetpos(G__mfp,&const_pos);
         }
         else {
            const_c = 0;
         }
         fprintf(G__mfp,"%s",symbol);
      }
      if(G__dispsource) G__fprinterr(G__serr,"%s",symbol);
    }

    if(1==slash) {
       slash=0;
       if('/'==c && 0==symbol[0] && 0==single_quote && 0==double_quote) {
          SET_READINGFILE; /* ON777 */
          G__fgetline(symbol);
          SET_WRITINGFILE; /* ON777 */
          fprintf(G__mfp,"/%s\n",symbol);
          if(G__dispsource) G__fprinterr(G__serr,"/%s\n",symbol);
          ++G__mline;
          continue;
       }
       else if('*'==c && 0==symbol[0] && 0==single_quote && 0==double_quote) {
          fprintf(G__mfp,"/\n");
          if(G__dispsource) G__fprinterr(G__serr,"/\n");
          ++G__mline;
          SET_READINGFILE; /* ON777 */
          G__skip_comment();
          SET_WRITINGFILE; /* ON777 */
          continue;
       }
    }

    if(0==single_quote && 0==double_quote) {
       if('{'==c) ++mparen;
       else if('}'==c) {
          --mparen;
          if(0==mparen) {
             fputc(c,G__mfp);
#ifndef G__OLDIMPLEMENTATION1485
             if(G__dispsource) G__fputerr(c);
#else
             if(G__dispsource) fputc(c,G__serr);
#endif
             break;
          }
       }
       else if(';'==c && 0==mparen) break;
    }

    if('\''==c && 0==double_quote)
       single_quote = single_quote ^ 1 ;

    else if('"'==c && 0==single_quote)
       double_quote = double_quote ^ 1 ;

    if('/'==c) slash=1;

    fputc(c,G__mfp);
#ifndef G__OLDIMPLEMENTATION1485
    if(G__dispsource) G__fputerr(c);
#else
    if(G__dispsource) fputc(c,G__serr);
#endif
    if('\n'==c||'\r'==c) ++G__mline;
  }

  if(2==isclasstemplate) {
     fprintf(G__mfp,";");
     if(G__dispsource) G__fprinterr(G__serr,";");
  }
  else if(1==isclasstemplate
     && ';'!=c
     ) {
        SET_READINGFILE; /* ON777 */
        G__fgetstream(symbol,";");
        const_c = 0;
        SET_WRITINGFILE; /* ON777 */
        fprintf(G__mfp,"%s ;",symbol);
        if(G__dispsource) G__fprinterr(G__serr,"%s ;",symbol);
     }
  else if(';'==c) {
     fputc(c,G__mfp);
#ifndef G__OLDIMPLEMENTATION1485
     if(G__dispsource) G__fputerr(c);
#else
     if(G__dispsource) fputc(c,G__serr);
#endif
  }
  fputc('\n',G__mfp);
#ifndef G__OLDIMPLEMENTATION1485
  if(G__dispsource) G__fputerr('\n');
#else
  if(G__dispsource) fputc('\n',G__serr);
#endif
  ++G__mline;

  /* finish string substitution */
  G__disp_mask=0;
  fgetpos(G__mfp,&G__nextmacro);
  fflush(G__mfp);

  /*******************************************************************
  * rewind tmpfile and parse template class or function
  ********************************************************************/
  if(G__dispsource) {
     G__fprinterr(G__serr,"!!! Reading template %s\n",tagname);
  }

  fsetpos(G__mfp,&pos);
  G__ifile.fp=G__mfp;

  store_prerun = G__prerun;
  store_tagnum = G__tagnum;
  store_def_tagnum = G__def_tagnum;
  store_tagdefining = G__tagdefining;
  store_tmplt_def_tagnum = G__tmplt_def_tagnum;
  store_def_struct_member = G__def_struct_member;
  store_var_type = G__var_type;
  store_breaksignal=G__breaksignal;
  store_no_exec_compile = G__no_exec_compile;
  store_asm_noverflow = G__asm_noverflow;
  store_func_now=G__func_now;
  store_func_page=G__func_page;
  store_decl=G__decl;
  store_ifunc = G__p_ifunc;
  store_asm_wholefunction = G__asm_wholefunction;
  store_reftype=G__reftype;
  store_memberfunc_tagnum = G__memberfunc_tagnum;
  store_globalcomp = G__globalcomp;

  G__prerun=1;
  G__tagnum = -1;
  G__tmplt_def_tagnum = G__def_tagnum;
  /* instantiated template objects in scope that template is declared */
  G__def_tagnum = parent_tagnum;
  G__tagdefining = parent_tagnum;
  G__def_struct_member = (parent_tagnum != -1);
  if(G__exec_memberfunc) G__memberfunc_tagnum = parent_tagnum;
  G__var_type = 'p';
  G__breaksignal=0;
  G__abortbytecode(); /* This has to be 'suspend', indeed. */
  G__no_exec_compile=0;
  G__func_now = -1;
  G__func_page = 0;
  G__decl=0;
  G__p_ifunc = &G__ifunc;
  G__asm_wholefunction = 0;
  G__reftype=G__PARANORMAL;

  G__exec_statement();

  G__func_now=store_func_now;
  G__func_page=store_func_page;
  G__decl=store_decl;
  G__ASSERT(0==G__decl || 1==G__decl);
  G__p_ifunc = store_ifunc;
  G__asm_noverflow=store_asm_noverflow;
  G__no_exec_compile=store_no_exec_compile;
  G__prerun=store_prerun;
  G__tagnum=store_tagnum;
  G__tmplt_def_tagnum = store_tmplt_def_tagnum;
  G__def_tagnum=store_def_tagnum;
  G__tagdefining = store_tagdefining;
  G__def_struct_member = store_def_struct_member;
  G__var_type=store_var_type;
  G__breaksignal=store_breaksignal;
  G__asm_wholefunction = store_asm_wholefunction;
  G__reftype=store_reftype;
  G__memberfunc_tagnum = store_memberfunc_tagnum;
  G__globalcomp = store_globalcomp;

  /* restore input file */
  G__ifile = store_ifile;
  if(G__ifile.filenum>=0)
     G__security = G__srcfile[G__ifile.filenum].security;
  else
     G__security = G__SECURE_LEVEL0;
  /* protect the case when template is instantiated from command line */
  if(G__ifile.fp) fsetpos(G__ifile.fp,&orig_pos);

  if(G__dispsource) {
     G__fprinterr(G__serr,"\n!!!Complete instantiating template %s\n",tagname);
  }

  if(store_mfline) fsetpos(G__mfp,&store_mfpos);
}

/**************************************************************************
* G__templatesubstitute()
*
* Substitute macro argument
*
**************************************************************************/
int G__templatesubstitute(symbol,callpara,defpara,templatename,tagname,c,npara
                          ,isnew
                         )
char *symbol;
struct G__Charlist *callpara;
struct G__Templatearg *defpara;
char *templatename;
char *tagname;
int c;
int npara;
int isnew;
{
  int flag=0;
  static int state=0;

  /* template name substitution */
  if(strcmp(symbol,templatename)==0) {
    if('<'!=c) {
      strcpy(symbol,tagname);
      state=0;
      return(flag);
    }
    else {
      state=1;
      return(flag);
    }
  }

  while(defpara) {
    if(strcmp(defpara->string,symbol)==0) {
      if(callpara->string) {
	strcpy(symbol,callpara->string);
      }
      else if(defpara->default_parameter) {
	strcpy(symbol,defpara->default_parameter);
      }
      else {
	G__fprinterr(G__serr,"Error: template argument for %s missing"
		,defpara->string);
	G__genericerror((char*)NULL);
      }
      if('('==c && symbol[0] &&
         0==isnew &&
 	 ('*'==symbol[strlen(symbol)-1] || strchr(symbol,' ') ||  
	  strchr(symbol,'<') )
	 ) {
	char temp[G__LONGLINE];
	strcpy(temp,symbol);
	sprintf(symbol,"(%s)",temp);
      }
      if(state) {
	if(state==npara 
	   && '*'!=c
	   ) flag=1;
	++state;
      }

      break;
    }
    else {
      state=0;
    }
    defpara = defpara->next;
    if(callpara) callpara=callpara->next;
  }

  /* this is only workaround for STL Allocator */
  if(strcmp(symbol,"Allocator")==0) strcpy(symbol,G__Allocator);

  return(flag);
}

/**************************************************************************
* G__freedeftemplateclass()
**************************************************************************/
void G__freedeftemplateclass(deftmpclass)
struct G__Definedtemplateclass *deftmpclass;
{
  if(deftmpclass->next) {
    G__freedeftemplateclass(deftmpclass->next);
    free((void*)deftmpclass->next);
    deftmpclass->next = (struct G__Definedtemplateclass *)NULL;
  }
  if(deftmpclass->spec_arg) {
    G__freetemplatearg(deftmpclass->spec_arg);
    deftmpclass->spec_arg = (struct G__Templatearg*)NULL;
  }
  if(deftmpclass->specialization) {
    G__freedeftemplateclass(deftmpclass->specialization);
    free((void*)deftmpclass->specialization);
    deftmpclass->specialization=(struct G__Definedtemplateclass*)NULL;
  }
  G__freetemplatearg(deftmpclass->def_para);
  deftmpclass->def_para=(struct G__Templatearg *)NULL;
  if(deftmpclass->name) {
    free((void*)deftmpclass->name);
    deftmpclass->name=(char*)NULL;
  }
#ifdef G__TEMPLATEMEMFUNC
  G__freetemplatememfunc(&(deftmpclass->memfunctmplt));
#endif
  G__IntList_free(deftmpclass->instantiatedtagnum);
  deftmpclass->instantiatedtagnum=(struct G__IntList*)NULL;
}

#ifdef G__TEMPLATEMEMFUNC
/**************************************************************************
* G__freetemplatememfunc()
**************************************************************************/
void G__freetemplatememfunc(memfunctmplt)
struct G__Definedtemplatememfunc *memfunctmplt;
{
  if(memfunctmplt->next) {
    G__freetemplatememfunc(memfunctmplt->next);
    free((void*)memfunctmplt->next);
    memfunctmplt->next=(struct G__Definedtemplatememfunc *)NULL;
  }
}
#endif

/**************************************************************************
* G__freetemplatearg()
**************************************************************************/
void G__freetemplatearg(def_para)
struct G__Templatearg *def_para;
{
  if(def_para) {
    if(def_para->next) G__freetemplatearg(def_para->next);
    if(def_para->string) free((void*)def_para->string);
    if(def_para->default_parameter) free((void*)def_para->default_parameter);
    free((void*)def_para);
  }
}



#ifdef G__TEMPLATEFUNC
/***********************************************************************
* G__gettemplatearg()
*
*  search matches for template argument
***********************************************************************/
char *G__gettemplatearg(n,def_para)
int n;
struct G__Templatearg *def_para;
{
  /* char *result; */
  int i;
  G__ASSERT(def_para);
  for(i=1;i<n;i++) {
    if(def_para->next) def_para = def_para->next;
  }
  return(def_para->string);
}

/***********************************************************************
* G__istemplatearg()
*
*  search matches for template argument
***********************************************************************/
int G__istemplatearg(paraname,def_para)
char *paraname;
struct G__Templatearg *def_para;
{
  int result=1;
  while(def_para) {
    if(strcmp(def_para->string,paraname)==0) return(result);
    def_para = def_para->next;
    ++result;
  }
  return(0);
}


/***********************************************************************
* G__checkset_charlist()
*
* Check and set actual template argument
***********************************************************************/
int G__checkset_charlist(typename,pcall_para,narg
			 ,ftype
			 )
char *typename;
struct G__Charlist *pcall_para;
int narg;
int ftype;
{
  int i;
  for(i=1;i<narg;i++) {
    if(!pcall_para->next) {
      pcall_para->next = (struct G__Charlist*)malloc(sizeof(struct G__Charlist));
      pcall_para->next->next = (struct G__Charlist*)NULL;
      pcall_para->next->string = (char*)NULL;
    }
    pcall_para = pcall_para->next;
  }

  if(pcall_para->string) {
    if('U'==ftype) {
      int len=strlen(typename);
      if(len && '*'==typename[len-1]) {
	typename[len-1] = '\0';
	if(strcmp(typename,pcall_para->string)==0) {
	  typename[len-1] = '*';
	  return(1);
	}
	typename[len-1] = '*';
      }
    }
    if(strcmp(typename,pcall_para->string)==0) return(1);
    else                                       return(0);
  }
  pcall_para->string = (char*)malloc(strlen(typename)+1);
  strcpy(pcall_para->string,typename);

  if('U'==ftype) {
    int len=strlen(typename);
    if(len && '*'==typename[len-1]) {
      pcall_para->string[len-1] = '\0';
    }
  }

  return(1);
}

/***********************************************************************
* G__matchtemplatefunc()
*
* Test if given function arguments and template function arguments
* matches.
***********************************************************************/
int G__matchtemplatefunc(deftmpfunc,libp,pcall_para,funcmatch)
struct G__Definetemplatefunc *deftmpfunc;
struct G__param *libp;
struct G__Charlist *pcall_para;
int funcmatch;
{
  int fparan,paran;
  int ftype,type;
  int ftagnum,tagnum;
  int ftypenum,typenum;
  int freftype,reftype,ref;
  /* int fparadefault; */
  int fargtmplt;
  int i;
  char paratype[G__LONGLINE];
  int *fntarg;
  int fnt;
  char **fntargc;

  fparan = deftmpfunc->func_para.paran;
  paran = libp->paran;

  /* more argument in calling function, unmatch */
  if(paran>fparan) return(0);
  if(fparan>paran) {
    if(!deftmpfunc->func_para.paradefault[paran]) return(0);
  }

  for(i=0;i<paran;i++) {
    /* get template information for simplicity */
    ftype = deftmpfunc->func_para.type[i];
    ftagnum = deftmpfunc->func_para.tagnum[i];
    ftypenum = deftmpfunc->func_para.typenum[i];
    freftype = deftmpfunc->func_para.reftype[i];
    fargtmplt = deftmpfunc->func_para.argtmplt[i];
    fntarg = deftmpfunc->func_para.ntarg[i];
    fnt = deftmpfunc->func_para.nt[i];
    fntargc = deftmpfunc->func_para.ntargc[i];

    /* get parameter information for simplicity */
    type = libp->para[i].type;
    tagnum = libp->para[i].tagnum;
    typenum = libp->para[i].typenum;
    ref = libp->para[i].ref;
    if(
       'u'==libp->para[i].type ||
       isupper(libp->para[i].type))
      reftype=libp->para[i].obj.reftype.reftype;
    /*else if(ref) reftype=G__PARAREFERENCE;*/
    else reftype=G__PARANORMAL;

    /* match parameter */
    if(-1==fargtmplt) {
      char *p;
      char *cntarg[20];
      int cnt=0;
      int j;
      int basetagnum;
      int basen;
      int bn;
      int bmatch;
      /* fixed argument type */
      if(type==ftype&&ftagnum==tagnum&&(0==freftype||ref
					||freftype==reftype
					)) {
	continue;
      }
      /* assuming that the argument type is a template class */
      if('u'!=type || -1==tagnum) return(0);
      /* template argument  (T<E> a) */
      basen = G__struct.baseclass[tagnum]->basen;
      bn = -1;
      basetagnum = tagnum;
      bmatch=0;
      while(0==bmatch && bn<basen) {
	int nest=0;
	cnt=0;
	if(bn>=0) basetagnum = G__struct.baseclass[tagnum]->basetagnum[bn];
	++bn;
	bmatch=1;
	strcpy(paratype,G__fulltagname(basetagnum,0));
	cntarg[cnt++]=paratype;  /* T <x,E,y> */
	p = strchr(paratype,'<');
	if(!p) {/* unmatch */
	  if(G__EXACT==funcmatch) return(0);
	  bmatch = 0;
	  continue;
	}
	do {       /*  T<x,E,y>     */
	  *p = 0;  /*   ^ ^ ^       */
	  ++p;     /*    ^ ^ ^      */
	  cntarg[cnt++] = p;
	  while((0!=(*p) && ','!=(*p) && '>'!=(*p)) || nest) {
	    if('<'==(*p)) ++nest;
	    else if('>'==(*p)) --nest;
	    ++p;
	  }
	} while(','==(*p));
	if('>'==(*p)) *p = 0;  /* the last '>' */
	if(' '== (*(p-1))) *(p-1) = 0;
	/* match template argument */
	if(fnt>cnt) {/* unmatch */
	  if(G__EXACT==funcmatch) return(0);
	  bmatch = 0;
	  continue;
	}
	else if(fnt<cnt) {/* unmatch, check default template argument */
	  int ix;
	  struct G__Templatearg *tmparg;
	  struct G__Definedtemplateclass *tmpcls;
	  tmpcls=G__defined_templateclass(paratype);
	  if(!tmpcls) {
	    if(G__EXACT==funcmatch) return(0);
	    bmatch = 0;
	    continue;
	  }
	  tmparg = tmpcls->def_para;
	  for(ix=0;ix<fnt-1&&tmparg;ix++) tmparg=tmparg->next;
	  /* Note: This one is a correct behavior. Current implementation is
           * workaround for old and new STL mixture
           *  if(!tmparg || !tmparg->default_parameter) { */
	  if(tmparg && !tmparg->default_parameter) {
	    if(G__EXACT==funcmatch) return(0);
	    bmatch = 0;
	    continue;
	  }
	}
	for(j=0;j<fnt&&j<cnt;j++) {
	  if(fntarg[j]) {
	    if(G__checkset_charlist(cntarg[j],pcall_para,fntarg[j],ftype)) {
	      /* match or newly set template argument */
	    }
	    else {
	      /* template argument is already set to different type, unmatch */
	      if(G__EXACT==funcmatch) return(0);
	      bmatch = 0;
	      break;
	    }
	  }
	  else if((char*)NULL==fntargc[j]||strcmp(cntarg[j],fntargc[j])!=0) {
	    if(G__EXACT==funcmatch) return(0);
	    bmatch = 0;
	    break;
	  }
	}
      }
      if(0==bmatch) return(0);
    }
    else if(fargtmplt) {
      if(isupper(ftype) && islower(type)) {
	/* umnatch , pointer level f(T* x) <= f(1) */
	return(0);
      }
      /* template argument  (T a) */
      if(G__PARAREFERENCE==reftype)
	strcpy(paratype,G__type2string(type,tagnum,-1,0,0));
      else
	strcpy(paratype,G__type2string(type,tagnum,-1,reftype,0));
      if(strncmp(paratype,"class ",6)==0) {
	int j=0,i=6;
	do {
	  paratype[j++] = paratype[i];
	} while(paratype[i++]);
      }
      else if(strncmp(paratype,"struct ",7)==0) {
	int j=0,i=7;
	do {
	  paratype[j++] = paratype[i];
	} while(paratype[i++]);
      }
      if(G__checkset_charlist(paratype,pcall_para,fargtmplt,ftype)) {
	/* match or newly set template argument */
      }
      else {
	/* template argument is already set to different type, unmatch */
	return(0);
      }
    }
    else {
      /* fixed argument type */
      if(type==ftype&&ftagnum==tagnum&&(0==freftype||ref)) {
	/* match, check next */
      }
      else if(G__EXACT!=funcmatch &&
	      (('u'==type&&'u'==ftype)||('U'==type&&'U'==ftype)) &&
	      (-1!=G__ispublicbase(tagnum,ftagnum,libp->para[i].obj.i))) {
	/* match with conversion */
      }
      else {
	/* unmatch */
	return(0);
      }
    }
  }

  return(1); /* All parameters match */

}

/***********************************************************************
* G__freetemplatefunc()
*
*
***********************************************************************/
void G__freetemplatefunc(deftmpfunc)
struct G__Definetemplatefunc *deftmpfunc;
{
  int i;
  if(deftmpfunc->next) {
    G__freetemplatefunc(deftmpfunc->next);
    free((void*)deftmpfunc->next);
    deftmpfunc->next = (struct G__Definetemplatefunc*)NULL;
  }
  if(deftmpfunc->def_para) {
    G__freetemplatearg(deftmpfunc->def_para);
    deftmpfunc->def_para = (struct G__Templatearg*)NULL;
  }
  if(deftmpfunc->name) {
    free((void*)deftmpfunc->name);
    deftmpfunc->name=(char*)NULL;
    for(i=0;i<G__MAXFUNCPARA;i++) {
      if(deftmpfunc->func_para.ntarg[i]) {
	int j;
	for(j=0;j<deftmpfunc->func_para.nt[i];j++) {
	  if(deftmpfunc->func_para.ntargc[i][j])
	    free(deftmpfunc->func_para.ntargc[i][j]);
	}
	free((void*)deftmpfunc->func_para.ntargc[i]);
	deftmpfunc->func_para.ntargc[i]=(char**)NULL;
	free((void*)deftmpfunc->func_para.ntarg[i]);
	deftmpfunc->func_para.ntarg[i]=(int*)NULL;
	deftmpfunc->func_para.nt[i]=0;
      }
    }
  }
}

/***********************************************************************
* G__templatefunc()
*
* Search matching template function, search by name then parameter.
* If match found, expand template, parse as pre-run and execute it.
***********************************************************************/
int G__templatefunc(result,funcname,libp,hash,funcmatch)
G__value *result;
char *funcname;
struct G__param *libp;
int hash;
int funcmatch;
{
  struct G__Definetemplatefunc *deftmpfunc;
  struct G__Charlist call_para;
  int store_exec_memberfunc;
  struct G__ifunc_table *ifunc;
  char *pexplicitarg;
  int env_tagnum=G__get_envtagnum();
  struct G__inheritance *baseclass;
  int store_friendtagnum = G__friendtagnum;
  /* int i; */

  if(-1!=env_tagnum && G__struct.baseclass[env_tagnum]->basen!=0)
    baseclass = G__struct.baseclass[env_tagnum];
  else
    baseclass = (struct G__inheritance*)NULL;

  if(/* 0==libp->paran && */ (pexplicitarg=strchr(funcname,'<'))) {
    /* funcname="f<int>" ->  funcname="f" , pexplicitarg="int>" */
    int tmp=0;
    *pexplicitarg = 0;
    if(G__defined_templateclass(funcname)) {
      *pexplicitarg = '<';
      pexplicitarg = (char*)NULL;
    }
    else {
      ++pexplicitarg;
      G__hash(funcname,hash,tmp);
    }
  }
  /* else pexplicitarg==NULL */

  call_para.string = (char*)NULL;
  call_para.next = (struct G__Charlist*)NULL;
  deftmpfunc = &G__definedtemplatefunc;

  /* Search matching template function name */
  while(deftmpfunc->next) {
    G__freecharlist(&call_para);
    if(deftmpfunc->hash==hash && strcmp(deftmpfunc->name,funcname)==0 &&
       (G__matchtemplatefunc(deftmpfunc,libp,&call_para,funcmatch)
	|| pexplicitarg
	)) {

      if(-1!=deftmpfunc->parent_tagnum &&
	 env_tagnum!=deftmpfunc->parent_tagnum) {
	if(baseclass) {
	  int temp;
	  for(temp=0;temp<baseclass->basen;temp++) {
	    if(baseclass->basetagnum[temp]==deftmpfunc->parent_tagnum) {
	      goto match_found;
	    }
	  }
          /* look in global scope (handle for using declaration info */
          for(temp=0;temp<G__globalusingnamespace.basen;temp++) {
            if(G__globalusingnamespace.basetagnum[temp]==deftmpfunc->parent_tagnum) {
              goto match_found;
            }
          }
	}
	deftmpfunc = deftmpfunc->next;
	continue;
      }
    match_found:

      G__friendtagnum = deftmpfunc->friendtagnum;

      if(pexplicitarg) {
	int npara=0;
	G__gettemplatearglist(pexplicitarg,&call_para
			      ,deftmpfunc->def_para,&npara
			      ,-1
			      );
      }

      if(pexplicitarg) {
	int tmp=0;
	char *p = pexplicitarg-1;
	pexplicitarg = (char*)malloc(strlen(funcname)+1);
	strcpy(pexplicitarg,funcname);
	*p = '<';
	G__hash(funcname,hash,tmp);
      }
      else {
	pexplicitarg = "";
      }

      /* matches funcname and parameter,
       * then expand the template and parse as prerun */
      G__replacetemplate(
			 pexplicitarg
			 ,funcname
			 ,&call_para /* needs to make this up */
			 ,deftmpfunc->def_fp
			 ,deftmpfunc->line
			 ,deftmpfunc->filenum
			 ,&(deftmpfunc->def_pos)
			 ,deftmpfunc->def_para
			 ,0
			 ,SHRT_MAX /* large enough number */
			 ,deftmpfunc->parent_tagnum
			 );

      G__friendtagnum = store_friendtagnum;

      if(pexplicitarg && pexplicitarg[0]) {
	free((void*)pexplicitarg);
      }

      /* call the expanded template function */
      store_exec_memberfunc = G__exec_memberfunc;
      if(-1!=deftmpfunc->parent_tagnum
	 ) {
	/* Need to do something for member function template */
	ifunc = G__struct.memfunc[deftmpfunc->parent_tagnum];
      }
      else {
	G__exec_memberfunc=0;
	ifunc = &G__ifunc;
      }
      if(G__interpret_func(result,funcname,libp,hash
			   ,ifunc
			   ,funcmatch
			   ,G__TRYNORMAL)==0) {
	G__fprinterr(G__serr,"Internal error: template function call %s failed"
		,funcname);
	G__genericerror((char*)NULL);
	*result = G__null;
      }
      G__exec_memberfunc = store_exec_memberfunc;
      G__freecharlist(&call_para);
      return(1); /* match */
    }
    deftmpfunc = deftmpfunc->next;
  }
  G__freecharlist(&call_para);
  return(0);  /* no match */
}
#endif /* G__TEMPLATEFUNC */

/***********************************************************************
* G__createtemplatefunc()
*
* Create template function entry
***********************************************************************/
int G__createtemplatefunc(funcname,targ,line_number,ppos)
char *funcname;
struct G__Templatearg *targ;
int line_number;
fpos_t *ppos;
{
  /*  template<class T,class E> type func(T a,E b,int a) {
   *                                      ^   */
#ifdef G__TEMPLATEFUNC
  struct G__Definetemplatefunc *deftmpfunc;
  /* fpos_t store_pos; */
  /* int store_line; */
  char paraname[G__MAXNAME];
  char temp[G__LONGLINE];
  /* struct G__Templatearg *tmparg; */
  int c,tmp;
  int unsigned_flag,reftype,pointlevel;
  int tagnum,typenum;
  int narg;
  int i;

  /**************************************************************
  * get to the end of list
  **************************************************************/
  deftmpfunc = &G__definedtemplatefunc;
  while(deftmpfunc->next) deftmpfunc = deftmpfunc->next;

  /**************************************************************
  * store linenumber , file pointer and file position
  **************************************************************/
  deftmpfunc->line=line_number;
  deftmpfunc->def_pos = *ppos;
  deftmpfunc->def_fp=G__ifile.fp;
  deftmpfunc->filenum = G__ifile.filenum;

  /**************************************************************
  * store template argument list
  **************************************************************/
  deftmpfunc->def_para = targ;

  /**************************************************************
  * store funcname and hash
  **************************************************************/
  {
    char *p;
    deftmpfunc->name=(char*)malloc(strlen(funcname)+1);
    strcpy(deftmpfunc->name,funcname);
    p = G__strrstr(deftmpfunc->name,"::");
    if(p) {
      *p = 0;
      deftmpfunc->parent_tagnum = G__defined_tagname(deftmpfunc->name,0);
      p = G__strrstr(funcname,"::");
      strcpy(deftmpfunc->name,p+2);
      G__hash(deftmpfunc->name,deftmpfunc->hash,tmp);
    }
    else {
      strcpy(deftmpfunc->name,funcname);
      G__hash(funcname,deftmpfunc->hash,tmp);
      deftmpfunc->parent_tagnum = G__get_envtagnum();
    }
  }
  deftmpfunc->friendtagnum = G__friendtagnum;

  /**************************************************************
  * allocate next list entry
  **************************************************************/
  deftmpfunc->next
  =(struct G__Definetemplatefunc*)malloc(sizeof(struct G__Definetemplatefunc));
  deftmpfunc->next->next = (struct G__Definetemplatefunc*)NULL;
  deftmpfunc->next->def_para = (struct G__Templatearg*)NULL;
  deftmpfunc->next->name = (char*)NULL;
  for(i=0;i<G__MAXFUNCPARA;i++) {
    deftmpfunc->next->func_para.ntarg[i]=(int*)NULL;
    deftmpfunc->next->func_para.nt[i]=0;
  }


  /**************************************************************
  * Parse template function parameter information
  **************************************************************/
  /*  template<class T,class E> type func(T a,E b,int a) {
   *                                      ^   */
  deftmpfunc->func_para.paran = tmp = 0;
  c=0;
  /* read file and get type of parameter */
  while(')'!=c) {
    /* initialize template function parameter attributes */
    deftmpfunc->func_para.type[tmp] = 0;
    deftmpfunc->func_para.tagnum[tmp] = -1;
    deftmpfunc->func_para.typenum[tmp] = -1;
    deftmpfunc->func_para.reftype[tmp] = G__PARANORMAL;
    deftmpfunc->func_para.paradefault[tmp] = 0;
    deftmpfunc->func_para.argtmplt[tmp] = -1;
    deftmpfunc->func_para.ntarg[tmp] = (int*)NULL;
    deftmpfunc->func_para.nt[tmp] = 0;

    unsigned_flag = reftype = pointlevel = 0;

    /*  template<class T,template<class U> class E> type func(T a,E<T> b) {
     *                                                        ^   ^  */

    do { /* read typename */
      c = G__fgetname_template(paraname,",)<*&=");
    } while(strcmp(paraname,"class")==0 || strcmp(paraname,"struct")==0 ||
	    strcmp(paraname,"const")==0 || strcmp(paraname,"volatile")==0
            || strcmp(paraname,"typename")==0
	    );

    /* Don't barf on an empty arg list. */
    if (paraname[0] == '\0' && c == ')' && tmp == 0) break;

    /*  template<class T,template<class U> class E> type func(T a,E<T> b) {
     *                                                         ^   ^  */
    /* if(isspace(c)) c = G__fgetname(temp,"<,()*&[="); */
    /*  template<class T,template<class U> class E> type func(T a,E<T> b) {
     *                                                          ^  ^  */

    /* 1. function parameter, fixed fundamental type */
    if(strcmp(paraname,"unsigned")==0) {
      unsigned_flag = -1;
      if('*'!=c && '&'!=c) c = G__fgetname(paraname,",)*&=");
    }
    else if(strcmp(paraname,"signed")==0) {
      unsigned_flag = 0;
      if('*'!=c && '&'!=c) c = G__fgetname(paraname,",)*&=");
    }
    if(strcmp(paraname,"int")==0) {
      deftmpfunc->func_para.type[tmp] = 'i' + unsigned_flag;
    }
    else if(strcmp(paraname,"char")==0) {
      deftmpfunc->func_para.type[tmp] = 'c' + unsigned_flag;
    }
    else if(strcmp(paraname,"short")==0) {
      deftmpfunc->func_para.type[tmp] = 's' + unsigned_flag;
    }
    else if(strcmp(paraname,"bool")==0) {
      deftmpfunc->func_para.type[tmp] = 'g';
    }
    else if(strcmp(paraname,"long")==0) {
      deftmpfunc->func_para.type[tmp] = 'l' + unsigned_flag;
      if('*'!=c && '&'!=c) {
	c = G__fgetname(paraname,",)*&[=");
	if(strcmp(paraname,"double")==0) deftmpfunc->func_para.type[tmp]='d';
      }
    }
    else if(strcmp(paraname,"double")==0) {
      deftmpfunc->func_para.type[tmp] = 'd';
    }
    else if(strcmp(paraname,"float")==0) {
      deftmpfunc->func_para.type[tmp] = 'f';
    }
    else if(strcmp(paraname,"void")==0) {
      deftmpfunc->func_para.type[tmp] = 'y';
    }
    else if(strcmp(paraname,"FILE")==0) {
      deftmpfunc->func_para.type[tmp] = 'e';
    }
    else if(unsigned_flag) {
      deftmpfunc->func_para.type[tmp] = 'i' + unsigned_flag;
    }

    /* 2. function parameter, template class */
    else if('<'==c) {
      char *ntargc[20];
      int ntarg[20];
      int nt=0;
      int i;
      /* f(T<E,K> a) or f(c<E,K> a) or f(c<E,b> a)
       * f(T<E> a) or f(c<T> a) or f(T<c> a) */
      deftmpfunc->func_para.type[tmp]='u';
      deftmpfunc->func_para.argtmplt[tmp] = -1;
      deftmpfunc->func_para.typenum[tmp] = -1;
      deftmpfunc->func_para.tagnum[tmp] = -1;
      /* 2.1.   f(T<x,E,y> a)
       *  ntarg   0 1 2 3     */
      do {
	ntarg[nt]=G__istemplatearg(paraname,deftmpfunc->def_para);
	if(0==ntarg[nt]) {
	  ntargc[nt] = malloc(strlen(paraname)+1);
	  strcpy(ntargc[nt],paraname);
	}
	++nt;
	c = G__fgetstream(paraname,",>");
      } while(','==c);
      if('>'==c) {
	ntarg[nt]=G__istemplatearg(paraname,deftmpfunc->def_para);
	if(0==ntarg[nt]) {
	  ntargc[nt] = malloc(strlen(paraname)+1);
	  strcpy(ntargc[nt],paraname);
	}
	++nt;
      }
      deftmpfunc->func_para.nt[tmp] = nt;
      deftmpfunc->func_para.ntarg[tmp] = (int*)malloc(sizeof(int)*nt);
      deftmpfunc->func_para.ntargc[tmp] = (char**)malloc(sizeof(char*)*nt);
      for(i=0;i<nt;i++) {
	deftmpfunc->func_para.ntarg[tmp][i] = ntarg[i];
	if(0==ntarg[i]) deftmpfunc->func_para.ntargc[tmp][i] = ntargc[i];
	else deftmpfunc->func_para.ntargc[tmp][i] = (char*)NULL;
      }
    }

    /* 3. function parameter, template argument */
    else if((narg=G__istemplatearg(paraname,deftmpfunc->def_para))) {
      /* f(T a) */
      if('*'==c) deftmpfunc->func_para.type[tmp]='U';
      else       deftmpfunc->func_para.type[tmp]='u';
      deftmpfunc->func_para.argtmplt[tmp]=narg;
    }

    /* 4. function parameter, fixed typedef or class,struct */
    else {
      /* f(c a) */
      /* 4.1. function parameter, fixed typedef */
      if(-1!=(typenum=G__defined_typename(paraname))) {
	deftmpfunc->func_para.type[tmp]=G__newtype.type[typenum];
	deftmpfunc->func_para.typenum[tmp]=typenum;
	deftmpfunc->func_para.tagnum[tmp]=G__newtype.tagnum[typenum];
      }
      /* 4.2. function parameter, fixed class,struct */
      else if(-1!=(tagnum=G__defined_tagname(paraname,0))) {
	/* Following 2 lines are questionable */
	if('*'==c) deftmpfunc->func_para.type[tmp]='U';
	else       deftmpfunc->func_para.type[tmp]='u';
	deftmpfunc->func_para.typenum[tmp] = -1;
	deftmpfunc->func_para.tagnum[tmp] = tagnum;
      }
      else {
	G__genericerror("Internal error: global function template arg type");
      }
    }

    /* Check pointlevel and reftype */
    while(','!=c && ')'!=c) {
      switch(c) {
      case '(': /* pointer to function */
	deftmpfunc->func_para.type[tmp] = 'Y';
	deftmpfunc->func_para.typenum[tmp] = -1;
	deftmpfunc->func_para.tagnum[tmp] = -1;
	c=G__fignorestream(")");
	c=G__fignorestream(",)");
	break;
      case '=':
	deftmpfunc->func_para.paradefault[tmp] = 1;
	c=G__fignorestream(",)");
	break;
      case '[':
	c=G__fignorestream("]");
	c = G__fgetname(temp,",()*&[=");
	++pointlevel;
	break;
      case '*':
	++pointlevel;
	c = G__fgetname(temp,",()*&[=");
	break;
      case '&':
	++reftype;
	c = G__fgetname(temp,",()*&[=");
	break;
      default:
	c = G__fgetname(temp,",()*&[=");
	break;
      }
    }
    /*  template<class T,template<class U> class E> type func(T a,E<T> b) {
     *                                                           ^      ^  */

    if(reftype) {
      if(pointlevel)
	deftmpfunc->func_para.type[tmp]=toupper(deftmpfunc->func_para.type[tmp]);
      deftmpfunc->func_para.reftype[tmp] = G__PARAREFERENCE;
    }
    else {
      switch(pointlevel) {
      case 0:
	deftmpfunc->func_para.reftype[tmp] = G__PARANORMAL ;
	break;
      case 1:
	deftmpfunc->func_para.type[tmp] =
	  toupper(deftmpfunc->func_para.type[tmp]) ;
	deftmpfunc->func_para.reftype[tmp] = G__PARANORMAL ;
	break;
      case 2:
	deftmpfunc->func_para.type[tmp] =
	  toupper(deftmpfunc->func_para.type[tmp]) ;
	deftmpfunc->func_para.reftype[tmp] = G__PARAP2P ;
	break;
      default:
	deftmpfunc->func_para.type[tmp] =
	  toupper(deftmpfunc->func_para.type[tmp]) ;
	deftmpfunc->func_para.reftype[tmp] = G__PARAP2P2P ;
	break;
      }
    }

    ++tmp;
    deftmpfunc->func_para.paran = tmp;
  }

  /*Hack by Scott Snyder: try not to gag on forward decl of template memfunc*/
  {
    int c = G__fignorestream(";{");
    if (';'!=c) G__fignorestream("}");
  }

#else /* G__TEMPLATEFUNC */
  G__genericerror("Limitation: Global function template ignored");
  G__fignorestream("{");
  G__fignorestream("}");
#endif /* G__TEMPLATEFUNC */
  return(0);
}

#endif /* G__TEMPLATECLASS */


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
