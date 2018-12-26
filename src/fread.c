/* /% C %/ */
/***********************************************************************
 * cint (C/C++ interpreter)
 ************************************************************************
 * Source file fread.c
 ************************************************************************
 * Description:
 *  Utility to read source file
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

#include "common.h"


#define G__storeOrigPos                                          \
   int store_linenum = G__ifile.line_number

#define G__eofOrigError                                          \



#ifdef G__MULTIBYTE
/***********************************************************************
* G__CodingSystem()
***********************************************************************/
int G__CodingSystem(c)
int c;
{
  c &= 0x7f;
  switch(G__lang) {
  case G__UNKNOWNCODING:
    if(0x1f<c&&c<0x60) {
      /* assuming there is no half-sized kana chars, this code does not
       * exist in S-JIS, set EUC flag and return 0 */
      G__lang=G__EUC;
      return(0); 
    }
    return(1); /* assuming S-JIS but not sure yet */
  case G__EUC:
    return(0);
  case G__SJIS:
    if(c<=0x1f || (0x60<=c && c<=0x7c)) return(1);
    else                                return(0);
  case G__ONEBYTE: 
    return(0);
  }
  return(1);
}
#endif

/***********************************************************************
* G__isstoragekeyword()
***********************************************************************/
static int G__isstoragekeyword(buf)
char *buf;
{
  if(!buf) return(0);
  if(strcmp(buf,"const")==0 ||
     strcmp(buf,"unsigned")==0 ||
     strcmp(buf,"signed")==0 ||
     strcmp(buf,"int")==0 ||
     strcmp(buf,"long")==0 ||
     strcmp(buf,"short")==0 
#ifndef G__OLDIMPLEMENTATION1855
     || strcmp(buf,"char")==0
#endif
#ifndef G__OLDIMPLEMENTATION1859
     || strcmp(buf,"double")==0
     || strcmp(buf,"float")==0
#endif
     || strcmp(buf,"volatile")==0 
     || strcmp(buf,"register")==0 
     || (G__iscpp && strcmp(buf,"typename")==0)
     ) {
    return(1);
  }
  else {
    return(0);  }
}

