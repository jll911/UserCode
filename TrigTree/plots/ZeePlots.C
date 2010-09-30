#include "ZeePlots.h"

#include <iostream>
#include <sstream>
#include <stdexcept>

using std::cout;
using std::endl;

int main(int argc, char **argv)
{
  if ( argc == 1 )
  {
    cout << "No filename specified.\n";
    return 1;
  }

  if ( argc == 2 )
    ZeePlots t(argv[1], -1, 0);
  else if ( argc == 3 )
    ZeePlots t(argv[1], atol(argv[2]), 0);
  else if ( argc == 4 )
    ZeePlots t(argv[1], atol(argv[2]), atol(argv[3]));
  
  return 0;
}

ZeePlots::ZeePlots(string sample, double limit, double first) : sample_(sample)
{
  string plots = "Plots_" + sample.substr( sample.find_last_of("/") + 1, sample.size() );

  TFile *f = new TFile(sample.c_str());
  f->cd( (sample+":/demo").c_str() );

  TTree *tree = (TTree*)gDirectory->Get("TrigTree");
  //  TTree *tree = (TTree*)f->Get("TrigTree");

  Init(tree);

  //  hFile = new TFile(plots.c_str(),"RECREATE");
  hFile = new TFile(plots.c_str(),"CREATE");

  //  if (!hFile.IsOpen())
  if (!hFile->IsOpen())
    {
      std::cout << "Output file " << plots << " already exists!  Program exiting." << std::endl;
      return;
    }

  cout << "Creating " << plots << endl;

  Loop(limit, first);

  //  std::cout << "Loop done" << std::endl;
}

ZeePlots::~ZeePlots()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
   
   hFile->Write();
   hFile->Close();
}



void ZeePlots::Loop(double limit, double first)
{
  //   std::cout << "Entered Loop(), limit is " << limit << std::endl;

   if (fChain == 0)
     {
       std::cout << "Oops, no associated tree" << std::endl;
       return;
     }

   Long64_t nentries = fChain->GetEntriesFast();
   
   cutList_  =  "NoCuts";
//    cutList_ += ";TriggerMatchL1";
//    cutList_ += ";TriggerHlt"; 
   cutList_ += ";SpikesRemoved;SCEt";
//    cutList_ += ";MissingHits;Dist;deltaCotTheta";
   cutList_ += ";ConvRej";
//    //   cutList_ += ";RelIsoTrk;RelIsoEcal;RelIsoHcal";
   cutList_ += ";RelIsoComb";
//    cutList_ += ";SigIeIe;deltaPhiIn;deltaEtaIn;HoE";
   cutList_ += ";Eid";
//    //   cutList_ += ";TriggerMatchL1";
//    //   cutList_ += ";EcalDrv"; 
//    //   cutList_ += ";TrkDrv"; 
   cutVec  = stringParser(cutList_);
   
   triggersOfInterestL1 = "NoTrigger;L1_SingleEG5;L1_SingleEG8";// L1_SingleEG5;L1_SingleEG8; 
   triggersOfInterestHlt = "HLT_Photon10_L1R;HLT_Photon15_L1R;HLT_Photon15NewThresh_L1R;HLT_Photon20_L1R";
   triggersOfInterestHlt += ";HLT_Photon15_Cleaned_L1R;HLT_Photon20_Cleaned_L1R;HLT_Photon30_Cleaned_L1R"; // ;HLT_Ele15_SW_L1R;HLT_Ele10_SW_L1R 20 and 25// need to code these into foundElectronMatchToHlt() 
   triggersOfInterestHlt += ";HLT_Ele10_LW_L1R;HLT_Ele15_LW_L1R;HLT_Ele20_LW_L1R"; // ;HLT_Ele15_SW_L1R;HLT_Ele10_SW_L1R 20 and 25// need to code these into foundElectronMatchToHlt() 
   triggersOfInterestHlt += ";HLT_Ele10_SW_L1R;HLT_Ele15_SW_L1R;HLT_Ele20_SW_L1R;HLT_Ele25_SW_L1R"; // ;HLT_Ele15_SW_L1R;HLT_Ele10_SW_L1R 20 and 25// need to code these into foundElectronMatchToHlt() 
   // 

   wpList_ = "WP95";//"WP80;WP95";//
   wpVec = stringParser(wpList_);
   
   //   BookHistograms();
   BookAnalysisHistograms();

   std::cout << "Total number of entries: " << nentries << "  limit: " << limit << "  first event: " << first << std::endl;

   double count = 0;

   //   for (Long64_t jentry=0; jentry<nentries;jentry++) 
   for (Long64_t jentry=first; jentry<nentries;jentry++) 
   {
      ientry = LoadTree(jentry);
      //      if (ientry < 0 || ientry == limit ) 
      if (ientry < 0 || count == limit ) 
      {
        cout << "Stopped at event #" << ientry << endl;
        break;
      }

      count++;

      fChain->GetEntry(jentry);
      
      if ( jentry%100000 == 0 ) std::cout << "Entry #" << jentry << std::endl;
      //      if ( jentry%10000 == 0 ) std::cout << "Entry #" << jentry << std::endl;
      
      //      if( sample_ == "data" && !(EvtInfo_BunchXing==1 || EvtInfo_BunchXing==1786)) continue;

      //      if ( !(EvtInfo_RunNum==142558) ) continue;

      triggers.clear();
      triggers["NoTrigger"] = 1;
      for(unsigned int i=0; i < trigName->size(); i++)
        triggers[trigName->at(i)] = trigResults->at(i);

      for(unsigned int i=0; i < L1trigName->size(); i++)
        triggers[L1trigName->at(i)] = L1trigResults->at(i);

      triggers["HLT_Photon15NewThresh_L1R"] = triggers["HLT_Photon10_L1R"];

      if ( selectRecoElectrons() )
      {
	//        fillRecoElectronL1Histograms(0.5);
	//        fillRecoElectronHltHistograms(0.5, 0.2);
	fillRecoElectronAnalysisHistograms(0.5, 0.2);
      }

   } // end loop over events
}


void
ZeePlots::fillRecoElectronL1Histograms(double l1match)
{
  WP wpEb, wpEe;
   
  std::vector<std::string> trigVec = stringParser(triggersOfInterestL1);
  
  for ( unsigned int w = 0; w < wpVec.size(); ++w )
  {
    setWp( wpVec.at(w), wpEb, wpEe);
     
    for ( unsigned int j = 0; j < cutVec.size(); ++j )
    {
      string where( fabs( recoElectronCollection.at(0).Eta() ) < 1.4442 ? "EB" : "EE");
      WP     wp   ( fabs( recoElectronCollection.at(0).Eta() ) < 1.4442 ? wpEb : wpEe);
      
      if ( !applyRecoEleCut( cutVec.at(j), wp, 0 ) ) break;
      
      for ( unsigned int i = 0; i < trigVec.size(); ++i )
      {
        if ( triggers[trigVec.at(i)] )
        {
          if ( trigVec.at(i) == "NoTrigger" )
          {
//             fillRecoElectronHistoCollection((wpVec.at(w))+"_"+(trigVec.at(i))+"_"+(cutVec.at(j))+"_"+where);
//             fillRecoElectronHistoCollection((wpVec.at(w))+"_"+(trigVec.at(i))+"_"+(cutVec.at(j))+"_ALL");
          }
          else if ( recoElectronCollection.at(0).ClosestL1dr() < l1match ) // fill only if L1 match
          {
//             fillRecoElectronHistoCollection((wpVec.at(w))+"_"+(trigVec.at(i))+"_"+(cutVec.at(j))+"_"+where);
//             fillRecoElectronHistoCollection((wpVec.at(w))+"_"+(trigVec.at(i))+"_"+(cutVec.at(j))+"_ALL");
          }
        }
      }
    }
//       if ( EvtInfo_EventNum == 17193265 || 
//            EvtInfo_EventNum == 50105705 || 
//            EvtInfo_EventNum == 36100271 )
//       {
//         for ( unsigned int i = 0; i < recoElectronCollection.size(); ++i )
//         {
//           WP     wp   ( fabs( recoElectronCollection.at(i).Eta() ) < 1.4442 ? wpEb : wpEe);
//           cout << "Event: " << EvtInfo_EventNum << "(" << i << ")" << endl;
//           cout << "        ScEt          : " << recoElectronCollection.at(i).ScEt() << "\t---" << applyRecoEleCut("SCEt", wp, i ) << endl;
//           cout << "        Pt            : " << recoElectronCollection.at(i).Pt() << endl;
//           cout << "        Eta           : " << recoElectronCollection.at(i).Eta() << "\t---" << electronInEta( ElectronEta->at(recoElectronCollection.at(i).Pos()) ) << endl;
//           cout << "        Phi           : " << recoElectronCollection.at(i).Phi() << "\t---" << endl;
//           cout << "        MissingHits   : " << ElectroneleMissingHits->at(recoElectronCollection.at(i).Pos()) << "\t---" << applyRecoEleCut("MissingHits", wp, i ) << endl;
//           cout << "        Dist          : " << fabs(ElectroneleDist->at(recoElectronCollection.at(i).Pos()))  << "\t---" << applyRecoEleCut("Dist", wp, i ) << endl;
//           cout << "        deltaCotTheta : " << fabs(ElectroneleDeltaCotTheta->at(recoElectronCollection.at(i).Pos())) << "\t---" << applyRecoEleCut("deltaCotTheta", wp, i ) << endl;
//           cout << "        RelIsoTrk     : " << ElectroneleRelIsoTrk->at(recoElectronCollection.at(i).Pos()) << "\t---" << applyRecoEleCut("RelIsoTrk", wp, i ) << endl;
//           cout << "        RelIsoEcal    : " << ElectroneleRelIsoEcal->at(recoElectronCollection.at(i).Pos()) << "\t---" << applyRecoEleCut("RelIsoEcal", wp, i ) << endl;
//           cout << "        RelIsoHcal    : " << ElectroneleRelIsoHcal->at(recoElectronCollection.at(i).Pos()) << "\t---" << applyRecoEleCut("RelIsoHcal", wp, i ) << endl;
//           cout << "        SigIeIe       : " << recoElectronCollection.at(i).SigIeIe() << "\t---" << applyRecoEleCut("SigIeIe", wp, i ) << endl;
//           cout << "        deltaPhiIn    : " << fabs( recoElectronCollection.at(i).DeltaPhiIn() ) << "\t---" << applyRecoEleCut("deltaPhiIn", wp, i ) << endl;
//           cout << "        deltaEtaIn    : " << fabs( recoElectronCollection.at(i).DeltaEtaIn() ) << "\t---" << applyRecoEleCut("deltaEtaIn", wp, i ) << endl;
//           cout << "        HoE           : " << recoElectronCollection.at(i).HoE() << "\t---" << applyRecoEleCut("HoE", wp, i ) << endl;
//           cout << "        TriggerMatchL1  : " << recoElectronCollection.at(i).ClosestL1dr() << "\t---" << applyRecoEleCut("TriggerMatchL1", wp, i ) << endl;
//       }
//       }
  }
}

