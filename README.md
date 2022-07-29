
<h1>ESP32 Temperature Logger</h1>	

<h4>This is a project to log the temperature from 2 probes, as well as the GPS time, date, and location into a csv file. </h4>


<h2>The instructions to use the device are as follows:</h2>	

  1. Power the logger with the USB-C port built into the device. A USB power bank is also suitable. 
  2. Insert a FAT-32 formatted microSD card. A day of logging will produce a few megabytes of data at most, so size is not a big issue. 
  3. Press the "Setup" (lower) button and follow the instructions to set the time on the device. If there is a GPS connection, the current time with the calculated offset is displayed. This offset is needed because GPS devices report time in UTC, which is the time zone around longitude 0. 
  4. After setting the time, press start to begin recording. The name of the file on the SD card will be displayed on the screen.
  5. The data is recorded every 5 seconds. 
  6. To finish logging, press the "Stop" (lower) button. 
  7. After the save icon has disappeared and the main screen is visible, you may remove the SD card. 
  8. The CSV files can now be processed in excel or another suitable program. 

How to Update the Code:
  
  