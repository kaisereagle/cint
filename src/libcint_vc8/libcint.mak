# Microsoft Developer Studio Generated NMAKE File, Based on libcint.dsp
!IF "$(CFG)" == ""
CFG=libcint - Win32 Release
!MESSAGE No configuration specified. Defaulting to libcint - Win32 Release.
!ENDIF 

!IF "$(CFG)" != "libcint - Win32 Release" && "$(CFG)" != "libcint - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
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
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "libcint - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\libcint.dll"


CLEAN :
	-@erase "$(INTDIR)\API.OBJ"
	-@erase "$(INTDIR)\APIIF.OBJ"
	-@erase "$(INTDIR)\AUXU.OBJ"
	-@erase "$(INTDIR)\BASECLS.OBJ"
	-@erase "$(INTDIR)\bc_assign.obj"
   	-@erase "$(INTDIR)\bc_cfunc.OBJ"
	-@erase "$(INTDIR)\bc_eh.obj"
	-@erase "$(INTDIR)\bc_inst.obj"
	-@erase "$(INTDIR)\bc_reader.OBJ"
	-@erase "$(INTDIR)\bc_vtbl.obj"
	-@erase "$(INTDIR)\bc_autoobj.OBJ"
	-@erase "$(INTDIR)\bc_debug.OBJ"
	-@erase "$(INTDIR)\bc_exec.OBJ"
	-@erase "$(INTDIR)\bc_parse.OBJ"
	-@erase "$(INTDIR)\bc_type.obj"
	-@erase "$(INTDIR)\CALLFUNC.OBJ"
	-@erase "$(INTDIR)\CAST.OBJ"
	-@erase "$(INTDIR)\CLASS.OBJ"
	-@erase "$(INTDIR)\DATAMBR.OBJ"
	-@erase "$(INTDIR)\DEBUG.OBJ"
	-@erase "$(INTDIR)\DECL.OBJ"
	-@erase "$(INTDIR)\DISP.OBJ"
	-@erase "$(INTDIR)\DUMP.OBJ"
	-@erase "$(INTDIR)\END.OBJ"
	-@erase "$(INTDIR)\ERROR.OBJ"
	-@erase "$(INTDIR)\EXPR.OBJ"
	-@erase "$(INTDIR)\FREAD.OBJ"
	-@erase "$(INTDIR)\FUNC.OBJ"
	-@erase "$(INTDIR)\G__CFUNC.OBJ"
	-@erase "$(INTDIR)\G__SETUP.OBJ"
	-@erase "$(INTDIR)\GCOLL.OBJ"
	-@erase "$(INTDIR)\GLOBAL1.OBJ"
	-@erase "$(INTDIR)\GLOBAL2.OBJ"
	-@erase "$(INTDIR)\IFUNC.OBJ"
	-@erase "$(INTDIR)\INHERIT.OBJ"
	-@erase "$(INTDIR)\INIT.OBJ"
	-@erase "$(INTDIR)\INPUT.OBJ"
	-@erase "$(INTDIR)\INTRPT.OBJ"
	-@erase "$(INTDIR)\LOADFILE.OBJ"
	-@erase "$(INTDIR)\MACRO.OBJ"
	-@erase "$(INTDIR)\MALLOC.OBJ"
	-@erase "$(INTDIR)\MEMTEST.OBJ"
	-@erase "$(INTDIR)\METHOD.OBJ"
	-@erase "$(INTDIR)\METHODAR.OBJ"
	-@erase "$(INTDIR)\NEW.OBJ"
	-@erase "$(INTDIR)\NEWLINK.OBJ"
	-@erase "$(INTDIR)\OLDLINK.OBJ"
	-@erase "$(INTDIR)\OPR.OBJ"
	-@erase "$(INTDIR)\PARSE.OBJ"
	-@erase "$(INTDIR)\PAUSE.OBJ"
	-@erase "$(INTDIR)\PCODE.OBJ"
	-@erase "$(INTDIR)\PRAGMA.OBJ"
	-@erase "$(INTDIR)\QUOTE.OBJ"
	-@erase "$(INTDIR)\SCRUPTO.OBJ"
	-@erase "$(INTDIR)\SHL.OBJ"
	-@erase "$(INTDIR)\SIZEOF.OBJ"
	-@erase "$(INTDIR)\STDSTRCT.OBJ"
	-@erase "$(INTDIR)\STRUCT.OBJ"
	-@erase "$(INTDIR)\STUB.OBJ"
	-@erase "$(INTDIR)\TMPLT.OBJ"
	-@erase "$(INTDIR)\Token.obj"
	-@erase "$(INTDIR)\TYPE.OBJ"
	-@erase "$(INTDIR)\TYPEDEF.OBJ"
	-@erase "$(INTDIR)\TYPEDF.OBJ"
	-@erase "$(INTDIR)\VAL2A.OBJ"
	-@erase "$(INTDIR)\VALUE.OBJ"
	-@erase "$(INTDIR)\VAR.OBJ"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc7strm.obj"
	-@erase "$(INTDIR)\WINNT.OBJ"
	-@erase "$(OUTDIR)\libcint.dll"
	-@erase "$(OUTDIR)\libcint.exp"
	-@erase "$(OUTDIR)\libcint.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /w /W0 /GX /O2 /I "..\.." /I ".." /I "..\..\lib\vc7strm" /I "..\..\lib\stdstrct" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "G__NEWSTDHEADER" /D "G__VISUAL" /D "G__WIN32" /D "G__SHAREDLIB" /D "G__REDIRECTIO" /D "G__P2FCAST" /Fp"$(INTDIR)\libcint.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\libcint.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib /nologo /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\libcint.pdb" /machine:I386 /def:".\libcint.def" /out:"$(OUTDIR)\libcint.dll" /implib:"$(OUTDIR)\libcint.lib" 
