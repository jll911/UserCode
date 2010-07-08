#define EwkDqmTreeHang_cxx
#include "EwkDqmTreeHang.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void EwkDqmTreeHang::Loop()
{
  std::cout << "Entering loop" << std::endl;
//   In a ROOT session, you can do:
//      Root > .L EwkDqmTreeHang.C
//      Root > EwkDqmTreeHang t
//      Root > t.GetEntry(12); // Fill t data members with entry number 12
//      Root > t.Show();       // Show values of entry 12
//      Root > t.Show(16);     // Read and show values of entry 16
//      Root > t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
   if (fChain == 0) return;

   //   Long64_t nentries = fChain->GetEntriesFast();
   Long64_t nentries = 1000000;
   //   Long64_t nentries = 100000;
   //   Long64_t nentries = 10000;
   //   Long64_t nentries = 1000;
   //   Long64_t nentries = 10;
   //   Long64_t nentries = 2;

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) 
     //for (Long64_t jentry=40000; jentry<nentries;jentry++) 
     {
       // HACKS to study hanging!
       if (jentry != 6997 && jentry != 23990 && jentry != 32514 && 
	   jentry != 47919 && jentry != 48763 && jentry != 70033 && 
	   jentry != 72581 && jentry != 95758 && jentry != 131493 && 
	   jentry != 132921 && jentry != 203308 && jentry != 203380) continue;

       //if (jentry % 10000 == 0)
       if (jentry % 1 == 0)
	 {
	   std::cout << "\rNow on entry " << jentry << " of " << nentries
		     << std::flush;
	 }

       Long64_t ientry = LoadTree(jentry);
       std::cout << "Loaded entry" << std::endl;
       if (ientry < 0) break;
       nb = fChain->GetEntry(jentry);   nbytes += nb;
       // if (Cut(ientry) < 0) continue;

       highestScEtElectronScEt = -999.;
       highestScEtElectronEta = -999.;
       highestScEtElectronPhi = -999.;
       highestScEtElectronIndex = -999.;

       highestScEtElectronPt = -999.;

       highestScEtElectronIsBarrel = false;
       highestScEtElectronIsEndcap = false;

       highestScEtElectronIsEcalDriven = false;
       highestScEtElectronIsTrackerDriven = false;

       highestScEtElectronNHitsMiss = -9999.;
       highestScEtElectronDist = 999.;
       highestScEtElectronDcotTheta = 999.;

       highestScEtElectronIsoVar = -999.;
       highestScEtElectronEcalIso = -999.;
       highestScEtElectronHcalIso = -999.;
       highestScEtElectronTrackIso = -999.;

       highestScEtElectronSieie = -999.;
       highestScEtElectronDetain = -999.;
       highestScEtElectronDphiin = -999.;
       highestScEtElectronHoE = -999.;

       for (Int_t i = 0; i < ele_nelectrons; i++)
	 {
	   // spike rejection already done in ntuple code
	   
	   if (ele_ecalenergy[i] > highestScEtElectronScEt)
	     {
	       highestScEtElectronScEt = ele_ecalenergy[i];
	       highestScEtElectronEta = ele_eta[i];
	       highestScEtElectronPhi = ele_phi[i];
	       highestScEtElectronIndex = i;

	       highestScEtElectronPt = ele_pt[i];

	       highestScEtElectronIsBarrel = ele_isBarrel[i];
	       highestScEtElectronIsEndcap = ele_isEndcap[i];
	       highestScEtElectronIsEcalDriven = ele_isEcalDriven[i];
	       highestScEtElectronIsTrackerDriven = ele_isTrackerDriven[i];

	       highestScEtElectronNHitsMiss = ele_nexpectedinnerhits[i];
	       highestScEtElectronDist = ele_dist[i];
	       highestScEtElectronDcotTheta = ele_dcottheta[i];
	       
	       // highestScEtElectronIsoVar = -999.;
	       highestScEtElectronEcalIso = ele_dr03ecalrechitsumet[i];
	       highestScEtElectronHcalIso = ele_dr03hcaltowersumet[i];
	       highestScEtElectronTrackIso = ele_dr03tksumpt[i];
	       
	       highestScEtElectronSieie = ele_sigmaietaieta[i];
	       highestScEtElectronDetain = ele_deltaetasuperclustertrackatvtx[i];
	       highestScEtElectronDphiin = ele_deltaphisuperclustertrackatvtx[i];
	       highestScEtElectronHoE = ele_hcaloverecal[i];

	     }
	 } // end loop over electrons

       if (highestScEtElectronScEt > 0)
	 {
	   std::cout << "Entry " << jentry << ": Highest electron!  sc et: " << highestScEtElectronScEt << std::endl;

	   // CALCULATE COMBINED ISO VAR
	   // Combined Isolation for Barrel = ( electron->dr03TkSumPt() + max(0., electron->dr03EcalRecHitSumEt() - 1.) + electron->dr03HcalTowerSumEt() ) / electron->p4().Pt() 
	   // Combined Isolation for Endcaps = ( electron->dr03TkSumPt() + electron->dr03EcalRecHitSumEt() + electron->dr03HcalTowerSumEt() ) / electron->p4().Pt() 

	   Float_t combIsoVar = 999.;
	   if (highestScEtElectronIsBarrel)
	     {
	       combIsoVar = (highestScEtElectronTrackIso + max(0., (double) highestScEtElectronEcalIso - 1) + highestScEtElectronHcalIso) / highestScEtElectronPt;
	     }
	   else if (highestScEtElectronIsEndcap)
	     {
	       combIsoVar = (highestScEtElectronTrackIso + highestScEtElectronEcalIso + highestScEtElectronHcalIso) / highestScEtElectronPt;
	     }

	   //result_WP = false;
	   result_WP = true;
	   if (highestScEtElectronNHitsMiss > nHitsMiss[wpIndex]) 
	     {
	       result_WP = false;
	       std::cout << "nHitsMiss = " << highestScEtElectronNHitsMiss << " which is greater than " << nHitsMiss[wpIndex] << std::endl;
	     }
	   if (highestScEtElectronDist < dist[wpIndex] && highestScEtElectronDist > (-1.)*dist[wpIndex])
	     {
	       result_WP = false;
	       std::cout << "dist = " << highestScEtElectronDist << " absval of which is less than " << dist[wpIndex] << std::endl;
	     }
	   if (highestScEtElectronDcotTheta < dcotTheta[wpIndex] && highestScEtElectronDcotTheta > (-1.)* dcotTheta[wpIndex]) 
	     {
	       result_WP = false;
	       std::cout << "dcottheta = " << highestScEtElectronDcotTheta << " absval of which is less than " << dcotTheta[wpIndex] << std::endl;
	     }
       
	   if (!result_WP)
	     std::cout << "Failed conversion rejection" << std::endl;

	   if (highestScEtElectronIsBarrel)
	     {
	       std::cout << "Endcap" << std::endl;
	       if (highestScEtElectronIsoVar > isolVarBarrel[wpIndex]) result_WP = false;
	   
	       if (highestScEtElectronSieie > sieieBarrel[wpIndex]) result_WP = false;
	       if (highestScEtElectronDetain > detainBarrel[wpIndex]) result_WP = false;
	       if (highestScEtElectronDphiin > dphiinBarrel[wpIndex]) result_WP = false;
	       if (highestScEtElectronHoE > hoeBarrel[wpIndex]) result_WP = false;
	     }
	   else if (highestScEtElectronIsEndcap)
	     {
	       std::cout << "Endcap" << std::endl;
	       if (highestScEtElectronIsoVar > isolVarEndcap[wpIndex]) result_WP = false;
	   
	       if (highestScEtElectronSieie > sieieEndcap[wpIndex]) result_WP = false;
	       if (highestScEtElectronDetain > detainEndcap[wpIndex]) result_WP = false;
	       if (highestScEtElectronDphiin > dphiinEndcap[wpIndex]) result_WP = false;
	       if (highestScEtElectronHoE > hoeEndcap[wpIndex]) result_WP = false;
	     }
	   else
	     {
	       result_WP = false;
	       std::cout << "Not barrel or endcap" << std::endl;
	     }

	   if (result_WP)
	     {
	       std::cout << " passed WP" << std::endl;
	     }
	   
	 }

       // Get trigger results
       result_HLTElec10_ = false; // HLT_Ele10_LW_L1R
       result_HLTElec15_ = false; // HLT_Ele15_LW_L1R
       result_HLTPhot10_ = false; // HLT_Photon10_L1R
       result_HLTPhot15_ = false; // HLT_Photon15_L1R
       result_L1EG5_ = false; // L1_SingleEG5
       result_L1EG8_ = false; // L1_SingleEG8

       for (Int_t iTrg = 0; iTrg < kMax1; iTrg++)
	 {
	   //       std::cout << /*(string)*/ triggersOfInterest_first[iTrg].c_str() << std::endl;
	   //        std::cout << /*(string)*/ (*triggersOfInterest_first) << std::endl;
	   //        if (triggersOfInterest_second[iTrg]) std::cout << "  -- true" << std::endl;
	   //        else std::cout << "  -- false" << std::endl;

	   if ( triggersOfInterest_first[iTrg].find("HLT_Ele10_LW_L1R") != std::string::npos && triggersOfInterest_second[iTrg] == true)
	     {
	       result_HLTElec10_ = true;
	       std::cout << "Trigger passed!" << std::endl;
	     }
	   if ( triggersOfInterest_first[iTrg].find("HLT_Ele15_LW_L1R") != std::string::npos && triggersOfInterest_second[iTrg] == true)
	     {
	       result_HLTElec15_ = true;
	       std::cout << "Trigger passed!" << std::endl;
	     }
	   if ( triggersOfInterest_first[iTrg].find("HLT_Photon10_L1R") != std::string::npos && triggersOfInterest_second[iTrg] == true)
	     {
	       result_HLTPhot10_ = true;
	       std::cout << "Trigger passed!" << std::endl;
	     }
	   if ( triggersOfInterest_first[iTrg].find("HLT_Photon15_L1R") != std::string::npos && triggersOfInterest_second[iTrg] == true)
	     {
	       result_HLTPhot15_ = true;
	       std::cout << "Trigger passed!" << std::endl;
	     }
	 }

       for (Int_t iTrg = 0; iTrg < kMax2; iTrg++)
	 {
	   if ( l1TriggersOfInterest_first[iTrg].find("L1_SingleEG5") != std::string::npos && l1TriggersOfInterest_second[iTrg] == true)
	     {
	       result_L1EG5_ = true;
	       std::cout << "Trigger passed!" << std::endl;
	     }
	   if ( l1TriggersOfInterest_first[iTrg].find("L1_SingleEG8") != std::string::npos && l1TriggersOfInterest_second[iTrg] == true)
	     {
	       result_L1EG8_ = true;
	       std::cout << "Trigger passed!" << std::endl;
	     }
	 }


       // trigger object matching
       // check dR of highest reco object compared to trigger objects
       // if matched, set match true!
       // if matched trigger object over threshold (or in any of HLT cases)
       // set corresponding threshold result true
       result_match_L1obj_ = false;
       //   result_match_HLTobj_ = false;

       result_matched_HLTElec10_ = false;
       result_matched_HLTElec15_ = false;
       result_matched_HLTPhot10_ = false;
       result_matched_HLTPhot15_ = false;
       result_matched_L1EG5_ = false;
       result_matched_L1EG8_ = false;


       Float_t deltaR = 999.;

       // l1
       Int_t iL1Matched = 999;
       for (Int_t l1count = 0; l1count < l1_nL1Obj; l1count++)
	 {
	   Float_t deltaEta = highestScEtElectronEta - l1_eta[l1count];
	   Float_t deltaPhi = highestScEtElectronPhi - l1_phi[l1count];
	   while (deltaPhi > 3.1415926535)
	     {
	       deltaPhi = deltaPhi - 2*3.1415926535;
	     }
	   while (deltaPhi < -3.1415926535)
	     {
	       deltaPhi = deltaPhi + 2*3.1415926535;
	     }
	   Float_t temp = sqrt(deltaEta*deltaEta + deltaPhi*deltaPhi);
	   if (temp < deltaR)
	     {
	       deltaR = temp;
	       iL1Matched = l1count;
	     }
	 }
       std::cout << "Final deltaR is " << deltaR << std::endl;
       if (deltaR < 0.5)
	 {
	   result_match_L1obj_ = true;
	   std::cout << "L1 match in event " << jentry << std::endl;
	   if (l1_et[iL1Matched] > 5.)
	     {
	       result_matched_L1EG5_ = true;
	       std::cout << "  over 5 GeV" << std::endl;
	       if (l1_et[iL1Matched] > 8.)
		 {
		   result_matched_L1EG8_ = true;
		   std::cout << "  over 8 GeV" << std::endl;
		 }
	     }
	 }


       // hlt
       // Elec10
       deltaR = 999.;
       //       Int_t iHltElec10Matched = 999;
       for (Int_t hltcount = 0; hltcount < hlt_nHltObj_Elec10; hltcount++)
	 {
	   Float_t deltaEta = highestScEtElectronEta - hlt_Elec10_eta[hltcount];
	   Float_t deltaPhi = highestScEtElectronPhi - hlt_Elec10_phi[hltcount];
	   while (deltaPhi > 3.1415926535)
	     {
	       deltaPhi = deltaPhi - 2*3.1415926535;
	     }
	   while (deltaPhi < -3.1415926535)
	     {
	       deltaPhi = deltaPhi + 2*3.1415926535;
	     }
	   Float_t temp = sqrt(deltaEta*deltaEta + deltaPhi*deltaPhi);
	   if (temp < deltaR)
	     {
	       deltaR = temp;
	       //	       iHltElec10Matched = hltcount;
	     }
	 }
       if (deltaR < 0.5)
	 {
	   result_matched_HLTElec10_ = true;
	   std::cout << "HLT match elec 10" << std::endl;
	 }

       // Elec15
       deltaR = 999.;
       //       Int_t iHltElec15Matched = 999;
       for (Int_t hltcount = 0; hltcount < hlt_nHltObj_Elec15; hltcount++)
	 {
	   Float_t deltaEta = highestScEtElectronEta - hlt_Elec15_eta[hltcount];
	   Float_t deltaPhi = highestScEtElectronPhi - hlt_Elec15_phi[hltcount];
	   while (deltaPhi > 3.1415926535)
	     {
	       deltaPhi = deltaPhi - 2*3.1415926535;
	     }
	   while (deltaPhi < -3.1415926535)
	     {
	       deltaPhi = deltaPhi + 2*3.1415926535;
	     }
	   Float_t temp = sqrt(deltaEta*deltaEta + deltaPhi*deltaPhi);
	   if (temp < deltaR)
	     {
	       deltaR = temp;
	       //	       iHltElec15Matched = hltcount;
	     }
	 }
       if (deltaR < 0.5)
	 {
	   result_matched_HLTElec15_ = true;
	   std::cout << "HLT match elec 15" << std::endl;
	 }

       // Phot10
       deltaR = 999.;
       //       Int_t iHltPhot10Matched = 999;
       for (Int_t hltcount = 0; hltcount < hlt_nHltObj_Phot10; hltcount++)
	 {
	   Float_t deltaEta = highestScEtElectronEta - hlt_Phot10_eta[hltcount];
	   Float_t deltaPhi = highestScEtElectronPhi - hlt_Phot10_phi[hltcount];
	   while (deltaPhi > 3.1415926535)
	     {
	       deltaPhi = deltaPhi - 2*3.1415926535;
	     }
	   while (deltaPhi < -3.1415926535)
	     {
	       deltaPhi = deltaPhi + 2*3.1415926535;
	     }
	   Float_t temp = sqrt(deltaEta*deltaEta + deltaPhi*deltaPhi);
	   if (temp < deltaR)
	     {
	       deltaR = temp;
	       //	       iHltPhot10Matched = hltcount;
	     }
	 }
       if (deltaR < 0.5)
	 {
	   result_matched_HLTPhot10_ = true;
	   std::cout << "HLT match phot 10" << std::endl;
	 }

       // Phot15
       deltaR = 999.;
       //       Int_t iHltPhot15Matched = 999;
       if (hlt_nHltObj_Phot15 > 0)
	 {
	   std::cout << "N Phot 15 objects: " << hlt_nHltObj_Phot15 << std::endl;
	 }
       for (Int_t hltcount = 0; hltcount < hlt_nHltObj_Phot15; hltcount++)
	 {
	   Float_t deltaEta = highestScEtElectronEta - hlt_Phot15_eta[hltcount];
	   Float_t deltaPhi = highestScEtElectronPhi - hlt_Phot15_phi[hltcount];
	   while (deltaPhi > 3.1415926535)
	     {
	       deltaPhi = deltaPhi - 2*3.1415926535;
	     }
	   while (deltaPhi < -3.1415926535)
	     {
	       deltaPhi = deltaPhi + 2*3.1415926535;
	     }
	   Float_t temp = sqrt(deltaEta*deltaEta + deltaPhi*deltaPhi);
	   if (temp < deltaR)
	     {
	       deltaR = temp;
	       //	       iHltPhot15Matched = hltcount;
	       std::cout << "Newly assigned min deltaR of " << deltaR 
			 << " from dEta " << deltaEta 
			 << " and dPhi " << deltaPhi << std::endl;
	     }
	 }
       if (deltaR < 0.5)
	 {
	   result_matched_HLTPhot15_ = true;
	   std::cout << "HLT match phot 15 with deltaR " << deltaR << std::endl;
	 }






       // FILL EFFICIENCIES!

       // unmatched
       if (highestScEtElectronScEt > 0.) // make sure there is one, i.e. it's not -999.
	 {
	   if (result_WP)
	     {
	       if (highestScEtElectronIsBarrel)
		 {
		   if (highestScEtElectronIsEcalDriven)
		     {
		       eff_denom_ETA_BARREL__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET_->Fill(highestScEtElectronScEt);
		       eff_denom_ETA_BARREL__SEED_ECAL__TRIG_UNMATCHED__VS_ETA_->Fill(highestScEtElectronEta);
		       eff_denom_ETA_BARREL__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET_->Fill(highestScEtElectronScEt);
		       eff_denom_ETA_BARREL__SEED_BOTH__TRIG_UNMATCHED__VS_ETA_->Fill(highestScEtElectronEta);
		       eff_denom_ETA_BOTH__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET_->Fill(highestScEtElectronScEt);
		       eff_denom_ETA_BOTH__SEED_ECAL__TRIG_UNMATCHED__VS_ETA_->Fill(highestScEtElectronEta);
		       eff_denom_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET_->Fill(highestScEtElectronScEt);
		       eff_denom_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ETA_->Fill(highestScEtElectronEta);
		       if (result_HLTElec10_)
			 {
			   eff_num_ETA_BARREL__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_ELEC10_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BARREL__SEED_ECAL__TRIG_UNMATCHED__VS_ETA__HLT_ELEC10_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BARREL__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_ELEC10_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BARREL__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__HLT_ELEC10_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BOTH__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_ELEC10_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BOTH__SEED_ECAL__TRIG_UNMATCHED__VS_ETA__HLT_ELEC10_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_ELEC10_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__HLT_ELEC10_->Fill(highestScEtElectronEta);
			 }
		       if (result_HLTElec15_)
			 {
			   eff_num_ETA_BARREL__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_ELEC15_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BARREL__SEED_ECAL__TRIG_UNMATCHED__VS_ETA__HLT_ELEC15_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BARREL__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_ELEC15_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BARREL__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__HLT_ELEC15_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BOTH__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_ELEC15_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BOTH__SEED_ECAL__TRIG_UNMATCHED__VS_ETA__HLT_ELEC15_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_ELEC15_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__HLT_ELEC15_->Fill(highestScEtElectronEta);
			 }
		       if (result_HLTPhot10_)
			 {
			   eff_num_ETA_BARREL__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_PHOT10_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BARREL__SEED_ECAL__TRIG_UNMATCHED__VS_ETA__HLT_PHOT10_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BARREL__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_PHOT10_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BARREL__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__HLT_PHOT10_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BOTH__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_PHOT10_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BOTH__SEED_ECAL__TRIG_UNMATCHED__VS_ETA__HLT_PHOT10_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_PHOT10_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__HLT_PHOT10_->Fill(highestScEtElectronEta);
			 }
		       if (result_HLTPhot15_)
			 {
			   eff_num_ETA_BARREL__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_PHOT15_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BARREL__SEED_ECAL__TRIG_UNMATCHED__VS_ETA__HLT_PHOT15_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BARREL__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_PHOT15_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BARREL__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__HLT_PHOT15_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BOTH__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_PHOT15_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BOTH__SEED_ECAL__TRIG_UNMATCHED__VS_ETA__HLT_PHOT15_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_PHOT15_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__HLT_PHOT15_->Fill(highestScEtElectronEta);
			 }
		       // L1
		       if (result_L1EG5_)
			 {
			   eff_num_ETA_BARREL__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET__L1_EG5_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BARREL__SEED_ECAL__TRIG_UNMATCHED__VS_ETA__L1_EG5_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BARREL__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__L1_EG5_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BARREL__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__L1_EG5_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BOTH__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET__L1_EG5_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BOTH__SEED_ECAL__TRIG_UNMATCHED__VS_ETA__L1_EG5_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__L1_EG5_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__L1_EG5_->Fill(highestScEtElectronEta);
			 }
		       if (result_L1EG8_)
			 {
			   eff_num_ETA_BARREL__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET__L1_EG8_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BARREL__SEED_ECAL__TRIG_UNMATCHED__VS_ETA__L1_EG8_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BARREL__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__L1_EG8_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BARREL__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__L1_EG8_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BOTH__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET__L1_EG8_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BOTH__SEED_ECAL__TRIG_UNMATCHED__VS_ETA__L1_EG8_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__L1_EG8_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__L1_EG8_->Fill(highestScEtElectronEta);
			 }
		   
		     }
		   if (highestScEtElectronIsTrackerDriven)
		     {
		       eff_denom_ETA_BARREL__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET_->Fill(highestScEtElectronScEt);
		       eff_denom_ETA_BARREL__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA_->Fill(highestScEtElectronEta);
		       eff_denom_ETA_BARREL__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET_->Fill(highestScEtElectronScEt);
		       eff_denom_ETA_BARREL__SEED_BOTH__TRIG_UNMATCHED__VS_ETA_->Fill(highestScEtElectronEta);
		       eff_denom_ETA_BOTH__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET_->Fill(highestScEtElectronScEt);
		       eff_denom_ETA_BOTH__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA_->Fill(highestScEtElectronEta);
		       eff_denom_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET_->Fill(highestScEtElectronScEt);
		       eff_denom_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ETA_->Fill(highestScEtElectronEta);
		       if (result_HLTElec10_)
			 {
			   eff_num_ETA_BARREL__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_ELEC10_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BARREL__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA__HLT_ELEC10_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BARREL__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_ELEC10_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BARREL__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__HLT_ELEC10_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BOTH__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_ELEC10_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BOTH__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA__HLT_ELEC10_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_ELEC10_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__HLT_ELEC10_->Fill(highestScEtElectronEta);
			 }
		       if (result_HLTElec15_)
			 {
			   eff_num_ETA_BARREL__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_ELEC15_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BARREL__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA__HLT_ELEC15_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BARREL__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_ELEC15_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BARREL__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__HLT_ELEC15_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BOTH__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_ELEC15_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BOTH__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA__HLT_ELEC15_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_ELEC15_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__HLT_ELEC15_->Fill(highestScEtElectronEta);
			 }
		       if (result_HLTPhot10_)
			 {
			   eff_num_ETA_BARREL__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_PHOT10_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BARREL__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA__HLT_PHOT10_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BARREL__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_PHOT10_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BARREL__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__HLT_PHOT10_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BOTH__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_PHOT10_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BOTH__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA__HLT_PHOT10_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_PHOT10_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__HLT_PHOT10_->Fill(highestScEtElectronEta);
			 }
		       if (result_HLTPhot15_)
			 {
			   eff_num_ETA_BARREL__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_PHOT15_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BARREL__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA__HLT_PHOT15_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BARREL__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_PHOT15_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BARREL__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__HLT_PHOT15_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BOTH__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_PHOT15_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BOTH__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA__HLT_PHOT15_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_PHOT15_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__HLT_PHOT15_->Fill(highestScEtElectronEta);
			 }
		       // L1
		       if (result_L1EG5_)
			 {
			   eff_num_ETA_BARREL__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET__L1_EG5_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BARREL__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA__L1_EG5_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BARREL__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__L1_EG5_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BARREL__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__L1_EG5_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BOTH__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET__L1_EG5_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BOTH__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA__L1_EG5_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__L1_EG5_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__L1_EG5_->Fill(highestScEtElectronEta);
			 }
		       if (result_L1EG8_)
			 {
			   eff_num_ETA_BARREL__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET__L1_EG8_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BARREL__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA__L1_EG8_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BARREL__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__L1_EG8_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BARREL__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__L1_EG8_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BOTH__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET__L1_EG8_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BOTH__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA__L1_EG8_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__L1_EG8_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__L1_EG8_->Fill(highestScEtElectronEta);
			 }

		     }
		 }
	       if (highestScEtElectronIsEndcap)
		 {
		   if (highestScEtElectronIsEcalDriven)
		     {
		       eff_denom_ETA_ENDCAP__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET_->Fill(highestScEtElectronScEt);
		       eff_denom_ETA_ENDCAP__SEED_ECAL__TRIG_UNMATCHED__VS_ETA_->Fill(highestScEtElectronEta);
		       eff_denom_ETA_ENDCAP__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET_->Fill(highestScEtElectronScEt);
		       eff_denom_ETA_ENDCAP__SEED_BOTH__TRIG_UNMATCHED__VS_ETA_->Fill(highestScEtElectronEta);
		       eff_denom_ETA_BOTH__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET_->Fill(highestScEtElectronScEt);
		       eff_denom_ETA_BOTH__SEED_ECAL__TRIG_UNMATCHED__VS_ETA_->Fill(highestScEtElectronEta);
		       eff_denom_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET_->Fill(highestScEtElectronScEt);
		       eff_denom_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ETA_->Fill(highestScEtElectronEta);
		       if (result_HLTElec10_)
			 {
			   eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_ELEC10_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_UNMATCHED__VS_ETA__HLT_ELEC10_->Fill(highestScEtElectronEta);
			   eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_ELEC10_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__HLT_ELEC10_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BOTH__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_ELEC10_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BOTH__SEED_ECAL__TRIG_UNMATCHED__VS_ETA__HLT_ELEC10_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_ELEC10_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__HLT_ELEC10_->Fill(highestScEtElectronEta);
			 }
		       if (result_HLTElec15_)
			 {
			   eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_ELEC15_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_UNMATCHED__VS_ETA__HLT_ELEC15_->Fill(highestScEtElectronEta);
			   eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_ELEC15_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__HLT_ELEC15_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BOTH__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_ELEC15_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BOTH__SEED_ECAL__TRIG_UNMATCHED__VS_ETA__HLT_ELEC15_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_ELEC15_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__HLT_ELEC15_->Fill(highestScEtElectronEta);
			 }
		       if (result_HLTPhot10_)
			 {
			   eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_PHOT10_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_UNMATCHED__VS_ETA__HLT_PHOT10_->Fill(highestScEtElectronEta);
			   eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_PHOT10_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__HLT_PHOT10_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BOTH__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_PHOT10_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BOTH__SEED_ECAL__TRIG_UNMATCHED__VS_ETA__HLT_PHOT10_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_PHOT10_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__HLT_PHOT10_->Fill(highestScEtElectronEta);
			 }
		       if (result_HLTPhot15_)
			 {
			   eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_PHOT15_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_UNMATCHED__VS_ETA__HLT_PHOT15_->Fill(highestScEtElectronEta);
			   eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_PHOT15_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__HLT_PHOT15_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BOTH__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_PHOT15_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BOTH__SEED_ECAL__TRIG_UNMATCHED__VS_ETA__HLT_PHOT15_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_PHOT15_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__HLT_PHOT15_->Fill(highestScEtElectronEta);
			 }
		       // L1
		       if (result_L1EG5_)
			 {
			   eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET__L1_EG5_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_UNMATCHED__VS_ETA__L1_EG5_->Fill(highestScEtElectronEta);
			   eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__L1_EG5_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__L1_EG5_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BOTH__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET__L1_EG5_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BOTH__SEED_ECAL__TRIG_UNMATCHED__VS_ETA__L1_EG5_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__L1_EG5_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__L1_EG5_->Fill(highestScEtElectronEta);
			 }
		       if (result_L1EG8_)
			 {
			   eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET__L1_EG8_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_UNMATCHED__VS_ETA__L1_EG8_->Fill(highestScEtElectronEta);
			   eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__L1_EG8_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__L1_EG8_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BOTH__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET__L1_EG8_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BOTH__SEED_ECAL__TRIG_UNMATCHED__VS_ETA__L1_EG8_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__L1_EG8_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__L1_EG8_->Fill(highestScEtElectronEta);
			 }

		     }
		   if (highestScEtElectronIsTrackerDriven)
		     {
		       eff_denom_ETA_ENDCAP__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET_->Fill(highestScEtElectronScEt);
		       eff_denom_ETA_ENDCAP__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA_->Fill(highestScEtElectronEta);
		       eff_denom_ETA_ENDCAP__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET_->Fill(highestScEtElectronScEt);
		       eff_denom_ETA_ENDCAP__SEED_BOTH__TRIG_UNMATCHED__VS_ETA_->Fill(highestScEtElectronEta);
		       eff_denom_ETA_BOTH__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET_->Fill(highestScEtElectronScEt);
		       eff_denom_ETA_BOTH__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA_->Fill(highestScEtElectronEta);
		       eff_denom_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET_->Fill(highestScEtElectronScEt);
		       eff_denom_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ETA_->Fill(highestScEtElectronEta);
		       if (result_HLTElec10_)
			 {
			   eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_ELEC10_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA__HLT_ELEC10_->Fill(highestScEtElectronEta);
			   eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_ELEC10_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__HLT_ELEC10_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BOTH__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_ELEC10_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BOTH__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA__HLT_ELEC10_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_ELEC10_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__HLT_ELEC10_->Fill(highestScEtElectronEta);
			 }
		       if (result_HLTElec15_)
			 {
			   eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_ELEC15_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA__HLT_ELEC15_->Fill(highestScEtElectronEta);
			   eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_ELEC15_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__HLT_ELEC15_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BOTH__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_ELEC15_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BOTH__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA__HLT_ELEC15_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_ELEC15_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__HLT_ELEC15_->Fill(highestScEtElectronEta);
			 }
		       if (result_HLTPhot10_)
			 {
			   eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_PHOT10_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA__HLT_PHOT10_->Fill(highestScEtElectronEta);
			   eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_PHOT10_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__HLT_PHOT10_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BOTH__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_PHOT10_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BOTH__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA__HLT_PHOT10_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_PHOT10_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__HLT_PHOT10_->Fill(highestScEtElectronEta);
			 }
		       if (result_HLTPhot15_)
			 {
			   eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_PHOT15_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA__HLT_PHOT15_->Fill(highestScEtElectronEta);
			   eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_PHOT15_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__HLT_PHOT15_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BOTH__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_PHOT15_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BOTH__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA__HLT_PHOT15_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_PHOT15_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__HLT_PHOT15_->Fill(highestScEtElectronEta);
			 }
		       // L1
		       if (result_L1EG5_)
			 {
			   eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET__L1_EG5_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA__L1_EG5_->Fill(highestScEtElectronEta);
			   eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__L1_EG5_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__L1_EG5_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BOTH__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET__L1_EG5_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BOTH__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA__L1_EG5_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__L1_EG5_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__L1_EG5_->Fill(highestScEtElectronEta);
			 }
		       if (result_L1EG8_)
			 {
			   eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET__L1_EG8_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA__L1_EG8_->Fill(highestScEtElectronEta);
			   eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__L1_EG8_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__L1_EG8_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BOTH__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET__L1_EG8_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BOTH__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA__L1_EG8_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__L1_EG8_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__L1_EG8_->Fill(highestScEtElectronEta);
			 }

		     }
		 }
	     }
	 }
       // matched
       if (highestScEtElectronScEt > 0.) // make sure there is one, i.e. it's not -999.
	 {
	   std::cout << "highest > 0" << std::endl;
	   if (result_WP)
	     {
	       std::cout << "pass WP" << std::endl;
	       if (highestScEtElectronIsBarrel)
		 {
		   std::cout << "is barrel" << std::endl;
		   if (highestScEtElectronIsEcalDriven)
		     {
		       std::cout << "is ecal driven" << std::endl;
		       eff_denom_ETA_BARREL__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET_->Fill(highestScEtElectronScEt);
		       eff_denom_ETA_BARREL__SEED_ECAL__TRIG_MATCHED__VS_ETA_->Fill(highestScEtElectronEta);
		       eff_denom_ETA_BARREL__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET_->Fill(highestScEtElectronScEt);
		       eff_denom_ETA_BARREL__SEED_BOTH__TRIG_MATCHED__VS_ETA_->Fill(highestScEtElectronEta);
		       eff_denom_ETA_BOTH__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET_->Fill(highestScEtElectronScEt);
		       eff_denom_ETA_BOTH__SEED_ECAL__TRIG_MATCHED__VS_ETA_->Fill(highestScEtElectronEta);
		       eff_denom_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET_->Fill(highestScEtElectronScEt);
		       eff_denom_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ETA_->Fill(highestScEtElectronEta);
		       if (result_matched_HLTElec10_)
			 {
			   eff_num_ETA_BARREL__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_ELEC10_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BARREL__SEED_ECAL__TRIG_MATCHED__VS_ETA__HLT_ELEC10_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BARREL__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_ELEC10_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BARREL__SEED_BOTH__TRIG_MATCHED__VS_ETA__HLT_ELEC10_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BOTH__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_ELEC10_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BOTH__SEED_ECAL__TRIG_MATCHED__VS_ETA__HLT_ELEC10_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_ELEC10_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ETA__HLT_ELEC10_->Fill(highestScEtElectronEta);
			 }
		       if (result_matched_HLTElec15_)
			 {
			   eff_num_ETA_BARREL__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_ELEC15_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BARREL__SEED_ECAL__TRIG_MATCHED__VS_ETA__HLT_ELEC15_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BARREL__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_ELEC15_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BARREL__SEED_BOTH__TRIG_MATCHED__VS_ETA__HLT_ELEC15_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BOTH__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_ELEC15_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BOTH__SEED_ECAL__TRIG_MATCHED__VS_ETA__HLT_ELEC15_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_ELEC15_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ETA__HLT_ELEC15_->Fill(highestScEtElectronEta);
			 }
		       if (result_matched_HLTPhot10_)
			 {
			   eff_num_ETA_BARREL__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_PHOT10_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BARREL__SEED_ECAL__TRIG_MATCHED__VS_ETA__HLT_PHOT10_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BARREL__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_PHOT10_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BARREL__SEED_BOTH__TRIG_MATCHED__VS_ETA__HLT_PHOT10_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BOTH__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_PHOT10_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BOTH__SEED_ECAL__TRIG_MATCHED__VS_ETA__HLT_PHOT10_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_PHOT10_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ETA__HLT_PHOT10_->Fill(highestScEtElectronEta);
			 }
		       if (result_matched_HLTPhot15_)
			 {
			   std::cout << "Filling nums for hlt phot 15 matched" << std::endl;
			   eff_num_ETA_BARREL__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_PHOT15_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BARREL__SEED_ECAL__TRIG_MATCHED__VS_ETA__HLT_PHOT15_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BARREL__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_PHOT15_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BARREL__SEED_BOTH__TRIG_MATCHED__VS_ETA__HLT_PHOT15_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BOTH__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_PHOT15_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BOTH__SEED_ECAL__TRIG_MATCHED__VS_ETA__HLT_PHOT15_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_PHOT15_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ETA__HLT_PHOT15_->Fill(highestScEtElectronEta);
			 }
		       // L1
		       if (result_matched_L1EG5_)
			 {
			   eff_num_ETA_BARREL__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET__L1_EG5_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BARREL__SEED_ECAL__TRIG_MATCHED__VS_ETA__L1_EG5_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BARREL__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__L1_EG5_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BARREL__SEED_BOTH__TRIG_MATCHED__VS_ETA__L1_EG5_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BOTH__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET__L1_EG5_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BOTH__SEED_ECAL__TRIG_MATCHED__VS_ETA__L1_EG5_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__L1_EG5_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ETA__L1_EG5_->Fill(highestScEtElectronEta);
			 }
		       if (result_matched_L1EG8_)
			 {
			   eff_num_ETA_BARREL__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET__L1_EG8_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BARREL__SEED_ECAL__TRIG_MATCHED__VS_ETA__L1_EG8_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BARREL__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__L1_EG8_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BARREL__SEED_BOTH__TRIG_MATCHED__VS_ETA__L1_EG8_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BOTH__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET__L1_EG8_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BOTH__SEED_ECAL__TRIG_MATCHED__VS_ETA__L1_EG8_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__L1_EG8_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ETA__L1_EG8_->Fill(highestScEtElectronEta);
			 }
		   
		     }
		   if (highestScEtElectronIsTrackerDriven)
		     {
		       std::cout << "is tracker driven" << std::endl;
		       eff_denom_ETA_BARREL__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET_->Fill(highestScEtElectronScEt);
		       eff_denom_ETA_BARREL__SEED_TRACKER__TRIG_MATCHED__VS_ETA_->Fill(highestScEtElectronEta);
		       eff_denom_ETA_BARREL__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET_->Fill(highestScEtElectronScEt);
		       eff_denom_ETA_BARREL__SEED_BOTH__TRIG_MATCHED__VS_ETA_->Fill(highestScEtElectronEta);
		       eff_denom_ETA_BOTH__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET_->Fill(highestScEtElectronScEt);
		       eff_denom_ETA_BOTH__SEED_TRACKER__TRIG_MATCHED__VS_ETA_->Fill(highestScEtElectronEta);
		       eff_denom_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET_->Fill(highestScEtElectronScEt);
		       eff_denom_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ETA_->Fill(highestScEtElectronEta);
		       if (result_matched_HLTElec10_)
			 {
			   eff_num_ETA_BARREL__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_ELEC10_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BARREL__SEED_TRACKER__TRIG_MATCHED__VS_ETA__HLT_ELEC10_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BARREL__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_ELEC10_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BARREL__SEED_BOTH__TRIG_MATCHED__VS_ETA__HLT_ELEC10_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BOTH__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_ELEC10_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BOTH__SEED_TRACKER__TRIG_MATCHED__VS_ETA__HLT_ELEC10_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_ELEC10_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ETA__HLT_ELEC10_->Fill(highestScEtElectronEta);
			 }
		       if (result_matched_HLTElec15_)
			 {
			   eff_num_ETA_BARREL__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_ELEC15_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BARREL__SEED_TRACKER__TRIG_MATCHED__VS_ETA__HLT_ELEC15_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BARREL__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_ELEC15_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BARREL__SEED_BOTH__TRIG_MATCHED__VS_ETA__HLT_ELEC15_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BOTH__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_ELEC15_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BOTH__SEED_TRACKER__TRIG_MATCHED__VS_ETA__HLT_ELEC15_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_ELEC15_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ETA__HLT_ELEC15_->Fill(highestScEtElectronEta);
			 }
		       if (result_matched_HLTPhot10_)
			 {
			   eff_num_ETA_BARREL__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_PHOT10_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BARREL__SEED_TRACKER__TRIG_MATCHED__VS_ETA__HLT_PHOT10_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BARREL__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_PHOT10_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BARREL__SEED_BOTH__TRIG_MATCHED__VS_ETA__HLT_PHOT10_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BOTH__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_PHOT10_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BOTH__SEED_TRACKER__TRIG_MATCHED__VS_ETA__HLT_PHOT10_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_PHOT10_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ETA__HLT_PHOT10_->Fill(highestScEtElectronEta);
			 }
		       if (result_matched_HLTPhot15_)
			 {
			   std::cout << "photon 15 filling" << std::endl;
			   eff_num_ETA_BARREL__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_PHOT15_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BARREL__SEED_TRACKER__TRIG_MATCHED__VS_ETA__HLT_PHOT15_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BARREL__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_PHOT15_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BARREL__SEED_BOTH__TRIG_MATCHED__VS_ETA__HLT_PHOT15_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BOTH__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_PHOT15_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BOTH__SEED_TRACKER__TRIG_MATCHED__VS_ETA__HLT_PHOT15_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_PHOT15_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ETA__HLT_PHOT15_->Fill(highestScEtElectronEta);
			 }
		       // L1
		       if (result_matched_L1EG5_)
			 {
			   eff_num_ETA_BARREL__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET__L1_EG5_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BARREL__SEED_TRACKER__TRIG_MATCHED__VS_ETA__L1_EG5_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BARREL__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__L1_EG5_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BARREL__SEED_BOTH__TRIG_MATCHED__VS_ETA__L1_EG5_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BOTH__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET__L1_EG5_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BOTH__SEED_TRACKER__TRIG_MATCHED__VS_ETA__L1_EG5_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__L1_EG5_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ETA__L1_EG5_->Fill(highestScEtElectronEta);
			 }
		       if (result_matched_L1EG8_)
			 {
			   eff_num_ETA_BARREL__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET__L1_EG8_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BARREL__SEED_TRACKER__TRIG_MATCHED__VS_ETA__L1_EG8_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BARREL__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__L1_EG8_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BARREL__SEED_BOTH__TRIG_MATCHED__VS_ETA__L1_EG8_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BOTH__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET__L1_EG8_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BOTH__SEED_TRACKER__TRIG_MATCHED__VS_ETA__L1_EG8_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__L1_EG8_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ETA__L1_EG8_->Fill(highestScEtElectronEta);
			 }

		     }
		 }
	       if (highestScEtElectronIsEndcap)
		 {
		   std::cout << "is endcap" << std::endl;
		   if (highestScEtElectronIsEcalDriven)
		     {
		       std::cout << "ecal driven" << std::endl;
		       eff_denom_ETA_ENDCAP__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET_->Fill(highestScEtElectronScEt);
		       eff_denom_ETA_ENDCAP__SEED_ECAL__TRIG_MATCHED__VS_ETA_->Fill(highestScEtElectronEta);
		       eff_denom_ETA_ENDCAP__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET_->Fill(highestScEtElectronScEt);
		       eff_denom_ETA_ENDCAP__SEED_BOTH__TRIG_MATCHED__VS_ETA_->Fill(highestScEtElectronEta);
		       eff_denom_ETA_BOTH__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET_->Fill(highestScEtElectronScEt);
		       eff_denom_ETA_BOTH__SEED_ECAL__TRIG_MATCHED__VS_ETA_->Fill(highestScEtElectronEta);
		       eff_denom_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET_->Fill(highestScEtElectronScEt);
		       eff_denom_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ETA_->Fill(highestScEtElectronEta);
		       if (result_matched_HLTElec10_)
			 {
			   eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_ELEC10_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_MATCHED__VS_ETA__HLT_ELEC10_->Fill(highestScEtElectronEta);
			   eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_ELEC10_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_MATCHED__VS_ETA__HLT_ELEC10_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BOTH__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_ELEC10_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BOTH__SEED_ECAL__TRIG_MATCHED__VS_ETA__HLT_ELEC10_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_ELEC10_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ETA__HLT_ELEC10_->Fill(highestScEtElectronEta);
			 }
		       if (result_matched_HLTElec15_)
			 {
			   eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_ELEC15_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_MATCHED__VS_ETA__HLT_ELEC15_->Fill(highestScEtElectronEta);
			   eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_ELEC15_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_MATCHED__VS_ETA__HLT_ELEC15_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BOTH__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_ELEC15_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BOTH__SEED_ECAL__TRIG_MATCHED__VS_ETA__HLT_ELEC15_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_ELEC15_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ETA__HLT_ELEC15_->Fill(highestScEtElectronEta);
			 }
		       if (result_matched_HLTPhot10_)
			 {
			   eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_PHOT10_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_MATCHED__VS_ETA__HLT_PHOT10_->Fill(highestScEtElectronEta);
			   eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_PHOT10_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_MATCHED__VS_ETA__HLT_PHOT10_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BOTH__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_PHOT10_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BOTH__SEED_ECAL__TRIG_MATCHED__VS_ETA__HLT_PHOT10_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_PHOT10_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ETA__HLT_PHOT10_->Fill(highestScEtElectronEta);
			 }
		       if (result_matched_HLTPhot15_)
			 {
			   eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_PHOT15_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_MATCHED__VS_ETA__HLT_PHOT15_->Fill(highestScEtElectronEta);
			   eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_PHOT15_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_MATCHED__VS_ETA__HLT_PHOT15_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BOTH__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_PHOT15_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BOTH__SEED_ECAL__TRIG_MATCHED__VS_ETA__HLT_PHOT15_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_PHOT15_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ETA__HLT_PHOT15_->Fill(highestScEtElectronEta);
			 }
		       // L1
		       if (result_matched_L1EG5_)
			 {
			   eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET__L1_EG5_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_MATCHED__VS_ETA__L1_EG5_->Fill(highestScEtElectronEta);
			   eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__L1_EG5_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_MATCHED__VS_ETA__L1_EG5_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BOTH__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET__L1_EG5_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BOTH__SEED_ECAL__TRIG_MATCHED__VS_ETA__L1_EG5_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__L1_EG5_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ETA__L1_EG5_->Fill(highestScEtElectronEta);
			 }
		       if (result_matched_L1EG8_)
			 {
			   eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET__L1_EG8_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_MATCHED__VS_ETA__L1_EG8_->Fill(highestScEtElectronEta);
			   eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__L1_EG8_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_MATCHED__VS_ETA__L1_EG8_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BOTH__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET__L1_EG8_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BOTH__SEED_ECAL__TRIG_MATCHED__VS_ETA__L1_EG8_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__L1_EG8_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ETA__L1_EG8_->Fill(highestScEtElectronEta);
			 }

		     }
		   if (highestScEtElectronIsTrackerDriven)
		     {
		       std::cout << "is tracker driven" << std::endl;
		       eff_denom_ETA_ENDCAP__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET_->Fill(highestScEtElectronScEt);
		       eff_denom_ETA_ENDCAP__SEED_TRACKER__TRIG_MATCHED__VS_ETA_->Fill(highestScEtElectronEta);
		       eff_denom_ETA_ENDCAP__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET_->Fill(highestScEtElectronScEt);
		       eff_denom_ETA_ENDCAP__SEED_BOTH__TRIG_MATCHED__VS_ETA_->Fill(highestScEtElectronEta);
		       eff_denom_ETA_BOTH__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET_->Fill(highestScEtElectronScEt);
		       eff_denom_ETA_BOTH__SEED_TRACKER__TRIG_MATCHED__VS_ETA_->Fill(highestScEtElectronEta);
		       eff_denom_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET_->Fill(highestScEtElectronScEt);
		       eff_denom_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ETA_->Fill(highestScEtElectronEta);
		       if (result_matched_HLTElec10_)
			 {
			   eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_ELEC10_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_MATCHED__VS_ETA__HLT_ELEC10_->Fill(highestScEtElectronEta);
			   eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_ELEC10_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_MATCHED__VS_ETA__HLT_ELEC10_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BOTH__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_ELEC10_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BOTH__SEED_TRACKER__TRIG_MATCHED__VS_ETA__HLT_ELEC10_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_ELEC10_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ETA__HLT_ELEC10_->Fill(highestScEtElectronEta);
			 }
		       if (result_matched_HLTElec15_)
			 {
			   eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_ELEC15_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_MATCHED__VS_ETA__HLT_ELEC15_->Fill(highestScEtElectronEta);
			   eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_ELEC15_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_MATCHED__VS_ETA__HLT_ELEC15_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BOTH__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_ELEC15_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BOTH__SEED_TRACKER__TRIG_MATCHED__VS_ETA__HLT_ELEC15_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_ELEC15_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ETA__HLT_ELEC15_->Fill(highestScEtElectronEta);
			 }
		       if (result_matched_HLTPhot10_)
			 {
			   eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_PHOT10_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_MATCHED__VS_ETA__HLT_PHOT10_->Fill(highestScEtElectronEta);
			   eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_PHOT10_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_MATCHED__VS_ETA__HLT_PHOT10_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BOTH__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_PHOT10_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BOTH__SEED_TRACKER__TRIG_MATCHED__VS_ETA__HLT_PHOT10_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_PHOT10_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ETA__HLT_PHOT10_->Fill(highestScEtElectronEta);
			 }
		       if (result_matched_HLTPhot15_)
			 {
			   eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_PHOT15_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_MATCHED__VS_ETA__HLT_PHOT15_->Fill(highestScEtElectronEta);
			   eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_PHOT15_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_MATCHED__VS_ETA__HLT_PHOT15_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BOTH__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_PHOT15_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BOTH__SEED_TRACKER__TRIG_MATCHED__VS_ETA__HLT_PHOT15_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_PHOT15_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ETA__HLT_PHOT15_->Fill(highestScEtElectronEta);
			 }
		       // L1
		       if (result_matched_L1EG5_)
			 {
			   eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET__L1_EG5_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_MATCHED__VS_ETA__L1_EG5_->Fill(highestScEtElectronEta);
			   eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__L1_EG5_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_MATCHED__VS_ETA__L1_EG5_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BOTH__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET__L1_EG5_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BOTH__SEED_TRACKER__TRIG_MATCHED__VS_ETA__L1_EG5_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__L1_EG5_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ETA__L1_EG5_->Fill(highestScEtElectronEta);
			 }
		       if (result_matched_L1EG8_)
			 {
			   eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET__L1_EG8_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_MATCHED__VS_ETA__L1_EG8_->Fill(highestScEtElectronEta);
			   eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__L1_EG8_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_MATCHED__VS_ETA__L1_EG8_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BOTH__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET__L1_EG8_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BOTH__SEED_TRACKER__TRIG_MATCHED__VS_ETA__L1_EG8_->Fill(highestScEtElectronEta);
			   eff_num_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__L1_EG8_->Fill(highestScEtElectronScEt);
			   eff_num_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ETA__L1_EG8_->Fill(highestScEtElectronEta);
			 }
		     }
		 }
	     }
	 }

     } // end loop over entries

   // make efficiencies?
   const int ptBinArraySize = 17;
   const int etaBinArraySize = 16;

   Double_t ptBins[ptBinArraySize] = {0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 24, 28, 32, 40, 48, 60};
   Double_t etaBins[etaBinArraySize] = {-3.0, -2.6, -2.2, -1.8, -1.4, -1.0, -0.6, -0.2, 0.2, 0.6, 1.0, 1.4, 1.8, 2.2, 2.6, 3.0};

