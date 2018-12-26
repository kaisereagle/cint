
typedef float Float_t;
typedef double Double_t;
typedef int Int_t;

class TMath {
 public:
  static Float_t Sqrt(Float_t x) { return(x); }
};

Float_t x;
class TMatrix {
public:
  TMatrix() {}
  TMatrix(Int_t a,Int_t b) {}
  virtual Float_t& operator()(Int_t a,Int_t b) { x=a*b*1.1;return(x); }
};


