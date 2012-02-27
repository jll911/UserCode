from rootplot.tree2hists import RootTree, Plot
from math import pi
from array import array      # to allow making Float_t arrays for ROOT hists
from ROOT import TH1F, TH2F  # import other kinds of hists as neeeded

# Amount of data in pb-1
inv_lumi = 36 
#inv_lumi = 33.7 # data in all the data hlt paths
#inv_lumi = 18.1 # data in HLT_Photon70_Cleaned_L1R_v1 in Photon2010B
#inv_lumi = 8.6 # data in HLT_Photon30  2010B and A
#inv_lumi = 5.76 # data in Photon2010B (eff HLT_Photon30)

file_list = {}

file_list['gjet_mad'] = [
    RootTree("photonsfromwenu/tree", fileName="original_files/GJets_TuneD6T_HT-20To40_7TeV-madgraph_Fall10-START38_V12-v1_AODSIM_photonjet.root", scale=31910*inv_lumi/(8175240.*43/43), cuts=""),
    RootTree("photonsfromwenu/tree", fileName="original_files/GJets_TuneD6T_HT-40To100_7TeV-madgraph_Fall10-START38_V12-v1_AODSIM_photonjet.root", scale=23620*inv_lumi/(2217101.*13/13), cuts=""),
    RootTree("photonsfromwenu/tree", fileName="original_files/GJets_TuneD6T_HT-100To200_7TeV-madgraph_Fall10-START38_V12-v1_AODSIM_photonjet.root", scale=3476*inv_lumi/(1065691.*7/7), cuts=""),
    RootTree("photonsfromwenu/tree", fileName="original_files/GJets_TuneD6T_HT-200_7TeV-madgraph_Fall10-START38_V12-v1_AODSIM_photonjet.root", scale=485*inv_lumi/(1142171.*7/10), cuts=""),
    ]
file_list['qcd_mad'] = [
    RootTree("photonsfromwenu/tree", fileName="original_files/QCD_TuneD6T_HT-50To100_7TeV-madgraph_Fall10-START38_V12-v1_AODSIM_photonjet.root", scale=30e6*inv_lumi/(218573.*6/6), cuts=""),
    RootTree("photonsfromwenu/tree", fileName="original_files/QCD_TuneD6T_HT-100To250_7TeV-madgraph_Fall10-START38_V12-v1_AODSIM_photonjet.root", scale=7e6*inv_lumi/(10198724.*55/56), cuts=""),
    RootTree("photonsfromwenu/tree", fileName="original_files/QCD_TuneD6T_HT-250To500_7TeV-madgraph_Fall10-START38_V12-v1_AODSIM_photonjet.root", scale=171e3*inv_lumi/(4590620.*26/26), cuts=""),
    RootTree("photonsfromwenu/tree", fileName="original_files/QCD_TuneD6T_HT-500To1000_7TeV-madgraph_Fall10-START38_V12-v1_AODSIM_photonjet.root", scale=5.2e3*inv_lumi/(7380455.*41/41), cuts=""),
    RootTree("photonsfromwenu/tree", fileName="original_files/QCD_TuneD6T_HT-1000_7TeV-madgraph_Fall10-START38_V12-v1_AODSIM_photonjet.root", scale=83*1.0*inv_lumi/(1705728.*14/15), cuts=""),
    ]
