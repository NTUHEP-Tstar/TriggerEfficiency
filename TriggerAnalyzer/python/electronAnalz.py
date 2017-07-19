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
    process.source.lumisToProcess = LumiList.LumiList(filename = '/wk_cms/sam7k9621/CMSSW_9_2_6/src/TriggerEfficiency/TriggerAnalyzer/test/lumi.txt').getVLuminosityBlockRange()

process.demo = cms.EDAnalyzer(
   'TriggerAnalyzerElectron',
   tag = cms.InputTag('electrontool','Tag'),
   probe = cms.InputTag('electrontool','Probe'),
   etabin = cms.vdouble(-2.5, -2.1, -2, -1.566, -1.444, -0.8 ,0, 0.8 ,1.444, 1.566, 2, 2.1, 2.5),
   tagtrigger = cms.VPSet(
       cms.PSet(
           ptbin = cms.vdouble(10,20,30,31,32,33,34,35,36,37,38,39,40,50,60,200),
           HLT = cms.vstring("HLT_Ele35_WPTight_Gsf_v*"),
           name = cms.string("ele35"),
           ptcut = cms.double(35),
           etacut = cms.double(100)
           ),
       cms.PSet(
           ptbin = cms.vdouble(10,20,25,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,50,60,200),
           HLT = cms.vstring("HLT_Ele38_WPTight_Gsf_v*"),
           name = cms.string("ele38"),
           ptcut = cms.double(38),
           etacut = cms.double(100)
           ),
       cms.PSet(
           ptbin = cms.vdouble(10,20,25,30,35,40,41,42,43,44,45,46,47,48,49,50,60,200),
           HLT = cms.vstring("HLT_Ele40_WPTight_Gsf_v*"),
           name = cms.string("ele40"),
           ptcut = cms.double(38),
           etacut = cms.double(100)
           )
       ),
   protrigger = cms.VPSet(
       cms.PSet(
           HLT = cms.vstring("HLT_Ele35_WPTight_Gsf_v*"),
           probePtCut = cms.double(35),
           probeEtaCut = cms.double(100)
           ),
       cms.PSet(
           HLT = cms.vstring("HLT_Ele38_WPTight_Gsf_v*"),
           probePtCut = cms.double(41),
           probeEtaCut = cms.double(100)
           ),
       cms.PSet(
           HLT = cms.vstring("HLT_Ele40_WPTight_Gsf_v*"),
           probePtCut = cms.double(43),
           probeEtaCut = cms.double(100)
           )
       )
   )

process.TFileService = cms.Service("TFileService",
fileName=cms.string(options.outputfile)
)
process.p = cms.Path(process.demo)
