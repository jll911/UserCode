/////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Jun  2 15:57:34 2010 by ROOT version 5.27/02
// from TTree TrigTree/TrigTree
// found on file: ../../../data/TrigTree_mc.root
//////////////////////////////////////////////////////////

#ifndef ZeePlots_h
#define ZeePlots_h

#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TH1D.h>

#include "helpers.h"
//#include "DataFormats/Math/interface/LorentzVector.h"
//#include "Math/LorentzVector.h"
#include "Math/GenVector/LorentzVector.h"

typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > XYZTLorentzVector;

using std::map;
using std::string;
using std::vector;

class electron;

struct WP
{
  public :
  WP() {;}
  WP(unsigned int mh, double d, double dct, double ci, double rti, double rei, double rhi, double sieie, double dp, double de, double he) :
    missingHits(mh), dist(d), deltaCotTheta(dct), combinedIso(ci), relTrackIso(rti), relEcalIso(rei), relHcalIso(rhi), 
    sigIeIe(sieie), hoE(he), dPhiIn(dp), dEtaIn(de)
    {;}
    
  unsigned int missingHits;
  double dist;
  double deltaCotTheta;
  double combinedIso;
  double relTrackIso;
  double relEcalIso;
  double relHcalIso;
  double sigIeIe;
  double hoE;
  double dPhiIn;
  double dEtaIn;
};

class ZeePlots {
public :
   TFile          *hFile;
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain
   Long64_t        ientry; // event number
   
   std::vector<electron> recoElectronCollection;
   std::map<std::string, int> triggers;
   std::map<std::string, TH1D*> hists;
   std::map<std::string, std::set<unsigned int> > usedHltCands;
/*    std::map<std::string, std::vector<unsigned int> > hltElecMatch; */
   std::vector<std::string> cutVec;
   std::vector<std::string> wpVec;
   std::string cutList_;
   std::string wpList_;
   std::string sample_;
   std::string triggersOfInterestL1;
   std::string triggersOfInterestHlt;

   // Declaration of leaf types
   Int_t           EvtInfo_EventNum;
   Int_t           EvtInfo_RunNum;
   Int_t           EvtInfo_LumiSection;
   Int_t           EvtInfo_BunchXing;
   Double_t        ElectronNum;
   vector<double>  *ElectronPt;
   vector<double>  *ElectronEta;
   vector<double>  *ElectronPhi;
   vector<double>  *ElectronEt;
   vector<int>     *ElectroneleEcalDriven;
   vector<int>     *ElectroneleTrkDriven;
/*    vector<double>  *ElectronDhElecClsTrkAtCalo; */
/*    vector<double>  *ElectronDhSeedClsTrkAtCalo; */
   vector<double>  *ElectronDhSuperClsTrkAtVtx;
/*    vector<double>  *ElectronDphiElecClsTrkAtCalo; */
/*    vector<double>  *ElectronDphiSeedClsTrkAtCalo; */
   vector<double>  *ElectronDphiSuperClsTrkAtVtx;
/*    vector<double>  *ElectronPositionXTrkAtVtx; */
/*    vector<double>  *ElectronPositionYTrkAtVtx; */
/*    vector<double>  *ElectronPositionZTrkAtVtx; */
/*    vector<double>  *ElectronMomentumXTrkAtVtx; */
/*    vector<double>  *ElectronMomentumYTrkAtVtx; */
/*    vector<double>  *ElectronMomentumZTrkAtVtx; */
/*    vector<double>  *ElectronPositionXTrkAtCalo; */
/*    vector<double>  *ElectronPositionYTrkAtCalo; */
/*    vector<double>  *ElectronPositionZTrkAtCalo; */
/*    vector<double>  *ElectronMomentumXTrkAtCalo; */
/*    vector<double>  *ElectronMomentumYTrkAtCalo; */
/*    vector<double>  *ElectronMomentumZTrkAtCalo; */
/*    vector<double>  *ElectroneEleClsOverPout; */
/*    vector<double>  *ElectroneSeedClsOverP; */
/*    vector<double>  *ElectroneSeedClsOverPout; */
/*    vector<double>  *ElectroneSuperClsOverP; */
/*    vector<double>  *ElectroneleMomErr; */
/*    vector<double>  *Electroneledr03EcalRecHitSumEt; */
/*    vector<double>  *Electroneledr03HcalDepth1TowerSumEt; */
/*    vector<double>  *Electroneledr03HcalDepth2TowerSumEt; */
/*    vector<double>  *Electroneledr03HcalTowerSumEt; */
/*    vector<double>  *Electroneledr03TkSumPt; */
/*    vector<double>  *Electroneledr04EcalRecHitSumEt; */
/*    vector<double>  *Electroneledr04HcalDepth1TowerSumEt; */
/*    vector<double>  *Electroneledr04HcalDepth2TowerSumEt; */
/*    vector<double>  *Electroneledr04HcalTowerSumEt; */
/*    vector<double>  *Electroneledr04TkSumPt; */
   vector<double>  *ElectroneleRelIsoEcal;
   vector<double>  *ElectroneleRelIsoHcal;
   vector<double>  *ElectroneleRelIsoTrk;
   vector<double>  *ElectroneleRelIsoComb;
   vector<double>  *ElectroneleMissingHits;
   vector<double>  *ElectroneleDist;
   vector<double>  *ElectroneleDeltaCotTheta;
   vector<double>  *ElectroneleConvRadius;
   vector<double>  *ElectroneleMaxOver3x3;
/*    vector<double>  *Electronele3x3; */
/*    vector<double>  *ElectroneleMax; */
/*    vector<double>  *Electrone1x5; */
/*    vector<double>  *Electrone2x5Max; */
/*    vector<double>  *Electrone5x5; */
/*    vector<double>  *Electroneler1x5; */
/*    vector<double>  *Electroneler2x5max; */
/*    vector<double>  *Electronscpreshowerenergy; */
/*    vector<double>  *Electronscetawidth; */
/*    vector<double>  *Electronscphiwidth; */
   vector<double>  *ElectroneleScEnergy;
   vector<double>  *ElectroneleScEta;
   vector<double>  *ElectroneleScEt;
   vector<double>  *Electroneleenergy;
/*    vector<double>  *ElectronelehcalDepth1OverEcal; */
/*    vector<double>  *ElectronelehcalDepth2OverEcal; */
   vector<double>  *ElectronelehcalOverEcal;
/*    vector<double>  *ElectronelesigmaEtaEta; */
   vector<double>  *ElectronelesigmaIetaIeta;
/*    vector<double>  *ElectronelebasicClustersSize; */
/*    vector<double>  *ElectronelenumberOfBrems; */
/*    vector<double>  *Electronelefbrem; */
/*    vector<double>  *ElectronelescPixCharge; */
/*    vector<double>  *Electronelectfcharge; */
   vector<double>  *Electronelecharge;
/*    vector<double>  *ElectroneleisGsfScPixChargeConsistent; */
/*    vector<double>  *ElectroneleisGsfCtfChargeConsistent; */
/*    vector<double>  *ElectroneleisGsfCtfScPixChargeConsistent; */
/*    vector<double>  *ElectronelevertexChi2; */
/*    vector<double>  *ElectronelevertexNdof; */
/*    vector<double>  *ElectronelevertexNormalizedChi2; */
/*    vector<double>  *Electronelevx; */
/*    vector<double>  *Electronelevy; */
/*    vector<double>  *Electronelevz; */
/*    vector<double>  *Electronelep; */
   vector<double>  *Electronelepx;
   vector<double>  *Electronelepy;
   vector<double>  *Electronelepz;
   vector<int>     *trigResults;
   vector<string>  *trigName;
   vector<double>  *hltEle10LWEt;
   vector<double>  *hltEle10LWPt;
   vector<double>  *hltEle10LWEta;
   vector<double>  *hltEle10LWPhi;
   vector<double>  *hltEle15LWEt;
   vector<double>  *hltEle15LWPt;
   vector<double>  *hltEle15LWEta;
   vector<double>  *hltEle15LWPhi;
   vector<double>  *hltEle20LWEt;
   vector<double>  *hltEle20LWPt;
   vector<double>  *hltEle20LWEta;
   vector<double>  *hltEle20LWPhi;
   vector<double>  *hltEle10SWEt;
   vector<double>  *hltEle10SWPt;
   vector<double>  *hltEle10SWEta;
   vector<double>  *hltEle10SWPhi;
   vector<double>  *hltEle15SWEt;
   vector<double>  *hltEle15SWPt;
   vector<double>  *hltEle15SWEta;
   vector<double>  *hltEle15SWPhi;
   vector<double>  *hltEle20SWEt;
   vector<double>  *hltEle20SWPt;
   vector<double>  *hltEle20SWEta;
   vector<double>  *hltEle20SWPhi;
   vector<double>  *hltEle25SWEt;
   vector<double>  *hltEle25SWPt;
   vector<double>  *hltEle25SWEta;
   vector<double>  *hltEle25SWPhi;
   vector<double>  *hltPhoton10Et;
   vector<double>  *hltPhoton10Pt;
   vector<double>  *hltPhoton10Eta;
   vector<double>  *hltPhoton10Phi;
   vector<double>  *hltPhoton15Et;
   vector<double>  *hltPhoton15Pt;
   vector<double>  *hltPhoton15Eta;
   vector<double>  *hltPhoton15Phi;
   vector<double>  *hltPhoton20Et;
   vector<double>  *hltPhoton20Pt;
   vector<double>  *hltPhoton20Eta;
   vector<double>  *hltPhoton20Phi;
   vector<double>  *hltPhoton15CleanEt;
   vector<double>  *hltPhoton15CleanPt;
   vector<double>  *hltPhoton15CleanEta;
   vector<double>  *hltPhoton15CleanPhi;
   vector<double>  *hltPhoton20CleanEt;
   vector<double>  *hltPhoton20CleanPt;
   vector<double>  *hltPhoton20CleanEta;
   vector<double>  *hltPhoton20CleanPhi;
   vector<double>  *hltPhoton30CleanEt;
   vector<double>  *hltPhoton30CleanPt;
   vector<double>  *hltPhoton30CleanEta;
   vector<double>  *hltPhoton30CleanPhi;
   vector<int>     *L1trigResults;
   vector<int>     *L1trigErrCode;
   vector<string>  *L1trigName;
   vector<double>  *l1IsoEleEt;
   vector<double>  *l1IsoEleEnergy;
   vector<double>  *l1IsoEleEta;
   vector<double>  *l1IsoElePhi;
   vector<double>  *l1NonIsoEleEt;
   vector<double>  *l1NonIsoEleEnergy;
   vector<double>  *l1NonIsoEleEta;
   vector<double>  *l1NonIsoElePhi;
   vector<int>     *ElectronelePassWp95SpikeCut;
   vector<int>     *ElectronelePassWp95ScEtCut;
   vector<int>     *ElectronelePassWp95MissHitsCut;
   vector<int>     *ElectronelePassWp95DistCut;
   vector<int>     *ElectronelePassWp95DCotThetaCut;
   vector<int>     *ElectronelePassWp95ConvRejCut;
   vector<int>     *ElectronelePassWp95RelIsoEcalCut;
   vector<int>     *ElectronelePassWp95RelIsoHcalCut;
   vector<int>     *ElectronelePassWp95RelIsoTrkCut;
   vector<int>     *ElectronelePassWp95RelIsoIndivCut;
   vector<int>     *ElectronelePassWp95RelIsoCombCut;
   vector<int>     *ElectronelePassWp95IsoCut;
   vector<int>     *ElectronelePassWp95SieieCut;
   vector<int>     *ElectronelePassWp95DphiinCut;
   vector<int>     *ElectronelePassWp95DetainCut;
   vector<int>     *ElectronelePassWp95HoeCut;
   vector<int>     *ElectronelePassWp95EidCut;
   vector<int>     *ElectronelePassWp95;


