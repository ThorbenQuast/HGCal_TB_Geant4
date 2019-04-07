# HGCal Beam Tests 2017-18: Geant4 Based Event Display & Standalone Simulation

## Scope
This repository contains the source code for two programs, namely one for **event visualisation** and one for **standalone simulation of CMS High Granularity Calorimeter prototypes** as they were tested with paricle beam in 2017-18. The programs are based on libraries and functionalities provided by the **Geant4 framework**.
Implemented geometries are intended to only resemble the prototpye configurations like they were tested in reality. An **accurate correspondence between the real and implemented configuration is not guaranteed**.

## Software requirements
These two programs were developed and are tested with the following software:

* GEANT4 10.4 (mandatory for compilation and execution)
* ROOT 6.14 (mandatory for compilation and execution)
* cmake 3.4 (mandatory for compilation)
* QT5 or OpenGL (recommended, for visualisation of the geometry and the particle showering)

## Event Display
### Installation
* ```cd /simulation```
* ```mkdir build; cd build```
* ```cmake ..``` 
* ```make install```, warnings can be ignored.

In case of successful compilation, an executable called ```EventDisplay``` should be added to the ```build``` directory.

### Running the display
The default command to initialise the display executed from the ```build``` directory is simply: ```./EventDisplay```.
Alternatively, a dedicated steering file can be passed as command line argument to the program. If none is specified, ```init_vis.mac``` is taken. This file starts the visualisation tool and loads more detailed settings from ```vis.mac```. As example, the configuration is set to 22, an energy threshold is defined and both reconstructed data from HGCal and AHCAL are linked to the program in this file.

### Commands
* ```EventDisplay/setup/configuration <index>```: Load the defined geometry. Ineffective if set index is not implemented. This command can only be run per session, i.e. only one geometry can be defined instance of the program.
* ```EventDisplay/ntuple/path <string>```: Path to the reconstructed HGC ntuple files. These ntuples must be reconstructed with the October2018_v12 software tag, or newer. 
* ```EventDisplay/ntuple/pathAHCAL <string>```: Path to the reconstructed AHCAL ntuple files. v2 reconstruction of the data accumulated in October 2018 is supported.
* ```EventDisplay/ntuple/pathAHCAL <string>```: Path to the reconstructed AHCAL ntuple files. v2 reconstruction of the data accumulated in October 2018 is supported.
* ```EventDisplay/ntuple/AHCALOffset <int>```: Applies an offset to the event number in the AHCAL ntuples. 
* ```EventDisplay/ntuple/energyThreshold <float>```: Only deposits above this energy threshold are visualised (Unit: MIPs).
* ```EventDisplay/ntuple/timeCut <float>```: Only deposits with time of arrival below this value are visualised (Unit: nano seconds). For any positive timeCut, only HGCal hits are displayed.
* ```EventDisplay/ntuple/showEvent <int>```: Reads the specified event number and visualises the hits.

### Color schemes
The event display makes use of a hard coded version of the ROOT rainbow color scheme for colorisation of calorimeter hits. The energy scale ranges from 0 to 500 MIPs. Modifications can be implemented in ```/simulation/include/colors.hh```.

## Standalone Simulation
### Installation
* ```cd /simulation```
* ```mkdir build; cd build```
* ```cmake ..``` 
* ```make install```, warnings can be ignored.

In case of successful compilation, an executable called ```Simulation``` should be added to the ```build``` directory.

### Running the simulation
The standalone simulation can be run either using the visualisation tool like for the EventDisplay (QT5 or OpenGL required) or without. While the particle type, momentum and place of the beam is configurable, its direction is always parallel to the z-axis.

