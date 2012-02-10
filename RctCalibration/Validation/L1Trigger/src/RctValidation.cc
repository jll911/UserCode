#include "Validation/L1Trigger/interface/RctValidation.h"
#include "Math/GenVector/VectorUtil.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include <algorithm>

using namespace std;
using namespace reco;
using namespace edm;

//
// constructors and destructor
//
RctValidation::RctValidation( const edm::ParameterSet& iConfig ) :
  ecalTPGs_(iConfig.getParameter<edm::InputTag>("ecalTPGs")),
  hcalTPGs_(iConfig.getParameter<edm::InputTag>("hcalTPGs")),
  rctEGammas_(iConfig.getParameter<edm::InputTag >("rctEGamma")),
  gctEGammas_(iConfig.getParameter<std::vector<edm::InputTag> >("gctEGamma")),
  refEGammas_(iConfig.getParameter<edm::InputTag>("genEGamma")),
  //  refJetSums_(iConfig.getParameter<edm::InputTag>("genJetSums")),
  directory_(iConfig.getParameter<std::string>("directory")),  
  outfile_(iConfig.getUntrackedParameter<std::string>("outputFileName")),
  maxEt_(iConfig.getUntrackedParameter<double>("maxEt",100)),
  binsEt_(iConfig.getUntrackedParameter<int>("binsEt",20)),
  binsEta_(iConfig.getUntrackedParameter<int>("binsEta",32)),
  binsPhi_(iConfig.getUntrackedParameter<int>("binsPhi",36)),
  matchDR_(iConfig.getUntrackedParameter<double>("matchDeltaR",0.5)),


  egammaThreshold_(iConfig.getUntrackedParameter<double>("gammaThreshold",5.)),
  tpgSumWindow_(iConfig.getUntrackedParameter<double>("tpgSumWindow",0.4)),
  thresholdForEtaPhi_(iConfig.getUntrackedParameter<double>("thresholdForEtaPhi",4.)),
  barrelBoundary_(iConfig.getUntrackedParameter<double>("barrelBarrelBoundry",1.442)),
  endcapBoundary_(iConfig.getUntrackedParameter<double>("endcapBoundry",1.566)),

  isolation_(iConfig.getUntrackedParameter<double>("iso",1.5)),
  HEcut_(iConfig.getUntrackedParameter<double>("heCut",0.1)),
  
  matchL1Objects_(iConfig.getUntrackedParameter<bool>("matchL1Objects",true))


