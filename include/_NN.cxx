/****************************************************************************
* NN.cxx
*  2 layer RBN pretraining + 3 layer NN back propagation learinig
*  Input level 0, 1
*
*
****************************************************************************/
#ifdef __CINT__
#include <NeuralNet.h>
#else
#include "MachineLearningElementNN.h"
#include "NeuralNetClustering.h"
#endif
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <string>
#include <map>
using namespace std;

///////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
int main(int argc,char** argv) {
#ifdef __CINT__
  G__optimizemode(3);
#endif
  DEBUG=0; //defined in  NeuralNet library
  int seed = 1; // random seed for NeuralNet initialization and RBM
  int nline=40; // number of chars in line for NeuralNetDataSet::disp
  int nodisp=0;
  int cluster=0; // 0 for using pre-defined classification, >0 for clustering
#ifdef HIER
  int clusterH=300; // clustering upper limit
#else
  int clusterH=30; // clustering upper limit
#endif
  int clusteringMode= -1; // clustering mode
  const int RND=1, CNT=2,BDR=3, NONBDR=4, ALLDATA=5; // NN learning sampling mode
  int trainednet=0; // loading pre-trained neural network weights
  NN_REAL Err=0,dErr=0,RBM_Err=0; // NN convergence local limit, Not used
  int enableSeparatedLearning=0; // split NN learning for complex clustering, not ready
  NN_REAL TotalErrThresh=(NN_REAL)0.2; // NN convergence total limit
  NN_REAL FNRthresh=0.5;
  NN_REAL FPRthresh=1.0;
  int isAllSamples = 0; // use all training samples for NN training
  string To,So; // output file name
  vector<string> Trainings, Samples; // input file names (supervisor and test)

  int nTranspose2 = 0 ; // Transpose Training data row/column for cross clustering
  int nTranspose = 0 ; // Transpose Training data row/column for cross clustering
  string NameFile;     // read data name from given file
  string saveTrainingData; // transposed data output file name
  int hier=0;
  string VectorDimensionFile; // read active input vector dimension from file
  int nameClustering=1; // enable name clustering
  int sortid=0, regexpid=0;
  int dispNN=0;
  string StatOutFile;
  int StatM=0;
  int outputdata=1; // data: 0x01 , inter cls stat: 0x02, all stat: 0x03,
  int sortcontents=0;
  FVectorSimilarity simfunc=NormalizedInnerProduct;

  ///////////////////////////////////////////////////////////////
  int i;
  if(argc<=1) {
    fprintf(stderr,"Usage: NeuralNet <options>\n");
    fprintf(stderr,"    -T [TrainingData].csv\n");
    fprintf(stderr,"    -S [SampleData].csv\n");
    fprintf(stderr,"    -To [TrainingOutput].csv\n");
    fprintf(stderr,"    -So [SampleOutput].csv\n");
    fprintf(stderr,"    -C [NumberOfCluster] <[upperlimit]>\n");
    fprintf(stderr,"        NumberOfCluster=0 : use predefined label\n");
    fprintf(stderr,"    -mode [link-mode] 0:complete,1:single(default),2:average\n");
    fprintf(stderr,"    -l [errlim] [derrlim] [err/size]\n");
    fprintf(stderr,"    -s [randomseed]\n");
    fprintf(stderr,"    -separated : Enable separated learning\n");
    fprintf(stderr,"    -E [TotalErrThresh],  <0 skip NN conversion\n");
    fprintf(stderr,"    -FNR [FalseNegativeRateThresh] (default=0.5)\n");
    fprintf(stderr,"    -FPR [FalsePositiveRateThresh] (default=1.0)\n");
    fprintf(stderr,"    -ALL : Training with all sample\n");
    fprintf(stderr,"    -t : Use trained neural network weight\n");
    fprintf(stderr,"    -t -t : Clustering only, no neural network activity\n");
    fprintf(stderr,"    -d [NcharInLine] : set number of symbols in single line\n");
    fprintf(stderr,"    -n : do not display detail of result\n");
    // following options should be separated as NNdataTranspose.exe in future
    fprintf(stderr,"    -R [nTranspose] : Transpose TrainigData row/column\n");
    fprintf(stderr,"    -name [nameFile] : obtain data ID(name) from [namefile]\n");
    fprintf(stderr,"    -save [fname] : save training data and exit\n");
    fprintf(stderr,"    -hier : save hierarchy classification\n");
    fprintf(stderr,"    -v [vectorDimensionFile] [nTranspose2]\n");
    fprintf(stderr,"    -noname : No name clustering\n");
    fprintf(stderr,"    -sortv : Sort data by contents before clustering\n");
    fprintf(stderr,"    -sortt|y|c : Sort data by supervisor/neural-net/name classification\n");
    fprintf(stderr,"    -regexpt|r|c : Generate Regular Expression label\n");
    fprintf(stderr,"    -simfunc [innerprod|euclid] : similarity function\n");
    fprintf(stderr,"    -dispNN : display neural network weight parameter statistics\n");
    fprintf(stderr,"    -stat [StatOutFile] [M} [Outmode] : Output supervisor statistics for incremental clustering\n");
    fprintf(stderr,"                                      : [M} number of Mini-cluster\n");
    fprintf(stderr,"                                      : [Outmode} 0: stat only, 1:+data, 2:+inter clst stat, 4:+all stat\n");
#ifdef CLUSTERING_MULTI_THREAD
    fprintf(stderr,"    -thread [N] : number of threads(default=4)\n");
#endif
    exit(255);
  }
  for(i=1;i<argc;i++) {
    if(strcmp(argv[i],"-t")==0) ++trainednet;
    else if(strcmp(argv[i],"-T")==0) Trainings.push_back(argv[++i]);
    else if(strcmp(argv[i],"-S")==0) Samples.push_back(argv[++i]);
    else if(strcmp(argv[i],"-To")==0) To=argv[++i];
    else if(strcmp(argv[i],"-So")==0) So=argv[++i];
    else if(strcmp(argv[i],"-g")==0) ++DEBUG;
    else if(strcmp(argv[i],"-n")==0) ++nodisp;
    else if(strcmp(argv[i],"-s")==0) seed = atoi(argv[++i]);
    else if(strcmp(argv[i],"-C")==0) {
      cluster= atoi(argv[++i]);
      if(i+1<argc && isdigit(argv[i+1][0])) clusterH=atoi(argv[++i]);
    }
    else if(strcmp(argv[i],"-mode")==0) clusteringMode= atoi(argv[++i]);
    else if(strcmp(argv[i],"-d")==0) nline = atoi(argv[++i]);
    else if(strcmp(argv[i],"-separated")==0) enableSeparatedLearning=1;
    else if(strcmp(argv[i],"-E")==0) TotalErrThresh = atof(argv[++i]);
    else if(strcmp(argv[i],"-FNR")==0) FNRthresh = atof(argv[++i]);
    else if(strcmp(argv[i],"-FPR")==0) FPRthresh = atof(argv[++i]);
    else if(strcmp(argv[i],"-ALL")==0) ++isAllSamples;
    // following options should be separated as NNdataTranspose.exe in future
    else if(strcmp(argv[i],"-R")==0) nTranspose = atoi(argv[++i]);
    else if(strcmp(argv[i],"-name")==0) NameFile = argv[++i];
    else if(strcmp(argv[i],"-save")==0) saveTrainingData = argv[++i];
    else if(strcmp(argv[i],"-hier")==0) ++hier;
    else if(strcmp(argv[i],"-sortv")==0) sortcontents=1;
    else if(strcmp(argv[i],"-sortt")==0) sortid=1;
    else if(strcmp(argv[i],"-sorty")==0) sortid=2;
    else if(strcmp(argv[i],"-sortc")==0) sortid=3;
    else if(strcmp(argv[i],"-regexpt")==0) regexpid=1;
    else if(strcmp(argv[i],"-regexpy")==0) regexpid=2;
    else if(strcmp(argv[i],"-regexpc")==0) regexpid=3;
    else if(strcmp(argv[i],"-v")==0) {
      VectorDimensionFile = argv[++i];
      if(i+1<argc && isdigit(argv[i+1][0])) nTranspose2=atoi(argv[++i]);
    }
    else if(strcmp(argv[i],"-simfunc")==0) {
      if(strncmp(argv[i+1],"innerprod",3)==0) simfunc=NormalizedInnerProduct;
      else if(strncmp(argv[i+1],"euclid",3)==0) simfunc=NormalizedEuclideanDistance;
      ++i;
    }
    else if(strcmp(argv[i],"-noname")==0) nameClustering^=1;
    else if(strcmp(argv[i],"-dispNN")==0) dispNN^=1;
    else if(strcmp(argv[i],"-l")==0) {
      Err=atof(argv[++i]);
      dErr=atof(argv[++i]);
      RBM_Err=atof(argv[++i]);
    }
    else if(strcmp(argv[i],"-stat")==0) {
      StatOutFile = argv[++i];
      StatM = atoi(argv[++i]);
      if(i+1<argc && isdigit(argv[i+1][0])) outputdata=atoi(argv[++i]);
    }
#ifdef CLUSTERING_MULTI_THREAD
    else if(strcmp(argv[i],"-thread")==0) {
      Clustering_num_threads = atoi(argv[++i]);
    }
#endif
    //else ReadCondition(argv[i]);
  }

  ////////////////////////////////////////////////////////////////////////
  if(seed) RandomSeed(seed);
  MachineLearningElementNN mle;

  ////////////////////////////////////////////////////////////////////////
  for(i=0;i<Trainings.size();i++) {
    printf("Reading Training Data from %s\n",Trainings[i].c_str());
    mle.readNNDataFiles(Trainings[i].c_str());
  }
  printf("d.size=%lu\n",mle.masterData().size());

  /////////////////////////////////////////////////////////////////////////
  if(nTranspose) { // transpose input data for cross clustering
	printf("Transpose input data\n");
    mle.masterData().transpose(nTranspose);
  }
  if(NameFile!="") {
	printf("Read data sample names from '%s'\n",NameFile.c_str());
    mle.masterData().readNNName(NameFile.c_str(),1,0,1);
  }

  /////////////////////////////////////////////////////////////////////////
  if(VectorDimensionFile!="") { // Select input vector dimension 
    vector<int> vectordim;
    readVectorcsvInt(VectorDimensionFile.c_str(),vectordim);
    if(mle.masterData().selectInputDimensions(vectordim,nTranspose2)) exit(255);
    mle.setVectorDimensionFile(VectorDimensionFile.c_str());
  }

  /////////////////////////////////////////////////////////////////////////
  if(sortcontents) { // sort input data by contents
    mle.masterData() = mle.masterData().sort_contents();
  }

  /////////////////////////////////////////////////////////////////////////
  // Do data clustering or use pre-defined classification
  mle.setClustering(cluster,clusterH);
  if(cluster>0) {
    printf("Clustering...");
    if(clusteringMode<0) {
      if(nameClustering) mle.ClusteringName(); // data name clustering (optional)
      mle.Clustering();     // data clustering
    }
    else {
      if(nameClustering) mle.ClusteringName(clusteringMode); //data name clustering(optional)
      mle.Clustering(clusteringMode,simfunc);     // data clustering
    }
    printf("done\n");
    mle.masterData().m_clst.disp_property();
  }
  else if(cluster==0) { // use pre-defined classification
    mle.masterData().make_similaritymatrix();
    mle.masterData().m_clst.disp_property();
  }

  /////////////////////////////////////////////////////////////////////////
  // -save [file] <-hier <-hier>>
  if(saveTrainingData!="") { // save training data (mainly for transposing)
    printf("Save input data file '%s' and exit\n",saveTrainingData.c_str());
    mle.masterData().saveNNDataFile(saveTrainingData.c_str(),hier);
    exit(0);
  }

  /////////////////////////////////////////////////////////////////////////
  // Set NeuralNet training conditions
  if(isAllSamples) {
	mle.clearSamplingPolicy();
	mle.addSamplingPolicy(ALLDATA);
  }
  mle.setEnableSeparatedLearning(enableSeparatedLearning);
  mle.setTotalErrThresh(TotalErrThresh);
  mle.setFNRthresh(FNRthresh);
  mle.setFPRthresh(FPRthresh);

  // NeuralNet training or loading per-trained neuralnet weight parameters
  int stat;
  if(trainednet==1) {
    stat=mle.load(); // load pre-trained NeuralNet weights
    if(stat) {
      printf("Error: Failed loading trained neural network\n");
      exit(1);
    }
  }
  else if(trainednet>=2) {
    // Do nothing about NeuralNet, just output clustering result
    mle.masterData().dispcsv();
    if(To!="") mle.masterData().dispcsv(To.c_str(),"w");
    //exit(0);
  }
  else {
    stat=mle.Learn(); // NN learning, learned weight is automatically saved
  }

  if(stat) {
    printf("!!!Learning Failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
  }

  ///////////////////////////////////////////////////////////////////////////
  printf("After training Verification ************************************\n");
  NN_REAL eval = mle.UpdateOutput_Evaluation();
  switch(sortid){
  case 1: mle.masterData() = mle.masterData().sort_t(); break;
  case 2: mle.masterData() = mle.masterData().sort_y(); break;
  case 3: mle.masterData() = mle.masterData().sort_c(); break;
  default: break;
  }
  switch(regexpid){
  case 1: mle.masterData().regexp_t(); break;
  case 2: mle.masterData().regexp_y(); break;
  case 3: mle.masterData().regexp_c(); break;
  default: break;
  }
  if(nodisp<2) mle.learningData().disp(stdout,nline);
  if(nodisp<1) mle.masterData().disp(stdout,nline);
  //mle.learningData().dispcsv();
  mle.masterData().dispcsv();
  // -To [outfile] <-hier>
#ifdef HIER
  if(To!="") mle.masterData().dispcsv(To.c_str(),"w",hier?hier:1);
#else
  if(To!="") mle.masterData().dispcsv(To.c_str(),"w",hier);
#endif

  ///////////////////////////////////////////////////////////////////////////
  // Evaluate other samples
  NeuralNetDataSet Sample;
  for(i=0;i<Samples.size();i++) {
    printf("Reading Sample Data from %s\n",Samples[i].c_str());
    Sample.readNNDataFiles(Samples[i].c_str());
  }
  if(Sample.Nsample()) {
    //G__pause();
    printf("Evaluation *****************************************************\n");
    eval = mle.UpdateOutput_Evaluation(Sample);
    Sample.mergeSupervisor(mle.masterData());
    switch(sortid){
    case 1: Sample = Sample.sort_t(); break;
    case 2: Sample = Sample.sort_y(); break;
    case 3: Sample = Sample.sort_c(); break;
    default: break;
    }
    switch(regexpid){
    case 1: Sample.regexp_t(); break;
    case 2: Sample.regexp_y(); break;
    case 3: Sample.regexp_c(); break;
    default: break;
    }
    if(nodisp<1) Sample.disp(stdout,nline);
    Sample.dispcsv();
    // -So [outfile] <-hier>
#ifdef HIER
    if(So!="") Sample.dispcsv(So.c_str(),"w",hier?hier:1);
#else
    if(So!="") Sample.dispcsv(So.c_str(),"w",hier);
#endif
  }
  if(dispNN) mle.mainNet().disp();

  ///////////////////////////////////////////////////////////////////
  // Output Supervisor Statistics for Incremental clsutering
  if(StatOutFile!="") {
    printf("Saving Supervisor Statistics for incremental clustering '%s' StatM=%d out=%d\n"
	   ,StatOutFile.c_str(),StatM,outputdata);
    mle.masterData().saveStatDataFile(StatOutFile.c_str(),StatM
				      ,clusteringMode
				      ,outputdata);
  }

  return(0);
}
///////////////////////////////////////////////////////////////////////

