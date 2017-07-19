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
    PlotSysErrorPt(tri);
    PlotSysErrorEta(tri);

    Clean();
}

extern void SetPath(string& p,const string& m){
    string l = trinamer.GetOption<string>("lepton") ;
    string r = trinamer.GetOption<string>("run");
    if(m == "")
        p = trinamer.ResultsDir() /  l + "_" + r + ".root";
    else
        p = trinamer.ResultsDir() /  l + "_" + r + "_" + m + ".root";
    cout<<"Using file : "<<p<<endl;
}

extern void InitSysOri(const string& tri){
   
    string path;

    SetPath(path);
    TFile* file = TFile::Open( path.c_str() );
    
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
    
    string path;
    SetPath(path, trinamer.GetOption<string>("method"));
    TFile* file = TFile::Open( path.c_str() );
    
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
   
    /******************************************************************************************/
    
    pad11->cd();
    TH1F* h22=gPad->DrawFrame(pframe_x_min,0.95,pframe_x_max,1.05,"");
    SetHist(h22,"","Scale Factor");
    ( trinamer.GetDataPt() )->Draw("EP same");
    ( trinamer.GetMCPt()   )->Draw("EP same");

    string mod = trinamer.GetSingleData<string>( trinamer.GetOption<string>("method")  );
    TLegend* leg = SetTLeg(0.4,0.1,0.5,0.3);
    leg->AddEntry( ("d"+tri).c_str(), ( triname+" ("+ecut+")"               ).c_str(), "lp");
    leg->AddEntry( ("m"+tri).c_str(), ( triname+" ("+ecut+")" + "(modified)").c_str(), "lp");
    leg->AddEntry((TObject*)0, mod.c_str()  ,"");
    TPaveText* pave = SetTPave(ptext_x_min,0.95+0.1*0.85,ptext_x_max,0.95+0.1*0.95);
    plt::DrawCMSLabel();
    plt::DrawLuminosity(lumi);

    /******************************************************************************************/

    pad22->cd();
    TH1F* h33=gPad->DrawFrame(pframe_x_min,-0.02,pframe_x_max,0.02,"");
    SetHist(h33,"Pt [GeV]","Systematic error",10);
    h33->GetYaxis()->SetLabelSize( 12 );
    h33->GetXaxis()->SetTitleOffset( 3 );
    TH1D* ratio = SetComGraph(_pbin, trinamer.GetDataPt(), trinamer.GetMCPt(),"error");
    ratio->Draw("EP same");

    TLine* line1 = SetTLine(0,0,220,0);
    
    SaveToRoot(ratio, trinamer.GetFileName("scale","root"),"pt_"+tri);
    plt::SaveToPDF( c, trinamer.GetFileName( "scale_pt_"+tri, "pdf" ) );

    /******************************************************************************************/
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

    string mod = trinamer.GetSingleData<string>( trinamer.GetOption<string>("method")  );
    TLegend* leg = SetTLeg(eleg_x_min-0.04,eleg_y_min-0.03,eleg_x_max-0.04,eleg_y_max);
    leg->AddEntry( ("d"+tri).c_str(), ( triname+" ("+pcut+")"               ).c_str(), "lp");
    leg->AddEntry( ("m"+tri).c_str(), ( triname+" ("+pcut+")" + "(modified)").c_str(), "lp");
    leg->AddEntry((TObject*)0, mod.c_str()  ,"");
    TPaveText* pave = SetTPave(etext_x_min,ymax*0.85,etext_x_max,ymax*0.95);
    plt::DrawCMSLabel();
    plt::DrawLuminosity(lumi);
    /******************************************************************************************/
    pad22->cd();
    TH1F* h33=gPad->DrawFrame(eframe_x_min,-0.02,eframe_x_max,0.02,"");
    SetHist(h33,"Eta","Systematic error",10);
    h33->GetYaxis()->SetLabelSize( 12 );
    h33->GetXaxis()->SetTitleOffset( 3 );
    TH1D* ratio = SetComGraph(_ebin, trinamer.GetDataEta(), trinamer.GetMCEta(),"error");
    ratio->Draw("EP same");

    TLine* line1 = SetTLine(-3,0,3,0);
   
    SaveToRoot(ratio, trinamer.GetFileName("scale","root"),"eta_"+tri);
    plt::SaveToPDF( c, trinamer.GetFileName( "scale_eta_"+tri, "pdf" ) );
    /******************************************************************************************/
    delete pad11;
    delete pad22;
    delete line1;
    delete leg;
    delete pave;
    delete c;
    delete ratio;
    
}
