//
//  comparisonPlots.cpp
//

#include "comparisonPlots.h"

//______________________________________________________________
comparisonPlots::comparisonPlots(const string& inputFileName) {

  m_inputFile = TFile::Open(inputFileName.c_str());
}

//______________________________________________________________
comparisonPlots::~comparisonPlots() {
}

//______________________________________________________________
void comparisonPlots::initialize() {

  signalTree = (TTree*) m_inputFile->Get("signal");
  backgroundTree = (TTree*) m_inputFile->Get("background");
  
  return;
}

//______________________________________________________________
void comparisonPlots::makeECGscatter(TString cClean) {
  
  makeECGscatter(cClean, "ch2ECG", "", "");
  
  return;
}

//______________________________________________________________
void comparisonPlots::makeECGscatterInv(TString cClean) {
  
  Float_t fileID;
  signalTree->SetBranchAddress("fileID",&fileID);
  Float_t numberOfFiles = signalTree->GetMaximum("fileID")+1;
  
  for (Float_t i = 0; i < numberOfFiles; i++) {
    TString cut = "fileID==";
    cut += TStringize(i);
    
    signalTree->Draw(">>list", cut, "entrylist");
    TEntryList* list = (TEntryList*) gDirectory->Get("list");
    signalTree->SetEntryList(list);
    Float_t patientID = signalTree->GetMaximum("patientID");
    TString patient = "Patient " + TStringize(patientID);
    
    makeECGscatter(cClean, TStringize(i), (TCut) cut, patient);
    
    delete list;
    signalTree->SetEntryList(0);
  }
  
  return;
}

//______________________________________________________________
void comparisonPlots::makeRRdistribution(TString cClean) {

  makeRRdistribution(cClean, "chRRint", "");
  
  return;
}

//______________________________________________________________
void comparisonPlots::makeRRprofile(TString cClean) {

  makeRRprofile(cClean, "cpRRint", "");
  
  return;
}

//______________________________________________________________
void comparisonPlots::makeRRscatter(TString cClean) {

  makeRRscatter(cClean, "ch2RRint", "", "");
  
  return;
}

//______________________________________________________________
void comparisonPlots::makeRRscatterInv(TString cClean) {
  
  Float_t fileID;
  signalTree->SetBranchAddress("fileID",&fileID);
  Float_t numberOfFiles = signalTree->GetMaximum("fileID")+1;
  
  for (Float_t i = 0; i < numberOfFiles; i++) {
    TString cut = "fileID==";
    cut += TStringize(i);
    
    signalTree->Draw(">>list", cut, "entrylist");
    TEntryList* list = (TEntryList*) gDirectory->Get("list");
    signalTree->SetEntryList(list);
    Float_t patientID = signalTree->GetMaximum("patientID");
    TString patient = "Patient " + TStringize(patientID);
    
    makeRRscatter(cClean, TStringize(i), (TCut) cut, patient);
    
    delete list;
    signalTree->SetEntryList(0);
  }
  
  return;
}

//______________________________________________________________
void comparisonPlots::makeRRscatterOutlier() {
  
  makeRRscatterOutlier("ch2RRintOutlier");
  
  return;
}

//______________________________________________________________
void comparisonPlots::makeEctopicProfile() {

  makeEctopicProfile("cpEctopic", "");
  
  return;
}

//______________________________________________________________
void comparisonPlots::makeEctopicDistribution() {
  
  makeEctopicDistribution("chEctopic", "");
  
  return;
}

//______________________________________________________________
void comparisonPlots::makeMeanProfile() {

  makeMeanProfile("cpMean", "");
  
  return;
}

//______________________________________________________________
void comparisonPlots::makeMeanScatter() {

  makeMeanScatter("ch2Mean", "", "");
  
  return;
}

//______________________________________________________________
void comparisonPlots::makeStdevProfile() {

  makeStdevProfile("cpStdev", "");
  
  return;
}

//______________________________________________________________
void comparisonPlots::makeStdevScatter() {

  makeStdevScatter("ch2Stdev", "", "");
  
  return;
}

