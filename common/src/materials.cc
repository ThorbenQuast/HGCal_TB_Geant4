#include "materials.hh"


G4SubtractionSolid* HexagonPhysical(G4String name, G4double cellThickness, G4double cellSideLength) {
  G4double full_cellX = (2.) * cellSideLength;
  G4double full_cellY = sqrt(3.) * cellSideLength;
  G4Box* solidFullcell = new G4Box(name,                       //its name
                                   0.5 * full_cellX, 0.5 * full_cellY, 0.5 * cellThickness); //its size


  G4double deltaXDash = cellSideLength;
  G4double deltaYDash = sqrt(3) / 4 * cellSideLength;

  G4Box* solidCutcell = new G4Box(name,                       //its name
                                  0.5 * deltaXDash, 0.5 * (deltaYDash), 1.*cellThickness); //its size


  G4double DeltaTheta[4] = {30.*deg, 150.*deg, 210.*deg, 330.*deg};
  G4double DeltaTheta_rot[4] = {60.*deg, 120.*deg, 240 * deg, 300 * deg};
  G4double Delta = sqrt(3) / 2 * cellSideLength + deltaYDash / 2;

  G4RotationMatrix* rot = new G4RotationMatrix;
  rot->rotateZ(DeltaTheta_rot[0]);
  std::vector<G4SubtractionSolid*> subtracted;
  subtracted.push_back(new G4SubtractionSolid("cellSubtracted", solidFullcell, solidCutcell, rot, G4ThreeVector(cos(DeltaTheta[0])*Delta, sin(DeltaTheta[0])*Delta, 0.)));

  for (int i = 1; i < 4; i++) {
    rot->rotateZ(-DeltaTheta_rot[i - 1]);
    rot->rotateZ(DeltaTheta_rot[i]);
    subtracted.push_back(new G4SubtractionSolid("cellSubtracted", subtracted[i - 1], solidCutcell, rot, G4ThreeVector(cos(DeltaTheta[i])*Delta, sin(DeltaTheta[i])*Delta, 0.)));
  }
  delete rot;

  return subtracted[3];

}

G4LogicalVolume* HexagonLogical(G4String name, G4double cellThickness, G4double cellSideLength, G4Material* material) {
  return new G4LogicalVolume(HexagonPhysical(name, cellThickness, cellSideLength),          //its solid
                             material,           //its material
                             name);            //its name

}

void HGCalTBMaterials::DefineMaterials() {
  /***** Definition of all available materials *****/
  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();
  mat_Vacuum = nist->FindOrBuildMaterial("G4_Galactic");
  mat_AIR = nist->FindOrBuildMaterial("G4_AIR");
  mat_Ar = nist->FindOrBuildMaterial("G4_Ar");
  mat_Al = nist->FindOrBuildMaterial("G4_Al");
  mat_Fe = nist->FindOrBuildMaterial("G4_Fe");
  mat_Glass = nist->FindOrBuildMaterial("G4_GLASS_PLATE");
  mat_Steel = nist->FindOrBuildMaterial("G4_STAINLESS-STEEL");
  mat_Pb = nist->FindOrBuildMaterial("G4_Pb");
  mat_Cu = nist->FindOrBuildMaterial("G4_Cu");
  mat_W = nist->FindOrBuildMaterial("G4_W");
  mat_Si = nist->FindOrBuildMaterial("G4_Si");
  mat_KAPTON = nist->FindOrBuildMaterial("G4_KAPTON");
  mat_Au = nist->FindOrBuildMaterial("G4_Au");
  mat_PCB = nist->FindOrBuildMaterial("G4_C");
  mat_Quartz = nist->FindOrBuildMaterial("G4_SILICON_DIOXIDE");

  //AHCAL SiPMs
  G4double a = 1.01 * g / mole;
  G4Element* elH = new G4Element("Hydrogen", "H2", 1., a);
  a = 12.01 * g / mole;
  G4Element* elC = new G4Element("Carbon", "C", 6., a);
  G4double density = 1.032 * g / cm3;
  mat_Polystyrene = new G4Material("Polystyrene", density, 2);
  mat_Polystyrene->AddElement(elC, 19);
  mat_Polystyrene->AddElement(elH, 21);

  //CuW alloy: 60% Cu, 40% W in mass
  G4double Cu_frac_in_CuW = 0.6;
  mat_CuW = new G4Material("CuW", mat_Cu->GetDensity()*Cu_frac_in_CuW + mat_W->GetDensity() * (1 - Cu_frac_in_CuW), 2);
  mat_CuW->AddMaterial(mat_Cu, Cu_frac_in_CuW);
  mat_CuW->AddMaterial(mat_W, 1 - Cu_frac_in_CuW);


  //LYSO implementation from here: https://github.com/cacunas/ParG4_cluster/blob/master/LYSO/src/LYSOGeometry.cc
  G4Element* Oxigen   = nist->FindOrBuildElement("O");
  G4Element* Lutetium = nist->FindOrBuildElement("Lu");
  G4Element* Yttrium  = nist->FindOrBuildElement("Y");
  //Lutetium Oxide Lu2O3
  G4Material* LutetiumOxide = new G4Material("LutetiumOxide", 9.41 * g / cm3, 2);
  LutetiumOxide->AddElement(Lutetium, 2);
  LutetiumOxide->AddElement(Oxigen, 3);
  //Yttrium Oxide Y2O3
  G4Material* YttriumOxide = new G4Material("YttriumOxide", 5.01 * g / cm3, 2);
  YttriumOxide->AddElement(Yttrium, 2);
  YttriumOxide->AddElement(Oxigen, 3);
  //Build LYSO Material
  mat_LYSO = new G4Material("LYSO", 7.1 * g / cm3, 3);
  mat_LYSO->AddMaterial(LutetiumOxide,  81 * perCent);
  mat_LYSO->AddMaterial(mat_Quartz, 14 * perCent);
  mat_LYSO->AddMaterial(YttriumOxide,    5 * perCent);

}

