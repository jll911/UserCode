#!/usr/bin/env python
#
# Simple example script that uses RooFit to
# determine number of signal and background events in a data histogram.
# Requires 3 histograms (TH1F): data, mc_signal, mc_background.
# It basically determines the scales needed on mc_signal and mc_background
# to fit the data histogram.
#
# Fitting is done in get_num_sig_bkg().
# Setup of the histograms is done in main().
# Run this script by typing at the prompt:
# $ ./find_num_sig.py
#
# Michael Anderson, Jan 2011

from math import sqrt
import sys  # For exiting program
try:
    # ROOT stuff
    from ROOT import TAxis, TFile, TTree, TH1F, TH2F, TH3F, gROOT, gDirectory
    # RooFit stuff
    from ROOT import RooAddPdf, RooArgList, RooArgSet, RooDataHist, RooFit, RooHistPdf, RooRealVar, RooMCStudy, RooPlot
    # rootplot stuff
    from rootplot.rootmath import Target, newadd
except Exception, e:
    print e
    print ("Use a python that has PyROOT installed.")
    sys.exit(0)

def get_num_sig_bkg(hist_DataTemplate,
                    hist_SignalTemplate,
                    hist_BackgdTemplate,
                    fit_range_min,
                    fit_range_max):
    '''Given 3 input histograms (TH1F), and a fit range, this function finds
    the amount of signal and background that sum up to the data histogram.
    It does histogram fits.'''
    # Find range of data template
    data_min = hist_DataTemplate.GetXaxis().GetXmin()
    data_max = hist_DataTemplate.GetXaxis().GetXmax()
    
    # Create basic variables
    x = RooRealVar("x","x",data_min,data_max)
    x.setBins(hist_DataTemplate.GetXaxis().GetNbins())  # Binned x values
    nsig = RooRealVar("nsig","number of signal events"    , 0, hist_DataTemplate.Integral())
    nbkg = RooRealVar("nbkg","number of background events", 0, hist_DataTemplate.Integral())
    
    # Create RooDataHists from input TH1Fs
    dh = RooDataHist("dh","dh",RooArgList(x),hist_DataTemplate)
    ds = RooDataHist("ds","ds",RooArgList(x),hist_SignalTemplate)
    db = RooDataHist("db","db",RooArgList(x),hist_BackgdTemplate)
    
    # Create Probability Distribution Functions from Monte Carlo
    sigPDF = RooHistPdf("sigPDF", "sigPDF", RooArgSet(x), ds)
    bkgPDF = RooHistPdf("bkgPDF", "bkgPDF", RooArgSet(x), db)
    
    model = RooAddPdf("model","(g1+g2)+a",RooArgList(bkgPDF,sigPDF),RooArgList(nbkg,nsig))
    
    # Find the edges of the bins that contain the fit range min/max
    data_min = hist_DataTemplate.GetXaxis().GetBinLowEdge(hist_DataTemplate.GetXaxis().FindFixBin(fit_range_min))
    data_max = hist_DataTemplate.GetXaxis().GetBinUpEdge(hist_DataTemplate.GetXaxis().FindFixBin(fit_range_max))
    
    r = model.fitTo(dh,RooFit.Save(),RooFit.Minos(0),RooFit.PrintEvalErrors(0),
                    RooFit.Extended(),RooFit.Range(data_min,data_max))
    r.Print("v")

    #print nsig.getVal(), nsig.getError(), nbkg.getVal(), nbkg.getError()
    #  Create pull distribution
    #mcstudy = RooMCStudy(model, RooArgSet(x), RooFit.Binned(1), RooFit.Silence(),
    #                     RooFit.Extended(),
    #                     RooFit.FitOptions(RooFit.Save(1),
    #                                       RooFit.PrintEvalErrors(0),
    #                                       RooFit.Minos(0))
    #                    )
    #mcstudy.generateAndFit(500)                          # Generate and fit toy MC
    #pull_dist = mcstudy.plotPull(nsig, -3.0, 3.0, 30, 1)  # make pull distribution
    pull_dist = None
    return [nsig.getVal(), nsig.getError(), nbkg.getVal(), nbkg.getError(), pull_dist]

