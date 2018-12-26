
//#include <stdio.h>
//#include <string>
//#include <stdlib.h>
void drift_velocity(double x, double y, double& vx, double& vy) {
  double ex, ey, mu;
      ex = -1.0*(
	-0.086354*(x+0.1 )/(pow(x+0.1 ,2)+pow(y-1,2))
	-0.066419*(x     )/(pow(x     ,2)+pow(y-1,2))
	-0.086354*(x-0.1 )/(pow(x-0.1 ,2)+pow(y-1,2))
		    
	-0.012136*(x+0.08)/(pow(x+0.08,2)+pow(y  ,2))
	-0.009957*(x+0.06)/(pow(x+0.06,2)+pow(y  ,2))
	-0.009957*(x-0.06)/(pow(x-0.06,2)+pow(y  ,2))
	-0.012136*(x-0.08)/(pow(x-0.08,2)+pow(y  ,2))
	
	+0.102912*(x+0.1 )/(pow(x+0.1 ,2)+pow(y+1,2))
	+0.078810*(x     )/(pow(x,     2)+pow(y+1,2))
	+0.102912*(x-0.1 )/(pow(x-0.1 ,2)+pow(y+1,2))
	  );
}

void gabriele() {
  double x=1,y=2,vx=3,vy=4;
  for (int i=0; i<2; i++) {
    drift_velocity(x,y,vx,vy);
  }
}

int main() {
  gabriele() ;
  return 0;
}

