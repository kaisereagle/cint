2014 Jan 09  Masaharu Goto

#  Eigen library trial
 Files
  - EigenTest1.cxx
  - makeEigenTest1

 Usage: Linux only
   $ make -f makeEigenTest1



#  K measn method, EM (Expectation maximization) method

 Files
  - KmeansEM.h 
     Kmeans & EM method source file

  - LinkDef.h
     Kmeans & EM library ROOT expose information

  - makeroot.linux
     makefile for ROOT EM library 

  - MatrixCalc.h
     Wrapper library for Eigen inverse and determinant calculation

  - Vector.h
     gotom Vector and Matrix library copied from $CINTSYSDIR/lib/NVector

  - KmeansEMtest1.cxx
     test script
  - KmeansEMtest2.cxx
     Improved (structured) test script, same contents as test1
  - KmeansEMtest3.cxx
     Improved (gaussian polar) test script, same contents as test1

  - KmeansEMDict.h/.cxx
     ROOT dictionary for EM library

  - KmeansEM.dll
     EM ROOT shared library

 Usage: 
   $     make -f makeroot.linux
   c:\> nmake -f makeroot.vcnet

   $ root
   root[] .x KmeansEMtest3.cxx

   root[] .x KmeansEMtest3.cxx(2,2,2,2) // sigma = 2
   root[] .x KmeansEMtest3.cxx(2,0.2,2,2) // sigma unballance, bad case

 Programming Issue:
   Eigen library requires Matrix size as template argument. This means
  matrix size has to be predetermined at compilation. Dirty work-around is
  using in MatrixCalc.h. 


# ����
  C:\Tools\PRML\KMeans_EM\KmeansEMtest2.cxx

  C:\AAA\DesignConsideration\MPPT_Data\corgr5.cxx
  root[] corr(cssmu,0,1)


# Kmeans�@�AEM�@�����ƍl�@

  - Kmeans�@
�@  �P�������肵�ē��삷��B�������A�v�f����unballance������Ɩڂł݂�
�@�@���炩���̂ł����ނł��Ȃ����Ƃ�����B

  - EM method �̎�����
   KmeansEMtest1-3.cxx �� 2��������Gauss���z 2���ޖ������������B
    Kmeans�@�͏�Ɋ��Ғʂ�̎���
    EM�@�͎������Ȃ��ꍇ��������
     + bxsigma �� bysigma �̕��U�̔䂪�傫���Ǝ������������Ȃ�
     + Kmeans�@�Ŏ��������Ă��V���Ă��Ȃ��Ǝ������Ȃ�
    
   Program �̕s��Ȃ̂� EM�@���̂��̖̂��Ȃ̂��s���B
�@ PRML ���� 9.2�� p150 �ɋL�ڂ̖�肩���m��Ȃ��B
    "����̗v�f������̃f�[�^�_�Ɏ�������΁A�ΐ��ޓx�𑝉�����������ɓ���"

    �܂肠�鎟���̕��U��0�ɋ߂Â��ƁA���̎����̖ޓx�΂��肪�D���ɂȂ��Ă�
    �܂��A�K�؂ȕ��ނ��ł��Ȃ��Ȃ�B

    �Ƃ���Ɗe�����̕��U�������x��data�����������Ƃ��ł��Ȃ�?
    �ꎞ�I�ɕ��ςɗ������������U��傫�����ĉ�������p150�ɂ͏����Ă���B

      
