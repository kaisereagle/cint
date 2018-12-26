
del ..\..\stl\vector.dll
makecint -mk Makevec -dl vector.dll -H vec.h  -cint -Z0
nmake -f Makevec CFG="vector - Win32 Release"
move Release\vector.dll ..\..\stl\vector.dll
