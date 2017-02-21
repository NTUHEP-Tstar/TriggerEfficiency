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

#---------------------------------------------------

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
   'TriggerAnalyzerMuon',
   tag = cms.InputTag('muontool','Tag'),
   probe = cms.InputTag('muontool','Probe'),
   etabin = cms.vdouble(-2.4,-2.1,-1.6,-1.2,-0.9,-0.3,-0.2,0,0.2,0.3,0.9,1.2,1.6,2.1,2.4),
   tagtrigger = cms.VPSet(
        cms.PSet(
            ptbin = cms.vdouble(20,21,22,23,24,25,26,27,28,29,30,40,50,60,120),
            HLT = cms.vstring("HLT_IsoMu22_v*","HLT_IsoTkMu22_v*"),
            name = cms.string("total_mu22_tkmu22"),
            ptcut = cms.double(25),
            etacut = cms.double(100)
            ),
        cms.PSet(
            ptbin = cms.vdouble(20,21,22,23,24,25,26,27,28,29,30,40,50,60,120),
            HLT = cms.vstring("HLT_IsoMu24_v*","HLT_IsoTkMu24_v*"),
            name = cms.string("toal_mu24_tkmu24"),
            ptcut = cms.double(27),
            etacut = cms.double(100)
            ),
        cms.PSet(
            ptbin = cms.vdouble(20,21,22,23,24,25,26,27,28,29,30,40,50,60,120),
            HLT = cms.vstring("HLT_IsoMu27_v*","HLT_IsoTkMu27_v*"),
            name = cms.string("total_mu27_tkmu27"),
            ptcut = cms.double(30),
            etacut = cms.double(100)
            ),
        cms.PSet(
            ptbin = cms.vdouble(20,25,30,40,41,42,43,44,45,46,47,48,49,50,60,120),
            HLT = cms.vstring("HLT_Mu45_eta2p1_v*"),
            name = cms.string("total_mu45"),
            ptcut = cms.double(50),
            etacut = cms.double(2.1)
            ),
        cms.PSet(
            ptbin = cms.vdouble(20,25,30,40,45,46,47,48,49,50,51,52,53,54,55,60,120),
            HLT = cms.vstring("HLT_Mu50_v*","HLT_TkMu50_v*"),
            name = cms.string("total_mu50_tkmu50"),
            ptcut = cms.double(53),
            etacut = cms.double(100)
            )
        ),
   protrigger = cms.VPSet(
        cms.PSet(
            HLT = cms.vstring("HLT_IsoMu22_v*","HLT_IsoTkMu22_v*"),
            probeEtaCut = cms.double(100),
            probePtCut = cms.double(25)
            ),
        cms.PSet(
            HLT = cms.vstring("HLT_IsoMu24_v*","HLT_IsoTkMu24_v*"),
            probeEtaCut = cms.double(100),
            probePtCut = cms.double(27)
            ),
        cms.PSet(
            HLT = cms.vstring("HLT_IsoMu27_v*","HLT_IsoTkMu27_v*"),
            probeEtaCut = cms.double(100),
            probePtCut = cms.double(30)
            ),
        cms.PSet(
            HLT = cms.vstring("HLT_Mu45_eta2p1_v*"),
            probeEtaCut = cms.double(2.1),
            probePtCut = cms.double(50)
            ),
        cms.PSet(
            HLT = cms.vstring("HLT_Mu50_v*","HLT_TkMu50_v*"),
            probeEtaCut = cms.double(100),
            probePtCut = cms.double(53)
            )
        )
)

process.TFileService = cms.Service("TFileService",
fileName=cms.string(options.outputfile)
)
process.p = cms.Path(process.demo)
