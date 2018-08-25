
#include "EventAction.hh"
#include "RunAction.hh"

#include "G4Event.hh"
#include "G4SDManager.hh"
#include "G4RunManager.hh"
#include "SiliconPixelHit.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::EventAction(RunAction* runAction)
	: G4UserEventAction(),
	  fRunAction(runAction),
	  fEdep(0.)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::~EventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::BeginOfEventAction(const G4Event*)
{
	fEdep = 0.;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::EndOfEventAction(const G4Event* event)
{
	auto hce = event->GetHCofThisEvent();
	auto sdManager = G4SDManager::GetSDMpointer();
	std::cout<<"End of event..."<<std::endl;
	G4int collId = sdManager->GetCollectionID("SiliconPixelHitCollection");
	auto hc = hce->GetHC(collId);		//load the SiCell collection
	if ( ! hc ) return;
	for (unsigned int i = 0; i < hc->GetSize(); ++i) {
		auto hit = static_cast<SiliconPixelHit*>(hc->GetHit(i));
		if (hit->GetEdep()>0)
			std::cout<<hit->ID()<<": "<<hit->GetX()<<","<<hit->GetY()<<","<<hit->GetZ()<<"   E [GeV]: "<<hit->GetEdep()<<" ("<<hit->GetEdepNonIonizing()<<")  TOA: "<<hit->GetTOA()<<" ns"<<std::endl;
	}

	// accumulate statistics in run action
	fRunAction->AddEdep(fEdep);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
