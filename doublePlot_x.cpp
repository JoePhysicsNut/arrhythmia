//
//  doublePlot_x.cpp
//

#include "doublePlot.h"
#include <string>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

int main (int argc, char ** argv) {
  
  if (argc != 4) {
    cout << "Usage: " << argv[0] << "  ECG file" << "  RR file" << "  outputFileName" << endl;
    return 0;
  }
  
  char* ecginFile = argv[1];
  string ecginputFileName = ecginFile;
  cout << "ECG input file: " << ecginputFileName << endl;
  
  char* rrinFile = argv[2];
  string rrinputFileName = rrinFile;
  cout << "RR input file: " << rrinputFileName << endl;
  
  char* outFile = argv[3];
  string outputFileName = outFile;
  cout << "Output file: " << outputFileName << endl;
  
  //Create the analysis object
  doublePlot* plot = new doublePlot(ecginputFileName, rrinputFileName, outputFileName);
  
  cout << "Initializing" << endl;
  plot->initialize();
  
  cout << "Creating the plot" << endl;
  plot->makePlot();
  
  cout << "Finalizing" << endl;
  plot->finalize();
  
  delete plot;
  
  return 0;
}
