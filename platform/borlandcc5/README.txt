platform/borlandcc5/README.txt

 This directory contains Windows-NT/9x/Me/2000/XP Borland C++ Compiler 5.5 
 installation scripts. You can compile CINT on Windows using Borland compiler.

NOTE: This Borland C++ makefile is configured as DLL.  


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

   Go to c:\cint\platform\borlandcc5 directory and edit Makefile and MAKEINFO
  there. Default setup assumes CINTSYSDIR=C:\CINT and BCCDIR=C:\Borland\BCC55.
  If this is not the case, you need to modify CINTSYSDIR and BCCDIR in
  Makefile and MAKEINFO. 

        c:\cint> cd c:\cint\platform\borlandcc5
        ### edit Makefile and MAKEINFO if needed ###

   Run setup.bat. This script updates c:\autoexec.bat. Reboot system after 
  installation. 

        c:\cint\platform\borlandcc5> setup.bat
        ### REBOOT SYSTEM ###

  NOTE: Many warning messages will be shown during the setup. Please ignore
        those for the moment. The warnings are mostly due to strict prototype 
        checking of new Borland compiler.

# Compile optional DLLs

  Due to odds in recent Windows' batch engine, setup.bat script may not run
  properly for compiling optional DLLs. If you can not find 
  c:\cint\include\*.dll and c:\cint\stl\*.dll, you need to run setupbc.bat
  script in each of following directories.

    c:\cint\lib\longlong  : 64bit integer and 92bit floating point

        c:\> cd \cint\lib\longlong
        c:\cint\lib\longlong> setupbc.bat

    c:\cint\lib\win32api  : Win32 API 

        c:\> cd \cint\lib\win32api
        c:\cint\lib\win32api> setupbc.bat

    c:\cint\lib\posix     : Posix system calls

        c:\> cd \cint\lib\posix
        c:\cint\lib\posix> setupbc.bat

    c:\cint\lib\dll_stl   : STL 

        c:\> cd \cint\lib\dll_stl
        c:\cint\lib\dll_stl> setupbc.bat


############################################################################
# Making User Library as DLL
############################################################################

 From cint5.15.53, makecint is supported for Borland C++ compiler 5.5. The 
 way makecint is used is compatible to UNIX version. 

 First, you need to have a header file, which describe interface to the
 compiled user library. You also need body of the library either as source 
 file, object file(.obj) or library file(.lib/.dll). Use makecint.exe to 
 generate makefile.  Suppose you have user.h, user.cxx and create user.dll.

    c:> makecint -mk makeuser -dl user.dll -H user.h -C++ user.cxx

 A makefile "makeuser" is generated.  Use make.exe to build user.dll.

    c:> make -f makeuser

 In this process, dictionary source file (G__cpp_user.h, G__cpp_user.cpp)
 is generated. The dictionary file and user.cxx are compiled and linked
 as an output object user.dll.

 In order to use user.dll, simply give it to Cint. 

    c:> cint user.dll

 Or read it from source code using '#include "user.dll"' statement.

    // your main source
    #include "user.dll"
    int main() {
      // you can use things in user.dll here
      return 0;
    }

 Try demos in c:\cint\demo\makecint\complex , array and darray.

    c:\cint> cd demo\makecint\complex
    c:\cint\demo\makecint\complex> bcdll.bat

    c:\cint> cd demo\makecint\array
    c:\cint\demo\makecint\array> bcdll.bat

    c:\cint> cd demo\makecint\darray
    c:\cint\demo\makecint\darray> bcdll.bat


 For more detail, please read documentations under doc directory.

    c:\cint\doc\cint.txt
    c:\cint\doc\makecint.txt
    c:\cint\doc\ref.txt
    c:\cint\doc\extlib.txt
         etc...


############################################################################
# Embed User Library into new Cint Executable
############################################################################

 You can also embed user library into Cint main executable.
 The process is almost same as that of making DLL.

 First, you need to have a header file, which describe interface to the
 compiled user library. You also need body of the library either as source 
 file, object file(.obj) or library file(.lib/.dll). Use makecint.exe to 
 generate makefile.  Suppose you have user.h, user.cxx and create user.exe.

    c:> makecint -mk makeuser -o user.exe -H user.h -C++ user.cxx

 A makefile "makeuser" and main source file G__mainuser.cxx are generated.  
 G__mainuser.cxx is a simple source code that initialize and start Cint
 interpreter. Please refer to doc\ref.txt for APIs used in G__mainuser.cxx.
 Use make.exe to build user.exe.

    c:> make -f makeuser

 In this process, dictionary source file (G__cpp_user.h, G__cpp_user.cpp)
 is generated. The dictionary file, G__mainuser.cxx and user.cxx are compiled 
 and linked as an output object user.exe.

 In order to use user.exe, simply run it. The user.exe behaves the same
 as cint.exe except that the user library is embedded.

    c:> user.exe


############################################################################
# Embed User Library and Cint into user host Executable
############################################################################

 You can also embed Cint and user library into main executable.
 The process is same as that of making DLL.

 First, you need to have a header file, which describe interface to the
 compiled user library. You also need body of the library either as source 
 file, object file(.obj) or library file(.lib/.dll). In this case, you also
 need to provide a host program with main() function. Suppose the host 
 program is provided as host.cxx. An example of host.cxx is shonw below.

    /******************************************************
    * host.cxx : Host program example. 
    ******************************************************/
    #include <stdio.h>
    #include "G__ci.h"
    
    int main(int argc,char **argv)
    {
      int result;
      G__setothermain(2);         // you can use G__init_cint("cint");
      result=G__main(argc,argv);  // instead of G__setothermain and G__main
    
      G__calc("printf(\"t884m\\n\")");
      while(!G__pause());
    
      G__scratch_all();
      return(result);
    }

 Use makecint.exe to generate makefile. You have user.h, user.cxx, host.cxx 
 and create userhost.exe. You must use -m option to inform makecint that 
 you will provide main() function. -I$(CINTSYSDIR) option is needed to add 
 include path for Cint system header file G__ci.h. 


    c:> makecint -mk makeuserhost -I$(CINTSYSDIR) -m -o userhost.exe -H user.h -C++ user.cxx host.cxx

 Note that for include path, you need to use either '/' or '\\' as directory 
 separator. Single backslach '\' does not work. This limitation comes from
 Borland's make.exe utility.
     GOOD : -IC:/CINT  or  -IC:\\CINT  or  -I$(CINTSYSDIR)
     BAD  : -IC:\CINT  or  -I%CINTSYSDIR%

 Makefile "makeuserhost" and main source file G__inituserhost.cxx are 
 generated. G__inituserhost.cxx is a simple source code registering
 dictionary initialization routine. Please refer to doc\ref.txt for 
 APIs used in G__inituserhost.cxx. Use make.exe to build userhost.exe.

    c:> make -f makeuserhost

 In this process, dictionary source file (G__cpp_user.h, G__cpp_user.cpp)
 is generated. The dictionary file, G__inituserhost.cxx, user.cxx and 
 host.cxxare compiled and linked as an output object userhost.exe.

 Behavior of userhost.exe depends on what kind of problem you have in
 host.cxx. 

    c:> userhost.exe
