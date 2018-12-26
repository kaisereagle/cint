/****************************************************************
* cintsock.c
*  TCP/IP connection library 
****************************************************************/

#include "cintsock.h"

#ifndef _WIN32

int closesocket(SOCKET destSocket) {
  int status;
  status = shutdown(destSocket,2);
  status = close(destSocket);
  return(status);
}

int WSACleanup() { return(0); }
int WSAStartup(WORD wVersionRequested,LPWSADATA lpWSAData) { return(0); }
int MAKEWORD(int a,int b) {return(0);}

#endif
