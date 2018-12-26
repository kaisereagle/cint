echo off
REM ###########################################################################
REM # platform\visualCpp13\setup.bat
REM #
REM # CINT (C/C++ interpreter) installation script for Visual C++ 13.0 or later
REM #
REM ###########################################################################

REM # SET CINTSYSDIR TEMPOLARILLY
SET CINTSYSDIR=C:\CINT

REM # COPY MAKEINFO
del %CINTSYSDIR%\MAKEINFO
del %CINTSYSDIR%\make.bat
copy %CINTSYSDIR%\platform\visualCpp13\MAKEINFO %CINTSYSDIR%\MAKEINFO
copy %CINTSYSDIR%\platform\visualCpp13\update.bat %CINTSYSDIR%\update.bat
copy %CINTSYSDIR%\platform\visualCpp13\updatedb.bat %CINTSYSDIR%\updatedb.bat


REM ##########################################################################
REM # Build libcint.dll
REM ##########################################################################
mkdir %CINTSYSDIR%\src\libcint
copy %CINTSYSDIR%\platform\visualCpp13\src\libcintvc13.mak %CINTSYSDIR%\src\libcint\libcintvc13.mak
copy %CINTSYSDIR%\platform\visualCpp13\src\libcintvc13.def %CINTSYSDIR%\src\libcint\libcintvc13.def

REM # BUILD LIBCINT.DLL
cd %CINTSYSDIR%\src\libcint
REM nmake -f libcintvc13.mak clean CFG="libcint - Win32 Release"
nmake -f libcintvc13.mak 
del %CINTSYSDIR%\libcint.dll
del %CINTSYSDIR%\libcint.lib
move %CINTSYSDIR%\src\libcint\Release\libcint.dll %CINTSYSDIR%\libcint.dll
move %CINTSYSDIR%\src\libcint\Release\libcint.lib %CINTSYSDIR%\libcint.lib

REM ##########################################################################
REM # Build cint.exe
REM ##########################################################################
REM # COPY PROJECT FILES
mkdir %CINTSYSDIR%\cintvc13
copy %CINTSYSDIR%\platform\visualCpp13\cintvc13.mak %CINTSYSDIR%\cintvc13\cintvc13.mak

REM # BUILD CINT.EXE
cd %CINTSYSDIR%\cintvc13
REM nmake -f cintvc13.mak CLEAN CFG="cint - Win32 Release"
nmake -f cintvc13.mak 
del %CINTSYSDIR%\cint.exe
move %CINTSYSDIR%\cintvc13\Release\cint.exe %CINTSYSDIR%\cint.exe
nmake -f cintvc13.mak clean
rmdir Release

REM ##########################################################################
REM # Building makecint.exe
REM ##########################################################################
cd %CINTSYSDIR%\platform\VisualCpp13\tool\makecint
nmake -f makecint.mak
copy Release\makecint.exe %CINTSYSDIR%\makecint.exe

REM ##########################################################################
REM # Generate platform dependent include files
REM ##########################################################################
mkdir %CINTSYSDIR%\include\mkincldvc13
copy %CINTSYSDIR%\platform\visualCpp13\include\mkincldvc13.mak %CINTSYSDIR%\include\mkincldvc13\mkincldvc13.mak
cd %CINTSYSDIR%\include\mkincldvc13
nmake -f mkincldvc13.mak
del %CINTSYSDIR%\include\mkincld.exe
move Release\mkincld.exe %CINTSYSDIR%\include\mkincld.exe
nmake -f mkincldvc13.mak clean
rmdir Release
cd $CINTSYSDIR\include
mkincld.exe


REM ########################################################################
REM # Build aux library
REM ########################################################################
cd %CINTSYSDIR%\platform\visualCpp13
%CINTSYSDIR%\cint.exe setup.cxx -p visualCpp13

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

echo Removing %CINTSYSDIR%\platform\visualCpp13\tool\makecint\Release
del %CINTSYSDIR%\platform\visualCpp13\tool\makecint\Release
rmdir %CINTSYSDIR%\platform\visualCpp13\tool\makecint\Release

echo Removing %CINTSYSDIR%\lib\longlong\Release
del %CINTSYSDIR%\lib\longlong\Release
rmdir %CINTSYSDIR%\lib\longlong\Release

echo Removing %CINTSYSDIR%\lib\stdstrct\Release
del %CINTSYSDIR%\lib\stdstrct\Release
rmdir %CINTSYSDIR%\lib\stdstrct\Release

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

