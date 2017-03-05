#include "TriggerEfficiency/TriggerDraw/interface/PlotObj.hpp"
#include "TriggerEfficiency/TriggerDraw/interface/eConstant.hpp"
#include "ManagerUtils/PlotUtils/interface/Common.hpp"
#include "TGraphErrors.h"
#include "TFile.h"
#include "TDirectory.h"
#include "TCanvas.h"
#include <iostream>
/*******************************************************************************
*   Global function
*******************************************************************************/

extern void PlotSysError(const string& tri){
    InitSysOri(tri);
    InitSysNew(tri);

    SetGraphName(tri);

//    PlotSysErrorPt(tri);
    PlotSysErrorEta(tri);

    Clean();
}

extern void InitSysOri(const string& tri){
    
    TFile* file = TFile::Open( ( trinamer.GetSingleData<string>("oripath") ).c_str() );
    
    TH1D* pt;
    pt = (TH1D*) file->Get( ("pt_"+tri).c_str() );
    TH1D* eta;
    eta = (TH1D*) file->Get( ("eta_"+tri).c_str() );
    
    TGraphErrors* _pt  = new TGraphErrors(pt);
    TGraphErrors* _eta = new TGraphErrors(eta);


    trinamer.SetPtData(_pt);
    trinamer.SetEtaData(_eta);

    file->Close();
}

extern void InitSysNew(const string& tri){
    
    TFile* file = TFile::Open( ( trinamer.GetSingleData<string>("newpath") ).c_str() );
    
    TH1D* pt;
    pt = (TH1D*) file->Get( ("pt_"+tri).c_str() );
    TH1D* eta;
    eta = (TH1D*) file->Get( ("eta_"+tri).c_str() );
    
    TGraphErrors* _pt  = new TGraphErrors(pt);
    TGraphErrors* _eta = new TGraphErrors(eta);

    trinamer.SetPtMC(_pt);
    trinamer.SetEtaMC(_eta);

    file->Close();
}


extern void PlotSysErrorPt(const string& tri){
    string triname = dra::GetSingle<string>("triname"    ,trinamer.GetSubTree(tri));
    string ecut    = dra::GetSingle<string>("ecut"       ,trinamer.GetSubTree(tri));
    vector<double> _pbin = dra::GetList<double>("pt" ,trinamer.GetSubTree(tri));
    
    SetGraph(trinamer.GetDataPt(), trinamer.GetMCPt());
    /******************************************************************************************/
    TCanvas* c = plt::NewCanvas();
    TPad* pad11= plt::NewTopPad();
    TPad* pad22= plt::NewBottomPad();
    pad11->Draw();
    pad22->Draw();
    
    pad11->cd();
    TH1F* h22=gPad->DrawFrame(pframe_x_min,pframe_y_min,pframe_x_max,pframe_y_max,"");
    plt::SetAxis(h22);
    SetHist(h22,"","Scale Factor");
    ( trinamer.GetDataPt() )->Draw("EP same");
    ( trinamer.GetMCPt()   )->Draw("EP same");

    TLegend* leg = SetTLeg(tri, triname, ecut,pleg_x_min,pleg_y_min,pleg_x_max,pleg_y_max);
    TPaveText* pave = SetTPave(ptext_x_min,1.03,ptext_x_max,1.14);
    plt::DrawCMSLabel();
    plt::DrawLuminosity(lumi);

    /******************************************************************************************/

    pad22->cd();
    TH1F* h33=gPad->DrawFrame(pframe_x_min,-0.02,pframe_x_max,0.02,"");
    SetHist(h33,"Pt [GeV]","Systematic error");
    h33->GetYaxis()->SetLabelSize( 12 );
    h33->GetXaxis()->SetTitleOffset( 3 );
    TH1D* ratio = SetComGraph(_pbin, trinamer.GetDataPt(), trinamer.GetMCPt(),"compare");
    ratio->Draw("EP same");

    TLine* line1 = SetTLine(0,0,130,0);
    
    /******************************************************************************************/
    plt::SaveToPDF( c, trinamer.GetFileName( "pt_"+tri, "pdf" ) );

    delete pad11;
    delete pad22;
    delete line1;
    delete leg;
    delete pave;
    delete c;
    delete ratio;

}



extern void PlotSysErrorEta(const string& tri){
    string triname = dra::GetSingle<string>("triname"    ,trinamer.GetSubTree(tri));
    string pcut    = dra::GetSingle<string>("pcut"       ,trinamer.GetSubTree(tri));
    vector<double> _ebin = dra::GetList<double>("eta" ,trinamer.GetSubTree(tri));
    SetGraph(trinamer.GetDataEta(), trinamer.GetMCEta());

    TCanvas* c = plt::NewCanvas();
    TPad* pad11= plt::NewTopPad();
    TPad* pad22= plt::NewBottomPad();
    pad11->Draw();
    pad22->Draw();

    /******************************************************************************************/

    double ymax = GetYLimit(trinamer.GetDataEta(),trinamer.GetMCEta());

    pad11->cd();
    TH1F* h22=gPad->DrawFrame(eframe_x_min,eframe_y_min,eframe_x_max,ymax,"");
    SetHist(h22,"","Scale Factor");
    ( trinamer.GetDataEta() )->Draw("EP same");
    ( trinamer.GetMCEta()   )->Draw("EP same");

    TLegend* leg = SetTLeg(tri, triname, pcut,eleg_x_min-0.04,eleg_y_min-0.03,eleg_x_max-0.04,eleg_y_max);
    TPaveText* pave = SetTPave(etext_x_min,ymax*0.85,etext_x_max,ymax*0.95);
    plt::DrawCMSLabel();
    plt::DrawLuminosity(lumi);
    /******************************************************************************************/
    pad22->cd();
    TH1F* h33=gPad->DrawFrame(eframe_x_min,-0.02,eframe_x_max,0.02,"");
    SetHist(h33,"Eta","Systematic error");
    h33->GetYaxis()->SetLabelSize( 12 );
    h33->GetXaxis()->SetTitleOffset( 3 );
    TH1D* ratio = SetComGraph(_ebin, trinamer.GetDataEta(), trinamer.GetMCEta(),"error");
    ratio->Draw("EP same");

    TLine* line1 = SetTLine(-3,0,3,0);
    
    plt::SaveToPDF( c, trinamer.GetFileName( "eta_"+tri, "pdf" ) );
    delete pad11;
    delete pad22;
    delete line1;
    delete leg;
    delete pave;
    delete c;
    delete ratio;
    
}