//    //DrawEfficiency(TH1F * numHist, TH1F * denomHist, TString imageName, Double_t lowBins[], const int binArraySize)
//    DrawEfficiency(eff_num_ETA_BARREL__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_ELEC10_, 
// 		  eff_denom_ETA_BARREL__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET_, 
// 		  "eff_ETA_BARREL__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_ELEC10_", 
// 		  ptBins, ptBinArraySize);

//    DrawEfficiency(eff_num_ETA_BARREL__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_ELEC15_, 
// 		  eff_denom_ETA_BARREL__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET_, 
// 		  "eff_ETA_BARREL__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_ELEC15_", 
// 		  ptBins, ptBinArraySize);

//    DrawEfficiency(eff_num_ETA_BARREL__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_PHOT10_, 
// 		  eff_denom_ETA_BARREL__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET_, 
// 		  "eff_ETA_BARREL__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_PHOT10_", 
// 		  ptBins, ptBinArraySize);

   DrawEfficiency(eff_num_ETA_BARREL__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_PHOT15_, 
		  eff_denom_ETA_BARREL__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET_, 
		  "eff_ETA_BARREL__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_PHOT15_", 
		  ptBins, ptBinArraySize);

//    DrawEfficiency(eff_num_ETA_BARREL__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET__L1_EG5_, 
// 		  eff_denom_ETA_BARREL__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET_, 
// 		  "eff_ETA_BARREL__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET__L1_EG5_", 
// 		  ptBins, ptBinArraySize);

