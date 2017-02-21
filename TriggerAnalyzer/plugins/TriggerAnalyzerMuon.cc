#include <memory>
#include <math.h>
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/FWLite/interface/Handle.h"
#include "TGraphAsymmErrors.h"
#include "TH1.h"
#include "TCanvas.h"
#include "TFile.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
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

class TriggerAnalyzerMuon : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
public:
    explicit TriggerAnalyzerMuon(const edm::ParameterSet&);
    ~TriggerAnalyzerMuon();

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

TriggerAnalyzerMuon::TriggerAnalyzerMuon(const edm::ParameterSet& iConfig):
    etabin(iConfig.getParameter<vector<double> >("etabin")),
    vtri(iConfig.getParameter<vector<edm::ParameterSet>>("tagtrigger")),
    vpri(iConfig.getParameter<vector<edm::ParameterSet>>("protrigger")),
    probe ( consumes<vector<pat::Muon>>(iConfig.getParameter<edm::InputTag> ("probe") ) ),  //module
    tag ( consumes<vector<pat::Muon>>  (iConfig.getParameter<edm::InputTag>("tag") ) )
{

    for(int i=0; i<(int)vtri.size(); i++) {      //each pset has a corresponding Trig class
        vector<double> ptbin=vtri[i].getParameter<vector<double>>("ptbin");
        string triname = vtri[i].getParameter<string>("name");
        tri.push_back(new Trig(ptbin,etabin,triname));
    }

}


TriggerAnalyzerMuon::~TriggerAnalyzerMuon()
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
TriggerAnalyzerMuon::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
    using namespace edm;
    Handle<vector<pat::Muon> > probehandle;
    Handle<vector<pat::Muon> > taghandle;
    iEvent.getByToken( probe, probehandle );
    iEvent.getByToken( tag, taghandle );

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
            pat::Muon mu1=(*taghandle)[j];
            pat::Muon mu2=(*probehandle)[j];

            /////////////////////////zmass window///////////////////////
            TLorentzVector muon1(mu1.px(),mu1.py(),mu1.pz(),mu1.energy());
            TLorentzVector muon2(mu2.px(),mu2.py(),mu2.pz(),mu2.energy());
            double mass = (muon1+muon2).M();
            if(mass<60 || mass>120)
                break;
            
            //pass tag cut
            bool passtag = false;
            for(int  k=0; k<(int)thltname.size() ; k++){
                if( mu1.hasUserInt(thltname[k]) && mu1.pt()>ptcut && fabs(mu1.eta()) < etacut ){
                    passtag = true;
                    break;
                }
            }
            
            //pass probe cut
            if(!passtag)
                continue;

            //filling in total
            
            if ( fabs( mu2.eta() ) < hlteta )
                tri[i]->tpfill(mu2.pt());
            if(mu2.pt()>hltpt)
                tri[i]->tefill(mu2.eta());

            //filling in pass
            for(int  k=0; k<(int)phltname.size() ; k++){
                 if(mu2.hasUserInt(phltname[k])) {
                     if ( fabs( mu2.eta() ) < hlteta )
                         tri[i]->ppfill(mu2.pt());
                     if(mu2.pt()>hltpt)
                         tri[i]->pefill(mu2.eta());
                     break;
                
                 }
            }
        }
    }
}


// ------------ method called once each job just before starting event loop  ------------
void
TriggerAnalyzerMuon::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void
TriggerAnalyzerMuon::endJob()
{

}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
TriggerAnalyzerMuon::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
    //The following says we do not know what parameters are allowed so do no validation
    // Please change this to state exactly what you do use, even if it is no parameters
    edm::ParameterSetDescription desc;
    desc.setUnknown();
    descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(TriggerAnalyzerMuon);
