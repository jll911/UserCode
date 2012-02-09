# -*- coding: utf-8 -*-

import sys
import os
#import dbs_discovery
import FWCore.ParameterSet.Config as cms
import datetime

from FWCore.ParameterSet.VarParsing import VarParsing
options = VarParsing ('analysis')

NOW = datetime.datetime.now()
TODAY = NOW.strftime("%d%b%y")

print "Date is "
print TODAY

# options.register ('rctConfigModule',
#                   '',
#                   VarParsing.multiplicity.singleton,
#                   VarParsing.varType.string,
#                   "Name of python module configuring RCT params")
# options.register ('isolationThreshold',
#                   '',
#                   VarParsing.multiplicity.singleton,
#                   VarParsing.varType.float,
#                   "Configurable isolation threshold")
options.register ('isTest',
                  'False',
                  VarParsing.multiplicity.singleton,
                  VarParsing.varType.bool,
                  "Specify if a test run")
options.parseArguments()


PREFIXTREE = "RctEmTree-scaleFactors-"
PREFIXHISTS = "output-RctVal-MINE-scaleFactors-"
SUFFIX = ".root"

print options.isTest

if options.isTest == True:
    PREFIXTREE = "TEST-" + PREFIXTREE
    PREFIXHISTS = "TEST-" + PREFIXHISTS
    print "picked up the test option!"

#print PREFIXTREE
#print PREFIXHISTS

SCALEVERSION = "identity"
#if "v1" in options.rctConfigModule:
#    SCALEVERSION += "V1"
#if "v2" in options.rctConfigModule:
#    SCALEVERSION += "V2"
   

#THRESHOLD = str(options.isolationThreshold).replace('.','p')
#THRESHOLD = str(process.RCTConfigProducers.eMinForHoECut).replace('.','p')
THRESHOLD = "2p0"

FILENAME = "scale" + SCALEVERSION + "-thr" + THRESHOLD + "-" + TODAY + SUFFIX

process = cms.Process("RCTVAL")

process.load("FWCore.MessageLogger.MessageLogger_cfi")
#process.MessageLogger.cerr.FwkReport.reportEvery = 1000
process.load("DQMServices.Core.DQM_cfg")

process.load("Configuration.StandardSequences.Services_cff")
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.MagneticField_38T_cff")
process.load("Configuration.StandardSequences.Reconstruction_cff")

process.TFileService = cms.Service("TFileService",
                                   #fileName = cms.string("RctEmTree-24Nov11.root"),
                                   fileName = cms.string(PREFIXTREE + FILENAME),
                                   closeFileFast = cms.untracked.bool(True)
                                   )



# electron selector

process.load("ElectroWeakAnalysis.WENu.simpleEleIdSequence_cff") 
#process.ElectronIDs = cms.Sequence(process.simpleEleIdSequence)
#process.simpleEleId70relIso.src = cms.InputTag("selectedElectronsPlateau")
process.simpleEleId70relIso.src = cms.InputTag("selectedElectronsTurnOn20")
#process.simpleEleId95relIso.src = cms.InputTag("selectedElectronsPlateau")


