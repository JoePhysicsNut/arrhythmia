#include <string>
#include <iostream>
#include <iomanip>
#include <numeric>
#include <fstream>
#include "TMath.h"
#include "TFile.h"
#include "TH1F.h"
#include "TTree.h"
#include "TGraphAsymmErrors.h"
#include "TCut.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TPaveText.h"
#include "TLatex.h"
#include "TEfficiency.h"
#include "rootStyle.h"

using namespace std;

//______________________________________________________________
TGraphAsymmErrors* perfectPerformance() {
  
  Float_t x[1] = {0.0145};
  Float_t y[1] = {0.978};
  TGraphAsymmErrors* graph = new TGraphAsymmErrors(1, x, y);
  
  return graph;
}

//______________________________________________________________
void printContents(TGraphAsymmErrors* graph) {
  
  cout << fixed << showpoint;
  cout << setprecision(2);
  
  cout << "For graph " << graph->GetName() << endl;
  Int_t nBins = graph->GetN();
  for (Int_t i = 0; i < nBins; i++) {
    Double_t x, y;
    graph->GetPoint(i, x, y);
    cout << x << " " << y << endl;
  }
  
  return;
}

//______________________________________________________________
void printContents(TH1F* hist) {
  
  cout << fixed << showpoint;
  cout << setprecision(2);
  
  cout << "For hist: " << hist->GetName() << endl;
  
  Int_t nBins = hist->GetNbinsX();
  for (Int_t i = 0; i <= nBins; i++) {
    cout << hist->GetXaxis()->GetBinCenter(i) << " " << hist->GetBinContent(i) << endl;
  }
  
  return;
}

//______________________________________________________________
void normalise(TH1* hist) {

  hist->Scale(1./hist->GetSumOfWeights());
  Int_t dimension = hist->GetDimension();
  if (dimension == 1) hist->GetYaxis()->SetTitle("A.U.");
  if (dimension == 2) hist->GetZaxis()->SetTitle("A.U.");
  
  return;
}

//______________________________________________________________
TCanvas* makePlots(TH1* hist1, TH1* hist2, TString name, TString leg_first, TString leg_second, TString additionalLabel) {
  
  TCanvas* canv = new TCanvas(name, name);
  gStyle->SetEndErrorSize(4);
  hist1->SetLineWidth(2);
  hist1->SetMarkerStyle(20);
  hist1->SetLineColor(kSpring+2);
  hist1->SetMarkerColor(kSpring+2);
  
  hist2->SetLineWidth(2);
  hist2->SetMarkerStyle(26);
  hist2->SetLineColor(kBlue+2);
  hist2->SetMarkerColor(kBlue+2);
  hist1->SetStats(false);
  hist2->SetStats(false);

  Float_t max1 = hist1->GetMaximum();
  Float_t max2 = hist2->GetMaximum();
  
  if (max2 > max1) {
    hist2->GetYaxis()->SetRangeUser(0., 1.4*max2);
    hist2->Draw();
    hist1->Draw("same");
  } else {
    hist1->GetYaxis()->SetRangeUser(0., 1.4*max1);
    hist1->Draw();
    hist2->Draw("same");
  }
  
  TLegend* leg = new TLegend(0.6,0.7,0.93,0.87);
  leg->AddEntry(hist1, leg_first, "l");
  leg->AddEntry(hist2, leg_second, "l");
  leg->SetFillColor(kWhite);
  leg->SetFillStyle(0);
  leg->SetLineColor(kWhite);
  leg->SetLineWidth(0);
  leg->Draw();
  
  TPaveText* label = new TPaveText(gStyle->GetPadLeftMargin() + 0.05,
                                   0.85 - gStyle->GetPadTopMargin(),
                                   gStyle->GetPadLeftMargin() + 0.20,
                                   0.93 - gStyle->GetPadTopMargin(),
                                   "BRNDC");
  label->AddText(additionalLabel);
  label->SetFillColor(0);
  label->SetTextAlign(12);
  label->SetBorderSize(0);
  label->Draw();
  
  return canv;
}

//______________________________________________________________
void cosmetics(TGraphAsymmErrors* graph) {
  
  Int_t nBins = graph->GetN();
  for (Int_t i = 0; i < nBins; i++) {
    graph->SetPointError(i, 0., 0., 0., 0.);
  }
  
  graph->SetLineWidth(3);
  graph->SetLineColor(kAzure-3);
  graph->SetMarkerColor(kAzure-3);
  graph->SetFillColor(kAzure-3);
  
  return;
}