file_list['gjet'] = [
#    RootTree("photonsfromwenu/tree", fileName="original_files/G_Pt_15to30_TuneZ2_7TeV_pythia6_Fall10-START38_V12-v1_AODSIM_photonjet.root", scale=1.717e5*1.0*inv_lumi/(1025840.*7/7), cuts=""),
#    RootTree("photonsfromwenu/tree", fileName="original_files/G_Pt_30to50_TuneZ2_7TeV_pythia6_Fall10-START38_V12-v1_AODSIM_photonjet.root", scale=1.669e4*1.0*inv_lumi/(1025480.*6/6), cuts=""),
    RootTree("photonsfromwenu/tree", fileName="original_files/G_Pt_50to80_TuneZ2_7TeV_pythia6_Fall10-START38_V12-v1_AODSIM_photonjet.root", scale=2.722e3*1.0*inv_lumi/(1024608.*5/6), cuts=""),
    RootTree("photonsfromwenu/tree", fileName="original_files/G_Pt_80to120_TuneZ2_7TeV_pythia6_Fall10-START38_V12-v1_AODSIM_photonjet.root", scale=4.472e2*1.0*inv_lumi/(1023361.*6/6), cuts=""),
    RootTree("photonsfromwenu/tree", fileName="original_files/G_Pt_120to170_TuneZ2_7TeV_pythia6_Fall10-START38_V12-v1_AODSIM_photonjet.root", scale=8.417e1*1.0*inv_lumi/(1023361.*6/6), cuts=""),
    RootTree("photonsfromwenu/tree", fileName="original_files/G_Pt_170to300_TuneZ2_7TeV_pythia6_Fall10-START38_V12-v1_AODSIM_photonjet.root", scale=2.264e1*1.0*inv_lumi/(1100000.*6/6), cuts=""),
    #RootTree("photonsfromwenu/tree", fileName="original_files/G_Pt_300to470_TuneZ2_7TeV_pythia6_Fall10-START38_V12-v1_AODSIM_photonjet.root", scale=1.493*1.0*inv_lumi/(1098904.*6/6), cuts="")
    ]
file_list['qcdembc'] = [
#    RootTree("photonsfromwenu/tree", fileName="original_files/QCD_Pt-20to30_EMEnriched_TuneZ2_7TeV-pythia6_Fall10-START38_V12-v1_AODSIM_photonjet.root", scale=288700000*0.00989*inv_lumi/(37169939.*191/191), cuts=""),
    RootTree("photonsfromwenu/tree", fileName="original_files/QCD_Pt-30to80_EMEnriched_TuneZ2_7TeV-pythia6_Fall10-START38_V12-v1_AODSIM_photonjet.root", scale=74540000*0.06118*inv_lumi/(71845473.*358/367), cuts=""),
    RootTree("photonsfromwenu/tree", fileName="original_files/QCD_Pt-80to170_EMEnriched_TuneZ2_7TeV-pythia6_Fall10-START38_V12-v1_AODSIM_photonjet.root", scale=1193000*0.158*inv_lumi/(8073559.*42/42), cuts=""),
    RootTree("photonsfromwenu/tree", fileName="original_files/QCD_Pt-20to30_BCtoE_TuneZ2_7TeV-pythia6_Fall10-START38_V12-v1_AODSIM_photonjet.root", scale=288700000*0.00060*inv_lumi/(2243439.*12/12), cuts=""),
    RootTree("photonsfromwenu/tree", fileName="original_files/QCD_Pt-30to80_BCtoE_TuneZ2_7TeV-pythia6_Fall10-START38_V12-v1_AODSIM_photonjet.root", scale=74540000*0.00241*inv_lumi/(1995502.*10/10), cuts=""),
    RootTree("photonsfromwenu/tree", fileName="original_files/QCD_Pt-80to170_BCtoE_TuneZ2_7TeV-pythia6_Fall10-START38_V12-v1_AODSIM_photonjet.root", scale=1193000*0.01087*inv_lumi/(1043390.*6/6), cuts=""),
    # Should one included higher pt qcd samples?
    RootTree("photonsfromwenu/tree", fileName="original_files/QCD_Pt_170to300_TuneZ2_7TeV_pythia6_Fall10-START38_V12-v1_GEN-SIM-RECO_photonjet.root", scale=2.426e4*1.0*inv_lumi/(3220080.*18/18), cuts=""),
    RootTree("photonsfromwenu/tree", fileName="original_files/QCD_Pt_300to470_TuneZ2_7TeV_pythia6_Fall10-START38_V12-v1_GEN-SIM-RECO_photonjet.root", scale=1.168e3*1.0*inv_lumi/(3171240.*16/17), cuts="")
    ]
