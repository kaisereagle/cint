cl -o mksockh.exe mksockh.c 
mksockh
del mksockh.exe
del mksockh.obj
del mksockh.def

move %cintsysdir%\include\winsock.h %cintsysdir%\include\_winsock.h

rem FOR VC6.0 or later
makecint -mk Makefile -dl cintsock.dll -h cintsock.h -C cintsock.c -l "c:\Program Files\Microsoft Visual Studio\VC98\Lib\wsock32.lib" -cint -Z0

rem FOR VC5.0 or later
rem makecint -mk Makefile -dl cintsock.dll -h cintsock.h -C cintsock.c -l "c:\Program Files\DevStudio\Vc\Lib\wsock32.lib" -cint -Z0

rem FOR VC4.2 or older
rem makecint -mk Makefile -dl cintsock.dll -h cintsock.h -C cintsock.c -l "c:\msdev\Lib\wsock32.lib"

nmake -f Makefile CFG="cintsock - Win32 Release"
del ..\..\include\cintsock.dll
move Release\cintsock.dll %cintsysdir%\include\cintsock.dll
move %cintsysdir%\include\_winsock.h %cintsysdir%\include\winsock.h
echo off
rem echo #
rem echo #####################################
rem echo # Answer YES to following questions #
rem echo #####################################
rem echo #
rem del Release
rem rmdir Release
del Makefile
del G__*
del *.def
del make.bat
