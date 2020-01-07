//
//  doublePlot.h
//  ArrhythmiaPrediction
//
//  Created by Marek Sirendi on 12/05/2018.
//
//

#ifndef __ArrhythmiaPrediction__doublePlot__
#define __ArrhythmiaPrediction__doublePlot__

#include "TFile.h"
#include "TGraph.h"
#include "TPaveText.h"
#include "TStyle.h"
#include "TPad.h"

using namespace std;

class doublePlot {
public:
  //Constructor & destructor
  doublePlot(const string& ecgInputFileName, const string& rrInputFileName, const string& outputFileName);
  ~doublePlot();
  
  void initialize();
  void makePlot();
  void finalize();
  
private:

  TFile* m_ecgInputFile;
  TFile* m_rrInputFile;
  string m_outputFileName;
  
  TGraph* m_ecg;
  TGraph* m_prediction;
  
  void cosmeticsPads(TPad* top, TPad* bottom);
  void cosmeticsTop(TGraph* graph);
  void cosmeticsBottom(TGraph* graph);
  void adjustAxes(TGraph* graph);
};

#endif /* defined(__ArrhythmiaPrediction__doublePlot__) */