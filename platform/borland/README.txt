platform/borland/README

 This directory contains Windows-NT C++ Builder 3.0 (Borland C++ 5.3) 
 project files. You can compile CINT on Windows-NT using Borland compiler.

NOTE: This Borland C++ project file is configured as DLL.  
      For Borland C++ Compiler 5.5 , go to platform/borlandcc5 directory.


############################################################################
############################################################################
# Installation
############################################################################
############################################################################

  You must install CINT in c:\cint directory. Make c:\cint directory, copy 
  cint.tgz and unpack by gzip+tar or winzip twice.

        c:\> mkdir c:\cint
        c:\> cd c:\cint
        c:\cint> copy [where_you_get_package] c:\cint\cint.tgz
        c:\cint> gzip -vd cint.tgz
        c:\cint> tar xvf cint.tar

 C++ Builder 3.0 (Borland C++ 5.3)
   Go to c:\cint\platform\borland directory and run setup.bat. This script
   updates c:\autoexec.bat. Reboot system after installation. 

        c:\cint> cd c:\cint\platform\borland
        c:\cint\platform\borland> setup.bat
        ### REBOOT SYSTEM ###

 NOTE: You can not build libcint.dll by C++ Builder IDE (Integrated 
       Development Environment). Cint sources has .cxx file extention
       for C++ sources and IDE does not take this. You must build libcint.dll
       by make.exe utility.
       If you need to build libcint.dll using IDE, rename all the .cxx files
       to .cpp.


############################################################################
# Making Precompiled Library as DLL
############################################################################

 Now, makecint is supported for C++ Builder. C++ Builder version of makecint
 creates makefile and G__bc_xxx.cpp source. When running make.exe, it creates
 interface method source file and .dll object. The way makecint is used is
 is compatible to UNIX version. 

 Try demos in c:\cint\demo\makecint\complex , array and darray.

    c:\cint> cd demo\makecint\complex
    c:\cint\demo\makecint\complex> bcdll.bat

    c:\cint> cd demo\makecint\array
    c:\cint\demo\makecint\array> bcdll.bat

    c:\cint> cd demo\makecint\darray
    c:\cint\demo\makecint\darray> bcdll.bat

