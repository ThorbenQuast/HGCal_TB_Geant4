#include "config24_October2018_3.hh"


void defineConfig25_October2018_3_parasitic(std::vector<std::pair<std::string, G4double> > &dz_map, G4double &viewpoint) {
	viewpoint = 9.3 * m;

	dz_map.push_back(std::make_pair("DWC", 0.0 * m));
	dz_map.push_back(std::make_pair("DWC", 2.0 * m));
	dz_map.push_back(std::make_pair("DWC", 0.3 * m));
	dz_map.push_back(std::make_pair("Scintillator", 1.5 * m));
	dz_map.push_back(std::make_pair("DWC", 0.3 * m));
	
	//Emulate the HERD setup
	dz_map.push_back(std::make_pair("HERD_calorimeter_slab", 10. * m));
	for (size_t i=1; i<=21; i++) dz_map.push_back(std::make_pair("HERD_calorimeter_slab", 0));

	dz_map.push_back(std::make_pair("DWC", 5. * m- 63. * cm));
	dz_map.push_back(std::make_pair("DWC", 7. * m));

	dz_map.push_back(std::make_pair("Scintillator", 0.3 * m));
	dz_map.push_back(std::make_pair("Scintillator", 2.0 * m));


	
	//same as configuration 24 from here on
	dz_map.push_back(std::make_pair("Al_case", 2.08 * m));


	dz_map.push_back(std::make_pair("Pb_absorber_EE", 18.4 * cm));
	dz_map.push_back(std::make_pair("Cu_absorber_EE", 0.9 * cm));
	dz_map.push_back(std::make_pair("Pb_absorber_EE", 0.7 * cm));

	//EE1
	dz_map.push_back(std::make_pair("PCB", 1.43 * cm));
	dz_map.push_back(std::make_pair("Si_wafer", 0.));
	dz_map.push_back(std::make_pair("Kapton_layer", 0.));
	dz_map.push_back(std::make_pair("CuW_baseplate", 0.));
	dz_map.push_back(std::make_pair("Cu_absorber_EE", 0.));

	dz_map.push_back(std::make_pair("W_absorber_EE", 0.8 * cm));
	dz_map.push_back(std::make_pair("Pb_absorber_EE", 0.4 * cm));

	//EE2
	dz_map.push_back(std::make_pair("PCB", 1.3 * cm));
	dz_map.push_back(std::make_pair("Si_wafer", 0.));
	dz_map.push_back(std::make_pair("Kapton_layer", 0.));
	dz_map.push_back(std::make_pair("CuW_baseplate", 0.));
	dz_map.push_back(std::make_pair("Cu_absorber_EE", 0.));

	dz_map.push_back(std::make_pair("W_absorber_EE", 0.8 * cm));
	dz_map.push_back(std::make_pair("Pb_absorber_EE", 0.4 * cm));

	//EE3
	dz_map.push_back(std::make_pair("PCB", 0.8 * cm));
	dz_map.push_back(std::make_pair("Si_wafer", 0.));
	dz_map.push_back(std::make_pair("Kapton_layer", 0.));
	dz_map.push_back(std::make_pair("CuW_baseplate", 0.));
	dz_map.push_back(std::make_pair("Cu_absorber_EE", 0.));

	dz_map.push_back(std::make_pair("W_absorber_EE", 0.8 * cm));
	dz_map.push_back(std::make_pair("Pb_absorber_EE", 0.4 * cm));

	//EE4
	dz_map.push_back(std::make_pair("PCB", 1.0 * cm));
	dz_map.push_back(std::make_pair("Si_wafer", 0.));
	dz_map.push_back(std::make_pair("Kapton_layer", 0.));
	dz_map.push_back(std::make_pair("CuW_baseplate", 0.));
	dz_map.push_back(std::make_pair("Cu_absorber_EE", 0.));

	dz_map.push_back(std::make_pair("W_absorber_EE", 0.7 * cm));
	dz_map.push_back(std::make_pair("Pb_absorber_EE", 0.16 * cm));

	//EE5
	dz_map.push_back(std::make_pair("PCB", 0.86 * cm));
	//dz_map.push_back(std::make_pair("Si_wafer", 0.));   //inactive in beam test
	dz_map.push_back(std::make_pair("Kapton_layer", 0. + 0.300 * mm));
	dz_map.push_back(std::make_pair("CuW_baseplate", 0.));
	dz_map.push_back(std::make_pair("Cu_absorber_EE", 0.));

	dz_map.push_back(std::make_pair("W_absorber_EE", 0.76 * cm));
	dz_map.push_back(std::make_pair("Pb_absorber_EE", 0.9 * cm));

	//EE6
	dz_map.push_back(std::make_pair("PCB", 0.77 * cm));
	dz_map.push_back(std::make_pair("Si_wafer", 0.));
	dz_map.push_back(std::make_pair("Kapton_layer", 0.));
	dz_map.push_back(std::make_pair("CuW_baseplate", 0.));
	dz_map.push_back(std::make_pair("Cu_absorber_EE", 0.));

	dz_map.push_back(std::make_pair("W_absorber_EE", 0.7 * cm));
	dz_map.push_back(std::make_pair("Pb_absorber_EE", 0.4 * cm));
	dz_map.push_back(std::make_pair("Cu_absorber_EE", 0.3 * cm));
	dz_map.push_back(std::make_pair("Pb_absorber_EE", 0.31 * cm));
	dz_map.push_back(std::make_pair("Cu_absorber_EE", 0.31 * cm));
	dz_map.push_back(std::make_pair("Pb_absorber_EE", 0.48 * cm));

	//EE7
	dz_map.push_back(std::make_pair("PCB", 1.23 * cm));
	dz_map.push_back(std::make_pair("Si_wafer", 0.));
	dz_map.push_back(std::make_pair("Kapton_layer", 0.));
	dz_map.push_back(std::make_pair("CuW_baseplate", 0.));
	dz_map.push_back(std::make_pair("Cu_absorber_EE", 0.));

	dz_map.push_back(std::make_pair("W_absorber_EE", 0.71 * cm));
	dz_map.push_back(std::make_pair("Pb_absorber_EE", 0.53 * cm));
	dz_map.push_back(std::make_pair("Cu_absorber_EE", 0.22 * cm));
	dz_map.push_back(std::make_pair("Pb_absorber_EE", 0.79 * cm));
	dz_map.push_back(std::make_pair("Cu_absorber_EE", 0.13 * cm));
	dz_map.push_back(std::make_pair("Pb_absorber_EE", 0.27 * cm));
	dz_map.push_back(std::make_pair("Cu_absorber_EE", 0.35 * cm));
	dz_map.push_back(std::make_pair("Pb_absorber_EE", 0.21 * cm));

	//EE8
	dz_map.push_back(std::make_pair("PCB", 0.7 * cm));
	dz_map.push_back(std::make_pair("Si_wafer", 0.));
	dz_map.push_back(std::make_pair("Kapton_layer", 0.));
	dz_map.push_back(std::make_pair("CuW_baseplate", 0.));
	dz_map.push_back(std::make_pair("Cu_absorber_EE", 0.));

	dz_map.push_back(std::make_pair("Al_case", 4 * cm));


	//FH10
	dz_map.push_back(std::make_pair("PCB_DAISY", 0.4 * cm));
	dz_map.push_back(std::make_pair("Si_wafer_DAISY", 0.));
	dz_map.push_back(std::make_pair("Kapton_layer_DAISY", 0.));
	dz_map.push_back(std::make_pair("Cu_baseplate_DAISY", 0.));
	dz_map.push_back(std::make_pair("Cu_absorber_FH", 0.));

	dz_map.push_back(std::make_pair("Fe_absorber_FH", 1.85 * cm));

	//FH11
	dz_map.push_back(std::make_pair("PCB_DAISY", 0.44 * cm));
	dz_map.push_back(std::make_pair("Si_wafer_DAISY", 0.));
	dz_map.push_back(std::make_pair("Kapton_layer_DAISY", 0.));
	dz_map.push_back(std::make_pair("Cu_baseplate_DAISY", 0.));
	dz_map.push_back(std::make_pair("Cu_absorber_FH", 0.));

	dz_map.push_back(std::make_pair("Fe_absorber_FH", 1.8 * cm));

	//FH12
	dz_map.push_back(std::make_pair("PCB_DAISY", 0.4 * cm));
	dz_map.push_back(std::make_pair("Si_wafer_DAISY", 0.));
	dz_map.push_back(std::make_pair("Kapton_layer_DAISY", 0.));
	dz_map.push_back(std::make_pair("Cu_baseplate_DAISY", 0.));
	dz_map.push_back(std::make_pair("Cu_absorber_FH", 0.));

	dz_map.push_back(std::make_pair("Fe_absorber_FH", 1.8 * cm));

	//FH1
	dz_map.push_back(std::make_pair("PCB_DAISY", 0.7 * cm));
	dz_map.push_back(std::make_pair("Si_wafer_DAISY", 0.));
	dz_map.push_back(std::make_pair("Kapton_layer_DAISY", 0.));
	dz_map.push_back(std::make_pair("Cu_baseplate_DAISY", 0.));
	dz_map.push_back(std::make_pair("Cu_absorber_FH", 0.));

	dz_map.push_back(std::make_pair("Fe_absorber_FH", 1.86 * cm));

	//FH8
	dz_map.push_back(std::make_pair("PCB_DAISY", 0.4 * cm));
	dz_map.push_back(std::make_pair("Si_wafer_DAISY", 0.));
	dz_map.push_back(std::make_pair("Kapton_layer_DAISY", 0.));
	dz_map.push_back(std::make_pair("Cu_baseplate_DAISY", 0.));
	dz_map.push_back(std::make_pair("Cu_absorber_FH", 0.));

	dz_map.push_back(std::make_pair("Fe_absorber_FH", 1.7 * cm));

	//FH9
	dz_map.push_back(std::make_pair("PCB_DAISY", 0.7 * cm));
	dz_map.push_back(std::make_pair("Si_wafer_DAISY", 0.));
	dz_map.push_back(std::make_pair("Kapton_layer_DAISY", 0.));
	dz_map.push_back(std::make_pair("Cu_baseplate_DAISY", 0.));
	dz_map.push_back(std::make_pair("Cu_absorber_FH", 0.));

	dz_map.push_back(std::make_pair("Fe_absorber_FH", 5.2 * cm));
	dz_map.push_back(std::make_pair("Steel_case", 4.0 * cm));

	//FH7
	dz_map.push_back(std::make_pair("PCB_DAISY", 0.4 * cm));
	dz_map.push_back(std::make_pair("Si_wafer_DAISY", 0.));
	dz_map.push_back(std::make_pair("Kapton_layer_DAISY", 0.));
	dz_map.push_back(std::make_pair("Cu_baseplate_DAISY", 0.));
	dz_map.push_back(std::make_pair("Cu_absorber_FH", 0.));

	dz_map.push_back(std::make_pair("Fe_absorber_FH", 1.6 * cm));

	//FH6
	dz_map.push_back(std::make_pair("PCB_DAISY", 0.5 * cm));
	dz_map.push_back(std::make_pair("Si_wafer_DAISY", 0.));
	dz_map.push_back(std::make_pair("Kapton_layer_DAISY", 0.));
	dz_map.push_back(std::make_pair("Cu_baseplate_DAISY", 0.));
	dz_map.push_back(std::make_pair("Cu_absorber_FH", 0.));

	dz_map.push_back(std::make_pair("Fe_absorber_FH", 1.8 * cm));

	//FH2
	dz_map.push_back(std::make_pair("PCB_DAISY", 0.94 * cm));
	dz_map.push_back(std::make_pair("Si_wafer_DAISY", 0.));
	dz_map.push_back(std::make_pair("Kapton_layer_DAISY", 0.));
	dz_map.push_back(std::make_pair("Cu_baseplate_DAISY", 0.));
	dz_map.push_back(std::make_pair("Cu_absorber_FH", 0.));

	dz_map.push_back(std::make_pair("Fe_absorber_FH", 1.71 * cm));

	//FH3
	dz_map.push_back(std::make_pair("PCB_DAISY", 0.33 * cm));
	dz_map.push_back(std::make_pair("Si_wafer_DAISY", 0.));
	dz_map.push_back(std::make_pair("Kapton_layer_DAISY", 0.));
	dz_map.push_back(std::make_pair("Cu_baseplate_DAISY", 0.));
	dz_map.push_back(std::make_pair("Cu_absorber_FH", 0.));

	dz_map.push_back(std::make_pair("Fe_absorber_FH", 2.0 * cm));

	//FH4
	dz_map.push_back(std::make_pair("PCB_DAISY", 0.9 * cm));
	dz_map.push_back(std::make_pair("Si_wafer_DAISY", 0.));
	dz_map.push_back(std::make_pair("Kapton_layer_DAISY", 0.));
	dz_map.push_back(std::make_pair("Cu_baseplate_DAISY", 0.));
	dz_map.push_back(std::make_pair("Cu_absorber_FH", 0.));

	dz_map.push_back(std::make_pair("Fe_absorber_FH", 1.745 * cm));

	//FH5
	dz_map.push_back(std::make_pair("PCB_DAISY", 0.6 * cm));
	dz_map.push_back(std::make_pair("Si_wafer_DAISY", 0.));
	dz_map.push_back(std::make_pair("Kapton_layer_DAISY", 0.));
	dz_map.push_back(std::make_pair("Cu_baseplate_DAISY", 0.));
	dz_map.push_back(std::make_pair("Cu_absorber_FH", 0.));

	dz_map.push_back(std::make_pair("Steel_case", 3.5 * cm));


	dz_map.push_back(std::make_pair("Scintillator", 10 * cm));

}