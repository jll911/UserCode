import FWCore.ParameterSet.Config as cms

process = cms.Process("EwkDQM")
process.load("UserCode.DQMStandaloneEwkElectron.ewkElecStandaloneDQM_cfi")
process.load("Configuration.StandardSequences.Geometry_cff")

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("ewkElecStandaloneDQM-test.root"),
                                   closeFileFast = cms.untracked.bool(True) # set to true if lots of objects BUT NOT if multiple references to objects, e.g. a histogram and a canvas containing the histogram
                                   )
  

#process.load("runsDecDataJan29_cff")
#process.load("runsDecDataMCJan29_cff")

process.source = cms.Source ("PoolSource",
                             fileNames = cms.untracked.vstring(
#    '/store/data/Commissioning10/MinimumBias/RAW-RECO/v9/000/133/874/A6D842AC-D44F-DF11-9907-00E0817917AB.root'#,
    '/store/data/Commissioning10/MinimumBias/RAW-RECO/v9/000/133/877/A8FA282A-8350-DF11-ABD3-003048D45F9C.root'#,
##    '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/133/450/96B6F1AA-6D4A-DF11-8622-003048BAA5A8.root',
##    '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/959/F800D72C-9145-DF11-8DFA-003048670A06.root'

#    '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0180/84AD316D-284E-DF11-A5E5-002618943940.root'#, #132959
#    '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0180/22417EE1-264E-DF11-9592-00261894389C.root' # 133450
    ),
                             eventsToProcess = cms.untracked.VEventRange(cms.EventRange( '133877:28405693', '133877:28405693')#,
#    cms.EventRange( '133874:21466935', '133874:21466935')#,
#                                                                         cms.EventRange( '133450:2792439', '133450:2792439')#,
#                                                                         cms.EventRange( '132959:346686', '132959:346686')
                                                                         )
                             )

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
#    input = cms.untracked.int32(1000)
#    input = cms.untracked.int32(100000)
)

process.load("Configuration/StandardSequences/FrontierConditions_GlobalTag_cff")
#process.GlobalTag.globaltag = cms.string("START3X_V26A::All") # the one for jan29 MC rereco
process.GlobalTag.globaltag = cms.string("GR10_P_V5::All") 

process.load("JetMETCorrections.Configuration.L2L3Corrections_900GeV_cff")
from JetMETCorrections.Type1MET.MetType1Corrections_cff import metJESCorAK5CaloJet
process.metMuonJESCorAK5 = metJESCorAK5CaloJet.clone()
process.metMuonJESCorAK5.inputUncorMetLabel = "corMetGlobalMuons"

process.metCorSequence = cms.Sequence(process.metMuonJESCorAK5)


#process.MessageLogger = cms.Service("MessageLogger",
#    destinations = cms.untracked.vstring('detailedInfoTest'),
#    detailedInfoTest = cms.untracked.PSet(
#            default = cms.untracked.PSet( limit = cms.untracked.int32(10) ),
#            threshold = cms.untracked.string('INFO')
#            #threshold = cms.untracked.string('ERROR')
#    )
#)

#process.p = cms.Path(process.ewkElecStandaloneDQM)
process.p = cms.Path(process.metCorSequence*process.ewkElecStandaloneDQM)


