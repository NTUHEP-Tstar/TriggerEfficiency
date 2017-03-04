#include "TriggerEfficiency/TriggerDraw/interface/PlotObj.hpp"
#include "ManagerUtils/PlotUtils/interface/Common.hpp"
#include "TriggerEfficiency/TriggerDraw/interface/eConstant.hpp"
#include <iostream>
#include "TCanvas.h"

/*******************************************************************************
*   Global variable
*******************************************************************************/

Color_t color[10] = {kRed+2,kBlue-7,kGreen+3,kMagenta+2,kYellow-3,kGreen-9,kBlue-9,kRed-9,kCyan+3,kGray };

Style_t  mstyle[10] = {20,21,24,25,31,33,27,23,22,26};

dra::Triggermgr trinamer("TriggerDraw");


/*******************************************************************************
*   Global function
*******************************************************************************/

extern void SaveToRoot(TH1* h, const string& fname, const string& objname){
    TFile f(fname.c_str(),"UPDATE");
    h->Write( objname.c_str(), TFile::kOverwrite  );

}

extern void Clean(){
    delete trinamer.GetDataPt();
    delete trinamer.GetDataEta();
    delete trinamer.GetMCPt();
    delete trinamer.GetMCEta();

    trinamer.SetPtData ( NULL);
    trinamer.SetEtaData( NULL);
    trinamer.SetPtMC   ( NULL);
    trinamer.SetEtaMC  ( NULL);

}

extern TH1D* SetComGraph(vector<double>& _bin, TGraph* dEff, TGraph* mEff){
   
    double* bin    = &_bin[0];
    double  binnum = _bin.size();
    

    TH1D* ratio = new TH1D("ratio","",binnum-1,bin);
            
    for(int j=1;j<binnum;j++){
        double deff = dEff->GetY()[j-1];
        double meff = mEff->GetY()[j-1];
        double derr = dEff->GetErrorY(j-1);
        double merr = mEff->GetErrorY(j-1);
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
        
        ratio->SetBinContent(j,seff);
        ratio->SetBinError(j,serr);    
    }

    return ratio;
}

extern void SetGraph(TGraph* mEff, TGraph* dEff){

    mEff->SetLineColor(color[0]);
    mEff->SetMarkerColor(color[0]);
    mEff->SetMarkerStyle(mstyle[0]);
    mEff->SetMarkerSize(0.6);

    dEff->SetLineColor(color[1]);
    dEff->SetMarkerColor(color[1]);
    dEff->SetMarkerStyle(mstyle[1]);
    dEff->SetMarkerSize(0.6);
}


extern void PlotEta(const string& tri){


    string triname = dra::GetSingle<string>("triname"    ,trinamer.GetSubTree(tri));
    string pcut    = dra::GetSingle<string>("pcut"       ,trinamer.GetSubTree(tri));
    
    //initial pbin
    vector<double> _ebin = dra::GetList<double>("eta" ,trinamer.GetSubTree(tri));
    TH1D* ratio = SetComGraph(_ebin, trinamer.GetDataEta(), trinamer.GetMCEta());
    
    SetGraph(trinamer.GetDataEta(), trinamer.GetMCEta());

    /******************************************************************************************/
    
    TCanvas* c = plt::NewCanvas();
    TPad* pad11= plt::NewTopPad();
    TPad* pad22= plt::NewBottomPad();
    pad11->Draw();
    pad22->Draw();

    /******************************************************************************************/
    
    pad11->cd();

    TH1F* h22=gPad->DrawFrame(eframe_x_min,eframe_y_min,eframe_x_max,eframe_y_max,"");
    plt::SetAxis(h22);
    h22->GetXaxis()->SetTitle("");
    h22->GetYaxis()->SetTitle("Efficiency");
    h22->GetXaxis()->SetLabelSize(0 );
    ( trinamer.GetDataEta() )->Draw("EP same");
    ( trinamer.GetMCEta()   )->Draw("EP same");


    TLine* line = SetTLine(eline_x_min,eline_y_min,eline_x_max,eline_y_max);
    TLegend* leg = SetTLeg(tri, triname, pcut,eleg_x_min,eleg_y_min,eleg_x_max,eleg_y_max);
    TPaveText* pave = SetTPave(etext_x_min,etext_y_min,etext_x_max,etext_y_max);
    plt::DrawCMSLabel();
    plt::DrawLuminosity(lumi);


    /******************************************************************************************/
    
    pad22->cd();
    TH1F* h33=gPad->DrawFrame(eframe_x_min,0.8,eframe_x_max,1.2,"");
    plt::SetAxis(h33);
    h33->GetXaxis()->SetTitle("Eta");
    h33->GetYaxis()->SetTitle("Data / MC");
    h33->GetYaxis()->SetLabelSize( 12 );
    h33->GetXaxis()->SetTitleOffset( 3 );
    ratio->Draw("EP same");
    ratio->SetMarkerStyle(21);
    ratio->SetStats(kFALSE);

    TLine* line1 = SetTLine(eline_x_min,eline_y_min,eline_x_max,eline_y_max);

    
    /******************************************************************************************/
    plt::SaveToPDF( c, trinamer.GetFileName( "eta_"+tri, "pdf" ) );
    SaveToRoot(ratio, trinamer.GetFileName("","root")  , "pt_"+tri );

    delete pad11;
    delete pad22;
    delete line;
    delete line1;
    delete leg;
    delete pave;
    delete c;

    delete ratio;

}



