#include "TriggerEfficiency/TriggerDraw/interface/PlotObj.hpp"
#include "TGraphAsymmErrors.h"
#include "TFile.h"
#include "TDirectory.h"
#include <iostream>
/*******************************************************************************
*   Global function
*******************************************************************************/

extern void PlotCompare(const string& tri){
    InitComMC(tri);
    InitComData(tri);
    
    SetGraphName(tri);
    
    PlotPt(tri) ;
    PlotEta(tri);

    Clean();
}

extern void InitComData(const string& tri){ 

    TFile* file = TFile::Open( ( trinamer.GetSingleData<string>("datapath") ).c_str()  );
    TDirectory* dir = file->GetDirectory("demo");
    
    TH1D* passPt;
    TH1D* passEta;
    TH1D* totalPt;
    TH1D* totalEta;

    dir->GetObject( ("pass_pt_"  +tri).c_str(), passPt  );
    dir->GetObject( ("pass_eta_" +tri).c_str(), passEta );
    dir->GetObject( ("total_pt_" +tri).c_str(), totalPt );
    dir->GetObject( ("total_eta_"+tri).c_str(), totalEta);
    
    TGraphAsymmErrors* peff = new TGraphAsymmErrors() ; 
    TGraphAsymmErrors* eeff = new TGraphAsymmErrors();
    peff->BayesDivide(passPt,  totalPt,  "b");
    eeff->BayesDivide(passEta, totalEta,"b");
    
    trinamer.SetPtData( peff  );
    trinamer.SetEtaData( eeff  );

    file->Close();
}


extern void InitComMC(const string& tri){

    TFile* file = TFile::Open( ( trinamer.GetSingleData<string>("mcpath") ).c_str() );
    TDirectory* dir = file->GetDirectory("demo");
    
    TH1D* passPt;
    TH1D* passEta;
    TH1D* totalPt;
    TH1D* totalEta;

    dir->GetObject( ("pass_pt_"  +tri).c_str(),passPt);
    dir->GetObject( ("pass_eta_" +tri).c_str(),passEta);
    dir->GetObject( ("total_pt_" +tri).c_str(),totalPt);
    dir->GetObject( ("total_eta_"+tri).c_str(),totalEta);
    
    TGraphAsymmErrors* peff = new TGraphAsymmErrors() ; 
    TGraphAsymmErrors* eeff = new TGraphAsymmErrors();
    peff->BayesDivide(passPt,  totalPt, "b");
    eeff->BayesDivide(passEta, totalEta,"b");
    
    trinamer.SetPtMC( peff  );
    trinamer.SetEtaMC( eeff  );

    file->Close();
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
