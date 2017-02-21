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

#define PLOT_Y_MAX 0.8

double ErrorProp(double o,double om,double d,double dm)     //Data MC
{
    double result = sqrt( om*om*( (d*d)/(o*o*o*o) ) + dm*dm*(1/(o*o)) );
    return result;
}


int main()
{

    TFile* original = new TFile("/wk_cms/sam7k9621/CMSSW_8_0_10/src/eCompare_ori.root");
    TH1D* origin_pt[2];
    origin_pt[0] = (TH1D*) original->Get("pt_scale_ele27");
    origin_pt[1] = (TH1D*) original->Get("pt_scale_ele32");


    TH1D* origin_eta[2];
    origin_eta[0] = (TH1D*) original->Get("etascale_ele27");
    origin_eta[1] = (TH1D*) original->Get("etascale_ele32");


    cout<<"read original file"<<endl;

    TFile* newone = new TFile("/wk_cms/sam7k9621/CMSSW_8_0_10/src/eCompare_dev.root");
    TH1D* deviation_pt[2];
    deviation_pt[0] = (TH1D*) newone->Get("pt_scale_ele27");
    deviation_pt[1] = (TH1D*) newone->Get("pt_scale_ele32");

    TH1D* deviation_eta[2];
    deviation_eta[0] = (TH1D*) newone->Get("etascale_ele27");
    deviation_eta[1] = (TH1D*) newone->Get("etascale_ele32");

    cout<<"read newone file"<<endl;

    double ebin[] = {-2.5, -2.1, -2, -1.566, -1.444, -0.8 ,0, 0.8 ,1.444, 1.566, 2, 2.1, 2.5};
    
    double p27[] ={10,20,30,35,40,41,42,43,44,45,46,47,48,49,50,51,52,53,60,200};
    double p32[] ={10,20,25,30,31,32,33,34,35,36,37,38,39,40,50,60,200};
    
    double* pbin[] = {p27,p32};
    int     binnum[]={19,16};


    string name[] =
    {
        "scale_ele27",
        "scale_ele32"
    };


    string edesc[] = {"Ele45_WPLoose || Ele27_eta2p1_WPLoost (P_{T} > 48)",
                     "Ele32_eta2p1_WPTight (P_{T} > 35)"
                    };
    
    string pdesc[] = {"Ele45_WPLoose || Ele27_eta2p1_WPLoost ( |#eta| < 2.1  )",
                     "Ele32_eta2p1_WPTight ( |#eta| < 2.1  )"
                    };



    for(int i=0; i<2; i++)
    {
        extern Color_t color[10];
        extern Style_t mstyle[10];

        cout<<"now print "<<i<<" th"<<endl;

        TH1D* ratio = new TH1D(("pt_"+name[i]).c_str(),name[i].c_str(),binnum[i],pbin[i]);
        for(int j=1; j<binnum[i]+1; j++)
        {
            double oeff = origin_pt   [i]->GetBinContent(j);
            double deff = deviation_pt[i]->GetBinContent(j);
            double oerr = origin_pt   [i]->GetBinError(j);
            double derr = deviation_pt[i]->GetBinError(j);

            double seff = (deff-oeff)/oeff;
            double serr = ErrorProp(oeff,oerr,deff,derr);
            cout<<"oeff "<<oeff<<endl;
            cout<<"deff "<<deff<<endl;

            if(oeff==0){
                seff=0;
                serr=0;
            }

            ratio->SetBinContent(j,seff);
            ratio->SetBinError(j,serr);
            cout<<"seff "<<seff<<endl;
        }
       
        cout<<"here"<<endl;
        

        TH1D* _ratio = new TH1D( ("eta_"+name[i]).c_str(),name[i].c_str(),12,ebin);
        for(int j=1;j<13;j++){

            cout<<origin_eta[i]<<endl;
            cout<<deviation_eta[i]<<endl;

            double oeff = origin_eta   [i]->GetBinContent(j);
            double deff = deviation_eta[i]->GetBinContent(j);
            double oerr = origin_eta   [i]->GetBinError(j);
            double derr = deviation_eta[i]->GetBinError(j);

            double seff = (deff-oeff)/oeff;
            double serr = ErrorProp(oeff,oerr,deff,derr);

            if(oeff==0){
                seff=0;
                serr=0;
            }

            _ratio->SetBinContent(j,seff);
            _ratio->SetBinError(j,serr);
        }
    

        cout<<"gggg"<<endl;

        TCanvas* c = new TCanvas();
        TPad* pad11= plt::NewTopPad();
        TPad* pad22= plt::NewBottomPad();
        pad11->Draw();
        pad11->SetPad(0,0.28,1,1);
        pad22->Draw();

        pad11->cd();


       double pymin,pymax;

        if(i==0){
            pymin=0.95;
            pymax=1.05;
        }
        else{
            pymin=0.95;
            pymax=1.05;
        }

        TH1F* h22=gPad->DrawFrame(pframe_x_min,pymin,pframe_x_max,pymax,"");
        
        plt::SetAxis(h22);
        h22->GetXaxis()->SetTitle("");
        h22->GetYaxis()->SetTitle("Scale Factor");
        h22->GetXaxis()->SetLabelSize(0);
        origin_pt   [i]->Draw("EP same");
        deviation_pt[i]->Draw("EP same");

        origin_pt   [i]->SetLineColor(color[0]);
        origin_pt   [i]->SetMarkerColor(color[0]);
        origin_pt   [i]->SetMarkerStyle(mstyle[0]);
        origin_pt   [i]->SetMarkerSize(0.6);

        deviation_pt[i]->SetLineColor(color[1]);
        deviation_pt[i]->SetMarkerColor(color[1]);
        deviation_pt[i]->SetMarkerStyle(mstyle[1]);
        deviation_pt[i]->SetMarkerSize(0.6);

        double length = pymax-pymin;

        TPaveText *ppt = plt::NewTextBox(ptext_x_min,length*0.85+pymin,ptext_x_max,length*0.95+pymin);
        ppt->AddText(text);
        ppt->Draw();
        plt::DrawCMSLabel();
        plt::DrawLuminosity(lumi);

        
        TLegend *leg ;
        
        if(i==0){
           leg  = plt::NewLegend(eleg_x_min,eleg_y_min-0.18,eleg_x_max,eleg_y_max-0.08);
        }
        else{
            leg = plt::NewLegend(eleg_x_min+0.1,eleg_y_min-0.18,eleg_x_max+0.1,eleg_y_max-0.08);
        }
        leg->SetLineColor(kWhite);
        leg->SetTextSize( 15 );
        leg->AddEntry(origin_pt[i],pdesc[i].c_str(),"lp");
        leg->AddEntry(deviation_pt[i], (pdesc[i]+" (modified)").c_str(),"lp");
        leg->AddEntry((TObject*)0,modif,"");
        leg->Draw();

        pad22->cd();

        TH1F* h33=gPad->DrawFrame(pframe_x_min,-0.02,pframe_x_max,0.02,"");
        plt::SetAxis(h33);
        h33->GetXaxis()->SetTitle("Pt [GeV]");
        h33->GetYaxis()->SetTitle("Systematic error");
        h33->GetYaxis()->SetLabelSize( 10 );
        h33->GetXaxis()->SetTitleOffset( 3 );
        h33->GetYaxis()->SetTitleSize(18);
        ratio->Draw("EP same");
        ratio->SetMarkerStyle(21);
        ratio->SetStats("kFalse");
        
        TLine *line = new TLine(0,0,130,0);
        line->SetLineColor(kRed);
        line->SetLineStyle(8);
        line->Draw();

        plt::SaveToPDF(c,Form("syserr_pt%d.pdf",i));
        

        TCanvas* c1 = new TCanvas();
        TPad* pad1 = plt::NewTopPad();
        TPad* pad2 = plt::NewBottomPad();
        pad1->Draw();
        pad1->SetPad(0,0.28,1,1);
        pad2->Draw();
        pad1->cd();

        
        double ymax;
        if(plt::GetYmax(origin_pt[i]) > plt::GetYmax(deviation_eta[i]))
            ymax = plt::GetYmax(origin_eta[i]);
        else
            ymax = plt::GetYmax(deviation_eta[i]);

        ymax *= 1.3;


        TH1F* h2=gPad->DrawFrame(eframe_x_min,eframe_y_min,eframe_x_max,ymax,"");
        plt::SetAxis(h2);
        h2->GetXaxis()->SetTitle("");
        h2->GetYaxis()->SetTitle("Scale Factor");
        h2->GetXaxis()->SetLabelSize(0);
        origin_eta   [i]->Draw("EP same");
        deviation_eta[i]->Draw("EP same");

        origin_eta   [i]->SetLineColor(color[0]);
        origin_eta   [i]->SetMarkerColor(color[0]);
        origin_eta   [i]->SetMarkerStyle(mstyle[0]);
        origin_eta   [i]->SetMarkerSize(0.6);

        deviation_eta[i]->SetLineColor(color[1]);
        deviation_eta[i]->SetMarkerColor(color[1]);
        deviation_eta[i]->SetMarkerStyle(mstyle[1]);
        deviation_eta[i]->SetMarkerSize(0.6);

        
        TPaveText *pt = plt::NewTextBox(etext_x_min,ymax*0.85,etext_x_max,ymax*0.95);
        pt->AddText(text);
        pt->Draw();
        plt::DrawCMSLabel();
        plt::DrawLuminosity(lumi);

        TLegend *legg = plt::NewLegend(eleg_x_min-0.04,eleg_y_min-0.03,eleg_x_max-0.04,eleg_y_max);
        legg->SetLineColor(kWhite);
        legg->AddEntry(origin_pt[i],edesc[i].c_str(),"lp");
        legg->AddEntry(deviation_pt[i], (edesc[i]+" (modified)").c_str(),"lp");
        legg->AddEntry((TObject*)0,modif,"");
        legg->Draw();

        pad2->cd();


        TH1F* h3=gPad->DrawFrame(eframe_x_min,-0.02,eframe_x_max,0.02,"");
        plt::SetAxis(h3);
        h3->GetXaxis()->SetTitle("Eta ");
        h3->GetYaxis()->SetTitle("Systematic error");
        h3->GetYaxis()->SetLabelSize( 10 );
        h3->GetXaxis()->SetTitleOffset( 3 );
        h3->GetYaxis()->SetTitleSize(18);
        _ratio->Draw("EP same");
        _ratio->SetMarkerStyle(21);
        _ratio->SetStats("kFalse");
        
        TLine *linee = new TLine(-3,0,3,0);
        linee->SetLineColor(kRed);
        linee->SetLineStyle(8);
        linee->Draw();

        plt::SaveToPDF(c1,Form("syserr_eta%d.pdf",i));
        
    }
}
