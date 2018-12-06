#include "config_DESY2018_42.hh"

void defineDESY2018_Config42(std::vector<std::pair<std::string, G4double> > &dz_map, G4double &viewpoint) {
	viewpoint = -16.5 * m;

	dz_map.push_back(std::make_pair("DATURA", 3.0 * m));
	dz_map.push_back(std::make_pair("DATURA", 15.3 * cm));
	dz_map.push_back(std::make_pair("DATURA", 15.2 * cm));

	dz_map.push_back(std::make_pair("PCB_rot30", 6.3 * cm));
	dz_map.push_back(std::make_pair("Si_wafer_rot30", 0.));
	dz_map.push_back(std::make_pair("Kapton_layer_rot30", 0.));
	dz_map.push_back(std::make_pair("Cu_baseplate_rot30", 0.));

	dz_map.push_back(std::make_pair("DATURA", 28.0 * cm));
	dz_map.push_back(std::make_pair("DATURA", 15.2 * cm));
	dz_map.push_back(std::make_pair("DATURA", 15.3 * cm));
	

	dz_map.push_back(std::make_pair("W_4mm_absorber_EE_DESY2018", 23.4 * cm));
	dz_map.push_back(std::make_pair("W_4mm_absorber_EE_DESY2018", 0));
	dz_map.push_back(std::make_pair("W_4mm_absorber_EE_DESY2018", 0));
	dz_map.push_back(std::make_pair("W_4mm_absorber_EE_DESY2018", 0));

	dz_map.push_back(std::make_pair("Cu_baseplate", 2.4 * cm));	
	dz_map.push_back(std::make_pair("Kapton_layer", 0.));
	dz_map.push_back(std::make_pair("Si_wafer", 0.));
	dz_map.push_back(std::make_pair("PCB", 0));


	dz_map.push_back(std::make_pair("PCB", 1.5 * cm));
	dz_map.push_back(std::make_pair("Si_wafer", 0.));
	dz_map.push_back(std::make_pair("Kapton_layer", 0.));
	dz_map.push_back(std::make_pair("Cu_baseplate", 0.));
}