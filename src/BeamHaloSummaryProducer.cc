#include "RecoMET/METProducers/interface/BeamHaloSummaryProducer.h"

/*
  [class]:  BeamHaloSummaryProducer
  [authors]: R. Remington, The University of Florida
  [description]: See BeamHaloSummaryProducer.h
  [date]: October 15, 2009
*/

using namespace edm;
using namespace std;
using namespace reco;

BeamHaloSummaryProducer::BeamHaloSummaryProducer(const edm::ParameterSet& iConfig)
{
  IT_CSCHaloData = iConfig.getParameter<edm::InputTag>("CSCHaloDataLabel");
  IT_EcalHaloData = iConfig.getParameter<edm::InputTag>("EcalHaloDataLabel");
  IT_HcalHaloData = iConfig.getParameter<edm::InputTag>("HcalHaloDataLabel");
  IT_GlobalHaloData = iConfig.getParameter<edm::InputTag>("GlobalHaloDataLabel");
  
  L_EcalPhiWedgeEnergy = (float) iConfig.getParameter<double>("l_EcalPhiWedgeEnergy");
  L_EcalPhiWedgeConstituents = iConfig.getParameter<int>("l_EcalPhiWedgeConstituents");
  L_EcalPhiWedgeToF = (float)iConfig.getParameter<double>("l_EcalPhiWedgeToF");
  L_EcalPhiWedgeConfidence = (float)iConfig.getParameter<double>("l_EcalPhiWedgeConfidence");
  L_EcalShowerShapesRoundness = (float)iConfig.getParameter<double>("l_EcalShowerShapesRoundness");
  L_EcalShowerShapesAngle =(float) iConfig.getParameter<double>("l_EcalShowerShapesAngle");  

  T_EcalPhiWedgeEnergy = (float)iConfig.getParameter<double>("t_EcalPhiWedgeEnergy");
  T_EcalPhiWedgeConstituents = iConfig.getParameter<int>("t_EcalPhiWedgeConstituents");
  T_EcalPhiWedgeToF = (float)iConfig.getParameter<double>("t_EcalPhiWedgeToF");
  T_EcalPhiWedgeConfidence = (float)iConfig.getParameter<double>("t_EcalPhiWedgeConfidence");
  T_EcalShowerShapesRoundness = (float)iConfig.getParameter<double>("t_EcalShowerShapesRoundness");
  T_EcalShowerShapesAngle = (float)iConfig.getParameter<double>("t_EcalShowerShapesAngle");

  L_HcalPhiWedgeEnergy = (float)iConfig.getParameter<double>("l_HcalPhiWedgeEnergy");
  L_HcalPhiWedgeConstituents = iConfig.getParameter<int>("l_HcalPhiWedgeConstituents");
  L_HcalPhiWedgeToF = (float)iConfig.getParameter<double>("l_HcalPhiWedgeToF");
  L_HcalPhiWedgeConfidence = (float)iConfig.getParameter<double>("l_HcalPhiWedgeConfidence");
  
  T_HcalPhiWedgeEnergy = (float)iConfig.getParameter<double>("t_HcalPhiWedgeEnergy");
  T_HcalPhiWedgeConstituents = iConfig.getParameter<int>("t_HcalPhiWedgeConstituents");
  T_HcalPhiWedgeToF = (float)iConfig.getParameter<double>("t_HcalPhiWedgeToF");
  T_HcalPhiWedgeConfidence = (float)iConfig.getParameter<double>("t_HcalPhiWedgeConfidence");

  produces<BeamHaloSummary>();
}

