#include "TriggerEfficiency/TriggerDraw/interface/PlotObj.hpp"
#include "TFile.h"
#include "TDirectory.h"
#include "TCanvas.h"
#include "TGraphErrors.h"
#include <iostream>
/*******************************************************************************
*   Global function
*******************************************************************************/
extern void drawtest(){
    TCanvas* c = new TCanvas();
    (trinamer.GetDataPt()) -> Draw("AEP");
    c->SaveAs("test.pdf");

    cout<< (trinamer.GetDataPt()) ->GetY()[3] <<" "<<(trinamer.GetDataPt()) ->GetErrorY(3)<<endl;
}

extern void PlotSysError(const string& tri){
    InitSysOri(tri);
  
    drawtest();
    //    InitSysNew(tri);

//    SetGraphName(tri);

//    PlotPt(tri);
//    PlotEta(tri);

//    Clean();
}

extern void ConvertTGraph(TGraph* g, TH1* h){
    double binnum = h->GetXaxis()->GetNbins();
    cout<<"binnum "<<binnum<<endl;
    
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

}

extern void InitSysNew(const string& tri){
    
    TFile* file = TFile::Open( ( trinamer.GetSingleData<string>("newpath") ).c_str() );
    
    TH1D* pt;
    pt = (TH1D*) file->Get( ("pt_scale_"+tri).c_str() );
    TH1D* eta;
    eta = (TH1D*) file->Get( ("eta_scale_"+tri).c_str() );
    
    TGraph* _pt  = new TGraph(pt);
    TGraph* _eta = new TGraph(eta);

    trinamer.SetPtMC(_pt);
    trinamer.SetEtaMC(_eta);

}
