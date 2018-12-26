
//#pragma includepath "\mpn\ke\proto3"
#include "VString.h"
#include "VObject.h"

class VCompany : public VObject {
 public:
  VCompany();
  VCompany(VCompany& x);
  VCompany(Char_t* nameIn,Char_t* countryIn);
  VCompany(Char_t* nameIn,Int_t num);
  VCompany& operator=(VCompany& x);
  ~VCompany();
  void set(Char_t* nameIn,Char_t* countryIn);
  Char_t* Name() { return(name.String()); }
  Char_t* Syozoku() { return(country.String()); }
  void disp();
 private:
  VString name;
  VString country;
};

