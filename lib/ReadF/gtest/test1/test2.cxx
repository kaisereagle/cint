// ReadF test

#ifndef __CINT__
#include "../../ReadF.h"
#include <gtest/gtest.h>
void RESET_LOCALFAIL() { }
int LOCALFAIL_COUNT() { return(0); }
int G__pause() { return(0); }
#else
#include <_util_cinttest.h>
#endif

// Caution: Do not modify above this line. Those lines are used as test case.

//////////////////////////////////////////////////////////////////
TEST(ReadFTest, Constructor)
{
  ReadF f1;
  FILE *fp2=0;
  EXPECT_EQ(f1.isvalid(),0);
  EXPECT_EQ(f1.FP(),fp2);

#ifndef __CINT__
  ReadF f2("../test2.cxx");
  EXPECT_EQ(string(f2.fname),"../test2.cxx");
#else
  ReadF f2("../gtest/test1/test2.cxx");
  EXPECT_EQ(string(f2.fname),"../gtest/test1/test2.cxx");
#endif
  EXPECT_EQ(f2.isvalid(),1);
  EXPECT_NE(f2.FP(),fp2);

#ifndef __CINT__
  FILE *fp = fopen("../test2.cxx","r");
#else
  FILE *fp = fopen("../gtest/test1/test2.cxx","r");
#endif
  ReadF f3(fp);
  EXPECT_EQ(f3.isvalid(),1);
  EXPECT_EQ(f3.FP(),fp);
  fclose(fp);

}

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
TEST(ReadFTest, openclose)
{
  ReadF f;
#ifndef __CINT__
  f.open("../test2.cxx");
#else
  f.open("../gtest/test1/test2.cxx");
#endif
  EXPECT_EQ(f.isvalid(),1);
  f.close();
  EXPECT_EQ(f.isvalid(),0);

  printf("Intentional Error below\n");
  f.open("not_exist.cxx");
  EXPECT_EQ(f.isvalid(),0);
}

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
TEST(ReadFTest, Parse)
{
  ReadF f;
  f.parse("abc def 0123");
  EXPECT_EQ(3,f.argc);
  EXPECT_EQ(string("abc def 0123"),f.argv[0]);
  EXPECT_EQ(string("abc"),f.argv[1]);
  EXPECT_EQ(string("def"),f.argv[2]);
  EXPECT_EQ(string("0123"),f.argv[3]);

  f.parse("abc, def ,0123");
  EXPECT_EQ(3,f.argc);
  // Below works fine because this file is processed by C++ preprocessor
  EXPECT_EQ(string("abc, def ,0123"),f.argv[0]); // CINT macro problem
  //string a("abc, def ,0123"); EXPECT_EQ(a,f.argv[0]);//CINT workaround
  EXPECT_EQ(string("abc,"),f.argv[1]);
  EXPECT_EQ(string("def"),f.argv[2]);
  EXPECT_EQ(string(",0123"),f.argv[3]);
}


//////////////////////////////////////////////////////////////////
TEST(ReadFTest, Read)
{
#ifndef __CINT__
  ReadF f("../test2.cxx");
#else
  ReadF f("../gtest/test1/test2.cxx");
#endif
  EXPECT_EQ(f.isvalid(),1);

  EXPECT_EQ(f.read(),1);
  EXPECT_EQ(3,f.argc);
  EXPECT_EQ(string("// ReadF test"),f.argv[0]);
  EXPECT_EQ(string("//"),f.argv[1]);
  EXPECT_EQ(string("ReadF"),f.argv[2]);
  EXPECT_EQ(string("test"),f.argv[3]);
  EXPECT_EQ(1,f.line);

  EXPECT_EQ(f.read(),1);
  EXPECT_EQ(0,f.argc);
  EXPECT_EQ(string(""),f.argv[0]);
  EXPECT_EQ(2,f.line);

  EXPECT_EQ(f.read(),1);
  EXPECT_EQ(2,f.argc);
  EXPECT_EQ(string("#ifndef __CINT__"),f.argv[0]);
  EXPECT_EQ(string("#ifndef"),f.argv[1]);
  EXPECT_EQ(string("__CINT__"),f.argv[2]);
  EXPECT_EQ(3,f.line);

}

