cint 5.15.175 / 6.0.38         ( CINT is pronounced "C-int" )
     | |  |
     | |  +- Patch level (changed almost weekly at each release)
     | +- Minor version  (changed at DLL binary incompatibility)
     +- Major version    (major architecture change)

 Author                 Masaharu Goto
 Copyright(c) 1995~2015 Masaharu Goto (gotom@hanno.jp)


Note: Search 'Installation' for installation instruction.


====== CINT ABSTRACT =======================================================

 "cint" is a C/C++ interpreter which has following features.

 * Support K&R-C, ANSI-C, ANSI-C++
    Cint has 80-90% coverage on K&R-C, ANSI-C and C++ language constructs. 
   (Multiple inheritance, virtual function, function overloading, operator 
   overloading, default parameter, template, etc..)  Cint is solid enough 
   to interpret its own source code. 
    Cint is not aimed to be a 100% ANSI/ISO compliant C++ language processor.
   It rather is a portable script language environment which is close enough 
   to the standard C++.

 * Handling Huge C/C++ source code
    Cint can handle huge C/C++ source code. This has been a problem for other
   C++ interpreter. Cint is quick in loading source files. Cint can interpret
   its own over 60,000 lines source code.

 * Interpretation & Native Code Execution can be mixed
    Depending on speed and interactiveness requirement, you can mix Native
   Code execution and interpretation. "makecint" makes it possible to 
   encapsulate arbitrary C/C++ object as a precompiled library. Precompiled
   library can be configured as a Dynamic Link Library. Access between
   interpreted code and precompiled code can be done seamlessly in both
   direction. 

 * Single-Language solution
    Cint/makecint is a Single-Language environment. It works with any 
   ANSI-C/C++ compiler to provide the interpreter environment on top of it.

 * Bridge between serious programmers and other professionals
    Cint is meant to be a bridging tool between software and non-software
   professionals. C++ looks rather easy under the interpreter environment.
   It helps non-software professionals to talk in the same language to
   their software counterpart. Today's System-On-Silicon evolution demands
   integration and standardization of design tools in software, hardware, IC
   and system design processes. Cint is a key enabling technology to this
   critical issue.

 * Dynamic C++
    Cint is dynamic. It can process C++ statements from command line,
   dynamically define/erase class definition and functions, load/unload 
   source files and Dynamic Link Library.  Extended Run Time Type 
   Identification mechanism is provided. This will allow you to explore
   unthinkable way of using C++.

 * Built-in Debugger, class browser
    Cint has a built-in debugger with an extensive capability to debug
   complex C++ execution flow. Text base class browser is a part of the 
   debugger capability.

 * Portability
    CINT works on number of Operating Systems.
   HP-UX, Linux, SunOS, Solaris, AIX, Alpha-OSF, IRIX, FreeBSD, NetBSD, 
   NEC EWS4800, NewsOS, BeBox, Windows-NT ,Windows-9x, MS-DOS, MacOS, VMS,
   NextStep, Convex. Porting should be easy. Refer to platform/README. 
   What about OS2, VxWorks, etc...?

 * CINT users spread world wide
    Many people world-wide are using Cint. Many C/C++ libraries have 
   been encapsulated. CERN and Fermi-Lab choose Cint as front-end command 
   processor and script interpreter for "ROOT" Object Oriented Software 
   Framework. The ROOT/CINT framework had been used in Large Hadron 
   Collider(LHC) research project.


License: ===================================================================

 * License condition described in this README file overrides other 
  descriptions if there is any difference.

 * Copyright of Cint and associated tools are owned by Agilent Technologies
  Japan Company and the author. 

 * Source code, binary executable or library of Cint and associated tools 
  can be used, modified and distributed with no royalty for any purpose 
  provided that the copyright notice appear in all copies and that both 
  that copyright notice and this permission notice appear in supporting 
  documentation. Registration is recommended for commercial use (=Selling
  a software that uses cint as a component). Send e-mail to the author 
  (gotom@hanno.jp) with your name, e-mail address, institute, 
  purpose of using cint and computer platform.

 * If a modification is made on any of the source or documentation, it has
  to be clearly documented and expressed.

 * Agilent Technologies Japan and the Author make no representations 
  about the suitability of this software for any purpose.  It is provided 
  "as is" without express or implied warranty.

 * Above condition overrides other license condition described in source code 
  and other documentation if there is conflict. It applies and will not be 
  changed for this revision of CINT package. 

 * readline, glob and malloc directories contain files associated with GNU 
  readline library which is copylefted by GNU project. Refer to General
  Public License(GPL). Cint and the GNU readline are completely separate 
  software packages which can work independently.

 * For copyright notice and licensing of AIX dlfcn, please read 
  platform/aixdlfcn/README.


Getting the latest package =================================================

 The latest source code package is available from following sources.

 Cint web page:
   http://www.hanno.jp/gotom/Cint.html (NEW)
    Click 'Download at the bottom of the page.

   http://root.cern.ch/root/Cint.html (OLD)


 Contact:
   gotom@hanno.jp 
   masaharu_goto@keysight.com

   cint@root.cern.ch (OLD)
     Send request to 'Majordomo@pcroot.cern.ch' containing following line for
     subscription.
       subscribe cint [preferred mail address]
     CINT mailing list can be accessed as follows.
       http://root.cern.ch/root/cinttalk/AboutCint.html


