import FWCore.ParameterSet.Config as cms
import sys


process = cms.Process("SKIM")

process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
#process.GlobalTag.globaltag = 'GR_R_42_V19::All'
process.GlobalTag.globaltag = 'GR_R_42_V21::All' # they say to use the rereco global tags for analysis of prompt data


process.maxEvents = cms.untracked.PSet(
        input = cms.untracked.int32(500)
        )

process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring(
#    '/store/data/Run2011A/TauPlusX/AOD/PromptReco-v4/000/165/099/88FDF12C-D17F-E011-8317-003048F1C832.root',
    '/store/data/Run2011A/Commissioning/RAW/v2/000/161/351/CC6A8150-3756-E011-9F78-0030486733D8.root'
    
    )
                            )


process.triggerSelection = cms.EDFilter( "TriggerResultsFilter",
                                         triggerConditions = cms.vstring(
    'HLT_L1SingleEG12_v3' ),
#    'HLT_L1SingleEG12_v1' ),
                                         hltResults = cms.InputTag( "TriggerResults", "", "HLT" ),
                                         l1tResults = cms.InputTag( "" ),
                                         l1tIgnoreMask = cms.bool( False ),
                                         l1techIgnorePrescales = cms.bool( False ),
                                         daqPartitions = cms.uint32( 1 ),
                                         throw = cms.bool( True )
                                         )



process.skim = cms.Path(process.triggerSelection)



process.out = cms.OutputModule("PoolOutputModule",
                               fileName = cms.untracked.string('skim.root'),
                               outputCommands = cms.untracked.vstring('keep *_*_*_*'#,
                                                #                      'drop *_*_*_SKIM'
                                                                      ),
                               SelectEvents = cms.untracked.PSet(
    SelectEvents=cms.vstring("skim")
    )
                               )

process.e = cms.EndPath(process.out)


process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True)
    
    )




                                                                                         
