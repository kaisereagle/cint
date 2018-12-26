// refassign.cxx

#include <stdio.h>

double x[10];
double& func(int i)
{
  return(x[i]);
}

int main()
{
  int i;
  for(i=0;i<10;i++) {x[i]=i+1;}

  for(i=0;i<10;i++) printf("%d %g\n",i,func(i));
  
  func(2) = 153;

  for(i=0;i<10;i++) printf("%d %g\n",i,x[i]);

  return 0;
}
