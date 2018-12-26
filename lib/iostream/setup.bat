makecint -mk makefile -dl iostream.dll -H iostrm.h fstrm.h strstrm.h LinkDef.h
nmake -f makefile CFG="iostream - Win32 Release"
move iostream.dll ..\..\include\iostream.dll
nmake -f makefile CFG="iostream - Win32 Release" clean
del Release\*
del #*
rmdir Release

