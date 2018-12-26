
makecint -mk Makestdfunc -dl stdfunc.dll -h stdfunc.h -cint -Z0
make.exe -f Makestdfunc 
move Release\stdfunc.dll ..\..\include/stdfunc.dll
del G__*
del Makestdfunc

