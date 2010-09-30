#include "UserCode/TrigTree/interface/HltTree.h"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "DataFormats/HLTReco/interface/TriggerObject.h"
#include "FWCore/Common/interface/TriggerNames.h" 

HltTree::HltTree(std::string name, edm::InputTag trigTag, edm::InputTag trigEventTag, TTree* tree)
{
  trigTag_=trigTag;
  trigEventTag_=trigEventTag;
  name_=name;
  tree_=tree;

  tree_->Branch("trigResults",&trigResult_);
  tree_->Branch("trigName",&trigName_);
  
  tree_->Branch("hltEle10LWEt",      &hltEle10LWEt_);
  tree_->Branch("hltEle10LWPt",      &hltEle10LWPt_);
  tree_->Branch("hltEle10LWEta",     &hltEle10LWEta_);
  tree_->Branch("hltEle10LWPhi",     &hltEle10LWPhi_);

  tree_->Branch("hltEle15LWEt",      &hltEle15LWEt_);
  tree_->Branch("hltEle15LWPt",      &hltEle15LWPt_);
  tree_->Branch("hltEle15LWEta",     &hltEle15LWEta_);
  tree_->Branch("hltEle15LWPhi",     &hltEle15LWPhi_);

  tree_->Branch("hltEle20LWEt",      &hltEle20LWEt_);
  tree_->Branch("hltEle20LWPt",      &hltEle20LWPt_);
  tree_->Branch("hltEle20LWEta",     &hltEle20LWEta_);
  tree_->Branch("hltEle20LWPhi",     &hltEle20LWPhi_);

  tree_->Branch("hltEle10SWEt",      &hltEle10SWEt_);
  tree_->Branch("hltEle10SWPt",      &hltEle10SWPt_);
  tree_->Branch("hltEle10SWEta",     &hltEle10SWEta_);
  tree_->Branch("hltEle10SWPhi",     &hltEle10SWPhi_);

  tree_->Branch("hltEle15SWEt",      &hltEle15SWEt_);
  tree_->Branch("hltEle15SWPt",      &hltEle15SWPt_);
  tree_->Branch("hltEle15SWEta",     &hltEle15SWEta_);
  tree_->Branch("hltEle15SWPhi",     &hltEle15SWPhi_);

  tree_->Branch("hltEle20SWEt",      &hltEle20SWEt_);
  tree_->Branch("hltEle20SWPt",      &hltEle20SWPt_);
  tree_->Branch("hltEle20SWEta",     &hltEle20SWEta_);
  tree_->Branch("hltEle20SWPhi",     &hltEle20SWPhi_);

  tree_->Branch("hltEle25SWEt",      &hltEle25SWEt_);
  tree_->Branch("hltEle25SWPt",      &hltEle25SWPt_);
  tree_->Branch("hltEle25SWEta",     &hltEle25SWEta_);
  tree_->Branch("hltEle25SWPhi",     &hltEle25SWPhi_);

  tree_->Branch("hltPhoton10Et",     &hltPhoton10Et_);
  tree_->Branch("hltPhoton10Pt",     &hltPhoton10Pt_);
  tree_->Branch("hltPhoton10Eta",    &hltPhoton10Eta_);
  tree_->Branch("hltPhoton10Phi",    &hltPhoton10Phi_);

  tree_->Branch("hltPhoton15Et",     &hltPhoton15Et_);
  tree_->Branch("hltPhoton15Pt",     &hltPhoton15Pt_);
  tree_->Branch("hltPhoton15Eta",    &hltPhoton15Eta_);
  tree_->Branch("hltPhoton15Phi",    &hltPhoton15Phi_);

  tree_->Branch("hltPhoton20Et",     &hltPhoton20Et_);
  tree_->Branch("hltPhoton20Pt",     &hltPhoton20Pt_);
  tree_->Branch("hltPhoton20Eta",    &hltPhoton20Eta_);
  tree_->Branch("hltPhoton20Phi",    &hltPhoton20Phi_);

  tree_->Branch("hltPhoton15CleanEt",     &hltPhoton15CleanEt_);
  tree_->Branch("hltPhoton15CleanPt",     &hltPhoton15CleanPt_);
  tree_->Branch("hltPhoton15CleanEta",    &hltPhoton15CleanEta_);
  tree_->Branch("hltPhoton15CleanPhi",    &hltPhoton15CleanPhi_);

  tree_->Branch("hltPhoton20CleanEt",     &hltPhoton20CleanEt_);
  tree_->Branch("hltPhoton20CleanPt",     &hltPhoton20CleanPt_);
  tree_->Branch("hltPhoton20CleanEta",    &hltPhoton20CleanEta_);
  tree_->Branch("hltPhoton20CleanPhi",    &hltPhoton20CleanPhi_);

  tree_->Branch("hltPhoton30CleanEt",     &hltPhoton30CleanEt_);
  tree_->Branch("hltPhoton30CleanPt",     &hltPhoton30CleanPt_);
  tree_->Branch("hltPhoton30CleanEta",    &hltPhoton30CleanEta_);
  tree_->Branch("hltPhoton30CleanPhi",    &hltPhoton30CleanPhi_);

}

