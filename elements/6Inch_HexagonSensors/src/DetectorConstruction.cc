
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


#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include <vector>
#include <cstdlib>
#include <string>

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


  //lets make one big hexagon made out of silicon
  double waferThickness = 0.3 * mm;
  double waferXY = 12 * cm;
  G4Box* solidFullWafer = new G4Box("fullWafer",                       //its name
       0.5*waferXY, 0.5*waferXY, 0.5*waferThickness);     //its size


  G4Box* solidCutWafer = new G4Box("fullWafer",                       //its name
       0.25*waferXY, 0.25*waferXY, 1.*waferThickness);     //its size                   


  G4RotationMatrix* rot = new G4RotationMatrix;
  rot->rotateZ(45*deg);
  
  std::vector<G4SubtractionSolid*> subtracted;
  subtracted.push_back(new G4SubtractionSolid("waferWithHole", solidFullWafer, solidCutWafer, rot, G4ThreeVector(0.5*waferXY,0.5*waferXY,0.)));

  for (int i=0; i<3; i++) subtracted.push_back(new G4SubtractionSolid("waferWithHole", subtracted[i], solidCutWafer, rot, G4ThreeVector(i!=1 ? -0.5*waferXY: 0.5*waferXY ,i < 2 ? -0.5*waferXY: 0.5*waferXY, 0.)));


  G4Material* fullWafer_mat = nist->FindOrBuildMaterial("G4_Si");
  G4LogicalVolume* logicFullWafer =  new G4LogicalVolume(subtracted[3],          //its solid
                        fullWafer_mat,           //its material
                        "fullWafer");            //its name

  
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(0., 0., 0.),       //at (0,0,0)
                      logicFullWafer,            //its logical volume
                      "fullWafer",               //its name
                      logicWorld,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      checkOverlaps);        //overlaps checking
  
  fScoringVolume = logicWorld;
  //
  //always return the physical World
  //
  return physWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