   Double_t        scEBNum;
   vector<double>  *scEBEnergy;
   vector<double>  *scEBEt;
   vector<double>  *scEBEta;
   vector<double>  *scEBPhi;
   vector<double>  *scEBRawEnergy;
   vector<double>  *scEBpreshowerEnergy;
   vector<double>  *scEBenergy1;
   vector<double>  *scEBenergy2x2;
   vector<double>  *scEBenergy3x3;
   vector<double>  *scEBenergy1x5;
   vector<double>  *scEBenergy2x5;
   vector<double>  *scEBenergy5x5;
   vector<double>  *scEBHoverE;
   vector<double>  *scEBx;
   vector<double>  *scEBy;
   vector<double>  *scEBz;
   vector<double>  *scEBetaWidth;
   vector<double>  *scEBphiWidth;
   vector<double>  *scEBsigmaetaeta;
   vector<double>  *scEBsigmaIetaIeta;
   vector<double>  *scEBsigmaphiphi;
   vector<double>  *scEBsigmaIphiIphi;
   vector<double>  *scEBsize;
   vector<double>  *scEBnBasicClusters;
   Double_t        scEENum;
   vector<double>  *scEEEnergy;
   vector<double>  *scEEEt;
   vector<double>  *scEEEta;
   vector<double>  *scEEPhi;
   vector<double>  *scEERawEnergy;
   vector<double>  *scEEpreshowerEnergy;
   vector<double>  *scEEenergy1;
   vector<double>  *scEEenergy2x2;
   vector<double>  *scEEenergy3x3;
   vector<double>  *scEEenergy1x5;
   vector<double>  *scEEenergy2x5;
   vector<double>  *scEEenergy5x5;
   vector<double>  *scEEHoverE;
   vector<double>  *scEEx;
   vector<double>  *scEEy;
   vector<double>  *scEEz;
   vector<double>  *scEEetaWidth;
   vector<double>  *scEEphiWidth;
   vector<double>  *scEEsigmaetaeta;
   vector<double>  *scEEsigmaIetaIeta;
   vector<double>  *scEEsigmaphiphi;
   vector<double>  *scEEsigmaIphiIphi;
   vector<double>  *scEEsize;
   vector<double>  *scEEnBasicClusters;


