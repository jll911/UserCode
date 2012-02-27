//=====================================================================-*-C++-*-
// File and Version Information:
//      $Id: RooUnfoldExample.cxx,v 1.1 2011/08/17 13:54:34 anderson Exp $
//
// Description:
//      Simple example usage of the RooUnfold package using toy MC.
//
// Authors: Tim Adye <T.J.Adye@rl.ac.uk> and Fergus Wilson <fwilson@slac.stanford.edu>
//
//==============================================================================

#if !defined(__CINT__) || defined(__MAKECINT__)
#include <iostream>
using std::cout;
using std::endl;

#include "TRandom.h"
#include "TH1D.h"

#include "RooUnfoldResponse.h"
#include "RooUnfoldBayes.h"
//#include "RooUnfoldSvd.h"
//#include "RooUnfoldBinByBin.h"
#endif

//==============================================================================
// Example Unfolding
//==============================================================================

void RooUnfoldExample() {
#ifdef __CINT__
  gSystem->Load("libRooUnfold");
#endif

  TString dir_data              = "g_pass_Liso_allEta_allPt";
  TString name_sim_true         = "h_genjet_num_pt30";
  TString name_sim_true_v_meas  = "h_genVpf30_jet_num";
  TString name_data_meas        = "h_pfjet_num_pt30";
  TString name_data_meas_unfold = "h_pfjet_num_pt30";  // Used in output

  cout << "==================================== TRAIN ====================================" << endl;
  TFile *file_simulated = new TFile("../2011spring/hist_files/Hists_PhotonJet_GEN.root","read");
	  //TFile *file_simulated = new TFile("../2011spring/hist_files/Hists_PhotonJet_Madgraph_GEN.root","read");
	  //TFile *file_data      = new TFile("../2011spring/hist_files/Hists_Data.root","read");
	  //TFile *file_data      = new TFile("../2011spring/hist_files_postPurity/Hists_Data_scaled_summed.root","read");
  TFile *file_data      = new TFile("../2011spring/hist_files_postPurity/Hists_Data_scaled_summed_effCorr.root","read");
  TFile *file_data_unfolded = new TFile("../2011spring/Hists_Data_unfolded_Pythia.root","UPDATE");

  TH1F *h_1D_sim_true         = (TH1F*)file_simulated->Get(dir_data+"/"+name_sim_true);
  TH2F *h_sim_true_v_measured = (TH2F*)file_simulated->Get("g_pass_Liso_allEta_allPt/" + name_sim_true_v_meas);
  TH1F *h_1D_data_measured    = (TH1F*)file_data->Get(dir_data+"/"+name_data_meas);

  // Create Response matrix
  RooUnfoldResponse response(h_sim_true_v_measured->ProjectionX(),
							 h_1D_sim_true,
							 h_sim_true_v_measured);

  cout << "==================================== TEST =====================================" << endl;
	  //TH1F* hTrue= (TH1F*)file_simulated->Get("g_pass_Liso_all/h_genjet_num_bin");
	  //TH1F* hMeas= (TH1F*)file_data->Get("g_pass_Liso_all/h_pfjet_num_bin");
  TH1F* hTrue = h_1D_sim_true;
  TH1F* hMeas = h_1D_data_measured;

  cout << "==================================== UNFOLD ===================================" << endl;
  RooUnfoldBayes    unfold (&response, hMeas, 4);    // OR
//RooUnfoldSvd      unfold (&response, hMeas, 4);   // OR
//RooUnfoldBinByBin unfold (&response, hMeas);

  TH1D* hReco= (TH1D*) unfold.Hreco();

  unfold.PrintTable (cout, hTrue);
  hReco->SetLineColor(kRed);
  hReco->Draw();
  hMeas->Draw("SAME");
	  //hTrue->SetLineColor(8);
	  //hTrue->Draw("SAME");

  hReco->SetName(name_data_meas_unfold);
  file_data_unfolded->cd();
  gDirectory->mkdir(dir_data);
  file_data_unfolded->cd(dir_data);
  hReco->Write();
  file_data_unfolded->Close();
  cout << "Created " << file_data_unfolded->GetName() << endl;
}

#ifndef __CINT__
int main () { RooUnfoldExample(); return 0; }  // Main program when run stand-alone
#endif