/***********************************************************************
* G__fgetname_template(string,endmark)
*
*
* char *string       : string until the endmark appears
* char *endmark      : specify endmark characters
*
*   read one non-space char string upto next space char or endmark
*  char.
*
* 1) skip space char until non space char appears
* 2) Store non-space char to char *string. If space char is surrounded by
*   quotation, it is stored.
* 3) if space char or one of endmark char which is not surrounded by
*   quotation appears, stop reading and return the last char.
*
*
*   '     azAZ09*&^%/     '
*    ----------------^        return(' ');
*
* if ";" is given as end mark
*   '     azAZ09*&^%/;  '
*    ----------------^        return(';');
*
***********************************************************************/
int G__fgetname_template(string,endmark)
char *string,*endmark;
{
  short i=0,l;
  int c,prev;
  short single_quote=0,double_quote=0,flag=0,spaceflag,ignoreflag;
  int nest=0;
  int tmpltnest=0;
  char *pp = string;
  int pflag = 0;
  
  spaceflag=0;

  do {
    ignoreflag=0;
    c=G__fgetc() ;
    
    if((single_quote==0)&&(double_quote==0)&&nest==0) {
      l=0;
      while((prev=endmark[l++])!='\0') {
	if(c==prev) {
	  flag=1;
	  ignoreflag=1;
	}
      }
    }

  backtoreadtemplate:
    
    switch(c) {
    case ' ':
    case '\t':
    case '\n':
    case '\r':
    case '\f':
      if((single_quote==0)&&(double_quote==0)) {
	string[i] = '\0';  /* temporarily close the string */
	if(tmpltnest) {
	  if(G__isstoragekeyword(pp)) {
	    if(G__iscpp && strcmp("typename",pp)==0) {
	      i -= 8;
	      c=' ';
	      ignoreflag = 1;
	    }
	    else {
	      pp=string+i+1;
	      c=' ';
	    }
	    break;
	  }
	  else if('*'==string[i-1]) {
	    pflag = 1;
	  }
	}
	if(strlen(pp)<8 && strncmp(pp,"typename",8)==0 && pp!=string) {
	  i -= 8;
	}
	ignoreflag=1;
	if(spaceflag!=0&&0==nest) flag=1;
      }
      break;
    case '"':
      if(single_quote==0) {
	spaceflag=1;
	double_quote ^= 1;
      }
      break;
    case '\'':
      if(double_quote==0) {
	spaceflag=1;
	single_quote ^= 1;
      }
      break;

    case '<':
      if((single_quote==0)&&(double_quote==0)&&
	 strncmp(pp,"operator",8)!=0
	 ) {
	int lnest=0;
	++nest;
	string[i] = '\0';
        pp = string+i;
        while(pp>string && (pp[-1]!='<' || lnest) 
	      && pp[-1]!=',' && pp[-1]!=' ') {
	  switch(pp[-1]) {
	  case '>': ++lnest; break;
	  case '<': --lnest; break;
	  }
	  --pp;
	}
        if(G__defined_templateclass(pp)) ++tmpltnest;
	pp = string+i+1;
      }
      spaceflag=1;
      break;

    case '(':
      if((single_quote==0)&&(double_quote==0)) {
	pp = string+i+1;
	++nest;
      }
      spaceflag=1;
      break;

    case '>':
      if((single_quote==0)&&(double_quote==0)&&
	 strncmp(string,"operator",8)!=0) {
	--nest;
	if(tmpltnest) --tmpltnest;
	if(nest<0) {
	  string[i]='\0';
	  return(c);
	}
	else if(i && '>'==string[i-1]) {
	  /* A<A<int> > */
	  string[i++]=' ';
	}
	else if(i>2 && isspace(string[i-1]) && '>'!=string[i-2]) {
	  --i;
	}
      }
      spaceflag=1;
      break;
    case ')':
      if((single_quote==0)&&(double_quote==0)) {
	--nest;
	if(nest<0) {
	  string[i]='\0';
	  return(c);
	}
      }
      spaceflag=1;
      break;
    case '/':
      if((single_quote==0)&&(double_quote==0)) {
	/* comment */
	string[i++] = c ;
	
	c=G__fgetc();
	switch(c) {
	case '*':
	  G__skip_comment();
	  --i;
	  ignoreflag=1;
	  break;
	case '/':
	  G__fignoreline();
	  --i;
	  ignoreflag=1;
	  break;
	default:
	  fseek(G__ifile.fp,-1,SEEK_CUR);
	  if(G__dispsource) G__disp_mask=1;
	  spaceflag=1;
	  ignoreflag=1;
	  break;
	}
      }
      
      break;
      
    case '#':
      if(single_quote==0&&double_quote==0&&(i==0||string[i-1]!='$')) {
	G__pp_command();
	ignoreflag=1;
#ifdef G__TEMPLATECLASS
	c=' ';
#endif
      }
      break;

    case EOF:
      G__unexpectedEOF("G__fgetname():2");
      string[i] = '\0';
      return(c);

    case '*':
    case '&':
      if(i>0 && ' '==string[i-1] && nest && single_quote==0&&double_quote==0) 
	--i;
      break;

    case ',':
      pp = string+i+1;

    default:
      spaceflag=1;
#ifdef G__MULTIBYTE
      if(G__IsDBCSLeadByte(c) && !ignoreflag) {
	string[i++]=c;
	c=G__fgetc() ;
	G__CheckDBCS2ndByte(c);
      }
#endif
      break;
    }
    
    if(ignoreflag==0) {
      if(pflag && (isalpha(c) || '_'==c)) {
	string[i++] = ' ' ;
      }
      pflag = 0;
      string[i++] = c ;
      G__CHECK(G__SECURE_BUFFER_SIZE,i>=G__LONGLINE,return(EOF));
    }
    
  } while(flag==0) ;

  if(isspace(c)) {
    c=G__fgetspace();
    l=0;
    flag=0;
    while((prev=endmark[l++])!='\0') {
      if(c==prev) {
	flag=1;
      }
    }
    if(!flag) {
      if('<'==c) {
	if(strncmp(string,"operator",8)==0) string[i++]=c;
	flag=ignoreflag=0;
	goto backtoreadtemplate;
      }
      fseek(G__ifile.fp,-1,SEEK_CUR);
      if(G__dispsource) G__disp_mask=1;
      c=' ';
    }
  }
  
  string[i]='\0';
  
  return(c);
}
/***********************************************************************
* G__fgetstream_newtemplate(string,endmark)
*
* Called by
*   G__exec_statement()
*   G__exec_statement()
*
* char *string       : string until the endmark appears
* char *endmark      : specify endmark characters
*
*  read source file until specified endmark char appears.
*
* 1) read source file and store char to char *string.
*   If char is space char which is not surrounded by quoatation
*   it is not stored into char *string.
* 2) When one of endmark char appears or parenthesis nesting of
*   parenthesis gets negative , like '())' , stop reading and 
*   return the last char.
*
*  *endmark=";"
*     '  ab cd e f g;hijklm '
*      -------------^          *string="abcdefg"; return(';');
*
*  *endmark=";"
*     ' abc );'
*      -----^    *string="abc"; return(')');
*
*     'abc=new xxx;'
*     'func(new xxx);'
*
***********************************************************************/
int G__fgetstream_newtemplate(string,endmark)
char *string,*endmark;
{
  short i=0,l;
  int c,prev;
  int nest=0,single_quote=0,double_quote=0,flag=0,ignoreflag;
  int commentflag=0;
  char *pp = string;
  short inew=0;

  do {
    ignoreflag=0;
    c=G__fgetc() ;
    
    if((nest<=0)&&(single_quote==0)&&(double_quote==0)) {
      l=0;
      while((prev=endmark[l++])!='\0') {
	if(c==prev) {
	  flag=1;
	  ignoreflag=1;
	}
      }
    }
    
    switch(c) {
    case ' ':
    case '\f':
    case '\n':
    case '\r':
    case '\t':
      string[i] = '\0';
      if(G__isstoragekeyword(pp)) {
	pp=string+i+1;
	commentflag=0;
	c=' ';
	break;
      }
      commentflag=0;
      if((single_quote==0)&&(double_quote==0)) {
	c=' ';
	switch(i-inew) {
	case 3:
	  if(strncmp(string+inew,"new",3)!=0)
	    ignoreflag=1;
	  break;
	default:
	  inew=i;
	  ignoreflag=1;
	  break;
	}
      }
      break;
    case ',':
      /* may be following line is needed. 1999/5/31 */
      /* if(i>2 && ' '==string[i-1] && isalnum(string[i-2])) --i; */
      pp = string+i+1;
    case '=':
      if((single_quote==0)&&(double_quote==0)) {
	inew=i+1;
      }
      break;
    case '<':
      if((single_quote==0)&&(double_quote==0)) {
	string[i]=0;
        if(G__defined_templateclass(pp)) ++nest;
	inew=i+1;
	pp = string+i+1;
      }
      break;
    case '{':
    case '(':
    case '[':
      if((single_quote==0)&&(double_quote==0)) {
	++nest;
	inew=i+1;
#ifdef G__OLDIMPLEMENTATION1520_YET_BUG
	pp = string+i+1; /* This creates a side effect with stl/demo/testall */
#endif
      }
      break;
    case '>':
      if(0==nest||(i&&'-'==string[i-1])) break;
      else if(nest && i && '>'==string[i-1]
	      && 0==double_quote && 0==single_quote
	      ) string[i++]=' ';
    case '}':
    case ')':
    case ']':
      if((single_quote==0)&&(double_quote==0)) {
	/* may be following line is needed. 1999/5/31 */
	/* if(i>2 && ' '==string[i-1] && isalnum(string[i-2])) --i; */
	nest--;
	inew=i+1;
      }
      break;
    case '"':
      if(single_quote==0) {
	double_quote ^= 1;
      }
      break;
    case '\'':
      if(double_quote==0) {
	single_quote ^= 1;
      }
      break;
      
    case '\\':
      if(ignoreflag==0) {
	string[i++] = c ;
	c=G__fgetc() ;
      }
      break;
      
    case '/':
      if(0==double_quote && 0==single_quote && i>0 && string[i-1]=='/' &&
	 commentflag) {
	i--;
	G__fignoreline();
	ignoreflag=1;
      }
      else {
	commentflag=1;
      }
      break;

    case '&':
      if(i>0 && ' '==string[i-1] && nest && single_quote==0&&double_quote==0) 
	--i;
      break;
      
    case '*':
      /* comment */
      if(0==double_quote && 0==single_quote) {
	if(i>0 && string[i-1]=='/' && commentflag) {
	  G__skip_comment();
	  --i;
	  ignoreflag=1;
	}
	else if(i>2 && isspace(string[i-1]) && 
		(isalnum(string[i-2])||'_'==string[i-2])
		) {
	  --i;
	}
      }
      break;

    case '#':
      if(single_quote==0&&double_quote==0&&(i==0||string[i-1]!='$')) {
	G__pp_command();
	ignoreflag=1;
#ifdef G__TEMPLATECLASS
	c=' ';
#endif
      }
      break;

    case EOF:
      G__unexpectedEOF("G__fgetstream_newtemplate():2");
      string[i] = '\0';
      return(c);

#ifdef G__MULTIBYTE
    default:
      if(G__IsDBCSLeadByte(c) && !ignoreflag) {
	string[i++]=c;
	c=G__fgetc() ;
	G__CheckDBCS2ndByte(c);
      }
      break;
#endif
    }
    
    if(ignoreflag==0) {
      string[i++] = c ;
      G__CHECK(G__SECURE_BUFFER_SIZE,i>=G__LONGLINE,return(EOF));
    }
    
  } while(flag==0) ;
  
  string[i]='\0';
  
  return(c);
}
/***********************************************************************
* G__fgetstream_template(string,endmark)
*
* char *string       : string until the endmark appears
* char *endmark      : specify endmark characters
*
*  read source file until specified endmark char appears.
*
* 1) read source file and store char to char *string.
*   If char is space char which is not surrounded by quoatation
*   it is not stored into char *string.
* 2) When one of endmark char appears or parenthesis nesting of
*   parenthesis gets negative , like '())' , stop reading and 
*   return the last char.
*
*  *endmark=";"
*     '  ab cd e f g;hijklm '
*      -------------^          *string="abcdefg"; return(';');
*
*  *endmark=";"
*     ' abc );'
*      -----^    *string="abc"; return(')');
*
***********************************************************************/
int G__fgetstream_template(string,endmark)
char *string,*endmark;
{
  short i=0,l;
  int c,prev;
  short nest=0,single_quote=0,double_quote=0,flag=0,ignoreflag;
  int commentflag=0;
  char *pp = string;
  int pflag = 0;
  
  
  do {
    ignoreflag=0;
    c=G__fgetc() ;
    
    if((nest==0)&&(single_quote==0)&&(double_quote==0)) {
      l=0;
      while((prev=endmark[l++])!='\0') {
	if(c==prev) {
	  flag=1;
	  ignoreflag=1;
	}
      }
    }
    
    switch(c) {
    case ' ':
    case '\f':
    case '\n':
    case '\r':
    case '\t':
      commentflag=0;
      if((single_quote==0)&&(double_quote==0)) {
	string[i] = '\0';
	if(G__isstoragekeyword(pp)) {
	  if(G__iscpp && strcmp("typename",pp)==0) {
	    i -= 8;
	    c=' ';
	    ignoreflag = 1;
	  }
	  else {
	    pp=string+i+1;
	    c=' ';
	  }
	  break;
	}
	else if(i&&'*'==string[i-1]) {
	  pflag = 1;
	}
#define G__OLDIMPLEMENTATION1894
	ignoreflag=1;
      }
      break;
    case '<':
      if((single_quote==0)&&(double_quote==0)) {
#ifdef G__OLDIMPLEMENTATION1721_YET
	string[i]=0;
        if(G__defined_templateclass(pp)) ++nest;
#endif
	pp = string+i+1;
      }
#ifdef G__OLDIMPLEMENTATION1721_YET
      break;
#endif
    case '{':
    case '(':
    case '[':
      if((single_quote==0)&&(double_quote==0)) {
	pp = string+i+1;
	nest++;
      }
      break;
    case '>':
      if(i&&'-'==string[i-1]) break; /* need to test for >> ??? */
    case '}':
    case ')':
    case ']':
      if((single_quote==0)&&(double_quote==0)) {
	if(i>2 && ' '==string[i-1] && isalnum(string[i-2])) --i;
	nest--;
	if(nest<0) {
	  flag=1;
	  ignoreflag=1;
	}
	else if('>'==c && i && '>'==string[i-1]) {
	  /* A<A<int> > */
	  string[i++]=' ';
	}
      }
      break;
    case '"':
      if(single_quote==0) double_quote ^= 1;
      break;
    case '\'':
      if(double_quote==0) single_quote ^= 1;
      break;
      
    case '\\':
      if(ignoreflag==0) {
	string[i++] = c ;
	c=G__fgetc() ;
      }
      break;
      
    case '/':
      if(0==double_quote && 0==single_quote && i>0 && string[i-1]=='/' &&
	 commentflag) {
	G__fignoreline();
	--i;
	ignoreflag=1;
      }
      else {
	commentflag=1;
      }
      break;

    case '&':
      if(i>0 && ' '==string[i-1] && nest && single_quote==0&&double_quote==0) 
	--i;
      break;
      
    case '*':
      /* comment */
#ifndef G__OLDIMPLEMENTATION1864
      if(0==double_quote && 0==single_quote && i>0) {
	if(string[i-1]=='/' && commentflag) {
	  G__skip_comment();
	  --i;
	  ignoreflag=1;
	}
	else 
	  if(i>2 && 
	     isspace(string[i-1]) && 
	     (isalnum(string[i-2])||'_'==string[i-2])
	     ) {
	  --i;
	}
      }

#else
      if(0==double_quote && 0==single_quote && i>0 && string[i-1]=='/' &&
	 commentflag) {
	G__skip_comment();
	--i;
	ignoreflag=1;
      }
#endif
      break;

    case '#':
      if(single_quote==0&&double_quote==0&&(i==0||string[i-1]!='$')) {
	G__pp_command();
	ignoreflag=1;
#ifdef G__TEMPLATECLASS
	c=' ';
#endif
      }
      break;

    case EOF:
      G__unexpectedEOF("G__fgetstream_template():2");
      string[i] = '\0';
      return(c);
      /* break; */


    case ',':
      if(i>2 && ' '==string[i-1] && isalnum(string[i-2])) --i;
      pp = string+i+1;

#ifdef G__MULTIBYTE
    default:
      if(G__IsDBCSLeadByte(c) && !ignoreflag) {
	string[i++]=c;
	c=G__fgetc() ;
	G__CheckDBCS2ndByte(c);
      }
      break;
#endif
    }
    
    if(ignoreflag==0) {
      if(pflag && (isalpha(c) || '_'==c)) {
	string[i++] = ' ' ;
      }
      pflag = 0;
      string[i++] = c ;
      G__CHECK(G__SECURE_BUFFER_SIZE,i>=G__LONGLINE,return(EOF));
    }
    
  } while(flag==0) ;
  
  string[i]='\0';
  
  return(c);
}
/***********************************************************************
* G__getstream_template(source,isrc,string,endmark)
*
* char *source;      : source string. If NULL, read from input file
* int *isrc;         : char position of the *source if source!=NULL
* char *string       : substring until the endmark appears
* char *endmark      : specify endmark characters
*
*
*  Get substring of char *source; until one of endmark char is found.
* Return string is not used.
*  Only used in G__getexpr() to handle 'cond?true:faulse' opeartor.
*
*   char *endmark=";";
*   char *source="abcdefg * hijklmn ;   "
*                 ------------------^      *string="abcdefg*hijklmn"
*
*   char *source="abcdefg * hijklmn) ;   "
*                 -----------------^       *string="abcdefg*hijklmn"
*
***********************************************************************/
int G__getstream_template(source,isrc,string,endmark)
char *source;
int *isrc;
char *string,*endmark;
{
  short i=0,l;
  int c,prev;
  short nest=0,single_quote=0,double_quote=0,flag=0,ignoreflag;
  char *pp = string;
  int pflag = 0;
  

  do {
    ignoreflag=0;
    c = source[(*isrc)++] ;
    
    if((nest==0)&&(single_quote==0)&&(double_quote==0)) {
      l=0;
      while((prev=endmark[l++])!='\0') {
	if(c==prev) {
	  flag=1;
	  ignoreflag=1;
	}
      }
    }
    
    switch(c) {
    case '"':
      if(single_quote==0) double_quote ^= 1;
      break;
    case '\'':
      if(double_quote==0) single_quote ^= 1;
      break;
    case '<':
      if((single_quote==0)&&(double_quote==0)) {
	pp = string+i+1;
      }
    case '{':
    case '(':
    case '[':
      if((single_quote==0)&&(double_quote==0)) {
	pp = string+i+1;
	nest++;
      }
      break;
    case '>':
      if(i&&'-'==string[i-1]) break;
    case '}':
    case ')':
    case ']':
      if((single_quote==0)&&(double_quote==0)) {
	if(i>2 && ' '==string[i-1] && isalnum(string[i-2])) --i;
	nest--;
	if(nest<0) {
	  flag=1;
	  ignoreflag=1;
	}
	else if('>'==c && i && '>'==string[i-1]) {
	  /* A<A<int> > */
	  string[i++]=' ';
	}
      }
      break;
    case ' ':
    case '\f':
    case '\n':
    case '\r':
    case '\t':
      if((single_quote==0)&&(double_quote==0)) {
	string[i] = '\0';
	if(G__isstoragekeyword(pp)) {
	  if(G__iscpp && strcmp("typename",pp)==0) {
	    i -= 8;
	    c=' ';
	    ignoreflag = 1;
	  }
	  else {
	    pp=string+i+1;
	    c=' ';
	  }
	  break;
	}
	else if('*'==string[i-1]) {
	  pflag = 1;
	}
	ignoreflag=1;
      }
      break;
    case '\0':
      /* if((single_quote==0)&&(double_quote==0)) { */
      flag=1;
      ignoreflag=1;
      /* } */
      break;
    case EOF:
      G__unexpectedEOF("G__getstream()");
      string[i] = '\0';
      break;


    case ',':
      if(i>2 && ' '==string[i-1] && isalnum(string[i-2])) --i;
      pp = string+i+1;

#ifdef G__MULTIBYTE
    default:
      if(G__IsDBCSLeadByte(c) && !ignoreflag) {
	string[i++]=c;
	c=G__fgetc() ;
	G__CheckDBCS2ndByte(c);
      }
      break;
#endif
    }
    
    if(ignoreflag==0) {
      if(pflag && (isalpha(c) || '_'==c)) {
	string[i++] = ' ' ;
      }
      pflag = 0;
      string[i++] = c ;
      G__CHECK(G__SECURE_BUFFER_SIZE,i>=G__LONGLINE,return(EOF));
    }
    
  } while(flag==0) ;
  
  string[i]='\0';
  
  return(c);
}

