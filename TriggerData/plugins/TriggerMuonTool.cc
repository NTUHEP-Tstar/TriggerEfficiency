#include <memory>
#include <math.h>
#include <stdlib.h>
#include "TLorentzVector.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/StreamID.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/FWLite/interface/Handle.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "ManagerUtils/PhysUtils/interface/MCHelper.hpp"
#include "DataFormats/Math/interface/deltaR.h"
using namespace std;


class TriggerMuonTool : public edm::stream::EDFilter<>
{
public:
    explicit TriggerMuonTool(const edm::ParameterSet&);
    ~TriggerMuonTool();

    static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

private:
    virtual void beginStream(edm::StreamID) override;
    virtual bool filter(edm::Event&, const edm::EventSetup&) override;
    virtual void endStream() override;



    bool zParent(const pat::MuonCollection& ) const;
    bool passTKIso(const pat::Muon& ,const double ) const;
    bool passPFIso(const pat::Muon& ,const double ) const;
    bool passId(const pat::Muon& ,const string ) ;
    bool passKin(const pat::Muon& ,const bool  ) const;
    double delR(const pat::Muon& ,const edm::TriggerNames&, const string);
    void passTrigger(pat::Muon& ,const edm::TriggerNames &);
    
    const edm::EDGetToken muonsrc;
    edm::Handle<vector<pat::Muon> > muonhandle;
    const edm::EDGetToken elesrc;
    edm::Handle<vector<pat::Electron> > elehandle;

    const edm::EDGetTokenT<vector<reco::Vertex> > vtxsrc;
    edm::Handle<reco::VertexCollection>  vtxhandle;

    reco::VertexCollection vtx;

    const edm::EDGetToken hltInputTag;
    const edm::EDGetToken hltObjectsInputTag;
    edm::Handle <edm::TriggerResults> triggerResults;
    edm::Handle<pat::TriggerObjectStandAloneCollection> triggerObjects;

    //input tag
 //   string muoninput;

    string tagid;
    string proid;
    double tEtaMax;
    double tPtMin;
    double pEtaMax;
    double pPtMin;
    double tPFIso;
    double pPFIso;
    double tTKIso;
    double pTKIso;
    double zmin;
    double zmax;
    vector<edm::ParameterSet> trigger;
    bool useMC;
};

TriggerMuonTool::TriggerMuonTool(const edm::ParameterSet& iConfig):
    muonsrc ( consumes<vector<pat::Muon> >(iConfig.getParameter<edm::InputTag>("muonsrc") ) ),
    elesrc ( consumes<vector<pat::Electron> >(iConfig.getParameter<edm::InputTag>("elesrc") ) ),
    vtxsrc  ( consumes<vector<reco::Vertex> > (iConfig.getParameter<edm::InputTag >("vtxsrc") ) ),
    hltInputTag(consumes<edm::TriggerResults>(iConfig.getParameter<edm::InputTag>("HLTInputTag"))),
    hltObjectsInputTag(consumes<vector<pat::TriggerObjectStandAlone> >(iConfig.getParameter<edm::InputTag>("HLTObjectsInputTag"))),
  //  muoninput(iConfig.getParameter<string>("muoninput")),
    tagid(iConfig.getParameter<string>("TagPassID")),
    proid(iConfig.getParameter<string>("ProbePassID")),
    tEtaMax(iConfig.getParameter<double>("tagEtaMax")),
    tPtMin(iConfig.getParameter<double>("tagPtMin")),
    pEtaMax(iConfig.getParameter<double>("probeEtaMax")),
    pPtMin(iConfig.getParameter<double>("probePtMin")),
    tPFIso(iConfig.getParameter<double>("TagPassPFIso")),
    pPFIso(iConfig.getParameter<double>("ProbePassPFIso")),
    tTKIso(iConfig.getParameter<double>("TagPassTKIso")),
    pTKIso(iConfig.getParameter<double>("ProbePassTKIso")),
    zmin(iConfig.getParameter<double>("Zmassmin")),
    zmax(iConfig.getParameter<double>("Zmassmax")),
    trigger(iConfig.getParameter<vector<edm::ParameterSet>>("triggerCache")),
    useMC(iConfig.getParameter<bool>("useMC"))
{
    //produces<vector<pat::Muon>>(muonsrc);
    produces< vector<pat::Muon> >("Tag");
    produces< vector<pat::Muon> >("Probe");
}


TriggerMuonTool::~TriggerMuonTool()
{
}


double TriggerMuonTool::delR(const pat::Muon& mu,const edm::TriggerNames& names ,const string label) 
{
    vector<double> dR;
    for (pat::TriggerObjectStandAlone obj : *triggerObjects)
    {
        obj.unpackPathNames(names);
        if(obj.hasFilterLabel(label))
            dR.push_back( deltaR( mu.eta(), mu.phi(), obj.eta(), obj.phi() ) );
    }

    if(dR.empty())
        return 999;
    else
    {
        std::sort(dR.begin(),dR.end());
        return dR.front();
    }

}


void TriggerMuonTool::passTrigger(pat::Muon& mu,const edm::TriggerNames &names)
{
    for(int i=0; i< (int)(trigger.size()); i++){
        string name =trigger[i].getParameter<string>("HLTName");
        string label=trigger[i].getParameter<string>("FilterName");
    
        if ( delR(mu,names,label)<0.1 )
        {
            mu.addUserInt(name,1);
        }
    }
}

