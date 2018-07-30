#include "G4VHit.hh"
#include "G4Allocator.hh"
#include "G4THitsCollection.hh"

class SiliconPixelHit : public G4VHit {
	public:
		SiliconPixelHit(const G4int layer);
		~SiliconPixelHit() {};
		void Print() {};
		void AddEdep(const double e) {eDep += e; }

		G4double GetEdep() const {return eDep;}
		G4int GetLayerNumber() const { return layerNumber;}
	private:
		const G4int layerNumber;
		G4double eDep;
};

typedef G4THitsCollection<SiliconPixelHit> SiliconPixelHitCollection;