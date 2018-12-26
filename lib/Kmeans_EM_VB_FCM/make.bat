
nmake CFG="Kmeans_EM_VB_FCM - Win32 Release" %1 %2 %3 %4 %5 %6
del Kmeans_EM_VB_FCM.dll
move Release\Kmeans_EM_VB_FCM.dll .\Kmeans_EM_VB_FCM.dll

