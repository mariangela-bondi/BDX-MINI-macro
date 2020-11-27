
#include "TTree.h"
#include "TFile.h"
#include <TStyle.h>
#include <TLine.h>
#include <iostream>
#include <string>
#include <vector>
#include "TMath.h"
#include "TApplication.h"
#include "TCanvas.h"
#include "TSystem.h"
#include "TLegend.h"
#include "TROOT.h"
#include "TProof.h"
#include "TF1.h"
#include "TH1.h"
#include <TGraphErrors.h>
#include "CheckCosmic_selector.h"
using namespace std;

int main(){
        // run index -> bisogna aggiungere i run della presa dati Agosto -Settembre
//    int runs[26]={1324,1325,1326,1328,1329,1330,1331,1332,1333,1334,1335,1336,1337,1338,1339,1342,1343 ,1344,1347,1348,1349,1350,1351,1352,1353,1354};
    int runs[2]={1331,1411};
   
    TGraphErrors *g=new TGraphErrors();
  //for (int ii=0;ii<26;ii++){
    for (int ii=0;ii<2;ii++){
        cout << "********* Run  "<<runs[ii]<<" ************"<<endl;
        /* Leggo i Files necessari per l'analisi*/
      TFile *f=new TFile(Form("/Users/Mariangela/work/BDX-MINI/cosmicCheck/%i.outDST.root",runs[ii]));  // legge il file root : bisogna cambiare percorso
        TH1D *h=(TH1D*)f->Get("hControl");
        TTree *t=(TTree*)f->Get("tOut");
    ifstream fbad(Form("/Users/Mariangela/work/BDX-MINI/cosmicCheck/%i.badEvents.dat",runs[ii])); // legge il file con la lista degli event NUmber degli eventi di rumore: bisogna cambiare percorso

        CheckCosmic_selector *mySelector = new CheckCosmic_selector();
        
        
        /* riempio gli elemnti del vettore vec_badEvents con gli eventNumber degli eventi cattivi*/
        vector<int> vec_badEvents;
        int n1, n2;
        if(!fbad) { // file couldn't be opened
           cerr << "Error: file could not be opened" << endl;
           exit(1);
        }
        while ( !fbad.eof() ) { // keep reading until end-of-file
        fbad>>n1>>n2;
            vec_badEvents.push_back(n2);
      //      cout << n1<< " "<<n2<<endl;
        }
        
        /*  Selector per contare gli eventi buoni con Etot >50 */
        /* Controllo che ogni evento Etot>50 non sia un bad event: controllo che il suo event Number non sia un elemento del vettore badEvents*/
  
        mySelector->badevents =vec_badEvents;
        t->Process(mySelector);
        // numero di eventi buoni
        int good_events = mySelector->good_events;
        cout << "GOOD events "<< good_events<<endl;

      double Toff=h->GetBinContent(5);
      double Ton=h->GetBinContent(3);
      int N=t->GetEntries("(Etot>50)");    // conto tutti gli eventi con Etot > 50 ma senza verificare se tra di essi ci sono badEvents (eventi di rumore)

        
      g->SetPoint(ii,ii,N/Toff);
      g->SetPointError(ii,0,sqrt(N)/Toff);
      
      cout<<ii<<" "<<runs[ii]<<" "<<N<<" "<<Toff<<" "<<Ton<<" "<<endl;
        delete mySelector;

    }

    // plot graphs
    g->Draw("AP");
    
    // save graphs in a root file
    
    
    
} //end main()
