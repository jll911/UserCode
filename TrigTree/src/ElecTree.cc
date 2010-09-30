#include <iostream>

#include "RecoEcal/EgammaCoreTools/interface/EcalClusterLazyTools.h"
#include "UserCode/TrigTree/interface/ElecTree.h"

// for conversion finder
#include "DataFormats/GeometryVector/interface/GlobalPoint.h"
#include "DataFormats/Scalers/interface/DcsStatus.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "MagneticField/Engine/interface/MagneticField.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"
#include "RecoEgamma/EgammaTools/interface/ConversionFinder.h"
#include "RecoEgamma/EgammaTools/interface/ConversionInfo.h"

ElecTree::ElecTree(const edm::ParameterSet& iConfig, std::string name, TTree* tree)
{
  nElec_=0;
  name_=name;
  tree_=tree;
  SetBranches();
  
  recHitsEBTag_ = iConfig.getUntrackedParameter<edm::InputTag>("RecHitsEBTag",edm::InputTag("reducedEcalRecHitsEB"));
  recHitsEETag_ = iConfig.getUntrackedParameter<edm::InputTag>("RecHitsEETag",edm::InputTag("reducedEcalRecHitsEE"));
  
  dcsTag_ = iConfig.getUntrackedParameter<edm::InputTag>("DcsTag",edm::InputTag("scalersRawToDigi"));
  
  isData_ = iConfig.getUntrackedParameter<bool>("IsData");
}


ElecTree::~ElecTree(){
  delete tree_;
}


