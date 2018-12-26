echo off
REM ###########################################################################
REM # platform\symantec\setup.bat
REM #
REM # CINT (C/C++ interpreter) installation script for Symantec C++ 7.2
REM #
REM ###########################################################################

REM # SET CINTSYSDIR TEMPOLARILLY
SET CINTSYSDIR=C:\CINT

REM # COPY PROJECT FILES
del %CINTSYSDIR%\MAKEINFO
del %CINTSYSDIR%\make.bat
copy %CINTSYSDIR%\platform\symantec\MAKEINFO %CINTSYSDIR%\MAKEINFO
copy %CINTSYSDIR%\platform\symantec\make.bat %CINTSYSDIR%\make.bat

copy %CINTSYSDIR%\platform\symantec\cint.mak %CINTSYSDIR%\cint.mak
copy %CINTSYSDIR%\platform\symantec\cint.opn %CINTSYSDIR%\cint.opn
copy %CINTSYSDIR%\platform\symantec\cint.def %CINTSYSDIR%\cint.def
copy %CINTSYSDIR%\platform\symantec\cint.lnk %CINTSYSDIR%\cint.lnk
copy %CINTSYSDIR%\platform\symantec\cint.prj %CINTSYSDIR%\cint.prj

copy %CINTSYSDIR%\platform\symantec\src\libcint.def %CINTSYSDIR%\src\libcint.def
copy %CINTSYSDIR%\platform\symantec\src\libcint.lnk %CINTSYSDIR%\src\libcint.lnk
copy %CINTSYSDIR%\platform\symantec\src\libcint.mak %CINTSYSDIR%\src\libcint.mak
copy %CINTSYSDIR%\platform\symantec\src\libcint.opn %CINTSYSDIR%\src\libcint.opn
copy %CINTSYSDIR%\platform\symantec\src\libcint.prj %CINTSYSDIR%\src\libcint.prj

copy %CINTSYSDIR%\platform\symantec\include\mkincld.def %CINTSYSDIR%\include\mkincld.def
copy %CINTSYSDIR%\platform\symantec\include\mkincld.lnk %CINTSYSDIR%\include\mkincld.lnk
copy %CINTSYSDIR%\platform\symantec\include\mkincld.mak %CINTSYSDIR%\include\mkincld.mak
copy %CINTSYSDIR%\platform\symantec\include\mkincld.opn %CINTSYSDIR%\include\mkincld.opn
copy %CINTSYSDIR%\platform\symantec\include\mkincld.prj %CINTSYSDIR%\include\mkincld.prj

REM # BUILD LIBCINT.DLL
cd %CINTSYSDIR%\src
smake -f libcint.mak clean
smake -f libcint.mak
del %CINTSYSDIR%\LIBCINT.DLL
del %CINTSYSDIR%\LIBCINT.LIB
move %CINTSYSDIR%\src\LIBCINT.DLL %CINTSYSDIR%\LIBCINT.DLL
move %CINTSYSDIR%\src\LIBCINT.LIB %CINTSYSDIR%\LIBCINT.LIB

REM # BUILD CINT.EXE
cd %CINTSYSDIR%
smake -f cint.mak clean
smake -f cint.mak

REM # Create standard include files by mkincld program
cd %CINTSYSDIR%\include
smake -f mkincld.mak clean
smake -f mkincld.mak
mkincld

REM # BUILD MAKECINT.EXE
cd %CINTSYSDIR%\platform\symantec\tool
smake -f makecint.mak clean
smake -f makecint.mak
del %CINTSYSDIR%\makecint.exe
move makecint.exe %CINTSYSDIR%\makecint.exe

REM # go back to original directory 
cd %CINTSYSDIR%\platform\symantec

REM # add environment variables to autoexec.bat
%CINTSYSDIR%\CINT autoexec.c

cd %CINTSYSDIR%
echo End of CINT installation

