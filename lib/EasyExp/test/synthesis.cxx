//////////////////////////////////////////////////////////////////////
// EasyExp synthesis test
//
//  2015/Aug/12  Masaahru Goto
//////////////////////////////////////////////////////////////////////
#pragma include <EasyExp.dll> // Test Target
//#include <CSV.h>       // Utility, Caution:: this library also includes EasyExp. 
//#include <exception>
#include "_util_cinttest.h"

char *emptypattern[] = { 0 };

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// string list
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
char *expect1a = "(apple.|banana.|orange.|potato.)"; 
char *pattern1a[] = {
  "apple.",
  "banana.",
  "orange.",
  "apple.",
  "potato.",
  "apple.",
  0
};
char *_pattern1a[] = { 
  "apples.",
  "bananas.",
  "apples.",
  "bicycle.",
  "",
  0 
};

//////////////////////////////////////////////////////////////////////
char *expect1b = "*"; 
char *pattern1b[] = {
  "apple.",
  "banana.",
  "orange.",
  "apple.",
  "potato.",
  "pineapple.",
  "bycicle.",
  "car.",
  "bus.",
  "train.",
  "apple.",
  0
};
char *_pattern1b[] = {0};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// char list
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
char *expect2a = "?"; 
//char *expect2a = "[abcopt]";  // alternative
char *pattern2a[] = {
  "a", "b", "o", "a", "p", "p", "b", "c", "b", "t", "a",
  0
};
char *_pattern2a[] = {0};

//////////////////////////////////////////////////////////////////////
char *expect2b = "[abco]"; 
char *pattern2b[] = {
  "a", "b", "o", "a", "b", "o", "a", "c", "b", "a", "a",
  0
};
char *_pattern2b[] = {
  "t", "z", "1", "2", 
  0
};

//////////////////////////////////////////////////////////////////////
char *expect2c = "[a-z]"; 
char *pattern2c[] = {
  "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", 
  "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z",
  0
};
char *_pattern2c[] = {
  "A", "Z", "1", "2", 
  0
};

//////////////////////////////////////////////////////////////////////
char *expect2d = "?"; 
char *pattern2d[] = {
  "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", 
  "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z",
  0
};
char *_pattern2d[] = {
  0
};

//////////////////////////////////////////////////////////////////////
char *expect2e = "[0-9a-z]"; 
char *pattern2e[] = {
  "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", 
  "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z",
  "0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
  0
};
char *_pattern2e[] = {
  "A", "Z", 
  0
};

//////////////////////////////////////////////////////////////////////
//char *expect2f = "*"; 
char *expect2f = "[0-9a-z:0-1]"; 
char *pattern2f[] = {
  "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", 
  "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z",
  "0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
  "",
  0
};
char *_pattern2f[] = {
  "A", "Z", 
  0
};

