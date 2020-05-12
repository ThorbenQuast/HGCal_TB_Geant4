
#include "SteppingAction.hh"
#include "EventAction.hh"
#include "DetectorConstruction.hh"

#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"

#include "G4VProcess.hh"
#include "G4StepPoint.hh"

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


    //investigate pre-step 
  G4StepPoint* preStepPoint = aStep->GetPreStepPoint();
  if (preStepPoint&&preStepPoint->GetProcessDefinedStep()) {
    if((preStepPoint->GetProcessDefinedStep()->GetProcessType()==4)&&(preStepPoint->GetProcessDefinedStep()->GetProcessSubType()==121)) {    //Hadronic interaction has process type == 4 and inelastic interaction has SubProcessType==121
      G4double pre_step_time = preStepPoint->GetGlobalTime() / CLHEP::ns;
      if (pre_step_time < this->fEventAction->firstHadInteractionTime) {
        this->fEventAction->firstHadInteractionDepth = preStepPoint->GetPosition().z() / CLHEP::cm;
        this->fEventAction->firstHadInteractionTime = pre_step_time;
      }
    }
  }
    //investigate post-step
  G4StepPoint* postStepPoint = aStep->GetPostStepPoint();
  if (postStepPoint&&postStepPoint->GetProcessDefinedStep()) {
    if((postStepPoint->GetProcessDefinedStep()->GetProcessType()==4)&&(postStepPoint->GetProcessDefinedStep()->GetProcessSubType()==121)) {    //Hadronic interaction has process type == 4
      G4double post_step_time = postStepPoint->GetGlobalTime() / CLHEP::ns;
      if (post_step_time < this->fEventAction->firstHadInteractionTime) {
        this->fEventAction->firstHadInteractionDepth = postStepPoint->GetPosition().z() / CLHEP::cm;
        this->fEventAction->firstHadInteractionTime = post_step_time;
      }
    }
  }

  // check if we are in scoring volume
  if (volume != fScoringVolume) return;
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

