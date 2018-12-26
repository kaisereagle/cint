/*****************************************************************************
 * _serialize.cxx
 *  Description: 
 *    class serializer code generator
 *  Usage:
 *  Status: Production
 *  Date: 2015/Sep/18
 *  Author: Masaharu Goto
 *****************************************************************************/
/************************************************************************
 * Copyright(c) 1995~2015  Masaharu Goto (cint@pcroot.cern.ch)
 *
 * Permission to use, copy, modify and distribute this software and its 
 * documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation. The author makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 ************************************************************************/

#include <string>
#include <vector>
#include <map>
#include <ertti>
#pragma include <EasyExp.h>

EasyExp eseri("*+seri*");
EasyExp ebind("*+bind*");
EasyExp eID("*ID*");
map<string,int> targetclass;
///////////////////////////////////////////////////////////////////////////////
void serializeClass(G__ClassInfo& cls) {
  //printf("%s:%d class %s {\n",cls.FileName(),cls.LineNumber(),cls.Name());
  printf("class %s {\n",cls.Name());
  G__DataMemberInfo dat(cls);
  while(dat.Next()) {
    long property = dat.Property();
    printf("  ");
    if(eseri.match(dat.Title())) printf("+");
    else if(ebind.match(dat.Title())) printf("*");
    else                     printf("    -");
    if(eID.match(dat.Title())) printf("&");
    if(property&G__BIT_ISPUBLIC)    printf("public: ");
    if(property&G__BIT_ISPROTECTED) printf("protected: ");
    if(property&G__BIT_ISPRIVATE)   printf("private: ");
    if(property&G__BIT_ISCONSTANT)  printf("const ");
    printf("%s %s; //%s\n",dat.Type()->Name(),dat.Name(),dat.Title());
  }
}
///////////////////////////////////////////////////////////////////////////////
void serialize(const map<string,int>& target) {
  G__ClassInfo cls;
  while(cls.Next()) {
    if(targetclass[cls.Name()]) {
      serializeClass(cls);
    }
  }
}
///////////////////////////////////////////////////////////////////////////////
void searchSerializerTarget(const map<string,int>& target) {
  G__ClassInfo cls;
  G__DataMemberInfo dat;
  while(cls.Next()) {
    long property = cls.Property();
    if(property&(G__BIT_ISCLASS|G__BIT_ISSTRUCT)) {
      if(!cls.FileName() || target[cls.FileName()]==0) continue;
      dat.Init(cls);
      while(dat.Next()) {
	if(eseri.match(dat.Title())) ++targetclass[cls.Name()];
	if(ebind.match(dat.Title())) ++targetclass[cls.Name()];
	if(eID.match(dat.Title())) ++targetclass[cls.Name()];
      }
    }
  }
}
///////////////////////////////////////////////////////////////////////////////
void loadfiles(const map<string,int>& target) {
  map<string,int>::iterator first,last=target.end();
  for(first=target.begin();first!=last;++first) {
    if((*first).second) {
      G__loadfile((*first).first.c_str());
    }
  }
}
///////////////////////////////////////////////////////////////////////////////

int main(int argc,char** argv) {
  map<string,int> target;
  for(int i=1;i<argc;i++) {
    if(strcmp(argv[i],"-t")==0) target[argv[++i]]=1;
    else if(strcmp(argv[i],"-?")==0) {
      printf("Usage: cint _serialize.cxx <option> [fileExpr]\n");
      printf("  -t [target_src]  : target source file\n");
      exit(0);
    }
  }
  //loadfiles(target);
  searchSerializerTarget(target);
  serialize(target);
  G__pause();
  return(0);
}


///////////////////////////////////////////////////////////////////////////////

