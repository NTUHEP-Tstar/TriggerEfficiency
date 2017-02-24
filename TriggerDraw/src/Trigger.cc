#include "TriggerEfficiency/TriggerDraw/interface/Trigger.hpp"
#include "TFile.h"
#include "TDirectory.h"
using namespace dra;
using namespace std;

Trigger::Trigger(const string& t, bool monte){
    if(monte){
        
        TFile* File = new TFile("/wk_cms/sam7k9621/CMSSW_8_0_10/src/TriggerEfficiency/TriggerAnalyzer/data/MC_data/electron/electron_MC_analyz.root");
        TDirectory* dir = File->GetDirectory("demo");
        
        dir->GetObject( ("pass_pt_"+t).c_str(),passPt);
        dir->GetObject( ("pass_eta_"+t).c_str(),passEta);
        dir->GetObject( ("total_pt_"+t).c_str(),totalPt);
        dir->GetObject( ("total_eta_"+t).c_str(),totalEta);
        
    }
    
    else{
            
        TFile* File = new TFile("/wk_cms/sam7k9621/CMSSW_8_0_10/src/TriggerEfficiency/TriggerAnalyzer/data/electron/electron_anlyz.root");
        TDirectory* dir = File->GetDirectory("demo");
        
        dir->GetObject( ("pass_pt_"+t).c_str(),passPt);
        dir->GetObject( ("pass_eta_"+t).c_str(),passEta);
        dir->GetObject( ("total_pt_"+t).c_str(),totalPt);
        dir->GetObject( ("total_eta_"+t).c_str(),totalEta);
    
    }
        
      
    effPt =new TGraphAsymmErrors(passPt ,totalPt ,"b");
    effEta=new TGraphAsymmErrors(passEta,totalEta,"b");
}

TGraphAsymmErrors* Trigger::getPtEff(){
    return effPt;
}

TGraphAsymmErrors* Trigger::getEtaEff(){
    return effEta;
}
