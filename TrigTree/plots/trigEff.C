#include "trigEff.h"

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
    trigEff t(argv[1], -1);
  else if ( argc == 3 )
    trigEff t(argv[1], atol(argv[2]));
  
  return 0;
}

trigEff::trigEff(string sample, double limit) : sample_(sample)
{
  string plots = "Plots_" + sample.substr( sample.find_last_of("/") + 1, sample.size() );

  TFile *f = new TFile(sample.c_str());
  if ( sample.find("Slim") == std::string::npos ) f->cd( (sample+":/demo").c_str() );

  TTree *tree = (TTree*)f->Get("TrigTree");

  Init(tree);

  hFile = new TFile(plots.c_str(),"RECREATE");

  cout << "Creating " << plots << endl;

  Loop(!(sample.find("EG") == std::string::npos || sample.find("MB") == std::string::npos || sample.find("MinBias") == std::string::npos), limit);
}

trigEff::~trigEff()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
   
   hFile->Write();
   hFile->Close();
}



void trigEff::Loop(bool isData, double limit)
{
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();
   
   cutList_  =  "NoCuts;SpikesRemoved;SCEt";
   cutList_ += ";MissingHits;Dist;deltaCotTheta";
   cutList_ += ";RelIsoTrk;RelIsoEcal;RelIsoHcal";
//    cutList_ += ";RelIsoComb";
   cutList_ += ";SigIeIe;deltaPhiIn;deltaEtaIn;HoE";
   cutList_ += ";AllCuts";
//    cutList_ += ";EcalDrv";
   cutVec  = stringParser(cutList_);
   
   triggersOfInterestL1 = "NoTrigger;L1_SingleEG5;L1_SingleEG8";// L1_SingleEG5;L1_SingleEG8; 
   triggersOfInterestHlt = "HLT_Ele10_LW_L1R;HLT_Ele15_LW_L1R;HLT_Photon10_L1R;HLT_Photon15_L1R;HLT_Photon15NewThresh_L1R"; //HLT_Ele10_LW_L1R;HLT_Ele15_LW_L1R;HLT_Photon10_L1R;HLT_Photon15_L1R

   wpList_ = "WP80;WP95";//;WP95";
   wpVec = stringParser(wpList_);
   
   BookHistograms();
   std::map<double, double> runs;

   for (Long64_t jentry=0; jentry<nentries;jentry++) 
   {
      ientry = LoadTree(jentry);
      if (ientry < 0 || ientry == limit ) 
      {
        cout << "Stopped in event #" << ientry << endl;
        break;
      }

      fChain->GetEntry(jentry);
      
      if ( jentry%100000 == 0 ) std::cout << "Entry #" << jentry << std::endl;
      
//       if( sample_ == "data" && !(EvtInfo_BunchXing==1 || EvtInfo_BunchXing==1786)) continue;

      triggers.clear();
      triggers["NoTrigger"] = 1;
      for(unsigned int i=0; i < trigName->size(); i++)
        triggers[trigName->at(i)] = trigResults->at(i);

      for(unsigned int i=0; i < L1trigName->size(); i++)
        triggers[L1trigName->at(i)] = L1trigResults->at(i);
      
      triggers["HLT_Photon15NewThresh_L1R"] = triggers["HLT_Photon10_L1R"];
      
      // skip data events with no ecal activity
      if ( isData && !triggers["HLT_Activity_Ecal_SC7"] ) continue;
      
      runs[EvtInfo_RunNum] += 1;
//       if ( ientry == 82 )
//       {   
//         cout << "EvtInfo_EventNum: " << EvtInfo_EventNum << " : EvtInfo_RunNum : " << EvtInfo_RunNum << " : " << " : EvtInfo_LumiSection : " << EvtInfo_LumiSection << " : EvtInfo_BunchXing : "<< EvtInfo_BunchXing << endl;
//         for ( unsigned int i = 0; i < ElectronEt->size(); ++i )
//         {
//                 cout 
//                     << "reco :\tPt: " << ElectronPt->at(i)
//                     << "\tEt: " << ElectronEt->at(i)
//                     << "\tSCEt: " << ElectroneleScEt->at(i)
//                     << "\tEta: " << ElectronEta->at(i)
//                     << "\tPhi: " << ElectronPhi->at(i)
//                     << "\tEcal: " << ElectroneleEcalDriven->at(i)
//                     << "\tTrk: " << ElectroneleTrkDriven->at(i)
//                     << endl;
//         }
//         for ( unsigned int i = 0; i < hltPhoton15Pt->size(); ++i )
//         {
//                 cout 
//                     << "hlt :\tPt: " << hltPhoton15Pt->at(i)
//                     << "\tEta: " << hltPhoton15Eta->at(i)
//                     << "\tPhi: " << hltPhoton15Phi->at(i)
//                     << endl;
//         }        
//         
//       }

      if ( selectRecoElectrons(0.5) )
      {
        fillRecoElectronL1Histograms();
        fillRecoElectronHltHistograms(0.2);
      }
   }
   
   cout << "Processed runs: ";
   for ( std::map<double, double>::iterator itr = runs.begin(); itr != runs.end(); ++itr )
   {
    cout <<  itr->first << ", ";
   }
   cout << endl;
}


