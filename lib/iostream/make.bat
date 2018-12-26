
nmake CFG="iostream - Win32 Release" %1 %2 %3 %4 %5 %6
del iostream.dll
move Release\iostream.dll .\iostream.dll

