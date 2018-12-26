copy ../../include/ReadF.h ReadF.h
copy ../../include/ReadF.cxx ReadF.cxx
makecint -mk makelinux -dl ReadF.dll -H ReadF.h -C++ ReadF.cxx
make -f makelinux
mv ReadF.dll ../../include/ReadF.dll
make -f makelinux clean