//______________________________________________________________
void comparisonPlots::makeMLProfile() {

  makeMLProfile("cpML", "");
  
  return;
}

//______________________________________________________________
void comparisonPlots::makeMLScatter() {

  makeMLScatter("ch2ML", "", "");
  
  return;
}

//______________________________________________________________
void comparisonPlots::computeEfficiency() {

  TEfficiency* signalEfficiency = computeEfficiency(signalTree, "signalEfficiency", "");
  TEfficiency* backgroundEfficiency = computeEfficiency(backgroundTree, "backgroundEfficiency", "");
  
  makePlots(signalEfficiency, backgroundEfficiency, "classifierOutputEfficiency", "Arrhythmic", "Normal");
  
  return;
}

//______________________________________________________________
void comparisonPlots::makePatientLevel() {
  
  vector<TString> MVAs;
  MVAs.push_back("BDT");
  MVAs.push_back("KNN");
  MVAs.push_back("PDERS");
  MVAs.push_back("SVM");
  MVAs.push_back("MLPBNN");
  
  vector<pair<Double_t, Double_t> > edges;
  edges.push_back(make_pair(0.3, 0.7));
  edges.push_back(make_pair(0., 1.));
  edges.push_back(make_pair(0., 1.));
  edges.push_back(make_pair(0., 1.));
  edges.push_back(make_pair(0., 1.));
  
  for (unsigned int i = 0; i < MVAs.size(); i++) {
    makePatientLevel(MVAs[i], edges[i]);
  }
  
  return;
}

//______________________________________________________________
void comparisonPlots::finalize() {

  m_inputFile->Close();

  return;
}

//______________________________________________________________
void comparisonPlots::makeECGscatter(TString cClean, TString name, TCut cut, TString additionalLabel) {
  
  TString variable = "ECGval";
  variable += cClean;

  TH2F* h2ECGvalsig = new TH2F("h2ECGvalsig", ";#it{t} [s];ECG [mV]", 250, 0., 1., 250, -4., 7.);
  //TH2F* h2ECGvalbkg = new TH2F("h2ECGvalbkg", ";#it{t} [s];RR interval [ms]", 250, 0., 600., 250, -1., 1.);

  signalTree->Draw(variable+":time>>h2ECGvalsig", cut);
  //backgroundTree->Draw(variable+":time>>h2ECGvalbkg", cut);
  
  makePlots(h2ECGvalsig, name+cClean, "Arrhythmic", additionalLabel);
  //makePlots(h2ECGvalsig, h2ECGvalbkg, name+cClean, "Arrhythmic", "Normal", additionalLabel);
  
  return;
}

//______________________________________________________________
void comparisonPlots::makeRRdistribution(TString cClean, TString name, TCut cut) {
  
  TString variable = "RRint";
  variable += cClean;
  
  TH1F* hRRintSig     = new TH1F("hRRintSig", ";RR interval [ms];Beats", 20, 200., 1400.);
  TH1F* hRRintSigCrop = new TH1F("hRRintSigCrop", ";RR interval [ms];Beats", 20, 200., 1400.);
  TH1F* hRRintBkg     = new TH1F("hRRintBkg", ";RR interval [ms];Beats", 20, 200., 1400.);
  
  signalTree->Draw(variable+">>hRRintSig", cut);
  signalTree->Draw(variable+">>hRRintSigCrop", "timeToDetection>300" && cut);
  backgroundTree->Draw(variable+">>hRRintBkg", cut);
  
  normalise(hRRintSig);
  normalise(hRRintSigCrop);
  normalise(hRRintBkg);
  
  makePlots(hRRintSig, hRRintSigCrop, hRRintBkg, name+cClean, "Arrhythmic", "Arrhythmic (5min prior)", "Normal");
  
  return;
}

