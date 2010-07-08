import FWCore.ParameterSet.Config as cms

process = cms.Process("EwkDQM")
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("UserCode.DQMStandaloneEwkElectron.ewkElecStandaloneDQM_cfi")
#process.ewkElecStandaloneDQM.IsData = True

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("ewkElecStandaloneDQM-test.root"),
                                   closeFileFast = cms.untracked.bool(True) # set to true if lots of objects BUT NOT if multiple references to objects, e.g. a histogram and a canvas containing the histogram
                                   )
  
process.maxEvents = cms.untracked.PSet(
#    input = cms.untracked.int32(-1)
#    input = cms.untracked.int32(1000)
    input = cms.untracked.int32(10000)
#    input = cms.untracked.int32(15)
#    input = cms.untracked.int32(100000)
)


#process.load("runsDecDataJan29_cff")
#process.load("UserCode.DQMStandaloneEwkElectron.runsDecDataMCJan29_cff")
#process.readFiles = cms.untracked.vstring()
#process.secFiles = cms.untracked.vstring()
process.source = cms.Source ("PoolSource",
                             fileNames = cms.untracked.vstring(
    '/store/data/Commissioning10/MinimumBias/RAW-RECO/v9/000/135/802/60FDD869-4B65-DF11-8E32-0025B3E06382.root',
    '/store/data/Commissioning10/MinimumBias/RAW-RECO/v9/000/135/799/169919DA-4865-DF11-B4D8-00E081791757.root',
    '/store/data/Commissioning10/MinimumBias/RAW-RECO/v9/000/135/797/7000870F-4965-DF11-BD9B-003048D4776A.root',
    '/store/data/Commissioning10/MinimumBias/RAW-RECO/v9/000/135/794/1AAFC699-4965-DF11-A157-0025B3E066A0.root',
    '/store/data/Commissioning10/MinimumBias/RAW-RECO/v9/000/135/793/6C11FCB7-4B65-DF11-8CD0-00E08178C133.root',
    '/store/data/Commissioning10/MinimumBias/RAW-RECO/v9/000/135/787/C4CC42ED-4865-DF11-8385-002481E150FE.root',
    '/store/data/Commissioning10/MinimumBias/RAW-RECO/v9/000/135/777/DA4FABF0-4865-DF11-B0CC-00E081B08D11.root',
    '/store/data/Commissioning10/MinimumBias/RAW-RECO/v9/000/135/771/B6C6A4DB-4865-DF11-BFDC-003048D47A16.root',
    '/store/data/Commissioning10/MinimumBias/RAW-RECO/v9/000/135/735/FAB17A5D-4465-DF11-8DBF-00E08178C031.root',
    '/store/data/Commissioning10/MinimumBias/RAW-RECO/v9/000/135/735/F8025043-4465-DF11-BC1D-00E0817918B9.root',
    '/store/data/Commissioning10/MinimumBias/RAW-RECO/v9/000/135/735/F6E4945A-4465-DF11-8C95-003048D460B6.root',
    '/store/data/Commissioning10/MinimumBias/RAW-RECO/v9/000/135/735/EEA12B69-4465-DF11-9F58-003048D45FA6.root',
    '/store/data/Commissioning10/MinimumBias/RAW-RECO/v9/000/135/735/EE0E5C3F-4465-DF11-839B-00E081791761.root',
    '/store/data/Commissioning10/MinimumBias/RAW-RECO/v9/000/135/735/E4BDEF5B-4465-DF11-BDFB-003048D47766.root',
    '/store/data/Commissioning10/MinimumBias/RAW-RECO/v9/000/135/735/E2E6D96D-4465-DF11-AF04-00E08178C075.root',
    '/store/data/Commissioning10/MinimumBias/RAW-RECO/v9/000/135/735/DCE0D46A-4465-DF11-87FA-00E0817918D3.root',
    '/store/data/Commissioning10/MinimumBias/RAW-RECO/v9/000/135/735/D87BEB58-4465-DF11-8DF4-003048D476C4.root',
    '/store/data/Commissioning10/MinimumBias/RAW-RECO/v9/000/135/735/D8481062-4465-DF11-9958-0025B3E0642C.root',
    '/store/data/Commissioning10/MinimumBias/RAW-RECO/v9/000/135/735/D65D2AF5-8964-DF11-AE5F-002481E14E62.root',
    '/store/data/Commissioning10/MinimumBias/RAW-RECO/v9/000/135/735/D4CDB62B-8664-DF11-A7FE-001A6478946C.root',
    '/store/data/Commissioning10/MinimumBias/RAW-RECO/v9/000/135/735/CAA18048-4865-DF11-9A7A-003048D477C0.root',
    '/store/data/Commissioning10/MinimumBias/RAW-RECO/v9/000/135/735/B2E8DD3D-4465-DF11-99E1-003048D45FD6.root',
    '/store/data/Commissioning10/MinimumBias/RAW-RECO/v9/000/135/735/B25CD365-4465-DF11-BF32-003048D45FEE.root',
    '/store/data/Commissioning10/MinimumBias/RAW-RECO/v9/000/135/735/A23F0467-4465-DF11-A90B-003048D46076.root',
    '/store/data/Commissioning10/MinimumBias/RAW-RECO/v9/000/135/735/96E4FA6D-4465-DF11-B995-00E081B08D11.root',
    '/store/data/Commissioning10/MinimumBias/RAW-RECO/v9/000/135/735/8AB777C4-8764-DF11-846D-003048D4610E.root',
    '/store/data/Commissioning10/MinimumBias/RAW-RECO/v9/000/135/735/8A787863-4465-DF11-8304-003048D45FD6.root',
    '/store/data/Commissioning10/MinimumBias/RAW-RECO/v9/000/135/735/6A19F85D-4465-DF11-9CC6-00E081791859.root',
    '/store/data/Commissioning10/MinimumBias/RAW-RECO/v9/000/135/735/5E05EB79-8D64-DF11-9532-00E081791769.root',
    '/store/data/Commissioning10/MinimumBias/RAW-RECO/v9/000/135/735/4C902D3A-4865-DF11-8774-003048D477A4.root',
    '/store/data/Commissioning10/MinimumBias/RAW-RECO/v9/000/135/735/48BBFB52-4465-DF11-923A-003048D47A40.root',
    '/store/data/Commissioning10/MinimumBias/RAW-RECO/v9/000/135/735/44FF26F3-8464-DF11-9AA4-002481E14E30.root',
    '/store/data/Commissioning10/MinimumBias/RAW-RECO/v9/000/135/735/44706B64-4465-DF11-B3F2-002481E15148.root',
    '/store/data/Commissioning10/MinimumBias/RAW-RECO/v9/000/135/735/362F3A49-4465-DF11-8810-00E081791787.root',
    '/store/data/Commissioning10/MinimumBias/RAW-RECO/v9/000/135/735/34FD0F61-4465-DF11-8350-003048D4776A.root',
    '/store/data/Commissioning10/MinimumBias/RAW-RECO/v9/000/135/735/302DD371-4465-DF11-994F-0025B3E06382.root',
    '/store/data/Commissioning10/MinimumBias/RAW-RECO/v9/000/135/735/2AEC0A5E-4465-DF11-94D1-003048D45F54.root',
    '/store/data/Commissioning10/MinimumBias/RAW-RECO/v9/000/135/735/243BD88B-8864-DF11-BB8A-003048D45FB8.root',
    '/store/data/Commissioning10/MinimumBias/RAW-RECO/v9/000/135/735/20A23D2C-8464-DF11-8AE4-00E081791767.root',
    '/store/data/Commissioning10/MinimumBias/RAW-RECO/v9/000/135/735/1E344362-4465-DF11-80E5-00E08177F14F.root',
    '/store/data/Commissioning10/MinimumBias/RAW-RECO/v9/000/135/735/1C29FD55-4465-DF11-A299-00E08178C133.root',
    '/store/data/Commissioning10/MinimumBias/RAW-RECO/v9/000/135/735/1A05356C-4465-DF11-88CF-003048D45FBE.root',
    '/store/data/Commissioning10/MinimumBias/RAW-RECO/v9/000/135/735/1643D663-4465-DF11-AA9C-001A64789DA8.root',
    '/store/data/Commissioning10/MinimumBias/RAW-RECO/v9/000/135/735/061C335B-8C64-DF11-82B5-001A64789DE4.root',
    '/store/data/Commissioning10/MinimumBias/RAW-RECO/v9/000/135/735/00E79FF2-8B64-DF11-93B1-00304866C368.root',
    '/store/data/Commissioning10/MinimumBias/RAW-RECO/v9/000/135/726/B2845DA7-4B65-DF11-9EDB-003048D47A8A.root',
    '/store/data/Commissioning10/MinimumBias/RAW-RECO/v9/000/135/722/C6FF10BE-4265-DF11-9951-003048D462C2.root',
    '/store/data/Commissioning10/MinimumBias/RAW-RECO/v9/000/135/719/2881FF3C-4265-DF11-86A5-003048D46030.root',
    '/store/data/Commissioning10/MinimumBias/RAW-RECO/v9/000/135/716/CEDA8048-4265-DF11-B8F7-003048D4776A.root',
    '/store/data/Commissioning10/MinimumBias/RAW-RECO/v9/000/135/712/1210DA54-6E64-DF11-B840-00E081791783.root',
    '/store/data/Commissioning10/MinimumBias/RAW-RECO/v9/000/135/709/3E51DE4A-4265-DF11-9FDC-0025B3E06516.root',
    '/store/data/Commissioning10/MinimumBias/RAW-RECO/v9/000/135/705/54040245-4265-DF11-B63E-003048D460C4.root',
    '/store/data/Commissioning10/MinimumBias/RAW-RECO/v9/000/135/700/3E86ED37-6C64-DF11-AEBC-001A64789CF8.root',
    '/store/data/Commissioning10/MinimumBias/RAW-RECO/v9/000/135/698/CE38FC4B-4265-DF11-8C5F-0025B3E065FE.root',
    '/store/data/Commissioning10/MinimumBias/RAW-RECO/v9/000/135/677/AA6A0913-7064-DF11-9B88-003048D47A6A.root',
    '/store/data/Commissioning10/MinimumBias/RAW-RECO/v9/000/135/675/0C65528C-6A64-DF11-8250-00E0817918A7.root',
    '/store/data/Commissioning10/MinimumBias/RAW-RECO/v9/000/135/664/808E74A3-4265-DF11-96C0-003048635E06.root',
    '/store/data/Commissioning10/MinimumBias/RAW-RECO/v9/000/135/659/6ADEF5DD-4265-DF11-B0F7-003048D46008.root'
    )
                             )

process.load("Configuration/StandardSequences/FrontierConditions_GlobalTag_cff")
#process.GlobalTag.globaltag = cms.string("START3X_V26A::All") # the one for jan29 MC rereco
process.GlobalTag.globaltag = cms.string("GR10_P_V5::All") # tag for promptreco for 35X?

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