{
  geo = new TriggerTowerGeometry();
  rctGeo = new L1RCTParameters();
  
  if ( outfile_.size() != 0 ) 
    {
      cout << "RCT Validation histograms will be saved to " << outfile_.c_str() << endl;
    }
  
  
  //Get General Monitoring Parameters
  store = &*edm::Service<DQMStore>();
  if(store)
    {

      store->setCurrentFolder(directory_);

      TPG_Resolution    = store->book1D("TPG_Resolution"," TPG (e/#gamma ET-ref e/#gamma E_{T})/Ref e/#gamma E_{T} ",100,-1.,1.);
      RCT_Resolution    = store->book1D("RCT_Resolution"," RCT (e/#gamma ET-ref e/#gamma E_{T})/Ref e/#gamma E_{T} ",100,-1.,1.);
      char histo[200];	
      for(int i =0 ; i< 28 ; ++i){
	
	sprintf(histo, "Resolution for RCT electrons i#eta %i",i+1);
	RCT_IetaResolution[i] = 	 store->book1D(histo,histo,100,-1.,1.);
      }
      RCT_EtaResolution = store->book1D("RCT_EtaResolution"," RCT e/#gamma #eta-ref e/#gamma #eta", 100, -2,2);
      RCT_PhiResolution = store->book1D("RCT_PhiResolution"," RCT e/#gamma #phi-ref e/#gamma #phi", 100, -2,2);


      refPt     = store->book1D("refPt" ,"ref e/#gamma P_{T}",binsEt_,0.,maxEt_);
      refPtNum  = store->book1D("refPtNum" ,"ref e/#gamma P_{T}",binsEt_,0.,maxEt_);
      refPtDen  = store->book1D("refPtDen" ,"ref e/#gamma P_{T}",binsEt_,0.,maxEt_);
      refPtEff  = store->book1D("refPtEff" ,"efficiency",binsEt_,0.,maxEt_);
      refPtIsoNum  = store->book1D("refPtIsoNum" ,"ref e/#gamma P_{T}",binsEt_,0.,maxEt_);
      refPtIsoDen  = store->book1D("refPtIsoDen" ,"ref e/#gamma P_{T}",binsEt_,0.,maxEt_);
      refPtIsoEff  = store->book1D("refPtIsoEff" ,"iso efficiency",binsEt_,0.,maxEt_);
      //		refPt->setResetMe(false );
      refPtBarrel     = store->book1D("refPtBarrel" ,"ref e/#gamma P_{T} (barrel)",binsEt_,0.,maxEt_);
      refPtEndcap     = store->book1D("refPtEndcap" ,"ref e/#gamma P_{T} (endcap)",binsEt_,0.,maxEt_);
      refEt     = store->book1D("refEt" ,"ref e/#gamma E_{T}",binsEt_,0.,maxEt_);
      refEta    = store->book1D("refEta","ref e/#gamma #eta",binsEta_,-3,3);
      refPhi    = store->book1D("refPhi","ref e/#gamma #phi",binsPhi_,-3.145,3.145);
      refEtaPhi = store->book2D("refEtaPhi","ref e/#gamma #eta #phi",binsEta_,-3,3,binsPhi_,-3.145,3.145);

      // CAN YOU DO THIS BEFORE THEY'RE FILLED??
      refPt->getTH1F()->Sumw2();
      refPtEndcap->getTH1F()->Sumw2();
      refPtBarrel->getTH1F()->Sumw2();
      refEt->getTH1F()->Sumw2();
      refEta->getTH1F()->Sumw2();
      refPhi->getTH1F()->Sumw2();
      //      refEtaPhi->getTH1F()->Sumw2();

      tpgEffPt     = store->book1D("tpgEffPt" ,"tpg e/#gamma P_{T}",binsEt_,0.,maxEt_);
      tpgEffPtBarrel     = store->book1D("tpgEffPtBarrel" ,"tpg e/#gamma P_{T} (barrel)",binsEt_,0.,maxEt_);
      tpgEffPtEndcap     = store->book1D("tpgEffPtEndcap" ,"tpg e/#gamma P_{T} (endcap)",binsEt_,0.,maxEt_);
      tpgEffEt     = store->book1D("tpgEffEt" ,"tpg e/#gamma E_{T}",binsEt_,0.,maxEt_);
      tpgEffEta    = store->book1D("tpgEffEta","tpg e/#gamma #eta",binsEta_,-3,3);
      tpgEffPhi    = store->book1D("tpgEffPhi","tpg e/#gamma #phi",binsPhi_,-3.145,3.145);
      tpgEffEtaPhi = store->book2D("tpgEffEtaPhi","tpg e/#gamma #eta #phi",binsEta_,-3,3,binsPhi_,-3.145,3.145);


      tpgEffPt->getTH1F()->Sumw2();
      tpgEffPtBarrel->getTH1F()->Sumw2();		
      tpgEffPtEndcap->getTH1F()->Sumw2();
      
      tpgEffEt->getTH1F()->Sumw2();
      tpgEffEta->getTH1F()->Sumw2();
      tpgEffPhi->getTH1F()->Sumw2();

      HCALtpgPt = store->book1D("HCALtpgPt" ,"HCAL tpg e/#gamma P_{T}",80,0.,20);
      HCALtpgPt->getTH1F()->Sumw2();
      //      tpgEffEtaPhi->getTH1F()->Sumw2();



      rctEffPt     = store->book1D("rctEffPt" ,"rct e/#gamma P_{T}",binsEt_,0.,maxEt_);
      rctEffPtBarrel     = store->book1D("rctEffPtBarrel" ,"rct e/#gamma P_{T} (barrel)",binsEt_,0.,maxEt_);
      rctEffPtEndcap     = store->book1D("rctEffPtEndcap" ,"rct e/#gamma P_{T} (endcap)",binsEt_,0.,maxEt_);
      rctEffEt     = store->book1D("rctEffEt" ,"rct e/#gamma E_{T}",binsEt_,0.,maxEt_);
      rctEffEta    = store->book1D("rctEffEta","rct e/#gamma #eta",binsEta_,-3,3);
      rctEffPhi    = store->book1D("rctEffPhi","rct e/#gamma #phi",binsPhi_,-3.145,3.145);
      rctEffEtaPhi = store->book2D("rctEffEtaPhi","rct e/#gamma #eta #phi",binsEta_,-3,3,binsPhi_,-3.145,3.145);

      
      rctEffPtHighest = store->book1D("rctEffPtHighest" ,"rct e/#gamma E_{T}",binsEt_,0.,maxEt_);

      rctPtNoSpike = store->book1D("rctPtNoSpike" ,"rct e/#gamma E_{T} wo spike",binsEt_,0.,maxEt_);
      rctPtEGFGcut = store->book1D("rctPtEGFGcut" ,"FG: rct e/#gamma E_{T} objects",binsEt_,0.,maxEt_);
      rctPtEGHEcut = store->book1D("rctPtEGHEcut" ,"H/E: rct e/#gamma E_{T} objects",binsEt_,0.,maxEt_);
      rctPtIsoEGHEFGcut = store->book1D("rctPtIsoEGHEFGcut" ,"HE+FG: rct isolated e/#gamma E_{T}",binsEt_,0.,maxEt_);
      rctPtIsoEGHEFGIsocut = store->book1D("rctPtIsoEGHEFGIsocut" ,"HE+FG iso: rct isolated e/#gamma E_{T}",binsEt_,0.,maxEt_);


      rctPtNoSpike->getTH1F()->Sumw2();
      rctPtEGHEcut->getTH1F()->Sumw2();
      rctPtEGFGcut->getTH1F()->Sumw2();
      rctPtIsoEGHEFGcut->getTH1F()->Sumw2();
      rctPtIsoEGHEFGIsocut->getTH1F()->Sumw2();


      rctEffPt->getTH1F()->Sumw2();
      rctEffPtBarrel->getTH1F()->Sumw2();
      rctEffPtEndcap->getTH1F()->Sumw2();
      rctEffEt->getTH1F()->Sumw2();
      rctEffEta->getTH1F()->Sumw2();
      rctEffPhi->getTH1F()->Sumw2();
      rctEffPtHighest->getTH1F()->Sumw2();
      //      rctEffEtaPhi->getTH1F()->Sumw2();


      rctIsoEffPt     = store->book1D("rctIsoEffPt" ,"rctIso e/#gamma P_{T}",binsEt_,0.,maxEt_);
      rctIsoEffPtBarrel     = store->book1D("rctIsoEffPtBarrel" ,"rctIso e/#gamma P_{T} (barrel)",binsEt_,0.,maxEt_);
      rctIsoEffPtEndcap     = store->book1D("rctIsoEffPtEndcap" ,"rctIso e/#gamma P_{T} (endcap)",binsEt_,0.,maxEt_);
      rctIsoEffEt     = store->book1D("rctIsoEffEt" ,"rctIso e/#gamma E_{T}",binsEt_,0.,maxEt_);
      rctIsoEffEta    = store->book1D("rctIsoEffEta","rctIso e/#gamma #eta",binsEta_,-3,3);
      rctIsoEffPhi    = store->book1D("rctIsoEffPhi","rctIso e/#gamma #phi",binsPhi_,-3.1458,3.1459);
      rctIsoEffEtaPhi = store->book2D("rctIsoEffEtaPhi","rctIso e/#gamma #eta #phi",binsEta_,-3,3,binsPhi_,-3.1459,3.1459);


      rctIsoEffPtHighest = store->book1D("rctIsoEffPtHighest" ,"rct e/#gamma E_{T}",binsEt_,0.,maxEt_);
      
      rctIsoEffPt->getTH1F()->Sumw2();
      rctIsoEffPtBarrel->getTH1F()->Sumw2();
      rctIsoEffPtEndcap->getTH1F()->Sumw2();
      rctIsoEffEt->getTH1F()->Sumw2();
      rctIsoEffEta->getTH1F()->Sumw2();
      rctIsoEffPhi->getTH1F()->Sumw2();
      rctIsoEffPtHighest->getTH1F()->Sumw2();
      //      rctIsoEffEtaPhi->getTH1F()->Sumw2();


      gctEffPt     = store->book1D("gctEffPt" ,"gct e/#gamma P_{T}",binsEt_,0.,maxEt_);
      gctEffEt     = store->book1D("gctEffEt" ,"gct e/#gamma E_{T}",binsEt_,0.,maxEt_);
      gctEffEta    = store->book1D("gctEffEta","gct e/#gamma #eta",binsEta_,-3,3);
      gctEffPhi    = store->book1D("gctEffPhi","gct e/#gamma #phi",binsPhi_,-3.1459,3.1459);
      gctEffEtaPhi = store->book2D("gctEffEtaPhi","gct e/#gamma #eta #phi",binsEta_,-3,3,binsPhi_,-3.1459,3.1459);


      gctEffPt->getTH1F()->Sumw2();
      gctEffEt->getTH1F()->Sumw2();
      gctEffEta->getTH1F()->Sumw2();
      gctEffPhi->getTH1F()->Sumw2();
      //      gctEffEtaPhi->getTH1F()->Sumw2();

      gctIsoEffPt     = store->book1D("gctIsoEffPt" ,"gctIso e/#gamma P_{T}",binsEt_,0.,maxEt_);
      gctIsoEffEt     = store->book1D("gctIsoEffEt" ,"gctIso e/#gamma E_{T}",binsEt_,0.,maxEt_);
      gctIsoEffEta    = store->book1D("gctIsoEffEta","gctIso e/#gamma #eta",binsEta_,-3,3);
      gctIsoEffPhi    = store->book1D("gctIsoEffPhi","gctIso e/#gamma #phi",binsPhi_,-3.1459,3.1459);
      gctIsoEffEtaPhi = store->book2D("gctIsoEffEtaPhi","gctIso e/#gamma #eta #phi",binsEta_,-3,3,binsPhi_,-3.1459,3.1459);


      gctIsoEffPt->getTH1F()->Sumw2();

      gctIsoEffEt->getTH1F()->Sumw2();
      gctIsoEffEta->getTH1F()->Sumw2();
      gctIsoEffPhi->getTH1F()->Sumw2();



      rctEtaCorr = store->bookProfile("rctEtaCorr", "RCT #eta corrections", binsEta_,-3,3, 50, 0, 2," "); 

      rctEtaCorrIEta = store->bookProfile("rctEtaCorrIEta", "RCT #eta corrections", 65, -32.5, 32.5, 50, 0, 2," ");
      rctEtaCorrAbsIEta = store->bookProfile("rctEtaCorrAbsIEta", "RCT |#eta| corrections", 32, 0.5, 32.5, 50, 0, 2," ");

      rctEtaCorr1Tower = store->bookProfile("rctEtaCorr1Tower", "RCT #eta corrections (1 Tower restriction)", binsEta_,-3,3, 50, 0, 2," "); 
      rctEtaCorrIEta1Tower = store->bookProfile("rctEtaCorrIEta1Tower", "RCT #eta corrections (1 Tower restriction)", 65, -32.5, 32.5, 50, 0, 2," ");
      rctEtaCorrAbsIEta1Tower = store->bookProfile("rctEtaCorrAbsIEta1Tower", "RCT |#eta| corrections (1 Tower restriction)", 32, 0.5, 32.5, 50, 0, 2," ");

      rctEtaCorr12Tower = store->bookProfile("rctEtaCorr12Tower", "RCT #eta corrections (1 or 2 Tower restriction)", binsEta_,-3,3, 50, 0, 2," "); 
      rctEtaCorrIEta12Tower = store->bookProfile("rctEtaCorrIEta12Tower", "RCT #eta corrections (1 or 2 Tower restriction)", 65, -32.5, 32.5, 50, 0, 2," ");
      rctEtaCorrAbsIEta12Tower = store->bookProfile("rctEtaCorrAbsIEta12Tower", "RCT |#eta| corrections (1 or 2 Tower restriction)", 32, 0.5, 32.5, 50, 0, 2," ");
		
		
      rctEtaCorrEt = store->bookProfile("rctEtaCorrEt", "RCT #eta corrections", binsEta_,-3,3, 50, 0, 2," "); 
      rctEtaCorrIEtaEt = store->bookProfile("rctEtaCorrIEtaEt", "RCT #eta corrections", 65, -32.5, 32.5, 50, 0, 2," ");
      rctEtaCorrAbsIEtaEt = store->bookProfile("rctEtaCorrAbsIEtaEt", "RCT |#eta| corrections", 32, 0.5, 32.5, 50, 0, 2," ");
      
      rctEtaCorr1TowerEt = store->bookProfile("rctEtaCorr1TowerEt", "RCT #eta corrections (1 Tower restriction)", binsEta_,-3,3, 50, 0, 2," "); 
      rctEtaCorrIEta1TowerEt = store->bookProfile("rctEtaCorrIEta1TowerEt", "RCT #eta corrections (1 Tower restriction)", 65, -32.5, 32.5, 50, 0, 2," ");
      rctEtaCorrAbsIEta1TowerEt = store->bookProfile("rctEtaCorrAbsIEta1TowerEt", "RCT |#eta| corrections (1 Tower restriction)", 32, 0.5, 32.5, 50, 0, 2," ");
      
      rctEtaCorr12TowerEt = store->bookProfile("rctEtaCorr12TowerEt", "RCT #eta corrections (1 or 2 Tower restriction)", binsEta_,-3,3, 50, 0, 2," "); 
      rctEtaCorrIEta12TowerEt = store->bookProfile("rctEtaCorrIEta12TowerEt", "RCT #eta corrections (1 or 2 Tower restriction)", 65, -32.5, 32.5, 50, 0, 2," ");
      rctEtaCorrAbsIEta12TowerEt = store->bookProfile("rctEtaCorrAbsIEta12TowerEt", "RCT |#eta| corrections (1 or 2 Tower restriction)", 32, 0.5, 32.5, 50, 0, 2," ");
      
      
      
      regionEGtowers = store->book1D("regionEGtowers","nTowers in Region with L1 EG candidate",17,-0.5,16.5);
      regionEGtowersRatio = store->book1D("regionEGtowersRatio", "energy of peak tower to total of Region", 20, 0, 1);
      regionEG2towersRatio = store->book1D("regionEG2towersRatio", "energy of 2 peak tower to total of Region", 20, 0, 1);
      regionEGHighTowerEt = store->book1D("regionEGHighTowerEt" ,"tpg e/#gamma E_{T} of high tower in object",binsEt_,0.,maxEt_);
      
      
      regionEGtowersRatio->getTH1F()->Sumw2();
      regionEGHighTowerEt->getTH1F()->Sumw2();
      
      rctHEvL1Et = store->book2D("rctHEvL1Et","H/E values versus L1 e/#gamma E_{T}", 64, -0.5, 32.5, 40, 0, 1);
      rctHEvEt = store->book2D("rctHEvEt","H/E values versus e/#gamma E_{T}", 64, -0.5, 32.5, 40, 0, 1);
      rctHEvECALEt = store->book2D("rctHEvECALEt","H/E values versus ECAL e/#gamma E_{T}", 64, -0.5, 32.5, 40, 0, 1);
      rctHEvHCALEt = store->book2D("rctHEvHCALEt","H/E values versus HCAL e/#gamma E_{T}", 64, -0.5, 32.5, 40, 0, 1);
      rctHE = store->book1D("rctHE","H/E values of L1 e/#gamma objects", 40, 0, 1);
      rctHEafterFG = store->book1D("rctHEafterFG","H/E values of L1 e/#gamma objects", 40, 0, 1);
      regionHE = store->book1D("regionHE","H/E values of L1 e/#gamma 3x3 objects", 40, 0, 1);
      regionMaxHE = store->book1D("regionMaxHE","H/E values of L1 e/#gamma 3x3 objects", 40, 0, 1);
      regionMaxHEafterFG = store->book1D("regionMaxHEafterFG","H/E values of L1 e/#gamma 3x3 objects", 40, 0, 1);
      
      rctHEvL1Et->getTH2F()->Sumw2();
      rctHEvEt->getTH2F()->Sumw2();
      rctHEvECALEt->getTH2F()->Sumw2();
      rctHEvHCALEt->getTH2F()->Sumw2();
      rctHE->getTH1F()->Sumw2();
      rctHEafterFG->getTH1F()->Sumw2();
      regionMaxHE->getTH1F()->Sumw2();
      regionMaxHEafterFG->getTH1F()->Sumw2();
      
      regionHE->getTH1F()->Sumw2();
      
      tpgECALsecondtower  = store->book1D("tpgECALsecondtower" ,"tpg e/#gamma E_{T} of neighboring tower in object",80,0.,20);
      
      tpgHCALSurrounding  = store->book1D("tpgHCALSurrounding" ,"tpg e/#gamma E_{T} sum of HCAL  of surrounding towers )",80,0.,20);
      tpgECALSurrounding  = store->book1D("tpgECALSurrounding" ,"tpg e/#gamma E_{T} sum of HCAL region towers (minus neighbor)",80,0.,20);
      
      dirHCALclosest = store->book2D("dirHCALclosest","direction of a HCAL tower with energy in ieta/iphi space",3,-1.5,1.5,3,-1.5,1.5);
      dirHCALclosestWeighted = store->book2D("dirHCALclosestWeighted","direction of a HCAL tower with energy in ieta/iphi space*energy",3,-1.5,1.5,3,-1.5,1.5);
      sumECAL  = store->book1D("sumECAL" ,"tpg e/#gamma E_{T} sum of ECAL 3x3 region towers",80,0.,40);
      sumHCAL  = store->book1D("sumHCAL" ,"tpg e/#gamma E_{T} sum of HCAL 3x3 region towers",80,0.,40);
      regionSum =	    store->book1D("regionSum" ,"tpg e/#gamma E_{T} sum of 3x3 region towers",80,0.,40);
      diffSumEgamma = store->book1D("diffSumEgamma", "energy of other towers",40,0.,10);
      minLSum = store->book1D("minLSum", "Smallest Neighbor Corner Energy",40,0.,10);
      minLSumHE = store->book1D("minLSumHE", "Smallest Neighbor Corner Energy",40,0.,10);
      
      rctFGneighborEt = store->book1D("rctFGneighborEt","Max Et of neighbor with FG", 80, 0., 40);
      rctFGMainTowerEt = store->book1D("rctFGMainTowerEt"," Et of central with FG", 40, 0., 20);
      //		rctNoFGneighborEt = store->book1D("rctFGneighborEt","Max Et of neighbor without FG", 40, 0., 20);
      rctNoFGMainTowerEt = store->book1D("rctNoFGMainTowerEt"," Et of central without FG", 80, 0., 40);
      
      
      rctFGneighborEt->getTH1F()->Sumw2();
      rctFGMainTowerEt->getTH1F()->Sumw2();
      rctNoFGMainTowerEt->getTH1F()->Sumw2();
      tpgECALsecondtower->getTH1F()->Sumw2();
      tpgHCALSurrounding->getTH1F()->Sumw2();
      tpgECALSurrounding->getTH1F()->Sumw2();
      dirHCALclosest->getTH2F()->Sumw2();
      dirHCALclosestWeighted->getTH2F()->Sumw2();		
      sumECAL->getTH1F()->Sumw2();
      sumHCAL->getTH1F()->Sumw2();
      diffSumEgamma->getTH1F()->Sumw2();
      minLSum->getTH1F()->Sumw2();
      minLSumHE->getTH1F()->Sumw2();
      
      store->setVerbose(1);

    }
}

