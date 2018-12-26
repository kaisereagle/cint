# Microsoft Developer Studio Generated NMAKE File, Format Version 4.00
# ** DO NOT EDIT **

##################################################################
IPATH  = /I "C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\include" \
         /I "C:\Program Files (x86)\Microsoft SDKs\Windows\v7.1A\Include" 

#LIB         =  \
#   "C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\lib\msvcprt.lib" \
#   "C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\lib\MSVCRT.lib" \
#   "C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\lib\OLDNAMES.lib" \
#   "C:\Program Files (x86)\Microsoft SDKs\Windows\v7.1A\Lib\Uuid.lib" \
#   "C:\Program Files (x86)\Microsoft SDKs\Windows\v7.1A\Lib\Kernel32.lib"

LIB 	= \
 "C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\lib\OLDNAMES.lib" \
 "C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\lib\libcmt.lib" \
 "C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\lib\libcpmt.lib" \
 "C:\Program Files (x86)\Microsoft SDKs\Windows\v7.1A\Lib\Kernel32.lib" \
 "C:\Program Files (x86)\Microsoft SDKs\Windows\v7.1A\Lib\Uuid.lib" 

CFG=libcint - Win32 Release
##################################################################


# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

!IF "$(CFG)" == ""
CFG=libcint - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to libcint - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "libcint - Win32 Release" && "$(CFG)" !=\
 "libcint - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "libcint.mak" CFG="libcint - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "libcint - Win32 Release" (based on\
 "Win32 (x86) Dynamic-Link Library")
!MESSAGE "libcint - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 
################################################################################
# Begin Project
# PROP Target_Last_Scanned "libcint - Win32 Debug"
RSC=rc.exe
CPP=cl.exe
MTL=mktyplib.exe

!IF  "$(CFG)" == "libcint - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
OUTDIR=.\Release
INTDIR=.\Release

ALL : "$(OUTDIR)\libcint.dll"

CLEAN : 
	-@erase ".\Release\libcint.dll"
	-@erase ".\Release\VAL2A.OBJ"
	-@erase ".\Release\FUNC.OBJ"
	-@erase ".\Release\CLASS.OBJ"
	-@erase ".\Release\GLOBAL2.OBJ"
	-@erase ".\Release\PARSE.OBJ"
	-@erase ".\Release\API.OBJ"
	-@erase ".\Release\SHL.OBJ"
	-@erase ".\Release\WINNT.OBJ"
	-@erase ".\Release\METHODAR.OBJ"
	-@erase ".\Release\END.OBJ"
	-@erase ".\Release\DEBUG.OBJ"
	-@erase ".\Release\STUB.OBJ"
	-@erase ".\Release\ERROR.OBJ"
	-@erase ".\Release\DATAMBR.OBJ"
	-@erase ".\Release\GLOBAL1.OBJ"
	-@erase ".\Release\QUOTE.OBJ"
	-@erase ".\Release\INHERIT.OBJ"
	-@erase ".\Release\CAST.OBJ"
	-@erase ".\Release\INPUT.OBJ"
	-@erase ".\Release\DUMP.OBJ"
	-@erase ".\Release\EXPR.OBJ"
	-@erase ".\Release\IFUNC.OBJ"
	-@erase ".\Release\PRAGMA.OBJ"
	-@erase ".\Release\Token.obj"
	-@erase ".\Release\GCOLL.OBJ"
	-@erase ".\Release\AUXU.OBJ"
	-@erase ".\Release\DMYSTRM.OBJ"
	-@erase ".\Release\OPR.OBJ"
	-@erase ".\Release\G__SETUP.OBJ"
	-@erase ".\Release\INTRPT.OBJ"
	-@erase ".\Release\NEWLINK.OBJ"
	-@erase ".\Release\MEMTEST.OBJ"
	-@erase ".\Release\INIT.OBJ"
	-@erase ".\Release\LOADFILE.OBJ"
	-@erase ".\Release\DECL.OBJ"
	-@erase ".\Release\TYPEDEF.OBJ"
	-@erase ".\Release\G__CFUNC.OBJ"
	-@erase ".\Release\BASECLS.OBJ"
	-@erase ".\Release\CALLFUNC.OBJ"
	-@erase ".\Release\PCODE.OBJ"
	-@erase ".\Release\SIZEOF.OBJ"
	-@erase ".\Release\STDSTRCT.OBJ"
	-@erase ".\Release\DISP.OBJ"
	-@erase ".\Release\TMPLT.OBJ"
	-@erase ".\Release\TYPE.OBJ"
	-@erase ".\Release\NEW.OBJ"
	-@erase ".\Release\MALLOC.OBJ"
	-@erase ".\Release\PAUSE.OBJ"
	-@erase ".\Release\MACRO.OBJ"
	-@erase ".\Release\OLDLINK.OBJ"
	-@erase ".\Release\FREAD.OBJ"
	-@erase ".\Release\VALUE.OBJ"
	-@erase ".\Release\METHOD.OBJ"
	-@erase ".\Release\STRUCT.OBJ"
	-@erase ".\Release\SCRUPTO.OBJ"
	-@erase ".\Release\VAR.OBJ"
	-@erase ".\Release\APIIF.OBJ"
	-@erase ".\Release\TYPEDF.OBJ"
	-@erase ".\Release\BC_EXEC.OBJ"
	-@erase ".\Release\BC_INST.OBJ"
	-@erase ".\Release\BC_CFUNC.OBJ"
	-@erase ".\Release\BC_PARSE.OBJ"
	-@erase ".\Release\BC_VTBL.OBJ"
	-@erase ".\Release\BC_DEBUG.OBJ"
	-@erase ".\Release\BC_ASSIGN.OBJ"
	-@erase ".\Release\BC_AUTOOBJ.OBJ"
	-@erase ".\Release\BC_READER.OBJ"
	-@erase ".\Release\BC_TYPE.OBJ"
	-@erase ".\Release\BC_EH.OBJ"
	-@erase ".\Release\BC_ITEM.OBJ"
	-@erase ".\Release\libcint.lib"
	-@erase ".\Release\libcint.exp"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MT /w /W0 /GX /O2 /I "..\..\..\cint" /I "..\..\src" /I "..\..\lib\vcstream" /I "..\..\lib\stdstrct" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "G__VISUAL" /D "G__WIN32" /D "G__SHAREDLIB" /D "G__REDIRECTIO" /D "G__P2FCAST" /YX /c
CPP_PROJ=/nologo /MT /w /W0 /EHsc /D __VC13__ $(IPATH) /O2 /I "..\..\..\cint" /I "..\..\src" /I\
 "..\..\lib\vcstream" /I "..\..\lib\stdstrct" /D "NDEBUG" /D "WIN32" /D\
 "_WINDOWS" /D "G__VISUAL" /D "G__WIN32" /D "G__SHAREDLIB" /D "G__REDIRECTIO" /D\
 "G__P2FCAST" /Fp"$(INTDIR)/libcint.pch" /Fo"$(INTDIR)/" /c
CPP_OBJS=.\Release/
CPP_SBRS=
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/libcint.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib /nologo /subsystem:windows /dll /machine:I386
LINK32_FLAGS=$(LIB) /nologo /subsystem:windows /dll \
 /incremental:no /pdb:"$(OUTDIR)/libcint.pdb" /machine:I386 /def:".\libcintvc13.def"\
 /out:"$(OUTDIR)/libcint.dll" /implib:"$(OUTDIR)/libcint.lib" 