void HGCalTBMaterials::setEventDisplayColorScheme() {
  visAttributes = new G4VisAttributes(G4Colour(.0, 0.0, 0.0));
  visAttributes->SetVisibility(false);
  Si_wafer_logical->SetVisAttributes(visAttributes);

  visAttributes = new G4VisAttributes(G4Colour(.3, 0.3, 0.3, 0.015));
  visAttributes->SetVisibility(true);
  Si_pixel_logical->SetVisAttributes(visAttributes);

  visAttributes = new G4VisAttributes(G4Colour(0.4, 0.4, 0.4, 0.01));
  visAttributes->SetVisibility(false);
  CuW_baseplate_logical->SetVisAttributes(visAttributes);

  visAttributes = new G4VisAttributes(G4Colour(0.4, 0.4, 0.4, 0.01));
  visAttributes->SetVisibility(false);
  Cu_baseplate_logical->SetVisAttributes(visAttributes);

  visAttributes = new G4VisAttributes(G4Colour(0.4, 0.4, 0.4, 0.01));
  visAttributes->SetVisibility(false);
  PCB_baseplate_logical->SetVisAttributes(visAttributes);

  visAttributes = new G4VisAttributes(G4Colour(0.4, 0.4, 0.4, 0.01));
  visAttributes->SetVisibility(false);
  Kapton_layer_logical->SetVisAttributes(visAttributes);

  visAttributes = new G4VisAttributes(G4Colour(0.4, 0.4, 0.4, 0.01));
  visAttributes->SetVisibility(true);
  Al_case_logical->SetVisAttributes(visAttributes);

  visAttributes = new G4VisAttributes(G4Colour(0.4, 0.4, 0.4, 0.01));
  visAttributes->SetVisibility(true);
  Steel_case_logical->SetVisAttributes(visAttributes);

  visAttributes = new G4VisAttributes(G4Colour(0.4, 0.4, 0.4, 0.01));
  visAttributes->SetVisibility(true);
  Pb_absorber_EE_logical->SetVisAttributes(visAttributes);

  visAttributes = new G4VisAttributes(G4Colour(0.4, 0.4, 0.4, 0.01));
  visAttributes->SetVisibility(true);
  Cu_absorber_EE_logical->SetVisAttributes(visAttributes);

  visAttributes = new G4VisAttributes(G4Colour(0.4, 0.4, 0.4, 0.01));
  visAttributes->SetVisibility(true);
  W_absorber_EE_logical->SetVisAttributes(visAttributes);
  W_2mm_absorber_EE_DESY2018_logical->SetVisAttributes(visAttributes);
  W_4mm_absorber_EE_DESY2018_logical->SetVisAttributes(visAttributes);

  visAttributes = new G4VisAttributes(G4Colour(0.4, 0.4, 0.4, 0.01));
  visAttributes->SetVisibility(true);
  Cu_absorber_FH_logical->SetVisAttributes(visAttributes);

  visAttributes = new G4VisAttributes(G4Colour(0.4, 0.4, 0.4, 0.01));
  visAttributes->SetVisibility(true);
  Fe_absorber_FH_logical->SetVisAttributes(visAttributes);

  visAttributes = new G4VisAttributes(G4Colour(0.4, 0.4, 0.4, 0.01));
  visAttributes->SetVisibility(false);
  AHCAL_SiPM_logical->SetVisAttributes(visAttributes);

  visAttributes = new G4VisAttributes(G4Colour(0.4, 0.4, 0.4, 0.01));
  visAttributes->SetVisibility(true);
  AHCAL_SiPM_2x2HUB_logical->SetVisAttributes(visAttributes);

  visAttributes = new G4VisAttributes(G4Colour(0.4, 0.4, 0.4, 0.01));
  visAttributes->SetVisibility(true);
  Al_absorber_AHCAL_logical->SetVisAttributes(visAttributes);

  visAttributes = new G4VisAttributes(G4Colour(0.4, 0.4, 0.4, 0.01));
  visAttributes->SetVisibility(false);
  PCB_AHCAL_logical->SetVisAttributes(visAttributes);

  visAttributes = new G4VisAttributes(G4Colour(0.4, 0.4, 0.4, 0.01));
  visAttributes->SetVisibility(false);
  Fe_absorber_AHCAL_logical->SetVisAttributes(visAttributes);

  visAttributes = new G4VisAttributes(G4Colour(1.0, 1.0, 1.0, 0.01));
  visAttributes->SetVisibility(false);
  scintillator_logical->SetVisAttributes(visAttributes);

  visAttributes = new G4VisAttributes(G4Colour(1.0, 1.0, 1.0, 0.01));
  visAttributes->SetVisibility(false);
  MCP_logical->SetVisAttributes(visAttributes);

  visAttributes = new G4VisAttributes(G4Colour(1.0, 1.0, 1.0, 0.7));
  visAttributes->SetVisibility(false);
  DWC_logical->SetVisAttributes(visAttributes);

  visAttributes = new G4VisAttributes(G4Colour(0.05, 0.05, 0.05, 0.0));
  visAttributes->SetVisibility(false);
  DWC_gas_logical->SetVisAttributes(visAttributes);

  visAttributes = new G4VisAttributes(G4Colour(1.0, 1.0, 1.0, 0.2));
  visAttributes->SetVisibility(true);
  DATURA_logical->SetVisAttributes(visAttributes);

  visAttributes = new G4VisAttributes(G4Colour(0.4, 0.4, 0.4, 0.01));
  visAttributes->SetVisibility(false);
  HERD_calorimeter_logical->SetVisAttributes(visAttributes);  
  visAttributes->SetVisibility(true);
  HERD_calorimeter_slab_logical->SetVisAttributes(visAttributes);  
}

