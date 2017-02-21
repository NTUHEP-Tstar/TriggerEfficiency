// Package:    TriggerMCTool/TriggerMuonEff
// Class:      TriggerMuonEff
/*
 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Pu-Sheng Chen
//         Created:  Sun, 04 Dec 2016 05:59:56 GMT
#include <memory>
#include <math.h>
#include <iostream>
#include "TH2D.h"
#include "TLorentzVector.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/FWLite/interface/Handle.h"
#include "TEfficiency.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
using namespace std;

class TriggerMuonEff : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
   public:
      explicit TriggerMuonEff(const edm::ParameterSet&);
      ~TriggerMuonEff();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;

      // ----------member data ---------------------------
      vector<double> ptbin;
      vector<double> etabin;

      vector<edm::ParameterSet> tagtrigger;
      vector<edm::ParameterSet> protrigger;
      vector<TH2D*> total;
      vector<TH2D*> pass;
                
      const edm::EDGetToken probe;
      const edm::EDGetToken tag;

      edm::Service<TFileService> fs;
      TEfficiency* peff;
 
};
TriggerMuonEff::TriggerMuonEff(const edm::ParameterSet& iConfig):
    ptbin (iConfig.getParameter<vector<double> >("ptbin")),
    etabin(iConfig.getParameter<vector<double> >("etabin")),
    tagtrigger(iConfig.getParameter<vector<edm::ParameterSet> >("tagtrigger")),
    protrigger(iConfig.getParameter<vector<edm::ParameterSet> >("protrigger")),
    probe ( consumes<vector<pat::Muon>>(iConfig.getParameter<edm::InputTag> ("probe") ) ),  //module
    tag ( consumes<vector<pat::Muon>>  (iConfig.getParameter<edm::InputTag>("tag") ) )
{
    peff=0;
    
    
    int esize = etabin.size();
    int psize = ptbin.size();
    double _etabin[ esize ];
    double _ptbin[ psize ];
    for(unsigned i =0;i<etabin.size();i++)
        _etabin[i]=etabin[i];
    for(unsigned i =0;i<ptbin.size();i++)
        _ptbin[i]=ptbin[i];



    double* a=_etabin; 
    double* b=_ptbin;

    for(unsigned i=0;i<protrigger.size();i++){
        string tname = tagtrigger[i].getParameter<string>("name");
        string pname = protrigger[i].getParameter<string>("name");
        total.push_back( new TH2D( tname.c_str() , "Total pt to eta" , esize-1 , a , psize-1 , b )) ;
        pass .push_back( new TH2D( pname.c_str() , "Pass pt to eta"  , esize-1 , a , psize-1 , b )) ;
    }
}


TriggerMuonEff::~TriggerMuonEff()
{
}

    void
TriggerMuonEff::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup){
    using namespace edm;
    Handle<vector<pat::Muon> > probehandle;
    Handle<vector<pat::Muon> > taghandle;
    iEvent.getByToken( probe, probehandle );
    iEvent.getByToken( tag, taghandle );


    for(unsigned i=0; i<tagtrigger.size(); i++){
        
        //set tag cut
        vector<string> thltname = tagtrigger[i].getParameter<vector<string>>("HLT");
        double ptcut = tagtrigger[i].getParameter<double>("ptcut");
        double etacut= tagtrigger[i].getParameter<double>("etacut");
        //set probe cut
        vector<string> phltname = protrigger[i].getParameter<vector<string>>("HLT");

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

            total[i]->Fill(mu2.eta(),mu2.pt());

            //filling in pass
            for(int  k=0; k<(int)phltname.size() ; k++){
                 if(mu2.hasUserInt(phltname[k])) {
                     pass[i]->Fill(mu2.eta(),mu2.pt());
                     break;
                 }
            
            }
        }
    }

}


void 
TriggerMuonEff::beginJob()
{
}

void 
TriggerMuonEff::endJob() 
{
    for(unsigned i =0; i<tagtrigger.size(); i++){
         if(TEfficiency::CheckConsistency(*pass[i],*total[i] )){
             peff = fs->make<TEfficiency>(*(pass[i]),*(total[i]));
             peff->SetTitle("Eta to pt;eta;pt");
         }
    }
}

void
TriggerMuonEff::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(TriggerMuonEff);
