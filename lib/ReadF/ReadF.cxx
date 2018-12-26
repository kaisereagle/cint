/*****************************************************************************
* ReadF.C
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

#ifdef __CINT__
#pragma security level0
#endif

#include "ReadF.h"

#define CRLFINLINE
#define CATQUOTE
//#define DOUBLEQUOTEESCAPE

#ifdef E__MULTIBYTE
#define E__UNKNOWNCODING 0
#define E__EUC           1
#define E__SJIS          2
#define E__JIS           3
#define E__ONEBYTE       4 
#define E__UNICODE       5 
#endif

static void debugdisp(const string& name,const string& s) {
  printf("%s '%s' %2lu: ",name.c_str(),s.c_str(),s.size());
  for(unsigned int i=0;i<s.size();i++) {
    printf(" %d",s[i]);
    if(isprint(s[i])) printf("'%c' ",s[i]);
    else printf(" ");
  }
  printf("\n");
}

static void copystr(int n,vector<char*>& argv,const vector<string>& arg) {
  if(n==0) argv.clear();
  else     argv.resize(n);
  for(unsigned int i=0;i<arg.size();i++) argv.push_back((char*)arg[i].c_str());
}

/*****************************************************************************
* Constructors and destructors
*****************************************************************************/
void ReadF::init(const char* filename) {
  //fp = fopen(filename,"rb");
  fp = fopen(filename,"r");
  initialize();
  if(fp) {
    openflag=1;
    fname = filename;
  }
  else {
    fprintf(stderr,"ReadF: cannot open %s\n",filename);
  }
}

ReadF::ReadF(FILE *fpin)
{
  fp = fpin;
  initialize();
  openflag=0;
  if((FILE*)NULL==fpin) {
    openflag=0;
    fprintf(stderr,"ReadF: NULL pointer is given\n");
  }
}

ReadF::~ReadF()
{
  if(openflag && fp) fclose(fp);
  fp = (FILE*)NULL;
}

/*****************************************************************************
* file open/close
*****************************************************************************/
int ReadF::open(const char* filename) {
  close();
  fp = fopen(filename,"r");
  if(fp) {
    openflag=1;
    line = 0;
  }
  else fprintf(stderr,"ReadF: can not open %s\n",filename);
  fname = filename;
  return(openflag);
}

int ReadF::close() {
  if(openflag && fp) fclose(fp);
  fp=(FILE*)NULL;
  openflag=0;
  line = -1;
  return(1);
}


/*****************************************************************************
* set routines
*****************************************************************************/
void ReadF::initialize() {
  line = 0;
  argc = 0;
  setseparator(" \t\v\r\f");
  setendofline("\n\r");
  setdelimiter("");
  setquotation("");
  keepquotation = false;
  escapebyquotation = false;

  G__MultiByteMode::init();
  //noeofcat = 0;

  parsebuf = (char*)NULL;
}

void ReadF::setseparator(const char *separatorin) {
  separator= separatorin; 
}

void ReadF::setdelimiter(const char *delimiterin) {
  delimiter = delimiterin; 
}

void ReadF::setendofline(const char *endoflinein) {
  endofline = endoflinein; 
}

void ReadF::setquotation(const char *quotationin) {
  quotation = quotationin; 
}

/*****************************************************************************
* Reading one line
*****************************************************************************/
int ReadF::read() {
  if((!fp && !parsebuf) || line<0) return(0);

  argv.clear();
  arg.clear();
  sepv.clear();
  sep.clear();
  wholeline = "";
  argv.push_back((char*)wholeline.c_str());
  int c;
  string buf,sp;
  while(1) {
    c=readword(sp,buf);
    if((!isendofline(c) && c!=EOF) ||buf.size()) {
      //wholeline += buf;
      arg.push_back((char*)buf.c_str()); 
      argv.push_back((char*)arg.back().c_str());
    }
    //if(c && c!=EOF && !isendofline(c)) wholeline += c;
    argv[0] = (char*)wholeline.c_str();
    argc = arg.size();
    if(isendofline(c) || !c) {
      if(sp.size()) 
	{sep.push_back(sp); sepv.push_back((char*)sep.back().c_str()); sp="";}
      ++line;
      copystr(1,argv,arg); copystr(0,sepv,sep);
      return(1);
    }
    if(EOF==c) {
      if(sp.size()) 
	{sep.push_back(sp); sepv.push_back((char*)sep.back().c_str()); }
      line = -1;
      copystr(1,argv,arg); copystr(0,sepv,sep);
      if(argc) return(1);
      return(0);
    }
  } 

  copystr(1,argv,arg); copystr(0,sepv,sep);
  return(0); // never reach here
}