file_list['qcd'] = [
    RootTree("photonsfromwenu/tree", fileName="original_files/QCD_Pt_15to30_TuneZ2_7TeV_pythia6_Fall10-START38_V12-v1_GEN-SIM-RECO_photonjet.root", scale=8.159e8*1.0*inv_lumi/(5454640.*25/25), cuts=""),
    RootTree("photonsfromwenu/tree", fileName="original_files/QCD_Pt_30to50_TuneZ2_7TeV_pythia6_Fall10-START38_V12-v1_GEN-SIM-RECO_photonjet.root", scale=5.312e7*1.0*inv_lumi/(3264660.*17/17), cuts=""),
    RootTree("photonsfromwenu/tree", fileName="original_files/QCD_Pt_50to80_TuneZ2_7TeV_pythia6_Fall10-START38_V12-v1_GEN-SIM-RECO_photonjet.root", scale=6.359e6*1.0*inv_lumi/(3191546.*17/17), cuts=""),
    RootTree("photonsfromwenu/tree", fileName="original_files/QCD_Pt_80to120_TuneZ2_7TeV_pythia6_Fall10-START38_V12-v1_GEN-SIM-RECO_photonjet.root", scale=7.843e5*1.0*inv_lumi/(3208299.*17/17), cuts=""),
    RootTree("photonsfromwenu/tree", fileName="original_files/QCD_Pt_120to170_TuneZ2_7TeV_pythia6_Fall10-START38_V12-v1_GEN-SIM-RECO_photonjet.root", scale=1.151e5*1.0*inv_lumi/(3045200.*18/18), cuts=""),
    RootTree("photonsfromwenu/tree", fileName="original_files/QCD_Pt_170to300_TuneZ2_7TeV_pythia6_Fall10-START38_V12-v1_GEN-SIM-RECO_photonjet.root", scale=2.426e4*1.0*inv_lumi/(3220080.*18/18), cuts=""),
    RootTree("photonsfromwenu/tree", fileName="original_files/QCD_Pt_300to470_TuneZ2_7TeV_pythia6_Fall10-START38_V12-v1_GEN-SIM-RECO_photonjet.root", scale=1.168e3*1.0*inv_lumi/(3171240.*16/17), cuts=""),
    RootTree("photonsfromwenu/tree", fileName="original_files/QCD_Pt_470to600_TuneZ2_7TeV_pythia6_Fall10-START38_V12-v1_GEN-SIM-RECO_photonjet.root", scale=7.022e1*1.0*inv_lumi/(2019732.*11/11), cuts="")
    ]
file_list['data'] = [
    RootTree("photonsfromwenu/tree", fileName="original_files/Data_138564_143962_HLT20.root", scale=1.0, cuts=""),
    RootTree("photonsfromwenu/tree", fileName="original_files/Data_144010_147116_HLT30.root", scale=1.0, cuts=""),
    RootTree("photonsfromwenu/tree", fileName="original_files/Data_147196_148058_HLT50.root", scale=1.0, cuts=""),
    RootTree("photonsfromwenu/tree", fileName="original_files/Data_148822_149294_HLT70.root", scale=1.0, cuts=""),
    #RootTree("photonsfromwenu/tree", fileName="original_files/Data_138564_144114_HLT20.root", scale=1.0, cuts=""),
    #RootTree("photonsfromwenu/tree", fileName="original_files/Data_146428_147116_HLT30.root", scale=1.0, cuts=""),
    #RootTree("photonsfromwenu/tree", fileName="original_files/Data_147196_148058_HLT50.root", scale=1.0, cuts=""),
    #RootTree("photonsfromwenu/tree", fileName="original_files/Data_148822_149294_HLT70.root", scale=1.0, cuts=""),
#    RootTree("photonsfromwenu/tree", fileName="original_files/EG_Run2010A-Dec22ReReco_v1_AOD_photonjet.root", scale=1.0, cuts=""),
#    RootTree("photonsfromwenu/tree", fileName="original_files/Photon_Run2010B-Dec22ReReco_v1_AOD_photonjet.root", scale=1.0, cuts=""),
    #RootTree("photonsfromwenu/tree", fileName="original_files/Photon_Run2010B-Dec22ReReco_v1_AOD_photonjet_HLT50.root", scale=1.0, cuts=""),
    #RootTree("photonsfromwenu/tree", fileName="original_files/Photon_Run2010B-Dec22ReReco_v1_AOD_photonjet_HLT70.root", scale=1.0, cuts="")
    ]
file_list['data50'] = [
#    RootTree("photonsfromwenu/tree", fileName="original_files/Photon_Run2010B-Dec22ReReco_v1_AOD_photonjet_HLT50.root", scale=1.0, cuts=""),
    ]
file_list['data70'] = [
#    RootTree("photonsfromwenu/tree", fileName="original_files/Photon_Run2010B-Dec22ReReco_v1_AOD_photonjet_HLT70.root", scale=1.0, cuts=""),
    ]



