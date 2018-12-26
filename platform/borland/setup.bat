rem # CINT setup script for Borland C++ builder 3.0

REM # SET CINTSYSDIR TEMPOLARILLY
SET CINTSYSDIR=C:\CINT

rem # Make libcint.dll , libcint.lib ##################################
make -f libcint.bpr
move libcint.dll ..\..\libcint.dll
move libcint.lib ..\..\libcint.lib

rem # Make cint.exe ###################################################
make -f cint.bpr
move cint.exe ..\..\cint.exe

rem # Make makecint.exe ###############################################
make -f makecint.bpr
move makecint.exe ..\..\makecint.exe

rem # Compile mkincld.exe and run it in include dir to make stdio.h ###
make -f mkincld.bpr
move mkincld.exe ..\..\include\mkincld.exe
cd ..\..\include
mkincld.exe
cd ..\platform\borland

rem # Copy platform dependency files ##################################
copy MAKEINFO ..\..\MAKEINFO
copy MAKEINF2 ..\..\MAKEINF2

rem # Remove files ####################################################
rem make -f libcint.bpr clean
rem make -f cint.bpr clean
rem make -f makecint.bpr clean
rem make -f mkincld.bpr clean

rem # Add environment variables to autoexec.bat ######################
%CINTSYSDIR%\CINT autoexec.c
SET PATH="%CINTSYSDIR%;%PATH%"

rem # Build option library ###########################################
rem # Failing following build is not fatal .
cd ..\..\lib\win32api
%CINTSYSDIR%\CINT -x main(){system(\"setupbc.bat\");}
cd ..\posix
%CINTSYSDIR%\CINT -x main(){system(\"setupbc.bat\");}
cd ..\socket
%CINTSYSDIR%\CINT -x main(){system(\"setupbc.bat\");}
rem cd ..\longlong
rem %CINTSYSDIR%\CINT -x main(){system(\"setupbc.bat\");}
cd ..\stdstrct
%CINTSYSDIR%\CINT -x main(){system(\"setupbc.bat\");}
cd ..\dll_stl
%CINTSYSDIR%\CINT -x main(){system(\"setupbc.bat\");}
rem # Failing above build is not fatal .

cd ..\..\platform\borland

echo off
echo #
echo ##########################################################
echo # AUTOEXEC.bat may have been modified. 
echo # --- Reboot Windows ---
echo ##########################################################
echo #
