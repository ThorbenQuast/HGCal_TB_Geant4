#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"
#include <vector>


/// Event action class
///

class EventAction : public G4UserEventAction
{
  public:
    EventAction();
    virtual ~EventAction();

    virtual void BeginOfEventAction(const G4Event* event);
    virtual void EndOfEventAction(const G4Event* event);

    std::vector<G4int>     	  hits_ID;
    std::vector<G4double>     hits_x;
    std::vector<G4double>     hits_y;
    std::vector<G4double>     hits_z;
    std::vector<G4double>     hits_Edep;
    std::vector<G4double>     hits_EdepNonIonising;
    std::vector<G4double>     hits_TOA;
  private:
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

    
