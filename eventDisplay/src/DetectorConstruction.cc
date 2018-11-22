
#include "DetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4SubtractionSolid.hh"
#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"

#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"


#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include <vector>
#include <cstdlib>
#include <string>
#include <cmath>



G4LogicalVolume* HexagonLogical(G4String name, G4double cellThickness, G4double cellSideLength, G4Material* material) {
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

  return new G4LogicalVolume(subtracted[3],          //its solid
                             material,           //its material
                             name);            //its name

}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
  : G4VUserDetectorConstruction(),
    fScoringVolume(0),
    _configuration(-1)
{

  DefineCommands();
  ntuplepath = "";
  m_inputFile = NULL;
  m_inputTree = NULL;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{

  // visual attributes
  G4VisAttributes* visAttributes;
  HGCalLayerDistances.clear();

  /***** Definition of all available materials *****/
  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();
  mat_Vacuum = nist->FindOrBuildMaterial("G4_Galactic");
  G4Material* mat_AIR = nist->FindOrBuildMaterial("G4_AIR");
  G4Material* mat_Ar = nist->FindOrBuildMaterial("G4_Ar");
  G4Material* mat_Al = nist->FindOrBuildMaterial("G4_Al");
  G4Material* mat_Fe = nist->FindOrBuildMaterial("G4_Fe");
  G4Material* mat_Glass = nist->FindOrBuildMaterial("G4_GLASS_PLATE");
  G4Material* mat_Steel = nist->FindOrBuildMaterial("G4_STAINLESS-STEEL");
  G4Material* mat_Pb = nist->FindOrBuildMaterial("G4_Pb");
  G4Material* mat_Cu = nist->FindOrBuildMaterial("G4_Cu");
  G4Material* mat_W = nist->FindOrBuildMaterial("G4_W");
  G4Material* mat_Si = nist->FindOrBuildMaterial("G4_Si");
  G4Material* mat_KAPTON = nist->FindOrBuildMaterial("G4_KAPTON");
  G4Material* mat_Au = nist->FindOrBuildMaterial("G4_Au");
  G4Material* mat_PCB = nist->FindOrBuildMaterial("G4_C");
  G4Material* mat_Quartz = nist->FindOrBuildMaterial("G4_SILICON_DIOXIDE");

  //AHCAL SiPMs
  G4double a = 1.01 * g / mole;
  G4Element* elH = new G4Element("Hydrogen", "H2", 1., a);
  a = 12.01 * g / mole;
  G4Element* elC = new G4Element("Carbon", "C", 6., a);
  G4double density = 1.032 * g / cm3;
  G4Material* mat_Polystyrene = new G4Material("Polystyrene", density, 2);
  mat_Polystyrene->AddElement(elC, 19);
  mat_Polystyrene->AddElement(elH, 21);

  //CuW alloy: 60% Cu, 40% W in mass
  G4double Cu_frac_in_CuW = 0.6;
  G4Material* mat_CuW = new G4Material("CuW", mat_Cu->GetDensity()*Cu_frac_in_CuW + mat_W->GetDensity() * (1 - Cu_frac_in_CuW), 2);
  mat_CuW->AddMaterial(mat_Cu, Cu_frac_in_CuW);
  mat_CuW->AddMaterial(mat_W, 1 - Cu_frac_in_CuW);


  /***** Definition of the world = beam line *****/
  beamLineLength = 36 * m;
  beamLineXY = 9 * m;

  // World = Beam line
  G4Box* solidWorld = new G4Box("World", 0.5 * beamLineXY, 0.5 * beamLineXY, 0.5 * beamLineLength);

  G4Material* world_mat = mat_AIR;
  logicWorld = new G4LogicalVolume(solidWorld, world_mat, "World");



  /***** Definition of silicon (wafer) sensors *****/
  //300 microns thickness only
  Si_pixel_sideLength = 0.6496345 * cm;
  Si_wafer_thickness = 0.3 * mm;
  alpha = 60. / 180. * M_PI;
  Si_wafer_sideLength = 11 * Si_pixel_sideLength;

  Si_wafer_logical = HexagonLogical("Si_wafer", Si_wafer_thickness, Si_wafer_sideLength, mat_AIR);
  visAttributes = new G4VisAttributes(G4Colour(.0, 0.0, 0.0));
  visAttributes->SetVisibility(false);
  Si_wafer_logical->SetVisAttributes(visAttributes);

  //Silicon pixel setups
  double dx = 2 * sin(alpha) * Si_pixel_sideLength;
  double dy = Si_pixel_sideLength * (2. + 2 * cos(alpha));
  Si_pixel_logical = HexagonLogical("SiCell", Si_wafer_thickness, Si_pixel_sideLength, mat_Si);
  visAttributes = new G4VisAttributes(G4Colour(.3, 0.3, 0.3, 0.015));
  visAttributes->SetVisibility(true);
  Si_pixel_logical->SetVisAttributes(visAttributes);

  //new G4PVPlacement(0, G4ThreeVector(0, 0., 0.), Si_pixel_logical, "SiCell", Si_wafer_logical, false, 0, true);
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

  /***** Definition of all baseplates *****/
  //CuW
  G4double CuW_baseplate_thickness = 1.2 * mm;
  G4double CuW_baseplate_sideLength = 11 * Si_pixel_sideLength;
  CuW_baseplate_logical = HexagonLogical("CuW_baseplate", CuW_baseplate_thickness, CuW_baseplate_sideLength, mat_CuW);
  visAttributes = new G4VisAttributes(G4Colour(0.4, 0.4, 0.4, 0.01));
  visAttributes->SetVisibility(false);
  CuW_baseplate_logical->SetVisAttributes(visAttributes);
  thickness_map["CuW_baseplate"] = CuW_baseplate_thickness;
  logical_volume_map["CuW_baseplate"] = CuW_baseplate_logical;

  //Cu
  G4double Cu_baseplate_thickness = 1.2 * mm;
  G4double Cu_baseplate_sideLength = 11 * Si_pixel_sideLength;
  Cu_baseplate_logical = HexagonLogical("Cu_baseplate", Cu_baseplate_thickness, Cu_baseplate_sideLength, mat_Cu);
  visAttributes = new G4VisAttributes(G4Colour(0.4, 0.4, 0.4, 0.01));
  visAttributes->SetVisibility(false);
  Cu_baseplate_logical->SetVisAttributes(visAttributes);
  thickness_map["Cu_baseplate"] = Cu_baseplate_thickness;
  logical_volume_map["Cu_baseplate"] = Cu_baseplate_logical;

  //PCB
  G4double PCB_baseplate_thickness = 1.2 * mm;
  G4double PCB_baseplate_sideLength = 11 * Si_pixel_sideLength;
  PCB_baseplate_logical = HexagonLogical("PCB", PCB_baseplate_thickness, PCB_baseplate_sideLength, mat_PCB);
  visAttributes = new G4VisAttributes(G4Colour(0.4, 0.4, 0.4, 0.01));
  visAttributes->SetVisibility(false);
  PCB_baseplate_logical->SetVisAttributes(visAttributes);
  thickness_map["PCB"] = PCB_baseplate_thickness;
  logical_volume_map["PCB"] = PCB_baseplate_logical;

  //Kapton layer
  G4double Kapton_layer_thickness = 0.05 * mm;
  G4double Kapton_layer_sideLength = 11 * Si_pixel_sideLength;
  Kapton_layer_logical = HexagonLogical("Kapton_layer", Kapton_layer_thickness, Kapton_layer_sideLength, mat_KAPTON);
  visAttributes = new G4VisAttributes(G4Colour(0.4, 0.4, 0.4, 0.01));
  visAttributes->SetVisibility(false);
  Kapton_layer_logical->SetVisAttributes(visAttributes);
  thickness_map["Kapton_layer"] = Kapton_layer_thickness;
  logical_volume_map["Kapton_layer"] = Kapton_layer_logical;

  //half Kapton layer
  G4double Kapton_half_layer_thickness = 0.025 * mm;
  G4double Kapton_half_layer_sideLength = 11 * Si_pixel_sideLength;
  Kapton_half_layer_logical = HexagonLogical("Kapton_half_layer", Kapton_half_layer_thickness, Kapton_half_layer_sideLength, mat_KAPTON);
  visAttributes = new G4VisAttributes(G4Colour(0.4, 0.4, 0.4, 0.01));
  visAttributes->SetVisibility(false);
  Kapton_half_layer_logical->SetVisAttributes(visAttributes);
  thickness_map["Kapton_half_layer"] = Kapton_half_layer_thickness;
  logical_volume_map["Kapton_half_layer"] = Kapton_half_layer_logical;

  //half Gold layer
  G4double Gold_half_layer_thickness = 0.025 * mm;
  G4double Gold_half_layer_sideLength = 11 * Si_pixel_sideLength;
  Gold_half_layer_logical = HexagonLogical("Gold_half_layer", Gold_half_layer_thickness, Gold_half_layer_sideLength, mat_Au);
  visAttributes = new G4VisAttributes(G4Colour(0.4, 0.4, 0.4, 0.01));
  visAttributes->SetVisibility(false);
  Gold_half_layer_logical->SetVisAttributes(visAttributes);
  thickness_map["Gold_half_layer"] = Gold_half_layer_thickness;
  logical_volume_map["Gold_half_layer"] = Gold_half_layer_logical;


  /***** Definition of absorber plates *****/
  G4double Al_case_thickness = 2.6 * mm;
  G4double Al_case_xy = 60 * cm;
  G4Box* Al_case_solid = new G4Box("Al_case", 0.5 * Al_case_xy, 0.5 * Al_case_xy, 0.5 * Al_case_thickness);
  Al_case_logical = new G4LogicalVolume(Al_case_solid, mat_Al, "Al_case");
  visAttributes = new G4VisAttributes(G4Colour(0.4, 0.4, 0.4, 0.01));
  visAttributes->SetVisibility(true);
  Al_case_logical->SetVisAttributes(visAttributes);
  thickness_map["Al_case"] = Al_case_thickness;
  logical_volume_map["Al_case"] = Al_case_logical;

  G4double Steel_case_thickness = 0.9 * cm;
  G4double Steel_case_xy = 60 * cm;
  G4Box* Steel_case_solid = new G4Box("Steel_case", 0.5 * Steel_case_xy, 0.5 * Steel_case_xy, 0.5 * Steel_case_thickness);
  Steel_case_logical = new G4LogicalVolume(Steel_case_solid, mat_Steel, "Steel_case");
  visAttributes = new G4VisAttributes(G4Colour(0.4, 0.4, 0.4, 0.01));
  visAttributes->SetVisibility(true);
  Steel_case_logical->SetVisAttributes(visAttributes);
  thickness_map["Steel_case"] = Steel_case_thickness;
  logical_volume_map["Steel_case"] = Steel_case_logical;

  //defintion of absorber plates in the EE part
  G4double Pb_absorber_EE_thickness = 4.9 * mm;
  G4double Pb_absorber_EE_xy = 30 * cm;
  G4Box* Pb_absorber_EE_solid = new G4Box("Pb_absorber_EE", 0.5 * Pb_absorber_EE_xy, 0.5 * Pb_absorber_EE_xy, 0.5 * Pb_absorber_EE_thickness);
  Pb_absorber_EE_logical = new G4LogicalVolume(Pb_absorber_EE_solid, mat_Pb, "Pb_absorber_EE");
  visAttributes = new G4VisAttributes(G4Colour(0.4, 0.4, 0.4, 0.01));
  visAttributes->SetVisibility(true);
  Pb_absorber_EE_logical->SetVisAttributes(visAttributes);
  thickness_map["Pb_absorber_EE"] = Pb_absorber_EE_thickness;
  logical_volume_map["Pb_absorber_EE"] = Pb_absorber_EE_logical;

  G4double Cu_absorber_EE_thickness = 6 * mm;
  G4double Cu_absorber_EE_xy = 30 * cm;
  G4Box* Cu_absorber_EE_solid = new G4Box("Cu_absorber_EE", 0.5 * Cu_absorber_EE_xy, 0.5 * Cu_absorber_EE_xy, 0.5 * Cu_absorber_EE_thickness);
  Cu_absorber_EE_logical = new G4LogicalVolume(Cu_absorber_EE_solid, mat_Cu, "Cu_absorber_EE");
  visAttributes = new G4VisAttributes(G4Colour(0.4, 0.4, 0.4, 0.01));
  visAttributes->SetVisibility(true);
  Cu_absorber_EE_logical->SetVisAttributes(visAttributes);
  thickness_map["Cu_absorber_EE"] = Cu_absorber_EE_thickness;
  logical_volume_map["Cu_absorber_EE"] = Cu_absorber_EE_logical;

  //defintion of absorber plates in the FH part
  G4double Cu_absorber_FH_thickness = 6 * mm;
  G4double Cu_absorber_FH_xy = 50 * cm;
  G4Box* Cu_absorber_FH_solid = new G4Box("Cu_absorber_FH", 0.5 * Cu_absorber_FH_xy, 0.5 * Cu_absorber_FH_xy, 0.5 * Cu_absorber_FH_thickness);
  Cu_absorber_FH_logical = new G4LogicalVolume(Cu_absorber_FH_solid, mat_Cu, "Cu_absorber_FH");
  visAttributes = new G4VisAttributes(G4Colour(0.4, 0.4, 0.4, 0.01));
  visAttributes->SetVisibility(true);
  Cu_absorber_FH_logical->SetVisAttributes(visAttributes);
  thickness_map["Cu_absorber_FH"] = Cu_absorber_FH_thickness;
  logical_volume_map["Cu_absorber_FH"] = Cu_absorber_FH_logical;

  G4double Fe_absorber_FH_thickness = 41 * mm;
  G4double Fe_absorber_FH_xy = 50 * cm;
  G4Box* Fe_absorber_FH_solid = new G4Box("Fe_absorber_FH", 0.5 * Fe_absorber_FH_xy, 0.5 * Fe_absorber_FH_xy, 0.5 * Fe_absorber_FH_thickness);
  Fe_absorber_FH_logical = new G4LogicalVolume(Fe_absorber_FH_solid, mat_Fe, "Fe_absorber_FH");
  visAttributes = new G4VisAttributes(G4Colour(0.4, 0.4, 0.4, 0.01));
  visAttributes->SetVisibility(true);
  Fe_absorber_FH_logical->SetVisAttributes(visAttributes);
  thickness_map["Fe_absorber_FH"] = Fe_absorber_FH_thickness;
  logical_volume_map["Fe_absorber_FH"] = Fe_absorber_FH_logical;


  /***** Definition of AHCAL elements *****/
  G4double AHCAL_SiPM_thickness = 5.4 * mm;
  AHCAL_SiPM_xy = 3 * cm;
  G4Box* AHCAL_SiPM_solid = new G4Box("AHCAL_SiPM", 0.5 * AHCAL_SiPM_xy, 0.5 * AHCAL_SiPM_xy, 0.5 * AHCAL_SiPM_thickness);
  AHCAL_SiPM_logical = new G4LogicalVolume(AHCAL_SiPM_solid, mat_Polystyrene, "AHCAL_SiPM");
  visAttributes = new G4VisAttributes(G4Colour(0.4, 0.4, 0.4, 0.01));
  visAttributes->SetVisibility(true);
  AHCAL_SiPM_logical->SetVisAttributes(visAttributes);
  thickness_map["AHCAL_SiPM"] = AHCAL_SiPM_thickness;
  logical_volume_map["AHCAL_SiPM"] = AHCAL_SiPM_logical;


  G4double AHCAL_SiPM_2x2HUB_xy = 2 * 12 * AHCAL_SiPM_xy + 0.01 * mm;
  G4double AHCAL_SiPM_2x2HUB_thickness = AHCAL_SiPM_thickness + 0.01 * mm;
  G4Box* AHCAL_SiPM_2x2HUB_solid = new G4Box("AHCAL_SiPM_2x2HUB", 0.5 * AHCAL_SiPM_2x2HUB_xy, 0.5 * AHCAL_SiPM_2x2HUB_xy, 0.5 * AHCAL_SiPM_2x2HUB_thickness);
  AHCAL_SiPM_2x2HUB_logical = new G4LogicalVolume(AHCAL_SiPM_2x2HUB_solid, mat_AIR, "AHCAL_SiPM_2x2HUB");
  visAttributes = new G4VisAttributes(G4Colour(0.4, 0.4, 0.4, 0.01));
  visAttributes->SetVisibility(true);
  AHCAL_SiPM_2x2HUB_logical->SetVisAttributes(visAttributes);
  thickness_map["AHCAL_SiPM_2x2HUB"] = AHCAL_SiPM_2x2HUB_thickness;
  logical_volume_map["AHCAL_SiPM_2x2HUB"] = AHCAL_SiPM_2x2HUB_logical;
  int copy_counter = 0;
  for (float _dx = -11.5; _dx <= 11.5; _dx = _dx + 1.) for (float _dy = -11.5; _dy <= 11.5; _dy = _dy + 1.)
      new G4PVPlacement(0, G4ThreeVector(_dx * AHCAL_SiPM_xy, _dy * AHCAL_SiPM_xy, 0), AHCAL_SiPM_logical, "AHCAL_SiPM", AHCAL_SiPM_2x2HUB_logical, false, copy_counter++, true);




  G4double Al_absorber_AHCAL_thickness = 1 * mm;
  G4double Al_absorber_AHCAL_xy = 2 * 12 * AHCAL_SiPM_xy;
  G4Box* Al_absorber_AHCAL_solid = new G4Box("Al_absorber_AHCAL", 0.5 * Al_absorber_AHCAL_xy, 0.5 * Al_absorber_AHCAL_xy, 0.5 * Al_absorber_AHCAL_thickness);
  Al_absorber_AHCAL_logical = new G4LogicalVolume(Al_absorber_AHCAL_solid, mat_Al, "Al_absorber_AHCAL");
  visAttributes = new G4VisAttributes(G4Colour(0.4, 0.4, 0.4, 0.01));
  visAttributes->SetVisibility(true);
  Al_absorber_AHCAL_logical->SetVisAttributes(visAttributes);
  thickness_map["Al_absorber_AHCAL"] = Al_absorber_AHCAL_thickness;
  logical_volume_map["Al_absorber_AHCAL"] = Al_absorber_AHCAL_logical;

  G4double PCB_AHCAL_thickness = 1.2 * mm;
  G4double PCB_AHCAL_xy = 2 * 12 * AHCAL_SiPM_xy;
  G4Box* PCB_AHCAL_solid = new G4Box("PCB_AHCAL", 0.5 * PCB_AHCAL_xy, 0.5 * PCB_AHCAL_xy, 0.5 * PCB_AHCAL_thickness);
  PCB_AHCAL_logical = new G4LogicalVolume(PCB_AHCAL_solid, mat_PCB, "PCB_AHCAL");
  visAttributes = new G4VisAttributes(G4Colour(0.4, 0.4, 0.4, 0.01));
  visAttributes->SetVisibility(true);
  PCB_AHCAL_logical->SetVisAttributes(visAttributes);
  thickness_map["PCB_AHCAL"] = PCB_AHCAL_thickness;
  logical_volume_map["PCB_AHCAL"] = PCB_AHCAL_logical;

  G4double Fe_absorber_AHCAL_thickness = 17 * mm;
  G4double Fe_absorber_AHCAL_x = 80.8 * cm;
  G4double Fe_absorber_AHCAL_y = 65.7 * cm;
  G4Box* Fe_absorber_AHCAL_solid = new G4Box("Fe_absorber_AHCAL", 0.5 * Fe_absorber_AHCAL_x, 0.5 * Fe_absorber_AHCAL_y, 0.5 * Fe_absorber_AHCAL_thickness);
  Fe_absorber_AHCAL_logical = new G4LogicalVolume(Fe_absorber_AHCAL_solid, mat_Fe, "Fe_absorber_AHCAL");
  visAttributes = new G4VisAttributes(G4Colour(0.4, 0.4, 0.4, 0.01));
  visAttributes->SetVisibility(true);
  Fe_absorber_AHCAL_logical->SetVisAttributes(visAttributes);
  thickness_map["Fe_absorber_AHCAL"] = Fe_absorber_AHCAL_thickness;
  logical_volume_map["Fe_absorber_AHCAL"] = Fe_absorber_AHCAL_logical;


  /***** Definition of beam line elements *****/
  //scintillators
  G4double scintillator_thickness = 10 * mm;
  G4double scintillator_xy = 10 * cm;
  G4Box* scintillator_solid = new G4Box("Scintillator", 0.5 * scintillator_xy, 0.5 * scintillator_xy, 0.5 * scintillator_thickness);
  scintillator_logical = new G4LogicalVolume(scintillator_solid, mat_PCB, "Scintillator");
  visAttributes = new G4VisAttributes(G4Colour(1.0, 1.0, 1.0, 0.01));
  visAttributes->SetVisibility(false);
  scintillator_logical->SetVisAttributes(visAttributes);
  thickness_map["Scintillator"] = scintillator_thickness;
  logical_volume_map["Scintillator"] = scintillator_logical;

  //MCPs = quartz disks
  G4double MCP_thickness = 10 * mm;
  G4double MCP_radius = 2 * cm;
  G4Tubs* MCP_solid = new G4Tubs("MCP", 0. , MCP_radius, MCP_thickness, 0, 360 * degree);
  MCP_logical = new G4LogicalVolume(MCP_solid, mat_Quartz, "MCP");
  visAttributes = new G4VisAttributes(G4Colour(1.0, 1.0, 1.0, 0.01));
  visAttributes->SetVisibility(false);
  MCP_logical->SetVisAttributes(visAttributes);
  thickness_map["MCP"] = MCP_thickness;
  logical_volume_map["MCP"] = MCP_logical;


  //DWC related material
  G4double DWC_thickness = 10 * mm;
  G4double DWC_xy = 10 * cm;
  G4Box* DWC_solid = new G4Box("DWC", 0.5 * DWC_xy, 0.5 * DWC_xy, 0.5 * DWC_thickness);
  DWC_logical = new G4LogicalVolume(DWC_solid, mat_Glass, "DWC");
  visAttributes = new G4VisAttributes(G4Colour(1.0, 1.0, 1.0, 0.7));
  visAttributes->SetVisibility(false);
  DWC_logical->SetVisAttributes(visAttributes);
  thickness_map["DWC"] = DWC_thickness;
  logical_volume_map["DWC"] = DWC_logical;

  G4double DWC_gas_thickness = DWC_thickness - 2 * mm;
  G4double DWC_gas_xy = DWC_xy - 2 * mm;
  G4Box * DWC_gas_solid = new G4Box("DWC_gas", 0.5 * DWC_gas_xy, 0.5 * DWC_gas_xy, 0.5 * DWC_gas_thickness);
  DWC_gas_logical = new G4LogicalVolume(DWC_gas_solid, mat_Ar, "DWC_gas");
  visAttributes = new G4VisAttributes(G4Colour(0.05, 0.05, 0.05, 0.0));
  visAttributes->SetVisibility(false);
  DWC_gas_logical->SetVisAttributes(visAttributes);


  new G4PVPlacement(0,                     //no rotation
                    G4ThreeVector(0, 0., 0.),       //at (0,0,0)
                    DWC_gas_logical,            //its logical volume
                    "DWC_gas",               //its name
                    DWC_logical,                     //its mother  volume
                    false,                 //no boolean operation
                    0,                     //copy number
                    true);        //overlaps checking



  /****  START OF TEST ****/
  G4VPhysicalVolume* physWorld =
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(0., 0., 0.),       //at (0,0,0)
                      logicWorld,            //its logical volume
                      "World",               //its name
                      0,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      true);        //overlaps checking


  /****  END OF TEST ****/


  fScoringVolume = logicWorld;


  return physWorld;
}