void
ZeePlots::fillRecoElectronHltHistograms(double l1match, double hltmatch)
{
  WP wpEb, wpEe;
  
  std::vector<std::string> trigVec = stringParser(triggersOfInterestHlt);
  
  for ( unsigned int i = 0; i < trigVec.size(); ++i )
    {
      if ( not recoElectronCollection.at(0).ClosestL1dr() < l1match and 
	   not foundElectronMatchToHlt( trigVec.at(i), hltmatch, 0 ) ) continue; // 0.5 l1 0.2 hlt
      
      for ( unsigned int w = 0; w < wpVec.size(); ++w )
	{
	  setWp( wpVec.at(w), wpEb, wpEe);
	  
	  for ( unsigned int j = 0; j < cutVec.size(); ++j )
	    {
	      string where( fabs( recoElectronCollection.at(0).Eta() ) < 1.4442 ? "EB" : "EE");
	      WP     wp   ( fabs( recoElectronCollection.at(0).Eta() ) < 1.4442 ? wpEb : wpEe);
	      
	      if ( !applyRecoEleCut( cutVec.at(j), wp, 0 ) ) break;
	      
	      if ( triggers[trigVec.at(i)] )
		{
		  //           fillRecoElectronHistoCollection((wpVec.at(w))+"_"+(trigVec.at(i))+"_"+(cutVec.at(j))+"_"+where);
		  //           fillRecoElectronHistoCollection((wpVec.at(w))+"_"+(trigVec.at(i))+"_"+(cutVec.at(j))+"_ALL");
		}
	    }
	}
    }
}