DEF_FILE= \
	".\libcintvc13.def"
LINK32_OBJS= \
	"$(INTDIR)/VAL2A.OBJ" \
	"$(INTDIR)/FUNC.OBJ" \
	"$(INTDIR)/CLASS.OBJ" \
	"$(INTDIR)/GLOBAL2.OBJ" \
	"$(INTDIR)/PARSE.OBJ" \
	"$(INTDIR)/API.OBJ" \
	"$(INTDIR)/SHL.OBJ" \
	"$(INTDIR)/WINNT.OBJ" \
	"$(INTDIR)/METHODAR.OBJ" \
	"$(INTDIR)/END.OBJ" \
	"$(INTDIR)/DEBUG.OBJ" \
	"$(INTDIR)/STUB.OBJ" \
	"$(INTDIR)/ERROR.OBJ" \
	"$(INTDIR)/DATAMBR.OBJ" \
	"$(INTDIR)/GLOBAL1.OBJ" \
	"$(INTDIR)/QUOTE.OBJ" \
	"$(INTDIR)/INHERIT.OBJ" \
	"$(INTDIR)/CAST.OBJ" \
	"$(INTDIR)/INPUT.OBJ" \
	"$(INTDIR)/DUMP.OBJ" \
	"$(INTDIR)/EXPR.OBJ" \
	"$(INTDIR)/IFUNC.OBJ" \
	"$(INTDIR)/PRAGMA.OBJ" \
	"$(INTDIR)/Token.obj" \
	"$(INTDIR)/GCOLL.OBJ" \
	"$(INTDIR)/AUXU.OBJ" \
	"$(INTDIR)/DMYSTRM.OBJ" \
	"$(INTDIR)/OPR.OBJ" \
	"$(INTDIR)/G__SETUP.OBJ" \
	"$(INTDIR)/INTRPT.OBJ" \
	"$(INTDIR)/NEWLINK.OBJ" \
	"$(INTDIR)/MEMTEST.OBJ" \
	"$(INTDIR)/INIT.OBJ" \
	"$(INTDIR)/LOADFILE.OBJ" \
	"$(INTDIR)/DECL.OBJ" \
	"$(INTDIR)/TYPEDEF.OBJ" \
	"$(INTDIR)/G__CFUNC.OBJ" \
	"$(INTDIR)/BASECLS.OBJ" \
	"$(INTDIR)/CALLFUNC.OBJ" \
	"$(INTDIR)/PCODE.OBJ" \
	"$(INTDIR)/SIZEOF.OBJ" \
	"$(INTDIR)/STDSTRCT.OBJ" \
	"$(INTDIR)/DISP.OBJ" \
	"$(INTDIR)/TMPLT.OBJ" \
	"$(INTDIR)/TYPE.OBJ" \
	"$(INTDIR)/NEW.OBJ" \
	"$(INTDIR)/MALLOC.OBJ" \
	"$(INTDIR)/PAUSE.OBJ" \
	"$(INTDIR)/MACRO.OBJ" \
	"$(INTDIR)/OLDLINK.OBJ" \
	"$(INTDIR)/FREAD.OBJ" \
	"$(INTDIR)/VALUE.OBJ" \
	"$(INTDIR)/METHOD.OBJ" \
	"$(INTDIR)/STRUCT.OBJ" \
	"$(INTDIR)/SCRUPTO.OBJ" \
	"$(INTDIR)/VAR.OBJ" \
	"$(INTDIR)/APIIF.OBJ" \
	"$(INTDIR)/BC_EXEC.OBJ" \
	"$(INTDIR)/BC_INST.OBJ" \
	"$(INTDIR)/BC_CFUNC.OBJ" \
	"$(INTDIR)/BC_PARSE.OBJ" \
	"$(INTDIR)/BC_VTBL.OBJ" \
	"$(INTDIR)/BC_DEBUG.OBJ" \
	"$(INTDIR)/BC_ASSIGN.OBJ" \
	"$(INTDIR)/BC_AUTOOBJ.OBJ" \
	"$(INTDIR)/BC_READER.OBJ" \
	"$(INTDIR)/BC_TYPE.OBJ" \
	"$(INTDIR)/BC_EH.OBJ" \
	"$(INTDIR)/BC_ITEM.OBJ" \
	"$(INTDIR)/TYPEDF.OBJ"

"$(OUTDIR)\libcint.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "libcint - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "$(OUTDIR)\libcint.dll"

CLEAN : 
	-@erase ".\Debug\vc40.pdb"
	-@erase ".\Debug\vc40.idb"
	-@erase ".\Debug\libcint.dll"
	-@erase ".\Debug\QUOTE.OBJ"
	-@erase ".\Debug\INPUT.OBJ"
	-@erase ".\Debug\DMYSTRM.OBJ"
	-@erase ".\Debug\IFUNC.OBJ"
	-@erase ".\Debug\Token.obj"
	-@erase ".\Debug\LOADFILE.OBJ"
	-@erase ".\Debug\TYPEDF.OBJ"
	-@erase ".\Debug\GCOLL.OBJ"
	-@erase ".\Debug\G__CFUNC.OBJ"
	-@erase ".\Debug\OLDLINK.OBJ"
	-@erase ".\Debug\CALLFUNC.OBJ"
	-@erase ".\Debug\INTRPT.OBJ"
	-@erase ".\Debug\FUNC.OBJ"
	-@erase ".\Debug\OPR.OBJ"
	-@erase ".\Debug\SHL.OBJ"
	-@erase ".\Debug\MALLOC.OBJ"
	-@erase ".\Debug\GLOBAL2.OBJ"
	-@erase ".\Debug\PCODE.OBJ"
	-@erase ".\Debug\STUB.OBJ"
	-@erase ".\Debug\BASECLS.OBJ"
	-@erase ".\Debug\TMPLT.OBJ"
	-@erase ".\Debug\METHOD.OBJ"
	-@erase ".\Debug\STRUCT.OBJ"
	-@erase ".\Debug\CAST.OBJ"
	-@erase ".\Debug\DUMP.OBJ"
	-@erase ".\Debug\NEW.OBJ"
	-@erase ".\Debug\EXPR.OBJ"
	-@erase ".\Debug\PAUSE.OBJ"
	-@erase ".\Debug\MACRO.OBJ"
	-@erase ".\Debug\FREAD.OBJ"
	-@erase ".\Debug\VALUE.OBJ"
	-@erase ".\Debug\DATAMBR.OBJ"
	-@erase ".\Debug\GLOBAL1.OBJ"
	-@erase ".\Debug\AUXU.OBJ"
	-@erase ".\Debug\STDSTRCT.OBJ"
	-@erase ".\Debug\INHERIT.OBJ"
	-@erase ".\Debug\VAR.OBJ"
	-@erase ".\Debug\APIIF.OBJ"
	-@erase ".\Debug\SCRUPTO.OBJ"
	-@erase ".\Debug\METHODAR.OBJ"
	-@erase ".\Debug\VAL2A.OBJ"
	-@erase ".\Debug\INIT.OBJ"
	-@erase ".\Debug\DECL.OBJ"
	-@erase ".\Debug\CLASS.OBJ"
	-@erase ".\Debug\PARSE.OBJ"
	-@erase ".\Debug\API.OBJ"
	-@erase ".\Debug\SIZEOF.OBJ"
	-@erase ".\Debug\WINNT.OBJ"
	-@erase ".\Debug\DISP.OBJ"
	-@erase ".\Debug\TYPE.OBJ"
	-@erase ".\Debug\NEWLINK.OBJ"
	-@erase ".\Debug\MEMTEST.OBJ"
	-@erase ".\Debug\TYPEDEF.OBJ"
	-@erase ".\Debug\END.OBJ"
	-@erase ".\Debug\DEBUG.OBJ"
	-@erase ".\Debug\PRAGMA.OBJ"
	-@erase ".\Debug\ERROR.OBJ"
	-@erase ".\Debug\G__SETUP.OBJ"
	-@erase ".\Debug\BC_EXEC.OBJ"
	-@erase ".\Debug\BC_INST.OBJ"
	-@erase ".\Debug\BC_CFUNC.OBJ"
	-@erase ".\Debug\BC_PARSE.OBJ"
	-@erase ".\Debug\BC_VTBL.OBJ"
	-@erase ".\Debug\BC_DEBUG.OBJ"
	-@erase ".\Debug\BC_ASSIGN.OBJ"
	-@erase ".\Debug\BC_AUTOOBJ.OBJ"
	-@erase ".\Debug\BC_READER.OBJ"
	-@erase ".\Debug\BC_TYPE.OBJ"
	-@erase ".\Debug\BC_EH.OBJ"
	-@erase ".\Debug\BC_ITEM.OBJ"
	-@erase ".\Debug\libcint.ilk"
	-@erase ".\Debug\libcint.lib"
	-@erase ".\Debug\libcint.exp"
	-@erase ".\Debug\libcint.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /I "..\..\..\cint" /I "..\..\src" /I "..\..\lib\vcstream" /I "..\..\lib\stdstrct" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "G__VISUAL" /D "G__WIN32" /D "G__SHAREDLIB" /D "G__REDIRECTIO" /D "G__P2FCAST"  /YX /c