void DetectorConstruction::ConstructHGCal() {
  /***** Start the placement *****/
  //
  //
  // Option to switch on/off checking of volumes overlaps
  //


  std::vector<std::pair<std::string, G4double> > dz_map;

  G4double z0 = -beamLineLength / 2.;

  if (_configuration == 22 || _configuration == 221) {
    dz_map.push_back(std::make_pair("DWC", 0.0 * m));
    dz_map.push_back(std::make_pair("DWC", 2.0 * m));
    dz_map.push_back(std::make_pair("DWC", 0.3 * m));
    dz_map.push_back(std::make_pair("Scintillator", 1.5 * m));
    dz_map.push_back(std::make_pair("DWC", 0.3 * m));
    dz_map.push_back(std::make_pair("DWC", 15. * m));
    dz_map.push_back(std::make_pair("DWC", 7. * m));

    dz_map.push_back(std::make_pair("Scintillator", 0.3 * m));
    dz_map.push_back(std::make_pair("Scintillator", 2.0 * m));

    if (_configuration == 22) dz_map.push_back(std::make_pair("Al_case", 0.1 * m));
    else {
      dz_map.push_back(std::make_pair("MCP", 0.0 * m));
      dz_map.push_back(std::make_pair("MCP", 0.0 * m));
      dz_map.push_back(std::make_pair("Al_case", 0.1 * m - 2 * thickness_map["MCP"]));
    }


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

    //beginning of FH
    dz_map.push_back(std::make_pair("Steel_case", 3.5 * cm));

    //FH6, orientation correct?
    dz_map.push_back(std::make_pair("PCB_DAISY", 0.3 * cm));
    dz_map.push_back(std::make_pair("Si_wafer_DAISY", 0.));
    dz_map.push_back(std::make_pair("Kapton_layer_DAISY", 0.));
    dz_map.push_back(std::make_pair("Cu_baseplate_DAISY", 0.));
    dz_map.push_back(std::make_pair("Cu_absorber_FH", 0.));

    dz_map.push_back(std::make_pair("Fe_absorber_FH", 1.8 * cm));

    //FH3, orientation correct?
    dz_map.push_back(std::make_pair("PCB_DAISY", 0.3 * cm));
    dz_map.push_back(std::make_pair("Si_wafer_DAISY", 0.));
    dz_map.push_back(std::make_pair("Kapton_layer_DAISY", 0.));
    dz_map.push_back(std::make_pair("Cu_baseplate_DAISY", 0.));
    dz_map.push_back(std::make_pair("Cu_absorber_FH", 0.));

    dz_map.push_back(std::make_pair("Fe_absorber_FH", 1.3 * cm));

    //FH2, orientation correct?
    dz_map.push_back(std::make_pair("PCB_DAISY", 0.8 * cm));
    dz_map.push_back(std::make_pair("Si_wafer_DAISY", 0.));
    dz_map.push_back(std::make_pair("Kapton_layer_DAISY", 0.));
    dz_map.push_back(std::make_pair("Cu_baseplate_DAISY", 0.));
    dz_map.push_back(std::make_pair("Cu_absorber_FH", 0.));

    dz_map.push_back(std::make_pair("Fe_absorber_FH", 1.5 * cm));

    //FH5, orientation correct?
    dz_map.push_back(std::make_pair("PCB_DAISY", 0.7 * cm));
    dz_map.push_back(std::make_pair("Si_wafer_DAISY", 0.));
    dz_map.push_back(std::make_pair("Kapton_layer_DAISY", 0.));
    dz_map.push_back(std::make_pair("Cu_baseplate_DAISY", 0.));
    dz_map.push_back(std::make_pair("Cu_absorber_FH", 0.));

    dz_map.push_back(std::make_pair("Fe_absorber_FH", 1.7 * cm));

    //FH8, orientation correct?
    dz_map.push_back(std::make_pair("PCB_DAISY", 0.4 * cm));
    dz_map.push_back(std::make_pair("Si_wafer_DAISY", 0.));
    dz_map.push_back(std::make_pair("Kapton_layer_DAISY", 0.));
    dz_map.push_back(std::make_pair("Cu_baseplate_DAISY", 0.));
    dz_map.push_back(std::make_pair("Cu_absorber_FH", 0.));

    dz_map.push_back(std::make_pair("Fe_absorber_FH", 1.6 * cm));

    //FH9, orientation correct?
    dz_map.push_back(std::make_pair("PCB_DAISY", 0.5 * cm));
    dz_map.push_back(std::make_pair("Si_wafer_DAISY", 0.));
    dz_map.push_back(std::make_pair("Kapton_layer_DAISY", 0.));
    dz_map.push_back(std::make_pair("Cu_baseplate_DAISY", 0.));
    dz_map.push_back(std::make_pair("Cu_absorber_FH", 0.));


    dz_map.push_back(std::make_pair("Steel_case", 1.4 * cm));
    dz_map.push_back(std::make_pair("Fe_absorber_FH", 3.6 * cm));
    dz_map.push_back(std::make_pair("Steel_case", 5.2 * cm));


    //FH7, orientation correct?
    dz_map.push_back(std::make_pair("PCB_DAISY", 0.3 * cm));
    dz_map.push_back(std::make_pair("Si_wafer_DAISY", 0.));
    dz_map.push_back(std::make_pair("Kapton_layer_DAISY", 0.));
    dz_map.push_back(std::make_pair("Cu_baseplate_DAISY", 0.));
    dz_map.push_back(std::make_pair("Cu_absorber_FH", 0.));

    dz_map.push_back(std::make_pair("Fe_absorber_FH", 1.7 * cm));

    //FH1, orientation correct?
    dz_map.push_back(std::make_pair("PCB_DAISY", 1.1 * cm));
    dz_map.push_back(std::make_pair("Si_wafer_DAISY", 0.));
    dz_map.push_back(std::make_pair("Kapton_layer_DAISY", 0.));
    dz_map.push_back(std::make_pair("Cu_baseplate_DAISY", 0.));
    dz_map.push_back(std::make_pair("Cu_absorber_FH", 0.));

    dz_map.push_back(std::make_pair("Fe_absorber_FH", 1.9 * cm));

    //FH2, orientation correct?
    dz_map.push_back(std::make_pair("PCB_DAISY", 0.9 * cm));
    dz_map.push_back(std::make_pair("Si_wafer_DAISY", 0.));
    dz_map.push_back(std::make_pair("Kapton_layer_DAISY", 0.));
    dz_map.push_back(std::make_pair("Cu_baseplate_DAISY", 0.));
    dz_map.push_back(std::make_pair("Cu_absorber_FH", 0.));

    dz_map.push_back(std::make_pair("Fe_absorber_FH", 2.0 * cm));

    //FH10, orientation correct?
    dz_map.push_back(std::make_pair("PCB", 1.1 * cm));
    dz_map.push_back(std::make_pair("Si_wafer", 0.));
    dz_map.push_back(std::make_pair("Kapton_layer", 0.));
    dz_map.push_back(std::make_pair("Cu_baseplate", 0.));
    dz_map.push_back(std::make_pair("Cu_absorber_FH", 0.));

    dz_map.push_back(std::make_pair("Fe_absorber_FH", 1.8 * cm));

    //FH11, orientation correct?
    dz_map.push_back(std::make_pair("PCB", 1.0 * cm));
    dz_map.push_back(std::make_pair("Si_wafer", 0.));
    dz_map.push_back(std::make_pair("Kapton_layer", 0.));
    dz_map.push_back(std::make_pair("Cu_baseplate", 0.));
    dz_map.push_back(std::make_pair("Cu_absorber_FH", 0.));

    dz_map.push_back(std::make_pair("Fe_absorber_FH", 1.7 * cm));

    //FH12, orientation correct?
    dz_map.push_back(std::make_pair("PCB", 1.0 * cm));
    dz_map.push_back(std::make_pair("Si_wafer", 0.));
    dz_map.push_back(std::make_pair("Kapton_layer", 0.));
    dz_map.push_back(std::make_pair("Cu_baseplate", 0.));
    dz_map.push_back(std::make_pair("Cu_absorber_FH", 0.));

    dz_map.push_back(std::make_pair("Steel_case", 1.9 * cm));

    /*
    //AHCAL
    dz_map.push_back(std::make_pair("Fe_absorber_AHCAL", 50.0 * cm));
    for (int l = 0; l < 39; l++) {
      dz_map.push_back(std::make_pair("Al_absorber_AHCAL", 0.5 * cm));
      dz_map.push_back(std::make_pair("AHCAL_SiPM_2x2HUB", 0.));
      dz_map.push_back(std::make_pair("Al_absorber_AHCAL", 0.));
      dz_map.push_back(std::make_pair("Fe_absorber_AHCAL", 0.5 * cm));
    }
    dz_map.push_back(std::make_pair("Fe_absorber_AHCAL", 1.1 * cm));
    dz_map.push_back(std::make_pair("Fe_absorber_AHCAL", 1.1 * cm));
    dz_map.push_back(std::make_pair("Al_absorber_AHCAL", 0.5 * cm));
    dz_map.push_back(std::make_pair("AHCAL_SiPM_2x2HUB", 0.));
    dz_map.push_back(std::make_pair("Al_absorber_AHCAL", 0.));
    dz_map.push_back(std::make_pair("Fe_absorber_AHCAL", 0.5 * cm));
    */
  }



  /*****    START GENERIC PLACEMENT ALGORITHM  FOR THE SETUP  *****/
  std::map<std::string, int> copy_counter_map;
  for (size_t item_index = 0; item_index < dz_map.size(); item_index++) {
    std::string item_type = dz_map[item_index].first;
    G4double dz = dz_map[item_index].second;
    z0 += dz;

    std::cout << "Placing " << item_type << " at position z [mm]=" << z0 / mm << std::endl;
    if (item_type.find("Si_wafer") != std::string::npos) {
      HGCalLayerDistances.push_back(z0);
    }
    if (item_type.find("_DAISY") != std::string::npos) {
      item_type.resize(item_type.find("_DAISY"));
      if (copy_counter_map.find(item_type) == copy_counter_map.end()) copy_counter_map[item_type] = 0;
      double dx_ = 2 * sin(alpha) * 11 * Si_pixel_sideLength;
      double dy_ = 11 * Si_pixel_sideLength * (2. + 2 * cos(alpha));
      int nRows_[3] = {1, 2, 1};
      for (int nC = 0; nC < 3; nC++) {
        for (int middle_index = 0; middle_index < nRows_[nC]; middle_index++) {
          new G4PVPlacement(0, G4ThreeVector(dy_ * (middle_index - nRows_[nC] / 2. + 0.5), -nC * dx_ / 2, z0 + 0.5 * thickness_map[item_type]), logical_volume_map[item_type], item_type, logicWorld, false, copy_counter_map[item_type]++, true);
          if (nC <= 0) continue;
          new G4PVPlacement(0, G4ThreeVector(dy_ * (middle_index - nRows_[nC] / 2. + 0.5), +nC * dx_ / 2, z0 + 0.5 * thickness_map[item_type]), logical_volume_map[item_type], item_type, logicWorld, false, copy_counter_map[item_type]++, true);
        }
      }
      z0 += thickness_map[item_type];
    } else {
      if (copy_counter_map.find(item_type) == copy_counter_map.end()) copy_counter_map[item_type] = 0;
      new G4PVPlacement(0, G4ThreeVector(0., 0., z0 + 0.5 * thickness_map[item_type]), logical_volume_map[item_type], item_type, logicWorld, false, copy_counter_map[item_type]++, true); //todo: index
      z0 += thickness_map[item_type];
    }

  }

}

