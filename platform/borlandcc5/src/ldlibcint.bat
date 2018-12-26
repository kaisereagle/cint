bcc32 -WD  -elibcint.dll  cbstrm.obj stdstrct.obj Api.obj Class.obj BaseCls.obj Type.obj DataMbr.obj Method.obj MethodAr.obj CallFunc.obj Typedf.obj Apiif.obj Token.obj auxu.obj cast.obj debug.obj decl.obj disp.obj dump.obj end.obj error.obj expr.obj fread.obj func.obj gcoll.obj global1.obj global2.obj g__cfunc.obj ifunc.obj inherit.obj init.obj input.obj intrpt.obj loadfile.obj macro.obj malloc.obj memtest.obj new.obj newlink.obj oldlink.obj opr.obj parse.obj pause.obj pcode.obj pragma.obj quote.obj scrupto.obj shl.obj sizeof.obj struct.obj stub.obj tmplt.obj typedef.obj val2a.obj value.obj var.obj \cint\main\G__setup.obj winnt.obj
implib -c libcint.lib libcint.dll
move libcint.dll ..\libcint.dll
move libcint.lib ..\libcint.lib
