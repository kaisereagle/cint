
// My 1st Google test 

#ifndef __CINT__
#include "../../EasyExp.h"
#include <gtest/gtest.h>
extern void RESET_LOCALFAIL();
extern int LOCALFAIL_COUNT() ;
extern int G__pause() ;
#include <string>
#include <vector>
using namespace std;
#else
#include <_util_cinttest.h>
#endif



// Thorough pattern match test is performed in module test ../../test


///////////////////////////////////////////////////////////
TEST(EasyExpMethodTest, Constructor)
{
  EasyExp e1;
  EXPECT_EQ(e1.argv.size(),0);
  EXPECT_EQ(e1.element_size(),0);
  EXPECT_EQ(e1.replace_element_size(),0);

  EasyExp e2("pattern*match");
  EXPECT_EQ(e2.argv.size(),0);
  EXPECT_EQ(e2.element_size(),3);
  EXPECT_EQ(e2.replace_element_size(),0);

  EasyExp e3("pattern*match","$0=Pattern");
  EXPECT_EQ(e3.argv.size(),0);
  EXPECT_EQ(e3.element_size(),3);
  EXPECT_EQ(e3.replace_element_size(),1);

  EasyExp e4("pattern*match",EasyExp::Exact);
  EXPECT_EQ(e4.argv.size(),0);
  EXPECT_EQ(e4.element_size(),2);
  EXPECT_EQ(e4.replace_element_size(),0);

  EasyExp e5("pattern*match","$1=xxx",EasyExp::Exact);
  printf("intentional error above\n");
  EXPECT_EQ(e5.argv.size(),0);
  EXPECT_EQ(e5.element_size(),3);
  EXPECT_EQ(e5.replace_element_size(),2);

  EasyExp e6("pattern*match",EasyExp::Auto);
  EXPECT_EQ(e6.argv.size(),0);
  EXPECT_EQ(e6.element_size(),3);
  EXPECT_EQ(e6.replace_element_size(),0);

  EasyExp e7("pattern*match","$1=xxx",EasyExp::Auto);
  EXPECT_EQ(e7.argv.size(),0);
  EXPECT_EQ(e7.element_size(),3);
  EXPECT_EQ(e7.replace_element_size(),2);

  EasyExp e8("pattern*match",EasyExp::RegEx);
  EXPECT_EQ(e8.argv.size(),0);
  EXPECT_EQ(e8.element_size(),3);
  EXPECT_EQ(e8.replace_element_size(),0);

  EasyExp e9("pattern*match","$1=xxx",EasyExp::RegEx);
  EXPECT_EQ(e9.argv.size(),0);
  EXPECT_EQ(e9.element_size(),3);
  EXPECT_EQ(e9.replace_element_size(),2);

}

///////////////////////////////////////////////////////////
TEST(EasyExpMethodTest, compile)
{
  string a;
  EasyExp e2; e2.compile("pattern*match");
  EXPECT_EQ(e2.argv.size(),0);
  EXPECT_EQ(e2.element_size(),3);
  EXPECT_EQ(e2.replace_element_size(),0);
  EXPECT_EQ(e2.replace("patternABCDmatch",a),1);
  EXPECT_EQ(a,"patternABCDmatch");
    

  EasyExp e3; e3.compile_replace("pattern*match","$0=Pattern");
  EXPECT_EQ(e3.argv.size(),0);
  EXPECT_EQ(e3.element_size(),3);
  EXPECT_EQ(e3.replace_element_size(),1);
  EXPECT_EQ(e3.replace("patternABCDmatch",a),1);
  EXPECT_EQ(a,"PatternABCDmatch");

  EasyExp e4; e4.compile("pattern*match",EasyExp::Exact);
  EXPECT_EQ(e4.argv.size(),0);
  EXPECT_EQ(e4.element_size(),2);
  EXPECT_EQ(e4.replace_element_size(),0);
  EXPECT_EQ(e4.replace("pattern*match",a),1);
  EXPECT_EQ(a,"pattern*match");

  EasyExp e5; e5.compile_replace("pattern*match","$1=xxx",EasyExp::Exact);
  printf("intentional error above\n");
  EXPECT_EQ(e5.argv.size(),0);
  EXPECT_EQ(e5.element_size(),3);
  EXPECT_EQ(e5.replace_element_size(),2);
  EXPECT_EQ(e5.replace("pattern*match",a),1);
  EXPECT_EQ(a,"patternxxxmatch");

  EasyExp e6; e6.compile("pattern*match",EasyExp::Auto);
  EXPECT_EQ(e6.argv.size(),0);
  EXPECT_EQ(e6.element_size(),3);
  EXPECT_EQ(e6.replace_element_size(),0);
  EXPECT_EQ(e6.replace("patternABCDmatch",a),1);
  EXPECT_EQ(a,"patternABCDmatch");

  EasyExp e7; e7.compile_replace("pattern*match","$1=xxx",EasyExp::Auto);
  EXPECT_EQ(e7.argv.size(),0);
  EXPECT_EQ(e7.element_size(),3);
  EXPECT_EQ(e7.replace_element_size(),2);
  EXPECT_EQ(e7.replace("patternABCDmatch",a),1);
  EXPECT_EQ(a,"patternxxxmatch");

  EasyExp e8; e8.compile("pattern*match",EasyExp::RegEx);
  EXPECT_EQ(e8.argv.size(),0);
  EXPECT_EQ(e8.element_size(),3);
  EXPECT_EQ(e8.replace_element_size(),0);
  EXPECT_EQ(e8.replace("patternABCDmatch",a),1);
  EXPECT_EQ(a,"patternABCDmatch");

  EasyExp e9; e9.compile_replace("pattern*match","$1=xxx",EasyExp::RegEx);
  EXPECT_EQ(e9.argv.size(),0);
  EXPECT_EQ(e9.element_size(),3);
  EXPECT_EQ(e9.replace_element_size(),2);
  EXPECT_EQ(e9.replace("patternABCDmatch",a),1);
  EXPECT_EQ(a,"patternxxxmatch");

  EasyExp e10; e10.compile_replace("pattern*match","");
  EXPECT_EQ(e10.argv.size(),0);
  EXPECT_EQ(e10.element_size(),3);
  EXPECT_EQ(e10.replace_element_size(),0); //<< should be 0 ???
  EXPECT_EQ(e10.replace("patternABCDmatch",a),1);
  EXPECT_EQ(a,"patternABCDmatch");
}

