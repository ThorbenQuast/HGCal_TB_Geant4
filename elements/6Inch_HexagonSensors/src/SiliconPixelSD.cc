#include "SiliconPixelSD.hh"

SiliconPixelSD::SiliconPixelSD(G4String name) : G4VSensitiveDetector(name) {
	G4cout<<"creating a sensitive detector with name: "<<name<<G4endl;
	collectionName.insert("SiliconPixelHitCollection");

}

SiliconPixelSD::~SiliconPixelSD()
{}

void SiliconPixelSD::Initialize(G4HCofThisEvent* HCE){
	hitCollection = new SiliconPixelHitCollection(GetName(), collectionName[0]);

	static G4int HCID = -1;
	if (HCID<0) HCID = GetCollectionID(0);
	HCE->AddHitsCollection(HCID, hitCollection);

};
void SiliconPixelSD::EndOfEvent(G4HCofThisEvent* HCE){
};


G4bool SiliconPixelSD::ProcessHits(G4Step *step, G4TouchableHistory *ROhist) {
	G4TouchableHandle touchable = step->GetPreStepPoint()->GetTouchableHandle();
	G4String vol_name = touchable->GetVolume(0)->GetName();

	//todo: add time
	//set spatial coordinates
	//define hit as one SD
	G4double edep = step->GetTotalEnergyDeposit();

	std::cout<<"name: "<<vol_name<<" energy: "<<edep<<std::endl;

	return true;
}