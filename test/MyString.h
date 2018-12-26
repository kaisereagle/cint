// MyString.h

#ifndef MYSTRING_H
#define MYSTRING_H

#ifdef __hpux
#include <iostream.h>
#else
#include <iostream>
using namespace std;
#endif

// ������N���X ///////////////////////////////////////////////////////
class MyString {
 public:
  // ������
  MyString();                      // �f�t�H���g�E�R���X�g���N�^
  MyString(const MyString& strin); // �R�s�[�E�R���X�g���N�^
  MyString(const char* charin);    // char* �� MyString �ɕϊ�
  MyString(const double din);      // ���l�� MyString �ɕϊ�

  // �I������
  ~MyString(); // �f�X�g���N�^

  // �����o�֐��ɂ�鉉�Z�q���d��`
  MyString& operator=(const MyString& strin); //������Z�q
  MyString& operator+=(const MyString& str1); //������̘A��

  // �t�����h�֐��ɂ�鉉�Z�q���d��`
  friend bool operator==(const MyString& str1,const MyString& str2);
  friend bool operator!=(const MyString& str1,const MyString& str2)
    { return(!(str1==str2)); }
  friend MyString operator+(const MyString& str1,const MyString& str2);
  friend ostream& operator<<(ostream& ost,const MyString& str);

  // ���擾
  char operator[](int index) const;     //�����v�f�擾
  char* c_str() const { return(pstr); } // char* �Ƃ��ĕ������Ԃ�
  int length() const { return(len); }   // ������

 private:
  char *pstr;
  int len;
};

#endif
