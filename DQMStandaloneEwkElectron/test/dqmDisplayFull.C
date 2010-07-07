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


// common stuff

TFile * inFile;
TFile * referenceFile;

void SavePlot(const char* filename);
void drawHistogram(TString histoName, TString imageName, bool doScaling = true);
void drawEfficiency(TString histoName, TString denomName, TString imageName, Double_t lowBins[], const int binArraySize);
//void drawEfficiency2D(TString histoName, TString denomName, TString imageName, Double_t lowBinsX[], const int binArraySizeX, Double_t lowBinsY[], const int binArraySizeY);


// jet stuff

//enum { ETA_BELOW_THREE, ETA_OVER_THREE, ETA_ALL };
enum { ETA_ALL, ETA_BELOW_THREE, ETA_OVER_THREE };

TString canvases[5] = { "COMMON", "ONLY", "ENERGY", "ENERGYFRACTION", "MULTIPLICITIES" };
TString jetCollections[5] = { "PF", "CALO", "PFCORR", "CALOCORR", "GEN" };
TString whereEta[3] = { "ALLETA", "ETABELOW3", "ETAOVER3" };
TString which[2] = { "ALL", "HIGHEST" };
TString when[2] = { "BEFORE", "AFTER" };

void createCommonCanvas(TString jetCollection, unsigned int etaRegion, TString numJets, TString beforeAfter );
void createCaloCanvas(TString jetCollection, unsigned int etaRegion, TString numJets, TString beforeAfter );
void createPfEnergyCanvas(TString jetCollection, unsigned int etaRegion, TString numJets, TString beforeAfter );
void createPfEnergyFractionCanvas(TString jetCollection, unsigned int etaRegion, TString numJets, TString beforeAfter );
void createPfMultiplicityCanvas(TString jetCollection, unsigned int etaRegion, TString numJets, TString beforeAfter );
void addPlot(TString jetCollection, TString variable, unsigned int etaRegion, TString numJets, TString beforeAfter, TString title);
void getJetPath(TString&  toReturn, TString jetCollection, unsigned int etaRegion);

// electron stuff

Float_t scaleFactor;

void makeElectronTopLevelPlots();
void makeElectronKinematicsPlots();
void makeElectronShowerShapePlots();
void makeElectronHoEPlots();
void makeElectronIsolationPlots();
void makeElectronTrackExtrapolationPlots();
void makeElectronTrackClusterMatchPlots();
void makeElectronVertexPlots();
void makeElectronChargePlots();
void makeElectronMiscPlots();

void makeZPlots();

void makeSuperClusterPlots();

void makeTriggerPlots();

void makeMetPlots();

void makeJetPlots();


// The function that gets everything going

void dqmDisplayFull(TString inputFilename, TString referenceFilename)
{

  //  G__loadfile("setTDRStyleMine.h");
  G__loadfile("setTDRStyle.h");
  setTDRStyle();
  //  gStyle->SetOptStat(110010);
  gStyle->SetOptStat(111110);
  //  gStyle->SetTitleY(1.05);

  // input
  inFile = TFile::Open(inputFilename);

  // reference to compare to
  referenceFile = TFile::Open(referenceFilename);

  gROOT->SetBatch( true );

  TCanvas * cPlot = new TCanvas("cPlot","cPlot",600,400);
  
  makeElectronTopLevelPlots();
  makeElectronKinematicsPlots();
  makeElectronShowerShapePlots();
  makeElectronHoEPlots();
  makeElectronIsolationPlots();
  makeElectronTrackExtrapolationPlots();
  makeElectronTrackClusterMatchPlots();
  makeElectronVertexPlots();
  makeElectronChargePlots();
  makeElectronMiscPlots();
  makeSuperClusterPlots();
  
  makeZPlots();

  makeMetPlots();
  
  makeJetPlots();

  makeTriggerPlots();

  inFile->Close();

  //  delete inputFile;

  gROOT->SetBatch( false );

}


void SavePlot(const char* filename)
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



void drawHistogram(TString histoName, TString imageName, bool doScaling)
{
  scaleFactor = 1;
  TH1F * hist1 = dynamic_cast<TH1F*>(inFile->Get("ewkElecStandaloneDQM/"+histoName));
  TH1F * hist2 = dynamic_cast<TH1F*>(referenceFile->Get("ewkElecStandaloneDQM/"+histoName));

  if (doScaling)
    {
      Double_t nEvents = hist1->GetEntries();
      Double_t nRefEvents = hist2->GetEntries();
  
      if (nRefEvents != 0)
	{
	  scaleFactor = nEvents / nRefEvents;
	}
      else
	{
	  scaleFactor = 1;
	}
  
      hist2->Scale(scaleFactor);
    }
  
  //STACK
  THStack * s_1 = new THStack( "s_1", hist1->GetTitle() );

  hist1->Sumw2();
  hist1->SetMarkerColor(1);
  hist1->SetMarkerStyle(8);
  hist1->SetMarkerSize(0.8);
  //hist1->Draw("ep");
  s_1->Add(hist1);
  
  hist2->SetLineColor(4);
  hist2->SetLineWidth(2);
  //hist2->Draw("same");
  s_1->Add(hist2);

  s_1->Draw("nostack");
  TAxis * histXAxis = (TAxis*) hist1->GetXaxis();
  TAxis * stackXAxis = (TAxis*) s_1->GetXaxis();
  stackXAxis->SetTitle(histXAxis->GetTitle());
  s_1->Draw("nostack");

  SavePlot(imageName);
}

void drawEfficiency(TString histoName, TString denomName, TString imageName, Double_t lowBins[], const int binArraySize)
{
  scaleFactor = 1;

  //  Double_t lowBins[17] = {0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 24, 28, 32, 40, 48, 60};
  //  Double_t dummyBins[17] = {0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 24, 28, 32, 40, 48, 60};
  //  Int_t nBins = sizeof(*lowBins)/sizeof(Double_t);
//   Int_t nBins = sizeof(dummyBins)/sizeof(Double_t);
//   std::cout << "Size of array is " << sizeof(dummyBins) << ", size of type Double_t is " << sizeof(Double_t) << std::endl;
//  Int_t nBins = sizeof(lowBins)/sizeof(Double_t);
  Int_t nBins = (Int_t) binArraySize - 1;
  //  std::cout << "Size of array is " << sizeof(lowBins) << ", size of type Double_t is " << sizeof(Double_t) << std::endl;
  //  std::cout << "Number of elements in array is " << nBins << ", number of bins to rebin to is " << nBins-1 << std::endl;

  TH1F * hist1 = dynamic_cast<TH1F*>(inFile->Get("ewkElecStandaloneDQM/"+histoName));
  //  TH1F * histNew1 = hist1->Rebin(16,"histNew1",lowBins);  
  TH1F * histNew1 = hist1->Rebin(nBins,"histNew1",lowBins);  
  TH1F * hist2 = dynamic_cast<TH1F*>(referenceFile->Get("ewkElecStandaloneDQM/"+histoName));
  //  TH1F * histNew2 = hist2->Rebin(16,"histNew2",lowBins);
  TH1F * histNew2 = hist2->Rebin(nBins,"histNew2",lowBins);

  //  TH1F * denom1 =  dynamic_cast<TH1F*>(inFile->Get("ewkElecStandaloneDQM/TRG_DENOMINATOR_EFF"));
  TH1F * denom1 =  dynamic_cast<TH1F*>(inFile->Get("ewkElecStandaloneDQM/"+denomName));
  //  denom1->Rebin(2);
  //  TH1F * denomNew1 = denom1->Rebin(16,"denomNew1",lowBins);
  TH1F * denomNew1 = denom1->Rebin(nBins,"denomNew1",lowBins);
  //  TH1F * denom2 =  dynamic_cast<TH1F*>(referenceFile->Get("ewkElecStandaloneDQM/TRG_DENOMINATOR_EFF"));
  TH1F * denom2 =  dynamic_cast<TH1F*>(referenceFile->Get("ewkElecStandaloneDQM/"+denomName));
  //  denom2->Rebin(2);
  //  TH1F * denomNew2 = denom2->Rebin(16,"denomNew2",lowBins);
  TH1F * denomNew2 = denom2->Rebin(nBins,"denomNew2",lowBins);



//   if (doScaling)
//     {
//       Double_t nEvents = hist1->GetEntries();
//       Double_t nRefEvents = hist2->GetEntries();
  
//       if (nRefEvents != 0)
// 	{
// 	  scaleFactor = nEvents / nRefEvents;
// 	}
//       else
// 	{
// 	  scaleFactor = 1;
// 	}
  
//       hist2->Scale(scaleFactor);
//     }
  
  //STACK
  THStack * s_1 = new THStack( "s_1", histNew1->GetTitle() );


  histNew1->SetMarkerColor(1);
  histNew1->SetMarkerStyle(8);
  histNew1->SetMarkerSize(0.8);
  //  histNew1->Rebin(2);  // REBIN
  //  denomNew1->Rebin(2); // REBIN
  histNew1->Sumw2();
  denomNew1->Sumw2();
  //  histNew1->Divide(denomNew1);
  TGraphAsymmErrors * gr = new TGraphAsymmErrors(); 
  gr->BayesDivide(histNew1, denomNew1,"w");
  gr->SetMarkerColor(1);
  gr->SetMarkerStyle(8);
  gr->SetMarkerSize(0.8);

  //  s_1->Add(histNew1);
  
  histNew2->SetLineColor(4);
  histNew2->SetLineWidth(2);
  //  histNew2->Rebin(2);  // REBIN
  //  denomNew2->Rebin(2); // REBIN
  histNew2->Divide(denomNew2);
  s_1->Add(histNew2);
  //  s_1->GetXaxis()->SetAxisTitle(hist1->GetXaxis()->GetAxisTitle);

  s_1->Draw("nostack");
  TAxis * histXAxis = (TAxis*) histNew1->GetXaxis();
  TAxis * stackXAxis = (TAxis*) s_1->GetXaxis();
  stackXAxis->SetTitle(histXAxis->GetTitle());

  s_1->Draw("nostack");
  gr->Draw("zp");

  SavePlot(imageName);
}


// void drawEfficiency2D(TString histoName, TString denomName, TString imageName, Double_t lowBinsX[], const int binArraySizeX, Double_t lowBinsY[], const int binArraySizeY)
// {
//   scaleFactor = 1;

//   //  Double_t lowBins[17] = {0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 24, 28, 32, 40, 48, 60};
//   //  Double_t dummyBins[17] = {0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 24, 28, 32, 40, 48, 60};
//   //  Int_t nBins = sizeof(*lowBins)/sizeof(Double_t);
// //   Int_t nBins = sizeof(dummyBins)/sizeof(Double_t);
// //   std::cout << "Size of array is " << sizeof(dummyBins) << ", size of type Double_t is " << sizeof(Double_t) << std::endl;
// //  Int_t nBins = sizeof(lowBins)/sizeof(Double_t);
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



// //   if (doScaling)
// //     {
// //       Double_t nEvents = hist1->GetEntries();
// //       Double_t nRefEvents = hist2->GetEntries();
  
// //       if (nRefEvents != 0)
// // 	{
// // 	  scaleFactor = nEvents / nRefEvents;
// // 	}
// //       else
// // 	{
// // 	  scaleFactor = 1;
// // 	}
  
// //       hist2->Scale(scaleFactor);
// //     }
  
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


void
createCommonCanvas(TString jetCollection, unsigned int etaRegion, TString numJets, TString beforeAfter )
{
    if ( ! ( etaRegion == ETA_BELOW_THREE || etaRegion == ETA_OVER_THREE || etaRegion == ETA_ALL ) )
    {
        cerr << "ERROR: Invalid eta region requested! Requested: " << jetCollection << "_COMMON_" << etaRegion << "_" << numJets << "_" << beforeAfter << endl;
        return;
    }
    
    TString label = "JET_"+jetCollection+"_COMMON_"+whereEta[etaRegion]+"_"+numJets+"_"+beforeAfter;
    
    if ( ! inFile->Get("ewkElecStandaloneDQM/JET_"+jetCollection+"_ENERGY_ETABELOW3_"+numJets+"_"+beforeAfter) )
    {
        cerr << "ERROR: Could not create canvas for " << label << endl;
        return;
    }
    
    cout << label  << endl;

    TCanvas* canvas = new TCanvas(label,label, 1200, 800);
    canvas->Divide(3,2);
    
    canvas->cd(1);
    addPlot(jetCollection, "ENERGY", etaRegion, numJets, beforeAfter, "energy");
    
    canvas->cd(2);
    addPlot(jetCollection, "ET", etaRegion, numJets, beforeAfter, "E_{T}");
    
    canvas->cd(3);
    addPlot(jetCollection, "PT", etaRegion, numJets, beforeAfter, "P_{T}");
    
    canvas->cd(4);
    addPlot(jetCollection, "ETA", etaRegion, numJets, beforeAfter, "#eta");
    
    canvas->cd(5);
    addPlot(jetCollection, "PHI", etaRegion, numJets, beforeAfter, "#phi");   

    canvas->cd();
    
    TString path;
    getJetPath(path, jetCollection, etaRegion);
    SavePlot(label.Prepend(path));
}





void 
createCaloCanvas(TString jetCollection, unsigned int etaRegion, TString numJets, TString beforeAfter )
{
    if ( ! ( etaRegion == ETA_BELOW_THREE || etaRegion == ETA_OVER_THREE || etaRegion == ETA_ALL ) )
    {
        cerr << "ERROR: Invalid eta region requested! Requested: " << jetCollection << "_ONLY_" << etaRegion << "_" << numJets << "_" << beforeAfter << endl;
        return;
    }
    
    TString label = "JET_"+jetCollection+"_ONLY_"+whereEta[etaRegion]+"_"+numJets+"_"+beforeAfter;
    
    if ( ! inFile->Get("ewkElecStandaloneDQM/JET_"+jetCollection+"_ENERGY_ETABELOW3_"+numJets+"_"+beforeAfter) )
    {
        cerr << "ERROR: Could not create canvas for " << label << endl;
        return;
    }
    
    cout << label  << endl;

    TCanvas* canvas = new TCanvas(label,label, 1200, 800);
    canvas->Divide(3,2);
    
    canvas->cd(1);
    addPlot(jetCollection, "N60", etaRegion, numJets, beforeAfter, "N60");
    
    canvas->cd(2);
    addPlot(jetCollection, "N90", etaRegion, numJets, beforeAfter, "N90");
    
    canvas->cd(3);
    addPlot(jetCollection, "TOTALEMENERGY", etaRegion, numJets, beforeAfter, "Total EM Energy");
    
    canvas->cd(4);
    addPlot(jetCollection, "TOTALHADENERGY", etaRegion, numJets, beforeAfter, "Total Hadronic Energy");
    
    canvas->cd(5);
    addPlot(jetCollection, "EMENERGYFRACTION", etaRegion, numJets, beforeAfter, "EM Energy Fraction");    
       
    canvas->cd();
    
    TString path;
    getJetPath(path, jetCollection, etaRegion);
    SavePlot(label.Prepend(path));
}



