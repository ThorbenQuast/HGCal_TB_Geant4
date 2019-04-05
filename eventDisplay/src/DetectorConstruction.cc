#include "DetectorConstruction.hh"

#include "colors.hh"

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
  ntuplepath = "";
  energyThreshold = 0;
  time_cut = -1;
  m_inputFileHGCal = NULL;
  m_inputTreeHGCal = NULL;

  m_inputFileAHCAL = NULL;
  m_inputTreeAHCAL = NULL;

  m_inputFileTracking = NULL;
  m_inputTreeTracking = NULL;
  m_trackingMethod = "HGCalMIPTracking";
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  HGCalLayerDistances.clear();
  HGCalLayerRotation.clear();
  AHCALLayerDistances.clear();

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
      G4RotationMatrix* rot = new G4RotationMatrix;
      if (item_type.find("_rot30") != std::string::npos) rot->rotateZ(30 * deg);
      else rot->rotateZ(0);
      HGCalLayerRotation.push_back(rot);
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

  ClearHits();


  //HGCal hits
  if ((m_inputTreeHGCal != NULL) && (m_inputFileHGCal->IsOpen())) {
    for (unsigned int i = 0; i < m_inputTreeHGCal->GetEntries(); i++) {
      m_inputTreeHGCal->GetEntry(i);
      if (eventID == eventIndex) break;
    }
    if (eventID != eventIndex) std::cout << "[WARNING] loaded HGCAL event " << eventID << " !=" << eventIndex << std::endl;

    for (unsigned int nhit = 0; nhit < Nhits; nhit++) {
      if (rechit_noise_flag_->at(nhit)) continue;

      Float16_t hit_energy = rechit_energy_->at(nhit);
      if (hit_energy < energyThreshold) continue;

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

      float hit_toa = rechit_toa_->at(nhit);
      if ((time_cut >= 0) && (hit_toa > time_cut)) continue;
      if ((time_cut >= 0) && (hit_toa < 0)) continue;

      VisHit* hit = new VisHit;
      hit->name = "HGCAL-Hit";
      hit->layer = rechit_layer_->at(nhit);
      hit->x = rechit_x_->at(nhit) * cm;
      hit->y = rechit_y_->at(nhit) * cm;
      hit->energy = hit_energy;

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


  //Tracking frames
  if (time_cut < 0) {
    if ((m_inputTreeTracking != NULL) && (m_inputFileTracking->IsOpen())) {
      //loop over all tracks
      for (unsigned int i = 0; i < m_inputTreeTracking->GetEntries(); i++) {
        m_inputTreeTracking->GetEntry(i);
        if (Tracking_eventID != eventIndex) continue;
        for (int ncluster = 0; ncluster < cluster_layer->size(); ncluster++) {
          int clusterID = 1000 * cluster_module->at(ncluster) + 100 * cluster_chip->at(ncluster) + cluster_channel->at(ncluster);
          if (std::find(TrackingFrame_IDs.begin(), TrackingFrame_IDs.end(), clusterID) != TrackingFrame_IDs.end()) continue;

          VisHit* frame = new VisHit;
          frame->name = "Tracking-Frame";
          frame->layer = cluster_layer->at(ncluster);
          frame->x = cluster_x->at(ncluster) * cm;
          frame->y = cluster_y->at(ncluster) * cm;
          frame->energy = 0;
          setTrackingFrameColor(frame);

          TrackingFramesForVisualisation.push_back(frame);
          TrackingFrame_IDs.push_back(clusterID);
        }
      }
      std::cout << "Number of unqiue tracking frames in file: " << TrackingFrame_IDs.size() << std::endl;
    }
  } else {
    std::cout << "timing cut: " << time_cut << " prevents tracking frames to be visualised" << std::endl;
  }

  DrawHits();
}

void DetectorConstruction::ClearHits() {
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

  for (size_t nframe = 0; nframe < TrackingFramesForVisualisation.size(); nframe++) {
    VisHit* hit = TrackingFramesForVisualisation[nframe];
    hit->physicalVolume->SetTranslation(G4ThreeVector(0, 0., -BEAMLINELENGTH / 2));
    hit->physicalVolume->GetLogicalVolume()->SetVisAttributes(visAttributes);
    delete hit;
  }
  TrackingFramesForVisualisation.clear();
  TrackingFrame_IDs.clear();

  for (size_t nhit = 0; nhit < MIPMPVsForVisualisation.size(); nhit++) {
    VisHit* hit = MIPMPVsForVisualisation[nhit];
    hit->physicalVolume->SetTranslation(G4ThreeVector(0, 0., -BEAMLINELENGTH / 2));
    hit->physicalVolume->GetLogicalVolume()->SetVisAttributes(visAttributes);
    delete hit;
  }
  MIPMPVsForVisualisation.clear();
}

void DetectorConstruction::DrawHits() {
  G4VisAttributes* visAttributes = new G4VisAttributes(G4Colour(1, 1, 1, 0.));
  visAttributes->SetVisibility(false);
  /*****    START GENERIC HIT PLACEMENT ALGORITHM FOR THE SETUP  *****/
  for (size_t nhit = 0; nhit < HGCalHitsForVisualisation.size(); nhit++) {
    VisHit* hit = HGCalHitsForVisualisation[nhit];
    G4LogicalVolume* hit_logical = materials->newSiPixelHitLogical(hit->name);
    visAttributes = new G4VisAttributes(G4Colour(hit->red, hit->green, hit->blue, 1.));
    visAttributes->SetVisibility(true);
    hit_logical->SetVisAttributes(visAttributes);
    hit->physicalVolume = new G4PVPlacement(HGCalLayerRotation[hit->layer - 1], G4ThreeVector(hit->x, hit->y, HGCalLayerDistances[hit->layer - 1]), hit_logical, hit->name, logicWorld, false, 0, false);
  }


  for (size_t nhit = 0; nhit < AHCALHitsForVisualisation.size(); nhit++) {
    VisHit* hit = AHCALHitsForVisualisation[nhit];
    G4LogicalVolume* hit_logical = materials->newSiPMHitLogical(hit->name);
    visAttributes = new G4VisAttributes(G4Colour(hit->red, hit->green, hit->blue, 1.));
    visAttributes->SetVisibility(true);
    hit_logical->SetVisAttributes(visAttributes);
    hit->physicalVolume = new G4PVPlacement(0, G4ThreeVector(hit->x, hit->y, AHCALLayerDistances[hit->layer - 1]), hit_logical, hit->name, logicWorld, false, 0, false);
  }


  for (size_t nframe = 0; nframe < TrackingFramesForVisualisation.size(); nframe++) {
    VisHit* hit = TrackingFramesForVisualisation[nframe];
    G4LogicalVolume* hit_logical = materials->newSiPixelHitFrameLogical(hit->name, 1 * mm);
    visAttributes = new G4VisAttributes(G4Colour(hit->red, hit->green, hit->blue, 1.));
    visAttributes->SetVisibility(true);
    hit_logical->SetVisAttributes(visAttributes);
    hit->physicalVolume = new G4PVPlacement(0, G4ThreeVector(hit->x, hit->y, HGCalLayerDistances[hit->layer - 1]), hit_logical, hit->name, logicWorld, false, 0, false);
  }


  for (size_t nhit = 0; nhit < MIPMPVsForVisualisation.size(); nhit++) {
    VisHit* hit = MIPMPVsForVisualisation[nhit];
    G4LogicalVolume* hit_logical = materials->newSiPixelHitLogical(hit->name);
    visAttributes = new G4VisAttributes(G4Colour(hit->red, hit->green, hit->blue, 1.));
    visAttributes->SetVisibility(true);
    hit_logical->SetVisAttributes(visAttributes);
    hit->physicalVolume = new G4PVPlacement(HGCalLayerRotation[hit->layer - 1], G4ThreeVector(hit->x, hit->y, HGCalLayerDistances[hit->layer - 1]), hit_logical, hit->name, logicWorld, false, 0, false);
  }

  G4RunManager::GetRunManager()->GeometryHasBeenModified();
  G4UImanager* UImanager = G4UImanager::GetUIpointer();
  UImanager->ApplyCommand("/vis/drawVolume");

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
  if (m_inputFileHGCal->IsOpen()) {
    std::cout << "Opened " << ntuplepath << std::endl;
    m_inputTreeHGCal->SetBranchAddress("event", &eventID, &hgcalBranches["eventID"]);
    m_inputTreeHGCal->SetBranchAddress("NRechits", &Nhits, &hgcalBranches["Nhits"]);
    m_inputTreeHGCal->SetBranchAddress("rechit_layer", &rechit_layer_, &hgcalBranches["rechit_layer_"]);
    m_inputTreeHGCal->SetBranchAddress("rechit_module", &rechit_module_, &hgcalBranches["rechit_module_"]);
    m_inputTreeHGCal->SetBranchAddress("rechit_chip", &rechit_chip_, &hgcalBranches["rechit_chip_"]);
    m_inputTreeHGCal->SetBranchAddress("rechit_channel", &rechit_channel_, &hgcalBranches["rechit_channel_"]);
    m_inputTreeHGCal->SetBranchAddress("rechit_type", &rechit_type_, &hgcalBranches["rechit_type_"]);
    m_inputTreeHGCal->SetBranchAddress("rechit_energy", &rechit_energy_, &hgcalBranches["rechit_energy_"]);
    m_inputTreeHGCal->SetBranchAddress("rechit_toaRise_time", &rechit_toa_, &hgcalBranches["rechit_toa_"]);
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
  } else {
    m_inputFileHGCal = NULL;
    std::cout << "HGCAl file: " << ntuplepath << " not opened..." << std::endl;
  }

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
  if (m_inputFileAHCAL->IsOpen()) {
    std::cout << "Opened " << ntupleAHCALpath << std::endl;
    m_inputTreeAHCAL->SetBranchAddress("eventNumber", &AHCAL_eventID, &ahcalBranches["eventNumber"]);
    m_inputTreeAHCAL->SetBranchAddress("ahc_nHits", &AHCAL_Nhits, &ahcalBranches["ahc_nHits"]);
    m_inputTreeAHCAL->SetBranchAddress("ahc_hitI", ahc_hitI_, &ahcalBranches["ahc_hitI"]);
    m_inputTreeAHCAL->SetBranchAddress("ahc_hitJ", ahc_hitJ_, &ahcalBranches["ahc_hitJ"]);
    m_inputTreeAHCAL->SetBranchAddress("ahc_hitK", ahc_hitK_, &ahcalBranches["ahc_hitK"]);
    m_inputTreeAHCAL->SetBranchAddress("ahc_hitEnergy", ahc_hitEnergy_, &ahcalBranches["ahc_hitEnergy"]);
    AHCAL_eventID = AHCAL_Nhits = 0;
  } else {
    m_inputFileAHCAL = NULL;
    std::cout << "AHCAL file: " << ntupleAHCALpath << " not opened..." << std::endl;
  }


}

void DetectorConstruction::OpenTrackingNtuple(G4String path) {
  //reading from the file
  if (path == "") {
    return;
  } else {

  }
  if (m_inputFileTracking == NULL) {
    m_inputFileTracking = new TFile(path.c_str(), "READ");
    m_inputTreeTracking = (TTree*)m_inputFileTracking->Get(("corryvreckan/" + m_trackingMethod + "/HGCalTracking_Tracks").c_str());

    trackingBranches["eventID"] = new TBranch;
    trackingBranches["cluster_layer"] = new TBranch;
    trackingBranches["cluster_x"] = new TBranch;
    trackingBranches["cluster_y"] = new TBranch;
    trackingBranches["cluster_module"] = new TBranch;
    trackingBranches["cluster_chip"] = new TBranch;
    trackingBranches["cluster_channel"] = new TBranch;
  } else {
    std::cout << "Closing " << ntupleTrackingpath << std::endl;
    m_inputFileTracking->Close();
    m_inputFileTracking = new TFile(path.c_str(), "READ");
    m_inputTreeTracking = (TTree*)m_inputFileTracking->Get(("corryvreckan/" + m_trackingMethod + "/HGCalTracking_Tracks").c_str());
  }
  ntupleTrackingpath = path;
  if (m_inputFileTracking->IsOpen()) {
    std::cout << "Opened " << ntupleTrackingpath << std::endl;
    m_inputTreeTracking->SetBranchAddress("eventID", &Tracking_eventID, &trackingBranches["eventID"]);
    m_inputTreeTracking->SetBranchAddress("associated_cluster_layer", &cluster_layer, &trackingBranches["cluster_layer"]);
    m_inputTreeTracking->SetBranchAddress("associated_cluster_x", &cluster_x, &trackingBranches["cluster_x"]);
    m_inputTreeTracking->SetBranchAddress("associated_cluster_y", &cluster_y, &trackingBranches["cluster_y"]);
    m_inputTreeTracking->SetBranchAddress("associated_cluster_module", &cluster_module, &trackingBranches["cluster_module"]);
    m_inputTreeTracking->SetBranchAddress("associated_cluster_chip", &cluster_chip, &trackingBranches["cluster_chip"]);
    m_inputTreeTracking->SetBranchAddress("associated_cluster_channel", &cluster_channel, &trackingBranches["cluster_channel"]);
  } else {
    m_inputFileTracking = NULL;
    std::cout << "Tracking file: " << ntupleTrackingpath << " not opened..." << std::endl;
  }

}


void DetectorConstruction::VisualiseMIPMPV(G4String path) {
  //reading from the file
  if (path == "") {
    return;
  }


  TFile* m_inputFileMIPMPV = new TFile(path.c_str(), "READ");
  if (m_inputFileMIPMPV->IsOpen()) {
    std::cout << "Opened " << path << std::endl;
    TTree* m_inputTreeMIPMPV = (TTree*)m_inputFileMIPMPV->Get("tree");
    ClearHits();

    trackingBranches["calib_layer"] = new TBranch;
    trackingBranches["calib_module"] = new TBranch;
    trackingBranches["calib_chip"] = new TBranch;
    trackingBranches["calib_channel"] = new TBranch;
    trackingBranches["calib_MPV"] = new TBranch;
    trackingBranches["calib_x"] = new TBranch;
    trackingBranches["calib_y"] = new TBranch;
    trackingBranches["calib_z"] = new TBranch;

    int calib_layer;
    int calib_module;
    int calib_chip;
    int calib_channel;
    float calib_MPV;
    float calib_x;
    float calib_y;
    float calib_z;

    m_inputTreeMIPMPV->SetBranchAddress("calib_layer", &calib_layer, &trackingBranches["calib_layer"]);
    m_inputTreeMIPMPV->SetBranchAddress("calib_module", &calib_module, &trackingBranches["calib_module"]);
    m_inputTreeMIPMPV->SetBranchAddress("calib_chip", &calib_chip, &trackingBranches["calib_chip"]);
    m_inputTreeMIPMPV->SetBranchAddress("calib_channel", &calib_channel, &trackingBranches["calib_channel"]);
    m_inputTreeMIPMPV->SetBranchAddress("calib_MPV", &calib_MPV, &trackingBranches["calib_MPV"]);
    m_inputTreeMIPMPV->SetBranchAddress("calib_x", &calib_x, &trackingBranches["calib_x"]);
    m_inputTreeMIPMPV->SetBranchAddress("calib_y", &calib_y, &trackingBranches["calib_y"]);
    m_inputTreeMIPMPV->SetBranchAddress("calib_z", &calib_z, &trackingBranches["calib_z"]);

    std::cout << "Number of entries: " << m_inputTreeMIPMPV->GetEntries() << std::endl;
    for (unsigned int i = 0; i < m_inputTreeMIPMPV->GetEntries(); i++) {
      m_inputTreeMIPMPV->GetEntry(i);
      VisHit* hit = new VisHit;
      hit->name = "MIP-MPV";
      hit->layer = calib_layer;
      hit->x = calib_x * cm;
      hit->y = calib_y * cm;
      hit->energy = calib_MPV;

      setMIPMPVColor(hit);

      MIPMPVsForVisualisation.push_back(hit);
    }

    m_inputFileMIPMPV->Close();
    delete m_inputFileMIPMPV;

    DrawHits();
  } else {
    std::cout << "Cannot find " << path << std::endl;
  }

};

void DetectorConstruction::SelectConfiguration(G4int val) {

  if (_configuration != -1) return;

  default_viewpoint = 0;
  if (val == 1) defineConfigs1_2_Summer2017(dz_map, default_viewpoint);
  else if (val == 2) defineConfigs1_2_Summer2017(dz_map, default_viewpoint);
  else if (val == 3) defineConfig3_September2017(dz_map, default_viewpoint);
  else if (val == 15) defineDESY2018_Config42(dz_map, default_viewpoint);
  else if (val == 17) defineConfigs17To21_June2018(dz_map, default_viewpoint);
  else if (val == 18) defineConfigs17To21_June2018(dz_map, default_viewpoint);
  else if (val == 19) defineConfigs17To21_June2018(dz_map, default_viewpoint);
  else if (val == 21) defineConfigs17To21_June2018(dz_map, default_viewpoint);
  else if (val == 22) defineConfig22_October2018_1(dz_map, default_viewpoint);
  else if (val == 23) defineConfig23_October2018_2(dz_map, default_viewpoint);
  else if (val == 24) defineConfig24_October2018_3(dz_map, default_viewpoint);
  else if (val == 25) defineConfig25_October2018_3_parasitic(dz_map, default_viewpoint);
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

  auto& visualiseMIPMPVCmd
    = fMessenger->DeclareMethod("VisualiseMIPMPV",
                                &DetectorConstruction::VisualiseMIPMPV,
                                "Path to the ntuple file with MIP MPVs");
  visualiseMIPMPVCmd.SetDefaultValue("");



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


  auto& ntupleTrackingPathCmd
    = fMessenger->DeclareMethod("pathTracking",
                                &DetectorConstruction::OpenTrackingNtuple,
                                "Path to the Tracking ntuple");
  ntupleTrackingPathCmd.SetDefaultValue("");

  auto& TrackingMethodCmd
    = fMessenger->DeclareProperty("TrackingMethod", m_trackingMethod);
  TrackingMethodCmd.SetParameterName("TrackingMethod", true);
  TrackingMethodCmd.SetDefaultValue("HGCalMIPTracking");



  auto& energyThresholdCmd
    = fMessenger->DeclareProperty("energyThreshold", energyThreshold);
  G4String guidance = "minimum energy for visualisation in MIPs";
  energyThresholdCmd.SetGuidance(guidance);
  energyThresholdCmd.SetParameterName("energyThreshold", true);
  energyThresholdCmd.SetDefaultValue("0");


  auto& timeCutCmd
    = fMessenger->DeclareProperty("timeCut", time_cut);
  guidance = "max time in terms of TOA";
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
