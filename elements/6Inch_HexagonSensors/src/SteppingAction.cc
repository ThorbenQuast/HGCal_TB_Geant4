
#include "SteppingAction.hh"
#include "EventAction.hh"
#include "DetectorConstruction.hh"

#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::SteppingAction(EventAction* eventAction)
: G4UserSteppingAction(),
  fEventAction(eventAction),
  fScoringVolume(0)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::~SteppingAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingAction::UserSteppingAction(const G4Step* aStep)
{
  if (!fScoringVolume) { 
    const DetectorConstruction* detectorConstruction
      = static_cast<const DetectorConstruction*>
        (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
    fScoringVolume = detectorConstruction->GetScoringVolume();   
  }

  // get volume of the current step
  G4LogicalVolume* volume 
    = aStep->GetPreStepPoint()->GetTouchableHandle()
      ->GetVolume()->GetLogicalVolume();

      /*
  G4int stepID = aStep->GetTrack()->GetCurrentStepNumber();
  G4String volume_name = aStep->GetTrack()->GetVolume()->GetName();
  G4String particle_name = aStep->GetTrack()->GetDynamicParticle()->GetDefinition()->GetParticleName();
  G4double pre_energy = aStep->GetPreStepPoint()->GetKineticEnergy();
  G4double post_energy = aStep->GetPostStepPoint()->GetKineticEnergy();
  G4String next_volume = "";  
  
  G4double Edeposit = aStep->GetTotalEnergyDeposit();

  std::cout<<"Step: "<<stepID<<": in "<<volume_name<<" has energy deposit: "<<Edeposit<<"  particle: "<<particle_name<<"  energy before: "<<pre_energy<<"  after: "<<post_energy<<std::endl;
  */

  // check if we are in scoring volume
  if (volume != fScoringVolume) return;

  // collect energy deposited in this step
  G4double edepStep = aStep->GetTotalEnergyDeposit();
  fEventAction->AddEdep(edepStep);  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

