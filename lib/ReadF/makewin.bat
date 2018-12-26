
copy ..\..\include\ReadF.h ReadF.h
copy ..\..\include\ReadF.cxx ReadF.cxx

makecint -mk makevcnet -dl ReadF.dll -H ReadF.h -C++ ReadF.cxx
nmake -f makevcnet CFG="ReadF - Win32 Release"
move Release\ReadF.dll ..\..\include\ReadF.dll
make -f makevcnet clean
del Release\*
rmdir Release