//    DrawEfficiency(eff_num_ETA_BARREL__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET__L1_EG8_, 
// 		  eff_denom_ETA_BARREL__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET_, 
// 		  "eff_ETA_BARREL__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET__L1_EG8_", 
// 		  ptBins, ptBinArraySize);




//    DrawEfficiency(eff_num_ETA_BARREL__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_ELEC10_, 
// 		  eff_denom_ETA_BARREL__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET_, 
// 		  "eff_ETA_BARREL__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_ELEC10_", 
// 		  ptBins, ptBinArraySize);

//    DrawEfficiency(eff_num_ETA_BARREL__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_ELEC15_, 
// 		  eff_denom_ETA_BARREL__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET_, 
// 		  "eff_ETA_BARREL__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_ELEC15_", 
// 		  ptBins, ptBinArraySize);

//    DrawEfficiency(eff_num_ETA_BARREL__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_PHOT10_, 
// 		  eff_denom_ETA_BARREL__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET_, 
// 		  "eff_ETA_BARREL__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_PHOT10_", 
// 		  ptBins, ptBinArraySize);

   DrawEfficiency(eff_num_ETA_BARREL__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_PHOT15_, 
		  eff_denom_ETA_BARREL__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET_, 
		  "eff_ETA_BARREL__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_PHOT15_", 
		  ptBins, ptBinArraySize);

//    DrawEfficiency(eff_num_ETA_BARREL__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET__L1_EG5_, 
// 		  eff_denom_ETA_BARREL__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET_, 
// 		  "eff_ETA_BARREL__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET__L1_EG5_", 
// 		  ptBins, ptBinArraySize);

//    DrawEfficiency(eff_num_ETA_BARREL__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET__L1_EG8_, 
// 		  eff_denom_ETA_BARREL__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET_, 
// 		  "eff_ETA_BARREL__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET__L1_EG8_", 
// 		  ptBins, ptBinArraySize);


   // WRITE HISTOS TO FILE

   WriteHistograms();


}


