/*****************************************************************************
* ReadF.h
*
* Description: Parser library associated with open source C++ interpreter CINT
* Author : Masaharu Goto
*****************************************************************************/
/************************************************************************
 * Copyright(c) 1995~2015  Masaharu Goto (cint@pcroot.cern.ch) (gotom@hanno.jp)
 *
 * Permission to use, copy, modify and distribute this software and its 
 * documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation. The author makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 ************************************************************************/

#if defined(__CINT__) && !defined(__MAKECINT__)
#pragma include_noerr <ReadF.dll>
#endif


#ifndef READFILE_H
#define READFILE_H

#include "MultiByteChar.h"
#ifndef E__MULTIBYTELIB
#define E__MULTIBYTE
#endif

#define ESCAPEBYQUOTATION

#define G__READFILESL

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <string>
#include <vector>
using namespace std;

#if defined(__CINT__) && !defined(G__P2F)
typedef  int  bool;
#endif

#define ReadF_posSIZE 10

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
class ReadF 
#ifdef E__MULTIBYTELIB
: public G__MultiByteMode
#endif
{
 public:
  int argc;               // number of arguments in one line
  vector<char*> argv;     // argument buffer
  vector<char*> sepv;      //
  //vector<const char*> argv;     // argument buffer
  //vector<const char*> sepv;      //
  int line;               // line number
  string fname;

  ReadF() { initialize(); fp=0; }
  ReadF(const char *filename) {init(filename);}
  ReadF(const string& filename) {init(filename.c_str());}
  ReadF(FILE *fpin);
  ~ReadF();

  int open(const char *filename);
  int open(const string& filename) {return(open(filename.c_str()));}
  int close();

  void parse(const char* s) {
    parsebuf = s;
    iparse = 0;
    read();
    parsebuf = (const char*)NULL;
  }
  void parse(const string& s) {parse(s.c_str());}

  vector<string> separatetoken(const string& str) {
    parse(str);
    vector<string> vs;
    for(int i=1;i<=argc;i++) vs.push_back(argv[i]);
    return(vs);
  }


  int read();
  int readword();
  int readword(string& sp,string& buf);
  //int regex(char *pattern,char *string=(char*)NULL);
  void setseparator(const char *separatorin); 
  void setdelimiter(const char *delimiterin); 
  void setendofline(const char *endoflinein); 
  void setquotation(const char *quotationin); 
  void setseparator(const string& separatorin) {setseparator(separatorin.c_str()); }
  void setdelimiter(const string& delimiterin) {setdelimiter(delimiterin.c_str()); }
  void setendofline(const string& endoflinein) {setendofline(endofline.c_str()); }
  void setquotation(const string& quotationin) {setquotation(quotationin.c_str()); }
  void setkeepquotation(bool flag) { keepquotation=flag; }
//  bool keepquotation() const { return(keepquotation); }

  void setescapebyquotation(bool flag) { escapebyquotation=flag; }

  // getpos and rewind method have problem. Do not use those.
  int getpos(int i=0) { 
    if(fp && i<ReadF_posSIZE) {
      fgetpos(fp,&pos[i]);  
      posline[i] = line;
      return(1); 
    }
    else return(0);
  }
  // getpos and rewind method have problem. Do not use those.
  int rewind(int i=0) { 
    if(fp && i<ReadF_posSIZE) {
      fsetpos(fp,&pos[i]);  
      line = posline[i];
      return(1); 
    }
    else return(0);
  }
#ifndef __MAKECINT__
  int setpos(fpos_t posin,int linein) { if(fp) {fsetpos(fp,&posin); line=linein; } return(fp?1:0); }
#endif


  //int isvalid() { if(fp) return(1); else return(0); }
  int isvalid() { if(fp&&line>=0) return(1); else return(0); }
  void disp();

  FILE *FP() { return(fp); }

 //private:
  FILE *fp;
 private:
  int openflag;

  string wholeline;
  vector<string> arg;
  vector<string> sep;
  const char *parsebuf;
  int   iparse;

  string separator;
  string delimiter;
  string endofline;
  string quotation;


  void init(const char* filename);
  void initialize();
  int isseparator(int c) {return(separator.find(c)!=string::npos?1:0);}
  int isdelimiter(int c) {return(delimiter.find(c)!=string::npos?1:0);}
  int isendofline(int c) {return(endofline.find(c)!=string::npos?1:0);}
  int isquotation(int c) {return(quotation.find(c)!=string::npos?1:0);}
  fpos_t pos[ReadF_posSIZE];
  int posline[ReadF_posSIZE];
  bool keepquotation;
  bool escapebyquotation;
  //char* parsequotation(char *p,FILE* fp);

  //int noeofcat;
 public:
  //void setnoeofcat(int in) { noeofcat = in; }

#if 1
  ////////////////////////////////////////////////////////////
  int readuntil(char un,char* buf) {
    int c;
    int i=0;
    int quote=0;
    do {
      //c= fgetc(fp);
      c = parsebuf?parsebuf[iparse++]:fgetc(fp);
      buf[i++]=c;
      if(c=='"') quote^=1; 
      if(IsDBCSLeadByte(c)) {
        c = parsebuf?parsebuf[iparse++]:fgetc(fp);
        buf[i++]=c;
        CheckDBCS2ndByte(c);
      }
    } while((c!=un && !isendofline(c) && c!=EOF) || quote==1);
    //} while((c!=un && c!='\n' && c!=EOF) || quote==1);
    buf[i-1] = 0;
    if(isendofline(c)) ++line;
    return(c);
  }
  ////////////////////////////////////////////////////////////
  int skipuntil(char un) {
    int c;
    int quote=0;
    do {
      //c= fgetc(fp);
      c = parsebuf?parsebuf[iparse++]:fgetc(fp);
      if(c=='"') quote^=1; 
      if(IsDBCSLeadByte(c)) {
        c = parsebuf?parsebuf[iparse++]:fgetc(fp);
        CheckDBCS2ndByte(c);
      }
    } while((c!=un && !isendofline(c) && c!=EOF) || quote==1);
    //} while((c!=un && c!='\n' && c!=EOF) || quote==1);
    if(isendofline(c)) ++line;
    return(c);
  }
  ////////////////////////////////////////////////////////////
  int skipline() {
    int c;
    do {
      //c= fgetc(fp);
      c = parsebuf?parsebuf[iparse++]:fgetc(fp);
      if(IsDBCSLeadByte(c)) {
        c = parsebuf?parsebuf[iparse++]:fgetc(fp);
        CheckDBCS2ndByte(c);
      }
    } while(!isendofline(c) && c!=EOF);
    ++line;
    return(c);
  }
  void goback() { fseek(fp,0,SEEK_SET); line=0; }
  int fseek_cur(int offset) { return(fseek(fp,offset,SEEK_CUR)); }
  int fseek_set(int offset) { return(fseek(fp,offset,SEEK_SET)); }
  int fseek_end(int offset) { return(fseek(fp,offset,SEEK_END)); }
#endif
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
inline vector<string> separate_token(const string& str,const string& separator){
  ReadF p; p.setseparator(separator); p.parse(str);
  vector<string> vs;
  for(int i=1;i<=p.argc;i++) vs.push_back(p.argv[i]);
  return(vs);
}

#endif


#ifndef EXTRACT_PATH
#define EXTRACT_PATH
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
inline string extract_basename(const string& orig) {
  size_t pos = orig.find_last_of('.');
  if(pos!=string::npos) return(orig.substr(0,pos));
  else                  return(orig);
}

//////////////////////////////////////////////////////////////////////////
inline string extract_suffix(const string& orig) {
  size_t pos = orig.find_last_of('.');
  if(pos!=string::npos) return(orig.substr(pos));
  else                  return("");
}

//////////////////////////////////////////////////////////////////////////
inline string extract_dirname(const string& orig) {
  size_t pos = orig.find_last_of('/');
  if(pos!=string::npos) return(orig.substr(0,pos));
  pos = orig.find_last_of('\\');
  if(pos!=string::npos) return(orig.substr(0,pos));
  return("");
}

//////////////////////////////////////////////////////////////////////////
inline string extract_filename(const string& orig) {
  size_t pos = orig.find_last_of('/');
  if(pos!=string::npos) return(orig.substr(pos+1));
  pos = orig.find_last_of('\\');
  if(pos!=string::npos) return(orig.substr(pos+1));
  return(orig);
}

#endif // EXTRACT_PATH


