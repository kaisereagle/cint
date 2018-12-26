
copy %CINTSYSDIR%\include\ReadF.cxx ReadF.cxx
copy %CINTSYSDIR%\include\ReadF.h ReadF.h
copy %CINTSYSDIR%\include\MultiByteChar.h MultiByteChar.h

nmake -f makeroot.vcnet clean
nmake -f makeroot.vcnet 
move ReadF.dll %ROOTSYS%\cint\include\ReadF.dll
