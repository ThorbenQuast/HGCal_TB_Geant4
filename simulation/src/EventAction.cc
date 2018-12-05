
#include "EventAction.hh"
#include "RunAction.hh"

#include "G4Event.hh"
#include "G4SDManager.hh"
#include "G4RunManager.hh"
#include "SiliconPixelHit.hh"
#include "SiPMHit.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::EventAction()
	: G4UserEventAction()
{
	hitTimeCut = -1;
	toaThreshold = 0;
	DefineCommands();
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
	hits_type.clear();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::EndOfEventAction(const G4Event* event)
{
	auto analysisManager = G4AnalysisManager::Instance();
	std::cout << "Simulated event " << event->GetEventID() << std::endl;
	analysisManager->FillNtupleIColumn(0, event->GetEventID());
	analysisManager->FillNtupleDColumn(1, event->GetPrimaryVertex()->GetX0() / CLHEP::cm);
	analysisManager->FillNtupleDColumn(2, event->GetPrimaryVertex()->GetY0() / CLHEP::cm);
	analysisManager->FillNtupleDColumn(3, event->GetPrimaryVertex()->GetZ0() / CLHEP::cm);


	auto hce = event->GetHCofThisEvent();
	auto sdManager = G4SDManager::GetSDMpointer();
	G4int collId;


	//HGCAL EE + FH
	collId = sdManager->GetCollectionID("SiliconPixelHitCollection");
	auto hc = hce->GetHC(collId);
	if ( ! hc ) return;
	double esum_HGCAL = 0; double cogz_HGCAL = 0; int Nhits_HGCAL = 0;
	for (unsigned int i = 0; i < hc->GetSize(); ++i) {
		auto hit = static_cast<SiliconPixelHit*>(hc->GetHit(i));
		hit->Digitise(hitTimeCut / CLHEP::ns, toaThreshold / CLHEP::keV );

		if (hit->isValidHit()) {
			hits_ID.push_back(hit->ID());
			hits_x.push_back(hit->GetX());
			hits_y.push_back(hit->GetY());
			hits_z.push_back(hit->GetZ());
			hits_Edep.push_back(hit->GetEdep());
			hits_EdepNonIonising.push_back(hit->GetEdepNonIonizing());
			hits_TOA.push_back(hit->GetTOA());
			hits_type.push_back(0);

			Nhits_HGCAL++;
			esum_HGCAL += hit->GetEdep() * CLHEP::keV / CLHEP::MeV;
			cogz_HGCAL += hit->GetZ() * hit->GetEdep();
		}
	}
	if (esum_HGCAL > 0) cogz_HGCAL /= esum_HGCAL;

	analysisManager->FillNtupleDColumn(12, esum_HGCAL);
	analysisManager->FillNtupleDColumn(13, cogz_HGCAL);
	analysisManager->FillNtupleIColumn(14, Nhits_HGCAL);


	//AHCAL 
	collId = sdManager->GetCollectionID("SiPMHitCollection");
	hc = hce->GetHC(collId);
	if ( ! hc ) return;
	double esum_AHCAL = 0; double cogz_AHCAL = 0; int Nhits_AHCAL = 0;
	for (unsigned int i = 0; i < hc->GetSize(); ++i) {
		auto hit = static_cast<SiPMHit*>(hc->GetHit(i));
		hit->Digitise(-1, 0 );

		if (hit->isValidHit()) {
			hits_ID.push_back(hit->ID());
			hits_x.push_back(hit->GetX());
			hits_y.push_back(hit->GetY());
			hits_z.push_back(hit->GetZ());
			hits_Edep.push_back(hit->GetEdep());
			hits_EdepNonIonising.push_back(hit->GetEdepNonIonizing());
			hits_TOA.push_back(hit->GetTOA());
			hits_type.push_back(1);

			Nhits_AHCAL++;
			esum_AHCAL += hit->GetEdep() * CLHEP::keV / CLHEP::MeV;
			cogz_AHCAL += hit->GetZ() * hit->GetEdep();
		}
	}
	if (esum_AHCAL > 0) cogz_AHCAL /= esum_AHCAL;

	analysisManager->FillNtupleDColumn(15, esum_AHCAL);
	analysisManager->FillNtupleDColumn(16, cogz_AHCAL);
	analysisManager->FillNtupleIColumn(17, Nhits_AHCAL);



	analysisManager->AddNtupleRow();
}


void EventAction::DefineCommands() {

	fMessenger
	    = new G4GenericMessenger(this,
	                             "/Simulation/hits/",
	                             "Primary generator control");


	// time cut command
	auto& timeCutCmd
	    = fMessenger->DeclarePropertyWithUnit("timeCut", "ns", hitTimeCut,
	            "Size of time window for hit digitalisation");
	timeCutCmd.SetParameterName("timeCut", true);
	timeCutCmd.SetRange("timeCut>=-1");
	timeCutCmd.SetDefaultValue("-1");

	// toa threshold command
	auto& toaThresholdCmd
	    = fMessenger->DeclarePropertyWithUnit("toaThreshold", "keV", toaThreshold,
	            "Threshold for TOA activation");
	toaThresholdCmd.SetParameterName("toaThreshold", true);
	toaThresholdCmd.SetRange("toaThreshold>=0");
	toaThresholdCmd.SetDefaultValue("0");

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
