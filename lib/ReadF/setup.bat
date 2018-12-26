
copy ..\..\include\ReadF.cxx ReadF.cxx
copy ..\..\include\ReadF.h ReadF.h
copy ..\..\include\MultiByteChar.h MultiByteChar.h
copy ..\..\include\MultiByteChar.cxx MultiByteChar.cxx

makecint -mk makevcnet -dl ReadF.dll -H ReadF.h LinkDef.h -C++ ReadF.cxx MultiByteChar.cxx
nmake -f makevcnet CFG="ReadF - Win32 Release" clean
nmake -f makevcnet CFG="ReadF - Win32 Release"
move Release\ReadF.dll ..\..\\include\ReadF.dll
nmake -f makevcnet CFG="ReadF - Win32 Release" clean
del *.dll
del Release\*
rmdir Release
