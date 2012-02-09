# -*- coding: utf-8 -*-

import sys
import os
#import dbs_discovery
import FWCore.ParameterSet.Config as cms

process = cms.Process("RCTVAL")

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1000
process.load("DQMServices.Core.DQM_cfg")

process.load("Configuration.StandardSequences.Services_cff")
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.MagneticField_38T_cff")
process.load("Configuration.StandardSequences.Reconstruction_cff")

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("RctEmTree-Rates-28Nov11.root"),
                                   closeFileFast = cms.untracked.bool(True)
                                   )



# electron selector

process.load("ElectroWeakAnalysis.WENu.simpleEleIdSequence_cff") 
#process.ElectronIDs = cms.Sequence(process.simpleEleIdSequence)
process.simpleEleId70relIso.src = cms.InputTag("selectedElectronsPlateau")
#process.simpleEleId95relIso.src = cms.InputTag("selectedElectronsPlateau")


process.load("Configuration.EventContent.EventContent_cff")

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
#'/store/data/Run2010B/Electron/RAW-RECO/v2/000/146/644/544F6D08-12CB-DF11-9059-003048678FF6.root'
#'/store/data/Run2010B/MinimumBias/RAW-RECO/v2/000/146/944/36A3519B-B3CC-DF11-B099-002618943877.root'
#                                '/store/data/Run2010B/EGMonitor/RAW/v1/000/147/826/6A32EF3E-41D6-DF11-966F-001617DBD5AC.root'
#       $inputFileNames
#                               '/store/data/Run2011A/SingleElectron/RAW-RECO/WElectron-May10ReReco-v1/0000/EEC6F492-A67B-E011-A716-0025901D40CA.root'
    '/store/data/Run2011A/Commissioning/RAW/v2/000/161/351/CC6A8150-3756-E011-9F78-0030486733D8.root'

#                                '/store/user/swanson/LowECandsRaw/pickevents_156_1_l3O.root'
           #                     '/store/data/Run2010A/EG/RAW/Aug13ReHLTReReco_PreProd_v3/0002/3A6F5B01-F6A9-DF11-A4F9-0030488A0ACE.root'
#                                '/store/data/Run2010B/Electron/RAW-RECO/WZEG-Dec22Skim_v2/0047/2EC213E1-9E12-E011-8F6D-003048678B26.root'
    ),
#                             noEventSort = cms.untracked.bool(False)
)

#process.load("Validation.L1Trigger.Cert_160404_179431_7TeV_PromptReco_Collisions11_CMSSWConfig_cff")
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange(process.myLumisToProcess.lumisToProcess)
 
process.l1RCTParametersTest = cms.EDAnalyzer("L1RCTParametersTester")
process.write = cms.EDAnalyzer("L1CaloInputScalesGenerator")

process.l1RCTScaleTest = cms.EDAnalyzer("L1ScalesTester")
process.l1RCTChannelMaskTest = cms.EDAnalyzer("L1RCTChannelMaskTester")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
#process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1) )
#process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1000) )
#process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100) )
#process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(5000) )

process.load("DQMServices.Core.DQM_cfg")
process.load("DQMServices.Components.DQMEnvironment_cfi")


process.load("Validation.L1Trigger.L1TRCToffline_DataReEmul_cff")
#process.load("Validation.L1Trigger.L1TRCToffline_HE3_cff")

process.highestElectron = cms.EDFilter("LargestPtCandViewSelector",
                           src = cms.InputTag("gsfElectrons"),
                           maxNumber = cms.uint32(2)
                         )
# process.selectedPhotons = cms.EDFilter("PhotonSelector",
#    src = cms.InputTag("photons"),
#    cut = cms.string('pt>1.0&&hadronicOverEm()<0.05&&ecalRecHitSumEtConeDR03()<3&&maxEnergyXtal()/e3x3()<0.99&&r2x5>.9'),
#    filter = cms.bool(True)
# )                                       

# process.selectedElectrons = cms.EDFilter("GsfElectronSelector",
#    src = cms.InputTag("gsfElectrons"),
#    cut = cms.string('pt>1'),
#    filter = cms.bool(True)
# )    

# select a subset of the GsfElectron collection based on the quality stored in a ValueMap
process.electronsWp70 = cms.EDProducer("BtagGsfElectronSelector",
                                       input     = cms.InputTag( "selectedElectronsPlateau" ),
                                       #input     = cms.InputTag( 'gsfElectrons' ),
                                       selection = cms.InputTag('simpleEleId70relIso'),
                                       cut       = cms.double(6.5) ### 7 == passing all conv rej, iso, id cuts
                                       )

process.electronsWp95 = cms.EDProducer("BtagGsfElectronSelector",
                                       input     = cms.InputTag( "selectedElectronsPlateau" ),
                                       #input     = cms.InputTag( "gsfElectrons" ),
                                       selection = cms.InputTag('simpleEleId95relIso'),
                                       cut       = cms.double(6.5) ### 7 == passing all convrej, iso, id cuts
                                       )

