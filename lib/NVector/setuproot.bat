copy %CINTSYSDIR%\lib\NVector\Histogram.h Histogram.h
copy %CINTSYSDIR%\lib\NVector\Vector.h Vector.h
copy %CINTSYSDIR%\lib\NVector\LinkDef.h LinkDef.h
nmake -f makeroot.vcnet clean
nmake -f makeroot.vcnet
move libNVector.dll %ROOTSYS%\cint\include\NVector.dll
