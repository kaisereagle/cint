File: $CINTSYSDIR/demo/graphs/README

 Demonstration of scientific calculation and XY graph plotting. You need 
'xgraph' utility and X window to run these scripts. These examples show
you good examples of how C++ scripts look like.

	$ cint eular.c
	$ cint fir.c
	$ cint fir2.c
	$ cint stripline.c
	    .
	    .

 Stream object for ROOT graphics is supported from .
 X-Y Graph demo can also be used on ROOT. 

       $ root
       root[] #include "eular.c"
       root[] main()
       root[] #include "fir.c"
       root[] main()
            .
            .

 Or

	$ root -l eular.c
	$ root -l fir.c
	$ root -l fir2.c
	$ root -l stripline.c
	    .
	    .

  root -l option supress showing splash screen
