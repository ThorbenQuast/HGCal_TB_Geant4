# Event Display
## Installation
* ```cd /simulation```
* ```mkdir build; cd build```
* ```cmake ..``` 
* ```make install```, warnings can be ignored.

In case of successful compilation, an executable called ```EventDisplay``` should be added to the ```build``` directory.

## Running the display
The default command to initialise the display executed from the ```build``` directory is simply: ```./EventDisplay```.
Alternatively, a dedicated steering file can be passed as command line argument to the program. If none is specified, ```init_vis.mac``` is taken. This file starts the visualisation tool and loads more detailed settings from ```vis.mac```. As example, the configuration is set to 22, an energy threshold is defined and both reconstructed data from HGCal and AHCAL are linked to the program in this file.

## Commands
* ```/EventDisplay/setup/configuration <index>```: Load the defined geometry. Ineffective if set index is not implemented. This command can only be run per session, i.e. only one geometry can be defined instance of the program.
 
* ```/EventDisplay/ntuple/path <string>```: Path to the reconstructed HGC ntuple files. These ntuples must be reconstructed with the October2018_v12 software tag, or newer. 
* ```/EventDisplay/ntuple/pathAHCAL <string>```: Path to the reconstructed AHCAL ntuple files. v2 reconstruction of the data accumulated in October 2018 is supported.
* ```/EventDisplay/ntuple/pathAHCAL <string>```: Path to the reconstructed AHCAL ntuple files. v2 reconstruction of the data accumulated in October 2018 is supported.
* ```/EventDisplay/ntuple/AHCALOffset <int>```: Applies an offset to the event number in the AHCAL ntuples. 
* ```/EventDisplay/ntuple/energyThreshold <float>```: Only deposits above this energy threshold are visualised (Unit: MIPs).
* ```/EventDisplay/ntuple/timeCut <float>```: Only deposits with time of arrival below this value are visualised (Unit: nanoseconds). For any positive timeCut, only HGCal hits are displayed.
* ```/EventDisplay/ntuple/showEvent <int>```: Reads the specified event number and visualises the hits.

### Color schemes
The event display makes use of a hard coded version of the ROOT rainbow color scheme for colorisation of calorimeter hits. The energy scale ranges from 0 to 500 MIPs. Modifications can be implemented in ```/simulation/include/colors.hh```.
