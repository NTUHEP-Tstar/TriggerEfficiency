import FWCore.ParameterSet.Config as cms

process = cms.Process("TEfficiency")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
fileNames = cms.untracked.vstring('file:/wk_cms/sam7k9621/MC_data/muon/MCData_v2/muon_9.root')
)
process.muoneff = cms.EDAnalyzer(
    'TriggerMCMuonEff',
    tag = cms.InputTag("muontool","Tag"),
    probe =  cms.InputTag("muontool","Probe"),
    pusrc = cms.InputTag("slimmedAddPileupInfo"),
    filename = cms.FileInPath("TriggerMCTool/TriggerEff/test/pileupweight_69200.csv"),
    etabin = cms.vdouble(0,0.9,1.2,2.1,2.4),
    ptbin = cms.vdouble(20,25,30,40,50,60,120),
    tagtrigger = cms.VPSet(
        cms.PSet(
            HLT = cms.vstring("HLT_IsoMu22_v*","HLT_IsoTkMu22_v*"),
            name = cms.string("total_mu22_tkmu22"),
            ptcut = cms.double(25),
            etacut = cms.double(100)
            ),
        cms.PSet(
            HLT = cms.vstring("HLT_IsoMu24_v*","HLT_IsoTkMu24_v*"),
            name = cms.string("total_mu24_tkmu24"),
            ptcut = cms.double(27),
            etacut = cms.double(100)
            ),
        cms.PSet(
            HLT = cms.vstring("HLT_IsoMu27_v*","HLT_IsoTkMu27_v*"),
            name = cms.string("total_mu27_tkmu27"),
            ptcut = cms.double(30),
            etacut = cms.double(100)
            ),
        cms.PSet(
            HLT = cms.vstring("HLT_Mu45_eta2p1_v*"),
            name = cms.string("total_mu45"),
            ptcut = cms.double(50),
            etacut = cms.double(2.1)
            ),
        cms.PSet(
            HLT = cms.vstring("HLT_Mu50_v*","HLT_TkMu50_v*"),
            name = cms.string("total_mu50_tkmu50"),
            ptcut = cms.double(53),
            etacut = cms.double(100)
            )
        ),
    protrigger = cms.VPSet(
        cms.PSet(
            HLT = cms.vstring("HLT_IsoMu22_v*","HLT_IsoTkMu22_v*"),
            name = cms.string("pass_mu22_tkmu22")
            ),
        cms.PSet(
            HLT = cms.vstring("HLT_IsoMu24_v*","HLT_IsoTkMu24_v*"),
            name = cms.string("pass_mu24_tkmu24")
            ),
        cms.PSet(
            HLT = cms.vstring("HLT_IsoMu27_v*","HLT_IsoTkMu27_v*"),
            name = cms.string("pass_mu27_tkmu27")
            ),
        cms.PSet(
            HLT = cms.vstring("HLT_Mu45_eta2p1_v*"),
            name = cms.string("pass_mu45")
            ),
        cms.PSet(
            HLT = cms.vstring("HLT_Mu50_v*","HLT_TkMu50_v*"),
            name = cms.string("pass_mu50_tkmu50")
            )
        )
)
process.TFileService = cms.Service("TFileService",
fileName=cms.string('/wk_cms/sam7k9621/MC_data/muon/result_eff_v2/tight_MC_eff_80.root')
)
process.p = cms.Path(
        process.muoneff
        )
