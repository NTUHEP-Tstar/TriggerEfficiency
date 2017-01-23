#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "ManagerUtils/PlotUtils/interface/Common.hpp"
#include "TStyle.h"
#include "TH2.h"
using namespace std;
int main(){


    double ebin[]={-2.5,-2.1,-1.6,-1.4,-0.8,0.8,1.4,1.6,2.1,2.5};
    double pbin[]={0,27,28,29,30,32,35,40,45,50,100};

    double* eb=ebin;
    double* pb=pbin;

    TH2D* eff = new TH2D("eff","Eta-Pt",9,eb,10,pb);

    fstream fin;
    vector<double> val;
    fin.open("/wk_cms/sam7k9621/CMSSW_8_0_10/src/TriggerMCTool/TriggerPlot/test/HLT_EleSingle.txt",ios::in);
    string line;
    while(getline(fin,line)){
        istringstream txt(line);
        while( getline(txt,line,' ') ){
            istringstream v(line);
            double value;
            v>>value;
            val.push_back(value);
        }

        int xbin=0;
        int ybin=0;

        if(val[0] == -2.5)
            xbin=1;
        if(val[0] == -2.1)
            xbin=2;
        if(val[0] == -1.6)
            xbin=3;
        if(val[0] == -1.4)
            xbin=4;
        if(val[0] == -0.8)
            xbin=5;
        if(val[0] == 0.8)
            xbin=6;
        if(val[0] == 1.4)
            xbin=7;
        if(val[0] == 1.6)
            xbin=8;
        if(val[0] == 2.1)
            xbin=9;
        if(val[0] == 2.5)
            xbin=10;
        
            
        if(val[2] == 0)
            ybin=1;
        if(val[2] == 27)
            ybin=2;
        if(val[2] == 28)
            ybin=3;
        if(val[2] == 29)
            ybin=4;
        if(val[2] == 30)
            ybin=5;
        if(val[2] == 32)
            ybin=6;
        if(val[2] == 35)
            ybin=7;
        if(val[2] == 40)
            ybin=8;
        if(val[2] == 45)
            ybin=9;
        if(val[2] == 50)
            ybin=10;
        if(val[2] == 100)
            ybin=11;


        
        eff->SetBinContent(xbin,ybin,val[4]);
        eff->SetBinError(xbin,ybin,val[5]);
        
        val.clear();
    }


    const Double_t min = 0;
    const Double_t max = 1;
  
    const Int_t nLevels = 9999;
    Double_t levels[nLevels];

  
    for(int i = 1; i < nLevels; i++) {
        levels[i] = min + (max - min) / (nLevels - 1) * (i);
    }
    levels[0] = 0.001;
    
    
    TCanvas* c= new TCanvas("c","c");
    gStyle->SetOptStat(0);
    plt::SetAxis(eff);
    TPaveText *pt=plt::NewTextBox(-2,85,2,95);
    pt->AddText("Ele45_WPLoose_Gsf || Ele27_eta2p1_WPLoose_Gsf (correction factor 0.93)");
    pt->SetTextSize( 13 );
    eff->SetTitle("");
    eff->SetContour((sizeof(levels)/sizeof(Double_t)), levels);
    eff->GetZaxis()->SetRangeUser(min,max);
    eff->GetXaxis()->SetTitle("#eta");
    eff->GetYaxis()->SetTitle("p_{T}");
    
    eff->Draw("COLZ  text");
    pt->Draw();
    plt::SaveToPDF(c,"official.pdf");









}
