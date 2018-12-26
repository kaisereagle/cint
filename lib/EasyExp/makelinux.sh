copy ../../include/RegExp.h RegExp.h
copy ../../include/RegExp.cxx RegExp.cxx
makecint -mk makelinux -dl RegExp.dll -H RegExp.h -C++ RegExp.cxx
make -f makelinux
mv RegExp.dll ../../include/RegExp.dll
make -f makelinux clean
