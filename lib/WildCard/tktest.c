#include <stdio.h>
#include <tcl.h>


main() {
  test();
  WildCard_MainLoop();
}

/***************************************************************************
* test function
***************************************************************************/
int test() {
  Tcl_Interp *interp;
  interp = Tcl_CreateInterp();

  Tk_Window tkwin;
  tkwin=Tk_CreateMainWindow(interp,"unix:0.0","appName","className");

  Tk_Window button;
  button=Tk_CreateWindowFromPath(interp,tkwin,".appName","unix:0.0");
    
  Tk_Window what;
  what = Tk_NameToWindow(interp,".appName",tkwin);
  what = Tk_NameToWindow(interp,".",tkwin);

  Tk_MainLoop();

  Tk_DestroyWindow(tkwin);
}