void
ZeePlots::fillRecoElectronAnalysisHistograms(double l1match, double hltmatch)
{
  WP wpEb, wpEe;
  
  std::vector<std::string> trigVec = stringParser(triggersOfInterestHlt);
  
  //  usedHltCands.clear();
  
  //   for ( unsigned int w = 0; w < wpVec.size(); ++w )
  //     {
  //      setWp( wpVec.at(w), wpEb, wpEe);
  setWp( "WP95", wpEb, wpEe);
      
  //      bool histForCombTrigsFilled = false;
      
  //       for ( unsigned int i = 0; i < trigVec.size(); ++i )
  // 	{
  // 	  // requires highest electron to pass, i.e. requires event to pass
  // 	  if ( not recoElectronCollection.at(0).ClosestL1dr() < l1match and 
  // 	       not foundElectronMatchToHlt( trigVec.at(i), hltmatch, 0 ) ) continue; // 0.5 l1 0.2 hlt
  // 	  //     if ( not recoElectronCollection.at(0).ClosestL1dr() < l1match ) continue; 
  
	  
  double invMass_ = 0.;
  double invMassCorr_ = 0.;
  double Zpt_ = 0.;
  double Zeta_ = 0.;
  double Zrapidity_ = 0.;
  double Zphi_ = 0.;
	  
  //  std::cout << "New event" << std::endl;

  hists["nElectronsPreCuts"]->Fill( recoElectronCollection.size() );

  double countPostCuts = 0.;

  // Loop through first electron
  for ( unsigned int elec1 = 0; elec1 < recoElectronCollection.size() /*&& elec < 3*/; ++elec1 )
    {
      
      string where1( fabs( recoElectronCollection.at(elec1).Eta() ) < 1.4442 ? "EB" : "EE");
      //      WP     wp1   ( fabs( recoElectronCollection.at(elec1).Eta() ) < 1.4442 ? wpEb : wpEe);

      bool pass1 = true;

      for ( unsigned int j = 0; j < cutVec.size(); ++j )
	{
	  //if ( !applyRecoEleCut( cutVec.at(j), wp, 0 ) ) break;
	  if ( cutVec.at(j).compare("TriggerHlt") == 0 )
	    {
	      bool passTrigger1 = false;
	      for ( unsigned int i = 0; i < trigVec.size(); ++i )
		{
		  if (foundElectronMatchToHlt( trigVec.at(i), hltmatch, elec1 ) ) 
		    {
		      passTrigger1 = true;
		    }
		}
	      pass1 = (pass1 && passTrigger1);
	    }
	  //	  else if ( !applyRecoEleCut( cutVec.at(j), wp1, elec1 ) ) 
	  else if ( !applyRecoEleWp95Cut( cutVec.at(j), elec1 ) ) 
	    {
	      pass1 = false; 
	    }
	  //	      else
	  if (pass1)
	    {
	      //		  std::cout << "Filling 1 after step " << j << std::endl;
	      // scet
	      //hists["WP95_AllTrigs_"+(cutVec.at(j))+"_"+(itos(j))+"_"+(where1)+"_ScEt"]->Fill( recoElectronCollection.at(elec1).ScEt() );
	      hists["WP95_AllTrigs_"+(cutVec.at(j))+"_"+(itos(j))+"_"+(where1)+"_ScEt"]->Fill( recoElectronCollection.at(elec1).ScEt() );
	      hists["WP95_AllTrigs_"+(cutVec.at(j))+"_"+(itos(j))+"_ALL"+"_ScEt"]->Fill( recoElectronCollection.at(elec1).ScEt() );
	      // eta
	      //hists["WP95_AllTrigs_"+(cutVec.at(j))+"_"+(itos(j))+"_"+(where1)+"_Eta"]->Fill( recoElectronCollection.at(elec1).Eta() );
	      //	      hists["WP95_AllTrigs_"+(cutVec.at(j))+"_"+(itos(j))+"_ALL"+"_Eta"]->Fill( recoElectronCollection.at(elec1).Eta() );
	      hists["WP95_AllTrigs_"+(cutVec.at(j))+"_"+(itos(j))+"_Eta"]->Fill( recoElectronCollection.at(elec1).Eta() );
	      // phi
	      hists["WP95_AllTrigs_"+(cutVec.at(j))+"_"+(itos(j))+"_"+(where1)+"_Phi"]->Fill( recoElectronCollection.at(elec1).Phi() );
	      hists["WP95_AllTrigs_"+(cutVec.at(j))+"_"+(itos(j))+"_ALL"+"_Phi"]->Fill( recoElectronCollection.at(elec1).Phi() );

	      //std::cout << "Elec 1 Kinematics plots filled" << std::endl;

	      if ( j+1 < cutVec.size() )
		{
		  if ( cutVec.at(j+1).compare("ConvRej") == 0 )
		    {
		      hists["WP95_AllTrigs_"+(cutVec.at(j))+"_"+(itos(j))+"_"+(where1)+"_MissHits"]->Fill( recoElectronCollection.at(elec1).MissHits() );
		      hists["WP95_AllTrigs_"+(cutVec.at(j))+"_"+(itos(j))+"_ALL"+"_MissHits"]->Fill( recoElectronCollection.at(elec1).MissHits() );
		      hists["WP95_AllTrigs_"+(cutVec.at(j))+"_"+(itos(j))+"_"+(where1)+"_Dist"]->Fill( fabs(recoElectronCollection.at(elec1).Dist() ) );
		      hists["WP95_AllTrigs_"+(cutVec.at(j))+"_"+(itos(j))+"_ALL"+"_Dist"]->Fill( fabs(recoElectronCollection.at(elec1).Dist() ) );
		      hists["WP95_AllTrigs_"+(cutVec.at(j))+"_"+(itos(j))+"_"+(where1)+"_DCotTh"]->Fill( fabs(recoElectronCollection.at(elec1).DCotTheta() ) );
		      hists["WP95_AllTrigs_"+(cutVec.at(j))+"_"+(itos(j))+"_ALL"+"_DCotTh"]->Fill( fabs(recoElectronCollection.at(elec1).DCotTheta() ) );
		    }
		  if ( cutVec.at(j+1).compare("MissingHits") == 0 )
		    {
 		      hists["WP95_AllTrigs_"+(cutVec.at(j))+"_"+(itos(j))+"_"+(where1)+"_MissHits"]->Fill( recoElectronCollection.at(elec1).MissHits() );
 		      hists["WP95_AllTrigs_"+(cutVec.at(j))+"_"+(itos(j))+"_ALL"+"_MissHits"]->Fill( recoElectronCollection.at(elec1).MissHits() );
		    }
		  if ( cutVec.at(j+1).compare("Dist") == 0 )
		    {
 		      hists["WP95_AllTrigs_"+(cutVec.at(j))+"_"+(itos(j))+"_"+(where1)+"_Dist"]->Fill( fabs(recoElectronCollection.at(elec1).Dist() ) );
 		      hists["WP95_AllTrigs_"+(cutVec.at(j))+"_"+(itos(j))+"_ALL"+"_Dist"]->Fill( fabs(recoElectronCollection.at(elec1).Dist() ) );
		    }
		  if ( cutVec.at(j+1).compare("deltaCotTheta") == 0 )
		    {
 		      hists["WP95_AllTrigs_"+(cutVec.at(j))+"_"+(itos(j))+"_"+(where1)+"_DCotTh"]->Fill( fabs(recoElectronCollection.at(elec1).DCotTheta() ) );
 		      hists["WP95_AllTrigs_"+(cutVec.at(j))+"_"+(itos(j))+"_ALL"+"_DCotTh"]->Fill( fabs(recoElectronCollection.at(elec1).DCotTheta() ) );
		    }

		  if ( cutVec.at(j+1).compare("RelIsoComb") == 0 )
		    {
 		      hists["WP95_AllTrigs_"+(cutVec.at(j))+"_"+(itos(j))+"_"+(where1)+"_CombRelIso"]->Fill( ElectroneleRelIsoComb->at(recoElectronCollection.at(elec1).Pos()) );
 		      hists["WP95_AllTrigs_"+(cutVec.at(j))+"_"+(itos(j))+"_ALL"+"_CombRelIso"]->Fill( ElectroneleRelIsoComb->at(recoElectronCollection.at(elec1).Pos()) );
		    }

		  if ( cutVec.at(j+1).compare("Eid") == 0 )
		    {
		      hists["WP95_AllTrigs_"+(cutVec.at(j))+"_"+(itos(j))+"_"+(where1)+"_HoE"]->Fill( recoElectronCollection.at(elec1).HoE() );
		      hists["WP95_AllTrigs_"+(cutVec.at(j))+"_"+(itos(j))+"_ALL"+"_HoE"]->Fill( recoElectronCollection.at(elec1).HoE() );
		      hists["WP95_AllTrigs_"+(cutVec.at(j))+"_"+(itos(j))+"_"+(where1)+"_dEtaIn"]->Fill( recoElectronCollection.at(elec1).DeltaEtaIn() );
		      hists["WP95_AllTrigs_"+(cutVec.at(j))+"_"+(itos(j))+"_ALL"+"_dEtaIn"]->Fill( recoElectronCollection.at(elec1).DeltaEtaIn() );
		      hists["WP95_AllTrigs_"+(cutVec.at(j))+"_"+(itos(j))+"_"+(where1)+"_dPhiIn"]->Fill( recoElectronCollection.at(elec1).DeltaPhiIn() );
		      hists["WP95_AllTrigs_"+(cutVec.at(j))+"_"+(itos(j))+"_ALL"+"_dPhiIn"]->Fill( recoElectronCollection.at(elec1).DeltaPhiIn() );
		      hists["WP95_AllTrigs_"+(cutVec.at(j))+"_"+(itos(j))+"_"+(where1)+"_SigIeIe"]->Fill( recoElectronCollection.at(elec1).SigIeIe() );
		      hists["WP95_AllTrigs_"+(cutVec.at(j))+"_"+(itos(j))+"_ALL"+"_SigIeIe"]->Fill( recoElectronCollection.at(elec1).SigIeIe() );
		    }
		  if ( cutVec.at(j+1).compare("HoE") == 0 )
		    {
 		      hists["WP95_AllTrigs_"+(cutVec.at(j))+"_"+(itos(j))+"_"+(where1)+"_HoE"]->Fill( recoElectronCollection.at(elec1).HoE() );
 		      hists["WP95_AllTrigs_"+(cutVec.at(j))+"_"+(itos(j))+"_ALL"+"_HoE"]->Fill( recoElectronCollection.at(elec1).HoE() );
		    }
		  if ( cutVec.at(j+1).compare("deltaEtaIn") == 0 )
		    {
 		      hists["WP95_AllTrigs_"+(cutVec.at(j))+"_"+(itos(j))+"_"+(where1)+"_dEtaIn"]->Fill( recoElectronCollection.at(elec1).DeltaEtaIn() );
 		      hists["WP95_AllTrigs_"+(cutVec.at(j))+"_"+(itos(j))+"_ALL"+"_dEtaIn"]->Fill( recoElectronCollection.at(elec1).DeltaEtaIn() );
		    }
		  if ( cutVec.at(j+1).compare("deltaPhiIn") == 0 )
		    {
 		      hists["WP95_AllTrigs_"+(cutVec.at(j))+"_"+(itos(j))+"_"+(where1)+"_dPhiIn"]->Fill( recoElectronCollection.at(elec1).DeltaPhiIn() );
 		      hists["WP95_AllTrigs_"+(cutVec.at(j))+"_"+(itos(j))+"_ALL"+"_dPhiIn"]->Fill( recoElectronCollection.at(elec1).DeltaPhiIn() );
		    }
		  if ( cutVec.at(j+1).compare("SigIeIe") == 0 )
		    {
 		      hists["WP95_AllTrigs_"+(cutVec.at(j))+"_"+(itos(j))+"_"+(where1)+"_SigIeIe"]->Fill( recoElectronCollection.at(elec1).SigIeIe() );
 		      hists["WP95_AllTrigs_"+(cutVec.at(j))+"_"+(itos(j))+"_ALL"+"_SigIeIe"]->Fill( recoElectronCollection.at(elec1).SigIeIe() );
		    }
		}

	    }
	}
      // after all cuts
      if (pass1)
	{
	  countPostCuts++;
	  //	  std::cout << EvtInfo_EventNum << std::endl;
// 	  std::cout << "Run " << EvtInfo_RunNum << "  event " << EvtInfo_EventNum << " has electron, Et=" 
// 		    << recoElectronCollection.at(elec1).ScEt() 
// 		    << " eta=" << recoElectronCollection.at(elec1).Eta()
// 		    << " missHits=" << recoElectronCollection.at(elec1).Eta()
// 		    << " detain=" << recoElectronCollection.at(elec1).DeltaEtaIn()
// 		    << " dphiin=" << recoElectronCollection.at(elec1).DeltaPhiIn()
// 		    << " sieie=" << recoElectronCollection.at(elec1).SigIeIe()
// 		    << " iso=" << ElectroneleRelIsoComb->at(recoElectronCollection.at(elec1).Pos())
// 		    << std::endl;
	}

      // Looping through second electron
      for ( unsigned int elec2 = 0; elec2 < elec1 /*&& elec < 3*/; ++elec2 )
	{
	  
	  string where2( fabs( recoElectronCollection.at(elec2).Eta() ) < 1.4442 ? "EB" : "EE");
	  //	  WP     wp2   ( fabs( recoElectronCollection.at(elec2).Eta() ) < 1.4442 ? wpEb : wpEe);
	      
	  bool pass2 = true;
	  invMass_ = 0.;
	  invMassCorr_ = 0.;

	  for ( unsigned int j = 0; j < cutVec.size(); ++j )
	    {
	      if ( cutVec.at(j).compare("TriggerHlt") == 0 )
		{
		  bool passTrigger2 = false;
		  for ( unsigned int i = 0; i < trigVec.size(); ++i )
		    {
		      if (foundElectronMatchToHlt( trigVec.at(i), hltmatch, elec2 ) ) 
			{
			  passTrigger2 = true; 
			}
		    }
		  pass2 = (pass2 && passTrigger2);
		}
	      //	      else if ( !applyRecoEleCut( cutVec.at(j), wp2, elec2 ) ) 
	      else if ( !applyRecoEleWp95Cut( cutVec.at(j), elec2 ) ) 
		{
		  pass2 = false; 
		}
	      //	      else
	      if (pass2)
		{
// 		  //		  std::cout << "Filling 2 after step " << j << std::endl;
// 		  // scet
// 		  hists["WP95_AllTrigs_"+(cutVec.at(j))+"_"+(itos(j))+"_"+(where2)+"_ScEt"]->Fill( recoElectronCollection.at(elec2).ScEt() );
// 		  hists["WP95_AllTrigs_"+(cutVec.at(j))+"_"+(itos(j))+"_ALL"+"_ScEt"]->Fill( recoElectronCollection.at(elec2).ScEt() );
// 		  // eta
// 		  hists["WP95_AllTrigs_"+(cutVec.at(j))+"_"+(itos(j))+"_"+(where2)+"_Eta"]->Fill( recoElectronCollection.at(elec2).Eta() );
// 		  hists["WP95_AllTrigs_"+(cutVec.at(j))+"_"+(itos(j))+"_ALL"+"_Eta"]->Fill( recoElectronCollection.at(elec2).Eta() );
// 		  // phi
// 		  hists["WP95_AllTrigs_"+(cutVec.at(j))+"_"+(itos(j))+"_"+(where2)+"_Phi"]->Fill( recoElectronCollection.at(elec2).Phi() );
// 		  hists["WP95_AllTrigs_"+(cutVec.at(j))+"_"+(itos(j))+"_ALL"+"_Phi"]->Fill( recoElectronCollection.at(elec2).Phi() );

// 		  //std::cout << "Elec 2 Kinematics plots filled" << std::endl;
		  
// 		  // miss hits
// 		  hists["WP95_AllTrigs_"+(cutVec.at(j))+"_"+(itos(j))+"_"+(where1)+"_MissHits"]->Fill( recoElectronCollection.at(elec2).MissHits() );
// 		  hists["WP95_AllTrigs_"+(cutVec.at(j))+"_"+(itos(j))+"_ALL"+"_MissHits"]->Fill( recoElectronCollection.at(elec2).MissHits() );
// 		  // dist
// 		  hists["WP95_AllTrigs_"+(cutVec.at(j))+"_"+(itos(j))+"_"+(where1)+"_Dist"]->Fill( fabs(recoElectronCollection.at(elec2).Dist() ) );
// 		  hists["WP95_AllTrigs_"+(cutVec.at(j))+"_"+(itos(j))+"_ALL"+"_Dist"]->Fill( fabs(recoElectronCollection.at(elec2).Dist() ) );
// 		  // d cot theta
// 		  hists["WP95_AllTrigs_"+(cutVec.at(j))+"_"+(itos(j))+"_"+(where1)+"_DCotTh"]->Fill( fabs(recoElectronCollection.at(elec2).DCotTheta() ) );
// 		  hists["WP95_AllTrigs_"+(cutVec.at(j))+"_"+(itos(j))+"_ALL"+"_DCotTh"]->Fill( fabs(recoElectronCollection.at(elec2).DCotTheta() ) );

// 		  //std::cout << "Elec 2 Conv rej plots filled" << std::endl;
		  
// 		  // h/e
// 		  hists["WP95_AllTrigs_"+(cutVec.at(j))+"_"+(itos(j))+"_"+(where2)+"_HoE"]->Fill( recoElectronCollection.at(elec2).HoE() );
// 		  hists["WP95_AllTrigs_"+(cutVec.at(j))+"_"+(itos(j))+"_ALL"+"_HoE"]->Fill( recoElectronCollection.at(elec2).HoE() );
// 		  // detain
// 		  hists["WP95_AllTrigs_"+(cutVec.at(j))+"_"+(itos(j))+"_"+(where2)+"_dEtaIn"]->Fill( recoElectronCollection.at(elec2).DeltaEtaIn() );
// 		  hists["WP95_AllTrigs_"+(cutVec.at(j))+"_"+(itos(j))+"_ALL"+"_dEtaIn"]->Fill( recoElectronCollection.at(elec2).DeltaEtaIn() );
// 		  // dphiin
// 		  hists["WP95_AllTrigs_"+(cutVec.at(j))+"_"+(itos(j))+"_"+(where2)+"_dPhiIn"]->Fill( recoElectronCollection.at(elec2).DeltaPhiIn() );
// 		  hists["WP95_AllTrigs_"+(cutVec.at(j))+"_"+(itos(j))+"_ALL"+"_dPhiIn"]->Fill( recoElectronCollection.at(elec2).DeltaPhiIn() );
// 		  // sieie
// 		  hists["WP95_AllTrigs_"+(cutVec.at(j))+"_"+(itos(j))+"_"+(where2)+"_SigIeIe"]->Fill( recoElectronCollection.at(elec2).SigIeIe() );
// 		  hists["WP95_AllTrigs_"+(cutVec.at(j))+"_"+(itos(j))+"_ALL"+"_SigIeIe"]->Fill( recoElectronCollection.at(elec2).SigIeIe() );

// 		  //std::cout << "Elec 2 EleID plots filled" << std::endl;

// 		  // relcombiso
// 		  hists["WP95_AllTrigs_"+(cutVec.at(j))+"_"+(itos(j))+"_"+(where2)+"_CombRelIso"]->Fill( ElectroneleRelIsoComb->at(recoElectronCollection.at(elec2).Pos()) );
// 		  hists["WP95_AllTrigs_"+(cutVec.at(j))+"_"+(itos(j))+"_ALL"+"_CombRelIso"]->Fill( ElectroneleRelIsoComb->at(recoElectronCollection.at(elec2).Pos()) );

		  //std::cout << "Elec 2 Iso plots filled" << std::endl;
		}
	      
	      // dielectron stuff
	      if (pass1 && pass2)
		{
		  hists["WP95_AllTrigs_"+(cutVec.at(j))+"_"+(itos(j))+"_BothPass_ScEt"]->Fill(recoElectronCollection.at(elec1).ScEt());
		  hists["WP95_AllTrigs_"+(cutVec.at(j))+"_"+(itos(j))+"_BothPass_ScEt"]->Fill(recoElectronCollection.at(elec2).ScEt());

// 		  double elecMass = 0.000000510998910; // in GeV
// 		  invMass_ = invMass(elecMass,
// 				     recoElectronCollection.at(elec1).Energy(),
// 				     recoElectronCollection.at(elec1).Px(),
// 				     recoElectronCollection.at(elec1).Py(),
// 				     recoElectronCollection.at(elec1).Pz(),
// 				     elecMass,
// 				     recoElectronCollection.at(elec2).Energy(),
// 				     recoElectronCollection.at(elec2).Px(),
// 				     recoElectronCollection.at(elec2).Py(),
// 				     recoElectronCollection.at(elec2).Pz()
// 				     );

                  // NOTE THIS METHOD ONLY USES THE 4 FREE PARAMETERS INSTEAD OF ALL 5 THAT CAN BE MEASURED
		  //		  math::XYZTLorentzVector ZcandP4;
		  XYZTLorentzVector ZcandP4;
		  //		  XYZTVector ZcandP4;
// 		  ZcandP4 += math::XYZTLorentzVector(recoElectronCollection.at(elec1).Px(), 
// 						     recoElectronCollection.at(elec1).Py(), 
// 						     recoElectronCollection.at(elec1).Pz(), 
// 						     recoElectronCollection.at(elec1).ScEnergy());
// 		  ZcandP4 += math::XYZTLorentzVector(recoElectronCollection.at(elec2).Px(), 
// 						     recoElectronCollection.at(elec2).Py(), 
// 						     recoElectronCollection.at(elec2).Pz(), 
// 						     recoElectronCollection.at(elec2).ScEnergy());
// 		  invMass_ = ZcandP4.M();

		  Zcand z(this,elec1,elec2);
		  invMass_ = z.Mass();
		  invMassCorr_ = z.CorrectedMass();
		  Zpt_ = z.Pt();
		  Zeta_ = z.Eta();
		  Zrapidity_ = z.Rapidity();
		  Zphi_ = z.Phi();

// 		  std::cout << "Inv mass of " << invMass_ 
// 			    << " in run " << EvtInfo_RunNum
// 			    << "  LS " << EvtInfo_LumiSection
// 			    << "  event " << EvtInfo_EventNum
// 			    << std::endl;
		  // plot it
		  // wpVec.at(w))+"_"+(trigVec.at(i))
		  // hists[which+"_ScEt"]->Fill( invMass_ );
		  //std::cout << "Now filling hists[" << "WP95_AllTrigs_invMass" << "]" << std::endl;
		  //		  std::cout << "Filling inv mass" << std::endl;
		  if (recoElectronCollection.at(elec1).Charge() != recoElectronCollection.at(elec2).Charge() )
		    {
		      hists["WP95_AllTrigs_"+(cutVec.at(j))+"_"+(itos(j))+"_invMass"]->Fill( invMass_ );
		      hists["WP95_AllTrigs_"+(cutVec.at(j))+"_"+(itos(j))+"_invMassCorr"]->Fill( invMassCorr_ );
// 		      hists["WP95_AllTrigs_"+(cutVec.at(j))+"_"+(itos(j))+"_Zpt"]->Fill( z.Pt() );
// 		      hists["WP95_AllTrigs_"+(cutVec.at(j))+"_"+(itos(j))+"_Zeta"]->Fill( z.Eta() );
// 		      hists["WP95_AllTrigs_"+(cutVec.at(j))+"_"+(itos(j))+"_Zphi"]->Fill( z.Phi() );
		      hists["WP95_AllTrigs_"+(cutVec.at(j))+"_"+(itos(j))+"_Zpt"]->Fill( Zpt_ );
		      hists["WP95_AllTrigs_"+(cutVec.at(j))+"_"+(itos(j))+"_Zeta"]->Fill( Zeta_ );
		      hists["WP95_AllTrigs_"+(cutVec.at(j))+"_"+(itos(j))+"_Zrapidity"]->Fill( Zrapidity_ );
		      hists["WP95_AllTrigs_"+(cutVec.at(j))+"_"+(itos(j))+"_Zphi"]->Fill( Zphi_ );
		      if (where1 == "EB" && where2 == "EB")
			{
			  hists["WP95_AllTrigs_"+(cutVec.at(j))+"_"+(itos(j))+"_EBEB_invMass"]->Fill( invMass_ );
			}
		      if ((where1 == "EB" && where2 == "EE") || (where1 == "EE" && where1 == "EB"))
			{
			  hists["WP95_AllTrigs_"+(cutVec.at(j))+"_"+(itos(j))+"_EBEE_invMass"]->Fill( invMass_ );
			}
		      if (where1 == "EE" && where2 == "EE")
			{
			  hists["WP95_AllTrigs_"+(cutVec.at(j))+"_"+(itos(j))+"_EEEE_invMass"]->Fill( invMass_ );
			}
		      // 		      if (!histForCombTrigsFilled)
		      // 			{
		      // 			  hists["WP95_CombinedTrigs_invMass"]->Fill( invMass_ );
		      // 			  histForCombTrigsFilled = true;
		      // 			}
		    } // end of opposite charge
		} // end of pass1 and pass2
	    } // end of loop through cutVec
	  
	  //if (pass1) std::cout << "elec1 passes all cuts" << std::endl;
	  //if (pass2) std::cout << "elec2 passes all cuts" << std::endl;
	  //if ( triggers[trigVec.at(i)] )
	  //if ( !triggers[trigVec.at(i)] )
	  //usedHltCands.clear();

// 	  if (pass1)
// 	    {
// 	      // scet
// 	      hists["WP95_AllTrigs_Trigger_"+(where1)+"_ScEt"]->Fill( recoElectronCollection.at(elec1).ScEt() );
// 	      hists["WP95_AllTrigs_Trigger_ALL_ScEt"]->Fill( recoElectronCollection.at(elec1).ScEt() );
// 	      // eta
// 	      hists["WP95_AllTrigs_Trigger_"+(where1)+"_Eta"]->Fill( recoElectronCollection.at(elec1).Eta() );
// 	      hists["WP95_AllTrigs_Trigger_ALL_Eta"]->Fill( recoElectronCollection.at(elec1).Eta() );
// 	      // phi
// 	      hists["WP95_AllTrigs_Trigger_"+(where1)+"_Phi"]->Fill( recoElectronCollection.at(elec1).Phi() );
// 	      hists["WP95_AllTrigs_Trigger_ALL_Phi"]->Fill( recoElectronCollection.at(elec1).Phi() );
// 	      // h/e
// 	      hists["WP95_AllTrigs_Trigger_"+(where1)+"_HoE"]->Fill( recoElectronCollection.at(elec1).HoE() );
// 	      hists["WP95_AllTrigs_Trigger_ALL_HoE"]->Fill( recoElectronCollection.at(elec1).HoE() );
// 	      // detain
// 	      hists["WP95_AllTrigs_Trigger_"+(where1)+"_dEtaIn"]->Fill( recoElectronCollection.at(elec1).DeltaEtaIn() );
// 	      hists["WP95_AllTrigs_Trigger_ALL_dEtaIn"]->Fill( recoElectronCollection.at(elec1).DeltaEtaIn() );
// 	      // dphiin
// 	      hists["WP95_AllTrigs_Trigger_"+(where1)+"_dPhiIn"]->Fill( recoElectronCollection.at(elec1).DeltaPhiIn() );
// 	      hists["WP95_AllTrigs_Trigger_ALL_dPhiIn"]->Fill( recoElectronCollection.at(elec1).DeltaPhiIn() );
// 	      // sieie
// 	      hists["WP95_AllTrigs_Trigger_"+(where1)+"_SigIeIe"]->Fill( recoElectronCollection.at(elec1).SigIeIe() );
// 	      hists["WP95_AllTrigs_Trigger_ALL_SigIeIe"]->Fill( recoElectronCollection.at(elec1).SigIeIe() );
// 	      // relcombiso
// 	      hists["WP95_AllTrigs_Trigger_"+(where1)+"_CombRelIso"]->Fill( ElectroneleRelIsoComb->at(recoElectronCollection.at(elec1).Pos()) );
// 	      hists["WP95_AllTrigs_Trigger_ALL_CombRelIso"]->Fill( ElectroneleRelIsoComb->at(recoElectronCollection.at(elec1).Pos()) );
// 	    }
// 	  if (pass2)
// 	    {
// 	      // scet
// 	      hists["WP95_AllTrigs_Trigger_"+(where2)+"_ScEt"]->Fill( recoElectronCollection.at(elec2).ScEt() );
// 	      hists["WP95_AllTrigs_Trigger_ALL_ScEt"]->Fill( recoElectronCollection.at(elec2).ScEt() );
// 	      // eta
// 	      hists["WP95_AllTrigs_Trigger_"+(where2)+"_Eta"]->Fill( recoElectronCollection.at(elec2).Eta() );
// 	      hists["WP95_AllTrigs_Trigger_ALL_Eta"]->Fill( recoElectronCollection.at(elec2).Eta() );
// 	      // phi
// 	      hists["WP95_AllTrigs_Trigger_"+(where2)+"_Phi"]->Fill( recoElectronCollection.at(elec2).Phi() );
// 	      hists["WP95_AllTrigs_Trigger_ALL_Phi"]->Fill( recoElectronCollection.at(elec2).Phi() );
// 	      // h/e
// 	      hists["WP95_AllTrigs_Trigger_"+(where2)+"_HoE"]->Fill( recoElectronCollection.at(elec2).HoE() );
// 	      hists["WP95_AllTrigs_Trigger_ALL_HoE"]->Fill( recoElectronCollection.at(elec2).HoE() );
// 	      // detain
// 	      hists["WP95_AllTrigs_Trigger_"+(where2)+"_dEtaIn"]->Fill( recoElectronCollection.at(elec2).DeltaEtaIn() );
// 	      hists["WP95_AllTrigs_Trigger_ALL_dEtaIn"]->Fill( recoElectronCollection.at(elec2).DeltaEtaIn() );
// 	      // dphiin
// 	      hists["WP95_AllTrigs_Trigger_"+(where2)+"_dPhiIn"]->Fill( recoElectronCollection.at(elec2).DeltaPhiIn() );
// 	      hists["WP95_AllTrigs_Trigger_ALL_dPhiIn"]->Fill( recoElectronCollection.at(elec2).DeltaPhiIn() );
// 	      // sieie
// 	      hists["WP95_AllTrigs_Trigger_"+(where2)+"_SigIeIe"]->Fill( recoElectronCollection.at(elec2).SigIeIe() );
// 	      hists["WP95_AllTrigs_Trigger_ALL_SigIeIe"]->Fill( recoElectronCollection.at(elec2).SigIeIe() );
// 	      // relcombiso
// 	      hists["WP95_AllTrigs_Trigger_"+(where2)+"_CombRelIso"]->Fill( ElectroneleRelIsoComb->at(recoElectronCollection.at(elec2).Pos()) );
// 	      hists["WP95_AllTrigs_Trigger_ALL_CombRelIso"]->Fill( ElectroneleRelIsoComb->at(recoElectronCollection.at(elec2).Pos()) );
// 	    }
	  
	  
// 	  // dielectron stuff
// 	  if (pass1 && pass2)
// 	    {
// 	      //std::cout << "Both electrons pass: " ;
// 	      // get quantities of respective electrons
// 	      //recoElectronCollection.at(elec1).Eta()
// 	      // calculate inv mass for electron pair
// 	      double elecMass = 0.000000510998910; // in GeV
// 	      invMass_ = invMass(elecMass,
// 				 recoElectronCollection.at(elec1).Energy(),
// 				 recoElectronCollection.at(elec1).Px(),
// 				 recoElectronCollection.at(elec1).Py(),
// 				 recoElectronCollection.at(elec1).Pz(),
// 				 elecMass,
// 				 recoElectronCollection.at(elec2).Energy(),
// 				 recoElectronCollection.at(elec2).Px(),
// 				 recoElectronCollection.at(elec2).Py(),
// 				 recoElectronCollection.at(elec2).Pz()
// 				 );
// 	      std::cout << "Inv mass of " << invMass_ 
// 			<< " in run " << EvtInfo_RunNum
// 			<< "  LS " << EvtInfo_LumiSection
// 			<< "  event " << EvtInfo_EventNum
// 			<< std::endl;
// 	      // plot it
// 	      // wpVec.at(w))+"_"+(trigVec.at(i))
// 	      // hists[which+"_ScEt"]->Fill( invMass_ );
// 	      //std::cout << "Now filling hists[" << "WP95_AllTrigs_invMass" << "]" << std::endl;
// 	      hists["WP95_AllTrigs_Trigger_invMass"]->Fill( invMass_ );
// 	      if (where1 == "EB" && where2 == "EB")
// 		{
// 		  hists["WP95_AllTrigs_Trigger_EBEB_invMass"]->Fill( invMass_ );
// 		}
// 	      if ((where1 == "EB" && where2 == "EE") || (where1 == "EE" && where1 == "EB"))
// 		{
// 		  hists["WP95_AllTrigs_Trigger_EBEE_invMass"]->Fill( invMass_ );
// 		}
// 	      if (where1 == "EE" && where2 == "EE")
// 		{
// 		  hists["WP95_AllTrigs_Trigger_EEEE_invMass"]->Fill( invMass_ );
// 		}
// 	      // 		      if (!histForCombTrigsFilled)
// 	      // 			{
// 	      // 			  hists["WP95_CombinedTrigs_invMass"]->Fill( invMass_ );
// 	      // 			  histForCombTrigsFilled = true;
// 	      // 			}
// 	    } // end of pass1 and pass2
	} // elec2
    } // elec1
  hists["nElectronsPostCuts"]->Fill(countPostCuts);
  if (countPostCuts > 0)
    {
//       std::cout << "Run " << EvtInfo_RunNum << "  event " << EvtInfo_EventNum 
// 		<< " has " << countPostCuts << " electron(s)" << std::endl;
    }
  //	} // trigs
  //    } // wp's
}

