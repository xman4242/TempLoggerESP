#include "temp.h"

//If something breaks, check the instances of your classes and whatnot
//TODO:
//Display programming
//Match file output in the serial monitor?
//Wrap some stuff into fancy functions, esp. stuff from the setup routine
TEMP::TEMP() : onewire(ONE_WIRE_PIN),
               dtemp(&onewire),
               spi(VSPI)
{
}

void TEMP::Setup()
{
  EEPROM.begin(1);
  numBoot = EEPROM.read(EEPROM_BOOT_COUNTER_LOCATION);
  numBoot++;
  EEPROM.write(EEPROM_BOOT_COUNTER_LOCATION, numBoot);
  EEPROM.commit();
  spi.begin(CLK_PIN, MISO_PIN, MOSI_PIN, CS_PIN);
  pinMode(CS_PIN, OUTPUT);
  dtemp.begin();

  Serial.println("Initalizing the SD card...");
  if (!SD.begin(CS_PIN, spi, 80000000))
  {
    Serial.println("SD Init Failed, try again");
  }
  else
  {
    Serial.println("SD Init Success!");
  }
  initFile();
}

void TEMP::Loop()
{
  // Get the temp sensor data
  if (_NextTempMillis < millis())
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
  //Display the current temperature, look into bmp image
}

void TEMP::makeFileName(char *buffer, int value)
{
  sprintf(buffer, "temperatures%d.csv", value);
}

void TEMP::initFile()
{
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
