import FWCore.ParameterSet.Config as cms

process = cms.Process("PhotonsFromWenu")

process.load("PhotonJetAnalysis.PhotonsFromWenu.photonsfromwenu_cfi")
process.load("FWCore.MessageService.MessageLogger_cfi")
# configure modules via Global Tag
# https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideFrontierConditions
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
#process.GlobalTag.globaltag = 'GR_R_38X_V13::All'
#process.GlobalTag.globaltag = 'FT_R_38X_V14A' # 38X Data
#process.GlobalTag.globaltag = 'FT_R_39X_V4A::All' # 39X Data
#process.GlobalTag.globaltag = 'START42_V13::All' # 42X MC
#process.GlobalTag.globaltag = 'GR_P_V22::All' # 2011 PromptReco v4
process.GlobalTag.globaltag = 'GR_R_42_V20::All' # 2011 reprocessing

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10000) )
process.MessageLogger.cerr.FwkReport.reportEvery = 100

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(

    # 2011 data

    '/store/data/Run2011A/Photon/AOD/PromptReco-v4/000/165/400/104198A0-0485-E011-8469-003048F110BE.root',
    '/store/data/Run2011A/Photon/AOD/PromptReco-v4/000/165/400/32F050C7-E584-E011-8B94-0030487A3DE0.root',
    '/store/data/Run2011A/Photon/AOD/PromptReco-v4/000/165/400/7C9AAD82-ED84-E011-80FA-0030487CD178.root',
    '/store/data/Run2011A/Photon/AOD/PromptReco-v4/000/165/484/3E5B36CC-3786-E011-80D4-001D09F2915A.root',
    '/store/data/Run2011A/Photon/AOD/PromptReco-v4/000/165/620/5056A7B0-7888-E011-946A-001D09F29146.root',
    '/store/data/Run2011A/Photon/AOD/PromptReco-v4/000/165/620/7420733C-9288-E011-9C31-0019B9F72F97.root',
    '/store/data/Run2011A/Photon/AOD/PromptReco-v4/000/165/999/2EB23724-7E8B-E011-B473-003048F118E0.root',
    '/store/data/Run2011A/Photon/AOD/PromptReco-v4/000/165/999/B88B6312-838B-E011-B3C6-001D09F28EA3.root'

    )
)

process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Geometry.CaloEventSetup.CaloGeometry_cfi")
process.load("Geometry.CMSCommonData.cmsIdealGeometryXML_cfi")

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
    #hltResults = cms.InputTag( "TriggerResults", "", "REDIGI38X" ), # For MC
    hltResults = cms.InputTag( "TriggerResults", "", "HLT" ),        # For Data
    #triggerConditions = cms.vstring( "HLT_Photon30_Cleaned_L1R" ),   # For Data
#    triggerConditions = cms.vstring( "HLT_Photon75_CaloIdVL_v2" ),   # For MC... Summer11 random path
    triggerConditions = cms.vstring( "HLT_Photon75_CaloIdVL_v4" ),   # For data, random run
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

#process.photonsfromwenu.hltResultsCollection   = cms.InputTag( "TriggerResults", "", "REDIGI38X" )
process.photonsfromwenu.minTransMass           = cms.double( -1.0 )
process.photonsfromwenu.minMetEt               = cms.double( -1.0 )
process.photonsfromwenu.minTransMass           = cms.double( -1.0 )
process.photonsfromwenu.minMetEt               = cms.double( -1.0 )
process.photonsfromwenu.requireElecPassCuts    = cms.bool( False )
process.photonsfromwenu.usePhotonMatchedToElec = cms.bool( False )
process.photonsfromwenu.minJetPt               = cms.double( 30.0 )

#process.nEventsTotal    = cms.EDProducer("EventCountProducer")

process.load('JetMETCorrections.Configuration.DefaultJEC_cff')

process.p = cms.Path(
    #process.nEventsTotal        *
    process.scrapingVeto        *
    process.primaryVertexFilter *
    process.HBHENoiseFilter     *
    process.triggerFilter       *
    #process.filterMET           *
    #process.filterElectrons     *
    process.ak5PFJetsL2L3 *
    process.photonsfromwenu)
