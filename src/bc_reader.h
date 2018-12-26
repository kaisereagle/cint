/* /% C %/ */
/***********************************************************************
 * cint (C/C++ interpreter)
 ************************************************************************
 * Header file bc_reader.h
 ************************************************************************
 * Description:
 *  source stream reader
 *    possibly with preprocessor macro resolution in future implementation
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
#ifndef BC_READER_H
#define BC_READER_H

extern "C" {
#ifdef __CINT__
#include "../G__ci.h"
#else
#include "common.h"
#endif
}

#include <string>
using namespace std;
extern const string G__endmark;

/***********************************************************************
 * G__fstream
 ***********************************************************************/
class G__fstream {
  FILE  *m_fp;
  fpos_t m_pos;
  int    m_linenum;
  int    m_c;
 public:
  void Init(G__input_file& ifile);
  void Init(const char *) { }
  int fgetc() { return(G__fgetc()); } // legacy
  void putback(int goback= -1) { fseek(G__ifile.fp,goback,SEEK_CUR); }
  void setpos(fpos_t& pos) { m_pos=pos; rewindpos(); }
  void storepos(int c=0);
  int rewindpos() ;
};

/***********************************************************************
 * G__sstream
 ***********************************************************************/
class G__sstream {
  char  *m_str;
  unsigned long m_pos;
  unsigned long m_store_pos;
  int    m_linenum;
  int    m_c;
 public:
  void Init(G__input_file& /* ifile */) { }
  void Init(const char *istring) { m_str=(char*)istring; m_pos=0; }
  int fgetc() { return(m_str[m_pos++]); }
  void putback(int goback= -1) { m_pos+=goback; }
  void setpos(fpos_t& /* pos */) { }
  void storepos(int c=0) { m_store_pos=m_pos; m_c=c; }
  int rewindpos() { m_pos=m_store_pos; return(m_c); }
};

/***********************************************************************
 * G__virtualreader
 ***********************************************************************/
class G__virtualreader {
 public:
  //G__srcreader() { Init(); }
  virtual ~G__virtualreader() { }
  virtual void Init(G__input_file& /* ifile */)  { }
  virtual void Init(const char* /* s */) { }

  virtual void setpos(fpos_t /* pos */) { }

  virtual int fgetc() =0 ;
  virtual int fgetc_gettoken() =0;

  virtual int fgettoken(string& token,const string& endmark=G__endmark) =0;
  virtual int fskipcomment(int c='/') =0;
  virtual int fpp_command(int c=0) =0;
  virtual int fgetquotation(string& token,int c,int openquotation=0) =0;

  virtual int fgetspace() =0;

  virtual int fappendtoken(string& token,int c,const string& endmark=G__endmark)=0;

  virtual int fgetstream(string& phrase,const string& endmark,int nest=0)=0;
  virtual int fgetstream_template(string& phrase,const string& endmark,int nest=0) =0;

  virtual int fignorestream(const string& endmark,int nest=0) =0;
  virtual int fignorestream_template(const string& endmark,int nest=0) =0;

  virtual int fgetstream_(string& phrase,const string& endmark,int nest=0)=0;
  virtual int fgetstream_template_(string& phrase,const string& endmark,int nest=0) =0;

  virtual int fignorestream_(const string& endmark,int nest=0) =0;
  virtual int fignorestream_template_(const string& endmark,int nest=0) =0;

  virtual void fignoreline() =0;
  virtual int skipCcomment() =0;

  virtual void putback() =0;

  virtual void storepos(int c=0)=0;
  virtual int rewindpos()=0;

};


/***********************************************************************
 * G__srcreader
 ***********************************************************************/
template<class T>   // T is either G__fstream or G__sstream
class G__srcreader : public G__virtualreader {
  T m_stream;
 public:
  //G__srcreader() { Init(); }
  void Init(G__input_file& ifile) { m_stream.Init(ifile); }
  void Init(const char* s) { m_stream.Init(s); }

  void setpos(fpos_t pos) { m_stream.setpos(pos); }

  int fgetc() { return(m_stream.fgetc()); } 
  int fgetc_gettoken() { return(fskipcomment(fgetc())); }
  int fgetc_separator();

  int fgettoken(string& token,const string& endmark=G__endmark);
  int fskipcomment(int c='/');
  int fpp_command(int c=0);
  int fgetquotation(string& token,int c,int openquotation=0);

