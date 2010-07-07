import FWCore.ParameterSet.Config as cms

process = cms.Process("EwkDQM")
process.load("UserCode.DQMStandaloneEwkElectron.ewkElecStandaloneDQM_cfi")
process.load("Configuration.StandardSequences.Geometry_cff")

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("ewkElecStandaloneDQM-test.root"),
                                   closeFileFast = cms.untracked.bool(True) # set to true if lots of objects BUT NOT if multiple references to objects, e.g. a histogram and a canvas containing the histogram
                                   )
  

#process.load("runsDecDataJan29_cff")
process.load("runsDecDataMCJan29_cff")

process.maxEvents = cms.untracked.PSet(
#    input = cms.untracked.int32(-1)
    input = cms.untracked.int32(1000)
#    input = cms.untracked.int32(100000)
)

process.load("Configuration/StandardSequences/FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = cms.string("START3X_V26A::All") # the one for jan29 MC rereco

process.load("JetMETCorrections.Configuration.L2L3Corrections_900GeV_cff")
from JetMETCorrections.Type1MET.MetType1Corrections_cff import metJESCorAK5CaloJet
process.metMuonJESCorAK5 = metJESCorAK5CaloJet.clone()
process.metMuonJESCorAK5.inputUncorMetLabel = "corMetGlobalMuons"

process.metCorSequence = cms.Sequence(process.metMuonJESCorAK5)


process.MessageLogger = cms.Service("MessageLogger",
    destinations = cms.untracked.vstring('detailedInfoTest'),
    detailedInfoTest = cms.untracked.PSet(
            default = cms.untracked.PSet( limit = cms.untracked.int32(10) ),
            threshold = cms.untracked.string('INFO')
            #threshold = cms.untracked.string('ERROR')
    )
)

#process.p = cms.Path(process.ewkElecStandaloneDQM)
process.p = cms.Path(process.metCorSequence*process.ewkElecStandaloneDQM)


