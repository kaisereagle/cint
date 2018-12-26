ORIGIN		= Symantec C++
ORIGIN_VER	= Version 7.20
VERSION		= RELEASE

!IFDEF SUB_DEBUG
DEBUG		= $(SUB_DEBUG)
NDEBUG		= !$(SUB_DEBUG)
!ELSE
DEBUG		= 0
NDEBUG		= 1
!ENDIF

PROJ		= libcint
APPTYPE		= WIN32 DLL
PROJTYPE	= DLL

CC		= SC
CPP		= SPP
MAKE		= SMAKE
RC		= RCC
HC		= HC31
ASM		= SC
DISASM		= OBJ2ASM
LNK		= LINK
DLLS		= 

HEADERS		= common.h ..\lib\stdstrct\stdstr.h stdstrct.h  \
		..\lib\stream\strstrm.h ..\lib\stream\strm.h ..\lib\stream\stdiostr.h ..\lib\stream\fstrm.h  \
		..\lib\stream\iostrm.h libstrm.h Apiif.h Token.h  \
		Typedf.h CallFunc.h DataMbr.h MethodAr.h  \
		Method.h Type.h BaseCls.h Class.h  \
		Property.h Api.h dllrev.h ..\..\SC\INCLUDE\win32\objidl.h  \
		..\..\SC\INCLUDE\win32\unknwn.h ..\..\SC\INCLUDE\win32\wtypes.h ..\..\SC\INCLUDE\win32\ole2.h ..\..\SC\INCLUDE\win32\windows.h  \
		..\..\SC\INCLUDE\win32\rpcndr.h ..\..\SC\INCLUDE\win32\rpc.h ..\..\SC\INCLUDE\win32\poppack.h ..\..\SC\INCLUDE\win32\pshpack1.h  \
		global.h fproto.h security.h ..\G__ci.h 

DEFFILE		= LIBCINT.DEF

!IF $(DEBUG)
OUTPUTDIR	= .
CREATEOUTPUTDIR	=
TARGETDIR	= .
CREATETARGETDIR	=

LIBS		= KERNEL32.LIB GDI32.LIB USER32.LIB 

CFLAGS		=  -j0 -mn -C -WD -S -3 -a8 -c -g 
LFLAGS		=  /CO /NOI /DO /DE /XN /NT /ENTRY:__DllMainCRTStartup@12 /VERS:1.0 /BAS:268435456 /A:512 /IMPL:$(TARGETDIR)\$$SCW$$.LIB
DEFINES		= -DG__SHAREDLIB -DG__SYMANTEC
!ELSE
OUTPUTDIR	= .
CREATEOUTPUTDIR	=
TARGETDIR	= .
CREATETARGETDIR	=

LIBS		= KERNEL32.LIB GDI32.LIB USER32.LIB 

CFLAGS		=  -j0 -mn -WD -3 -a8 -c -w- -w2 -w6 -w7 
LFLAGS		=  /NOI /DO /DE /PACKF /XN /NT /ENTRY:__DllMainCRTStartup@12 /VERS:1.0 /BAS:268435456 /A:512 /IMPL:$(TARGETDIR)\$$SCW$$.LIB
DEFINES		= -DG__SHAREDLIB -DG__SYMANTEC -DG__REDIRECTIO
!ENDIF

HFLAGS		= $(CFLAGS) 
MFLAGS		= MASTERPROJ=$(PROJ) 
LIBFLAGS	=  /C 
RESFLAGS	=  -j0 -32 
DEBUGGERFLAGS	=  
AFLAGS		= $(CFLAGS) 
HELPFLAGS	= 

MODEL		= N

PAR		= PROJS BATS OBJS

RCDEFINES	= 

INCLUDES	= -Ic:\cint -Ic:\cint\lib\stream -Ic:\cint\src -Ic:\cint\lib\stdstrct

INCLUDEDOBJS	= 

OBJS		= AUXU.OBJ CAST.OBJ DEBUG.OBJ  \
		DECL.OBJ DISP.OBJ DUMP.OBJ END.OBJ  \
		ERROR.OBJ EXPR.OBJ FREAD.OBJ FUNC.OBJ  \
		G__CFUNC.OBJ GCOLL.OBJ GLOBAL1.OBJ GLOBAL2.OBJ  \
		IFUNC.OBJ INHERIT.OBJ INIT.OBJ INPUT.OBJ  \
		INTRPT.OBJ LOADFILE.OBJ MACRO.OBJ MALLOC.OBJ  \
		NEW.OBJ NEWLINK.OBJ OLDLINK.OBJ OPR.OBJ  \
		PARSE.OBJ PAUSE.OBJ PCODE.OBJ PRAGMA.OBJ  \
		QUOTE.OBJ SCRUPTO.OBJ SHL.OBJ SIZEOF.OBJ  \
		STRUCT.OBJ STUB.OBJ TMPLT.OBJ TYPEDEF.OBJ  \
		VAL2A.OBJ VALUE.OBJ VAR.OBJ WINNT.OBJ  \
		API.OBJ APIIF.OBJ BASECLS.OBJ CALLFUNC.OBJ  \
		CLASS.OBJ DATAMBR.OBJ METHOD.OBJ METHODAR.OBJ  \
		TYPE.OBJ TYPEDF.OBJ LIBSTRM.OBJ ..\MAIN\G__SETUP.OBJ  \
		STDSTRCT.OBJ Token.OBJ 