/***********************************************************************
* G__fgetspace()
*
* Called by
*    G__define_var()     'type xxxx(...'
*    G__define_struct()  'typename  xxxx'
*    G__define_type()    'tagname   xxxx'
*    G__defined_type()   'type varname'
*
*  read source file until non space character appears
*
* 1) read until non-space char appears
* 2) return first non-space char
*
*     '         abcd...'
*      ---------^     return('a');
*
***********************************************************************/
int G__fgetspace()
{
  int c;
  short flag=0;

  do {
    c=G__fgetc() ;
    
    switch(c) {
    case ' ':
    case '\t':
    case '\n':
    case '\r':
    case '\f':
      break;

    case '/':
      /* comment */
      
      c=G__fgetc();
      switch(c) {
      case '*':
	G__skip_comment();
	break;
      case '/':
	G__fignoreline();
	break;
      default:
	fseek(G__ifile.fp,-1,SEEK_CUR);
	if(c=='\n' /* || c=='\r' */ ) --G__ifile.line_number;
	if(G__dispsource) G__disp_mask=1;
	c='/';
	flag=1;
	break;
      }
      break;

    case '#':
      G__pp_command();
#ifdef G__TEMPLATECLASS
      c=' ';
#endif
      break;
      
    case EOF:
      G__unexpectedEOF("G__fgetspace():2");
      return(c);
      /* break; */
    default:
      flag=1;
      break;
    }
    
  } while(flag==0) ;
  
  return(c);
}