//______________________________________________________________
void comparisonPlots::makeRRprofile(TString cClean, TString name, TCut cut) {

  TString variable = "RRint";
  variable += cClean;

  TProfile* pRRintsig = new TProfile("pRRintsig", ";#it{t} [s];Mean RR interval [ms]", 250, 0., 900.);
  TProfile* pRRintbkg = new TProfile("pRRintbkg", ";#it{t} [s];Mean RR interval [ms]", 250, 0., 900.);
  
  signalTree->Draw(variable+":timeToDetection>>pRRintsig", cut);
  backgroundTree->Draw(variable+":timeToDetection>>pRRintbkg", cut);
  
  makePlots(pRRintsig, pRRintbkg, name+cClean, "Arrhythmic", "Normal", 2);
  
  return;
}

//______________________________________________________________
void comparisonPlots::makeRRscatter(TString cClean, TString name, TCut cut, TString additionalLabel) {

  TString variable = "RRint";
  variable += cClean;

  TH2F* h2RRintsig = new TH2F("h2RRintsig", ";#it{t} [s];RR interval [ms]", 250, 0., 1200., 250, 100., 2200.);
  TH2F* h2RRintbkg = new TH2F("h2RRintbkg", ";#it{t} [s];RR interval [ms]", 250, 0., 1200., 250, 100., 2200.);
  
  signalTree->Draw(variable+":timeToDetection>>h2RRintsig", cut);
  backgroundTree->Draw(variable+":timeToDetection>>h2RRintbkg", cut);
  
  makePlots(h2RRintsig, h2RRintbkg, name+cClean, "Arrhythmic", "Normal", additionalLabel);
  
  return;
}

//______________________________________________________________
void comparisonPlots::makeRRscatterOutlier(TString name) {

  TH2F* h2RRintEctopic = new TH2F("h2RRintEctopic", ";#it{t} [s];RR interval [ms]", 250, 0., 1200., 250, 100., 2200.);
  TH2F* h2RRintOutlier = new TH2F("h2RRintOutlier", ";#it{t} [s];RR interval [ms]", 250, 0., 1200., 250, 100., 2200.);

  signalTree->Draw("RRint:timeToDetection>>h2RRintEctopic", "ectopicBeat==1");
  signalTree->Draw("RRint:timeToDetection>>h2RRintOutlier", "ectopicBeat==0&&outlier==1");

  makePlots(h2RRintEctopic, h2RRintOutlier, name, "Ectopic beats", "Noise", "");
  
  return;
}

//______________________________________________________________
void comparisonPlots::makeEctopicProfile(TString name, TCut cut) {

  TProfile* pEctopicSig = new TProfile("pEctopicSig", ";#it{t} [s];Mean fraction of ectopic beats", 15, 0., 1400.);
  TProfile* pEctopicBkg = new TProfile("pEctopicBkg", ";#it{t} [s];Mean fraction of ectopic beats", 15, 0., 1400.);
  
  signalTree->Draw("ectopicBeat:timeToDetection>>pEctopicSig", cut);
  backgroundTree->Draw("ectopicBeat:timeToDetection>>pEctopicBkg", cut);
  
  makePlots(pEctopicSig, pEctopicBkg, name, "Arrhythmic", "Normal", 3);
  
  return;
}

//______________________________________________________________
void comparisonPlots::makeEctopicDistribution(TString name, TCut cut) {
  
  TH1F* hEctopicSig = new TH1F("hEctopicSig", ";Mean fraction of ectopic beats;Frequency", 20, 0., 1.);
  TH1F* hEctopicBkg = new TH1F("hEctopicBkg", ";Mean fraction of ectopic beats;Frequency", 20, 0., 1.);
  
  signalTree->Draw("ectopicBeatsFrequency>>hEctopicSig", cut);
  backgroundTree->Draw("ectopicBeatsFrequency>>hEctopicBkg", cut);
  
  normalise(hEctopicSig);
  normalise(hEctopicBkg);
  
  makePlots(hEctopicSig, hEctopicBkg, name, "Arrhythmic", "Normal");
  
  return;
}