void HGCalTBMaterials::setSimulationColorScheme() {
  visAttributes = new G4VisAttributes(G4Colour(.0, 0.0, 0.0));
  visAttributes->SetVisibility(false);
  Si_wafer_logical->SetVisAttributes(visAttributes);

  visAttributes = new G4VisAttributes(G4Colour(.3, 0.3, 0.3, 1.0));
  visAttributes->SetVisibility(true);
  Si_pixel_logical->SetVisAttributes(visAttributes);

  visAttributes = new G4VisAttributes(G4Colour(.5, 0.0, 0.5, 0.3));
  visAttributes->SetVisibility(true);
  CuW_baseplate_logical->SetVisAttributes(visAttributes);

  visAttributes = new G4VisAttributes(G4Colour(.1, 0.2, 0.5, 0.3));
  visAttributes->SetVisibility(true);
  Cu_baseplate_logical->SetVisAttributes(visAttributes);

  visAttributes = new G4VisAttributes(G4Colour(.0, 1., 0.0, 0.3));
  visAttributes->SetVisibility(true);
  PCB_baseplate_logical->SetVisAttributes(visAttributes);

  visAttributes = new G4VisAttributes(G4Colour(.4, 0.4, 0.0, 0.3));
  visAttributes->SetVisibility(true);
  Kapton_layer_logical->SetVisAttributes(visAttributes);

  visAttributes = new G4VisAttributes(G4Colour(0.4, 0.4, 0.4, 0.5));
  visAttributes->SetVisibility(true);
  Al_case_logical->SetVisAttributes(visAttributes);

  visAttributes = new G4VisAttributes(G4Colour(0.4, 0.4, 0.4, 0.5));
  visAttributes->SetVisibility(true);
  Steel_case_logical->SetVisAttributes(visAttributes);

  visAttributes = new G4VisAttributes(G4Colour(0.1, 0.4, 0.8, 0.1));
  visAttributes->SetVisibility(true);
  Pb_absorber_EE_logical->SetVisAttributes(visAttributes);

  visAttributes = new G4VisAttributes(G4Colour(.1, 0.2, 0.5, 0.1));
  visAttributes->SetVisibility(true);
  Cu_absorber_EE_logical->SetVisAttributes(visAttributes);

  visAttributes = new G4VisAttributes(G4Colour(.1, 0.2, 0.5, 0.1));
  visAttributes->SetVisibility(true);
  W_absorber_EE_logical->SetVisAttributes(visAttributes);
  W_2mm_absorber_EE_DESY2018_logical->SetVisAttributes(visAttributes);
  W_4mm_absorber_EE_DESY2018_logical->SetVisAttributes(visAttributes);

  visAttributes = new G4VisAttributes(G4Colour(.1, 0.2, 0.5, 0.1));
  visAttributes->SetVisibility(true);
  Cu_absorber_FH_logical->SetVisAttributes(visAttributes);

  visAttributes = new G4VisAttributes(G4Colour(0.4, 0.4, 0.4, 0.1));
  visAttributes->SetVisibility(true);
  Fe_absorber_FH_logical->SetVisAttributes(visAttributes);

  visAttributes = new G4VisAttributes(G4Colour(.3, 0.3, 0.3, 1.0));
  visAttributes->SetVisibility(true);
  AHCAL_SiPM_logical->SetVisAttributes(visAttributes);

  visAttributes = new G4VisAttributes(G4Colour(0.4, 0.4, 0.4, 0.5));
  visAttributes->SetVisibility(true);
  AHCAL_SiPM_2x2HUB_logical->SetVisAttributes(visAttributes);

  visAttributes = new G4VisAttributes(G4Colour(0.4, 0.4, 0.4, 0.5));
  visAttributes->SetVisibility(true);
  Al_absorber_AHCAL_logical->SetVisAttributes(visAttributes);

  visAttributes = new G4VisAttributes(G4Colour(.0, 1., 0.0, 0.3));
  visAttributes->SetVisibility(true);
  PCB_AHCAL_logical->SetVisAttributes(visAttributes);

  visAttributes = new G4VisAttributes(G4Colour(0.4, 0.4, 0.4, 0.1));
  visAttributes->SetVisibility(true);
  Fe_absorber_AHCAL_logical->SetVisAttributes(visAttributes);

  visAttributes = new G4VisAttributes(G4Colour(1.0, 1.0, 1.0, 0.5));
  visAttributes->SetVisibility(true);
  scintillator_logical->SetVisAttributes(visAttributes);

  visAttributes = new G4VisAttributes(G4Colour(1.0, 1.0, 1.0, 0.5));
  visAttributes->SetVisibility(true);
  MCP_logical->SetVisAttributes(visAttributes);

  visAttributes = new G4VisAttributes(G4Colour(1.0, 1.0, 1.0, 0.7));
  visAttributes->SetVisibility(true);
  DWC_logical->SetVisAttributes(visAttributes);

  visAttributes = new G4VisAttributes(G4Colour(0.05, 0.05, 0.05, 0.0));
  visAttributes->SetVisibility(true);
  DWC_gas_logical->SetVisAttributes(visAttributes);

  visAttributes = new G4VisAttributes(G4Colour(1.0, 1.0, 1.0, 0.7));
  visAttributes->SetVisibility(true);
  DATURA_logical->SetVisAttributes(visAttributes);

  visAttributes = new G4VisAttributes(G4Colour(0.1, 0.1, 0.1, 0.3));
  visAttributes->SetVisibility(false);
  HERD_calorimeter_logical->SetVisAttributes(visAttributes);  
  visAttributes->SetVisibility(true);
  HERD_calorimeter_slab_logical->SetVisAttributes(visAttributes);  
}