void DetectorConstruction::ConstructSDandField() {

}


void DetectorConstruction::ReadNtupleEvent(G4int eventIndex) {
  //cleanup
  for (size_t nhit = 0; nhit < HGCalHitsForVisualisation.size(); nhit++) {
    HGCalHit* hit = HGCalHitsForVisualisation[nhit];
    hit->physicalVolume->SetTranslation(G4ThreeVector(0, 0., -beamLineLength / 2));
    delete hit;
  }
  HGCalHitsForVisualisation.clear();


  for (unsigned int i = 0; i < m_inputTree->GetEntries(); i++) {
    m_inputTree->GetEntry(i);
    if (eventID == eventIndex) break;
  }

  for (unsigned int nhit = 0; nhit < Nhits; nhit++) {
    unsigned int type = rechit_type_->at(nhit);
    if (type == 1) continue;
    if (type == 2) continue;
    if (type == 3) continue;
    if (type == 5) continue;
    /*
    ID = 0 : full cell,
    ID = 1 : calibration pad,
    ID = 2 : half cell,
    ID = 3 : mouse bite cell,
    ID = 4 : outer calibration pad cell,
    ID = 5 : merged cell
    */
    Float16_t hit_energy = rechit_energy_->at(nhit);
    if (hit_energy < 0.5) continue;


    HGCalHit* hit = new HGCalHit;
    hit->layer = rechit_layer_->at(nhit);
    hit->x = rechit_x_->at(nhit) * cm;
    hit->y = rechit_y_->at(nhit) * cm;


    float _r, _g, _b;
    if (hit_energy < 3.5) {_r = 0; _g = 0; _b = 1;}
    else if (hit_energy < 50.) {_r = 0; _g = 0.5; _b = 0.5;}
    else if (hit_energy < 100.) {_r = 0; _g = 1; _b = 0;}
    else if (hit_energy < 150.) {_r = 0.5; _g = 0.5; _b = 0;}
    else if (hit_energy < 200.) {_r = 0.8; _g = 0.2; _b = 0;}
    else if (hit_energy < 300.) {_r = 1.0; _g = 0.; _b = 0;}
    else if (hit_energy < 500.) {_r = 0.5; _g = 0.; _b = 0.5;}
    else {_r = 0.; _g = 0.; _b = 0.;}
    hit->red = _r;
    hit->green = _g;
    hit->blue = _b;


    HGCalHitsForVisualisation.push_back(hit);
  }
  std::cout << "Number of hits in file: " << Nhits << std::endl;
  std::cout << "Number of hits for visualisation: " << HGCalHitsForVisualisation.size() << std::endl;

  /*****    START GENERIC PLACEMENT ALGORITHM  FOR THE SETUP  *****/
  G4VisAttributes* visAttributes;
  for (size_t nhit = 0; nhit < HGCalHitsForVisualisation.size(); nhit++) {
    HGCalHit* hit = HGCalHitsForVisualisation[nhit];
    G4LogicalVolume* hit_logical = HexagonLogical(hit->name, Si_wafer_thickness, Si_pixel_sideLength, mat_Vacuum);
    visAttributes = new G4VisAttributes(G4Colour(hit->red, hit->green, hit->blue, 1.));
    visAttributes->SetVisibility(true);
    hit_logical->SetVisAttributes(visAttributes);
    std::cout << "Placing HGCal hit: " << hit->layer << "  " << hit->x << "  " << hit->y << std::endl;
    hit->physicalVolume = new G4PVPlacement(0, G4ThreeVector(hit->x, hit->y, HGCalLayerDistances[hit->layer - 1]), hit_logical, hit->name, logicWorld, false, 0, false);
  }

  G4RunManager::GetRunManager()->GeometryHasBeenModified();
  G4UImanager* UImanager = G4UImanager::GetUIpointer();
  UImanager->ApplyCommand("/vis/drawVolume");
  UImanager->ApplyCommand("/vis/viewer/set/viewpointThetaPhi 115 20");
  UImanager->ApplyCommand("/vis/viewer/set/targetPoint 0 0 11.3 m");

}