   // List of branches
   TBranch        *b_EvtInfo_EventNum;   //!
   TBranch        *b_EvtInfo_RunNum;   //!
   TBranch        *b_EvtInfo_LumiSection;   //!
   TBranch        *b_EvtInfo_BunchXing;   //!
   TBranch        *b_ElectronNum;   //!
   TBranch        *b_ElectronPt;   //!
   TBranch        *b_ElectronEta;   //!
   TBranch        *b_ElectronPhi;   //!
   TBranch        *b_ElectronEt;   //!
   TBranch        *b_ElectroneleEcalDriven;   //!
   TBranch        *b_ElectroneleTrkDriven;   //!
/*    TBranch        *b_ElectronDhElecClsTrkAtCalo;   //! */
/*    TBranch        *b_ElectronDhSeedClsTrkAtCalo;   //! */
   TBranch        *b_ElectronDhSuperClsTrkAtVtx;   //!
/*    TBranch        *b_ElectronDphiElecClsTrkAtCalo;   //! */
/*    TBranch        *b_ElectronDphiSeedClsTrkAtCalo;   //! */
   TBranch        *b_ElectronDphiSuperClsTrkAtVtx;   //!
/*    TBranch        *b_ElectronPositionXTrkAtVtx;   //! */
/*    TBranch        *b_ElectronPositionYTrkAtVtx;   //! */
/*    TBranch        *b_ElectronPositionZTrkAtVtx;   //! */
/*    TBranch        *b_ElectronMomentumXTrkAtVtx;   //! */
/*    TBranch        *b_ElectronMomentumYTrkAtVtx;   //! */
/*    TBranch        *b_ElectronMomentumZTrkAtVtx;   //! */
/*    TBranch        *b_ElectronPositionXTrkAtCalo;   //! */
/*    TBranch        *b_ElectronPositionYTrkAtCalo;   //! */
/*    TBranch        *b_ElectronPositionZTrkAtCalo;   //! */
/*    TBranch        *b_ElectronMomentumXTrkAtCalo;   //! */
/*    TBranch        *b_ElectronMomentumYTrkAtCalo;   //! */
/*    TBranch        *b_ElectronMomentumZTrkAtCalo;   //! */
/*    TBranch        *b_ElectroneEleClsOverPout;   //! */
/*    TBranch        *b_ElectroneSeedClsOverP;   //! */
/*    TBranch        *b_ElectroneSeedClsOverPout;   //! */
/*    TBranch        *b_ElectroneSuperClsOverP;   //! */
/*    TBranch        *b_ElectroneleMomErr;   //! */
/*    TBranch        *b_Electroneledr03EcalRecHitSumEt;   //! */
/*    TBranch        *b_Electroneledr03HcalDepth1TowerSumEt;   //! */
/*    TBranch        *b_Electroneledr03HcalDepth2TowerSumEt;   //! */
/*    TBranch        *b_Electroneledr03HcalTowerSumEt;   //! */
/*    TBranch        *b_Electroneledr03TkSumPt;   //! */
/*    TBranch        *b_Electroneledr04EcalRecHitSumEt;   //! */
/*    TBranch        *b_Electroneledr04HcalDepth1TowerSumEt;   //! */
/*    TBranch        *b_Electroneledr04HcalDepth2TowerSumEt;   //! */
/*    TBranch        *b_Electroneledr04HcalTowerSumEt;   //! */
/*    TBranch        *b_Electroneledr04TkSumPt;   //! */
   TBranch        *b_ElectroneleRelIsoEcal;   //!
   TBranch        *b_ElectroneleRelIsoHcal;   //!
   TBranch        *b_ElectroneleRelIsoTrk;   //!
   TBranch        *b_ElectroneleRelIsoComb;   //!
   TBranch        *b_ElectroneleMissingHits;   //!
   TBranch        *b_ElectroneleDist;   //!
   TBranch        *b_ElectroneleDeltaCotTheta;   //!
   TBranch        *b_ElectroneleConvRadius;   //!
   TBranch        *b_ElectroneleMaxOver3x3;   //!
/*    TBranch        *b_Electronele3x3;   //! */
/*    TBranch        *b_ElectroneleMax;   //! */
/*    TBranch        *b_Electrone1x5;   //! */
/*    TBranch        *b_Electrone2x5Max;   //! */
/*    TBranch        *b_Electrone5x5;   //! */
/*    TBranch        *b_Electroneler1x5;   //! */
/*    TBranch        *b_Electroneler2x5max;   //! */
/*    TBranch        *b_Electronscpreshowerenergy;   //! */
/*    TBranch        *b_Electronscetawidth;   //! */
/*    TBranch        *b_Electronscphiwidth;   //! */
   TBranch        *b_ElectroneleScEnergy;   //!
   TBranch        *b_ElectroneleScEta;   //!
   TBranch        *b_ElectroneleScEt;   //!
   TBranch        *b_Electroneleenergy;   //!
/*    TBranch        *b_ElectronelehcalDepth1OverEcal;   //! */
/*    TBranch        *b_ElectronelehcalDepth2OverEcal;   //! */
   TBranch        *b_ElectronelehcalOverEcal;   //!
/*    TBranch        *b_ElectronelesigmaEtaEta;   //! */
   TBranch        *b_ElectronelesigmaIetaIeta;   //!
/*    TBranch        *b_ElectronelebasicClustersSize;   //! */
/*    TBranch        *b_ElectronelenumberOfBrems;   //! */
/*    TBranch        *b_Electronelefbrem;   //! */
/*    TBranch        *b_ElectronelescPixCharge;   //! */
/*    TBranch        *b_Electronelectfcharge;   //! */
   TBranch        *b_Electronelecharge;   //!
/*    TBranch        *b_ElectroneleisGsfScPixChargeConsistent;   //! */
/*    TBranch        *b_ElectroneleisGsfCtfChargeConsistent;   //! */
/*    TBranch        *b_ElectroneleisGsfCtfScPixChargeConsistent;   //! */
/*    TBranch        *b_ElectronelevertexChi2;   //! */
/*    TBranch        *b_ElectronelevertexNdof;   //! */
/*    TBranch        *b_ElectronelevertexNormalizedChi2;   //! */
/*    TBranch        *b_Electronelevx;   //! */
/*    TBranch        *b_Electronelevy;   //! */
/*    TBranch        *b_Electronelevz;   //! */
/*    TBranch        *b_Electronelep;   //! */
   TBranch        *b_Electronelepx;   //!
   TBranch        *b_Electronelepy;   //!
   TBranch        *b_Electronelepz;   //!
   TBranch        *b_trigResults;   //!
   TBranch        *b_trigName;   //!
   TBranch        *b_hltEle10LWEt;   //!
   TBranch        *b_hltEle10LWPt;   //!
   TBranch        *b_hltEle10LWEta;   //!
   TBranch        *b_hltEle10LWPhi;   //!
   TBranch        *b_hltEle15LWEt;   //!
   TBranch        *b_hltEle15LWPt;   //!
   TBranch        *b_hltEle15LWEta;   //!
   TBranch        *b_hltEle15LWPhi;   //!
   TBranch        *b_hltEle20LWEt;   //!
   TBranch        *b_hltEle20LWPt;   //!
   TBranch        *b_hltEle20LWEta;   //!
   TBranch        *b_hltEle20LWPhi;   //!
   TBranch        *b_hltEle10SWEt;   //!
   TBranch        *b_hltEle10SWPt;   //!
   TBranch        *b_hltEle10SWEta;   //!
   TBranch        *b_hltEle10SWPhi;   //!
   TBranch        *b_hltEle15SWEt;   //!
   TBranch        *b_hltEle15SWPt;   //!
   TBranch        *b_hltEle15SWEta;   //!
   TBranch        *b_hltEle15SWPhi;   //!
   TBranch        *b_hltEle20SWEt;   //!
   TBranch        *b_hltEle20SWPt;   //!
   TBranch        *b_hltEle20SWEta;   //!
   TBranch        *b_hltEle20SWPhi;   //!
   TBranch        *b_hltEle25SWEt;   //!
   TBranch        *b_hltEle25SWPt;   //!
   TBranch        *b_hltEle25SWEta;   //!
   TBranch        *b_hltEle25SWPhi;   //!
   TBranch        *b_hltPhoton10Et;   //!
   TBranch        *b_hltPhoton10Pt;   //!
   TBranch        *b_hltPhoton10Eta;   //!
   TBranch        *b_hltPhoton10Phi;   //!
   TBranch        *b_hltPhoton15Et;   //!
   TBranch        *b_hltPhoton15Pt;   //!
   TBranch        *b_hltPhoton15Eta;   //!
   TBranch        *b_hltPhoton15Phi;   //!
   TBranch        *b_hltPhoton20Et;   //!
   TBranch        *b_hltPhoton20Pt;   //!
   TBranch        *b_hltPhoton20Eta;   //!
   TBranch        *b_hltPhoton20Phi;   //!
   TBranch        *b_hltPhoton15CleanEt;   //!
   TBranch        *b_hltPhoton15CleanPt;   //!
   TBranch        *b_hltPhoton15CleanEta;   //!
   TBranch        *b_hltPhoton15CleanPhi;   //!
   TBranch        *b_hltPhoton20CleanEt;   //!
   TBranch        *b_hltPhoton20CleanPt;   //!
   TBranch        *b_hltPhoton20CleanEta;   //!
   TBranch        *b_hltPhoton20CleanPhi;   //!
   TBranch        *b_hltPhoton30CleanEt;   //!
   TBranch        *b_hltPhoton30CleanPt;   //!
   TBranch        *b_hltPhoton30CleanEta;   //!
   TBranch        *b_hltPhoton30CleanPhi;   //!
   TBranch        *b_L1trigResults;   //!
   TBranch        *b_L1trigErrCode;   //!
   TBranch        *b_L1trigName;   //!
   TBranch        *b_l1IsoEleEt;   //!
   TBranch        *b_l1IsoEleEnergy;   //!
   TBranch        *b_l1IsoEleEta;   //!
   TBranch        *b_l1IsoElePhi;   //!
   TBranch        *b_l1NonIsoEleEt;   //!
   TBranch        *b_l1NonIsoEleEnergy;   //!
   TBranch        *b_l1NonIsoEleEta;   //!
   TBranch        *b_l1NonIsoElePhi;   //!
   TBranch        *b_ElectronelePassWp95SpikeCut;   //!
   TBranch        *b_ElectronelePassWp95ScEtCut;   //!
   TBranch        *b_ElectronelePassWp95MissHitsCut;   //!
   TBranch        *b_ElectronelePassWp95DistCut;   //!
   TBranch        *b_ElectronelePassWp95DCotThetaCut;   //!
   TBranch        *b_ElectronelePassWp95ConvRejCut;   //!
   TBranch        *b_ElectronelePassWp95RelIsoEcalCut;   //!
   TBranch        *b_ElectronelePassWp95RelIsoHcalCut;   //!
   TBranch        *b_ElectronelePassWp95RelIsoTrkCut;   //!
   TBranch        *b_ElectronelePassWp95RelIsoIndivCut;   //!
   TBranch        *b_ElectronelePassWp95RelIsoCombCut;   //!
   TBranch        *b_ElectronelePassWp95IsoCut;   //!
   TBranch        *b_ElectronelePassWp95SieieCut;   //!
   TBranch        *b_ElectronelePassWp95DphiinCut;   //!
   TBranch        *b_ElectronelePassWp95DetainCut;   //!
   TBranch        *b_ElectronelePassWp95HoeCut;   //!
   TBranch        *b_ElectronelePassWp95EidCut;   //!
   TBranch        *b_ElectronelePassWp95;   //!
   
   #ifdef USE_SC
   TBranch        *b_scEBNum;   //!
   TBranch        *b_scEBEnergy;   //!
   TBranch        *b_scEBEt;   //!
   TBranch        *b_scEBEta;   //!
   TBranch        *b_scEBPhi;   //!
   TBranch        *b_scEBRawEnergy;   //!
   TBranch        *b_scEBpreshowerEnergy;   //!
   TBranch        *b_scEBenergy1;   //!
   TBranch        *b_scEBenergy2x2;   //!
   TBranch        *b_scEBenergy3x3;   //!
   TBranch        *b_scEBenergy1x5;   //!
   TBranch        *b_scEBenergy2x5;   //!
   TBranch        *b_scEBenergy5x5;   //!
   TBranch        *b_scEBHoverE;   //!
   TBranch        *b_scEBx;   //!
   TBranch        *b_scEBy;   //!
   TBranch        *b_scEBz;   //!
   TBranch        *b_scEBetaWidth;   //!
   TBranch        *b_scEBphiWidth;   //!
   TBranch        *b_scEBsigmaetaeta;   //!
   TBranch        *b_scEBsigmaIetaIeta;   //!
   TBranch        *b_scEBsigmaphiphi;   //!
   TBranch        *b_scEBsigmaIphiIphi;   //!
   TBranch        *b_scEBsize;   //!
   TBranch        *b_scEBnBasicClusters;   //!
   TBranch        *b_scEENum;   //!
   TBranch        *b_scEEEnergy;   //!
   TBranch        *b_scEEEt;   //!
   TBranch        *b_scEEEta;   //!
   TBranch        *b_scEEPhi;   //!
   TBranch        *b_scEERawEnergy;   //!
   TBranch        *b_scEEpreshowerEnergy;   //!
   TBranch        *b_scEEenergy1;   //!
   TBranch        *b_scEEenergy2x2;   //!
   TBranch        *b_scEEenergy3x3;   //!
   TBranch        *b_scEEenergy1x5;   //!
   TBranch        *b_scEEenergy2x5;   //!
   TBranch        *b_scEEenergy5x5;   //!
   TBranch        *b_scEEHoverE;   //!
   TBranch        *b_scEEx;   //!
   TBranch        *b_scEEy;   //!
   TBranch        *b_scEEz;   //!
   TBranch        *b_scEEetaWidth;   //!
   TBranch        *b_scEEphiWidth;   //!
   TBranch        *b_scEEsigmaetaeta;   //!
   TBranch        *b_scEEsigmaIetaIeta;   //!
   TBranch        *b_scEEsigmaphiphi;   //!
   TBranch        *b_scEEsigmaIphiIphi;   //!
   TBranch        *b_scEEsize;   //!
   TBranch        *b_scEEnBasicClusters;   //!
   #endif
   
   ZeePlots(string sample, double limit, double first);
   virtual ~ZeePlots();
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop(double limit, double first);
   virtual void     BookHistograms();
   virtual void     BookAnalysisHistograms();
   virtual Bool_t   Notify();
   bool selectRecoElectrons();
   
   bool applyRecoEleCut(std::string cut, const WP & wp, unsigned int which);
   bool applyRecoEleWp95Cut(std::string cut, unsigned int which);
   bool foundElectronMatchToHlt( std::string trigger, double dR, unsigned int which );
   //   void matchElectronsToHlt( double dRmatch );
   
   void fillRecoElectronL1Histograms(double l1match);
   void fillRecoElectronHltHistograms(double l1match, double hltmatch);
   void fillRecoElectronAnalysisHistograms(double l1match, double hltmatch);
   void fillRecoElectronHistoCollection(string whichHisto, unsigned int ele);
   
   std::vector<std::string> stringParser(std::string cutString);
   void setWp(std::string givenWp, WP & wpEb, WP & wpEe);
   double invMass(double mass1, double energy1, double px1, double py1, double pz1, double mass2, double energy2, double px2, double py2, double pz2);

};

