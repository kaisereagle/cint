
// My 1st Google test 

#ifndef __CINT__
#include "../../EasyExp.h"
#include <gtest/gtest.h>
void RESET_LOCALFAIL() { }
int LOCALFAIL_COUNT() { return(0); }
int G__pause() { return(0); }
#else
#include <_util_cinttest.h>
#endif

//////////////////////////////////////////////////////////////////
TEST(EasyExpCompileTest, ZeroLengthStringList1)
{
  RESET_LOCALFAIL();

  EasyExp e1("pattern*(.tsv|.csv)");        // must have (.tsv|.csv)
  EXPECT_FALSE(e1.backannotation()!="pattern*(.tsv|.csv)") ;
  if(LOCALFAIL_COUNT()) G__pause();
  EXPECT_FALSE(e1.pattern()!="pattern*(.tsv|.csv)") ;
  if(LOCALFAIL_COUNT()) G__pause();

  EXPECT_FALSE(e1.element_size()!=3) ;
  EXPECT_FALSE(strcmp(e1.element(0)->what(),"MatchString")) ;
  EXPECT_FALSE(strcmp(e1.element(0)->contents(0),"pattern")) ;

  EXPECT_FALSE(strcmp(e1.element(1)->what(),"MatchStarStringList")) ;
  EXPECT_FALSE(e1.element(1)->nlist()!=2) ;
  EXPECT_FALSE(strcmp(e1.element(1)->contents(0),".tsv")) ;
  EXPECT_FALSE(strcmp(e1.element(1)->contents(1),".csv")) ;

  EXPECT_FALSE(strcmp(e1.element(2)->what(),"MatchEnd")) ;

  if(LOCALFAIL_COUNT()) G__pause();

  EasyExp e2("pattern*(|.tsv|.csv)");       // same as pattern*
  EXPECT_FALSE(e2.backannotation()!="pattern*(.tsv|.csv|)") ;
  EXPECT_FALSE(e2.pattern()!="pattern*(|.tsv|.csv)") ;

  EXPECT_FALSE(e2.element_size()!=3) ;
  EXPECT_FALSE(strcmp(e2.element(0)->what(),"MatchString")) ;
  EXPECT_FALSE(strcmp(e2.element(0)->contents(0),"pattern")) ;

  EXPECT_FALSE(strcmp(e2.element(1)->what(),"MatchStarStringList")) ;
  EXPECT_FALSE(e2.element(1)->nlist()!=3) ;
  EXPECT_FALSE(strcmp(e2.element(1)->contents(0),".tsv")) ;
  EXPECT_FALSE(strcmp(e2.element(1)->contents(1),".csv")) ;
  EXPECT_FALSE(strcmp(e2.element(1)->contents(2),"")) ;

  EXPECT_FALSE(strcmp(e2.element(2)->what(),"MatchEnd")) ;

  if(LOCALFAIL_COUNT()) G__pause();


  EasyExp e3("pattern*(.tsv|.csv)(|x)");    // must have (.tsv|.csv|.tsvx|.csvx)
  EXPECT_FALSE(e3.backannotation()!="pattern*(.tsv|.csv)(x|)") ;
  EXPECT_FALSE(e3.pattern()!="pattern*(.tsv|.csv)(|x)") ;

  EXPECT_FALSE(e3.element_size()!=4) ;
  EXPECT_FALSE(strcmp(e3.element(0)->what(),"MatchString")) ;
  EXPECT_FALSE(strcmp(e3.element(0)->contents(0),"pattern")) ;

  EXPECT_FALSE(strcmp(e3.element(1)->what(),"MatchStarStringList")) ;
  EXPECT_FALSE(e3.element(1)->nlist()!=2) ;
  EXPECT_FALSE(strcmp(e3.element(1)->contents(0),".tsv")) ;
  EXPECT_FALSE(strcmp(e3.element(1)->contents(1),".csv")) ;

  EXPECT_FALSE(strcmp(e3.element(2)->what(),"MatchStringList")) ;
  EXPECT_FALSE(e3.element(2)->nlist()!=2) ;
  EXPECT_FALSE(strcmp(e3.element(2)->contents(0),"x")) ;
  EXPECT_FALSE(strcmp(e3.element(2)->contents(1),"")) ;

  EXPECT_FALSE(strcmp(e3.element(3)->what(),"MatchEnd")) ;

  if(LOCALFAIL_COUNT()) G__pause();


  EasyExp e4("pattern*(|.tsv|.csv)(|x)");   // same as pattern*
  EXPECT_FALSE(e4.backannotation()!="pattern*(.tsv|.csv|)(x|)") ;
  EXPECT_FALSE(e4.pattern()!="pattern*(|.tsv|.csv)(|x)") ;

  EXPECT_FALSE(e4.element_size()!=4) ;
  EXPECT_FALSE(strcmp(e4.element(0)->what(),"MatchString")) ;
  EXPECT_FALSE(strcmp(e4.element(0)->contents(0),"pattern")) ;

  EXPECT_FALSE(strcmp(e4.element(1)->what(),"MatchStarStringList")) ;
  EXPECT_FALSE(e4.element(1)->nlist()!=3) ;
  EXPECT_FALSE(strcmp(e4.element(1)->contents(0),".tsv")) ;
  EXPECT_FALSE(strcmp(e4.element(1)->contents(1),".csv")) ;
  EXPECT_FALSE(strcmp(e4.element(1)->contents(2),"")) ;

  EXPECT_FALSE(strcmp(e4.element(2)->what(),"MatchStringList")) ;
  EXPECT_FALSE(e4.element(2)->nlist()!=2) ;
  EXPECT_FALSE(strcmp(e4.element(2)->contents(0),"x")) ;
  EXPECT_FALSE(strcmp(e4.element(2)->contents(1),"")) ;

  EXPECT_FALSE(strcmp(e4.element(3)->what(),"MatchEnd")) ;

  if(LOCALFAIL_COUNT()) G__pause();

  EasyExp e5("pattern*(|.tsv|.csv)[x:0-1]");// same as pattern*
  EXPECT_FALSE(e5.backannotation()!="pattern*(.tsv|.csv|)[x:0-1]") ;
  EXPECT_FALSE(e5.pattern()!="pattern*(|.tsv|.csv)[x:0-1]") ;

  EXPECT_FALSE(e5.element_size()!=4) ;
  EXPECT_FALSE(strcmp(e5.element(0)->what(),"MatchString")) ;
  EXPECT_FALSE(strcmp(e5.element(0)->contents(0),"pattern")) ;

  EXPECT_FALSE(strcmp(e5.element(1)->what(),"MatchStarStringList")) ;
  EXPECT_FALSE(e5.element(1)->nlist()!=3) ;
  EXPECT_FALSE(strcmp(e5.element(1)->contents(0),".tsv")) ;
  EXPECT_FALSE(strcmp(e5.element(1)->contents(1),".csv")) ;
  EXPECT_FALSE(strcmp(e5.element(1)->contents(2),"")) ;

  EXPECT_FALSE(strcmp(e5.element(2)->what(),"MatchCharList")) ;
  EXPECT_FALSE(e5.element(2)->nlist()!=1) ;
  EXPECT_FALSE(strcmp(e5.element(2)->contents(0),"x")) ;
  EXPECT_FALSE(e5.element(2)->parameter(0)!=0) ;
  EXPECT_FALSE(e5.element(2)->parameter(1)!=1) ;

  EXPECT_FALSE(strcmp(e5.element(3)->what(),"MatchEnd")) ;

  if(LOCALFAIL_COUNT()) G__pause();

  EasyExp e6("pattern*(.tsv|.csv)*(|x)");   // must have  (.tsv|.csv)
  EXPECT_FALSE(e6.backannotation()!="pattern*(.tsv|.csv)*(x|)") ;
  EXPECT_FALSE(e6.pattern()!="pattern*(.tsv|.csv)*(|x)") ;

  EXPECT_FALSE(e6.element_size()!=4) ;
  EXPECT_FALSE(strcmp(e6.element(0)->what(),"MatchString")) ;
  EXPECT_FALSE(strcmp(e6.element(0)->contents(0),"pattern")) ;

  EXPECT_FALSE(strcmp(e6.element(1)->what(),"MatchStarStringList")) ;
  EXPECT_FALSE(e6.element(1)->nlist()!=2) ;
  EXPECT_FALSE(strcmp(e6.element(1)->contents(0),".tsv")) ;
  EXPECT_FALSE(strcmp(e6.element(1)->contents(1),".csv")) ;

  EXPECT_FALSE(strcmp(e6.element(2)->what(),"MatchStarStringList")) ;
  EXPECT_FALSE(e6.element(2)->nlist()!=2) ;
  EXPECT_FALSE(strcmp(e6.element(2)->contents(0),"x")) ;
  EXPECT_FALSE(strcmp(e6.element(2)->contents(1),"")) ;

  EXPECT_FALSE(strcmp(e6.element(3)->what(),"MatchEnd")) ;

  if(LOCALFAIL_COUNT()) G__pause();

  EasyExp e7("pattern*(|.tsv|.csv)*(|x)");  // same as pattern*
  EXPECT_FALSE(e7.backannotation()!="pattern*(.tsv|.csv|)*(x|)") ;
  EXPECT_FALSE(e7.pattern()!="pattern*(|.tsv|.csv)*(|x)") ;

  EXPECT_FALSE(e7.element_size()!=4) ;
  EXPECT_FALSE(strcmp(e7.element(0)->what(),"MatchString")) ;
  EXPECT_FALSE(strcmp(e7.element(0)->contents(0),"pattern")) ;

  EXPECT_FALSE(strcmp(e7.element(1)->what(),"MatchStarStringList")) ;
  EXPECT_FALSE(e7.element(1)->nlist()!=3) ;
  EXPECT_FALSE(strcmp(e7.element(1)->contents(0),".tsv")) ;
  EXPECT_FALSE(strcmp(e7.element(1)->contents(1),".csv")) ;
  EXPECT_FALSE(strcmp(e7.element(1)->contents(2),"")) ;

  EXPECT_FALSE(strcmp(e7.element(2)->what(),"MatchStarStringList")) ;
  EXPECT_FALSE(e7.element(2)->nlist()!=2) ;
  EXPECT_FALSE(strcmp(e7.element(2)->contents(0),"x")) ;
  EXPECT_FALSE(strcmp(e7.element(2)->contents(1),"")) ;

  EXPECT_FALSE(strcmp(e7.element(3)->what(),"MatchEnd")) ;

  if(LOCALFAIL_COUNT()) G__pause();
}


