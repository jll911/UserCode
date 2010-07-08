runEwkDqmTree(const char* filename){
  gROOT->LoadMacro("/afs/cern.ch/user/j/jleonard/scratch0/jtest/CMSSW_3_5_4/src/UserCode/DQMStandaloneEwkElectron/test/EwkDqmTree.C");
  EwkDqmTree tree(filename);
  tree.Loop();
}
