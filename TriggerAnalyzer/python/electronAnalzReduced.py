import FWCore.ParameterSet.Config as cms

process = cms.Process("ElectronAnalz")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",

fileNames = cms.untracked.vstring('file:/wk_cms/sam7k9621/electron/era_h2/output_electron_2.root')
)
import FWCore.PythonUtilities.LumiList as LumiList
process.source.lumisToProcess = LumiList.LumiList(filename = '/wk_cms/sam7k9621/reduced.json').getVLuminosityBlockRange()
process.demo = cms.EDAnalyzer(
   'TriggerAnalyzerElectron',
   tag = cms.InputTag('electrontool','Tag'),
   probe = cms.InputTag('electrontool','Probe'),
   etabin = cms.vdouble(-2.5, -2, -1.566, -1.444, -0.8 ,0, 0.8 ,1.444, 1.566, 2, 2.5),
   tagtrigger = cms.VPSet(
       cms.PSet(
           ptbin = cms.vdouble(10,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,40,50,60,200),
           HLT = cms.vstring("HLT_Ele27_WPTight_Gsf_v*"),
           name = cms.string("ele27_tight"),
           ptcut = cms.double(0),
           etacut = cms.double(100)
           ),
       cms.PSet(
           ptbin = cms.vdouble(10,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,40,50,200),
           HLT = cms.vstring("HLT_Ele27_WPTight_Gsf_v*"),
           name = cms.string("ele27_tight_eta2p1"),
           ptcut = cms.double(0),
           etacut = cms.double(100)
           )
       ),
   protrigger = cms.VPSet(
       cms.PSet(
           HLT = cms.vstring("HLT_Ele27_WPTight_Gsf_v*"),
           probePtCut = cms.double(32),
           probeEtaCut = cms.double(100)
           ),
       cms.PSet(
           HLT = cms.vstring("HLT_Ele27_WPTight_Gsf_v*"),
           probePtCut = cms.double(32),
           probeEtaCut = cms.double(2.1)
           )
       )
   )
process.TFileService = cms.Service("TFileService",
fileName=cms.string('/wk_cms/sam7k9621/electron/result_analyzreduced/electron_h2_eff_2.root')
)
process.p = cms.Path(process.demo)