void
ElecTree::ElecInfoCollector(const edm::Event& iEvent, const edm::EventSetup& iSetup, const reco::GsfElectron& ele)
{
  // First figure out if passes SC Et slimming selection
  double scEnergy = ele.superCluster()->energy();
  double scEta = ele.superCluster()->eta(); // sc eta
  double scEt = scEnergy * sin(2*atan(exp( scEta ) ) ) ; 

  if (scEt < 12.) return;

  // 0 = barrel, 1 = endcap
  // trigger -- not here
  double wp95SpikeCut[] = {0.9, 0.9};
  double wp95ScEtCut[] = {20., 20.};

  double wp95RelIsoCombCut[] = {0.15, 0.10};
  double wp95RelIsoTrkCut[] = {0.15, 0.08};
  double wp95RelIsoEcalCut[] = {2.00, 0.06};
  double wp95RelIsoHcalCut[] = {0.12, 0.05};

  double wp95SieieCut[] = {0.01, 0.03};
  double wp95DphiinCut[] = {0.8, 0.7};
  double wp95DetainCut[] = {0.007, 999.};
  double wp95HoeCut[] = {0.15, 0.07};

  int isBarrelOrEndcap = 0;

  nElec_=nElec_+1;
  elept_.push_back(ele.pt());
  eleeta_.push_back(ele.eta());
  elephi_.push_back(ele.phi());
  eleet_.push_back(ele.et());
  elep_.push_back(ele.p()) ;
  elepx_.push_back(ele.px()) ;
  elepy_.push_back(ele.py()) ;
  elepz_.push_back(ele.pz()) ;

  eleEcalDriven_.push_back(ele.ecalDrivenSeed());
  eleTrkDriven_.push_back(ele.trackerDrivenSeed());
  
//   dhElecClsTrkCalo_.push_back(ele.deltaEtaEleClusterTrackAtCalo());
//   dhSeedClsTrkCalo_.push_back(ele.deltaEtaSeedClusterTrackAtCalo());
  float detain = ele.deltaEtaSuperClusterTrackAtVtx();
  //  dhSuperClsTrkVtx_.push_back(ele.deltaEtaSuperClusterTrackAtVtx()); // detain
  dhSuperClsTrkVtx_.push_back(detain);

//   dPhiElecClsTrkCalo_.push_back(ele.deltaPhiEleClusterTrackAtCalo());
//   dPhiSeedClsTrkCalo_.push_back(ele.deltaPhiSeedClusterTrackAtCalo());
  float dphiin = ele.deltaPhiSuperClusterTrackAtVtx(); 
  //  dPhiSuperClsTrkVtx_.push_back(ele.deltaPhiSuperClusterTrackAtVtx()); // dphiin
  dPhiSuperClsTrkVtx_.push_back(dphiin); 
 
//   trkPosXVtx_.push_back(ele.trackPositionAtVtx().X());
//   trkPosYVtx_.push_back(ele.trackPositionAtVtx().Y());
//   trkPosZVtx_.push_back(ele.trackPositionAtVtx().Z());

//   trkMomXVtx_.push_back(ele.trackMomentumAtVtx().X());
//   trkMomYVtx_.push_back(ele.trackMomentumAtVtx().Y());
//   trkMomZVtx_.push_back(ele.trackMomentumAtVtx().Z());

//   trkPosXCalo_.push_back(ele.trackPositionAtCalo().X());
//   trkPosYCalo_.push_back(ele.trackPositionAtCalo().Y());
//   trkPosZCalo_.push_back(ele.trackPositionAtCalo().Z());

//   trkMomXCalo_.push_back(ele.trackMomentumAtCalo().X());
//   trkMomYCalo_.push_back(ele.trackMomentumAtCalo().Y());
//   trkMomZCalo_.push_back(ele.trackMomentumAtCalo().Z());

//   eEleClsOverPout_.push_back(ele.eEleClusterOverPout());
//   eSeedClsOverP_.push_back(ele.eSeedClusterOverP());
//   eSeedClsOverPout_.push_back(ele.eSeedClusterOverPout());
//   eSuperClsOverP_.push_back(ele.eSuperClusterOverP());
  
//   eleMomErr_.push_back(ele.electronMomentumError());

//   // isolation dR 0.3     
//   eledr03EcalRecHitSumEt_.push_back(ele.dr03EcalRecHitSumEt());
//   eledr03HcalDepth1TowerSumEt_.push_back(ele.dr03HcalDepth1TowerSumEt());
//   eledr03HcalDepth2TowerSumEt_.push_back(ele.dr03HcalDepth2TowerSumEt());
//   eledr03HcalTowerSumEt_.push_back(ele.dr03HcalTowerSumEt());
//   eledr03TkSumPt_.push_back(ele.dr03TkSumPt());
  
//   // isolation dR 0.4
//   eledr04EcalRecHitSumEt_.push_back(ele.dr04EcalRecHitSumEt());
//   eledr04HcalDepth1TowerSumEt_.push_back(ele.dr04HcalDepth1TowerSumEt());
//   eledr04HcalDepth2TowerSumEt_.push_back(ele.dr04HcalDepth2TowerSumEt());
//   eledr04HcalTowerSumEt_.push_back(ele.dr04HcalTowerSumEt());
//   eledr04TkSumPt_.push_back(ele.dr04TkSumPt());
  
  // relative isolations
  float relIsoEcal = ele.dr03EcalRecHitSumEt()/ele.p4().Pt(); 
  //  eleRelIsoEcal_.push_back(ele.dr03EcalRecHitSumEt()/ele.p4().Pt()); // ecal iso
  eleRelIsoEcal_.push_back(relIsoEcal); 
  float relIsoHcal = ele.dr03HcalTowerSumEt()/ele.p4().Pt(); 
  //  eleRelIsoHcal_.push_back(ele.dr03HcalTowerSumEt()/ele.p4().Pt()); // hcal iso
  eleRelIsoHcal_.push_back(relIsoHcal); 
  float relIsoTrk = ele.dr03TkSumPt()/ele.p4().Pt(); 
  //  eleRelIsoTrk_.push_back(ele.dr03TkSumPt()/ele.p4().Pt()); // track iso
  eleRelIsoTrk_.push_back(relIsoTrk);
  float relIsoComb = 0.;
  if ( fabs(ele.eta()) < 1.479 ) // barrel
    //    eleRelIsoComb_.push_back( ( ele.dr03TkSumPt() + std::max(0., ele.dr03EcalRecHitSumEt() - 1.) + ele.dr03HcalTowerSumEt() ) / ele.p4().Pt() ) ;
    relIsoComb = ( ele.dr03TkSumPt() + std::max(0., ele.dr03EcalRecHitSumEt() - 1.) + ele.dr03HcalTowerSumEt() ) / ele.p4().Pt() ;
  else
    //    eleRelIsoComb_.push_back( ( ele.dr03TkSumPt() + ele.dr03EcalRecHitSumEt() + ele.dr03HcalTowerSumEt() ) / ele.p4().Pt() ) ; 
    relIsoComb = ( ele.dr03TkSumPt() + ele.dr03EcalRecHitSumEt() + ele.dr03HcalTowerSumEt() ) / ele.p4().Pt() ; 
  eleRelIsoComb_.push_back( relIsoComb ) ; // combreliso

  ConversionRejection(iEvent, iSetup, ele);
  
  EcalClusterLazyTools lazyTool(iEvent, iSetup, recHitsEBTag_, recHitsEETag_); 
  bc = ele.superCluster()->seed().get(); // get the basic cluster
  float eleMaxOver3x3 = lazyTool.eMax(*bc) / lazyTool.e3x3(*bc) ; 
  //  eleMaxOver3x3_.push_back( lazyTool.eMax(*bc) / lazyTool.e3x3(*bc)  ); // ele max over 3x3 aka spikes
  eleMaxOver3x3_.push_back( eleMaxOver3x3 ); 
//   eleMax_.push_back ( lazyTool.eMax(*bc) );
//   ele3x3_.push_back ( lazyTool.e3x3(*bc) );
  
//   e1x5_.push_back(ele.e1x5());
//   e2x5Max_.push_back(ele.e2x5Max()) ;
//   e5x5_.push_back(ele.e5x5()) ;
//   eler1x5_.push_back(ele.e1x5()/ele.e5x5());
//   eler2x5max_.push_back(ele.e2x5Max()/ele.e5x5());

  reco::SuperClusterRef supercluster = ele.superCluster();

//   scpreshowerenergy_.push_back(supercluster->preshowerEnergy());
//   scetawidth_.push_back(supercluster->etaWidth());
//   scphiwidth_.push_back(supercluster->phiWidth());
  //  eleScEnergy_.push_back(ele.superCluster()->energy());
  eleScEnergy_.push_back(scEnergy);
  //  eleScEta_.push_back(ele.superCluster()->eta());
  eleScEta_.push_back(scEta); 
  //  double scEt = ele.superCluster()->energy() * sin(2*atan(exp( ele.superCluster()->eta() ) ) ) ; 
  //  eleScEt_.push_back( ele.superCluster()->energy() * sin(2*atan(exp( ele.superCluster()->eta() ) ) ) ); // sc et
  eleScEt_.push_back(scEt); 
  eleenergy_.push_back(ele.energy());

//   elehcalDepth1OverEcal_.push_back(ele.hcalDepth1OverEcal());
//   elehcalDepth2OverEcal_.push_back(ele.hcalDepth2OverEcal());
  float hOe = ele.hcalOverEcal(); 
  //  elehcalOverEcal_.push_back(ele.hcalOverEcal()); // hoe
  elehcalOverEcal_.push_back(hOe); 
  
  // sigma eta eta, sigma ieta ieta
//   elesigmaEtaEta_.push_back(ele.sigmaEtaEta());
  float sieie = ele.sigmaIetaIeta(); 
  //  elesigmaIetaIeta_.push_back(ele.sigmaIetaIeta()); // sigma ieta ieta
  elesigmaIetaIeta_.push_back(sieie); 
  
  // misc + charge
//   elebasicClustersSize_.push_back(ele.basicClustersSize()) ;
//   elenumberOfBrems_.push_back(ele.numberOfBrems());
//   elefbrem_.push_back(ele.fbrem()) ;
//   elescPixCharge_.push_back(ele.scPixCharge()) ;
//   electfcharge_.push_back(0.);
//   if (ele.closestCtfTrackRef().isNonnull())
//     {
//       electfcharge_.push_back(ele.closestCtfTrackRef()->charge());
//     }
  elecharge_.push_back(ele.charge());
//   eleisGsfScPixChargeConsistent_.push_back(ele.isGsfScPixChargeConsistent());
//   eleisGsfCtfChargeConsistent_.push_back(ele.isGsfCtfChargeConsistent());
//   eleisGsfCtfScPixChargeConsistent_.push_back(ele.isGsfCtfScPixChargeConsistent());
  
//   // vertex
//   elevertexChi2_.push_back(ele.vertexChi2());
//   elevertexNdof_.push_back(ele.vertexNdof());
//   elevertexNormalizedChi2_.push_back(ele.vertexNormalizedChi2());
//   elevx_.push_back(ele.vx());
//   elevy_.push_back(ele.vy());
//   elevz_.push_back(ele.vz());

  // does it pass wp95?
  isBarrelOrEndcap = ( (scEta < 1.4442 && scEta > -1.4442) ? 0 : 1 );

  elePassWp95SpikeCut = ( eleMaxOver3x3 < wp95SpikeCut[isBarrelOrEndcap] ? 1 : 0 ); // ele max over 3x3

  elePassWp95ScEtCut = ( scEt > wp95ScEtCut[isBarrelOrEndcap] ? 1 : 0 );

  elePassWp95RelIsoEcalCut = ( relIsoEcal < wp95RelIsoEcalCut[isBarrelOrEndcap] ? 1 : 0 );
  elePassWp95RelIsoHcalCut = ( relIsoHcal < wp95RelIsoHcalCut[isBarrelOrEndcap] ? 1 : 0 );
  elePassWp95RelIsoTrkCut = ( relIsoTrk < wp95RelIsoTrkCut[isBarrelOrEndcap] ? 1 : 0 );
  elePassWp95RelIsoCombCut = ( relIsoComb < wp95RelIsoCombCut[isBarrelOrEndcap] ? 1 : 0 );
  elePassWp95RelIsoIndivCut = ( elePassWp95RelIsoEcalCut && elePassWp95RelIsoHcalCut && elePassWp95RelIsoTrkCut); 
  elePassWp95IsoCut = ( elePassWp95RelIsoIndivCut || elePassWp95RelIsoCombCut); 

  elePassWp95SieieCut = ( sieie < wp95SieieCut[isBarrelOrEndcap] ? 1 : 0 );
  elePassWp95DetainCut = ( detain < wp95DetainCut[isBarrelOrEndcap] ? 1 : 0 );
  elePassWp95DphiinCut = ( dphiin < wp95DphiinCut[isBarrelOrEndcap] ? 1 : 0 );
  elePassWp95HoeCut = ( hOe < wp95HoeCut[isBarrelOrEndcap] ? 1 : 0 );
  elePassWp95EidCut = (elePassWp95SieieCut && elePassWp95DetainCut && elePassWp95DphiinCut);

//   elePassWp95 = (elePassWp95SpikeCut && elePassWp95ScEtCut && 
// 		 elePassWp95MissHitsCut && elePassWp95DistCut && elePassWp95DCotThetaCut && 
// 		 ((elePassWp95RelIsoEcalCut && elePassWp95RelIsoHcalCut && elePassWp95RelIsoTrkCut) || 
// 		  (elePassWp95RelIsoCombCut)) && 
// 		 elePassWp95SieieCut && elePassWp95DetainCut && elePassWp95DphiinCut && elePassWp95HoeCut);

  elePassWp95 = (elePassWp95SpikeCut && elePassWp95ScEtCut && elePassWp95ConvRejCut && elePassWp95IsoCut && elePassWp95EidCut);

  elePassWp95SpikeCut_.push_back(elePassWp95SpikeCut);
  elePassWp95ScEtCut_.push_back(elePassWp95ScEtCut);

  elePassWp95MissHitsCut_.push_back(elePassWp95MissHitsCut);
  elePassWp95DistCut_.push_back(elePassWp95DistCut);
  elePassWp95DCotThetaCut_.push_back(elePassWp95DCotThetaCut);
  elePassWp95ConvRejCut_.push_back(elePassWp95ConvRejCut);

  elePassWp95RelIsoEcalCut_.push_back(elePassWp95RelIsoEcalCut);
  elePassWp95RelIsoHcalCut_.push_back(elePassWp95RelIsoHcalCut);
  elePassWp95RelIsoTrkCut_.push_back(elePassWp95RelIsoTrkCut);
  elePassWp95RelIsoIndivCut_.push_back(elePassWp95RelIsoIndivCut);
  elePassWp95RelIsoCombCut_.push_back(elePassWp95RelIsoCombCut);
  elePassWp95IsoCut_.push_back(elePassWp95IsoCut);

  elePassWp95SieieCut_.push_back(elePassWp95SieieCut);
  elePassWp95DetainCut_.push_back(elePassWp95DetainCut);
  elePassWp95DphiinCut_.push_back(elePassWp95DphiinCut);
  elePassWp95HoeCut_.push_back(elePassWp95HoeCut);
  elePassWp95EidCut_.push_back(elePassWp95EidCut);

  elePassWp95_.push_back(elePassWp95);

}