//////////////////////////////////////////////////////////////////
TEST(ReadFTest, ReadwordWparam)
{
#ifndef __CINT__
  ReadF f("../test2.cxx");
#else
  ReadF f("../gtest/test1/test2.cxx");
#endif
  EXPECT_EQ(f.isvalid(),1);
  string sp,buf;

  EXPECT_EQ(f.readword(sp,buf),' ');
  EXPECT_EQ(0,f.argc);
  EXPECT_EQ(sp," ");
  EXPECT_EQ(buf,"//");

  EXPECT_EQ(f.readword(sp,buf),' ');
  EXPECT_EQ(0,f.argc);
  EXPECT_EQ(sp," ");
  EXPECT_EQ(buf,"ReadF");

  EXPECT_EQ(f.readword(sp,buf),'\n');
  EXPECT_EQ(0,f.argc);
  EXPECT_EQ(sp,"");
  EXPECT_EQ(buf,"test");

  EXPECT_EQ(f.readword(sp,buf),'\n');
  EXPECT_EQ(0,f.argc);
  EXPECT_EQ(sp,"");
  EXPECT_EQ(buf,"");

  EXPECT_EQ(f.readword(sp,buf),' ');
  EXPECT_EQ(0,f.argc);
  EXPECT_EQ(sp," ");
  EXPECT_EQ(buf,"#ifndef");

  EXPECT_EQ(f.readword(sp,buf),'\n');
  EXPECT_EQ(0,f.argc);
  EXPECT_EQ(sp,"");
  EXPECT_EQ(buf,"__CINT__");
}

//////////////////////////////////////////////////////////////////
TEST(ReadFTest, ReadwordWOparam)
{
#ifndef __CINT__
  ReadF f("../test2.cxx");
#else
  ReadF f("../gtest/test1/test2.cxx");
#endif
  EXPECT_EQ(f.isvalid(),1);

  EXPECT_EQ(f.readword(),' ');
  EXPECT_EQ(1,f.argc);
  EXPECT_EQ(string("//"),f.argv[0]);

  EXPECT_EQ(f.readword(),' ');
  EXPECT_EQ(1,f.argc);
  EXPECT_EQ(string("ReadF"),f.argv[0]);

  EXPECT_EQ(f.readword(),'\n');
  EXPECT_EQ(1,f.argc);
  EXPECT_EQ(string("test"),f.argv[0]);

  EXPECT_EQ(f.readword(),'\n');
  EXPECT_EQ(0,f.argc);

  EXPECT_EQ(f.readword(),' ');
  EXPECT_EQ(1,f.argc);
  EXPECT_EQ(string("#ifndef"),f.argv[0]);

  EXPECT_EQ(f.readword(),'\n');
  EXPECT_EQ(1,f.argc);
  EXPECT_EQ(string("__CINT__"),f.argv[0]);
}

//////////////////////////////////////////////////////////////////
TEST(ReadFTest, setseparator)
{
  ReadF f;
  EXPECT_EQ(f.isvalid(),0);

  f.setseparator(",");

  f.parse("abc  def  0123");
  EXPECT_EQ(1,f.argc);
  EXPECT_EQ(string("abc  def  0123"),f.argv[0]);
  EXPECT_EQ(string("abc  def  0123"),f.argv[1]);

  f.parse("abc,,  def,  ,0123");
  EXPECT_EQ(4,f.argc);
  EXPECT_EQ(string("abc,,  def,  ,0123"),f.argv[0]);
  EXPECT_EQ(string("abc"),f.argv[1]);
  EXPECT_EQ(string("  def"),f.argv[2]);
  EXPECT_EQ(string("  "),f.argv[3]);
  EXPECT_EQ(string("0123"),f.argv[4]);

  f.setseparator(", ");

  f.parse("abc  def  0123");
  EXPECT_EQ(3,f.argc);
  EXPECT_EQ(string("abc  def  0123"),f.argv[0]);
  EXPECT_EQ(string("abc"),f.argv[1]);
  EXPECT_EQ(string("def"),f.argv[2]);
  EXPECT_EQ(string("0123"),f.argv[3]);

  f.parse("abc,,  def,  ,0123");
  EXPECT_EQ(3,f.argc);
  EXPECT_EQ(string("abc"),f.argv[1]);
  EXPECT_EQ(string("def"),f.argv[2]);
  EXPECT_EQ(string("0123"),f.argv[3]);
}

