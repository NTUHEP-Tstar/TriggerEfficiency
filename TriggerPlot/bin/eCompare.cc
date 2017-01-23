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

double ErrorProp(double x1,double m1,double x2,double m2)     //Data MC
{
    double result = sqrt( (m1*m1)/(x2*x2) + (m2*m2)*(x1*x1)/(x2*x2*x2*x2));
    return result;
}
class Trigger{
    public:
        Trigger(string t,bool monte){
            if(monte){
                TFile* File = new TFile("/wk_cms/sam7k9621/MC_data/electron/electron_MC_analyz.root");
                TDirectory* dir = gFile->GetDirectory("demo");
            dir->GetObject( ("pass_pt_"+t).c_str(),passPt);
            dir->GetObject( ("pass_eta_"+t).c_str(),passEta);
            dir->GetObject( ("total_pt_"+t).c_str(),totalPt);
            dir->GetObject( ("total_eta_"+t).c_str(),totalEta);
            }
            else{
                TFile* File = new TFile("/wk_cms/sam7k9621/electron/electron_anlyz.root");
                TDirectory* dir = gFile->GetDirectory("demo");
            dir->GetObject( ("pass_pt_"+t).c_str(),passPt);
            dir->GetObject( ("pass_eta_"+t).c_str(),passEta);
            dir->GetObject( ("total_pt_"+t).c_str(),totalPt);
            dir->GetObject( ("total_eta_"+t).c_str(),totalEta);
            }
        
            effPt =new TGraphAsymmErrors(passPt ,totalPt ,"b");
            effEta=new TGraphAsymmErrors(passEta,totalEta,"b");
            
        
        }

        TGraphAsymmErrors* getPtEff(){
            return effPt;
        }

        TGraphAsymmErrors* getEtaEff(){
            return effEta;
        }

    private:
        TH1D* passPt;
        TH1D* passEta;
        TH1D* totalPt;
        TH1D* totalEta;
        TGraphAsymmErrors* effPt;
        TGraphAsymmErrors* effEta;

};

