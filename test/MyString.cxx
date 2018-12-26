// MyString.cxx

#include <string.h>
#include <stdio.h>
#include "MyString.h"

// �f�t�H���g�E�R���X�g���N�^ ///////////////////////////////////////
MyString::MyString() 
{
  len=0;
  pstr=0;
}

// �R�s�[�E�R���X�g���N�^ ///////////////////////////////////////////
MyString::MyString(const MyString& strin) 
{
  len=0;
  pstr=0;
  if(0!=strin.len) {
    len = strin.len;
    pstr = new char[len+1];
    strcpy(pstr,strin.pstr);
  }
}

// char* �� MyString �ɕϊ�����R���X�g���N�^ ///////////////////////
MyString::MyString(const char* charin)
{
  len=0;
  pstr=0;
  if(0!=charin && 0!=charin[0]) {
    len = strlen(charin);
    pstr = new char[len+1];
    strcpy(pstr,charin);
  }
}

// ���l�� MyString �ɕϊ�����R���X�g���N�^ /////////////////////////
MyString::MyString(const double din)
{
  char buf[50];
  len=0;
  pstr=0;
  sprintf(buf,"%g",din);
#if 1
  *(this) = buf; // TODO,  segv
#else
  len = strlen(buf);
  pstr=new char[len+1];
  strcpy(pstr,buf);
#endif
}

// �f�X�g���N�^ /////////////////////////////////////////////////////
MyString::~MyString()
{
  if(pstr) delete[] pstr;
}

// ������Z�q //////////////////////////////////////////////////////
MyString& MyString::operator=(const MyString& strin) 
{
  // �ȑO�̕�������e������
  if(pstr) delete[] pstr;
  pstr=0;
  len=0;

  // �E�ӂ̕��������
  if(strin.len) {
    len = strin.len;
    pstr = new char[len+1];
    //fprintf(stderr,"a %s %p\n",pstr,&strin);
    strcpy(pstr,strin.pstr);
  }
  return(*this);
}

// �����o�֐��ɂ�镶����̘A�� ///////////////////////////////////
MyString& MyString::operator+=(const MyString& strin) 
{
  if(0==strin.pstr) return(*this);
  if(0==pstr) {
    len = strin.len;
    delete[] pstr;
    pstr = new char[len+1];
    strcpy(pstr,strin.pstr);
  }
  else {
    len += strin.len;
    char *p = new char[len+1];
    strcpy(p,pstr);
    strcat(p,strin.pstr);
    delete[] pstr;
    pstr = p;
  }
  return(*this);
}

// �t�����h�֐��ɂ��A�� /////////////////////////////////////////
MyString operator+(const MyString& str1,const MyString& str2)
{
  if(0==str1.pstr) return(str2);
  if(0==str2.pstr) return(str1);

  MyString str3(str1);
  str3 += str2;
  return(str3);
}

// ���l���艉�Z�q /////////////////////////////////////////////////
bool operator==(const MyString& str1,const MyString& str2)
{
  if( (0==str1.pstr && 0==str2.pstr) ||
      (str1.len==str2.len && 0==strcmp(str1.pstr,str2.pstr) ) ) {
    return(true);
  }
  else {
    return(false);
  }
}

// �o�̓X�g���[�����Z�q //////////////////////////////////////////
ostream& operator<<(ostream& ost,const MyString& str)
{
  if(str.pstr) ost << str.pstr ;
  return(ost);
}

// �����v�f�擾 /////////////////////////////////////////////////
char MyString::operator[](int index) const
{
  if(index<0||len<index) return('\0');
  else                   return(pstr[index]);
}

