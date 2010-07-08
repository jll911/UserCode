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

void dqmDisplayMET(TString inputFilename)
{
  // filename
  //  TString inputFilename("DQM-histos-19feb10.root");
  //  TString inputFilename(inputFilenameChars);
  TString outputFilename = inputFilename.Copy();
  outputFilename.Prepend("DisplayMET-");

  // input
  TFile * inputFile = TFile::Open(inputFilename);

  // output
  TFile * outputFile = new TFile(outputFilename,"RECREATE");

  inputFile->cd();
  ewkElecStandaloneDQM->cd();


  gROOT->SetBatch( true );

  // canvases

  const int numTypes = 6;
  TString types[numTypes] = {"CALO","CALOCORR","PF","PFCORR","TC","GEN"};
  TString when[2] = {"BEFORE","AFTER"};

  for( int i = 0; i < numTypes; i++ ) {
    for( int j = 0; j < 2; j++ ) {
      TCanvas * c_Met = new TCanvas("c_"+types[i]+"MET_"+when[j],"Top-level "+ types[i]+"MET histograms "+when[j]+" cuts",1200,800);
      c_Met->Divide(3,2);
    
      c_Met->cd(1);
      //       TH1F * h_caloMET = (TH1F*) gDirectory->Get("MET_"+types[i]+"_ET_"+when[j]);
      //       h_caloMET->Draw();
      (TH1F*) gDirectory->Get("MET_"+types[i]+"_ET_"+when[j])->Draw();
      
      c_Met->cd(2);
      //       TH1F * h_caloMEX = (TH1F*) gDirectory->Get("MET_"+types[i]+"_EX_"+when[j]); 
      //       h_caloMEX->Draw();
      (TH1F*) gDirectory->Get("MET_"+types[i]+"_EX_"+when[j])->Draw();
      
      c_Met->cd(3);
      //       TH1F * h_caloMEY = (TH1F*) gDirectory->Get("MET_"+types[i]+"_EY_"+when[j]);
      //       h_caloMEY->Draw();
      (TH1F*) gDirectory->Get("MET_"+types[i]+"_EY_"+when[j])->Draw();
      
      c_Met->cd(4);
      //       TH1F * h_caloMET_phi = (TH1F*) gDirectory->Get("MET_"+types[i]+"_PHI_"+when[j]); 
      //       h_caloMET_phi->Draw();
      (TH1F*) gDirectory->Get("MET_"+types[i]+"_PHI_"+when[j])->Draw(); 
      
      c_Met->cd(5);
      //       TH1F * h_caloMET_sumEt = (TH1F*) gDirectory->Get("MET_"+types[i]+"_SUMET_"+when[j]);
      //       h_caloMET_sumEt->Draw();
      (TH1F*) gDirectory->Get("MET_"+types[i]+"_SUMET_"+when[j])->Draw();
      
      c_Met->cd(6);
      //       TH1F * h_caloMET_En = (TH1F*) gDirectory->Get("MET_"+types[i]+"_ENERGY_"+when[j]);
      //       h_caloMET_En->Draw();
      (TH1F*) gDirectory->Get("MET_"+types[i]+"_ENERGY_"+when[j])->Draw();
      
      outputFile->WriteTObject(c_Met);
    }
  }  

  for( int i = 0; i < numTypes; i++ ) {
    for( int j = 0; j < 2; j++ ) {
      TCanvas * c_Ws = new TCanvas("c_W_"+types[i]+"MET_"+when[j],"Top-level "+ types[i]+"W histograms "+when[j]+" cuts",800,800);
      c_Ws->Divide(2,2);
//       std::cout << "W_" << types[i] << "MET_MT_" << when[j] << endl;
      c_Ws->cd(1);
      (TH1F*) gDirectory->Get("W_"+types[i]+"MET_MT_"+when[j])->Draw();
      
      c_Ws->cd(2);
      (TH1F*) gDirectory->Get("W_"+types[i]+"MET_PT_"+when[j])->Draw();
      
      c_Ws->cd(3);
      (TH1F*) gDirectory->Get("W_"+types[i]+"MET_PHI_"+when[j])->Draw();
      
      c_Ws->cd(4);
      (TH1F*) gDirectory->Get("W_"+types[i]+"MET_RAPIDITY_"+when[j])->Draw(); 
      
      outputFile->WriteTObject(c_Ws);
    }
  }


  gROOT->SetBatch( false );
  // close things

  inputFile->Close();
  outputFile->Close();

  // delete leftovers

  for( int i = 0; i < numTypes; i++ ) {
    for( int j = 0; j < 2; j++ ) {
      TCanvas * c1 = gROOT->FindObject("c_"+types[i]+"MET_"+when[j]);
      delete c1;
      TCanvas * c2 = gROOT->FindObject("c_W_"+types[i]+"MET_"+when[j]);
      delete c2;
      ;
    }
  }

  delete inputFile;
  delete outputFile;

  gROOT->SetBatch( false );
}
