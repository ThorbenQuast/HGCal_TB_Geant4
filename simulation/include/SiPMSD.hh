#include "G4VSensitiveDetector.hh"
#include "G4SDManager.hh"
#include "SiPMHit.hh"


class SiPMSD : public G4VSensitiveDetector {
	public:
		SiPMSD(G4String name);
		~SiPMSD();
		SiPMHitCollection* hitCollection;
		G4bool ProcessHits(G4Step *step, G4TouchableHistory *ROhist);

		void Initialize(G4HCofThisEvent* HCE);
		void EndOfEvent(G4HCofThisEvent* HCE);
	private:
		std::map<int, SiPMHit*> tmp_hits;

};	