#endif


Int_t ZeePlots::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t ZeePlots::LoadTree(Long64_t entry)
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

void ZeePlots::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   ElectronPt = 0;
   ElectronEta = 0;
   ElectronPhi = 0;
   ElectronEt = 0;
   ElectroneleEcalDriven = 0;
   ElectroneleTrkDriven = 0;
/*    ElectronDhElecClsTrkAtCalo = 0; */
/*    ElectronDhSeedClsTrkAtCalo = 0; */
   ElectronDhSuperClsTrkAtVtx = 0;
/*    ElectronDphiElecClsTrkAtCalo = 0; */
/*    ElectronDphiSeedClsTrkAtCalo = 0; */
   ElectronDphiSuperClsTrkAtVtx = 0;
/*    ElectronPositionXTrkAtVtx = 0; */
/*    ElectronPositionYTrkAtVtx = 0; */
/*    ElectronPositionZTrkAtVtx = 0; */
/*    ElectronMomentumXTrkAtVtx = 0; */
/*    ElectronMomentumYTrkAtVtx = 0; */
/*    ElectronMomentumZTrkAtVtx = 0; */
/*    ElectronPositionXTrkAtCalo = 0; */
/*    ElectronPositionYTrkAtCalo = 0; */
/*    ElectronPositionZTrkAtCalo = 0; */
/*    ElectronMomentumXTrkAtCalo = 0; */
/*    ElectronMomentumYTrkAtCalo = 0; */
/*    ElectronMomentumZTrkAtCalo = 0; */
/*    ElectroneEleClsOverPout = 0; */
/*    ElectroneSeedClsOverP = 0; */
/*    ElectroneSeedClsOverPout = 0; */
/*    ElectroneSuperClsOverP = 0; */
/*    ElectroneleMomErr = 0; */
/*    Electroneledr03EcalRecHitSumEt = 0; */
/*    Electroneledr03HcalDepth1TowerSumEt = 0; */
/*    Electroneledr03HcalDepth2TowerSumEt = 0; */
/*    Electroneledr03HcalTowerSumEt = 0; */
/*    Electroneledr03TkSumPt = 0; */
/*    Electroneledr04EcalRecHitSumEt = 0; */
/*    Electroneledr04HcalDepth1TowerSumEt = 0; */
/*    Electroneledr04HcalDepth2TowerSumEt = 0; */
/*    Electroneledr04HcalTowerSumEt = 0; */
/*    Electroneledr04TkSumPt = 0; */
   ElectroneleRelIsoEcal = 0;
   ElectroneleRelIsoHcal = 0;
   ElectroneleRelIsoTrk = 0;
   ElectroneleRelIsoComb = 0;
   ElectroneleMissingHits = 0;
   ElectroneleDist = 0;
   ElectroneleDeltaCotTheta = 0;
   ElectroneleConvRadius = 0;
   ElectroneleMaxOver3x3 = 0;
/*    Electronele3x3 = 0; */
/*    ElectroneleMax = 0; */
/*    Electrone1x5 = 0; */
/*    Electrone2x5Max = 0; */
/*    Electrone5x5 = 0; */
/*    Electroneler1x5 = 0; */
/*    Electroneler2x5max = 0; */
/*    Electronscpreshowerenergy = 0; */
/*    Electronscetawidth = 0; */
/*    Electronscphiwidth = 0; */
   ElectroneleScEnergy = 0;
   ElectroneleScEta = 0;
   ElectroneleScEt = 0;
   Electroneleenergy = 0;
/*    ElectronelehcalDepth1OverEcal = 0; */
/*    ElectronelehcalDepth2OverEcal = 0; */
   ElectronelehcalOverEcal = 0;
/*    ElectronelesigmaEtaEta = 0; */
   ElectronelesigmaIetaIeta = 0;
/*    ElectronelebasicClustersSize = 0; */
/*    ElectronelenumberOfBrems = 0; */
/*    Electronelefbrem = 0; */
/*    ElectronelescPixCharge = 0; */
/*    Electronelectfcharge = 0; */
   Electronelecharge = 0;
/*    ElectroneleisGsfScPixChargeConsistent = 0; */
/*    ElectroneleisGsfCtfChargeConsistent = 0; */
/*    ElectroneleisGsfCtfScPixChargeConsistent = 0; */
/*    ElectronelevertexChi2 = 0; */
/*    ElectronelevertexNdof = 0; */
/*    ElectronelevertexNormalizedChi2 = 0; */
/*    Electronelevx = 0; */
/*    Electronelevy = 0; */
/*    Electronelevz = 0; */
/*    Electronelep = 0; */
   Electronelepx = 0;
   Electronelepy = 0;
   Electronelepz = 0;
   trigResults = 0;
   trigName = 0;
   hltEle10LWEt = 0;
   hltEle10LWPt = 0;
   hltEle10LWEta = 0;
   hltEle10LWPhi = 0;
   hltEle15LWEt = 0;
   hltEle15LWPt = 0;
   hltEle15LWEta = 0;
   hltEle15LWPhi = 0;
   hltEle20LWEt = 0;
   hltEle20LWPt = 0;
   hltEle20LWEta = 0;
   hltEle20LWPhi = 0;
   hltEle10SWEt = 0;
   hltEle10SWPt = 0;
   hltEle10SWEta = 0;
   hltEle10SWPhi = 0;
   hltEle15SWEt = 0;
   hltEle15SWPt = 0;
   hltEle15SWEta = 0;
   hltEle15SWPhi = 0;
   hltEle20SWEt = 0;
   hltEle20SWPt = 0;
   hltEle20SWEta = 0;
   hltEle20SWPhi = 0;
   hltEle25SWEt = 0;
   hltEle25SWPt = 0;
   hltEle25SWEta = 0;
   hltEle25SWPhi = 0;
   hltPhoton10Et = 0;
   hltPhoton10Pt = 0;
   hltPhoton10Eta = 0;
   hltPhoton10Phi = 0;
   hltPhoton15Et = 0;
   hltPhoton15Pt = 0;
   hltPhoton15Eta = 0;
   hltPhoton15Phi = 0;
   hltPhoton20Et = 0;
   hltPhoton20Pt = 0;
   hltPhoton20Eta = 0;
   hltPhoton20Phi = 0;
   hltPhoton15CleanEt = 0;
   hltPhoton15CleanPt = 0;
   hltPhoton15CleanEta = 0;
   hltPhoton15CleanPhi = 0;
   hltPhoton20CleanEt = 0;
   hltPhoton20CleanPt = 0;
   hltPhoton20CleanEta = 0;
   hltPhoton20CleanPhi = 0;
   hltPhoton30CleanEt = 0;
   hltPhoton30CleanPt = 0;
   hltPhoton30CleanEta = 0;
   hltPhoton30CleanPhi = 0;
   L1trigResults = 0;
   L1trigErrCode = 0;
   L1trigName = 0;
   l1IsoEleEt = 0;
   l1IsoEleEnergy = 0;
   l1IsoEleEta = 0;
   l1IsoElePhi = 0;
   l1NonIsoEleEt = 0;
   l1NonIsoEleEnergy = 0;
   l1NonIsoEleEta = 0;
   l1NonIsoElePhi = 0;
   ElectronelePassWp95SpikeCut = 0;
   ElectronelePassWp95ScEtCut = 0;
   ElectronelePassWp95MissHitsCut = 0;
   ElectronelePassWp95DistCut = 0;
   ElectronelePassWp95DCotThetaCut = 0;
   ElectronelePassWp95ConvRejCut = 0;
   ElectronelePassWp95RelIsoEcalCut = 0;
   ElectronelePassWp95RelIsoHcalCut = 0;
   ElectronelePassWp95RelIsoTrkCut = 0;
   ElectronelePassWp95RelIsoIndivCut = 0;
   ElectronelePassWp95RelIsoCombCut = 0;
   ElectronelePassWp95IsoCut = 0;
   ElectronelePassWp95SieieCut = 0;
   ElectronelePassWp95DphiinCut = 0;
   ElectronelePassWp95DetainCut = 0;
   ElectronelePassWp95HoeCut = 0;
   ElectronelePassWp95EidCut = 0;
   ElectronelePassWp95 = 0;
   
   #ifdef USE_SC
   scEBEnergy = 0;
   scEBEt = 0;
   scEBEta = 0;
   scEBPhi = 0;
   scEBRawEnergy = 0;
   scEBpreshowerEnergy = 0;
   scEBenergy1 = 0;
   scEBenergy2x2 = 0;
   scEBenergy3x3 = 0;
   scEBenergy1x5 = 0;
   scEBenergy2x5 = 0;
   scEBenergy5x5 = 0;
   scEBHoverE = 0;
   scEBx = 0;
   scEBy = 0;
   scEBz = 0;
   scEBetaWidth = 0;
   scEBphiWidth = 0;
   scEBsigmaetaeta = 0;
   scEBsigmaIetaIeta = 0;
   scEBsigmaphiphi = 0;
   scEBsigmaIphiIphi = 0;
   scEBsize = 0;
   scEBnBasicClusters = 0;
   scEEEnergy = 0;
   scEEEt = 0;
   scEEEta = 0;
   scEEPhi = 0;
   scEERawEnergy = 0;
   scEEpreshowerEnergy = 0;
   scEEenergy1 = 0;
   scEEenergy2x2 = 0;
   scEEenergy3x3 = 0;
   scEEenergy1x5 = 0;
   scEEenergy2x5 = 0;
   scEEenergy5x5 = 0;
   scEEHoverE = 0;
   scEEx = 0;
   scEEy = 0;
   scEEz = 0;
   scEEetaWidth = 0;
   scEEphiWidth = 0;
   scEEsigmaetaeta = 0;
   scEEsigmaIetaIeta = 0;
   scEEsigmaphiphi = 0;
   scEEsigmaIphiIphi = 0;
   scEEsize = 0;
   scEEnBasicClusters = 0;
  #endif
  
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("EvtInfo_EventNum", &EvtInfo_EventNum, &b_EvtInfo_EventNum);
   fChain->SetBranchAddress("EvtInfo_RunNum", &EvtInfo_RunNum, &b_EvtInfo_RunNum);
   fChain->SetBranchAddress("EvtInfo_LumiSection", &EvtInfo_LumiSection, &b_EvtInfo_LumiSection);
   fChain->SetBranchAddress("EvtInfo_BunchXing", &EvtInfo_BunchXing, &b_EvtInfo_BunchXing);
   fChain->SetBranchAddress("ElectronNum", &ElectronNum, &b_ElectronNum);
   fChain->SetBranchAddress("ElectronPt", &ElectronPt, &b_ElectronPt);
   fChain->SetBranchAddress("ElectronEta", &ElectronEta, &b_ElectronEta);
   fChain->SetBranchAddress("ElectronPhi", &ElectronPhi, &b_ElectronPhi);
   fChain->SetBranchAddress("ElectronEt", &ElectronEt, &b_ElectronEt);
   fChain->SetBranchAddress("ElectroneleEcalDriven", &ElectroneleEcalDriven, &b_ElectroneleEcalDriven);
   fChain->SetBranchAddress("ElectroneleTrkDriven", &ElectroneleTrkDriven, &b_ElectroneleTrkDriven);
