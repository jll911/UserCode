#ifndef MYFUNCTS_H_
#define MYFUNCTS_H_

#include <cstdlib>
#include <cmath>
#include <sstream>

#include "TStyle.h"
#include "TH1D.h"
#include "TH1.h"

struct hBins
{
    std::string yAxis;
    std::string xAxis;
    int nBins;
    double low;
    double high;
};


/// calculating dPhi
/// from DataFormats/Math/interface/deltaPhi.h
inline double deltaPhi(double phi1, double phi2) 
{ 
    double result = phi1 - phi2;
    while (result > M_PI) result -= 2*M_PI;
    while (result <= -M_PI) result += 2*M_PI;
    return result;
}

/// calculating deltaR
/// from DataFormats/Math/interface/deltaR.h
inline double deltaR(double eta1, double phi1, double eta2, double phi2)
{
    double deta = eta1 - eta2;
    double dphi = deltaPhi(phi1, phi2);
    return sqrt(deta*deta + dphi*dphi);
}


/// convert int to string
inline std::string itos(int i) 
{
    std::stringstream s;
    s << i;
    return s.str();
}


/// sets integer bin labels to a given plot
inline void setIntegerBinLabels( TH1* h )
{
    h->GetXaxis()->SetNdivisions( h->GetNbinsX() );
    h->GetXaxis()->CenterLabels(true);
}




/// sets integer bin labels to a given 2d plot
inline void setIntegerBinLabels2D( TH1* h )
{
    h->GetXaxis()->SetNdivisions( h->GetNbinsX() );
    h->GetYaxis()->SetNdivisions( h->GetNbinsY() );
    h->GetXaxis()->CenterLabels(true);
    h->GetYaxis()->CenterLabels(true);
}


/// returns true if electron in tracker coverage
inline bool electronInEta(Float_t eEta)
{
  //    return ( fabs(eEta) < 2.5 && ( fabs(eEta) < 1.4442 || fabs(eEta) > 1.56 ) );
  return ( fabs(eEta) < 2.5 && ( fabs(eEta) < 1.4442 || fabs(eEta) > 1.566 ) ); // from Jeremy
}


/// used to format single-digit integers
inline std::string corrStr(int i){ if (i > 9) return itos(i); else return "0" + itos(i); }

void setTDRStyle(); 

TH1D * myTH1D(std::string name,
                        hBins bins,
                        int lColor = kBlack,
                        int lWidth = 2,
                        int lStyle = 1,
                        int mStyle = 1 );

TH1D * myTH1D(std::string name, 
                        hBins bins,
                        const double *xbins,
                        int lColor = kBlack,
                        int lWidth = 2,
                        int lStyle = 1,
                        int mStyle = 1 );
#endif
