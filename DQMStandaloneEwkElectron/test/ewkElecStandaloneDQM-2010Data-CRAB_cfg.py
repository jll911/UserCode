import FWCore.ParameterSet.Config as cms

process = cms.Process("EwkDQM")
process.load("UserCode.DQMStandaloneEwkElectron.ewkElecStandaloneDQM_cfi")
process.load("Configuration.StandardSequences.Geometry_cff")


process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("ewkElecStandaloneDQM-2010Data.root"),
                                   closeFileFast = cms.untracked.bool(True) # set to true if lots of objects BUT NOT if multiple references to objects, e.g. a histogram and a canvas containing the histogram
                                   )
  

#process.load("runsDecDataMar3_cff")
#process.load("run132440Express_cff")
#process.load("runsGoodCollsv7_2apr10_cff")
process.source = cms.Source ("PoolSource",fileNames = cms.untracked.vstring())

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
#    input = cms.untracked.int32(1000)
#    input = cms.untracked.int32(100000)
)


#
# MET corrections
#

process.load("JetMETCorrections.Configuration.L2L3Corrections_900GeV_cff")
from JetMETCorrections.Type1MET.MetType1Corrections_cff import metJESCorAK5CaloJet
process.metMuonJESCorAK5 = metJESCorAK5CaloJet.clone()
process.metMuonJESCorAK5.inputUncorMetLabel = "corMetGlobalMuons"

process.metCorSequence = cms.Sequence(process.metMuonJESCorAK5)


#
# Global tag
#

process.load("Configuration/StandardSequences/FrontierConditions_GlobalTag_cff")
# latest 2010 data-taking global tag according to SWGuideFrontierConditions 29mar10
process.GlobalTag.globaltag = cms.string("GR10_P_V5::All") 
#process.GlobalTag.globaltag = cms.string("GR10_E_V4::All") # FOR EXPRESS STREAM!!



# THE FOLLOWING FROM DEC 19 RECIPES TWIKI PAGE

#process.load('L1TriggerConfig.L1GtConfigProducers.L1GtTriggerMaskTechTrigConfig_cff')
#process.load('HLTrigger/HLTfilters/hltLevel1GTSeed_cfi')
#process.L1T1=process.hltLevel1GTSeed.clone()
#process.L1T1.L1TechTriggerSeeding = cms.bool(True)
##process.L1T1.L1SeedsLogicalExpression = cms.string('0 AND (40 OR 41) AND NOT (36 OR 37 OR 38 OR 39)')
#process.L1T1.L1SeedsLogicalExpression = cms.string('(40 OR 41) AND NOT (36 OR 37 OR 38 OR 39)') # remove 0 to do the way Patrick does (I think) and add 2nd HLT path below.  also, no bit 0 in MC
##process.bscnobeamhalo = cms.Path(process.L1T1)



# this is for filtering on HLT path
#process.hltHighLevel = cms.EDFilter("HLTHighLevel",
#                                    TriggerResultsTag = cms.InputTag("TriggerResults","","HLT"),
#                                    #HLTPaths = cms.vstring('HLT_MinBiasBSC','HLT_PhysicsDeclared'), # provide list of HLT paths (or patterns) you want
#                                    HLTPaths = cms.vstring('HLT_MinBiasBSC'), # provide list of HLT paths (or patterns) you want
#                                    eventSetupPathsKey = cms.string(''), # not empty => use read paths from AlCaRecoTriggerBitsRcd via this key
#                                    andOr = cms.bool(True),             # how to deal with multiple triggers: True (OR) accept if ANY is true, False (AND) acc\ept if ALL are true
#                                    throw = cms.bool(True)    # throw exception on unknown path names
#                                    )

# output of runregparser, requiring ECAL HCAL L1 HLT PIX STRIP = GOOD
# comment out for MC running!
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('124030:2-124030:9999','124024:2-124024:83','124008:1-124008:1','123906:18-123906:28','123818:2-123818:42','123596:2-123596:9999','123591:71-123591:9999')
# from run registry!
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('132440:107-132440:9999')




# removal of beam scraping events
process.noscraping = cms.EDFilter("FilterOutScraping",
                                  applyfilter = cms.untracked.bool(True),
                                  #debugOn = cms.untracked.bool(True),
                                  debugOn = cms.untracked.bool(False),
                                  numtrack = cms.untracked.uint32(10),
                                  thresh = cms.untracked.double(0.25)
                                  )


#process.MessageLogger = cms.Service("MessageLogger",
#                                    destinations = cms.untracked.vstring('detailedInfo2010'),
#                                    detailedInfo2010 = cms.untracked.PSet(
#    default = cms.untracked.PSet( limit = cms.untracked.int32(10) ),
#    threshold = cms.untracked.string('INFO')
#    #threshold = cms.untracked.string('ERROR')
#    )
#                                    )

process.load("FWCore.MessageLogger.MessageLogger_cfi")

#process.ana = cms.EDAnalyzer("EventContentAnalyzer")
#process.p = cms.Path(process.ewkElecDQM+process.dqmSaver)

# path from official DQM and dec data
#process.p = cms.Path(process.L1T1+process.hltHighLevel+process.noscraping+process.ewkElecDQM+process.dqmSaver)
# path from standalone on MC
#process.p = cms.Path(process.metCorSequence*process.ewkElecStandaloneDQM)
# let their powers combine!
#process.p = cms.Path(process.L1T1+process.hltHighLevel+process.noscraping+process.metCorSequence+process.ewkElecStandaloneDQM)
process.p = cms.Path(process.noscraping+process.metCorSequence+process.ewkElecStandaloneDQM)

