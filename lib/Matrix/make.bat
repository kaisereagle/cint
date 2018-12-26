
nmake CFG="NVector - Win32 Release" %1 %2 %3 %4 %5 %6
del NVector.dll
move Release\NVector.dll .\NVector.dll

