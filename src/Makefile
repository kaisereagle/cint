CINTSYSDIR =  /mnt/hgfs/C/cint_linux64
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
#READLINEA   = /usr/lib64/libreadline.so
READLINEA   = -lreadline
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
##############################################################
# $CINTSYSDIR/src/Makefile.base
#
# Author                 Masaharu Goto 
# Copyright(c) 1995~2003 Masaharu Goto (MXJ02154@niftyserve$(OBJPOST)r.jp)
#
# Description:
#   This file is a base of cint library makefile. There are examples
#  of platform dependent setup information under platform
#  directory. Do following to create a complete Makefile,
#
#  $ cat ../platform/[YourPlatform] Makefile.base > Makefile
#
##############################################################
CINT_V6     = bc_autoobj$(OBJPOST) bc_cfunc$(OBJPOST) bc_inst$(OBJPOST) bc_parse$(OBJPOST) bc_reader$(OBJPOST) bc_type$(OBJPOST) bc_exec$(OBJPOST) bc_vtbl$(OBJPOST) bc_eh$(OBJPOST) bc_debug$(OBJPOST) bc_assign$(OBJPOST)

#CINTSYSDIR   = ..
IPATH    = -I.. $(SYSIPATH)
MACRO    = $(SYSMACRO) $(OTHMACRO) 

############################################################################
# common variable definition 
############################################################################

SECURITYH =     security.h
DLLREVH =       dllrev.h

HEADERS =       common.h global.h fproto.h memtest.h sunos.h ../G__ci.h 

OBJECTS = 	$(PLATFORM) \
		$(STDLIBS) \
		$(APIO) \
		$(CINT_V6) \
		auxu$(OBJPOST) \
		cast$(OBJPOST) \
		debug$(OBJPOST) \
		decl$(OBJPOST) \
		disp$(OBJPOST) \
		dump$(OBJPOST) \
		end$(OBJPOST) \
		error$(OBJPOST) \
		expr$(OBJPOST) \
		fread$(OBJPOST) \
		func$(OBJPOST) \
		gcoll$(OBJPOST) \
		global1$(OBJPOST) \
		global2$(OBJPOST) \
		g__cfunc$(OBJPOST) \
		ifunc$(OBJPOST) \
		inherit$(OBJPOST) \
		init$(OBJPOST) \
		input$(OBJPOST) \
		intrpt$(OBJPOST) \
		loadfile$(OBJPOST) \
		macro$(OBJPOST) \
		malloc$(OBJPOST) \
		memtest$(OBJPOST) \
		new$(OBJPOST) \
		newlink$(OBJPOST) \
		oldlink$(OBJPOST) \
		opr$(OBJPOST) \
		parse$(OBJPOST) \
		pause$(OBJPOST) \
		pcode$(OBJPOST) \
		pragma$(OBJPOST) \
		quote$(OBJPOST) \
		scrupto$(OBJPOST) \
		shl$(OBJPOST) \
		sizeof$(OBJPOST) \
		struct$(OBJPOST) \
		stub$(OBJPOST) \
		tmplt$(OBJPOST) \
		typedef$(OBJPOST) \
		val2a$(OBJPOST) \
		value$(OBJPOST) \
		var$(OBJPOST)

# Cint core as static library
$(CINTLIB) : $(OBJECTS)
	if test -r $(CINTLIB); then rm $(CINTLIB);fi
	$(AR) $(AROPT) $@ $(OBJECTS);
	-if [ -f "$(RANLIB)" ]; then $(RANLIB) $(CINTLIB); fi

# Cint core as shared library on Linux
$(CINTSYSDIR)/libcint.so : $(OBJECTS)
	$(LD) $(LDDLLOPT) $(SHLIB_OPT) -o $@ $(OBJECTS)

# Cint core as shared library on HP-UX
$(CINTSYSDIR)/libcint.sl : $(OBJECTS)
	$(LD) $(LDDLLOPT) $(SHLIB_OPT) -o $@ $(OBJECTS)

