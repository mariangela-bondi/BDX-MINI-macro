
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
  int runs[39]={1324,1325,1326,1328,1329,1330,1331,1332,1333,1334,1335,1336,1337,1338,1339,1342,1343,1344,1347,1348,1349,1350,1351,1352,1353,1354,1402,1403,1404,1405,1406,1407,1408,1409,1410,1411,1412,1413,1414}; //run 1339 too short
  //int runs[2]={1324, 1325};
  //int runs[38]={1324,1325,1326,1328,1329,1330,1331,1332,1333,1334,1335,1336,1337,1338,1342,1343,1344,1347,1348,1349,1350,1351,1352,1353,1354,1402,1403,1404,1405,1406,1407,1408,1409,1410,1411,1412,1413,1414};
  
    TGraphErrors *g=new TGraphErrors();
    TGraphErrors *g_tot=new TGraphErrors();
    TGraphErrors *g_time_limited=new TGraphErrors();
    TGraphErrors *g_events_n=new TGraphErrors();

    for (int ii=0;ii<39;ii++){
      //for (int ii=0;ii<2;ii++){
      cout << "********* Run  "<<runs[ii]<<" ************"<<endl;
      /* Leggo i Files necessari per l'analisi*/
      TFile *f=new TFile(Form("/home/msprea/Scrivania/Tesi/DatiInformazioniLaser/Data/%i.outDST.root",runs[ii]));  // legge il file root : bisogna cambiare percorso
      TH1D *h=(TH1D*)f->Get("hControl");
      TTree *t=(TTree*)f->Get("tOut");
      ifstream fbad(Form("/home/msprea/Scrivania/Tesi/DatiInformazioniLaser/Data/%i.badEvents.dat",runs[ii])); // legge il file con la lista degli event NUmber degli eventi di rumore: bisogna cambiare percorso
      
      CheckCosmic_selector *mySelector = new CheckCosmic_selector();
      
      TH1D *status=(TH1D*)f->Get("hStatus_selector");
      double time=0; 
      double t_max=10*3600; //4 h time
      int end=0; //time of the last entry
      int stat;
      while(time<t_max+1){ //loop ends when 4 h of beam off data are collected
	stat=status->GetBinContent(end);
	if(stat==0){ //if the bean is off the time scales
	  time++;
	}
	end++;
      }
      
      int tmp;
      vector<int> bad_events;
      while(fbad>>tmp){
	fbad>>tmp;
	bad_events.push_back(tmp);
      }
      fbad.close();
      bad_events.push_back(1000000000); //add a fake last entry
      
      
      mySelector->end=end;
      mySelector->bad_events=bad_events;

      t->Process(mySelector);
      // numero di eventi buoni
      //    int good_events = mySelector->good_events;
        int N=mySelector->good_events;
	int M=mySelector->events;
	int P=mySelector->good_events_time_limited;

	cout << "GOOD events "<< N <<endl;
	cout << "TOTAL events " <<M << endl;
	cout << "GOOD events in 4 h " << P << endl;
	
	double Toff=h->GetBinContent(5);
	double Ton=h->GetBinContent(3);
	
        
      g->SetPoint(ii,runs[ii],N/Toff);
      //cout << "punto:" << g->GetPointY(ii) << endl;
      g->SetPointError(ii,0,sqrt(N)/Toff);
      //cout << "errore: " << g->GetErrorY(ii) << endl;
      
      g_tot->SetPoint(ii,runs[ii],M/Toff);
      //cout << "punto:" << g_tot->GetPointY(ii) << endl;
      g_tot->SetPointError(ii,0,sqrt(M)/Toff);
      //cout << "errore: " << g_tot->GetErrorY(ii) << endl;
      
      g_time_limited->SetPoint(ii,runs[ii],P/t_max);
      //cout << "punto:" << g_time_limited->GetPointY(ii) << endl;
      g_time_limited->SetPointError(ii,0,sqrt(P)/t_max);
      //cout << "errore: " << g_time_limited->GetErrorY(ii) << endl;

      g_events_n->SetPoint(ii, runs[ii], P);
      g_events_n->SetPointError(ii, 0, sqrt(P));

      //cout<<ii<<" "<<runs[ii]<<" "<<N<<" "<<Toff<<" "<<Ton<<" "<<endl;
        delete mySelector;

    }

    TCanvas *c=new TCanvas("out", "", 10, 10, 800, 800);
    c->Divide(2,2);
    // plot graphs
    
    
    TF1 f_good("f_good", "[0]");
    TF1 f_tot("f_tot", "[0]");
    TF1 f_time_limited("f_time_limited", "[0]");
    TF1 n_events("n", "[0]");
    g_tot->Fit("f_tot");
    g->Fit("f_good");
    g_time_limited->Fit("f_time_limited");
    g_events_n->Fit("n");

    c->cd(1);
    g_tot->SetTitle("TOTAL events");
    g_tot->Draw("AP");
    g_tot->GetXaxis()->SetTitle("Run Number");
    g_tot->GetYaxis()->SetTitle("Rate [Hz]");


    c->cd(2);
    g->SetTitle("GOOD events");
    g->Draw("AP");    
    g->GetXaxis()->SetTitle("Run Number");
    g->GetYaxis()->SetTitle("Rate [Hz]");

    c->cd(3);
    g_time_limited->SetTitle("GOOD events in 4 h");
    g_time_limited->Draw("AP");   
    g_time_limited->GetXaxis()->SetTitle("Run Number");
    g_time_limited->GetYaxis()->SetTitle("Rate [Hz]");
 
    c->cd(4);
    g_events_n->SetTitle("Number of events vs run");
    g_events_n->Draw("AP");
    g_events_n->GetXaxis()->SetTitle("Run Number");
    g_events_n->GetYaxis()->SetTitle("Number of events");

    gStyle->SetOptFit();
    c->Print("out.pdf","pdf");
 
    // save graphs in a root file
    
} //end main()
