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
process.GlobalTag.globaltag = 'START42_V13::All' # 42X MC

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10000) )
process.MessageLogger.cerr.FwkReport.reportEvery = 100

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
#     '/store/mc/Fall10/GJets_TuneD6T_HT-40To100_7TeV-madgraph/AODSIM/START38_V12-v1/0031/E860FA7D-96DF-DF11-AF48-842B2B17EB6A.root',
#     '/store/mc/Fall10/GJets_TuneD6T_HT-40To100_7TeV-madgraph/AODSIM/START38_V12-v1/0031/E67103E5-C1DF-DF11-9D85-00304865C254.root',
#     #'/store/mc/Fall10/GJets_TuneD6T_HT-40To100_7TeV-madgraph/AODSIM/START38_V12-v1/0031/BEF24104-2FDF-DF11-B087-001A9227D335.root',
#     #'/store/mc/Fall10/GJets_TuneD6T_HT-40To100_7TeV-madgraph/AODSIM/START38_V12-v1/0031/90D7CE52-C5DE-DF11-82A2-0002C90A3402.root',
#     #'/store/mc/Fall10/GJets_TuneD6T_HT-40To100_7TeV-madgraph/AODSIM/START38_V12-v1/0031/82AED18B-B0DF-DF11-9954-0026B95BCAC3.root',
#     #'/store/mc/Fall10/GJets_TuneD6T_HT-40To100_7TeV-madgraph/AODSIM/START38_V12-v1/0031/723ABE6B-E6DE-DF11-80AF-003048678D6C.root',
#     #'/store/mc/Fall10/GJets_TuneD6T_HT-40To100_7TeV-madgraph/AODSIM/START38_V12-v1/0031/68742FAF-B5DF-DF11-9A92-00188B7AD282.root',
#     #'/store/mc/Fall10/GJets_TuneD6T_HT-40To100_7TeV-madgraph/AODSIM/START38_V12-v1/0031/56AA4846-B4DF-DF11-A344-001E68862A2B.root'
#     #'/store/mc/Fall10/QCD_TuneD6T_HT-50To100_7TeV-madgraph/AODSIM/START38_V12-v1/0006/F008894F-D9E1-DF11-BD4F-0002C90B7F2E.root',
#     #'/store/mc/Fall10/QCD_TuneD6T_HT-50To100_7TeV-madgraph/AODSIM/START38_V12-v1/0006/EA350142-D9E1-DF11-9DF9-001E6849D360.root',
#     #'/store/mc/Fall10/QCD_TuneD6T_HT-50To100_7TeV-madgraph/AODSIM/START38_V12-v1/0006/E8AAB57F-E3E0-DF11-920A-003048678F9C.root'
# #    '/store/data/Run2010B/Photon/AOD/Nov4ReReco_v1/0010/00369D9A-E7EB-DF11-B62B-0017A477083C.root',
# #    '/store/data/Run2010B/Photon/AOD/Nov4ReReco_v1/0010/2E2DE6F5-EBEB-DF11-A034-0017A4770824.root',
# #    '/store/data/Run2010B/Photon/AOD/Nov4ReReco_v1/0010/581F47F1-EBEB-DF11-8FD5-0017A4770438.root',
#     #'/store/data/Run2010B/Photon/AOD/Nov4ReReco_v1/0010/72F8EC8E-E7EB-DF11-998D-00237DA1CDBE.root',
#     #'/store/data/Run2010B/Photon/AOD/Nov4ReReco_v1/0010/A48792FB-EBEB-DF11-9A23-00237DA1AC2A.root',
#     #'/store/data/Run2010B/Photon/AOD/Nov4ReReco_v1/0007/043B336C-D7EA-DF11-86AE-0017A4770024.root',
#     #'/store/data/Run2010B/Photon/AOD/Nov4ReReco_v1/0007/047576AE-D1EA-DF11-810A-001F29C4C336.root',
#     #'/store/data/Run2010B/Photon/AOD/Nov4ReReco_v1/0007/049DBEDA-D5EA-DF11-A23E-0017A4770C1C.root',
#     #'/store/data/Run2010B/Photon/AOD/Nov4ReReco_v1/0007/06740B5E-D7EA-DF11-8B91-0017A4770C00.root',
#     #'/store/data/Run2010B/Photon/AOD/Nov4ReReco_v1/0007/08043297-D1EA-DF11-B8BC-00237DA096F8.root'
#     #'/store/data/Run2010B/Electron/AOD/Dec22ReReco_v1/0024/ECDC8A93-E00F-E011-94CC-0018F3D09680.root',
#     #'/store/data/Run2010B/Electron/AOD/Dec22ReReco_v1/0024/DCA9EEC8-E40F-E011-8F60-00261894397F.root',
#     #'/store/data/Run2010B/Electron/AOD/Dec22ReReco_v1/0024/CE6D2337-E10F-E011-88FD-003048679162.root'

    # Summer11 MC

    #/store/mc/Summer11/QCD_Pt-80to170_EMEnriched_TuneZ2_7TeV-pythia6/AODSIM/PU_S4_START42_V11-v1/0000/
    '/store/mc/Summer11/QCD_Pt-80to170_EMEnriched_TuneZ2_7TeV-pythia6/AODSIM/PU_S4_START42_V11-v1/0000/002B18AD-578F-E011-80FC-1CC1DE1CED1C.root',
    '/store/mc/Summer11/QCD_Pt-80to170_EMEnriched_TuneZ2_7TeV-pythia6/AODSIM/PU_S4_START42_V11-v1/0000/56206E33-538F-E011-BACA-0025B3E0216C.root',
    '/store/mc/Summer11/QCD_Pt-80to170_EMEnriched_TuneZ2_7TeV-pythia6/AODSIM/PU_S4_START42_V11-v1/0000/A6DA689A-558F-E011-B404-1CC1DE1CDD02.root',
    '/store/mc/Summer11/QCD_Pt-80to170_EMEnriched_TuneZ2_7TeV-pythia6/AODSIM/PU_S4_START42_V11-v1/0000/00CF8423-588F-E011-82FD-1CC1DE1D16D4.root',
    '/store/mc/Summer11/QCD_Pt-80to170_EMEnriched_TuneZ2_7TeV-pythia6/AODSIM/PU_S4_START42_V11-v1/0000/562A5CA5-578F-E011-BCE4-1CC1DE1CDDBC.root',
    '/store/mc/Summer11/QCD_Pt-80to170_EMEnriched_TuneZ2_7TeV-pythia6/AODSIM/PU_S4_START42_V11-v1/0000/A81B5EB2-5A8F-E011-9BCC-001CC4C10E02.root',
    '/store/mc/Summer11/QCD_Pt-80to170_EMEnriched_TuneZ2_7TeV-pythia6/AODSIM/PU_S4_START42_V11-v1/0000/00E20C98-578F-E011-8AAB-0017A477043C.root',
    '/store/mc/Summer11/QCD_Pt-80to170_EMEnriched_TuneZ2_7TeV-pythia6/AODSIM/PU_S4_START42_V11-v1/0000/5651AD4B-5C8F-E011-9F39-001F296A371E.root',
    '/store/mc/Summer11/QCD_Pt-80to170_EMEnriched_TuneZ2_7TeV-pythia6/AODSIM/PU_S4_START42_V11-v1/0000/A85B8026-848F-E011-A7ED-0017A477001C.root'
    
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
    triggerConditions = cms.vstring( "HLT_Photon75_CaloIdVL_v2" ),   # For MC... Summer11 random path
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