///////////////////////////////////////////////////////////
TEST(EasyExpMethodTest, matchSingle)
{
  EasyExp e1; e1.compile("pattern*match");
  EXPECT_EQ(e1.pattern(),"pattern*match");
  EXPECT_TRUE(e1.match("patternmatch"));
  EXPECT_EQ(e1.argv.size(),3);
  EXPECT_EQ(e1.argv[0],"pattern");
  EXPECT_EQ(e1.argv[1],"");
  EXPECT_EQ(e1.argv[2],"match");

  EXPECT_TRUE(e1.match("patternAAAmatch"));
  EXPECT_EQ(e1.argv.size(),3);
  EXPECT_EQ(e1.argv[0],"pattern");
  EXPECT_EQ(e1.argv[1],"AAA");
  EXPECT_EQ(e1.argv[2],"match");

  EXPECT_FALSE(e1.match("PatternAAAMatch"));
  EXPECT_EQ(e1.argv.size(),0);

  EXPECT_FALSE(e1.match("PatternAAAMatch",false));
  EXPECT_EQ(e1.argv.size(),0);

  EXPECT_TRUE(e1.match("PatternAAAMatch",true));
  EXPECT_EQ(e1.argv.size(),3);
  EXPECT_EQ(e1.argv[0],"Pattern");
  EXPECT_EQ(e1.argv[1],"AAA");
  EXPECT_EQ(e1.argv[2],"Match");
}

///////////////////////////////////////////////////////////
TEST(EasyExpMethodTest, matchMultiple)
{
  EasyExp e1; e1.compile("pattern*match");

  vector<string> in,out;       //  icase false  true
  in.push_back("patternAAAmatch");  //     1     1
  in.push_back("patternbbbmatch");  //     1     1
  in.push_back("patterncccmatch");  //     1     1
  in.push_back("PatternAAAMatch");  //     0     1
  in.push_back("PatternbbbMatch");  //     0     1
  in.push_back("PatterncccMatch");  //     0     1
  in.push_back("xpatterncccmatch"); //     0     0
  in.push_back("patterncccmatchy"); //     0     0

  out = e1.match(in);
  EXPECT_EQ(out.size(),3);

  out = e1.match(in,false);
  EXPECT_EQ(out.size(),3);

  out = e1.match(in,true);
  EXPECT_EQ(out.size(),6);

  out = e1._match(in);
  EXPECT_EQ(out.size(),5);

  out = e1._match(in,false);
  EXPECT_EQ(out.size(),5);

  out = e1._match(in,true);
  EXPECT_EQ(out.size(),2);
}

