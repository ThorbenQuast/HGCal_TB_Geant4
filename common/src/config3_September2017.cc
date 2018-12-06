#include "config3_September2017.hh"

void defineConfig3_September2017(std::vector<std::pair<std::string, G4double> > &dz_map, G4double &viewpoint) {
	viewpoint = 1.3 * m;

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

	dz_map.push_back(std::make_pair("Pb_absorber_EE", 267.0 * mm));
	dz_map.push_back(std::make_pair("Cu_absorber_EE", 0.));
	dz_map.push_back(std::make_pair("Pb_absorber_EE", 0));

	//EE1
	dz_map.push_back(std::make_pair("Cu_absorber_EE", 0.));	
	dz_map.push_back(std::make_pair("CuW_baseplate", 0.));
	dz_map.push_back(std::make_pair("Kapton_layer", 0));
	dz_map.push_back(std::make_pair("Si_wafer", 0.));
	dz_map.push_back(std::make_pair("PCB", 0));

	dz_map.push_back(std::make_pair("Pb_absorber_EE", 166.6 * mm));

	//EE2
	dz_map.push_back(std::make_pair("Cu_absorber_EE", 0.));
	dz_map.push_back(std::make_pair("CuW_baseplate", 0.));
	dz_map.push_back(std::make_pair("Kapton_layer", 0));
	dz_map.push_back(std::make_pair("Si_wafer", 0.));
	dz_map.push_back(std::make_pair("PCB", 0));

	dz_map.push_back(std::make_pair("Pb_absorber_EE", 11.6 * mm));

	//EE3
	dz_map.push_back(std::make_pair("Cu_absorber_EE", 0.));
	dz_map.push_back(std::make_pair("CuW_baseplate", 0.));
	dz_map.push_back(std::make_pair("Kapton_layer", 0));
	dz_map.push_back(std::make_pair("Si_wafer", 0.));
	dz_map.push_back(std::make_pair("PCB", 0));

	dz_map.push_back(std::make_pair("Pb_absorber_EE", 10.6 * mm));

	//EE4
	dz_map.push_back(std::make_pair("Cu_absorber_EE", 0.));
	dz_map.push_back(std::make_pair("CuW_baseplate", 0.));
	dz_map.push_back(std::make_pair("Kapton_layer", 0));
	dz_map.push_back(std::make_pair("Si_wafer", 0.));
	dz_map.push_back(std::make_pair("PCB", 0));

	dz_map.push_back(std::make_pair("Pb_absorber_EE", 16.72 * mm));

	//EE5
	dz_map.push_back(std::make_pair("Cu_absorber_EE", 0.7 * mm));
	dz_map.push_back(std::make_pair("CuW_baseplate", 0.));
	dz_map.push_back(std::make_pair("Kapton_layer", 0));
	dz_map.push_back(std::make_pair("Si_wafer", 0.));
	dz_map.push_back(std::make_pair("PCB", 0));

	dz_map.push_back(std::make_pair("Pb_absorber_EE", 2.6 * mm));
	dz_map.push_back(std::make_pair("Cu_absorber_EE", 0.));
	dz_map.push_back(std::make_pair("Pb_absorber_EE", 0));
	dz_map.push_back(std::make_pair("Cu_absorber_EE", 0.));	
	dz_map.push_back(std::make_pair("Pb_absorber_EE", 0));

	//EE6
	dz_map.push_back(std::make_pair("PCB", 9.6 * mm));
	dz_map.push_back(std::make_pair("Si_wafer", 0.));
	dz_map.push_back(std::make_pair("Kapton_layer", 0));
	dz_map.push_back(std::make_pair("CuW_baseplate", 0.));
	dz_map.push_back(std::make_pair("Cu_absorber_EE", 0.));

	dz_map.push_back(std::make_pair("Pb_absorber_EE", 0));
	dz_map.push_back(std::make_pair("Cu_absorber_EE", 0.));	
	dz_map.push_back(std::make_pair("Pb_absorber_EE", 0));


	//EE7
	dz_map.push_back(std::make_pair("Cu_absorber_EE", 0.));
	dz_map.push_back(std::make_pair("CuW_baseplate", 0.));
	dz_map.push_back(std::make_pair("Kapton_layer", 0));
	dz_map.push_back(std::make_pair("Si_wafer", 0.));
	dz_map.push_back(std::make_pair("PCB", 0));



	dz_map.push_back(std::make_pair("Pb_absorber_EE", 16.6 * mm));
	dz_map.push_back(std::make_pair("Cu_absorber_EE", 0.));
	dz_map.push_back(std::make_pair("Pb_absorber_EE", 0));
	dz_map.push_back(std::make_pair("Cu_absorber_EE", 0.));
	dz_map.push_back(std::make_pair("Pb_absorber_EE", 0));
	dz_map.push_back(std::make_pair("Cu_absorber_EE", 0.));


	dz_map.push_back(std::make_pair("Fe_absorber_FH", 3 * mm));	
	dz_map.push_back(std::make_pair("Al_case", 0. * cm));

	dz_map.push_back(std::make_pair("Steel_case", 38 * mm));



	//FH1
	dz_map.push_back(std::make_pair("Fe_absorber_FH", 7 * mm));	
	dz_map.push_back(std::make_pair("Cu_absorber_FH", 0.));
	dz_map.push_back(std::make_pair("Cu_baseplate", 0.));
	dz_map.push_back(std::make_pair("Kapton_layer", 0));
	dz_map.push_back(std::make_pair("Si_wafer", 0.));
	dz_map.push_back(std::make_pair("PCB", 0));	
	
	dz_map.push_back(std::make_pair("Fe_absorber_FH", 9.32 * mm));	
	
	//FH2
	dz_map.push_back(std::make_pair("Cu_absorber_FH", 6 * mm));
	dz_map.push_back(std::make_pair("Cu_baseplate", 0.));
	dz_map.push_back(std::make_pair("Kapton_layer", 0));
	dz_map.push_back(std::make_pair("Si_wafer", 0.));
	dz_map.push_back(std::make_pair("PCB", 0));		
	
	dz_map.push_back(std::make_pair("Fe_absorber_FH", 10.6 * mm));	
	
	//FH3
	dz_map.push_back(std::make_pair("Cu_absorber_FH", 8 * mm));
	dz_map.push_back(std::make_pair("Cu_baseplate", 0.));
	dz_map.push_back(std::make_pair("Kapton_layer", 0));
	dz_map.push_back(std::make_pair("Si_wafer", 0.));
	dz_map.push_back(std::make_pair("PCB", 0));
	
	dz_map.push_back(std::make_pair("Fe_absorber_FH", 6.7 * mm));	

	//FH4
	dz_map.push_back(std::make_pair("Cu_absorber_FH", 6 * mm));
	dz_map.push_back(std::make_pair("Cu_baseplate", 0.));
	dz_map.push_back(std::make_pair("Kapton_layer", 0));
	dz_map.push_back(std::make_pair("Si_wafer", 0.));
	dz_map.push_back(std::make_pair("PCB", 0));
	
	dz_map.push_back(std::make_pair("Fe_absorber_FH", 10.24 * mm));	
	
	//FH5
	dz_map.push_back(std::make_pair("Cu_absorber_FH", 9 * mm));
	dz_map.push_back(std::make_pair("Cu_baseplate", 0.));
	dz_map.push_back(std::make_pair("Kapton_layer", 0));
	dz_map.push_back(std::make_pair("Si_wafer", 0.));
	dz_map.push_back(std::make_pair("PCB", 0));
	
	dz_map.push_back(std::make_pair("Fe_absorber_FH", 10.6 * mm));	
	
	//FH6
	dz_map.push_back(std::make_pair("Cu_absorber_FH", 6 * mm));
	dz_map.push_back(std::make_pair("Cu_baseplate", 0.));
	dz_map.push_back(std::make_pair("Kapton_layer", 0));
	dz_map.push_back(std::make_pair("Si_wafer", 0.));
	dz_map.push_back(std::make_pair("PCB", 0));

	dz_map.push_back(std::make_pair("Steel_case", 8.6 * mm));
	dz_map.push_back(std::make_pair("Steel_case", 63  * mm));

	dz_map.push_back(std::make_pair("Fe_absorber_FH", 0));	
	
	//FH7
	dz_map.push_back(std::make_pair("Cu_absorber_FH", 7 * mm));
	dz_map.push_back(std::make_pair("Cu_baseplate", 0.));
	dz_map.push_back(std::make_pair("Kapton_layer", 0));
	dz_map.push_back(std::make_pair("Si_wafer", 0.));
	dz_map.push_back(std::make_pair("PCB", 0));	

	dz_map.push_back(std::make_pair("Fe_absorber_FH", 13.6 * mm));	
	
	//FH8
	dz_map.push_back(std::make_pair("Cu_absorber_FH", 6 * mm));
	dz_map.push_back(std::make_pair("Cu_baseplate", 0.));
	dz_map.push_back(std::make_pair("Kapton_layer", 0));
	dz_map.push_back(std::make_pair("Si_wafer", 0.));
	dz_map.push_back(std::make_pair("PCB", 0));	

	dz_map.push_back(std::make_pair("Fe_absorber_FH", 10.6 * mm));	
	
	//FH9
	dz_map.push_back(std::make_pair("Cu_absorber_FH", 6 * mm));
	dz_map.push_back(std::make_pair("Cu_baseplate", 0.));
	dz_map.push_back(std::make_pair("Kapton_layer", 0));
	dz_map.push_back(std::make_pair("Si_wafer", 0.));
	dz_map.push_back(std::make_pair("PCB", 0));	

	dz_map.push_back(std::make_pair("Fe_absorber_FH", 10.6 * mm));	
	
	//FH10
	dz_map.push_back(std::make_pair("Cu_absorber_FH", 7 * mm));
	dz_map.push_back(std::make_pair("Cu_baseplate", 0.));
	dz_map.push_back(std::make_pair("Kapton_layer", 0));
	dz_map.push_back(std::make_pair("Si_wafer", 0.));
	dz_map.push_back(std::make_pair("PCB", 0));	

	dz_map.push_back(std::make_pair("Fe_absorber_FH", 22.6 * mm));	
	dz_map.push_back(std::make_pair("Fe_absorber_FH", 18.0 * mm));	

	dz_map.push_back(std::make_pair("Steel_case", 18 * mm));

}