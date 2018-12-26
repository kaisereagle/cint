REM # BUILD LIBCINT.DLL
cd %CINTSYSDIR%\src\libcint
REM nmake -f libcint.mak clean CFG="libcint - Win32 Debug"
nmake -f libcint.mak ALL CFG="libcint - Win32 Debug"
del %CINTSYSDIR%\libcint.dll
del %CINTSYSDIR%\libcint.lib
move %CINTSYSDIR%\src\libcint\Debug\libcint.dll %CINTSYSDIR%\libcint.dll
move %CINTSYSDIR%\src\libcint\Debug\libcint.lib %CINTSYSDIR%\libcint.lib
cd %CINTSYSDIR%\include
mkincld

REM # BUILD CINT.EXE
cd %CINTSYSDIR%\cint
REM nmake -f cint.mak CLEAN CFG="cint - Win32 Debug"
nmake -f cint.mak ALL CFG="cint - Win32 Debug"
del %CINTSYSDIR%\cint.exe
move %CINTSYSDIR%\cint\Debug\cint.exe %CINTSYSDIR%\cint.exe

cd %CINTSYSDIR%
