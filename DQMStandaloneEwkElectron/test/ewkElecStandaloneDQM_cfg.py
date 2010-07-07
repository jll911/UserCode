import FWCore.ParameterSet.Config as cms

process = cms.Process("EwkDQM")
process.load("UserCode.DQMStandaloneEwkElectron.ewkElecStandaloneDQM_cfi")
process.load("Configuration.StandardSequences.Geometry_cff")


process.TFileService = cms.Service("TFileService",
#                                   fileName = cms.string('$outputFileName'),
                                  fileName = cms.string('DQMHistosMC.root'),
                               
                                   closeFileFast = cms.untracked.bool(True) # set to true if lots of objects BUT NOT if multiple references to objects, e.g. a histogram and a canvas containing the histogram
                                   )
  

#process.load("runsDecDataJan29_cff")
process.load("runsDecDataMCJan29_cff")
#process.source = cms.Source("PoolSource",
#                           fileNames = cms.untracked.vstring( $inputFileNames )
#)


process.maxEvents = cms.untracked.PSet(
#    input = cms.untracked.int32(-1)
    input = cms.untracked.int32(1000)
#    input = cms.untracked.int32(100000)
)

process.load("JetMETCorrections.Configuration.L2L3Corrections_Summer09_7TeV_ReReco332_cff")
## process.load("JetMETCorrections.Configuration.L2L3Corrections_900GeV_cff")
from JetMETCorrections.Type1MET.MetType1Corrections_cff import metJESCorAK5CaloJet
process.metMuonJESCorAK5 = metJESCorAK5CaloJet.clone()
process.metMuonJESCorAK5.inputUncorMetLabel = "corMetGlobalMuons"
process.metCorSequence = cms.Sequence(process.metMuonJESCorAK5)



process.MessageLogger = cms.Service("MessageLogger",
     destinations = cms.untracked.vstring('detailedInfo'),
     detailedInfo = cms.untracked.PSet(
         default = cms.untracked.PSet( limit = cms.untracked.int32(1) ),
         threshold = cms.untracked.string('INFO')
         #threshold = cms.untracked.string('ERROR')
         )
                                     )
process.p = cms.Path(process.metCorSequence*process.ewkElecStandaloneDQM)
#process.p = cms.Path(process.ewkElecStandaloneDQM)