DEF_FILE= \
	".\libcint.def"
LINK32_OBJS= \
	"$(INTDIR)\API.OBJ" \
	"$(INTDIR)\APIIF.OBJ" \
	"$(INTDIR)\AUXU.OBJ" \
	"$(INTDIR)\BASECLS.OBJ" \
	"$(INTDIR)\bc_assign.obj" \
   	"$(INTDIR)\bc_cfunc.OBJ" \
	"$(INTDIR)\bc_eh.obj" \
	"$(INTDIR)\bc_inst.obj" \
	"$(INTDIR)\bc_reader.OBJ" \
	"$(INTDIR)\bc_vtbl.obj" \
	"$(INTDIR)\bc_autoobj.OBJ" \
	"$(INTDIR)\bc_debug.OBJ" \
	"$(INTDIR)\bc_exec.OBJ" \
	"$(INTDIR)\bc_parse.OBJ" \
	"$(INTDIR)\bc_type.obj" \
	"$(INTDIR)\CALLFUNC.OBJ" \
	"$(INTDIR)\CAST.OBJ" \
	"$(INTDIR)\CLASS.OBJ" \
	"$(INTDIR)\DATAMBR.OBJ" \
	"$(INTDIR)\DEBUG.OBJ" \
	"$(INTDIR)\DECL.OBJ" \
	"$(INTDIR)\DISP.OBJ" \
	"$(INTDIR)\DUMP.OBJ" \
	"$(INTDIR)\END.OBJ" \
	"$(INTDIR)\ERROR.OBJ" \
	"$(INTDIR)\EXPR.OBJ" \
	"$(INTDIR)\FREAD.OBJ" \
	"$(INTDIR)\FUNC.OBJ" \
	"$(INTDIR)\G__CFUNC.OBJ" \
	"$(INTDIR)\G__SETUP.OBJ" \
	"$(INTDIR)\GCOLL.OBJ" \
	"$(INTDIR)\GLOBAL1.OBJ" \
	"$(INTDIR)\GLOBAL2.OBJ" \
	"$(INTDIR)\IFUNC.OBJ" \
	"$(INTDIR)\INHERIT.OBJ" \
	"$(INTDIR)\INIT.OBJ" \
	"$(INTDIR)\INPUT.OBJ" \
	"$(INTDIR)\INTRPT.OBJ" \
	"$(INTDIR)\LOADFILE.OBJ" \
	"$(INTDIR)\MACRO.OBJ" \
	"$(INTDIR)\MALLOC.OBJ" \
	"$(INTDIR)\MEMTEST.OBJ" \
	"$(INTDIR)\METHOD.OBJ" \
	"$(INTDIR)\METHODAR.OBJ" \
	"$(INTDIR)\NEW.OBJ" \
	"$(INTDIR)\NEWLINK.OBJ" \
	"$(INTDIR)\OLDLINK.OBJ" \
	"$(INTDIR)\OPR.OBJ" \
	"$(INTDIR)\PARSE.OBJ" \
	"$(INTDIR)\PAUSE.OBJ" \
	"$(INTDIR)\PCODE.OBJ" \
	"$(INTDIR)\PRAGMA.OBJ" \
	"$(INTDIR)\QUOTE.OBJ" \
	"$(INTDIR)\SCRUPTO.OBJ" \
	"$(INTDIR)\SHL.OBJ" \
	"$(INTDIR)\SIZEOF.OBJ" \
	"$(INTDIR)\STDSTRCT.OBJ" \
	"$(INTDIR)\STRUCT.OBJ" \
	"$(INTDIR)\STUB.OBJ" \
	"$(INTDIR)\TMPLT.OBJ" \
	"$(INTDIR)\Token.obj" \
	"$(INTDIR)\TYPE.OBJ" \
	"$(INTDIR)\TYPEDEF.OBJ" \
	"$(INTDIR)\TYPEDF.OBJ" \
	"$(INTDIR)\VAL2A.OBJ" \
	"$(INTDIR)\VALUE.OBJ" \
	"$(INTDIR)\VAR.OBJ" \
	"$(INTDIR)\vc7strm.obj" \
	"$(INTDIR)\WINNT.OBJ"

