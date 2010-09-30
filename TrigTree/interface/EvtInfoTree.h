#ifndef evttree_h
#define evttree_h

#include <memory>
#include <string>
#include <iostream>
#include <vector>
#include "TTree.h" 
#include "FWCore/Framework/interface/Event.h" 
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"


class EvtInfoTree{

 public:
  EvtInfoTree(std::string name, TTree* tree
	   /*,const edm::ParameterSet iConfig*/);
  
  ~EvtInfoTree();
  void EvtInfoCollector(const edm::Event& iEvent); 
  void Clear();
 private:
  EvtInfoTree(){};//Don't allow user
  void SetBranches();
  void AddBranch(int* x, std::string name);
    
  TTree *tree_;
  std::string name_;
  int nEvt_;
  int nRun_;
  int nLumiS_;
  int bunchX_;
};

#endif
