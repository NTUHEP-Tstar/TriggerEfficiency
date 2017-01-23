// Package:    TriggerMCTool/TriggerMCMuonEff
// Class:      TriggerMCMuonEff
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
#include <string>
#include <vector>
#include <fstream>
#include "TH2D.h"
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
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
using namespace std;

class TriggerMCMuonEff : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
   public:
      explicit TriggerMCMuonEff(const edm::ParameterSet&);
      ~TriggerMCMuonEff();

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

      const edm::EDGetToken pusrc;

      edm::Service<TFileService> fs;
      TEfficiency* peff;

      vector<double> weights;
 
      vector<double> readweight(const string& );
};
TriggerMCMuonEff::TriggerMCMuonEff(const edm::ParameterSet& iConfig):
    ptbin (iConfig.getParameter<vector<double> >("ptbin")),
    etabin(iConfig.getParameter<vector<double> >("etabin")),
    tagtrigger(iConfig.getParameter<vector<edm::ParameterSet> >("tagtrigger")),
    protrigger(iConfig.getParameter<vector<edm::ParameterSet> >("protrigger")),
    probe ( consumes<vector<pat::Muon>>(iConfig.getParameter<edm::InputTag> ("probe") ) ),  //module
    tag ( consumes<vector<pat::Muon>>  (iConfig.getParameter<edm::InputTag>("tag") ) ),
    pusrc ( consumes<vector<PileupSummaryInfo>>(iConfig.getParameter<edm::InputTag>("pusrc") ) ),
    weights( readweight(iConfig.getParameter<edm::FileInPath>("filename").fullPath()) )
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
        total.push_back(fs->make<TH2D>( tname.c_str() , "Total pt to eta" , esize-1 , a , psize-1 , b )) ;
        pass .push_back(fs->make<TH2D>( pname.c_str() , "Pass pt to eta"  , esize-1 , a , psize-1 , b )) ;
    }
}


TriggerMCMuonEff::~TriggerMCMuonEff()
{
}

    void
TriggerMCMuonEff::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup){
    using namespace edm;
    Handle<vector<pat::Muon> > probehandle;
    Handle<vector<pat::Muon> > taghandle;
    Handle<vector<PileupSummaryInfo> > puhandle;
    iEvent.getByToken( probe, probehandle );
    iEvent.getByToken( tag, taghandle );
    iEvent.getByToken( pusrc, puhandle );


    double pileupweight = 1.0;
    const unsigned pu = puhandle->at( 0 ).getPU_NumInteractions();
    if( pu < weights.size() ){
        pileupweight = weights.at( pu );
    }




    vector<int> totalid;

    for(int k=0;k< (int)taghandle->size();k++)
    {
        pat::Muon muon=(*taghandle)[k];
        for(unsigned i=0;i<tagtrigger.size();i++){
            vector<string> hltname = tagtrigger[i].getParameter<vector<string>>("HLT");
            double ptcut = tagtrigger[i].getParameter<double>("ptcut");
            double etacut= tagtrigger[i].getParameter<double>("etacut");

            for(int  j=0; j<(int)hltname.size() ;j++){
                if( muon.hasUserInt(hltname[j]) && muon.pt()>ptcut && fabs(muon.eta()) < etacut ){
                    totalid.push_back(k);
                    break;
                }
            }
        }
    }
    
    for(int k=0;k< (int)totalid.size();k++)
    {
        pat::Muon muon=(*probehandle)[ totalid[k]  ];
        for(unsigned i=0;i<protrigger.size();i++){
            vector<string> hltname = protrigger[i].getParameter<vector<string>>("HLT");
            total[i]->Fill(muon.eta(),muon.pt(),pileupweight);
            for(unsigned j=0; j<hltname.size() ;j++){
                if(muon.hasUserInt(hltname[j])){
                    pass[i]->Fill(muon.eta(),muon.pt(),pileupweight);
                    break;
                }
            }
        }
    }
    
}

vector<double> TriggerMCMuonEff::readweight(const string& filename){
    vector<double> w;
    ifstream fin;
    fin.open(filename,ifstream::in);
    string line;
    while(getline(fin,line)){
        w.push_back(stod(line));
    }
    return w;
}

void 
TriggerMCMuonEff::beginJob()
{
}

void 
TriggerMCMuonEff::endJob() 
{
}

void
TriggerMCMuonEff::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(TriggerMCMuonEff);