/*    fChain->SetBranchAddress("ElectronDhElecClsTrkAtCalo", &ElectronDhElecClsTrkAtCalo, &b_ElectronDhElecClsTrkAtCalo); */
/*    fChain->SetBranchAddress("ElectronDhSeedClsTrkAtCalo", &ElectronDhSeedClsTrkAtCalo, &b_ElectronDhSeedClsTrkAtCalo); */
   fChain->SetBranchAddress("ElectronDhSuperClsTrkAtVtx", &ElectronDhSuperClsTrkAtVtx, &b_ElectronDhSuperClsTrkAtVtx);
/*    fChain->SetBranchAddress("ElectronDphiElecClsTrkAtCalo", &ElectronDphiElecClsTrkAtCalo, &b_ElectronDphiElecClsTrkAtCalo); */
/*    fChain->SetBranchAddress("ElectronDphiSeedClsTrkAtCalo", &ElectronDphiSeedClsTrkAtCalo, &b_ElectronDphiSeedClsTrkAtCalo); */
   fChain->SetBranchAddress("ElectronDphiSuperClsTrkAtVtx", &ElectronDphiSuperClsTrkAtVtx, &b_ElectronDphiSuperClsTrkAtVtx);
/*    fChain->SetBranchAddress("ElectronPositionXTrkAtVtx", &ElectronPositionXTrkAtVtx, &b_ElectronPositionXTrkAtVtx); */
/*    fChain->SetBranchAddress("ElectronPositionYTrkAtVtx", &ElectronPositionYTrkAtVtx, &b_ElectronPositionYTrkAtVtx); */
/*    fChain->SetBranchAddress("ElectronPositionZTrkAtVtx", &ElectronPositionZTrkAtVtx, &b_ElectronPositionZTrkAtVtx); */
/*    fChain->SetBranchAddress("ElectronMomentumXTrkAtVtx", &ElectronMomentumXTrkAtVtx, &b_ElectronMomentumXTrkAtVtx); */
/*    fChain->SetBranchAddress("ElectronMomentumYTrkAtVtx", &ElectronMomentumYTrkAtVtx, &b_ElectronMomentumYTrkAtVtx); */
/*    fChain->SetBranchAddress("ElectronMomentumZTrkAtVtx", &ElectronMomentumZTrkAtVtx, &b_ElectronMomentumZTrkAtVtx); */
/*    fChain->SetBranchAddress("ElectronPositionXTrkAtCalo", &ElectronPositionXTrkAtCalo, &b_ElectronPositionXTrkAtCalo); */
/*    fChain->SetBranchAddress("ElectronPositionYTrkAtCalo", &ElectronPositionYTrkAtCalo, &b_ElectronPositionYTrkAtCalo); */
/*    fChain->SetBranchAddress("ElectronPositionZTrkAtCalo", &ElectronPositionZTrkAtCalo, &b_ElectronPositionZTrkAtCalo); */
/*    fChain->SetBranchAddress("ElectronMomentumXTrkAtCalo", &ElectronMomentumXTrkAtCalo, &b_ElectronMomentumXTrkAtCalo); */
/*    fChain->SetBranchAddress("ElectronMomentumYTrkAtCalo", &ElectronMomentumYTrkAtCalo, &b_ElectronMomentumYTrkAtCalo); */
/*    fChain->SetBranchAddress("ElectronMomentumZTrkAtCalo", &ElectronMomentumZTrkAtCalo, &b_ElectronMomentumZTrkAtCalo); */
/*    fChain->SetBranchAddress("ElectroneEleClsOverPout", &ElectroneEleClsOverPout, &b_ElectroneEleClsOverPout); */
/*    fChain->SetBranchAddress("ElectroneSeedClsOverP", &ElectroneSeedClsOverP, &b_ElectroneSeedClsOverP); */
/*    fChain->SetBranchAddress("ElectroneSeedClsOverPout", &ElectroneSeedClsOverPout, &b_ElectroneSeedClsOverPout); */
/*    fChain->SetBranchAddress("ElectroneSuperClsOverP", &ElectroneSuperClsOverP, &b_ElectroneSuperClsOverP); */
/*    fChain->SetBranchAddress("ElectroneleMomErr", &ElectroneleMomErr, &b_ElectroneleMomErr); */
/*    fChain->SetBranchAddress("Electroneledr03EcalRecHitSumEt", &Electroneledr03EcalRecHitSumEt, &b_Electroneledr03EcalRecHitSumEt); */
/*    fChain->SetBranchAddress("Electroneledr03HcalDepth1TowerSumEt", &Electroneledr03HcalDepth1TowerSumEt, &b_Electroneledr03HcalDepth1TowerSumEt); */
/*    fChain->SetBranchAddress("Electroneledr03HcalDepth2TowerSumEt", &Electroneledr03HcalDepth2TowerSumEt, &b_Electroneledr03HcalDepth2TowerSumEt); */
/*    fChain->SetBranchAddress("Electroneledr03HcalTowerSumEt", &Electroneledr03HcalTowerSumEt, &b_Electroneledr03HcalTowerSumEt); */
/*    fChain->SetBranchAddress("Electroneledr03TkSumPt", &Electroneledr03TkSumPt, &b_Electroneledr03TkSumPt); */
/*    fChain->SetBranchAddress("Electroneledr04EcalRecHitSumEt", &Electroneledr04EcalRecHitSumEt, &b_Electroneledr04EcalRecHitSumEt); */
/*    fChain->SetBranchAddress("Electroneledr04HcalDepth1TowerSumEt", &Electroneledr04HcalDepth1TowerSumEt, &b_Electroneledr04HcalDepth1TowerSumEt); */
/*    fChain->SetBranchAddress("Electroneledr04HcalDepth2TowerSumEt", &Electroneledr04HcalDepth2TowerSumEt, &b_Electroneledr04HcalDepth2TowerSumEt); */
/*    fChain->SetBranchAddress("Electroneledr04HcalTowerSumEt", &Electroneledr04HcalTowerSumEt, &b_Electroneledr04HcalTowerSumEt); */
/*    fChain->SetBranchAddress("Electroneledr04TkSumPt", &Electroneledr04TkSumPt, &b_Electroneledr04TkSumPt); */
   fChain->SetBranchAddress("ElectroneleRelIsoEcal", &ElectroneleRelIsoEcal, &b_ElectroneleRelIsoEcal);
   fChain->SetBranchAddress("ElectroneleRelIsoHcal", &ElectroneleRelIsoHcal, &b_ElectroneleRelIsoHcal);
   fChain->SetBranchAddress("ElectroneleRelIsoTrk", &ElectroneleRelIsoTrk, &b_ElectroneleRelIsoTrk);
   fChain->SetBranchAddress("ElectroneleRelIsoComb", &ElectroneleRelIsoComb, &b_ElectroneleRelIsoComb);
   fChain->SetBranchAddress("ElectroneleMissingHits", &ElectroneleMissingHits, &b_ElectroneleMissingHits);
   fChain->SetBranchAddress("ElectroneleDist", &ElectroneleDist, &b_ElectroneleDist);
   fChain->SetBranchAddress("ElectroneleDeltaCotTheta", &ElectroneleDeltaCotTheta, &b_ElectroneleDeltaCotTheta);
   fChain->SetBranchAddress("ElectroneleConvRadius", &ElectroneleConvRadius, &b_ElectroneleConvRadius);
   fChain->SetBranchAddress("ElectroneleMaxOver3x3", &ElectroneleMaxOver3x3, &b_ElectroneleMaxOver3x3);
/*    fChain->SetBranchAddress("Electronele3x3", &Electronele3x3, &b_Electronele3x3); */
/*    fChain->SetBranchAddress("ElectroneleMax", &ElectroneleMax, &b_ElectroneleMax); */
/*    fChain->SetBranchAddress("Electrone1x5", &Electrone1x5, &b_Electrone1x5); */
/*    fChain->SetBranchAddress("Electrone2x5Max", &Electrone2x5Max, &b_Electrone2x5Max); */
/*    fChain->SetBranchAddress("Electrone5x5", &Electrone5x5, &b_Electrone5x5); */
/*    fChain->SetBranchAddress("Electroneler1x5", &Electroneler1x5, &b_Electroneler1x5); */
/*    fChain->SetBranchAddress("Electroneler2x5max", &Electroneler2x5max, &b_Electroneler2x5max); */
/*    fChain->SetBranchAddress("Electronscpreshowerenergy", &Electronscpreshowerenergy, &b_Electronscpreshowerenergy); */
/*    fChain->SetBranchAddress("Electronscetawidth", &Electronscetawidth, &b_Electronscetawidth); */
/*    fChain->SetBranchAddress("Electronscphiwidth", &Electronscphiwidth, &b_Electronscphiwidth); */
   fChain->SetBranchAddress("ElectroneleScEnergy", &ElectroneleScEnergy, &b_ElectroneleScEnergy);
   fChain->SetBranchAddress("ElectroneleScEta", &ElectroneleScEta, &b_ElectroneleScEta);
   fChain->SetBranchAddress("ElectroneleScEt", &ElectroneleScEt, &b_ElectroneleScEt);
   fChain->SetBranchAddress("Electroneleenergy", &Electroneleenergy, &b_Electroneleenergy);