process.selectedElectronsPlateau = cms.EDFilter("GsfElectronSelector",
   src = cms.InputTag("gsfElectrons"),
   cut = cms.string('pt>10&&pt<25'),
   filter = cms.bool(True)
)    

## EG2
process.EG2 = cms.EDAnalyzer('RctValidation',
                ecalTPGs = cms.InputTag("ecalDigis:EcalTriggerPrimitives"),
                rctEGamma = cms.InputTag('gctDigis'),
                gctEGamma = cms.VInputTag(cms.InputTag('gctDigis','isoEm'),
                                          cms.InputTag('gctDigis','nonIsoEm')
                ),                          
                genEGamma = cms.InputTag("highestPhoton"),
                directory = cms.string("L1T/RCTPhotons/EG2"),
                maxEt = cms.untracked.double(40),
                binsEt = cms.untracked.int32(40),
                gammaThreshold = cms.untracked.double(2.)              
)

process.SC2 = cms.EDAnalyzer('RctValidation',
                ecalTPGs = cms.InputTag("ecalDigis:EcalTriggerPrimitives"),
                rctEGamma = cms.InputTag('gctDigis'),
                gctEGamma = cms.VInputTag(cms.InputTag('gctDigis','isoEm'),
                                          cms.InputTag('gctDigis','nonIsoEm')
                ),                          
                genEGamma = cms.InputTag("selectSC"),
                directory = cms.string("L1T/RCTPhotons/SC2"),
                maxEt = cms.untracked.double(40),
                binsEt = cms.untracked.int32(40),
                gammaThreshold = cms.untracked.double(2.)              
)

process.gsf2 = cms.EDAnalyzer('RctValidation',
                ecalTPGs = cms.InputTag("ecalDigis:EcalTriggerPrimitives"),
                hcalTPGs = cms.InputTag("hcalDigis"),
                rctEGamma = cms.InputTag('gctDigis'),
                gctEGamma = cms.VInputTag(cms.InputTag('gctDigis','isoEm'),
                                          cms.InputTag('gctDigis','nonIsoEm')
                ),                          
                genEGamma = cms.InputTag("gsfElectrons"),
                directory = cms.string("L1T/RCTPhotons/GSF2"),
                maxEt = cms.untracked.double(40),
                binsEt = cms.untracked.int32(40),
                gammaThreshold = cms.untracked.double(2.)              
)

## EG5
process.EG5 = cms.EDAnalyzer('RctValidation',
                ecalTPGs = cms.InputTag("ecalDigis:EcalTriggerPrimitives"),
                rctEGamma = cms.InputTag('gctDigis'),
                gctEGamma = cms.VInputTag(cms.InputTag('gctDigis','isoEm'),
                                          cms.InputTag('gctDigis','nonIsoEm')
                ),                          
                genEGamma = cms.InputTag("highestPhoton"),
                directory = cms.string("L1T/RCTPhotons/EG5"),
                maxEt = cms.untracked.double(40),
                binsEt = cms.untracked.int32(40),
                gammaThreshold = cms.untracked.double(5.)              
)

## EG8
process.EG8 = cms.EDAnalyzer('RctValidation',
                ecalTPGs = cms.InputTag("ecalDigis:EcalTriggerPrimitives"),
                rctEGamma = cms.InputTag('gctDigis'),
                gctEGamma = cms.VInputTag(cms.InputTag('gctDigis','isoEm'),
                                          cms.InputTag('gctDigis','nonIsoEm')
                ),                          
                genEGamma = cms.InputTag("highestPhoton"),
                directory = cms.string("L1T/RCTPhotons/EG8"),
                maxEt = cms.untracked.double(40),
                binsEt = cms.untracked.int32(40),
                gammaThreshold = cms.untracked.double(8.)              
)



#Reconfigure Environment and saver
process.DQM.debug = cms.untracked.bool(True)
process.dqmSaver.saveByRun = cms.untracked.int32(-1)
process.dqmSaver.saveAtJobEnd = cms.untracked.bool(True)
process.dqmSaver.workflow = cms.untracked.string('/A/N/C')
process.dqmSaver.forceRunNumber = cms.untracked.int32(555)
process.DQMStore.verbose=0


process.gsf8 = cms.EDAnalyzer('RctValidation',
                ecalTPGs = cms.InputTag("ecalDigis:EcalTriggerPrimitives"),
                hcalTPGs = cms.InputTag("hcalDigis"),
                rctEGamma = cms.InputTag('gctDigis'),
                gctEGamma = cms.VInputTag(cms.InputTag('gctDigis','isoEm'),
                                          cms.InputTag('gctDigis','nonIsoEm')
                ),                          
                genEGamma = cms.InputTag("electronsWp70"),
#                genEGamma = cms.InputTag("electronsWp95"),
#                genEGamma = cms.InputTag("selectedElectronsPlateau"),
#                genEGamma = cms.InputTag("selectedElectrons"),
                directory = cms.string("L1T/ALL/GSF8base"),
                maxEt = cms.untracked.double(60),
                binsEt = cms.untracked.int32(60),
                gammaThreshold = cms.untracked.double(8.),
                #outputFileName = cms.untracked.string('$outputFileName')
                #outputFileName = cms.untracked.string('outputFileName.root')
                outputFileName = cms.untracked.string('output-RctVal-MINE-Rates-28Nov11.root')
)

