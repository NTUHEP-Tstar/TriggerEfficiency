#include <memory>
#include <math.h>
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/FWLite/interface/Handle.h"
#include "TGraphAsymmErrors.h"
#include "TH1.h"
#include "TCanvas.h"
#include "TFile.h"
#include <iostream>
#include <fstream>
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
#include "TLorentzVector.h"
using namespace std;

class hist {
    edm::Service<TFileService> fs;
public:
    hist(double arr[],int size,string name) {
        str1="pass_";
        str2="total_";
        str1+=name;
        str2+=name;
        pass=fs->make<TH1D>(str1.c_str(),str1.c_str(),size,arr);
        total=fs->make<TH1D>(str2.c_str(),str2.c_str(),size,arr);
    }
    ~hist() {
    }
    void pfill(double d,double w) {
        pass->Fill(d,w);
    }
    void tfill(double d,double w) {
        total->Fill(d,w);
    }


private:
    TH1D* total;
    TH1D* pass;
    string str1;
    string str2;
};

class Trig {
public:

    Trig(vector<double>& ptbin,vector<double>& etabin,string name) {

        str1="pt_";
        str2="eta_";
        str1+=name;
        str2+=name;
        setArray(ptbin,etabin);
        pt =new hist(pb , ptbin.size()-1  , str1);
        eta=new hist(eb , etabin.size()-1 , str2);
    }
    ~Trig() {
        delete pb;
        delete eb;
        delete pt;
        delete eta;
    }
    void setArray(vector<double>& p,vector<double>& e) {
        pb=new double[p.size()];
        eb=new double[e.size()];

        for(int i=0; i<(int)p.size(); i++)
            pb[i]=p[i];
        for(int i=0; i<(int)e.size(); i++)
            eb[i]=e[i];

    }
    void ppfill(double d,double w) {
        pt->pfill(d,w);
    }
    void tpfill(double d,double w) {
        pt->tfill(d,w);
    }
    void pefill(double d,double w) {
        eta->pfill(d,w);
    }
    void tefill(double d,double w) {
        eta->tfill(d,w);
    }
private:
    double* pb;
    double* eb;
    hist* pt;
    hist* eta;
    string str1;
    string str2;
};

class TriggerAnalyzerMCElectron : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
public:
    explicit TriggerAnalyzerMCElectron(const edm::ParameterSet&);
    ~TriggerAnalyzerMCElectron();

    static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


private:
    virtual void beginJob() override;
    virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
    virtual void endJob() override;

    // ----------member data ---------------------------

    edm::Service<TFileService> fs;
    TH1D* zmass1;
    TH1D* zmass2;
    
    vector<Trig*> tri;
    vector<double> etabin;
    vector<edm::ParameterSet> vtri;
    vector<edm::ParameterSet> vpri;
    const edm::EDGetToken probe;
    const edm::EDGetToken tag;
    const edm::EDGetToken pusrc;
    vector<double> weights;
    vector<double> readweight(const string& );
};

TriggerAnalyzerMCElectron::TriggerAnalyzerMCElectron(const edm::ParameterSet& iConfig):
    etabin(iConfig.getParameter<vector<double> >("etabin")),
    vtri(iConfig.getParameter<vector<edm::ParameterSet>>("tagtrigger")),
    vpri(iConfig.getParameter<vector<edm::ParameterSet>>("protrigger")),
    probe ( consumes<vector<pat::Electron>>(iConfig.getParameter<edm::InputTag> ("probe") ) ),  //module
    tag ( consumes<vector<pat::Electron>>  (iConfig.getParameter<edm::InputTag>("tag") ) ),
    pusrc ( consumes<vector<PileupSummaryInfo>>(iConfig.getParameter<edm::InputTag>("pusrc") ) ),
    weights( readweight(iConfig.getParameter<edm::FileInPath>("filename").fullPath()) )
{

    zmass1 = fs->make<TH1D>("ele27","ele27",100,50,130);
    zmass2 = fs->make<TH1D>("ele32","ele32",100,50,130);

    for(int i=0; i<(int)vtri.size(); i++) {      //each pset has a corresponding Trig class
        vector<double> ptbin=vtri[i].getParameter<vector<double>>("ptbin");
        string triname = vtri[i].getParameter<string>("name");
        tri.push_back(new Trig(ptbin,etabin,triname));
    }

}


