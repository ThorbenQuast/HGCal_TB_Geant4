
#include "DetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
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
  G4double env_sizeXY = 20*cm, env_sizeZ = 740*mm;
  //G4Material* env_mat = nist->FindOrBuildMaterial("G4_WATER");
   
  // Option to switch on/off checking of volumes overlaps
  //
  G4bool checkOverlaps = true;

  //     
  // World
  //
  G4double world_sizeXY = 1.5*env_sizeXY;
  G4double world_sizeZ  = 1.5*env_sizeZ;
  G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");
  
  G4Box* solidWorld =    
    new G4Box("World",                       //its name
       0.5*world_sizeXY, 0.5*world_sizeXY, 0.5*world_sizeZ);     //its size
      
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
  
  std::vector<G4double> thicknesses;
  std::vector<int> material_index;

  material_index.push_back(4); thicknesses.push_back(2.6*mm);  //iron
 
  material_index.push_back(0); thicknesses.push_back(267*mm);  //air
  material_index.push_back(4); thicknesses.push_back(0.3*mm);  //iron
  material_index.push_back(2); thicknesses.push_back(4.9*mm);  //lead
  material_index.push_back(4); thicknesses.push_back(0.3*mm);  //iron
  material_index.push_back(3); thicknesses.push_back(6.0*mm);  //copper
  material_index.push_back(4); thicknesses.push_back(0.3*mm);  //iron
  material_index.push_back(2); thicknesses.push_back(4.9*mm);  //lead
  material_index.push_back(4); thicknesses.push_back(0.3*mm);  //iron
  material_index.push_back(3); thicknesses.push_back(6.0*mm);  //copper
  material_index.push_back(1); thicknesses.push_back(1.2*mm);  //tungsten
  material_index.push_back(5); thicknesses.push_back(1.9*mm);  //silicon

  material_index.push_back(0); thicknesses.push_back(166.6*mm);  //air
  material_index.push_back(4); thicknesses.push_back(0.3*mm);  //iron
  material_index.push_back(2); thicknesses.push_back(4.9*mm);  //lead
  material_index.push_back(4); thicknesses.push_back(0.3*mm);  //iron
  material_index.push_back(3); thicknesses.push_back(6.0*mm);  //copper
  material_index.push_back(1); thicknesses.push_back(1.2*mm);  //tungsten
  material_index.push_back(5); thicknesses.push_back(1.9*mm);  //silicon

  material_index.push_back(0); thicknesses.push_back(11.6*mm);  //air
  material_index.push_back(4); thicknesses.push_back(0.3*mm);  //iron
  material_index.push_back(2); thicknesses.push_back(4.9*mm);  //lead
  material_index.push_back(4); thicknesses.push_back(0.3*mm);  //iron
  material_index.push_back(3); thicknesses.push_back(6.0*mm);  //copper
  material_index.push_back(1); thicknesses.push_back(1.2*mm);  //tungsten
  material_index.push_back(5); thicknesses.push_back(1.9*mm);  //silicon

  material_index.push_back(0); thicknesses.push_back(10.6*mm);  //air
  material_index.push_back(4); thicknesses.push_back(0.3*mm);  //iron
  material_index.push_back(2); thicknesses.push_back(4.9*mm);  //lead
  material_index.push_back(4); thicknesses.push_back(0.3*mm);  //iron
  material_index.push_back(3); thicknesses.push_back(6.0*mm);  //copper
  material_index.push_back(1); thicknesses.push_back(1.2*mm);  //tungsten
  material_index.push_back(5); thicknesses.push_back(1.9*mm);  //silicon


  material_index.push_back(0); thicknesses.push_back(16.72*mm);  //air
  material_index.push_back(4); thicknesses.push_back(0.3*mm);  //iron
  material_index.push_back(2); thicknesses.push_back(4.9*mm);  //lead
  material_index.push_back(4); thicknesses.push_back(0.3*mm);  //iron
  material_index.push_back(0); thicknesses.push_back(0.7*mm);  //air
  material_index.push_back(3); thicknesses.push_back(6.0*mm);  //copper
  material_index.push_back(1); thicknesses.push_back(1.2*mm);  //tungsten
  material_index.push_back(5); thicknesses.push_back(1.9*mm);  //silicon

  material_index.push_back(0); thicknesses.push_back(2.6*mm);  //air
  material_index.push_back(4); thicknesses.push_back(0.3*mm);  //iron
  material_index.push_back(2); thicknesses.push_back(4.9*mm);  //lead
  material_index.push_back(4); thicknesses.push_back(0.3*mm);  //iron
  material_index.push_back(3); thicknesses.push_back(6.0*mm);  //copper
  material_index.push_back(4); thicknesses.push_back(0.3*mm);  //iron
  material_index.push_back(2); thicknesses.push_back(4.9*mm);  //lead
  material_index.push_back(4); thicknesses.push_back(0.3*mm);  //iron
  material_index.push_back(3); thicknesses.push_back(6.0*mm);  //copper
  material_index.push_back(4); thicknesses.push_back(0.3*mm);  //iron
  material_index.push_back(2); thicknesses.push_back(4.9*mm);  //lead
  material_index.push_back(4); thicknesses.push_back(0.3*mm);  //iron
  material_index.push_back(0); thicknesses.push_back(9.6*mm);  //air

  material_index.push_back(5); thicknesses.push_back(1.9*mm);  //silicon
  material_index.push_back(1); thicknesses.push_back(1.2*mm);  //tungsten
  material_index.push_back(3); thicknesses.push_back(6.0*mm);  //copper
  material_index.push_back(4); thicknesses.push_back(0.3*mm);  //iron
  material_index.push_back(2); thicknesses.push_back(4.9*mm);  //lead
  material_index.push_back(4); thicknesses.push_back(0.3*mm);  //iron
  material_index.push_back(3); thicknesses.push_back(6.0*mm);  //copper
  material_index.push_back(4); thicknesses.push_back(0.3*mm);  //iron
  material_index.push_back(2); thicknesses.push_back(4.9*mm);  //lead
  material_index.push_back(4); thicknesses.push_back(0.3*mm);  //iron
  material_index.push_back(3); thicknesses.push_back(6.0*mm);  //copper
  material_index.push_back(1); thicknesses.push_back(1.2*mm);  //tungsten
  material_index.push_back(5); thicknesses.push_back(1.9*mm);  //silicon

  material_index.push_back(0); thicknesses.push_back(16.6*mm);  //air
  material_index.push_back(4); thicknesses.push_back(0.3*mm);  //iron
  material_index.push_back(2); thicknesses.push_back(4.9*mm);  //lead
  material_index.push_back(4); thicknesses.push_back(0.3*mm);  //iron
  material_index.push_back(3); thicknesses.push_back(6.0*mm);  //copper
  material_index.push_back(4); thicknesses.push_back(0.3*mm);  //iron
  material_index.push_back(2); thicknesses.push_back(4.9*mm);  //lead
  material_index.push_back(4); thicknesses.push_back(0.3*mm);  //iron
  material_index.push_back(3); thicknesses.push_back(6.0*mm);  //copper
  material_index.push_back(4); thicknesses.push_back(0.3*mm);  //iron
  material_index.push_back(2); thicknesses.push_back(4.9*mm);  //lead
  material_index.push_back(4); thicknesses.push_back(0.3*mm);  //iron
  material_index.push_back(3); thicknesses.push_back(6.0*mm);  //copper
  
  material_index.push_back(0); thicknesses.push_back(3*mm);  //air

  material_index.push_back(4); thicknesses.push_back(40*mm);  //air
  material_index.push_back(4); thicknesses.push_back(2.6*mm);  //air
    


  G4double z_pos = 0;
  auto visAttributes = new G4VisAttributes(G4Colour(1.0,1.0,1.0));
  for (size_t i=0; i<material_index.size(); i++) {
    z_pos = z_pos + thicknesses[i];

    G4Box* solidEnv =    
    new G4Box(("material_"+std::to_string(i)).c_str(),                    //its name
    0.5*env_sizeXY, 0.5*env_sizeXY, 0.5*thicknesses[i]); //its size

    std::string mat_string = "";
    switch (material_index[i]) {
      case 1:
        mat_string = "G4_W";visAttributes = new G4VisAttributes(true, G4Colour(G4Colour().Brown().GetRed(), G4Colour().Brown().GetGreen(), G4Colour().Brown().GetBlue(), 0.1)); break;
      case 2:
        mat_string = "G4_Pb";visAttributes = new G4VisAttributes(true, G4Colour(G4Colour().Gray().GetRed(), G4Colour().Gray().GetGreen(), G4Colour().Gray().GetBlue(), 0.1)); break;
      case 3:
        mat_string = "G4_Cu";visAttributes = new G4VisAttributes(true, G4Colour(G4Colour().Brown().GetRed(), G4Colour().Brown().GetGreen(), G4Colour().Brown().GetBlue(), 0.1)); break;
      case 4:
        mat_string = "G4_Fe";visAttributes = new G4VisAttributes(true, G4Colour(G4Colour().Grey().GetRed(), G4Colour().Grey().GetGreen(), G4Colour().Grey().GetBlue(), 0.15)); break;
      case 5:
        mat_string = "G4_Si";visAttributes = new G4VisAttributes(true, G4Colour(G4Colour().Green().GetRed(), G4Colour().Green().GetGreen(), G4Colour().Green().GetBlue(), 1.0)); break;
      case 0:
      default:
        mat_string = "G4_AIR";visAttributes = new G4VisAttributes(false, G4Colour(G4Colour().Grey().GetRed(), G4Colour().Grey().GetGreen(), G4Colour().Grey().GetBlue(), 0.01)); break;
    }
    G4Material* env_mat = nist->FindOrBuildMaterial(mat_string.c_str());

    G4LogicalVolume* logicEnv =                         
    new G4LogicalVolume(solidEnv,            //its solid
                    env_mat,             //its material
                    ("material_"+std::to_string(i)).c_str());         //its name
    
    visAttributes->SetLineWidth(3);
    logicEnv->SetVisAttributes(visAttributes);

    new G4PVPlacement(0,                       //no rotation
                G4ThreeVector(0, 0, (z_pos - 0.5*thicknesses[i])-env_sizeZ/2.),        
                logicEnv,                //its logical volume
                ("material_"+std::to_string(i)).c_str(),              //its name
                logicWorld,              //its mother  volume
                false,                   //no boolean operation
                0,                       //copy number
                checkOverlaps);          //overlaps checking  
  }

  
  fScoringVolume = logicWorld;
  //
  //always return the physical World
  //
  return physWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
