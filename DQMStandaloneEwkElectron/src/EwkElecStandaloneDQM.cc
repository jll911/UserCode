#include "UserCode/DQMStandaloneEwkElectron/src/EwkElecStandaloneDQM.h"

#include <boost/algorithm/string.hpp>
#include <vector>
//#include <pair>

#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/JetReco/interface/JetID.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/METReco/interface/MET.h"
#include "DataFormats/METReco/interface/CaloMET.h"
#include "DataFormats/METReco/interface/CaloMETCollection.h"
#include "DataFormats/METReco/interface/CaloMETFwd.h" 
#include "DataFormats/METReco/interface/PFMET.h"
#include "DataFormats/METReco/interface/PFMETFwd.h"
#include "DataFormats/METReco/interface/GenMET.h"
#include "DataFormats/METReco/interface/GenMETFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"
#include "PhysicsTools/CandUtils/interface/CandCombiner.h"
#include "CommonTools/Utils/interface/MassRangeSelector.h"
#include "DataFormats/Common/interface/RefProd.h"
#include "FWCore/Framework/interface/TriggerNames.h" // 36X: #include "FWCore/Common/interface/TriggerNames.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "RecoEcal/EgammaCoreTools/interface/EcalClusterLazyTools.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/EgammaReco/interface/BasicClusterFwd.h"
//#include "DataFormats/EgammaReco/interface/BasicCluster.h"
#include "DataFormats/Provenance/interface/EventID.h"

#include "UserCode/DQMStandaloneEwkElectron/test/setTDRStyle.h"
//#include "DQMServices/Core/interface/DQMStore.h"
//#include "DQMServices/Core/interface/MonitorElement.h"

//#include "DataFormats/MuonReco/interface/Muon.h"

// for using pair of string and bool for triggers fired.  BUT! this isn't a ROOT macro!
//#ifdef __MAKECINT__
#ifdef __CINT__
#pragma link C++ class pair<std::string,bool>+;
#pragma link C++ class map<std::string,bool>+;
#endif

using namespace edm;
using namespace std;
using namespace reco;


const int EwkElecStandaloneDQM::k_ELEC_MAX;
const int EwkElecStandaloneDQM::k_SC_MAX;

EwkElecStandaloneDQM::EwkElecStandaloneDQM( const ParameterSet & cfg ) :
  //
  makeTree_(cfg.getUntrackedParameter<bool>("MakeTree", false)),
      // Input collections
      trigTag_(cfg.getUntrackedParameter<edm::InputTag> ("TrigTag", edm::InputTag("TriggerResults::HLT"))),
      elecTag_(cfg.getUntrackedParameter<edm::InputTag> ("ElecTag", edm::InputTag("gsfElectrons"))), 
      superClustersEBTag_(cfg.getUntrackedParameter<edm::InputTag>("SuperClustersEBTag", edm::InputTag("correctedHybridSuperClusters"))),
      superClustersEETag_(cfg.getUntrackedParameter<edm::InputTag>("SuperClustersEETag", edm::InputTag("correctedMulti5x5SuperClustersWithPreshower"))),
      recHitsEBTag_(cfg.getUntrackedParameter<edm::InputTag>("RecHitsEBTag",edm::InputTag("reducedEcalRecHitsEB"))),
      recHitsEETag_(cfg.getUntrackedParameter<edm::InputTag>("RecHitsEETag",edm::InputTag("reducedEcalRecHitsEE"))),

      //      metIncludesMuons_(cfg.getUntrackedParameter<bool> ("METIncludesMuons", false)),
      // Main cuts 
      //elecTrig_(cfg.getUntrackedParameter<std::vector<std::string> > ("ElecTrig", "HLT_Ele10_SW_L1R")), 
      elecTrig_(cfg.getUntrackedParameter<std::vector<std::string> > ("ElecTrig")), 
      ptCut_(cfg.getUntrackedParameter<double>("PtCut", 10.)),
      etaCut_(cfg.getUntrackedParameter<double>("EtaCut", 2.4)),
      sieieCutBarrel_(cfg.getUntrackedParameter<double>("SieieBarrel", 0.01)),
      sieieCutEndcap_(cfg.getUntrackedParameter<double>("SieieEndcap", 0.028)),
      hoECutBarrel_(cfg.getUntrackedParameter<double>("HoEBarrel", 0.05)),
      hoECutEndcap_(cfg.getUntrackedParameter<double>("HoEEndcap", 0.025)),
      detainCutBarrel_(cfg.getUntrackedParameter<double>("DetainBarrel", 0.0071)),
      detainCutEndcap_(cfg.getUntrackedParameter<double>("DetainEndcap", 0.0066)),
      dphiinCutBarrel_(cfg.getUntrackedParameter<double>("DphiinBarrel", 0.0071)),
      dphiinCutEndcap_(cfg.getUntrackedParameter<double>("DphiinEndcap", 0.0066)),
//       dphiinCutBarrel_(cfg.getUntrackedParameter<double>("DphiinBarrel", 0.02)),
//       dphiinCutEndcap_(cfg.getUntrackedParameter<double>("DphiinEndcap", 0.04)),
//       hovereCutBarrel_(cfg.getUntrackedParameter<double>("HoverEBarrel", 0.05)),
//       hovereCutEndcap_(cfg.getUntrackedParameter<double>("HoverEEndcap", 0.025)),
      //      isRelativeIso_(cfg.getUntrackedParameter<bool>("IsRelativeIso", true)),
      //      isCombinedIso_(cfg.getUntrackedParameter<bool>("IsCombinedIso", false)),
      //      isoCut03_(cfg.getUntrackedParameter<double>("IsoCut03", 0.1)),
      ecalIsoCutBarrel_(cfg.getUntrackedParameter<double>("EcalIsoCutBarrel", 5.7)),
      ecalIsoCutEndcap_(cfg.getUntrackedParameter<double>("EcalIsoCutEndcap", 5.0)),
      hcalIsoCutBarrel_(cfg.getUntrackedParameter<double>("HcalIsoCutBarrel", 8.1)),
      hcalIsoCutEndcap_(cfg.getUntrackedParameter<double>("HcalIsoCutEndcap", 3.4)),
      trkIsoCutBarrel_(cfg.getUntrackedParameter<double>("TrkIsoCutBarrel", 7.2)),
      trkIsoCutEndcap_(cfg.getUntrackedParameter<double>("TrkIsoCutEndcap", 5.1)),
      mtMin_(cfg.getUntrackedParameter<double>("MtMin", -999999)),
      mtMax_(cfg.getUntrackedParameter<double>("MtMax", 999999.)),

      // Z criteria
      mZMin_(cfg.getUntrackedParameter<double>("MZMin",0.)),
      mZMax_(cfg.getUntrackedParameter<double>("MZMax",999999.)),

      // Jets
      jetCaloTag_(cfg.getUntrackedParameter<edm::InputTag> ("JetCaloTag", edm::InputTag("ak5CaloJets"))),
      //jetCaloTag_(cfg.getUntrackedParameter<edm::InputTag> ("JetCaloTag")),
      jetCaloCorrTag_(cfg.getUntrackedParameter<edm::InputTag> ("JetCaloCorrTag", edm::InputTag("L2L3CorJetAK5Calo"))),
      jetPfTag_(cfg.getUntrackedParameter<edm::InputTag> ("JetPfTag", edm::InputTag("ak5PFJets"))),
      jetPfCorrTag_(cfg.getUntrackedParameter<edm::InputTag> ("JetPfCorrTag", edm::InputTag("L2L3CorJetAK5PF"))),
      jetGenTag_(cfg.getUntrackedParameter<edm::InputTag> ("GenJetTag", edm::InputTag("ak5GenJets"))),
      minJetCaloPt_(cfg.getUntrackedParameter<double>("JetCaloPtCut", 10.)),
      minJetCaloCorrPt_(cfg.getUntrackedParameter<double>("JetCaloCorrPtCut", 10.)),
      minJetPfPt_(cfg.getUntrackedParameter<double>("JetPfPtCut", 10.)),
      minJetPfCorrPt_(cfg.getUntrackedParameter<double>("JetPfCorrPtCut", 10.)),
      minJetGenPt_(cfg.getUntrackedParameter<double>("JetGenPtCut", 10.)),
      useCorrCaloJets_(cfg.getUntrackedParameter<bool>("UseCorrCaloJets", true)),
      useCorrPfJets_(cfg.getUntrackedParameter<bool>("UseCorrPFJets", true)),
      useGenJets_(cfg.getUntrackedParameter<bool>("UseGenJets", true)),
      minJetElectronDr_(cfg.getUntrackedParameter<double>("MinJetElectronDr", 0.5)),

      //Met
      metCaloTag_(cfg.getUntrackedParameter<edm::InputTag> ("MetCaloTag", edm::InputTag("met"))),
      metCaloCorrTag_(cfg.getUntrackedParameter<edm::InputTag> ("MetCaloCorrTag", edm::InputTag("metJESCorAK5CaloJet"))),
      metPfTag_(cfg.getUntrackedParameter<edm::InputTag> ("MetPfTag", edm::InputTag("pfMet"))),
      metPfCorrTag_(cfg.getUntrackedParameter<edm::InputTag> ("MetPfCorrTag", edm::InputTag("pfMet"))),
      metTcTag_(cfg.getUntrackedParameter<edm::InputTag> ("MetTcTag", edm::InputTag("tcMet"))),
      metGenTag_(cfg.getUntrackedParameter<edm::InputTag> ("GenMetTag", edm::InputTag("genMet"))),
      metMin_(cfg.getUntrackedParameter<double>("MetMin", -999999.)),
      metMax_(cfg.getUntrackedParameter<double>("MetMax", 999999.)),
      useCorrCaloMet_(cfg.getUntrackedParameter<bool>("UseCorrCaloMet", true)),
      useCorrPfMet_(cfg.getUntrackedParameter<bool>("UseCorrPFMet", true)),
      useGenMet_(cfg.getUntrackedParameter<bool>("UseGenMet", true)),


      // Muon quality cuts
      //      dxyCut_(cfg.getUntrackedParameter<double>("DxyCut", 0.2)),
      //      normalizedChi2Cut_(cfg.getUntrackedParameter<double>("NormalizedChi2Cut", 10.)),
      //      trackerHitsCut_(cfg.getUntrackedParameter<int>("TrackerHitsCut", 11)),
      //      isAlsoTrackerMuon_(cfg.getUntrackedParameter<bool>("IsAlsoTrackerMuon", true)),

      // Z rejection
      //      ptThrForZ1_(cfg.getUntrackedParameter<double>("PtThrForZ1", 20.)),
      //      ptThrForZ2_(cfg.getUntrackedParameter<double>("PtThrForZ2", 10.)),

      // Top rejection
      eJetMin_(cfg.getUntrackedParameter<double>("EJetMin", 999999.)),
      nJetMax_(cfg.getUntrackedParameter<int>("NJetMax", 999999))//,
      
//       caloJetCollection_(cfg.getUntrackedParameter<edm:InputTag>("CaloJetCollection","sisCone5CaloJets"))

//      histFileName_(cfg.getUntrackedParameter<std::string>("HistFile","histos.root"))

{
  //  std::cout << "ENTERING CONSTRUCTOR" << std::endl;
  triggersOfInterest_ = new std::map<std::string,bool>;
  l1TriggersOfInterest_ = new std::map<std::string,bool>;

//   triggersOfInterest_ = new std::map<std::string,bool>;
//   l1TriggersOfInterest_ = new std::map<std::string,bool>;


  setTDRStyle();
  init_histograms();
}

<<<<<<< EwkElecStandaloneDQM.cc
EwkElecStandaloneDQM::~EwkElecStandaloneDQM() 
{
  delete triggersOfInterest_;
  delete l1TriggersOfInterest_;
}

=======
// EwkElecStandaloneDQM::~EwkElecStandaloneDQM()
// {
//   delete triggersOfInterest_;
//   delete l1TriggersOfInterest_;
// }


>>>>>>> 1.31
void EwkElecStandaloneDQM::beginRun(const Run& r, const EventSetup&) {
      nall = 0;
      nsel = 0;

      nrec = 0; 
      neid = 0;
      niso = 0; 
//       nhlt = 0; 
//       nmet = 0;
}


void EwkElecStandaloneDQM::beginJob() {
  //      theDbe = Service<DQMStore>().operator->();
  //      theDbe->setCurrentFolder("Physics/EwkElecDQM");

  //  histFile_ = new TFile(histFileName_);
  //  init_histograms();
}


void EwkElecStandaloneDQM::init_histograms() 
{

//   m_ttree = tFileService->make<TTree>("ttree", "ttree");
//   m_ttree->Branch("station", &m_ttree_station, "station/I");
//   m_ttree->Branch("chamber", &m_ttree_chamber, "chamber/I");
//   m_ttree->Branch("resid", &m_ttree_resid, "resid/F");
//   m_ttree->Branch("residslope", &m_ttree_residslope, "residslope/F");
//   m_ttree->Branch("phi", &m_ttree_phi, "phi/F");
//   m_ttree->Branch("qoverpt", &m_ttree_qoverpt, "qoverpt/F");

  if (makeTree_)
    {
      tree_ = fs->make<TTree>("EwkDqmTree","EwkDqmTree");

      //   tree_->Branch("",,"/");
      //  tree_->Branch("",,"/i");
      
      //      std::cout << "CREATING BRANCHES" << std::endl;
      tree_->Branch("runNumber",&runNumber_,"runNumber/i");
      tree_->Branch("lumiSection",&lumiSection_,"lumiSection/i");
      tree_->Branch("eventNumber",&eventNumber_,"eventNumber/i");
      tree_->Branch("ele_trig",&ele_trig_,"ele_trig/O"); 
      // how to deal with the different triggers in the ntuple?? vector of pairs?
      //tree_->Branch("triggersOfInterest",triggersOfInterest_,"triggersOfInterest"); // std::map somehow
      //tree_->Branch("triggersOfInterest","std::map<std::string,bool>",&triggersOfInterest_); 
      //      tree_->Branch("triggersOfInterest","std::map<std::string,bool>",triggersOfInterest_); // pointer // CURRENT
      //tree_->Branch("l1TriggersOfInterest",l1TriggersOfInterest_,"l1TriggersOfInterest"); //
      //tree_->Branch("l1TriggersOfInterest","std::map<std::string,bool>",&l1TriggersOfInterest_); //
      //      tree_->Branch("l1TriggersOfInterest","std::map<std::string,bool>",l1TriggersOfInterest_); // pointer // CURRENT
      // for now just separate vectors
//       tree_->Branch("hlTriggersNames_",hlTriggersNames_,"hlTriggersNames_/C"); // /C is character string terminated by the 0 character
//       tree_->Branch("l1TriggersNames_",l1TriggersNames_,"l1TriggersNames_/C"); // 
//       tree_->Branch("hlTriggersResults_",hlTriggersResults_,"hlTriggersResults_/O"); 
//       tree_->Branch("l1TriggersResults_",l1TriggersResults_,"l1TriggersResults_/O"); 


      tree_->Branch("hasWCand",&hasWCand_,"hasWCand/O"); 
      tree_->Branch("hasZCand",&hasZCand_,"hasZCand/O"); 
      
      tree_->Branch("ele_nelectrons",&ele_nelectrons_,"ele_nelectrons/I");
      tree_->Branch("ele_isEcalDriven",ele_isEcalDriven_,"ele_isEcalDriven[ele_nelectrons]/O"); // bool
      tree_->Branch("ele_isTrackerDriven",ele_isTrackerDriven_,"ele_isTrackerDriven[ele_nelectrons]/O"); 
      tree_->Branch("ele_isBarrel",ele_isBarrel_,"ele_isBarrel[ele_nelectrons]/O"); 
      tree_->Branch("ele_isEndcap",ele_isEndcap_,"ele_isEndcap[ele_nelectrons]/O"); 
      tree_->Branch("ele_eta",ele_eta_,"ele_eta[ele_nelectrons]/F");
      tree_->Branch("ele_phi",ele_phi_,"ele_phi[ele_nelectrons]/F");
      tree_->Branch("ele_dxy",ele_dxy_,"ele_dxy[ele_nelectrons]/F");
      tree_->Branch("ele_pt",ele_pt_,"ele_pt[ele_nelectrons]/F");
      tree_->Branch("ele_energy",ele_energy_,"ele_energy[ele_nelectrons]/F");
      tree_->Branch("ele_ecalenergy",ele_ecalenergy_,"ele_ecalenergy[ele_nelectrons]/F");
      tree_->Branch("ele_ecalenergyerror",ele_ecalenergyerror_,"ele_ecalenergyerror[ele_nelectrons]/F");
      tree_->Branch("ele_et",ele_et_,"ele_et[ele_nelectrons]/F");
      tree_->Branch("ele_p",ele_p_,"ele_p[ele_nelectrons]/F");
      tree_->Branch("ele_electronmomentumerror",ele_electronmomentumerror_,"ele_electronmomentumerror[ele_nelectrons]/F");
      tree_->Branch("ele_px",ele_px_,"ele_px[ele_nelectrons]/F");
      tree_->Branch("ele_py",ele_py_,"ele_py[ele_nelectrons]/F");
      tree_->Branch("ele_pz",ele_pz_,"ele_pz[ele_nelectrons]/F");
      
      //  tree_->Branch("ele_dxy",ele_dxy_,"ele_dxy[ele_nelectrons]/F");
      
      tree_->Branch("ele_deltaetaeleclustertrackatcalo",ele_deltaetaeleclustertrackatcalo_,"ele_deltaetaeleclustertrackatcalo[ele_nelectrons]/F");
      tree_->Branch("ele_deltaetaseedclustertrackatcalo",ele_deltaetaseedclustertrackatcalo_,"ele_deltaetaseedclustertrackatcalo[ele_nelectrons]/F");
      tree_->Branch("ele_deltaetasuperclustertrackatvtx",ele_deltaetasuperclustertrackatvtx_,"ele_deltaetasuperclustertrackatvtx[ele_nelectrons]/F");
      
      tree_->Branch("ele_deltaphieleclustertrackatcalo",ele_deltaphieleclustertrackatcalo_,"ele_deltaphieleclustertrackatcalo[ele_nelectrons]/F");
      tree_->Branch("ele_deltaphiseedclustertrackatcalo",ele_deltaphiseedclustertrackatcalo_,"ele_deltaphiseedclustertrackatcalo[ele_nelectrons]/F");
      tree_->Branch("ele_deltaphisuperclustertrackatvtx",ele_deltaphisuperclustertrackatvtx_,"ele_deltaphisuperclustertrackatvtx[ele_nelectrons]/F");
      
      tree_->Branch("ele_trackpositionatvtxx",ele_trackpositionatvtxx_,"ele_trackpositionatvtxx[ele_nelectrons]/F");
      tree_->Branch("ele_trackpositionatvtxy",ele_trackpositionatvtxy_,"ele_trackpositionatvtxy[ele_nelectrons]/F");
      tree_->Branch("ele_trackpositionatvtxz",ele_trackpositionatvtxz_,"ele_trackpositionatvtxz[ele_nelectrons]/F");
      
      tree_->Branch("ele_trackmomentumatvtxx",ele_trackmomentumatvtxx_,"ele_trackmomentumatvtxx[ele_nelectrons]/F");
      tree_->Branch("ele_trackmomentumatvtxy",ele_trackmomentumatvtxy_,"ele_trackmomentumatvtxy[ele_nelectrons]/F");
      tree_->Branch("ele_trackmomentumatvtxz",ele_trackmomentumatvtxz_,"ele_trackmomentumatvtxz[ele_nelectrons]/F");
      
      tree_->Branch("ele_trackpositionatcalox",ele_trackpositionatcalox_,"ele_trackpositionatcalox[ele_nelectrons]/F");
      tree_->Branch("ele_trackpositionatcaloy",ele_trackpositionatcaloy_,"ele_trackpositionatcaloy[ele_nelectrons]/F");
      tree_->Branch("ele_trackpositionatcaloz",ele_trackpositionatcaloz_,"ele_trackpositionatcaloz[ele_nelectrons]/F");
      
      tree_->Branch("ele_trackmomentumatcalox",ele_trackmomentumatcalox_,"ele_trackmomentumatcalox[ele_nelectrons]/F");
      tree_->Branch("ele_trackmomentumatcaloy",ele_trackmomentumatcaloy_,"ele_trackmomentumatcaloy[ele_nelectrons]/F");
      tree_->Branch("ele_trackmomentumatcaloz",ele_trackmomentumatcaloz_,"ele_trackmomentumatcaloz[ele_nelectrons]/F");
      
      tree_->Branch("ele_eeleclusteroverpout",ele_eeleclusteroverpout_,"ele_eeleclusteroverpout[ele_nelectrons]/F");
      tree_->Branch("ele_eseedclusteroverp",ele_eseedclusteroverp_,"ele_eseedclusteroverp[ele_nelectrons]/F");
      tree_->Branch("ele_eseedclusteroverpout",ele_eseedclusteroverpout_,"ele_eseedclusteroverpout[ele_nelectrons]/F");
      tree_->Branch("ele_esuperclusteroverp",ele_esuperclusteroverp_,"ele_esuperclusteroverp[ele_nelectrons]/F");
      
      tree_->Branch("ele_dr03ecalrechitsumet",ele_dr03ecalrechitsumet_,"ele_dr03ecalrechitsumet[ele_nelectrons]/F");
      tree_->Branch("ele_dr03hcaldepth1towersumet",ele_dr03hcaldepth1towersumet_,"ele_dr03hcaldepth1towersumet[ele_nelectrons]/F");
      tree_->Branch("ele_dr03hcaldepth2towersumet",ele_dr03hcaldepth2towersumet_,"ele_dr03hcaldepth2towersumet[ele_nelectrons]/F");
      tree_->Branch("ele_dr03hcaltowersumet",ele_dr03hcaltowersumet_,"ele_dr03hcaltowersumet[ele_nelectrons]/F");
      tree_->Branch("ele_dr03tksumpt",ele_dr03tksumpt_,"ele_dr03tksumpt[ele_nelectrons]/F");
      
      tree_->Branch("ele_dr04ecalrechitsumet",ele_dr04ecalrechitsumet_,"ele_dr04ecalrechitsumet[ele_nelectrons]/F");
      tree_->Branch("ele_dr04hcaldepth1towersumet",ele_dr04hcaldepth1towersumet_,"ele_dr04hcaldepth1towersumet[ele_nelectrons]/F");
      tree_->Branch("ele_dr04hcaldepth2towersumet",ele_dr04hcaldepth2towersumet_,"ele_dr04hcaldepth2towersumet[ele_nelectrons]/F");
      tree_->Branch("ele_dr04hcaltowersumet",ele_dr04hcaltowersumet_,"ele_dr04hcaltowersumet[ele_nelectrons]/F");
      tree_->Branch("ele_dr04tksumpt",ele_dr04tksumpt_,"ele_dr04tksumpt[ele_nelectrons]/F"); 
      
      tree_->Branch("ele_e1x5",ele_e1x5_,"ele_e1x5[ele_nelectrons]/F");
      tree_->Branch("ele_e2x5max",ele_e2x5max_,"ele_e2x5max[ele_nelectrons]/F");
      tree_->Branch("ele_e5x5",ele_e5x5_,"ele_e5x5[ele_nelectrons]/F");
      tree_->Branch("ele_r1x5",ele_r1x5_,"ele_r1x5[ele_nelectrons]/F");
      tree_->Branch("ele_r2x5max",ele_r2x5max_,"ele_r2x5max[ele_nelectrons]/F");
      tree_->Branch("ele_scpreshowerenergy",ele_scpreshowerenergy_,"ele_scpreshowerenergy[ele_nelectrons]/F");
      tree_->Branch("ele_scetawidth",ele_scetawidth_,"ele_scetawidth[ele_nelectrons]/F");
      tree_->Branch("ele_scphiwidth",ele_scphiwidth_,"ele_scphiwidth[ele_nelectrons]/F");
      tree_->Branch("ele_scr9",ele_scr9_,"ele_scr9[ele_nelectrons]/F");
      tree_->Branch("ele_sceseedoveresupercluster",ele_sceseedoveresupercluster_,"ele_sceseedoveresupercluster[ele_nelectrons]/F");
      tree_->Branch("ele_rmax3x3",ele_rmax3x3_,"ele_rmax3x3[ele_nelectrons]/F");
      
      tree_->Branch("ele_hcaldepth1overecal",ele_hcaldepth1overecal_,"ele_hcaldepth1overecal[ele_nelectrons]/F");
      tree_->Branch("ele_hcaldepth2overecal",ele_hcaldepth2overecal_,"ele_hcaldepth2overecal[ele_nelectrons]/F");
      tree_->Branch("ele_hcaloverecal",ele_hcaloverecal_,"ele_hcaloverecal[ele_nelectrons]/F");
      
      tree_->Branch("ele_sigmaetaeta",ele_sigmaetaeta_,"ele_sigmaetaeta[ele_nelectrons]/F");
      tree_->Branch("ele_sigmaietaieta",ele_sigmaietaieta_,"ele_sigmaietaieta[ele_nelectrons]/F");
      
      // bremstuff
      tree_->Branch("ele_basicclusterssize",ele_basicclusterssize_,"ele_basicclusterssize[ele_nelectrons]/F");
      tree_->Branch("ele_numberofbrems",ele_numberofbrems_,"ele_numberofbrems[ele_nelectrons]/F");
      tree_->Branch("ele_fbrem",ele_fbrem_,"ele_fbrem[ele_nelectrons]/F");
      
      // charge stuff
      tree_->Branch("ele_scpixcharge",ele_scpixcharge_,"ele_scpixcharge[ele_nelectrons]/F");
      tree_->Branch("ele_gsfcharge",ele_gsfcharge_,"ele_gsfcharge[ele_nelectrons]/F");
      tree_->Branch("ele_ctfcharge",ele_ctfcharge_,"ele_ctfcharge[ele_nelectrons]/F");
      tree_->Branch("ele_isgsfscpixchargeconsistent",ele_isgsfscpixchargeconsistent_,"ele_isgsfscpixchargeconsistent[ele_nelectrons]/F");
      tree_->Branch("ele_isgsfctfchargeconsistent",ele_isgsfctfchargeconsistent_,"ele_isgsfctfchargeconsistent[ele_nelectrons]/F");
      tree_->Branch("ele_isgsfctfscpixchargeconsistent",ele_isgsfctfscpixchargeconsistent_,"ele_isgsfctfscpixchargeconsistent[ele_nelectrons]/F");
      tree_->Branch("ele_charge",ele_charge_,"ele_charge[ele_nelectrons]/F");
      
      // vertex/beamspot stuff
      tree_->Branch("ele_vertexChi2",ele_vertexChi2_,"ele_vertexChi2[ele_nelectrons]/F");
      tree_->Branch("ele_vertexNdof",ele_vertexNdof_,"ele_vertexNdof[ele_nelectrons]/F");
      tree_->Branch("ele_vertexNormalizedChi2",ele_vertexNormalizedChi2_,"ele_vertexNormalizedChi2[ele_nelectrons]/F");
      tree_->Branch("ele_vx",ele_vx_,"ele_vx[ele_nelectrons]/F");
      tree_->Branch("ele_vy",ele_vy_,"ele_vy[ele_nelectrons]/F");
      tree_->Branch("ele_vz",ele_vz_,"ele_vz[ele_nelectrons]/F");
      /*   tree_->Branch("",ele_xBS_,""); */
      /*   tree_->Branch("",ele_yBS_,""); */
      /*   tree_->Branch("",ele_zBS_,""); */
      


      // supercluster stuff
      tree_->Branch("sc_nSuperClusters",&sc_nSuperClusters_,"sc_nSuperClusters/i");
      tree_->Branch("sc_nSuperClustersBarrel",&sc_nSuperClustersBarrel_,"sc_nSuperClustersBarrel/i");
      tree_->Branch("sc_nSuperClustersEndcap",&sc_nSuperClustersEndcap_,"sc_nSuperClustersEndcap/i");
      tree_->Branch("sc_isBarrel",sc_isBarrel_,"sc_isBarrel[sc_nSuperClusters]/O");
      tree_->Branch("sc_isEndcap",sc_isEndcap_,"sc_isEndcap[sc_nSuperClusters]/O");
      tree_->Branch("sc_energy",sc_energy_,"sc_energy[sc_nSuperClusters]/F");
      tree_->Branch("sc_eta",sc_eta_,"sc_eta[sc_nSuperClusters]/F");
      tree_->Branch("sc_phi",sc_phi_,"sc_phi[sc_nSuperClusters]/F");
      tree_->Branch("sc_rawEnergy",sc_rawEnergy_,"sc_rawEnergy[sc_nSuperClusters]/F");
      tree_->Branch("sc_preshowerEnergy",sc_preshowerEnergy_,"sc_preshowerEnergy[sc_nSuperClusters]/F"); // endcap-only!
      tree_->Branch("sc_etaWidth",sc_etaWidth_,"sc_etaWidth[sc_nSuperClusters]/F");
      tree_->Branch("sc_phiWidth",sc_phiWidth_,"sc_phiWidth[sc_nSuperClusters]/F");
      tree_->Branch("sc_nBasicClusters",sc_nBasicClusters_,"sc_nBasicClusters[sc_nSuperClusters]/I");
      tree_->Branch("sc_r9",sc_r9_,"sc_r9[sc_nSuperClusters]/F");
      tree_->Branch("sc_eSeedOverESuperCluster",sc_eSeedOverESuperCluster_,"sc_eSeedOverESuperCluster[sc_nSuperClusters]/F");

    }




  //  char chtitle[256] = "";
  //  char prefix[256] = "(GSF Electron) ";

  std::string chtitle;
  std::string collName = "(GSF Electron) ";
  std::string plotString;

  //      for (int i=0; i<2; ++i) {
  //  snprintf(chtitle, 255, "Muon transverse momentum (global muon) [GeV]");
  //  pt_before_ = new TH1F("PT_BEFORECUTS",chtitle,100,0.,100.);
  //  pt_after_ = new TH1F("PT_LASTCUT",chtitle,100,0.,100.);

  plotString = "Electron transverse momentum;p_{T} (GeV);";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_pt_before_ = fs->make<TH1F>("ELE_PT_BEFORECUTS",chtitle.data(),100,0.,100.);
  chtitle = collName+"after N-1 cuts: "+plotString;
  h_ele_pt_after_ = fs->make<TH1F>("ELE_PT_LASTCUT",chtitle.data(),100,0.,100.);
	
  plotString = "Electron transverse momentum for ECAL-driven electrons;p_{T} (GeV);";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_ptEcalDriven_before_ = fs->make<TH1F>("ELE_PTECALDRIVEN_BEFORECUTS",chtitle.data(),100,0.,100.);
  chtitle = collName+"after N-1 cuts: "+plotString;
  h_ele_ptEcalDriven_after_ = fs->make<TH1F>("ELE_PTECALDRIVEN_LASTCUT",chtitle.data(),100,0.,100.);
	
  plotString = "Electron transverse momentum for tracker-driven electrons;p_{T} (GeV)";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_ptTrackerDriven_before_ = fs->make<TH1F>("ELE_PTTRKDRIVEN_BEFORECUTS",chtitle.data(),100,0.,100.);
  chtitle = collName+"after N-1 cuts: "+plotString;
  h_ele_ptTrackerDriven_after_ = fs->make<TH1F>("ELE_PTTRKDRIVEN_LASTCUT",chtitle.data(),100,0.,100.);
	
  plotString = "Electron pseudo-rapidity;#eta;";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_eta_before_ = fs->make<TH1F>("ELE_ETA_BEFORECUTS",chtitle.data(),50,-2.5,2.5);
  chtitle = collName+"after N-1 cuts: "+plotString;
  h_ele_eta_after_ = fs->make<TH1F>("ELE_ETA_LASTCUT",chtitle.data(),50,-2.5,2.5);
	
  plotString = "Electron #phi;#phi;";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_phi_before_ = fs->make<TH1F>("ELE_PHI_BEFORECUTS",chtitle.data(),64,-3.2,3.2);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_phi_after_ = fs->make<TH1F>("ELE_PHI_AFTERCUTS",chtitle.data(),64,-3.2,3.2);
	
  plotString = "Electron #sigma_{i#etai#eta} (barrel);#sigma_{i#etai#eta};";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_sieiebarrel_before_ = fs->make<TH1F>("ELE_SIEIEBARREL_BEFORECUTS",chtitle.data(),70,0.,0.07);
  chtitle = collName+"after N-1 cuts: "+plotString;
  h_ele_sieiebarrel_after_ = fs->make<TH1F>("ELE_SIEIEBARREL_LASTCUT",chtitle.data(),70,0.,0.07);
	
  plotString = "Electron #sigma_{i#etai#eta} (endcap);#sigma_{i#etai#eta};";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_sieieendcap_before_ = fs->make<TH1F>("ELE_SIEIEENDCAP_BEFORECUTS",chtitle.data(),70,0.,0.07);
  chtitle = collName+"after N-1 cuts: "+plotString;
  h_ele_sieieendcap_after_ = fs->make<TH1F>("ELE_SIEIEENDCAP_LASTCUT",chtitle.data(),70,0.,0.07);
	
  plotString = "Electron H/E (barrel);H/E;";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_hoebarrel_before_ = fs->make<TH1F>("ELE_HOEBARREL_BEFORECUTS",chtitle.data(),50,0.,0.1);
  chtitle = collName+"after N-1 cuts: "+plotString;
  h_ele_hoebarrel_after_ = fs->make<TH1F>("ELE_HOEBARREL_LASTCUT",chtitle.data(),50,0.,0.1);
	
  plotString = "Electron H/E (endcap);H/E;";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_hoeendcap_before_ = fs->make<TH1F>("ELE_HOEENDCAP_BEFORECUTS",chtitle.data(),50,0.,0.1);
  chtitle = collName+"after N-1 cuts: "+plotString;
  h_ele_hoeendcap_after_ = fs->make<TH1F>("ELE_HOEENDCAP_LASTCUT",chtitle.data(),50,0.,0.1);
	
  plotString = "Electron #Delta#eta_{in} (barrel);#Delta#eta_{in};";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_detainbarrel_before_ = fs->make<TH1F>("ELE_DETAINBARREL_BEFORECUTS",chtitle.data(),40,-0.02,0.02);
  chtitle = collName+"after N-1 cuts: "+plotString;
  h_ele_detainbarrel_after_ = fs->make<TH1F>("ELE_DETAINBARREL_LASTCUT",chtitle.data(),40,-0.02,0.02);
	
  plotString = "Electron #Delta#eta_{in} (endcap);#Delta#eta_{in};";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_detainendcap_before_ = fs->make<TH1F>("ELE_DETAINENDCAP_BEFORECUTS",chtitle.data(),40,-0.02,0.02);
  chtitle = collName+"after N-1 cuts: "+plotString;
  h_ele_detainendcap_after_ = fs->make<TH1F>("ELE_DETAINENDCAP_LASTCUT",chtitle.data(),40,-0.02,0.02);
	
  plotString = "Electron #Delta#phi_{in} (barrel);#Delta#phi_{in};";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_dphiinbarrel_before_ = fs->make<TH1F>("ELE_DPHIINBARREL_BEFORECUTS",chtitle.data(),40,-0.1,0.1);
  chtitle = collName+"after N-1 cuts: "+plotString;
  h_ele_dphiinbarrel_after_ = fs->make<TH1F>("ELE_DPHIINBARREL_LASTCUT",chtitle.data(),40,-0.1,0.1);
	
  plotString = "Electron #Delta#phi_{in} (endcap);#Delta#phi_{in};";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_dphiinendcap_before_ = fs->make<TH1F>("ELE_DPHIINENDCAP_BEFORECUTS",chtitle.data(),40,-0.1,0.1);
  chtitle = collName+"after N-1 cuts: "+plotString;
  h_ele_dphiinendcap_after_ = fs->make<TH1F>("ELE_DPHIINENDCAP_LASTCUT",chtitle.data(),40,-0.1,0.1);
	
  plotString = "Electron transverse distance to beam spot [cm]";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_dxy_before_ = fs->make<TH1F>("ELE_DXY_BEFORECUTS",chtitle.data(),100,-0.5,0.5);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_dxy_after_ = fs->make<TH1F>("ELE_DXY_AFTERCUTS",chtitle.data(),100,-0.5,0.5);
	
  //  if (isRelativeIso_) {
  //        if (isCombinedIso_) {
  //              snprintf(chtitle, 255, "Relative (combined) isolation variable");
  //        } else {
  //              snprintf(chtitle, 255, "Relative (tracker) isolation variable");
  //        }
  //        iso_before_ = fs->make<TH1F>("ISO_BEFORECUTS",chtitle,100, 0., 1.);
  //        iso_after_ = fs->make<TH1F>("ISO_LASTCUT",chtitle,100, 0., 1.);
  //  } else {
  //        if (isCombinedIso_) {
  //              snprintf(chtitle, 255, "Absolute (combined) isolation variable [GeV]");
  //        } else {
  //              snprintf(chtitle, 255, "Absolute (tracker) isolation variable [GeV]");
  //        }
  //        iso_before_ = fs->make<TH1F>("ISO_BEFORECUTS",chtitle,100, 0., 20.);
  //        iso_after_ = fs->make<TH1F>("ISO_LASTCUT",chtitle,100, 0., 20.);
  //  }

  plotString = "Absolute ECAL isolation variable (barrel);ECAL isolation (GeV);";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_ecalisobarrel_before_ = fs->make<TH1F>("ELE_ECALISOBARREL_BEFORECUTS",chtitle.data(),50,0.,25.);
  chtitle = collName+"after N-1 cuts: "+plotString;
  h_ele_ecalisobarrel_after_ = fs->make<TH1F>("ELE_ECALISOBARREL_LASTCUT",chtitle.data(),50,0.,25.);
   
  plotString = "Absolute ECAL isolation variable (endcap);ECAL isolation (GeV);";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_ecalisoendcap_before_ = fs->make<TH1F>("ELE_ECALISOENDCAP_BEFORECUTS",chtitle.data(),50,0.,25.);
  chtitle = collName+"after N-1 cuts: "+plotString;
  h_ele_ecalisoendcap_after_ = fs->make<TH1F>("ELE_ECALISOENDCAP_LASTCUT",chtitle.data(),50,0.,25.);
  
  plotString = "Absolute HCAL isolation variable (barrel);HCAL isolation (GeV);";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_hcalisobarrel_before_ = fs->make<TH1F>("ELE_HCALISOBARREL_BEFORECUTS",chtitle.data(),50,0.,25.);
  chtitle = collName+"after N-1 cuts: "+plotString;
  h_ele_hcalisobarrel_after_ = fs->make<TH1F>("ELE_HCALISOBARREL_LASTCUT",chtitle.data(),50,0.,25.);
  
  plotString = "Absolute HCAL isolation variable (endcap);HCAL isolation (GeV);";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_hcalisoendcap_before_ = fs->make<TH1F>("ELE_HCALISOENDCAP_BEFORECUTS",chtitle.data(),50,0.,25.);
  chtitle = collName+"after N-1 cuts: "+plotString;
  h_ele_hcalisoendcap_after_ = fs->make<TH1F>("ELE_HCALISOENDCAP_LASTCUT",chtitle.data(),50,0.,25.);
  
  plotString = "Absolute track isolation variable (barrel);Track isolation (GeV);";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_trkisobarrel_before_ = fs->make<TH1F>("ELE_TRKISOBARREL_BEFORECUTS",chtitle.data(),50,0.,25.);
  chtitle = collName+"after N-1 cuts: "+plotString;
  h_ele_trkisobarrel_after_ = fs->make<TH1F>("ELE_TRKISOBARREL_LASTCUT",chtitle.data(),50,0.,25.);
  
  plotString = "Absolute track isolation variable (endcap);Track isolation (GeV);";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_trkisoendcap_before_ = fs->make<TH1F>("ELE_TRKISOENDCAP_BEFORECUTS",chtitle.data(),50,0.,25.);
  chtitle = collName+"after N-1 cuts: "+plotString;
  h_ele_trkisoendcap_after_ = fs->make<TH1F>("ELE_TRKISOENDCAP_LASTCUT",chtitle.data(),50,0.,25.);

  
  //snprintf(chtitle, 255, "Trigger response (bit %s)", elecTrig_.data());
  plotString = "Trigger response for selected EG trigger"; // elecTrig_ now a vector of strings
  chtitle = collName+"before cuts: "+plotString;
  h_ele_trig_before_ = fs->make<TH1F>("ELE_TRIG_BEFORECUTS",chtitle.data(),2,-0.5,1.5);
  chtitle = collName+"after N-1 cuts: "+plotString;
  h_ele_trig_after_ = fs->make<TH1F>("ELE_TRIG_LASTCUT",chtitle.data(),2,-0.5,1.5);
  
  plotString = "Trigger response for EG/Jet/MB triggers"; 
  chtitle = collName+"before cuts: "+plotString;
  h_ele_triggersFired_before_ = fs->make<TH1F>("ELE_TRIGGERSFIRED_BEFORECUTS",chtitle.data(),10,0.,10.);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_triggersFired_after_ = fs->make<TH1F>("ELE_TRIGGERSFIRED_AFTERCUTS",chtitle.data(),10,0.,10.);
  
  plotString = "Dielectron invariant mass;Invariant mass (GeV);";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_invmass_before_ = fs->make<TH1F>("ELE_INVMASS_BEFORECUTS",chtitle.data(),100,0.,200.);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_invmass_after_ = fs->make<TH1F>("ELE_INVMASS_AFTERCUTS",chtitle.data(),100,0.,200.);
  
  plotString = "Number of electrons in event;# electrons;";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_nelectrons_before_ = fs->make<TH1F>("ELE_NELECTRONS_BEFORECUTS",chtitle.data(),10,-0.5,9.5);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_nelectrons_after_ = fs->make<TH1F>("ELE_NELECTRONS_AFTERCUTS",chtitle.data(),10,-0.5,9.5);

  std::string mettag = metCaloTag_.label().data();
  plotString = "Transverse mass ("+mettag+");m_{T} (GeV);";
  chtitle = collName+"before cuts: "+plotString;
  h_mt_before_ = fs->make<TH1F>("MT_BEFORECUTS",chtitle.data(),150,0.,300.);
  chtitle = collName+"after N-1 cuts: "+plotString;
  h_mt_after_ = fs->make<TH1F>("MT_LASTCUT",chtitle.data(),150,0.,300.);
  
  mettag = metCaloTag_.label().data();
  plotString = "Missing transverse energy ("+mettag+");MET (GeV)";
  chtitle = collName+"before cuts: "+plotString;
  h_met_before_ = fs->make<TH1F>("MET_BEFORECUTS",chtitle.data(),100,0.,200.);
  chtitle = collName+"after N-1 cuts: "+plotString;
  h_met_after_ = fs->make<TH1F>("MET_LASTCUT",chtitle.data(),100,0.,200.);
  


  // trigger plots


  // DENOMINATORS

  // electron efficiency denom
  plotString = "electron E_{T} (eff denominator);E_{T} (GeV);"; 
  chtitle = "Trigger: "+plotString;
  h_trg_Denominator_eff_ = fs->make<TH1F>("TRG_DENOMINATOR_EFF",chtitle.data(),60,0.,60.);

  // electron efficiency denom
  plotString = "electron E_{T} (eff denominator) ecalDriven-only;E_{T} (GeV);"; 
  chtitle = "Trigger: "+plotString;
  h_trg_Denominator_ecalDriven_eff_ = fs->make<TH1F>("TRG_DENOMINATOR_ECALDRIVEN_EFF",chtitle.data(),60,0.,60.);

  // electron efficiency denom
  plotString = "electron E_{T} (eff denominator) passing all cuts;E_{T} (GeV);"; 
  chtitle = "Trigger: "+plotString;
  h_trg_Denominator_good_eff_ = fs->make<TH1F>("TRG_DENOMINATOR_GOOD_EFF",chtitle.data(),60,0.,60.);

  // supercluster efficiency denom
  plotString = "supercluster E_{T} (eff denominator);E_{T} (GeV);"; 
  chtitle = "Trigger: "+plotString;
  h_trg_Denominator_SC_eff_ = fs->make<TH1F>("TRG_DENOMINATOR_SC_EFF",chtitle.data(),60,0.,60.);

//   // photon efficiency denom


  // electron efficiency denom, barrel-only
  plotString = "electron E_{T} (eff denominator) barrel-only;E_{T} (GeV);"; 
  chtitle = "Trigger: "+plotString;
  h_trg_Denominator_barrel_eff_ = fs->make<TH1F>("TRG_DENOMINATOR_BARREL_EFF",chtitle.data(),60,0.,60.);

  // electron efficiency denom, barrel-only
  plotString = "electron E_{T} (eff denominator) barrel-only, ecalDriven-only;E_{T} (GeV);"; 
  chtitle = "Trigger: "+plotString;
  h_trg_Denominator_ecalDriven_barrel_eff_ = fs->make<TH1F>("TRG_DENOMINATOR_ECALDRIVEN_BARREL_EFF",chtitle.data(),60,0.,60.);

  // electron efficiency denom, barrel-only
  plotString = "electron E_{T} (eff denominator) barrel-only, passing all cuts;E_{T} (GeV);"; 
  chtitle = "Trigger: "+plotString;
  h_trg_Denominator_good_barrel_eff_ = fs->make<TH1F>("TRG_DENOMINATOR_GOOD_BARREL_EFF",chtitle.data(),60,0.,60.);

  // supercluster efficiency denom, barrel-only
  plotString = "supercluster E_{T} (eff denominator) barrel-only;E_{T} (GeV);"; 
  chtitle = "Trigger: "+plotString;
  h_trg_Denominator_SC_barrel_eff_ = fs->make<TH1F>("TRG_DENOMINATOR_SC_BARREL_EFF",chtitle.data(),60,0.,60.);


  // electron efficiency denom, endcap-only
  plotString = "electron E_{T} (eff denominator) endcap-only;E_{T} (GeV);"; 
  chtitle = "Trigger: "+plotString;
  h_trg_Denominator_endcap_eff_ = fs->make<TH1F>("TRG_DENOMINATOR_ENDCAP_EFF",chtitle.data(),60,0.,60.);

  // electron efficiency denom, endcap-only
  plotString = "electron E_{T} (eff denominator) endcap-only, ecalDriven-only;E_{T} (GeV);"; 
  chtitle = "Trigger: "+plotString;
  h_trg_Denominator_ecalDriven_endcap_eff_ = fs->make<TH1F>("TRG_DENOMINATOR_ECALDRIVEN_ENDCAP_EFF",chtitle.data(),60,0.,60.);

  // electron efficiency denom, endcap-only
  plotString = "electron E_{T} (eff denominator) endcap-only, passing all cuts;E_{T} (GeV);"; 
  chtitle = "Trigger: "+plotString;
  h_trg_Denominator_good_endcap_eff_ = fs->make<TH1F>("TRG_DENOMINATOR_GOOD_ENDCAP_EFF",chtitle.data(),60,0.,60.);

  // supercluster efficiency denom, endcap-only
  plotString = "supercluster E_{T} (eff denominator) endcap-only;E_{T} (GeV);"; 
  chtitle = "Trigger: "+plotString;
  h_trg_Denominator_SC_endcap_eff_ = fs->make<TH1F>("TRG_DENOMINATOR_SC_ENDCAP_EFF",chtitle.data(),60,0.,60.);



<<<<<<< EwkElecStandaloneDQM.cc

  // NUMERATORS

  // electron triggers
  plotString = "response vs. electron E_{T} for HLT_L1SINGLEEG5;E_{T} (GeV);"; 
=======


  // ALL ELECTRONS

  // electron
  plotString = "response vs. electron p_{T} for HLT_L1SINGLEEG5;p_{T} (GeV);"; 
>>>>>>> 1.31
  chtitle = "Trigger: "+plotString;
  h_trg_HLT_L1SingleEG5_eff_ = fs->make<TH1F>("TRG_HLT_L1SINGLEEG5_EFF",chtitle.data(),60,0.,60.);

  plotString = "response vs. electron E_{T} for HLT_L1SINGLEEG8;E_{T} (GeV);"; 
  chtitle = "Trigger: "+plotString;
  h_trg_HLT_L1SingleEG8_eff_ = fs->make<TH1F>("TRG_HLT_L1SINGLEEG8_EFF",chtitle.data(),60,0.,60.);

  plotString = "response vs. electron E_{T} for HLT_Ele10_LW_L1R;E_{T} (GeV);"; 
  chtitle = "Trigger: "+plotString;
  h_trg_HLT_Ele10_LW_L1R_eff_ = fs->make<TH1F>("TRG_HLT_ELE10_LW_L1R_EFF",chtitle.data(),60,0.,60.);

  plotString = "response vs. electron E_{T} for HLT_Ele10_LW_EleId_L1R;E_{T} (GeV);"; 
  chtitle = "Trigger: "+plotString;
  h_trg_HLT_Ele10_LW_EleId_L1R_eff_ = fs->make<TH1F>("TRG_HLT_ELE10_LW_ELEID_L1R_EFF",chtitle.data(),60,0.,60.);

  plotString = "response vs. electron E_{T} for HLT_Ele15_LW_L1R;E_{T} (GeV);"; 
  chtitle = "Trigger: "+plotString;
  h_trg_HLT_Ele15_LW_L1R_eff_ = fs->make<TH1F>("TRG_HLT_ELE15_LW_L1R_EFF",chtitle.data(),60,0.,60.);

  plotString = "response vs. electron E_{T} for HLT_Ele15_SiStrip_L1R;E_{T} (GeV);"; 
  chtitle = "Trigger: "+plotString;
  h_trg_HLT_Ele15_SiStrip_L1R_eff_ = fs->make<TH1F>("TRG_HLT_ELE15_SISTRIP_L1R_EFF",chtitle.data(),60,0.,60.);

  plotString = "response vs. electron E_{T} for HLT_Ele20_LW_L1R;E_{T} (GeV);"; 
  chtitle = "Trigger: "+plotString;
  h_trg_HLT_Ele20_LW_L1R_eff_ = fs->make<TH1F>("TRG_HLT_ELE20_LW_L1R_EFF",chtitle.data(),60,0.,60.);


  // photon triggers
  plotString = "response vs. electron E_{T} for HLT_Photon10_L1R;E_{T} (GeV);"; 
  chtitle = "Trigger: "+plotString;
  h_trg_HLT_Photon10_L1R_eff_ = fs->make<TH1F>("TRG_HLT_PHOTON10_L1R_EFF",chtitle.data(),60,0.,60.);

  plotString = "response vs. electron E_{T} for HLT_Photon15_L1R;E_{T} (GeV);"; 
  chtitle = "Trigger: "+plotString;
  h_trg_HLT_Photon15_L1R_eff_ = fs->make<TH1F>("TRG_HLT_PHOTON15_L1R_EFF",chtitle.data(),60,0.,60.);

  plotString = "response vs. electron E_{T} for HLT_Photon15_TrackIso_L1R;E_{T} (GeV);"; 
  chtitle = "Trigger: "+plotString;
  h_trg_HLT_Photon15_TrackIso_L1R_eff_ = fs->make<TH1F>("TRG_HLT_PHOTON15_TRACKISO_L1R_EFF",chtitle.data(),60,0.,60.);

  plotString = "response vs. electron E_{T} for HLT_Photon15_LooseEcalIso_L1R;E_{T} (GeV);"; 
  chtitle = "Trigger: "+plotString;
  h_trg_HLT_Photon15_LooseEcalIso_L1R_eff_ = fs->make<TH1F>("TRG_HLT_PHOTON15_LOOSEECALISO_L1R_EFF",chtitle.data(),60,0.,60.);

  plotString = "response vs. electron E_{T} for HLT_Photon20_L1R;E_{T} (GeV);"; 
  chtitle = "Trigger: "+plotString;
  h_trg_HLT_Photon20_L1R_eff_ = fs->make<TH1F>("TRG_HLT_PHOTON20_L1R_EFF",chtitle.data(),60,0.,60.);

  plotString = "response vs. electron E_{T} for HLT_Photon30_L1R;E_{T} (GeV);"; 
  chtitle = "Trigger: "+plotString;
  h_trg_HLT_Photon30_L1R_eff_ = fs->make<TH1F>("TRG_HLT_PHOTON30_L1R_EFF",chtitle.data(),60,0.,60.);


  // L1 triggers

  plotString = "response vs. electron E_{T} for L1_SingleIsoEG5;E_{T} (GeV);"; 
  chtitle = "Trigger: "+plotString;
  h_trg_L1_SingleIsoEG5_eff_ = fs->make<TH1F>("TRG_L1_SINGLEISOEG5_EFF",chtitle.data(),60,0.,60.);

  plotString = "response vs. electron E_{T} for L1_SingleIsoEG8;E_{T} (GeV);"; 
  chtitle = "Trigger: "+plotString;
  h_trg_L1_SingleIsoEG8_eff_ = fs->make<TH1F>("TRG_L1_SINGLEISOEG8_EFF",chtitle.data(),60,0.,60.);

  plotString = "response vs. electron E_{T} for L1_SingleIsoEG10;E_{T} (GeV);"; 
  chtitle = "Trigger: "+plotString;
  h_trg_L1_SingleIsoEG10_eff_ = fs->make<TH1F>("TRG_L1_SINGLEISOEG10_EFF",chtitle.data(),60,0.,60.);

  plotString = "response vs. electron E_{T} for L1_SingleIsoEG12;E_{T} (GeV);"; 
  chtitle = "Trigger: "+plotString;
  h_trg_L1_SingleIsoEG12_eff_ = fs->make<TH1F>("TRG_L1_SINGLEISOEG12_EFF",chtitle.data(),60,0.,60.);

  plotString = "response vs. electron E_{T} for L1_SingleIsoEG15;E_{T} (GeV);"; 
  chtitle = "Trigger: "+plotString;
  h_trg_L1_SingleIsoEG15_eff_ = fs->make<TH1F>("TRG_L1_SINGLEISOEG15_EFF",chtitle.data(),60,0.,60.);

  plotString = "response vs. electron E_{T} for L1_SingleEG2;E_{T} (GeV);"; 
  chtitle = "Trigger: "+plotString;
  h_trg_L1_SingleEG2_eff_ = fs->make<TH1F>("TRG_L1_SINGLEEG2_EFF",chtitle.data(),60,0.,60.);

  plotString = "response vs. electron E_{T} for L1_SingleEG5;E_{T} (GeV);"; 
  chtitle = "Trigger: "+plotString;
  h_trg_L1_SingleEG5_eff_ = fs->make<TH1F>("TRG_L1_SINGLEEG5_EFF",chtitle.data(),60,0.,60.);

<<<<<<< EwkElecStandaloneDQM.cc






=======



  // GOOD ELECTRONS

  // electron
  plotString = "response vs. electron p_{T} for HLT_L1SINGLEEG5, good electrons only;p_{T} (GeV);"; 
  chtitle = "Trigger: "+plotString;
  h_trg_HLT_L1SingleEG5_eff_goodEle_ = fs->make<TH1F>("TRG_HLT_L1SINGLEEG5_EFF_GOODELE",chtitle.data(),60,0.,60.);
//   chtitle = collName+"after N-1 cuts: "+plotString;
//   trg_trig_after_ = fs->make<TH1F>("ELE_TRIG_LASTCUT",chtitle.data(),2,-0.5,1.5);

  plotString = "response vs. electron p_{T} for HLT_L1SINGLEEG8, good electrons only;p_{T} (GeV);"; 
  chtitle = "Trigger: "+plotString;
  h_trg_HLT_L1SingleEG8_eff_goodEle_ = fs->make<TH1F>("TRG_HLT_L1SINGLEEG8_EFF_GOODELE",chtitle.data(),60,0.,60.);

  plotString = "response vs. electron p_{T} for HLT_Ele10_LW_L1R, good electrons only;p_{T} (GeV);"; 
  chtitle = "Trigger: "+plotString;
  h_trg_HLT_Ele10_LW_L1R_eff_goodEle_ = fs->make<TH1F>("TRG_HLT_ELE10_LW_L1R_EFF_GOODELE",chtitle.data(),60,0.,60.);

  plotString = "response vs. electron p_{T} for HLT_Ele10_LW_EleId_L1R, good electrons only;p_{T} (GeV);"; 
  chtitle = "Trigger: "+plotString;
  h_trg_HLT_Ele10_LW_EleId_L1R_eff_goodEle_ = fs->make<TH1F>("TRG_HLT_ELE10_LW_ELEID_L1R_EFF_GOODELE",chtitle.data(),60,0.,60.);

  plotString = "response vs. electron p_{T} for HLT_Ele15_LW_L1R, good electrons only;p_{T} (GeV);"; 
  chtitle = "Trigger: "+plotString;
  h_trg_HLT_Ele15_LW_L1R_eff_goodEle_ = fs->make<TH1F>("TRG_HLT_ELE15_LW_L1R_EFF_GOODELE",chtitle.data(),60,0.,60.);

  plotString = "response vs. electron p_{T} for HLT_Ele15_SiStrip_L1R, good electrons only;p_{T} (GeV);"; 
  chtitle = "Trigger: "+plotString;
  h_trg_HLT_Ele15_SiStrip_L1R_eff_goodEle_ = fs->make<TH1F>("TRG_HLT_ELE15_SISTRIP_L1R_EFF_GOODELE",chtitle.data(),60,0.,60.);

  plotString = "response vs. electron p_{T} for HLT_Ele20_LW_L1R, good electrons only;p_{T} (GeV);"; 
  chtitle = "Trigger: "+plotString;
  h_trg_HLT_Ele20_LW_L1R_eff_goodEle_ = fs->make<TH1F>("TRG_HLT_ELE20_LW_L1R_EFF_GOODELE",chtitle.data(),60,0.,60.);


  // photon  
  plotString = "response vs. electron p_{T} for HLT_Photon10_L1R, good electrons only;p_{T} (GeV);"; 
  chtitle = "Trigger: "+plotString;
  h_trg_HLT_Photon10_L1R_eff_goodEle_ = fs->make<TH1F>("TRG_HLT_PHOTON10_L1R_EFF_GOODELE",chtitle.data(),60,0.,60.);

  plotString = "response vs. electron p_{T} for HLT_Photon15_L1R, good electrons only;p_{T} (GeV);"; 
  chtitle = "Trigger: "+plotString;
  h_trg_HLT_Photon15_L1R_eff_goodEle_ = fs->make<TH1F>("TRG_HLT_PHOTON15_L1R_EFF_GOODELE",chtitle.data(),60,0.,60.);

  plotString = "response vs. electron p_{T} for HLT_Photon15_TrackIso_L1R, good electrons only;p_{T} (GeV);"; 
  chtitle = "Trigger: "+plotString;
  h_trg_HLT_Photon15_TrackIso_L1R_eff_goodEle_ = fs->make<TH1F>("TRG_HLT_PHOTON15_TRACKISO_L1R_EFF_GOODELE",chtitle.data(),60,0.,60.);

  plotString = "response vs. electron p_{T} for HLT_Photon15_LooseEcalIso_L1R, good electrons only"; 
  chtitle = "Trigger: "+plotString;
  h_trg_HLT_Photon15_LooseEcalIso_L1R_eff_goodEle_ = fs->make<TH1F>("TRG_HLT_PHOTON15_LOOSEECALISO_L1R_EFF_GOODELE",chtitle.data(),60,0.,60.);

  plotString = "response vs. electron p_{T} for HLT_Photon20_L1R, good electrons only;p_{T} (GeV);"; 
  chtitle = "Trigger: "+plotString;
  h_trg_HLT_Photon20_L1R_eff_goodEle_ = fs->make<TH1F>("TRG_HLT_PHOTON20_L1R_EFF_GOODELE",chtitle.data(),60,0.,60.);

  plotString = "response vs. electron p_{T} for HLT_Photon30_L1R, good electrons only;p_{T} (GeV);"; 
  chtitle = "Trigger: "+plotString;
  h_trg_HLT_Photon30_L1R_eff_goodEle_ = fs->make<TH1F>("TRG_HLT_PHOTON30_L1R_EFF_GOODELE",chtitle.data(),60,0.,60.);


  // L1 triggers

  plotString = "response vs. electron p_{T} for L1_SingleIsoEG5, good electrons only;p_{T} (GeV);"; 
  chtitle = "Trigger: "+plotString;
  h_trg_L1_SingleIsoEG5_eff_goodEle_ = fs->make<TH1F>("TRG_L1_SINGLEISOEG5_EFF_GOODELE",chtitle.data(),60,0.,60.);

  plotString = "response vs. electron p_{T} for L1_SingleIsoEG8, good electrons only;p_{T} (GeV);"; 
  chtitle = "Trigger: "+plotString;
  h_trg_L1_SingleIsoEG8_eff_goodEle_ = fs->make<TH1F>("TRG_L1_SINGLEISOEG8_EFF_GOODELE",chtitle.data(),60,0.,60.);

  plotString = "response vs. electron p_{T} for L1_SingleIsoEG10, good electrons only;p_{T} (GeV);"; 
  chtitle = "Trigger: "+plotString;
  h_trg_L1_SingleIsoEG10_eff_goodEle_ = fs->make<TH1F>("TRG_L1_SINGLEISOEG10_EFF_GOODELE",chtitle.data(),60,0.,60.);

  plotString = "response vs. electron p_{T} for L1_SingleIsoEG12, good electrons only;p_{T} (GeV);"; 
  chtitle = "Trigger: "+plotString;
  h_trg_L1_SingleIsoEG12_eff_goodEle_ = fs->make<TH1F>("TRG_L1_SINGLEISOEG12_EFF_GOODELE",chtitle.data(),60,0.,60.);

  plotString = "response vs. electron p_{T} for L1_SingleIsoEG15, good electrons only;p_{T} (GeV);"; 
  chtitle = "Trigger: "+plotString;
  h_trg_L1_SingleIsoEG15_eff_goodEle_ = fs->make<TH1F>("TRG_L1_SINGLEISOEG15_EFF_GOODELE",chtitle.data(),60,0.,60.);

  plotString = "response vs. electron p_{T} for L1_SingleEG2, good electrons only;p_{T} (GeV);"; 
  chtitle = "Trigger: "+plotString;
  h_trg_L1_SingleEG2_eff_goodEle_ = fs->make<TH1F>("TRG_L1_SINGLEEG2_EFF_GOODELE",chtitle.data(),60,0.,60.);

  plotString = "response vs. electron p_{T} for L1_SingleEG5, good electrons only;p_{T} (GeV);"; 
  chtitle = "Trigger: "+plotString;
  h_trg_L1_SingleEG5_eff_goodEle_ = fs->make<TH1F>("TRG_L1_SINGLEEG5_EFF_GOODELE",chtitle.data(),60,0.,60.);

  plotString = "response vs. electron p_{T} for L1_SingleEG8, good electrons only;p_{T} (GeV);"; 
  chtitle = "Trigger: "+plotString;
  h_trg_L1_SingleEG8_eff_goodEle_ = fs->make<TH1F>("TRG_L1_SINGLEEG8_EFF_GOODELE",chtitle.data(),60,0.,60.);

  plotString = "response vs. electron p_{T} for L1_SingleEG10, good electrons only;p_{T} (GeV);"; 
  chtitle = "Trigger: "+plotString;
  h_trg_L1_SingleEG10_eff_goodEle_ = fs->make<TH1F>("TRG_L1_SINGLEEG10_EFF_GOODELE",chtitle.data(),60,0.,60.);

  plotString = "response vs. electron p_{T} for L1_SingleEG12, good electrons only;p_{T} (GeV);"; 
  chtitle = "Trigger: "+plotString;
  h_trg_L1_SingleEG12_eff_goodEle_ = fs->make<TH1F>("TRG_L1_SINGLEEG12_EFF_GOODELE",chtitle.data(),60,0.,60.);

  plotString = "response vs. electron p_{T} for L1_SingleEG15, good electrons only;p_{T} (GeV);"; 
  chtitle = "Trigger: "+plotString;
  h_trg_L1_SingleEG15_eff_goodEle_ = fs->make<TH1F>("TRG_L1_SINGLEEG15_EFF_GOODELE",chtitle.data(),60,0.,60.);

  plotString = "response vs. electron p_{T} for L1_SingleEG20, good electrons only;p_{T} (GeV);"; 
  chtitle = "Trigger: "+plotString;
  h_trg_L1_SingleEG20_eff_goodEle_ = fs->make<TH1F>("TRG_L1_SINGLEEG20_EFF_GOODELE",chtitle.data(),60,0.,60.);





>>>>>>> 1.31
  /////////////////// special
  plotString = "electron E_{T} vs triggers fired;E_{T} (GeV);"; 
  chtitle = "Trigger: "+plotString;
  h_trg_highestEtVsThreshold_ = fs->make<TH2F>("TRG_HIGHESTETVSTHRESHOLD",chtitle.data(),20,0.,20.,60,0.,60.);

  plotString = "response vs. electron E_{T} for L1_SingleEG5: not fired because result = false;E_{T} (GeV);"; 
  chtitle = "Trigger: "+plotString;
  h_trg_L1_SingleEG5_eff_notfire1_ = fs->make<TH1F>("TRG_L1_SINGLEEG5_EFF_NOTFIRE1",chtitle.data(),60,0.,60.);

  plotString = "response vs. electron E_{T} for L1_SingleEG5: not fired because trigger not in triggersOfInterest;E_{T} (GeV);"; 
  chtitle = "Trigger: "+plotString;
  h_trg_L1_SingleEG5_eff_notfire2_ = fs->make<TH1F>("TRG_L1_SINGLEEG5_EFF_NOTFIRE2",chtitle.data(),60,0.,60.);

  plotString = "response vs. electron E_{T} for L1_SingleEG5, barrel-only;E_{T} (GeV);"; 
  chtitle = "Trigger: "+plotString;
  h_trg_L1_SingleEG5_eff_barrel_ = fs->make<TH1F>("TRG_L1_SINGLEEG5_EFF_BARREL",chtitle.data(),60,0.,60.);

  plotString = "response vs. electron E_{T} for L1_SingleEG5, barrel-only;E_{T} (GeV);"; 
  chtitle = "Trigger: "+plotString;
  h_trg_Denominator_eff_barrel_ = fs->make<TH1F>("TRG_DENOMINATOR_EFF_BARREL",chtitle.data(),60,0.,60.);

  plotString = "response vs. electron E_{T} for L1_SingleEG5, ECAL-driven electrons only;E_{T} (GeV);"; 
  chtitle = "Trigger: "+plotString;
  h_trg_L1_SingleEG5_eff_ecalDriven_ = fs->make<TH1F>("TRG_L1_SINGLEEG5_EFF_ECALDRIVEN",chtitle.data(),60,0.,60.);

  plotString = "response vs. electron E_{T} for L1_SingleEG5, ECAL-driven electrons only;E_{T} (GeV);"; 
  chtitle = "Trigger: "+plotString;
  h_trg_Denominator_eff_ecalDriven_ = fs->make<TH1F>("TRG_DENOMINATOR_EFF_ECALDRIVEN",chtitle.data(),60,0.,60.);

  plotString = "response vs. electron E_{T} for L1_SingleEG5, good electrons only;E_{T} (GeV);"; 
  chtitle = "Trigger: "+plotString;
  h_trg_L1_SingleEG5_eff_goodEle_ = fs->make<TH1F>("TRG_L1_SINGLEEG5_EFF_GOODELE",chtitle.data(),60,0.,60.);

  plotString = "response vs. electron E_{T} for L1_SingleEG5, good electrons only;E_{T} (GeV);"; 
  chtitle = "Trigger: "+plotString;
  h_trg_Denominator_eff_goodEle_ = fs->make<TH1F>("TRG_DENOMINATOR_EFF_GOODELE",chtitle.data(),60,0.,60.);

  // eta
  plotString = "denominator for #eta;#eta;"; 
  chtitle = "Trigger: "+plotString;
  h_trg_Denominator_eff_eta_ = fs->make<TH1F>("TRG_DENOMINATOR_EFF_ETA",chtitle.data(),64,-3.2,3.2);

  plotString = "response vs. electron #eta for L1_SingleEG5;#eta;"; 
  chtitle = "Trigger: "+plotString;
  h_trg_L1_SingleEG5_eff_eta_ = fs->make<TH1F>("TRG_L1_SINGLEEG5_EFF_ETA",chtitle.data(),64,-3.2,3.2);

  plotString = "response vs. electron #eta for L1_SingleEG5 (for electrons with E_{T} > 5 Gev);#eta;"; 
  chtitle = "Trigger: "+plotString;
  h_trg_L1_SingleEG5_eff_eta_cutEt5_ = fs->make<TH1F>("TRG_L1_SINGLEEG5_EFF_ETA_CUTET5",chtitle.data(),64,-3.2,3.2);

  plotString = "response vs. electron #eta for L1_SingleEG5 (for electrons with E_{T} > 5 Gev);#eta;"; 
  chtitle = "Trigger: "+plotString;
  h_trg_Denominator_eff_eta_cutEt5_ = fs->make<TH1F>("TRG_DENOMINATOR_EFF_ETA_CUTET5",chtitle.data(),64,-3.2,3.2);

  plotString = "response vs. electron #eta for L1_SingleEG5: not fired because result = false;#eta;"; 
  chtitle = "Trigger: "+plotString;
  h_trg_L1_SingleEG5_eff_eta_notfire1_ = fs->make<TH1F>("TRG_L1_SINGLEEG5_EFF_ETA_NOTFIRE1",chtitle.data(),64,-3.2,3.2);

  plotString = "response vs. electron #eta for L1_SingleEG5: not fired because trigger not in triggersOfInterest;#eta;"; 
  chtitle = "Trigger: "+plotString;
  h_trg_L1_SingleEG5_eff_eta_notfire2_ = fs->make<TH1F>("TRG_L1_SINGLEEG5_EFF_ETA_NOTFIRE2",chtitle.data(),64,-3.2,3.2);

  // pt and eta
  plotString = "eff of #eta and electron E_{T} denominator;#eta;E_{T} (GeV)"; 
  chtitle = "Trigger: "+plotString;
  h_trg_Denominator_eff_etaEt_ = fs->make<TH2F>("TRG_DENOMINATOR_EFF_ETAET",chtitle.data(),64,-3.2,3.2,60,0.,60.);

  plotString = "eff of #eta and electron E_{T} for L1_SingleEG5;#eta;E_{T} (GeV)"; 
  chtitle = "Trigger: "+plotString;
  h_trg_L1_SingleEG5_eff_etaEt_ = fs->make<TH2F>("TRG_L1_SINGLEEG5_EFF_ETAET",chtitle.data(),64,-3.2,3.2,60,0.,60.);
  ///////////////////////end special

  plotString = "response vs. electron E_{T} for L1_SingleEG8;E_{T} (GeV);"; 
  chtitle = "Trigger: "+plotString;
  h_trg_L1_SingleEG8_eff_ = fs->make<TH1F>("TRG_L1_SINGLEEG8_EFF",chtitle.data(),60,0.,60.);

  plotString = "response vs. electron E_{T} for L1_SingleEG10;E_{T} (GeV);"; 
  chtitle = "Trigger: "+plotString;
  h_trg_L1_SingleEG10_eff_ = fs->make<TH1F>("TRG_L1_SINGLEEG10_EFF",chtitle.data(),60,0.,60.);

  plotString = "response vs. electron E_{T} for L1_SingleEG12;E_{T} (GeV);"; 
  chtitle = "Trigger: "+plotString;
  h_trg_L1_SingleEG12_eff_ = fs->make<TH1F>("TRG_L1_SINGLEEG12_EFF",chtitle.data(),60,0.,60.);

  plotString = "response vs. electron E_{T} for L1_SingleEG15;E_{T} (GeV);"; 
  chtitle = "Trigger: "+plotString;
  h_trg_L1_SingleEG15_eff_ = fs->make<TH1F>("TRG_L1_SINGLEEG15_EFF",chtitle.data(),60,0.,60.);

  plotString = "response vs. electron E_{T} for L1_SingleEG20;E_{T} (GeV);"; 
  chtitle = "Trigger: "+plotString;
  h_trg_L1_SingleEG20_eff_ = fs->make<TH1F>("TRG_L1_SINGLEEG20_EFF",chtitle.data(),60,0.,60.);





  // More detailed GSF electron quantities
  // energies

  // TWO BELOW NOT ACTUALLY USED CURRENTLY!!  according to twiki CMS.SWGuideGsfElectronObject 29mar10
  // ACTUALLY, the plots aren't just zero!!!  So put them back in
  // SC energy, corrected if correction has been applied
  chtitle = collName+"before cuts: "+"ECAL energy of electron supercluster;SC energy (GeV);";
  h_ele_ecalEnergy_before_ = fs->make<TH1F>("ELE_ECALENERGY_BEFORECUTS",chtitle.data(),100,0.,100.);
  chtitle = collName+"after cuts: "+"ECAL energy of electron supercluster;SC energy (GeV);";
  h_ele_ecalEnergy_after_ = fs->make<TH1F>("ELE_ECALENERGY_AFTERCUTS",chtitle.data(),100,0.,100.);
  
  // error on SC energy 
  chtitle = collName+"before cuts: "+"ECAL energy error;energy error (GeV);";
  h_ele_ecalEnergyError_before_ = fs->make<TH1F>("ELE_ECALENERGYERROR_BEFORECUTS",chtitle.data(),100,0.,5.);
  chtitle = collName+"after cuts: "+"ECAL energy error;energy error (GeV);";
  h_ele_ecalEnergyError_after_ = fs->make<TH1F>("ELE_ECALENERGYERROR_AFTERCUTS",chtitle.data(),100,0.,5.);
  


  // the electron cluster eta - track eta position at the PCA to the electron cluster, extrapolated from the outermost state
  plotString = "#Delta#eta between electron cluster and track;#Delta#eta;";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_deltaEtaEleClusterTrackAtCalo_before_ = fs->make<TH1F>("ELE_DELTAETAELECLUSTERTRACKATCALO_BEFORECUTS",chtitle.data(),40,-0.2,0.2);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_deltaEtaEleClusterTrackAtCalo_after_ = fs->make<TH1F>("ELE_DELTAETAELECLUSTERTRACKATCALO_AFTERCUTS",chtitle.data(),40,-0.2,0.2);

  // the seed cluster eta - track eta position at the PCA to the seed cluster, extrapolated from the outermost track state.
  plotString = "#Delta#eta between seed cluster and track;#Delta#eta;";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_deltaEtaSeedClusterTrackAtCalo_before_ = fs->make<TH1F>("ELE_DELTAETASEEDCLUSTERTRACKATCALO_BEFORECUTS",chtitle.data(),40,-0.2,0.2);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_deltaEtaSeedClusterTrackAtCalo_after_ = fs->make<TH1F>("ELE_DELTAETASEEDCLUSTERTRACKATCALO_AFTERCUTS",chtitle.data(),40,-0.2,0.2);

  // the supercluster eta - track eta position at the PCA to the supercluster, extrapolated from the innermost track state.
  plotString = "#Delta#eta between supercluster and track;#Delta#eta;";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_deltaEtaSuperClusterTrackAtVtx_before_ = fs->make<TH1F>("ELE_DELTAETASUPERCLUSTERTRACKATVTX_BEFORECUTS",chtitle.data(),40,-0.2,0.2);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_deltaEtaSuperClusterTrackAtVtx_after_ = fs->make<TH1F>("ELE_DELTAETASUPERCLUSTERTRACKATVTX_AFTERCUTS",chtitle.data(),40,-0.2,0.2);

	
  // the electron cluster phi - track phi position at the PCA to the electron cluster, extrapolated from the outermost state
  plotString = "#Delta#phi between electron cluster and track;#Delta#phi;";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_deltaPhiEleClusterTrackAtCalo_before_ = fs->make<TH1F>("ELE_DELTAPHIELECLUSTERTRACKATCALO_BEFORECUTS",chtitle.data(),40,-0.2,0.2);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_deltaPhiEleClusterTrackAtCalo_after_ = fs->make<TH1F>("ELE_DELTAPHIELECLUSTERTRACKATCALO_AFTERCUTS",chtitle.data(),40,-0.2,0.2);

  // the seed cluster phi - track phi position at the PCA to the seed cluster, extrapolated from the outermost track state.
  plotString = "#Delta#phi between seed cluster and track;#Delta#phi;";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_deltaPhiSeedClusterTrackAtCalo_before_ = fs->make<TH1F>("ELE_DELTAPHISEEDCLUSTERTRACKATCALO_BEFORECUTS",chtitle.data(),40,-0.2,0.2);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_deltaPhiSeedClusterTrackAtCalo_after_ = fs->make<TH1F>("ELE_DELTAPHISEEDCLUSTERTRACKATCALO_AFTERCUTS",chtitle.data(),40,-0.2,0.2);

  // the supercluster phi - track phi position at the PCA to the supercluster, extrapolated from the innermost track state.
  plotString = "#Delta#phi between supercluster and track;#Delta#phi;";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_deltaPhiSuperClusterTrackAtVtx_before_ = fs->make<TH1F>("ELE_DELTAPHISUPERCLUSTERTRACKATVTX_BEFORECUTS",chtitle.data(),40,-0.2,0.2);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_deltaPhiSuperClusterTrackAtVtx_after_ = fs->make<TH1F>("ELE_DELTAPHISUPERCLUSTERTRACKATVTX_AFTERCUTS",chtitle.data(),40,-0.2,0.2);



  
  // track position at PCA to beam spot -- x coord
  plotString = "track x-position at PCA to beam spot;x;";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_trackPositionAtVtxX_before_ = fs->make<TH1F>("ELE_TRACKPOSITIONATVTXX_BEFORECUTS",chtitle.data(),50,-2.5,2.5);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_trackPositionAtVtxX_after_ = fs->make<TH1F>("ELE_TRACKPOSITIONATVTXX_AFTERCUTS",chtitle.data(),50,-2.5,2.5);

  // track position at PCA to beam spot -- y coord
  plotString = "track y-position at PCA to beam spot;y;";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_trackPositionAtVtxY_before_ = fs->make<TH1F>("ELE_TRACKPOSITIONATVTXY_BEFORECUTS",chtitle.data(),50,-2.5,2.5);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_trackPositionAtVtxY_after_ = fs->make<TH1F>("ELE_TRACKPOSITIONATVTXY_AFTERCUTS",chtitle.data(),50,-2.5,2.5);

  // track position at PCA to beam spot -- z coord
  plotString = "track z-position at PCA to beam spot;z;";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_trackPositionAtVtxZ_before_ = fs->make<TH1F>("ELE_TRACKPOSITIONATVTXZ_BEFORECUTS",chtitle.data(),50,-25.,25.); // wider range
  chtitle = collName+"after cuts: "+plotString;
  h_ele_trackPositionAtVtxZ_after_ = fs->make<TH1F>("ELE_TRACKPOSITIONATVTXZ_AFTERCUTS",chtitle.data(),50,-25.,25.);   // of values

  // track momentum at PCA to beam spot -- x coord
  plotString = "track x-momentum at PCA to beam spot;p_{x};";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_trackMomentumAtVtxX_before_ = fs->make<TH1F>("ELE_TRACKMOMENTUMATVTXX_BEFORECUTS",chtitle.data(),100,-50.,50.);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_trackMomentumAtVtxX_after_ = fs->make<TH1F>("ELE_TRACKMOMENTUMATVTXX_AFTERCUTS",chtitle.data(),100,-50.,50.);

  // track momentum at PCA to beam spot -- y coord
  plotString = "track y-momentum at PCA to beam spot;p_{y};";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_trackMomentumAtVtxY_before_ = fs->make<TH1F>("ELE_TRACKMOMENTUMATVTXY_BEFORECUTS",chtitle.data(),100,-50.,50.);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_trackMomentumAtVtxY_after_ = fs->make<TH1F>("ELE_TRACKMOMENTUMATVTXY_AFTERCUTS",chtitle.data(),100,-50.,50.);

  // track momentum at PCA to beam spot -- z coord
  plotString = "track z-momentum at PCA to beam spot;p_{z};";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_trackMomentumAtVtxZ_before_ = fs->make<TH1F>("ELE_TRACKMOMENTUMATVTXZ_BEFORECUTS",chtitle.data(),100,-50.,50.);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_trackMomentumAtVtxZ_after_ = fs->make<TH1F>("ELE_TRACKMOMENTUMATVTXZ_AFTERCUTS",chtitle.data(),100,-50.,50.);




  // track position at PCA to supercluster -- x coord
  plotString = "track x-position at PCA to supercluster;x;";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_trackPositionAtCaloX_before_ = fs->make<TH1F>("ELE_TRACKPOSITIONATCALOX_BEFORECUTS",chtitle.data(),50,-250.,250.);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_trackPositionAtCaloX_after_ = fs->make<TH1F>("ELE_TRACKPOSITIONATCALOX_AFTERCUTS",chtitle.data(),50,-250.,250.);

  // track position at PCA to supercluster -- y coord
  plotString = "track y-position at PCA to supercluster;y;";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_trackPositionAtCaloY_before_ = fs->make<TH1F>("ELE_TRACKPOSITIONATCALOY_BEFORECUTS",chtitle.data(),50,-250.,250.);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_trackPositionAtCaloY_after_ = fs->make<TH1F>("ELE_TRACKPOSITIONATCALOY_AFTERCUTS",chtitle.data(),50,-250.,250.);

  // track position at PCA to supercluster -- z coord
  plotString = "track z-position at PCA to supercluster;z;";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_trackPositionAtCaloZ_before_ = fs->make<TH1F>("ELE_TRACKPOSITIONATCALOZ_BEFORECUTS",chtitle.data(),50,-500.,500.);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_trackPositionAtCaloZ_after_ = fs->make<TH1F>("ELE_TRACKPOSITIONATCALOZ_AFTERCUTS",chtitle.data(),50,-500.,500.);

  // track momentum at PCA to supercluster -- x coord
  plotString = "track x-momentum at PCA to supercluster;p_{x};";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_trackMomentumAtCaloX_before_ = fs->make<TH1F>("ELE_TRACKMOMENTUMATCALOX_BEFORECUTS",chtitle.data(),100,-50.,50.);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_trackMomentumAtCaloX_after_ = fs->make<TH1F>("ELE_TRACKMOMENTUMATCALOX_AFTERCUTS",chtitle.data(),100,-50.,50.);

  // track momentum at PCA to supercluster -- y coord
  plotString = "track y-momentum at PCA to supercluster;p_{y};";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_trackMomentumAtCaloY_before_ = fs->make<TH1F>("ELE_TRACKMOMENTUMATCALOY_BEFORECUTS",chtitle.data(),100,-50.,50.);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_trackMomentumAtCaloY_after_ = fs->make<TH1F>("ELE_TRACKMOMENTUMATCALOY_AFTERCUTS",chtitle.data(),100,-50.,50.);

  // track momentum at PCA to supercluster -- z coord
  plotString = "track z-momentum at PCA to supercluster;p_{z};";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_trackMomentumAtCaloZ_before_ = fs->make<TH1F>("ELE_TRACKMOMENTUMATCALOZ_BEFORECUTS",chtitle.data(),100,-50.,50.);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_trackMomentumAtCaloZ_after_ = fs->make<TH1F>("ELE_TRACKMOMENTUMATCALOZ_AFTERCUTS",chtitle.data(),100,-50.,50.);


	

  // the electron cluster energy / track momentum at the PCA to the electron cluster (as defined by electronCluster()), extrapolated from the outermost track state.
  plotString = "cluster E / track p (outer);E/p;";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_eEleClusterOverPout_before_ = fs->make<TH1F>("ELE_EELECLUSTEROVERPOUT_BEFORECUTS",chtitle.data(),75,0.,3.);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_eEleClusterOverPout_after_ = fs->make<TH1F>("ELE_EELECLUSTEROVERPOUT_AFTERCUTS",chtitle.data(),75,0.,3.);

  // the seed cluster energy / track momentum at the PCA to the beam spot. 
  plotString = "seed cluster E / track p;E/p;";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_eSeedClusterOverP_before_ = fs->make<TH1F>("ELE_ESEEDCLUSTEROVERP_BEFORECUTS",chtitle.data(),75,0.,3.);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_eSeedClusterOverP_after_ = fs->make<TH1F>("ELE_ESEEDCLUSTEROVERP_AFTERCUTS",chtitle.data(),75,0.,3.);

  // the seed cluster energy / track momentum at the PCA to the seed cluster, extrapolated from the outermost track state. 
  plotString = "seed cluster E / track p (outer);E/p;";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_eSeedClusterOverPout_before_ = fs->make<TH1F>("ELE_ESEEDCLUSTEROVERPOUT_BEFORECUTS",chtitle.data(),75,0.,3.);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_eSeedClusterOverPout_after_ = fs->make<TH1F>("ELE_ESEEDCLUSTEROVERPOUT_AFTERCUTS",chtitle.data(),75,0.,3.);

  // the supercluster energy / track momentum at the point of closest approach (PCA) to the beam spot.
  plotString = "supercluster E / track p;E/p;";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_eSuperClusterOverP_before_ = fs->make<TH1F>("ELE_ESUPERCLUSTEROVERP_BEFORECUTS",chtitle.data(),75,0.,3.);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_eSuperClusterOverP_after_ = fs->make<TH1F>("ELE_ESUPERCLUSTEROVERP_AFTERCUTS",chtitle.data(),75,0.,3.);


  // the final electron momentum error
  plotString = "Error on final electron momentum;Momentum error (GeV);";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_electronMomentumError_before_ = fs->make<TH1F>("ELE_ELECTRONMOMENTUMERROR_BEFORECUTS",chtitle.data(),100,0.,5.);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_electronMomentumError_after_ = fs->make<TH1F>("ELE_ELECTRONMOMENTUMERROR_AFTERCUTS",chtitle.data(),100,0.,5.);

	

  // isolation variables

  // 0.3

  // both barrel and endcap
  // deltaR radius 0.3: ecal iso deposit with electron footprint removed. 
  plotString = "ECAL isolation (#DeltaR = 0.3), barrel+endcap;ECAL isolation (GeV);";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_dr03EcalRecHitSumEt_before_ = fs->make<TH1F>("ELE_DR03ECALRECHITSUMET_BEFORECUTS",chtitle.data(),100,0.,10.);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_dr03EcalRecHitSumEt_after_ = fs->make<TH1F>("ELE_DR03ECALRECHITSUMET_AFTERCUTS",chtitle.data(),100,0.,10.);

  // deltaR radius 0.3: hcal depth 1 iso deposit with electron footprint removed.
  plotString = "HCAL isolation, depth 1 (#DeltaR = 0.3), barrel+endcap;HCAL isolation (GeV);";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_dr03HcalDepth1TowerSumEt_before_ = fs->make<TH1F>("ELE_DR03HCALDEPTH1TOWERSUMET_BEFORECUTS",chtitle.data(),100,0.,5.);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_dr03HcalDepth1TowerSumEt_after_ = fs->make<TH1F>("ELE_DR03HCALDEPTH1TOWERSUMET_AFTERCUTS",chtitle.data(),100,0.,5.);

  // deltaR radius 0.3: hcal depth 2 iso deposit with electron footprint removed.
  plotString = "HCAL isolation, depth 2 (#DeltaR = 0.3), barrel+endcap;HCAL isolation (GeV);";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_dr03HcalDepth2TowerSumEt_before_ = fs->make<TH1F>("ELE_DR03HCALDEPTH2TOWERSUMET_BEFORECUTS",chtitle.data(),100,0.,5.);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_dr03HcalDepth2TowerSumEt_after_ = fs->make<TH1F>("ELE_DR03HCALDEPTH2TOWERSUMET_AFTERCUTS",chtitle.data(),100,0.,5.);

  // deltaR radius 0.3: hcal iso deposit with electron footprint removed (simple sum of depths 1 and 2)
  plotString = "HCAL isolation (#DeltaR = 0.3), barrel+endcap;HCAL isolation (GeV);";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_dr03HcalTowerSumEt_before_ = fs->make<TH1F>("ELE_DR03HCALTOWERSUMET_BEFORECUTS",chtitle.data(),100,0.,5.);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_dr03HcalTowerSumEt_after_ = fs->make<TH1F>("ELE_DR03HCALTOWERSUMET_AFTERCUTS",chtitle.data(),100,0.,5.);

  // deltaR radius 0.3: track iso deposit with electron footprint removed
  plotString = "Track isolation (#DeltaR = 0.3), barrel+endcap;Track isolation (GeV);";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_dr03TkSumPt_before_ = fs->make<TH1F>("ELE_DR03TKSUMPT_BEFORECUTS",chtitle.data(),100,0.,10.);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_dr03TkSumPt_after_ = fs->make<TH1F>("ELE_DR03TKSUMPT_AFTERCUTS",chtitle.data(),100,0.,10.);

  // just barrel
  // deltaR radius 0.3: ecal iso deposit with electron footprint removed. 
  plotString = "ECAL isolation (#DeltaR = 0.3), barrel-only;ECAL isolation (GeV);";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_dr03EcalRecHitSumEtBarrel_before_ = fs->make<TH1F>("ELE_DR03ECALRECHITSUMETBARREL_BEFORECUTS",chtitle.data(),100,0.,10.);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_dr03EcalRecHitSumEtBarrel_after_ = fs->make<TH1F>("ELE_DR03ECALRECHITSUMETBARREL_AFTERCUTS",chtitle.data(),100,0.,10.);

  // deltaR radius 0.3: hcal depth 1 iso deposit with electron footprint removed.
  plotString = "HCAL isolation, depth 1 (#DeltaR = 0.3), barrel-only;HCAL isolation (GeV);";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_dr03HcalDepth1TowerSumEtBarrel_before_ = fs->make<TH1F>("ELE_DR03HCALDEPTH1TOWERSUMETBARREL_BEFORECUTS",chtitle.data(),100,0.,5.);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_dr03HcalDepth1TowerSumEtBarrel_after_ = fs->make<TH1F>("ELE_DR03HCALDEPTH1TOWERSUMETBARREL_AFTERCUTS",chtitle.data(),100,0.,5.);

  // deltaR radius 0.3: hcal depth 2 iso deposit with electron footprint removed.
  plotString = "HCAL isolation, depth 2 (#DeltaR = 0.3), barrel-only;HCAL isolation (GeV);";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_dr03HcalDepth2TowerSumEtBarrel_before_ = fs->make<TH1F>("ELE_DR03HCALDEPTH2TOWERSUMETBARREL_BEFORECUTS",chtitle.data(),100,0.,5.);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_dr03HcalDepth2TowerSumEtBarrel_after_ = fs->make<TH1F>("ELE_DR03HCALDEPTH2TOWERSUMETBARREL_AFTERCUTS",chtitle.data(),100,0.,5.);

  // deltaR radius 0.3: hcal iso deposit with electron footprint removed (simple sum of depths 1 and 2)
  plotString = "HCAL isolation (#DeltaR = 0.3), barrel-only;HCAL isolation (GeV);";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_dr03HcalTowerSumEtBarrel_before_ = fs->make<TH1F>("ELE_DR03HCALTOWERSUMETBARREL_BEFORECUTS",chtitle.data(),100,0.,5.);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_dr03HcalTowerSumEtBarrel_after_ = fs->make<TH1F>("ELE_DR03HCALTOWERSUMETBARREL_AFTERCUTS",chtitle.data(),100,0.,5.);

  // deltaR radius 0.3: track iso deposit with electron footprint removed
  plotString = "Track isolation (#DeltaR = 0.3), barrel-only;Track isolation (GeV);";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_dr03TkSumPtBarrel_before_ = fs->make<TH1F>("ELE_DR03TKSUMPTBARREL_BEFORECUTS",chtitle.data(),100,0.,10.);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_dr03TkSumPtBarrel_after_ = fs->make<TH1F>("ELE_DR03TKSUMPTBARREL_AFTERCUTS",chtitle.data(),100,0.,10.);

  // just endcap
  // deltaR radius 0.3: ecal iso deposit with electron footprint removed. 
  plotString = "ECAL isolation (#DeltaR = 0.3), endcap-only;ECAL isolation (GeV);";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_dr03EcalRecHitSumEtEndcap_before_ = fs->make<TH1F>("ELE_DR03ECALRECHITSUMETENDCAP_BEFORECUTS",chtitle.data(),100,0.,10.);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_dr03EcalRecHitSumEtEndcap_after_ = fs->make<TH1F>("ELE_DR03ECALRECHITSUMETENDCAP_AFTERCUTS",chtitle.data(),100,0.,10.);

  // deltaR radius 0.3: hcal depth 1 iso deposit with electron footprint removed.
  plotString = "HCAL isolation, depth 1 (#DeltaR = 0.3), endcap-only;HCAL isolation (GeV);";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_dr03HcalDepth1TowerSumEtEndcap_before_ = fs->make<TH1F>("ELE_DR03HCALDEPTH1TOWERSUMETENDCAP_BEFORECUTS",chtitle.data(),100,0.,5.);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_dr03HcalDepth1TowerSumEtEndcap_after_ = fs->make<TH1F>("ELE_DR03HCALDEPTH1TOWERSUMETENDCAP_AFTERCUTS",chtitle.data(),100,0.,5.);

  // deltaR radius 0.3: hcal depth 2 iso deposit with electron footprint removed.
  plotString = "HCAL isolation, depth 2 (#DeltaR = 0.3), endcap-only;HCAL isolation (GeV);";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_dr03HcalDepth2TowerSumEtEndcap_before_ = fs->make<TH1F>("ELE_DR03HCALDEPTH2TOWERSUMETENDCAP_BEFORECUTS",chtitle.data(),100,0.,5.);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_dr03HcalDepth2TowerSumEtEndcap_after_ = fs->make<TH1F>("ELE_DR03HCALDEPTH2TOWERSUMETENDCAP_AFTERCUTS",chtitle.data(),100,0.,5.);

  // deltaR radius 0.3: hcal iso deposit with electron footprint removed (simple sum of depths 1 and 2)
  plotString = "HCAL isolation (#DeltaR = 0.3), endcap-only;HCAL isolation (GeV);";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_dr03HcalTowerSumEtEndcap_before_ = fs->make<TH1F>("ELE_DR03HCALTOWERSUMETENDCAP_BEFORECUTS",chtitle.data(),100,0.,5.);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_dr03HcalTowerSumEtEndcap_after_ = fs->make<TH1F>("ELE_DR03HCALTOWERSUMETENDCAP_AFTERCUTS",chtitle.data(),100,0.,5.);

  // deltaR radius 0.3: track iso deposit with electron footprint removed
  plotString = "Track isolation (#DeltaR = 0.3), endcap-only;Track isolation (GeV);";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_dr03TkSumPtEndcap_before_ = fs->make<TH1F>("ELE_DR03TKSUMPTENDCAP_BEFORECUTS",chtitle.data(),100,0.,10.);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_dr03TkSumPtEndcap_after_ = fs->make<TH1F>("ELE_DR03TKSUMPTENDCAP_AFTERCUTS",chtitle.data(),100,0.,10.);


  // 0.4

  // both barrel and endcap
  // deltaR radius 0.4: ecal iso deposit with electron footprint removed. 
  plotString = "ECAL isolation (#DeltaR = 0.4), barrel+endcap;ECAL isolation (GeV);";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_dr04EcalRecHitSumEt_before_ = fs->make<TH1F>("ELE_DR04ECALRECHITSUMET_BEFORECUTS",chtitle.data(),100,0.,10.);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_dr04EcalRecHitSumEt_after_ = fs->make<TH1F>("ELE_DR04ECALRECHITSUMET_AFTERCUTS",chtitle.data(),100,0.,10.);

  // deltaR radius 0.4: hcal depth 1 iso deposit with electron footprint removed.
  plotString = "HCAL isolation, depth 1 (#DeltaR = 0.4), barrel+endcap;HCAL isolation (GeV);";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_dr04HcalDepth1TowerSumEt_before_ = fs->make<TH1F>("ELE_DR04HCALDEPTH1TOWERSUMET_BEFORECUTS",chtitle.data(),100,0.,5.);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_dr04HcalDepth1TowerSumEt_after_ = fs->make<TH1F>("ELE_DR04HCALDEPTH1TOWERSUMET_AFTERCUTS",chtitle.data(),100,0.,5.);

  // deltaR radius 0.4: hcal depth 2 iso deposit with electron footprint removed.
  plotString = "HCAL isolation, depth 2 (#DeltaR = 0.4), barrel+endcap;HCAL isolation (GeV);";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_dr04HcalDepth2TowerSumEt_before_ = fs->make<TH1F>("ELE_DR04HCALDEPTH2TOWERSUMET_BEFORECUTS",chtitle.data(),100,0.,5.);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_dr04HcalDepth2TowerSumEt_after_ = fs->make<TH1F>("ELE_DR04HCALDEPTH2TOWERSUMET_AFTERCUTS",chtitle.data(),100,0.,5.);

  // deltaR radius 0.4: hcal iso deposit with electron footprint removed (simple sum of depths 1 and 2)
  plotString = "HCAL isolation (#DeltaR = 0.4), barrel+endcap;HCAL isolation (GeV);";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_dr04HcalTowerSumEt_before_ = fs->make<TH1F>("ELE_DR04HCALTOWERSUMET_BEFORECUTS",chtitle.data(),100,0.,5.);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_dr04HcalTowerSumEt_after_ = fs->make<TH1F>("ELE_DR04HCALTOWERSUMET_AFTERCUTS",chtitle.data(),100,0.,5.);

  // deltaR radius 0.4: track iso deposit with electron footprint removed
  plotString = "Track isolation (#DeltaR = 0.4), barrel+endcap;Track isolation (GeV);";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_dr04TkSumPt_before_ = fs->make<TH1F>("ELE_DR04TKSUMPT_BEFORECUTS",chtitle.data(),100,0.,10.);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_dr04TkSumPt_after_ = fs->make<TH1F>("ELE_DR04TKSUMPT_AFTERCUTS",chtitle.data(),100,0.,10.);

  // just barrel
  // deltaR radius 0.4: ecal iso deposit with electron footprint removed. 
  plotString = "ECAL isolation (#DeltaR = 0.4), barrel-only;ECAL isolation (GeV);";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_dr04EcalRecHitSumEtBarrel_before_ = fs->make<TH1F>("ELE_DR04ECALRECHITSUMETBARREL_BEFORECUTS",chtitle.data(),100,0.,10.);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_dr04EcalRecHitSumEtBarrel_after_ = fs->make<TH1F>("ELE_DR04ECALRECHITSUMETBARREL_AFTERCUTS",chtitle.data(),100,0.,10.);

  // deltaR radius 0.4: hcal depth 1 iso deposit with electron footprint removed.
  plotString = "HCAL isolation, depth 1 (#DeltaR = 0.4), barrel-only;HCAL isolation (GeV);";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_dr04HcalDepth1TowerSumEtBarrel_before_ = fs->make<TH1F>("ELE_DR04HCALDEPTH1TOWERSUMETBARREL_BEFORECUTS",chtitle.data(),100,0.,5.);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_dr04HcalDepth1TowerSumEtBarrel_after_ = fs->make<TH1F>("ELE_DR04HCALDEPTH1TOWERSUMETBARREL_AFTERCUTS",chtitle.data(),100,0.,5.);

  // deltaR radius 0.4: hcal depth 2 iso deposit with electron footprint removed.
  plotString = "HCAL isolation, depth 2 (#DeltaR = 0.4), barrel-only;HCAL isolation (GeV);";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_dr04HcalDepth2TowerSumEtBarrel_before_ = fs->make<TH1F>("ELE_DR04HCALDEPTH2TOWERSUMETBARREL_BEFORECUTS",chtitle.data(),100,0.,5.);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_dr04HcalDepth2TowerSumEtBarrel_after_ = fs->make<TH1F>("ELE_DR04HCALDEPTH2TOWERSUMETBARREL_AFTERCUTS",chtitle.data(),100,0.,5.);

  // deltaR radius 0.4: hcal iso deposit with electron footprint removed (simple sum of depths 1 and 2)
  plotString = "HCAL isolation (#DeltaR = 0.4), barrel-only;HCAL isolation (GeV);";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_dr04HcalTowerSumEtBarrel_before_ = fs->make<TH1F>("ELE_DR04HCALTOWERSUMETBARREL_BEFORECUTS",chtitle.data(),100,0.,5.);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_dr04HcalTowerSumEtBarrel_after_ = fs->make<TH1F>("ELE_DR04HCALTOWERSUMETBARREL_AFTERCUTS",chtitle.data(),100,0.,5.);

  // deltaR radius 0.4: track iso deposit with electron footprint removed
  plotString = "Track isolation (#DeltaR = 0.4), barrel-only;Track isolation (GeV);";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_dr04TkSumPtBarrel_before_ = fs->make<TH1F>("ELE_DR04TKSUMPTBARREL_BEFORECUTS",chtitle.data(),100,0.,10.);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_dr04TkSumPtBarrel_after_ = fs->make<TH1F>("ELE_DR04TKSUMPTBARREL_AFTERCUTS",chtitle.data(),100,0.,10.);

  // just endcap
  // deltaR radius 0.4: ecal iso deposit with electron footprint removed. 
  plotString = "ECAL isolation (#DeltaR = 0.4), endcap-only;ECAL isolation (GeV);";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_dr04EcalRecHitSumEtEndcap_before_ = fs->make<TH1F>("ELE_DR04ECALRECHITSUMETENDCAP_BEFORECUTS",chtitle.data(),100,0.,10.);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_dr04EcalRecHitSumEtEndcap_after_ = fs->make<TH1F>("ELE_DR04ECALRECHITSUMETENDCAP_AFTERCUTS",chtitle.data(),100,0.,10.);

  // deltaR radius 0.4: hcal depth 1 iso deposit with electron footprint removed.
  plotString = "HCAL isolation, depth 1 (#DeltaR = 0.4), endcap-only;HCAL isolation (GeV);";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_dr04HcalDepth1TowerSumEtEndcap_before_ = fs->make<TH1F>("ELE_DR04HCALDEPTH1TOWERSUMETENDCAP_BEFORECUTS",chtitle.data(),100,0.,5.);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_dr04HcalDepth1TowerSumEtEndcap_after_ = fs->make<TH1F>("ELE_DR04HCALDEPTH1TOWERSUMETENDCAP_AFTERCUTS",chtitle.data(),100,0.,5.);

  // deltaR radius 0.4: hcal depth 2 iso deposit with electron footprint removed.
  plotString = "HCAL isolation, depth 2 (#DeltaR = 0.4), endcap-only;HCAL isolation (GeV);";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_dr04HcalDepth2TowerSumEtEndcap_before_ = fs->make<TH1F>("ELE_DR04HCALDEPTH2TOWERSUMETENDCAP_BEFORECUTS",chtitle.data(),100,0.,5.);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_dr04HcalDepth2TowerSumEtEndcap_after_ = fs->make<TH1F>("ELE_DR04HCALDEPTH2TOWERSUMETENDCAP_AFTERCUTS",chtitle.data(),100,0.,5.);

  // deltaR radius 0.4: hcal iso deposit with electron footprint removed (simple sum of depths 1 and 2)
  plotString = "HCAL isolation (#DeltaR = 0.4), endcap-only;HCAL isolation (GeV);";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_dr04HcalTowerSumEtEndcap_before_ = fs->make<TH1F>("ELE_DR04HCALTOWERSUMETENDCAP_BEFORECUTS",chtitle.data(),100,0.,5.);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_dr04HcalTowerSumEtEndcap_after_ = fs->make<TH1F>("ELE_DR04HCALTOWERSUMETENDCAP_AFTERCUTS",chtitle.data(),100,0.,5.);

  // deltaR radius 0.4: track iso deposit with electron footprint removed
  plotString = "Track isolation (#DeltaR = 0.4), endcap-only;Track isolation (GeV);";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_dr04TkSumPtEndcap_before_ = fs->make<TH1F>("ELE_DR04TKSUMPTENDCAP_BEFORECUTS",chtitle.data(),100,0.,10.);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_dr04TkSumPtEndcap_after_ = fs->make<TH1F>("ELE_DR04TKSUMPTENDCAP_AFTERCUTS",chtitle.data(),100,0.,10.);


  // shower shape variables

  // both barrel and endcap
  // energy inside 1x5 in etaxphi around the seed Xtal. 
  plotString = "energy inside 1x5 (#etax#phi) around seed crystal, barrel+endcap;energy (GeV);";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_e1x5_before_ = fs->make<TH1F>("ELE_E1X5_BEFORECUTS",chtitle.data(),100,0.,100.);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_e1x5_after_ = fs->make<TH1F>("ELE_E1X5_AFTERCUTS",chtitle.data(),100,0.,100.);

  // energy inside 2x5 in etaxphi around the seed Xtal (max bwt the 2 possible sums)
  plotString = "energy inside 2x5 (#etax#phi) around seed crystal (max of 2), barrel+endcap;energy (GeV);";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_e2x5Max_before_ = fs->make<TH1F>("ELE_E2X5MAX_BEFORECUTS",chtitle.data(),100,0.,100.);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_e2x5Max_after_ = fs->make<TH1F>("ELE_E2X5MAX_AFTERCUTS",chtitle.data(),100,0.,100.);

  // energy inside 5x5 in etaxphi around the seed Xtal. 
  plotString = "energy inside 5x5 (#etax#phi) around seed crystal, barrel+endcap;energy (GeV);";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_e5x5_before_ = fs->make<TH1F>("ELE_E5X5_BEFORECUTS",chtitle.data(),100,0.,100.);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_e5x5_after_ = fs->make<TH1F>("ELE_E5X5_AFTERCUTS",chtitle.data(),100,0.,100.);

  // energy inside 1x5 in etaxphi around the seed Xtal / energy in 5x5 
  plotString = "energy inside 1x5 (#etax#phi) / energy in 5x5, barrel+endcap;energy (GeV);";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_r1x5_before_ = fs->make<TH1F>("ELE_R1X5_BEFORECUTS",chtitle.data(),50,0.,1.5);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_r1x5_after_ = fs->make<TH1F>("ELE_R1X5_AFTERCUTS",chtitle.data(),50,0.,1.5);

  // energy inside 2x5 in etaxphi around the seed Xtal (max bwt the 2 possible sums) / energy in 5x5
  plotString = "energy inside 2x5 (#etax#phi) (max of 2) / energy in 5x5, barrel+endcap;energy (GeV);";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_r2x5Max_before_ = fs->make<TH1F>("ELE_R2X5MAX_BEFORECUTS",chtitle.data(),50,0.,1.5);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_r2x5Max_after_ = fs->make<TH1F>("ELE_R2X5MAX_AFTERCUTS",chtitle.data(),50,0.,1.5);

//   // supercluster preshower energy
//   plotString = "supercluster preshower energy, barrel+endcap;energy (GeV);";
//   chtitle = collName+"before cuts: "+plotString;
//   h_ele_scPreshowerEnergy_before_ = fs->make<TH1F>("ELE_SCPRESHOWERENERGY_BEFORECUTS",chtitle.data(),100,0.,50.);
//   chtitle = collName+"after cuts: "+plotString;
//   h_ele_scPreshowerEnergy_after_ = fs->make<TH1F>("ELE_SCPRESHOWERENERGY_AFTERCUTS",chtitle.data(),100,0.,50.);

  // supercluster eta width
  plotString = "supercluster #eta width, barrel+endcap;#eta;";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_scEtaWidth_before_ = fs->make<TH1F>("ELE_SCETAWIDTH_BEFORECUTS",chtitle.data(),30,0.,0.3);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_scEtaWidth_after_ = fs->make<TH1F>("ELE_SCETAWIDTH_AFTERCUTS",chtitle.data(),30,0.,0.3);

  // supercluster phi width
  plotString = "supercluster #phi width, barrel+endcap;#phi;";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_scPhiWidth_before_ = fs->make<TH1F>("ELE_SCPHIWIDTH_BEFORECUTS",chtitle.data(),30,0.,0.3);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_scPhiWidth_after_ = fs->make<TH1F>("ELE_SCPHIWIDTH_AFTERCUTS",chtitle.data(),30,0.,0.3);

  // supercluster r9 (3x3 energy / 5x5 energy)
  plotString = "supercluster r9, barrel+endcap;r9;";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_scR9_before_ = fs->make<TH1F>("ELE_SCR9_BEFORECUTS",chtitle.data(),60,0.,1.2);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_scR9_after_ = fs->make<TH1F>("ELE_SCR9_AFTERCUTS",chtitle.data(),60,0.,1.2);

  // supercluster seed energy / supercluster energy
  plotString = "E_{seed}/E_{SC}, barrel+endcap;E_{seed}/E_{SC};";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_scESeedOverESuperCluster_before_ = fs->make<TH1F>("ELE_SCESEEDOVERESUPERCLUSTER_BEFORECUTS",chtitle.data(),60,0.,1.2);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_scESeedOverESuperCluster_after_ = fs->make<TH1F>("ELE_SCESEEDOVERESUPERCLUSTER_AFTERCUTS",chtitle.data(),60,0.,1.2);


  // supercluster max energy / 3x3
  plotString = "E_{max}/E_{3x3}, barrel+endcap;E_{max}/E_{3x3};";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_rMax3x3_before_ = fs->make<TH1F>("ELE_RMAX3X3_BEFORECUTS",chtitle.data(),55,0.,1.1);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_rMax3x3_after_ = fs->make<TH1F>("ELE_RMAX3X3_AFTERCUTS",chtitle.data(),55,0.,1.1);


  // barrel-only
  // energy inside 1x5 in etaxphi around the seed Xtal. 
  plotString = "energy inside 1x5 (#etax#phi) around seed crystal, barrel-only;energy (GeV);";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_e1x5Barrel_before_ = fs->make<TH1F>("ELE_E1X5BARREL_BEFORECUTS",chtitle.data(),100,0.,100.);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_e1x5Barrel_after_ = fs->make<TH1F>("ELE_E1X5BARREL_AFTERCUTS",chtitle.data(),100,0.,100.);

  // energy inside 2x5 in etaxphi around the seed Xtal (max bwt the 2 possible sums)
  plotString = "energy inside 2x5 (#etax#phi) around seed crystal (max of 2), barrel-only;energy (GeV);";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_e2x5MaxBarrel_before_ = fs->make<TH1F>("ELE_E2X5MAXBARREL_BEFORECUTS",chtitle.data(),100,0.,100.);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_e2x5MaxBarrel_after_ = fs->make<TH1F>("ELE_E2X5MAXBARREL_AFTERCUTS",chtitle.data(),100,0.,100.);

  // energy inside 5x5 in etaxphi around the seed Xtal. 
  plotString = "energy inside 5x5 (#etax#phi) around seed crystal, barrel-only;energy (GeV);";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_e5x5Barrel_before_ = fs->make<TH1F>("ELE_E5X5BARREL_BEFORECUTS",chtitle.data(),100,0.,100.);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_e5x5Barrel_after_ = fs->make<TH1F>("ELE_E5X5BARREL_AFTERCUTS",chtitle.data(),100,0.,100.);

  // energy inside 1x5 in etaxphi around the seed Xtal / energy in 5x5 
  plotString = "energy inside 1x5 (#etax#phi) / energy in 5x5, barrel-only;energy (GeV);";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_r1x5Barrel_before_ = fs->make<TH1F>("ELE_R1X5BARREL_BEFORECUTS",chtitle.data(),50,0.,1.5);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_r1x5Barrel_after_ = fs->make<TH1F>("ELE_R1X5BARREL_AFTERCUTS",chtitle.data(),50,0.,1.5);

  // energy inside 2x5 in etaxphi around the seed Xtal (max bwt the 2 possible sums) / energy in 5x5
  plotString = "energy inside 2x5 (#etax#phi) (max of 2) / energy in 5x5, barrel-only;energy (GeV);";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_r2x5MaxBarrel_before_ = fs->make<TH1F>("ELE_R2X5MAXBARREL_BEFORECUTS",chtitle.data(),50,0.,1.5);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_r2x5MaxBarrel_after_ = fs->make<TH1F>("ELE_R2X5MAXBARREL_AFTERCUTS",chtitle.data(),50,0.,1.5);

//   // supercluster preshower energy, barrel-only
//   plotString = "supercluster preshower energy, barrel-only;energy (GeV);";
//   chtitle = collName+"before cuts: "+plotString;
//   h_ele_scPreshowerEnergyBarrel_before_ = fs->make<TH1F>("ELE_SCPRESHOWERENERGYBARREL_BEFORECUTS",chtitle.data(),100,0.,50.);
//   chtitle = collName+"after cuts: "+plotString;
//   h_ele_scPreshowerEnergyBarrel_after_ = fs->make<TH1F>("ELE_SCPRESHOWERENERGYBARREL_AFTERCUTS",chtitle.data(),100,0.,50.);

  // supercluster eta width, barrel-only
  plotString = "supercluster #eta width, barrel-only;#eta;";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_scEtaWidthBarrel_before_ = fs->make<TH1F>("ELE_SCETAWIDTHBARREL_BEFORECUTS",chtitle.data(),30,0.,0.3);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_scEtaWidthBarrel_after_ = fs->make<TH1F>("ELE_SCETAWIDTHBARREL_AFTERCUTS",chtitle.data(),30,0.,0.3);

  // supercluster phi width, barrel-only
  plotString = "supercluster #phi width, barrel-only;#phi;";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_scPhiWidthBarrel_before_ = fs->make<TH1F>("ELE_SCPHIWIDTHBARREL_BEFORECUTS",chtitle.data(),30,0.,0.3);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_scPhiWidthBarrel_after_ = fs->make<TH1F>("ELE_SCPHIWIDTHBARREL_AFTERCUTS",chtitle.data(),30,0.,0.3);

  // supercluster r9 (3x3 energy / 5x5 energy)
  plotString = "supercluster r9, barrel-only;r9;";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_scR9Barrel_before_ = fs->make<TH1F>("ELE_SCR9BARREL_BEFORECUTS",chtitle.data(),60,0.,1.2);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_scR9Barrel_after_ = fs->make<TH1F>("ELE_SCR9BARREL_AFTERCUTS",chtitle.data(),60,0.,1.2);

  // supercluster seed energy / supercluster energy
  plotString = "E_{seed}/E_{SC}, barrel-only;E_{seed}/E_{SC};";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_scESeedOverESuperClusterBarrel_before_ = fs->make<TH1F>("ELE_SCESEEDOVERESUPERCLUSTERBARREL_BEFORECUTS",chtitle.data(),60,0.,1.2);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_scESeedOverESuperClusterBarrel_after_ = fs->make<TH1F>("ELE_SCESEEDOVERESUPERCLUSTERBARREL_AFTERCUTS",chtitle.data(),60,0.,1.2);

  // supercluster max energy / 3x3
  plotString = "E_{max}/E_{3x3}, barrel-only;E_{max}/E_{3x3};";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_rMax3x3Barrel_before_ = fs->make<TH1F>("ELE_RMAX3X3BARREL_BEFORECUTS",chtitle.data(),55,0.,1.1);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_rMax3x3Barrel_after_ = fs->make<TH1F>("ELE_RMAX3X3BARREL_AFTERCUTS",chtitle.data(),55,0.,1.1);


  // endcap-only
  // energy inside 1x5 in etaxphi around the seed Xtal. 
  plotString = "energy inside 1x5 (#etax#phi) around seed crystal, endcap-only;energy (GeV);";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_e1x5Endcap_before_ = fs->make<TH1F>("ELE_E1X5ENDCAP_BEFORECUTS",chtitle.data(),100,0.,100.);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_e1x5Endcap_after_ = fs->make<TH1F>("ELE_E1X5ENDCAP_AFTERCUTS",chtitle.data(),100,0.,100.);

  // energy inside 2x5 in etaxphi around the seed Xtal (max bwt the 2 possible sums)
  plotString = "energy inside 2x5 (#etax#phi) around seed crystal (max of 2), endcap-only;energy (GeV);";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_e2x5MaxEndcap_before_ = fs->make<TH1F>("ELE_E2X5MAXENDCAP_BEFORECUTS",chtitle.data(),100,0.,100.);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_e2x5MaxEndcap_after_ = fs->make<TH1F>("ELE_E2X5MAXENDCAP_AFTERCUTS",chtitle.data(),100,0.,100.);

  // energy inside 5x5 in etaxphi around the seed Xtal. 
  plotString = "energy inside 5x5 (#etax#phi) around seed crystal, endcap-only;energy (GeV);";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_e5x5Endcap_before_ = fs->make<TH1F>("ELE_E5X5ENDCAP_BEFORECUTS",chtitle.data(),100,0.,100.);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_e5x5Endcap_after_ = fs->make<TH1F>("ELE_E5X5ENDCAP_AFTERCUTS",chtitle.data(),100,0.,100.);

  // energy inside 1x5 in etaxphi around the seed Xtal / energy in 5x5 
  plotString = "energy inside 1x5 (#etax#phi) / energy in 5x5, endcap-only;energy (GeV);";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_r1x5Endcap_before_ = fs->make<TH1F>("ELE_R1X5ENDCAP_BEFORECUTS",chtitle.data(),50,0.,1.5);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_r1x5Endcap_after_ = fs->make<TH1F>("ELE_R1X5ENDCAP_AFTERCUTS",chtitle.data(),50,0.,1.5);

  // energy inside 2x5 in etaxphi around the seed Xtal (max bwt the 2 possible sums) / energy in 5x5
  plotString = "energy inside 2x5 (#etax#phi) (max of 2) / energy in 5x5, endcap-only;energy (GeV);";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_r2x5MaxEndcap_before_ = fs->make<TH1F>("ELE_R2X5MAXENDCAP_BEFORECUTS",chtitle.data(),50,0.,1.5);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_r2x5MaxEndcap_after_ = fs->make<TH1F>("ELE_R2X5MAXENDCAP_AFTERCUTS",chtitle.data(),50,0.,1.5);

  // supercluster preshower energy, endcap-only
  plotString = "supercluster preshower energy, endcap-only;energy (GeV);";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_scPreshowerEnergyEndcap_before_ = fs->make<TH1F>("ELE_SCPRESHOWERENERGYENDCAP_BEFORECUTS",chtitle.data(),60,0.,30.);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_scPreshowerEnergyEndcap_after_ = fs->make<TH1F>("ELE_SCPRESHOWERENERGYENDCAP_AFTERCUTS",chtitle.data(),60,0.,30.);

  // supercluster eta width, endcap-only
  plotString = "supercluster #eta width, endcap-only;#eta;";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_scEtaWidthEndcap_before_ = fs->make<TH1F>("ELE_SCETAWIDTHENDCAP_BEFORECUTS",chtitle.data(),30,0.,0.3);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_scEtaWidthEndcap_after_ = fs->make<TH1F>("ELE_SCETAWIDTHENDCAP_AFTERCUTS",chtitle.data(),30,0.,0.3);

  // supercluster phi width, endcap-only
  plotString = "supercluster #phi width, endcap-only;#phi;";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_scPhiWidthEndcap_before_ = fs->make<TH1F>("ELE_SCPHIWIDTHENDCAP_BEFORECUTS",chtitle.data(),50,0.,1.);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_scPhiWidthEndcap_after_ = fs->make<TH1F>("ELE_SCPHIWIDTHENDCAP_AFTERCUTS",chtitle.data(),50,0.,1.);

  // supercluster r9 (3x3 energy / 5x5 energy)
  plotString = "supercluster r9, endcap-only;r9;";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_scR9Endcap_before_ = fs->make<TH1F>("ELE_SCR9ENDCAP_BEFORECUTS",chtitle.data(),60,0.,1.2);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_scR9Endcap_after_ = fs->make<TH1F>("ELE_SCR9ENDCAP_AFTERCUTS",chtitle.data(),60,0.,1.2);

  // supercluster seed energy / supercluster energy
  plotString = "E_{seed}/E_{SC}, endcap-only;E_{seed}/E_{SC};";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_scESeedOverESuperClusterEndcap_before_ = fs->make<TH1F>("ELE_SCESEEDOVERESUPERCLUSTERENDCAP_BEFORECUTS",chtitle.data(),60,0.,1.2);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_scESeedOverESuperClusterEndcap_after_ = fs->make<TH1F>("ELE_SCESEEDOVERESUPERCLUSTERENDCAP_AFTERCUTS",chtitle.data(),60,0.,1.2);

  // supercluster max energy / 3x3
  plotString = "E_{max}/E_{3x3}, endcap-only;E_{max}/E_{3x3};";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_rMax3x3Endcap_before_ = fs->make<TH1F>("ELE_RMAX3X3ENDCAP_BEFORECUTS",chtitle.data(),55,0.,1.1);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_rMax3x3Endcap_after_ = fs->make<TH1F>("ELE_RMAX3X3ENDCAP_AFTERCUTS",chtitle.data(),55,0.,1.1);



  // more shower shape, i.e. H/E

  // both barrel and endcap

  // hcal over ecal seed cluster energy using first hcal depth (hcal is energy of towers within dR=015). 
  plotString = "H/E_{seedcluster} for 1st HCAL depth, barrel+endcap;H/E;";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_hcalDepth1OverEcal_before_ = fs->make<TH1F>("ELE_HCALDEPTH1OVERECAL_BEFORECUTS",chtitle.data(),100,0.,1.);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_hcalDepth1OverEcal_after_ = fs->make<TH1F>("ELE_HCALDEPTH1OVERECAL_AFTERCUTS",chtitle.data(),100,0.,1.);

  // hcal over ecal seed cluster energy using 2nd hcal depth (hcal is energy of towers within dR=015). 
  plotString = "H/E_{seedcluster} for 2nd HCAL depth, barrel+endcap;H/E;";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_hcalDepth2OverEcal_before_ = fs->make<TH1F>("ELE_HCALDEPTH2OVERECAL_BEFORECUTS",chtitle.data(),100,0.,1.);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_hcalDepth2OverEcal_after_ = fs->make<TH1F>("ELE_HCALDEPTH2OVERECAL_AFTERCUTS",chtitle.data(),100,0.,1.);

  // simple sum of H/E for HCAL depths 1 and 2
  plotString = "H/E_{seedcluster} for both HCAL depths, barrel+endcap;H/E;";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_hcalOverEcal_before_ = fs->make<TH1F>("ELE_HCALOVERECAL_BEFORECUTS",chtitle.data(),100,0.,1.);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_hcalOverEcal_after_ = fs->make<TH1F>("ELE_HCALOVERECAL_AFTERCUTS",chtitle.data(),100,0.,1.);


  // barrel-only

  // hcal over ecal seed cluster energy using first hcal depth (hcal is energy of towers within dR=015). 
  plotString = "H/E_{seedcluster} for 1st HCAL depth, barrel-only;H/E;";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_hcalDepth1OverEcalBarrel_before_ = fs->make<TH1F>("ELE_HCALDEPTH1OVERECALBARREL_BEFORECUTS",chtitle.data(),100,0.,1.);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_hcalDepth1OverEcalBarrel_after_ = fs->make<TH1F>("ELE_HCALDEPTH1OVERECALBARREL_AFTERCUTS",chtitle.data(),100,0.,1.);

  // hcal over ecal seed cluster energy using 2nd hcal depth (hcal is energy of towers within dR=015). 
  plotString = "H/E_{seedcluster} for 2nd HCAL depth, barrel-only;H/E;";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_hcalDepth2OverEcalBarrel_before_ = fs->make<TH1F>("ELE_HCALDEPTH2OVERECALBARREL_BEFORECUTS",chtitle.data(),100,0.,1.);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_hcalDepth2OverEcalBarrel_after_ = fs->make<TH1F>("ELE_HCALDEPTH2OVERECALBARREL_AFTERCUTS",chtitle.data(),100,0.,1.);

  // simple sum of H/E for HCAL depths 1 and 2
  plotString = "H/E_{seedcluster} for both HCAL depths, barrel-only;H/E;";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_hcalOverEcalBarrel_before_ = fs->make<TH1F>("ELE_HCALOVERECALBARREL_BEFORECUTS",chtitle.data(),100,0.,1.);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_hcalOverEcalBarrel_after_ = fs->make<TH1F>("ELE_HCALOVERECALBARREL_AFTERCUTS",chtitle.data(),100,0.,1.);


  // endcap-only

  // hcal over ecal seed cluster energy using first hcal depth (hcal is energy of towers within dR=015). 
  plotString = "H/E_{seedcluster} for 1st HCAL depth, endcap-only;H/E;";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_hcalDepth1OverEcalEndcap_before_ = fs->make<TH1F>("ELE_HCALDEPTH1OVERECALENDCAP_BEFORECUTS",chtitle.data(),100,0.,1.);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_hcalDepth1OverEcalEndcap_after_ = fs->make<TH1F>("ELE_HCALDEPTH1OVERECALENDCAP_AFTERCUTS",chtitle.data(),100,0.,1.);

  // hcal over ecal seed cluster energy using 2nd hcal depth (hcal is energy of towers within dR=015). 
  plotString = "H/E_{seedcluster} for 2nd HCAL depth, endcap-only;H/E;";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_hcalDepth2OverEcalEndcap_before_ = fs->make<TH1F>("ELE_HCALDEPTH2OVERECALENDCAP_BEFORECUTS",chtitle.data(),100,0.,1.);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_hcalDepth2OverEcalEndcap_after_ = fs->make<TH1F>("ELE_HCALDEPTH2OVERECALENDCAP_AFTERCUTS",chtitle.data(),100,0.,1.);

  // simple sum of H/E for HCAL depths 1 and 2
  plotString = "H/E_{seedcluster} for both HCAL depths, endcap-only;H/E;";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_hcalOverEcalEndcap_before_ = fs->make<TH1F>("ELE_HCALOVERECALENDCAP_BEFORECUTS",chtitle.data(),100,0.,1.);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_hcalOverEcalEndcap_after_ = fs->make<TH1F>("ELE_HCALOVERECALENDCAP_AFTERCUTS",chtitle.data(),100,0.,1.);




  // MORE shower-shape ie sieie

  // barrel and endcap
  // sigma eta eta
  plotString = "#sigma_{#eta#eta}, barrel+endcap;#sigma_{#eta#eta};";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_sigmaEtaEta_before_ = fs->make<TH1F>("ELE_SIGMAETAETA_BEFORECUTS",chtitle.data(),100,0.,0.1);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_sigmaEtaEta_after_ = fs->make<TH1F>("ELE_SIGMAETAETA_AFTERCUTS",chtitle.data(),100,0.,0.1);

  // sigma ieta ieta
  plotString = "#sigma_{i#etai#eta}, barrel+endcap;#sigma_{i#etai#eta};";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_sigmaIetaIeta_before_ = fs->make<TH1F>("ELE_SIGMAIETAIETA_BEFORECUTS",chtitle.data(),100,0.,0.1);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_sigmaIetaIeta_after_ = fs->make<TH1F>("ELE_SIGMAIETAIETA_AFTERCUTS",chtitle.data(),100,0.,0.1);


  // barrel-only
  // sigma eta eta
  plotString = "#sigma_{#eta#eta}, barrel-only;#sigma_{#eta#eta};";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_sigmaEtaEtaBarrel_before_ = fs->make<TH1F>("ELE_SIGMAETAETABARREL_BEFORECUTS",chtitle.data(),100,0.,0.1);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_sigmaEtaEtaBarrel_after_ = fs->make<TH1F>("ELE_SIGMAETAETABARREL_AFTERCUTS",chtitle.data(),100,0.,0.1);

  // sigma ieta ieta
  plotString = "#sigma_{i#etai#eta}, barrel-only;#sigma_{i#etai#eta};";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_sigmaIetaIetaBarrel_before_ = fs->make<TH1F>("ELE_SIGMAIETAIETABARREL_BEFORECUTS",chtitle.data(),100,0.,0.1);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_sigmaIetaIetaBarrel_after_ = fs->make<TH1F>("ELE_SIGMAIETAIETABARREL_AFTERCUTS",chtitle.data(),100,0.,0.1);


  // endcap-only
  // sigma eta eta
  plotString = "#sigma_{#eta#eta}, endcap-only;#sigma_{#eta#eta};";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_sigmaEtaEtaEndcap_before_ = fs->make<TH1F>("ELE_SIGMAETAETAENDCAP_BEFORECUTS",chtitle.data(),100,0.,0.1);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_sigmaEtaEtaEndcap_after_ = fs->make<TH1F>("ELE_SIGMAETAETAENDCAP_AFTERCUTS",chtitle.data(),100,0.,0.1);

  // sigma ieta ieta
  plotString = "#sigma_{i#etai#eta}, endcap-only;#sigma_{i#etai#eta};";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_sigmaIetaIetaEndcap_before_ = fs->make<TH1F>("ELE_SIGMAIETAIETAENDCAP_BEFORECUTS",chtitle.data(),100,0.,0.1);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_sigmaIetaIetaEndcap_after_ = fs->make<TH1F>("ELE_SIGMAIETAIETAENDCAP_AFTERCUTS",chtitle.data(),100,0.,0.1);




  // bremstuff and otherstuff

  // number of basic clusters 
  plotString = "Number of basic clusters;# clusters;";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_basicClustersSize_before_ = fs->make<TH1F>("ELE_BASICCLUSTERSSIZE_BEFORECUTS",chtitle.data(),10,-0.5,9.5);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_basicClustersSize_after_ = fs->make<TH1F>("ELE_BASICCLUSTERSSIZE_AFTERCUTS",chtitle.data(),10,-0.5,9.5);

  // number of brems: this utility method returns the number of basic clusters minus one. 
  plotString = "Number of brems;# brems;";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_numberOfBrems_before_ = fs->make<TH1F>("ELE_NUMBEROFBREMS_BEFORECUTS",chtitle.data(),10,-0.5,9.5);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_numberOfBrems_after_ = fs->make<TH1F>("ELE_NUMBEROFBREMS_AFTERCUTS",chtitle.data(),10,-0.5,9.5);

  // the brem fraction from the gsf fit, ie (track momentum in - track momentum out) / track momentum in. 
  plotString = "Fraction of momentum lost to brem;fbrem;";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_fbrem_before_ = fs->make<TH1F>("ELE_FBREM_BEFORECUTS",chtitle.data(),60,-0.2,1.0);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_fbrem_after_ = fs->make<TH1F>("ELE_FBREM_AFTERCUTS",chtitle.data(),60,-0.2,1.0);

  // sc pixel charge 
  plotString = "supercluster-pixel charge;charge;";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_scPixCharge_before_ = fs->make<TH1F>("ELE_SCPIXCHARGE_BEFORECUTS",chtitle.data(),5,-2.5,2.5);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_scPixCharge_after_ = fs->make<TH1F>("ELE_SCPIXCHARGE_AFTERCUTS",chtitle.data(),5,-2.5,2.5);

  // gsf track charge 
  plotString = "GSF track charge;charge;";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_gsfCharge_before_ = fs->make<TH1F>("ELE_GSFCHARGE_BEFORECUTS",chtitle.data(),5,-2.5,2.5);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_gsfCharge_after_ = fs->make<TH1F>("ELE_GSFCHARGE_AFTERCUTS",chtitle.data(),5,-2.5,2.5);

  // ctf track charge 
  plotString = "CTF track charge;charge;";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_ctfCharge_before_ = fs->make<TH1F>("ELE_CTFCHARGE_BEFORECUTS",chtitle.data(),5,-2.5,2.5);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_ctfCharge_after_ = fs->make<TH1F>("ELE_CTFCHARGE_AFTERCUTS",chtitle.data(),5,-2.5,2.5);

  // charge consistency between sc pix and gsf track
  plotString = "charge consistency between SC-pixel and GSF track;bool IsChargeConsistent;";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_gsfScPixChargeConsistent_before_ = fs->make<TH1F>("ELE_GSFSCPIXCHARGECONSISTENT_BEFORECUTS",chtitle.data(),2,0.,2.);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_gsfScPixChargeConsistent_after_ = fs->make<TH1F>("ELE_GSFSCPIXCHARGECONSISTENT_AFTERCUTS",chtitle.data(),2,0.,2.);

  // charge consistency between ctf and gsf tracks
  plotString = "charge consistency between CTF and GSF tracks;bool IsChargeConsistent;";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_gsfCtfChargeConsistent_before_ = fs->make<TH1F>("ELE_GSFCTFCHARGECONSISTENT_BEFORECUTS",chtitle.data(),2,0.,2.);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_gsfCtfChargeConsistent_after_ = fs->make<TH1F>("ELE_GSFCTFCHARGECONSISTENT_AFTERCUTS",chtitle.data(),2,0.,2.);

  // charge consistency between sc pix and ctf and gsf tracks
  plotString = "charge consistency between SC-pixel and CTF and GSF tracks;bool IsChargeConsistent;";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_gsfCtfScPixChargeConsistent_before_ = fs->make<TH1F>("ELE_GSFCTFSCPIXCHARGECONSISTENT_BEFORECUTS",chtitle.data(),2,0.,2.);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_gsfCtfScPixChargeConsistent_after_ = fs->make<TH1F>("ELE_GSFCTFSCPIXCHARGECONSISTENT_AFTERCUTS",chtitle.data(),2,0.,2.);


  // CANDIDATE

  // candidate charge
  plotString = "charge;charge;";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_charge_before_ = fs->make<TH1F>("ELE_CHARGE_BEFORECUTS",chtitle.data(),5,-2.5,2.5);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_charge_after_ = fs->make<TH1F>("ELE_CHARGE_AFTERCUTS",chtitle.data(),5,-2.5,2.5);

  // candidate energy
  plotString = "energy;energy (GeV);";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_energy_before_ = fs->make<TH1F>("ELE_ENERGY_BEFORECUTS",chtitle.data(),100,0.,100.);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_energy_after_ = fs->make<TH1F>("ELE_ENERGY_AFTERCUTS",chtitle.data(),100,0.,100.);

  // candidate transverse energy
  plotString = "E_{T};E_{T} (GeV);";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_et_before_ = fs->make<TH1F>("ELE_ET_BEFORECUTS",chtitle.data(),100,0.,100.);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_et_after_ = fs->make<TH1F>("ELE_ET_AFTERCUTS",chtitle.data(),100,0.,100.);

  // candidate momentum magnitude
  plotString = "momentum;|p| (GeV);";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_p_before_ = fs->make<TH1F>("ELE_P_BEFORECUTS",chtitle.data(),100,0.,100.);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_p_after_ = fs->make<TH1F>("ELE_P_AFTERCUTS",chtitle.data(),100,0.,100.);

  // candidate momentum in the x direction
  plotString = "momentum in x-direction;p_{x} (GeV);";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_px_before_ = fs->make<TH1F>("ELE_PX_BEFORECUTS",chtitle.data(),100,-100.,100.);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_px_after_ = fs->make<TH1F>("ELE_PX_AFTERCUTS",chtitle.data(),100,-100.,100.);

  // candidate momentum in the y direction
  plotString = "momentum in y-direction;p_{y} (GeV);";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_py_before_ = fs->make<TH1F>("ELE_PY_BEFORECUTS",chtitle.data(),100,-100.,100.);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_py_after_ = fs->make<TH1F>("ELE_PY_AFTERCUTS",chtitle.data(),100,-100.,100.);

  // candidate momentum in the z direction
  plotString = "momentum in z-direction;p_{z} (GeV);";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_pz_before_ = fs->make<TH1F>("ELE_PZ_BEFORECUTS",chtitle.data(),100,-100.,100.);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_pz_after_ = fs->make<TH1F>("ELE_PZ_AFTERCUTS",chtitle.data(),100,-100.,100.);



  // vertex chi^2
  plotString = "vertex #chi^{2};#chi^{2};";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_vertexChi2_before_ = fs->make<TH1F>("ELE_VERTEXCHI2_BEFORECUTS",chtitle.data(),100,0.,10.);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_vertexChi2_after_ = fs->make<TH1F>("ELE_VERTEXCHI2_AFTERCUTS",chtitle.data(),100,0.,10.);

  // vertex number of degrees of freedom
  plotString = "vertex # degrees of freedom;nDOF;";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_vertexNdof_before_ = fs->make<TH1F>("ELE_VERTEXNDOF_BEFORECUTS",chtitle.data(),100,0.,10.);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_vertexNdof_after_ = fs->make<TH1F>("ELE_VERTEXNDOF_AFTERCUTS",chtitle.data(),100,0.,10.);

  // vertex normalized chi^2
  plotString = "vertex normalized #chi^{2};#chi^{2};";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_vertexNormalizedChi2_before_ = fs->make<TH1F>("ELE_VERTEXNORMALIZEDCHI2_BEFORECUTS",chtitle.data(),100,0.,10.);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_vertexNormalizedChi2_after_ = fs->make<TH1F>("ELE_VERTEXNORMALIZEDCHI2_AFTERCUTS",chtitle.data(),100,0.,10.);

//   // beamspot position in the x direction
//   plotString = "beamspot position in x-direction;v_{x} (cm);";
//   chtitle = collName+"before cuts: "+plotString;
//   h_ele_BSx_before_ = fs->make<TH1F>("ELE_XBS_BEFORECUTS",chtitle.data(),50,-2.5,2.5);
//   chtitle = collName+"after cuts: "+plotString;
//   h_ele_BSx_after_ = fs->make<TH1F>("ELE_XBS_AFTERCUTS",chtitle.data(),50,-2.5,2.5);

//   // beamspot position in the y direction
//   plotString = "beamspot position in y-direction;v_{y} (cm);";
//   chtitle = collName+"before cuts: "+plotString;
//   h_ele_yBS_before_ = fs->make<TH1F>("ELE_YBS_BEFORECUTS",chtitle.data(),50,-2.5,2.5);
//   chtitle = collName+"after cuts: "+plotString;
//   h_ele_yBS_after_ = fs->make<TH1F>("ELE_YBS_AFTERCUTS",chtitle.data(),50,-2.5,2.5);

//   // beamspot position in the z direction
//   plotString = "beamspot position in z-direction;v_{z} (cm);";
//   chtitle = collName+"before cuts: "+plotString;
//   h_ele_zBS_before_ = fs->make<TH1F>("ELE_ZBS_BEFORECUTS",chtitle.data(),50,-25.,25.);
//   chtitle = collName+"after cuts: "+plotString;
//   h_ele_zBS_after_ = fs->make<TH1F>("ELE_ZBS_AFTERCUTS",chtitle.data(),50,-25.,25.);

  // vertex position in the x direction
  plotString = "vertex position in x-direction;v_{x} (cm);";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_vx_before_ = fs->make<TH1F>("ELE_VX_BEFORECUTS",chtitle.data(),50,-2.5,2.5);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_vx_after_ = fs->make<TH1F>("ELE_VX_AFTERCUTS",chtitle.data(),50,-2.5,2.5);

  // vertex position in the y direction
  plotString = "vertex position in y-direction;v_{y} (cm);";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_vy_before_ = fs->make<TH1F>("ELE_VY_BEFORECUTS",chtitle.data(),50,-2.5,2.5);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_vy_after_ = fs->make<TH1F>("ELE_VY_AFTERCUTS",chtitle.data(),50,-2.5,2.5);

  // vertex position in the z direction
  plotString = "vertex position in z-direction;v_{z} (cm);";
  chtitle = collName+"before cuts: "+plotString;
  h_ele_vz_before_ = fs->make<TH1F>("ELE_VZ_BEFORECUTS",chtitle.data(),50,-25.,25.);
  chtitle = collName+"after cuts: "+plotString;
  h_ele_vz_after_ = fs->make<TH1F>("ELE_VZ_AFTERCUTS",chtitle.data(),50,-25.,25.);



  // supercluster quantities
  // both barrel and endcap

  collName = "(Superclusters) ";

  // number of superclusters in event
  chtitle = collName+"Supercluster: Number of superclusters in event;# Superclusters;";
  h_sc_nSuperClusters_ = fs->make<TH1F>("SC_NSUPERCLUSTERS",chtitle.data(),20,-0.5,19.5);

  // supercluster energy
  chtitle = collName+"Supercluster: energy, barrel+endcap;E (GeV);";
  h_sc_energy_ = fs->make<TH1F>("SC_ENERGY",chtitle.data(),100,0.,100.);

  // supercluster eta
  chtitle = collName+"Supercluster: #eta;#eta;";
  h_sc_eta_ = fs->make<TH1F>("SC_ETA",chtitle.data(),50,-2.5,2.5);

  // supercluster phi
  chtitle = collName+"Supercluster: #phi, barrel+endcap;#phi;";
  h_sc_phi_ = fs->make<TH1F>("SC_PHI",chtitle.data(),50,-3.14,3.14);

  // raw supercluster energy
  chtitle = collName+"Supercluster: raw energy, barrel+endcap;E (GeV);";
  h_sc_rawEnergy_ = fs->make<TH1F>("SC_RAWENERGY",chtitle.data(),100,0.,100.);

//   // preshower energy
//   chtitle = collName+"Supercluster: Preshower energy;E (GeV);";
//   h_sc_preshowerEnergy_ = fs->make<TH1F>("SC_PRESHOWERENERGY",chtitle.data(),100,0.,50.);

  // supercluster eta width
  chtitle = collName+"Supercluster: #eta width, barrel+endcap;#eta;";
  h_sc_etaWidth_ = fs->make<TH1F>("SC_ETAWIDTH",chtitle.data(),30,0.,0.3);

  // supercluster phi width
  chtitle = collName+"Supercluster: #phi width, barrel+endcap;#phi;";
  h_sc_phiWidth_ = fs->make<TH1F>("SC_PHIWIDTH",chtitle.data(),30,0.,0.3);

  // number of basic clusters in supercluster
  chtitle = collName+"Supercluster: Number of basic clusters, barrel+endcap;# clusters;";
  h_sc_nBasicClusters_ = fs->make<TH1F>("SC_NBASICCLUSTERS",chtitle.data(),10,-0.5,9.5);

  // r9 of supercluster
  chtitle = collName+"Supercluster: R9, barrel+endcap;r9;";
  h_sc_r9_ = fs->make<TH1F>("SC_R9",chtitle.data(),60,0.,1.2);
  
  // energy of seed cluster / energy of supercluster
  chtitle = collName+"Supercluster: E_{seed}/E_{SC}, barrel+endcap;Energy (GeV);";
  h_sc_eSeedOverESuperCluster_ = fs->make<TH1F>("SC_ESEEDOVERESUPERCLUSTER",chtitle.data(),60,0.,1.2);
  


  // barrel
  // supercluster energy
  chtitle = collName+"Supercluster: energy, barrel-only;E (GeV);";
  h_sc_energyBarrel_ = fs->make<TH1F>("SC_ENERGYBARREL",chtitle.data(),100,0.,100.);

  // supercluster phi
  chtitle = collName+"Supercluster: #phi, barrel-only;#phi;";
  h_sc_phiBarrel_ = fs->make<TH1F>("SC_PHIBARREL",chtitle.data(),50,-3.14,3.14);

  // raw supercluster energy
  chtitle = collName+"Supercluster: raw energy, barrel-only;E (GeV);";
  h_sc_rawEnergyBarrel_ = fs->make<TH1F>("SC_RAWENERGYBARREL",chtitle.data(),100,0.,100.);

//   // preshower energy
//   chtitle = collName+"Supercluster: Preshower energy, barrel-only;E (GeV);";
//   h_sc_preshowerEnergyBarrel_ = fs->make<TH1F>("SC_PRESHOWERENERGYBARREL",chtitle.data(),100,0.,50.);

  // supercluster eta width
  chtitle = collName+"Supercluster: #eta width, barrel-only;#eta;";
  h_sc_etaWidthBarrel_ = fs->make<TH1F>("SC_ETAWIDTHBARREL",chtitle.data(),30,0.,0.3);

  // supercluster phi width
  chtitle = collName+"Supercluster: #phi width, barrel-only;#phi;";
  h_sc_phiWidthBarrel_ = fs->make<TH1F>("SC_PHIWIDTHBARREL",chtitle.data(),30,0.,0.3);

  // number of basic clusters in supercluster
  chtitle = collName+"Supercluster: Number of basic clusters, barrel-only;# clusters;";
  h_sc_nBasicClustersBarrel_ = fs->make<TH1F>("SC_NBASICCLUSTERSBARREL",chtitle.data(),10,-0.5,9.5);

  // r9 of supercluster, barrel-only
  chtitle = collName+"Supercluster: R9, barrel-only;r9;";
  h_sc_r9Barrel_ = fs->make<TH1F>("SC_R9BARREL",chtitle.data(),60,0.,1.2);
  
  // energy of seed cluster / energy of supercluster, barrel-only
  chtitle = collName+"Supercluster: E_{seed}/E_{SC}, barrel-only;Energy (GeV);";
  h_sc_eSeedOverESuperClusterBarrel_ = fs->make<TH1F>("SC_ESEEDOVERESUPERCLUSTERBARREL",chtitle.data(),60,0.,1.2);
  

  // endcap
  // supercluster energy
  chtitle = collName+"Supercluster: energy, endcap-only;E (GeV);";
  h_sc_energyEndcap_ = fs->make<TH1F>("SC_ENERGYENDCAP",chtitle.data(),100,0.,100.);

  // supercluster phi
  chtitle = collName+"Supercluster: #phi, endcap-only;#phi;";
  h_sc_phiEndcap_ = fs->make<TH1F>("SC_PHIENDCAP",chtitle.data(),50,-3.14,3.14);

  // raw supercluster energy
  chtitle = collName+"Supercluster: raw energy, endcap-only;E (GeV);";
  h_sc_rawEnergyEndcap_ = fs->make<TH1F>("SC_RAWENERGYENDCAP",chtitle.data(),100,0.,100.);

  // preshower energy
  chtitle = collName+"Supercluster: Preshower energy, endcap-only;E (GeV);";
  h_sc_preshowerEnergyEndcap_ = fs->make<TH1F>("SC_PRESHOWERENERGYENDCAP",chtitle.data(),100,0.,50.);

  // supercluster eta width
  chtitle = collName+"Supercluster: #eta width, endcap-only;#eta;";
  h_sc_etaWidthEndcap_ = fs->make<TH1F>("SC_ETAWIDTHENDCAP",chtitle.data(),30,0.,0.3);

  // supercluster phi width
  chtitle = collName+"Supercluster #phi width, endcap-only;#phi;";
  h_sc_phiWidthEndcap_ = fs->make<TH1F>("SC_PHIWIDTHENDCAP",chtitle.data(),30,0.,0.3);

  // number of basic clusters in supercluster
  chtitle = collName+"Supercluster: Number of basic clusters, endcap-only;# clusters;";
  h_sc_nBasicClustersEndcap_ = fs->make<TH1F>("SC_NBASICCLUSTERSENDCAP",chtitle.data(),10,-0.5,9.5);

  // r9 of supercluster, endcap-only
  chtitle = collName+"Supercluster: R9, endcap-only;r9;";
  h_sc_r9Endcap_ = fs->make<TH1F>("SC_R9ENDCAP",chtitle.data(),60,0.,1.2);
  
  // energy of seed cluster / energy of supercluster, endcap-only
  chtitle = collName+"Supercluster: E_{seed}/E_{SC}, endcap-only;Energy (GeV);";
  h_sc_eSeedOverESuperClusterEndcap_ = fs->make<TH1F>("SC_ESEEDOVERESUPERCLUSTERENDCAP",chtitle.data(),60,0.,1.2);
  


  // Z candidate quantities

  collName = "(Z candidates) ";

  // pt of all Z cands
  plotString = "p_{T} of all Z candidates;p_{T} (GeV);";
  chtitle = collName+"before cuts: "+plotString;
  h_Z_pt_before_ = fs->make<TH1F>("Z_PT_BEFORECUTS",chtitle.data(),100,0.,100.);
  chtitle = collName+"after cuts: "+plotString;
  h_Z_pt_after_ = fs->make<TH1F>("Z_PT_AFTERCUTS",chtitle.data(),100,0.,100.);

  // eta of all Z cands
  plotString = "#eta of all Z candidates;#eta;";
  chtitle = collName+"before cuts: "+plotString;
  h_Z_eta_before_ = fs->make<TH1F>("Z_ETA_BEFORECUTS",chtitle.data(),50,-2.5,2.5);
  chtitle = collName+"after cuts: "+plotString;
  h_Z_eta_after_ = fs->make<TH1F>("Z_ETA_AFTERCUTS",chtitle.data(),50,-2.5,2.5);

  // phi of all Z cands
  plotString = "#phi of all Z candidates;#phi;";
  chtitle = collName+"before cuts: "+plotString;
  h_Z_phi_before_ = fs->make<TH1F>("Z_PHI_BEFORECUTS",chtitle.data(),50,-3.14,3.14);
  chtitle = collName+"after cuts: "+plotString;
  h_Z_phi_after_ = fs->make<TH1F>("Z_PHI_AFTERCUTS",chtitle.data(),50,-3.14,3.14);

  // mass of all Z cands
  plotString = "mass of all Z candidates;m_{Z} (GeV);";
  chtitle = collName+"before cuts: "+plotString;
  h_Z_mass_before_ = fs->make<TH1F>("Z_MASS_BEFORECUTS",chtitle.data(),100,0.,200.);
  chtitle = collName+"after cuts: "+plotString;
  h_Z_mass_after_ = fs->make<TH1F>("Z_MASS_AFTERCUTS",chtitle.data(),100,0.,200.);


  //   // pt of top Z cand
  //  plotString = 
  //   chtitle = collName+"p_{T} of top Z candidate;p_{T} (GeV);";
  //   Z_pt_top_before_ = fs->make<TH1F>("Z_PT_TOP_BEFORECUTS",chtitle.data(),100,0.,100.);
  //   Z_pt_top_after_ = fs->make<TH1F>("Z_PT_TOP_AFTERCUTS",chtitle.data(),100,0.,100.);

  //   // eta of top Z cand
    //  plotString = 
  //   chtitle = collName+"#eta of top Z candidate;#eta;";
  //   Z_eta_top_before_ = fs->make<TH1F>("Z_ETA_TOP_BEFORECUTS",chtitle.data(),50,-2.5,2.5);
  //   Z_eta_top_after_ = fs->make<TH1F>("Z_ETA_TOP_AFTERCUTS",chtitle.data(),50,-2.5,2.5);

  //   // phi of top Z cand
    //  plotString = 
  //   chtitle = collName+"#phi of top Z candidate;#phi;";
  //   Z_phi_top_before_ = fs->make<TH1F>("Z_PHI_TOP_BEFORECUTS",chtitle.data(),50,-3.14,3.14);
  //   Z_phi_top_after_ = fs->make<TH1F>("Z_PHI_TOP_AFTERCUTS",chtitle.data(),50,-3.14,3.14);

  //   // mass of top Z cand
    //  plotString = 
  //   chtitle = collName+"mass of top Z candidate;m_{Z} (GeV);";
  //   Z_mass_top_before_ = fs->make<TH1F>("Z_MASS_TOP_BEFORECUTS",chtitle.data(),100,0.,200.);
  //   Z_mass_top_after_ = fs->make<TH1F>("Z_MASS_TOP_AFTERCUTS",chtitle.data(),100,0.,200.);


  bookPfHistos( pfCandHisto );

  bookJetHistos( CALO, caloJetHisto);
  if ( useCorrCaloJets_) bookJetHistos( CALO_CORR, caloCorrJetHisto);
  bookJetHistos( PF, pfJetHisto);
  if ( useCorrPfJets_ ) bookJetHistos( PF_CORR, pfCorrJetHisto);
  if ( useGenJets_ ) bookJetHistos( GEN, genJetHisto);
  
  ////////MET STUFF
  bookMetHistos( CALO, caloMetHisto, wBosonCaloMetHisto);
  if ( useCorrCaloMet_) bookMetHistos( CALO_CORR, caloCorrMetHisto, wBosonCaloCorrMetHisto);
  bookMetHistos( PF, pfMetHisto, wBosonPfMetHisto);
  if ( useCorrPfMet_ ) bookMetHistos( PF_CORR, pfCorrMetHisto, wBosonPfCorrMetHisto);
  bookMetHistos( TC, tcMetHisto, wBosonTcMetHisto);
  bookMetHistos( GEN, genMetHisto, wBosonGenMetHisto);
  
}

void EwkElecStandaloneDQM::bookPfHistos( std::map<std::string, TH1F*> & map ) {
//   reco::PFCandidate (particleFlow, particleFlow)("", "electrons");
//   reco:PFCluster (particleFlowClusterECAL,particleFlowClusterHCAL,particleFlowClusterHFEM, particleFlowClusterHFHAD,particleFlowClusterPS) ("");
//   reco::PFBlock(particleFlowBlock)("");
  ;
  /*
    std::string when[2] = { "before", "after" };

    vector<PlotVariable> varsCommon;
    varsCommon.push_back( PlotVariable("Et",     20,  0., 200.) );
    varsCommon.push_back( PlotVariable("Ex",    20, -50.,   50.) );
    varsCommon.push_back( PlotVariable("Ey",    20, -50.,   50.) );
    varsCommon.push_back( PlotVariable("sumEt",     20,  0., 500.) );
    varsCommon.push_back( PlotVariable("Energy", 20,  0., 200.) );
    varsCommon.push_back( PlotVariable("Phi", 20,  -5., 5.) );

    for ( int i = 0; i < 2; ++i ) {
      
      for ( std::vector<PlotVariable>::iterator itr = varsCommon.begin(); itr != varsCommon.end(); ++itr ) {
	label = (*itr).name + " (" + collection + ") ;" + (*itr).name;
	hname = (*itr).name + "_" + when[i];
	map[hname] = fs->make<TH1F>((boost::to_upper_copy("MET_"+metType+"_"+hname)).data(), label.data(), (*itr).nBins, (*itr).binLow, (*itr).binHigh);
      }
    }

  */

//     std::string when[2] = { "before", "after" };
//     std::string type[6] = { "all", "chargedHad", "chargedEM", "muon", "neutralEM", "neutralHad" };
//     vector<PlotVariable> varsCommon;
//     //varsCommon.push_back( PlotVariable("particleID",     10,  0., 10.) );
//     varsCommon.push_back( PlotVariable("Et",    20,   0.,  150.) );
//     varsCommon.push_back( PlotVariable("Ex",    20, -50.,   50.) );
//     varsCommon.push_back( PlotVariable("Ey",    20, -50.,   50.) );
//     varsCommon.push_back( PlotVariable("Ez",    20, -50.,   50.) );
//     varsCommon.push_back( PlotVariable("sumEt",     20,  0., 500.) );
//     varsCommon.push_back( PlotVariable("Energy", 20,  0., 200.) );
//     varsCommon.push_back( PlotVariable("Phi", 20,  -5., 5.) );

//     for ( int i = 0; i < 2; ++i ) {
//       for ( int j = 0; j < 6; ++j ) {
      
// 	for ( std::vector<PlotVariable>::iterator itr = varsCommon.begin(); itr != varsCommon.end(); ++itr ) {
// 	  label = (*itr).name + " (" + type[j] + ") ;" + (*itr).name;
// 	  hname = (*itr).name + "_" + type[j] + "_" + when[i];
// 	  map[hname] = fs->make<TH1F>((boost::to_upper_copy("PFCANDIDATE_"+type[j]+"_"+hname)).data(), label.data(), (*itr).nBins, (*itr).binLow, (*itr).binHigh);
// 	}
//       }
//     }


} 

void EwkElecStandaloneDQM::fillPfCandidateHistos(std::string type, std::string when) {
//   reco::PFCandidate (particleFlow, particleFlow)("", "electrons");
//   reco:PFCluster (particleFlowClusterECAL,particleFlowClusterHCAL,particleFlowClusterHFEM, particleFlowClusterHFHAD,particleFlowClusterPS) ("");
//   reco::PFBlock(particleFlowBlock)("");l

//NEED TO PASS EVENT OR DO THIS ELSEWHERE
//   if (!ev.getByLabel(pfCandTag_, pfCandidateCollection_)) {
//     LogError("") << ">>> pf candidate collection does not exist !!!";
//     return false;
//   }
//   std::map<std::string, TH1F*> * map;
//   map = &pfCandHisto; 
//   edm::Handle< reco::PFCandidateCollection > pfCandidates;
//   pfCandidates = pfCanditateCollection_;
  
//   for ( PFCandidateCollection::const_iterator itr = pfCandidates->begin(); itr != pfCandidates->end(); ++itr ) { 

//     //   (*map)["particleID_" + when]->Fill(itr.particleId());//1 =chargedHad, 2 = ChargedEM, 3 = Muon, 4 = NeutralEM, 5 = NeutralHad, 6&7 = ?
//     (*map)["Energy_" + type + when]->Fill(itr.energy());
    

//   }
  ;
}     



void EwkElecStandaloneDQM::endJob() 
{
//   std::cout << "HISTOGRAMS NOW BEING DIVIDED" << std::endl;

//   // divide the efficiency histograms
//   trg_HLT_Photon30_L1R_eff_->Divide(trg_Denominator_eff_);
//   trg_HLT_Ele20_LW_L1R_eff_->Divide(trg_Denominator_eff_);
//   trg_HLT_Photon20_L1R_eff_->Divide(trg_Denominator_eff_);
//   trg_HLT_Ele15_LW_L1R_eff_->Divide(trg_Denominator_eff_);
//   trg_HLT_Ele15_SiStrip_L1R_eff_->Divide(trg_Denominator_eff_);
//   trg_HLT_Photon15_L1R_eff_->Divide(trg_Denominator_eff_);
//   trg_HLT_Photon15_TrackIso_L1R_eff_->Divide(trg_Denominator_eff_);
//   trg_HLT_Photon15_LooseEcalIso_L1R_eff_->Divide(trg_Denominator_eff_);
//   trg_HLT_Ele10_LW_L1R_eff_->Divide(trg_Denominator_eff_);
//   trg_HLT_Ele10_LW_EleId_L1R_eff_->Divide(trg_Denominator_eff_);
//   trg_HLT_Photon10_L1R_eff_->Divide(trg_Denominator_eff_);
//   trg_HLT_L1SingleEG8_eff_->Divide(trg_Denominator_eff_);
//   trg_HLT_L1SingleEG5_eff_->Divide(trg_Denominator_eff_); 

}

void EwkElecStandaloneDQM::endRun(const Run& r, const EventSetup&) {

  // overall
  double all = nall;
  double esel = nsel/all;
  LogVerbatim("") << "\n>>>>>> SELECTION SUMMARY BEGIN >>>>>>>>>>>>>>>";
  LogVerbatim("") << "Total number of events analyzed: " << nall << " [events]";
  LogVerbatim("") << "Total number of events selected: " << nsel << " [events]";
  LogVerbatim("") << "Overall efficiency:             " << "(" << setprecision(4) << esel*100. <<" +/- "<< setprecision(2) << sqrt(esel*(1-esel)/all)*100. << ")%";
  
  double erec = nrec/all;
  double eeid = neid/all;
  double eiso = niso/all;
//   double ehlt = nhlt/all;
//   double emet = nmet/all;
  
  // general reconstruction step??
  double num = nrec;
  double eff = erec;
  double err = sqrt(eff*(1-eff)/all);
  LogVerbatim("") << "Passing Pt/Eta/Quality cuts:    " << num << " [events], (" << setprecision(4) << eff*100. <<" +/- "<< setprecision(2) << err*100. << ")%";

  // electron ID step  
  num = neid;
  eff = eeid;
  err = sqrt(eff*(1-eff)/all);
  double effstep = 0.;
  double errstep = 0.;
  if (nrec>0) effstep = eeid/erec;
  if (nrec>0) errstep = sqrt(effstep*(1-effstep)/nrec);
  LogVerbatim("") << "Passing eID cuts:         " << num << " [events], (" << setprecision(4) << eff*100. <<" +/- "<< setprecision(2) << err*100. << ")%, to previous step: (" <<  setprecision(4) << effstep*100. << " +/- "<< setprecision(2) << errstep*100. <<")%";
  
  // isolation step  
  num = niso;
  eff = eiso;
  err = sqrt(eff*(1-eff)/all);
  effstep = 0.;
  errstep = 0.;
  if (neid>0) effstep = eiso/eeid;
  if (neid>0) errstep = sqrt(effstep*(1-effstep)/neid);
  LogVerbatim("") << "Passing isolation cuts:         " << num << " [events], (" << setprecision(4) << eff*100. <<" +/- "<< setprecision(2) << err*100. << ")%, to previous step: (" <<  setprecision(4) << effstep*100. << " +/- "<< setprecision(2) << errstep*100. <<")%";
  
//   // trigger step
//   num = nhlt;
//   eff = ehlt;
//   err = sqrt(eff*(1-eff)/all);
//   effstep = 0.;
//   errstep = 0.;
//   if (niso>0) effstep = ehlt/eiso;
//   if (niso>0) errstep = sqrt(effstep*(1-effstep)/niso);
//   LogVerbatim("") << "Passing HLT criteria:           " << num << " [events], (" << setprecision(4) << eff*100. <<" +/- "<< setprecision(2) << err*100. << ")%, to previous step: (" <<  setprecision(4) << effstep*100. << " +/- "<< setprecision(2) << errstep*100. <<")%";
  
  // trigger step
  num = nsel;
  eff = esel;
  err = sqrt(eff*(1-eff)/all);
  effstep = 0.;
  errstep = 0.;
  if (niso>0) effstep = esel/eiso;
  if (niso>0) errstep = sqrt(effstep*(1-effstep)/niso);
  LogVerbatim("") << "Passing HLT criteria:           " << num << " [events], (" << setprecision(4) << eff*100. <<" +/- "<< setprecision(2) << err*100. << ")%, to previous step: (" <<  setprecision(4) << effstep*100. << " +/- "<< setprecision(2) << errstep*100. <<")%";
  
//   // met/acoplanarity cuts 
//   num = nmet;
//   eff = emet;
//   err = sqrt(eff*(1-eff)/all);
//   effstep = 0.;
//   errstep = 0.;
//   if (nhlt>0) effstep = emet/ehlt;
//   if (nhlt>0) errstep = sqrt(effstep*(1-effstep)/nhlt);
//   LogVerbatim("") << "Passing MET/acoplanarity cuts:  " << num << " [events], (" << setprecision(4) << eff*100. <<" +/- "<< setprecision(2) << err*100. << ")%, to previous step: (" <<  setprecision(4) << effstep*100. << " +/- "<< setprecision(2) << errstep*100. <<")%";
  
//   // Z/top selection cuts ALSO LAST STEP so "sel" for "selection"
//   num = nsel;
//   eff = esel;
//   err = sqrt(eff*(1-eff)/all);
//   effstep = 0.;
//   errstep = 0.;
//   if (nmet>0) effstep = esel/emet;
//   if (nmet>0) errstep = sqrt(effstep*(1-effstep)/nmet);
//   LogVerbatim("") << "Passing Z/top rejection cuts:   " << num << " [events], (" << setprecision(4) << eff*100. <<" +/- "<< setprecision(2) << err*100. << ")%, to previous step: (" <<  setprecision(4) << effstep*100. << " +/- "<< setprecision(2) << errstep*100. <<")%";
  
  LogVerbatim("") << ">>>>>> SELECTION SUMMARY END   >>>>>>>>>>>>>>>\n";


}




void EwkElecStandaloneDQM::analyze (const Event & ev, const EventSetup & es) {
//  LogWarning("") << ">>> analyze";

      // Reset global event selection flags
      bool rec_sel = false;
      bool eid_sel = false;
      bool iso_sel = false;
//       bool hlt_sel = false;
      bool met_sel = false;
      bool all_sel = false;

      // Getting the event information
      edm::EventID eventId = ev.id();
      runNumber_ = eventId.run();
      lumiSection_ = eventId.luminosityBlock();
      eventNumber_ = eventId.event();


//       // Muon collection
//       Handle<View<Muon> > muonCollection;
//       if (!ev.getByLabel(muonTag_, muonCollection)) {
//             LogWarning("") << ">>> Muon collection does not exist !!!";
//             return;
//       }
//       unsigned int muonCollectionSize = muonCollection->size();

      // Electron collection
      if (!ev.getByLabel(elecTag_, electronCollection)) {
            LogWarning("") << ">>> Electron collection does not exist !!!";
            return;
      }
      ele_nelectrons_ = electronCollection->size();
      if (ele_nelectrons_ > k_ELEC_MAX)
	{
	  LogWarning("") << "More than " << k_ELEC_MAX << " electrons in the collection!  "
			 << "Electrons beyond #20 will be ignored";
	}

      // Beam spot
      Handle<reco::BeamSpot> beamSpotHandle;
      if (!ev.getByLabel(InputTag("offlineBeamSpot"), beamSpotHandle)) {
            LogWarning("") << ">>> No beam spot found !!!";
            return;
      }

      // Supercluster collections
      if (!ev.getByLabel(superClustersEBTag_, superClustersEBCollection)) {
            LogWarning("") << ">>> Superclusters EB collection does not exist !!!";
            return;
      }
      sc_nSuperClustersBarrel_ = superClustersEBCollection->size();
      if (!ev.getByLabel(superClustersEETag_, superClustersEECollection)) {
            LogWarning("") << ">>> Superclusters EE collection does not exist !!!";
            return;
      }
      sc_nSuperClustersEndcap_ = superClustersEECollection->size();

      if ( (int) sc_nSuperClustersBarrel_ > k_SC_MAX)
	{
	  LogWarning("") << "More than " << k_SC_MAX << " superclusters in the barrel!  "
			 << "SCs beyond this will be ignored";
	}
      if ( (int) sc_nSuperClustersEndcap_ > k_SC_MAX)
	{
	  LogWarning("") << "More than " << k_SC_MAX << " superclusters in the endcap!  "
			 << "SCs beyond this will be ignored";
	}

      sc_nSuperClusters_ = sc_nSuperClustersBarrel_ + sc_nSuperClustersEndcap_;

//       if (!ev.getByLabel(recHitsEBTag_,recHitsEBCollection)) 
// 	{
// 	  LogWarning("") << ">>> RecHits EB collection does not exist !!!";
// 	  return;
// 	}
//       if (!ev.getByLabel(recHitsEETag_,recHitsEECollection)) 
// 	{
// 	  LogWarning("") << ">>> RecHits EE collection does not exist !!!";
// 	  return;
// 	}
      EcalClusterLazyTools lazyTool(ev, es, recHitsEBTag_, recHitsEETag_);      

      // Trigger (HLT)
      Handle<TriggerResults> triggerResults;
      TriggerNames trigNames; // 36X: line deleted 
      if (!ev.getByLabel(trigTag_, triggerResults)) {
            LogWarning("") << ">>> TRIGGER collection does not exist !!!";
            return;
      }
      trigNames.init(*triggerResults); // 36X: const edm::TriggerNames & trigNames = ev.triggerNames(*triggerResults);
      bool trigger_fired = false;

      //      std::map<std::string,bool> * triggersOfInterest;
      //std::map<std::string,bool> triggersOfInterest;

      triggersOfInterest_.clear();
      l1TriggersOfInterest_.clear();

      /*
      for (unsigned int i=0; i<triggerResults->size(); i++) {
            if (triggerResults->accept(i)) {
                  LogTrace("") << "Accept by: " << i << ", Trigger: " << trigNames.triggerName(i);
            }
      }
      */

      

      // clearing previous event's triggers
//       triggersOfInterest_.clear();
//       l1TriggersOfInterest_.clear();
//      std::cout << "CLEARING PREVIOUS LIST" << std::endl;
      triggersOfInterest_->clear();
      l1TriggersOfInterest_->clear();

      // the following gives error on CRAFT08 data where itrig1=19 (vector index out of range)
      /*
      int itrig1 = trigNames.triggerIndex(muonTrig_);
      if (triggerResults->accept(itrig1)) trigger_fired = true;
      */
      //suggested replacement: lm250909
      std::vector<std::string> passedTriggers;
      for (unsigned int i=0; i<triggerResults->size(); i++) 
	{
	  std::string trigName = trigNames.triggerName(i);
	  for (unsigned int j = 0; j < elecTrig_.size(); j++)
	    {
	      if ( (trigName.find("Ele") != std::string::npos) || (trigName.find("EG") != std::string::npos) 
		     || (trigName.find("Jet") != std::string::npos) || (trigName.find("Bias") != std::string::npos) 
		     || (trigName.find("Photon") != std::string::npos) ) 
		{
		  if (triggerResults->accept(i)) 
		    {
		      //std::cout << "INSERTING TRIGGER INFO PAIR" << std::endl;
		      //triggersOfInterest->insert(std::pair<std::string,bool>(trigName, true) );
		      std::pair<std::string,bool> dummyPair(trigName, true);
		      //triggersOfInterest_.insert(dummyPair);
		      triggersOfInterest_->insert(dummyPair);
		      h_ele_triggersFired_before_->Fill( trigName.c_str() , 1 );
		      passedTriggers.push_back(trigName);
		      if ( trigName == elecTrig_.at(j) )
			{
			  trigger_fired = true;
			}
		    }
		  else 
		    {
		      //triggersOfInterest->insert(std::pair<std::string,bool>(trigName, false) );
		      std::pair<std::string,bool> dummyPair(trigName, false);
		      //triggersOfInterest_.insert(dummyPair);
		      triggersOfInterest_->insert(dummyPair);
		      h_ele_triggersFired_before_->Fill( trigName.c_str() , 0 );
		      h_ele_triggersFired_after_->Fill( trigName.c_str() , 0 );
		    } 
		}
	    }
	}


      LogTrace("") << ">>> Trigger bit: " << trigger_fired << " for one of (" ;
      for (unsigned int k = 0; k < elecTrig_.size(); k++)
	{
	  LogTrace("") << elecTrig_.at(k) << " ";
	}
      LogTrace("") << ")";
	
      h_ele_trig_before_->Fill(trigger_fired);

//       // Loop to reject/control Z->mumu is done separately
//       unsigned int nmuonsForZ1 = 0;
//       unsigned int nmuonsForZ2 = 0;
//       for (unsigned int i=0; i<muonCollectionSize; i++) {
//             const Muon& mu = muonCollection->at(i);
//             if (!mu.isGlobalMuon()) continue;
//             double pt = mu.pt();
//             if (pt>ptThrForZ1_) nmuonsForZ1++;
//             if (pt>ptThrForZ2_) nmuonsForZ2++;
//       }
//       LogTrace("") << "> Z rejection: muons above " << ptThrForZ1_ << " [GeV]: " << nmuonsForZ1;
//       LogTrace("") << "> Z rejection: muons above " << ptThrForZ2_ << " [GeV]: " << nmuonsForZ2;
//       nz1_before_->Fill(nmuonsForZ1);
//       nz2_before_->Fill(nmuonsForZ2);

      // L1 Trigger
      m_l1GtUtils.retrieveL1EventSetup(es);
      int iErrorCode = -1;

      std::vector< std::string > l1TrigNames;
      l1TrigNames.push_back("L1_SingleIsoEG5");
      l1TrigNames.push_back("L1_SingleIsoEG8");
      l1TrigNames.push_back("L1_SingleIsoEG10");
      l1TrigNames.push_back("L1_SingleIsoEG12");
      l1TrigNames.push_back("L1_SingleIsoEG15");
      l1TrigNames.push_back("L1_SingleEG2");
      l1TrigNames.push_back("L1_SingleEG5");
      l1TrigNames.push_back("L1_SingleEG8");
      l1TrigNames.push_back("L1_SingleEG10");
      l1TrigNames.push_back("L1_SingleEG12");
      l1TrigNames.push_back("L1_SingleEG15");
      l1TrigNames.push_back("L1_SingleEG20"); 

      //std::map<std::string,bool> l1TriggersOfInterest;

      for (unsigned int n = 0; n < l1TrigNames.size(); n++)
	{ 
	  bool decision = m_l1GtUtils.decisionBeforeMask(ev, l1TrigNames.at(n), iErrorCode);

	  if (iErrorCode == 0) 
	    {
	      // do something, everything's cool
	      std::pair<std::string,bool> dummyPair(l1TrigNames.at(n), decision);
	      //l1TriggersOfInterest_.insert(dummyPair);
	      l1TriggersOfInterest_->insert(dummyPair);
	    } 
	  else if (iErrorCode == 1) 
	    {
	      // trigger does not exist in the L1 menu
	      std::cerr << "Trigger " << l1TrigNames.at(n)
			<< " does not exist in this L1 menu! Skipping" 
			<< std::endl;
	    } 
	  else 
	    {
	      // error - see error code
	      std::cerr << "An error occurred for trigger " 
			<< l1TrigNames.at(n) << ", error code: " 
			<< iErrorCode << std::endl;
	    }
	}


      // Retrieve Jet & Met collections
      if ( not getJetCollections(ev) ) return;
      if ( not getMetCollections(ev) ) return;


      // Fill jets, return jnets
      int nCaloJets = fillCaloJets(CALO, "before");
      if ( useCorrCaloJets_ ) fillCaloJets(CALO_CORR, "before");
      fillPfJets(PF, "before");
      if ( useCorrPfJets_ ) fillPfJets(PF_CORR, "before");
      if ( useGenJets_ ) fillGenJets("before");


      math::XYZTLorentzVector v_e1(0., 0., 0., 0.);




      if( ele_nelectrons_ > 0 ) {
        const GsfElectron& elec = electronCollection->at(0);
        v_e1.SetPxPyPzE(elec.px(),elec.py(),elec.pz(),elec.energy());
      }

      fillCaloMetHistograms( "before", false, v_e1 ); 
      if(useCorrCaloMet_) {  
        fillCaloMetHistograms( "before", true, v_e1 ); 
      }
  
      fillPfMetHistograms( "before", false, v_e1  ); 
      if(useCorrPfMet_) {
        fillPfMetHistograms(  "before", true, v_e1  ); 
      }
  
      fillTcMetHistograms( "before", v_e1  ); 
  
      if(useGenMet_) {
        fillGenMetHistograms( "before", v_e1  ); 
      }
      //         // MET
      double met_px = 0.;
      double met_py = 0.;
      Handle<View<MET> > metCollection;
      if (!ev.getByLabel(metCaloTag_, metCollection)) {
	LogWarning("") << ">>> MET collection does not exist !!!";
	return;
      }
      const MET& met = metCollection->at(0);
      met_px = met.px();
      met_py = met.py();      double met_et = sqrt(met_px*met_px+met_py*met_py);
      LogTrace("") << ">>> MET, MET_px, MET_py: " << met_et << ", " << met_px << ", " << met_py << " [GeV]";
      h_met_before_->Fill(met_et);



      // Start counting
      nall++;

      // Histograms per event should be done only once, so keep track of them
      bool hlt_hist_done = false;
      //bool minv_hist_done = false;
       bool met_hist_done = false;
//       bool nz1_hist_done = false;
//       bool nz2_hist_done = false;
      bool njets_hist_done = false;

      // Central selection criteria
      const int NFLAGS = 13; // number of individual selection criteria
      // 0: pt cut           | rec
      // 1: eta cut          | rec
      // 2: sieie            | eid
      // 3: hoe              | eid
      // 4: detain           | eid
      // 5: dphiin           | eid
      // 6: ecal iso         | iso
      // 7: hcal iso         | iso
      // 8: trk iso          | iso
      // 9: trigger fired    | hlt/all
      // 10: mt
      // 11: met
      // 12: njets
      bool electron_sel[NFLAGS];

      // for invariant mass calculation
      // keep track of highest-pt electrons for initial (RECO) electrons 
      // and "good" electrons (passing all cuts)
      // first array dimension is for first or second good electron
      // second array dimension is for relevant quantities of good electron
      //    [0]: 1 for electron found or 0 for not found (if 0, no other quantities filled)
      //    [1]: mSqr
      //    [2]: E
      //    [3]: px
      //    [4]: py
      //    [5]: pz
      // inv mass = sqrt(m_1^2 + m_2^2 + 2*(E_1*E_2 - (px1*px2 + py1*py2 + pz1+pz2) ) )
      double electron[2][6];
      double goodElectron[2][6];
      nGoodElectrons = 0;
      for (unsigned int i = 0; i < 2; i++)
	{
	  for (unsigned int j = 0; j < 6; j++)
	    {
	      electron[i][j] = 0.;
	      goodElectron[i][j] = 0.;
	    }
	}

<<<<<<< EwkElecStandaloneDQM.cc
      // keep track of electron quantities for calculating efficiencies
      double highestElectronEt = 0.;
      double highestEtElectronEta = -999.;
      double highestEtElectronDetain = 999.;
      double highestEtElectronHoE = 999.;
      bool highestEtElectronIsBarrel = false;
      bool highestEtElectronIsEndcap = false;
      bool highestEtElectronIsEcalDriven = false;
      bool highestEtElectronIsGood = false;
      //      double highestElectronPt = 0.;
      //       double highestPtElectronEta = -999.;
      //       double highestPtElectronDetain = 999.;
      //       double highestPtElectronHoE = 999.;
      //       bool highestPtElectronIsBarrel = false;
      //       bool highestPtElectronIsEcalDriven = false;
      //       bool highestPtElectronIsGood = false;
      //       //      double highestGoodElectronPt = 0.;
=======
      double highestElectronEt = 0.;
      double highestEtElectronEta = -999.;
      double highestEtElectronDetain = 999.;
      double highestEtElectronHoE = 999.;
      bool highestEtElectronIsBarrel = false;
      bool highestEtElectronIsEcalDriven = false;
      bool highestEtElectronIsGood = false;
      //      double highestGoodElectronPt = 0.;
>>>>>>> 1.31
      hasWCand_ = false;
      hasZCand_ = false;


      reco::GsfElectronCollection* goodElecs = new reco::GsfElectronCollection();
      //      std::cout << "Size of good electrons vector is " << goodElecs->size() << "   ";
      
      for (int i = 0; i < ele_nelectrons_; i++) 
	{

	  if (i >= k_ELEC_MAX)
	    {
	      LogWarning("") << "At electron " << i << ", max allowed is " << k_ELEC_MAX 
			     << ", skipping rest of electrons in this event";
	      break;
	    }

	  for (int j = 0; j < NFLAGS; ++j) 
	    {
	      electron_sel[j] = false;
            }
	  
	  const GsfElectron& elec = electronCollection->at(i);
	  //if (!mu.isGlobalMuon()) continue;
	  //if (mu.globalTrack().isNull()) continue;
	  //if (mu.innerTrack().isNull()) continue;

	  reco::GsfTrackRef track = elec.gsfTrack();
	  reco::SuperClusterRef supercluster = elec.superCluster();
	  //const reco::BasicClusterRef bcref = superCluster()->seed();
	  const reco::CaloClusterPtr bcref = supercluster->seed();
	  const reco::BasicCluster *bc = bcref.get();

	  float ee3x3 = lazyTool.e3x3(*bc);
	  float eeMax = lazyTool.eMax(*bc);
	  ele_rmax3x3_[i] = eeMax/ee3x3;
	  
	  LogTrace("") << "> elec: processing electron number " << i << "...";
	  //reco::TrackRef gm = mu.globalTrack();
	  //reco::TrackRef tk = mu.innerTrack();
	  // should have stuff for electron track?
	  

	  // just filling all the stuff that's not cut on

	  // stuff we need to know before we start!
	  ele_pt_[i] = elec.pt();
	  ele_px_[i] = elec.px();
	  ele_py_[i] = elec.py();
	  ele_eta_[i] = elec.eta();
	  ele_phi_[i] = elec.phi(); // for good measure

	  bool hasPtGt10 = false;
	  if (ele_et_[i] > 10.)
	    {
	      hasPtGt10 = true;
	    }

	  ele_isBarrel_[i] = false;
	  ele_isEndcap_[i] = false;
	  if (ele_eta_[i] < 1.4442 && ele_eta_[i] > -1.4442)
	    {
	      ele_isBarrel_[i] = true;
	    }
	  else if ((ele_eta_[i] > 1.56 && ele_eta_[i] < 2.4) || (ele_eta_[i] < -1.56 && ele_eta_[i] > -2.4))
	    {
	      ele_isEndcap_[i] = true;
	    }

	  if (hasPtGt10)
	    {
	      h_ele_rMax3x3_before_->Fill(ele_rmax3x3_[i]);
	      if (ele_isBarrel_[i])
		h_ele_rMax3x3Barrel_before_->Fill(ele_rmax3x3_[i]);
	      else if (ele_isEndcap_[i])
		h_ele_rMax3x3Endcap_before_->Fill(ele_rmax3x3_[i]);
	    }

	  if (ele_rmax3x3_[i] > 0.95)
	    continue;
	  
	  ele_isEcalDriven_[i] = false;
	  ele_isTrackerDriven_[i] = false;

	  ele_isEcalDriven_[i] = elec.ecalDrivenSeed();
	  ele_isTrackerDriven_[i] = elec.trackerDrivenSeed();


	  // GSF electron
	  ele_ecalenergy_[i] = elec.ecalEnergy();
	  ele_ecalenergyerror_[i] = elec.ecalEnergyError();

	  if (hasPtGt10)
	    {
	      h_ele_ecalEnergy_before_->Fill(ele_ecalenergy_[i]) ;
	      h_ele_ecalEnergyError_before_->Fill(ele_ecalenergyerror_[i]) ;        // how useful are these errors?
	    }
	  
	  ele_deltaetaeleclustertrackatcalo_[i] = elec.deltaEtaEleClusterTrackAtCalo();
	  ele_deltaetaseedclustertrackatcalo_[i] = elec.deltaEtaSeedClusterTrackAtCalo();
	  ele_deltaetasuperclustertrackatvtx_[i] = elec.deltaEtaSuperClusterTrackAtVtx();

	  if (hasPtGt10)
	    {
	      h_ele_deltaEtaEleClusterTrackAtCalo_before_->Fill(ele_deltaetaeleclustertrackatcalo_[i]) ;
	      h_ele_deltaEtaSeedClusterTrackAtCalo_before_->Fill(ele_deltaetaseedclustertrackatcalo_[i]) ;
	      h_ele_deltaEtaSuperClusterTrackAtVtx_before_->Fill(ele_deltaetasuperclustertrackatvtx_[i]) ;
	    }
	  
	  ele_deltaphieleclustertrackatcalo_[i] = elec.deltaPhiEleClusterTrackAtCalo();
	  ele_deltaphiseedclustertrackatcalo_[i] = elec.deltaPhiSeedClusterTrackAtCalo();
	  ele_deltaphisuperclustertrackatvtx_[i] = elec.deltaPhiSuperClusterTrackAtVtx();

	  if (hasPtGt10)
	    {
	      h_ele_deltaPhiEleClusterTrackAtCalo_before_->Fill(ele_deltaphieleclustertrackatcalo_[i]) ;
	      h_ele_deltaPhiSeedClusterTrackAtCalo_before_->Fill(ele_deltaphiseedclustertrackatcalo_[i]) ;
	      h_ele_deltaPhiSuperClusterTrackAtVtx_before_->Fill(ele_deltaphisuperclustertrackatvtx_[i]) ;
	    }

	  ele_trackpositionatvtxx_[i] = (float) elec.trackPositionAtVtx().X();
	  ele_trackpositionatvtxy_[i] = (float) elec.trackPositionAtVtx().Y();
	  ele_trackpositionatvtxz_[i] = (float) elec.trackPositionAtVtx().Z();

	  if (hasPtGt10)
	    {
	      h_ele_trackPositionAtVtxX_before_->Fill(ele_trackpositionatvtxx_[i]) ;
	      h_ele_trackPositionAtVtxY_before_->Fill(ele_trackpositionatvtxy_[i]) ;
	      h_ele_trackPositionAtVtxZ_before_->Fill(ele_trackpositionatvtxz_[i]) ;
	    }

	  ele_trackmomentumatvtxx_[i] = (float) elec.trackMomentumAtVtx().X();
	  ele_trackmomentumatvtxy_[i] = (float) elec.trackMomentumAtVtx().Y();
	  ele_trackmomentumatvtxz_[i] = (float) elec.trackMomentumAtVtx().Z();

	  if (hasPtGt10)
	    {
	      h_ele_trackMomentumAtVtxX_before_->Fill(ele_trackmomentumatvtxx_[i]) ;
	      h_ele_trackMomentumAtVtxY_before_->Fill(ele_trackmomentumatvtxy_[i]) ;
	      h_ele_trackMomentumAtVtxZ_before_->Fill(ele_trackmomentumatvtxz_[i]) ;
	    }

	  ele_trackpositionatcalox_[i] = (float) elec.trackPositionAtCalo().X();
	  ele_trackpositionatcaloy_[i] = (float) elec.trackPositionAtCalo().Y();
	  ele_trackpositionatcaloz_[i] = (float) elec.trackPositionAtCalo().Z();

	  if (hasPtGt10)
	    {
	      h_ele_trackPositionAtCaloX_before_->Fill(ele_trackpositionatcalox_[i]) ;
	      h_ele_trackPositionAtCaloY_before_->Fill(ele_trackpositionatcaloy_[i]) ;
	      h_ele_trackPositionAtCaloZ_before_->Fill(ele_trackpositionatcaloz_[i]) ;
	    }

	  ele_trackmomentumatcalox_[i] = (float) elec.trackMomentumAtCalo().X();
	  ele_trackmomentumatcaloy_[i] = (float) elec.trackMomentumAtCalo().Y();
	  ele_trackmomentumatcaloz_[i] = (float) elec.trackMomentumAtCalo().Z();

	  if (hasPtGt10)
	    {
	      h_ele_trackMomentumAtCaloX_before_->Fill(ele_trackmomentumatcalox_[i]) ;
	      h_ele_trackMomentumAtCaloY_before_->Fill(ele_trackmomentumatcaloy_[i]) ;
	      h_ele_trackMomentumAtCaloZ_before_->Fill(ele_trackmomentumatcaloz_[i]) ;
	    }

	  ele_eeleclusteroverpout_[i] = elec.eEleClusterOverPout();
	  ele_eseedclusteroverp_[i] = elec.eSeedClusterOverP();
	  ele_eseedclusteroverpout_[i] = elec.eSeedClusterOverPout();
	  ele_esuperclusteroverp_[i] = elec.eSuperClusterOverP(); 
	  
	  if (hasPtGt10)
	    {
	      h_ele_eEleClusterOverPout_before_->Fill(ele_eeleclusteroverpout_[i]) ;
	      h_ele_eSeedClusterOverP_before_->Fill(ele_eseedclusteroverp_[i]) ;
	      h_ele_eSeedClusterOverPout_before_->Fill(ele_eseedclusteroverpout_[i]) ;
	      h_ele_eSuperClusterOverP_before_->Fill(ele_esuperclusteroverp_[i]) ; 
	    }
	  
	  ele_electronmomentumerror_[i] = elec.electronMomentumError();

	  if (hasPtGt10)
	    {
	      h_ele_electronMomentumError_before_->Fill(ele_electronmomentumerror_[i]) ;
	    }

	  // isolation dR 0.3	  
	  ele_dr03ecalrechitsumet_[i] = elec.dr03EcalRecHitSumEt () ;
	  ele_dr03hcaldepth1towersumet_[i] = elec.dr03HcalDepth1TowerSumEt() ;
	  ele_dr03hcaldepth2towersumet_[i] = elec.dr03HcalDepth2TowerSumEt() ;
	  ele_dr03hcaltowersumet_[i] = elec.dr03HcalTowerSumEt() ;
	  ele_dr03tksumpt_[i] = elec.dr03TkSumPt() ;
	  
	  if (hasPtGt10)
	    {
	      h_ele_dr03EcalRecHitSumEt_before_->Fill(ele_dr03ecalrechitsumet_[i]) ;
	      h_ele_dr03HcalDepth1TowerSumEt_before_->Fill(ele_dr03hcaldepth1towersumet_[i]) ;
	      h_ele_dr03HcalDepth2TowerSumEt_before_->Fill(ele_dr03hcaldepth2towersumet_[i]) ;
	      h_ele_dr03HcalTowerSumEt_before_->Fill(ele_dr03hcaltowersumet_[i]) ;
	      h_ele_dr03TkSumPt_before_->Fill(ele_dr03tksumpt_[i]) ;
	    
	  
	      if (ele_isBarrel_[i])
		{
		  h_ele_dr03EcalRecHitSumEtBarrel_before_->Fill(ele_dr03ecalrechitsumet_[i]) ;
		  h_ele_dr03HcalDepth1TowerSumEtBarrel_before_->Fill(ele_dr03hcaldepth1towersumet_[i]) ;
		  h_ele_dr03HcalDepth2TowerSumEtBarrel_before_->Fill(ele_dr03hcaldepth2towersumet_[i]) ;
		  h_ele_dr03HcalTowerSumEtBarrel_before_->Fill(ele_dr03hcaltowersumet_[i]) ;
		  h_ele_dr03TkSumPtBarrel_before_->Fill(ele_dr03tksumpt_[i]) ;
		}
	      if (ele_isEndcap_[i])
		{
		  h_ele_dr03EcalRecHitSumEtEndcap_before_->Fill(ele_dr03ecalrechitsumet_[i]) ;
		  h_ele_dr03HcalDepth1TowerSumEtEndcap_before_->Fill(ele_dr03hcaldepth1towersumet_[i]) ;
		  h_ele_dr03HcalDepth2TowerSumEtEndcap_before_->Fill(ele_dr03hcaldepth2towersumet_[i]) ;
		  h_ele_dr03HcalTowerSumEtEndcap_before_->Fill(ele_dr03hcaltowersumet_[i]) ;
		  h_ele_dr03TkSumPtEndcap_before_->Fill(ele_dr03tksumpt_[i]) ;
		}
	    }
	  
	  // isolation dR 0.4
	  ele_dr04ecalrechitsumet_[i] = elec.dr04EcalRecHitSumEt() ;
	  ele_dr04hcaldepth1towersumet_[i] = elec.dr04HcalDepth1TowerSumEt() ;
	  ele_dr04hcaldepth2towersumet_[i] = elec.dr04HcalDepth2TowerSumEt() ;
	  ele_dr04hcaltowersumet_[i] = elec.dr04HcalTowerSumEt() ;
	  ele_dr04tksumpt_[i] = elec.dr04TkSumPt() ; 
	  
	  if (hasPtGt10)
	    {
	      h_ele_dr04EcalRecHitSumEt_before_->Fill(ele_dr04ecalrechitsumet_[i]) ;
	      h_ele_dr04HcalDepth1TowerSumEt_before_->Fill(ele_dr04hcaldepth1towersumet_[i]) ;
	      h_ele_dr04HcalDepth2TowerSumEt_before_->Fill(ele_dr04hcaldepth2towersumet_[i]) ;
	      h_ele_dr04HcalTowerSumEt_before_->Fill(ele_dr04hcaltowersumet_[i]) ;
	      h_ele_dr04TkSumPt_before_->Fill(ele_dr04tksumpt_[i]) ; 
	      
	      if (ele_isBarrel_[i])
		{
		  h_ele_dr04EcalRecHitSumEtBarrel_before_->Fill(ele_dr04ecalrechitsumet_[i]) ;
		  h_ele_dr04HcalDepth1TowerSumEtBarrel_before_->Fill(ele_dr04hcaldepth1towersumet_[i]) ;
		  h_ele_dr04HcalDepth2TowerSumEtBarrel_before_->Fill(ele_dr04hcaldepth2towersumet_[i]) ;
		  h_ele_dr04HcalTowerSumEtBarrel_before_->Fill(ele_dr04hcaltowersumet_[i]) ;
		  h_ele_dr04TkSumPtBarrel_before_->Fill(ele_dr04tksumpt_[i]) ; 
		}
	      if (ele_isEndcap_[i])
		{
		  h_ele_dr04EcalRecHitSumEtEndcap_before_->Fill(ele_dr04ecalrechitsumet_[i]) ;
		  h_ele_dr04HcalDepth1TowerSumEtEndcap_before_->Fill(ele_dr04hcaldepth1towersumet_[i]) ;
		  h_ele_dr04HcalDepth2TowerSumEtEndcap_before_->Fill(ele_dr04hcaldepth2towersumet_[i]) ;
		  h_ele_dr04HcalTowerSumEtEndcap_before_->Fill(ele_dr04hcaltowersumet_[i]) ;
		  h_ele_dr04TkSumPtEndcap_before_->Fill(ele_dr04tksumpt_[i]) ; 
		}
	    }

	  ele_e1x5_[i] = elec.e1x5() ;
	  ele_e2x5max_[i] = elec.e2x5Max() ;
	  ele_e5x5_[i] = elec.e5x5() ;
	  ele_r1x5_[i] = ele_e1x5_[i] / ele_e5x5_[i] ;
	  ele_r2x5max_[i] = ele_e2x5max_[i] / ele_e5x5_[i] ;
	  ele_scpreshowerenergy_[i] = supercluster->preshowerEnergy();
	  ele_scetawidth_[i] = supercluster->etaWidth();
	  ele_scphiwidth_[i] = supercluster->phiWidth();
	  ele_energy_[i] = elec.energy() ;
// 	  float ee3x3 = lazyTool.e3x3(*bc);
// 	  float eeMax = lazyTool.eMax(*bc);
	  ele_scr9_[i] = ee3x3 / ele_energy_[i]; // energy in 3x3 around seed cluster / SC energy
	  ele_sceseedoveresupercluster_[i] = eeMax / ele_energy_[i]; // energy of max crystal / SC energy
	  
	  if (hasPtGt10)
	    {
	      h_ele_e1x5_before_->Fill(ele_e1x5_[i]) ;
	      h_ele_e2x5Max_before_->Fill(ele_e2x5max_[i]) ;
	      h_ele_e5x5_before_->Fill(ele_e5x5_[i]) ;
	      h_ele_r1x5_before_->Fill(ele_r1x5_[i]) ;
	      h_ele_r2x5Max_before_->Fill(ele_r2x5max_[i]) ;
	      h_ele_scEtaWidth_before_->Fill(ele_scetawidth_[i]) ;
	      h_ele_scPhiWidth_before_->Fill(ele_scphiwidth_[i]) ;
	      h_ele_scR9_before_->Fill(ele_scr9_[i]) ;
	      h_ele_scESeedOverESuperCluster_before_->Fill(ele_sceseedoveresupercluster_[i]);
	      
	      if (ele_isBarrel_[i])
		{
		  h_ele_e1x5Barrel_before_->Fill(ele_e1x5_[i]) ;
		  h_ele_e2x5MaxBarrel_before_->Fill(ele_e2x5max_[i]) ;
		  h_ele_e5x5Barrel_before_->Fill(ele_e5x5_[i]) ;
		  h_ele_r1x5Barrel_before_->Fill(ele_r1x5_[i]) ;
		  h_ele_r2x5MaxBarrel_before_->Fill(ele_r2x5max_[i]) ;
		  h_ele_scEtaWidthBarrel_before_->Fill(ele_scetawidth_[i]) ;
		  h_ele_scPhiWidthBarrel_before_->Fill(ele_scphiwidth_[i]) ;
		  h_ele_scR9Barrel_before_->Fill(ele_scr9_[i]) ;
		  h_ele_scESeedOverESuperClusterBarrel_before_->Fill(ele_sceseedoveresupercluster_[i]);
		}	  
	      if (ele_isEndcap_[i])
		{
		  h_ele_e1x5Endcap_before_->Fill(ele_e1x5_[i]) ;
		  h_ele_e2x5MaxEndcap_before_->Fill(ele_e2x5max_[i]) ;
		  h_ele_e5x5Endcap_before_->Fill(ele_e5x5_[i]) ;
		  h_ele_r1x5Endcap_before_->Fill(ele_r1x5_[i]) ;
		  h_ele_r2x5MaxEndcap_before_->Fill(ele_r2x5max_[i]) ;
		  h_ele_scPreshowerEnergyEndcap_before_->Fill(ele_scpreshowerenergy_[i]) ;
		  h_ele_scEtaWidthEndcap_before_->Fill(ele_scetawidth_[i]) ;
		  h_ele_scPhiWidthEndcap_before_->Fill(ele_scphiwidth_[i]) ;
		  h_ele_scR9Endcap_before_->Fill(ele_scr9_[i]) ;
		  h_ele_scESeedOverESuperClusterEndcap_before_->Fill(ele_sceseedoveresupercluster_[i]);
		}	  
	    }

	  ele_hcaldepth1overecal_[i] = elec.hcalDepth1OverEcal() ;
	  ele_hcaldepth2overecal_[i] = elec.hcalDepth2OverEcal() ;
	  ele_hcaloverecal_[i] = elec.hcalOverEcal() ; 
	  
	  if (hasPtGt10)
	    {
	      h_ele_hcalDepth1OverEcal_before_->Fill(ele_hcaldepth1overecal_[i]) ;
	      h_ele_hcalDepth2OverEcal_before_->Fill(ele_hcaldepth2overecal_[i]) ;
	      h_ele_hcalOverEcal_before_->Fill(ele_hcaloverecal_[i]) ; 

	      if (ele_isBarrel_[i])
		{
		  h_ele_hcalDepth1OverEcalBarrel_before_->Fill(ele_hcaldepth1overecal_[i]) ;
		  h_ele_hcalDepth2OverEcalBarrel_before_->Fill(ele_hcaldepth2overecal_[i]) ;
		  h_ele_hcalOverEcalBarrel_before_->Fill(ele_hcaloverecal_[i]) ; 
		}	  
	      if (ele_isEndcap_[i])
		{
		  h_ele_hcalDepth1OverEcalEndcap_before_->Fill(ele_hcaldepth1overecal_[i]) ;
		  h_ele_hcalDepth2OverEcalEndcap_before_->Fill(ele_hcaldepth2overecal_[i]) ;
		  h_ele_hcalOverEcalEndcap_before_->Fill(ele_hcaloverecal_[i]) ; 
		}	  
	    }

	  // sigma eta eta, sigma ieta ieta
	  ele_sigmaetaeta_[i] = elec.sigmaEtaEta() ;
	  ele_sigmaietaieta_[i] = elec.sigmaIetaIeta() ; 
	  
	  if (hasPtGt10)
	    {
	      h_ele_sigmaEtaEta_before_->Fill(ele_sigmaetaeta_[i]) ;
	      h_ele_sigmaIetaIeta_before_->Fill(ele_sigmaietaieta_[i]) ; 

	      if (ele_isBarrel_[i])
		{
		  h_ele_sigmaEtaEtaBarrel_before_->Fill(ele_sigmaetaeta_[i]) ;
		  h_ele_sigmaIetaIetaBarrel_before_->Fill(ele_sigmaietaieta_[i]) ; 
		}
	      if (ele_isEndcap_[i])
		{
		  h_ele_sigmaEtaEtaEndcap_before_->Fill(ele_sigmaetaeta_[i]) ;
		  h_ele_sigmaIetaIetaEndcap_before_->Fill(ele_sigmaietaieta_[i]) ; 
		}
	    }	  

	  // misc + charge
	  ele_basicclusterssize_[i] = elec.basicClustersSize() ; 
	  ele_numberofbrems_[i] = elec.numberOfBrems() ; 
	  ele_fbrem_[i] = elec.fbrem() ; 
	  ele_scpixcharge_[i] = elec.scPixCharge() ;
	  ele_ctfcharge_[i] = 0.;
	  if (elec.closestCtfTrackRef().isNonnull()) 
	    {
	      ele_ctfcharge_[i] = elec.closestCtfTrackRef()->charge();

	    }
	  ele_charge_[i] = elec.charge() ;
	  ele_isgsfscpixchargeconsistent_[i] = elec.isGsfScPixChargeConsistent() ;
	  ele_isgsfctfchargeconsistent_[i] = elec.isGsfCtfChargeConsistent() ;
	  ele_isgsfctfscpixchargeconsistent_[i] = elec.isGsfCtfScPixChargeConsistent() ;

	  if (hasPtGt10)
	    {
	      h_ele_basicClustersSize_before_->Fill(ele_basicclusterssize_[i]) ; 
	      h_ele_numberOfBrems_before_->Fill(ele_numberofbrems_[i]) ; 
	      h_ele_fbrem_before_->Fill(ele_fbrem_[i]) ; 
	      h_ele_scPixCharge_before_->Fill(ele_scpixcharge_[i]) ;
	      h_ele_ctfCharge_before_->Fill(ele_ctfcharge_[i]) ;
	      h_ele_charge_before_->Fill(ele_charge_[i]);
	      h_ele_gsfScPixChargeConsistent_before_->Fill(ele_isgsfscpixchargeconsistent_[i]);
	      h_ele_gsfCtfChargeConsistent_before_->Fill(ele_isgsfctfchargeconsistent_[i]);
	      h_ele_gsfCtfScPixChargeConsistent_before_->Fill(ele_isgsfctfscpixchargeconsistent_[i]);
	    }


	  // candidate
	  //	  float c_energy = elec.energy() ; // done previously
	  ele_et_[i] = elec.et() ;
	  ele_p_[i] = elec.p() ;
	  ele_px_[i] = elec.px() ;
	  ele_py_[i] = elec.py() ;
	  ele_pz_[i] = elec.pz() ;

	  if (hasPtGt10)
	    {
	      h_ele_energy_before_->Fill(ele_energy_[i]) ;
	      h_ele_et_before_->Fill(ele_et_[i]) ;
	      h_ele_p_before_->Fill(ele_p_[i]) ;
	      h_ele_px_before_->Fill(ele_px_[i]) ;
	      h_ele_py_before_->Fill(ele_py_[i]) ;
	      h_ele_pz_before_->Fill(ele_pz_[i]) ;
	    }

	  // vertex
	  ele_vertexChi2_[i] = elec.vertexChi2() ;
	  ele_vertexNdof_[i] = elec.vertexNdof() ;
	  ele_vertexNormalizedChi2_[i] = elec.vertexNormalizedChi2() ;
	  ele_vx_[i] = elec.vx() ;
	  ele_vy_[i] = elec.vy() ;
	  ele_vz_[i] = elec.vz() ;

	  if (hasPtGt10)
	    {
	      h_ele_vertexChi2_before_->Fill(ele_vertexChi2_[i]) ;
	      h_ele_vertexNdof_before_->Fill(ele_vertexNdof_[i]) ;
	      h_ele_vertexNormalizedChi2_before_->Fill(ele_vertexNormalizedChi2_[i]) ;
	      h_ele_vx_before_->Fill(ele_vx_[i]) ;
	      h_ele_vy_before_->Fill(ele_vy_[i]) ;
	      h_ele_vz_before_->Fill(ele_vz_[i]) ;
	    }


	  // doing the cuts now!
	  if (i < 2)
	    {
	      electron[i][0] = 1.;
	      electron[i][1] = elec.massSqr();
	      electron[i][2] = elec.energy();
	      electron[i][3] = elec.px();
	      electron[i][4] = elec.py();
	      electron[i][5] = elec.pz();
	    }

	  // Pt,eta cuts
	  LogTrace("") << "\t... pt, eta: " << ele_pt_[i] << " [GeV], " << ele_eta_[i];;
	  if (ele_pt_[i] > ptCut_) electron_sel[0] = true; 
	  if (fabs(ele_eta_[i]) < etaCut_) electron_sel[1] = true; 

	  //             // d0, chi2, nhits quality cuts
// 	  const Point beamSpotPosition = beamSpotHandle->position();
// 	  double dxy = track->dxy(beamSpotPosition);
	  ele_dxy_[i] = track->dxy(beamSpotHandle->position());
	  ele_gsfcharge_[i] = track->charge();

	  //             double normalizedChi2 = gm->normalizedChi2();
	  //             double trackerHits = tk->numberOfValidHits();
	  //             LogTrace("") << "\t... dxy, normalizedChi2, trackerHits, isTrackerMuon?: " << dxy << " [cm], " << normalizedChi2 << ", " << trackerHits << ", " << mu.isTrackerMuon();
	  //             if (fabs(dxy)<dxyCut_) muon_sel[2] = true; 
	  //             if (normalizedChi2<normalizedChi2Cut_) muon_sel[3] = true; 
	  //             if (trackerHits>=trackerHitsCut_) muon_sel[4] = true; 
	  //             if (mu.isTrackerMuon()) muon_sel[5] = true; 
	  
	  if (hasPtGt10)
	    {
	      h_ele_pt_before_->Fill(ele_pt_[i]);
	      if(ele_isEcalDriven_[i])
		{
		  h_ele_ptEcalDriven_before_->Fill(ele_pt_[i]);
		}
	      if(ele_isTrackerDriven_[i])
		{
		  h_ele_ptTrackerDriven_before_->Fill(ele_pt_[i]);
		}
	      h_ele_eta_before_->Fill(ele_eta_[i]);
	      h_ele_phi_before_->Fill(ele_phi_[i]);
	      h_ele_dxy_before_->Fill(ele_dxy_[i]);
	      h_ele_gsfCharge_before_->Fill(ele_gsfcharge_[i]);
	      //             chi2_before_->Fill(normalizedChi2);
	      //             nhits_before_->Fill(trackerHits);
	      //             tkmu_before_->Fill(mu.isTrackerMuon());
	    }	  

	  
	  // Electron ID cuts
	  //	  ele_sigmaietaieta_[i] = elec.sigmaIetaIeta(); // done already, with shower shape variables
	  //ele_hoe_[i] = elec.hcalOverEcal();
	  ele_deltaetasuperclustertrackatvtx_[i] = elec.deltaEtaSuperClusterTrackAtVtx(); // detain
	  ele_deltaphisuperclustertrackatvtx_[i] = elec.deltaPhiSuperClusterTrackAtVtx(); // dphiin
	  if (ele_isBarrel_[i] && ele_sigmaietaieta_[i] < sieieCutBarrel_) electron_sel[2] = true; 
	  if (ele_isEndcap_[i] && ele_sigmaietaieta_[i] < sieieCutEndcap_) electron_sel[2] = true; 
	  if (ele_isBarrel_[i] && ele_hcaloverecal_[i] < hoECutBarrel_) electron_sel[3] = true; 
	  if (ele_isEndcap_[i] && ele_hcaloverecal_[i] < hoECutEndcap_) electron_sel[3] = true; 
	  if (ele_isBarrel_[i] && ele_deltaetasuperclustertrackatvtx_[i] < detainCutBarrel_ && ele_deltaetasuperclustertrackatvtx_[i] > detainCutBarrel_*-1.) electron_sel[4] = true; 
	  if (ele_isEndcap_[i] && ele_deltaetasuperclustertrackatvtx_[i] < detainCutEndcap_ && ele_deltaetasuperclustertrackatvtx_[i] > detainCutEndcap_*-1.) electron_sel[4] = true; 
	  if (ele_isBarrel_[i] && ele_deltaphisuperclustertrackatvtx_[i] < dphiinCutBarrel_ && ele_deltaphisuperclustertrackatvtx_[i] > dphiinCutBarrel_*-1.) electron_sel[5] = true; 
	  if (ele_isEndcap_[i] && ele_deltaphisuperclustertrackatvtx_[i] < dphiinCutEndcap_ && ele_deltaphisuperclustertrackatvtx_[i] > dphiinCutEndcap_*-1.) electron_sel[5] = true; 
	  if (ele_isBarrel_[i])
	    {
	      LogTrace("") << "\t... sieie value " << ele_sigmaietaieta_[i] << " (barrel), pass? " << electron_sel[2]; 
	      LogTrace("") << "\t... hcaloverecal value " << ele_hcaloverecal_[i] << " (barrel), pass? " << electron_sel[3]; 
	      LogTrace("") << "\t... detain value " << ele_deltaetasuperclustertrackatvtx_[i] << " (barrel), pass? " << electron_sel[4]; 
	      LogTrace("") << "\t... dphiin value " << ele_deltaphisuperclustertrackatvtx_[i] << " (barrel), pass? " << electron_sel[5]; 
	    }
	  else if (ele_isEndcap_[i])
	    {
	      LogTrace("") << "\t... sieie value " << ele_sigmaietaieta_[i] << " (endcap), pass? " << electron_sel[2]; 
	      LogTrace("") << "\t... hcaloverecal value " << ele_hcaloverecal_[i] << " (endcap), pass? " << electron_sel[3]; 
	      LogTrace("") << "\t... detain value " << ele_deltaetasuperclustertrackatvtx_[i] << " (endcap), pass? " << electron_sel[4]; 
	      LogTrace("") << "\t... dphiin value " << ele_deltaphisuperclustertrackatvtx_[i] << " (endcap), pass? " << electron_sel[5]; 
	    }
	  
	  if (hasPtGt10)
	    {
	      if (ele_isBarrel_[i]) 
		{
		  h_ele_sieiebarrel_before_->Fill(ele_sigmaietaieta_[i]);
		  h_ele_hoebarrel_before_->Fill(ele_hcaloverecal_[i]);
		  h_ele_detainbarrel_before_->Fill(ele_deltaetasuperclustertrackatvtx_[i]);
		  h_ele_dphiinbarrel_before_->Fill(ele_deltaphisuperclustertrackatvtx_[i]);
		}
	      else if (ele_isEndcap_[i]) 
		{
		  h_ele_sieieendcap_before_->Fill(ele_sigmaietaieta_[i]);
		  h_ele_hoeendcap_before_->Fill(ele_hcaloverecal_[i]);
		  h_ele_detainendcap_before_->Fill(ele_deltaetasuperclustertrackatvtx_[i]);
		  h_ele_dphiinendcap_before_->Fill(ele_deltaphisuperclustertrackatvtx_[i]);
		}
	    }
	  
	  
	  
	  // Isolation cuts
// 	  double ele_ecalisovar_[i] = elec.dr03EcalRecHitSumEt();  // picked one set! 
// 	  double ele_hcalisovar_[i] = elec.dr03HcalTowerSumEt();   // try others if 
// 	  double ele_trkisovar_[i] = elec.dr04TkSumPt();           // doesn't work 
	  //if (isCombinedIso_) {
	  //isovar += mu.isolationR03().emEt;
	  //isovar += mu.isolationR03().hadEt;
	  //}
	  //if (isRelativeIso_) isovar /= pt;
	  if (ele_isBarrel_[i] && ele_dr03ecalrechitsumet_[i] <ecalIsoCutBarrel_) electron_sel[6] = true; 
	  if (ele_isEndcap_[i] && ele_dr03ecalrechitsumet_[i] <ecalIsoCutEndcap_) electron_sel[6] = true; 
	  if (ele_isBarrel_[i] && ele_dr03hcaltowersumet_[i] <hcalIsoCutBarrel_) electron_sel[7] = true; 
	  if (ele_isEndcap_[i] && ele_dr03hcaltowersumet_[i] <hcalIsoCutEndcap_) electron_sel[7] = true; 
	  if (ele_isBarrel_[i] && ele_dr04tksumpt_[i] <trkIsoCutBarrel_) electron_sel[8] = true;   
	  if (ele_isEndcap_[i] && ele_dr04tksumpt_[i] <trkIsoCutEndcap_) electron_sel[8] = true;   
	  if (ele_isBarrel_[i])
	    {
	      LogTrace("") << "\t... ecal isolation value " << ele_dr03ecalrechitsumet_[i] << " (barrel), pass? " << electron_sel[6]; 
	      LogTrace("") << "\t... hcal isolation value " << ele_dr03hcaltowersumet_[i] << " (barrel), pass? " << electron_sel[7];
	      LogTrace("") << "\t... trk isolation value " << ele_dr04tksumpt_[i] << " (barrel), pass? " << electron_sel[8];
	    }
	  else if (ele_isEndcap_[i])
	    {
	      LogTrace("") << "\t... ecal isolation value " << ele_dr03ecalrechitsumet_[i] << " (endcap), pass? " << electron_sel[6]; 
	      LogTrace("") << "\t... hcal isolation value " << ele_dr03hcaltowersumet_[i] << " (endcap), pass? " << electron_sel[7];
	      LogTrace("") << "\t... trk isolation value " << ele_dr04tksumpt_[i] << " (endcap), pass? " << electron_sel[8];
	    }
	  
	  //iso_before_->Fill(isovar);
	  if (hasPtGt10)
	    {
	      if (ele_isBarrel_[i]) 
		{
		  h_ele_ecalisobarrel_before_->Fill(ele_dr03ecalrechitsumet_[i]);
		  h_ele_hcalisobarrel_before_->Fill(ele_dr03hcaltowersumet_[i]);
		  h_ele_trkisobarrel_before_->Fill(ele_dr04tksumpt_[i]);
		}
	      else if (ele_isEndcap_[i]) 
		{
		  h_ele_ecalisoendcap_before_->Fill(ele_dr03ecalrechitsumet_[i]);
		  h_ele_hcalisoendcap_before_->Fill(ele_dr03hcaltowersumet_[i]);
		  h_ele_trkisoendcap_before_->Fill(ele_dr04tksumpt_[i]);
		}
	    }
	  
	  
	  // HLT 
	  if (trigger_fired) electron_sel[9] = true; 
	  
	  
	  //             // MET/MT cuts
	  double w_et = met_et+ele_pt_[i];
	  double w_px = met_px+ele_px_[i];
	  double w_py = met_py+ele_py_[i];
	  
	  double massT = w_et*w_et - w_px*w_px - w_py*w_py;
	  massT = (massT>0) ? sqrt(massT) : 0;
	  
	  LogTrace("") << "\t... W mass, W_et, W_px, W_py: " << massT << ", " << w_et << ", " << w_px << ", " << w_py << " [GeV]";
	  if (massT>mtMin_ && massT<mtMax_) electron_sel[10] = true; 
	  if (hasPtGt10)
	    {
	      h_mt_before_->Fill(massT);
	    }
	  if (met_et>metMin_ && met_et<metMax_) electron_sel[11] = true; 
	  
	  if (nCaloJets<=nJetMax_) electron_sel[12] = true; 
	  
	  // Collect necessary flags "per electron"
	  int flags_passed = 0;
	  bool rec_sel_this = true;
	  bool eid_sel_this = true;
	  bool iso_sel_this = true;
	  bool met_sel_this = true;
	  bool all_sel_this = true;
	  for (int j=0; j<NFLAGS; ++j) 
	    {
	      if (electron_sel[j]) flags_passed += 1;
	      if (j<2  && !electron_sel[j]) rec_sel_this = false;
	      if (j<6  && !electron_sel[j]) eid_sel_this = false;
	      if (j<9  && !electron_sel[j]) iso_sel_this = false;
	      // 9 is trigger
	      if (j<13 && !electron_sel[j]) met_sel_this = false;
	      if (!electron_sel[j]) all_sel_this = false;
	    }
	  
	  if (all_sel_this)
	    {
	      if (nGoodElectrons < 2)
		{
		  goodElectron[nGoodElectrons][0] = 1.;
		  goodElectron[nGoodElectrons][1] = elec.massSqr();
		  goodElectron[nGoodElectrons][2] = elec.energy();
		  goodElectron[nGoodElectrons][3] = elec.px();
		  goodElectron[nGoodElectrons][4] = elec.py();
		  goodElectron[nGoodElectrons][5] = elec.pz();
		}
	      nGoodElectrons++;
	    }

	  //             // "rec" => pt,eta and quality cuts are satisfied
	  //             if (rec_sel_this) rec_sel = true;
	  //             // "iso" => "rec" AND "muon is isolated"
	  //             if (iso_sel_this) iso_sel = true;
	  //             // "hlt" => "iso" AND "event is triggered"
	  //             if (hlt_sel_this) hlt_sel = true;
	  // 	         // "all" => "met" AND "Z/top rejection cuts"
	  //             if (all_sel_this) all_sel = true;
	  
	  // "rec" => pt,eta cuts are satisfied
	  if (rec_sel_this) rec_sel = true;
	  // "eid" => "rec" AND "electron passes ID"
	  if (eid_sel_this) iso_sel = true;
	  // "iso" => "eid" AND "electron is isolated"
	  if (iso_sel_this) iso_sel = true;
	  // "met" => "iso" AND "MET/MT"
	  if (met_sel_this) met_sel = true;
	  // "all" => "met" AND "event is triggered"
	  if (all_sel_this) all_sel = true;

	  // Quantities for highest-pt electron
<<<<<<< EwkElecStandaloneDQM.cc
	  //	  if (ele_pt_[i] > highestElectronPt)
	  if (ele_et_[i] > highestElectronEt)
=======
	  if (ele_et_[i] > highestElectronEt)
>>>>>>> 1.31
	    {
<<<<<<< EwkElecStandaloneDQM.cc
	      //	      highestElectronPt = ele_pt_[i];
	      // 	      highestPtElectronEta = ele_eta_[i];
	      // 	      highestPtElectronDetain = ele_deltaetasuperclustertrackatvtx_[i];
	      // 	      highestPtElectronHoE = ele_hcaloverecal_[i];
	      // 	      highestPtElectronIsBarrel = ele_isBarrel_[i];
	      // 	      highestPtElectronIsEcalDriven = ele_isEcalDriven_[i];

	      highestElectronEt = ele_et_[i];
	      highestEtElectronEta = ele_eta_[i];
	      highestEtElectronDetain = ele_deltaetasuperclustertrackatvtx_[i];
	      highestEtElectronHoE = ele_hcaloverecal_[i];
	      highestEtElectronIsBarrel = ele_isBarrel_[i];
	      highestEtElectronIsEndcap = ele_isEndcap_[i];
	      highestEtElectronIsEcalDriven = ele_isEcalDriven_[i];
=======
	      highestElectronEt = ele_et_[i];
	      highestEtElectronEta = ele_eta_[i];
	      highestEtElectronDetain = ele_deltaetasuperclustertrackatvtx_[i];
	      highestEtElectronHoE = ele_hcaloverecal_[i];
	      highestEtElectronIsBarrel = ele_isBarrel_[i];
	      highestEtElectronIsEcalDriven = ele_isEcalDriven_[i];
>>>>>>> 1.31
	    }



	  // Do N-1 histograms now (and only once for global event quantities)
	  if (flags_passed >= (NFLAGS-1)) 
	    {
	      if (!electron_sel[0] || flags_passed==NFLAGS) 
		{
		  h_ele_pt_after_->Fill(ele_pt_[i]);
		  if (ele_isEcalDriven_[i])
		    {
		      h_ele_ptEcalDriven_after_->Fill(ele_pt_[i]);
		    }
		  if (ele_isTrackerDriven_[i])
		    {
		      h_ele_ptTrackerDriven_after_->Fill(ele_pt_[i]);
		    }
		}
	      if (!electron_sel[1] || flags_passed==NFLAGS) 
		{
		  h_ele_eta_after_->Fill(ele_eta_[i]);
		}
	      if (!electron_sel[2] || flags_passed==NFLAGS) 
		{
		  if (ele_isBarrel_[i])
		    {
		      h_ele_sieiebarrel_after_->Fill(ele_sigmaietaieta_[i]);
		    }
		  else if (ele_isEndcap_[i])
		    {
		      h_ele_sieieendcap_after_->Fill(ele_sigmaietaieta_[i]);
		    }
		}
	      if (!electron_sel[3] || flags_passed==NFLAGS) 
		{
		  if (ele_isBarrel_[i])
		    {
		      h_ele_hoebarrel_after_->Fill(ele_hcaloverecal_[i]);
		    }
		  else if (ele_isEndcap_[i])
		    {
		      h_ele_hoeendcap_after_->Fill(ele_hcaloverecal_[i]);
		    }
		}
	      if (!electron_sel[4] || flags_passed==NFLAGS) 
		{
		  if (ele_isBarrel_[i])
		    {
		      h_ele_detainbarrel_after_->Fill(ele_deltaetasuperclustertrackatvtx_[i]);
		    }
		  else if (ele_isEndcap_[i])
		    {
		      h_ele_detainendcap_after_->Fill(ele_deltaetasuperclustertrackatvtx_[i]);
		    }
		}
	      if (!electron_sel[5] || flags_passed==NFLAGS) 
		{
		  if (ele_isBarrel_[i])
		    {
		      h_ele_dphiinbarrel_after_->Fill(ele_deltaphisuperclustertrackatvtx_[i]);
		    }
		  else if (ele_isEndcap_[i])
		    {
		      h_ele_dphiinendcap_after_->Fill(ele_deltaphisuperclustertrackatvtx_[i]);
		    }
		}
	      if (!electron_sel[6] || flags_passed==NFLAGS) 
		{
		  if (ele_isBarrel_[i])
		    {
		      h_ele_ecalisobarrel_after_->Fill(ele_dr03ecalrechitsumet_[i]);
		    }
		  else if (ele_isEndcap_[i])
		    {
		      h_ele_ecalisoendcap_after_->Fill(ele_dr03ecalrechitsumet_[i]);
		    }
		}
	      if (!electron_sel[7] || flags_passed==NFLAGS) 
		{
		  if (ele_isBarrel_[i])
		    {
		      h_ele_hcalisobarrel_after_->Fill(ele_dr03hcaltowersumet_[i]);
		    }
		  else if (ele_isEndcap_[i])
		    {
		      h_ele_hcalisoendcap_after_->Fill(ele_dr03hcaltowersumet_[i]);
		    }
		}
	      if (!electron_sel[8] || flags_passed==NFLAGS) 
		{
		  if (ele_isBarrel_[i])
		    {
		      h_ele_trkisobarrel_after_->Fill(ele_dr04tksumpt_[i]);
		    }
		  else if (ele_isEndcap_[i])
		    {
		      h_ele_trkisoendcap_after_->Fill(ele_dr04tksumpt_[i]);
		    }
		}
	      if (!electron_sel[9] || flags_passed==NFLAGS) 
		{
		  if (!hlt_hist_done) 
		    {
		      h_ele_trig_after_->Fill(trigger_fired);
		    }
		}
	      hlt_hist_done = true;
	      if (!electron_sel[10] || flags_passed==NFLAGS) 
		{
		  h_mt_after_->Fill(massT);
		  if (massT > 40.)
		    {
		      std::cout << "WCAND AFTER CUTS in run " << runNumber_ << " LS " << lumiSection_ 
				<< " event " << eventNumber_ << " with transverse mass " << massT << std::endl;
		      hasWCand_ = true;
		    }
		}
	      if (!electron_sel[11] || flags_passed==NFLAGS) 
		{
		  if (!met_hist_done) {
		    h_met_after_->Fill(met_et);



		    fillCaloMetHistograms( "after", false, v_e1 ); 
		    if(useCorrCaloMet_) {  
		      fillCaloMetHistograms( "after", true, v_e1 ); 
		    }
		    fillPfMetHistograms( "after", false, v_e1 ); 
		    if(useCorrPfMet_) {
		      fillPfMetHistograms( "after", true, v_e1 ); 
		    }  
		    fillTcMetHistograms( "after", v_e1 ); 
  
		    if(useGenMet_) {
		      //Gen electron???
		      fillGenMetHistograms( "after", v_e1 ); 
		    }

		  }
		  met_hist_done = true;
		}

	      if ( not njets_hist_done && ( !electron_sel[12] || flags_passed==NFLAGS ) ) {
                fillCaloJets(CALO, "after");
                if ( useCorrCaloJets_ ) fillCaloJets(CALO_CORR, "after");
                fillPfJets(PF, "after");
                if ( useCorrPfJets_ ) fillPfJets(PF_CORR, "after");
                if ( useGenJets_ ) fillGenJets("after");

                njets_hist_done = true;
	      }
	      
	      // for quantities AFTER ALL CUTS
	      if (flags_passed == NFLAGS)
		{
		  highestEtElectronIsGood = true;
		  
		  reco::GsfElectron* elecCopy = new reco::GsfElectron(elec);
		  goodElecs->push_back(*elecCopy);
		  delete elecCopy;

		  // trigger
		  for (unsigned int n = 0; n < passedTriggers.size(); n++)
		    {
		      h_ele_triggersFired_after_->Fill(passedTriggers.at(n).c_str(),1);
		    }
		  
		  // spike criterion
		  h_ele_rMax3x3_after_->Fill(ele_rmax3x3_[i]);
		  if (ele_isBarrel_[i])
		    h_ele_rMax3x3Barrel_after_->Fill(ele_rmax3x3_[i]);
		  else if (ele_isEndcap_[i])
		    h_ele_rMax3x3Endcap_after_->Fill(ele_rmax3x3_[i]);
		  
		  h_ele_phi_after_->Fill(ele_phi_[i]);

		  // dxy
		  h_ele_dxy_after_->Fill(ele_dxy_[i]);

		  h_ele_gsfCharge_after_->Fill(ele_gsfcharge_[i]);

		  // fill gsf electron quantities
		  h_ele_ecalEnergy_after_->Fill(ele_ecalenergy_[i]) ;
		  h_ele_ecalEnergyError_after_->Fill(ele_ecalenergyerror_[i]) ;
		  
		  h_ele_deltaEtaEleClusterTrackAtCalo_after_->Fill(ele_deltaetaeleclustertrackatcalo_[i]) ;
		  h_ele_deltaEtaSeedClusterTrackAtCalo_after_->Fill(ele_deltaetaseedclustertrackatcalo_[i]) ;
		  h_ele_deltaEtaSuperClusterTrackAtVtx_after_->Fill(ele_deltaetasuperclustertrackatvtx_[i]) ;
		  
		  h_ele_deltaPhiEleClusterTrackAtCalo_after_->Fill(ele_deltaphieleclustertrackatcalo_[i]) ;
		  h_ele_deltaPhiSeedClusterTrackAtCalo_after_->Fill(ele_deltaphiseedclustertrackatcalo_[i]) ;
		  h_ele_deltaPhiSuperClusterTrackAtVtx_after_->Fill(ele_deltaphisuperclustertrackatvtx_[i]) ;
		  
		  h_ele_trackPositionAtVtxX_after_->Fill(ele_trackpositionatvtxx_[i]) ;
		  h_ele_trackPositionAtVtxY_after_->Fill(ele_trackpositionatvtxy_[i]) ;
		  h_ele_trackPositionAtVtxZ_after_->Fill(ele_trackpositionatvtxz_[i]) ;
		  
		  h_ele_trackMomentumAtVtxX_after_->Fill(ele_trackmomentumatvtxx_[i]) ;
		  h_ele_trackMomentumAtVtxY_after_->Fill(ele_trackmomentumatvtxy_[i]) ;
		  h_ele_trackMomentumAtVtxZ_after_->Fill(ele_trackmomentumatvtxz_[i]) ;
		  
		  h_ele_trackPositionAtCaloX_after_->Fill(ele_trackpositionatcalox_[i]) ;
		  h_ele_trackPositionAtCaloY_after_->Fill(ele_trackpositionatcaloy_[i]) ;
		  h_ele_trackPositionAtCaloZ_after_->Fill(ele_trackpositionatcaloz_[i]) ;
		  
		  h_ele_trackMomentumAtCaloX_after_->Fill(ele_trackmomentumatcalox_[i]) ;
		  h_ele_trackMomentumAtCaloY_after_->Fill(ele_trackmomentumatcaloy_[i]) ;
		  h_ele_trackMomentumAtCaloZ_after_->Fill(ele_trackmomentumatcaloz_[i]) ;
		  
		  h_ele_eEleClusterOverPout_after_->Fill(ele_eeleclusteroverpout_[i]) ;
		  h_ele_eSeedClusterOverP_after_->Fill(ele_eseedclusteroverp_[i]) ;
		  h_ele_eSeedClusterOverPout_after_->Fill(ele_eseedclusteroverpout_[i]) ;
		  h_ele_eSuperClusterOverP_after_->Fill(ele_esuperclusteroverp_[i]) ; 
		  
		  h_ele_electronMomentumError_after_->Fill(ele_electronmomentumerror_[i]) ;
		  
		  h_ele_dr03EcalRecHitSumEt_after_->Fill(ele_dr03ecalrechitsumet_[i]) ;
		  h_ele_dr03HcalDepth1TowerSumEt_after_->Fill(ele_dr03hcaldepth1towersumet_[i]) ;
		  h_ele_dr03HcalDepth2TowerSumEt_after_->Fill(ele_dr03hcaldepth2towersumet_[i]) ;
		  h_ele_dr03HcalTowerSumEt_after_->Fill(ele_dr03hcaltowersumet_[i]) ;
		  h_ele_dr03TkSumPt_after_->Fill(ele_dr03tksumpt_[i]) ;
		  
		  if (ele_isBarrel_[i])
		    {
		      h_ele_dr03EcalRecHitSumEtBarrel_after_->Fill(ele_dr03ecalrechitsumet_[i]) ;
		      h_ele_dr03HcalDepth1TowerSumEtBarrel_after_->Fill(ele_dr03hcaldepth1towersumet_[i]) ;
		      h_ele_dr03HcalDepth2TowerSumEtBarrel_after_->Fill(ele_dr03hcaldepth2towersumet_[i]) ;
		      h_ele_dr03HcalTowerSumEtBarrel_after_->Fill(ele_dr03hcaltowersumet_[i]) ;
		      h_ele_dr03TkSumPtBarrel_after_->Fill(ele_dr03tksumpt_[i]) ;
		    }
		  if (ele_isEndcap_[i])
		    {
		      h_ele_dr03EcalRecHitSumEtEndcap_after_->Fill(ele_dr03ecalrechitsumet_[i]) ;
		      h_ele_dr03HcalDepth1TowerSumEtEndcap_after_->Fill(ele_dr03hcaldepth1towersumet_[i]) ;
		      h_ele_dr03HcalDepth2TowerSumEtEndcap_after_->Fill(ele_dr03hcaldepth2towersumet_[i]) ;
		      h_ele_dr03HcalTowerSumEtEndcap_after_->Fill(ele_dr03hcaltowersumet_[i]) ;
		      h_ele_dr03TkSumPtEndcap_after_->Fill(ele_dr03tksumpt_[i]) ;
		    }
	  
		  h_ele_dr04EcalRecHitSumEt_after_->Fill(ele_dr04ecalrechitsumet_[i]) ;
		  h_ele_dr04HcalDepth1TowerSumEt_after_->Fill(ele_dr04hcaldepth1towersumet_[i]) ;
		  h_ele_dr04HcalDepth2TowerSumEt_after_->Fill(ele_dr04hcaldepth2towersumet_[i]) ;
		  h_ele_dr04HcalTowerSumEt_after_->Fill(ele_dr04hcaltowersumet_[i]) ;
		  h_ele_dr04TkSumPt_after_->Fill(ele_dr04tksumpt_[i]) ; 
		  
		  if (ele_isBarrel_[i])
		    {
		      h_ele_dr04EcalRecHitSumEtBarrel_after_->Fill(ele_dr04ecalrechitsumet_[i]) ;
		      h_ele_dr04HcalDepth1TowerSumEtBarrel_after_->Fill(ele_dr04hcaldepth1towersumet_[i]) ;
		      h_ele_dr04HcalDepth2TowerSumEtBarrel_after_->Fill(ele_dr04hcaldepth2towersumet_[i]) ;
		      h_ele_dr04HcalTowerSumEtBarrel_after_->Fill(ele_dr04hcaltowersumet_[i]) ;
		      h_ele_dr04TkSumPtBarrel_after_->Fill(ele_dr04tksumpt_[i]) ; 
		    }
		  if (ele_isEndcap_[i])
		    {
		      h_ele_dr04EcalRecHitSumEtEndcap_after_->Fill(ele_dr04ecalrechitsumet_[i]) ;
		      h_ele_dr04HcalDepth1TowerSumEtEndcap_after_->Fill(ele_dr04hcaldepth1towersumet_[i]) ;
		      h_ele_dr04HcalDepth2TowerSumEtEndcap_after_->Fill(ele_dr04hcaldepth2towersumet_[i]) ;
		      h_ele_dr04HcalTowerSumEtEndcap_after_->Fill(ele_dr04hcaltowersumet_[i]) ;
		      h_ele_dr04TkSumPtEndcap_after_->Fill(ele_dr04tksumpt_[i]) ; 
		    }

		  h_ele_e1x5_after_->Fill(ele_e1x5_[i]) ;
		  h_ele_e2x5Max_after_->Fill(ele_e2x5max_[i]) ;
		  h_ele_e5x5_after_->Fill(ele_e5x5_[i]) ;
		  h_ele_r1x5_after_->Fill(ele_r1x5_[i]) ;
		  h_ele_r2x5Max_after_->Fill(ele_r2x5max_[i]) ;
		  h_ele_scEtaWidth_after_->Fill(ele_scetawidth_[i]) ;
		  h_ele_scPhiWidth_after_->Fill(ele_scphiwidth_[i]) ;
		  h_ele_scR9_after_->Fill(ele_scr9_[i]) ;
		  h_ele_scESeedOverESuperCluster_after_->Fill(ele_sceseedoveresupercluster_[i]);
		  
		  if (ele_isBarrel_[i])
		    {
		      h_ele_e1x5Barrel_after_->Fill(ele_e1x5_[i]) ;
		      h_ele_e2x5MaxBarrel_after_->Fill(ele_e2x5max_[i]) ;
		      h_ele_e5x5Barrel_after_->Fill(ele_e5x5_[i]) ;
		      h_ele_r1x5Barrel_after_->Fill(ele_r1x5_[i]) ;
		      h_ele_r2x5MaxBarrel_after_->Fill(ele_r2x5max_[i]) ;
		      h_ele_scEtaWidthBarrel_after_->Fill(ele_scetawidth_[i]) ;
		      h_ele_scPhiWidthBarrel_after_->Fill(ele_scphiwidth_[i]) ;
		      h_ele_scR9Barrel_after_->Fill(ele_scr9_[i]) ;
		      h_ele_scESeedOverESuperClusterBarrel_after_->Fill(ele_sceseedoveresupercluster_[i]);
		    }	  
		  if (ele_isEndcap_[i])
		    {
		      h_ele_e1x5Endcap_after_->Fill(ele_e1x5_[i]) ;
		      h_ele_e2x5MaxEndcap_after_->Fill(ele_e2x5max_[i]) ;
		      h_ele_e5x5Endcap_after_->Fill(ele_e5x5_[i]) ;
		      h_ele_r1x5Endcap_after_->Fill(ele_r1x5_[i]) ;
		      h_ele_r2x5MaxEndcap_after_->Fill(ele_r2x5max_[i]) ;
		      h_ele_scPreshowerEnergyEndcap_after_->Fill(ele_scpreshowerenergy_[i]) ;
		      h_ele_scEtaWidthEndcap_after_->Fill(ele_scetawidth_[i]) ;
		      h_ele_scPhiWidthEndcap_after_->Fill(ele_scphiwidth_[i]) ;
		      h_ele_scR9Endcap_after_->Fill(ele_scr9_[i]) ;
		      h_ele_scESeedOverESuperClusterEndcap_after_->Fill(ele_sceseedoveresupercluster_[i]);
		    }	  

		  h_ele_hcalDepth1OverEcal_after_->Fill(ele_hcaldepth1overecal_[i]) ;
		  h_ele_hcalDepth2OverEcal_after_->Fill(ele_hcaldepth2overecal_[i]) ;
		  h_ele_hcalOverEcal_after_->Fill(ele_hcaloverecal_[i]) ; 
		  
		  if (ele_isBarrel_[i])
		    {
		      h_ele_hcalDepth1OverEcalBarrel_after_->Fill(ele_hcaldepth1overecal_[i]) ;
		      h_ele_hcalDepth2OverEcalBarrel_after_->Fill(ele_hcaldepth2overecal_[i]) ;
		      h_ele_hcalOverEcalBarrel_after_->Fill(ele_hcaloverecal_[i]) ; 
		    }	  
		  if (ele_isEndcap_[i])
		    {
		      h_ele_hcalDepth1OverEcalEndcap_after_->Fill(ele_hcaldepth1overecal_[i]) ;
		      h_ele_hcalDepth2OverEcalEndcap_after_->Fill(ele_hcaldepth2overecal_[i]) ;
		      h_ele_hcalOverEcalEndcap_after_->Fill(ele_hcaloverecal_[i]) ; 
		    }	  
		  
		  h_ele_sigmaEtaEta_after_->Fill(ele_sigmaetaeta_[i]) ;
		  h_ele_sigmaIetaIeta_after_->Fill(ele_sigmaietaieta_[i]) ; 
	  
		  if (ele_isBarrel_[i])
		    {
		      h_ele_sigmaEtaEtaBarrel_after_->Fill(ele_sigmaetaeta_[i]) ;
		      h_ele_sigmaIetaIetaBarrel_after_->Fill(ele_sigmaietaieta_[i]) ; 
		    }
		  if (ele_isEndcap_[i])
		    {
		      h_ele_sigmaEtaEtaEndcap_after_->Fill(ele_sigmaetaeta_[i]) ;
		      h_ele_sigmaIetaIetaEndcap_after_->Fill(ele_sigmaietaieta_[i]) ; 
		    }
		  
		  h_ele_basicClustersSize_after_->Fill(ele_basicclusterssize_[i]) ; 
		  h_ele_numberOfBrems_after_->Fill(ele_numberofbrems_[i]) ; 
		  h_ele_fbrem_after_->Fill(ele_fbrem_[i]) ; 
		  h_ele_scPixCharge_after_->Fill(ele_scpixcharge_[i]) ;
		  h_ele_ctfCharge_after_->Fill(ele_ctfcharge_[i]) ;
		  h_ele_charge_after_->Fill(ele_charge_[i]);
		  h_ele_gsfScPixChargeConsistent_after_->Fill(ele_isgsfscpixchargeconsistent_[i]);
		  h_ele_gsfCtfChargeConsistent_after_->Fill(ele_isgsfctfchargeconsistent_[i]);
		  h_ele_gsfCtfScPixChargeConsistent_after_->Fill(ele_isgsfctfscpixchargeconsistent_[i]);



		  // candidate quantities
		  h_ele_energy_after_->Fill(ele_energy_[i]) ;
		  h_ele_et_after_->Fill(ele_et_[i]) ;
		  h_ele_p_after_->Fill(ele_p_[i]) ;
		  h_ele_px_after_->Fill(ele_px_[i]) ;
		  h_ele_py_after_->Fill(ele_py_[i]) ;
		  h_ele_pz_after_->Fill(ele_pz_[i]) ;
		  
		  h_ele_vertexChi2_after_->Fill(ele_vertexChi2_[i]) ;
		  h_ele_vertexNdof_after_->Fill(ele_vertexNdof_[i]) ;
		  h_ele_vertexNormalizedChi2_after_->Fill(ele_vertexNormalizedChi2_[i]) ;
		  h_ele_vx_after_->Fill(ele_vx_[i]) ;
		  h_ele_vy_after_->Fill(ele_vy_[i]) ;
		  h_ele_vz_after_->Fill(ele_vz_[i]) ;
		  
		}
	      
            } // end N-1 histos block
	} // end loop through electrons

      LogTrace("") << "Numbers of good electrons is " << goodElecs->size() << std::endl;

      // efficiencies! with some loose electron ID requirements
      if (highestEtElectronDetain < 0.1 && highestEtElectronDetain > -0.1 && highestEtElectronHoE < 0.05)
	{
	  // fill trigger efficiency denominator histogram
<<<<<<< EwkElecStandaloneDQM.cc
	  //h_trg_Denominator_eff_->Fill(highestElectronEt);
	  h_trg_Denominator_eff_eta_->Fill(highestEtElectronEta);
	  h_trg_Denominator_eff_etaEt_->Fill(highestEtElectronEta,highestElectronEt,1.);

	  for (int nColls = 0; nColls < N_COLLS; nColls++)
	    {
	      for (int nEta = 0; nEta < N_ETA; nEta++)
		{
		  for (int nSel = 0; nSel < N_SEL; nSel++)
		    {
		      for (int nSeeds = 0; nSeeds < N_SEEDS; nSeeds++)
			{
			  // electrons
			  if (nColls == 0)
			    {
			      // barrel, endcap
			      if (nEta == 0 || 
				  (nEta == 1 && highestEtElectronIsBarrel) || 
				  (nEta == 2 && highestEtElectronIsEndcap) )
				{
				  h_trg_Denominator_Et_eff_[nColls][nEta][nSel][nSeeds]->Fill(highestElectronEt);

				  // FILLING THE NUMERATORS

				  // HLT photon triggers
				  if (triggersOfInterest_->find("HLT_Photon30_L1R") != triggersOfInterest_->end() && triggersOfInterest_->find("HLT_Photon30_L1R")->second == true) 
				    {
				      h_trg_HLT_Photon30_L1R_eff_[nColls][nEta][nSel][nSeeds]->Fill(highestElectronEt);
				    }
				  if (triggersOfInterest_->find("HLT_Photon20_L1R") != triggersOfInterest_->end() && triggersOfInterest_->find("HLT_Photon20_L1R")->second == true) 
				    {
				      h_trg_HLT_Photon20_L1R_eff_[nColls][nEta][nSel][nSeeds]->Fill(highestElectronEt);
				    }
				  if (triggersOfInterest_->find("HLT_Photon15_L1R") != triggersOfInterest_->end() && triggersOfInterest_->find("HLT_Photon15_L1R")->second == true) 
				    {
				      h_trg_HLT_Photon15_L1R_eff_[nColls][nEta][nSel][nSeeds]->Fill(highestElectronEt);
				    }
				  if (triggersOfInterest_->find("HLT_Photon10_L1R") != triggersOfInterest_->end() && triggersOfInterest_->find("HLT_Photon10_L1R")->second == true) 
				    {
				      h_trg_HLT_Photon10_L1R_eff_[nColls][nEta][nSel][nSeeds]->Fill(highestElectronEt);
				    }

				  // HLT electron triggers
				  if (triggersOfInterest_->find("HLT_Ele20_LW_L1R") != triggersOfInterest_->end() && triggersOfInterest_->find("HLT_Ele20_LW_L1R")->second == true) 
				    {
				      h_trg_HLT_Ele20_LW_L1R_eff_[nColls][nEta][nSel][nSeeds]->Fill(highestElectronEt);
				    }
				  if (triggersOfInterest_->find("HLT_Ele15_LW_L1R") != triggersOfInterest_->end() && triggersOfInterest_->find("HLT_Ele15_LW_L1R")->second == true) 
				    {
				      h_trg_HLT_Ele15_LW_L1R_eff_[nColls][nEta][nSel][nSeeds]->Fill(highestElectronEt);
				    }
				  if (triggersOfInterest_->find("HLT_Ele10_LW_L1R") != triggersOfInterest_->end() && triggersOfInterest_->find("HLT_Ele10_LW_L1R")->second == true) 
				    {
				      h_trg_HLT_Ele10_LW_L1R_eff_[nColls][nEta][nSel][nSeeds]->Fill(highestElectronEt);
				    }
				  
				  // L1 EG triggers
				  if (l1TriggersOfInterest_->find("L1_SingleEG20") != l1TriggersOfInterest_->end() && l1TriggersOfInterest_->find("L1_Single20")->second == true) 
				    {
				      h_trg_L1_SingleEG20_eff_[nColls][nEta][nSel][nSeeds]->Fill(highestElectronEt);
				    }
				  if (l1TriggersOfInterest_->find("L1_SingleEG15") != l1TriggersOfInterest_->end() && l1TriggersOfInterest_->find("L1_Single15")->second == true) 
				    {
				      h_trg_L1_SingleEG15_eff_[nColls][nEta][nSel][nSeeds]->Fill(highestElectronEt);
				    }
				  if (l1TriggersOfInterest_->find("L1_SingleEG12") != l1TriggersOfInterest_->end() && l1TriggersOfInterest_->find("L1_Single12")->second == true) 
				    {
				      h_trg_L1_SingleEG12_eff_[nColls][nEta][nSel][nSeeds]->Fill(highestElectronEt);
				    }
				  if (l1TriggersOfInterest_->find("L1_SingleEG10") != l1TriggersOfInterest_->end() && l1TriggersOfInterest_->find("L1_Single10")->second == true) 
				    {
				      h_trg_L1_SingleEG10_eff_[nColls][nEta][nSel][nSeeds]->Fill(highestElectronEt);
				    }
				  if (l1TriggersOfInterest_->find("L1_SingleEG8") != l1TriggersOfInterest_->end() && l1TriggersOfInterest_->find("L1_Single8")->second == true) 
				    {
				      h_trg_L1_SingleEG8_eff_[nColls][nEta][nSel][nSeeds]->Fill(highestElectronEt);
				    }
				  if (l1TriggersOfInterest_->find("L1_SingleEG5") != l1TriggersOfInterest_->end() && l1TriggersOfInterest_->find("L1_Single5")->second == true) 
				    {
				      h_trg_L1_SingleEG5_eff_[nColls][nEta][nSel][nSeeds]->Fill(highestElectronEt);
				    }
				  if (l1TriggersOfInterest_->find("L1_SingleEG2") != l1TriggersOfInterest_->end() && l1TriggersOfInterest_->find("L1_Single2")->second == true) 
				    {
				      h_trg_L1_SingleEG2_eff_[nColls][nEta][nSel][nSeeds]->Fill(highestElectronEt);
				    }

				  
				}

			    }
			  // superclusters
			  if (nColls == 1)
			    {
			    }
			}
		    }
		}
	    }
=======
	  h_trg_Denominator_eff_->Fill(highestElectronEt);
	  h_trg_Denominator_eff_eta_->Fill(highestEtElectronEta);
	  h_trg_Denominator_eff_etaPt_->Fill(highestEtElectronEta,highestElectronEt,1.);
>>>>>>> 1.31
	  
	  // fill special denoms
	  if (highestElectronEt > 5.)
	    {
<<<<<<< EwkElecStandaloneDQM.cc
	      h_trg_Denominator_eff_eta_cutEt5_->Fill(highestEtElectronEta);
=======
	      h_trg_Denominator_eff_eta_cutPt5_->Fill(highestEtElectronEta);
>>>>>>> 1.31
	    }
	  if (highestEtElectronIsEcalDriven)
	    {
	      h_trg_Denominator_eff_ecalDriven_->Fill(highestElectronEt);
	    }
	  if (highestEtElectronIsBarrel)
	    {
	      h_trg_Denominator_eff_barrel_->Fill(highestElectronEt);
	    }
	  if (highestEtElectronIsGood)
	    {
	      h_trg_Denominator_eff_goodEle_->Fill(highestElectronEt);
	    }

	  // fill trigger efficiency numerator histograms
	  //       if (highestElectronEt > 30.)
	  // 	{
	  if (triggersOfInterest_->find("HLT_Photon30_L1R") != triggersOfInterest_->end() && triggersOfInterest_->find("HLT_Photon30_L1R")->second == true) 
	    {
<<<<<<< EwkElecStandaloneDQM.cc
	      h_trg_HLT_Photon30_L1R_eff_->Fill(highestElectronEt);
	      h_trg_highestEtVsThreshold_->Fill("HLT_Photon30_L1R",highestElectronEt,1.);
=======
	      h_trg_HLT_Photon30_L1R_eff_->Fill(highestElectronEt);
	      h_trg_highestEtVsThreshold_->Fill("HLT_Photon30_L1R",highestElectronEt,1.);
	      if (highestEtElectronIsGood)
		{
		  h_trg_HLT_Photon30_L1R_eff_goodEle_->Fill(highestElectronEt);
		}
>>>>>>> 1.31
	    }
	  // 	}
	  //       if (highestElectronEt > 20.)
	  // 	{
	  if (triggersOfInterest_->find("HLT_Ele20_LW_L1R") != triggersOfInterest_->end() && triggersOfInterest_->find("HLT_Ele20_LW_L1R")->second == true) 
	    {
<<<<<<< EwkElecStandaloneDQM.cc
	      h_trg_HLT_Ele20_LW_L1R_eff_->Fill(highestElectronEt);
	      h_trg_highestEtVsThreshold_->Fill("HLT_Ele20_LW_L1R",highestElectronEt,1.);
=======
	      h_trg_HLT_Ele20_LW_L1R_eff_->Fill(highestElectronEt);
	      h_trg_highestEtVsThreshold_->Fill("HLT_Ele20_LW_L1R",highestElectronEt,1.);
	      if (highestEtElectronIsGood)
		{
		  h_trg_HLT_Ele20_LW_L1R_eff_goodEle_->Fill(highestElectronEt);
		}
>>>>>>> 1.31
	    }
	  if (triggersOfInterest_->find("HLT_Photon20_L1R") != triggersOfInterest_->end() && triggersOfInterest_->find("HLT_Photon20_L1R")->second == true) 
	    {
<<<<<<< EwkElecStandaloneDQM.cc
	      h_trg_HLT_Photon20_L1R_eff_->Fill(highestElectronEt);
	      h_trg_highestEtVsThreshold_->Fill("HLT_Photon20_L1R",highestElectronEt,1.);
=======
	      h_trg_HLT_Photon20_L1R_eff_->Fill(highestElectronEt);
	      h_trg_highestEtVsThreshold_->Fill("HLT_Photon20_L1R",highestElectronEt,1.);
	      if (highestEtElectronIsGood)
		{
		  h_trg_HLT_Photon20_L1R_eff_goodEle_->Fill(highestElectronEt);
		}
>>>>>>> 1.31
	    }
	  // L1!
	  if (l1TriggersOfInterest_->find("L1_SingleEG20") != l1TriggersOfInterest_->end() && l1TriggersOfInterest_->find("L1_SingleEG20")->second == true) 
	    {
<<<<<<< EwkElecStandaloneDQM.cc
	      h_trg_L1_SingleEG20_eff_->Fill(highestElectronEt);
	      h_trg_highestEtVsThreshold_->Fill("L1_SingleEG20",highestElectronEt,1.);
=======
	      h_trg_L1_SingleEG20_eff_->Fill(highestElectronEt);
	      h_trg_highestEtVsThreshold_->Fill("L1_SingleEG20",highestElectronEt,1.);
	      if (highestEtElectronIsGood)
		{
		  h_trg_L1_SingleEG20_eff_goodEle_->Fill(highestElectronEt);
		}
>>>>>>> 1.31
	    }
	  // 	}
	  //       if (highestElectronEt > 15.)
	  // 	{
	  if (triggersOfInterest_->find("HLT_Ele15_LW_L1R") != triggersOfInterest_->end() && triggersOfInterest_->find("HLT_Ele15_LW_L1R")->second == true) 
	    {
<<<<<<< EwkElecStandaloneDQM.cc
	      h_trg_HLT_Ele15_LW_L1R_eff_->Fill(highestElectronEt);
	      h_trg_highestEtVsThreshold_->Fill("HLT_Ele15_LW_L1R",highestElectronEt,1.);
=======
	      h_trg_HLT_Ele15_LW_L1R_eff_->Fill(highestElectronEt);
	      h_trg_highestEtVsThreshold_->Fill("HLT_Ele15_LW_L1R",highestElectronEt,1.);
	      if (highestEtElectronIsGood)
		{
		  h_trg_HLT_Ele15_LW_L1R_eff_goodEle_->Fill(highestElectronEt);
		}
>>>>>>> 1.31
	    }
	  if (triggersOfInterest_->find("HLT_Ele15_SiStrip_L1R") != triggersOfInterest_->end() && triggersOfInterest_->find("HLT_Ele15_SiStrip_L1R")->second == true) 
	    {
<<<<<<< EwkElecStandaloneDQM.cc
	      h_trg_HLT_Ele15_SiStrip_L1R_eff_->Fill(highestElectronEt);
	      h_trg_highestEtVsThreshold_->Fill("HLT_Ele15_SiStrip_L1R",highestElectronEt,1.);
=======
	      h_trg_HLT_Ele15_SiStrip_L1R_eff_->Fill(highestElectronEt);
	      h_trg_highestEtVsThreshold_->Fill("HLT_Ele15_SiStrip_L1R",highestElectronEt,1.);
	      if (highestEtElectronIsGood)
		{
		  h_trg_HLT_Ele15_SiStrip_L1R_eff_goodEle_->Fill(highestElectronEt);
		}
>>>>>>> 1.31
	    }
	  if (triggersOfInterest_->find("HLT_Photon15_L1R") != triggersOfInterest_->end() && triggersOfInterest_->find("HLT_Photon15_L1R")->second == true) 
	    {
<<<<<<< EwkElecStandaloneDQM.cc
	      h_trg_HLT_Photon15_L1R_eff_->Fill(highestElectronEt);
	      h_trg_highestEtVsThreshold_->Fill("HLT_Photon15_L1R",highestElectronEt,1.);
=======
	      h_trg_HLT_Photon15_L1R_eff_->Fill(highestElectronEt);
	      h_trg_highestEtVsThreshold_->Fill("HLT_Photon15_L1R",highestElectronEt,1.);
	      if (highestEtElectronIsGood)
		{
		  h_trg_HLT_Photon15_L1R_eff_goodEle_->Fill(highestElectronEt);
		}
>>>>>>> 1.31
	    }
	  if (triggersOfInterest_->find("HLT_Photon15_TrackIso_L1R") != triggersOfInterest_->end() && triggersOfInterest_->find("HLT_Photon15_TrackIso_L1R")->second == true) 
	    {
<<<<<<< EwkElecStandaloneDQM.cc
	      h_trg_HLT_Photon15_TrackIso_L1R_eff_->Fill(highestElectronEt);
	      h_trg_highestEtVsThreshold_->Fill("HLT_Photon15_TrackIso_L1R",highestElectronEt,1.);
=======
	      h_trg_HLT_Photon15_TrackIso_L1R_eff_->Fill(highestElectronEt);
	      h_trg_highestEtVsThreshold_->Fill("HLT_Photon15_TrackIso_L1R",highestElectronEt,1.);
	      if (highestEtElectronIsGood)
		{
		  h_trg_HLT_Photon15_TrackIso_L1R_eff_goodEle_->Fill(highestElectronEt);
		}

>>>>>>> 1.31
	    }
	  if (triggersOfInterest_->find("HLT_Photon15_LooseEcalIso_L1R") != triggersOfInterest_->end() && triggersOfInterest_->find("HLT_Photon15_LooseEcalIso_L1R")->second == true) 
	    {
<<<<<<< EwkElecStandaloneDQM.cc
	      h_trg_HLT_Photon15_LooseEcalIso_L1R_eff_->Fill(highestElectronEt);
	      h_trg_highestEtVsThreshold_->Fill("HLT_Photon15_LooseEcalIso_L1R",highestElectronEt,1.);
=======
	      h_trg_HLT_Photon15_LooseEcalIso_L1R_eff_->Fill(highestElectronEt);
	      h_trg_highestEtVsThreshold_->Fill("HLT_Photon15_LooseEcalIso_L1R",highestElectronEt,1.);
	      if (highestEtElectronIsGood)
		{
		  h_trg_HLT_Photon15_LooseEcalIso_L1R_eff_goodEle_->Fill(highestElectronEt);
		}
>>>>>>> 1.31
	    }
      
	  // L1!
	  if (l1TriggersOfInterest_->find("L1_SingleIsoEG15") != l1TriggersOfInterest_->end() && l1TriggersOfInterest_->find("L1_SingleIsoEG15")->second == true) 
	    {
<<<<<<< EwkElecStandaloneDQM.cc
	      h_trg_L1_SingleIsoEG15_eff_->Fill(highestElectronEt);
	      h_trg_highestEtVsThreshold_->Fill("L1_SingleIsoEG15",highestElectronEt,1.);
=======
	      h_trg_L1_SingleIsoEG15_eff_->Fill(highestElectronEt);
	      h_trg_highestEtVsThreshold_->Fill("L1_SingleIsoEG15",highestElectronEt,1.);
	      if (highestEtElectronIsGood)
		{
		  h_trg_L1_SingleIsoEG15_eff_goodEle_->Fill(highestElectronEt);
		}
>>>>>>> 1.31
	    }
	  // L1!
	  if (l1TriggersOfInterest_->find("L1_SingleEG15") != l1TriggersOfInterest_->end() && l1TriggersOfInterest_->find("L1_SingleEG15")->second == true) 
	    {
<<<<<<< EwkElecStandaloneDQM.cc
	      h_trg_L1_SingleEG15_eff_->Fill(highestElectronEt);
	      h_trg_highestEtVsThreshold_->Fill("L1_SingleEG15",highestElectronEt,1.);
=======
	      h_trg_L1_SingleEG15_eff_->Fill(highestElectronEt);
	      h_trg_highestEtVsThreshold_->Fill("L1_SingleEG15",highestElectronEt,1.);
	      if (highestEtElectronIsGood)
		{
		  h_trg_L1_SingleEG15_eff_goodEle_->Fill(highestElectronEt);
		}
>>>>>>> 1.31
	    }
	  // 	}
	  //       if (highestElectronEt > 12.)
	  // 	{
	  // L1!
	  if (l1TriggersOfInterest_->find("L1_SingleIsoEG12") != l1TriggersOfInterest_->end() && l1TriggersOfInterest_->find("L1_SingleIsoEG12")->second == true) 
	    {
<<<<<<< EwkElecStandaloneDQM.cc
	      h_trg_L1_SingleIsoEG12_eff_->Fill(highestElectronEt);
	      h_trg_highestEtVsThreshold_->Fill("L1_SingleIsoEG12",highestElectronEt,1.);
=======
	      h_trg_L1_SingleIsoEG12_eff_->Fill(highestElectronEt);
	      h_trg_highestEtVsThreshold_->Fill("L1_SingleIsoEG12",highestElectronEt,1.);
	      if (highestEtElectronIsGood)
		{
		  h_trg_L1_SingleIsoEG12_eff_goodEle_->Fill(highestElectronEt);
		}
>>>>>>> 1.31
	    }
	  // L1!
	  if (l1TriggersOfInterest_->find("L1_SingleEG12") != l1TriggersOfInterest_->end() && l1TriggersOfInterest_->find("L1_SingleEG12")->second == true) 
	    {
<<<<<<< EwkElecStandaloneDQM.cc
	      h_trg_L1_SingleEG12_eff_->Fill(highestElectronEt);
	      h_trg_highestEtVsThreshold_->Fill("L1_SingleEG12",highestElectronEt,1.);
=======
	      h_trg_L1_SingleEG12_eff_->Fill(highestElectronEt);
	      h_trg_highestEtVsThreshold_->Fill("L1_SingleEG12",highestElectronEt,1.);
	      if (highestEtElectronIsGood)
		{
		  h_trg_L1_SingleEG12_eff_goodEle_->Fill(highestElectronEt);
		}
>>>>>>> 1.31
	    }
	  // 	}
	  //       if (highestElectronEt > 10.)
	  // 	{
	  if (triggersOfInterest_->find("HLT_Ele10_LW_L1R") != triggersOfInterest_->end() && triggersOfInterest_->find("HLT_Ele10_LW_L1R")->second == true) 
	    {
<<<<<<< EwkElecStandaloneDQM.cc
	      h_trg_HLT_Ele10_LW_L1R_eff_->Fill(highestElectronEt);
	      h_trg_highestEtVsThreshold_->Fill("HLT_Ele10_LW_L1R",highestElectronEt,1.);
=======
	      h_trg_HLT_Ele10_LW_L1R_eff_->Fill(highestElectronEt);
	      h_trg_highestEtVsThreshold_->Fill("HLT_Ele10_LW_L1R",highestElectronEt,1.);
	      if (highestEtElectronIsGood)
		{
		  h_trg_HLT_Ele10_LW_L1R_eff_goodEle_->Fill(highestElectronEt);
		}
>>>>>>> 1.31
	    }
	  if (triggersOfInterest_->find("HLT_Ele10_LW_EleId_L1R") != triggersOfInterest_->end() && triggersOfInterest_->find("HLT_Ele10_LW_EleId_L1R")->second == true) 
	    {
<<<<<<< EwkElecStandaloneDQM.cc
	      h_trg_HLT_Ele10_LW_EleId_L1R_eff_->Fill(highestElectronEt);
	      h_trg_highestEtVsThreshold_->Fill("HLT_Ele10_LW_EleId_L1R",highestElectronEt,1.);
=======
	      h_trg_HLT_Ele10_LW_EleId_L1R_eff_->Fill(highestElectronEt);
	      h_trg_highestEtVsThreshold_->Fill("HLT_Ele10_LW_EleId_L1R",highestElectronEt,1.);
	      if (highestEtElectronIsGood)
		{
		  h_trg_HLT_Ele10_LW_EleId_L1R_eff_goodEle_->Fill(highestElectronEt);
		}

>>>>>>> 1.31
	    }
	  if (triggersOfInterest_->find("HLT_Photon10_L1R") != triggersOfInterest_->end() && triggersOfInterest_->find("HLT_Photon10_L1R")->second == true) 
	    {
<<<<<<< EwkElecStandaloneDQM.cc
	      h_trg_HLT_Photon10_L1R_eff_->Fill(highestElectronEt);
	      h_trg_highestEtVsThreshold_->Fill("HLT_Photon10_L1R",highestElectronEt,1.);
=======
	      h_trg_HLT_Photon10_L1R_eff_->Fill(highestElectronEt);
	      h_trg_highestEtVsThreshold_->Fill("HLT_Photon10_L1R",highestElectronEt,1.);
	      if (highestEtElectronIsGood)
		{
		  h_trg_HLT_Photon10_L1R_eff_goodEle_->Fill(highestElectronEt);
		}
>>>>>>> 1.31
	    }
	  // L1!
	  if (l1TriggersOfInterest_->find("L1_SingleIsoEG10") != l1TriggersOfInterest_->end() && l1TriggersOfInterest_->find("L1_SingleIsoEG10")->second == true) 
	    {
<<<<<<< EwkElecStandaloneDQM.cc
	      h_trg_L1_SingleIsoEG10_eff_->Fill(highestElectronEt);
	      h_trg_highestEtVsThreshold_->Fill("L1_SingleIsoEG10",highestElectronEt,1.);
=======
	      h_trg_L1_SingleIsoEG10_eff_->Fill(highestElectronEt);
	      h_trg_highestEtVsThreshold_->Fill("L1_SingleIsoEG10",highestElectronEt,1.);
	      if (highestEtElectronIsGood)
		{
		  h_trg_L1_SingleIsoEG10_eff_goodEle_->Fill(highestElectronEt);
		}
>>>>>>> 1.31
	    }
	  // L1!
	  if (l1TriggersOfInterest_->find("L1_SingleEG10") != l1TriggersOfInterest_->end() && l1TriggersOfInterest_->find("L1_SingleEG10")->second == true) 
	    {
<<<<<<< EwkElecStandaloneDQM.cc
	      h_trg_L1_SingleEG10_eff_->Fill(highestElectronEt);
	      h_trg_highestEtVsThreshold_->Fill("L1_SingleEG10",highestElectronEt,1.);
=======
	      h_trg_L1_SingleEG10_eff_->Fill(highestElectronEt);
	      h_trg_highestEtVsThreshold_->Fill("L1_SingleEG10",highestElectronEt,1.);
	      if (highestEtElectronIsGood)
		{
		  h_trg_L1_SingleEG10_eff_goodEle_->Fill(highestElectronEt);
		}
>>>>>>> 1.31
	    }
	  // 	}
	  //       if (highestElectronEt > 8.)
	  // 	{
	  if (triggersOfInterest_->find("HLT_L1SingleEG8") != triggersOfInterest_->end() && triggersOfInterest_->find("HLT_L1SingleEG8")->second == true) 
	    {
<<<<<<< EwkElecStandaloneDQM.cc
	      h_trg_HLT_L1SingleEG8_eff_->Fill(highestElectronEt);
	      h_trg_highestEtVsThreshold_->Fill("HLT_L1SingleEG8",highestElectronEt,1.);
=======
	      h_trg_HLT_L1SingleEG8_eff_->Fill(highestElectronEt);
	      h_trg_highestEtVsThreshold_->Fill("HLT_L1SingleEG8",highestElectronEt,1.);
	      if (highestEtElectronIsGood)
		{
		  h_trg_HLT_L1SingleEG8_eff_goodEle_->Fill(highestElectronEt);
		}
>>>>>>> 1.31
	    }
	  // L1!
	  if (l1TriggersOfInterest_->find("L1_SingleIsoEG8") != l1TriggersOfInterest_->end() && l1TriggersOfInterest_->find("L1_SingleIsoEG8")->second == true) 
	    {
<<<<<<< EwkElecStandaloneDQM.cc
	      h_trg_L1_SingleIsoEG8_eff_->Fill(highestElectronEt);
	      h_trg_highestEtVsThreshold_->Fill("L1_SingleIsoEG8",highestElectronEt,1.);
=======
	      h_trg_L1_SingleIsoEG8_eff_->Fill(highestElectronEt);
	      h_trg_highestEtVsThreshold_->Fill("L1_SingleIsoEG8",highestElectronEt,1.);
	      if (highestEtElectronIsGood)
		{
		  h_trg_L1_SingleIsoEG8_eff_goodEle_->Fill(highestElectronEt);
		}
>>>>>>> 1.31
	    }
	  // L1!
	  if (l1TriggersOfInterest_->find("L1_SingleEG8") != l1TriggersOfInterest_->end() && l1TriggersOfInterest_->find("L1_SingleEG8")->second == true) 
	    {
<<<<<<< EwkElecStandaloneDQM.cc
	      h_trg_L1_SingleEG8_eff_->Fill(highestElectronEt);
	      h_trg_highestEtVsThreshold_->Fill("L1_SingleEG8",highestElectronEt,1.);
=======
	      h_trg_L1_SingleEG8_eff_->Fill(highestElectronEt);
	      h_trg_highestEtVsThreshold_->Fill("L1_SingleEG8",highestElectronEt,1.);
	      if (highestEtElectronIsGood)
		{
		  h_trg_L1_SingleEG8_eff_goodEle_->Fill(highestElectronEt);
		}
>>>>>>> 1.31
	    }
	  // 	}
	  //       if (highestElectronEt > 5.)
	  // 	{
	  if (triggersOfInterest_->find("HLT_L1SingleEG5") != triggersOfInterest_->end() && triggersOfInterest_->find("HLT_L1SingleEG5")->second == true) 
	    {
<<<<<<< EwkElecStandaloneDQM.cc
	      h_trg_HLT_L1SingleEG5_eff_->Fill(highestElectronEt);
	      h_trg_highestEtVsThreshold_->Fill("HLT_L1SingleEG5",highestElectronEt,1.);
=======
	      h_trg_HLT_L1SingleEG5_eff_->Fill(highestElectronEt);
	      h_trg_highestEtVsThreshold_->Fill("HLT_L1SingleEG5",highestElectronEt,1.);
	      if (highestEtElectronIsGood)
		{
		  h_trg_HLT_L1SingleEG5_eff_goodEle_->Fill(highestElectronEt);
		}
>>>>>>> 1.31
	    }
	  // L1!
	  if (l1TriggersOfInterest_->find("L1_SingleIsoEG5") != l1TriggersOfInterest_->end() && l1TriggersOfInterest_->find("L1_SingleIsoEG5")->second == true) 
	    {
<<<<<<< EwkElecStandaloneDQM.cc
	      h_trg_L1_SingleIsoEG5_eff_->Fill(highestElectronEt);
	      h_trg_highestEtVsThreshold_->Fill("L1_SingleIsoEG5",highestElectronEt,1.);
=======
	      h_trg_L1_SingleIsoEG5_eff_->Fill(highestElectronEt);
	      h_trg_highestEtVsThreshold_->Fill("L1_SingleIsoEG5",highestElectronEt,1.);
	      if (highestEtElectronIsGood)
		{
		  h_trg_L1_SingleIsoEG5_eff_goodEle_->Fill(highestElectronEt);
		}
>>>>>>> 1.31
	    }
	  // L1!
	  if (l1TriggersOfInterest_->find("L1_SingleEG5") != l1TriggersOfInterest_->end() && l1TriggersOfInterest_->find("L1_SingleEG5")->second == true) 
	    {
<<<<<<< EwkElecStandaloneDQM.cc
	      h_trg_L1_SingleEG5_eff_->Fill(highestElectronEt);
	      h_trg_L1_SingleEG5_eff_eta_->Fill(highestEtElectronEta);
	      h_trg_L1_SingleEG5_eff_etaEt_->Fill(highestEtElectronEta,highestElectronEt,1.);
	      h_trg_highestEtVsThreshold_->Fill("L1_SingleEG5",highestElectronEt,1.);
	      if (highestElectronEt > 5.)
=======
	      h_trg_L1_SingleEG5_eff_->Fill(highestElectronEt);
	      h_trg_L1_SingleEG5_eff_eta_->Fill(highestEtElectronEta);
	      h_trg_L1_SingleEG5_eff_etaPt_->Fill(highestEtElectronEta,highestElectronEt,1.);
	      h_trg_highestEtVsThreshold_->Fill("L1_SingleEG5",highestElectronEt,1.);
	      if (highestElectronEt > 5.)
>>>>>>> 1.31
		{
<<<<<<< EwkElecStandaloneDQM.cc
		  h_trg_L1_SingleEG5_eff_eta_cutEt5_->Fill(highestEtElectronEta);
=======
		  h_trg_L1_SingleEG5_eff_eta_cutPt5_->Fill(highestEtElectronEta);
>>>>>>> 1.31
		}
	      if (highestEtElectronIsBarrel)
		{
		  h_trg_L1_SingleEG5_eff_barrel_->Fill(highestElectronEt);
		}
	      if (highestEtElectronIsEcalDriven)
		{
		  h_trg_L1_SingleEG5_eff_ecalDriven_->Fill(highestElectronEt);
		}
	      if (highestEtElectronIsGood)
		{
		  h_trg_L1_SingleEG5_eff_goodEle_->Fill(highestElectronEt);
		}
	    }
	  else if (l1TriggersOfInterest_->find("L1_SingleEG5") != l1TriggersOfInterest_->end())
	    {
	      h_trg_L1_SingleEG5_eff_notfire1_->Fill(highestElectronEt);
	      h_trg_L1_SingleEG5_eff_eta_notfire1_->Fill(highestEtElectronEta);
	    }
	  else
	    {
	      h_trg_L1_SingleEG5_eff_notfire2_->Fill(highestElectronEt);
	      h_trg_L1_SingleEG5_eff_eta_notfire2_->Fill(highestEtElectronEta);
	    }
	  // 	}
	  //       if (highestElectronEt > 2.)
	  // 	{
	  // L1! 2!
	  if (l1TriggersOfInterest_->find("L1_SingleEG2") != l1TriggersOfInterest_->end() && l1TriggersOfInterest_->find("L1_SingleEG2")->second == true) 
	    {
<<<<<<< EwkElecStandaloneDQM.cc
	      h_trg_L1_SingleEG2_eff_->Fill(highestElectronEt);
	      h_trg_highestEtVsThreshold_->Fill("L1_SingleEG2",highestElectronEt,1.);
=======
	      h_trg_L1_SingleEG2_eff_->Fill(highestElectronEt);
	      h_trg_highestEtVsThreshold_->Fill("L1_SingleEG2",highestElectronEt,1.);
	      if (highestEtElectronIsGood)
		{
		  h_trg_L1_SingleEG2_eff_goodEle_->Fill(highestElectronEt);
		}
>>>>>>> 1.31
	    }
	  //	}
	}      


      // construct Z candidates for initial electron collection (can't use for goodElecs!)
      //      edm::Handle<reco::CandidateCollection> electronCandidates;
      //      ev.getByLabel( "gsfElectrons", electronCandidates );
      //      reco::CandidateRefProd electronsRef( electronCandidates );
      edm::Handle<CandidateView> electronView;
      ev.getByLabel( "gsfElectrons", electronView );
      //      MassRangeSelector<reco::Candidate> massRange( 70, 110 ); // leave as-is right now
      //      MassRangeSelector massRange( 70., 110. ); 
      MassRangeSelector massRange( mZMin_, mZMax_ ); 
      //      CandCombiner<MassRangeSelector<reco::Candidate> > combiner( +1, -1 );
      CandCombiner<MassRangeSelector> combiner( massRange, +1, -1 );
      //      std::auto_ptr<reco::CompositeCandidateCollection> zCands = combiner.combine( electronsRef );
      std::auto_ptr<reco::CompositeCandidateCollection> zCands_before = combiner.combine( electronView );
      LogTrace("") << "Before cuts: Reconstructed " << zCands_before->size() << " Z candidates" << endl;

      // create new composite candidate collection
      std::auto_ptr<CompositeCandidateCollection> zCands_after(new CompositeCandidateCollection); 
      // loop over first elec
      for (unsigned int index1 = 0; index1 < goodElecs->size(); index1++)
	{
	  // loop over second elec index > 1st elec
	  for (unsigned int index2 = index1 + 1; index2 < goodElecs->size(); index2++)
	    {
// 	      // get values of daughter particles
// 	      int charge1 = goodElecs->at(index1).charge();
// 	      int charge2 = goodElecs->at(index2).charge();
// 	      math::XYZTLorentzVector fourMom1 = goodElecs->at(index1).p4();
// 	      math::XYZTLorentzVector fourMom2 = goodElecs->at(index2).p4();
// 	      // create new composite candidate, push back
// 	      CompositeCandidate* candidate = new CompositeCandidate( (charge1 + charge2), (fourMom1 + fourMom2) );


// Kira says this crashes on some events, her ad hoc replacement below
// 	      CompositeCandidate* candidate;
// 	      candidate->addDaughter(goodElecs->at(index1));
// 	      candidate->addDaughter(goodElecs->at(index2));
// 	      AddFourMomenta addP4;
// 	      addP4.set( *candidate );

// 	      zCands_after->push_back(*candidate);
// 	      delete candidate;

	      CompositeCandidate candidate;
	      candidate.addDaughter(goodElecs->at(index1));
	      candidate.addDaughter(goodElecs->at(index2));
	      AddFourMomenta addP4;
	      addP4.set( candidate );

	      zCands_after->push_back(candidate);

	      std::cout << "ZCAND AFTER CUTS in run " << runNumber_ << " LS " << lumiSection_ 
			<< " event " << eventNumber_ << " with mass " << candidate.mass() << std::endl;
	      hasZCand_ = true;
	    }
	}
      LogTrace("") << "After cuts: Reconstructed " << zCands_after->size() << " Z candidates" << endl;


      // inv mass = sqrt(m_1^2 + m_2^2 + 2*(E_1*E_2 - (px1*px2 + py1*py2 + pz1+pz2) ) )
      double invMass;

      h_ele_nelectrons_before_->Fill(ele_nelectrons_);
      if (ele_nelectrons_ > 1)
	{
	  invMass = sqrt(electron[0][1] + electron[1][1] + 2*(electron[0][2]*electron[1][2] - (electron[0][3]*electron[1][3] + electron[0][4]*electron[1][4] + electron[0][5]*electron[1][5]) ) );
	  h_ele_invmass_before_->Fill(invMass);
	  for (unsigned int cand = 0; cand < zCands_before->size(); cand++)
	    {
	      h_Z_pt_before_->Fill(zCands_before->at(cand).pt());
	      h_Z_eta_before_->Fill(zCands_before->at(cand).eta());
	      h_Z_phi_before_->Fill(zCands_before->at(cand).phi());
	      h_Z_mass_before_->Fill(zCands_before->at(cand).mass());
	    }
	}

      int nGoodElecs = goodElecs->size();

      //      ele_nelectrons_after_->Fill(nGoodElectrons);
      h_ele_nelectrons_after_->Fill(nGoodElecs);
      //      if (nGoodElectrons > 1)
      if (nGoodElecs > 1)
	{
	  invMass = sqrt(goodElectron[0][1] + goodElectron[1][1] + 2*(goodElectron[0][2]*goodElectron[1][2] - (goodElectron[0][3]*goodElectron[1][3] + goodElectron[0][4]*goodElectron[1][4] + goodElectron[0][5]*goodElectron[1][5]) ) );
	  h_ele_invmass_after_->Fill(invMass);
	  for (unsigned int cand = 0; cand < zCands_after->size(); cand++)
	    {
	      h_Z_pt_after_->Fill(zCands_after->at(cand).pt());
	      h_Z_eta_after_->Fill(zCands_after->at(cand).eta());
	      h_Z_phi_after_->Fill(zCands_after->at(cand).phi());
	      h_Z_mass_after_->Fill(zCands_after->at(cand).mass());
	    }
	}


      // Superclusters

      //      sc_nSuperClusters_->Fill(superClustersEBCollectionSize+superClustersEECollectionSize);
      h_sc_nSuperClusters_->Fill(sc_nSuperClusters_);

      int scCount = 0;
      // barrel
      for(std::vector<reco::SuperCluster>::const_iterator i =
 	    superClustersEBCollection->begin(); i != superClustersEBCollection->end(); ++i)
//       for (unsigned int i = 0; i < superClustersEBCollection->size(); i++)
 	{
	  if (scCount >= k_SC_MAX)
	    {
	      LogWarning("") << "At barrel supercluster " << scCount << ", max allowed number is " << k_SC_MAX
			     << ", skipping rest of barrel superclusters";
	      break;
	    }

	  const reco::CaloClusterPtr sc_bcref = i->seed();
	  const reco::BasicCluster *sc_bc = sc_bcref.get();
	  
	  sc_energy_[scCount] = i->energy();
	  sc_eta_[scCount] = i->eta();
	  sc_phi_[scCount] = i->phi();
	  sc_rawEnergy_[scCount] = i->rawEnergy();
	  sc_preshowerEnergy_[scCount] = i->preshowerEnergy(); // SHOULD BE ZERO IN BARREL, HERE FOR COMPLETENESS
	  sc_etaWidth_[scCount] = i->etaWidth();
	  sc_phiWidth_[scCount] = i->phiWidth();
	  sc_nBasicClusters_[scCount] = i->clustersSize();
	  sc_r9_[scCount] = lazyTool.e3x3(*sc_bc)/sc_energy_[scCount]; // energy in 3x3 around seed cluster / SC energy
	  sc_eSeedOverESuperCluster_[scCount] = lazyTool.eMax(*sc_bc)/sc_energy_[scCount]; // energy of max crystal / SC energy

	  h_sc_energy_->Fill(sc_energy_[scCount]) ;
	  h_sc_eta_->Fill(sc_eta_[scCount]) ;
	  h_sc_phi_->Fill(sc_phi_[scCount]) ;
	  h_sc_rawEnergy_->Fill(sc_rawEnergy_[scCount]) ;
	  h_sc_etaWidth_->Fill(sc_etaWidth_[scCount]) ;
	  h_sc_phiWidth_->Fill(sc_phiWidth_[scCount]) ;
	  h_sc_nBasicClusters_->Fill(sc_nBasicClusters_[scCount]) ;
	  h_sc_r9_->Fill(sc_r9_[scCount]) ;
	  h_sc_eSeedOverESuperCluster_->Fill(sc_eSeedOverESuperCluster_[scCount]);

	  h_sc_energyBarrel_->Fill(sc_energy_[scCount]) ;
	  h_sc_phiBarrel_->Fill(sc_phi_[scCount]) ;
	  h_sc_rawEnergyBarrel_->Fill(sc_rawEnergy_[scCount]) ;
	  h_sc_etaWidthBarrel_->Fill(sc_etaWidth_[scCount]) ;
	  h_sc_phiWidthBarrel_->Fill(sc_phiWidth_[scCount]) ;
	  h_sc_nBasicClustersBarrel_->Fill(sc_nBasicClusters_[scCount]) ;
	  h_sc_r9Barrel_->Fill(sc_r9_[scCount]) ;
	  h_sc_eSeedOverESuperClusterBarrel_->Fill(sc_eSeedOverESuperCluster_[scCount]);

	  scCount++;
	}
       
      scCount = 0;
      // endcap
      for(std::vector<reco::SuperCluster>::const_iterator i =
	    superClustersEECollection->begin(); i != superClustersEECollection->end(); ++i)
//       for (unsigned int i = 0; i < superClustersEECollection->size(); i++)
	{
	  if (scCount >= k_SC_MAX)
	    {
	      LogWarning("") << "At endcap supercluster " << scCount << ", max allowed number is " << k_SC_MAX
			     << ", skipping rest of endcap superclusters";
	      break;
	    }

	  const reco::CaloClusterPtr sc_bcref = i->seed();
	  const reco::BasicCluster *sc_bc = sc_bcref.get();
	  
	  sc_energy_[scCount] = i->energy();
	  sc_eta_[scCount] = i->eta();
	  sc_phi_[scCount] = i->phi();
	  sc_rawEnergy_[scCount] = i->rawEnergy();
	  sc_preshowerEnergy_[scCount] = i->preshowerEnergy();
	  sc_etaWidth_[scCount] = i->etaWidth();
	  sc_phiWidth_[scCount] = i->phiWidth();
	  sc_nBasicClusters_[scCount] = i->clustersSize();
	  sc_r9_[scCount] = lazyTool.e3x3(*sc_bc)/ele_energy_[scCount]; // energy in 3x3 around seed cluster / SC energy
	  sc_eSeedOverESuperCluster_[scCount] = lazyTool.eMax(*sc_bc)/ele_energy_[scCount]; // energy of max crystal / SC energy

	  h_sc_energy_->Fill(sc_energy_[scCount]) ;
	  h_sc_eta_->Fill(sc_eta_[scCount]) ;
	  h_sc_phi_->Fill(sc_phi_[scCount]) ;
	  h_sc_rawEnergy_->Fill(sc_rawEnergy_[scCount]) ;
	  h_sc_etaWidth_->Fill(sc_etaWidth_[scCount]) ;
	  h_sc_phiWidth_->Fill(sc_phiWidth_[scCount]) ;
	  h_sc_nBasicClusters_->Fill(sc_nBasicClusters_[scCount]) ;
	  h_sc_r9_->Fill(sc_r9_[scCount]) ;
	  h_sc_eSeedOverESuperCluster_->Fill(sc_eSeedOverESuperCluster_[scCount]);
	  
	  h_sc_energyEndcap_->Fill(sc_energy_[scCount]) ;
	  h_sc_phiEndcap_->Fill(sc_phi_[scCount]) ;
	  h_sc_rawEnergyEndcap_->Fill(sc_rawEnergy_[scCount]) ;
	  h_sc_preshowerEnergyEndcap_->Fill(sc_preshowerEnergy_[scCount]) ;
	  h_sc_etaWidthEndcap_->Fill(sc_etaWidth_[scCount]) ;
	  h_sc_phiWidthEndcap_->Fill(sc_phiWidth_[scCount]) ;
	  h_sc_nBasicClustersEndcap_->Fill(sc_nBasicClusters_[scCount]) ;
	  h_sc_r9Endcap_->Fill(sc_r9_[scCount]) ;
	  h_sc_eSeedOverESuperClusterEndcap_->Fill(sc_eSeedOverESuperCluster_[scCount]);

	  scCount++;
	}


      // trigger printout for interesting events
      if (hasWCand_ || hasZCand_)
	{
	  for (std::map<std::string,bool>::iterator it = triggersOfInterest_->begin(); it != triggersOfInterest_->end(); it++)
	    {
	      if (it->second == true)
		{
		  std::cout << "CAND event, Trigger fired: " << it->first << std::endl;
		}
	    }
	}









      // Collect final flags
      if (rec_sel) nrec++;
      if (eid_sel) neid++;
      if (iso_sel) niso++;
      //      if (hlt_sel) nhlt++;
      //      if (met_sel) nmet++;

      if (all_sel) {
            nsel++;
            LogTrace("") << ">>>> Event ACCEPTED";
      } else {
            LogTrace("") << ">>>> Event REJECTED";
      }

      delete goodElecs;

      if (makeTree_)
	{
	  //std::cout << "GOTTEN ALL THE WAY TO TREE-FILLING" << std::endl;
	  tree_->Fill();
	  //std::cout << "DONE WITH TREE-FILLING" << std::endl;
	}

      return;
}



void
EwkElecStandaloneDQM::bookJetHistos( unsigned int type, std::map<std::string, TH1F*> & map) {
    std::string jetType;
    std::string collection;
    double minPt;
    std::string label;
    std::string hname;

    switch ( type ) {
        case PF:
            jetType = "Pf";
            collection = jetPfTag_.label();
            minPt = minJetPfPt_;
            break;
        case PF_CORR:
            jetType = "PfCorr";
            collection = jetPfCorrTag_.label();
            minPt = minJetPfCorrPt_;
            break;
        case CALO:
            jetType = "Calo";
            collection = jetCaloTag_.label();
            minPt = minJetCaloPt_;
            break;
        case CALO_CORR:
            jetType = "CaloCorr";
            collection = jetCaloCorrTag_.label();
            minPt = minJetCaloCorrPt_;
            break;
        case GEN:
            jetType = "Gen";
            collection = jetGenTag_.label();
            minPt = minJetGenPt_;
            break;
        default:
            LogWarning("") << ">>> Unknown jet collection, cannot book histograms! (got : " << type << ")";
            return;
    }

    std::string where[2] = { "_EtaBelow3", "_EtaOver3" };
    std::string when[2] =  { "before", "after" };
    std::string which[2] = { "highest", "all" };

    vector<PlotVariable> varsCommon;
    vector<PlotVariable> varsCalo;
    vector<PlotVariable> varsPf;
  
    int nbins = 60;//20;
      
    for ( unsigned int i = 0; i < 2; ++i ) {
        varsCommon.push_back( PlotVariable("Et" + where[i],     nbins,  0., 200.) );
        varsCommon.push_back( PlotVariable("Eta" + where[i],    nbins, -5.,   5.) );
        varsCommon.push_back( PlotVariable("Phi" + where[i],    nbins, -5.,   5.) );
        varsCommon.push_back( PlotVariable("Pt" + where[i],     nbins,  0., 200.) );
        varsCommon.push_back( PlotVariable("Energy" + where[i], nbins,  0., 200.) );

        varsCalo.push_back( PlotVariable("n60" + where[i],              20, -0.5,  19.5) );
        varsCalo.push_back( PlotVariable("n90" + where[i],              20, -0.5,  19.5) );
        varsCalo.push_back( PlotVariable("totalEMEnergy" + where[i]   , nbins,  0.,  200. ) );
        varsCalo.push_back( PlotVariable("totalHadEnergy" + where[i]  , nbins,  0.,  200. ) );
        varsCalo.push_back( PlotVariable("emEnergyFraction" + where[i], nbins,  0.,    1. ) );

        varsPf.push_back( PlotVariable("chargedHadronEnergy" + where[i], nbins,  0. , 200. ) );
        varsPf.push_back( PlotVariable("neutralHadronEnergy" + where[i], nbins,  0. , 200. ) );
        varsPf.push_back( PlotVariable("chargedEmEnergy" + where[i],     nbins,  0. , 200. ) );
        varsPf.push_back( PlotVariable("neutralEmEnergy" + where[i],     nbins,  0. , 200. ) );
        varsPf.push_back( PlotVariable("chargedMuEnergy" + where[i],     nbins,  0. , 200. ) );
        varsPf.push_back( PlotVariable("chargedHadronEnergyFraction" + where[i], nbins,  0.,    1. ) );
        varsPf.push_back( PlotVariable("neutralHadronEnergyFraction" + where[i], nbins,  0.,    1. ) );
        varsPf.push_back( PlotVariable("chargedEmEnergyFraction" + where[i],     nbins,  0.,    1. ) );
        varsPf.push_back( PlotVariable("neutralEmEnergyFraction" + where[i],     nbins,  0.,    1. ) );
        varsPf.push_back( PlotVariable("chargedMuEnergyFraction" + where[i],     nbins,  0.,    1. ) );
        varsPf.push_back( PlotVariable("chargedMultiplicity" + where[i],         20, -0.5,  19.5) );
        varsPf.push_back( PlotVariable("neutralMultiplicity" + where[i],         20, -0.5,  19.5) );
        varsPf.push_back( PlotVariable("muonMultiplicity" + where[i],            20, -0.5,  19.5) );
    }
    
    for ( int i = 0; i < 2; ++i ) {
        label = collection + " - number of Jets above " + dtos(minPt) + " GeV (" + ( i == 0 ? "before" : "after") + " electron cuts)" ;
        hname = "nJets_" + when[i];
        map[hname] = fs->make<TH1F>((boost::to_upper_copy("JET_"+jetType+"_"+hname)).data(), label.data(), 10, -0.5, 9.5);

        for ( int k = 0; k < 2; ++k ) {
            for ( std::vector<PlotVariable>::iterator itr = varsCommon.begin(); itr != varsCommon.end(); ++itr ) {
                label = which[k] + " " + collection + " - " + (*itr).name + " above " + dtos(minPt) + " GeV " + when[i] + " cuts";
                hname = (*itr).name + "_" + which[k] + "_" + when[i];
                map[hname] = fs->make<TH1F>((boost::to_upper_copy("JET_"+jetType+"_"+hname)).data(), label.data(), (*itr).nBins, (*itr).binLow, (*itr).binHigh);
            }

            if ( type == CALO || type == CALO_CORR) {
                for ( std::vector<PlotVariable>::iterator itr = varsCalo.begin(); itr != varsCalo.end(); ++itr ) {
                    label = which[k] + " " + collection + " - " + (*itr).name + " above " + dtos(minPt) + " GeV " + when[i] + " cuts";
                    hname = (*itr).name + "_" + which[k] + "_" + when[i];
                    map[hname] = fs->make<TH1F>((boost::to_upper_copy("JET_"+jetType+"_"+hname)).data(), label.data(), (*itr).nBins, (*itr).binLow, (*itr).binHigh);
                }
            }

            if ( type == PF ) {
                for ( std::vector<PlotVariable>::iterator itr = varsPf.begin(); itr != varsPf.end(); ++itr ) {
                    label = which[k] + " "+ collection + " - " + (*itr).name + " above " + dtos(minPt) + " GeV " + when[i] + " cuts";
                    hname = (*itr).name + "_" + which[k] + "_" + when[i];
                    map[hname] = fs->make<TH1F>((boost::to_upper_copy("JET_"+jetType+"_"+hname)).data(), label.data(), (*itr).nBins, (*itr).binLow, (*itr).binHigh);
                }
            }
        }
    }
}


void
EwkElecStandaloneDQM::bookMetHistos( unsigned int type, std::map<std::string, TH1F*> & map, std::map<std::string, TH1F*> & mapW) {
//         LogWarning("") << ">>> booking met histos";
    std::string metType;
    std::string collection;
    double minPt;
    std::string label;
    std::string hname;

    switch ( type ) {
        case PF:
            metType = "Pf";
            collection = metPfTag_.label();
            minPt = metMin_;
            break;
        case PF_CORR:
            metType = "PfCorr";
            collection = metPfCorrTag_.label();
            minPt = metMin_;
            break;
        case CALO:
            metType = "Calo";
            collection = metCaloTag_.label();
            minPt = metMin_;
            break;
        case CALO_CORR:
            metType = "CaloCorr";
            collection = metCaloCorrTag_.label();
            minPt = metMin_;
            break;
        case GEN:
            metType = "Gen";
            collection = metGenTag_.label();
            minPt = metMin_;
            break;
        case TC:
            metType = "Tc";
            collection = metTcTag_.label();
            minPt = metMin_;
            break;
        default:
            LogWarning("") << ">>> Unknown met collection, cannot book histograms! (got : " << type << ")";
            return;
    }

    std::string when[2] = { "before", "after" };
    int nbins = 60;//20;
  double caloMax = 350;
  
    vector<PlotVariable> varsCommon;
    varsCommon.push_back( PlotVariable("Et",     nbins,  0., 200.) );
    varsCommon.push_back( PlotVariable("Ex",    nbins, -50.,   50.) );
    varsCommon.push_back( PlotVariable("Ey",    nbins, -50.,   50.) );
    varsCommon.push_back( PlotVariable("sumEt",     nbins,  0., 500.) );
//     varsCommon.push_back( PlotVariable("Energy", nbins,  0., 200.) );
    varsCommon.push_back( PlotVariable("Phi", nbins,  -5., 5.) );
    varsCommon.push_back( PlotVariable("Sig", nbins,  0, 20.) );
    if(metType == "Pf" || metType == "PfCorr" || metType == "Gen") {
      varsCommon.push_back( PlotVariable("NeutralEmEtFrac", nbins,  0., 1.) );
      varsCommon.push_back( PlotVariable("NeutralHadEtFrac", nbins,  0., 1.) );
      varsCommon.push_back( PlotVariable("ChargedEmEtFrac", nbins,  0., 1.) );
      varsCommon.push_back( PlotVariable("ChargedHadEtFrac", nbins,  0., 1.) );
      varsCommon.push_back( PlotVariable("MuonEtFrac", nbins,  0., 1.) );
    }
    if(metType == "Gen") {
      varsCommon.push_back( PlotVariable("InvisibleEtFraction", nbins,  0., 1.) );
    }
    if(metType == "Calo" || metType == "CaloCorr") {
      varsCommon.push_back( PlotVariable("maxEtInEmTowers", nbins,  0., caloMax) );
      varsCommon.push_back( PlotVariable("maxEtInHadTowers", nbins,  0., caloMax) );
      varsCommon.push_back( PlotVariable("etFractionHadronic", nbins,  0., 1.) );
      varsCommon.push_back( PlotVariable("emEtFraction", nbins,  0., 1.) );
      varsCommon.push_back( PlotVariable("hadEtInHB", nbins,  0., caloMax) );
      varsCommon.push_back( PlotVariable("hadEtInHO", nbins,  0., caloMax) );
      varsCommon.push_back( PlotVariable("hadEtInHE", nbins,  0., caloMax) );
      varsCommon.push_back( PlotVariable("hadEtInHF", nbins,  0., caloMax) );
      varsCommon.push_back( PlotVariable("emEtInEB", nbins,  0., caloMax) );
      varsCommon.push_back( PlotVariable("emEtInEE", nbins,  0., caloMax) );
      varsCommon.push_back( PlotVariable("emEtInHF", nbins,  0., caloMax) );    
    }
    if(metType == "Tc") {
      ;
// meTcNeutralEMFraction  = _dbe->book1D("METTask_TcNeutralEMFraction", "METTask_TcNeutralEMFraction" ,50,0.,1.);
// meTcNeutralHadFraction = _dbe->book1D("METTask_TcNeutralHadFraction","METTask_TcNeutralHadFraction",50,0.,1.);
// meTcChargedEMFraction  = _dbe->book1D("METTask_TcChargedEMFraction", "METTask_TcChargedEMFraction" ,50,0.,1.);
// meTcChargedHadFraction = _dbe->book1D("METTask_TcChargedHadFraction","METTask_TcChargedHadFraction",50,0.,1.);
// meTcMuonFraction       = _dbe->book1D("METTask_TcMuonFraction",      "METTask_TcMuonFraction"      ,50,0.,1.);

// meTcMETIonFeedbck      = _dbe->book1D("METTask_TcMETIonFeedbck", "METTask_TcMETIonFeedbck" ,500,0,1000);
// meTcMETHPDNoise        = _dbe->book1D("METTask_TcMETHPDNoise",   "METTask_TcMETHPDNoise"   ,500,0,1000);
// meTcMETRBXNoise        = _dbe->book1D("METTask_TcMETRBXNoise",   "METTask_TcMETRBXNoise"   ,500,0,1000);
    }
    

    for ( int i = 0; i < 2; ++i ) {
      
      for ( std::vector<PlotVariable>::iterator itr = varsCommon.begin(); itr != varsCommon.end(); ++itr ) {
	label = (*itr).name + " (" + collection + ") "+when[i]+" cuts "+";" + (*itr).name;
	hname = (*itr).name + "_" + when[i];
	map[hname] = fs->make<TH1F>((boost::to_upper_copy("MET_"+metType+"_"+hname)).data(), label.data(), (*itr).nBins, (*itr).binLow, (*itr).binHigh);
      }
    }


    //Book W plots tooooooo

    vector<PlotVariable> varsCommonW;
    varsCommonW.push_back( PlotVariable("Mt",       nbins,   0., 200.) );
    varsCommonW.push_back( PlotVariable("Pt",       nbins,   0., 200.) );
    varsCommonW.push_back( PlotVariable("Phi",      nbins,  -5., 5.) );
    varsCommonW.push_back( PlotVariable("Rapidity", nbins,  -5., 5.) );


    for ( int i = 0; i < 2; ++i ) {
      
      for ( std::vector<PlotVariable>::iterator itr = varsCommonW.begin(); itr != varsCommonW.end(); ++itr ) {
	label = "W " + (*itr).name + " (" + collection + ") "+when[i]+" cuts "+";" + (*itr).name;
	hname = (*itr).name + "_" + when[i];
	mapW[hname] = fs->make<TH1F>((boost::to_upper_copy("W_"+metType+"MET_"+hname)).data(), label.data(), (*itr).nBins, (*itr).binLow, (*itr).binHigh);
// 	LogError("") << ">>> W plot made " << "W_" << metType << "MET_" << hname ;
      }       
    }

}




bool
EwkElecStandaloneDQM::getJetCollections(const Event & ev) {
    if (!ev.getByLabel(jetCaloTag_, jetCaloCollection_)) {
        LogError("") << ">>> CaloJet collection does not exist !!!";
        return false;
    }

    if (useCorrCaloJets_ && not ev.getByLabel(jetCaloCorrTag_, jetCaloCorrCollection_) ) {
        LogError("") << ">>> Corrected CaloJet collection does not exist !!!";
        useCorrCaloJets_ = false;
    }

    if (!ev.getByLabel(jetPfTag_, jetPfCollection_)) {
        LogError("") << ">>> PfJet collection does not exist !!!";
        return false;
    }

    if ( useCorrPfJets_ && not ev.getByLabel(jetPfCorrTag_, jetPfCorrCollection_)) {
        LogError("") << ">>> Corrected PfJet collection does not exist !!!";
        useCorrPfJets_ =  false;
    }

    if ( useGenJets_ && not ev.getByLabel(jetGenTag_, jetGenCollection_)) {
        LogWarning("") << ">>> GenJet collection does not exist, disabling use of GenJets";
        useGenJets_ = false;
    }

    return true;
}

bool
EwkElecStandaloneDQM::getMetCollections(const Event & ev) {
//         LogWarning("") << ">>>getting met collection ";
    if (!ev.getByLabel(metCaloTag_, metCaloCollection_)) {
        LogError("") << ">>> CaloJet collection does not exist !!!";
        return false;
    }

    if (useCorrCaloMet_ && not ev.getByLabel(metCaloCorrTag_, metCaloCorrCollection_) ) {
        LogError("") << ">>> Corrected CaloMet collection does not exist !!!";
        useCorrCaloMet_ = false;
    }

    if (!ev.getByLabel(metPfTag_, metPfCollection_)) {
        LogError("") << ">>> PfMet collection does not exist !!!";
        return false;
    }

    if ( useCorrPfMet_ && not ev.getByLabel(metPfCorrTag_, metPfCorrCollection_)) {
        LogError("") << ">>> Corrected PfMet collection does not exist !!!";
        useCorrPfMet_ =  false;
    }
  
    if (!ev.getByLabel(metTcTag_, metTcCollection_)) {
        LogError("") << ">>> TcMet collection does not exist !!!";
        return false;
    }

    if ( useGenMet_ && not ev.getByLabel(metGenTag_, metGenCollection_)) {
        LogWarning("") << ">>> GenMet collection does not exist, disabling use of GenMet";
        useGenMet_ = false;
    }

    return true;
}


int
EwkElecStandaloneDQM::fillCaloJets(unsigned int type, std::string when) {

    edm::Handle<reco::CaloJetCollection> jetCollection;
    std::map<std::string, TH1F*> * histoMap;
    double minPt;
    bool corrected = false;
    std::string collection;

    switch ( type ) {
        case CALO:
            jetCollection = jetCaloCollection_;
            histoMap = &caloJetHisto;
            collection = jetCaloTag_.label();
            minPt = minJetCaloPt_;
            break;
        case CALO_CORR:
            jetCollection = jetCaloCorrCollection_;
            histoMap = &caloCorrJetHisto;
            collection = jetCaloCorrTag_.label();
            minPt = minJetCaloCorrPt_;
            corrected = true;
            break;
        default:
            LogWarning("") << ">>> Wrong caloJet collection! (got : " << type << ")";
            return 0;
    }

    int nJets = 0;

    if ( jetCollection->empty() ) return 0;

    CaloJet *jet = jetCollection->begin()->clone();

    for ( CaloJetCollection::const_iterator itr = jetCollection->begin(); itr != jetCollection->end(); ++itr ) {
        if ( itr->pt() < minPt ) continue;
        bool isElectron = false;

        for ( GsfElectronCollection::const_iterator itrEl = electronCollection->begin(); itrEl != electronCollection->end(); ++itrEl ) {
            if ( deltaR(itr->eta(), itr->phi(), itrEl->eta(), itrEl->phi()) < minJetElectronDr_ ) {
                isElectron = true;
                break;
            }
        }

        if ( isElectron ) continue;

        fillCaloJetHistograms( histoMap, "all", when, &(*itr), corrected ); // not the same as (itr)!

        // keep highest Et jet
        if ( itr->et() > jet->et() )
            jet = itr->clone();

        ++nJets;
    }

    if ( nJets != 0 )
        fillCaloJetHistograms( histoMap, "highest", when, jet, corrected );

    LogTrace("") << ">>> Total number of caloJets (" << collection << "): " << jetCollection->size();
    LogTrace("") << ">>> Number of caloJets (" << collection << ") over " << minPt << " [GeV]: " << nJets;
    (*histoMap)["nJets_" + when]->Fill(nJets);

    return nJets;
}


int
EwkElecStandaloneDQM::fillPfJets(unsigned int type, std::string when) {

    edm::Handle<reco::PFJetCollection> jetCollection;
    std::map<std::string, TH1F*> * histoMap;
    std::string collection;
    bool corrected = false;
    double minPt;

    switch ( type ) {
        case PF:
            jetCollection = jetPfCollection_;
            histoMap = &pfJetHisto;
            collection = jetPfTag_.label();
            minPt = minJetPfPt_;
            break;
        case PF_CORR:
            jetCollection = jetPfCorrCollection_;
            histoMap = &pfCorrJetHisto;
            collection = jetPfCorrTag_.label();
            minPt = minJetPfCorrPt_;
            corrected = true;
            break;
        default:
            LogWarning("") << ">>> Wrong pfJet collection! (got : " << type << ")";
            return 0;
    }

    int nJets = 0;

    if ( jetCollection->empty() ) return 0;

    PFJet *jet = jetCollection->begin()->clone();

    for ( PFJetCollection::const_iterator itr = jetCollection->begin(); itr != jetCollection->end(); ++itr ) {
        if ( itr->pt() < minPt ) continue;
        bool isElectron = false;

        for ( GsfElectronCollection::const_iterator itrEl = electronCollection->begin(); itrEl != electronCollection->end(); ++itrEl ) {
            if ( deltaR(itr->eta(), itr->phi(), itrEl->eta(), itrEl->phi()) < minJetElectronDr_ ) {
                isElectron = true;
                break;
            }
        }

        if ( isElectron ) continue;

        fillPfJetHistograms( histoMap, "all", when, &(*itr), corrected ); // not the same as (itr)!

        // keep highest Et jet
        if ( itr->et() > jet->et() )
            jet = itr->clone();

        ++nJets;
    }

    if ( nJets != 0 )
        fillPfJetHistograms( histoMap, "highest", when, jet, corrected );

    LogTrace("") << ">>> Total number of pfJets (" << collection << "): " << jetCollection->size();
    LogTrace("") << ">>> Number of pfJets (" << collection << ") over " << minPt << " [GeV]: " << nJets;
    (*histoMap)["nJets_" + when]->Fill(nJets);

    return nJets;
}





int
EwkElecStandaloneDQM::fillGenJets(std::string when) {

    int nJets = 0;

    if ( jetGenCollection_->empty() ) return 0;

    GenJet *jet = jetGenCollection_->begin()->clone();

    for ( GenJetCollection::const_iterator itr = jetGenCollection_->begin(); itr != jetGenCollection_->end(); ++itr ) {
        if ( itr->et() < minJetPfPt_) continue;
        bool isElectron = false;

        for ( GsfElectronCollection::const_iterator itrEl = electronCollection->begin(); itrEl != electronCollection->end(); ++itrEl ) {
            if ( deltaR(itr->eta(), itr->phi(), itrEl->eta(), itrEl->phi()) < 0.5 ) {
                isElectron = true;
                break;
            }
        }

        if ( isElectron ) continue;

        fillGenJetHistograms( "all", when, &(*itr) ); // not the same as (itr)!

        // keep highest Et jet
        if ( itr->et() > jet->et() )
            jet = itr->clone();

        ++nJets;
    }

    if ( nJets != 0 )
        fillGenJetHistograms( "highest", when, jet);

    LogTrace("") << ">>> Total number of GenJets: " << jetGenCollection_->size();
    LogTrace("") << ">>> Number of GenJets over " << minJetGenPt_ << " [GeV]: " << nJets;
    genJetHisto["nJets_" + when]->Fill(nJets);

    return nJets;
}




void
EwkElecStandaloneDQM::fillCaloJetHistograms( std::map<std::string, TH1F*> * map, std::string which, std::string when, const reco::CaloJet *jet, bool corrected ) {

    std::string where = fabs(jet->eta()) >= 3 ? "_EtaOver3_" : "_EtaBelow3_"; 

    // common
    (*map)["Eta" + where + which + "_" + when]->Fill(jet->eta());
    (*map)["Et" + where + which + "_" + when]->Fill(jet->et());
    (*map)["Pt" + where + which + "_" + when]->Fill(jet->pt());
    (*map)["Energy" + where + which + "_" + when]->Fill(jet->energy());
    (*map)["Phi" + where + which + "_" + when]->Fill(jet->phi());
    // caloJet specific
    (*map)["n60" + where + which + "_" + when]->Fill(jet->n60());
    (*map)["n90" + where + which + "_" + when]->Fill(jet->n90());
    (*map)["emEnergyFraction" + where + which + "_" + when]->Fill(jet->emEnergyFraction());
    (*map)["totalEMEnergy" + where + which + "_" + when]->Fill(jet->emEnergyInEB() + jet->emEnergyInEE() + jet->emEnergyInHF());
    (*map)["totalHadEnergy" + where + which + "_" + when]->Fill(jet->hadEnergyInHB() + jet->hadEnergyInHE() + jet->hadEnergyInHF() + jet->hadEnergyInHO());
}



void
EwkElecStandaloneDQM::fillPfJetHistograms(std::map<std::string, TH1F*> * map, std::string which, std::string when, const reco::PFJet *jet, bool corrected ) {

    std::string where = fabs(jet->eta()) >= 3 ? "_EtaOver3_" : "_EtaBelow3_"; 

    // common
    (*map)["Eta" + where + which + "_" + when]->Fill(jet->eta());
    (*map)["Et" + where + which + "_" + when]->Fill(jet->et());
    (*map)["Pt" + where + which + "_" + when]->Fill(jet->pt());
    (*map)["Energy" + where + which + "_" + when]->Fill(jet->energy());
    (*map)["Phi" + where + which + "_" + when]->Fill(jet->phi());
    // pfJet specific
    if ( not corrected ) {
        (*map)["chargedHadronEnergy" + where + which + "_" + when]->Fill(jet->chargedHadronEnergy());
        (*map)["neutralHadronEnergy" + where + which + "_" + when]->Fill(jet->neutralHadronEnergy());
        (*map)["chargedEmEnergy" + where + which + "_" + when]->Fill(jet->chargedEmEnergy());
        (*map)["neutralEmEnergy" + where + which + "_" + when]->Fill(jet->neutralEmEnergy());
        (*map)["chargedMuEnergy" + where + which + "_" + when]->Fill(jet->chargedMuEnergy());
        (*map)["chargedHadronEnergyFraction" + where + which + "_" + when]->Fill(jet->chargedHadronEnergyFraction());
        (*map)["neutralHadronEnergyFraction" + where + which + "_" + when]->Fill(jet->neutralHadronEnergyFraction());
        (*map)["chargedEmEnergyFraction" + where + which + "_" + when]->Fill(jet->chargedEmEnergyFraction());
        (*map)["neutralEmEnergyFraction" + where + which + "_" + when]->Fill(jet->neutralEmEnergyFraction());
        (*map)["chargedMuEnergyFraction" + where + which + "_" + when]->Fill(jet->chargedMuEnergyFraction());
        (*map)["chargedMultiplicity" + where + which + "_" + when]->Fill(jet->chargedMultiplicity());
        (*map)["neutralMultiplicity" + where + which + "_" + when]->Fill(jet->neutralMultiplicity());
        (*map)["muonMultiplicity" + where + which + "_" + when]->Fill(jet->muonMultiplicity());
    }
}



void
EwkElecStandaloneDQM::fillGenJetHistograms(std::string which, std::string when, const reco::GenJet *jet) {

    std::string where = fabs(jet->eta()) >= 3 ? "_EtaOver3_" : "_EtaBelow3_"; 

    // common
    genJetHisto["Eta_" + which + "_" + when]->Fill(jet->eta());
    genJetHisto["Et" + where + which + "_" + when]->Fill(jet->et());
    genJetHisto["Pt" + where + which + "_" + when]->Fill(jet->pt());
    genJetHisto["Energy" + where + which + "_" + when]->Fill(jet->energy());
    genJetHisto["Phi" + where + which + "_" + when]->Fill(jet->phi());
}


void
EwkElecStandaloneDQM::fillCaloMetHistograms(  std::string when, bool corrected, math::XYZTLorentzVector v_e1) {

  //    LogWarning("") << ">>> fillCaloMetHisograms";
//     LogWarning("") << ">>> electron has pt " << v_e1.pt();
  std::map<std::string, TH1F*> * map;
  std::map<std::string, TH1F*> * mapW;
  edm::Handle< reco::CaloMETCollection > caloMEThandle;
  if( corrected ) {
    caloMEThandle = metCaloCorrCollection_;
    map = &caloCorrMetHisto;
    mapW = &wBosonCaloCorrMetHisto;
  }
  else {
    caloMEThandle = metCaloCollection_;
    map = &caloMetHisto;
    mapW = &wBosonCaloMetHisto;
  }
  const CaloMET& met = caloMEThandle->front();
  
    // common
  (*map)["Et_" + when]->Fill(met.et());
  (*map)["Ex_" + when]->Fill(met.px());
  (*map)["Ey_" + when]->Fill(met.py());
  (*map)["sumEt_" + when]->Fill(met.sumEt());
//   (*map)["Energy_" + when]->Fill(met.energy());
  (*map)["Phi_" + when]->Fill(met.phi());
  (*map)["Sig_" + when]->Fill(met.mEtSig());

  (*map)["maxEtInEmTowers_" + when]->Fill(met.maxEtInEmTowers());
  (*map)["maxEtInHadTowers_" + when]->Fill(met.maxEtInHadTowers());
  (*map)["etFractionHadronic_" + when]->Fill(met.etFractionHadronic());
  (*map)["emEtFraction_" + when]->Fill(met.emEtFraction());
  (*map)["hadEtInHB_" + when]->Fill(met.hadEtInHB());
  (*map)["hadEtInHO_" + when]->Fill(met.hadEtInHO());
  (*map)["hadEtInHE_" + when]->Fill(met.hadEtInHE());
  (*map)["hadEtInHF_" + when]->Fill(met.hadEtInHF());
  (*map)["emEtInEB_" + when]->Fill(met.emEtInEB());
  (*map)["emEtInEE_" + when]->Fill(met.emEtInEE());
  (*map)["emEtInHF_" + when]->Fill(met.emEtInHF());

  //    LogWarning("") << ">>> filled CaloMetHisograms";
  if(v_e1.pt() > 0) {
    math::XYZTLorentzVector v_w(0.,0.,0.,0.) ;
    math::XYZTLorentzVector v_me(met.px(), met.py(), 0., met.et()) ;
    v_w = v_e1 + v_me ;

    double w_et = met.et()+v_e1.pt();              // v_w.Et();//
    double w_px = met.px()+v_e1.px();	           // v_w.px();//
    double w_py = met.py()+v_e1.py();	           // v_w.py();//
    double w_pt = sqrt(pow(w_px,2)+pow(w_py,2));   // v_w.pt();//
    double massT = w_et*w_et - w_px*w_px - w_py*w_py;
    massT = (massT>0) ? sqrt(massT) : 0;
    (*mapW)["Mt_" + when]->Fill(massT);
    (*mapW)["Pt_" + when]->Fill(w_pt);
    (*mapW)["Phi_" + when]->Fill(v_w.phi());
    (*mapW)["Rapidity_" + when]->Fill(v_w.Rapidity());
  }
}

void
EwkElecStandaloneDQM::fillPfMetHistograms( std::string when, bool corrected, math::XYZTLorentzVector v_e1 ) {
  std::map<std::string, TH1F*> * map;
  std::map<std::string, TH1F*> * mapW;
  edm::Handle< reco::PFMETCollection > pfMEThandle;
  if( corrected ) {
    pfMEThandle = metPfCorrCollection_;
    map = &pfCorrMetHisto; 
    mapW = &wBosonPfCorrMetHisto;
  }
  else {
    pfMEThandle = metPfCollection_;
    map = &pfMetHisto;
    mapW = &wBosonPfMetHisto;
  }
  const PFMET& met = pfMEThandle->front();
  //  LogWarning("") << ">>> pf met histos";

  // common
  (*map)["Et_" + when]->Fill(met.et());
  (*map)["Ex_" + when]->Fill(met.px());
  (*map)["Ey_" + when]->Fill(met.py());
  (*map)["sumEt_" + when]->Fill(met.sumEt());
//   (*map)["Energy_" + when]->Fill(met.energy());
  (*map)["Phi_" + when]->Fill(met.phi());
  (*map)["Sig_" + when]->Fill(met.mEtSig());

  (*map)["NeutralEmEtFrac_" + when]->Fill(met.NeutralEMEtFraction());
  (*map)["NeutralHadEtFrac_" + when]->Fill(met.NeutralHadEtFraction());
  (*map)["ChargedEmEtFrac_" + when]->Fill(met.ChargedEMEtFraction());
  (*map)["ChargedHadEtFrac_" + when]->Fill(met.ChargedHadEtFraction());
  (*map)["MuonEtFrac_" + when]->Fill(met.MuonEtFraction());

  //  LogWarning("") << ">>> pf met histos filled";

  
  if(v_e1.pt() > 0) {
    math::XYZTLorentzVector v_w(0.,0.,0.,0.) ;
    math::XYZTLorentzVector v_me(met.px(), met.py(), 0., met.et()) ;
    v_w = v_e1 + v_me ;
    
    double w_et = met.et()+v_e1.pt();              // v_w.Et();//met.et()+v_e1.pt();
    double w_px = met.px()+v_e1.px();              // v_w.px();//met.px()+v_e1.px();
    double w_py = met.py()+v_e1.py();              // v_w.py();//met.py()+v_e1.py();
    double w_pt = sqrt(pow(w_px,2)+pow(w_py,2));   // v_w.pt();//sqrt(pow(w_px,2)+pow(w_py,2)); 
    double massT = w_et*w_et - w_px*w_px - w_py*w_py;
    massT = (massT>0) ? sqrt(massT) : 0;
    (*mapW)["Mt_" + when]->Fill(massT);
    (*mapW)["Pt_" + when]->Fill(w_pt);
    (*mapW)["Phi_" + when]->Fill(v_w.phi());
    (*mapW)["Rapidity_" + when]->Fill(v_w.Rapidity());
  }
  
  //  LogWarning("") << ">>> end pf met histos";

}

void
EwkElecStandaloneDQM::fillTcMetHistograms( std::string when, math::XYZTLorentzVector v_e1 ) {
  
  std::map<std::string, TH1F*> * map;
  std::map<std::string, TH1F*> * mapW;
  edm::Handle< reco::METCollection > tcMEThandle;
  tcMEThandle = metTcCollection_;
  map = &tcMetHisto;
  mapW = &wBosonTcMetHisto;
  const MET& met = tcMEThandle->front();
  
  // common
  (*map)["Et_" + when]->Fill(met.et());
  (*map)["Ex_" + when]->Fill(met.px());
  (*map)["Ey_" + when]->Fill(met.py());
  (*map)["sumEt_" + when]->Fill(met.sumEt());
//   (*map)["Energy_" + when]->Fill(met.energy());
  (*map)["Phi_" + when]->Fill(met.phi());
  (*map)["Sig_" + when]->Fill(met.mEtSig());
  
  
  if(v_e1.pt() > 0) {
    math::XYZTLorentzVector v_w(0.,0.,0.,0.) ;
    math::XYZTLorentzVector v_me(met.px(), met.py(), 0., met.et()) ;
    v_w = v_e1 + v_me ;
    
    double w_et = met.et()+v_e1.pt();              // v_w.Et();//met.et()+v_e1.pt();
    double w_px = met.px()+v_e1.px();              // v_w.px();//met.px()+v_e1.px();
    double w_py = met.py()+v_e1.py();              // v_w.py();//met.py()+v_e1.py();
    double w_pt = sqrt(pow(w_px,2)+pow(w_py,2));   // v_w.pt();//sqrt(pow(w_px,2)+pow(w_py,2)); 
    double massT = w_et*w_et - w_px*w_px - w_py*w_py;
    massT = (massT>0) ? sqrt(massT) : 0;
    (*mapW)["Mt_" + when]->Fill(massT);
    (*mapW)["Pt_" + when]->Fill(w_pt);
    (*mapW)["Phi_" + when]->Fill(v_w.phi());
    (*mapW)["Rapidity_" + when]->Fill(v_w.Rapidity());
  }
  
}

void
EwkElecStandaloneDQM::fillGenMetHistograms( std::string when, math::XYZTLorentzVector v_e1 ) {
  //  LogWarning("") << ">>> start gen met histos";  
  std::map<std::string, TH1F*> * map;
  std::map<std::string, TH1F*> * mapW;
  edm::Handle< reco::GenMETCollection > genMEThandle;
  genMEThandle = metGenCollection_;
  map = &genMetHisto;
  mapW = &wBosonGenMetHisto;
  const GenMET& met = genMEThandle->front();

  // common
  (*map)["Et_" + when]->Fill(met.et());
  (*map)["Ex_" + when]->Fill(met.px());
  (*map)["Ey_" + when]->Fill(met.py());
  (*map)["sumEt_" + when]->Fill(met.sumEt());
//   (*map)["Energy_" + when]->Fill(met.energy());
  (*map)["Phi_" + when]->Fill(met.phi());
  (*map)["Sig_" + when]->Fill(met.mEtSig());
  
  (*map)["NeutralEmEtFrac_" + when]->Fill(met.NeutralEMEtFraction());
  (*map)["NeutralHadEtFrac_" + when]->Fill(met.NeutralHadEtFraction());
  (*map)["ChargedEmEtFrac_" + when]->Fill(met.ChargedEMEtFraction());
  (*map)["ChargedHadEtFrac_" + when]->Fill(met.ChargedHadEtFraction());
  (*map)["MuonEtFrac_" + when]->Fill(met.MuonEtFraction());
  (*map)["InvisibleEtFraction_" + when]->Fill(met.InvisibleEtFraction());
  
  if(v_e1.pt() > 0) {
    math::XYZTLorentzVector v_w(0.,0.,0.,0.) ;
    math::XYZTLorentzVector v_me(met.px(), met.py(), 0., met.et()) ;
    v_w = v_e1 + v_me ;
    
    double w_et = met.et()+v_e1.pt();              // v_w.Et();//met.et()+v_e1.pt();
    double w_px = met.px()+v_e1.px();              // v_w.px();//met.px()+v_e1.px();
    double w_py = met.py()+v_e1.py();              // v_w.py();//met.py()+v_e1.py();
    double w_pt = sqrt(pow(w_px,2)+pow(w_py,2));   // v_w.pt();//sqrt(pow(w_px,2)+pow(w_py,2)); 
    double massT = w_et*w_et - w_px*w_px - w_py*w_py;
    massT = (massT>0) ? sqrt(massT) : 0;
    (*mapW)["Mt_" + when]->Fill(massT);
    (*mapW)["Pt_" + when]->Fill(w_pt);
    (*mapW)["Phi_" + when]->Fill(v_w.phi());
    (*mapW)["Rapidity_" + when]->Fill(v_w.Rapidity());
  }
  //  LogWarning("") << ">>> end gen met histos";  
}





