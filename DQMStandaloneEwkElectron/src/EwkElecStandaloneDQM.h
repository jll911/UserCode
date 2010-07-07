#ifndef EwkElecStandaloneDQM_H
#define EwkElecStandaloneDQM_H

/** \class EwkElecStandaloneDQM
 *
 *  Standalone DQM for EWK Electrons
 *
 */
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"

#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/JetReco/interface/GenJetCollection.h"
#include "DataFormats/JetReco/interface/PFJetCollection.h"


#include "DataFormats/METReco/interface/CaloMETCollection.h"
#include "DataFormats/METReco/interface/GenMETCollection.h"
#include "DataFormats/METReco/interface/PFMETCollection.h"
#include "DataFormats/METReco/interface/METCollection.h"


#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/ParameterSet/interface/InputTag.h" // 36X: #include "FWCore/Utilities/interface/InputTag.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "L1Trigger/GlobalTriggerAnalyzer/interface/L1GtUtils.h"

//class DQMStore;
//class MonitorElement;

//class TH1F;
//class TFile;
#include <map>
#include <sstream>
#include <string>

#include "TH1F.h"
#include "TH2F.h"
#include "TFile.h"
#include "TTree.h"

class EwkElecStandaloneDQM : public edm::EDAnalyzer {
public:
  EwkElecStandaloneDQM (const edm::ParameterSet &);
  ~EwkElecStandaloneDQM ();
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void beginJob();
  virtual void endJob();
  virtual void beginRun(const edm::Run&, const edm::EventSetup&);
  virtual void endRun(const edm::Run&, const edm::EventSetup&);

  void init_histograms();

private:
  edm::Service<TFileService> fs;
  L1GtUtils m_l1GtUtils;

  TTree * tree_;

  bool makeTree_;

  edm::InputTag trigTag_;
  edm::InputTag elecTag_;
  edm::InputTag superClustersEBTag_;
  edm::InputTag superClustersEETag_;
  edm::InputTag recHitsEBTag_;
  edm::InputTag recHitsEETag_;
  edm::InputTag metTag_;
  bool metIncludesMuons_;

  const std::vector<std::string> elecTrig_;

  double ptCut_;
  double etaCut_;

  double sieieCutBarrel_;
  double sieieCutEndcap_;
  double hoECutBarrel_;
  double hoECutEndcap_;
  double detainCutBarrel_;
  double detainCutEndcap_;
  double dphiinCutBarrel_;
  double dphiinCutEndcap_;

  //  bool isRelativeIso_;
  //  bool isCombinedIso_;

  //  double isoCut03_;
  double ecalIsoCutBarrel_;
  double ecalIsoCutEndcap_;
  double hcalIsoCutBarrel_;
  double hcalIsoCutEndcap_;
  double trkIsoCutBarrel_;
  double trkIsoCutEndcap_;
  double mtMin_;
  double mtMax_;
  double mZMin_;
  double mZMax_;

  /////////////////////////////////////////
  // Jets
  // Input Tags
  edm::InputTag jetCaloTag_;
  edm::InputTag jetCaloCorrTag_;
  edm::InputTag jetPfTag_;
  edm::InputTag jetPfCorrTag_;
  edm::InputTag jetGenTag_;
  // cuts
  double minJetCaloPt_;
  double minJetCaloCorrPt_;
  double minJetPfPt_;
  double minJetPfCorrPt_;
  double minJetGenPt_;
  bool useCorrCaloJets_;
  bool useCorrPfJets_;
  bool useGenJets_;
  double minJetElectronDr_;
  // Collections
  edm::Handle<reco::CaloJetCollection> jetCaloCollection_;
  edm::Handle<reco::CaloJetCollection> jetCaloCorrCollection_;
  edm::Handle<reco::PFJetCollection > jetPfCollection_;
  edm::Handle<reco::PFJetCollection > jetPfCorrCollection_;
  edm::Handle<reco::GenJetCollection> jetGenCollection_;
  // Histograms
  std::map<std::string, TH1F*> caloJetHisto;
  std::map<std::string, TH1F*> caloCorrJetHisto;
  std::map<std::string, TH1F*> pfJetHisto;
  std::map<std::string, TH1F*> pfCorrJetHisto;
  std::map<std::string, TH1F*> genJetHisto;
  // Methods
  void bookJetHistos( unsigned int type, std::map<std::string, TH1F*> & map );
  bool getJetCollections(const edm::Event & ev);
  bool getMetCollections(const edm::Event & ev);
  int fillCaloJets(unsigned int type, std::string when);
  int fillPfJets(unsigned int type, std::string when);
  int fillGenJets(std::string when);
  void fillCaloJetHistograms(std::map<std::string, TH1F*> * map, std::string which, std::string when, const reco::CaloJet *jet, bool corrected );
  void fillPfJetHistograms(std::map<std::string, TH1F*> * map, std::string which, std::string when, const reco::PFJet *jet, bool corrected );
  void fillGenJetHistograms(std::string which, std::string when, const reco::GenJet *jet);

  // Met Input Tags
  edm::InputTag metCaloTag_;
  edm::InputTag metCaloCorrTag_;
  edm::InputTag metPfTag_;
  edm::InputTag metPfCorrTag_;
  edm::InputTag metTcTag_;
  edm::InputTag metGenTag_;
  // cuts
  double metMin_;
  double metMax_;
  bool useCorrCaloMet_;
  bool useCorrPfMet_;
  bool useGenMet_;
  // Met Collections 
  edm::Handle<reco::CaloMETCollection> metCaloCollection_;
  edm::Handle<reco::CaloMETCollection> metCaloCorrCollection_;
  edm::Handle<reco::PFMETCollection  > metPfCollection_;
  edm::Handle<reco::PFMETCollection  > metPfCorrCollection_;
  edm::Handle<reco::METCollection    > metTcCollection_;
  edm::Handle<reco::GenMETCollection > metGenCollection_;
  // Met Histograms
  std::map<std::string, TH1F*> caloMetHisto;
  std::map<std::string, TH1F*> caloCorrMetHisto;
  std::map<std::string, TH1F*> pfMetHisto;
  std::map<std::string, TH1F*> pfCorrMetHisto;
  std::map<std::string, TH1F*> tcMetHisto;
  std::map<std::string, TH1F*> genMetHisto;

