#ifndef RunAction_h
#define RunAction_h 1

#include "G4UserRunAction.hh"
#include "EventAction.hh"
#include "G4Accumulable.hh"
#include "globals.hh"
#include "g4root.hh"
#include "G4GenericMessenger.hh"

class G4Run;

/// Run action class
///
/// In EndOfRunAction(), it calculates the dose in the selected volume 
/// from the energy deposit accumulated via stepping and event actions.
/// The computed dose is then printed on the screen.

class EventAction;


class RunAction : public G4UserRunAction
{
  public:
    RunAction(EventAction*);
    virtual ~RunAction();

    // virtual G4Run* GenerateRun();
    virtual void BeginOfRunAction(const G4Run*);
    virtual void EndOfRunAction(const G4Run*);

  private:
    EventAction* fEventAction;
  	G4String fOutputFileDir;
  	G4GenericMessenger* fMessenger;
};

#endif

