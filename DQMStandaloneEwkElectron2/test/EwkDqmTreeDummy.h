//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Fri Jun 11 14:56:16 2010 by ROOT version 5.22/00d
// from TTree EwkDqmTree/EwkDqmTree
// found on file: /tmp/jleonard/ewkElecStandaloneDQM-2010GoodCollsv9-9jun10.root
//////////////////////////////////////////////////////////

#ifndef EwkDqmTreeDummy_h
#define EwkDqmTreeDummy_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
   const Int_t kMax = 62;
   const Int_t kMax = 12;

class EwkDqmTreeDummy {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   UInt_t          runNumber;
   UInt_t          lumiSection;
   UInt_t          eventNumber;
   Bool_t          ele_trig;
   Int_t           triggersOfInterest_;
   string          triggersOfInterest_first[kMax];
   Bool_t          triggersOfInterest_second[kMax];   //[_]
   Int_t           l1TriggersOfInterest_;
   string          l1TriggersOfInterest_first[kMax];
   Bool_t          l1TriggersOfInterest_second[kMax];   //[_]
   Bool_t          hasWCand;
   Bool_t          hasZCand;
   Int_t           ele_nelectrons;
   Bool_t          ele_isEcalDriven[6];   //[ele_nelectrons]
   Bool_t          ele_isTrackerDriven[6];   //[ele_nelectrons]
   Bool_t          ele_isBarrel[6];   //[ele_nelectrons]
   Bool_t          ele_isEndcap[6];   //[ele_nelectrons]
   Float_t         ele_eta[6];   //[ele_nelectrons]
   Float_t         ele_phi[6];   //[ele_nelectrons]
   Float_t         ele_dxy[6];   //[ele_nelectrons]
   Float_t         ele_pt[6];   //[ele_nelectrons]
   Float_t         ele_energy[6];   //[ele_nelectrons]
   Float_t         ele_ecalenergy[6];   //[ele_nelectrons]
   Float_t         ele_ecalenergyerror[6];   //[ele_nelectrons]
   Float_t         ele_et[6];   //[ele_nelectrons]
   Float_t         ele_scet[6];   //[ele_nelectrons]
   Float_t         ele_p[6];   //[ele_nelectrons]
   Float_t         ele_electronmomentumerror[6];   //[ele_nelectrons]
   Float_t         ele_px[6];   //[ele_nelectrons]
   Float_t         ele_py[6];   //[ele_nelectrons]
   Float_t         ele_pz[6];   //[ele_nelectrons]
   Int_t           ele_nexpectedinnerhits[6];   //[ele_nelectrons]
   Float_t         ele_dcottheta[6];   //[ele_nelectrons]
   Float_t         ele_dist[6];   //[ele_nelectrons]
   Float_t         ele_deltaetaeleclustertrackatcalo[6];   //[ele_nelectrons]
   Float_t         ele_deltaetaseedclustertrackatcalo[6];   //[ele_nelectrons]
   Float_t         ele_deltaetasuperclustertrackatvtx[6];   //[ele_nelectrons]
   Float_t         ele_deltaphieleclustertrackatcalo[6];   //[ele_nelectrons]
   Float_t         ele_deltaphiseedclustertrackatcalo[6];   //[ele_nelectrons]
   Float_t         ele_deltaphisuperclustertrackatvtx[6];   //[ele_nelectrons]
   Float_t         ele_trackpositionatvtxx[6];   //[ele_nelectrons]
   Float_t         ele_trackpositionatvtxy[6];   //[ele_nelectrons]
   Float_t         ele_trackpositionatvtxz[6];   //[ele_nelectrons]
   Float_t         ele_trackmomentumatvtxx[6];   //[ele_nelectrons]
   Float_t         ele_trackmomentumatvtxy[6];   //[ele_nelectrons]
   Float_t         ele_trackmomentumatvtxz[6];   //[ele_nelectrons]
   Float_t         ele_trackpositionatcalox[6];   //[ele_nelectrons]
   Float_t         ele_trackpositionatcaloy[6];   //[ele_nelectrons]
   Float_t         ele_trackpositionatcaloz[6];   //[ele_nelectrons]
   Float_t         ele_trackmomentumatcalox[6];   //[ele_nelectrons]
   Float_t         ele_trackmomentumatcaloy[6];   //[ele_nelectrons]
   Float_t         ele_trackmomentumatcaloz[6];   //[ele_nelectrons]
   Float_t         ele_eeleclusteroverpout[6];   //[ele_nelectrons]
   Float_t         ele_eseedclusteroverp[6];   //[ele_nelectrons]
   Float_t         ele_eseedclusteroverpout[6];   //[ele_nelectrons]
   Float_t         ele_esuperclusteroverp[6];   //[ele_nelectrons]
   Float_t         ele_dr03ecalrechitsumet[6];   //[ele_nelectrons]
   Float_t         ele_dr03hcaldepth1towersumet[6];   //[ele_nelectrons]
   Float_t         ele_dr03hcaldepth2towersumet[6];   //[ele_nelectrons]
   Float_t         ele_dr03hcaltowersumet[6];   //[ele_nelectrons]
   Float_t         ele_dr03tksumpt[6];   //[ele_nelectrons]
   Float_t         ele_dr04ecalrechitsumet[6];   //[ele_nelectrons]
   Float_t         ele_dr04hcaldepth1towersumet[6];   //[ele_nelectrons]
   Float_t         ele_dr04hcaldepth2towersumet[6];   //[ele_nelectrons]
   Float_t         ele_dr04hcaltowersumet[6];   //[ele_nelectrons]
   Float_t         ele_dr04tksumpt[6];   //[ele_nelectrons]
   Float_t         ele_e1x5[6];   //[ele_nelectrons]
   Float_t         ele_e2x5max[6];   //[ele_nelectrons]
   Float_t         ele_e5x5[6];   //[ele_nelectrons]
   Float_t         ele_r1x5[6];   //[ele_nelectrons]
   Float_t         ele_r2x5max[6];   //[ele_nelectrons]
   Float_t         ele_scpreshowerenergy[6];   //[ele_nelectrons]
   Float_t         ele_scetawidth[6];   //[ele_nelectrons]
   Float_t         ele_scphiwidth[6];   //[ele_nelectrons]
   Float_t         ele_scr9[6];   //[ele_nelectrons]
   Float_t         ele_sceseedoveresupercluster[6];   //[ele_nelectrons]
   Float_t         ele_rmax3x3[6];   //[ele_nelectrons]
   Float_t         ele_hcaldepth1overecal[6];   //[ele_nelectrons]
   Float_t         ele_hcaldepth2overecal[6];   //[ele_nelectrons]
   Float_t         ele_hcaloverecal[6];   //[ele_nelectrons]
   Float_t         ele_sigmaetaeta[6];   //[ele_nelectrons]
   Float_t         ele_sigmaietaieta[6];   //[ele_nelectrons]
   Float_t         ele_basicclusterssize[6];   //[ele_nelectrons]
   Float_t         ele_numberofbrems[6];   //[ele_nelectrons]
   Float_t         ele_fbrem[6];   //[ele_nelectrons]
   Float_t         ele_scpixcharge[6];   //[ele_nelectrons]
   Float_t         ele_gsfcharge[6];   //[ele_nelectrons]
   Float_t         ele_ctfcharge[6];   //[ele_nelectrons]
   Float_t         ele_isgsfscpixchargeconsistent[6];   //[ele_nelectrons]
   Float_t         ele_isgsfctfchargeconsistent[6];   //[ele_nelectrons]
   Float_t         ele_isgsfctfscpixchargeconsistent[6];   //[ele_nelectrons]
   Float_t         ele_charge[6];   //[ele_nelectrons]
   Float_t         ele_vertexChi2[6];   //[ele_nelectrons]
   Float_t         ele_vertexNdof[6];   //[ele_nelectrons]
   Float_t         ele_vertexNormalizedChi2[6];   //[ele_nelectrons]
   Float_t         ele_vx[6];   //[ele_nelectrons]
   Float_t         ele_vy[6];   //[ele_nelectrons]
   Float_t         ele_vz[6];   //[ele_nelectrons]
   UInt_t          hlt_nHltObj_Elec10;
   UInt_t          hlt_nHltObj_Elec15;
   UInt_t          hlt_nHltObj_Phot10;
   UInt_t          hlt_nHltObj_Phot15;
   Float_t         hlt_Elec10_et[8];   //[hlt_nHltObj_Elec10]
   Float_t         hlt_Elec10_eta[8];   //[hlt_nHltObj_Elec10]
   Float_t         hlt_Elec10_phi[8];   //[hlt_nHltObj_Elec10]
   Float_t         hlt_Elec15_et[7];   //[hlt_nHltObj_Elec15]
   Float_t         hlt_Elec15_eta[7];   //[hlt_nHltObj_Elec15]
   Float_t         hlt_Elec15_phi[7];   //[hlt_nHltObj_Elec15]
   Float_t         hlt_Phot10_et[8];   //[hlt_nHltObj_Phot10]
   Float_t         hlt_Phot10_eta[8];   //[hlt_nHltObj_Phot10]
   Float_t         hlt_Phot10_phi[8];   //[hlt_nHltObj_Phot10]
   Float_t         hlt_Phot15_et[7];   //[hlt_nHltObj_Phot15]
   Float_t         hlt_Phot15_eta[7];   //[hlt_nHltObj_Phot15]
   Float_t         hlt_Phot15_phi[7];   //[hlt_nHltObj_Phot15]
   UInt_t          l1_nL1Obj;
   Float_t         l1_et[8];   //[l1_nL1Obj]
   Float_t         l1_eta[8];   //[l1_nL1Obj]
   Float_t         l1_phi[8];   //[l1_nL1Obj]
   UInt_t          sc_nSuperClusters;
   UInt_t          sc_nSuperClustersBarrel;
   UInt_t          sc_nSuperClustersEndcap;
   Bool_t          sc_isBarrel[29];   //[sc_nSuperClusters]
   Bool_t          sc_isEndcap[29];   //[sc_nSuperClusters]
   Float_t         sc_energy[29];   //[sc_nSuperClusters]
   Float_t         sc_eta[29];   //[sc_nSuperClusters]
   Float_t         sc_phi[29];   //[sc_nSuperClusters]
   Float_t         sc_et[29];   //[sc_nSuperClusters]
   Float_t         sc_rawEnergy[29];   //[sc_nSuperClusters]
   Float_t         sc_preshowerEnergy[29];   //[sc_nSuperClusters]
   Float_t         sc_etaWidth[29];   //[sc_nSuperClusters]
   Float_t         sc_phiWidth[29];   //[sc_nSuperClusters]
   Int_t           sc_nBasicClusters[29];   //[sc_nSuperClusters]
   Float_t         sc_r9[29];   //[sc_nSuperClusters]
   Float_t         sc_eSeedOverESuperCluster[29];   //[sc_nSuperClusters]