//______________________________________________________________
TCanvas* makePlots(TGraphAsymmErrors* hist1, TString name) {
  
  TCanvas* canv = new TCanvas(name, name);
  gStyle->SetEndErrorSize(4);
  hist1->SetLineWidth(4);
  hist1->SetMarkerStyle(20);
  hist1->SetLineColor(kAzure+10);
  hist1->SetMarkerColor(kAzure+10);
  hist1->SetFillColor(kAzure+10);
  hist1->SetFillStyle(3144);
  
  TGraphAsymmErrors* hist2 = perfectPerformance();
  hist2->SetLineWidth(4);
  hist2->SetMarkerStyle(34);
  hist2->SetLineColor(kSpring+2);
  hist2->SetMarkerColor(kSpring+2);
  hist2->SetMarkerSize(1.8*hist2->GetMarkerSize());
  
  hist1->SetMaximum(1.);
  hist1->SetMinimum(0.);
  hist1->GetXaxis()->SetRangeUser(0., 1.);
  hist1->Draw("A3");
  TGraphAsymmErrors* hist0 = (TGraphAsymmErrors*) hist1->Clone();
  cosmetics(hist0);
  hist0->Draw("lsame");
  hist2->Draw("psame");
  
  TLegend* leg = new TLegend(0.45,0.2,0.95,0.4);
  leg->AddEntry(hist0, "CodeRhythm", "l");
  leg->AddEntry(hist2, "Perfect performance", "p");
  leg->SetFillColor(kWhite);
  leg->SetFillStyle(0);
  leg->SetLineColor(kWhite);
  leg->SetLineWidth(0);
  leg->Draw();
  
  return canv;
}

//______________________________________________________________
TCanvas* makePlots(TGraphAsymmErrors* hist1, TGraphAsymmErrors* hist2, TString name, TString leg_first, TString leg_second, int side) {
  
  TCanvas* canv = new TCanvas(name, name);
  gStyle->SetEndErrorSize(4);
  hist1->SetLineWidth(4);
  hist1->SetMarkerStyle(20);
  hist1->SetLineColor(kSpring+2);
  hist1->SetMarkerColor(kSpring+2);
  hist1->SetFillColor(kSpring+2);
  hist1->SetFillStyle(3144);
  
  hist2->SetLineWidth(4);
  hist2->SetMarkerStyle(26);
  hist2->SetLineColor(kBlue+2);
  hist2->SetMarkerColor(kBlue+2);
  hist2->SetFillColor(kBlue+2);
  hist2->SetFillStyle(3144);
  
  hist1->SetMaximum(1.);
  hist1->SetMinimum(0.);
  hist1->GetXaxis()->SetRangeUser(0., 1.);
  hist1->Draw("A3");
  hist2->Draw("3same");
  
  TLegend* leg = nullptr;
  if (side == 1) leg = new TLegend(0.4,0.7,0.95,0.9);
  if (side == 2) leg = new TLegend(0.4,0.2,0.95,0.4);
  if (side == 3) leg = new TLegend(0.18,0.2,0.7,0.4);
  leg->AddEntry(hist1, leg_first, "l");
  leg->AddEntry(hist2, leg_second, "l");
  leg->SetFillColor(kWhite);
  leg->SetFillStyle(0);
  leg->SetLineColor(kWhite);
  leg->SetLineWidth(0);
  leg->Draw();
  
  return canv;
}

//______________________________________________________________
vector<Float_t> helper(Float_t num, Float_t denom) {
  
  Float_t center = 0.;
  Float_t up = 0.;
  Float_t down = 0.;
  
  if (num > 0. && denom > 0.) {
    center = num/denom;
    up = (Float_t) TEfficiency::ClopperPearson(denom, num, 0.95, 1);
    down = (Float_t) TEfficiency::ClopperPearson(denom, num, 0.95, 0);
  }
  
  vector<Float_t> result;
  result.push_back(center);
  result.push_back(up-center);
  result.push_back(center-down);
  
  return result;
}
  
//______________________________________________________________
vector<Float_t> computeAccuracy(Float_t TP, Float_t TN, Float_t FP, Float_t FN) {
  
  Float_t num = TP + TN;
  Float_t denom = TP + TN + FP + FN;
  
  return helper(num, denom);
}

//______________________________________________________________
vector<Float_t> computeSensitivity(Float_t TP, Float_t FN) {
  
  Float_t num = TP;
  Float_t denom = TP + FN;
  
  return helper(num, denom);
}

//______________________________________________________________
vector<Float_t> computeSpecificity(Float_t TN, Float_t FP) {

  Float_t num = TN;
  Float_t denom = TN + FP;
  
  return helper(num, denom);
}

//______________________________________________________________
vector<Float_t> computePrecision(Float_t TP, Float_t FP) { // Defn equivalent to Positive Predictive Value
  
  Float_t num = TP;
  Float_t denom = TP + FP;
  
  return helper(num, denom);
}

