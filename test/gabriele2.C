
#include <stdio.h>
#include <string>
#include <stdlib.h>
void drift_velocity(double x, double y, double& vx, double& vy) {
  double ex, ey, mu;
  const double alpha=2, beta=0, v0=1;
  if (x==0 && y==0) 
    {
    printf("x=y=0\n");
    vx = 0;
    vy = 0;
    return;
    }
  const int model=4;
  switch (model) 
    {
      //Radial Field
    case 1: 
      ex = x/(x*x + y*y);
      ey = y/(x*x + y*y);
      mu = 5; 
      vx = mu * ex; 
      vy = mu * ey;
      break;
    
    case 2:
      vx = v0 + alpha*x;
      vy = y * beta;
      break;
      
      //Constant Field
    case 3:
      ex = 1;
      ey = 0;
      mu = 5.0; 
      vx = mu * ex; 
      vy = mu * ey;
      break;
      // Gem Like-Field
    case 4:
      mu = 1.0; 
      ex = -1.0*(
	-0.08635411*(x+0.1 )/(pow(x+0.1 ,2)+pow(y-1,2))
	-0.066419*(x     )/(pow(x     ,2)+pow(y-1,2))
	-0.086354*(x-0.1 )/(pow(x-0.1 ,2)+pow(y-1,2))
		    
	-0.012136*(x+0.08)/(pow(x+0.08,2)+pow(y  ,2))
	//-0.009957*(x+0.06)/(pow(x+0.06,2)+pow(y  ,2))
#if 1
	-0.9*(x-0.5)/(2)
	-0.8*(x-0.6)/(2)
#else
	-0.9012341351231*(x-0.51234123531)/(2)
	-0.8012341351231*(x-0.61234123531)/(2)
#endif
	//-0.9*(x-0.6)/(pow(x-0.6,2)+pow(y,2))
	//-0.009957*(x-0.06)/(pow(x-0.06,2)+pow(y  ,2))
	//-0.012136*(x-0.08)/(pow(x-0.08,2)+pow(y  ,2))
	
	//+0.102912*(x+0.1 )/(pow(x+0.1 ,2)+pow(y+1,2))
	//+0.078810*(x     )/(pow(x,     2)+pow(y+1,2))
	+0.102912*(x-0.1 )/(pow(x-0.1 ,2)+pow(y+1,2))
	  );

      ey = -1.0*(
	-0.086354*(x+0.1 )/(pow(x+0.1 ,2)+pow(y-1,2))
	+0.078810*(x     )/(pow(x,     2)+pow(y+1,2))
	  );
      vx = -mu * ex; 
      vy = -mu * ey;
      break;
    }
}

void gabriele() {
  double vx, vy, x, y, dt=0.1;
  x = -0.01;
  y = 0.1;
  for (int jt=0; jt<10; jt++) {
    drift_velocity(x,y,vx,vy);
    x += vx * dt;
    y += vy * dt;
    printf("- \t %d \t %g \t %g \t v: %g %g \n", jt, x, y, vx, vy);
  }
  printf("\n");
  x = +0.01;
  y = 0.1;
  for (int jt=0; jt<10; jt++) {
    drift_velocity(x,y,vx,vy);
    x += vx * dt;
    y += vy * dt;
    printf("+ \t %d \t %g \t %g \t v: %g %g \n", jt, x, y, vx, vy);
  }
}

int main() {
  gabriele() ;
  return 0;
}
