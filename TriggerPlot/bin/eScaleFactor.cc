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

    TFile* MCFile = new TFile("/wk_cms/sam7k9621/CMSSW_8_0_10/src/TriggerEfficiency/TriggerEff/data/MC_data/electron/electron_MC_eff.root","READ");
    TDirectory* MCdir = MCFile->GetDirectory("electroneff");
    TH2D** pass=new TH2D*[2];
    TH2D** total=new TH2D*[2];
    MCdir->GetObject("total_ele27",total[0]);
    MCdir->GetObject("pass_ele45_ele27" , pass[0]);
    MCdir->GetObject("total_ele32",total[1]);
    MCdir->GetObject("pass_ele32" , pass[1]);

    TFile* dataFile = new TFile("/wk_cms/sam7k9621/CMSSW_8_0_10/src/TriggerEfficiency/TriggerEff/data/electron/electron_eff.root","READ");
    TDirectory* datadir = dataFile->GetDirectory("electroneff");
    TEfficiency** dataEff=new TEfficiency*[2];
    datadir->GetObject("total_ele27_clone",dataEff[0]);
    datadir->GetObject("total_ele32_clone",dataEff[1]);

    double ebin[] = {-2.5, -2.1, -2, -1.566, -1.444, -0.8 ,0, 0.8 ,1.444, 1.566, 2, 2.1, 2.5};
    double pbin[] = {10,20,30,40,50,200};
    string text[] ={"Ele45_WPLoose || Ele27_eta2p1_WPLoost",
                    "Ele32_eta2p1_WPTight"};
    string cut[] ={"P_{T} > 48 #eta < 2.1",
                    "P_{T} > 35 #eta < 2.1"
    };

    string name[] ={
        "scale_ele27_ele45",
        "scale_ele32"
    };

    TFile f("eScaleFactor.root","RECREATE");
    
    for(int k=0; k<2; k++)
    {


        TH2D* scale = new TH2D(name[k].c_str(),"",12,ebin,5,pbin);
        double seff = 0;
        double serr = 0;


        for(int i =1; i<13; i++)
        {
            for(int j=1; j<6; j++)
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
                
                double meff = monteEff->GetY()[0];
                double merr = monteEff->GetErrorY (0);

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
        
        TPaveText *pt = plt::NewTextBox(0,180,2.3,195);
        pt->AddText("Electron ScaleFactor");
        
        
        pt->Draw();
        plt::DrawCMSLabel();
        plt::DrawLuminosity(35685);


        TLegend* leg =plt::NewLegend(0.4,0.7,0.8,0.8);
        leg->SetLineColor(kWhite);
        leg->AddEntry((TObject*)0,text[k].c_str(),""); 
        leg->AddEntry((TObject*)0,cut[k].c_str(),""); 
        leg->Draw(); 
        
        c1->SaveAs( (name[k]+".pdf").c_str() );
        delete c1;
        
        f.Write();
        cout<<"done"<<endl;
        delete scale;
    }
    f.Close();
}
