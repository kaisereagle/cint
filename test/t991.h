
#ifndef TOBJECT
#define TOBJECT

typedef int Option_t;

class TObject {
 public:
  TObject() { }
  TObject(const TObject& x) { }
  virtual ~TObject() { }
};

#endif