RctValidation::~RctValidation()
{
   if(geo != 0)
     delete geo;
   
   if(rctGeo != 0)
     delete rctGeo;
   
   // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)
  
}


//--------------------------------------------------------


//--------------------------------------------------------
void RctValidation::beginRun(const edm::Run& r, const EventSetup& iSetup) {
  //Get The Geometry and the scale
  ESHandle< L1CaloGeometry > caloGeomESH ;
  iSetup.get< L1CaloGeometryRecord >().get( caloGeomESH ) ;
  // const L1CaloGeometry* caloGeom = caloGeomESH.product();
  caloGeom = caloGeomESH.product();
	
  // get energy scale to convert input from ECAL
  edm::ESHandle<L1CaloEcalScale> ecalScale;
  iSetup.get<L1CaloEcalScaleRcd>().get(ecalScale);
  //  const L1CaloEcalScale* eS = ecalScale.product();
  eS = ecalScale.product();

  // get energy scale to convert input from ECAL
  edm::ESHandle<L1CaloHcalScale> hcalScale;
  iSetup.get<L1CaloHcalScaleRcd>().get(hcalScale);
  //  const L1CaloEcalScale* eS = ecalScale.product();
  hS = hcalScale.product();
	
  ESHandle< L1CaloEtScale > emScale ;
  iSetup.get< L1EmEtScaleRcd >().get( emScale ) ;
  //  const L1CaloEtScale* emS = emScale.product();
  emS = emScale.product();
	
  //	std::cout << " reference pt nentries at begining of new run " << refPt->getEntries() << std::endl;
	
}

//--------------------------------------------------------
void RctValidation::beginLuminosityBlock(const LuminosityBlock& lumiSeg, 
				      const EventSetup& context) {
  
}

