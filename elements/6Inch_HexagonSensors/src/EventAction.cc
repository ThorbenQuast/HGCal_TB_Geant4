
#include "EventAction.hh"
#include "RunAction.hh"

#include "G4Event.hh"
#include "G4SDManager.hh"
#include "G4RunManager.hh"
#include "SiliconPixelHit.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::EventAction()
	: G4UserEventAction()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::~EventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::BeginOfEventAction(const G4Event* EventAction)
{
	hits_ID.clear();
	hits_x.clear();
	hits_y.clear();
	hits_z.clear();
	hits_Edep.clear();
	hits_EdepNonIonising.clear();
	hits_TOA.clear();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::EndOfEventAction(const G4Event* event)
{
	auto analysisManager = G4AnalysisManager::Instance();
	std::cout<<"Simulated event "<<event->GetEventID()<<std::endl;
	analysisManager->FillNtupleIColumn(0, event->GetEventID());
	analysisManager->FillNtupleDColumn(1, event->GetPrimaryVertex()->GetX0()/CLHEP::cm);
	analysisManager->FillNtupleDColumn(2, event->GetPrimaryVertex()->GetY0()/CLHEP::cm);
	analysisManager->FillNtupleDColumn(3, event->GetPrimaryVertex()->GetZ0()/CLHEP::cm);


	auto hce = event->GetHCofThisEvent();
	auto sdManager = G4SDManager::GetSDMpointer();
	G4int collId = sdManager->GetCollectionID("SiliconPixelHitCollection");
	auto hc = hce->GetHC(collId);
	if ( ! hc ) return;
	for (unsigned int i = 0; i < hc->GetSize(); ++i) {
		auto hit = static_cast<SiliconPixelHit*>(hc->GetHit(i));
		if (hit->GetEdep() > 0) {
			hits_ID.push_back(hit->ID());
			hits_x.push_back(hit->GetX());
			hits_y.push_back(hit->GetY());
			hits_z.push_back(hit->GetZ());
			hits_Edep.push_back(hit->GetEdep());
			hits_EdepNonIonising.push_back(hit->GetEdepNonIonizing());
			hits_TOA.push_back(hit->GetTOA());
		}
	}
	analysisManager->AddNtupleRow();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
