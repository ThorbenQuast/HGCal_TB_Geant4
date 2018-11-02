#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "G4LogicalVolume.hh"
#include <vector>
#include "G4GenericMessenger.hh"

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
    G4LogicalVolume* Si_pixel_logical;
    
    void DefineCommands();
    G4GenericMessenger* fMessenger;
    void SelectConfiguration(G4int val);
    G4int _configuration;


    G4double beamLineLength;
    G4double beamLineXY;

    void ConstructHGCal();
    G4double Si_pixel_sideLength;
    G4double Si_wafer_thickness;
    double alpha;
    G4double Si_wafer_sideLength;

    std::map<std::string, G4double> thickness_map;
    std::map<std::string, G4LogicalVolume*> logical_volume_map;

    G4LogicalVolume* Si_wafer_logical;
    G4LogicalVolume* CuW_baseplate_logical;
    G4LogicalVolume* Cu_baseplate_logical;
    G4LogicalVolume* PCB_baseplate_logical;
    G4LogicalVolume* Kapton_layer_logical;
    G4LogicalVolume* Al_case_logical;
    G4LogicalVolume* Steel_case_logical;
    G4LogicalVolume* Pb_absorber_EE_logical;
    G4LogicalVolume* Cu_absorber_EE_logical;
    G4LogicalVolume* Cu_absorber_FH_logical;
    G4LogicalVolume* Fe_absorber_FH_logical;
    G4LogicalVolume* scintillator_logical;
    G4LogicalVolume* DWC_logical;
    G4LogicalVolume* DWC_gas_logical;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