// ----------------------------------------------------------
void 
RctValidation::analyze(const Event& iEvent, const EventSetup& iSetup )
{
  //std::cout << " looking for reference " << std::endl;
  //     	std::cout << " reference pt nentries at begining of new event " << refPt->getEntries() << std::endl;
  //Get Reference Objects
  edm::Handle<edm::View<reco::Candidate> > genEGamma;
  bool gotRef = iEvent.getByLabel(refEGammas_,genEGamma) && !genEGamma.failedToGet();

  if(!gotRef)
    std::cout << " no reference found " << std::endl;
  //	else	
  //	std::cout << "  reference FOUND " << std::endl;
  //get ECAL TPGs
  edm::Handle<EcalTrigPrimDigiCollection> ecalTPGs;
  bool gotECALTPs = iEvent.getByLabel(ecalTPGs_,ecalTPGs);
  if(!gotECALTPs)
    printf("NO ECAL TPs found\n");

  //get ECAL TPGs
  edm::Handle<HcalTrigPrimDigiCollection> hcalTPGs;
  bool gotHCALTPs = iEvent.getByLabel(hcalTPGs_,hcalTPGs);
  if(!gotHCALTPs)
    printf("NO HCAL TPs found\n");

  //Get Egammas from RCT
  double highestEG = 0;
  double highestEGiso = 0;
  L1GctEmCandCollection rctEGammas;
  edm::Handle<L1CaloEmCollection> egamma;

  // loop through cands, find highest-et, record highest-et iso energy
  L1CaloEmCand rctEGamma;	
  if(iEvent.getByLabel(rctEGammas_,egamma) && !egamma.failedToGet())
    for(L1CaloEmCollection::const_iterator i=egamma->begin();i!=egamma->end();++i)
    {
      double i_et = emS->et(i->rank());
      L1CaloEmCand rctEGammaTemp = *i;
      rctEGammas.push_back(L1GctEmCand(rctEGammaTemp));
      if(highestEG < i_et) {
	highestEG = i_et;
	rctEGamma = *i;
      }
      if ( i->isolated() )
        highestEGiso = max(highestEGiso, i_et);

    }
  double rctEGamma_et = emS->et(rctEGamma.rank());

  // now do stuff with that highest-et candidate
  if ( highestEG > 0 ) {
    rctEffPtHighest->Fill(highestEG);   
    if( highestEG>=egammaThreshold_ && !matchL1Objects_){ // if don't require matching of l1 objects?
      EcalTrigPrimDigiCollection* l1TrigTowers = new EcalTrigPrimDigiCollection();
      findRegionTowers(rctEGamma,*ecalTPGs,l1TrigTowers); // this is the deal with finding the candidate's trigger towers

      EcalTriggerPrimitiveDigi highestEtTT;
      int nTowers =0 ;
      double totalEnergy = 0., highestEtTTet =-1.,highestEtTTHCAL= 0.;
      bool centralFG = false, centralSpike = false;
      // go through the trigger towers in the region for that cand
      for(EcalTrigPrimDigiCollection::const_iterator iTT = l1TrigTowers->begin() ; iTT != l1TrigTowers->end(); ++iTT){
	//			  std::cout << "loop, high trig tower et " << highestEtTTet <<std::endl;
	int iTT_ieta = iTT->id().ieta();
	double et = eS->et(iTT->compressedEt(),abs(iTT_ieta),iTT_ieta/abs(iTT_ieta));	
	//			  std::cout << "loop, trig tower  compressed et " << iTT->compressedEt(); //<<std::endl;	
	totalEnergy += et; // sum alllll the towers
	if(et> 0)
	  ++nTowers; // only count towers with energy > 0
	if(et > highestEtTTet ) { // pick highest-et tower yay -- dude, isn't it always the central one??  YES, and this is messing with TPLF ordering for cands made up of two same-energy towers!  Well, this is for the whole region, not the 3x3 around a cand.  But it is still messing with TPLF (maybe doesn't matter)
	  highestEtTT = *iTT;
	  highestEtTTet =et;
	  centralFG = iTT->fineGrain() && et< 30; // WHY IS THIS HARD-CODED. (yes, that's a period.)
	  centralSpike = (iTT->l1aSpike()==0); // ooh, looks like EcalTrigPrimDigi has a new method
	}
      }
//       int ieta =  highestEtTT.id().ieta();
//       int iphi = highestEtTT.id().iphi();


      // don't look at spike events or fineGrain events  
      if(!centralSpike) { // wait, this actually only refers to spike events, we're still looking at "fine grain" events
	  
	rctPtNoSpike->Fill(rctEGamma_et);
	  
 	int ieta =  highestEtTT.id().ieta();
 	int iphi = highestEtTT.id().iphi();
	double energyMaxFGcorner = -1;
	//    std::cout<< " new part begining";
	std::vector<double> tpgs3x3 =    find3x3Towers(ieta, iphi, *ecalTPGs, *hcalTPGs); // HERE's where we get the 3x3 around the highest-et cand
	//    std::cout << " after finding 3x3 towers size " << tpgs3x3.size() <<std::endl;
	double ecal3x3Tot =0.;
	double hcal3x3Tot =0.;
	double highestNeighborEcal = -1;
	double minELsum = 9999;
	double highestHEneighbor = 0;
	// loop through towers in 3x3
	for(int i = 0; i< 9; ++i){

	  double ecalTTEt= tpgs3x3.at(i);
	  double hcalTTEt = tpgs3x3.at(i+9);
	    
	  // i:
	  //   | 0 | 1 | 2 |
	  //   | 3 | 4 | 5 | 
	  //   | 6 | 7 | 8 |

	  // i/3:                 i/3 - 1:
	  //   | 0 | 0 | 0 |       |-1 |-1 |-1 |
	  //   | 1 | 1 | 1 |  ==>  | 0 | 0 | 0 |
	  //   | 2 | 2 | 2 |       | 1 | 1 | 1 |

	  // i%3:                 i%3 - 1:
	  //   | 0 | 1 | 2 |       |-1 | 0 | 1 |
	  //   | 0 | 1 | 2 |  ==>  |-1 | 0 | 1 |
	  //   | 0 | 1 | 2 |       |-1 | 0 | 1 |


	  if(hcalTTEt>0  && ecalTTEt < 30){ // hard-coded again!
	    dirHCALclosest->Fill((i%3)-1,(i/3)-1); // fills position of given tower (if it has energy), relative to central tower (candidate tower)
	    dirHCALclosestWeighted->Fill((i%3)-1,(i/3)-1, hcalTTEt); // same, weighted by energy
	    if(ecalTTEt >2){  // minimum ecal energy for he cut // HARD-CODED
	      if(highestHEneighbor < (hcalTTEt/ecalTTEt )) // get highest H/E value for any of the towers -- including the central tower?? I THINK THIS IS A BUG, it's different later!
		highestHEneighbor = hcalTTEt/ecalTTEt ;
	    }  else{ 
	      if(hcalTTEt >2) // above noise threshold // HARD-CODED
		highestHEneighbor = 0.99;  // if ecal < 2 and hcal > 2, make it max value less than 1 as opposed to anything greater than 1
	    }
	  }
	      
	  // i%2:
	  //   | 0 | 1 | 0 |
	  //   | 1 | 0 | 1 |
	  //   | 0 | 1 | 0 |

	  ecal3x3Tot += ecalTTEt;
	  hcal3x3Tot += hcalTTEt;
	  if( i%2 ==1  )  { // find neighbors of ecal // only want the direct neighbors?
	    if(ecalTTEt > highestNeighborEcal)
	      highestNeighborEcal = ecalTTEt;
	  }

	  if(i%2 ==0 && i!=4){ // only want the corners?
	    double tempELsum = 0;
	    int cornerEta = i/3;
	    int cornerPhi = i%3;
	    for(int j=0; j<3; ++j){
	      if(tempELsum <  tpgs3x3.at(cornerEta*3 + j)) // what exactly is this looking at? where's the sum? and why isn't it using a whole L at once?
		tempELsum =   tpgs3x3.at(cornerEta*3 + j);
	      if(tempELsum <  tpgs3x3.at(j*3 + cornerPhi))
		tempELsum = tpgs3x3.at(j*3 + cornerPhi);
	    }
	    if(tempELsum < minELsum)
	      minELsum = tempELsum;
	  }
	  if(i==4)
	    highestEtTTHCAL = hcalTTEt; // if central tower, set hcal energy
	  else
	    if(tpgs3x3.at(i+18 ) == 1) // else if fine grain set, 
	      energyMaxFGcorner = ecalTTEt;   // make whatever this variable is equal to the ECAL energy
	} // end loop through towers in 3x3
	    
	  
	double rctHe =highestEtTTHCAL/highestEtTTet;
	rctHE->Fill(rctHe);
	rctHEvL1Et->Fill(rctEGamma_et,rctHe);
	    
	rctHEvECALEt->Fill(highestEtTTet,rctHe);
	rctHEvHCALEt->Fill(highestEtTTHCAL,rctHe);
	HCALtpgPt->Fill(highestEtTTHCAL);
	tpgECALsecondtower->Fill(highestNeighborEcal);
	tpgHCALSurrounding->Fill(hcal3x3Tot); // but not just surrounding towers, also includes central tower! ah, because they don't count hcal for central tower, I guess
	tpgECALSurrounding->Fill(ecal3x3Tot-highestEtTTet); 
	regionSum->Fill(hcal3x3Tot+ecal3x3Tot);
	regionHE->Fill(hcal3x3Tot/ecal3x3Tot);
	diffSumEgamma->Fill(ecal3x3Tot-highestEtTTet - highestNeighborEcal); // difference between what two quantities here?
	    
	regionMaxHE->Fill(highestHEneighbor);
	if(centralFG) {
	  rctFGMainTowerEt->Fill(highestEtTTet);
	} else {
	  rctPtEGFGcut->Fill(rctEGamma_et);
	  rctNoFGMainTowerEt->Fill(highestEtTTet);
	  rctHEafterFG->Fill(rctHe);
	  if(rctHe < HEcut_){
	    rctPtEGHEcut->Fill(rctEGamma_et);

	    if(energyMaxFGcorner > 0) 
	      rctFGneighborEt->Fill(energyMaxFGcorner);
	    else {
	      minLSum->Fill(minELsum);
	      regionMaxHEafterFG->Fill(highestHEneighbor);
	      if(highestHEneighbor < HEcut_) {
		rctPtIsoEGHEFGcut->Fill(rctEGamma_et);
		minLSumHE->Fill(minELsum);
		if(minELsum < isolation_)
		  rctPtIsoEGHEFGIsocut->Fill(rctEGamma_et);
	      }
	    }
	  }
	}
      } // end if !centralSpike
    } // end if highestEGet >=egammaThreshold and !matchL1objects
  } // end if highestEGet > 0
  else	
    std::cout << " no RCT objects found" << std::endl;

  if ( highestEGiso > 0 )
    rctIsoEffPtHighest->Fill(highestEGiso);

  //Get Egammas from GCT
  L1GctEmCandCollection gctEGammas;
  edm::Handle<L1GctEmCandCollection> gctEgamma;
  if(iEvent.getByLabel(gctEGammas_[0],gctEgamma))
    gctEGammas = *gctEgamma;

  //Get Isolated Egammas from GCT
  L1GctEmCandCollection gctIsoEGammas;
  edm::Handle<L1GctEmCandCollection> gctIsoEgamma;
  if(iEvent.getByLabel(gctEGammas_[1],gctIsoEgamma))
    gctIsoEGammas = *gctIsoEgamma;

  //  	std::cout << "testing" << std::endl;
  //OK Now Efficiency calculations
	
  //  	std::cout << " number of objects " << genEGamma->size() <<std::endl;
  if(gotRef){
    for(  edm::View<reco::Candidate> ::const_iterator j = genEGamma->begin() ;   j != genEGamma->end(); ++j ) // loop through gen cands
      //   for(unsigned int j=0;j<genEGamma->size();++j) 
      {
	double j_pt = j->pt();
	double j_et = j->et();
	double j_eta = j->eta();
	double j_phi = j->phi();
	//bool j_iso = j->isolated();
	math::XYZTLorentzVector j_p4 = j->p4();

	refPtDen->Fill(j_pt);
	//if(j_iso)
	//{
	refPtIsoDen->Fill(j_pt);
	//}

	// SEE IF THERE ARE ANY L1 MATCHES
	bool refIsMatched = false;
	bool l1IsIsolated = false;
        if(rctEGammas.size()>0)
	  {
	    for(L1GctEmCandCollection::const_iterator i=rctEGammas.begin();i!=rctEGammas.end();++i)
	      {
		double i_et = emS->et(i->rank());
		if(i_et>=egammaThreshold_)
		  {
		    //create lorentz vector
		    math::PtEtaPhiMLorentzVector rctVec = rctLorentzVector(*i);
		    double deltaR = ROOT::Math::VectorUtil::DeltaR(rctVec,j_p4);
		    if( deltaR < matchDR_)
		      {
			//rctNearReference.push_back(*i);
			refIsMatched = true;
			l1IsIsolated = i->isolated();
			//break;
		      }
		  }
	      }
	  }
	if (refIsMatched)
	  {
	    refPtNum->Fill(j_pt);	    
	    if(l1IsIsolated)
	      {
		refPtIsoNum->Fill(j_pt);
	      }
	  }

	//	math::PtEtaPhiMLorentzVector j_p4 = j->p4();
	//std::cout << " electron energy " << j_pt <<std::endl;
	if(j_pt < 1) 
	  continue;
		
	// coming up with a list of nearby gen-level cands to any given gen-level cand, I think
	std::vector<unsigned int>  refNearReference;	
	bool higherRefPresent = false;
	//				for(  edm::View<reco::Candidate> ::const_iterator refCand = genEGamma->begin() ;   refCand != genEGamma->end(); ++refCand ){
	for(unsigned int refCand=0;refCand<genEGamma->size();++refCand) {  // nested loop through gen cands again
	  if(genEGamma->at(refCand).pt() == j_pt && ROOT::Math::VectorUtil::DeltaR(genEGamma->at(refCand).p4(),j_p4) == 0)  {
	    // then they're the same object -- can't you just look at their addresses or something?
	    continue;
	  }
	  //					if(refCand->pt() > j-pt()  && 
	  if(genEGamma->at(refCand).pt() > 1){		
	    double deltaR = ROOT::Math::VectorUtil::DeltaR(genEGamma->at(refCand).p4(),j_p4);
	    if( deltaR < matchDR_) {
	      refNearReference.push_back(refCand);				
	      if(genEGamma->at(refCand).pt() > j_pt)
		higherRefPresent = true;								
	    }
	  }
	}
	if(higherRefPresent) // only take those gen cands that have no higher nearby neighbors (defined by matchDR_).  why?
	  continue;
		
		
	refPt->Fill(j_pt);
	refEt->Fill(j_et);
	refEta->Fill(j_eta);
	refPhi->Fill(j_phi);
	//		std::cout << "filling reference" << std::endl;
	if(fabs(j_phi) < barrelBoundary_)
	  refPtBarrel->Fill(j_pt);
	else if(fabs(j_phi) > endcapBoundary_)
	  refPtEndcap->Fill(j_pt);
	
	if(j_pt>thresholdForEtaPhi_)
	  refEtaPhi->Fill(j_eta,j_phi);
	//		else
	//			continue;
		
	//TPG Efficiency
	//get TPG Sum
	double tpgS = tpgSum(j_p4.Vect(),*ecalTPGs,tpgSumWindow_);//,caloGeom,eS,tpgSumWindow_); // how does he figure out what tpgSumWindow to use??  it's not going to correspond exactly to a L1 candidate anyhow, because those work with things in x,y-space, not r-space
	//If there is cosignificant ecal energy over threshold passes: // what does "cosignificant" mean?
	if(tpgS>=egammaThreshold_)
	  {
	    tpgEffPt->Fill(j_pt);
	    tpgEffEt->Fill(j_et);
	    tpgEffEta->Fill(j_eta);
	    tpgEffPhi->Fill(j_phi);
	    if(j_pt>thresholdForEtaPhi_)
	      tpgEffEtaPhi->Fill(j_eta,j_phi);
	  }
	//		std::cout << "testing2 " << std::endl;
	if(tpgS<emS->et(63)) // max energy
	  TPG_Resolution->Fill((tpgS-j_pt)/j_pt);

	//get RCT Efficiency-Find the highest object near the ref
	L1GctEmCandCollection rctNearReference;
	/*
	  std::vector<unsigned int>  refNearReference;	
	  //		for(  edm::View<reco::Candidate> ::const_iterator refCand = genEGamma->begin() ;   refCand != genEGamma->end(); ++refCand ){
	  for(unsigned int refCand=0;refCand<genEGamma->size();++refCand) { 
	  if(genEGamma->at(refCand).pt()  == j_pt && ROOT::Math::VectorUtil::DeltaR(genEGamma->at(refCand).p4(),j_p4) == 0)  continue;
	  if(genEGamma->at(refCand).pt() > 1){		
	  double deltaR = ROOT::Math::VectorUtil::DeltaR(genEGamma->at(refCand).p4(),j_p4);
	  if( deltaR < matchDR_)
	  refNearReference.push_back(refCand);
	  }
	  }
	*/
	if(rctEGammas.size()>0)
	  for(L1GctEmCandCollection::const_iterator i=rctEGammas.begin();i!=rctEGammas.end();++i)
	    {
	      double i_et = emS->et(i->rank());
	      if(i_et>=egammaThreshold_)
		{
		  //create lorentz vector
		  math::PtEtaPhiMLorentzVector rctVec = rctLorentzVector(*i);//,caloGeom),emS);
		  double deltaR = ROOT::Math::VectorUtil::DeltaR(rctVec,j_p4);
		  //ok now match and do it over threshold
		  if( deltaR < matchDR_)// && rctVec.pt() >= egammaThreshold_ )
		    rctNearReference.push_back(*i);
		}	     
	    }
	if(rctNearReference.size()==0)
	  //	  std::cout << "no L1 egamma found"
// 		    << " for ref: Et = " << j_et 
// 		    << "  eta = " << j_eta
// 		    << "  phi = " << j_phi
//		    << std::endl;
	if(rctNearReference.size()>0) 
	  {
	    //		  std::cout << "testing 3a" << std::endl;
	    //Ok find the highest matched rct cand
	    RCTEmSorter sorter;
	    RCTEmEnergySorter esorter(*j, emS); // j is our ref cand
	    std::sort(rctNearReference.begin(),rctNearReference.end(),sorter); // sort RCT cands near reference -- BUT!!!  THIS DOES NOT WORK THE WAY JONATHAN THINKS IT DOES!!  Cand 0 is the LOWEST!
	    std::reverse(rctNearReference.begin(),rctNearReference.end()); // MAKE CAND 0 THE HIGHEST.
	    
// 	    std::cout << "Sorted RCT cands" << std::endl;
// 	    for (unsigned int count = 0; count < rctNearReference.size(); count++)
// 	      {
// 		std::cout << "cand " << count << ": Et = " << emS->et(rctNearReference.at(count).rank()) << std::endl;
// 	      }
	    
	    L1GctEmCand highestRCT = rctNearReference.at(0); // get highest one -- what if it's actually from another higher-et ref?  nope, we already took care of that above, we're only dealing with highest of all nearby refs
	    unsigned int highestRCT_rank = highestRCT.rank();
	    double highestRCT_et = emS->et(highestRCT_rank);

	    math::PtEtaPhiMLorentzVector highestVec = rctLorentzVector(highestRCT);//,caloGeom,emS);

	    //			  		  std::cout << "highest RCT found rank" << highestRCT << std::endl;
	    // this all looks familiar... ah, previously it was if we didn't require matching of l1 objects, now we are.
	    if(  matchL1Objects_){
	      EcalTrigPrimDigiCollection* l1TrigTowers = new EcalTrigPrimDigiCollection();
	      findRegionTowers(highestRCT,*ecalTPGs,l1TrigTowers);

	      EcalTriggerPrimitiveDigi highestEtTT;
	      int nTowers =0 ;
	      double totalEnergy = 0., highestEtTTet =-1.,highestEtTTHCAL= 0.;
	      bool centralFG = false, centralSpike = false;
	      for(EcalTrigPrimDigiCollection::const_iterator iTT = l1TrigTowers->begin() ; iTT != l1TrigTowers->end(); ++iTT){
		//	
		int iTT_ieta = iTT->id().ieta();
		double et = eS->et(iTT->compressedEt(),abs(iTT_ieta),iTT_ieta/abs(iTT_ieta));	
		//			  std::cout << "loop, trig tower  compressed et " << iTT->compressedEt(); //<<std::endl;	
		totalEnergy += et;
		if(et> 0)
		  ++nTowers;
		if(et > highestEtTTet ) {
		  highestEtTT = *iTT;
		  highestEtTTet =et;
		  centralFG = iTT->fineGrain()&&  et< 30;
		  centralSpike = (iTT->l1aSpike()==0 );
		} // end if et > highestEtTTet
	      } // end loop through ECAL towers
	      //	  std::cout << " high trig tower et " << highestEtTTet <<std::endl;
	      if(!centralSpike && highestRCT_et >=egammaThreshold_) {// don't look at spike events or fineGrain events // ahhhh, this time we also require the highest RCT Et to be over threshold
	  
		rctPtNoSpike->Fill(highestRCT_et);
	  
 		int ieta = highestEtTT.id().ieta();
 		int iphi = highestEtTT.id().iphi();
		double energyMaxFGcorner = -1;
		//    std::cout<< " new part begining";
		std::vector<double> tpgs3x3 =    find3x3Towers(ieta, iphi, *ecalTPGs, *hcalTPGs);
		//    std::cout << " after finding 3x3 towers size " << tpgs3x3.size() <<std::endl;
		double ecal3x3Tot =0.;
		double hcal3x3Tot =0.;
		double highestNeighborEcal = -1;
		double minELsum = 9999;
		double highestHEneighbor = 0;
		for(int i = 0; i< 9; ++i){

		  double ecalTTEt= tpgs3x3.at(i);
		  double hcalTTEt = tpgs3x3.at(i+9);
	    
		  if(hcalTTEt>0 && ecalTTEt < 30){
		    dirHCALclosest->Fill((i%3)-1,(i/3)-1);
		    dirHCALclosestWeighted->Fill((i%3)-1,(i/3)-1, hcalTTEt);
		    if(i != 4) { // this time we're omitting center tower
		      if(ecalTTEt >2){  // minimum ecal energy for he cut
			if(highestHEneighbor < (hcalTTEt/ecalTTEt ))
			  highestHEneighbor = hcalTTEt/ecalTTEt ;	      
		      } else{ 
			if(hcalTTEt >2) // above noise threshold
			  highestHEneighbor = .99;
		      }
		    }
		  }
		  ecal3x3Tot += ecalTTEt;
		  hcal3x3Tot += hcalTTEt;
		  if( i%2 ==1  )  { // find neighbors of ecal
		    if(ecalTTEt > highestNeighborEcal)
		      highestNeighborEcal = ecalTTEt;
		  }

		  if(i%2 ==0 && i!=4){ 
		    double tempELsum = 0;
		    int cornerEta = i/3;
		    int cornerPhi = i%3;
		    for(int j=0; j<3; ++j){
		      if(tempELsum <  tpgs3x3.at(cornerEta*3 + j))
			tempELsum =   tpgs3x3.at(cornerEta*3 + j);
		      if(tempELsum <  tpgs3x3.at(j*3 + cornerPhi))
			tempELsum = tpgs3x3.at(j*3 + cornerPhi);
		    }
		    if(tempELsum < minELsum)
		      minELsum = tempELsum;
		  }
		  if(i==4)
		    highestEtTTHCAL = hcalTTEt;
		  else
		    if(tpgs3x3.at(i+18 ) == 1)
		      energyMaxFGcorner = ecalTTEt;	      
		}
	    
	  
		double rctHe =highestEtTTHCAL/highestEtTTet;
		rctHE->Fill(rctHe);
		rctHEvL1Et->Fill(highestRCT_et,rctHe);
	    
		rctHEvECALEt->Fill(highestEtTTet,rctHe);
		rctHEvHCALEt->Fill(highestEtTTHCAL,rctHe);
		HCALtpgPt->Fill(highestEtTTHCAL);
		tpgECALsecondtower->Fill(highestNeighborEcal);
		tpgHCALSurrounding->Fill(hcal3x3Tot);
		tpgECALSurrounding->Fill(ecal3x3Tot-highestEtTTet);
		regionSum->Fill(hcal3x3Tot+ecal3x3Tot);
		regionHE->Fill(hcal3x3Tot/ecal3x3Tot);
		diffSumEgamma->Fill(ecal3x3Tot-highestEtTTet - highestNeighborEcal);
	    
		regionMaxHE->Fill(highestHEneighbor);
		if(centralFG) {
		  rctFGMainTowerEt->Fill(highestEtTTet);
		} else {
		  rctPtEGFGcut->Fill(highestRCT_et);
		  rctNoFGMainTowerEt->Fill(highestEtTTet);
		  rctHEafterFG->Fill(rctHe);
		  if(rctHe < HEcut_){
		    rctPtEGHEcut->Fill(highestRCT_et);
	      
		    if(energyMaxFGcorner > 0) 
		      rctFGneighborEt->Fill(energyMaxFGcorner);	      
		    else{
		      minLSum->Fill(minELsum);
		      regionMaxHEafterFG->Fill(highestHEneighbor);
		      if(highestHEneighbor < HEcut_) {
			rctPtIsoEGHEFGcut->Fill(highestRCT_et);
			minLSumHE->Fill(minELsum);
			if(minELsum < isolation_)
			  rctPtIsoEGHEFGIsocut->Fill(highestRCT_et);
		      }
		    }
		  }
		}
	      } // end of if !central spike and highest RCT over threshold
	    } // end of if match l1 objects	
      
	    // still inside if rctNearReference.size() > 0 block so still dealing with highest RCT -- are we double-counting some of these things??  the same plots are being filled again below! no, maybe we're not -- this looks like HCAL stuff instead

	    // find the towers of highest vec

	    // and again!  sort of
	    EcalTrigPrimDigiCollection* l1TrigTowers = new EcalTrigPrimDigiCollection();
	
	    findRegionTowers(highestRCT,*ecalTPGs,l1TrigTowers); // again get towers for region of highest RCT cand.  highest should always correspond to highest RCT cand (excepting TPLF situations), but I guess you still need to go through searching these guys for the highest because there's no automatic correspondence between cands and TTs

	    // get our highest-et (ECAL) trigger tower as usual
	    EcalTriggerPrimitiveDigi highestEtTT;
	    int nTowers =0 ;
	    double totalEnergy = 0., highestEtTTet =-1.,highestEtTTHCAL= 0.;
	    for(EcalTrigPrimDigiCollection::const_iterator iTT = l1TrigTowers->begin() ; iTT != l1TrigTowers->end(); ++iTT){
	      //			  std::cout << "loop, high trig tower et " << highestEtTTet <<std::endl;
	      int iTT_ieta = iTT->id().ieta();
	      double et = eS->et(iTT->compressedEt(),abs(iTT_ieta),iTT_ieta/abs(iTT_ieta));	
	      //			  std::cout << "loop, trig tower  compressed et " << iTT->compressedEt(); //<<std::endl;	
	      totalEnergy += et;
	      if(et> 0)
		++nTowers;
	      if(et > highestEtTTet ) {
		highestEtTT = *iTT;
		highestEtTTet =et;
	      }
	    }
	    int ieta = highestEtTT.id().ieta();
	    int iphi = highestEtTT.id().iphi();
	
	    HcalTrigTowerDetId ttDetId(ieta,iphi); // now we start doing something different -- HCAL! get HCAL trig tower corresponding to our highest-Et ECAL one
	
	    HcalTrigPrimDigiCollection::const_iterator hcalTT = hcalTPGs->find(ttDetId); // find it in our collection of HCAL tpgs
	    if(hcalTT->id() == ttDetId) // if TP is present in collection, hcalTT is the end piece // <-- ??  I guess only TPs with significant energy are in collection?
	      highestEtTTHCAL = hS->et(hcalTT->SOI_compressedEt(),ieta,ttDetId.zside());
	    else //
	      highestEtTTHCAL = 0; // if it's not there, it has no energy

	
	    //			if(highestEtTTHCAL ==-1)  { //find out this sucker
	    //			  std::cout << "- energy hcal found ! compressed et " <<hcalTT->SOI_compressedEt()  << " ieta " << HCALieta << " zside ? " << ttDetId.zside() <<std::endl;
	    //			}
	    //			  std::cout << " bad find: number of HcalTPGs is " << hcalTPGs->size() << std::endl;
	    //			int HCALnewIeta = hcalTT->id().ieta();
	    //			int HCALnewIphi = hcalTT->id().iphi();
	    //			std::cout<< " trig tower ieta " << HCALieta << " trig tower iphi " <<iTT->id().iphi()<< "hcal tower id " << ttDetId.rawId() << std::endl << " translated hcal ieta " << HCALnewIeta << "translated hcal iphi " << HCALnewIphi << std::endl;


	    if(highestRCT_et>=egammaThreshold_) {
		       
	      if(nTowers>0){ // if there is at least one tower with energy, which there must be if there's a cand there
		regionEGHighTowerEt->Fill(highestEtTTet);
		regionEGtowersRatio->Fill(highestEtTTet/totalEnergy); // rises to 1
	      }
	      regionEGtowers->Fill(nTowers); // actual peak at 2, tail up to 8ish
	      if(j_pt < egammaThreshold_){ // if our reference is below the EG threshold
				  
		std::cout << "low object above thresh " << highestVec << " high resolution RCT info" << " elec  info ET" << j_pt << " eta " << j_eta<< " phi " << j_phi <<std::endl;
		//			    std::cout << " all rct objects near " << std::endl;
		for(L1GctEmCandCollection::const_iterator ll = rctNearReference.begin(); ll != rctNearReference.end() ; ++ll){
		  std::cout << *ll  << std::endl;
		}
		//				  std::cout << " all ref objects near " << std::endl;
		for(unsigned int lk = 0; lk < refNearReference.size() ; ++lk)
		  std::cout << " elec  info ET" << genEGamma->at(lk).pt() << " eta " << genEGamma->at(lk).eta()<< " phi " << genEGamma->at(lk).phi() << std::endl;
	      }

	      //			  std::cout << " h/e is " << rctHe << " ecal energy " << highestEtTTet << " hcal energy " << highestEtTTHCAL << std::endl ;

	      rctEffPt->Fill(j_pt); // I'm still afraid we're double-counting... no, the previous stuff was refPt etc
	      rctEffEt->Fill(j_et);
	      rctEffEta->Fill(j_eta);
	      rctEffPhi->Fill(j_phi);
	      
	      if(fabs(j_phi) < barrelBoundary_)
		rctEffPtBarrel->Fill(j_pt);
	      else if(fabs(j_phi) > endcapBoundary_)
		rctEffPtEndcap->Fill(j_pt);
	      
	      if(j_pt>thresholdForEtaPhi_)
		rctEffEtaPhi->Fill(j_eta,j_phi);
	      
	      if(j_pt>=egammaThreshold_ && highestRCT_rank<63 && (j_pt < (0.8 * emS->et(63)))){  // only for non-saturating
		RCT_Resolution->Fill(( highestRCT_et-j_pt)/j_pt );
		RCT_IetaResolution[abs(ieta) -1]->Fill(( highestRCT_et-j_pt)/j_pt );
		if( (highestRCT_et-j_pt)/j_pt >1) {
		  continue;
		  //			  std:cout  << " high resolution RCT info"  << highestRCT << " elec  info ET" << j_pt << " eta " << j_eta<< " phi " << j_phi <<std::endl;
		}
		
	      }
	      //			std::cout << "testing 5" << std::endl;
	      if(highestRCT.isolated()) {
		
		rctIsoEffPt->Fill(j_pt);
		rctIsoEffEt->Fill(j_et);
		rctIsoEffEta->Fill(j_eta);
		rctIsoEffPhi->Fill(j_phi);
		if(j_pt>thresholdForEtaPhi_)
		  rctIsoEffEtaPhi->Fill(j_eta,j_phi);
		if(fabs(j_phi) < barrelBoundary_)
		  rctIsoEffPtBarrel->Fill(j_pt);
		else if(fabs(j_phi) > endcapBoundary_)
		  rctIsoEffPtEndcap->Fill(j_pt);
		
		
		// this is all still inside if highestRCT is isolated -- should it be???
		if( j_pt>=egammaThreshold_ && highestRCT_rank <63 && (j_pt < (1 * emS->et(63) )))  // only for non-saturating
		  {
		    
		    // HERE's finally where we fill all the correction factors

		    double rctEt = highestRCT_et; // why do we keep doing this calculation?

// 		    std::cout << "FILLING CORRECTION FACTORS!" << std::endl;
// 		    std::cout << "j_pt = " << j_pt
// 			      << "  highest Et = " << rctEt 
// 			      << "  highest iEta = " << ieta
// 			      << "  nTowers = " << nTowers
// 			      << "  egammaThreshold = " << egammaThreshold_
// 			      << std::endl;

		    rctEtaCorr->Fill( highestVec.eta(), j_pt/highestVec.pt() ); // okey-dokey, ratio of ref to rct cand.  why need highestVec?? doesn't it already exist in some other form?  like the Et?
		    rctEtaCorrIEta->Fill( ieta, j_pt/rctEt );
		    rctEtaCorrAbsIEta->Fill( fabs(ieta), j_pt/rctEt );
		    if(nTowers ==1 || (nTowers==2 && ieta >=27)){ // so it's NOT just single-tower cands!! well, above 27... why 27??  almost HF, but not quite!  and not even the abs of ieta!?!
		      rctEtaCorr1Tower->Fill( highestVec.eta(), j_pt/rctEt );
		      rctEtaCorrIEta1Tower->Fill( ieta, j_pt/rctEt );
		      rctEtaCorrAbsIEta1Tower->Fill( fabs(ieta), j_pt/rctEt );
		      rctEtaCorr1TowerEt->Fill( highestVec.eta(), j_et/rctEt );
		      rctEtaCorrIEta1TowerEt->Fill( ieta, j_et/rctEt );
		      rctEtaCorrAbsIEta1TowerEt->Fill( fabs(ieta), j_et/rctEt );
		      
		    }
		    if(nTowers ==2){
		      rctEtaCorr12Tower->Fill( highestVec.eta(), j_pt/rctEt ); // wait, this isn't 1 or 2 towers, it's 2 towers ONLY...
		      rctEtaCorrIEta12Tower->Fill( ieta, j_pt/rctEt );
		      rctEtaCorrAbsIEta12Tower->Fill( fabs(ieta), j_pt/rctEt );
		      rctEtaCorr12TowerEt->Fill( highestVec.eta(), j_et/rctEt );
		      rctEtaCorrIEta12TowerEt->Fill( ieta, j_et/rctEt );
		      rctEtaCorrAbsIEta12TowerEt->Fill( fabs(ieta), j_et/rctEt );
		      
		    }
		  }
	      } // end if highestRCT isolated
	      delete l1TrigTowers;
	    } // end of if highestRCT Et > threshold
	  } // end of if rctNearReference.size() > 0
	//		std::cout << "testing 6" << std::endl;
	
	//Now THE GCT Efficiency
	
	L1GctEmCandCollection gctNearReference;
	
	if(gctEGammas.size()>0)
	  for(L1GctEmCandCollection::const_iterator i=gctEGammas.begin();i!=gctEGammas.end();++i)
	    {
	      double i_et = emS->et(i->rank());
	      //if(i->rank()>0)
	      if(i_et>0)
		{
		  //create lorentz vector
		  math::PtEtaPhiMLorentzVector gctVec = rctLorentzVector(*i);//,caloGeom,emS);
		  double deltaR = ROOT::Math::VectorUtil::DeltaR(gctVec,j_p4);
		  //ok now match and do it over 10 GeV
		  if(deltaR<matchDR_&&gctVec.pt()>=egammaThreshold_) // why suddenly >= instead of > ?
		    {
		      gctNearReference.push_back(*i);
		    }
		}
	    }
	
	if(gctNearReference.size()>0) 
	  {
	    //Ok find the highest matched gct cand
	    RCTEmSorter gsorter;
	    
	    std::sort(gctNearReference.begin(),gctNearReference.end(),gsorter);
	    L1GctEmCand highestGCT = gctNearReference.at(0);
	    //get its LV
	    math::PtEtaPhiMLorentzVector highestVec = rctLorentzVector(highestGCT);//,caloGeom,emS);
	    //ask for threshold
	    if(highestVec.pt()>=egammaThreshold_) 
	      {
		gctEffPt->Fill(j_pt);
		gctEffEt->Fill(j_et);
		gctEffEta->Fill(j_eta);
		gctEffPhi->Fill(j_phi);
		if(j_pt>thresholdForEtaPhi_)
		  gctEffEtaPhi->Fill(j_eta,j_phi);
	      }
	  }
	
	
	L1GctEmCandCollection gctIsoNearReference;
	
	if(gctIsoEGammas.size()>0)
	  for(L1GctEmCandCollection::const_iterator i=gctIsoEGammas.begin();i!=gctIsoEGammas.end();++i){
	    double i_et = emS->et(i->rank());
	    //if(i->rank()>0)
	    if(i_et>0)
	      {
		//create lorentz vector
		math::PtEtaPhiMLorentzVector gctIsoVec = rctLorentzVector(*i);//,caloGeom,emS);
		double deltaR = ROOT::Math::VectorUtil::DeltaR(gctIsoVec,j_p4);
		//ok now match and do it over 10 GeV
		if(deltaR<matchDR_&&gctIsoVec.pt()>=egammaThreshold_)
		  {
		    gctIsoNearReference.push_back(*i);
		  }
	      }
	  }	

	if(gctIsoNearReference.size()>0) 
	  {
	    //Ok find the highest matched gctIso cand
	    RCTEmSorter ggsorter;
	    std::sort(gctIsoNearReference.begin(),gctIsoNearReference.end(),ggsorter);
	    L1GctEmCand highestGCTISO = gctIsoNearReference.at(0);
	    //get its LV
	    math::PtEtaPhiMLorentzVector highestVec = rctLorentzVector(highestGCTISO);//,caloGeom,emS);
	    //ask for threshold
	    if(highestVec.pt()>=egammaThreshold_) 
	      {
		gctEffPt->Fill(j_pt);
		gctEffEt->Fill(j_et);
		gctEffEta->Fill(j_eta);
		gctEffPhi->Fill(j_phi);
		if(j_pt>thresholdForEtaPhi_)
		  gctEffEtaPhi->Fill(j_eta,j_phi);
		
		gctIsoEffPt->Fill(j_pt);
		gctIsoEffEt->Fill(j_et);
		gctIsoEffEta->Fill(j_eta);
		gctIsoEffPhi->Fill(j_phi);
		if(j_pt>thresholdForEtaPhi_)
		  gctIsoEffEtaPhi->Fill(j_eta,j_phi);
	      }
	  }
      }
  }
  //	std::cout << " reference pt nentries at end of event " << refPt->getEntries() << std::endl;
}