CPP_PROJ=/nologo /MTd /W3 /Gm /EHsc /D __VC13__ $(IPATH) /Zi /Od /I "..\..\..\cint" /I "..\..\src" /I\
 "..\..\lib\vcstream" /I "..\..\lib\stdstrct" /D "_DEBUG" /D "WIN32" /D\
 "_WINDOWS" /D "G__VISUAL" /D "G__WIN32" /D "G__SHAREDLIB" /D "G__REDIRECTIO" /D\
 "G__P2FCAST" /Fp"$(INTDIR)/libcint.pch" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/libcint.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib /nologo /subsystem:windows /dll /debug /machine:I386
LINK32_FLAGS=$(LIB) /nologo /subsystem:windows /dll \
 /incremental:yes /pdb:"$(OUTDIR)/libcint.pdb" /debug /machine:I386\
 /def:".\libcintvc13.def" /out:"$(OUTDIR)/libcint.dll"\
 /implib:"$(OUTDIR)/libcint.lib" 
DEF_FILE= \
	".\libcintvc13.def"
LINK32_OBJS= \
	"$(INTDIR)/QUOTE.OBJ" \
	"$(INTDIR)/INPUT.OBJ" \
	"$(INTDIR)/DMYSTRM.OBJ" \
	"$(INTDIR)/IFUNC.OBJ" \
	"$(INTDIR)/Token.obj" \
	"$(INTDIR)/LOADFILE.OBJ" \
	"$(INTDIR)/TYPEDF.OBJ" \
	"$(INTDIR)/GCOLL.OBJ" \
	"$(INTDIR)/G__CFUNC.OBJ" \
	"$(INTDIR)/OLDLINK.OBJ" \
	"$(INTDIR)/CALLFUNC.OBJ" \
	"$(INTDIR)/INTRPT.OBJ" \
	"$(INTDIR)/FUNC.OBJ" \
	"$(INTDIR)/OPR.OBJ" \
	"$(INTDIR)/SHL.OBJ" \
	"$(INTDIR)/MALLOC.OBJ" \
	"$(INTDIR)/GLOBAL2.OBJ" \
	"$(INTDIR)/PCODE.OBJ" \
	"$(INTDIR)/STUB.OBJ" \
	"$(INTDIR)/BASECLS.OBJ" \
	"$(INTDIR)/TMPLT.OBJ" \
	"$(INTDIR)/METHOD.OBJ" \
	"$(INTDIR)/STRUCT.OBJ" \
	"$(INTDIR)/CAST.OBJ" \
	"$(INTDIR)/DUMP.OBJ" \
	"$(INTDIR)/NEW.OBJ" \
	"$(INTDIR)/EXPR.OBJ" \
	"$(INTDIR)/PAUSE.OBJ" \
	"$(INTDIR)/MACRO.OBJ" \
	"$(INTDIR)/FREAD.OBJ" \
	"$(INTDIR)/VALUE.OBJ" \
	"$(INTDIR)/DATAMBR.OBJ" \
	"$(INTDIR)/GLOBAL1.OBJ" \
	"$(INTDIR)/AUXU.OBJ" \
	"$(INTDIR)/STDSTRCT.OBJ" \
	"$(INTDIR)/INHERIT.OBJ" \
	"$(INTDIR)/VAR.OBJ" \
	"$(INTDIR)/APIIF.OBJ" \
	"$(INTDIR)/SCRUPTO.OBJ" \
	"$(INTDIR)/METHODAR.OBJ" \
	"$(INTDIR)/VAL2A.OBJ" \
	"$(INTDIR)/INIT.OBJ" \
	"$(INTDIR)/DECL.OBJ" \
	"$(INTDIR)/CLASS.OBJ" \
	"$(INTDIR)/PARSE.OBJ" \
	"$(INTDIR)/API.OBJ" \
	"$(INTDIR)/SIZEOF.OBJ" \
	"$(INTDIR)/WINNT.OBJ" \
	"$(INTDIR)/DISP.OBJ" \
	"$(INTDIR)/TYPE.OBJ" \
	"$(INTDIR)/NEWLINK.OBJ" \
	"$(INTDIR)/MEMTEST.OBJ" \
	"$(INTDIR)/TYPEDEF.OBJ" \
	"$(INTDIR)/END.OBJ" \
	"$(INTDIR)/DEBUG.OBJ" \
	"$(INTDIR)/PRAGMA.OBJ" \
	"$(INTDIR)/ERROR.OBJ" \
	"$(INTDIR)/BC_EXEC.OBJ" \
	"$(INTDIR)/BC_INST.OBJ" \
	"$(INTDIR)/BC_CFUNC.OBJ" \
	"$(INTDIR)/BC_PARSE.OBJ" \
	"$(INTDIR)/BC_VTBL.OBJ" \
	"$(INTDIR)/BC_DEBUG.OBJ" \
	"$(INTDIR)/BC_ASSIGN.OBJ" \
	"$(INTDIR)/BC_AUTOOBJ.OBJ" \
	"$(INTDIR)/BC_READER.OBJ" \
	"$(INTDIR)/BC_TYPE.OBJ" \
	"$(INTDIR)/BC_EH.OBJ" \
	"$(INTDIR)/BC_ITEM.OBJ" \
	"$(INTDIR)/G__SETUP.OBJ"

"$(OUTDIR)\libcint.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

################################################################################
# Begin Target

# Name "libcint - Win32 Release"
# Name "libcint - Win32 Debug"