/***********************************************************************
* G__fgetvarname(string,endmark)
*
***********************************************************************/
int G__fgetvarname(string,endmark)
char *string;
char *endmark;
{
  short i=0,l;
  int c,prev;
  short nest=0,single_quote=0,double_quote=0,flag=0,spaceflag=0,ignoreflag;
#ifdef G__TEMPLATEMEMFUNC
  int tmpltflag=0;
  int notmpltflag=0;
#endif
  char* pp = string;
  
  do {
    ignoreflag=0;
    c=G__fgetc() ;
    
    if((nest==0)&&(single_quote==0)&&(double_quote==0)) {
      l=0;
      while((prev=endmark[l++])!='\0') {
	if(c==prev) {
	  flag=1;
	  ignoreflag=1;
	}
      }
    }
    
    switch(c) {
    case ' ':
    case '\t':
    case '\n':
    case '\r':
    case '\f':
      if((single_quote==0)&&(double_quote==0)) {
        string[i] = '\0';
        if (tmpltflag && G__isstoragekeyword (pp)) {
          c = ' ';
          pp = string + i + 1;
          break;
        }
        ignoreflag = 1;
	if(nest==0&&spaceflag!=0) {
	  flag=1;
	}
      }
      break;
    case '"':
      if(nest==0&&single_quote==0) {
	spaceflag=1;
	double_quote ^= 1;
      }
      break;
    case '\'':
      if(nest==0&&double_quote==0) {
	spaceflag=1;
	single_quote ^= 1;
      }
      break;
#ifdef G__TEMPLATEMEMFUNC
    case '<':
      if((single_quote==0)&&(double_quote==0)) {
        pp = string + i + 1;
      }
      if(notmpltflag || (8==i && strncmp("operator",string,8)==0) 
	 || (9==i && (strncmp("&operator",string,9)==0||
		      strncmp("*operator",string,9)==0))
	 ) {
	notmpltflag=1;
	break;
      }
      else {
	tmpltflag=1;
      }
#endif
    case '{':
    case '(':
    case '[':
      if((single_quote==0)&&(double_quote==0)) {
	nest++;
      }
      break;
#ifdef G__TEMPLATEMEMFUNC
    case '>':
      if(!tmpltflag) break;
      else if(nest && i && '>'==string[i-1]) string[i++]=' ';
#endif
    case '}':
    case ')':
    case ']':
      if((single_quote==0)&&(double_quote==0)) {
	nest--;
	if(nest<0) {
	  flag=1;
	  ignoreflag=1;
	}
      }
      break;
    case '/':
      if((single_quote==0)&&(double_quote==0)) {
	/* comment */
	string[i++] = c ;
	
	c=G__fgetc();
	switch(c) {
	case '*':
	  G__skip_comment();
	  --i;
	  ignoreflag=1;
	  break;
	case '/':
	  G__fignoreline();
	  --i;
	  ignoreflag=1;
	  break;
	case ' ':
	case '\t':
	case '\n':
	case '\r':
	case '\f':
	  if((single_quote==0)&&(double_quote==0)) {
	    ignoreflag=1;
	    if(nest==0&&spaceflag!=0) {
	      flag=1;
	    }
	  }
	  break;
	case EOF:
	  G__unexpectedEOF("G__fgetvarname():1");
	  string[i] = '\0';
	  return(c);
	default:
	  fseek(G__ifile.fp,-1,SEEK_CUR);
	  if(G__dispsource) G__disp_mask=1;
	  spaceflag=1;
	  ignoreflag=1;
	  break;
	}
      }
      break;

    case '#':
      if(single_quote==0&&double_quote==0&&(i==0||string[i-1]!='$')) {
	G__pp_command();
	ignoreflag=1;
#ifdef G__TEMPLATECLASS
	c=' ';
#endif
      }
      break;

    case EOF:
      G__unexpectedEOF("G__fgetvarname():2");
      string[i] = '\0';
      return(c);

    case ',':
      pp = string + i + 1;
      /* fall through... */

    default:
      spaceflag=1;
#ifdef G__MULTIBYTE
      if(G__IsDBCSLeadByte(c) && !ignoreflag) {
	string[i++]=c;
	c=G__fgetc() ;
	G__CheckDBCS2ndByte(c);
      }
#endif
      break;
    }
    
    if(ignoreflag==0) {
      string[i++] = c ;
      G__CHECK(G__SECURE_BUFFER_SIZE,i>=G__LONGLINE,return(EOF));
    }
    
  } while(flag==0) ;
  
  string[i]='\0';
  
  return(c);
  
}

/***********************************************************************
* G__fgetname(string,endmark)
*
*
* char *string       : string until the endmark appears
* char *endmark      : specify endmark characters
*
*   read one non-space char string upto next space char or endmark
*  char.
*
* 1) skip space char until non space char appears
* 2) Store non-space char to char *string. If space char is surrounded by
*   quotation, it is stored.
* 3) if space char or one of endmark char which is not surrounded by
*   quotation appears, stop reading and return the last char.
*
*
*   '     azAZ09*&^%/     '
*    ----------------^        return(' ');
*
* if ";" is given as end mark
*   '     azAZ09*&^%/;  '
*    ----------------^        return(';');
*
***********************************************************************/
int G__fgetname(string,endmark)
char *string,*endmark;
{
  short i=0,l;
  int c,prev;
  short single_quote=0,double_quote=0,flag=0,spaceflag,ignoreflag;
  
  spaceflag=0;

  do {
    ignoreflag=0;
    c=G__fgetc() ;
    
    if((single_quote==0)&&(double_quote==0)) {
      l=0;
      while((prev=endmark[l++])!='\0') {
	if(c==prev) {
	  flag=1;
	  ignoreflag=1;
	}
      }
    }
    
    switch(c) {
    case ' ':
    case '\t':
    case '\n':
    case '\r':
    case '\f':
      if((single_quote==0)&&(double_quote==0)) {
	ignoreflag=1;
	if(spaceflag!=0) flag=1;
      }
      break;
    case '"':
      if(single_quote==0) {
	spaceflag=1;
	double_quote ^= 1;
      }
      break;
    case '\'':
      if(double_quote==0) {
	spaceflag=1;
	single_quote ^= 1;
      }
      break;
      /*
	case '\0':
	flag=1;
	ignoreflag=1;
	break;
	*/
    case '/':
      if((single_quote==0)&&(double_quote==0)) {
	/* comment */
	string[i++] = c ;
	
	c=G__fgetc();
	switch(c) {
	case '*':
	  G__skip_comment();
	  --i;
	  ignoreflag=1;
	  break;
	case '/':
	  G__fignoreline();
	  --i;
	  ignoreflag=1;
	  break;
	default:
	  fseek(G__ifile.fp,-1,SEEK_CUR);
	  if(G__dispsource) G__disp_mask=1;
	  spaceflag=1;
	  ignoreflag=1;
	  break;
	}
      }
      
      break;
      
    case '#':
      if(single_quote==0&&double_quote==0&&(i==0||string[i-1]!='$')) {
	G__pp_command();
	ignoreflag=1;
#ifdef G__TEMPLATECLASS
	c=' ';
#endif
      }
      break;

    case EOF:
      G__unexpectedEOF("G__fgetname():2");
      string[i] = '\0';
      return(c);
    default:
      spaceflag=1;
#ifdef G__MULTIBYTE
      if(G__IsDBCSLeadByte(c) && !ignoreflag) {
	string[i++]=c;
	c=G__fgetc() ;
	G__CheckDBCS2ndByte(c);
      }
#endif
      break;
    }
    
    if(ignoreflag==0) {
      string[i++] = c ;
      G__CHECK(G__SECURE_BUFFER_SIZE,i>=G__LONGLINE,return(EOF));
    }
    
  } while(flag==0) ;
  
  string[i]='\0';
  
  return(c);
}

