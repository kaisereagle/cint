# Mathematical Vector,(Matrix,) FFT, LSM library

# Files

 - Vector.h, Complex.h 

 - Histogram.h

 - SparseVector.h

 - Stat.h/cxx

 - Fft.h/cxx

 - Lsm.h/cxx

 - setup, setup.bat

 - $CINTSYSDIR/include/NVector.h


# Old files
 - NArray.h/cxx
 - Matrix23.h
 - statistics.h/c



# Peak detection, characteristics extraction

 - 微分
    noise の影響が大きい

 - 多項式回帰  2次回帰を用いて区間を下記分類
    平坦
    増加   増増加　直線増加　減増加
    減少   増減少　直線増加　減減少
    Peak   極大　　極小
    振動

 - 区間最大

 - 中心点区間最大
   注目する中心点が左右 N 点の区間最大、または区間最小かどうかを判定。

 - 中心点区間最大 & > thresh
   注目する中心点が左右 N 点の区間最大、または区間最小かどうかを判定。

 - 中心点区間最大 & 周辺区間最小との差>thresh
   注目する中心点が左右 N 点の区間最大、または区間最小かどうかを判定。

 - Residual
   停留値を求める。

  + Clustering と cluster 中心値を求める問題に帰結
     K means法
     EM法
     変分Bayes法
     Histogram peak-bottom separation
        Histogram envelove から正規分布fitting or Laplace近似

