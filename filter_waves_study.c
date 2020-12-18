#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <array>
#include <sstream>
#include <string>
#include "TH1D.h"
#include "TROOT.h"
#include <vector>

using namespace std;

class Event{
public:
    Event(){
        runN = eventN =0;
    };
    ~Event(){};
    int runN;
    int eventN;
    vector<double> ampl[4][16];
};

TH1D *hGOOD[4][16];
TH1D *hBAD[4][16];

int main(){
  int runs[20]={1324,1325,1326,1328,1329,1330,1332,1333,1334,1335,1336,1337,1338,1342,1343,1344,1347,1348,1349,1351};
  //int runs[2] = {1324, 1325};
  ifstream inFile;
  ifstream inFile_badEvents;
  
  for(int i=0; i<4; i++){
    for(int j=0; j<16; j++){
      hGOOD[i][j] = new TH1D(Form("hGOOD_%i_%i", i, j), Form("hGOOD_%i_%i; test; test", i, j), 50, -50000, 50000);
      hBAD[i][j] = new TH1D(Form("hBAD_%i_%i", i, j), Form("hBAD_%i_%i; test; test", i, j), 50, -50000, 50000);
    }
  }
  
  for (int ii=0;ii<20;ii++){
    cout << "RUN NUMBERS "<< runs[ii]<<endl;
    /*  Apertura file  */
    inFile.open(Form("%i.outWaves.root.txt", runs[ii]));
    inFile_badEvents.open(Form("%i.badEvents.dat",runs[ii]));
    
    vector<int> badEventsNumber;
    vector<Event*> events;
    int dT[4] = {4,4,2,2}; //ns --- sampling time; For Slot 0 and 1 Number of samples : 160 ;For Slot 2 and 3 Number of samples: 320.
    
    /* read badEvents file. Fill Vector badEventsNumber with bad_events EventNumber */
    int tmp;
    while(inFile_badEvents>>tmp) {
      inFile_badEvents>>tmp;
      badEventsNumber.push_back(tmp); //il file bad events stampa riga per riga anche il numero della run
    }
    badEventsNumber.push_back(10000000); //finto evento finale 
    
    int runN,eventN;
    int slot, channel;
    int count_line=0;
    int pos=5;
    
    string line;
    Event *event_dummy;
    /* read waveform file. Fill Vector Events */
    while (getline(inFile, line)){
      stringstream iss(line);
      if(count_line%65==0){
	event_dummy = new Event();
	iss>>event_dummy->runN>>event_dummy->eventN;
      }else{
	iss>> slot>> channel;
	if(channel<10) pos=5;
	if(channel>=10) pos=6;
            iss.seekg(pos);
            double value;
               while(iss>>value) event_dummy->ampl[slot][channel].push_back(value);
      }
      
      count_line +=1;
      if(count_line%65==0) events.push_back(event_dummy);
      
    }  //end loop while
    cout<<"NUMBER of EVENTS "<< events.size()<<endl;
    
    /* I used the code below for debugging*/
    
    /*
      for(int i=0; i<events.size(); i++){
      Event* test = events[i];
      cout << test->runN<<" "<<test->eventN<<endl;
      for(int s=0; s<4; s++){
      for(int c=0; c<16; c++){
      cout<<s<<" "<<c<<" "<< test->ampl[s][c].size()<<endl;
      for(int v=0; v<test->ampl[s][c].size(); v++){
      
      //       cout <<test->ampl[s][c][v]<< " "<<endl;
      }
      }
      }
      
      }
    */
    
    //definisco la funzione
    TF1 f("f", "[0]*cos([1]*(x-[2]))");
    
    //lavoro canale per canale, evento per evento
    for(int e=0; e<events.size(); e++){
      for(int i=0; i<4; i++){
	for(int j=0; j<16; j++){
	  
	  //prendo i dati dell'evento
	  Event* test = events[e];
	  //if(i==0 && j==0){
	  //	 cout << test->eventN << endl;
	  //}
	  vector<double> data = test->ampl[i][j]; 
	  
	  //trovo parametri del seno
	  double t_max=0, f_max=data[0], f_min=data[0];
	  for(int k=0; k<data.size(); k++){
	    if(data[k]>f_max){
		   f_max=data[k];
		   t_max=k;
		 }
	    if(data[k]<f_min){
	      f_min=data[k];
	    }
	  }
	  
	  //trovo tempo massimo 
	  t_max=t_max*dT[i];
	  //trovo ampiezza oscillazione 
	  //DA VERIFICARE
	  double mid=(f_max+f_min)/2;
	  f_max=f_max-mid;
	  
	  double T= 380; // time in ns
	  f.SetParameter(0, f_max);
	  f.SetParameter(1, 2*TMath::Pi()/T);
	  f.SetParameter(2, t_max);
	       
	  //calcolo della correlazione
	  double y=0;
	  double crosscorrelation=0;
	  for(int k=0; k<data.size(); k++){
	    crosscorrelation=crosscorrelation+data[k]*f.Eval(k*dT[i])*dT[i];
	  }
	  
	  vector<int>::iterator it; 
	  it = find (badEventsNumber.begin(), badEventsNumber.end(), test->eventN); 
	  if (it == badEventsNumber.end()) {
	    hGOOD[i][j]->Fill(crosscorrelation);
	  }else{
	    hBAD[i][j]->Fill(crosscorrelation);
	  }
	}
      }   
    }
    
    inFile.close();
    inFile_badEvents.close();
  } //end for on runNumber
  
    /*TCanvas *good=new TCanvas("good", "", 10, 10, 1600, 1600);
      good->Divide(8,8);
      for(int j=0; j<16; j++){
      for(int i=0; i<4; i++){
      good->cd(i*16+j+1);
      hGOOD[i][j]->Draw("");
      }
      }
            
      TCanvas *bad=new TCanvas("bad", "", 10, 10, 1600, 1600);
      bad->Divide(8,8);
      for(int j=0; j<16; j++){
      for(int i=0; i<4; i++){
      bad->cd(i*16+j+1);
      hBAD[i][j]->Draw("");
      }
      }*/
 
  TCanvas *good=new TCanvas("good", "", 10, 10, 1600, 1600);
  good->Divide(4,4);
  TCanvas *bad=new TCanvas("bad", "", 10, 10, 1600, 1600);
  bad->Divide(4,4);
  for(int i=0; i<4; i++){   
    for(int j=0; j<16; j++){
      good->cd(j+1);
      hGOOD[i][j]->Draw("");
      bad->cd(j+1);
      hBAD[i][j]->Draw("");
    }
  }
  
  return 0;
}
