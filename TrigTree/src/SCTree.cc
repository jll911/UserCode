#include "UserCode/TrigTree/interface/SCTree.h"
#include "DataFormats/EgammaReco/interface/BasicClusterFwd.h"
#include "RecoEcal/EgammaCoreTools/interface/EcalClusterLazyTools.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include <iostream>
#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "DataFormats/EgammaReco/interface/SuperClusterFwd.h"
#include "UserCode/TrigTree/interface/JetUtil.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Provenance/interface/ParameterSetID.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"

#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/EDMException.h"
#include "DataFormats/EgammaReco/interface/BasicClusterFwd.h"
#include "Calibration/EcalCalibAlgos/interface/ElectronRecalibSuperClusterAssociator.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronCoreFwd.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronCore.h"
#include "RecoEcal/EgammaCoreTools/interface/EcalClusterTools.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "DataFormats/Common/interface/Handle.h"
#include "Geometry/CaloEventSetup/interface/CaloTopologyRecord.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/CaloTopology/interface/CaloTopology.h"
#include "Geometry/CaloTopology/interface/CaloSubdetectorTopology.h"
#include "RecoEgamma/EgammaTools/interface/HoECalculator.h"
#include "RecoEcal/EgammaCoreTools/interface/EcalClusterLazyTools.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronCoreFwd.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronCore.h"
#include "DataFormats/EgammaCandidates/interface/Electron.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include <iostream>
using namespace std;
using namespace edm;



SCTree::SCTree(std::string name,TTree* tree,const edm::ParameterSet & cfg):
  recHitsEBTag_(cfg.getUntrackedParameter<edm::InputTag>("RecHitsEBTag",edm::InputTag("reducedEcalRecHitsEB"))),
  recHitsEETag_(cfg.getUntrackedParameter<edm::InputTag>("RecHitsEETag",edm::InputTag("reducedEcalRecHitsEE")))
{
  nscEB_=0;
  nscEE_=0;
  name_=name;
  tree_=tree;

  SetBranches();
}

void
SCTree::SetBranches(){

  AddBranch(&nscEB_,"scEBNum");
  AddBranch(&scEBenergy_,  "scEBEnergy");
  AddBranch(&scEBet_,  "scEBEt");
  AddBranch(&scEBeta_, "scEBEta");
  AddBranch(&scEBphi_, "scEBPhi");
  AddBranch(&scEBrawenergy_,"scEBRawEnergy");
  AddBranch(&scEBpreshowerEnergy_,"scEBpreshowerEnergy"); 
  AddBranch(&scEBenergy1_,"scEBenergy1");
  AddBranch(&scEBenergy2x2_,"scEBenergy2x2");
  AddBranch(&scEBenergy3x3_,"scEBenergy3x3");
  AddBranch(&scEBenergy1x5_,"scEBenergy1x5");
  AddBranch(&scEBenergy2x5_,"scEBenergy2x5");
  AddBranch(&scEBenergy5x5_,"scEBenergy5x5");
  AddBranch(&scEBHoverE_,"scEBHoverE");
  AddBranch(&scEBx_,"scEBx");
  AddBranch(&scEBy_,"scEBy");
  AddBranch(&scEBz_,"scEBz");
  AddBranch(&scEBetaWidth_,"scEBetaWidth");
  AddBranch(&scEBphiWidth_,"scEBphiWidth");
  AddBranch(&scEBsigmaetaeta_,"scEBsigmaetaeta");
  AddBranch(&scEBsigmaIetaIeta_,"scEBsigmaIetaIeta");
  AddBranch(&scEBsigmaphiphi_,"scEBsigmaphiphi");
  AddBranch(&scEBsigmaIphiIphi_,"scEBsigmaIphiIphi");
  AddBranch(&scEBsize_,"scEBsize");
  AddBranch(&scEBnBasicClusters_,"scEBnBasicClusters");

  AddBranch(&nscEE_,"scEENum");
  AddBranch(&scEEenergy_,  "scEEEnergy");
  AddBranch(&scEEet_,  "scEEEt");
  AddBranch(&scEEeta_, "scEEEta");
  AddBranch(&scEEphi_, "scEEPhi");
  AddBranch(&scEErawenergy_,"scEERawEnergy");
  AddBranch(&scEEpreshowerEnergy_,"scEEpreshowerEnergy"); // SHOULD BE ZERO IN BARREL, HERE FOR COMPLETENESS
  AddBranch(&scEEenergy1_,"scEEenergy1");
  AddBranch(&scEEenergy2x2_,"scEEenergy2x2");
  AddBranch(&scEEenergy3x3_,"scEEenergy3x3");
  AddBranch(&scEEenergy1x5_,"scEEenergy1x5");
  AddBranch(&scEEenergy2x5_,"scEEenergy2x5");
  AddBranch(&scEEenergy5x5_,"scEEenergy5x5");
  AddBranch(&scEEHoverE_,"scEEHoverE");
  AddBranch(&scEEx_,"scEEx");
  AddBranch(&scEEy_,"scEEy");
  AddBranch(&scEEz_,"scEEz");
  AddBranch(&scEEetaWidth_,"scEEetaWidth");
  AddBranch(&scEEphiWidth_,"scEEphiWidth");
  AddBranch(&scEEsigmaetaeta_,"scEEsigmaetaeta");
  AddBranch(&scEEsigmaIetaIeta_,"scEEsigmaIetaIeta");
  AddBranch(&scEEsigmaphiphi_,"scEEsigmaphiphi");
  AddBranch(&scEEsigmaIphiIphi_,"scEEsigmaIphiIphi");
  AddBranch(&scEEsize_,"scEEsize");
  AddBranch(&scEEnBasicClusters_,"scEEnBasicClusters");

}

