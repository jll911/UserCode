import FWCore.ParameterSet.Config as cms

process = cms.Process("PhotonsFromWenu")

process.load("PhotonJetAnalysis.PhotonsFromWenu.photonsfromwenu_cfi")
process.load("FWCore.MessageService.MessageLogger_cfi")
# configure modules via Global Tag
# https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideFrontierConditions
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = 'GR_R_38X_V13::All'


process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
    '/store/mc/Fall10/QCD_Pt-80to170_BCtoE_TuneZ2_7TeV-pythia6/AODSIM/START38_V12-v1/0006/F0F95508-0DC9-DF11-A00A-00151796D890.root',
    '/store/mc/Fall10/QCD_Pt-80to170_BCtoE_TuneZ2_7TeV-pythia6/AODSIM/START38_V12-v1/0006/D0E0D5F6-09C9-DF11-8A04-001D0967D77E.root'
    #'/store/mc/Fall10/QCD_Pt-30to80_EMEnriched_TuneZ2_7TeV-pythia6/AODSIM/START38_V12-v1/0034/4E13F346-89CC-DF11-A639-001A92810AD0.root',
    #'/store/mc/Fall10/QCD_Pt-30to80_EMEnriched_TuneZ2_7TeV-pythia6/AODSIM/START38_V12-v1/0034/30CFC3B1-BECC-DF11-8EDE-002618FDA259.root'
    #'/store/mc/Fall10/G_Pt_30to50_TuneZ2_7TeV_pythia6/AODSIM/START38_V12-v1/0004/8C11A24A-04CC-DF11-B0DE-001D0967DEF4.root',
    #'/store/mc/Fall10/G_Pt_30to50_TuneZ2_7TeV_pythia6/AODSIM/START38_V12-v1/0004/1E7C8361-46CC-DF11-9B3C-0024E86E8D80.root'
    )
)

process.scrapingVeto = cms.EDFilter(
    "FilterOutScraping",
    applyfilter = cms.untracked.bool(True),
    debugOn = cms.untracked.bool(False),
    numtrack = cms.untracked.uint32(10),
    thresh = cms.untracked.double(0.25)
    )

process.primaryVertexFilter = cms.EDFilter(
    "GoodVertexFilter",
    vertexCollection = cms.InputTag('offlinePrimaryVertices'),
    minimumNDOF      = cms.uint32  (4),
    maxAbsZ          = cms.double  (18),
    maxd0            = cms.double  (2)
    )

process.triggerFilter = cms.EDFilter(
    "TriggerResultsFilter",
    hltResults = cms.InputTag( "TriggerResults", "", "REDIGI38X" ), # For some MC
    #hltResults = cms.InputTag( "TriggerResults", "", "HLT" ),      # For Data
    triggerConditions = cms.vstring( "HLT_Photon30_Cleaned_L1R" ),
    l1tResults = cms.InputTag( "" ),
    l1tIgnoreMask = cms.bool( False ),
    daqPartitions = cms.uint32( 1 ),
    throw = cms.bool( True )
    )

process.filterMET = cms.EDFilter(
    "CandViewSelector",
    cut = cms.string('et > 15'),
    src = cms.InputTag("pfMet"), # Or just 'met'
    filter = cms.bool(True)
    )

## process.filterElectrons = cms.EDFilter(
##     "GsfElectronSelector",  # more specific form of CandViewSelector
##     cut = cms.string('et>20 & sigmaIetaIeta < 0.01 & deltaPhiSuperClusterTrackAtVtx<0.06 & deltaEtaSuperClusterTrackAtVtx<0.004 & hadronicOverEm<0.04 & dr03TkSumPt < et*0.07 & dr03EcalRecHitSumEt < et*0.09 & dr03HcalTowerSumEt < et*0.10'),
##     src = cms.InputTag("gsfElectrons"),
##     filter = cms.bool(True)
##     )

process.TFileService = cms.Service(
    "TFileService",
    fileName = cms.string('photonsFromWenu.root')
    )

process.photonsfromwenu.hltResultsCollection   = cms.InputTag( "TriggerResults", "", "REDIGI38X" )

#process.nEventsTotal    = cms.EDProducer("EventCountProducer")

process.p = cms.Path(
    #process.nEventsTotal        *
    process.scrapingVeto        *
    process.primaryVertexFilter *
    process.triggerFilter       *
    process.filterMET           *
#        process.filterElectrons     *
    process.photonsfromwenu)
