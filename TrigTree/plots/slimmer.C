#include "TFile.h"
#include "TTree.h"
#include <iostream>

void slimmer(TString inFile, TString outFile)
{
  TFile *f = new TFile(inFile);
  inFile += ":/demo";
  f->cd( inFile );

  TTree *oldtree = (TTree*)gDirectory->Get("TrigTree");

  long long max_tree_size = 200000000000LL; // 200 GB Max file size

  std::cout << "Setting tree size...\n";
  if(oldtree->GetMaxTreeSize() < max_tree_size ) 
    oldtree->SetMaxTreeSize(max_tree_size);
  
  std::cout << "Getting entries...\n";
  //  double nEntries  = (double) oldtree->GetEntries();
  Double_t nEntries  = (double) oldtree->GetEntries();
  std::cout << nEntries << "\n";
  
  if (!nEntries)
    {
      std::cout << "Input file error\n";
      return;
    }
  
  // Disable branches ?
  //  oldtree->SetBranchStatus("*",1);
  oldtree->SetBranchStatus("*",0);
  
  // ... switch on those you'd like to keep in the new tree
  //     std::cout << "Enabling branches...\n";
  oldtree->SetBranchStatus("EvtInfo_EventNum",     1);
  oldtree->SetBranchStatus("EvtInfo_RunNum",       1);
  oldtree->SetBranchStatus("EvtInfo_LumiSection",  1);
  oldtree->SetBranchStatus("EvtInfo_BunchXing",    1);
  oldtree->SetBranchStatus("ElectronNum",          1);
  oldtree->SetBranchStatus("ElectronPt",           1);
  oldtree->SetBranchStatus("ElectronEta",          1);
  oldtree->SetBranchStatus("ElectronPhi",          1);
  oldtree->SetBranchStatus("ElectronEt",           1);
  oldtree->SetBranchStatus("ElectroneleEcalDriven",           1);
  oldtree->SetBranchStatus("ElectroneleTrkDriven",            1);
  oldtree->SetBranchStatus("ElectronDhSuperClsTrkAtVtx",            1);
  oldtree->SetBranchStatus("ElectronDphiSuperClsTrkAtVtx",          1);
  oldtree->SetBranchStatus("ElectroneleRelIsoEcal",           1);
  oldtree->SetBranchStatus("ElectroneleRelIsoHcal",           1);
  oldtree->SetBranchStatus("ElectroneleRelIsoTrk",            1);
  oldtree->SetBranchStatus("ElectroneleRelIsoComb",           1);
  oldtree->SetBranchStatus("ElectroneleMissingHits",          1);
  oldtree->SetBranchStatus("ElectroneleDist",          1);
  oldtree->SetBranchStatus("ElectroneleDeltaCotTheta",          1);
  oldtree->SetBranchStatus("ElectroneleConvRadius",             1);
  oldtree->SetBranchStatus("ElectroneleMaxOver3x3",             1);
  oldtree->SetBranchStatus("ElectroneleScEnergy",          1);
  oldtree->SetBranchStatus("ElectroneleScEta",             1);
  oldtree->SetBranchStatus("ElectroneleScEt",              1);
  oldtree->SetBranchStatus("Electroneleenergy",            1);
  oldtree->SetBranchStatus("ElectronelehcalOverEcal",      1);
  oldtree->SetBranchStatus("ElectronelesigmaIetaIeta",          1);
  oldtree->SetBranchStatus("Electronelecharge",          1);
  oldtree->SetBranchStatus("Electronelepx",          1);
  oldtree->SetBranchStatus("Electronelepy",          1);
  oldtree->SetBranchStatus("Electronelepz",          1);
  oldtree->SetBranchStatus("trigResults",            1);
  oldtree->SetBranchStatus("trigName",               1);
  oldtree->SetBranchStatus("hlt*",                   1);
  oldtree->SetBranchStatus("L1trigResults",          1);
  oldtree->SetBranchStatus("L1trigErrCode",          1);
  oldtree->SetBranchStatus("L1trigName",             1);
  oldtree->SetBranchStatus("l1IsoEle*",              1);
  oldtree->SetBranchStatus("l1NonIsoEle*",           1);
  oldtree->SetBranchStatus("ElectronelePass*",          1);






  std::cout << "Creating output file...\n";
  TFile *newfile = new TFile(outFile, "recreate");
  
  std::cout << "Output file created, creating tree . . ." << std::endl;
  // keeping only events with trigger
  TTree *newTree = oldtree->CloneTree(0);
//   std::cout << "Setting directory" << std::endl;
//   newTree->SetDirectory(0);
  
  std::cout << "Creating vectors" << std::endl;
  vector<double>  *ElectroneleRelIsoComb;
  vector<double>  *ElectronelesigmaIetaIeta;
  std::cout << "Setting branch addresses" << std::endl;
  oldtree->SetBranchAddress("ElectroneleRelIsoComb",   &ElectroneleRelIsoComb);
  oldtree->SetBranchAddress("ElectronelesigmaIetaIeta",&ElectronelesigmaIetaIeta);
  //  double kept = 0;
  //  Int_t kept = 0;
  ULong_t kept = 0;
  
  //  Int_t nLoop = nEntries;
  ULong_t nLoop = nEntries;
  //Int_t nLoop = 1000000;
  std::cout << "Starting loop . . ." << std::endl;
  for ( Int_t i = 0; i < nEntries; i++ ) 
  //  for ( ULong_t i = 0; i < nLoop; i++ ) 
    {
      oldtree->GetEntry(i);
      
      if ( i%100000 == 0 ) std::cout << "Entry #" << i << std::endl;
      
      bool keep = false;
      
      if (  ElectroneleRelIsoComb->size() )
	{
	  for ( unsigned int j = 0; j < ElectroneleRelIsoComb->size(); ++j )
	    {
	      if ( ElectroneleRelIsoComb->at(j) < 0.4 && ElectronelesigmaIetaIeta->at(j) < 0.07 ) 
		{
		  keep = true;
		  ++kept;
		  break;
		}
	    }
	}
      
      if ( keep )
        newTree->Fill();
    }
  
  if(newTree->GetMaxTreeSize() < max_tree_size ) 
    newTree->SetMaxTreeSize(max_tree_size);
  
  //  std::cout << "Kept " << kept << " out of " << nEntries << " events (" << kept*100/nEntries << " %).\n";
  std::cout << "Kept " << kept << " out of " << nLoop << " events (" << kept*100/nLoop << " %).\n";

  std::cout << "Wrapping up...cd'ing to new file . . .\n";
  newfile->cd();
  std::cout << "Writing newTree\n";
  newTree->Write();
  std::cout << "Closing newfile\n";
  newfile->Close();
  
  //  std::cout << "Kept " << kept << " out of " << nEntries << " events (" << kept*100/nEntries << " %).\n";
}

