
copy ..\..\include\RegExp.h RegExp.h
copy ..\..\include\RegExp.cxx RegExp.cxx
copy ..\..\include\ReadF.h ReadF.h
copy ..\..\include\ReadF.cxx ReadF.cxx
makecint -mk makevcnet -dl excel.dll -H Excel.h ReadF.h RegExp.h Stat.h Value.h -C++ Excel.cxx ReadF.cxx RegExp.cxx Stat.cxx Value.cxx

