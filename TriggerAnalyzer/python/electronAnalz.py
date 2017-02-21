import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as opts

options = opts.VarParsing('analysis')
options.register(
    'inputfile',
    '',
    opts.VarParsing.multiplicity.list,
    opts.VarParsing.varType.string,
    'root file to process'
)

options.register(
    'outputfile',
    '',
    opts.VarParsing.multiplicity.singleton,
    opts.VarParsing.varType.string,
    'output file name'
)

options.register(
    'useMC',
    'data',
    opts.VarParsing.multiplicity.singleton,
    opts.VarParsing.varType.bool,
    'use MC sample or not'
)

options.parseArguments()

#-----------------------------------------------

process = cms.Process("demo")
process.load("FWCore.MessageService.MessageLogger_cfi")
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",

fileNames = cms.untracked.vstring(options.inputfile)
)

if not options.useMC :
    import FWCore.PythonUtilities.LumiList as LumiList
    process.source.lumisToProcess = LumiList.LumiList(filename = '/wk_cms/sam7k9621/CMSSW_8_0_10/src/TriggerEfficiency/TriggerData/data/golden.json').getVLuminosityBlockRange()

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
           ptcut = cms.double(32),
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
           HLT = cms.vstring("HLT_Ele45_WPLoose_Gsf_v*","HLT_Ele27_eta2p1_WPLoose_Gsf_v*"),
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
fileName=cms.string(options.outputfile)
)
process.p = cms.Path(process.demo)
