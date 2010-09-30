#include <ctime>
#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include <vector>

#include "TCanvas.h"
#include "TFile.h"
#include "TGraphAsymmErrors.h"
#include "TH1D.h"
#include "TLegend.h"
#include "TPaveText.h"
#include "TStyle.h"

TFile *fileData = new TFile("Plots_Slimmer_MBMay27thReReco_v1_V11.root");
TFile *fileMcMB = new TFile("Plots_Slimmer_SampleMBV11.root");
TFile *fileMcZ  = new TFile("Plots_Slimmer_SampleZV11.root");

void setTDRStyle();

/// convert int to string
std::string itos(int i)
{
    std::stringstream s;
    s << i;
    return s.str();
}

std::vector<std::string> 
stringParser(std::string cutString)
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



TCanvas* makeCan(TString noTrig, TString trig, unsigned int cnt)
{
  TCanvas* c1 = new TCanvas(trig, trig);
  c1->SetGridx(true);
  c1->SetGridy(true);
  
  TString xTitle = "";
  TString yTitle = "";
  
  if ( trig.Contains("ScEt") )
    xTitle = "ScEt";
  else if ( trig.Contains("Eta") )
    xTitle = "#eta";
  else if ( trig.Contains("Phi") )
    xTitle = "#phi";
  
  if ( trig.Contains("SingleEG5") )
    yTitle = "L1 Single EG5 efficiency";
  else if ( trig.Contains("SingleEG8") )
    yTitle = "L1 Single EG8 efficiency";
  else if ( trig.Contains("HLT_Ele10") )
    yTitle = "HLT Electron10 efficiency";
  else if ( trig.Contains("HLT_Ele10") )
    yTitle = "HLT Electron15 efficiency";
  else if ( trig.Contains("HLT_Photon10") )
    yTitle = "HLT Photon10 efficiency";
  else if ( trig.Contains("HLT_Photon15_") )
    yTitle = "HLT Photon15 efficiency";
  else if ( trig.Contains("HLT_Photon15New") )
    yTitle = "HLT Photon15 w/EG5 seed efficiency";

  cout << cnt << " ~ " << trig << ": getting... data";
  TH1D *histDataNoTrig = (TH1D*)fileData->Get(noTrig)->Clone();
  TH1D *histDataTrig   = (TH1D*)fileData->Get(trig)->Clone();
    
  cout << " MB";
  TH1D *histMcMbNoTrig = (TH1D*)fileMcMB->Get(noTrig)->Clone();
  TH1D *histMcMbTrig   = (TH1D*)fileMcMB->Get(trig)->Clone();
  
  cout << " Z";
  TH1D *histMcZNoTrig = (TH1D*)fileMcZ->Get(noTrig)->Clone();
  TH1D *histMcZTrig   = (TH1D*)fileMcZ->Get(trig)->Clone();
 
  cout << ";dividing... data";    
  TGraphAsymmErrors * graphData = new TGraphAsymmErrors(); 
  graphData->BayesDivide(histDataTrig, histDataNoTrig,"w");
  graphData->GetYaxis()->SetRangeUser(0, 1.1);
  graphData->SetMarkerColor(1);
  graphData->SetMarkerStyle(8);
  graphData->SetMarkerSize(0.8);
  graphData->SetLineWidth(2);

  cout << " MB";
  TGraphAsymmErrors * graphMcMb = new TGraphAsymmErrors(); 
  graphMcMb->BayesDivide(histMcMbTrig, histMcMbNoTrig,"w");
  graphMcMb->GetYaxis()->SetRangeUser(0, 1.1);
  graphMcMb->SetLineColor(4);  
  graphMcMb->SetMarkerColor(4);
  graphMcMb->SetMarkerStyle(3);
  graphMcMb->SetLineWidth(2);

  cout << " Z;";
  TGraphAsymmErrors * graphMcZ = new TGraphAsymmErrors(); 
  graphMcZ->BayesDivide(histMcZTrig, histMcZNoTrig,"w");
  graphMcZ->GetYaxis()->SetRangeUser(0, 1.1);
  graphMcZ->GetYaxis()->SetTitle(yTitle);
  graphMcZ->GetXaxis()->SetTitle(xTitle);
  graphMcZ->SetLineColor(kRed);  
  graphMcZ->SetMarkerColor(kRed);
  graphMcZ->SetMarkerStyle(24);
  graphMcZ->SetLineWidth(2);
  
  cout << " creating legend;";
  TLegend *leg = new TLegend(0.71,0.16,0.95,0.33);
  leg->SetFillStyle(0);
  leg->AddEntry(graphData,"Data","lp");
  leg->AddEntry(graphMcMb,"MinBias MC",  "lp");
  leg->AddEntry(graphMcZ ,"Z MC",  "lp");
  
//   TPaveText * pav = new TPaveText(0.65,0.36,0.95,0.50);
//   pav->AddText("CMS");
  
  cout << " drawing;";
  graphMcZ->Draw("AP");
  graphMcMb->Draw("P same");
  graphData->Draw("P same");
//   graphData->Draw("AP");
  leg->Draw("same");
//   pav->Draw("same");
  
  c1->Modified();
  c1->cd();
  c1->SaveAs("SAVE/eff"+itos(cnt)+"_"+trig+".gif");
  c1->SaveAs("SAVE/eff"+itos(cnt)+"_"+trig+".pdf");
//   c1->SaveAs("SAVE/eff"+itos(cnt)+"_"+trig+".C");

  cout << " done!\n";
  return c1;
}