def dir_GetEtaBin(dir):
    '''Assumes dir is of the format "x_x_x_etabin_ptbin"'''
    return dir.split('_')[3]

def dir_GetPtBin(dir):
    '''Assumes dir is of the format "x_x_x_etabin_ptbin"'''
    return int(dir.split('_')[4])

def dir_GetJetBin(dir):
    '''Assumes dir is of the format "x_x_x_etabin_jetbin"'''
    return dir.split('_')[4]
    
def ErrorPropagate(A,a,B,b):
    '''A (B) are number of signal (background),
    and a (b) are the error on number of signal (background).
    This function returns error on function F = A/(A+B)'''
    F = A/(A+B)
    term0 = a/A
    term1 = sqrt(a**2+b**2)/(A+B)
    term2 = -2*(a*sqrt(a**2+b**2)/(A*(A+B)))*(-1.)  # A and B are correlated
    return sqrt( (F**2) * (term0**2 + term1**2 + term2**2))


def th1f_SetBinContent(hist, content, errors=[], start_bin=0):
    for bin in range(start_bin, hist.GetNbinsX()+1):
        hist.SetBinContent(bin, content[bin-start_bin])
        if errors:
            hist.SetBinError(bin, errors[bin-start_bin])
        else:
            hist.SetBinError(bin, 0)

def create_purity_vs_photonPt():
    print "Making purity vs photon pt plots..."
    # Make purity plots (Data fraction signal vs photon pT)
    temp = {}
    temp_err = {}
    purity_by_etabin = {}
    purity_err_by_etabin = {}
    for dir in data_fraction_signal:
        temp.setdefault(dir_GetEtaBin(dir), {})[dir_GetPtBin(dir)] = data_fraction_signal[dir]
        temp_err.setdefault(dir_GetEtaBin(dir), {})[dir_GetPtBin(dir)] = data_fraction_signal_error[dir]
    for etabin,values in temp.items():
        purity_by_etabin[etabin]=[x[1] for x in sorted(values.items())]
    for etabin,values in temp_err.items():
        purity_err_by_etabin[etabin]=[x[1] for x in sorted(values.items())]
    output_example = TFile("purity_data.root","recreate")
    output_example.cd()
    for etabin in purity_by_etabin:
        temp_hist = file_data.Get('g_pass_Liso_%s_allPt/h_photon_pt_bin' % etabin)
        current_hist = temp_hist.Clone("purity_%s" % etabin)
        current_hist.GetYaxis().SetTitle("purity")
        th1f_SetBinContent(current_hist, purity_by_etabin[etabin], purity_err_by_etabin[etabin], 9)
        current_hist.Write()
    output_example.Close()
    print "Created %s" % output_example.GetName()