!IF  "$(CFG)" == "libcint - Win32 Release"

!ELSEIF  "$(CFG)" == "libcint - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=..\API.CXX
DEP_CPP_API_C=\
	".\..\Api.h"\
	".\..\common.h"\
	".\..\..\..\cint\G__ci.h"\
	".\..\Property.h"\
	".\..\Class.h"\
	".\..\BaseCls.h"\
	".\..\Type.h"\
	".\..\Method.h"\
	".\..\MethodAr.h"\
	".\..\DataMbr.h"\
	".\..\CallFunc.h"\
	".\..\Typedf.h"\
	".\..\Token.h"\
	".\..\sunos.h"\
	".\..\newsos.h"\
	".\..\memtest.h"\
	".\..\security.h"\
	".\..\fproto.h"\
	".\..\global.h"\
	

"$(INTDIR)\API.OBJ" : $(SOURCE) $(DEP_CPP_API_C) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=..\APIIF.CXX
DEP_CPP_APIIF=\
	".\..\Apiif.h"\
	".\..\..\..\cint\G__ci.h"\
	".\..\Api.h"\
	".\..\sunos.h"\
	".\..\newsos.h"\
	".\..\memtest.h"\
	".\..\Property.h"\
	".\..\Class.h"\
	".\..\BaseCls.h"\
	".\..\Type.h"\
	".\..\Method.h"\
	".\..\MethodAr.h"\
	".\..\DataMbr.h"\
	".\..\CallFunc.h"\
	".\..\Typedf.h"\
	".\..\Token.h"\
	

"$(INTDIR)\APIIF.OBJ" : $(SOURCE) $(DEP_CPP_APIIF) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=..\AUXU.C
DEP_CPP_AUXU_=\
	".\..\common.h"\
	".\..\..\..\cint\G__ci.h"\
	".\..\memtest.h"\
	".\..\security.h"\
	".\..\fproto.h"\
	".\..\global.h"\
	".\..\sunos.h"\
	".\..\newsos.h"\
	

"$(INTDIR)\AUXU.OBJ" : $(SOURCE) $(DEP_CPP_AUXU_) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=..\BASECLS.CXX
DEP_CPP_BASEC=\
	".\..\Api.h"\
	".\..\common.h"\
	".\..\..\..\cint\G__ci.h"\
	".\..\Property.h"\
	".\..\Class.h"\
	".\..\BaseCls.h"\
	".\..\Type.h"\
	".\..\Method.h"\
	".\..\MethodAr.h"\
	".\..\DataMbr.h"\
	".\..\CallFunc.h"\
	".\..\Typedf.h"\
	".\..\Token.h"\
	".\..\sunos.h"\
	".\..\newsos.h"\
	".\..\memtest.h"\
	".\..\security.h"\
	".\..\fproto.h"\
	".\..\global.h"\
	

"$(INTDIR)\BASECLS.OBJ" : $(SOURCE) $(DEP_CPP_BASEC) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=..\CALLFUNC.CXX
DEP_CPP_CALLF=\
	".\..\Api.h"\
	".\..\common.h"\
	".\..\..\..\cint\G__ci.h"\
	".\..\Property.h"\
	".\..\Class.h"\
	".\..\BaseCls.h"\
	".\..\Type.h"\
	".\..\Method.h"\
	".\..\MethodAr.h"\
	".\..\DataMbr.h"\
	".\..\CallFunc.h"\
	".\..\Typedf.h"\
	".\..\Token.h"\
	".\..\sunos.h"\
	".\..\newsos.h"\
	".\..\memtest.h"\
	".\..\security.h"\
	".\..\fproto.h"\
	".\..\global.h"\
	

"$(INTDIR)\CALLFUNC.OBJ" : $(SOURCE) $(DEP_CPP_CALLF) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=..\CAST.C
DEP_CPP_CAST_=\
	".\..\common.h"\
	".\..\..\..\cint\G__ci.h"\
	".\..\memtest.h"\
	".\..\security.h"\
	".\..\fproto.h"\
	".\..\global.h"\
	".\..\sunos.h"\
	".\..\newsos.h"\
	

"$(INTDIR)\CAST.OBJ" : $(SOURCE) $(DEP_CPP_CAST_) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=..\CLASS.CXX
DEP_CPP_CLASS=\
	".\..\Api.h"\
	".\..\common.h"\
	".\..\..\..\cint\G__ci.h"\
	".\..\Property.h"\
	".\..\Class.h"\
	".\..\BaseCls.h"\
	".\..\Type.h"\
	".\..\Method.h"\
	".\..\MethodAr.h"\
	".\..\DataMbr.h"\
	".\..\CallFunc.h"\
	".\..\Typedf.h"\
	".\..\Token.h"\
	".\..\sunos.h"\
	".\..\newsos.h"\
	".\..\memtest.h"\
	".\..\security.h"\
	".\..\fproto.h"\
	".\..\global.h"\
	

"$(INTDIR)\CLASS.OBJ" : $(SOURCE) $(DEP_CPP_CLASS) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=..\DATAMBR.CXX
DEP_CPP_DATAM=\
	".\..\Api.h"\
	".\..\common.h"\
	".\..\..\..\cint\G__ci.h"\
	".\..\Property.h"\
	".\..\Class.h"\
	".\..\BaseCls.h"\
	".\..\Type.h"\
	".\..\Method.h"\
	".\..\MethodAr.h"\
	".\..\DataMbr.h"\
	".\..\CallFunc.h"\
	".\..\Typedf.h"\
	".\..\Token.h"\
	".\..\sunos.h"\
	".\..\newsos.h"\
	".\..\memtest.h"\
	".\..\security.h"\
	".\..\fproto.h"\
	".\..\global.h"\
	

"$(INTDIR)\DATAMBR.OBJ" : $(SOURCE) $(DEP_CPP_DATAM) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=..\DEBUG.C
DEP_CPP_DEBUG=\
	".\..\common.h"\
	".\..\..\..\cint\G__ci.h"\
	".\..\memtest.h"\
	".\..\security.h"\
	".\..\fproto.h"\
	".\..\global.h"\
	".\..\sunos.h"\
	".\..\newsos.h"\
	

"$(INTDIR)\DEBUG.OBJ" : $(SOURCE) $(DEP_CPP_DEBUG) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=..\DECL.C
DEP_CPP_DECL_=\
	".\..\common.h"\
	".\..\..\..\cint\G__ci.h"\
	".\..\memtest.h"\
	".\..\security.h"\
	".\..\fproto.h"\
	".\..\global.h"\
	".\..\sunos.h"\
	".\..\newsos.h"\
	

"$(INTDIR)\DECL.OBJ" : $(SOURCE) $(DEP_CPP_DECL_) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=..\DISP.C
DEP_CPP_DISP_=\
	".\..\common.h"\
	".\..\..\..\cint\G__ci.h"\
	".\..\memtest.h"\
	".\..\security.h"\
	".\..\fproto.h"\
	".\..\global.h"\
	".\..\sunos.h"\
	".\..\newsos.h"\
	

"$(INTDIR)\DISP.OBJ" : $(SOURCE) $(DEP_CPP_DISP_) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=..\DUMP.C
DEP_CPP_DUMP_=\
	".\..\common.h"\
	".\..\..\..\cint\G__ci.h"\
	".\..\memtest.h"\
	".\..\security.h"\
	".\..\fproto.h"\
	".\..\global.h"\
	".\..\sunos.h"\
	".\..\newsos.h"\
	