//////////////////////////////////////////////////////////////////////
char *expect2g = "*"; 
//char *expect2g = "[0-9a-z:0-1]"; 
char *pattern2g[] = {
  "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", 
  "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z",
  "0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
  "",
  0
};
char *_pattern2g[] = {
  0
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// Number range
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//char *expect3a = "*";
char *expect3a = "(1995..2015:4)";   // FIXED
char *pattern3a[] = {
  "1997",
  "2012",
  "1999",
  "2015",
  "2014",
  "1995",
  "2012",
  "2001",
  "1999",
  "1996",
  "2001",
  0
};
char *_pattern3a[] = {
  "2016",
  "1994",
  "1993",
  "2994",
  0
};

//////////////////////////////////////////////////////////////////////
char *expect3b = "(1995..2015:4)"; 
char *pattern3b[] = {
  "1997",
  "1997",
  "2015",
  "2014",
  "1995",
  0
};
char *_pattern3b[] = {
  "2016",
  "1994",
  "1993",
  "2994",
  0
};


//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// string + string list
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
char *expect4a = "I like (an apple.|apples.|bananas.|oranges.|potatos.)"; 
char *pattern4a[] = {
  "I like apples.",
  "I like bananas.",
  "I like oranges.",
  "I like an apple.",
  "I like potatos.",
  0
};
char *_pattern4a[] = { 
  "I buy apples.",
  "I sell bananas.",
  "I grow apples.",
  "I like bicycle.",
  "I like ",
  0 
};

//////////////////////////////////////////////////////////////////////
char *expect4b = "I like (an apple.|apples.|bananas.|oranges.|potatos.)"; 
char *pattern4b[] = {
  "I like apples.",
  "I like bananas.",
  "I like oranges.",
  "I like an apple.",
  "I like potatos.",
  "I like apples.",
  0
};
char *_pattern4b[] = { 
  "I buy apples.",
  "I sell bananas.",
  "I grow apples.",
  "I like bicycle.",
  "I like ",
  0 
};
//////////////////////////////////////////////////////////////////////
char *expect4c = "I like (|an apple.|apples.|bananas.|oranges.|potatos.)"; 
char *pattern4c[] = {
  "I like apples.",
  "I like bananas.",
  "I like oranges.",
  "I like an apple.",
  "I like potatos.",
  "I like apples.",
  "I like ",
  0
};
char *_pattern4c[] = { 
  "I buy apples.",
  "I sell bananas.",
  "I grow apples.",
  "I like bicycle.",
  0 
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// string list + string + string list
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
char *expect5a = "* like (an apple.|apples.|bananas.|oranges.|potatos.)"; 
//char *expect5a = "(I|We|You) like (an apple.|apples.|bananas.|oranges.|potatos.)"; 
char *pattern5a[] = {
  "We like apples.",
  "I like bananas.",
  "You like oranges.",
  "I like oranges.",
  "I like an apple.",
  "I like potatos.",
  0
};
char *_pattern5a[] = { 
  "I buy apples.",
  "I sell bananas.",
  "I grow apples.",
  //"We like potatos.",  naive bayes limitation, 
  "I like bicycle.",
  "I like ",
  0 
};

//////////////////////////////////////////////////////////////////////
char *expect5b = "* like (apple.|banana.|orange.|potato.)"; 
//char *expect5b = "(I|We|You) like (apple.|banana.|orange.|potato.)"; 
char *pattern5b[] = {
  "You like apple.",
  "I like banana.",
  "We like orange.",
  "I like apple.",
  "I like potato.",
  "I like apple.",
  "We like apple.",
  //"I like ",
  0
};
char *_pattern5b[] = { 
  "I buy apples.",
  "I sell bananas.",
  "I grow apples.",
  "I like bicycle.",
  "I like ",
  0 
};

//////////////////////////////////////////////////////////////////////
char *expect5c = "* like (|apple.|banana.|orange.|potato.)"; 
//char *expect5c = "(|I|We|You) like (|apple.|banana.|orange.|potato.)"; 
char *pattern5c[] = {
  "You like apple.",
  "I like banana.",
  "We like orange.",
  "I like apple.",
  "I like potato.",
  "I like apple.",
  "We like apple.",
  " like ",
  0
};
char *_pattern5c[] = { 
  "I buy apples.",
  "I sell bananas.",
  "I grow apples.",
  "I like bicycle.",
  //"I like ",
  0 
};

//////////////////////////////////////////////////////////////////////
char *expect5d = "* like( apple.| banana.| orange.| potato.|s potato.)"; 
//char *expect5d = "(He|I|We|You) like( apple.| banana.| orange.| potato.|s potato.)"; 
//char *expect5d = "(He|I|We|You) like[s:0-1] (apple.|banana.|orange.|potato.)"; // IMPROVEMENT DESIRABLE BUT NOT IMPORTANT
char *pattern5d[] = {
  "You like apple.",
  "I like banana.",
  "We like orange.",
  "I like apple.",
  "He likes potato.",
  "I like apple.",
  "We like apple.",
  "We like potato.",
  0
};
char *_pattern5d[] = { 
  "I buy apples.",
  "I sell bananas.",
  "I grow apples.",
  "I like bicycle.",
  //"I like ",
  0 
};

//////////////////////////////////////////////////////////////////////
// combination test
//////////////////////////////////////////////////////////////////////
char *expect6a = "condition_[ABC]_20150(801..930:3).txt"; 
char *pattern6a[] = {
  "condition_C_20150915.txt",
  "condition_B_20150930.txt",
  "condition_A_20150801.txt",
  "condition_C_20150801.txt",
  "condition_C_20150902.txt",
  "condition_A_20150801.txt",
  0
};
char *_pattern6a[] = { 
  "condition_A_20140801.txt",
  "condition_A_20150731.txt",
  "condition_D_20150801.txt",
  0 
};

//////////////////////////////////////////////////////////////////////
char *expect6b = "condition_?_20(000914..150930:6).txt"; 
//char *expect6b = "condition_[A-F]_20(000914..150930:6).txt"; 
//char *expect6b = "condition_[ABCDEF]_20(000914..150930:6).txt"; 
char *pattern6b[] = {
  "condition_C_20000914.txt",
  "condition_B_20150930.txt",
  "condition_A_20150801.txt",
  "condition_C_20150801.txt",
  "condition_F_20150902.txt",
  "condition_A_20150801.txt",
  "condition_D_20150801.txt",
  "condition_E_20150801.txt",
  "condition_E_20150801.txt",
  0
};
char *_pattern6b[] = { 
  "condition_A_20160801.txt",
  "condition_C_20150931.txt",
  "condition_D_19990801.txt",
  0 
};

//////////////////////////////////////////////////////////////////////
char *expect6c = "condition_?_20(000915..150930:6).txt";  // IMPROVED
//char *expect6c = "condition_[A-F]_20(000915..150930:6).txt";  // IMPROVED
//char *expect6c = "condition_[A-F]_20(|0009)15(|0801|0902|0930).txt"; 
char *pattern6c[] = {
  "condition_C_20000915.txt",
  "condition_B_20150930.txt",
  "condition_A_20150801.txt",
  "condition_C_20150801.txt",
  "condition_F_20150902.txt",
  "condition_A_20150801.txt",
  "condition_D_20150801.txt",
  "condition_E_20150801.txt",
  "condition_E_20150801.txt",
  0
};
char *_pattern6c[] = { 
  "condition_A_20160801.txt",
  "condition_C_20150931.txt",
  "condition_D_19990801.txt",
  0 
};

//////////////////////////////////////////////////////////////////////
//char *expect6d = "condition_?_20*15*.txt"; 
char *expect6d = "condition_?_20*.txt";  // IMPROVED
char *pattern6d[] = {
  "condition_C_20000915.txt",
  "condition_B_20150930.txt",
  "condition_A_20150801.txt",
  "condition_C_20150801.txt",
  "condition_F_20150902.txt",
  "condition_A_20150801.txt",
  "condition_D_20150801.txt",
  "condition_E_20150801.txt",
  "condition_E_20150801.txt",
  0
};
char *_pattern6d[] = { 
  0 
};

//////////////////////////////////////////////////////////////////////
char *expect6e = "condition_[A-F]_20*.txt";  // IMPROVED
char *pattern6e[] = {
  "condition_C_20000915.txt",
  "condition_B_20150930.txt",
  "condition_A_20150801.txt",
  "condition_C_20150801.txt",
  "condition_F_20150902.txt",
  "condition_A_20150801.txt",
  "condition_D_20150801.txt",
  "condition_E_20150801.txt",
  "condition_E_20150801.txt",
  0
};
char *_pattern6e[] = { 
  "condition_X_20150801.txt",
  "condition_X_20150931.txt",
  "condition_Z_19990801.txt",
  0 
};

//////////////////////////////////////////////////////////////////////
char *expect6f = "condition_[A-F]_20(000915..150930:6).txt";  // IMPROVED
char *pattern6f[] = {
  "condition_C_20000915.txt",
  "condition_B_20150930.txt",
  "condition_A_20150801.txt",
  "condition_C_20150801.txt",
  "condition_F_20150902.txt",
  "condition_A_20150801.txt",
  "condition_D_20150801.txt",
  "condition_E_20150801.txt",
  "condition_E_20150801.txt",
  0
};
char *_pattern6f[] = { 
  "condition_C_20150801.txt",
  "condition_A_20150930.txt",
  "condition_B_20000915.txt",
  0 
};

//////////////////////////////////////////////////////////////////////
// mode and list limit
//////////////////////////////////////////////////////////////////////
//char *expect7a = "[A-F]_com_(1999..2015|4)_com_(apple|orange|banana|pear|grape).txt"; 
char *expect7a = "?_com_*_com_*.txt";  
char *pattern7a[] = {
  "C_com_1999_com_apple.txt",
  "B_com_2012_com_ornage.txt",
  "A_com_2005_com_banana.txt",
  "C_com_2011_com_pear.txt",
  "F_com_2001_com_pear.txt",
  "A_com_2009_com_grape.txt",
  "D_com_2012_com_apple.txt",
  "E_com_2005_com_orange.txt",
  "E_com_2015_com_apple.txt",
  0
};
char *_pattern7a[] = { 
  0 
};
int mode7a=0;
int limit7a=5;

//////////////////////////////////////////////////////////////////////
char *expect7b = "[A-F]_com_(1999..2015|4)_com_(apple|orange|banana|pear|grape).txt"; 
//char *expect7b = "?_com_*_com_*.txt";  
char *pattern7b[] = {
  "C_com_1999_com_apple.txt",
  "B_com_2012_com_ornage.txt",
  "A_com_2005_com_banana.txt",
  "C_com_2011_com_pear.txt",
  "F_com_2001_com_pear.txt",
  "A_com_2009_com_grape.txt",
  "D_com_2012_com_apple.txt",
  "E_com_2005_com_orange.txt",
  "E_com_2015_com_apple.txt",
  0
};
char *_pattern7b[] = { 
  0 
};
int mode7b=1;
int limit7b=5;

//////////////////////////////////////////////////////////////////////
char *expect7c = "?_com_*_com_*.txt"; 
//char *expect7c = "[A-F]_com_(1999..2015:4)_com_*.txt"; 
char *pattern7c[] = {
  "C_com_1999_com_apple.txt",
  "B_com_2012_com_ornage.txt",
  "A_com_2005_com_banana.txt",
  "C_com_2011_com_pear.txt",
  "F_com_2001_com_pear.txt",
  "A_com_2009_com_grape.txt",
  "D_com_2012_com_apple.txt",
  "E_com_2005_com_orange.txt",
  "E_com_2015_com_apple.txt",
  0
};
char *_pattern7c[] = { 
  0 
};
int mode7c=1;
int limit7c=4;

//////////////////////////////////////////////////////////////////////
//char *expect8a = "./DesignConsideration/000_*.csv(|_C)[35:0-1](|.csv)";
string expect8a1= "./DesignConsideration/000_(ExpSynth/RuleApp|ExpSynth/UI|MATS_Experiment/t2|Measurement/EUT4/_tmp__NNclassification|Measurement/EUT5C5_EUT1-5|Measurement/NNDB/NNInput_EUT1_XX|Measurement/NNDB/NNInput_XX|Measurement/_NNclassification|Measurement/_tmp__NNclassification|XAAT_SelfAdjust/TroubleBoardList201312_5chXMU_Repair|XAAT_SelfAdjust/all_data/X7000-61023-00679403215828|XAAT_SelfAdjust/testitem_grouping|";
string expect8a=expect8a1+"XAAT_SelfAdjust_test3/01_aligned/X7000-61023-00779427234074_ch20|XAAT_SelfAdjust_test3/01_split/X7000-61023-00679403215679_ch26|XAAT_SelfAdjust_test3/NNinput/X7000-61023-00679403215679_ch29|XAAT_SelfAdjust_test3/TroubleBoardList201312_5chXMU_Repair|XAAT_SelfAdjust_test3/all_data/X7000-61023-00174|XAAT_SelfAdjust_test3/testitemstat|XAAT_TestData/PTReport_DB_JP5202|XAAT_TestData/XAAT_PT_grouping|XAAT_TestData_test3/01_aligned/X7001A-JP52020208_1|XAAT_TestData_test3/01_split/X7001A-JP52020208_9|XAAT_TestData_test3/NNinput/X7001A-JP52020200_1|XAAT_TestData_test3/PTReport_DB_JP5202T|XAAT_TestData_test3/XAAT_PT_groupingT|XAAT_TestData_test3/testitemstat).csv(|_C)*(|.csv)";
//string expect8a=expect8a1+"XAAT_SelfAdjust_test3/01_aligned/X7000-61023-00779427234074_ch20|XAAT_SelfAdjust_test3/01_split/X7000-61023-00679403215679_ch26|XAAT_SelfAdjust_test3/NNinput/X7000-61023-00679403215679_ch29|XAAT_SelfAdjust_test3/TroubleBoardList201312_5chXMU_Repair|XAAT_SelfAdjust_test3/all_data/X7000-61023-00174|XAAT_SelfAdjust_test3/testitemstat|XAAT_TestData/PTReport_DB_JP5202|XAAT_TestData/XAAT_PT_grouping|XAAT_TestData_test3/01_aligned/X7001A-JP52020208_1|XAAT_TestData_test3/01_split/X7001A-JP52020208_9|XAAT_TestData_test3/NNinput/X7001A-JP52020200_1|XAAT_TestData_test3/PTReport_DB_JP5202T|XAAT_TestData_test3/XAAT_PT_groupingT|XAAT_TestData_test3/testitemstat).csv(|_C)[35:0-1](|.csv)";
char *pattern8a[] = {
  "./DesignConsideration/000_Measurement/NNDB/NNInput_EUT1_XX.csv_C5.csv",
  "./DesignConsideration/000_Measurement/NNDB/NNInput_XX.csv_C3.csv",
  "./DesignConsideration/000_MATS_Experiment/t2.csv",
  "./DesignConsideration/000_XAAT_SelfAdjust/TroubleBoardList201312_5chXMU_Repair.csv",
  "./DesignConsideration/000_XAAT_SelfAdjust/all_data/X7000-61023-00679403215828.csv",
  "./DesignConsideration/000_XAAT_SelfAdjust/testitem_grouping.csv",
  "./DesignConsideration/000_XAAT_SelfAdjust_test3/01_aligned/X7000-61023-00779427234074_ch20.csv",
  "./DesignConsideration/000_XAAT_SelfAdjust_test3/01_split/X7000-61023-00679403215679_ch26.csv",
  "./DesignConsideration/000_XAAT_SelfAdjust_test3/NNinput/X7000-61023-00679403215679_ch29.csv",
  "./DesignConsideration/000_XAAT_SelfAdjust_test3/TroubleBoardList201312_5chXMU_Repair.csv",
  "./DesignConsideration/000_XAAT_SelfAdjust_test3/all_data/X7000-61023-00174.csv",
  "./DesignConsideration/000_XAAT_SelfAdjust_test3/testitemstat.csv",
  "./DesignConsideration/000_XAAT_TestData/XAAT_PT_grouping.csv",
  "./DesignConsideration/000_XAAT_TestData/PTReport_DB_JP5202.csv",
  "./DesignConsideration/000_XAAT_TestData_test3/01_aligned/X7001A-JP52020208_1.csv",
  "./DesignConsideration/000_XAAT_TestData_test3/01_split/X7001A-JP52020208_9.csv",
  "./DesignConsideration/000_XAAT_TestData_test3/XAAT_PT_groupingT.csv",
  "./DesignConsideration/000_XAAT_TestData_test3/NNinput/X7001A-JP52020200_1.csv",
  "./DesignConsideration/000_XAAT_TestData_test3/PTReport_DB_JP5202T.csv",
  "./DesignConsideration/000_XAAT_TestData_test3/testitemstat.csv",
  "./DesignConsideration/000_Measurement/EUT4/_tmp__NNclassification.csv",
  "./DesignConsideration/000_Measurement/EUT5C5_EUT1-5.csv",
  "./DesignConsideration/000_Measurement/_NNclassification.csv",
  "./DesignConsideration/000_Measurement/_tmp__NNclassification.csv",
  "./DesignConsideration/000_ExpSynth/RuleApp.csv",
  "./DesignConsideration/000_ExpSynth/UI.csv",
  0
};
char *_pattern8a[] = { 
  "./DesignConsideration/000_*.csv_C7.csv",
  "./DesignConsideration/000_.slefijasasijalijfsefj.csv",
  0 
};
int generality_mode8a=0;
int limit8a=30;
int position_mode8a=0;

//////////////////////////////////////////////////////////////////////
//char *expect8b = "./DesignConsideration/000_*.csv";
char *expect8b = "./DesignConsideration/000_*.csv(|_C)*(|.csv)";
char *pattern8b[] = {
  "./DesignConsideration/000_Measurement/NNDB/NNInput_EUT1_XX.csv_C5.csv",
  "./DesignConsideration/000_Measurement/NNDB/NNInput_XX.csv_C3.csv",
  "./DesignConsideration/000_MATS_Experiment/t2.csv",
  "./DesignConsideration/000_XAAT_SelfAdjust/TroubleBoardList201312_5chXMU_Repair.csv",
  "./DesignConsideration/000_XAAT_SelfAdjust/all_data/X7000-61023-00679403215828.csv",
  "./DesignConsideration/000_XAAT_SelfAdjust/testitem_grouping.csv",
  "./DesignConsideration/000_XAAT_SelfAdjust_test3/01_aligned/X7000-61023-00779427234074_ch20.csv",
  "./DesignConsideration/000_XAAT_SelfAdjust_test3/01_split/X7000-61023-00679403215679_ch26.csv",
  "./DesignConsideration/000_XAAT_SelfAdjust_test3/NNinput/X7000-61023-00679403215679_ch29.csv",
  "./DesignConsideration/000_XAAT_SelfAdjust_test3/TroubleBoardList201312_5chXMU_Repair.csv",
  "./DesignConsideration/000_XAAT_SelfAdjust_test3/all_data/X7000-61023-00174.csv",
  "./DesignConsideration/000_XAAT_SelfAdjust_test3/testitemstat.csv",
  "./DesignConsideration/000_XAAT_TestData/XAAT_PT_grouping.csv",
  "./DesignConsideration/000_XAAT_TestData/PTReport_DB_JP5202.csv",
  "./DesignConsideration/000_XAAT_TestData_test3/01_aligned/X7001A-JP52020208_1.csv",
  "./DesignConsideration/000_XAAT_TestData_test3/01_split/X7001A-JP52020208_9.csv",
  "./DesignConsideration/000_XAAT_TestData_test3/XAAT_PT_groupingT.csv",
  "./DesignConsideration/000_XAAT_TestData_test3/NNinput/X7001A-JP52020200_1.csv",
  "./DesignConsideration/000_XAAT_TestData_test3/PTReport_DB_JP5202T.csv",
  "./DesignConsideration/000_XAAT_TestData_test3/testitemstat.csv",
  "./DesignConsideration/000_Measurement/EUT4/_tmp__NNclassification.csv",
  "./DesignConsideration/000_Measurement/EUT5C5_EUT1-5.csv",
  "./DesignConsideration/000_Measurement/_NNclassification.csv",
  "./DesignConsideration/000_Measurement/_tmp__NNclassification.csv",
  "./DesignConsideration/000_ExpSynth/RuleApp.csv",
  "./DesignConsideration/000_ExpSynth/UI.csv",
  0
};
char *_pattern8b[] = { 
  0 
};
int generality_mode8b=0;
int limit8b=30;
int position_mode8b=0;

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

int pass=0;
//////////////////////////////////////////////////////////////////////
// test engine
//////////////////////////////////////////////////////////////////////
int test(const string& name,const string& expect,char* pattern[],char* _pattern[]
	 ,int generality_mode=0,int ListLimit=5,int position_mode=0) {
  int err=0,i;

  // prepare learning pattern
  vector<string> pat,_pat;
  int i=0; while(pattern[i])  pat.push_back(pattern[i++]);
  i=0; while(_pattern[i])  _pat.push_back(_pattern[i++]);
  
  // easyexp synthesis
  string synth = EasyExpSynthesis(pat,_pat,generality_mode,ListLimit,position_mode);

#ifdef DEBUG
  printf("# synth='%s' expct='%s'\n",synth.c_str(),expect.c_str());
#endif

  // expectation
  EXPECT_EQ(expect,synth);
  if(expect!="" && expect!=synth) {
    printf("%s Expression mismatch\n",name.c_str());
    ++err;
  }
  else ++pass;

  // pattern match validation
  EasyExp e(synth.c_str());
  vector<string> ma  = e.match(pat);
  vector<string> _ma  = e._match(pat);
  EXPECT_EQ(ma.size(),pat.size()) ;
  if(ma.size()!=pat.size()) {
    printf("%s superviser samples = %d , matched samples = %d\n",name.c_str(),pat.size(),ma.size());
    for(i=0;i<ma.size();i++) printf("m '%s'\n",ma[i].c_str());
    for(i=0;i<_ma.size();i++) printf("u '%s'\n",_ma[i].c_str());
    ++err;
  }
  else ++pass;

  // pattern unmatch validation
  vector<string> um  = e.match(_pat);
  EXPECT_EQ(um.size(),0) ;
  if(um.size()>0) {
    printf("%s anti-superviser samples = %d , matched samples = %d\n",name.c_str(),_pat.size(),um.size());
    for(i=0;i<um.size();i++) printf(" '%s'\n",um[i].c_str());
    ++err;
  }
  else ++pass;

  if(err) {
    printf(" synth='");
    puts(synth.c_str());
    printf("'\n expct='");
    puts(expect.c_str());
    printf("'\n");
  }
  return(err);
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
int main() {
  int err=0;
  printf("### EasyExp Synthesis test\n");

  printf("test1a-b\n");
  err+=test("test1a",expect1a,pattern1a,_pattern1a);
  err+=test("test1b",expect1b,pattern1b,_pattern1b);

  printf("test2a-g\n");
  err+=test("test2a",expect2a,pattern2a,_pattern2a);
  err+=test("test2b",expect2b,pattern2b,_pattern2b);
  err+=test("test2c",expect2c,pattern2c,_pattern2c);
  err+=test("test2d",expect2d,pattern2d,_pattern2d);
  err+=test("test2e",expect2e,pattern2e,_pattern2e);
  err+=test("test2f",expect2f,pattern2f,_pattern2f);
  err+=test("test2g",expect2g,pattern2g,_pattern2g);

  printf("test3a-b\n");
  err+=test("test3a",expect3a,pattern3a,_pattern3a);
  err+=test("test3b",expect3b,pattern3b,_pattern3b);

  printf("test4a-c\n");
  err+=test("test4a",expect4a,pattern4a,_pattern4a);
  err+=test("test4b",expect4b,pattern4b,_pattern4b);
  err+=test("test4c",expect4c,pattern4c,_pattern4c);

  printf("test5a-d\n");
  err+=test("test5a",expect5a,pattern5a,_pattern5a);
  err+=test("test5b",expect5b,pattern5b,_pattern5b);
  err+=test("test5c",expect5c,pattern5c,_pattern5c);
  err+=test("test5d",expect5d,pattern5d,_pattern5d);

  printf("test6a-f\n");
  err+=test("test6a",expect6a,pattern6a,_pattern6a);
  err+=test("test6b",expect6b,pattern6b,_pattern6b);
  err+=test("test6c",expect6c,pattern6c,_pattern6c);
  err+=test("test6d",expect6d,pattern6d,_pattern6d);
  err+=test("test6e",expect6e,pattern6e,_pattern6e);
  err+=test("test6f",expect6f,pattern6f,_pattern6f);
  printf("Known issue: 6f -> condition_(C_20000915|B_20150930|...).txt\n");

  printf("test7a-c\n");
  err+=test("test7a",expect7a,pattern7a,_pattern7a,mode7a,limit7a);
  err+=test("test7b",expect7a,pattern7a,_pattern7a,mode7a,limit7a);
  err+=test("test7c",expect7c,pattern7c,_pattern7c,mode7c,limit7c);

  printf("test8a-c\n");
  err+=test("test8a",expect8a.c_str(),pattern8a,_pattern8a,generality_mode8a,limit8a,position_mode8a);
  err+=test("test8b",expect8b,pattern8b,_pattern8b,generality_mode8b,limit8b,position_mode8b);

  printf("EasyExp Synthesis test result pass=%d  fail=%d\n",pass,err);
  SAVE_PASSFAIL("synthesis");
  exit(err);
}

//////////////////////////////////////////////////////////////////////


