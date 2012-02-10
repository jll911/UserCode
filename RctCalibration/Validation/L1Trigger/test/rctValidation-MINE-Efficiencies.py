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

options.register ('isTest',
                  'False',
                  VarParsing.multiplicity.singleton,
                  VarParsing.varType.bool,
                  "Specify if a test run")
options.parseArguments()


PREFIXHISTS = "RctValHists-Effs-"
SUFFIX = ".root"

if options.isTest == True:
    PREFIXHISTS = "TEST-" + PREFIXHISTS
    print "Running in test mode!"

FILEROOT = TODAY + SUFFIX

process = cms.Process("RCTVAL")

#process.load("Configuration.StandardSequences.RawToDigi_cff") # for MC
process.load("Configuration.StandardSequences.RawToDigi_Data_cff") # for data

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = cms.string('GR_R_44_V13::All') # 2011 reprocessing, 44X

#process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(50000) )
if options.isTest == True:
    process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100) )
    
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(

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
    ),
)

#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange(process.myLumisToProcess.lumisToProcess)
 

process.load("FWCore.MessageLogger.MessageLogger_cfi")
#process.MessageLogger.cerr.FwkReport.reportEvery = 1000

process.load("Configuration.StandardSequences.Services_cff")
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.MagneticField_38T_cff")
process.load("Configuration.StandardSequences.Reconstruction_cff")


process.load("DQMServices.Core.DQM_cfg")
process.load("DQMServices.Components.DQMEnvironment_cfi")

process.load("Validation.L1Trigger.Rct_LUTconfiguration_v3_cff")

# electron selector
process.load("ElectroWeakAnalysis.WENu.simpleEleIdSequence_cff") 
process.simpleEleId70relIso.src = cms.InputTag("selectedElectronsPlateau")

# select a subset of the GsfElectron collection based on the quality stored in a ValueMap
process.electronsWp70 = cms.EDProducer("BtagGsfElectronSelector",
                                       input     = cms.InputTag( "selectedElectronsPlateau" ),
                                       #input     = cms.InputTag( 'gsfElectrons' ),
                                       selection = cms.InputTag('simpleEleId70relIso'),
                                       cut       = cms.double(6.5) ### 7 == passing all conv rej, iso, id cuts
                                       )

process.selectedElectronsPlateau = cms.EDFilter("GsfElectronSelector",
                                                src = cms.InputTag("gsfElectrons"),
                                                #   cut = cms.string('pt>10&&pt<25'),
                                                cut = cms.string('pt>5&&pt<50'),
                                                filter = cms.bool(True)
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
                directory = cms.string("L1T/ALL/GSF8base"),
                maxEt = cms.untracked.double(60),
                binsEt = cms.untracked.int32(60),
                gammaThreshold = cms.untracked.double(8.),
                #outputFileName = cms.untracked.string('output-RctVal-MINE-24Nov11.root')
                outputFileName = cms.untracked.string(PREFIXHISTS + FILEROOT)
)

process.gsf12          = process.gsf8.clone()
process.gsf12.directory = cms.string("L1T/ALL/GSF12base")
process.gsf12.gammaThreshold = cms.untracked.double(12.)

process.gsf15          = process.gsf12.clone()
process.gsf15.directory = cms.string("L1T/ALL/GSF15base")
process.gsf15.gammaThreshold = cms.untracked.double(15.)

process.gsf18          = process.gsf12.clone()
process.gsf18.directory = cms.string("L1T/ALL/GSF18base")
process.gsf18.gammaThreshold = cms.untracked.double(18.)

process.gsf20          = process.gsf8.clone()
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



# process.rctEmulDigis = cms.EDProducer("L1RCTProducer",
#                                       #    hcalDigis = cms.VInputTag(cms.InputTag("simHcalTriggerPrimitiveDigis")),#MC
#                                       hcalDigis = cms.VInputTag(cms.InputTag("hcalDigis")),                    #Data
#                                       useDebugTpgScales = cms.bool(False),
#                                       useEcal = cms.bool(True),
#                                       useHcal = cms.bool(True),
#                                       #    ecalDigis = cms.VInputTag(cms.InputTag("simEcalTriggerPrimitiveDigis")),  #MC
#                                       ecalDigis = cms.VInputTag(cms.InputTag("ecalDigis:EcalTriggerPrimitives")), #Data
#                                       BunchCrossings = cms.vint32(0)
#                                       )

from L1Trigger.RegionalCaloTrigger.rctDigis_cfi import rctDigis
process.rctEmulDigis = rctDigis
process.rctEmulDigis.hcalDigis = cms.VInputTag(cms.InputTag("hcalDigis"))
process.rctEmulDigis.ecalDigis = cms.VInputTag(cms.InputTag("ecalDigis:EcalTriggerPrimitives"))

process.rctemul = cms.Sequence(
    cms.SequencePlaceholder("rctEmulDigis")
    )

process.p1 = cms.Path(process.RawToDigi*
                      process.selectedElectronsPlateau*
                      process.simpleEleId70relIso*
                      process.electronsWp70*
                      process.rctemul*
                      process.gsf8new*
                      process.gsf12new*
                      process.gsf15new*
                      process.gsf18new*
                      process.gsf20new
)




process.e = cms.EndPath(process.dqmSaver)

process.schedule = cms.Schedule(process.p1)

#print process.dumpPython()