# Define histograms to plot
bins_et     = array("f", [30.0, 35.0, 40.0, 45.0, 50.0, 55.0, 60, 65, 70, 80, 100, 120, 200]) # example custom bins
bins_njet   = array("f", [-0.5, 0.5, 1.5, 2.5, 3.5, 4.5, 5.5, 6.5, 8.5])  # last bin is for 7+ jets

plot_list = {}

plot_list['gen_all'] = [
    #Plot("genjet_num"     , TH1F("h_genjet_num"      ,"Number of genJets (pt>30GeV);Num of Jets", 7, -0.5, 6.5)),
    Plot("genjet_num_pt20", TH1F("h_genjet_num_pt20" ,"Number of genJets (p_{T}>20GeV);Exclusive Jet Number;Number of Events", 7, -0.5, 6.5)),
    Plot("genjet_num_pt30", TH1F("h_genjet_num_pt30" ,"Number of genJets (p_{T}>30GeV);Exclusive Jet Number;Number of Events", 7, -0.5, 6.5)),
    #Plot("pfjet_num:genjet_num", TH2F("h_pfVgen_jet_num" ,"Number of Jets (pt>30GeV);Num of genJets; Num pfJets", 7, -0.5, 6.5,7, -0.5, 6.5)),
    Plot("genjet_num_pt20:pfjet_num_pt20", TH2F("h_genVpf20_jet_num" ,"Number of Jets (p_{T}>20GeV);Num of PFJets; Num of GenJets", 7, -0.5, 6.5,7, -0.5, 6.5)),
    Plot("genjet_num_pt30:pfjet_num_pt30", TH2F("h_genVpf30_jet_num" ,"Number of Jets (p_{T}>30GeV);Num of PFJets; Num of GenJets", 7, -0.5, 6.5,7, -0.5, 6.5)),
    ]

plot_list['photon_iso'] = [
    Plot("photon_hOverE"  ,TH1F("h_photon_HoverE"    ,"Lead Photon;Hadronic/EM"                       , 20, 0.0, 0.2)),
    Plot("photon_ecalIso" ,TH1F("h_photon_ecalIso04" ,"Lead Photon;#SigmaEcal E_{T} in 0.4 cone (GeV)", 20, -1.0, 19)),
    Plot("photon_hcalIso" ,TH1F("h_photon_hcalIso04" ,"Lead Photon;#SigmaHcal E_{T} in 0.4 cone (GeV)", 20, 0, 20)),
    Plot("photon_trackIso",TH1F("h_photon_trackIso04","Lead Photon;#Sigmatrack p_{T} in hollow 0.4 cone (GeV)", 20, 0, 20)),
    ]

