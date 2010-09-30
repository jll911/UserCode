import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")

#process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
#process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1000) )
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10) )

process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration/StandardSequences/FrontierConditions_GlobalTag_cff")
#process.GlobalTag.globaltag = cms.string("START3X_V26A::All") # the one for jan29 MC rereco
process.GlobalTag.globaltag = cms.string("START36_V10A::All") # 36X MC
#process.GlobalTag.globaltag = cms.string("GR10_P_V5::All") # data above 133332

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
#    $inputFileNames
#     'file:run135149_1evt.root'
#        '/store/mc/Spring10/MinBias/GEN-SIM-RECO/START3X_V25B_356ReReco-v1/0007/FE90A396-233C-DF11-8106-002618943898.root',
#       '/store/mc/Spring10/MinBias/GEN-SIM-RAW/START3X_V25B-v1/0104/FECFDECD-9739-DF11-A00E-001A92971AAA.root', #/MinBias/Spring10-START3X_V25B-v1/GEN-SIM-RAW
#        '/store/mc/Spring10/MinBias/GEN-SIM-RECO/START3X_V26A_357ReReco-v3/0188/FE8C8E24-7C50-DF11-8490-0030486790A6.root'#, #/MinBias/Spring10-START3X_V26A_357ReReco-v3/GEN-SIM-RECO 
    '/store/mc/Summer10/Zee/AODSIM/START36_V9_S09-v1/0035/8E61CE3B-CE79-DF11-84EE-003048D4610A.root' 
    )
)
# require physics declared
process.load('HLTrigger.special.hltPhysicsDeclared_cfi')
process.hltPhysicsDeclared.L1GtReadoutRecordTag = 'gtDigis'

# require scraping filter
process.scrapingVeto = cms.EDFilter("FilterOutScraping",
                                    applyfilter = cms.untracked.bool(True),
                                    debugOn = cms.untracked.bool(False),
                                    numtrack = cms.untracked.uint32(10),
                                    thresh = cms.untracked.double(0.2)
                                    )


# configure HLT
process.load('L1TriggerConfig.L1GtConfigProducers.L1GtTriggerMaskTechTrigConfig_cff')
process.load('HLTrigger/HLTfilters/hltLevel1GTSeed_cfi')
process.hltLevel1GTSeed.L1TechTriggerSeeding = cms.bool(True)
process.hltLevel1GTSeed.L1SeedsLogicalExpression = cms.string('0 AND (40 OR 41) AND NOT (36 OR 37 OR 38 OR 39) AND NOT ((42 AND NOT 43) OR (43 AND NOT 42))')


process.primaryVertexFilter = cms.EDFilter("GoodVertexFilter",
                                           vertexCollection = cms.InputTag('offlinePrimaryVertices'),
                                           minimumNDOF = cms.uint32(4) ,
                                           maxAbsZ = cms.double(15), 
                                           maxd0 = cms.double(2) 
                                           )

# select events with two electrons
process.diElectronFilter = cms.EDFilter("PtMinGsfElectronCountFilter",
                                        #src = cms.InputTag("gsfElectrons::Demo"),
                                        src = cms.InputTag("gsfElectrons"),
                                        ptMin = cms.double(0.0),
                                        minNumber = cms.uint32(2)
                                        )

process.ntuplizer = cms.EDAnalyzer('TrigTree',
                                   fillSC = cms.untracked.bool(False),
                                   IsData = cms.untracked.bool(False),
                                   TrigTag = cms.untracked.InputTag("TriggerResults::REDIGI36X"),
                                   TrigEventTag = cms.untracked.InputTag("hltTriggerSummaryAOD::REDIGI36X")
                                   )

process.TFileService = cms.Service("TFileService", 
                                   #fileName = cms.string("$outputFileName"),
                                   fileName = cms.string("test-MC.root"),
                                   closeFileFast = cms.untracked.bool(True)
                                   )


process.p = cms.Path(
   # process.hltLevel1GTSeed* #MC->block
    process.scrapingVeto*
   # process.hltPhysicsDeclared* #MC->block
    process.primaryVertexFilter*
    process.diElectronFilter*
    process.ntuplizer
    )
#process.p = cms.Path(process.demo)
