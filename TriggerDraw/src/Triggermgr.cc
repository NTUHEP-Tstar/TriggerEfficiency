#include "TriggerEfficiency/TriggerDraw/interface/Triggermgr.hpp"
#include "TDirectory.h"

using namespace std;
using namespace dra;

Triggermgr::Triggermgr(const string& subdir, const string& file, const string& t):
    Pathmgr("TriggerEfficiency", subdir),
    Readmgr( SettingsDir() / "eTrigger.json")
{
    File = new TFile( ( DatasDir() / file ).c_str() );
    TDirectory* dir = File->GetDirectory("demo");
    
    dir->GetObject( ("pass_pt_"+t).c_str(),passPt);
    dir->GetObject( ("pass_eta_"+t).c_str(),passEta);
    dir->GetObject( ("total_pt_"+t).c_str(),totalPt);
    dir->GetObject( ("total_eta_"+t).c_str(),totalEta);

    effPt =new TGraphAsymmErrors(passPt ,totalPt ,"b");
    effEta=new TGraphAsymmErrors(passEta,totalEta,"b");

}

Triggermgr::~Triggermgr(){
    delete File;
    delete effPt;
    delete effEta;
}


TGraphAsymmErrors* Triggermgr::getPtEff(){
    return effPt;
}

TGraphAsymmErrors* Triggermgr::getEtaEff(){
    return effEta;
}
