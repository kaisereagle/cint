rem # copy ..\..\include\Stat.h Stat.h
rem # copy ..\..\include\Stat.cxx Stat.cxx
copy Stat.h   ..\..\include\Stat.h 
copy Stat.cxx ..\..\include\Stat.cxx 
copy ..\..\include\ReadF.h ReadF.h
copy ..\..\include\ReadF.cxx ReadF.cxx
copy ..\..\include\MultiByteChar.h MultiByteChar.h
copy ..\..\include\MultiByteChar.cxx MultiByteChar.cxx


makecint -mk makevcnet -dll NVector.dll -DDISPFILE -H Vector.h Complex.h Fft.h Lsm.h SparseVector.h Histogram.h Stat.h Gauss.h Vectoriostream.h  VirtualGraph.h cintgraph.h ReadF.h LinkDef.h -C++ Fft.cxx Lsm.cxx Stat.cxx Gauss.cxx VirtualGraph.cxx cintgraph.cxx Vectoriostream.cxx ReadF.cxx MultiByteChar.cxx

nmake -f makevcnet CFG="NVector - Win32 Release"
del ..\..\include\NVector.dll
copy Release\NVector.dll ..\..\include\NVector.dll 
nmake -f makevcnet CFG="NVector - Win32 Release" clean
del Release\*
del NVector.dll
rmdir Release
