#ifndef electree_h
#define electree_h

#include <memory>
#include <string>
#include <iostream>
#include <vector>
#include "TTree.h" 
#include "FWCore/Framework/interface/Event.h" 
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
#include "DataFormats/EgammaReco/interface/BasicClusterFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"

class ElecTree{

 public:
  ElecTree(const edm::ParameterSet&, std::string name, TTree* tree
	   /*,const edm::ParameterSet iConfig*/);
  
  ~ElecTree();
  void ElecInfoCollector(const edm::Event&, const edm::EventSetup&, const reco::GsfElectron& elec); 
  void Clear();
 private:
  ElecTree(){};//Don't allow user
  void SetBranches();
  void ConversionRejection(const edm::Event&, const edm::EventSetup&, const reco::GsfElectron& elec);
  void AddBranch(double* x, std::string name);
  void AddBranch(std::vector<double>*, std::string name);
  void AddBranch(std::vector<int>*, std::string name);
  //void AddBranch(bool* x, std::string name);
  //void AddBranch(double* x, std::string name);
  
  edm::InputTag recHitsEBTag_, recHitsEETag_;
  edm::InputTag dcsTag_;
  bool isData_;
  TTree *tree_;
  std::string name_;
  double nElec_;
  const reco::BasicCluster *bc;
  std::vector<double> elept_;
  std::vector<double> eleeta_;
  std::vector<double> elephi_;
  std::vector<double> eleet_;

  std::vector<int> eleEcalDriven_;
  std::vector<int> eleTrkDriven_;

/*   std::vector<double> dhElecClsTrkCalo_; */
/*   std::vector<double> dhSeedClsTrkCalo_; */
  std::vector<double> dhSuperClsTrkVtx_;
  
/*   std::vector<double> dPhiElecClsTrkCalo_; */
/*   std::vector<double> dPhiSeedClsTrkCalo_; */
  std::vector<double> dPhiSuperClsTrkVtx_;
  
/*   std::vector<double> trkPosXVtx_; */
/*   std::vector<double> trkPosYVtx_; */
/*   std::vector<double> trkPosZVtx_; */
  
/*   std::vector<double> trkMomXVtx_; */
/*   std::vector<double> trkMomYVtx_; */
/*   std::vector<double> trkMomZVtx_; */
  
/*   std::vector<double> trkPosXCalo_; */
/*   std::vector<double> trkPosYCalo_; */
/*   std::vector<double> trkPosZCalo_; */
  
/*   std::vector<double> trkMomXCalo_; */
/*   std::vector<double> trkMomYCalo_; */
/*   std::vector<double> trkMomZCalo_; */

/*   std::vector<double> eEleClsOverPout_; */
/*   std::vector<double> eSeedClsOverP_; */
/*   std::vector<double> eSeedClsOverPout_; */
/*   std::vector<double> eSuperClsOverP_; */
  
/*   std::vector<double> eleMomErr_; */

/*   // isolation dR 0.3      */
/*   std::vector<double> eledr03EcalRecHitSumEt_; */
/*   std::vector<double> eledr03HcalDepth1TowerSumEt_; */
/*   std::vector<double> eledr03HcalDepth2TowerSumEt_; */
/*   std::vector<double> eledr03HcalTowerSumEt_; */
/*   std::vector<double> eledr03TkSumPt_; */
  
/*   // isolation dR 0.4 */
/*   std::vector<double> eledr04EcalRecHitSumEt_; */
/*   std::vector<double> eledr04HcalDepth1TowerSumEt_; */
/*   std::vector<double> eledr04HcalDepth2TowerSumEt_; */
/*   std::vector<double> eledr04HcalTowerSumEt_; */
/*   std::vector<double> eledr04TkSumPt_; */
  
  // relative isolations
  std::vector<double> eleRelIsoEcal_ ;
  std::vector<double> eleRelIsoHcal_ ;
  std::vector<double> eleRelIsoTrk_ ;
  std::vector<double> eleRelIsoComb_ ;
  
