
#include "DetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"

#include "G4Box.hh"
#include "G4SubtractionSolid.hh"
#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"

#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"

#include "SiliconPixelSD.hh"

#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include <vector>
#include <cstdlib>
#include <string>
#include <cmath>



G4LogicalVolume* HexagonLogical(G4String name, G4double cellThickness, G4double cellSideLength, G4Material* material) {
  G4double full_cellX = sqrt(3.) * cellSideLength;
  G4double full_cellY = (2.) * cellSideLength;
  G4Box* solidFullcell = new G4Box(name,                       //its name
                                   0.5 * full_cellX, 0.5 * full_cellY, 0.5 * cellThickness); //its size


  G4double deltaXDash = cellSideLength;
  G4double deltaYDash = sqrt(3) / 4 * cellSideLength;

  G4Box* solidCutcell = new G4Box(name,                       //its name
                                  0.5 * deltaXDash, 0.5 * (deltaYDash), 1.*cellThickness); //its size


  G4double DeltaTheta[4] = {60.*deg, 120.*deg, 240.*deg, 300.*deg};
  G4double DeltaTheta_rot[4] = {30.*deg, 150.*deg, 210 * deg, 330 * deg};
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

  return new G4LogicalVolume(subtracted[3],          //its solid
                             material,           //its material
                             name);            //its name

}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
  : G4VUserDetectorConstruction(),
    fScoringVolume(0)
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{

  // visual attributes
  G4VisAttributes* visAttributes;

  // Option to switch on/off checking of volumes overlaps
  //
  G4bool checkOverlaps = true;

  /***** Definition of all available materials *****/
  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();
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
  G4Material* mat_PCB = nist->FindOrBuildMaterial("G4_PLEXGILASS");
  //CuW alloy: 60% Cu, 40% W in mass
  G4double Cu_frac_in_CuW = 0.6;
  G4Material* mat_CuW = new G4Material("CuW", mat_Cu->GetDensity()*Cu_frac_in_CuW + mat_W->GetDensity() * (1 - Cu_frac_in_CuW), 2);
  mat_CuW->AddMaterial(mat_Cu, Cu_frac_in_CuW);
  mat_CuW->AddMaterial(mat_W, 1 - Cu_frac_in_CuW);


  /***** Definition of the world = beam line *****/
  G4double beamLineLength = 30 * m;
  G4double beamLineXY = 4 * m;

  // World = Beam line
  G4Box* solidWorld = new G4Box("World", 0.5 * beamLineXY, 0.5 * beamLineXY, 0.5 * beamLineLength);

  G4Material* world_mat = mat_AIR;
  logicWorld = new G4LogicalVolume(solidWorld, world_mat, "World");


  /***** Definition of silicon (wafer) sensors *****/
  //300 microns thickness only
  G4double Si_pixel_sideLength = 0.6496345 * cm;
  G4double Si_wafer_thickness = 0.3 * mm;
  double alpha = 60. / 180. * M_PI;
  G4double Si_wafer_sideLength = 11 * Si_pixel_sideLength;

  G4LogicalVolume* Si_wafer_logical = HexagonLogical("Si_wafer", Si_wafer_thickness, Si_wafer_sideLength, mat_AIR);
  visAttributes = new G4VisAttributes(G4Colour(.0, 0.0, 0.0));
  visAttributes->SetVisibility(false);
  Si_wafer_logical->SetVisAttributes(visAttributes);

  //Silicon pixel setups
  double dx = 2 * sin(alpha) * Si_pixel_sideLength;
  double dy = Si_pixel_sideLength * (2. + 2 * cos(alpha));
  Si_pixel_logical = HexagonLogical("SiCell", Si_wafer_thickness, Si_pixel_sideLength, mat_Si);
  new G4PVPlacement(0, G4ThreeVector(0, 0., 0.), Si_pixel_logical, "SiCell", Si_wafer_logical, false, 0, true);
  int index = 1;
  int nRows[11] = {7, 6, 7, 6, 5, 6, 5, 4, 5, 4, 3};
  for (int nC = 0; nC < 11; nC++) {
    for (int middle_index = 0; middle_index < nRows[nC]; middle_index++) {
      new G4PVPlacement(0, G4ThreeVector(nC * dx / 2, dy * (middle_index - nRows[nC] / 2. + 0.5), 0.), Si_pixel_logical, "SiCell", Si_wafer_logical, false, index++, true);
      if (nC <= 0) continue;
      new G4PVPlacement(0, G4ThreeVector(-nC * dx / 2, dy * (middle_index - nRows[nC] / 2. + 0.5), 0.), Si_pixel_logical, "SiCell", Si_wafer_logical, false, index++, true);
    }
  }

  /***** Definition of all baseplates *****/
  //CuW
  G4double CuW_baseplate_thickness = 1.2 * mm;
  G4double CuW_baseplate_sideLength = 11 * Si_pixel_sideLength;
  G4LogicalVolume* CuW_baseplate_logical = HexagonLogical("CuW_baseplate", CuW_baseplate_thickness, CuW_baseplate_sideLength, mat_CuW);
  visAttributes = new G4VisAttributes(G4Colour(.0, 0.2, 0.2));
  visAttributes->SetVisibility(true);
  CuW_baseplate_logical->SetVisAttributes(visAttributes);

  //Cu
  G4double Cu_baseplate_thickness = 1.2 * mm;
  G4double Cu_baseplate_sideLength = 11 * Si_pixel_sideLength;
  G4LogicalVolume* Cu_baseplate_logical = HexagonLogical("Cu_baseplate", Cu_baseplate_thickness, Cu_baseplate_sideLength, mat_Cu);
  visAttributes = new G4VisAttributes(G4Colour(.0, 0., 0.2));
  visAttributes->SetVisibility(true);
  Cu_baseplate_logical->SetVisAttributes(visAttributes);

  //PCB
  G4double PCB_baseplate_thickness = 1.2 * mm;
  G4double PCB_baseplate_sideLength = 11 * Si_pixel_sideLength;
  G4LogicalVolume* PCB_baseplate_logical = HexagonLogical("PCB_baseplate", PCB_baseplate_thickness, PCB_baseplate_sideLength, mat_PCB);
  visAttributes = new G4VisAttributes(G4Colour(.0, 0., 0.2));
  visAttributes->SetVisibility(true);
  PCB_baseplate_logical->SetVisAttributes(visAttributes);

  //Kapton layer
  G4double Kapton_layer_thickness = 1.2 * mm;
  G4double Kapton_layer_sideLength = 11 * Si_pixel_sideLength;
  G4LogicalVolume* Kapton_layer_logical = HexagonLogical("Kapton_layer", Kapton_layer_thickness, Kapton_layer_sideLength, mat_KAPTON);
  visAttributes = new G4VisAttributes(G4Colour(.2, 1., 0.4));
  visAttributes->SetVisibility(true);
  Kapton_layer_logical->SetVisAttributes(visAttributes);



  /***** Definition of absorber plates *****/
  G4double Al_case_thickness = 2.6 * mm;
  G4double Al_case_xy = 60 * cm;
  G4Box* Al_case_solid = new G4Box("Al_case", 0.5*Al_case_xy, 0.5*Al_case_xy, 0.5*Al_case_thickness);
  G4LogicalVolume* Al_case_logical = new G4LogicalVolume(Al_case_solid, mat_Al, "Al_case");
  visAttributes = new G4VisAttributes(G4Colour(0.4, 0.4, 0.4));
  visAttributes->SetVisibility(true);
  Al_case_logical->SetVisAttributes(visAttributes);

  G4double Steel_case_thickness = 2.6 * mm;
  G4double Steel_case_xy = 60 * cm;
  G4Box* Steel_case_solid = new G4Box("Steel_case", 0.5*Steel_case_xy, 0.5*Steel_case_xy, 0.5*Steel_case_thickness);
  G4LogicalVolume* Steel_case_logical = new G4LogicalVolume(Steel_case_solid, mat_Steel, "Steel_case");
  visAttributes = new G4VisAttributes(G4Colour(0.4, 0.4, 0.4));
  visAttributes->SetVisibility(true);
  Steel_case_logical->SetVisAttributes(visAttributes);

  //defintion of absorber plates in the EE part
  G4double Pb_absorber_EE_thickness = 4.9 * mm;
  G4double Pb_absorber_EE_xy = 30 * cm;
  G4Box* Pb_absorber_EE_solid = new G4Box("Pb_absorber_EE", 0.5*Pb_absorber_EE_xy, 0.5*Pb_absorber_EE_xy, 0.5*Pb_absorber_EE_thickness);
  G4LogicalVolume* Pb_absorber_EE_logical = new G4LogicalVolume(Pb_absorber_EE_solid, mat_Pb, "Pb_absorber_EE");
  visAttributes = new G4VisAttributes(G4Colour(0.4, 0.4, 0.4));
  visAttributes->SetVisibility(true);
  Pb_absorber_EE_logical->SetVisAttributes(visAttributes);

  G4double Cu_absorber_EE_thickness = 6 * mm;
  G4double Cu_absorber_EE_xy = 30 * cm;
  G4Box* Cu_absorber_EE_solid = new G4Box("Cu_absorber_EE", 0.5*Cu_absorber_EE_xy, 0.5*Cu_absorber_EE_xy, 0.5*Cu_absorber_EE_thickness);
  G4LogicalVolume* Cu_absorber_EE_logical = new G4LogicalVolume(Cu_absorber_EE_solid, mat_Cu, "Cu_absorber_EE");
  visAttributes = new G4VisAttributes(G4Colour(.0, 0., 0.2));
  visAttributes->SetVisibility(true);
  Cu_absorber_EE_logical->SetVisAttributes(visAttributes);

  //defintion of absorber plates in the FH part
  G4double Cu_absorber_FH_thickness = 6 * mm;
  G4double Cu_absorber_FH_xy = 50 * cm;
  G4Box* Cu_absorber_FH_solid = new G4Box("Cu_absorber_FH", 0.5*Cu_absorber_FH_xy, 0.5*Cu_absorber_FH_xy, 0.5*Cu_absorber_FH_thickness);
  G4LogicalVolume* Cu_absorber_FH_logical = new G4LogicalVolume(Cu_absorber_FH_solid, mat_Cu, "Cu_absorber_FH");
  visAttributes = new G4VisAttributes(G4Colour(.0, 0., 0.2));
  visAttributes->SetVisibility(true);
  Cu_absorber_FH_logical->SetVisAttributes(visAttributes);

  G4double Fe_absorber_FH_thickness = 40 * mm;
  G4double Fe_absorber_FH_xy = 50 * cm;
  G4Box* Fe_absorber_FH_solid = new G4Box("Fe_absorber_FH", 0.5*Fe_absorber_FH_xy, 0.5*Fe_absorber_FH_xy, 0.5*Fe_absorber_FH_thickness);
  G4LogicalVolume* Fe_absorber_FH_logical = new G4LogicalVolume(Fe_absorber_FH_solid, mat_Fe, "Fe_absorber_FH");
  visAttributes = new G4VisAttributes(G4Colour(0.3, 0.4, 0.3));
  visAttributes->SetVisibility(true);
  Fe_absorber_FH_logical->SetVisAttributes(visAttributes);


  /***** Definition of beam line elements *****/
  //scintillators
  G4double scintillator_thickness = 10 * mm;
  G4double scintillator_xy = 10 * cm;
  G4Box* scintillator_solid = new G4Box("scintillator", 0.5*scintillator_xy, 0.5*scintillator_xy, 0.5*scintillator_thickness);
  G4LogicalVolume* scintillator_logical = new G4LogicalVolume(scintillator_solid, mat_PCB, "scintillator");
  visAttributes = new G4VisAttributes(G4Colour(1.0, 1.0, 1.0));
  visAttributes->SetVisibility(true);
  scintillator_logical->SetVisAttributes(visAttributes);


  //DWC related material
  G4double DWC_thickness = 10 * mm;
  G4double DWC_xy = 10 * cm;
  G4Box* DWC_solid = new G4Box("DWC", 0.5*DWC_xy, 0.5*DWC_xy, 0.5*DWC_thickness);
  G4LogicalVolume* DWC_logical = new G4LogicalVolume(DWC_solid, mat_Glass, "DWC");
  visAttributes = new G4VisAttributes(G4Colour(1.0, 1.0, 1.0));
  visAttributes->SetVisibility(true);
  DWC_logical->SetVisAttributes(visAttributes);

  G4double DWC_gas_thickness = DWC_thickness - 2 * mm;
  G4double DWC_gas_xy = DWC_xy - 2 * mm;
  G4Box * DWC_gas_solid = new G4Box("DWC_gas", 0.5*DWC_gas_xy, 0.5*DWC_gas_xy, 0.5*DWC_gas_thickness);
  G4LogicalVolume* DWC_gas_logical = new G4LogicalVolume(DWC_gas_solid, mat_Ar, "DWC_gas");
  visAttributes = new G4VisAttributes(G4Colour(0.05, 0.05, 0.05));
  visAttributes->SetVisibility(true);
  DWC_gas_logical->SetVisAttributes(visAttributes);

  new G4PVPlacement(0,                     //no rotation
                    G4ThreeVector(0, 0., 0.),       //at (0,0,0)
                    DWC_gas_logical,            //its logical volume
                    "DWC",               //its name
                    DWC_logical,                     //its mother  volume
                    false,                 //no boolean operation
                    0,                     //copy number
                    true);        //overlaps checking




  /***** Start the placement *****/
  //
  //

  G4VPhysicalVolume* physWorld =
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(0., 0., 0.),       //at (0,0,0)
                      logicWorld,            //its logical volume
                      "World",               //its name
                      0,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      checkOverlaps);        //overlaps checking


  //Todo: 1. generic placement of material
  //Todo: 2. ensure that hit coordinates are given in the world frame instead of mother only  

  /****  START OF TEST ****/
  new G4PVPlacement(0, G4ThreeVector(0., 0., -52*cm), Fe_absorber_FH_logical, "Fe_absorber_FH", logicWorld, false, 0, true);
  new G4PVPlacement(0, G4ThreeVector(0., 0., -44*cm), Fe_absorber_FH_logical, "Fe_absorber_FH", logicWorld, false, 0, true);
  new G4PVPlacement(0, G4ThreeVector(0., 0., -36*cm), Fe_absorber_FH_logical, "Fe_absorber_FH", logicWorld, false, 0, true);
  new G4PVPlacement(0, G4ThreeVector(0., 0., -28*cm), Fe_absorber_FH_logical, "Fe_absorber_FH", logicWorld, false, 0, true);
  new G4PVPlacement(0, G4ThreeVector(0., 0., -20*cm), Fe_absorber_FH_logical, "Fe_absorber_FH", logicWorld, false, 0, true);
  new G4PVPlacement(0, G4ThreeVector(0., 0., -12*cm), Fe_absorber_FH_logical, "Fe_absorber_FH", logicWorld, false, 1, true);
  new G4PVPlacement(0, G4ThreeVector(0., 0., -4*cm), Fe_absorber_FH_logical, "Fe_absorber_FH", logicWorld, false, 2, true);


  double dx_ = 2 * sin(alpha) * 11 * Si_pixel_sideLength;
  double dy_ = 11 * Si_pixel_sideLength * (2. + 2 * cos(alpha));

  int index_ = 0;
  int nRows_[3] = {1, 2, 1};
  for (int nC = 0; nC < 3; nC++) {
    for (int middle_index = 0; middle_index < nRows_[nC]; middle_index++) {
      new G4PVPlacement(0, G4ThreeVector(nC * dx_ / 2, dy_ * (middle_index - nRows_[nC] / 2. + 0.5), 0.), Si_wafer_logical, "Si_wafer", logicWorld, false, index_++, true);
      if (nC <= 0) continue;
      new G4PVPlacement(0, G4ThreeVector(-nC * dx_ / 2, dy_ * (middle_index - nRows_[nC] / 2. + 0.5), 0.), Si_wafer_logical, "Si_wafer", logicWorld, false, index_++, true);
    }
  }
  /****  END OF TEST ****/


  fScoringVolume = logicWorld;


  return physWorld;
}


void DetectorConstruction::ConstructSDandField() {
  G4SDManager* sdman = G4SDManager::GetSDMpointer();

  SiliconPixelSD* sensitive = new SiliconPixelSD((Si_pixel_logical->GetName() + "_sensitive").c_str());
  sdman->AddNewDetector(sensitive);
  Si_pixel_logical->SetSensitiveDetector(sensitive);


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