void
trigEff::fillRecoElectronL1Histograms()
{
  WP wpEb, wpEe;
   
  std::vector<std::string> trigVec = stringParser(triggersOfInterestL1);
  double trigThresh = 999;
//   unsigned int elec = 0;
  
  for ( unsigned int w = 0; w < wpVec.size(); ++w )
  {
    setWp( wpVec.at(w), wpEb, wpEe);

    // taking up to 3 reco electrons -- L1 trigger gives us 4 candidates
    for ( unsigned int elec = 0; elec < recoElectronCollection.size() && elec < 3; ++elec )
    {      
      for ( unsigned int j = 0; j < cutVec.size(); ++j )
      {      
        string where( fabs( recoElectronCollection.at(elec).Eta() ) < 1.4442 ? "EB" : "EE");
        WP     wp   ( fabs( recoElectronCollection.at(elec).Eta() ) < 1.4442 ? wpEb : wpEe);

        if ( !applyRecoEleCut( cutVec.at(j), wp, elec ) ) break;
        
        for ( unsigned int i = 0; i < trigVec.size(); ++i )
        {
          if ( trigVec.at(i).find("5") != std::string::npos )
            trigThresh = 5;
          else if ( trigVec.at(i).find("8") != std::string::npos )
            trigThresh = 8;
            
          if ( triggers[trigVec.at(i)] )
          {
            if ( trigVec.at(i) == "NoTrigger" )
            {
              fillRecoElectronHistoCollection((wpVec.at(w))+"_"+(trigVec.at(i))+"_"+(cutVec.at(j))+"_"+where, elec);
              fillRecoElectronHistoCollection((wpVec.at(w))+"_"+(trigVec.at(i))+"_"+(cutVec.at(j))+"_ALL", elec);
            }
            else if ( recoElectronCollection.at(elec).ClosestL1Et() >= trigThresh )
            {
              fillRecoElectronHistoCollection((wpVec.at(w))+"_"+(trigVec.at(i))+"_"+(cutVec.at(j))+"_"+where, elec);
              fillRecoElectronHistoCollection((wpVec.at(w))+"_"+(trigVec.at(i))+"_"+(cutVec.at(j))+"_ALL", elec);
            }
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
//           cout << "        TriggerMatch  : " << recoElectronCollection.at(i).ClosestL1dr() << "\t---" << applyRecoEleCut("TriggerMatch", wp, i ) << endl;
//       }
//       }
  }
}

void
trigEff::fillRecoElectronHltHistograms(double hltmatch)
{
  WP wpEb, wpEe;
   
  std::vector<std::string> trigVec = stringParser(triggersOfInterestHlt);
  hltElecMatch.clear();
  matchElectronsToHlt( hltmatch );
  
  double trigThresh = 999;

  for ( unsigned int w = 0; w < wpVec.size(); ++w )
  {
    setWp( wpVec.at(w), wpEb, wpEe);

//     usedHltCands.clear();
    // taking up to 3 reco electrons -- L1 trigger gives us 4 candidates
    for ( unsigned int elec = 0; elec < recoElectronCollection.size() && elec < 3; ++elec )
    {
      for ( unsigned int trg = 0; trg < trigVec.size(); ++trg )
      {    
        if ( hltElecMatch[trigVec.at(trg)].at(elec) == 999 ) continue;
//         if ( !(foundElectronMatchToHlt( trigVec.at(i), hltmatch, elec )) ) continue;

//         if ( trigVec.at(trg) == "HLT_Photon15_L1R" && ientry == 82 )
//         {
//             cout << ientry 
//                  << ":\tPt: " << recoElectronCollection.at(elec).Pt()
//                  << "\tEt: " << recoElectronCollection.at(elec).Et()
//                  << "\tSCEt: " << recoElectronCollection.at(elec).ScEt()
// //                  << "\tEta: " << recoElectronCollection.at(elec).Eta()
// //                  << "\tPhi: " << recoElectronCollection.at(elec).Phi()
//                  << "\tHLT pt: " << hltPhoton15Pt->at(hltElecMatch[trigVec.at(trg)].at(elec))
// //                  << "\tHLT eta: " << hltPhoton15Eta->at(hltElecMatch[trigVec.at(trg)].at(elec)) 
// //                  << "\tHLT phi: " << hltPhoton15Phi->at(hltElecMatch[trigVec.at(trg)].at(elec)) 
//                  << "\tdr : " << deltaR( hltPhoton15Eta->at(hltElecMatch[trigVec.at(trg)].at(elec)), hltPhoton15Phi->at(hltElecMatch[trigVec.at(trg)].at(elec)), recoElectronCollection.at(elec).Eta(), recoElectronCollection.at(elec).Phi() )
// //                  << "\tdphi : " << deltaPhi( hltPhoton15Phi->at(hltElecMatch[trigVec.at(trg)].at(elec)), recoElectronCollection.at(elec).Phi() )
// //                  << "\tdeta : " << ( hltPhoton15Eta->at(hltElecMatch[trigVec.at(trg)].at(elec)) - recoElectronCollection.at(elec).Eta() )
//                  << endl;
//         }
      
        for ( unsigned int j = 0; j < cutVec.size(); ++j )
        {   
          string where( fabs( recoElectronCollection.at(elec).Eta() ) < 1.4442 ? "EB" : "EE");
          WP     wp   ( fabs( recoElectronCollection.at(elec).Eta() ) < 1.4442 ? wpEb : wpEe);

          if ( !applyRecoEleCut( cutVec.at(j), wp, elec ) ) break;
        
          if ( trigVec.at(trg) == "HLT_Photon10_L1R" || trigVec.at(trg) == "HLT_Ele10_LW_L1R" )
            trigThresh = 5;
          else if ( trigVec.at(trg) == "HLT_Photon15NewThresh_L1R" )
            trigThresh = 5;
          else if ( trigVec.at(trg) == "HLT_Photon15_L1R" || trigVec.at(trg) == "HLT_Ele15_LW_L1R" )
            trigThresh = 8;

          if ( triggers[trigVec.at(trg)] && recoElectronCollection.at(elec).ClosestL1Et() >= trigThresh )
          {
            fillRecoElectronHistoCollection((wpVec.at(w))+"_"+(trigVec.at(trg))+"_"+(cutVec.at(j))+"_"+where, elec);
            fillRecoElectronHistoCollection((wpVec.at(w))+"_"+(trigVec.at(trg))+"_"+(cutVec.at(j))+"_ALL", elec);
          }
        }
      }
    }
  }
}

void
trigEff::matchElectronsToHlt( double dRmatch )
{
  vector<string> trigVec = stringParser(triggersOfInterestHlt);
  
  vector<double> *hltEta = hltEle10LWEta;
  vector<double> *hltPhi = hltEle10LWPhi;
  vector<double> *hltPt = hltEle10LWPt;
  
  map<unsigned int, unsigned int> gotMatch;
  
  for ( unsigned int trg = 0; trg < trigVec.size(); ++trg )
  {
    gotMatch.clear();
    
    if ( trigVec.at(trg) == "HLT_Ele10_LW_L1R" )
    {
      hltEta = hltEle10LWEta;
      hltPhi = hltEle10LWPhi;
      hltPt  = hltEle10LWPt;
    }
    else if ( trigVec.at(trg) == "HLT_Ele15_LW_L1R" )
    {
      hltEta = hltEle15LWEta;
      hltPhi = hltEle15LWPhi;
      hltPt  = hltEle15LWPt;
    }
    else if ( trigVec.at(trg) == "HLT_Photon10_L1R" || trigVec.at(trg) == "HLT_Photon15NewThresh_L1R" )
    {
      hltEta = hltPhoton10Eta;
      hltPhi = hltPhoton10Phi;
      hltPt  = hltPhoton10Pt;
    }
    else if ( trigVec.at(trg) == "HLT_Photon15_L1R" )
    {
      hltEta = hltPhoton15Eta;
      hltPhi = hltPhoton15Phi;
      hltPt  = hltPhoton15Pt;
    }
  
    double dr = 0;
    for ( unsigned int elec = 0; elec < recoElectronCollection.size(); ++elec )
    {
      double closestDr = 50;
      unsigned int closestHlt = 999;
          
      for ( unsigned int i = 0; i < hltEta->size(); ++i )
      {
        if ( gotMatch[i] == 1 ) continue;
        
        dr = deltaR( hltEta->at(i), hltPhi->at(i), recoElectronCollection.at(elec).Eta(), recoElectronCollection.at(elec).Phi() );
        
        if ( trigVec.at(trg) == "HLT_Photon15NewThresh_L1R" 
            && hltPt->at(i) >= 15 
            && dr < dRmatch && dr < closestDr )
        {
          closestDr = dr; 
          closestHlt = i;
        }
        
        if ( trigVec.at(trg) != "HLT_Photon15NewThresh_L1R" 
            && dr < dRmatch && dr < closestDr )
        {
          closestDr = dr; 
          closestHlt = i;
        }
      }
      
      if ( closestDr < dRmatch )
        gotMatch[closestHlt] = 1;
      
      hltElecMatch[trigVec.at(trg)].push_back(closestHlt);
    }
  }
}

bool
trigEff::foundElectronMatchToHlt( std::string trigger, double dRmatch, unsigned int which)
{
  vector<double> *hltEta = hltEle10LWEta;
  vector<double> *hltPhi = hltEle10LWPhi;
  vector<double> *hltPt = hltEle10LWPt;
  
  if ( trigger == "HLT_Ele10_LW_L1R" )
  {
    hltEta = hltEle10LWEta;
    hltPhi = hltEle10LWPhi;
    hltPt  = hltEle10LWPt;
  }
  else if ( trigger == "HLT_Ele15_LW_L1R" )
  {
    hltEta = hltEle15LWEta;
    hltPhi = hltEle15LWPhi;
    hltPt  = hltEle15LWPt;
  }
  else if ( trigger == "HLT_Photon10_L1R" || trigger == "HLT_Photon15NewThresh_L1R" )
  {
    hltEta = hltPhoton10Eta;
    hltPhi = hltPhoton10Phi;
    hltPt  = hltPhoton10Pt;
  }
  else if ( trigger == "HLT_Photon15_L1R" )
  {
    hltEta = hltPhoton15Eta;
    hltPhi = hltPhoton15Phi;
    hltPt  = hltPhoton15Pt;
  }
  
  for ( unsigned int i = 0; i < hltEta->size(); ++i )
  {
    if ( usedHltCands[trigger].find(50+i) != usedHltCands[trigger].end() ) continue;
    
    if ( trigger == "HLT_Photon15NewThresh_L1R" 
         && hltPt->at(i) >= 15 
         && deltaR( hltEta->at(i), hltPhi->at(i), recoElectronCollection.at(which).Eta(), recoElectronCollection.at(which).Phi() ) < dRmatch )
    {
      usedHltCands[trigger].insert(50+i);
      return true;
    }
    
    if ( trigger != "HLT_Photon15NewThresh_L1R" 
         && deltaR( hltEta->at(i), hltPhi->at(i), recoElectronCollection.at(which).Eta(), recoElectronCollection.at(which).Phi() ) < dRmatch )
    {
      usedHltCands[trigger].insert(50+i);
      return true;
    }
  }
  
  return false;
}

void
trigEff::fillRecoElectronHistoCollection(string whichHisto, unsigned int ele)
{
  hists[whichHisto+"_ScEt"]->Fill( recoElectronCollection.at(ele).ScEt() );
//   hists[whichHisto+"_Et"]->Fill( recoElectronCollection.at(ele).Et() );
//   hists[whichHisto+"_Pt"]->Fill( recoElectronCollection.at(ele).Pt() );
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
trigEff::applyRecoEleCut(std::string cut, const WP & wp, unsigned int which)
{ 
  if      ( cut == "NoCuts"  )       return true;
  else if ( cut == "SpikesRemoved" ) return ElectroneleMaxOver3x3->at(recoElectronCollection.at(which).Pos()) < 0.9 ; 
  else if ( cut == "SCEt"    )       return recoElectronCollection.at(which).ScEt() > 0 ;
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
  else if ( cut == "TrkDrv" )        return !( ElectroneleTrkDriven->at(recoElectronCollection.at(which).Pos() ) );
  else if ( cut == "EcalDrv" )       return !( ElectroneleEcalDriven->at(recoElectronCollection.at(which).Pos() ) );
  else if ( cut == "AllCuts" )       return true ;
  else throw std::runtime_error("Unknown cut string \'" + cut + "\'");
}



bool 
trigEff::selectRecoElectrons(double deltaRCone)
{
  recoElectronCollection.clear();
  std::set<unsigned int> usedL1;
  
  for ( unsigned int i = 0; i < ElectronEt->size(); ++i )
  {
    if ( !( electronInEta( ElectronEta->at(i) ) ) ) continue;
    
    electron ele(this, i, deltaRCone, usedL1);

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
    
    usedL1.insert( ( ele.ClosestL1IsIso() ? ele.ClosestL1() : 1000 + ele.ClosestL1() ) );
  }
  
  return !(recoElectronCollection.empty());
}



void
trigEff::BookHistograms()
{
  string y = "events";
  hBins bin_Et         = { y, "E_{T}"              ,  10,  0.  ,  60.   };
  double varBinsEt[11] = { 0, 2, 4, 6, 8, 10, 12, 16, 20, 25, 60 }; 
//   hBins bin_Pt         = { y, "P_{T}"              ,  10,  0.  ,  60.   };
//   hBins bin_deltaEt    = { y, "#Delta E_{T}"       , 100, -20.  ,   20.   };
//   hBins bin_deltaEtrel = { y, "#Delta E_{T, rel}"  , 100, -2.  ,   2.   };
  hBins bin_Eta        = { y, "#eta"               ,  20, -4.  ,   4.   };
  hBins bin_Phi        = { y, "#phi"               ,  20, -4.  ,   4.   };
//   hBins bin_idHoE      = { y, "H/E"                , 100,  0.  ,   0.05 };
//   hBins bin_idDphi     = { y, "#Delta_{#phiin}"    , 100, -0.1 ,   0.1  };
//   hBins bin_idDeta     = { y, "#Delta_{#etain}"    , 140, -0.12,   0.12 };
//   hBins bin_idDsIeIe   = { y, "#sigma_{i#etai#eta}", 140,  0.  ,   0.07 };
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
        for ( unsigned int k = 0; k < 3; ++k)
        {
          base = (wpVec.at(w))+"_"+(trigVec.at(i))+"_"+(cutVec.at(j))+"_"+where[k];

          hists[base+"_ScEt"]    = myTH1D(base+"_ScEt",bin_Et, varBinsEt);
//           hists[base+"_Et"]      = myTH1D(base+"_Et",bin_Et);
//           hists[base+"_Pt"]      = myTH1D(base+"_Pt",bin_Pt);
          hists[base+"_Eta"]     = myTH1D(base+"_Eta",bin_Eta);
          hists[base+"_Phi"]     = myTH1D(base+"_Phi",bin_Phi);
//           hists[base+"_HoE"]     = myTH1D(base+"_HoE",bin_idHoE);
//           hists[base+"_dEtaIn"]  = myTH1D(base+"_dEtaIn",bin_idDphi);
//           hists[base+"_dPhiIn"]  = myTH1D(base+"_dPhiIn",bin_idDeta);
//           hists[base+"_SigIeIe"] = myTH1D(base+"_SigIeIe",bin_idDsIeIe);
// 
//           hists[base+"_L1Et"]          = myTH1D(base+"_L1Et",bin_Et);
//           hists[base+"_L1DeltaEt"]     = myTH1D(base+"_L1DeltaEt",bin_deltaEt);
//           hists[base+"_L1DeltaEtRel"]  = myTH1D(base+"_L1DeltaEtRel",bin_deltaEtrel);
// 
//           hists[base+"_MaxOver3x3"]    = myTH1D(base+"_MaxOver3x3",bin_MaxOver3x3);
        }
      }
    }
  }
  
  cout << "Booked histograms\n";
}

void 
trigEff::setWp(std::string givenWp, WP & wpEb, WP & wpEe)
{
  WP jess(0, 0, 0, 0, 0, 0, 0, 0, 0.05, 0, 0.1 );
  // missingHits dist deltaCotTheta combinedIso relTrackIso relEcalIso relHcalIso
  // sigIeIe 
  WP wp95EB( 1, 0.00, 0.00, 0.15, 0.15, 2.00, 0.12, 0.01, 0.8, 0.007, 0.15 );
  WP wp95EE( 1, 0.00, 0.00, 0.10, 0.08, 0.06, 0.05, 0.03, 0.7, 0.010, 0.07 );
  
  // cross-check numbers!
//   WP wp90EB( 1, 0.02, 0.02, 0.10, 0.12, 0.09, 0.10, 0.01, 0.8, 0.007, 0.12 );
//   WP wp90EE( 1, 0.02, 0.02, 0.07, 0.05, 0.06, 0.03, 0.03, 0.7, 0.009, 0.05 );
   
  WP wp80EB( 0, 0.02, 0.02, 0.07, 0.09, 0.07, 0.10 , 0.01, 0.06, 0.004, 0.040 );
  WP wp80EE( 0, 0.02, 0.02, 0.06, 0.04, 0.05, 0.025, 0.03, 0.03, 0.007, 0.025 );

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
trigEff::stringParser(std::string cutString)
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