process.load("Configuration.EventContent.EventContent_cff")

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
#'/store/data/Run2010B/Electron/RAW-RECO/v2/000/146/644/544F6D08-12CB-DF11-9059-003048678FF6.root'
#'/store/data/Run2010B/MinimumBias/RAW-RECO/v2/000/146/944/36A3519B-B3CC-DF11-B099-002618943877.root'
#                                '/store/data/Run2010B/EGMonitor/RAW/v1/000/147/826/6A32EF3E-41D6-DF11-966F-001617DBD5AC.root'
#       $inputFileNames
#                               '/store/data/Run2011A/SingleElectron/RAW-RECO/WElectron-May10ReReco-v1/0000/EEC6F492-A67B-E011-A716-0025901D40CA.root'
## Run2011A
#     '/store/data/Run2011A/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v4/0000/16D4C6A7-C888-E011-B3E9-0018F3D0965A.root',
#     '/store/data/Run2011A/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v4/0000/00586781-1AA4-E011-BCFC-0018F3D09624.root',
#     '/store/data/Run2011A/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v4/0000/00B05691-EC93-E011-A051-002618FDA28E.root',
#     '/store/data/Run2011A/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v4/0000/00C8ED4A-318C-E011-8D51-00304867918A.root',
#     '/store/data/Run2011A/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v4/0000/00CFB44E-638D-E011-845F-0026189438E4.root',
#     '/store/data/Run2011A/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v4/0000/00E4EB3D-999E-E011-9930-003048678BAA.root',
#     '/store/data/Run2011A/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v4/0000/022BDA18-0896-E011-BFDD-00248C0BE005.root',
#     '/store/data/Run2011A/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v4/0000/02580F53-E58F-E011-BAA1-0026189438E4.root',
#     '/store/data/Run2011A/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v4/0000/0258D9D6-2D9A-E011-8CE7-0026189437FE.root',
#     '/store/data/Run2011A/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v4/0000/025E5053-AD8F-E011-9DEB-00261894389C.root',
#     '/store/data/Run2011A/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v4/0000/02600068-DB9A-E011-A1ED-001BFCDBD166.root',
#     '/store/data/Run2011A/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v4/0000/028BE86B-5B90-E011-836E-002618943975.root',
#     '/store/data/Run2011A/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v4/0000/0293D66C-898E-E011-87A0-0026189437F0.root',
#     '/store/data/Run2011A/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v4/0000/02E52A20-C48B-E011-B803-00261894391C.root',
#     '/store/data/Run2011A/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v4/0000/02F00C3B-9396-E011-9423-0018F3D096E8.root',
#     '/store/data/Run2011A/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v4/0000/02F59743-198C-E011-9E39-002618943860.root',
#     '/store/data/Run2011A/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v4/0000/02F7E67E-0F8C-E011-B0EC-001A92971BC8.root',
#     '/store/data/Run2011A/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v4/0000/02FFC130-C19A-E011-8C7D-00261894387D.root',
#     '/store/data/Run2011A/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v4/0000/040815B8-2D99-E011-BEF2-003048678F06.root',
#     '/store/data/Run2011A/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v4/0000/04496805-60A3-E011-A37C-003048678E92.root'

    ## Run2011BDoubleElectron Z electron skim: /hdfs/store/data/Run2011B/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v1/0000/
    '/store/data/Run2011B/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v1/0000/00142CEE-F6ED-E011-9E48-0024E86E8DA7.root',
    '/store/data/Run2011B/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v1/0000/002D23D0-01EC-E011-9E56-001D096B0DB4.root',
    '/store/data/Run2011B/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v1/0000/00384F26-E3E1-E011-86E1-0024E8768224.root',
    '/store/data/Run2011B/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v1/0000/0047EDD1-A9EA-E011-880A-0024E876A889.root',
    '/store/data/Run2011B/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v1/0000/0055F108-94F3-E011-AE09-001D0967D39B.root',
    '/store/data/Run2011B/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v1/0000/00747E8D-DEF5-E011-8C15-001D0967DD0A.root',
    '/store/data/Run2011B/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v1/0000/0091CDBF-6BDE-E011-A36A-002618943922.root',
    '/store/data/Run2011B/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v1/0000/00C0A6A2-D8EA-E011-9FF8-0024E86E8CFE.root',
    '/store/data/Run2011B/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v1/0000/00E690D6-E403-E111-AFA2-0024E8768D41.root',
    '/store/data/Run2011B/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v1/0000/023D5B61-18F0-E011-A4C3-0015178C6ADC.root',
    '/store/data/Run2011B/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v1/0000/0269AD8B-69F1-E011-B185-00151796D7F4.root',
    '/store/data/Run2011B/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v1/0000/026BBE1E-A8F3-E011-A64F-0026B94D1ABB.root',
    '/store/data/Run2011B/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v1/0000/0278BA38-9EEE-E011-9EAB-0015178C4A90.root',
    '/store/data/Run2011B/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v1/0000/0296EC33-51FA-E011-9714-0015178C66B8.root',
    '/store/data/Run2011B/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v1/0000/02BEEDA9-FEF8-E011-BC7F-0024E87680F4.root',
    '/store/data/Run2011B/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v1/0000/02C50F06-0BF9-E011-8E4F-0026B94D1B23.root',
    '/store/data/Run2011B/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v1/0000/02C6A448-A300-E111-881A-0024E8768072.root',
    '/store/data/Run2011B/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v1/0000/02FA085A-8A01-E111-8ADC-0026B94D1ABB.root',
    '/store/data/Run2011B/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v1/0000/04065BF4-28F5-E011-95AB-00151796C088.root',
    '/store/data/Run2011B/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v1/0000/04111F29-AD03-E111-9FC9-0015178C4CDC.root',
    '/store/data/Run2011B/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v1/0000/04150CD9-40E8-E011-95D6-001D096B0F80.root',
    '/store/data/Run2011B/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v1/0000/043B9E47-26F5-E011-AAA6-001D0967DAC1.root',
    '/store/data/Run2011B/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v1/0000/04B9E5BF-31E8-E011-89D8-0024E8766408.root',
    '/store/data/Run2011B/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v1/0000/04C9497F-5EF1-E011-B017-00151796D5F8.root',
    '/store/data/Run2011B/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v1/0000/04DAE6A0-32EE-E011-B229-0015178C6BA4.root',
    '/store/data/Run2011B/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v1/0000/04FB9888-03FE-E011-9A0D-0015178C49D4.root',
    '/store/data/Run2011B/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v1/0000/0619A845-59DF-E011-B321-00266CFAEA68.root',
    '/store/data/Run2011B/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v1/0000/0637423F-BBF3-E011-BEA0-00151796D5C0.root',
    '/store/data/Run2011B/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v1/0000/06460CA5-8800-E111-BFAB-0026B94D1AE2.root',
    '/store/data/Run2011B/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v1/0000/06532423-46E6-E011-B2F9-0024E86E8DA7.root',
    '/store/data/Run2011B/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v1/0000/065443F5-AAE4-E011-8F08-001D0967DA44.root',
    '/store/data/Run2011B/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v1/0000/065615F1-12DF-E011-9F5D-0024E8768D41.root',
    '/store/data/Run2011B/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v1/0000/0669929C-23DE-E011-A59E-001D0967BC3E.root',
    '/store/data/Run2011B/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v1/0000/06D8D174-DB03-E111-BE25-001D0967DAC1.root',
    '/store/data/Run2011B/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v1/0000/06E57AC4-0CF0-E011-A71E-0024E8768072.root',
    '/store/data/Run2011B/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v1/0000/06F19769-12F9-E011-90DA-0015178C4A68.root',
    '/store/data/Run2011B/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v1/0000/08039C1D-97F3-E011-A54F-00151796D8B0.root',
    '/store/data/Run2011B/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v1/0000/0804AE62-0CF9-E011-A7B5-0024E87680F4.root',
    '/store/data/Run2011B/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v1/0000/08634C4C-D6DE-E011-B497-0024E8768CD9.root',
    '/store/data/Run2011B/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v1/0000/088CB5AF-2EF0-E011-8480-001D0967CF95.root',
    '/store/data/Run2011B/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v1/0000/08A33A07-ABE4-E011-9CD4-001D0967D580.root',
    '/store/data/Run2011B/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v1/0000/08B5C000-0AF9-E011-9C06-001D0967404B.root',
    '/store/data/Run2011B/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v1/0000/08BBF92F-67F6-E011-B6B7-0015178C4994.root',
    '/store/data/Run2011B/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v1/0000/08C5480E-7FFC-E011-8965-00266CFAE854.root',
    '/store/data/Run2011B/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v1/0000/08E53264-1AE8-E011-863D-001D0967DD0A.root',
    '/store/data/Run2011B/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v1/0000/08FAE92F-78E3-E011-8D6F-0026B94E27FD.root',
    '/store/data/Run2011B/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v1/0000/0A13C52E-FAF8-E011-B830-00A0D1EE29D0.root',
    '/store/data/Run2011B/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v1/0000/0A2D9FAF-7800-E111-9153-0015178C4D8C.root',
    '/store/data/Run2011B/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v1/0000/0A75C0F4-F9F8-E011-8E7C-00151796D8D0.root',
    '/store/data/Run2011B/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v1/0000/0AF129F5-EFEF-E011-9DD4-0015178C674C.root'


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

