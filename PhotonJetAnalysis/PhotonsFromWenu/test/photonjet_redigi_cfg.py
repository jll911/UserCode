import FWCore.ParameterSet.Config as cms

process = cms.Process("PhotonsFromWenu")

process.load("PhotonJetAnalysis.PhotonsFromWenu.photonsfromwenu_cfi")
process.load("FWCore.MessageService.MessageLogger_cfi")
# configure modules via Global Tag
# https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideFrontierConditions
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = 'GR_R_38X_V13::All'  # 38X Monte Carlo
#process.GlobalTag.globaltag = 'FT_R_38X_V14A::All' # 386 Data

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100) )
process.MessageLogger.cerr.FwkReport.reportEvery = 1

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
    #'/store/mc/Fall10/QCD_Pt-80to170_BCtoE_TuneZ2_7TeV-pythia6/AODSIM/START38_V12-v1/0006/F0F95508-0DC9-DF11-A00A-00151796D890.root',
    #'/store/mc/Fall10/QCD_Pt-80to170_BCtoE_TuneZ2_7TeV-pythia6/AODSIM/START38_V12-v1/0006/D0E0D5F6-09C9-DF11-8A04-001D0967D77E.root',
    #'/store/mc/Fall10/QCD_Pt-80to170_BCtoE_TuneZ2_7TeV-pythia6/AODSIM/START38_V12-v1/0006/CE268E45-14C9-DF11-A5CA-001D0967D535.root',
    #'/store/mc/Fall10/QCD_Pt-80to170_BCtoE_TuneZ2_7TeV-pythia6/AODSIM/START38_V12-v1/0006/BC4C4A68-5DC9-DF11-9731-0015178C65F4.root',
    #'/store/mc/Fall10/QCD_Pt-80to170_BCtoE_TuneZ2_7TeV-pythia6/AODSIM/START38_V12-v1/0006/B0B06BDB-0CC9-DF11-9F74-0015178C0100.root',
    #'/store/mc/Fall10/QCD_Pt-80to170_BCtoE_TuneZ2_7TeV-pythia6/AODSIM/START38_V12-v1/0006/AC5FF6EE-18C9-DF11-A946-0024E87687BE.root'
    '/store/mc/Fall10/QCD_Pt-80to170_EMEnriched_TuneZ2_7TeV-pythia6/AODSIM/START38_V12-v1/0004/FEED4BF2-FACB-DF11-9855-0015170AE288.root',
    '/store/mc/Fall10/QCD_Pt-80to170_EMEnriched_TuneZ2_7TeV-pythia6/AODSIM/START38_V12-v1/0004/FEC6F044-F3CB-DF11-BCAB-003048D47716.root',
    '/store/mc/Fall10/QCD_Pt-80to170_EMEnriched_TuneZ2_7TeV-pythia6/AODSIM/START38_V12-v1/0004/F8EDCE41-F2CB-DF11-BB8A-002481E150B6.root',
    '/store/mc/Fall10/QCD_Pt-80to170_EMEnriched_TuneZ2_7TeV-pythia6/AODSIM/START38_V12-v1/0004/F89AD195-F3CB-DF11-87D8-0025B3E05BC4.root',
    '/store/mc/Fall10/QCD_Pt-80to170_EMEnriched_TuneZ2_7TeV-pythia6/AODSIM/START38_V12-v1/0004/F4377A45-F8CB-DF11-A2D1-0025B31E3CBE.root',
    '/store/mc/Fall10/QCD_Pt-80to170_EMEnriched_TuneZ2_7TeV-pythia6/AODSIM/START38_V12-v1/0004/F0B93253-F0CB-DF11-B7E4-002481E15522.root',
    '/store/mc/Fall10/QCD_Pt-80to170_EMEnriched_TuneZ2_7TeV-pythia6/AODSIM/START38_V12-v1/0004/F01560F5-00CC-DF11-BFC7-002481E14F1E.root'
    #'/store/mc/Fall10/G_Pt_30to50_TuneZ2_7TeV_pythia6/AODSIM/START38_V12-v1/0004/8C11A24A-04CC-DF11-B0DE-001D0967DEF4.root',
    #'/store/mc/Fall10/G_Pt_30to50_TuneZ2_7TeV_pythia6/AODSIM/START38_V12-v1/0004/1E7C8361-46CC-DF11-9B3C-0024E86E8D80.root',
    #'/store/mc/Fall10/G_Pt_30to50_TuneZ2_7TeV_pythia6/AODSIM/START38_V12-v1/0003/ACDD5A05-D7CB-DF11-BC02-0015178C4B94.root',
    #'/store/mc/Fall10/G_Pt_30to50_TuneZ2_7TeV_pythia6/AODSIM/START38_V12-v1/0002/7603E817-B6CB-DF11-9465-001D096B1007.root',
