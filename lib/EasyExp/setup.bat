
copy ..\..\include\EasyExp.h EasyExp.h
copy ..\..\include\EasyExp.cxx EasyExp.cxx
copy ..\..\include\ReadF.h ReadF.h
copy ..\..\include\ReadF.cxx ReadF.cxx
copy ..\..\include\MultiByteChar.h MultiByteChar.h
copy ..\..\include\MultiByteChar.cxx MultiByteChar.cxx
copy ..\posix\winposix.c winposix.c
copy ..\posix\winposix.h winposix.h
copy ..\..\lib\NVector\Stat.h Stat.h
copy ..\..\lib\NVector\Stat.cxx Stat.cxx

makecint -mk makevcnet -dl EasyExp.dll -H EasyExp.h LinkDef.h -C++ Stat.cxx ReadF.cxx EasyExp.cxx MultiByteChar.cxx -C winposix.c
nmake -f makevcnet CFG="EasyExp - Win32 Release"
move Release\EasyExp.dll ..\..\include\EasyExp.dll

nmake -f makevcnet CFG="EasyExp - Win32 Release" clean
del *.dll
del Release\*
rmdir Release