int main()
{

    Trigger mtri27("ele27",true);
    Trigger mtri32("ele32",true);
    Trigger mtrigger[]={mtri27,mtri32};

    cout<<"read monte"<<endl;

    Trigger dtri27("ele27",false);
    Trigger dtri32("ele32" ,false);
    Trigger dtrigger[]={dtri27,dtri32};

    cout<<"read data"<<endl;

    double ebin[] = {-2.5, -2.1, -2, -1.566, -1.444, -0.8 ,0, 0.8 ,1.444, 1.566, 2, 2.1, 2.5};
    
    
    double p27[] ={10,20,30,35,40,41,42,43,44,45,46,47,48,49,50,51,52,53,60,200};
    double p32[] ={10,20,25,30,31,32,33,34,35,36,37,38,39,40,50,60,200};
    
    
    
    double* pbin[] = {p27,p32};
    int     binnum[]={19,16};
    
    


    string tri[]={
        "Ele45_WPLoose || Ele27_eta2p1_WPLoost",
        "Ele32_eta2p1_WPTight",
    };
    
    
    string desc[] ={"Ele45_WPLoose* || Ele27_eta2p1_WPLoost",
                    "Ele32_eta2p1_WPTight"};
    
    string pcut[] ={"P_{T} > 48",
                    "P_{T} > 35",
    };

    string ecut[] ={
        "#eta < 2.1",
        "#eta < 2.1"
    };


    for(int i=0; i<2; i++)
    {

        extern Color_t color[10];
        extern Style_t mstyle[10];

        TGraphAsymmErrors* mPtEff  = mtrigger[i].getPtEff();
	    mPtEff->SetName( ("mPt"+tri[i]).c_str());
        TGraphAsymmErrors* mEtaEff = mtrigger[i].getEtaEff();
	    mEtaEff->SetName( ("mEta"+tri[i]).c_str());
        TGraphAsymmErrors* dPtEff  = dtrigger[i].getPtEff();
	    dPtEff->SetName( ("dPt"+tri[i]).c_str());
        TGraphAsymmErrors* dEtaEff = dtrigger[i].getEtaEff();
	    dEtaEff->SetName( ("dEta"+tri[i]).c_str());

        TCanvas* c = new TCanvas();
        TPad* pad11= plt::NewTopPad();
        TPad* pad22= plt::NewBottomPad();
        pad11->Draw();
        pad11->SetPad(0, 0.28, 1., 1.0 );
        pad22->Draw();

        pad11->cd();

        TH1F* h22=gPad->DrawFrame(pframe_x_min,pframe_y_min,pframe_x_max,pframe_y_max,"");
        plt::SetAxis(h22);
        h22->GetXaxis()->SetTitle("");
        h22->GetYaxis()->SetTitle("Efficiency");
        h22->GetXaxis()->SetLabelSize(0 );
        mPtEff->Draw("EP same");
        dPtEff->Draw("EP same");

        mPtEff->SetLineColor(color[0]);
        mPtEff->SetMarkerColor(color[0]);
        mPtEff->SetMarkerStyle(mstyle[0]);
        mPtEff->SetMarkerSize(0.6);

        dPtEff->SetLineColor(color[1]);
        dPtEff->SetMarkerColor(color[1]);
        dPtEff->SetMarkerStyle(mstyle[1]);
        dPtEff->SetMarkerSize(0.6);
        
        TLine *lline = new TLine(pline_x_min,pline_y_min,pline_x_max,pline_y_max);
        lline->SetLineColor(kRed);
        lline->SetLineStyle(8);
        lline->Draw();

        TPaveText *ppt = plt::NewTextBox(ptext_x_min,ptext_y_min,ptext_x_max,ptext_y_max);
        ppt->AddText(text);
        ppt->Draw();
        plt::DrawCMSLabel();
        plt::DrawLuminosity(lumi);
        
        TLegend *lleg = plt::NewLegend(pleg_x_min,pleg_y_min,pleg_x_max,pleg_y_max);
        lleg->SetLineColor(kWhite);
        lleg->AddEntry(("mPt"+tri[i]).c_str(),("MC   "+desc[i]+" ("+ecut[i]+")").c_str(),"lp");
        lleg->AddEntry(("dPt"+tri[i]).c_str(),("Data "+desc[i]+" ("+ecut[i]+")").c_str(),"lp");
        lleg->SetTextSize(14);
        lleg->Draw();
    
        pad22->cd();

        TH1F* _ratio = new TH1F("_ratio","",binnum[i],pbin[i]);
        
        for(int j=1;j<binnum[i]+1;j++){
            double deff = dPtEff->GetY()[j-1];
            double meff = mPtEff->GetY()[j-1];
            double derr = dPtEff->GetErrorY(j-1);
            double merr = mPtEff->GetErrorY(j-1);
            double seff;
            double serr;
            if(meff!=0){
                seff= deff/meff;
                serr = ErrorProp(deff,derr,meff,merr);
            }
            else{
                seff=0;
                serr=0;
            }
            _ratio->SetBinContent(j,seff);
            _ratio->SetBinError(j,serr);
        }
        TH1F* h33;
        h33=gPad->DrawFrame(pframe_x_min,0,pframe_x_max,2,"");
        plt::SetAxis(h33);
        h33->GetXaxis()->SetTitle("Pt [GeV]");
        h33->GetYaxis()->SetTitle("Data / MC");
        h33->GetYaxis()->SetLabelSize( 12 );
        h33->GetXaxis()->SetTitleOffset( 3 );
        _ratio->Draw("EP same");
        _ratio->SetMarkerStyle(21);
        _ratio->SetStats(kFALSE);
        TLine *lline1 = new TLine(pline_x_min,pline_y_min,pline_x_max,pline_y_max);
        lline1->SetLineColor(kRed);
        lline1->SetLineStyle(8);
        lline1->Draw();

        plt::SaveToPDF(c,Form("pteff%d.pdf",i));




        /********************************************************************************************/
        
        TCanvas* c1 = new TCanvas();
        TPad* pad1= plt::NewTopPad();
        TPad* pad2= plt::NewBottomPad();
        pad1->Draw();
        pad1->SetPad(0, 0.28, 1., 1.0 );
        pad2->Draw();
        
        pad1->cd();
        
        TH1F* h2=gPad->DrawFrame(eframe_x_min,eframe_y_min,eframe_x_max,eframe_y_max,"");
        plt::SetAxis(h2);
        h2->GetXaxis()->SetTitle("");
        h2->GetYaxis()->SetTitle("Efficiency");
        h2->GetXaxis()->SetLabelSize(0 );
        mEtaEff->Draw("EP same");
        dEtaEff->Draw("EP same");

        mEtaEff->SetLineColor(color[0]);
        mEtaEff->SetMarkerColor(color[0]);
        mEtaEff->SetMarkerStyle(mstyle[0]);
        mEtaEff->SetMarkerSize(0.6);

        dEtaEff->SetLineColor(color[1]);
        dEtaEff->SetMarkerColor(color[1]);
        dEtaEff->SetMarkerStyle(mstyle[1]);
        dEtaEff->SetMarkerSize(0.6);
        
        TLine *line = new TLine(eline_x_min,eline_y_min,eline_x_max,eline_y_max);
        line->SetLineColor(kRed);
        line->SetLineStyle(8);
        line->Draw();

        TPaveText *pt = plt::NewTextBox(etext_x_min,etext_y_min,etext_x_max,etext_y_max);
        pt->AddText(text);
        pt->Draw();
        plt::DrawCMSLabel();
        plt::DrawLuminosity(lumi);
        
        TLegend *leg = plt::NewLegend(eleg_x_min,eleg_y_min,eleg_x_max,eleg_y_max);
        leg->SetLineColor(kWhite);
        leg->AddEntry(("mEta"+tri[i]).c_str(),("MC   "+desc[i]+" ("+pcut[i]+"GeV)").c_str(),"lp");
        leg->AddEntry(("dEta"+tri[i]).c_str(),("Data "+desc[i]+" ("+pcut[i]+"GeV)").c_str(),"lp");
        leg->SetTextSize(14);
        leg->Draw();
    


        pad2->cd();

        TH1F* ratio = new TH1F("ratio","",12,ebin);
        
        for(int j=1;j<13;j++){
            double deff = dEtaEff->GetY()[j-1];
            double meff = mEtaEff->GetY()[j-1];
            double derr = dEtaEff->GetErrorY(j-1);
            double merr = mEtaEff->GetErrorY(j-1);
            double seff = deff/meff;
            double serr = ErrorProp(deff,derr,meff,merr);
            ratio->SetBinContent(j,seff);
            ratio->SetBinError(j,serr);
        
        }
        TH1F* h3=gPad->DrawFrame(eframe_x_min,0,eframe_x_max,1.4,"");
        plt::SetAxis(h3);
        h3->GetXaxis()->SetTitle("Eta");
        h3->GetYaxis()->SetTitle("Data / MC");
        h3->GetYaxis()->SetLabelSize( 12 );
        h3->GetXaxis()->SetTitleOffset( 3 );
        ratio->Draw("EP same");
        ratio->SetMarkerStyle(21);
        ratio->SetStats(kFALSE);
        TLine *line1 = new TLine(eline_x_min,eline_y_min,eline_x_max,eline_y_max);
        line1->SetLineColor(kRed);
        line1->SetLineStyle(8);
        line1->Draw();

        plt::SaveToPDF(c1,Form("etaeff%d.pdf",i));
        delete _ratio;
        delete ratio;
        delete c1;
    }
}
