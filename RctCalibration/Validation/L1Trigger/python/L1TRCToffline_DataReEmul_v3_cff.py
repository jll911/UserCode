import FWCore.ParameterSet.Config as cms

#from Configuration.StandardSequences.FrontierConditions_GlobalTag_cff import *

#add'n
#from Configuration.StandardSequences.Geometry_cff import * #BEAUTIFY

#unpacking
#from Configuration.StandardSequences.RawToDigi_Data_cff import *

#emulator/comparator
#from L1Trigger.HardwareValidation.L1HardwareValidation_cff import * #BEAUTIFY
#from L1Trigger.Configuration.L1Config_cff import *

#for LUTs
#from DQM.L1TMonitor.Rct_LUTconfiguration_133158_cff import *
#from Validation.L1Trigger.Rct_LUTconfiguration_MC_cff import *
#from Validation.L1Trigger.Rct_LUTconfigurationIDENTITY_MC_cff import *
from Validation.L1Trigger.Rct_LUTconfiguration_v3_cff import *
#from DQM.L1TMonitor.christos1 import *

# For the GT
#from L1TriggerConfig.L1GtConfigProducers.L1GtConfig_cff import * #BEAUTIFY

#dqm
rctEmulDigis = cms.EDProducer("L1RCTProducer",
#    hcalDigis = cms.VInputTag(cms.InputTag("simHcalTriggerPrimitiveDigis")),#MC
    hcalDigis = cms.VInputTag(cms.InputTag("hcalDigis")),                    #Data   
    useDebugTpgScales = cms.bool(False),
    useEcal = cms.bool(True),
    useHcal = cms.bool(True),
#    ecalDigis = cms.VInputTag(cms.InputTag("simEcalTriggerPrimitiveDigis")),  #MC
    ecalDigis = cms.VInputTag(cms.InputTag("ecalDigis:EcalTriggerPrimitives")), #Data
    BunchCrossings = cms.vint32(0)
)

# BEAUTIFY

# l1tderct = cms.EDFilter("L1TdeRCT",
#     rctSourceData = cms.InputTag("l1GctHwDigis"),
#     HistFolder = cms.untracked.string('L1TEMU/L1TdeRCT/'),
#     outputFile = cms.untracked.string('./run133158.root'),
#     verbose = cms.untracked.bool(False),
#     DQMStore = cms.untracked.bool(True),
#     singlechannelhistos = cms.untracked.bool(False),
#     ecalTPGData = cms.InputTag("",""),
#     rctSourceEmul = cms.InputTag("rctDigis"),
#     disableROOToutput = cms.untracked.bool(False),
#     hcalTPGData = cms.InputTag("",""),
#     gtEGAlgoName = cms.string("L1_SingleEG2"),
#     doubleThreshold = cms.int32(3),
#     gtDigisLabel = cms.InputTag("gtDigis")
# )

# #MC
# #l1tderct.rctSourceData = 'simRctDigis'
# #l1tderct.rctSourceEmul = 'rctEmulDigis'
# #l1tderct.ecalTPGData = 'simEcalTriggerPrimitiveDigis'
# #l1tderct.hcalTPGData = 'simHcalTriggerPrimitiveDigis'

# #Data
# l1tderct.rctSourceData = 'gctDigis'
# l1tderct.rctSourceEmul = 'rctEmulDigis'
# #l1tderct.ecalTPGData = 'ecalEBunpacker:EcalTriggerPrimitives'
# l1tderct.ecalTPGData = 'ecalDigis:EcalTriggerPrimitives'
# l1tderct.hcalTPGData = 'hcalDigis:HcalTriggerPrimitives'

# #l1trct = cms.EDFilter("L1TRCT",
# #    DQMStore = cms.untracked.bool(True),
# #    disableROOToutput = cms.untracked.bool(False),
# #    outputFile = cms.untracked.string('./run133158.root'),
# #    rctSource = cms.InputTag("l1GctHwDigis","","DQM"),
# #    verbose = cms.untracked.bool(False)
# #)

# #l1trct.rctSource = 'gctDigis'

rctemul = cms.Sequence(
#     cms.SequencePlaceholder("RawToDigi")
    cms.SequencePlaceholder("rctEmulDigis")
#    *cms.SequencePlaceholder("l1trct")
#    *cms.SequencePlaceholder("l1tderct")
)
