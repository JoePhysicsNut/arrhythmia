//
//  comparisonPlots.h
//

#ifndef __comparisonPlots__
#define __comparisonPlots__

#include "Analysis.h"

using namespace std;

class comparisonPlots : public Analysis {
  
public:
  //Constructor & destructor
  comparisonPlots(const string& inputFileName);
  ~comparisonPlots();
  
  void initialize();
  
  //ECG
  void makeECGscatter(TString cClean);
  void makeECGscatterInv(TString cClean);
  
  //RRint
  void makeRRdistribution(TString cClean);
  void makeRRprofile(TString cClean);
  void makeRRscatter(TString cClean);
  void makeRRscatterInv(TString cClean);
  void makeRRscatterOutlier();
  
  //Ectopic beats
  void makeEctopicProfile();
  void makeEctopicDistribution();
  
  //Mean and stdev
  void makeMeanProfile();
  void makeMeanScatter();
  void makeMeanScatterInv();
  
  void makeStdevProfile();
  void makeStdevScatter();
  void makeStdevScatterInv();
  
  //ML output
  void makeMLProfile();
  void makeMLScatter();
  void makeMLScatterInv();
  
  //Efficiency
  void computeEfficiency();
  
  //Patient level ML
  void makePatientLevel();
  void makePatientLevelMIMIC();
  
  void finalize();
  
private:

  TFile* m_inputFile;
  TTree* signalTree;
  TTree* backgroundTree;
  
  //ECG
  void makeECGscatter(TString cClean, TString name, TCut cut, TString additionalLabel);
  
  //RRint
  void makeRRdistribution(TString cClean, TString name, TCut cut);
  void makeRRprofile(TString cClean, TString name, TCut cut);
  void makeRRscatter(TString cClean, TString name, TCut cut, TString additionalLabel);
  void makeRRscatterOutlier(TString name);
  
  //Ectopic beats
  void makeEctopicProfile(TString name, TCut cut);
  void makeEctopicDistribution(TString name, TCut cut);
  
  //Mean and stdev
  void makeMeanProfile(TString name, TCut cut);
  void makeMeanScatter(TString name, TCut cut, TString additionalLabel);
  void makeStdevProfile(TString name, TCut cut);
  void makeStdevScatter(TString name, TCut cut, TString additionalLabel);
  
  //ML output
  void makeMLProfile(TString name, TCut cut);
  void makeMLScatter(TString name, TCut cut, TString additionalLabel);
  
  //Efficiency
  TEfficiency* computeEfficiency(TTree* tree, TString name, TCut cut);
  
  //Helpers
  pair<Float_t, Float_t> computeParameters(TTree* tree, string name);
  void computeNormalisation(string MVA, Float_t& norm, Float_t& shift);
  void askAFriendVoter(TTree* tree, TH1F* hist, TCut cut);
  
  //Patient level ML
  void makePatientLevel(TString name, pair<Double_t, Double_t> edges);
};

#endif /* defined(__comparisonPlots__) */
