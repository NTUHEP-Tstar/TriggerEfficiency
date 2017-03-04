#include "TriggerEfficiency/TriggerDraw/interface/PlotObj.hpp"
#include "TFile.h"
#include "TDirectory.h"
#include "TCanvas.h"
#include "TGraphErrors.h"
#include <iostream>
/*******************************************************************************
*   Global function
*******************************************************************************/

extern void PlotSysError(const string& tri){

    InitSysOri(tri);
    InitSysNew(tri);

    SetGraphName(tri);

 //   PlotPt(tri);
//    PlotEta(tri);

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
