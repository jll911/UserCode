#include "UserCode/TrigTree/interface/L1Tree.h"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/L1Trigger/interface/L1EmParticle.h"
#include "DataFormats/L1Trigger/interface/L1EmParticleFwd.h"

L1Tree::L1Tree(std::string name,TTree* tree)
{
  name_=name;
  tree_=tree;

  tree_->Branch("L1trigResults",&L1trigResult_);
  tree_->Branch("L1trigErrCode",&L1trigErrCode_);
  tree_->Branch("L1trigName",&L1trigName_);

  tree_->Branch("l1IsoEleEt",        &l1IsoEleEt_);
  tree_->Branch("l1IsoEleEnergy",    &l1IsoEleEnergy_);
  tree_->Branch("l1IsoEleEta",       &l1IsoEleEta_);
  tree_->Branch("l1IsoElePhi",       &l1IsoElePhi_);
  tree_->Branch("l1NonIsoEleEt",     &l1NonIsoEleEt_);
  tree_->Branch("l1NonIsoEleEnergy", &l1NonIsoEleEnergy_);
  tree_->Branch("l1NonIsoEleEta",    &l1NonIsoEleEta_);
  tree_->Branch("l1NonIsoElePhi",    &l1NonIsoElePhi_);
}

void
L1Tree::Fill(const edm::Event& iEvent,const edm::EventSetup& iSetup)
{
  using namespace l1extra ;
  using namespace edm; 
  
  m_l1GtUtils.retrieveL1EventSetup(iSetup);
  
  // L1 Trigger
  m_l1GtUtils.retrieveL1EventSetup(iSetup);
  int iErrorCode = -1;

  std::vector< std::string > l1TrigNames;
  l1TrigNames.push_back("L1_SingleIsoEG5");
  l1TrigNames.push_back("L1_SingleIsoEG8");
  l1TrigNames.push_back("L1_SingleIsoEG10");
  l1TrigNames.push_back("L1_SingleIsoEG12");
  l1TrigNames.push_back("L1_SingleIsoEG15");
  l1TrigNames.push_back("L1_SingleEG2");
  l1TrigNames.push_back("L1_SingleEG5");
  l1TrigNames.push_back("L1_SingleEG8");
  l1TrigNames.push_back("L1_SingleEG10");
  l1TrigNames.push_back("L1_SingleEG12");
  l1TrigNames.push_back("L1_SingleEG15");
  l1TrigNames.push_back("L1_SingleEG20"); 

  std::map<std::string,int> l1TriggersOfInterest_;

  for (unsigned int n = 0; n < l1TrigNames.size(); n++)
    { 
      bool decision = 
        m_l1GtUtils.decisionBeforeMask
        (iEvent, l1TrigNames.at(n), iErrorCode);
      
      L1trigName_.push_back(l1TrigNames.at(n));
      L1trigResult_.push_back(decision);
      L1trigErrCode_.push_back(iErrorCode);

//       if (decision) std::cout << l1TrigNames.at(n) << ":" << decision << std::endl;

      /*if iErrCode=0 (fine), =1 
      (Trig. doesn't exist in this L1 menu), 
      =else, an error with trigger.*/
    }

    Handle<L1EmParticleCollection> isoEmColl ;
    iEvent.getByLabel("l1extraParticles", "Isolated", isoEmColl ) ;

   for( L1EmParticleCollection::const_iterator emItr = isoEmColl->begin() ;
        emItr != isoEmColl->end() ;
        ++emItr )
   {
       l1IsoEleEt_.push_back(emItr->et());
       l1IsoEleEnergy_.push_back(emItr->energy());
       l1IsoEleEta_.push_back(emItr->eta());
       l1IsoElePhi_.push_back(emItr->phi());
   }
    
    Handle<L1EmParticleCollection> nonIsoEmColl ;
    iEvent.getByLabel("l1extraParticles", "NonIsolated", nonIsoEmColl ) ;
    
   for( L1EmParticleCollection::const_iterator emItr = nonIsoEmColl->begin() ;
        emItr != nonIsoEmColl->end() ;
        ++emItr )
   {
       l1NonIsoEleEt_.push_back(emItr->et());
       l1NonIsoEleEnergy_.push_back(emItr->energy());
       l1NonIsoEleEta_.push_back(emItr->eta());
       l1NonIsoElePhi_.push_back(emItr->phi());
   }
}


void
L1Tree::Clear()
{
  L1trigResult_.clear();
  L1trigErrCode_.clear();
  L1trigName_.clear();
  l1IsoEleEt_.clear();
  l1IsoEleEnergy_.clear();
  l1IsoEleEta_.clear();
  l1IsoElePhi_.clear();
  l1NonIsoEleEt_.clear();
  l1NonIsoEleEnergy_.clear();
  l1NonIsoEleEta_.clear();
  l1NonIsoElePhi_.clear();
}