# Cint core as shared library on Cygwin
$(CINTSYSDIR)/libcint.dll : $(OBJECTS)
	$(LD) $(LDDLLOPT) $(SHLIB_OPT) -o $@ $(OBJECTS) $(LDOPT)
	$(DLLTOOL) --as=$(AS) --dllname $@ --def libcint.def --output-lib $(CINTSYSDIR)/libcint.lib

#	$(LDDLL) -s --base-file libcint.base $(LDDLLOPT) $(SHLIB_OPT) --dll -o $@ $(OBJECTS) $(LIBS) -e G__cygwin_dll_entry
#	$(DLLTOOL) --as=$(AS) --dllname $(OBJECT) --def libcint.def --base-file libcint.base --output-exp libcint.exp
#	$(LDDLL) -s --base-file libcint.base libcint.exp $(LDDLLOPT) $(SHLIB_OPT) --dll -o $@ $(OBJECTS) $(LIBS) -e G__cygwin_dll_entry
#	$(DLLTOOL) --as=$(AS) --dllname $(OBJECT) --def libcint.def --base-file libcint.base --output-exp libcint.exp
#	$(LDDLL) -s libcint.exp $(LDDLLOPT) $(SHLIB_OPT) --dll -o $@ $(OBJECTS) $(LIBS) -e G__cygwin_dll_entry
#	$(DLLTOOL) --as=$(AS) --dllname $@ --def libcint.def --output-lib $(CINTSYSDIR)/libcint.lib

############################################################################
# iostream library
############################################################################
# Common for many old C++ compilers
libstrm$(OBJPOST) : libstrm.cxx libstrm.h $(HEADERS) ../lib/stream/iostrm.h
	$(CPP) -I../lib/stream $(IPATH) $(MACRO) $(OPTIMIZE) -c libstrm.cxx

# Microsoft Visual C++ compiler
vcstrm$(OBJPOST) : vcstrm.cxx vcstrm.h $(HEADERS) ../lib/vcstream/iostrm.h
	$(CPP) -I../lib/vcstream $(IPATH) $(MACRO) $(OPTIMIZE) -c vcstrm.cxx

# QNX Dinkum STL
vc7strm$(OBJPOST) : vc7strm.cxx vc7strm.h $(HEADERS) ../lib/vc7strm/iostrm.h
	$(CPP) -I../lib/vc7strm $(IPATH) $(MACRO) $(OPTIMIZE) -c vc7strm.cxx

# Borland C++ compiler
bcstrm$(OBJPOST) : bcstrm.cxx bcstrm.h $(HEADERS) ../lib/bcsteam/iostrm.h
	$(CPP) -I../lib/bcstream $(IPATH) $(MACRO) $(OPTIMIZE) -c bcstrm.cxx

# Borland C++ Builder
cbstrm$(OBJPOST) : cbstrm.cpp cbstrm.h $(HEADERS) ../lib/cbsteam/iostrm.h
	$(CPP) -I../lib/cbstream $(IPATH) $(MACRO) $(OPTIMIZE) -c cbstrm.cpp

# KAI C++ compiler
kccstrm$(OBJPOST) : kccstrm.cxx kccstrm.h $(HEADERS)
	$(CPP) -I../lib/kccstream $(IPATH) $(MACRO) $(OPTIMIZE) -c kccstrm.cxx

# Sun CC5 compiler
sunstrm$(OBJPOST) : sunstrm.cxx sunstrm.h $(HEADERS) ../lib/snstream/iostrm.h
	$(CPP) -I../lib/snstream $(IPATH) $(MACRO) $(OPTIMIZE) -c sunstrm.cxx

sun5strm$(OBJPOST) : sun5strm.cxx sunstrm.h $(HEADERS) ../lib/snstream/iostrm.h
	$(CPP) -I../lib/snstream $(IPATH) $(MACRO) $(OPTIMIZE) -c sun5strm.cxx

# GNU C++ compiler ver 3.xx
gcc3strm$(OBJPOST) : gcc3strm.cxx gcc3strm.h $(HEADERS) ../lib/gcc3strm/iostrm.h
	$(CPP) -I../lib/gcc3strm $(IPATH) $(MACRO) $(OPTIMIZE) -c gcc3strm.cxx