  std::map<std::string, TH1F*> wBosonCaloMetHisto;
  std::map<std::string, TH1F*> wBosonCaloCorrMetHisto;
  std::map<std::string, TH1F*> wBosonPfMetHisto;
  std::map<std::string, TH1F*> wBosonPfCorrMetHisto;
  std::map<std::string, TH1F*> wBosonTcMetHisto;
  std::map<std::string, TH1F*> wBosonGenMetHisto;
  // Met Methods
  void bookMetHistos( unsigned int type, std::map<std::string, TH1F*> & map, std::map<std::string, TH1F*> & mapW );
  void fillCaloMetHistograms( std::string when, bool corrected, math::XYZTLorentzVector e1 );
  void fillPfMetHistograms( std::string when, bool corrected, math::XYZTLorentzVector v_e1 );
  void fillTcMetHistograms( std::string when, math::XYZTLorentzVector v_e1 );
  void fillGenMetHistograms( std::string when, math::XYZTLorentzVector v_e1 );

  edm::InputTag pfCandTag_;
  edm::Handle<reco::PFCandidateCollection  > pfCandidateCollection_;
  std::map<std::string, TH1F*> pfCandHisto;
  void bookPfHistos(std::map<std::string, TH1F*> & map);
  void fillPfCandidateHistos(std::string type, std::string when);

  // enums
  enum { CALO, CALO_CORR, PF, PF_CORR, GEN, TC };
  // structs
  struct PlotVariable {
    std::string name;
    int nBins;
    double binLow;
    double binHigh;
    PlotVariable( std::string n, int nB, double bL, double bH):
        name(n), nBins(nB), binLow(bL), binHigh(bH) { }
  };
  /////////////////////////////////////////




  /////////////////////////////////////////
  // Electrons
  // Collection
  edm::Handle<reco::GsfElectronCollection> electronCollection;
  // Superclusters collections
  edm::Handle<std::vector<reco::SuperCluster> > superClustersEBCollection;
  edm::Handle<std::vector<reco::SuperCluster> > superClustersEECollection;
/*   // RecHits */
/*   edm::Handle< EcalRecHitCollection > recHitsEBCollection; */
/*   edm::Handle< EcalRecHitCollection > recHitsEECollection; */

  //  double dxyCut_;
  //  double normalizedChi2Cut_;
  //  int trackerHitsCut_;
  //  bool isAlsoTrackerMuon_;

  //  double ptThrForZ1_;
  //  double ptThrForZ2_;

  double eJetMin_;
  double nJetMax_;


  unsigned int nall;
  unsigned int nrec;
  unsigned int neid;
  unsigned int niso;
/*   unsigned int nhlt; */
/*   unsigned int nmet; */
  unsigned int nsel;

  //  unsigned int nRecoElectrons;
  unsigned int nGoodElectrons;


  // VARIABLES FOR STORING IN TREE (AND FILLING HISTOGRAMS)


  // event variables
  unsigned int runNumber_;
  unsigned int lumiSection_;
  unsigned int eventNumber_; 
  bool ele_trig_; // 1 PER EVENT
  // how to deal with the different triggers in the ntuple??
  /*   float ele_triggersFired_; */
  std::map<std::string,bool> * triggersOfInterest_;
  std::map<std::string,bool> * l1TriggersOfInterest_;
  static const int k_HLTRIG_MAX = 100;
  static const int k_L1TRIG_MAX = 100;
/*   std::string hlTriggersNames_[k_HLTRIG_MAX]; */
/*   std::string l1TriggersNames_[k_L1TRIG_MAX]; */
  char* hlTriggersNames_[k_HLTRIG_MAX]; // ?
  char* l1TriggersNames_[k_L1TRIG_MAX]; // ?
  bool hlTriggersResults_[k_HLTRIG_MAX];
  bool l1TriggersResults_[k_L1TRIG_MAX];

  bool hasWCand_;
  bool hasZCand_;


  // electron variables

  static const int k_ELEC_MAX = 20;
  static const int k_SC_MAX = 20;
  //  static const int k_ELEC_MAX = 20;
  //  static const int k_ELEC_MAX = 20;

  int ele_nelectrons_; // 1 PER EVENT
  bool ele_isEcalDriven_[k_ELEC_MAX];
  bool ele_isTrackerDriven_[k_ELEC_MAX];
  bool ele_isBarrel_[k_ELEC_MAX];
  bool ele_isEndcap_[k_ELEC_MAX];
  float ele_eta_[k_ELEC_MAX];
  float ele_phi_[k_ELEC_MAX];
  float ele_pt_[k_ELEC_MAX];
  float ele_energy_[k_ELEC_MAX];
  float ele_ecalenergy_[k_ELEC_MAX];
  float ele_ecalenergyerror_[k_ELEC_MAX];
  float ele_et_[k_ELEC_MAX];
  float ele_p_[k_ELEC_MAX];
  float ele_electronmomentumerror_[k_ELEC_MAX];
  float ele_px_[k_ELEC_MAX];
  float ele_py_[k_ELEC_MAX];
  float ele_pz_[k_ELEC_MAX];
  
  float ele_dxy_[k_ELEC_MAX];

  float ele_deltaetaeleclustertrackatcalo_[k_ELEC_MAX];
  float ele_deltaetaseedclustertrackatcalo_[k_ELEC_MAX];
  float ele_deltaetasuperclustertrackatvtx_[k_ELEC_MAX];
  
  float ele_deltaphieleclustertrackatcalo_[k_ELEC_MAX];
  float ele_deltaphiseedclustertrackatcalo_[k_ELEC_MAX];
  float ele_deltaphisuperclustertrackatvtx_[k_ELEC_MAX];

  float ele_trackpositionatvtxx_[k_ELEC_MAX];
  float ele_trackpositionatvtxy_[k_ELEC_MAX];
  float ele_trackpositionatvtxz_[k_ELEC_MAX];

  float ele_trackmomentumatvtxx_[k_ELEC_MAX];
  float ele_trackmomentumatvtxy_[k_ELEC_MAX];
  float ele_trackmomentumatvtxz_[k_ELEC_MAX];
  
  float ele_trackpositionatcalox_[k_ELEC_MAX];
  float ele_trackpositionatcaloy_[k_ELEC_MAX];
  float ele_trackpositionatcaloz_[k_ELEC_MAX];

  float ele_trackmomentumatcalox_[k_ELEC_MAX];
  float ele_trackmomentumatcaloy_[k_ELEC_MAX];
  float ele_trackmomentumatcaloz_[k_ELEC_MAX];
  
  float ele_eeleclusteroverpout_[k_ELEC_MAX];
  float ele_eseedclusteroverp_[k_ELEC_MAX];
  float ele_eseedclusteroverpout_[k_ELEC_MAX];
  float ele_esuperclusteroverp_[k_ELEC_MAX];

