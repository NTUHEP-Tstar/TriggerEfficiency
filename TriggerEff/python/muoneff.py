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

process = cms.Process("TEfficiency")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
fileNames = cms.untracked.vstring(inputfile)
)
if not options.useMC :
    import FWCore.PythonUtilities.LumiList as LumiList
    process.source.lumisToProcess = LumiList.LumiList(filename = '/wk_cms/sam7k9621/CMSSW_8_0_10/src/TriggerEfficiency/TriggerData/data/golden.json').getVLuminosityBlockRange()
process.muoneff = cms.EDAnalyzer(
    'TriggerMuonEff',
    tag = cms.InputTag("muontool","Tag"),
    probe =  cms.InputTag("muontool","Probe"),
    etabin = cms.vdouble(0,0.9,1.2,2.1,2.4),
    ptbin = cms.vdouble(20,25,30,40,50,60,120),
    tagtrigger = cms.VPSet(
        cms.PSet(
            HLT = cms.vstring("HLT_IsoMu22_v*","HLT_IsoTkMu22_v*"),
            name = cms.string("total_mu22_tkmu22"),
            ptcut = cms.double(30),
            etacut = cms.double(100)
            ),
        cms.PSet(
            HLT = cms.vstring("HLT_IsoMu24_v*","HLT_IsoTkMu24_v*"),
            name = cms.string("total_mu24_tkmu24"),
            ptcut = cms.double(32),
            etacut = cms.double(100)
            ),
        cms.PSet(
            HLT = cms.vstring("HLT_IsoMu27_v*","HLT_IsoTkMu27_v*"),
            name = cms.string("total_mu27_tkmu27"),
            ptcut = cms.double(35),
            etacut = cms.double(100)
            ),
        cms.PSet(
            HLT = cms.vstring("HLT_Mu45_eta2p1_v*"),
            name = cms.string("total_mu45"),
            ptcut = cms.double(55),
            etacut = cms.double(2.1)
            ),
        cms.PSet(
            HLT = cms.vstring("HLT_Mu50_v*","HLT_TkMu50_v*"),
            name = cms.string("total_mu50_tkmu50"),
            ptcut = cms.double(58),
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
fileName=cms.string(options.outputfile)
)
process.p = cms.Path(
        process.muoneff
        )