extern void PlotPt(const string& tri){
    

    //http://einverne.github.io/post/2016/01/boost-learning-note-7.html

    string triname = dra::GetSingle<string>("triname"    ,trinamer.GetSubTree(tri));
    string ecut    = dra::GetSingle<string>("ecut"       ,trinamer.GetSubTree(tri));
    
    //initial pbin
    vector<double> _pbin = dra::GetList<double>("pt" ,trinamer.GetSubTree(tri));
    TH1D* ratio = SetComGraph(_pbin, trinamer.GetDataPt(), trinamer.GetMCPt());
    
    SetGraph(trinamer.GetDataPt(), trinamer.GetMCPt());

    /******************************************************************************************/
    
    TCanvas* c = plt::NewCanvas();
    TPad* pad11= plt::NewTopPad();
    TPad* pad22= plt::NewBottomPad();
    pad11->Draw();
    pad22->Draw();

    /******************************************************************************************/
    
    pad11->cd();

    TH1F* h22=gPad->DrawFrame(pframe_x_min,pframe_y_min,pframe_x_max,pframe_y_max,"");
    plt::SetAxis(h22);
    h22->GetXaxis()->SetTitle("");
    h22->GetYaxis()->SetTitle("Efficiency");
    h22->GetXaxis()->SetLabelSize(0 );
    ( trinamer.GetDataPt() )->Draw("EP same");
    ( trinamer.GetMCPt()   )->Draw("EP same");


    TLine* line = SetTLine(pline_x_min,pline_y_min,pline_x_max,pline_y_max);
    TLegend* leg = SetTLeg(tri, triname, ecut,pleg_x_min,pleg_y_min,pleg_x_max,pleg_y_max);
    TPaveText* pave = SetTPave(ptext_x_min,ptext_y_min,ptext_x_max,ptext_y_max);
    plt::DrawCMSLabel();
    plt::DrawLuminosity(lumi);


    /******************************************************************************************/

    pad22->cd();
    TH1F* h33=gPad->DrawFrame(pframe_x_min,0.8,pframe_x_max,1.2,"");
    plt::SetAxis(h33);
    h33->GetXaxis()->SetTitle("Pt [GeV]");
    h33->GetYaxis()->SetTitle("Data / MC");
    h33->GetYaxis()->SetLabelSize( 12 );
    h33->GetXaxis()->SetTitleOffset( 3 );
    ratio->Draw("EP same");
    ratio->SetMarkerStyle(21);
    ratio->SetStats(kFALSE);
    

    TLine* line1 = SetTLine(pline_x_min,pline_y_min,pline_x_max,pline_y_max);
    
    /******************************************************************************************/
    plt::SaveToPDF( c, trinamer.GetFileName( "pt_"+tri, "pdf" ) );
    SaveToRoot(ratio, trinamer.GetFileName("","root")  , "pt_"+tri );

    delete pad11;
    delete pad22;
    delete line;
    delete line1;
    delete leg;
    delete pave;
    delete c;
    delete ratio;
}

extern void SetGraphName(const string& tri){

    trinamer.GetDataPt()->SetName( ("d"+tri).c_str()  );
    trinamer.GetMCPt()  ->SetName( ("m"+tri).c_str()  );
    trinamer.GetDataEta()->SetName( ("d"+tri).c_str()  );
    trinamer.GetMCEta()  ->SetName( ("m"+tri).c_str()  );
}

extern TLine* SetTLine(const double& xmin, const double& ymin, const double& xmax, const double& ymax ){
    TLine* line = new TLine(xmin,ymin,xmax,ymax);
    line->SetLineColor(kRed);
    line->SetLineStyle(8);
    line->Draw();
    return line;
}

extern TPaveText* SetTPave(const double& xmin, const double& ymin, const double& xmax, const double& ymax){
    TPaveText *pt = plt::NewTextBox(xmin,ymin,xmax,ymax);
    pt->AddText(text);
    pt->Draw();
    return pt;
}

extern TLegend* SetTLeg(const string& tri,const string& triname,const string& cut,const double& xmin,const double& ymin,const double& xmax,const double& ymax){
    
    TLegend* leg = plt::NewLegend(xmin,ymin,xmax,ymax);
    leg->SetLineColor(kWhite);
    leg->AddEntry( ("m"+tri).c_str(), ( "MC   "+triname+" ("+cut+")" ).c_str(), "lp");
    leg->AddEntry( ("d"+tri).c_str(), ( "Data "+triname+" ("+cut+")" ).c_str(), "lp");
    leg->SetTextSize(14);
    leg->Draw();
    return leg;
}
