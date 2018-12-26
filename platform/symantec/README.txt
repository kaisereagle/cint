platform/symantec/README

 This directory contains Windows-NT Symantec C++ 7.2 project files. You can
 compile CINT on Windows-NT with Symantec C++ 7.2.

NOTE: This Symantec project file is configured as DLL.  

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

 Symantec C++ 7.2
   Go to c:\cint\platform\symantec directory and run setup.bat. This script
   updates c:\autoexec.bat. Reboot system after installation.

        c:\cint> cd c:\cint\platform\symantec
        c:\cint\platform\symantec> setup.bat
        ### REBOOT SYSTEM ###


IN CASE OF TROUBLE:

   Please make sure following environment variables are set in c:\autoexec.bat
   Symantec C++ must be installed with command line compiler option.

        SET PATH=%PATH%;C:\SC\BIN
        REM SET HELP=%HELP%;C:\SC\HELP


############################################################################
# Making Precompiled Library as DLL
############################################################################

 Now, makecint is supported for Symantec C++. Symantec version of makecint
 creates makefile and .lnk file. When running smake, it creates interface
 method source file, .def file and .dll object. The way makecint is used is
 compatible to UNIX version. 

 Try demos in c:\cint\demo\makecint\complex , darray , readfile , krcc and 
 stub.

    c:\cint> cd demo\makecint\complex
    c:\cint\demo\makecint\complex> scsetup.bat
    c:\cint\demo\makecint\complex> scdll.bat

    c:\cint> cd demo\makecint\darray
    c:\cint\demo\makecint\darray> scsetup.bat
    c:\cint\demo\makecint\darray> scdll.bat

    c:\cint> cd demo\makecint\krcc
    c:\cint\demo\makecint\krcc> scsetup.bat
    c:\cint\demo\makecint\krcc> scdll.bat

    c:\cint> cd demo\makecint\readfile
    c:\cint\demo\makecint\readfile> scsetup.bat
    c:\cint\demo\makecint\readfile> scdll.bat

    c:\cint> cd demo\makecint\stub
    c:\cint\demo\makecint\stub> scsetup.bat
    c:\cint\demo\makecint\stub> scdll.bat