"$(OUTDIR)\libcint.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "libcint - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\libcint.dll"


CLEAN :
	-@erase "$(INTDIR)\API.OBJ"
	-@erase "$(INTDIR)\APIIF.OBJ"
	-@erase "$(INTDIR)\AUXU.OBJ"
	-@erase "$(INTDIR)\BASECLS.OBJ"
	-@erase "$(INTDIR)\bc_assign.obj"
   	-@erase "$(INTDIR)\bc_cfunc.OBJ"
	-@erase "$(INTDIR)\bc_eh.obj"
	-@erase "$(INTDIR)\bc_inst.obj"
	-@erase "$(INTDIR)\bc_reader.OBJ"
	-@erase "$(INTDIR)\bc_vtbl.obj"
	-@erase "$(INTDIR)\bc_autoobj.OBJ"
	-@erase "$(INTDIR)\bc_debug.OBJ"
	-@erase "$(INTDIR)\bc_exec.OBJ"
	-@erase "$(INTDIR)\bc_parse.OBJ"
	-@erase "$(INTDIR)\bc_type.obj"
	-@erase "$(INTDIR)\CALLFUNC.OBJ"
	-@erase "$(INTDIR)\CAST.OBJ"
	-@erase "$(INTDIR)\CLASS.OBJ"
	-@erase "$(INTDIR)\DATAMBR.OBJ"
	-@erase "$(INTDIR)\DEBUG.OBJ"
	-@erase "$(INTDIR)\DECL.OBJ"
	-@erase "$(INTDIR)\DISP.OBJ"
	-@erase "$(INTDIR)\DUMP.OBJ"
	-@erase "$(INTDIR)\END.OBJ"
	-@erase "$(INTDIR)\ERROR.OBJ"
	-@erase "$(INTDIR)\EXPR.OBJ"
	-@erase "$(INTDIR)\FREAD.OBJ"
	-@erase "$(INTDIR)\FUNC.OBJ"
	-@erase "$(INTDIR)\G__CFUNC.OBJ"
	-@erase "$(INTDIR)\G__SETUP.OBJ"
	-@erase "$(INTDIR)\GCOLL.OBJ"
	-@erase "$(INTDIR)\GLOBAL1.OBJ"
	-@erase "$(INTDIR)\GLOBAL2.OBJ"
	-@erase "$(INTDIR)\IFUNC.OBJ"
	-@erase "$(INTDIR)\INHERIT.OBJ"
	-@erase "$(INTDIR)\INIT.OBJ"
	-@erase "$(INTDIR)\INPUT.OBJ"
	-@erase "$(INTDIR)\INTRPT.OBJ"
	-@erase "$(INTDIR)\LOADFILE.OBJ"
	-@erase "$(INTDIR)\MACRO.OBJ"
	-@erase "$(INTDIR)\MALLOC.OBJ"
	-@erase "$(INTDIR)\MEMTEST.OBJ"
	-@erase "$(INTDIR)\METHOD.OBJ"
	-@erase "$(INTDIR)\METHODAR.OBJ"
	-@erase "$(INTDIR)\NEW.OBJ"
	-@erase "$(INTDIR)\NEWLINK.OBJ"
	-@erase "$(INTDIR)\OLDLINK.OBJ"
	-@erase "$(INTDIR)\OPR.OBJ"
	-@erase "$(INTDIR)\PARSE.OBJ"
	-@erase "$(INTDIR)\PAUSE.OBJ"
	-@erase "$(INTDIR)\PCODE.OBJ"
	-@erase "$(INTDIR)\PRAGMA.OBJ"
	-@erase "$(INTDIR)\QUOTE.OBJ"
	-@erase "$(INTDIR)\SCRUPTO.OBJ"
	-@erase "$(INTDIR)\SHL.OBJ"
	-@erase "$(INTDIR)\SIZEOF.OBJ"
	-@erase "$(INTDIR)\STDSTRCT.OBJ"
	-@erase "$(INTDIR)\STRUCT.OBJ"
	-@erase "$(INTDIR)\STUB.OBJ"
	-@erase "$(INTDIR)\TMPLT.OBJ"
	-@erase "$(INTDIR)\Token.obj"
	-@erase "$(INTDIR)\TYPE.OBJ"
	-@erase "$(INTDIR)\TYPEDEF.OBJ"
	-@erase "$(INTDIR)\TYPEDF.OBJ"
	-@erase "$(INTDIR)\VAL2A.OBJ"
	-@erase "$(INTDIR)\VALUE.OBJ"
	-@erase "$(INTDIR)\VAR.OBJ"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\vc7strm.obj"
	-@erase "$(INTDIR)\WINNT.OBJ"
	-@erase "$(OUTDIR)\libcint.dll"
	-@erase "$(OUTDIR)\libcint.exp"
	-@erase "$(OUTDIR)\libcint.ilk"
	-@erase "$(OUTDIR)\libcint.lib"
	-@erase "$(OUTDIR)\libcint.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\.." /I ".." /I "..\..\lib\vc7strm" /I "..\..\lib\stdstrct" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "G__NEWSTDHEADER" /D "G__VISUAL" /D "G__WIN32" /D "G__SHAREDLIB" /D "G__REDIRECTIO" /D "G__P2FCAST" /D "G__ASM_DBG" /Fp"$(INTDIR)\libcint.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\libcint.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib /nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\libcint.pdb" /debug /machine:I386 /def:".\libcint.def" /out:"$(OUTDIR)\libcint.dll" /implib:"$(OUTDIR)\libcint.lib" 
