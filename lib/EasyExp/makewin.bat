
copy ..\..\include\RegExp.h RegExp.h
copy ..\..\include\RegExp.cxx RegExp.cxx

makecint -mk makevcnet -dl RegExp.dll -H RegExp.h -C++ RegExp.cxx
make -f makevcnet
move RegExp.dll ..\..\include\RegExp.dll
make -f makevcnet clean
del Release\*
rmdir Release