/***********************************************************************
* G__getname(source,isrc,string,endmark)
*
*
* char *string       : string until the endmark appears
* char *endmark      : specify endmark characters
*
*   read one non-space char string upto next space char or endmark
*  char.
*
* 1) skip space char until non space char appears
* 2) Store non-space char to char *string. If space char is surrounded by
*   quotation, it is stored.
* 3) if space char or one of endmark char which is not surrounded by
*   quotation appears, stop reading and return the last char.
*
*
*   '     azAZ09*&^%/     '
*    ----------------^        return(' ');
*
* if ";" is given as end mark
*   '     azAZ09*&^%/;  '
*    ----------------^        return(';');
*
***********************************************************************/
int G__getname(source,isrc,string,endmark)
char *source;
int *isrc;
char *string,*endmark;
{
  short i=0,l;
  int c,prev;
  short single_quote=0,double_quote=0,flag=0,spaceflag,ignoreflag;
  
  spaceflag=0;

  do {
    ignoreflag=0;
    c = source[(*isrc)++] ;
    
    if((single_quote==0)&&(double_quote==0)) {
      l=0;
      while((prev=endmark[l++])!='\0') {
	if(c==prev) {
	  flag=1;
	  ignoreflag=1;
	}
      }
    }
    
    switch(c) {
    case ' ':
    case '\t':
    case '\n':
    case '\r':
    case '\f':
      if((single_quote==0)&&(double_quote==0)) {
	ignoreflag=1;
	if(spaceflag!=0) flag=1;
      }
      break;
    case '"':
      if(single_quote==0) {
	spaceflag=1;
	double_quote ^= 1;
      }
      break;
    case '\'':
      if(double_quote==0) {
	spaceflag=1;
	single_quote ^= 1;
      }
      break;
      /*
	case '\0':
	flag=1;
	ignoreflag=1;
	break;
	*/
#ifdef G__NEVER
    case '/':
      if((single_quote==0)&&(double_quote==0)) {
	/* comment */
	string[i++] = c ;
	
	c = source[(*isrc)++] ;
	switch(c) {
	case '*':
	  G__skip_comment();
	  --i;
	  ignoreflag=1;
	  break;
	case '/':
	  G__fignoreline();
	  --i;
	  ignoreflag=1;
	  break;
	default:
	  fseek(G__ifile.fp,-1,SEEK_CUR);
	  if(G__dispsource) G__disp_mask=1;
	  spaceflag=1;
	  ignoreflag=1;
	  break;
	}
      }
      
      break;
      
    case '#':
      if(single_quote==0&&double_quote==0&&(i==0||string[i-1]!='$')) {
	G__pp_command();
	ignoreflag=1;
#ifdef G__TEMPLATECLASS
	c=' ';
#endif
      }
      break;
#endif

    case EOF:
      G__unexpectedEOF("G__fgetname():2");
      string[i] = '\0';
      return(c);
    default:
      spaceflag=1;
#ifdef G__MULTIBYTE
      if(G__IsDBCSLeadByte(c) && !ignoreflag) {
	string[i++]=c;
	c = source[(*isrc)++] ;
	G__CheckDBCS2ndByte(c);
      }
#endif
      break;
    }
    
    if(ignoreflag==0) {
      string[i++] = c ;
      G__CHECK(G__SECURE_BUFFER_SIZE,i>=G__LONGLINE,return(EOF));
    }
    
  } while(flag==0) ;
  
  string[i]='\0';
  
  return(c);
}

/***********************************************************************
 *
 ***********************************************************************/
int G__getfullpath(string,pbegin,i)
char *string;
char *pbegin;
int i;
{
  int tagnum= -1,typenum;
  string[i] = '\0';
  if(0==pbegin[0]) return(i);
  typenum = G__defined_typename(pbegin);
  if(-1==typenum) tagnum = G__defined_tagname(pbegin,1);
  if((-1!=typenum && -1!=G__newtype.parent_tagnum[typenum]) ||
     (-1!=tagnum  && -1!=G__struct.parent_tagnum[tagnum])) {
    strcpy(pbegin,G__type2string(0,tagnum,typenum,0,0));
    i = strlen(string);
  }
  return(i);
}

/***********************************************************************
* G__fdumpstream(string,endmark)
* char *string       : string until the endmark appears
* char *endmark      : specify endmark characters
*
*  This function is used only for reading pointer to function arguments.
*    type (*)(....)  type(*p2f)(....)
***********************************************************************/
int G__fdumpstream(string,endmark)
char *string,*endmark;
{
  short i=0,l;
  int c,prev;
  short nest=0,single_quote=0,double_quote=0,flag=0,ignoreflag;
  int commentflag=0;
  char *pbegin = string;
  int tmpltnest=0;
  
  do {
    ignoreflag=0;
    c=G__fgetc() ;
    
    if((nest==0)&&(single_quote==0)&&(double_quote==0)) {
      l=0;
      while((prev=endmark[l++])!='\0') {
	if(c==prev) {
	  flag=1;
	  ignoreflag=1;
	}
      }
    }
    
    switch(c) {
    case ' ':
    case '\t':
    case '\n':
    case '\r':
    case '\f':
      commentflag=0;
      if((single_quote==0)&&(double_quote==0)) {
	c=' ';
	if(i>0 && isspace(string[i-1])) {
	  ignoreflag=1;
	}
	else {
	  i = G__getfullpath(string,pbegin,i);
	}
	if(tmpltnest==0) pbegin = string+i+1-ignoreflag;
      }
      break;

    case '<':
      if((single_quote==0)&&(double_quote==0)) {
	string[i]=0;
        if(G__defined_templateclass(pbegin)) ++tmpltnest;
      }
      break;
    case '>':
      if((single_quote==0)&&(double_quote==0)) {
	if(tmpltnest) --tmpltnest;
      }
      break;
	 
    case '{':
    case '(':
    case '[':
      if((single_quote==0)&&(double_quote==0)) {
	nest++;
	pbegin = string+i+1;
      }
      break;
    case '}':
    case ')':
    case ']':
      if((single_quote==0)&&(double_quote==0)) {
	nest--;
	if(nest<0) {
	  flag=1;
	  ignoreflag=1;
	}
	i = G__getfullpath(string,pbegin,i);
	pbegin = string+i+1-ignoreflag;
      }
      break;
    case '"':
      if(single_quote==0) {
	double_quote ^= 1;
      }
      break;
    case '\'':
      if(double_quote==0) {
	single_quote ^= 1;
      }
      break;
      
    case '\\':
      if(ignoreflag==0) {
	string[i++] = c ;
	c=G__fgetc() ;
      }
      break;
      
      /*
	case '\0':
	flag=1;
	ignoreflag=1;
	break;
	*/

      
    case '/':
      if(0==double_quote && 0==single_quote && i>0 && string[i-1]=='/' &&
	 commentflag) {
	G__fignoreline();
	--i;
	ignoreflag=1;
      }
      else {
	commentflag=1;
      }
      break;
      
    case '*':
      /* comment */
      if(0==double_quote && 0==single_quote && i>0 && string[i-1]=='/' &&
	 commentflag) {
	G__skip_comment();
	--i;
	ignoreflag=1;
      }
      if(ignoreflag==0) i = G__getfullpath(string,pbegin,i);
      pbegin = string+i+1-ignoreflag;
      break;

    case '&':
    case ',':
      i = G__getfullpath(string,pbegin,i);
      pbegin = string+i+1;
      break;

    case '#':
      if(single_quote==0&&double_quote==0&&(i==0||string[i-1]!='$')) {
	G__pp_command();
	ignoreflag=1;
#ifdef G__TEMPLATECLASS
	c=' ';
#endif
      }
      break;

    case EOF:
      G__unexpectedEOF("G__fdumpstream():2");
      string[i] = '\0';
      return(c);

#ifdef G__MULTIBYTE
    default:
      if(G__IsDBCSLeadByte(c) && !ignoreflag) {
	string[i++]=c;
	c=G__fgetc() ;
	G__CheckDBCS2ndByte(c);
      }
      break;
#endif
    }
    
    if(ignoreflag==0) {
      string[i++] = c ;
      G__CHECK(G__SECURE_BUFFER_SIZE,i>=G__LONGLINE,return(EOF));
    }
    
  } while(flag==0) ;
  
  string[i]='\0';
  
  return(c);
}