#    '/store/mc/Fall10/G_Pt_30to50_TuneZ2_7TeV_pythia6/AODSIM/START38_V12-v1/0002/4A445DB8-BDCB-DF11-803E-00151796D708.root',
#    '/store/mc/Fall10/G_Pt_50to80_TuneZ2_7TeV_pythia6/AODSIM/START38_V12-v1/0002/6853C639-49C9-DF11-874C-001D0967D55D.root',
#    '/store/mc/Fall10/G_Pt_50to80_TuneZ2_7TeV_pythia6/AODSIM/START38_V12-v1/0001/D4E744AC-BCC8-DF11-8F15-0024E86E8D25.root',
    #'/store/mc/Fall10/G_Pt_50to80_TuneZ2_7TeV_pythia6/AODSIM/START38_V12-v1/0001/D266DB7D-B4C8-DF11-82FF-0024E87680F4.root'
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
    #maxAbsZ          = cms.double  (18),
    maxAbsZ          = cms.double(24),
    maxd0            = cms.double  (2)
    )

process.load('CommonTools/RecoAlgos/HBHENoiseFilter_cfi')
process.HBHENoiseFilter.maxRBXEMF = cms.double(0.01)

process.triggerFilter = cms.EDFilter(
    "TriggerResultsFilter",
    hltResults = cms.InputTag( "TriggerResults", "", "REDIGI38X" ), # For MC
    #hltResults = cms.InputTag( "TriggerResults", "", "HLT" ),        # For Data
    triggerConditions = cms.vstring( "HLT_Photon30_Cleaned_L1R" ),   # For Data
    l1tResults = cms.InputTag( "" ),
    l1tIgnoreMask = cms.bool( False ),
    l1techIgnorePrescales  = cms.bool( True ),
    daqPartitions = cms.uint32( 1 ),
    throw = cms.bool( True )
    )

## process.filterMET = cms.EDFilter(
##     "CandViewSelector",
##     cut = cms.string('et > 15'),
##     src = cms.InputTag("pfMet"), # Or just 'met'
##     filter = cms.bool(True)
##     )

## process.filterElectrons = cms.EDFilter(
##     "GsfElectronSelector",  # more specific form of CandViewSelector
##     cut = cms.string('et>20 & sigmaIetaIeta < 0.01 & deltaPhiSuperClusterTrackAtVtx<0.06 & deltaEtaSuperClusterTrackAtVtx<0.004 & hadronicOverEm<0.04 & dr03TkSumPt < et*0.07 & dr03EcalRecHitSumEt < et*0.09 & dr03HcalTowerSumEt < et*0.10'),
##     src = cms.InputTag("gsfElectrons"),
##     filter = cms.bool(True)
##     )

process.TFileService = cms.Service(
    "TFileService",
    fileName = cms.string('photonjet.root')
    )

process.photonsfromwenu.hltResultsCollection   = cms.InputTag( "TriggerResults", "", "REDIGI38X" )
process.photonsfromwenu.minTransMass           = cms.double( -1.0 )
process.photonsfromwenu.minMetEt               = cms.double( -1.0 )
process.photonsfromwenu.minTransMass           = cms.double( -1.0 )
process.photonsfromwenu.minMetEt               = cms.double( -1.0 )
process.photonsfromwenu.requireElecPassCuts    = cms.bool( False )
process.photonsfromwenu.usePhotonMatchedToElec = cms.bool( False )
process.photonsfromwenu.minJetPt               = cms.double( 30.0 )

#process.nEventsTotal    = cms.EDProducer("EventCountProducer")

process.load('JetMETCorrections.Configuration.DefaultJEC_cff')

process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")
process.printTree = cms.EDAnalyzer("ParticleTreeDrawer",
                                   src = cms.InputTag("genParticles"),
                                   #src = cms.InputTag("genParticleCandidates"),
                                   printP4 = cms.untracked.bool(False),
                                   printPtEtaPhi = cms.untracked.bool(False),
                                   printVertex = cms.untracked.bool(False),
                                   printStatus = cms.untracked.bool(False),
                                   printIndex = cms.untracked.bool(False),
                                   #status = cms.untracked.vint32( 3 )
                                   )   

process.p = cms.Path(
    #process.nEventsTotal        *
    process.scrapingVeto        *
    process.primaryVertexFilter *
    process.HBHENoiseFilter     *
    process.triggerFilter       *
    #process.filterMET           *
    #process.filterElectrons     *
    process.ak5PFJetsL2L3 *
    process.photonsfromwenu*
    process.printTree)
