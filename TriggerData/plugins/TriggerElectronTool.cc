#include <memory>
#include <math.h>
#include <stdlib.h>
#include "DataFormats/Common/interface/Ptr.h"
#include "RecoEgamma/EgammaTools/interface/EffectiveAreas.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/StreamID.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "TLorentzVector.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/FWLite/interface/Handle.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "ManagerUtils/PhysUtils/interface/MCHelper.hpp"
#include "DataFormats/Math/interface/deltaR.h"
using namespace std;


class TriggerElectronTool : public edm::stream::EDFilter<>
{
public:
    explicit TriggerElectronTool(const edm::ParameterSet&);
    ~TriggerElectronTool();

    static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

private:
    virtual void beginStream(edm::StreamID) override;
    virtual bool filter(edm::Event&, const edm::EventSetup&) override;
    virtual void endStream() override;

    bool zParent(pat::ElectronCollection& ) const;
    bool passKin(const pat::Electron& , bool  ) const;
    double delR(const pat::Electron& ,const edm::TriggerNames&, const string );
    void passTrigger(pat::Electron& ,const edm::TriggerNames &) ;
    bool passId(const edm::Ptr<pat::Electron>& ,const string ) const;
    bool passImpact(const pat::Electron& el)  const;

    const edm::EDGetToken muonsrc;
    edm::Handle<vector<pat::Muon> > muonhandle;
    const edm::EDGetToken elesrc;
    edm::Handle<vector<pat::Electron> > elehandle;
    const edm::EDGetTokenT<vector<reco::Vertex> > vtxsrc;
    edm::Handle<reco::VertexCollection>  vtxhandle;

    const edm::EDGetToken hltInputTag;
    const edm::EDGetToken hltObjectsInputTag;
    edm::Handle < edm::TriggerResults > triggerResults;
    edm::Handle<pat::TriggerObjectStandAloneCollection> triggerObjects;

    string tagid;
    string proid;
    double tEtaMax;
    double pEtaMax;
    double tPtMin;
    double pPtMin;
    double zmin;
    double zmax;
    vector<edm::ParameterSet> trigger;
    bool useMC;
    bool pImpact;
    bool tImpact;

    const edm::EDGetTokenT<edm::ValueMap<bool>> looseMapToken;
    edm::Handle<edm::ValueMap<bool>> looseMapHandle;
    const edm::EDGetTokenT<edm::ValueMap<bool>> mediumMapToken;
    edm::Handle<edm::ValueMap<bool>> mediumMapHandle;
    const edm::EDGetTokenT<edm::ValueMap<bool>> tightMapToken;
    edm::Handle<edm::ValueMap<bool>> tightMapHandle;
    const edm::EDGetTokenT<edm::ValueMap<bool>> heepMapToken;
    edm::Handle<edm::ValueMap<bool>> heepMapHandle;

    const edm::EDGetTokenT<edm::ValueMap<bool>> safeCutToken;
    edm::Handle<edm::ValueMap<bool>> safecutHandle;

};

TriggerElectronTool::TriggerElectronTool(const edm::ParameterSet& iConfig):
    muonsrc ( consumes<vector<pat::Muon> >(iConfig.getParameter<edm::InputTag>("muonsrc") ) ),
    elesrc ( consumes<vector<pat::Electron> >(iConfig.getParameter<edm::InputTag>("elesrc") ) ),
    vtxsrc  ( consumes<vector<reco::Vertex> > (iConfig.getParameter<edm::InputTag >("vtxsrc") ) ),
    hltInputTag (consumes<edm::TriggerResults>(iConfig.getParameter<edm::InputTag>("HLTInputTag"))),
    hltObjectsInputTag (consumes<vector<pat::TriggerObjectStandAlone> >(iConfig.getParameter<edm::InputTag>("HLTObjectsInputTag"))),
    tagid (iConfig.getParameter<string>("TagPassID")),
    proid (iConfig.getParameter<string>("ProbePassID")),
    tEtaMax (iConfig.getParameter<double>("tagEtaMax")),
    pEtaMax (iConfig.getParameter<double>("probeEtaMax")),
    tPtMin (iConfig.getParameter <double>("tagPtMin")),
    pPtMin (iConfig.getParameter <double>("probePtMin")),
    zmin (iConfig.getParameter   <double>("Zmassmin")),
    zmax (iConfig.getParameter   <double>("Zmassmax")),
    trigger (iConfig.getParameter<vector<edm::ParameterSet>>("triggerCache")),
    useMC (iConfig.getParameter<bool>("useMC")),
    pImpact(iConfig.getParameter<bool>("probeImpact") ),
    tImpact(iConfig.getParameter<bool>("tagImpact") ),
    looseMapToken ( consumes<edm::ValueMap<bool>> ( iConfig.getParameter<edm::InputTag>("looseMap")) ) ,
    mediumMapToken ( consumes<edm::ValueMap<bool>> (iConfig.getParameter<edm::InputTag>("mediumMap")) ),
    tightMapToken ( consumes<edm::ValueMap<bool>> (iConfig.getParameter<edm::InputTag> ("tightMap")) ),
    heepMapToken ( consumes<edm::ValueMap<bool>> (iConfig.getParameter<edm::InputTag>("heepMap")) ),
    safeCutToken (consumes<edm::ValueMap<bool>> (iConfig.getParameter<edm::InputTag>("safeCut")) )
{
    produces<vector<pat::Electron>>("Tag");
    produces<vector<pat::Electron>>("Probe");
}


