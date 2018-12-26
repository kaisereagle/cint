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

CFG=cint - Win32 Release
##################################################################

# TARGTYPE "Win32 (x86) Console Application" 0x0103

!IF "$(CFG)" == ""
CFG=cint - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to cint - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "cint - Win32 Release" && "$(CFG)" != "cint - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "cint.mak" CFG="cint - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "cint - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "cint - Win32 Debug" (based on "Win32 (x86) Console Application")
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
# PROP Target_Last_Scanned "cint - Win32 Debug"
RSC=rc.exe
CPP=cl.exe

!IF  "$(CFG)" == "cint - Win32 Release"

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

ALL : "$(OUTDIR)\cint.exe"

CLEAN : 
	-@erase ".\Release\cint.exe"
	-@erase ".\Release\CPPMAIN.OBJ"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MT /W3 /EHsc /D __VC13__ $(IPATH) /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/cint.pch"  /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/cint.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib /nologo /subsystem:console /machine:I386
LINK32_FLAGS=$(LIB) /nologo /subsystem:console /incremental:no\
 /pdb:"$(OUTDIR)/cint.pdb" /machine:I386 /out:"$(OUTDIR)/cint.exe" 
LINK32_OBJS= \
	"$(INTDIR)/CPPMAIN.OBJ" \
	"..\LIBCINT.lib"

"$(OUTDIR)\cint.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "cint - Win32 Debug"

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

ALL : "$(OUTDIR)\cint.exe"

CLEAN : 
	-@erase ".\Debug\vc40.pdb"
	-@erase ".\Debug\vc40.idb"
	-@erase ".\Debug\cint.exe"
	-@erase ".\Debug\CPPMAIN.OBJ"
	-@erase ".\Debug\cint.ilk"
	-@erase ".\Debug\cint.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MTd /W3 /Gm /EHsc /D __VC13__ $(IPATH) /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/cint.pch"  /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/cint.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=$(LIB) /nologo /subsystem:console\
 /incremental:yes /pdb:"$(OUTDIR)/cint.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/cint.exe" 
LINK32_OBJS= \
	"$(INTDIR)/CPPMAIN.OBJ" \
	"..\LIBCINT.lib"

"$(OUTDIR)\cint.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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

# Name "cint - Win32 Release"
# Name "cint - Win32 Debug"

!IF  "$(CFG)" == "cint - Win32 Release"

!ELSEIF  "$(CFG)" == "cint - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=..\MAIN\CPPMAIN.CXX

"$(INTDIR)\CPPMAIN.OBJ" : $(SOURCE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=..\LIBCINT.lib

!IF  "$(CFG)" == "cint - Win32 Release"

!ELSEIF  "$(CFG)" == "cint - Win32 Debug"

!ENDIF 

# End Source File
# End Target
# End Project
################################################################################
