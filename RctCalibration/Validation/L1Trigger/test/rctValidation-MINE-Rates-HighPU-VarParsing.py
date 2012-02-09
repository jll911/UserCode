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


PREFIXTREE = "RctEmTree-Rates-HighPU-"
PREFIXHISTS = "output-RctVal-MINE-Rates-HighPU-"
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

## High PU Run, L1EGHPF Run2011B data, skimmed on L1 EG 12, stored in my castor area
#    'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-30Nov11/skim_100_1_5Ht.root',  
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_100_1_6dj.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_101_1_UDn.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_102_1_u8t.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_103_1_dkd.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_104_1_NHz.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_105_1_NLG.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_106_1_bPU.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_107_1_Dbh.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_108_1_8ni.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_109_1_S3O.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_10_1_SIb.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_110_1_0HU.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_111_1_MYl.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_112_1_Zt1.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_113_1_iO5.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_114_1_1G2.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_115_1_yph.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_116_1_kSG.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_117_1_1mo.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_118_1_LJi.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_119_1_MCL.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_11_1_o5g.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_120_1_snO.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_121_1_0iM.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_122_1_Tws.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_123_1_F2H.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_124_1_GGt.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_125_1_6xU.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_126_1_bQw.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_127_1_cB5.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_128_1_RR2.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_129_1_a7L.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_12_1_PtX.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_130_1_UCU.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_131_1_TcS.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_132_1_4P8.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_133_1_Mg9.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_134_1_r6t.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_135_1_zX8.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_136_1_75t.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_137_1_wGk.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_138_1_o2j.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_139_1_Yjy.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_13_1_7pA.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_140_1_ack.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_141_1_QO4.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_142_1_gVa.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_143_1_6NH.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_144_1_knQ.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_145_1_glG.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_146_1_c7C.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_147_1_ITE.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_148_1_5Bp.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_149_1_VAs.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_14_1_93F.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_150_1_iWk.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_151_1_zM6.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_152_1_nT5.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_153_1_1nf.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_154_1_1Xf.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_155_1_rLJ.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_156_1_8PN.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_157_1_EDh.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_158_1_t3D.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_159_1_jKY.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_15_1_xKD.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_160_1_n8Q.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_16_1_yRy.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_17_1_RXn.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_18_1_Ynd.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_19_1_nxY.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_1_1_O8A.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_20_1_D9E.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_21_1_d9a.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_22_1_Yj2.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_23_1_icH.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_24_1_AnC.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_25_1_9vT.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_26_1_xkE.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_27_1_E4o.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_28_1_sT3.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_29_1_4ds.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_2_1_OsE.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_30_1_6Lb.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_31_1_8Jd.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_32_1_YIc.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_33_1_MqX.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_34_1_xdV.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_35_1_LfK.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_36_1_Lta.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_37_1_RN8.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_38_1_1C3.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_39_1_udf.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_3_1_wOM.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_40_1_C9O.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_41_1_6ok.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_42_1_MbI.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_43_1_CMY.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_44_1_R8a.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_45_1_LAt.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_46_1_cma.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_47_1_Mux.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_48_1_b6i.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_49_1_Nx6.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_4_1_OzY.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_50_1_n5T.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_51_1_O8j.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_52_1_YC8.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_53_1_qNV.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_54_1_wGM.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_55_1_g9N.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_56_1_p9Z.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_57_1_onE.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_58_1_PRK.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_59_1_Boe.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_5_1_H3J.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_60_1_q7R.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_61_1_M8K.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_62_1_op1.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_63_1_UVR.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_64_1_Yzz.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_65_1_0nU.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_66_1_LL1.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_67_1_0Wz.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_68_1_v51.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_69_1_gXV.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_6_1_FgF.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_70_1_erQ.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_71_1_vmq.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_72_1_nPN.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_73_1_93o.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_74_1_e6c.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_75_1_YEv.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_76_1_M45.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_77_1_hAt.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_78_1_xE7.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_79_1_wx0.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_7_1_nQE.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_80_1_hiO.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_81_1_4Ov.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_82_1_Ey0.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_83_1_EUl.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_84_1_WF7.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_85_1_Sg0.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_86_1_ShJ.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_87_1_pr4.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_88_1_QAC.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_89_1_N4M.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_8_1_7yY.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_90_1_a0A.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_91_1_W2s.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_92_1_3tA.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_93_1_CU1.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_94_1_4tt.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_95_1_4eh.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_96_1_b64.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_97_1_VO9.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_98_1_Jyo.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_99_1_TM7.root',
'file:/tmp/jleonard/Skim-HLT-L1SingleEG12-13Dec11/skim_9_1_LPK.root'





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
