
#include <iostream>
#include "UserCode/TrigTree/interface/EvtInfoTree.h"


EvtInfoTree::EvtInfoTree(std::string name, TTree* tree){
  name_=name;
  tree_=tree;
  SetBranches();
}


EvtInfoTree::~EvtInfoTree(){
  delete tree_;
}


void
EvtInfoTree::EvtInfoCollector(const edm::Event& iEvent){
  nEvt_   = iEvent.id().event();
  nRun_   = iEvent.id().run();
  nLumiS_ = iEvent.luminosityBlock();
  bunchX_ = iEvent.bunchCrossing();

}


void
EvtInfoTree::SetBranches(){
  AddBranch(&nEvt_,"EventNum");
  AddBranch(&nRun_,  "RunNum");
  AddBranch(&nLumiS_, "LumiSection");
  AddBranch(&bunchX_, "BunchXing");
}

void 
EvtInfoTree::AddBranch(int* x, std::string name){
  std::string brName="EvtInfo_"+name;
  tree_->Branch(brName.c_str(),x,(brName+"/I").c_str());
}

void 
EvtInfoTree::Clear(){
  nEvt_   = -99999;
  nRun_   = -99999;
  nLumiS_ = -99999;
  bunchX_ = -99999;
}
