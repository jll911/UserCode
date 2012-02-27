// -*- C++ -*-
//
// Package:    PhotonsFromWenu
// Class:      PhotonsFromWenu
// 
/**\class PhotonsFromWenu PhotonsFromWenu.cc PhotonJetAnalysis/PhotonsFromWenu/src/PhotonsFromWenu.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Mike Anderson
//         Created:  Mon Aug  2 10:48:45 CDT 2010
// $Id: PhotonsFromWenu.cc,v 1.18 2011/08/01 16:48:23 anderson Exp $
//
//


// Standard
#include <memory>
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

// Physics Objects
#include "DataFormats/EgammaCandidates/interface/Photon.h"
#include "DataFormats/EgammaCandidates/interface/PhotonFwd.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
#include "DataFormats/JetReco/interface/CaloJet.h"

// Detector related
#include "DataFormats/CaloRecHit/interface/CaloCluster.h"
#include "RecoEcal/EgammaCoreTools/interface/EcalClusterLazyTools.h"
#include "DataFormats/DetId/interface/DetId.h"

// Jet extra stuff
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"

// Tracks
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

// Missing Transverse Energy
#include "DataFormats/METReco/interface/PFMETCollection.h"
#include "DataFormats/METReco/interface/METCollection.h"
#include "DataFormats/METReco/interface/MET.h"
#include "DataFormats/METReco/interface/CaloMET.h"
#include "DataFormats/METReco/interface/CaloMETCollection.h"
#include "DataFormats/METReco/interface/CaloMETFwd.h" 
#include "DataFormats/METReco/interface/PFMET.h"
#include "DataFormats/METReco/interface/PFMETFwd.h"

// Vertex
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

// Generator
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"

// Math stuff                                                                                                             
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/Math/interface/deltaPhi.h"

// Trigger
#include "DataFormats/Common/interface/TriggerResults.h"
#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"

// TFileService, for storing histograms
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "TH1F.h"
#include "TH2F.h"
#include "TTree.h"
#include "math.h"

class PhotonsFromWenu : public edm::EDAnalyzer {
public:
  explicit PhotonsFromWenu(const edm::ParameterSet&);
  ~PhotonsFromWenu();


private:
  virtual void beginRun(const edm::Run&, const edm::EventSetup&) ;
  virtual void beginJob() ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;

  // ----------member data ---------------------------
  edm::InputTag cfg_electronCollection;
  edm::InputTag cfg_photonCollection;
  edm::InputTag cfg_jetCollection;
  edm::InputTag cfg_pfjetCollection;
  edm::InputTag cfg_trackCollection;
  edm::InputTag cfg_genjetCollection;
  edm::InputTag cfg_metCollection;
  double cfg_minTransMass;
  double cfg_minMetEt;
  double cfg_minJetPt;
  double cfg_maxJetAbsEta;
  bool   cfg_requireElecPassCuts;
  bool   cfg_usePhotonMatchedToElec;
  double cfg_maxDeltaR;
  double cfg_minDeltaRPhotonJet;
  edm::InputTag cfg_hltResultsCollection;
  std::vector<std::string> cfg_HltPaths;
  edm::InputTag cfg_vertexCollection;

  HLTConfigProvider hltConfig_;

  // Tree
  TTree* tree;
  float electron_et;
  float electron_pt;
  float electron_sieie;
  float electron_hOverE;
  float electron_deltaPhi;
  float electron_deltaEta;
  float electron_TkSumPt;
  float electron_EcalIso;
  float electron_HcalIso;
  int   electron_classification;
  float e_p_deltaR;
  float e_met_deltaPhi;
  float photon_pt;
  float photon_eta;
  float photon_phi;
  float photon_hOverE;
  float photon_sieie;
  float photon_sipip;
  float photon_seedTime;
  bool  photon_hasPixelSeed;
  float photon_ecalIso;
  float photon_hcalIso;
  float photon_trackIso;
  float photon_vertex_x;
  float photon_vertex_y;
  float photon_vertex_z;
  float jet_pt;
  float jet_eta;
  float jet_phi;
  float jet2_pt;
  float jet2_eta;
  float jet2_phi;
  float jet3_pt;
  float jet3_eta;
  float jet3_phi;
  float jet4_pt;
  float jet4_eta;
  float jet4_phi;
  int jet_num;
  float jet_sumAllEt;
  float pfjet_pt;
  float pfjet_eta;
  float pfjet_phi;
  float pfjet2_pt;
  float pfjet2_eta;
  float pfjet2_phi;
  float pfjet3_pt;
  float pfjet3_eta;
  float pfjet3_phi;
  float pfjet4_pt;
  float pfjet4_eta;
  float pfjet4_phi;
  int pfjet_num;
  int pfjet_num_pt20;
  int pfjet_num_pt30;
  int pfjet_num_pt20hi;
  int pfjet_num_pt20lo;
  int pfjet_num_pt30hi;
  int pfjet_num_pt30lo;
  float genphoton_pt;
  float genphoton_eta;
  float genphoton_phi;
  float genjet_pt;
  float genjet2_pt;
  float genjet3_pt;
  float genjet4_pt;
  int genjet_num;
  int genjet_num_pt20;
  int genjet_num_pt30;
  float p_met_massT;
  float e_met_massT;
  float met_et;
  float met_sumEt;
  float gen_weight;
  float gen_pthat;
  double fastjet_rho;
  int vertex_num;

  // Histograms
  TH1F* h_electron_pt;
  TH1F* h_electron_eta;
  TH1F* h_electron_phi;
  TH1F* h_e_p_deltaR;
  TH1F* h_e_p_deltaPhi;
  TH1F* h_e_p_deltaEta;
  TH1F* h_e_p_deltaPt;
  TH1F* h_photon_pt;
  TH1F* h_photon_eta;
  TH1F* h_photon_phi;
  TH1F* h_photon_sieie;
  TH1F* h_photon_HoverE;
  TH1F* h_photon_ecalIso;
  TH1F* h_photon_hcalIso;
  TH1F* h_photon_trackIso;
  // Photon pt but with one cut removed
  TH1F* h_photon_pt_sieie;
  TH1F* h_photon_pt_HoverE;
  TH1F* h_photon_pt_ecalIso;
  TH1F* h_photon_pt_hcalIso;
  TH1F* h_photon_pt_trackIso;
  TH1F *h_photon_eta_sieie;
  TH1F *h_photon_eta_HoverE;
  TH1F *h_photon_eta_ecalIso;
  TH1F *h_photon_eta_hcalIso;
  TH1F *h_photon_eta_trackIso;

  TH1F* h_met_et;
  TH1F* h_p_met_massT;
  TH1F* h_e_met_massT;
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
PhotonsFromWenu::PhotonsFromWenu(const edm::ParameterSet& iConfig) {
  //cfg_HltProcessName     = iConfig.getParameter<string>("HltProcessName");
  cfg_electronCollection  = iConfig.getParameter<edm::InputTag>("electronCollection");
  cfg_photonCollection    = iConfig.getParameter<edm::InputTag>("photonCollection");
  cfg_jetCollection       = iConfig.getParameter<edm::InputTag>("jetCollection");
  cfg_pfjetCollection     = iConfig.getParameter<edm::InputTag>("pfjetCollection");
  cfg_trackCollection     = iConfig.getParameter<edm::InputTag>("trackCollection");
  cfg_genjetCollection    = iConfig.getParameter<edm::InputTag>("genjetCollection");
  cfg_metCollection       = iConfig.getParameter<edm::InputTag>("metCollection");
  cfg_vertexCollection    = iConfig.getParameter<edm::InputTag>("vertexCollection");
  cfg_minTransMass        = iConfig.getParameter<double>("minTransMass");
  cfg_minMetEt            = iConfig.getParameter<double>("minMetEt");
  cfg_minJetPt            = iConfig.getParameter<double>("minJetPt");
  cfg_maxJetAbsEta        = iConfig.getParameter<double>("maxJetAbsEta");
  cfg_requireElecPassCuts = iConfig.getParameter<bool>("requireElecPassCuts");
  cfg_usePhotonMatchedToElec = iConfig.getParameter<bool>("usePhotonMatchedToElec");
  cfg_maxDeltaR           = iConfig.getParameter<double>("maxDeltaR");
  cfg_minDeltaRPhotonJet  = iConfig.getParameter<double>("minDeltaRPhotonJet");
  cfg_hltResultsCollection= iConfig.getParameter<edm::InputTag>("hltResultsCollection");
  cfg_HltPaths            = iConfig.getParameter<std::vector<std::string> >("HltPaths");
}


PhotonsFromWenu::~PhotonsFromWenu() {
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
}

void PhotonsFromWenu::beginRun(const edm::Run & iRun, const edm::EventSetup & iSetup) {
  bool changed(true);
  if (hltConfig_.init(iRun, iSetup, "HLT", changed)) {
    // if init returns TRUE, initialisation has succeeded!
    if (changed) {
      // The HLT config has actually changed wrt the previous Run, hence rebook your
      // histograms or do anything else dependent on the revised HLT config
    }
  } else {
    // if init returns FALSE, initialisation has NOT succeeded, which indicates a problem
    // with the file and/or code and needs to be investigated!
    std::cout << " HLT config extraction failure with process name " << "HLT" << std::endl;
    //LogError("MyAnalyzer") << " HLT config extraction failure with process name " << processName_;
    // In this case, all access methods will return empty values!
  }
}


// ------------ method called once each job just before starting event loop  ------------
void PhotonsFromWenu::beginJob() {
  edm::Service<TFileService> fs;

  tree = fs->make<TTree>("tree","Wenu events - Electron with matched photon");
  tree->Branch("electron_et"      , &electron_et,    "electron_et/F");
  tree->Branch("electron_pt"      , &electron_pt,    "electron_pt/F");
  tree->Branch("electron_sieie"   , &electron_sieie, "electron_sieie/F");
  tree->Branch("electron_hOverE"  , &electron_hOverE,"electron_hOverE/F");
  tree->Branch("electron_deltaPhi", &electron_deltaPhi,"electron_deltaPhi/F");
  tree->Branch("electron_deltaEta", &electron_deltaEta,"electron_deltaEta/F");
  tree->Branch("electron_TkSumPt" , &electron_TkSumPt,"electron_TkSumPt/F");
  tree->Branch("electron_EcalIso" , &electron_EcalIso,"electron_EcalIso/F");
  tree->Branch("electron_HcalIso" , &electron_HcalIso,"electron_HcalIso/F");
  tree->Branch("electron_classification", &electron_classification,"electron_classification/I");
  tree->Branch("e_p_deltaR"    , &e_p_deltaR, "e_p_deltaR/F");
  tree->Branch("e_met_deltaPhi", &e_met_deltaPhi, "e_met_deltaPhi/F");
  tree->Branch("photon_pt"     , &photon_pt,      "photon_pt/F");
  tree->Branch("photon_eta"    , &photon_eta,     "photon_eta/F");
  tree->Branch("photon_phi"    , &photon_phi,     "photon_phi/F");
  tree->Branch("photon_hOverE" , &photon_hOverE,  "photon_hOverE/F");
  tree->Branch("photon_sieie"  , &photon_sieie,   "photon_sieie/F");
  tree->Branch("photon_sipip"  , &photon_sipip,   "photon_sipip/F");
  tree->Branch("photon_seedTime", &photon_seedTime,"photon_seedTime/F");
  tree->Branch("photon_hasPixelSeed", &photon_hasPixelSeed,"photon_hasPixelSeed/O");
  tree->Branch("photon_ecalIso", &photon_ecalIso, "photon_ecalIso/F");
  tree->Branch("photon_hcalIso", &photon_hcalIso, "photon_hcalIso/F");
  tree->Branch("photon_trackIso",&photon_trackIso,"photon_trackIso/F");
  tree->Branch("jet_pt"        , &jet_pt,  "jet_pt/F");
  tree->Branch("jet_eta"       , &jet_eta, "jet_eta/F");
  tree->Branch("jet_phi"       , &jet_phi, "jet_phi/F");
  tree->Branch("jet2_pt"       , &jet2_pt, "jet2_pt/F");
  tree->Branch("jet2_eta"      , &jet2_eta,"jet2_eta/F");
  tree->Branch("jet2_phi"      , &jet2_phi,"jet2_phi/F");
  tree->Branch("jet3_pt"       , &jet3_pt, "jet3_pt/F");
  tree->Branch("jet3_eta"      , &jet3_eta,"jet3_eta/F");
  tree->Branch("jet3_phi"      , &jet3_phi,"jet3_phi/F");
  tree->Branch("jet4_pt"       , &jet4_pt, "jet4_pt/F");
  tree->Branch("jet4_eta"      , &jet4_eta,"jet4_eta/F");
  tree->Branch("jet4_phi"      , &jet4_phi,"jet4_phi/F");
  tree->Branch("jet_num"       , &jet_num ,"jet_num/I");
  tree->Branch("jet_sumAllEt"  , &jet_sumAllEt, "jet_sumAllEt/F");
  tree->Branch("pfjet_pt"      , &pfjet_pt,  "pfjet_pt/F");
  tree->Branch("pfjet_eta"     , &pfjet_eta, "pfjet_eta/F");
  tree->Branch("pfjet_phi"     , &pfjet_phi, "pfjet_phi/F");
  tree->Branch("pfjet2_pt"     , &pfjet2_pt, "pfjet2_pt/F");
  tree->Branch("pfjet2_eta"    , &pfjet2_eta,"pfjet2_eta/F");
  tree->Branch("pfjet2_phi"    , &pfjet2_phi,"pfjet2_phi/F");
  tree->Branch("pfjet3_pt"     , &pfjet3_pt, "pfjet3_pt/F");
  tree->Branch("pfjet3_eta"    , &pfjet3_eta,"pfjet3_eta/F");
  tree->Branch("pfjet3_phi"    , &pfjet3_phi,"pfjet3_phi/F");
  tree->Branch("pfjet4_pt"     , &pfjet4_pt, "pfjet4_pt/F");
  tree->Branch("pfjet4_eta"    , &pfjet4_eta,"pfjet4_eta/F");
  tree->Branch("pfjet4_phi"    , &pfjet4_phi,"pfjet4_phi/F");
  tree->Branch("pfjet_num"     , &pfjet_num ,"pfjet_num/I");
  tree->Branch("pfjet_num_pt20"  , &pfjet_num_pt20  , "pfjet_num_pt20/I");
  tree->Branch("pfjet_num_pt20hi", &pfjet_num_pt20hi, "pfjet_num_pt20hi/I");
  tree->Branch("pfjet_num_pt20lo", &pfjet_num_pt20lo, "pfjet_num_pt20lo/I");
  tree->Branch("pfjet_num_pt30"  , &pfjet_num_pt30  , "pfjet_num_pt30/I");
  tree->Branch("pfjet_num_pt30hi", &pfjet_num_pt30hi, "pfjet_num_pt30hi/I");
  tree->Branch("pfjet_num_pt30lo", &pfjet_num_pt30lo, "pfjet_num_pt30lo/I");
  tree->Branch("genphoton_pt"  , &genphoton_pt , "genphoton_pt/F");
  tree->Branch("genphoton_eta" , &genphoton_eta, "genphoton_eta/F");
  tree->Branch("genphoton_phi" , &genphoton_phi, "genphoton_phi/F");
  tree->Branch("genjet_pt"     , &genjet_pt,  "genjet_pt/F");
  tree->Branch("genjet2_pt"    , &genjet2_pt, "genjet2_pt/F");
  tree->Branch("genjet3_pt"    , &genjet3_pt, "genjet3_pt/F");
  tree->Branch("genjet4_pt"    , &genjet4_pt, "genjet4_pt/F");
  tree->Branch("genjet_num"    , &genjet_num, "genjet_num/I");
  tree->Branch("genjet_num_pt20", &genjet_num_pt20, "genjet_num_pt20/I");
  tree->Branch("genjet_num_pt30", &genjet_num_pt30, "genjet_num_pt30/I");
  tree->Branch("p_met_massT"   , &p_met_massT, "p_met_massT/F");
  tree->Branch("e_met_massT"   , &e_met_massT, "e_met_massT/F");
  tree->Branch("met_et"        , &met_et,    "met_et/F");
  tree->Branch("met_sumEt"     , &met_sumEt, "met_sumEt/F");
  tree->Branch("gen_weight"    , &gen_weight, "gen_weight/F");
  tree->Branch("gen_pthat"     , &gen_pthat, "gen_pthat/F");
  tree->Branch("fastjet_rho"   , &fastjet_rho, "fastjet_rho/D");
  tree->Branch("vertex_num"    , &vertex_num, "vertex_num/I");

  // Electron Plots
  h_electron_pt  = fs->make<TH1F>("h_electron_pt", "e with highest p_{T};p_{T}(e) (GeV)", 24, 0., 120.0);
  h_electron_eta = fs->make<TH1F>("h_electron_eta","e with highest p_{T};#eta(e)", 40, -3.0, 3.0);
  h_electron_phi = fs->make<TH1F>("h_electron_phi","e with highest p_{T};#phi(e);entries/bin", 22, (-1.-1./10.)*3.14159, (1.+1./10.)*3.14159);

  // Plots using electron & photon data
  h_e_p_deltaR   = fs->make<TH1F>("h_e_p_deltaR"  ,"#DeltaR between electron and photon;#DeltaR(e,#gamma);entries/bin"      , 20, 0.0, 0.2);
  h_e_p_deltaEta = fs->make<TH1F>("h_e_p_deltaEta","#Delta#eta between electron and photon;#Delta#eta(e,#gamma);entries/bin", 20, 0.0, 0.2);
  h_e_p_deltaPhi = fs->make<TH1F>("h_e_p_deltaPhi","#Delta#phi between electron and photon;#Delta#phi(e,#gamma);entries/bin", 20, 0.0, 0.2);
  h_e_p_deltaPt  = fs->make<TH1F>("h_e_p_deltaPt" ,"(p_{T}(e)-p_{T}(#gamma))/p_{T}(e);#Deltap_{T}/p_{T}(e);entries/bin",20, -0.2,0.2);

  // Photon Plots
  h_photon_pt    = fs->make<TH1F>("h_photon_pt" ,"Photon #DeltaR-matched to e;p_{T}(#gamma) (GeV)", 24, 0., 120.0);
  h_photon_eta   = fs->make<TH1F>("h_photon_eta","Photon #DeltaR-matched to e;#eta(#gamma)", 40, -3.0, 3.0);
  h_photon_phi   = fs->make<TH1F>("h_pho_phi"  ,"Photon #DeltaR-matched to e;#phi;entries/bin"                 , 22, (-1.-1./10.)*3.14159, (1.+1./10.)*3.14159);
  //  photon selection plots
  h_photon_sieie   = fs->make<TH1F>("h_photon_sigmaIetaIeta","Photon #DeltaR-matched to e;#sigma_{i#etai#eta}", 24, 0.0, 0.06);
  h_photon_HoverE  = fs->make<TH1F>("h_photon_HoverE"      ,"Photon #DeltaR-matched to e;Hadronic/EM"        , 20, 0.0, 0.2);
  h_photon_ecalIso = fs->make<TH1F>("h_photon_ecalIso04"   ,"Photon #DeltaR-matched to e;#SigmaEcal E_{T} in 0.4 cone (GeV)" ,20, -1.0, 15);
  h_photon_hcalIso = fs->make<TH1F>("h_photon_hcalIso04"   ,"Photon #DeltaR-matched to e;#SigmaHcal E_{T} in 0.4 cone (GeV)" ,20, 0, 15);
  h_photon_trackIso= fs->make<TH1F>("h_photon_trackIso04"  ,"Photon #DeltaR-matched to e;#Sigmatrack p_{T} in hollow 0.4 cone (GeV)",20, 0, 15); 

  // Plots to be used for efficiency calculations
  h_photon_pt_sieie    = fs->make<TH1F>("h_photon_pt_sieie"   ,"Photon #DeltaR-matched to e;p_{T}(#gamma) (GeV)", 24, 0., 120.0);
  h_photon_pt_HoverE   = fs->make<TH1F>("h_photon_pt_HoverE"  ,"Photon #DeltaR-matched to e;p_{T}(#gamma) (GeV)", 24, 0., 120.0);
  h_photon_pt_ecalIso  = fs->make<TH1F>("h_photon_pt_ecalIso" ,"Photon #DeltaR-matched to e;p_{T}(#gamma) (GeV)", 24, 0., 120.0);
  h_photon_pt_hcalIso  = fs->make<TH1F>("h_photon_pt_hcalIso" ,"Photon #DeltaR-matched to e;p_{T}(#gamma) (GeV)", 24, 0., 120.0);
  h_photon_pt_trackIso = fs->make<TH1F>("h_photon_pt_trackIso","Photon #DeltaR-matched to e;p_{T}(#gamma) (GeV)", 24, 0., 120.0);
  h_photon_eta_sieie   = fs->make<TH1F>("h_photon_eta_sieie"  ,"Photon #DeltaR-matched to e;#eta(#gamma)", 40, -3.0, 3.0);
  h_photon_eta_HoverE  = fs->make<TH1F>("h_photon_eta_HoverE" ,"Photon #DeltaR-matched to e;#eta(#gamma)", 40, -3.0, 3.0);
  h_photon_eta_ecalIso = fs->make<TH1F>("h_photon_eta_ecalIso","Photon #DeltaR-matched to e;#eta(#gamma)", 40, -3.0, 3.0);
  h_photon_eta_hcalIso = fs->make<TH1F>("h_photon_eta_hcalIso","Photon #DeltaR-matched to e;#eta(#gamma)", 40, -3.0, 3.0);
  h_photon_eta_trackIso= fs->make<TH1F>("h_photon_eta_trackIso","Photon #DeltaR-matched to e;#eta(#gamma)", 40, -3.0, 3.0);

  // Other plots
  TString metLabel = cfg_metCollection.label();
  h_met_et      = fs->make<TH1F>("h_met_et", "Missing E_{T} ("+metLabel+");Met (GeV)", 24,0.,120.0);
  h_p_met_massT = fs->make<TH1F>("h_p_met_massT","Transverse Mass (Met,#gamma);M_{T}(Met,#gamma) (GeV)",20,0.,120.0);
  h_e_met_massT = fs->make<TH1F>("h_e_met_massT","Transverse Mass (Met,e);M_{T}(Met,e) (GeV)",20,0.,120.0);
}

// ------------ method called to for each event  ------------
void
PhotonsFromWenu::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
   using namespace edm;
   using namespace reco;

   // Set TTree variables to default values
   electron_et = -9.0;
   electron_pt = -9.0;
   electron_sieie = -9.0;
   electron_hOverE= -9.0;
   electron_deltaPhi= -9.0;
   electron_deltaEta= -9.0;
   electron_TkSumPt= -9.0;
   electron_EcalIso= -9.0;
   electron_HcalIso= -9.0;
   electron_classification = -9;
   e_p_deltaR = -9.0;
   e_met_deltaPhi = -9.0;
   photon_pt  = -9.0;
   photon_eta = -9.0;
   photon_phi = -9.0;
   photon_hOverE = -9.0;
   photon_sieie  = -9.0;
   photon_sipip  = -9.0;
   photon_seedTime = -9.0;
   photon_hasPixelSeed = true;
   photon_ecalIso= -9.0;
   photon_hcalIso= -9.0;
   photon_trackIso = -9.0;
   jet_pt = 0.0;
   jet_eta = -9.0;
   jet_phi = -9.0;
   jet2_pt = -9.0;
   jet2_eta = -9.0;
   jet2_phi = -9.0;
   jet3_pt = -9.0;
   jet3_eta = -9.0;
   jet3_phi = -9.0;
   jet4_pt = -9.0;
   jet4_eta = -9.0;
   jet4_phi = -9.0;
   jet_num = 0;
   pfjet_pt = 0.0;
   pfjet_eta = -9.0;
   pfjet_phi = -9.0;
   pfjet2_pt = -9.0;
   pfjet2_eta = -9.0;
   pfjet2_phi = -9.0;
   pfjet3_pt  = -9.0;
   pfjet3_eta = -9.0;
   pfjet3_phi = -9.0;
   pfjet4_pt  = -9.0;
   pfjet4_eta = -9.0;
   pfjet4_phi = -9.0;
   pfjet_num  = 0;
   pfjet_num_pt20 = 0;
   pfjet_num_pt30 = 0;
   genphoton_pt = -9.0;
   genphoton_eta = -9.0;
   genphoton_phi = -9.0;
   genjet_pt = 0;
   genjet2_pt = 0;
   genjet3_pt = 0;
   genjet4_pt = 0;
   genjet_num = 0;
   genjet_num_pt20 = 0;
   genjet_num_pt30 = 0;
   jet_sumAllEt = 0.0;
   p_met_massT   = -9.0;
   e_met_massT = -9.0;
   met_et = -9.0;
   met_sumEt = -9.0;
   gen_weight = -9.0;
   gen_pthat = -9.0;
   fastjet_rho = -9.0;
   vertex_num = -1;

   // ------------ Get GEN Info if possible ------------
   Handle<GenEventInfoProduct> genEventScale;
   iEvent.getByLabel("generator", genEventScale);
   if (genEventScale.isValid()) {
     // This is Monte Carlo
     if( genEventScale->hasBinningValues() ) {
       gen_pthat  = genEventScale->binningValues()[0];
       gen_weight = genEventScale->weight();
     }
     //  Only include the following code if you know you NEED a gen photon in acceptance
     Handle<GenParticleCollection> genParticles;
     iEvent.getByLabel("genParticles", genParticles);
     for(size_t i = 0; i<genParticles->size() && i<20; ++i) {
       const GenParticle *genParticle = &(*genParticles)[i];
       if (genParticle->pdgId() == 22 && genParticle->status() == 1) {
	 genphoton_pt = genParticle->pt();
	 genphoton_eta = genParticle->eta();
	 genphoton_phi = genParticle->phi();
	 break;  // found lead gen photon, break
       }
     }
   } /*else {
     //   ----- No Gen Info, this is Data -----
     /// getting directly the currently active prescale value for a given trigger path
     /// (calls above prescaleSet and prescaleValue methods internally)

     // ------------ Loop over triggers ------------
     // grab the prescale value of the trigger with the highest Et threshold that it passed
     Handle<TriggerResults> triggerResults;
     iEvent.getByLabel(cfg_hltResultsCollection, triggerResults);
     if (!triggerResults.isValid()) {
       std::cout << cfg_hltResultsCollection << " collection not valid." << std::endl;
       return;
     }
     unsigned int triggerIndex; // index of trigger path
     bool passed_HLT;
     for (unsigned int i=0; i<cfg_HltPaths.size(); i++) {
       passed_HLT = false;
       triggerIndex = hltConfig_.triggerIndex(cfg_HltPaths[i]);
       if (triggerIndex < triggerResults->size()) passed_HLT = triggerResults->accept(triggerIndex);
       if (passed_HLT) hlt_prescale = hltConfig_.prescaleValue(iEvent, iSetup, cfg_HltPaths[i]);
       }
   }*/


   // ------------ Loop over event vertices ------------
   Handle<VertexCollection> vertexHandle;
   iEvent.getByLabel(cfg_vertexCollection, vertexHandle);
   VertexCollection::const_iterator v = vertexHandle->begin();
   vertex_num = vertexHandle->size();  // Number of vertices in the event
   // Primary vertex
   float primary_vertex_x = (*vertexHandle)[0].x();
   float primary_vertex_y = (*vertexHandle)[0].y();
   float primary_vertex_z = (*vertexHandle)[0].z();
   //std::cout << "primary vertex:\t" << primary_vertex_x << "\t" << primary_vertex_y << "\t" << primary_vertex_z << std::endl;


   // ------------- Get FastJet rho correction ----------
   Handle<double> fastJetRhoHandle;
   iEvent.getByLabel("kt6PFJetsMINE", "rho", fastJetRhoHandle);
   fastjet_rho = *fastJetRhoHandle;


   // ------------ Loop over electrons ------------
   Handle<GsfElectronCollection> electronCollection;
   iEvent.getByLabel(cfg_electronCollection, electronCollection);
   if (!electronCollection.isValid()) {
     std::cout << cfg_electronCollection << " collection not valid." << std::endl;
     return;
   }

   float electron_px = -9.0;
   float electron_py = -9.0;
   float electron_eta = -9.0;
   float electron_phi = -9.0;
   // non-sequential cuts
   bool e_pass0_sieie = false;
   bool e_pass1_deltaPhi = false;
   bool e_pass2_deltaEta = false;
   bool e_pass3_hOverE   = false;
   bool e_pass4_TkSumPt  = false;
   bool e_pass5_EcalIso  = false;
   bool e_pass6_HcalIso  = false;
   bool e_pass_allcuts   = false;
   for (GsfElectronCollection::const_iterator recoElectron = electronCollection->begin(); recoElectron != electronCollection->end(); recoElectron++ ) {
     // grab highest-pT electron from collection...?
     if (recoElectron->pt() > electron_pt) {
       /*if (cfg_requireElecPassCuts &&
	   recoElectron->sigmaIetaIeta() > 0.01 &&
	   recoElectron->deltaPhiSuperClusterTrackAtVtx() > 0.06 &&
	   recoElectron->deltaEtaSuperClusterTrackAtVtx() > 0.004 &&
	   recoElectron->hadronicOverEm() > 0.04 &&
	   recoElectron->dr03TkSumPt() > recoElectron->et()*0.07 &&
	   recoElectron->dr03EcalRecHitSumEt() > recoElectron->et()*0.09 &&
	   recoElectron->dr03HcalTowerSumEt() > recoElectron->et()*0.10) {
	 continue;
	 }*/
       electron_et = recoElectron->et();
       electron_sieie   = recoElectron->sigmaIetaIeta();
       electron_hOverE  = recoElectron->hadronicOverEm();
       electron_deltaPhi= recoElectron->deltaPhiSuperClusterTrackAtVtx();
       electron_deltaEta= recoElectron->deltaEtaSuperClusterTrackAtVtx();
       electron_TkSumPt = recoElectron->dr03TkSumPt();
       electron_EcalIso = recoElectron->dr03EcalRecHitSumEt();
       electron_HcalIso = recoElectron->dr03HcalTowerSumEt();
       electron_classification = recoElectron->classification();
       e_pass0_sieie    = electron_sieie < 0.01;  // ALL THESE THRESHOLDS HARD-CODED!!!
       e_pass1_deltaPhi = electron_deltaPhi < 0.06;
       e_pass2_deltaEta = electron_deltaEta < 0.004;
       e_pass3_hOverE   = electron_hOverE < 0.04;
       e_pass4_TkSumPt  = electron_TkSumPt < electron_et*0.07;
       e_pass5_EcalIso  = electron_EcalIso < electron_et*0.09;
       e_pass6_HcalIso  = electron_HcalIso < electron_et*0.10;
       e_pass_allcuts   = e_pass0_sieie && e_pass1_deltaPhi && e_pass2_deltaEta && e_pass3_hOverE && e_pass4_TkSumPt && e_pass5_EcalIso && e_pass6_HcalIso;
       electron_pt = recoElectron->pt();
       electron_px = recoElectron->px();
       electron_py = recoElectron->py();
       electron_eta = recoElectron->eta();
       electron_phi = recoElectron->phi();
       break; // found best electron, stop looping on electrons
     }
   }
   if (cfg_usePhotonMatchedToElec && electron_pt<20.0) return; // because don't want any photons matched to electron < 20 GeV? ANOTHER HARD-CODED THREHSOLD

   // ------------ Loop over photons ------------
   Handle<PhotonCollection> photonCollection;
   iEvent.getByLabel(cfg_photonCollection, photonCollection);
   if (!photonCollection.isValid()) {
     std::cout << cfg_photonCollection << " collection not valid." << std::endl;
     return;
   }

   Handle<EcalRecHitCollection> EBReducedRecHits;
   iEvent.getByLabel("reducedEcalRecHitsEB", EBReducedRecHits);
   Handle<EcalRecHitCollection> EEReducedRecHits;
   iEvent.getByLabel("reducedEcalRecHitsEE", EEReducedRecHits);
   if (!EBReducedRecHits.isValid() or !EEReducedRecHits.isValid()) {
     std::cout << "reducedEcalRecHitsEB or reducedEcalRecHitsEE" << " collection not valid." << std::endl;
     return;
   }
   EcalClusterLazyTools lazyTool(iEvent, iSetup, InputTag("reducedEcalRecHitsEB"), InputTag("reducedEcalRecHitsEE"));

   float photon_px     = -9.0;
   float photon_py     = -9.0;
   bool  photon_isBarrel = false;
   bool  photon_isEndcap = false;
   for (PhotonCollection::const_iterator recoPhoton = photonCollection->begin(); recoPhoton!=photonCollection->end(); recoPhoton++) {
     if (recoPhoton->pt()>photon_pt) {
       if (cfg_usePhotonMatchedToElec && deltaR(electron_eta, electron_phi, recoPhoton->eta(), recoPhoton->phi()) > cfg_maxDeltaR) continue;
       if (recoPhoton->pt() < 15 or fabs(recoPhoton->eta()) > 2.5 or (fabs(recoPhoton->eta())>1.45 and fabs(recoPhoton->eta())<1.55)) continue;
       photon_pt     = recoPhoton->pt();
       photon_px     = recoPhoton->px();
       photon_py     = recoPhoton->py();
       photon_eta    = recoPhoton->eta();
       photon_phi    = recoPhoton->phi(); 
       photon_hOverE = recoPhoton->hadronicOverEm();
       photon_sieie  = recoPhoton->sigmaIetaIeta();
       photon_hasPixelSeed  = recoPhoton->hasPixelSeed();
       // Next few lines get sigma-iphi-iphi
       const reco::CaloClusterPtr  seed = recoPhoton->superCluster()->seed();
       if (seed.isAvailable()) {
	 // Get sigma-iphi-iphi
	 std::vector<float> vCov = lazyTool.covariances(*seed);
	 photon_sipip  = sqrt(vCov[2]);   // This is Sqrt(covPhiPhi)
	 // Get seed time
	 DetId id      = lazyTool.getMaximum(*seed).first;
	 const EcalRecHitCollection & rechits = ( recoPhoton->isEB() ? *EBReducedRecHits : *EEReducedRecHits);
	 EcalRecHitCollection::const_iterator it = rechits.find( id );
	 if( it != rechits.end() ) {
	   photon_seedTime = it->time();
	 } else {
	   photon_seedTime = -10.0;  // Seed not found in the rechits collection? odd.
	 }
       } else {
	 //std::cout << "No seed for photon eta=" << recoPhoton->eta() << std::endl;
       }
       photon_ecalIso  = recoPhoton->ecalRecHitSumEtConeDR04();
       photon_hcalIso  = recoPhoton->hcalTowerSumEtConeDR04();
       photon_trackIso = recoPhoton->trkSumPtHollowConeDR04();
       photon_isBarrel = fabs(photon_eta)<1.4442;
       photon_isEndcap = fabs(photon_eta)>1.566 && fabs(photon_eta)<2.5;
       photon_vertex_x = recoPhoton->vertex().x();
       photon_vertex_y = recoPhoton->vertex().y();
       photon_vertex_z = recoPhoton->vertex().z();
       //std::cout<< "photon vertex:\t" << photon_vertex_x << "\t" << photon_vertex_y << "\t"<< photon_vertex_z << std::endl;
       break; // found highest pt photon, stop looping on photons
     }
   }
   if (photon_pt<70.0) return; // HARD-CODED

   // ------------ Loop over Calo jets ------------
   Handle<CaloJetCollection> jetHandle;
   iEvent.getByLabel(cfg_jetCollection, jetHandle);
   if (!jetHandle.isValid()) {
     std::cout << cfg_jetCollection << " collection not valid." << std::endl;
     return;
   }

   for (CaloJetCollection::const_iterator i_jet = jetHandle->begin(); i_jet != jetHandle->end(); i_jet++) {
     
     // if jet overlaps with photon, go to next jet in list
     if ( deltaR(i_jet->eta(), i_jet->phi(), photon_eta, photon_phi) < cfg_minDeltaRPhotonJet ) continue;

     // if jet doesn't pass ID cuts, go to next jet in list
     if ( fabs(i_jet->eta())<2.6 && i_jet->emEnergyFraction()<0.01 ) continue; // LOOK UP THESE ID CUTS

     // if jet doesn't pass min pt, go to next jet in list
     if (i_jet->pt()<cfg_minJetPt) continue;

     jet_num++;
     jet_sumAllEt += i_jet->et();
    
     if (i_jet->pt()>jet_pt) {
       jet4_pt  = jet3_pt;
       jet4_eta = jet3_eta;
       jet4_phi = jet3_phi;
       jet3_pt  = jet2_pt;
       jet3_eta = jet2_eta;
       jet3_phi = jet2_phi;
       jet2_pt  = jet_pt;
       jet2_eta = jet_eta;
       jet2_phi = jet_phi;
       jet_pt   = i_jet->pt();
       jet_eta  = i_jet->eta();
       jet_phi  = i_jet->phi();
     } else if (i_jet->pt()>jet2_pt) {
       jet4_pt  = jet3_pt;
       jet4_eta = jet3_eta;
       jet4_phi = jet3_phi;
       jet3_pt  = jet2_pt;
       jet3_eta = jet2_eta;
       jet3_phi = jet2_phi;
       jet2_pt  = i_jet->pt();
       jet2_eta = i_jet->eta();
       jet2_phi = i_jet->phi();
     } else if (i_jet->pt()>jet3_pt) {
       jet4_pt  = jet3_pt;
       jet4_eta = jet3_eta;
       jet4_phi = jet3_phi;
       jet3_pt  = i_jet->pt();
       jet3_eta = i_jet->eta();
       jet3_phi = i_jet->phi();
     } else if (i_jet->pt()>jet4_pt) {
       jet4_pt  = i_jet->pt();
       jet4_eta = i_jet->eta();
       jet4_phi = i_jet->phi();
     }

   }

   // ------------ Loop over PFJets ------------
   edm::Handle<edm::View<reco::Jet> > pfjetHandle;
   iEvent.getByLabel(cfg_pfjetCollection, pfjetHandle);
   if (!pfjetHandle.isValid()) {
     std::cout << cfg_pfjetCollection << " collection not valid." << std::endl;
     return;
   }

   // Get Track collection (to find where the jet's vertex is)
   edm::Handle<reco::TrackCollection> tracks;
   iEvent.getByLabel(cfg_trackCollection, tracks);
   if (!pfjetHandle.isValid()) {
     std::cout << cfg_trackCollection << " collection not valid." << std::endl;
     return;
   }

   // next few lines are for jet energy uncertainty
   std::string JEC_PATH("CondFormats/JetMETObjects/data/");
   //edm::FileInPath fip(JEC_PATH+"Spring10_Uncertainty_AK5Calo.txt");
   edm::FileInPath fip(JEC_PATH+"Spring10_Uncertainty_AK5PF.txt");
   // edm::FileInPath fip(JEC_PATH+"Spring10_Uncertainty_AK5JPT.txt");
   JetCorrectionUncertainty *jecUnc = new JetCorrectionUncertainty(fip.fullPath());

   for (edm::View<reco::Jet>::const_iterator i_jet = pfjetHandle->begin(); i_jet != pfjetHandle->end (); i_jet++) {

     reco::PFJet pfjet = static_cast<const reco::PFJet &>(*i_jet);

     // if jet doesn't pass min pt and have reasonable eta, go to next jet in list
     if ( pfjet.pt()<20 || fabs(pfjet.eta()) > cfg_maxJetAbsEta ) continue;

     // if jet overlaps with photon, go to next jet in list
     if ( deltaR(pfjet.eta(), pfjet.phi(), photon_eta, photon_phi) < cfg_minDeltaRPhotonJet ) continue;

     // Jet ID from Kira Grogg 
     //   http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/grogg/src/JetTreeFiller.cc?revision=1.8&view=markup
     if ( !(pfjet.neutralHadronEnergyFraction() < 0.99 &&
	    pfjet.neutralEmEnergyFraction()     < 0.99 &&
	    pfjet.chargedHadronEnergyFraction() > 0    &&
	    pfjet.chargedMultiplicity()         > 0    &&
	    pfjet.chargedEmEnergyFraction()     < 0.99)   ) continue;	
     //std::cout<< "pfjet vertex:\t" << pfjet.vertex().x() << "\t" << pfjet.vertex().y() << "\t"<< pfjet.vertex().z() << std::endl;
     //std::cout << "pfjet eta: " << pfjet.eta() << "\tpfjet pt: " << pfjet.pt() << std::endl;
     // Next few lines are for jet energy uncertainty
     jecUnc->setJetEta(pfjet.eta()); // Give rapidity of jet you want uncertainty on
     jecUnc->setJetPt(pfjet.pt());   // Also give the corrected pt of the jet you want the uncertainty on
     float uncer0 = jecUnc->getUncertainty(true);  // official (pt,eta) dependent uncertainties
     float uncer1 = 0.015;           // calibration changes and release differences // where does this come from?
     float uncer2 = 0.75*0.8*2.2*1/pfjet.pt();  // E_PU x JA x AvgPU x C / pT  // woo-ee!  where does this come from, too?
     float total_jetpt_uncert = sqrt(uncer0*uncer0+uncer1*uncer1+uncer2*uncer2);
     float pfjet_pt_high = (1+total_jetpt_uncert) * pfjet.pt();
     float pfjet_pt_low  = (1-total_jetpt_uncert) * pfjet.pt();

     // If the jet's leading track does not come from the event vertex, go to next in jet list
     float dist_jetVtx_eventVtx = 99;
     if (tracks->size() > 0) {
       float max_matched_track_pt = -99;
       float max_matched_track_Vx = -99;
       float max_matched_track_Vy = -99;
       float max_matched_track_Vz = -99;
       for (reco::TrackCollection::const_iterator track = tracks->begin(); track != tracks->end(); track++){
	 float track_pt = track->pt();
	 float dR_track_jet = deltaR(track->eta(), track->phi(), pfjet.eta(), pfjet.phi());
	 //std::cout << "track_pt = " << track_pt << "\t dR = "<< dR_track_jet << std::endl;
	 if (dR_track_jet < 0.4 && track_pt > max_matched_track_pt) {
	   // This track is in the jet, and highest pt found
	   max_matched_track_pt = track_pt;
	   max_matched_track_Vx = track->vx();
	   max_matched_track_Vy = track->vy();
	   max_matched_track_Vz = track->vz();
	 }
       }
       dist_jetVtx_eventVtx = sqrt( pow((max_matched_track_Vx - primary_vertex_x),2) + 
				    pow((max_matched_track_Vy - primary_vertex_y),2) + 
				    pow((max_matched_track_Vz - primary_vertex_z),2) );
       //std::cout << "jet vertex:\t" << max_matched_track_Vx << "\t" << max_matched_track_Vy << "\t" << max_matched_track_Vz << "\tdist=" << dist_jetVtx_eventVtx << "\tpt="<< pfjet.pt() << "\teta=" << pfjet.eta() << std::endl;
     }
     //std::cout << "jet_pt = " << pfjet.pt() <<"\t dist_jetVtx_eventVtx = " << dist_jetVtx_eventVtx << std::endl;
     if (dist_jetVtx_eventVtx > 0.2) continue;

     if (pfjet.pt()>20) pfjet_num_pt20++;
     if (pfjet.pt()>30) pfjet_num_pt30++;
     if (pfjet_pt_high>30) pfjet_num_pt30hi++;
     if (pfjet_pt_low>30)  pfjet_num_pt30lo++;
     if (pfjet_pt_high>20) pfjet_num_pt20hi++;
     if (pfjet_pt_low>20)  pfjet_num_pt20lo++;
     if (pfjet.pt()<cfg_minJetPt) continue;
     // Jet passes all cuts, store info about it
     pfjet_num++;

     if (pfjet.pt()>pfjet_pt) {
       pfjet4_pt  = pfjet3_pt;
       pfjet4_eta = pfjet3_eta;
       pfjet4_phi = pfjet3_phi;
       pfjet3_pt  = pfjet2_pt;
       pfjet3_eta = pfjet2_eta;
       pfjet3_phi = pfjet2_phi;
       pfjet2_pt  = pfjet_pt;
       pfjet2_eta = pfjet_eta;
       pfjet2_phi = pfjet_phi;
       pfjet_pt   = pfjet.pt();
       pfjet_eta  = pfjet.eta();
       pfjet_phi  = pfjet.phi();
     } else if (pfjet.pt()>pfjet2_pt) {
       pfjet4_pt  = pfjet3_pt;
       pfjet4_eta = pfjet3_eta;
       pfjet4_phi = pfjet3_phi;
       pfjet3_pt  = pfjet2_pt;
       pfjet3_eta = pfjet2_eta;
       pfjet3_phi = pfjet2_phi;
       pfjet2_pt  = pfjet.pt();
       pfjet2_eta = pfjet.eta();
       pfjet2_phi = pfjet.phi();
     } else if (pfjet.pt()>pfjet3_pt) {
       pfjet4_pt  = pfjet3_pt;
       pfjet4_eta = pfjet3_eta;
       pfjet4_phi = pfjet3_phi;
       pfjet3_pt  = pfjet.pt();
       pfjet3_eta = pfjet.eta();
       pfjet3_phi = pfjet.phi();
     } else if (pfjet.pt()>pfjet4_pt) {
       pfjet4_pt  = pfjet.pt();
       pfjet4_eta = pfjet.eta();
       pfjet4_phi = pfjet.phi();
     }
   }
   

   // ------------ Loop over Gen Jets ------------
   // We actually want gen jets that deltaR match the reco jets we found
   edm::Handle<edm::View<reco::Jet> > genjetHandle;
   iEvent.getByLabel(cfg_genjetCollection, genjetHandle);
   if (genjetHandle.isValid()) {
     // Probably running on DATA rather than MC
     for (edm::View<reco::Jet>::const_iterator jet = genjetHandle->begin (); jet != genjetHandle->end (); ++jet) {
       reco::GenJet genjet = static_cast<const reco::GenJet &>(*jet);
       if ( genphoton_pt>0 && deltaR(genjet.eta(), genjet.phi(), genphoton_eta, genphoton_phi) < cfg_minDeltaRPhotonJet ) continue; // overlap with photon
       if (fabs(genjet.eta()) > cfg_maxJetAbsEta) continue; // out of acceptance
       if (genjet.pt()>20) genjet_num_pt20++; // I guess these numbers are necessarily greater than the regular genjet_num? (if threshold lower?)
       if (genjet.pt()>30) genjet_num_pt30++;
       if ( genjet.pt()<cfg_minJetPt ) continue; // pt too low
       // Jet passes all genjet requirements, store info about it
       genjet_num++; 
       float curJet_eta = genjet.eta();
       float curJet_phi = genjet.phi();
       // Now find a gen jet that dR matches the pfJets
       // NOTE: No min pt cut for the gen jet
       if ( pfjet_pt>10 && deltaR(pfjet_eta, pfjet_phi, curJet_eta, curJet_phi) < 0.4 ) {
	 genjet_pt = genjet.pt();
       }
       if ( pfjet2_pt>10 && deltaR(pfjet2_eta, pfjet2_phi, curJet_eta, curJet_phi) < 0.4 ) {
	 genjet2_pt = genjet.pt();
       }
       if ( pfjet3_pt>10 && deltaR(pfjet3_eta, pfjet3_phi, curJet_eta, curJet_phi) < 0.4 ) {
	 genjet3_pt = genjet.pt();
       }
       if ( pfjet4_pt>10 && deltaR(pfjet4_eta, pfjet4_phi, curJet_eta, curJet_phi) < 0.4 ) {
	 genjet4_pt = genjet.pt();
       } // what is going on here with pairing e.g. pfjet3 and genjet3
     }
   }


   // ------------ Get MET ------------
   edm::Handle<edm::View<reco::MET> > METhandle;
   iEvent.getByLabel(cfg_metCollection, METhandle);
   if (!METhandle.isValid()) {
     std::cout << cfg_metCollection << " collection not valid." << std::endl;
     return;
   }
   const MET& met = METhandle->front();
   met_et = met.et();
   met_sumEt = met.sumEt();
   float met_px = met.px();
   float met_py = met.py();
   float met_phi = met.phi();
   if (met_et < cfg_minMetEt) return;

   // ------------ Calculate transverse mass ------------
   float tmp_et = met_et+photon_pt;
   float tmp_px = met_px+photon_px;
   float tmp_py = met_py+photon_py;
   if (photon_pt>0.0){
     p_met_massT = tmp_et*tmp_et - tmp_px*tmp_px - tmp_py*tmp_py;
     p_met_massT = (p_met_massT>0) ? sqrt(p_met_massT) : 0;
   } else {
     p_met_massT = 0.0;
   }

   if (electron_pt>0.0){
     tmp_et = met_et+electron_pt;
     tmp_px = met_px+electron_px;
     tmp_py = met_py+electron_py;
     e_met_massT = tmp_et*tmp_et - tmp_px*tmp_px - tmp_py*tmp_py;
     e_met_massT = (e_met_massT>0) ? sqrt(e_met_massT) : 0;
   } else {
     e_met_massT = 0.0;
   }

   // ------------ Fill Histograms and TTree ------------
   if ( photon_isBarrel || photon_isEndcap ) {
     if ( !cfg_requireElecPassCuts || (cfg_requireElecPassCuts && e_pass_allcuts && e_met_massT>cfg_minTransMass) ) {
       h_electron_pt ->Fill(electron_pt);
       h_electron_eta->Fill(electron_eta);
       h_electron_phi->Fill(electron_phi);

       e_p_deltaR = deltaR(electron_eta, electron_phi, photon_eta, photon_phi);
       e_met_deltaPhi = fabs( deltaPhi(electron_phi, met_phi) );
       h_e_p_deltaR  ->Fill( e_p_deltaR );
       h_e_p_deltaPhi->Fill( fabs(deltaPhi(electron_phi,photon_phi)) );
       h_e_p_deltaEta->Fill( fabs(electron_eta-photon_eta) );
       h_e_p_deltaPt ->Fill( (electron_pt-photon_pt)/electron_pt );

       h_photon_pt   ->Fill(photon_pt);
       h_photon_eta  ->Fill(photon_eta);
       h_photon_phi  ->Fill(photon_phi);

       h_met_et->Fill(met_et);
       h_p_met_massT->Fill(p_met_massT);
       h_e_met_massT->Fill(e_met_massT);
       // Fill the tree
       tree->Fill();
     }
     // Fill these 5 photon plots if electron passed all cuts but one in plot
     if (e_pass0_sieie && e_pass1_deltaPhi && e_pass2_deltaEta && e_pass4_TkSumPt && e_pass5_EcalIso && e_pass6_HcalIso) {
       h_photon_HoverE->Fill(photon_hOverE);
       h_photon_pt_HoverE->Fill(photon_pt);
       h_photon_eta_HoverE->Fill(photon_eta);
     }
     if (e_pass1_deltaPhi && e_pass2_deltaEta && e_pass3_hOverE && e_pass4_TkSumPt && e_pass5_EcalIso && e_pass6_HcalIso) { 
       h_photon_sieie ->Fill(photon_sieie);
       h_photon_pt_sieie->Fill(photon_pt);
       h_photon_eta_sieie->Fill(photon_eta);
     }
     if (e_pass0_sieie && e_pass1_deltaPhi && e_pass2_deltaEta && e_pass3_hOverE && e_pass4_TkSumPt && e_pass6_HcalIso) {
       h_photon_ecalIso ->Fill(photon_ecalIso);
       h_photon_pt_ecalIso->Fill(photon_pt);
       h_photon_eta_ecalIso->Fill(photon_eta);
     }
     if (e_pass0_sieie && e_pass1_deltaPhi && e_pass2_deltaEta && e_pass3_hOverE && e_pass4_TkSumPt && e_pass5_EcalIso) {
       h_photon_hcalIso ->Fill(photon_hcalIso);
       h_photon_pt_hcalIso->Fill(photon_pt);
       h_photon_eta_hcalIso->Fill(photon_eta);
     }
     if (e_pass0_sieie && e_pass1_deltaPhi && e_pass2_deltaEta && e_pass3_hOverE && e_pass5_EcalIso && e_pass6_HcalIso) {
       h_photon_trackIso->Fill(photon_trackIso);
       h_photon_pt_trackIso->Fill(photon_pt);
       h_photon_eta_trackIso->Fill(photon_eta);
     }
   }
}


// ------------ method called once each job just after ending the event loop  ------------
void 
PhotonsFromWenu::endJob() {
}

//define this as a plug-in
DEFINE_FWK_MODULE(PhotonsFromWenu);
