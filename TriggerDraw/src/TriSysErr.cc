#include "TriggerEfficiency/TriggerDraw/interface/PlotObj.hpp"
#include "TFile.h"
#include "TDirectory.h"

/*******************************************************************************
*   Global function
*******************************************************************************/


extern void PlotSysError(const string& tri){
    InitSysOri(tri);
    InitSysNew(tri);

    SetGraphName(tri);

//    PlotPt(tri);
//    PlotEta(tri);

    Clean();
}

extern void InitSysOri(const string& tri){
    
    TFile* file = TFile::Open( ( trinamer.GetSingleData<string>("oripath") ).c_str() );
    
    TH1D* pt;
    pt = (TH1D*) file->Get( ("pt_scale_"+tri).c_str() );
    TH1D* eta;
    eta = (TH1D*) file->Get( ("eta_scale_"+tri).c_str() );
    
    TGraph* _pt  = new TGraph(pt);
    TGraph* _eta = new TGraph(eta);

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
