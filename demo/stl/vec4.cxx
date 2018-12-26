#include <iostream.h>
#include <vector.h>
#include <algo.h>

int main()
{
  vector<int> vector1,vector2;
  const int N=10;
  for(int k=0;k!=N;++k) vector1.push_back(rand()%10);
  for(int k=0;k!=N;++k) cout << vector1[k] << " ";
  cout << endl;

  sort(vector1.begin(),vector1.end());
  
  for(int k=0;k!=N;++k) cout << vector1[k] << " ";
  cout << endl;
}

