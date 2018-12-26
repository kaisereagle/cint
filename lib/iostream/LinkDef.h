/////////////////////////////////////////////////////////////////
// LinkDef.h
/////////////////////////////////////////////////////////////////

inline int G__ateval(const ostream& os) {
  printf("(ostream& %p)\n",&os);
  return(1);
}

inline int G__ateval(const istream& os) {
  printf("(istream& %p)\n",&os);
  return(1);
}

inline int G__ateval(const ofstream& os) {
  printf("(ofstream& %p)\n",&os);
  return(1);
}

inline int G__ateval(const ifstream& os) {
  printf("(ifstream& %p)\n",&os);
  return(1);
}

inline int G__ateval(const ostrstream& os) {
  printf("(ostrstream& %p)\n",&os);
  return(1);
}

inline int G__ateval(const istrstream& os) {
  printf("(istrstream& %p)\n",&os);
  return(1);
}

#ifdef __MAKECINT__
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
#pragma link off all classes;
#pragma link off all functions;
#pragma link off all global;

#pragma link C++ class ios_base;
#pragma link C++ class ios;
#pragma link C++ class ostream;
#pragma link C++ class istream;
#pragma link C++ class ostrstream;
#pragma link C++ class istrstream;
#pragma link C++ class ofstream;
#pragma link C++ class ifstream;

#if !defined(G__WIN32)
#pragma link C++ global cin;
#pragma link C++ global cout;
#pragma link C++ global cerr;
#endif
#pragma link C++ global clog;

#pragma link C++ function dec;
#pragma link C++ function endl;
#pragma link C++ function ends;
#pragma link C++ function flush;
#pragma link C++ function hex;
#pragma link C++ function oct;
#pragma link C++ function ws;

#pragma link C++ function G__ateval;

#endif
