#include "SiliconPixelSD.hh"

SiliconPixelSD::SiliconPixelSD(G4String name) : G4VSensitiveDetector("SiliconPixelHitCollection") {
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

	tmp_hits.clear();

};
void SiliconPixelSD::EndOfEvent(G4HCofThisEvent* HCE){
	for (std::map<int, SiliconPixelHit*>::iterator it = tmp_hits.begin(); it!= tmp_hits.end(); it++) hitCollection->insert(it->second);
};


G4bool SiliconPixelSD::ProcessHits(G4Step *step, G4TouchableHistory *ROhist) {
	G4TouchableHandle touchable = step->GetPreStepPoint()->GetTouchableHandle();
	

	G4int copy_no_cell = touchable->GetVolume(0)->GetCopyNo();
	G4int copy_no_sensor = touchable->GetVolume(1)->GetCopyNo();
	int tmp_ID = 1000*copy_no_sensor+copy_no_cell;
	if (tmp_hits.find(tmp_ID) == tmp_hits.end()) {		//make new hit
		G4String vol_name = touchable->GetVolume(0)->GetName();
		tmp_hits[tmp_ID] = new SiliconPixelHit(vol_name, copy_no_sensor, copy_no_cell);
		G4double hit_x = (touchable->GetVolume(1)->GetTranslation().x()+touchable->GetVolume(0)->GetTranslation().x())/CLHEP::cm;
		G4double hit_y = (touchable->GetVolume(1)->GetTranslation().x()+touchable->GetVolume(0)->GetTranslation().y())/CLHEP::cm;
		G4double hit_z = touchable->GetVolume(1)->GetTranslation().z()/CLHEP::cm;
		tmp_hits[tmp_ID]->SetPosition(hit_x, hit_y, hit_z);		//in mm
	}

	
	G4double edep = step->GetTotalEnergyDeposit()/CLHEP::keV;		//in keV
	G4double edep_nonIonizing = step->GetNonIonizingEnergyDeposit()/CLHEP::keV;

	G4double timedep = step->GetPostStepPoint()->GetGlobalTime()/CLHEP::ns;

	tmp_hits[tmp_ID]->AddEdep(edep);
	tmp_hits[tmp_ID]->AddEdepNonIonizing(edep_nonIonizing);
	tmp_hits[tmp_ID]->UpdateTimeOfArrival(edep, timedep);

	return true;
}