void
HltTree::Fill(const edm::Event& iEvent)
{
  using namespace edm;

  //  std::cout << "Trigger tag is " << trigTag_ << " and trigger event tag is " << trigEventTag_ << std::endl;

  std::string trigProcess(trigTag_.process());
  
  edm::Handle<edm::TriggerResults> trigResults;

  //  edm::InputTag trigTag_("TriggerResults::HLT");
  if (not iEvent.getByLabel(trigTag_, trigResults)) {
    std::cout << ">>> TRIGGER collection does not exist !!!\n";
    return;
  }

  const edm::TriggerNames & trigNames = iEvent.triggerNames(*trigResults);

  for (unsigned int i=0; i<trigResults->size(); i++)
  {
    std::string trigName = trigNames.triggerName(i);

    if ( (trigName.find("_Ele") != std::string::npos) || 
         (trigName.find("_Photon") != std::string::npos) ||
          (trigName.find("Activity_Ecal") != std::string::npos) )
    { 
      trigName_.push_back(trigName);
      int trigResult = trigResults->accept(i);
      trigResult_.push_back(trigResult);
    }
  }
  
  // get HLT candiates
  edm::Handle<trigger::TriggerEvent> trgEvent;
  //  iEvent.getByLabel(InputTag("hltTriggerSummaryAOD","","HLT"), trgEvent);   
  iEvent.getByLabel(trigEventTag_, trgEvent);   
  const trigger::TriggerObjectCollection& TOC(trgEvent->getObjects());   

  // names of filters can be found at
  // https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideEgammaHLT
  // or, if not there (outdated), in
  // http://cmslxr.fnal.gov/lxr/source/HLTrigger/Configuration/python/HLT_FULL_cff.py
  
  // HLT_Ele10_LW_L1R
  //  edm::InputTag myLastFilter("hltL1NonIsoHLTNonIsoSingleElectronLWEt10PixelMatchFilter","","HLT");
  edm::InputTag myLastFilter("hltL1NonIsoHLTNonIsoSingleElectronLWEt10PixelMatchFilter","",trigProcess);
  //  std::cout << "Trigger: HLT_Ele10_LW_L1R" << std::flush;  

  // filterIndex must be less than the size of trgEvent or you get a CMSException: _M_range_check
  if ( trgEvent->filterIndex(myLastFilter) < trgEvent->sizeFilters() ) 
  {
    const trigger::Keys& keys( trgEvent->filterKeys( trgEvent->filterIndex(myLastFilter) ) );
    //    std::cout << " good" << std::endl;

    for ( unsigned int hlto = 0; hlto < keys.size(); hlto++ ) 
    {
      trigger::size_type hltf = keys[hlto];
      const trigger::TriggerObject& L3obj(TOC[hltf]);

      //      std::cout << "Object:  Et " << L3obj.et() << std::endl;
      hltEle10LWEt_.push_back( L3obj.et() );
      hltEle10LWPt_.push_back( L3obj.pt() );
      hltEle10LWEta_.push_back( L3obj.eta() );
      hltEle10LWPhi_.push_back( L3obj.phi() );
    }
  }

  // HLT_Ele15_LW_L1R
  //  myLastFilter = edm::InputTag("hltL1NonIsoHLTNonIsoSingleElectronLWEt15PixelMatchFilter","","HLT");
  myLastFilter = edm::InputTag("hltL1NonIsoHLTNonIsoSingleElectronLWEt15PixelMatchFilter","",trigProcess);
  //  std::cout << "Trigger: HLT_Ele15_LW_L1R" << std::flush;    

  // filterIndex must be less than the size of trgEvent or you get a CMSException: _M_range_check
  if ( trgEvent->filterIndex(myLastFilter) < trgEvent->sizeFilters() ) 
  {
    const trigger::Keys& keys( trgEvent->filterKeys( trgEvent->filterIndex(myLastFilter) ) );
    //    std::cout << " good" << std::endl;

    for ( unsigned int hlto = 0; hlto < keys.size(); hlto++ ) 
    {
      trigger::size_type hltf = keys[hlto];
      const trigger::TriggerObject& L3obj(TOC[hltf]);
      
      //      std::cout << "Object:  Et " << L3obj.et() << std::endl;
      hltEle15LWEt_.push_back( L3obj.et() );
      hltEle15LWPt_.push_back( L3obj.pt() );
      hltEle15LWEta_.push_back( L3obj.eta() );
      hltEle15LWPhi_.push_back( L3obj.phi() );
    }
  }


  // HLT_Ele20_LW_L1R
  //  myLastFilter = edm::InputTag("hltL1NonIsoHLTNonIsoSingleElectronLWEt20PixelMatchFilter","","HLT");
  myLastFilter = edm::InputTag("hltL1NonIsoHLTNonIsoSingleElectronLWEt20PixelMatchFilter","",trigProcess);
  //  std::cout << "Trigger: HLT_Ele20_LW_L1R" << std::flush;

  // filterIndex must be less than the size of trgEvent or you get a CMSException: _M_range_check
  if ( trgEvent->filterIndex(myLastFilter) < trgEvent->sizeFilters() ) 
  {
    const trigger::Keys& keys( trgEvent->filterKeys( trgEvent->filterIndex(myLastFilter) ) );
    //    std::cout << " good" << std::endl;

    for ( unsigned int hlto = 0; hlto < keys.size(); hlto++ ) 
    {
      trigger::size_type hltf = keys[hlto];
      const trigger::TriggerObject& L3obj(TOC[hltf]);
      
      //      std::cout << "Object:  Et " << L3obj.et() << std::endl;
      hltEle20LWEt_.push_back( L3obj.et() );
      hltEle20LWPt_.push_back( L3obj.pt() );
      hltEle20LWEta_.push_back( L3obj.eta() );
      hltEle20LWPhi_.push_back( L3obj.phi() );
    }
  }


  // HLT_Ele10_SW_L1R
  //  myLastFilter = edm::InputTag("hltL1NonIsoHLTNonIsoSingleElectronEt10PixelMatchFilter","","HLT");
  myLastFilter = edm::InputTag("hltL1NonIsoHLTNonIsoSingleElectronEt10PixelMatchFilter","",trigProcess);
  //  std::cout << "Trigger: HLT_Ele10_SW_L1R" << std::flush;
  
  // filterIndex must be less than the size of trgEvent or you get a CMSException: _M_range_check
  if ( trgEvent->filterIndex(myLastFilter) < trgEvent->sizeFilters() ) 
  {
    const trigger::Keys& keys( trgEvent->filterKeys( trgEvent->filterIndex(myLastFilter) ) );
    //    std::cout << " good" << std::endl;

    for ( unsigned int hlto = 0; hlto < keys.size(); hlto++ ) 
    {
      trigger::size_type hltf = keys[hlto];
      const trigger::TriggerObject& L3obj(TOC[hltf]);

      //      std::cout << "Object:  Et " << L3obj.et() << std::endl;      
      hltEle10SWEt_.push_back( L3obj.et() );
      hltEle10SWPt_.push_back( L3obj.pt() );
      hltEle10SWEta_.push_back( L3obj.eta() );
      hltEle10SWPhi_.push_back( L3obj.phi() );
    }
  }

  // HLT_Ele15_SW_L1R
  //  myLastFilter = edm::InputTag("hltL1NonIsoHLTNonIsoSingleElectronEt15PixelMatchFilter","","HLT");
  myLastFilter = edm::InputTag("hltL1NonIsoHLTNonIsoSingleElectronEt15PixelMatchFilter","",trigProcess);
  //  std::cout << "Trigger: HLT_Ele15_SW_L1R" << std::flush;  

  // filterIndex must be less than the size of trgEvent or you get a CMSException: _M_range_check
  if ( trgEvent->filterIndex(myLastFilter) < trgEvent->sizeFilters() ) 
  {
    const trigger::Keys& keys( trgEvent->filterKeys( trgEvent->filterIndex(myLastFilter) ) );
    //    std::cout << " good" << std::endl;

    for ( unsigned int hlto = 0; hlto < keys.size(); hlto++ ) 
    {
      trigger::size_type hltf = keys[hlto];
      const trigger::TriggerObject& L3obj(TOC[hltf]);
      
      //      std::cout << "Object:  Et " << L3obj.et() << std::endl;
      hltEle15SWEt_.push_back( L3obj.et() );
      hltEle15SWPt_.push_back( L3obj.pt() );
      hltEle15SWEta_.push_back( L3obj.eta() );
      hltEle15SWPhi_.push_back( L3obj.phi() );
    }
  }


  // HLT_Ele20_SW_L1R
  //  myLastFilter = edm::InputTag("hltL1NonIsoHLTNonIsoSingleElectronEt20PixelMatchFilter","","HLT");
  myLastFilter = edm::InputTag("hltL1NonIsoHLTNonIsoSingleElectronEt20PixelMatchFilter","",trigProcess);
  //  std::cout << "Trigger: HLT_Ele20_SW_L1R" << std::flush;
  
  // filterIndex must be less than the size of trgEvent or you get a CMSException: _M_range_check
  if ( trgEvent->filterIndex(myLastFilter) < trgEvent->sizeFilters() ) 
  {
    const trigger::Keys& keys( trgEvent->filterKeys( trgEvent->filterIndex(myLastFilter) ) );
    //    std::cout << " good" << std::endl;

    for ( unsigned int hlto = 0; hlto < keys.size(); hlto++ ) 
    {
      trigger::size_type hltf = keys[hlto];
      const trigger::TriggerObject& L3obj(TOC[hltf]);
      
      //      std::cout << "Object:  Et " << L3obj.et() << std::endl;
      hltEle20SWEt_.push_back( L3obj.et() );
      hltEle20SWPt_.push_back( L3obj.pt() );
      hltEle20SWEta_.push_back( L3obj.eta() );
      hltEle20SWPhi_.push_back( L3obj.phi() );
    }
  }


  // HLT_Ele25_SW_L1R
  //  myLastFilter = edm::InputTag("hltL1NonIsoHLTNonIsoSingleElectronEt25PixelMatchFilter","","HLT");
  myLastFilter = edm::InputTag("hltL1NonIsoHLTNonIsoSingleElectronEt25PixelMatchFilter","",trigProcess);
  //  std::cout << "Trigger: HLT_Ele25_SW_L1R" << std::flush;  

  // filterIndex must be less than the size of trgEvent or you get a CMSException: _M_range_check
  if ( trgEvent->filterIndex(myLastFilter) < trgEvent->sizeFilters() ) 
  {
    const trigger::Keys& keys( trgEvent->filterKeys( trgEvent->filterIndex(myLastFilter) ) );
    //    std::cout << " good" << std::endl;

    for ( unsigned int hlto = 0; hlto < keys.size(); hlto++ ) 
    {
      trigger::size_type hltf = keys[hlto];
      const trigger::TriggerObject& L3obj(TOC[hltf]);
      
      //      std::cout << "Object:  Et " << L3obj.et() << std::endl;
      hltEle25SWEt_.push_back( L3obj.et() );
      hltEle25SWPt_.push_back( L3obj.pt() );
      hltEle25SWEta_.push_back( L3obj.eta() );
      hltEle25SWPhi_.push_back( L3obj.phi() );
    }
  }


  // HLT_Photon10_L1R
  //  myLastFilter = edm::InputTag("hltL1NonIsoHLTNonIsoSinglePhotonEt10HcalIsolFilter","","HLT");
  myLastFilter = edm::InputTag("hltL1NonIsoHLTNonIsoSinglePhotonEt10HcalIsolFilter","",trigProcess);
  //  std::cout << "Trigger: HLT_Photon10_L1R" << std::flush;

  if ( trgEvent->filterIndex(myLastFilter) < trgEvent->sizeFilters() ) 
  {
    const trigger::Keys& keys( trgEvent->filterKeys( trgEvent->filterIndex(myLastFilter) ) );
    //    std::cout << " good" << std::endl;

    for ( unsigned int hlto = 0; hlto < keys.size(); hlto++ ) 
    {
      trigger::size_type hltf = keys[hlto];
      const trigger::TriggerObject& L3obj(TOC[hltf]);
      
      //      std::cout << "Object:  Et " << L3obj.et() << std::endl;
      hltPhoton10Et_.push_back( L3obj.et() );
      hltPhoton10Pt_.push_back( L3obj.pt() );
      hltPhoton10Eta_.push_back( L3obj.eta() );
      hltPhoton10Phi_.push_back( L3obj.phi() );
    }
  }


  // HLT_Photon15_L1R
  //  myLastFilter = edm::InputTag("hltL1NonIsoHLTNonIsoSinglePhotonEt15HcalIsolFilter","","HLT");
  myLastFilter = edm::InputTag("hltL1NonIsoHLTNonIsoSinglePhotonEt15HcalIsolFilter","",trigProcess);
  //  std::cout << "Trigger: HLT_Photon15_L1R" << std::flush;

  if ( trgEvent->filterIndex(myLastFilter) < trgEvent->sizeFilters() ) 
  {
    const trigger::Keys& keys( trgEvent->filterKeys( trgEvent->filterIndex(myLastFilter) ) );
    //    std::cout << " good" << std::endl;

    for ( unsigned int hlto = 0; hlto < keys.size(); hlto++ ) 
    {
      trigger::size_type hltf = keys[hlto];
      const trigger::TriggerObject& L3obj(TOC[hltf]);
      
      //      std::cout << "Object:  Et " << L3obj.et() << std::endl;
      hltPhoton15Et_.push_back( L3obj.et() );
      hltPhoton15Pt_.push_back( L3obj.pt() );
      hltPhoton15Eta_.push_back( L3obj.eta() );
      hltPhoton15Phi_.push_back( L3obj.phi() );
    }
  }

  // HLT_Photon20_L1R
  //  myLastFilter = edm::InputTag("hltL1NonIsoHLTNonIsoSinglePhotonEt20HcalIsolFilter","","HLT");
  myLastFilter = edm::InputTag("hltL1NonIsoHLTNonIsoSinglePhotonEt20HcalIsolFilter","",trigProcess);
  //  std::cout << "Trigger: HLT_Photon20_L1R" << std::flush;

  if ( trgEvent->filterIndex(myLastFilter) < trgEvent->sizeFilters() ) 
  {
    const trigger::Keys& keys( trgEvent->filterKeys( trgEvent->filterIndex(myLastFilter) ) );
    //    std::cout << " good" << std::endl;

    for ( unsigned int hlto = 0; hlto < keys.size(); hlto++ ) 
    {
      trigger::size_type hltf = keys[hlto];
      const trigger::TriggerObject& L3obj(TOC[hltf]);
      
      //      std::cout << "Object:  Et " << L3obj.et() << std::endl;
      hltPhoton20Et_.push_back( L3obj.et() );
      hltPhoton20Pt_.push_back( L3obj.pt() );
      hltPhoton20Eta_.push_back( L3obj.eta() );
      hltPhoton20Phi_.push_back( L3obj.phi() );
    }
  }

  // HLT_Photon15_Cleaned_L1R
  //  myLastFilter = edm::InputTag("hltL1NonIsoHLTNonIsoSinglePhotonEt15CleanedHcalIsolFilter","","HLT");
  myLastFilter = edm::InputTag("hltL1NonIsoHLTNonIsoSinglePhotonEt15CleanedHcalIsolFilter","",trigProcess);
  //  std::cout << "Trigger: HLT_Photon15_Cleaned_L1R" << std::flush;

  if ( trgEvent->filterIndex(myLastFilter) < trgEvent->sizeFilters() ) 
  {
    const trigger::Keys& keys( trgEvent->filterKeys( trgEvent->filterIndex(myLastFilter) ) );
    //    std::cout << " good" << std::endl;

    for ( unsigned int hlto = 0; hlto < keys.size(); hlto++ ) 
    {
      trigger::size_type hltf = keys[hlto];
      const trigger::TriggerObject& L3obj(TOC[hltf]);
      
      //      std::cout << "Object:  Et " << L3obj.et() << std::endl;
      hltPhoton15CleanEt_.push_back( L3obj.et() );
      hltPhoton15CleanPt_.push_back( L3obj.pt() );
      hltPhoton15CleanEta_.push_back( L3obj.eta() );
      hltPhoton15CleanPhi_.push_back( L3obj.phi() );
    }
  }

  // HLT_Photon20_Cleaned_L1R
  //  myLastFilter = edm::InputTag("hltL1NonIsoHLTNonIsoSinglePhotonEt20CleanedHcalIsolFilter","","HLT");
  myLastFilter = edm::InputTag("hltL1NonIsoHLTNonIsoSinglePhotonEt20CleanedHcalIsolFilter","",trigProcess);
  //  std::cout << "Trigger: HLT_Photon20_Cleaned_L1R" << std::flush;

  if ( trgEvent->filterIndex(myLastFilter) < trgEvent->sizeFilters() ) 
  {
    const trigger::Keys& keys( trgEvent->filterKeys( trgEvent->filterIndex(myLastFilter) ) );
    //    std::cout << " good" << std::endl;

    for ( unsigned int hlto = 0; hlto < keys.size(); hlto++ ) 
    {
      trigger::size_type hltf = keys[hlto];
      const trigger::TriggerObject& L3obj(TOC[hltf]);
      
      //      std::cout << "Object:  Et " << L3obj.et() << std::endl;
      hltPhoton20CleanEt_.push_back( L3obj.et() );
      hltPhoton20CleanPt_.push_back( L3obj.pt() );
      hltPhoton20CleanEta_.push_back( L3obj.eta() );
      hltPhoton20CleanPhi_.push_back( L3obj.phi() );
    }
  }

  // HLT_Photon30_Cleaned_L1R
  //  myLastFilter = edm::InputTag("hltL1NonIsoHLTNonIsoSinglePhotonEt30CleanedHcalIsolFilter","","HLT");
  myLastFilter = edm::InputTag("hltL1NonIsoHLTNonIsoSinglePhotonEt30CleanedHcalIsolFilter","",trigProcess);
  //  std::cout << "Trigger: HLT_Photon30_Cleaned_L1R" << std::flush;

  if ( trgEvent->filterIndex(myLastFilter) < trgEvent->sizeFilters() ) 
  {
    const trigger::Keys& keys( trgEvent->filterKeys( trgEvent->filterIndex(myLastFilter) ) );
    //    std::cout << " good" << std::endl;

    for ( unsigned int hlto = 0; hlto < keys.size(); hlto++ ) 
    {
      trigger::size_type hltf = keys[hlto];
      const trigger::TriggerObject& L3obj(TOC[hltf]);
      
      //      std::cout << "Object:  Et " << L3obj.et() << std::endl;
      hltPhoton30CleanEt_.push_back( L3obj.et() );
      hltPhoton30CleanPt_.push_back( L3obj.pt() );
      hltPhoton30CleanEta_.push_back( L3obj.eta() );
      hltPhoton30CleanPhi_.push_back( L3obj.phi() );
    }
  }

  
}