void 
createPfEnergyCanvas(TString jetCollection, unsigned int etaRegion, TString numJets, TString beforeAfter )
{
    if ( ! ( etaRegion == ETA_BELOW_THREE || etaRegion == ETA_OVER_THREE || etaRegion == ETA_ALL ) )
    {
        cerr << "ERROR: Invalid eta region requested! Requested: " << jetCollection << "_ENERGY_" << etaRegion << "_" << numJets << "_" << beforeAfter << endl;
        return;
    }
    
    TString label = "JET_"+jetCollection+"_ENERGY_"+whereEta[etaRegion]+"_"+numJets+"_"+beforeAfter;
    
    if ( ! inFile->Get("ewkElecStandaloneDQM/JET_"+jetCollection+"_ENERGY_ETABELOW3_"+numJets+"_"+beforeAfter) )
    {
        cerr << "ERROR: Could not create canvas for " << label << endl;
        return;
    }
    
    cout << label  << endl;

    TCanvas* canvas = new TCanvas(label,label, 1200, 800);
    canvas->Divide(3,2);
    
    canvas->cd(1);
    addPlot(jetCollection, "CHARGEDHADRONENERGY", etaRegion, numJets, beforeAfter, "Charged Hadron Energy");
    
    canvas->cd(2);
    addPlot(jetCollection, "NEUTRALHADRONENERGY", etaRegion, numJets, beforeAfter, "Neutral Hadron Energy");
    
    canvas->cd(3);
    addPlot(jetCollection, "CHARGEDMUENERGY", etaRegion, numJets, beforeAfter, "Charged #mu Energy");
    
    canvas->cd(4);
    addPlot(jetCollection, "CHARGEDEMENERGY", etaRegion, numJets, beforeAfter, "Charged EM Energy");
    
    canvas->cd(5);
    addPlot(jetCollection, "NEUTRALEMENERGY", etaRegion, numJets, beforeAfter, "Neutral EM Energy");
    
    canvas->cd();

    TString path;
    getJetPath(path, jetCollection, etaRegion);
    SavePlot(label.Prepend(path));
}



void 
createPfEnergyFractionCanvas(TString jetCollection, unsigned int etaRegion, TString numJets, TString beforeAfter )
{
    if ( ! ( etaRegion == ETA_BELOW_THREE || etaRegion == ETA_OVER_THREE || etaRegion == ETA_ALL ) )
    {
        cerr << "ERROR: Invalid eta region requested! Requested: " << jetCollection << "_ENERGYFRACTION_" << etaRegion << "_" << numJets << "_" << beforeAfter << endl;
        return;
    }
    
    TString label = "JET_"+jetCollection+"_ENERGYFRACTION_"+whereEta[etaRegion]+"_"+numJets+"_"+beforeAfter;
    
    if ( ! inFile->Get("ewkElecStandaloneDQM/JET_"+jetCollection+"_ENERGY_ETABELOW3_"+numJets+"_"+beforeAfter) )
    {
        cerr << "ERROR: Could not create canvas for " << label << endl;
        return;
    }
    
    cout << label  << endl;

    TCanvas* canvas = new TCanvas(label,label, 1200, 800);
    canvas->Divide(3,2);
    
    canvas->cd(1);
    addPlot(jetCollection, "CHARGEDHADRONENERGYFRACTION", etaRegion, numJets, beforeAfter, "Charged Hadron Energy Fraction");
    
    canvas->cd(2);
    addPlot(jetCollection, "NEUTRALHADRONENERGYFRACTION", etaRegion, numJets, beforeAfter, "Neutral Hadron Energy Fraction");
    
    canvas->cd(3);
    addPlot(jetCollection, "CHARGEDMUENERGYFRACTION", etaRegion, numJets, beforeAfter, "Charged #mu Energy Fraction");
    
    canvas->cd(4);
    addPlot(jetCollection, "CHARGEDEMENERGYFRACTION", etaRegion, numJets, beforeAfter, "Charged EM Energy Fraction");
    
    canvas->cd(5);
    addPlot(jetCollection, "NEUTRALEMENERGYFRACTION", etaRegion, numJets, beforeAfter, "Neutral EM Energy Fraction");
    
    canvas->cd();

    TString path;
    getJetPath(path, jetCollection, etaRegion);
    SavePlot(label.Prepend(path));
}



void 
createPfMultiplicityCanvas(TString jetCollection, unsigned int etaRegion, TString numJets, TString beforeAfter )
{
    if ( ! ( etaRegion == ETA_BELOW_THREE || etaRegion == ETA_OVER_THREE || etaRegion == ETA_ALL ) )
    {
        cerr << "ERROR: Invalid eta region requested! Requested: " << jetCollection << "_MULTIPLICITIES_" << etaRegion << "_" << numJets << "_" << beforeAfter << endl;
        return;
    }
    
    TString label = "JET_"+jetCollection+"_MULTIPLICITIES_"+whereEta[etaRegion]+"_"+numJets+"_"+beforeAfter;
    
    if ( ! inFile->Get("ewkElecStandaloneDQM/JET_"+jetCollection+"_ENERGY_ETABELOW3_"+numJets+"_"+beforeAfter) )
    {
        cerr << "ERROR: Could not create canvas for " << label << endl;
        return;
    }
    
    cout << label  << endl;

    TCanvas* canvas = new TCanvas(label,label, 800, 800);
    canvas->Divide(2,2);
    
    canvas->cd(1);
    addPlot(jetCollection, "CHARGEDMULTIPLICITY", etaRegion, numJets, beforeAfter, "Charged Multiplicity");
    
    canvas->cd(2);
    addPlot(jetCollection, "NEUTRALMULTIPLICITY", etaRegion, numJets, beforeAfter, "Neutral Multiplicity");
    
    canvas->cd(3);
    addPlot(jetCollection, "MUONMULTIPLICITY", etaRegion, numJets, beforeAfter, "#mu multiplicity");
      
    canvas->cd();

    TString path;
    getJetPath(path, jetCollection, etaRegion);
    SavePlot(label.Prepend(path));
}



void addPlot(TString jetCollection, TString variable, unsigned int etaRegion, TString numJets, TString beforeAfter, TString title)
{
  TString beforeAfter2 = beforeAfter;
  beforeAfter2.ToLower();
  TString jetCollection2 = jetCollection;
  jetCollection2.ToLower();
  
    if ( etaRegion == ETA_ALL )
    {
      // input file
      TH1F * hist1 = dynamic_cast<TH1F*>(inFile->Get("ewkElecStandaloneDQM/JET_"+jetCollection+"_" + variable + "_ETABELOW3_" + numJets + "_" + beforeAfter) );
      // reference file
      TH1F * hist2 = dynamic_cast<TH1F*>(referenceFile->Get("ewkElecStandaloneDQM/JET_"+jetCollection+"_" + variable + "_ETABELOW3_" + numJets + "_" + beforeAfter) );

      hist1->Add(dynamic_cast<TH1F*>inFile->Get("ewkElecStandaloneDQM/JET_"+jetCollection+"_" + variable + "_ETAOVER3_"+numJets+"_"+beforeAfter));
      Double_t nEvents = hist1->GetEntries();

      TString highAll = ( numJets == "ALL" ? "All " : "Highest ");
      TString titleLower = highAll + jetCollection2 + " jet " + title + " " + beforeAfter2 + " cuts";
      
      THStack * s_1 = new THStack( "s_1", titleLower );

      hist1->Sumw2();
      hist1->SetMarkerColor(1);
      hist1->SetMarkerStyle(8);
      hist1->SetMarkerSize(0.5);

      s_1->Add(hist1);

      hist2->Add(dynamic_cast<TH1F*>referenceFile->Get("ewkElecStandaloneDQM/JET_"+jetCollection+"_" + variable + "_ETAOVER3_"+numJets+"_"+beforeAfter));
      Double_t nRefEvents = hist2->GetEntries();  
      
      if (nRefEvents != 0)
      {
        scaleFactor = nEvents / nRefEvents;
      }
      else
      {
        scaleFactor = 1;
      }

      //hist2->Scale(scaleFactor); //Already scaled in > (<) eta section 
                                    // Comment (Christos): This (or other pitfalls like this) can be avoided if 
                                    //                     we use Clone() when we Get the histograms
      hist2->SetLineColor(4);
      hist2->SetLineWidth(2);

      s_1->Add(hist2);
      
      s_1->Draw("nostack");

    }
    else
    {
        if ( etaRegion == ETA_BELOW_THREE )
            title += " (|#eta| < 3)";
        else
            title += " (|#eta| > 3)";
   
      // input file        
      TH1F * hist1 = dynamic_cast<TH1F*>(inFile->Get("ewkElecStandaloneDQM/JET_"+jetCollection+"_" + variable + "_"+whereEta[etaRegion]+"_"+numJets+"_"+beforeAfter));
      Double_t nEvents = hist1->GetEntries();

      // reference file
      TH1F * hist2 = dynamic_cast<TH1F*>(referenceFile->Get("ewkElecStandaloneDQM/JET_"+jetCollection+"_" + variable + "_"+whereEta[etaRegion]+"_"+numJets+"_"+beforeAfter));
      Double_t nRefEvents = hist2->GetEntries();
      
      if (nRefEvents != 0)
      {
        scaleFactor = nEvents / nRefEvents;
      }
      else
      {
        scaleFactor = 1;
      }
      
      TString highAll = ( numJets == "ALL" ? "All " : "Highest ");
      TString titleLower = highAll + jetCollection2 + " jet " + title + " " + beforeAfter2 + " cuts";
      
      THStack * s_1 = new THStack( "s_1", titleLower );//hist1->GetTitle()
  
      hist1->Sumw2();
      hist1->SetMarkerColor(1);
      hist1->SetMarkerStyle(8);
      hist1->SetMarkerSize(0.5);

      s_1->Add(hist1);

      hist2->Scale(scaleFactor);
      hist2->SetLineColor(4);
      hist2->SetLineWidth(2);

      s_1->Add(hist2);
      
      s_1->Draw("nostack");

    }
}



void
getJetPath(TString&  toReturn, TString jetCollection, unsigned int etaRegion)
{
    toReturn = "jet" + jetCollection + "Plots/";
    
    if ( etaRegion == ETA_BELOW_THREE )
        toReturn += "ETABELOW3/";
    else if ( etaRegion == ETA_OVER_THREE )
        toReturn += "ETAOVER3/";
}


void makeJetPlots() {
   //   // Christos' jet magic
   for ( int eta = 2; eta > -1; eta-- ) // HACK adding histograms was persistent!
   {
     for ( unsigned int w = 0; w < 2; ++w )
     {
       for ( unsigned int q = 0; q < 2; ++q )
       {
         for ( unsigned int j = 0; j < 5; ++j ) // loop over jets
         createCommonCanvas(jetCollections[j], eta, which[w], when[q]);
         
         createCaloCanvas("CALO", eta, which[w], when[q]);
         createCaloCanvas("CALOCORR", eta, which[w], when[q]);
         createPfEnergyCanvas("PF", eta, which[w], when[q]);
         createPfEnergyCanvas("PFCORR", eta, which[w], when[q]);
         createPfEnergyFractionCanvas("PF", eta, which[w], when[q]);
         createPfEnergyFractionCanvas("PFCORR", eta, which[w], when[q]);
         createPfMultiplicityCanvas("PF", eta, which[w], when[q]);
         createPfMultiplicityCanvas("PFCORR", eta, which[w], when[q]);
       }
     }
   }
  for ( unsigned int i = 0; i < 5; ++i ) // loop over canvases
    {
     for ( unsigned int j = 0; j < 5; ++j ) // loop over jets
     {
       for ( int eta = 0; eta < 3; ++eta )
       {
         for ( unsigned int w = 0; w < 2; ++w )
         {
           for ( unsigned int q = 0; q < 2; ++q )
           {
           TString label = "JET_" + jetCollections[j] + "_" + canvases[i] + "_" + whereEta[eta] + "_" + which[w] + "_" + when[q] ;
           TCanvas * c1 = (TCanvas*) gROOT->FindObject(label);
           if ( c1 ) delete c1;    
           }
         }
       }
     }
   }   
}



