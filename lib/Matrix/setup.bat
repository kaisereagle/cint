
makecint -mk makevcnet -dl NVector.dll -H LinkDef.h 
make -f makevcnet
move NVector.dll %CINTSYSDIR%\include\NVector.dll 