HGCalTBMaterials::HGCalTBMaterials() {
  DefineMaterials();

  defineSiWaferAndCells();
  defineHGCalBaseplates();
  defineHGCalCases();
  defineHGCalEEAbsorbers();
  defineHGCalFHAbsorbers();
  defineAHCALSiPM();
  defineAHCALAbsorbers();
  defineBeamLineElements();
  defineHERDCalorimeter();

};


void HGCalTBMaterials::defineSiWaferAndCells() {
  /***** Definition of silicon (wafer) sensors *****/
  //300 microns thickness only
  Si_pixel_sideLength = 0.6496345 * cm;
  Si_wafer_thickness = 0.3 * mm;
  alpha = 60. / 180. * M_PI;
  Si_wafer_sideLength = 11 * Si_pixel_sideLength;

  Si_wafer_logical = HexagonLogical("Si_wafer", Si_wafer_thickness, Si_wafer_sideLength, mat_AIR);

  //Silicon pixel setups
  double dx = 2 * sin(alpha) * Si_pixel_sideLength;
  double dy = Si_pixel_sideLength * (2. + 2 * cos(alpha));
  Si_pixel_logical = HexagonLogical("SiCell", Si_wafer_thickness, Si_pixel_sideLength, mat_Si);

  int index = 1;
  int nRows[11] = {7, 6, 7, 6, 5, 6, 5, 4, 5, 4, 3};
  for (int nC = 0; nC < 11; nC++) {
    for (int middle_index = 0; middle_index < nRows[nC]; middle_index++) {
      new G4PVPlacement(0, G4ThreeVector(dy * (middle_index - nRows[nC] / 2. + 0.5), nC * dx / 2, 0.), Si_pixel_logical, "SiCell", Si_wafer_logical, false, index++, true);
      if (nC <= 0) continue;
      new G4PVPlacement(0, G4ThreeVector(dy * (middle_index - nRows[nC] / 2. + 0.5), -nC * dx / 2, 0.), Si_pixel_logical, "SiCell", Si_wafer_logical, false, index++, true);
    }
  }

  thickness_map["Si_wafer"] = Si_wafer_thickness;
  logical_volume_map["Si_wafer"] = Si_wafer_logical;
}

void HGCalTBMaterials::defineHGCalBaseplates() {
  /***** Definition of all baseplates *****/
  //CuW
  G4double CuW_baseplate_thickness = 1.2 * mm;
  G4double CuW_baseplate_sideLength = 11 * Si_pixel_sideLength;
  CuW_baseplate_logical = HexagonLogical("CuW_baseplate", CuW_baseplate_thickness, CuW_baseplate_sideLength, mat_CuW);
  thickness_map["CuW_baseplate"] = CuW_baseplate_thickness;
  logical_volume_map["CuW_baseplate"] = CuW_baseplate_logical;

  //Cu
  G4double Cu_baseplate_thickness = 1.2 * mm;
  G4double Cu_baseplate_sideLength = 11 * Si_pixel_sideLength;
  Cu_baseplate_logical = HexagonLogical("Cu_baseplate", Cu_baseplate_thickness, Cu_baseplate_sideLength, mat_Cu);
  thickness_map["Cu_baseplate"] = Cu_baseplate_thickness;
  logical_volume_map["Cu_baseplate"] = Cu_baseplate_logical;

  //PCB
  G4double PCB_baseplate_thickness = 1.2 * mm;
  G4double PCB_baseplate_sideLength = 11 * Si_pixel_sideLength;
  PCB_baseplate_logical = HexagonLogical("PCB", PCB_baseplate_thickness, PCB_baseplate_sideLength, mat_PCB);
  thickness_map["PCB"] = PCB_baseplate_thickness;
  logical_volume_map["PCB"] = PCB_baseplate_logical;

  //Kapton layer
  G4double Kapton_layer_thickness = 0.05 * mm;
  G4double Kapton_layer_sideLength = 11 * Si_pixel_sideLength;
  Kapton_layer_logical = HexagonLogical("Kapton_layer", Kapton_layer_thickness, Kapton_layer_sideLength, mat_KAPTON);
  thickness_map["Kapton_layer"] = Kapton_layer_thickness;
  logical_volume_map["Kapton_layer"] = Kapton_layer_logical;
}