//______________________________________________________________
Float_t computeFScoreUncertainty(Float_t PR, Float_t PR_uncert, Float_t SE, Float_t SE_uncert) {
  
  Float_t preterm = 0.;
  Float_t posterm = 0.;
  
  Float_t preterm_denom = ((SE+PR)*(SE+PR));
  Float_t posterm_sq = PR_uncert*PR_uncert + SE_uncert*SE_uncert;
  
  if (preterm_denom > 0. && posterm_sq > 0.) {
    preterm = (2.*SE*SE)/preterm_denom;
    posterm = sqrt(posterm_sq);
  }
  
  return preterm*posterm;
}
  
//______________________________________________________________
vector<Float_t> computeFScore(Float_t PR, Float_t PR_up, Float_t PR_down, Float_t SE, Float_t SE_up, Float_t SE_down) {

  Float_t num = 2.*PR*SE;
  Float_t denom = PR + SE;
  Float_t fscore = 0.;
  if (denom > 0.) fscore = num/denom;
  Float_t fscore_up = computeFScoreUncertainty(PR, PR_up, SE, SE_up);
  Float_t fscore_down = computeFScoreUncertainty(PR, PR_down, SE, SE_down);
  
  vector<Float_t> result;
  result.push_back(fscore);
  result.push_back(fscore_up);
  result.push_back(fscore_down);
  
  return result;
}

//______________________________________________________________
vector<Float_t> computeRMSE(TH1F* hSignal, TH1F* hBackground, Float_t* x, Int_t classificationBin) {

  Int_t nBins = hSignal->GetNbinsX();

  Float_t nSignal = hSignal->Integral();
  Float_t nBackground = hBackground->Integral();
  Float_t sum = nSignal + nBackground;
  
  if (sum == 0.) {
    vector<Float_t> result;
    result.push_back(1.);
    result.push_back(0.);
    result.push_back(0.);
    return result;
  }
  
  // False negatives
  Float_t FNbrier = 0.;
  if (classificationBin > 0) {
    for (Int_t j = 1; j < classificationBin; j++) {
      Float_t nEvents = hSignal->GetBinContent(j);
      Float_t probability = hSignal->GetXaxis()->GetBinCenter(j);
      FNbrier += nEvents*(x[classificationBin] - probability)*(x[classificationBin] - probability); //distance to decision boundary (x[classificationBin]) weighted by the number of patients
    }
  }
  // False positives
  Float_t FPbrier = 0.;
  if (classificationBin < nBins) {
    for (Int_t k = classificationBin+1; k <= nBins; k++) {
      Float_t nEvents = hBackground->GetBinContent(k);
      Float_t probability = hBackground->GetXaxis()->GetBinCenter(k);
      FPbrier += nEvents*(probability - x[classificationBin])*(probability - x[classificationBin]);
    }
  }
  
  Float_t num = FNbrier + FPbrier;
  Float_t denom = sum;
  vector<Float_t> result = helper(num, denom);
  
  if (result[0] > 0.) {
    result[0] = sqrt(result[0]);
    result[1] = result[1]/(2.*result[0]);
    result[2] = result[2]/(2.*result[0]);
  }
  
  return result;
}

