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

double ErrorProp(double o,double om,double d,double dm)     //Data MC
{
    double result = sqrt( om*om*( (d*d)/(o*o*o*o) ) + dm*dm*(1/(o*o)) );
    return result;
}


int main()
{

    TFile* original = new TFile("/wk_cms/sam7k9621/CMSSW_8_0_10/src/mScaleFactor_origin.root");
    TH2D** origin=new TH2D*[5];
    original->GetObject("scale_mu22_tkmu22",origin[0]);
    original->GetObject("scale_mu24_tkmu24",origin[1]);
    original->GetObject("scale_mu27_tkmu27",origin[2]);
    original->GetObject("scale_mu45"       ,origin[3]);
    original->GetObject("scale_mur50_tkmu50",origin[4]);
    


    TFile* newone = new TFile("/wk_cms/sam7k9621/CMSSW_8_0_10/src/mScaleFactor_dev.root");
    TH2D** deviation=new TH2D*[5];
    newone->GetObject("scale_mu22_tkmu22",deviation[0]);
    newone->GetObject("scale_mu24_tkmu24",deviation[1]);
    newone->GetObject("scale_mu27_tkmu27",deviation[2]);
    newone->GetObject("scale_mu45"       ,deviation[3]);
    newone->GetObject("scale_mur50_tkmu50",deviation[4]);


    double ebin[] = {0,0.9,1.2,2.1,2.4};
    double pbin[] = {20,25,30,40,50,60,120};
    
    string name[] ={
        "scale_mu22_tkmu22",
        "scale_mu24_tkmu24",
        "scale_mu27_tkmu27",
        "scale_mu45",
        "scale_mu50_tkmu50"
    };

    
    string text[] ={"IsoMu22 || IsoTkMu22",
                    "IsoMu24 || IsoTkMu24",
                    "IsoMu27 || IsoTkMu27",
                    "Mu45_eta2p1",
                    "Mu50 || TkMu50"};
    
    string cut[] ={"probe cut:P_{T} > 25 ",
                   "probe cut:P_{T} > 27",
                   "probe cut:P_{T} > 30",
                   "probe cut:P_{T} > 48 #eta < 2.1",
                   "probe cut:P_{T} > 53"
    };



    for(int k=0; k<5; k++)
    {
        TH2D* syserr = new TH2D(name[k].c_str(),"",4,ebin,6,pbin);


        for(int i =1; i<5; i++)
        {
            for(int j=1; j<7; j++)
            {


                double ocontent = origin[k]   ->GetBinContent(i,j);
                double dcontent = deviation[k]->GetBinContent(i,j);
                double oerr     = origin[k]   ->GetBinErrorLow(i,j);
                double derr     = deviation[k]->GetBinErrorLow(i,j);
               
                cout<<name[k]<<endl;
                
                double seff =  (dcontent-ocontent)/ocontent ;
                double serr = ErrorProp(ocontent,oerr,dcontent,derr);
               
                if(seff<-0.2)
                    cout<<"seff "<<seff<<endl;

                if(ocontent==0){
                    seff=0;
                    serr=0;
                }



                syserr->SetBinContent(i,j,seff);
                syserr->SetBinError(i,j,serr);
                
            }
        }

        const Double_t min = -1;
        const Double_t max = 1;

        const Int_t nLevels = 99999;
        Double_t levels[nLevels];


        for(int i = 1; i < nLevels; i++)
        {
            levels[i] = min + (max - min) / (nLevels - 1) * (i);
        }
        levels[0] = -1;

        TCanvas* c1= new TCanvas();
        gStyle->SetOptStat(0);
        syserr->Draw("COLZ textE");
        syserr->SetContour((sizeof(levels)/sizeof(Double_t)), levels);
        syserr->GetZaxis()->SetRangeUser(min,max);
        syserr->GetXaxis()->SetTitle("Eta");
        syserr->GetYaxis()->SetTitle("P_{T} [GeV/c^{2}]");
        
        TPaveText *ptt = plt::NewTextBox(0.05,110,0.73,118);
        ptt->AddText("Systematic error");
        ptt->Draw();

        TPaveText *pt = plt::NewTextBox(1.25,110,2.35,118);
        pt->AddText(text[k].c_str());
        pt->Draw();
        plt::DrawCMSLabel();
        plt::DrawLuminosity(36500);

        TLegend* leg =plt::NewLegend(0.42,0.73,0.82,0.83);
        leg->SetLineColor(kWhite);
        leg->AddEntry((TObject*)0,"( change Z mass window from 60-120 to 70-110 )",""); 
//        leg->SetTextSize(14);
        leg->Draw(); 
       
        plt::SaveToPDF( c1,name[k]+".pdf" );
//        c1->SaveAs( (name[k]+".pdf").c_str() );
        delete c1;
        delete syserr;
    }
}
