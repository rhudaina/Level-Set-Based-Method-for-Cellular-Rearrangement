# Level Set-Based Method for Cellular Rearrangement [![GitHub license](https://img.shields.io/badge/license-MIT-blue.svg?style=flat-square)](https://github.com/rhudaina/Level-Set-Based-Method-for-Cellular-Rearrangement/blob/main/LICENSE)

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
matlab -nodisplay -r "addpath(genpath('code')); generate_result"
```
    
If the last command does not work, run the following script within MATLAB.
```
generate_result.m
```
The resulting movie is saved explicitly to subfolder `/results`. That's it.

### Parameters ###

* Date truncation: `mydata = Quandl.get('NSE/OIL', 'start_date','yyyy-mm-dd','end_date','yyyy-mm-dd');`
* Frequency Change: `mydata = Quandl.get('NSE/OIL", 'collapse','annual');` ("weekly"|"monthly"|"quarterly"|"annual")
* Transformations: `mydata: = Quandl.get('NSE/OIL','transformation','rdiff');` ("diff"|"rdiff"|"normalize"|"cumulative")
* Return only n number of rows: `mydata = Quandl.get('NSE/OIL','rows',5);`

### Initial Configuration ###




## Copyright ##

Copyright (c) 2021 Rhudaina Mohammad, Hideki Murakawa, Karel Svadlenka, and Hideru Togashi


### License ###

MIT License. See the LICENSE file for details
