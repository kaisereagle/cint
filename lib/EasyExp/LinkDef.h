
#ifdef __MAKECINT__

#pragma link off all classes;
#pragma link off all typedef;
#pragma link off all globals;
#pragma link off all fnctions;

#pragma link C++ global EASYEXPH;
#pragma link C++ global EasyExpH;
#pragma link C++ class EasyExp;
#pragma link C++ class MatchElement;
#pragma link C++ class vector<EasyExp>;
#pragma link C++ class Stat<int>;
#pragma link C++ class Stat<long>;
#pragma link C++ class Stat<float>;
#pragma link C++ class Stat<double>;
//#pragma link C++ class EasyExpReplace-;
#pragma link C++ class Scandir;
#pragma link C++ class G__MultiByteMode;
#pragma link C++ global  EasyExp_DEBUG;

//#pragma link C++ class TF_IDF;
#pragma link C++ class TermFrequency;
#pragma link C++ class TermFrequencyTable;
#pragma link C++ class DocumentFrequency;
#pragma link C++ class InverseDocumentFrequencyTable;
#pragma link C++ class TF_IDF;
#pragma link C++ class map<string,DocumentFrequency*>;
#pragma link C++ class map<string,DocumentFrequency*>::iterator;
#pragma link C++ class map<string,DocumentFrequency*>::value_type;
#pragma link C++ class map<string,TermFrequency*>;
#pragma link C++ class map<string,TermFrequency*>::iterator;
#pragma link C++ class map<string,TermFrequency*>::value_type;

//void commonsubstring(vector<string>& r,const string& a,const string& b,int MinString=3) ;
void commonsubstring(map<string,int>& comstr,const string& a,const string& b,int MinString) ;
#pragma link C++ funciton commonsubstring;

#pragma link C++ global EasyExp_num_threads;
#pragma link C++ global EasyExp_num_threads_limit;

#endif