math::PtEtaPhiMLorentzVector
RctValidation::rctLorentzVector(const L1GctEmCand& cand)//,const L1CaloGeometry*geom ,const  L1CaloEtScale* scale)
{
  double eta = caloGeom->etaBinCenter( cand.etaIndex(),true) ;
  double phi = caloGeom->emJetPhiBinCenter( cand.phiIndex() ) ;
  double et = emS->et( cand.rank() ) ;
  et+=1e-6;//correction !

  return   math::PtEtaPhiMLorentzVector(et,eta,phi,0); 
}



double 
RctValidation::tpgSum(const math::XYZVector& direction,const EcalTrigPrimDigiCollection& ecalDigis,double deltaR)//const L1CaloGeometry* geom ,const  L1CaloEcalScale* scale,double deltaR)
{
  double tpgSum=0;
  for(EcalTrigPrimDigiCollection::const_iterator i=ecalDigis.begin();i!=ecalDigis.end();++i) {
    //calculate the TPG in eta phi space
    int ieta = i->id().ieta();
    int iphi = i->id().iphi();
//     double eta = geo->eta(i->id().ieta());
//     double phi = geo->phi(i->id().iphi());
    double eta = geo->eta(ieta);
    double phi = geo->phi(iphi);

    //    double et = eS->et(i->compressedEt(),abs(i->id().ieta()),i->id().ieta()/abs(i->id().ieta()));
    double et = eS->et(i->compressedEt(),abs(ieta),ieta/abs(ieta));

    math::PtEtaPhiMLorentzVector v(et,eta,phi,0.0);

    if(ROOT::Math::VectorUtil::DeltaR(v.Vect(),direction)<deltaR) {
      tpgSum+=et;
    }
  }

//  printf("TPG SUM =%f\n",tpgSum);
  return tpgSum;
}