// void
// trigEff::matchElectronsToHlt( double dRmatch )
// {
//   vector<string> trigVec = stringParser(triggersOfInterestHlt);

//   vector<double> *hltEta = hltEle10LWEta;
//   vector<double> *hltPhi = hltEle10LWPhi;
//   vector<double> *hltPt = hltEle10LWPt;

//   map<unsigned int, unsigned int> gotMatch;

//   for ( unsigned int trg = 0; trg < trigVec.size(); ++trg )
//     {
//       gotMatch.clear();

//       if ( trigVec.at(trg) == "HLT_Ele10_LW_L1R" )
// 	{
// 	  hltEta = hltEle10LWEta;
// 	  hltPhi = hltEle10LWPhi;
// 	  hltPt  = hltEle10LWPt;
// 	}
//       else if ( trigVec.at(trg) == "HLT_Ele15_LW_L1R" )
// 	{
// 	  hltEta = hltEle15LWEta;
// 	  hltPhi = hltEle15LWPhi;
// 	  hltPt  = hltEle15LWPt;
// 	}
//       else if ( trigVec.at(trg) == "HLT_Photon10_L1R" || trigVec.at(trg) == "HLT_Photon15NewThresh_L1R" )
// 	{
// 	  hltEta = hltPhoton10Eta;
// 	  hltPhi = hltPhoton10Phi;
// 	  hltPt  = hltPhoton10Pt;
// 	}
//       else if ( trigVec.at(trg) == "HLT_Photon15_L1R" )
// 	{
// 	  hltEta = hltPhoton15Eta;
// 	  hltPhi = hltPhoton15Phi;
// 	  hltPt  = hltPhoton15Pt;
// 	}

