# Microsoft Developer Studio Project File - Name="libcint" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=libcint - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "libcint.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "libcint.mak" CFG="libcint - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "libcint - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "libcint - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "libcint - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\Release"
# PROP BASE Intermediate_Dir ".\Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\Release"
# PROP Intermediate_Dir ".\Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MD /w /W0 /GX /O2 /I "..\.." /I ".." /I "..\..\lib\iccstrm" /I "..\..\lib\stdstrct" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "G__NEWSTDHEADER" /D "G__VISUAL" /D "G__WIN32" /D "G__SHAREDLIB" /D "G__REDIRECTIO" /D "G__P2FCAST" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib /nologo /subsystem:windows /dll /machine:I386

!ELSEIF  "$(CFG)" == "libcint - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\Debug"
# PROP BASE Intermediate_Dir ".\Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\Debug"
# PROP Intermediate_Dir ".\Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\.." /I ".." /I "..\..\lib\iccstrm" /I "..\..\lib\stdstrct" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "G__NEWSTDHEADER" /D "G__VISUAL" /D "G__WIN32" /D "G__SHAREDLIB" /D "G__REDIRECTIO" /D "G__P2FCAST" /D "G__ASM_DBG" /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib /nologo /subsystem:windows /dll /debug /machine:I386

!ENDIF 

# Begin Target

# Name "libcint - Win32 Release"
# Name "libcint - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat;for;f90"
# Begin Source File

SOURCE=..\API.CXX
# End Source File
# Begin Source File

SOURCE=..\APIIF.CXX
# End Source File
# Begin Source File

SOURCE=..\AUXU.C
# End Source File
# Begin Source File

SOURCE=..\BASECLS.CXX
# End Source File
# Begin Source File

SOURCE=..\BC_ASSIGN.CXX	
# End Source File
# Begin Source File

SOURCE=..\BC_CFUNC.CXX	
# End Source File
# Begin Source File

SOURCE=..\BC_EH.CXX	
# End Source File
# Begin Source File

SOURCE=..\BC_INST.CXX	
# End Source File
# Begin Source File

SOURCE=..\BC_READER.CXX	
# End Source File
# Begin Source File

SOURCE=..\BC_VTBL.CXX
# End Source File
# Begin Source File

SOURCE=..\BC_AUTOOBJ.CXX	
# End Source File
# Begin Source File

SOURCE=..\BC_DEBUG.CXX	
# End Source File
# Begin Source File

SOURCE=..\BC_EXEC.CXX	
# End Source File
# Begin Source File

SOURCE=..\BC_PARSE.CXX	
# End Source File
# Begin Source File

SOURCE=..\BC_TYPE.CXX
# End Source File
# Begin Source File

SOURCE=..\CALLFUNC.CXX
# End Source File
# Begin Source File

SOURCE=..\CAST.C
# End Source File
# Begin Source File

SOURCE=..\CLASS.CXX
# End Source File
# Begin Source File

SOURCE=..\DATAMBR.CXX
# End Source File
# Begin Source File

SOURCE=..\DEBUG.C
# End Source File
# Begin Source File

SOURCE=..\DECL.C
# End Source File
# Begin Source File

SOURCE=..\DISP.C
# End Source File
# Begin Source File

SOURCE=..\DUMP.C
# End Source File
# Begin Source File

SOURCE=..\END.C
# End Source File
# Begin Source File

SOURCE=..\ERROR.C
# End Source File
# Begin Source File

SOURCE=..\EXPR.C
# End Source File
# Begin Source File

SOURCE=..\FREAD.C
# End Source File
# Begin Source File

SOURCE=..\FUNC.C
# End Source File
# Begin Source File

SOURCE=..\G__CFUNC.C
# End Source File
# Begin Source File

SOURCE=..\..\MAIN\G__SETUP.C
# End Source File
# Begin Source File

SOURCE=..\GCOLL.C
# End Source File
# Begin Source File

SOURCE=..\GLOBAL1.C
# End Source File
# Begin Source File

SOURCE=..\GLOBAL2.C
# End Source File
# Begin Source File

SOURCE=..\IFUNC.C
# End Source File
# Begin Source File

SOURCE=..\INHERIT.C
# End Source File
# Begin Source File

SOURCE=..\INIT.C
# End Source File
# Begin Source File

SOURCE=..\INPUT.C
# End Source File
# Begin Source File

SOURCE=..\INTRPT.C
# End Source File
# Begin Source File