   // List of branches
   TBranch        *b_runNumber;   //!
   TBranch        *b_lumiSection;   //!
   TBranch        *b_eventNumber;   //!
   TBranch        *b_ele_trig;   //!
   TBranch        *b__;   //!
   TBranch        *b_triggersOfInterest_first;   //!
   TBranch        *b_triggersOfInterest_second;   //!
   TBranch        *b__;   //!
   TBranch        *b_l1TriggersOfInterest_first;   //!
   TBranch        *b_l1TriggersOfInterest_second;   //!
   TBranch        *b_hasWCand;   //!
   TBranch        *b_hasZCand;   //!
   TBranch        *b_ele_nelectrons;   //!
   TBranch        *b_ele_isEcalDriven;   //!
   TBranch        *b_ele_isTrackerDriven;   //!
   TBranch        *b_ele_isBarrel;   //!
   TBranch        *b_ele_isEndcap;   //!
   TBranch        *b_ele_eta;   //!
   TBranch        *b_ele_phi;   //!
   TBranch        *b_ele_dxy;   //!
   TBranch        *b_ele_pt;   //!
   TBranch        *b_ele_energy;   //!
   TBranch        *b_ele_ecalenergy;   //!
   TBranch        *b_ele_ecalenergyerror;   //!
   TBranch        *b_ele_et;   //!
   TBranch        *b_ele_scet;   //!
   TBranch        *b_ele_p;   //!
   TBranch        *b_ele_electronmomentumerror;   //!
   TBranch        *b_ele_px;   //!
   TBranch        *b_ele_py;   //!
   TBranch        *b_ele_pz;   //!
   TBranch        *b_ele_nexpectedinnerhits;   //!
   TBranch        *b_ele_dcottheta;   //!
   TBranch        *b_ele_dist;   //!
   TBranch        *b_ele_deltaetaeleclustertrackatcalo;   //!
   TBranch        *b_ele_deltaetaseedclustertrackatcalo;   //!
   TBranch        *b_ele_deltaetasuperclustertrackatvtx;   //!
   TBranch        *b_ele_deltaphieleclustertrackatcalo;   //!
   TBranch        *b_ele_deltaphiseedclustertrackatcalo;   //!
   TBranch        *b_ele_deltaphisuperclustertrackatvtx;   //!
   TBranch        *b_ele_trackpositionatvtxx;   //!
   TBranch        *b_ele_trackpositionatvtxy;   //!
   TBranch        *b_ele_trackpositionatvtxz;   //!
   TBranch        *b_ele_trackmomentumatvtxx;   //!
   TBranch        *b_ele_trackmomentumatvtxy;   //!
   TBranch        *b_ele_trackmomentumatvtxz;   //!
   TBranch        *b_ele_trackpositionatcalox;   //!
   TBranch        *b_ele_trackpositionatcaloy;   //!
   TBranch        *b_ele_trackpositionatcaloz;   //!
   TBranch        *b_ele_trackmomentumatcalox;   //!
   TBranch        *b_ele_trackmomentumatcaloy;   //!
   TBranch        *b_ele_trackmomentumatcaloz;   //!
   TBranch        *b_ele_eeleclusteroverpout;   //!
   TBranch        *b_ele_eseedclusteroverp;   //!
   TBranch        *b_ele_eseedclusteroverpout;   //!
   TBranch        *b_ele_esuperclusteroverp;   //!
   TBranch        *b_ele_dr03ecalrechitsumet;   //!
   TBranch        *b_ele_dr03hcaldepth1towersumet;   //!
   TBranch        *b_ele_dr03hcaldepth2towersumet;   //!
   TBranch        *b_ele_dr03hcaltowersumet;   //!
   TBranch        *b_ele_dr03tksumpt;   //!
   TBranch        *b_ele_dr04ecalrechitsumet;   //!
   TBranch        *b_ele_dr04hcaldepth1towersumet;   //!
   TBranch        *b_ele_dr04hcaldepth2towersumet;   //!
   TBranch        *b_ele_dr04hcaltowersumet;   //!
   TBranch        *b_ele_dr04tksumpt;   //!
   TBranch        *b_ele_e1x5;   //!
   TBranch        *b_ele_e2x5max;   //!
   TBranch        *b_ele_e5x5;   //!
   TBranch        *b_ele_r1x5;   //!
   TBranch        *b_ele_r2x5max;   //!
   TBranch        *b_ele_scpreshowerenergy;   //!
   TBranch        *b_ele_scetawidth;   //!
   TBranch        *b_ele_scphiwidth;   //!
   TBranch        *b_ele_scr9;   //!
   TBranch        *b_ele_sceseedoveresupercluster;   //!
   TBranch        *b_ele_rmax3x3;   //!
   TBranch        *b_ele_hcaldepth1overecal;   //!
   TBranch        *b_ele_hcaldepth2overecal;   //!
   TBranch        *b_ele_hcaloverecal;   //!
   TBranch        *b_ele_sigmaetaeta;   //!
   TBranch        *b_ele_sigmaietaieta;   //!
   TBranch        *b_ele_basicclusterssize;   //!
   TBranch        *b_ele_numberofbrems;   //!
   TBranch        *b_ele_fbrem;   //!
   TBranch        *b_ele_scpixcharge;   //!
   TBranch        *b_ele_gsfcharge;   //!
   TBranch        *b_ele_ctfcharge;   //!
   TBranch        *b_ele_isgsfscpixchargeconsistent;   //!
   TBranch        *b_ele_isgsfctfchargeconsistent;   //!
   TBranch        *b_ele_isgsfctfscpixchargeconsistent;   //!
   TBranch        *b_ele_charge;   //!
   TBranch        *b_ele_vertexChi2;   //!
   TBranch        *b_ele_vertexNdof;   //!
   TBranch        *b_ele_vertexNormalizedChi2;   //!
   TBranch        *b_ele_vx;   //!
   TBranch        *b_ele_vy;   //!
   TBranch        *b_ele_vz;   //!
   TBranch        *b_hlt_nHltObj_Elec10;   //!
   TBranch        *b_hlt_nHltObj_Elec15;   //!
   TBranch        *b_hlt_nHltObj_Phot10;   //!
   TBranch        *b_hlt_nHltObj_Phot15;   //!
   TBranch        *b_hlt_Elec10_et;   //!
   TBranch        *b_hlt_Elec10_eta;   //!
   TBranch        *b_hlt_Elec10_phi;   //!
   TBranch        *b_hlt_Elec15_et;   //!
   TBranch        *b_hlt_Elec15_eta;   //!
   TBranch        *b_hlt_Elec15_phi;   //!
   TBranch        *b_hlt_Phot10_et;   //!
   TBranch        *b_hlt_Phot10_eta;   //!
   TBranch        *b_hlt_Phot10_phi;   //!
   TBranch        *b_hlt_Phot15_et;   //!
   TBranch        *b_hlt_Phot15_eta;   //!
   TBranch        *b_hlt_Phot15_phi;   //!
   TBranch        *b_l1_nL1Obj;   //!
   TBranch        *b_l1_et;   //!
   TBranch        *b_l1_eta;   //!
   TBranch        *b_l1_phi;   //!
   TBranch        *b_sc_nSuperClusters;   //!
   TBranch        *b_sc_nSuperClustersBarrel;   //!
   TBranch        *b_sc_nSuperClustersEndcap;   //!
   TBranch        *b_sc_isBarrel;   //!
   TBranch        *b_sc_isEndcap;   //!
   TBranch        *b_sc_energy;   //!
   TBranch        *b_sc_eta;   //!
   TBranch        *b_sc_phi;   //!
   TBranch        *b_sc_et;   //!
   TBranch        *b_sc_rawEnergy;   //!
   TBranch        *b_sc_preshowerEnergy;   //!
   TBranch        *b_sc_etaWidth;   //!
   TBranch        *b_sc_phiWidth;   //!
   TBranch        *b_sc_nBasicClusters;   //!
   TBranch        *b_sc_r9;   //!
   TBranch        *b_sc_eSeedOverESuperCluster;   //!

