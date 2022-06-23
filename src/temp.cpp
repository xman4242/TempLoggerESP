#include "temp.h"

//If something breaks, check the instances of your classes and whatnot
//TODO:
//Display programming
//Match file output in the serial monitor?
//Wrap some stuff into fancy functions, esp. stuff from the setup routine
TEMP::TEMP() : onewire(ONE_WIRE_PIN),
               dtemp(&onewire),
               spi(VSPI),
               screen()
{
}

void TEMP::Setup()
{ 
  //Start the EEPROM and get the boot number of this cycle
  EEPROM.begin(1);
  numBoot = EEPROM.read(EEPROM_BOOT_COUNTER_LOCATION);
  numBoot++;
  EEPROM.write(EEPROM_BOOT_COUNTER_LOCATION, numBoot);
  EEPROM.commit();

  //Buttons are GPIO 0 and GPIO 35, init them here. They are active low. 
  pinMode(0,INPUT);
  pinMode(35,INPUT);
  pinMode(CS_PIN, OUTPUT);
  //Start the SPI and the oneWire library for peripheral communication
  spi.begin(CLK_PIN, MISO_PIN, MOSI_PIN, CS_PIN);
  dtemp.begin();
  //Start the screen
  screen.init();
  screen.setRotation(1);
  screen.setTextSize(2);
  screen.fillScreen(TFT_BLACK);
  screen.setTextColor(TFT_RED, TFT_BLUE,true);
  screen.drawString("Press the button\nNext to the switch\nTo start ", 0, 0, 2); 
}

void TEMP::Loop()
{ 
  //Use button 35 to start, button 0 to stop. Button 35 is by the power switch
  buttonState = digitalRead(35);
  offButtonState = digitalRead(0);
  
  if((!prevButtonState && buttonState) || isRunning)
  { 
    if(!isRunning) 
    { 
      screen.drawString("Generating File\n......",0,0,2);
      initFile();
      
      isRunning = true;
    }

    // Get the temp sensor data
    if (_NextTempMillis <= millis())
    {
      _NextTempMillis += READ_WAIT_MS;
      for (int i = 0; i < numberOfSensors; i++)
      {
        temperatureVal[i] = dtemp.getTempFByIndex(i);
      }

      // put it in the csv file
      tempFile.printf("%ld,", long(_NextTempMillis - READ_WAIT_MS));
      for (int i = 0; i < numberOfSensors; i++)
      { 
        //We need to make sure we don't have extra commas
        if(i-1 == numberOfSensors) tempFile.printf("%lf", temperatureVal[i]);
        else tempFile.printf("%lf,", temperatureVal[i]);
      }
      tempFile.printf("\n");
      
    }
  }

  if(!prevOffButtonState && buttonState && isRunning) endLogging(100,10);
  prevButtonState = buttonState;
  prevOffButtonState = offButtonState;
}

void TEMP::displayLoop()
{
  if(_NextDisplayMillis < millis())
  { 
    _NextDisplayMillis += 200;
    for (int i = 0; i < numberOfSensors; i++)
    { 
      sprintf(buffer,"Temp %d is: %.2lf",i,temperatureVal[i]);
      String draw(buffer); 
      screen.drawString(draw,0,0,2);
      delay(MS_PER_TEMP);
    }
    screen.drawBitmap(0,0,img,135,240,TFT_RED);
    delay(100);
  }
}

void TEMP::makeFileName(char *buffer, int value)
{
  sprintf(buffer, "temperatures%d.csv", value);
}

void TEMP::initFile()
{ 
  //Init the SD card and open the file for data logging
  Serial.println("Initalizing the SD card...");
  if (!SD.begin(CS_PIN, spi, 80000000)) Serial.println("SD Init Failed, try again");
  else Serial.println("SD Init Success!");

  // Only one file can be open at a time, and make sure we don't do a bad thing
  Serial.println("Generating file header....");
  makeFileName(fileString, numBoot);

  tempFile = SD.open(fileString, FILE_WRITE);
  tempFile.printf("Millis Since Start,");

  for (int i = 0; i < numberOfSensors; i++)
  {
    if(i-1 == numberOfSensors) tempFile.printf("Probe %d",i);
    else tempFile.printf("Probe %d",i);
  }
  tempFile.printf("\n (ms),");

  for (int i = 0; i < numberOfSensors; i++)
  {
    if(i-1 == numberOfSensors) tempFile.printf("(Deg F)");
    else tempFile.printf("(Deg F),");
  }
  tempFile.printf("\n");
  sprintf(buffer,"File %s has been made, starting the recording...",fileString);
  Serial.println(buffer);
}

void TEMP::endLogging(int flashMS, int numFlash)
{
  Serial.println("Logging Finished");
  Serial.println("Closing file");
  tempFile.close();
  Serial.println("Shutting Down");
  for (int i = 0; i < numFlash; i++)
  {
    screen.fillScreen(TFT_RED);
    delay(flashMS);
    screen.setTextColor(TFT_WHITE, TFT_RED,true);
    screen.drawString("SHUTTING\nDOWN",0,0,2);
    delay(flashMS);
  }
  ESP.restart();
}