SOURCE=.\libcint.def
# End Source File
# Begin Source File

SOURCE=..\LOADFILE.C
# End Source File
# Begin Source File

SOURCE=..\MACRO.C
# End Source File
# Begin Source File

SOURCE=..\MALLOC.C
# End Source File
# Begin Source File

SOURCE=..\MEMTEST.C
# End Source File
# Begin Source File

SOURCE=..\METHOD.CXX
# End Source File
# Begin Source File

SOURCE=..\METHODAR.CXX
# End Source File
# Begin Source File

SOURCE=..\NEW.C
# End Source File
# Begin Source File

SOURCE=..\NEWLINK.C
# End Source File
# Begin Source File

SOURCE=..\OLDLINK.C
# End Source File
# Begin Source File

SOURCE=..\OPR.C
# End Source File
# Begin Source File

SOURCE=..\PARSE.C
# End Source File
# Begin Source File

SOURCE=..\PAUSE.C
# End Source File
# Begin Source File

SOURCE=..\PCODE.C
# End Source File
# Begin Source File

SOURCE=..\PRAGMA.C
# End Source File
# Begin Source File

SOURCE=..\QUOTE.C
# End Source File
# Begin Source File

SOURCE=..\SCRUPTO.C
# End Source File
# Begin Source File

SOURCE=..\SHL.C
# End Source File
# Begin Source File

SOURCE=..\SIZEOF.C
# End Source File
# Begin Source File

SOURCE=..\STDSTRCT.C
# End Source File
# Begin Source File

SOURCE=..\STRUCT.C
# End Source File
# Begin Source File

SOURCE=..\STUB.C
# End Source File
# Begin Source File

SOURCE=..\TMPLT.C
# End Source File
# Begin Source File

SOURCE=..\Token.cxx
# End Source File
# Begin Source File

SOURCE=..\TYPE.CXX
# End Source File
# Begin Source File

SOURCE=..\TYPEDEF.C
# End Source File
# Begin Source File

SOURCE=..\TYPEDF.CXX
# End Source File
# Begin Source File

SOURCE=..\VAL2A.C
# End Source File
# Begin Source File

SOURCE=..\VALUE.C
# End Source File
# Begin Source File

SOURCE=..\VAR.C
# End Source File
# Begin Source File

SOURCE=..\iccstrm.cxx
# End Source File
# Begin Source File

SOURCE=..\WINNT.C
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=..\Api.h
# End Source File
# Begin Source File

SOURCE=..\Apiif.h
# End Source File
# Begin Source File

SOURCE=..\BaseCls.h
# End Source File
# Begin Source File

SOURCE=..\bc_assign.h	
# End Source File
# Begin Source File

SOURCE=..\bc_cfunc.h	
# End Source File
# Begin Source File

SOURCE=..\bc_eh.h		
# End Source File
# Begin Source File

SOURCE=..\bc_exec_asm.h	
# End Source File
# Begin Source File

SOURCE=..\bc_linkdef.h	
# End Source File
# Begin Source File

SOURCE=..\bc_reader.h	
# End Source File
# Begin Source File

SOURCE=..\bc_vtbl.h
# End Source File
# Begin Source File

SOURCE=..\bc_autoobj.h	
# End Source File
# Begin Source File

SOURCE=..\bc_debug.h	
# End Source File
# Begin Source File

SOURCE=..\bc_exec.h	
# End Source File
# Begin Source File

SOURCE=..\bc_inst.h	
# End Source File
# Begin Source File

SOURCE=..\bc_parse.h	
# End Source File
# Begin Source File

SOURCE=..\bc_type.h
# End Source File
# Begin Source File

SOURCE=..\CallFunc.h
# End Source File
# Begin Source File

SOURCE=..\Class.h
# End Source File
# Begin Source File

SOURCE=..\DataMbr.h
# End Source File
# Begin Source File

SOURCE=..\memtest.h
# End Source File
# Begin Source File

SOURCE=..\Method.h
# End Source File
# Begin Source File

SOURCE=..\MethodAr.h
# End Source File
# Begin Source File

SOURCE=..\stdstrct.h
# End Source File
# Begin Source File

SOURCE=..\Token.h
# End Source File
# Begin Source File

SOURCE=..\Type.h
# End Source File
# Begin Source File

SOURCE=..\Typedf.h
# End Source File
# Begin Source File

SOURCE=..\vcstrm.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
