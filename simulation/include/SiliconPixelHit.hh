#include "G4VHit.hh"
#include "G4Allocator.hh"
#include "G4THitsCollection.hh"
#include <vector>
#include <cstdlib>

class SiliconPixelHit : public G4VHit {
	public:
		SiliconPixelHit(G4String, G4int, G4int);
		~SiliconPixelHit() {};
		void Print() {};
		G4int ID() {return 1000*copy_no_sensor+copy_no_cell;}
		void AddEdep(const G4double e, const G4double t) {if (e>0) eDep.push_back(std::make_pair(e, t)); }
		void AddEdepNonIonizing(const G4double e, const G4double t) {if (e>0) edep_nonIonizing.push_back(std::make_pair(e, t)); }
		
		void Digitise(const G4double timeWindow, const G4double toaThreshold);


		void SetPosition(G4double x, G4double y, G4double z) {
			this->pos_x = x;
			this->pos_y = y;
			this->pos_z = z;
		}

		G4double GetX() {return this->pos_x;}
		G4double GetY() {return this->pos_y;}
		G4double GetZ() {return this->pos_z;}

		
		bool isValidHit() const {return _isValidHit;}
		G4double GetEdep() const {return eDep_digi;}
		G4double GetEdepNonIonizing() const {return edep_nonIonizing_digi;}
		G4double GetTOA() const {return timeOfArrival_digi;}
		G4double GetLastTOA() const {return timeOfArrival_last_digi;}

	private:
		G4String vol_name;
		G4int copy_no_cell; 
		G4int copy_no_sensor; 
		G4double pos_x;		//positions in mm
		G4double pos_y;		//positions in mm
		G4double pos_z;		//positions in mm

		std::vector<std::pair<G4double, G4double> > eDep;
		std::vector<std::pair<G4double, G4double> > edep_nonIonizing;

		//processed values
		bool _isValidHit;
		G4double eDep_digi;
		G4double edep_nonIonizing_digi;
		G4double timeOfArrival_digi;
		G4double timeOfArrival_last_digi;
};

typedef G4THitsCollection<SiliconPixelHit> SiliconPixelHitCollection;