def main():
    print "Opening input root files"
    
    # Open input files
    file_data   = TFile("hist_files/Hists_Data.root", "read")
    #file_data   = TFile("hist_files/Hists_Data_JESup.root", "read")
    #file_data   = TFile("hist_files/Hists_Data_JESdown.root", "read")
    file_signal = TFile("hist_files/Hists_PhotonJet_Madgraph.root", "read")
    #file_signal = TFile("hist_files/Hists_PhotonJet.root", "read")
    file_backgd = TFile("hist_files/Hists_DataTrkFlip.root", "read")
    #file_backgd = TFile("hist_files/Hists_DataTrkFlip_JESup.root", "read")
    #file_backgd = TFile("hist_files/Hists_DataTrkFlip_JESdown.root", "read")
    #file_backgd = TFile("hist_files/Hists_DataTrkFlip_SidebandUP.root", "read")
    #file_backgd = TFile("hist_files/Hists_DataTrkFlip_SidebandDOWN.root", "read")
    
    
    # After fitting to determine number of signal background
    # these files will store the scaled histograms
    tfile_data_scaled_summed = TFile("hist_files_postPurity/Hists_Data_scaled_summed.root", "recreate")
    file_mcsignal_output = TFile("hist_files_postPurity/Hists_PhotonJet_binned_scaled.root","recreate")
    #file_mcbkgd_output   = TFile("Hists_QCD_binned_scaled.root","recreate")
    file_mcbkgd_output   = TFile("hist_files_postPurity/Hists_DataTrkFlip_binned_scaled.root", "recreate")
    file_pullDist_output = TFile("hist_files_postPurity/Hists_pullDistributions.root", "recreate")
    
    # Create list of directories to do fitting in
    dirs = []
    dirs = ["g_pass_Liso_barEta_0jets30","g_pass_Liso_barEta_1jets30",
        "g_pass_Liso_barEta_2jets30","g_pass_Liso_barEta_3jets30",
        "g_pass_Liso_endEta_0jets30","g_pass_Liso_endEta_1jets30",
        "g_pass_Liso_endEta_2jets30","g_pass_Liso_endEta_3jets30"
        ]
    
    data_fraction_signal = {} # Dictionary to store fraction of signal. key = dir name
    data_fraction_signal_error = {} # Ditionary to store error on fraction of signal
    mc_signal_scale = {}      # Dictionary to store MC Signal scale factors
    mc_backgd_scale = {}      # Dictionary to store MC Background scale factors
    
    # Do the fits in each directory
    for dir in dirs:
        # Histogram to fit, and fit range depend on eta bin:
        if 'endEta' in dir:
            name_templateHist = "h_photon_sieie_end"
            fit_min = 0.02
            fit_max = 0.05
            #if dir_GetPtBin(dir)>5: fit_max = 0.04
        else:
            name_templateHist = "h_photon_sieie"
            fit_min = 0.006
            fit_max = 0.02
            #if dir_GetPtBin(dir)>10: fit_max = 0.016
        
        # Get input TH1F that is used to determine % signal and
        # background in data
        hist_data  = file_data.Get('%s/%s' % (dir,name_templateHist))
        hist_McSig = file_signal.Get('%s/%s' % (dir,name_templateHist))
        hist_McBkg = file_backgd.Get('%s/%s' % (dir,name_templateHist))
        
        if not (hist_data and hist_McSig and hist_McBkg): continue
        # Set range the fit is done over (depends on eta bin)
        # Determine amount of signal/background in data
        if (hist_McBkg.GetEntries() > 10):
            print "calculating purity"
            num_sig, num_sig_error, num_bkg, num_bkg_error, pull_dist = \
                    get_num_sig_bkg(hist_data, hist_McSig, hist_McBkg,fit_min,fit_max)
        else:
            print "too few entries in one bin (<10) to do fit reasonably. Exiting..."
            sys.exit()
        
        if (num_sig>0.0 or num_bkg>0.0):
            data_fraction_signal[dir] = num_sig / (num_sig + num_bkg) # Fraction signal in data
            data_fraction_signal_error[dir] = ErrorPropagate(num_sig, num_sig_error, num_bkg, num_bkg_error)
        else:
            data_fraction_signal[dir]       = 0.0
            data_fraction_signal_error[dir] = 0.0
        if (hist_McSig.Integral()>0 and hist_McBkg.Integral()>0):
            mc_signal_scale[dir] = num_sig / hist_McSig.Integral() # MC Signal scaled to events in data
            mc_backgd_scale[dir] = num_bkg / hist_McBkg.Integral() # MC Backgd scaled to events in data
        else:
            mc_signal_scale[dir] = 0.0
            mc_backgd_scale[dir] = 0.0
            
        print "%s" % dir
        print "\t%f\t%f\t%f" % (num_sig,num_bkg,hist_data.Integral())
        print "\t%f\t%f" % (num_sig_error, num_bkg_error)
        # Write the pull distribution to a root file
        #file_pullDist_output.cd()
        #pull_dist.SetName(dir)
        #pull_dist.Write()
        print "--------------------------------------------------------------"
    
    print "\nSummary of dir, fraction signal, and error on fraction signal"
    for dir in sorted(data_fraction_signal):
        print "%s : %0.2f +/- %0.2f" % (dir, data_fraction_signal[dir], data_fraction_signal_error[dir])
    #sys.exit()
    
    # Fraction of signal & background in each dir in data file has
    # been determined, now subtract background from data
    target_list = {}
    print "\nData\n----------"
    print "Dir\t\t\tScale\tError"
    for dir in data_fraction_signal:
        print "%s\t%0.3f\t%0.3f" % (dir, data_fraction_signal[dir], data_fraction_signal_error[dir])
        data_target = Target(filename=file_data.GetName(),
                             path=dir,
                             scale=1.0)
        bkgd_target = Target(filename=file_backgd.GetName(),
                             path=dir,
                             scale=-mc_backgd_scale[dir])
        target_list.setdefault(dir_GetEtaBin(dir), []).append(data_target)
        target_list.setdefault('allEta', []).append(data_target)
        target_list.setdefault(dir_GetEtaBin(dir), []).append(bkgd_target)
        target_list.setdefault('allEta', []).append(bkgd_target)
        #current_target = Target(filename=file_data.GetName(),
        #                        path=dir,
        #                        scale=data_fraction_signal[dir],
        #                        scale_error=data_fraction_signal_error[dir])
        #target_list.setdefault(dir_GetEtaBin(dir), []).append(current_target)
        #target_list.setdefault('allEta', []).append(current_target)
    # Sum the different pT bins
    for list in target_list:
        newadd(tfile_data_scaled_summed, target_list[list], dest_path="g_pass_Liso_"+str(list)+"_allPt")
        print "Created %s" % ("g_pass_Liso_"+str(list)+"_allPt")
    print "Created %s" % tfile_data_scaled_summed.GetName()
    
    # Create new root file with MC Signal scaled to amount in data
    print "\nMC Signal\n----------"
    print "Dir\t\tScale"
    target_list = {}
    for dir in mc_signal_scale:
        print "%s\t%f" % (dir, mc_signal_scale[dir])
        current_target = Target(filename=file_signal.GetName(), path=dir, scale=mc_signal_scale[dir])
        target_list.setdefault(dir_GetEtaBin(dir), []).append(current_target)
        newadd(file_mcsignal_output, [current_target], dest_path=dir) # Scale this dir
    # Sum the different pT bins
    print "summing signal across pt bins..."
    for list in target_list:
        newadd(file_mcsignal_output, target_list[list], dest_path="g_pass_Liso_"+str(list)+"_allPt")
        print "Created %s" % ("g_pass_Liso_"+str(list)+"_allPt")
    print "Created %s" % file_mcsignal_output.GetName()
    
    # Create new root file with MC Background scaled to amount in data
    print "\nMC Background\n----------"
    print "Dir\t\tScale"
    target_list = {}
    for dir in mc_backgd_scale:
        print "%s\t%f" % (dir, mc_backgd_scale[dir])
        current_target = Target(filename=file_backgd.GetName(), path=dir, scale=mc_backgd_scale[dir])
        target_list.setdefault(dir_GetEtaBin(dir), []).append(current_target)
        newadd(file_mcbkgd_output, [current_target], dest_path=dir)
    # Sum the different pT bins
    for list in target_list:
        newadd(file_mcbkgd_output, target_list[list], dest_path="g_pass_Liso_"+str(list)+"_allPt")
        print "Created %s" % ("g_pass_Liso_"+str(list)+"_allPt")
    print "Created %s" % file_mcbkgd_output.GetName()
    
if __name__ == "__main__":
    main()