  float ele_dr03ecalrechitsumet_[k_ELEC_MAX];
  float ele_dr03hcaldepth1towersumet_[k_ELEC_MAX];
  float ele_dr03hcaldepth2towersumet_[k_ELEC_MAX];
  float ele_dr03hcaltowersumet_[k_ELEC_MAX];
  float ele_dr03tksumpt_[k_ELEC_MAX];
  
  float ele_dr04ecalrechitsumet_[k_ELEC_MAX];
  float ele_dr04hcaldepth1towersumet_[k_ELEC_MAX];
  float ele_dr04hcaldepth2towersumet_[k_ELEC_MAX];
  float ele_dr04hcaltowersumet_[k_ELEC_MAX];
  float ele_dr04tksumpt_[k_ELEC_MAX]; 
  
  float ele_e1x5_[k_ELEC_MAX];
  float ele_e2x5max_[k_ELEC_MAX];
  float ele_e5x5_[k_ELEC_MAX];
  float ele_r1x5_[k_ELEC_MAX];
  float ele_r2x5max_[k_ELEC_MAX];
  float ele_scpreshowerenergy_[k_ELEC_MAX];
  float ele_scetawidth_[k_ELEC_MAX];
  float ele_scphiwidth_[k_ELEC_MAX];
  float ele_scr9_[k_ELEC_MAX];
  float ele_sceseedoveresupercluster_[k_ELEC_MAX];
  float ele_rmax3x3_[k_ELEC_MAX];

  
  float ele_hcaldepth1overecal_[k_ELEC_MAX];
  float ele_hcaldepth2overecal_[k_ELEC_MAX];
  float ele_hcaloverecal_[k_ELEC_MAX];
  
  float ele_sigmaetaeta_[k_ELEC_MAX];
  float ele_sigmaietaieta_[k_ELEC_MAX];

  // bremstuff
  float ele_basicclusterssize_[k_ELEC_MAX];
  float ele_numberofbrems_[k_ELEC_MAX];
  float ele_fbrem_[k_ELEC_MAX];

  // charge stuff
  float ele_scpixcharge_[k_ELEC_MAX];
  float ele_gsfcharge_[k_ELEC_MAX];
  float ele_ctfcharge_[k_ELEC_MAX];
  float ele_isgsfscpixchargeconsistent_[k_ELEC_MAX];
  float ele_isgsfctfchargeconsistent_[k_ELEC_MAX];
  float ele_isgsfctfscpixchargeconsistent_[k_ELEC_MAX];
  float ele_charge_[k_ELEC_MAX];

  // vertex/beamspot stuff
  float ele_vertexChi2_[k_ELEC_MAX];
  float ele_vertexNdof_[k_ELEC_MAX];
  float ele_vertexNormalizedChi2_[k_ELEC_MAX];
  float ele_vx_[k_ELEC_MAX];
  float ele_vy_[k_ELEC_MAX];
  float ele_vz_[k_ELEC_MAX];
/*   float ele_xBS_; */
/*   float ele_yBS_; */
/*   float ele_zBS_; */



  // combinations of electrons
  float ele_invmass_; // N PER EVENT! how to deal with ??



  // met variables
  float massT_; //
  float met_et_; //



  // Z candidate stuff: all candidates
  //  int Z_nZCands_;
  float Z_pt_before_ ;  // how to do before and after??
  float Z_pt_after_ ;
  float Z_eta_before_ ;
  float Z_eta_after_ ;
  float Z_phi_before_ ;
  float Z_phi_after_ ;
  float Z_mass_before_ ;
  float Z_mass_after_ ;





  // Supercluster stuff
  unsigned int sc_nSuperClusters_;
  unsigned int sc_nSuperClustersBarrel_;
  unsigned int sc_nSuperClustersEndcap_;
  bool sc_isBarrel_[2*k_SC_MAX];
  bool sc_isEndcap_[2*k_SC_MAX];
  float sc_energy_[2*k_SC_MAX];
  float sc_eta_[2*k_SC_MAX];
  float sc_phi_[2*k_SC_MAX];
  float sc_rawEnergy_[2*k_SC_MAX];
  float sc_preshowerEnergy_[2*k_SC_MAX]; // endcap-only!
  float sc_etaWidth_[2*k_SC_MAX];
  float sc_phiWidth_[2*k_SC_MAX];
  int sc_nBasicClusters_[2*k_SC_MAX];
  float sc_r9_[2*k_SC_MAX];
  float sc_eSeedOverESuperCluster_[2*k_SC_MAX];





  // plots of ELECTRON variables to be cut on

  TH1F* h_ele_pt_before_;
  TH1F* h_ele_pt_after_;
  TH1F* h_ele_ptEcalDriven_before_;
  TH1F* h_ele_ptEcalDriven_after_;
  TH1F* h_ele_ptTrackerDriven_before_;
  TH1F* h_ele_ptTrackerDriven_after_;
  TH1F* h_ele_eta_before_;
  TH1F* h_ele_eta_after_;
  TH1F* h_ele_phi_before_;
  TH1F* h_ele_phi_after_;
  TH1F* h_ele_sieiebarrel_before_;
  TH1F* h_ele_sieiebarrel_after_;
  TH1F* h_ele_sieieendcap_before_;
  TH1F* h_ele_sieieendcap_after_;
  TH1F* h_ele_hoebarrel_before_;
  TH1F* h_ele_hoebarrel_after_;
  TH1F* h_ele_hoeendcap_before_;
  TH1F* h_ele_hoeendcap_after_;
  TH1F* h_ele_detainbarrel_before_;
  TH1F* h_ele_detainbarrel_after_;
  TH1F* h_ele_detainendcap_before_;
  TH1F* h_ele_detainendcap_after_;
  TH1F* h_ele_dphiinbarrel_before_;
  TH1F* h_ele_dphiinbarrel_after_;
  TH1F* h_ele_dphiinendcap_before_;
  TH1F* h_ele_dphiinendcap_after_;
  TH1F* h_ele_dxy_before_;
  TH1F* h_ele_dxy_after_;
/*   TH1F* nhits_before_; */
/*   TH1F* nhits_after_; */
  TH1F* h_ele_ecalisobarrel_before_;
  TH1F* h_ele_ecalisobarrel_after_;
  TH1F* h_ele_ecalisoendcap_before_;
  TH1F* h_ele_ecalisoendcap_after_;
  TH1F* h_ele_hcalisobarrel_before_;
  TH1F* h_ele_hcalisobarrel_after_;
  TH1F* h_ele_hcalisoendcap_before_;
  TH1F* h_ele_hcalisoendcap_after_;
  TH1F* h_ele_trkisobarrel_before_;
  TH1F* h_ele_trkisobarrel_after_;
  TH1F* h_ele_trkisoendcap_before_;
  TH1F* h_ele_trkisoendcap_after_;
  TH1F* h_ele_trig_before_;
  TH1F* h_ele_trig_after_;
  TH1F* h_ele_triggersFired_before_;
  TH1F* h_ele_triggersFired_after_;
  TH1F* h_ele_invmass_before_;
  TH1F* h_ele_invmass_after_;
  TH1F* h_ele_nelectrons_before_;
  TH1F* h_ele_nelectrons_after_;
  TH1F* h_mt_before_;
  TH1F* h_mt_after_;
  TH1F* h_met_before_;
  TH1F* h_met_after_;