//       double dr = 0;
//       for ( unsigned int elec = 0; elec < recoElectronCollection.size(); ++elec )
// 	{
// 	  double closestDr = 50;
// 	  unsigned int closestHlt = 999;

// 	  for ( unsigned int i = 0; i < hltEta->size(); ++i )
// 	    {
// 	      if ( gotMatch[i] == 1 ) continue;

// 	      dr = deltaR( hltEta->at(i), hltPhi->at(i), recoElectronCollection.at(elec).Eta(), recoElectronCollection.at(elec).Phi() );

// 	      if ( trigVec.at(trg) == "HLT_Photon15NewThresh_L1R"
// 		   && hltPt->at(i) >= 15
// 		   && dr < dRmatch && dr < closestDr )
// 		{
// 		  closestDr = dr;
// 		  closestHlt = i;
// 		}
// 	      if ( trigVec.at(trg) != "HLT_Photon15NewThresh_L1R"
// 		   && dr < dRmatch && dr < closestDr )
// 		{
// 		  closestDr = dr;
// 		  closestHlt = i;
// 		}
// 	    }

// 	  if ( closestDr < dRmatch )
// 	    gotMatch[closestHlt] = 1;

// 	  hltElecMatch[trigVec.at(trg)].push_back(closestHlt);
// 	}
//     }
// }

bool
ZeePlots::foundElectronMatchToHlt( std::string trigger, double dRmatch, unsigned int which)
{
  vector<double> *hltEta = hltEle10LWEta;
  vector<double> *hltPhi = hltEle10LWPhi;
  vector<double> *hltPt = hltEle10LWPt;
  vector<double> *hltEt = hltEle10LWEt;

  //  std::cout << "Trigger is " << trigger << std::endl;
  
  if ( trigger == "HLT_Ele10_LW_L1R" )
  {
    hltEta = hltEle10LWEta;
    hltPhi = hltEle10LWPhi;
    hltPt  = hltEle10LWPt;
    hltEt  = hltEle10LWEt;
  }
  else if ( trigger == "HLT_Ele15_LW_L1R" )
  {
    hltEta = hltEle15LWEta;
    hltPhi = hltEle15LWPhi;
    hltPt  = hltEle15LWPt;
    hltEt  = hltEle15LWEt;
  }
  else if ( trigger == "HLT_Ele20_LW_L1R" )
  {
    hltEta = hltEle20LWEta;
    hltPhi = hltEle20LWPhi;
    hltPt  = hltEle20LWPt;
    hltEt  = hltEle20LWEt;
  }
  else if ( trigger == "HLT_Ele10_SW_L1R" )
  {
    hltEta = hltEle10SWEta;
    hltPhi = hltEle10SWPhi;
    hltPt  = hltEle10SWPt;
    hltEt  = hltEle10SWEt;
  }
  else if ( trigger == "HLT_Ele15_SW_L1R" )
  {
    hltEta = hltEle15SWEta;
    hltPhi = hltEle15SWPhi;
    hltPt  = hltEle15SWPt;
    hltEt  = hltEle15SWEt;
  }
  else if ( trigger == "HLT_Ele20_SW_L1R" )
  {
    hltEta = hltEle20SWEta;
    hltPhi = hltEle20SWPhi;
    hltPt  = hltEle20SWPt;
    hltEt  = hltEle20SWEt;
  }
  else if ( trigger == "HLT_Ele25_SW_L1R" )
  {
    hltEta = hltEle25SWEta;
    hltPhi = hltEle25SWPhi;
    hltPt  = hltEle25SWPt;
    hltEt  = hltEle25SWEt;
  }
  else if ( trigger == "HLT_Photon10_L1R" || trigger == "HLT_Photon15NewThresh_L1R" )
  {
    hltEta = hltPhoton10Eta;
    hltPhi = hltPhoton10Phi;
    hltPt  = hltPhoton10Pt;
    hltEt  = hltPhoton10Et;
  }
  else if ( trigger == "HLT_Photon15_L1R" )
  {
    hltEta = hltPhoton15Eta;
    hltPhi = hltPhoton15Phi;
    hltPt  = hltPhoton15Pt;
    hltEt  = hltPhoton15Et;
  }
  else if ( trigger == "HLT_Photon20_L1R" )
  {
    hltEta = hltPhoton20Eta;
    hltPhi = hltPhoton20Phi;
    hltPt  = hltPhoton20Pt;
    hltEt  = hltPhoton20Et;
  }
  else if ( trigger == "HLT_Photon15_Cleaned_L1R" )
  {
    hltEta = hltPhoton15CleanEta;
    hltPhi = hltPhoton15CleanPhi;
    hltPt  = hltPhoton15CleanPt;
    hltEt  = hltPhoton15CleanEt;
  }
  else if ( trigger == "HLT_Photon20_Cleaned_L1R" )
  {
    hltEta = hltPhoton20CleanEta;
    hltPhi = hltPhoton20CleanPhi;
    hltPt  = hltPhoton20CleanPt;
    hltEt  = hltPhoton20CleanEt;
  }
  else if ( trigger == "HLT_Photon30_Cleaned_L1R" )
  {
    hltEta = hltPhoton30CleanEta;
    hltPhi = hltPhoton30CleanPhi;
    hltPt  = hltPhoton30CleanPt;
    hltEt  = hltPhoton30CleanEt;
  }

  for ( unsigned int i = 0; i < hltEta->size(); ++i )
  {
//     if ( deltaR( hltEta->at(i), hltPhi->at(i), recoElectronCollection.at(which).Eta(), recoElectronCollection.at(which).Phi() ) < dRmatch )
//       return true;
//    std::cout << "Loop " << i << " of " << hltEta->size() << std::endl;

//     if ( usedHltCands[trigger].find(50+i) != usedHltCands[trigger].end() )
//       {
// 	std::cout << "Cand already used" << std::endl;
// 	continue;
//       }

    if ( trigger == "HLT_Photon15NewThresh_L1R"
         && hltPt->at(i) >= 15
         && deltaR( hltEta->at(i), hltPhi->at(i), recoElectronCollection.at(which).Eta(), recoElectronCollection.at(which).Phi() ) < dRmatch )
      {
	//	std::cout << "Photon15 new thresh passed" << std::endl;
	//	usedHltCands[trigger].insert(50+i);
	return true;
      }

    if ( trigger != "HLT_Photon15NewThresh_L1R" 
         && deltaR( hltEta->at(i), hltPhi->at(i), recoElectronCollection.at(which).Eta(), recoElectronCollection.at(which).Phi() ) < dRmatch )
      {
	//	std::cout << "Non-Photon15 new thresh passed" << std::endl;
	//	usedHltCands[trigger].insert(50+i);
	return true;
      }
  }
  
  //  std::cout << "Match not found" << std::endl;
  return false;
}