Installation: ==============================================================

 - Getting source package

  Source and Windows binary package is distributed as cint.tar.gz from Cint 
 web page.

  * Windows  Visual C++ 13.0 32bit

      1) Download 'cint.tar.gz'
         The package includes source and windows binary
      2) Unpack cint.tar.gz as C:\cint 
         a) If you have gzip and tar
              put cint.tar.gz as C:\cint.tar.gz, 
              C:\> gzip -vd cint.tar.gz 
              C:\> tar xvf cint.tar
         b) If you have Winzip
              Click or drug-drop cint.tar.gz on to Winzip
	      Copy 'cint' directory to C:\cint
      3) Set environmental variable
         Start -> Control Panel -> System -> Detail -> Environmental variable
	    CINTSYSDIR = C:\cint
            PATH = %PATH%;%CINTSYSDIR%

      If you want to build CINT by yourself, (normally not necessary)
      Go to c:\cint\platform\visualCpp13 directory and run setup.bat script
          c:\> cd \cint\platform\visualCpp13
          c:\cint\platform\visualCpp13> setup.bat

  * Linux 64bit  gcc 4.4
  * Linux 32bit  gcc 4.1
  * MacOS-10.10 64bit

      1) Download 'cint.tar.gz'
      2) Unpack cint.tar.gz 
         For example if you put it as $HOME/cint.tar.gz
	   $ cd $HOME
           $ gunzip -c cint.tar.gz | tar xvf - 
         This will create $HOME/cint
      3) Set environmental variable
	    export CINTSYSDIR = $HOME/cint
            expoet PATH = $PATH;$CINTSYSDIR
      4) Remove Windows Binary
          $ cd $CINTSYSDIR/platform
	  $ sh clean_bin.sh
      5) Run setup script
          $ cd $CINTSYSDIR
	  $ sh ./setup platform/linux_RH_64_so        for Linux 64bit
	  $ sh ./setup platform/linux_RH7.2_gcc3_so   for Linux 32bit
	  $ sh ./setup platform/macos_10.10_clang     for MaxOS-X 64bit

         If above setup files do not work, there are other examples in platform
         directory.


   You may also need to set following variables.

	bash		LD_LIBRARY_PATH=.:$CINTSYSDIR:$LD_LIBRARY_PATH
			LD_ELF_LIBRARY_PATH=$LD_LIBRARY_PATH
			export LD_LIBRARY_PATH LD_ELF_LIBRARY_PATH


   Then, you can use 'cint'(interpreter itself) and 'makecint'(compile utility).

	$ cint <options> <[file1].c [file2].c [shl].sl ...> [main].c
	$ makecint <-o|-dl> <[obj]|[shl].sl> -H [src].h -C++ [src].C 


Caution ====================================================================

   Cint creates temp-files in /tmp, /usr/tmp, \temp or windows\temp
   directory. Those files are automatically removed in normal situation but
   sometimes left unremoved by accident. Check /tmp and /usr/tmp regulary 
   to remove garbage.  Temp-files are named *_cint.

Documentation ==============================================================

	$CINTSYSDIR/doc/cint.txt     # cint manual
	$CINTSYSDIR/doc/makecint.txt # makecint manual
	$CINTSYSDIR/doc/limitati.txt # cint syntax limitation
	$CINTSYSDIR/doc/limitnum.txt # cint quantitive limitation
	$CINTSYSDIR/doc/ref.txt      # cint special feature, ERTTI API
	$CINTSYSDIR/doc/cintapi.txt  # cint API
	$CINTSYSDIR/doc/bytecode.txt # cint incremental bytecode compiler


Demo =======================================================================

    You can find demonstration programs in $CINTSYSDIR/demo/ directory.
   These programs can be referred as programming examples. Please read README
   file under there. Makecint examples are found in $CINTSYSDIR/demo/makecint
   directory.

    It is highly recommended to start with those demo programs. 


Contributers ==============================================================

  The Author would like to thank Fons Rademakers and Rene Brun in CERN
 for jointly develop ping ROOT-CINT framework system. Their invaluable advice 
 has been improving cint. They ported CINT on 10 different UNIX platforms.

  Philippe Canal in Fermi-Lab contributes to many enhancements and bug fixes.

  Osamu Kotanigawa is giving great advice on Windows-NT Visual C++ and 
 Borland C++ porting. He created src/vcstrm.cxx and src/bcstrm.cxx. His
 contribution improved CINT in many aspects.

  Jacek Holeczek refined DLL schemes on IBM AIX. He wrote platform/aix*
 and platform/aixMakefileBaseAdd.

  Dibyendu Majumdar in U.K. ported CINT on MS-DOS using DJGPP.

  Kiyoshi Yamamoto, in CQ publishing company, is giving me opportunity to 
 write a book and articles about CINT.

  Masa Habu in HP Cupertino suggested usefulness of C++ interpreter in 1992
 when CINT was only a C interpreter.

  Junichi Mizoguchi in Agilent Technologies Japan created a base concept 
 of makecint by his automated software test execution environment.

  And many thanks to people in Hewlett-Packard Japan/Agilent Technologies/
 Keysight Technologies Japan Hachioji-site.

The Author ===============================================================

 Masaharu Goto

  Please send e-mail to gotom@hanno.jp