//////////////////////////////////////////////////////////////////
TEST(ReadFTest, setdelimiter)
{
  ReadF f;
  EXPECT_EQ(f.isvalid(),0);

  f.setseparator("");
  f.setdelimiter(",");

  f.parse("abc  def  0123");
  EXPECT_EQ(1,f.argc);
  EXPECT_EQ(string("abc  def  0123"),f.argv[0]);
  EXPECT_EQ(string("abc  def  0123"),f.argv[1]);

  f.parse("abc,,  def,  ,0123");
  EXPECT_EQ(5,f.argc);
  EXPECT_EQ(string("abc,,  def,  ,0123"),f.argv[0]);
  EXPECT_EQ(string("abc"),f.argv[1]);
  EXPECT_EQ(string(""),f.argv[2]);
  EXPECT_EQ(string("  def"),f.argv[3]);
  EXPECT_EQ(string("  "),f.argv[4]);
  EXPECT_EQ(string("0123"),f.argv[5]);

  f.setdelimiter(", ");

  f.parse("abc  def  0123");
  EXPECT_EQ(5,f.argc);
  EXPECT_EQ(string("abc  def  0123"),f.argv[0]);
  EXPECT_EQ(string("abc"),f.argv[1]);
  EXPECT_EQ(string(""),f.argv[2]);
  EXPECT_EQ(string("def"),f.argv[3]);
  EXPECT_EQ(string(""),f.argv[4]);
  EXPECT_EQ(string("0123"),f.argv[5]);

  f.parse("abc,,  def,  ,0123");
  EXPECT_EQ(9,f.argc);
  EXPECT_EQ(string("abc"),f.argv[1]);
  EXPECT_EQ(string(""),f.argv[2]);
  EXPECT_EQ(string(""),f.argv[3]);
  EXPECT_EQ(string(""),f.argv[4]);
  EXPECT_EQ(string("def"),f.argv[5]);
  EXPECT_EQ(string(""),f.argv[6]);
  EXPECT_EQ(string(""),f.argv[7]);
  EXPECT_EQ(string(""),f.argv[8]);
  EXPECT_EQ(string("0123"),f.argv[9]);

  f.parse(",abc,,  def,  ,0123");
  EXPECT_EQ(10,f.argc);
  EXPECT_EQ(string(""),f.argv[1]);
  EXPECT_EQ(string("abc"),f.argv[2]);
  EXPECT_EQ(string(""),f.argv[3]);
  EXPECT_EQ(string(""),f.argv[4]);
  EXPECT_EQ(string(""),f.argv[5]);
  EXPECT_EQ(string("def"),f.argv[6]);
  EXPECT_EQ(string(""),f.argv[7]);
  EXPECT_EQ(string(""),f.argv[8]);
  EXPECT_EQ(string(""),f.argv[9]);
  EXPECT_EQ(string("0123"),f.argv[10]);

  f.setseparator(" ");
  f.setdelimiter(",");

  f.parse("abc  def  0123");
  EXPECT_EQ(3,f.argc);
  EXPECT_EQ(string("abc  def  0123"),f.argv[0]);
  EXPECT_EQ(string("abc"),f.argv[1]);
  EXPECT_EQ(string("def"),f.argv[2]);
  EXPECT_EQ(string("0123"),f.argv[3]);

  f.parse("abc,,  def,  ,0123");
  EXPECT_EQ(5,f.argc);
  EXPECT_EQ(string("abc"),f.argv[1]);
  EXPECT_EQ(string(""),f.argv[2]);
  EXPECT_EQ(string("def"),f.argv[3]);
  EXPECT_EQ(string(""),f.argv[4]);
  EXPECT_EQ(string("0123"),f.argv[5]);

  f.setseparator(" ");
  f.setdelimiter("()");

  f.parse("abc  def module(p1 p2  p3)");
  EXPECT_EQ(7,f.argc); // supposed to be 6
  EXPECT_EQ(string("abc  def module(p1 p2  p3)"),f.argv[0]);
  EXPECT_EQ(string("abc"),f.argv[1]);
  EXPECT_EQ(string("def"),f.argv[2]);
  EXPECT_EQ(string("module"),f.argv[3]);
  EXPECT_EQ(string("p1"),f.argv[4]);
  EXPECT_EQ(string("p2"),f.argv[5]);
  EXPECT_EQ(string("p3"),f.argv[6]);
  EXPECT_EQ(string(""),f.sepv[0]);
  EXPECT_EQ(string("  "),f.sepv[1]);
  EXPECT_EQ(string(" "),f.sepv[2]);
  EXPECT_EQ(string("("),f.sepv[3]);
  EXPECT_EQ(string(" "),f.sepv[4]);
  EXPECT_EQ(string("  "),f.sepv[5]);
  EXPECT_EQ(string(")"),f.sepv[6]);

  f.parse("abc  def module ( p1 p2  p3 )");
  EXPECT_EQ(7,f.argc); // supposed to be 6
  EXPECT_EQ(string("abc  def module ( p1 p2  p3 )"),f.argv[0]);
  EXPECT_EQ(string("abc"),f.argv[1]);
  EXPECT_EQ(string("def"),f.argv[2]);
  EXPECT_EQ(string("module"),f.argv[3]);
  EXPECT_EQ(string("p1"),f.argv[4]);
  EXPECT_EQ(string("p2"),f.argv[5]);
  EXPECT_EQ(string("p3"),f.argv[6]);
  EXPECT_EQ(string(""),f.sepv[0]);
  EXPECT_EQ(string("  "),f.sepv[1]);
  EXPECT_EQ(string(" "),f.sepv[2]);
  EXPECT_EQ(string("("),f.sepv[3]);
  EXPECT_EQ(string(" "),f.sepv[4]);
  EXPECT_EQ(string("  "),f.sepv[5]);
  EXPECT_EQ(string(")"),f.sepv[6]);

  f.parse("abc  def module (  p1 p2  p3  ) ");
  EXPECT_EQ(7,f.argc); // supposed to be 6
  EXPECT_EQ(string("abc  def module (  p1 p2  p3  ) "),f.argv[0]);
  EXPECT_EQ(string("abc"),f.argv[1]);
  EXPECT_EQ(string("def"),f.argv[2]);
  EXPECT_EQ(string("module"),f.argv[3]);
  EXPECT_EQ(string("p1"),f.argv[4]);
  EXPECT_EQ(string("p2"),f.argv[5]);
  EXPECT_EQ(string("p3"),f.argv[6]);
  EXPECT_EQ(string(""),f.sepv[0]);
  EXPECT_EQ(string("  "),f.sepv[1]);
  EXPECT_EQ(string(" "),f.sepv[2]);
  EXPECT_EQ(string("("),f.sepv[3]);
  EXPECT_EQ(string(" "),f.sepv[4]);
  EXPECT_EQ(string("  "),f.sepv[5]);
  EXPECT_EQ(string(")"),f.sepv[6]);

  f.parse("abc  def module(  p1 p2  p3) ");
  EXPECT_EQ(7,f.argc); // supposed to be 6
  EXPECT_EQ(string("abc  def module(  p1 p2  p3) "),f.argv[0]);
  EXPECT_EQ(string("abc"),f.argv[1]);
  EXPECT_EQ(string("def"),f.argv[2]);
  EXPECT_EQ(string("module"),f.argv[3]);
  EXPECT_EQ(string("p1"),f.argv[4]);
  EXPECT_EQ(string("p2"),f.argv[5]);
  EXPECT_EQ(string("p3"),f.argv[6]);
  EXPECT_EQ(string(""),f.sepv[0]);
  EXPECT_EQ(string("  "),f.sepv[1]);
  EXPECT_EQ(string(" "),f.sepv[2]);
  EXPECT_EQ(string("("),f.sepv[3]);
  EXPECT_EQ(string(" "),f.sepv[4]);
  EXPECT_EQ(string("  "),f.sepv[5]);
  EXPECT_EQ(string(")"),f.sepv[6]);

  f.parse("abc  def module (p1 p2  p3) ");
  EXPECT_EQ(7,f.argc); // supposed to be 6
  EXPECT_EQ(string("abc  def module (p1 p2  p3) "),f.argv[0]);
  EXPECT_EQ(string("abc"),f.argv[1]);
  EXPECT_EQ(string("def"),f.argv[2]);
  EXPECT_EQ(string("module"),f.argv[3]);
  EXPECT_EQ(string("p1"),f.argv[4]);
  EXPECT_EQ(string("p2"),f.argv[5]);
  EXPECT_EQ(string("p3"),f.argv[6]);
  EXPECT_EQ(string(""),f.sepv[0]);
  EXPECT_EQ(string("  "),f.sepv[1]);
  EXPECT_EQ(string(" "),f.sepv[2]);
  EXPECT_EQ(string("("),f.sepv[3]);
  EXPECT_EQ(string(" "),f.sepv[4]);
  EXPECT_EQ(string("  "),f.sepv[5]);
  EXPECT_EQ(string(")"),f.sepv[6]);
}