void EwkDqmTreeHang::InitHistograms()
{

  // initializing histograms

/* 		  enum {ETA_BARREL, ETA_ENDCAP, ETA_BOTH, ETA_LAST}; */
/* 		  enum {SEED_ECAL, SEED_TRACKER, SEED_BOTH, SEED_LAST}; */
/* 		  enum {TRIG_MATCHED, TRIG_UNMATCHED, TRIG_MATCH_LAST}; */
/* 		  enum {VS_ELEC_SC_ET, VS_ETA, VS_LAST}; */
/* 		  enum {HLT_ELEC10, HLT_ELEC15, HLT_PHOT10, HLT_PHOT15, L1_EG5, L1_EG8, TRIG_OF_INTEREST_LAST}; */

  // barrel, ecal, match, vs elec SC ET
  eff_denom_ETA_BARREL__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET_ = new TH1F("eff_denom_ETA_BARREL__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET_","elec SC E_{T}, barrel, ECAL-driven, trig-matched;Electron SC E_{T};",60,0.,60.);
  eff_num_ETA_BARREL__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_ELEC10_ = new TH1F("eff_num_ETA_BARREL__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_ELEC10_","Eff vs elec SC E_{T}, barrel, ECAL-driven, trig-matched, HLT_ELEC10;Electron SC E_{T};",60,0.,60.);
  eff_num_ETA_BARREL__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_ELEC15_ = new TH1F("eff_num_ETA_BARREL__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_ELEC15_","Eff vs elec SC E_{T}, barrel, ECAL-driven, trig-matched, HLT_ELEC15;Electron SC E_{T};",60,0.,60.);
  eff_num_ETA_BARREL__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_PHOT10_ = new TH1F("eff_num_ETA_BARREL__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_PHOT10_","Eff vs elec SC E_{T}, barrel, ECAL-driven, trig-matched, HLT_PHOT10;Electron SC E_{T};",60,0.,60.);
  eff_num_ETA_BARREL__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_PHOT15_ = new TH1F("eff_num_ETA_BARREL__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_PHOT15_","Eff vs elec SC E_{T}, barrel, ECAL-driven, trig-matched, HLT_PHOT15;Electron SC E_{T};",60,0.,60.);
  eff_num_ETA_BARREL__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET__L1_EG5_ = new TH1F("eff_num_ETA_BARREL__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET__L1_EG5_","Eff vs elec SC E_{T}, barrel, ECAL-driven, trig-matched, L1_EG5;Electron SC E_{T};",60,0.,60.);
  eff_num_ETA_BARREL__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET__L1_EG8_ = new TH1F("eff_num_ETA_BARREL__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET__L1_EG8_","Eff vs elec SC E_{T}, barrel, ECAL-driven, trig-matched, L1_EG8;Electron SC E_{T};",60,0.,60.);
  
  // barrel, ecal, match, vs eta
  eff_denom_ETA_BARREL__SEED_ECAL__TRIG_MATCHED__VS_ETA_ = new TH1F("eff_denom_ETA_BARREL__SEED_ECAL__TRIG_MATCHED__VS_ETA_","elec #eta, barrel, ECAL-driven, trig-matched;Electron #eta;",64,-3.2,3.2);
  eff_num_ETA_BARREL__SEED_ECAL__TRIG_MATCHED__VS_ETA__HLT_ELEC10_ = new TH1F("eff_num_ETA_BARREL__SEED_ECAL__TRIG_MATCHED__VS_ETA__HLT_ELEC10_","Eff vs elec #eta, barrel, ECAL-driven, trig-matched, HLT_ELEC10;Electron #eta;",64,-3.2,3.2);
  eff_num_ETA_BARREL__SEED_ECAL__TRIG_MATCHED__VS_ETA__HLT_ELEC15_ = new TH1F("eff_num_ETA_BARREL__SEED_ECAL__TRIG_MATCHED__VS_ETA__HLT_ELEC15_","Eff vs elec #eta, barrel, ECAL-driven, trig-matched, HLT_ELEC15;Electron #eta;",64,-3.2,3.2);
  eff_num_ETA_BARREL__SEED_ECAL__TRIG_MATCHED__VS_ETA__HLT_PHOT10_ = new TH1F("eff_num_ETA_BARREL__SEED_ECAL__TRIG_MATCHED__VS_ETA__HLT_PHOT10_","Eff vs elec #eta, barrel, ECAL-driven, trig-matched, HLT_PHOT10;Electron #eta;",64,-3.2,3.2);
  eff_num_ETA_BARREL__SEED_ECAL__TRIG_MATCHED__VS_ETA__HLT_PHOT15_ = new TH1F("eff_num_ETA_BARREL__SEED_ECAL__TRIG_MATCHED__VS_ETA__HLT_PHOT15_","Eff vs elec #eta, barrel, ECAL-driven, trig-matched, HLT_PHOT15;Electron #eta;",64,-3.2,3.2);
  eff_num_ETA_BARREL__SEED_ECAL__TRIG_MATCHED__VS_ETA__L1_EG5_ = new TH1F("eff_num_ETA_BARREL__SEED_ECAL__TRIG_MATCHED__VS_ETA__L1_EG5_","Eff vs elec #eta, barrel, ECAL-driven, trig-matched, L1_EG5;Electron #eta;",64,-3.2,3.2);
  eff_num_ETA_BARREL__SEED_ECAL__TRIG_MATCHED__VS_ETA__L1_EG8_ = new TH1F("eff_num_ETA_BARREL__SEED_ECAL__TRIG_MATCHED__VS_ETA__L1_EG8_","Eff vs elec #eta, barrel, ECAL-driven, trig-matched, L1_EG8;Electron #eta;",64,-3.2,3.2);

  // barrel, ecal, non-match, vs elec SC ET
  eff_denom_ETA_BARREL__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET_ = new TH1F("eff_denom_ETA_BARREL__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET_","elec SC E_{T}, barrel, ECAL-driven, non-trig-matched;Electron SC E_{T};",60,0.,60.);
  eff_num_ETA_BARREL__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_ELEC10_ = new TH1F("eff_num_ETA_BARREL__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_ELEC10_","Eff vs elec SC E_{T}, barrel, ECAL-driven, non-trig-matched, HLT_ELEC10;Electron SC E_{T};",60,0.,60.);
  eff_num_ETA_BARREL__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_ELEC15_ = new TH1F("eff_num_ETA_BARREL__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_ELEC15_","Eff vs elec SC E_{T}, barrel, ECAL-driven, non-trig-matched, HLT_ELEC15;Electron SC E_{T};",60,0.,60.);
  eff_num_ETA_BARREL__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_PHOT10_ = new TH1F("eff_num_ETA_BARREL__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_PHOT10_","Eff vs elec SC E_{T}, barrel, ECAL-driven, non-trig-matched, HLT_PHOT10;Electron SC E_{T};",60,0.,60.);
  eff_num_ETA_BARREL__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_PHOT15_ = new TH1F("eff_num_ETA_BARREL__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_PHOT15_","Eff vs elec SC E_{T}, barrel, ECAL-driven, non-trig-matched, HLT_PHOT15;Electron SC E_{T};",60,0.,60.);
  eff_num_ETA_BARREL__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET__L1_EG5_ = new TH1F("eff_num_ETA_BARREL__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET__L1_EG5_","Eff vs elec SC E_{T}, barrel, ECAL-driven, non-trig-matched, L1_EG5;Electron SC E_{T};",60,0.,60.);
  eff_num_ETA_BARREL__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET__L1_EG8_ = new TH1F("eff_num_ETA_BARREL__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET__L1_EG8_","Eff vs elec SC E_{T}, barrel, ECAL-driven, non-trig-matched, L1_EG8;Electron SC E_{T};",60,0.,60.);

  // barrel, ecal, non-match, vs eta
  eff_denom_ETA_BARREL__SEED_ECAL__TRIG_UNMATCHED__VS_ETA_ = new TH1F("eff_denom_ETA_BARREL__SEED_ECAL__TRIG_UNMATCHED__VS_ETA_","elec #eta, barrel, ECAL-driven, non-trig-matched;Electron #eta;",64,-3.2,3.2);
  eff_num_ETA_BARREL__SEED_ECAL__TRIG_UNMATCHED__VS_ETA__HLT_ELEC10_ = new TH1F("eff_num_ETA_BARREL__SEED_ECAL__TRIG_UNMATCHED__VS_ETA__HLT_ELEC10_","Eff vs elec #eta, barrel, ECAL-driven, non-trig-matched, HLT_ELEC10;Electron #eta;",64,-3.2,3.2);
  eff_num_ETA_BARREL__SEED_ECAL__TRIG_UNMATCHED__VS_ETA__HLT_ELEC15_ = new TH1F("eff_num_ETA_BARREL__SEED_ECAL__TRIG_UNMATCHED__VS_ETA__HLT_ELEC15_","Eff vs elec #eta, barrel, ECAL-driven, non-trig-matched, HLT_ELEC15;Electron #eta;",64,-3.2,3.2);
  eff_num_ETA_BARREL__SEED_ECAL__TRIG_UNMATCHED__VS_ETA__HLT_PHOT10_ = new TH1F("eff_num_ETA_BARREL__SEED_ECAL__TRIG_UNMATCHED__VS_ETA__HLT_PHOT10_","Eff vs elec #eta, barrel, ECAL-driven, non-trig-matched, HLT_PHOT10;Electron #eta;",64,-3.2,3.2);
  eff_num_ETA_BARREL__SEED_ECAL__TRIG_UNMATCHED__VS_ETA__HLT_PHOT15_ = new TH1F("eff_num_ETA_BARREL__SEED_ECAL__TRIG_UNMATCHED__VS_ETA__HLT_PHOT15_","Eff vs elec #eta, barrel, ECAL-driven, non-trig-matched, HLT_PHOT15;Electron #eta;",64,-3.2,3.2);
  eff_num_ETA_BARREL__SEED_ECAL__TRIG_UNMATCHED__VS_ETA__L1_EG5_ = new TH1F("eff_num_ETA_BARREL__SEED_ECAL__TRIG_UNMATCHED__VS_ETA__L1_EG5_","Eff vs elec #eta, barrel, ECAL-driven, non-trig-matched, L1_EG5;Electron #eta;",64,-3.2,3.2);
  eff_num_ETA_BARREL__SEED_ECAL__TRIG_UNMATCHED__VS_ETA__L1_EG8_ = new TH1F("eff_num_ETA_BARREL__SEED_ECAL__TRIG_UNMATCHED__VS_ETA__L1_EG8_","Eff vs elec #eta, barrel, ECAL-driven, non-trig-matched, L1_EG8;Electron #eta;",64,-3.2,3.2);

  // barrel, tracker, match, vs elec SC ET
  eff_denom_ETA_BARREL__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET_ = new TH1F("eff_denom_ETA_BARREL__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET_","elec SC E_{T}, barrel, tracker-driven, non-trig-matched;Electron SC E_{T};",60,0.,60.);
  eff_num_ETA_BARREL__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_ELEC10_ = new TH1F("eff_num_ETA_BARREL__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_ELEC10_","Eff vs elec SC E_{T}, barrel, tracker-driven, non-trig-matched, HLT_ELEC10;Electron SC E_{T};",60,0.,60.);
  eff_num_ETA_BARREL__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_ELEC15_ = new TH1F("eff_num_ETA_BARREL__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_ELEC15_","Eff vs elec SC E_{T}, barrel, tracker-driven, non-trig-matched, HLT_ELEC15;Electron SC E_{T};",60,0.,60.);
  eff_num_ETA_BARREL__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_PHOT10_ = new TH1F("eff_num_ETA_BARREL__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_PHOT10_","Eff vs elec SC E_{T}, barrel, tracker-driven, non-trig-matched, HLT_PHOT10;Electron SC E_{T};",60,0.,60.);
  eff_num_ETA_BARREL__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_PHOT15_ = new TH1F("eff_num_ETA_BARREL__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_PHOT15_","Eff vs elec SC E_{T}, barrel, tracker-driven, non-trig-matched, HLT_PHOT15;Electron SC E_{T};",60,0.,60.);
  eff_num_ETA_BARREL__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET__L1_EG5_ = new TH1F("eff_num_ETA_BARREL__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET__L1_EG5_","Eff vs elec SC E_{T}, barrel, tracker-driven, non-trig-matched, L1_EG5;Electron SC E_{T};",60,0.,60.);
  eff_num_ETA_BARREL__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET__L1_EG8_ = new TH1F("eff_num_ETA_BARREL__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET__L1_EG8_","Eff vs elec SC E_{T}, barrel, tracker-driven, non-trig-matched, L1_EG8;Electron SC E_{T};",60,0.,60.);

  // barrel, tracker, match, vs eta
  eff_denom_ETA_BARREL__SEED_TRACKER__TRIG_MATCHED__VS_ETA_ = new TH1F("eff_denom_ETA_BARREL__SEED_TRACKER__TRIG_MATCHED__VS_ETA_","elec #eta, barrel, tracker-driven, trig-matched;Electron #eta;",64,-3.2,3.2);
  eff_num_ETA_BARREL__SEED_TRACKER__TRIG_MATCHED__VS_ETA__HLT_ELEC10_ = new TH1F("eff_num_ETA_BARREL__SEED_TRACKER__TRIG_MATCHED__VS_ETA__HLT_ELEC10_","Eff vs elec #eta, barrel, tracker-driven, trig-matched, HLT_ELEC10;Electron #eta;",64,-3.2,3.2);
  eff_num_ETA_BARREL__SEED_TRACKER__TRIG_MATCHED__VS_ETA__HLT_ELEC15_ = new TH1F("eff_num_ETA_BARREL__SEED_TRACKER__TRIG_MATCHED__VS_ETA__HLT_ELEC15_","Eff vs elec #eta, barrel, tracker-driven, trig-matched, HLT_ELEC15;Electron #eta;",64,-3.2,3.2);
  eff_num_ETA_BARREL__SEED_TRACKER__TRIG_MATCHED__VS_ETA__HLT_PHOT10_ = new TH1F("eff_num_ETA_BARREL__SEED_TRACKER__TRIG_MATCHED__VS_ETA__HLT_PHOT10_","Eff vs elec #eta, barrel, tracker-driven, trig-matched, HLT_PHOT10;Electron #eta;",64,-3.2,3.2);
  eff_num_ETA_BARREL__SEED_TRACKER__TRIG_MATCHED__VS_ETA__HLT_PHOT15_ = new TH1F("eff_num_ETA_BARREL__SEED_TRACKER__TRIG_MATCHED__VS_ETA__HLT_PHOT15_","Eff vs elec #eta, barrel, tracker-driven, trig-matched, HLT_PHOT15;Electron #eta;",64,-3.2,3.2);
  eff_num_ETA_BARREL__SEED_TRACKER__TRIG_MATCHED__VS_ETA__L1_EG5_ = new TH1F("eff_num_ETA_BARREL__SEED_TRACKER__TRIG_MATCHED__VS_ETA__L1_EG5_","Eff vs elec #eta, barrel, tracker-driven, trig-matched, L1_EG5;Electron #eta;",64,-3.2,3.2);
  eff_num_ETA_BARREL__SEED_TRACKER__TRIG_MATCHED__VS_ETA__L1_EG8_ = new TH1F("eff_num_ETA_BARREL__SEED_TRACKER__TRIG_MATCHED__VS_ETA__L1_EG8_","Eff vs elec #eta, barrel, tracker-driven, trig-matched, L1_EG8;Electron #eta;",64,-3.2,3.2);

  // barrel, tracker, non-match, vs elec SC ET
  eff_denom_ETA_BARREL__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET_ = new TH1F("eff_denom_ETA_BARREL__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET_","elec SC E_{T}, barrel, tracker-driven, non-trig-matched;Electron SC E_{T};",60,0.,60.);
  eff_num_ETA_BARREL__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_ELEC10_ = new TH1F("eff_num_ETA_BARREL__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_ELEC10_","Eff vs elec SC E_{T}, barrel, tracker-driven, non-trig-matched, HLT_ELEC10;Electron SC E_{T};",60,0.,60.);
  eff_num_ETA_BARREL__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_ELEC15_ = new TH1F("eff_num_ETA_BARREL__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_ELEC15_","Eff vs elec SC E_{T}, barrel, tracker-driven, non-trig-matched, HLT_ELEC15;Electron SC E_{T};",60,0.,60.);
  eff_num_ETA_BARREL__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_PHOT10_ = new TH1F("eff_num_ETA_BARREL__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_PHOT10_","Eff vs elec SC E_{T}, barrel, tracker-driven, non-trig-matched, HLT_PHOT10;Electron SC E_{T};",60,0.,60.);
  eff_num_ETA_BARREL__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_PHOT15_ = new TH1F("eff_num_ETA_BARREL__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_PHOT15_","Eff vs elec SC E_{T}, barrel, tracker-driven, non-trig-matched, HLT_PHOT15;Electron SC E_{T};",60,0.,60.);
  eff_num_ETA_BARREL__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET__L1_EG5_ = new TH1F("eff_num_ETA_BARREL__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET__L1_EG5_","Eff vs elec SC E_{T}, barrel, tracker-driven, non-trig-matched, L1_EG5;Electron SC E_{T};",60,0.,60.);
  eff_num_ETA_BARREL__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET__L1_EG8_ = new TH1F("eff_num_ETA_BARREL__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET__L1_EG8_","Eff vs elec SC E_{T}, barrel, tracker-driven, non-trig-matched, L1_EG8;Electron SC E_{T};",60,0.,60.);

  // barrel, tracker, non-match, vs eta
  eff_denom_ETA_BARREL__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA_ = new TH1F("eff_denom_ETA_BARREL__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA_","elec #eta, barrel, tracker-driven, non-trig-matched;Electron #eta;",64,-3.2,3.2);
  eff_num_ETA_BARREL__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA__HLT_ELEC10_ = new TH1F("eff_num_ETA_BARREL__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA__HLT_ELEC10_","Eff vs elec #eta, barrel, tracker-driven, non-trig-matched, HLT_ELEC10;Electron #eta;",64,-3.2,3.2);
  eff_num_ETA_BARREL__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA__HLT_ELEC15_ = new TH1F("eff_num_ETA_BARREL__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA__HLT_ELEC15_","Eff vs elec #eta, barrel, tracker-driven, non-trig-matched, HLT_ELEC15;Electron #eta;",64,-3.2,3.2);
  eff_num_ETA_BARREL__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA__HLT_PHOT10_ = new TH1F("eff_num_ETA_BARREL__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA__HLT_PHOT10_","Eff vs elec #eta, barrel, tracker-driven, non-trig-matched, HLT_PHOT10;Electron #eta;",64,-3.2,3.2);
  eff_num_ETA_BARREL__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA__HLT_PHOT15_ = new TH1F("eff_num_ETA_BARREL__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA__HLT_PHOT15_","Eff vs elec #eta, barrel, tracker-driven, non-trig-matched, HLT_PHOT15;Electron #eta;",64,-3.2,3.2);
  eff_num_ETA_BARREL__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA__L1_EG5_ = new TH1F("eff_num_ETA_BARREL__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA__L1_EG5_","Eff vs elec #eta, barrel, tracker-driven, non-trig-matched, L1_EG5;Electron #eta;",64,-3.2,3.2);
  eff_num_ETA_BARREL__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA__L1_EG8_ = new TH1F("eff_num_ETA_BARREL__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA__L1_EG8_","Eff vs elec #eta, barrel, tracker-driven, non-trig-matched, L1_EG8;Electron #eta;",64,-3.2,3.2);

  // barrel, ecal+tracker, match, vs elec SC ET
  eff_denom_ETA_BARREL__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET_ = new TH1F("eff_denom_ETA_BARREL__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET_","elec SC E_{T}, barrel, ecal+tracker-driven, trig-matched;Electron SC E_{T};",60,0.,60.);
  eff_num_ETA_BARREL__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_ELEC10_ = new TH1F("eff_num_ETA_BARREL__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_ELEC10_","Eff vs elec SC E_{T}, barrel, ecal+tracker-driven, trig-matched, HLT_ELEC10;Electron SC E_{T};",60,0.,60.);
  eff_num_ETA_BARREL__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_ELEC15_ = new TH1F("eff_num_ETA_BARREL__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_ELEC15_","Eff vs elec SC E_{T}, barrel, ecal+tracker-driven, trig-matched, HLT_ELEC15;Electron SC E_{T};",60,0.,60.);
  eff_num_ETA_BARREL__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_PHOT10_ = new TH1F("eff_num_ETA_BARREL__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_PHOT10_","Eff vs elec SC E_{T}, barrel, ecal+tracker-driven, trig-matched, HLT_PHOT10;Electron SC E_{T};",60,0.,60.);
  eff_num_ETA_BARREL__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_PHOT15_ = new TH1F("eff_num_ETA_BARREL__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_PHOT15_","Eff vs elec SC E_{T}, barrel, ecal+tracker-driven, trig-matched, HLT_PHOT15;Electron SC E_{T};",60,0.,60.);
  eff_num_ETA_BARREL__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__L1_EG5_ = new TH1F("eff_num_ETA_BARREL__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__L1_EG5_","Eff vs elec SC E_{T}, barrel, ecal+tracker-driven, trig-matched, L1_EG5;Electron SC E_{T};",60,0.,60.);
  eff_num_ETA_BARREL__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__L1_EG8_ = new TH1F("eff_num_ETA_BARREL__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__L1_EG8_","Eff vs elec SC E_{T}, barrel, ecal+tracker-driven, trig-matched, L1_EG8;Electron SC E_{T};",60,0.,60.);

  // barrel, ecal+tracker, match, vs eta
  eff_denom_ETA_BARREL__SEED_BOTH__TRIG_MATCHED__VS_ETA_ = new TH1F("eff_denom_ETA_BARREL__SEED_BOTH__TRIG_MATCHED__VS_ETA_","elec #eta, barrel, ecal+tracker-driven, trig-matched;Electron #eta;",64,-3.2,3.2);
  eff_num_ETA_BARREL__SEED_BOTH__TRIG_MATCHED__VS_ETA__HLT_ELEC10_ = new TH1F("eff_num_ETA_BARREL__SEED_BOTH__TRIG_MATCHED__VS_ETA__HLT_ELEC10_","Eff vs elec #eta, barrel, ecal+tracker-driven, trig-matched, HLT_ELEC10;Electron #eta;",64,-3.2,3.2);
  eff_num_ETA_BARREL__SEED_BOTH__TRIG_MATCHED__VS_ETA__HLT_ELEC15_ = new TH1F("eff_num_ETA_BARREL__SEED_BOTH__TRIG_MATCHED__VS_ETA__HLT_ELEC15_","Eff vs elec #eta, barrel, ecal+tracker-driven, trig-matched, HLT_ELEC15;Electron #eta;",64,-3.2,3.2);
  eff_num_ETA_BARREL__SEED_BOTH__TRIG_MATCHED__VS_ETA__HLT_PHOT10_ = new TH1F("eff_num_ETA_BARREL__SEED_BOTH__TRIG_MATCHED__VS_ETA__HLT_PHOT10_","Eff vs elec #eta, barrel, ecal+tracker-driven, trig-matched, HLT_PHOT10;Electron #eta;",64,-3.2,3.2);
  eff_num_ETA_BARREL__SEED_BOTH__TRIG_MATCHED__VS_ETA__HLT_PHOT15_ = new TH1F("eff_num_ETA_BARREL__SEED_BOTH__TRIG_MATCHED__VS_ETA__HLT_PHOT15_","Eff vs elec #eta, barrel, ecal+tracker-driven, trig-matched, HLT_PHOT15;Electron #eta;",64,-3.2,3.2);
  eff_num_ETA_BARREL__SEED_BOTH__TRIG_MATCHED__VS_ETA__L1_EG5_ = new TH1F("eff_num_ETA_BARREL__SEED_BOTH__TRIG_MATCHED__VS_ETA__L1_EG5_","Eff vs elec #eta, barrel, ecal+tracker-driven, trig-matched, L1_EG5;Electron #eta;",64,-3.2,3.2);
  eff_num_ETA_BARREL__SEED_BOTH__TRIG_MATCHED__VS_ETA__L1_EG8_ = new TH1F("eff_num_ETA_BARREL__SEED_BOTH__TRIG_MATCHED__VS_ETA__L1_EG8_","Eff vs elec #eta, barrel, ecal+tracker-driven, trig-matched, L1_EG8;Electron #eta;",64,-3.2,3.2);

  // barrel, ecal+tracker, non-match, vs elec SC ET
  eff_denom_ETA_BARREL__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET_ = new TH1F("eff_denom_ETA_BARREL__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET_","elec SC E_{T}, barrel, ecal+tracker-driven, non-trig-matched;Electron SC E_{T};",60,0.,60.);
  eff_num_ETA_BARREL__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_ELEC10_ = new TH1F("eff_num_ETA_BARREL__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_ELEC10_","Eff vs elec SC E_{T}, barrel, ecal+tracker-driven, non-trig-matched, HLT_ELEC10;Electron SC E_{T};",60,0.,60.);
  eff_num_ETA_BARREL__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_ELEC15_ = new TH1F("eff_num_ETA_BARREL__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_ELEC15_","Eff vs elec SC E_{T}, barrel, ecal+tracker-driven, non-trig-matched, HLT_ELEC15;Electron SC E_{T};",60,0.,60.);
  eff_num_ETA_BARREL__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_PHOT10_ = new TH1F("eff_num_ETA_BARREL__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_PHOT10_","Eff vs elec SC E_{T}, barrel, ecal+tracker-driven, non-trig-matched, HLT_PHOT10;Electron SC E_{T};",60,0.,60.);
  eff_num_ETA_BARREL__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_PHOT15_ = new TH1F("eff_num_ETA_BARREL__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_PHOT15_","Eff vs elec SC E_{T}, barrel, ecal+tracker-driven, non-trig-matched, HLT_PHOT15;Electron SC E_{T};",60,0.,60.);
  eff_num_ETA_BARREL__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__L1_EG5_ = new TH1F("eff_num_ETA_BARREL__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__L1_EG5_","Eff vs elec SC E_{T}, barrel, ecal+tracker-driven, non-trig-matched, L1_EG5;Electron SC E_{T};",60,0.,60.);
  eff_num_ETA_BARREL__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__L1_EG8_ = new TH1F("eff_num_ETA_BARREL__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__L1_EG8_","Eff vs elec SC E_{T}, barrel, ecal+tracker-driven, non-trig-matched, L1_EG8;Electron SC E_{T};",60,0.,60.);

  // barrel, ecal+tracker, non-match, vs eta
  eff_denom_ETA_BARREL__SEED_BOTH__TRIG_UNMATCHED__VS_ETA_ = new TH1F("eff_denom_ETA_BARREL__SEED_BOTH__TRIG_UNMATCHED__VS_ETA_","elec #eta, barrel, ecal+tracker-driven, non-trig-matched;Electron #eta;",64,-3.2,3.2);
  eff_num_ETA_BARREL__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__HLT_ELEC10_ = new TH1F("eff_num_ETA_BARREL__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__HLT_ELEC10_","Eff vs elec #eta, barrel, ecal+tracker-driven, non-trig-matched, HLT_ELEC10;Electron #eta;",64,-3.2,3.2);
  eff_num_ETA_BARREL__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__HLT_ELEC15_ = new TH1F("eff_num_ETA_BARREL__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__HLT_ELEC15_","Eff vs elec #eta, barrel, ecal+tracker-driven, non-trig-matched, HLT_ELEC15;Electron #eta;",64,-3.2,3.2);
  eff_num_ETA_BARREL__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__HLT_PHOT10_ = new TH1F("eff_num_ETA_BARREL__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__HLT_PHOT10_","Eff vs elec #eta, barrel, ecal+tracker-driven, non-trig-matched, HLT_PHOT10;Electron #eta;",64,-3.2,3.2);
  eff_num_ETA_BARREL__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__HLT_PHOT15_ = new TH1F("eff_num_ETA_BARREL__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__HLT_PHOT15_","Eff vs elec #eta, barrel, ecal+tracker-driven, non-trig-matched, HLT_PHOT15;Electron #eta;",64,-3.2,3.2);
  eff_num_ETA_BARREL__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__L1_EG5_ = new TH1F("eff_num_ETA_BARREL__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__L1_EG5_","Eff vs elec #eta, barrel, ecal+tracker-driven, non-trig-matched, L1_EG5;Electron #eta;",64,-3.2,3.2);
  eff_num_ETA_BARREL__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__L1_EG8_ = new TH1F("eff_num_ETA_BARREL__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__L1_EG8_","Eff vs elec #eta, barrel, ecal+tracker-driven, non-trig-matched, L1_EG8;Electron #eta;",64,-3.2,3.2);

  // endcap, ecal, match, vs elec SC ET
  eff_denom_ETA_ENDCAP__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET_ = new TH1F("eff_denom_ETA_ENDCAP__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET_","elec SC E_{T}, endcap, ECAL-driven, trig-matched;Electron SC E_{T};",60,0.,60.);
  eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_ELEC10_ = new TH1F("eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_ELEC10_","Eff vs elec SC E_{T}, endcap, ECAL-driven, trig-matched, HLT_ELEC10;Electron SC E_{T};",60,0.,60.);
  eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_ELEC15_ = new TH1F("eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_ELEC15_","Eff vs elec SC E_{T}, endcap, ECAL-driven, trig-matched, HLT_ELEC15;Electron SC E_{T};",60,0.,60.);
  eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_PHOT10_ = new TH1F("eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_PHOT10_","Eff vs elec SC E_{T}, endcap, ECAL-driven, trig-matched, HLT_PHOT10;Electron SC E_{T};",60,0.,60.);
  eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_PHOT15_ = new TH1F("eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_PHOT15_","Eff vs elec SC E_{T}, endcap, ECAL-driven, trig-matched, HLT_PHOT15;Electron SC E_{T};",60,0.,60.);
  eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET__L1_EG5_ = new TH1F("eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET__L1_EG5_","Eff vs elec SC E_{T}, endcap, ECAL-driven, trig-matched, L1_EG5;Electron SC E_{T};",60,0.,60.);
  eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET__L1_EG8_ = new TH1F("eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET__L1_EG8_","Eff vs elec SC E_{T}, endcap, ECAL-driven, trig-matched, L1_EG8;Electron SC E_{T};",60,0.,60.);

  // endcap, ecal, match, vs eta
  eff_denom_ETA_ENDCAP__SEED_ECAL__TRIG_MATCHED__VS_ETA_ = new TH1F("eff_denom_ETA_ENDCAP__SEED_ECAL__TRIG_MATCHED__VS_ETA_","elec #eta, endcap, ECAL-driven, trig-matched;Electron #eta;",64,-3.2,3.2);
  eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_MATCHED__VS_ETA__HLT_ELEC10_ = new TH1F("eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_MATCHED__VS_ETA__HLT_ELEC10_","Eff vs elec #eta, endcap, ECAL-driven, trig-matched, HLT_ELEC10;Electron #eta;",64,-3.2,3.2);
  eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_MATCHED__VS_ETA__HLT_ELEC15_ = new TH1F("eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_MATCHED__VS_ETA__HLT_ELEC15_","Eff vs elec #eta, endcap, ECAL-driven, trig-matched, HLT_ELEC15;Electron #eta;",64,-3.2,3.2);
  eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_MATCHED__VS_ETA__HLT_PHOT10_ = new TH1F("eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_MATCHED__VS_ETA__HLT_PHOT10_","Eff vs elec #eta, endcap, ECAL-driven, trig-matched, HLT_PHOT10;Electron #eta;",64,-3.2,3.2);
  eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_MATCHED__VS_ETA__HLT_PHOT15_ = new TH1F("eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_MATCHED__VS_ETA__HLT_PHOT15_","Eff vs elec #eta, endcap, ECAL-driven, trig-matched, HLT_PHOT15;Electron #eta;",64,-3.2,3.2);
  eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_MATCHED__VS_ETA__L1_EG5_ = new TH1F("eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_MATCHED__VS_ETA__L1_EG5_","Eff vs elec #eta, endcap, ECAL-driven, trig-matched, L1_EG5;Electron #eta;",64,-3.2,3.2);
  eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_MATCHED__VS_ETA__L1_EG8_ = new TH1F("eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_MATCHED__VS_ETA__L1_EG8_","Eff vs elec #eta, endcap, ECAL-driven, trig-matched, L1_EG8;Electron #eta;",64,-3.2,3.2);

  // endcap, ecal, non-match, vs elec SC ET
  eff_denom_ETA_ENDCAP__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET_ = new TH1F("eff_denom_ETA_ENDCAP__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET_","elec SC E_{T}, endcap, ECAL-driven, non-trig-matched;Electron SC E_{T};",60,0.,60.);
  eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_ELEC10_ = new TH1F("eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_ELEC10_","Eff vs elec SC E_{T}, endcap, ECAL-driven, non-trig-matched, HLT_ELEC10;Electron SC E_{T};",60,0.,60.);
  eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_ELEC15_ = new TH1F("eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_ELEC15_","Eff vs elec SC E_{T}, endcap, ECAL-driven, non-trig-matched, HLT_ELEC15;Electron SC E_{T};",60,0.,60.);
  eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_PHOT10_ = new TH1F("eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_PHOT10_","Eff vs elec SC E_{T}, endcap, ECAL-driven, non-trig-matched, HLT_PHOT10;Electron SC E_{T};",60,0.,60.);
  eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_PHOT15_ = new TH1F("eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_PHOT15_","Eff vs elec SC E_{T}, endcap, ECAL-driven, non-trig-matched, HLT_PHOT15;Electron SC E_{T};",60,0.,60.);
  eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET__L1_EG5_ = new TH1F("eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET__L1_EG5_","Eff vs elec SC E_{T}, endcap, ECAL-driven, non-trig-matched, L1_EG5;Electron SC E_{T};",60,0.,60.);
  eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET__L1_EG8_ = new TH1F("eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET__L1_EG8_","Eff vs elec SC E_{T}, endcap, ECAL-driven, non-trig-matched, L1_EG8;Electron SC E_{T};",60,0.,60.);

  // endcap, ecal, non-match, vs eta
  eff_denom_ETA_ENDCAP__SEED_ECAL__TRIG_UNMATCHED__VS_ETA_ = new TH1F("eff_denom_ETA_ENDCAP__SEED_ECAL__TRIG_UNMATCHED__VS_ETA_","elec #eta, endcap, ECAL-driven, non-trig-matched;Electron #eta;",64,-3.2,3.2);
  eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_UNMATCHED__VS_ETA__HLT_ELEC10_ = new TH1F("eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_UNMATCHED__VS_ETA__HLT_ELEC10_","Eff vs elec #eta, endcap, ECAL-driven, non-trig-matched, HLT_ELEC10;Electron #eta;",64,-3.2,3.2);
  eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_UNMATCHED__VS_ETA__HLT_ELEC15_ = new TH1F("eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_UNMATCHED__VS_ETA__HLT_ELEC15_","Eff vs elec #eta, endcap, ECAL-driven, non-trig-matched, HLT_ELEC15;Electron #eta;",64,-3.2,3.2);
  eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_UNMATCHED__VS_ETA__HLT_PHOT10_ = new TH1F("eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_UNMATCHED__VS_ETA__HLT_PHOT10_","Eff vs elec #eta, endcap, ECAL-driven, non-trig-matched, HLT_PHOT10;Electron #eta;",64,-3.2,3.2);
  eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_UNMATCHED__VS_ETA__HLT_PHOT15_ = new TH1F("eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_UNMATCHED__VS_ETA__HLT_PHOT15_","Eff vs elec #eta, endcap, ECAL-driven, non-trig-matched, HLT_PHOT15;Electron #eta;",64,-3.2,3.2);
  eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_UNMATCHED__VS_ETA__L1_EG5_ = new TH1F("eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_UNMATCHED__VS_ETA__L1_EG5_","Eff vs elec #eta, endcap, ECAL-driven, non-trig-matched, L1_EG5;Electron #eta;",64,-3.2,3.2);
  eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_UNMATCHED__VS_ETA__L1_EG8_ = new TH1F("eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_UNMATCHED__VS_ETA__L1_EG8_","Eff vs elec #eta, endcap, ECAL-driven, non-trig-matched, L1_EG8;Electron #eta;",64,-3.2,3.2);

  // endcap, tracker, match, vs elec SC ET
  eff_denom_ETA_ENDCAP__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET_ = new TH1F("eff_denom_ETA_ENDCAP__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET_","elec SC E_{T}, endcap, tracker-driven, non-trig-matched;Electron SC E_{T};",60,0.,60.);
  eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_ELEC10_ = new TH1F("eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_ELEC10_","Eff vs elec SC E_{T}, endcap, tracker-driven, non-trig-matched, HLT_ELEC10;Electron SC E_{T};",60,0.,60.);
  eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_ELEC15_ = new TH1F("eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_ELEC15_","Eff vs elec SC E_{T}, endcap, tracker-driven, non-trig-matched, HLT_ELEC15;Electron SC E_{T};",60,0.,60.);
  eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_PHOT10_ = new TH1F("eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_PHOT10_","Eff vs elec SC E_{T}, endcap, tracker-driven, non-trig-matched, HLT_PHOT10;Electron SC E_{T};",60,0.,60.);
  eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_PHOT15_ = new TH1F("eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_PHOT15_","Eff vs elec SC E_{T}, endcap, tracker-driven, non-trig-matched, HLT_PHOT15;Electron SC E_{T};",60,0.,60.);
  eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET__L1_EG5_ = new TH1F("eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET__L1_EG5_","Eff vs elec SC E_{T}, endcap, tracker-driven, non-trig-matched, L1_EG5;Electron SC E_{T};",60,0.,60.);
  eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET__L1_EG8_ = new TH1F("eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET__L1_EG8_","Eff vs elec SC E_{T}, endcap, tracker-driven, non-trig-matched, L1_EG8;Electron SC E_{T};",60,0.,60.);

  // endcap, tracker, match, vs eta
  eff_denom_ETA_ENDCAP__SEED_TRACKER__TRIG_MATCHED__VS_ETA_ = new TH1F("eff_denom_ETA_ENDCAP__SEED_TRACKER__TRIG_MATCHED__VS_ETA_","elec #eta, endcap, tracker-driven, trig-matched;Electron #eta;",64,-3.2,3.2);
  eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_MATCHED__VS_ETA__HLT_ELEC10_ = new TH1F("eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_MATCHED__VS_ETA__HLT_ELEC10_","Eff vs elec #eta, endcap, tracker-driven, trig-matched, HLT_ELEC10;Electron #eta;",64,-3.2,3.2);
  eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_MATCHED__VS_ETA__HLT_ELEC15_ = new TH1F("eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_MATCHED__VS_ETA__HLT_ELEC15_","Eff vs elec #eta, endcap, tracker-driven, trig-matched, HLT_ELEC15;Electron #eta;",64,-3.2,3.2);
  eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_MATCHED__VS_ETA__HLT_PHOT10_ = new TH1F("eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_MATCHED__VS_ETA__HLT_PHOT10_","Eff vs elec #eta, endcap, tracker-driven, trig-matched, HLT_PHOT10;Electron #eta;",64,-3.2,3.2);
  eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_MATCHED__VS_ETA__HLT_PHOT15_ = new TH1F("eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_MATCHED__VS_ETA__HLT_PHOT15_","Eff vs elec #eta, endcap, tracker-driven, trig-matched, HLT_PHOT15;Electron #eta;",64,-3.2,3.2);
  eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_MATCHED__VS_ETA__L1_EG5_ = new TH1F("eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_MATCHED__VS_ETA__L1_EG5_","Eff vs elec #eta, endcap, tracker-driven, trig-matched, L1_EG5;Electron #eta;",64,-3.2,3.2);
  eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_MATCHED__VS_ETA__L1_EG8_ = new TH1F("eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_MATCHED__VS_ETA__L1_EG8_","Eff vs elec #eta, endcap, tracker-driven, trig-matched, L1_EG8;Electron #eta;",64,-3.2,3.2);

  // endcap, tracker, non-match, vs elec SC ET
  eff_denom_ETA_ENDCAP__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET_ = new TH1F("eff_denom_ETA_ENDCAP__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET_","elec SC E_{T}, endcap, tracker-driven, non-trig-matched;Electron SC E_{T};",60,0.,60.);
  eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_ELEC10_ = new TH1F("eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_ELEC10_","Eff vs elec SC E_{T}, endcap, tracker-driven, non-trig-matched, HLT_ELEC10;Electron SC E_{T};",60,0.,60.);
  eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_ELEC15_ = new TH1F("eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_ELEC15_","Eff vs elec SC E_{T}, endcap, tracker-driven, non-trig-matched, HLT_ELEC15;Electron SC E_{T};",60,0.,60.);
  eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_PHOT10_ = new TH1F("eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_PHOT10_","Eff vs elec SC E_{T}, endcap, tracker-driven, non-trig-matched, HLT_PHOT10;Electron SC E_{T};",60,0.,60.);
  eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_PHOT15_ = new TH1F("eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_PHOT15_","Eff vs elec SC E_{T}, endcap, tracker-driven, non-trig-matched, HLT_PHOT15;Electron SC E_{T};",60,0.,60.);
  eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET__L1_EG5_ = new TH1F("eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET__L1_EG5_","Eff vs elec SC E_{T}, endcap, tracker-driven, non-trig-matched, L1_EG5;Electron SC E_{T};",60,0.,60.);
  eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET__L1_EG8_ = new TH1F("eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET__L1_EG8_","Eff vs elec SC E_{T}, endcap, tracker-driven, non-trig-matched, L1_EG8;Electron SC E_{T};",60,0.,60.);

  // endcap, tracker, non-match, vs eta
  eff_denom_ETA_ENDCAP__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA_ = new TH1F("eff_denom_ETA_ENDCAP__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA_","elec #eta, endcap, tracker-driven, non-trig-matched;Electron #eta;",64,-3.2,3.2);
  eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA__HLT_ELEC10_ = new TH1F("eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA__HLT_ELEC10_","Eff vs elec #eta, endcap, tracker-driven, non-trig-matched, HLT_ELEC10;Electron #eta;",64,-3.2,3.2);
  eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA__HLT_ELEC15_ = new TH1F("eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA__HLT_ELEC15_","Eff vs elec #eta, endcap, tracker-driven, non-trig-matched, HLT_ELEC15;Electron #eta;",64,-3.2,3.2);
  eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA__HLT_PHOT10_ = new TH1F("eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA__HLT_PHOT10_","Eff vs elec #eta, endcap, tracker-driven, non-trig-matched, HLT_PHOT10;Electron #eta;",64,-3.2,3.2);
  eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA__HLT_PHOT15_ = new TH1F("eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA__HLT_PHOT15_","Eff vs elec #eta, endcap, tracker-driven, non-trig-matched, HLT_PHOT15;Electron #eta;",64,-3.2,3.2);
  eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA__L1_EG5_ = new TH1F("eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA__L1_EG5_","Eff vs elec #eta, endcap, tracker-driven, non-trig-matched, L1_EG5;Electron #eta;",64,-3.2,3.2);
  eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA__L1_EG8_ = new TH1F("eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA__L1_EG8_","Eff vs elec #eta, endcap, tracker-driven, non-trig-matched, L1_EG8;Electron #eta;",64,-3.2,3.2);

  // endcap, ecal+tracker, match, vs elec SC ET
  eff_denom_ETA_ENDCAP__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET_ = new TH1F("eff_denom_ETA_ENDCAP__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET_","elec SC E_{T}, endcap, ecal+tracker-driven, trig-matched;Electron SC E_{T};",60,0.,60.);
  eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_ELEC10_ = new TH1F("eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_ELEC10_","Eff vs elec SC E_{T}, endcap, ecal+tracker-driven, trig-matched, HLT_ELEC10;Electron SC E_{T};",60,0.,60.);
  eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_ELEC15_ = new TH1F("eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_ELEC15_","Eff vs elec SC E_{T}, endcap, ecal+tracker-driven, trig-matched, HLT_ELEC15;Electron SC E_{T};",60,0.,60.);
  eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_PHOT10_ = new TH1F("eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_PHOT10_","Eff vs elec SC E_{T}, endcap, ecal+tracker-driven, trig-matched, HLT_PHOT10;Electron SC E_{T};",60,0.,60.);
  eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_PHOT15_ = new TH1F("eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_PHOT15_","Eff vs elec SC E_{T}, endcap, ecal+tracker-driven, trig-matched, HLT_PHOT15;Electron SC E_{T};",60,0.,60.);
  eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__L1_EG5_ = new TH1F("eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__L1_EG5_","Eff vs elec SC E_{T}, endcap, ecal+tracker-driven, trig-matched, L1_EG5;Electron SC E_{T};",60,0.,60.);
  eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__L1_EG8_ = new TH1F("eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__L1_EG8_","Eff vs elec SC E_{T}, endcap, ecal+tracker-driven, trig-matched, L1_EG8;Electron SC E_{T};",60,0.,60.);

  // endcap, ecal+tracker, match, vs eta
  eff_denom_ETA_ENDCAP__SEED_BOTH__TRIG_MATCHED__VS_ETA_ = new TH1F("eff_denom_ETA_ENDCAP__SEED_BOTH__TRIG_MATCHED__VS_ETA_","elec #eta, endcap, ecal+tracker-driven, trig-matched;Electron #eta;",64,-3.2,3.2);
  eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_MATCHED__VS_ETA__HLT_ELEC10_ = new TH1F("eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_MATCHED__VS_ETA__HLT_ELEC10_","Eff vs elec #eta, endcap, ecal+tracker-driven, trig-matched, HLT_ELEC10;Electron #eta;",64,-3.2,3.2);
  eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_MATCHED__VS_ETA__HLT_ELEC15_ = new TH1F("eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_MATCHED__VS_ETA__HLT_ELEC15_","Eff vs elec #eta, endcap, ecal+tracker-driven, trig-matched, HLT_ELEC15;Electron #eta;",64,-3.2,3.2);
  eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_MATCHED__VS_ETA__HLT_PHOT10_ = new TH1F("eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_MATCHED__VS_ETA__HLT_PHOT10_","Eff vs elec #eta, endcap, ecal+tracker-driven, trig-matched, HLT_PHOT10;Electron #eta;",64,-3.2,3.2);
  eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_MATCHED__VS_ETA__HLT_PHOT15_ = new TH1F("eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_MATCHED__VS_ETA__HLT_PHOT15_","Eff vs elec #eta, endcap, ecal+tracker-driven, trig-matched, HLT_PHOT15;Electron #eta;",64,-3.2,3.2);
  eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_MATCHED__VS_ETA__L1_EG5_ = new TH1F("eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_MATCHED__VS_ETA__L1_EG5_","Eff vs elec #eta, endcap, ecal+tracker-driven, trig-matched, L1_EG5;Electron #eta;",64,-3.2,3.2);
  eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_MATCHED__VS_ETA__L1_EG8_ = new TH1F("eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_MATCHED__VS_ETA__L1_EG8_","Eff vs elec #eta, endcap, ecal+tracker-driven, trig-matched, L1_EG8;Electron #eta;",64,-3.2,3.2);

  // endcap, ecal+tracker, non-match, vs elec SC ET
  eff_denom_ETA_ENDCAP__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET_ = new TH1F("eff_denom_ETA_ENDCAP__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET_","elec SC E_{T}, endcap, ecal+tracker-driven, non-trig-matched;Electron SC E_{T};",60,0.,60.);
  eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_ELEC10_ = new TH1F("eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_ELEC10_","Eff vs elec SC E_{T}, endcap, ecal+tracker-driven, non-trig-matched, HLT_ELEC10;Electron SC E_{T};",60,0.,60.);
  eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_ELEC15_ = new TH1F("eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_ELEC15_","Eff vs elec SC E_{T}, endcap, ecal+tracker-driven, non-trig-matched, HLT_ELEC15;Electron SC E_{T};",60,0.,60.);
  eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_PHOT10_ = new TH1F("eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_PHOT10_","Eff vs elec SC E_{T}, endcap, ecal+tracker-driven, non-trig-matched, HLT_PHOT10;Electron SC E_{T};",60,0.,60.);
  eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_PHOT15_ = new TH1F("eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_PHOT15_","Eff vs elec SC E_{T}, endcap, ecal+tracker-driven, non-trig-matched, HLT_PHOT15;Electron SC E_{T};",60,0.,60.);
  eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__L1_EG5_ = new TH1F("eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__L1_EG5_","Eff vs elec SC E_{T}, endcap, ecal+tracker-driven, non-trig-matched, L1_EG5;Electron SC E_{T};",60,0.,60.);
  eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__L1_EG8_ = new TH1F("eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__L1_EG8_","Eff vs elec SC E_{T}, endcap, ecal+tracker-driven, non-trig-matched, L1_EG8;Electron SC E_{T};",60,0.,60.);

  // endcap, ecal+tracker, non-match, vs eta
  eff_denom_ETA_ENDCAP__SEED_BOTH__TRIG_UNMATCHED__VS_ETA_ = new TH1F("eff_denom_ETA_ENDCAP__SEED_BOTH__TRIG_UNMATCHED__VS_ETA_","elec #eta, endcap, ecal+tracker-driven, non-trig-matched;Electron #eta;",64,-3.2,3.2);
  eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__HLT_ELEC10_ = new TH1F("eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__HLT_ELEC10_","Eff vs elec #eta, endcap, ecal+tracker-driven, non-trig-matched, HLT_ELEC10;Electron #eta;",64,-3.2,3.2);
  eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__HLT_ELEC15_ = new TH1F("eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__HLT_ELEC15_","Eff vs elec #eta, endcap, ecal+tracker-driven, non-trig-matched, HLT_ELEC15;Electron #eta;",64,-3.2,3.2);
  eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__HLT_PHOT10_ = new TH1F("eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__HLT_PHOT10_","Eff vs elec #eta, endcap, ecal+tracker-driven, non-trig-matched, HLT_PHOT10;Electron #eta;",64,-3.2,3.2);
  eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__HLT_PHOT15_ = new TH1F("eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__HLT_PHOT15_","Eff vs elec #eta, endcap, ecal+tracker-driven, non-trig-matched, HLT_PHOT15;Electron #eta;",64,-3.2,3.2);
  eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__L1_EG5_ = new TH1F("eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__L1_EG5_","Eff vs elec #eta, endcap, ecal+tracker-driven, non-trig-matched, L1_EG5;Electron #eta;",64,-3.2,3.2);
  eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__L1_EG8_ = new TH1F("eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__L1_EG8_","Eff vs elec #eta, endcap, ecal+tracker-driven, non-trig-matched, L1_EG8;Electron #eta;",64,-3.2,3.2);

  // barrel+endcap, ecal, match, vs elec SC ET
  eff_denom_ETA_BOTH__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET_ = new TH1F("eff_denom_ETA_BOTH__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET_","elec SC E_{T}, barrel+endcap, ECAL-driven, trig-matched;Electron SC E_{T};",60,0.,60.);
  eff_num_ETA_BOTH__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_ELEC10_ = new TH1F("eff_num_ETA_BOTH__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_ELEC10_","Eff vs elec SC E_{T}, barrel+endcap, ECAL-driven, trig-matched, HLT_ELEC10;Electron SC E_{T};",60,0.,60.);
  eff_num_ETA_BOTH__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_ELEC15_ = new TH1F("eff_num_ETA_BOTH__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_ELEC15_","Eff vs elec SC E_{T}, barrel+endcap, ECAL-driven, trig-matched, HLT_ELEC15;Electron SC E_{T};",60,0.,60.);
  eff_num_ETA_BOTH__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_PHOT10_ = new TH1F("eff_num_ETA_BOTH__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_PHOT10_","Eff vs elec SC E_{T}, barrel+endcap, ECAL-driven, trig-matched, HLT_PHOT10;Electron SC E_{T};",60,0.,60.);
  eff_num_ETA_BOTH__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_PHOT15_ = new TH1F("eff_num_ETA_BOTH__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_PHOT15_","Eff vs elec SC E_{T}, barrel+endcap, ECAL-driven, trig-matched, HLT_PHOT15;Electron SC E_{T};",60,0.,60.);
  eff_num_ETA_BOTH__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET__L1_EG5_ = new TH1F("eff_num_ETA_BOTH__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET__L1_EG5_","Eff vs elec SC E_{T}, barrel+endcap, ECAL-driven, trig-matched, L1_EG5;Electron SC E_{T};",60,0.,60.);
  eff_num_ETA_BOTH__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET__L1_EG8_ = new TH1F("eff_num_ETA_BOTH__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET__L1_EG8_","Eff vs elec SC E_{T}, barrel+endcap, ECAL-driven, trig-matched, L1_EG8;Electron SC E_{T};",60,0.,60.);

  // barrel+endcap, ecal, match, vs eta
  eff_denom_ETA_BOTH__SEED_ECAL__TRIG_MATCHED__VS_ETA_ = new TH1F("eff_denom_ETA_BOTH__SEED_ECAL__TRIG_MATCHED__VS_ETA_","elec #eta, barrel+endcap, ECAL-driven, trig-matched;Electron #eta;",64,-3.2,3.2);
  eff_num_ETA_BOTH__SEED_ECAL__TRIG_MATCHED__VS_ETA__HLT_ELEC10_ = new TH1F("eff_num_ETA_BOTH__SEED_ECAL__TRIG_MATCHED__VS_ETA__HLT_ELEC10_","Eff vs elec #eta, barrel+endcap, ECAL-driven, trig-matched, HLT_ELEC10;Electron #eta;",64,-3.2,3.2);
  eff_num_ETA_BOTH__SEED_ECAL__TRIG_MATCHED__VS_ETA__HLT_ELEC15_ = new TH1F("eff_num_ETA_BOTH__SEED_ECAL__TRIG_MATCHED__VS_ETA__HLT_ELEC15_","Eff vs elec #eta, barrel+endcap, ECAL-driven, trig-matched, HLT_ELEC15;Electron #eta;",64,-3.2,3.2);
  eff_num_ETA_BOTH__SEED_ECAL__TRIG_MATCHED__VS_ETA__HLT_PHOT10_ = new TH1F("eff_num_ETA_BOTH__SEED_ECAL__TRIG_MATCHED__VS_ETA__HLT_PHOT10_","Eff vs elec #eta, barrel+endcap, ECAL-driven, trig-matched, HLT_PHOT10;Electron #eta;",64,-3.2,3.2);
  eff_num_ETA_BOTH__SEED_ECAL__TRIG_MATCHED__VS_ETA__HLT_PHOT15_ = new TH1F("eff_num_ETA_BOTH__SEED_ECAL__TRIG_MATCHED__VS_ETA__HLT_PHOT15_","Eff vs elec #eta, barrel+endcap, ECAL-driven, trig-matched, HLT_PHOT15;Electron #eta;",64,-3.2,3.2);
  eff_num_ETA_BOTH__SEED_ECAL__TRIG_MATCHED__VS_ETA__L1_EG5_ = new TH1F("eff_num_ETA_BOTH__SEED_ECAL__TRIG_MATCHED__VS_ETA__L1_EG5_","Eff vs elec #eta, barrel+endcap, ECAL-driven, trig-matched, L1_EG5;Electron #eta;",64,-3.2,3.2);
  eff_num_ETA_BOTH__SEED_ECAL__TRIG_MATCHED__VS_ETA__L1_EG8_ = new TH1F("eff_num_ETA_BOTH__SEED_ECAL__TRIG_MATCHED__VS_ETA__L1_EG8_","Eff vs elec #eta, barrel+endcap, ECAL-driven, trig-matched, L1_EG8;Electron #eta;",64,-3.2,3.2);

  // barrel+endcap, ecal, non-match, vs elec SC ET
  eff_denom_ETA_BOTH__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET_ = new TH1F("eff_denom_ETA_BOTH__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET_","elec SC E_{T}, barrel+endcap, ECAL-driven, non-trig-matched;Electron SC E_{T};",60,0.,60.);
  eff_num_ETA_BOTH__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_ELEC10_ = new TH1F("eff_num_ETA_BOTH__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_ELEC10_","Eff vs elec SC E_{T}, barrel+endcap, ECAL-driven, non-trig-matched, HLT_ELEC10;Electron SC E_{T};",60,0.,60.);
  eff_num_ETA_BOTH__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_ELEC15_ = new TH1F("eff_num_ETA_BOTH__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_ELEC15_","Eff vs elec SC E_{T}, barrel+endcap, ECAL-driven, non-trig-matched, HLT_ELEC15;Electron SC E_{T};",60,0.,60.);
  eff_num_ETA_BOTH__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_PHOT10_ = new TH1F("eff_num_ETA_BOTH__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_PHOT10_","Eff vs elec SC E_{T}, barrel+endcap, ECAL-driven, non-trig-matched, HLT_PHOT10;Electron SC E_{T};",60,0.,60.);
  eff_num_ETA_BOTH__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_PHOT15_ = new TH1F("eff_num_ETA_BOTH__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_PHOT15_","Eff vs elec SC E_{T}, barrel+endcap, ECAL-driven, non-trig-matched, HLT_PHOT15;Electron SC E_{T};",60,0.,60.);
  eff_num_ETA_BOTH__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET__L1_EG5_ = new TH1F("eff_num_ETA_BOTH__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET__L1_EG5_","Eff vs elec SC E_{T}, barrel+endcap, ECAL-driven, non-trig-matched, L1_EG5;Electron SC E_{T};",60,0.,60.);
  eff_num_ETA_BOTH__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET__L1_EG8_ = new TH1F("eff_num_ETA_BOTH__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET__L1_EG8_","Eff vs elec SC E_{T}, barrel+endcap, ECAL-driven, non-trig-matched, L1_EG8;Electron SC E_{T};",60,0.,60.);

  // barrel+endcap, ecal, non-match, vs eta
  eff_denom_ETA_BOTH__SEED_ECAL__TRIG_UNMATCHED__VS_ETA_ = new TH1F("eff_denom_ETA_BOTH__SEED_ECAL__TRIG_UNMATCHED__VS_ETA_","elec #eta, barrel+endcap, ECAL-driven, non-trig-matched;Electron #eta;",64,-3.2,3.2);
  eff_num_ETA_BOTH__SEED_ECAL__TRIG_UNMATCHED__VS_ETA__HLT_ELEC10_ = new TH1F("eff_num_ETA_BOTH__SEED_ECAL__TRIG_UNMATCHED__VS_ETA__HLT_ELEC10_","Eff vs elec #eta, barrel+endcap, ECAL-driven, non-trig-matched, HLT_ELEC10;Electron #eta;",64,-3.2,3.2);
  eff_num_ETA_BOTH__SEED_ECAL__TRIG_UNMATCHED__VS_ETA__HLT_ELEC15_ = new TH1F("eff_num_ETA_BOTH__SEED_ECAL__TRIG_UNMATCHED__VS_ETA__HLT_ELEC15_","Eff vs elec #eta, barrel+endcap, ECAL-driven, non-trig-matched, HLT_ELEC15;Electron #eta;",64,-3.2,3.2);
  eff_num_ETA_BOTH__SEED_ECAL__TRIG_UNMATCHED__VS_ETA__HLT_PHOT10_ = new TH1F("eff_num_ETA_BOTH__SEED_ECAL__TRIG_UNMATCHED__VS_ETA__HLT_PHOT10_","Eff vs elec #eta, barrel+endcap, ECAL-driven, non-trig-matched, HLT_PHOT10;Electron #eta;",64,-3.2,3.2);
  eff_num_ETA_BOTH__SEED_ECAL__TRIG_UNMATCHED__VS_ETA__HLT_PHOT15_ = new TH1F("eff_num_ETA_BOTH__SEED_ECAL__TRIG_UNMATCHED__VS_ETA__HLT_PHOT15_","Eff vs elec #eta, barrel+endcap, ECAL-driven, non-trig-matched, HLT_PHOT15;Electron #eta;",64,-3.2,3.2);
  eff_num_ETA_BOTH__SEED_ECAL__TRIG_UNMATCHED__VS_ETA__L1_EG5_ = new TH1F("eff_num_ETA_BOTH__SEED_ECAL__TRIG_UNMATCHED__VS_ETA__L1_EG5_","Eff vs elec #eta, barrel+endcap, ECAL-driven, non-trig-matched, L1_EG5;Electron #eta;",64,-3.2,3.2);
  eff_num_ETA_BOTH__SEED_ECAL__TRIG_UNMATCHED__VS_ETA__L1_EG8_ = new TH1F("eff_num_ETA_BOTH__SEED_ECAL__TRIG_UNMATCHED__VS_ETA__L1_EG8_","Eff vs elec #eta, barrel+endcap, ECAL-driven, non-trig-matched, L1_EG8;Electron #eta;",64,-3.2,3.2);

  // barrel+endcap, tracker, match, vs elec SC ET
  eff_denom_ETA_BOTH__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET_ = new TH1F("eff_denom_ETA_BOTH__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET_","elec SC E_{T}, barrel+endcap, tracker-driven, non-trig-matched;Electron SC E_{T};",60,0.,60.);
  eff_num_ETA_BOTH__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_ELEC10_ = new TH1F("eff_num_ETA_BOTH__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_ELEC10_","Eff vs elec SC E_{T}, barrel+endcap, tracker-driven, non-trig-matched, HLT_ELEC10;Electron SC E_{T};",60,0.,60.);
  eff_num_ETA_BOTH__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_ELEC15_ = new TH1F("eff_num_ETA_BOTH__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_ELEC15_","Eff vs elec SC E_{T}, barrel+endcap, tracker-driven, non-trig-matched, HLT_ELEC15;Electron SC E_{T};",60,0.,60.);
  eff_num_ETA_BOTH__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_PHOT10_ = new TH1F("eff_num_ETA_BOTH__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_PHOT10_","Eff vs elec SC E_{T}, barrel+endcap, tracker-driven, non-trig-matched, HLT_PHOT10;Electron SC E_{T};",60,0.,60.);
  eff_num_ETA_BOTH__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_PHOT15_ = new TH1F("eff_num_ETA_BOTH__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_PHOT15_","Eff vs elec SC E_{T}, barrel+endcap, tracker-driven, non-trig-matched, HLT_PHOT15;Electron SC E_{T};",60,0.,60.);
  eff_num_ETA_BOTH__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET__L1_EG5_ = new TH1F("eff_num_ETA_BOTH__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET__L1_EG5_","Eff vs elec SC E_{T}, barrel+endcap, tracker-driven, non-trig-matched, L1_EG5;Electron SC E_{T};",60,0.,60.);
  eff_num_ETA_BOTH__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET__L1_EG8_ = new TH1F("eff_num_ETA_BOTH__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET__L1_EG8_","Eff vs elec SC E_{T}, barrel+endcap, tracker-driven, non-trig-matched, L1_EG8;Electron SC E_{T};",60,0.,60.);

  // barrel+endcap, tracker, match, vs eta
  eff_denom_ETA_BOTH__SEED_TRACKER__TRIG_MATCHED__VS_ETA_ = new TH1F("eff_denom_ETA_BOTH__SEED_TRACKER__TRIG_MATCHED__VS_ETA_","elec #eta, barrel+endcap, tracker-driven, trig-matched;Electron #eta;",64,-3.2,3.2);
  eff_num_ETA_BOTH__SEED_TRACKER__TRIG_MATCHED__VS_ETA__HLT_ELEC10_ = new TH1F("eff_num_ETA_BOTH__SEED_TRACKER__TRIG_MATCHED__VS_ETA__HLT_ELEC10_","Eff vs elec #eta, barrel+endcap, tracker-driven, trig-matched, HLT_ELEC10;Electron #eta;",64,-3.2,3.2);
  eff_num_ETA_BOTH__SEED_TRACKER__TRIG_MATCHED__VS_ETA__HLT_ELEC15_ = new TH1F("eff_num_ETA_BOTH__SEED_TRACKER__TRIG_MATCHED__VS_ETA__HLT_ELEC15_","Eff vs elec #eta, barrel+endcap, tracker-driven, trig-matched, HLT_ELEC15;Electron #eta;",64,-3.2,3.2);
  eff_num_ETA_BOTH__SEED_TRACKER__TRIG_MATCHED__VS_ETA__HLT_PHOT10_ = new TH1F("eff_num_ETA_BOTH__SEED_TRACKER__TRIG_MATCHED__VS_ETA__HLT_PHOT10_","Eff vs elec #eta, barrel+endcap, tracker-driven, trig-matched, HLT_PHOT10;Electron #eta;",64,-3.2,3.2);
  eff_num_ETA_BOTH__SEED_TRACKER__TRIG_MATCHED__VS_ETA__HLT_PHOT15_ = new TH1F("eff_num_ETA_BOTH__SEED_TRACKER__TRIG_MATCHED__VS_ETA__HLT_PHOT15_","Eff vs elec #eta, barrel+endcap, tracker-driven, trig-matched, HLT_PHOT15;Electron #eta;",64,-3.2,3.2);
  eff_num_ETA_BOTH__SEED_TRACKER__TRIG_MATCHED__VS_ETA__L1_EG5_ = new TH1F("eff_num_ETA_BOTH__SEED_TRACKER__TRIG_MATCHED__VS_ETA__L1_EG5_","Eff vs elec #eta, barrel+endcap, tracker-driven, trig-matched, L1_EG5;Electron #eta;",64,-3.2,3.2);
  eff_num_ETA_BOTH__SEED_TRACKER__TRIG_MATCHED__VS_ETA__L1_EG8_ = new TH1F("eff_num_ETA_BOTH__SEED_TRACKER__TRIG_MATCHED__VS_ETA__L1_EG8_","Eff vs elec #eta, barrel+endcap, tracker-driven, trig-matched, L1_EG8;Electron #eta;",64,-3.2,3.2);

  // barrel+endcap, tracker, non-match, vs elec SC ET
  eff_denom_ETA_BOTH__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET_ = new TH1F("eff_denom_ETA_BOTH__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET_","elec SC E_{T}, barrel+endcap, tracker-driven, non-trig-matched;Electron SC E_{T};",60,0.,60.);
  eff_num_ETA_BOTH__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_ELEC10_ = new TH1F("eff_num_ETA_BOTH__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_ELEC10_","Eff vs elec SC E_{T}, barrel+endcap, tracker-driven, non-trig-matched, HLT_ELEC10;Electron SC E_{T};",60,0.,60.);
  eff_num_ETA_BOTH__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_ELEC15_ = new TH1F("eff_num_ETA_BOTH__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_ELEC15_","Eff vs elec SC E_{T}, barrel+endcap, tracker-driven, non-trig-matched, HLT_ELEC15;Electron SC E_{T};",60,0.,60.);
  eff_num_ETA_BOTH__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_PHOT10_ = new TH1F("eff_num_ETA_BOTH__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_PHOT10_","Eff vs elec SC E_{T}, barrel+endcap, tracker-driven, non-trig-matched, HLT_PHOT10;Electron SC E_{T};",60,0.,60.);
  eff_num_ETA_BOTH__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_PHOT15_ = new TH1F("eff_num_ETA_BOTH__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_PHOT15_","Eff vs elec SC E_{T}, barrel+endcap, tracker-driven, non-trig-matched, HLT_PHOT15;Electron SC E_{T};",60,0.,60.);
  eff_num_ETA_BOTH__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET__L1_EG5_ = new TH1F("eff_num_ETA_BOTH__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET__L1_EG5_","Eff vs elec SC E_{T}, barrel+endcap, tracker-driven, non-trig-matched, L1_EG5;Electron SC E_{T};",60,0.,60.);
  eff_num_ETA_BOTH__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET__L1_EG8_ = new TH1F("eff_num_ETA_BOTH__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET__L1_EG8_","Eff vs elec SC E_{T}, barrel+endcap, tracker-driven, non-trig-matched, L1_EG8;Electron SC E_{T};",60,0.,60.);

  // barrel+endcap, tracker, non-match, vs eta
  eff_denom_ETA_BOTH__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA_ = new TH1F("eff_denom_ETA_BOTH__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA_","elec #eta, barrel+endcap, tracker-driven, non-trig-matched;Electron #eta;",64,-3.2,3.2);
  eff_num_ETA_BOTH__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA__HLT_ELEC10_ = new TH1F("eff_num_ETA_BOTH__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA__HLT_ELEC10_","Eff vs elec #eta, barrel+endcap, tracker-driven, non-trig-matched, HLT_ELEC10;Electron #eta;",64,-3.2,3.2);
  eff_num_ETA_BOTH__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA__HLT_ELEC15_ = new TH1F("eff_num_ETA_BOTH__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA__HLT_ELEC15_","Eff vs elec #eta, barrel+endcap, tracker-driven, non-trig-matched, HLT_ELEC15;Electron #eta;",64,-3.2,3.2);
  eff_num_ETA_BOTH__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA__HLT_PHOT10_ = new TH1F("eff_num_ETA_BOTH__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA__HLT_PHOT10_","Eff vs elec #eta, barrel+endcap, tracker-driven, non-trig-matched, HLT_PHOT10;Electron #eta;",64,-3.2,3.2);
  eff_num_ETA_BOTH__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA__HLT_PHOT15_ = new TH1F("eff_num_ETA_BOTH__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA__HLT_PHOT15_","Eff vs elec #eta, barrel+endcap, tracker-driven, non-trig-matched, HLT_PHOT15;Electron #eta;",64,-3.2,3.2);
  eff_num_ETA_BOTH__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA__L1_EG5_ = new TH1F("eff_num_ETA_BOTH__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA__L1_EG5_","Eff vs elec #eta, barrel+endcap, tracker-driven, non-trig-matched, L1_EG5;Electron #eta;",64,-3.2,3.2);
  eff_num_ETA_BOTH__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA__L1_EG8_ = new TH1F("eff_num_ETA_BOTH__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA__L1_EG8_","Eff vs elec #eta, barrel+endcap, tracker-driven, non-trig-matched, L1_EG8;Electron #eta;",64,-3.2,3.2);

  // barrel+endcap, ecal+tracker, match, vs elec SC ET
  eff_denom_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET_ = new TH1F("eff_denom_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET_","elec SC E_{T}, barrel+endcap, ecal+tracker-driven, trig-matched;Electron SC E_{T};",60,0.,60.);
  eff_num_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_ELEC10_ = new TH1F("eff_num_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_ELEC10_","Eff vs elec SC E_{T}, barrel+endcap, ecal+tracker-driven, trig-matched, HLT_ELEC10;Electron SC E_{T};",60,0.,60.);
  eff_num_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_ELEC15_ = new TH1F("eff_num_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_ELEC15_","Eff vs elec SC E_{T}, barrel+endcap, ecal+tracker-driven, trig-matched, HLT_ELEC15;Electron SC E_{T};",60,0.,60.);
  eff_num_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_PHOT10_ = new TH1F("eff_num_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_PHOT10_","Eff vs elec SC E_{T}, barrel+endcap, ecal+tracker-driven, trig-matched, HLT_PHOT10;Electron SC E_{T};",60,0.,60.);
  eff_num_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_PHOT15_ = new TH1F("eff_num_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_PHOT15_","Eff vs elec SC E_{T}, barrel+endcap, ecal+tracker-driven, trig-matched, HLT_PHOT15;Electron SC E_{T};",60,0.,60.);
  eff_num_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__L1_EG5_ = new TH1F("eff_num_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__L1_EG5_","Eff vs elec SC E_{T}, barrel+endcap, ecal+tracker-driven, trig-matched, L1_EG5;Electron SC E_{T};",60,0.,60.);
  eff_num_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__L1_EG8_ = new TH1F("eff_num_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__L1_EG8_","Eff vs elec SC E_{T}, barrel+endcap, ecal+tracker-driven, trig-matched, L1_EG8;Electron SC E_{T};",60,0.,60.);

  // barrel+endcap, ecal+tracker, match, vs eta
  eff_denom_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ETA_ = new TH1F("eff_denom_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ETA_","elec #eta, barrel+endcap, ecal+tracker-driven, trig-matched;Electron #eta;",64,-3.2,3.2);
  eff_num_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ETA__HLT_ELEC10_ = new TH1F("eff_num_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ETA__HLT_ELEC10_","Eff vs elec #eta, barrel+endcap, ecal+tracker-driven, trig-matched, HLT_ELEC10;Electron #eta;",64,-3.2,3.2);
  eff_num_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ETA__HLT_ELEC15_ = new TH1F("eff_num_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ETA__HLT_ELEC15_","Eff vs elec #eta, barrel+endcap, ecal+tracker-driven, trig-matched, HLT_ELEC15;Electron #eta;",64,-3.2,3.2);
  eff_num_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ETA__HLT_PHOT10_ = new TH1F("eff_num_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ETA__HLT_PHOT10_","Eff vs elec #eta, barrel+endcap, ecal+tracker-driven, trig-matched, HLT_PHOT10;Electron #eta;",64,-3.2,3.2);
  eff_num_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ETA__HLT_PHOT15_ = new TH1F("eff_num_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ETA__HLT_PHOT15_","Eff vs elec #eta, barrel+endcap, ecal+tracker-driven, trig-matched, HLT_PHOT15;Electron #eta;",64,-3.2,3.2);
  eff_num_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ETA__L1_EG5_ = new TH1F("eff_num_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ETA__L1_EG5_","Eff vs elec #eta, barrel+endcap, ecal+tracker-driven, trig-matched, L1_EG5;Electron #eta;",64,-3.2,3.2);
  eff_num_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ETA__L1_EG8_ = new TH1F("eff_num_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ETA__L1_EG8_","Eff vs elec #eta, barrel+endcap, ecal+tracker-driven, trig-matched, L1_EG8;Electron #eta;",64,-3.2,3.2);

  // barrel+endcap, ecal+tracker, non-match, vs elec SC ET
  eff_denom_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET_ = new TH1F("eff_denom_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET_","elec SC E_{T}, barrel+endcap, ecal+tracker-driven, non-trig-matched;Electron SC E_{T};",60,0.,60.);
  eff_num_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_ELEC10_ = new TH1F("eff_num_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_ELEC10_","Eff vs elec SC E_{T}, barrel+endcap, ecal+tracker-driven, non-trig-matched, HLT_ELEC10;Electron SC E_{T};",60,0.,60.);
  eff_num_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_ELEC15_ = new TH1F("eff_num_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_ELEC15_","Eff vs elec SC E_{T}, barrel+endcap, ecal+tracker-driven, non-trig-matched, HLT_ELEC15;Electron SC E_{T};",60,0.,60.);
  eff_num_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_PHOT10_ = new TH1F("eff_num_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_PHOT10_","Eff vs elec SC E_{T}, barrel+endcap, ecal+tracker-driven, non-trig-matched, HLT_PHOT10;Electron SC E_{T};",60,0.,60.);
  eff_num_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_PHOT15_ = new TH1F("eff_num_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_PHOT15_","Eff vs elec SC E_{T}, barrel+endcap, ecal+tracker-driven, non-trig-matched, HLT_PHOT15;Electron SC E_{T};",60,0.,60.);
  eff_num_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__L1_EG5_ = new TH1F("eff_num_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__L1_EG5_","Eff vs elec SC E_{T}, barrel+endcap, ecal+tracker-driven, non-trig-matched, L1_EG5;Electron SC E_{T};",60,0.,60.);
  eff_num_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__L1_EG8_ = new TH1F("eff_num_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__L1_EG8_","Eff vs elec SC E_{T}, barrel+endcap, ecal+tracker-driven, non-trig-matched, L1_EG8;Electron SC E_{T};",60,0.,60.);

  // barrel+endcap, ecal+tracker, non-match, vs eta
  eff_denom_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ETA_ = new TH1F("eff_denom_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ETA_","elec #eta, barrel+endcap, ecal+tracker-driven, non-trig-matched;Electron #eta;",64,-3.2,3.2);
  eff_num_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__HLT_ELEC10_ = new TH1F("eff_num_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__HLT_ELEC10_","Eff vs elec #eta, barrel+endcap, ecal+tracker-driven, non-trig-matched, HLT_ELEC10;Electron #eta;",64,-3.2,3.2);
  eff_num_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__HLT_ELEC15_ = new TH1F("eff_num_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__HLT_ELEC15_","Eff vs elec #eta, barrel+endcap, ecal+tracker-driven, non-trig-matched, HLT_ELEC15;Electron #eta;",64,-3.2,3.2);
  eff_num_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__HLT_PHOT10_ = new TH1F("eff_num_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__HLT_PHOT10_","Eff vs elec #eta, barrel+endcap, ecal+tracker-driven, non-trig-matched, HLT_PHOT10;Electron #eta;",64,-3.2,3.2);
  eff_num_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__HLT_PHOT15_ = new TH1F("eff_num_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__HLT_PHOT15_","Eff vs elec #eta, barrel+endcap, ecal+tracker-driven, non-trig-matched, HLT_PHOT15;Electron #eta;",64,-3.2,3.2);
  eff_num_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__L1_EG5_ = new TH1F("eff_num_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__L1_EG5_","Eff vs elec #eta, barrel+endcap, ecal+tracker-driven, non-trig-matched, L1_EG5;Electron #eta;",64,-3.2,3.2);
  eff_num_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__L1_EG8_ = new TH1F("eff_num_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__L1_EG8_","Eff vs elec #eta, barrel+endcap, ecal+tracker-driven, non-trig-matched, L1_EG8;Electron #eta;",64,-3.2,3.2);

}