DEF_FILE= \
	".\libcint.def"
LINK32_OBJS= \
	"$(INTDIR)\API.OBJ" \
	"$(INTDIR)\APIIF.OBJ" \
	"$(INTDIR)\AUXU.OBJ" \
	"$(INTDIR)\BASECLS.OBJ" \
	"$(INTDIR)\bc_assign.obj" \
   	"$(INTDIR)\bc_cfunc.OBJ" \
	"$(INTDIR)\bc_eh.obj" \
	"$(INTDIR)\bc_inst.obj" \
	"$(INTDIR)\bc_reader.OBJ" \
	"$(INTDIR)\bc_vtbl.obj" \
	"$(INTDIR)\bc_autoobj.OBJ" \
	"$(INTDIR)\bc_debug.OBJ" \
	"$(INTDIR)\bc_exec.OBJ" \
	"$(INTDIR)\bc_parse.OBJ" \
	"$(INTDIR)\bc_type.obj" \
	"$(INTDIR)\CALLFUNC.OBJ" \
	"$(INTDIR)\CAST.OBJ" \
	"$(INTDIR)\CLASS.OBJ" \
	"$(INTDIR)\DATAMBR.OBJ" \
	"$(INTDIR)\DEBUG.OBJ" \
	"$(INTDIR)\DECL.OBJ" \
	"$(INTDIR)\DISP.OBJ" \
	"$(INTDIR)\DUMP.OBJ" \
	"$(INTDIR)\END.OBJ" \
	"$(INTDIR)\ERROR.OBJ" \
	"$(INTDIR)\EXPR.OBJ" \
	"$(INTDIR)\FREAD.OBJ" \
	"$(INTDIR)\FUNC.OBJ" \
	"$(INTDIR)\G__CFUNC.OBJ" \
	"$(INTDIR)\G__SETUP.OBJ" \
	"$(INTDIR)\GCOLL.OBJ" \
	"$(INTDIR)\GLOBAL1.OBJ" \
	"$(INTDIR)\GLOBAL2.OBJ" \
	"$(INTDIR)\IFUNC.OBJ" \
	"$(INTDIR)\INHERIT.OBJ" \
	"$(INTDIR)\INIT.OBJ" \
	"$(INTDIR)\INPUT.OBJ" \
	"$(INTDIR)\INTRPT.OBJ" \
	"$(INTDIR)\LOADFILE.OBJ" \
	"$(INTDIR)\MACRO.OBJ" \
	"$(INTDIR)\MALLOC.OBJ" \
	"$(INTDIR)\MEMTEST.OBJ" \
	"$(INTDIR)\METHOD.OBJ" \
	"$(INTDIR)\METHODAR.OBJ" \
	"$(INTDIR)\NEW.OBJ" \
	"$(INTDIR)\NEWLINK.OBJ" \
	"$(INTDIR)\OLDLINK.OBJ" \
	"$(INTDIR)\OPR.OBJ" \
	"$(INTDIR)\PARSE.OBJ" \
	"$(INTDIR)\PAUSE.OBJ" \
	"$(INTDIR)\PCODE.OBJ" \
	"$(INTDIR)\PRAGMA.OBJ" \
	"$(INTDIR)\QUOTE.OBJ" \
	"$(INTDIR)\SCRUPTO.OBJ" \
	"$(INTDIR)\SHL.OBJ" \
	"$(INTDIR)\SIZEOF.OBJ" \
	"$(INTDIR)\STDSTRCT.OBJ" \
	"$(INTDIR)\STRUCT.OBJ" \
	"$(INTDIR)\STUB.OBJ" \
	"$(INTDIR)\TMPLT.OBJ" \
	"$(INTDIR)\Token.obj" \
	"$(INTDIR)\TYPE.OBJ" \
	"$(INTDIR)\TYPEDEF.OBJ" \
	"$(INTDIR)\TYPEDF.OBJ" \
	"$(INTDIR)\VAL2A.OBJ" \
	"$(INTDIR)\VALUE.OBJ" \
	"$(INTDIR)\VAR.OBJ" \
	"$(INTDIR)\vc7strm.obj" \
	"$(INTDIR)\WINNT.OBJ"

