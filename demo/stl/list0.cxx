#include <stdio.h>
#include <iostream.h>
#include <list.h> // problem in precompiled list class and cint
#include <algo.h>
#include <assert.h>

main()
{
  list<int> test;
  //cout << "begin=" << test.begin() << " end=" << test.end() << endl;
  printf("begin=%x end=%x\n",test.begin(),test.end());
}
