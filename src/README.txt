src/README.txt

# Cint 6.x Major re-engineering

 Re-engineering source file names start with "bc_"

    bc_autoobj.cxx/.h
    bc_cfunc.cxx/.h
    bc_inst.cxx/.h
    bc_parse.cxx/.h
    bc_reader.cxx/.h
    bc_type.cxx/.h

    bc_dict.cxx/.h  : Legacy Cint dictionary for debugging purpose


NOTE: 
  PLEASE DO NOT TAKE CINT SOURCE CODE AS PROGRAMMING EXAMPLE OR REFERENCEE
 OF AN IMTERPRETER IMPLEMENTATION. CINT IS A CHUNK OF LEGACY CODE WHICH IS 
 SUBJECT TO REENGINEERING. IT IS SAFER TO USE CINT ONLY FROM OUTSIDE.


 This directory contains CINT - C++ interpreter core source code.
Source files are categorized as follows.

# ERTTI API source files
  ERTTI (Extensive Run Time Type Identification) API provides means of 
 inspecting symbol table and class structure of interpreted source code.
 Cint symbol table is encapsureated in 9 C++ classes.  Refer to following
 source code and corresponding header files for ERTTI specs. Also, refer
 to doc/ref.txt.

     Api.cxx
     BaseCls.cxx
     CallFunc.cxx
     Class.cxx
     DataMbr.cxx
     Method.cxx
     MethodAr.cxx
     Token.cxx
     Type.cxx
     Typedf.cxx

  Apiif.cxx contains interface method for ERTTI APIs. This file is generated
 by 'cint -c-1'.

     Apiif.cxx 


# CINT - C++ interpreter core
  Following source files are the core of the interpreter. Those files are
 written in K&R style C for portability reason.  
  Please do not regard those source files as programming example or 
 implementation reference. I started to write Cint when I wasn't 
 knowledgeble to modern programming methodology. 
  All of the global names start with 'G__' to avoid name conflict. Macro
 G__OLDIMPLEMENTATION??? are used for revision control. G__OLDIMPLEMENTATION???
 are usually not defined.

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
     g__cfunc.c
     gcoll.c
     global1.c
     global2.c
     ifunc.c
     inherit.c
     init.c
     input.c
     intrpt.c
     loadfile.c
     macro.c
     malloc.c
     memtest.c
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

  Files named after specific computer platform are only needed for 
 corresponding environment. 

     macos.c
     newsos.c
     sunos.c
     winnt.c

# Revision history
  HISTORY file contains source code modification history. Macro
 G__OLDIMPLEMENTATION??? are used to track modification history within
 single set of source files.

     HISTORY

# C++ iostream library interface method
  Following source files include C++ iostream library interface method.
 In most cases, libstrm.cxx works, however, special version is needed
 for Visual C++ and Borland C++. If non of following files work, use 
 fakestrm.cxx.  dmystrm.cxx is provided for C-compiler-only installation.
 Refer platform/README.txt for more detail.

     libstrm.cxx     # HP-UX, Linux, Solaris, AIX, IRIX, gcc 2.x, etc,...
     gcc3strm.cxx    # gcc 3.x
     sun5strm.cxx    # Solaris, Sun C++ Compiler 5.x
     vcstrm.cxx      # Visual C++ 4.0/5.0/6.0
     cbstrm.cpp      # Borland C++ Builder 3.0, Borland C++ compiler 5.5
     bcstrm.cxx      # Borland C++ (older version)
     kccstrm.cxx     # KAI C++ compiler
     iccstrm.cxx     # Intel C++ compiler
     fakestrm.cxx    # Dummy for C++ compiler
     dmystrm.c       # Dummy for C compiler

# ANSI C standard library struct interface method
  stdstrct.c is needed to build cint. It contains inferface method for
 defining struct.

     stdstrct.c
     dmystrct.c