void
ZeePlots::fillRecoElectronHistoCollection(string whichHisto, unsigned int ele)
{
  hists[whichHisto+"_ScEt"]->Fill( recoElectronCollection.at(ele).ScEt() );
//   hists[whichHisto+"_Et"]->Fill( recoElectronCollection.at(ele).Et() );
//  hists[whichHisto+"_Pt"]->Fill( recoElectronCollection.at(ele).Pt() );
  if ( recoElectronCollection.at(ele).ScEt() > 20 )
  {
    hists[whichHisto+"_Eta"]->Fill( recoElectronCollection.at(ele).Eta() );
    hists[whichHisto+"_Phi"]->Fill( recoElectronCollection.at(ele).Phi() );
  }
//   hists[which+"_HoE"]->Fill( recoElectronCollection.at(0).HoE() );
//   hists[which+"_dEtaIn"]->Fill( recoElectronCollection.at(0).DeltaEtaIn() );
//   hists[which+"_dPhiIn"]->Fill( recoElectronCollection.at(0).DeltaPhiIn() );
//   hists[which+"_SigIeIe"]->Fill( recoElectronCollection.at(0).SigIeIe() );
//   
//   if ( recoElectronCollection.at(0).ClosestL1IsIso() )
//   {
//     hists[which+"_L1Et"]->Fill( l1IsoEleEt->at(recoElectronCollection.at(0).ClosestL1() ) );
//     hists[which+"_L1DeltaEt"]->Fill( recoElectronCollection.at(0).Et() - l1IsoEleEt->at(recoElectronCollection.at(0).ClosestL1() ) );
//     hists[which+"_L1DeltaEtRel"]->Fill( ( recoElectronCollection.at(0).Et() - l1IsoEleEt->at(recoElectronCollection.at(0).ClosestL1() ) ) / recoElectronCollection.at(0).Et() );
//   }
//   else
//   {
//     hists[which+"_L1Et"]->Fill( l1NonIsoEleEt->at(recoElectronCollection.at(0).ClosestL1() ) );
//     hists[which+"_L1DeltaEt"]->Fill( recoElectronCollection.at(0).Et() - l1NonIsoEleEt->at(recoElectronCollection.at(0).ClosestL1() ) );
//     hists[which+"_L1DeltaEtRel"]->Fill( ( recoElectronCollection.at(0).Et() - l1NonIsoEleEt->at(recoElectronCollection.at(0).ClosestL1() ) ) / recoElectronCollection.at(0).Et() );
//   }
// 
//   hists[which+"_MaxOver3x3"]->Fill( ElectroneleMaxOver3x3->at(recoElectronCollection.at(0).Pos()) );
}


bool
ZeePlots::applyRecoEleCut(std::string cut, const WP & wp, unsigned int which)
{ 
  if      ( cut == "NoCuts"  )       return true;
  else if ( cut == "SpikesRemoved" ) return ElectroneleMaxOver3x3->at(recoElectronCollection.at(which).Pos()) < 0.9 ; 
  else if ( cut == "SCEt"    )       return recoElectronCollection.at(which).ScEt() > 20. ; // 0 ;
  else if ( cut == "MissingHits" )   return ElectroneleMissingHits->at(recoElectronCollection.at(which).Pos()) <= wp.missingHits;
  else if ( cut == "Dist" )          return fabs(ElectroneleDist->at(recoElectronCollection.at(which).Pos()))  >= wp.dist;
  else if ( cut == "deltaCotTheta" ) return fabs(ElectroneleDeltaCotTheta->at(recoElectronCollection.at(which).Pos())) >= wp.deltaCotTheta;
  else if ( cut == "RelIsoComb" )    return ElectroneleRelIsoComb->at(recoElectronCollection.at(which).Pos()) < wp.combinedIso;
  else if ( cut == "RelIsoEcal" )    return ElectroneleRelIsoEcal->at(recoElectronCollection.at(which).Pos()) < wp.relEcalIso;
  else if ( cut == "RelIsoHcal" )    return ElectroneleRelIsoHcal->at(recoElectronCollection.at(which).Pos()) < wp.relHcalIso;
  else if ( cut == "RelIsoTrk"  )    return ElectroneleRelIsoTrk->at(recoElectronCollection.at(which).Pos())  < wp.relTrackIso;
  else if ( cut == "SigIeIe" )       return recoElectronCollection.at(which).SigIeIe()  < wp.sigIeIe ;
  else if ( cut == "deltaPhiIn" )    return fabs( recoElectronCollection.at(which).DeltaPhiIn() ) < wp.dPhiIn;
  else if ( cut == "deltaEtaIn" )    return fabs( recoElectronCollection.at(which).DeltaEtaIn() ) < wp.dEtaIn;
  else if ( cut == "HoE"     )       return recoElectronCollection.at(which).HoE() < wp.hoE;
  else if ( cut == "TriggerMatchL1") return recoElectronCollection.at(which).ClosestL1dr() < 0.5 ;
  else if ( cut == "TrkDrv" )        return !( ElectroneleTrkDriven->at(recoElectronCollection.at(which).Pos() ) );
  else if ( cut == "EcalDrv" )       return !( ElectroneleEcalDriven->at(recoElectronCollection.at(which).Pos() ) );
  else throw std::runtime_error("Unknown cut string \'" + cut + "\'");
}

bool
ZeePlots::applyRecoEleWp95Cut(std::string cut, unsigned int which)
{ 
  if      ( cut == "NoCuts"  )       return true;
  else if ( cut == "SpikesRemoved" ) return ElectronelePassWp95SpikeCut->at(recoElectronCollection.at(which).Pos() ); 
  else if ( cut == "SCEt"    )       return ElectronelePassWp95ScEtCut->at(recoElectronCollection.at(which).Pos() );
  else if ( cut == "MissingHits" )   return ElectronelePassWp95MissHitsCut->at(recoElectronCollection.at(which).Pos() );
  else if ( cut == "Dist" )          return ElectronelePassWp95DistCut->at(recoElectronCollection.at(which).Pos() );
  else if ( cut == "deltaCotTheta" ) return ElectronelePassWp95DCotThetaCut->at(recoElectronCollection.at(which).Pos() );
  else if ( cut == "ConvRej" )       return ElectronelePassWp95ConvRejCut->at(recoElectronCollection.at(which).Pos() );
  else if ( cut == "RelIsoComb" )    return ElectronelePassWp95RelIsoCombCut->at(recoElectronCollection.at(which).Pos() );
  else if ( cut == "RelIsoEcal" )    return ElectronelePassWp95RelIsoEcalCut->at(recoElectronCollection.at(which).Pos() );
  else if ( cut == "RelIsoHcal" )    return ElectronelePassWp95RelIsoHcalCut->at(recoElectronCollection.at(which).Pos() );
  else if ( cut == "RelIsoTrk"  )    return ElectronelePassWp95RelIsoTrkCut->at(recoElectronCollection.at(which).Pos() );
  else if ( cut == "RelIsoIndiv"  )  return ElectronelePassWp95RelIsoIndivCut->at(recoElectronCollection.at(which).Pos() );
  else if ( cut == "SigIeIe" )       return ElectronelePassWp95SieieCut->at(recoElectronCollection.at(which).Pos() );
  else if ( cut == "deltaPhiIn" )    return ElectronelePassWp95DphiinCut->at(recoElectronCollection.at(which).Pos() );
  else if ( cut == "deltaEtaIn" )    return ElectronelePassWp95DetainCut->at(recoElectronCollection.at(which).Pos() );
  else if ( cut == "HoE"     )       return ElectronelePassWp95HoeCut->at(recoElectronCollection.at(which).Pos() );
  else if ( cut == "Eid"     )       return ElectronelePassWp95EidCut->at(recoElectronCollection.at(which).Pos() );
  else if ( cut == "WP95"     )      return ElectronelePassWp95->at(recoElectronCollection.at(which).Pos() );
  //  else if ( cut == "TriggerMatchL1") return ;
  //  else if ( cut == "TrkDrv" )        return ;
  //  else if ( cut == "EcalDrv" )       return ;
  else throw std::runtime_error("Unknown cut string \'" + cut + "\'");
}

