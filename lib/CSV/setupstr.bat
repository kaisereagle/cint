copy %CINTSYSDIR%\include\EasyExp.cxx EasyExp.cxx
copy %CINTSYSDIR%\include\EasyExp.h EasyExp.h
copy %CINTSYSDIR%\include\ReadF.cxx ReadF.cxx
copy %CINTSYSDIR%\include\ReadF.h ReadF.h
copy %CINTSYSDIR%\include\MultiByteChar.h MultiByteChar.h
copy %CINTSYSDIR%\lib\posix\winposix.c winposix.c
copy %CINTSYSDIR%\lib\posix\winposix.h winposix.h

makecint -mk Makevcnet -dl excel.dll -I %CINTSYSDIR% -D __CINTLIB__  -H Excel.h ReadF.h EasyExp.h Stat.h Value.h LinkDef.h StrCollection.h -C++ Excel.cxx ReadF.cxx EasyExp.cxx Stat.cxx Value.cxx StrCollection.cxx -C winposix.c

nmake CFG="excel - Win32 Release" -f makevcnet
del excel.dll
move Release\excel.dll ..\..\include\excel.dll

del release\*
#del make.bat
del G__*
rmdir release