plot_list['rec_all'] = [
    Plot("(photon_pt-pfjet_pt)/(photon_pt)", TH1F("h_dpt_photon_pfjet","(p_{T}(#gamma) - p_{T}(jet))/p_{T}(#gamma)  (#Delta#phi(#gamma,jet)>2.8);#Deltap_{T}/p_{T}(#gamma);Events / 0.1",20,-1.25,0.75), "pfjet_num<2&&delta_phi(photon_phi,pfjet_phi)>2.8"),
    #Plot("photon_pt" , TH1F("h_photon_pt_bin" ,"Lead Photon;p_{T}(#gamma) [GeV/c^{2}];Events / bin", len(bins_et)-1, bins_et)),
    Plot("photon_pt" , TH1F("h_photon_pt" ,"Lead Photon;p_{T}(#gamma) [GeV];Events / 5 GeV", 28, 60., 200.0)),
    Plot("photon_eta", TH1F("h_photon_eta","Lead Photon;#eta(#gamma);Events / 0.2"         , 25, -2.5, 2.5)),
    #    Plot("photon_eta", TH1F("h_photon_eta","Lead Photon;#eta(#gamma)"       , 15, -1.45, 1.45)),
    #    Plot("photon_eta", TH1F("h_photon_eta_end","Lead Photon;#eta(#gamma)"   , 30, -2.5, 2.5)),
    Plot("photon_phi", TH1F("h_photon_phi","Lead Photon;#phi(#gamma);Events / 0.314"   , 20, -pi, pi)),
    #    Plot("photon_phi", TH1F("h_photon_phi_plusE" ,"Lead Photon (positive #eta);#phi(#gamma)", 20, -pi, pi), "photon_eta>0.0"),
    #    Plot("photon_phi", TH1F("h_photon_phi_minusE","Lead Photon (negative #eta);#phi(#gamma)", 20, -pi, pi), "photon_eta<0.0"),
    #    Plot("photon_phi", TH1F("h_photon_phi_minusEfar" ,"Lead Photon (#eta<-2.0);#phi(#gamma)", 20, -pi, pi), "photon_eta<-2.0"),
    #    Plot("photon_phi", TH1F("h_photon_phi_munusEnear","Lead Photon (-2.0<|#eta|<-1.55);#phi(#gamma)", 20, -pi, pi), "photon_eta>-2.0&&photon_eta<-1.55"),
    #Plot("photon_eta:photon_phi",TH2F("h_photon_eta_phi", "Lead Photon;#phi;#eta", 10, -pi, pi, 7, -1.45, 1.45)),
    #Plot("photon_eta:photon_pt" ,TH2F("h_photon_eta_pt" , "Lead Photon;p_{T};#eta", 4, 30., 50.0, 7, -1.45, 1.45)),
    #Plot("photon_sieie"  , TH1F("h_photon_sieie"     ,"Lead Photon;#sigma_{i#etai#eta}(#gamma)",15, 0, 0.0225)),
    #Plot("photon_sieie"  , TH1F("h_photon_sieie_end" ,"Lead Photon;#sigma_{i#etai#eta}(#gamma)",18, 0, 0.054)),

    #Plot("jet_sumAllEt"  , TH1F("h_jet_sumAllEt","Sum of Jet Et (for jets #DeltaR>0.4 from e);#SigmaJet E_{T} (GeV)", 20, 0.0, 120)),
    #Plot("jet_pt"   , TH1F("h_jet_pt"     ,"Lead Jet;p_{T}(jet) (GeV)", 40, 0., 200.0)),
    #Plot("jet_eta"  , TH1F("h_jet_eta"    ,"Lead Jet;#eta(jet)"       , 20, -5.0, 5.0)),
    #Plot("jet_phi"  , TH1F("h_jet_phi"    ,"Lead Jet;#phi(jet)"       , 10, (-1.-1./4.)*pi, (1.+1./4.)*pi)),
    #Plot("jet2_pt"  , TH1F("h_jet2_pt"    ,"2nd Lead Jet;p_{T}(jet) (GeV)", 40, 0., 200.0), "jet2_pt>20"),
    #Plot("jet2_eta" , TH1F("h_jet2_eta"   ,"2nd Lead Jet;#eta(jet)"       , 20, -5.0, 5.0), "jet2_pt>20"),
    #Plot("jet2_phi" , TH1F("h_jet2_phi"   ,"2nd Lead Jet;#phi(jet)"       , 10, (-1.-1./4.)*pi, (1.+1./4.)*pi), "jet2_pt>20"),
    #Plot("jet_num"  , TH1F("h_jet_num"    ,"Number of Calo Jets (pt>20GeV);Num of Jets", 9, -0.5, 8.5)),
    Plot("pfjet_pt" , TH1F("h_pfjet_pt"   ,"Lead pfJet;p_{T}(jet) [GeV];Events / 10 GeV", 20, 0., 200.0), "pfjet_pt>30"),
    Plot("pfjet_eta", TH1F("h_pfjet_eta"  ,"Lead pfJet;#eta(jet);Events / 0.2"       , 24, -2.4, 2.4)),
    Plot("pfjet_phi", TH1F("h_pfjet_phi"  ,"Lead pfJet;#phi(jet);Events / 0.314"     , 20, -pi, pi)),
    Plot("pfjet2_pt" , TH1F("h_pfjet2_pt" ,"2nd Lead pfJet;p_{T}(2^{nd} jet) [GeV];Events / 10 GeV", 20, 0., 200.0), "pfjet2_pt>30"),
    #    Plot("pfjet2_eta", TH1F("h_pfjet2_eta","2nd Lead pfJet;#eta(2^{nd} jet)"       , 30, -2.5, 2.5), "pfjet2_pt>30"),
    #    Plot("pfjet2_phi", TH1F("h_pfjet2_phi","2nd Lead pfJet;#phi(2^{nd} jet)"       , 10, (-1.-1./4.)*pi, (1.+1./4.)*pi), "pfjet2_pt>20"),
    Plot("pfjet3_pt" , TH1F("h_pfjet3_pt" ,"3rd Lead pfJet;p_{T}(3^{rd} jet) [GeV];Events / 10 GeV", 20, 0., 200.0), "pfjet3_pt>30"),
    #    Plot("pfjet3_eta", TH1F("h_pfjet3_eta","3rd Lead pfJet;#eta(3^{rd} jet)"       , 20, -5.0, 5.0), "pfjet3_pt>20"),
    #    Plot("pfjet3_phi", TH1F("h_pfjet3_phi","3rd Lead pfJet;#phi(3^{rd} jet)"       , 10, (-1.-1./4.)*pi, (1.+1./4.)*pi), "pfjet3_pt>20"),
    #    Plot("pfjet4_pt" , TH1F("h_pfjet4_pt" ,"4th Lead pfJet;p_{T}(4^{th} jet) (GeV)", 40, 0., 200.0), "pfjet4_pt>20"),
    #    Plot("pfjet4_eta", TH1F("h_pfjet4_eta","4th Lead pfJet;#eta(4^{th} jet)"       , 20, -5.0, 5.0), "pfjet4_pt>20"),
    #    Plot("pfjet4_phi", TH1F("h_pfjet4_phi","4th Lead pfJet;#phi(4^{th} jet)"       , 10, (-1.-1./4.)*pi, (1.+1./4.)*pi), "pfjet4_pt>20"),
    #Plot("pfjet_num"     , TH1F("h_pfjet_num"     , "Number of pfJets (pt>30GeV);Num of Jets", 7, -0.5, 6.5)),
    Plot("pfjet_num_pt20", TH1F("h_pfjet_num_pt20", "Number of pfJets (p_{T}>20GeV);Exclusive Jet Number;Number of Events", 7, -0.5, 6.5)),
    Plot("pfjet_num_pt30", TH1F("h_pfjet_num_pt30", "Number of pfJets (p_{T}>30GeV);Exclusive Jet Number;Number of Events", 7, -0.5, 6.5)),
    #Plot("pfjet_num" , TH1F("h_pfjet_num_bin", "Number of pfJets (pt>30GeV);Num of Jets", len(bins_njet)-1, bins_njet)),
    #Plot("met_et/jet_sumAllEt", TH1F("h_met_o_jetSumEt","MET/Sum of Jet Et;MET/Sum Jet E_{T}", 20, 0.0, 10)),
    #Plot("delta_phi(photon_phi,pfjet_phi)", TH1F("h_dPhi_photon_pfjet","#Delta#phi(#gamma, Lead pfJet);#Delta#phi(#gamma,jet)",20, 0.0, pi)),
    #    Plot("delta_R(photon_eta,photon_phi,pfjet_eta,pfjet_phi)", TH1F("h_dR_photon_pfjet","#DeltaR(#gamma, Lead pfJet);#DeltaR(#gamma,jet)",20,0.0,7.0)),
    #    Plot("(photon_pt-pfjet_pt)/(photon_pt)", TH1F("h_dpt_photon_pfjet","(p_{T}(#gamma) - p_{T}(jet))/p_{T}(#gamma);#Deltap_{T}/p_{T}(#gamma)",20,-1.25,0.75), "pfjet_num<2&&delta_phi(photon_phi,pfjet_phi)>2.8"),
    #    Plot("pfjet2_pt/photon_pt", TH1F("h_pfjet2pt_photonpt","p_{T} ratio of 2nd jet / photon;p_{T}(2nd jet)/p_{T}(#gamma)", 20, 0.0, 2.0), "pfjet2_pt>20"),
    #    Plot("delta_phi(photon_phi,pfjet2_phi)", TH1F("h_dPhi_photon_pfjet2","#Delta#phi(#gamma, 2nd Lead Jet);#Delta#phi(#gamma,2^{nd} jet)",20, 0.0, pi), "pfjet2_pt>20"),
    #    Plot("delta_R(photon_eta,photon_phi,pfjet2_eta,pfjet2_phi)", TH1F("h_dR_photon_pfjet2","#DeltaR(#gamma,2^{nd} jet);#DeltaR(#gamma,2^{nd} jet)",20,0.0,7.0), "pfjet2_pt>20"),
    #    Plot("delta_phi(pfjet_phi,pfjet2_phi)", TH1F("h_dPhi_pfjet_pfjet2","#Delta#phi(Lead Jet, 2nd Jet);#Delta#phi(jet, 2^{nd} jet)",20, 0.0, pi), "pfjet2_pt>20"),
    #    Plot("delta_R(pfjet_eta,pfjet_phi,pfjet2_eta,pfjet2_phi)", TH1F("h_dR_pfjet_pfjet2","#DeltaR(Lead Jet, 2^{nd} Jet);#DeltaR(jet, 2^{nd} jet)",20,0.0,7.0), "pfjet2_pt>20"),
    #Plot("e_p_deltaR"    , TH1F("h_e_p_deltaR"  ,"#DeltaR between electron and photon object;#DeltaR(e,#gamma)", 20, 0.0, 0.2)),
    #Plot("p_met_massT"   , TH1F("h_p_met_massT","Transverse Mass (Met,#gamma);M_{T}(Met,#gamma) (GeV)", 24, 0., 120.0)),
    #Plot("e_met_massT"   , TH1F("h_e_met_massT","Transverse Mass (Met,e);M_{T}(Met,e) (GeV)"          , 24, 0., 120.0)),
    #Plot("met_et"        , TH1F("h_met_et", "Missing E_{T};MET (GeV)",20, 0., 100.0)),
    #Plot("e_met_deltaPhi", TH1F("h_e_met_deltaPhi", "#Delta#phi between e and Met;#Delta#phi(e,Met)", 20, 0.0, pi)),
    #Plot("met_et/met_sumEt",TH1F("h_met_et_over_sumEt","MET/SumEt;MET/SumEt", 20, 0.0, 2.0)),
    #Plot("met_et/(met_sumEt-photon_pt)",TH1F("h_met_et_over_sumEt_mPpt","MET/(SumEt-p_{T}(#gamma));MET/(SumEt-p_{T}(#gamma))", 20, 0.0, 2.0)),
    #Plot("met_et/met_sumEt:met_et",TH2F("h_metSumEt_vs_met","MET/SumEt vs MET; MET (GeV); MET/SumEt", 20, 15.0, 60.0, 20, 0.0, 2.0))
    ]

