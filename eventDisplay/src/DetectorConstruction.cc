#include "DetectorConstruction.hh"

#include "colors.hh"

#include "config1_2_Summer2017.hh"
#include "config3_September2017.hh"
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
  ntuplepath = "";
  time_cut = -1;
  m_inputFileHGCal = NULL;
  m_inputTreeHGCal = NULL;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  HGCalLayerDistances.clear();

  //definition of the materials
  materials = new HGCalTBMaterials(); 
  materials->setEventDisplayColorScheme();

  /***** Definition of the world = beam line *****/

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
    //special for the event display
    if (item_type.find("Si_wafer") != std::string::npos) {
      HGCalLayerDistances.push_back(z0);
    }
    if (item_type.find("AHCAL_SiPM") != std::string::npos) {
      AHCALLayerDistances.push_back(z0);
    }
    //places the item at inside the world at z0, z0 is incremented by the item's thickness
    materials->placeItemInLogicalVolume(item_type, z0, logicWorld);
  }

  G4RunManager::GetRunManager()->GeometryHasBeenModified();
  G4UImanager* UImanager = G4UImanager::GetUIpointer();
  UImanager->ApplyCommand("/vis/drawVolume");
  UImanager->ApplyCommand("/vis/viewer/set/targetPoint 0 0 " + std::to_string(default_viewpoint / m) + " m");

}

void DetectorConstruction::ConstructSDandField() {

}


