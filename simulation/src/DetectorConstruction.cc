#include "DetectorConstruction.hh"

#include "config22_October2018_1.hh"
#include "config23_October2018_2.hh"
#include "config24_October2018_3.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
  : G4VUserDetectorConstruction(),
    fScoringVolume(0),
    _configuration(-1)
{

  DefineCommands();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  //definition of the materials
  materials = new HGCalTBMaterials();
  materials->setSimulationColorScheme();

  /***** Definition of the world = beam line *****/
  beamLineLength = 36 * m;
  beamLineXY = 9 * m;


  // World = Beam line
  G4Box* solidWorld = new G4Box("World", 0.5 * beamLineXY, 0.5 * beamLineXY, 0.5 * beamLineLength);

  G4Material* world_mat = materials->air();
  logicWorld = new G4LogicalVolume(solidWorld, world_mat, "World");

  G4VPhysicalVolume* physWorld =
    new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld, "World", 0, false, 0, true);


  fScoringVolume = logicWorld;
  return physWorld;
}


void DetectorConstruction::ConstructHGCal() {
  std::vector<std::pair<std::string, G4double> > dz_map;

  G4double z0 = -beamLineLength / 2.;

  if (_configuration == 22) defineConfig22_October2018_1(dz_map);
  else if (_configuration == 23) defineConfig23_October2018_2(dz_map);
  else if (_configuration == 24) defineConfig24_October2018_3(dz_map);
  else {
    std::cout << "Configuration " << _configuration << " not implemented --> return"; 
    return;
  }
  
  std::cout << "Constructing configuration " << _configuration << std::endl;


  /*****    START GENERIC PLACEMENT ALGORITHM  FOR THE SETUP  *****/
  for (size_t item_index = 0; item_index < dz_map.size(); item_index++) {
    std::string item_type = dz_map[item_index].first;
    G4double dz = dz_map[item_index].second;
    z0 += dz;
    
    //places the item at inside the world at z0, z0 is incremented by the item's thickness
    materials->placeItemInLogicalVolume(item_type, z0, logicWorld);
  }

}

void DetectorConstruction::ConstructSDandField() {
  G4SDManager* sdman = G4SDManager::GetSDMpointer();

  SiliconPixelSD* sensitiveSilicon = new SiliconPixelSD((materials->getSi_pixel_logical()->GetName() + "_sensitive").c_str());
  sdman->AddNewDetector(sensitiveSilicon);
  materials->getSi_pixel_logical()->SetSensitiveDetector(sensitiveSilicon);

  SiPMSD* sensitiveSiPM = new SiPMSD((materials->getAHCAL_SiPM_logical()->GetName() + "_sensitive").c_str());
  sdman->AddNewDetector(sensitiveSiPM);
  materials->getAHCAL_SiPM_logical()->SetSensitiveDetector(sensitiveSiPM);

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
                                      "/Simulation/setup/",
                                      "Configuration specifications");

  // configuration command
  auto& configCmd
    = fMessenger->DeclareMethod("config",
                                &DetectorConstruction::SelectConfiguration,
                                "Select the configuration (22-24)");
  configCmd.SetParameterName("config", true);
  configCmd.SetDefaultValue("22");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
