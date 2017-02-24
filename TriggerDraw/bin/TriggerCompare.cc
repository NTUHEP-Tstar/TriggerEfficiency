#include <iostream>
#include <vector>
#include <math.h>


#include "ManagerUtils/PlotUtils/interface/Common.hpp"
#include "TriggerEfficiency/TriggerDraw/interface/Readmgr.hpp"
#include "TriggerEfficiency/TriggerDraw/interface/Parsermgr.hpp"
#include "TriggerEfficiency/TriggerDraw/interface/Trigger.hpp"
#include "TriggerEfficiency/TriggerDraw/interface/eConstant.hpp"
#include "TriggerEfficiency/TriggerDraw/interface/Common.hpp"


using namespace std;
namespace opt = boost::program_options;

int main(int argc, char* argv[]){

    opt::options_description de( "Command for TriggerCompare" );
    de.add_options()
        ( "lepton,l", opt::value<string>()->required(), "Compare which lepton" )
        ( "run,r", opt::value<string>()->required(), "Use which era of input" )
        ( "method,m", opt::value<string>()->required(), "Output file name" )
    ;

    dra::Parsermgr trinamer("TriggerDraw");
    trinamer.AddOptions( de );
    const int run = trinamer.ParseOptions( argc, argv );
    if( run == dra::Parsermgr::HELP_PARSER  ){ return 0; }
    if( run == dra::Parsermgr::FAIL_PARSER ){ return 1; }
    trinamer.SetFileName( {"method","run"}  );

    /******************************************************************************************************/

    dra::Trigger mtri27("ele27",true);
    dra::Trigger mtri32("ele32",true);
    dra::Trigger mtrigger[]={mtri27,mtri32};

    dra::Trigger dtri27("ele27",false);
    dra::Trigger dtri32("ele32" ,false);
    dra::Trigger dtrigger[]={dtri27,dtri32};


    vector<string> triggerlist = trinamer.GetListData<string>("triggerlist");
    vector<string> triggername = trinamer.GetListData<string>("triggername");
    vector<string> pcut        = trinamer.GetListData<string>("pcut");
    vector<string> ecut        = trinamer.GetListData<string>("ecut");
    
    TFile f(trinamer.GetFileName("Analyz","root").c_str(),"RECREATE");


    for(int i=0; i<2; i++)
    {

        vector<double> _pbin;
        vector<double> _ebin;

        _pbin = dra::GetList<double>("pt" ,trinamer.GetSubTree(triggerlist[i]));
        _ebin = dra::GetList<double>("eta",trinamer.GetSubTree(triggerlist[i]));
        
        double* pbin = &_pbin[0];
        double* ebin = &_ebin[0];
        double  pbinnum = _pbin.size();
        double  ebinnum = _ebin.size();

        TGraphAsymmErrors* mPtEff  = mtrigger[i].getPtEff();
        mPtEff->SetName( ("mPt"+triggername[i]).c_str());
        TGraphAsymmErrors* mEtaEff = mtrigger[i].getEtaEff();
        mEtaEff->SetName( ("mEta"+triggername[i]).c_str());
        TGraphAsymmErrors* dPtEff  = dtrigger[i].getPtEff();
        dPtEff->SetName( ("dPt"+triggername[i]).c_str());
        TGraphAsymmErrors* dEtaEff = dtrigger[i].getEtaEff();
        dEtaEff->SetName( ("dEta"+triggername[i]).c_str());

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
        lleg->AddEntry(("mPt"+triggername[i]).c_str(),("MC   "+triggername[i]+" ("+ecut[i]+")").c_str(),"lp");
        lleg->AddEntry(("dPt"+triggername[i]).c_str(),("Data "+triggername[i]+" ("+ecut[i]+")").c_str(),"lp");
        lleg->SetTextSize(14);
        lleg->Draw();
    
        pad22->cd();

        TH1D* _ratio = new TH1D(("pt_"+triggerlist[i]).c_str(),"",pbinnum-1,pbin);
        
        for(int j=1;j<pbinnum;j++){
            double deff = dPtEff->GetY()[j-1];
            double meff = mPtEff->GetY()[j-1];
            double derr = dPtEff->GetErrorY(j-1);
            double merr = mPtEff->GetErrorY(j-1);
            double seff;
            double serr;
            if(meff!=0){
                seff= deff/meff;
                serr = dra::ErrorProp(deff,derr,meff,merr);
            }
            else{
                seff=0;
                serr=0;
            }
            _ratio->SetBinContent(j,seff);
            _ratio->SetBinError(j,serr);
        }

        _ratio->Write();


        TH1F* h33;
        h33=gPad->DrawFrame(pframe_x_min,0.8,pframe_x_max,1.2,"");
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

        plt::SaveToPDF(c,trinamer.GetFileName("pt_"+triggerlist[i],"pdf"));




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
        leg->AddEntry(("mEta"+triggername[i]).c_str(),("MC   "+triggername[i]+" ("+pcut[i]+"GeV)").c_str(),"lp");
        leg->AddEntry(("dEta"+triggername[i]).c_str(),("Data "+triggername[i]+" ("+pcut[i]+"GeV)").c_str(),"lp");
        leg->SetTextSize(14);
        leg->Draw();
    

        pad2->cd();

        TH1D* ratio = new TH1D(("eta"+triggername[i]).c_str(),"",ebinnum-1,ebin);
        
        for(int j=1;j<ebinnum;j++){
            double deff = dEtaEff->GetY()[j-1];
            double meff = mEtaEff->GetY()[j-1];
            double derr = dEtaEff->GetErrorY(j-1);
            double merr = mEtaEff->GetErrorY(j-1);
            double seff = deff/meff;
            double serr = dra::ErrorProp(deff,derr,meff,merr);
            ratio->SetBinContent(j,seff);
            ratio->SetBinError(j,serr);
        
        }

        ratio->Write();

        TH1F* h3=gPad->DrawFrame(eframe_x_min,0.8,eframe_x_max,1.2,"");
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
        
        plt::SaveToPDF(c,trinamer.GetFileName("eta_"+triggerlist[i],"pdf"));

        delete _ratio;
        delete ratio;
        delete c1;

        f.Write();
    }


    f.Close();


}