void makeMetPlots() {
  // Kira's MET enchantments

  const int numTypes = 6;
  TString types[numTypes] = {"CALO","CALOCORR","PF","PFCORR","TC","GEN"};
  
  const int nVarsCalo = 11;
  TString varsCalo[nVarsCalo] = {"HADETINHB","HADETINHE","HADETINHF","HADETINHO","MAXETINEMTOWERS",
    "MAXETINHADTOWERS","EMETFRACTION","ETFRACTIONHADRONIC",
    "EMETINEB","EMETINEE","EMETINHF"};
  
  const int nVarsPf = 5;
  TString varsPf[nVarsPf] = {"CHARGEDEMETFRAC","CHARGEDHADETFRAC","MUONETFRAC","NEUTRALEMETFRAC","NEUTRALHADETFRAC"};
  
  TString folderName = "metPlots/";
  
  TString folderName2 = "metPlotsDetails/"; 
  
  for( unsigned int i = 0; i < numTypes; i++ ) 
  {
    for( unsigned int j = 0; j < 2; j++ ) 
    {
      if ( ! inFile->Get("ewkElecStandaloneDQM/MET_"+types[i]+"_ET_"+when[j]) )
      {
        cerr << "ERROR: Could not create MET canvases for " << types[i] << endl;
        continue;
      }
      
      drawHistogram(TString("MET_"+types[i]+"_ET_"+when[j]),TString("MET_"+types[i]+"_ET_"+when[j]).Prepend(folderName));
      
      drawHistogram(TString("MET_"+types[i]+"_EX_"+when[j]),TString("MET_"+types[i]+"_EX_"+when[j]).Prepend(folderName));
      
      drawHistogram(TString("MET_"+types[i]+"_EY_"+when[j]),TString("MET_"+types[i]+"_EY_"+when[j]).Prepend(folderName));
      
      drawHistogram(TString("MET_"+types[i]+"_PHI_"+when[j]),TString("MET_"+types[i]+"_PHI_"+when[j]).Prepend(folderName));
      
      drawHistogram(TString("MET_"+types[i]+"_SUMET_"+when[j]),TString("MET_"+types[i]+"_SUMET_"+when[j]).Prepend(folderName));
      
      drawHistogram(TString("MET_"+types[i]+"_SIG_"+when[j]),TString("MET_"+types[i]+"_SIG_"+when[j]).Prepend(folderName));
      //Detailed plots
      if(types[i] == "CALO") {
        for( unsigned int k = 0; k < nVarsCalo; k++ ) 
	  {
	    drawHistogram(TString("MET_"+types[i]+"_"+varsCalo[k]+"_"+when[j]),TString("MET_"+types[i]+"_"+varsCalo[k]+"_"+when[j]).Prepend(folderName2));
	  }
      }
      if(types[i] == "PF") {           
        for( unsigned int k = 0; k < nVarsPf; k++ ) 
	  {
	    drawHistogram(TString("MET_"+types[i]+"_"+varsPf[k]+"_"+when[j]),TString("MET_"+types[i]+"_"+varsPf[k]+"_"+when[j]).Prepend(folderName2));
	  }           
      }
    }
  }  
  
  TString folderName = "wPlots/";
  
  for( unsigned int i = 0; i < numTypes; i++ ) 
  {
    for( unsigned int j = 0; j < 2; j++ ) 
    {      
      if ( ! inFile->Get("ewkElecStandaloneDQM/MET_"+types[i]+"_ET_"+when[j]) )
      {
        cerr << "ERROR: Could not create W canvases for " << types[i] << endl;
        continue;
      }
      
      drawHistogram(TString("W_"+types[i]+"MET_MT_"+when[j]),TString("W_"+types[i]+"MET_MT_"+when[j]).Prepend(folderName));
      
      drawHistogram(TString("W_"+types[i]+"MET_PT_"+when[j]),TString("W_"+types[i]+"MET_PT_"+when[j]).Prepend(folderName));
      
      drawHistogram(TString("W_"+types[i]+"MET_PHI_"+when[j]),TString("W_"+types[i]+"MET_PHI_"+when[j]).Prepend(folderName));
      
      drawHistogram(TString("W_"+types[i]+"MET_RAPIDITY_"+when[j]),TString("W_"+types[i]+"MET_RAPIDITY_"+when[j]).Prepend(folderName));
      
    }
  }
  
}



