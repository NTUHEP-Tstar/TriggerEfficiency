// Package:    TriggerMCTool/TriggerElectronEff
// Class:      TriggerElectronEff
/*
 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Pu-Sheng Chen
//         Created:  Sun, 04 Dec 2016 05:59:56 GMT
#include <memory>
#include <iostream>
#include "TH2D.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/FWLite/interface/Handle.h"
#include "TEfficiency.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "TLorentzVector.h"
using namespace std;

class TriggerElectronEff : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
   public:
      explicit TriggerElectronEff(const edm::ParameterSet&);
      ~TriggerElectronEff();

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
TriggerElectronEff::TriggerElectronEff(const edm::ParameterSet& iConfig):
    ptbin (iConfig.getParameter<vector<double> >("ptbin")),
    etabin(iConfig.getParameter<vector<double> >("etabin")),
    tagtrigger(iConfig.getParameter<vector<edm::ParameterSet> >("tagtrigger")),
    protrigger(iConfig.getParameter<vector<edm::ParameterSet> >("protrigger")),
    probe ( consumes<vector<pat::Electron>>(iConfig.getParameter<edm::InputTag> ("probe") ) ),  //module
    tag ( consumes<vector<pat::Electron>>  (iConfig.getParameter<edm::InputTag>("tag") ) )
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
        total.push_back(new TH2D( tname.c_str() , "Total pt to eta" , esize-1 , a , psize-1 , b )) ;
        pass .push_back(new TH2D( pname.c_str() , "Pass pt to eta"  , esize-1 , a , psize-1 , b )) ;
    }
}


TriggerElectronEff::~TriggerElectronEff()
{
}

    void
TriggerElectronEff::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup){
    using namespace edm;
    Handle<vector<pat::Electron> > probehandle;
    Handle<vector<pat::Electron> > taghandle;
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

            total[i]->Fill(el2.superCluster()->eta(),el2.pt());

            //filling in pass
            for(int  k=0; k<(int)phltname.size() ; k++){
                 if(el2.hasUserInt(phltname[k])) {
                     pass[i]->Fill(el2.superCluster()->eta(),el2.pt());
                     break;
                 }
            
            }
        }
    }

}


void 
TriggerElectronEff::beginJob()
{
}

void 
TriggerElectronEff::endJob() 
{
    for(unsigned i =0; i<tagtrigger.size(); i++){
         if(TEfficiency::CheckConsistency(*pass[i],*total[i] )){
             peff = fs->make<TEfficiency>(*(pass[i]),*(total[i]));
             peff->SetTitle("Eta to pt;eta;pt");
         }
    }
}

void
TriggerElectronEff::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(TriggerElectronEff);
