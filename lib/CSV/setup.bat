
copy ..\..\include\EasyExp.cxx EasyExp.cxx
copy ..\..\include\EasyExp.h EasyExp.h
copy ..\..\include\StatEasyExp.cxx StatEasyExp.cxx
copy ..\..\include\StatEasyExp.h StatEasyExp.h
copy ..\..\include\ReadF.cxx ReadF.cxx
copy ..\..\include\ReadF.h ReadF.h
copy ..\..\include\MultiByteChar.h MultiByteChar.h
copy ..\..\include\MultiByteChar.cxx MultiByteChar.cxx
copy ..\..\lib\posix\winposix.c winposix.c
copy ..\..\lib\posix\winposix.h winposix.h
copy ..\..\lib\NVector\Stat.h Stat.h
copy ..\..\lib\NVector\Stat.cxx Stat.cxx
copy ..\..\lib\NVector\Gauss.h Gauss.h
copy ..\..\lib\NVector\Gauss.cxx Gauss.cxx
copy ..\..\lib\NVector\Vector.h Vector.h
copy ..\..\lib\NVector\Histogram.h Histogram.h
copy ..\..\lib\NVector\Vectoriostream.h Vectoriostream.h 
copy ..\..\lib\NVector\Vectoriostream.cxx Vectoriostream.cxx 
copy ..\..\lib\NVector\VirtualGraph.cxx VirtualGraph.cxx
copy ..\..\lib\NVector\VirtualGraph.h VirtualGraph.h
copy ..\..\lib\NVector\cintgraph.h cintgraph.h
copy ..\..\lib\NVector\cintgraph.cxx cintgraph.cxx
copy ..\..\lib\NVector\Complex.h Complex.h

makecint -mk Makevcnet -dl CSV.dll -I ..\.. -DDISPFILE -D __CINTLIB__  -DUNIQUE_HIST -DNOCOMPLEX -DNOMINMAXCLUSTERING -H Collection.h FileStrCollection.h ReadF.h EasyExp.h StatEasyExp.h Stat.h Gauss.h Value.h Clustering.h icsv.h csviostream.h LinkDef.h -C++ Collection.cxx FileStrCollection.cxx ReadF.cxx EasyExp.cxx StatEasyExp.cxx Stat.cxx Gauss.cxx Value.cxx Clustering.cxx icsv.cxx Vectoriostream.cxx csviostream.cxx VirtualGraph.cxx cintgraph.cxx MultiByteChar.cxx -C winposix.c 

nmake CFG="CSV - Win32 Release" -f makevcnet
del ..\..\include\CSV.dll
move Release\CSV.dll ..\..\include\CSV.dll

nmake CFG="CSV - Win32 Release" -f makevcnet clean
del release\*
del make.bat
del G__*
del CSV.dll
rmdir release
