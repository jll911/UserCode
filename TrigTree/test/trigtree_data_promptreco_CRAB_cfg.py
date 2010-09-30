import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")

#process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1000) )

process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration/StandardSequences/FrontierConditions_GlobalTag_cff")
#process.GlobalTag.globaltag = cms.string("START3X_V26A::All") # the one for jan29 MC rereco
#process.GlobalTag.globaltag = cms.string("GR10_P_V5::All") # prompt reco above 133332
process.GlobalTag.globaltag = cms.string("GR_R_36X_V12::All") # 

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
#    $inputFileNames
#    'rfio:///castor/cern.ch/user/c/clazarid/V11/Slimmer_MBMay27thReReco_v1_V11.root'
#    'rfio:///castor/cern.ch/user/c/clazarid/V11/SampleMBMay27thReReco_v1_V11_Run133885.root'
    '/store/data/Run2010A/EG/RECO/May27thReReco_v1/0174/B4EB92F0-956A-DF11-8431-003048F0E186.root'
    #'/store/user/lazaridis/SkimGoodCollV9/data7TeVSkim_830_0.root'
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

process.load("RecoEgamma.EgammaTools.correctedElectronsProducer_cfi")

# select events with two electrons
process.diElectronFilter = cms.EDFilter("PtMinGsfElectronCountFilter",
                                        src = cms.InputTag("gsfElectrons::Demo"),
                                        ptMin = cms.double(0.0),
                                        minNumber = cms.uint32(2)
                                        )


process.ntuplizer = cms.EDAnalyzer('TrigTree',
                              fillSC = cms.untracked.bool(False),
                              IsData = cms.untracked.bool(True),
                              ElecTag = cms.untracked.InputTag("gsfElectrons::Demo")
                              )

process.TFileService = cms.Service("TFileService", 
                                   #fileName = cms.string("$outputFileName"),
                                   #fileName = cms.string("test-data.root"),
                                   fileName = cms.string("trigtree-2010Data.root"),
                                   closeFileFast = cms.untracked.bool(True)
                                   )


process.p = cms.Path(
    process.hltLevel1GTSeed* #MC->block
    process.scrapingVeto*
    process.hltPhysicsDeclared* #MC->block
    process.primaryVertexFilter*
    process.gsfElectrons*
    process.diElectronFilter*
    process.ntuplizer
    )
#process.p = cms.Path(process.demo)