# Intel C++ compiler
iccstrm$(OBJPOST) : iccstrm.cxx iccstrm.h $(HEADERS) ../lib/gcc3strm/iostrm.h
	$(CPP) -I../lib/iccstrm $(IPATH) $(MACRO) $(OPTIMIZE) -c iccstrm.cxx

# Use interpreted iostream
fakestrm$(OBJPOST) : fakestrm.cxx $(HEADERS)
	$(CPP) $(IPATH) $(MACRO) $(OPTIMIZE) -c fakestrm.cxx

# C compiler only
dmystrm$(OBJPOST) : dmystrm.c $(HEADERS)
	$(CC) $(IPATH) $(MACRO) $(OPTIMIZE) -c dmystrm.c

# For all platform switching automatically , experimental
allstrm$(OBJPOST) : allstrm.cxx $(HEADERS)
	$(CPP) $(IPATH) $(MACRO) $(OPTIMIZE) -c allstrm.cxx

############################################################################
# SunOS 4.1 specific source
############################################################################
sunos$(OBJPOST) : sunos.c $(HEADERS)
	$(CC) $(IPATH) $(MACRO) $(OPTIMIZE) -DNONANSI -c sunos.c

############################################################################
# NewsOS specific source
############################################################################
newsos$(OBJPOST) : newsos.c $(HEADERS)
	$(CC) $(IPATH) $(MACRO) $(OPTIMIZE) -DNONANSI -c newsos.c

############################################################################
# Windows-NT specific source
############################################################################
winnt$(OBJPOST) : winnt.c $(HEADERS)
	$(CC) $(IPATH) $(MACRO) $(OPTIMIZE) -c winnt.c

############################################################################
# input.c depends on GNU readline library
############################################################################
input$(OBJPOST) : input.c $(HEADERS)
	  $(CC) $(IPATH) $(MACRO) $(OPTIMIZE) -DG__GNUREADLINE -c input.c ;\

############################################################################
# lconv, div_t, ldiv_t, tm struct
############################################################################
stdstrct$(OBJPOST) : stdstrct.c stdstrct.h $(HEADERS)
	$(CC) -I../lib/stdstrct $(IPATH) $(MACRO) $(OPTIMIZE) -c stdstrct.c

dmystrct$(OBJPOST) : dmystrct.c $(HEADERS)
	$(CC) $(IPATH) $(MACRO) $(OPTIMIZE) -c dmystrct.c

############################################################################
# To install cint core as shared library
############################################################################
$(CINTSYSDIR)/main/G__setup$(OBJPOST) : $(CINTSYSDIR)/main/G__setup.c
	$(CC) $(IPATH) $(MACRO) $(OPTIMIZE) -c $< -o $@

############################################################################
# Common sources
############################################################################
auxu$(OBJPOST) : auxu.c $(HEADERS)
	$(CC) $(IPATH) $(MACRO) $(OPTIMIZE) -c auxu.c

cast$(OBJPOST) : cast.c $(HEADERS) $(SECURITYH)
	$(CC) $(IPATH) $(MACRO) $(OPTIMIZE) -c cast.c

debug$(OBJPOST) : debug.c $(HEADERS)
	$(CC) $(IPATH) $(MACRO) $(OPTIMIZE) -c debug.c

decl$(OBJPOST) : decl.c $(HEADERS)
	$(CC) $(IPATH) $(MACRO) $(OPTIMIZE) -c decl.c

disp$(OBJPOST) : disp.c $(HEADERS)
	$(CC) $(IPATH) $(MACRO) $(OPTIMIZE) -c disp.c

dmyinit$(OBJPOST) : dmyinit.c $(HEADERS)
	$(CC) $(IPATH) $(MACRO) $(OPTIMIZE) -c dmyinit.c

dump$(OBJPOST) : dump.c $(HEADERS)
	$(CC) $(IPATH) $(MACRO) $(OPTIMIZE) -c dump.c

end$(OBJPOST) : end.c $(HEADERS) $(SECURITYH)
	$(CC) $(IPATH) $(MACRO) $(OPTIMIZE) -c end.c