void makeTriggerPlots()
{

  TString folder("triggerPlots/");

  const int ptBinArraySize = 17;
  const int etaBinArraySize = 16;

  Double_t ptBins[ptBinArraySize] = {0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 24, 28, 32, 40, 48, 60};
  Double_t etaBins[etaBinArraySize] = {-3.0, -2.6, -2.2, -1.8, -1.4, -1.0, -0.6, -0.2, 0.2, 0.6, 1.0, 1.4, 1.8, 2.2, 2.6, 3.0};

  drawEfficiency("TRG_HLT_L1SINGLEEG5_EFF",TString("TRG_DENOMINATOR_EFF"),TString("trg_HLT_L1SingleEG5_eff").Prepend(folder),ptBins,ptBinArraySize);
  drawEfficiency("TRG_HLT_L1SINGLEEG8_EFF",TString("TRG_DENOMINATOR_EFF"),TString("trg_HLT_L1SingleEG8_eff").Prepend(folder),ptBins,ptBinArraySize);
  drawEfficiency("TRG_HLT_ELE10_LW_L1R_EFF",TString("TRG_DENOMINATOR_EFF"),TString("trg_HLT_Ele10_LW_L1R_eff").Prepend(folder),ptBins,ptBinArraySize);
  drawEfficiency("TRG_HLT_ELE10_LW_ELEID_L1R_EFF",TString("TRG_DENOMINATOR_EFF"),TString("trg_HLT_Ele10_LW_EleId_L1R_eff").Prepend(folder),ptBins,ptBinArraySize);
  drawEfficiency("TRG_HLT_ELE15_LW_L1R_EFF",TString("TRG_DENOMINATOR_EFF"),TString("trg_HLT_Ele15_LW_L1R_eff").Prepend(folder),ptBins,ptBinArraySize);
  drawEfficiency("TRG_HLT_ELE15_SISTRIP_L1R_EFF",TString("TRG_DENOMINATOR_EFF"),TString("trg_HLT_Ele15_SiStrip_L1R_eff").Prepend(folder),ptBins,ptBinArraySize);
  drawEfficiency("TRG_HLT_ELE20_LW_L1R_EFF",TString("TRG_DENOMINATOR_EFF"),TString("trg_HLT_Ele20_LW_L1R_eff").Prepend(folder),ptBins,ptBinArraySize);  

  drawEfficiency("TRG_HLT_PHOTON10_L1R_EFF",TString("TRG_DENOMINATOR_EFF"),TString("trg_HLT_Photon10_L1R_eff").Prepend(folder),ptBins,ptBinArraySize);  
  drawEfficiency("TRG_HLT_PHOTON15_L1R_EFF",TString("TRG_DENOMINATOR_EFF"),TString("trg_HLT_Photon15_L1R_eff").Prepend(folder),ptBins,ptBinArraySize);  
  drawEfficiency("TRG_HLT_PHOTON15_TRACKISO_L1R_EFF",TString("TRG_DENOMINATOR_EFF"),TString("trg_HLT_Photon15_TrackIso_L1R_eff").Prepend(folder),ptBins,ptBinArraySize);  
  drawEfficiency("TRG_HLT_PHOTON15_LOOSEECALISO_L1R_EFF",TString("TRG_DENOMINATOR_EFF"),TString("trg_HLT_Photon15_LooseEcalIso_L1R_eff").Prepend(folder),ptBins,ptBinArraySize);  
  drawEfficiency("TRG_HLT_PHOTON20_L1R_EFF",TString("TRG_DENOMINATOR_EFF"),TString("trg_HLT_Photon20_L1R_eff").Prepend(folder),ptBins,ptBinArraySize);  
  drawEfficiency("TRG_HLT_PHOTON30_L1R_EFF",TString("TRG_DENOMINATOR_EFF"),TString("trg_HLT_Photon30_L1R_eff").Prepend(folder),ptBins,ptBinArraySize);  


  // L1
  drawEfficiency("TRG_L1_SINGLEISOEG5_EFF",TString("TRG_DENOMINATOR_EFF"),TString("trg_L1_SingleIsoEG5_eff").Prepend(folder),ptBins,ptBinArraySize);  
  drawEfficiency("TRG_L1_SINGLEISOEG8_EFF",TString("TRG_DENOMINATOR_EFF"),TString("trg_L1_SingleIsoEG8_eff").Prepend(folder),ptBins,ptBinArraySize);  
  drawEfficiency("TRG_L1_SINGLEISOEG10_EFF",TString("TRG_DENOMINATOR_EFF"),TString("trg_L1_SingleIsoEG10_eff").Prepend(folder),ptBins,ptBinArraySize);  
  drawEfficiency("TRG_L1_SINGLEISOEG12_EFF",TString("TRG_DENOMINATOR_EFF"),TString("trg_L1_SingleIsoEG12_eff").Prepend(folder),ptBins,ptBinArraySize);  
  drawEfficiency("TRG_L1_SINGLEISOEG15_EFF",TString("TRG_DENOMINATOR_EFF"),TString("trg_L1_SingleIsoEG15_eff").Prepend(folder),ptBins,ptBinArraySize);  

  drawEfficiency("TRG_L1_SINGLEEG2_EFF",TString("TRG_DENOMINATOR_EFF"),TString("trg_L1_SingleEG2_eff").Prepend(folder),ptBins,ptBinArraySize);  
  drawEfficiency("TRG_L1_SINGLEEG5_EFF",TString("TRG_DENOMINATOR_EFF"),TString("trg_L1_SingleEG5_eff").Prepend(folder),ptBins,ptBinArraySize);  
  drawEfficiency("TRG_L1_SINGLEEG8_EFF",TString("TRG_DENOMINATOR_EFF"),TString("trg_L1_SingleEG8_eff").Prepend(folder),ptBins,ptBinArraySize);  
  drawEfficiency("TRG_L1_SINGLEEG10_EFF",TString("TRG_DENOMINATOR_EFF"),TString("trg_L1_SingleEG10_eff").Prepend(folder),ptBins,ptBinArraySize);  
  drawEfficiency("TRG_L1_SINGLEEG12_EFF",TString("TRG_DENOMINATOR_EFF"),TString("trg_L1_SingleEG12_eff").Prepend(folder),ptBins,ptBinArraySize);  
  drawEfficiency("TRG_L1_SINGLEEG15_EFF",TString("TRG_DENOMINATOR_EFF"),TString("trg_L1_SingleEG15_eff").Prepend(folder),ptBins,ptBinArraySize);  
  drawEfficiency("TRG_L1_SINGLEEG20_EFF",TString("TRG_DENOMINATOR_EFF"),TString("trg_L1_SingleEG20_eff").Prepend(folder),ptBins,ptBinArraySize);  

<<<<<<< dqmDisplayFull.C


  // GOOD ELE

  //  drawEfficiency("TRG_HLT_L1SINGLEEG5_EFF_GOODELE",TString("TRG_DENOMINATOR_EFF_GOODELE"),TString("trg_HLT_L1SingleEG5_eff_goodEle").Prepend(folder),ptBins,ptBinArraySize);
  //  drawEfficiency("TRG_HLT_L1SINGLEEG8_EFF_GOODELE",TString("TRG_DENOMINATOR_EFF_GOODELE"),TString("trg_HLT_L1SingleEG8_eff_goodEle").Prepend(folder),ptBins,ptBinArraySize);
  drawEfficiency("TRG_HLT_ELE10_LW_L1R_EFF_GOODELE",TString("TRG_DENOMINATOR_EFF_GOODELE"),TString("trg_HLT_Ele10_LW_L1R_eff_goodEle").Prepend(folder),ptBins,ptBinArraySize);
  drawEfficiency("TRG_HLT_ELE10_LW_ELEID_L1R_EFF_GOODELE",TString("TRG_DENOMINATOR_EFF_GOODELE"),TString("trg_HLT_Ele10_LW_EleId_L1R_eff_goodEle").Prepend(folder),ptBins,ptBinArraySize);
  drawEfficiency("TRG_HLT_ELE15_LW_L1R_EFF_GOODELE",TString("TRG_DENOMINATOR_EFF_GOODELE"),TString("trg_HLT_Ele15_LW_L1R_eff_goodEle").Prepend(folder),ptBins,ptBinArraySize);
  drawEfficiency("TRG_HLT_ELE15_SISTRIP_L1R_EFF_GOODELE",TString("TRG_DENOMINATOR_EFF_GOODELE"),TString("trg_HLT_Ele15_SiStrip_L1R_eff_goodEle").Prepend(folder),ptBins,ptBinArraySize);
  drawEfficiency("TRG_HLT_ELE20_LW_L1R_EFF_GOODELE",TString("TRG_DENOMINATOR_EFF_GOODELE"),TString("trg_HLT_Ele20_LW_L1R_eff_goodEle").Prepend(folder),ptBins,ptBinArraySize);  

  drawEfficiency("TRG_HLT_PHOTON10_L1R_EFF_GOODELE",TString("TRG_DENOMINATOR_EFF_GOODELE"),TString("trg_HLT_Photon10_L1R_eff_goodEle").Prepend(folder),ptBins,ptBinArraySize);  
  drawEfficiency("TRG_HLT_PHOTON15_L1R_EFF_GOODELE",TString("TRG_DENOMINATOR_EFF_GOODELE"),TString("trg_HLT_Photon15_L1R_eff_goodEle").Prepend(folder),ptBins,ptBinArraySize);  
  drawEfficiency("TRG_HLT_PHOTON15_TRACKISO_L1R_EFF_GOODELE",TString("TRG_DENOMINATOR_EFF_GOODELE"),TString("trg_HLT_Photon15_TrackIso_L1R_eff_goodEle").Prepend(folder),ptBins,ptBinArraySize);  
  drawEfficiency("TRG_HLT_PHOTON15_LOOSEECALISO_L1R_EFF_GOODELE",TString("TRG_DENOMINATOR_EFF_GOODELE"),TString("trg_HLT_Photon15_LooseEcalIso_L1R_eff_goodEle").Prepend(folder),ptBins,ptBinArraySize);  
  drawEfficiency("TRG_HLT_PHOTON20_L1R_EFF_GOODELE",TString("TRG_DENOMINATOR_EFF_GOODELE"),TString("trg_HLT_Photon20_L1R_eff_goodEle").Prepend(folder),ptBins,ptBinArraySize);  
  drawEfficiency("TRG_HLT_PHOTON30_L1R_EFF_GOODELE",TString("TRG_DENOMINATOR_EFF_GOODELE"),TString("trg_HLT_Photon30_L1R_eff_goodEle").Prepend(folder),ptBins,ptBinArraySize);  


  // L1
  drawEfficiency("TRG_L1_SINGLEISOEG5_EFF_GOODELE",TString("TRG_DENOMINATOR_EFF_GOODELE"),TString("trg_L1_SingleIsoEG5_eff_goodEle").Prepend(folder),ptBins,ptBinArraySize);  
  drawEfficiency("TRG_L1_SINGLEISOEG8_EFF_GOODELE",TString("TRG_DENOMINATOR_EFF_GOODELE"),TString("trg_L1_SingleIsoEG8_eff_goodEle").Prepend(folder),ptBins,ptBinArraySize);  
  drawEfficiency("TRG_L1_SINGLEISOEG10_EFF_GOODELE",TString("TRG_DENOMINATOR_EFF_GOODELE"),TString("trg_L1_SingleIsoEG10_eff_goodEle").Prepend(folder),ptBins,ptBinArraySize);  
  drawEfficiency("TRG_L1_SINGLEISOEG12_EFF_GOODELE",TString("TRG_DENOMINATOR_EFF_GOODELE"),TString("trg_L1_SingleIsoEG12_eff_goodEle").Prepend(folder),ptBins,ptBinArraySize);  
  drawEfficiency("TRG_L1_SINGLEISOEG15_EFF_GOODELE",TString("TRG_DENOMINATOR_EFF_GOODELE"),TString("trg_L1_SingleIsoEG15_eff_goodEle").Prepend(folder),ptBins,ptBinArraySize);  

  drawEfficiency("TRG_L1_SINGLEEG2_EFF_GOODELE",TString("TRG_DENOMINATOR_EFF_GOODELE"),TString("trg_L1_SingleEG2_eff_goodEle").Prepend(folder),ptBins,ptBinArraySize);  
  drawEfficiency("TRG_L1_SINGLEEG5_EFF_GOODELE",TString("TRG_DENOMINATOR_EFF_GOODELE"),TString("trg_L1_SingleEG5_eff_goodEle").Prepend(folder),ptBins,ptBinArraySize);  
  drawEfficiency("TRG_L1_SINGLEEG8_EFF_GOODELE",TString("TRG_DENOMINATOR_EFF_GOODELE"),TString("trg_L1_SingleEG8_eff_goodEle").Prepend(folder),ptBins,ptBinArraySize);  
  drawEfficiency("TRG_L1_SINGLEEG10_EFF_GOODELE",TString("TRG_DENOMINATOR_EFF_GOODELE"),TString("trg_L1_SingleEG10_eff_goodEle").Prepend(folder),ptBins,ptBinArraySize);  
  drawEfficiency("TRG_L1_SINGLEEG12_EFF_GOODELE",TString("TRG_DENOMINATOR_EFF_GOODELE"),TString("trg_L1_SingleEG12_eff_goodEle").Prepend(folder),ptBins,ptBinArraySize);  
  drawEfficiency("TRG_L1_SINGLEEG15_EFF_GOODELE",TString("TRG_DENOMINATOR_EFF_GOODELE"),TString("trg_L1_SingleEG15_eff_goodEle").Prepend(folder),ptBins,ptBinArraySize);  
  drawEfficiency("TRG_L1_SINGLEEG20_EFF_GOODELE",TString("TRG_DENOMINATOR_EFF_GOODELE"),TString("trg_L1_SingleEG20_eff_goodEle").Prepend(folder),ptBins,ptBinArraySize);  




=======



  // GOOD ELEC

  drawEfficiency("TRG_HLT_L1SINGLEEG5_EFF_GOODELE",TString("TRG_DENOMINATOR_EFF_GOODELE"),TString("trg_HLT_L1SingleEG5_eff_goodEle").Prepend(folder),ptBins,ptBinArraySize);
  drawEfficiency("TRG_HLT_L1SINGLEEG8_EFF_GOODELE",TString("TRG_DENOMINATOR_EFF_GOODELE"),TString("trg_HLT_L1SingleEG8_eff_goodEle").Prepend(folder),ptBins,ptBinArraySize);
  drawEfficiency("TRG_HLT_ELE10_LW_L1R_EFF_GOODELE",TString("TRG_DENOMINATOR_EFF_GOODELE"),TString("trg_HLT_Ele10_LW_L1R_eff_goodEle").Prepend(folder),ptBins,ptBinArraySize);
  drawEfficiency("TRG_HLT_ELE10_LW_ELEID_L1R_EFF_GOODELE",TString("TRG_DENOMINATOR_EFF_GOODELE"),TString("trg_HLT_Ele10_LW_EleId_L1R_eff_goodEle").Prepend(folder),ptBins,ptBinArraySize);
  drawEfficiency("TRG_HLT_ELE15_LW_L1R_EFF_GOODELE",TString("TRG_DENOMINATOR_EFF_GOODELE"),TString("trg_HLT_Ele15_LW_L1R_eff_goodEle").Prepend(folder),ptBins,ptBinArraySize);
  drawEfficiency("TRG_HLT_ELE15_SISTRIP_L1R_EFF_GOODELE",TString("TRG_DENOMINATOR_EFF_GOODELE"),TString("trg_HLT_Ele15_SiStrip_L1R_eff_goodEle").Prepend(folder),ptBins,ptBinArraySize);
  drawEfficiency("TRG_HLT_ELE20_LW_L1R_EFF_GOODELE",TString("TRG_DENOMINATOR_EFF_GOODELE"),TString("trg_HLT_Ele20_LW_L1R_eff_goodEle").Prepend(folder),ptBins,ptBinArraySize);  

  drawEfficiency("TRG_HLT_PHOTON10_L1R_EFF_GOODELE",TString("TRG_DENOMINATOR_EFF_GOODELE"),TString("trg_HLT_Photon10_L1R_eff_goodEle").Prepend(folder),ptBins,ptBinArraySize);  
  drawEfficiency("TRG_HLT_PHOTON15_L1R_EFF_GOODELE",TString("TRG_DENOMINATOR_EFF_GOODELE"),TString("trg_HLT_Photon15_L1R_eff_goodEle").Prepend(folder),ptBins,ptBinArraySize);  
  drawEfficiency("TRG_HLT_PHOTON15_TRACKISO_L1R_EFF_GOODELE",TString("TRG_DENOMINATOR_EFF_GOODELE"),TString("trg_HLT_Photon15_TrackIso_L1R_eff_goodEle").Prepend(folder),ptBins,ptBinArraySize);  
  drawEfficiency("TRG_HLT_PHOTON15_LOOSEECALISO_L1R_EFF_GOODELE",TString("TRG_DENOMINATOR_EFF_GOODELE"),TString("trg_HLT_Photon15_LooseEcalIso_L1R_eff_goodEle").Prepend(folder),ptBins,ptBinArraySize);  
  drawEfficiency("TRG_HLT_PHOTON20_L1R_EFF_GOODELE",TString("TRG_DENOMINATOR_EFF_GOODELE"),TString("trg_HLT_Photon20_L1R_eff_goodEle").Prepend(folder),ptBins,ptBinArraySize);  
  drawEfficiency("TRG_HLT_PHOTON30_L1R_EFF_GOODELE",TString("TRG_DENOMINATOR_EFF_GOODELE"),TString("trg_HLT_Photon30_L1R_eff_goodEle").Prepend(folder),ptBins,ptBinArraySize);  


  // L1
  drawEfficiency("TRG_L1_SINGLEISOEG5_EFF_GOODELE",TString("TRG_DENOMINATOR_EFF_GOODELE"),TString("trg_L1_SingleIsoEG5_eff_goodEle").Prepend(folder),ptBins,ptBinArraySize);  
  drawEfficiency("TRG_L1_SINGLEISOEG8_EFF_GOODELE",TString("TRG_DENOMINATOR_EFF_GOODELE"),TString("trg_L1_SingleIsoEG8_eff_goodEle").Prepend(folder),ptBins,ptBinArraySize);  
  drawEfficiency("TRG_L1_SINGLEISOEG10_EFF_GOODELE",TString("TRG_DENOMINATOR_EFF_GOODELE"),TString("trg_L1_SingleIsoEG10_eff_goodEle").Prepend(folder),ptBins,ptBinArraySize);  
  drawEfficiency("TRG_L1_SINGLEISOEG12_EFF_GOODELE",TString("TRG_DENOMINATOR_EFF_GOODELE"),TString("trg_L1_SingleIsoEG12_eff_goodEle").Prepend(folder),ptBins,ptBinArraySize);  
  drawEfficiency("TRG_L1_SINGLEISOEG15_EFF_GOODELE",TString("TRG_DENOMINATOR_EFF_GOODELE"),TString("trg_L1_SingleIsoEG15_eff_goodEle").Prepend(folder),ptBins,ptBinArraySize);  

  drawEfficiency("TRG_L1_SINGLEEG2_EFF_GOODELE",TString("TRG_DENOMINATOR_EFF_GOODELE"),TString("trg_L1_SingleEG2_eff_goodEle").Prepend(folder),ptBins,ptBinArraySize);  
  drawEfficiency("TRG_L1_SINGLEEG5_EFF_GOODELE",TString("TRG_DENOMINATOR_EFF_GOODELE"),TString("trg_L1_SingleEG5_eff_goodEle").Prepend(folder),ptBins,ptBinArraySize);  
  drawEfficiency("TRG_L1_SINGLEEG8_EFF_GOODELE",TString("TRG_DENOMINATOR_EFF_GOODELE"),TString("trg_L1_SingleEG8_eff_goodEle").Prepend(folder),ptBins,ptBinArraySize);  
  drawEfficiency("TRG_L1_SINGLEEG10_EFF_GOODELE",TString("TRG_DENOMINATOR_EFF_GOODELE"),TString("trg_L1_SingleEG10_eff_goodEle").Prepend(folder),ptBins,ptBinArraySize);  
  drawEfficiency("TRG_L1_SINGLEEG12_EFF_GOODELE",TString("TRG_DENOMINATOR_EFF_GOODELE"),TString("trg_L1_SingleEG12_eff_goodEle").Prepend(folder),ptBins,ptBinArraySize);  
  drawEfficiency("TRG_L1_SINGLEEG15_EFF_GOODELE",TString("TRG_DENOMINATOR_EFF_GOODELE"),TString("trg_L1_SingleEG15_eff_goodEle").Prepend(folder),ptBins,ptBinArraySize);  
  drawEfficiency("TRG_L1_SINGLEEG20_EFF_GOODELE",TString("TRG_DENOMINATOR_EFF_GOODELE"),TString("trg_L1_SingleEG20_eff_goodEle").Prepend(folder),ptBins,ptBinArraySize);  










>>>>>>> 1.13
  // SPECIAL
  // threshold

  // barrel-only
  drawEfficiency("TRG_L1_SINGLEEG5_EFF_BARREL",
		 TString("TRG_DENOMINATOR_EFF_BARREL"),
		 TString("trg_L1_SingleEG5_eff_barrel").Prepend(folder),ptBins,ptBinArraySize);

  // ecal-driven-only
  drawEfficiency("TRG_L1_SINGLEEG5_EFF_ECALDRIVEN",
                 TString("TRG_DENOMINATOR_EFF_ECALDRIVEN"),
                 TString("trg_L1_SingleEG5_eff_ecalDriven").Prepend(folder),ptBins,ptBinArraySize);

<<<<<<< dqmDisplayFull.C
//   // good electrons only
//   drawEfficiency("TRG_L1_SINGLEEG5_EFF_GOODELE",
//                  TString("TRG_DENOMINATOR_EFF_GOODELE"),
//                  TString("trg_L1_SingleEG5_eff_goodEle").Prepend(folder),ptBins,ptBinArraySize);
=======
  // good electrons only
//   drawEfficiency("TRG_L1_SINGLEEG5_EFF_GOODELE",
//                  TString("TRG_DENOMINATOR_EFF_GOODELE"),
//                  TString("trg_L1_SingleEG5_eff_goodEle").Prepend(folder),ptBins,ptBinArraySize);
>>>>>>> 1.13

  // notfire
  drawEfficiency("TRG_L1_SINGLEEG5_EFF_NOTFIRE1",
		 TString("TRG_DENOMINATOR_EFF"),
		 TString("trg_L1_SingleEG5_eff_notfire1").Prepend(folder),ptBins,ptBinArraySize);  

  drawEfficiency("TRG_L1_SINGLEEG5_EFF_NOTFIRE2",
		 TString("TRG_DENOMINATOR_EFF"),
		 TString("trg_L1_SingleEG5_eff_notfire2").Prepend(folder),ptBins,ptBinArraySize);  

  // eta
  drawEfficiency("TRG_L1_SINGLEEG5_EFF_ETA",TString("TRG_DENOMINATOR_EFF_ETA"),TString("trg_L1_SingleEG5_eff_eta").Prepend(folder),etaBins,etaBinArraySize);  

//   drawEfficiency("TRG_L1_SINGLEEG5_EFF_ETA_CUTET5",
// 		 //		 TString("TRG_DENOMINATOR_EFF"),
// 		 TString("TRG_DENOMINATOR_EFF_ETA_CUTET5"),
// 		 TString("trg_L1_SingleEG5_eff_eta_cutEt5").Prepend(folder),etaBins,etaBinArraySize);

  // eta notfire
  drawEfficiency("TRG_L1_SINGLEEG5_EFF_ETA_NOTFIRE1",TString("TRG_DENOMINATOR_EFF_ETA"),TString("trg_L1_SingleEG5_eff_eta_notfire1").Prepend(folder),etaBins,etaBinArraySize);  

  drawEfficiency("TRG_L1_SINGLEEG5_EFF_ETA_NOTFIRE2",TString("TRG_DENOMINATOR_EFF_ETA"),TString("trg_L1_SingleEG5_eff_eta_notfire2").Prepend(folder),etaBins,etaBinArraySize);  


}



// TOP-LEVEL ELECTRON
//     * eta
//     * et
//       * pt ecal, tracker driven
//     * top-level ecal/hcal/track iso barrel/endcap
//     * top-level delta eta in barrel/endcap
//     * top-level sigma ieta ieta barrel/endcap
//     * dxy
//     * fbrem
//     * number of electrons
//     * specified trigger fires
//     * all triggers of interest fired 
//     * inv mass


