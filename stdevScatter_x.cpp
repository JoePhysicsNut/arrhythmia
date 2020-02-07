//
//  comparisonPlots_x.cpp
//

#include "comparisonPlots.h"
#include <string>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

int main (int argc, char ** argv) {

  if (argc != 2) {
    cout << "Usage: " << argv[0] << "  Inputfilename" << endl;
    return 0;
  }
  
  char* inFile = argv[1];
  string inputFileName = inFile;
  cout << "Input file: " << inputFileName << endl;
  
  //Create the analysis object
  comparisonPlots* cPlots = new comparisonPlots(inputFileName);
  
  cout << "Initializing" << endl;
  cPlots->initialize();
  
  cout << "Making the Stdev scatter plot" << endl;
  cPlots->makeStdevScatter();
  
  cout << "Finalizing" << endl;
  cPlots->finalize();
  
  delete cPlots;
  
  return 0;
}
