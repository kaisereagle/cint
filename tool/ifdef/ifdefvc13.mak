##########################################################################
# Makefile for ifdef resolver
#
#  Author   : Masaharu Goto
#  Date     : 8 Feb 1994
#  Date     : 8 Jan 2001
#
##########################################################################
CPP = cl
CC  = cl

###############################################################
IPATH       = /I "C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\include"
LIB         =  \
   "C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\lib\msvcprt.lib" \
   "C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\lib\MSVCRT.lib" \
   "C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\lib\OLDNAMES.lib" \
   "C:\Program Files (x86)\Microsoft SDKs\Windows\v7.1A\Lib\Uuid.lib" \
   "C:\Program Files (x86)\Microsoft SDKs\Windows\v7.1A\Lib\Kernel32.lib"

###############################################################

OPTIMIZE    = /O2 /D "NODEBUG"
MACRO    =   
OPTION 	   = /nologo /MD /W3 /EHsc /D "WIN32" /D "_WIN32" /D "_WINDOWS" /D "__SC__" /D "__VC13__"
###############################################################

ifdef.exe : ifdef.obj get.obj winnt.obj
	$(CPP) $(OPTIMIZE) $(OPTION) $(IPATH) $(LIB) -o ifdef.exe ifdef.obj get.obj winnt.obj

ifdef.obj: ifdef.cxx
	$(CPP) $(OPTIMIZE) $(OPTION) $(IPATH) -DG__GET -c -o ifdef.obj ifdef.cxx

get.obj: get.c
	$(CC) $(OPTIMIZE) $(OPTION) $(IPATH) -DG__IFDEF -c -o get.obj get.c

winnt.obj: ../../src/winnt.c
	$(CC) $(OPTIMIZE) $(OPTION) $(IPATH) -c -o winnt.obj ../../src/winnt.c


clean:
	del *.obj
	del ifdef.exe