void makeElectronTopLevelPlots()
{
  
  TString folder("electronTopLevelPlots/");
  
  // before cuts
  //   ELE_ETA_BEFORECUTS->Draw();
  //   SavePlot(TString("eta_before").Prepend(folder));
  
  drawHistogram("ELE_ETA_BEFORECUTS",TString("eta_before").Prepend(folder));
  
  drawHistogram("ELE_PHI_BEFORECUTS",TString("phi_before").Prepend(folder));
  
  drawHistogram("ELE_PT_BEFORECUTS",TString("pT_before").Prepend(folder));
  
  drawHistogram("ELE_PTECALDRIVEN_BEFORECUTS",TString("ptEcalDriven_before").Prepend(folder));
  
  drawHistogram("ELE_PTTRKDRIVEN_BEFORECUTS",TString("ptTrkDriven_before").Prepend(folder));
  
  drawHistogram("ELE_SIEIEBARREL_BEFORECUTS",TString("sieieBarrel_before").Prepend(folder));
  
  drawHistogram("ELE_SIEIEENDCAP_BEFORECUTS",TString("sieieEndcap_before").Prepend(folder));
  
  drawHistogram("ELE_DETAINBARREL_BEFORECUTS",TString("detainBarrel_before").Prepend(folder));
  
  drawHistogram("ELE_DETAINENDCAP_BEFORECUTS",TString("detainEndcap_before").Prepend(folder));
  
  drawHistogram("ELE_ECALISOBARREL_BEFORECUTS",TString("ecalIsoBarrel_before").Prepend(folder));
  
  drawHistogram("ELE_ECALISOENDCAP_BEFORECUTS",TString("ecalIsoEndcap_before").Prepend(folder));
  
  drawHistogram("ELE_HCALISOBARREL_BEFORECUTS",TString("hcalIsoBarrel_before").Prepend(folder));
  
  drawHistogram("ELE_HCALISOENDCAP_BEFORECUTS",TString("hcalIsoEndcap_before").Prepend(folder));
  
  drawHistogram("ELE_TRKISOBARREL_BEFORECUTS",TString("trkIsoBarrel_before").Prepend(folder));
  
  drawHistogram("ELE_TRKISOENDCAP_BEFORECUTS",TString("trkIsoEndcap_before").Prepend(folder));
  
  drawHistogram("ELE_TRIG_BEFORECUTS",TString("trig_before").Prepend(folder));
  
  // special stuff to make triggers fired legible
  Float_t bottomMargin = gPad->GetBottomMargin();
  TH1F * dummy = dynamic_cast<TH1F*>(inFile->Get("ewkElecStandaloneDQM/ELE_TRIGGERSFIRED_BEFORECUTS") );
  dummy->LabelsOption("v","x");
  gPad->SetBottomMargin(0.5);
  dummy->Sumw2();
  dummy->SetMarkerColor(1);
  dummy->SetMarkerStyle(8);
  dummy->SetMarkerSize(0.8);
  dummy->Draw("ep");
  dummy = dynamic_cast<TH1F*>(referenceFile->Get("ewkElecStandaloneDQM/ELE_TRIGGERSFIRED_BEFORECUTS") );
  dummy->Scale(scaleFactor);
  dummy->SetLineColor(4);
  dummy->SetLineWidth(2);
  dummy->Draw("same");
  SavePlot(TString("triggersFired_before").Prepend(folder));
  gPad->SetBottomMargin(bottomMargin);
  
  drawHistogram("ELE_DXY_BEFORECUTS",TString("dxy_before").Prepend(folder));
  
  drawHistogram("ELE_FBREM_BEFORECUTS",TString("fBrem_before").Prepend(folder));
  
  drawHistogram("ELE_INVMASS_BEFORECUTS",TString("invMass_before").Prepend(folder));
  
  drawHistogram("ELE_NELECTRONS_BEFORECUTS",TString("nElectrons_before").Prepend(folder));
  
  
  // after cuts
  drawHistogram("ELE_ETA_LASTCUT",TString("eta_after").Prepend(folder));
  
  drawHistogram("ELE_PHI_AFTERCUTS",TString("phi_after").Prepend(folder));
  
  drawHistogram("ELE_PT_LASTCUT",TString("pT_after").Prepend(folder));
  
  drawHistogram("ELE_PTECALDRIVEN_LASTCUT",TString("ptEcalDriven_after").Prepend(folder));
  
  drawHistogram("ELE_PTTRKDRIVEN_LASTCUT",TString("ptTrkDriven_after").Prepend(folder));
  
  drawHistogram("ELE_SIEIEBARREL_LASTCUT",TString("sieieBarrel_after").Prepend(folder));
  
  drawHistogram("ELE_SIEIEENDCAP_LASTCUT",TString("sieieEndcap_after").Prepend(folder));
  
  drawHistogram("ELE_DETAINBARREL_LASTCUT",TString("detainBarrel_after").Prepend(folder));
  
  drawHistogram("ELE_DETAINENDCAP_LASTCUT",TString("detainEndcap_after").Prepend(folder));
  
  drawHistogram("ELE_ECALISOBARREL_LASTCUT",TString("ecalIsoBarrel_after").Prepend(folder));
  
  drawHistogram("ELE_ECALISOENDCAP_LASTCUT",TString("ecalIsoEndcap_after").Prepend(folder));
  
  drawHistogram("ELE_HCALISOBARREL_LASTCUT",TString("hcalIsoBarrel_after").Prepend(folder));
  
  drawHistogram("ELE_HCALISOENDCAP_LASTCUT",TString("hcalIsoEndcap_after").Prepend(folder));
  
  drawHistogram("ELE_TRKISOBARREL_LASTCUT",TString("trkIsoBarrel_after").Prepend(folder));
  
  drawHistogram("ELE_TRKISOENDCAP_LASTCUT",TString("trkIsoEndcap_after").Prepend(folder));
  
  drawHistogram("ELE_TRIG_LASTCUT",TString("trig_after").Prepend(folder));
  
  //  drawHistogram("ELE_TRIGGERSFIRED_AFTERCUTS",TString("triggersFired_after").Prepend(folder));
  dummy = dynamic_cast<TH1F*>(inFile->Get("ewkElecStandaloneDQM/ELE_TRIGGERSFIRED_AFTERCUTS") );
  dummy->LabelsOption("v","x");
  gPad->SetBottomMargin(0.5);
  dummy->Sumw2();
  dummy->SetMarkerColor(1);
  dummy->SetMarkerStyle(8);
  dummy->SetMarkerSize(0.8);
  dummy->Draw("ep");
  dummy = dynamic_cast<TH1F*>(referenceFile->Get("ewkElecStandaloneDQM/ELE_TRIGGERSFIRED_AFTERCUTS") );
  dummy->Scale(scaleFactor);
  dummy->SetLineColor(4);
  dummy->SetLineWidth(2);
  dummy->Draw("same");
  SavePlot(TString("triggersFired_after").Prepend(folder));
  gPad->SetBottomMargin(bottomMargin);
  
  drawHistogram("ELE_DXY_AFTERCUTS",TString("dxy_after").Prepend(folder));
  
  drawHistogram("ELE_FBREM_AFTERCUTS",TString("fBrem_after").Prepend(folder));
  
  drawHistogram("ELE_INVMASS_AFTERCUTS",TString("invMass_after").Prepend(folder));
  
  drawHistogram("ELE_NELECTRONS_AFTERCUTS",TString("nElectrons_after").Prepend(folder));
  
}



// ELECTRON KINEMATICS

//     * eta
//     * et
//       * pt ecal, tracker driven
//       * ecal energy (after corrections)
//     * energy
//     * ecal energy error
//     * momentum error
//     * pt
//     * px
//     * py
//     * pz
//     * p 

void makeElectronKinematicsPlots()
{
  
  TString folder("electronKinematicsPlots/");
  
  // before
  
  //  drawHistogram("ELE_ETA_BEFORECUTS",TString("eta_before").Prepend(folder));
  
  drawHistogram("ELE_ET_BEFORECUTS",TString("eT_before").Prepend(folder));
  
  //  drawHistogram("ELE_PTECALDRIVEN_BEFORECUTS",TString("ptEcalDriven_before").Prepend(folder));
  
  //  drawHistogram("ELE_PTTRKDRIVEN_BEFORECUTS",TString("ptTrkDriven_before").Prepend(folder));
  
  drawHistogram("ELE_ENERGY_BEFORECUTS",TString("energy_before").Prepend(folder));
  
  drawHistogram("ELE_ECALENERGY_BEFORECUTS",TString("ecalEnergy_before").Prepend(folder));
  
  drawHistogram("ELE_ECALENERGYERROR_BEFORECUTS",TString("ecalEnergyError_before").Prepend(folder));
  
  drawHistogram("ELE_ELECTRONMOMENTUMERROR_BEFORECUTS",TString("eleMomError_before").Prepend(folder));
  
  //  drawHistogram("ELE_PT_BEFORECUTS",TString("pT_before").Prepend(folder));
  
  drawHistogram("ELE_PX_BEFORECUTS",TString("pX_before").Prepend(folder));
  
  drawHistogram("ELE_PY_BEFORECUTS",TString("pY_before").Prepend(folder));
  
  drawHistogram("ELE_PZ_BEFORECUTS",TString("pZ_before").Prepend(folder));
  
  drawHistogram("ELE_P_BEFORECUTS",TString("p_before").Prepend(folder));
  
  
  
  // after
  
  //  drawHistogram("ELE_ETA_LASTCUT",TString("eta_after").Prepend(folder));
  
  drawHistogram("ELE_ET_AFTERCUTS",TString("eT_after").Prepend(folder));
  
  //  drawHistogram("ELE_PTECALDRIVEN_LASTCUT",TString("ptEcalDriven_after").Prepend(folder));
  
  //  drawHistogram("ELE_PTTRKDRIVEN_LASTCUT",TString("ptTrkDriven_after").Prepend(folder));
  
  drawHistogram("ELE_ENERGY_AFTERCUTS",TString("energy_after").Prepend(folder));
  
  drawHistogram("ELE_ECALENERGY_AFTERCUTS",TString("ecalEnergy_after").Prepend(folder));
  
  drawHistogram("ELE_ECALENERGYERROR_AFTERCUTS",TString("ecalEnergyError_after").Prepend(folder));
  
  drawHistogram("ELE_ELECTRONMOMENTUMERROR_AFTERCUTS",TString("eleMomError_after").Prepend(folder));
  
  //  drawHistogram("ELE_PT_LASTCUT",TString("pT_after").Prepend(folder));
  
  drawHistogram("ELE_PX_AFTERCUTS",TString("pX_after").Prepend(folder));
  
  drawHistogram("ELE_PY_AFTERCUTS",TString("pY_after").Prepend(folder));
  
  drawHistogram("ELE_PZ_AFTERCUTS",TString("pZ_after").Prepend(folder));
  
  drawHistogram("ELE_P_AFTERCUTS",TString("p_after").Prepend(folder));
  
}




// ELECTRON SHOWER SHAPE

//     *  top-level sigma ieta ieta barrel/endcap
//     * e1x5 barrel/endcap/both
//     * e2x5 max barrel/endcap/both
//     * e5x5 barrel/endcap/both
//     * 1x5/5x5 barrel/endcap/both
//     * 2x5max/5x5 barrel/endcap/both
//     * sigma eta eta barrel/endcap/both
//     * sigma ieta ieta barrel/endcap/both
//     * supercluster eta/phi width barrel/endcap/both
//     * Eseed/Esupercluster
//     * supercluster preshower energy barrel/endcap/both 
//     * r9