TriggerElectronTool::~TriggerElectronTool()
{
}


double TriggerElectronTool::delR(const pat::Electron& el ,const edm::TriggerNames &names, const string label)
{

    vector<double> dR;

    for (pat::TriggerObjectStandAlone obj : *triggerObjects)
    {
        obj.unpackPathNames(names);
        if(obj.hasFilterLabel(label))
            dR.push_back( deltaR( el.superCluster()->eta(), el.phi(), obj.eta(), obj.phi() ) );
    }

    if(dR.empty())
        return 999;
    else
    {
        std::sort(dR.begin(),dR.end());
        return dR.front();
    }


}


void TriggerElectronTool::passTrigger(pat::Electron& el,const edm::TriggerNames &names)
{

    for(int i=0; i< (int)trigger.size(); i++)
    {
        string name  = trigger[i].getParameter<string>("HLTName");
        string label = trigger[i].getParameter<string>("FilterName");

        if( delR(el,names,label)<0.1)
        {
            el.addUserInt(name,1);
        }
    }

}
bool TriggerElectronTool::zParent(pat::ElectronCollection& ele) const
{
    TLorentzVector lep1( ele[0].px(), ele[0].py(), ele[0].pz(), ele[0].energy() );
    TLorentzVector lep2( ele[1].px(), ele[1].py(), ele[1].pz(), ele[1].energy() );
    double mass = ( lep1+lep2 ).M();
    return (mass > zmin && mass < zmax);
}
bool TriggerElectronTool::passKin(const pat::Electron& el, bool isTag ) const
{
    double eta=el.superCluster()->eta();
    double pt=el.pt();
    bool MuKin(false);
    if(isTag)
        MuKin = ( ( fabs(eta)<tEtaMax )   &&   ( fabs(eta)<=1.4442 || fabs(eta)>=1.5660 )   &&   pt>tPtMin);
    else
        MuKin = ( ( fabs(eta)<tEtaMax )   &&   ( fabs(eta)<=1.4442 || fabs(eta)>=1.5660 )   &&   pt>pPtMin);


    return MuKin;

}

bool TriggerElectronTool::passId(const edm::Ptr<pat::Electron>& elptr,const string level) const
{
    if(level == "loose") {
        if( (*looseMapHandle)[elptr] )
            return true;
    }
    if(level == "medium") {
        if( (*mediumMapHandle)[elptr]  )
            return true;
    }
    if(level == "tight") {
        if( (*tightMapHandle)[elptr]  )
            return true;
    }
    if(level == "heep") {
        if( (*heepMapHandle)[elptr]  )
            return true;
    }
    return false;
}

//referenced from
//https://twiki.cern.ch/twiki/bin/view/CMS/CutBasedElectronIdentificationRun2#Recipe80X

bool TriggerElectronTool::passImpact(const pat::Electron& el) const
{
    
    if( fabs( el.eta() ) < 1.4442  ) { //barrel
        if( fabs( el.gsfTrack()->dxy( vtxhandle->front().position() ) ) < 0.05 ) {
            return true;
        }
        if( fabs( el.gsfTrack()->dz( vtxhandle->front().position() ) ) < 0.10 ) {
            return true;
        }
    } 
    else {
        if( fabs( el.gsfTrack()->dxy( vtxhandle->front().position() ) ) < 0.10 ) {
            return true;
        }
        if( fabs( el.gsfTrack()->dz( vtxhandle->front().position() ) ) < 0.20 ) {
            return true;
        }
    }
    return false;
}