  int fgetspace() ;

  int fappendtoken(string& token,int c,const string& endmark=G__endmark) ;

  int fgetstream(string& phrase,const string& endmark,int nest=0) 
   {return(fgetstream_core(phrase,endmark,nest,0));}
  int fgetstream_template(string& phrase,const string& endmark,int nest=0) 
   {return(fgetstream_core(phrase,endmark,nest,1));}

  int fignorestream(const string& endmark,int nest=0) 
   {string phrase; return(fgetstream_core(phrase,endmark,nest,0));}
  int fignorestream_template(const string& endmark,int nest=0) 
   {string phrase; return(fgetstream_core(phrase,endmark,nest,1));}

  int fgetstream_(string& phrase,const string& endmark,int nest=0) 
   {return(fgetstream_core(phrase,endmark,nest,0,1));}
  int fgetstream_template_(string& phrase,const string& endmark,int nest=0) 
   {return(fgetstream_core(phrase,endmark,nest,1,1));}

  int fignorestream_(const string& endmark,int nest=0) 
   {string phrase; return(fgetstream_core(phrase,endmark,nest,0,1));}
  int fignorestream_template_(const string& endmark,int nest=0) 
   {string phrase; return(fgetstream_core(phrase,endmark,nest,1,1));}

  void fignoreline() { G__fignoreline(); }          // legacy
  int skipCcomment() { return(G__skip_comment()); } // legacy

  void putback() { m_stream.putback(); } // legacy

  void storepos(int c=0) { m_stream.storepos(c); }
  int rewindpos() { return(m_stream.rewindpos()); }

 private:
  int fgetstream_core(string& phrase,const string& endmark
		      ,int nest,int istemplate,int exitAtBlacket=0) ;
};

/***********************************************************************
 * G__srcreader

***********************************************************************/

//////////////////////////////////////////////////////////////////////
template<class T>
int G__srcreader<T>::fgettoken(string& token,const string& endmark) {
  // word1   word2 + tmplt<tmplt<type>     >
  // ^ ----> ^^---> ^ ---> ^ --> ^ -> ^ ->^^
  // word1' ' word2+ tmplt< tmplt< type> ""' ' "">

  int c;
  //token.clear();
  token="";

  // if preprocessor macro is handled, I guess it has to be done here.
  //  if stat is set, token/c pair has to be returned from macro

#define G__OLDIMPLEMENTATION2064
 l_begin:

  c = fgetc_gettoken();
  if(c==EOF) {
    G__genericerror("Error: Unexpected end of file (1)");
    return(c);
  }
  if(c==0) return(c);
  if(isspace(c)) while(isspace(c)) c=fgetc_gettoken();
  if(c=='#') {
    fpp_command(c);
    goto l_begin;
  }

  for(;;) {
    if(c=='-' && token.size()>1 
       && (isdigit(token[0])||'.'==token[0])
       && tolower(token[token.size()-1])=='e') {
    }
    else if(c=='"' || c=='\'') {
      c = fgetquotation(token,c,1);
    }
    else if(string::npos!=endmark.find(c)) break;
    if(c==EOF) {
      G__genericerror("Error: Unexpected end of file (2)");
      break;
    }
    if(c) token.append((string::size_type)1,(char)c);
    else  return(c);
    c = fgetc_gettoken();
    if(c=='#') c=fpp_command(c);
  }

  // if preprocessor macro is handled, I guess it has to be done here.
  //  look up macro table, if there is a hit, set stat and return the first
  //  token/c pair.

  if(isspace(c)) {
    while(isspace(c)) c=fgetc_gettoken();
    if(c==0) return(c);
    if(string::npos==endmark.find(c)) {
      c=' ';
      putback();
    }
  }
  return(c);
}

//////////////////////////////////////////////////////////////////////
template<class T>
int G__srcreader<T>::fgetc_separator() {
  // word1   word2 + tmplt<tmplt<type>     >
  //      --.^    -^                  -----^

  int c;
  c = fgetc_gettoken();
  if(isspace(c)) {
    while(isspace(c)) c=fgetc_gettoken();
    if(c==0) return(c);
    if(string::npos==G__endmark.find(c)) {
      c=' ';
      putback();
    }
  }
  return(c);
}

