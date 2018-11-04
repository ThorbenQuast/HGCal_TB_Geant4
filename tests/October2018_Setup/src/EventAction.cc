
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
	G4int collId = sdManager->GetCollectionID("SiliconPixelHitCollection");
	auto hc = hce->GetHC(collId);
	if ( ! hc ) return;
	double esum = 0; double cogz = 0; int Nhits = 0;
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

			Nhits++;
			esum += hit->GetEdep() * CLHEP::keV / CLHEP::MeV;
			cogz += hit->GetZ() * hit->GetEdep();
		}
	}
	if (esum > 0) cogz /= esum;

	analysisManager->FillNtupleDColumn(11, esum);
	analysisManager->FillNtupleDColumn(12, cogz / CLHEP::cm);
	analysisManager->FillNtupleIColumn(13, Nhits);

	analysisManager->CreateNtupleDColumn("signalSum_MeV");    // column Id = 11
	analysisManager->CreateNtupleDColumn("COGZ_cm");    // column Id = 12
	analysisManager->CreateNtupleIColumn("NHits");    // column Id = 13

	analysisManager->AddNtupleRow();
}


void EventAction::DefineCommands() {

	fMessenger
	    = new G4GenericMessenger(this,
	                             "/HGCalOctober2018/hits/",
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
