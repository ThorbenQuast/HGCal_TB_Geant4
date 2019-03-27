#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "G4RunManager.hh"
#include "globals.hh"
#include "G4LogicalVolume.hh"
#include "SiliconPixelSD.hh"
#include "SiPMSD.hh"
#include <vector>
#include "G4GenericMessenger.hh"
#include "G4UImanager.hh"
#include "G4UserLimits.hh"
#include "G4ProductionCuts.hh"

#include "materials.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;

/// Detector construction class to define materials and geometry.

class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    DetectorConstruction();
    virtual ~DetectorConstruction();

    virtual G4VPhysicalVolume* Construct();
    virtual void ConstructSDandField();
    
    G4LogicalVolume* GetScoringVolume() const { return fScoringVolume; }

    

  protected:
    G4LogicalVolume*  fScoringVolume;


  private:
    G4LogicalVolume* logicWorld;
    std::vector<std::pair<std::string, G4double> > dz_map;
    G4double default_viewpoint;
    void DefineCommands();
    G4GenericMessenger* fMessenger;
    void SelectConfiguration(G4int val);
    void SetStepSizeSilicon(G4double val);
    G4int _configuration;

    HGCalTBMaterials* materials;

    void ConstructHGCal();

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