#### With visualisation
The default command to initialise the simulation with visualisation is analogous to the event display: ```./Simulation```.
Alternatively, a dedicated steering file can be passed as command line argument to the program. If none is specified, ```init_vis.mac``` is taken. This file starts the visualisation tool and loads more detailed settings from ```vis.mac```. As example, the configuration is set to 22, the output file path is set to *HGCalTB\_October2018\_config22\_pi+\_150GeV.root* and the particle gun is set to generate 150 GeV pions.

#### Without visualisation
In order to run the simulation without visualisation, adjust the ```run.mac``` steering file to your needs and execute it with ```./Simulation run.mac```.
In the provided example file, four physics tunes (step sizes) are simulated sequentially for 10000 200 GeV muons and 50 GeV electrons each. Eight output files are produced in total.

### Output file
Per-event calorimeter hits are computed and written out to file after each run in form of a *ROOT-TTree*. Its branches are as follows:

* ```eventID```: Event counter.
* ```beamX(Y,Z)_cm```: Position of the particle gun in X(Y,Z) [unit: cm].
* ```ID```: Unique ID of a calorimeter hit.
* ```x(y,z)_cm```: Position of the calorimeter hit's origin in the global X(Y,Z) [unit: cm].
* ```Edep_keV```: Summed energy (from ionization) over all steps inside the sensitive volume [unit: keV].
* ```EdepNonIonizing_keV```: Summed non-ionizing energy  over all steps inside the sensitive volume [unit: keV].
* ```TOA_ns```: Time of the step for which a predefined energy threshold in the sum within the hit volume is exceeded [unit: ns].
* ```TOA_last_ns```: Time of the last step  within the hit volume [unit: ns].
* ```type```: Type of the hit, 0=HGCal (silicon cell), 1=AHCAL (scintillator).

In addition, three typical calorimeter observables are computed.

* ```signalSum_HGCAL(AHCAL)_MeV```: Energy sum of all hits within the HGCal (AHCAL) prototypes [unit: MeV].
* ```COGZ_HGCAL(AHCAL)_cm```: Depth of the shower characterised by the center-of-gravity along the beam axis within the HGCal (AHCAL) prototypes [unit: cm].
* ```NHits_HGCAL(AHCAL)_MeV```: Number of all hits within the HGCal (AHCAL) prototypes.

### Commands

## Implemented Geometries
Geometries and the corresponding material are commonly implemented for both the event display and the standalone simulation. The source code is located in ```/common/``` and it is compiled together with either program.

### Implemented material
All materials are defined globally and their visualisation attributes are set in ```/common/src/materials.cc```. Pointers to the logical volumes are stored within an ```std::map<std::string, G4LogicalVolume*>``` map which is available to both programs. 

#### Sensitive material
Geant4 steps within the volume of placed sensitive material are tracked and serve as input for the computation of calorimeter hits. For that, ionizing and non-ionizing energy contributions within each sensitive volume are summed and written to a file for each shower. The following materials are defined as sensitive.
##### Hexagoal HGC silicon diodes
Defined in ```void HGCalTBMaterials::defineSiWaferAndCells()```.

* "SiCell": Hexagon with side length of 0.6496345 cm and thickness of 0.3 mm, filled with silicon. This is the model for a full ~1cm2 HGC silicon diode.
* "SiWafer": Hexagon with side length of 11x 0.6496345 cm and thickness of 0.3 mm, filled with air. "SiCell" volumes are placed into this wafer making a 6'' HGC prototype silicon sensor made from full hexagonal diodes only.

##### AHCAL scintillator tiles
Defined in ```void HGCalTBMaterials::defineAHCALSiPM()```.

* "AHCAL_SiPM": Rectangular box with x-y length of 3 cm and thickness of 5.4 mm filled with Polystyrene. This is the model for a CALICE AHCAL scintillator tile.
* "AHCAL\_SiPM\_2x2HUB": Rectangular box with x-y length of 24x3 cm and thickness of 5.4 mm filled with air. "AHCAL_SiPM" volumes are placed into the hub making a full sensitive layer of the CALICE AHCAL technical prototype (2018).