//______________________________________________________________
void comparisonPlots::makeMeanProfile(TString name, TCut cut) {

  TProfile* pMeansig = new TProfile("pMeansig", ";#it{t} [s];Mean Mean RR interval [ms]", 250, 0., 1100.);
  TProfile* pMeanbkg = new TProfile("pMeanbkg", ";#it{t} [s];Mean Mean RR interval [ms]", 250, 0., 1100.);
  
  signalTree->Draw("mean_120:timeToDetection>>pMeansig", cut && "mean_120>-999.");
  backgroundTree->Draw("mean_120:timeToDetection>>pMeanbkg", cut && "mean_120>-999.");
  
  makePlots(pMeansig, pMeanbkg, name, "Arrhythmic", "Normal", 3);
  
  return;
}

//______________________________________________________________
void comparisonPlots::makeMeanScatter(TString name, TCut cut, TString additionalLabel) {

  TH2F* h2Meansig = new TH2F("h2Meansig", ";#it{t} [s];Mean RR interval [ms]", 250, 0., 1200., 250, -20., 40.);
  TH2F* h2Meanbkg = new TH2F("h2Meanbkg", ";#it{t} [s];Mean RR interval [ms]", 250, 0., 1200., 250, -20., 40.);
  
  signalTree->Draw("mean_120:timeToDetection>>h2Meansig", cut);
  backgroundTree->Draw("mean_120:timeToDetection>>h2Meanbkg", cut);
  
  makePlots(h2Meansig, h2Meanbkg, name, "Arrhythmic", "Normal", additionalLabel);
  
  return;
}

//______________________________________________________________
void comparisonPlots::makeMeanScatterInv() {

  Float_t fileID;
  signalTree->SetBranchAddress("fileID",&fileID);
  Float_t numberOfFiles = signalTree->GetMaximum("fileID")+1;
  
  for (Float_t i = 0; i < numberOfFiles; i++) {
    TString cut = "fileID==";
    cut += TStringize(i);
    
    signalTree->Draw(">>list", cut, "entrylist");
    TEntryList* list = (TEntryList*) gDirectory->Get("list");
    signalTree->SetEntryList(list);
    Float_t patientID = signalTree->GetMaximum("patientID");
    TString patient = "Patient " + TStringize(patientID);

    makeMeanScatter(TStringize(i), (TCut) cut, patient);

    delete list;
    signalTree->SetEntryList(0);
  }

  return;
}

//______________________________________________________________
void comparisonPlots::makeStdevProfile(TString name, TCut cut) {

  TProfile* pStdevsig = new TProfile("pStdevsig", ";#it{t} [s];#it{#sigma} [ms]", 250, 0., 900.);
  TProfile* pStdevbkg = new TProfile("pStdevbkg", ";#it{t} [s];#it{#sigma} [ms]", 250, 0., 900.);
  
  signalTree->Draw("stdev_100:timeToDetection>>pStdevsig", cut && "stdev_100>0");
  backgroundTree->Draw("stdev_100:timeToDetection>>pStdevbkg", cut && "stdev_100>0");
  
  makePlots(pStdevsig, pStdevbkg, name, "Arrhythmic", "Normal", 1);
  
  return;
}

//______________________________________________________________
void comparisonPlots::makeStdevScatter(TString name, TCut cut, TString additionalLabel) {

  TH2F* h2Stdevsig = new TH2F("h2Stdevsig", ";#it{t} [s];#it{#sigma}_{RR} [ms]", 250, 0., 1200., 250, 0., 250.);
  TH2F* h2Stdevbkg = new TH2F("h2Stdevbkg", ";#it{t} [s];#it{#sigma}_{RR} [ms]", 250, 0., 1200., 250, 0., 250.);
  
  signalTree->Draw("stdev_100:timeToDetection>>h2Stdevsig", cut);
  backgroundTree->Draw("stdev_100:timeToDetection>>h2Stdevbkg", cut);
  
  makePlots(h2Stdevsig, h2Stdevbkg, name, "Arrhythmic", "Normal", additionalLabel);
  
  return;
}