void DetectorConstruction::ReadNtupleEvent(G4int eventIndex) {
  //cleanup
  G4VisAttributes* visAttributes = new G4VisAttributes(G4Colour(1, 1, 1, 0.));
  visAttributes->SetVisibility(false);

  for (size_t nhit = 0; nhit < HGCalHitsForVisualisation.size(); nhit++) {
    VisHit* hit = HGCalHitsForVisualisation[nhit];
    hit->physicalVolume->SetTranslation(G4ThreeVector(0, 0., -BEAMLINELENGTH / 2));
    hit->physicalVolume->GetLogicalVolume()->SetVisAttributes(visAttributes);
    delete hit;
  }
  HGCalHitsForVisualisation.clear();

  for (size_t nhit = 0; nhit < AHCALHitsForVisualisation.size(); nhit++) {
    VisHit* hit = AHCALHitsForVisualisation[nhit];
    hit->physicalVolume->SetTranslation(G4ThreeVector(0, 0., -BEAMLINELENGTH / 2));
    hit->physicalVolume->GetLogicalVolume()->SetVisAttributes(visAttributes);
    delete hit;
  }
  AHCALHitsForVisualisation.clear();


  //HGCal hits
  if ((m_inputTreeHGCal != NULL) && (m_inputFileHGCal->IsOpen())) {
    for (unsigned int i = 0; i < m_inputTreeHGCal->GetEntries(); i++) {
      m_inputTreeHGCal->GetEntry(i);
      if (eventID == eventIndex) break;
    }
    if (eventID != eventIndex) std::cout << "[WARNING] loaded HGCAL event " << eventID << " !=" << eventIndex << std::endl;

    for (unsigned int nhit = 0; nhit < Nhits; nhit++) {
      if (rechit_noise_flag_->at(nhit)) continue;

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

      unsigned short hit_tot = rechit_toa_->at(nhit);
      if ((time_cut >= 0) && (hit_tot < time_cut)) continue;

      VisHit* hit = new VisHit;
      hit->name = "HGCAL-Hit";
      hit->layer = rechit_layer_->at(nhit);
      hit->x = rechit_x_->at(nhit) * cm;
      hit->y = rechit_y_->at(nhit) * cm;
      hit->energy = rechit_energy_->at(nhit);

      setHGCALHitColor(hit);

      HGCalHitsForVisualisation.push_back(hit);
    }
    std::cout << "Number of hits in HGCAL file: " << Nhits << std::endl;
    std::cout << "Number of hits for visualisation: " << HGCalHitsForVisualisation.size() << std::endl;
  } else {
    std::cout << "HGCAL file is not open!" << std::endl;
  }



  //AHCAL hits
  if (time_cut < 0) {
    if ((m_inputTreeAHCAL != NULL) && (m_inputFileAHCAL->IsOpen())) {
      for (unsigned int i = 0; i < m_inputTreeAHCAL->GetEntries(); i++) {
        m_inputTreeAHCAL->GetEntry(i);
        if (AHCAL_eventID == eventIndex + ahcalOffset) break;
      }
      if (AHCAL_eventID != eventIndex + ahcalOffset) std::cout << "[WARNING] loaded AHCAL event " << AHCAL_eventID << " !=" << eventIndex + ahcalOffset << std::endl;

      for (int nhit = 0; nhit < AHCAL_Nhits; nhit++) {
        VisHit* hit = new VisHit;
        hit->name = "AHCAL-Hit";
        hit->layer = ahc_hitK_[nhit];
        hit->x = -(ahc_hitI_[nhit] - 12.) * materials->getAHCAL_SiPM_xy();
        hit->y = (ahc_hitJ_[nhit] - 12.) * materials->getAHCAL_SiPM_xy();
        hit->energy = ahc_hitEnergy_[nhit];

        setAHCALHitColor(hit);

        AHCALHitsForVisualisation.push_back(hit);
      }
      std::cout << "Number of hits in AHCAL file: " << AHCAL_Nhits << std::endl;
      std::cout << "Number of hits for visualisation: " << AHCALHitsForVisualisation.size() << std::endl;
    } else {
      std::cout << "AHCAL file is not open!" << std::endl;
    }
  } else {
    std::cout << "timing cut: " << time_cut << " prevents AHCAL hits to be visualised" << std::endl;
  }




  /*****    START GENERIC PLACEMENT ALGORITHM  FOR THE SETUP  *****/
  for (size_t nhit = 0; nhit < HGCalHitsForVisualisation.size(); nhit++) {
    VisHit* hit = HGCalHitsForVisualisation[nhit];
    G4LogicalVolume* hit_logical = materials->newSiPixelHitLogical(hit->name);
    visAttributes = new G4VisAttributes(G4Colour(hit->red, hit->green, hit->blue, 1.));
    visAttributes->SetVisibility(true);
    hit_logical->SetVisAttributes(visAttributes);
    //std::cout << "Placing HGCal hit: " << hit->layer << "  " << hit->x << "  " << hit->y << std::endl;
    hit->physicalVolume = new G4PVPlacement(0, G4ThreeVector(hit->x, hit->y, HGCalLayerDistances[hit->layer - 1]), hit_logical, hit->name, logicWorld, false, 0, false);
  }


  for (size_t nhit = 0; nhit < AHCALHitsForVisualisation.size(); nhit++) {
    VisHit* hit = AHCALHitsForVisualisation[nhit];
    G4LogicalVolume* hit_logical = materials->newSiPMHitLogical(hit->name);
    visAttributes = new G4VisAttributes(G4Colour(hit->red, hit->green, hit->blue, 1.));
    visAttributes->SetVisibility(true);
    hit_logical->SetVisAttributes(visAttributes);
    hit->physicalVolume = new G4PVPlacement(0, G4ThreeVector(hit->x, hit->y, AHCALLayerDistances[hit->layer - 1]), hit_logical, hit->name, logicWorld, false, 0, false);
  }


  G4RunManager::GetRunManager()->GeometryHasBeenModified();
  G4UImanager* UImanager = G4UImanager::GetUIpointer();
  UImanager->ApplyCommand("/vis/drawVolume");
  //UImanager->ApplyCommand("/vis/viewer/set/viewpointThetaPhi 115 20");
  //UImanager->ApplyCommand("/vis/viewer/set/targetPoint 0 0 12.3 m");

}


