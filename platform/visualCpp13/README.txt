platform/visualCpp13/README

 This directory contains Windows-NT Visual C++ project files. You can
 compile CINT on Windows with Visual C++ 13.0.

NOTE: This Visual C++ project file is configured as DLL.  

############################################################################
############################################################################
# Installation
############################################################################
############################################################################

# Compile and Install Cint core

  You must install CINT in c:\cint directory. Make c:\cint directory, copy 
  cint.tgz and unpack by gzip+tar or winzip twice.

        c:\> mkdir c:\cint
        c:\> cd c:\cint
        c:\cint> copy [where_you_get_package] c:\cint\cint.tgz
        c:\cint> gzip -vd cint.tgz
        c:\cint> tar xvf cint.tar

  You need to set environment variable

       CINTSYSDIR=C:\cint
       PATH=%PATH%;%CINTSYSDIR%


 Visual C++ 13.0

   Copy c:\cint\MAKEINFO

        c:\> copy \cint\platform\VisualCpp13\MAKEINFO \cint\MAKEINFO

   Build c:\cint\libcint.dll 

        c:\> cd c:\cint\src\libcint
        c:\> nmake -f libcintvc13.mak
        c:\> copy Release\libcint.dll \cint\libcint.dll
        c:\> copy Release\libcint.lib \cint\libcint.lib
        c:\> del Release\*
        c:\> rmdir Release

   Build c:\cint\cint.exe

        c:\> cd c:\cint\cintvc13
        c:\> nmake -f cintvc13.mak
        c:\> copy Release\cint.exe \cint\cint.exe
        c:\> del Release\*
        c:\> rmdir Release

   Build c:\makecint.exe

        c:\> cd c:\cint\platform\VisualCpp13\tool\makecint
        c:\> nmake -f makecint.mak
        c:\> copy Release\makecint.exe \cint\makecint.exe
        c:\> del Release\*
        c:\> rmdir Release

   Build libraries

        c:\> cd c:\cint\lib\longlong
        c:\> setup.bat

        c:\> cd c:\cint\lib\win32api
        c:\> setup.bat

        c:\> cd c:\cint\lib\posix
        c:\> setup.bat

        c:\> cd c:\cint\lib\socket
        c:\> setup.bat

        c:\> cd c:\cint\lib\stdstuct
        c:\> setup.bat

        c:\> cd c:\cint\lib\dll_stl
        c:\> setup.bat


   Build optional libraries
        c:\> cd c:\cint\lib\ReadF
        c:\> setup.bat

        c:\> cd c:\cint\lib\EasyExp
        c:\> setup.bat

        c:\> cd c:\cint\lib\NVector
        c:\> setup.bat
  

 Most of the above are automated in .\setup.bat