void
ElecTree::ConversionRejection(const edm::Event& iEvent, const edm::EventSetup& iSetup, const reco::GsfElectron& ele)
{
  int wp95MissHitsCut = 1;
  double wp95DistCut = 0.;
  double wp95DCotThetaCut = 0.;

  double evt_bField;
  
  edm::Handle<reco::TrackCollection> tracks_h;
  iEvent.getByLabel("generalTracks", tracks_h);
  
  if (isData_) 
  {
    edm::Handle<DcsStatusCollection> dcsHandle;
    iEvent.getByLabel(dcsTag_, dcsHandle);
    
    // scale factor = 3.801/18166.0 which are average values taken over a stable two week period
    float currentToBFieldScaleFactor = 2.09237036221512717e-04;
    float current = (*dcsHandle)[0].magnetCurrent();
    evt_bField = current*currentToBFieldScaleFactor;
  }
  else 
  {
    edm::ESHandle<MagneticField> magneticField;
    iSetup.get<IdealMagneticFieldRecord>().get(magneticField);
        
    evt_bField = magneticField->inTesla(GlobalPoint(0.,0.,0.)).z();
  }
  
  ConversionFinder convFinder;
  ConversionInfo convInfo = convFinder.getConversionInfo(ele, tracks_h, evt_bField);

  int missingHits = ele.gsfTrack().get()->trackerExpectedHitsInner().numberOfHits();
  //  eleMissingHits_.push_back( ele.gsfTrack().get()->trackerExpectedHitsInner().numberOfHits() ); // miss hits
  eleMissingHits_.push_back(missingHits);
  int dist = convInfo.dist();
  //  eleDist_.push_back(convInfo.dist() ); // dist
  eleDist_.push_back(dist);
  int dCotTheta = convInfo.dcot(); 
  //  eleDeltaCotTheta_.push_back(convInfo.dcot() ) ; // dcottheta
  eleDeltaCotTheta_.push_back(dCotTheta) ; 

  eleConvRadius_.push_back(convInfo.radiusOfConversion() ) ;

  // does it pass conversion rejection cuts?
  elePassWp95MissHitsCut = ( missingHits <= wp95MissHitsCut ? 1 : 0 );
  elePassWp95DistCut = ( fabs(dist) >= wp95DistCut ? 1 : 0 );
  elePassWp95DCotThetaCut = ( fabs(dCotTheta) >= wp95DCotThetaCut ? 1 : 0 );
  
  elePassWp95ConvRejCut = (elePassWp95MissHitsCut && (elePassWp95DistCut || elePassWp95DCotThetaCut));
}


