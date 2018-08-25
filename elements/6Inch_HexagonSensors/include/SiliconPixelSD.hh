#include "G4VSensitiveDetector.hh"
#include "G4SDManager.hh"
#include "SiliconPixelHit.hh"


class SiliconPixelSD : public G4VSensitiveDetector {
	public:
		SiliconPixelSD(G4String name);
		~SiliconPixelSD();
		SiliconPixelHitCollection* hitCollection;
		G4bool ProcessHits(G4Step *step, G4TouchableHistory *ROhist);

		void Initialize(G4HCofThisEvent* HCE);
		void EndOfEvent(G4HCofThisEvent* HCE);
	private:
		std::map<int, SiliconPixelHit*> tmp_hits;

};	