

/****************************************************************
* G__exec_asm()
*
*  Execute bytecode , compiled on-the-fly by interpreter.
*
****************************************************************/
#if  defined(G__ROOT)
extern "C" void* G__new_interpreted_object G__P((int size));
extern "C" void G__delete_interpreted_object G__P((void* p));
#endif
#ifdef __cplusplus
int G__exec_asm(int start,int stack,G__value *presult,long localmem) {
#else
int G__exec_asm(start,stack,presult,localmem) 
int start;
int stack;
G__value *presult;
long localmem;
{
#endif
  int i;                  /* misc counter */
  int pc;               /* instruction program counter */
  int sp;               /* data stack pointer */
  int strosp=0;           /* struct offset stack pointer */
  long struct_offset_stack[G__MAXSTRSTACK]; /*struct offset stack, was int */
  int gvpp=0;           /* struct offset stack pointer */
  long store_globalvarpointer[G__MAXSTRSTACK]; /**/
  char *funcname;         /* function name */
  /*int (*pfunc)(); */
  G__InterfaceMethod pfunc;
  struct G__param fpara;  /* func,var parameter buf */
  int *cntr;
  long store_struct_offset=0;
  int store_tagnum=0,store_return=0;
  struct G__tempobject_list *store_p_tempbuf=NULL;
#ifdef G__ASM_IFUNC
  char funcnamebuf[G__MAXNAME];
  int store_memberfunc_tagnum;
  long store_memberfunc_struct_offset;
  int store_exec_memberfunc;
#endif
  G__value *result;
#ifdef G__ASM_DBG
  int asm_step;
#endif
  int Nreorder;
  long store_memfuncenv_struct_offset[G__MAXSTRSTACK];
  short store_memfuncenv_tagnum[G__MAXSTRSTACK];
  char store_memfuncenv_var_type[G__MAXSTRSTACK];
  int memfuncenv_p=0;
  int pinc;
  int size;
  struct G__var_array *var;
  void (*p2fldst) G__P((G__value*,int*,long,struct G__var_array*,long));
  void (*p2fop2) G__P((G__value*,G__value*));
  void (*p2fop1) G__P((G__value*));
#ifdef G__ASM_WHOLEFUNC
  long store_struct_offset_localmem;
  struct G__ifunc_table *ifunc;
#endif
  int store_cpp_aryindex[10];
  int store_cpp_aryindexp=0;
  int store_step=0;
  long dtorfreeoffset=0;


  G__no_exec_compile=0;

#ifndef G__OLDIMPLEMENTATION
  *presult = G__null;
#endif


  /****************************************
  * local compile asembler execution start
  ****************************************/
#ifdef G__ASM_DBG
  if(G__asm_dbg) G__fprinterr(G__serr,"LOOP COMPILE EXECUTION START\n");
  asm_step = G__asm_step;
#endif

  pc=start;
  sp=stack;

  struct_offset_stack[0]=0;

  G__asm_exec = 1;

  G__asm_param = &fpara;


#ifdef G__ASM_DBG
  while(pc<G__MAXINST) {
#else
  pcode_parse_start:
#endif

#ifdef G__ASM_DBG
    if(asm_step) {
      if(!G__pause())  asm_step=0;
    }
#endif

#ifdef G__ASM_DBG
/*DEBUG*/
    /*
    if(G__asm_dbg) {
      G__fprinterr(G__serr,"G__store_struct_offset=%x\n",G__store_struct_offset);
    }
    */
#endif

    switch(G__INST(G__asm_inst[pc])) {

    case G__LDST_VAR_P:
      /***************************************
      * inst
      * 0 G__LDST_VAR_P
      * 1 index
      * 2 void (*f)(pbuf,psp,offset,var,ig15)
      * 3 (not use)
      * 4 var_array pointer
      * stack
      * sp          <-
      ***************************************/
#ifdef G__ASM_DBG
      if(G__asm_dbg) {
	var=(struct G__var_array*)G__asm_inst[pc+4];
	G__fprinterr(G__serr,"%3x,%d: LDST_VAR_P index=%d ldst=%d %s"
		,pc,sp,G__asm_inst[pc+1],G__asm_inst[pc+3]
		,var->varnamebuf[G__asm_inst[pc+1]]);
      }
#endif
      p2fldst = (void (*) G__P((G__value*,int*,long,struct G__var_array*,long)))G__asm_inst[pc+2];
      (*p2fldst)(G__asm_stack,&sp,0
	     ,(struct G__var_array*)G__asm_inst[pc+4],G__asm_inst[pc+1]);
      pc+=5;
#ifdef G__ASM_DBG
      if(G__asm_dbg) {
	G__fprinterr(G__serr," -> %d %d %d\n"
		     , G__asm_stack[sp-1].obj.i
		     , G__asm_stack[sp-1].type
		     , G__asm_stack[sp-1].tagnum);
      }
#endif
#ifdef G__ASM_DBG
      break;
#else
      goto pcode_parse_start;
#endif

#ifdef G__ASM_WHOLEFUNC
    case G__LDST_LVAR_P:
      /***************************************
      * inst
      * 0 G__LDST_LVAR_P
      * 1 index
      * 2 void (*f)(pbuf,psp,offset,var,ig15)
      * 3 (not use)
      * 4 var_array pointer
      * stack
      * sp          <-
      ***************************************/
#ifdef G__ASM_DBG
      if(G__asm_dbg) {
	var=(struct G__var_array*)G__asm_inst[pc+4];
	G__fprinterr(G__serr,"%3x,%d: LDST_LVAR_P index=%d ldst=%d %s "
		,pc,sp,G__asm_inst[pc+1],G__asm_inst[pc+3]
		,var->varnamebuf[G__asm_inst[pc+1]]);
      }
#endif
      p2fldst = (void (*) G__P((G__value*,int*,long,struct G__var_array*,long)))G__asm_inst[pc+2];
      (*p2fldst)(G__asm_stack,&sp,localmem /* temprary */
	     ,(struct G__var_array*)G__asm_inst[pc+4],G__asm_inst[pc+1]);
      pc+=5;
#ifdef G__ASM_DBG
      if(G__asm_dbg) {
	G__fprinterr(G__serr,"%d %g\n"
		,G__asm_stack[sp-1].obj.i,G__asm_stack[sp-1].obj.d);
      }
      break;
#else
      goto pcode_parse_start;
#endif
#endif

    case G__LDST_MSTR_P:
      /***************************************
      * inst
      * 0 G__LDST_MSTR_P
      * 1 index
      * 2 void (*f)(pbuf,psp,offset,var,ig15)
      * 3 (not use)
      * 4 var_array pointer
      * stack
      * sp          <-
      ***************************************/
#ifdef G__ASM_DBG
      if(G__asm_dbg) {
	var=(struct G__var_array*)G__asm_inst[pc+4];
	G__fprinterr(G__serr,"%3x,%d: LDST_MSTR_P index=%d ldst=%d %s stos=%lx\n"
		,pc,sp,G__asm_inst[pc+1],G__asm_inst[pc+3]
		,var->varnamebuf[G__asm_inst[pc+1]],G__store_struct_offset);
      }
#endif
      p2fldst = (void (*) G__P((G__value*,int*,long,struct G__var_array*,long)))G__asm_inst[pc+2];
      (*p2fldst)(G__asm_stack,&sp,G__store_struct_offset
	     ,(struct G__var_array*)G__asm_inst[pc+4],G__asm_inst[pc+1]);
      pc+=5;
#ifdef G__ASM_DBG
      break;
#else
      goto pcode_parse_start;
#endif

    case G__LDST_VAR_INDEX:
      /***************************************
      * inst
      * 0 G__LDST_VAR_INDEX
      * 1 *arrayindex
      * 2 void (*f)(pbuf,psp,offset,p,ctype,
      * 3 index
      * 4 pc increment
      * 5 local_global    &1 : param_local  , &2 : array_local
      * 6 var_array pointer
      * stack
      * sp          <-
      ***************************************/
#ifdef G__ASM_DBG
      if(G__asm_dbg) {
	var = (struct G__var_array*)G__asm_inst[pc+6];
	G__fprinterr(G__serr,"%3x,%d: LDST_VAR_INDEX index=%d %s\n"
		,pc,sp,G__asm_inst[pc+3]
		,var->varnamebuf[G__asm_inst[pc+3]]);
      }
#endif
      G__asm_stack[sp].obj.i = (G__asm_inst[pc+5]&1) ? 
	*(int*)(G__asm_inst[pc+1]+localmem) : *(int*)G__asm_inst[pc+1];
      G__asm_stack[sp++].type = 'i';
      p2fldst = (void (*) G__P((G__value*,int*,long,struct G__var_array*,long)))G__asm_inst[pc+2];
      (*p2fldst)(G__asm_stack,&sp, (G__asm_inst[pc+5]&2)?localmem:0 
	     ,(struct G__var_array*)G__asm_inst[pc+6],G__asm_inst[pc+3]);
      pc+=G__asm_inst[pc+4];
#ifdef G__ASM_DBG
      break;
#else
      goto pcode_parse_start;
#endif

    case G__LDST_VAR_INDEX_OPR:
      /***************************************
      * inst
      * 0 G__LDST_VAR_INDEX_OPR
      * 1 *int1
      * 2 *int2
      * 3 opr +,-
      * 4 void (*f)(pbuf,psp,offset,p,ctype,
      * 5 index
      * 6 pc increment
      * 7 local_global    &1 int1, &2 int2, &4 array
      * 8 var_array pointer
      * stack
      * sp          <-
      ***************************************/
#ifdef G__ASM_DBG
      if(G__asm_dbg) {
	var = (struct G__var_array*)G__asm_inst[pc+8];
	G__fprinterr(G__serr,"%3x,%d: LDST_VAR_INDEX_OPR index=%d %s\n"
		,pc,sp,G__asm_inst[pc+5]
		,var->varnamebuf[G__asm_inst[pc+5]]);
      }
#endif
      
      switch(G__asm_inst[pc+3]) {
      case '+':
	G__asm_stack[sp].obj.i = 
	  ((G__asm_inst[pc+7]&1)?
	   (*(int*)(G__asm_inst[pc+1]+localmem)):(*(int*)G__asm_inst[pc+1]))
	    +
	  ((G__asm_inst[pc+7]&2)?
	   (*(int*)(G__asm_inst[pc+2]+localmem)):(*(int*)G__asm_inst[pc+2]));
	break;
      case '-':
	G__asm_stack[sp].obj.i = 
	  ((G__asm_inst[pc+7]&1)?
	   (*(int*)(G__asm_inst[pc+1]+localmem)):(*(int*)G__asm_inst[pc+1]))
	    -
	  ((G__asm_inst[pc+7]&2)?
	   (*(int*)(G__asm_inst[pc+2]+localmem)):(*(int*)G__asm_inst[pc+2]));
	break;
      }
      G__asm_stack[sp++].type = 'i';
      p2fldst = (void (*) G__P((G__value*,int*,long,struct G__var_array*,long)))G__asm_inst[pc+4];
      (*p2fldst)(G__asm_stack,&sp
	     ,(G__asm_inst[pc+7]&4) ? localmem : 0
	     ,(struct G__var_array*)G__asm_inst[pc+8],G__asm_inst[pc+5]);
      pc+=G__asm_inst[pc+6];
#ifdef G__ASM_DBG
      break;
#else
      goto pcode_parse_start;
#endif


    case G__OP2_OPTIMIZED:
      /***************************************
      * inst
      * 0 OP2_OPTIMIZED
      * 1 (*p2f)(buf,buf)
      * stack
      * sp-2  a
      * sp-1  a           <-
      * sp    G__null
      ***************************************/
#ifdef G__ASM_DBG
      if(G__asm_dbg) 
	G__fprinterr(G__serr,"%3x,%d: OP2_OPTIMIZED %c:%d:%g %c:%d:%g" ,pc,sp
		     ,G__asm_stack[sp-2].type,G__asm_stack[sp-2].obj.i,G__asm_stack[sp-2].obj.d
		     ,G__asm_stack[sp-1].type,G__asm_stack[sp-1].obj.i,G__asm_stack[sp-1].obj.d);
#endif
      p2fop2 = (void (*) G__P((G__value*,G__value*)))G__asm_inst[pc+1];
      (*p2fop2)(&G__asm_stack[sp-1],&G__asm_stack[sp-2]);
      pc+=2;
      --sp;
#ifdef G__ASM_DBG
      if(G__asm_dbg) G__fprinterr(G__serr," -> %c:%d\n",G__asm_stack[sp-1].type
				  ,G__asm_stack[sp-1].obj.i);
#endif
#ifdef G__ASM_DBG
      break;
#else
      goto pcode_parse_start;
#endif

    case G__OP1_OPTIMIZED:
      /***************************************
      * inst
      * 0 OP1_OPTIMIZED
      * 1 (*p2f)(buf)
      * stack
      * sp-1  a
      * sp    G__null     <-
      ***************************************/
#ifdef G__ASM_DBG
      if(G__asm_dbg) 
	G__fprinterr(G__serr,"%3x,%d: OP1_OPTIMIZED %c:%d:%g" ,pc,sp 
		     ,G__asm_stack[sp-1].type,G__asm_stack[sp-1].obj.i,G__asm_stack[sp-1].obj.d);
#endif
      p2fop1 = (void (*) G__P((G__value*)))G__asm_inst[pc+1];
      (*p2fop1)(&G__asm_stack[sp-1]);
      pc+=2;
#ifdef G__ASM_DBG
      if(G__asm_dbg) G__fprinterr(G__serr," -> %c:%d:%g\n",G__asm_stack[sp-1].type
				  ,G__asm_stack[sp-1].obj.i,G__asm_stack[sp-1].obj.d);
#endif
#ifdef G__ASM_DBG
      break;
#else
      goto pcode_parse_start;
#endif



    case G__LD:
      /***************************************
      * inst
      * 0 G__LD
      * 1 address in data stack
      * stack
      * sp    a
      * sp+1             <-
      ***************************************/
#ifdef G__ASM_DBG
      if(G__asm_dbg) G__fprinterr(G__serr,"%3x,%d: LD %g,%d from %x %x,%x\n"
			     ,pc,sp
			     ,G__double(G__asm_stack[G__asm_inst[pc+1]])
			     ,G__int(G__asm_stack[G__asm_inst[pc+1]])
			     ,G__asm_inst[pc+1]
			     ,G__asm_stack,&G__asm_stack[G__asm_inst[pc+1]]);
#endif
      G__asm_stack[sp]=G__asm_stack[G__asm_inst[pc+1]];
      pc+=2;
      ++sp;
#ifdef G__ASM_DBG
      break;
#else
      goto pcode_parse_start;
#endif

    case G__CL:
      /***************************************
      * 0 CL
      *  clear stack pointer
      ***************************************/
#ifdef G__ASM_DBG
      if(G__asm_dbg) G__fprinterr(G__serr,"%3x,%d: CL %s:%d\n",pc,sp
		 ,G__srcfile[G__asm_inst[pc+1]/G__CL_FILESHIFT].filename
				  ,G__asm_inst[pc+1]&G__CL_LINEMASK);
#endif
      {
        G__ifile.line_number=G__asm_inst[pc+1]&G__CL_LINEMASK;
        G__ifile.filenum=(short)(G__asm_inst[pc+1]/G__CL_FILESHIFT);
        if((G__srcfile[G__ifile.filenum].maxline>G__ifile.line_number &&
            G__TESTBREAK&G__srcfile[G__ifile.filenum].breakpoint[G__ifile.line_number]) ||
           G__step) {
	  if(G__srcfile[G__ifile.filenum].breakpoint[G__ifile.line_number]&G__CONTUNTIL)
	    G__srcfile[G__ifile.filenum].breakpoint[G__ifile.line_number] &= G__NOCONTUNTIL;
          struct G__input_file store_ifile = G__ifile;
          if(G__ifile.filenum>=0) {
            strcpy(G__ifile.name,G__srcfile[G__ifile.filenum].filename);
            G__bc_setlinenum(G__ifile.line_number);
          }
          if(1 || G__istrace) {
            G__istrace |= 0x80;
            G__pr(G__serr,G__ifile);
            G__istrace &= 0x3f;
          }
	  G__pause();
	  G__ifile=store_ifile;
        }
      }
      G__delete_autoobjectstack(G__scopelevel);
      pc+=2;
      sp=0;
      strosp=0;
      struct_offset_stack[0]=0;
#ifdef G__ASM_DBG
      break;
#else
      goto pcode_parse_start;
#endif

    case G__OP2:
      /***************************************
      * inst
      * 0 OP2
      * 1 (+,-,*,/,%,@,>>,<<,&,|)
      * stack
      * sp-2  a
      * sp-1  b          <-
      * sp
      ***************************************/
#ifdef G__ASM_DBG
      if(G__asm_dbg) {
	if(isprint(G__asm_inst[pc+1]))
	  G__fprinterr(G__serr,"%3x,%d: OP2 %g '%c'%d %g\n"
		  ,pc,sp
		  ,G__double(G__asm_stack[sp-2])
		  ,G__asm_inst[pc+1]
		  ,G__asm_inst[pc+1]
		  ,G__double(G__asm_stack[sp-1]));
	else
	  G__fprinterr(G__serr,"%3x,%d: OP2 %g %d %g\n"
		  ,pc,sp
		  ,G__double(G__asm_stack[sp-2])
		  ,G__asm_inst[pc+1]
		  ,G__double(G__asm_stack[sp-1]));
      }
#endif
      G__bstore((char)G__asm_inst[pc+1]
		,G__asm_stack[sp-1],&G__asm_stack[sp-2]);
#ifdef G__ASM_DBG
      if(G__asm_dbg)
	G__fprinterr(G__serr," result=%g\n",G__double(G__asm_stack[sp-2]));
#endif
      pc+=2;
      --sp;
#ifdef G__ASM_DBG
      break;
#else
      goto pcode_parse_start;
#endif



    case G__CMPJMP:
      /***************************************
      * 0 CMPJMP
      * 1 *G__asm_test_X()
      * 2 *a
      * 3 *b
      * 4 next_pc
      ***************************************/
#ifdef G__ASM_DBG
      if(G__asm_dbg) G__fprinterr(G__serr,"%3x,%d: CMPJMP (0x%lx)%d (0x%lx)%d to %x\n"
			     ,pc,sp
			     ,G__asm_inst[pc+2],*(int *)G__asm_inst[pc+2]
			     ,G__asm_inst[pc+3],*(int *)G__asm_inst[pc+3]
			     ,G__asm_inst[pc+4]);
#endif
      if(!(*(int (*) G__P((int*,int*)))G__asm_inst[pc+1])((int *)G__asm_inst[pc+2]
					,(int *)G__asm_inst[pc+3])){
	pc=G__asm_inst[pc+4];
      }
      else {
	pc+=5;
      }
#ifdef G__ASM_DBG
      break;
#else
      goto pcode_parse_start;
#endif


    case G__INCJMP:
      /***************************************
      * 0 INCJMP
      * 1 *cntr
      * 2 increment
      * 3 next_pc
      ***************************************/
#ifdef G__ASM_DBG
      if(G__asm_dbg) G__fprinterr(G__serr,"%3x,%d: INCJMP *(int*)0x%x+%d to %x\n"
			     ,pc,sp ,G__asm_inst[pc+1] ,G__asm_inst[pc+2]
			     ,G__asm_inst[pc+3]);
#endif
      cntr=(int*)G__asm_inst[pc+1];
      *cntr = *cntr+G__asm_inst[pc+2];
      pc=G__asm_inst[pc+3];
      sp=0;
      strosp=0;
      struct_offset_stack[0]=0;
#ifdef G__ASM_DBG
      break;
#else
      goto pcode_parse_start;
#endif

    case G__CNDJMP:
      /***************************************
      * 0 CNDJMP   (jump if 0)
      * 1 next_pc
      * stack
      * sp-1         <-
      * sp
      ***************************************/
#ifdef G__ASM_DBG
      if(G__asm_dbg) G__fprinterr(G__serr,"%3x,%d: CNDJMP %d to %x\n"
			     ,pc,sp ,G__int(G__asm_stack[sp-1])
			     ,G__asm_inst[pc+1]);
#endif
      result = &G__asm_stack[sp-1];
      if(0.0==G__doubleM(result)) pc=G__asm_inst[pc+1];
      else                        pc+=2;
      --sp;
#ifdef G__ASM_DBG
      break;
#else
      goto pcode_parse_start;
#endif


    case G__JMP:
      /***************************************
      * 0 JMP
      * 1 next_pc
      ***************************************/
#ifdef G__ASM_DBG
      if(G__asm_dbg) G__fprinterr(G__serr,"%3x,%d: JMP %x\n"
			     ,pc,sp,G__asm_inst[pc+1]);
#endif
      pc=G__asm_inst[pc+1];
#ifdef G__ASM_DBG
      break;
#else
      goto pcode_parse_start;
#endif


    case G__POP:
      /***************************************
      * inst
      * 0 G__POP
      * stack
      * sp-1            <-
      * sp
      ***************************************/
#ifdef G__ASM_DBG
      if(G__asm_dbg) G__fprinterr(G__serr,"%3x,%d: POP %g -> %g\n" ,pc,sp
			     ,G__double(G__asm_stack[sp-1])
			     ,G__double(G__asm_stack[sp-2]));
#endif
      ++pc;
      --sp;
#ifdef G__ASM_DBG
      break;
#else
      goto pcode_parse_start;
#endif

    case G__LD_FUNC:
      /***************************************
      * inst
      * 0 G__LD_FUNC
      * 1 *name
      * 2 hash
      * 3 paran
      * 4 (*func)()
      * stack
      * sp-paran+1      <- sp-paran+1
      * sp-2
      * sp-1
      * sp
      ***************************************/
      ld_func:
#ifdef G__ASM_DBG
      if(G__asm_dbg) {
	if(G__asm_inst[pc+1]<G__MAXSTRUCT)
	  G__fprinterr(G__serr,"%3x,%d: LD_FUNC %s paran=%d\n" ,pc,sp
		  ,"compiled",G__asm_inst[pc+3]);
	else
	  G__fprinterr(G__serr,"%3x,%d: LD_FUNC %s paran=%d\n" ,pc,sp
		  ,(char *)G__asm_inst[pc+1],G__asm_inst[pc+3]);
      }
#endif
      funcname=(char *)G__asm_inst[pc+1];
      fpara.paran=G__asm_inst[pc+3];
      pfunc = (G__InterfaceMethod)G__asm_inst[pc+4] ;
      for(i=0;i<fpara.paran;i++) {
	fpara.para[i]=G__asm_stack[sp-fpara.paran+i];
	if(0==fpara.para[i].ref) {
	  switch(fpara.para[i].type) {
	  case 'f':
	  case 'b':
	  case 'c':
	  case 'r':
	  case 's':
	    break;
	  default:
	    fpara.para[i].ref=(long)(&fpara.para[i].obj);
	    break;
	  }
	}
      }
      sp-=fpara.paran;
      result = &G__asm_stack[sp];
      result->type = 0;
      if(0>G__asm_inst[pc+2]) {
	result->type = -G__asm_inst[pc+2];
	result->tagnum = G__asm_inst[pc+1];
	result->typenum = -1; /* This may have problem with pointer to
			       * member function */
      }
      result->ref = 0; 
      if(G__stepover) {
        store_step=G__step;
        G__step=0;
      }
#ifdef G__EXCEPTIONWRAPPER
      G__asm_exec=0;
      dtorfreeoffset = 
	G__ExceptionWrapper(pfunc,result,funcname,&fpara,G__asm_inst[pc+2]);
      G__asm_exec=1;
#else
      dtorfreeoffset = (*pfunc)(result,funcname,&fpara,G__asm_inst[pc+2]);
#endif
      if(G__stepover) G__step |= store_step;
      pc+=5;
      if(result->type) ++sp;
      if(G__return==G__RETURN_TRY) {
	if(G__CATCH!=G__dasm(G__serr,1)) {
	  G__asm_exec=0;
	  return(1);
	}
	G__asm_exec=1;
      }
      if(G__return!=G__RETURN_NON) {
	G__asm_exec=0;
	return(1);
      }
#ifdef G__ASM_DBG
      break;
#else
      goto pcode_parse_start;
#endif

    case G__RETURN:
      /***************************************
      * 0 RETURN
      ***************************************/
#ifdef G__ASM_DBG
      if(G__asm_dbg) G__fprinterr(G__serr,"%3x,%d: RETURN\n" ,pc,sp);
#endif
      pc++;
      /****************************************
       * local compile asembler execution flag
       ****************************************/
      G__asm_exec = 0;
      return(1); /* return 1 if successfully terminate */
      /* return(0); */

    case G__CAST:
      /***************************************
      * 0 CAST
      * 1 type
      * 2 typenum
      * 3 tagnum
      * 4 reftype 
      * stack
      * sp-1    <- cast on this
      * sp
      ***************************************/
#ifdef G__ASM_DBG
      if(G__asm_dbg) G__fprinterr(G__serr,"%3x,%d: CAST to %c\n"
			     ,pc,sp,(char)G__asm_inst[pc+1]);
#endif
      {
	int tagnum = G__asm_stack[sp-1].tagnum;
	G__asm_stack[sp-1].typenum = G__asm_inst[pc+2];
	G__asm_stack[sp-1].tagnum = G__asm_inst[pc+3];
	G__asm_cast((int)G__asm_inst[pc+1],&G__asm_stack[sp-1]
		    ,tagnum,G__asm_inst[pc+4]);
	if(isupper(G__asm_inst[pc+1]))
	  G__asm_stack[sp-1].obj.reftype.reftype = G__asm_inst[pc+4];
	pc+=5;
      }
#ifdef G__ASM_DBG
      break;
#else
      goto pcode_parse_start;
#endif

    case G__OP1:
      /***************************************
      * inst
      * 0 OP1
      * 1 (+,-)
      * stack
      * sp-1  a
      * sp    G__null     <-
      ***************************************/
#ifdef G__ASM_DBG
      if(G__asm_dbg) {
	if(G__asm_inst[pc+1])
	  G__fprinterr(G__serr,"%3x,%d: OP1 '%c'%d %g ,%d\n" ,pc,sp
		  ,G__asm_inst[pc+1]
		  ,G__asm_inst[pc+1]
		  ,G__double(G__asm_stack[sp-1]),sp);
	else
	  G__fprinterr(G__serr,"%3x,%d: OP1 %d %g ,%d\n" ,pc,sp
		  ,G__asm_inst[pc+1]
		  ,G__double(G__asm_stack[sp-1]),sp);
      }
#endif
      G__asm_stack[sp]=G__asm_stack[sp-1];
      G__asm_stack[sp-1]=G__null;
      G__bstore((char)G__asm_inst[pc+1],G__asm_stack[sp],&G__asm_stack[sp-1]);
      pc+=2;
#ifdef G__ASM_DBG
      break;
#else
      goto pcode_parse_start;
#endif

    case G__LETVVAL:
      /***************************************
      * inst
      * 0 LETVVAL
      * stack
      * sp-2  a
      * sp-1  b          <-
      * sp
      ***************************************/
#ifdef G__ASM_DBG
      if(G__asm_dbg) G__fprinterr(G__serr,"%3x,%d: LETVVAL\n" ,pc,sp);
#endif
      G__letVvalue(&G__asm_stack[sp-1],G__asm_stack[sp-2]);
      ++pc;
      --sp;
#ifdef G__ASM_DBG
      break;
#else
      goto pcode_parse_start;
#endif

    case G__ADDSTROS:
      /***************************************
      * inst
      * 0 ADDSTROS
      * 1 addoffset
      ***************************************/
#ifdef G__ASM_DBG
      if(G__asm_dbg)
	G__fprinterr(G__serr,"%3x,%d: ADDSTROS %d\n" ,pc,sp,G__asm_inst[pc+1]);
#endif
      G__store_struct_offset+=G__asm_inst[pc+1];
      pc+=2;
#ifdef G__ASM_DBG
      break;
#else
      goto pcode_parse_start;
#endif

    case G__LETPVAL:
      /***************************************
      * inst
      * 0 LETPVAL
      * stack
      * sp-2  a
      * sp-1  b          <-
      * sp
      ***************************************/
#ifdef G__ASM_DBG
      if(G__asm_dbg) G__fprinterr(G__serr,"%3x,%d: LETPVAL\n" ,pc,sp);
#endif
      G__letvalue(&G__asm_stack[sp-1],G__asm_stack[sp-2]);
      ++pc;
      --sp;
#ifdef G__ASM_DBG
      break;
#else
      goto pcode_parse_start;
#endif



    case G__TOPNTR:
      /***************************************
      * inst
      * 0 TOPNTR
      * stack
      * sp-1  a          <-
      * sp
      ***************************************/
#ifdef G__ASM_DBG
      if(G__asm_dbg) G__fprinterr(G__serr,"%3x,%d: TOPNTR\n" ,pc,sp);
#endif
      G__val2pointer(&G__asm_stack[sp-1]);
      ++pc;
#ifdef G__ASM_DBG
      break;
#else
      goto pcode_parse_start;
#endif

    case G__NOT:
      /***************************************
      * 0 NOT
      ***************************************/
#ifdef G__ASM_DBG
      if(G__asm_dbg) G__fprinterr(G__serr,"%3x,%d: NOT !%d\n"
			     ,pc,sp ,G__int(G__asm_stack[sp-1]));
#endif
      G__letint(&G__asm_stack[sp-1],'i',(long)(!G__int(G__asm_stack[sp-1])));
      ++pc;
#ifdef G__ASM_DBG
      break;
#else
      goto pcode_parse_start;
#endif

    case G__BOOL:
      /***************************************
       * 0 BOOL
       ***************************************/
#ifdef G__ASM_DBG
      if(G__asm_dbg) G__fprinterr(G__serr,"%3x,%d: BOOL %d\n"
			     ,pc,sp ,G__int(G__asm_stack[sp-1]));
#endif
      G__letint(&G__asm_stack[sp-1],'i',G__int(G__asm_stack[sp-1])?1:0);
      G__asm_stack[sp-1].tagnum = -1;
      G__asm_stack[sp-1].typenum = -1;
      ++pc;
#ifdef G__ASM_DBG
      break;
#else
      goto pcode_parse_start;
#endif


    case G__ISDEFAULTPARA:
      /***************************************
      * 0 ISDEFAULTPARA
      * 1 next_pc
      ***************************************/
#ifdef G__ASM_DBG
      if(G__asm_dbg) G__fprinterr(G__serr,"%3x,%d: !ISDEFAULTPARA JMP %x\n"
			     ,pc,sp ,G__asm_inst[pc+1]);
#endif
      if(sp>0) pc=G__asm_inst[pc+1];
      else     pc+=2;
#ifdef G__ASM_DBG
      break;
#else
      goto pcode_parse_start;
#endif


#define G__TUNEUP_BY_SEPARATION
#if defined(G__TUNEUP_BY_SEPARATION) && !defined(G__ASM_DBG)
    }
    switch(G__INST(G__asm_inst[pc])) {
#endif

    case G__LD_VAR:
      /***************************************
      * inst
      * 0 G__LD_VAR
      * 1 index
      * 2 paran
      * 3 point_level
      * 4 var_array pointer
      * stack
      * sp-paran+1      <- sp-paran+1
      * sp-2
      * sp-1
      * sp
      ***************************************/
#ifdef G__ASM_DBG
      if(G__asm_dbg) G__fprinterr(G__serr,"%3x,%d: LD_VAR index=%d paran=%d point %c"
			     ,pc,sp,G__asm_inst[pc+1],G__asm_inst[pc+2]
			     ,G__asm_inst[pc+3]);
#endif
      G__asm_index=G__asm_inst[pc+1];
      fpara.paran=G__asm_inst[pc+2];
      G__var_type=(char)G__asm_inst[pc+3];
      for(i=0;i<fpara.paran;i++) fpara.para[i]=G__asm_stack[sp-fpara.paran+i];
      sp-=fpara.paran;
      G__asm_stack[sp]=G__getvariable("",&i
				      ,(struct G__var_array*)G__asm_inst[pc+4]
				      ,(struct G__var_array*)NULL);
      pc+=5;
#ifdef G__ASM_DBG
      if(G__asm_dbg)
	G__fprinterr(G__serr," return=%g\n",G__double(G__asm_stack[sp]));
#endif
      ++sp;
#ifdef G__ASM_DBG
      break;
#else
      goto pcode_parse_start;
#endif

    case G__ST_VAR:
      /***************************************
      * inst
      * 0 G__ST_VAR
      * 1 index
      * 2 paran
      * 3 point_level
      * 4 var_array pointer
      * stack
      * sp-paran        <- sp-paran
      * sp-2
      * sp-1
      * sp
      ***************************************/
#ifdef G__ASM_DBG
      if(G__asm_dbg) G__fprinterr(G__serr,"%3x,%d: ST_VAR index=%d paran=%d point %c"
			     ,pc,sp,G__asm_inst[pc+1],G__asm_inst[pc+2]
			     ,G__asm_inst[pc+3]);
#endif
      G__asm_index=G__asm_inst[pc+1];
      fpara.paran=G__asm_inst[pc+2];
      G__var_type=(char)G__asm_inst[pc+3];
      for(i=0;i<fpara.paran;i++) fpara.para[i]=G__asm_stack[sp-fpara.paran+i];
      sp-=fpara.paran;
#ifdef G__ASM_DBG
      if(G__asm_dbg) G__fprinterr(G__serr,"  value=%g\n"
			     ,G__double(G__asm_stack[sp-1]));
#endif
      G__letvariable("",G__asm_stack[sp-1]
		     ,(struct G__var_array*)G__asm_inst[pc+4]
		     ,(struct G__var_array*)NULL);
      pc+=5;
#ifdef G__ASM_DBG
      break;
#else
      goto pcode_parse_start;
#endif

    case G__LD_MSTR:
      /***************************************
      * inst
      * 0 G__LD_MSTR
      * 1 index
      * 2 paran
      * 3 point_level
      * 4 *structmem
      * stack
      * sp-paran+1      <- sp-paran+1
      * sp-2
      * sp-1
      * sp
      ***************************************/
#ifdef G__ASM_DBG
      if(G__asm_dbg) G__fprinterr(G__serr,"%3x,%d: LD_MSTR index=%d paran=%d 0x%lx"
			     ,pc,sp,G__asm_inst[pc+1],G__asm_inst[pc+2]
			     ,G__store_struct_offset);
#endif
      G__asm_index=G__asm_inst[pc+1];
      fpara.paran=G__asm_inst[pc+2];
      G__var_type=(char)G__asm_inst[pc+3];
      for(i=0;i<fpara.paran;i++) fpara.para[i]=G__asm_stack[sp-fpara.paran+i];
      sp-=fpara.paran;
      G__asm_stack[sp]=G__getvariable(""
				      ,&i
				      ,(struct G__var_array *)G__asm_inst[pc+4]
				      ,&G__global);
      pc+=5;
#ifdef G__ASM_DBG
      if(G__asm_dbg) G__fprinterr(G__serr," return=%g , 0x%lx\n"
			     ,G__double(G__asm_stack[sp])
			     ,G__int(G__asm_stack[sp]));
#endif
      ++sp;
#ifdef G__ASM_DBG
      break;
#else
      goto pcode_parse_start;
#endif

    case G__ST_MSTR:
      /***************************************
      * inst
      * 0 G__ST_MSTR
      * 1 index
      * 2 paran
      * 3 point_level
      * 4 *structmem
      * stack
      * sp-paran        <- sp-paran
      * sp-2
      * sp-1
      * sp
      ***************************************/
#ifdef G__ASM_DBG
      if(G__asm_dbg) G__fprinterr(G__serr,"%3x,%d: ST_MSTR index=%d paran=%d 0x%lx"
			     ,pc,sp,G__asm_inst[pc+1],G__asm_inst[pc+2]
			     ,G__store_struct_offset);
#endif
      G__asm_index=G__asm_inst[pc+1];
      fpara.paran=G__asm_inst[pc+2];
      G__var_type=(char)G__asm_inst[pc+3];
      for(i=0;i<fpara.paran;i++) fpara.para[i]=G__asm_stack[sp-fpara.paran+i];
      sp-=fpara.paran;
#ifdef G__ASM_DBG
      if(G__asm_dbg)
	G__fprinterr(G__serr,"  value=%g,%d\n" 
		    ,G__double(G__asm_stack[sp-1]),G__int(G__asm_stack[sp-1]));
#endif

      G__letvariable("",G__asm_stack[sp-1]
		     ,(struct G__var_array *)G__asm_inst[pc+4]
		     ,&G__global);
      pc+=5;
#ifdef G__ASM_DBG
      break;
#else
      goto pcode_parse_start;
#endif


#ifdef G__ASM_WHOLEFUNC
    case G__LD_LVAR:
      /***************************************
      * inst
      * 0 G__LD_LVAR
      * 1 index
      * 2 paran
      * 3 point_level
      * 4 var_array pointer
      * stack
      * sp-paran+1      <- sp-paran+1
      * sp-2
      * sp-1
      * sp
      ***************************************/
#ifdef G__ASM_DBG
      if(G__asm_dbg) G__fprinterr(G__serr,"%3x,%d: LD_LVAR index=%d paran=%d point %c"
			     ,pc,sp,G__asm_inst[pc+1],G__asm_inst[pc+2]
			     ,G__asm_inst[pc+3]);
#endif
      G__asm_index=G__asm_inst[pc+1];
      fpara.paran=G__asm_inst[pc+2];
      G__var_type=(char)G__asm_inst[pc+3];
      for(i=0;i<fpara.paran;i++) fpara.para[i]=G__asm_stack[sp-fpara.paran+i];
      sp-=fpara.paran;
      store_struct_offset_localmem = G__store_struct_offset;
      G__store_struct_offset = (long)localmem;
      G__asm_stack[sp]=G__getvariable("",&i
				      ,(struct G__var_array*)G__asm_inst[pc+4]
				      ,&G__global);
      G__store_struct_offset = store_struct_offset_localmem;
      pc+=5;
#ifdef G__ASM_DBG
      if(G__asm_dbg)
	G__fprinterr(G__serr," return=%g\n",G__double(G__asm_stack[sp]));
#endif
      ++sp;
#ifdef G__ASM_DBG
      break;
#else
      goto pcode_parse_start;
#endif

    case G__ST_LVAR:
      /***************************************
      * inst
      * 0 G__ST_LVAR
      * 1 index
      * 2 paran
      * 3 point_level
      * 4 var_array pointer
      * stack
      * sp-paran        <- sp-paran
      * sp-2
      * sp-1
      * sp
      ***************************************/
#ifdef G__ASM_DBG
      if(G__asm_dbg) G__fprinterr(G__serr,"%3x,%d: ST_LVAR index=%d paran=%d point %c"
			     ,pc,sp,G__asm_inst[pc+1],G__asm_inst[pc+2]
			     ,G__asm_inst[pc+3]);
#endif
      G__asm_index=G__asm_inst[pc+1];
      fpara.paran=G__asm_inst[pc+2];
      G__var_type=(char)G__asm_inst[pc+3];
      for(i=0;i<fpara.paran;i++) fpara.para[i]=G__asm_stack[sp-fpara.paran+i];
      sp-=fpara.paran;
#ifdef G__ASM_DBG
      if(G__asm_dbg) G__fprinterr(G__serr,"  value=%g\n"
			     ,G__double(G__asm_stack[sp-1]));
#endif
      store_struct_offset_localmem = G__store_struct_offset;
      G__store_struct_offset = (long)localmem;
      G__letvariable("",G__asm_stack[sp-1]
		     ,(struct G__var_array*)G__asm_inst[pc+4]
		     ,&G__global);
      G__store_struct_offset = store_struct_offset_localmem;
      pc+=5;
#ifdef G__ASM_DBG
      break;
#else
      goto pcode_parse_start;
#endif


#endif /* G__ASM_WHOLEFUNC */


    case G__CMP2:
      /***************************************
      * 0 CMP2
      * 1 operator
      * stack
      * sp-1         <-
      * sp
      ***************************************/
#ifdef G__ASM_DBG
      if(G__asm_dbg) G__fprinterr(G__serr,"%3x,%d: CMP2 %g '%c' %g\n"
			     ,pc,sp ,G__double(G__asm_stack[sp-2])
			     ,G__asm_inst[pc+1],G__double(G__asm_stack[sp-1]));
#endif
      G__letint(&G__asm_stack[sp-2] ,'i'
		,(long)G__btest((char)G__asm_inst[pc+1]
				,G__asm_stack[sp-2] ,G__asm_stack[sp-1]));
      pc+=2;
      --sp;
#ifdef G__ASM_DBG
      break;
#else
      goto pcode_parse_start;
#endif

    case G__PUSHSTROS:
      /***************************************
      * inst
      * 0 G__PUSHSTROS
      * stack
      * sp           <- sp-paran
      ***************************************/
#ifdef G__ASM_DBG
      if(G__asm_dbg) G__fprinterr(G__serr,"%3x,%d: PUSHSTROS 0x%lx strosp=%ld\n"
			     ,pc,sp,G__store_struct_offset,strosp);
#endif
      struct_offset_stack[strosp]=G__store_struct_offset;
      ++strosp;
      ++pc;
#ifdef G__ASM_DBG
      break;
#else
      goto pcode_parse_start;
#endif

    case G__SETSTROS:
      /***************************************
      * inst
      * 0 G__SETSTROS
      * stack
      * sp-1         <- sp-paran
      * sp
      ***************************************/
#ifdef G__ASM_DBG
      if(G__asm_dbg) G__fprinterr(G__serr,"%3x,%d: SETSTROS 0x%lx\n"
			     ,pc,sp,G__int(G__asm_stack[sp-1]));
#endif
      G__store_struct_offset=G__int(G__asm_stack[sp-1]);
      --sp;
      ++pc;
#ifdef G__ASM_DBG
      break;
#else
      goto pcode_parse_start;
#endif

    case G__POPSTROS:
      /***************************************
      * inst
      * 0 G__POPSTROS
      * stack
      * sp           <- sp-paran
      ***************************************/
#ifdef G__ASM_DBG
      if(G__asm_dbg) G__fprinterr(G__serr,"%3x,%d: POPSTROS 0x%lx strosp=%ld\n"
			     ,pc,sp,struct_offset_stack[strosp-1],strosp);
#endif
      G__store_struct_offset=struct_offset_stack[strosp-1];
      --strosp;
      ++pc;
#ifdef G__ASM_DBG
      break;
#else
      goto pcode_parse_start;
#endif

    case G__SETTEMP:
      /***************************************
      * 0 SETTEMP
      ***************************************/
      store_p_tempbuf = G__p_tempbuf->prev;
      if(G__p_tempbuf) {
#ifdef G__ASM_DBG
	if(G__asm_dbg) G__fprinterr(G__serr,"%3x,%d: SETTEMP 0x%lx\n"
				    ,pc,sp ,G__p_tempbuf->obj.obj.i);
#endif
	store_struct_offset = G__store_struct_offset;
	store_tagnum = G__tagnum;
	store_return=G__return;
	G__store_struct_offset = G__p_tempbuf->obj.obj.i;
	G__tagnum = G__p_tempbuf->obj.tagnum;
	G__return=G__RETURN_NON;
      }
      ++pc;
#ifdef G__ASM_DBG
      break;
#else
      goto pcode_parse_start;
#endif

    case G__FREETEMP:
      /***************************************
      * 0 FREETEMP
      ***************************************/
#ifdef G__ASM_DBG
      if(G__asm_dbg) G__fprinterr(G__serr,"%3x,%d: FREETEMP 0x%lx\n"
			     ,pc,sp ,store_p_tempbuf);
#endif
      G__store_struct_offset = store_struct_offset;
      G__tagnum = store_tagnum;
      G__return=store_return;
      if(G__p_tempbuf && store_p_tempbuf) {
#ifdef G__ASM_IFUNC
	if(-1==G__p_tempbuf->obj.tagnum ||
	   -1!=G__struct.iscpplink[G__p_tempbuf->obj.tagnum]) {
	  free((void*)G__p_tempbuf->obj.obj.i);
	}
#endif
	free((void*)G__p_tempbuf);
	G__p_tempbuf = store_p_tempbuf;
      }
      ++pc;
#ifdef G__ASM_DBG
      break;
#else
      goto pcode_parse_start;
#endif


    case G__GETRSVD:
      /***************************************
      * 0 GETRSVD
      * 1 item+1
      * stack
      * sp-1  ptr    <-
      * sp
      ***************************************/
#ifdef G__ASM_DBG
      if(G__asm_dbg) G__fprinterr(G__serr,"%3x,%d: GETRSVD $%s 0x%x\n"
			     ,pc,sp
			     ,(char*)G__asm_inst[pc+1]
			     ,G__int(G__asm_stack[sp-1]));
#endif
      G__asm_stack[sp-1]
	= (*G__GetSpecialObject)((char*)G__asm_inst[pc+1]
			 ,(void**)G__int(G__asm_stack[sp-1])
			 ,(void**)G__int(G__asm_stack[sp-1])+G__LONGALLOC);
      pc+=2;
#ifdef G__ASM_DBG
      break;
#else
      goto pcode_parse_start;
#endif


    case G__REWINDSTACK:
      /***************************************
      * inst
      * 0 G__REWINDSTACK
      * 1 rewind
      * stack
      * sp-2            <-  ^
      * sp-1                | rewind
      * sp              <- ..
      ***************************************/
#ifdef G__ASM_DBG
      if(G__asm_dbg)
	G__fprinterr(G__serr,"%3x,%d: REWINDSTACK %d\n" ,pc,sp,G__asm_inst[pc+1]);
#endif
      sp -= G__asm_inst[pc+1];
      pc+=2;
#ifdef G__ASM_DBG
      break;
#else
      goto pcode_parse_start;
#endif

    case G__CND1JMP:
      /***************************************
      * 0 CND1JMP   (jump if 1)
      * 1 next_pc
      * stack
      * sp-1         <-
      * sp
      ***************************************/
#ifdef G__ASM_DBG
      if(G__asm_dbg) G__fprinterr(G__serr,"%3x,%d: CND1JMP %d to %x\n"
			     ,pc,sp ,G__int(G__asm_stack[sp-1])
			     ,G__asm_inst[pc+1]);
#endif
      result = &G__asm_stack[sp-1];
      if(0.0!=G__doubleM(result)) pc=G__asm_inst[pc+1];
      else                        pc+=2;
      --sp;
#ifdef G__ASM_DBG
      break;
#else
      goto pcode_parse_start;
#endif

#ifdef G__ASM_IFUNC
    case G__LD_IFUNC:
      /***************************************
      * inst
      * 0 G__LD_IFUNC
      * 1 *name
      * 2 hash          // unused
      * 3 paran
      * 4 p_ifunc
      * 5 funcmatch
      * 6 memfunc_flag
      * 7 index
      * stack
      * sp-paran+1      <- sp-paran+1
      * sp-2
      * sp-1
      * sp
      ***************************************/
#ifdef G__ASM_DBG
      if(G__asm_dbg) G__fprinterr(G__serr,"%3x,%d: LD_IFUNC %s paran=%d\n" ,pc,sp
			     ,(char *)G__asm_inst[pc+1],G__asm_inst[pc+3]);
#endif
      G__asm_index = G__asm_inst[pc+7];
      ifunc = (struct G__ifunc_table*)G__asm_inst[pc+4];
      if(G__cintv6 && G__LD_IFUNC_optimize(ifunc,G__asm_index,G__asm_inst,pc)) goto ld_func;
#ifdef G__ASM_WHOLEFUNC
      if(ifunc->pentry[G__asm_index]->bytecode&&G__asm_inst[pc]==G__LD_IFUNC
	 && 0==ifunc->isvirtual[G__asm_index]
	 ) {
#ifdef G__ASM_DBG
	if(G__asm_dbg) G__fprinterr(G__serr,"call G__exec_bytecode optimized\n");
#endif	
	G__asm_inst[pc] = G__LD_FUNC;
	G__asm_inst[pc+1] = (long)(ifunc->pentry[G__asm_index]->bytecode);
	G__asm_inst[pc+4] = (long)G__exec_bytecode;
	G__asm_inst[pc+5] = G__JMP;
	G__asm_inst[pc+6] = pc+8;
	G__asm_inst[pc+7] = G__NOP;
	goto ld_func;
      }
#endif
      strcpy(funcnamebuf,(char*)G__asm_inst[pc+1]);
      fpara.paran=G__asm_inst[pc+3];
      pfunc = (G__InterfaceMethod)G__asm_inst[pc+4] ;
      for(i=0;i<fpara.paran;i++) fpara.para[i]=G__asm_stack[sp-fpara.paran+i];
      sp-=fpara.paran;
      store_exec_memberfunc = G__exec_memberfunc;
      store_memberfunc_tagnum = G__memberfunc_tagnum;
      store_memberfunc_struct_offset = G__memberfunc_struct_offset;
      G__interpret_func(&G__asm_stack[sp],funcnamebuf,&fpara,G__asm_inst[pc+2]
			,ifunc
			,G__asm_inst[pc+5],G__asm_inst[pc+6]);
      G__memberfunc_tagnum = store_memberfunc_tagnum;
      G__memberfunc_struct_offset = store_memberfunc_struct_offset;
      G__exec_memberfunc = store_exec_memberfunc;
      pc+=8;
      if('~'!=funcnamebuf[0]) ++sp;
      if(G__return==G__RETURN_TRY) {
	if(G__CATCH!=G__dasm(G__serr,1)) {
	  G__asm_exec=0;
	  return(1);
	}
	G__asm_exec=1;
      }
      if(G__return!=G__RETURN_NON) {
	G__asm_exec=0;
	return(1);
      }
#ifdef G__ASM_DBG
      break;
#else
      goto pcode_parse_start;
#endif

    case G__NEWALLOC:
      /***************************************
      * inst
      * 0 G__NEWALLOC
      * 1 size     0 if arena
      * 2 isclass&&array
      * stack
      * sp-2     <- arena
      * sp-1     <- pinc
      * sp
      ***************************************/
      if(G__asm_inst[pc+1]) {
#if  defined(G__ROOT)
	G__store_struct_offset
	  =(long)G__new_interpreted_object(G__asm_inst[pc+1]*G__asm_stack[sp-1].obj.i);
#else
	G__store_struct_offset
	  =(long)malloc(G__asm_inst[pc+1]*G__asm_stack[sp-1].obj.i);
#endif
      }
      else {
	G__store_struct_offset = G__asm_stack[sp-2].obj.i;
      }
      if(0==G__store_struct_offset)
	G__genericerror("Error: malloc failed for new operator");
#ifdef G__ASM_DBG
      if(G__asm_dbg) G__fprinterr(G__serr,"%3x,%d: NEWALLOC size(%d)*%d : 0x%lx\n"
			     ,pc,sp,G__asm_inst[pc+1]
			     ,G__int(G__asm_stack[sp-1])
			     ,G__store_struct_offset);
#endif
      pinc=G__int(G__asm_stack[sp-1]);
      if(G__asm_inst[pc+2]) {
	G__alloc_newarraylist(G__store_struct_offset,pinc);
      }
      if(G__asm_inst[pc+1]) --sp;
      else                  sp-=2;
      pc+=3;
#ifdef G__ASM_DBG
      break;
#else
      goto pcode_parse_start;
#endif

    case G__SET_NEWALLOC:
      /***************************************
      * inst
      * 0 G__SET_NEWALLOC
      * 1 tagnum
      * 2 type&reftype
      * stack
      * sp-1 
      * sp        G__store_struct_offset
      * sp+1   <-
      ***************************************/
#ifdef G__ASM_DBG
      if(G__asm_dbg) G__fprinterr(G__serr,"%3x,%d: SET_NEWALLOC 0x%lx %d %d\n"
				  ,pc,sp,G__store_struct_offset
				  ,G__asm_inst[pc+1],G__asm_inst[pc+2]);
#endif
      ++sp; /* didn't understand meaning of cheating LD_IFUNC */
      G__asm_stack[sp-1].obj.reftype.reftype = (G__asm_inst[pc+2]>>8);
      G__asm_stack[sp-1].type = G__asm_inst[pc+2]&0xff;
      G__asm_stack[sp-1].obj.i=G__store_struct_offset;
      G__asm_stack[sp-1].tagnum = G__asm_inst[pc+1];
      G__asm_stack[sp-1].typenum = -1;
      pc+=3;
      /* sp; stack pointer won't change, cheat LD_IFUNC result */
#ifdef G__ASM_DBG
      break;
#else
      goto pcode_parse_start;
#endif

    case G__DELETEFREE:
      /***************************************
      * inst
      * 0 G__DELETEFREE
      * 1 isarray  0: simple free, 1: array, 2: virtual free
      ***************************************/
#ifdef G__ASM_DBG
      if(G__asm_dbg) G__fprinterr(G__serr,"%3x,%d: DELETEFREE %lx\n"
			     ,pc,sp,G__store_struct_offset);
#endif
      if(G__store_struct_offset) {
	if(G__asm_inst[pc+1]==2) {
	  G__store_struct_offset += dtorfreeoffset;
	  dtorfreeoffset = 0;
	}
#if  defined(G__ROOT)
	G__delete_interpreted_object((void*)G__store_struct_offset);
#else
	free((void*)G__store_struct_offset);
#endif
      }
      pc+=2;
#ifdef G__ASM_DBG
      break;
#else
      goto pcode_parse_start;
#endif

    case G__SWAP:
      /***************************************
      * inst
      * 0 G__SWAP
      * stack
      * sp-2          sp-1
      * sp-1          sp-2
      * sp       <-   sp
      ***************************************/
#ifdef G__ASM_DBG
      if(G__asm_dbg) G__fprinterr(G__serr,"%3x,%d: SWAP\n",pc,sp);
#endif
      G__asm_stack[sp] = G__asm_stack[sp-2];
      G__asm_stack[sp-2] = G__asm_stack[sp-1];
      G__asm_stack[sp-1] = G__asm_stack[sp];
      ++pc;
#ifdef G__ASM_DBG
      break;
#else
      goto pcode_parse_start;
#endif

#endif /* G__ASM_IFUNC */

    case G__BASECONV:
      /***************************************
      * inst
      * 0 G__BASECONV
      * 1 formal_tagnum
      * 2 baseoffset
      * stack
      * sp-2          sp-1
      * sp-1          sp-2
      * sp       <-   sp
      ***************************************/
#ifdef G__ASM_DBG
      if(G__asm_dbg) G__fprinterr(G__serr,"%3x,%d: BASECONV %d %d\n",pc,sp
			     ,G__asm_inst[pc+1],G__asm_inst[pc+2]);
#endif
      G__asm_stack[sp-1].typenum = -1;
      G__asm_stack[sp-1].tagnum = G__asm_inst[pc+1];
      if(G__asm_stack[sp-1].ref==G__asm_stack[sp-1].obj.i)
	G__asm_stack[sp-1].ref += G__asm_inst[pc+2];
      G__asm_stack[sp-1].obj.i += G__asm_inst[pc+2];
/* #define G__OLDIMPLEMENTATION2077 */
      if(0==G__asm_stack[sp-1].ref && 'u'==G__asm_stack[sp-1].type)
	G__asm_stack[sp-1].ref += G__asm_stack[sp-1].obj.i;
      pc+=3;
#ifdef G__ASM_DBG
      break;
#else
      goto pcode_parse_start;
#endif

    case G__STORETEMP:
      /***************************************
      * 0 STORETEMP
      * stack
      * sp-1
      * sp       <-  sp
      ***************************************/
#ifdef G__ASM_DBG
      if(G__asm_dbg) G__fprinterr(G__serr,"%3x,%d: STORETEMP 0x%lx\n"
			     ,pc,sp ,G__p_tempbuf->obj.obj.i);
#endif
      G__store_tempobject(G__asm_stack[sp-1]);
      ++pc;
#ifdef G__ASM_DBG
      break;
#else
      goto pcode_parse_start;
#endif

    case G__ALLOCTEMP:
      /***************************************
      * 0 ALLOCTEMP
      * 1 tagnum
      * stack
      * sp-1
      * sp       <-  sp
      ***************************************/
#ifdef G__ASM_DBG
      if(G__asm_dbg) G__fprinterr(G__serr,"%3x,%d: ALLOCTEMP %s\n"
			     ,pc,sp,G__struct.name[G__asm_inst[pc+1]]);
#endif
      G__alloc_tempobject(G__asm_inst[pc+1],-1);
      pc+=2;
#ifdef G__ASM_DBG
      break;
#else
      goto pcode_parse_start;
#endif

    case G__POPTEMP:
      /***************************************
      * 0 POPTEMP
      * 1 tagnum
      * stack
      * sp-1
      * sp      <-  sp
      ***************************************/
#ifdef G__ASM_DBG
      if(G__asm_dbg) G__fprinterr(G__serr,"%3x,%d: POPTEMP 0x%lx %d\n"
			     ,pc,sp ,store_p_tempbuf,G__asm_inst[pc+1]);
#endif
      if(-1!=G__asm_inst[pc+1]) {
	G__asm_stack[sp-1].tagnum = G__asm_inst[pc+1];
	G__asm_stack[sp-1].typenum = -1;
	G__asm_stack[sp-1].type = 'u';
	G__asm_stack[sp-1].obj.i = G__store_struct_offset;
	G__asm_stack[sp-1].ref = G__store_struct_offset;
      }
      G__store_struct_offset = store_struct_offset;
      G__tagnum = store_tagnum;
      G__return=store_return;
      pc+=2;
#ifdef G__ASM_DBG
      break;
#else
      goto pcode_parse_start;
#endif

    case G__REORDER:
      /***************************************
      * 0 REORDER
      * 1 paran(total)
      * 2 ig25(arrayindex)
      * stack      paran=4 ig25=2    x y z w -> x y z w z w -> x y x y z w -> w z x y
      * sp-3    <-  sp-1
      * sp-2    <-  sp-3
      * sp-1    <-  sp-2
      * sp      <-  sp
      ***************************************/
#ifdef G__ASM_DBG
      if(G__asm_dbg) G__fprinterr(G__serr,"%3x,%d: REORDER paran=%d ig25=%d\n"
			     ,pc,sp ,G__asm_inst[pc+1],G__asm_inst[pc+2]);
#endif
      /* x y z w */
      Nreorder = G__asm_inst[pc+1]-G__asm_inst[pc+2];
      for(i=0;i<Nreorder;i++) G__asm_stack[sp+i] = G__asm_stack[sp+i-Nreorder];
      /* x y z w z w */
      for(i=0;i<G__asm_inst[pc+2];i++)
	G__asm_stack[sp-i-1] = G__asm_stack[sp-i-Nreorder-1];
      /* x y x y z w */
      for(i=0;i<Nreorder;i++)
	G__asm_stack[sp-G__asm_inst[pc+1]+i] = G__asm_stack[sp+Nreorder-1-i];
      /* w z x y z w */
      pc+=3;
#ifdef G__ASM_DBG
      break;
#else
      goto pcode_parse_start;
#endif

    case G__LD_THIS:
      /***************************************
      * 0 LD_THIS
      * 1 point_level;
      * stack
      * sp-1
      * sp
      * sp+1   <-
      ***************************************/
#ifdef G__ASM_DBG
      if(G__asm_dbg) G__fprinterr(G__serr,"%3x,%d: LD_THIS 0x%lx %s\n"
			     ,pc,sp ,G__store_struct_offset
			     ,G__struct.name[G__tagnum]);
#endif
      G__var_type = (char)G__asm_inst[pc+1];
      G__getthis(&G__asm_stack[sp],"this","this");
      G__var_type = 'p';
      pc+=2;
      ++sp;
#ifdef G__ASM_DBG
      break;
#else
      goto pcode_parse_start;
#endif

    case G__RTN_FUNC:
      /***************************************
      * 0 RTN_FUNC
      * 1 isreturnvalue    0:no return val, 1:with return val, 2:
      * stack
      * sp-1   -> return this
      * sp
      ***************************************/
#ifdef G__ASM_DBG
      if(G__asm_dbg) G__fprinterr(G__serr,"%3x,%d: RTN_FUNC %d\n"
			     ,pc,sp ,G__asm_inst[pc+1]);
#endif
      /* return from   'try {  }' block */
      if(2==G__asm_inst[pc+1]) return(1); 
      G__asm_exec = 0;
      G__return=G__RETURN_NORMAL;
      if(G__asm_inst[pc+1]) *presult = G__asm_stack[sp-1];
      else                  *presult = G__null;
      pc+=2;
      --sp;
      return(1);
#ifdef G__NEVER
#ifdef G__ASM_DBG
      break;
#else
      goto pcode_parse_start;
#endif
#endif

    case G__SETMEMFUNCENV:
      /***************************************
      * 0 SETMEMFUNCENV:
      ***************************************/
#ifdef G__ASM_DBG
      if(G__asm_dbg) G__fprinterr(G__serr,"%3x,%d: SETMEMFUNCENV %lx <- %lx %ld\n"
			     ,pc,sp ,G__store_struct_offset
			     ,G__memberfunc_struct_offset
			     ,memfuncenv_p);
#endif
      store_memfuncenv_tagnum[memfuncenv_p] = G__tagnum;
      store_memfuncenv_struct_offset[memfuncenv_p] = G__store_struct_offset;
      store_memfuncenv_var_type[memfuncenv_p] = G__var_type;
      ++memfuncenv_p;
      G__tagnum = G__memberfunc_tagnum;
      G__store_struct_offset = G__memberfunc_struct_offset;
      G__var_type = 'p';
      pc+=1;
#ifdef G__ASM_DBG
      break;
#else
      goto pcode_parse_start;
#endif

    case G__RECMEMFUNCENV:
      /***************************************
      * 0 RECMEMFUNCENV:
      ***************************************/
#ifdef G__ASM_DBG
      if(G__asm_dbg) G__fprinterr(G__serr,"%3x,%d: RECMEMFUNCENV %lx <- %lx %ld\n"
			     ,pc,sp ,G__store_struct_offset
			     ,store_memfuncenv_struct_offset
			     ,memfuncenv_p-1);
#endif
      --memfuncenv_p;
      G__var_type = store_memfuncenv_var_type[memfuncenv_p];
      G__tagnum = store_memfuncenv_tagnum[memfuncenv_p];
      G__store_struct_offset = store_memfuncenv_struct_offset[memfuncenv_p];
      pc+=1;
#ifdef G__ASM_DBG
      break;
#else
      goto pcode_parse_start;
#endif

    case G__ADDALLOCTABLE:
      /***************************************
      * 0 ADDALLOCTABLE:
      * sp-1   --> add alloctable
      * sp   <-
      ***************************************/
#ifdef G__ASM_DBG
      if(G__asm_dbg) G__fprinterr(G__serr,"%3x,%d: ADDALLOCTABLE \n" ,pc,sp);
#endif
      G__add_alloctable((void*)G__asm_stack[sp-1].obj.i
			,G__asm_stack[sp-1].type
			,G__asm_stack[sp-1].tagnum);
      pc+=1;
#ifdef G__ASM_DBG
      break;
#else
      goto pcode_parse_start;
#endif

    case G__DELALLOCTABLE:
      /***************************************
      * 0 DELALLOCTABLE:
      * sp-1   --> del alloctable
      * sp   <-
      ***************************************/
#ifdef G__ASM_DBG
      if(G__asm_dbg) G__fprinterr(G__serr,"%3x,%d: DELALLOCTABLE \n" ,pc,sp);
#endif
      G__del_alloctable((void*)G__asm_stack[sp-1].obj.i);
      pc+=1;
#ifdef G__ASM_DBG
      break;
#else
      goto pcode_parse_start;
#endif



    case G__BASEDESTRUCT:
      /***************************************
      * 0 BASEDESTRUCT:
      * 1 tagnum
      * 2 isarray
      ***************************************/
#ifdef G__ASM_DBG
      if(G__asm_dbg) G__fprinterr(G__serr,"%3x,%d: BASEDESTRUCT tagnum=%d\n"
			     ,pc,sp,G__asm_inst[pc+1]);
#endif
      store_tagnum = G__tagnum;
      G__tagnum = G__asm_inst[pc+1];
      store_struct_offset = G__store_struct_offset;
      size = G__struct.size[G__tagnum];
      if(G__asm_inst[pc+2]) pinc=G__free_newarraylist(G__store_struct_offset);
      else pinc=1;
      G__asm_exec = 0;
      for(i=pinc-1;i>=0;--i) {
	G__basedestructor();
	G__store_struct_offset += size;
      }
      G__asm_exec = 1;
      G__store_struct_offset = store_struct_offset;
      G__tagnum = store_tagnum;
      pc+=3;
      if(G__return==G__RETURN_TRY) {
	if(G__CATCH!=G__dasm(G__serr,1)) {
	  G__asm_exec=0;
	  return(1);
	}
	G__asm_exec=1;
      }
#ifdef G__ASM_DBG
      break;
#else
      goto pcode_parse_start;
#endif


    case G__REDECL:
      /***************************************
      * 0 REDECL:
      * 1 ig15
      * 2 var
      * stack
      * sp-2
      * sp-1           ->
      * sp
      ***************************************/
      var = (struct G__var_array*)G__asm_inst[pc+2];
      var->p[G__asm_inst[pc+1]] = G__int(G__asm_stack[sp-1]);
#ifdef G__ASM_DBG
      if(G__asm_dbg) G__fprinterr(G__serr,"%3x,%d: REDECL %s 0x%lx\n"
			     ,pc,sp,var->varnamebuf[G__asm_inst[pc+1]]
			     ,var->p[G__asm_inst[pc+1]]);
#endif
      pc+=3;
      --sp;
#ifdef G__ASM_DBG
      break;
#else
      goto pcode_parse_start;
#endif


    case G__TOVALUE:
      /***************************************
      * 0 TOVALUE:
      * (1 p2f)   (1401)
      * sp-1           ->
      * sp
      ***************************************/
#ifdef G__ASM_DBG
      if(G__asm_dbg) G__fprinterr(G__serr,"%3x,%d: TOVALUE\n",pc,sp);
#endif
      {
	G__p2f_tovalue p2f_tovalue = (G__p2f_tovalue)G__asm_inst[pc+1];
	(*p2f_tovalue)(&G__asm_stack[sp-1]);
	pc+=2;
      }
#ifdef G__ASM_DBG
      break;
#else
      goto pcode_parse_start;
#endif


    case G__INIT_REF:
      /***************************************
      * inst
      * 0 G__INIT_REF
      * 1 index
      * 2 paran       // not used, always 0?
      * 3 point_level
      * 4 var_array pointer
      * stack
      * sp-paran 
      * sp-2
      * sp-1            <-
      * sp
      ***************************************/
#ifdef G__ASM_DBG
      if(G__asm_dbg) G__fprinterr(G__serr,"%3x,%d: INIT_REF index=%d paran=%d point %c"
			     ,pc,sp,G__asm_inst[pc+1],G__asm_inst[pc+2]
			     ,G__asm_inst[pc+3]);
#endif
#ifdef G__ASM_DBG
      if(G__asm_dbg) G__fprinterr(G__serr,"  value=%ld,%p ref=%p\n"
				  ,G__int(G__asm_stack[sp-1])
				  ,G__int(G__asm_stack[sp-1])
	                          ,G__asm_stack[sp-1].ref);
#endif
      var = (struct G__var_array*)G__asm_inst[pc+4];
      *(long*)(var->p[G__asm_inst[pc+1]]+localmem)=G__asm_stack[sp-1].ref;
      pc+=5;
#ifdef G__ASM_DBG
      break;
#else
      goto pcode_parse_start;
#endif

    case G__PUSHCPY:
      /***************************************
      * inst
      * 0 G__PUSHCPY
      * stack
      * sp
      * sp+1            <-
      ***************************************/
#ifdef G__ASM_DBG
      if(G__asm_dbg) G__fprinterr(G__serr,"%3x,%d: PUSHCPY %g,%p,ref=%p\n"
				  ,pc,sp,G__double(G__asm_stack[sp-1])
				  ,G__int(G__asm_stack[sp-1])
				  ,G__asm_stack[sp-1].ref);
#endif
      ++pc;
      G__asm_stack[sp]=G__asm_stack[sp-1];
/* #define G__OLDIMPLEMENTATION2078 */
      ++sp;
#ifdef G__ASM_DBG
      break;
#else
      goto pcode_parse_start;
#endif

    case G__LETNEWVAL:
      /***************************************
      * inst
      * 0 LETNEWVAL
      * stack
      * sp-2  a
      * sp-1  b          <-
      * sp
      ***************************************/
#ifdef G__ASM_DBG
      if(G__asm_dbg) G__fprinterr(G__serr,"%3x,%d: LETNEWVAL\n" ,pc,sp);
#endif
      G__letvalue(&G__asm_stack[sp-2],G__asm_stack[sp-1]);
      ++pc;
      --sp;
#ifdef G__ASM_DBG
      break;
#else
      goto pcode_parse_start;
#endif

    case G__SETGVP:
      /***************************************
      * inst
      * 0 SETGVP
      * 1 p or flag      0:use stack-1, 1:push, -1:pop
      * stack
      * sp-1  b          <-
      * sp
      ***************************************/
#ifdef G__ASM_DBG
      if(G__asm_dbg) 
	G__fprinterr(G__serr,"%3x,%d: SETGVP %d %d\n",pc,sp,G__asm_inst[pc+1],gvpp);
#endif
      switch(G__asm_inst[pc+1]) {
      case -1:
	if(gvpp) G__globalvarpointer = store_globalvarpointer[--gvpp];
	break;
      case 0:
	store_globalvarpointer[gvpp++] = G__globalvarpointer;
	G__globalvarpointer = G__asm_stack[sp-1].obj.i;
	break;
      case 1:
	store_globalvarpointer[gvpp++] = G__globalvarpointer;
	G__globalvarpointer = G__store_struct_offset;
	break;
      default: /* looks like this case is not used. 2004/7/19 */
	store_globalvarpointer[gvpp++] = G__globalvarpointer;
	G__globalvarpointer = G__asm_inst[pc+1];
	break;
      }
      pc+=2;
#ifdef G__ASM_DBG
      break;
#else
      goto pcode_parse_start;
#endif

    case G__TOPVALUE:
      /***************************************
      * 0 TOPVALUE:
      * sp-1           ->
      * sp
      ***************************************/
#ifdef G__ASM_DBG
      if(G__asm_dbg) G__fprinterr(G__serr,"%3x,%d: TOPVALUE",pc,sp);
#endif
      G__asm_toXvalue(&G__asm_stack[sp-1]);
#ifdef G__ASM_DBG
      if(G__asm_dbg) G__fprinterr(G__serr," %x\n",G__asm_stack[sp-1].obj.i);
#endif
      ++pc;
#ifdef G__ASM_DBG
      break;
#else
      goto pcode_parse_start;
#endif

#ifndef G__OLDIMPLEMENTATION1073
    case G__CTOR_SETGVP: 
      /***************************************
      * inst
      * 0 CTOR_SETGVP
      * 1 index
      * 2 var_array pointer
      * 3 mode, 0 local block scope, 1 member offset, 2 static
      ***************************************/
      store_globalvarpointer[gvpp++] = G__globalvarpointer; /* ??? */
      var=(struct G__var_array*)G__asm_inst[pc+2];
      switch(G__asm_inst[pc+3]) {
      case 0:
        G__globalvarpointer = localmem+var->p[G__asm_inst[pc+1]];
        break;
      case 1:
        G__globalvarpointer = G__store_struct_offset+var->p[G__asm_inst[pc+1]];
        break;
      case 2: /* not used so far. Just for generality  */
        G__globalvarpointer = var->p[G__asm_inst[pc+1]];
        break;
      }
#ifdef G__ASM_DBG
      if(G__asm_dbg) 
	G__fprinterr(G__serr,"%3x,%d: CTOR_SETGVP %p\n",pc,sp
		     ,G__globalvarpointer);
#endif
      pc+=4;
#ifdef G__ASM_DBG
      break;
#else
      goto pcode_parse_start;
#endif
#endif /* ON1073 */


    case G__TRY:
      /***************************************
      * inst
      * 0 TRY
      * 1 first_catchblock 
      * 2 endof_catchblock
      ***************************************/
#ifdef G__ASM_DBG
      if(G__asm_dbg) 
	G__fprinterr(G__serr,"%3x,%d: TRY %lx %lx\n",pc,sp,G__asm_inst[pc+1],G__asm_inst[pc+2]);
#endif
      {
        switch(G__bc_exec_try_bytecode(pc+3,sp,presult,localmem)) {
        case G__TRY_NORMAL:
          pc=G__asm_inst[pc+2];
          break;
        case G__TRY_INTERPRETED_EXCEPTION:
        case G__TRY_COMPILED_EXCEPTION:
          G__delete_autoobjectstack(G__scopelevel);
          G__asm_stack[sp++]=G__exceptionbuffer;
          pc=G__asm_inst[pc+1];
          break;
        case G__TRY_UNCAUGHT:
        default:
          /* pc+=3; */
          break;
        }
      }
#ifdef G__ASM_DBG
      break;
#else
      goto pcode_parse_start;
#endif

    case G__TYPEMATCH:
      /***************************************
      * inst
      * 0 TYPEMATCH
      * 1 address in data stack
      * stack
      * sp-1    a      <- comparee
      * sp             <- ismatch
      ***************************************/
#ifdef G__ASM_DBG
      if(G__asm_dbg) 
	G__fprinterr(G__serr,"%3x,%d: TYPEMATCH %ld\n",pc,sp,G__asm_inst[pc+1]);
#endif
      G__letint(&G__asm_stack[sp],'i',
		(long)G__bc_exec_typematch_bytecode(&G__asm_stack[G__asm_inst[pc+1]],
						    &G__asm_stack[sp-1]));
      pc+=2;
      ++sp;
#ifdef G__ASM_DBG
      break;
#else
      goto pcode_parse_start;
#endif

    case G__ALLOCEXCEPTION:
      /***************************************
      * inst
      * 0 ALLOCEXCEPTION
      * 1 tagnum
      * stack
      * sp    a
      * sp+1             <-
      ***************************************/
#ifdef G__ASM_DBG
      if(G__asm_dbg) 
	G__fprinterr(G__serr,"%3x,%d: ALLOCEXCEPTION %ld\n"
                     ,pc,sp,G__asm_inst[pc+1]);
#endif
      G__asm_stack[sp] = G__alloc_exceptionbuffer(G__asm_inst[pc+1]);
      store_struct_offset = G__store_struct_offset;
      store_tagnum = G__tagnum;
      store_return=G__return;
      G__store_struct_offset = G__asm_stack[sp].obj.i;
      G__tagnum = G__asm_stack[sp].tagnum;
      G__return=G__RETURN_NON; /* ??? */
      pc+=2;
#ifdef G__ASM_DBG
      break;
#else
      goto pcode_parse_start;
#endif

    case G__DESTROYEXCEPTION:
      /***************************************
      * inst
      * 0 DESTROYEXCEPTION
      ***************************************/
#ifdef G__ASM_DBG
      if(G__asm_dbg) 
	G__fprinterr(G__serr,"%3x,%d: DESTROYEXCEPTION\n",pc,sp);
#endif
      G__free_exceptionbuffer();
      ++pc;
#ifdef G__ASM_DBG
      break;
#else
      goto pcode_parse_start;
#endif

    case G__THROW:
      /***************************************
      * inst
      * 0 THROW
      * stack
      * sp-1    <-
      * sp
      ***************************************/
#ifdef G__ASM_DBG
      if(G__asm_dbg) G__fprinterr(G__serr,"%3x,%d: THROW\n",pc,sp);
#endif
      // TODO, it is questionable to set G__exceptionbuffer here. 
      // Maybe better setting this in catch block in G__bc_try_bytecode()
      G__exceptionbuffer = G__asm_stack[sp-1]; 

      G__bc_exec_throw_bytecode(&G__asm_stack[sp-1]);
      return(1);

    case G__CATCH:
      /***************************************
      * inst         This instruction is not needed. Never used
      * 0 CATCH
      * 1 filenum
      * 2 linenum
      * 3 pos
      * 4  "
      ***************************************/
      pc+=5;
      /* Do nothing here and skip catch block for normal execution */
#ifdef G__ASM_DBG
      break;
#else
      goto pcode_parse_start;
#endif

    case G__SETARYINDEX:
      /***************************************
      * inst
      * 0 SETARYINDEX
      * 1 allocflag, 1: new object, 0: auto object
      ***************************************/
#ifdef G__ASM_DBG
      if(G__asm_dbg) G__fprinterr(G__serr,"%3x,%d: SETARYINDEX\n",pc,sp);
#endif
      store_cpp_aryindex[store_cpp_aryindexp++] = G__cpp_aryconstruct;
      G__cpp_aryconstruct = G__int(G__asm_stack[sp-1]);
      if(G__asm_inst[pc+1]) --sp;
      pc+=2;
#ifdef G__ASM_DBG
      break;
#else
      goto pcode_parse_start;
#endif

    case G__RESETARYINDEX:
      /***************************************
      * inst
      * 0 RESETARYINDEX
      * 1 allocflag, 1: new object, 0: auto object
      ***************************************/
#ifdef G__ASM_DBG
      if(G__asm_dbg) G__fprinterr(G__serr,"%3x,%d: RESETARYINDEX\n",pc,sp);
#endif
      if(G__asm_inst[pc+1]) {
	G__alloc_newarraylist(G__int(G__asm_stack[sp-1]),G__cpp_aryconstruct);
      }
      if(store_cpp_aryindexp>0)
	G__cpp_aryconstruct = store_cpp_aryindex[--store_cpp_aryindexp];
      pc+=2;
#ifdef G__ASM_DBG
      break;
#else
      goto pcode_parse_start;
#endif

    case G__GETARYINDEX:
      /***************************************
      * inst
      * 0 GETARYINDEX
      ***************************************/
#ifdef G__ASM_DBG
      if(G__asm_dbg) G__fprinterr(G__serr,"%3x,%d: GETARYINDEX\n",pc,sp);
#endif
      store_cpp_aryindex[store_cpp_aryindexp++] = G__cpp_aryconstruct;
      G__cpp_aryconstruct = G__free_newarraylist(G__store_struct_offset);
      ++pc;
#ifdef G__ASM_DBG
      break;
#else
      goto pcode_parse_start;
#endif


    case G__ENTERSCOPE:
      /***************************************
      * inst
      * 0 ENTERSCOPE
      ***************************************/
#ifdef G__ASM_DBG
      if(G__asm_dbg) G__fprinterr(G__serr,"%3x,%d: ENTERSCOPE\n",pc,sp);
#endif
      ++G__scopelevel;
      ++pc;
#ifdef G__ASM_DBG
      break;
#else
      goto pcode_parse_start;
#endif

    case G__EXITSCOPE:
      /***************************************
      * inst
      * 0 EXITSCOPE
      ***************************************/
#ifdef G__ASM_DBG
      if(G__asm_dbg) G__fprinterr(G__serr,"%3x,%d: EXITSCOPE\n",pc,sp);
#endif
      G__delete_autoobjectstack(--G__scopelevel);
      ++pc;
#ifdef G__ASM_DBG
      break;
#else
      goto pcode_parse_start;
#endif

    case G__PUTAUTOOBJ:
      /***************************************
      * inst
      * 0 PUTAUTOOBJ
      * 1 var
      * 2 ig15
      ***************************************/
#ifdef G__ASM_DBG
      if(G__asm_dbg) 
       G__fprinterr(G__serr,"%3x,%d: PUTAUTOOBJ\n",pc,sp);
#endif
      var=(struct G__var_array*)G__asm_inst[pc+1];
      i=(int)G__asm_inst[pc+2];
      G__push_autoobjectstack((void*)(localmem+var->p[i])
			      ,var->p_tagtable[i],var->varlabel[i][1]+1
                              ,G__scopelevel,0);
      pc+=3;
#ifdef G__ASM_DBG
      break;
#else
      goto pcode_parse_start;
#endif

    case G__CASE:
      /***************************************
      * inst
      * 0 CASE
      * 1 *casetable
      * stack
      * sp-1         <- 
      * sp
      ***************************************/
#ifdef G__ASM_DBG
      if(G__asm_dbg) 
       G__fprinterr(G__serr,"%3x,%d: CASE\n",pc,sp);
#endif
      pc = G__bc_casejump((void*)G__asm_inst[pc+1],G__int(G__asm_stack[sp-1]));
      /* pc+=2; */
      --sp;
#ifdef G__ASM_DBG
      break;
#else
      goto pcode_parse_start;
#endif


    case G__MEMCPY:
      /***************************************
      * inst
      * 0 MEMCPY
      * stack
      * sp-4
      * sp-3        ORIG  
      * sp-2        DEST  <- sp-2   this implementation is quetionable
      * sp-1        SIZE
      * sp
      ***************************************/
#ifdef G__ASM_DBG
      if(G__asm_dbg) G__fprinterr(G__serr,"%3x,%d: MEMCPY %lx %lx %ld\n",pc,sp
				  ,G__int(G__asm_stack[sp-2])
				  ,G__int(G__asm_stack[sp-3])
				  ,G__int(G__asm_stack[sp-1]));
#endif
      memcpy((void*)G__int(G__asm_stack[sp-2]),(void*)G__int(G__asm_stack[sp-3]),
	     G__int(G__asm_stack[sp-1]));
      ++pc;
      /* -3 might be better, because MEMCPY instruction is only used in 
       *implicit copy ctor and operator=. Need to be careful about other use */
      sp -= 2;  
#ifdef G__ASM_DBG
      break;
#else
      goto pcode_parse_start;
#endif

    case G__MEMSETINT:
      /***************************************
      * inst
      * 0 MEMSETINT
      * 1 mode,  0:no offset, 1: G__store_struct_offset, 2: localmem
      * 2 numdata
      * 3 adr
      * 4 data
      * 5 adr
      * 6 data
      * ...
      ***************************************/
#ifdef G__ASM_DBG
      if(G__asm_dbg) G__fprinterr(G__serr,"%3x,%d: MEMSETINT %ld %ld\n",pc,sp
				  ,G__asm_inst[pc+1],G__asm_inst[pc+2]);
#endif
      {
        int i;
	int n=G__asm_inst[pc+2];
	long plong;
	switch(G__asm_inst[pc+1]) {
        case 0: plong=0; break;
        case 1: plong=G__store_struct_offset; break;
        case 2: plong=localmem; break;
        default: plong=0; break;
	}
        for(i=0;i<n;++i) {
#ifdef G__ASM_DBG
	  if(G__asm_dbg) G__fprinterr(G__serr,"  %ld %ld\n"
				      ,G__asm_inst[pc+3+i*2]
				      ,G__asm_inst[pc+4+i*2]);
#endif
	  *(long*)(plong+G__asm_inst[pc+3+i*2]) = G__asm_inst[pc+4+i*2];
        }
      }
      pc+=G__asm_inst[pc+2]*2+3;
#ifdef G__ASM_DBG
      break;
#else
      goto pcode_parse_start;
#endif

    case G__JMPIFVIRTUALOBJ:
      /***************************************
      * inst
      * 0 JMPIFVIRTUALOBJ
      * 1 offset
      * 2 next_pc
      ***************************************/
#ifdef G__ASM_DBG
      if(G__asm_dbg) G__fprinterr(G__serr,"%3x,%d: JMPIFVIRTUALOBJ %lx %lx\n"
				  ,pc,sp,G__asm_inst[pc+1],G__asm_inst[pc+2]);
#endif
      {
        long *pvos= (long*)(G__store_struct_offset + G__asm_inst[pc+1]);
	if(*pvos<0) pc=G__asm_inst[pc+2];
	else pc+=3;
      }
#ifdef G__ASM_DBG
      break;
#else
      goto pcode_parse_start;
#endif


    case G__VIRTUALADDSTROS:
      /***************************************
      * inst
      * 0 VIRTUALADDSTROS
      * 1 tagnum
      * 2 baseclass
      * 3 basen
      ***************************************/
#ifdef G__ASM_DBG
      if(G__asm_dbg) G__fprinterr(G__serr,"%3x,%d: VIRTUALADDSTROS %lx %lx\n"
				  ,pc,sp,G__asm_inst[pc+1],G__asm_inst[pc+3]);
#endif
      {
	int tagnum = G__asm_inst[pc+1];
	struct G__inheritance *baseclass 
	  = (struct G__inheritance*)G__asm_inst[pc+2];
	int basen = G__asm_inst[pc+3];
	G__store_struct_offset+=G__getvirtualbaseoffset(G__store_struct_offset
						     ,tagnum,baseclass,basen);
      }
      pc+=4;
#ifdef G__ASM_DBG
      break;
#else
      goto pcode_parse_start;
#endif

    case G__PAUSE:
      /***************************************
      * inst
      * 0 PAUSE
      ***************************************/
#ifdef G__ASM_DBG
      if(G__asm_dbg) G__fprinterr(G__serr,"%3x,%d: PAUSE\n",pc,sp);
#endif
      printf("%3x,%d: PAUSE ",pc,sp);
      printf("inst=%p stack=%p localmem=%lx tag=%d stros=%lx gvp=%lx\n"
	     ,G__asm_inst,G__asm_stack,localmem
	     ,G__tagnum
	     ,G__store_struct_offset
	     ,G__globalvarpointer);
      G__pause();
      ++pc;
#ifdef G__ASM_DBG
      break;
#else
      goto pcode_parse_start;
#endif

#ifdef G__NEVER_BUT_KEEP
    case G__NOP:
      /***************************************
      * 0 NOP
      ***************************************/
#ifdef G__ASM_DBG
      if(G__asm_dbg) G__fprinterr(G__serr,"%3x,%d: NOP\n" ,pc,sp);
#endif
      ++pc;
#ifdef G__ASM_DBG
      break;
#else
      goto pcode_parse_start;
#endif
#endif /* G__NEVER_BUT_KEEP */

    default:
      /***************************************
      * Illegal instruction.
      * This is a double check and should
      * never happen.
      ***************************************/
#ifdef G__ASM_DBG
      if(G__asm_dbg) G__fprinterr(G__serr,"%3x,%d: ILLEGAL INST\n" ,pc,sp);
#endif
      G__asm_execerr("Illegal instruction",(int)G__asm_inst[pc]);
      G__dasm(stderr,0);
      return(0);
    }

#ifdef G__ASM_DBG
    /****************************************
     * Error that sp exceeded remaining data
     * stack depth G__asm_dt.
     * It is unlikely but this error could
     * occur if too many constants appears
     * within compiled loop and there are
     * deep nesting expression.
     ****************************************/
#ifdef G__ASM_DBG
    if(sp>=G__asm_dt) {
      G__asm_execerr("Data stack overflow",sp);
      return(0);
    }
#endif

#ifdef G__ASM_DBG
  }
#else
  goto pcode_parse_start;
#endif


  /****************************************
   * Error that pc exceeded G__MAXINST
   * This is a double check and should never
   * happen.
   ****************************************/
  G__asm_execerr("Instruction memory overrun",pc);
  return(0);
#endif
}



