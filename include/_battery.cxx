
#if defined(G__WIN32) 
#include <windows.h>
#endif

main() {
#if defined(G__WIN32) 
  HANDLE hProcess = GetCurrentProcess();
  SetPriorityClass(hProcess,IDLE_PRIORITY_CLASS);
#endif
  int i=0;
  for(;;) if(++i%10000000==0) printf("i=%d\n",i);
}
