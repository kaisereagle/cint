# Microsoft Developer Studio Generated NMAKE File, Format Version 13.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

!IF "$(CFG)" == ""
CFG=iostream - Win32 Release
!MESSAGE No configuration specified.  Defaulting to iostream - Win32 Release.
!ENDIF 

!IF "$(CFG)" != "iostream - Win32 Release" && "$(CFG)" !=\
 "iostream - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "iostream.mak" CFG="iostream - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "iostream - Win32 Release" (based on\
 "Win32 (x86) Dynamic-Link Library")
!MESSAGE "iostream - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 
################################################################################
!IF  "$(CFG)" == "iostream - Win32 Release"
LIB = \
   "C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\lib\msvcprt.lib" \
  "C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\lib\MSVCRT.lib" \
  "C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\lib\OLDNAMES.lib" \
  "C:\Program Files (x86)\Microsoft SDKs\Windows\v7.1A\Lib\Uuid.lib" \
  "C:\Program Files (x86)\Microsoft SDKs\Windows\v7.1A\Lib\Kernel32.lib" 
!ELSE
LIB = \
   "C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\lib\msvcprtd.lib" \
  "C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\lib\MSVCRTd.lib" \
  "C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\lib\OLDNAMES.lib" \
  "C:\Program Files (x86)\Microsoft SDKs\Windows\v7.1A\Lib\Uuid.lib" \
  "C:\Program Files (x86)\Microsoft SDKs\Windows\v7.1A\Lib\Kernel32.lib" 
!ENDIF
# CINT SPECIFIC
IPATH      = \
  /I "C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\include" \
  /I "C:\Program Files (x86)\Microsoft SDKs\Windows\v7.1A\Include" \

IPATHCINT      = 
MACRO      = 
MACROCINT      = -D __VC13__ 
CHEADER    = 
CHEADERCINT = 

CPPHEADER  = \
		iostrm.h \
		fstrm.h \
		strstrm.h \
		LinkDef.h 
CPPHEADERCINT  = \
		iostrm.h \
		fstrm.h \
		strstrm.h \
		LinkDef.h 

CSTUB    = 
CSTUBCINT = 

CPPSTUB    = 
CPPSTUBCINT = 

DLLSPEC       =
CIFC       =
CIFH       =
CIFO       =
CPPIFC     = G__cpp_iostream.cxx
CPPIFH     = G__cpp_iostream.h
CPPIFO     = G__cpp_iostream.obj

################################################################################
# Begin Project
MTL=mktyplib.exe
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "iostream - Win32 Release"

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

ALL : "$(OUTDIR)\iostream.dll"

CLEAN : 
	-@erase ".\Release\iostream.dll"
	-@erase ".\iostream.def"
	-@erase ".\Release\G__cpp_iostream.obj"
	-@erase ".\G__cpp_iostream.cxx"
	-@erase ".\G__cpp_iostream.h"
	-@erase ".\Release\iostream.lib"
	-@erase ".\Release\iostream.exp"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /EHsc -D __VC13__ /O2 /I "$(CINTSYSDIR)" /I "$(CINTSYSDIR)/src" /D "NDEBUG" /D\
 "WIN32" /D "_WINDOWS" /D "G__VISUAL" /D "G__WIN32" /D "G__SHAREDLIB"\
 /Fp"$(INTDIR)/iostream.pch" /Fo"$(INTDIR)/" /c $(IPATH) $(MACRO)
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
BSC32_FLAGS=/nologo /o"$(OUTDIR)/iostream.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /machine:I386
LINK32_FLAGS=$(LIB) /nologo \
 /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)/iostream.pdb"\
 /machine:I386 /def:".\iostream.def" /out:"$(OUTDIR)/iostream.dll" /implib:"$(OUTDIR)/iostream.lib" 
DEF_FILE= .\iostream.def
LINK32_OBJS= \
	".\Release\G__cpp_iostream.obj" \
	"c:\cint\LIBCINT.lib"

"$(OUTDIR)\iostream.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<
   copy "$(OUTDIR)\iostream.dll" iostream.dll

!ELSEIF  "$(CFG)" == "iostream - Win32 Debug"

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

ALL : "$(OUTDIR)\iostream.dll"

CLEAN : 
	-@erase ".\Debug\iostream.dll"
	-@erase ".\iostream.def"
	-@erase ".\Debug\G__cpp_iostream.obj"
	-@erase ".\G__cpp_iostream.cxx"
	-@erase ".\G__cpp_iostream.h"
	-@erase ".\Debug\iostream.ilk"
	-@erase ".\Debug\iostream.lib"
	-@erase ".\Debug\iostream.exp"
	-@erase ".\Debug\iostream.pdb"
	-@erase ".\Debug\vc40.pdb"
	-@erase ".\Debug\vc40.idb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /EHsc -D __VC13__ /Zi /Od /I "$(CINTSYSDIR)" /I "$(CINTSYSDIR)/src" /D\
 "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "G__VISUAL" /D "G__WIN32" /D\
 "G__SHAREDLIB" /Fp"$(INTDIR)/iostream.pch" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/"\
 /c $(IPATH) $(MACRO)
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
BSC32_FLAGS=/nologo /o"$(OUTDIR)/iostream.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /debug /machine:I386
LINK32_FLAGS=$(LIB) /nologo\
 /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)/iostream.pdb" /debug\
 /machine:I386 /def:".\iostream.def" /out:"$(OUTDIR)/iostream.dll" /implib:"$(OUTDIR)/iostream.lib" 
DEF_FILE= .\iostream.def
LINK32_OBJS= \
	".\Debug\G__cpp_iostream.obj" \
	"c:\cint\LIBCINT.lib"

"$(OUTDIR)\iostream.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<
   copy "$(OUTDIR)\iostream.dll" iostream.dll

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

# Name "iostream - Win32 Release"
# Name "iostream - Win32 Debug"

!IF  "$(CFG)" == "iostream - Win32 Release"

!ELSEIF  "$(CFG)" == "iostream - Win32 Debug"

!ENDIF 

################################################################################
################################################################################
# Begin Source File

SOURCE=.\G__cpp_iostream.cxx
DEP_CPP_G__CP=\
	".\G__cpp_iostream.h"\
	
NODEP_CPP_G__CP=\
	"c:\cint\G__ci.h"\
	

"$(INTDIR)\G__cpp_iostream.obj" : $(SOURCE) $(DEP_CPP_G__CP) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

################################################################################
$(CPPIFC) : $(CPPHEADER) $(CPPSTUB)
	cint   -w1 -ziostream -n$(CPPIFC) $(DLLSPEC) -D__MAKECINT__  -c-1 -A $(IPATHCINT) $(MACROCINT) $(CPPHEADERCINT)

.\iostream.def : $(CPPHEADER) $(CPPSTUB)
	cint   -w1 -ziostream -n$(CPPIFC) $(DLLSPEC) -D__MAKECINT__  -c-1 -A $(IPATHCINT) $(MACROCINT) $(CPPHEADERCINT)

# End Source File
################################################################################
# Begin Source File

SOURCE=c:\cint\LIBCINT.lib

!IF  "$(CFG)" == "iostream - Win32 Release"

!ELSEIF  "$(CFG)" == "iostream - Win32 Debug"

!ENDIF 

# End Source File
# End Target
# End Project
################################################################################

