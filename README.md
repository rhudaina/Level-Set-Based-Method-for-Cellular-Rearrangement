# Level Set-Based Method for Cellular Rearrangement [![GitHub license](https://img.shields.io/badge/license-MIT-blue.svg?style=flat-square)](https://github.com/rhudaina/Level-Set-Based-Method-for-Cellular-Rearrangement/blob/main/LICENSE)

## Installation ##

Clone the repository
 
    git clone https://github.com/rhudaina/Level-Set-Based-Method-for-Cellular-Rearrangement.git

or download zip and unzip the repository into a directory of your choice.

### Dependencies ###

This code is created with:
* [GNU gcc compiler](https://gcc.gnu.org/install/index.html)
* [FFTW](http://www.fftw.org/download.html)
* [MATLAB](https://www.mathworks.com/products/get-matlab.html)


## Getting Started ##

Open terminal and go to your directory.

    cd code
    bash run.sh
    matlab -nodisplay -r "addpath(genpath('code')); generate_result"
    
If matlab is not linked in the CLI, do the following. Within MATLAB go to code >>

    generate_result.m

That's it.

## Initial Configuration ##



## Parameters ##

* Date truncation: `mydata = Quandl.get('NSE/OIL', 'start_date','yyyy-mm-dd','end_date','yyyy-mm-dd');`
* Frequency Change: `mydata = Quandl.get('NSE/OIL", 'collapse','annual');` ("weekly"|"monthly"|"quarterly"|"annual")
* Transformations: `mydata: = Quandl.get('NSE/OIL','transformation','rdiff');` ("diff"|"rdiff"|"normalize"|"cumulative")
* Return only n number of rows: `mydata = Quandl.get('NSE/OIL','rows',5);`


## Copyright ##

Copyright (c) 2021 Rhudaina Mohammad, Hideki Murakawa, Karel Svadlenka, and Hideru Togashi


### License ###

MIT License. See the LICENSE file for details