void
RctValidation::findRegionTowers(const L1GctEmCand& cand,const EcalTrigPrimDigiCollection& ecalDigis, EcalTrigPrimDigiCollection* regionTPs)//,const L1CaloGeometry* geom ,const  L1CaloEcalScale* scale)
{
  //	EcalTrigPrimDigiCollection regionTPs;
  //	std::cout << "finding regions initialization" <<std::endl;
  L1CaloRegionDetId caloDetId = cand.regionId();
  
  
  unsigned crate  = caloDetId.rctCrate();
  unsigned card	= caloDetId.rctCard();
  unsigned region = caloDetId.rctRegion();
  //std::cout << "finding objects in highest RCT crate " << crate << " card " << card << " region " << region ;
  unsigned short iTow0 = 0;
  unsigned short iTow16 = 15;
  if(region == 1) {
    iTow0 = 16;
    iTow16 = 31;
  }
  short rctEtaMin = rctGeo->calcIEta(crate,card, iTow0);
  short rctEtaMax = rctGeo->calcIEta(crate,card, iTow16);
  
  
  unsigned short rctPhiMin = rctGeo->calcIPhi(crate,card, iTow0);
  unsigned short rctPhiMax = rctGeo->calcIPhi(crate,card, iTow16);
  
  if(rctPhiMax < rctPhiMin) {	// switch the two
    unsigned short tempPhi = rctPhiMax;
    rctPhiMax = rctPhiMin;
    rctPhiMin = tempPhi;
  }
  if(rctEtaMax < rctEtaMin) {	// switch the two
    short tempEta = rctEtaMax;
    rctEtaMax = rctEtaMin;
    rctEtaMin = tempEta;
  }
  
  //	std::cout << " ieta range " << rctEtaMin  <<" - " << rctEtaMax << " iphi range " << rctPhiMin << " - " << rctPhiMax << std::endl;
  
  for(EcalTrigPrimDigiCollection::const_iterator i=ecalDigis.begin();i!=ecalDigis.end() ;++i) {	
    
    short ieta = (short) i->id().ieta(); 
    // Note absIeta counts from 1-28 (not 0-27)
    
    if(ieta >= rctEtaMin && ieta <= rctEtaMax) {
      unsigned short cal_iphi = (unsigned short) i->id().iphi(); 
      unsigned short iphi = (72 + 18 - cal_iphi) % 72; // transform TOWERS (not regions) into local rct (intuitive) phi bins
      if(iphi >= rctPhiMin && iphi <= rctPhiMax){
	//std::cout << "finding regions initialization loop" <<std::endl <<std::flush;
	regionTPs->push_back(*i);
	//std::cout << "finding regions pushed back loop region size " << regionTPs->size() <<std::endl <<std::flush;
	//std::cout << "tt inside rct region " << *i << std::endl;
      }
    }
    if( regionTPs->size() >= 16)
      break;
  }
  //		std::cout << " at end of finding regions " << std::cout <<std::flush;
  
  
  return ;
  
}