void EwkDqmTreeHang::WriteHistograms()
{
  //   outputHistoFile->WriteTObject();

  // BARREL
  //                            eff_denom_ETA_BARREL__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET_
  //  outputHistoFile->WriteTObject(eff_denom_ETA_BARREL__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET_);
  outputHistoFile->WriteTObject(eff_num_ETA_BARREL__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_ELEC10_);
  outputHistoFile->WriteTObject(eff_num_ETA_BARREL__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_ELEC15_);
  outputHistoFile->WriteTObject(eff_num_ETA_BARREL__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_PHOT10_);
  outputHistoFile->WriteTObject(eff_num_ETA_BARREL__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_PHOT15_);
  outputHistoFile->WriteTObject(eff_num_ETA_BARREL__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET__L1_EG5_);
  outputHistoFile->WriteTObject(eff_num_ETA_BARREL__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET__L1_EG8_);

  outputHistoFile->WriteTObject(eff_denom_ETA_BARREL__SEED_ECAL__TRIG_MATCHED__VS_ETA_);
  outputHistoFile->WriteTObject(eff_num_ETA_BARREL__SEED_ECAL__TRIG_MATCHED__VS_ETA__HLT_ELEC10_);
  outputHistoFile->WriteTObject(eff_num_ETA_BARREL__SEED_ECAL__TRIG_MATCHED__VS_ETA__HLT_ELEC15_);
  outputHistoFile->WriteTObject(eff_num_ETA_BARREL__SEED_ECAL__TRIG_MATCHED__VS_ETA__HLT_PHOT10_);
  outputHistoFile->WriteTObject(eff_num_ETA_BARREL__SEED_ECAL__TRIG_MATCHED__VS_ETA__HLT_PHOT15_);
  outputHistoFile->WriteTObject(eff_num_ETA_BARREL__SEED_ECAL__TRIG_MATCHED__VS_ETA__L1_EG5_);
  outputHistoFile->WriteTObject(eff_num_ETA_BARREL__SEED_ECAL__TRIG_MATCHED__VS_ETA__L1_EG8_);

  outputHistoFile->WriteTObject(eff_denom_ETA_BARREL__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET_);
  outputHistoFile->WriteTObject(eff_num_ETA_BARREL__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_ELEC10_);
  outputHistoFile->WriteTObject(eff_num_ETA_BARREL__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_ELEC15_);
  outputHistoFile->WriteTObject(eff_num_ETA_BARREL__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_PHOT10_);
  outputHistoFile->WriteTObject(eff_num_ETA_BARREL__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_PHOT15_);
  outputHistoFile->WriteTObject(eff_num_ETA_BARREL__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET__L1_EG5_);
  outputHistoFile->WriteTObject(eff_num_ETA_BARREL__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET__L1_EG8_);

  outputHistoFile->WriteTObject(eff_denom_ETA_BARREL__SEED_ECAL__TRIG_UNMATCHED__VS_ETA_);
  outputHistoFile->WriteTObject(eff_num_ETA_BARREL__SEED_ECAL__TRIG_UNMATCHED__VS_ETA__HLT_ELEC10_);
  outputHistoFile->WriteTObject(eff_num_ETA_BARREL__SEED_ECAL__TRIG_UNMATCHED__VS_ETA__HLT_ELEC15_);
  outputHistoFile->WriteTObject(eff_num_ETA_BARREL__SEED_ECAL__TRIG_UNMATCHED__VS_ETA__HLT_PHOT10_);
  outputHistoFile->WriteTObject(eff_num_ETA_BARREL__SEED_ECAL__TRIG_UNMATCHED__VS_ETA__HLT_PHOT15_);
  outputHistoFile->WriteTObject(eff_num_ETA_BARREL__SEED_ECAL__TRIG_UNMATCHED__VS_ETA__L1_EG5_);
  outputHistoFile->WriteTObject(eff_num_ETA_BARREL__SEED_ECAL__TRIG_UNMATCHED__VS_ETA__L1_EG8_);

  outputHistoFile->WriteTObject(eff_denom_ETA_BARREL__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET_);
  outputHistoFile->WriteTObject(eff_num_ETA_BARREL__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_ELEC10_);
  outputHistoFile->WriteTObject(eff_num_ETA_BARREL__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_ELEC15_);
  outputHistoFile->WriteTObject(eff_num_ETA_BARREL__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_PHOT10_);
  outputHistoFile->WriteTObject(eff_num_ETA_BARREL__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_PHOT15_);
  outputHistoFile->WriteTObject(eff_num_ETA_BARREL__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET__L1_EG5_);
  outputHistoFile->WriteTObject(eff_num_ETA_BARREL__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET__L1_EG8_);

  outputHistoFile->WriteTObject(eff_denom_ETA_BARREL__SEED_TRACKER__TRIG_MATCHED__VS_ETA_);
  outputHistoFile->WriteTObject(eff_num_ETA_BARREL__SEED_TRACKER__TRIG_MATCHED__VS_ETA__HLT_ELEC10_);
  outputHistoFile->WriteTObject(eff_num_ETA_BARREL__SEED_TRACKER__TRIG_MATCHED__VS_ETA__HLT_ELEC15_);
  outputHistoFile->WriteTObject(eff_num_ETA_BARREL__SEED_TRACKER__TRIG_MATCHED__VS_ETA__HLT_PHOT10_);
  outputHistoFile->WriteTObject(eff_num_ETA_BARREL__SEED_TRACKER__TRIG_MATCHED__VS_ETA__HLT_PHOT15_);
  outputHistoFile->WriteTObject(eff_num_ETA_BARREL__SEED_TRACKER__TRIG_MATCHED__VS_ETA__L1_EG5_);
  outputHistoFile->WriteTObject(eff_num_ETA_BARREL__SEED_TRACKER__TRIG_MATCHED__VS_ETA__L1_EG8_);

  outputHistoFile->WriteTObject(eff_denom_ETA_BARREL__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET_);
  outputHistoFile->WriteTObject(eff_num_ETA_BARREL__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_ELEC10_);
  outputHistoFile->WriteTObject(eff_num_ETA_BARREL__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_ELEC15_);
  outputHistoFile->WriteTObject(eff_num_ETA_BARREL__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_PHOT10_);
  outputHistoFile->WriteTObject(eff_num_ETA_BARREL__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_PHOT15_);
  outputHistoFile->WriteTObject(eff_num_ETA_BARREL__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET__L1_EG5_);
  outputHistoFile->WriteTObject(eff_num_ETA_BARREL__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET__L1_EG8_);

  outputHistoFile->WriteTObject(eff_denom_ETA_BARREL__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA_);
  outputHistoFile->WriteTObject(eff_num_ETA_BARREL__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA__HLT_ELEC10_);
  outputHistoFile->WriteTObject(eff_num_ETA_BARREL__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA__HLT_ELEC15_);
  outputHistoFile->WriteTObject(eff_num_ETA_BARREL__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA__HLT_PHOT10_);
  outputHistoFile->WriteTObject(eff_num_ETA_BARREL__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA__HLT_PHOT15_);
  outputHistoFile->WriteTObject(eff_num_ETA_BARREL__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA__L1_EG5_);
  outputHistoFile->WriteTObject(eff_num_ETA_BARREL__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA__L1_EG8_);

  outputHistoFile->WriteTObject(eff_denom_ETA_BARREL__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET_);
  outputHistoFile->WriteTObject(eff_num_ETA_BARREL__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_ELEC10_);
  outputHistoFile->WriteTObject(eff_num_ETA_BARREL__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_ELEC15_);
  outputHistoFile->WriteTObject(eff_num_ETA_BARREL__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_PHOT10_);
  outputHistoFile->WriteTObject(eff_num_ETA_BARREL__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_PHOT15_);
  outputHistoFile->WriteTObject(eff_num_ETA_BARREL__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__L1_EG5_);
  outputHistoFile->WriteTObject(eff_num_ETA_BARREL__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__L1_EG8_);

  outputHistoFile->WriteTObject(eff_denom_ETA_BARREL__SEED_BOTH__TRIG_MATCHED__VS_ETA_);
  outputHistoFile->WriteTObject(eff_num_ETA_BARREL__SEED_BOTH__TRIG_MATCHED__VS_ETA__HLT_ELEC10_);
  outputHistoFile->WriteTObject(eff_num_ETA_BARREL__SEED_BOTH__TRIG_MATCHED__VS_ETA__HLT_ELEC15_);
  outputHistoFile->WriteTObject(eff_num_ETA_BARREL__SEED_BOTH__TRIG_MATCHED__VS_ETA__HLT_PHOT10_);
  outputHistoFile->WriteTObject(eff_num_ETA_BARREL__SEED_BOTH__TRIG_MATCHED__VS_ETA__HLT_PHOT15_);
  outputHistoFile->WriteTObject(eff_num_ETA_BARREL__SEED_BOTH__TRIG_MATCHED__VS_ETA__L1_EG5_);
  outputHistoFile->WriteTObject(eff_num_ETA_BARREL__SEED_BOTH__TRIG_MATCHED__VS_ETA__L1_EG8_);

  outputHistoFile->WriteTObject(eff_denom_ETA_BARREL__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET_);
  outputHistoFile->WriteTObject(eff_num_ETA_BARREL__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_ELEC10_);
  outputHistoFile->WriteTObject(eff_num_ETA_BARREL__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_ELEC15_);
  outputHistoFile->WriteTObject(eff_num_ETA_BARREL__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_PHOT10_);
  outputHistoFile->WriteTObject(eff_num_ETA_BARREL__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_PHOT15_);
  outputHistoFile->WriteTObject(eff_num_ETA_BARREL__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__L1_EG5_);
  outputHistoFile->WriteTObject(eff_num_ETA_BARREL__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__L1_EG8_);

  outputHistoFile->WriteTObject(eff_denom_ETA_BARREL__SEED_BOTH__TRIG_UNMATCHED__VS_ETA_);
  outputHistoFile->WriteTObject(eff_num_ETA_BARREL__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__HLT_ELEC10_);
  outputHistoFile->WriteTObject(eff_num_ETA_BARREL__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__HLT_ELEC15_);
  outputHistoFile->WriteTObject(eff_num_ETA_BARREL__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__HLT_PHOT10_);
  outputHistoFile->WriteTObject(eff_num_ETA_BARREL__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__HLT_PHOT15_);
  outputHistoFile->WriteTObject(eff_num_ETA_BARREL__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__L1_EG5_);
  outputHistoFile->WriteTObject(eff_num_ETA_BARREL__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__L1_EG8_);

  // ENDCAP
  outputHistoFile->WriteTObject(eff_denom_ETA_ENDCAP__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET_);
  outputHistoFile->WriteTObject(eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_ELEC10_);
  outputHistoFile->WriteTObject(eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_ELEC15_);
  outputHistoFile->WriteTObject(eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_PHOT10_);
  outputHistoFile->WriteTObject(eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_PHOT15_);
  outputHistoFile->WriteTObject(eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET__L1_EG5_);
  outputHistoFile->WriteTObject(eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET__L1_EG8_);

  outputHistoFile->WriteTObject(eff_denom_ETA_ENDCAP__SEED_ECAL__TRIG_MATCHED__VS_ETA_);
  outputHistoFile->WriteTObject(eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_MATCHED__VS_ETA__HLT_ELEC10_);
  outputHistoFile->WriteTObject(eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_MATCHED__VS_ETA__HLT_ELEC15_);
  outputHistoFile->WriteTObject(eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_MATCHED__VS_ETA__HLT_PHOT10_);
  outputHistoFile->WriteTObject(eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_MATCHED__VS_ETA__HLT_PHOT15_);
  outputHistoFile->WriteTObject(eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_MATCHED__VS_ETA__L1_EG5_);
  outputHistoFile->WriteTObject(eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_MATCHED__VS_ETA__L1_EG8_);

  outputHistoFile->WriteTObject(eff_denom_ETA_ENDCAP__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET_);
  outputHistoFile->WriteTObject(eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_ELEC10_);
  outputHistoFile->WriteTObject(eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_ELEC15_);
  outputHistoFile->WriteTObject(eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_PHOT10_);
  outputHistoFile->WriteTObject(eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_PHOT15_);
  outputHistoFile->WriteTObject(eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET__L1_EG5_);
  outputHistoFile->WriteTObject(eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET__L1_EG8_);

  outputHistoFile->WriteTObject(eff_denom_ETA_ENDCAP__SEED_ECAL__TRIG_UNMATCHED__VS_ETA_);
  outputHistoFile->WriteTObject(eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_UNMATCHED__VS_ETA__HLT_ELEC10_);
  outputHistoFile->WriteTObject(eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_UNMATCHED__VS_ETA__HLT_ELEC15_);
  outputHistoFile->WriteTObject(eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_UNMATCHED__VS_ETA__HLT_PHOT10_);
  outputHistoFile->WriteTObject(eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_UNMATCHED__VS_ETA__HLT_PHOT15_);
  outputHistoFile->WriteTObject(eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_UNMATCHED__VS_ETA__L1_EG5_);
  outputHistoFile->WriteTObject(eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_UNMATCHED__VS_ETA__L1_EG8_);

  outputHistoFile->WriteTObject(eff_denom_ETA_ENDCAP__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET_);
  outputHistoFile->WriteTObject(eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_ELEC10_);
  outputHistoFile->WriteTObject(eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_ELEC15_);
  outputHistoFile->WriteTObject(eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_PHOT10_);
  outputHistoFile->WriteTObject(eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_PHOT15_);
  outputHistoFile->WriteTObject(eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET__L1_EG5_);
  outputHistoFile->WriteTObject(eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET__L1_EG8_);

  outputHistoFile->WriteTObject(eff_denom_ETA_ENDCAP__SEED_TRACKER__TRIG_MATCHED__VS_ETA_);
  outputHistoFile->WriteTObject(eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_MATCHED__VS_ETA__HLT_ELEC10_);
  outputHistoFile->WriteTObject(eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_MATCHED__VS_ETA__HLT_ELEC15_);
  outputHistoFile->WriteTObject(eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_MATCHED__VS_ETA__HLT_PHOT10_);
  outputHistoFile->WriteTObject(eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_MATCHED__VS_ETA__HLT_PHOT15_);
  outputHistoFile->WriteTObject(eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_MATCHED__VS_ETA__L1_EG5_);
  outputHistoFile->WriteTObject(eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_MATCHED__VS_ETA__L1_EG8_);

  outputHistoFile->WriteTObject(eff_denom_ETA_ENDCAP__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET_);
  outputHistoFile->WriteTObject(eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_ELEC10_);
  outputHistoFile->WriteTObject(eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_ELEC15_);
  outputHistoFile->WriteTObject(eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_PHOT10_);
  outputHistoFile->WriteTObject(eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_PHOT15_);
  outputHistoFile->WriteTObject(eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET__L1_EG5_);
  outputHistoFile->WriteTObject(eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET__L1_EG8_);

  outputHistoFile->WriteTObject(eff_denom_ETA_ENDCAP__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA_);
  outputHistoFile->WriteTObject(eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA__HLT_ELEC10_);
  outputHistoFile->WriteTObject(eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA__HLT_ELEC15_);
  outputHistoFile->WriteTObject(eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA__HLT_PHOT10_);
  outputHistoFile->WriteTObject(eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA__HLT_PHOT15_);
  outputHistoFile->WriteTObject(eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA__L1_EG5_);
  outputHistoFile->WriteTObject(eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA__L1_EG8_);

  outputHistoFile->WriteTObject(eff_denom_ETA_ENDCAP__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET_);
  outputHistoFile->WriteTObject(eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_ELEC10_);
  outputHistoFile->WriteTObject(eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_ELEC15_);
  outputHistoFile->WriteTObject(eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_PHOT10_);
  outputHistoFile->WriteTObject(eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_PHOT15_);
  outputHistoFile->WriteTObject(eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__L1_EG5_);
  outputHistoFile->WriteTObject(eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__L1_EG8_);

  outputHistoFile->WriteTObject(eff_denom_ETA_ENDCAP__SEED_BOTH__TRIG_MATCHED__VS_ETA_);
  outputHistoFile->WriteTObject(eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_MATCHED__VS_ETA__HLT_ELEC10_);
  outputHistoFile->WriteTObject(eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_MATCHED__VS_ETA__HLT_ELEC15_);
  outputHistoFile->WriteTObject(eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_MATCHED__VS_ETA__HLT_PHOT10_);
  outputHistoFile->WriteTObject(eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_MATCHED__VS_ETA__HLT_PHOT15_);
  outputHistoFile->WriteTObject(eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_MATCHED__VS_ETA__L1_EG5_);
  outputHistoFile->WriteTObject(eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_MATCHED__VS_ETA__L1_EG8_);

  outputHistoFile->WriteTObject(eff_denom_ETA_ENDCAP__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET_);
  outputHistoFile->WriteTObject(eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_ELEC10_);
  outputHistoFile->WriteTObject(eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_ELEC15_);
  outputHistoFile->WriteTObject(eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_PHOT10_);
  outputHistoFile->WriteTObject(eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_PHOT15_);
  outputHistoFile->WriteTObject(eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__L1_EG5_);
  outputHistoFile->WriteTObject(eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__L1_EG8_);

  outputHistoFile->WriteTObject(eff_denom_ETA_ENDCAP__SEED_BOTH__TRIG_UNMATCHED__VS_ETA_);
  outputHistoFile->WriteTObject(eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__HLT_ELEC10_);
  outputHistoFile->WriteTObject(eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__HLT_ELEC15_);
  outputHistoFile->WriteTObject(eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__HLT_PHOT10_);
  outputHistoFile->WriteTObject(eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__HLT_PHOT15_);
  outputHistoFile->WriteTObject(eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__L1_EG5_);
  outputHistoFile->WriteTObject(eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__L1_EG8_);

  // BARREL+ENDCAP
  outputHistoFile->WriteTObject(eff_denom_ETA_BOTH__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET_);
  outputHistoFile->WriteTObject(eff_num_ETA_BOTH__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_ELEC10_);
  outputHistoFile->WriteTObject(eff_num_ETA_BOTH__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_ELEC15_);
  outputHistoFile->WriteTObject(eff_num_ETA_BOTH__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_PHOT10_);
  outputHistoFile->WriteTObject(eff_num_ETA_BOTH__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_PHOT15_);
  outputHistoFile->WriteTObject(eff_num_ETA_BOTH__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET__L1_EG5_);
  outputHistoFile->WriteTObject(eff_num_ETA_BOTH__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET__L1_EG8_);

  outputHistoFile->WriteTObject(eff_denom_ETA_BOTH__SEED_ECAL__TRIG_MATCHED__VS_ETA_);
  outputHistoFile->WriteTObject(eff_num_ETA_BOTH__SEED_ECAL__TRIG_MATCHED__VS_ETA__HLT_ELEC10_);
  outputHistoFile->WriteTObject(eff_num_ETA_BOTH__SEED_ECAL__TRIG_MATCHED__VS_ETA__HLT_ELEC15_);
  outputHistoFile->WriteTObject(eff_num_ETA_BOTH__SEED_ECAL__TRIG_MATCHED__VS_ETA__HLT_PHOT10_);
  outputHistoFile->WriteTObject(eff_num_ETA_BOTH__SEED_ECAL__TRIG_MATCHED__VS_ETA__HLT_PHOT15_);
  outputHistoFile->WriteTObject(eff_num_ETA_BOTH__SEED_ECAL__TRIG_MATCHED__VS_ETA__L1_EG5_);
  outputHistoFile->WriteTObject(eff_num_ETA_BOTH__SEED_ECAL__TRIG_MATCHED__VS_ETA__L1_EG8_);

  outputHistoFile->WriteTObject(eff_denom_ETA_BOTH__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET_);
  outputHistoFile->WriteTObject(eff_num_ETA_BOTH__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_ELEC10_);
  outputHistoFile->WriteTObject(eff_num_ETA_BOTH__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_ELEC15_);
  outputHistoFile->WriteTObject(eff_num_ETA_BOTH__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_PHOT10_);
  outputHistoFile->WriteTObject(eff_num_ETA_BOTH__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_PHOT15_);
  outputHistoFile->WriteTObject(eff_num_ETA_BOTH__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET__L1_EG5_);
  outputHistoFile->WriteTObject(eff_num_ETA_BOTH__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET__L1_EG8_);

  outputHistoFile->WriteTObject(eff_denom_ETA_BOTH__SEED_ECAL__TRIG_UNMATCHED__VS_ETA_);
  outputHistoFile->WriteTObject(eff_num_ETA_BOTH__SEED_ECAL__TRIG_UNMATCHED__VS_ETA__HLT_ELEC10_);
  outputHistoFile->WriteTObject(eff_num_ETA_BOTH__SEED_ECAL__TRIG_UNMATCHED__VS_ETA__HLT_ELEC15_);
  outputHistoFile->WriteTObject(eff_num_ETA_BOTH__SEED_ECAL__TRIG_UNMATCHED__VS_ETA__HLT_PHOT10_);
  outputHistoFile->WriteTObject(eff_num_ETA_BOTH__SEED_ECAL__TRIG_UNMATCHED__VS_ETA__HLT_PHOT15_);
  outputHistoFile->WriteTObject(eff_num_ETA_BOTH__SEED_ECAL__TRIG_UNMATCHED__VS_ETA__L1_EG5_);
  outputHistoFile->WriteTObject(eff_num_ETA_BOTH__SEED_ECAL__TRIG_UNMATCHED__VS_ETA__L1_EG8_);

  outputHistoFile->WriteTObject(eff_denom_ETA_BOTH__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET_);
  outputHistoFile->WriteTObject(eff_num_ETA_BOTH__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_ELEC10_);
  outputHistoFile->WriteTObject(eff_num_ETA_BOTH__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_ELEC15_);
  outputHistoFile->WriteTObject(eff_num_ETA_BOTH__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_PHOT10_);
  outputHistoFile->WriteTObject(eff_num_ETA_BOTH__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_PHOT15_);
  outputHistoFile->WriteTObject(eff_num_ETA_BOTH__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET__L1_EG5_);
  outputHistoFile->WriteTObject(eff_num_ETA_BOTH__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET__L1_EG8_);

  outputHistoFile->WriteTObject(eff_denom_ETA_BOTH__SEED_TRACKER__TRIG_MATCHED__VS_ETA_);
  outputHistoFile->WriteTObject(eff_num_ETA_BOTH__SEED_TRACKER__TRIG_MATCHED__VS_ETA__HLT_ELEC10_);
  outputHistoFile->WriteTObject(eff_num_ETA_BOTH__SEED_TRACKER__TRIG_MATCHED__VS_ETA__HLT_ELEC15_);
  outputHistoFile->WriteTObject(eff_num_ETA_BOTH__SEED_TRACKER__TRIG_MATCHED__VS_ETA__HLT_PHOT10_);
  outputHistoFile->WriteTObject(eff_num_ETA_BOTH__SEED_TRACKER__TRIG_MATCHED__VS_ETA__HLT_PHOT15_);
  outputHistoFile->WriteTObject(eff_num_ETA_BOTH__SEED_TRACKER__TRIG_MATCHED__VS_ETA__L1_EG5_);
  outputHistoFile->WriteTObject(eff_num_ETA_BOTH__SEED_TRACKER__TRIG_MATCHED__VS_ETA__L1_EG8_);

  outputHistoFile->WriteTObject(eff_denom_ETA_BOTH__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET_);
  outputHistoFile->WriteTObject(eff_num_ETA_BOTH__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_ELEC10_);
  outputHistoFile->WriteTObject(eff_num_ETA_BOTH__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_ELEC15_);
  outputHistoFile->WriteTObject(eff_num_ETA_BOTH__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_PHOT10_);
  outputHistoFile->WriteTObject(eff_num_ETA_BOTH__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_PHOT15_);
  outputHistoFile->WriteTObject(eff_num_ETA_BOTH__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET__L1_EG5_);
  outputHistoFile->WriteTObject(eff_num_ETA_BOTH__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET__L1_EG8_);

  outputHistoFile->WriteTObject(eff_denom_ETA_BOTH__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA_);
  outputHistoFile->WriteTObject(eff_num_ETA_BOTH__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA__HLT_ELEC10_);
  outputHistoFile->WriteTObject(eff_num_ETA_BOTH__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA__HLT_ELEC15_);
  outputHistoFile->WriteTObject(eff_num_ETA_BOTH__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA__HLT_PHOT10_);
  outputHistoFile->WriteTObject(eff_num_ETA_BOTH__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA__HLT_PHOT15_);
  outputHistoFile->WriteTObject(eff_num_ETA_BOTH__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA__L1_EG5_);
  outputHistoFile->WriteTObject(eff_num_ETA_BOTH__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA__L1_EG8_);

  outputHistoFile->WriteTObject(eff_denom_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET_);
  outputHistoFile->WriteTObject(eff_num_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_ELEC10_);
  outputHistoFile->WriteTObject(eff_num_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_ELEC15_);
  outputHistoFile->WriteTObject(eff_num_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_PHOT10_);
  outputHistoFile->WriteTObject(eff_num_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_PHOT15_);
  outputHistoFile->WriteTObject(eff_num_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__L1_EG5_);
  outputHistoFile->WriteTObject(eff_num_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__L1_EG8_);

  outputHistoFile->WriteTObject(eff_denom_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ETA_);
  outputHistoFile->WriteTObject(eff_num_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ETA__HLT_ELEC10_);
  outputHistoFile->WriteTObject(eff_num_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ETA__HLT_ELEC15_);
  outputHistoFile->WriteTObject(eff_num_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ETA__HLT_PHOT10_);
  outputHistoFile->WriteTObject(eff_num_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ETA__HLT_PHOT15_);
  outputHistoFile->WriteTObject(eff_num_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ETA__L1_EG5_);
  outputHistoFile->WriteTObject(eff_num_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ETA__L1_EG8_);

  outputHistoFile->WriteTObject(eff_denom_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET_);
  outputHistoFile->WriteTObject(eff_num_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_ELEC10_);
  outputHistoFile->WriteTObject(eff_num_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_ELEC15_);
  outputHistoFile->WriteTObject(eff_num_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_PHOT10_);
  outputHistoFile->WriteTObject(eff_num_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_PHOT15_);
  outputHistoFile->WriteTObject(eff_num_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__L1_EG5_);
  outputHistoFile->WriteTObject(eff_num_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__L1_EG8_);

  outputHistoFile->WriteTObject(eff_denom_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ETA_);
  outputHistoFile->WriteTObject(eff_num_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__HLT_ELEC10_);
  outputHistoFile->WriteTObject(eff_num_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__HLT_ELEC15_);
  outputHistoFile->WriteTObject(eff_num_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__HLT_PHOT10_);
  outputHistoFile->WriteTObject(eff_num_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__HLT_PHOT15_);
  outputHistoFile->WriteTObject(eff_num_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__L1_EG5_);
  outputHistoFile->WriteTObject(eff_num_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__L1_EG8_);

}