"$(OUTDIR)\libcint.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("libcint.dep")
!INCLUDE "libcint.dep"
!ELSE 
!MESSAGE Warning: cannot find "libcint.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "libcint - Win32 Release" || "$(CFG)" == "libcint - Win32 Debug"
SOURCE=..\API.CXX

"$(INTDIR)\API.OBJ" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\APIIF.CXX

"$(INTDIR)\APIIF.OBJ" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\AUXU.C

"$(INTDIR)\AUXU.OBJ" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=..\bc_assign.cxx

"$(INTDIR)\bc_assign.OBJ" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\bc_cfunc.cxx

"$(INTDIR)\bc_cfunc.OBJ" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\bc_eh.cxx

"$(INTDIR)\bc_eh.OBJ" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\bc_inst.cxx

"$(INTDIR)\bc_inst.OBJ" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\bc_reader.cxx

"$(INTDIR)\bc_reader.OBJ" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\bc_vtbl.cxx

"$(INTDIR)\bc_vtbl.OBJ" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\bc_autoobj.cxx

"$(INTDIR)\bc_autoobj.OBJ" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\bc_debug.cxx

"$(INTDIR)\bc_debug.OBJ" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\bc_exec.cxx

"$(INTDIR)\bc_exec.OBJ" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\bc_parse.cxx

"$(INTDIR)\bc_parse.OBJ" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\bc_type.cxx

"$(INTDIR)\bc_type.OBJ" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)



SOURCE=..\BASECLS.CXX

