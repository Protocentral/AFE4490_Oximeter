AF4400 Oximeter SHIELD 
================================

[![Oximeter](https://www.protocentral.com/3130-tm_thickbox_default/afe4400-pulse-oximeter-shield-kit-for-arduino.jpg)  
*AFE4400 Pulse Oximeter Shield Kit for Arduino (PC-41234)*](https://www.protocentral.com/biomedical/861-afe4400-pulse-oximeter-shield-kit-for-arduino-642078949425.html)

This pulse oximetry shield from ProtoCentral uses the AFE4400 IC to enble your Arduino to measure heart rate as well as SpO2 values.
Pulse oximetry is an indirect method of measuring the oxygen levels in the blood. The sensor measures the skin's absorbance of red and IR light wavelengths to calculate the oxygen levels. The measurement is done by a probe that clips on to a finger and contains emitters as well as a light sensor.
Since the amount of blood flowing through any blood vessel varies (pulses) with the rate of blood from the heart, this can also be used for measuring heartrate without the need for connecting any ECG electrodes. 
Used along with Brainbay, this shield can display the real-time PPG as well as heart-rate values in addition to SpO2.

Features:
* TI AFE4400 Single chip pulse pulsoximetry front-end IC
* Standard Nellcor compatible DB7 connector for probe
* Calculates Spo2 values with provided code
* Real-time display of PPG (Photoplethysmogram)

Includes:
----------
* 1x ProtoCentral Pulse Oximetry shield for Arduino
* 1x Set of stackable Arduino headers
* 1x "Nellcor compatible" Pulse oximetry finger probe. 

Repository Contents
-------------------
* **/Libraries** - Arduino library and example sketches.
* **/Hardware** - All Eagle design files (.brd, .sch)
* **/extras** - includes the datasheet
* **/Brainbay** - setup  and configuration file
 
Using Brainbay
----------------
 Brainbay is an open-source application originally desgined for bio-feedback applications, it can be easily used for          visualizing the ECG in real-time. Brainbay can be downloaded from the following link:
 https://github.com/Protocentral/AFE4400_Oximeter/tree/master/Brainbay_setup_and_configfiles
 You can use the Windows installer and follow the instructions on the screen to install Brainbay on your computer
 Brainbay now has to be configured to work for our data format and that can be achieved by the using the configuration file   https://github.com/Protocentral/AFE4400_Oximeter/tree/master/Brainbay_setup_and_configfiles 
 In Brainbay, use Design>Load Design to load the configuration file that you downloaded in the previous link.
 Right click the EEG block in brainbay, and select the right COM port and baud rate 57600, press connect and then press play  (F7). In Brainbay, now the following are displayed

![Oximeter](https://www.protocentral.com/img/cms/ads1292r_shield/afe4400brainbayy.png)  

 
Oxygen Saturation level displayed in the Serial Terminal

![OximeterSerialOutput](https://www.protocentral.com/img/cms/ads1292r_shield/oximeter_serial_1.png)


License Information
-------------------
This product is open source!

Please use, reuse, and modify these files as you see fit. Please maintain attribution to Protocentral and release anything derivative under the same license.
