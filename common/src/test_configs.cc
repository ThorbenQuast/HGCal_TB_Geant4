#include "test_configs.hh"

void defineTestConfig100(std::vector<std::pair<std::string, G4double> > &dz_map, G4double &viewpoint) {
	viewpoint = 0.8 * m;

	dz_map.push_back(std::make_pair("DWC", 0.0 * m));
	dz_map.push_back(std::make_pair("DWC", 2.0 * m));
	dz_map.push_back(std::make_pair("DWC", 0.3 * m));
	dz_map.push_back(std::make_pair("Scintillator", 1.5 * m));
	dz_map.push_back(std::make_pair("DWC", 0.3 * m));
	dz_map.push_back(std::make_pair("DWC", 13. * m));
	dz_map.push_back(std::make_pair("DWC", 2. * m));

	dz_map.push_back(std::make_pair("Scintillator", 0.3 * m));
	dz_map.push_back(std::make_pair("Scintillator", 1.0 * m));

	dz_map.push_back(std::make_pair("Al_case", 0. * cm));

	//EE not read out
	dz_map.push_back(std::make_pair("PCB", 150.3 * mm));
	dz_map.push_back(std::make_pair("Si_wafer", 0.));
	dz_map.push_back(std::make_pair("Kapton_layer", 0.3 * mm));
	dz_map.push_back(std::make_pair("CuW_baseplate", 0.));
	dz_map.push_back(std::make_pair("Cu_absorber_EE", 0.));
	
}