void makeElectronShowerShapePlots()
{
  
  TString folder("electronShowerShapePlots/");
  
  // before
  //  drawHistogram("ELE_SIEIEBARREL_BEFORECUTS",TString("sieieBarrel_before").Prepend(folder));
  
  //  drawHistogram("ELE_SIEIEENDCAP_BEFORECUTS",TString("sieieEndcap_before").Prepend(folder));
  
  drawHistogram("ELE_E1X5_BEFORECUTS",TString("e1x5_before").Prepend(folder));
  
  drawHistogram("ELE_E1X5BARREL_BEFORECUTS",TString("e1x5Barrel_before").Prepend(folder));
  
  drawHistogram("ELE_E1X5ENDCAP_BEFORECUTS",TString("e1x5Endcap_before").Prepend(folder));
  
  drawHistogram("ELE_E2X5MAX_BEFORECUTS",TString("e2x5Max_before").Prepend(folder));
  
  drawHistogram("ELE_E2X5MAXBARREL_BEFORECUTS",TString("e2x5MaxBarrel_before").Prepend(folder));
  
  drawHistogram("ELE_E2X5MAXENDCAP_BEFORECUTS",TString("e2x5MaxEndcap_before").Prepend(folder));
  
  drawHistogram("ELE_E5X5_BEFORECUTS",TString("e5x5_before").Prepend(folder));
  
  drawHistogram("ELE_E5X5BARREL_BEFORECUTS",TString("e5x5Barrel_before").Prepend(folder));
  
  drawHistogram("ELE_E5X5ENDCAP_BEFORECUTS",TString("e5x5Endcap_before").Prepend(folder));
  
  drawHistogram("ELE_R1X5_BEFORECUTS",TString("r1x5_before").Prepend(folder));
  
  drawHistogram("ELE_R1X5BARREL_BEFORECUTS",TString("r1x5Barrel_before").Prepend(folder));
  
  drawHistogram("ELE_R1X5ENDCAP_BEFORECUTS",TString("r1x5Endcap_before").Prepend(folder));
  
  drawHistogram("ELE_R2X5MAX_BEFORECUTS",TString("r2x5Max_before").Prepend(folder));
  
  drawHistogram("ELE_R2X5MAXBARREL_BEFORECUTS",TString("r2x5MaxBarrel_before").Prepend(folder));
  
  drawHistogram("ELE_R2X5MAXENDCAP_BEFORECUTS",TString("r2x5MaxEndcap_before").Prepend(folder));
  
  drawHistogram("ELE_SIGMAETAETA_BEFORECUTS",TString("sigmaEtaEta_before").Prepend(folder));
  
  drawHistogram("ELE_SIGMAETAETABARREL_BEFORECUTS",TString("sigmaEtaEtaBarrel_before").Prepend(folder));
  
  drawHistogram("ELE_SIGMAETAETAENDCAP_BEFORECUTS",TString("sigmaEtaEtaEndcap_before").Prepend(folder));
  
  drawHistogram("ELE_SIGMAIETAIETA_BEFORECUTS",TString("sigmaIetaIeta_before").Prepend(folder));
  
  drawHistogram("ELE_SIGMAIETAIETABARREL_BEFORECUTS",TString("sigmaIetaIetaBarrel_before").Prepend(folder));
  
  drawHistogram("ELE_SIGMAIETAIETAENDCAP_BEFORECUTS",TString("sigmaIetaIetaEndcap_before").Prepend(folder));
  
  drawHistogram("ELE_SCESEEDOVERESUPERCLUSTER_BEFORECUTS",TString("scESeedOverSuperCluster_before").Prepend(folder));
  
  drawHistogram("ELE_SCESEEDOVERESUPERCLUSTERBARREL_BEFORECUTS",TString("scESeedOverSuperClusterBarrel_before").Prepend(folder));
  
  drawHistogram("ELE_SCESEEDOVERESUPERCLUSTERENDCAP_BEFORECUTS",TString("scESeedOverSuperClusterEndcap_before").Prepend(folder));
  
  drawHistogram("ELE_SCETAWIDTH_BEFORECUTS",TString("scEtaWidth_before").Prepend(folder));
  
  drawHistogram("ELE_SCETAWIDTHBARREL_BEFORECUTS",TString("scEtaWidthBarrel_before").Prepend(folder));
  
  drawHistogram("ELE_SCETAWIDTHENDCAP_BEFORECUTS",TString("scEtaWidthEndcap_before").Prepend(folder));
  
  drawHistogram("ELE_SCPHIWIDTH_BEFORECUTS",TString("scPhiWidth_before").Prepend(folder));
  
  drawHistogram("ELE_SCPHIWIDTHBARREL_BEFORECUTS",TString("scPhiWidthBarrel_before").Prepend(folder));
  
  drawHistogram("ELE_SCPHIWIDTHENDCAP_BEFORECUTS",TString("scPhiWidthEndcap_before").Prepend(folder));
  
  //  drawHistogram("ELE_SCPRESHOWERENERGY_BEFORECUTS",TString("scPreshowerEnergy_before").Prepend(folder));
  
  //  drawHistogram("ELE_SCPRESHOWERENERGYBARREL_BEFORECUTS",TString("scPreshowerEnergyBarrel_before").Prepend(folder));
  
  drawHistogram("ELE_SCPRESHOWERENERGYENDCAP_BEFORECUTS",TString("scPreshowerEnergyEndcap_before").Prepend(folder));
  
  drawHistogram("ELE_SCR9_BEFORECUTS",TString("scR9_before").Prepend(folder));
  
  drawHistogram("ELE_SCR9BARREL_BEFORECUTS",TString("scR9Barrel_before").Prepend(folder));
  
  drawHistogram("ELE_SCR9ENDCAP_BEFORECUTS",TString("scR9Endcap_before").Prepend(folder));
  
  
  // 3x3
  drawHistogram("ELE_RMAX3X3_BEFORECUTS",TString("rMax3x3_before").Prepend(folder));
  
  drawHistogram("ELE_RMAX3X3BARREL_BEFORECUTS",TString("rMax3x3Barrel_before").Prepend(folder));
  
  drawHistogram("ELE_RMAX3X3ENDCAP_BEFORECUTS",TString("rMax3x3Endcap_before").Prepend(folder));
  
  
  
  
  // after
  //  drawHistogram("ELE_SIEIEBARREL_LASTCUT",TString("sieieBarrel_after").Prepend(folder));
  
  //  drawHistogram("ELE_SIEIEENDCAP_LASTCUT",TString("sieieEndcap_after").Prepend(folder));
  
  drawHistogram("ELE_E1X5_AFTERCUTS",TString("e1x5_after").Prepend(folder));
  
  drawHistogram("ELE_E1X5BARREL_AFTERCUTS",TString("e1x5Barrel_after").Prepend(folder));
  
  drawHistogram("ELE_E1X5ENDCAP_AFTERCUTS",TString("e1x5Endcap_after").Prepend(folder));
  
  drawHistogram("ELE_E2X5MAX_AFTERCUTS",TString("e2x5Max_after").Prepend(folder));
  
  drawHistogram("ELE_E2X5MAXBARREL_AFTERCUTS",TString("e2x5MaxBarrel_after").Prepend(folder));
  
  drawHistogram("ELE_E2X5MAXENDCAP_AFTERCUTS",TString("e2x5MaxEndcap_after").Prepend(folder));
  
  drawHistogram("ELE_E5X5_AFTERCUTS",TString("e5x5_after").Prepend(folder));
  
  drawHistogram("ELE_E5X5BARREL_AFTERCUTS",TString("e5x5Barrel_after").Prepend(folder));
  
  drawHistogram("ELE_E5X5ENDCAP_AFTERCUTS",TString("e5x5Endcap_after").Prepend(folder));
  
  drawHistogram("ELE_R1X5_AFTERCUTS",TString("r1x5_after").Prepend(folder));
  
  drawHistogram("ELE_R1X5BARREL_AFTERCUTS",TString("r1x5Barrel_after").Prepend(folder));
  
  drawHistogram("ELE_R1X5ENDCAP_AFTERCUTS",TString("r1x5Endcap_after").Prepend(folder));
  
  drawHistogram("ELE_R2X5MAX_AFTERCUTS",TString("r2x5Max_after").Prepend(folder));
  
  drawHistogram("ELE_R2X5MAXBARREL_AFTERCUTS",TString("r2x5MaxBarrel_after").Prepend(folder));
  
  drawHistogram("ELE_R2X5MAXENDCAP_AFTERCUTS",TString("r2x5MaxEndcap_after").Prepend(folder));
  
  drawHistogram("ELE_SIGMAETAETA_AFTERCUTS",TString("sigmaEtaEta_after").Prepend(folder));
  
  drawHistogram("ELE_SIGMAETAETABARREL_AFTERCUTS",TString("sigmaEtaEtaBarrel_after").Prepend(folder));
  
  drawHistogram("ELE_SIGMAETAETAENDCAP_AFTERCUTS",TString("sigmaEtaEtaEndcap_after").Prepend(folder));
  
  drawHistogram("ELE_SIGMAIETAIETA_AFTERCUTS",TString("sigmaIetaIeta_after").Prepend(folder));
  
  drawHistogram("ELE_SIGMAIETAIETABARREL_AFTERCUTS",TString("sigmaIetaIetaBarrel_after").Prepend(folder));
  
  drawHistogram("ELE_SIGMAIETAIETAENDCAP_AFTERCUTS",TString("sigmaIetaIetaEndcap_after").Prepend(folder));
  
  drawHistogram("ELE_SCESEEDOVERESUPERCLUSTER_AFTERCUTS",TString("scESeedOverSuperCluster_after").Prepend(folder));
  
  drawHistogram("ELE_SCESEEDOVERESUPERCLUSTERBARREL_AFTERCUTS",TString("scESeedOverSuperClusterBarrel_after").Prepend(folder));
  
  drawHistogram("ELE_SCESEEDOVERESUPERCLUSTERENDCAP_AFTERCUTS",TString("scESeedOverSuperClusterEndcap_after").Prepend(folder));
  
  drawHistogram("ELE_SCETAWIDTH_AFTERCUTS",TString("scEtaWidth_after").Prepend(folder));
  
  drawHistogram("ELE_SCETAWIDTHBARREL_AFTERCUTS",TString("scEtaWidthBarrel_after").Prepend(folder));
  
  drawHistogram("ELE_SCETAWIDTHENDCAP_AFTERCUTS",TString("scEtaWidthEndcap_after").Prepend(folder));
  
  drawHistogram("ELE_SCPHIWIDTH_AFTERCUTS",TString("scPhiWidth_after").Prepend(folder));
  
  drawHistogram("ELE_SCPHIWIDTHBARREL_AFTERCUTS",TString("scPhiWidthBarrel_after").Prepend(folder));
  
  drawHistogram("ELE_SCPHIWIDTHENDCAP_AFTERCUTS",TString("scPhiWidthEndcap_after").Prepend(folder));
  
  //  drawHistogram("ELE_SCPRESHOWERENERGY_AFTERCUTS",TString("scPreshowerEnergy_after").Prepend(folder));
  
  //  drawHistogram("ELE_SCPRESHOWERENERGYBARREL_AFTERCUTS",TString("scPreshowerEnergyBarrel_after").Prepend(folder));
  
  drawHistogram("ELE_SCPRESHOWERENERGYENDCAP_AFTERCUTS",TString("scPreshowerEnergyEndcap_after").Prepend(folder));
  
  drawHistogram("ELE_SCR9_AFTERCUTS",TString("scR9_after").Prepend(folder));
  
  drawHistogram("ELE_SCR9BARREL_AFTERCUTS",TString("scR9Barrel_after").Prepend(folder));
  
  drawHistogram("ELE_SCR9ENDCAP_AFTERCUTS",TString("scR9Endcap_after").Prepend(folder));
  
  // 3x3
  drawHistogram("ELE_RMAX3X3_AFTERCUTS",TString("rMax3x3_after").Prepend(folder));
  
  drawHistogram("ELE_RMAX3X3BARREL_AFTERCUTS",TString("rMax3x3Barrel_after").Prepend(folder));
  
  drawHistogram("ELE_RMAX3X3ENDCAP_AFTERCUTS",TString("rMax3x3Endcap_after").Prepend(folder));
  
}




// ELECTRON H/E 

//     * hcal depth 1 over ecal barrel/endcap/both
//     * hcal depth 2 over ecal barrel/endcap/both
//     * hcal over ecal barrel/endcap/both 

void makeElectronHoEPlots()
{
  
  TString folder("electronHoEPlots/");
  
  // before
  
  drawHistogram("ELE_HCALDEPTH1OVERECAL_BEFORECUTS",TString("hcalDepth1OverEcal_before").Prepend(folder));
  
  drawHistogram("ELE_HCALDEPTH1OVERECALBARREL_BEFORECUTS",TString("hcalDepth1OverEcalBarrel_before").Prepend(folder));
  
  drawHistogram("ELE_HCALDEPTH1OVERECALENDCAP_BEFORECUTS",TString("hcalDepth1OverEcalEndcap_before").Prepend(folder));
  
  drawHistogram("ELE_HCALDEPTH2OVERECAL_BEFORECUTS",TString("hcalDepth2OverEcal_before").Prepend(folder));
  
  drawHistogram("ELE_HCALDEPTH2OVERECALBARREL_BEFORECUTS",TString("hcalDepth2OverEcalBarrel_before").Prepend(folder));
  
  drawHistogram("ELE_HCALDEPTH2OVERECALENDCAP_BEFORECUTS",TString("hcalDepth2OverEcalEndcap_before").Prepend(folder));
  
  drawHistogram("ELE_HCALOVERECAL_BEFORECUTS",TString("hcalOverEcal_before").Prepend(folder));
  
  drawHistogram("ELE_HCALOVERECALBARREL_BEFORECUTS",TString("hcalOverEcalBarrel_before").Prepend(folder));
  
  drawHistogram("ELE_HCALOVERECALENDCAP_BEFORECUTS",TString("hcalOverEcalEndcap_before").Prepend(folder));
  
  
  // after
  
  drawHistogram("ELE_HCALDEPTH1OVERECAL_AFTERCUTS",TString("hcalDepth1OverEcal_after").Prepend(folder));
  
  drawHistogram("ELE_HCALDEPTH1OVERECALBARREL_AFTERCUTS",TString("hcalDepth1OverEcalBarrel_after").Prepend(folder));
  
  drawHistogram("ELE_HCALDEPTH1OVERECALENDCAP_AFTERCUTS",TString("hcalDepth1OverEcalEndcap_after").Prepend(folder));
  
  drawHistogram("ELE_HCALDEPTH2OVERECAL_AFTERCUTS",TString("hcalDepth2OverEcal_after").Prepend(folder));
  
  drawHistogram("ELE_HCALDEPTH2OVERECALBARREL_AFTERCUTS",TString("hcalDepth2OverEcalBarrel_after").Prepend(folder));
  
  drawHistogram("ELE_HCALDEPTH2OVERECALENDCAP_AFTERCUTS",TString("hcalDepth2OverEcalEndcap_after").Prepend(folder));
  
  drawHistogram("ELE_HCALOVERECAL_AFTERCUTS",TString("hcalOverEcal_after").Prepend(folder));
  
  drawHistogram("ELE_HCALOVERECALBARREL_AFTERCUTS",TString("hcalOverEcalBarrel_after").Prepend(folder));
  
  drawHistogram("ELE_HCALOVERECALENDCAP_AFTERCUTS",TString("hcalOverEcalEndcap_after").Prepend(folder));
  
}




// ELECTRON ISOLATION

//    * top-level ecal/hcal/track iso barrel/endcap
//    * dr 03/04 ecal barrel/endcap/both
//    * dr 03/04 hcal depth1/depth2/both barrel/endcap/both
//    * dr 03/04 track barrel/endcap/both 

