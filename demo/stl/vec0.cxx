#include <vector.h>
#include <iostream.h>
#include <stdio.h>

main()
{
  vector<int> test;
#if !(G__GNUC>=3)
  cout << "begin=" << test.begin() << " end=" << test.end() << endl;
#endif
  printf("begin=%x end=%x\n",test.begin(),test.end());
}
