
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
       0.5*full_cellX, 0.5*full_cellY, 0.5*cellThickness);     //its size


  G4double deltaXDash = cellSideLength;
  G4double deltaYDash = sqrt(3)/4 * cellSideLength;
  
  G4Box* solidCutcell = new G4Box(name,                       //its name
       0.5*deltaXDash, 0.5*(deltaYDash), 1.*cellThickness);     //its size                   


  G4double DeltaTheta[4] = {60.*deg, 120.*deg, 240.*deg, 300.*deg};
  G4double DeltaTheta_rot[4] = {30.*deg,150.*deg,210*deg,330*deg};
  G4double Delta = sqrt(3)/2 * cellSideLength + deltaYDash/2;
  
  G4RotationMatrix* rot = new G4RotationMatrix;
  rot->rotateZ(DeltaTheta_rot[0]);
  std::vector<G4SubtractionSolid*> subtracted;
  subtracted.push_back(new G4SubtractionSolid("cellSubtracted", solidFullcell, solidCutcell, rot, G4ThreeVector(cos(DeltaTheta[0])*Delta, sin(DeltaTheta[0])*Delta, 0.)));
  
  for (int i=1; i<4; i++) {
    rot->rotateZ(-DeltaTheta_rot[i-1]);
    rot->rotateZ(DeltaTheta_rot[i]);
    subtracted.push_back(new G4SubtractionSolid("cellSubtracted", subtracted[i-1], solidCutcell, rot, G4ThreeVector(cos(DeltaTheta[i])*Delta, sin(DeltaTheta[i])*Delta, 0.)));  
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
  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();
  
  // Envelope parameters
  //
  //G4Material* env_mat = nist->FindOrBuildMaterial("G4_WATER");
   
  // Option to switch on/off checking of volumes overlaps
  //
  G4bool checkOverlaps = true;

  //     
  // World
  //
  G4double world_sizeXYZ = 50*cm;
  
  G4Box* solidWorld =    
    new G4Box("World",                       //its name
       0.5*world_sizeXYZ, 0.5*world_sizeXYZ, 0.5*world_sizeXYZ);     //its size
      
  G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");
  G4LogicalVolume* logicWorld =                         
    new G4LogicalVolume(solidWorld,          //its solid
                        world_mat,           //its material
                        "World");            //its name
                                   
  G4VPhysicalVolume* physWorld = 
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(0., 0., 0.),       //at (0,0,0)
                      logicWorld,            //its logical volume
                      "World",               //its name
                      0,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      checkOverlaps);        //overlaps checking

  //parameters for the hexagon
  G4double waferThickness = 0.3 * mm;
  G4double cellSideLength = 0.6 * cm;
  double alpha = 60./180. * M_PI;
  double dx = 2*sin(alpha)*cellSideLength;
  double dy = cellSideLength*(2.+2*cos(alpha));
  

  G4double full_waferX = 11*dx;
  G4double full_waferY = 7*dy;
  G4Material* wafer_mat = nist->FindOrBuildMaterial("G4_AIR");
  G4Box* solidFullWafer = new G4Box("waferPhysical",                       //its name
       0.5*full_waferX, 0.5*full_waferY, 0.5*waferThickness);     //its size

  G4LogicalVolume* waferLogical = new G4LogicalVolume(solidFullWafer,          //its solid
                        wafer_mat,           //its material
                        "waferLogical");   


  G4Material* cell_mat = nist->FindOrBuildMaterial("G4_Si");
  
  hexagonLogical = HexagonLogical("SiCell", waferThickness, cellSideLength, cell_mat);
  new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(0, 0., 0.),       //at (0,0,0)
                      hexagonLogical,            //its logical volume
                      "SiCell",               //its name
                      waferLogical,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      true);        //overlaps checking  
  

  //sensor setup
  int index = 1;
  int nRows[11] = {7, 6, 7, 6, 5, 6, 5, 4, 5, 4, 3};  
  for (int nC=0; nC<11; nC++) {
    for (int middle_index=0; middle_index<nRows[nC]; middle_index++){
      new G4PVPlacement(0,                     //no rotation
                          G4ThreeVector(nC*dx/2, dy*(middle_index-nRows[nC]/2.+0.5), 0.),       
                          hexagonLogical,            //its logical volume
                          "SiCell",               //its name
                          waferLogical,                     //its mother  volume
                          false,                 //no boolean operation
                          index++,                     //copy number
                          true);        //overlaps checking  
      
      if (nC<=0) continue;
      new G4PVPlacement(0,                     //no rotation
                          G4ThreeVector(-nC*dx/2, dy*(middle_index-nRows[nC]/2.+0.5), 0.),       
                          hexagonLogical,            //its logical volume
                          "SiCell",               //its name
                          waferLogical,                     //its mother  volume
                          false,                 //no boolean operation
                          index++,                     //copy number
                          true);        //overlaps checking  
    }
  }
  

  G4VisAttributes* visAttributes = new G4VisAttributes(G4Colour(0.0,1.0,0.0));
  visAttributes->SetVisibility(false);
  waferLogical->SetVisAttributes(visAttributes);

  
  //some absorber upstream
  G4Box* solidAbsorber =    
    new G4Box("Absorber",                       //its name
       0.3*world_sizeXYZ, 0.3*world_sizeXYZ, 0.03*world_sizeXYZ);     //its size
      
  G4Material* Absorber_mat = nist->FindOrBuildMaterial("G4_Pb");
  G4LogicalVolume* logicAbsorber =                         
    new G4LogicalVolume(solidAbsorber,          //its solid
                        Absorber_mat,           //its material
                        "Absorber");            //its name

  visAttributes = new G4VisAttributes(G4Colour(.2,0.2,0.2));
  visAttributes->SetVisibility(true);
  logicAbsorber->SetVisAttributes(visAttributes);
  
  for (int i=0; i<9; i++) {
    //place the wafer
    new G4PVPlacement(0,                     //no rotation
        G4ThreeVector(0,0,(-0.4+0.1*i)*world_sizeXYZ),       //at (0,0,0)
        waferLogical,            //its logical volume
        "waferPlacement",               //its name
        logicWorld,                     //its mother  volume
        false,                 //no boolean operation
        i,                     //copy number
        true);        //overlaps checking  

    new G4PVPlacement(0,                     //no rotation
        G4ThreeVector(0., 0., (-0.45+0.1*i)*world_sizeXYZ), 
        logicAbsorber,            //its logical volume
        "Absorber",               //its name
        logicWorld,                     //its mother  volume
        false,                 //no boolean operation
        0,                     //copy number
        checkOverlaps);        //overlaps checking

  }





  fScoringVolume = logicWorld;
  
  
  return physWorld;
}


void DetectorConstruction::ConstructSDandField(){
  G4SDManager* sdman = G4SDManager::GetSDMpointer();

  SiliconPixelSD* sensitive = new SiliconPixelSD((hexagonLogical->GetName()+"_sensitive").c_str());
  sdman->AddNewDetector(sensitive);
  hexagonLogical->SetSensitiveDetector(sensitive);
    
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
