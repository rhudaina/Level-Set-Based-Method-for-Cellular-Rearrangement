# Level Set-Based Method for Cellular Rearrangement [![GitHub license](https://img.shields.io/badge/license-MIT-blue.svg?style=flat-square)](https://github.com/rhudaina/Level-Set-Based-Method-for-Cellular-Rearrangement/blob/main/LICENSE)


## Installation ##

Download the folder "+Quandl" into the directory of your choice. Then within MATLAB go to file >> Set path... and add the directory containing "+Quandl" to the list (if it isn't already). That's it.

Two things to note, the '+' in "+Quandl" is important in the folder name. It tells Matlab to recognize get.m and auth.m as part of the Quandl package. Secondly, make sure you don't add the "+Quandl" folder in set path. You should be adding the folder that contains it.


### Dependencies ###

* The GNU gcc compiler https://gcc.gnu.org/install/index.html
* FFTW http://www.fftw.org/download.html
* MATLAB https://www.mathworks.com/products/get-matlab.html

<!-- This package now REQUIRES MATLAB. It can be found [here](http://www.mathworks.com/matlabcentral/fileexchange/35693-urlread2). -->

<!-- Unzip the package and place it in the same directory as +Quandl in the folder +urlread2. -->


### Parameters ###

* Date truncation: `mydata = Quandl.get('NSE/OIL', 'start_date','yyyy-mm-dd','end_date','yyyy-mm-dd');`
* Frequency Change: `mydata = Quandl.get('NSE/OIL", 'collapse','annual');` ("weekly"|"monthly"|"quarterly"|"annual")
* Transformations: `mydata: = Quandl.get('NSE/OIL','transformation','rdiff');` ("diff"|"rdiff"|"normalize"|"cumulative")
* Return only n number of rows: `mydata = Quandl.get('NSE/OIL','rows',5);`


### Copyright ###

Copyright (c) [2021] [Rhudaina Mohammad, Hideki Murakawa, Karel Svadlenka, and Hideru Togashi]


### License ###

MIT License. See the LICENSE file for details
