//////////////////////////////////////////
//
//  Macro to take plots from output of
//  standalone electron DQM and display 
//  them in a nice format
//
//////////////////////////////////////////


#include "TCanvas.h"
#include "TFile.h"
#include "TH1F.h"
#include "TROOT.h"
#include "TString.h"

#include <iostream>
using std::cout;
using std::endl;

// void scaleHist(TH1 & th, Int_t color);


void scaleHist(TH1 & th, Int_t color) {
  if(th->Integral()!=0) {
    scale = 1/th->Integral();      th->Scale(scale);      th->SetLineColor(color);
  }
}


void dqmDisplayMetCompare(TString inputFilename1, TString name1, TString inputFilename2, TString name2)
{
  // filename
  //  TString inputFilename("DQM-histos-19feb10.root");
  //  TString inputFilename(inputFilenameChars);
  TString outputFilename = inputFilename1.Copy();
  outputFilename.Prepend("DisplayMET-");
  outputFilename = "displayMET_test.root";
  // input
  TString inputFileName[2] = {name1, name2};//{inputFilename1, inputFilename2};
  TFile * inputFile1 = TFile::Open(inputFilename1);
  TFile * inputFile2 = TFile::Open(inputFilename2);

  // output
  TFile * outputFile = new TFile(outputFilename,"RECREATE");
 
  inputFile1->cd();
  ewkElecStandaloneDQM->cd();


  gROOT->SetBatch( true );

  // canvases

  const int numTypes = 6;
  TString types[numTypes] = {"CALO","CALOCORR","PF","PFCORR","TC","GEN"};
  TString when[2] = {"BEFORE","AFTER"};
  TString vars[6] = {"ET","EX","EY","PHI","SUMET","ENERGY"};
  TString varsW[4] = {"MT","PT","PHI","RAPIDITY"};

  for( int i = 0; i < numTypes; i++ ) 
    {
      for( int j = 0; j < 2; j++ ) 
	{
	  outputFile->cd();
	  TCanvas * c_Met = new TCanvas("c_"+types[i]+"MET_"+when[j],"Top-level "+ types[i]+"MET histograms "+when[j]+" cuts",1200,800);
	  c_Met->Divide(3,2);
	  for( int v = 0; v < 6; v++ ) 
	    {
	      THStack * s_c1 = new THStack( "s_"+types[i]+"MET_"+vars[v]+"_"+when[j], types[i]+"MET_"+vars[v]+"_"+when[j] );
	      TLegend *leg = new TLegend( 0.69, 0.86, 0.99, 0.99, "" );
	      c_Met->cd(v+1);
	      for(int k = 0; k < 2; k++ ) 
		{
		  TString drawOpt = "";
		  if(k == 0) 
		    {
		      inputFile1->cd("ewkElecStandaloneDQM"); drawOpt = ""; Int_t color = kBlue;
		    }
		  if(k == 1) 
		    {
		      inputFile2->cd("ewkElecStandaloneDQM"); drawOpt = "same"; Int_t color = kRed;
		    }
		  double scale = 1;
		  TH1F * th;
		  th = (TH1F*) gDirectory->Get("MET_"+types[i]+"_"+vars[v]+"_"+when[j]);
		  if(th->Integral()!=0) 
		    {
		      scale = 1/th->Integral();      th->Scale(scale);      th->SetLineColor(color);
		      s_c1->Add(th);
		      leg->AddEntry( th,inputFileName[k],"L") ;
		      //            th->Draw("same");
		    }
		} // k file
	      s_c1->Draw("nostack");  
	      leg->SetFillColor(0);
	      leg->Draw();
	    } // v variable
	  outputFile->WriteTObject(c_Met);
	  c_Met->Print("c_"+types[i]+"MET_"+when[j]+".pdf");
	} // j when
      //}  // i type
      //for( int i = 0; i < numTypes; i++ ) {
      for( int j = 0; j < 2; j++ ) 
	{
	  TH1F * th;
	  TCanvas * c_Ws = new TCanvas("c_W_"+types[i]+"MET_"+when[j],"Top-level "+ types[i]+"W histograms "+when[j]+" cuts",800,800);
	  c_Ws->Divide(2,2);         
	  for( int v = 0; v < 4; v++ ) 
	    {
	      THStack * s_c1 = new THStack( "s_W"+types[i]+"MET_"+varsW[v]+"_"+when[j], "W_"+types[i]+"MET_"+varsW[v]+"_"+when[j] );
	      TLegend *leg = new TLegend( 0.69, 0.86, 0.99, 0.99, "" );
	      c_Ws->cd(v+1);
	      for(int k = 0; k < 2; k++ ) 
		{
		  TString drawOpt = "";
		  if(k == 0) 
		    {
		      inputFile1->cd("ewkElecStandaloneDQM"); drawOpt = ""; Int_t color = kBlue;
		    }
		  if(k == 1) 
		    {
		      inputFile2->cd("ewkElecStandaloneDQM"); drawOpt = "same"; Int_t color = kRed;
		    }
		  double scale = 1;
		  TH1F * th;
		  th = (TH1F*) gDirectory->Get("W_"+types[i]+"MET_"+varsW[v]+"_"+when[j]);    
		  if(th->Integral()!=0) 
		    {
		      scale = 1/th->Integral();      th->Scale(scale);      th->SetLineColor(color);    
		      s_c1->Add(th);
		      leg->AddEntry( th,inputFileName[k],"L") ;
		      //            th->Draw("same");
		    }
		} // k file
	      s_c1->Draw("nostack");
	      leg->SetFillColor(0);
	      leg->Draw();
	    } // v variable
	  outputFile->WriteTObject(c_Ws);
	  c_Ws->Print("c_W_"+types[i]+"MET_"+when[j]+".pdf");
	}
    }
  
  
  gROOT->SetBatch( false );
  // close things
  
  inputFile1->Close();
  inputFile2->Close();
  outputFile->Close();

  // delete leftovers

  for( int i = 0; i < numTypes; i++ ) 
    {
      for( int j = 0; j < 2; j++ ) 
	{
	  TCanvas * c1 = gROOT->FindObject("c_"+types[i]+"MET_"+when[j]);
	  delete c1;
	  TCanvas * c2 = gROOT->FindObject("c_W_"+types[i]+"MET_"+when[j]);
	  delete c2;
	  ;
	}
    }
  
  delete inputFile1;
  delete inputFile2;
  delete outputFile;

  gROOT->SetBatch( false );
}