void EwkDqmTreeHang::DeleteHistograms()
{

  // initializing histograms

/* 		  enum {ETA_BARREL, ETA_ENDCAP, ETA_BOTH, ETA_LAST}; */
/* 		  enum {SEED_ECAL, SEED_TRACKER, SEED_BOTH, SEED_LAST}; */
/* 		  enum {TRIG_MATCHED, TRIG_UNMATCHED, TRIG_MATCH_LAST}; */
/* 		  enum {VS_ELEC_SC_ET, VS_ETA, VS_LAST}; */
/* 		  enum {HLT_ELEC10, HLT_ELEC15, HLT_PHOT10, HLT_PHOT15, L1_EG5, L1_EG8, TRIG_OF_INTEREST_LAST}; */

  // barrel, ecal, match, vs elec SC ET
  delete eff_denom_ETA_BARREL__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET_;
  delete eff_num_ETA_BARREL__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_ELEC10_;
  delete eff_num_ETA_BARREL__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_ELEC15_;
  delete eff_num_ETA_BARREL__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_PHOT10_;
  delete eff_num_ETA_BARREL__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_PHOT15_;
  delete eff_num_ETA_BARREL__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET__L1_EG5_;
  delete eff_num_ETA_BARREL__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET__L1_EG8_;
  
  // barrel, ecal, match, vs eta
  delete eff_denom_ETA_BARREL__SEED_ECAL__TRIG_MATCHED__VS_ETA_;
  delete eff_num_ETA_BARREL__SEED_ECAL__TRIG_MATCHED__VS_ETA__HLT_ELEC10_;
  delete eff_num_ETA_BARREL__SEED_ECAL__TRIG_MATCHED__VS_ETA__HLT_ELEC15_;
  delete eff_num_ETA_BARREL__SEED_ECAL__TRIG_MATCHED__VS_ETA__HLT_PHOT10_;
  delete eff_num_ETA_BARREL__SEED_ECAL__TRIG_MATCHED__VS_ETA__HLT_PHOT15_;
  delete eff_num_ETA_BARREL__SEED_ECAL__TRIG_MATCHED__VS_ETA__L1_EG5_;
  delete eff_num_ETA_BARREL__SEED_ECAL__TRIG_MATCHED__VS_ETA__L1_EG8_;
  
  // barrel, ecal, non-match, vs elec SC ET
  delete eff_denom_ETA_BARREL__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET_;
  delete eff_num_ETA_BARREL__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_ELEC10_;
  delete eff_num_ETA_BARREL__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_ELEC15_;
  delete eff_num_ETA_BARREL__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_PHOT10_;
  delete eff_num_ETA_BARREL__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_PHOT15_;
  delete eff_num_ETA_BARREL__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET__L1_EG5_;
  delete eff_num_ETA_BARREL__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET__L1_EG8_;
  
  // barrel, ecal, non-match, vs eta
  delete eff_denom_ETA_BARREL__SEED_ECAL__TRIG_UNMATCHED__VS_ETA_;
  delete eff_num_ETA_BARREL__SEED_ECAL__TRIG_UNMATCHED__VS_ETA__HLT_ELEC10_;
  delete eff_num_ETA_BARREL__SEED_ECAL__TRIG_UNMATCHED__VS_ETA__HLT_ELEC15_;
  delete eff_num_ETA_BARREL__SEED_ECAL__TRIG_UNMATCHED__VS_ETA__HLT_PHOT10_;
  delete eff_num_ETA_BARREL__SEED_ECAL__TRIG_UNMATCHED__VS_ETA__HLT_PHOT15_;
  delete eff_num_ETA_BARREL__SEED_ECAL__TRIG_UNMATCHED__VS_ETA__L1_EG5_;
  delete eff_num_ETA_BARREL__SEED_ECAL__TRIG_UNMATCHED__VS_ETA__L1_EG8_;
  
  // barrel, tracker, match, vs elec SC ET
  delete eff_denom_ETA_BARREL__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET_;
  delete eff_num_ETA_BARREL__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_ELEC10_;
  delete eff_num_ETA_BARREL__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_ELEC15_;
  delete eff_num_ETA_BARREL__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_PHOT10_;
  delete eff_num_ETA_BARREL__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_PHOT15_;
  delete eff_num_ETA_BARREL__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET__L1_EG5_;
  delete eff_num_ETA_BARREL__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET__L1_EG8_;

  // barrel, tracker, match, vs eta
  delete eff_denom_ETA_BARREL__SEED_TRACKER__TRIG_MATCHED__VS_ETA_;
  delete eff_num_ETA_BARREL__SEED_TRACKER__TRIG_MATCHED__VS_ETA__HLT_ELEC10_;
  delete eff_num_ETA_BARREL__SEED_TRACKER__TRIG_MATCHED__VS_ETA__HLT_ELEC15_;
  delete eff_num_ETA_BARREL__SEED_TRACKER__TRIG_MATCHED__VS_ETA__HLT_PHOT10_;
  delete eff_num_ETA_BARREL__SEED_TRACKER__TRIG_MATCHED__VS_ETA__HLT_PHOT15_;
  delete eff_num_ETA_BARREL__SEED_TRACKER__TRIG_MATCHED__VS_ETA__L1_EG5_;
  delete eff_num_ETA_BARREL__SEED_TRACKER__TRIG_MATCHED__VS_ETA__L1_EG8_;

  // barrel, tracker, non-match, vs elec SC ET
  delete eff_denom_ETA_BARREL__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET_;
  delete eff_num_ETA_BARREL__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_ELEC10_;
  delete eff_num_ETA_BARREL__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_ELEC15_;
  delete eff_num_ETA_BARREL__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_PHOT10_;
  delete eff_num_ETA_BARREL__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_PHOT15_;
  delete eff_num_ETA_BARREL__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET__L1_EG5_;
  delete eff_num_ETA_BARREL__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET__L1_EG8_;

  // barrel, tracker, non-match, vs eta
  delete eff_denom_ETA_BARREL__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA_;
  delete eff_num_ETA_BARREL__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA__HLT_ELEC10_;
  delete eff_num_ETA_BARREL__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA__HLT_ELEC15_;
  delete eff_num_ETA_BARREL__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA__HLT_PHOT10_;
  delete eff_num_ETA_BARREL__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA__HLT_PHOT15_;
  delete eff_num_ETA_BARREL__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA__L1_EG5_;
  delete eff_num_ETA_BARREL__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA__L1_EG8_;

  // barrel, ecal+tracker, match, vs elec SC ET
  delete eff_denom_ETA_BARREL__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET_;
  delete eff_num_ETA_BARREL__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_ELEC10_;
  delete eff_num_ETA_BARREL__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_ELEC15_;
  delete eff_num_ETA_BARREL__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_PHOT10_;
  delete eff_num_ETA_BARREL__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_PHOT15_;
  delete eff_num_ETA_BARREL__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__L1_EG5_;
  delete eff_num_ETA_BARREL__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__L1_EG8_;

  // barrel, ecal+tracker, match, vs eta
  delete eff_denom_ETA_BARREL__SEED_BOTH__TRIG_MATCHED__VS_ETA_;
  delete eff_num_ETA_BARREL__SEED_BOTH__TRIG_MATCHED__VS_ETA__HLT_ELEC10_;
  delete eff_num_ETA_BARREL__SEED_BOTH__TRIG_MATCHED__VS_ETA__HLT_ELEC15_;
  delete eff_num_ETA_BARREL__SEED_BOTH__TRIG_MATCHED__VS_ETA__HLT_PHOT10_;
  delete eff_num_ETA_BARREL__SEED_BOTH__TRIG_MATCHED__VS_ETA__HLT_PHOT15_;
  delete eff_num_ETA_BARREL__SEED_BOTH__TRIG_MATCHED__VS_ETA__L1_EG5_;
  delete eff_num_ETA_BARREL__SEED_BOTH__TRIG_MATCHED__VS_ETA__L1_EG8_;

  // barrel, ecal+tracker, non-match, vs elec SC ET
  delete eff_denom_ETA_BARREL__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET_;
  delete eff_num_ETA_BARREL__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_ELEC10_;
  delete eff_num_ETA_BARREL__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_ELEC15_;
  delete eff_num_ETA_BARREL__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_PHOT10_;
  delete eff_num_ETA_BARREL__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_PHOT15_;
  delete eff_num_ETA_BARREL__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__L1_EG5_;
  delete eff_num_ETA_BARREL__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__L1_EG8_;

  // barrel, ecal+tracker, non-match, vs eta
  delete eff_denom_ETA_BARREL__SEED_BOTH__TRIG_UNMATCHED__VS_ETA_;
  delete eff_num_ETA_BARREL__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__HLT_ELEC10_;
  delete eff_num_ETA_BARREL__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__HLT_ELEC15_;
  delete eff_num_ETA_BARREL__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__HLT_PHOT10_;
  delete eff_num_ETA_BARREL__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__HLT_PHOT15_;
  delete eff_num_ETA_BARREL__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__L1_EG5_;
  delete eff_num_ETA_BARREL__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__L1_EG8_;

  // endcap, ecal, match, vs elec SC ET
  delete eff_denom_ETA_ENDCAP__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET_;
  delete eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_ELEC10_;
  delete eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_ELEC15_;
  delete eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_PHOT10_;
  delete eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_PHOT15_;
  delete eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET__L1_EG5_;
  delete eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET__L1_EG8_;

  // endcap, ecal, match, vs eta
  delete eff_denom_ETA_ENDCAP__SEED_ECAL__TRIG_MATCHED__VS_ETA_;
  delete eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_MATCHED__VS_ETA__HLT_ELEC10_;
  delete eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_MATCHED__VS_ETA__HLT_ELEC15_;
  delete eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_MATCHED__VS_ETA__HLT_PHOT10_;
  delete eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_MATCHED__VS_ETA__HLT_PHOT15_;
  delete eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_MATCHED__VS_ETA__L1_EG5_;
  delete eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_MATCHED__VS_ETA__L1_EG8_;

  // endcap, ecal, non-match, vs elec SC ET
  delete eff_denom_ETA_ENDCAP__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET_;
  delete eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_ELEC10_;
  delete eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_ELEC15_;
  delete eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_PHOT10_;
  delete eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_PHOT15_;
  delete eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET__L1_EG5_;
  delete eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET__L1_EG8_;

  // endcap, ecal, non-match, vs eta
  delete eff_denom_ETA_ENDCAP__SEED_ECAL__TRIG_UNMATCHED__VS_ETA_;
  delete eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_UNMATCHED__VS_ETA__HLT_ELEC10_;
  delete eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_UNMATCHED__VS_ETA__HLT_ELEC15_;
  delete eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_UNMATCHED__VS_ETA__HLT_PHOT10_;
  delete eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_UNMATCHED__VS_ETA__HLT_PHOT15_;
  delete eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_UNMATCHED__VS_ETA__L1_EG5_;
  delete eff_num_ETA_ENDCAP__SEED_ECAL__TRIG_UNMATCHED__VS_ETA__L1_EG8_;

  // endcap, tracker, match, vs elec SC ET
  delete eff_denom_ETA_ENDCAP__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET_;
  delete eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_ELEC10_;
  delete eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_ELEC15_;
  delete eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_PHOT10_;
  delete eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_PHOT15_;
  delete eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET__L1_EG5_;
  delete eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET__L1_EG8_;

  // endcap, tracker, match, vs eta
  delete eff_denom_ETA_ENDCAP__SEED_TRACKER__TRIG_MATCHED__VS_ETA_;
  delete eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_MATCHED__VS_ETA__HLT_ELEC10_;
  delete eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_MATCHED__VS_ETA__HLT_ELEC15_;
  delete eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_MATCHED__VS_ETA__HLT_PHOT10_;
  delete eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_MATCHED__VS_ETA__HLT_PHOT15_;
  delete eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_MATCHED__VS_ETA__L1_EG5_;
  delete eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_MATCHED__VS_ETA__L1_EG8_;

  // endcap, tracker, non-match, vs elec SC ET
  delete eff_denom_ETA_ENDCAP__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET_;
  delete eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_ELEC10_;
  delete eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_ELEC15_;
  delete eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_PHOT10_;
  delete eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_PHOT15_;
  delete eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET__L1_EG5_;
  delete eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET__L1_EG8_;

  // endcap, tracker, non-match, vs eta
  delete eff_denom_ETA_ENDCAP__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA_;
  delete eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA__HLT_ELEC10_;
  delete eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA__HLT_ELEC15_;
  delete eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA__HLT_PHOT10_;
  delete eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA__HLT_PHOT15_;
  delete eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA__L1_EG5_;
  delete eff_num_ETA_ENDCAP__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA__L1_EG8_;

  // endcap, ecal+tracker, match, vs elec SC ET
  delete eff_denom_ETA_ENDCAP__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET_;
  delete eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_ELEC10_;
  delete eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_ELEC15_;
  delete eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_PHOT10_;
  delete eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_PHOT15_;
  delete eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__L1_EG5_;
  delete eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__L1_EG8_;

  // endcap, ecal+tracker, match, vs eta
  delete eff_denom_ETA_ENDCAP__SEED_BOTH__TRIG_MATCHED__VS_ETA_;
  delete eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_MATCHED__VS_ETA__HLT_ELEC10_;
  delete eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_MATCHED__VS_ETA__HLT_ELEC15_;
  delete eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_MATCHED__VS_ETA__HLT_PHOT10_;
  delete eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_MATCHED__VS_ETA__HLT_PHOT15_;
  delete eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_MATCHED__VS_ETA__L1_EG5_;
  delete eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_MATCHED__VS_ETA__L1_EG8_;

  // endcap, ecal+tracker, non-match, vs elec SC ET
  delete eff_denom_ETA_ENDCAP__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET_;
  delete eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_ELEC10_;
  delete eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_ELEC15_;
  delete eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_PHOT10_;
  delete eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_PHOT15_;
  delete eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__L1_EG5_;
  delete eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__L1_EG8_;

  // endcap, ecal+tracker, non-match, vs eta
  delete eff_denom_ETA_ENDCAP__SEED_BOTH__TRIG_UNMATCHED__VS_ETA_;
  delete eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__HLT_ELEC10_;
  delete eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__HLT_ELEC15_;
  delete eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__HLT_PHOT10_;
  delete eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__HLT_PHOT15_;
  delete eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__L1_EG5_;
  delete eff_num_ETA_ENDCAP__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__L1_EG8_;

  // barrel+endcap, ecal, match, vs elec SC ET
  delete eff_denom_ETA_BOTH__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET_;
  delete eff_num_ETA_BOTH__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_ELEC10_;
  delete eff_num_ETA_BOTH__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_ELEC15_;
  delete eff_num_ETA_BOTH__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_PHOT10_;
  delete eff_num_ETA_BOTH__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_PHOT15_;
  delete eff_num_ETA_BOTH__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET__L1_EG5_;
  delete eff_num_ETA_BOTH__SEED_ECAL__TRIG_MATCHED__VS_ELEC_SC_ET__L1_EG8_;

  // barrel+endcap, ecal, match, vs eta
  delete eff_denom_ETA_BOTH__SEED_ECAL__TRIG_MATCHED__VS_ETA_;
  delete eff_num_ETA_BOTH__SEED_ECAL__TRIG_MATCHED__VS_ETA__HLT_ELEC10_;
  delete eff_num_ETA_BOTH__SEED_ECAL__TRIG_MATCHED__VS_ETA__HLT_ELEC15_;
  delete eff_num_ETA_BOTH__SEED_ECAL__TRIG_MATCHED__VS_ETA__HLT_PHOT10_;
  delete eff_num_ETA_BOTH__SEED_ECAL__TRIG_MATCHED__VS_ETA__HLT_PHOT15_;
  delete eff_num_ETA_BOTH__SEED_ECAL__TRIG_MATCHED__VS_ETA__L1_EG5_;
  delete eff_num_ETA_BOTH__SEED_ECAL__TRIG_MATCHED__VS_ETA__L1_EG8_;

  // barrel+endcap, ecal, non-match, vs elec SC ET
  delete eff_denom_ETA_BOTH__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET_;
  delete eff_num_ETA_BOTH__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_ELEC10_;
  delete eff_num_ETA_BOTH__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_ELEC15_;
  delete eff_num_ETA_BOTH__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_PHOT10_;
  delete eff_num_ETA_BOTH__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_PHOT15_;
  delete eff_num_ETA_BOTH__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET__L1_EG5_;
  delete eff_num_ETA_BOTH__SEED_ECAL__TRIG_UNMATCHED__VS_ELEC_SC_ET__L1_EG8_;

  // barrel+endcap, ecal, non-match, vs eta
  delete eff_denom_ETA_BOTH__SEED_ECAL__TRIG_UNMATCHED__VS_ETA_;
  delete eff_num_ETA_BOTH__SEED_ECAL__TRIG_UNMATCHED__VS_ETA__HLT_ELEC10_;
  delete eff_num_ETA_BOTH__SEED_ECAL__TRIG_UNMATCHED__VS_ETA__HLT_ELEC15_;
  delete eff_num_ETA_BOTH__SEED_ECAL__TRIG_UNMATCHED__VS_ETA__HLT_PHOT10_;
  delete eff_num_ETA_BOTH__SEED_ECAL__TRIG_UNMATCHED__VS_ETA__HLT_PHOT15_;
  delete eff_num_ETA_BOTH__SEED_ECAL__TRIG_UNMATCHED__VS_ETA__L1_EG5_;
  delete eff_num_ETA_BOTH__SEED_ECAL__TRIG_UNMATCHED__VS_ETA__L1_EG8_;

  // barrel+endcap, tracker, match, vs elec SC ET
  delete eff_denom_ETA_BOTH__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET_;
  delete eff_num_ETA_BOTH__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_ELEC10_;
  delete eff_num_ETA_BOTH__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_ELEC15_;
  delete eff_num_ETA_BOTH__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_PHOT10_;
  delete eff_num_ETA_BOTH__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_PHOT15_;
  delete eff_num_ETA_BOTH__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET__L1_EG5_;
  delete eff_num_ETA_BOTH__SEED_TRACKER__TRIG_MATCHED__VS_ELEC_SC_ET__L1_EG8_;

  // barrel+endcap, tracker, match, vs eta
  delete eff_denom_ETA_BOTH__SEED_TRACKER__TRIG_MATCHED__VS_ETA_;
  delete eff_num_ETA_BOTH__SEED_TRACKER__TRIG_MATCHED__VS_ETA__HLT_ELEC10_;
  delete eff_num_ETA_BOTH__SEED_TRACKER__TRIG_MATCHED__VS_ETA__HLT_ELEC15_;
  delete eff_num_ETA_BOTH__SEED_TRACKER__TRIG_MATCHED__VS_ETA__HLT_PHOT10_;
  delete eff_num_ETA_BOTH__SEED_TRACKER__TRIG_MATCHED__VS_ETA__HLT_PHOT15_;
  delete eff_num_ETA_BOTH__SEED_TRACKER__TRIG_MATCHED__VS_ETA__L1_EG5_;
  delete eff_num_ETA_BOTH__SEED_TRACKER__TRIG_MATCHED__VS_ETA__L1_EG8_;

  // barrel+endcap, tracker, non-match, vs elec SC ET
  delete eff_denom_ETA_BOTH__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET_;
  delete eff_num_ETA_BOTH__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_ELEC10_;
  delete eff_num_ETA_BOTH__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_ELEC15_;
  delete eff_num_ETA_BOTH__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_PHOT10_;
  delete eff_num_ETA_BOTH__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_PHOT15_;
  delete eff_num_ETA_BOTH__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET__L1_EG5_;
  delete eff_num_ETA_BOTH__SEED_TRACKER__TRIG_UNMATCHED__VS_ELEC_SC_ET__L1_EG8_;

  // barrel+endcap, tracker, non-match, vs eta
  delete eff_denom_ETA_BOTH__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA_;
  delete eff_num_ETA_BOTH__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA__HLT_ELEC10_;
  delete eff_num_ETA_BOTH__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA__HLT_ELEC15_;
  delete eff_num_ETA_BOTH__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA__HLT_PHOT10_;
  delete eff_num_ETA_BOTH__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA__HLT_PHOT15_;
  delete eff_num_ETA_BOTH__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA__L1_EG5_;
  delete eff_num_ETA_BOTH__SEED_TRACKER__TRIG_UNMATCHED__VS_ETA__L1_EG8_;

  // barrel+endcap, ecal+tracker, match, vs elec SC ET
  delete eff_denom_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET_;
  delete eff_num_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_ELEC10_;
  delete eff_num_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_ELEC15_;
  delete eff_num_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_PHOT10_;
  delete eff_num_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__HLT_PHOT15_;
  delete eff_num_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__L1_EG5_;
  delete eff_num_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ELEC_SC_ET__L1_EG8_;
  
  // barrel+endcap, ecal+tracker, match, vs eta
  delete eff_denom_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ETA_;
  delete eff_num_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ETA__HLT_ELEC10_;
  delete eff_num_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ETA__HLT_ELEC15_;
  delete eff_num_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ETA__HLT_PHOT10_;
  delete eff_num_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ETA__HLT_PHOT15_;
  delete eff_num_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ETA__L1_EG5_;
  delete eff_num_ETA_BOTH__SEED_BOTH__TRIG_MATCHED__VS_ETA__L1_EG8_;

  // barrel+endcap, ecal+tracker, non-match, vs elec SC ET
  delete eff_denom_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET_;
  delete eff_num_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_ELEC10_;
  delete eff_num_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_ELEC15_;
  delete eff_num_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_PHOT10_;
  delete eff_num_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__HLT_PHOT15_;
  delete eff_num_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__L1_EG5_;
  delete eff_num_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ELEC_SC_ET__L1_EG8_;

  // barrel+endcap, ecal+tracker, non-match, vs eta
  delete eff_denom_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ETA_;
  delete eff_num_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__HLT_ELEC10_;
  delete eff_num_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__HLT_ELEC15_;
  delete eff_num_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__HLT_PHOT10_;
  delete eff_num_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__HLT_PHOT15_;
  delete eff_num_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__L1_EG5_;
  delete eff_num_ETA_BOTH__SEED_BOTH__TRIG_UNMATCHED__VS_ETA__L1_EG8_;

}

