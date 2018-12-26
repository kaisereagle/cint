
copy ..\..\include\EasyExp.h EasyExp.h
copy ..\..\include\EasyExp.cxx EasyExp.cxx
copy %CINTSYSDIR%\include\MultiByteChar.h MultiByteChar.h
copy %CINTSYSDIR%\lib\posix\winposix.c winposix.c
copy %CINTSYSDIR%\lib\posix\winposix.h winposix.h

nmake -f makeroot.vcnet clean
nmake -f makeroot.vcnet

