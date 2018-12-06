#include "configs17To21_June2018.hh"

void defineConfigs17To21_June2018(std::vector<std::pair<std::string, G4double> > &dz_map, G4double &viewpoint) {
	viewpoint = 1.8 * m;

	dz_map.push_back(std::make_pair("DWC", 0.0 * m));
	dz_map.push_back(std::make_pair("DWC", 2.0 * m));
	dz_map.push_back(std::make_pair("DWC", 0.3 * m));
	dz_map.push_back(std::make_pair("Scintillator", 1.5 * m));
	dz_map.push_back(std::make_pair("DWC", 0.3 * m));
	dz_map.push_back(std::make_pair("DWC", 13. * m));
	dz_map.push_back(std::make_pair("DWC", 2. * m));

	dz_map.push_back(std::make_pair("Scintillator", 0.3 * m));
	dz_map.push_back(std::make_pair("Scintillator", 2.0 * m));

	dz_map.push_back(std::make_pair("Al_case", 8. * cm));

	//EE1
	dz_map.push_back(std::make_pair("Pb_absorber_EE", 12 * cm));
	dz_map.push_back(std::make_pair("PCB", 0.3 * cm));
	dz_map.push_back(std::make_pair("Si_wafer", 0.));
	dz_map.push_back(std::make_pair("Kapton_layer", 0.));
	dz_map.push_back(std::make_pair("CuW_baseplate", 0.));
	dz_map.push_back(std::make_pair("Cu_absorber_EE", 0.));
	dz_map.push_back(std::make_pair("CuW_baseplate", 0.));
	dz_map.push_back(std::make_pair("Kapton_layer", 0.));
	dz_map.push_back(std::make_pair("Si_wafer", 0.));
	dz_map.push_back(std::make_pair("PCB", 0));

	dz_map.push_back(std::make_pair("Al_case", 0.6 * cm));

	//EE2
	dz_map.push_back(std::make_pair("Pb_absorber_EE", 0.5 * cm));
	dz_map.push_back(std::make_pair("PCB", 0.7 * cm));
	dz_map.push_back(std::make_pair("Si_wafer", 0.));
	dz_map.push_back(std::make_pair("Kapton_layer", 0.));
	dz_map.push_back(std::make_pair("CuW_baseplate", 0.));
	dz_map.push_back(std::make_pair("Cu_absorber_EE", 0.));
	dz_map.push_back(std::make_pair("CuW_baseplate", 0.));
	dz_map.push_back(std::make_pair("Kapton_layer", 0.));
	dz_map.push_back(std::make_pair("Si_wafer", 0.));
	dz_map.push_back(std::make_pair("PCB", 0));

	//EE3
	dz_map.push_back(std::make_pair("Pb_absorber_EE", 1.1 * cm));
	dz_map.push_back(std::make_pair("PCB", 0.7 * cm));
	dz_map.push_back(std::make_pair("Si_wafer", 0.));
	dz_map.push_back(std::make_pair("Kapton_layer", 0.));
	dz_map.push_back(std::make_pair("CuW_baseplate", 0.));
	dz_map.push_back(std::make_pair("Cu_absorber_EE", 0.));
	dz_map.push_back(std::make_pair("CuW_baseplate", 0.));
	dz_map.push_back(std::make_pair("Kapton_layer", 0.));
	dz_map.push_back(std::make_pair("Si_wafer", 0.));
	dz_map.push_back(std::make_pair("PCB", 0));

	//EE4
	dz_map.push_back(std::make_pair("Pb_absorber_EE", 1.2 * cm));
	dz_map.push_back(std::make_pair("PCB", 0.7 * cm));
	dz_map.push_back(std::make_pair("Si_wafer", 0.));
	dz_map.push_back(std::make_pair("Kapton_layer", 0.));
	dz_map.push_back(std::make_pair("CuW_baseplate", 0.));
	dz_map.push_back(std::make_pair("Cu_absorber_EE", 0.));
	dz_map.push_back(std::make_pair("CuW_baseplate", 0.));
	dz_map.push_back(std::make_pair("Kapton_layer", 0.));
	dz_map.push_back(std::make_pair("Si_wafer", 0.));
	dz_map.push_back(std::make_pair("PCB", 0));

	//EE5
	dz_map.push_back(std::make_pair("Pb_absorber_EE", 1.2 * cm));
	dz_map.push_back(std::make_pair("PCB", 0.7 * cm));
	dz_map.push_back(std::make_pair("Si_wafer", 0.));
	dz_map.push_back(std::make_pair("Kapton_layer", 0.));
	dz_map.push_back(std::make_pair("CuW_baseplate", 0.));
	dz_map.push_back(std::make_pair("Cu_absorber_EE", 0.));
	dz_map.push_back(std::make_pair("CuW_baseplate", 0.));
	dz_map.push_back(std::make_pair("Kapton_layer", 0.));
	dz_map.push_back(std::make_pair("Si_wafer", 0.));
	dz_map.push_back(std::make_pair("PCB", 0));

	//EE6
	dz_map.push_back(std::make_pair("Pb_absorber_EE", 1.2 * cm));
	dz_map.push_back(std::make_pair("PCB", 0.7 * cm));
	dz_map.push_back(std::make_pair("Si_wafer", 0.));
	dz_map.push_back(std::make_pair("Kapton_layer", 0.));
	dz_map.push_back(std::make_pair("CuW_baseplate", 0.));
	dz_map.push_back(std::make_pair("Cu_absorber_EE", 0.));
	dz_map.push_back(std::make_pair("CuW_baseplate", 0.));
	dz_map.push_back(std::make_pair("Kapton_layer", 0.));
	dz_map.push_back(std::make_pair("Si_wafer", 0.));
	dz_map.push_back(std::make_pair("PCB", 0));

	//EE7
	dz_map.push_back(std::make_pair("Pb_absorber_EE", 1.0 * cm));
	dz_map.push_back(std::make_pair("PCB", 0.7 * cm));
	dz_map.push_back(std::make_pair("Si_wafer", 0.));
	dz_map.push_back(std::make_pair("Kapton_layer", 0.));
	dz_map.push_back(std::make_pair("CuW_baseplate", 0.));
	dz_map.push_back(std::make_pair("Cu_absorber_EE", 0.));
	dz_map.push_back(std::make_pair("CuW_baseplate", 0.));
	dz_map.push_back(std::make_pair("Kapton_layer", 0.));
	dz_map.push_back(std::make_pair("Si_wafer", 0.));
	dz_map.push_back(std::make_pair("PCB", 0));

	//EE8
	dz_map.push_back(std::make_pair("Pb_absorber_EE", 1.0 * cm));
	dz_map.push_back(std::make_pair("PCB", 0.7 * cm));
	dz_map.push_back(std::make_pair("Si_wafer", 0.));
	dz_map.push_back(std::make_pair("Kapton_layer", 0.));
	dz_map.push_back(std::make_pair("CuW_baseplate", 0.));
	dz_map.push_back(std::make_pair("Cu_absorber_EE", 0.));
	dz_map.push_back(std::make_pair("CuW_baseplate", 0.));
	dz_map.push_back(std::make_pair("Kapton_layer", 0.));
	dz_map.push_back(std::make_pair("Si_wafer", 0.));
	dz_map.push_back(std::make_pair("PCB", 0));

	//EE9
	dz_map.push_back(std::make_pair("Pb_absorber_EE", 1.0 * cm));
	dz_map.push_back(std::make_pair("PCB", 0.7 * cm));
	dz_map.push_back(std::make_pair("Si_wafer", 0.));
	dz_map.push_back(std::make_pair("Kapton_layer", 0.));
	dz_map.push_back(std::make_pair("CuW_baseplate", 0.));
	dz_map.push_back(std::make_pair("Cu_absorber_EE", 0.));
	dz_map.push_back(std::make_pair("CuW_baseplate", 0.));
	dz_map.push_back(std::make_pair("Kapton_layer", 0.));
	dz_map.push_back(std::make_pair("Si_wafer", 0.));
	dz_map.push_back(std::make_pair("PCB", 0));

	//EE10
	dz_map.push_back(std::make_pair("Pb_absorber_EE", 1.0 * cm));
	dz_map.push_back(std::make_pair("PCB", 0.7 * cm));
	dz_map.push_back(std::make_pair("Si_wafer", 0.));
	dz_map.push_back(std::make_pair("Kapton_layer", 0.));
	dz_map.push_back(std::make_pair("CuW_baseplate", 0.));
	dz_map.push_back(std::make_pair("Cu_absorber_EE", 0.));
	dz_map.push_back(std::make_pair("CuW_baseplate", 0.));
	dz_map.push_back(std::make_pair("Kapton_layer", 0.));
	dz_map.push_back(std::make_pair("Si_wafer", 0.));
	dz_map.push_back(std::make_pair("PCB", 0));

	//EE11
	dz_map.push_back(std::make_pair("Pb_absorber_EE", 1.0 * cm));
	dz_map.push_back(std::make_pair("PCB", 1.0 * cm));
	dz_map.push_back(std::make_pair("Si_wafer", 0.));
	dz_map.push_back(std::make_pair("Kapton_layer", 0.));
	dz_map.push_back(std::make_pair("CuW_baseplate", 0.));
	dz_map.push_back(std::make_pair("Cu_absorber_EE", 0.));
	dz_map.push_back(std::make_pair("CuW_baseplate", 0.));
	dz_map.push_back(std::make_pair("Kapton_layer", 0.));
	dz_map.push_back(std::make_pair("Si_wafer", 0.));
	dz_map.push_back(std::make_pair("PCB", 0));

	//EE12
	dz_map.push_back(std::make_pair("Pb_absorber_EE", 1.4 * cm));
	dz_map.push_back(std::make_pair("PCB", 1.0 * cm));
	dz_map.push_back(std::make_pair("Si_wafer", 0.));
	dz_map.push_back(std::make_pair("Kapton_layer", 0.));
	dz_map.push_back(std::make_pair("CuW_baseplate", 0.));
	dz_map.push_back(std::make_pair("Cu_absorber_EE", 0.));
	dz_map.push_back(std::make_pair("CuW_baseplate", 0.));
	dz_map.push_back(std::make_pair("Kapton_layer", 0.));
	dz_map.push_back(std::make_pair("Si_wafer", 0.));
	dz_map.push_back(std::make_pair("PCB", 0));

	//EE13
	dz_map.push_back(std::make_pair("Pb_absorber_EE", 1.4 * cm));
	dz_map.push_back(std::make_pair("PCB", 0.7 * cm));
	dz_map.push_back(std::make_pair("Si_wafer", 0.));
	dz_map.push_back(std::make_pair("Kapton_layer", 0.));
	dz_map.push_back(std::make_pair("CuW_baseplate", 0.));
	dz_map.push_back(std::make_pair("Cu_absorber_EE", 0.));
	dz_map.push_back(std::make_pair("CuW_baseplate", 0.));
	dz_map.push_back(std::make_pair("Kapton_layer", 0.));
	dz_map.push_back(std::make_pair("Si_wafer", 0.));
	dz_map.push_back(std::make_pair("PCB", 0));

	//EE14
	dz_map.push_back(std::make_pair("Pb_absorber_EE", 1.4 * cm));
	dz_map.push_back(std::make_pair("PCB", 0.7 * cm));
	dz_map.push_back(std::make_pair("Si_wafer", 0.));
	dz_map.push_back(std::make_pair("Kapton_layer", 0.));
	dz_map.push_back(std::make_pair("CuW_baseplate", 0.));
	dz_map.push_back(std::make_pair("Cu_absorber_EE", 0.));
	dz_map.push_back(std::make_pair("CuW_baseplate", 0.));
	dz_map.push_back(std::make_pair("Kapton_layer", 0.));
	dz_map.push_back(std::make_pair("Si_wafer", 0.));
	dz_map.push_back(std::make_pair("PCB", 0));

	dz_map.push_back(std::make_pair("Al_case", 8.4 * cm));


}