bool TriggerElectronTool::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{

    iEvent.getByToken(muonsrc, muonhandle);
    iEvent.getByToken(elesrc, elehandle);
    iEvent.getByToken(vtxsrc, vtxhandle);
    iEvent.getByToken(hltInputTag, triggerResults);
    iEvent.getByToken(hltObjectsInputTag, triggerObjects);

    iEvent.getByToken(safeCutToken , safecutHandle);
    iEvent.getByToken(looseMapToken, looseMapHandle);
    iEvent.getByToken(mediumMapToken,mediumMapHandle);
    iEvent.getByToken(tightMapToken, tightMapHandle);
    iEvent.getByToken(heepMapToken, heepMapHandle);

    const edm::TriggerNames &names = iEvent.triggerNames(*triggerResults);

    //initialize the handle (pointer to a collection of something)
    pat::MuonCollection muons;
    if(muonhandle.isValid())
    {
        muons = *muonhandle;
    }

    pat::ElectronCollection electrons;
    if(elehandle.isValid())
    {
        electrons = *elehandle;
    }

    //to confirm there's no muons
    for(int i=0; i< (int)muons.size(); i++)
    {
        if(muons[i].pt()>8)
            return false;
    }

    //pre-selection based on pt
    vector <int> eId;
    pat::ElectronCollection ele;

    for(int i=0; i< (int)electrons.size(); i++)
    {
        const edm::Ptr<pat::Electron> elptr(elehandle,i);
        if(electrons[i].pt()>8 && ((*safecutHandle)[elptr]) ) {  //check if electron pass the safe cut
            ele.push_back(electrons[i]);
            eId.push_back(i);             // to store the original position in the handle
        }
    }
    //to confirm that ele only has two electrons
    if(ele.size()!=2)
        return false;

    int first  = rand()%2;
    int second = (first+1)%2;
    //build the Ptr of tag&probe
    const edm::Ptr<pat::Electron> tElptr(elehandle,eId[first]);
    const edm::Ptr<pat::Electron> pElptr(elehandle,eId[second]);


    bool tid = passId( tElptr,tagid);
    bool pid = passId( pElptr,proid);

    bool passtagkin   = passKin(ele[first] , true);
    bool passprobekin = passKin(ele[second], false);

    //tag preselection
    if(!(tid && passtagkin)) return false;
    //probe preselection
    if(!(pid && passprobekin)) return false;
    //to confirm if it can pass the impact parameter cut
    if(tImpact) {
        if(!passImpact(ele[first]))
            return false;
    }
    if(pImpact) {
        if(!passImpact(ele[second]))
            return false;
    }
    //to confirm MC truth and their mother is the same z
    if(useMC)
    {
        if(ele[first].genLepton()== NULL || ele[second].genLepton()== NULL)
            return false;

        if (!zParent(ele))
            return false;

        if(!( GetDirectMother(ele[first].genLepton(),23) == GetDirectMother(ele[second].genLepton(),23) ))
            return false;
    }

    //to mark the electron that pass the criteria
    passTrigger(ele[first],names);
    passTrigger(ele[second],names);

    auto_ptr< vector< pat::Electron> > tagEleptr ( new vector< pat::Electron> );
    auto_ptr< vector<pat::Electron> > probeEleptr ( new vector< pat::Electron> );
    tagEleptr  ->push_back(ele[first]);
    probeEleptr->push_back(ele[second]);
    
    //output.root label name
    iEvent.put(tagEleptr, "Tag");
    iEvent.put(probeEleptr , "Probe");
    return true;

}
void TriggerElectronTool::beginStream(edm::StreamID)
{
}



void
TriggerElectronTool::endStream()
{
}

void
TriggerElectronTool::fillDescriptions(edm::ConfigurationDescriptions& descriptions)
{
    edm::ParameterSetDescription desc;
    desc.setUnknown();
    descriptions.addDefault(desc);
}

DEFINE_FWK_MODULE(TriggerElectronTool);
