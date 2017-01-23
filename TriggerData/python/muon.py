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
        fileNames=cms.untracked.vstring('/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISummer16MiniAODv2-PUMoriond17_HCALDebug_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'
            )
        )

process.options = cms.untracked.PSet(wantSummary=cms.untracked.bool(True))


process.muontool = cms.EDFilter(
        "TriggerMuonTool",
        muonsrc = cms.InputTag( "slimmedMuons" ),
        elesrc = cms.InputTag( "slimmedElectrons"),
        #tag criteria
        TagPassID  = cms.string("tight"),       #Require tag muon to pass a ID ( input will be "loose"/"tight"/"HighPT" )
        TagPassPFIso = cms.double(0.12),      #require tag muon to pass particle flow isolation cut
        TagPassTKIso = cms.double(0.05),     #require tag muon to pass tracking isolation cut
        #probe criteria
        ProbePassID  = cms.string("tight"),     #Require probe muon to pass a ID ( input will be "loose"/"tight"/"HighPT" )
        ProbePassPFIso = cms.double(0.12),    #require probe muon to pass particle flow isolation cut
        ProbePassTKIso = cms.double(0.05),   #require probe muon to pass tracking isolation cut
        #trigger we used
        triggerCache = cms.VPSet(
            cms.PSet(
                HLTName = cms.string("HLT_IsoMu22_v*"),
                FilterName = cms.string("hltL3crIsoL1sMu20L1f0L2f10QL3f22QL3trkIsoFiltered0p09")
            ),

            cms.PSet(
                HLTName = cms.string("HLT_IsoTkMu22_v*"),
                FilterName = cms.string("hltL3fL1sMu20L1f0Tkf22QL3trkIsoFiltered0p09")
            ),
            cms.PSet(
                HLTName = cms.string("HLT_IsoMu24_v*"),
                FilterName = cms.string("hltL3crIsoL1sMu22L1f0L2f10QL3f24QL3trkIsoFiltered0p09")
            ),
            cms.PSet(
                HLTName = cms.string("HLT_IsoTkMu24_v*"),
                FilterName = cms.string("hltL3fL1sMu22L1f0Tkf24QL3trkIsoFiltered0p09")
            ),
            cms.PSet(
                HLTName = cms.string("HLT_IsoMu27_v*"),
                FilterName = cms.string("hltL3crIsoL1sMu22Or25L1f0L2f10QL3f27QL3trkIsoFiltered0p09")
            ),
            cms.PSet(
                HLTName = cms.string("HLT_IsoTkMu27_v*"),
                FilterName = cms.string("hltL3fL1sMu22Or25L1f0Tkf27QL3trkIsoFiltered0p09")
            ),
            cms.PSet(
                HLTName = cms.string("HLT_Mu45_eta2p1_v*"),
                FilterName = cms.string("hltL3fL1sMu22Or25L1f0L2f10QL3Filtered45e2p1Q")
            ),
            cms.PSet(
                HLTName = cms.string("HLT_Mu50_v*"),
                FilterName = cms.string("hltL3fL1sMu22Or25L1f0L2f10QL3Filtered50Q")
                ),
            cms.PSet(
                HLTName = cms.string("HLT_TkMu50_v*"),
                FilterName = cms.string("hltL3fL1sMu25f0TkFiltered50Q")
                )

        ),
        #trigger object
        vtxsrc = cms.InputTag("offlineSlimmedPrimaryVertices"),
        HLTInputTag = cms.InputTag('TriggerResults','','HLT'),
        HLTObjectsInputTag = cms.InputTag('selectedPatTrigger'),
        #customized cut
        Zmassmin = cms.double(60),
        Zmassmax = cms.double(120),
        tagPtMin    = cms.double(30),
        tagEtaMax   = cms.double(2.4),
        probePtMin  = cms.double(8),
        probeEtaMax = cms.double(2.4),
        useMC = cms.bool(False)  # Check whether lepton has gen level lepto and  whether tag and probe has MC truth Z parent
        )


process.filterpath = cms.Path(
        process.muontool
        )


process.edmOut = cms.OutputModule(
        "PoolOutputModule",
        fileName = cms.untracked.string("result/MCdata"),

        # datatype _ module(process.xxx) _ label(comes from .cc) _ process(cms.Process)
        outputCommands=cms.untracked.vstring(
            "keep *",
            "drop *_slimmedPhotons_*_*",
            "drop *_slimmedJets*_*_*",
            "drop *_slimmedTaus_*_*"
           # "drop *",
           # "keep *_muontool_*_*"
            ),
        SelectEvents=cms.untracked.PSet(SelectEvents=cms.vstring('filterpath'))
        )

process.endPath = cms.EndPath(
        process.edmOut
        )