/*    fChain->SetBranchAddress("ElectronelehcalDepth1OverEcal", &ElectronelehcalDepth1OverEcal, &b_ElectronelehcalDepth1OverEcal); */
/*    fChain->SetBranchAddress("ElectronelehcalDepth2OverEcal", &ElectronelehcalDepth2OverEcal, &b_ElectronelehcalDepth2OverEcal); */
   fChain->SetBranchAddress("ElectronelehcalOverEcal", &ElectronelehcalOverEcal, &b_ElectronelehcalOverEcal);
/*    fChain->SetBranchAddress("ElectronelesigmaEtaEta", &ElectronelesigmaEtaEta, &b_ElectronelesigmaEtaEta); */
   fChain->SetBranchAddress("ElectronelesigmaIetaIeta", &ElectronelesigmaIetaIeta, &b_ElectronelesigmaIetaIeta);
/*    fChain->SetBranchAddress("ElectronelebasicClustersSize", &ElectronelebasicClustersSize, &b_ElectronelebasicClustersSize); */
/*    fChain->SetBranchAddress("ElectronelenumberOfBrems", &ElectronelenumberOfBrems, &b_ElectronelenumberOfBrems); */
/*    fChain->SetBranchAddress("Electronelefbrem", &Electronelefbrem, &b_Electronelefbrem); */
/*    fChain->SetBranchAddress("ElectronelescPixCharge", &ElectronelescPixCharge, &b_ElectronelescPixCharge); */
/*    fChain->SetBranchAddress("Electronelectfcharge", &Electronelectfcharge, &b_Electronelectfcharge); */
   fChain->SetBranchAddress("Electronelecharge", &Electronelecharge, &b_Electronelecharge);