#process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
#process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1) )
#process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1000) )
#process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(5000) )
#process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(50000) )
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(200000) )
if options.isTest == True:
    process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1) )
    

process.load("DQMServices.Core.DQM_cfg")
process.load("DQMServices.Components.DQMEnvironment_cfi")

#MODULE_NAME = "Validation.L1Trigger."+options.rctConfigModule
#process.load("Validation.L1Trigger.L1TRCToffline_DataReEmul_cff")
#process.load("Validation.L1Trigger."+options.rctConfigModule)
process.load("Validation.L1Trigger.L1TRCToffline_DataReEmul_identity_cff")
#process.load(MODULE_NAME)
#process.RCTConfigProducers.eMinForHoECut = options.isolationThreshold
#process.RCTConfigProducers.hMinForHoECut = options.isolationThreshold

print "eMin cut is ", process.RCTConfigProducers.eMinForHoECut
print "hMin cut is ", process.RCTConfigProducers.hMinForHoECut


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
                                       #input     = cms.InputTag( "selectedElectronsPlateau" ),
                                       input     = cms.InputTag( "selectedElectronsTurnOn20" ),
                                       #input     = cms.InputTag( 'gsfElectrons' ),
                                       selection = cms.InputTag('simpleEleId70relIso'),
                                       cut       = cms.double(6.5) ### 7 == passing all conv rej, iso, id cuts
                                       )

