REM # setup script for Borland C++ compiler 5.5 ##########################

REM # Copy files #########################################################
copy Makefile ..\..\Makefile
copy MAKEINFO ..\..\MAKEINFO
copy src\Makefile ..\..\src\Makefile
copy src\ldlibcint.bat ..\..\src\ldlibcint.bat

REM # Build libcint.dll and libcint.lib #################################
cd ..\..\src
make.exe -f Makefile clean
make.exe -f Makefile
del libcint.tds
del *.obj

REM # Build cint.exe ####################################################
cd ..
make.exe -f Makefile cint.exe
del cint.tds

REM # Build makecint.exe ################################################
cd platform\borlandcc5
make.exe -f makemakecint clean
make.exe -f makemakecint
del makecint.tds
del *.obj

rem # Add environment variables to autoexec.bat ######################
%CINTSYSDIR%\CINT ..\borland\autoexec.c
SET PATH="%CINTSYSDIR%;%PATH%"

rem # Build option library ###########################################
rem # Failing following build is not fatal .
cd ..\..\lib\win32api
%CINTSYSDIR%\CINT -x main(){system(\"setupbc.bat\");}
cd ..\posix
%CINTSYSDIR%\CINT -x main(){system(\"setupbc.bat\");}
cd ..\socket
%CINTSYSDIR%\CINT -x main(){system(\"setupbc.bat\");}
rem # Compiling long.dll fails due to strange __int64 behavior in BCC5
rem cd ..\longlong
rem %CINTSYSDIR%\CINT -x main(){system(\"setupbc.bat\");}
rem # Compiling stdcxxfunc.dll fails due to lack of math overload function
cd ..\stdstrct
%CINTSYSDIR%\CINT -x main(){system(\"setupbc.bat\");}
cd ..\dll_stl
%CINTSYSDIR%\CINT -x main(){system(\"setupbc.bat\");}
rem # Failing above build is not fatal .

rem # Build ifdef.exe ################################################
cd ..\..\tool\ifdef
make.exe -f Makefile.bcc5
move ifdef.exe ..\..\ifdef.exe
make.exe -f Makefile.bcc5 clean

cd ..\..\platform\borlandcc5

echo off
echo #
echo ##########################################################
echo # AUTOEXEC.bat may have been modified. 
echo # --- Reboot Windows ---
echo ##########################################################
echo #

