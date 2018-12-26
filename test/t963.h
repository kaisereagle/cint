// resolved
// 021211rootcint.txt, Maarten 
// 021214rootcint.txt
// masa/*
//  forward class declaration and enclosed class definition causes 
//  member function to be mis-interpreted.

#include <stdio.h>

template <class T> class TPhTopoData;
typedef TPhTopoData<int> TPhTopoDataI;

template <class T> class TPhTopoData {
 public:
  TPhTopoData() {}
  TPhTopoData(const TPhTopoData &a);
  void disp() const { printf("TPhTopoData<T>.disp()\n"); }
  class TError  {
   public:
    void disp() const { printf("TPhTopoData<T>::TError.disp()\n"); }
  };
};

template <class T> TPhTopoData<T>::TPhTopoData(const TPhTopoData<T> &a) {
  printf("TPhTopoData<T>::TPhTopoData()\n");
}

// tmplt.c line 2316  ON1587