std::vector<double>
RctValidation::find3x3Towers(int etaCentral, int phiCentral ,const EcalTrigPrimDigiCollection& ecalTPGs,const HcalTrigPrimDigiCollection& hcalTPGs)//, std::vector<double> tpgs3x3)//,const L1CaloGeometry* geom ,const  L1CaloEcalScale* scale)
{
  std::vector<double> tpgs3x3(27);
  for(int k =0; k< 27; ++k)
    tpgs3x3[k] = 0;
  //  std::cout << " in 3x3 finding eta " << etaCentral << " phi " << phiCentral <<std::endl;
  for(int i = 0; i <= 2 ;++i){ //eta iterator
    int ieta = etaCentral + i -1;
    if(abs(ieta) > 28) // out of eta range
      continue;
    if(ieta == 0)
      ieta = -1*etaCentral;
    for(int j = 0; j<= 2; ++j){ //phi iterator


      int iphi = (phiCentral +j+72 -1) %72;
      if(iphi == 0) iphi = 72;
      //        std::cout << " in tower loop eta " << ieta << " phi " << iphi <<std::endl;
      HcalTrigTowerDetId ttDetId(ieta,iphi);
      HcalTrigPrimDigiCollection::const_iterator hcalTT = hcalTPGs.find(ttDetId);
      if(hcalTT->id() == ttDetId) // if TP is present in collection, hcalTT is the end piece
	tpgs3x3[i*3+j+9] = hS->et(hcalTT->SOI_compressedEt(),abs(ieta),ieta/abs(ieta));//hcalTT->SOI_compressedEt();
	//highestEtTTHCAL = 
      EcalSubdetector subdet = ( abs(ieta) <= 17  ) ? EcalBarrel : EcalEndcap ;
      EcalTrigTowerDetId ttEDetId(ieta/abs(ieta), subdet, abs(ieta), iphi);
      //      EcalTrigTowerDetId ttEDetId(ieta,iphi);
      EcalTrigPrimDigiCollection::const_iterator ecalTT = ecalTPGs.find(ttEDetId);

      if(ecalTT->id() == ttEDetId){ // if TP is present in collection, hcalTT is the end piece
	tpgs3x3[i*3+j] =eS->et(ecalTT->compressedEt(),abs(ieta),ieta/abs(ieta));// ecalTT->compressedEt();
	if(ecalTT->fineGrain() && eS->et(ecalTT->compressedEt(),abs(ieta),ieta/abs(ieta)) <30)
	  tpgs3x3[i*3+j+18] = 1;
      }
	
	//      else //
	//	highestEtTTHCAL = 0;
    }
  }


  //    std::cout << " after finding 3x3 towers size " << tpgs3x3.size() <<std::endl;
  return tpgs3x3;
}


