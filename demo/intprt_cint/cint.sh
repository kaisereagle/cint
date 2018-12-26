echo 'CINT:: This is very slow. CINT interpreter interprets itself.'
echo 
cint.exe -I$CINTSYSDIR -I$CINTSYSDIR/src +P testmain.c $*

