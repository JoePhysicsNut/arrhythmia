//
//doublePlot.cpp
//ArrhythmiaPrediction
//
//Created by Marek Sirendi on 12/05/2018.
//
//

#include "doublePlot.h"
#include "rootStyle.h"
#include "TROOT.h"
#include "TCanvas.h"
#include "TTree.h"
#include "TAxis.h"

//______________________________________________________________
doublePlot::doublePlot(const string& ecgInputFileName, const string& rrInputFileName, const string& outputFileName) {

  m_ecgInputFile = TFile::Open(ecgInputFileName.c_str(), "READ");
  m_rrInputFile = TFile::Open(rrInputFileName.c_str(), "READ");
  m_outputFileName = "Output/" + outputFileName;
  rootStyle();
}

//______________________________________________________________
doublePlot::~doublePlot() {
}

//______________________________________________________________
void doublePlot::initialize() {
  
  TTree* ecgTree = (TTree*) m_ecgInputFile->Get("signal");
  TTree* rrTree = (TTree*) m_rrInputFile->Get("signal");
  
  ecgTree->Draw("ECGval:time", "patientID==13&&time>424&&time<430", "goff");
  rrTree->Draw("BDT:time", "type==-1&&time>150&&time<310", "goff");
 
  m_ecg = new TGraph(ecgTree->GetSelectedRows(), ecgTree->GetV2(), ecgTree->GetV1());
  m_prediction = new TGraph(rrTree->GetSelectedRows(), rrTree->GetV2(), rrTree->GetV1());
  
  m_ecg->SetNameTitle("m_ecg", "m_ecg;#it{t} [s];ECG [mV]");
  m_prediction->SetNameTitle("m_prediction", "m_prediction;;Prob");
  
  return;
}

//______________________________________________________________
void doublePlot::finalize() {
  
  m_ecgInputFile->Close();
  m_rrInputFile->Close();
  
  return;
}

//______________________________________________________________
void doublePlot::makePlot() {

  TCanvas* canvas = new TCanvas(m_outputFileName.c_str(), m_outputFileName.c_str());
  canvas->Divide(1, 2, 0.1, 0.1);
  TPad* topPad = (TPad*) canvas->GetListOfPrimitives()->At(0);
  TPad* bottomPad = (TPad*) canvas->GetListOfPrimitives()->At(1);
  cosmeticsPads(topPad, bottomPad);

  cosmeticsTop(m_ecg);
  cosmeticsBottom(m_prediction);

  canvas->cd(1);
  m_ecg->Draw("AL");

  canvas->cd(2);
  m_prediction->Draw("AP");

  canvas->Print((m_outputFileName+".pdf").c_str());

  return;
}

//______________________________________________________________
void doublePlot::cosmeticsPads(TPad* top, TPad* bottom) {

  top->SetPad(0., 0.2, 1., 1.);
  top->SetBottomMargin(0.15);

  bottom->SetPad(0., 0., 1., 0.2);
  bottom->SetBottomMargin(0.25);
  bottom->SetTopMargin(0.08);

  return;
}

//______________________________________________________________
void doublePlot::cosmeticsTop(TGraph* graph) {

  graph->SetMarkerStyle(20);
  graph->SetLineColor(kBlack);
  
  //TAxis* xAxis = graph->GetXaxis();
  //xAxis->SetNdivisions(512);
  //xAxis->SetRangeUser(300., 301.);
  
  TAxis* yAxis = graph->GetYaxis();
  yAxis->CenterTitle(true);
  yAxis->SetRangeUser(-0.8, 2.5);
  
  return;
}

//______________________________________________________________
void doublePlot::cosmeticsBottom(TGraph* graph) {

  graph->SetLineColor(kBlack);
  graph->SetFillColor(kBlack);
  graph->SetMarkerSize(0.4);
  
  TAxis* xAxis = graph->GetXaxis();
  //xAxis->SetNdivisions(512);
  //xAxis->SetRangeUser(300., 301.);
  xAxis->SetTickLength(5.*xAxis->GetTickLength());
  xAxis->SetLabelSize(0.25);

  TAxis* yAxis = graph->GetYaxis();
  yAxis->SetRangeUser(0., 1.);
  yAxis->SetNdivisions(402);
  yAxis->SetLabelSize(0.25);
  yAxis->SetTitleSize(4.*yAxis->GetTitleSize());
  yAxis->SetTitleOffset(0.25*yAxis->GetTitleOffset());
  yAxis->CenterTitle(true);
  
  return;
}
