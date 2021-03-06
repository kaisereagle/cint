platform/visual/README

 This directory contains Windows-NT Visual C++ project files. You can
 compile CINT on Windows-NT with Visual C++.

NOTE: This Visual C++ project file is configured as DLL.  

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

 Visual C++ 4.0, 5.0, 6.0
   If you use Visual C++ 5.0, you need to first run 
   c:\program files\devstudio\vc\bin\vcvars32.bat batch file to 
   setup VC++ environment variables. If you use Visual C++ 6.0, you need to
   run c:\Program Files\Microsoft Visual Studio\VC98\bin\vcvars32.bat. 
   Before running this, please make sure that your MSDOS Command Window 
   has sufficient environment variable space. Edit MSDOS Command Window 
   property and increase environment variable space. You may also need 
   to comment out following line from vcvars32.bat.

        rem set vcsource=L:\DEVSTUDIO

   Then run the script

        c:\> cd \
        c:\> cd "program files"
        c:\> cd devstudio\vc\bin                       << In case of VC++ 5.0
        c:\> cd "Microsoft Visual Studio\VC98\bin"     << In case of VC++ 6.0
        c:\> vcvars32.bat

   You do not need to do above if you use Visual C++ 4.0. 

   Go to c:\cint\platform\visual directory and run setup.bat. This script
   updates c:\autoexec.bat. Reboot system after installation. 

        c:\cint> cd c:\cint\platform\visual

   Depending on operation system and compiler version, run following
   setup script.

     Windows-95/98/Me , VC++ 6.0
        c:\cint\platform\visual> setup.bat

     Windows-2000 , VC++ 6.0
      It looks like Windows-2000 has problem setting environment variablle in
      batch script. Set environment variable CINTSYSDIR=c:\cint and 
      PATH=%CINTSYSDIR%;%PATH% first. Then run following command.
        c:\cint\platform\visual> setup2000.bat

     Windows-NT , VC++ 6.0
        c:\cint\platform\visual> setupNT.bat

     Windows-95/98/Me , VC++ 5.0 or older
        c:\cint\platform\visual> setupold.bat

     Windows-NT , VC++ 5.0 or older
        c:\cint\platform\visual> setupNTold.bat


   Then reboot system.
        ### REBOOT SYSTEM ###



IN CASE OF TROBLE:

   If you install Visuall C++ with mininum configuration, some library files
   may be missing. In case you see link error at linking libcint.dll, copy
   following files from VC++ CD-ROM and try again. For example,

     CD-ROM:\MSDEV\LIB\LIBCMT.LIB   =>  C:\MSDEV\LIB\LIBCMT.LIB
     CD-ROM:\MSDEV\LIB\LIBCMTD.LIB  =>  C:\MSDEV\LIB\LIBCMTD.LIB

   If you find problems running cl.exe, loading include files or linking 
   library , set following environment variable in C:\AUTOEXEC.BAT, reboot
   system and try 'setup.bat' again in c:\cint\platform\visual.

      SET PATH=%PATH%;C:\MSDEV\BIN
      SET include=c:\msdev\mfc\include;c:\msdev\include;c:\include;%include%
      SET lib=c:\msdev\mfc\lib;c:\msdev\lib;%lib%

############################################################################
# Making Precompiled Library as DLL
############################################################################

 Now, makecint is supported for Visual C++. Visual C++ version of makecint
 creates makefile and make.bat script. When running make.bat, it creates
 interface method source file, .def file and .dll object. The way makecint 
 is used is compatible to UNIX version. 

 Try demos in c:\cint\demo\makecint\complex , darray , readfile , krcc and 
 stub. Try following.

    c:\cint> cd demo\makecint\complex
    c:\cint\demo\makecint\complex> setup.bat
    c:\cint\demo\makecint\complex> setupdll.bat

    c:\cint> cd demo\makecint\darray
    c:\cint\demo\makecint\darray> setup.bat
    c:\cint\demo\makecint\darray> setupdll.bat

    c:\cint> cd demo\makecint\krcc
    c:\cint\demo\makecint\krcc> setup.bat
    c:\cint\demo\makecint\krcc> setupdll.bat

    c:\cint> cd demo\makecint\readfile
    c:\cint\demo\makecint\readfile> setup.bat
    c:\cint\demo\makecint\readfile> setupdll.bat

    c:\cint> cd demo\makecint\stub
    c:\cint\demo\makecint\stub> setup.bat
    c:\cint\demo\makecint\stub> setupdll.bat