/***********************************************************************
* G__fgetstream(string,endmark)
*
*
* char *string       : string until the endmark appears
* char *endmark      : specify endmark characters
*
*
*  read source file until specified endmark char appears.
*
* 1) read source file and store char to char *string.
*   If char is space char which is not surrounded by quoatation
*   it is not stored into char *string.
* 2) When one of endmark char appears or parenthesis nesting of
*   parenthesis gets negative , like '())' , stop reading and 
*   return the last char.
*
*  *endmark=";"
*     '  ab cd e f g;hijklm '
*      -------------^          *string="abcdefg"; return(';');
*
*  *endmark=";"
*     ' abc );'
*      -----^    *string="abc"; return(')');
*
***********************************************************************/
int G__fgetstream(string,endmark)
char *string,*endmark;
{
  short i=0,l;
  int c,prev;
  short nest=0,single_quote=0,double_quote=0,flag=0,ignoreflag;
  int commentflag=0;
  
  
  do {
    ignoreflag=0;
    c=G__fgetc() ;
    
    if((nest==0)&&(single_quote==0)&&(double_quote==0)) {
      l=0;
      while((prev=endmark[l++])!='\0') {
	if(c==prev) {
	  flag=1;
	  ignoreflag=1;
	}
      }
    }
    
    switch(c) {
    case '\f':
    case '\n':
    case '\r':
    case '\t':
    case ' ':
      commentflag=0;
      if((single_quote==0)&&(double_quote==0)) {
	ignoreflag=1;
      }
      break;
    case '{':
    case '(':
    case '[':
      if((single_quote==0)&&(double_quote==0)) {
	nest++;
      }
      break;
    case '}':
    case ')':
    case ']':
      if((single_quote==0)&&(double_quote==0)) {
	nest--;
	if(nest<0) {
	  flag=1;
	  ignoreflag=1;
	}
      }
      break;
    case '"':
      if(single_quote==0) {
	double_quote ^= 1;
      }
      break;
    case '\'':
      if(double_quote==0) {
	single_quote ^= 1;
      }
      break;
      
    case '\\':
      if(ignoreflag==0) {
	string[i++] = c ;
	c=G__fgetc() ;
      }
      break;
      
      /*
	case '\0':
	flag=1;
	ignoreflag=1;
	break;
	*/

      
    case '/':
      if(0==double_quote && 0==single_quote && i>0 && string[i-1]=='/' &&
	 commentflag) {
	G__fignoreline();
	--i;
	ignoreflag=1;
        if (strchr (endmark, '\n') != 0) {
          c = '\n';
          flag = 1;
        }
      }
      else {
	commentflag=1;
      }
      break;
      
    case '*':
      /* comment */
      if(0==double_quote && 0==single_quote && i>0 && string[i-1]=='/' &&
	 commentflag) {
	G__skip_comment();
	--i;
	ignoreflag=1;
      }
      break;

    case '#':
      if(single_quote==0&&double_quote==0&&
	 0==flag &&
	 (i==0||string[i-1]!='$')) {
	G__pp_command();
	ignoreflag=1;
#ifdef G__TEMPLATECLASS
	c=' ';
#endif
      }
      break;

    case EOF:
      G__unexpectedEOF("G__fgetstream():2");
      string[i] = '\0';
      return(c);

#ifdef G__MULTIBYTE
    default:
      if(G__IsDBCSLeadByte(c) && !ignoreflag) {
	string[i++]=c;
	c=G__fgetc() ;
	G__CheckDBCS2ndByte(c);
      }
      break;
#endif
    }
    
    if(ignoreflag==0) {
      string[i++] = c ;
      G__CHECK(G__SECURE_BUFFER_SIZE,i>=G__LONGLINE,return(EOF));
    }
    
  } while(flag==0) ;
  
  string[i]='\0';
  
  return(c);
}

/***********************************************************************
* G__fignorestream(endmark)
*
*
* char *endmark      : specify endmark characters
*
*  skip source file until specified endmark char appears.
* This function is identical to G__fgetstream() except it does not
* return char *string;
*
* 1) read source file.
* 2) When one of endmark char appears or parenthesis nesting of
*   parenthesis gets negative , like '())' , stop reading and 
*   return the last char.
*
*  *endmark=";"
*     '  ab cd e f g;hijklm '
*      -------------^           return(';');
*
*  *endmark=";"
*     ' abc );'
*      -----^                   return(')');
*
***********************************************************************/
int G__fignorestream(endmark)
char *endmark;
{
  short l;
  int c,prev;
  short nest=0,single_quote=0,double_quote=0,flag=0;
  
  do {
    c=G__fgetc() ;
    
    
    if((nest==0)&&(single_quote==0)&&(double_quote==0)) {
      l=0;
      while((prev=endmark[l++])!='\0') {
	if(c==prev) {
	  flag=1;
	}
      }
    }
    
    switch(c) {
    case '{':
    case '(':
    case '[':
      if((single_quote==0)&&(double_quote==0)) {
	nest++;
      }
      break;
    case '}':
    case ')':
    case ']':
      if((single_quote==0)&&(double_quote==0)) {
	nest--;
	if(nest<0) {
	  flag=1;
	}
      }
      break;
    case '"':
      if(single_quote==0) {
	double_quote ^= 1;
      }
      break;
    case '\'':
      if(double_quote==0) {
	single_quote ^= 1;
      }
      break;
      
    case '\\':
      if(flag==0) c=G__fgetc() ;
      break;
      
    case '/':
      if((single_quote==0)&&(double_quote==0)) {
	/* comment */
	
	c=G__fgetc();
	switch(c) {
	case '*':
	  G__skip_comment();
	  break;
	case '/':
	  G__fignoreline();
	  break;
	default:
	  fseek(G__ifile.fp,-1,SEEK_CUR);
	  if(c=='\n' /* || c=='\r' */) --G__ifile.line_number;
	  if(G__dispsource) G__disp_mask=1;
	  c='/';
	  /* flag=1; BUG BUG, WHY */
	  break;
	}
      }
      break;
      
    /* need to handle preprocessor statements */
      
    case EOF:
      G__unexpectedEOF("G__fignorestream():3");
      return(c);

#ifdef G__MULTIBYTE
    default:
      if(G__IsDBCSLeadByte(c)) {
	c=G__fgetc() ;
	G__CheckDBCS2ndByte(c);
      }
      break;
#endif
    }
    
  } while(flag==0) ;
  
  return(c);
}

/***********************************************************************
* G__ignorestream(source,isrc,endmark)
*
*
* char *endmark      : specify endmark characters
*
*  skip source file until specified endmark char appears.
* This function is identical to G__fgetstream() except it does not
* return char *string;
*
* 1) read source file.
* 2) When one of endmark char appears or parenthesis nesting of
*   parenthesis gets negative , like '())' , stop reading and 
*   return the last char.
*
*  *endmark=";"
*     '  ab cd e f g;hijklm '
*      -------------^           return(';');
*
*  *endmark=";"
*     ' abc );'
*      -----^                   return(')');
*
***********************************************************************/
int G__ignorestream(source,isrc,endmark)
char *source;
int* isrc;
char *endmark;
{
  short l;
  int c,prev;
  short nest=0,single_quote=0,double_quote=0,flag=0;
  
  
  do {
    c = source[(*isrc)++] ;
    
    
    if((nest==0)&&(single_quote==0)&&(double_quote==0)) {
      l=0;
      while((prev=endmark[l++])!='\0') {
	if(c==prev) {
	  flag=1;
	}
      }
    }
    
    switch(c) {
    case '{':
    case '(':
    case '[':
      if((single_quote==0)&&(double_quote==0)) {
	nest++;
      }
      break;
    case '}':
    case ')':
    case ']':
      if((single_quote==0)&&(double_quote==0)) {
	nest--;
	if(nest<0) {
	  flag=1;
	}
      }
      break;
    case '"':
      if(single_quote==0) {
	double_quote ^= 1;
      }
      break;
    case '\'':
      if(double_quote==0) {
	single_quote ^= 1;
      }
      break;
      
    case '\\':
      if(flag==0) c = source[(*isrc)++] ;
      break;
      
#ifdef G__NEVER
    case '/':
      if((single_quote==0)&&(double_quote==0)) {
	/* comment */
	
	c = source[(*isrc)++] ;
	switch(c) {
	case '*':
	  G__skip_comment();
	  break;
	case '/':
	  G__fignoreline();
	  break;
	default:
	  fseek(G__ifile.fp,-1,SEEK_CUR);
	  if(c=='\n' /* || c=='\r' */) --G__ifile.line_number;
	  if(G__dispsource) G__disp_mask=1;
	  c='/';
	  /* flag=1; BUG BUG, WHY */
	  break;
	}
      }
      break;
#endif
      
    /* need to handle preprocessor statements */
      
    case EOF:
      G__unexpectedEOF("G__fignorestream():3");
      return(c);

#ifdef G__MULTIBYTE
    default:
      if(G__IsDBCSLeadByte(c)) {
	c = source[(*isrc)++] ;
	G__CheckDBCS2ndByte(c);
      }
      break;
#endif
    }
    
  } while(flag==0) ;
  
  return(c);
}


