platform/README

 Copyright(c) 1995~2015 Masaharu Goto (gotom@hanno.jp)

Description ================================================================

 Cint multiple platform support directory README file. This file contains 
 following information.

  - CINT porting requirement
  - UNIX platform dependency file format
  - UNIX porting status
  - Windows, MS-DOS porting status
  - MacOS, BeOS, NextStep, VMS porting status
  - Trouble shooting


Minimum requirement =======================================================

ANSI-C/C++ compiler : MUST 
 As of May/2015, CINT can be compiled with Visual C++ 13.0, gcc4.4 on Linux and
 llvm on MacOS-X. CINT used be compiled with other kinds of C/C++ compilers. 
 Porting shouldn't be difficult.

sizeof(long)>=sizeof(void*) : MUST
 Cint handles pointers as long in many places. So long object must have
 sufficient size to store full address information

32bit or 64bit processor : MUST
 Microprocessor architecture has to be 32bit or 64bit. 16bit architecture
 is too small to keep full Cint object in its address space.


About platform directory ===================================================

 The "platform" directory contains files to describe platform dependent
 information. You will find files which have name of computer platforms. 
 Please read these files as examples.

 One of the platform dependency file must be copied as $CINTSYSDIR/MAKEINFO.
 The MAKEINFO file is used when you run makecint or cint. It is important 
 to prepare an appropriate MAKEINFO file.


UNIX =======================================================================