"$(INTDIR)\BASECLS.OBJ" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\CALLFUNC.CXX

"$(INTDIR)\CALLFUNC.OBJ" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\CAST.C

"$(INTDIR)\CAST.OBJ" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\CLASS.CXX

"$(INTDIR)\CLASS.OBJ" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\DATAMBR.CXX

"$(INTDIR)\DATAMBR.OBJ" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\DEBUG.C

"$(INTDIR)\DEBUG.OBJ" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\DECL.C

"$(INTDIR)\DECL.OBJ" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\DISP.C

"$(INTDIR)\DISP.OBJ" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\DUMP.C

"$(INTDIR)\DUMP.OBJ" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\END.C

"$(INTDIR)\END.OBJ" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\ERROR.C

"$(INTDIR)\ERROR.OBJ" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\EXPR.C

"$(INTDIR)\EXPR.OBJ" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\FREAD.C

"$(INTDIR)\FREAD.OBJ" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\FUNC.C

"$(INTDIR)\FUNC.OBJ" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\G__CFUNC.C

"$(INTDIR)\G__CFUNC.OBJ" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\MAIN\G__SETUP.C

"$(INTDIR)\G__SETUP.OBJ" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\GCOLL.C

"$(INTDIR)\GCOLL.OBJ" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\GLOBAL1.C

"$(INTDIR)\GLOBAL1.OBJ" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\GLOBAL2.C

"$(INTDIR)\GLOBAL2.OBJ" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\IFUNC.C

"$(INTDIR)\IFUNC.OBJ" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\INHERIT.C

"$(INTDIR)\INHERIT.OBJ" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\INIT.C

"$(INTDIR)\INIT.OBJ" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\INPUT.C

"$(INTDIR)\INPUT.OBJ" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\INTRPT.C

"$(INTDIR)\INTRPT.OBJ" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\LOADFILE.C

"$(INTDIR)\LOADFILE.OBJ" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\MACRO.C

"$(INTDIR)\MACRO.OBJ" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\MALLOC.C

"$(INTDIR)\MALLOC.OBJ" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\MEMTEST.C

"$(INTDIR)\MEMTEST.OBJ" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\METHOD.CXX

"$(INTDIR)\METHOD.OBJ" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\METHODAR.CXX

"$(INTDIR)\METHODAR.OBJ" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\NEW.C

"$(INTDIR)\NEW.OBJ" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\NEWLINK.C

"$(INTDIR)\NEWLINK.OBJ" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\OLDLINK.C

"$(INTDIR)\OLDLINK.OBJ" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\OPR.C

"$(INTDIR)\OPR.OBJ" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\PARSE.C

"$(INTDIR)\PARSE.OBJ" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\PAUSE.C

"$(INTDIR)\PAUSE.OBJ" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\PCODE.C

"$(INTDIR)\PCODE.OBJ" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\PRAGMA.C

"$(INTDIR)\PRAGMA.OBJ" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\QUOTE.C

"$(INTDIR)\QUOTE.OBJ" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\SCRUPTO.C

"$(INTDIR)\SCRUPTO.OBJ" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\SHL.C

"$(INTDIR)\SHL.OBJ" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\SIZEOF.C

"$(INTDIR)\SIZEOF.OBJ" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\STDSTRCT.C

"$(INTDIR)\STDSTRCT.OBJ" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\STRUCT.C

"$(INTDIR)\STRUCT.OBJ" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\STUB.C

"$(INTDIR)\STUB.OBJ" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\TMPLT.C

"$(INTDIR)\TMPLT.OBJ" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Token.cxx

"$(INTDIR)\Token.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\TYPE.CXX

"$(INTDIR)\TYPE.OBJ" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\TYPEDEF.C

"$(INTDIR)\TYPEDEF.OBJ" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\TYPEDF.CXX

"$(INTDIR)\TYPEDF.OBJ" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\VAL2A.C

"$(INTDIR)\VAL2A.OBJ" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\VALUE.C

"$(INTDIR)\VALUE.OBJ" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\VAR.C

"$(INTDIR)\VAR.OBJ" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\vc7strm.cxx

"$(INTDIR)\vc7strm.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\WINNT.C

"$(INTDIR)\WINNT.OBJ" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)



!ENDIF 

