# Microsoft Developer Studio Generated NMAKE File, Format Version 13.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

!IF "$(CFG)" == ""
CFG=display0 - Win32 Release
!MESSAGE No configuration specified.  Defaulting to display0 - Win32 Release.
!ENDIF 

!IF "$(CFG)" != "display0 - Win32 Release" && "$(CFG)" !=\
 "display0 - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "display0.mak" CFG="display0 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "display0 - Win32 Release" (based on\
 "Win32 (x86) Dynamic-Link Library")
!MESSAGE "display0 - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 
################################################################################
!IF  "$(CFG)" == "display0 - Win32 Release"
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
CHEADER    = \
		display0.h 
CHEADERCINT = \
		display0.h 

CPPHEADER  = 
CPPHEADERCINT  = 

CSTUB    = 
CSTUBCINT = 

CPPSTUB    = 
CPPSTUBCINT = 

DLLSPEC       =
CIFC       = G__c_display0.c
CIFH       = G__c_display0.h
CIFO       = G__c_display0.obj
CPPIFC     =
CPPIFH     =
CPPIFO     =

################################################################################
# Begin Project
MTL=mktyplib.exe
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "display0 - Win32 Release"

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

ALL : "$(OUTDIR)\display0.dll"

CLEAN : 
	-@erase ".\Release\display0.dll"
	-@erase ".\display0.def"
	-@erase ".\Release\G__c_display0.obj"
	-@erase ".\G__c_display0.c"
	-@erase ".\G__c_display0.h"
	-@erase ".\Release\display0.lib"
	-@erase ".\Release\display0.exp"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /EHsc -D __VC13__ /O2 /I "$(CINTSYSDIR)" /I "$(CINTSYSDIR)/src" /D "NDEBUG" /D\
 "WIN32" /D "_WINDOWS" /D "G__VISUAL" /D "G__WIN32" /D "G__SHAREDLIB"\
 /Fp"$(INTDIR)/display0.pch" /Fo"$(INTDIR)/" /c $(IPATH) $(MACRO)
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
BSC32_FLAGS=/nologo /o"$(OUTDIR)/display0.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /machine:I386
LINK32_FLAGS=$(LIB) /nologo \
 /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)/display0.pdb"\
 /machine:I386 /def:".\display0.def" /out:"$(OUTDIR)/display0.dll" /implib:"$(OUTDIR)/display0.lib" 
DEF_FILE= .\display0.def
LINK32_OBJS= \
	".\Release\G__c_display0.obj" \
	"D:\work\cint\LIBCINT.lib"

"$(OUTDIR)\display0.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<
   copy "$(OUTDIR)\display0.dll" display0.dll

!ELSEIF  "$(CFG)" == "display0 - Win32 Debug"

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

ALL : "$(OUTDIR)\display0.dll"

CLEAN : 
	-@erase ".\Debug\display0.dll"
	-@erase ".\display0.def"
	-@erase ".\Debug\G__c_display0.obj"
	-@erase ".\G__c_display0.c"
	-@erase ".\G__c_display0.h"
	-@erase ".\Debug\display0.ilk"
	-@erase ".\Debug\display0.lib"
	-@erase ".\Debug\display0.exp"
	-@erase ".\Debug\display0.pdb"
	-@erase ".\Debug\vc40.pdb"
	-@erase ".\Debug\vc40.idb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /EHsc -D __VC13__ /Zi /Od /I "$(CINTSYSDIR)" /I "$(CINTSYSDIR)/src" /D\
 "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "G__VISUAL" /D "G__WIN32" /D\
 "G__SHAREDLIB" /Fp"$(INTDIR)/display0.pch" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/"\
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
BSC32_FLAGS=/nologo /o"$(OUTDIR)/display0.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /debug /machine:I386
LINK32_FLAGS=$(LIB) /nologo\
 /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)/display0.pdb" /debug\
 /machine:I386 /def:".\display0.def" /out:"$(OUTDIR)/display0.dll" /implib:"$(OUTDIR)/display0.lib" 
DEF_FILE= .\display0.def
LINK32_OBJS= \
	".\Debug\G__c_display0.obj" \
	"D:\work\cint\LIBCINT.lib"

"$(OUTDIR)\display0.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<
   copy "$(OUTDIR)\display0.dll" display0.dll

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

# Name "display0 - Win32 Release"
# Name "display0 - Win32 Debug"

!IF  "$(CFG)" == "display0 - Win32 Release"

!ELSEIF  "$(CFG)" == "display0 - Win32 Debug"

!ENDIF 

################################################################################
################################################################################
# Begin Source File

SOURCE=.\G__c_display0.c
DEP_CPP_G__CP=\
	".\G__c_display0.h"\
	
NODEP_CPP_G__CP=\
	"D:\work\cint\G__ci.h"\
	

"$(INTDIR)\G__c_display0.obj" : $(SOURCE) $(DEP_CPP_G__CP) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

$(CIFC) : $(CHEADER) $(CSTUB)
	cint   -K -w1 -zdisplay0 -n$(CIFC) $(DLLSPEC) -D__MAKECINT__  -c-2 $(IPATHCINT) $(MACROCINT) $(CHEADERCINT)

.\display0.def : $(CHEADER) $(CSTUB)
	cint   -K -w1 -zdisplay0 -n$(CIFC) $(DLLSPEC) -D__MAKECINT__  -c-2 $(IPATHCINT) $(MACROCINT) $(CHEADERCINT)

# End Source File
################################################################################
# Begin Source File

SOURCE=D:\work\cint\LIBCINT.lib

!IF  "$(CFG)" == "display0 - Win32 Release"

!ELSEIF  "$(CFG)" == "display0 - Win32 Debug"

!ENDIF 

# End Source File
# End Target
# End Project
################################################################################

