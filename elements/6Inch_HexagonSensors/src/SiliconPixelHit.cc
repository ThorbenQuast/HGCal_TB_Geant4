#include "SiliconPixelHit.hh"

SiliconPixelHit::SiliconPixelHit(G4String vol_name, G4int copy_no_sensor, G4int copy_no_cell) {
	this->vol_name = vol_name;;
	this->copy_no_cell = copy_no_cell; 
	this->copy_no_sensor = copy_no_sensor; 

	this->pos_x = this->pos_y = this->pos_z = -1; 
		
	this->eDep = 0;
	this->edep_nonIonizing = 0;
	this->timeOfArrival = -1;
}