void DetectorConstruction::OpenHGCALNtuple(G4String path) {
  //reading from the file
  if (path == "") {
    return;
  } else {


  }
  if (m_inputFileHGCal == NULL) {
    m_inputFileHGCal = new TFile(path.c_str(), "READ");
    m_inputTreeHGCal = (TTree*)m_inputFileHGCal->Get("rechitntupler/hits");

    hgcalBranches["eventID"] = new TBranch;
    hgcalBranches["Nhits"] = new TBranch;
    hgcalBranches["rechit_layer_"] = new TBranch;
    hgcalBranches["rechit_module_"] = new TBranch;
    hgcalBranches["rechit_chip_"] = new TBranch;
    hgcalBranches["rechit_channel_"] = new TBranch;
    hgcalBranches["rechit_type_"] = new TBranch;
    hgcalBranches["rechit_energy_"] = new TBranch;
    hgcalBranches["rechit_toa_"] = new TBranch;
    hgcalBranches["rechit_x_"] = new TBranch;
    hgcalBranches["rechit_y_"] = new TBranch;
    hgcalBranches["rechit_z_"] = new TBranch;
    hgcalBranches["rechit_noise_flag"] = new TBranch;
  } else {
    std::cout << "Closing " << ntuplepath << std::endl;
    m_inputFileHGCal->Close();
    m_inputFileHGCal = new TFile(path.c_str(), "READ");
    m_inputTreeHGCal = (TTree*)m_inputFileHGCal->Get("rechitntupler/hits");
  }
  ntuplepath = path;
  std::cout << "Opened " << ntuplepath << std::endl;
  m_inputTreeHGCal->SetBranchAddress("event", &eventID, &hgcalBranches["eventID"]);
  m_inputTreeHGCal->SetBranchAddress("NRechits", &Nhits, &hgcalBranches["Nhits"]);
  m_inputTreeHGCal->SetBranchAddress("rechit_layer", &rechit_layer_, &hgcalBranches["rechit_layer_"]);
  m_inputTreeHGCal->SetBranchAddress("rechit_module", &rechit_module_, &hgcalBranches["rechit_module_"]);
  m_inputTreeHGCal->SetBranchAddress("rechit_chip", &rechit_chip_, &hgcalBranches["rechit_chip_"]);
  m_inputTreeHGCal->SetBranchAddress("rechit_channel", &rechit_channel_, &hgcalBranches["rechit_channel_"]);
  m_inputTreeHGCal->SetBranchAddress("rechit_type", &rechit_type_, &hgcalBranches["rechit_type_"]);
  m_inputTreeHGCal->SetBranchAddress("rechit_energy", &rechit_energy_, &hgcalBranches["rechit_energy_"]);
  m_inputTreeHGCal->SetBranchAddress("rechit_toaRise", &rechit_toa_, &hgcalBranches["rechit_toa_"]);
  m_inputTreeHGCal->SetBranchAddress("rechit_x", &rechit_x_, &hgcalBranches["rechit_x_"]);
  m_inputTreeHGCal->SetBranchAddress("rechit_y", &rechit_y_, &hgcalBranches["rechit_y_"]);
  m_inputTreeHGCal->SetBranchAddress("rechit_z", &rechit_z_, &hgcalBranches["rechit_z_"]);
  m_inputTreeHGCal->SetBranchAddress("rechit_noise_flag", &rechit_noise_flag_, &hgcalBranches["rechit_noise_flag"]);

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


void DetectorConstruction::OpenAHCALNtuple(G4String path) {
  //reading from the file
  if (path == "") {
    return;
  } else {

  }
  if (m_inputFileAHCAL == NULL) {
    m_inputFileAHCAL = new TFile(path.c_str(), "READ");
    m_inputTreeAHCAL = (TTree*)m_inputFileAHCAL->Get("bigtree");

    ahcalBranches["eventNumber"] = new TBranch;
    ahcalBranches["ahc_nHits"] = new TBranch;
    ahcalBranches["ahc_hitI"] = new TBranch;
    ahcalBranches["ahc_hitJ"] = new TBranch;
    ahcalBranches["ahc_hitK"] = new TBranch;
    ahcalBranches["ahc_hitEnergy"] = new TBranch;
  } else {
    std::cout << "Closing " << ntupleAHCALpath << std::endl;
    m_inputFileAHCAL->Close();
    m_inputFileAHCAL = new TFile(path.c_str(), "READ");
    m_inputTreeAHCAL = (TTree*)m_inputFileAHCAL->Get("bigtree");
  }
  ntupleAHCALpath = path;
  std::cout << "Opened " << ntupleAHCALpath << std::endl;
  m_inputTreeAHCAL->SetBranchAddress("eventNumber", &AHCAL_eventID, &ahcalBranches["eventNumber"]);
  m_inputTreeAHCAL->SetBranchAddress("ahc_nHits", &AHCAL_Nhits, &ahcalBranches["ahc_nHits"]);
  m_inputTreeAHCAL->SetBranchAddress("ahc_hitI", ahc_hitI_, &ahcalBranches["ahc_hitI"]);
  m_inputTreeAHCAL->SetBranchAddress("ahc_hitJ", ahc_hitJ_, &ahcalBranches["ahc_hitJ"]);
  m_inputTreeAHCAL->SetBranchAddress("ahc_hitK", ahc_hitK_, &ahcalBranches["ahc_hitK"]);
  m_inputTreeAHCAL->SetBranchAddress("ahc_hitEnergy", ahc_hitEnergy_, &ahcalBranches["ahc_hitEnergy"]);

  AHCAL_eventID = AHCAL_Nhits = 0;

}


void DetectorConstruction::SelectConfiguration(G4int val) {

  if (_configuration != -1) return;

  default_viewpoint = 0;
  if (val == 1) defineConfigs1_2_Summer2017(dz_map, default_viewpoint);
  else if (val == 2) defineConfigs1_2_Summer2017(dz_map, default_viewpoint);
  else if (val == 3) defineConfig3_September2017(dz_map, default_viewpoint);
  else if (val == 22) defineConfig22_October2018_1(dz_map, default_viewpoint);
  else if (val == 23) defineConfig23_October2018_2(dz_map, default_viewpoint);
  else if (val == 24) defineConfig24_October2018_3(dz_map, default_viewpoint);
  else {
    std::cout << "Configuration " << val << " not implemented --> return";
    return;
  }
  _configuration = val;

  ConstructHGCal();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::DefineCommands()
{
  // Define /B5/detector command directory using generic messenger class
  fMessenger = new G4GenericMessenger(this,
                                      "/EventDisplay/setup/",
                                      "Configuration specifications");



  // configuration command
  auto& configCmd
    = fMessenger->DeclareMethod("configuration",
                                &DetectorConstruction::SelectConfiguration,
                                "Select the configuration (22-24)");
  configCmd.SetParameterName("index", true);
  configCmd.SetDefaultValue("22");


  fMessenger = new G4GenericMessenger(this,
                                      "/EventDisplay/ntuple/",
                                      "Configuration specifications");

  auto& eventCmd
    = fMessenger->DeclareMethod("showEvent",
                                &DetectorConstruction::ReadNtupleEvent,
                                "Select event for visualisation");
  eventCmd.SetParameterName("showEvent", true);
  eventCmd.SetDefaultValue(1);

  auto& ntuplePathCmd
    = fMessenger->DeclareMethod("path",
                                &DetectorConstruction::OpenHGCALNtuple,
                                "Path to the ntuple");
  ntuplePathCmd.SetDefaultValue("");

  auto& ntupleAHCALPathCmd
    = fMessenger->DeclareMethod("pathAHCAL",
                                &DetectorConstruction::OpenAHCALNtuple,
                                "Path to the AHCAL ntuple");
  ntupleAHCALPathCmd.SetDefaultValue("");


  auto& timeCutCmd
    = fMessenger->DeclareProperty("timeCut", time_cut);
  G4String guidance = "max time in terms of TOA";
  timeCutCmd.SetGuidance(guidance);
  timeCutCmd.SetParameterName("timeCut", true);
  timeCutCmd.SetDefaultValue("-1");

  auto& offsetAHCALCmd
    = fMessenger->DeclareProperty("AHCALOffset", ahcalOffset);
  guidance = "Event offset AHCAL vs HGCAL";
  offsetAHCALCmd.SetGuidance(guidance);
  offsetAHCALCmd.SetParameterName("AHCALOffset", true);
  offsetAHCALCmd.SetDefaultValue("0");

}

















//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
