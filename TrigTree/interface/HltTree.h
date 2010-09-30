#ifndef hlttree_h
#define hlttree_h

#include<iostream>
#include<string>
#include<vector>

#include "TTree.h"
#include "FWCore/Framework/interface/Event.h" 
#include "FWCore/Framework/interface/Frameworkfwd.h"

class HltTree{

 public:
  HltTree(std::string name, edm::InputTag trigTag, edm::InputTag trigEventTag, TTree* tree);
  void Fill(const edm::Event& iEvent);
  void Clear();
 private:
  HltTree(){};
 
  edm::InputTag trigTag_;
  edm::InputTag trigEventTag_;
  std::string name_;
  TTree* tree_;
  std::vector<int> trigResult_;
  std::vector<std::string> trigName_;
  
  std::vector<double> hltEle10LWEt_;
  std::vector<double> hltEle10LWPt_;
  std::vector<double> hltEle10LWEta_;
  std::vector<double> hltEle10LWPhi_;

  std::vector<double> hltEle15LWEt_;
  std::vector<double> hltEle15LWPt_;
  std::vector<double> hltEle15LWEta_;
  std::vector<double> hltEle15LWPhi_;

  std::vector<double> hltEle20LWEt_;
  std::vector<double> hltEle20LWPt_;
  std::vector<double> hltEle20LWEta_;
  std::vector<double> hltEle20LWPhi_;

  std::vector<double> hltEle10SWEt_;
  std::vector<double> hltEle10SWPt_;
  std::vector<double> hltEle10SWEta_;
  std::vector<double> hltEle10SWPhi_;

  std::vector<double> hltEle15SWEt_;
  std::vector<double> hltEle15SWPt_;
  std::vector<double> hltEle15SWEta_;
  std::vector<double> hltEle15SWPhi_;

  std::vector<double> hltEle20SWEt_;
  std::vector<double> hltEle20SWPt_;
  std::vector<double> hltEle20SWEta_;
  std::vector<double> hltEle20SWPhi_;

  std::vector<double> hltEle25SWEt_;
  std::vector<double> hltEle25SWPt_;
  std::vector<double> hltEle25SWEta_;
  std::vector<double> hltEle25SWPhi_;

  std::vector<double> hltPhoton10Et_ ;
  std::vector<double> hltPhoton10Pt_ ;
  std::vector<double> hltPhoton10Eta_ ;
  std::vector<double> hltPhoton10Phi_ ;

  std::vector<double> hltPhoton15Et_ ;
  std::vector<double> hltPhoton15Pt_ ;
  std::vector<double> hltPhoton15Eta_ ;
  std::vector<double> hltPhoton15Phi_ ;

  std::vector<double> hltPhoton20Et_ ;
  std::vector<double> hltPhoton20Pt_ ;
  std::vector<double> hltPhoton20Eta_ ;
  std::vector<double> hltPhoton20Phi_ ;

  std::vector<double> hltPhoton15CleanEt_ ;
  std::vector<double> hltPhoton15CleanPt_ ;
  std::vector<double> hltPhoton15CleanEta_ ;
  std::vector<double> hltPhoton15CleanPhi_ ;

  std::vector<double> hltPhoton20CleanEt_ ;
  std::vector<double> hltPhoton20CleanPt_ ;
  std::vector<double> hltPhoton20CleanEta_ ;
  std::vector<double> hltPhoton20CleanPhi_ ;

  std::vector<double> hltPhoton30CleanEt_ ;
  std::vector<double> hltPhoton30CleanPt_ ;
  std::vector<double> hltPhoton30CleanEta_ ;
  std::vector<double> hltPhoton30CleanPhi_ ;
};

#endif