void HGCalTBMaterials::defineHGCalCases() {
  G4double Al_case_thickness = 2.6 * mm;
  G4double Al_case_xy = 40 * cm;
  G4Box* Al_case_solid = new G4Box("Al_case", 0.5 * Al_case_xy, 0.5 * Al_case_xy, 0.5 * Al_case_thickness);
  Al_case_logical = new G4LogicalVolume(Al_case_solid, mat_Al, "Al_case");
  thickness_map["Al_case"] = Al_case_thickness;
  logical_volume_map["Al_case"] = Al_case_logical;

  G4double Steel_case_thickness = 0.9 * cm;
  G4double Steel_case_xy = 60 * cm;
  G4Box* Steel_case_solid = new G4Box("Steel_case", 0.5 * Steel_case_xy, 0.5 * Steel_case_xy, 0.5 * Steel_case_thickness);
  Steel_case_logical = new G4LogicalVolume(Steel_case_solid, mat_Steel, "Steel_case");
  thickness_map["Steel_case"] = Steel_case_thickness;
  logical_volume_map["Steel_case"] = Steel_case_logical;
}

void HGCalTBMaterials::defineHGCalEEAbsorbers() {
  //defintion of absorber plates in the EE part
  G4double Pb_absorber_EE_thickness = 4.9 * mm;
  G4double Pb_absorber_EE_xy = 30 * cm;
  G4Box* Pb_absorber_EE_solid = new G4Box("Pb_absorber_EE", 0.5 * Pb_absorber_EE_xy, 0.5 * Pb_absorber_EE_xy, 0.5 * Pb_absorber_EE_thickness);
  Pb_absorber_EE_logical = new G4LogicalVolume(Pb_absorber_EE_solid, mat_Pb, "Pb_absorber_EE");
  thickness_map["Pb_absorber_EE"] = Pb_absorber_EE_thickness;
  logical_volume_map["Pb_absorber_EE"] = Pb_absorber_EE_logical;

  G4double Cu_absorber_EE_thickness = 6 * mm;
  G4double Cu_absorber_EE_xy = 30 * cm;
  G4Box* Cu_absorber_EE_solid = new G4Box("Cu_absorber_EE", 0.5 * Cu_absorber_EE_xy, 0.5 * Cu_absorber_EE_xy, 0.5 * Cu_absorber_EE_thickness);
  Cu_absorber_EE_logical = new G4LogicalVolume(Cu_absorber_EE_solid, mat_Cu, "Cu_absorber_EE");
  thickness_map["Cu_absorber_EE"] = Cu_absorber_EE_thickness;
  logical_volume_map["Cu_absorber_EE"] = Cu_absorber_EE_logical;

  G4double W_absorber_EE_thickness = 2.8 * mm;
  G4double W_absorber_EE_xy = 30 * cm;
  G4Box* W_absorber_EE_solid = new G4Box("W_absorber_EE", 0.5 * W_absorber_EE_xy, 0.5 * W_absorber_EE_xy, 0.5 * W_absorber_EE_thickness);
  W_absorber_EE_logical = new G4LogicalVolume(W_absorber_EE_solid, mat_W, "W_absorber_EE");
  thickness_map["W_absorber_EE"] = W_absorber_EE_thickness;
  logical_volume_map["W_absorber_EE"] = W_absorber_EE_logical;


  G4double W_2mm_absorber_EE_DESY2018_thickness = 2. * mm;
  G4double W_2mm_absorber_EE_DESY2018_xy = 15 * cm;
  G4Box* W_2mm_absorber_EE_DESY2018_solid = new G4Box("W_2mm_absorber_EE_DESY2018", 0.5 * W_2mm_absorber_EE_DESY2018_xy, 0.5 * W_2mm_absorber_EE_DESY2018_xy, 0.5 * W_2mm_absorber_EE_DESY2018_thickness);
  W_2mm_absorber_EE_DESY2018_logical = new G4LogicalVolume(W_2mm_absorber_EE_DESY2018_solid, mat_W, "W_2mm_absorber_EE_DESY2018");
  thickness_map["W_2mm_absorber_EE_DESY2018"] = W_2mm_absorber_EE_DESY2018_thickness;
  logical_volume_map["W_2mm_absorber_EE_DESY2018"] = W_2mm_absorber_EE_DESY2018_logical;


  G4double W_4mm_absorber_EE_DESY2018_thickness = 4. * mm;
  G4double W_4mm_absorber_EE_DESY2018_xy = 15 * cm;
  G4Box* W_4mm_absorber_EE_DESY2018_solid = new G4Box("W_4mm_absorber_EE_DESY2018", 0.5 * W_4mm_absorber_EE_DESY2018_xy, 0.5 * W_4mm_absorber_EE_DESY2018_xy, 0.5 * W_4mm_absorber_EE_DESY2018_thickness);
  W_4mm_absorber_EE_DESY2018_logical = new G4LogicalVolume(W_4mm_absorber_EE_DESY2018_solid, mat_W, "W_4mm_absorber_EE_DESY2018");
  thickness_map["W_4mm_absorber_EE_DESY2018"] = W_4mm_absorber_EE_DESY2018_thickness;
  logical_volume_map["W_4mm_absorber_EE_DESY2018"] = W_4mm_absorber_EE_DESY2018_logical;
}

