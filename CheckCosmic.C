
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
  int runs[39]={1324,1325,1326,1328,1329,1330,1331,1332,1333,1334,1335,1336,1337,1338,1339,1342,1343,1344,1347,1348,1349,1350,1351,1352,1353,1354,1402,1403,1404,1405,1406,1407,1408,1409,1410,1411,1412,1413,1414};
  //int runs[2]={1331,1411};
   
    TGraphErrors *g=new TGraphErrors();
    TGraphErrors *g_tot=new TGraphErrors();
    TCanvas *c=new TCanvas("out", "", 10, 10, 800, 400);

  for (int ii=0;ii<39;ii++){
    //for (int ii=0;ii<2;ii++){
        cout << "********* Run  "<<runs[ii]<<" ************"<<endl;
        /* Leggo i Files necessari per l'analisi*/
      TFile *f=new TFile(Form("/home/msprea/Scrivania/Tesi/Stabilità/%i.outDST.root",runs[ii]));  // legge il file root : bisogna cambiare percorso
        TH1D *h=(TH1D*)f->Get("hControl");
        TTree *t=(TTree*)f->Get("tOut");
    ifstream fbad(Form("//home/msprea/Scrivania/Tesi/Stabilità/%i.badEvents.dat",runs[ii])); // legge il file con la lista degli event NUmber degli eventi di rumore: bisogna cambiare percorso

        CheckCosmic_selector *mySelector = new CheckCosmic_selector();

        
        t->Process(mySelector);
        // numero di eventi buoni
    //    int good_events = mySelector->good_events;
        int N=mySelector->good_events;
	int M=mySelector->events;
        
        cout << "GOOD events "<< N <<endl;
	cout << "TOTAL events " <<M << endl;

      double Toff=h->GetBinContent(5);
      double Ton=h->GetBinContent(3);

        
      g->SetPoint(ii,ii,N/Toff);
      cout << "punto:" << g->GetPointY(ii) << endl;
      g->SetPointError(ii,0,sqrt(N)/Toff);
      cout << "errore: " << g->GetErrorY(ii) << endl;
      
      g_tot->SetPoint(ii,ii,M/Toff);
      g_tot->SetPointError(ii,0,sqrt(M)/Toff);
      
      cout<<ii<<" "<<runs[ii]<<" "<<N<<" "<<Toff<<" "<<Ton<<" "<<endl;
        delete mySelector;

    }

  c->Divide(2,1);
  // plot graphs
  
  c->cd(1);
  g_tot->Draw("AP");

  c->cd(2);
  g->Draw("AP");    // save graphs in a root file
  c->Print();
 
    
} //end main()
