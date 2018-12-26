

#include <vector>
#ifndef __hpux
using namespace std;
#endif

typedef int Int_t;

#ifdef __MAKECINT__
#pragma link C++ class vector<vector< Int_t> >;
#endif