"$(INTDIR)\DUMP.OBJ" : $(SOURCE) $(DEP_CPP_DUMP_) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=..\END.C
DEP_CPP_END_C=\
	".\..\common.h"\
	".\..\..\..\cint\G__ci.h"\
	".\..\memtest.h"\
	".\..\security.h"\
	".\..\fproto.h"\
	".\..\global.h"\
	".\..\sunos.h"\
	".\..\newsos.h"\
	

"$(INTDIR)\END.OBJ" : $(SOURCE) $(DEP_CPP_END_C) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=..\ERROR.C
DEP_CPP_ERROR=\
	".\..\common.h"\
	".\..\..\..\cint\G__ci.h"\
	".\..\memtest.h"\
	".\..\security.h"\
	".\..\fproto.h"\
	".\..\global.h"\
	".\..\sunos.h"\
	".\..\newsos.h"\
	

"$(INTDIR)\ERROR.OBJ" : $(SOURCE) $(DEP_CPP_ERROR) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=..\EXPR.C
DEP_CPP_EXPR_=\
	".\..\common.h"\
	".\..\..\..\cint\G__ci.h"\
	".\..\memtest.h"\
	".\..\security.h"\
	".\..\fproto.h"\
	".\..\global.h"\
	".\..\sunos.h"\
	".\..\newsos.h"\
	

"$(INTDIR)\EXPR.OBJ" : $(SOURCE) $(DEP_CPP_EXPR_) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=..\FREAD.C
DEP_CPP_FREAD=\
	".\..\common.h"\
	".\..\..\..\cint\G__ci.h"\
	".\..\memtest.h"\
	".\..\security.h"\
	".\..\fproto.h"\
	".\..\global.h"\
	".\..\sunos.h"\
	".\..\newsos.h"\
	

"$(INTDIR)\FREAD.OBJ" : $(SOURCE) $(DEP_CPP_FREAD) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=..\FUNC.C
DEP_CPP_FUNC_=\
	".\..\common.h"\
	".\..\..\..\cint\G__ci.h"\
	".\..\memtest.h"\
	".\..\security.h"\
	".\..\fproto.h"\
	".\..\global.h"\
	".\..\sunos.h"\
	".\..\newsos.h"\
	

"$(INTDIR)\FUNC.OBJ" : $(SOURCE) $(DEP_CPP_FUNC_) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=..\G__CFUNC.C
DEP_CPP_G__CF=\
	".\..\common.h"\
	".\..\..\..\cint\G__ci.h"\
	".\..\memtest.h"\
	".\..\security.h"\
	".\..\fproto.h"\
	".\..\global.h"\
	".\..\sunos.h"\
	".\..\newsos.h"\
	

"$(INTDIR)\G__CFUNC.OBJ" : $(SOURCE) $(DEP_CPP_G__CF) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=..\GCOLL.C
DEP_CPP_GCOLL=\
	".\..\common.h"\
	".\..\..\..\cint\G__ci.h"\
	".\..\memtest.h"\
	".\..\security.h"\
	".\..\fproto.h"\
	".\..\global.h"\
	".\..\sunos.h"\
	".\..\newsos.h"\
	

"$(INTDIR)\GCOLL.OBJ" : $(SOURCE) $(DEP_CPP_GCOLL) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=..\GLOBAL1.C
DEP_CPP_GLOBA=\
	".\..\common.h"\
	".\..\..\..\cint\G__ci.h"\
	".\..\memtest.h"\
	".\..\security.h"\
	".\..\fproto.h"\
	".\..\global.h"\
	".\..\sunos.h"\
	".\..\newsos.h"\
	

"$(INTDIR)\GLOBAL1.OBJ" : $(SOURCE) $(DEP_CPP_GLOBA) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=..\GLOBAL2.C
DEP_CPP_GLOBAL=\
	".\..\common.h"\
	".\..\..\..\cint\G__ci.h"\
	".\..\memtest.h"\
	".\..\security.h"\
	".\..\fproto.h"\
	".\..\global.h"\
	".\..\sunos.h"\
	".\..\newsos.h"\
	

"$(INTDIR)\GLOBAL2.OBJ" : $(SOURCE) $(DEP_CPP_GLOBAL) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=..\IFUNC.C
DEP_CPP_IFUNC=\
	".\..\common.h"\
	".\..\..\..\cint\G__ci.h"\
	".\..\memtest.h"\
	".\..\security.h"\
	".\..\fproto.h"\
	".\..\global.h"\
	".\..\sunos.h"\
	".\..\newsos.h"\
	

"$(INTDIR)\IFUNC.OBJ" : $(SOURCE) $(DEP_CPP_IFUNC) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=..\INHERIT.C
DEP_CPP_INHER=\
	".\..\common.h"\
	".\..\..\..\cint\G__ci.h"\
	".\..\memtest.h"\
	".\..\security.h"\
	".\..\fproto.h"\
	".\..\global.h"\
	".\..\sunos.h"\
	".\..\newsos.h"\
	

"$(INTDIR)\INHERIT.OBJ" : $(SOURCE) $(DEP_CPP_INHER) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=..\INIT.C
DEP_CPP_INIT_=\
	".\..\common.h"\
	".\..\..\..\cint\G__ci.h"\
	".\..\memtest.h"\
	".\..\security.h"\
	".\..\fproto.h"\
	".\..\global.h"\
	".\..\sunos.h"\
	".\..\newsos.h"\
	

"$(INTDIR)\INIT.OBJ" : $(SOURCE) $(DEP_CPP_INIT_) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=..\INPUT.C
DEP_CPP_INPUT=\
	".\..\common.h"\
	".\..\..\..\cint\G__ci.h"\
	".\..\memtest.h"\
	".\..\security.h"\
	".\..\fproto.h"\
	".\..\global.h"\
	".\..\sunos.h"\
	".\..\newsos.h"\
	

"$(INTDIR)\INPUT.OBJ" : $(SOURCE) $(DEP_CPP_INPUT) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=..\INTRPT.C
DEP_CPP_INTRP=\
	".\..\common.h"\
	".\..\..\..\cint\G__ci.h"\
	".\..\memtest.h"\
	".\..\security.h"\
	".\..\fproto.h"\
	".\..\global.h"\
	".\..\sunos.h"\
	".\..\newsos.h"\
	

"$(INTDIR)\INTRPT.OBJ" : $(SOURCE) $(DEP_CPP_INTRP) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=..\LOADFILE.C
DEP_CPP_LOADF=\
	".\..\common.h"\
	".\..\..\..\cint\G__ci.h"\
	".\..\memtest.h"\
	".\..\security.h"\
	".\..\fproto.h"\
	".\..\global.h"\
	".\..\sunos.h"\
	".\..\newsos.h"\
	

"$(INTDIR)\LOADFILE.OBJ" : $(SOURCE) $(DEP_CPP_LOADF) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=..\MACRO.C
DEP_CPP_MACRO=\
	".\..\common.h"\
	".\..\..\..\cint\G__ci.h"\
	".\..\memtest.h"\
	".\..\security.h"\
	".\..\fproto.h"\
	".\..\global.h"\
	".\..\sunos.h"\
	".\..\newsos.h"\
	