void makeElectronIsolationPlots()
{
  
  TString folder("electronIsolationPlots/");
  
  // before cuts
  drawHistogram("ELE_DR03ECALRECHITSUMET_BEFORECUTS",TString("dr03EcalRecHitSumEt_before").Prepend(folder));
  
  drawHistogram("ELE_DR03ECALRECHITSUMETBARREL_BEFORECUTS",TString("dr03EcalRecHitSumEtBarrel_before").Prepend(folder));
  
  drawHistogram("ELE_DR03ECALRECHITSUMETENDCAP_BEFORECUTS",TString("dr03EcalRecHitSumEtEndcap_before").Prepend(folder));
  
  drawHistogram("ELE_DR03HCALTOWERSUMET_BEFORECUTS",TString("dr03HcalTowerSumEt_before").Prepend(folder));
  
  drawHistogram("ELE_DR03HCALTOWERSUMETBARREL_BEFORECUTS",TString("dr03HcalTowerSumEtBarrel_before").Prepend(folder));
  
  drawHistogram("ELE_DR03HCALTOWERSUMETENDCAP_BEFORECUTS",TString("dr03HcalTowerSumEtEndcap_before").Prepend(folder));
  
  drawHistogram("ELE_DR03HCALDEPTH1TOWERSUMET_BEFORECUTS",TString("dr03HcalDepth1TowerSumEt_before").Prepend(folder));
  
  drawHistogram("ELE_DR03HCALDEPTH1TOWERSUMETBARREL_BEFORECUTS",TString("dr03HcalDepth1TowerSumEtBarrel_before").Prepend(folder));
  
  drawHistogram("ELE_DR03HCALDEPTH1TOWERSUMETENDCAP_BEFORECUTS",TString("dr03HcalDepth1TowerSumEtEndcap_before").Prepend(folder));
  
  drawHistogram("ELE_DR03HCALDEPTH2TOWERSUMET_BEFORECUTS",TString("dr03HcalDepth2TowerSumEt_before").Prepend(folder));
  
  drawHistogram("ELE_DR03HCALDEPTH2TOWERSUMETBARREL_BEFORECUTS",TString("dr03HcalDepth2TowerSumEtBarrel_before").Prepend(folder));
  
  drawHistogram("ELE_DR03HCALDEPTH2TOWERSUMETENDCAP_BEFORECUTS",TString("dr03HcalDepth2TowerSumEtEndcap_before").Prepend(folder));
  
  drawHistogram("ELE_DR03TKSUMPT_BEFORECUTS",TString("dr03TkSumPt_before").Prepend(folder));
  
  drawHistogram("ELE_DR03TKSUMPTBARREL_BEFORECUTS",TString("dr03TkSumPtBarrel_before").Prepend(folder));
  
  drawHistogram("ELE_DR03TKSUMPTENDCAP_BEFORECUTS",TString("dr03TkSumPtEndcap_before").Prepend(folder));
  
  drawHistogram("ELE_DR04ECALRECHITSUMET_BEFORECUTS",TString("dr04EcalRecHitSumEt_before").Prepend(folder));
  
  drawHistogram("ELE_DR04ECALRECHITSUMETBARREL_BEFORECUTS",TString("dr04EcalRecHitSumEtBarrel_before").Prepend(folder));
  
  drawHistogram("ELE_DR04ECALRECHITSUMETENDCAP_BEFORECUTS",TString("dr04EcalRecHitSumEtEndcap_before").Prepend(folder));
  
  drawHistogram("ELE_DR04HCALTOWERSUMET_BEFORECUTS",TString("dr04HcalTowerSumEt_before").Prepend(folder));
  
  drawHistogram("ELE_DR04HCALTOWERSUMETBARREL_BEFORECUTS",TString("dr04HcalTowerSumEtBarrel_before").Prepend(folder));
  
  drawHistogram("ELE_DR04HCALTOWERSUMETENDCAP_BEFORECUTS",TString("dr04HcalTowerSumEtEndcap_before").Prepend(folder));
  
  drawHistogram("ELE_DR04HCALDEPTH1TOWERSUMET_BEFORECUTS",TString("dr04HcalDepth1TowerSumEt_before").Prepend(folder));
  
  drawHistogram("ELE_DR04HCALDEPTH1TOWERSUMETBARREL_BEFORECUTS",TString("dr04HcalDepth1TowerSumEtBarrel_before").Prepend(folder));
  
  drawHistogram("ELE_DR04HCALDEPTH1TOWERSUMETENDCAP_BEFORECUTS",TString("dr04HcalDepth1TowerSumEtEndcap_before").Prepend(folder));
  
  drawHistogram("ELE_DR04HCALDEPTH2TOWERSUMET_BEFORECUTS",TString("dr04HcalDepth2TowerSumEt_before").Prepend(folder));
  
  drawHistogram("ELE_DR04HCALDEPTH2TOWERSUMETBARREL_BEFORECUTS",TString("dr04HcalDepth2TowerSumEtBarrel_before").Prepend(folder));
  
  drawHistogram("ELE_DR04HCALDEPTH2TOWERSUMETENDCAP_BEFORECUTS",TString("dr04HcalDepth2TowerSumEtEndcap_before").Prepend(folder));
  
  drawHistogram("ELE_DR04TKSUMPT_BEFORECUTS",TString("dr04TkSumPt_before").Prepend(folder));
  
  drawHistogram("ELE_DR04TKSUMPTBARREL_BEFORECUTS",TString("dr04TkSumPtBarrel_before").Prepend(folder));
  
  drawHistogram("ELE_DR04TKSUMPTENDCAP_BEFORECUTS",TString("dr04TkSumPtEndcap_before").Prepend(folder));
  
  
  // after cuts
  drawHistogram("ELE_DR03ECALRECHITSUMET_AFTERCUTS",TString("dr03EcalRecHitSumEt_after").Prepend(folder));
  
  drawHistogram("ELE_DR03ECALRECHITSUMETBARREL_AFTERCUTS",TString("dr03EcalRecHitSumEtBarrel_after").Prepend(folder));
  
  drawHistogram("ELE_DR03ECALRECHITSUMETENDCAP_AFTERCUTS",TString("dr03EcalRecHitSumEtEndcap_after").Prepend(folder));
  
  drawHistogram("ELE_DR03HCALTOWERSUMET_AFTERCUTS",TString("dr03HcalTowerSumEt_after").Prepend(folder));
  
  drawHistogram("ELE_DR03HCALTOWERSUMETBARREL_AFTERCUTS",TString("dr03HcalTowerSumEtBarrel_after").Prepend(folder));
  
  drawHistogram("ELE_DR03HCALTOWERSUMETENDCAP_AFTERCUTS",TString("dr03HcalTowerSumEtEndcap_after").Prepend(folder));
  
  drawHistogram("ELE_DR03HCALDEPTH1TOWERSUMET_AFTERCUTS",TString("dr03HcalDepth1TowerSumEt_after").Prepend(folder));
  
  drawHistogram("ELE_DR03HCALDEPTH1TOWERSUMETBARREL_AFTERCUTS",TString("dr03HcalDepth1TowerSumEtBarrel_after").Prepend(folder));
  
  drawHistogram("ELE_DR03HCALDEPTH1TOWERSUMETENDCAP_AFTERCUTS",TString("dr03HcalDepth1TowerSumEtEndcap_after").Prepend(folder));
  
  drawHistogram("ELE_DR03HCALDEPTH2TOWERSUMET_AFTERCUTS",TString("dr03HcalDepth2TowerSumEt_after").Prepend(folder));
  
  drawHistogram("ELE_DR03HCALDEPTH2TOWERSUMETBARREL_AFTERCUTS",TString("dr03HcalDepth2TowerSumEtBarrel_after").Prepend(folder));
  
  drawHistogram("ELE_DR03HCALDEPTH2TOWERSUMETENDCAP_AFTERCUTS",TString("dr03HcalDepth2TowerSumEtEndcap_after").Prepend(folder));
  
  drawHistogram("ELE_DR03TKSUMPT_AFTERCUTS",TString("dr03TkSumPt_after").Prepend(folder));
  
  drawHistogram("ELE_DR03TKSUMPTBARREL_AFTERCUTS",TString("dr03TkSumPtBarrel_after").Prepend(folder));
  
  drawHistogram("ELE_DR03TKSUMPTENDCAP_AFTERCUTS",TString("dr03TkSumPtEndcap_after").Prepend(folder));
  
  drawHistogram("ELE_DR04ECALRECHITSUMET_AFTERCUTS",TString("dr04EcalRecHitSumEt_after").Prepend(folder));
  
  drawHistogram("ELE_DR04ECALRECHITSUMETBARREL_AFTERCUTS",TString("dr04EcalRecHitSumEtBarrel_after").Prepend(folder));
  
  drawHistogram("ELE_DR04ECALRECHITSUMETENDCAP_AFTERCUTS",TString("dr04EcalRecHitSumEtEndcap_after").Prepend(folder));
  
  drawHistogram("ELE_DR04HCALTOWERSUMET_AFTERCUTS",TString("dr04HcalTowerSumEt_after").Prepend(folder));
  
  drawHistogram("ELE_DR04HCALTOWERSUMETBARREL_AFTERCUTS",TString("dr04HcalTowerSumEtBarrel_after").Prepend(folder));
  
  drawHistogram("ELE_DR04HCALTOWERSUMETENDCAP_AFTERCUTS",TString("dr04HcalTowerSumEtEndcap_after").Prepend(folder));
  
  drawHistogram("ELE_DR04HCALDEPTH1TOWERSUMET_AFTERCUTS",TString("dr04HcalDepth1TowerSumEt_after").Prepend(folder));
  
  drawHistogram("ELE_DR04HCALDEPTH1TOWERSUMETBARREL_AFTERCUTS",TString("dr04HcalDepth1TowerSumEtBarrel_after").Prepend(folder));
  
  drawHistogram("ELE_DR04HCALDEPTH1TOWERSUMETENDCAP_AFTERCUTS",TString("dr04HcalDepth1TowerSumEtEndcap_after").Prepend(folder));
  
  drawHistogram("ELE_DR04HCALDEPTH2TOWERSUMET_AFTERCUTS",TString("dr04HcalDepth2TowerSumEt_after").Prepend(folder));
  
  drawHistogram("ELE_DR04HCALDEPTH2TOWERSUMETBARREL_AFTERCUTS",TString("dr04HcalDepth2TowerSumEtBarrel_after").Prepend(folder));
  
  drawHistogram("ELE_DR04HCALDEPTH2TOWERSUMETENDCAP_AFTERCUTS",TString("dr04HcalDepth2TowerSumEtEndcap_after").Prepend(folder));
  
  drawHistogram("ELE_DR04TKSUMPT_AFTERCUTS",TString("dr04TkSumPt_after").Prepend(folder));
  
  drawHistogram("ELE_DR04TKSUMPTBARREL_AFTERCUTS",TString("dr04TkSumPtBarrel_after").Prepend(folder));
  
  drawHistogram("ELE_DR04TKSUMPTENDCAP_AFTERCUTS",TString("dr04TkSumPtEndcap_after").Prepend(folder));
  
}





// ELECTRON TRACK EXTRAPOLATION 

//    * track momentum at calo/vtx x/y/z
//    * track position at calo/vtx x/y/z 

void makeElectronTrackExtrapolationPlots()
{
  
  TString folder("electronTrackExtrapolationPlots/");
  
  // before
  
  drawHistogram("ELE_TRACKPOSITIONATCALOX_BEFORECUTS",TString("trkPosAtCaloX_before").Prepend(folder));
  
  drawHistogram("ELE_TRACKPOSITIONATCALOY_BEFORECUTS",TString("trkPosAtCaloY_before").Prepend(folder));
  
  drawHistogram("ELE_TRACKPOSITIONATCALOZ_BEFORECUTS",TString("trkPosAtCaloZ_before").Prepend(folder));
  
  drawHistogram("ELE_TRACKPOSITIONATVTXX_BEFORECUTS",TString("trkPosAtVtxX_before").Prepend(folder));
  
  drawHistogram("ELE_TRACKPOSITIONATVTXY_BEFORECUTS",TString("trkPosAtVtxY_before").Prepend(folder));
  
  drawHistogram("ELE_TRACKPOSITIONATVTXZ_BEFORECUTS",TString("trkPosAtVtxZ_before").Prepend(folder));
  
  drawHistogram("ELE_TRACKMOMENTUMATCALOX_BEFORECUTS",TString("trkMomAtCaloX_before").Prepend(folder));
  
  drawHistogram("ELE_TRACKMOMENTUMATCALOY_BEFORECUTS",TString("trkMomAtCaloY_before").Prepend(folder));
  
  drawHistogram("ELE_TRACKMOMENTUMATCALOZ_BEFORECUTS",TString("trkMomAtCaloZ_before").Prepend(folder));
  
  drawHistogram("ELE_TRACKMOMENTUMATVTXX_BEFORECUTS",TString("trkMomAtVtxX_before").Prepend(folder));
  
  drawHistogram("ELE_TRACKMOMENTUMATVTXY_BEFORECUTS",TString("trkMomAtVtxY_before").Prepend(folder));
  
  drawHistogram("ELE_TRACKMOMENTUMATVTXZ_BEFORECUTS",TString("trkMomAtVtxZ_before").Prepend(folder));
  
  
  // after
  
  drawHistogram("ELE_TRACKPOSITIONATCALOX_AFTERCUTS",TString("trkPosAtCaloX_after").Prepend(folder));
  
  drawHistogram("ELE_TRACKPOSITIONATCALOY_AFTERCUTS",TString("trkPosAtCaloY_after").Prepend(folder));
  
  drawHistogram("ELE_TRACKPOSITIONATCALOZ_AFTERCUTS",TString("trkPosAtCaloZ_after").Prepend(folder));
  
  drawHistogram("ELE_TRACKPOSITIONATVTXX_AFTERCUTS",TString("trkPosAtVtxX_after").Prepend(folder));
  
  drawHistogram("ELE_TRACKPOSITIONATVTXY_AFTERCUTS",TString("trkPosAtVtxY_after").Prepend(folder));
  
  drawHistogram("ELE_TRACKPOSITIONATVTXZ_AFTERCUTS",TString("trkPosAtVtxZ_after").Prepend(folder));
  
  drawHistogram("ELE_TRACKMOMENTUMATCALOX_AFTERCUTS",TString("trkMomAtCaloX_after").Prepend(folder));
  
  drawHistogram("ELE_TRACKMOMENTUMATCALOY_AFTERCUTS",TString("trkMomAtCaloY_after").Prepend(folder));
  
  drawHistogram("ELE_TRACKMOMENTUMATCALOZ_AFTERCUTS",TString("trkMomAtCaloZ_after").Prepend(folder));
  
  drawHistogram("ELE_TRACKMOMENTUMATVTXX_AFTERCUTS",TString("trkMomAtVtxX_after").Prepend(folder));
  
  drawHistogram("ELE_TRACKMOMENTUMATVTXY_AFTERCUTS",TString("trkMomAtVtxY_after").Prepend(folder));
  
  drawHistogram("ELE_TRACKMOMENTUMATVTXZ_AFTERCUTS",TString("trkMomAtVtxZ_after").Prepend(folder));
  
}



// ELECTRON TRACK-CLUSTER MATCHING

//    * top-level delta eta in barrel/endcap
//    * delta eta,phi ele cluster track at calo
//    * delta eta,phi seed cluster track at calo
//    * delta eta,phi supercluster track at vertex
//    * e ele cluster / pout
//    * e seed cluster / pout
//    * e seed cluster / p
//    * e super cluster / p 

