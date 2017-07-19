import FWCore.ParameterSet.Config as cms
process = cms.Process("TagAndProbe")

process.load("FWCore.MessageService.MessageLogger_cfi")
#https://github.com/ikrav/EgammaWork/blob/ntupler_and_VID_demos_8.0.3/ElectronNtupler/test/runElectrons_VID_CutBased_Summer16_HLTSafe_demo.py
process.load("Configuration.StandardSequences.GeometryRecoDB_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")


from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc', '')


process.MessageLogger.cerr.FwkReport.reportEvery = 1

process.maxEvents = cms.untracked.PSet( input=cms.untracked.int32(-1))

process.source = cms.Source(
        "PoolSource",
        fileNames=cms.untracked.vstring('file:/wk_cms2/yichen/public/MiniAOD/Run2017/SingleElectron_Run2017A-PromptReco-v2.root'
            )
        )

process.options = cms.untracked.PSet(wantSummary=cms.untracked.bool(True))

#######################################

from PhysicsTools.SelectorUtils.tools.vid_id_tools import *
dataFormat = DataFormat.MiniAOD
switchOnVIDElectronIdProducer(process, dataFormat)
# define which IDs we want to produce
my_id_modules = ['RecoEgamma.ElectronIdentification.Identification.cutBasedElectronHLTPreselecition_Summer16_V1_cff',
                 'RecoEgamma.ElectronIdentification.Identification.cutBasedElectronID_Summer16_80X_V1_cff',
                 'RecoEgamma.ElectronIdentification.Identification.heepElectronID_HEEPV60_cff']

#add them to the VID producer
for idmod in my_id_modules:
    setupAllVIDIdsInModule(process,idmod,setupVIDElectronSelection)

########################################

process.electrontool = cms.EDFilter(
        "TriggerElectronTool",
        muonsrc = cms.InputTag( "slimmedMuons"),
        elesrc = cms.InputTag("slimmedElectrons"),
        #trigger object
        vtxsrc = cms.InputTag("offlineSlimmedPrimaryVertices"),
        HLTInputTag = cms.InputTag('TriggerResults','','HLT'),
        HLTObjectsInputTag = cms.InputTag('slimmedPatTrigger'),
        #trigger we used
        triggerCache = cms.VPSet(
            cms.PSet(
                HLTName = cms.string("HLT_Ele35_WPTight_Gsf_v*"),
                FilterName = cms.string("hltEle35noerWPTightGsfTrackIsoFilter")
            ),
            cms.PSet(
                HLTName = cms.string("HLT_Ele38_WPTight_Gsf_v*"),
                FilterName = cms.string("hltEle38noerWPTightGsfTrackIsoFilter")
            ),
            cms.PSet(
                HLTName = cms.string("HLT_Ele40_WPTight_Gsf_v*"),
                FilterName = cms.string("hltEle40noerWPTightGsfTrackIsoFilter")
            )
        ),
       #tag criteria
       TagPassID = cms.string("tight"),   #Require tag electron to pass a ID ( input will be "loose"/"tight"/"medium"/"heep" )
       tagImpact = cms.bool(True),
       #probe criteria
       ProbePassID = cms.string("tight"),  #Require probe electron to pass a ID ( input will be "loose"/"tight"/"medium"/"heep" )
       probeImpact = cms.bool(True),

       #safe cut and ID
       #https://twiki.cern.ch/twiki/bin/view/CMS/CutBasedElectronIdentificationRun2#Recipe80X
       looseMap = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-Summer16-80X-V1-loose"),
       mediumMap = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-Summer16-80X-V1-medium"),
       tightMap = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-Summer16-80X-V1-tight"),
       heepMap = cms.InputTag("egmGsfElectronIDs:heepElectronID-HEEPV60"),
       safeCut = cms.InputTag("egmGsfElectronIDs:cutBasedElectronHLTPreselection-Summer16-V1"),
       #customized cut
       Zmassmin = cms.double(60),
       Zmassmax = cms.double(120),
       tagPtMin = cms.double(30),
       tagEtaMax = cms.double(2.5),
       probePtMin = cms.double(8),
       probeEtaMax = cms.double(2.5),
       useMC = cms.bool(False)   # Check whether lepton has gen level lepto and  whether tag and probe has MC truth Z parent
        )


process.filterpath = cms.Path(
        process.egmGsfElectronIDSequence
        *process.electrontool
        )


process.edmOut = cms.OutputModule(
        "PoolOutputModule",
        fileName = cms.untracked.string("2017_electron.root"),

        # datatype _ module(process.xxx) _ label(comes from .cc) _ process(cms.Process)
        outputCommands=cms.untracked.vstring(
            "keep *",
            "drop *_slimmedPhotons_*_*",
            "drop *_slimmedJets*_*_*",
            "drop *_slimmedTaus_*_*"

            #"keep *_electrontool_*_*"
            ),
        SelectEvents=cms.untracked.PSet(SelectEvents=cms.vstring('filterpath'))
        )

process.endPath = cms.EndPath(
        process.edmOut
        )



