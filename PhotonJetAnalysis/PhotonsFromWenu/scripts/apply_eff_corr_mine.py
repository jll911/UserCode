#!/usr/bin/env python

# The purpose of this code is to scale histograms from one or more
# files by another histogram (like to make an efficiecy correction)

from ROOT import TFile, TH1F, TH2F, TGraphAsymmErrors, gROOT, gDirectory
gROOT.SetBatch()


if __name__ == '__main__':
    #filename_eff = "hist_files/Hists_PhotonJet_efficiency.root"
    filename_eff = "hist_files/Hists_PhotonJet_Madgraph_efficiency.root"
    
    tfile_eff    = TFile(filename_eff,"read")
    print "Opened %s" % filename_eff
    tfile_output = TFile(filename_eff[:-5]+"_njet_eff.root","recreate")
    
    h_pairs = {
        # Name        :  (Numerator, Denominator)
        "barEta_pt20" : ("g_pass_Liso_barEta_allPt/h_pfjet_num_pt20","g_barEta_allPt/h_pfjet_num_pt20"),
        "barEta_pt30" : ("g_pass_Liso_barEta_allPt/h_pfjet_num_pt30","g_barEta_allPt/h_pfjet_num_pt30"),
        "endEta_pt20" : ("g_pass_Liso_endEta_allPt/h_pfjet_num_pt20","g_endEta_allPt/h_pfjet_num_pt20"),
        "endEta_pt30" : ("g_pass_Liso_endEta_allPt/h_pfjet_num_pt30","g_endEta_allPt/h_pfjet_num_pt30")}

    tfile_output.cd()
    h_eff_v_numjet = {}
    for name in sorted(h_pairs):
        numer = tfile_eff.Get(h_pairs[name][0])
        denom = tfile_eff.Get(h_pairs[name][1])
        temp = numer.Clone()
        temp.SetName(name)
        temp.GetYaxis().SetTitle("efficiency")
        temp.Divide(denom)
        h_eff_v_numjet[name] = temp
        temp.Write()
        print "%s = %s  div  %s" % (name, h_pairs[name][0], h_pairs[name][1])
    print "Created %s" % tfile_output.GetName()
    
    #
    # Divide Data hists by Efficiency hists to correct for efficiency
    #
    filename_data = "hist_files_postPurity/Hists_Data_scaled_summed.root"
    
    tfile_data = TFile(filename_data,"read")
    tfile_data_effCor = TFile(filename_data[:-5]+"_effCorr.root","recreate")
    
    list_DivPairs = [
        ("g_pass_Liso_barEta_allPt/h_pfjet_num_pt20","barEta_pt20"),
        ("g_pass_Liso_barEta_allPt/h_pfjet_num_pt30","barEta_pt30"),
        ("g_pass_Liso_endEta_allPt/h_pfjet_num_pt20","endEta_pt20"),
        ("g_pass_Liso_endEta_allPt/h_pfjet_num_pt30","endEta_pt30"),
        ]
    tfile_data_effCor.cd()
    for numer_name, denom_name in list_DivPairs:
        print denom_name
        dir = numer_name.split('/')[0]
        tfile_data_effCor.mkdir(dir)
        tfile_data_effCor.cd(dir)
        temp = tfile_data.Get(numer_name).Clone()
        temp.Divide(h_eff_v_numjet[denom_name])
        temp.Write()
        
    
    #
    # Combine barrel and endcap
    #
    
    tfile_data_effCor.cd()
    tfile_data_effCor.mkdir("g_pass_Liso_allEta_allPt")
    tfile_data_effCor.cd("g_pass_Liso_allEta_allPt")
    temp0 = tfile_data_effCor.Get("g_pass_Liso_barEta_allPt/h_pfjet_num_pt20").Clone()
    temp1 = tfile_data_effCor.Get("g_pass_Liso_endEta_allPt/h_pfjet_num_pt20").Clone()
    temp0.Add(temp1)
    temp0.Write()
    temp0 = tfile_data_effCor.Get("g_pass_Liso_barEta_allPt/h_pfjet_num_pt30").Clone()
    temp1 = tfile_data_effCor.Get("g_pass_Liso_endEta_allPt/h_pfjet_num_pt30").Clone()
    temp0.Add(temp1)
    temp0.Write()
    print "Created %s" % tfile_data_effCor.GetName()
    
    
    tfile_output.Close()
    tfile_data_effCor.Close()
