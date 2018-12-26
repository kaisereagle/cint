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


# 実験
  C:\Tools\PRML\KMeans_EM\KmeansEMtest2.cxx

  C:\AAA\DesignConsideration\MPPT_Data\corgr5.cxx
  root[] corr(cssmu,0,1)


# Kmeans法、EM法実験と考察

  - Kmeans法
　  単純かつ安定して動作する。ただし、要素数にunballanceがあると目でみて
　　明らかものでも分類できないことがある。

  - EM method の収束性
   KmeansEMtest1-3.cxx で 2次元混合Gauss分布 2分類問題を実験した。
    Kmeans法は常に期待通りの収束
    EM法は収束しない場合があった
     + bxsigma と bysigma の分散の比が大きいと収束性が悪くなる
     + Kmeans法で収束させてお膳立てしないと収束しない
    
   Program の不具合なのか EM法そのものの問題なのか不明。
　 PRML 下巻 9.2章 p150 に記載の問題かも知れない。
    "一方の要素が特定のデータ点に収束すれば、対数尤度を増加させる方向に働く"

    つまりある次元の分散が0に近づくと、その次元の尤度ばかりが優勢になってし
    まい、適切な分類ができなくなる。

    とすると各次元の分散が同程度のdataしか扱うことができない?
    一時的に平均に乱数を加え分散を大きくして回避するとp150には書いてある。

      
