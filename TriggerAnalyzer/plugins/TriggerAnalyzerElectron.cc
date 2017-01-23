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
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
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
    void pfill(double d) {
        pass->Fill(d);
    }
    void tfill(double d) {
        total->Fill(d);
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
    void ppfill(double d) {
        pt->pfill(d);
    }
    void tpfill(double d) {
        pt->tfill(d);
    }
    void pefill(double d) {
        eta->pfill(d);
    }
    void tefill(double d) {
        eta->tfill(d);
    }
private:
    double* pb;
    double* eb;
    hist* pt;
    hist* eta;
    string str1;
    string str2;
};

class TriggerAnalyzerElectron : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
public:
    explicit TriggerAnalyzerElectron(const edm::ParameterSet&);
    ~TriggerAnalyzerElectron();

    static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


private:
    virtual void beginJob() override;
    virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
    virtual void endJob() override;

    // ----------member data ---------------------------

    vector<Trig*> tri;
    vector<double> etabin;
    vector<edm::ParameterSet> vtri;
    vector<edm::ParameterSet> vpri;
    const edm::EDGetToken probe;
    const edm::EDGetToken tag;


};

TriggerAnalyzerElectron::TriggerAnalyzerElectron(const edm::ParameterSet& iConfig):
    etabin(iConfig.getParameter<vector<double> >("etabin")),
    vtri(iConfig.getParameter<vector<edm::ParameterSet>>("tagtrigger")),
    vpri(iConfig.getParameter<vector<edm::ParameterSet>>("protrigger")),
    probe ( consumes<vector<pat::Electron>>(iConfig.getParameter<edm::InputTag> ("probe") ) ),  //module
    tag ( consumes<vector<pat::Electron>>  (iConfig.getParameter<edm::InputTag>("tag") ) )
{
    //  double ebin[14]= {-2.4,-2.1,-1.6,-1.2,-0.9,-0.3,-0.2,0.2,0.3,0.9,1.2,1.6,2.1,2.4};


    for(int i=0; i<(int)vtri.size(); i++) {      //each pset has a corresponding Trig class
        vector<double> ptbin=vtri[i].getParameter<vector<double>>("ptbin");
        string triname = vtri[i].getParameter<string>("name");
        tri.push_back(new Trig(ptbin,etabin,triname));
    }

}


TriggerAnalyzerElectron::~TriggerAnalyzerElectron()
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
TriggerAnalyzerElectron::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
    using namespace edm;
    Handle<vector<pat::Electron> > probehandle;
    Handle<vector<pat::Electron> > taghandle;
    iEvent.getByToken( probe, probehandle );
    iEvent.getByToken( tag, taghandle );

    vector<int> totalid;
    

    for(int k=0; k< (int)taghandle->size(); k++)
    {
        pat::Electron el=(*taghandle)[k];
        for(unsigned i=0; i<vtri.size(); i++) {
            vector<string> hltname = vtri[i].getParameter<vector<string>>("HLT");
            double ptcut = vtri[i].getParameter<double>("ptcut");
            double etacut= vtri[i].getParameter<double>("etacut");

            for(int  j=0; j<(int)hltname.size() ; j++) {
                if( el.hasUserInt(hltname[j]) && el.pt()>ptcut && fabs(el.superCluster()->eta()) < etacut ) {
                    totalid.push_back(k);
                    break;
                }
            }
        }
    }

    for(int k=0; k< (int)totalid.size(); k++)
    {
        pat::Electron el=(*probehandle)[ totalid[k]  ];   //get the corresponding  probed el with tagged el
        for(unsigned i=0; i<vtri.size(); i++) {                    //how many pset i have
            vector<string> hltname = vpri[i].getParameter<vector<string>>("HLT");         //each pset has its hlts and its pt cut
            double hltpt   = vpri[i].getParameter<double>("probePtCut");
            double hlteta  = vpri[i].getParameter<double>("probeEtaCut");
            
            if ( fabs( el.superCluster()->eta() ) < hlteta )
                tri[i]->tpfill(el.pt());
            if(el.pt()>hltpt)
                tri[i]->tefill(el.superCluster()->eta());

            for(unsigned j=0; j<hltname.size() ; j++) {
                if(el.hasUserInt(hltname[j])) {
                    if ( fabs( el.superCluster()->eta() ) < hlteta )
                        tri[i]->ppfill(el.pt());
                    if(el.pt()>hltpt)
                        tri[i]->pefill(el.superCluster()->eta());
                    break;
                }
            }
        }
    }
}

// ------------ method called once each job just before starting event loop  ------------
void
TriggerAnalyzerElectron::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void
TriggerAnalyzerElectron::endJob()
{

}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
TriggerAnalyzerElectron::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
    //The following says we do not know what parameters are allowed so do no validation
    // Please change this to state exactly what you do use, even if it is no parameters
    edm::ParameterSetDescription desc;
    desc.setUnknown();
    descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(TriggerAnalyzerElectron);