# process.electronsWp95 = cms.EDProducer("BtagGsfElectronSelector",
#                                        input     = cms.InputTag( "selectedElectronsPlateau" ),
#                                        #input     = cms.InputTag( "gsfElectrons" ),
#                                        selection = cms.InputTag('simpleEleId95relIso'),
#                                        cut       = cms.double(6.5) ### 7 == passing all convrej, iso, id cuts
#                                        )

process.selectedElectronsPlateau = cms.EDFilter("GsfElectronSelector",
                                                src = cms.InputTag("gsfElectrons"),
                                                #   cut = cms.string('pt>10&&pt<25'),
                                                cut = cms.string('pt>5&&pt<50'),
                                                filter = cms.bool(True)
)    

process.selectedElectronsTurnOn20 = cms.EDFilter("GsfElectronSelector",
                                                 src = cms.InputTag("gsfElectrons"),
                                                 #   cut = cms.string('pt>10&&pt<25'),
                                                 cut = cms.string('pt>18&&pt<30'),
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
                #outputFileName = cms.untracked.string('output-RctVal-MINE-24Nov11.root')
                outputFileName = cms.untracked.string(PREFIXHISTS + FILENAME)
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
#                      process.selectedElectronsPlateau*
                      process.selectedElectronsTurnOn20*
                      process.simpleEleId70relIso*
#                      process.simpleEleId95relIso*
                      process.electronsWp70*
#                      process.electronsWp95*
                      process.rctemul*
                      process.rctAnalyzer*
#                      process.selectedElectrons*
                      process.gsf8new*
                      process.gsf12new*
                      process.gsf15new*
                      process.gsf18new*
                      process.gsf20new
)




process.e = cms.EndPath(process.dqmSaver)

process.schedule = cms.Schedule(process.p1)

#print process.dumpPython()
