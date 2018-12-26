platform/visualCpp7/README

 This directory contains Windows-NT Visual C++ project files. You can
 compile CINT on Windows-NT with Visual C++.

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

 Visual C++ 7.0
   If you use Visual C++ 7.0, you need to first run 
   c:\program files\Microsoft Visual Studio .NET 2003\vc7\bin\vcvars32.bat 
   script to setup VC++ environment variables. 
   Before running this, please make sure that your MSDOS Command Window 
   has sufficient environment variable space. Edit MSDOS Command Window 
   property and increase environment variable space. 

   Go to c:\cint\platform\visualCpp7 directory and run setup.bat. This script
   updates c:\autoexec.bat. Reboot system after installation. 

        c:\cint> cd c:\cint\platform\visualCpp7

   Depending on operation system and compiler version, run following
   setup script.

        c:\cint\platform\visualCpp7> setup.bat

   Recent Windows operation system has problem running batch command. It can
   not properly run child process.  Because of this, you need to run setup.bat
   script in lib/longlong, lib/win32api, lib/posix and lib/dll_stl directories.


   Then reboot system.
        ### REBOOT SYSTEM ###


# Compile optional DLLs

  Due to odds in recent Windows' batch engine, setup.bat script may not run
  properly for compiling optional DLLs. If you can not find 
  c:\cint\include\*.dll and c:\cint\stl\*.dll, you need to run setup.bat
  script in each of following directories.

    c:\cint\lib\longlong  : 64bit integer and 92bit floating point

        c:\> cd \cint\lib\longlong
        c:\cint\lib\longlong> setup.bat

    c:\cint\lib\win32api  : Win32 API 

        c:\> cd \cint\lib\win32api
        c:\cint\lib\win32api> setup.bat

    c:\cint\lib\posix     : Posix system calls

        c:\> cd \cint\lib\posix
        c:\cint\lib\posix> setup.bat

    c:\cint\lib\dll_stl   : STL 

        c:\> cd \cint\lib\dll_stl
        c:\cint\lib\dll_stl> setup.bat



IN CASE OF TROBLE:

  As stated above, recent Windows operating system has problem running
  batch script in hierarchy.  If installation script fails, run setup.bat
  indivisually in each subdirectory.