Platform dependency file format:

 Format of the platform dependency file is same as Makefile. Refer to 
 platform/gcc_max file as a generic template. You need to specify name of
 C and C++ compiler, compiler and linker options, libraries and file extension.
 You need to set these parameters appropriately.

  RM		= command to remove file, rm , remove,

  CP		= command to copy file ,  cp , copy, etc

  AR		= Archiver,  ar

  AROPT         = Archiver option. Usually set clq , in Solaris set cq

  KRCC          = K&R C compiler, cc, gcc, gcc -traditional etc, This is for
                 compiling readline library.

  CC		= ANSI C compiler, cc, gcc, /usr/ucb/cc, etc

  CPP		= ANSI C++ compiler, CC, g++. If you don't have a C++ compiler
		 this can be omitted. Read very carefully about APIA, STDLIBS
                 described below.

  LD		= Linker, CC, g++, cc. If you have C++ compiler, LD must
		 be same as name of the C++ compiler

  CPREP		= C preprocessor. cc -E , gcc -E,  Usually, name of C compiler
		 + -E

  CPPPREP 	= C++ preprocessor, CC -E , g++ -E, Usually, name of C++
		 compiler + -E

  CCDLLOPT	= Compiler option to create Dynamic Link Library, +Z, -fPIC
		 If you do not use DLL, no need to specify.

  LDDLLOPT	= Linker option to create Dynamic Link Library, -b, -shared
		 etc... If you do not use DLL, no need to specify.

  OPTIMIZE 	= Compiler optimization flag, -O or -g

  LDOPT		= Linking library. Math lib -lm is at least needed. 
		  If you use GNU readline library -ltermcap -lbsd , 
		 -lcurses -lBSD or equivalent library must be given.
		   In case using Dynamic Linking capability, you may need 
		  to add appropriate option and library, for example ,
		  -ldl -rdynamic, -ldld. 

  SHLIB_OPT     = You need to set this variable as '$(CCDLLOPT)' if you
		  install cint core as shared library libcint.so.
		  You do not need to set SHLIB_OPT otherwise.

  SYSMACRO	= Define macro to turn on specific feature. Macros defined 
		  here will be used for compiling CINT core as well as user
		  defined precompiled library.
		  -DG__REGEXP : Using regular expression
		  -DG__REGEXP1 : Using regular expression in SGI
		  -DG__SHAREDLIB : HP-UX8/9 without C++ compiler, WinNT
		  -DG__SHAREDLIB -DG__HPUXCPPDLL : HP-UX8/9 with C++ compiler
		  -DG__SHAREDLIB -DG__OSFDLL : Linux1.2, SunOS, OSF, etc
		  -DG__SIGNEDCHAR : This option is needed if your computer or
				 compiler handles 'char' as 'unsigned char'.
				 CINT depends that 'char' is signed. 
				 Alternatively, you can use signed-char option
				 of the compiler.
		  -DG__LONGBUF : Expand CINT name buffer for handling long 
				 template class/function names.
		  -DG__VARIABLEFPOS : If you define this macro, file name and
				 file position for global variable is included
				 in the CINT symbol table. This feature is
				 useful when you use Cint as source code
				 analyzer.
		  -DG__TYPEDEFPOS : If you define this macro, file name and
				 file position for typedef is included
				 in the CINT symbol table. This feature is
				 useful when you use Cint as source code
				 analyzer.
		  -DG__REFCONV : This macro activates bug fix for standard
				 conversion problem for reference type 
				 argument. Change was made at 5.14.15. If
				 you activate this fix , new version of Cint
				 can read DLL made by older version, but older
				 Cint can not read DLL made by newer version.
				 This macro may already be defined in G__ci.h
				 header. In that case, please do not define
				 this in platform dependency file.
		  -DG__CPPIF_EXTERNC : When trying examples in demo/makecint,
				 if you have errors that type of funcp
				 argument mismatches by 'static int(*)(...)'
				 and 'extern "C" int(*)(...)', define this
				 macro. Defining this macro, cint will 
				 generate interface method as 'extern "C"'.
		  -DG__CPPIF_STATIC : This macro enforces interface method
				 to be static C++ function. This is a default.
				 G__CPPIF_STATIC and G__CPPIF_EXTERNC are 
				 exclusive.
		  -DG__CONSTNESSFLAG : This macro activates function 
				 overloading resolution by object constness.
				 This feature has following problems.
				  - DLL/sharedlib binary incompatibility
				  - STL container const_iterator does not work
				 It is not recommended to define this macro
				 at this moment. 
		  -DG__ERRORCALLBACK : If this macro is defined, error messages
				 can be redirected through an user defined
				 function. The callback function can be set by
				 G__set_errmsgcallback() API.
		  -DG__NEWSTDHEADER : If this macro is defined, cint expects
				 standard header file name without .h when
				 compiling libstrm.cxx. This is needed for 
				 g++ 2.97 or later.
		  -DG__DLL_SYM_UNDERSCORE: This macro has to be defined if 
				 you use a compiler or linker that handles 
				 symbol name as _[name] in binary object. 
				 CINT's dynamic loader looks for _[name] 
				 instead of [name].

  OTHMACRO	= Define macro to turn on specific feature. Macros defined
		  here will be used only for compiling CINT core.
                  -DG__P2FDECL :  Due to compiler limitations, user can
                  -DG__P2FCAST : define one of G__P2FDECL or G__P2FCAST for
                                 handling true pointer to global function.
                                 It is OK not to define either. You can not
                                 use true pointer to function in interpreter
                                 environment.
                                 To test which of G__P2FDECL or G__P2FCAST 
                                 works, please try following and see if it
                                 compiles without error.

                                  $ makecint -mk Makefile -H t.h
                                  $ make

                                  // t.h
                                  template<class A> class B { A obja; }
                                  void f(void) {}
                                  void f(int a) {}
                                  B<int> f(B<int> &a) { return a;}

                  -DG__REDIRECTIO : This macro can be defined in UNIX platform
                                 to make use of I/O redirection in debugger
                                 interface. 
                  -DG__SPECIALSTDIO : This macro replaces stdio related 
                                 ANSI library functions with a special 
                                 version that you can provide with G__xxx.
                                 for example, printf is replaced with
                                 G__printf. This macro is only defined when
                                 building CINT as dynamically loadable
                                 extension to Tcl7.5/Tk4.1 under Windows-NT/9x
                                 environment.
		  -DG__PTR2MEMFUNC : This macro activates pointer to member
				 function support. Activated by default from
				 cint5.15.37.  But, the implementation is not 
				 perfect, especially seamless access of
				 pointer to member between interpreted and
				 compiled code is not implemented yet. 
		  -DG__STD_NAMESPACE : Don't use this macro now. This macro
				 validates std namespace. Normally std is
				 handled as global scope.
		  -DG__EXCEPTIONWRAPPER : This macro activates exception 
				 catching at calling precompiled C++ function.
				 Any thrown C++ exceptions will be transformed
				 as CINT exception.
		  -DG__STD_EXCEPTION : This macro activates handling of
				 compiled std::exception catching. Class
				 derived from std::exception are handled
				 as std::exception. If this macro is defined
				 G__EXCEPTIONWRAPPER is automatically defined.
				 If you add this flag, you have to use a C++ 
				 compiler with C++ RTTI capability, especially
				 typeid(). In other words, if you disable C++ 
				 RTTI (for example -fno-rtti for g++), this
				 flag must be removed.
		  -DG__NOMATHLIB : This macro disables ANSI C math library
				 functions. Math library embedded in cint 
				 does not support function overloading. It
				 is called regardless of argument matching.
				 You can define this macro to disable this
				 feature. You need to create interface 
				 method for math library by yourself in this
				 case.
		  -DG__NONSCALARFPOS : VMS has non scalar fpos_t. This macro
				 is defined in G__ci.h to handle VMS fpos_t.
				 Normally, there is no need to define this 
				 macro in platform dependency file.
		  -DG__NONSCALARFPOS2 : New g++ comming with RedHat7.0 Linux
				 package has non scalar fpos_t. This macro
				 is defined in G__ci.h to handle RH7.0 fpos_t.
				 Normally, there is no need to define this 
				 macro in platform dependency file.
		  -DCINTSYSDIR=\"/usr/local/cint\" : If this macro is defined,
				 cint can work without CINTSYSDIR environment
				 variable. Value described here will be used 
				 by default for CINTSYSDIR. This setup is 
				 useful for packaging Cint with operating
				 system distribution when an extra environment
				 variable must be avoided and installation is
				 always targetted to a fixed system directory.
				 CAUTION: Avoid using this macro for HP-UX aCC.
				 It can cause problem with include path 
				 setting.
		  -DG__MULTITHREADLIBCINT : If this macro is defined, Cint 
				 dictionary resolves functions in libcint.so
				 by explicit calls to G__SetCCintApiPointers 
				 or G__SetCppCintApiPointers. This is very
				 specialy for libcint.so multi-threading 
				 scheme. For expert use only. DO NOT USE
				 THIS FLAG UNLESS YOU DEEPLY UNDERSTAND IT.
		  -DG__NOMAKEINFO : If this macro is defined, Cint does not
				 refer to $CINTSYSDIR/MAKEINFO. Information
				 inside MAKEINFO is handled as "". This switch
				 exits so that user can use Cint core 
				 independently without any other system files.
		  -DG__NOPOWEROPR : Disable '**' power operator to avoid
				 ambiguity. 'a**b' is interpreted as 'a*(*b)'
				 assuming 'b' is a pointer. If this macro is
				 not defined 'a**b' is interpreted as 
				 'a power b'.
		  -DG__NO_STDLIBS : Disable ANSI C library functions and CINT
				 API functions to be used from interpreted 
				 source code. In order to completely disable
				 standard library functions, you also need to
				 delete include/*.dll and stl/*.dll from
				 Cint installation directory.
		  -DG__SUPPRESS_UNUSEDPARA : Suppress unused parameter warning
				 when compiling Cint source code.
		  -DG__TMPFILE : Use tempnam() instead of tmpnam() which is 
				 claimed to be a bad library, for creating
				 temporary file. 
		  -DG__64BIT : For 64bit platform.
		  -DG__NATIVELONGLONG : Use native 'long long', 'unsigned long
				 long'  as 64bit integer and 'long double' as
				 92bit floating point number.

  SYSIPATH	= System Include path. Usually no need specify.

  MAINO		= main() function .o file. If you a have C++ compiler, this
		 must be $(CINTSYSDIR)/main/G__cppmain.o  If you only have
		 C compiler, this must be $(CINTSYSDIR)/main/G__main.o

  CINTLIB	= Cint core library. $(CINTSYSDIR)/src/G__ci.a

  READLINEA	= GNU readline library. GNU readline library is compiled and
		 linked only if $(CINTSYSDIR)/readline/libreadline.a is
		 given here.  Can specify /usr/local/lib/libreadline.a if
		 you already have readline library installed in your machine.
		 If you leave this selection blank, the readline won't be 
		 used at all.

  APIO		= Cint Extended Run Time Type Identification library. If you
		 have a C++ compiler, set 'Api.o Class.o BaseCls.o Type.o 
		 DataMbr.o Method.o MethodAr.o CallFunc.o Typedf.o Apiif.o'

  RANLIB	= /usr/bin/ranlib

  STDLIBS	= If you have C++ compiler, set libstrm.o to use iostream 
		 library. libstrm.o is for non-template iostream library.
		 cbstrm.o is provided for template base iostream. This library
		 is verified only on Borland C++ Builder, so far.
		 If neither of libstrm.o and cbstrm.o work, set fakestrm.o.
		 if you only have C compiler, set dmystrm.o. 
		  In addition to iostream stub library, you need to set
		 stdstrct.o to support ANSI-C standard library structure.
		 Example:
		     C++ compiler + non-template iostream
			STDLIBS = libstrm.o  stdstrct.o
		     C++ compiler + template base iostream
			STDLIBS = cbstrm.o   stdstrct.o
		     C++ compiler with no iostream library
			STDLIBS = fakestrm.o stdstrct.o
		     C compiler only
			STDLIBS = dmystrm.o  stdstrct.o

  PLATFORM	= Platform dependent source file. If some C standard library
		 functions are missing in your compiler environment, you need
		 to specify substitution here. For example, in SunOS 4.1 with
		 /usr/ucb/cc, sunos.o must be specified. In case there is no
		 getopt() and alarm() functions you need to specify
		 winnt.o here. 
		  You may need to create your own substitution library. In 
		 that case, please add the source under src/ directory,
		 modify Makefile.base and send me copy of these files.

  CSRCPOST	= C source file extension, .c

  CHDRPOST	= C header file extension, .h

  CPPSRCPOST	= C++ source file extension, .C, .CPP, .CXX, .cpp, .cxx

  CPPHDRPOST	= C++ header file extension, .h

  OBJPOST	= Object file extension, .o, .OBJ

  DLLPOST	= Dynamic Link Library file extension.  .dll , .DLL , .so ,
		 .sl or .dl  if DLL is supported, blank if not. Additional
		 argument enforces DLLPOST to be used DLLPOST whatever 
		 extension is given to makecint.
		 Example1:   DLLPOST = .dll
		 Example2:   DLLPOST = .so
		 Example3:   DLLPOST = .so  MustUseThis


Running setup script:

 After you have created platform information file, you can try running 
 setup script. The setup accepts platform description file as an argument.
 Specified platform file is copied as MAKEINFO file and used when platform
 dependent information is needed.

	$ sh setup platform/[Your-Platform]

 If this fails, please look into following Makefiles and figure out the
 problem by yourself.

	src/Makefile
	readline/Makefile
	Makefile


UNIX PORTING STATUS =========================================================

MacOS-X requirements:
      Xcode development environment has to be installed. Xcode is distributed 
      free of change from Apple. See instruction in Apple home page.

MacOS-10.7
   platform/macosx_10.10_clang
        The latest build on MacOS10 as of May2015 with clang/llvm
	
Linux requirements:
      When installing Linux package, you need to add following packages in
      order to build Cint. Basically, you need to install developper 
      environment.
	binutils (d),  gcc (d), gcc-g++ (d), make (d), kernel-headers (d),
	readline (l), ncurses (l), glibc (l)

Linux 2.6 64bit RedHat 
   platform/linux_RH_64_so
        The latest build on Linux as of May2015 with Red Hat 64bit.

Linux 1.1 (platform/linux1.1)
	linux1.1 is a quite stable environment, however, it does not support
	Dynamic Linking capability.

Linux 1.2 (platform/linux1.2)
	There is a bug in Linux 1.2's iostream library. Cint crashes when
        ostream is used. 

Linux 1.3 (platform/linux1.2)
Linux 2.0 (platform/linux2.0)
Linux_RH* (platform/linux_RH*)
Linux_debian* (platform/linux_debian*)
	Linux1.3 and Linux2.* supports all cint/makecint features. Linux2.0
	is the most stable platform to run CINT at this moment. All of the
        features can be used including dynamic linking.
	There are several variations for Linux2.* configuration.
   platform/linux_RH4.2
   platform/linux_RH5.1
   platform/linux_RH5.2
   platform/linux_RH6.2
   platform/linux_RH7.0
   platform/linux_RH7.3
	Those configurations are the best matches for specified RedHat Linux
	version. 
	Note that RedHat7.3 with gcc2.96 has a minor problem in iostream 
	library and it complains that 'computed value is never used'. This 
	is a gcc bug.
   platform/linux2.0_so
   platform/linux_RH4.2_so
   platform/linux_RH5.1_so
   platform/linux_RH5.2_so
   platform/linux_RH6.2_so
   platform/linux_RH7.0_so
   platform/linux_RH7.3_so
   platform/linux_RH7.3_gcc3_so
   platform/linux_RH7.3_gcc4_so
   platform/linux2.0_so_only_C
	Cint core is normally created as static library src/G__ci.a.
	Above configurations create cint core as shared library libcint.so.
	You need to set LD_LIBRARY_PATH and LD_ELF_LIBRARY_PATH to include
	$CINTSYSDIR.
	Note that RedHat7.3 with gcc2.96 has a minor problem in iostream 
	library and it complains that 'computed value is never used'. This 
	is a gcc bug.
   platform/linux_RH6.2_mtso
   platform/linux_RH6.2_gcc3_mtso
   platform/linux_RH7.3_mtso
	Those *_mtso are special and experimantal setup for multi-thread 
	libcint.so testing. DO NOT USE THIS UNLESS YOU DEEPLY UNDERSTAND.
	With *_mtso configuration, you can only compile user library as
	DLL and not as an archive library.
   platform/linux_RH6.2_gcc3
	This configuration is for RedHat6.2 Linux with gcc-3.00. gcc-3.00
	has major changes from 2.x. There still remains problems. Tips for
	gcc-3.0 for other platforms are,
	1. Remove -DG__P2FCAST and/or -DG__P2FDECL from OTHMACRO
	2. Replace libstrm.o with gcc3strm.o in STDLIBS
	Some Cint functionalities do not work with this configuration. The 
	autohor is fighting against those problems.
   platform/linux_debian_slink
	Linux Debian 2.1 package.
   platform/linux_debian_potato
	Linux Debian 2.2 package.
   platform/linux_debian_potato2
	Linux Debian 2.2 package. This setup is aimed for making a clean
	Cint installation under Debian GNU/Linux. A macro CINTSYSDIR=\"dir\"
	is set in Makefile so that cint and makecint can work without
	CINTSYSDIR environment variable. CINTSYSDIR will be set to a value
	described in this file by default. Use INSTALLBIN script to install 
	binary only package then. This setup is a contribution of Richard 
	Kreckel (Richard.Kreckel@Uni-Mainz.DE) 2001/Feb.

   platform/linux_RH_64_so
        RedHat 64bit Linux OS.


HP-UX 8.0,9.0,10.0,11.0 with HP CC , cc (platform/hpux8,hpux11)
	You need C/C++ development environment on HP-UX. C/C++ compiler 
	which accepts -g option is needed. All of cint/makecint features can 
	be used. The only exception is dynamic linking of template class.

HP-UX 10.2,11.0 with HP aCC (platform/hpux10_acc,hpux11_acc)
	HP-UX is the main platform of cint. So all of cint/makecint features
	can be used. aCC does not like $CINTSYSDIR/include directory when
	building cint. Hence in the $CINTSYSDIR/setup script,  include
	directory is renamed as Include and 'make' is executed. It renames
	the directory again and run 'make' again.

HP-UX 10.2,11.0 with HP aCC (platform/hpux10_acc_so,hpux11_acc_so)
        Same as above, except that cint core is installed as shared library
	libcint.sl.

HP-UX 8.0,9.0,10.0 with HP cc (platform/hpux8_only_C)
	It is possible to install cint only with C compiler. You need C 
	development environment on HP-UX. C compiler which accepts -g 
	option is needed.

HP-UX 9.0 with gcc (platform/hpux8_gcc)
	This configuration provides most of the CINT feature. Dynamic 
	linking of library does not work although it should be possible.


Solaris with gcc (platform/solaris_gcc)
	This configuration provides most of the CINT feature. Dynamic 
	linking of library can not be used although it should be possible.
	This may be due to wrong compiler and linker options in 
	platform/solaris_gcc file. Your contribution to improve this feature
	is greatly appreciated.

Solaris with SUNWsprc CC (platform/solaris4.3, solaris5.4)
	Sun sparc C compiler has problem compiling old K&R C library in GNU
	readline. So GNU readline is not used in solaris5.4. solaris4.3 uses
	/usr/local/lib/libreadline.a.  Dynamic linking is supported.

Solaris with SUNWsprc CC5 (platform/solaris5.4_cc5,solaris5.7_cc5, 
	                   solaris5.7_cc5_B)
   platform/solaris5.4_cc5
   platform/solaris5.7_cc5
	CC5 is claimed to be 100% ANSI/ISO C++ compliant compiler. There are
	problems about strict type checking. In the standard, pointer to C++
	function can be different from pointer to C function. A compiler can
	include some additional information for function overloading in
	pointer to C++ function. So far I have not seen any compiler
	implementing this scheme, however, in future, compilers may have
	different structure for pointer to C++ function. CINT heavily depends
	on the fact that pointer to function can be assigned to void*. With
	the standard, this assumption can not be held. 
	To solve this problem, CINT can use pointer to C function only. The 
	implementation is 80% done and can be activated by adding 
	-DG__CPPIF_EXTERNC to SYSMACRO. There are some remaining problems.

	Sun sparc C compiler has problem compiling old K&R C library in GNU
	readline. So GNU readline is not used in solaris5.4. solaris4.3 uses
	/usr/local/lib/libreadline.a.  Dynamic linking is supported.

   platform/solaris5.7_cc5_B
	This configuration is different from above two only in iostream 
	library dictionary setup. sun5strm.cxx, which is created specifically
	for Sun-CC5, is used.

Solaris with SUNWsprc CC5 (platform/solaris5.7_cc5_so,solaris5.7_cc5_B_so)
	Same as above, except cint core is installed as shared library
	libcint.so.

SunOS 4.1 with /usr/ucb/cc (platform/sunos4_only_C)
	makecint can be used with some trick. makecint only accepts ANSI
	style function headers while /usr/ucb/cc only accepts K&R. You
	need to use #ifdef __MAKECINT__ to switch between those cases.
	Look at an example in demo/makecint/KRcc.  Dynamic linking of
	library can not be used.

SunOS 4.1 with gcc (platform/sunos_gcc)
	Installation package is improved for this platform. Dynamic linking
	may not work although it should be possible. This may be due to wrong
	compiler and linker options in platform/sunos_gcc Your contribution 
	to improve this feature is greatly appreciated.

SGI IRIX with CC (platform/sgi5)
SGI IRIX with gcc (platform/sgi5_gcc)
SGI IRIX with KCC (platform/sgi6.kcc)
	CINT has been ported to those platforms. There are quite a few 
        users on this platform. Some people have installation problem
        due to 'if' statements in Makefile. In such cases, try
        Makefile.base.sgi and src/Makefile.base.sgi. Move these files
        as Makefile.base and src/Makefile.base, then run setup script.
        If you get error running Makefile, please debug by yourself.

	Another thing you need to keep in mind is nature of 'char'.
	It seems that SGI has unsigned 'char' by default. 'char' needs to
	be signed in order for Cint to operate correctly. There is a switch
	for SGI C++ compiler to choose between signed and unsigned 'char'.
	You need to either  use signed 'char' option  or  add a macro
	'SYSMACRO = -DG__SIGNEDCHAR' in platform dependency file.

FreeBSD (platform/freebsd2.1R, freebsd_B, freebsd_C, freebsd_min)
	CINT has been ported to FreeBSD. 
   platform/freebsd2.1R
	This configuration supports full feature of CINT under FreeBSD2.1R.
	This configuration can be good for later version of FreeBSD. You need
	to define -DG__DLL_SYM_UNDERSCORE in order to use shared library under
	FreeBSD.
   platform/freebsd_B
   platform/freebsd_C
	Those configuration should support full feature of CINT. However,
	details are not confirmed.
   platform/freebsd_min
	This configuration is a mininum setup for FreeBSD. This does not
	support shared library and other interesting features.

NetBSD (platform/netbsd)
	CINT has been ported to NetBSD. But the author has no detail
	information.

DEC ALPHA OSF with gcc (platform/alpha_gcc)
	All of the features can be used including dynamic linking.

DEC ALPHA OSF with cxx (platform/alpha_cxx)
	Dynamic Linking of precompiled library does not work, although it
	should be possible.

IBM AIX with xlC (platform/aix3_xlC, aix4_xlC)
	Tested by Jacek Holeczek in GSI. Special technique is needed to
	make use of DLL. Thanks to Jacek, full feature of cint/makecint can 
	be used.

IBM AIX with gcc (platform/aix3_gcc, aix4_gcc)
	CINT has been ported to those platforms, however, not tested well.

Sony NewsOS4 (platform/newsos4_only_C)
Sony NewsOS6 (platform/newsos6_only_C)
	Sony News was the last major UNIX operating system to port. This
	work was done by Mr. Tomita in Hihon-Densi-Keisan Co (Japan Electronic
	Computer Co.)  If you have problem, please read error message 
        carefully and check src/newsos.c.

Hitachi HI-UX
	HI-UX is an HP-UX running on Hitachi computer. HP-UX platform 
	dependent files can be used. Cint runs on Hitachi workstation and
	supercomputer.

NEC System-V MIPS-R4000
        CINT has been ported to NEC EWS4800 by Dr. Otofuji of Akita Univ, 
	Japan. Following is an installation note that he wrote.

        To install Cint, it is highly recommended that GCC is installed 
        formerly.  When you wish to use full (?) functions of Cint, it is 
        recommended that the version is newer than gcc-2.7.2 which support 
        shared library, and libg++.

        Instalation of Cint into NEC EWS4800 is somewhat tricky, because the 
        comliler does pot support fully position independent code.  Beacause 
        it is not able to find symbol in the executable, we use shared library
        in order to find symbols which are necessary in runtime.

        The following procedure shows how to install under the following 
        assumptions:
         o gcc, and g++ is installed at /usr/local/bin
         o libg++ is installed at /usr/local/lib as shared library.
         o whole programs are installed into /usr/local/lib/cint

        1. mkdir Cint
        2. goto directory Cint
        3. type "gunzip -c ../cint-5.12.2.tar.gz | tar xf -"
        4. rename 
             src/Makefile.base.nec      -> src/Makefile.base
             readline/Makefile.base.nec -> readline/Makefile.base
             Makefile.base.nec          -> Makefile.base
        5. As we use libg++ and src/libcint.so, set environment LD_LIBRARY_PATH
           as
             /usr/local/lib:`pwd`/src
        6. type "sh setup platform/nec_ux_gcc"
        7. copy necessary files to /usr/local/lib/cint. necessary files are
             C2cxx cxx2C G__ci.h MAKEINFO makecint lib/* include/* tool/*
             doc/*
        8. copy src/libcint.so to /usr/local/lib/cint/libcint.so.
        9. set LD_LIBRARY_PATH and LD_RUN_PATH as
             /usr/local/lib:/usr/local/lib/cint
        10. remove cint, and type "make cint". (In this step, it may be 
            necessary to change Makefile slightly.)
        11. copy cint to /usr/local/lib/cint.
        12. make shell /usr/local/bin/cint:
       
           #!/bin/sh
           CINTSYSDIR=/usr/local/lib/cint
           PATH=$CINTSYSDIR:$PATH
           export PATH CINTSYSDIR
           exec /usr/local/lib/cint/cint $*
 
Convex
	CINT has been ported to Convex. This work was done in Russia.
	There is no automated installation script.

Windows-NT/9x/Me/2000/Xp CYGWIN compiler with DLL (platform/cygwin51_gcc3_so)
   platform/cygwin51_gcc3_so
	Rommel Custodio and Joseph Canedo contributed CINT porting on
	CYGWIN. Shin-ya Okumura contributed for gcc3.x support. 
	platform/cygwin51_gcc3_so is a recommended platform file for Cygwin.
	Only this setup supports full functionality of Cint including dynamic
	linking.
   platform/cygwin1
   platform/cygwin2
   platform/cygwin3
	Those versions are old and works only for archived version. Use
	cygwin51_gcc3_so instead.


WINDOWS PORTING STATUS ======================================================

Windows-NT 3.51  Visual C++ 4.0         , Symantec C++ 7.2 
Windows-NT 4.0   Visual C++ 4.0/5.0/6.0 , Symantec C++ 7.2
Windows-9x/Me    Visual C++ 4.0/5.0/6.0 , Symantec C++ 7.2
Windows-2000     Visual C++ 6.0
	CINT can provide full functionality with Visual C++ 4.0. Refer to
        platform/visual/README. Valery Fine of CERN, Osamu Kotanikawa of 
        Fuji-Xerox company and a couple of other people ported CINT on 
        Visual C++. This is the most recommended compiler to use CINT on
	Windows-NT/9x.
	Automatic installation script is provided for Visual C++ 4.0 and
	Symantec C++ 7.2 for above Win32 based platforms. All of the features
	including Dynamic Linking of library is available. Please read 
	platform/visual/README.txt and platform/symantec/README.txt.
	There is a bug in Visual C++ 6.0 that the compiler can not handle
	exception handling in cint generated dictionary file. The bug was
	workedaround by '#ifndef G__VC60BUGFIX'. This macro must be defined
	when the bug is fixed in VC++ 6.0.
        From cint5.14.62, compile script for VC++ 6.0 and VC++5.0(and older)
        are separated. For older version use setupold.bat or setupNTold.bat.
        Refer to platform/visual/README.txt
	For Windows-2000 and Visual C++ 6.0, use platform/visual/setup2000.bat.

	This setup uses old iostream library(non-templated version). If you 
	want to use new templated iostream, see below.

Windows-Me/2000/XP   Visual C++ 6.0/7.x
Windows-7 	     Visual C++ 13.0
	Same as above except this setting uses templated version of iostream.
	Go to platform/visualCpp6 for VC++6.0 and platform/visualCpp7 for
	VC++7.0 and run setup.bat.  Refer to platform/visualCppX/README.txt

Windows-NT 3.5  Symantec C++
	Automatic installation script is provided for Symantec C++ 7.2. 
	All of the features including Dynamic Linking of library is available.
	Please read platform/symantec/README.txt. 
        Dynamic unlinking of DLL doesn't work due to Symantec's FreeLibrary() 
	bug.

Windows-NT/9x  Borland C++
        Osamu Kotanikawa of Fuji-Xerox company ported CINT on Borland C++.
        Automated installation script and project files are not provided with
        this package. Read "Windows-NT/9x" section of this file carefully to
	compile cint. Makecint is not available now, although it is 
	technically possible.

Windows-NT/9x/Me  Borland C++ Builder 3.0
	Automatic installation script is provided for C++ Builder 3.0.
	Most of the features including Dynamic Linking of library is 
	available. Please read platform/borland/README.txt.

Windows-NT/9x  Borland C++ Compiler 5.5
        Free version of Borland BCC32.exe comiler is available from 
        http://www.borland.com.  BCC32.exe version 5.5 is supported from
        cint5.15.53.  Automatic installation script is provided as 
        platform/borlandcc5/setup.bat. Most of the features including 
        Dynamic Linking of library is available. Please read 
        platform/borlandcc5/README.txt.

Windows-3.1
	Cint was once compiled on Windows-3.1 using Turbo-C++. However, it 
	did not perform well because of 64k limitations and incomplete stdio
	emulation. 

MS-DOS           DJGPP compiler
Windows-3.1      DJGPP compiler
Windows-NT/9x/Me DJGPP compiler
	DJGPP is a GNU compiler for MS-DOS 16bit operating system. It 
	provides 32bit C/C++ development environment on MS-DOS. Makefile 
	is provided as platform/djgpp/Makefile. There is an automatic
	installation script platform/djgpp/setup.bat
	Read platform/djgpp/README.txt.

Windows-NT/9x/Me CYGWIN compiler
	Use platform/cygwin51_gcc3_so or platform/cygwin51_gcc3_so_v6 to 
	compile Cint using Cygwin.

Windows-NT/9x other compilers ============================================

 Other C++ compiler can compile CINT by following procedure.

 1)  Creating directory
   Create c:\cint directory. 
   Environment variable CINTSYSDIR must be set to 'c:\cint'.

 2)  Copy and unpack cint source package
   If you get cint.tgz source code package, copy it under c:\cint and 
   unpack by gzip and tar.
     cd c:\cint
     copy a:\cint.tgz c:\cint\cint.tgz
     gzip -vd cint.tgz
     tar xvf cint.tar

 3) Create c:\cint\libcint.dll and c:\cint\libcint.lib
   Create a project to make libcint.dll and libcint.lib. Following 
   source files must be compiled and linked. In case of Borland C++ Builder,
   you may need to rename C++ source files from .cxx to .cpp.

     Setup
	IPATH = -I\cint -I\cint\src -I\cint\lib\stream
	  (in case of VC++  IPATH = -I\cint -I\cint\src -I\cint\lib\vcstream)
	  (in case of VC++7 IPATH = -I\cint -I\cint\src -I\cint\lib\iccstrm)
	  (in case of BC++  IPATH = -I\cint -I\cint\src -I\cint\lib\bcstream)
	  (in case of C++B  IPATH = -I\cint -I\cint\src -I\cint\lib\ccstream)
	MACRO = -DG__SHAREDLIB
	OBJECT = c:\cint\libcint.dll

     Source code Under c:\cint\src
		winnt.c
		???strm.cxx (bcstrm.cxx in case of Borland C++)
                            (cbstrm.cxx in case of C++ Builder)
                            (vcstrm.cxx in case of Visual C++4.0-6.0)
                            (iccstrm.cxx in case of Intel compiler or VC++7.0)
                            (gcc3strm.cxx in case of gcc3.00 or later)
                            (kccstrm.cxx in case of KCC compiler)
                            (libstrm.cxx for most of the other compilers)
                            (fakestrm.cxx in case libstrm.cxx fails)
		stdstrct.c  (dmystrct.c in case of failure)
		Api.cxx
		Apiif.cxx
		BaseCls.cxx
		CallFunc.cxx
		Class.cxx
		DataMbr.cxx
		Method.cxx
		MethodAr.cxx
		Type.cxx
		Typedf.cxx
		auxu.c
		cast.c
		debug.c
		decl.c
		disp.c
		dump.c
		end.c
		error.c
		expr.c
		fread.c
		func.c
		gcoll.c
		global1.c
		global2.c
		g__cfunc.c
		ifunc.c
		inherit.c
		init.c
		input.c
		intrpt.c
		loadfile.c
		macro.c
		malloc.c
		new.c
		newlink.c
		oldlink.c
		opr.c
		parse.c
		pause.c
		pcode.c
		pragma.c
		quote.c
		scrupto.c
		shl.c
		sizeof.c
		struct.c
		stub.c
		tmplt.c
		typedef.c
		val2a.c
		value.c
		var.c

   Then, build libcint.dll. If libcint.dll and libcint.lib are created 
   in a directory other than c:\cint, copy them as c:\cint\libcint.dll and
   c:\cint\libcint.lib.


 4) Create cint

   Create another project to compile cint.exe. You need 
   to compile and link following files.

		IPATH = -Ic:\cint
		MACRO =
		OBJECT = c:\cint\cint.exe

		c:\cint\libcint.lib
		c:\cint\main\cppmain.cxx
		c:\cint\main\G__setup.c

   Then, build cint.exe. If cint.exe is created in a directory other than
   c:\cint, copy it as c:\cint\cint.exe.


 5) Create header files

   Cint requires system adaptive standard header files in $CINTSYSDIR/include
   directory. You can create them automatically by include/mkincld.c
   Compile $CINTSYSDIR/include/mkincld.c to create include/mkincld.exe.
   Then run it in the $CINTSYSDIR/include directory. This will update 
   system dependent constants in the standard header files.


 6) Set environment variable

   Set an environment variable CINTSYSDIR to where you installed cint.
   Also set PATH as 'set PATH=%PATH%;%CINTSYSDIR%'.  You can set those
   variables in c:\autoexec.bat.  Now, you can use cint as stand-alone 
   C++ interpreter.


 7) How about makecint?

   You can not use makecint in Windows-NT environment now. But you can do
   equivalent by 'cint -c-1'.  Makecint automates following procedure for 
   UNIX environment which is not available in Windows-NT/9x. 

   You need to create interface routine source file. If you have C++ header 
   files A.h B.h C.h , do following. This will create G__cpplink.C and
   G__cpplink.h. You need to rename G__cpplink.C as G__cpplink.CPP if your
   compiler only accepts .CPP extension for C++ source file.

	$ cint -c-1 -D__MAKECINT__  A.h B.h C.h 
	$ mv G__cpplink.C G__cpplink.CPP

   Compile G__cpplink.CPP. Compile main/G__setup.c with '-DG__CPPLINK_ON' 
   option.  Compile source files A.CPP B.CPP and C.CPP.
   Now you should have following objects and library. Link them all to make
   an executable.

	IPATH = -I $CINTSYSDIR
	MACRO = -DG__CPPLINK_ON
	OBJECT = ABC.exe

	A.CPP
	B.CPP
	C.CPP
	G__cpplink.CPP
	$CINTSYSDIR/main/G__setup.c
	$CINTSYSDIR/main/G__cppmain.OBJ
	$CINTSYSDIR/src/G__ci.lib
	

   If you have C header files D.h E.h F.h , do following. This will create
   G__clink.c and G__clink.h.

	$ cint -c-2 -K -D__MAKECINT__ D.h E.h F.h
 
   Compile G__clink.c. Compile main/G__setup.c with '-DG__CLINK_ON' option.
   Compile source files D.c E.c F.c.
   Now you should have following objects and library. Link them all to make
   an executable.

	IPATH = -I $CINTSYSDIR
	MACRO = -DG__CLINK_ON
	OBJECT = DEF.exe

	D.c 
	E.c
	F.c
	G__clink.c
	$CINTSYSDIR/main/G__setup.c
	$CINTSYSDIR/main/G__cppmain.OBJ
	$CINTSYSDIR/src/G__ci.lib

   If you have both C++ and C header files, you need to compile 
   main/G__setup.c with '-DG__CPPLINK_ON -DG__CLINK_ON' option.


MacOS and BeOS PORTING STATUS ==============================================

MacOS
BeOS
	CINT has been ported to MacOS and BeOS by Fons Rademakers of CERN. 
	There is no automated installation scripts for these platforms.
	Contact rdm@hpsalo.cern.ch if you need detail information.

NextStep PORTING STATUS ====================================================

NextStep
	There was a report that Cint has been ported to NextStep with small
	modification. No detail information.

VMS PORTING STATUS =========================================================

VMS
	CINT has been ported to VMS by Kaori Wada and Kris Hagel in
	Cyclotron Institute, Texas A & M University. They also ported
	ROOT.

TROUBLE SHOOTING ===========================================================

'if' statement in Makefile:
   In some computer platform, 'make' does not take 'if' statement in 
   Makefile. In such case, you need to remove 'if's from Makefile.base and
   src/Makefile.base prior to running installation script.

C++ source file extention:
   If your compiler does not accept *.cxx as C++ source file, rename those
   files and modify $CINTSYSDIR/src/Makefile.base and
   $CINTSYSDIR/Makefile.base before running setup. This work is automated by
   $CINTSYSDIR/cxx2C shell script.

GNU reaadline library compile or link errors:
   CINT uses old version of GNU readline library. This library uses old BSD
   style library which may be a problem for some new platforms. If you find
   problems compiling and linking libreadline.a, you can try following 
   workaround.

   1) If you have newer version of libreadline.a somewhere in your computer,
   let READLINEA in platform dependency file point it. For example, If you 
   have /usr/local/lib/libreadline.a 
	READLINEA   = /usr/local/lib/libreadline.a

   2) You can install CINT without GNU readline library. Please eliminate
   READLINEA variable in platform dependency file. 
	READLINEA   = 

libstrm.cxx compile error:
   libstrm.cxx may not compile in some platform. libstrm.cxx is an interface
   to real iostream library. Many details of the iostream varies from system 
   to system. There are several solutions to this problem. In case of VC++,
   you must use vcstrm.cxx instead of libstrm.cxx.

   1) Give up using fake iostream library provided by cint

    This is the simplest solution. You simply edit platform dependency file
    modifying STDLIBS variable as follows.

      STDLIBS    = libstrm.o stdstrct.o
                     |
                     V
      STDLIBS    = fakestrm.o stdstrct.o

   2) Modifying libstrm.cxx

    You could modify src/libstrm.cxx by hand to clear the problem. Commenting
    out specific lines will not cause significant problem if you do not use
    iostream deeply.

   3) Create libstrm.cxx for your environment

    libstrm.cxx is generated by cint -c-1 interface method generator. This
    procedure is similar to making interface method of user specific 
    precompiled library. Example of how you can make libstrm.cxx is in 
    lib/stream directory. Refer to lib/stream/README.

makecint:
    Makecint doesn't work in some environment mainly due to bug in the
    compilers. Following are most likely compiler bugs.

    1) static operator delete
    Makecint creates static operator delete in G__cpp_XXX.C file. Some
    compilers do not take this. Solution to this is to eliminate 
    G__N_EXPLICITDESTRUCTOR macro in src/newlink.c and recompile CINT.

    2) Casting of overloaded function
    CINT uses casting of pointer to overloaded function in G__cpp_XXX.C.
    This can be done either by casting or declaration. Some compilers can
    not handle casting to overloaded function or declaration of pointer to
    overloaded function. You can choose implementation by setting OTHMACRO
    variable in platform dependency file either as -DG__P2FDECL or 
    -DG__P2FCAST. It is OK defining neither.

Dynamic Linking of Precompiled Library:
    Dynamic Linking of Percompiled Library should work in most of the 
    environment. You may find problems, however, due to wrong compiler and
    linker option settings in platform dependency file. Your contribution to
    improve this feature is greatly appreciated.
