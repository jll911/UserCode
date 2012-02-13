// given a candidate vs energy distribution, 
// integrates the distribution. 
// normalizes according to total luminosity and effective luminosity

#include <iostream>
using std::cout;
using std::endl;

#include "TFile.h"
#include "TH1F.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TGaxis.h"
#include "TLegend.h"

// SetLineColor args
// 1   black
// 2   red
// 3   green
// 4   dark blue
// 5   yellow
// 6   magenta
// 7
// 8


TString makeName(TString name, int number)
{
  TString dummyString(name);
  dummyString.Append("[");
  dummyString += number;
  dummyString.Append("]");
  return dummyString;
}


TString makeName(TString name, int number1, int number2)
{
  TString dummyString(name);
  dummyString.Append("[");
  dummyString += number1;
  dummyString.Append("][");
  dummyString += number2;
  dummyString.Append("]");
  return dummyString;
}


Int_t integrateRatePlots()
{
  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(0);
  //  gStyle->SetOptTitle(0);
  TGaxis::SetMaxDigits(3);

  const Int_t nFiles = 3;
  //  const Int_t nElectrons = 3;

  TFile * histoFile = new TFile("integratedRates-eta2p1-26Jan12.root","RECREATE");
  TFile * inputFile[nFiles]; 

  inputFile[0] = TFile::Open("RctEmTree-Rates-scaleV1-thr2p0-01Dec11.root"); // v1, 2.0, BOTH ISO AND NON-ISO
  inputFile[1] = TFile::Open("RctEmTree-Rates-scaleV3-thr1p0-12Jan12.root"); // v3, 1.0, iso
  inputFile[2] = TFile::Open("RctEmTree-Rates-HighPU-scaleV3-thr1p0-18Jan12.root"); // v3, 1.0, highPU, iso

  TH1F * h_rank[nFiles], * h_rankIntegrated[nFiles];
  TH1F * h_rankBarrel[nFiles], * h_rankBarrelIntegrated[nFiles];

  // scale factor here is 1./(total n LS * 23 s/LS) * (total lumi / effective lumi) to get from total event counts to rate (event count per second)
  Float_t scaleFactor[3];
  scaleFactor[0] = 14.2 * 1./(15379.*23.) * (713729./68.794); 
  scaleFactor[1] = 1./(15379.*23.) * (713729./68.794); 
  scaleFactor[2] = 1./(299. * 23.) * (299.556 / 49.926) * (201.78 / 4.3559); // scaled up to 2e33 inst lumi from 4e31 -- those last two numbers are times 10^31 

  // tree stuff
  TTree * tree;
  vector<int>     *emRank;
  vector<int>     *emIso;
  vector<int>     *emIeta;
  TBranch        *b_emRank;
  TBranch        *b_emIso;
  TBranch        *b_emIeta;

  // end tree stuff

//   Int_t nBins = 64;
//   Float_t xMin = 0., xMax = 64.;
  Int_t nBins = 52;
  Float_t xMin = 12., xMax = 64.;

  // |eta| < 1.5, barrel-only
//   Int_t barrelIEtaBoundaryLower = 7; // this is REGION ieta -- goes from 4 to 17, not including HF.  
//   Int_t barrelIEtaBoundaryUpper = 14; // this takes first four regions (last 3 are endcap)

// |eta| < 2.1  
  Int_t barrelIEtaBoundaryLower = 8; // this is REGION ieta -- goes from 4 to 17, not including HF.  
  Int_t barrelIEtaBoundaryUpper = 13; // this takes first three regions (first 24 towers), going to eta 2.1720
  
  TCanvas * rankCanvas = new TCanvas("rankCanvas","EM rank distribution");
  TCanvas * rankBarrelCanvas = new TCanvas("rankBarrelCanvas","EM rank distribution (|#eta| < 2.1)");

  TCanvas * integratedCanvas = new TCanvas("integratedCanvas","Rate by trigger threshold");
  integratedCanvas->cd();
  TPad * pad1 = new TPad("pad1","pad1",0.,0.35,1.0,1.0);
  pad1->SetBottomMargin(0);
  pad1->Draw();
  TPad * pad2 = new TPad("pad2","pad2",0.,0.,1.0,0.35);
  pad2->SetTopMargin(0);
  pad2->SetBottomMargin(0.3);
  pad2->Draw();

  TCanvas * integratedBarrelCanvas = new TCanvas("integratedBarrelCanvas","Rate by trigger threshold (|#eta| < 2.1)");
  integratedBarrelCanvas->cd();
  TPad * pad1B = new TPad("pad1B","pad1B",0.,0.35,1.0,1.0);
  pad1B->SetBottomMargin(0);
  pad1B->Draw();
  TPad * pad2B = new TPad("pad2B","pad2B",0.,0.,1.0,0.35);
  pad2B->SetTopMargin(0);
  pad2B->SetBottomMargin(0.3);
  pad2B->Draw();


  for (Int_t file = 0; file < nFiles; file++)
    {
      std::cout << "File " << file << std::endl;
      
      inputFile[file]->cd();

      TString treeName = TString("rctAnalyzer/emTree;1");

      std::cout << "getting tree" << std::endl;
      gDirectory->GetObject(treeName,tree);

      //   std::cout << "X min = " << h_rank->GetXaxis()->GetXmin() << std::endl;
      //   std::cout << "X max = " << h_rank->GetXaxis()->GetXmax() << std::endl;
      
      //   Int_t nBins = h_rank->GetNbinsX();
      
      //   h_rank = new TH1F("h_rank","EM Rank",nBins,h_rank->GetXaxis()->GetXmin(),h_rank->GetXaxis()->GetXmax());
      //   h_rankIntegrated = new TH1F("h_rankIntegrated","Rates by trigger threshold",nBins,h_rank->GetXaxis()->GetXmin(),h_rank->GetXaxis()->GetXmax());
      std::cout << "making histograms" << std::endl;
      
      h_rank[file] = new TH1F(makeName("h_rank",file),"EM Rank",nBins,xMin,xMax);
      h_rankIntegrated[file] = new TH1F(makeName("h_rankIntegrated",file),"Rates by trigger threshold",nBins,xMin,xMax);
      h_rankBarrel[file] = new TH1F(makeName("h_rankBarrel",file),"EM Rank (|#eta| < 2.1)",nBins,xMin,xMax);
      h_rankBarrelIntegrated[file] = new TH1F(makeName("h_rankBarrelIntegrated",file),"Rates by trigger threshold (|#eta| < 2.1)",nBins,xMin,xMax);
      
      rankCanvas->cd();

      // tree stuff
      emRank = 0;
      emIso = 0;
      emIeta = 0;
      
      tree->SetBranchAddress("emRank", &emRank, &b_emRank);
      tree->SetBranchAddress("emIso", &emIso, &b_emIso);
      tree->SetBranchAddress("emIeta", &emIeta, &b_emIeta);
      b_emRank->SetAutoDelete(kTRUE);
      b_emIso->SetAutoDelete(kTRUE);
      b_emIeta->SetAutoDelete(kTRUE);

      tree->SetBranchStatus("*",0);
      tree->SetBranchStatus("emRank",1);
      tree->SetBranchStatus("emIso",1);
      tree->SetBranchStatus("emIeta",1);

      // loop over tree
      std::cout << "Looping over tree" << std::endl;
      Long64_t nentries = tree->GetEntriesFast();
      for (Long64_t jentry = 0; jentry < nentries; jentry++) 
	{
	  //std::cout << "entry " << jentry << std::endl;
	  //Long64_t ientry = tree->LoadTree(jentry);
	  //tree->LoadTree(jentry); // Set current entry.
	  tree->GetEntry(jentry); // Read all branches of entry and return total number of bytes read.
	  
	  Int_t elecSize = emRank->size();
	  for (Int_t elec = 0; elec < elecSize; elec++)
	    {
	      if (file == 0) // isolated and non-isolated
		{
		  h_rank[file]->Fill(emRank->at(elec));
		  if (emIeta->at(elec) >= barrelIEtaBoundaryLower &&
		      emIeta->at(elec) <= barrelIEtaBoundaryUpper)
		    {
		      h_rankBarrel[file]->Fill(emRank->at(elec));
		    }
		  break;
		}
	      else // isolated only
		{
		  if (emIso->at(elec) == 1)
		    {
		      h_rank[file]->Fill(emRank->at(elec));
		      if (emIeta->at(elec) >= barrelIEtaBoundaryLower &&
			emIeta->at(elec) <= barrelIEtaBoundaryUpper)
			{
			  h_rankBarrel[file]->Fill(emRank->at(elec));
			}
		      break;
		    }
		}
	    }
	}
      
      // end tree stuff
      
      // making pretty colors for the lines in the rank plots
      h_rank[file]->SetLineColor(file+1);
      if (file+1 == 3)
	{
	  h_rank[file]->SetLineColor(8);
	}
      if (file+1 == 5)
	{
	  h_rank[file]->SetLineColor(41);
	}
      h_rank[file]->SetLineWidth(2);

      h_rankBarrel[file]->SetLineColor(file+40);
      h_rankBarrel[file]->SetLineWidth(2);

      if (file == 0)
	{
	  h_rank[file]->Draw();
	}
      else
	{
	  h_rank[file]->Draw("same");
	  h_rankBarrel[file]->Draw("same");
	}

      rankBarrelCanvas->cd();
      if (file == 0)
	{
	  h_rankBarrel[file]->Draw();
	}
      else
	{
	  h_rankBarrel[file]->Draw("same");
	}

      // doing the integration      
      Int_t eventsAboveThreshold = 0;
      for (int i = nBins-1; i >= 0; i--)
	{
	  eventsAboveThreshold = eventsAboveThreshold + h_rank[file]->GetBinContent(i);
	  std::cout << "bin is " << i << ", bin content is " << h_rank[file]->GetBinContent(i)
		    << ", eventsAboveThreshold is " << eventsAboveThreshold << std::endl;
	  h_rankIntegrated[file]->Fill(xMin+i,eventsAboveThreshold);
	}
      eventsAboveThreshold = 0;
      for (int i = nBins-1; i >= 0; i--)
	{
	  eventsAboveThreshold = eventsAboveThreshold + h_rankBarrel[file]->GetBinContent(i);
	  std::cout << "bin is " << i << ", bin content is " << h_rankBarrel[file]->GetBinContent(i)
		    << ", eventsAboveThreshold is " << eventsAboveThreshold << std::endl;
	  h_rankBarrelIntegrated[file]->Fill(xMin+i,eventsAboveThreshold);
	}
      std::cout << "sumw2" << std::endl;
      h_rankIntegrated[file]->Sumw2();
      h_rankIntegrated[file]->Scale(scaleFactor[file]);
      h_rankBarrelIntegrated[file]->Sumw2();
      h_rankBarrelIntegrated[file]->Scale(scaleFactor[file]);
      
      // more pretty colors, this time for integrated plots
      h_rankIntegrated[file]->SetLineColor(file+1);
      if (file+1 == 3)
	{
	  h_rankIntegrated[file]->SetLineColor(8);
	}
      if (file+1 == 5)
	{
	  h_rankIntegrated[file]->SetLineColor(41);
	}
      h_rankIntegrated[file]->SetLineWidth(2);

      if (file == 1)
	{
	  h_rankBarrelIntegrated[file]->SetLineColor(kRed+2);
	}
      if (file == 2)
	{
	  h_rankBarrelIntegrated[file]->SetLineColor(kYellow+2);
	}
      h_rankBarrelIntegrated[file]->SetLineWidth(2);

      integratedCanvas->cd();
      pad1->cd();
      std::cout << "Draw" << std::endl;
      if(file == 0)
	{
	  h_rankIntegrated[file]->Draw();
	}
      else
	{
	  h_rankIntegrated[file]->Draw("same");
	  h_rankBarrelIntegrated[file]->Draw("same");
	}

      integratedBarrelCanvas->cd();
      pad1B->cd();
      std::cout << "Draw" << std::endl;
      if(file == 0)
	{
	  h_rankBarrelIntegrated[file]->Draw();
	}
      else
	{
	  h_rankBarrelIntegrated[file]->Draw("same");
	}
      std::cout << "Done drawing" << std::endl;
      
    }

  std::cout << "Making legend" << std::endl;

  TLegend legend(0.3,0.6,0.89,0.89);
  legend.AddEntry(h_rankIntegrated[0],"v1 scales, thr = 2.0, Relaxed");
  legend.AddEntry(h_rankIntegrated[1],"v3 scales, thr = 1.0, Isolated");
  legend.AddEntry(h_rankBarrelIntegrated[1],"v3 scales, thr = 1.0, Isolated, |#eta| < 2.1");
  legend.AddEntry(h_rankIntegrated[2],"High-PU, v3 scales, thr = 1.0, Isolated");
  legend.AddEntry(h_rankBarrelIntegrated[2],"High-PU, v3 scales, thr = 1.0, Isolated, |#eta| < 2.1");
  legend.SetFillColor(0);

  integratedCanvas->cd();
  pad1->cd();
  legend.Draw("same");

  integratedBarrelCanvas->cd();
  pad1B->cd();
  legend.Draw("same");


  std::cout << "Making ratio plots" << std::endl;

  integratedCanvas->cd();
  pad2->cd();
  TH1F * r_eff1;
  TH1F * r_eff2;
  r_eff1 = (TH1F*) h_rankIntegrated[1]->Clone("r_eff1");
  r_eff2 = (TH1F*) h_rankIntegrated[2]->Clone("r_eff2");
  r_eff1->Divide(h_rankIntegrated[0]);
  r_eff2->Divide(h_rankIntegrated[0]);
  r_eff1->SetTitle("");
  r_eff2->SetTitle("");
  r_eff1->GetXaxis()->SetRangeUser(xMin,xMax);
  r_eff2->GetXaxis()->SetRangeUser(xMin,xMax);
  r_eff1->SetMinimum(0.3);
  r_eff1->SetMaximum(1.0);
  r_eff2->SetMinimum(0.3);
  r_eff2->SetMaximum(1.0);
  r_eff1->GetXaxis()->SetTitle("Trigger threshold (GeV)");
  r_eff1->GetXaxis()->SetLabelSize(0.07);
  r_eff1->GetXaxis()->SetTitleSize(0.07);
  r_eff1->GetYaxis()->SetLabelSize(0.07);
  r_eff2->GetXaxis()->SetTitle("Trigger threshold (GeV)");
  r_eff2->GetXaxis()->SetLabelSize(0.07);
  r_eff2->GetXaxis()->SetTitleSize(0.07);
  r_eff2->GetYaxis()->SetLabelSize(0.07);
  r_eff1->Draw();
  r_eff2->Draw("same");

  integratedBarrelCanvas->cd();
  pad2B->cd();
  TH1F * r_eff1B;
  TH1F * r_eff2B;
  r_eff1B = (TH1F*) h_rankBarrelIntegrated[1]->Clone("r_eff1B");
  r_eff2B = (TH1F*) h_rankBarrelIntegrated[2]->Clone("r_eff2B");
  r_eff1B->Divide(h_rankIntegrated[0]);
  r_eff2B->Divide(h_rankIntegrated[0]);
  r_eff1B->SetTitle("");
  r_eff2B->SetTitle("");
  r_eff1B->GetXaxis()->SetRangeUser(xMin,xMax);
  r_eff2B->GetXaxis()->SetRangeUser(xMin,xMax);
  r_eff1B->SetMinimum(0.3);
  r_eff1B->SetMaximum(1.0);
  r_eff2B->SetMinimum(0.3);
  r_eff2B->SetMaximum(1.0);
  r_eff1B->GetXaxis()->SetTitle("Trigger threshold (GeV)");
  r_eff1B->GetXaxis()->SetLabelSize(0.07);
  r_eff1B->GetXaxis()->SetTitleSize(0.07);
  r_eff1B->GetYaxis()->SetLabelSize(0.07);
  r_eff2B->GetXaxis()->SetTitle("Trigger threshold (GeV)");
  r_eff2B->GetXaxis()->SetLabelSize(0.07);
  r_eff2B->GetXaxis()->SetTitleSize(0.07);
  r_eff2B->GetYaxis()->SetLabelSize(0.07);
  r_eff1B->Draw();
  r_eff2B->Draw("same");

  integratedCanvas->cd();
  r_eff1B->Draw("same");
  r_eff2B->Draw("same");

  TLine * unity;
  unity = new TLine(0.,1.0,64.,1.0);
  unity->SetX1(gPad->PadtoX(xMin));
  unity->SetX2(gPad->PadtoX(xMax));
  unity->SetLineColor(kBlue);

  integratedCanvas->cd();
  pad2->cd();
  r_eff1B->Draw("same");
  r_eff2B->Draw("same");
  unity->Draw("same");

  integratedBarrelCanvas->cd();
  pad2B->cd();
  unity->Draw("same");

  std::cout << "Writing canvases to file" << std::endl;
  histoFile->WriteTObject(rankCanvas);
  histoFile->WriteTObject(rankBarrelCanvas);
  histoFile->WriteTObject(integratedCanvas);
  histoFile->WriteTObject(integratedBarrelCanvas);

  std::cout << "Closing input files" << std::endl;
  for (Int_t i = 0; i < nFiles; i++)
    {
      inputFile[i]->Close();
    }

  std::cout << "Deleting canvases" << std::endl;

  delete rankCanvas;
  delete rankBarrelCanvas;
  delete pad1;
  delete pad2;
  delete integratedCanvas;
  delete pad1B;
  delete pad2B;
  delete integratedBarrelCanvas;

  return 0;

}
