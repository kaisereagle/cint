echo off
REM ###########################################################################
REM # platform\visualCpp6\setup.bat
REM #
REM # CINT (C/C++ interpreter) installation script for Visual C++ 6.0 or later
REM #
REM ###########################################################################

REM # SET CINTSYSDIR TEMPOLARILLY
SET CINTSYSDIR=C:\CINT

REM # COPY MAKEINFO
del %CINTSYSDIR%\MAKEINFO
del %CINTSYSDIR%\make.bat
copy %CINTSYSDIR%\platform\visualCpp6\MAKEINFO %CINTSYSDIR%\MAKEINFO
copy %CINTSYSDIR%\platform\visualCpp6\update.bat %CINTSYSDIR%\update.bat
copy %CINTSYSDIR%\platform\visualCpp6\updatedb.bat %CINTSYSDIR%\updatedb.bat

REM # COPY PROJECT FILES
mkdir %CINTSYSDIR%\cint
copy %CINTSYSDIR%\platform\visualCpp6\cint.mak %CINTSYSDIR%\cint\cint.mak
copy %CINTSYSDIR%\platform\visualCpp6\cint.dsw %CINTSYSDIR%\cint\cint.dsw
copy %CINTSYSDIR%\platform\visualCpp6\cint.dsp %CINTSYSDIR%\cint\cint.dsp

mkdir %CINTSYSDIR%\src\libcint
copy %CINTSYSDIR%\platform\visualCpp6\src\libcint.mak %CINTSYSDIR%\src\libcint\libcint.mak
copy %CINTSYSDIR%\platform\visualCpp6\src\libcint.def %CINTSYSDIR%\src\libcint\libcint.def
copy %CINTSYSDIR%\platform\visualCpp6\src\libcint.dsw %CINTSYSDIR%\src\libcint\libcint.dsw
copy %CINTSYSDIR%\platform\visualCpp6\src\libcint.dsp %CINTSYSDIR%\src\libcint\libcint.dsp

mkdir %CINTSYSDIR%\include\mkincld
copy %CINTSYSDIR%\platform\visualCpp6\include\mkincld.mak %CINTSYSDIR%\include\mkincld\mkincld.mak
copy %CINTSYSDIR%\platform\visualCpp6\include\mkincld.mdp %CINTSYSDIR%\include\mkincld\mkincld.mdp

REM # BUILD LIBCINT.DLL
cd %CINTSYSDIR%\src\libcint
REM nmake -f libcint.mak clean CFG="libcint - Win32 Release"
nmake -f libcint.mak ALL CFG="libcint - Win32 Release"
del %CINTSYSDIR%\libcint.dll
del %CINTSYSDIR%\libcint.lib
move %CINTSYSDIR%\src\libcint\Release\libcint.dll %CINTSYSDIR%\libcint.dll
move %CINTSYSDIR%\src\libcint\Release\libcint.lib %CINTSYSDIR%\libcint.lib

REM # BUILD CINT.EXE
cd %CINTSYSDIR%\cint
REM nmake -f cint.mak CLEAN CFG="cint - Win32 Release"
nmake -f cint.mak ALL CFG="cint - Win32 Release"
del %CINTSYSDIR%\cint.exe
move %CINTSYSDIR%\cint\Release\cint.exe %CINTSYSDIR%\cint.exe

REM ########################################################################
REM # Build aux library
REM ########################################################################
cd %CINTSYSDIR%\platform\visualCpp6
%CINTSYSDIR%\cint.exe setup.cxx -p visualCpp6

echo #
echo #####################################
echo # Answer YES to following questions #
echo #####################################
echo #

echo Remiving %CINTSYSDIR%\cint\Release
del %CINTSYSDIR%\cint\Release
rmdir %CINTSYSDIR%\cint\Release

echo Removing %CINTSYSDIR%\src\libcint\Release
del %CINTSYSDIR%\src\libcint\Release
rmdir %CINTSYSDIR%\src\libcint\Release

echo Removing %CINTSYSDIR%\include\mkincld\Release
del %CINTSYSDIR%\include\mkincld\Release
rmdir %CINTSYSDIR%\include\mkincld\Release

echo Removing %CINTSYSDIR%\platform\visualCpp6\tool\makecint\Release
del %CINTSYSDIR%\platform\visualCpp6\tool\makecint\Release
rmdir %CINTSYSDIR%\platform\visualCpp6\tool\makecint\Release

echo Removing %CINTSYSDIR%\lib\longlong\Release
del %CINTSYSDIR%\lib\longlong\Release
rmdir %CINTSYSDIR%\lib\longlong\Release

echo Removing %CINTSYSDIR%\lib\win32api\Release
del %CINTSYSDIR%\lib\win32api\Release
rmdir %CINTSYSDIR%\lib\win32api\Release

echo Removing %CINTSYSDIR%\lib\posix\Release
del %CINTSYSDIR%\lib\posix\Release
rmdir %CINTSYSDIR%\lib\posix\Release

echo Removing %CINTSYSDIR%\lib\socket\Release
del %CINTSYSDIR%\lib\socket\Release
rmdir %CINTSYSDIR%\lib\socket\Release

echo Removing %CINTSYSDIR%\lib\dll_stl\Release
del %CINTSYSDIR%\lib\dll_stl\Release
rmdir %CINTSYSDIR%\lib\dll_stl\Release

echo #
echo ##########################################################
REM echo # AUTOEXEC.bat may have been modified. 
REM echo # --- Reboot Windows ---
echo # Modify AUTOEXEC.bat to add following lines, then Reboot
echo # set CINTSYSDIR=c:\cint
echo # set PATH=%CINTSYSDIR%;%PATH
echo ##########################################################
echo #