void makeElectronTrackClusterMatchPlots()
{
  
  TString folder("electronTrackClusterMatchPlots/");
  
  // before
  
  //  drawHistogram("ELE_DETAINBARREL_BEFORECUTS",TString("detainBarrel_before").Prepend(folder));
  
  //  drawHistogram("ELE_DETAINENDCAP_BEFORECUTS",TString("detainEndcap_before").Prepend(folder));
  
  drawHistogram("ELE_DELTAETAELECLUSTERTRACKATCALO_BEFORECUTS",TString("dEtaEleClusTrkAtCalo_before").Prepend(folder));
  
  drawHistogram("ELE_DELTAETASEEDCLUSTERTRACKATCALO_BEFORECUTS",TString("dEtaSeedClusTrkAtCalo_before").Prepend(folder));
  
  drawHistogram("ELE_DELTAETASUPERCLUSTERTRACKATVTX_BEFORECUTS",TString("dEtaSuperClusTrkAtCalo_before").Prepend(folder));
  
  drawHistogram("ELE_DELTAPHIELECLUSTERTRACKATCALO_BEFORECUTS",TString("dPhiEleClusTrkAtCalo_before").Prepend(folder));
  
  drawHistogram("ELE_DELTAPHISEEDCLUSTERTRACKATCALO_BEFORECUTS",TString("dPhiSeedClusTrkAtCalo_before").Prepend(folder));
  
  drawHistogram("ELE_DELTAPHISUPERCLUSTERTRACKATVTX_BEFORECUTS",TString("dPhiSuperClusTrkAtVtx_before").Prepend(folder));
  
  drawHistogram("ELE_EELECLUSTEROVERPOUT_BEFORECUTS",TString("eEleClusOverPout_before").Prepend(folder));
  
  drawHistogram("ELE_ESEEDCLUSTEROVERP_BEFORECUTS",TString("eSeedClusOverP_before").Prepend(folder));
  
  drawHistogram("ELE_ESEEDCLUSTEROVERPOUT_BEFORECUTS",TString("eSeedClusOverPout_before").Prepend(folder));
  
  drawHistogram("ELE_ESUPERCLUSTEROVERP_BEFORECUTS",TString("eSuperClusOverP_before").Prepend(folder));
  
  
  
  // after
  
  //  drawHistogram("ELE_DETAINBARREL_LASTCUT",TString("detainBarrel_after").Prepend(folder));
  
  //  drawHistogram("ELE_DETAINENDCAP_LASTCUT",TString("detainEndcap_after").Prepend(folder));
  
  drawHistogram("ELE_DELTAETAELECLUSTERTRACKATCALO_AFTERCUTS",TString("dEtaEleClusTrkAtCalo_after").Prepend(folder));
  
  drawHistogram("ELE_DELTAETASEEDCLUSTERTRACKATCALO_AFTERCUTS",TString("dEtaSeedClusTrkAtCalo_after").Prepend(folder));
  
  drawHistogram("ELE_DELTAETASUPERCLUSTERTRACKATVTX_AFTERCUTS",TString("dEtaSuperClusTrkAtCalo_after").Prepend(folder));
  
  drawHistogram("ELE_DELTAPHIELECLUSTERTRACKATCALO_AFTERCUTS",TString("dPhiEleClusTrkAtCalo_after").Prepend(folder));
  
  drawHistogram("ELE_DELTAPHISEEDCLUSTERTRACKATCALO_AFTERCUTS",TString("dPhiSeedClusTrkAtCalo_after").Prepend(folder));
  
  drawHistogram("ELE_DELTAPHISUPERCLUSTERTRACKATVTX_AFTERCUTS",TString("dPhiSuperClusTrkAtVtx_after").Prepend(folder));
  
  drawHistogram("ELE_EELECLUSTEROVERPOUT_AFTERCUTS",TString("eEleClusOverPout_after").Prepend(folder));
  
  drawHistogram("ELE_ESEEDCLUSTEROVERP_AFTERCUTS",TString("eSeedClusOverP_after").Prepend(folder));
  
  drawHistogram("ELE_ESEEDCLUSTEROVERPOUT_AFTERCUTS",TString("eSeedClusOverPout_after").Prepend(folder));
  
  drawHistogram("ELE_ESUPERCLUSTEROVERP_AFTERCUTS",TString("eSuperClusOverP_after").Prepend(folder));
  
}



// ELECTRON VERTEX PLOTS

//     * chi2
//     * normalized chi2
//     * number of degrees of freedom
//     * position x/y/z  

void makeElectronVertexPlots()
{
  
  TString folder("electronVertexPlots/");
  
  // before
  
  //  drawHistogram("ELE_VERTEXCHI2_BEFORECUTS",TString("vertexChi2_before").Prepend(folder));  // empty so far!
  
  //  drawHistogram("ELE_VERTEXNORMALIZEDCHI2_BEFORECUTS",TString("vertexNormalizedChi2_before").Prepend(folder)); // ditto
  
  //  drawHistogram("ELE_VERTEXNDOF_BEFORECUTS",TString("vertexNDoF_before").Prepend(folder)); // ditto
  
  drawHistogram("ELE_VX_BEFORECUTS",TString("vx_before").Prepend(folder));
  
  drawHistogram("ELE_VY_BEFORECUTS",TString("vy_before").Prepend(folder));
  
  drawHistogram("ELE_VZ_BEFORECUTS",TString("vz_before").Prepend(folder));
  
  
  // after
  
  //  drawHistogram("ELE_VERTEXCHI2_AFTERCUTS",TString("vertexChi2_after").Prepend(folder));
  
  //  drawHistogram("ELE_VERTEXNORMALIZEDCHI2_AFTERCUTS",TString("vertexNormalizedChi2_after").Prepend(folder));
  
  //  drawHistogram("ELE_VERTEXNDOF_AFTERCUTS",TString("vertexNDoF_after").Prepend(folder));
  
  drawHistogram("ELE_VX_AFTERCUTS",TString("vx_after").Prepend(folder));
  
  drawHistogram("ELE_VY_AFTERCUTS",TString("vy_after").Prepend(folder));
  
  drawHistogram("ELE_VZ_AFTERCUTS",TString("vz_after").Prepend(folder));
  
}




// ELECTRON CHARGE INFORMATION

//    * electron charge
//    * sc pix charge
//    * gsf track charge
//    * sc pix / gsf track charge consistency 

void makeElectronChargePlots()
{
  
  TString folder("electronChargePlots/");
  
  // before
  
  drawHistogram("ELE_CHARGE_BEFORECUTS",TString("charge_before").Prepend(folder));
  
  drawHistogram("ELE_SCPIXCHARGE_BEFORECUTS",TString("scPixCharge_before").Prepend(folder));
  
  drawHistogram("ELE_GSFCHARGE_BEFORECUTS",TString("gsfCharge_before").Prepend(folder));
  
  drawHistogram("ELE_CTFCHARGE_BEFORECUTS",TString("ctfCharge_before").Prepend(folder));
  
  drawHistogram("ELE_GSFSCPIXCHARGECONSISTENT_BEFORECUTS",TString("gsfScPixChargeConsistent_before").Prepend(folder));
  
  drawHistogram("ELE_GSFCTFCHARGECONSISTENT_BEFORECUTS",TString("gsfCtfChargeConsistent_before").Prepend(folder));
  
  drawHistogram("ELE_GSFCTFSCPIXCHARGECONSISTENT_BEFORECUTS",TString("gsfCtfScPixChargeConsistent_before").Prepend(folder));
  
  
  // after
  
  drawHistogram("ELE_CHARGE_AFTERCUTS",TString("charge_after").Prepend(folder));
  
  drawHistogram("ELE_SCPIXCHARGE_AFTERCUTS",TString("scPixCharge_after").Prepend(folder));
  
  drawHistogram("ELE_GSFCHARGE_AFTERCUTS",TString("gsfCharge_after").Prepend(folder));
  
  drawHistogram("ELE_CTFCHARGE_AFTERCUTS",TString("ctfCharge_after").Prepend(folder));
  
  drawHistogram("ELE_GSFSCPIXCHARGECONSISTENT_AFTERCUTS",TString("gsfScPixChargeConsistent_after").Prepend(folder));
  
  drawHistogram("ELE_GSFCTFCHARGECONSISTENT_AFTERCUTS",TString("gsfCtfChargeConsistent_after").Prepend(folder));
  
  drawHistogram("ELE_GSFCTFSCPIXCHARGECONSISTENT_AFTERCUTS",TString("gsfCtfScPixChargeConsistent_after").Prepend(folder));
  
}




// ELECTRON MISCELLANEOUS PLOTS

//     * basic clusters size
//     * dxy
//     * fbrem
//     * number of electrons
//     * number of brems
//     * specified trigger fires
//     * all triggers of interest fired 
//     * inv mass

void makeElectronMiscPlots()
{
  
  TString folder("electronMiscPlots/");
  
  // before
  
  drawHistogram("ELE_BASICCLUSTERSSIZE_BEFORECUTS",TString("basicClustersSize_before").Prepend(folder));
  
  //  drawHistogram("ELE_DXY_BEFORECUTS",TString("dxy_before").Prepend(folder));
  
  //  drawHistogram("ELE_FBREM_BEFORECUTS",TString("fBrem_before").Prepend(folder));
  
  //  drawHistogram("ELE_NELECTRONS_BEFORECUTS",TString("nElectrons_before").Prepend(folder));
  
  drawHistogram("ELE_NUMBEROFBREMS_BEFORECUTS",TString("numberOfBrems_before").Prepend(folder));
  
  //  drawHistogram("ELE_TRIG_BEFORECUTS",TString("trig_before").Prepend(folder));
  
  //  drawHistogram("ELE_TRIGGERSFIRED_BEFORECUTS",TString("triggersFired_before").Prepend(folder));
  
  //  drawHistogram("ELE_INVMASS_BEFORECUTS",TString("invMass_before").Prepend(folder));
  
  
  // after
  
  drawHistogram("ELE_BASICCLUSTERSSIZE_AFTERCUTS",TString("basicClustersSize_after").Prepend(folder));
  
  //  drawHistogram("ELE_DXY_AFTERCUTS",TString("dxy_after").Prepend(folder));
  
  //  drawHistogram("ELE_FBREM_AFTERCUTS",TString("fBrem_after").Prepend(folder));
  
  //  drawHistogram("ELE_NELECTRONS_AFTERCUTS",TString("nElectrons_after").Prepend(folder));
  
  drawHistogram("ELE_NUMBEROFBREMS_AFTERCUTS",TString("numberOfBrems_after").Prepend(folder));
  
  //  drawHistogram("ELE_TRIG_LASTCUT",TString("trig_after").Prepend(folder));
  
  //  drawHistogram("ELE_TRIGGERSFIRED_AFTERCUTS",TString("triggersFired_after").Prepend(folder));
  
  //  drawHistogram("ELE_INVMASS_AFTERCUTS",TString("invMass_after").Prepend(folder));
  
}




// Z!
    
//     *  mass
//     * pt
//     * eta
//     * phi 
    
void makeZPlots()
{
  
  TString folder("zPlots/");
  
  // before
  
  drawHistogram("Z_MASS_BEFORECUTS",TString("z_mass_before").Prepend(folder));
  
  drawHistogram("Z_PT_BEFORECUTS",TString("z_pt_before").Prepend(folder));
  
  drawHistogram("Z_ETA_BEFORECUTS",TString("z_eta_before").Prepend(folder));
  
  drawHistogram("Z_PHI_BEFORECUTS",TString("z_phi_before").Prepend(folder));
  
  
  
  // after
  
  drawHistogram("Z_MASS_AFTERCUTS",TString("z_mass_after").Prepend(folder));
  
  drawHistogram("Z_PT_AFTERCUTS",TString("z_pt_after").Prepend(folder));
  
  drawHistogram("Z_ETA_AFTERCUTS",TString("z_eta_after").Prepend(folder));
  
  drawHistogram("Z_PHI_AFTERCUTS",TString("z_phi_after").Prepend(folder));
  
}




// SUPERCLUSTERS

//     *  energy barrel/endcap/both
//     * eta
//     * eta width barrel/endcap/both
//     * n basic clusters barrel/endcap/both
//     * n super clusters
//     * phi barrel/endcap/both
//     * phi width barrel/endcap/both
//     * preshower energy barrel/endcap/both
//     * raw energy barrel/endcap/both 

void makeSuperClusterPlots()
{
  
  TString folder("superClusterPlots/");
  
  drawHistogram("SC_ENERGY",TString("sc_energy").Prepend(folder));
  
  drawHistogram("SC_ENERGYBARREL",TString("sc_energyBarrel").Prepend(folder));
  
  drawHistogram("SC_ENERGYENDCAP",TString("sc_energyEndcap").Prepend(folder));
  
  drawHistogram("SC_ETA",TString("sc_eta").Prepend(folder));
  
  drawHistogram("SC_ETAWIDTH",TString("sc_etaWidth").Prepend(folder));
  
  drawHistogram("SC_ETAWIDTHBARREL",TString("sc_etaWidthBarrel").Prepend(folder));
  
  drawHistogram("SC_ETAWIDTHENDCAP",TString("sc_etaWidthEndcap").Prepend(folder));
  
  drawHistogram("SC_NBASICCLUSTERS",TString("sc_nBasicClusters").Prepend(folder));
  
  drawHistogram("SC_NBASICCLUSTERSBARREL",TString("sc_nBasicClustersBarrel").Prepend(folder));
  
  drawHistogram("SC_NBASICCLUSTERSENDCAP",TString("sc_nBasicClustersEndcap").Prepend(folder));
  
  drawHistogram("SC_NSUPERCLUSTERS",TString("sc_nSuperClusters").Prepend(folder));
  
  drawHistogram("SC_PHI",TString("sc_phi").Prepend(folder));
  
  drawHistogram("SC_PHIBARREL",TString("sc_phiBarrel").Prepend(folder));
  
  drawHistogram("SC_PHIENDCAP",TString("sc_phiEndcap").Prepend(folder));
  
  drawHistogram("SC_PHIWIDTH",TString("sc_phiWidth").Prepend(folder));
  
  drawHistogram("SC_PHIWIDTHBARREL",TString("sc_phiWidthBarrel").Prepend(folder));
  
  drawHistogram("SC_PHIWIDTHENDCAP",TString("sc_phiWidthEndcap").Prepend(folder));
  
  //  drawHistogram("SC_PRESHOWERENERGY",TString("sc_preshowerEnergy").Prepend(folder));
  
  //  drawHistogram("SC_PRESHOWERENERGYBARREL",TString("sc_preshowerEnergyBarrel").Prepend(folder));
  
  drawHistogram("SC_PRESHOWERENERGYENDCAP",TString("sc_preshowerEnergyEndcap").Prepend(folder));
  
  drawHistogram("SC_RAWENERGY",TString("sc_rawEnergy").Prepend(folder));
  
  drawHistogram("SC_RAWENERGYBARREL",TString("sc_rawEnergyBarrel").Prepend(folder));
  
  drawHistogram("SC_RAWENERGYENDCAP",TString("sc_rawEnergyEndcap").Prepend(folder));
  
}












    
