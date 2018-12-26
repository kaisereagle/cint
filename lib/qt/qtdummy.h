// qtdummy.h

/////////////////////////////////////////////////////////////////////
// Emulation of string constant in macro
/////////////////////////////////////////////////////////////////////
const char* clicked() { return("clicked()"); }
const char* quit() { return("quit()"); }

/////////////////////////////////////////////////////////////////
// Emulation of macro
/////////////////////////////////////////////////////////////////////

#undef METHOD
const char* METHOD(const char* action) {
  static char buf[100];
  sprintf(buf,"0%s",action);
  return(buf);
}

#undef SLOT
const char* SLOT(const char* action) {
  static char buf[100];
  sprintf(buf,"1%s",action);
  return(buf);
}

#undef SIGNAL
const char* SIGNAL(const char* action) {
  static char buf[100];
  sprintf(buf,"2%s",action);
  return(buf);
}