void
ElecTree::SetBranches(){

  AddBranch(&nElec_,"Num");
  AddBranch(&elept_,  "Pt");
  AddBranch(&eleeta_, "Eta");
  AddBranch(&elephi_, "Phi");
  AddBranch(&eleet_,"Et");

  AddBranch(&eleEcalDriven_, "eleEcalDriven");
  AddBranch(&eleTrkDriven_, "eleTrkDriven");

//   AddBranch(&dhElecClsTrkCalo_,"DhElecClsTrkAtCalo");
//   AddBranch(&dhSeedClsTrkCalo_,"DhSeedClsTrkAtCalo");
  AddBranch(&dhSuperClsTrkVtx_,"DhSuperClsTrkAtVtx");
 
//   AddBranch(&dPhiElecClsTrkCalo_,"DphiElecClsTrkAtCalo");
//   AddBranch(&dPhiSeedClsTrkCalo_,"DphiSeedClsTrkAtCalo");
  AddBranch(&dPhiSuperClsTrkVtx_,"DphiSuperClsTrkAtVtx");
 
//   AddBranch(&trkPosXVtx_,"PositionXTrkAtVtx");
//   AddBranch(&trkPosYVtx_,"PositionYTrkAtVtx");
//   AddBranch(&trkPosZVtx_,"PositionZTrkAtVtx");

//   AddBranch(&trkMomXVtx_,"MomentumXTrkAtVtx");
//   AddBranch(&trkMomYVtx_,"MomentumYTrkAtVtx");
//   AddBranch(&trkMomZVtx_,"MomentumZTrkAtVtx");

//   AddBranch(&trkPosXCalo_,"PositionXTrkAtCalo");
//   AddBranch(&trkPosYCalo_,"PositionYTrkAtCalo");
//   AddBranch(&trkPosZCalo_,"PositionZTrkAtCalo");

//   AddBranch(&trkMomXCalo_,"MomentumXTrkAtCalo");
//   AddBranch(&trkMomYCalo_,"MomentumYTrkAtCalo");
//   AddBranch(&trkMomZCalo_,"MomentumZTrkAtCalo");

//   AddBranch(&eEleClsOverPout_,"eEleClsOverPout");
//   AddBranch(&eSeedClsOverP_,"eSeedClsOverP");
//   AddBranch(&eSeedClsOverPout_,"eSeedClsOverPout");
//   AddBranch(&eSuperClsOverP_,"eSuperClsOverP");
  
//   AddBranch(&eleMomErr_,"eleMomErr");

//   // isolation dR 0.3     
//   AddBranch(&eledr03EcalRecHitSumEt_,"eledr03EcalRecHitSumEt");
//   AddBranch(&eledr03HcalDepth1TowerSumEt_,"eledr03HcalDepth1TowerSumEt");
//   AddBranch(&eledr03HcalDepth2TowerSumEt_,"eledr03HcalDepth2TowerSumEt");
//   AddBranch(&eledr03HcalTowerSumEt_,"eledr03HcalTowerSumEt");
//   AddBranch(&eledr03TkSumPt_,"eledr03TkSumPt");
  
//   // isolation dR 0.4
//   AddBranch(&eledr04EcalRecHitSumEt_,"eledr04EcalRecHitSumEt");
//   AddBranch(&eledr04HcalDepth1TowerSumEt_,"eledr04HcalDepth1TowerSumEt");
//   AddBranch(&eledr04HcalDepth2TowerSumEt_,"eledr04HcalDepth2TowerSumEt");
//   AddBranch(&eledr04HcalTowerSumEt_,"eledr04HcalTowerSumEt");
//   AddBranch(&eledr04TkSumPt_,"eledr04TkSumPt");
  
  // relative isolations
  AddBranch(&eleRelIsoEcal_, "eleRelIsoEcal");
  AddBranch(&eleRelIsoHcal_, "eleRelIsoHcal");
  AddBranch(&eleRelIsoTrk_,  "eleRelIsoTrk");
  AddBranch(&eleRelIsoComb_,  "eleRelIsoComb");
  
  // conversion rejection
  AddBranch(&eleMissingHits_, "eleMissingHits");
  AddBranch(&eleDist_, "eleDist" );
  AddBranch(&eleDeltaCotTheta_, "eleDeltaCotTheta");
  AddBranch(&eleConvRadius_, "eleConvRadius");
  
  AddBranch(&eleMaxOver3x3_, "eleMaxOver3x3");
//   AddBranch(&ele3x3_, "ele3x3");
//   AddBranch(&eleMax_, "eleMax");

//   AddBranch(&e1x5_,"e1x5");
//   AddBranch(&e2x5Max_,"e2x5Max");
//   AddBranch(&e5x5_,"e5x5");
//   AddBranch(&eler1x5_,"eler1x5");
//   AddBranch(&eler2x5max_,"eler2x5max");
  
//   AddBranch(&scpreshowerenergy_,"scpreshowerenergy");
//   AddBranch(&scetawidth_,"scetawidth");
//   AddBranch(&scphiwidth_,"scphiwidth");
  AddBranch(&eleScEnergy_, "eleScEnergy");
  AddBranch(&eleScEta_, "eleScEta");
  AddBranch(&eleScEt_, "eleScEt");
  AddBranch(&eleenergy_,"eleenergy");
  
//   AddBranch(&elehcalDepth1OverEcal_,"elehcalDepth1OverEcal");
//   AddBranch(&elehcalDepth2OverEcal_,"elehcalDepth2OverEcal");
  AddBranch(&elehcalOverEcal_,"elehcalOverEcal");

//   AddBranch(&elesigmaEtaEta_,"elesigmaEtaEta");
  AddBranch(&elesigmaIetaIeta_,"elesigmaIetaIeta");
  
//   AddBranch(&elebasicClustersSize_,"elebasicClustersSize");
//   AddBranch(&elenumberOfBrems_,"elenumberOfBrems");
//   AddBranch(&elefbrem_,"elefbrem");
//   AddBranch(&elescPixCharge_,"elescPixCharge");
//   AddBranch(&electfcharge_,"electfcharge");

  AddBranch(&elecharge_,"elecharge");
//   AddBranch(&eleisGsfScPixChargeConsistent_,"eleisGsfScPixChargeConsistent");
//   AddBranch(&eleisGsfCtfChargeConsistent_,"eleisGsfCtfChargeConsistent");
//   AddBranch(&eleisGsfCtfScPixChargeConsistent_,"eleisGsfCtfScPixChargeConsistent");

//   AddBranch(&elevertexChi2_,"elevertexChi2");
//   AddBranch(&elevertexNdof_,"elevertexNdof");
//   AddBranch(&elevertexNormalizedChi2_,"elevertexNormalizedChi2");
//   AddBranch(&elevx_,"elevx");
//   AddBranch(&elevy_,"elevy");
//   AddBranch(&elevz_,"elevz");
  
  AddBranch(&elep_,"elep");
  AddBranch(&elepx_,"elepx");
  AddBranch(&elepy_,"elepy");
  AddBranch(&elepz_,"elepz");

  // WP selections
  AddBranch(&elePassWp95SpikeCut_,"elePassWp95SpikeCut");
  AddBranch(&elePassWp95ScEtCut_,"elePassWp95ScEtCut");

  AddBranch(&elePassWp95MissHitsCut_,"elePassWp95MissHitsCut");
  AddBranch(&elePassWp95DistCut_,"elePassWp95DistCut");
  AddBranch(&elePassWp95DCotThetaCut_,"elePassWp95DCotThetaCut");
  AddBranch(&elePassWp95ConvRejCut_,"elePassWp95ConvRejCut");

  AddBranch(&elePassWp95RelIsoEcalCut_,"elePassWp95RelIsoEcalCut");
  AddBranch(&elePassWp95RelIsoHcalCut_,"elePassWp95RelIsoHcalCut");
  AddBranch(&elePassWp95RelIsoTrkCut_,"elePassWp95RelIsoTrkCut");
  AddBranch(&elePassWp95RelIsoIndivCut_,"elePassWp95RelIsoIndivCut");
  AddBranch(&elePassWp95RelIsoCombCut_,"elePassWp95RelIsoCombCut");
  AddBranch(&elePassWp95IsoCut_,"elePassWp95IsoCut");

  AddBranch(&elePassWp95SieieCut_,"elePassWp95SieieCut");
  AddBranch(&elePassWp95DetainCut_,"elePassWp95DetainCut");
  AddBranch(&elePassWp95DphiinCut_,"elePassWp95DphiinCut");
  AddBranch(&elePassWp95HoeCut_,"elePassWp95HoeCut");
  AddBranch(&elePassWp95EidCut_,"elePassWp95EidCut");

  AddBranch(&elePassWp95_,"elePassWp95");
}