//////////////////////////////////////////////////////////////////
TEST(EasyExpCompileTest, LongPattern)
{
  RESET_LOCALFAIL();

  EasyExp e1("pn[A-Z:2-3]_(1985..2015:4)(01..12:2)(01..31:2)_(0000..9999:4).csv");
  EXPECT_FALSE(e1.backannotation()!="pn[ABCDEFGHIJKLMNOPQRSTUVWXYZ:2-3]_(1985..2015:4)(1..12:2)(1..31:2)_(0..9999:4).csv") ;
  if(LOCALFAIL_COUNT()) G__pause();
  EXPECT_FALSE(e1.pattern()!="pn[A-Z:2-3]_(1985..2015:4)(01..12:2)(01..31:2)_(0000..9999:4).csv") ;
  if(LOCALFAIL_COUNT()) G__pause();

  EXPECT_FALSE(e1.element_size()!=10) ;
  EXPECT_FALSE(strcmp(e1.element(0)->what(),"MatchString")) ;
  EXPECT_FALSE(strcmp(e1.element(0)->contents(0),"pn")) ;
  if(LOCALFAIL_COUNT()) G__pause();

  EXPECT_FALSE(strcmp(e1.element(1)->what(),"MatchCharList")) ;
  EXPECT_FALSE(e1.element(1)->nlist()!=26) ;
  EXPECT_FALSE(strcmp(e1.element(1)->contents(0),"A")) ;
  EXPECT_FALSE(strcmp(e1.element(1)->contents(25),"Z")) ;
  EXPECT_FALSE(e1.element(1)->parameter(0)!=2) ;
  EXPECT_FALSE(e1.element(1)->parameter(1)!=3) ;
  if(LOCALFAIL_COUNT()) G__pause();

  EXPECT_FALSE(strcmp(e1.element(2)->what(),"MatchString")) ;
  EXPECT_FALSE(strcmp(e1.element(2)->contents(0),"_")) ;
  if(LOCALFAIL_COUNT()) G__pause();

  EXPECT_FALSE(strcmp(e1.element(3)->what(),"MatchNumberRange")) ;
  EXPECT_FALSE(strcmp(e1.element(3)->contents(0),"1985")) ;
  EXPECT_FALSE(strcmp(e1.element(3)->contents(1),"2015")) ;
  EXPECT_FALSE(strcmp(e1.element(3)->contents(2),"4")) ;
  if(LOCALFAIL_COUNT()) G__pause();

  EXPECT_FALSE(strcmp(e1.element(4)->what(),"MatchNumberRange")) ;
  EXPECT_FALSE(strcmp(e1.element(4)->contents(0),"1")) ;
  EXPECT_FALSE(strcmp(e1.element(4)->contents(1),"12")) ;
  EXPECT_FALSE(strcmp(e1.element(4)->contents(2),"2")) ;
  if(LOCALFAIL_COUNT()) G__pause();

  EXPECT_FALSE(strcmp(e1.element(5)->what(),"MatchNumberRange")) ;
  EXPECT_FALSE(strcmp(e1.element(5)->contents(0),"1")) ;
  EXPECT_FALSE(strcmp(e1.element(5)->contents(1),"31")) ;
  EXPECT_FALSE(strcmp(e1.element(5)->contents(2),"2")) ;
  if(LOCALFAIL_COUNT()) G__pause();

  EXPECT_FALSE(strcmp(e1.element(6)->what(),"MatchString")) ;
  EXPECT_FALSE(strcmp(e1.element(6)->contents(0),"_")) ;

  if(LOCALFAIL_COUNT()) G__pause();
  EXPECT_FALSE(strcmp(e1.element(7)->what(),"MatchNumberRange")) ;
  EXPECT_FALSE(strcmp(e1.element(7)->contents(0),"0")) ;
  EXPECT_FALSE(strcmp(e1.element(7)->contents(1),"9999")) ;
  EXPECT_FALSE(strcmp(e1.element(7)->contents(2),"4")) ;
  if(LOCALFAIL_COUNT()) G__pause();

  EXPECT_FALSE(strcmp(e1.element(8)->what(),"MatchString")) ;
  EXPECT_FALSE(strcmp(e1.element(8)->contents(0),".csv")) ;

  if(LOCALFAIL_COUNT()) G__pause();

  EXPECT_FALSE(strcmp(e1.element(9)->what(),"MatchEnd")) ;

  if(LOCALFAIL_COUNT()) G__pause();
  
  EasyExp e2("fr*_(apple|orange|banana)_(2001..2020)_*.(txt|doc|docx)");
  EXPECT_FALSE(e2.backannotation()!="fr*_(apple|orange|banana)_(2001..2020)_*.(txt|doc|docx)") ;
  if(LOCALFAIL_COUNT()) G__pause();
  EXPECT_FALSE(e2.pattern()!="fr*_(apple|orange|banana)_(2001..2020)_*.(txt|doc|docx)") ;
  if(LOCALFAIL_COUNT()) G__pause();

  EXPECT_FALSE(e2.element_size()!=9) ;
  EXPECT_FALSE(strcmp(e2.element(0)->what(),"MatchString")) ;
  EXPECT_FALSE(strcmp(e2.element(0)->contents(0),"fr")) ;
  if(LOCALFAIL_COUNT()) G__pause();

  EXPECT_FALSE(strcmp(e2.element(1)->what(),"MatchStarString")) ;
  EXPECT_FALSE(strcmp(e2.element(1)->contents(0),"_")) ;
  if(LOCALFAIL_COUNT()) G__pause();

  EXPECT_FALSE(strcmp(e2.element(2)->what(),"MatchStringList")) ;
  EXPECT_FALSE(e2.element(2)->nlist()!=3) ;
  EXPECT_FALSE(strcmp(e2.element(2)->contents(0),"apple")) ;
  EXPECT_FALSE(strcmp(e2.element(2)->contents(1),"orange")) ;
  EXPECT_FALSE(strcmp(e2.element(2)->contents(2),"banana")) ;
  if(LOCALFAIL_COUNT()) G__pause();

  EXPECT_FALSE(strcmp(e2.element(3)->what(),"MatchString")) ;
  EXPECT_FALSE(strcmp(e2.element(3)->contents(0),"_")) ;
  if(LOCALFAIL_COUNT()) G__pause();

  EXPECT_FALSE(strcmp(e2.element(4)->what(),"MatchNumberRange")) ;
  EXPECT_FALSE(strcmp(e2.element(4)->contents(0),"2001")) ;
  EXPECT_FALSE(strcmp(e2.element(4)->contents(1),"2020")) ;
  EXPECT_FALSE(strcmp(e2.element(4)->contents(2),"0")) ;
  if(LOCALFAIL_COUNT()) G__pause();

  EXPECT_FALSE(strcmp(e2.element(5)->what(),"MatchString")) ;
  EXPECT_FALSE(strcmp(e2.element(5)->contents(0),"_")) ;
  if(LOCALFAIL_COUNT()) G__pause();

  EXPECT_FALSE(strcmp(e2.element(6)->what(),"MatchStarString")) ;
  EXPECT_FALSE(strcmp(e2.element(6)->contents(0),".")) ;
  if(LOCALFAIL_COUNT()) G__pause();

  EXPECT_FALSE(strcmp(e2.element(7)->what(),"MatchStringList")) ;
  EXPECT_FALSE(e2.element(7)->nlist()!=3) ;
  EXPECT_FALSE(strcmp(e2.element(7)->contents(0),"txt")) ;
  EXPECT_FALSE(strcmp(e2.element(7)->contents(1),"doc")) ;
  EXPECT_FALSE(strcmp(e2.element(7)->contents(2),"docx")) ;
  if(LOCALFAIL_COUNT()) G__pause();

  EXPECT_FALSE(strcmp(e2.element(8)->what(),"MatchEnd")) ;

  if(LOCALFAIL_COUNT()) G__pause();

  EasyExp e3("pn[A-Z:2-3]_[0-9:8]_[0-9:4].csv");
  EXPECT_FALSE(e3.backannotation()!="pn[ABCDEFGHIJKLMNOPQRSTUVWXYZ:2-3]_[0123456789:8]_[0123456789:4].csv") ;
  if(LOCALFAIL_COUNT()) G__pause();
  EXPECT_FALSE(e3.pattern()!="pn[A-Z:2-3]_[0-9:8]_[0-9:4].csv") ;
  if(LOCALFAIL_COUNT()) G__pause();

  EXPECT_FALSE(e3.element_size()!=8) ;
  EXPECT_FALSE(strcmp(e3.element(0)->what(),"MatchString")) ;
  EXPECT_FALSE(strcmp(e3.element(0)->contents(0),"pn")) ;
  if(LOCALFAIL_COUNT()) G__pause();

  EXPECT_FALSE(strcmp(e3.element(1)->what(),"MatchCharList")) ;
  EXPECT_FALSE(e3.element(1)->nlist()!=26) ;
  EXPECT_FALSE(strcmp(e3.element(1)->contents(0),"A")) ;
  EXPECT_FALSE(strcmp(e3.element(1)->contents(25),"Z")) ;
  EXPECT_FALSE(e3.element(1)->parameter(0)!=2) ;
  EXPECT_FALSE(e3.element(1)->parameter(1)!=3) ;
  if(LOCALFAIL_COUNT()) G__pause();

  EXPECT_FALSE(strcmp(e3.element(2)->what(),"MatchString")) ;
  EXPECT_FALSE(strcmp(e3.element(2)->contents(0),"_")) ;
  if(LOCALFAIL_COUNT()) G__pause();

  EXPECT_FALSE(strcmp(e3.element(3)->what(),"MatchCharList")) ;
  EXPECT_FALSE(e3.element(3)->nlist()!=10) ;
  EXPECT_FALSE(strcmp(e3.element(3)->contents(0),"0")) ;
  EXPECT_FALSE(strcmp(e3.element(3)->contents(9),"9")) ;
  EXPECT_FALSE(e3.element(3)->parameter(0)!=8) ;
  EXPECT_FALSE(e3.element(3)->parameter(1)!=-1) ;
  if(LOCALFAIL_COUNT()) G__pause();

  EXPECT_FALSE(strcmp(e3.element(4)->what(),"MatchString")) ;
  EXPECT_FALSE(strcmp(e3.element(4)->contents(0),"_")) ;
  if(LOCALFAIL_COUNT()) G__pause();

  EXPECT_FALSE(strcmp(e3.element(5)->what(),"MatchCharList")) ;
  EXPECT_FALSE(e3.element(5)->nlist()!=10) ;
  EXPECT_FALSE(strcmp(e3.element(5)->contents(0),"0")) ;
  EXPECT_FALSE(strcmp(e3.element(5)->contents(9),"9")) ;
  EXPECT_FALSE(e3.element(5)->parameter(0)!=4) ;
  EXPECT_FALSE(e3.element(5)->parameter(1)!=-1) ;
  if(LOCALFAIL_COUNT()) G__pause();

  EXPECT_FALSE(strcmp(e3.element(6)->what(),"MatchString")) ;
  EXPECT_FALSE(strcmp(e3.element(6)->contents(0),".csv")) ;
  if(LOCALFAIL_COUNT()) G__pause();

  EXPECT_FALSE(strcmp(e3.element(7)->what(),"MatchEnd")) ;

  if(LOCALFAIL_COUNT()) G__pause();

  EasyExp e4("pn[A-Z:2-3]_[0-9:6-10]_[0-9:*].csv");
  EXPECT_FALSE(e4.backannotation()!="pn[ABCDEFGHIJKLMNOPQRSTUVWXYZ:2-3]_[0123456789:6-10]_[0123456789:*].csv") ;
  if(LOCALFAIL_COUNT()) G__pause();
  EXPECT_FALSE(e4.pattern()!="pn[A-Z:2-3]_[0-9:6-10]_[0-9:*].csv") ;
  if(LOCALFAIL_COUNT()) G__pause();

  EXPECT_FALSE(e4.element_size()!=8) ;
  EXPECT_FALSE(strcmp(e4.element(0)->what(),"MatchString")) ;
  EXPECT_FALSE(strcmp(e4.element(0)->contents(0),"pn")) ;
  if(LOCALFAIL_COUNT()) G__pause();

  EXPECT_FALSE(strcmp(e4.element(1)->what(),"MatchCharList")) ;
  EXPECT_FALSE(e4.element(1)->nlist()!=26) ;
  EXPECT_FALSE(strcmp(e4.element(1)->contents(0),"A")) ;
  EXPECT_FALSE(strcmp(e4.element(1)->contents(25),"Z")) ;
  EXPECT_FALSE(e4.element(1)->parameter(0)!=2) ;
  EXPECT_FALSE(e4.element(1)->parameter(1)!=3) ;
  if(LOCALFAIL_COUNT()) G__pause();

  EXPECT_FALSE(strcmp(e4.element(2)->what(),"MatchString")) ;
  EXPECT_FALSE(strcmp(e4.element(2)->contents(0),"_")) ;
  if(LOCALFAIL_COUNT()) G__pause();

  EXPECT_FALSE(strcmp(e4.element(3)->what(),"MatchCharList")) ;
  EXPECT_FALSE(e4.element(3)->nlist()!=10) ;
  EXPECT_FALSE(strcmp(e4.element(3)->contents(0),"0")) ;
  EXPECT_FALSE(strcmp(e4.element(3)->contents(9),"9")) ;
  EXPECT_FALSE(e4.element(3)->parameter(0)!=6) ;
  EXPECT_FALSE(e4.element(3)->parameter(1)!=10) ;
  if(LOCALFAIL_COUNT()) G__pause();

  EXPECT_FALSE(strcmp(e4.element(4)->what(),"MatchString")) ;
  EXPECT_FALSE(strcmp(e4.element(4)->contents(0),"_")) ;
  if(LOCALFAIL_COUNT()) G__pause();

  EXPECT_FALSE(strcmp(e4.element(5)->what(),"MatchCharList")) ;
  EXPECT_FALSE(e4.element(5)->nlist()!=10) ;
  EXPECT_FALSE(strcmp(e4.element(5)->contents(0),"0")) ;
  EXPECT_FALSE(strcmp(e4.element(5)->contents(9),"9")) ;
  EXPECT_FALSE(e4.element(5)->parameter(0)!=-1) ;
  EXPECT_FALSE(e4.element(5)->parameter(1)!=-1) ;
  if(LOCALFAIL_COUNT()) G__pause();

  EXPECT_FALSE(strcmp(e4.element(6)->what(),"MatchString")) ;
  EXPECT_FALSE(strcmp(e4.element(6)->contents(0),".csv")) ;
  if(LOCALFAIL_COUNT()) G__pause();

  EXPECT_FALSE(strcmp(e4.element(7)->what(),"MatchEnd")) ;

  if(LOCALFAIL_COUNT()) G__pause();

  EasyExp e5("pn[A-Z:2-3]_[0-9:*]_[0-9:*].csv");
  EXPECT_FALSE(e5.backannotation()!="pn[ABCDEFGHIJKLMNOPQRSTUVWXYZ:2-3]_[0123456789:*]_[0123456789:*].csv") ;
  if(LOCALFAIL_COUNT()) G__pause();
  EXPECT_FALSE(e5.pattern()!="pn[A-Z:2-3]_[0-9:*]_[0-9:*].csv") ;
  if(LOCALFAIL_COUNT()) G__pause();

  EXPECT_FALSE(e5.element_size()!=8) ;
  EXPECT_FALSE(strcmp(e5.element(0)->what(),"MatchString")) ;
  EXPECT_FALSE(strcmp(e5.element(0)->contents(0),"pn")) ;
  if(LOCALFAIL_COUNT()) G__pause();

  EXPECT_FALSE(strcmp(e5.element(1)->what(),"MatchCharList")) ;
  EXPECT_FALSE(e5.element(1)->nlist()!=26) ;
  EXPECT_FALSE(strcmp(e5.element(1)->contents(0),"A")) ;
  EXPECT_FALSE(strcmp(e5.element(1)->contents(25),"Z")) ;
  EXPECT_FALSE(e5.element(1)->parameter(0)!=2) ;
  EXPECT_FALSE(e5.element(1)->parameter(1)!=3) ;
  if(LOCALFAIL_COUNT()) G__pause();

  EXPECT_FALSE(strcmp(e5.element(2)->what(),"MatchString")) ;
  EXPECT_FALSE(strcmp(e5.element(2)->contents(0),"_")) ;
  if(LOCALFAIL_COUNT()) G__pause();

  EXPECT_FALSE(strcmp(e5.element(3)->what(),"MatchCharList")) ;
  EXPECT_FALSE(e5.element(3)->nlist()!=10) ;
  EXPECT_FALSE(strcmp(e5.element(3)->contents(0),"0")) ;
  EXPECT_FALSE(strcmp(e5.element(3)->contents(9),"9")) ;
  EXPECT_FALSE(e5.element(3)->parameter(0)!=-1) ;
  EXPECT_FALSE(e5.element(3)->parameter(1)!=-1) ;
  if(LOCALFAIL_COUNT()) G__pause();

  EXPECT_FALSE(strcmp(e5.element(4)->what(),"MatchString")) ;
  EXPECT_FALSE(strcmp(e5.element(4)->contents(0),"_")) ;
  if(LOCALFAIL_COUNT()) G__pause();

  EXPECT_FALSE(strcmp(e5.element(5)->what(),"MatchCharList")) ;
  EXPECT_FALSE(e5.element(5)->nlist()!=10) ;
  EXPECT_FALSE(strcmp(e5.element(5)->contents(0),"0")) ;
  EXPECT_FALSE(strcmp(e5.element(5)->contents(9),"9")) ;
  EXPECT_FALSE(e5.element(5)->parameter(0)!=-1) ;
  EXPECT_FALSE(e5.element(5)->parameter(1)!=-1) ;
  if(LOCALFAIL_COUNT()) G__pause();

  EXPECT_FALSE(strcmp(e5.element(6)->what(),"MatchString")) ;
  EXPECT_FALSE(strcmp(e5.element(6)->contents(0),".csv")) ;
  if(LOCALFAIL_COUNT()) G__pause();

  EXPECT_FALSE(strcmp(e5.element(7)->what(),"MatchEnd")) ;

  if(LOCALFAIL_COUNT()) G__pause();
}


