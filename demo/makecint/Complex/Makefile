# Microsoft Developer Studio Generated NMAKE File, Format Version 4.00
# ** DO NOT EDIT **

CFG=Complex - Win32 Release

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

!IF "$(CFG)" == ""
CFG=Complex - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to Complex - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "Complex - Win32 Release" && "$(CFG)" !=\
 "Complex - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "Complex.mak" CFG="Complex - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Complex - Win32 Release" (based on\
 "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Complex - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 
################################################################################
# CINT SPECIFIC
IPATH  = /I "C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\include" \
         /I "C:\Program Files (x86)\Microsoft SDKs\Windows\v7.1A\Include" 
IPATHCINT      = 
MACRO      = 
MACROCINT      = 
CHEADER    = 
CHEADERCINT = 

CPPHEADER  = \
		Complex.h 
CPPHEADERCINT  = \
		Complex.h 

CSTUB    = 
CSTUBCINT = 

CPPSTUB    = 
CPPSTUBCINT = 

DLLSPEC       =
CIFC       =
CIFH       =
CIFO       =
CPPIFC     = G__cpp_Complex.cxx
CPPIFH     = G__cpp_Complex.h
CPPIFO     = G__cpp_Complex.obj

################################################################################
# Begin Project
MTL=mktyplib.exe
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Complex - Win32 Release"

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

ALL : "$(OUTDIR)\Complex.dll"

CLEAN : 
	-@erase ".\Release\Complex.dll"
	-@erase ".\Release\Complex.obj"
	-@erase ".\Complex.def"
	-@erase ".\Release\G__cpp_Complex.obj"
	-@erase ".\G__cpp_Complex.cxx"
	-@erase ".\G__cpp_Complex.h"
	-@erase ".\Release\Complex.lib"
	-@erase ".\Release\Complex.exp"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "$(CINTSYSDIR)" /I "$(CINTSYSDIR)/src" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "G__VISUAL" /D "G__WIN32" /D "G__SHAREDLIB" /YX /c
CPP_PROJ=/nologo /MT /W3 /EHsc /D __VC13__ $(IPATH) /O2 /I "$(CINTSYSDIR)" /I "$(CINTSYSDIR)/src" /D "NDEBUG" /D\
 "WIN32" /D "_WINDOWS" /D "G__VISUAL" /D "G__WIN32" /D "G__SHAREDLIB"\
 /Fp"$(INTDIR)/Complex.pch" /Fo"$(INTDIR)/" /c $(IPATH) $(MACRO)
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
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Complex.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo\
 /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)/Complex.pdb"\
 /machine:I386 /def:".\Complex.def" /out:"$(OUTDIR)/Complex.dll" /implib:"$(OUTDIR)/Complex.lib" 
DEF_FILE= .\Complex.def
LINK32_OBJS= \
	".\Release\Complex.obj" \
	".\Release\G__cpp_Complex.obj" \
	"c:\cint\LIBCINT.lib"

"$(OUTDIR)\Complex.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Complex - Win32 Debug"

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

ALL : "$(OUTDIR)\Complex.dll"

CLEAN : 
	-@erase ".\Debug\Complex.dll"
	-@erase ".\Debug\Complex.obj"
	-@erase ".\Complex.def"
	-@erase ".\Debug\G__cpp_Complex.obj"
	-@erase ".\G__cpp_Complex.cxx"
	-@erase ".\G__cpp_Complex.h"
	-@erase ".\Debug\Complex.ilk"
	-@erase ".\Debug\Complex.lib"
	-@erase ".\Debug\Complex.exp"
	-@erase ".\Debug\Complex.pdb"
	-@erase ".\Debug\vc40.pdb"
	-@erase ".\Debug\vc40.idb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /I "$(CINTSYSDIR)" /I "$(CINTSYSDIR)/src" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "G__VISUAL" /D "G__WIN32" /D "G__SHAREDLIB" /YX /c
CPP_PROJ=/nologo /MTd /W3 /Gm /EHsc /D __VC13__ $(IPATH) /Zi /Od /I "$(CINTSYSDIR)" /I "$(CINTSYSDIR)/src" /D\
 "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "G__VISUAL" /D "G__WIN32" /D\
 "G__SHAREDLIB" /Fp"$(INTDIR)/Complex.pch" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/"\
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
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Complex.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /debug /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo\
 /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)/Complex.pdb" /debug\
 /machine:I386 /def:".\Complex.def" /out:"$(OUTDIR)/Complex.dll" /implib:"$(OUTDIR)/Complex.lib" 
DEF_FILE= .\Complex.def
LINK32_OBJS= \
	".\Debug\Complex.obj" \
	".\Debug\G__cpp_Complex.obj" \
	"c:\cint\LIBCINT.lib"

"$(OUTDIR)\Complex.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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

# Name "Complex - Win32 Release"
# Name "Complex - Win32 Debug"

!IF  "$(CFG)" == "Complex - Win32 Release"

!ELSEIF  "$(CFG)" == "Complex - Win32 Debug"

!ENDIF 

################################################################################
################################################################################
# Begin Source File

SOURCE=.\Complex.cxx
DEP_CPP_COMPL=$(CPPHEADER)
	

"$(INTDIR)\Complex.obj" : $(SOURCE) $(DEP_CPP_COMPL) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\G__cpp_Complex.cxx
DEP_CPP_G__CP=\
	".\G__cpp_Complex.h"\
	
NODEP_CPP_G__CP=\
	"c:\cint\G__ci.h"\
	

"$(INTDIR)\G__cpp_Complex.obj" : $(SOURCE) $(DEP_CPP_G__CP) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

################################################################################
$(CPPIFC) : $(CPPHEADER) $(CPPSTUB)
	cint   -w1 -zComplex -n$(CPPIFC) $(DLLSPEC) -D__MAKECINT__  -c-1 -A $(IPATHCINT) $(MACROCINT) $(CPPHEADERCINT)

.\Complex.def : $(CPPHEADER) $(CPPSTUB)
	cint   -w1 -zComplex -n$(CPPIFC) $(DLLSPEC) -D__MAKECINT__  -c-1 -A $(IPATHCINT) $(MACROCINT) $(CPPHEADERCINT)

# End Source File
################################################################################
# Begin Source File

SOURCE=c:\cint\LIBCINT.lib

!IF  "$(CFG)" == "Complex - Win32 Release"

!ELSEIF  "$(CFG)" == "Complex - Win32 Debug"

!ENDIF 

# End Source File
# End Target
# End Project
################################################################################