TriggerAnalyzerMCElectron::~TriggerAnalyzerMCElectron()
{
    for(int i=0;i<(int)tri.size();i++){
        delete tri[i];
    }
}


//
// member functions
//

// ------------ method called for each event  ------------
void
TriggerAnalyzerMCElectron::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
    using namespace edm;
    Handle<vector<pat::Electron> > probehandle;
    Handle<vector<pat::Electron> > taghandle;
    Handle<vector<PileupSummaryInfo> > puhandle;
    iEvent.getByToken( probe, probehandle );
    iEvent.getByToken( tag, taghandle );
    iEvent.getByToken( pusrc, puhandle );

    double pileupweight = 1.0;
    const unsigned pu = puhandle->at( 0 ).getPU_NumInteractions();
    if( pu < weights.size() ){
        pileupweight = weights.at( pu );
    }
  


    for(unsigned i=0; i<vtri.size(); i++){
        
        //set tag cut
        vector<string> thltname = vtri[i].getParameter<vector<string>>("HLT");
        double ptcut = vtri[i].getParameter<double>("ptcut");
        double etacut= vtri[i].getParameter<double>("etacut");
        //set probe cut
        vector<string> phltname = vpri[i].getParameter<vector<string>>("HLT");
        double hltpt   = vpri[i].getParameter<double>("probePtCut");
        double hlteta  = vpri[i].getParameter<double>("probeEtaCut");

        for(int j=0; j< (int)taghandle->size(); j++){
            pat::Electron el1=(*taghandle)[j];
            pat::Electron el2=(*probehandle)[j];

            /////////////////////////zmass window///////////////////////
            TLorentzVector ele1(el1.px(),el1.py(),el1.pz(),el1.energy());
            TLorentzVector ele2(el2.px(),el2.py(),el2.pz(),el2.energy());
            double mass = (ele1+ele2).M();
            if(mass<60 || mass>120)
                break;
            
            //pass tag cut
            bool passtag = false;
            for(int  k=0; k<(int)thltname.size() ; k++){
                if( el1.hasUserInt(thltname[k]) && el1.pt()>ptcut && fabs(el1.superCluster()->eta()) < etacut ){
                    passtag = true;
                    break;
                }
            }
            
            //pass probe cut
            if(!passtag)
                continue;

            //filling in total
            
            if ( fabs( el2.superCluster()->eta() ) < hlteta )
                tri[i]->tpfill(el2.pt(),pileupweight);
            if(el2.pt()>hltpt)
                tri[i]->tefill(el2.superCluster()->eta(),pileupweight);

            //filling in pass
            for(int  k=0; k<(int)phltname.size() ; k++){
                 if(el2.hasUserInt(phltname[k])) {
                     if ( fabs( el2.superCluster()->eta() ) < hlteta )
                         tri[i]->ppfill(el2.pt(),pileupweight);
                     if(el2.pt()>hltpt)
                         tri[i]->pefill(el2.superCluster()->eta(),pileupweight);
                     break;
                 }
            }

           //store zmass 
           if(fabs( el2.superCluster()->eta() ) < hlteta && el2.pt()>hltpt){
               if(i==0)
                   zmass1->Fill(mass,pileupweight);
               else
                   zmass2->Fill(mass,pileupweight);
           }
        }
    }

}


vector<double> TriggerAnalyzerMCElectron::readweight(const string& filename){
    vector<double> w;
    ifstream fin;
    fin.open(filename,ifstream::in);
    string line;
    while(getline(fin,line)){
        w.push_back(stod(line));
    }
    return w;
}

// ------------ method called once each job just before starting event loop  ------------
void
TriggerAnalyzerMCElectron::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void
TriggerAnalyzerMCElectron::endJob()
{

}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
TriggerAnalyzerMCElectron::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
    //The following says we do not know what parameters are allowed so do no validation
    // Please change this to state exactly what you do use, even if it is no parameters
    edm::ParameterSetDescription desc;
    desc.setUnknown();
    descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(TriggerAnalyzerMCElectron);
