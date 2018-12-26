############################################################
# platform/msdos/Makefile
#  Platform dependent information for MS-DOS DJGPP
############################################################

# Tools
RM          = rm -f
CP          = cp
AR          = ar
AROPT       = clq
KRCC        = gcc -traditional -I$(DJGPPDIR)/include
CC          = gcc -I$(DJGPPDIR)/include
CPP         = gxx -I$(DJGPPDIR)/include
LD          = gxx -I$(DJGPPDIR)/include
CPREP       = gcc -E -I$(DJGPPDIR)/include
CPPPREP     = gxx -E -I$(DJGPPDIR)/include

# Compiler and linker option
CCDLLOPT    =
LDDLLOPT    =
OPTIMIZE    = -O
LDOPT       = -L$(DJGPPDIR)/lib -lm
SYSMACRO    =
OTHMACRO    = -DG__DJGPP
SYSIPATH    = 

# libraries
MAINO       = $(CINTSYSDIR)/main/cppmain.o
CINTLIB     = $(CINTSYSDIR)/src/G__ci.a
READLINEA   =
APIO        = Api.o Class.o BaseCls.o Type.o DataMbr.o Method.o MethodAr.o CallFunc.o Typedf.o Apiif.o Token.o
RANLIB      = ranlib
STDLIBS     = dmystrm.o stdstrct.o
PLATFORM    =

# source code postfix
CSRCPOST    = .c
CHDRPOST    = .h
CPPSRCPOST  = .cxx
CPPHDRPOST  = .h
OBJPOST     = .o
DLLPOST     =



##############################################################
# $CINTSYSDIR/src/Makefile.base
#
# Description:
#   This file is a base of cint library makefile. There are examples
#  of platform dependent setup information under platform
#  directory. Do following to create a complete Makefile,
#
#  $ cat ../platform/[YourPlatform] Makefile.base > Makefile
#
##############################################################

CINTSYSDIR   = ..
IPATH    = -I.. $(SYSIPATH) 
MACRO    = $(SYSMACRO) $(OTHMACRO)

############################################################################
# common variable definition
############################################################################

SECURITYH =     security.h
DLLREVH =       dllrev.h

HEADERS =       common.h global.h fproto.h memtest.h sunos.h ../G__ci.h

OBJECTS =       $(PLATFORM) \
                $(STDLIBS) \
                $(APIO) \
                auxu.o \
                cast.o \
                debug.o \
                decl.o \
                disp.o \
                dump.o \
                end.o \
                error.o \
                expr.o \
                fread.o \
                func.o \
                gcoll.o \
                global1.o \
                global2.o \
                g__cfunc.o \
                ifunc.o \
                inherit.o \
                init.o \
                input.o \
                intrpt.o \
                loadfile.o \
                macro.o \
                malloc.o \
                memtest.o \
                new.o \
                newlink.o \
                oldlink.o \
                opr.o \
                parse.o \
                pause.o \
                pcode.o \
                pragma.o \
                quote.o \
                scrupto.o \
                shl.o \
                sizeof.o \
                struct.o \
                stub.o \
                tmplt.o \
                typedef.o \
                val2a.o \
                value.o \
                var.o

CINTEXE  = $(CINTSYSDIR)/cint.exe
MAKECINTEXE  = $(CINTSYSDIR)/makecint.exe


done : $(CINTEXE) $(MAKECINTEXE)
	echo > done

$(MAKECINTEXE) : ../tool/makecint.c
	$(CC) $(IPATH) $(MACRO) $(OPTIMIZE) -c ../tool/makecint.c -o ../tool/makecint.o
	$(LD) -o $(MAKECINTEXE) ../tool/makecint.o $(LDOPT)

$(CINTEXE) : $(CINTLIB) ../main/cppmain.cxx ../main/g__setup.c
	$(CC) $(IPATH) $(MACRO) $(OPTIMIZE) -c ../main/g__setup.c
	$(CPP) $(IPATH) $(MACRO) $(OPTIMIZE) -c ../main/cppmain.cxx -o ../main/cppmain.o
	$(LD) -o $(CINTEXE) ../main/cppmain.o g__setup.o $(CINTLIB) $(OPTIMIZE) $(LDOPT)

$(CINTLIB) : $(OBJECTS)
	$(RM) $@
	$(AR) $(AROPT) $@ $(OBJECTS)
	$(RANLIB) $(CINTLIB)

############################################################################
# iostream library
############################################################################
libstrm.o : libstrm.cxx libstrm.h $(HEADERS)
	$(CPP) -I../lib/stream $(IPATH) $(MACRO) $(OPTIMIZE) -c libstrm.cxx

dmystrm.o : dmystrm.c $(HEADERS)
	$(CC) $(IPATH) $(MACRO) $(OPTIMIZE) -c dmystrm.c

# Visual C++
vcstrm.o : vcstrm.cxx vcstrm.h $(HEADERS)
	$(CPP) -I../lib/vcstream $(IPATH) $(MACRO) $(OPTIMIZE) -c vcstrm.cxx

