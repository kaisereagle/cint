// MultiByteChar test

#ifndef __CINT__
#include "../../ReadF.h"
#include <gtest/gtest.h>
#include <ctype.h>
#else
#include <_util_cinttest.h>
#endif

//////////////////////////////////////////////////////////////////////////
TEST(MultiByteCharTest, DefaultLang)
{
  G__MultiByteMode a;
#ifdef __APPLE__
  EXPECT_EQ(a.GetCodingSystem(),G__MultiByteMode::E__UNICODE);
#else
  EXPECT_EQ(a.GetCodingSystem(),G__MultiByteMode::E__UNKNOWNCODING);
#endif
  
  a.setDefaultLang(G__MultiByteMode::E__EUC); 
  a.init();
  EXPECT_EQ(a.GetCodingSystem(),G__MultiByteMode::E__EUC);
  
  a.setDefaultLang(G__MultiByteMode::E__SJIS); 
  a.init();
  EXPECT_EQ(a.GetCodingSystem(),G__MultiByteMode::E__SJIS);
  
  a.setUNICODE(); 
  a.init();
  EXPECT_EQ(a.GetCodingSystem(),G__MultiByteMode::E__UNICODE);

  a.setSJIS(); 
  a.init();
  EXPECT_EQ(a.GetCodingSystem(),G__MultiByteMode::E__SJIS);

  a.setEUC(); 
  a.init();
  EXPECT_EQ(a.GetCodingSystem(),G__MultiByteMode::E__EUC);

  a.setUNKNOWN(); 
  a.init();
  EXPECT_EQ(a.GetCodingSystem(),G__MultiByteMode::E__UNKNOWNCODING);

  a.setUNICODE(); 
  a.SetCodingSystem();
  EXPECT_EQ(a.GetCodingSystem(),G__MultiByteMode::E__UNICODE);

  a.setSJIS();
  a.SetCodingSystem();
  EXPECT_EQ(a.GetCodingSystem(),G__MultiByteMode::E__SJIS);

  a.setEUC(); 
  a.SetCodingSystem();
  EXPECT_EQ(a.GetCodingSystem(),G__MultiByteMode::E__EUC);

  a.setUNKNOWN(); 
  a.SetCodingSystem();
  EXPECT_EQ(a.GetCodingSystem(),G__MultiByteMode::E__UNKNOWNCODING);

  a.SetCodingSystem(G__MultiByteMode::E__EUC);
  EXPECT_EQ(a.GetCodingSystem(),G__MultiByteMode::E__EUC);

  a.SetCodingSystem(G__MultiByteMode::E__SJIS);
  EXPECT_EQ(a.GetCodingSystem(),G__MultiByteMode::E__SJIS);

  a.SetCodingSystem(G__MultiByteMode::E__UNICODE);
  EXPECT_EQ(a.GetCodingSystem(),G__MultiByteMode::E__UNICODE);

  a.SetCodingSystem(G__MultiByteMode::E__UNKNOWNCODING);
  EXPECT_EQ(a.GetCodingSystem(),G__MultiByteMode::E__UNKNOWNCODING);

}

//////////////////////////////////////////////////////////////////////////
TEST(MultiByteCharTest, IsDBCSLeadByte)
{
  G__MultiByteMode a;
  
  a.setEUC(); a.SetCodingSystem();
  EXPECT_EQ(a.IsDBCSLeadByte('a'),0);
  EXPECT_EQ(a.IsDBCSLeadByte(0x80),0);

  a.setSJIS(); a.SetCodingSystem();
  EXPECT_EQ(a.IsDBCSLeadByte('a'),0);
  EXPECT_EQ(a.IsDBCSLeadByte(0x80),1);

  a.setUNICODE(); a.SetCodingSystem();
  EXPECT_EQ(a.IsDBCSLeadByte('a'),0);
  EXPECT_EQ(a.IsDBCSLeadByte(0x80),0);
}

//////////////////////////////////////////////////////////////////////////
TEST(MultiByteCharTest, CheckDBCS2ndByte)
{
  G__MultiByteMode a;
  
  a.setEUC(); 
  a.SetCodingSystem();
  EXPECT_EQ(a.GetCodingSystem(),G__MultiByteMode::E__EUC);

  a.CheckDBCS2ndByte(0x80);
  EXPECT_EQ(a.GetCodingSystem(),G__MultiByteMode::E__EUC);

  a.CheckDBCS2ndByte('a');
  EXPECT_EQ(a.GetCodingSystem(),G__MultiByteMode::E__SJIS);

  a.setUNICODE(); 
  a.SetCodingSystem();
  EXPECT_EQ(a.GetCodingSystem(),G__MultiByteMode::E__UNICODE);

  a.CheckDBCS2ndByte(0x80);
  EXPECT_EQ(a.GetCodingSystem(),G__MultiByteMode::E__UNICODE);

  a.CheckDBCS2ndByte('a');
  EXPECT_EQ(a.GetCodingSystem(),G__MultiByteMode::E__SJIS);

  a.setSJIS(); 
  a.SetCodingSystem();
  EXPECT_EQ(a.GetCodingSystem(),G__MultiByteMode::E__SJIS);

  a.CheckDBCS2ndByte(0x80);
  EXPECT_EQ(a.GetCodingSystem(),G__MultiByteMode::E__SJIS);

  a.CheckDBCS2ndByte('a');
  EXPECT_EQ(a.GetCodingSystem(),G__MultiByteMode::E__SJIS);
}