  // trigger histograms

  //  std::map<std::string, TH1F*> trgHistos;

  // SPECIAL
  TH2F * h_trg_highestEtVsThreshold_;
  TH1F * h_trg_L1_SingleEG5_eff_notfire1_;
  TH1F * h_trg_L1_SingleEG5_eff_notfire2_;
  TH1F * h_trg_L1_SingleEG5_eff_barrel_;
  TH1F * h_trg_Denominator_eff_barrel_;
  TH1F * h_trg_L1_SingleEG5_eff_ecalDriven_;
  TH1F * h_trg_Denominator_eff_ecalDriven_;
  //  TH1F * h_trg_L1_SingleEG5_eff_goodEle_;
  TH1F * h_trg_L1_SingleEG5_eff_eta_;
  TH1F * h_trg_Denominator_eff_eta_;
  TH1F * h_trg_L1_SingleEG5_eff_eta_cutEt5_;
  TH1F * h_trg_Denominator_eff_eta_cutEt5_;
  TH1F * h_trg_L1_SingleEG5_eff_eta_notfire1_;
  TH1F * h_trg_L1_SingleEG5_eff_eta_notfire2_;
  TH2F * h_trg_L1_SingleEG5_eff_etaEt_;
  TH2F * h_trg_Denominator_eff_etaEt_;
  // SPECIAL

<<<<<<< EwkElecStandaloneDQM.h
  // n collections: electrons, SC's, (photons)
  // n eta divisions: both, barrel, endcap
  // n selections: minimal, full
  // n seeds: both (OR), ecaldriven (i.e. not tracker-driven), (tracker-driven-only)
  const int N_COLLS = 2; // can assign here??
  const int N_ETA = 3;
  const int N_SEL = 1;
  const int N_SEEDS = 1;

  bool highestObjectCollsFlag[];
  bool highestObjectEtaFlag[];
  bool highestObjectSelFlag[]; // but not same selection for electrons, SCs, etc.
  bool highestObjectSeedsFlag[]; // same -- do SC's have ecaldriven vs trackerdriven?

  // n triggers
  // HLT
  //  photon
  //    photon 30
  //    photon 20
  //    photon 15
  //    photon track iso 15
  //    photon loose ecal iso 15
  //    photon 10
  //  electron
  //    ele LW 20
  //    ele LW 15
  //    ele LW si strip 15
  //    ele LW 10
  //    ele LW eleid 10
  //  L1 passthrough
  //    single EG 8
  //    single EG 5
  // L1
  //    single EG 20
  //    single EG 15
  //    single iso EG 15
  //    single EG 12
  //    single iso EG 12
  //    single EG 10
  //    single iso EG 10
  //    single EG 8
  //    single iso EG 8
  //    single EG 5
  //    single iso EG 5
  //    single EG 2


  const int N_TRIGGERS = 14; // SUBJECT TO CHANGE!!!  Can make configurable or whatever??  But filling needs to be hard-coded, or something . . .

  // denominators
  TH1F * h_trg_Denominator_Et_eff_[N_COLLS][N_ETA][N_SEL][N_SEEDS];  

  //  TH1F * h_trg_Numerator_Et_eff_[N_COLLS][N_ETA][N_SEL][N_SEEDS][N_TRIGGERS];

  //  TH1F * h_trg_HLT_L1SingleEG5_eff_[N_COLLS][N_ETA][N_SEL][N_SEEDS];
  //  TH1F * h_trg_HLT_L1SingleEG8_eff_[N_COLLS][N_ETA][N_SEL][N_SEEDS];
  TH1F * h_trg_HLT_Ele10_LW_L1R_eff_[N_COLLS][N_ETA][N_SEL][N_SEEDS];
  //  TH1F * h_trg_HLT_Ele10_LW_EleId_L1R_eff_;
  TH1F * h_trg_HLT_Ele15_LW_L1R_eff_[N_COLLS][N_ETA][N_SEL][N_SEEDS];
  //  TH1F * h_trg_HLT_Ele15_SiStrip_L1R_eff_;
  TH1F * h_trg_HLT_Ele20_LW_L1R_eff_[N_COLLS][N_ETA][N_SEL][N_SEEDS];
  TH1F * h_trg_HLT_Photon10_L1R_eff_[N_COLLS][N_ETA][N_SEL][N_SEEDS];
  TH1F * h_trg_HLT_Photon15_L1R_eff_[N_COLLS][N_ETA][N_SEL][N_SEEDS];
  //  TH1F * h_trg_HLT_Photon15_TrackIso_L1R_eff_;
  //  TH1F * h_trg_HLT_Photon15_LooseEcalIso_L1R_eff_;
  TH1F * h_trg_HLT_Photon20_L1R_eff_[N_COLLS][N_ETA][N_SEL][N_SEEDS];
  TH1F * h_trg_HLT_Photon30_L1R_eff_[N_COLLS][N_ETA][N_SEL][N_SEEDS];

  //  TH1F * h_trg_L1_SingleIsoEG5_eff_;
  //  TH1F * h_trg_L1_SingleIsoEG8_eff_;
  //  TH1F * h_trg_L1_SingleIsoEG10_eff_;
  //  TH1F * h_trg_L1_SingleIsoEG12_eff_;
  //  TH1F * h_trg_L1_SingleIsoEG15_eff_;
  TH1F * h_trg_L1_SingleEG2_eff_[N_COLLS][N_ETA][N_SEL][N_SEEDS];
  TH1F * h_trg_L1_SingleEG5_eff_[N_COLLS][N_ETA][N_SEL][N_SEEDS];
  TH1F * h_trg_L1_SingleEG8_eff_[N_COLLS][N_ETA][N_SEL][N_SEEDS];
  TH1F * h_trg_L1_SingleEG10_eff_[N_COLLS][N_ETA][N_SEL][N_SEEDS];
  TH1F * h_trg_L1_SingleEG12_eff_[N_COLLS][N_ETA][N_SEL][N_SEEDS];
  TH1F * h_trg_L1_SingleEG15_eff_[N_COLLS][N_ETA][N_SEL][N_SEEDS];
  TH1F * h_trg_L1_SingleEG20_eff_[N_COLLS][N_ETA][N_SEL][N_SEEDS];
=======
  TH1F * h_trg_Denominator_eff_;  // the denominator
  TH1F * h_trg_Denominator_eff_goodEle_;