void divide()
{
  setTDRStyle();
  
  TFile* oFile = TFile::Open( "SAVE/histos.root", "RECREATE" );
  
  std::vector<TString> denoms;
  std::vector<TString> enums;
  std::vector<unsigned int> counts;
  
//   std::string    cutList_  =  "NoCuts;SpikesRemoved;SCEt";
//    cutList_ += ";MissingHits;Dist;deltaCotTheta";
//    cutList_ += ";RelIsoTrk;RelIsoEcal;RelIsoHcal";
//    cutList_ += ";SigIeIe;deltaPhiIn;deltaEtaIn;HoE";

  std::string cutList_ = "AllCuts";

   
  std::string triggersOfInterest = "L1_SingleEG5;L1_SingleEG8;HLT_Photon10_L1R;HLT_Photon15_L1R;HLT_Photon15NewThresh_L1R"; 
  // L1_SingleEG5;L1_SingleEG8;HLT_Photon10_L1R;HLT_Photon15_L1R;HLT_Photon15NewThresh_L1R
//   std::string triggersOfInterest = "HLT_Photon10_L1R;HLT_Photon15_L1R;HLT_Ele10_LW_L1R;HLT_Ele15_LW_L1R";

  std::map<std::string, std::string> relTrigEff; // L1 seed, HLT
   relTrigEff["HLT_Photon10_L1R"] = "L1_SingleEG5";
   relTrigEff["HLT_Photon15_L1R"] = "L1_SingleEG8";
   relTrigEff["HLT_Photon15NewThresh_L1R"] = "L1_SingleEG5";
//  relTrigEff["HLT_Ele10_LW_L1R"] = "L1_SingleEG5";
//  relTrigEff["HLT_Ele15_LW_L1R"] = "L1_SingleEG8";
    
  std::string wpList_ = "WP80;WP95";
  std::string where[3] = { "ALL", "EB", "EE"};
 
  std::vector<std::string> trigVec = stringParser(triggersOfInterest);
  std::vector<std::string> cutVec = stringParser(cutList_);
  std::vector<std::string> wpVec = stringParser(wpList_);
  

  unsigned long count = 0; 

  for ( unsigned int w = 0; w < wpVec.size(); ++w )
  {
    count += 10000000;
    for ( unsigned int t = 0; t < trigVec.size(); ++t )
    {
      count += 100000;
      for ( unsigned int c = 0; c < cutVec.size(); ++c )
      {
        count += 1000;
        for ( unsigned int p = 0; p < 3; ++p )
        {
          count++;
          counts.push_back(count);
          enums.push_back( (TString)((wpVec.at(w))+"_"+(trigVec.at(t))+"_"+(cutVec.at(c))+"_"+where[p]+"_ScEt") );
          denoms.push_back( (TString)((wpVec.at(w))+"_NoTrigger_"+(cutVec.at(c))+"_"+where[p]+"_ScEt") );
          count++;
          counts.push_back(count);
          enums.push_back( (TString)((wpVec.at(w))+"_"+(trigVec.at(t))+"_"+(cutVec.at(c))+"_"+where[p]+"_Eta") );
          denoms.push_back( (TString)((wpVec.at(w))+"_NoTrigger_"+(cutVec.at(c))+"_"+where[p]+"_Eta") );
          count++;
          counts.push_back(count);
          enums.push_back( (TString)((wpVec.at(w))+"_"+(trigVec.at(t))+"_"+(cutVec.at(c))+"_"+where[p]+"_Phi") );
          denoms.push_back( (TString)((wpVec.at(w))+"_NoTrigger_"+(cutVec.at(c))+"_"+where[p]+"_Phi") );       
//          count++;
//          counts.push_back(count);
//          enums.push_back( (TString)((wpVec.at(w))+"_"+(trigVec.at(t))+"_"+(cutVec.at(c))+"_"+where[p]+"_Pt") );
//          denoms.push_back( (TString)((wpVec.at(w))+"_NoTrigger_"+(cutVec.at(c))+"_"+where[p]+"_Pt") );     
        }
      }
    }
  }


  count += 500000000;

  for ( unsigned int w = 0; w < wpVec.size(); ++w )
  {
    count += 1000000;
    for ( std::map<std::string, std::string>::iterator itr = relTrigEff.begin(); itr != relTrigEff.end(); ++itr )
    {
      count += 10000;
      for ( unsigned int c = 0; c < cutVec.size(); ++c )
      {
        count += 100;
        for ( unsigned int p = 0; p < 3; ++p )
        {
          count++;
          counts.push_back(count);
          enums.push_back(  (TString)((wpVec.at(w))+"_"+(itr->first)  +"_"+(cutVec.at(c))+"_"+where[p]+"_ScEt") );
          denoms.push_back( (TString)((wpVec.at(w))+"_"+(itr->second) +"_"+(cutVec.at(c))+"_"+where[p]+"_ScEt") );
          count++;
          counts.push_back(count);
          enums.push_back(  (TString)((wpVec.at(w))+"_"+(itr->first)+"_"+(cutVec.at(c))+"_"+where[p]+"_Eta") );
          denoms.push_back( (TString)((wpVec.at(w))+"_"+(itr->second) +"_"+(cutVec.at(c))+"_"+where[p]+"_Eta") );
          count++;
          counts.push_back(count);
          enums.push_back(  (TString)((wpVec.at(w))+"_"+(itr->first)+"_"+(cutVec.at(c))+"_"+where[p]+"_Phi") );
          denoms.push_back( (TString)((wpVec.at(w))+"_"+(itr->second) +"_"+(cutVec.at(c))+"_"+where[p]+"_Phi") ); 
//           count++;
//           counts.push_back(count);
//           enums.push_back(  (TString)((wpVec.at(w))+"_"+(itr->first)+"_"+(cutVec.at(c))+"_"+where[p]+"_Pt") );
//           denoms.push_back( (TString)((wpVec.at(w))+"_"+(itr->second) +"_"+(cutVec.at(c))+"_"+where[p]+"_Pt") ); 
        }
      }
    }
  }
  
  for ( unsigned int i = 0; i < enums.size(); ++i )
  {
    TCanvas *canvas = makeCan( denoms.at(i), enums.at(i), counts.at(i) );
    oFile->cd();
    canvas->Write();    
    delete canvas;
  }

  oFile->Write();
  oFile->Close();
}




