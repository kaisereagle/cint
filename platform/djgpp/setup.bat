REM # CINT setup script for DJGPP

copy Makefile ..\..\src\Makefile
copy MAKEINFO ..\..\MAKEINFO
copy %DJGPPDIR%\lib\crt0.o ..\..\src\crt0.o
cd ..\..\src
make -f Makefile clean
make -f Makefile
REM # Use gmake if there is another make.exe probably from Borland C++
REM # gmake -f Makefile clean
REM # gmake -f Makefile

rem cd ..\platform\msdos


