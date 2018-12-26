
############################################################
# Automatically created makefile for NVector.dll
############################################################

# Copying $CINTSYSDIR/MAKEINFO #############################

############################################################
# platform/linux_RH7.3_so
#  Platform dependent information for LINUX 2.4 RedHat7.3 or later
#  Build libcint.so instead of G__ci.a
#  Based on Gunay ARSLEN's contribution
############################################################

# Tools
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
CCDLLOPT    = -fPIC
LDDLLOPT    = -shared
OPTIMIZE    = -O2
LDOPT       = -lm -ldl -lncurses
SHLIB_OPT   = -Wl,-hlibcint.so
SYSMACRO    = -DG__REGEXP -DG__SHAREDLIB -DG__OSFDLL -DG__ANSI -DG__ERRORCALLBACK
OTHMACRO    = -DG__P2FCAST -DG__REDIRECTIO -DG__DETECT_NEWDEL -DG__POSIX $(CCDLLOPT) -DG__STD_EXCEPTION 
SYSIPATH    =

# libraries
MAINO	    = $(CINTSYSDIR)/main/G__cppmain.o
CINTLIB     = $(CINTSYSDIR)/libcint.so
READLINEA   = /usr/lib/libreadline.so
APIO	    = Api.o Class.o BaseCls.o Type.o DataMbr.o Method.o MethodAr.o CallFunc.o Typedf.o Apiif.o Token.o
RANLIB	    = /usr/bin/ranlib
STDLIBS     = dmystrm.o stdstrct.o
PLATFORM    = $(CINTSYSDIR)/main/G__setup.o

# source code postfix
CSRCPOST    = .c
CHDRPOST    = .h
CPPSRCPOST  = .C
CPPHDRPOST  = .h
OBJPOST     = .o
DLLPOST     = .dl










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
IPATH      = $(SYSIPATH) 
MACRO      = $(SYSMACRO)
CINTSYSDIR = /mnt/hgfs/C/cint_linux64
CINTIPATH  = -I$(CINTSYSDIR)
OBJECT     = NVector.dll
OPTION     = $(CCDLLOPT)
DLLSPEC    =
LINKSPEC   = -DG__CPPLINK_ON

# Set File names ###########################################
CIFC       =
CIFH       =
CIFO       =
CPPIFC     = G__cpp_NVector.C
CPPIFH     = G__cpp_NVector.h
CPPIFO     = G__cpp_NVector.o

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
		Fft.o \
		Lsm.o \
		Stat.o \
		VirtualGraph.o \
		cintgraph.o \
		Vectoriostream.o 

RMCPPOFILES = \
		Fft.o \
		Lsm.o \
		Stat.o \
		VirtualGraph.o \
		cintgraph.o \
		Vectoriostream.o 

CPPHEADER  = \
		Vector.h \
		Complex.h \
		Fft.h \
		Lsm.h \
		SparseVector.h \
		Histogram.h \
		Stat.h \
		Vectoriostream.h \
		VirtualGraph.h \
		cintgraph.h \
		LinkDef.h 
CPPHEADERCINT  = \
		Vector.h \
		Complex.h \
		Fft.h \
		Lsm.h \
		SparseVector.h \
		Histogram.h \
		Stat.h \
		Vectoriostream.h \
		VirtualGraph.h \
		cintgraph.h \
		LinkDef.h 

CPPSTUB    = 
CPPSTUBCINT = 

# Link Object #############################################
$(OBJECT) : $(COFILES) $(CPPOFILES) $(CIFO) $(CPPIFO)
	$(LD) $(LDDLLOPT) $(OPTIMIZE) $(IPATH) $(MACRO) $(CCOPT) -o $(OBJECT) $(COFILES) $(CIFO) $(CPPIFO) $(CPPOFILES) $(LIBS)
	$(CP) $(OBJECT) $(CINTSYSDIR)/$(OBJECT)

# Compile User C source files ##############################

# Compile User C++ source files ############################
Fft.o : Fft.cxx $(CPPHEADER)
	$(CPP) $(IPATH) $(MACRO) $(OPTIMIZE) $(OPTION) $(CCOPT) -o Fft.o -c Fft.cxx

Lsm.o : Lsm.cxx $(CPPHEADER)
	$(CPP) $(IPATH) $(MACRO) $(OPTIMIZE) $(OPTION) $(CCOPT) -o Lsm.o -c Lsm.cxx

Stat.o : Stat.cxx $(CPPHEADER)
	$(CPP) $(IPATH) $(MACRO) $(OPTIMIZE) $(OPTION) $(CCOPT) -o Stat.o -c Stat.cxx

VirtualGraph.o : VirtualGraph.cxx $(CPPHEADER)
	$(CPP) $(IPATH) $(MACRO) $(OPTIMIZE) $(OPTION) $(CCOPT) -o VirtualGraph.o -c VirtualGraph.cxx

cintgraph.o : cintgraph.cxx $(CPPHEADER)
	$(CPP) $(IPATH) $(MACRO) $(OPTIMIZE) $(OPTION) $(CCOPT) -o cintgraph.o -c cintgraph.cxx

Vectoriostream.o : Vectoriostream.cxx $(CPPHEADER)
	$(CPP) $(IPATH) $(MACRO) $(OPTIMIZE) $(OPTION) $(CCOPT) -o Vectoriostream.o -c Vectoriostream.cxx



# Compile C++ Interface routine ##########################
$(CPPIFO) : $(CPPIFC)
	$(CPP) $(CINTIPATH) $(IPATH) $(MACRO) $(OPTIMIZE) $(OPTION) $(CCOPT) -c $(CPPIFC)

# Create C++ Interface routine ###########################
$(CPPIFC) : $(CPPHEADER) $(CPPSTUB) $(CINTSYSDIR)/cint
	$(CINTSYSDIR)/cint  -w1 -zNVector -n$(CPPIFC) $(DLLSPEC) -D__MAKECINT__ -DG__MAKECINT  -c-1 -A $(IPATH) $(MACRO) $(CINTOPT) $(CPPHEADERCINT)


# Clean up #################################################
clean :
	$(RM) $(OBJECT) core $(CIFO) $(CIFC) $(CIFH) $(CPPIFO) $(CPPIFC) $(CPPIFH) $(RMCOFILES) $(RMCPPOFILES)

# re-makecint ##############################################
makecint :
	makecint -mk makelinux -dll NVector.dll -H Vector.h Complex.h Fft.h Lsm.h SparseVector.h Histogram.h Stat.h Vectoriostream.h VirtualGraph.h cintgraph.h LinkDef.h -C++ Fft.cxx Lsm.cxx Stat.cxx VirtualGraph.cxx cintgraph.cxx Vectoriostream.cxx 