  TH1F * h_trg_HLT_L1SingleEG5_eff_;
  TH1F * h_trg_HLT_L1SingleEG5_eff_goodEle_;
  TH1F * h_trg_HLT_L1SingleEG8_eff_;
  TH1F * h_trg_HLT_L1SingleEG8_eff_goodEle_;
  TH1F * h_trg_HLT_Ele10_LW_L1R_eff_;
  TH1F * h_trg_HLT_Ele10_LW_L1R_eff_goodEle_;
  TH1F * h_trg_HLT_Ele10_LW_EleId_L1R_eff_;
  TH1F * h_trg_HLT_Ele10_LW_EleId_L1R_eff_goodEle_;
  TH1F * h_trg_HLT_Ele15_LW_L1R_eff_;
  TH1F * h_trg_HLT_Ele15_LW_L1R_eff_goodEle_;
  TH1F * h_trg_HLT_Ele15_SiStrip_L1R_eff_;
  TH1F * h_trg_HLT_Ele15_SiStrip_L1R_eff_goodEle_;
  TH1F * h_trg_HLT_Ele20_LW_L1R_eff_;
  TH1F * h_trg_HLT_Ele20_LW_L1R_eff_goodEle_;
  TH1F * h_trg_HLT_Photon10_L1R_eff_;
  TH1F * h_trg_HLT_Photon10_L1R_eff_goodEle_;
  TH1F * h_trg_HLT_Photon15_L1R_eff_;
  TH1F * h_trg_HLT_Photon15_L1R_eff_goodEle_;
  TH1F * h_trg_HLT_Photon15_TrackIso_L1R_eff_;
  TH1F * h_trg_HLT_Photon15_TrackIso_L1R_eff_goodEle_;
  TH1F * h_trg_HLT_Photon15_LooseEcalIso_L1R_eff_;
  TH1F * h_trg_HLT_Photon15_LooseEcalIso_L1R_eff_goodEle_;
  TH1F * h_trg_HLT_Photon20_L1R_eff_;
  TH1F * h_trg_HLT_Photon20_L1R_eff_goodEle_;
  TH1F * h_trg_HLT_Photon30_L1R_eff_;
  TH1F * h_trg_HLT_Photon30_L1R_eff_goodEle_;

  TH1F * h_trg_L1_SingleIsoEG5_eff_;
  TH1F * h_trg_L1_SingleIsoEG5_eff_goodEle_;
  TH1F * h_trg_L1_SingleIsoEG8_eff_;
  TH1F * h_trg_L1_SingleIsoEG8_eff_goodEle_;
  TH1F * h_trg_L1_SingleIsoEG10_eff_;
  TH1F * h_trg_L1_SingleIsoEG10_eff_goodEle_;
  TH1F * h_trg_L1_SingleIsoEG12_eff_;
  TH1F * h_trg_L1_SingleIsoEG12_eff_goodEle_;
  TH1F * h_trg_L1_SingleIsoEG15_eff_;
  TH1F * h_trg_L1_SingleIsoEG15_eff_goodEle_;
  TH1F * h_trg_L1_SingleEG2_eff_;
  TH1F * h_trg_L1_SingleEG2_eff_goodEle_;
  TH1F * h_trg_L1_SingleEG5_eff_;
  TH1F * h_trg_L1_SingleEG5_eff_goodEle_;
  TH1F * h_trg_L1_SingleEG8_eff_;
  TH1F * h_trg_L1_SingleEG8_eff_goodEle_;
  TH1F * h_trg_L1_SingleEG10_eff_;
  TH1F * h_trg_L1_SingleEG10_eff_goodEle_;
  TH1F * h_trg_L1_SingleEG12_eff_;
  TH1F * h_trg_L1_SingleEG12_eff_goodEle_;
  TH1F * h_trg_L1_SingleEG15_eff_;
  TH1F * h_trg_L1_SingleEG15_eff_goodEle_;
  TH1F * h_trg_L1_SingleEG20_eff_;
  TH1F * h_trg_L1_SingleEG20_eff_goodEle_;
>>>>>>> 1.22
  



  // (a lot of) other variables

  // FROM GSF ELECTRON

  TH1F * h_ele_ecalEnergy_before_ ;
  TH1F * h_ele_ecalEnergy_after_ ;
  TH1F * h_ele_ecalEnergyError_before_ ;        // how useful are these errors?
  TH1F * h_ele_ecalEnergyError_after_ ;
  
  TH1F * h_ele_deltaEtaEleClusterTrackAtCalo_before_ ;
  TH1F * h_ele_deltaEtaEleClusterTrackAtCalo_after_ ;
  TH1F * h_ele_deltaEtaSeedClusterTrackAtCalo_before_ ;
  TH1F * h_ele_deltaEtaSeedClusterTrackAtCalo_after_ ;
  TH1F * h_ele_deltaEtaSuperClusterTrackAtVtx_before_ ;
  TH1F * h_ele_deltaEtaSuperClusterTrackAtVtx_after_ ;
  
  TH1F * h_ele_deltaPhiEleClusterTrackAtCalo_before_ ;
  TH1F * h_ele_deltaPhiEleClusterTrackAtCalo_after_ ;
  TH1F * h_ele_deltaPhiSeedClusterTrackAtCalo_before_ ;
  TH1F * h_ele_deltaPhiSeedClusterTrackAtCalo_after_ ;
  TH1F * h_ele_deltaPhiSuperClusterTrackAtVtx_before_ ;
  TH1F * h_ele_deltaPhiSuperClusterTrackAtVtx_after_ ;

  TH1F * h_ele_trackPositionAtVtxX_before_ ;
  TH1F * h_ele_trackPositionAtVtxX_after_ ;
  TH1F * h_ele_trackPositionAtVtxY_before_ ;
  TH1F * h_ele_trackPositionAtVtxY_after_ ;
  TH1F * h_ele_trackPositionAtVtxZ_before_ ;
  TH1F * h_ele_trackPositionAtVtxZ_after_ ;

