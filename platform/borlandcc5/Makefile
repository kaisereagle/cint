# platform/borlandcc5/Makefile 
#  This makefile is specifically tailored for Borland C++ Compiler version 5.5
#  Set CINTSYSDIR and BCCDIR appropiately. Run platform\borlandcc5\setup.bat 
#  for installation. Do not directory use 'make' in this directory. It does
#  not work.

# Directory where you install Cint
CINTSYSDIR =  c:\cint

# Directory where Borland C++ Compiler 5.5 exists
BCCDIR      = c:\Borland\BCC55

############################################################
# platform/borlandcc5
#  Platform dependent information for Borland C++ Compiler 5.5
############################################################

BCCINC      = -I$(BCCDIR)\Include
BCCLIB      = -L$(BCCDIR)\Lib

# Tools
RM	    = del 
CP	    = copy
KRCC        = bcc32 $(BCCINC)
CC          = bcc32 $(BCCINC)
CPP         = bcc32 -P $(BCCINC)
LD          = bcc32 $(BCCINC) $(BCCLIB)
CPREP       = cpp32 -Sr -Sc $(BCCINC)
CPPPREP     = cpp32 -Sr -Sc $(BCCINC)

# Compiler and linker option
CCDLLOPT    = 
LDDLLOPT    = -WD
OPTIMIZE    = -O2
LDOPT       = 
SHLIB_OPT   = 
SYSMACRO    = -DG__BORLANDCC5 -DG__NEWSTDHEADER
OTHMACRO    = 
SYSIPATH    =

# libraries
MAINO	    = $(CINTSYSDIR)\main\G__cppmain.obj
CINTLIB     = $(CINTSYSDIR)\libcint.lib
READLINEA   = 
APIO	    = Api.obj Class.obj BaseCls.obj Type.obj DataMbr.obj Method.obj MethodAr.obj CallFunc.obj Typedf.obj Apiif.obj Token.obj
RANLIB	    = 
STDLIBS     = cbstrm.obj stdstrct.obj
PLATFORM    = winnt.obj

# source code postfix
CSRCPOST    = .c
CHDRPOST    = .h
CPPSRCPOST  = .cpp
CPPHDRPOST  = .h
OBJPOST     = .obj
DLLPOST     = .dll








##############################################################
# $CINTSYSDIR/Makefile.base
#
# Author                 Masaharu Goto 
# Copyright(c) 1995~2001 Masaharu Goto (MXJ02154@niftyserve.or.jp)
#
# Description:
#   This file is a base of cint makefile. There are examples
#  of platform dependent setup information under platform
#  directory. Do following to create a complete Makefile,
#
#  $ cat platform/[YourPlatform] Makefile.base > Makefile
#
##############################################################

#CINTSYSDIR  = .

MACRO       = $(SYSMACRO) $(OTHMACRO)


##############################################################
# Generate ios enum value specific to the platform
##############################################################
done : ifdef.exe
	echo > done 

##############################################################
# Compile ifdef.exe tool
##############################################################
ifdef.exe : lib\done
	cd tool\ifdef
	make.exe -f Makefile.bcc5 BCCINC=$(BCCINC) BCCLIB=$(BCCLIB)
	move ifdef.exe ..\..\ifdef.exe
	cd ..\..

##############################################################
# Generate ios enum value specific to the platform
#  not needed for BCC55
##############################################################
#include\iosenum.h : cint.exe libcint.dll include\stdio.h include\iosenum.cxx 
#	cd include
#	del stdfunc.dll
#	..\cint -I. iosenum.cxx
#	mail MXJ02154@niftyserve.or.jp < MAKEINFO

##############################################################
# Compile makecint executable
##############################################################
lib\done : cint.exe libcint.dll makecint.exe
	cd lib
	make -f Makefile
	cd ..

##############################################################
# Compile makecint executable
##############################################################
makecint.exe : platform\borlandcc5\makecint.c
	cd platform\borlandcc5
	make -f makemakecint BCCINC=$(BCCINC) BCCLIB=$(BCCLIB)
	cd ..\..

##############################################################
# Generate standard header files
##############################################################
include\stdio.h : include\mkincld.c
	$(CC) -I.. $(MACRO) -einclude\mkincld.exe include\mkincld.c
	(cd include;.\mkincld)

##############################################################
# Link cint executable
##############################################################
cint.exe : $(CINTLIB) $(MAINO) $(READLINEA) $(CINTSYSDIR)\libcint.lib
	$(LD) $(OPTIMIZE) $(MACRO) -ecint.exe libcint.lib main\G__cppmain.obj $(READLINEA) $(LDOPT)

##############################################################
# Compile precompiled library setup function which is blank
##############################################################
main\G__setup.obj : main\G__setup.c G__ci.h
	$(CC) $(CLINK) $(CPPLINK) $(MACRO) $(OPTIMIZE) -o G__setup.obj -c main\G__setup.c
	mv G__setup.obj main\G__setup.obj

libcint.dll : G__ci.h
	cd src
	make
	cd ..


##############################################################
# Compile GNU readline library. This is done only when 
# READLINEA is set to readline\libreadline.a in platform 
# dependency file
##############################################################
#$(CINTSYSDIR)\readline\libreadline.a : readline\*.c readline\*.h
#	(cd readline ; make)

##############################################################
# Compile cint interpreter core library
##############################################################
$(CINTSYSDIR)\src\G__ci.a : G__ci.h src\*.c src\*.cxx *.h src\*.h $(READLINEA)
	(cd src ; make CINTSYSDIR=$(CINTSYSDIR) )

$(CINTSYSDIR)\libcint.lib : G__ci.h $(READLINEA)
	cd src
	make -f Makefile BCCINC=$(BCCINC) BCCLIB=$(BCCLIB)
	cd ..
#	(cd src ; make -f Makefile)

$(CINTSYSDIR)\libcint.so : G__ci.h src\*.c src\*.cxx *.h src\*.h $(READLINEA)
	(cd src ; make CINTSYSDIR=$(CINTSYSDIR) $(CINTSYSDIR)\libcint.so)

$(CINTSYSDIR)\libcint.sl : G__ci.h src\*.c src\*.cxx *.h src\*.h $(READLINEA)
	(cd src ; make CINTSYSDIR=$(CINTSYSDIR) $(CINTSYSDIR)\libcint.sl)

##############################################################
# Compile cint main function. 
##############################################################
$(CINTSYSDIR)\main\G__cppmain.obj : main\cppmain.cxx
	$(CPP) $(OPTIMIZE) $(SYSIPATH) $(MACRO) -omain\G__cppmain.obj -c main\cppmain.cxx

$(CINTSYSDIR)\main\G__main.obj : main\G__main.c
	$(CC) $(OPTIMIZE) -o main\G__main.obj -c main\G__main.c

##############################################################
# Cleanup
##############################################################
clean :
	$(RM) cint.exe 
	$(RM) libcint.dll
	$(RM) libcint.lib
	$(RM) main\*.obj 
	$(RM) src\*.obj 
	$(RM) done 
	$(RM) core
	$(RM) makecint.exe 

#	$(RM) include\mkincld 
#	$(RM) include\stdio.h 
#	$(RM) include\iosenum.h 
#	(cd src; make -f Makefile.bc55 clean)

cleanbin :
	$(RM) src\*.obj 
	$(RM) $(CINTLIB) 
