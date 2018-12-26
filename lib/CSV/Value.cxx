// Value.cxx

#include "Value.h"

///////////////////////////////////////////////////
void value::set(double x) { 
  char tmp[50];
  sprintf(tmp,"%g",x);
  s = tmp;
  d = x;
  // cstr=s.c_str();
}

///////////////////////////////////////////////////
void value::set(const char* x) {
  count = 0;
  s=x; 
  //cstr=s.c_str();
  if(isdbl(x)) {
    d=atof(x); 
  }
  else {
    d=0.0;
  }
}
///////////////////////////////////////////////////
void value::set(const string& x) {
  count = 0;
  s=x; 
  //cstr=s.c_str();
  if(isdbl(x)) d=atof(x.c_str()); 
  else d=0.0;
}

///////////////////////////////////////////////////
void value::disp(FILE* fp) const {
#ifdef DOUBLEQUOTEESCAPE
  if((strchr(s.c_str(),'"'))) 
    PrintDoubleQuote(fp,s);
  else 
#endif
#ifdef DOUBLEQUOTEESCAPE2
  if((strchr(s.c_str(),'"'))) 
    fprintf(fp,"\"%s\",",s.c_str());
  else 
#endif
  if((strchr(s.c_str(),',')||strchr(s.c_str(),'\n')) && s[0]!='"') 
    fprintf(fp,"\"%s\",",s.c_str());
  else            
    fprintf(fp,"%s,",s.c_str());
}
///////////////////////////////////////////////////
void value::PrintDoubleQuote(FILE* fp,const string& s) const {
  fprintf(fp,"\"");
  for(unsigned int i=0;i<s.size();i++) {
    if(s[i]=='"') fprintf(fp,"\"\"");
    else          fprintf(fp,"%c",s[i]);
  }
  fprintf(fp,"\",");
}

///////////////////////////////////////////////////
value operator+(const value& a,const value& b) {
  value c;
  if(a.isdouble() && b.isdouble()) c.set(a.d+b.d);
  else c.set(a.s+b.s);
  return(c);
}
///////////////////////////////////////////////////
value operator-(const value& a,const value& b) {
  value c;
  if(a.isdouble() && b.isdouble()) c.set(a.d-b.d);
  else {c=a; c.s+="+"; c.s+=b.s; c.d=0; /*c.cstr=c.s.c_str();*/}
  return(c);
}
///////////////////////////////////////////////////
value operator*(const value& a,const value& b) {
  value c;
  if(a.isdouble() && b.isdouble()) c.set(a.d*b.d);
  else {c=a; c.s+="*"; c.s+=b.s; c.d=0; /*c.cstr=c.s.c_str();*/}
  return(c);
}
///////////////////////////////////////////////////
value operator/(const value& a,const value& b) {
  value c;
  if(a.isdouble() && b.isdouble()) c.set(a.d/b.d);
  else {c=a; c.s+="/"; c.s+=b.s; c.d=0; /*c.cstr=c.s.c_str();*/}
  return(c);
}
///////////////////////////////////////////////////
bool operator==(const value& a,const value& b) {
  if(a.isdouble() && b.isdouble()) return(a.d==b.d);
  return(a.s==b.s);
}
///////////////////////////////////////////////////
bool operator!=(const value& a,const value& b) {
  if(a.isdouble() && b.isdouble()) return(a.d!=b.d);
  return(a.s!=b.s);
}
///////////////////////////////////////////////////
bool operator<(const value& a,const value& b) {
  if(a.isdouble() && b.isdouble()) return(a.d<b.d);
  return(a.s<b.s);
}
///////////////////////////////////////////////////
bool operator>(const value& a,const value& b) {
  if(a.isdouble() && b.isdouble()) return(a.d>b.d);
  return(a.s>b.s);
}
///////////////////////////////////////////////////

bool isdbl(const string& s) {
  if(!s.size()) return(false);
  switch(s[0]) {
  case '.':
  case '-':
  case '+':
    if(s.size()<2) return(false);
    if(isdigit(s[1])||s[1]=='.') return(true);
    else return(false);
  default:
    if(isdigit(s[0])) return(true);
    else return(false);
  }
  return(false);
}
///////////////////////////////////////////////////

bool isint(const string& s) {
  if(!s.size()) return(false);
  int n=s.size();
  for(int i=0;i<n;i++) {
    switch(s[0]) {
    case ',':
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      break;
    default:
      return(false);
    }
  }
  return(true);
}
///////////////////////////////////////////////////
