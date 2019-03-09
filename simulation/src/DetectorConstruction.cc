#include "DetectorConstruction.hh"

#include "test_configs.hh"

#include "config1_2_Summer2017.hh"
#include "config3_September2017.hh"

#include "config_DESY2018_42.hh"
#include "configs17To21_June2018.hh"
#include "config22_October2018_1.hh"
#include "config23_October2018_2.hh"
#include "config24_October2018_3.hh"
#include "config25_October2018_3_parasitic.hh"


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

  // World = Beam line
  G4Box* solidWorld = new G4Box("World", 0.5 * BEAMLINEXY * m, 0.5 * BEAMLINEXY * m, 0.5 * BEAMLINELENGTH * m);

  G4Material* world_mat = materials->air();
  logicWorld = new G4LogicalVolume(solidWorld, world_mat, "World");

  G4VPhysicalVolume* physWorld =
    new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld, "World", 0, false, 0, true);


  fScoringVolume = logicWorld;
  return physWorld;
}


void DetectorConstruction::ConstructHGCal() {
  

  G4double z0 = -BEAMLINELENGTH * m / 2.;
  
  std::cout << "Constructing configuration " << _configuration << std::endl;

  /*****    START GENERIC PLACEMENT ALGORITHM  FOR THE SETUP  *****/
  for (size_t item_index = 0; item_index < dz_map.size(); item_index++) {
    std::string item_type = dz_map[item_index].first;
    G4double dz = dz_map[item_index].second;
    z0 += dz;
    
    //places the item at inside the world at z0, z0 is incremented by the item's thickness
    materials->placeItemInLogicalVolume(item_type, z0, logicWorld);
  }


  G4RunManager::GetRunManager()->GeometryHasBeenModified();
  G4UImanager* UImanager = G4UImanager::GetUIpointer();
  UImanager->ApplyCommand("/vis/drawVolume");
  UImanager->ApplyCommand("/vis/viewer/set/targetPoint 0 0 "+std::to_string(default_viewpoint/m)+" m");
  UImanager->ApplyCommand("/vis/scene/add/trajectories smooth");
  UImanager->ApplyCommand("/vis/scene/add/hits");

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

  default_viewpoint = 0;
  if (val==1) defineConfigs1_2_Summer2017(dz_map, default_viewpoint);
  else if (val==2) defineConfigs1_2_Summer2017(dz_map, default_viewpoint);
  else if (val==3) defineConfig3_September2017(dz_map, default_viewpoint);
  else if (val==15) defineDESY2018_Config42(dz_map, default_viewpoint);
  else if (val == 17) defineConfigs17To21_June2018(dz_map, default_viewpoint);
  else if (val == 18) defineConfigs17To21_June2018(dz_map, default_viewpoint);
  else if (val == 19) defineConfigs17To21_June2018(dz_map, default_viewpoint);
  else if (val == 21) defineConfigs17To21_June2018(dz_map, default_viewpoint);  
  else if (val == 22) defineConfig22_October2018_1(dz_map, default_viewpoint);
  else if (val == 23) defineConfig23_October2018_2(dz_map, default_viewpoint);
  else if (val == 24) defineConfig24_October2018_3(dz_map, default_viewpoint);
  else if (val == 25) defineConfig25_October2018_3_parasitic(dz_map, default_viewpoint);
  else if (val == 100) defineTestConfig100(dz_map, default_viewpoint);
  else if (val == 101) defineTestConfig101(dz_map, default_viewpoint);
  else if (val == 102) defineTestConfig102(dz_map, default_viewpoint);
  else {
    std::cout << "Configuration " << val << " not implemented --> return" << std::endl;; 
    return;
  }
  _configuration = val;

  ConstructHGCal();
}

void DetectorConstruction::SetStepSizeSilicon(G4double val) {
  //setting the step size in silicon:
  G4double maxTrackLength = val*0.001*mm;
  materials->getSi_pixel_logical()->SetUserLimits(new G4UserLimits(0,maxTrackLength));
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
    = fMessenger->DeclareMethod("configuration",
                                &DetectorConstruction::SelectConfiguration,
                                "Select the configuration (22-24)");
  configCmd.SetParameterName("index", true);
  configCmd.SetDefaultValue("22");


  auto& SiStepSizeCmd
    = fMessenger->DeclareMethod("stepSilicon",
                                &DetectorConstruction::SetStepSizeSilicon,
                                "Maximum step size in silicon pixels, unit: microns");
  SiStepSizeCmd.SetParameterName("size", true);
  SiStepSizeCmd.SetDefaultValue("50.");  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
