
#include <string>

namespace crap {
  static std::string s_unknown=std::string("unknown");
  //the previous line core dumps, however the next line works:
  //static const std::string s_unknown("unknown");
}

int main() {
  printf("%s\n",crap::s_unknown.c_str());
  return 0;
}
