REM # BUILD LIBCINT.DLL
cd %CINTSYSDIR%\src\libcint
REM nmake -f libcint.mak clean CFG="libcint - Win32 Release"
nmake -f libcint.mak ALL CFG="libcint - Win32 Release"
del %CINTSYSDIR%\libcint.dll
del %CINTSYSDIR%\libcint.lib
move %CINTSYSDIR%\src\libcint\Release\libcint.dll %CINTSYSDIR%\libcint.dll
move %CINTSYSDIR%\src\libcint\Release\libcint.lib %CINTSYSDIR%\libcint.lib
cd %CINTSYSDIR%\include
mkincld
cd %CINTSYSDIR%
