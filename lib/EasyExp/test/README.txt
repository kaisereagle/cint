
EasyExp, Scandir, EasyExp Synthesis Module test using CINT
Sep 2015, Masaharu Goto

# Prerequisite
 - You have C++ compiler , VC++13, gcc4.x
 - CINT C++ interpreter  5.15.176 or later

# Build target 

 Linux,  MacOS 
   $  cd $CINTSYSDIR/lib/EasyExp
   $  sh setup

 Windows
   C:>  cd %CINTSYSDIR%\lib\EasyExp
   C:>  setup.bat


# Running tests

 Linux, MacOS
   $  cd $CINTSYSDIR/lib/EasyExp/test
   $  cint testall.cxx -p 

 Windows
   C:>  cd %CINTSYSDIR%\lib\EasyExp\test
   C:>  cint testall.cxx -p 

 Caution: 
   The test runs without -p option, however, line number of the error may 
  be reported incorrectly.
  