/***********************************************************************
* G__fgetstream_new(string,endmark)
*
* Called by
*   G__exec_statement()
*   G__exec_statement()
*
* char *string       : string until the endmark appears
* char *endmark      : specify endmark characters
*
*  read source file until specified endmark char appears.
*
* 1) read source file and store char to char *string.
*   If char is space char which is not surrounded by quoatation
*   it is not stored into char *string.
* 2) When one of endmark char appears or parenthesis nesting of
*   parenthesis gets negative , like '())' , stop reading and 
*   return the last char.
*
*  *endmark=";"
*     '  ab cd e f g;hijklm '
*      -------------^          *string="abcdefg"; return(';');
*
*  *endmark=";"
*     ' abc );'
*      -----^    *string="abc"; return(')');
*
*     'abc=new xxx;'
*     'func(new xxx);'
*
***********************************************************************/
int G__fgetstream_new(string,endmark)
char *string,*endmark;
{
  short i=0,l;
  int c,prev;
  int nest=0,single_quote=0,double_quote=0,flag=0,ignoreflag;
  int commentflag=0;

  short inew=0;

  do {
    ignoreflag=0;
    c=G__fgetc() ;
    
    if((nest<=0)&&(single_quote==0)&&(double_quote==0)) {
      l=0;
      while((prev=endmark[l++])!='\0') {
	if(c==prev) {
	  flag=1;
	  ignoreflag=1;
	}
      }
    }
    
    switch(c) {
    case '\f':
    case '\n':
    case '\r':
    case '\t':
    case ' ':
      commentflag=0;
      if((single_quote==0)&&(double_quote==0)) {
	c=' ';
	switch(i-inew) {
	case 3:
	  if(strncmp(string+inew,"new",3)!=0)
	    ignoreflag=1;
	  break;
#ifndef G__PHLIPPE33
	case 5:
          /* keep the space after const */
	  if(strncmp(string+inew,"const",5)!=0)
	    ignoreflag=1;
	  break;
#endif
	default:
	  inew=i;
	  ignoreflag=1;
	  break;
	}
      }
      break;
    case ',':
    case '=':
      if((single_quote==0)&&(double_quote==0)) {
	inew=i+1;
      }
      break;
    case '{':
    case '(':
    case '[':
      if((single_quote==0)&&(double_quote==0)) {
	++nest;
	inew=i+1;
      }
      break;
    case '}':
    case ')':
    case ']':
      if((single_quote==0)&&(double_quote==0)) {
	nest--;
	inew=i+1;
      }
      break;
    case '"':
      if(single_quote==0) {
	double_quote ^= 1;
      }
      break;
    case '\'':
      if(double_quote==0) {
	single_quote ^= 1;
      }
      break;
      
    case '\\':
      if(ignoreflag==0) {
	string[i++] = c ;
	c=G__fgetc() ;
      }
      break;
      
    case '/':
      if(0==double_quote && 0==single_quote && i>0 && string[i-1]=='/' &&
	 commentflag) {
	i--;
	G__fignoreline();
	ignoreflag=1;
      }
      else {
	commentflag=1;
      }
      break;
      
    case '*':
      /* comment */
      if(0==double_quote && 0==single_quote && i>0 && string[i-1]=='/' &&
	 commentflag) {
	G__skip_comment();
	--i;
	ignoreflag=1;
      }
      break;

    case '#':
      if(single_quote==0&&double_quote==0&&(i==0||string[i-1]!='$')) {
	G__pp_command();
	ignoreflag=1;
#ifdef G__TEMPLATECLASS
	c=' ';
#endif
      }
      break;

    case EOF:
      G__unexpectedEOF("G__fgetstream_new():2");
      string[i] = '\0';
      return(c);

#ifdef G__MULTIBYTE
    default:
      if(G__IsDBCSLeadByte(c) && !ignoreflag) {
	string[i++]=c;
	c=G__fgetc() ;
	G__CheckDBCS2ndByte(c);
      }
      break;
#endif
    }
    
    if(ignoreflag==0) {
      string[i++] = c ;
      G__CHECK(G__SECURE_BUFFER_SIZE,i>=G__LONGLINE,return(EOF));
    }
    
  } while(flag==0) ;
  
  string[i]='\0';
  
  return(c);
}

/***********************************************************************
* G__fgetstream_spaces(string,endmark)
*
* char *string       : string until the endmark appears
* char *endmark      : specify endmark characters
*
*  read source file until specified endmark char appears.
*
*  Just like G__fgetstream(), except that spaces are not
*  completely removed.  Multiple spaces are, however, collapased
*  into a single space; leading and trailing spaces are also removed.
*
*  *endmark=";"
*     '  ab cd e f g;hijklm '
*      -------------^          *string="ab cd e f g"; return(';');
*
*  *endmark=";"
*     ' abc );'
*      -----^    *string="abc"; return(')');
*
***********************************************************************/
int G__fgetstream_spaces(string,endmark)
char *string,*endmark;
{
  short i=0,l;
  int c,prev;
  int nest=0,single_quote=0,double_quote=0,flag=0,ignoreflag;
  int commentflag=0;
  int last_was_space = 0;

  do {
    ignoreflag=0;
    c=G__fgetc() ;
    
    if((nest<=0)&&(single_quote==0)&&(double_quote==0)) {
      l=0;
      while((prev=endmark[l++])!='\0') {
	if(c==prev) {
	  flag=1;
	  ignoreflag=1;
	}
      }
    }
    
    switch(c) {
    case '\f':
    case '\n':
    case '\r':
    case '\t':
    case ' ':
      commentflag=0;
      if((single_quote==0)&&(double_quote==0)) {
	c=' ';
        if (last_was_space || i == 0)
          ignoreflag=1;
      }
      break;
    case '{':
    case '(':
    case '[':
      if((single_quote==0)&&(double_quote==0)) {
	++nest;
      }
      break;
    case '}':
    case ')':
    case ']':
      if((single_quote==0)&&(double_quote==0)) {
	nest--;
      }
      break;
    case '"':
      if(single_quote==0) {
	double_quote ^= 1;
      }
      break;
    case '\'':
      if(double_quote==0) {
	single_quote ^= 1;
      }
      break;
      
    case '\\':
      if(ignoreflag==0) {
	string[i++] = c ;
	c=G__fgetc() ;
      }
      break;
      
    case '/':
      if(0==double_quote && 0==single_quote && i>0 && string[i-1]=='/' &&
	 commentflag) {
	i--;
	G__fignoreline();
	ignoreflag=1;
      }
      else {
	commentflag=1;
      }
      break;
      
    case '*':
      /* comment */
      if(0==double_quote && 0==single_quote && i>0 && string[i-1]=='/' &&
	 commentflag) {
	G__skip_comment();
	--i;
	ignoreflag=1;
      }
      break;

    case '#':
      if(single_quote==0&&double_quote==0&&(i==0||string[i-1]!='$')) {
	G__pp_command();
	ignoreflag=1;
#ifdef G__TEMPLATECLASS
	c=' ';
#endif
      }
      break;

    case EOF:
      G__unexpectedEOF("G__fgetstream_new():2");
      string[i] = '\0';
      return(c);

#ifdef G__MULTIBYTE
    default:
      if(G__IsDBCSLeadByte(c) && !ignoreflag) {
	string[i++]=c;
	c=G__fgetc() ;
	G__CheckDBCS2ndByte(c);
      }
      break;
#endif
    }
    
    if(ignoreflag==0) {
      string[i++] = c ;
      G__CHECK(G__SECURE_BUFFER_SIZE,i>=G__LONGLINE,return(EOF));
    }

    last_was_space = (c == ' ');
    
  } while(flag==0) ;

  while (i > 0 && string[i-1] == ' ')
    --i;
  
  string[i]='\0';
  
  return(c);
}