# Borland C++
bcstrm.o : bcstrm.cxx bcstrm.h $(HEADERS)
	$(CPP) -I../lib/bcstream $(IPATH) $(MACRO) $(OPTIMIZE) -c bcstrm.cxx

############################################################################
# SunOS 4.1 specific source
############################################################################
sunos.o : sunos.c $(HEADERS)
	$(CC) $(IPATH) $(MACRO) $(OPTIMIZE) -DNONANSI -c sunos.c

############################################################################
# NewsOS specific source
############################################################################
newsos.o : newsos.c $(HEADERS)
	$(CC) $(IPATH) $(MACRO) $(OPTIMIZE) -DNONANSI -c newsos.c

############################################################################
# Windows-NT specific source
############################################################################
winnt.o : winnt.c $(HEADERS)
	$(CC) $(IPATH) $(MACRO) $(OPTIMIZE) -c winnt.c

############################################################################
# input.c depends on GNU readline library
############################################################################
input.o : input.c $(HEADERS)
	$(CC) $(IPATH) $(MACRO) $(OPTIMIZE) -c input.c

############################################################################
# lconv, div_t, ldiv_t, tm struct
############################################################################
stdstrct.o : stdstrct.c stdstrct.h $(HEADERS)
	$(CC) -I../lib/stdstrct $(IPATH) $(MACRO) $(OPTIMIZE) -c stdstrct.c

dmystrct.o : dmystrct.c $(HEADERS)
	$(CC) $(IPATH) $(MACRO) $(OPTIMIZE) -c dmystrct.c

############################################################################
# Common sources
############################################################################
auxu.o : auxu.c $(HEADERS)
	$(CC) $(IPATH) $(MACRO) $(OPTIMIZE) -c auxu.c

cast.o : cast.c $(HEADERS) $(SECURITYH)
	$(CC) $(IPATH) $(MACRO) $(OPTIMIZE) -c cast.c

debug.o : debug.c $(HEADERS)
	$(CC) $(IPATH) $(MACRO) $(OPTIMIZE) -c debug.c

decl.o : decl.c $(HEADERS)
	$(CC) $(IPATH) $(MACRO) $(OPTIMIZE) -c decl.c

disp.o : disp.c $(HEADERS)
	$(CC) $(IPATH) $(MACRO) $(OPTIMIZE) -c disp.c

dump.o : dump.c $(HEADERS)
	$(CC) $(IPATH) $(MACRO) $(OPTIMIZE) -c dump.c

end.o : end.c $(HEADERS) $(SECURITYH)
	$(CC) $(IPATH) $(MACRO) $(OPTIMIZE) -c end.c

error.o : error.c $(HEADERS) $(SECURITYH)
	$(CC) $(IPATH) $(MACRO) $(OPTIMIZE) -c error.c

expr.o : expr.c $(HEADERS)
	$(CC) $(IPATH) $(MACRO) $(OPTIMIZE) -c expr.c

fread.o : fread.c $(HEADERS)
	$(CC) $(IPATH) $(MACRO) $(OPTIMIZE) -c fread.c

func.o : func.c $(HEADERS) $(SECURITYH)
	$(CC) $(IPATH) $(MACRO) $(OPTIMIZE) -c func.c

gcoll.o : gcoll.c $(HEADERS) $(SECURITYH)
	$(CC) $(IPATH) $(MACRO) $(OPTIMIZE) -c gcoll.c

global1.o : global1.c $(HEADERS)
	$(CC) $(IPATH) $(MACRO) $(OPTIMIZE) -c global1.c

global2.o : global2.c $(HEADERS) $(SECURITYH)
	$(CC) $(IPATH) $(MACRO) $(OPTIMIZE) -c global2.c

g__cfunc.o : g__cfunc.c $(HEADERS) $(SECURITYH)
	$(CC) $(IPATH) $(MACRO) $(OPTIMIZE) -c g__cfunc.c

ifunc.o : ifunc.c $(HEADERS) $(SECURITYH)
	$(CC) $(IPATH) $(MACRO) $(OPTIMIZE) -c ifunc.c

inherit.o : inherit.c $(HEADERS)
	$(CC) $(IPATH) $(MACRO) $(OPTIMIZE) -c inherit.c

init.o : init.c $(HEADERS) $(SECURITYH)
	$(CC) $(IPATH) $(MACRO) $(OPTIMIZE) -c init.c

intrpt.o : intrpt.c $(HEADERS)
	$(CC) $(IPATH) $(MACRO) $(OPTIMIZE) -c intrpt.c

loadfile.o : loadfile.c $(HEADERS) $(SECURITYH)
	$(CC) $(IPATH) $(MACRO) $(OPTIMIZE) -c loadfile.c

macro.o : macro.c $(HEADERS)
	$(CC) $(IPATH) $(MACRO) $(OPTIMIZE) -c macro.c

malloc.o : malloc.c $(HEADERS)
	$(CC) $(IPATH) $(MACRO) $(OPTIMIZE) -c malloc.c

