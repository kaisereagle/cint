
copy %CINTSYSDIR%\lib\Excel\Excel.h Excel.h
copy %CINTSYSDIR%\lib\Excel\Excel.cxx Excel.cxx
copy %CINTSYSDIR%\lib\Excel\StrCollection.h StrCollection.h
copy %CINTSYSDIR%\lib\Excel\StrCollection.cxx StrCollection.cxx
copy %CINTSYSDIR%\lib\Excel\Clustering.cxx Clustering.cxx 
copy %CINTSYSDIR%\lib\Excel\Clustering.h Clustering.h 
copy %CINTSYSDIR%\lib\Excel\Stat.h Stat.h 
copy %CINTSYSDIR%\lib\Excel\Stat.cxx Stat.cxx 
copy %CINTSYSDIR%\include\EasyExp.cxx EasyExp.cxx
copy %CINTSYSDIR%\include\EasyExp.h EasyExp.h
copy %CINTSYSDIR%\include\StatEasyExp.cxx StatEasyExp.cxx
copy %CINTSYSDIR%\include\StatEasyExp.h StatEasyExp.h
copy %CINTSYSDIR%\include\MultiByteChar.cxx MultiByteChar.cxx
copy %CINTSYSDIR%\include\MultiByteChar.h MultiByteChar.h
copy %CINTSYSDIR%\include\EasyExp.h %ROOTSYS%\cint\include\EasyExp.h
copy %CINTSYSDIR%\include\ReadF.cxx ReadF.cxx
copy %CINTSYSDIR%\include\ReadF.h ReadF.h
copy %CINTSYSDIR%\lib\Excel\LinkDef.h LinkDef.h
copy %CINTSYSDIR%\lib\Excel\makeroot.vcnet makeroot.vcnet
copy %CINTSYSDIR%\lib\posix\winposix.c winposix.c
copy %CINTSYSDIR%\lib\posix\winposix.h winposix.h
copy %CINTSYSDIR%\lib\NVector\Vector.h Vector.h
copy %CINTSYSDIR%\lib\NVector\Histogram.h Histogram.h

nmake -f makeroot.vcnet clean
nmake -f makeroot.vcnet
move libExcel.dll %ROOTSYS%\cint\include\excel.dll


