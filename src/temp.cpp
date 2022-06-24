#include "temp.h"

//If something breaks, check the instances of your classes and whatnot
TEMP::TEMP() : onewire(ONE_WIRE_PIN),
               dtemp(&onewire),
               spi(VSPI),
               screen()
{
}

void TEMP::Setup()
{ 
  //Start the EEPROM and get the boot number of this cycle
  EEPROM.begin(sizeof(int));
  numBoot = EEPROM.read(EEPROM_BOOT_COUNTER_LOCATION);
  numBoot++;
  EEPROM.write(EEPROM_BOOT_COUNTER_LOCATION, numBoot);
  EEPROM.commit();

  //Buttons are GPIO 0 and GPIO 35, init them here. They are active low. 
  pinMode(0,INPUT_PULLUP);
  pinMode(35,INPUT_PULLUP);
  pinMode(2,OUTPUT);
  
  pinMode(CS_PIN, OUTPUT);
  //Start the SPI and the oneWire library for peripheral communication
  spi.begin(CLK_PIN, MISO_PIN, MOSI_PIN, CS_PIN);
  dtemp.begin();
  //Start the screen
  screen.init();
  screen.setRotation(5);
  screen.setTextSize(2);
  screen.fillScreen(TFT_BLACK);
  screen.setTextColor(TFT_GREEN);
  screen.drawString("Temp 0:",0,0,2);
  screen.drawString("Temp 1:",0,60,2);   
  sprintf(buffer,"%.2lf",temperatureVal[0]);
  String tempString0(buffer);
  sprintf(buffer,"%.2lf",temperatureVal[1]);
  String tempString1(buffer);
  screen.drawString("N/A",0,30,2);
  screen.drawString("N/A",0,90,2);
  screen.drawString("Start -->",130,0,2);
  screen.drawString("End  -->",130,100,2);
}

void TEMP::Loop()
{ 
  //Use button 35 to start, button 0 to stop. Button 35 is by the power switch
  buttonState = digitalRead(35);
  offButtonState = digitalRead(0);

  if(!prevOffButtonState && buttonState && isRunning) endLogging(100,10);
  if((!prevButtonState && buttonState) || isRunning)
  { 
    if(!isRunning) 
    {  
      screen.fillScreen(TFT_WHITE);
      screen.setTextColor(TFT_BLACK);
      screen.drawString("Generating File\n......",0,0,2);
      initFile();
      screen.setRotation(5);
      screen.fillScreen(TFT_WHITE);
      isRunning = true;
    }

    // Get the temp sensor data
    if (_NextTempMillis <= millis())
    {
      _NextTempMillis += READ_WAIT_MS;
      dtemp.requestTemperatures(); 
      for (int i = 0; i < numberOfSensors; i++)
      {
        temperatureVal[i] = dtemp.getTempFByIndex(i);
      }

      // put it in the csv file
      tempFile.printf("%ld,", long(_NextTempMillis - READ_WAIT_MS));
      for (int i = 0; i < numberOfSensors; i++)
      { 
        Serial.print("Temp ");
        Serial.print(i);
        Serial.print(" is ");
        Serial.print(temperatureVal[i]);
        Serial.println("Deg F");
        //We need to make sure we don't have extra commas
        if(i-1 == numberOfSensors) tempFile.printf("%lf", temperatureVal[i]);
        else tempFile.printf("%lf,", temperatureVal[i]);
      }
      tempFile.printf("\n");
    }
  }
  prevButtonState = buttonState;
  prevOffButtonState = offButtonState;
}

void TEMP::displayLoop()
{ 
  if(_NextDisplayMillis < millis())
  { 
    _NextDisplayMillis += 1500;
    screen.fillScreen(TFT_BLACK);
    screen.setTextColor(TFT_RED);
    screen.drawString("Temp 0:",0,0,2);
    screen.drawString("Temp 1:",0,60,2);   
    sprintf(buffer,"%.2lf",temperatureVal[0]);
    String tempString0(buffer);
    sprintf(buffer,"%.2lf",temperatureVal[1]);
    String tempString1(buffer);
    screen.drawString(tempString0,0,30,2);
    screen.drawString(tempString1,0,90,2);
    screen.drawBitmap(120,0,img,120,135,TFT_RED);
    
  }
}

void TEMP::makeFileName(char *buffer, int *value)
{
  sprintf(buffer, "temperatures%d.csv", *value);
}

void TEMP::initFile()
{ 
  //Init the SD card and open the file for data logging
  Serial.println("Initalizing the SD card...");
  if (!SD.begin(CS_PIN, spi, 80000000)) Serial.println("SD Init Failed, try again");
  else Serial.println("SD Init Success!");

  // Only one file can be open at a time, and make sure we don't do a bad thing
  Serial.println("Generating file header....");
  makeFileName(fileString, &numBoot);

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
  shuttingDown = true;
  Serial.println("Logging Finished");
  Serial.println("Closing file");
  tempFile.close();
  Serial.println("Shutting Down"); 
  screen.fillScreen(TFT_BLUE);
  screen.setTextColor(TFT_WHITE);
  screen.drawString("Shutting",0,0,2);
  screen.drawString("Down...",0,30,2);
  screen.fillScreen(TFT_BLUE);
  ESP.restart();
}