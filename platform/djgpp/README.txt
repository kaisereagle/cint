platform\djgpp\readme.txt

# INSTALLATION ###########################################################

 This directory contains CINT makefile for MSDOS using DJGPP. This 
distribution is experimental. You may need to modify Makefile to 
complete compilation. 

 1) You have to install DJGPP in your computer first. See CYGNUS Web
   page for getting the latest DJGPP.  You need C/C++ compiler and
   make utility. You need to create a directory for DJGPP which contains
   include, lib and bin directories.

 2) Create directory to install cint.  For example, 

      c:\> mkdir c:\cintdos

 3) Set the environment variables CINTSYSDIR, DJGPPDIR and PATH. For
    example, if you'll install DJGPP to c:\djgpp and cint to c:\cintdos.
    Be careful, you need to use '/' notation like UNIX for CINTSYSDIR and
    DJGPPDIR.

      c:\> set CINTSYSDIR=/cintdos
      c:\> set DJGPPDIR=/djgpp
      c:\> set PATH=%PATH%;c:\cintdos;c:\djgpp\bin

 4) Unpack cint.tgz there. You might have done this already, because 
   you are reading this file.

      c:\> copy [some_where]\cint.tgz %CINTSYSDIR%\cint.tgz
      c:\> gzip -vd cint.tgz
      c:\> tar xvf cint.tar

 5) Copy DJGPP's crt0.o as src\crt0.o in CINT directory. I do not know
   the reason but DJGPP complains unless crt0.o exists in current working
   directory. For example, if you installed DJGPP into c:\djgpp and CINT 
   into c:\cintdos, you'll do following.
   
      c:\> copy c:\djgpp\lib\crt0.o c:\cintdos\src\crt0.o

 6) Change directory to %CINTSYSDIR%\platform\djgpp and run setup.bat 
   script.

      c:\> cd %cintsysdir%\platform\djgpp
      c:\cintdos\platform\djgpp\> setup.bat

 setup.bat uses GNU make.exe. If you have another make.exe from other
compiler system, cint installation will fail. Please make sure that
you use GNU make.exe.

 If above goes successfully, you can use cint.exe and makecint.exe.
You could try some examples in demo/makecint/ReadFile, demo/makecint/KRcc
and demo/makecint/Stub directories. If you have bash, try 'bash setup' under
these directories.


 CINT on DJGPP is experimental. Please report problems you encounter to
the author cint@pcroot.cern.ch.


# COMMENTS ##############################################################

# iostream library

 CYGNUS distributes C++ iostream library as separate package. This setup
does not use iostream library. If you have the iostream library, change
following files before running setup.bat script.

  %CINTSYSDIR%\platform\djgpp\Makefile
      STDLIBS = dmystrm.o stdstrct.o
                  |
                  V
      STDLIBS = libstrm.o stdstrct.o

  %CINTSYSDIR%\platform\djgpp\MAKEINFO
      STDLIBS = dmystrm.o stdstrct.o
                  |
                  V
      STDLIBS = libstrm.o stdstrct.o

