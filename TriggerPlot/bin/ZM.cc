#include <iostream>
#include <vector>
#include "TEfficiency.h"
#include "TH1.h"
#include "TH2.h"
#include "TFile.h"
#include "TDirectory.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TGraphAsymmErrors.h"
#include "ManagerUtils/PlotUtils/interface/Common.hpp"
#include <math.h>
#include "TriggerEfficiency/TriggerPlot/interface/eConstant.h"
using namespace std;

int main()
{

    TFile* original = new TFile("/wk_cms/sam7k9621/CMSSW_8_0_10/src/TriggerEfficiency/TriggerAnalyzer/data/electron/electron_anlyz.root");   //data
    TDirectory* dir = gFile->GetDirectory("demo");
    TH1D* pass;
    TH1D* fail;

    dir->GetObject("ele32_pass",pass);
    dir->GetObject("ele32_fail",fail);
//    mass_data[0] = (TH1D*) original->Get("ele27");
//    mass_data[1] = (TH1D*) original->Get("ele32");

    cout<<"read file"<<endl;





        extern Color_t color[10];
        extern Style_t mstyle[10];

        TCanvas* c = new TCanvas();

        double ymax = plt::GetYmax(pass);
        ymax*=1.3;

        TH1F* h22=gPad->DrawFrame(70,0,130,ymax,"");
        
        plt::SetAxis(h22);
        h22->GetXaxis()->SetTitle("");
        h22->GetYaxis()->SetTitle("Z mass ");
        h22->GetXaxis()->SetTitle("GeV/c^{2}");
        pass->Draw("EP same");
        fail->Draw("EP same");

        pass   ->SetLineColor(color[0]);
        pass   ->SetMarkerColor(color[0]);
        pass   ->SetMarkerStyle(mstyle[0]);
        pass   ->SetMarkerSize(0.6);

        fail->SetLineColor(color[1]);
        fail->SetMarkerColor(color[1]);
        fail->SetMarkerStyle(mstyle[1]);
        fail->SetMarkerSize(0.6);


        TPaveText *ppt = plt::NewTextBox(110,ymax*0.85,127,ymax*0.95);
        ppt->AddText(text);
        ppt->Draw();
        plt::DrawCMSLabel();
        plt::DrawLuminosity(lumi);

       
        TLegend *leg =plt::NewLegend(0.65,0.4,0.75,0.6);
        
        leg->SetLineColor(kWhite);
        leg->SetTextSize( 18 );
        leg->AddEntry(pass,"pass probe","lp");
        leg->AddEntry(fail,"fail to pass probe","lp");
        leg->Draw();

        plt::SaveToPDF(c, "zmass.pdf");
        
}
