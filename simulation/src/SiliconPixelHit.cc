#include "SiliconPixelHit.hh"

//#define DEBUG

SiliconPixelHit::SiliconPixelHit(G4String vol_name, G4int copy_no_sensor, G4int copy_no_cell) {
	this->vol_name = vol_name;;
	this->copy_no_cell = copy_no_cell;
	this->copy_no_sensor = copy_no_sensor;

	this->pos_x = this->pos_y = this->pos_z = -1;


	this->eDep_digi = -1;
	this->edep_nonIonizing_digi = -1;
	this->timeOfArrival_digi = -1;
	this->timeOfArrival_last_digi = -1;
	this->_isValidHit = false;;

}


void SiliconPixelHit::Digitise(const G4double timeWindow, const G4double toaThreshold) {
	
	//process energy deposits
	if (eDep.size() == 0) {
		_isValidHit = false;
		return;
	}

	std::sort(eDep.begin(), eDep.end(), [](const std::pair<G4double, G4double>& left, const std::pair<G4double, G4double>& right) {
		return left.second < right.second;		//second = time
	});

	G4double firstHitTime = eDep[0].second;
	eDep_digi = 0;
	for (size_t i=0; i<eDep.size(); i++) {
		if (timeWindow<0 || eDep[i].second < firstHitTime+timeWindow) eDep_digi += eDep[i].first; 
#ifdef DEBUG		
		else std::cout<<"Rejecting hit ("<<eDep[i].first<<" keV)  at time "<<eDep[i].second<< "(start: "<<firstHitTime<<")"<<std::endl;
#endif		
		if (eDep_digi>toaThreshold) {
			if (timeOfArrival_digi == -1) timeOfArrival_digi = eDep[i].second;
			timeOfArrival_last_digi = eDep[i].second;
		}
	}
	_isValidHit = (eDep_digi > 0);
#ifdef DEBUG		
	std::cout<<timeOfArrival_digi<<" to "<<timeOfArrival_last_digi<<"  vs  energy: "<<eDep_digi<<std::endl;
#endif		


	//non ionizing part, does not contribute to TOAs
	if (edep_nonIonizing.size() == 0) return;
		
		
	std::sort(edep_nonIonizing.begin(), edep_nonIonizing.end(), [](const std::pair<G4double, G4double>& left, const std::pair<G4double, G4double>& right) {
		return left.second < right.second;		//second = time
	});

	edep_nonIonizing_digi = 0;
	for (size_t i=0; i<eDep.size(); i++) {
		if (timeWindow==-1 || edep_nonIonizing[i].second < firstHitTime+timeWindow) edep_nonIonizing_digi += edep_nonIonizing[i].first;
	}

};