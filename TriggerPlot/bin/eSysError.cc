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

    TFile* original = new TFile("/wk_cms/sam7k9621/CMSSW_8_0_10/src/eScaleFactor_origin.root");
    TH2D** origin=new TH2D*[2];
    original->GetObject("scale_ele27_ele45"     ,origin[0]);
    original->GetObject("scale_ele32",origin[1]);
    

    TFile* newone = new TFile("/wk_cms/sam7k9621/CMSSW_8_0_10/src/eScaleFactor_dev.root");
    TH2D** deviation=new TH2D*[2];
    newone->GetObject("scale_ele27_ele45",deviation[0]);
    newone->GetObject("scale_ele32",deviation[1]);

    double ebin[] = {-2.5, -2.1, -2, -1.566, -1.444, -0.8 ,0, 0.8 ,1.444, 1.566, 2, 2.1, 2.5};
    double pbin[] = {10,20,30,40,50,200};

    string name[] ={
        "scale_ele27_ele45",
        "scale_ele32"
    };

    string text[] ={"Ele45_WPLoose || Ele27_eta2p1_WPLoost",
                    "Ele32_eta2p1_WPTight"};
    string cut[] ={
        "probe cut:P_{T} > 48 #eta < 2.1",
        "probe cut:P_{T} > 35 #eta < 2.1"
    };
    
    for(int k=0; k<2; k++)
    {


        TH2D* syserr = new TH2D(name[k].c_str(),"",12,ebin,5,pbin);

        for(int i =1; i<13; i++)
        {
            for(int j=1; j<6; j++)
            {


                double ocontent = origin[k]->GetBinContent(i,j);
                double dcontent = deviation[k]->GetBinContent(i,j);
                double oerr = origin[k]->GetBinErrorLow(i,j);
                double derr = deviation[k]->GetBinErrorLow(i,j);

                double seff =  (dcontent-ocontent)/ocontent ;
                double serr = ErrorProp(ocontent,oerr,dcontent,derr);

                if(seff<-0.2)
                    cout<<"seff"<<seff<<endl;

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

        const Int_t nLevels = 999999;
        Double_t levels[nLevels];


        for(int i = 1; i < nLevels; i++)
        {
            levels[i] = min + (max - min) / (nLevels - 1) * (i);
        }
        levels[0] = -2;

        TCanvas* c1= new TCanvas();
        gStyle->SetOptStat(0);
        syserr->Draw("COLZ textE");
        syserr->SetContour((sizeof(levels)/sizeof(Double_t)), levels);
        syserr->GetZaxis()->SetRangeUser(min,max);
        syserr->GetXaxis()->SetTitle("Eta");
        syserr->GetYaxis()->SetTitle("P_{T} [GeV/c^{2}]");
        
        TPaveText *ptt = plt::NewTextBox(-2.4,180,-1.2,195);
        ptt->AddText("Systematic error");
        ptt->Draw();

        TPaveText *pt = plt::NewTextBox(0,180,2.3,195);
        pt->AddText(text[k].c_str());
        if(k==0)
            pt->SetTextSize(13);
        pt->Draw();
        plt::DrawCMSLabel();
        plt::DrawLuminosity(36500);

        TLegend* leg =plt::NewLegend(0.4,0.7,0.8,0.8);
        leg->SetLineColor(kWhite);
        leg->AddEntry((TObject*)0,"( change different MC dataset )",""); 
        leg->Draw(); 
//        leg->SetTextSize(14); 
        
        plt::SaveToPDF( c1 , name[k]+".pdf" );
        
//        c1->SaveAs( (name[k]+".pdf").c_str() );
        delete c1;
        delete syserr;
    }
}