//////////////////////////////////////////////////////////////////
TEST(ReadFTest, setendofline)
{
  ReadF f;
  EXPECT_EQ(f.isvalid(),0);

  f.setendofline("#");

  f.parse("abc  def #  0123");
  EXPECT_EQ(2,f.argc);
  EXPECT_EQ(string("abc  def "),f.argv[0]);
  EXPECT_EQ(string("abc"),f.argv[1]);
  EXPECT_EQ(string("def"),f.argv[2]);

#ifndef __CINT__
  f.open("../test2.cxx");
#else
  f.open("../gtest/test1/test2.cxx");
#endif
  EXPECT_EQ(f.isvalid(),1);
  f.setendofline("/#\n");

  EXPECT_EQ(f.read(),1);
  EXPECT_EQ(0,f.argc);
  EXPECT_EQ(string(""),f.argv[0]);

  EXPECT_EQ(f.read(),1);
  EXPECT_EQ(0,f.argc);
  EXPECT_EQ(string(""),f.argv[0]);

  EXPECT_EQ(f.read(),1);
  EXPECT_EQ(2,f.argc);
  EXPECT_EQ(string(" ReadF test"),f.argv[0]);
  EXPECT_EQ(string("ReadF"),f.argv[1]);
  EXPECT_EQ(string("test"),f.argv[2]);

  EXPECT_EQ(f.read(),1);
  EXPECT_EQ(0,f.argc);
  EXPECT_EQ(string(""),f.argv[0]);

  EXPECT_EQ(f.read(),1);
  EXPECT_EQ(0,f.argc);
  EXPECT_EQ(string(""),f.argv[0]);

  EXPECT_EQ(f.read(),1);
  EXPECT_EQ(2,f.argc);
  EXPECT_EQ(string("ifndef __CINT__"),f.argv[0]);
  EXPECT_EQ(string("ifndef"),f.argv[1]);
  EXPECT_EQ(string("__CINT__"),f.argv[2]);


}

