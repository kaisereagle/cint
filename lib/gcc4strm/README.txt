lib/gcc4strm/README

 lib/gcc4strm directory exists for creating iostream library linkage file
src/gcc4strm.cxx and src/gcc4strm.h for g++ 3.00 compiler. These files 
contain interface methods for iostream library. You can create those by 
doing 'make' under this directory. Usually nobody but only author should 
do this. User doesn't need to recognize this.
 Files in this directory are originally copied from lib/snstream/* and 
modified for gcc4strm.

 cbstream.cpp is based on template based stream library.

 Creating src/gcc4strm.cxx

 1) Just do 'make' in this directory. 
  I think we can forget all below because of new new/delete scheme.

 ...................................................................

 1) Edit src/newlink.c
  Start a text editor, look for '#define G__N_EXPLICITDESTRUCTOR'. Please
  comment this line out. 

 2) Create special 'cint'
  Go back to $CINTSYSDIR and do make to create special 'cint'. You may need
  to specify src/fakestrm.cxx in src/Makefile.

 3) Come to lib/gcc4strm directory and do make to create src/gcc4strm.cxx.

 4) Edit src/newlink.c
  Edit src/newlink.c and restore '#define G__N_EXPLICITDESTRUCTOR' macro.

 5) Go back to $CINTSYSDIR and do make to create updated cint.
