#include "temp.h"

TEMP::TEMP() : onewire(ONE_WIRE_PIN),
               dtemp(&onewire),
               spi(HSPI),
               screen(),
               gpsSerial(2)
               
{
}
//TODO: Make the device save in between recordings. 
void TEMP::Setup()
{  
  
  //Start the EEPROM and get the boot number of this cycle
  EEPROM.begin(sizeof(int));
  numBoot = EEPROM.read(EEPROM_BOOT_COUNTER_LOCATION);
  numBoot++;
  if(numBoot > 99) numBoot = 0;
  EEPROM.write(EEPROM_BOOT_COUNTER_LOCATION, numBoot);
  utcOffset = EEPROM.read(EEPROM_TIME_OFFSET_LOCATION);
  EEPROM.commit();
  //GPS Serial Start, we use hardware serial because its faster
  gpsSerial.begin(9600, SERIAL_8N1, GPS_TX, GPS_RX);
  //Buttons are GPIO 0 and GPIO 35, init them here. They are active low. 
  pinMode(0,INPUT_PULLUP);
  pinMode(35,INPUT_PULLUP);
  pinMode(2,OUTPUT);
  pinMode(CS_PIN, OUTPUT);
  pinMode(CARD_DETECT_PIN,INPUT_PULLUP);

  //Start the SPI and the oneWire library for SD card and the Temp Sensors
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
  //Here is all the code for the setup menus
  if((prevOffButtonState && !offButtonState && !isRunning) || inSetup)
  { 
    if(firstSetup)
    { 
      firstSetup = false;
      inSetup = true;
      screen.fillScreen(TFT_BLACK);
      screen.setTextColor(TFT_YELLOW);
      sprintf(buffer,"UTC Offset is: %2i",utcOffset);
      screen.drawString(buffer,0,0,2);
      screen.drawString("Up to increment",0,30,2);
      screen.drawString("Down to Save",0,60,2);
      if(hour() == 0) screen.drawString("No GPS Signal",0,90,2);
      else
      {
        screen.drawString(formattedHour() + ":" + formattedMin(),0,90,2);
      }
      utcOffset--;
    }
    
    if(prevOffButtonState && !offButtonState)
    { 
      if(utcOffset >= 24) utcOffset = 0;
      else 
      {
        utcOffset++;
      }
      screen.fillScreen(TFT_BLACK);
      screen.setTextColor(TFT_YELLOW);
      sprintf(buffer,"UTC Offset is: %i",utcOffset);
      screen.drawString(buffer,0,0,2);
      screen.drawString("Down to increment",0,30,2);
      screen.drawString("Up to Save",0,60,2);
      if(hour() == 0) screen.drawString("No GPS Signal",0,90,2);
      else
      {
        screen.drawString(formattedHour() + ":" + formattedMin(),0,90,2);
      }
      delay(100);
    }
    if(prevButtonState && !buttonState)
    { 
      EEPROM.write(EEPROM_TIME_OFFSET_LOCATION, utcOffset);
      EEPROM.commit();
      inSetup = false;
      ESP.restart();
    }
  } 
  //If the bottom button is pressed while we are logging, we stop logging
  if(!prevOffButtonState && buttonState && isRunning) endLogging(500);

  //The default screen that displays the temp while logging
  if((!prevButtonState && buttonState && !inSetup) || isRunning)
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

    // Get the temp sensor data
    if (_NextTempMillis <= millis())
    {
      _NextTempMillis += READ_WAIT_MS;
      dtemp.requestTemperatures(); 
      for (int i = 0; i < numberOfSensors; i++)
      {
        temperatureVal[i] = dtemp.getTempFByIndex(i);
      }

      tempFile.printf("%ld,", long((_NextTempMillis - READ_WAIT_MS)/1000));
      for (int i = 0; i < numberOfSensors; i++)
      { 
        if(i == numberOfSensors-1) tempFile.printf("%lf", temperatureVal[i]);
        else tempFile.printf("%lf,", temperatureVal[i]);
      }
    
      thermometerLoop();
      tempFile.printf(",%s:%s:%s",formattedHour(),formattedMin(),formattedSec());
      tempFile.printf(",%i/%i/%i",month(),day(),year());
      tempFile.printf(",%f",flat);
      tempFile.printf(",%f",flon);
      tempFile.printf("\n");
      //numFileLines++;
      //Serial.println(numFileLines);
      //Probably save file here 
      tempFile.close();
      tempFile = SD.open(fileString, FILE_APPEND);
      
    }
  }
  prevButtonState = buttonState;
  prevOffButtonState = offButtonState;
}