// void EwkDqmTreeHang::DrawEfficiency(TString histoName, TString denomName, TString imageName, Double_t lowBins[], const int binArraySize)
// {
//   scaleFactor = 1;

//   //  Double_t lowBins[17] = {0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 24, 28, 32, 40, 48, 60};
//   //  Double_t dummyBins[17] = {0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 24, 28, 32, 40, 48, 60};
//   //  Int_t nBins = sizeof(*lowBins)/sizeof(Double_t);
//   //   Int_t nBins = sizeof(dummyBins)/sizeof(Double_t);
//   //   std::cout << "Size of array is " << sizeof(dummyBins) << ", size of type Double_t is " << sizeof(Double_t) << std::endl;
//   //  Int_t nBins = sizeof(lowBins)/sizeof(Double_t);
//   Int_t nBins = (Int_t) binArraySize - 1;
//   //  std::cout << "Size of array is " << sizeof(lowBins) << ", size of type Double_t is " << sizeof(Double_t) << std::endl;
//   //  std::cout << "Number of elements in array is " << nBins << ", number of bins to rebin to is " << nBins-1 << std::endl;

//   TH1F * hist1 = dynamic_cast<TH1F*>(inFile->Get("ewkElecStandaloneDQM/"+histoName));
//   //  TH1F * histNew1 = hist1->Rebin(16,"histNew1",lowBins);
//   TH1F * histNew1 = hist1->Rebin(nBins,"histNew1",lowBins);
//   TH1F * hist2 = dynamic_cast<TH1F*>(referenceFile->Get("ewkElecStandaloneDQM/"+histoName));
//   //  TH1F * histNew2 = hist2->Rebin(16,"histNew2",lowBins);
//   TH1F * histNew2 = hist2->Rebin(nBins,"histNew2",lowBins);

