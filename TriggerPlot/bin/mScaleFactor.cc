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
using namespace std;

double ErrorProp(double x1,double m1,double x2,double m2)     //Data MC
{
    double result = sqrt( (m1*m1)/(x2*x2) + (m2*m2)*(x1*x1)/(x2*x2*x2*x2));
    return result;
}


int main()
{

    TFile* MCFile = new TFile("/wk_cms/sam7k9621/MC_data/muon/muon_MC_eff.root");
    TDirectory* MCdir = gFile->GetDirectory("muoneff");
    TH2D** pass=new TH2D*[5];
    TH2D** total=new TH2D*[5];
    MCdir->GetObject("total_mu22_tkmu22",total[0]);
    MCdir->GetObject("pass_mu22_tkmu22" , pass[0]);
    MCdir->GetObject("total_mu24_tkmu24",total[1]);
    MCdir->GetObject("pass_mu24_tkmu24" , pass[1]);
    MCdir->GetObject("total_mu27_tkmu27",total[2]);
    MCdir->GetObject("pass_mu27_tkmu27" , pass[2]);
    MCdir->GetObject("total_mu45",total[3]);
    MCdir->GetObject("pass_mu45" , pass[3]);
    MCdir->GetObject("total_mu50_tkmu50",total[4]);
    MCdir->GetObject("pass_mu50_tkmu50" , pass[4]);
    


    TFile* dataFile = new TFile("/wk_cms/sam7k9621/muon/tight/muon_tight_eff.root");
    TDirectory* datadir = gFile->GetDirectory("muoneff");
    TEfficiency** dataEff=new TEfficiency*[5];
    datadir->GetObject("total_mu22_tkmu22_clone",dataEff[0]);
    datadir->GetObject("total_mu24_tkmu24_clone",dataEff[1]);
    datadir->GetObject("total_mu27_tkmu27_clone",dataEff[2]);
    datadir->GetObject("total_mu45_clone",dataEff[3]);
    datadir->GetObject("total_mu50_tkmu50_clone",dataEff[4]);


    double ebin[] = {0,0.9,1.2,2.1,2.4};
    double pbin[] = {20,25,30,40,50,60,120};
    
    string name[] ={
        "scale_mu22_tkmu22",
        "scale_mu24_tkmu24",
        "scale_mu27_tkmu27",
        "scale_mu45",
        "scale_mur50_tkmu50"
    };
    
    string text[] ={"IsoMu22 || IsoTkMu22",
                    "IsoMu24 || IsoTkMu24",
                    "IsoMu27 || IsoTkMu27",
                    "Mu45_eta2p1",
                    "Mu50 || TkMu50"};
    
    string cut[] ={"P_{T} > 25",
                    "P_{T} > 27",
                    "P_{T} > 30",
                    "P_{T} > 48 #eta < 2.1",
                    "P_{T} > 53"
    };

    TFile f("mScaleFactor.root","RECREATE");


    for(int k=0; k<5; k++)
    {


        TH2D* scale = new TH2D(name[k].c_str(),"",4,ebin,6,pbin);

        double seff = 0;
        double serr = 0;


        for(int i =1; i<5; i++)
        {
            for(int j=1; j<7; j++)
            {

                TH1D* p = new TH1D("p","pass mc ",1,0,10);
                TH1D* t = new TH1D("t","total mc",1,0,10);

                int binidx = dataEff[k]->GetGlobalBin( i,j ) ;
                double deff = dataEff[k]->GetEfficiency( binidx );
                double derr = dataEff[k]->GetEfficiencyErrorLow(binidx);

                double dp = pass[k] ->GetBinContent(i,j);
                double dt = total[k]->GetBinContent(i,j);
                p->SetBinContent(1,dp);
                t->SetBinContent(1,dt);
                TGraphAsymmErrors* monteEff = new TGraphAsymmErrors(p,t,"b");
                double meff=monteEff->GetY()[0];
                double merr=monteEff->GetErrorY (0);

                if(meff!=0)
                {
                    seff = deff/meff;
                    serr = ErrorProp(deff,derr,meff,merr);
                }
                else
                {
                    seff = 0;
                    serr = 0;
                }
                scale->SetBinContent(i,j,seff);
                scale->SetBinError(i,j,serr);


                delete p;
                delete t;
                delete monteEff;
            }
        }

        const Double_t min = 0;
        const Double_t max = 2;

        const Int_t nLevels = 9999;
        Double_t levels[nLevels];


        for(int i = 1; i < nLevels; i++)
        {
            levels[i] = min + (max - min) / (nLevels - 1) * (i);
        }
        levels[0] = 0.0001;

        TCanvas* c1= new TCanvas();
        gStyle->SetOptStat(0);
        scale->Draw("COLZ textE");
        scale->SetContour((sizeof(levels)/sizeof(Double_t)), levels);
        scale->GetZaxis()->SetRangeUser(min,max);
        scale->GetXaxis()->SetTitle("Eta");
        scale->GetYaxis()->SetTitle("P_{T} [GeV/c^{2}]");
        scale->Write();
        
        TPaveText *pt = plt::NewTextBox(1.2,110,2.3,118);
        pt->AddText("Muon ScaleFactor");
        pt->Draw();
        plt::DrawCMSLabel();
        plt::DrawLuminosity(36500);

        TLegend* leg =plt::NewLegend(0.4,0.7,0.8,0.8);
        leg->SetLineColor(kWhite);
        leg->AddEntry((TObject*)0,text[k].c_str(),""); 
        leg->AddEntry((TObject*)0,cut[k].c_str(),""); 
        leg->Draw(); 
        
        c1->SaveAs( (name[k]+".pdf").c_str() );
        delete c1;

        f.Write();

        delete scale;
    }
    f.Close();
}
