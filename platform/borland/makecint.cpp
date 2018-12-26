
#pragma hdrstop
#include <condefs.h>

USEUNIT("libmake.c");
//---------------------------------------------------------------------------
extern "C" int G__makecint(int argc,char** argv);
//---------------------------------------------------------------------------
#pragma argsused
int main(int argc, char **argv)
{
        return(G__makecint(argc,argv));
}