error$(OBJPOST) : error.c $(HEADERS) $(SECURITYH)
	$(CC) $(IPATH) $(MACRO) $(OPTIMIZE) -c error.c

expr$(OBJPOST) : expr.c $(HEADERS)
	$(CC) $(IPATH) $(MACRO) $(OPTIMIZE) -c expr.c

fread$(OBJPOST) : fread.c $(HEADERS)
	$(CC) $(IPATH) $(MACRO) $(OPTIMIZE) -c fread.c

func$(OBJPOST) : func.c $(HEADERS) $(SECURITYH)
	$(CC) $(IPATH) $(MACRO) $(OPTIMIZE) -c func.c

gcoll$(OBJPOST) : gcoll.c $(HEADERS) $(SECURITYH)
	$(CC) $(IPATH) $(MACRO) $(OPTIMIZE) -c gcoll.c

global1$(OBJPOST) : global1.c $(HEADERS)
	$(CC) $(IPATH) $(MACRO) $(OPTIMIZE) -c global1.c

global2$(OBJPOST) : global2.c $(HEADERS) $(SECURITYH)
	$(CC) $(IPATH) $(MACRO) $(OPTIMIZE) -c global2.c

g__cfunc$(OBJPOST) : g__cfunc.c $(HEADERS) $(SECURITYH)
	$(CC) $(IPATH) $(MACRO) $(OPTIMIZE) -c g__cfunc.c

ifunc$(OBJPOST) : ifunc.c $(HEADERS) $(SECURITYH)
	$(CC) $(IPATH) $(MACRO) $(OPTIMIZE) -c ifunc.c

inherit$(OBJPOST) : inherit.c $(HEADERS)
	$(CC) $(IPATH) $(MACRO) $(OPTIMIZE) -c inherit.c

init$(OBJPOST) : init.c $(HEADERS) $(SECURITYH)
	$(CC) $(IPATH) $(MACRO) $(OPTIMIZE) -c init.c

intrpt$(OBJPOST) : intrpt.c $(HEADERS)
	$(CC) $(IPATH) $(MACRO) $(OPTIMIZE) -c intrpt.c

loadfile$(OBJPOST) : loadfile.c $(HEADERS) $(SECURITYH)
	$(CC) $(IPATH) $(MACRO) $(OPTIMIZE) -c loadfile.c

macro$(OBJPOST) : macro.c $(HEADERS)
	$(CC) $(IPATH) $(MACRO) $(OPTIMIZE) -c macro.c

malloc$(OBJPOST) : malloc.c $(HEADERS)
	$(CC) $(IPATH) $(MACRO) $(OPTIMIZE) -c malloc.c

memtest$(OBJPOST) : memtest.c $(HEADERS)
	$(CC) $(IPATH) $(MACRO) $(OPTIMIZE) -c memtest.c

new$(OBJPOST) : new.c $(HEADERS) $(SECURITYH) $(SECURITYH)
	$(CC) $(IPATH) $(MACRO) $(OPTIMIZE) -c new.c

newlink$(OBJPOST) : newlink.c $(HEADERS) $(DLLREVH)
	$(CC) $(IPATH) $(MACRO) $(OPTIMIZE) -c newlink.c

oldlink$(OBJPOST) : oldlink.c $(HEADERS)
	$(CC) $(IPATH) $(MACRO) $(OPTIMIZE) -c oldlink.c

opr$(OBJPOST) : opr.c $(HEADERS) $(SECURITYH)
	$(CC) $(IPATH) $(MACRO) $(OPTIMIZE) -c opr.c

parse$(OBJPOST) : parse.c $(HEADERS) $(SECURITYH)
	$(CC) $(IPATH) $(MACRO) $(OPTIMIZE) -c parse.c

pause$(OBJPOST) : pause.c $(HEADERS) $(SECURITYH)
	$(CC) $(IPATH) $(MACRO) $(OPTIMIZE) -c pause.c

pcode$(OBJPOST) : pcode.c $(HEADERS) bc_exec_asm.h
	$(CC) $(IPATH) $(MACRO) $(OPTIMIZE) -c pcode.c

