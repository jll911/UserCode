import FWCore.ParameterSet.Config as cms

photonsfromwenu = cms.EDAnalyzer(
    'PhotonsFromWenu',
    electronCollection     = cms.InputTag("gsfElectrons"),
    photonCollection       = cms.InputTag("photons"),
    jetCollection          = cms.InputTag("ak5CaloJets"),
    pfjetCollection        = cms.InputTag("ak5PFJetsL2L3"),
    trackCollection        = cms.InputTag("generalTracks"),
    genjetCollection       = cms.InputTag("ak5GenJets"),
    metCollection          = cms.InputTag("pfMet"),
    vertexCollection       = cms.InputTag("offlinePrimaryVertices"),
    # Next two requirements must be met for histograms & tree to be filled
    minTransMass           = cms.double( 15.0 ), # Min transverse mass using electron & Met
    minMetEt               = cms.double( 10.0 ),
    minJetPt               = cms.double( -1.0 ), # -1.0 = require no jet presence
    maxJetAbsEta           = cms.double( 2.4 ), # only consider jets with |eta| < this
    requireElecPassCuts    = cms.bool( False ), # False: Highest pt electron will be used; True: electron must pass "golden" cuts
    usePhotonMatchedToElec = cms.bool( True ), # False: Photon with highest Pt will be used; True: Photon within dR of electron will be used
    maxDeltaR              = cms.double( 0.1 ), # Between electron and photon
    minDeltaRPhotonJet     = cms.double( 0.5 ), # Jets will not be counted unless they are at least this far from lead photon
    hltResultsCollection   = cms.InputTag( "TriggerResults", "", "HLT" ),
    HltPaths               = cms.vstring("HLT_Photon30_Cleaned_L1R"),  # Event must pass one of these
    pfjetIdLoose           = cms.PSet()  # Null parameter set
    )
