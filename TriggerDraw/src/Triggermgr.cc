#include "TriggerEfficiency/TriggerDraw/interface/Triggermgr.hpp"
#include "TDirectory.h"

using namespace std;
using namespace dra;

Triggermgr::Triggermgr(const string& subdir):
    Pathmgr("TriggerEfficiency", subdir),
    Readmgr( SettingsDir() / "eTrigger.json"),
    Parsermgr()
{

}

Triggermgr::~Triggermgr(){
    delete datapt;
    delete dataeta;
    delete mcpt;
    delete mceta;
}

string Triggermgr::GetFileName(const string& prefix, const string& type){
    string ans = ""; 
    for( auto& name : GetNamelist() ){
        ans += ( "_" + OptName(name) );
    }
    if(prefix == ""){
        ans.erase(ans.begin());
    }
    return ResultsDir() / ( prefix+ans+"."+type );
}

/***************************************/

void Triggermgr::SetPtData(TGraph* t){
    datapt = t;
}
void Triggermgr::SetEtaData(TGraph* t){
    dataeta = t;
}
void Triggermgr::SetPtMC(TGraph* t){
    mcpt = t;
}
void Triggermgr::SetEtaMC(TGraph* t){
    mceta = t;
}

/***************************************/

TGraph* Triggermgr::GetDataPt(){
    return datapt;
}
TGraph* Triggermgr::GetDataEta(){
    return dataeta;
}
TGraph* Triggermgr::GetMCPt(){
    return mcpt;
}
TGraph* Triggermgr::GetMCEta(){
    return mceta;
}


/***************************************/
