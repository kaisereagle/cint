
#include <iostream>

class xclass {
 public:
  void f(int open_mode = std::ios::in);
};
namespace myspace {
   class xclass {
   public:
       void f(int open_mode = std::ios::in);
   };
   class myclass {
   public:
       myclass(const char *name, int open_mode = std::ios::in);
   };
}

