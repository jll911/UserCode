//////////////////////////////////////////
//
//  Macro to take plots from output of
//  standalone electron DQM and display 
//  them in a nice format
//
//  Combines plots from multiple files
//  Max number of files is 6 
//  More can be added, modify this macro
//
// Usage:
// (test = output file name)
// root -b -q "dqmDisplayMetCompare.C(\"test\",\"/tmp/grogg/BCtoE_Pt80to170_reRecoPreprod_smaller.root\",\"reRecoPreprod\",1, \"/tmp/grogg/BCtoE_Pt80to170_31X.root\",\"31X\",1)"
// root -b -q "dqmDisplayMetCompare.C(\"test\",\"/tmp/grogg/EMEnriched_Pt80to170_reRecoPreprod_smaller.root\",\"reRecoPreprod\",1, \"/tmp/grogg/EMEnriched_Pt80to170_31X.root\",\"31X\",1)"
//
//////////////////////////////////////////


#include "TCanvas.h"
#include "TFile.h"
#include "TH1F.h"
#include "TROOT.h"
#include "TString.h"
#include <vector> 
#include <iostream>
using std::cout;
using std::endl;

void scaleHist(TH1 & th, Int_t norm);
void plotHist(TH1 & th, THStack & s_c1, TLegend & leg, Int_t norms, Color_t color, TString names);

//Make the number of files comfigurable?  Make only one file possible

void scaleHist(TH1 & th, Int_t norm) {
  double scale = 1;
  if(norm == -1)
    scale = 1;
  if(th.Integral() != 0 && norm == 0) 
    scale = 1/th.Integral();    
  else if(norm != 0)
    scale = 1/(double)norm;
  th.Scale(scale);
}

void plotHist(TH1 & th, THStack & s_c1, TLegend & leg, Int_t norms, Color_t color, TString names) {
  scaleHist(th, norms);
  th.SetLineWidth(2);
  th.SetLineColor(color);
  s_c1.Add(&th);
  leg.AddEntry( &th,names,"L") ;
}


TList *FileList;


