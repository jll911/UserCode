import FWCore.ParameterSet.Config as cms

#for LUTs
from Validation.L1Trigger.Rct_LUTconfiguration_v3_cff import *

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

rctemul = cms.Sequence(
    cms.SequencePlaceholder("rctEmulDigis")
)
