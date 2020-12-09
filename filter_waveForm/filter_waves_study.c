#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <array>
#include <sstream>
#include <string>
#include "TH1D.h"
#include "TROOT.h"

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
TH1D *hTest[4][16];

int main()
{
   // int runs[26]={1324,1325,1326,1328,1329,1330,1331,1332,1333,1334,1335,1336,1337,1338,1339,1342,1343 ,1344,1347,1348,1349,1350,1351,1352,1353,1354};
    int runs[2] = {1324, 1325};
    ifstream inFile;
    ifstream inFile_badEvents;
  
    for(int i=0; i<4; i++){
        for(int j=0; j<16; j++){
            hTest[i][j] = new TH1D(Form("hTest_%i_%i", i, j), Form("hTest_%i_%i; test; test", i, j), 100, 0, 100);
        }
    }
    
    for (int ii=0;ii<2;ii++){
        cout << "RUN NUMBERS "<< runs[ii]<<endl;
        /*  Apertura file  */
    inFile.open(Form("%i.outWaves.root.txt", runs[ii]));
    inFile_badEvents.open(Form("%i.badEvents.dat",runs[ii]));
        
        vector<int> badEventsNumber;
        vector<Event*> events;
        int dT[4] = {4,4,2,2}; //ns --- sampling time; For Slot 0 and 1 Number of samples : 160 ;For Slot 2 and 3 Number of samples: 320.
        
     /* read badEvents file. Fill Vector badEventsNumber with bad_events EventNumber */
        int tmp;
        while(inFile_badEvents>>tmp) badEventsNumber.push_back(tmp);

      
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
        
        
    inFile.close();
    inFile_badEvents.close();
    } //end for on runNumber
    return 0;
}
