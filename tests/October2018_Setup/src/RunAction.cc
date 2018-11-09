
#include "RunAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "DetectorConstruction.hh"
// #include "Run.hh"

#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4AccumulableManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction(EventAction* eventAction)
  : G4UserRunAction(),
    fEventAction(eventAction),
    fOutputFileDir("sim_HGCalOctober2018")
{

  fMessenger
    = new G4GenericMessenger(this,
                             "/HGCalOctober2018/output/",
                             "Output control");

  // randomizePrimary command
  auto& fileNameCommand
    = fMessenger->DeclareProperty("file", fOutputFileDir);
  G4String guidance
    = "Define output file location.";
  fileNameCommand.SetGuidance(guidance);
  fileNameCommand.SetParameterName("filename", true);
  fileNameCommand.SetDefaultValue("sim_HGCalOctober2018");

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::~RunAction()
{
  delete G4AnalysisManager::Instance();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run*) {
  // Create analysis manager
  // The choice of analysis technology is done via selectin of a namespacels
  auto analysisManager = G4AnalysisManager::Instance();
  G4cout << "Using " << analysisManager->GetType() << G4endl;

  // Default settings
  analysisManager->SetNtupleMerging(true);
  // Note: merging ntuples is available only with Root output
  analysisManager->SetVerboseLevel(1);
  std::cout << "Output file is: " << fOutputFileDir << std::endl;
  analysisManager->SetFileName(fOutputFileDir);

  // Book histograms, ntuple
  //

  // Creating ntuple
  //

  if ( fEventAction ) {
    analysisManager->CreateNtuple("CaloHits", "CaloHits");
    analysisManager->CreateNtupleIColumn("eventID");    // column Id = 0
    analysisManager->CreateNtupleDColumn("beamX_cm");    // column Id = 1
    analysisManager->CreateNtupleDColumn("beamY_cm");    // column Id = 2
    analysisManager->CreateNtupleDColumn("beamZ_cm");    // column Id = 3
    analysisManager->CreateNtupleIColumn("ID", fEventAction->hits_ID);    // column Id = 4
    analysisManager->CreateNtupleDColumn("x_cm", fEventAction->hits_x);    // column Id = 5
    analysisManager->CreateNtupleDColumn("y_cm", fEventAction->hits_y);    // column Id = 6
    analysisManager->CreateNtupleDColumn("z_cm", fEventAction->hits_z);    // column Id = 7
    analysisManager->CreateNtupleDColumn("Edep_keV", fEventAction->hits_Edep);    // column Id = 8
    analysisManager->CreateNtupleDColumn("EdepNonIonizing_keV", fEventAction->hits_EdepNonIonising);    // column Id = 9
    analysisManager->CreateNtupleDColumn("TOA_ns", fEventAction->hits_TOA);    // column Id = 10
    analysisManager->CreateNtupleIColumn("type", fEventAction->hits_type);    // column Id = 11
    
    analysisManager->CreateNtupleDColumn("signalSum_HGCAL_MeV");    // column Id = 12
    analysisManager->CreateNtupleDColumn("COGZ_HGCAL_cm");    // column Id = 13
    analysisManager->CreateNtupleIColumn("NHits_HGCAL");    // column Id = 14
    
    analysisManager->CreateNtupleDColumn("signalSum_AHCAL_MeV");    // column Id = 12
    analysisManager->CreateNtupleDColumn("COGZ_AHCAL_cm");    // column Id = 13
    analysisManager->CreateNtupleIColumn("NHits_AHCAL");    // column Id = 14    
    analysisManager->FinishNtuple();
  }

  // Open the output file
  analysisManager->OpenFile();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run* run)
{
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->Write();
  analysisManager->CloseFile();
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