void HGCalTBMaterials::defineHGCalFHAbsorbers() {
  //defintion of absorber plates in the FH part
  G4double Cu_absorber_FH_thickness = 6 * mm;
  G4double Cu_absorber_FH_xy = 50 * cm;
  G4Box* Cu_absorber_FH_solid = new G4Box("Cu_absorber_FH", 0.5 * Cu_absorber_FH_xy, 0.5 * Cu_absorber_FH_xy, 0.5 * Cu_absorber_FH_thickness);
  Cu_absorber_FH_logical = new G4LogicalVolume(Cu_absorber_FH_solid, mat_Cu, "Cu_absorber_FH");
  thickness_map["Cu_absorber_FH"] = Cu_absorber_FH_thickness;
  logical_volume_map["Cu_absorber_FH"] = Cu_absorber_FH_logical;

  G4double Fe_absorber_FH_thickness = 41 * mm;
  G4double Fe_absorber_FH_xy = 50 * cm;
  G4Box* Fe_absorber_FH_solid = new G4Box("Fe_absorber_FH", 0.5 * Fe_absorber_FH_xy, 0.5 * Fe_absorber_FH_xy, 0.5 * Fe_absorber_FH_thickness);
  Fe_absorber_FH_logical = new G4LogicalVolume(Fe_absorber_FH_solid, mat_Fe, "Fe_absorber_FH");
  thickness_map["Fe_absorber_FH"] = Fe_absorber_FH_thickness;
  logical_volume_map["Fe_absorber_FH"] = Fe_absorber_FH_logical;
}

void HGCalTBMaterials::defineAHCALSiPM() {
  G4double AHCAL_SiPM_thickness = 5.4 * mm;
  AHCAL_SiPM_xy = 3 * cm;
  AHCAL_SiPM_solid = new G4Box("AHCAL_SiPM", 0.5 * AHCAL_SiPM_xy, 0.5 * AHCAL_SiPM_xy, 0.5 * AHCAL_SiPM_thickness);
  AHCAL_SiPM_logical = new G4LogicalVolume(AHCAL_SiPM_solid, mat_Polystyrene, "AHCAL_SiPM");
  thickness_map["AHCAL_SiPM"] = AHCAL_SiPM_thickness;
  logical_volume_map["AHCAL_SiPM"] = AHCAL_SiPM_logical;


  G4double AHCAL_SiPM_2x2HUB_xy = 2 * 12 * AHCAL_SiPM_xy + 0.01 * mm;
  G4double AHCAL_SiPM_2x2HUB_thickness = AHCAL_SiPM_thickness + 0.01 * mm;
  G4Box* AHCAL_SiPM_2x2HUB_solid = new G4Box("AHCAL_SiPM_2x2HUB", 0.5 * AHCAL_SiPM_2x2HUB_xy, 0.5 * AHCAL_SiPM_2x2HUB_xy, 0.5 * AHCAL_SiPM_2x2HUB_thickness);
  AHCAL_SiPM_2x2HUB_logical = new G4LogicalVolume(AHCAL_SiPM_2x2HUB_solid, mat_AIR, "AHCAL_SiPM_2x2HUB");
  thickness_map["AHCAL_SiPM_2x2HUB"] = AHCAL_SiPM_2x2HUB_thickness;
  logical_volume_map["AHCAL_SiPM_2x2HUB"] = AHCAL_SiPM_2x2HUB_logical;
  int copy_counter = 0;
  for (float _dx = -11.5; _dx <= 11.5; _dx = _dx + 1.) for (float _dy = -11.5; _dy <= 11.5; _dy = _dy + 1.)
      new G4PVPlacement(0, G4ThreeVector(_dx * AHCAL_SiPM_xy, _dy * AHCAL_SiPM_xy, 0), AHCAL_SiPM_logical, "AHCAL_SiPM", AHCAL_SiPM_2x2HUB_logical, false, copy_counter++, true);
}

void HGCalTBMaterials::defineAHCALAbsorbers() {
  G4double Al_absorber_AHCAL_thickness = 1 * mm;
  G4double Al_absorber_AHCAL_xy = 2 * 12 * AHCAL_SiPM_xy;
  G4Box* Al_absorber_AHCAL_solid = new G4Box("Al_absorber_AHCAL", 0.5 * Al_absorber_AHCAL_xy, 0.5 * Al_absorber_AHCAL_xy, 0.5 * Al_absorber_AHCAL_thickness);
  Al_absorber_AHCAL_logical = new G4LogicalVolume(Al_absorber_AHCAL_solid, mat_Al, "Al_absorber_AHCAL");
  thickness_map["Al_absorber_AHCAL"] = Al_absorber_AHCAL_thickness;
  logical_volume_map["Al_absorber_AHCAL"] = Al_absorber_AHCAL_logical;

  G4double PCB_AHCAL_thickness = 1.2 * mm;
  G4double PCB_AHCAL_xy = 2 * 12 * AHCAL_SiPM_xy;
  G4Box* PCB_AHCAL_solid = new G4Box("PCB_AHCAL", 0.5 * PCB_AHCAL_xy, 0.5 * PCB_AHCAL_xy, 0.5 * PCB_AHCAL_thickness);
  PCB_AHCAL_logical = new G4LogicalVolume(PCB_AHCAL_solid, mat_PCB, "PCB_AHCAL");
  thickness_map["PCB_AHCAL"] = PCB_AHCAL_thickness;
  logical_volume_map["PCB_AHCAL"] = PCB_AHCAL_logical;

  G4double Fe_absorber_AHCAL_thickness = 17 * mm;
  G4double Fe_absorber_AHCAL_x = 80.8 * cm;
  G4double Fe_absorber_AHCAL_y = 65.7 * cm;
  G4Box* Fe_absorber_AHCAL_solid = new G4Box("Fe_absorber_AHCAL", 0.5 * Fe_absorber_AHCAL_x, 0.5 * Fe_absorber_AHCAL_y, 0.5 * Fe_absorber_AHCAL_thickness);
  Fe_absorber_AHCAL_logical = new G4LogicalVolume(Fe_absorber_AHCAL_solid, mat_Fe, "Fe_absorber_AHCAL");
  thickness_map["Fe_absorber_AHCAL"] = Fe_absorber_AHCAL_thickness;
  logical_volume_map["Fe_absorber_AHCAL"] = Fe_absorber_AHCAL_logical;
}