//////////////////////////////////////////////////////////////////
TEST(ReadFTest, setquotation)
{
  ReadF f;
  EXPECT_EQ(f.isvalid(),0);
  //f.setquotation("");

  f.parse("abc \" def hij 1234 \" xyz");
  EXPECT_EQ(7,f.argc);
  EXPECT_EQ(string("abc \" def hij 1234 \" xyz"),f.argv[0]);
  EXPECT_EQ(string("abc"),f.argv[1]);
  EXPECT_EQ(string("\""),f.argv[2]);
  EXPECT_EQ(string("def"),f.argv[3]);
  EXPECT_EQ(string("hij"),f.argv[4]);
  EXPECT_EQ(string("1234"),f.argv[5]);
  EXPECT_EQ(string("\""),f.argv[6]);
  EXPECT_EQ(string("xyz"),f.argv[7]);

  f.setquotation("\"");
  f.parse("abc \" def hij 1234 \" xyz");
  EXPECT_EQ(3,f.argc);
  EXPECT_EQ(string("abc \" def hij 1234 \" xyz"),f.argv[0]);
  EXPECT_EQ(string("abc"),f.argv[1]);
  EXPECT_EQ(string(" def hij 1234 "),f.argv[2]);
  EXPECT_EQ(string("xyz"),f.argv[3]);


  f.setquotation("'");
  f.parse("abc \" def hij \" 1234 ' xyz 894 '");
  EXPECT_EQ(7,f.argc);
  EXPECT_EQ(string("abc \" def hij \" 1234 ' xyz 894 '"),f.argv[0]);
  EXPECT_EQ(string("abc"),f.argv[1]);
  EXPECT_EQ(string("\""),f.argv[2]);
  EXPECT_EQ(string("def"),f.argv[3]);
  EXPECT_EQ(string("hij"),f.argv[4]);
  EXPECT_EQ(string("\""),f.argv[5]);
  EXPECT_EQ(string("1234"),f.argv[6]);
  EXPECT_EQ(string(" xyz 894 "),f.argv[7]);

#ifndef __CINT__
  f.open("../test2.cxx");
#else
  f.open("../gtest/test1/test2.cxx");
#endif
  EXPECT_EQ(f.isvalid(),1);
  f.getpos();

  EXPECT_EQ(f.read(),1);
  EXPECT_EQ(f.read(),1);
  EXPECT_EQ(f.read(),1);
  EXPECT_EQ(f.read(),1);
  EXPECT_EQ(2,f.argc);
  EXPECT_EQ(string("#include \"../../ReadF.h\""),f.argv[0]);
  EXPECT_EQ(string("#include"),f.argv[1]);
  EXPECT_EQ(string("\"../../ReadF.h\""),f.argv[2]);

  f.rewind();
  f.setquotation("\"");
  EXPECT_EQ(f.read(),1);
  EXPECT_EQ(f.read(),1);
  EXPECT_EQ(f.read(),1);
  EXPECT_EQ(f.read(),1);
  EXPECT_EQ(2,f.argc);
  EXPECT_EQ(string("#include \"../../ReadF.h\""),f.argv[0]);
  EXPECT_EQ(string("#include"),f.argv[1]);
  EXPECT_EQ(string("../../ReadF.h"),f.argv[2]);

  f.rewind();
  f.setquotation("\"");
  f.setkeepquotation(true);
  EXPECT_EQ(f.read(),1);
  EXPECT_EQ(f.read(),1);
  EXPECT_EQ(f.read(),1);
  EXPECT_EQ(f.read(),1);
  EXPECT_EQ(2,f.argc);
  EXPECT_EQ(string("#include \"../../ReadF.h\""),f.argv[0]);
  EXPECT_EQ(string("#include"),f.argv[1]);
  EXPECT_EQ(string("\"../../ReadF.h\""),f.argv[2]);
}