pragma$(OBJPOST) : pragma.c $(HEADERS) $(SECURITYH)
	$(CC) $(IPATH) $(MACRO) $(OPTIMIZE) -c pragma.c

quote$(OBJPOST) : quote.c $(HEADERS)
	$(CC) $(IPATH) $(MACRO) $(OPTIMIZE) -c quote.c

scrupto$(OBJPOST) : scrupto.c $(HEADERS) $(SECURITYH)
	$(CC) $(IPATH) $(MACRO) $(OPTIMIZE) -c scrupto.c

sizeof$(OBJPOST) : sizeof.c $(HEADERS)
	$(CC) $(IPATH) $(MACRO) $(OPTIMIZE) -c sizeof.c

shl$(OBJPOST) : shl.c $(HEADERS) $(DLLREVH)
	$(CC) $(IPATH) $(MACRO) $(OPTIMIZE) -c shl.c

struct$(OBJPOST) : struct.c $(HEADERS)
	$(CC) $(IPATH) $(MACRO) $(OPTIMIZE) -c struct.c

stub$(OBJPOST) : stub.c $(HEADERS)
	$(CC) $(IPATH) $(MACRO) $(OPTIMIZE) -c stub.c

tmplt$(OBJPOST) : tmplt.c $(HEADERS)
	$(CC) $(IPATH) $(MACRO) $(OPTIMIZE) -c tmplt.c

typedef$(OBJPOST) : typedef.c $(HEADERS)
	$(CC) $(IPATH) $(MACRO) $(OPTIMIZE) -c typedef.c

val2a$(OBJPOST) : val2a.c $(HEADERS)
	$(CC) $(IPATH) $(MACRO) $(OPTIMIZE) -c val2a.c

value$(OBJPOST) : value.c $(HEADERS)
	$(CC) $(IPATH) $(MACRO) $(OPTIMIZE) -c value.c

var$(OBJPOST) : var.c $(HEADERS) $(SECURITYH)
	$(CC) $(IPATH) $(MACRO) $(OPTIMIZE) -c var.c

############################################################################
# ERTTI API
############################################################################

APIHEADER = Api.h \
	  Class.h \
	  BaseCls.h \
	  Type.h \
	  DataMbr.h \
	  Method.h \
	  MethodAr.h \
	  CallFunc.h \
	  Typedf.h \
	  Apiif.h \
          Token.h \
	  ../G__ci.h

Token$(OBJPOST) : $(APIHEADER) Token.cxx
	$(CPP) $(IPATH) $(OPTIMIZE) $(MACRO) -c Token.cxx

Typedf$(OBJPOST) : $(APIHEADER) Typedf.cxx
	$(CPP) $(IPATH) $(OPTIMIZE) $(MACRO) -c Typedf.cxx

Class$(OBJPOST) : $(APIHEADER) Class.cxx
	$(CPP) $(IPATH) $(OPTIMIZE) $(MACRO) -c Class.cxx

BaseCls$(OBJPOST) : $(APIHEADER) BaseCls.cxx
	$(CPP) $(IPATH) $(OPTIMIZE) $(MACRO) -c BaseCls.cxx
	
Type$(OBJPOST) : $(APIHEADER) Type.cxx
	$(CPP) $(IPATH) $(OPTIMIZE) $(MACRO) -c Type.cxx

DataMbr$(OBJPOST) : $(APIHEADER) DataMbr.cxx
	$(CPP) $(IPATH) $(OPTIMIZE) $(MACRO) -c DataMbr.cxx

Method$(OBJPOST) : $(APIHEADER) Method.cxx
	$(CPP) $(IPATH) $(OPTIMIZE) $(MACRO) -c Method.cxx

MethodAr$(OBJPOST) : $(APIHEADER) MethodAr.cxx
	$(CPP) $(IPATH) $(OPTIMIZE) $(MACRO) -c MethodAr.cxx

CallFunc$(OBJPOST) : $(APIHEADER) CallFunc.cxx
	$(CPP) $(IPATH) $(OPTIMIZE) $(MACRO) -c CallFunc.cxx