void HGCalTBMaterials::defineBeamLineElements() {
  /***** Definition of beam line elements *****/
  //scintillators
  G4double scintillator_thickness = 10 * mm;
  G4double scintillator_xy = 10 * cm;
  G4Box* scintillator_solid = new G4Box("Scintillator", 0.5 * scintillator_xy, 0.5 * scintillator_xy, 0.5 * scintillator_thickness);
  scintillator_logical = new G4LogicalVolume(scintillator_solid, mat_PCB, "Scintillator");
  thickness_map["Scintillator"] = scintillator_thickness;
  logical_volume_map["Scintillator"] = scintillator_logical;

  //MCPs = quartz disks
  G4double MCP_thickness = 10 * mm;
  G4double MCP_radius = 2 * cm;
  G4Tubs* MCP_solid = new G4Tubs("MCP", 0. , MCP_radius, MCP_thickness, 0, 360 * degree);
  MCP_logical = new G4LogicalVolume(MCP_solid, mat_Quartz, "MCP");
  thickness_map["MCP"] = MCP_thickness;
  logical_volume_map["MCP"] = MCP_logical;


  //DWC related material
  G4double DWC_thickness = 10 * mm;
  G4double DWC_xy = 10 * cm;
  G4Box* DWC_solid = new G4Box("DWC", 0.5 * DWC_xy, 0.5 * DWC_xy, 0.5 * DWC_thickness);
  DWC_logical = new G4LogicalVolume(DWC_solid, mat_Glass, "DWC");
  thickness_map["DWC"] = DWC_thickness;
  logical_volume_map["DWC"] = DWC_logical;

  G4double DWC_gas_thickness = DWC_thickness - 2 * mm;
  G4double DWC_gas_xy = DWC_xy - 2 * mm;
  G4Box * DWC_gas_solid = new G4Box("DWC_gas", 0.5 * DWC_gas_xy, 0.5 * DWC_gas_xy, 0.5 * DWC_gas_thickness);
  DWC_gas_logical = new G4LogicalVolume(DWC_gas_solid, mat_Ar, "DWC_gas");

  new G4PVPlacement(0, G4ThreeVector(0, 0., 0.), DWC_gas_logical, "DWC_gas", DWC_logical, false, 0, true);


  //DESY DATURA beam telescope
  G4double DATURA_thickness = 15 * mm;
  G4double DATURA_xy = 10 * cm;
  G4double DATURA_window_x = 2 * cm;
  G4double DATURA_window_y = 1 * cm;
  G4Box* DATURA_full_solid = new G4Box("DATURA_full", 0.5 * DATURA_xy, 0.5 * DATURA_xy, 0.5 * DATURA_thickness);
  G4Box* DATURA_cut_solid = new G4Box("DATURA_cut", 0.5 * DATURA_window_x, 0.5 * DATURA_window_y, 0.5 * DATURA_thickness);
  G4SubtractionSolid* DATURA_solid = new G4SubtractionSolid("DATURA", DATURA_full_solid, DATURA_cut_solid, 0, G4ThreeVector(0, 0, 0.));
  DATURA_logical = new G4LogicalVolume(DATURA_solid, mat_Al, "DATURA");

  thickness_map["DATURA"] = DATURA_thickness;
  logical_volume_map["DATURA"] = DATURA_logical;

}


void HGCalTBMaterials::defineHERDCalorimeter(){ 

  //details about HERD's calorimeter: 
  //http://herd.ihep.ac.cn, 07 December 2018
   
  G4double HERD_calorimeter_thickness = 3 * cm;
  G4double HERD_calorimeter_xy = 3 * cm;
  G4Box* HERD_calorimeter_solid = new G4Box("HERD_calorimeter", 0.5 * HERD_calorimeter_xy, 0.5 * HERD_calorimeter_xy, 0.5 * HERD_calorimeter_thickness);
  HERD_calorimeter_logical = new G4LogicalVolume(HERD_calorimeter_solid, mat_LYSO, "HERD_calorimeter");
  thickness_map["HERD_calorimeter"] = HERD_calorimeter_thickness;
  logical_volume_map["HERD_calorimeter"] = HERD_calorimeter_logical;


  G4double HERD_calorimeter_slab_xy = 21 * HERD_calorimeter_xy + 0.01 * mm;
  G4double HERD_calorimeter_slab_thickness = HERD_calorimeter_thickness + 0.0001 * mm;
  G4Box* HERD_calorimeter_slab_solid = new G4Box("HERD_calorimeter_slab", 0.5 * HERD_calorimeter_slab_xy, 0.5 * HERD_calorimeter_slab_xy, 0.5 * HERD_calorimeter_slab_thickness);
  HERD_calorimeter_slab_logical = new G4LogicalVolume(HERD_calorimeter_slab_solid, mat_AIR, "HERD_calorimeter_slab");
  thickness_map["HERD_calorimeter_slab"] = HERD_calorimeter_slab_thickness;
  logical_volume_map["HERD_calorimeter_slab"] = HERD_calorimeter_slab_logical;
  int copy_counter = 0;
  for (float _dx = -10.0; _dx <= 10.0; _dx = _dx + 1.) for (float _dy = -10.0; _dy <= 10.0; _dy = _dy + 1.) {
      new G4PVPlacement(0, G4ThreeVector(_dx * HERD_calorimeter_xy, _dy * HERD_calorimeter_xy, 0), HERD_calorimeter_logical, "HERD_calorimeter_slab", HERD_calorimeter_slab_logical, false, copy_counter++, true);  
  }
}



