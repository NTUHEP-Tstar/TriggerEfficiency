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
    TH1D* mass_data[2];
    dir->GetObject("ele27",mass_data[0]);
    dir->GetObject("ele27",mass_data[1]);
//    mass_data[0] = (TH1D*) original->Get("ele27");
//    mass_data[1] = (TH1D*) original->Get("ele32");

    cout<<"read original file"<<endl;

    TFile* newone = new TFile("/wk_cms/sam7k9621/CMSSW_8_0_10/src/TriggerEfficiency/TriggerAnalyzer/data/MC_data/electron/electron_MC_analyz.root");    //mc
    TDirectory* _dir = gFile->GetDirectory("demo");
    TH1D* mass_mc[2];
    _dir->GetObject("ele27",mass_mc[0]);
    _dir->GetObject("ele27",mass_mc[1]);
//    mass_mc[0] = (TH1D*) newone->Get("ele27");
//    mass_mc[1] = (TH1D*) newone->Get("ele32");

    cout<<"read newone file"<<endl;




    string edesc[] = {"Ele45_WPLoose || Ele27_eta2p1_WPLoost (P_{T} > 48)",
                     "Ele32_eta2p1_WPTight (P_{T} > 35)"
                    };
    
    string pdesc[] = {"Ele45_WPLoose || Ele27_eta2p1_WPLoost",
                     "Ele32_eta2p1_WPTight"
                    };

    string pcut[] ={ 
        "probe : P_{T} > 48 & |#eta| < 2.1",
        "probe : P_{T} > 35 & |#eta| < 2.1"
    };
    string tcut[] ={
        "tag    : P_{T} > 30   ",
        "tag    : P_{T} > 35 & |#eta| < 2.1 "
    };


    for(int i=0; i<2; i++)
    {
        extern Color_t color[10];
        extern Style_t mstyle[10];

        cout<<"now print "<<i<<" th"<<endl;


//normalized 
    

        cout<<mass_data[i]<<endl;
        cout<<mass_mc[i]<<endl;

        double ratio = mass_data[i]->Integral()/mass_mc[i]->Integral();
        mass_mc[i]->Scale(ratio);

        TCanvas* c = new TCanvas();
        TPad* pad11= plt::NewTopPad();
        TPad* pad22= plt::NewBottomPad();
        pad11->Draw();
        pad11->SetPad(0,0.28,1,1);
        pad22->Draw();

        pad11->cd();

        double ymax = plt::GetYmax(mass_data[i]);
        ymax*=1.3;

        TH1F* h22=gPad->DrawFrame(60,0,140,ymax,"");
        
        plt::SetAxis(h22);
        h22->GetXaxis()->SetTitle("");
        h22->GetYaxis()->SetTitle("Z mass ");
        h22->GetXaxis()->SetLabelSize(0);
        mass_data   [i]->Draw("EP same");
        mass_mc[i]->Draw("EP same");

        mass_data   [i]->SetLineColor(color[0]);
        mass_data   [i]->SetMarkerColor(color[0]);
        mass_data   [i]->SetMarkerStyle(mstyle[0]);
        mass_data   [i]->SetMarkerSize(0.6);

        mass_mc[i]->SetLineColor(color[1]);
        mass_mc[i]->SetMarkerColor(color[1]);
        mass_mc[i]->SetMarkerStyle(mstyle[1]);
        mass_mc[i]->SetMarkerSize(0.6);


        TPaveText *ppt = plt::NewTextBox(115,ymax*0.85,135,ymax*0.95);
        ppt->AddText(text);
        ppt->Draw();
        plt::DrawCMSLabel();
        plt::DrawLuminosity(lumi);

       
        TLegend *leg =plt::NewLegend(0.5,0.4,0.6,0.6);
        
        leg->SetLineColor(kWhite);
        if(i==0)
            leg->SetTextSize( 13 );
        else
            leg->SetTextSize(15);
        leg->AddEntry(mass_data[i],("Data: "+pdesc[i]).c_str(),"lp");
        leg->AddEntry(mass_mc[i]  ,("MC  : "+pdesc[i]).c_str(),"lp");
        leg->AddEntry((TObject*)0,pcut[i].c_str(),"");
        leg->AddEntry((TObject*)0,tcut[i].c_str(),"");
        leg->Draw();

        pad22->cd();


        TH1 *hist= (TH1*)mass_data[i]->Clone();
        hist->SetName("hist");
        hist->Divide(mass_mc[i]);

        TH1F* h33=gPad->DrawFrame(60,0,140,2,"");
        plt::SetAxis(h33);
        h33->GetXaxis()->SetTitle("GeV/c^{2}");
        h33->GetYaxis()->SetTitle("Data/MC");
        h33->GetYaxis()->SetLabelSize( 10 );
        h33->GetXaxis()->SetTitleOffset( 3 );
        h33->GetYaxis()->SetTitleSize(18);
        hist->Draw("EP same");
        hist->SetMarkerStyle(21);
        hist->SetStats("kFalse");
        
        TLine *line = new TLine(60,1,140,1);
        line->SetLineColor(kRed);
        line->SetLineStyle(8);
        line->Draw();

        plt::SaveToPDF(c,Form("syserr_pt%d.pdf",i));
        
    }
}