bool TriggerMuonTool::zParent(const pat::MuonCollection& muons) const
{
    TLorentzVector lep1( muons[0].px(), muons[0].py(), muons[0].pz(), muons[0].energy() );
    TLorentzVector lep2( muons[1].px(),muons[1].py(),muons[1].pz(),muons[1].energy() );
    double mass = ( lep1+lep2 ).M();
    return (mass > zmin && mass < zmax);
}

bool TriggerMuonTool::passTKIso(const pat::Muon& mu,const double cut) const
{
    if(mu.isolationR03().sumPt/mu.pt() < cut )
        return true;
    else
        return false;
}

bool TriggerMuonTool::passPFIso(const pat::Muon& mu,const double cut) const
{
    double  chIso   = mu.pfIsolationR04().sumChargedHadronPt;
    double  nhIso   = mu.pfIsolationR04().sumNeutralHadronEt;
    double  gIso    = mu.pfIsolationR04().sumPhotonEt;
    double  puchIso = mu.pfIsolationR04().sumPUPt;
    double  relIso  = (chIso + TMath::Max(0.,nhIso+gIso-0.5*puchIso)) / mu.pt();
    if(relIso<cut)  //0.15
        return true;
    else
        return false;
}

bool TriggerMuonTool::passId(const pat::Muon& mu,const string level) 
{
    if(level=="loose")
    {
        return mu.isLooseMuon();
    }
    if(level=="tight")
    {
        return mu.isTightMuon(vtx.front());
    }
    if(level=="HighPT")
    {
        return mu.isHighPtMuon(vtx.front());
    }
    return false;
}

bool TriggerMuonTool::passKin(const pat::Muon& mu,const bool isTag ) const
{
    double eta=mu.eta();
    double pt=mu.pt();
    bool MuKin(false);
    if(isTag)
        MuKin =( (eta<=tEtaMax && eta>= -tEtaMax) && pt>tPtMin );
    else
        MuKin =( (eta<=pEtaMax && eta>= -pEtaMax) && pt>pPtMin );

    return MuKin;

}

bool
TriggerMuonTool::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
    //initialize handles
    iEvent.getByToken(muonsrc,muonhandle);
    iEvent.getByToken(elesrc ,elehandle);
    iEvent.getByToken(vtxsrc ,vtxhandle);
    iEvent.getByToken(hltInputTag, triggerResults);
    iEvent.getByToken(hltObjectsInputTag, triggerObjects);
    const edm::TriggerNames &names = iEvent.triggerNames(*triggerResults);

    //initialize the object by handle
    pat::MuonCollection _muons;
    if(muonhandle.isValid())
    {
        _muons = *muonhandle;
    }
    pat::ElectronCollection electrons;
    if(elehandle.isValid())
    {
        electrons = *elehandle;
    }
    if(vtxhandle.isValid() )
    {
        vtx=*vtxhandle;
    }

    //to confirm there's no electrons
    for(int i=0; i< (int)electrons.size(); i++)
    {
        if(electrons[i].pt()>10 && electrons[i].eta()<2.5 && electrons[i].eta()>-2.5)
            return false;
    }

    //pre-cut based on pt
    pat::MuonCollection muons;
    for(int i=0; i< (int)_muons.size(); i++)
    {
        if(_muons[i].pt()>10 && _muons[i].eta()<2.5 && _muons[i].eta()>-2.5)
            muons.push_back(_muons[i]);
    }
    
    //to confirm that muonlistptr only has two muons
    if(muons.size()!=2)
        return false;
    
    //randomly choose the muons for tag
    srand( (unsigned) time(NULL) );
    int first  = rand()%2;
    int second = (first+1)%2;
    
    //whether pass the pre-selection
    bool passtagid   = passId( muons[first], tagid);
    bool passprobeid = passId( muons[second], proid);

    bool tagpfiso   = passPFIso(muons[first], tPFIso);
    bool probepfiso = passPFIso(muons[second], pPFIso);
    bool tagtkiso   = passTKIso(muons[first], tTKIso);
    bool probetkiso = passTKIso(muons[second], pTKIso);

    bool passtagkin   = passKin(muons[first] , true);
    bool passprobekin = passKin(muons[second],false);

    //to confirm MC truth and their mother is the same z
    if(useMC)
    {
        if(muons[first].genLepton()== NULL || muons[second].genLepton()== NULL)
            return false;
        if (!zParent(muons))
            return false;
        if(!( GetDirectMother(muons[first].genLepton(),23) == GetDirectMother(muons[second].genLepton(),23) ))
            return false;
    }
    //tag preselection
    if(!(passtagid && tagpfiso && passtagkin && tagtkiso)) return false;
    //probe preselection
    if(!(passprobeid && probepfiso && passprobekin && probetkiso)) return false;

    //mark the passing particle
    passTrigger(muons[first],names);
    passTrigger(muons[second],names);

    auto_ptr< vector<pat::Muon> > tagMuonptr ( new vector< pat::Muon> );
    auto_ptr< vector<pat::Muon> > probeMuonptr ( new vector< pat::Muon> );
    tagMuonptr->push_back(muons[first]);
    probeMuonptr->push_back(muons[second]);


    iEvent.put(tagMuonptr, "Tag");
    iEvent.put(probeMuonptr,"Probe");
    return true;



}




void TriggerMuonTool::beginStream(edm::StreamID)
{
}



void
TriggerMuonTool::endStream()
{
}

void
TriggerMuonTool::fillDescriptions(edm::ConfigurationDescriptions& descriptions)
{
    edm::ParameterSetDescription desc;
    desc.setUnknown();
    descriptions.addDefault(desc);
}

DEFINE_FWK_MODULE(TriggerMuonTool);