plot_list['sieie_only'] = [
    Plot("photon_sieie"        , TH1F("h_photon_sieie"     ,"Lead Photon;#sigma_{i#etai#eta}(#gamma);Number of Events",15, 0, 0.0225)),
    #Plot("photon_sieie"       , TH1F("h_photon_sieie_end" ,"Lead Photon;#sigma_{i#etai#eta}(#gamma);Number of Events",18, 0, 0.054)),
    Plot("photon_sieie-0.0003", TH1F("h_photon_sieie_end" ,"Lead Photon;#sigma_{i#etai#eta}(#gamma);Number of Events",18, 0, 0.054)),
    ]

plot_list['jet_counts'] = [
    Plot("pfjet_num_pt30hi", TH1F("h_pfjet_num_pt30hi", "Number of pfJets (p_{T}>30GeV);Exclusive Jet Number;Number of Events", 7, -0.5, 6.5)),
    Plot("pfjet_num_pt30"  , TH1F("h_pfjet_num_pt30n"  , "Number of pfJets (p_{T}>30GeV);Exclusive Jet Number;Number of Events", 7, -0.5, 6.5)),
    Plot("pfjet_num_pt30lo", TH1F("h_pfjet_num_pt30lo", "Number of pfJets (p_{T}>30GeV);Exclusive Jet Number;Number of Events", 7, -0.5, 6.5)),
    Plot("pfjet_num_pt20hi", TH1F("h_pfjet_num_pt20hi", "Number of pfJets (p_{T}>20GeV);Exclusive Jet Number;Number of Events", 7, -0.5, 6.5)),
    Plot("pfjet_num_pt20"  , TH1F("h_pfjet_num_pt20n"  , "Number of pfJets (p_{T}>20GeV);Exclusive Jet Number;Number of Events", 7, -0.5, 6.5)),
    Plot("pfjet_num_pt20lo", TH1F("h_pfjet_num_pt20lo", "Number of pfJets (p_{T}>20GeV);Exclusive Jet Number;Number of Events", 7, -0.5, 6.5)),
    ]
