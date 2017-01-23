import FWCore.ParameterSet.Config as cms

process = cms.Process("TEfficiency")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
fileNames = cms.untracked.vstring('file:/wk_cms/sam7k9621/MC_data/electron/MCData_v2/electron_9.root')
)
process.electroneff = cms.EDAnalyzer(
   'TriggerMCElectronEff',
   tag = cms.InputTag("electrontool","Tag"),
   probe =  cms.InputTag("electrontool","Probe"),
   etabin = cms.vdouble(-2.5, -2.1, -2, -1.566, -1.444, -0.8 ,0, 0.8 ,1.444, 1.566, 2, 2.1, 2.5),
   pusrc = cms.InputTag("slimmedAddPileupInfo"),
   filename = cms.FileInPath("TriggerMCTool/TriggerEff/test/pileupweight_69200.csv"),
   ptbin = cms.vdouble(10,20,30,40,50,200),
   tagtrigger = cms.VPSet(
       cms.PSet(
           HLT = cms.vstring("HLT_Ele27_WPTight_Gsf_v*"),
           name = cms.string("total_ele27"),
           ptcut = cms.double(30),
           etacut = cms.double(100)
           ),
       cms.PSet(
           HLT = cms.vstring("HLT_Ele32_eta2p1_WPTight_Gsf_v*"),
           name = cms.string("total_ele32"),
           ptcut = cms.double(35),
           etacut = cms.double(2.1)
           )
       ),
   protrigger = cms.VPSet(
       cms.PSet(
           HLT = cms.vstring("HLT_Ele45_WPLoose_Gsf_v*","HLT_Ele27_eta2p1_WPLoost_Gsf_v*"),
           name = cms.string("pass_ele45_ele27")
           ),
       cms.PSet(
           HLT = cms.vstring("HLT_Ele32_eta2p1_WPTight_Gsf_v*"),
           name = cms.string("pass_ele32")
           )
       )
   )
process.TFileService = cms.Service("TFileService",
fileName=cms.string('/wk_cms/sam7k9621/MC_data/electron/result_eff_v2/electron_MC_eff_80.root')
 )

process.p = cms.Path(
        process.electroneff
        )