void DetectorConstruction::OpenNtuple(G4String path) {
  //reading from the file
  if (path == "") {
    return;
  } else {
    
    
  }
  if (m_inputFile==NULL) {
    m_inputFile = new TFile(path.c_str(), "READ");
    m_inputTree = (TTree*)m_inputFile->Get("rechitntupler/hits");

    b_eventID = new TBranch;
    b_Nhits = new TBranch;
    b_rechit_layer_ = new TBranch;
    b_rechit_module_ = new TBranch;
    b_rechit_chip_ = new TBranch;
    b_rechit_channel_ = new TBranch;
    b_rechit_type_ = new TBranch;
    b_rechit_energy_ = new TBranch;
    b_rechit_x_ = new TBranch;
    b_rechit_y_ = new TBranch;
    b_rechit_z_ = new TBranch;
  } else {
    std::cout<<"Closing "<<ntuplepath<<std::endl;
    m_inputFile->Close();
    m_inputFile = new TFile(path.c_str(), "READ");
    m_inputTree = (TTree*)m_inputFile->Get("rechitntupler/hits");
  }
  ntuplepath = path;
  std::cout << "Opened " << ntuplepath << std::endl;
  m_inputTree->SetBranchAddress("event", &eventID, &b_eventID);
  m_inputTree->SetBranchAddress("NRechits", &Nhits, &b_Nhits);
  m_inputTree->SetBranchAddress("rechit_layer", &rechit_layer_, &b_rechit_layer_);
  m_inputTree->SetBranchAddress("rechit_module", &rechit_module_, &b_rechit_module_);
  m_inputTree->SetBranchAddress("rechit_chip", &rechit_chip_, &b_rechit_chip_);
  m_inputTree->SetBranchAddress("rechit_channel", &rechit_channel_, &b_rechit_channel_);
  m_inputTree->SetBranchAddress("rechit_type", &rechit_type_, &b_rechit_type_);
  m_inputTree->SetBranchAddress("rechit_energy", &rechit_energy_, &b_rechit_energy_);
  m_inputTree->SetBranchAddress("rechit_x", &rechit_x_, &b_rechit_x_);
  m_inputTree->SetBranchAddress("rechit_y", &rechit_y_, &b_rechit_y_);
  m_inputTree->SetBranchAddress("rechit_z", &rechit_z_, &b_rechit_z_);

  eventID = Nhits = 0;
  rechit_layer_ = 0;
  rechit_module_ = 0;
  rechit_chip_ = 0;
  rechit_channel_ = 0;
  rechit_type_ = 0;
  rechit_energy_ = 0;
  rechit_x_ = 0;
  rechit_y_ = 0;
  rechit_z_ = 0;


}


void DetectorConstruction::SelectConfiguration(G4int val) {

  if (_configuration != -1) return;

  _configuration = val;

  ConstructHGCal();
  // tell G4RunManager that we change the geometry
  G4RunManager::GetRunManager()->GeometryHasBeenModified();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::DefineCommands()
{
  // Define /B5/detector command directory using generic messenger class
  fMessenger = new G4GenericMessenger(this,
                                      "/EventDisplay/ntuple/",
                                      "Configuration specifications");



  // configuration command
  auto& configCmd
    = fMessenger->DeclareMethod("config",
                                &DetectorConstruction::SelectConfiguration,
                                "Select the configuration (22-24)");
  configCmd.SetParameterName("config", true);
  configCmd.SetDefaultValue("22");

  auto& eventCmd
    = fMessenger->DeclareMethod("showEvent",
                                &DetectorConstruction::ReadNtupleEvent,
                                "Select event for visualisation");
  eventCmd.SetParameterName("showEvent", true);
  eventCmd.SetDefaultValue(1);


  auto& ntuplePathCmd
    = fMessenger->DeclareMethod("path",
                                &DetectorConstruction::OpenNtuple,
                                "Path to the ntuple");
  ntuplePathCmd.SetDefaultValue("");

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
