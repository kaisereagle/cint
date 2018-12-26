
class TestOperator {
 public:
  typedef unsigned int value_type;

  TestOperator():m_int(1234)  { }

  // This line works
  //operator unsigned int () { return m_int; }

  // this line does not work.
  operator value_type () { return m_int; }

 private:
  unsigned int m_int;

};


