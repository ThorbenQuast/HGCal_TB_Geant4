#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "G4RunManager.hh"
#include "globals.hh"
#include "G4LogicalVolume.hh"
#include <vector>
#include "G4GenericMessenger.hh"
#include "G4UImanager.hh"

#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "materials.hh"


class G4VPhysicalVolume;
class G4LogicalVolume;

struct VisHit {
    G4int layer;
    G4double x;
    G4double y;
    G4double z;
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
    std::vector<std::pair<std::string, G4double> > dz_map;
    G4double default_viewpoint;

    void DefineCommands();
    G4GenericMessenger* fMessenger;
    void SelectConfiguration(G4int val);
    G4int _configuration;

    void OpenHGCALNtuple(G4String);
    G4String ntuplepath;
    void OpenAHCALNtuple(G4String);
    G4String ntupleAHCALpath;
    void OpenTrackingNtuple(G4String);
    G4String ntupleTrackingpath;
    void OpenStandaloneNtuple(G4String);
    G4String ntupleStandalonepath;

    void VisualiseMIPMPV(G4String);

    void ReadNtupleEvent(G4int);

    HGCalTBMaterials* materials;

    void ConstructHGCal();
    void ConstructColorBar();


//ntuple readout
    std::vector<G4double> HGCalLayerDistances;
    std::vector<G4RotationMatrix*> HGCalLayerRotation;
    std::vector<VisHit*> HGCalHitsForVisualisation;
    std::vector<VisHit*> StandaloneHitsForVisualisation;

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
    std::vector<Float16_t>* rechit_toa_;
    std::vector<Float16_t>* rechit_x_;
    std::vector<Float16_t>* rechit_y_;
    std::vector<Float16_t>* rechit_z_;
    std::vector<bool>* rechit_noise_flag_;
    
    /*      //use for WGAN based ntuples
    unsigned int rechit_layer_[28*12*15];
    unsigned int rechit_module_[28*12*15];
    unsigned int rechit_chip_[28*12*15];
    unsigned int rechit_channel_[28*12*15];
    unsigned int rechit_type_[28*12*15];
    double rechit_energy_[28*12*15];
    double rechit_toa_[28*12*15];
    double rechit_x_[28*12*15];
    double rechit_y_[28*12*15];
    double rechit_z_[28*12*15];
    bool rechit_noise_flag_[28*12*15];    
    */
    std::vector<G4double> AHCALLayerDistances;
    std::vector<VisHit*> AHCALHitsForVisualisation;
    int ahcalOffset;

    std::vector<VisHit*> TrackingFramesForVisualisation;
    std::vector<int> TrackingFrame_IDs;

    float energyThreshold;
    float time_cut;

    //AHCAL readout
    TFile* m_inputFileAHCAL;
    TTree* m_inputTreeAHCAL;
    std::map<std::string, TBranch*> ahcalBranches;

    int AHCAL_eventID;
    int AHCAL_Nhits;
    std::vector<int>* ahc_hitI_;
    std::vector<int>* ahc_hitJ_;
    std::vector<int>* ahc_hitK_;
    std::vector<Float_t>* ahc_hitEnergy_;


    //Tracking file readout
    TFile* m_inputFileTracking;
    TTree* m_inputTreeTracking;
    G4String m_trackingMethod;
    std::map<std::string, TBranch*> trackingBranches;

    unsigned int Tracking_eventID;
    std::vector<unsigned int>* cluster_layer;
    std::vector<float>* cluster_x;
    std::vector<float>* cluster_y;
    std::vector<float>* cluster_module;
    std::vector<float>* cluster_chip;
    std::vector<float>* cluster_channel;
    

    //standalone simulation file
    std::map<std::string, TBranch*> standaloneBranches;
    TFile* m_inputFileStandalone;
    TTree* m_inputTreeStandalone;
    int standalone_EventID;
    std::vector<double>* standalone_x;
    std::vector<double>* standalone_y;
    std::vector<double>* standalone_z;
    std::vector<double>* standalone_E;
    std::vector<double>* standalone_TOA;

    //MPV visualisation
    std::vector<VisHit*> MIPMPVsForVisualisation;
    
    
    void DrawHits();
    void ClearHits();

//alignment correction
    std::vector<std::pair<float, float> > transverse_alignment_HGCal;    
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