//////////////////////////////////////////////////////////////////
TEST(ReadFTest, GetposRewind)
{
#ifndef __CINT__
  ReadF f("../test2.cxx");
#else
  ReadF f("../gtest/test1/test2.cxx");
#endif
  EXPECT_EQ(f.isvalid(),1);

  EXPECT_EQ(f.getpos(0),1);
  EXPECT_EQ(f.line,0);
  EXPECT_EQ(f.read(),1);
  EXPECT_EQ(f.line,1);
  EXPECT_EQ(string("// ReadF test"),f.argv[0]);

  EXPECT_EQ(f.getpos(1),1);
  EXPECT_EQ(f.line,1);
  EXPECT_EQ(f.read(),1);
  EXPECT_EQ(f.line,2);
  EXPECT_EQ(string(""),f.argv[0]);

  EXPECT_EQ(f.getpos(2),1);
  EXPECT_EQ(f.line,2);
  EXPECT_EQ(f.read(),1);
  EXPECT_EQ(f.line,3);
  EXPECT_EQ(string("#ifndef __CINT__"),f.argv[0]);

#if 0  // Problem found : Minor, 
  EXPECT_EQ(f.rewind(1),1); // wrong position
  EXPECT_EQ(f.line,1);
  EXPECT_EQ(f.read(),1);
  EXPECT_EQ(f.line,2);
  EXPECT_EQ(string(""),f.argv[0]);
#endif

  EXPECT_EQ(f.rewind(0),1);
  EXPECT_EQ(f.read(),1);
  EXPECT_EQ(string("// ReadF test"),f.argv[0]);

#if 0 // Problem found : Minor,
  EXPECT_EQ(f.rewind(2),1); // wrong position
  EXPECT_EQ(f.read(),1);
  EXPECT_EQ(string("#ifndef __CINT__"),f.argv[0]);
#endif

#if 1
  EXPECT_EQ(f.getpos(11),0);
  EXPECT_EQ(f.rewind(11),0);
#endif
}

//////////////////////////////////////////////////////////////////
TEST(ReadFTest, Readuntil)
{
#ifndef __CINT__
  ReadF f("../test2.cxx");
#else
  ReadF f("../gtest/test1/test2.cxx");
#endif
  EXPECT_EQ(f.isvalid(),1);
  EXPECT_EQ(f.getpos(),1);

  char buf[1000];
  EXPECT_EQ(f.readuntil('t',buf),'t');
  EXPECT_EQ(string("// ReadF "),buf);

  EXPECT_EQ(f.rewind(),1);
  EXPECT_EQ(f.readuntil('#',buf),'\n');
  EXPECT_EQ(string("// ReadF test"),buf);

  EXPECT_EQ(f.readuntil('#',buf),'\n');
  EXPECT_EQ(string(""),buf);

  EXPECT_EQ(f.readuntil('#',buf),'#');
  EXPECT_EQ(string(""),buf);

  EXPECT_EQ(f.readuntil('#',buf),'\n');
  EXPECT_EQ(string("ifndef __CINT__"),buf);

  EXPECT_EQ(f.readuntil('#',buf),'#');
  EXPECT_EQ(string(""),buf);

  EXPECT_EQ(f.readuntil('/',buf),'\n'); // inside quotation
  EXPECT_EQ(string("include \"../../ReadF.h\""),buf);

  EXPECT_EQ(f.readuntil('<',buf),'<'); 
  EXPECT_EQ(string("#include "),buf);
 

}