//////////////////////////////////////////////////////////////////////
// G__srcreader::fpp_command(int c)
//
//  # if      COND
//  # ifdef   MACRO
//  # ifndef  MACRO
//  # elif    COND
//  # else
//  # endif
//  # <line> <file>
//   ^ -----> ^
// TODO
//  # define MACRO
//  # pragma
//  # error
//  # <line> <file>
//////////////////////////////////////////////////////////////////////
template<class T>
int G__srcreader<T>::fpp_command(int c) {
  char buf[G__ONELINE];
  c=G__fgetname(buf,"\n\r"); // legacy
  string condition=buf;
  if(condition.size() && isdigit(condition.c_str()[0])) {
    if('\n'!=c && '\r'!=c) fignoreline();
    G__ifile.line_number=atoi(condition.c_str());
  }
  else if(condition=="else"||
	  condition=="elif")   G__pp_skip(1);  // legacy
  else if(condition=="if")     G__pp_if();     // legacy
  else if(condition=="ifdef")  G__pp_ifdef(1); // legacy
  else if(condition=="ifndef") G__pp_ifdef(0); // legacy
  else if('\n'!=c && '\r'!=c)  fignoreline();
  c=' ';
  return(c);
}

//////////////////////////////////////////////////////////////////////
template<class T>
int G__srcreader<T>::fskipcomment(int c) {
  //    //
  //    /*    */
  //     ^
  if(c=='#') c=fpp_command(c);
  if(c!='/') return(c);
  c=fgetc();
  if(c=='/') fignoreline();
  else if(c=='*') skipCcomment();
  else if(c==0) return(c);
  else if(c==EOF) return(c);
  else {
    c='/';
    putback();
    return(c);
  }
  c=' ';
  return(c);
}

//////////////////////////////////////////////////////////////////////
template<class T>
int G__srcreader<T>::fgetquotation(string& token,int c,int /* openquotation */) {
  int quote=c;
  if(c) token.append((string::size_type)1,(char)c);
  for(;;) {
    c = fgetc();
    if(c==EOF) {
      G__genericerror("Error: Unexpected end of file (3)");
      break;
    }
    if(c==0) break;
    if(c==quote) break;
    if(c=='\\') {
      token.append((string::size_type)1,(char)c);
      c = fgetc();
    }
    else if(G__IsDBCSLeadByte(c)) {
      token.append((string::size_type)1,(char)c);
      c=fgetc();
      G__CheckDBCS2ndByte(c);
    }
    if(c==EOF) {
      G__genericerror("Error: Unexpected end of file (4)");
      break;
    }
    if(c==0) break;
    token.append((string::size_type)1,(char)c);
  }
  return(c);
}

//////////////////////////////////////////////////////////////////////
template<class T>
int G__srcreader<T>::fappendtoken(string& token,int c,const string& endmark) {
  string addition;
  if(c) token.append((string::size_type)1,(char)c);
  c = fgettoken(addition,endmark);
  token.append(addition);
  return(c);
}

//////////////////////////////////////////////////////////////////////
template<class T>
int G__srcreader<T>::fgetstream_core(string& phrase,const string& endmark
				,int nest,int istemplate,int exitAtBlacket) {
  int c=0;
  //phrase.clear();
  phrase="";
  string token;
  for(;;) {
    c = fgettoken(token);
    phrase.append(token);
    if( nest<=0 && string::npos!=endmark.find(c)) goto l_end;
    switch(c) {
    case '<':
      if(!istemplate) break;
    case '[':
    case '{':
    case '(':
      ++nest;
      break;
    case '>':
      if(!istemplate) break;
      if(phrase.size() && phrase[phrase.size()-1]=='>') 
	phrase.append((string::size_type)1,' ');
    case ']':
    case '}':
    case ')':
      --nest;
#define G__OLDIMPLEMENTATION2240
#ifndef G__OLDIMPLEMENTATION2241
      if(exitAtBlacket && nest<0) goto l_end;
#else
      if(nest<0) break;
#endif
      break;
    case 0:
      goto l_end;
    }
    phrase.append((string::size_type)1,(char)c);
  }
 l_end:
  return(c);
}

//////////////////////////////////////////////////////////////////////
template<class T>
int G__srcreader<T>::fgetspace() {
  return(G__fgetspace());
}

//////////////////////////////////////////////////////////////////////


#endif