process.gsf12          = process.gsf8.clone()
#process.gsf12.genEGamma = cms.InputTag("selectedElectrons")
process.gsf12.directory = cms.string("L1T/ALL/GSF12base")
process.gsf12.gammaThreshold = cms.untracked.double(12.)

process.gsf15          = process.gsf12.clone()
#process.gsf15.genEGamma = cms.InputTag("selectedElectrons")
process.gsf15.directory = cms.string("L1T/ALL/GSF15base")
process.gsf15.gammaThreshold = cms.untracked.double(15.)

process.gsf18          = process.gsf12.clone()
#process.gsf18.genEGamma = cms.InputTag("selectedElectrons")
process.gsf18.directory = cms.string("L1T/ALL/GSF18base")
process.gsf18.gammaThreshold = cms.untracked.double(18.)

process.gsf20          = process.gsf8.clone()
#process.gsf20.genEGamma = cms.InputTag("selectedElectrons")
process.gsf20.directory = cms.string("L1T/ALL/GSF20base")
process.gsf20.gammaThreshold = cms.untracked.double(20.)




process.gsf8new          = process.gsf8.clone()
process.gsf8new.directory = cms.string("L1T/ALL/GSF8new")
process.gsf8new.rctEGamma = cms.InputTag('rctEmulDigis')

process.gsf12new          = process.gsf12.clone()
process.gsf12new.directory = cms.string("L1T/ALL/GSF12new")
process.gsf12new.rctEGamma = cms.InputTag('rctEmulDigis')

process.gsf15new       = process.gsf15.clone()
process.gsf15new.directory = cms.string("L1T/ALL/GSF15new")
process.gsf15new.rctEGamma = cms.InputTag('rctEmulDigis')

process.gsf18new       = process.gsf18.clone()
process.gsf18new.directory = cms.string("L1T/ALL/GSF18new")
process.gsf18new.rctEGamma = cms.InputTag('rctEmulDigis')

process.gsf20new       = process.gsf20.clone()
process.gsf20new.directory = cms.string("L1T/ALL/GSF20new")
process.gsf20new.rctEGamma = cms.InputTag('rctEmulDigis')



process.MEtoEDMConverter = cms.EDProducer("MEtoEDMConverter",
                                    Name = cms.untracked.string('MEtoEDMConverter'),
                                    Verbosity = cms.untracked.int32(2), # 0 provides no output
                                                                        # 1 provides basic output
                                                                        # 2 provide more detailed output
                                    Frequency = cms.untracked.int32(50),
                                    MEPathToSave = cms.untracked.string('.'),
                                    deleteAfterCopy = cms.untracked.bool(False)
)

process.o = cms.OutputModule("PoolOutputModule",
                             splitLevel = cms.untracked.int32(0),
#                             fileName = cms.untracked.string('$outputFileName'),
                             fileName = cms.untracked.string('outputRawZee.root'),
                                 outputCommands = cms.untracked.vstring(
                                      'drop *_*_*_*',
                                      'keep *_MEtoEDMConverter_*_*'
                                 )

                                )

process.rctAnalyzer = cms.EDAnalyzer("L1RCTTestAnalyzer",
                                     #hcalDigisLabel = cms.InputTag("hcalTriggerPrimitiveDigis"),
                                     hcalDigisLabel = cms.InputTag("hcalDigis"),
                                     showEmCands = cms.untracked.bool(True),
                                     ecalDigisLabel = cms.InputTag("ecalDigis:EcalTriggerPrimitives"),
                                     #rctDigisLabel = cms.InputTag("rctDigis"),
                                     rctDigisLabel = cms.InputTag("rctEmulDigis"),
                                     #showRegionSums = cms.untracked.bool(True)
                                     showRegionSums = cms.untracked.bool(False)
                                     )


process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
#process.GlobalTag.globaltag = cms.string('GR_R_42_V19::All')
process.GlobalTag.globaltag = cms.string('GR_R_42_V20::All') # 2011 reprocessing

process.load("Configuration.StandardSequences.RawToDigi_Data_cff")
process.load("Configuration.StandardSequences.Geometry_cff")


process.p1 = cms.Path(process.RawToDigi*
                      process.rctemul*
                      process.rctAnalyzer#*
# #                      process.selectedElectrons*
#                       process.selectedElectronsPlateau*
#                       process.simpleEleId70relIso*
# #                      process.simpleEleId95relIso*
#                       process.electronsWp70*
# #                      process.electronsWp95*
# #                        process.gsf8*
#                       process.gsf8new*
# #                        process.gsf12*
#                       process.gsf12new*
# #                        process.gsf15*
#                       process.gsf15new*
# #                        process.gsf18*
#                       process.gsf18new*
# #                        process.gsf20*
#                       process.gsf20new
)




process.e = cms.EndPath(process.dqmSaver)

process.schedule = cms.Schedule(process.p1)

#print process.dumpPython()