Api$(OBJPOST) : $(APIHEADER) Api.cxx
	$(CPP) $(IPATH) $(OPTIMIZE) $(MACRO) -c Api.cxx

Apiif$(OBJPOST) : $(APIHEADER) Apiif.cxx
	$(CPP) $(IPATH) $(OPTIMIZE) $(MACRO) -c Apiif.cxx

longif$(OBJPOST) : $(APIHEADER) longif.cxx
	$(CPP) $(IPATH) $(OPTIMIZE) $(MACRO) -c longif.cxx

longif3$(OBJPOST) : $(APIHEADER) longif3.cxx
	$(CPP) $(IPATH) $(OPTIMIZE) $(MACRO) -c longif3.cxx

############################################################################
# Cint6.x Re-engineering
############################################################################

BCHEADER = bc_autoobj.h \
	   bc_cfunc.h \
	   bc_inst.h \
	   bc_parse.h \
	   bc_reader.h \
	   bc_type.h \
	   bc_exec.h \
	   bc_vtbl.h \
	   bc_eh.h \
	   bc_debug.h \
	   bc_assign.h \
	   common.h \
	  ../G__ci.h

bc_autoobj$(OBJPOST) : $(APIHEADER) $(BCHEADER) bc_autoobj.cxx
	$(CPP) $(IPATH) $(OPTIMIZE) $(MACRO) -c bc_autoobj.cxx

bc_cfunc$(OBJPOST) : $(APIHEADER) $(BCHEADER) bc_cfunc.cxx
	$(CPP) $(IPATH) $(OPTIMIZE) $(MACRO) -c bc_cfunc.cxx

bc_inst$(OBJPOST) : $(APIHEADER) $(BCHEADER) bc_inst.cxx
	$(CPP) $(IPATH) $(OPTIMIZE) $(MACRO) -c bc_inst.cxx

bc_parse$(OBJPOST) : $(APIHEADER) $(BCHEADER) bc_parse.cxx
	$(CPP) $(IPATH) $(OPTIMIZE) $(MACRO) -c bc_parse.cxx

bc_reader$(OBJPOST) : $(APIHEADER) $(BCHEADER) bc_reader.cxx
	$(CPP) $(IPATH) $(OPTIMIZE) $(MACRO) -c bc_reader.cxx

bc_type$(OBJPOST) : $(APIHEADER) $(BCHEADER) bc_type.cxx
	$(CPP) $(IPATH) $(OPTIMIZE) $(MACRO) -c bc_type.cxx

bc_exec$(OBJPOST) : $(APIHEADER) $(BCHEADER) bc_exec.cxx
	$(CPP) $(IPATH) $(OPTIMIZE) $(MACRO) -c bc_exec.cxx

bc_vtbl$(OBJPOST) : $(APIHEADER) $(BCHEADER) bc_vtbl.cxx
	$(CPP) $(IPATH) $(OPTIMIZE) $(MACRO) -c bc_vtbl.cxx

bc_eh$(OBJPOST) : $(APIHEADER) $(BCHEADER) bc_eh.cxx
	$(CPP) $(IPATH) $(OPTIMIZE) $(MACRO) -c bc_eh.cxx

bc_debug$(OBJPOST) : $(APIHEADER) $(BCHEADER) bc_debug.cxx
	$(CPP) $(IPATH) $(OPTIMIZE) $(MACRO) -c bc_debug.cxx

bc_assign$(OBJPOST) : $(APIHEADER) $(BCHEADER) bc_assign.cxx
	$(CPP) $(IPATH) $(OPTIMIZE) $(MACRO) -c bc_assign.cxx

bc_item$(OBJPOST) : $(APIHEADER) $(BCHEADER) bc_item.cxx
	$(CPP) $(IPATH) $(OPTIMIZE) $(MACRO) -c bc_item.cxx

# Not used
bc_dict$(OBJPOST) : $(APIHEADER) $(BCHEADER) bc_dict.cxx
	$(CPP) $(IPATH) $(OPTIMIZE) $(MACRO) -c bc_dict.cxx


############################################################################
# clean
############################################################################
clean :
	$(RM) $(OBJECTS) $(CINTLIB) core

