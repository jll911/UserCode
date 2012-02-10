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

PREFIXTREE = "RctEmTree-Rates-"
SUFFIX = ".root"

if options.isTest == True:
    PREFIXTREE = "TEST-" + PREFIXTREE
    print "Running in test mode!"

FILEROOT = TODAY + SUFFIX

process = cms.Process("RCTVAL")

#process.load("Configuration.StandardSequences.RawToDigi__cff") # for MC
process.load("Configuration.StandardSequences.RawToDigi_Data_cff") # for data

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = cms.string('GR_R_44_V13::All') # 2011 reprocessing

#process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(50000) )
if options.isTest == True:
    process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(

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
    ),
)

#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange(process.myLumisToProcess.lumisToProcess)
 
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1000

process.load("Configuration.StandardSequences.Services_cff")
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.MagneticField_38T_cff")
process.load("Configuration.StandardSequences.Reconstruction_cff")

process.TFileService = cms.Service("TFileService",
                                   #fileName = cms.string("RctEmTree-24Nov11.root"),
                                   fileName = cms.string(PREFIXTREE + FILEROOT),
                                   closeFileFast = cms.untracked.bool(True)
                                   )

# process.load("DQMServices.Core.DQM_cfg")
# process.load("DQMServices.Components.DQMEnvironment_cfi")

process.load("Validation.L1Trigger.Rct_LUTconfiguration_v3_cff") 

# # electron selector
# process.load("ElectroWeakAnalysis.WENu.simpleEleIdSequence_cff") 
# process.simpleEleId70relIso.src = cms.InputTag("selectedElectronsPlateau")

# # select a subset of the GsfElectron collection based on the quality stored in a ValueMap
# process.electronsWp70 = cms.EDProducer("BtagGsfElectronSelector",
#                                        input     = cms.InputTag( "selectedElectronsPlateau" ),
#                                        #input     = cms.InputTag( 'gsfElectrons' ),
#                                        selection = cms.InputTag('simpleEleId70relIso'),
#                                        cut       = cms.double(6.5) ### 7 == passing all conv rej, iso, id cuts
#                                        )

# process.selectedElectronsPlateau = cms.EDFilter("GsfElectronSelector",
#                                                 src = cms.InputTag("gsfElectrons"),
#                                                 #   cut = cms.string('pt>10&&pt<25'),
#                                                 cut = cms.string('pt>5&&pt<50'),
#                                                 filter = cms.bool(True)
# )    

# #Reconfigure Environment and saver
# process.DQM.debug = cms.untracked.bool(True)
# process.dqmSaver.saveByRun = cms.untracked.int32(-1)
# process.dqmSaver.saveAtJobEnd = cms.untracked.bool(True)
# process.dqmSaver.workflow = cms.untracked.string('/A/N/C')
# process.dqmSaver.forceRunNumber = cms.untracked.int32(555)
# process.DQMStore.verbose=0


# process.gsf8 = cms.EDAnalyzer('RctValidation',
#                 ecalTPGs = cms.InputTag("ecalDigis:EcalTriggerPrimitives"),
#                 hcalTPGs = cms.InputTag("hcalDigis"),
#                 rctEGamma = cms.InputTag('gctDigis'),
#                 gctEGamma = cms.VInputTag(cms.InputTag('gctDigis','isoEm'),
#                                           cms.InputTag('gctDigis','nonIsoEm')
#                 ),                          
#                 genEGamma = cms.InputTag("electronsWp70"),
#                 directory = cms.string("L1T/ALL/GSF8base"),
#                 maxEt = cms.untracked.double(60),
#                 binsEt = cms.untracked.int32(60),
#                 gammaThreshold = cms.untracked.double(8.),
#                 #outputFileName = cms.untracked.string('output-RctVal-MINE-24Nov11.root')
#                 outputFileName = cms.untracked.string(PREFIXHISTS + FILEROOT)
# )

# process.gsf12          = process.gsf8.clone()
# process.gsf12.directory = cms.string("L1T/ALL/GSF12base")
# process.gsf12.gammaThreshold = cms.untracked.double(12.)

# process.gsf15          = process.gsf12.clone()
# process.gsf15.directory = cms.string("L1T/ALL/GSF15base")
# process.gsf15.gammaThreshold = cms.untracked.double(15.)

# process.gsf18          = process.gsf12.clone()
# process.gsf18.directory = cms.string("L1T/ALL/GSF18base")
# process.gsf18.gammaThreshold = cms.untracked.double(18.)

# process.gsf20          = process.gsf8.clone()
# process.gsf20.directory = cms.string("L1T/ALL/GSF20base")
# process.gsf20.gammaThreshold = cms.untracked.double(20.)




# process.gsf8new          = process.gsf8.clone()
# process.gsf8new.directory = cms.string("L1T/ALL/GSF8new")
# process.gsf8new.rctEGamma = cms.InputTag('rctEmulDigis')

# process.gsf12new          = process.gsf12.clone()
# process.gsf12new.directory = cms.string("L1T/ALL/GSF12new")
# process.gsf12new.rctEGamma = cms.InputTag('rctEmulDigis')

# process.gsf15new       = process.gsf15.clone()
# process.gsf15new.directory = cms.string("L1T/ALL/GSF15new")
# process.gsf15new.rctEGamma = cms.InputTag('rctEmulDigis')

# process.gsf18new       = process.gsf18.clone()
# process.gsf18new.directory = cms.string("L1T/ALL/GSF18new")
# process.gsf18new.rctEGamma = cms.InputTag('rctEmulDigis')

# process.gsf20new       = process.gsf20.clone()
# process.gsf20new.directory = cms.string("L1T/ALL/GSF20new")
# process.gsf20new.rctEGamma = cms.InputTag('rctEmulDigis')



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

process.rctAnalyzer = cms.EDAnalyzer("L1RCTTestAnalyzer",
                                     #hcalDigisLabel = cms.InputTag("hcalTriggerPrimitiveDigis"),
                                     hcalDigisLabel = cms.InputTag("hcalDigis"),
                                     #showEmCands = cms.untracked.bool(True),
                                     showEmCands = cms.untracked.bool(False),
                                     ecalDigisLabel = cms.InputTag("ecalDigis:EcalTriggerPrimitives"),
                                     #rctDigisLabel = cms.InputTag("rctDigis"),
                                     rctDigisLabel = cms.InputTag("rctEmulDigis"),
                                     #showRegionSums = cms.untracked.bool(True)
                                     showRegionSums = cms.untracked.bool(False)
                                     )


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




# process.e = cms.EndPath(process.dqmSaver)

process.schedule = cms.Schedule(process.p1)

#print process.dumpPython()
