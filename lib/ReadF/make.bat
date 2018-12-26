
nmake CFG="ReadF - Win32 Release" %1 %2 %3 %4 %5 %6
del ReadF.dll
move Release\ReadF.dll .\ReadF.dll