//   //  TH1F * denom1 =  dynamic_cast<TH1F*>(inFile->Get("ewkElecStandaloneDQM/TRG_DENOMINATOR_EFF"));
//   TH1F * denom1 =  dynamic_cast<TH1F*>(inFile->Get("ewkElecStandaloneDQM/"+denomName));
//   //  denom1->Rebin(2);
//   //  TH1F * denomNew1 = denom1->Rebin(16,"denomNew1",lowBins);
//   TH1F * denomNew1 = denom1->Rebin(nBins,"denomNew1",lowBins);
//   //  TH1F * denom2 =  dynamic_cast<TH1F*>(referenceFile->Get("ewkElecStandaloneDQM/TRG_DENOMINATOR_EFF"));
//   TH1F * denom2 =  dynamic_cast<TH1F*>(referenceFile->Get("ewkElecStandaloneDQM/"+denomName));
//   //  denom2->Rebin(2);
//   //  TH1F * denomNew2 = denom2->Rebin(16,"denomNew2",lowBins);
//   TH1F * denomNew2 = denom2->Rebin(nBins,"denomNew2",lowBins);


//   //STACK
//   THStack * s_1 = new THStack( "s_1", histNew1->GetTitle() );


//   histNew1->SetMarkerColor(1);
//   histNew1->SetMarkerStyle(8);
//   histNew1->SetMarkerSize(0.8);
//   //  histNew1->Rebin(2);  // REBIN
//   //  denomNew1->Rebin(2); // REBIN
//   histNew1->Sumw2();
//   denomNew1->Sumw2();
//   //  histNew1->Divide(denomNew1);
//   TGraphAsymmErrors * gr = new TGraphAsymmErrors();
//   gr->BayesDivide(histNew1, denomNew1,"w");
//   gr->SetMarkerColor(1);
//   gr->SetMarkerStyle(8);
//   gr->SetMarkerSize(0.8);

//   //  s_1->Add(histNew1);

//   histNew2->SetLineColor(4);
//   histNew2->SetLineWidth(2);
//   //  histNew2->Rebin(2);  // REBIN
//   //  denomNew2->Rebin(2); // REBIN
//   histNew2->Divide(denomNew2);
//   s_1->Add(histNew2);
//   //  s_1->GetXaxis()->SetAxisTitle(hist1->GetXaxis()->GetAxisTitle);

//   s_1->Draw("nostack");
//   TAxis * histXAxis = (TAxis*) histNew1->GetXaxis();
//   TAxis * stackXAxis = (TAxis*) s_1->GetXaxis();
//   stackXAxis->SetTitle(histXAxis->GetTitle());

//   s_1->Draw("nostack");
//   gr->Draw("zp");

//   SavePlot(imageName);
// }

/*TH1F * */
void
//EwkDqmTreeHang::DrawEfficiency(TString histoName, TString denomName, TString imageName, Double_t lowBins[], const int binArraySize)
//EwkDqmTreeHang::DrawEfficiency(TH1F * numHist, TH1F * denomHist, TH1F * effHist, /*TString imageName,*/ Double_t lowBins[], const int binArraySize)
EwkDqmTreeHang::DrawEfficiency(TH1F * numHist, TH1F * denomHist, TString imageName, Double_t lowBins[], const int binArraySize)
{
  //  scaleFactor = 1;

  Int_t nBins = (Int_t) binArraySize - 1;

  //  TH1F * hist1 = dynamic_cast<TH1F*>(inFile->Get("ewkElecStandaloneDQM/"+histoName));
  //  TH1F * hist1 = dynamic_cast<TH1F*>(inFile->Get(histoName));
  TH1F * hist = (TH1F*) numHist->Clone("hist");
  TH1F * histNew = (TH1F*) hist->Rebin(nBins,"histNew",lowBins);
  //   TH1F * hist2 = dynamic_cast<TH1F*>(referenceFile->Get("ewkElecStandaloneDQM/"+histoName));
  //   TH1F * histNew2 = hist2->Rebin(nBins,"histNew2",lowBins);

  //  TH1F * denom1 =  dynamic_cast<TH1F*>(inFile->Get("ewkElecStandaloneDQM/"+denomName));
  //  TH1F * denom1 =  dynamic_cast<TH1F*>(inFile->Get(denomName));
  TH1F * denom = (TH1F*) denomHist->Clone("denom");
  TH1F * denomNew = (TH1F*) denom->Rebin(nBins,"denomNew",lowBins);
  //   TH1F * denom2 =  dynamic_cast<TH1F*>(referenceFile->Get("ewkElecStandaloneDQM/"+denomName));
  //   TH1F * denomNew2 = denom2->Rebin(nBins,"denomNew2",lowBins);


  //STACK
  //  THStack * s_1 = new THStack( "s_1", histNew->GetTitle() );


  histNew->SetMarkerColor(1);
  histNew->SetMarkerStyle(8);
  histNew->SetMarkerSize(0.8);
  //  histNew->Rebin(2);  // REBIN
  //  denomNew->Rebin(2); // REBIN
  histNew->Sumw2();
  denomNew->Sumw2();
  //  histNew->Divide(denomNew1);
  TGraphAsymmErrors * gr = new TGraphAsymmErrors();
  gr->BayesDivide(histNew, denomNew,"w");
  gr->SetMarkerColor(1);
  gr->SetMarkerStyle(8);
  gr->SetMarkerSize(0.8);

  //  s_1->Add(histNew1);

//   histNew2->SetLineColor(4);
//   histNew2->SetLineWidth(2);
//   //  histNew2->Rebin(2);  // REBIN
//   //  denomNew2->Rebin(2); // REBIN
//   histNew2->Divide(denomNew2);
//   s_1->Add(histNew2);
//   //  s_1->GetXaxis()->SetAxisTitle(hist1->GetXaxis()->GetAxisTitle);

//  s_1->Draw("nostack");
  TAxis * histXAxis = (TAxis*) histNew->GetXaxis();
  //  TAxis * stackXAxis = (TAxis*) s_1->GetXaxis();
  TAxis * graphXAxis = (TAxis*) gr->GetXaxis();
  graphXAxis->SetTitle(histXAxis->GetTitle());

  //  s_1->Draw("nostack");
  gr->Draw("zp");

  SavePlot(imageName);
  //  outputHistoFile->WriteTObject();

  delete gr;
}

void EwkDqmTreeHang::SavePlot(const char* filename)
{
  // PNG
  TString s1(filename);
  s1.Append(".png");
  gPad->Print(s1.Data(),"png");

  // C++ macro
  TString s2(filename);
  s2.Append(".C");
  gPad->Print(s2.Data(),"cxx");
}