void
SCTree::AddBranch(std::vector<double>* vec, std::string name){
  std::string brName=name;//"SuperClus"+name;
  tree_->Branch(brName.c_str(),vec);
} 
  
void 
SCTree::AddBranch(double* x, std::string name){
  std::string brName=name;
  tree_->Branch(brName.c_str(),x,(brName+"/D").c_str());
} 

SCTree::~SCTree(){
  delete tree_;
}
 
void
SCTree::Fill(const edm::Event& iEvent,const edm::EventSetup& iSetup){
  
  edm::ESHandle<CaloTopology> CaloTopo;
  iSetup.get<CaloTopologyRecord>().get(CaloTopo);
  const CaloTopology *topology = CaloTopo.product();

  edm::ESHandle<CaloGeometry> caloGeom;
  iSetup.get<CaloGeometryRecord>().get(caloGeom);

  edm::Handle<EcalRecHitCollection> EcalRecHitBarrelCollection;
  edm::Handle<EcalRecHitCollection> EcalRecHitEndcapCollection;
  iEvent.getByLabel("ecalRecHit", "EcalRecHitsEB", EcalRecHitBarrelCollection);
  iEvent.getByLabel("ecalRecHit", "EcalRecHitsEE", EcalRecHitEndcapCollection);

  const EcalRecHitCollection* ecalRecHitBarrelCollection = EcalRecHitBarrelCollection.product();
  const EcalRecHitCollection* ecalRecHitEndcapCollection = EcalRecHitEndcapCollection.product();

  EcalClusterLazyTools lazyTools(iEvent, iSetup, recHitsEBTag_, recHitsEETag_);      
  
  // Superclusters collections
  edm::Handle<reco::SuperClusterCollection> superClustersEBCollectionH;
  edm::Handle<reco::SuperClusterCollection> superClustersEECollectionH;
  edm::InputTag SuperClustersEBTag_("correctedHybridSuperClusters");
  edm::InputTag SuperClustersEETag_("correctedMulti5x5SuperClustersWithPreshower");
  
  if (!iEvent.getByLabel(SuperClustersEBTag_, superClustersEBCollectionH)) {
    std::cout<<">>> Superclusters EB collection does not exist !!!"<<std::endl;
    return;
  }
  
  if (!iEvent.getByLabel(SuperClustersEETag_, superClustersEECollectionH)) {
    std::cout<<">>> Superclusters EE collection does not exist !!!"<<std::endl;
    return;
  }
  
  reco::SuperClusterCollection superClustersEBCollection(*(superClustersEBCollectionH.product()));
  std::sort(superClustersEBCollection.begin(),superClustersEBCollection.end(),scEtGreater());

  reco::SuperClusterCollection superClustersEECollection(*(superClustersEECollectionH.product()));
  std::sort(superClustersEECollection.begin(),superClustersEECollection.end(),scEtGreater());
  
  reco::SuperClusterCollection::const_iterator scEB;

  // Do For CORRECTED BARREL SuperClusters

  for(scEB=superClustersEBCollection.begin(); scEB!=superClustersEBCollection.end(); scEB++ )
    {
      nscEB_=nscEB_+1;
      // kinematics
      scEBnBasicClusters_.push_back(scEB->clustersSize());
      scEBet_.push_back(scEB->energy()/std::cosh(scEB->position().eta()));
      scEBenergy_.push_back(scEB->energy());
      scEBeta_.push_back(scEB->eta());
      scEBphi_.push_back(scEB->phi());
      scEBrawenergy_.push_back(scEB->rawEnergy());
	  
      // cluster shape
      scEBpreshowerEnergy_.push_back(scEB->preshowerEnergy()); // ZERO BARREL..
      std::vector<float> cov ;
      cov = EcalClusterTools::covariances(*(scEB->seed()),&(*ecalRecHitBarrelCollection), &(*topology) , caloGeom.product(),  4.7);
      std::vector<float> localcov ;
      localcov = EcalClusterTools::localCovariances(*(scEB->seed()),&(*ecalRecHitBarrelCollection), &(*topology) );
      
      //double HoECalculator::operator() ( const reco::SuperCluster* clus , const edm::Event& e , const edm::EventSetup& c) 
      // {
      //   return getHoE(GlobalPoint(clus->x(),clus->y(),clus->z()), clus->energy(), e,c);
      // }

      HoECalculator a ;
      a = HoECalculator(caloGeom);
      double hovere = a(&(*scEB),iEvent,iSetup);
      scEBenergy1_.push_back(lazyTools.getMaximum(*(scEB->seed())).second);
      scEBenergy2x2_.push_back(lazyTools.e2x2(*(scEB->seed())));
      scEBenergy3x3_.push_back(lazyTools.e3x3(*(scEB->seed())));
      scEBenergy1x5_.push_back(lazyTools.e1x5(*(scEB->seed())));
      scEBenergy2x5_.push_back(lazyTools.e2x5Max(*(scEB->seed())));
      scEBenergy5x5_.push_back(lazyTools.e5x5(*(scEB->seed())));
      scEBHoverE_.push_back(hovere);
      scEBx_.push_back(scEB->x());
      scEBy_.push_back(scEB->y());
      scEBz_.push_back(scEB->z());
      scEBetaWidth_.push_back(scEB->etaWidth());
      scEBphiWidth_.push_back(scEB->phiWidth());
      scEBsigmaetaeta_.push_back(sqrt(cov[0]));
      scEBsigmaIetaIeta_.push_back(sqrt(localcov[0]));
      scEBsigmaphiphi_.push_back(sqrt(cov[2]));
      scEBsigmaIphiIphi_.push_back(sqrt(localcov[2]));
      scEBsize_.push_back(scEB->size());
    }

  //Do for CORRECTED ENDCAP SuperClusters

  reco::SuperClusterCollection::const_iterator scEE;
  for(scEE=superClustersEECollection.begin(); scEE!=superClustersEECollection.end(); scEE++ )
    {
      nscEE_=nscEE_+1;
      // kinematics
      scEEnBasicClusters_.push_back(scEE->clustersSize());
      scEEet_.push_back(scEE->energy()/std::cosh(scEE->position().eta()));
      scEEenergy_.push_back(scEE->energy());
      scEEeta_.push_back(scEE->eta());
      scEEphi_.push_back(scEE->phi());
      scEErawenergy_.push_back(scEE->rawEnergy());
	  
      // cluster shape
      scEEpreshowerEnergy_.push_back(scEE->preshowerEnergy()); 

      std::vector<float> cov ;
      cov = EcalClusterTools::covariances(*(scEE->seed()),&(*ecalRecHitEndcapCollection), &(*topology) , caloGeom.product(),  4.7);
      std::vector<float> localcov ;
      localcov = EcalClusterTools::localCovariances(*(scEE->seed()),&(*ecalRecHitEndcapCollection), &(*topology) );
     
      HoECalculator a ;
      a = HoECalculator(caloGeom);
      double hovere = a(&(*scEE),iEvent,iSetup);
      scEEenergy1_.push_back(lazyTools.getMaximum(*(scEE->seed())).second);
      scEEenergy2x2_.push_back(lazyTools.e2x2(*(scEE->seed())));
      scEEenergy3x3_.push_back(lazyTools.e3x3(*(scEE->seed())));
      scEEenergy1x5_.push_back(lazyTools.e1x5(*(scEE->seed())));
      scEEenergy2x5_.push_back(lazyTools.e2x5Max(*(scEE->seed())));
      scEEenergy5x5_.push_back(lazyTools.e5x5(*(scEE->seed())));
      scEEHoverE_.push_back(hovere);
      scEEx_.push_back(scEE->x());
      scEEy_.push_back(scEE->y());
      scEEz_.push_back(scEE->z());
      scEEetaWidth_.push_back(scEE->etaWidth());
      scEEphiWidth_.push_back(scEE->phiWidth());
      scEEsigmaetaeta_.push_back(sqrt(cov[0]));
      scEEsigmaIetaIeta_.push_back(sqrt(localcov[0]));
      scEEsigmaphiphi_.push_back(sqrt(cov[2]));
      scEEsigmaIphiIphi_.push_back(sqrt(localcov[2]));
      scEEsize_.push_back(scEE->size());
      
    }


}


