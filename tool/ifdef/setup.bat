Rem copy ..\..\src\winnt.c winnt.c

nmake -f ifdefvc13.mak
move ifdef.exe %CINTSYSDIR%\ifdef.exe
nmake -f ifdefvc13.mak clean
