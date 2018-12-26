makecint -mk makemtlib -dl mtlib.dll -H mtlib.h
nmake -f makemtlib
cint mtmain.cxx
#nmake -f makemtlib clean