#### Passive material
Passive material contributes to the shower evolution but any information on deposited energy therein is not tracked.
##### HGCal baseplates
Defined in ```void HGCalTBMaterials::defineHGCalBaseplates()```.

* "CuW_baseplate": Hexagonal plate with same size as the "SiWafer" and a thickness of 1.2 mm. Filled with 60% copper and 40% tungsten.
* "Cu_baseplate": Hexagonal plate with same size as the "SiWafer" and a thickness of 1.2 mm. Filled with copper.
* "PCB": Hexagonal plate with same size as the "SiWafer" and a thickness of 1.2 mm. Filled with carbon.
* "Kapton_layer": Hexagonal plate with same size as the "SiWafer" and a thickness of 0.05 mm. Filled with G4\_KAPTON.

##### HGCal cases
Defined in ```void HGCalTBMaterials::defineHGCalCases()```.

* "Al_case": Rectangular box with x-y length of 40 cm and thickness of 2.6 mm filled with aluminium.
* "Steel_case": Rectangular box with x-y length of 60 cm and thickness of 2.6 mm filled with stainless steel.

##### HGCal EE absorbers
Defined in ```void HGCalTBMaterials::defineHGCalEEAbsorbers()```.

* "Pb_absorber\_EE": Rectangular box with x-y length of 30 cm and thickness of 4.9 mm filled with lead.
* "Cu_absorber\_EE": Rectangular box with x-y length of 30 cm and thickness of 6.0 mm filled with copper.
* "W_absorber\_EE": Rectangular box with x-y length of 30 cm and thickness of 2.8 mm filled with tungsten.
* "W\_2mm\_absorber\_EE_DESY2018": Rectangular box with x-y length of 15 cm and thickness of 2.0 mm filled with tungsten.
* "W\_4mm\_absorber\_EE_DESY2018": Rectangular box with x-y length of 15 cm and thickness of 4.0 mm filled with tungsten.

##### HGCal FH absorbers
Defined in ```void HGCalTBMaterials::defineHGCalFHAbsorbers()```.

* "Cu_absorber\_FH": Rectangular box with x-y length of 50 cm and thickness of 6.0 mm filled with copper.
* "Fe_absorber\_FH": Rectangular box with x-y length of 50 cm and thickness of 41.0 mm filled with stainless steel.

##### AHCAL absorbers
Defined in ```void HGCalTBMaterials::defineAHCALAbsorbers()```.

* "Al_absorber\_AHCAL": Rectangular box with x-y length of 72 cm and thickness of 1.0 mm filled with aluminium.
* "PCB_AHCAL": Rectangular box with x-y length of 72 cm and thickness of 1.2 mm filled with carbon.
* "Fe\_absorber_AHCAL": Rectangular box with x-length of 80.8 cm, y-length of 65.7 cm and thickness of 17.0 mm filled with stainless steel.

##### Beam line elements
Defined in ```void HGCalTBMaterials::defineBeamLineElements()```.

* "Scintillator": 10x10x1 cm3 box filled with carbon.
* "MCP": Disk with radius of 2 cm and a thickness of 10.0 mm. Filled with quartz.
* "Al_chip": 1x1x0.2 cm3 box filled with alumnium.
* "DWC": 10x10x1 cm3 box filled with glas. Inside cavity with 2mm wall thickness is filled with argon.
* "DATURA_full": 10x10x1.5 cm3 box with a 2x1 cm2 central window filled with alumnium.

##### HERD calorimeter
In October 2018, the HGCal prototype was tested with parasitic beam during a test of the HERD collaboration in the same beam line. A simplied model of its calorimeter is defined in ```void HGCalTBMaterials::defineHERDCalorimeter()```.


### Configurations
The subsequent sequence of placement of the geometries is defined for different calorimeter configurations.
Within the event display and the standalone simulation, defined materials are stacked such that their origin is positioned on the beam axis and their coordinate system is aligned with the global one. Shifts or rotations with respect to that coordinate system are not supported at this point.