void dqmDisplayMetCompare(TString outputFileName, TString inputFileName0, TString name0, Int_t norm0, TString inputFileName1 = "", TString name1 = "", Int_t norm1=0, TString inputFileName2 = "", TString name2 = "", Int_t norm2=0, TString inputFileName3 = "", TString name3 = "", Int_t norm3=0, TString inputFileName4 = "", TString name4 = "", Int_t norm4=0, TString inputFileName5 = "", TString name5 = "", Int_t norm5=0, )
{
  //  TString outputFilename = inputFilename1.Copy();
  //  outputFilename.Prepend("DisplayMET-");
  //TString outputFilename = "displayMET_test.root";
  // input
  TString names[6] = {name0, name1, name2, name3, name4, name5};
  Int_t norms[6] = {norm0, norm1, norm2, norm3, norm4, norm5};
  std::vector<TFile*> inputFiles;
  inputFiles.push_back( TFile::Open(inputFileName0) );
  Int_t numFiles = 1;
  if(inputFileName1!="") {
    inputFiles.push_back( TFile::Open(inputFileName1) );
    numFiles = 2;
  }
  if(inputFileName2!="") {
    inputFiles.push_back( TFile::Open(inputFileName2) );
    numFiles = 3;
  }
  if(inputFileName3!="") {
    inputFiles.push_back( TFile::Open(inputFileName3) );
    numFiles = 4;
  }
  if(inputFileName4!="") {
    inputFiles.push_back( TFile::Open(inputFileName4) );
    numFiles = 5;
  }
  if(inputFileName5!="") {
    inputFiles.push_back( TFile::Open(inputFileName5) );
    numFiles = 6;
  }
  

  
  FileList = new TList();
  
  // output
  TFile * outputFile = new TFile("displayMet_"+outputFileName+".root","RECREATE");
  TString pdfDir = outputFileName + "Pdfs/";
  system("mkdir " + pdfDir);
  
  gROOT->SetBatch( true );

  // canvases
  const int numTypes = 6;
  TString types[numTypes] = {"CALO","CALOCORR","PF","PFCORR","TC","GEN"};
  TString when[2] = {"BEFORE","AFTER"};
  const int nVars = 6;
  TString vars[nVars] = {"ET","EX","EY","PHI","SUMET","SIG"};

  const int nVarsCalo = 11;
  TString varsCalo[nVarsCalo] = {"HADETINHB","HADETINHE","HADETINHF","HADETINHO","MAXETINEMTOWERS",
			 "MAXETINHADTOWERS","EMETFRACTION","ETFRACTIONHADRONIC",
			 "EMETINEB","EMETINEE","EMETINHF"};
  const int nVarsPf = 5;
  TString varsPf[nVarsPf] = {"CHARGEDEMFRAC","CHARGEDHADFRAC","MUONFRAC","NEUTRALEMFRAC","NEUTRALHADFRAC"};
//   TString varsTc[0] = {};
//   TString varsGen[0] = {};
  TString varsW[4] = {"MT","PT","PHI","RAPIDITY"};
  Int_t color[] = {kBlue,kRed,kGreen+1,kBlack,kMagenta,kCyan,kYellow};
  
  for( int i = 0; i < numTypes; i++ ) 
    {
      for( int j = 0; j < 2; j++ ) 
	{
	  //MET plots
	  outputFile->cd();
	  TCanvas * c_Met = new TCanvas("c_"+types[i]+"MET_"+when[j],"Top-level "+ types[i]+"MET histograms "+when[j]+" cuts",1200,800);
	  c_Met->Divide(3,2);
	  for( int v = 0; v < nVars; v++ ) 
	    {
	      THStack * s_c1 = new THStack( "s_"+types[i]+"MET_"+vars[v]+"_"+when[j], types[i]+"MET_"+vars[v]+"_"+when[j] );
	      TLegend *leg = new TLegend( 0.69, 0.86, 0.99, 0.99, "" );
	      c_Met->cd(v+1);
	      
	      for(int k = 0; k < numFiles; k++ ) 
		{
		  inputFiles[k]->cd("ewkElecStandaloneDQM");
		  TH1F * th;
		  th = (TH1F*) gDirectory->Get("MET_"+types[i]+"_"+vars[v]+"_"+when[j]);		  
		  plotHist(*th,*s_c1,*leg, norms[k],color[k],names[k]);
		} // k file
	      s_c1->Draw("nostack");  
	      leg->SetFillColor(0);
	      leg->Draw();
	    } // v variable
	  outputFile->WriteTObject(c_Met);
	  c_Met->Print("c_"+types[i]+"MET_"+when[j]+".pdf");
	  if(types[i] == "CALO") 
	    {
	      TCanvas * c_MetCalo = new TCanvas("c_"+types[i]+"MET_Extras_"+when[j],"Details "+ types[i]+"MET histograms "+when[j]+" cuts",1200,800);
	      c_MetCalo->Divide(4,3);
	      for( int v = 0; v < nVarsCalo; v++ ) 
		{
		  THStack * s_c1 = new THStack( "s_"+types[i]+"MET_"+varsCalo[v]+"_"+when[j], types[i]+"MET_"+varsCalo[v]+"_"+when[j] );
		  TLegend *leg = new TLegend( 0.69, 0.86, 0.99, 0.99, "" );
		  c_MetCalo->cd(v+1);
	      
		  for(int k = 0; k < numFiles; k++ ) 
		    {
		      inputFiles[k]->cd("ewkElecStandaloneDQM");
		      TH1F * th;
		      th = (TH1F*) gDirectory->Get("MET_"+types[i]+"_"+varsCalo[v]+"_"+when[j]);
		      plotHist(*th,*s_c1,*leg, norms[k],color[k],names[k]);
		    } // k file
		  s_c1->Draw("nostack");  
		  leg->SetFillColor(0);
		  leg->Draw();
		} // v variable
	      outputFile->WriteTObject(c_MetCalo);
	      c_MetCalo->Print("c_"+types[i]+"MET_Extras_"+when[j]+".pdf");
	    }
	  if(types[i] == "PF") 
	    {
	      TCanvas * c_MetPf = new TCanvas("c_"+types[i]+"MET_Extras_"+when[j],"Details "+ types[i]+"MET histograms "+when[j]+" cuts",1200,800);
	      c_MetPf->Divide(3,2);
	      for( int v = 0; v < nVarsPf; v++ ) 
		{
		  THStack * s_c1 = new THStack( "s_"+types[i]+"MET_"+varsPf[v]+"_"+when[j], types[i]+"MET_"+varsPf[v]+"_"+when[j] );
		  TLegend *leg = new TLegend( 0.69, 0.86, 0.99, 0.99, "" );
		  c_MetPf->cd(v+1);
		  for(int k = 0; k < numFiles; k++ ) 
		    {
		      inputFiles[k]->cd("ewkElecStandaloneDQM");
		      TH1F * th;
		      // 	  cout << "MET_"<<types[i]<<"_"<<varsPf[v]<<"_"<<when[j] << endl;
		      th = (TH1F*) gDirectory->Get("MET_"+types[i]+"_"+varsPf[v]+"_"+when[j]);
		      plotHist(*th,*s_c1,*leg, norms[k],color[k],names[k]);
		    } // k file
		  s_c1->Draw("nostack");  
		  leg->SetFillColor(0);
		  leg->Draw();
		} // v variable
	      outputFile->WriteTObject(c_MetPf);
	      c_MetPf->Print("c_"+types[i]+"MET_Extras_"+when[j]+".pdf");
	    }
	  //W plots
	  outputFile->cd();
	  TCanvas * c_Ws = new TCanvas("c_W_"+types[i]+"MET_"+when[j],"Top-level "+ types[i]+"W histograms "+when[j]+" cuts",800,800);
	  c_Ws->Divide(2,2);
	  for( int v = 0; v < 4; v++ ) 
	    {
	      THStack * s_c1 = new THStack( "s_W"+types[i]+"MET_"+varsW[v]+"_"+when[j], "W_"+types[i]+"MET_"+varsW[v]+"_"+when[j] );
	      TLegend *leg = new TLegend( 0.69, 0.86, 0.99, 0.99, "" );
	      c_Ws->cd(v+1);
	      for(int k = 0; k < numFiles; k++ ) 
		{
		  inputFiles[k]->cd("ewkElecStandaloneDQM");
		  TH1F * th;
		  th = (TH1F*) gDirectory->Get("W_"+types[i]+"MET_"+varsW[v]+"_"+when[j]);
		  plotHist(*th,*s_c1,*leg, norms[k],color[k],names[k]);
		} // k file
	      s_c1->Draw("nostack");  
	      leg->SetFillColor(0);
	      leg->Draw();
	    } // v variable
	  outputFile->WriteTObject(c_Ws);
	  c_Ws->Print("c_W_"+types[i]+"MET_"+when[j]+".pdf");
      
	} // j when
    }
  gROOT->SetBatch( false );
  // close things
  for(int k = 0; k < numFiles; k++ ) 
    {//
      inputFiles[k]->Close();
      delete inputFiles[k];
    }
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

  delete outputFile;

  gROOT->SetBatch( false );
  
  //combine all pdfs into one
  system("gs -dNOPAUSE -sDEVICE=pdfwrite -sOUTPUTFILE="+outputFileName+".pdf -dBATCH c_*.pdf");
  system("mv c_*.pdf " + pdfDir);
  
}
