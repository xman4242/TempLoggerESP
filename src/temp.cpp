#include "temp.h"

TEMP::TEMP() : onewire(ONE_WIRE_PIN),
               dtemp(&onewire),
               spi(HSPI),
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
  pinMode(CARD_DETECT_PIN,INPUT_PULLUP);

  //Start the SPI and the oneWire library for peripheral communication
  spi.begin(CLK_PIN, MISO_PIN, MOSI_PIN, CS_PIN);
  dtemp.begin();

  //Start the screen
  screen.init();
  screen.setRotation(5);
  screen.setTextSize(2);
  if(!digitalRead(0) && !digitalRead(35)) resetBootCounter();
  flashSDWarning();
  mainScreen();
}

void TEMP::Loop()
{ 
  buttonState = digitalRead(35);
  offButtonState = digitalRead(0);

  if(!sdCardIsInserted())
  {
    flashSDWarning();
    mainScreen();
  }

  if(!prevOffButtonState && buttonState && isRunning) endLogging(500);

  if((!prevButtonState && buttonState) || isRunning)
  { 
    if(!isRunning) 
    {  
      initFile();
      screen.fillScreen(TFT_BLACK);
      screen.setTextColor(TFT_GREEN);
      screen.drawString("Generating",0,0,2);
      sprintf(buffer,"/temp%d",numBoot);
      screen.drawString (buffer,0,30,2);
      screen.drawBitmap(120,0,csvImg,120,135,TFT_GREEN);
      delay(1000);
      isRunning = true;
    }

    thermometerLoop();

    // Get the temp sensor data
    if (_NextTempMillis <= millis())
    {
      _NextTempMillis += READ_WAIT_MS;
      dtemp.requestTemperatures(); 
      for (int i = 0; i < numberOfSensors; i++)
      {
        temperatureVal[i] = dtemp.getTempFByIndex(i);
      }

      tempFile.printf("%ld,", long(_NextTempMillis - READ_WAIT_MS));
      for (int i = 0; i < numberOfSensors; i++)
      { 
        if(i == numberOfSensors-1) tempFile.printf("%lf", temperatureVal[i]);
        else tempFile.printf("%lf,", temperatureVal[i]);
      }
      tempFile.printf("\n");
    }
  }
  prevButtonState = buttonState;
  prevOffButtonState = offButtonState;
}

void TEMP::thermometerLoop()
{   
  if(_NextDisplayMillis < millis())
  {
    _NextDisplayMillis = millis() + 1000;
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
  sprintf(buffer, "/temp%d.csv", *value);
}

void TEMP::initFile()
{ 
  //Init the SD card and open the file for data logging
  while(!SD.begin(CS_PIN, spi))
  {  
    screen.fillScreen(TFT_BLACK);
    screen.setTextColor(TFT_YELLOW);
    screen.drawString("SD FAILED",0,0,2); 
    screen.drawString ("Retrying...",0,30,2);
    screen.drawBitmap(120,0,sdImg,120,135,TFT_YELLOW);
    delay(100); 
  }
  //if (!SD.begin(CS_PIN, spi,80000000)) Serial.println("SD Init Failed, try again");
  //else Serial.println("SD Init Success!");
  
  // Only one file can be open at a time, and make sure we don't do a bad thing
  Serial.println("Generating file header....");
  makeFileName(fileString, &numBoot);

  tempFile = SD.open(fileString, FILE_WRITE);
  tempFile.printf("Millis Since Start,");

  for (int i = 0; i < numberOfSensors; i++)
  {
    if(i == numberOfSensors-1) tempFile.printf("Probe %d",i);
    else tempFile.printf("Probe %d,",i);
  }
  tempFile.printf("\n(ms),");

  for (int i = 0; i < numberOfSensors; i++)
  {
    if(i == numberOfSensors-1) tempFile.printf("(Deg F)");
    else tempFile.printf("(Deg F),");
  }
  tempFile.printf("\n");
  sprintf(buffer,"File %s has been made, starting the recording...",fileString);
  Serial.println(buffer);
}

void TEMP::endLogging(int delayMs)
{ 
  shuttingDown = true;
  Serial.println("Logging Finished");
  Serial.println("Closing file");
  tempFile.close();
  Serial.println("Shutting Down"); 
  screen.fillScreen(TFT_BLACK);
  screen.setTextColor(TFT_PURPLE);
  screen.drawString("Saving",0,0,2);
  screen.drawString ("File.....",0,30,2);
  screen.drawBitmap(120,0,floppyImg,120,135,TFT_PURPLE);
  delay(delayMs);
  ESP.restart();
}

void TEMP::resetBootCounter()
{
  numBoot = 0;
  EEPROM.write(EEPROM_BOOT_COUNTER_LOCATION, numBoot);
  EEPROM.commit();
  Serial.println("Boot Counter Reset!");
  screen.fillScreen(TFT_BLACK);
  screen.setTextColor(TFT_DARKCYAN);
  screen.drawString("Boot Counter Reset!",0,0,2);
  delay(1500);
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

bool TEMP::sdCardIsInserted()
{
  return digitalRead(CARD_DETECT_PIN);
}

void TEMP::flashSDWarning()
{
  while(!sdCardIsInserted())
  {
    screen.fillScreen(TFT_BLACK);
    delay(100);
    screen.setTextColor(TFT_RED);
    screen.drawString("INSERT",0,0,2);
    screen.drawString ("SD CARD",0,30,2);
    screen.drawBitmap(120,0,warningImg,120,135,TFT_RED);
    delay(100);
  }
}

void TEMP::mainScreen()
{
    screen.fillScreen(TFT_BLACK);
    screen.setTextColor(TFT_DARKCYAN);
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
