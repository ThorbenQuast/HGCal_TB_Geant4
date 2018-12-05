#include "G4NistManager.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4SubtractionSolid.hh"
#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"

#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"


#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"


G4LogicalVolume* HexagonLogical(G4String name, G4double cellThickness, G4double cellSideLength, G4Material* material);


class HGCalTBMaterials {
private:
	void DefineMaterials();
	void defineSiWaferAndCells();
	void defineHGCalBaseplates();
	void defineHGCalCases();
	void defineHGCalEEAbsorbers();
	void defineHGCalFHAbsorbers();
	void defineAHCALSiPM();
	void defineAHCALAbsorbers();
	void defineBeamLineElements();

	G4VisAttributes* visAttributes;

	double alpha;	//rotation angle of Si hexagons
	G4double Si_pixel_sideLength;
	G4double Si_wafer_thickness;
	G4double Si_wafer_sideLength;

	G4double AHCAL_SiPM_xy;
	G4Box* AHCAL_SiPM_solid;

	std::map<std::string, G4double> thickness_map;
	std::map<std::string, G4LogicalVolume*> logical_volume_map;

	//all materials
	G4Material* mat_Vacuum;
	G4Material* mat_AIR;
	G4Material* mat_Ar;
	G4Material* mat_Al;
	G4Material* mat_Fe;
	G4Material* mat_Glass;
	G4Material* mat_Steel;
	G4Material* mat_Pb;
	G4Material* mat_Cu;
	G4Material* mat_W;
	G4Material* mat_Si;
	G4Material* mat_KAPTON;
	G4Material* mat_Au;
	G4Material* mat_PCB;
	G4Material* mat_Quartz;
	G4Material* mat_Polystyrene;
	G4Material* mat_CuW;

	G4LogicalVolume* Si_pixel_logical;
	G4LogicalVolume* Si_wafer_logical;
	G4LogicalVolume* CuW_baseplate_logical;
	G4LogicalVolume* Cu_baseplate_logical;
	G4LogicalVolume* PCB_baseplate_logical;
	G4LogicalVolume* Kapton_layer_logical;
	G4LogicalVolume* Al_case_logical;
	G4LogicalVolume* Steel_case_logical;
	G4LogicalVolume* Pb_absorber_EE_logical;
	G4LogicalVolume* Cu_absorber_EE_logical;
	G4LogicalVolume* W_absorber_EE_logical;
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

	std::map<std::string, int> copy_counter_map;


public:
	HGCalTBMaterials();
	void setEventDisplayColorScheme();
	G4Material* air() {return mat_AIR;}
	G4double getAHCAL_SiPM_xy() {return AHCAL_SiPM_xy;}

	void placeItemInLogicalVolume(std::string, G4double&, G4LogicalVolume*);
	G4LogicalVolume* newSiPixelHitLogical(std::string name);
	G4LogicalVolume* newSiPMHitLogical(std::string name);
};