void
ElecTree::AddBranch(std::vector<double>* vec, std::string name){
  std::string brName="Electron"+name;
  tree_->Branch(brName.c_str(),vec);
}


void
ElecTree::AddBranch(std::vector<int>* vec, std::string name){
      std::string brName="Electron"+name;
        tree_->Branch(brName.c_str(),vec);
} 

void 
ElecTree::AddBranch(double* x, std::string name){
  std::string brName="Electron"+name;
  tree_->Branch(brName.c_str(),x,(brName+"/D").c_str());
}

void 
ElecTree::Clear(){
  nElec_=0;

  elept_.clear();
  eleeta_.clear();
  elephi_.clear();
  eleet_.clear();
  eleEcalDriven_.clear();
  eleTrkDriven_.clear();
//   dhElecClsTrkCalo_.clear();
//   dhSeedClsTrkCalo_.clear();
  dhSuperClsTrkVtx_.clear();
//   dPhiElecClsTrkCalo_.clear();
//   dPhiSeedClsTrkCalo_.clear();
  dPhiSuperClsTrkVtx_.clear();
//   trkPosXVtx_.clear();
//   trkPosYVtx_.clear();
//   trkPosZVtx_.clear();
//   trkMomXVtx_.clear();
//   trkMomYVtx_.clear();
//   trkMomZVtx_.clear();
//   trkPosXCalo_.clear();
//   trkPosYCalo_.clear();
//   trkPosZCalo_.clear();
//   trkMomXCalo_.clear();
//   trkMomYCalo_.clear();
//   trkMomZCalo_.clear();

//   eEleClsOverPout_.clear();
//   eSeedClsOverP_.clear();
//   eSeedClsOverPout_.clear();
//   eSuperClsOverP_.clear();
  
//   eleMomErr_.clear();

//   // isolation dR 0.3     
//   eledr03EcalRecHitSumEt_.clear();
//   eledr03HcalDepth1TowerSumEt_.clear();
//   eledr03HcalDepth2TowerSumEt_.clear();
//   eledr03HcalTowerSumEt_.clear();
//   eledr03TkSumPt_.clear();
  
//   // isolation dR 0.4
//   eledr04EcalRecHitSumEt_.clear();
//   eledr04HcalDepth1TowerSumEt_.clear();
//   eledr04HcalDepth2TowerSumEt_.clear();
//   eledr04HcalTowerSumEt_.clear();
//   eledr04TkSumPt_.clear();
  
  // relative isolations
  eleRelIsoComb_.clear();
  eleRelIsoEcal_.clear();
  eleRelIsoHcal_.clear();
  eleRelIsoTrk_.clear();
  
  // conversion rejection
  eleMissingHits_.clear();
  eleDist_.clear();
  eleDeltaCotTheta_.clear();
  eleConvRadius_.clear();
  
  eleMaxOver3x3_.clear();
//   eleMax_.clear();
//   ele3x3_.clear();
 
//   e1x5_.clear();
//   e2x5Max_.clear();
//   e5x5_.clear();
//   eler1x5_.clear();
//   eler2x5max_.clear();
  
//   scpreshowerenergy_.clear();
//   scetawidth_.clear();
//   scphiwidth_.clear();
  eleScEnergy_.clear();
  eleScEta_.clear();
  eleScEt_.clear();
  eleenergy_.clear();
  
//   elehcalDepth1OverEcal_.clear();
//   elehcalDepth2OverEcal_.clear();
  elehcalOverEcal_.clear();
  
//   elesigmaEtaEta_.clear();
  elesigmaIetaIeta_.clear();
  
//   elebasicClustersSize_.clear();
//   elenumberOfBrems_.clear();
//   elefbrem_.clear();
//   elescPixCharge_.clear();
//   electfcharge_.clear();
  
  elecharge_.clear();
//   eleisGsfScPixChargeConsistent_.clear();
//   eleisGsfCtfChargeConsistent_.clear();
//   eleisGsfCtfScPixChargeConsistent_.clear();
  
//   elevertexChi2_.clear();
//   elevertexNdof_.clear();
//   elevertexNormalizedChi2_.clear();
//   elevx_.clear();
//   elevy_.clear();
//   elevz_.clear();
  
  elep_.clear();
  elepx_.clear();
  elepy_.clear();
  elepz_.clear();

  // WP selections
  elePassWp95_.clear();
  elePassWp95SpikeCut_.clear();
  elePassWp95ScEtCut_.clear();

  elePassWp95MissHitsCut_.clear();
  elePassWp95DistCut_.clear();
  elePassWp95DCotThetaCut_.clear();
  elePassWp95ConvRejCut_.clear();

  elePassWp95RelIsoEcalCut_.clear();
  elePassWp95RelIsoHcalCut_.clear();
  elePassWp95RelIsoTrkCut_.clear();
  elePassWp95RelIsoIndivCut_.clear();
  elePassWp95RelIsoCombCut_.clear();
  elePassWp95IsoCut_.clear();

  elePassWp95SieieCut_.clear();
  elePassWp95DetainCut_.clear();
  elePassWp95DphiinCut_.clear();
  elePassWp95HoeCut_.clear();
  elePassWp95EidCut_.clear();
  
}
