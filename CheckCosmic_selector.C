#define CheckCosmic_selector_cxx
// The class definition in CheckCosmic_selector.h has been generated automatically
// by the ROOT utility TTree::MakeSelector(). This class is derived
// from the ROOT class TSelector. For more information on the TSelector
// framework see $ROOTSYS/README/README.SELECTOR or the ROOT User Manual.


// The following methods are defined in this file:
//    Begin():        called every time a loop on the tree starts,
//                    a convenient place to create your histograms.
//    SlaveBegin():   called after Begin(), when on PROOF called only on the
//                    slave servers.
//    Process():      called for each event, in this function you decide what
//                    to read and fill your histograms.
//    SlaveTerminate: called at the end of the loop on the tree, when on PROOF
//                    called only on the slave servers.
//    Terminate():    called at the end of the loop on the tree,
//                    a convenient place to draw/fit your histograms.
//
// To use this file, try the following session on your Tree T:
//
// root> T->Process("CheckCosmic_selector.C")
// root> T->Process("CheckCosmic_selector.C","some options")
// root> T->Process("CheckCosmic_selector.C+")
//


#include "CheckCosmic_selector.h"
#include <TH2.h>
#include <TStyle.h>

#include <TH1.h>
#include <iostream>
#include <fstream>
#include <vector>

void CheckCosmic_selector::Begin(TTree * /*tree*/)
{
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();

   
  
}

void CheckCosmic_selector::SlaveBegin(TTree * /*tree*/)
{
  // The SlaveBegin() function is called after the Begin() function.
  // When running with PROOF SlaveBegin() is called on each slave server.
  // The tree argument is deprecated (on PROOF 0 is passed).
  events=0;
  good_events=0;
    
  TString option = GetOption();
  
}

Bool_t CheckCosmic_selector::Process(Long64_t entry)
{
  // The Process() function is called for each entry in the tree (or possibly
  // keyed object in the case of PROOF) to be processed. The entry argument
  // specifies which entry in the currently loaded tree is to be processed.
  // When processing keyed objects with PROOF, the object is already loaded
  // and is available via the fObject pointer.
  //
  // This function should contain the \"body\" of the analysis. It can contain
  // simple or elaborate selection criteria, run algorithms on the data
  // of the event and typically fill histograms.
  //
  // The processing can be stopped by calling Abort().
  //
  // Use fStatus to set the return value of TTree::Process().
  //
  // The return value is currently not used.
  
  
  fReader.SetLocalEntry(entry);
  
  /*if(entry==0){
    string file_name = std::to_string(*RunN)+".badEvents.dat";
    ifstream ifile;
    ifile.open(file_name.c_str());
    int tmp;
    while(ifile>>tmp){
      ifile>>tmp;
      bad_events.push_back(tmp);
    }
    ifile.close();
    bad_events.push_back(1000000000); //add a fake last entry
    }*/
  
  double E_ref=50;
  if(*Etot>E_ref){
    events++; //counting events 
    std::vector<int>::iterator it; 
    it = find (bad_events.begin(), bad_events.end(), *EventN); 
    if (it == bad_events.end()) {
      good_events++;
    }
  }

return kTRUE;
}

void CheckCosmic_selector::SlaveTerminate()
{
  // The SlaveTerminate() function is called after all entries or objects
  // have been processed. When running with PROOF SlaveTerminate() is called
  // on each slave server.

}

void CheckCosmic_selector::Terminate()
{
  // The Terminate() function is the last function to be called during
   // a query. It always runs on the client, it can be used to present
   // the results graphically or save the results to file.
}