//______________________________________________________________
vector<TGraphAsymmErrors*> computeMetrics(TH1F* hSignal, TH1F* hBackground, TString name) {
  
  Int_t nBins = hSignal->GetNbinsX();
  Float_t x[nBins], accuracy[nBins], sensitivity[nBins], specificity[nBins], FPR[nBins], precision[nBins], fScore[nBins], RMSE[nBins];
  Float_t x_up[nBins], accuracy_up[nBins], sensitivity_up[nBins], specificity_up[nBins], FPR_up[nBins], precision_up[nBins], fScore_up[nBins], RMSE_up[nBins];
  Float_t x_down[nBins], accuracy_down[nBins], sensitivity_down[nBins], specificity_down[nBins], FPR_down[nBins], precision_down[nBins], fScore_down[nBins], RMSE_down[nBins];
  
  for (Int_t i = 0; i <= nBins; i++) {
    x[i] = hSignal->GetXaxis()->GetBinUpEdge(i);
    x_up[i] = (hSignal->GetXaxis()->GetBinWidth(i))/2.;
    x_down[i] = x_up[i];
  }
  
  /* Metrics explained
  computeAccuracy(Float_t TP, Float_t TN, Float_t FP, Float_t FN)
  computeSensitivity(Float_t TP, Float_t FN)
  computeSpecificity(Float_t TN, Float_t FP)
  computePrecision(Float_t TP, Float_t FP)
  computeFScore(Float_t PR, Float_t SE)
  computeRMSE(TH1F* hSignal, TH1F* hBackground, Float_t* x, Int_t classificationBin)
  */
  
  for (Int_t i = 1; i <= nBins; i++) {
    Float_t TP = hSignal->Integral(i+1, nBins);
    Float_t TN = hBackground->Integral(1, i);
    Float_t FP = hBackground->Integral(i+1, nBins);
    Float_t FN = hSignal->Integral(1, i);
    
    vector<Float_t> result = computeAccuracy(TP, TN, FP, FN);
    accuracy[i-1] = result[0];
    accuracy_up[i-1] = result[1];
    accuracy_down[i-1] = result[2];
    vector<Float_t>().swap(result);
    
    result = computeSensitivity(TP, FN);
    sensitivity[i-1] = result[0];
    sensitivity_up[i-1] = result[1];
    sensitivity_down[i-1] = result[2];
    vector<Float_t>().swap(result);
    
    result = computeSpecificity(TN, FP);
    specificity[i-1] = result[0];
    specificity_up[i-1] = result[1];
    specificity_down[i-1] = result[2];
    vector<Float_t>().swap(result);
    
    FPR[i-1] = 1. - specificity[i-1];
    FPR_up[i-1] = specificity_up[i-1];
    FPR_down[i-1] = specificity_down[i-1];
    
    result = computePrecision(TP, FP);
    precision[i-1] = result[0];
    precision_up[i-1] = result[1];
    precision_down[i-1] = result[2];
    vector<Float_t>().swap(result);
    
    result = computeFScore(precision[i-1], precision_up[i-1], precision_down[i-1], sensitivity[i-1], sensitivity_up[i-1], sensitivity_down[i-1]);
    fScore[i-1] = result[0];
    fScore_up[i-1] = result[1];
    fScore_down[i-1] = result[2];
    vector<Float_t>().swap(result);
    
    result = computeRMSE(hSignal, hBackground, x, i);
    RMSE[i-1] = result[0];
    RMSE_up[i-1] = result[1];
    RMSE_down[i-1] = result[2];
  }
  
  vector<TGraphAsymmErrors*> gMetrics;
  gMetrics.push_back(new TGraphAsymmErrors(nBins, x, accuracy, x_down, x_up, accuracy_down, accuracy_up));
  gMetrics[0]->SetNameTitle(name+"accuracy",";Probability;Accuracy");
  gMetrics.push_back(new TGraphAsymmErrors(nBins, x, sensitivity, x_down, x_up, sensitivity_down, sensitivity_up));
  gMetrics[1]->SetNameTitle(name+"sensitivity",";Probability;Sensitivity");
  gMetrics.push_back(new TGraphAsymmErrors(nBins, x, specificity, x_down, x_up, specificity_down, specificity_up));
  gMetrics[2]->SetNameTitle(name+"specificity",";Probability;Specificity");
  gMetrics.push_back(new TGraphAsymmErrors(nBins, x, precision, x_down, x_up, precision_down, precision_up));
  gMetrics[3]->SetNameTitle(name+"precision",";Probability;Precision");
  gMetrics.push_back(new TGraphAsymmErrors(nBins, x, fScore, x_down, x_up, fScore_down, fScore_up));
  gMetrics[4]->SetNameTitle(name+"fScore",";Probability;F-score");
  gMetrics.push_back(new TGraphAsymmErrors(nBins, x, RMSE, x_down, x_up, RMSE_down, RMSE_up));
  gMetrics[5]->SetNameTitle(name+"RMSE",";Probability;RMSE");
  gMetrics.push_back(new TGraphAsymmErrors(nBins, FPR, sensitivity, FPR_down, FPR_up, sensitivity_down, sensitivity_up));
  gMetrics[6]->SetNameTitle(name+"ROC",";False Positive Rate;True Positive Rate");
  
  return gMetrics;
}

//______________________________________________________________
vector<TCanvas*> createCanvases(vector<TGraphAsymmErrors*> vgOne, vector<TGraphAsymmErrors*> vgTwo) {

  vector<int> sides;
  sides.push_back(2);
  sides.push_back(3);
  sides.push_back(2);
  sides.push_back(2);
  sides.push_back(3);
  sides.push_back(1);
  sides.push_back(2);

  vector<TCanvas*> canvases;
  for (unsigned int i = 0; i < vgOne.size(); i++) {
    canvases.push_back(makePlots(vgOne[i], vgTwo[i], vgOne[i]->GetName(), "Boosted decision tree", "Committee classifier", sides[i]));
  }
  
  return canvases;
}

//______________________________________________________________
void printCanvases(vector<TCanvas*> canvases, string outputFileName) {
  
  string mainName = "Output/" + outputFileName + ".pdf";
  string startName = mainName + "(";
  string endName = mainName + ")";
  
  if (canvases.size() > 2) {
    canvases[0]->Print(startName.c_str());
    
    for (unsigned int i = 1; i < (canvases.size()-1); i++) {
      if (canvases[i]) canvases[i]->Print(mainName.c_str());
    }
    
    canvases[canvases.size()-1]->Print(endName.c_str());
  } else if (canvases.size() == 2) {
    canvases[0]->Print(startName.c_str());
    canvases[canvases.size()-1]->Print(endName.c_str());
  } else canvases[0]->Print(mainName.c_str());
  
  return;
}