void
HltTree::Clear(){
  trigResult_.clear();
  trigName_.clear();
  
  hltEle10LWEt_.clear();
  hltEle10LWPt_.clear();
  hltEle10LWEta_.clear();
  hltEle10LWPhi_.clear();

  hltEle15LWEt_.clear();
  hltEle15LWPt_.clear();
  hltEle15LWEta_.clear();
  hltEle15LWPhi_.clear();

  hltEle20LWEt_.clear();
  hltEle20LWPt_.clear();
  hltEle20LWEta_.clear();
  hltEle20LWPhi_.clear();

  hltEle10SWEt_.clear();
  hltEle10SWPt_.clear();
  hltEle10SWEta_.clear();
  hltEle10SWPhi_.clear();

  hltEle15SWEt_.clear();
  hltEle15SWPt_.clear();
  hltEle15SWEta_.clear();
  hltEle15SWPhi_.clear();

  hltEle20SWEt_.clear();
  hltEle20SWPt_.clear();
  hltEle20SWEta_.clear();
  hltEle20SWPhi_.clear();

  hltEle25SWEt_.clear();
  hltEle25SWPt_.clear();
  hltEle25SWEta_.clear();
  hltEle25SWPhi_.clear();

  hltPhoton10Et_.clear();
  hltPhoton10Pt_.clear();
  hltPhoton10Eta_.clear();
  hltPhoton10Phi_.clear();

  hltPhoton15Et_.clear();
  hltPhoton15Pt_.clear();
  hltPhoton15Eta_.clear();
  hltPhoton15Phi_.clear();

  hltPhoton20Et_.clear();
  hltPhoton20Pt_.clear();
  hltPhoton20Eta_.clear();
  hltPhoton20Phi_.clear();

  hltPhoton15CleanEt_.clear();
  hltPhoton15CleanPt_.clear();
  hltPhoton15CleanEta_.clear();
  hltPhoton15CleanPhi_.clear();

  hltPhoton20CleanEt_.clear();
  hltPhoton20CleanPt_.clear();
  hltPhoton20CleanEta_.clear();
  hltPhoton20CleanPhi_.clear();

  hltPhoton30CleanEt_.clear();
  hltPhoton30CleanPt_.clear();
  hltPhoton30CleanEta_.clear();
  hltPhoton30CleanPhi_.clear();

}