#if 0
//////////////////////////////////////////////////////////////////
TEST(EasyExpCompileTest, StringStarString)
{
  RESET_LOCALFAIL();

  // this is too complex. Not tested
  EasyExp e1("pattern");  
  EasyExp e2("pattern!@#$%^&\\*\\(\\)-_=\\\\+\\[\\]:;/\\?<>.,~`"); 
  EasyExp e3("*pattern");  
  EasyExp e4("*pattern!@#$%^&\\*\\(\\)-_=\\\\+\\[\\]:;/\\?<>.,~`"); 
  EasyExp e5("pattern*");  
  EasyExp e6("pattern!@#$%^&\\*\\(\\)-_=\\\\+\\[\\]:;/\\?<>.,~`*"); 
  EasyExp e7("?pattern");  
  EasyExp e8("?pattern!@#$%^&\\*\\(\\)-_=\\\\+\\[\\]:;/\\?<>.,~`"); 
  EasyExp e9("pattern?");  
  EasyExp e10("pattern!@#$%^&\\*\\(\\)-_=\\\\+\\[\\]:;/\\?<>.,~`?"); 
}
#endif

//////////////////////////////////////////////////////////////////
TEST(EasyExpCompileTest, StringListStarStringList)
{
  RESET_LOCALFAIL();
  EasyExp e1("(abc|def|hij)");  
  EXPECT_FALSE(e1.backannotation()!="(abc|def|hij)") ;
  EXPECT_FALSE(e1.element_size()!=2) ;
  EXPECT_FALSE(e1.element(0)->nlist()!=3) ;
  EXPECT_FALSE(strcmp(e1.element(0)->what(),"MatchStringList")) ;
  EXPECT_FALSE(strcmp(e1.element(0)->contents(0),"abc")) ;
  EXPECT_FALSE(strcmp(e1.element(0)->contents(1),"def")) ;
  EXPECT_FALSE(strcmp(e1.element(0)->contents(2),"hij")) ;
  if(LOCALFAIL_COUNT()) G__pause();

  EXPECT_FALSE(strcmp(e1.element(1)->what(),"MatchEnd")) ;
  if(LOCALFAIL_COUNT()) G__pause();

  EasyExp e2("(abc||def|hij)");  
  EXPECT_FALSE(e2.backannotation()!="(abc|def|hij|)") ;
  if(LOCALFAIL_COUNT()) G__pause();
  EXPECT_FALSE(e2.element_size()!=2) ;
  EXPECT_FALSE(e2.element(0)->nlist()!=4) ;
  EXPECT_FALSE(strcmp(e2.element(0)->what(),"MatchStringList")) ;
  EXPECT_FALSE(strcmp(e2.element(0)->contents(0),"abc")) ;
  EXPECT_FALSE(strcmp(e2.element(0)->contents(1),"def")) ;
  EXPECT_FALSE(strcmp(e2.element(0)->contents(2),"hij")) ;
  EXPECT_FALSE(strcmp(e2.element(0)->contents(3),"")) ;
  if(LOCALFAIL_COUNT()) G__pause();

  EXPECT_FALSE(strcmp(e2.element(1)->what(),"MatchEnd")) ;
  if(LOCALFAIL_COUNT()) G__pause();

  EasyExp e3("*(abc|def|hij)");  
  EXPECT_FALSE(e3.backannotation()!="*(abc|def|hij)") ;
  if(LOCALFAIL_COUNT()) G__pause();
  EXPECT_FALSE(e3.element_size()!=2) ;
  EXPECT_FALSE(e3.element(0)->nlist()!=3) ;
  EXPECT_FALSE(strcmp(e3.element(0)->what(),"MatchStarStringList")) ;
  EXPECT_FALSE(strcmp(e3.element(0)->contents(0),"abc")) ;
  EXPECT_FALSE(strcmp(e3.element(0)->contents(1),"def")) ;
  EXPECT_FALSE(strcmp(e3.element(0)->contents(2),"hij")) ;
  if(LOCALFAIL_COUNT()) G__pause();

  EXPECT_FALSE(strcmp(e1.element(1)->what(),"MatchEnd")) ;
  if(LOCALFAIL_COUNT()) G__pause();

  EasyExp e4("(abc|def|hij)*");  
  EXPECT_FALSE(e4.backannotation()!="(abc|def|hij)*") ;
  if(LOCALFAIL_COUNT()) G__pause();
  EXPECT_FALSE(e4.element_size()!=2) ;
  EXPECT_FALSE(e4.element(0)->nlist()!=3) ;
  EXPECT_FALSE(strcmp(e4.element(0)->what(),"MatchStringList")) ;
  EXPECT_FALSE(strcmp(e4.element(0)->contents(0),"abc")) ;
  EXPECT_FALSE(strcmp(e4.element(0)->contents(1),"def")) ;
  EXPECT_FALSE(strcmp(e4.element(0)->contents(2),"hij")) ;
  if(LOCALFAIL_COUNT()) G__pause();

  EXPECT_FALSE(strcmp(e4.element(1)->what(),"MatchStarEnd")) ;
  if(LOCALFAIL_COUNT()) G__pause();

  EasyExp e5("*(abc|def|hij)*");  
  EXPECT_FALSE(e5.backannotation()!="*(abc|def|hij)*") ;
  if(LOCALFAIL_COUNT()) G__pause();
  EXPECT_FALSE(e5.element_size()!=2) ;
  EXPECT_FALSE(e5.element(0)->nlist()!=3) ;
  EXPECT_FALSE(strcmp(e5.element(0)->what(),"MatchStarStringList")) ;
  EXPECT_FALSE(strcmp(e5.element(0)->contents(0),"abc")) ;
  EXPECT_FALSE(strcmp(e5.element(0)->contents(1),"def")) ;
  EXPECT_FALSE(strcmp(e5.element(0)->contents(2),"hij")) ;
  if(LOCALFAIL_COUNT()) G__pause();

  EXPECT_FALSE(strcmp(e5.element(1)->what(),"MatchStarEnd")) ;
  if(LOCALFAIL_COUNT()) G__pause();

  EasyExp e6("(123|!#$%&'|-^\\|;:<>/|\\(\\)\\[\\]\\*\\?)");  

  EasyExp e7("*(abc|abcdef)hij*");  
  EXPECT_FALSE(e7.backannotation()!="*(abc|abcdef)hij*") ;
  if(LOCALFAIL_COUNT()) G__pause();
  EXPECT_FALSE(e7.element_size()!=3) ;
  EXPECT_FALSE(e7.element(0)->nlist()!=2) ;
  EXPECT_FALSE(strcmp(e7.element(0)->what(),"MatchStarStringList")) ;
  EXPECT_FALSE(strcmp(e7.element(0)->contents(0),"abc")) ;
  EXPECT_FALSE(strcmp(e7.element(0)->contents(1),"abcdef")) ;
  if(LOCALFAIL_COUNT()) G__pause();

  EXPECT_FALSE(strcmp(e7.element(1)->what(),"MatchString")) ;
  EXPECT_FALSE(strcmp(e7.element(1)->contents(0),"hij")) ;

  EXPECT_FALSE(strcmp(e7.element(2)->what(),"MatchStarEnd")) ;
  if(LOCALFAIL_COUNT()) G__pause();

  EasyExp e8("*(abc|abcdef)*");  
  EXPECT_FALSE(e8.backannotation()!="*(abc|abcdef)*") ;
  if(LOCALFAIL_COUNT()) G__pause();
  EXPECT_FALSE(e8.element_size()!=2) ;
  EXPECT_FALSE(e8.element(0)->nlist()!=2) ;
  EXPECT_FALSE(strcmp(e8.element(0)->what(),"MatchStarStringList")) ;
  EXPECT_FALSE(strcmp(e8.element(0)->contents(0),"abc")) ;
  EXPECT_FALSE(strcmp(e8.element(0)->contents(1),"abcdef")) ;
  if(LOCALFAIL_COUNT()) G__pause();

  EXPECT_FALSE(strcmp(e8.element(1)->what(),"MatchStarEnd")) ;
  if(LOCALFAIL_COUNT()) G__pause();
}

