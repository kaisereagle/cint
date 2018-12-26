del ..\..\stl\string.dll
del ..\..\stl\vector.dll
del G__*
del *.dll

makecint -mk Makestr -dl string.dll -H str.h  -cint -Z0
nmake -f Makestr CFG="string - Win32 Release"
move Release\string.dll ..\..\stl\string.dll

makecint -mk Makevec -dl vector.dll -H vec.h  -cint -Z0
nmake -f Makevec CFG="vector - Win32 Release"
move Release\vector.dll ..\..\stl\vector.dll

rem del Release\*
rem rmdir Release
rem del G__*
rem del Make*
rem del *.def