//////////////////////////////////////////////////////////////////////////
TEST(MultiByteCharTest, CodingSystem)
{
  G__MultiByteMode a;
  
  a.SetCodingSystem(G__MultiByteMode::E__UNKNOWNCODING);
  EXPECT_EQ(a.CodingSystem(0x20),0);
  EXPECT_EQ(a.GetCodingSystem(),G__MultiByteMode::E__EUC);

  a.SetCodingSystem(G__MultiByteMode::E__UNKNOWNCODING);
  EXPECT_EQ(a.CodingSystem(0x5f),0);
  EXPECT_EQ(a.GetCodingSystem(),G__MultiByteMode::E__EUC);

  a.SetCodingSystem(G__MultiByteMode::E__UNKNOWNCODING);
  EXPECT_EQ(a.CodingSystem(0x1f),1);
  EXPECT_EQ(a.GetCodingSystem(),G__MultiByteMode::E__UNKNOWNCODING);

  a.SetCodingSystem(G__MultiByteMode::E__UNKNOWNCODING);
  EXPECT_EQ(a.CodingSystem(0x60),1);
  EXPECT_EQ(a.GetCodingSystem(),G__MultiByteMode::E__UNKNOWNCODING);

  a.SetCodingSystem(G__MultiByteMode::E__EUC);
  EXPECT_EQ(a.CodingSystem(0x1f),0);
  EXPECT_EQ(a.GetCodingSystem(),G__MultiByteMode::E__EUC);
  EXPECT_EQ(a.CodingSystem(0x20),0);
  EXPECT_EQ(a.GetCodingSystem(),G__MultiByteMode::E__EUC);
  EXPECT_EQ(a.CodingSystem(0x5f),0);
  EXPECT_EQ(a.GetCodingSystem(),G__MultiByteMode::E__EUC);
  EXPECT_EQ(a.CodingSystem(0x60),0);
  EXPECT_EQ(a.GetCodingSystem(),G__MultiByteMode::E__EUC);

  a.SetCodingSystem(G__MultiByteMode::E__UNICODE);
  EXPECT_EQ(a.CodingSystem(0x1f),0);
  EXPECT_EQ(a.GetCodingSystem(),G__MultiByteMode::E__UNICODE);
  EXPECT_EQ(a.CodingSystem(0x20),0);
  EXPECT_EQ(a.GetCodingSystem(),G__MultiByteMode::E__UNICODE);
  EXPECT_EQ(a.CodingSystem(0x5f),0);
  EXPECT_EQ(a.GetCodingSystem(),G__MultiByteMode::E__UNICODE);
  EXPECT_EQ(a.CodingSystem(0x60),0);
  EXPECT_EQ(a.GetCodingSystem(),G__MultiByteMode::E__UNICODE);

  a.SetCodingSystem(G__MultiByteMode::E__SJIS);
  EXPECT_EQ(a.CodingSystem(0x1f),1);
  EXPECT_EQ(a.GetCodingSystem(),G__MultiByteMode::E__SJIS);
  EXPECT_EQ(a.CodingSystem(0x5f),0);
  EXPECT_EQ(a.GetCodingSystem(),G__MultiByteMode::E__SJIS);
  EXPECT_EQ(a.CodingSystem(0x60),1);
  EXPECT_EQ(a.GetCodingSystem(),G__MultiByteMode::E__SJIS);
  EXPECT_EQ(a.CodingSystem(0x7c),1);
  EXPECT_EQ(a.GetCodingSystem(),G__MultiByteMode::E__SJIS);
  EXPECT_EQ(a.CodingSystem(0x7d),0);
  EXPECT_EQ(a.GetCodingSystem(),G__MultiByteMode::E__SJIS);

}

//////////////////////////////////////////////////////////////////////////
TEST(MultiByteCharTest, strchrMutiByte)
{
#if 0
  char buf[50] = "ìcb";
  G__MultiByteMode a;
  EXPECT_EQ(a.strchr_multibyte(buf,'b'),buf+2);

  char buf[50] = "å„ì° ê≥é°";
  G__MultiByteMode a;
  EXPECT_EQ(a.strchr_multibyte(buf,' '),buf+5);
#endif
  
}

//////////////////////////////////////////////////////////////////////////


