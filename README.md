# Control of a XYTable
 Author: Janine Müller

## Description
A XY-Table is a construction built from two linear tracks, which are mounted perpendicular. To allow a movement in two directoins (x and y) the second track is mounted on the carriage of the first track. The x axis has a traveling distance of 2500mm and the y axis of 200mm.  
Both axis are driven by TRINAMIC motors and connected to the PC via RS232.  
This software allows to do measurements with this XYTable on scintillating fibres and fibre mats from scintillating fibres. For this a current source operated via RS232 allows to pilot UV-LEDs to excite the fibre(s).  
To get the response of the scintillating fibre (or mat) a spectrometer is read out via USB.  
It is possible to do measurements in x and y direction as well as only in x direction e.g. to determine attenuation lengths.  
Special mounting setups for the spectrometer allow as well a angular dependent measurement of the fibre response.


## Requirements to run terminal program


## Requirements  to run GUI
- QT5 (in .bashrc: export CMAKE_PREFIX_PATH=/usr/local/Qt-5.8.0/)
- QT5 uses different number format (n .bashrc: export LC_NUMERIC=en_US.UTF-8)

## How to run the control