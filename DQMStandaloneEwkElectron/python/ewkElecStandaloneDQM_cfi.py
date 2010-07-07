import FWCore.ParameterSet.Config as cms

# DQM monitor module for EWK-WMuNu
ewkElecStandaloneDQM = cms.EDAnalyzer("EwkElecStandaloneDQM",
      # generate the event tree?
      #MakeTree = cms.untracked.bool(False),
      MakeTree = cms.untracked.bool(True),

      # Input collections ->
      TrigTag = cms.untracked.InputTag("TriggerResults::HLT"),
      ElecTag = cms.untracked.InputTag("gsfElectrons"),

      # Main cuts ->
#      ElecTrig = cms.untracked.vstring("HLT_MinBiasBSC","HLT_MinBiasBSC"),
      ElecTrig = cms.untracked.vstring("HLT_Ele10_SW_L1R","HLT_Ele10_LW_L1R","HLT_Photon15_L1R","HLT_L1SingleEG5","HLT_L1SingleEG8","HLT_MinBiasBSC"),

      # Zee 95 values (95% efficient at catching Zee's)
      PtCut = cms.untracked.double(10.0), # 20.0 # "official" number
      EtaCut = cms.untracked.double(2.5), # 2.4
      SieieBarrel = cms.untracked.double(0.01),
      SieieEndcap = cms.untracked.double(0.03),
      HoEBarrel = cms.untracked.double(0.05),
      HoEEndcap = cms.untracked.double(0.04),
      DetainBarrel = cms.untracked.double(0.006),
      DetainEndcap = cms.untracked.double(0.008),
      DphiinBarrel = cms.untracked.double(0.8),
      DphiinEndcap = cms.untracked.double(0.7),
#      IsRelativeIso = cms.untracked.bool(True),
#      IsCombinedIso = cms.untracked.bool(False),
#      IsoCut03 = cms.untracked.double(0.1),
      EcalIsoCutBarrel = cms.untracked.double(5.0),
      EcalIsoCutEndcap = cms.untracked.double(3.0),
      HcalIsoCutBarrel = cms.untracked.double(5.0),
      HcalIsoCutEndcap = cms.untracked.double(2.0),
      TrkIsoCutBarrel = cms.untracked.double(7.0),
      TrkIsoCutEndcap = cms.untracked.double(8.0),
      MtMin = cms.untracked.double(-999999),
      MtMax = cms.untracked.double(999999.0),
      MetMin = cms.untracked.double(-999999.),
      MetMax = cms.untracked.double(999999.),

      # superclusters                                      
      SuperClustersEBTag = cms.untracked.InputTag("correctedHybridSuperClusters"),
      SuperClustersEETag = cms.untracked.InputTag("correctedMulti5x5SuperClustersWithPreshower"),

      # Z
      MZMin = cms.untracked.double(0.),
      MZMax = cms.untracked.double(999999.),

#      # Muon quality cuts ->
#      DxyCut = cms.untracked.double(0.2),
#      NormalizedChi2Cut = cms.untracked.double(10.),
#      TrackerHitsCut = cms.untracked.int32(11),
#      IsAlsoTrackerMuon = cms.untracked.bool(True),

#      # To suppress Zmm ->
#      PtThrForZ1 = cms.untracked.double(20.0),
#      PtThrForZ2 = cms.untracked.double(10.0),


      MetCaloTag = cms.untracked.InputTag("met"),
      MetCaloCorrTag = cms.untracked.InputTag("metMuonJESCorAK5"),# not working #metMuonJESCorAK5  #metJESCorAK5CaloJet
      MetPfTag = cms.untracked.InputTag("pfMet"),
      MetPfCorrTag = cms.untracked.InputTag("pfMetCorr"), #dummy, don't have correction yet
      MetTcTag = cms.untracked.InputTag("tcMet"), 
      GenMetTag = cms.untracked.InputTag("genMetTrue"),
      UseCorrCaloMet = cms.untracked.bool(True),
      UseCorrPFMet = cms.untracked.bool(False), 
      UseGenMet = cms.untracked.bool(False),               
      METIncludesMuons = cms.untracked.bool(False),

      # To further suppress ttbar ->
      EJetMin = cms.untracked.double(30.),
      NJetMax = cms.untracked.int32(999999),

      ### Jets
      #JetCaloTag = cms.untracked.InputTag("antikt5CaloJets"),#antikt5CaloJets#ak5CaloJets
      JetCaloTag = cms.untracked.InputTag("ak5CaloJets"),#antikt5CaloJets#ak5CaloJets
      #JetPfTag = cms.untracked.InputTag("antikt5PFJets"),#antikt5PFJets#ak5PFJets
      JetPfTag = cms.untracked.InputTag("ak5PFJets"),#antikt5PFJets#ak5PFJets
      JetCaloCorrTag = cms.untracked.InputTag("L2L3CorJetAK5Calo"),
      JetPfCorrTag = cms.untracked.InputTag("L2L3CorJetAK5PF"),
      GenJetTag = cms.untracked.InputTag("ak5GenJets"),
      JetCaloPtCut = cms.untracked.double(10.),
      JetCaloCorrPtCut = cms.untracked.double(10.),
      JetPfPtCut = cms.untracked.double(10.),
      JetPfCorrPtCut = cms.untracked.double(10.),
      JetGenPtCut = cms.untracked.double(10.),
      UseCorrCaloJets = cms.untracked.bool(False),
      UseCorrPFJets = cms.untracked.bool(False),
      UseGenJets = cms.untracked.bool(False),
      MinJetElectronDr = cms.untracked.double(0.5)
                      #      HistFile = cms.untracked.string("DQMHistos.root")
                            
)
