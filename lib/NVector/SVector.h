/**************************************************************************
* SparseVector.h
*
* 2013/Sep 
* Masaharu Goto
*
**************************************************************************/

#ifndef G__SPARCEVECTOR
#define G__SPARCEVECTOR

#include <map>
#include <algorithm>
#include <iostream>
#include <cmath>

//////////////////////////////////////////////////////////////////////
template<class T> class	SparceVector {
 public:
  typedef map<unsigned int,T>::iterator iterator;
  typedef map<unsigned int,T>::const_iterator const_iterator;

  ////////////////////////////////////////////////////////
  SparceVector() { m_default = 0; }
  SparceVector(const SparceVector) { }

  ////////////////////////////////////////////////////////
  void setsize(int Xsize) { m_Xsize=Xsize; }
  ////////////////////////////////////////////////////////
  unsigned int internalindex(int i) { 
    unsigned int ij = i;
    return(ij);
  }

  void index(unsigned int ij,int& i) {
    int i = ij; 
  }

  ////////////////////////////////////////////////////////
  // non sparce access
  //T& operator[](int ij) { return(m_sparcedata[ij]); }

  ////////////////////////////////////////////////////////
  // sparce access
  T& operator()(int i) { return(get(i)); }
  T& get(int i) { 
    unsigned int ij = internalindex(i);
    iterator it = m_sparcedata.find(ij);
    if(it==m_sparcedata.end()) {
      m_dmy = m_default;
      return(m_dmy);
    }
    else {
      return((*it).second);
    }
  }
  ////////////////////////////////////////////////////////
  T& set(int i,const T& val) {
    unsigned int ij = internalindex(i);
    iterator it = m_sparcedata.find(ij);
    if(it==m_sparcedata.end()) {
      pair<unsigned int,T> v(it,val);
      it = m_sparcedata.insert(v);
    }
    return((*it).second);
  }

  ////////////////////////////////////////////////////////
  iterator insert(int i,const T& val) {
    unsigned int ij = internalindex(i);
    pair<unsigned int,T> v(it,val);
    iterator it = m_sparcedata.insert(v);
    return(it);
  }

  ////////////////////////////////////////////////////////
  void index(iterator& it,int& i) { 
    if(m_it!=m_sparcedata.end()) {
      index((*m_it).first,i);
    }
    else {
      i = -1;
    }
  }

  ////////////////////////////////////////////////////////
  T& value(iterator& it) { 
    if(m_it!=m_sparcedata.end()) return((*it).second); 
    else {
      m_dmy = m_default;
      return(m_dmy);
    }
  }

  ////////////////////////////////////////////////////////
  iterator begin() {return(m_sparcedata.begin());}
  iterator end() {return(m_sparcedata.end());}
  ////////////////////////////////////////////////////////
  
 private:
  T m_default;
  T m_dmy;
  map<int,T> m_sparcedata;
  map<int,T>::iterator m_it;
  int m_Xsize;
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
template<class T> class	SparceMatrix {
 public:
  typedef map<unsigned int,T>::iterator iterator;
  typedef map<unsigned int,T>::const_iterator const_iterator;

  ////////////////////////////////////////////////////////
  SparceMatrix() { m_default = 0; }
  SparceMatrix(const SparceMatrix) { }

  ////////////////////////////////////////////////////////
  void setsize(int Xsize,int Ysize) { m_Xsize=Xsize; m_Ysize=Ysize; }
  ////////////////////////////////////////////////////////
  unsigned int internalindex(int i,int j) { 
    unsigned int ij = i*m_Ysize + j;
    return(ij);
  }

  void index(unsigned int ij,int& i,int& j) {
    int i = ij/m_Ysize; 
    int j = ij%m_Ysize;
  }

  ////////////////////////////////////////////////////////
  // non sparce access
  //T& operator[](int ij) { return(m_sparcedata[ij]); }

  ////////////////////////////////////////////////////////
  // sparce access
  T& operator()(int i,int j) { return(get(i,j)); }
  T& get(int i,int j) { 
    unsigned int ij = internalindex(i,j);
    iterator it = m_sparcedata.find(ij);
    if(it==m_sparcedata.end()) {
      m_dmy = m_default;
      return(m_dmy);
    }
    else {
      return((*it).second);
    }
  }
  ////////////////////////////////////////////////////////
  T& set(int i,int j,const T& val) {
    unsigned int ij = internalindex(i,j);
    iterator it = m_sparcedata.find(ij);
    if(it==m_sparcedata.end()) {
      pair<unsigned int,T> v(it,val);
      it = m_sparcedata.insert(v);
    }
    return((*it).second);
  }

  ////////////////////////////////////////////////////////
  iterator insert(int i,int j,const T& val) {
    unsigned int ij = internalindex(i,j);
    pair<unsigned int,T> v(it,val);
    iterator it = m_sparcedata.insert(v);
    return(it);
  }

  ////////////////////////////////////////////////////////
  void index(iterator& it,int& i,int& j) { 
    if(m_it!=m_sparcedata.end()) {
      index((*m_it).first,i,j);
    }
    else {
      i = j = -1;
    }
  }

  ////////////////////////////////////////////////////////
  T& value(iterator& it) { 
    if(m_it!=m_sparcedata.end()) return((*it).second); 
    else {
      m_dmy = m_default;
      return(m_dmy);
    }
  }

  ////////////////////////////////////////////////////////
  iterator begin() {return(m_sparcedata.begin());}
  iterator end() {return(m_sparcedata.end());}
  ////////////////////////////////////////////////////////
  
 private:
  T m_default;
  T m_dmy;
  map<int,T> m_sparcedata;
  map<int,T>::iterator m_it;
  int m_Xsize;
  int m_Ysize;
};

//////////////////////////////////////////////////////////////////////


#endif // G__SPARCEVECTOR

