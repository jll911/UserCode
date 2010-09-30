import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration/StandardSequences/FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = cms.string("GR_R_36X_V12A::All") # June 14th

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
#    $inputFileNames
#     '/store/user/lazaridis/SkimGoodCollV9/data7TeVSkim_830_0.root'
#'/store/data/Run2010A/EGMonitor/RECO/v4/000/139/100/6496E2E3-0585-DF11-86D8-0030486730C6.root'
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
                                    thresh = cms.untracked.double(0.25)
                                    )

process.primaryVertexFilter = cms.EDFilter("VertexSelector",
   src = cms.InputTag("offlinePrimaryVertices"),
   cut = cms.string("!isFake && ndof > 4 && abs(z) <= 15 && position.Rho <= 2"), # tracksSize() > 3 for the older cut
   filter = cms.bool(True),   # otherwise it won't filter the events, just produce an empty vertex collection.
)


process.demo = cms.EDAnalyzer('TrigTree', fillSC = cms.untracked.bool(False), IsData = cms.untracked.bool(True) )

process.TFileService = cms.Service("TFileService", 
                                   fileName = cms.string("outputFileName.root"),
                                   closeFileFast = cms.untracked.bool(True)
                                   )


process.p = cms.Path(
#    process.hltLevel1GTSeed* #MC->block
#    process.scrapingVeto*
#    process.hltPhysicsDeclared* #MC->block
    process.primaryVertexFilter*
    process.demo
    )
#process.p = cms.Path(process.demo)