void HGCalTBMaterials::placeItemInLogicalVolume(std::string item_type, G4double &z0, G4LogicalVolume* logicWorld) {
  if (item_type.find("_DAISY") != std::string::npos) {
    item_type.resize(item_type.find("_DAISY"));
    if (copy_counter_map.find(item_type) == copy_counter_map.end()) copy_counter_map[item_type] = 0;
    double dx_ = 2 * sin(alpha) * 11 * Si_pixel_sideLength;
    double dy_ = 11 * Si_pixel_sideLength * (2. + 2 * cos(alpha));
    int nRows_[3] = {1, 2, 1};
    for (int nC = 0; nC < 3; nC++) {
      for (int middle_index = 0; middle_index < nRows_[nC]; middle_index++) {
        new G4PVPlacement(0, G4ThreeVector(dy_ * (middle_index - nRows_[nC] / 2. + 0.5) , -nC * dx_ / 2 , z0 + 0.5 * thickness_map[item_type]), logical_volume_map[item_type], item_type, logicWorld, false, copy_counter_map[item_type]++, true);
        if (nC <= 0) continue;
        new G4PVPlacement(0, G4ThreeVector(dy_ * (middle_index - nRows_[nC] / 2. + 0.5) , +nC * dx_ / 2 , z0 + 0.5 * thickness_map[item_type]), logical_volume_map[item_type], item_type, logicWorld, false, copy_counter_map[item_type]++, true);
      }
    }
    z0 += thickness_map[item_type];
  } else if (item_type.find("_SUMMER2017TRIPLET") != std::string::npos) {
    item_type.resize(item_type.find("_SUMMER2017TRIPLET"));
    if (copy_counter_map.find(item_type) == copy_counter_map.end()) copy_counter_map[item_type] = 0;
    double dx_ = 2 * sin(alpha) * 11 * Si_pixel_sideLength;
    double dy_ = 11 * Si_pixel_sideLength * (2. + 2 * cos(alpha));
    int nRows_[2] = {1, 2};
    for (int nC = 0; nC < 2; nC++) {
      new G4PVPlacement(0, G4ThreeVector(-dy_ * (0 - nRows_[nC] / 2. + 0.5) , -nC * dx_ / 2 , z0 + 0.5 * thickness_map[item_type]), logical_volume_map[item_type], item_type, logicWorld, false, copy_counter_map[item_type]++, true);
      if (nC <= 0) continue;
      new G4PVPlacement(0, G4ThreeVector(-dy_ * (0 - nRows_[nC] / 2. + 0.5) , +nC * dx_ / 2 , z0 + 0.5 * thickness_map[item_type]), logical_volume_map[item_type], item_type, logicWorld, false, copy_counter_map[item_type]++, true);
    }
    z0 += thickness_map[item_type];
  } else if (item_type.find("_rot30") != std::string::npos) {
    item_type.resize(item_type.find("_rot30"));
    if (copy_counter_map.find(item_type) == copy_counter_map.end()) copy_counter_map[item_type] = 0;
    G4RotationMatrix* rot = new G4RotationMatrix;
    rot->rotateZ(30 * deg);
    new G4PVPlacement(rot, G4ThreeVector(0, 0, z0 + 0.5 * thickness_map[item_type]), logical_volume_map[item_type], item_type, logicWorld, false, copy_counter_map[item_type]++, true);
    z0 += thickness_map[item_type];
  } else {
    if (copy_counter_map.find(item_type) == copy_counter_map.end()) copy_counter_map[item_type] = 0;
    new G4PVPlacement(0, G4ThreeVector(0, 0, z0 + 0.5 * thickness_map[item_type]), logical_volume_map[item_type], item_type, logicWorld, false, copy_counter_map[item_type]++, true);
    z0 += thickness_map[item_type];
  }
}

G4LogicalVolume* HGCalTBMaterials::newSiPixelHitFrameLogical(std::string name, G4double frame_thickness) {
  G4SubtractionSolid* geometry = new G4SubtractionSolid(name, HexagonPhysical(name+"_1", Si_wafer_thickness+1*mm, Si_pixel_sideLength+frame_thickness), HexagonPhysical(name+"_2", Si_wafer_thickness+1.1*mm, Si_pixel_sideLength), 0, G4ThreeVector(0, 0, 0));
  return new G4LogicalVolume(geometry, mat_Vacuum, name); 
}

G4LogicalVolume* HGCalTBMaterials::newSiPixelHitLogical(std::string name) {
  return HexagonLogical(name, Si_wafer_thickness, Si_pixel_sideLength, mat_Vacuum);
};

G4LogicalVolume* HGCalTBMaterials::newSiPMHitLogical(std::string name) {
  return new G4LogicalVolume(AHCAL_SiPM_solid, mat_Vacuum, name);
}