bool 
ZeePlots::selectRecoElectrons()
{
  recoElectronCollection.clear();
  
  double postSelectedElectrons = 0;

  hists["nElectronsPreSelection"]->Fill(ElectronEt->size());

  for ( unsigned int i = 0; i < ElectronEt->size(); ++i )
  {

    //    if ( !( electronInEta( ElectronEta->at(i) ) ) ) continue;
    if ( !( electronInEta( ElectroneleScEta->at(i) ) ) ) continue;
    
    postSelectedElectrons++;

    electron ele(this, i);

    // add electron to all electrons vector, properly sorted 
    bool inserted = false;
    for ( std::vector<electron>::iterator itr = recoElectronCollection.begin(); itr != recoElectronCollection.end(); ++itr)
    {
      if ( ele.ScEt() > itr->ScEt() )
      {
          recoElectronCollection.insert( itr, ele );
          inserted = true;
          break;
      }
    }

    if ( !inserted ) recoElectronCollection.push_back(ele);
  }

  hists["nElectronsPostSelection"]->Fill(postSelectedElectrons);
  
  return !( recoElectronCollection.empty() );
}



void
ZeePlots::BookHistograms()
{
  string y = "events";
  //  hBins bin_Et         = { y, "E_{T}"              ,  10,  0.  ,  60.   };
  hBins bin_ScEt         = { y, "E_{T}"              ,  20,  0.  ,  100.   };
  //hBins bin_invMass    = { y, "M_{inv} (GeV)"      ,  20,  0.  ,  120.   };
  hBins bin_invMass    = { y, "M_{inv} (GeV)"      ,  48,  0.  ,  120.   };
  //  double varBinsEt[11] = { 0, 2, 4, 6, 8, 10, 12, 16, 20, 25, 60 }; 
  //  hBins bin_Pt         = { y, "P_{T}"              ,  10,  0.  ,  60.   };
//   hBins bin_deltaEt    = { y, "#Delta E_{T}"       , 100, -20.  ,   20.   };
//   hBins bin_deltaEtrel = { y, "#Delta E_{T, rel}"  , 100, -2.  ,   2.   };
  hBins bin_Eta        = { y, "#eta"               ,  20, -4.  ,   4.   };
  hBins bin_Phi        = { y, "#phi"               ,  20, -4.  ,   4.   };

  hBins bin_idHoE      = { y, "H/E"                , 100,  0.  ,   0.05 };
  hBins bin_idDphi     = { y, "#Delta_{#phiin}"    , 100, -0.1 ,   0.1  };
  hBins bin_idDeta     = { y, "#Delta_{#etain}"    , 140, -0.12,   0.12 };
  hBins bin_idDsIeIe   = { y, "#sigma_{i#etai#eta}", 140,  0.  ,   0.07 };

  hBins bin_isoCombRel = { y, "Isolation (GeV)"    , 100,  0.  ,   1.   };

//   hBins bin_MaxOver3x3 = { y, "Max/3x3"            ,  50,  0.  ,   1.00 };
  
  string where[3] = { "EB", "EE", "ALL" };

  std::vector<std::string> trigVec = stringParser(triggersOfInterestL1 + ";" + triggersOfInterestHlt);
  std::string base = "";
  
  for ( unsigned int w = 0; w < wpVec.size(); ++w )
  {
    for ( unsigned int i = 0; i < trigVec.size(); ++i )
    {
      for ( unsigned int j = 0; j < cutVec.size(); ++j )
      {
        for ( unsigned int k = 0; k < 3; ++k) // where
        {
          base = (wpVec.at(w))+"_"+(trigVec.at(i))+"_"+(cutVec.at(j))+"_"+(itos(j))+"_"+where[k];
	  //          base = "TrigEff_"+(wpVec.at(w))+"_"+(trigVec.at(i))+"_"+(cutVec.at(j))+"_"+(itos(j))+"_"+where[k];

	  // filled per electron
          hists[base+"_ScEt"]    = myTH1D(base+"_ScEt",bin_ScEt);
//           hists[base+"_Et"]      = myTH1D(base+"_Et",bin_Et);
//          hists[base+"_Pt"]      = myTH1D(base+"_Pt",bin_Pt);
          hists[base+"_Eta"]     = myTH1D(base+"_Eta",bin_Eta);
          hists[base+"_Phi"]     = myTH1D(base+"_Phi",bin_Phi);
          hists[base+"_HoE"]     = myTH1D(base+"_HoE",bin_idHoE);
          hists[base+"_dEtaIn"]  = myTH1D(base+"_dEtaIn",bin_idDphi);
          hists[base+"_dPhiIn"]  = myTH1D(base+"_dPhiIn",bin_idDeta);
          hists[base+"_SigIeIe"] = myTH1D(base+"_SigIeIe",bin_idDsIeIe);

          hists[base+"_CombRelIso"] = myTH1D(base+"_CombRelIso",bin_isoCombRel);

//           hists[base+"_L1Et"]          = myTH1D(base+"_L1Et",bin_Et);
//           hists[base+"_L1DeltaEt"]     = myTH1D(base+"_L1DeltaEt",bin_deltaEt);
//           hists[base+"_L1DeltaEtRel"]  = myTH1D(base+"_L1DeltaEtRel",bin_deltaEtrel);
// 
//           hists[base+"_MaxOver3x3"]    = myTH1D(base+"_MaxOver3x3",bin_MaxOver3x3);
        } // end loop over eta ("where")
      } // end loop over cuts

      // histograms for each trigger and WP
      //      base = "InvMass_"+(wpVec.at(w))+"_"+(trigVec.at(i));
      base = (wpVec.at(w))+"_"+(trigVec.at(i));
      //      hists[base+"_ScEt"]    = myTH1D(base+"_ScEt",bin_Et, varBinsEt); // HOW TO USE VARBINS TO REBIN

      // filled per electron
      hists[base+"_ScEt"]    = myTH1D(base+"_ScEt",bin_ScEt);
      hists[base+"_Eta"]     = myTH1D(base+"_Eta",bin_Eta);
      hists[base+"_Phi"]     = myTH1D(base+"_Phi",bin_Phi);
      hists[base+"_HoE"]     = myTH1D(base+"_HoE",bin_idHoE);
      hists[base+"_dEtaIn"]  = myTH1D(base+"_dEtaIn",bin_idDphi);
      hists[base+"_dPhiIn"]  = myTH1D(base+"_dPhiIn",bin_idDeta);
      hists[base+"_SigIeIe"] = myTH1D(base+"_SigIeIe",bin_idDsIeIe);
      
      hists[base+"_CombRelIso"] = myTH1D(base+"_CombRelIso",bin_isoCombRel);

      // filled per dielectron
      hists[base+"_invMass"]    = myTH1D(base+"_invMass",bin_invMass);
    } // end loop over trigs

    // histograms for each WP (passing any trigger of interest)
    base = (wpVec.at(w))+"_CombinedTrigs";

    // filled per electron
    hists[base+"_ScEt"]    = myTH1D(base+"_ScEt",bin_ScEt);
    hists[base+"_Eta"]     = myTH1D(base+"_Eta",bin_Eta);
    hists[base+"_Phi"]     = myTH1D(base+"_Phi",bin_Phi);
    hists[base+"_HoE"]     = myTH1D(base+"_HoE",bin_idHoE);
    hists[base+"_dEtaIn"]  = myTH1D(base+"_dEtaIn",bin_idDphi);
    hists[base+"_dPhiIn"]  = myTH1D(base+"_dPhiIn",bin_idDeta);
    hists[base+"_SigIeIe"] = myTH1D(base+"_SigIeIe",bin_idDsIeIe);
    
    hists[base+"_CombRelIso"] = myTH1D(base+"_CombRelIso",bin_isoCombRel);
    
    // filled per dielectron
    hists[base+"_invMass"]    = myTH1D(base+"_invMass",bin_invMass);
  } // end loop over wp's
  
  cout << "Booked histograms\n";
}

