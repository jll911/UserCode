# Configuration file for tree2hists
# Created Aug 06, 2010.
import sys
sys.path.insert(0, '')
_temp = __import__("t2h_filelist", globals(), locals(), ['file_list','plot_list'], -1)

list_of_files = _temp.file_list['data'] # choose:  gjet_mad, qcd_mad, gjet, qcdembc, qcd, data, data50, data70
list_of_plots = _temp.plot_list['rec_all'] + _temp.plot_list['sieie_only']  #+ _temp.plot_list['gen_all'] # choose: gen_all, rec_all, sieie_only

#output_filename = "Hists_PhotonJet_Madgraph.root"
#output_filename = "Hists_PhotonJet_Madgraph_unbinned.root"
#output_filename = "Hists_PhotonJet_Madgraph_photonID.root"
#output_filename = "Hists_PhotonJet_Madgraph_sideband.root"
#output_filename = "Hists_PhotonJet_Madgraph_GEN.root"
#output_filename = "Hists_PhotonJet_GEN.root"
#output_filename = "Hists_PhotonJet.root"
#output_filename = "Hists_PhotonJet_sideband.root"
#output_filename = "Hists_qcd.root"
#output_filename = "Hists_qcd_Madgraph.root"
#output_filename = "Hists_qcd_Madgraph_unbinned.root"
#output_filename = "Hists_qcd_Madgraph_photonID.root"
#output_filename = "Hists_qcd_Madgraph_sideband.root"
#output_filename = "Hists_qcdembc.root"
#output_filename = "Hists_qcdembc_sideband.root"
#output_filename = "Hists_Data.root"
#output_filename = "Hists_Data_unbinned.root"
#output_filename = "Hists_Data_JESup.root"
#output_filename = "Hists_Data_JetSys.root"
output_filename = "Hists_DataTrkFlip.root"
#output_filename = "Hists_DataTrkFlip_SidebandDOWN.root"
#output_filename = "Hists_Data_unbinned.root"
#output_filename = "Hists_Data50_unbinned.root"
output_filename = "hist_files/"+output_filename

useGenPhotonCuts   = False
binByPhotonPt      = False
binByNumJets30     = True
binByNumJets20     = False

genphoton_cuts = "genphoton_pt>75.0"
photon_cuts_   = "photon_pt>75&&photon_sieie>0.001"  # Basic
photon_cuts_L  = "photon_hOverE<0.05&&photon_ecalIso<4.2&&photon_hcalIso<2.2" # Loose
photon_cuts_T  = "photon_hOverE<0.03&&photon_ecalIso<2.4&&photon_hcalIso<1.0" # Tight
beam_halo_cuts = "((abs(photon_eta)<1.442&&(photon_sipip>0.009||photon_seedTime>-1.5))||(abs(photon_eta)>1.566))"
trk_cut_L      = "photon_trackIso<2.0"
trk_cut_T      = "photon_trackIso<0.9"
trk_cut_side   = "(photon_trackIso>2.0&&photon_trackIso<5.0)"
trk_cut_sideHI = "(photon_trackIso>3.0&&photon_trackIso<6.0)"
trk_cut_sideLO = "(photon_trackIso>2.0&&photon_trackIso<4.0)"
jet_cuts       = "pfjet_pt>30.0"

if useGenPhotonCuts:
    cut_for_all_files = "&&".join([genphoton_cuts])
    cuts_eta = [
        ("allEta", "|#eta^{#gamma}|<2.5"      , "abs(genphoton_eta)<2.5&&!(abs(genphoton_eta)<1.566&&abs(genphoton_eta)>1.442)"), # All
        ("barEta", "|#eta^{#gamma}|<1.442"    , "abs(genphoton_eta)<1.442"),                     # Barrel
        ("endEta", "1.566<|#eta^{#gamma}|<2.5", "abs(genphoton_eta)>1.566&&abs(genphoton_eta)<2.5") # Endcap
        ]
else:
    #cut_for_all_files = "&&".join(["photon_pt>30"])
    #cut_for_all_files = "&&".join([photon_cuts_,photon_cuts_L,trk_cut_L])   # For MC
    cut_for_all_files = "&&".join([photon_cuts_,photon_cuts_L,trk_cut_side,beam_halo_cuts])  # For Data
    cuts_eta = [
        ("allEta", "|#eta^{#gamma}|<2.5"      , "abs(photon_eta)<2.5&&!(abs(photon_eta)<1.566&&abs(photon_eta)>1.442)"), # All
        ("barEta", "|#eta^{#gamma}|<1.442"    , "abs(photon_eta)<1.442"),                     # Barrel
        ("endEta", "1.566<|#eta^{#gamma}|<2.5", "abs(photon_eta)>1.566&&abs(photon_eta)<2.5") # Endcap
        ]

