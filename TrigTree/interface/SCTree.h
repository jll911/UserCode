#ifndef sctree_h
#define sctree_h_

#include<iostream>
#include<string>
#include<vector>

#include "TTree.h"
#include "FWCore/Framework/interface/Event.h" 
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

class SCTree{

 public:
  SCTree(std::string name,TTree* tree,const edm::ParameterSet&);
  ~SCTree();
  void Fill(const edm::Event& iEvent,const edm::EventSetup& iSetup);
  void Clear();
 private:
  SCTree(){};
  void SetBranches();
  void AddBranch(double* x, std::string name);
  void AddBranch(std::vector<double>*, std::string name);
  
  std::string name_;
  TTree* tree_;
  double nscEB_;
  std::vector<double> scEBenergy_;
  std::vector<double> scEBet_;
  std::vector<double> scEBeta_;
  std::vector<double> scEBphi_;
  std::vector<double> scEBrawenergy_;
  std::vector<double> scEBpreshowerEnergy_; 
  std::vector<double> scEBenergy1_; 
  std::vector<double> scEBenergy2x2_; 
  std::vector<double> scEBenergy3x3_; 
  std::vector<double> scEBenergy1x5_; 
  std::vector<double> scEBenergy2x5_; 
  std::vector<double> scEBenergy5x5_; 
  std::vector<double> scEBHoverE_; 
  std::vector<double> scEBx_; 
  std::vector<double> scEBy_; 
  std::vector<double> scEBz_; 
  std::vector<double> scEBetaWidth_; 
  std::vector<double> scEBphiWidth_; 
  std::vector<double> scEBsigmaetaeta_;
  std::vector<double> scEBsigmaIetaIeta_;
  std::vector<double> scEBsigmaphiphi_;
  std::vector<double> scEBsigmaIphiIphi_;
  std::vector<double> scEBsize_;
  std::vector<double> scEBnBasicClusters_;

  double nscEE_;
  std::vector<double> scEEenergy_;
  std::vector<double> scEEet_;
  std::vector<double> scEEeta_;
  std::vector<double> scEEphi_;
  std::vector<double> scEErawenergy_;
  std::vector<double> scEEpreshowerEnergy_; 
  std::vector<double> scEEenergy1_; 
  std::vector<double> scEEenergy2x2_; 
  std::vector<double> scEEenergy3x3_; 
  std::vector<double> scEEenergy1x5_; 
  std::vector<double> scEEenergy2x5_; 
  std::vector<double> scEEenergy5x5_; 
  std::vector<double> scEEHoverE_; 
  std::vector<double> scEEx_; 
  std::vector<double> scEEy_; 
  std::vector<double> scEEz_; 
  std::vector<double> scEEetaWidth_; 
  std::vector<double> scEEphiWidth_; 
  std::vector<double> scEEsigmaetaeta_;
  std::vector<double> scEEsigmaIetaIeta_;
  std::vector<double> scEEsigmaphiphi_;
  std::vector<double> scEEsigmaIphiIphi_;
  std::vector<double> scEEsize_;
  std::vector<double> scEEnBasicClusters_;

  edm::InputTag recHitsEBTag_;
  edm::InputTag recHitsEETag_;

};



#endif