#### Special arrangements
Exceptions to this linear stacking are possible using the following postfixes:

* "\_DAISY": This arranges the geometry in a manner identical to how the silicon modules in the FH layers during the October 2018 beam tests were placed. Only "SiWafers" are supported.
* "\_SUMMER2017TRIPLET": This arranges the geometry in a manner identical to how the silicon modules in the FH layers during the July 2017 beam tests were placed. Only "SiWafers" are supported.
* "_rot30": A global rotation of the geometry in x-y by 30 deg. is applied.


#### !Important Disclaimer!
**Geometries defined below are motivated by the tested HGC calorimeter geometries in the years 2017-18. However, nothing more than a resemblance is guaranteed at this point. A detailed comparison of results from this simulation to data is highly deprecated!**

  
#### Summer/Fall 2017 (CERN, H2)
* Indexes 1 and 2 - ```/common/src/config1_2_Summer2017.cc```: 2 silicon layers in EE, 4 silicon layers in FH.
* Index 3 - ```/common/src/config3_September2017.cc```: 7 silicon layers in EE, 10 silicon layers in FH.

#### March 2018 (DESY, T21)
* Index 15 - ```/common/src/config_DESY2018_42.cc```: 1 silicon module in between the DATURA arms, 2 silicon modules behind 4x 4mm tungsten plates downstream.

#### June 2018 (CERN, H2)
* Indexes 17 to 21 -```/common/src/configs17To21_June2018.cc``` - indexes 17-21: 28-layer fully equipped EE HGC prototype.

#### October 2018 (CERN, H2)
* Index 22 - ```/common/src/config22_October2018_1.cc```: 28 silicon layers in EE, 12 layers in FH with first nine including daisy configurations of the silicon modules. 
* Index 23 - ```/common/src/config23_October2018_2.cc```: 28 silicon layers in EE, 11 layers in FH with last nine including daisy configurations of the silicon modules. 
* Index 24 - ```/common/src/config24_October2018_3.cc```: 8 silicon layers in EE, 12 layers in FH with all including daisy configurations of the silicon modules. 
* Index 25 - ```/common/src/config24_October2018_3_parasitic.cc```: 8 silicon layers in EE, 12 layers in FH with all including daisy configurations of the silicon modules. The HERD calorimeter model is placed upstream in the beam line with respect to the HGC calorimeter prototype model.

#### Test configurations
A few extra configuratons are defined which may serve as reference on how to define further geometries. The implementation can be found in: ```/common/src/test_configs.cc```.

* Index 100: One layer of silicon.
* Index 101: EE with 14 cassettes a 2 silicon modules per cassettes. These are spaced with equal distance with respect to each other
* Index 102: One layer of silicon placed behind 3x 4mm of tungsten. In addition, an alumnium chip is placed directly in front of the silicon module PCB.

### Adding a new geometry to the simulation
A few steps need to be followed in order to add a new custom-defined geometry to the framework.

* In ```/common/src/test_configs.cc```and ```/common/include/test_configs.hh``` define a new function with the same signature as the already existing ones.
* Define the view point, i.e. the point to which the visualisation tool will set its initial focus. The beam line extends from -20m to +20m.
* Define a sequence of material placement by appending to the ```dz_map``` object. The first argument is a string identifying the instance of the corresponding logical volume. Ensure that the referred logical volume exists, otherwise the program will abort during execution. The second argument represents the spacing to the previously stacked material.
* Add a call referring to the new function in the ```void DetectorConstruction::SelectConfiguration(G4int val)``` member of the ```DetectorConstruction.cc``` found in the ```/simulation/src``` directory. Ensure that there is no double definition of indices.


## Bugs and Feature Requests
Please use the github's issue tracking or contact the main developer: thorben.quast@cern.ch
