
nmake CFG="Complex - Win32 Release" %1 %2 %3 %4 %5 %6
del Complex.dll
move Release\Complex.dll .\Complex.dll