"$(INTDIR)\MACRO.OBJ" : $(SOURCE) $(DEP_CPP_MACRO) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=..\MALLOC.C
DEP_CPP_MALLO=\
	".\..\common.h"\
	".\..\..\..\cint\G__ci.h"\
	".\..\memtest.h"\
	".\..\security.h"\
	".\..\fproto.h"\
	".\..\global.h"\
	".\..\sunos.h"\
	".\..\newsos.h"\
	

"$(INTDIR)\MALLOC.OBJ" : $(SOURCE) $(DEP_CPP_MALLO) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=..\MEMTEST.C
DEP_CPP_MEMTE=\
	".\..\common.h"\
	".\..\..\..\cint\G__ci.h"\
	".\..\memtest.h"\
	".\..\security.h"\
	".\..\fproto.h"\
	".\..\global.h"\
	".\..\sunos.h"\
	".\..\newsos.h"\
	

"$(INTDIR)\MEMTEST.OBJ" : $(SOURCE) $(DEP_CPP_MEMTE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=..\METHOD.CXX
DEP_CPP_METHO=\
	".\..\Api.h"\
	".\..\common.h"\
	".\..\..\..\cint\G__ci.h"\
	".\..\Property.h"\
	".\..\Class.h"\
	".\..\BaseCls.h"\
	".\..\Type.h"\
	".\..\Method.h"\
	".\..\MethodAr.h"\
	".\..\DataMbr.h"\
	".\..\CallFunc.h"\
	".\..\Typedf.h"\
	".\..\Token.h"\
	".\..\sunos.h"\
	".\..\newsos.h"\
	".\..\memtest.h"\
	".\..\security.h"\
	".\..\fproto.h"\
	".\..\global.h"\
	

"$(INTDIR)\METHOD.OBJ" : $(SOURCE) $(DEP_CPP_METHO) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=..\METHODAR.CXX
DEP_CPP_METHOD=\
	".\..\Api.h"\
	".\..\common.h"\
	".\..\..\..\cint\G__ci.h"\
	".\..\Property.h"\
	".\..\Class.h"\
	".\..\BaseCls.h"\
	".\..\Type.h"\
	".\..\Method.h"\
	".\..\MethodAr.h"\
	".\..\DataMbr.h"\
	".\..\CallFunc.h"\
	".\..\Typedf.h"\
	".\..\Token.h"\
	".\..\sunos.h"\
	".\..\newsos.h"\
	".\..\memtest.h"\
	".\..\security.h"\
	".\..\fproto.h"\
	".\..\global.h"\
	

"$(INTDIR)\METHODAR.OBJ" : $(SOURCE) $(DEP_CPP_METHOD) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=..\NEW.C
DEP_CPP_NEW_C=\
	".\..\common.h"\
	".\..\..\..\cint\G__ci.h"\
	".\..\memtest.h"\
	".\..\security.h"\
	".\..\fproto.h"\
	".\..\global.h"\
	".\..\sunos.h"\
	".\..\newsos.h"\
	

"$(INTDIR)\NEW.OBJ" : $(SOURCE) $(DEP_CPP_NEW_C) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=..\NEWLINK.C
DEP_CPP_NEWLI=\
	".\..\common.h"\
	".\..\dllrev.h"\
	".\..\..\..\cint\G__ci.h"\
	".\..\memtest.h"\
	".\..\security.h"\
	".\..\fproto.h"\
	".\..\global.h"\
	".\..\sunos.h"\
	".\..\newsos.h"\
	

"$(INTDIR)\NEWLINK.OBJ" : $(SOURCE) $(DEP_CPP_NEWLI) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=..\OLDLINK.C
DEP_CPP_OLDLI=\
	".\..\common.h"\
	".\..\..\..\cint\G__ci.h"\
	".\..\memtest.h"\
	".\..\security.h"\
	".\..\fproto.h"\
	".\..\global.h"\
	".\..\sunos.h"\
	".\..\newsos.h"\
	

"$(INTDIR)\OLDLINK.OBJ" : $(SOURCE) $(DEP_CPP_OLDLI) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=..\OPR.C
DEP_CPP_OPR_C=\
	".\..\common.h"\
	".\..\..\..\cint\G__ci.h"\
	".\..\memtest.h"\
	".\..\security.h"\
	".\..\fproto.h"\
	".\..\global.h"\
	".\..\sunos.h"\
	".\..\newsos.h"\
	

"$(INTDIR)\OPR.OBJ" : $(SOURCE) $(DEP_CPP_OPR_C) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=..\PARSE.C
DEP_CPP_PARSE=\
	".\..\common.h"\
	".\..\..\..\cint\G__ci.h"\
	".\..\memtest.h"\
	".\..\security.h"\
	".\..\fproto.h"\
	".\..\global.h"\
	".\..\sunos.h"\
	".\..\newsos.h"\
	

"$(INTDIR)\PARSE.OBJ" : $(SOURCE) $(DEP_CPP_PARSE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=..\PAUSE.C
DEP_CPP_PAUSE=\
	".\..\common.h"\
	".\..\..\..\cint\G__ci.h"\
	".\..\memtest.h"\
	".\..\security.h"\
	".\..\fproto.h"\
	".\..\global.h"\
	".\..\sunos.h"\
	".\..\newsos.h"\
	

"$(INTDIR)\PAUSE.OBJ" : $(SOURCE) $(DEP_CPP_PAUSE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=..\PCODE.C
DEP_CPP_PCODE=\
	".\..\common.h"\
	".\..\..\..\cint\G__ci.h"\
	".\..\memtest.h"\
	".\..\security.h"\
	".\..\fproto.h"\
	".\..\global.h"\
	".\..\sunos.h"\
	".\..\newsos.h"\
	

"$(INTDIR)\PCODE.OBJ" : $(SOURCE) $(DEP_CPP_PCODE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=..\PRAGMA.C
DEP_CPP_PRAGM=\
	".\..\common.h"\
	".\..\..\..\cint\G__ci.h"\
	".\..\memtest.h"\
	".\..\security.h"\
	".\..\fproto.h"\
	".\..\global.h"\
	".\..\sunos.h"\
	".\..\newsos.h"\
	

"$(INTDIR)\PRAGMA.OBJ" : $(SOURCE) $(DEP_CPP_PRAGM) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=..\QUOTE.C
DEP_CPP_QUOTE=\
	".\..\common.h"\
	".\..\..\..\cint\G__ci.h"\
	".\..\memtest.h"\
	".\..\security.h"\
	".\..\fproto.h"\
	".\..\global.h"\
	".\..\sunos.h"\
	".\..\newsos.h"\
	

"$(INTDIR)\QUOTE.OBJ" : $(SOURCE) $(DEP_CPP_QUOTE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=..\SCRUPTO.C
DEP_CPP_SCRUP=\
	".\..\common.h"\
	".\..\..\..\cint\G__ci.h"\
	".\..\memtest.h"\
	".\..\security.h"\
	".\..\fproto.h"\
	".\..\global.h"\
	".\..\sunos.h"\
	".\..\newsos.h"\
	

"$(INTDIR)\SCRUPTO.OBJ" : $(SOURCE) $(DEP_CPP_SCRUP) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=..\SHL.C
DEP_CPP_SHL_C=\
	".\..\common.h"\
	".\..\dllrev.h"\
	".\..\..\..\cint\G__ci.h"\
	".\..\memtest.h"\
	".\..\security.h"\
	".\..\fproto.h"\
	".\..\global.h"\
	".\..\sunos.h"\
	".\..\newsos.h"\
	

"$(INTDIR)\SHL.OBJ" : $(SOURCE) $(DEP_CPP_SHL_C) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=..\SIZEOF.C
DEP_CPP_SIZEO=\
	".\..\common.h"\
	".\..\..\..\cint\G__ci.h"\
	".\..\memtest.h"\
	".\..\security.h"\
	".\..\fproto.h"\
	".\..\global.h"\
	".\..\sunos.h"\
	".\..\newsos.h"\
	

"$(INTDIR)\SIZEOF.OBJ" : $(SOURCE) $(DEP_CPP_SIZEO) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=..\STDSTRCT.C
DEP_CPP_STDST=\
	".\..\stdstrct.h"\
	".\..\..\..\cint\G__ci.h"\
	".\..\..\lib\stdstrct\stdstr.h"\
	".\..\sunos.h"\
	".\..\newsos.h"\
	".\..\memtest.h"\
	

"$(INTDIR)\STDSTRCT.OBJ" : $(SOURCE) $(DEP_CPP_STDST) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=..\STRUCT.C
DEP_CPP_STRUC=\
	".\..\common.h"\
	".\..\..\..\cint\G__ci.h"\
	".\..\memtest.h"\
	".\..\security.h"\
	".\..\fproto.h"\
	".\..\global.h"\
	".\..\sunos.h"\
	".\..\newsos.h"\
	

"$(INTDIR)\STRUCT.OBJ" : $(SOURCE) $(DEP_CPP_STRUC) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=..\STUB.C
DEP_CPP_STUB_=\
	".\..\common.h"\
	".\..\..\..\cint\G__ci.h"\
	".\..\memtest.h"\
	".\..\security.h"\
	".\..\fproto.h"\
	".\..\global.h"\
	".\..\sunos.h"\
	".\..\newsos.h"\
	

"$(INTDIR)\STUB.OBJ" : $(SOURCE) $(DEP_CPP_STUB_) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=..\TMPLT.C
DEP_CPP_TMPLT=\
	".\..\common.h"\
	".\..\..\..\cint\G__ci.h"\
	".\..\memtest.h"\
	".\..\security.h"\
	".\..\fproto.h"\
	".\..\global.h"\
	".\..\sunos.h"\
	".\..\newsos.h"\
	

"$(INTDIR)\TMPLT.OBJ" : $(SOURCE) $(DEP_CPP_TMPLT) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=..\TYPE.CXX
DEP_CPP_TYPE_=\
	".\..\Api.h"\
	".\..\common.h"\
	".\..\..\..\cint\G__ci.h"\
	".\..\Property.h"\
	".\..\Class.h"\
	".\..\BaseCls.h"\
	".\..\Type.h"\
	".\..\Method.h"\
	".\..\MethodAr.h"\
	".\..\DataMbr.h"\
	".\..\CallFunc.h"\
	".\..\Typedf.h"\
	".\..\Token.h"\
	".\..\sunos.h"\
	".\..\newsos.h"\
	".\..\memtest.h"\
	".\..\security.h"\
	".\..\fproto.h"\
	".\..\global.h"\
	

"$(INTDIR)\TYPE.OBJ" : $(SOURCE) $(DEP_CPP_TYPE_) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=..\TYPEDEF.C
DEP_CPP_TYPED=\
	".\..\common.h"\
	".\..\..\..\cint\G__ci.h"\
	".\..\memtest.h"\
	".\..\security.h"\
	".\..\fproto.h"\
	".\..\global.h"\
	".\..\sunos.h"\
	".\..\newsos.h"\
	

"$(INTDIR)\TYPEDEF.OBJ" : $(SOURCE) $(DEP_CPP_TYPED) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=..\TYPEDF.CXX
DEP_CPP_TYPEDF=\
	".\..\Api.h"\
	".\..\common.h"\
	".\..\..\..\cint\G__ci.h"\
	".\..\Property.h"\
	".\..\Class.h"\
	".\..\BaseCls.h"\
	".\..\Type.h"\
	".\..\Method.h"\
	".\..\MethodAr.h"\
	".\..\DataMbr.h"\
	".\..\CallFunc.h"\
	".\..\Typedf.h"\
	".\..\Token.h"\
	".\..\sunos.h"\
	".\..\newsos.h"\
	".\..\memtest.h"\
	".\..\security.h"\
	".\..\fproto.h"\
	".\..\global.h"\
	

"$(INTDIR)\TYPEDF.OBJ" : $(SOURCE) $(DEP_CPP_TYPEDF) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=..\VAL2A.C
DEP_CPP_VAL2A=\
	".\..\common.h"\
	".\..\..\..\cint\G__ci.h"\
	".\..\memtest.h"\
	".\..\security.h"\
	".\..\fproto.h"\
	".\..\global.h"\
	".\..\sunos.h"\
	".\..\newsos.h"\
	

"$(INTDIR)\VAL2A.OBJ" : $(SOURCE) $(DEP_CPP_VAL2A) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=..\VALUE.C
DEP_CPP_VALUE=\
	".\..\common.h"\
	".\..\..\..\cint\G__ci.h"\
	".\..\memtest.h"\
	".\..\security.h"\
	".\..\fproto.h"\
	".\..\global.h"\
	".\..\sunos.h"\
	".\..\newsos.h"\
	

"$(INTDIR)\VALUE.OBJ" : $(SOURCE) $(DEP_CPP_VALUE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=..\VAR.C
DEP_CPP_VAR_C=\
	".\..\common.h"\
	".\..\..\..\cint\G__ci.h"\
	".\..\memtest.h"\
	".\..\security.h"\
	".\..\fproto.h"\
	".\..\global.h"\
	".\..\sunos.h"\
	".\..\newsos.h"\
	

"$(INTDIR)\VAR.OBJ" : $(SOURCE) $(DEP_CPP_VAR_C) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=..\DMYSTRM.C
DEP_CPP_VCSTR=\
	".\..\vcstrm.h"\
	".\..\..\..\cint\G__ci.h"\
	".\..\..\lib\vcstream\iostrm.h"\
	".\..\..\lib\vcstream\fstrm.h"\
	".\..\..\lib\vcstream\strstrm.h"\
	".\..\sunos.h"\
	".\..\newsos.h"\
	".\..\memtest.h"\
	

"$(INTDIR)\DMYSTRM.OBJ" : $(SOURCE) $(DEP_CPP_VCSTR) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################ MANUALLY ADDED  20120606
################################################################################
# Begin Source File

SOURCE=..\BC_EXEC.CXX
DEP_CPP_BC_EXEC=\
	".\..\bc_exec.h"\
	".\..\..\..\cint\G__ci.h"\
	".\..\..\lib\vcstream\iostrm.h"\
	".\..\..\lib\vcstream\fstrm.h"\
	".\..\..\lib\vcstream\strstrm.h"\
	".\..\sunos.h"\
	".\..\newsos.h"\
	".\..\memtest.h"\
	

"$(INTDIR)\BC_EXEC.OBJ" : $(SOURCE) $(DEP_CPP_BC_EXEC) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=..\BC_INST.CXX
DEP_CPP_BC_INST=\
	".\..\bc_inst.h"\
	".\..\..\..\cint\G__ci.h"\
	".\..\..\lib\vcstream\iostrm.h"\
	".\..\..\lib\vcstream\fstrm.h"\
	".\..\..\lib\vcstream\strstrm.h"\
	".\..\sunos.h"\
	".\..\newsos.h"\
	".\..\memtest.h"\
	

"$(INTDIR)\BC_INST.OBJ" : $(SOURCE) $(DEP_CPP_BC_INST) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=..\BC_CFUNC.CXX
DEP_CPP_BC_CFUNC=\
	".\..\bc_cfunc.h"\
	".\..\..\..\cint\G__ci.h"\
	".\..\..\lib\vcstream\iostrm.h"\
	".\..\..\lib\vcstream\fstrm.h"\
	".\..\..\lib\vcstream\strstrm.h"\
	".\..\sunos.h"\
	".\..\newsos.h"\
	".\..\memtest.h"\
	

"$(INTDIR)\BC_CFUNC.OBJ" : $(SOURCE) $(DEP_CPP_BC_CFUNC) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=..\BC_PARSE.CXX
DEP_CPP_BC_PARSE=\
	".\..\bc_parse.h"\
	".\..\..\..\cint\G__ci.h"\
	".\..\..\lib\vcstream\iostrm.h"\
	".\..\..\lib\vcstream\fstrm.h"\
	".\..\..\lib\vcstream\strstrm.h"\
	".\..\sunos.h"\
	".\..\newsos.h"\
	".\..\memtest.h"\
	

"$(INTDIR)\BC_PARSE.OBJ" : $(SOURCE) $(DEP_CPP_BC_PARSE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=..\BC_DEBUG.CXX
DEP_CPP_BC_DEBUG=\
	".\..\bc_debug.h"\
	".\..\..\..\cint\G__ci.h"\
	".\..\..\lib\vcstream\iostrm.h"\
	".\..\..\lib\vcstream\fstrm.h"\
	".\..\..\lib\vcstream\strstrm.h"\
	".\..\sunos.h"\
	".\..\newsos.h"\
	".\..\memtest.h"\
	

"$(INTDIR)\BC_DEBUG.OBJ" : $(SOURCE) $(DEP_CPP_BC_DEBUG) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=..\BC_VTBL.CXX
DEP_CPP_BC_VTBL=\
	".\..\bc_vtbl.h"\
	".\..\..\..\cint\G__ci.h"\
	".\..\..\lib\vcstream\iostrm.h"\
	".\..\..\lib\vcstream\fstrm.h"\
	".\..\..\lib\vcstream\strstrm.h"\
	".\..\sunos.h"\
	".\..\newsos.h"\
	".\..\memtest.h"\
	

"$(INTDIR)\BC_VTBL.OBJ" : $(SOURCE) $(DEP_CPP_BC_VTBL) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=..\BC_ASSIGN.CXX
DEP_CPP_BC_ASSIGN=\
	".\..\bc_assign.h"\
	".\..\..\..\cint\G__ci.h"\
	".\..\..\lib\vcstream\iostrm.h"\
	".\..\..\lib\vcstream\fstrm.h"\
	".\..\..\lib\vcstream\strstrm.h"\
	".\..\sunos.h"\
	".\..\newsos.h"\
	".\..\memtest.h"\
	

"$(INTDIR)\BC_ASSIGN.OBJ" : $(SOURCE) $(DEP_CPP_BC_ASSIGN) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=..\BC_AUTOOBJ.CXX
DEP_CPP_BC_AUTOOBJ=\
	".\..\bc_autoobj.h"\
	".\..\..\..\cint\G__ci.h"\
	".\..\..\lib\vcstream\iostrm.h"\
	".\..\..\lib\vcstream\fstrm.h"\
	".\..\..\lib\vcstream\strstrm.h"\
	".\..\sunos.h"\
	".\..\newsos.h"\
	".\..\memtest.h"\
	

"$(INTDIR)\BC_AUTOOBJ.OBJ" : $(SOURCE) $(DEP_CPP_BC_AUTOOBJ) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=..\BC_READER.CXX
DEP_CPP_BC_READER=\
	".\..\bc_reader.h"\
	".\..\..\..\cint\G__ci.h"\
	".\..\..\lib\vcstream\iostrm.h"\
	".\..\..\lib\vcstream\fstrm.h"\
	".\..\..\lib\vcstream\strstrm.h"\
	".\..\sunos.h"\
	".\..\newsos.h"\
	".\..\memtest.h"\
	

"$(INTDIR)\BC_READER.OBJ" : $(SOURCE) $(DEP_CPP_BC_READER) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=..\BC_TYPE.CXX
DEP_CPP_BC_TYPE=\
	".\..\bc_type.h"\
	".\..\..\..\cint\G__ci.h"\
	".\..\..\lib\vcstream\iostrm.h"\
	".\..\..\lib\vcstream\fstrm.h"\
	".\..\..\lib\vcstream\strstrm.h"\
	".\..\sunos.h"\
	".\..\newsos.h"\
	".\..\memtest.h"\
	

"$(INTDIR)\BC_TYPE.OBJ" : $(SOURCE) $(DEP_CPP_BC_TYPE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=..\BC_EH.CXX
DEP_CPP_BC_EH=\
	".\..\bc_eh.h"\
	".\..\..\..\cint\G__ci.h"\
	".\..\..\lib\vcstream\iostrm.h"\
	".\..\..\lib\vcstream\fstrm.h"\
	".\..\..\lib\vcstream\strstrm.h"\
	".\..\sunos.h"\
	".\..\newsos.h"\
	".\..\memtest.h"\
	

"$(INTDIR)\BC_EH.OBJ" : $(SOURCE) $(DEP_CPP_BC_EH) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=..\BC_ITEM.CXX
DEP_CPP_BC_ITEM=\
	".\..\bc_item.h"\
	".\..\..\..\cint\G__ci.h"\
	".\..\..\lib\vcstream\iostrm.h"\
	".\..\..\lib\vcstream\fstrm.h"\
	".\..\..\lib\vcstream\strstrm.h"\
	".\..\sunos.h"\
	".\..\newsos.h"\
	".\..\memtest.h"\
	

"$(INTDIR)\BC_ITEM.OBJ" : $(SOURCE) $(DEP_CPP_BC_ITEM) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################ END MANUALLY ADDED SOURCE
################################################################################
# Begin Source File

SOURCE=..\WINNT.C

"$(INTDIR)\WINNT.OBJ" : $(SOURCE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=..\..\MAIN\G__SETUP.C

"$(INTDIR)\G__SETUP.OBJ" : $(SOURCE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\libcintvc13.def

!IF  "$(CFG)" == "libcint - Win32 Release"

!ELSEIF  "$(CFG)" == "libcint - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\Token.cxx
DEP_CPP_TOKEN=\
	".\..\Api.h"\
	".\..\common.h"\
	".\..\..\..\cint\G__ci.h"\
	".\..\Property.h"\
	".\..\Class.h"\
	".\..\BaseCls.h"\
	".\..\Type.h"\
	".\..\Method.h"\
	".\..\MethodAr.h"\
	".\..\DataMbr.h"\
	".\..\CallFunc.h"\
	".\..\Typedf.h"\
	".\..\Token.h"\
	".\..\sunos.h"\
	".\..\newsos.h"\
	".\..\memtest.h"\
	".\..\security.h"\
	".\..\fproto.h"\
	".\..\global.h"\
	

"$(INTDIR)\Token.obj" : $(SOURCE) $(DEP_CPP_TOKEN) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
# End Target
# End Project
################################################################################