/***********************************************************************
* G__getstream(source,isrc,string,endmark)
*
* Called by
*   G__getexpr()    '?:'
*   G__getexpr()    '?:'
*   G__getexpr()    '?:'
*
* char *source;      : source string. If NULL, read from input file
* int *isrc;         : char position of the *source if source!=NULL
* char *string       : substring until the endmark appears
* char *endmark      : specify endmark characters
*
*
*  Get substring of char *source; until one of endmark char is found.
* Return string is not used.
*  Only used in G__getexpr() to handle 'cond?true:faulse' opeartor.
*
*   char *endmark=";";
*   char *source="abcdefg * hijklmn ;   "
*                 ------------------^      *string="abcdefg*hijklmn"
*
*   char *source="abcdefg * hijklmn) ;   "
*                 -----------------^       *string="abcdefg*hijklmn"
*
***********************************************************************/
int G__getstream(source,isrc,string,endmark)
char *source;
int *isrc;
char *string,*endmark;
{
  short i=0,l;
  int c,prev;
  short nest=0,single_quote=0,double_quote=0,flag=0,ignoreflag;
  
  
  do {
    ignoreflag=0;
    c = source[(*isrc)++] ;
    
    
    if((nest==0)&&(single_quote==0)&&(double_quote==0)) {
      l=0;
      while((prev=endmark[l++])!='\0') {
	if(c==prev) {
	  flag=1;
	  ignoreflag=1;
	}
      }
    }
    
    switch(c) {
    case '"':
      if(single_quote==0) {
	double_quote ^= 1;
      }
      break;
    case '\'':
      if(double_quote==0) {
	single_quote ^= 1;
      }
      break;
    case '{':
    case '(':
    case '[':
      if((single_quote==0)&&(double_quote==0)) {
	nest++;
      }
      break;
    case '}':
    case ')':
    case ']':
      if((single_quote==0)&&(double_quote==0)) {
	nest--;
	if(nest<0) {
	  flag=1;
	  ignoreflag=1;
	}
      }
      break;
    case '\f':
    case '\n':
    case '\r':
    case '\t':
    case ' ':
      if((single_quote==0)&&(double_quote==0)) {
	ignoreflag=1;
      }
      break;
    case '\0':
      /* if((single_quote==0)&&(double_quote==0)) { */
      flag=1;
      ignoreflag=1;
      /* } */
      break;
    case EOF:
      G__unexpectedEOF("G__getstream()");
      string[i] = '\0';
      break;

#ifdef G__MULTIBYTE
    default:
      if(G__IsDBCSLeadByte(c) && !ignoreflag) {
	string[i++]=c;
	c=G__fgetc() ;
	G__CheckDBCS2ndByte(c);
      }
      break;
#endif
    }
    
    if(ignoreflag==0) {
      string[i++] = c ;
      G__CHECK(G__SECURE_BUFFER_SIZE,i>=G__LONGLINE,{string[i]='\0';return(EOF);});
    }
    
  } while(flag==0) ;
  
  string[i]='\0';
  
  return(c);
}



/***********************************************************************
* G__fignoreline()
*
* Called by
*    G__exec_if()         skip C++ style comment
*    G__exec_else_if()
*    G__exec_statement()
*    G__fgetspace()
*    G__fgetname()
*    G__fgetstream()
*    G__fignorestream()
*
*  Read and ignore line
*
*    'as aljaf alijflijaf lisflif\n'
*     ----------------------------^
*
***********************************************************************/
void G__fignoreline()
{
  int c;
  while((c=G__fgetc())!='\n' && c!='\r' && c!=EOF) {
#ifdef G__MULTIBYTE
    if(G__IsDBCSLeadByte(c)) {
      c=G__fgetc();
      G__CheckDBCS2ndByte(c);
    }
    else if(c=='\\') {
      c=G__fgetc();
      if('\r'==c||'\n'==c) c=G__fgetc();
    }
#else /* MULTIBYTE */
    if(c=='\\') {
      c=G__fgetc();
      if('\r'==c||'\n'==c) c=G__fgetc();
    }
#endif /* MULTIBYTE */
  }
}

/***********************************************************************
* G__fgetline()
*
*    'as aljaf alijflijaf lisflif\n'
*     ----------------------------^
***********************************************************************/
int G__fgetline(string)
char *string;
{
  int c;
  int i=0;
  while((c=G__fgetc())!='\n' && c!='\r' && c!=EOF) {
    string[i]=c;
    if(c=='\\') {
      c=G__fgetc();
      if('\r'==c||'\n'==c) c=G__fgetc();
      string[i]=c;
    }
    ++i;
  }
  string[i]='\0';
  return(c);
}

/***********************************************************************
* G__fsetcomment()
*
*
*   xxxxx;            // comment      \n
*         ^ ------------V-------------->
*
***********************************************************************/
void G__fsetcomment(pcomment)
struct G__comment_info *pcomment;
{
  int c;
  fpos_t pos;

  if(pcomment->filenum>=0 || pcomment->p.com) return;

  fgetpos(G__ifile.fp,&pos);

  while((isspace(c=fgetc(G__ifile.fp)) || ';'==c) && '\n'!=c && '\r'!=c) ;
  if('/'==c) {
    c=fgetc(G__ifile.fp);
    if('/'==c || '*'==c) {
      while(isspace(c=fgetc(G__ifile.fp))) {
	if('\n'==c || '\r'==c) {
	  fsetpos(G__ifile.fp,&pos);
	  return;
	}
      }
      if(G__ifile.fp==G__mfp) pcomment->filenum = G__MAXFILE;
      else                    pcomment->filenum = G__ifile.filenum;
      fseek(G__ifile.fp,-1,SEEK_CUR);
      fgetpos(G__ifile.fp,&pcomment->p.pos);
    }
  }
  fsetpos(G__ifile.fp,&pos);
  return;
}

/***********************************************************************
* G__eolcallback()
***********************************************************************/

void G__set_eolcallback(eolcallback)
void* eolcallback;
{
  G__eolcallback = (G__eolcallback_t)eolcallback;
}

/***********************************************************************
* G__fgetc()
*
*
*  Read one char from source file.
*  Count line number
*  Set G__break=1 when line number comes to break point
*  Display source file if G__dispsource==1
***********************************************************************/

int G__fgetc()
/* struct G__input_file *fin; */
{
  int c;

 try_again:

  c=fgetc(G__ifile.fp);
  
  switch(c) {
  case '\n':
  /* case '\r': */
    ++G__ifile.line_number;
    if(0==G__nobreak && 
       0==G__disp_mask&& 
       G__srcfile[G__ifile.filenum].breakpoint &&
       G__srcfile[G__ifile.filenum].maxline>G__ifile.line_number &&
       G__TESTBREAK&(G__srcfile[G__ifile.filenum].breakpoint[G__ifile.line_number]|=(!G__no_exec))
       && !G__cintv6
       ) {
      G__BREAKfgetc();
    }
    G__eof_count=0;
    if(G__dispsource) G__DISPNfgetc();
    if(G__eolcallback) (*G__eolcallback)(G__ifile.name,G__ifile.line_number);
    break;
  case EOF:
    G__EOFfgetc();
    break;
  case '\0':
    if(G__maybe_finish_macro()) goto try_again;
    /* otherwise, fail through to the default case */
  default:
    if(G__dispsource) G__DISPfgetc(c);
    break;
  }
  
  return( c ) ;
}

#ifndef G__OLDIMPLEMENTATION1649
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
