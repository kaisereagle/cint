#ifndef T1247
#define T1247

class A1 {
 private:
  A1(const A1& x) { }
 public:
  A1() { }
  friend class A2;
  void f(int x) { }
};

class A2 {
 private:
  A2() { }
 public:
  void f(int x) { }
};

class A3 {
 private:
  ~A3() { }
 public:
  friend class A2;
  void f(int x) { }
};

class A4 {
 private:
  A4& operator=(const A4& x) { return(*this); }
 public:
  void f(int x) { }
};

class C1 {
 protected:
  C1(const C1& x) { }
 public:
  C1() { }
  void f(int x) { }
};

class C2 {
 protected:
  C2() { }
 public:
  void f(int x) { }
};

class C3 {
 protected:
  ~C3() { }
 public:
  void f(int x) { }
};

class C4 {
 protected:
  C4& operator=(const C4& x) { return(*this); }
 public:
  void f(int x) { }
};
#endif