memtest.o : memtest.c $(HEADERS)
	$(CC) $(IPATH) $(MACRO) $(OPTIMIZE) -c memtest.c

new.o : new.c $(HEADERS) $(SECURITYH) $(SECURITYH)
	$(CC) $(IPATH) $(MACRO) $(OPTIMIZE) -c new.c

newlink.o : newlink.c $(HEADERS) $(DLLREVH)
	$(CC) $(IPATH) $(MACRO) $(OPTIMIZE) -c newlink.c

oldlink.o : oldlink.c $(HEADERS)
	$(CC) $(IPATH) $(MACRO) $(OPTIMIZE) -c oldlink.c

opr.o : opr.c $(HEADERS) $(SECURITYH)
	$(CC) $(IPATH) $(MACRO) $(OPTIMIZE) -c opr.c

parse.o : parse.c $(HEADERS) $(SECURITYH)
	$(CC) $(IPATH) $(MACRO) $(OPTIMIZE) -c parse.c

pause.o : pause.c $(HEADERS) $(SECURITYH)
	$(CC) $(IPATH) $(MACRO) $(OPTIMIZE) -c pause.c

pcode.o : pcode.c $(HEADERS)
	$(CC) $(IPATH) $(MACRO) $(OPTIMIZE) -c pcode.c

pragma.o : pragma.c $(HEADERS) $(SECURITYH)
	$(CC) $(IPATH) $(MACRO) $(OPTIMIZE) -c pragma.c

quote.o : quote.c $(HEADERS)
	$(CC) $(IPATH) $(MACRO) $(OPTIMIZE) -c quote.c

scrupto.o : scrupto.c $(HEADERS) $(SECURITYH)
	$(CC) $(IPATH) $(MACRO) $(OPTIMIZE) -c scrupto.c

sizeof.o : sizeof.c $(HEADERS)
	$(CC) $(IPATH) $(MACRO) $(OPTIMIZE) -c sizeof.c

shl.o : shl.c $(HEADERS) $(DLLREVH)
	$(CC) $(IPATH) $(MACRO) $(OPTIMIZE) -c shl.c

struct.o : struct.c $(HEADERS)
	$(CC) $(IPATH) $(MACRO) $(OPTIMIZE) -c struct.c

stub.o : stub.c $(HEADERS)
	$(CC) $(IPATH) $(MACRO) $(OPTIMIZE) -c stub.c

tmplt.o : tmplt.c $(HEADERS)
	$(CC) $(IPATH) $(MACRO) $(OPTIMIZE) -c tmplt.c

typedef.o : typedef.c $(HEADERS)
	$(CC) $(IPATH) $(MACRO) $(OPTIMIZE) -c typedef.c

val2a.o : val2a.c $(HEADERS)
	$(CC) $(IPATH) $(MACRO) $(OPTIMIZE) -c val2a.c

value.o : value.c $(HEADERS)
	$(CC) $(IPATH) $(MACRO) $(OPTIMIZE) -c value.c

var.o : var.c $(HEADERS) $(SECURITYH)
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

Token.o : $(APIHEADER) Token.cxx
	$(CPP) $(IPATH) $(OPTIMIZE) $(MACRO) -c Token.cxx

Typedf.o : $(APIHEADER) Typedf.cxx
	$(CPP) $(IPATH) $(OPTIMIZE) $(MACRO) -c Typedf.cxx

Class.o : $(APIHEADER) Class.cxx
	$(CPP) $(IPATH) $(OPTIMIZE) $(MACRO) -c Class.cxx

BaseCls.o : $(APIHEADER) BaseCls.cxx
	$(CPP) $(IPATH) $(OPTIMIZE) $(MACRO) -c BaseCls.cxx

Type.o : $(APIHEADER) Type.cxx
	$(CPP) $(IPATH) $(OPTIMIZE) $(MACRO) -c Type.cxx

DataMbr.o : $(APIHEADER) DataMbr.cxx
	$(CPP) $(IPATH) $(OPTIMIZE) $(MACRO) -c DataMbr.cxx

Method.o : $(APIHEADER) Method.cxx
	$(CPP) $(IPATH) $(OPTIMIZE) $(MACRO) -c Method.cxx

MethodAr.o : $(APIHEADER) MethodAr.cxx
	$(CPP) $(IPATH) $(OPTIMIZE) $(MACRO) -c MethodAr.cxx

CallFunc.o : $(APIHEADER) CallFunc.cxx
	$(CPP) $(IPATH) $(OPTIMIZE) $(MACRO) -c CallFunc.cxx

Api.o : $(APIHEADER) Api.cxx
	$(CPP) $(IPATH) $(OPTIMIZE) $(MACRO) -c Api.cxx

Apiif.o : $(APIHEADER) Apiif.cxx
	$(CPP) $(IPATH) $(OPTIMIZE) $(MACRO) -c Apiif.cxx


############################################################################
# clean
############################################################################
clean :
	$(RM) $(OBJECTS) $(CINTLIB) core