//////////////////////////////////////////////////////////////////
TEST(EasyExpCompileTest, CharListStarCharList)
{
  RESET_LOCALFAIL();
  EasyExp e1("[12aeg]");  
  EXPECT_FALSE(e1.backannotation()!="[12aeg]") ;
  EXPECT_FALSE(e1.pattern()!="[12aeg]") ;
  EXPECT_FALSE(e1.element_size()!=2) ;
  EXPECT_FALSE(strcmp(e1.element(0)->what(),"MatchCharList")) ;
  EXPECT_FALSE(e1.element(0)->nlist()!=5) ;
  EXPECT_FALSE(strcmp(e1.element(0)->contents(0),"1")) ;
  EXPECT_FALSE(strcmp(e1.element(0)->contents(1),"2")) ;
  EXPECT_FALSE(strcmp(e1.element(0)->contents(2),"a")) ;
  EXPECT_FALSE(strcmp(e1.element(0)->contents(3),"e")) ;
  EXPECT_FALSE(strcmp(e1.element(0)->contents(4),"g")) ;
  EXPECT_FALSE(strcmp(e1.element(1)->what(),"MatchEnd")) ;

  EasyExp e2("[a-z0-9]");  
  EXPECT_FALSE(e2.backannotation()!="[abcdefghijklmnopqrstuvwxyz0123456789]") ;
  EXPECT_FALSE(e2.pattern()!="[a-z0-9]") ;
  EXPECT_FALSE(e2.element_size()!=2) ;
  EXPECT_FALSE(strcmp(e2.element(0)->what(),"MatchCharList")) ;
  EXPECT_FALSE(e2.element(0)->nlist()!=36) ;
  EXPECT_FALSE(strcmp(e2.element(0)->contents(0),"a")) ;
  EXPECT_FALSE(strcmp(e2.element(0)->contents(25),"z")) ;
  EXPECT_FALSE(strcmp(e2.element(0)->contents(26),"0")) ;
  EXPECT_FALSE(strcmp(e2.element(0)->contents(35),"9")) ;
  EXPECT_FALSE(strcmp(e2.element(1)->what(),"MatchEnd")) ;

  EasyExp e3("*[12aeg]");  
  EXPECT_FALSE(e3.backannotation()!="*[12aeg]") ;
  EXPECT_FALSE(e3.pattern()!="*[12aeg]") ;
  EXPECT_FALSE(e3.element_size()!=2) ;
  EXPECT_FALSE(strcmp(e3.element(0)->what(),"MatchStarCharList")) ;
  EXPECT_FALSE(e3.element(0)->nlist()!=5) ;
  EXPECT_FALSE(strcmp(e3.element(0)->contents(0),"1")) ;
  EXPECT_FALSE(strcmp(e3.element(0)->contents(1),"2")) ;
  EXPECT_FALSE(strcmp(e3.element(0)->contents(2),"a")) ;
  EXPECT_FALSE(strcmp(e3.element(0)->contents(3),"e")) ;
  EXPECT_FALSE(strcmp(e3.element(0)->contents(4),"g")) ;
  EXPECT_FALSE(strcmp(e3.element(1)->what(),"MatchEnd")) ;

  EasyExp e4("*[a-z0-9]");  
  EXPECT_FALSE(e4.backannotation()!="*[abcdefghijklmnopqrstuvwxyz0123456789]") ;
  EXPECT_FALSE(e4.pattern()!="*[a-z0-9]") ;
  EXPECT_FALSE(e4.element_size()!=2) ;
  EXPECT_FALSE(strcmp(e4.element(0)->what(),"MatchStarCharList")) ;
  EXPECT_FALSE(e4.element(0)->nlist()!=36) ;
  EXPECT_FALSE(strcmp(e4.element(0)->contents(0),"a")) ;
  EXPECT_FALSE(strcmp(e4.element(0)->contents(25),"z")) ;
  EXPECT_FALSE(strcmp(e4.element(0)->contents(26),"0")) ;
  EXPECT_FALSE(strcmp(e4.element(0)->contents(35),"9")) ;
  EXPECT_FALSE(strcmp(e4.element(1)->what(),"MatchEnd")) ;

  EasyExp e5("[12aeg]*");  
  EXPECT_FALSE(e5.backannotation()!="[12aeg]*") ;
  EXPECT_FALSE(e5.pattern()!="[12aeg]*") ;
  EXPECT_FALSE(e5.element_size()!=2) ;
  EXPECT_FALSE(strcmp(e5.element(0)->what(),"MatchCharList")) ;
  EXPECT_FALSE(e5.element(0)->nlist()!=5) ;
  EXPECT_FALSE(strcmp(e5.element(0)->contents(0),"1")) ;
  EXPECT_FALSE(strcmp(e5.element(0)->contents(1),"2")) ;
  EXPECT_FALSE(strcmp(e5.element(0)->contents(2),"a")) ;
  EXPECT_FALSE(strcmp(e5.element(0)->contents(3),"e")) ;
  EXPECT_FALSE(strcmp(e5.element(0)->contents(4),"g")) ;
  EXPECT_FALSE(strcmp(e5.element(1)->what(),"MatchStarEnd")) ;

  EasyExp e6("[a-z0-9]*");  
  EXPECT_FALSE(e6.backannotation()!="[abcdefghijklmnopqrstuvwxyz0123456789]*") ;
  EXPECT_FALSE(e6.pattern()!="[a-z0-9]*") ;
  EXPECT_FALSE(e6.element_size()!=2) ;
  EXPECT_FALSE(strcmp(e6.element(0)->what(),"MatchCharList")) ;
  EXPECT_FALSE(e6.element(0)->nlist()!=36) ;
  EXPECT_FALSE(strcmp(e6.element(0)->contents(0),"a")) ;
  EXPECT_FALSE(strcmp(e6.element(0)->contents(25),"z")) ;
  EXPECT_FALSE(strcmp(e6.element(0)->contents(26),"0")) ;
  EXPECT_FALSE(strcmp(e6.element(0)->contents(35),"9")) ;
  EXPECT_FALSE(strcmp(e6.element(1)->what(),"MatchStarEnd")) ;

  EasyExp e7("*n[0-9a-z:3-5]p*");  
  EXPECT_FALSE(e7.backannotation()!="*n[0123456789abcdefghijklmnopqrstuvwxyz:3-5]p*") ;
  EXPECT_FALSE(e7.pattern()!="*n[0-9a-z:3-5]p*") ;

  EXPECT_FALSE(e7.element_size()!=4) ;
  EXPECT_FALSE(strcmp(e7.element(0)->what(),"MatchStarString")) ;
  EXPECT_FALSE(strcmp(e7.element(0)->contents(0),"n")) ;

  EXPECT_FALSE(strcmp(e7.element(1)->what(),"MatchCharList")) ;
  EXPECT_FALSE(e7.element(1)->nlist()!=36) ;
  EXPECT_FALSE(strcmp(e7.element(1)->contents(0),"0")) ;
  EXPECT_FALSE(strcmp(e7.element(1)->contents(9),"9")) ;
  EXPECT_FALSE(strcmp(e7.element(1)->contents(10),"a")) ;
  EXPECT_FALSE(strcmp(e7.element(1)->contents(35),"z")) ;
  EXPECT_FALSE(e7.element(1)->parameter(0)!=3) ;
  EXPECT_FALSE(e7.element(1)->parameter(1)!=5) ;

  EXPECT_FALSE(strcmp(e7.element(2)->what(),"MatchString")) ;
  EXPECT_FALSE(strcmp(e7.element(2)->contents(0),"p")) ;

  EXPECT_FALSE(strcmp(e7.element(3)->what(),"MatchStarEnd")) ;

  EasyExp e8("*n[0-9:2-5]p*");  
  EXPECT_FALSE(e8.backannotation()!="*n[0123456789:2-5]p*") ;
  EXPECT_FALSE(e8.pattern()!="*n[0-9:2-5]p*") ;

  EXPECT_FALSE(e8.element_size()!=4) ;
  EXPECT_FALSE(strcmp(e8.element(0)->what(),"MatchStarString")) ;
  EXPECT_FALSE(strcmp(e8.element(0)->contents(0),"n")) ;

  EXPECT_FALSE(strcmp(e8.element(1)->what(),"MatchCharList")) ;
  EXPECT_FALSE(e8.element(1)->nlist()!=10) ;
  EXPECT_FALSE(strcmp(e8.element(1)->contents(0),"0")) ;
  EXPECT_FALSE(strcmp(e8.element(1)->contents(9),"9")) ;
  EXPECT_FALSE(e8.element(1)->parameter(0)!=2) ;
  EXPECT_FALSE(e8.element(1)->parameter(1)!=5) ;

  EXPECT_FALSE(strcmp(e8.element(2)->what(),"MatchString")) ;
  EXPECT_FALSE(strcmp(e8.element(2)->contents(0),"p")) ;

  EXPECT_FALSE(strcmp(e8.element(3)->what(),"MatchStarEnd")) ;

  EasyExp e9("*n[0-9:0-2]p*");  
  EXPECT_FALSE(e9.backannotation()!="*n[0123456789:0-2]p*") ;
  EXPECT_FALSE(e9.pattern()!="*n[0-9:0-2]p*") ;

  EXPECT_FALSE(e9.element_size()!=4) ;
  EXPECT_FALSE(strcmp(e9.element(0)->what(),"MatchStarString")) ;
  EXPECT_FALSE(strcmp(e9.element(0)->contents(0),"n")) ;

  EXPECT_FALSE(strcmp(e9.element(1)->what(),"MatchCharList")) ;
  EXPECT_FALSE(e9.element(1)->nlist()!=10) ;
  EXPECT_FALSE(strcmp(e9.element(1)->contents(0),"0")) ;
  EXPECT_FALSE(strcmp(e9.element(1)->contents(9),"9")) ;
  EXPECT_FALSE(e9.element(1)->parameter(0)!=0) ;
  EXPECT_FALSE(e9.element(1)->parameter(1)!=2) ;

  EXPECT_FALSE(strcmp(e9.element(2)->what(),"MatchString")) ;
  EXPECT_FALSE(strcmp(e9.element(2)->contents(0),"p")) ;

  EXPECT_FALSE(strcmp(e8.element(3)->what(),"MatchStarEnd")) ;

  EasyExp e10("*n[-\\^]p*");  
  EXPECT_FALSE(e10.backannotation()!="*n[-^]p*") ; // Debatable
  //EXPECT_FALSE(e10.backannotation()!="*n[-\\^]p*") ; 
  //e10.disp();
  EXPECT_FALSE(e10.pattern()!="*n[-\\^]p*") ;

  EXPECT_FALSE(e10.element_size()!=4) ;
  EXPECT_FALSE(strcmp(e10.element(0)->what(),"MatchStarString")) ;
  EXPECT_FALSE(strcmp(e10.element(0)->contents(0),"n")) ;

  EXPECT_FALSE(strcmp(e10.element(1)->what(),"MatchCharList")) ;
  EXPECT_FALSE(e10.element(1)->nlist()!=2) ;
  EXPECT_FALSE(strcmp(e10.element(1)->contents(0),"-")) ;
  EXPECT_FALSE(strcmp(e10.element(1)->contents(1),"^")) ;
  EXPECT_FALSE(e10.element(1)->parameter(0)!=1) ;
  EXPECT_FALSE(e10.element(1)->parameter(1)!=-1) ;

  EXPECT_FALSE(strcmp(e10.element(2)->what(),"MatchString")) ;
  EXPECT_FALSE(strcmp(e10.element(2)->contents(0),"p")) ;

  EXPECT_FALSE(strcmp(e10.element(3)->what(),"MatchStarEnd")) ;

  if(LOCALFAIL_COUNT()) G__pause();
}
//////////////////////////////////////////////////////////////////
TEST(EasyExpCompileTest, NotCharListStarNotCharList)
{
  EasyExp e1("[^12aeg]");  
  EXPECT_FALSE(e1.backannotation()!="[^12aeg]") ;
  if(LOCALFAIL_COUNT()) G__pause();
  EXPECT_FALSE(e1.pattern()!="[^12aeg]") ;
  if(LOCALFAIL_COUNT()) G__pause();

  EXPECT_FALSE(e1.element_size()!=2) ;
  EXPECT_FALSE(strcmp(e1.element(0)->what(),"MatchNotCharList")) ;
  EXPECT_FALSE(e1.element(0)->nlist()!=5) ;
  EXPECT_FALSE(strcmp(e1.element(0)->contents(0),"1")) ;
  EXPECT_FALSE(strcmp(e1.element(0)->contents(1),"2")) ;
  EXPECT_FALSE(strcmp(e1.element(0)->contents(2),"a")) ;
  EXPECT_FALSE(strcmp(e1.element(0)->contents(3),"e")) ;
  EXPECT_FALSE(strcmp(e1.element(0)->contents(4),"g")) ;
  EXPECT_FALSE(strcmp(e1.element(1)->what(),"MatchEnd")) ;

  if(LOCALFAIL_COUNT()) G__pause();

  EasyExp e2("[^a-z0-9]");  
  EXPECT_FALSE(e2.backannotation()!="[^abcdefghijklmnopqrstuvwxyz0123456789]") ;
  if(LOCALFAIL_COUNT()) G__pause();
  EXPECT_FALSE(e2.pattern()!="[^a-z0-9]") ;
  if(LOCALFAIL_COUNT()) G__pause();

  EXPECT_FALSE(e2.element_size()!=2) ;
  EXPECT_FALSE(strcmp(e2.element(0)->what(),"MatchNotCharList")) ;
  EXPECT_FALSE(e2.element(0)->nlist()!=36) ;
  EXPECT_FALSE(strcmp(e2.element(0)->contents(0),"a")) ;
  EXPECT_FALSE(strcmp(e2.element(0)->contents(25),"z")) ;
  EXPECT_FALSE(strcmp(e2.element(0)->contents(26),"0")) ;
  EXPECT_FALSE(strcmp(e2.element(0)->contents(35),"9")) ;
  EXPECT_FALSE(strcmp(e2.element(1)->what(),"MatchEnd")) ;

  if(LOCALFAIL_COUNT()) G__pause();

  EasyExp e3("*[^12aeg]");  
  EXPECT_FALSE(e3.backannotation()!="*[^12aeg]") ;
  if(LOCALFAIL_COUNT()) G__pause();
  EXPECT_FALSE(e3.pattern()!="*[^12aeg]") ;
  if(LOCALFAIL_COUNT()) G__pause();

  EXPECT_FALSE(e3.element_size()!=2) ;
  EXPECT_FALSE(strcmp(e3.element(0)->what(),"MatchStarNotCharList")) ;
  EXPECT_FALSE(e3.element(0)->nlist()!=5) ;
  EXPECT_FALSE(strcmp(e3.element(0)->contents(0),"1")) ;
  EXPECT_FALSE(strcmp(e3.element(0)->contents(1),"2")) ;
  EXPECT_FALSE(strcmp(e3.element(0)->contents(2),"a")) ;
  EXPECT_FALSE(strcmp(e3.element(0)->contents(3),"e")) ;
  EXPECT_FALSE(strcmp(e3.element(0)->contents(4),"g")) ;
  EXPECT_FALSE(strcmp(e3.element(1)->what(),"MatchEnd")) ;

  if(LOCALFAIL_COUNT()) G__pause();

  EasyExp e4("*[^a-z0-9]");  
  EXPECT_FALSE(e4.backannotation()!="*[^abcdefghijklmnopqrstuvwxyz0123456789]") ;
  if(LOCALFAIL_COUNT()) G__pause();
  EXPECT_FALSE(e4.pattern()!="*[^a-z0-9]") ;
  if(LOCALFAIL_COUNT()) G__pause();

  EXPECT_FALSE(e4.element_size()!=2) ;
  EXPECT_FALSE(strcmp(e4.element(0)->what(),"MatchStarNotCharList")) ;
  EXPECT_FALSE(e4.element(0)->nlist()!=36) ;
  EXPECT_FALSE(strcmp(e4.element(0)->contents(0),"a")) ;
  EXPECT_FALSE(strcmp(e4.element(0)->contents(25),"z")) ;
  EXPECT_FALSE(strcmp(e4.element(0)->contents(26),"0")) ;
  EXPECT_FALSE(strcmp(e4.element(0)->contents(35),"9")) ;
  EXPECT_FALSE(strcmp(e4.element(1)->what(),"MatchEnd")) ;

  if(LOCALFAIL_COUNT()) G__pause();

  EasyExp e5("[^12aeg]*");  
  EXPECT_FALSE(e5.backannotation()!="[^12aeg]*") ;
  if(LOCALFAIL_COUNT()) G__pause();
  EXPECT_FALSE(e5.pattern()!="[^12aeg]*") ;
  if(LOCALFAIL_COUNT()) G__pause();

  EXPECT_FALSE(e5.element_size()!=2) ;
  EXPECT_FALSE(strcmp(e5.element(0)->what(),"MatchNotCharList")) ;
  EXPECT_FALSE(e5.element(0)->nlist()!=5) ;
  EXPECT_FALSE(strcmp(e5.element(0)->contents(0),"1")) ;
  EXPECT_FALSE(strcmp(e5.element(0)->contents(1),"2")) ;
  EXPECT_FALSE(strcmp(e5.element(0)->contents(2),"a")) ;
  EXPECT_FALSE(strcmp(e5.element(0)->contents(3),"e")) ;
  EXPECT_FALSE(strcmp(e5.element(0)->contents(4),"g")) ;
  EXPECT_FALSE(strcmp(e5.element(1)->what(),"MatchStarEnd")) ;

  if(LOCALFAIL_COUNT()) G__pause();

  EasyExp e6("[^a-z0-9]*");  
  EXPECT_FALSE(e6.backannotation()!="[^abcdefghijklmnopqrstuvwxyz0123456789]*") ;
  if(LOCALFAIL_COUNT()) G__pause();
  EXPECT_FALSE(e6.pattern()!="[^a-z0-9]*") ;
  if(LOCALFAIL_COUNT()) G__pause();

  EXPECT_FALSE(e6.element_size()!=2) ;
  EXPECT_FALSE(strcmp(e6.element(0)->what(),"MatchNotCharList")) ;
  EXPECT_FALSE(e6.element(0)->nlist()!=36) ;
  EXPECT_FALSE(strcmp(e6.element(0)->contents(0),"a")) ;
  EXPECT_FALSE(strcmp(e6.element(0)->contents(25),"z")) ;
  EXPECT_FALSE(strcmp(e6.element(0)->contents(26),"0")) ;
  EXPECT_FALSE(strcmp(e6.element(0)->contents(35),"9")) ;
  EXPECT_FALSE(strcmp(e6.element(1)->what(),"MatchStarEnd")) ;

  if(LOCALFAIL_COUNT()) G__pause();

  EasyExp e7("*n[^0-9a-z:3-5]p*");  
  EXPECT_FALSE(e7.backannotation()!="*n[^0123456789abcdefghijklmnopqrstuvwxyz:3-5]p*") ;
  if(LOCALFAIL_COUNT()) G__pause();
  EXPECT_FALSE(e7.pattern()!="*n[^0-9a-z:3-5]p*") ;
  if(LOCALFAIL_COUNT()) G__pause();

  EXPECT_FALSE(e7.element_size()!=4) ;
  EXPECT_FALSE(strcmp(e7.element(0)->what(),"MatchStarString")) ;
  EXPECT_FALSE(strcmp(e7.element(0)->contents(0),"n")) ;

  EXPECT_FALSE(strcmp(e7.element(1)->what(),"MatchNotCharList")) ;
  EXPECT_FALSE(e7.element(1)->nlist()!=36) ;
  EXPECT_FALSE(strcmp(e7.element(1)->contents(0),"0")) ;
  EXPECT_FALSE(strcmp(e7.element(1)->contents(9),"9")) ;
  EXPECT_FALSE(strcmp(e7.element(1)->contents(10),"a")) ;
  EXPECT_FALSE(strcmp(e7.element(1)->contents(35),"z")) ;
  EXPECT_FALSE(e7.element(1)->parameter(0)!=3) ;
  EXPECT_FALSE(e7.element(1)->parameter(1)!=5) ;

  EXPECT_FALSE(strcmp(e7.element(2)->what(),"MatchString")) ;
  EXPECT_FALSE(strcmp(e7.element(2)->contents(0),"p")) ;

  EXPECT_FALSE(strcmp(e7.element(3)->what(),"MatchStarEnd")) ;

  if(LOCALFAIL_COUNT()) G__pause();

  EasyExp e8("*n[^0-9a-z:3]p*");  
  EXPECT_FALSE(e8.backannotation()!="*n[^0123456789abcdefghijklmnopqrstuvwxyz:3]p*") ;
  if(LOCALFAIL_COUNT()) G__pause();
  EXPECT_FALSE(e8.pattern()!="*n[^0-9a-z:3]p*") ;
  if(LOCALFAIL_COUNT()) G__pause();

  EXPECT_FALSE(e8.element_size()!=4) ;
  EXPECT_FALSE(strcmp(e8.element(0)->what(),"MatchStarString")) ;
  EXPECT_FALSE(strcmp(e8.element(0)->contents(0),"n")) ;

  EXPECT_FALSE(strcmp(e8.element(1)->what(),"MatchNotCharList")) ;
  EXPECT_FALSE(e8.element(1)->nlist()!=36) ;
  EXPECT_FALSE(strcmp(e8.element(1)->contents(0),"0")) ;
  EXPECT_FALSE(strcmp(e8.element(1)->contents(9),"9")) ;
  EXPECT_FALSE(strcmp(e8.element(1)->contents(10),"a")) ;
  EXPECT_FALSE(strcmp(e8.element(1)->contents(35),"z")) ;
  EXPECT_FALSE(e8.element(1)->parameter(0)!=3) ;
  EXPECT_FALSE(e8.element(1)->parameter(1)!=-1) ;

  EXPECT_FALSE(strcmp(e8.element(2)->what(),"MatchString")) ;
  EXPECT_FALSE(strcmp(e8.element(2)->contents(0),"p")) ;

  EXPECT_FALSE(strcmp(e8.element(3)->what(),"MatchStarEnd")) ;

  if(LOCALFAIL_COUNT()) G__pause();

  EasyExp e9("*n[^0-9a-z:0-2]p*");  
  EXPECT_FALSE(e9.backannotation()!="*n[^0123456789abcdefghijklmnopqrstuvwxyz:0-2]p*") ;
  if(LOCALFAIL_COUNT()) G__pause();
  EXPECT_FALSE(e9.pattern()!="*n[^0-9a-z:0-2]p*") ;
  if(LOCALFAIL_COUNT()) G__pause();

  EXPECT_FALSE(e9.element_size()!=4) ;
  EXPECT_FALSE(strcmp(e9.element(0)->what(),"MatchStarString")) ;
  EXPECT_FALSE(strcmp(e9.element(0)->contents(0),"n")) ;

  EXPECT_FALSE(strcmp(e9.element(1)->what(),"MatchNotCharList")) ;
  EXPECT_FALSE(e9.element(1)->nlist()!=36) ;
  EXPECT_FALSE(strcmp(e9.element(1)->contents(0),"0")) ;
  EXPECT_FALSE(strcmp(e9.element(1)->contents(9),"9")) ;
  EXPECT_FALSE(strcmp(e9.element(1)->contents(10),"a")) ;
  EXPECT_FALSE(strcmp(e9.element(1)->contents(35),"z")) ;
  EXPECT_FALSE(e9.element(1)->parameter(0)!=0) ;
  EXPECT_FALSE(e9.element(1)->parameter(1)!=2) ;

  EXPECT_FALSE(strcmp(e9.element(2)->what(),"MatchString")) ;
  EXPECT_FALSE(strcmp(e9.element(2)->contents(0),"p")) ;

  EXPECT_FALSE(strcmp(e9.element(3)->what(),"MatchStarEnd")) ;

  if(LOCALFAIL_COUNT()) G__pause();

  EasyExp e10("*n[^\\-\\^0-9a-z:0-2]p*");  
  EXPECT_FALSE(e10.backannotation()!="*n[^-^0123456789abcdefghijklmnopqrstuvwxyz:0-2]p*") ;
  if(LOCALFAIL_COUNT()) G__pause();
  EXPECT_FALSE(e10.pattern()!="*n[^\\-\\^0-9a-z:0-2]p*") ;
  if(LOCALFAIL_COUNT()) G__pause();

  EXPECT_FALSE(e10.element_size()!=4) ;
  EXPECT_FALSE(strcmp(e10.element(0)->what(),"MatchStarString")) ;
  EXPECT_FALSE(strcmp(e10.element(0)->contents(0),"n")) ;

  EXPECT_FALSE(strcmp(e10.element(1)->what(),"MatchNotCharList")) ;
  EXPECT_FALSE(e10.element(1)->nlist()!=38) ;
  EXPECT_FALSE(strcmp(e10.element(1)->contents(0),"-")) ;
  EXPECT_FALSE(strcmp(e10.element(1)->contents(1),"^")) ;
  EXPECT_FALSE(strcmp(e10.element(1)->contents(2),"0")) ;
  EXPECT_FALSE(strcmp(e10.element(1)->contents(11),"9")) ;
  EXPECT_FALSE(strcmp(e10.element(1)->contents(12),"a")) ;
  EXPECT_FALSE(strcmp(e10.element(1)->contents(37),"z")) ;
  EXPECT_FALSE(e10.element(1)->parameter(0)!=0) ;
  EXPECT_FALSE(e10.element(1)->parameter(1)!=2) ;
  if(LOCALFAIL_COUNT()) G__pause();

  EXPECT_FALSE(strcmp(e10.element(2)->what(),"MatchString")) ;
  EXPECT_FALSE(strcmp(e10.element(2)->contents(0),"p")) ;
  if(LOCALFAIL_COUNT()) G__pause();

  EXPECT_FALSE(strcmp(e10.element(3)->what(),"MatchStarEnd")) ;

  if(LOCALFAIL_COUNT()) G__pause();
}

