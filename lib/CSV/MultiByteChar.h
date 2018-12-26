/*****************************************************************************
* MultiByteChar.h
*
* Multi byte character handling library
*
* 2012/Nov/9 Masaharu Goto
*
*****************************************************************************/


#ifndef E__MULTIBYTELIB
#define E__MULTIBYTELIB

class G__MultiByteMode {
 public:
 //private:
 enum MultiByteType {
   E__UNKNOWNCODING ,
   E__EUC           ,
   E__SJIS          ,
   E__JIS           ,
   E__ONEBYTE       ,
   E__UNICODE       ,
   E__NONE
 };
 public:
  G__MultiByteMode() {init();}
  void init() { lang=defaultlang; }
  static void setDefaultLang(int defaultin) {defaultlang=defaultin;}
  static void setUNICODE() {defaultlang=E__UNICODE;}
  static void setSJIS() {defaultlang=E__SJIS;}
  static void setEUC() {defaultlang=E__EUC;}
  static void setUNKNOWN() {defaultlang=E__UNKNOWNCODING;}
  void SetCodingSystem() {lang=defaultlang;}
  void SetCodingSystem(int coding) {lang=coding;}
  int GetCodingSystem() {return(lang);}
  //protected:
  int IsDBCSLeadByte(int c) {
    if(lang==E__UNICODE) return(0);
    return((0x80&c)&&E__EUC!=lang&&CodingSystem(c));
  }
  void CheckDBCS2ndByte(int c) {if(0==(0x80&c)) lang=E__SJIS; }
  int CodingSystem(int c) {
    if(-33>=c && -79<=c) return(0); // half kana
    c &= 0x7f;
    switch(lang) {
    case E__UNKNOWNCODING:
      if(0x1f<c&&c<0x60) {
        /* assuming there is no half-sized kana chars, this code does not
         * exist in S-JIS, set EUC flag and return 0 */
        lang=E__EUC;
        return(0); 
      }
      return(1); /* assuming S-JIS but not sure yet */
    case E__EUC:
      return(0);
    case E__SJIS:
      if(c<=0x1f || (0x60<=c && c<=0x7c)) return(1);
      else                                return(0);
    case E__ONEBYTE: 
    case E__UNICODE: 
      return(0);
    }
    return(1);
  }
  char* strchr_multibyte(const char* s,int c) {
    char *a = (char*)s;
    while(*a) {
      if(IsDBCSLeadByte(*a)) ++a;
      else if(*a == c) return(a);  
      ++a;
    }
    return(0);
  }
 private:
  int lang;
  static int defaultlang;
};

#endif