cuts_pt = [("30<p_{T}^{#gamma}<35", "photon_pt>30&&photon_pt<35"),
           ("35<p_{T}^{#gamma}<40", "photon_pt>35&&photon_pt<40"),
           ("40<p_{T}^{#gamma}<45", "photon_pt>40&&photon_pt<45"),
           ("45<p_{T}^{#gamma}<50", "photon_pt>45&&photon_pt<50"),
           ("50<p_{T}^{#gamma}<55", "photon_pt>50&&photon_pt<55"),
           ("55<p_{T}^{#gamma}<60", "photon_pt>55&&photon_pt<60"),
           ("60<p_{T}^{#gamma}<65", "photon_pt>60&&photon_pt<65"),
           ("65<p_{T}^{#gamma}<70", "photon_pt>65&&photon_pt<70"),
           ("70<p_{T}^{#gamma}<75", "photon_pt>70&&photon_pt<75"),
           ("75<p_{T}^{#gamma}<80", "photon_pt>75&&photon_pt<80"),
           ("80<p_{T}^{#gamma}<100", "photon_pt>80&&photon_pt<100"),
           ("100<p_{T}^{#gamma}<120", "photon_pt>100&&photon_pt<120"),
           ("120<p_{T}^{#gamma}<200", "photon_pt>120&&photon_pt<200"),
           #("200<p_{T}^{#gamma}", "photon_pt>200")
           ]
cuts_jets30 = [
    ("0 jets pt>30","pfjet_num_pt30==0"),
    ("1 jets pt>30","pfjet_num_pt30==1"),
    ("2 jets pt>30","pfjet_num_pt30==2"),
    ("3 jets pt>30","pfjet_num_pt30==3"),
    ("4 jets pt>30","pfjet_num_pt30==4"),
    ("5 jets pt>30","pfjet_num_pt30==5"),
    ("6 jets pt>30","pfjet_num_pt30==6"),
    ]
cuts_jets20 = [
    ("0 jets pt>20","pfjet_num_pt20==0"),
    ("1 jets pt>20","pfjet_num_pt20==1"),
    ("2 jets pt>20","pfjet_num_pt20==2"),
    ("3 jets pt>20","pfjet_num_pt20==3"),
    ("4 jets pt>20","pfjet_num_pt20==4"),
    ("5 jets pt>20","pfjet_num_pt20==5"),
    ("6 jets pt>20","pfjet_num_pt20==6")
    ]
# All plots are made for each "cut set".
# A "cut set" is 3 things: folder name to store hists in, string to add to hist titles, and cuts for these hists.
# Let cut_sets = [] to make all plots.
cut_sets = []
if binByPhotonPt:
    for i,cut_eta in enumerate(cuts_eta):
        if (cut_eta[0]=="barEta" or cut_eta[0]=="endEta"):
            for j,cut_pt in enumerate(cuts_pt):
                cut_sets.append(("g_pass_Liso_%s_%i"%(cut_eta[0],j), "("+cut_eta[1]+", "+cut_pt[0]+")", cut_pt[1] + "&&"+cut_eta[2]))

if binByNumJets30:
    for i, cut_eta in enumerate(cuts_eta):
        if (cut_eta[0]=="barEta" or cut_eta[0]=="endEta"):
            for j,cut_jets in enumerate(cuts_jets30):
                cut_sets.append(("g_pass_Liso_%s_%ijets30"%(cut_eta[0],j), "("+cut_eta[1]+", "+cut_jets[0]+")", cut_jets[1] + "&&"+cut_eta[2]))

if binByNumJets20:
    for i, cut_eta in enumerate(cuts_eta):
        if (cut_eta[0]=="barEta" or cut_eta[0]=="endEta"):
            for j,cut_jets in enumerate(cuts_jets30):
                cut_sets.append(("g_pass_Liso_%s_%ijets20"%(cut_eta[0],j), "("+cut_eta[1]+", "+cut_jets[0]+")", cut_jets[1] + "&&"+cut_eta[2]))

cut_sets.append(("g_pass_Liso_allEta_allPt", "("+cuts_eta[0][1]+")",cuts_eta[0][2]))
cut_sets.append(("g_pass_Liso_barEta_allPt", "("+cuts_eta[1][1]+")",cuts_eta[1][2]))
cut_sets.append(("g_pass_Liso_endEta_allPt", "("+cuts_eta[2][1]+")",cuts_eta[2][2]))

