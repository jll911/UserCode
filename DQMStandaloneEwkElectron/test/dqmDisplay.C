//////////////////////////////////////////
//
//  Macro to take plots from output of
//  standalone electron DQM and display 
//  them in a nice format
//
//////////////////////////////////////////

#include <iostream>
using std::cout;
using std::endl;


void dqmDisplay(TString inputFilename)
{

  gStyle->SetOptStat(110010);

  // filename
  TString outputFilename = inputFilename.Copy();
  outputFilename.Prepend("Display-TEST-");

  // input
  TFile * inputFile = TFile::Open(inputFilename);

  // output
  TFile * outputFile = new TFile(outputFilename,"RECREATE");

  inputFile->cd();
  ewkElecStandaloneDQM->cd();




  // canvases


  // top-level stuff

  // before cuts
  TCanvas * c_topLevelBefore = new TCanvas("c_topLevelBefore","Top-level histograms before cuts",1200,800);
  c_topLevelBefore->Divide(5,3);

  c_topLevelBefore->cd(1);
  TH1F * h_pT_before = (TH1F*) ELE_PT_BEFORECUTS->Clone("h_pT_before");  
  h_pT_before->Draw();

  c_topLevelBefore->cd(2);
  TH1F * h_eta_before = (TH1F*) ELE_ETA_BEFORECUTS->Clone("h_eta_before");  
  h_eta_before->Draw();

  c_topLevelBefore->cd(3);
  TH1F * h_sieieBarrel_before = (TH1F*) ELE_SIEIEBARREL_BEFORECUTS->Clone("h_sieieBarrel_before");  
  h_sieieBarrel_before->Draw();

  c_topLevelBefore->cd(4);
  TH1F * h_sieieEndcap_before = (TH1F*) ELE_SIEIEENDCAP_BEFORECUTS->Clone("h_sieieEndcap_before");  
  h_sieieEndcap_before->Draw();

  c_topLevelBefore->cd(5);
  TH1F * h_detainBarrel_before = (TH1F*) ELE_DETAINBARREL_BEFORECUTS->Clone("h_detainBarrel_before");  
  h_detainBarrel_before->Draw();

  c_topLevelBefore->cd(6);
  TH1F * h_detainEndcap_before = (TH1F*) ELE_DETAINENDCAP_BEFORECUTS->Clone("h_detainEndcap_before");  
  h_detainEndcap_before->Draw();

  c_topLevelBefore->cd(7);
  TH1F * h_ecalIsoBarrel_before = (TH1F*) ELE_ECALISOBARREL_BEFORECUTS->Clone("h_ecalIsoBarrel_before");  
  h_ecalIsoBarrel_before->Draw();

  c_topLevelBefore->cd(8);
  TH1F * h_ecalIsoEndcap_before = (TH1F*) ELE_ECALISOENDCAP_BEFORECUTS->Clone("h_ecalIsoEndcap_before");  
  h_ecalIsoEndcap_before->Draw();

  c_topLevelBefore->cd(8);
  TH1F * h_hcalIsoBarrel_before = (TH1F*) ELE_HCALISOBARREL_BEFORECUTS->Clone("h_hcalIsoBarrel_before");  
  h_hcalIsoBarrel_before->Draw();

  c_topLevelBefore->cd(9);
  TH1F * h_hcalIsoEndcap_before = (TH1F*) ELE_HCALISOENDCAP_BEFORECUTS->Clone("h_hcalIsoEndcap_before");  
  h_hcalIsoEndcap_before->Draw();

  c_topLevelBefore->cd(10);
  TH1F * h_trkIsoBarrel_before = (TH1F*) ELE_TRKISOBARREL_BEFORECUTS->Clone("h_trkIsoBarrel_before");  
  h_trkIsoBarrel_before->Draw();

  c_topLevelBefore->cd(11);
  TH1F * h_trkIsoEndcap_before = (TH1F*) ELE_TRKISOENDCAP_BEFORECUTS->Clone("h_trkIsoEndcap_before");  
  h_trkIsoEndcap_before->Draw();

  c_topLevelBefore->cd(12);
  TH1F * h_trig_before = (TH1F*) ELE_TRIG_BEFORECUTS->Clone("h_trig_before");  
  h_trig_before->Draw();

  c_topLevelBefore->cd(13);
  TH1F * h_fBrem_before = (TH1F*) ELE_FBREM_BEFORECUTS->Clone("h_fBrem_before");  
  h_fBrem_before->Draw();

  c_topLevelBefore->cd(14);
  TH1F * h_invMass_before = (TH1F*) ELE_INVMASS_BEFORECUTS->Clone("h_invMass_before");  
  h_invMass_before->Draw();

  c_topLevelBefore->cd(15);
  TH1F * h_nElectrons_before = (TH1F*) ELE_NELECTRONS_BEFORECUTS->Clone("h_nElectrons_before");  
  h_nElectrons_before->Draw();



  // after cuts, or last cut
  TCanvas * c_topLevelAfter = new TCanvas("c_topLevelAfter","Top-level histograms after cuts",1200,800);
  c_topLevelAfter->Divide(5,3);

  c_topLevelAfter->cd(1);
  TH1F * h_pT_after = (TH1F*) ELE_PT_LASTCUT->Clone("h_pT_after");  
  h_pT_after->Draw();

  c_topLevelAfter->cd(2);
  TH1F * h_eta_after = (TH1F*) ELE_ETA_LASTCUT->Clone("h_eta_after");  
  h_eta_after->Draw();

  c_topLevelAfter->cd(3);
  TH1F * h_sieieBarrel_after = (TH1F*) ELE_SIEIEBARREL_LASTCUT->Clone("h_sieieBarrel_after");  
  h_sieieBarrel_after->Draw();

  c_topLevelAfter->cd(4);
  TH1F * h_sieieEndcap_after = (TH1F*) ELE_SIEIEENDCAP_LASTCUT->Clone("h_sieieEndcap_after");  
  h_sieieEndcap_after->Draw();

  c_topLevelAfter->cd(5);
  TH1F * h_detainBarrel_after = (TH1F*) ELE_DETAINBARREL_LASTCUT->Clone("h_detainBarrel_after");  
  h_detainBarrel_after->Draw();

  c_topLevelAfter->cd(6);
  TH1F * h_detainEndcap_after = (TH1F*) ELE_DETAINENDCAP_LASTCUT->Clone("h_detainEndcap_after");  
  h_detainEndcap_after->Draw();

  c_topLevelAfter->cd(7);
  TH1F * h_ecalIsoBarrel_after = (TH1F*) ELE_ECALISOBARREL_LASTCUT->Clone("h_ecalIsoBarrel_after");  
  h_ecalIsoBarrel_after->Draw();

  c_topLevelAfter->cd(8);
  TH1F * h_ecalIsoEndcap_after = (TH1F*) ELE_ECALISOENDCAP_LASTCUT->Clone("h_ecalIsoEndcap_after");  
  h_ecalIsoEndcap_after->Draw();

  c_topLevelAfter->cd(8);
  TH1F * h_hcalIsoBarrel_after = (TH1F*) ELE_HCALISOBARREL_LASTCUT->Clone("h_hcalIsoBarrel_after");  
  h_hcalIsoBarrel_after->Draw();

  c_topLevelAfter->cd(9);
  TH1F * h_hcalIsoEndcap_after = (TH1F*) ELE_HCALISOENDCAP_LASTCUT->Clone("h_hcalIsoEndcap_after");  
  h_hcalIsoEndcap_after->Draw();

  c_topLevelAfter->cd(10);
  TH1F * h_trkIsoBarrel_after = (TH1F*) ELE_TRKISOBARREL_LASTCUT->Clone("h_trkIsoBarrel_after");  
  h_trkIsoBarrel_after->Draw();

  c_topLevelAfter->cd(11);
  TH1F * h_trkIsoEndcap_after = (TH1F*) ELE_TRKISOENDCAP_LASTCUT->Clone("h_trkIsoEndcap_after");  
  h_trkIsoEndcap_after->Draw();

  c_topLevelAfter->cd(12);
  TH1F * h_trig_after = (TH1F*) ELE_TRIG_LASTCUT->Clone("h_trig_after");  
  h_trig_after->Draw();

  c_topLevelAfter->cd(13);
  TH1F * h_fBrem_after = (TH1F*) ELE_FBREM_AFTERCUTS->Clone("h_fBrem_after");  
  h_fBrem_after->Draw();

  c_topLevelAfter->cd(14);
  TH1F * h_invMass_after = (TH1F*) ELE_INVMASS_AFTERCUTS->Clone("h_invMass_after");  
  h_invMass_after->Draw();

  c_topLevelAfter->cd(15);
  TH1F * h_nElectrons_after = (TH1F*) ELE_NELECTRONS_AFTERCUTS->Clone("h_nElectrons_after");  
  h_nElectrons_after->Draw();




  // shower shape variables

  // before
  TCanvas * c_showerShapeBefore = new TCanvas("c_showerShapeBefore","Shower shape histograms before cuts",1200,800);
  c_showerShapeBefore->Divide(5,3);

//   c_showerShapeBefore->cd(1);
//   TH1F * h_pT = (TH1F*) ELE_PT_BEFORECUTS->Clone("h_pT");  
//   h_pT->Draw();

  c_showerShapeBefore->cd(1);
  TH1F * h_e1x5_before = (TH1F*) ELE_E1X5_BEFORECUTS->Clone("h_e1x5_before");
  h_e1x5_before->Draw();

  c_showerShapeBefore->cd(2);
  TH1F * h_e2x5Max_before = (TH1F*) ELE_E2X5MAX_BEFORECUTS->Clone("h_e2x5Max_before");
  h_e2x5Max_before->Draw();

  c_showerShapeBefore->cd(3);
  TH1F * h_e5x5_before = (TH1F*) ELE_E5X5_BEFORECUTS->Clone("h_e5x5_before");
  h_e5x5_before->Draw();

  c_showerShapeBefore->cd(4);
  TH1F * h_hoe1_before = (TH1F*) ELE_HADRONICOVEREM1_BEFORECUTS->Clone("h_hoe1_before");
  h_hoe1_before->Draw();

  c_showerShapeBefore->cd(5);
  TH1F * h_hoe2_before = (TH1F*) ELE_HADRONICOVEREM2_BEFORECUTS->Clone("h_hoe2_before");
  h_hoe2_before->Draw();

  c_showerShapeBefore->cd(6);
  TH1F * h_hoe_before = (TH1F*) ELE_HADRONICOVEREM_BEFORECUTS->Clone("h_hoe_before");
  h_hoe_before->Draw();

  c_showerShapeBefore->cd(7);
  TH1F * h_h1oe_before = (TH1F*) ELE_HCALDEPTH1OVERECAL_BEFORECUTS->Clone("h_h1oe_before");
  h_h1oe_before->Draw();

  c_showerShapeBefore->cd(8);
  TH1F * h_h2oe_before = (TH1F*) ELE_HCALDEPTH2OVERECAL_BEFORECUTS->Clone("h_h2oe_before");
  h_h2oe_before->Draw();

  c_showerShapeBefore->cd(9);
  h_sieieBarrel_before->Draw();

  c_showerShapeBefore->cd(10);
  h_sieieEndcap_before->Draw();


  // after
  TCanvas * c_showerShapeAfter = new TCanvas("c_showerShapeAfter","Shower shape histograms after cuts",1200,800);
  c_showerShapeAfter->Divide(5,3);

//   c_showerShapeAfter->cd(1);
//   TH1F * h_pT = (TH1F*) ELE_PT_AFTERCUTS->Clone("h_pT");  
//   h_pT->Draw();

  c_showerShapeAfter->cd(1);
  TH1F * h_e1x5_after = (TH1F*) ELE_E1X5_AFTERCUTS->Clone("h_e1x5_after");
  h_e1x5_after->Draw();

  c_showerShapeAfter->cd(2);
  TH1F * h_e2x5Max_after = (TH1F*) ELE_E2X5MAX_AFTERCUTS->Clone("h_e2x5Max_after");
  h_e2x5Max_after->Draw();

  c_showerShapeAfter->cd(3);
  TH1F * h_e5x5_after = (TH1F*) ELE_E5X5_AFTERCUTS->Clone("h_e5x5_after");
  h_e5x5_after->Draw();

  c_showerShapeAfter->cd(4);
  TH1F * h_hoe1_after = (TH1F*) ELE_HADRONICOVEREM1_AFTERCUTS->Clone("h_hoe1_after");
  h_hoe1_after->Draw();

  c_showerShapeAfter->cd(5);
  TH1F * h_hoe2_after = (TH1F*) ELE_HADRONICOVEREM2_AFTERCUTS->Clone("h_hoe2_after");
  h_hoe2_after->Draw();

  c_showerShapeAfter->cd(6);
  TH1F * h_hoe_after = (TH1F*) ELE_HADRONICOVEREM_AFTERCUTS->Clone("h_hoe_after");
  h_hoe_after->Draw();

  c_showerShapeAfter->cd(7);
  TH1F * h_h1oe_after = (TH1F*) ELE_HCALDEPTH1OVERECAL_AFTERCUTS->Clone("h_h1oe_after");
  h_h1oe_after->Draw();

  c_showerShapeAfter->cd(8);
  TH1F * h_h2oe_after = (TH1F*) ELE_HCALDEPTH2OVERECAL_AFTERCUTS->Clone("h_h2oe_after");
  h_h2oe_after->Draw();

  c_showerShapeAfter->cd(9);
  h_sieieBarrel_after->Draw();

  c_showerShapeAfter->cd(10);
  h_sieieEndcap_after->Draw();



  // isolation variables

  // before cuts
  TCanvas * c_isolationBefore = new TCanvas("c_isolationBefore","Isolation histograms before cuts",1200,800);
  c_isolationBefore->Divide(5,3);

  c_isolationBefore->cd(1);
  TH1F * h_ecal03_before = (TH1F*) ELE_DR03ECALRECHITSUMET_BEFORECUTS->Clone("h_ecal03_before");  
  h_ecal03_before->Draw();

  c_isolationBefore->cd(2);
  TH1F * h_hcalTot03_before = (TH1F*) ELE_DR03HCALTOWERSUMET_BEFORECUTS->Clone("h_hcalTot03_before");  
  h_hcalTot03_before->Draw();

  c_isolationBefore->cd(3);
  TH1F * h_hcal1D03_before = (TH1F*) ELE_DR03HCALDEPTH1TOWERSUMET_BEFORECUTS->Clone("h_hcal1D03_before");  
  h_hcal1D03_before->Draw();

  c_isolationBefore->cd(4);
  TH1F * h_hcal2D03_before = (TH1F*) ELE_DR03HCALDEPTH2TOWERSUMET_BEFORECUTS->Clone("h_hcal2D03_before");  
  h_hcal2D03_before->Draw();

  c_isolationBefore->cd(5);
  TH1F * h_trk03_before = (TH1F*) ELE_DR03TKSUMPT_BEFORECUTS->Clone("h_trk03_before");  
  h_trk03_before->Draw();


  c_isolationBefore->cd(6);
  TH1F * h_ecal04_before = (TH1F*) ELE_DR04ECALRECHITSUMET_BEFORECUTS->Clone("h_ecal04_before");  
  h_ecal04_before->Draw();

  c_isolationBefore->cd(7);
  TH1F * h_hcalTot04_before = (TH1F*) ELE_DR04HCALTOWERSUMET_BEFORECUTS->Clone("h_hcalTot04_before");  
  h_hcalTot04_before->Draw();

  c_isolationBefore->cd(8);
  TH1F * h_hcal1D04_before = (TH1F*) ELE_DR04HCALDEPTH1TOWERSUMET_BEFORECUTS->Clone("h_hcal1D04_before");  
  h_hcal1D04_before->Draw();

  c_isolationBefore->cd(9);
  TH1F * h_hcal2D04_before = (TH1F*) ELE_DR04HCALDEPTH2TOWERSUMET_BEFORECUTS->Clone("h_hcal2D04_before");  
  h_hcal2D04_before->Draw();

  c_isolationBefore->cd(10);
  TH1F * h_trk04_before = (TH1F*) ELE_DR04TKSUMPT_BEFORECUTS->Clone("h_trk04_before");  
  h_trk04_before->Draw();


  // after cuts
  TCanvas * c_isolationAfter = new TCanvas("c_isolationAfter","Isolation histograms after cuts",1200,800);
  c_isolationAfter->Divide(5,3);

  c_isolationAfter->cd(1);
  TH1F * h_ecal03_after = (TH1F*) ELE_DR03ECALRECHITSUMET_AFTERCUTS->Clone("h_ecal03_after");  
  h_ecal03_after->Draw();

  c_isolationAfter->cd(2);
  TH1F * h_hcalTot03_after = (TH1F*) ELE_DR03HCALTOWERSUMET_AFTERCUTS->Clone("h_hcalTot03_after");  
  h_hcalTot03_after->Draw();

  c_isolationAfter->cd(3);
  TH1F * h_hcal1D03_after = (TH1F*) ELE_DR03HCALDEPTH1TOWERSUMET_AFTERCUTS->Clone("h_hcal1D03_after");  
  h_hcal1D03_after->Draw();

  c_isolationAfter->cd(4);
  TH1F * h_hcal2D03_after = (TH1F*) ELE_DR03HCALDEPTH2TOWERSUMET_AFTERCUTS->Clone("h_hcal2D03_after");  
  h_hcal2D03_after->Draw();

  c_isolationAfter->cd(5);
  TH1F * h_trk03_after = (TH1F*) ELE_DR03TKSUMPT_AFTERCUTS->Clone("h_trk03_after");  
  h_trk03_after->Draw();


  c_isolationAfter->cd(6);
  TH1F * h_ecal04_after = (TH1F*) ELE_DR04ECALRECHITSUMET_AFTERCUTS->Clone("h_ecal04_after");  
  h_ecal04_after->Draw();

  c_isolationAfter->cd(7);
  TH1F * h_hcalTot04_after = (TH1F*) ELE_DR04HCALTOWERSUMET_AFTERCUTS->Clone("h_hcalTot04_after");  
  h_hcalTot04_after->Draw();

  c_isolationAfter->cd(8);
  TH1F * h_hcal1D04_after = (TH1F*) ELE_DR04HCALDEPTH1TOWERSUMET_AFTERCUTS->Clone("h_hcal1D04_after");  
  h_hcal1D04_after->Draw();

  c_isolationAfter->cd(9);
  TH1F * h_hcal2D04_after = (TH1F*) ELE_DR04HCALDEPTH2TOWERSUMET_AFTERCUTS->Clone("h_hcal2D04_after");  
  h_hcal2D04_after->Draw();

  c_isolationAfter->cd(10);
  TH1F * h_trk04_after = (TH1F*) ELE_DR04TKSUMPT_AFTERCUTS->Clone("h_trk04_after");  
  h_trk04_after->Draw();




  // supercluster variables

  // before
  TCanvas * c_scBefore = new TCanvas("c_scBefore","Supercluster histograms before cuts",1200,800);
  c_scBefore->Divide(5,3);

  c_scBefore->cd(1);
  TH1F * h_nBasicClusters_before = (TH1F*) ELE_BASICCLUSTERSSIZE_BEFORECUTS->Clone("h_nBasicClusters_before");
  h_nBasicClusters_before->Draw();

  c_scBefore->cd(2);
  TH1F * h_scE1x5_before = (TH1F*) ELE_SCE1X5_BEFORECUTS->Clone("h_scE1x5_before");
  h_scE1x5_before->Draw();

  c_scBefore->cd(3);
  TH1F * h_scE2x5Max_before = (TH1F*) ELE_SCE2X5MAX_BEFORECUTS->Clone("h_scE2x5Max_before");
  h_scE2x5Max_before->Draw();

  c_scBefore->cd(4);
  TH1F * h_scE5x5_before = (TH1F*) ELE_SCE5X5_BEFORECUTS->Clone("h_scE5x5_before");
  h_scE5x5_before->Draw();

  c_scBefore->cd(5);
  TH1F * h_scPixCharge_before = (TH1F*) ELE_SCPIXCHARGE_BEFORECUTS->Clone("h_scPixCharge_before");
  h_scPixCharge_before->Draw();

  c_scBefore->cd(6);
  TH1F * h_scSigmaEtaEta_before = (TH1F*) ELE_SCSIGMAETAETA_BEFORECUTS->Clone("h_scSigmaEtaEta_before");
  h_scSigmaEtaEta_before->Draw();

  c_scBefore->cd(7);
  TH1F * h_scSigmaIetaIeta_before = (TH1F*) ELE_SCSIGMAIETAIETA_BEFORECUTS->Clone("h_scSigmaIetaIeta_before");
  h_scSigmaIetaIeta_before->Draw();


  // after
  TCanvas * c_scAfter = new TCanvas("c_scAfter","Supercluster histograms after cuts",1200,800);
  c_scAfter->Divide(5,3);

  c_scAfter->cd(1);
  TH1F * h_nBasicClusters_after = (TH1F*) ELE_BASICCLUSTERSSIZE_AFTERCUTS->Clone("h_nBasicClusters_after");
  h_nBasicClusters_after->Draw();

  c_scAfter->cd(2);
  TH1F * h_scE1x5_after = (TH1F*) ELE_SCE1X5_AFTERCUTS->Clone("h_scE1x5_after");
  h_scE1x5_after->Draw();

  c_scAfter->cd(3);
  TH1F * h_scE2x5Max_after = (TH1F*) ELE_SCE2X5MAX_AFTERCUTS->Clone("h_scE2x5Max_after");
  h_scE2x5Max_after->Draw();

  c_scAfter->cd(4);
  TH1F * h_scE5x5_after = (TH1F*) ELE_SCE5X5_AFTERCUTS->Clone("h_scE5x5_after");
  h_scE5x5_after->Draw();

  c_scAfter->cd(5);
  TH1F * h_scPixCharge_after = (TH1F*) ELE_SCPIXCHARGE_AFTERCUTS->Clone("h_scPixCharge_after");
  h_scPixCharge_after->Draw();

  c_scAfter->cd(6);
  TH1F * h_scSigmaEtaEta_after = (TH1F*) ELE_SCSIGMAETAETA_AFTERCUTS->Clone("h_scSigmaEtaEta_after");
  h_scSigmaEtaEta_after->Draw();

  c_scAfter->cd(7);
  TH1F * h_scSigmaIetaIeta_after = (TH1F*) ELE_SCSIGMAIETAIETA_AFTERCUTS->Clone("h_scSigmaIetaIeta_after");
  h_scSigmaIetaIeta_after->Draw();




  // track variables

  // before
  TCanvas * c_trackBefore = new TCanvas("c_trackBefore","Track quantities before cuts",1200,800);
  c_trackBefore->Divide(5,3);

  c_trackBefore->cd(1);
  TH1F * h_trackPositionAtCaloX_before = (TH1F*) ELE_TRACKPOSITIONATCALOX_BEFORECUTS->Clone("h_trackPositionAtCaloX_before");
  h_trackPositionAtCaloX_before->Draw();

  c_trackBefore->cd(2);
  TH1F * h_trackPositionAtCaloY_before = (TH1F*) ELE_TRACKPOSITIONATCALOY_BEFORECUTS->Clone("h_trackPositionAtCaloY_before");
  h_trackPositionAtCaloY_before->Draw();

  c_trackBefore->cd(3);
  TH1F * h_trackPositionAtCaloZ_before = (TH1F*) ELE_TRACKPOSITIONATCALOZ_BEFORECUTS->Clone("h_trackPositionAtCaloZ_before");
  h_trackPositionAtCaloZ_before->Draw();

  c_trackBefore->cd(4);
  TH1F * h_trackPositionAtVtxX_before = (TH1F*) ELE_TRACKPOSITIONATVTXX_BEFORECUTS->Clone("h_trackPositionAtVtxX_before");
  h_trackPositionAtVtxX_before->Draw();

  c_trackBefore->cd(5);
  TH1F * h_trackPositionAtVtxY_before = (TH1F*) ELE_TRACKPOSITIONATVTXY_BEFORECUTS->Clone("h_trackPositionAtVtxY_before");
  h_trackPositionAtVtxY_before->Draw();

  c_trackBefore->cd(6);
  TH1F * h_trackPositionAtVtxZ_before = (TH1F*) ELE_TRACKPOSITIONATVTXZ_BEFORECUTS->Clone("h_trackPositionAtVtxZ_before");
  h_trackPositionAtVtxZ_before->Draw();

  c_trackBefore->cd(7);
  TH1F * h_trackMomentumAtCaloX_before = (TH1F*) ELE_TRACKMOMENTUMATCALOX_BEFORECUTS->Clone("h_trackMomentumAtCaloX_before");
  h_trackMomentumAtCaloX_before->Draw();

  c_trackBefore->cd(8);
  TH1F * h_trackMomentumAtCaloY_before = (TH1F*) ELE_TRACKMOMENTUMATCALOY_BEFORECUTS->Clone("h_trackMomentumAtCaloY_before");
  h_trackMomentumAtCaloY_before->Draw();

  c_trackBefore->cd(9);
  TH1F * h_trackMomentumAtCaloZ_before = (TH1F*) ELE_TRACKMOMENTUMATCALOZ_BEFORECUTS->Clone("h_trackMomentumAtCaloZ_before");
  h_trackMomentumAtCaloZ_before->Draw();

  c_trackBefore->cd(10);
  TH1F * h_trackMomentumAtVtxX_before = (TH1F*) ELE_TRACKMOMENTUMATVTXX_BEFORECUTS->Clone("h_trackMomentumAtVtxX_before");
  h_trackMomentumAtVtxX_before->Draw();

  c_trackBefore->cd(11);
  TH1F * h_trackMomentumAtVtxY_before = (TH1F*) ELE_TRACKMOMENTUMATVTXY_BEFORECUTS->Clone("h_trackMomentumAtVtxY_before");
  h_trackMomentumAtVtxY_before->Draw();

  c_trackBefore->cd(12);
  TH1F * h_trackMomentumAtVtxZ_before = (TH1F*) ELE_TRACKMOMENTUMATVTXZ_BEFORECUTS->Clone("h_trackMomentumAtVtxZ_before");
  h_trackMomentumAtVtxZ_before->Draw();


  // after
  TCanvas * c_trackAfter = new TCanvas("c_trackAfter","Track quantities after cuts",1200,800);
  c_trackAfter->Divide(5,3);

  c_trackAfter->cd(1);
  TH1F * h_trackPositionAtCaloX_after = (TH1F*) ELE_TRACKPOSITIONATCALOX_AFTERCUTS->Clone("h_trackPositionAtCaloX_after");
  h_trackPositionAtCaloX_after->Draw();

  c_trackAfter->cd(2);
  TH1F * h_trackPositionAtCaloY_after = (TH1F*) ELE_TRACKPOSITIONATCALOY_AFTERCUTS->Clone("h_trackPositionAtCaloY_after");
  h_trackPositionAtCaloY_after->Draw();

  c_trackAfter->cd(3);
  TH1F * h_trackPositionAtCaloZ_after = (TH1F*) ELE_TRACKPOSITIONATCALOZ_AFTERCUTS->Clone("h_trackPositionAtCaloZ_after");
  h_trackPositionAtCaloZ_after->Draw();

  c_trackAfter->cd(4);
  TH1F * h_trackPositionAtVtxX_after = (TH1F*) ELE_TRACKPOSITIONATVTXX_AFTERCUTS->Clone("h_trackPositionAtVtxX_after");
  h_trackPositionAtVtxX_after->Draw();

  c_trackAfter->cd(5);
  TH1F * h_trackPositionAtVtxY_after = (TH1F*) ELE_TRACKPOSITIONATVTXY_AFTERCUTS->Clone("h_trackPositionAtVtxY_after");
  h_trackPositionAtVtxY_after->Draw();

  c_trackAfter->cd(6);
  TH1F * h_trackPositionAtVtxZ_after = (TH1F*) ELE_TRACKPOSITIONATVTXZ_AFTERCUTS->Clone("h_trackPositionAtVtxZ_after");
  h_trackPositionAtVtxZ_after->Draw();

  c_trackAfter->cd(7);
  TH1F * h_trackMomentumAtCaloX_after = (TH1F*) ELE_TRACKMOMENTUMATCALOX_AFTERCUTS->Clone("h_trackMomentumAtCaloX_after");
  h_trackMomentumAtCaloX_after->Draw();

  c_trackAfter->cd(8);
  TH1F * h_trackMomentumAtCaloY_after = (TH1F*) ELE_TRACKMOMENTUMATCALOY_AFTERCUTS->Clone("h_trackMomentumAtCaloY_after");
  h_trackMomentumAtCaloY_after->Draw();

  c_trackAfter->cd(9);
  TH1F * h_trackMomentumAtCaloZ_after = (TH1F*) ELE_TRACKMOMENTUMATCALOZ_AFTERCUTS->Clone("h_trackMomentumAtCaloZ_after");
  h_trackMomentumAtCaloZ_after->Draw();

  c_trackAfter->cd(10);
  TH1F * h_trackMomentumAtVtxX_after = (TH1F*) ELE_TRACKMOMENTUMATVTXX_AFTERCUTS->Clone("h_trackMomentumAtVtxX_after");
  h_trackMomentumAtVtxX_after->Draw();

  c_trackAfter->cd(11);
  TH1F * h_trackMomentumAtVtxY_after = (TH1F*) ELE_TRACKMOMENTUMATVTXY_AFTERCUTS->Clone("h_trackMomentumAtVtxY_after");
  h_trackMomentumAtVtxY_after->Draw();

  c_trackAfter->cd(12);
  TH1F * h_trackMomentumAtVtxZ_after = (TH1F*) ELE_TRACKMOMENTUMATVTXZ_AFTERCUTS->Clone("h_trackMomentumAtVtxZ_after");
  h_trackMomentumAtVtxZ_after->Draw();





  // track-matching variables

  // before
  TCanvas * c_trackMatchBefore = new TCanvas("c_trackMatchBefore","Track-matching quantities before cuts",1200,800);
  c_trackMatchBefore->Divide(5,3);

  c_trackMatchBefore->cd(1);
  TH1F * h_dEtaEleTrackCalo_before = (TH1F*) ELE_DELTAETAELECLUSTERTRACKATCALO_BEFORECUTS->Clone("h_dEtaEleTrackCalo_before");
  h_dEtaEleTrackCalo_before->Draw();

  c_trackMatchBefore->cd(2);
  TH1F * h_dEtaSeedTrackCalo_before = (TH1F*) ELE_DELTAETASEEDCLUSTERTRACKATCALO_BEFORECUTS->Clone("h_dEtaSeedTrackCalo_before");
  h_dEtaSeedTrackCalo_before->Draw();

  c_trackMatchBefore->cd(3);
  TH1F * h_dEtaSCTrackVtx_before = (TH1F*) ELE_DELTAETASUPERCLUSTERTRACKATVTX_BEFORECUTS->Clone("h_dEtaSCTrackVtx_before");
  h_dEtaSCTrackVtx_before->Draw();

  c_trackMatchBefore->cd(4);
  TH1F * h_dPhiEleTrackCalo_before = (TH1F*) ELE_DELTAPHIELECLUSTERTRACKATCALO_BEFORECUTS->Clone("h_dPhiEleTrackCalo_before");
  h_dPhiEleTrackCalo_before->Draw();

  c_trackMatchBefore->cd(5);
  TH1F * h_dPhiSeedTrackCalo_before = (TH1F*) ELE_DELTAPHISEEDCLUSTERTRACKATCALO_BEFORECUTS->Clone("h_dPhiSeedTrackCalo_before");
  h_dPhiSeedTrackCalo_before->Draw();

  c_trackMatchBefore->cd(6);
  TH1F * h_dPhiSCTrackVtx_before = (TH1F*) ELE_DELTAPHISUPERCLUSTERTRACKATVTX_BEFORECUTS->Clone("h_dPhiSCTrackVtx_before");
  h_dPhiSCTrackVtx_before->Draw();

  // e/p
  c_trackMatchBefore->cd(7);
  TH1F * h_eEleOPout_before = (TH1F*) ELE_EELECLUSTEROVERPOUT_BEFORECUTS->Clone("h_eEleOPout_before");
  h_eEleOPout_before->Draw();

  c_trackMatchBefore->cd(8);
  TH1F * h_eSeedOP_before = (TH1F*) ELE_ESEEDCLUSTEROVERP_BEFORECUTS->Clone("h_eSeedOP_before");
  h_eSeedOP_before->Draw();

  c_trackMatchBefore->cd(9);
  TH1F * h_eSeedOPout_before = (TH1F*) ELE_ESEEDCLUSTEROVERPOUT_BEFORECUTS->Clone("h_eSeedOPout_before");
  h_eSeedOPout_before->Draw();

  c_trackMatchBefore->cd(10);
  TH1F * h_eSCOP_before = (TH1F*) ELE_ESUPERCLUSTEROVERP_BEFORECUTS->Clone("h_eSCOP_before");
  h_eSCOP_before->Draw();


  // after
  TCanvas * c_trackMatchAfter = new TCanvas("c_trackMatchAfter","Track-matching quantities after cuts",1200,800);
  c_trackMatchAfter->Divide(5,3);

  c_trackMatchAfter->cd(1);
  TH1F * h_dEtaEleTrackCalo_after = (TH1F*) ELE_DELTAETAELECLUSTERTRACKATCALO_AFTERCUTS->Clone("h_dEtaEleTrackCalo_after");
  h_dEtaEleTrackCalo_after->Draw();

  c_trackMatchAfter->cd(2);
  TH1F * h_dEtaSeedTrackCalo_after = (TH1F*) ELE_DELTAETASEEDCLUSTERTRACKATCALO_AFTERCUTS->Clone("h_dEtaSeedTrackCalo_after");
  h_dEtaSeedTrackCalo_after->Draw();

  c_trackMatchAfter->cd(3);
  TH1F * h_dEtaSCTrackVtx_after = (TH1F*) ELE_DELTAETASUPERCLUSTERTRACKATVTX_AFTERCUTS->Clone("h_dEtaSCTrackVtx_after");
  h_dEtaSCTrackVtx_after->Draw();

  c_trackMatchAfter->cd(4);
  TH1F * h_dPhiEleTrackCalo_after = (TH1F*) ELE_DELTAPHIELECLUSTERTRACKATCALO_AFTERCUTS->Clone("h_dPhiEleTrackCalo_after");
  h_dPhiEleTrackCalo_after->Draw();

  c_trackMatchAfter->cd(5);
  TH1F * h_dPhiSeedTrackCalo_after = (TH1F*) ELE_DELTAPHISEEDCLUSTERTRACKATCALO_AFTERCUTS->Clone("h_dPhiSeedTrackCalo_after");
  h_dPhiSeedTrackCalo_after->Draw();

  c_trackMatchAfter->cd(6);
  TH1F * h_dPhiSCTrackVtx_after = (TH1F*) ELE_DELTAPHISUPERCLUSTERTRACKATVTX_AFTERCUTS->Clone("h_dPhiSCTrackVtx_after");
  h_dPhiSCTrackVtx_after->Draw();

  // e/p
  c_trackMatchAfter->cd(7);
  TH1F * h_eEleOPout_after = (TH1F*) ELE_EELECLUSTEROVERPOUT_AFTERCUTS->Clone("h_eEleOPout_after");
  h_eEleOPout_after->Draw();

  c_trackMatchAfter->cd(8);
  TH1F * h_eSeedOP_after = (TH1F*) ELE_ESEEDCLUSTEROVERP_AFTERCUTS->Clone("h_eSeedOP_after");
  h_eSeedOP_after->Draw();

  c_trackMatchAfter->cd(9);
  TH1F * h_eSeedOPout_after = (TH1F*) ELE_ESEEDCLUSTEROVERPOUT_AFTERCUTS->Clone("h_eSeedOPout_after");
  h_eSeedOPout_after->Draw();

  c_trackMatchAfter->cd(10);
  TH1F * h_eSCOP_after = (TH1F*) ELE_ESUPERCLUSTEROVERP_AFTERCUTS->Clone("h_eSCOP_after");
  h_eSCOP_after->Draw();




  // energy and momentum variables

  // before 
  TCanvas * c_energyMomBefore = new TCanvas("c_energyMomBefore","Energy and momentum variables before cuts",1200,800);
  c_energyMomBefore->Divide(5,3);

  c_energyMomBefore->cd(1);
  h_pT_before->Draw();

  c_energyMomBefore->cd(2);
  TH1F * h_ptEcal_before = (TH1F*) ELE_PTECALDRIVEN_BEFORECUTS->Clone("h_ptEcal_before");
  h_ptEcal_before->Draw();

  c_energyMomBefore->cd(3);
  TH1F * h_ptTrk_before = (TH1F*) ELE_PTTRKDRIVEN_BEFORECUTS->Clone("h_ptTrk_before");
  h_ptTrk_before->Draw();

  c_energyMomBefore->cd(4);
  TH1F * h_energy_before = (TH1F*) ELE_ENERGY_BEFORECUTS->Clone("h_energy_before");
  h_energy_before->Draw();

  c_energyMomBefore->cd(5);
  TH1F * h_caloEnergy_before = (TH1F*) ELE_CALOENERGY_BEFORECUTS->Clone("h_caloEnergy_before");
  h_caloEnergy_before->Draw();

  c_energyMomBefore->cd(6);
  TH1F * h_ecalEnergy_before = (TH1F*) ELE_ECALENERGY_BEFORECUTS->Clone("h_ecalEnergy_before");
  h_ecalEnergy_before->Draw();

  c_energyMomBefore->cd(7);
  TH1F * h_ecalEnergyError_before = (TH1F*) ELE_ECALENERGYERROR_BEFORECUTS->Clone("h_ecalEnergyError_before");
  h_ecalEnergyError_before->Draw();

  c_energyMomBefore->cd(8);
  TH1F * h_et_before = (TH1F*) ELE_ET_BEFORECUTS->Clone("h_et_before");
  h_et_before->Draw();

  c_energyMomBefore->cd(9);
  TH1F * h_p_before = (TH1F*) ELE_P_BEFORECUTS->Clone("h_p_before");
  h_p_before->Draw();

  c_energyMomBefore->cd(10);
  TH1F * h_px_before = (TH1F*) ELE_PX_BEFORECUTS->Clone("h_px_before");
  h_px_before->Draw();

  c_energyMomBefore->cd(11);
  TH1F * h_py_before = (TH1F*) ELE_PY_BEFORECUTS->Clone("h_py_before");
  h_py_before->Draw();

  c_energyMomBefore->cd(12);
  TH1F * h_pz_before = (TH1F*) ELE_PZ_BEFORECUTS->Clone("h_pz_before");
  h_pz_before->Draw();

  c_energyMomBefore->cd(13);
  TH1F * h_eleMomError_before = (TH1F*) ELE_ELECTRONMOMENTUMERROR_BEFORECUTS->Clone("h_eleMomError_before");
  h_eleMomError_before->Draw();


  // after 
  TCanvas * c_energyMomAfter = new TCanvas("c_energyMomAfter","Energy and momentum variables after cuts",1200,800);
  c_energyMomAfter->Divide(5,3);

  c_energyMomAfter->cd(1);
  h_pT_after->Draw();

  c_energyMomAfter->cd(2);
  TH1F * h_ptEcal_after = (TH1F*) ELE_PTECALDRIVEN_LASTCUT->Clone("h_ptEcal_after");
  h_ptEcal_after->Draw();

  c_energyMomAfter->cd(3);
  TH1F * h_ptTrk_after = (TH1F*) ELE_PTTRKDRIVEN_LASTCUT->Clone("h_ptTrk_after");
  h_ptTrk_after->Draw();

  c_energyMomAfter->cd(4);
  TH1F * h_energy_after = (TH1F*) ELE_ENERGY_AFTERCUTS->Clone("h_energy_after");
  h_energy_after->Draw();

  c_energyMomAfter->cd(5);
  TH1F * h_caloEnergy_after = (TH1F*) ELE_CALOENERGY_AFTERCUTS->Clone("h_caloEnergy_after");
  h_caloEnergy_after->Draw();

  c_energyMomAfter->cd(6);
  TH1F * h_ecalEnergy_after = (TH1F*) ELE_ECALENERGY_AFTERCUTS->Clone("h_ecalEnergy_after");
  h_ecalEnergy_after->Draw();

  c_energyMomAfter->cd(7);
  TH1F * h_ecalEnergyError_after = (TH1F*) ELE_ECALENERGYERROR_AFTERCUTS->Clone("h_ecalEnergyError_after");
  h_ecalEnergyError_after->Draw();

  c_energyMomAfter->cd(8);
  TH1F * h_et_after = (TH1F*) ELE_ET_AFTERCUTS->Clone("h_et_after");
  h_et_after->Draw();

  c_energyMomAfter->cd(9);
  TH1F * h_p_after = (TH1F*) ELE_P_AFTERCUTS->Clone("h_p_after");
  h_p_after->Draw();

  c_energyMomAfter->cd(10);
  TH1F * h_px_after = (TH1F*) ELE_PX_AFTERCUTS->Clone("h_px_after");
  h_px_after->Draw();

  c_energyMomAfter->cd(11);
  TH1F * h_py_after = (TH1F*) ELE_PY_AFTERCUTS->Clone("h_py_after");
  h_py_after->Draw();

  c_energyMomAfter->cd(12);
  TH1F * h_pz_after = (TH1F*) ELE_PZ_AFTERCUTS->Clone("h_pz_after");
  h_pz_after->Draw();

  c_energyMomAfter->cd(13);
  TH1F * h_eleMomError_after = (TH1F*) ELE_ELECTRONMOMENTUMERROR_AFTERCUTS->Clone("h_eleMomError_after");
  h_eleMomError_after->Draw();




//   // jet and met plots

//   // before cuts
//   TCanvas * c_jetMetBefore = new TCanvas("c_jetMetBefore","Jet/MET histograms before cuts",1200,800);
//   c_jetMetBefore->Divide(5,3);

//   c_jetMetBefore->cd(1);
//   TH1F * h_jetEt1_before = (TH1F*) ELE_JETET1_BEFORECUTS->Clone("h_jetEt1_before");  
//   h_jetEt1_before->Draw();

//   c_jetMetBefore->cd(2);
//   TH1F * h_jetEta1_before = (TH1F*) ELE_JETETA1_BEFORECUTS->Clone("h_jetEta1_before");  
//   h_jetEta1_before->Draw();

//   c_jetMetBefore->cd(3);
//   TH1F * h_met_before = (TH1F*) ELE_MET_BEFORECUTS->Clone("h_met_before");  
//   h_met_before->Draw();

//   c_jetMetBefore->cd(4);
//   TH1F * h_mt_before = (TH1F*) ELE_MT_BEFORECUTS->Clone("h_mt_before");  
//   h_mt_before->Draw();

//   c_jetMetBefore->cd(5);
//   TH1F * h_nJets_before = (TH1F*) ELE_NJETS_BEFORECUTS->Clone("h_nJets_before");  
//   h_nJets_before->Draw();


//   // after cuts
//   TCanvas * c_jetMetAfter = new TCanvas("c_jetMetAfter","Jet/MET histograms after cuts",1200,800);
//   c_jetMetAfter->Divide(5,3);

//   c_jetMetAfter->cd(1);
//   TH1F * h_jetEt1_after = (TH1F*) ELE_JETET1_AFTERCUTS->Clone("h_jetEt1_after");  
//   h_jetEt1_after->Draw();

//   c_jetMetAfter->cd(2);
//   TH1F * h_jetEta1_after = (TH1F*) ELE_JETETA1_AFTERCUTS->Clone("h_jetEta1_after");  
//   h_jetEta1_after->Draw();

//   c_jetMetAfter->cd(3);
//   TH1F * h_met_after = (TH1F*) ELE_MET_LASTCUT->Clone("h_met_after");  
//   h_met_after->Draw();

//   c_jetMetAfter->cd(4);
//   TH1F * h_mt_after = (TH1F*) ELE_MT_LASTCUT->Clone("h_mt_after");  
//   h_mt_after->Draw();

//   c_jetMetAfter->cd(5);
//   TH1F * h_nJets_after = (TH1F*) ELE_NJETS_LASTCUT->Clone("h_nJets_after");  
//   h_nJets_after->Draw();


    
  


  // write canvases to file

  outputFile->WriteTObject(c_topLevelBefore);
  outputFile->WriteTObject(c_topLevelAfter);
  outputFile->WriteTObject(c_showerShapeBefore);
  outputFile->WriteTObject(c_showerShapeAfter);
  outputFile->WriteTObject(c_isolationBefore);
  outputFile->WriteTObject(c_isolationAfter);
//   outputFile->WriteTObject(c_jetMetBefore);
//   outputFile->WriteTObject(c_jetMetAfter);
  outputFile->WriteTObject(c_trackMatchBefore);
  outputFile->WriteTObject(c_trackMatchAfter);
  outputFile->WriteTObject(c_trackBefore);
  outputFile->WriteTObject(c_trackAfter);
  outputFile->WriteTObject(c_energyMomBefore);
  outputFile->WriteTObject(c_energyMomAfter);
  outputFile->WriteTObject(c_scBefore);
  outputFile->WriteTObject(c_scAfter);
  

  // close things

  inputFile->Close();
  outputFile->Close();


  // delete leftovers

  delete c_topLevelBefore;
  delete c_topLevelAfter;
  delete c_showerShapeBefore;
  delete c_showerShapeAfter;
  delete c_isolationBefore;
  delete c_isolationAfter;
//   delete c_jetMetBefore;
//   delete c_jetMetAfter;
  delete c_trackMatchBefore;
  delete c_trackMatchAfter;
  delete c_trackBefore;
  delete c_trackAfter;
  delete c_energyMomBefore;
  delete c_energyMomAfter;
  delete c_scBefore;
  delete c_scAfter;

  delete inputFile;
  delete outputFile;

}