  TH1F * h_ele_trackMomentumAtVtxX_before_ ;
  TH1F * h_ele_trackMomentumAtVtxX_after_ ;
  TH1F * h_ele_trackMomentumAtVtxY_before_ ;
  TH1F * h_ele_trackMomentumAtVtxY_after_ ;
  TH1F * h_ele_trackMomentumAtVtxZ_before_ ;
  TH1F * h_ele_trackMomentumAtVtxZ_after_ ;
  
  TH1F * h_ele_trackPositionAtCaloX_before_ ;
  TH1F * h_ele_trackPositionAtCaloX_after_ ;
  TH1F * h_ele_trackPositionAtCaloY_before_ ;
  TH1F * h_ele_trackPositionAtCaloY_after_ ;
  TH1F * h_ele_trackPositionAtCaloZ_before_ ;
  TH1F * h_ele_trackPositionAtCaloZ_after_ ;

  TH1F * h_ele_trackMomentumAtCaloX_before_ ;
  TH1F * h_ele_trackMomentumAtCaloX_after_ ;
  TH1F * h_ele_trackMomentumAtCaloY_before_ ;
  TH1F * h_ele_trackMomentumAtCaloY_after_ ;
  TH1F * h_ele_trackMomentumAtCaloZ_before_ ;
  TH1F * h_ele_trackMomentumAtCaloZ_after_ ;
  
  TH1F * h_ele_eEleClusterOverPout_before_ ;
  TH1F * h_ele_eEleClusterOverPout_after_ ;
  TH1F * h_ele_eSeedClusterOverP_before_ ;
  TH1F * h_ele_eSeedClusterOverP_after_ ;
  TH1F * h_ele_eSeedClusterOverPout_before_ ;
  TH1F * h_ele_eSeedClusterOverPout_after_ ;
  TH1F * h_ele_eSuperClusterOverP_before_ ; 
  TH1F * h_ele_eSuperClusterOverP_after_ ; 

  TH1F * h_ele_electronMomentumError_before_ ;
  TH1F * h_ele_electronMomentumError_after_ ;

  TH1F * h_ele_dr03EcalRecHitSumEt_before_ ;
  TH1F * h_ele_dr03EcalRecHitSumEt_after_ ;
  TH1F * h_ele_dr03HcalDepth1TowerSumEt_before_ ;
  TH1F * h_ele_dr03HcalDepth1TowerSumEt_after_ ;
  TH1F * h_ele_dr03HcalDepth2TowerSumEt_before_ ;
  TH1F * h_ele_dr03HcalDepth2TowerSumEt_after_ ;
  TH1F * h_ele_dr03HcalTowerSumEt_before_ ;
  TH1F * h_ele_dr03HcalTowerSumEt_after_ ;
  TH1F * h_ele_dr03TkSumPt_before_ ;
  TH1F * h_ele_dr03TkSumPt_after_ ;
  
  TH1F * h_ele_dr03EcalRecHitSumEtBarrel_before_ ;
  TH1F * h_ele_dr03EcalRecHitSumEtBarrel_after_ ;
  TH1F * h_ele_dr03HcalDepth1TowerSumEtBarrel_before_ ;
  TH1F * h_ele_dr03HcalDepth1TowerSumEtBarrel_after_ ;
  TH1F * h_ele_dr03HcalDepth2TowerSumEtBarrel_before_ ;
  TH1F * h_ele_dr03HcalDepth2TowerSumEtBarrel_after_ ;
  TH1F * h_ele_dr03HcalTowerSumEtBarrel_before_ ;
  TH1F * h_ele_dr03HcalTowerSumEtBarrel_after_ ;
  TH1F * h_ele_dr03TkSumPtBarrel_before_ ;
  TH1F * h_ele_dr03TkSumPtBarrel_after_ ;
  
  TH1F * h_ele_dr03EcalRecHitSumEtEndcap_before_ ;
  TH1F * h_ele_dr03EcalRecHitSumEtEndcap_after_ ;
  TH1F * h_ele_dr03HcalDepth1TowerSumEtEndcap_before_ ;
  TH1F * h_ele_dr03HcalDepth1TowerSumEtEndcap_after_ ;
  TH1F * h_ele_dr03HcalDepth2TowerSumEtEndcap_before_ ;
  TH1F * h_ele_dr03HcalDepth2TowerSumEtEndcap_after_ ;
  TH1F * h_ele_dr03HcalTowerSumEtEndcap_before_ ;
  TH1F * h_ele_dr03HcalTowerSumEtEndcap_after_ ;
  TH1F * h_ele_dr03TkSumPtEndcap_before_ ;
  TH1F * h_ele_dr03TkSumPtEndcap_after_ ;
  
  TH1F * h_ele_dr04EcalRecHitSumEt_before_ ;
  TH1F * h_ele_dr04EcalRecHitSumEt_after_ ;
  TH1F * h_ele_dr04HcalDepth1TowerSumEt_before_ ;
  TH1F * h_ele_dr04HcalDepth1TowerSumEt_after_ ;
  TH1F * h_ele_dr04HcalDepth2TowerSumEt_before_ ;
  TH1F * h_ele_dr04HcalDepth2TowerSumEt_after_ ;
  TH1F * h_ele_dr04HcalTowerSumEt_before_ ;
  TH1F * h_ele_dr04HcalTowerSumEt_after_ ;
  TH1F * h_ele_dr04TkSumPt_before_ ; 
  TH1F * h_ele_dr04TkSumPt_after_ ; 
  
  TH1F * h_ele_dr04EcalRecHitSumEtBarrel_before_ ;
  TH1F * h_ele_dr04EcalRecHitSumEtBarrel_after_ ;
  TH1F * h_ele_dr04HcalDepth1TowerSumEtBarrel_before_ ;
  TH1F * h_ele_dr04HcalDepth1TowerSumEtBarrel_after_ ;
  TH1F * h_ele_dr04HcalDepth2TowerSumEtBarrel_before_ ;
  TH1F * h_ele_dr04HcalDepth2TowerSumEtBarrel_after_ ;
  TH1F * h_ele_dr04HcalTowerSumEtBarrel_before_ ;
  TH1F * h_ele_dr04HcalTowerSumEtBarrel_after_ ;
  TH1F * h_ele_dr04TkSumPtBarrel_before_ ; 
  TH1F * h_ele_dr04TkSumPtBarrel_after_ ; 
  
