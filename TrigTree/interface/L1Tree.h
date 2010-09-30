#ifndef l1tree_h
#define l1tree_h

#include<iostream>
#include<vector>
#include<string>
#include "TTree.h"
#include "FWCore/Framework/interface/Event.h" 
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "L1Trigger/GlobalTriggerAnalyzer/interface/L1GtUtils.h"

class L1Tree{

 public:
  L1Tree(std::string name,TTree* tree);
  void Fill(const edm::Event& iEvent,const edm::EventSetup& iSetup);
  void Clear();
 private:
  L1Tree(){};
  L1GtUtils m_l1GtUtils;
  
  std::string name_;
  TTree* tree_;
  std::vector<int> L1trigResult_;
  std::vector<int> L1trigErrCode_;
  std::vector<std::string> L1trigName_;

  std::vector<double> l1IsoEleEt_;
  std::vector<double> l1IsoEleEnergy_;
  std::vector<double> l1IsoEleEta_;
  std::vector<double> l1IsoElePhi_;
        
  std::vector<double> l1NonIsoEleEt_;
  std::vector<double> l1NonIsoEleEnergy_;
  std::vector<double> l1NonIsoEleEta_;
  std::vector<double> l1NonIsoElePhi_;
};

#endif
