############################################################
# Automatically created makefile for UserMain
############################################################

# Copying $CINTSYSDIR/MAKEINFO #############################

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

# End of $CINTSYSDIR/MAKEINFO ##############################

# Set variables ############################################
IPATH      = $(SYSIPATH) \
		-I/cygdrive/c/cint 
MACRO      = $(SYSMACRO)
CINTSYSDIR = /cygdrive/c/cint
CINTIPATH  = -I$(CINTSYSDIR)
OBJECT     = UserMain
OPTION     =
DLLSPEC    =
LINKSPEC   = -DG__CPPLINK_ON

# Set File names ###########################################
CIFC       =
CIFH       =
CIFO       =
CPPIFC     = G__cpp_UserMain.C
CPPIFH     = G__cpp_UserMain.h
CPPIFO     = G__cpp_UserMain.o

LIBS       = 

CCOPT      = 

CINTOPT      = 

COFILES    = 

RMCOFILES  = 

CHEADER    = 
CHEADERCINT = 

CSTUB      = 
CSTUBCINT  = 

CPPOFILES  = \
		UserMain.o 

RMCPPOFILES = \
		UserMain.o 

CPPHEADER  = \
		UserMain.h 
CPPHEADERCINT  = \
		UserMain.h 

CPPSTUB    = 
CPPSTUBCINT = 

# Link Object #############################################
$(OBJECT) : $(CINTLIB) $(READLINEA) $(COFILES) $(CPPOFILES) $(CIFO) $(CPPIFO) 
	$(LD) $(OPTIMIZE) $(IPATH) $(MACRO) $(CCOPT) -o $(OBJECT) $(CIFO) $(CPPIFO) $(COFILES) $(CPPOFILES) $(CINTLIB) $(READLINEA) $(LIBS) $(LDOPT)

# Compile User C source files ##############################

# Compile User C++ source files ############################
UserMain.o : UserMain.cxx $(CPPHEADER)
	$(CPP) $(IPATH) $(MACRO) $(OPTIMIZE) $(OPTION) $(CCOPT) -o UserMain.o -c UserMain.cxx



# Compile main function  #################################
G__main.o : G__main.cxx
	$(CPP) $(LINKSPEC) $(CINTIPATH) $(OPTIMIZE) $(OPTION) -o G__main.o -c G__main.cxx

# Compile C++ Interface routine ##########################
$(CPPIFO) : $(CPPIFC)
	$(CPP) $(CINTIPATH) $(IPATH) $(MACRO) $(OPTIMIZE) $(OPTION) $(CCOPT) -c $(CPPIFC)

# Create C++ Interface routine ###########################
$(CPPIFC) : $(CPPHEADER) $(CPPSTUB) $(CINTSYSDIR)/cint
	$(CINTSYSDIR)/cint  -w0 -zUserMain -n$(CPPIFC) $(DLLSPEC) -D__MAKECINT__ -DG__MAKECINT  -c-1 -A $(IPATH) $(MACRO) $(CINTOPT) $(CPPHEADERCINT)


# Clean up #################################################
clean :
	$(RM) $(OBJECT) core $(CIFO) $(CIFC) $(CIFH) $(CPPIFO) $(CPPIFC) $(CPPIFH) $(RMCOFILES) $(RMCPPOFILES) G__main.o

# re-makecint ##############################################
makecint :
	makecint -mk Makefile -m -I/cygdrive/c/cint -o UserMain -H UserMain.h -C++ UserMain.cxx 