void setTDRStyle()
{
    TStyle *tdrStyle = new TStyle("tdrStyle","Style for P-TDR");

    // For the canvas:
    tdrStyle->SetCanvasBorderMode(0);
    tdrStyle->SetCanvasColor(kWhite);
    tdrStyle->SetCanvasDefH(800); //Height of canvas
    tdrStyle->SetCanvasDefW(800); //Width of canvas
    tdrStyle->SetCanvasDefX(0);   //POsition on screen
    tdrStyle->SetCanvasDefY(0);

    // For the Pad:
    tdrStyle->SetPadBorderMode(0);
    // tdrStyle->SetPadBorderSize(Width_t size = 1);
    tdrStyle->SetPadColor(kWhite);
    tdrStyle->SetPadGridX(false);
    tdrStyle->SetPadGridY(false);
    tdrStyle->SetGridColor(0);
    tdrStyle->SetGridStyle(3);
    tdrStyle->SetGridWidth(1);

    // For the frame:
    tdrStyle->SetFrameBorderMode(0);
    tdrStyle->SetFrameBorderSize(1);
    tdrStyle->SetFrameFillColor(0);
    tdrStyle->SetFrameFillStyle(0);
    tdrStyle->SetFrameLineColor(1);
    tdrStyle->SetFrameLineStyle(1);
    tdrStyle->SetFrameLineWidth(1);

    // For the histo:
    // tdrStyle->SetHistFillColor(1);
    // tdrStyle->SetHistFillStyle(0);
    tdrStyle->SetHistLineColor(1);
    tdrStyle->SetHistLineStyle(0);
    tdrStyle->SetHistLineWidth(1);
    // tdrStyle->SetLegoInnerR(Float_t rad = 0.5);
    // tdrStyle->SetNumberContours(Int_t number = 20);

    tdrStyle->SetEndErrorSize(2);
    //tdrStyle->SetErrorMarker(20);
    tdrStyle->SetErrorX(0.);

    tdrStyle->SetMarkerStyle(20);

    //For the fit/function:
    tdrStyle->SetOptFit(1);
    tdrStyle->SetFitFormat("5.4g");
    tdrStyle->SetFuncColor(2);
    tdrStyle->SetFuncStyle(1);
    tdrStyle->SetFuncWidth(1);

    //For the date:
    tdrStyle->SetOptDate(0);
    // tdrStyle->SetDateX(Float_t x = 0.01);
    // tdrStyle->SetDateY(Float_t y = 0.01);

    // For the statistics box:
    tdrStyle->SetOptFile(0);
    tdrStyle->SetOptStat(0); // To display the mean and RMS:   SetOptStat("mr");
    tdrStyle->SetStatColor(kWhite);
    tdrStyle->SetStatFont(42);
    tdrStyle->SetStatFontSize(0.025);
    tdrStyle->SetStatTextColor(1);
    tdrStyle->SetStatFormat("6.4g");
    tdrStyle->SetStatBorderSize(1);
    tdrStyle->SetStatH(0.1);
    tdrStyle->SetStatW(0.15);
    //   tdrStyle->SetOptStat(110);
    // tdrStyle->SetStatStyle(Style_t style = 1001);
    // tdrStyle->SetStatX(Float_t x = 0);
    // tdrStyle->SetStatY(Float_t y = 0);

    // Margins:
    tdrStyle->SetPadTopMargin(0.05);
    tdrStyle->SetPadBottomMargin(0.13);
    tdrStyle->SetPadLeftMargin(0.16);
    tdrStyle->SetPadRightMargin(0.02);

    // For the Global title:

    tdrStyle->SetOptTitle(0);
    tdrStyle->SetTitleFont(42);
    tdrStyle->SetTitleColor(1);
    tdrStyle->SetTitleTextColor(1);
    tdrStyle->SetTitleFillColor(10);
    tdrStyle->SetTitleFontSize(0.05);
    // tdrStyle->SetTitleH(0); // Set the height of the title box
    // tdrStyle->SetTitleW(0); // Set the width of the title box
    // tdrStyle->SetTitleX(0); // Set the position of the title box
    // tdrStyle->SetTitleY(0.985); // Set the position of the title box
    // tdrStyle->SetTitleStyle(Style_t style = 1001);
    // tdrStyle->SetTitleBorderSize(2);

    // For the axis titles:

    tdrStyle->SetTitleColor(1, "XYZ");
    tdrStyle->SetTitleFont(42, "XYZ");
    tdrStyle->SetTitleSize(0.06, "XYZ");
    // tdrStyle->SetTitleXSize(Float_t size = 0.02); // Another way to set the size?
    // tdrStyle->SetTitleYSize(Float_t size = 0.02);
    tdrStyle->SetTitleXOffset(0.9);
    tdrStyle->SetTitleYOffset(1.25);
    // tdrStyle->SetTitleOffset(1.1, "Y"); // Another way to set the Offset

    // For the axis labels:

    tdrStyle->SetLabelColor(1, "XYZ");
    tdrStyle->SetLabelFont(42, "XYZ");
    tdrStyle->SetLabelOffset(0.007, "XYZ");
    tdrStyle->SetLabelSize(0.05, "XYZ");

    // For the axis:

    tdrStyle->SetAxisColor(1, "XYZ");
    tdrStyle->SetStripDecimals(kTRUE);
    tdrStyle->SetTickLength(0.03, "XYZ");
    tdrStyle->SetNdivisions(510, "XYZ");
    tdrStyle->SetPadTickX(1);  // To get tick marks on the opposite side of the frame
    tdrStyle->SetPadTickY(1);

    // Change for log plots:
    tdrStyle->SetOptLogx(0);
    tdrStyle->SetOptLogy(0);
    tdrStyle->SetOptLogz(0);

    // Postscript options:
    tdrStyle->SetPaperSize(20.,20.);
    // tdrStyle->SetLineScalePS(Float_t scale = 3);
    // tdrStyle->SetLineStyleString(Int_t i, const char* text);
    // tdrStyle->SetHeaderPS(const char* header);
    // tdrStyle->SetTitlePS(const char* pstitle);

    // tdrStyle->SetBarOffset(Float_t baroff = 0.5);
    // tdrStyle->SetBarWidth(Float_t barwidth = 0.5);
    // tdrStyle->SetPaintTextFormat(const char* format = "g");
    // tdrStyle->SetPalette(Int_t ncolors = 0, Int_t* colors = 0);
    // tdrStyle->SetTimeOffset(Double_t toffset);
    // tdrStyle->SetHistMinimumZero(kTRUE);
    
    tdrStyle->SetPalette(1,0); 
    
    tdrStyle->cd();
}


