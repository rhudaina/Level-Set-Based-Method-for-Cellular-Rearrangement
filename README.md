# Level Set-Based Method for Cellular Rearrangement [![GitHub license](https://img.shields.io/badge/license-MIT-blue.svg?style=flat-square)](https://github.com/rhudaina/Level-Set-Based-Method-for-Cellular-Rearrangement/blob/main/LICENSE)

This source code executes our proposed level set-based algorithm for realizing cellular rearrangements in various tissue morphological phenomena, e.g., total engulfment, cellular patterning in olfactory and auditory sensory tissues, cell sorting, and 3D cell internalization. 

## Installation ##

Clone the repository
 
    git clone https://github.com/rhudaina/Level-Set-Based-Method-for-Cellular-Rearrangement.git

or download zip and unzip the repository into a directory of your choice.

### Dependencies ###

This source code was created on macOS Version 10.15.5 and requires the following.
* [GNU gcc compiler](https://gcc.gnu.org/install/index.html) (clang-1200.0.32.21)
* [FFTW](http://www.fftw.org/download.html) 3.3.9
* [MATLAB](https://www.mathworks.com/products/get-matlab.html) R2020b

These dependencies can also be installed on a Windows-based and Unix-based device. Typical install time on a "normal" desktop computer should be around 5-10 minutes.

## Getting Started ##

Open a terminal window, access subfolder `/code`, and execute the following command.
```
bash run.sh
```
The expected output should be a movie file saved explicitly to subfolder `/results` with filename `simulation.mp4`.
If this does not exist, run the following script within MATLAB.
```
generate_result.m
```
 That's it.

## Setup ##

To setup a simulation, access subfolder `/data`, and edit `parameters.txt` and `initconfig.txt` accordingly.

### Parameters ###

* `$Dimension` : spatial dimension of a given cell aggregate, that is, either `2`- or `3`-dimensional 
* `$NodesByRow` : number of nodes per row in domain discretization
* `$NodesByCol` : number of nodes per column in domain discretization
* `$NumberStages` : number of stages at which sigmas (weights at cell-cell junctions) changes 
* `$NbStepsPerStage` : number of time steps per stage
* `$TimeStepSize` : time step size
* `$NumberCells` : number of cells in a given cell aggregate
* `$NumberTypes` : number of cell types in a given cell aggregate
* `$NbCellsPerType` : a row array containing the number of cells for each cell type
* `$SigmaPerStage` : a 2D array where each row indicates sigma values for each type of cell-cell junction (e.g., for a configuration of two cell types, say Blue and Orange, we have BB BO OO). The first row indicates the initial sigma values, and the succeeding rows the final sigma values at each stage. A configuration with 2 cell types and 3 stages, would result in a 4x3-array.
* `$Fluctuation` : range of fluctuation in sigmas (weights at cell-cell junctions)
* `$ErrorTolerance` : error tolerance in auction dynamics
* `$EpsilonScalingFactor` : epsilon scaling factor in auction dynamics
* `$InitialEpsilonValue` : initial epsilon value in auction dynamics
* `$NumberLocalNodes` : number of nodes defining discrete radius of a square neighborhood about each node
* `$CellLabel` : a string where each character labels each cell type (e.g., for a configuration of two cell types, say Blue and Orange, we have BO)
* `$DisplayType` : use `adhsion` if sigmas are taken as reciprocals of cell-cell adhesion values; otherwise, use `tension` to display sigma values
* `$Color` : a row array composed of 3-element row blocks corresponding to RGB colors (0-255) in a 0 to 1 scale (e.g., for a configuration of two cell types, say Blue and Orange, we have 0 0.4471 0.6980 0.9020 0.6235 0)

### Initial Configuration ###

* `$TypeOfCells` : a 2D array where each row indicates the cell reference number followed by the reference number of its type
* `$NodeCellRefNum` : a column array containing cell reference number of each node

For example, consider a cell aggregate with 9x9 resolution below. Here, we label 81 nodes from 0 to 80, 4 cells from 0 to 3 with 2 cell types (Blue=0, Orange=1).

<img src="https://github.com/rhudaina/Level-Set-Based-Method-for-Cellular-Rearrangement/blob/main/nodereference.png" width="700">

### Available simulation data ###
To reproduce a simulation from our manucript, unzip `SimulationData.zip`, then copy `parameters.txt` and `initconfig.txt` from the corresponding subfolder below.
* `Engulfment` : total engulfment of a blue cell mass by an orange cell mass
* `OlfactoryE14-WT` : simulation of cellular rearrangements of WT olfactory epithelium using experimentally measured β-catenin intensities
* `OlfactoryE14-KO` : simulation of cellular rearrangements of αN-catenin KO olfactory epithelium using experimentally measured β-catenin intensities
* `OlfactoryP1-WT` : simulation of a developing olfactory epithelium using hypothetical values of interfacial tensions at postnatal stage
* `AuditoryE14-WT` : simulation of cellular rearrangements of WT auditory epithelium in embryonic stage
* `AuditoryE14-KO` : simulation of cellular rearrangements of nectin-3 KO auditory epithelium in embryonic stage
* `sup_Wetting` : wetting phenomenon resolved in the numerical implementation of the proposed level set-based method
* `sup_Nucleation` : nucleation phenomenon resolved in the numerical implementation of the proposed level set-based method
* `sup_PartialMixing` : simulation of partial mixing
* `sup_PartialSorting` : simulation of partial sorting
* `sup_StrongSorting` : simulation of strong sorting
* `sup_Segregated` : segregated pattern resulting from an aggregate consisting of blue cells expressing nection-2 and N-cadherin and orange cells expressing nectin-2, N-cadherin, and E-cadherin
* `sup_Checkerboard` : checkerboard pattern resulting from an aggregate consisting of blue cells expressing nection-2 and N-cadherin and orange cells expressing nectin-3 and N-cadherin
* `sup_Football` : football (kagome) pattern resulting from an aggregate consisting of blue cells expressing nection-2 and N-cadherin and orange cells expressing nectin-3, N-cadherin, and E-cadherin
* `sup_AuditoryE14-WT` : simulation of developing WT auditory epithelium starting from a biased initial state with no fluctuation
* `sup_AuditoryE14-WTosc` : simulation of developing WT auditory epithelium starting from a biased initial state with added tension fluctuations of size up to 10%
* `sup_AuditoryE14-KOosc` : simulation of developing nectin-3 KO auditory epithelium starting from a biased initial state with added tension fluctuations of size up to 10%
* `sup_Internalization` : three-dimensional cell internalization from an initial cell doublet configuration

## Run time for demo dataset ##

On a "normal" desktop computer, the expected run time for demo dataset found in subfolder `/data` is around 18-20 seconds to generate the solution set, and around 51-55 seconds to generate the resulting movie file.

## Copyright ##

Copyright (c) 2021 Rhudaina Mohammad, Hideki Murakawa, Karel Svadlenka, and Hideru Togashi


### License ###

MIT License. See the LICENSE file for details

## Reference ##

"A numerical algorithm for modeling cellular rearrangements in tissue morphogenesis" by Rhudaina Mohammad, Hideki Murakawa, Karel Svadlenka, and Hideru Togashi to appear in Communications Biology. (Preprint available [doi: 10.21203/rs.3.rs-43983/v2](https://www.researchsquare.com/article/rs-43983/v2))
