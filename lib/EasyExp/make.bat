
nmake CFG="EasyExp - Win32 Release" %1 %2 %3 %4 %5 %6
del EasyExp.dll
move Release\EasyExp.dll .\EasyExp.dll

