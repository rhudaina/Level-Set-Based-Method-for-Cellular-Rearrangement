# Level Set-Based Method for Cellular Rearrangement [![GitHub license](https://img.shields.io/badge/license-MIT-blue.svg?style=flat-square)](https://github.com/rhudaina/Level-Set-Based-Method-for-Cellular-Rearrangement/blob/main/LICENSE)

This source code executes our proposed level set-based algorithm for realizing cellular rearrangements in various tissue morphological phenomena, e.g., total engulfment, cellular patterning in olfactory and auditory sensory tissues, cell sorting, and 3D cell internalization.

## Installation ##

Clone the repository
 
    git clone https://github.com/rhudaina/Level-Set-Based-Method-for-Cellular-Rearrangement.git

or download zip and unzip the repository into a directory of your choice.

### Dependencies ###

This source code was created with and requires the following.
* [GNU gcc compiler](https://gcc.gnu.org/install/index.html)
* [FFTW](http://www.fftw.org/download.html)
* [MATLAB](https://www.mathworks.com/products/get-matlab.html)


## Getting Started ##

Open a terminal window, access subfolder `/code`, and execute the following commands.
```
bash run.sh
```
The resulting movie `simulation.mp4` should be saved explicitly to subfolder `/results`.
If this does not exist, run the following script within MATLAB.
```
generate_result.m
```
 That's it.

## Setup ##

To setup a simulation, access subfolder `/data`, and edit `parameters.txt` and `initconfig.txt` accordingly.

### Parameters ###

* `$Dimension` : number of dimensions to specify the location of cell-cell junctions
* `$NodesByRow` : number of nodes per row in domain discretization
* `$NodesByCol` : number of nodes per column in domain discretization
* `$NumberStages` : number of stages at which interfacial tensions (sigmas) changes 
* `$NbStepsPerStage` : number of time steps per stage
* `$TimeStepSize` : time step size
* `$NumberCells` : number of cells in a given cell aggregate
* `$NumberTypes` : number of cell types in a given cell aggregate
* `$NbCellsPerType` : a row array containing the number of cells for each cell type
* `$SigmaPerStage` : a 2D array where each row indicates sigma values for each type of cell-cell junction (e.g., for a configuration of two cell types, we have BB BR RR). The first row indicates the initial sigma values, and the succeeding rows the final sigma values at each stage. A configuration with 2 cell types and 3 stages, would result in a 4x3-array.
* `$Fluctuation` : 
* `$ErrorTolerance` : 
* `$EpsilonScalingFactor` :
* `$InitialEpsilonValue` :
* `$NumberLocalNodes` :
* `$CellLabel` :
* `$DisplayType` :
* `$Color` : 

### Initial Configuration ###

* `$TypeOfCells` : a 
* `$NodeCellRefNum` : a column array containing cell reference number of each node

## Copyright ##

Copyright (c) 2021 Rhudaina Mohammad, Hideki Murakawa, Karel Svadlenka, and Hideru Togashi


### License ###

MIT License. See the LICENSE file for details