  // conversion rejection
  std::vector<double> eleMissingHits_ ;
  std::vector<double> eleDist_ ;
  std::vector<double> eleDeltaCotTheta_ ;
  std::vector<double> eleConvRadius_ ;
  
  std::vector<double> eleMaxOver3x3_ ;
/*   std::vector<double> eleMax_ ; */
/*   std::vector<double> ele3x3_ ; */
  
/*   std::vector<double> e1x5_; */
/*   std::vector<double> e2x5Max_; */
/*   std::vector<double> e5x5_; */
/*   std::vector<double> eler1x5_; */
/*   std::vector<double> eler2x5max_; */
  
/*   std::vector<double> scpreshowerenergy_; */
/*   std::vector<double> scetawidth_; */
/*   std::vector<double> scphiwidth_; */
  std::vector<double> eleScEnergy_;
  std::vector<double> eleScEta_;
  std::vector<double> eleScEt_;

  std::vector<double> eleenergy_;
  
/*   std::vector<double> elehcalDepth1OverEcal_; */
/*   std::vector<double> elehcalDepth2OverEcal_; */
  std::vector<double> elehcalOverEcal_;

/*   std::vector<double> elesigmaEtaEta_; */
  std::vector<double> elesigmaIetaIeta_;
  
/*   std::vector<double> elebasicClustersSize_; */
/*   std::vector<double> elenumberOfBrems_; */
/*   std::vector<double> elefbrem_; */
/*   std::vector<double> elescPixCharge_; */
/*   std::vector<double> electfcharge_; */

  std::vector<double> elecharge_;
/*   std::vector<double> eleisGsfScPixChargeConsistent_; */
/*   std::vector<double> eleisGsfCtfChargeConsistent_; */
/*   std::vector<double> eleisGsfCtfScPixChargeConsistent_; */

/*   std::vector<double> elevertexChi2_; */
/*   std::vector<double> elevertexNdof_; */
/*   std::vector<double> elevertexNormalizedChi2_; */
/*   std::vector<double> elevx_; */
/*   std::vector<double> elevy_; */
/*   std::vector<double> elevz_; */
  
  std::vector<double> elep_;
  std::vector<double> elepx_;
  std::vector<double> elepy_;
  std::vector<double> elepz_;

  // WP selections
  std::vector<int> elePassWp95_;
  std::vector<int> elePassWp95SpikeCut_;
  std::vector<int> elePassWp95ScEtCut_;

  std::vector<int> elePassWp95MissHitsCut_;
  std::vector<int> elePassWp95DistCut_;
  std::vector<int> elePassWp95DCotThetaCut_;
  std::vector<int> elePassWp95ConvRejCut_;

  std::vector<int> elePassWp95RelIsoEcalCut_;
  std::vector<int> elePassWp95RelIsoHcalCut_;
  std::vector<int> elePassWp95RelIsoTrkCut_;
  std::vector<int> elePassWp95RelIsoIndivCut_;
  std::vector<int> elePassWp95RelIsoCombCut_;
  std::vector<int> elePassWp95IsoCut_;

  std::vector<int> elePassWp95SieieCut_;
  std::vector<int> elePassWp95DetainCut_;
  std::vector<int> elePassWp95DphiinCut_;
  std::vector<int> elePassWp95HoeCut_;
  std::vector<int> elePassWp95EidCut_;

  // for use only within this class, not added to tree
  int elePassWp95SpikeCut;
  int elePassWp95ScEtCut;

  int elePassWp95MissHitsCut;
  int elePassWp95DistCut;
  int elePassWp95DCotThetaCut;
  int elePassWp95ConvRejCut;

  int elePassWp95RelIsoEcalCut;
  int elePassWp95RelIsoHcalCut;
  int elePassWp95RelIsoTrkCut;
  int elePassWp95RelIsoIndivCut;
  int elePassWp95RelIsoCombCut;
  int elePassWp95IsoCut;

  int elePassWp95SieieCut;
  int elePassWp95DetainCut;
  int elePassWp95DphiinCut;
  int elePassWp95HoeCut;
  int elePassWp95EidCut;

  int elePassWp95;

};

#endif
