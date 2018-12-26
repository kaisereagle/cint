/*****************************************************************************
* AbstractIO.h
*
* Description: Abstract Data I/O library associated with open source C++ interpreter CINT
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

#ifndef ABSTRACTIO_H
#define ABSTRACTIO_H

#include <string>
#include <vector>
#include <deque>

//////////////////////////////////////////////////////////////////////////
class Abstract_IO {
 public:
  virtual int write(FILE* fp,char a,const string& name)=0;
  virtual int write(FILE* fp,int a,const string& name)=0;
  virtual int write(FILE* fp,short a,const string& name)=0;
  virtual int write(FILE* fp,long a,const string& name)=0;
  virtual int write(FILE* fp,float a,const string& name)=0;
  virtual int write(FILE* fp,double a,const string& name)=0;

  template<class Container>
    virtual int write(FILE* fp,const Container& a,const string& name) {
    Container::const_iterator first,last=a.end();
    for(first=a.begin();first!=last;++first) {
      write(fp,(*first));
    }
  }
  
};

//////////////////////////////////////////////////////////////////////////
class CSV_IO : public Abstract_IO {
};

//////////////////////////////////////////////////////////////////////////
class XML_IO : public Abstract_IO {
};

//////////////////////////////////////////////////////////////////////////
class BIN_IO : public Abstract_IO {
};

//////////////////////////////////////////////////////////////////////////
#endif // ABSTRACTIO_H