   EwkDqmTreeDummy(TTree *tree=0);
   virtual ~EwkDqmTreeDummy();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef EwkDqmTreeDummy_cxx
EwkDqmTreeDummy::EwkDqmTreeDummy(TTree *tree)
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("/tmp/jleonard/ewkElecStandaloneDQM-2010GoodCollsv9-9jun10.root");
      if (!f) {
         f = new TFile("/tmp/jleonard/ewkElecStandaloneDQM-2010GoodCollsv9-9jun10.root");
         f->cd("/tmp/jleonard/ewkElecStandaloneDQM-2010GoodCollsv9-9jun10.root:/ewkElecStandaloneDQM");
      }
      tree = (TTree*)gDirectory->Get("EwkDqmTree");

   }
   Init(tree);
}

EwkDqmTreeDummy::~EwkDqmTreeDummy()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t EwkDqmTreeDummy::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t EwkDqmTreeDummy::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (!fChain->InheritsFrom(TChain::Class()))  return centry;
   TChain *chain = (TChain*)fChain;
   if (chain->GetTreeNumber() != fCurrent) {
      fCurrent = chain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void EwkDqmTreeDummy::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("runNumber", &runNumber, &b_runNumber);
   fChain->SetBranchAddress("lumiSection", &lumiSection, &b_lumiSection);
   fChain->SetBranchAddress("eventNumber", &eventNumber, &b_eventNumber);
   fChain->SetBranchAddress("ele_trig", &ele_trig, &b_ele_trig);
   fChain->SetBranchAddress("triggersOfInterest", &triggersOfInterest_, &b__);
   fChain->SetBranchAddress("triggersOfInterest.first", triggersOfInterest_first, &b_triggersOfInterest_first);
   fChain->SetBranchAddress("triggersOfInterest.second", triggersOfInterest_second, &b_triggersOfInterest_second);
   fChain->SetBranchAddress("l1TriggersOfInterest", &l1TriggersOfInterest_, &b__);
   fChain->SetBranchAddress("l1TriggersOfInterest.first", l1TriggersOfInterest_first, &b_l1TriggersOfInterest_first);
   fChain->SetBranchAddress("l1TriggersOfInterest.second", l1TriggersOfInterest_second, &b_l1TriggersOfInterest_second);
   fChain->SetBranchAddress("hasWCand", &hasWCand, &b_hasWCand);
   fChain->SetBranchAddress("hasZCand", &hasZCand, &b_hasZCand);
   fChain->SetBranchAddress("ele_nelectrons", &ele_nelectrons, &b_ele_nelectrons);
   fChain->SetBranchAddress("ele_isEcalDriven", ele_isEcalDriven, &b_ele_isEcalDriven);
   fChain->SetBranchAddress("ele_isTrackerDriven", ele_isTrackerDriven, &b_ele_isTrackerDriven);
   fChain->SetBranchAddress("ele_isBarrel", ele_isBarrel, &b_ele_isBarrel);
   fChain->SetBranchAddress("ele_isEndcap", ele_isEndcap, &b_ele_isEndcap);
   fChain->SetBranchAddress("ele_eta", ele_eta, &b_ele_eta);
   fChain->SetBranchAddress("ele_phi", ele_phi, &b_ele_phi);
   fChain->SetBranchAddress("ele_dxy", ele_dxy, &b_ele_dxy);
   fChain->SetBranchAddress("ele_pt", ele_pt, &b_ele_pt);
   fChain->SetBranchAddress("ele_energy", ele_energy, &b_ele_energy);
   fChain->SetBranchAddress("ele_ecalenergy", ele_ecalenergy, &b_ele_ecalenergy);
   fChain->SetBranchAddress("ele_ecalenergyerror", ele_ecalenergyerror, &b_ele_ecalenergyerror);
   fChain->SetBranchAddress("ele_et", ele_et, &b_ele_et);
   fChain->SetBranchAddress("ele_scet", ele_scet, &b_ele_scet);
   fChain->SetBranchAddress("ele_p", ele_p, &b_ele_p);
   fChain->SetBranchAddress("ele_electronmomentumerror", ele_electronmomentumerror, &b_ele_electronmomentumerror);
   fChain->SetBranchAddress("ele_px", ele_px, &b_ele_px);
   fChain->SetBranchAddress("ele_py", ele_py, &b_ele_py);
   fChain->SetBranchAddress("ele_pz", ele_pz, &b_ele_pz);
   fChain->SetBranchAddress("ele_nexpectedinnerhits", ele_nexpectedinnerhits, &b_ele_nexpectedinnerhits);
   fChain->SetBranchAddress("ele_dcottheta", ele_dcottheta, &b_ele_dcottheta);
   fChain->SetBranchAddress("ele_dist", ele_dist, &b_ele_dist);
   fChain->SetBranchAddress("ele_deltaetaeleclustertrackatcalo", ele_deltaetaeleclustertrackatcalo, &b_ele_deltaetaeleclustertrackatcalo);
   fChain->SetBranchAddress("ele_deltaetaseedclustertrackatcalo", ele_deltaetaseedclustertrackatcalo, &b_ele_deltaetaseedclustertrackatcalo);
   fChain->SetBranchAddress("ele_deltaetasuperclustertrackatvtx", ele_deltaetasuperclustertrackatvtx, &b_ele_deltaetasuperclustertrackatvtx);
   fChain->SetBranchAddress("ele_deltaphieleclustertrackatcalo", ele_deltaphieleclustertrackatcalo, &b_ele_deltaphieleclustertrackatcalo);
   fChain->SetBranchAddress("ele_deltaphiseedclustertrackatcalo", ele_deltaphiseedclustertrackatcalo, &b_ele_deltaphiseedclustertrackatcalo);
   fChain->SetBranchAddress("ele_deltaphisuperclustertrackatvtx", ele_deltaphisuperclustertrackatvtx, &b_ele_deltaphisuperclustertrackatvtx);
   fChain->SetBranchAddress("ele_trackpositionatvtxx", ele_trackpositionatvtxx, &b_ele_trackpositionatvtxx);
   fChain->SetBranchAddress("ele_trackpositionatvtxy", ele_trackpositionatvtxy, &b_ele_trackpositionatvtxy);
   fChain->SetBranchAddress("ele_trackpositionatvtxz", ele_trackpositionatvtxz, &b_ele_trackpositionatvtxz);
   fChain->SetBranchAddress("ele_trackmomentumatvtxx", ele_trackmomentumatvtxx, &b_ele_trackmomentumatvtxx);
   fChain->SetBranchAddress("ele_trackmomentumatvtxy", ele_trackmomentumatvtxy, &b_ele_trackmomentumatvtxy);
   fChain->SetBranchAddress("ele_trackmomentumatvtxz", ele_trackmomentumatvtxz, &b_ele_trackmomentumatvtxz);
   fChain->SetBranchAddress("ele_trackpositionatcalox", ele_trackpositionatcalox, &b_ele_trackpositionatcalox);
   fChain->SetBranchAddress("ele_trackpositionatcaloy", ele_trackpositionatcaloy, &b_ele_trackpositionatcaloy);
   fChain->SetBranchAddress("ele_trackpositionatcaloz", ele_trackpositionatcaloz, &b_ele_trackpositionatcaloz);
   fChain->SetBranchAddress("ele_trackmomentumatcalox", ele_trackmomentumatcalox, &b_ele_trackmomentumatcalox);
   fChain->SetBranchAddress("ele_trackmomentumatcaloy", ele_trackmomentumatcaloy, &b_ele_trackmomentumatcaloy);
   fChain->SetBranchAddress("ele_trackmomentumatcaloz", ele_trackmomentumatcaloz, &b_ele_trackmomentumatcaloz);
   fChain->SetBranchAddress("ele_eeleclusteroverpout", ele_eeleclusteroverpout, &b_ele_eeleclusteroverpout);
   fChain->SetBranchAddress("ele_eseedclusteroverp", ele_eseedclusteroverp, &b_ele_eseedclusteroverp);
   fChain->SetBranchAddress("ele_eseedclusteroverpout", ele_eseedclusteroverpout, &b_ele_eseedclusteroverpout);
   fChain->SetBranchAddress("ele_esuperclusteroverp", ele_esuperclusteroverp, &b_ele_esuperclusteroverp);
   fChain->SetBranchAddress("ele_dr03ecalrechitsumet", ele_dr03ecalrechitsumet, &b_ele_dr03ecalrechitsumet);
   fChain->SetBranchAddress("ele_dr03hcaldepth1towersumet", ele_dr03hcaldepth1towersumet, &b_ele_dr03hcaldepth1towersumet);
   fChain->SetBranchAddress("ele_dr03hcaldepth2towersumet", ele_dr03hcaldepth2towersumet, &b_ele_dr03hcaldepth2towersumet);
   fChain->SetBranchAddress("ele_dr03hcaltowersumet", ele_dr03hcaltowersumet, &b_ele_dr03hcaltowersumet);
   fChain->SetBranchAddress("ele_dr03tksumpt", ele_dr03tksumpt, &b_ele_dr03tksumpt);
   fChain->SetBranchAddress("ele_dr04ecalrechitsumet", ele_dr04ecalrechitsumet, &b_ele_dr04ecalrechitsumet);
   fChain->SetBranchAddress("ele_dr04hcaldepth1towersumet", ele_dr04hcaldepth1towersumet, &b_ele_dr04hcaldepth1towersumet);
   fChain->SetBranchAddress("ele_dr04hcaldepth2towersumet", ele_dr04hcaldepth2towersumet, &b_ele_dr04hcaldepth2towersumet);
   fChain->SetBranchAddress("ele_dr04hcaltowersumet", ele_dr04hcaltowersumet, &b_ele_dr04hcaltowersumet);
   fChain->SetBranchAddress("ele_dr04tksumpt", ele_dr04tksumpt, &b_ele_dr04tksumpt);
   fChain->SetBranchAddress("ele_e1x5", ele_e1x5, &b_ele_e1x5);
   fChain->SetBranchAddress("ele_e2x5max", ele_e2x5max, &b_ele_e2x5max);
   fChain->SetBranchAddress("ele_e5x5", ele_e5x5, &b_ele_e5x5);
   fChain->SetBranchAddress("ele_r1x5", ele_r1x5, &b_ele_r1x5);
   fChain->SetBranchAddress("ele_r2x5max", ele_r2x5max, &b_ele_r2x5max);
   fChain->SetBranchAddress("ele_scpreshowerenergy", ele_scpreshowerenergy, &b_ele_scpreshowerenergy);
   fChain->SetBranchAddress("ele_scetawidth", ele_scetawidth, &b_ele_scetawidth);
   fChain->SetBranchAddress("ele_scphiwidth", ele_scphiwidth, &b_ele_scphiwidth);
   fChain->SetBranchAddress("ele_scr9", ele_scr9, &b_ele_scr9);
   fChain->SetBranchAddress("ele_sceseedoveresupercluster", ele_sceseedoveresupercluster, &b_ele_sceseedoveresupercluster);
   fChain->SetBranchAddress("ele_rmax3x3", ele_rmax3x3, &b_ele_rmax3x3);
   fChain->SetBranchAddress("ele_hcaldepth1overecal", ele_hcaldepth1overecal, &b_ele_hcaldepth1overecal);
   fChain->SetBranchAddress("ele_hcaldepth2overecal", ele_hcaldepth2overecal, &b_ele_hcaldepth2overecal);
   fChain->SetBranchAddress("ele_hcaloverecal", ele_hcaloverecal, &b_ele_hcaloverecal);
   fChain->SetBranchAddress("ele_sigmaetaeta", ele_sigmaetaeta, &b_ele_sigmaetaeta);
   fChain->SetBranchAddress("ele_sigmaietaieta", ele_sigmaietaieta, &b_ele_sigmaietaieta);
   fChain->SetBranchAddress("ele_basicclusterssize", ele_basicclusterssize, &b_ele_basicclusterssize);
   fChain->SetBranchAddress("ele_numberofbrems", ele_numberofbrems, &b_ele_numberofbrems);
   fChain->SetBranchAddress("ele_fbrem", ele_fbrem, &b_ele_fbrem);
   fChain->SetBranchAddress("ele_scpixcharge", ele_scpixcharge, &b_ele_scpixcharge);
   fChain->SetBranchAddress("ele_gsfcharge", ele_gsfcharge, &b_ele_gsfcharge);
   fChain->SetBranchAddress("ele_ctfcharge", ele_ctfcharge, &b_ele_ctfcharge);
   fChain->SetBranchAddress("ele_isgsfscpixchargeconsistent", ele_isgsfscpixchargeconsistent, &b_ele_isgsfscpixchargeconsistent);
   fChain->SetBranchAddress("ele_isgsfctfchargeconsistent", ele_isgsfctfchargeconsistent, &b_ele_isgsfctfchargeconsistent);
   fChain->SetBranchAddress("ele_isgsfctfscpixchargeconsistent", ele_isgsfctfscpixchargeconsistent, &b_ele_isgsfctfscpixchargeconsistent);
   fChain->SetBranchAddress("ele_charge", ele_charge, &b_ele_charge);
   fChain->SetBranchAddress("ele_vertexChi2", ele_vertexChi2, &b_ele_vertexChi2);
   fChain->SetBranchAddress("ele_vertexNdof", ele_vertexNdof, &b_ele_vertexNdof);
   fChain->SetBranchAddress("ele_vertexNormalizedChi2", ele_vertexNormalizedChi2, &b_ele_vertexNormalizedChi2);
   fChain->SetBranchAddress("ele_vx", ele_vx, &b_ele_vx);
   fChain->SetBranchAddress("ele_vy", ele_vy, &b_ele_vy);
   fChain->SetBranchAddress("ele_vz", ele_vz, &b_ele_vz);
   fChain->SetBranchAddress("hlt_nHltObj_Elec10", &hlt_nHltObj_Elec10, &b_hlt_nHltObj_Elec10);
   fChain->SetBranchAddress("hlt_nHltObj_Elec15", &hlt_nHltObj_Elec15, &b_hlt_nHltObj_Elec15);
   fChain->SetBranchAddress("hlt_nHltObj_Phot10", &hlt_nHltObj_Phot10, &b_hlt_nHltObj_Phot10);
   fChain->SetBranchAddress("hlt_nHltObj_Phot15", &hlt_nHltObj_Phot15, &b_hlt_nHltObj_Phot15);
   fChain->SetBranchAddress("hlt_Elec10_et", hlt_Elec10_et, &b_hlt_Elec10_et);
   fChain->SetBranchAddress("hlt_Elec10_eta", hlt_Elec10_eta, &b_hlt_Elec10_eta);
   fChain->SetBranchAddress("hlt_Elec10_phi", hlt_Elec10_phi, &b_hlt_Elec10_phi);
   fChain->SetBranchAddress("hlt_Elec15_et", hlt_Elec15_et, &b_hlt_Elec15_et);
   fChain->SetBranchAddress("hlt_Elec15_eta", hlt_Elec15_eta, &b_hlt_Elec15_eta);
   fChain->SetBranchAddress("hlt_Elec15_phi", hlt_Elec15_phi, &b_hlt_Elec15_phi);
   fChain->SetBranchAddress("hlt_Phot10_et", hlt_Phot10_et, &b_hlt_Phot10_et);
   fChain->SetBranchAddress("hlt_Phot10_eta", hlt_Phot10_eta, &b_hlt_Phot10_eta);
   fChain->SetBranchAddress("hlt_Phot10_phi", hlt_Phot10_phi, &b_hlt_Phot10_phi);
   fChain->SetBranchAddress("hlt_Phot15_et", hlt_Phot15_et, &b_hlt_Phot15_et);
   fChain->SetBranchAddress("hlt_Phot15_eta", hlt_Phot15_eta, &b_hlt_Phot15_eta);
   fChain->SetBranchAddress("hlt_Phot15_phi", hlt_Phot15_phi, &b_hlt_Phot15_phi);
   fChain->SetBranchAddress("l1_nL1Obj", &l1_nL1Obj, &b_l1_nL1Obj);
   fChain->SetBranchAddress("l1_et", l1_et, &b_l1_et);
   fChain->SetBranchAddress("l1_eta", l1_eta, &b_l1_eta);
   fChain->SetBranchAddress("l1_phi", l1_phi, &b_l1_phi);
   fChain->SetBranchAddress("sc_nSuperClusters", &sc_nSuperClusters, &b_sc_nSuperClusters);
   fChain->SetBranchAddress("sc_nSuperClustersBarrel", &sc_nSuperClustersBarrel, &b_sc_nSuperClustersBarrel);
   fChain->SetBranchAddress("sc_nSuperClustersEndcap", &sc_nSuperClustersEndcap, &b_sc_nSuperClustersEndcap);
   fChain->SetBranchAddress("sc_isBarrel", sc_isBarrel, &b_sc_isBarrel);
   fChain->SetBranchAddress("sc_isEndcap", sc_isEndcap, &b_sc_isEndcap);
   fChain->SetBranchAddress("sc_energy", sc_energy, &b_sc_energy);
   fChain->SetBranchAddress("sc_eta", sc_eta, &b_sc_eta);
   fChain->SetBranchAddress("sc_phi", sc_phi, &b_sc_phi);
   fChain->SetBranchAddress("sc_et", sc_et, &b_sc_et);
   fChain->SetBranchAddress("sc_rawEnergy", sc_rawEnergy, &b_sc_rawEnergy);
   fChain->SetBranchAddress("sc_preshowerEnergy", sc_preshowerEnergy, &b_sc_preshowerEnergy);
   fChain->SetBranchAddress("sc_etaWidth", sc_etaWidth, &b_sc_etaWidth);
   fChain->SetBranchAddress("sc_phiWidth", sc_phiWidth, &b_sc_phiWidth);
   fChain->SetBranchAddress("sc_nBasicClusters", sc_nBasicClusters, &b_sc_nBasicClusters);
   fChain->SetBranchAddress("sc_r9", sc_r9, &b_sc_r9);
   fChain->SetBranchAddress("sc_eSeedOverESuperCluster", sc_eSeedOverESuperCluster, &b_sc_eSeedOverESuperCluster);
   Notify();
}

Bool_t EwkDqmTreeDummy::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void EwkDqmTreeDummy::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t EwkDqmTreeDummy::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef EwkDqmTreeDummy_cxx
