
#include <stdio.h>
#include <string.h>

// badinit.c
char *system = "WinNT";
//______________________________________________________________________________
void ls(char *path=0)
{
#ifdef __CINT__
   char s[256] = (!strcmp(system, "WinNT")) ? "dir /w " : "ls ";
#else
   char s[256] = "dir /w ";
#endif
   if (path) strcat(s,path);
   // gSystem.Exec(s);
   printf("%s\n",s);
}

int main() {
  for(int i=0;i<2;i++) ls();
  return 0;
}




