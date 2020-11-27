//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu Nov 26 18:00:53 2020 by ROOT version 6.22/04
// from TTree tOut/tOut
// found on file: 1331.outDST.root
//////////////////////////////////////////////////////////

#ifndef CheckCosmic_selector_h
#define CheckCosmic_selector_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <TTreeReaderArray.h>
using namespace std;
// Headers needed by this particular selector


class CheckCosmic_selector : public TSelector {
public :
   TTreeReader     fReader;  //!the tree reader
   TTree          *fChain = 0;   //!pointer to the analyzed TTree or TChain

   // Readers to access the data (delete the ones you do not need).
   TTreeReaderValue<Int_t> RunN = {fReader, "RunN"};
   TTreeReaderValue<Int_t> EventN = {fReader, "EventN"};
   TTreeReaderValue<ULong64_t> EventTime = {fReader, "EventTime"};
   TTreeReaderValue<Int_t> EventType = {fReader, "EventType"};
   TTreeReaderValue<Int_t> Multiplicity = {fReader, "Multiplicity"};
   TTreeReaderValue<Double_t> Weight = {fReader, "Weight"};
   TTreeReaderValue<Double_t> Etot = {fReader, "Etot"};
   TTreeReaderValue<Double_t> Eseed = {fReader, "Eseed"};


   CheckCosmic_selector(TTree * /*tree*/ =0) {
       
       good_events=0;
       
   }
   virtual ~CheckCosmic_selector() { }
   virtual Int_t   Version() const { return 2; }
   virtual void    Begin(TTree *tree);
   virtual void    SlaveBegin(TTree *tree);
   virtual void    Init(TTree *tree);
   virtual Bool_t  Notify();
   virtual Bool_t  Process(Long64_t entry);
   virtual Int_t   GetEntry(Long64_t entry, Int_t getall = 0) { return fChain ? fChain->GetTree()->GetEntry(entry, getall) : 0; }
   virtual void    SetOption(const char *option) { fOption = option; }
   virtual void    SetObject(TObject *obj) { fObject = obj; }
   virtual void    SetInputList(TList *input) { fInput = input; }
   virtual TList  *GetOutputList() const { return fOutput; }
   virtual void    SlaveTerminate();
   virtual void    Terminate();

      
    vector<int> badevents;
    int good_events;
    
   ClassDef(CheckCosmic_selector,0);

};

#endif

#ifdef CheckCosmic_selector_cxx
void CheckCosmic_selector::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the reader is initialized.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   fReader.SetTree(tree);
}

Bool_t CheckCosmic_selector::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}


#endif // #ifdef CheckCosmic_selector_cxx