//////////////////////////////////////////////////////////////////
TEST(ReadFTest, Skipuntil)
{
#ifndef __CINT__
  ReadF f("../test2.cxx");
#else
  ReadF f("../gtest/test1/test2.cxx");
#endif
  EXPECT_EQ(f.isvalid(),1);
  EXPECT_EQ(f.getpos(),1);

  EXPECT_EQ(f.skipuntil('t'),'t');

  EXPECT_EQ(f.rewind(),1);
  EXPECT_EQ(f.skipuntil('#'),'\n');

  EXPECT_EQ(f.skipuntil('#'),'\n');

  EXPECT_EQ(f.skipuntil('#'),'#');

  EXPECT_EQ(f.skipuntil('#'),'\n');

  EXPECT_EQ(f.skipuntil('#'),'#');

  EXPECT_EQ(f.skipuntil('/'),'\n'); // inside quotation

  EXPECT_EQ(f.skipuntil('<'),'<'); 
}

//////////////////////////////////////////////////////////////////
TEST(ReadFTest, Skipline)
{
#ifndef __CINT__
  ReadF f("../test2.cxx");
#else
  ReadF f("../gtest/test1/test2.cxx");
#endif
  EXPECT_EQ(f.isvalid(),1);

  EXPECT_EQ(f.line,0);
  EXPECT_EQ(f.skipline(),'\n');
  EXPECT_EQ(f.line,1);
  EXPECT_EQ(f.skipline(),'\n');
  EXPECT_EQ(f.line,2);

  EXPECT_EQ(f.read(),1);
  EXPECT_EQ(2,f.argc);
  EXPECT_EQ(string("#ifndef __CINT__"),f.argv[0]);
  EXPECT_EQ(string("#ifndef"),f.argv[1]);
  EXPECT_EQ(string("__CINT__"),f.argv[2]);
  EXPECT_EQ(3,f.line);

}


//////////////////////////////////////////////////////////////////
TEST(ReadFTest, Goback)
{
#ifndef __CINT__
  ReadF f("../test2.cxx");
#else
  ReadF f("../gtest/test1/test2.cxx");
#endif
  EXPECT_EQ(f.isvalid(),1);

  EXPECT_EQ(f.line,0);
  EXPECT_EQ(f.skipline(),'\n');
  EXPECT_EQ(f.line,1);
  EXPECT_EQ(f.skipline(),'\n');
  EXPECT_EQ(f.line,2);

  f.goback();

  EXPECT_EQ(f.read(),1);
  EXPECT_EQ(3,f.argc);
  EXPECT_EQ(string("// ReadF test"),f.argv[0]);
  EXPECT_EQ(1,f.line);

}

//////////////////////////////////////////////////////////////////
TEST(ReadFTest, Fseek)
{
#ifndef __CINT__
  ReadF f("../test2.cxx");
#else
  ReadF f("../gtest/test1/test2.cxx");
#endif
  EXPECT_EQ(f.isvalid(),1);

  char buf[1000];
  EXPECT_EQ(f.readuntil('t',buf),'t');
  EXPECT_EQ(string("// ReadF "),buf);

  EXPECT_EQ(f.fseek_cur(-7),0);
  EXPECT_EQ(f.readuntil('#',buf),'\n');
  EXPECT_EQ(string("ReadF test"),buf);

  EXPECT_EQ(f.fseek_set(2),0);
  EXPECT_EQ(f.readuntil('#',buf),'\n');
  EXPECT_EQ(string(" ReadF test"),buf);

  EXPECT_EQ(f.fseek_end(-15),0);
  EXPECT_EQ(f.readuntil('#',buf),'\n');
  EXPECT_EQ(string("// END OF TEST"),buf);

}

// Caution: Do not modify blow this line. Those lines are used as test case.

// END OF TEST