//______________________________________________________________
Float_t getYval(TGraphAsymmErrors* graph, Int_t bin) {

  Double_t x, y;
  graph->GetPoint(bin, x, y);
  
  return y;
}

//______________________________________________________________
pair<Float_t, Float_t> extractUncertainties(Float_t yVal, TGraphAsymmErrors* graph) {
  
  Float_t up = -1.;
  Float_t down = -1.;
  
  Int_t nBins = graph->GetN();
  for (Int_t i = 0; i < nBins; i++) {
    Double_t x, y;
    graph->GetPoint(i, x, y);
    if (y == yVal) {
      up = graph->GetErrorYhigh(i);
      down = graph->GetErrorYlow(i);
    }
  }
  
  return make_pair(up, down);
}

//______________________________________________________________
pair<Float_t, Float_t> computeROCUncertainties(TGraphAsymmErrors* graph) {
  
  TString name = graph->GetName();
  TString uname = name + "_up";
  TString dname = name + "_down";
  
  TGraphAsymmErrors* graph_up = (TGraphAsymmErrors*) graph->Clone(uname);
  TGraphAsymmErrors* graph_down = (TGraphAsymmErrors*) graph->Clone(dname);
  
  Int_t nBins = graph->GetN();
  for (Int_t i = 0; i < nBins; i++) {
    Double_t x, y;
    graph->GetPoint(i, x, y);
    Double_t up = graph->GetErrorYhigh(i);
    Double_t down = graph->GetErrorYlow(i);
    
    graph_up->SetPoint(i, x, y+up);
    graph_down->SetPoint(i, x, y-down);
  }
  
  Float_t middle = (Float_t) graph->Integral();
  Float_t up = (Float_t) graph_up->Integral() - middle;
  Float_t down = middle - (Float_t) graph_down->Integral();
  
  return make_pair(up, down);
}

//______________________________________________________________
vector<Float_t> ROCmetrics(vector<TGraphAsymmErrors*> gMetrics, string type) {
  
  Int_t nBins = gMetrics[0]->GetN();
  vector<Float_t> vROCmetrics;
  vROCmetrics.push_back(TMath::MaxElement(nBins, gMetrics[0]->GetY()));
  vROCmetrics.push_back(TMath::MaxElement(nBins, gMetrics[1]->GetY()));
  vROCmetrics.push_back(TMath::MaxElement(nBins, gMetrics[2]->GetY()));
  vROCmetrics.push_back(TMath::MaxElement(nBins, gMetrics[3]->GetY()));
  vROCmetrics.push_back(TMath::MaxElement(nBins, gMetrics[4]->GetY()));
  vROCmetrics.push_back(TMath::MinElement(nBins, gMetrics[5]->GetY()));
  vROCmetrics.push_back(0.5+gMetrics[6]->Integral());
  
  vector<pair<Float_t, Float_t>> vROCmetrics_uncert;
  for (unsigned int i = 0; i < (vROCmetrics.size()-1); i++) {
    vROCmetrics_uncert.push_back(extractUncertainties(vROCmetrics[i], gMetrics[i]));
  }
  
  vROCmetrics_uncert.push_back(computeROCUncertainties(gMetrics[vROCmetrics.size()-1]));
  
  Float_t optimalFraction = 0.;
  Int_t optimalBin = -1;
  for (Int_t i = 0; i < nBins; i++) {
    Double_t x, y;
    gMetrics[5]->GetPoint(i, x, y);
    if (y == vROCmetrics[5]) {
      optimalFraction = x;
      optimalBin = i;
    }
  }
  vROCmetrics.push_back(optimalFraction);
  
  // Find A, SE, SP, PE, F-score at the optimal working point
  vector<Float_t> vROCmetrics_optimalWP;
  for (unsigned int i = 0; i < 5; i++) {
    vROCmetrics_optimalWP.push_back(getYval(gMetrics[i], optimalBin));
  }
  
  vector<pair<Float_t, Float_t>> vROCmetrics_optimalWP_uncert;
  for (unsigned int i = 0; i < vROCmetrics_optimalWP.size(); i++) {
    vROCmetrics_optimalWP_uncert.push_back(extractUncertainties(vROCmetrics_optimalWP[i], gMetrics[i]));
  }
  
  ofstream fout;
  fout.open("Output/ROCmetrics_"+type+".txt");
  
  fout << fixed << showpoint;
  fout << setprecision(1);

  fout << "For case: " << type << endl;
  fout << "Accuracy: " << 100.*vROCmetrics_optimalWP[0] << "% +" << 100.*vROCmetrics_optimalWP_uncert[0].first << "% -" << 100.*vROCmetrics_optimalWP_uncert[0].second << "% (Max: " << 100.*vROCmetrics[0] << "% +" << 100.*vROCmetrics_uncert[0].first << "% -" << 100.*vROCmetrics_uncert[0].second << "%)" << endl;
  fout << "Sensitivity: " << 100.*vROCmetrics_optimalWP[1] << "% +" << 100.*vROCmetrics_optimalWP_uncert[1].first << "% -" << 100.*vROCmetrics_optimalWP_uncert[1].second << "% (Max: " << 100.*vROCmetrics[1] << "% +" << 100.*vROCmetrics_uncert[1].first << "% -" << 100.*vROCmetrics_uncert[1].second << "%)" << endl;
  fout << "Specificity: " << 100.*vROCmetrics_optimalWP[2] << "% +" << 100.*vROCmetrics_optimalWP_uncert[2].first << "% -" << 100.*vROCmetrics_optimalWP_uncert[2].second << "% (Max: " << 100.*vROCmetrics[2] << "% +" << 100.*vROCmetrics_uncert[2].first << "% -" << 100.*vROCmetrics_uncert[2].second << "%)" << endl;
  fout << "Precision: " << 100.*vROCmetrics_optimalWP[3] << "% +" << 100.*vROCmetrics_optimalWP_uncert[3].first << "% -" << 100.*vROCmetrics_optimalWP_uncert[3].second << "% (Max: " << 100.*vROCmetrics[3] << "% +" << 100.*vROCmetrics_uncert[3].first << "% -" << 100.*vROCmetrics_uncert[3].second << "%)" << endl;
  fout << setprecision(2);
  fout << "F-score: " << vROCmetrics_optimalWP[4] << " +" << vROCmetrics_optimalWP_uncert[4].first << " -" << vROCmetrics_optimalWP_uncert[4].second << " (Max: " << vROCmetrics[4] << " +" << vROCmetrics_uncert[4].first << " -" << vROCmetrics_uncert[4].second << ")" << endl;
  fout << "RMSE: " << vROCmetrics[5] << " +" << vROCmetrics_uncert[5].first << " -" << vROCmetrics_uncert[5].second << endl;
  fout << "AUROC: " << vROCmetrics[6] << " +" << vROCmetrics_uncert[6].first << " -" << vROCmetrics_uncert[6].second << endl;
  fout << "Optimal fraction: " << vROCmetrics[7] << endl;
  
  fout.close();
  
  return vROCmetrics;
}