void 
SCTree::Clear(){
  nscEB_=0;
  scEBenergy_.clear();
  scEBet_.clear();
  scEBeta_.clear();
  scEBphi_.clear();
  scEBrawenergy_.clear();
  scEBpreshowerEnergy_.clear(); 
  scEBenergy1_.clear(); // (lazyTools.getMaximum(*(scEB->seed())).second);
  scEBenergy2x2_.clear(); // (lazyTools.e2x2(*(scEB->seed())));
  scEBenergy3x3_.clear(); // (lazyTools.e3x3(*(scEB->seed())));
  scEBenergy1x5_.clear(); // (lazyTools.e1x5(*(scEB->seed())));
  scEBenergy2x5_.clear(); // (lazyTools.e2x5Max(*(scEB->seed())));
  scEBenergy5x5_.clear(); // (lazyTools.e5x5(*(scEB->seed())));
  scEBHoverE_.clear(); // (hovere);
  scEBx_.clear(); // (scEB->x());
  scEBy_.clear(); // (scEB->y());
  scEBz_.clear(); // (scEB->z());
  scEBetaWidth_.clear(); // (scEB->etaWidth());
  scEBphiWidth_.clear(); // (scEB->phiWidth());
  scEBsigmaetaeta_.clear(); // (sqrt(stdcov[0]));
  scEBsigmaIetaIeta_.clear(); // (sqrt(localcov[0]));
  scEBsigmaphiphi_.clear(); // (sqrt(stdcov[2]));
  scEBsigmaIphiIphi_.clear(); // (sqrt(localcov[2]));
  scEBsize_.clear(); // (scEB->size());
  scEBnBasicClusters_.clear();

  nscEE_=0;
  scEEenergy_.clear();
  scEEet_.clear();
  scEEeta_.clear();
  scEEphi_.clear();
  scEErawenergy_.clear();
  scEEpreshowerEnergy_.clear(); 
  scEEenergy1_.clear(); // (lazyTools.getMaximum(*(scEE->seed())).second);
  scEEenergy2x2_.clear(); // (lazyTools.e2x2(*(scEE->seed())));
  scEEenergy3x3_.clear(); // (lazyTools.e3x3(*(scEE->seed())));
  scEEenergy1x5_.clear(); // (lazyTools.e1x5(*(scEE->seed())));
  scEEenergy2x5_.clear(); // (lazyTools.e2x5Max(*(scEE->seed())));
  scEEenergy5x5_.clear(); // (lazyTools.e5x5(*(scEE->seed())));
  scEEHoverE_.clear(); // (hovere);
  scEEx_.clear(); // (scEE->x());
  scEEy_.clear(); // (scEE->y());
  scEEz_.clear(); // (scEE->z());
  scEEetaWidth_.clear(); // (scEE->etaWidth());
  scEEphiWidth_.clear(); // (scEE->phiWidth());
  scEEsigmaetaeta_.clear(); // (sqrt(stdcov[0]));
  scEEsigmaIetaIeta_.clear(); // (sqrt(localcov[0]));
  scEEsigmaphiphi_.clear(); // (sqrt(stdcov[2]));
  scEEsigmaIphiIphi_.clear(); // (sqrt(localcov[2]));
  scEEsize_.clear(); // (scEE->size());
  scEEnBasicClusters_.clear();


}
