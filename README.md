AF4490 Oximeter SHIELD 
================================
[![Oximeter](https://www.protocentral.com/3782-thickbox_default/afe4490-pulse-oximeter-shield-kit-for-arduino.jpg)  
*AFE4490 Pulse Oximeter Shield Kit for Arduino (PC-41234)*](https://www.protocentral.com/biomedical/996-afe4490-pulse-oximeter-shield-kit-for-arduino-642078949425.html)

This pulse oximetry shield from ProtoCentral uses the AFE4490 IC to enble your Arduino to measure heart rate as well as SpO2 values.
Pulse oximetry is an indirect method of measuring the oxygen levels in the blood. The sensor measures the skin's absorbance of red and IR light wavelengths to calculate the oxygen levels. The measurement is done by a probe that clips on to a finger and contains emitters as well as a light sensor.
Since the amount of blood flowing through any blood vessel varies (pulses) with the rate of blood from the heart, this can also be used for measuring heartrate without the need for connecting any ECG electrodes. 
Used along with Brainbay, this shield can display the real-time PPG as well as heart-rate values in addition to SpO2.

Features:
* TI AFE4490 Single chip pulse pulsoximetry front-end IC
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
 
Using Processing - Data Visualization Software (New!)
-----------------------------------------------------
 Processing is a data visualization software, in existence since 2001, used by artists and scientists alike. It’s an open source coding framework based on Java. If you are familiar with the Arduino environment, the Processing IDE is similar and you won’t have much of a learning curve to climb!
 
 The following are the steps to run the code:

### 1. Download Processing for your operating system

 Download the processing ide latest version from the link

* [Download Processing for your OS] (http://download.processing.org/)

Once downloaded, unzip the archive and install the app as per your OS.

### 2. Download the Processing code for Pulse Oximeter visualization

 a. Download the necessary files & directories or clone to your desktop from GitHub.

 b. Unzipping the archive should make a folder by name AFE4490 Oximeter Shield that contains the visualization code.

 c. Locate the Processing sketchbook directory on your computer. This should have been created automatically when you installed processing. Depending on your operating system, the path will be as follows:

* On Windows: c:/My Documents/Processing/
* On MAC: /Users/your_user_name/Documents/Processing/
* On Linux: /Home/your_user_name/sketchbook/

**Note:** This directory appears as "Processing" on Windows/Mac, and goes by the name "Sketchbook" on Linux. Create a subdirectory by name "libraries" if one doesn't exist already.

 d. From the above mentioned "AFE4490_Oximeter-master" directory Copy/Move the contents of the AFE4490_Oximeter-master/Processing/ces_view_oximeter folder to the Processing sketchbook directory which is also mentioned above (Locate the Processing sketchbook)

 e. Finally, copy the G4P directories from AFE4490_Oximeter-master\Processing\libraries and paste them into the libraries directory of your Processing sketchbook.

 f. You are all set now to get your first PPG wave form and SpO2 reading visualized from the AFE4490 Oximeter Shield!

### 3. Uploading Arduino Sketch For AFE4490 Oximeter Shield

 a. Open the Arduino IDE to load the embedded code for AFE4490 Oximeter Shield.

 b. Click on File -> Open and Browse the .ino code for Arduino from AFE4490_Oximeter-master\Libraries\Processing_sketch\IR_RED and export the application to Arduino.

### 4. Open Processing & launch the ces_view_oximeter

 a. If Processing IDE was open, close it and reopen to refresh the libraries and sketches. The repositories are not refreshed if the IDE was open while the Sketches and Libraries were being updated.

 b. Double-click any of the .pde files in the ces_view_oximeter directory to open all of the pulse oximeter code in the Processing IDE.

 c. If everything done so far was good, clicking the "run" button on the top left corner of the IDE, should run the code! If it does not, make sure you installed your libraries correctly.

 d. Once the GUI is running, select the port connect with pulse oximeter shield from the "SELECT PORT" dropdown as shown in the figure below

![Port Selection](https://github.com/Protocentral/AFE4400_Oximeter/blob/master/Processing/Final%20Output/Port-Selection.png)

 e. Once the port selection is appropriate the START button gets enabled. Click "START" to initiate visualization

 f. You should see the PPG ( RED and IR) waves generated with the values obtained from the AFE4490 Oximeter Shield Breakout Board as shown below.

![PPG Wave Form in Processing](https://github.com/Protocentral/AFE4400_Oximeter/blob/master/Processing/Final%20Output/PPG-Generated.png)


License Information
-------------------
This product is open source!

Please use, reuse, and modify these files as you see fit. Please maintain attribution to Protocentral and release anything derivative under the same license.