//______________________________________________________________
void comparisonPlots::makeStdevScatterInv() {

  Float_t fileID;
  signalTree->SetBranchAddress("fileID",&fileID);
  Float_t numberOfFiles = signalTree->GetMaximum("fileID")+1;
  
  for (Float_t i = 0; i < numberOfFiles; i++) {
    TString cut = "fileID==";
    cut += TStringize(i);
    
    signalTree->Draw(">>list", cut, "entrylist");
    TEntryList* list = (TEntryList*) gDirectory->Get("list");
    signalTree->SetEntryList(list);
    Float_t patientID = signalTree->GetMaximum("patientID");
    TString patient = "Patient " + TStringize(patientID);

    makeStdevScatter(TStringize(i), (TCut) cut, patient);

    delete list;
    signalTree->SetEntryList(0);
  }

  return;
}

//______________________________________________________________
void comparisonPlots::makeMLProfile(TString name, TCut cut) {

  TProfile* pMLsig = new TProfile("pMLsig", ";#it{t} [s];Probability", 100, 0., 950., "s");
  TProfile* pMLbkg = new TProfile("pMLbkg", ";#it{t} [s];Probability", 100, 0., 950., "s");
  
  signalTree->Draw("probability:timeToDetection>>pMLsig", cut);
  backgroundTree->Draw("probability:timeToDetection>>pMLbkg", cut);
  
  makePlots(pMLsig, pMLbkg, name, "Arrhythmic", "Normal", 1, 0.4, 0.6);
  
  return;
}

//______________________________________________________________
void comparisonPlots::makeMLScatter(TString name, TCut cut, TString additionalLabel) {

  TH2F* h2MLsig = new TH2F("h2MLsig", ";#it{t} [s];Classifier output", 250, 0., 1200., 250, -1., 1.5);
  TH2F* h2MLbkg = new TH2F("h2MLbkg", ";#it{t} [s];Classifier output", 250, 0., 1200., 250, -1., 1.5);
  
  signalTree->Draw("probability:timeToDetection>>h2MLsig", cut);
  backgroundTree->Draw("probability:timeToDetection>>h2MLbkg", cut);
  
  makePlots(h2MLsig, h2MLbkg, name, "Arrhythmic", "Normal", additionalLabel);
  
  return;
}

//______________________________________________________________
void comparisonPlots::makeMLScatterInv() {

  Float_t fileID;
  signalTree->SetBranchAddress("fileID",&fileID);
  Float_t numberOfFiles = signalTree->GetMaximum("fileID")+1;
  
  for (Float_t i = 0; i < numberOfFiles; i++) {
    TString cut = "fileID==";
    cut += TStringize(i);
    
    signalTree->Draw(">>list", cut, "entrylist");
    TEntryList* list = (TEntryList*) gDirectory->Get("list");
    signalTree->SetEntryList(list);
    Float_t patientID = signalTree->GetMaximum("patientID");
    TString patient = "Patient " + TStringize(patientID);

    makeMLScatter(TStringize(i), (TCut) cut, patient);

    delete list;
    signalTree->SetEntryList(0);
  }

  return;
}

//______________________________________________________________
TEfficiency* comparisonPlots::computeEfficiency(TTree* tree, TString name, TCut cut) {

  TCut cBASE = "";
  TCut cEffCut = "probability>0.5"; //50% was determined to be optimal using optimalThreshold
  
  TH1F* hPassed = new TH1F("hPassed_"+name,"hPassed " + name + ";#it{t} [s];Probability", 25, 0., 900.);
  TH1F* hTotal = new TH1F("hTotal_"+name,"hTotal " + name + ";#it{t} [s];Probability", 25, 0., 900.);
  
  tree->Draw("timeToDetection>>hPassed_"+name, cBASE && cEffCut && cut);
  tree->Draw("timeToDetection>>hTotal_"+name, cBASE && cut);

  TEfficiency* eff = createEfficiency(hPassed, hTotal, name);
  
  delete hPassed;
  delete hTotal;
  
  return eff;
}