//This is the display code for the thermometer image and temp
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
    screen.drawBitmap(120,0,thermometer,120,115,TFT_RED);
    screen.drawString("Stop -->",125,100,2);
  }
}

//Here is where we retrive and process the GPS data
void TEMP::gpsLoop()
{ 
  bool newData = false;

  for (unsigned long start = millis(); millis() - start < 1000;)
  {
    while (gpsSerial.available())
    {
      char c = gpsSerial.read();
      if (gps.encode(c)) 
        newData = true;
    }
  }
  
  if (newData)
  { 
    Serial.println("New Data Recived");

    gps.crack_datetime(&Year, &Month, &Day, &Hour, &Minute, &Second, &Hundredths, &age);
    if (age < 500) 
    {
      // set the Time to the latest GPS reading
      setTime(Hour, Minute, Second, Day, Month, Year);
      adjustTime(-1 * (utcOffset*SECS_PER_HOUR));
    }

    if (age == TinyGPS::GPS_INVALID_AGE) Serial.print("Invalid Age!");
    gps.f_get_position(&flat, &flon, &age);
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

  // Only one file can be open at a time, and make sure we don't do a bad thing
  Serial.println("Generating file header....");
  makeFileName(fileString, &numBoot);

  tempFile = SD.open(fileString, FILE_WRITE);
  tempFile.printf("Milliseconds Since Start,");

  for (int i = 0; i < numberOfSensors; i++)
  {
    if(i == numberOfSensors-1) tempFile.printf("Probe %d",i);
    else tempFile.printf("Probe %d,",i);
  }
  tempFile.printf(",GPS Time");
  tempFile.printf(",GPS Date");
  tempFile.printf(",GPS LAT");
  tempFile.printf(",GPS LONG");
  tempFile.printf("\n(ms),");

  for (int i = 0; i < numberOfSensors; i++)
  {
    if(i == numberOfSensors-1) tempFile.printf("(Deg F)");
    else tempFile.printf("(Deg F),");
  }
  tempFile.printf(",HH:mm:ss");
  tempFile.printf(",MM/DD/YYYY");
  tempFile.printf(",%c",176);
  tempFile.printf(",%c",176);
  tempFile.printf("\n");
  sprintf(buffer,"File %s has been made, starting the recording...",fileString);
  Serial.println(buffer);
}

void TEMP::endLogging(int delayMs)
{ 
  shuttingDown = true;
  screen.fillScreen(TFT_BLACK);
  screen.setTextColor(TFT_PURPLE);
  screen.drawString("Saving",0,0,2);
  screen.drawString ("File.....",0,30,2);
  screen.drawBitmap(120,0,floppyImg,120,135,TFT_PURPLE);
  Serial.println("Logging Finished");
  Serial.println("Closing file");
  tempFile.close();
  Serial.println("Shutting Down"); 
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
    screen.drawString("Setup -->",125,100,2);
}

String TEMP::formattedHour()
{ 
  char buffer[30];

  if(hour() < 10)
  {
    sprintf(buffer,"0%i",hour());
  }
  else
  {
    sprintf(buffer,"%i",hour());
  }
  return String(buffer);
}

String TEMP::formattedMin()
{
  char buffer[30];

  if(minute() < 10)
  {
    sprintf(buffer,"0%i",minute());
  }
  else
  {
    sprintf(buffer,"%i",minute());
  }
  return String(buffer);
}

String TEMP::formattedSec()
{
  char buffer[30];

  if(second() < 10)
  {
    sprintf(buffer,"0%i",second());
  }
  else
  {
    sprintf(buffer,"%i",second());
  }
  return String(buffer);
}