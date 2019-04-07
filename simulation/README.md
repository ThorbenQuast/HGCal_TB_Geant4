# Standalone Simulation
![sim_config22_150GeV_pion](../img/sim_config22_150GeV_pion.png)

## Installation
* ```cd /simulation```
* ```mkdir build; cd build```
* ```cmake ..``` 
* ```make install```, warnings can be ignored.

In case of successful compilation, an executable called ```Simulation``` should be added to the ```build``` directory.

## Running the simulation
The standalone simulation can be run either using the visualisation tool like for the EventDisplay (QT5 or OpenGL required) or without. While the particle type, momentum and place of the beam is configurable, its direction is always parallel to the z-axis.

### With visualisation
The default command to initialise the simulation with visualisation is analogous to the event display: ```./Simulation```.
Alternatively, a dedicated steering file can be passed as command line argument to the program. If none is specified, ```init_vis.mac``` is taken. This file starts the visualisation tool and loads more detailed settings from ```vis.mac```. As example, the configuration is set to 22, the output file path is set to *HGCalTB\_October2018\_config22\_pi+\_150GeV.root* and the particle gun is set to generate 150 GeV pions.

### Without visualisation
In order to run the simulation without visualisation, adjust the ```run.mac``` steering file to your needs and execute it with ```./Simulation run.mac```.
In the provided example file, four physics tunes (step sizes) are simulated sequentially for 10000 200 GeV muons and 50 GeV electrons each. Eight output files are produced in total.

## Output file
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

## Commands
* ```/Simulation/setup/configuration <index>```: Load the defined geometry. Ineffective if set index is not implemented. This command can only be run per session, i.e. only one geometry can be defined instance of the program.
* ```/Simulation/setup/stepSilicon <float>``` Sets the maximum step size in the sensitive silicon (unit: micrometer). ```Default: 50 (micrometer)```.

* ```/Simulation/output/file <string>```: Define the path of the output file. ```Default: sim_HGCalTB_G4Standalone.root```.

* ```/Simulation/generator/GaussianBeam <bool>``` Use a gaussian beam profile? If not a flat profile is assumed. ```Default: false```.
* ```/Simulation/generator/sigmaBeamX(Y) <float> <unit>``` Define the width of the beam profile (sigma for gaussian, half-range for a flat profile). ```Default: 0.001cm```.
* ```/Simulation/generator/momentum <float> <unit>``` Configure the momentum of the beam particles. ```Default: 10 GeV```.
* ```/Simulation/generator/particle <type>``` Configure the type of the beam particles (examples: mu+, pi-, e+, n). ```Default: e+```.

* ```/run/beamOn <int>```: Run the simulation of the set number of particles.