//______________________________________________________________
pair<Float_t, Float_t> computeParameters(TTree* tree, string name) {
  
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
void computeNormalisation(TTree* signalTree, TTree* backgroundTree, string MVA, Float_t& norm, Float_t& shift) {

  pair<Float_t, Float_t> sparams = computeParameters(signalTree, MVA);
  pair<Float_t, Float_t> bparams = computeParameters(backgroundTree, MVA);

  shift = sparams.first > bparams.first ? sparams.first : bparams.first;
  norm = sparams.second > bparams.second ? sparams.second : bparams.second;

  return;
}

//______________________________________________________________
void askAFriendVoter(TTree* signalTree, TTree* backgroundTree) {

  string topMVA = "BDT";
  string secondBest = "MLPBNN";

  Float_t norm;
  Float_t shift;
  computeNormalisation(signalTree, backgroundTree, topMVA, norm, shift);
  TCut topConfident = ("("+topMVA+">-1.)||("+topMVA+"<-0.9)").c_str();
  signalTree->Draw(("("+topMVA+"+"+to_string(shift)+")/"+to_string(norm)+">>hSHybrid").c_str(), topConfident);
  backgroundTree->Draw(("("+topMVA+"+"+to_string(shift)+")/"+to_string(norm)+">>hBHybrid").c_str(), topConfident);

  computeNormalisation(signalTree, backgroundTree, secondBest, norm, shift);
  TCut topUnsure = ("("+topMVA+"<1.)&&("+topMVA+">-0.9)").c_str();
  signalTree->Draw(("("+secondBest+"+"+to_string(shift)+")/"+to_string(norm)+">>+hSHybrid").c_str(), topUnsure);
  backgroundTree->Draw(("("+secondBest+"+"+to_string(shift)+")/"+to_string(norm)+">>+hBHybrid").c_str(), topUnsure);

  return;
}

//______________________________________________________________
void committeeVoter(TTree* signalTree, TTree* backgroundTree, TH1F*& hSignal, TH1F*& hBackground, vector<string> MVAs, vector<Float_t> vOptimalFraction, vector<Float_t> vROC) {
  
  Float_t sum = accumulate(vROC.begin(), vROC.end(), 0.);
  
  Float_t signalVals[MVAs.size()];
  Float_t backgroundVals[MVAs.size()];
  for (unsigned int i = 0; i < MVAs.size(); i++) {
    signalTree->SetBranchAddress(MVAs[i].c_str(), &signalVals[i]);
    backgroundTree->SetBranchAddress(MVAs[i].c_str(), &backgroundVals[i]);
  }
  
  Long64_t nentries = signalTree->GetEntries();
  for (Long64_t i = 0; i < nentries; i++) {
    signalTree->GetEntry(i);
    backgroundTree->GetEntry(i);
    Float_t signalCommitteeProb = 0.;
    Float_t backgroundCommitteeProb = 0.;
  
    for (unsigned int j = 0; j < MVAs.size(); j++) {
      Float_t signalVal = signalVals[j];
      Float_t backgroundVal = backgroundVals[j];
      Float_t norm;
      Float_t shift;
      computeNormalisation(signalTree, backgroundTree, MVAs[j], norm, shift);
      
      Float_t signalProb = (signalVal+shift)/norm;
      if (signalProb > vOptimalFraction[j]) signalCommitteeProb += vROC[j]/sum;
      
      Float_t backgroundProb = (backgroundVal+shift)/norm;
      if (backgroundProb > vOptimalFraction[j]) backgroundCommitteeProb += vROC[j]/sum;
    }
    
    hSignal->Fill(signalCommitteeProb);
    hBackground->Fill(backgroundCommitteeProb);
  }
  
  return;
}

//______________________________________________________________
int main(int argc, char ** argv) {
  
  if (argc != 2 && argc != 3) {
    cout << "Usage: " << argv[0] << "  Input file" << "  Cut (optional)" << endl;
    return 0;
  }
  
  char* inFile = argv[1];
  cout << "Input file: " << inFile << endl;
  
  rootStyle();
  
  TFile* inputFile = TFile::Open(inFile);
  
  TTree* fullSignalTree = (TTree*) inputFile->Get("signal");
  TTree* fullBackgroundTree = (TTree*) inputFile->Get("background");
  
  TTree* signalTree = nullptr;
  TTree* backgroundTree = nullptr;
  
  char* ccut = argv[2];
  if (ccut) {
    TCut cut = ccut;
    cout << "Cut: " << cut << endl;
    signalTree = fullSignalTree->CopyTree(cut);
    backgroundTree = fullBackgroundTree->CopyTree(cut);
  } else {
    signalTree = fullSignalTree;
    backgroundTree = fullBackgroundTree;
  }
  
  vector<string> MVAs;
  //Patient-level
  MVAs.push_back("BDT");
  MVAs.push_back("BDTG");
  MVAs.push_back("BDTD");
  //MVAs.push_back("LikelihoodMIX");
  //MVAs.push_back("SVM");
  //MVAs.push_back("KNN");
  //MVAs.push_back("prob_LD");
  //MVAs.push_back("FDA_SA");
  MVAs.push_back("MLP");
  MVAs.push_back("MLPBNN");
  
  //Beat-level
  MVAs.push_back("meansBDT");
  MVAs.push_back("meansBDTG");
  MVAs.push_back("meansBDTD");
  //MVAs.push_back("meansLikelihoodMIX");
  //MVAs.push_back("meansSVM");
  //MVAs.push_back("meansKNN");
  //MVAs.push_back("meansprob_LD");
  //MVAs.push_back("meansFDA_SA");
  MVAs.push_back("meansMLP");
  MVAs.push_back("meansMLPBNN");
  //MVAs.push_back("meansnnet1");
  //MVAs.push_back("meansnnet2");
  //MVAs.push_back("meansnnet3");
  
  vector<TH1F*> vSignal;
  vector<TH1F*> vBackground;
  vector<TCanvas*> vProbabilityCanvases;
  vector<vector<TGraphAsymmErrors*>> vvMetrics;
  vector<vector<Float_t>> vvROCmetrics;
  
  for (unsigned int i = 0; i < MVAs.size(); i++) {
    vSignal.push_back(new TH1F(("hSignal_" + MVAs[i]).c_str(),("hSignal " + MVAs[i] + ";Probability;# of events").c_str(), 100, -0.1, 1.1));
    vBackground.push_back(new TH1F(("hBackground_" + MVAs[i]).c_str(),("hBackground " + MVAs[i] + ";Probability;# of events").c_str(), 100, -0.1, 1.1));
    
    Float_t norm;
    Float_t shift;
    computeNormalisation(signalTree, backgroundTree, MVAs[i], norm, shift);
    signalTree->Draw(("("+MVAs[i]+"+"+to_string(shift)+")/"+to_string(norm)+">>hSignal_"+MVAs[i]).c_str(), "");
    backgroundTree->Draw(("("+MVAs[i]+"+"+to_string(shift)+")/"+to_string(norm)+">>hBackground_"+MVAs[i]).c_str(), "");
    
    vvMetrics.push_back(computeMetrics(vSignal[i], vBackground[i], "g"+MVAs[i]));
    vvROCmetrics.push_back(ROCmetrics(vvMetrics[i], MVAs[i]));
  }
  
  for (unsigned int i = 0; i < MVAs.size(); i++) {
    vProbabilityCanvases.push_back(makePlots(vSignal[i], vBackground[i], "h"+MVAs[i], "Arrhythmic", "Normal", MVAs[i]));
  }
  
   printCanvases(vProbabilityCanvases, "ROCmetrics_ProbDists");
  
  // Rank classifiers
  vector<pair<Float_t,string>> performanceMethodPair;
  for (unsigned int i = 0; i < MVAs.size(); i++) {
    performanceMethodPair.push_back(make_pair(vvROCmetrics[i][6], MVAs[i])); //[6] is AUROC
  }
  
  sort(performanceMethodPair.begin(), performanceMethodPair.end());
  
  ofstream fout;
  fout.open("Output/ROCmetrics_Overview.txt");
  fout << fixed << showpoint;
  fout << setprecision(2);
  
  for (unsigned int i = 0; i < performanceMethodPair.size(); i++) {
    fout << performanceMethodPair[i].second  << "\t" << performanceMethodPair[i].first << endl;
  }
  
  // Combine classifiers
  // Hybrid
  TH1F* hSHybrid = new TH1F("hSHybrid", "hSHybrid;Probability;# of events", 100, -0.1, 1.1);
  TH1F* hBHybrid = new TH1F("hBHybrid", "hBHybrid;Probability;# of events", 100, -0.1, 1.1);
  
  askAFriendVoter(signalTree, backgroundTree);
  
  TCanvas* cHybrid = makePlots(hSHybrid, hBHybrid, "hHybrid", "Arrhythmic", "Normal", "Hybrid");
  cHybrid->Print("Output/ROCmetrics_Hybrid.pdf");
  
  vector<TGraphAsymmErrors*> vHybridMetrics = computeMetrics(hSHybrid, hBHybrid, "gHybrid");
  vector<Float_t> vHybridROCmetrics = ROCmetrics(vHybridMetrics, "Hybrid");
  
  fout << "Hybrid\t" << vHybridROCmetrics[6] << endl; //[6] is AUROC

  // Committee
  // Store optimal bins in a separate vector
  vector<Float_t> vOptimalFraction;
  for (unsigned int i = 0; i < vvROCmetrics.size(); i++) {
    vOptimalFraction.push_back(vvROCmetrics[i][7]);
  }
  
  // Store weighted AUROC in a separate vector
  vector<Float_t> vwROC;
  for (unsigned int i = 0; i < vvROCmetrics.size(); i++) {
    vwROC.push_back(pow(vvROCmetrics[i][6],35.));
  }
  
  TH1F* hSCommittee = new TH1F("hSCommittee", "hSCommittee;Probability;# of events", 100, -0.1, 1.1);
  TH1F* hBCommittee = new TH1F("hBCommittee", "hBCommittee;Probability;# of events", 100, -0.1, 1.1);
  
  committeeVoter(signalTree, backgroundTree, hSCommittee, hBCommittee, MVAs, vOptimalFraction, vwROC);
  
  TCanvas* cCommittee = makePlots(hSCommittee, hBCommittee, "hCommittee", "Arrhythmic", "Normal", "Committee");
  cCommittee->Print("Output/ROCmetrics_Committee.pdf");
  
  vector<TGraphAsymmErrors*> vCommitteeMetrics = computeMetrics(hSCommittee, hBCommittee, "gCommittee");
  vector<Float_t> vCommitteeROCmetrics = ROCmetrics(vCommitteeMetrics, "Committee");
  
  fout << "Committee\t" << vCommitteeROCmetrics[6] << endl; //[6] is AUROC

  fout.close();
  
  vector<TCanvas*> canvases = createCanvases(vvMetrics[0], vCommitteeMetrics);
  TCanvas* cROC_perfect = makePlots(vCommitteeMetrics[6], "cROC_perfect"); //[6] is ROC
  canvases.push_back(cROC_perfect);
  
  printCanvases(canvases, "ROCmetrics");
  
  TFile* outFile = new TFile("Output/ROCmetrics.root", "RECREATE");
  outFile->cd();
  for (unsigned int i = 0; i < vProbabilityCanvases.size(); i++) {
    vProbabilityCanvases[i]->Write();
    cHybrid->Write();
    cCommittee->Write();
  }
  for (unsigned int i = 0; i < canvases.size(); i++) {
    canvases[i]->Write();
  }

  inputFile->Close();
  outFile->Close();
  
  return 0;
}
