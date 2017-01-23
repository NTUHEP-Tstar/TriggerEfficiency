import FWCore.ParameterSet.Config as cms

process = cms.Process("ElectronAnalz")
process.load("FWCore.MessageService.MessageLogger_cfi")
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )



process.source = cms.Source("PoolSource",

fileNames = cms.untracked.vstring('file:/wk_cms/sam7k9621/electron/era_h2/output_electron_2.root')
)
import FWCore.PythonUtilities.LumiList as LumiList
process.source.lumisToProcess = LumiList.LumiList(filename = '/wk_cms/sam7k9621/golden.json').getVLuminosityBlockRange()
process.demo = cms.EDAnalyzer(
   'TriggerAnalyzerElectron',
   tag = cms.InputTag('electrontool','Tag'),
   probe = cms.InputTag('electrontool','Probe'),
   etabin = cms.vdouble(-2.5, -2.1, -2, -1.566, -1.444, -0.8 ,0, 0.8 ,1.444, 1.566, 2, 2.1, 2.5),
   tagtrigger = cms.VPSet(
       cms.PSet(
           ptbin = cms.vdouble(10,20,30,35,40,41,42,43,44,45,46,47,48,49,50,51,52,53,60,200),
           HLT = cms.vstring("HLT_Ele27_WPTight_Gsf_v*"),
           name = cms.string("ele27"),
           ptcut = cms.double(30),
           etacut = cms.double(100)
           ),
       cms.PSet(
           ptbin = cms.vdouble(10,20,25,30,31,32,33,34,35,36,37,38,39,40,50,60,200),
           HLT = cms.vstring("HLT_Ele32_eta2p1_WPTight_Gsf_v*"),
           name = cms.string("ele32"),
           ptcut = cms.double(35),
           etacut = cms.double(2.1)
           )
       ),
   protrigger = cms.VPSet(
       cms.PSet(
           HLT = cms.vstring("HLT_Ele45_WPLoose_Gsf_v","HLT_Ele27_eta2p1_WPLoose_Gsf_v*"),
           probePtCut = cms.double(48),
           probeEtaCut = cms.double(2.1)
           ),
       cms.PSet(
           HLT = cms.vstring("HLT_Ele32_eta2p1_WPTight_Gsf_v*"),
           probePtCut = cms.double(35),
           probeEtaCut = cms.double(2.1)
           )
       )
   )
process.TFileService = cms.Service("TFileService",
fileName=cms.string('/wk_cms/sam7k9621/electron/result_analyz/electron_h2_eff_2.root')
)
process.p = cms.Path(process.demo)
