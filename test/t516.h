
class arr2 {
 public:
  double *p;
  double& operator[](int n) { return p[n]; }
};


class arr {
 public:
  double dat[10][10];
  arr2 operator[](int n) { 
    arr2 t;
    t.p = dat[n];
    return(t);
  }
};