/*    fChain->SetBranchAddress("ElectroneleisGsfScPixChargeConsistent", &ElectroneleisGsfScPixChargeConsistent, &b_ElectroneleisGsfScPixChargeConsistent); */
/*    fChain->SetBranchAddress("ElectroneleisGsfCtfChargeConsistent", &ElectroneleisGsfCtfChargeConsistent, &b_ElectroneleisGsfCtfChargeConsistent); */
/*    fChain->SetBranchAddress("ElectroneleisGsfCtfScPixChargeConsistent", &ElectroneleisGsfCtfScPixChargeConsistent, &b_ElectroneleisGsfCtfScPixChargeConsistent); */
/*    fChain->SetBranchAddress("ElectronelevertexChi2", &ElectronelevertexChi2, &b_ElectronelevertexChi2); */
/*    fChain->SetBranchAddress("ElectronelevertexNdof", &ElectronelevertexNdof, &b_ElectronelevertexNdof); */
/*    fChain->SetBranchAddress("ElectronelevertexNormalizedChi2", &ElectronelevertexNormalizedChi2, &b_ElectronelevertexNormalizedChi2); */
/*    fChain->SetBranchAddress("Electronelevx", &Electronelevx, &b_Electronelevx); */
/*    fChain->SetBranchAddress("Electronelevy", &Electronelevy, &b_Electronelevy); */
/*    fChain->SetBranchAddress("Electronelevz", &Electronelevz, &b_Electronelevz); */
/*    fChain->SetBranchAddress("Electronelep", &Electronelep, &b_Electronelep); */
   fChain->SetBranchAddress("Electronelepx", &Electronelepx, &b_Electronelepx);
   fChain->SetBranchAddress("Electronelepy", &Electronelepy, &b_Electronelepy);
   fChain->SetBranchAddress("Electronelepz", &Electronelepz, &b_Electronelepz);
   fChain->SetBranchAddress("trigResults", &trigResults, &b_trigResults);
   fChain->SetBranchAddress("trigName", &trigName, &b_trigName);
   fChain->SetBranchAddress("hltEle10LWEt", &hltEle10LWEt, &b_hltEle10LWEt);
   fChain->SetBranchAddress("hltEle10LWPt", &hltEle10LWPt, &b_hltEle10LWPt);
   fChain->SetBranchAddress("hltEle10LWEta", &hltEle10LWEta, &b_hltEle10LWEta);
   fChain->SetBranchAddress("hltEle10LWPhi", &hltEle10LWPhi, &b_hltEle10LWPhi);
   fChain->SetBranchAddress("hltEle15LWEt", &hltEle15LWEt, &b_hltEle15LWEt);
   fChain->SetBranchAddress("hltEle15LWPt", &hltEle15LWPt, &b_hltEle15LWPt);
   fChain->SetBranchAddress("hltEle15LWEta", &hltEle15LWEta, &b_hltEle15LWEta);
   fChain->SetBranchAddress("hltEle15LWPhi", &hltEle15LWPhi, &b_hltEle15LWPhi);
   fChain->SetBranchAddress("hltEle20LWEt", &hltEle20LWEt, &b_hltEle20LWEt);
   fChain->SetBranchAddress("hltEle20LWPt", &hltEle20LWPt, &b_hltEle20LWPt);
   fChain->SetBranchAddress("hltEle20LWEta", &hltEle20LWEta, &b_hltEle20LWEta);
   fChain->SetBranchAddress("hltEle20LWPhi", &hltEle20LWPhi, &b_hltEle20LWPhi);
   fChain->SetBranchAddress("hltEle10SWEt", &hltEle10SWEt, &b_hltEle10SWEt);
   fChain->SetBranchAddress("hltEle10SWPt", &hltEle10SWPt, &b_hltEle10SWPt);
   fChain->SetBranchAddress("hltEle10SWEta", &hltEle10SWEta, &b_hltEle10SWEta);
   fChain->SetBranchAddress("hltEle10SWPhi", &hltEle10SWPhi, &b_hltEle10SWPhi);
   fChain->SetBranchAddress("hltEle15SWEt", &hltEle15SWEt, &b_hltEle15SWEt);
   fChain->SetBranchAddress("hltEle15SWPt", &hltEle15SWPt, &b_hltEle15SWPt);
   fChain->SetBranchAddress("hltEle15SWEta", &hltEle15SWEta, &b_hltEle15SWEta);
   fChain->SetBranchAddress("hltEle15SWPhi", &hltEle15SWPhi, &b_hltEle15SWPhi);
   fChain->SetBranchAddress("hltEle20SWEt", &hltEle20SWEt, &b_hltEle20SWEt);
   fChain->SetBranchAddress("hltEle20SWPt", &hltEle20SWPt, &b_hltEle20SWPt);
   fChain->SetBranchAddress("hltEle20SWEta", &hltEle20SWEta, &b_hltEle20SWEta);
   fChain->SetBranchAddress("hltEle20SWPhi", &hltEle20SWPhi, &b_hltEle20SWPhi);
   fChain->SetBranchAddress("hltEle25SWEt", &hltEle25SWEt, &b_hltEle25SWEt);
   fChain->SetBranchAddress("hltEle25SWPt", &hltEle25SWPt, &b_hltEle25SWPt);
   fChain->SetBranchAddress("hltEle25SWEta", &hltEle25SWEta, &b_hltEle25SWEta);
   fChain->SetBranchAddress("hltEle25SWPhi", &hltEle25SWPhi, &b_hltEle25SWPhi);
   fChain->SetBranchAddress("hltPhoton10Et", &hltPhoton10Et, &b_hltPhoton10Et);
   fChain->SetBranchAddress("hltPhoton10Pt", &hltPhoton10Pt, &b_hltPhoton10Pt);
   fChain->SetBranchAddress("hltPhoton10Eta", &hltPhoton10Eta, &b_hltPhoton10Eta);
   fChain->SetBranchAddress("hltPhoton10Phi", &hltPhoton10Phi, &b_hltPhoton10Phi);
   fChain->SetBranchAddress("hltPhoton15Et", &hltPhoton15Et, &b_hltPhoton15Et);
   fChain->SetBranchAddress("hltPhoton15Pt", &hltPhoton15Pt, &b_hltPhoton15Pt);
   fChain->SetBranchAddress("hltPhoton15Eta", &hltPhoton15Eta, &b_hltPhoton15Eta);
   fChain->SetBranchAddress("hltPhoton15Phi", &hltPhoton15Phi, &b_hltPhoton15Phi);
   fChain->SetBranchAddress("hltPhoton20Et", &hltPhoton20Et, &b_hltPhoton20Et);
   fChain->SetBranchAddress("hltPhoton20Pt", &hltPhoton20Pt, &b_hltPhoton20Pt);
   fChain->SetBranchAddress("hltPhoton20Eta", &hltPhoton20Eta, &b_hltPhoton20Eta);
   fChain->SetBranchAddress("hltPhoton20Phi", &hltPhoton20Phi, &b_hltPhoton20Phi);
   fChain->SetBranchAddress("hltPhoton15CleanEt", &hltPhoton15CleanEt, &b_hltPhoton15CleanEt);
   fChain->SetBranchAddress("hltPhoton15CleanPt", &hltPhoton15CleanPt, &b_hltPhoton15CleanPt);
   fChain->SetBranchAddress("hltPhoton15CleanEta", &hltPhoton15CleanEta, &b_hltPhoton15CleanEta);
   fChain->SetBranchAddress("hltPhoton15CleanPhi", &hltPhoton15CleanPhi, &b_hltPhoton15CleanPhi);
   fChain->SetBranchAddress("hltPhoton20CleanEt", &hltPhoton20CleanEt, &b_hltPhoton20CleanEt);
   fChain->SetBranchAddress("hltPhoton20CleanPt", &hltPhoton20CleanPt, &b_hltPhoton20CleanPt);
   fChain->SetBranchAddress("hltPhoton20CleanEta", &hltPhoton20CleanEta, &b_hltPhoton20CleanEta);
   fChain->SetBranchAddress("hltPhoton20CleanPhi", &hltPhoton20CleanPhi, &b_hltPhoton20CleanPhi);
   fChain->SetBranchAddress("hltPhoton30CleanEt", &hltPhoton30CleanEt, &b_hltPhoton30CleanEt);
   fChain->SetBranchAddress("hltPhoton30CleanPt", &hltPhoton30CleanPt, &b_hltPhoton30CleanPt);
   fChain->SetBranchAddress("hltPhoton30CleanEta", &hltPhoton30CleanEta, &b_hltPhoton30CleanEta);
   fChain->SetBranchAddress("hltPhoton30CleanPhi", &hltPhoton30CleanPhi, &b_hltPhoton30CleanPhi);
   fChain->SetBranchAddress("L1trigResults", &L1trigResults, &b_L1trigResults);
   fChain->SetBranchAddress("L1trigErrCode", &L1trigErrCode, &b_L1trigErrCode);
   fChain->SetBranchAddress("L1trigName", &L1trigName, &b_L1trigName);
   fChain->SetBranchAddress("l1IsoEleEt", &l1IsoEleEt, &b_l1IsoEleEt);
   fChain->SetBranchAddress("l1IsoEleEnergy", &l1IsoEleEnergy, &b_l1IsoEleEnergy);
   fChain->SetBranchAddress("l1IsoEleEta", &l1IsoEleEta, &b_l1IsoEleEta);
   fChain->SetBranchAddress("l1IsoElePhi", &l1IsoElePhi, &b_l1IsoElePhi);
   fChain->SetBranchAddress("l1NonIsoEleEt", &l1NonIsoEleEt, &b_l1NonIsoEleEt);
   fChain->SetBranchAddress("l1NonIsoEleEnergy", &l1NonIsoEleEnergy, &b_l1NonIsoEleEnergy);
   fChain->SetBranchAddress("l1NonIsoEleEta", &l1NonIsoEleEta, &b_l1NonIsoEleEta);
   fChain->SetBranchAddress("l1NonIsoElePhi", &l1NonIsoElePhi, &b_l1NonIsoElePhi);
   fChain->SetBranchAddress("ElectronelePassWp95SpikeCut", &ElectronelePassWp95SpikeCut, &b_ElectronelePassWp95SpikeCut);
   fChain->SetBranchAddress("ElectronelePassWp95ScEtCut", &ElectronelePassWp95ScEtCut, &b_ElectronelePassWp95ScEtCut);
   fChain->SetBranchAddress("ElectronelePassWp95MissHitsCut", &ElectronelePassWp95MissHitsCut, &b_ElectronelePassWp95MissHitsCut);
   fChain->SetBranchAddress("ElectronelePassWp95DistCut", &ElectronelePassWp95DistCut, &b_ElectronelePassWp95DistCut);
   fChain->SetBranchAddress("ElectronelePassWp95DCotThetaCut", &ElectronelePassWp95DCotThetaCut, &b_ElectronelePassWp95DCotThetaCut);
   fChain->SetBranchAddress("ElectronelePassWp95ConvRejCut", &ElectronelePassWp95ConvRejCut, &b_ElectronelePassWp95ConvRejCut);
   fChain->SetBranchAddress("ElectronelePassWp95RelIsoEcalCut", &ElectronelePassWp95RelIsoEcalCut, &b_ElectronelePassWp95RelIsoEcalCut);
   fChain->SetBranchAddress("ElectronelePassWp95RelIsoHcalCut", &ElectronelePassWp95RelIsoHcalCut, &b_ElectronelePassWp95RelIsoHcalCut);
   fChain->SetBranchAddress("ElectronelePassWp95RelIsoTrkCut", &ElectronelePassWp95RelIsoTrkCut, &b_ElectronelePassWp95RelIsoTrkCut);
   fChain->SetBranchAddress("ElectronelePassWp95RelIsoIndivCut", &ElectronelePassWp95RelIsoIndivCut, &b_ElectronelePassWp95RelIsoIndivCut);
   fChain->SetBranchAddress("ElectronelePassWp95RelIsoCombCut", &ElectronelePassWp95RelIsoCombCut, &b_ElectronelePassWp95RelIsoCombCut);
   fChain->SetBranchAddress("ElectronelePassWp95IsoCut", &ElectronelePassWp95IsoCut, &b_ElectronelePassWp95IsoCut);
   fChain->SetBranchAddress("ElectronelePassWp95SieieCut", &ElectronelePassWp95SieieCut, &b_ElectronelePassWp95SieieCut);
   fChain->SetBranchAddress("ElectronelePassWp95DphiinCut", &ElectronelePassWp95DphiinCut, &b_ElectronelePassWp95DphiinCut);
   fChain->SetBranchAddress("ElectronelePassWp95DetainCut", &ElectronelePassWp95DetainCut, &b_ElectronelePassWp95DetainCut);
   fChain->SetBranchAddress("ElectronelePassWp95HoeCut", &ElectronelePassWp95HoeCut, &b_ElectronelePassWp95HoeCut);
   fChain->SetBranchAddress("ElectronelePassWp95EidCut", &ElectronelePassWp95EidCut, &b_ElectronelePassWp95EidCut);
   fChain->SetBranchAddress("ElectronelePassWp95", &ElectronelePassWp95, &b_ElectronelePassWp95);

   #ifdef USE_SC
   fChain->SetBranchAddress("scEBNum", &scEBNum, &b_scEBNum);
   fChain->SetBranchAddress("scEBEnergy", &scEBEnergy, &b_scEBEnergy);
   fChain->SetBranchAddress("scEBEt", &scEBEt, &b_scEBEt);
   fChain->SetBranchAddress("scEBEta", &scEBEta, &b_scEBEta);
   fChain->SetBranchAddress("scEBPhi", &scEBPhi, &b_scEBPhi);
   fChain->SetBranchAddress("scEBRawEnergy", &scEBRawEnergy, &b_scEBRawEnergy);
   fChain->SetBranchAddress("scEBpreshowerEnergy", &scEBpreshowerEnergy, &b_scEBpreshowerEnergy);
   fChain->SetBranchAddress("scEBenergy1", &scEBenergy1, &b_scEBenergy1);
   fChain->SetBranchAddress("scEBenergy2x2", &scEBenergy2x2, &b_scEBenergy2x2);
   fChain->SetBranchAddress("scEBenergy3x3", &scEBenergy3x3, &b_scEBenergy3x3);
   fChain->SetBranchAddress("scEBenergy1x5", &scEBenergy1x5, &b_scEBenergy1x5);
   fChain->SetBranchAddress("scEBenergy2x5", &scEBenergy2x5, &b_scEBenergy2x5);
   fChain->SetBranchAddress("scEBenergy5x5", &scEBenergy5x5, &b_scEBenergy5x5);
   fChain->SetBranchAddress("scEBHoverE", &scEBHoverE, &b_scEBHoverE);
   fChain->SetBranchAddress("scEBx", &scEBx, &b_scEBx);
   fChain->SetBranchAddress("scEBy", &scEBy, &b_scEBy);
   fChain->SetBranchAddress("scEBz", &scEBz, &b_scEBz);
   fChain->SetBranchAddress("scEBetaWidth", &scEBetaWidth, &b_scEBetaWidth);
   fChain->SetBranchAddress("scEBphiWidth", &scEBphiWidth, &b_scEBphiWidth);
   fChain->SetBranchAddress("scEBsigmaetaeta", &scEBsigmaetaeta, &b_scEBsigmaetaeta);
   fChain->SetBranchAddress("scEBsigmaIetaIeta", &scEBsigmaIetaIeta, &b_scEBsigmaIetaIeta);
   fChain->SetBranchAddress("scEBsigmaphiphi", &scEBsigmaphiphi, &b_scEBsigmaphiphi);
   fChain->SetBranchAddress("scEBsigmaIphiIphi", &scEBsigmaIphiIphi, &b_scEBsigmaIphiIphi);
   fChain->SetBranchAddress("scEBsize", &scEBsize, &b_scEBsize);
   fChain->SetBranchAddress("scEBnBasicClusters", &scEBnBasicClusters, &b_scEBnBasicClusters);
   fChain->SetBranchAddress("scEENum", &scEENum, &b_scEENum);
   fChain->SetBranchAddress("scEEEnergy", &scEEEnergy, &b_scEEEnergy);
   fChain->SetBranchAddress("scEEEt", &scEEEt, &b_scEEEt);
   fChain->SetBranchAddress("scEEEta", &scEEEta, &b_scEEEta);
   fChain->SetBranchAddress("scEEPhi", &scEEPhi, &b_scEEPhi);
   fChain->SetBranchAddress("scEERawEnergy", &scEERawEnergy, &b_scEERawEnergy);
   fChain->SetBranchAddress("scEEpreshowerEnergy", &scEEpreshowerEnergy, &b_scEEpreshowerEnergy);
   fChain->SetBranchAddress("scEEenergy1", &scEEenergy1, &b_scEEenergy1);
   fChain->SetBranchAddress("scEEenergy2x2", &scEEenergy2x2, &b_scEEenergy2x2);
   fChain->SetBranchAddress("scEEenergy3x3", &scEEenergy3x3, &b_scEEenergy3x3);
   fChain->SetBranchAddress("scEEenergy1x5", &scEEenergy1x5, &b_scEEenergy1x5);
   fChain->SetBranchAddress("scEEenergy2x5", &scEEenergy2x5, &b_scEEenergy2x5);
   fChain->SetBranchAddress("scEEenergy5x5", &scEEenergy5x5, &b_scEEenergy5x5);
   fChain->SetBranchAddress("scEEHoverE", &scEEHoverE, &b_scEEHoverE);
   fChain->SetBranchAddress("scEEx", &scEEx, &b_scEEx);
   fChain->SetBranchAddress("scEEy", &scEEy, &b_scEEy);
   fChain->SetBranchAddress("scEEz", &scEEz, &b_scEEz);
   fChain->SetBranchAddress("scEEetaWidth", &scEEetaWidth, &b_scEEetaWidth);
   fChain->SetBranchAddress("scEEphiWidth", &scEEphiWidth, &b_scEEphiWidth);
   fChain->SetBranchAddress("scEEsigmaetaeta", &scEEsigmaetaeta, &b_scEEsigmaetaeta);
   fChain->SetBranchAddress("scEEsigmaIetaIeta", &scEEsigmaIetaIeta, &b_scEEsigmaIetaIeta);
   fChain->SetBranchAddress("scEEsigmaphiphi", &scEEsigmaphiphi, &b_scEEsigmaphiphi);
   fChain->SetBranchAddress("scEEsigmaIphiIphi", &scEEsigmaIphiIphi, &b_scEEsigmaIphiIphi);
   fChain->SetBranchAddress("scEEsize", &scEEsize, &b_scEEsize);
   fChain->SetBranchAddress("scEEnBasicClusters", &scEEnBasicClusters, &b_scEEnBasicClusters);
   #endif
   
   Notify();
}

