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

options.register ('rctConfigModule',
                  '',
                  VarParsing.multiplicity.singleton,
                  VarParsing.varType.string,
                  "Name of python module configuring RCT params")
options.register ('isolationThreshold',
                  '',
                  VarParsing.multiplicity.singleton,
                  VarParsing.varType.float,
                  "Configurable isolation threshold")
options.register ('isTest',
                  'False',
                  VarParsing.multiplicity.singleton,
                  VarParsing.varType.bool,
                  "Specify if a test run")
options.parseArguments()


PREFIXTREE = "RctEmTree-Rates-"
PREFIXHISTS = "output-RctVal-MINE-Rates-"
SUFFIX = ".root"

print options.isTest

if options.isTest == True:
    PREFIXTREE = "TEST-" + PREFIXTREE
    PREFIXHISTS = "TEST-" + PREFIXHISTS
    print "Running in test mode!"

#print PREFIXTREE
#print PREFIXHISTS

SCALEVERSION = ""
if "v1" in options.rctConfigModule:
    SCALEVERSION += "V1"
if "v2" in options.rctConfigModule:
    SCALEVERSION += "V2"
if "v3" in options.rctConfigModule:
    SCALEVERSION += "V3"
   

THRESHOLD = str(options.isolationThreshold).replace('.','p')

FILENAME = "scale" + SCALEVERSION + "-thr" + THRESHOLD + "-" + TODAY + SUFFIX

process = cms.Process("RCTVAL")

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1000
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
#     ## Run2011BDoubleElectron Z electron skim: /hdfs/store/data/Run2011B/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v1/0000/
#     '/store/data/Run2011B/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v1/0000/00142CEE-F6ED-E011-9E48-0024E86E8DA7.root',
#     '/store/data/Run2011B/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v1/0000/002D23D0-01EC-E011-9E56-001D096B0DB4.root',
#     '/store/data/Run2011B/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v1/0000/00384F26-E3E1-E011-86E1-0024E8768224.root',
#     '/store/data/Run2011B/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v1/0000/0047EDD1-A9EA-E011-880A-0024E876A889.root',
#     '/store/data/Run2011B/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v1/0000/0055F108-94F3-E011-AE09-001D0967D39B.root',
#     '/store/data/Run2011B/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v1/0000/00747E8D-DEF5-E011-8C15-001D0967DD0A.root',
#     '/store/data/Run2011B/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v1/0000/0091CDBF-6BDE-E011-A36A-002618943922.root',
#     '/store/data/Run2011B/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v1/0000/00C0A6A2-D8EA-E011-9FF8-0024E86E8CFE.root',
#     '/store/data/Run2011B/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v1/0000/00E690D6-E403-E111-AFA2-0024E8768D41.root',
#     '/store/data/Run2011B/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v1/0000/023D5B61-18F0-E011-A4C3-0015178C6ADC.root',
#     '/store/data/Run2011B/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v1/0000/0269AD8B-69F1-E011-B185-00151796D7F4.root',
#     '/store/data/Run2011B/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v1/0000/026BBE1E-A8F3-E011-A64F-0026B94D1ABB.root',
#     '/store/data/Run2011B/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v1/0000/0278BA38-9EEE-E011-9EAB-0015178C4A90.root',
#     '/store/data/Run2011B/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v1/0000/0296EC33-51FA-E011-9714-0015178C66B8.root',
#     '/store/data/Run2011B/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v1/0000/02BEEDA9-FEF8-E011-BC7F-0024E87680F4.root',
#     '/store/data/Run2011B/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v1/0000/02C50F06-0BF9-E011-8E4F-0026B94D1B23.root',
#     '/store/data/Run2011B/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v1/0000/02C6A448-A300-E111-881A-0024E8768072.root',
#     '/store/data/Run2011B/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v1/0000/02FA085A-8A01-E111-8ADC-0026B94D1ABB.root',
#     '/store/data/Run2011B/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v1/0000/04065BF4-28F5-E011-95AB-00151796C088.root',
#     '/store/data/Run2011B/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v1/0000/04111F29-AD03-E111-9FC9-0015178C4CDC.root',
#     '/store/data/Run2011B/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v1/0000/04150CD9-40E8-E011-95D6-001D096B0F80.root',
#     '/store/data/Run2011B/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v1/0000/043B9E47-26F5-E011-AAA6-001D0967DAC1.root',
#     '/store/data/Run2011B/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v1/0000/04B9E5BF-31E8-E011-89D8-0024E8766408.root',
#     '/store/data/Run2011B/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v1/0000/04C9497F-5EF1-E011-B017-00151796D5F8.root',
#     '/store/data/Run2011B/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v1/0000/04DAE6A0-32EE-E011-B229-0015178C6BA4.root',
#     '/store/data/Run2011B/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v1/0000/04FB9888-03FE-E011-9A0D-0015178C49D4.root',
#     '/store/data/Run2011B/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v1/0000/0619A845-59DF-E011-B321-00266CFAEA68.root',
#     '/store/data/Run2011B/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v1/0000/0637423F-BBF3-E011-BEA0-00151796D5C0.root',
#     '/store/data/Run2011B/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v1/0000/06460CA5-8800-E111-BFAB-0026B94D1AE2.root',
#     '/store/data/Run2011B/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v1/0000/06532423-46E6-E011-B2F9-0024E86E8DA7.root',
#     '/store/data/Run2011B/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v1/0000/065443F5-AAE4-E011-8F08-001D0967DA44.root',
#     '/store/data/Run2011B/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v1/0000/065615F1-12DF-E011-9F5D-0024E8768D41.root',
#     '/store/data/Run2011B/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v1/0000/0669929C-23DE-E011-A59E-001D0967BC3E.root',
#     '/store/data/Run2011B/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v1/0000/06D8D174-DB03-E111-BE25-001D0967DAC1.root',
#     '/store/data/Run2011B/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v1/0000/06E57AC4-0CF0-E011-A71E-0024E8768072.root',
#     '/store/data/Run2011B/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v1/0000/06F19769-12F9-E011-90DA-0015178C4A68.root',
#     '/store/data/Run2011B/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v1/0000/08039C1D-97F3-E011-A54F-00151796D8B0.root',
#     '/store/data/Run2011B/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v1/0000/0804AE62-0CF9-E011-A7B5-0024E87680F4.root',
#     '/store/data/Run2011B/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v1/0000/08634C4C-D6DE-E011-B497-0024E8768CD9.root',
#     '/store/data/Run2011B/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v1/0000/088CB5AF-2EF0-E011-8480-001D0967CF95.root',
#     '/store/data/Run2011B/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v1/0000/08A33A07-ABE4-E011-9CD4-001D0967D580.root',
#     '/store/data/Run2011B/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v1/0000/08B5C000-0AF9-E011-9C06-001D0967404B.root',
#     '/store/data/Run2011B/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v1/0000/08BBF92F-67F6-E011-B6B7-0015178C4994.root',
#     '/store/data/Run2011B/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v1/0000/08C5480E-7FFC-E011-8965-00266CFAE854.root',
#     '/store/data/Run2011B/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v1/0000/08E53264-1AE8-E011-863D-001D0967DD0A.root',
#     '/store/data/Run2011B/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v1/0000/08FAE92F-78E3-E011-8D6F-0026B94E27FD.root',
#     '/store/data/Run2011B/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v1/0000/0A13C52E-FAF8-E011-B830-00A0D1EE29D0.root',
#     '/store/data/Run2011B/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v1/0000/0A2D9FAF-7800-E111-9153-0015178C4D8C.root',
#     '/store/data/Run2011B/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v1/0000/0A75C0F4-F9F8-E011-8E7C-00151796D8D0.root',
#     '/store/data/Run2011B/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v1/0000/0AF129F5-EFEF-E011-9DD4-0015178C674C.root'


