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
	G4int copy_no_cell = touchable->GetVolume(0)->GetCopyNo();
	G4int copy_no_sensor = touchable->GetVolume(1)->GetCopyNo();
	
	G4double edep = step->GetTotalEnergyDeposit();
	G4double edep_nonIonizing = step->GetNonIonizingEnergyDeposit();

	G4double timedep = step->GetPostStepPoint()->GetGlobalTime()/CLHEP::ns;

	//hit positions
	G4double hit_x = touchable->GetVolume(0)->GetTranslation().x()/CLHEP::mm;
	G4double hit_y = touchable->GetVolume(0)->GetTranslation().y()/CLHEP::mm;
	G4double hit_z = touchable->GetVolume(1)->GetTranslation().z()/CLHEP::mm;

	//track parameters
	G4int track_ID = step->GetTrack()->GetTrackID();
	G4int track_pdgID = step->GetTrack()->GetDefinition()->GetPDGEncoding();
	G4double track_phi = step->GetTrack()->GetMomentum().phi();
	G4double track_theta = step->GetTrack()->GetMomentum().theta();

	if (edep>0)
		std::cout<<"name: "<<vol_name<<"(copy:"<<copy_no_cell<<"-"<<copy_no_sensor<<") energy: "<<edep<<" non ionizing: "<<edep_nonIonizing<<"  at time: "<<timedep<<"  located: "<<hit_x<<","<<hit_y<<","<<hit_z<<"  pdgID: "<<track_pdgID<<"  phi: "<<track_phi<<"  theta: "<<track_theta<<"   track ID: "<<track_ID<<std::endl;

	return true;
}