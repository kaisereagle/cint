REM # setup script for Borland C++ compiler 5.5 ##########################

REM # Copy files #########################################################
copy Makefile ..\..\Makefile
copy MAKEINFO ..\..\MAKEINFO
copy src\Makefile ..\..\src\Makefile
copy lib\Makefile ..\..\lib\Makefile

REM # Build libcint.dll and libcint.lib #################################
cd ..\..
make.exe -f Makefile clean
make.exe -f Makefile

echo off
echo #
echo ##########################################################
echo # AUTOEXEC.bat may have been modified. 
echo # --- Reboot Windows ---
echo ##########################################################
echo #

