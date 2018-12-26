
del ..\..\stl\map.dll
del ..\..\stl\map2.dll

makecint -mk Makemap -dl map.dll -H mp.h  -cint -Z0
nmake -f Makemap CFG="map - Win32 Release"

makecint -mk Makemap2 -dl map2.dll -DG__MAP2 -H mp.h  -cint -Z0
nmake -f Makemap2 CFG="map2 - Win32 Release"

move Release\map.dll ..\..\stl\map.dll
move Release\map2.dll ..\..\stl\map2.dll

#del ..\..\stl\vector.dll
#makecint -mk Makevec -dl vector.dll -H vec.h  -cint -Z0
#nmake -f Makevec CFG="vector - Win32 Release"
#move Release\vector.dll ..\..\stl\vector.dll
