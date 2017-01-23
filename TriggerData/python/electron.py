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
        fileNames=cms.untracked.vstring('/store/mc/RunIISummer16MiniAODv2/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/MINIAODSIM/PUMoriond17_HCALDebug_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/50000/00312D7A-FEBD-E611-A713-002590DB923E.root'
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
        HLTObjectsInputTag = cms.InputTag('selectedPatTrigger'),
        #trigger we used
        triggerCache = cms.VPSet(
            cms.PSet(
                HLTName = cms.string("HLT_Ele32_eta2p1_WPTight_Gsf_v*"),
                FilterName = cms.string("hltEle32WPTightGsfTrackIsoFilter")
            ),
            cms.PSet(
                HLTName = cms.string("HLT_Ele27_eta2p1_WPLoose_Gsf_v*"),
                FilterName = cms.string("hltEle27erWPLooseGsfTrackIsoFilter")
            ),
            cms.PSet(
                HLTName = cms.string("HLT_Ele45_WPLoose_Gsf_v*"),
                FilterName = cms.string("hltEle45WPLooseGsfTrackIsoFilter")
            ),
            cms.PSet(
                HLTName = cms.string("HLT_Ele27_WPTight_Gsf_v*"),
                FilterName = cms.string("hltEle27WPTightGsfTrackIsoFilter")
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
        fileName = cms.untracked.string("result/MCdata/electron_MC.root"),

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



