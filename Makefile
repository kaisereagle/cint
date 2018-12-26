#
# CAUTION: This Makefile is generated for specific computer
#         platform and user environment. This one may not fit your
#         environment. Execute setup script to generate correct
#         makefile.
#
CINTSYSDIR =  /cygdrive/c/cint
############################################################
# platform/cygwin51_gcc3_so
#  Platform dependent information for Cygwin_NT-5.1
#  using gcc-3.00
#  Build libcint.dll instead of G__ci.a
############################################################

# Tools
AS	    = as
DLLTOOL	    = dlltool
LDDLL 	    = ld
#LIBS	    = -lcygwin -lkernel32 -luser32 -lgdi32 -lcomdlg32

RM	    = rm -f
CP	    = cp
AR	    = ar
AROPT       = clq
KRCC        = gcc -traditional
CC          = gcc -fexceptions -Wall
CPP         = g++ -Wall 
LD          = g++
CPREP       = gcc -E -C
CPPPREP     = g++ -E -C

# Compiler and linker option
CCDLLOPT    = -DBUILDING_DLL=1
LDDLLOPT    = -shared
# gcc-3.00 has a bug in -O2. Use -O instead. -O2 may be OK for later version
OPTIMIZE    = -g
LDOPT       = -lm -ltermcap -lreadline
SYSMACRO    = -DG__REGEXP -DG__SHAREDLIB -DG__OSFDLL -DG__ANSI -DG__ERRORCALLBACK -DG__CYGWIN=51 -DG__NEWSTDHEADER
OTHMACRO    = -DG__P2FCAST -DG__REDIRECTIO -DG__DETECT_NEWDEL -DG__POSIX -DG__STD_EXCEPTION $(CCDLLPOST) -DG__GNUREADLINE
SYSIPATH    =

# libraries
MAINO	    = $(CINTSYSDIR)/main/G__cppmain.o
CINTLIB     = $(CINTSYSDIR)/libcint.dll
READLINEA   = 
APIO	    = Api.o Class.o BaseCls.o Type.o DataMbr.o Method.o MethodAr.o CallFunc.o Typedf.o Apiif.o Token.o
RANLIB	    = /usr/bin/ranlib
STDLIBS     = gcc3strm.o stdstrct.o
PLATFORM    = $(CINTSYSDIR)/main/G__setup.o

# source code postfix
CSRCPOST    = .c
CHDRPOST    = .h
CPPSRCPOST  = .C
CPPHDRPOST  = .h
OBJPOST     = .o
DLLPOST     = .dll





##########################################################
# Control default behavior of cint command line input
##########################################################
#INPUTMODE = C++
#INPUTMODE = root
INPUTMODE = cint
#INPUTMODELOCK = on
INPUTMODELOCK = off
##############################################################
# $CINTSYSDIR/Makefile.base
#
# Author                 Masaharu Goto 
# Copyright(c) 1995~2001 Masaharu Goto (cint@pcroot.cern.ch)
#
# Description:
#   This file is a base of cint makefile. There are examples
#  of platform dependent setup information under platform
#  directory. Do following to create a complete Makefile,
#
#  $ cat platform/[YourPlatform] Makefile.base > Makefile
#
##############################################################
CINT_V6     = bc_autoobj.o bc_cfunc.o bc_inst.o bc_parse.o bc_reader.o bc_type.o bc_exec.o bc_vtbl.o bc_eh.o bc_debug.o bc_assign.o

#CINTSYSDIR  = .

MACRO       = $(SYSMACRO) $(OTHMACRO)

##############################################################
# Generate ios enum value specific to the platform
##############################################################
include/iosenum.h : cint include/stdio.h makecint include/iosenum.cxx
	if test -r include/iosenum.h; \
	then \
		touch include/iosenum.h; \
	else \
		(cd include;rm -f stdfunc.dll;../cint -I. iosenum.cxx); \
	fi
	echo > done

##############################################################
# Compile makecint executable
##############################################################
makecint : tool/makecint.c
	$(CC) $(MACRO) $(OPTIMIZE) -c tool/makecint.c -o tool/makecint.o
	$(LD) $(OPTIMIZE) tool/makecint.o $(LDOPT) -o makecint
	#(RM) tool/makecint.o

#	$(CC) $(MACRO) $(OPTIMIZE) -o makecint tool/makecint.c

##############################################################
# Generate standard header files
##############################################################
include/stdio.h : include/mkincld.c
	$(CC) -I.. $(MACRO) -c -o include/mkincld.o include/mkincld.c
	$(LD) $(OPTIMIZE) include/mkincld.o $(LDOPT) -o include/mkincld
	(cd include;./mkincld)

#	$(CC) -I.. $(MACRO) -o include/mkincld include/mkincld.c
#	(cd include;./mkincld)

##############################################################
# Link cint executable
##############################################################
cint : $(CINTLIB) main/G__setup.o $(MAINO) $(READLINEA) 
	$(LD) $(OPTIMIZE) -o cint $(MAINO) $(CINTLIB) main/G__setup.o $(READLINEA) $(LDOPT)

##############################################################
# Compile precompiled library setup function which is blank
##############################################################
main/G__setup.o : main/G__setup.c G__ci.h
	$(CC) $(CLINK) $(CPPLINK) $(MACRO) $(OPTIMIZE) -o G__setup.o -c main/G__setup.c
	mv G__setup.o main/G__setup.o

##############################################################
# Compile GNU readline library. This is done only when 
# READLINEA is set to readline/libreadline.a in platform 
# dependency file
##############################################################
$(CINTSYSDIR)/readline/libreadline.a : readline/*.c readline/*.h
	(cd readline ; make)

##############################################################
# Compile cint interpreter core library
##############################################################
$(CINTSYSDIR)/src/G__ci.a : G__ci.h src/*.c src/*.cxx *.h src/*.h $(READLINEA)
	(cd src ; make CINTSYSDIR=$(CINTSYSDIR) )

$(CINTSYSDIR)/libcint.so : G__ci.h src/*.c src/*.cxx *.h src/*.h $(READLINEA)
	(cd src ; make CINTSYSDIR=$(CINTSYSDIR) $(CINTSYSDIR)/libcint.so)

$(CINTSYSDIR)/libcint.sl : G__ci.h src/*.c src/*.cxx *.h src/*.h $(READLINEA)
	(cd src ; make CINTSYSDIR=$(CINTSYSDIR) $(CINTSYSDIR)/libcint.sl)

$(CINTSYSDIR)/libcint.dll : G__ci.h src/*.c src/*.cxx *.h src/*.h $(READLINEA)
	(cd src ; make CINTSYSDIR=$(CINTSYSDIR) $(CINTSYSDIR)/libcint.dll)

##############################################################
# Compile cint main function. 
##############################################################
$(CINTSYSDIR)/main/G__cppmain.o : main/cppmain.cxx
	$(CPP) $(OPTIMIZE) $(SYSIPATH) -o $(CINTSYSDIR)/main/G__cppmain.o -c main/cppmain.cxx

$(CINTSYSDIR)/main/G__main.o : main/G__main.c
	$(CC) $(OPTIMIZE) -o $(CINTSYSDIR)/main/G__main.o -c main/G__main.c

##############################################################
# Cleanup
##############################################################
clean :
	$(RM) main/*.o cint done core include/mkincld include/stdio.h makecint include/iosenum.h ; (cd src; make clean) ; (cd readline; make clean)

cleanbin :
	$(RM) main/*.o src/*.o $(CINTLIB) $(CINT_V6)
