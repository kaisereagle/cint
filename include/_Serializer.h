/*****************************************************************************
 * _Serialize.h
 *  Description: 
 *    Object serialization library
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


#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <cstdio>
#include <cstdlib>
#include "ReadF.h"
using namespace std;

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////
// Fundamental types
///////////////////////////////////////////////////////////////////////
int _serializeOut(FILE* fp,char a,const string& name) {
  if(!fp) return(255); 
  if(name!="") fprintf("%s,",name.c_str());
  fprintf(fp,"%d,",a); 
  return(0); 
}
int _serializeOut(FILE* fp,short a,const string& name) {
  if(!fp) return(255); 
  if(name!="") fprintf("%s,",name.c_str());
  fprintf(fp,"%d,",a); 
  return(0); 
}
int _serializeOut(FILE* fp,int a,const string& name)  {
  if(!fp) return(255); 
  if(name!="") fprintf("%s,",name.c_str());
  fprintf(fp,"%d,",a); 
  return(0); 
}
int _serializeOut(FILE* fp,long a,const string& name) {
  if(!fp) return(255); 
  if(name!="") fprintf("%s,",name.c_str());
  fprintf(fp,"%ld,",a); 
  return(0); 
}
int _serializeOut(FILE* fp,float a,const string& name) {
  if(!fp) return(255); 
  if(name!="") fprintf("%s,",name.c_str());
  fprintf(fp,"%g,",a); 
  return(0); 
}
int _serializeOut(FILE* fp,double a,const string& name) {
  if(!fp) return(255); 
  if(name!="") fprintf("%s,",name.c_str());
  fprintf(fp,"%g,",a); 
  return(0); 
}
int _serializeOut(FILE* fp,unsigned char a,const string& name)  {
  if(!fp) return(255); 
  if(name!="") fprintf("%s,",name.c_str());
  fprintf(fp,"%u,",a); 
  return(0); 
}
int _serializeOut(FILE* fp,unsigned short a,const string& name) {
  if(!fp) return(255); 
  if(name!="") fprintf("%s,",name.c_str());
  fprintf(fp,"%u,",a); 
  return(0); 
}
int _serializeOut(FILE* fp,unsigned int a,const string& name)  {
  if(!fp) return(255); 
  if(name!="") fprintf("%s,",name.c_str());
  fprintf(fp,"%u,",a); 
  return(0); 
}
int _serializeOut(FILE* fp,unsigned long a,const string& name) {
  if(!fp) return(255); 
  if(name!="") fprintf("%s,",name.c_str());
  fprintf(fp,"%lu,",a); 
  return(0); 
}


///////////////////////////////////////////////////////////////////////
// string types
///////////////////////////////////////////////////////////////////////
int _serializeOut(FILE* fp,const string& a,const string& name) {
  if(!fp) return(255); 
  if(name!="") fprintf("%s,",name.c_str());
  fprintf(fp,"%s,",a.c_str()); 
  return(0); 
}

int _serializeOut(FILE* fp,const char* a,const string& name) {
  if(!fp) return(255); 
  if(name!="") fprintf("%s,",name.c_str());
  if(a) fprintf(fp,"%s,",a); 
  else  fprintf(fp,",");
  return(0); 
}


///////////////////////////////////////////////////////////////////////
// Container
///////////////////////////////////////////////////////////////////////
template<class T> 
int _serializeOut(FILE* fp,const vector<T>& a,const string& name) {
  if(!fp) return(255);
  if(name!="") fprintf("%s,",name.c_str());
  _serializeOut(fp,"size",a.size());
  vector<T>::iterator first,last=a.end();
  for(first=a.begin();first!=last;++first) _serializeOut(fp,(*first));
  return(0);
}

template<class T> 
int _serializeOut(FILE* fp,const deque <T>& a,const string& name) {
  if(!fp) return(255);
  if(name!="") fprintf("%s,",name.c_str());
  _serializeOut(fp,"size",a.size());
  _serializeOut(fp,a.size());
  deque<T>::iterator first,last=a.end();
  for(first=a.begin();first!=last;++first) _serializeOut(fp,(*first));
  return(0);
}

template<class T> 
int _serializeOut(FILE* fp,const list<T>& a,const string& name) {
  if(!fp) return(255);
  if(name!="") fprintf("%s,",name.c_str());
  _serializeOut(fp,"size",a.size());
  _serializeOut(fp,a.size());
  list<T>::iterator first,last=a.end();
  for(first=a.begin();first!=last;++first) _serializeOut(fp,(*first));
  return(0);
}

template<class Key,Val> 
int _serializeOut(FILE* fp,const pair<Key,Val>& a,const string& name) {
  if(!fp) return(255);
  if(name!="") fprintf("%s,",name.c_str());
  _serializeOut(fp,"size",a.size());
  _serializeOut(fp,a.size());
  _serializeOut(fp,(*first).first);
  _serializeOut(fp,(*first).second);
  return(0);
}

template<class Key,Val> 
int _serializeOut(FILE* fp,const map<Key,Val>& a,const string& name) {
  if(!fp) return(255);
  if(name!="") fprintf("%s,",name.c_str());
  _serializeOut(fp,"size",a.size());
  _serializeOut(fp,a.size());
  map<Key,Val>::iterator first,last=a.end();
  for(first=a.begin();first!=last;++first) _serializeOut(fp,(*first));
  return(0);
}

template<class Key,Val> 
int _serializeOut(FILE* fp,const multimap<Key,Val>& a,const string& name) {
  if(!fp) return(255);
  if(name!="") fprintf("%s,",name.c_str());
  _serializeOut(fp,"size",a.size());
  _serializeOut(fp,a.size());
  map<Key,Val>::iterator first,last=a.end();
  for(first=a.begin();first!=last;++first) _serializeOut(fp,(*first));
  return(0);
}


///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
// streaming object and operator &
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
class SerializeOut {
 public:
  FILE* fp;
 public:
  SerializeOut(FILE* fpin) { init(fpin); }
  SerializeOut(const string& fname,const string& mode="w") {open(fname,mode); }
  int init(FILE* fpin) { fp=fpin; return(fp?1:0); }
  init open(const string& fname,const string& mode="w") {
    fp=fopen(fname.c_str(),mod.c_str());
    return(fp?1:0);
  }
  int close() {if(!fp) return(255); int stat=fclose(fp); fp=0; return(stat);}
};

template<class T> 
SerializeOut& _serializeOut(SerializeOut& s,T& a,const string& name) {
  _serializeOut(s.fp,name,a);
  return(*this);
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
// SerializeIn
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////
// Fundamental types
///////////////////////////////////////////////////////////////////////
// common template
template<class T>
int _serializeIn(FILE* fp,T& a,const string& name) {
  ReadF f(fp); f.setseparator(""); f.setdelimiter(",");
  return(_serializeIn(f,name,a)); return(0);
}

///////////////////////////////////////////////////////////////////////
int _serializeIn(ReadF& f,char& a,const string& name) {
  string sp,buf; 
  if(name!="") do { f.readword(sp,buf); } while(buf!=name);
  f.readword(sp,buf); a=atoi(buf.c_str()); 
  return(0);
}
int _serializeIn(ReadF& f,short& a,const string& name) {
  string sp,buf; 
  if(name!="") do { f.readword(sp,buf); } while(buf!=name);
  f.readword(sp,buf); a=atoi(buf.c_str()); 
  return(0);
}
int _serializeIn(ReadF& f,int& a,const string& name) {
  string sp,buf; 
  if(name!="") do { f.readword(sp,buf); } while(buf!=name);
  f.readword(sp,buf); a=atoi(buf.c_str()); 
  return(0);
}
int _serializeIn(ReadF& f,long& a,const string& name) {
  string sp,buf; 
  if(name!="") do { f.readword(sp,buf); } while(buf!=name);
  f.readword(sp,buf); a=atoi(buf.c_str()); 
  return(0);
}
int _serializeIn(ReadF& f,float& a,const string& name) {
  string sp,buf; 
  if(name!="") do { f.readword(sp,buf); } while(buf!=name);
  f.readword(sp,buf); a=atoi(buf.c_str()); 
  return(0);
}
int _serializeIn(ReadF& f,double& a,const string& name) {
  string sp,buf; 
  if(name!="") do { f.readword(sp,buf); } while(buf!=name);
  f.readword(sp,buf); a=atoi(buf.c_str()); 
  return(0);
}
int _serializeIn(ReadF& f,unsigned char& a,const string& name) {
  string sp,buf; 
  if(name!="") do { f.readword(sp,buf); } while(buf!=name);
  f.readword(sp,buf); a=atoi(buf.c_str()); 
  return(0);
}
int _serializeIn(ReadF& f,unsigned short& a,const string& name) {
  string sp,buf; 
  if(name!="") do { f.readword(sp,buf); } while(buf!=name);
  f.readword(sp,buf); a=atoi(buf.c_str()); 
  return(0);
}
int _serializeIn(ReadF& f,unsigned int& a,const string& name) {
  string sp,buf; 
  if(name!="") do { f.readword(sp,buf); } while(buf!=name);
  f.readword(sp,buf); a=atoi(buf.c_str()); 
  return(0);
}
int _serializeIn(ReadF& f,unsigned long& a,const string& name) {
  string sp,buf; 
  if(name!="") do { f.readword(sp,buf); } while(buf!=name);
  f.readword(sp,buf); a=atoi(buf.c_str()); 
  return(0);
}

///////////////////////////////////////////////////////////////////////
// string types
///////////////////////////////////////////////////////////////////////
int _serializeIn(ReadF& f,string& a,const string& name) {
  string sp,buf; 
  if(name!="") do { f.readword(sp,buf); } while(buf!=name);
  f.readword(sp,buf); a = buf.c_str(); 
  return(0);
}

int _serializeIn(ReadF& f,char* a,const string& name) {
  if(!a) return(255); 
  string sp,buf; 
  if(name!="") do { f.readword(sp,buf); } while(buf!=name);
  f.readword(sp,buf); strcpy(a,buf.c_str()); 
  return(0);
}

///////////////////////////////////////////////////////////////////////
// Container
///////////////////////////////////////////////////////////////////////
template<class T> 
int _serializeIn(FILE* fp,const vector<T>& a,const string& name) {
  if(!fp) return(255);
  string sp,buf; 
  ReadF f(fp); f.setseparator(""); f.setdelimiter(",");
  if(name!="") do { f.readword(sp,buf); } while(buf!=name);
  unsigned int Size,i; T v;
  _serializeIn(f,"size",Size);
  for(i=0;i<Size;++i) { _serializeIn(fp,"",v); a.push_back(v); }
  return(0);
}

template<class T> 
int _serializeIn(FILE* fp,const deque<T>& a,const string& name) {
  if(!fp) return(255);
  string sp,buf; 
  ReadF f(fp); f.setseparator(""); f.setdelimiter(",");
  if(name!="") do { f.readword(sp,buf); } while(buf!=name);
  unsigned int Size,i; T v;
  _serializeIn(f,"size",Size);
  for(i=0;i<Size;++i) { _serializeIn(fp,"",v); a.push_back(v); }
  return(0);
}

template<class T> 
int _serializeIn(FILE* fp,const list<T>& a,const string& name) {
  if(!fp) return(255);
  string sp,buf; 
  ReadF f(fp); f.setseparator(""); f.setdelimiter(",");
  if(name!="") do { f.readword(sp,buf); } while(buf!=name);
  unsigned int Size,i; T v;
  _serializeIn(f,"size",Size);
  for(i=0;i<Size;++i) { _serializeIn(fp,"",v); a.push_back(v); }
  return(0);
}

template<class Key,class Val> 
int _serializeIn(FILE* fp,const pair<T>& a,const string& name) {
  if(!fp) return(255);
  string sp,buf; 
  ReadF f(fp); f.setseparator(""); f.setdelimiter(",");
  if(name!="") do { f.readword(sp,buf); } while(buf!=name);
  Key k; Val v;
  _serializeIn(fp,"",k); _serializeIn(fp,"",v); a.first=k; a.second=v;
  return(0);
}

template<class Key,class Val> 
int _serializeIn(FILE* fp,const map<T>& a,const string& name) {
  if(!fp) return(255);
  string sp,buf; 
  ReadF f(fp); f.setseparator(""); f.setdelimiter(",");
  if(name!="") do { f.readword(sp,buf); } while(buf!=name);
  Key k; Val v;
  unsigned int Size,i;
  _serializeIn(fp,"size",Size);
  for(i=0;i<Size;++i) {
    _serializeIn(fp,"",k); _serializeIn(fp,"",v); 
    a[k]=v;
  }
  return(0);
}

template<class Key,class Val> 
int _serializeIn(FILE* fp,const multimap<T>& a,const string& name) {
  if(!fp) return(255);
  string sp,buf; 
  ReadF f(fp); f.setseparator(""); f.setdelimiter(",");
  if(name!="") do { f.readword(sp,buf); } while(buf!=name);
  Key k; Val v; pair<k,v> p;
  unsigned int Size,i;
  _serializeIn(fp,"size",Size);
  for(i=0;i<Size;++i)  {
    _serializeIn(fp,"",k); _serializeIn(fp,"",v); 
    p.first=k; p.second=v; a.insert(p); 

  }
  return(0);
}


///////////////////////////////////////////////////////////////////////
class SerializeIn {
 public:
  ReadF f;
 public:
  SerializeIn(FILE* fpin) { init(fpin);  }
  SerializeIn(const string& fname,const string& mode="r") {open(fname,mode); }
  init() { f.setseparator("") f.setdelimiter(","); }
  int init(FILE* fpin) { init(); f.setFP(fpin); return(f.isvalid());}
  int open(const string& fname,const string& mode="r") 
    {init(); f.fopen(fname.c_str()); return(f.isvalid());}
  int close() {return(f.close());}
  int close() {if(!fp) return(255); int stat=fclose(fp); fp=0; return(stat);}
};

template<class T> 
SerializeIn& _serializeIn(SerializeIn& s,T& a,const string& name) {
  string sp,buf; 
  if(name!="") do { s.f.readword(sp,buf); } while(buf!=name);
  _serializeIn(s.f,name,a);
  return(*this);
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
struct _serializerIn_t {
  void *p;
  int (*f)(SerializeIn&,void* p);
};


///////////////////////////////////////////////////////////////////////
SerializeIn& _serializeIn(SerializeIn& a,_EXAMPLE& a,const string& name) {
  map<string,_serializerIn_t> m;
  _serializerIn_t *si;
  while(a.f.read()) {
    if(a.f.argc>0) {
      if(strcmp(a.f.argv[1],"$$END")==0) return(a);
      si=m[a.f.argv[0]];
      if(si) {
	si->f(a,p);
      }
    }
  }
}
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
class SerialInChar : public SerializeIn {
 public:
  f.read();
};


///////////////////////////////////////////////////////////////////////
#endif // SERIALIZER_H

///////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
class _SerializeIn {
 protected:
  map<string,pair<_SerializeIn*,size_t> > m_map;
 public:
  virtual _SerializeIn& read(SerializeIn& in,void* pobj) { // for class object
    pair<SerializeIn*,size_t> p;
    while(in.f.read()) {
      if(strcmp(int.f.argv[1],"$END")==0) return(in);
      p = m_map[in.f.argv[1]];
      if(p.first) {
	p.first->read(in,pobj+p.second);
      }
    }
  }
};

///////////////////////////////////////////////////////////////////////
class _SerializeIn_int : public _SerializeIn {
 public:
  virtual _SerializeIn& read(SerializeIn& in,void* pobj) 
  {int* p = (int*)pobj; *p = atoi(in.f.argv[2]); return(in);}
} _SerializeIn_int_obj;

class _SerializeIn_double : public _SerializeIn {
 public:
  virtual _SerializeIn& read(SerializeIn& in,void* pobj) 
  {double* p = (double*)pobj; *p = atof(in.f.argv[2]); return(in);}
} _SerializeIn_double_obj;

class _SerializeIn_string : public _SerializeIn {
 public:
  virtual _SerializeIn& read(SerializeIn& in,void* pobj) 
  {string* p = (string*)pobj; *p = in.f.argv[2]; return(in);}
} _SerializeIn_string_obj;

class _SerializeIn_char_string: public _SerializeIn {
 public:
  virtual _SerializeIn& read(SerializeIn& in,void* pobj) 
  {char** p = (char**)pobj; strcpy(*p,in.f.argv[2]); return(in);}
} _SerializeIn_char__obj;

class _SerializeIn_char_pointer: public _SerializeIn {
 public:
  virtual _SerializeIn& read(SerializeIn& in,void* pobj) 
  {char** p = (char**)pobj; strcpy(*p,in.f.argv[2]); return(in);}
} _SerializeIn_char__obj;

template<class T>
class _SerializeIn_vector : public _SerializeIn {
 public:
  virtual _SerializeIn& read(SerializeIn& in,void* pobj) {
    vector<T>* p = (vector<T>*)pobj; 
    ; 
    return(in);
  }
} ;

#if 1
///////////////////////////////////////////////////////////////////////
// class
///////////////////////////////////////////////////////////////////////
class _EXAMPLE {
 public:
  int a; //+serialize
  double b; //+serialize
  string name; //+serialize
  vector<int> ary; //+serialize
 public:
  friend int _serializeOut(FILE* fp,const _EXAMPLE& a,const string& name);
  friend int _serializeIn(ReadF& f,_EXAMPLE& a,const string& name);
  friend int _serializeIn(FILE* fp,_EXAMPLE& a,const string& name);
};

// Automatically generated code
int _EXAMPLE::_serializeOut(FILE* fp,const _EXAMPLE& a,const string& name) {
  int stat=0;
  if(name!="") fprintf("%s,",name.c_str());
  stat+=_serializeOut(fp,"a",a.a);
  stat+=_serializeOut(fp,"b",a.b);
  stat+=_serializeOut(fp,"name",a.name);
  stat+=_serializeOut(fp,"ary",a.ary);
  return(stat);
}

class _SerializeIn__EXAMPLE : public _SerializeIn {
  _SerializeIn__EXAMPLE() {
    m_map["a"]=pair<_SerializeIn*,size_t>(_SerializeIn_int_obj
				       ,offsetof(_EXAMPLE,a));
    m_map["b"]=pair<_SerializeIn*,size_t>(_SerializeIn_double_obj
				       ,offsetof(_EXAMPLE,b));
    m_map["name"]=pair<_SerializeIn*,size_t>(_SerializeIn_string_obj
				       ,offsetof(_EXAMPLE,name));
    m_map["ary"]=pair<_SerializeIn*,size_t>(_SerializeIn_vectorint_obj
				       ,offsetof(_EXAMPLE,ary));
  }
} _SerializeIn__EXAMPLE_obj;


int _EXAMPLE::_serializeIn(ReadF& f,string& name,_EXAMPLE& a) {
  int stat=0;
  string sp,buf; 
  if(name!="") do { f.readword(sp,buf); } while(buf!=name);
  stat+=_serializeIn(f,"a",a.a);
  stat+=_serializeIn(f,"b",a.b);
  stat+=_serializeIn(f,"name",a.name);
  stat+=_serializeIn(f,"ary",a.ary);
  return(stat);
}

int _EXAMPLE::_serializeIn(FILE* fp,_EXAMPLE& a,const string& name) {
  int stat=0;
  ReadF f; f.setseparator(""); f.setdelimiter(",");
  return(_serializeIn(f,name,a));
}

#endif

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