RCFILES		= 

RESFILES	= 

SYMS		= 

HELPFILES	= 

BATS		= 

.SUFFIXES: .C .CP .CPP .CXX .CC .H .HPP .HXX .COM .EXE .DLL .LIB .RTF .DLG .ASM .RES .RC .OBJ 

.C.OBJ:
	$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$*.obj $*.c

.CPP.OBJ:
	$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$*.obj $*.cpp

.CXX.OBJ:
	$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$*.obj $*.cxx

.CC.OBJ:
	$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$*.obj $*.cc

.CP.OBJ:
	$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$*.obj $*.cp

.H.SYM:
	$(CC) $(HFLAGS) $(DEFINES) $(INCLUDES) -HF -o$(*B).sym $*.h

.HPP.SYM:
	$(CC) $(HFLAGS) $(DEFINES) $(INCLUDES) -HF -o$(*B).sym $*.hpp

.HXX.SYM:
	$(CC) $(HFLAGS) $(DEFINES) $(INCLUDES) -HF -o$(*B).sym $*.hxx

.C.EXP:
	$(CPP) $(CFLAGS) $(DEFINES) $(INCLUDES)   $*.c   -o$*.lst

.CPP.EXP:
	$(CPP) $(CFLAGS) $(DEFINES) $(INCLUDES) $*.cpp -o$*.lst

.CXX.EXP:
	$(CPP) $(CFLAGS) $(DEFINES) $(INCLUDES) $*.cxx -o$*.lst

.CP.EXP:
	$(CPP) $(CFLAGS) $(DEFINES) $(INCLUDES)  $*.cp  -o$*.lst

.CC.EXP:
	$(CPP) $(CFLAGS) $(DEFINES) $(INCLUDES)  $*.cc  -o$*.lst

.ASM.EXP:
	$(CPP) $(CFLAGS) $(DEFINES) $(INCLUDES) $*.asm -o$*.lst

.OBJ.COD:
	$(DISASM) $*.OBJ -c

.OBJ.EXE:
	$(LNK) $(LFLAGS) @$(PROJ).LNK

.RTF.HLP:
	$(HC) $(HELPFLAGS) $*.HPJ

.ASM.OBJ:
	$(ASM) $(AFLAGS) $(DEFINES) $(INCLUDES) -o$*.obj $*.asm

.RC.RES: 
	$(RC) $(RCDEFINES) $(RESFLAGS) $(INCLUDES) $*.rc -o$*.res

.DLG.RES:
	echo ^#include "windows.h" >$$$*.rc
	echo ^IF EXIST "$*.h" >>$$$*.rc
	echo ^#include "$*.h" >>$$$*.rc
	echo ^#include "$*.dlg" >>$$$*.rc
	$(RC) $(RCDEFINES) $(RESFLAGS) $(INCLUDES) $$$*.rc
	-del $*.res
	-ren $$$*.res $*.res



all:	createdir $(PRECOMPILE) $(SYMS) $(OBJS) $(INCLUDEDOBJS) $(POSTCOMPILE) $(TARGETDIR)\$(PROJ).$(PROJTYPE) $(POSTLINK) _done

createdir:
	$(CREATEOUTPUTDIR)
	$(CREATETARGETDIR)

$(TARGETDIR)\$(PROJ).$(PROJTYPE): $(OBJS) $(INCLUDEDOBJS) $(RCFILES) $(RESFILES) $(HELPFILES) $(DEFFILE)
			-del $(TARGETDIR)\$(PROJ).$(PROJTYPE)
			$(LNK) $(LFLAGS) @$(PROJ).LNK;
			-ren $(TARGETDIR)\$$SCW$$.$(PROJTYPE) $(PROJ).$(PROJTYPE)
			-del $(TARGETDIR)\$(PROJ).LIB
			-ren $(TARGETDIR)\$$SCW$$.LIB $(PROJ).LIB
			-echo $(TARGETDIR)\$(PROJ).$(PROJTYPE) built

_done:
		-echo $(PROJ).$(PROJTYPE) done

buildall:	clean	all


clean:
		-del $(TARGETDIR)\$$SCW$$.$(PROJTYPE)
		-del $(TARGETDIR)\$(PROJ).CLE
		-del $(OUTPUTDIR)\SCPH.SYM
		-del libcint.dpd
		-del $(OBJS)

cleanres:

res:		cleanres $(RCFILES) all


link:
		$(LNK) $(LFLAGS) @$(PROJ).LNK;
		-del $(TARGETDIR)\$(PROJ).$(PROJTYPE)
		-ren $(TARGETDIR)\$$SCW$$.$(PROJTYPE) $(PROJ).$(PROJTYPE)
			-del $(TARGETDIR)\$(PROJ).LIB
			-ren $(TARGETDIR)\$$SCW$$.LIB $(PROJ).LIB





!IF EXIST (libcint.dpd)
!INCLUDE libcint.dpd
!ENDIF


