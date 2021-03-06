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
OPTIMIZE    = -O
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
## -*- text -*- ####################################################
#								   #
# Makefile for readline and history libraries.			   #
#								   #
####################################################################
#
# readline/Makefile.base
#
#  This file is modified based on original Makefile included in the
# GNU readline library.
#
#  Modifier    Masaharu Goto
#

# Here is a rule for making .o files from .c files that doesn't force
# the type of the machine (like -sun3) into the flags.
.c.o:
	$(CC2) -c $(CFLAGS) $(LOCAL_INCLUDES) \
		$(READLINE_DEFINES) $(CPPFLAGS) $*.c

# Destination installation directory.  The libraries are copied to DESTDIR
# when you do a `make install', and the header files to INCDIR/readline/*.h.
DESTDIR = /usr/local/lib
INCDIR = /usr/local/include

# Define TYPES as -DVOID_SIGHANDLER if your operating system uses
# a return type of "void" for signal handlers.
# TYPES = -DVOID_SIGHANDLER

# Define USG as -DUSG if you are using a System V operating system.
#USG = -DUSG
# New libc requests USGr3 to be defined to correspond to dirent.h change
USG = -DUSGr3

# HP-UX compilation requires the BSD library.
LOCAL_LIBS = -lBSD

# Xenix compilation requires -ldir -lx
#LOCAL_LIBS = -ldir -lx

# Comment out "-DVI_MODE" if you don't think that anyone will ever desire
# the vi line editing mode and features.  Make sure to comment out the
# definition of VI if you remove the -DVI_MODE.
READLINE_DEFINES = $(TYPES) -DVI_MODE 

# DEBUG_FLAGS = -O
DEBUG_FLAGS = $(OPTIMIZE)
LDFLAGS = $(DEBUG_FLAGS) 
CFLAGS = $(DEBUG_FLAGS) $(USG) -I.

# A good alternative is gcc -traditional.
#CC = gcc -traditional
#CC2 = cc
CC2 = $(KRCC)
#RANLIB = /usr/bin/ranlib
#AR = ar
#RM = rm
#CP = cp

LOCAL_INCLUDES = -I../

XMALLOC_DIR = ../malloc/
XMALLOC_C   = $(XMALLOC_DIR)xmalloc.c
ALLOCA_C    = $(XMALLOC_DIR)alloca.c
TILDE_DIR   = ../glob/
TILDE_C     = $(TILDE_DIR)tilde.c

CSOURCES = readline.c history.c funmap.c keymaps.c vi_mode.c \
	   emacs_keymap.c vi_keymap.c $(XMALLOC_C) $(TILDE_C) $(ALLOCA_C)

HSOURCES = readline.h chardefs.h history.h keymaps.h
SOURCES  = $(CSOURCES) $(HSOURCES)

OBJECTS = readline.o vi_mode.o history.o funmap.o keymaps.o xmalloc.o tilde.o alloca.o

DOCSOURCE = doc/rlman.texinfo doc/rltech.texinfo doc/rluser.texinfo \
	    doc/hist.texinfo doc/hsuser.texinfo doc/hstech.texinfo
DOCOBJECT = doc/readline.dvi doc/history.dvi
DOCSUPPORT = doc/Makefile doc/texinfo.tex
DOCUMENTATION = $(DOCSOURCE) $(DOCOBJECT) $(DOCSUPPORT)

SUPPORT = COPYING Makefile $(DOCUMENTATION) ChangeLog examples

THINGS_TO_TAR = $(SOURCES) $(SUPPORT)

##########################################################################

all: libreadline.a

libreadline.a:	$(OBJECTS)
	$(RM) -f $@
	$(AR) $(AROPT) $@ $(OBJECTS)
	-if [ -f "$(RANLIB)" ]; then $(RANLIB) libreadline.a; fi

readline: readline.h chardefs.h $(OBJECTS)
	$(CC2) $(CFLAGS) $(CPPFLAGS) $(READLINE_DEFINES) \
	  $(LOCAL_INCLUDES) -DTEST -o readline readline.c vi_mode.o funmap.o \
	  $(TILDE_O) $(XMALLOC_O) keymaps.o history.o -L. -ltermcap

# These next two rules are designed to ensure that the object files
# are left in the current directory.
#
tilde.o: $(TILDE_C)
	rm -f tilde.o
	$(CC2) $(CFLAGS) $(CPPFLAGS) -c $(TILDE_C)
	@-cp tilde.o $(TILDE_DIR)
	@-cp $(TILDE_DIR)tilde.o tilde.o

xmalloc.o: $(XMALLOC_C)
	rm -f xmalloc.o
	$(CC2) $(CFLAGS) $(CPPFLAGS) -c $(XMALLOC_C)
	@-cp xmalloc.o $(XMALLOC_DIR)
	@-cp $(XMALLOC_DIR)xmalloc.o xmalloc.o

alloca.o: $(ALLOCA_C)
	rm -f alloca.o
	$(CC2) $(CFLAGS) $(CPPFLAGS) -c $(ALLOCA_C)
	@-cp alloca.o $(XMALLOC_DIR)
	@-cp $(XMALLOC_DIR)alloca.o alloca.o

readline.o:	readline.c readline.h chardefs.h  keymaps.h history.h
history.o:	history.c history.h
vi_mode.o:	vi_mode.c
funmap.o:	funmap.c readline.h
keymaps.o:	keymaps.c emacs_keymap.c vi_keymap.c keymaps.h chardefs.h

documentation: force
	(cd doc; make)
force:

readline.tar:	$(THINGS_TO_TAR)
	tar -cf readline.tar $(THINGS_TO_TAR)

readline.tar.Z:	readline.tar
	compress -f readline.tar

install:	$(DESTDIR)/libreadline.a includes

# The rule for 'includes' is written funny so that the if statement
# always returns TRUE unless there really was an error installing the
# include files.
includes:
	if [ -r $(INCDIR)/readline ]; then \
	  :; \
	else \
	  mkdir $(INCDIR)/readline && chmod a+r $(INCDIR)/readline; \
	fi
	$(CP) readline.h keymaps.h chardefs.h $(INCDIR)/readline/
clean:
	rm -f $(OBJECTS) *.a core
	(cd doc; make clean)

$(DESTDIR)/libreadline.a: libreadline.a
	-mv $(DESTDIR)/libreadline.a $(DESTDIR)/libreadline.old
	cp libreadline.a $(DESTDIR)/libreadline.a
	if [ -f "$(RANLIB)" ]; then $(RANLIB) -t $(DESTDIR)/libreadline.a; fi