Bool_t ZeePlots::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}


class electron
{
  public:
    electron(ZeePlots * const trig,  unsigned int pos)
    {
      pos_    = pos;
      charge  = trig->Electronelecharge->at(pos);
      pt      = trig->ElectronPt->at(pos);
      px      = trig->Electronelepx->at(pos);
      py      = trig->Electronelepy->at(pos);
      pz      = trig->Electronelepz->at(pos);
      et      = trig->ElectronEt->at(pos);
      energy  = trig->Electroneleenergy->at(pos);
      eta     = trig->ElectronEta->at(pos);
      phi     = trig->ElectronPhi->at(pos);
      sigIeIe = trig->ElectronelesigmaIetaIeta->at(pos);
      hoE     = trig->ElectronelehcalOverEcal->at(pos);
      dEtaIn  = trig->ElectronDhSuperClsTrkAtVtx->at(pos) ;
      dPhiIn  = trig->ElectronDphiSuperClsTrkAtVtx->at(pos) ;
      scEta   = trig->ElectroneleScEta->at(pos);
      scEnergy = trig->ElectroneleScEnergy->at(pos);
      scEt    = trig->ElectroneleScEt->at(pos);
      ecalDrv = trig->ElectroneleEcalDriven->at(pos);
      trkDrv  = trig->ElectroneleTrkDriven->at(pos);
      missHits = trig->ElectroneleMissingHits->at(pos);
      dist    = trig->ElectroneleDist->at(pos);
      dCotTheta = trig->ElectroneleDeltaCotTheta->at(pos);
      closestL1 = 999;
      closestL1dr  = 50;
      closestL1deta = 50;
      closestL1dphi = 50;

      where = (fabs( eta ) < 1.4442 ? "EB" : "EE");
      
      findClosestL1(trig);
    }
    
    
    unsigned int Pos() { return pos_; }
    double Charge() { return charge; }
    double Pt() { return pt; }
    double Px() { return px; }
    double Py() { return py; }
    double Pz() { return pz; }
    double Et() { return et; }
    double Energy() { return energy; }
    double Eta() { return eta; }
    double Phi() { return phi; }
    double SigIeIe() { return sigIeIe; }
    double HoE() { return hoE; }
    double DeltaEtaIn() { return dEtaIn; }
    double DeltaPhiIn() { return dPhiIn; }
    double ClosestL1() { return closestL1; }
    double ClosestL1En() { return closestL1Energy; }
    double ClosestL1Et() { return closestL1Et; }
    double ClosestL1IsIso() { return l1isol; }
    double ClosestL1dr() { return closestL1dr; }
    double ClosestL1deta() { return closestL1dphi; }
    double ClosestL1dphi() { return closestL1deta; }
    double ScEta() { return scEta; }
    double ScEnergy() { return scEnergy; }
    double ScEt() { return scEt; }
    bool   isEcalDriven() { return ecalDrv; }
    bool   isTrkDriven() { return trkDrv; }
    double MissHits() { return missHits; }
    double Dist() { return dist; }
    double DCotTheta() { return dCotTheta; }
    string Where() { return where; }

  private:
    electron();
    unsigned int pos_;
    double charge;
    double pt;
    double px;
    double py;
    double pz;
    double et;
    double energy;
    double eta;
    double phi;
    double sigIeIe;
    double hoE;
    double dEtaIn;
    double dPhiIn;
    unsigned int closestL1;
    double closestL1dr;
    double closestL1Energy;
    double closestL1Et;
    double closestL1deta;
    double closestL1dphi;
    double scEta;
    double scEnergy;
    double scEt;
    bool ecalDrv;
    bool trkDrv;
    bool l1isol;
    unsigned int missHits;
    double dist;
    double dCotTheta;
    string where;

    double dr;
    void findClosestL1(ZeePlots * const trig)
    {
      for ( unsigned int i = 0; i < trig->l1IsoEleEnergy->size(); ++i )
      {
        dr = deltaR(trig->l1IsoEleEta->at(i), trig->l1IsoElePhi->at(i), eta, phi);
        if ( dr < closestL1dr )
        {
            closestL1 = i;
            closestL1dr = dr;
            closestL1Energy = trig->l1IsoEleEnergy->at(i);
            closestL1Et = trig->l1IsoEleEt->at(i);
            closestL1deta = fabs(trig->l1IsoEleEta->at(i) - eta);
            closestL1dphi = deltaPhi(trig->l1IsoElePhi->at(i), phi);
            l1isol = true;
        }
      }

      for ( unsigned int i = 0; i < trig->l1NonIsoEleEnergy->size(); ++i )
      {
        dr = deltaR(trig->l1NonIsoEleEta->at(i), trig->l1NonIsoElePhi->at(i), eta, phi);
        if ( dr < closestL1dr )
        {
            closestL1 = i;
            closestL1dr = dr;
            closestL1Energy = trig->l1NonIsoEleEnergy->at(i);
            closestL1Et = trig->l1NonIsoEleEt->at(i);
            closestL1deta = fabs(trig->l1NonIsoEleEta->at(i) - eta);
            closestL1dphi = deltaPhi(trig->l1NonIsoElePhi->at(i), phi);
            l1isol = false;
        }
      }
    }
};


class Zcand
{
 public:
  Zcand( ZeePlots * const trig,  unsigned int e1, unsigned int e2)
    {
      index1 = e1;
      index2 = e2;

      elec1 = XYZTLorentzVector(trig->recoElectronCollection.at(e1).Px(),
				trig->recoElectronCollection.at(e1).Py(),
				trig->recoElectronCollection.at(e1).Pz(),
				trig->recoElectronCollection.at(e1).ScEnergy());
      elec2 = XYZTLorentzVector(trig->recoElectronCollection.at(e2).Px(),
				trig->recoElectronCollection.at(e2).Py(),
				trig->recoElectronCollection.at(e2).Pz(),
				trig->recoElectronCollection.at(e2).ScEnergy());
      ZcandP4 = elec1 + elec2;
      pt = ZcandP4.pt();
      eta = ZcandP4.eta();
      rapidity = ZcandP4.Rapidity();
      phi = ZcandP4.phi();
      mass = ZcandP4.M();

      if (trig->recoElectronCollection.at(e1).Where().compare("EB") && trig->recoElectronCollection.at(e2).Where().compare("EB"))
	{
	  correctedMass = mass * 1.0115;
	}
      else if ((trig->recoElectronCollection.at(e1).Where().compare("EE") && trig->recoElectronCollection.at(e2).Where().compare("EB"))
	       || (trig->recoElectronCollection.at(e1).Where().compare("EB") && trig->recoElectronCollection.at(e2).Where().compare("EE")))
	{
	  correctedMass = mass * sqrt(1.0115*1.0292);
	}
      else if (trig->recoElectronCollection.at(e1).Where().compare("EE") && trig->recoElectronCollection.at(e2).Where().compare("EE"))
	{
	  correctedMass = mass * 1.0292;
	}
      else 
	{
	  std::cout << "Electron(s) not EB or EE! Should not happen! elec1: " << trig->recoElectronCollection.at(e1).Where()
		    << "  elec2: " << trig->recoElectronCollection.at(e2).Where() << std::endl;
	  correctedMass = 9999.;
	}

      if (trig->recoElectronCollection.at(e1).Charge() != trig->recoElectronCollection.at(e2).Charge() ) productsAreOppSign = true;
      else productsAreOppSign = false;

    }

  double Pt() { return pt; }
  double Eta() { return eta; }
  double Rapidity() { return rapidity; }
  double Phi() { return phi; }
  double Mass() { return mass; }
  double CorrectedMass() { return correctedMass; }
  bool ProductsAreOppSign() { return productsAreOppSign; }
  
 private:
  Zcand();
  unsigned int index1;
  unsigned int index2;
  double pt;
  double eta;
  double rapidity;
  double phi;
  double mass;
  double correctedMass;
  bool productsAreOppSign;
/*   math::XYZTLorentzVector elec1; */
/*   math::XYZTLorentzVector elec2; */
/*   math::XYZTLorentzVector ZcandP4; */
  XYZTLorentzVector elec1;
  XYZTLorentzVector elec2;
  XYZTLorentzVector ZcandP4;
/*   XYZTVector elec1; */
/*   XYZTVector elec2; */
/*   XYZTVector ZcandP4; */
};