  TH1F * h_ele_dr04EcalRecHitSumEtEndcap_before_ ;
  TH1F * h_ele_dr04EcalRecHitSumEtEndcap_after_ ;
  TH1F * h_ele_dr04HcalDepth1TowerSumEtEndcap_before_ ;
  TH1F * h_ele_dr04HcalDepth1TowerSumEtEndcap_after_ ;
  TH1F * h_ele_dr04HcalDepth2TowerSumEtEndcap_before_ ;
  TH1F * h_ele_dr04HcalDepth2TowerSumEtEndcap_after_ ;
  TH1F * h_ele_dr04HcalTowerSumEtEndcap_before_ ;
  TH1F * h_ele_dr04HcalTowerSumEtEndcap_after_ ;
  TH1F * h_ele_dr04TkSumPtEndcap_before_ ; 
  TH1F * h_ele_dr04TkSumPtEndcap_after_ ; 
  
  TH1F * h_ele_e1x5_before_ ;
  TH1F * h_ele_e1x5_after_ ;
  TH1F * h_ele_e2x5Max_before_ ;
  TH1F * h_ele_e2x5Max_after_ ;
  TH1F * h_ele_e5x5_before_ ;
  TH1F * h_ele_e5x5_after_ ;
  TH1F * h_ele_r1x5_before_ ;
  TH1F * h_ele_r1x5_after_ ;
  TH1F * h_ele_r2x5Max_before_ ;
  TH1F * h_ele_r2x5Max_after_ ;
  TH1F * h_ele_scEtaWidth_before_ ;
  TH1F * h_ele_scEtaWidth_after_ ;
  TH1F * h_ele_scPhiWidth_before_ ;
  TH1F * h_ele_scPhiWidth_after_ ;
  TH1F * h_ele_scR9_before_ ;
  TH1F * h_ele_scR9_after_ ;
  TH1F * h_ele_scESeedOverESuperCluster_before_ ;
  TH1F * h_ele_scESeedOverESuperCluster_after_ ;
  TH1F * h_ele_rMax3x3_before_; 
  TH1F * h_ele_rMax3x3_after_; 

  TH1F * h_ele_e1x5Barrel_before_ ;
  TH1F * h_ele_e1x5Barrel_after_ ;
  TH1F * h_ele_e2x5MaxBarrel_before_ ;
  TH1F * h_ele_e2x5MaxBarrel_after_ ;
  TH1F * h_ele_e5x5Barrel_before_ ;
  TH1F * h_ele_e5x5Barrel_after_ ;
  TH1F * h_ele_r1x5Barrel_before_ ;
  TH1F * h_ele_r1x5Barrel_after_ ;
  TH1F * h_ele_r2x5MaxBarrel_before_ ;
  TH1F * h_ele_r2x5MaxBarrel_after_ ;
  TH1F * h_ele_scEtaWidthBarrel_before_ ;
  TH1F * h_ele_scEtaWidthBarrel_after_ ;
  TH1F * h_ele_scPhiWidthBarrel_before_ ;
  TH1F * h_ele_scPhiWidthBarrel_after_ ;
  TH1F * h_ele_scR9Barrel_before_ ;
  TH1F * h_ele_scR9Barrel_after_ ;
  TH1F * h_ele_scESeedOverESuperClusterBarrel_before_ ;
  TH1F * h_ele_scESeedOverESuperClusterBarrel_after_ ;
  TH1F * h_ele_rMax3x3Barrel_before_; 
  TH1F * h_ele_rMax3x3Barrel_after_; 
  
  TH1F * h_ele_e1x5Endcap_before_ ;
  TH1F * h_ele_e1x5Endcap_after_ ;
  TH1F * h_ele_e2x5MaxEndcap_before_ ;
  TH1F * h_ele_e2x5MaxEndcap_after_ ;
  TH1F * h_ele_e5x5Endcap_before_ ;
  TH1F * h_ele_e5x5Endcap_after_ ;
  TH1F * h_ele_r1x5Endcap_before_ ;
  TH1F * h_ele_r1x5Endcap_after_ ;
  TH1F * h_ele_r2x5MaxEndcap_before_ ;
  TH1F * h_ele_r2x5MaxEndcap_after_ ;
  TH1F * h_ele_scPreshowerEnergyEndcap_before_ ;
  TH1F * h_ele_scPreshowerEnergyEndcap_after_ ;
  TH1F * h_ele_scEtaWidthEndcap_before_ ;
  TH1F * h_ele_scEtaWidthEndcap_after_ ;
  TH1F * h_ele_scPhiWidthEndcap_before_ ;
  TH1F * h_ele_scPhiWidthEndcap_after_ ;
  TH1F * h_ele_scR9Endcap_before_ ;
  TH1F * h_ele_scR9Endcap_after_ ;
  TH1F * h_ele_scESeedOverESuperClusterEndcap_before_ ;
  TH1F * h_ele_scESeedOverESuperClusterEndcap_after_ ;
  TH1F * h_ele_rMax3x3Endcap_before_; 
  TH1F * h_ele_rMax3x3Endcap_after_; 

  
  TH1F * h_ele_hcalDepth1OverEcal_before_ ;
  TH1F * h_ele_hcalDepth1OverEcal_after_ ;
  TH1F * h_ele_hcalDepth2OverEcal_before_ ;
  TH1F * h_ele_hcalDepth2OverEcal_after_ ;
  TH1F * h_ele_hcalOverEcal_before_ ; 
  TH1F * h_ele_hcalOverEcal_after_ ; 
  
  TH1F * h_ele_hcalDepth1OverEcalBarrel_before_ ;
  TH1F * h_ele_hcalDepth1OverEcalBarrel_after_ ;
  TH1F * h_ele_hcalDepth2OverEcalBarrel_before_ ;
  TH1F * h_ele_hcalDepth2OverEcalBarrel_after_ ;
  TH1F * h_ele_hcalOverEcalBarrel_before_ ; 
  TH1F * h_ele_hcalOverEcalBarrel_after_ ; 
  
  TH1F * h_ele_hcalDepth1OverEcalEndcap_before_ ;
  TH1F * h_ele_hcalDepth1OverEcalEndcap_after_ ;
  TH1F * h_ele_hcalDepth2OverEcalEndcap_before_ ;
  TH1F * h_ele_hcalDepth2OverEcalEndcap_after_ ;
  TH1F * h_ele_hcalOverEcalEndcap_before_ ; 
  TH1F * h_ele_hcalOverEcalEndcap_after_ ; 
  
  TH1F * h_ele_sigmaEtaEta_before_ ;
  TH1F * h_ele_sigmaEtaEta_after_ ;
  TH1F * h_ele_sigmaIetaIeta_before_ ; 
  TH1F * h_ele_sigmaIetaIeta_after_ ; 

