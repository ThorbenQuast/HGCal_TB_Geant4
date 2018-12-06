#include "config1_2_Summer2017.hh"

void defineConfigs1_2_Summer2017(std::vector<std::pair<std::string, G4double> > &dz_map, G4double &viewpoint) {
	viewpoint = 1.8 * m;

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
	dz_map.push_back(std::make_pair("Kapton_layer", 0.3 * mm));
	dz_map.push_back(std::make_pair("CuW_baseplate", 0.));
	dz_map.push_back(std::make_pair("Cu_absorber_EE", 0.));
	
	//absorbers
	dz_map.push_back(std::make_pair("Pb_absorber_EE", 176.7 * mm));
	dz_map.push_back(std::make_pair("Cu_absorber_EE", 0.));
	dz_map.push_back(std::make_pair("Pb_absorber_EE", 0));
	dz_map.push_back(std::make_pair("Cu_absorber_EE", 0.));
	dz_map.push_back(std::make_pair("Pb_absorber_EE", 0));
	dz_map.push_back(std::make_pair("Cu_absorber_EE", 0.));
	dz_map.push_back(std::make_pair("Pb_absorber_EE", 0));
	dz_map.push_back(std::make_pair("Cu_absorber_EE", 0.));	
	

	//EE1
	dz_map.push_back(std::make_pair("CuW_baseplate", 0.));
	dz_map.push_back(std::make_pair("Kapton_layer", 0));
	dz_map.push_back(std::make_pair("Si_wafer", 0.));
	dz_map.push_back(std::make_pair("PCB", 0));

	//absorbers
	dz_map.push_back(std::make_pair("Pb_absorber_EE", 11.0 * mm));
	dz_map.push_back(std::make_pair("Cu_absorber_EE", 0.));
	dz_map.push_back(std::make_pair("Pb_absorber_EE", 0));
	dz_map.push_back(std::make_pair("Cu_absorber_EE", 0.));
	dz_map.push_back(std::make_pair("Pb_absorber_EE", 0));
	dz_map.push_back(std::make_pair("Cu_absorber_EE", 0.));
	dz_map.push_back(std::make_pair("Pb_absorber_EE", 0));
	dz_map.push_back(std::make_pair("Cu_absorber_EE", 0.));	
	dz_map.push_back(std::make_pair("Pb_absorber_EE", 0));
	dz_map.push_back(std::make_pair("Cu_absorber_EE", 0.));	
	dz_map.push_back(std::make_pair("Pb_absorber_EE", 0));
	dz_map.push_back(std::make_pair("Cu_absorber_EE", 0.));	
	dz_map.push_back(std::make_pair("Pb_absorber_EE", 0));
	dz_map.push_back(std::make_pair("Cu_absorber_EE", 0.));	
	dz_map.push_back(std::make_pair("Pb_absorber_EE", 0));


	//EE2
	dz_map.push_back(std::make_pair("PCB", 13.2 * mm));
	dz_map.push_back(std::make_pair("Si_wafer", 0.));
	dz_map.push_back(std::make_pair("Kapton_layer", 0));
	dz_map.push_back(std::make_pair("CuW_baseplate", 0.));
	dz_map.push_back(std::make_pair("Cu_absorber_EE", 0.));	
	dz_map.push_back(std::make_pair("Cu_absorber_EE", 0.));
	dz_map.push_back(std::make_pair("Pb_absorber_EE", 0));
	dz_map.push_back(std::make_pair("Cu_absorber_EE", 0.));
	dz_map.push_back(std::make_pair("Pb_absorber_EE", 0));


	dz_map.push_back(std::make_pair("Fe_absorber_FH", 11.4 * mm));	
	dz_map.push_back(std::make_pair("Al_case", 0. * cm));

	dz_map.push_back(std::make_pair("Steel_case", 38 * mm));

	//FH1
	dz_map.push_back(std::make_pair("Fe_absorber_FH", 78 * mm));	
	dz_map.push_back(std::make_pair("Cu_absorber_FH", 0.));
	dz_map.push_back(std::make_pair("Cu_baseplate", 0.));
	dz_map.push_back(std::make_pair("Kapton_layer", 0));
	dz_map.push_back(std::make_pair("Si_wafer", 0.));
	dz_map.push_back(std::make_pair("PCB", 0));	
	
	//FH2
	dz_map.push_back(std::make_pair("Fe_absorber_FH", 46.9 * mm));	
	dz_map.push_back(std::make_pair("Fe_absorber_FH", 0));	
	dz_map.push_back(std::make_pair("Fe_absorber_FH", 0));	
	dz_map.push_back(std::make_pair("Cu_absorber_FH", 0.));
	dz_map.push_back(std::make_pair("Cu_baseplate", 0.));
	dz_map.push_back(std::make_pair("Kapton_layer", 0));
	dz_map.push_back(std::make_pair("Si_wafer", 0.));
	dz_map.push_back(std::make_pair("PCB", 0));		

	dz_map.push_back(std::make_pair("Fe_absorber_FH", 26.9 * mm));	
	dz_map.push_back(std::make_pair("Steel_case", 0));
	dz_map.push_back(std::make_pair("Steel_case", 63 * mm));

	//FH3
	dz_map.push_back(std::make_pair("Fe_absorber_FH", 0));	
	dz_map.push_back(std::make_pair("Cu_absorber_FH", 0.));
	dz_map.push_back(std::make_pair("Cu_baseplate_SUMMER2017TRIPLET", 0.));
	dz_map.push_back(std::make_pair("Kapton_layer_SUMMER2017TRIPLET", 0));
	dz_map.push_back(std::make_pair("Si_wafer_SUMMER2017TRIPLET", 0.));
	dz_map.push_back(std::make_pair("PCB_SUMMER2017TRIPLET", 0));	

	//FH4
	dz_map.push_back(std::make_pair("Fe_absorber_FH", 58.9 * mm));	
	dz_map.push_back(std::make_pair("Fe_absorber_FH", 0));	
	dz_map.push_back(std::make_pair("Fe_absorber_FH", 0));	
	dz_map.push_back(std::make_pair("Cu_absorber_FH", 0.));
	dz_map.push_back(std::make_pair("Cu_baseplate_SUMMER2017TRIPLET", 0.));
	dz_map.push_back(std::make_pair("Kapton_layer_SUMMER2017TRIPLET", 0));
	dz_map.push_back(std::make_pair("Si_wafer_SUMMER2017TRIPLET", 0.));
	dz_map.push_back(std::make_pair("PCB_SUMMER2017TRIPLET", 0));		

	dz_map.push_back(std::make_pair("Fe_absorber_FH", 48.9 * mm));	
	dz_map.push_back(std::make_pair("Steel_case", 42 * mm));

}