void BeamHaloSummaryProducer::produce(Event& iEvent, const EventSetup& iSetup)
{
  // BeamHaloSummary object 
  std::auto_ptr<BeamHaloSummary> TheBeamHaloSummary( new BeamHaloSummary() );

  // CSC Specific Halo Data
  Handle<CSCHaloData> TheCSCHaloData;
  iEvent.getByLabel(IT_CSCHaloData, TheCSCHaloData);

  const CSCHaloData CSCData = (*TheCSCHaloData.product() );
  //Loose Id 
  if( CSCData.NumberOfHaloTriggers() || CSCData.NumberOfHaloTracks() ) 
    TheBeamHaloSummary->GetCSCHaloReport()[0] = 1;

  //Tight Id 
  if( CSCData.NumberOfHaloTriggers() && CSCData.NumberOfHaloTracks() )
    TheBeamHaloSummary->GetCSCHaloReport()[1] = 1;
  
  // Ecal Specific Halo Data
  Handle<EcalHaloData> TheEcalHaloData;
  iEvent.getByLabel(IT_EcalHaloData, TheEcalHaloData);
  
  const EcalHaloData EcalData = (*TheEcalHaloData.product() );
  const std::vector<PhiWedge> EcalWedges = EcalData.GetPhiWedges();
  bool EcalLooseId = false, EcalTightId = false;
  for( std::vector<PhiWedge>::const_iterator iWedge = EcalWedges.begin() ; iWedge != EcalWedges.end() ; iWedge++ )
    {
      bool EcaliPhi = false;
      //Loose Id
      if( iWedge-> Energy() > L_EcalPhiWedgeEnergy  && iWedge->NumberOfConstituents() > L_EcalPhiWedgeConstituents && iWedge->MaxTime() > L_EcalPhiWedgeToF)
	{
	  EcalLooseId = true;
	  EcaliPhi = true;
	}
      if( iWedge-> NumberOfConstituents() > L_EcalPhiWedgeConstituents  && 
	  (iWedge->PlusZDirectionConfidence() > L_EcalPhiWedgeConfidence || iWedge->PlusZDirectionConfidence() < (1-L_EcalPhiWedgeConfidence ) ) ) 
	{
	  EcalLooseId = true ;
	  EcaliPhi = true;
	}

      //Tight Id
      if( iWedge-> Energy() > T_EcalPhiWedgeEnergy  && iWedge->NumberOfConstituents() > T_EcalPhiWedgeConstituents && iWedge->MaxTime() > T_EcalPhiWedgeToF)
	{
	  EcalTightId = true;
	  EcaliPhi = true;
	}
      if( iWedge-> NumberOfConstituents() > T_EcalPhiWedgeConstituents  && 
	  (iWedge->PlusZDirectionConfidence() > T_EcalPhiWedgeConfidence || iWedge->PlusZDirectionConfidence() < (1-T_EcalPhiWedgeConfidence) ) ) 
	{
	  EcalTightId = true ;
	  EcaliPhi = true;
	}

      if( EcaliPhi ) 
	TheBeamHaloSummary->GetEcaliPhiSuspects().push_back( iWedge->iPhi() ) ;
    }
  
  edm::ValueMap<float> vm_Angle = EcalData.GetShowerShapesAngle();
  edm::ValueMap<float> vm_Roundness = EcalData.GetShowerShapesRoundness();
  //Access selected SuperClusters
  for(unsigned int n = 0 ; n < EcalData.GetSuperClusters().size() ; n++ )
    {
      edm::Ref<SuperClusterCollection> cluster(EcalData.GetSuperClusters(), n );
      float angle = vm_Angle[cluster];
      float roundness = vm_Roundness[cluster];

      //Loose Selection
      if(  (angle > 0. && angle < L_EcalShowerShapesAngle ) && ( roundness > 0. && roundness < L_EcalShowerShapesRoundness ) )
	EcalLooseId = true;

      //Tight Selection 
      if(  (angle > 0. && angle < T_EcalShowerShapesAngle ) && ( roundness > 0. && roundness < T_EcalShowerShapesRoundness ) )
	EcalTightId = true;
    }


 if( EcalLooseId ) 
    TheBeamHaloSummary->GetEcalHaloReport()[0] = 1;
  if( EcalTightId ) 
    TheBeamHaloSummary->GetEcalHaloReport()[1] = 1;
  


  // Hcal Specific Halo Data
  Handle<HcalHaloData> TheHcalHaloData;
  iEvent.getByLabel(IT_HcalHaloData, TheHcalHaloData);
  const HcalHaloData HcalData = (*TheHcalHaloData.product() );
  const std::vector<PhiWedge> HcalWedges = HcalData.GetPhiWedges();
  bool HcalLooseId = false, HcalTightId = false;
  for( std::vector<PhiWedge>::const_iterator iWedge = HcalWedges.begin() ; iWedge != HcalWedges.end() ; iWedge++ )
    {
      bool HcaliPhi = false;
      //Loose Id
      if( iWedge-> Energy() > L_HcalPhiWedgeEnergy  && iWedge->NumberOfConstituents() > L_HcalPhiWedgeConstituents && iWedge->MaxTime() > L_HcalPhiWedgeToF)
	{
	  HcalLooseId = true;
	  HcaliPhi = true;
	}
      if( iWedge-> NumberOfConstituents() > L_HcalPhiWedgeConstituents  && 
	  (iWedge->PlusZDirectionConfidence() > L_HcalPhiWedgeConfidence || iWedge->PlusZDirectionConfidence() < (1-L_HcalPhiWedgeConfidence ) ) ) 
	{
	  HcalLooseId = true ;
	  HcaliPhi = true;
	}

      //Tight Id
      if( iWedge-> Energy() > T_HcalPhiWedgeEnergy  && iWedge->NumberOfConstituents() > T_HcalPhiWedgeConstituents && iWedge->MaxTime() > T_HcalPhiWedgeToF)
	{
	  HcalTightId = true;
	  HcaliPhi = true;
	}
      if( iWedge-> NumberOfConstituents() > T_HcalPhiWedgeConstituents  && 
	  (iWedge->PlusZDirectionConfidence() > T_HcalPhiWedgeConfidence || iWedge->PlusZDirectionConfidence() < (1-T_HcalPhiWedgeConfidence) ) ) 
	{
	  HcalTightId = true ;
	  HcaliPhi = true;
	}

      if( HcaliPhi ) 
	TheBeamHaloSummary->GetHcaliPhiSuspects().push_back( iWedge->iPhi() ) ;
    }
  
  if( HcalLooseId ) 
    TheBeamHaloSummary->GetHcalHaloReport()[0] = 1;
  if( HcalTightId ) 
    TheBeamHaloSummary->GetHcalHaloReport()[1] = 1;

  // Global Halo Data
  Handle<GlobalHaloData> TheGlobalHaloData;
  iEvent.getByLabel(IT_GlobalHaloData, TheGlobalHaloData);
  bool GlobalLooseId;
  bool GlobalTightId;
  const GlobalHaloData GlobalData = (*TheGlobalHaloData.product() );
  const std::vector<PhiWedge> MatchedHcalWedges = GlobalData.GetMatchedHcalPhiWedges();
  const std::vector<PhiWedge> MatchedEcalWedges = GlobalData.GetMatchedEcalPhiWedges();

  //Loose Id
  if( MatchedEcalWedges.size() || MatchedHcalWedges.size() ) 
    GlobalLooseId = true;

  //Tight Id
  for( std::vector<PhiWedge>::const_iterator iWedge = MatchedEcalWedges.begin() ; iWedge != MatchedEcalWedges.end(); iWedge ++ ) 
    {
      if( iWedge->NumberOfConstituents() > T_EcalPhiWedgeConstituents )
	GlobalTightId = true;
      if( iWedge->MaxTime() < T_EcalPhiWedgeToF ) 
	GlobalTightId = true;
      if( iWedge->PlusZDirectionConfidence() > T_EcalPhiWedgeConfidence )
	GlobalTightId = true;
      if( iWedge->PlusZDirectionConfidence() < (1- T_EcalPhiWedgeConfidence ) )
	GlobalTightId = true;
    }

  for( std::vector<PhiWedge>::const_iterator iWedge = MatchedHcalWedges.begin() ; iWedge != MatchedHcalWedges.end(); iWedge ++ ) 
    {
      if( iWedge->NumberOfConstituents() > T_HcalPhiWedgeConstituents )
	GlobalTightId = true;
      if( iWedge->MaxTime() < T_HcalPhiWedgeToF ) 
	GlobalTightId = true;
      if( iWedge->PlusZDirectionConfidence() > T_HcalPhiWedgeConfidence )
	GlobalTightId = true;
      if( iWedge->PlusZDirectionConfidence() < ( 1 - T_HcalPhiWedgeConfidence ) )
	GlobalTightId = true;
    }
  
  if( GlobalLooseId ) 
    TheBeamHaloSummary->GetGlobalHaloReport()[0] = 1;
  if( GlobalTightId )
    TheBeamHaloSummary->GetGlobalHaloReport()[1] = 1;
  
  
  

  iEvent.put(TheBeamHaloSummary);
  return;
}

void BeamHaloSummaryProducer::beginJob(){return;}
void BeamHaloSummaryProducer::endJob(){return;}
void BeamHaloSummaryProducer::beginRun(edm::Run&, const edm::EventSetup&){return;}
void BeamHaloSummaryProducer::endRun(edm::Run&, const edm::EventSetup&){return;}
BeamHaloSummaryProducer::~BeamHaloSummaryProducer(){}