//////////////////////////////////////////////////////////////////
TEST(EasyExpCompileTest, NumberRangeStarNumberRange)
{
  RESET_LOCALFAIL();

  EasyExp e1("pattern(1999..2020)");           
  EXPECT_FALSE(e1.backannotation()!="pattern(1999..2020)") ;
  if(LOCALFAIL_COUNT()) G__pause();
  EXPECT_FALSE(e1.pattern()!="pattern(1999..2020)") ;
  if(LOCALFAIL_COUNT()) G__pause();


  EXPECT_FALSE(e1.element_size()!=3) ;
  EXPECT_FALSE(strcmp(e1.element(0)->what(),"MatchString")) ;
  EXPECT_FALSE(strcmp(e1.element(0)->contents(0),"pattern")) ;

  EXPECT_FALSE(strcmp(e1.element(1)->what(),"MatchNumberRange")) ;
  EXPECT_FALSE(strcmp(e1.element(1)->contents(0),"1999")) ;
  EXPECT_FALSE(strcmp(e1.element(1)->contents(1),"2020")) ;
  EXPECT_FALSE(strcmp(e1.element(1)->contents(2),"0")) ;

  EXPECT_FALSE(strcmp(e1.element(2)->what(),"MatchEnd")) ;

  if(LOCALFAIL_COUNT()) G__pause();

  EasyExp e2("pattern(1999..2020:4)");         
  EXPECT_FALSE(e2.backannotation()!="pattern(1999..2020:4)") ;
  if(LOCALFAIL_COUNT()) G__pause();
  EXPECT_FALSE(e2.pattern()!="pattern(1999..2020:4)") ;
  if(LOCALFAIL_COUNT()) G__pause();


  EXPECT_FALSE(e2.element_size()!=3) ;
  EXPECT_FALSE(strcmp(e2.element(0)->what(),"MatchString")) ;
  EXPECT_FALSE(strcmp(e2.element(0)->contents(0),"pattern")) ;

  EXPECT_FALSE(strcmp(e2.element(1)->what(),"MatchNumberRange")) ;
  EXPECT_FALSE(strcmp(e2.element(1)->contents(0),"1999")) ;
  EXPECT_FALSE(strcmp(e2.element(1)->contents(1),"2020")) ;
  EXPECT_FALSE(strcmp(e2.element(1)->contents(2),"4")) ;

  EXPECT_FALSE(strcmp(e2.element(2)->what(),"MatchEnd")) ;

  if(LOCALFAIL_COUNT()) G__pause();

  EasyExp e3("pattern*(1999..2020)");           
  EXPECT_FALSE(e3.backannotation()!="pattern*(1999..2020)") ;
  if(LOCALFAIL_COUNT()) G__pause();

  EXPECT_FALSE(e3.element_size()!=3) ;
  EXPECT_FALSE(strcmp(e3.element(0)->what(),"MatchString")) ;
  EXPECT_FALSE(strcmp(e3.element(0)->contents(0),"pattern")) ;

  EXPECT_FALSE(strcmp(e3.element(1)->what(),"MatchStarNumberRange")) ;
  EXPECT_FALSE(strcmp(e3.element(1)->contents(0),"1999")) ;
  EXPECT_FALSE(strcmp(e3.element(1)->contents(1),"2020")) ;
  EXPECT_FALSE(strcmp(e3.element(1)->contents(2),"0")) ;

  EXPECT_FALSE(strcmp(e3.element(2)->what(),"MatchEnd")) ;

  if(LOCALFAIL_COUNT()) G__pause();

  EasyExp e4("pattern*(1999..2020:4)");         
  EXPECT_FALSE(e4.backannotation()!="pattern*(1999..2020:4)") ;
  if(LOCALFAIL_COUNT()) G__pause();

  EXPECT_FALSE(e4.element_size()!=3) ;
  EXPECT_FALSE(strcmp(e4.element(0)->what(),"MatchString")) ;
  EXPECT_FALSE(strcmp(e4.element(0)->contents(0),"pattern")) ;

  EXPECT_FALSE(strcmp(e4.element(1)->what(),"MatchStarNumberRange")) ;
  EXPECT_FALSE(strcmp(e4.element(1)->contents(0),"1999")) ;
  EXPECT_FALSE(strcmp(e4.element(1)->contents(1),"2020")) ;
  EXPECT_FALSE(strcmp(e4.element(1)->contents(2),"4")) ;

  EXPECT_FALSE(strcmp(e4.element(2)->what(),"MatchEnd")) ;

  if(LOCALFAIL_COUNT()) G__pause();

  EasyExp e5("pattern(1999..2020)*");           
  EXPECT_FALSE(e5.backannotation()!="pattern(1999..2020)*") ;
  if(LOCALFAIL_COUNT()) G__pause();

  EXPECT_FALSE(e5.element_size()!=3) ;
  EXPECT_FALSE(strcmp(e5.element(0)->what(),"MatchString")) ;
  EXPECT_FALSE(strcmp(e5.element(0)->contents(0),"pattern")) ;

  EXPECT_FALSE(strcmp(e5.element(1)->what(),"MatchNumberRange")) ;
  EXPECT_FALSE(strcmp(e5.element(1)->contents(0),"1999")) ;
  EXPECT_FALSE(strcmp(e5.element(1)->contents(1),"2020")) ;
  EXPECT_FALSE(strcmp(e5.element(1)->contents(2),"0")) ;

  EXPECT_FALSE(strcmp(e5.element(2)->what(),"MatchStarEnd")) ;

  if(LOCALFAIL_COUNT()) G__pause();

  EasyExp e6("pattern(1999..2020:4)*");         
  EXPECT_FALSE(e6.backannotation()!="pattern(1999..2020:4)*") ;
  if(LOCALFAIL_COUNT()) G__pause();

  EXPECT_FALSE(e6.element_size()!=3) ;
  EXPECT_FALSE(strcmp(e6.element(0)->what(),"MatchString")) ;
  EXPECT_FALSE(strcmp(e6.element(0)->contents(0),"pattern")) ;

  EXPECT_FALSE(strcmp(e6.element(1)->what(),"MatchNumberRange")) ;
  EXPECT_FALSE(strcmp(e6.element(1)->contents(0),"1999")) ;
  EXPECT_FALSE(strcmp(e6.element(1)->contents(1),"2020")) ;
  EXPECT_FALSE(strcmp(e6.element(1)->contents(2),"4")) ;

  EXPECT_FALSE(strcmp(e6.element(2)->what(),"MatchStarEnd")) ;

  if(LOCALFAIL_COUNT()) G__pause();

  EasyExp e7("pattern*(1999..2020)*");          
  EXPECT_FALSE(e7.backannotation()!="pattern*(1999..2020)*") ;
  if(LOCALFAIL_COUNT()) G__pause();

  EXPECT_FALSE(e7.element_size()!=3) ;
  EXPECT_FALSE(strcmp(e7.element(0)->what(),"MatchString")) ;
  EXPECT_FALSE(strcmp(e7.element(0)->contents(0),"pattern")) ;

  EXPECT_FALSE(strcmp(e7.element(1)->what(),"MatchStarNumberRange")) ;
  EXPECT_FALSE(strcmp(e7.element(1)->contents(0),"1999")) ;
  EXPECT_FALSE(strcmp(e7.element(1)->contents(1),"2020")) ;
  EXPECT_FALSE(strcmp(e7.element(1)->contents(2),"0")) ;

  EXPECT_FALSE(strcmp(e7.element(2)->what(),"MatchStarEnd")) ;

  if(LOCALFAIL_COUNT()) G__pause();

  EasyExp e8("pattern*(1999..2020:4)*");         
  EXPECT_FALSE(e8.backannotation()!="pattern*(1999..2020:4)*") ;
  if(LOCALFAIL_COUNT()) G__pause();

  EXPECT_FALSE(e8.element_size()!=3) ;
  EXPECT_FALSE(strcmp(e8.element(0)->what(),"MatchString")) ;
  EXPECT_FALSE(strcmp(e8.element(0)->contents(0),"pattern")) ;

  EXPECT_FALSE(strcmp(e8.element(1)->what(),"MatchStarNumberRange")) ;
  EXPECT_FALSE(strcmp(e8.element(1)->contents(0),"1999")) ;
  EXPECT_FALSE(strcmp(e8.element(1)->contents(1),"2020")) ;
  EXPECT_FALSE(strcmp(e8.element(1)->contents(2),"4")) ;

  EXPECT_FALSE(strcmp(e8.element(2)->what(),"MatchStarEnd")) ;

  if(LOCALFAIL_COUNT()) G__pause();

  EasyExp e9("pattern[0-9:3](1999..2020:4)*");       
  EXPECT_FALSE(e9.backannotation()!="pattern[0123456789:3](1999..2020:4)*") ;
  if(LOCALFAIL_COUNT()) G__pause();

  EXPECT_FALSE(e9.element_size()!=4) ;
  EXPECT_FALSE(strcmp(e9.element(0)->what(),"MatchString")) ;
  EXPECT_FALSE(strcmp(e9.element(0)->contents(0),"pattern")) ;

  EXPECT_FALSE(strcmp(e9.element(1)->what(),"MatchCharList")) ;
  EXPECT_FALSE(e9.element(1)->nlist()!=10) ;
  EXPECT_FALSE(strcmp(e9.element(1)->contents(0),"0")) ;
  EXPECT_FALSE(strcmp(e9.element(1)->contents(9),"9")) ;
  EXPECT_FALSE(e9.element(1)->parameter(0)!=3) ;
  EXPECT_FALSE(e9.element(1)->parameter(1)!=-1) ;

  EXPECT_FALSE(strcmp(e9.element(2)->what(),"MatchNumberRange")) ;
  EXPECT_FALSE(strcmp(e9.element(2)->contents(0),"1999")) ;
  EXPECT_FALSE(strcmp(e9.element(2)->contents(1),"2020")) ;
  EXPECT_FALSE(strcmp(e9.element(2)->contents(2),"4")) ;

  EXPECT_FALSE(strcmp(e9.element(3)->what(),"MatchStarEnd")) ;

  if(LOCALFAIL_COUNT()) G__pause();

  EasyExp e10("pattern(010..050:3)(1999..2020:4)*");   
  EXPECT_FALSE(e10.backannotation()!="pattern(10..50:3)(1999..2020:4)*") ;
  if(LOCALFAIL_COUNT()) G__pause();

  EXPECT_FALSE(e10.element_size()!=4) ;
  EXPECT_FALSE(strcmp(e10.element(0)->what(),"MatchString")) ;
  EXPECT_FALSE(strcmp(e10.element(0)->contents(0),"pattern")) ;

  EXPECT_FALSE(strcmp(e10.element(1)->what(),"MatchNumberRange")) ;
  EXPECT_FALSE(strcmp(e10.element(1)->contents(0),"10")) ;
  EXPECT_FALSE(strcmp(e10.element(1)->contents(1),"50")) ;
  EXPECT_FALSE(strcmp(e10.element(1)->contents(2),"3")) ;

  EXPECT_FALSE(strcmp(e10.element(2)->what(),"MatchNumberRange")) ;
  EXPECT_FALSE(strcmp(e10.element(2)->contents(0),"1999")) ;
  EXPECT_FALSE(strcmp(e10.element(2)->contents(1),"2020")) ;
  EXPECT_FALSE(strcmp(e10.element(2)->contents(2),"4")) ;

  EXPECT_FALSE(strcmp(e10.element(3)->what(),"MatchStarEnd")) ;

  if(LOCALFAIL_COUNT()) G__pause();
}
//////////////////////////////////////////////////////////////////
TEST(EasyExpCompileTest, StarEndAnyChar)
{
  RESET_LOCALFAIL();
  EasyExp e1("");           
  EXPECT_FALSE(e1.backannotation()!="") ;
  EXPECT_FALSE(e1.element_size()!=1) ;
  EXPECT_FALSE(strcmp(e1.element(0)->what(),"MatchEnd")) ;

  if(LOCALFAIL_COUNT()) G__pause();

  EasyExp e2("*");         
  EXPECT_FALSE(e2.backannotation()!="*") ;
  if(LOCALFAIL_COUNT()) G__pause();

  EXPECT_FALSE(e2.element_size()!=1) ;
  EXPECT_FALSE(strcmp(e2.element(0)->what(),"MatchStarEnd")) ;

  if(LOCALFAIL_COUNT()) G__pause();

  EasyExp e3("?");           
  EXPECT_FALSE(e3.backannotation()!="?") ;
  if(LOCALFAIL_COUNT()) G__pause();

  EXPECT_FALSE(e3.element_size()!=2) ;
  EXPECT_FALSE(strcmp(e3.element(0)->what(),"MatchAnyChar")) ;
  EXPECT_FALSE(strcmp(e3.element(1)->what(),"MatchEnd")) ;

  if(LOCALFAIL_COUNT()) G__pause();

  EasyExp e4("pattern?PATTERN");         
  EXPECT_FALSE(e4.backannotation()!="pattern?PATTERN") ;
  if(LOCALFAIL_COUNT()) G__pause();

  EXPECT_FALSE(e4.element_size()!=4) ;
  EXPECT_FALSE(strcmp(e4.element(0)->what(),"MatchString")) ;
  EXPECT_FALSE(strcmp(e4.element(0)->contents(0),"pattern")) ;

  EXPECT_FALSE(strcmp(e4.element(1)->what(),"MatchAnyChar")) ;

  EXPECT_FALSE(strcmp(e4.element(2)->what(),"MatchString")) ;
  EXPECT_FALSE(strcmp(e4.element(2)->contents(0),"PATTERN")) ;

  EXPECT_FALSE(strcmp(e4.element(3)->what(),"MatchEnd")) ;

  if(LOCALFAIL_COUNT()) G__pause();

  EasyExp e5("pattern*");         
  EXPECT_FALSE(e5.backannotation()!="pattern*") ;
  if(LOCALFAIL_COUNT()) G__pause();

  EXPECT_FALSE(e5.element_size()!=2) ;
  EXPECT_FALSE(strcmp(e5.element(0)->what(),"MatchString")) ;
  EXPECT_FALSE(strcmp(e5.element(0)->contents(0),"pattern")) ;

  EXPECT_FALSE(strcmp(e5.element(1)->what(),"MatchStarEnd")) ;

  if(LOCALFAIL_COUNT()) G__pause();

  EasyExp e6("pattern?");          
  EXPECT_FALSE(e6.backannotation()!="pattern?") ;
  if(LOCALFAIL_COUNT()) G__pause();

  EXPECT_FALSE(e6.element_size()!=3) ;
  EXPECT_FALSE(strcmp(e6.element(0)->what(),"MatchString")) ;
  EXPECT_FALSE(strcmp(e6.element(0)->contents(0),"pattern")) ;

  EXPECT_FALSE(strcmp(e6.element(1)->what(),"MatchAnyChar")) ;

  EXPECT_FALSE(strcmp(e6.element(2)->what(),"MatchEnd")) ;

  if(LOCALFAIL_COUNT()) G__pause();

  EasyExp e7("?pattern");         
  EXPECT_FALSE(e7.backannotation()!="?pattern") ;
  if(LOCALFAIL_COUNT()) G__pause();

  EXPECT_FALSE(e7.element_size()!=3) ;
  EXPECT_FALSE(strcmp(e7.element(0)->what(),"MatchAnyChar")) ;

  EXPECT_FALSE(strcmp(e7.element(1)->what(),"MatchString")) ;
  EXPECT_FALSE(strcmp(e7.element(1)->contents(0),"pattern")) ;

  EXPECT_FALSE(strcmp(e7.element(2)->what(),"MatchEnd")) ;

  if(LOCALFAIL_COUNT()) G__pause();
}
//////////////////////////////////////////////////////////////////
#if 0
TEST(EasyExpCompileTest, Dummy)
{
}
#endif
