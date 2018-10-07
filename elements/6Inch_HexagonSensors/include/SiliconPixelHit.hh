#include "G4VHit.hh"
#include "G4Allocator.hh"
#include "G4THitsCollection.hh"

class SiliconPixelHit : public G4VHit {
	public:
		SiliconPixelHit(G4String, G4int, G4int);
		~SiliconPixelHit() {};
		void Print() {};
		G4int ID() {return 1000*copy_no_sensor+copy_no_cell;}
		void AddEdep(const double e) {eDep += e; }
		void AddEdepNonIonizing(const double e) {edep_nonIonizing += e; }
		void UpdateTimeOfArrival(const double e, const double t) {
			if (e<=0.) return;		//4 MIP cut
			if (this->timeOfArrival==-1) timeOfArrival = t;
			else if (this->timeOfArrival > t) this->timeOfArrival = t;
		}

		void SetPosition(G4double x, G4double y, G4double z) {
			this->pos_x = x;
			this->pos_y = y;
			this->pos_z = z;
		}

		G4double GetX() {return this->pos_x;}
		G4double GetY() {return this->pos_y;}
		G4double GetZ() {return this->pos_z;}

		G4double GetEdep() const {return eDep;}
		G4double GetEdepNonIonizing() const {return edep_nonIonizing;}
		G4double GetTOA() const {return timeOfArrival;}

	private:
		G4String vol_name;
		G4int copy_no_cell; 
		G4int copy_no_sensor; 
		G4double pos_x;		//positions in mm
		G4double pos_y;		//positions in mm
		G4double pos_z;		//positions in mm

		G4double eDep;
		G4double edep_nonIonizing;
		G4double timeOfArrival;
};

typedef G4THitsCollection<SiliconPixelHit> SiliconPixelHitCollection;