copy ..\..\lib\NVector\Vector.h Vector.h
copy ..\..\lib\NVector\Complex.h Complex.h

makecint -mk makevcnet -dll Kmeans_EM_VB_FCM.dll -I/Tools/Eigen -H Vector.h MatrixCalc.h Kmeans_EM_VB_FCM.h LinkDef.h 
nmake -f makevcnet CFG="Kmeans_EM_VB_FCM - Win32 Release"
move Release\Kmeans_EM_VB_FCM.dll ..\..\include\Kmeans_EM_VB_FCM.dll
nmake -f makevcnet CFG="Kmeans_EM_VB_FCM - Win32 Release" clean
del G__*
del *.dll
del Release\*
rmdir Release