void
ZeePlots::BookAnalysisHistograms()
{
  string y = "events";
  //  hBins bin_Et         = { y, "E_{T}"              ,  10,  0.  ,  60.   };
  //  hBins bin_ScEt         = { y, "E_{T}"              ,  20,  0.  ,  100.   };
  hBins bin_ScEt         = { y, "E_{T}"              ,  50,  0.  ,  100.   };
  //  hBins bin_invMass    = { y, "M_{inv} (GeV)"      ,  48,  0.  ,  120.   };
  //  hBins bin_invMass    = { y, "M_{inv} (GeV)"      ,  20,  0.  ,  120.   };  // OLD
  hBins bin_invMass    = { y, "M_{inv} (GeV)"      ,  30,  60.  ,  120.   };  
  //  double varBinsEt[11] = { 0, 2, 4, 6, 8, 10, 12, 16, 20, 25, 60 }; 
  hBins bin_Eta        = { y, "#eta"               ,  20, -4.  ,   4.   };
  hBins bin_Phi        = { y, "#phi"               ,  20, -4.  ,   4.   };

  hBins bin_crMissHit  = { y, "Missing Hits"       , 10,    0. ,  10.  };
  hBins bin_crDist     = { y, "Dist"               , 100,  -0.2 ,  0.2  };
  hBins bin_crDCotTh   = { y, "#deltacot#theta"    , 100,  -0.2 ,  0.2  };

  hBins bin_idHoE      = { y, "H/E"                , 100,  0.  ,   0.2 };
  hBins bin_idDphi     = { y, "#Delta_{#phiin}"    , 100, -0.1 ,   0.1  };
  hBins bin_idDeta     = { y, "#Delta_{#etain}"    , 140, -0.12,   0.12 };
  hBins bin_idDsIeIe   = { y, "#sigma_{i#etai#eta}", 140,  0.  ,   0.07 };

  hBins bin_isoCombRel = { y, "Isolation (GeV)"    , 100,  0.  ,   1.   };

  //   hBins bin_MaxOver3x3 = { y, "Max/3x3"            ,  50,  0.  ,   1.00 };

  hBins bin_nElectrons = { y, "Number of electrons",  20,  0.  ,  20.   };
  
  string where[3] = { "EB", "EE", "ALL" };
  
  //  std::vector<std::string> trigVec = stringParser(triggersOfInterestL1 + ";" + triggersOfInterestHlt);
  std::string base = "";
  
  for ( unsigned int j = 0; j < cutVec.size(); ++j )
    {
      // plots filled according to eta region
      for ( unsigned int k = 0; k < 3; ++k) // where
        {
          base = "WP95_AllTrigs_"+(cutVec.at(j))+"_"+(itos(j))+"_"+where[k];
	  
	  // filled per electron
          hists[base+"_ScEt"]       = myTH1D(base+"_ScEt",bin_ScEt);
          hists[base+"_Phi"]        = myTH1D(base+"_Phi",bin_Phi);

	  if ( j+1 < cutVec.size() )
	    {
	      // plots of conv rej vars
	      if ( cutVec.at(j+1).compare("ConvRej") == 0 )
		{
		  hists[base+"_MissHits"]   = myTH1D(base+"_MissHits",bin_crMissHit);
		  hists[base+"_Dist"]       = myTH1D(base+"_Dist",bin_crDist);
		  hists[base+"_DCotTh"]     = myTH1D(base+"_DCotTh",bin_crDCotTh);
		}
	      if ( cutVec.at(j+1).compare("MissingHits") == 0 )
		{
                   hists[base+"_MissHits"]   = myTH1D(base+"_MissHits",bin_crMissHit);
		}
	      if ( cutVec.at(j+1).compare("Dist") == 0 )
		{
 		  hists[base+"_Dist"]       = myTH1D(base+"_Dist",bin_crDist);
		}
	      if ( cutVec.at(j+1).compare("deltaCotTheta") == 0 )
		{
 		  hists[base+"_DCotTh"]     = myTH1D(base+"_DCotTh",bin_crDCotTh);
		}

	      // plots of iso vars	      
	      if ( cutVec.at(j+1).compare("RelIsoComb") == 0 ) 
		{
 		  hists[base+"_CombRelIso"] = myTH1D(base+"_CombRelIso",bin_isoCombRel);
		}
//               if ( cutVec.at(j+1).compare("RelIsoTrk") == 0 )
//                 {
// 		}
//               if ( cutVec.at(j+1).compare("RelIsoEcal") == 0 )
//                 {
// 		}
//               if ( cutVec.at(j+1).compare("RelIsoHcal") == 0 )
//                 {
// 		}
	      
	      // plots of eid vars
	      if ( cutVec.at(j+1).compare("Eid") == 0 )
		{
		  hists[base+"_HoE"]        = myTH1D(base+"_HoE",bin_idHoE);
		  hists[base+"_dEtaIn"]     = myTH1D(base+"_dEtaIn",bin_idDphi);
		  hists[base+"_dPhiIn"]     = myTH1D(base+"_dPhiIn",bin_idDeta);
		  hists[base+"_SigIeIe"]    = myTH1D(base+"_SigIeIe",bin_idDsIeIe);
		}
              if ( cutVec.at(j+1).compare("HoE") == 0 )
                {
 		  hists[base+"_HoE"]        = myTH1D(base+"_HoE",bin_idHoE);
		}
              if ( cutVec.at(j+1).compare("deltaEtaIn") == 0 )
                {
 		  hists[base+"_dEtaIn"]     = myTH1D(base+"_dEtaIn",bin_idDphi);
		}
              if ( cutVec.at(j+1).compare("deltaPhiIn") == 0 )
                {
 		  hists[base+"_dPhiIn"]     = myTH1D(base+"_dPhiIn",bin_idDeta);
		}
              if ( cutVec.at(j+1).compare("SigIeIe") == 0 )
                {
 		  hists[base+"_SigIeIe"]    = myTH1D(base+"_SigIeIe",bin_idDsIeIe);
		}
	    }

        } // end loop over eta ("where")
      
      base = "WP95_AllTrigs_"+(cutVec.at(j))+"_"+(itos(j));
      hists[base+"_Eta"]        = myTH1D(base+"_Eta",bin_Eta);
      hists[base+"_invMass"]    = myTH1D(base+"_invMass",bin_invMass);
      hists[base+"_invMassCorr"]    = myTH1D(base+"_invMassCorr",bin_invMass);
      hists[base+"_EBEB_invMass"]    = myTH1D(base+"_EBEB_invMass",bin_invMass);
      hists[base+"_EBEE_invMass"]    = myTH1D(base+"_EBEE_invMass",bin_invMass);
      hists[base+"_EEEE_invMass"]    = myTH1D(base+"_EEEE_invMass",bin_invMass);
      hists[base+"_BothPass_ScEt"] = myTH1D(base+"_BothPass_ScEt",bin_ScEt);

      hists[base+"_Zpt"]          = myTH1D(base+"_Zpt",bin_ScEt);
      hists[base+"_Zeta"]         = myTH1D(base+"_Zeta",bin_Eta);
      hists[base+"_Zrapidity"]    = myTH1D(base+"_Zrapidity",bin_Eta);
      hists[base+"_Zphi"]         = myTH1D(base+"_Zphi",bin_Phi);
      
    } // end loop over cuts
  
//   // after trigger
//   for ( unsigned int k = 0; k < 3; ++k) // where
//     {
//       //          base = (wpVec.at(w))+"_"+(trigVec.at(i))+"_"+(cutVec.at(j))+"_"+(itos(j))+"_"+where[k];
//       //          base = "TrigEff_"+(wpVec.at(w))+"_"+(trigVec.at(i))+"_"+(cutVec.at(j))+"_"+(itos(j))+"_"+where[k];
//       base = "WP95_AllTrigs_Trigger_"+where[k];
      
//       // filled per electron
//       hists[base+"_ScEt"]    = myTH1D(base+"_ScEt",bin_ScEt);
//       //           hists[base+"_Et"]      = myTH1D(base+"_Et",bin_Et);
//       //          hists[base+"_Pt"]      = myTH1D(base+"_Pt",bin_Pt);
//       hists[base+"_Eta"]     = myTH1D(base+"_Eta",bin_Eta);
//       hists[base+"_Phi"]     = myTH1D(base+"_Phi",bin_Phi);
//       hists[base+"_HoE"]     = myTH1D(base+"_HoE",bin_idHoE);
//       hists[base+"_dEtaIn"]  = myTH1D(base+"_dEtaIn",bin_idDphi);
//       hists[base+"_dPhiIn"]  = myTH1D(base+"_dPhiIn",bin_idDeta);
//       hists[base+"_SigIeIe"] = myTH1D(base+"_SigIeIe",bin_idDsIeIe);
      
//       hists[base+"_CombRelIso"] = myTH1D(base+"_CombRelIso",bin_isoCombRel);
      
//       //           hists[base+"_L1Et"]          = myTH1D(base+"_L1Et",bin_Et);
//       //           hists[base+"_L1DeltaEt"]     = myTH1D(base+"_L1DeltaEt",bin_deltaEt);
//       //           hists[base+"_L1DeltaEtRel"]  = myTH1D(base+"_L1DeltaEtRel",bin_deltaEtrel);
//       // 
//       //           hists[base+"_MaxOver3x3"]    = myTH1D(base+"_MaxOver3x3",bin_MaxOver3x3);
//     } // end loop over eta ("where")
  
//   base = "WP95_AllTrigs_Trigger";
//   hists[base+"_invMass"]    = myTH1D(base+"_invMass",bin_invMass);
//   hists[base+"_EBEB_invMass"]    = myTH1D(base+"_EBEB_invMass",bin_invMass);
//   hists[base+"_EBEE_invMass"]    = myTH1D(base+"_EBEE_invMass",bin_invMass);
//   hists[base+"_EEEE_invMass"]    = myTH1D(base+"_EEEE_invMass",bin_invMass);



  hists["nElectronsPreSelection"] = myTH1D("nElectronsPreSelection",bin_nElectrons);
  hists["nElectronsPostSelection"] = myTH1D("nElectronsPostSelection",bin_nElectrons);

  hists["nElectronsPreCuts"] = myTH1D("nElectronsPreCuts",bin_nElectrons);
  hists["nElectronsPostCuts"] = myTH1D("nElectronsPostCuts",bin_nElectrons);

  cout << "Booked analysis histograms\n";
}

void 
ZeePlots::setWp(std::string givenWp, WP & wpEb, WP & wpEe)
{
  WP jess(0, 0, 0, 0, 0, 0, 0, 0, 0.05, 0, 0.1 );
  //         missingHits dist  deltaCotTheta combinedIso relTrackIso relEcalIso relHcalIso sigIeIe dphiin detain hoe
  WP wp95EB( 1,          0.00, 0.00,         0.15,       0.15,       2.00,      0.12,      0.01,    0.8,  0.007, 0.15 ); //0.50 );
  WP wp95EE( 1,          0.00, 0.00,         0.10,       0.08,       0.06,      0.05,      0.03,    0.7,  999. , 0.07 ); //0.010, 0.07 );
  
  // cross-check numbers!
//   WP wp90EB( 1, 0.02, 0.02, 0.10, 0.12, 0.09, 0.10, 0.01, 0.8, 0.007, 0.12 );
//   WP wp90EE( 1, 0.02, 0.02, 0.07, 0.05, 0.06, 0.03, 0.03, 0.7, 0.009, 0.05 );
   
  WP wp80EB( 0, 0.02, 0.02, 0.07, 0.09, 0.07, 0.10 , 0.01, 0.06, 0.004, 0.040 );
  WP wp80EE( 0, 0.02, 0.02, 0.06, 0.04, 0.05, 0.025, 0.03, 0.03, 999. , 0.025 );//0.007, 0.025 );

// cross-check numbers!
//   WP wp60EB( 0, 0.02, 0.02, 0.03, 0.04 , 0.04, 0.03, 0.01, 0.025, 0.004, 0.025 );
//   WP wp60EE( 0, 0.02, 0.02, 0.02, 0.025, 0.02, 0.02, 0.03, 0.02 , 0.005, 0.025 );
  
  if ( givenWp.find ("95") != std::string::npos )
  {
    wpEb = wp95EB;
    wpEe = wp95EE;
  }
//   else if ( givenWp.find ("90") != std::string::npos )
//   {
//     wpEb = wp90EB;
//     wpEe = wp90EE;
//   }
  else if ( givenWp.find ("80") != std::string::npos )
  {
    wpEb = wp80EB;
    wpEe = wp80EE;
  }
//   else if ( givenWp.find ("60") != std::string::npos )
//   {
//     wpEb = wp60EB;
//     wpEe = wp60EE;
//   }
  else
  {
    cout << "Unknown WP!\n";
    wpEb = wpEe = jess;
  }

}




std::vector<std::string> 
ZeePlots::stringParser(std::string cutString)
{
  std::vector<string> toReturn;
  std::stringstream s1;
  
  for(std::string::const_iterator itr=cutString.begin(); itr!=cutString.end(); itr++)
  {
    if((*itr)!=';')
      s1<<*itr;
    else
    {
      toReturn.push_back(s1.str());
      s1.str("");
    }
    
    if( itr==(cutString.end()-1))
    {
      toReturn.push_back(s1.str());
      s1.str("");
    }
  } 
  
  return toReturn;
}

double
ZeePlots::invMass(double mass1, double energy1, double px1, double py1, double pz1, double mass2, double energy2, double px2, double py2, double pz2) 
{
  // inv mass = sqrt(m_1^2 + m_2^2 + 2*(E_1*E_2 - (px1*px2 + py1*py2 + pz1+pz2) ) )
  double invMass = sqrt(mass1*mass1 + mass2*mass2 + 2*(energy1*energy2 - (px1*px2 + py1*py2 + pz1*pz2) ) );
  return invMass;
}