  TH1F * h_ele_sigmaEtaEtaBarrel_before_ ;
  TH1F * h_ele_sigmaEtaEtaBarrel_after_ ;
  TH1F * h_ele_sigmaIetaIetaBarrel_before_ ; 
  TH1F * h_ele_sigmaIetaIetaBarrel_after_ ; 

  TH1F * h_ele_sigmaEtaEtaEndcap_before_ ;
  TH1F * h_ele_sigmaEtaEtaEndcap_after_ ;
  TH1F * h_ele_sigmaIetaIetaEndcap_before_ ; 
  TH1F * h_ele_sigmaIetaIetaEndcap_after_ ; 

  TH1F * h_ele_basicClustersSize_before_ ; 
  TH1F * h_ele_basicClustersSize_after_ ; 
  TH1F * h_ele_numberOfBrems_before_ ; 
  TH1F * h_ele_numberOfBrems_after_ ; 
  TH1F * h_ele_fbrem_before_ ; 
  TH1F * h_ele_fbrem_after_ ; 
  TH1F * h_ele_scPixCharge_before_ ;
  TH1F * h_ele_scPixCharge_after_ ;
  TH1F * h_ele_gsfCharge_before_ ;
  TH1F * h_ele_gsfCharge_after_ ;
  TH1F * h_ele_ctfCharge_before_ ;
  TH1F * h_ele_ctfCharge_after_ ;
  TH1F * h_ele_gsfScPixChargeConsistent_before_ ;
  TH1F * h_ele_gsfScPixChargeConsistent_after_ ;
  TH1F * h_ele_gsfCtfChargeConsistent_before_ ;
  TH1F * h_ele_gsfCtfChargeConsistent_after_ ;
  TH1F * h_ele_gsfCtfScPixChargeConsistent_before_ ;
  TH1F * h_ele_gsfCtfScPixChargeConsistent_after_ ;



  // FROM CANDIDATE

  TH1F * h_ele_charge_before_ ;
  TH1F * h_ele_charge_after_ ;
  TH1F * h_ele_energy_before_ ;
  TH1F * h_ele_energy_after_ ;
  TH1F * h_ele_et_before_ ;
  TH1F * h_ele_et_after_ ;
  TH1F * h_ele_p_before_ ;
  TH1F * h_ele_p_after_ ;
  TH1F * h_ele_px_before_ ;
  TH1F * h_ele_px_after_ ;
  TH1F * h_ele_py_before_ ;
  TH1F * h_ele_py_after_ ;
  TH1F * h_ele_pz_before_ ;
  TH1F * h_ele_pz_after_ ;

  TH1F * h_ele_vertexChi2_before_ ;
  TH1F * h_ele_vertexChi2_after_ ;
  TH1F * h_ele_vertexNdof_before_ ;
  TH1F * h_ele_vertexNdof_after_ ;
  TH1F * h_ele_vertexNormalizedChi2_before_ ;
  TH1F * h_ele_vertexNormalizedChi2_after_ ;
/*   TH1F * h_ele_xBS_before_ ; */
/*   TH1F * h_ele_xBS_after_ ; */
/*   TH1F * h_ele_yBS_before_ ; */
/*   TH1F * h_ele_yBS_after_ ; */
/*   TH1F * h_ele_zBS_before_ ; */
/*   TH1F * h_ele_zBS_after_ ; */
  TH1F * h_ele_vx_before_ ;
  TH1F * h_ele_vx_after_ ;
  TH1F * h_ele_vy_before_ ;
  TH1F * h_ele_vy_after_ ;
  TH1F * h_ele_vz_before_ ;
  TH1F * h_ele_vz_after_ ;



  // Supercluster stuff
  // both barrel and endcap
  TH1F * h_sc_nSuperClusters_ ;
  TH1F * h_sc_energy_ ;
  TH1F * h_sc_eta_ ;
  TH1F * h_sc_phi_ ;
  TH1F * h_sc_rawEnergy_ ;
  TH1F * h_sc_preshowerEnergy_ ;
  TH1F * h_sc_etaWidth_ ;
  TH1F * h_sc_phiWidth_ ;
  TH1F * h_sc_nBasicClusters_ ;
  TH1F * h_sc_r9_ ;
  TH1F * h_sc_eSeedOverESuperCluster_ ;

  // barrel
  TH1F * h_sc_energyBarrel_ ;
  TH1F * h_sc_phiBarrel_ ;
  TH1F * h_sc_rawEnergyBarrel_ ;
  TH1F * h_sc_preshowerEnergyBarrel_ ;
  TH1F * h_sc_etaWidthBarrel_ ;
  TH1F * h_sc_phiWidthBarrel_ ;
  TH1F * h_sc_nBasicClustersBarrel_ ;
  TH1F * h_sc_r9Barrel_ ;
  TH1F * h_sc_eSeedOverESuperClusterBarrel_ ;

  // endcap
  TH1F * h_sc_energyEndcap_ ;
  TH1F * h_sc_phiEndcap_ ;
  TH1F * h_sc_rawEnergyEndcap_ ;
  TH1F * h_sc_preshowerEnergyEndcap_ ;
  TH1F * h_sc_etaWidthEndcap_ ;
  TH1F * h_sc_phiWidthEndcap_ ;
  TH1F * h_sc_nBasicClustersEndcap_ ;
  TH1F * h_sc_r9Endcap_ ;
  TH1F * h_sc_eSeedOverESuperClusterEndcap_ ;



  // Z candidate stuff: all candidates
  TH1F * h_Z_pt_before_ ;
  TH1F * h_Z_pt_after_ ;
  TH1F * h_Z_eta_before_ ;
  TH1F * h_Z_eta_after_ ;
  TH1F * h_Z_phi_before_ ;
  TH1F * h_Z_phi_after_ ;
  TH1F * h_Z_mass_before_ ;
  TH1F * h_Z_mass_after_ ;

  // Z candidate stuff: top candidate 
/*   TH1F * Z_pt_top_before_ ; */
/*   TH1F * Z_pt_top_after_ ; */
/*   TH1F * Z_eta_top_before_ ; */
/*   TH1F * Z_eta_top_after_ ; */
/*   TH1F * Z_phi_top_before_ ; */
/*   TH1F * Z_phi_top_after_ ; */
/*   TH1F * Z_mass_top_before_ ; */
/*   TH1F * Z_mass_top_after_ ; */

  // don't really need beam spot position, right? this is electron DQM

  /////////////////////////////////////////




  std::string dtos(double d){std::stringstream s;    s << d;return s.str();}
  
};


#endif