## Commissioning Run2011B data, skimmed on L1 EG 12, stored in my castor area
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_100_1_5Ht.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_12_1_USh.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_159_1_eXV.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_188_1_tvU.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_216_1_nRv.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_48_1_NiY.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_77_1_CIP.root',
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_101_1_bjn.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_130_1_Wg5.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_15_1_NPQ.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_189_1_8uZ.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_217_1_71w.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_49_1_4nR.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_78_1_o9J.root',
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_102_1_yHs.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_131_1_VTK.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_160_1_esa.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_18_1_zyG.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_218_1_WE8.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_4_1_40F.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_79_1_pWS.root',
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_103_1_jmM.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_132_1_OZZ.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_161_1_U7k.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_190_1_iHS.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_21_1_qlV.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_50_1_LRu.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_7_1_QMT.root',
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_104_1_WEv.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_133_1_QoA.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_162_1_G8Z.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_191_1_q04.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_22_1_oqE.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_51_1_kYo.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_80_1_jCa.root',
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_105_1_VZv.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_134_1_tiV.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_163_1_1KN.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_192_1_CnO.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_23_1_MT1.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_52_1_ZrH.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_81_1_i6D.root',
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_106_1_KUL.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_135_1_W2z.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_164_1_pPz.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_193_1_gHc.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_24_1_jCq.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_53_1_jdP.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_82_1_nZF.root',
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_107_1_yK0.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_136_1_E24.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_165_1_H65.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_194_1_dpn.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_25_1_1QA.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_54_1_bxw.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_83_1_TaA.root',
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_108_1_28E.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_137_1_e2t.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_166_1_ttM.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_195_1_YVk.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_26_1_yPv.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_55_1_9AT.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_84_1_rki.root',
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_109_1_R2B.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_138_1_cYz.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_167_1_V6i.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_196_1_Bcy.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_27_1_tRm.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_56_1_jYw.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_85_1_Q9S.root',
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_10_1_EtT.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_139_1_fGt.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_168_1_CX8.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_197_1_Po7.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_28_1_pg1.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_57_1_wvG.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_86_1_tWK.root',
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_110_1_cL0.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_13_1_Dj2.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_169_1_7tA.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_198_1_2r6.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_29_1_04y.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_58_1_7S8.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_87_1_Xbv.root',
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_111_1_uKU.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_140_1_Hfl.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_16_1_keG.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_199_1_bpJ.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_2_1_61q.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_59_1_AOw.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_88_1_ByI.root',
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_112_1_H5A.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_141_1_aUB.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_170_1_jjp.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_19_1_opM.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_30_1_Bkp.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_5_1_poW.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_89_1_OUL.root',
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_113_1_EGS.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_142_1_eeq.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_171_1_PVq.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_1_1_pN5.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_31_1_MAl.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_60_1_bN2.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_8_1_1hx.root',
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_114_1_QYN.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_143_1_6yV.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_172_1_Jkq.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_200_1_YNM.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_32_1_2wQ.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_61_1_KBa.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_90_1_57E.root',
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_115_1_XaQ.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_144_1_gNu.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_173_1_Ugg.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_201_1_MjL.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_33_1_QZf.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_62_1_0S4.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_91_1_Sc5.root',
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_116_1_CoW.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_145_1_4Se.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_174_1_d4B.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_202_1_1CD.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_34_1_wAo.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_63_1_oWg.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_92_1_P95.root',
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_117_1_asZ.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_146_1_kn8.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_175_1_fEF.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_203_1_MH8.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_35_1_V8m.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_64_1_1DA.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_93_1_gvW.root',
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_118_1_6qW.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_147_1_177.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_176_1_HVX.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_204_1_oRS.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_36_1_dwX.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_65_1_J4R.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_94_1_Wrg.root',
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_119_1_OB7.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_148_1_gEA.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_177_1_IpS.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_205_1_CFE.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_37_1_QrR.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_66_1_ATY.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_95_1_OmD.root',
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_11_1_evD.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_149_1_m5X.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_178_1_eVq.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_206_1_oQ0.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_38_1_b7L.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_67_1_bmq.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_96_1_vnb.root',
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_120_1_gpy.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_14_1_CyE.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_179_1_vVW.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_207_1_TLF.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_39_1_4Pe.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_68_1_DRU.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_97_1_Hib.root',
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_121_1_lMk.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_150_1_MIW.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_17_1_4mC.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_208_1_yt8.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_3_1_rG1.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_69_1_RV2.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_98_1_0NH.root',
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_122_1_nBr.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_151_1_NFo.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_180_1_pSm.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_209_1_qek.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_40_1_WwB.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_6_1_M20.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_99_1_fsJ.root',
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_123_1_jyo.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_152_1_0ED.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_181_1_Azc.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_20_1_Pat.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_41_1_XgP.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_70_1_toO.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_9_1_35N.root',
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_124_1_PKN.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_153_1_XZd.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_182_1_jOB.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_210_1_UQW.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_42_1_7Dk.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_71_1_YoN.root',
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_125_1_90Y.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_154_1_TGc.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_183_1_13s.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_211_1_z8v.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_43_1_WxB.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_72_1_OFp.root',
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_126_1_0NA.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_155_1_8gb.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_184_1_0Di.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_212_1_XwN.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_44_1_523.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_73_1_r4M.root',
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_127_1_krZ.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_156_1_OEr.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_185_1_T6S.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_213_1_dpL.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_45_1_xSS.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_74_1_COA.root',
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_128_1_lNP.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_157_1_w0N.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_186_1_qlO.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_214_1_bPw.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_46_1_mUx.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_75_1_Mfs.root',
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_129_1_ZhN.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_158_1_Y0k.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_187_1_at7.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_215_1_AEC.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_47_1_vnl.root',  
    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_76_1_ym0.root'




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
#process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(5000) )
#process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(50000) )
if options.isTest == True:
    process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1) )
    

process.load("DQMServices.Core.DQM_cfg")
process.load("DQMServices.Components.DQMEnvironment_cfi")

#MODULE_NAME = "Validation.L1Trigger."+options.rctConfigModule
#process.load("Validation.L1Trigger.L1TRCToffline_DataReEmul_cff")
process.load("Validation.L1Trigger."+options.rctConfigModule)
#process.load(MODULE_NAME)
process.RCTConfigProducers.eMinForHoECut = options.isolationThreshold
process.RCTConfigProducers.hMinForHoECut = options.isolationThreshold

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
                                                #   cut = cms.string('pt>10&&pt<25'),
                                                cut = cms.string('pt>5&&pt<50'),
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
#                      process.simpleEleId70relIso*
##                      process.simpleEleId95relIso*
#                      process.electronsWp70*
##                      process.electronsWp95*
                      process.rctemul*
                      process.rctAnalyzer#*
##                      process.selectedElectrons*
#                      process.gsf8new*
#                      process.gsf12new*
#                      process.gsf15new*
#                      process.gsf18new*
#                      process.gsf20new
)




process.e = cms.EndPath(process.dqmSaver)

process.schedule = cms.Schedule(process.p1)

#print process.dumpPython()
