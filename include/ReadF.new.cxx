/*****************************************************************************
* ReadF.C
*****************************************************************************/

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
#endif

/*****************************************************************************
* Constructors and destructors
*****************************************************************************/
ReadF::ReadF(const char *filename)
{
  //fp = fopen(filename,"rb");
  fp = fopen(filename,"r");
  initialize();
  if(fp) {
    openflag=1;
  }
  else {
    fprintf(stderr,"ReadF: cannot open %s\n",filename);
  }
}

ReadF::ReadF(FILE *fpin)
{
  fp = fpin;
  openflag=0;
  initialize();
  if((FILE*)NULL==fpin) {
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
  if(fp) openflag=1;
  else fprintf(stderr,"ReadF: can not open %s\n",filename);
  return(openflag);
}

int ReadF::close() {
  if(openflag && fp) fclose(fp);
  fp=(FILE*)NULL;
  openflag=0;
  return(1);
}


/*****************************************************************************
* set routines
*****************************************************************************/
void ReadF::initialize()
{
  line = 0;
  argc = 0;
  setseparator(" \t\v\r\f");
  setendofline("\n");
  setdelimiter("");
  setquotation("");
  keepquotation = false;
  escapebyquotation = false;

  G__MultiByteMode::init();
  noeofcat = 0;

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
  if(!fp && !parsebuf) return(0);

  argv.clear();
  arg.clear();
  sepv.clear();
  sep.clear();
  wholeline = "";
  argv.push_back(wholeline.c_str());
  int c;
  string buf,sp;
  while(1) {
    c=readword(sp,buf);
    if(sp.size()) {
      wholeline += sp;
      sep.push_back(sp); sepv.push_back(sep.back().c_str());
    }
    if(buf.size()) {
      wholeline += buf;
      arg.push_back(buf.c_str()); argv.push_back(arg.back().c_str());
    }
    if(c && c!=EOF && !isendofline(c)) wholeline += c;
    argv[0] = wholeline.c_str();
    argc = arg.size();
    if(isendofline(c) || !c) return(1);
    if(EOF==c) {
      line = -1;
      return(0);
    }
    ++line;
  } 

  return(0); // never reach here
}

/*****************************************************************************
* Reading one word
*****************************************************************************/
int ReadF::readword(string& sp,string& buf) {
  int c;
  int isquote = 0;
  buf ="";
  sp ="";

  if(!fp && !parsebuf) return(0);

  while(1) {
    c = parsebuf?parsebuf[iparse++]:fgetc(fp);
    if(isdelimiter(c) || isendofline(c) || c==EOF || c==0) {
      goto end_of_readword;
    }
    if(!isseparator(c)) {
      if(isquotation(c)) ++isquote;
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
    else {
      sp += c;
    }
  } 

  while(1) {
    c = parsebuf?parsebuf[iparse++]:fgetc(fp);
    if(((isdelimiter(c) || isseparator(c) || isendofline(c))&& 0==isquote%2) || c==EOF || c==0) {
      goto end_of_readword;
    }
    if(isquotation(c)) {
      ++isquote;
      if(0==isquote%3) buf += "\"";
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
  return(c);
}

/*****************************************************************************
* Reading one word
*****************************************************************************/
int ReadF::readword() {
  int c;
  int isquote = 0;

  if(!fp && !parsebuf) return(0);
  string buf;

  while(1) {
    c = parsebuf?parsebuf[iparse++]:fgetc(fp);
    if(isdelimiter(c) || isendofline(c) || c==EOF || c==0) {
      goto end_of_readword;
    }
    if(!isseparator(c)) {
      if(isquotation(c)) ++isquote;
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
      if(0==isquote%3) buf += "\"";
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
    arg.push_back(buf.c_str());
    argv.push_back(arg.back().c_str());
  }
  argc=argv.size();
  return(c);
}

/*****************************************************************************
* disp()
*****************************************************************************/
void ReadF::disp() {
  int i;
  printf("%4d %3d ",line,argc);
  for(i=1;i<=argc;i++) {
    printf("| %s ",argv[i]);
  }
  printf("\n");
}