///////////////////////////////////////////////////////////
TEST(EasyExpMethodTest, complexity)
{
  EasyExp e1; 
  // string , star string
  e1.compile("pattern");
  EXPECT_EQ(e1.pattern(),"pattern");
  EXPECT_EQ(e1.complexity(),1);

  e1.compile("pattern*");
  EXPECT_EQ(e1.pattern(),"pattern*");
  EXPECT_EQ(e1.complexity(),2);

  e1.compile("*pattern");
  EXPECT_EQ(e1.pattern(),"*pattern");
  EXPECT_EQ(e1.complexity(),2);

  e1.compile("pattern*match");
  EXPECT_EQ(e1.pattern(),"pattern*match");
  EXPECT_EQ(e1.complexity(),3);

  e1.compile("pattern*match*");
  EXPECT_EQ(e1.pattern(),"pattern*match*");
  EXPECT_EQ(e1.complexity(),4);

  e1.compile("*pattern*match*");
  EXPECT_EQ(e1.pattern(),"*pattern*match*");
  EXPECT_EQ(e1.complexity(),5);

  // string list, star string list
  e1.compile("(apple|orange)");
  EXPECT_EQ(e1.pattern(),"(apple|orange)");
  EXPECT_EQ(e1.complexity(),1);

  e1.compile("(apple|orange)*");
  EXPECT_EQ(e1.pattern(),"(apple|orange)*");
  EXPECT_EQ(e1.complexity(),2);

  e1.compile("*(apple|orange)");
  EXPECT_EQ(e1.pattern(),"*(apple|orange)");
  EXPECT_EQ(e1.complexity(),2);

  e1.compile("*(apple|orange)*");
  EXPECT_EQ(e1.pattern(),"*(apple|orange)*");
  EXPECT_EQ(e1.complexity(),3);

  // char list, star char list
  e1.compile("[a-z]");
  EXPECT_EQ(e1.pattern(),"[a-z]");
  EXPECT_EQ(e1.complexity(),1);

  e1.compile("[a-z]*");
  EXPECT_EQ(e1.pattern(),"[a-z]*");
  EXPECT_EQ(e1.complexity(),2);

  e1.compile("*[a-z]");
  EXPECT_EQ(e1.pattern(),"*[a-z]");
  EXPECT_EQ(e1.complexity(),2);

  e1.compile("*[a-z]*");
  EXPECT_EQ(e1.pattern(),"*[a-z]*");
  EXPECT_EQ(e1.complexity(),3);

  // not char list, star not char list
  e1.compile("[^a-z]");
  EXPECT_EQ(e1.pattern(),"[^a-z]");
  EXPECT_EQ(e1.complexity(),1);

  e1.compile("[^a-z]*");
  EXPECT_EQ(e1.pattern(),"[^a-z]*");
  EXPECT_EQ(e1.complexity(),2);

  e1.compile("*[^a-z]");
  EXPECT_EQ(e1.pattern(),"*[^a-z]");
  EXPECT_EQ(e1.complexity(),2);

  e1.compile("*[^a-z]*");
  EXPECT_EQ(e1.pattern(),"*[^a-z]*");
  EXPECT_EQ(e1.complexity(),3);

  // number range , star number range
  e1.compile("(10..20)");
  EXPECT_EQ(e1.pattern(),"(10..20)");
  EXPECT_EQ(e1.complexity(),1);

  e1.compile("(10..20)*");
  EXPECT_EQ(e1.pattern(),"(10..20)*");
  EXPECT_EQ(e1.complexity(),2);

  e1.compile("*(10..20)");
  EXPECT_EQ(e1.pattern(),"*(10..20)");
  EXPECT_EQ(e1.complexity(),2);

  e1.compile("*(10..20)*");
  EXPECT_EQ(e1.pattern(),"*(10..20)*");
  EXPECT_EQ(e1.complexity(),3);

  // end , star end
  e1.compile("");
  EXPECT_EQ(e1.pattern(),"");
  EXPECT_EQ(e1.complexity(),0);
  EXPECT_EQ(e1.element_size(),1);
  
  e1.compile("*");
  EXPECT_EQ(e1.pattern(),"*");
  EXPECT_EQ(e1.complexity(),1);
}

///////////////////////////////////////////////////////////
#if 0
TEST(EasyExpMethodTest, disphelp)
{
  // omit tests for disp and help, because those are used 
  // only for debugging
  EasyExp e;
  //e.disp();
  //e.help();
}
#endif

///////////////////////////////////////////////////////////
TEST(EasyExpMethodTest, verificationsamples)
{
  EasyExp e;
  vector<string> out;

  e.compile("abc");
  out = e.verification_samples();
  EXPECT_EQ(out.size(),1);
  EXPECT_EQ(out[0],"abc");

  e.compile("(abc|def)");
  out = e.verification_samples();
  EXPECT_EQ(out.size(),2);
  EXPECT_EQ(out[0],"abc");
  EXPECT_EQ(out[1],"def");

  e.compile("(abc|def)_(xyz|lmn)");
  out = e.verification_samples();
  EXPECT_EQ(out.size(),2);
  EXPECT_EQ(out[0],"abc_xyz"); // this may not be true
  EXPECT_EQ(out[1],"def_lmn"); // this may not be true
}

///////////////////////////////////////////////////////////
#if 0
TEST(EasyExpMethodTest, disp)
{
}
#endif