//--------------------------------------------------------
void RctValidation::endLuminosityBlock(const LuminosityBlock& lumiSeg, 
				    const EventSetup& context) {
}
//--------------------------------------------------------
void RctValidation::endRun(const Run& r, const EventSetup& context){
	
  //		std::cout << " reference pt nentries at end of  run " << refPt->getEntries() << std::endl;
}
//--------------------------------------------------------
void RctValidation::endJob(){

//   // divide the histograms to make efficiencies
//   refPtEff = refPtNum->Divide(refPtDen);
  TH1F * h_refPtNum = refPtNum->getTH1F();
  TH1F * h_refPtDen = refPtDen->getTH1F();
  refPtEff->getTH1F()->Divide(h_refPtNum,h_refPtDen,1.,1.,"B"); // ideally use TGraphAsymmErrors::Divide() BUT MonitorElement is TH1F...?  Can do a TGAE ME? nope, it doesn't look like it.  
  TH1F * h_refPtIsoNum = refPtIsoNum->getTH1F();
  TH1F * h_refPtIsoDen = refPtIsoDen->getTH1F();
  refPtIsoEff->getTH1F()->Divide(h_refPtIsoNum,h_refPtIsoDen,1.,1.,"B"); // ideally use TGraphAsymmErrors::Divide() BUT MonitorElement is TH1F...?  Can do a TGAE ME? nope, it doesn't look like it.  

  if (  outfile_.size() != 0  &&store ) store->save(outfile_);
  return;
}



DEFINE_FWK_MODULE(RctValidation);