//______________________________________________________________
void comparisonPlots::makePatientLevel(TString name, pair<Double_t, Double_t> edges) {

  TString sigName = name;
  sigName += "sig";
  TH1F* hSig = new TH1F(sigName, ";Probability;Frequency", 16, edges.first, edges.second);

  TString bkgName = name;
  bkgName += "bkg";
  TH1F* hBkg = new TH1F(bkgName, ";Probability;Frequency", 16, edges.first, edges.second);
  
  signalTree->Draw(name+">>"+sigName);
  backgroundTree->Draw(name+">>"+bkgName);
  
  normalise(hSig);
  normalise(hBkg);
  
  makePlots(hSig, hBkg, name, "Arrhythmic", "Normal");
  
  return;
}

//______________________________________________________________
pair<Float_t, Float_t> comparisonPlots::computeParameters(TTree* tree, string name) {
  
  tree->SetEstimate(-1); // retain all data
  tree->Draw(name.c_str(), "", "goff"); // populate array
  Double_t* avals = tree->GetV1();
  vector<Float_t> vals(avals, avals+tree->GetSelectedRows());
  
  sort(vals.begin(), vals.end());
  
  Float_t min = vals[0] < 0. ? -vals[0] : 0.;
  pair<Float_t, Float_t> params = make_pair(min, vals[vals.size()-1]+min);
  
  avals = nullptr;
  
  return params;
}

//______________________________________________________________
void comparisonPlots::computeNormalisation(string MVA, Float_t& norm, Float_t& shift) {
  
  pair<Float_t, Float_t> sparams = computeParameters(signalTree, MVA);
  pair<Float_t, Float_t> bparams = computeParameters(backgroundTree, MVA);
  
  shift = sparams.first > bparams.first ? sparams.first : bparams.first;
  norm = sparams.second > bparams.second ? sparams.second : bparams.second;
  
  return;
}

//______________________________________________________________
void comparisonPlots::askAFriendVoter(TTree* tree, TH1F* hist, TCut cut) {
  
  string topMVA = "SVM";
  string secondBestMVA = "BDTD";
  
  Float_t norm;
  Float_t shift;
  computeNormalisation(topMVA, norm, shift);
  TCut topConfident = ("("+topMVA+">0.7)||("+topMVA+"<0.5)").c_str();
  tree->Draw(("("+topMVA+"+"+to_string(shift)+")/"+to_string(norm)+">>"+hist->GetName()).c_str(), topConfident && cut);
  
  computeNormalisation(secondBestMVA, norm, shift);
  TCut topUnsure = ("("+topMVA+"<0.7)&&("+topMVA+">0.5)").c_str();
  tree->Draw(("("+secondBestMVA+"+"+to_string(shift)+")/"+to_string(norm)+">>+"+hist->GetName()).c_str(), topUnsure && cut);
  
  return;
}

//______________________________________________________________
void comparisonPlots::makePatientLevelMIMIC() {
  
  TH1F* hSignalMVTDB = new TH1F("hSignalMVTDB", ";Probability;Frequency", 22, 0., 1.);
  TH1F* hSignalMIMIC = new TH1F("hSignalMIMIC", ";Probability;Frequency", 22, 0., 1.);
  TH1F* hSignalCUDB = new TH1F("hSignalCUDB", ";Probability;Frequency", 22, 0., 1.);
  TH1F* hBackground = new TH1F("hBackground", ";Probability;Frequency", 22, 0., 1.);
  
  askAFriendVoter(signalTree, hSignalMVTDB, "fileID<135");
  askAFriendVoter(signalTree, hSignalMIMIC, "fileID>134&&fileID<148");
  askAFriendVoter(signalTree, hSignalCUDB, "fileID>147&&fileID<166");
  askAFriendVoter(backgroundTree, hBackground, "");
  
  normalise(hSignalMVTDB);
  normalise(hSignalMIMIC);
  normalise(hSignalCUDB);
  normalise(hBackground);
  
  makePlots(hSignalMVTDB, hSignalMIMIC, hSignalCUDB, hBackground, "probDist", "ICD population", "Intensive care population", "Holter monitor population", "Control group");
  
  return;
}
