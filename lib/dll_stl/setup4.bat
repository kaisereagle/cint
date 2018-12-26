

del ..\..\stl\string.dll
makecint -mk Makestr -dl string.dll -H str.h  -cint -Z0
nmake -f Makestr CFG="string - Win32 Release"
move Release\string.dll ..\..\stl\string.dll