/*****************************************************************************
* Reading one word
*****************************************************************************/
int ReadF::readword(string& sp,string& buf) {
  int c;
  int isquote = 0;
  buf ="";

  if((!fp && !parsebuf) || line<0) return(0);

  // read until the first non-delimiter, non-separator char
  while(1) { 
    c = parsebuf?parsebuf[iparse++]:fgetc(fp);
    wholeline += c;
    if(isdelimiter(c)) {
      //     v
      // aaa (bbb    , sp==" "   buf==""
      // aaa ( bbb   , sp==" "   buf==""
      if(sp.size() && !isdelimiter(sp[0])) {
	sp=""; sp += c;
	continue;
      }
      //     v
      // aaa((bbb    , sp=="("    buf==""
      // aaa(( bbb   , sp=="("    buf==""
      //  aaa(bbb    , sp==""    buf==""
      //  aaa( bbb   , sp==""    buf==""
      sep.push_back(sp); sepv.push_back((char*)sep.back().c_str()); sp="";
      goto end_of_readword;
    }
    if(isendofline(c) || c==EOF || c==0) {
      goto end_of_readword;
    }
    if(!isseparator(c)) {// the first non-delimiter, non-separator char
      if(isquotation(c)) {
        if(keepquotation) buf += c;
        ++isquote;
      }
      else if(c=='\\') {
        c = parsebuf?parsebuf[iparse++]:fgetc(fp);
        wholeline += c;
        buf += c;
	if(c=='\n') ++line;
      }
      else buf += c;
      if(IsDBCSLeadByte(c)) {
        c = parsebuf?parsebuf[iparse++]:fgetc(fp);
        wholeline += c;
        CheckDBCS2ndByte(c);
        buf += c;
	//debugdisp("wholeline0",wholeline); // DEBUG
      }
      //debugdisp("wholeline1",wholeline); // DEBUG
      //debugdisp("buf1",buf); // DEBUG
      break;
    }
    else {// delimiter or separator char
      if(sp.size()==0 || !isdelimiter(sp[0])) sp += c;
    }
  } 

  sep.push_back(sp); sepv.push_back((char*)sep.back().c_str()); sp="";

  // read until delimiter or separator char
  while(1) {
    c = parsebuf?parsebuf[iparse++]:fgetc(fp);
    wholeline += c;
    //debugdisp("wholeline2",wholeline); // DEBUG
    //debugdisp("buf2",buf); // DEBUG
    if(((isdelimiter(c) || isseparator(c) || isendofline(c))&& 0==isquote%2) || c==EOF || c==0) {
      //printf("reached separator %d %c\n",c,c); //DEBUG
      goto end_of_readword;
    }
    if(isquotation(c)) {
      ++isquote;
      if(keepquotation) buf += c;
      else if(0==isquote%3) buf += "\"";
    }
    else if(c=='\\') {
      c = parsebuf?parsebuf[iparse++]:fgetc(fp);
      wholeline += c;
      buf += c;
    }
    else {
      buf += c;
    }
    if(IsDBCSLeadByte(c)) {
      c = parsebuf?parsebuf[iparse++]:fgetc(fp);
      wholeline += c;
      CheckDBCS2ndByte(c);
      buf += c;
    }
  }

 end_of_readword:
  if(isendofline(c) || !c || c==EOF) {
    if(wholeline.size()) wholeline.resize(wholeline.size()-1);
    //if(c==EOF) line = -1;
    if(c=='\r') {
      c = parsebuf?parsebuf[iparse++]:fgetc(fp);
      if(c!='\n') {
	--iparse;
	fseek(fp,-1,SEEK_CUR);
      }
    }
  }
  else {
    sp += c;
  }
  return(c);
}

/*****************************************************************************
* Reading one word
*****************************************************************************/
int ReadF::readword() {
  int c;
  int isquote = 0;

  if((!fp && !parsebuf) || line<0) return(0);
  string buf;

  while(1) {
    c = parsebuf?parsebuf[iparse++]:fgetc(fp);
    if(isdelimiter(c) || isendofline(c) || c==EOF || c==0) {
      goto end_of_readword;
    }
    if(!isseparator(c)) {
      if(isquotation(c)) {
        if(keepquotation) buf += c;
	++isquote;
      }
      else if(c=='\\') {
        c = parsebuf?parsebuf[iparse++]:fgetc(fp);
        buf += c;
      }
      else buf += c;
      if(IsDBCSLeadByte(c)) {
        c = parsebuf?parsebuf[iparse++]:fgetc(fp);
        CheckDBCS2ndByte(c);
        buf += c;
      }
      break;
    }
  } 

  while(1) {
    c = parsebuf?parsebuf[iparse++]:fgetc(fp);
    if(((isdelimiter(c) || isseparator(c) || isendofline(c))&& 0==isquote%2) || c==EOF || c==0) {
      goto end_of_readword;
    }
    if(isquotation(c)) {
      ++isquote;
      if(keepquotation) buf += c;
      else if(0==isquote%3) buf += "\"";
    }
    else if(c=='\\') {
      c = parsebuf?parsebuf[iparse++]:fgetc(fp);
      buf += c;
    }
    else buf += c;
    if(IsDBCSLeadByte(c)) {
      c = parsebuf?parsebuf[iparse++]:fgetc(fp);
      CheckDBCS2ndByte(c);
      buf += c;
    }
  }

 end_of_readword:
  if(c==EOF) {
    line = -1;
  }
  arg.clear();
  argv.clear();
  if(buf.size()) {
    arg.push_back((char*)buf.c_str());
    argv.push_back((char*)arg.back().c_str());
  }
  argc=argv.size();
  return(c);
}

/*****************************************************************************
* disp()
*****************************************************************************/
void ReadF::disp() {
  unsigned int i=0;
  printf("%4d %3d ",line,argc);
#if 1
  if(i<sepv.size()) printf(" \"%s\" ",sepv[i]);
  for(i=1;i<=(unsigned int)argc;i++) {
    if(i<argv.size()) printf(" '%s' ",argv[i]);
    if(i<sepv.size()) printf(" \"%s\" ",sepv[i]);
  }
#else
  for(i=1;i<=argc;i++) {
    printf("| %s ",argv[i]);
  }
#endif
  printf("\n");
}

//////////////////////////////////////////////////////////////////////////


