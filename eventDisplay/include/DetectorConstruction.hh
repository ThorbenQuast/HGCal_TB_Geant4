#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "G4LogicalVolume.hh"
#include <vector>
#include "G4GenericMessenger.hh"
#include "G4UImanager.hh"

#include "G4Box.hh"

#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"

class G4VPhysicalVolume;
class G4LogicalVolume;

/// Detector construction class to define materials and geometry.

struct VisHit {
    G4int layer;
    G4double x;
    G4double y;
    G4double energy;
    G4double red;
    G4double green;
    G4double blue;
    G4String name;
    G4VPhysicalVolume* physicalVolume;
};

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

    void OpenHGCALNtuple(G4String);
    G4String ntuplepath;
    void OpenAHCALNtuple(G4String);
    G4String ntupleAHCALpath;

    void ReadNtupleEvent(G4int);

    G4double beamLineLength;
    G4double beamLineXY;

    void ConstructHGCal();
    G4double Si_pixel_sideLength;
    G4double Si_wafer_thickness;
    double alpha;
    G4double Si_wafer_sideLength;

    G4double AHCAL_SiPM_xy;
    G4Box* AHCAL_SiPM_solid;

    std::map<std::string, G4double> thickness_map;
    std::map<std::string, G4LogicalVolume*> logical_volume_map;

    G4Material* mat_Vacuum;
    G4LogicalVolume* Si_wafer_logical;
    G4LogicalVolume* CuW_baseplate_logical;
    G4LogicalVolume* Cu_baseplate_logical;
    G4LogicalVolume* PCB_baseplate_logical;
    G4LogicalVolume* Kapton_layer_logical;
    G4LogicalVolume* Kapton_half_layer_logical;
    G4LogicalVolume* Gold_half_layer_logical;
    G4LogicalVolume* Al_case_logical;
    G4LogicalVolume* Steel_case_logical;
    G4LogicalVolume* Pb_absorber_EE_logical;
    G4LogicalVolume* Cu_absorber_EE_logical;
    G4LogicalVolume* Cu_absorber_FH_logical;
    G4LogicalVolume* Fe_absorber_FH_logical;
    G4LogicalVolume* AHCAL_SiPM_logical;
    G4LogicalVolume* AHCAL_SiPM_2x2HUB_logical;
    G4LogicalVolume* Al_absorber_AHCAL_logical;
    G4LogicalVolume* PCB_AHCAL_logical;
    G4LogicalVolume* Fe_absorber_AHCAL_logical;
    G4LogicalVolume* scintillator_logical;
    G4LogicalVolume* MCP_logical;
    G4LogicalVolume* DWC_logical;
    G4LogicalVolume* DWC_gas_logical;



    std::vector<G4double> HGCalLayerDistances;
    std::vector<VisHit*> HGCalHitsForVisualisation;
//ntuple readout
    TFile* m_inputFileHGCal;
    TTree* m_inputTreeHGCal;
    std::map<std::string, TBranch*> hgcalBranches;
    unsigned int eventID;
    unsigned int Nhits;
    std::vector<unsigned int>* rechit_layer_;
    std::vector<unsigned int>* rechit_module_;
    std::vector<unsigned int>* rechit_chip_;
    std::vector<unsigned int>* rechit_channel_;
    std::vector<unsigned int>* rechit_type_;
    std::vector<Float16_t>* rechit_energy_;
    std::vector<Float16_t>* rechit_x_;
    std::vector<Float16_t>* rechit_y_;
    std::vector<Float16_t>* rechit_z_;

    std::vector<G4double> AHCALLayerDistances;
    std::vector<VisHit*> AHCALHitsForVisualisation;
    int ahcalOffset;

    TFile* m_inputFileAHCAL;
    TTree* m_inputTreeAHCAL;
    std::map<std::string, TBranch*> ahcalBranches;

    int AHCAL_eventID;
    int AHCAL_Nhits;
    int ahc_hitI_[24*24*40];
    int ahc_hitJ_[24*24*40];
    int ahc_hitK_[24*24*40];
    float ahc_hitEnergy_[24*24*40];


//alignment correction
    std::vector<std::pair<float, float> > transverse_alignment_HGCal;    
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

