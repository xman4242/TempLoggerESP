#include "temp.h"

TEMP::TEMP() : onewire(ONE_WIRE_PIN),
               dtemp(&onewire)
{
}

void TEMP::Setup()
{
  pinMode(CS_PIN, OUTPUT);
  Serial.println("Initalizing the SD card...");
  if (!SD.begin(CS_PIN))
  {
    Serial.println("SD Init Failed, try again");
  }
  else
  {
    Serial.println("SD Init Success!");
  }
  // Only onen file can be open at a time, and make sure we don't overwrite the previous files. Need to figure out how to use EEPROM to do the boot counter
  makeFileName(fileString, numBoot);
  tempFile = SD.open(fileString, FILE_WRITE);
}

void TEMP::Loop()
{
}

void TEMP::makeFileName(char* buffer, int value)
{ 
  sprintf(buffer, "temperatures%d.csv",value);
}
